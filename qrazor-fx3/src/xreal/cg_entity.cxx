/// ============================================================================
/*
Copyright (C) 1997-2001 Id Software, Inc.
Copyright (C) 2004 Robert Beckebans <trebor_7@users.sourceforge.net>
Please see the file "AUTHORS" for a list of contributors

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
/// ============================================================================


/// includes ===================================================================
// system -------------------------------------------------------------------
// xreal --------------------------------------------------------------------
#include "cg_local.h"


void	CG_BeginFrame(const frame_t &frame)
{
	if(cg.frame_running)
		trap_Com_Error(ERR_DROP, "CG_BeginFrame: running frame");
		
	cg.frame_running = true;

	cg.frame_old	= cg.frame;
	cg.frame	= frame;
}

void	CG_AddEntity(int newnum, const entity_state_t *state)
{
	cg_entity_t *cent = &cg.entities[newnum];
	
	cent->prev = cent->current = *state;
	
	if(cent->current.index_sound)
	{
		trap_S_StartLoopSound
		(
			cent->current.origin, 
			cent->current.velocity_linear, 
			cent->current.getNumber(), 
			CHAN_AUTO, 
			trap_S_RegisterSound(trap_CL_GetConfigString(CS_SOUNDS + cent->current.index_sound))
		);
	}
	
	switch(cent->prev.type)
	{
		case ET_GENERIC:
		case ET_FUNC_STATIC:
		case ET_PROJECTILE_ROCKET:
			CG_AddGenericEntity(cent);
			break;
			
		case ET_LIGHT_OMNI:
		case ET_LIGHT_PROJ:
			CG_AddLightEntity(cent);
			break;
			
		default:
			break;
	}
}

void	CG_UpdateEntity(int newnum, const entity_state_t *state, bool changed)
{
	//trap_Com_Printf("CG_UpdateEntity: %i %i is type %i\n", newnum, state.getNumber(), state.type);
	
	// update entity
	cg.entities_parse[cg.entities_first % MAX_ENTITIES] = *state;
	cg.entities_first++;
	cg.frame.entities_num++;

	// check entity
	cg_entity_t *cent = &cg.entities[newnum];
	
	// some data changes will force no lerping
	if(state->index_model != cent->current.index_model || state->event == EV_TELEPORT)
	{
		cent->serverframe = -99;
	}
	
	if(cent->serverframe != cg.frame.serverframe - 1)
	{
		// wasn't in last update, so initialize some things
		
		// duplicate the current state so lerping doesn't hurt anything
		cent->prev = *state;
	}
	else
	{	// shuffle the last state to previous
		cent->prev = cent->current;
	}

	cent->serverframe_old = cent->serverframe;
	cent->serverframe = cg.frame.serverframe;
	cent->current = *state;
}

void	CG_RemoveEntity(int newnum, const entity_state_t *state)
{
	cg_entity_t *cent = &cg.entities[newnum];
	
	if(cent->prev.index_sound)
		trap_S_StopLoopSound(cent->prev.getNumber());
	
 	cent->prev = cent->current;
 	cent->current = *state;
	
	switch(cent->prev.type)
	{
		case ET_GENERIC:
		case ET_FUNC_STATIC:
		case ET_PROJECTILE_ROCKET:
			CG_RemoveGenericEntity(cent);
			break;
	
		case ET_LIGHT_OMNI:
		case ET_LIGHT_PROJ:
			CG_RemoveLightEntity(cent);
			break;
			
		default:
			break;
	}
}


void	CG_EndFrame(int entities_num)
{
	if(!cg.frame_running)
		trap_Com_Error(ERR_DROP, "CG_EndFrame: no running frame");
		
	cg.frame_running = false;

	if(cg.frame.valid)
		return;

	//TODO
	CG_CheckEntityEvents();
	CG_CheckPredictionError();
	CG_UpdateAnimationBuffer();
}

/*
void	CG_SetFrame(r_entity_t &ent, cg_entity_t *cent)
{
	// brush models can auto animate their frames
	int autoanim = 2*trap_CL_GetTime()/1000;
	
	const uint_t& effects = cent->current.effects;
	
	if(effects & EF_ANIM01)
		ent.frame = autoanim & 1;
		
	else if(effects & EF_ANIM23)
		ent.frame = 2 + (autoanim & 1);
		
	else if(effects & EF_ANIM_ALL)
		ent.frame = autoanim;
	
	else if(effects & EF_ANIM_ALLFAST)	// should be allfast_repeat
	{
		//ent.frame = trap_CL_GetTime() / 100;
		ent.frame = (trap_CL_GetTime() / 41) % 60;	// 24 Hz
	}
	else
		ent.frame = cent->current.frame;
}
*/

/*
===============
CG_GetEntitySoundOrigin

Called to get the sound spatialization origin
===============
*/
void	CG_GetEntitySoundOrigin(int ent, vec3_c &org)
{
	cg_entity_t	*cent;

	if(ent < 0 || ent >= MAX_ENTITIES)
		trap_Com_Error(ERR_DROP, "CG_GetEntitySoundOrigin: bad ent");
	cent = &cg.entities[ent];
	
	org = cent->current.origin;

	// FIXME: bmodel issues...
}



void	CG_UpdateOrigin(const cg_entity_t *cent, r_entity_t &rent, bool &update)
{
	if(cent->prev.origin != cent->current.origin)
		update = true;
	
	rent.origin.lerp(cent->prev.origin, cent->current.origin, cg.frame_lerp);
}

void	CG_UpdateFrame(const cg_entity_t *cent, r_entity_t &rent, bool &update)
{
	const uint_t& effects = cent->current.effects;
	
	if(effects & EF_AUTOANIM_TOGGLE_01_2)
	{
		rent.frame = (2*trap_CL_GetTime()/1000) & 1;
		rent.flags |= RF_AUTOANIM;
		update = true;
	}	
	else if(effects & EF_AUTOANIM_TOGGLE_23_2)
	{
		rent.frame = 2 + ((2*trap_CL_GetTime()/1000) & 1);
		rent.flags |= RF_AUTOANIM;
		update = true;
	}	
	else if(effects & EF_AUTOANIM_1)
	{
		rent.frame = trap_CL_GetTime()/1000;
		rent.flags |= RF_AUTOANIM;
		update = true;
	}
	else if(effects & EF_AUTOANIM_2)
	{
		rent.frame = trap_CL_GetTime()/500;
		rent.flags |= RF_AUTOANIM;
		update = true;
	}
	else if(effects & EF_AUTOANIM_10)
	{
		rent.frame = trap_CL_GetTime()/100;
		rent.flags |= RF_AUTOANIM;
		update = true;
	}
	else if(effects & EF_AUTOANIM_24)
	{
		rent.frame = (trap_CL_GetTime() / 41) % 40;
		rent.frame_old = cent->prev.frame;
		rent.flags |= RF_AUTOANIM;
		update = true;
	}
	else
	{
		if(cent->prev.frame != cent->current.frame)
			update = true;
		
		rent.frame = cent->current.frame;
		rent.frame_old = cent->prev.frame;
	}
}

void	CG_UpdateRotation(const cg_entity_t *cent, r_entity_t &rent, bool &update)
{
	if((cent->current.effects & EF_ROTATE) || cent->prev.quat != cent->current.quat)
		update = true;
		
	if(cent->current.effects & EF_ROTATE)
	{	
		// some bonus items auto-rotate
		float autorotate = anglemod(trap_CL_GetTime()/10);
		vec3_c angles(0, autorotate, 0);
		rent.quat.fromAngles(angles);
	}
	else
	{
		rent.quat.slerp(cent->prev.quat, cent->current.quat, cg.frame_lerp);
	}
}

void	CG_UpdateModel(const cg_entity_t *cent, r_entity_t &rent, bool &update)
{
	if(cg.model_draw[cent->prev.index_model] != cg.model_draw[cent->current.index_model])
		update = true;
		
	rent.model = cg.model_draw[cent->current.index_model];
}

void	CG_UpdateSound(const cg_entity_t *cent)
{
	if(!cent->prev.index_sound && cent->current.index_sound)
	{
		trap_S_StartLoopSound
		(
			cent->current.origin,
			cent->current.velocity_linear,
			cent->current.getNumber(),
			CHAN_AUTO,
			trap_S_RegisterSound(trap_CL_GetConfigString(CS_SOUNDS + cent->current.index_sound))
		);
	}
	else if(cent->prev.index_sound && cent->current.index_sound)
	{
		trap_S_UpdateLoopSound(cent->current.origin, cent->current.velocity_linear, cent->current.getNumber());
	}
	else if(cent->prev.index_sound && !cent->current.index_sound)
	{
		trap_S_StopLoopSound(cent->current.getNumber());
	}
}

void	CG_UpdateAnimation(const cg_entity_t *cent, r_entity_t &rent, bool &update)
{
	if(cg.animation_precache[cent->prev.index_animation] != cg.animation_precache[cent->current.index_animation])
		update = true;
		
	rent.animation = cg.animation_precache[cent->current.index_animation];
}

void	CG_UpdateShader(const cg_entity_t *cent, r_entity_t &rent, bool &update)
{
	if(cent->current.index_shader && cg.shader_precache[cent->prev.index_shader] != cg.shader_precache[cent->current.index_shader])
		update = true;
		
	rent.custom_shader = cg.shader_precache[cent->current.index_shader];
}

void	CG_UpdateLightShader(const cg_entity_t *cent, r_entity_t &rent, bool &update)
{
	if(cg.light_precache[cent->prev.index_light] != cg.light_precache[cent->current.index_light])
		update = true;
		
	rent.custom_light = cg.light_precache[cent->current.index_light];
}

void	CG_UpdateShaderParms(const cg_entity_t *cent, r_entity_t &rent, bool &update)
{
	for(int i=0; i<8; i++)
	{
		if(cent->prev.shaderparms[i] != cent->current.shaderparms[i])
			update = true;
			
		rent.shader_parms[i] = cent->current.shaderparms[i];
	}
}

void	CG_UpdateRenderFXFlags(const cg_entity_t *cent, r_entity_t &rent, bool &update)
{
	if(cent->prev.renderfx != cent->current.renderfx)
		update = true;
	
	rent.flags = cent->current.renderfx;
}

void	CG_AddGenericEntity(const cg_entity_t *cent)
{
//	trap_Com_DPrintf("adding generic entity %i ...\n", cent->current.getNumber());
	
	r_entity_t	rent;

//	if(!cent->current.index_model)
//		trap_Com_Error(ERR_DROP, "CG_AddGenericEntity: bad client game entity model index %i\n", cent->current.index_model);
	
	rent.model = cg.model_draw[cent->current.index_model];
//	if(rent.model < 0)
//		trap_Com_Error(ERR_DROP, "CG_AddGenericEntity: bad renderer entity model index %i\n", rent.model);
		
	rent.animation = cg.animation_precache[cent->current.index_animation];
		
	if(cent->current.index_shader)
		rent.custom_shader = cg.shader_precache[cent->current.index_shader];
		
	for(int i=0; i<8; i++)
		rent.shader_parms[i] = cent->current.shaderparms[i];
	
	rent.origin = cent->current.origin;
	rent.quat = cent->current.quat;
	
	rent.frame = cent->current.frame;
	rent.frame_old = cent->current.frame;
	
	rent.flags = cent->current.renderfx;
	
	trap_R_AddEntity(cent->current.getNumber(), rent);

	if(cent->current.index_light && !cent->current.vectors[0].isZero())
	{
		rent.custom_light = cg.light_precache[cent->current.index_light];
	
		rent.radius_aabb.clear();
			
		// compute bbox vertices in light space
		vec3_c vert0( cent->current.vectors[0][0], -cent->current.vectors[0][1], -cent->current.vectors[0][2]);
		vec3_c vert1( cent->current.vectors[0][0], -cent->current.vectors[0][1],  cent->current.vectors[0][2]);
		vec3_c vert2(-cent->current.vectors[0][0], -cent->current.vectors[0][1],  cent->current.vectors[0][2]);
		vec3_c vert3(-cent->current.vectors[0][0], -cent->current.vectors[0][1], -cent->current.vectors[0][2]);
		
		vec3_c vert4( cent->current.vectors[0][0],  cent->current.vectors[0][1], -cent->current.vectors[0][2]);
		vec3_c vert5( cent->current.vectors[0][0],  cent->current.vectors[0][1],  cent->current.vectors[0][2]);
		vec3_c vert6(-cent->current.vectors[0][0],  cent->current.vectors[0][1],  cent->current.vectors[0][2]);
		vec3_c vert7(-cent->current.vectors[0][0],  cent->current.vectors[0][1], -cent->current.vectors[0][2]);
			
		// rotate vertices in light space
		vert0.rotate(cent->current.quat);
		vert1.rotate(cent->current.quat);
		vert2.rotate(cent->current.quat);
		vert3.rotate(cent->current.quat);
		vert4.rotate(cent->current.quat);
		vert5.rotate(cent->current.quat);
		vert6.rotate(cent->current.quat);
		vert7.rotate(cent->current.quat);
			
		// transform vertices into world space and add them to the light world aabb
		rent.radius_aabb.addPoint(cent->current.origin + vert0);
		rent.radius_aabb.addPoint(cent->current.origin + vert1);
		rent.radius_aabb.addPoint(cent->current.origin + vert2);
		rent.radius_aabb.addPoint(cent->current.origin + vert3);
		rent.radius_aabb.addPoint(cent->current.origin + vert4);
		rent.radius_aabb.addPoint(cent->current.origin + vert5);
		rent.radius_aabb.addPoint(cent->current.origin + vert6);
		rent.radius_aabb.addPoint(cent->current.origin + vert7);
			
		rent.radius_value = rent.radius_aabb.radius();
			
		trap_R_AddLight(cent->current.getNumber(), rent, LIGHT_OMNI);
	}
}

void	CG_UpdateGenericEntity(const cg_entity_t *cent)
{
//	trap_Com_DPrintf("updating generic entity %i ...\n", cent->current.getNumber());
	
	r_entity_t	rent;
	bool		update = false;
	
	CG_UpdateOrigin(cent, rent, update);
	
	CG_UpdateRotation(cent, rent, update);
	
	CG_UpdateModel(cent, rent, update);
	
	CG_UpdateAnimation(cent, rent, update);
	
	CG_UpdateShader(cent, rent, update);
	
	CG_UpdateShaderParms(cent, rent, update);

	CG_UpdateFrame(cent, rent, update);
	
	CG_UpdateRenderFXFlags(cent, rent, update);
	
	if(update)
		trap_R_UpdateEntity(cent->current.getNumber(), rent);
		
//	update = false;
	
	CG_UpdateLightShader(cent, rent, update);
	
	if(cent->prev.vectors[0] != cent->current.vectors[0])
	{
		update = true;
	}
	
	if(cent->current.index_light && !cent->current.vectors[0].isZero() && update)
	{
		rent.radius.lerp(cent->prev.vectors[0], cent->current.vectors[0], cg.frame_lerp);
		
		rent.radius_aabb.clear();
			
		// compute bbox vertices in light space
		vec3_c vert0( rent.radius[0], -rent.radius[1], -rent.radius[2]);
		vec3_c vert1( rent.radius[0], -rent.radius[1],  rent.radius[2]);
		vec3_c vert2(-rent.radius[0], -rent.radius[1],  rent.radius[2]);
		vec3_c vert3(-rent.radius[0], -rent.radius[1], -rent.radius[2]);
	
		vec3_c vert4( rent.radius[0],  rent.radius[1], -rent.radius[2]);
		vec3_c vert5( rent.radius[0],  rent.radius[1],  rent.radius[2]);
		vec3_c vert6(-rent.radius[0],  rent.radius[1],  rent.radius[2]);
		vec3_c vert7(-rent.radius[0],  rent.radius[1], -rent.radius[2]);
			
		// rotate vertices in light space
		vert0.rotate(rent.quat);
		vert1.rotate(rent.quat);
		vert2.rotate(rent.quat);
		vert3.rotate(rent.quat);
		vert4.rotate(rent.quat);
		vert5.rotate(rent.quat);
		vert6.rotate(rent.quat);
		vert7.rotate(rent.quat);
			
		// transform vertices into world space and add them to the light world aabb
		rent.radius_aabb.addPoint(rent.origin + vert0);
		rent.radius_aabb.addPoint(rent.origin + vert1);
		rent.radius_aabb.addPoint(rent.origin + vert2);
		rent.radius_aabb.addPoint(rent.origin + vert3);
		rent.radius_aabb.addPoint(rent.origin + vert4);
		rent.radius_aabb.addPoint(rent.origin + vert5);
		rent.radius_aabb.addPoint(rent.origin + vert6);
		rent.radius_aabb.addPoint(rent.origin + vert7);
			
		rent.radius_value = rent.radius_aabb.radius();
	
		trap_R_UpdateLight(cent->current.getNumber(), rent, LIGHT_OMNI);
	}
}

void	CG_RemoveGenericEntity(const cg_entity_t *cent)
{
//	trap_Com_DPrintf("removing generic entity %i ...\n", cent->prev.getNumber());

	trap_R_RemoveEntity(cent->prev.getNumber());
	trap_R_RemoveLight(cent->prev.getNumber());
}


/*
===============
CG_AddEntities

Emits all entities, particles, and lights to the refresh
===============
*/
void	CG_UpdateEntities()
{
	//CG_AddPacketEntities();
	//CG_AddViewWeapon();
	//CG_AddBeams();
	//CG_AddExplosions();
	//CG_AddLasers();
	//CG_AddParticles();
	//CG_AddLights();
	
	for(int i=0; i<cg.frame.entities_num; i++)
	{
		entity_state_t *state = &cg.entities_parse[(cg.frame.entities_first + i) % MAX_ENTITIES];
		cg_entity_t *cent = &cg.entities[state->getNumber()];
		
		/*
		if(!cent->prev.getNumber() && !cent->current.getNumber())
			continue;
		
		if(!cent->prev.getNumber() && cent->current.getNumber())
		{
			switch(cent->prev.type)
			{
				case ET_GENERIC:
				case ET_FUNC_STATIC:
					CG_AddGenericEntity(cent);
					break;
					
				case ET_LIGHT_OMNI:
				case ET_LIGHT_PROJ:
					CG_AddLightEntity(cent);
					break;
				default:
					break;
			}
			continue;
		}
		*/
		
		/*
// 		if(cent->prev.getNumber() && !cent->current.getNumber())
		if((cent->serverframe_old == cg.frame.serverframe -1) && (cent->serverframe != cg.frame.serverframe))
		{
			// entity was in last frame but is not in the current
			switch(cent->prev.type)
			{
				case ET_GENERIC:
				case ET_FUNC_STATIC:
					CG_RemoveGenericEntity(cent);
					break;
					
				case ET_LIGHT_OMNI:
				case ET_LIGHT_PROJ:
					CG_RemoveLightEntity(cent);
					break;
				
				default:
					break;
			}
			continue;
		}
		*/
		
		CG_UpdateSound(cent);
		
		switch(cent->prev.type)
		{
			case ET_GENERIC:
 			case ET_FUNC_STATIC:
			case ET_PROJECTILE_ROCKET:
				CG_UpdateGenericEntity(cent);
				break;
		
// 			case ET_LIGHT_OMNI:
//  			case ET_LIGHT_PROJ:
//  				CG_UpdateLightEntity(cent);
//  				break;
			
			default:
				break;
		}
	}
}
