/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.
Copyright (C) 2006 Robert Beckebans <trebor_7@users.sourceforge.net>
Copyright (C) 2007 Jeremy Hughes <Encryption767@msn.com>

This file is part of XreaL source code.

XreaL source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

XreaL source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with XreaL source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
// g_misc.c

#include "g_local.h"


/*QUAKED func_group (0 0 0) ?
Used to group brushes together just for editor convenience.  They are turned into normal brushes by the utilities.
*/


/*QUAKED info_camp (0 0.5 0) (-4 -4 -4) (4 4 4)
Used as a positional target for calculations in the utilities (spotlights, etc), but removed during gameplay.
*/
void SP_info_camp(gentity_t * self)
{
	G_SetOrigin(self, self->s.origin);
}


/*QUAKED info_null (0 0.5 0) (-4 -4 -4) (4 4 4)
Used as a positional target for calculations in the utilities (spotlights, etc), but removed during gameplay.
*/
void SP_info_null(gentity_t * self)
{
	G_FreeEntity(self);
}


/*QUAKED info_notnull (0 0.5 0) (-4 -4 -4) (4 4 4)
Used as a positional target for in-game calculation, like jumppad targets.
target_position does the same thing
*/
void SP_info_notnull(gentity_t * self)
{
	G_SetOrigin(self, self->s.origin);
}


/*QUAKED light (0 1 0) (-8 -8 -8) (8 8 8) linear
Non-displayed light.
"light" overrides the default 300 intensity.
Linear checbox gives linear falloff instead of inverse square
Lights pointed at a target will be spotlights.
"radius" overrides the default 64 unit radius of a spotlight at the target point.
*/
void SP_light(gentity_t * self)
{
	G_FreeEntity(self);
}



/*
=================================================================================

TELEPORTERS

=================================================================================
*/

void TeleportPlayer(gentity_t * player, vec3_t origin, vec3_t angles)
{
	gentity_t      *tent;

	// use temp events at source and destination to prevent the effect
	// from getting dropped by a second player event
	if(player->client->sess.sessionTeam != TEAM_SPECTATOR)
	{
		tent = G_TempEntity(player->client->ps.origin, EV_PLAYER_TELEPORT_OUT);
		tent->s.clientNum = player->s.clientNum;

		tent = G_TempEntity(origin, EV_PLAYER_TELEPORT_IN);
		tent->s.clientNum = player->s.clientNum;
	}

	// unlink to make sure it can't possibly interfere with G_KillBox
	trap_UnlinkEntity(player);

	VectorCopy(origin, player->client->ps.origin);
	player->client->ps.origin[2] += 1;

	// spit the player out
	AngleVectors(angles, player->client->ps.velocity, NULL, NULL);
	VectorScale(player->client->ps.velocity, 400, player->client->ps.velocity);
	player->client->ps.pm_time = 160;	// hold time
	player->client->ps.pm_flags |= PMF_TIME_KNOCKBACK;

	// toggle the teleport bit so the client knows to not lerp
	player->client->ps.eFlags ^= EF_TELEPORT_BIT;
//unlagged - backward reconciliation #3
	// we don't want players being backward-reconciled back through teleporters
	//G_ResetHistory( player );
//unlagged - backward reconciliation #3
	// set angles
	SetClientViewAngle(player, angles);

	// kill anything at the destination
	if(player->client->sess.sessionTeam != TEAM_SPECTATOR)
	{
		G_KillBox(player);
	}

	// save results of pmove
	BG_PlayerStateToEntityState(&player->client->ps, &player->s, qtrue, qfalse);

	// use the precise origin for linking
	VectorCopy(player->client->ps.origin, player->r.currentOrigin);

	if(player->client->sess.sessionTeam != TEAM_SPECTATOR)
	{
		trap_LinkEntity(player);
	}
}


/*QUAKED misc_teleporter_dest (1 0 0) (-32 -32 -24) (32 32 -16)
Point teleporters at these.
Now that we don't have teleport destination pads, this is just
an info_notnull
*/
void SP_misc_teleporter_dest(gentity_t * ent)
{
}


//===========================================================

/*QUAKED misc_model (1 0 0) (-16 -16 -16) (16 16 16)
"model"		arbitrary .md3 file to display
*/
void SP_misc_model(gentity_t * ent)
{

#if 0
	ent->s.modelindex = G_ModelIndex(ent->model);
	VectorSet(ent->mins, -16, -16, -16);
	VectorSet(ent->maxs, 16, 16, 16);
	trap_LinkEntity(ent);

	G_SetOrigin(ent, ent->s.origin);
	VectorCopy(ent->s.angles, ent->s.apos.trBase);
#else
	G_FreeEntity(ent);
#endif
}

//===========================================================

void locateCamera(gentity_t * ent)
{
	vec3_t          dir;
	gentity_t      *target;
	gentity_t      *owner;

	owner = G_PickTarget(ent->target);
	if(!owner)
	{
		G_Printf("Couldn't find target for misc_partal_surface\n");
		G_FreeEntity(ent);
		return;
	}
	ent->r.ownerNum = owner->s.number;

	// frame holds the rotate speed
	if(owner->spawnflags & 1)
	{
		ent->s.frame = 25;
	}
	else if(owner->spawnflags & 2)
	{
		ent->s.frame = 75;
	}

	// swing camera ?
	if(owner->spawnflags & 4)
	{
		// set to 0 for no rotation at all
		ent->s.powerups = 0;
	}
	else
	{
		ent->s.powerups = 1;
	}

	// clientNum holds the rotate offset
	ent->s.clientNum = owner->s.clientNum;

	VectorCopy(owner->s.origin, ent->s.origin2);

	// see if the portal_camera has a target
	target = G_PickTarget(owner->target);
	if(target)
	{
		VectorSubtract(target->s.origin, owner->s.origin, dir);
		VectorNormalize(dir);
	}
	else
	{
		G_SetMovedir(owner->s.angles, dir);
	}

	ent->s.eventParm = DirToByte(dir);
}

/*QUAKED misc_portal_surface (0 0 1) (-8 -8 -8) (8 8 8)
The portal surface nearest this entity will show a view from the targeted misc_portal_camera, or a mirror view if untargeted.
This must be within 64 world units of the surface!
*/
void SP_misc_portal_surface(gentity_t * ent)
{
	VectorClear(ent->r.mins);
	VectorClear(ent->r.maxs);
	trap_LinkEntity(ent);

	ent->r.svFlags = SVF_PORTAL;
	ent->s.eType = ET_PORTAL;

	if(!ent->target)
	{
		VectorCopy(ent->s.origin, ent->s.origin2);
	}
	else
	{
		ent->think = locateCamera;
		ent->nextthink = level.time + 100;
	}

}

/*QUAKED misc_portalsky_surface (0 0 1) (-8 -8 -8) (8 8 8)
The portal surface nearest this entity will show a view from the targeted misc_portal_camera, or a mirror view if untargeted.
This must be within 64 world units of the surface!
*/

void SP_misc_portalsky_surface(gentity_t * ent)
{
	level.skyportal = qtrue;
	VectorClear(ent->r.mins);
	VectorClear(ent->r.maxs);
	trap_LinkEntity(ent);
	VectorCopy(ent->r.currentOrigin, level.skyportal_origin);
}

/*QUAKED emission_spark_emission (0 0 1) (-8 -8 -8) (8 8 8)
This must have a target position to work!!!
"speed" speed of the sparks
*/
void SP_spark_emission(gentity_t * ent)
{
	vec3_t          dir;
	int             vel;

//  gentity_t *owner;

	VectorClear(ent->r.mins);
	VectorClear(ent->r.maxs);
	trap_LinkEntity(ent);

	ent->s.eType = ET_SPARK;

	G_SetOrigin(ent, ent->s.origin);

	G_SpawnInt("speed", "20", &vel);
	ent->s.powerups = vel;

	VectorSubtract(ent->s.origin, ent->target, dir);
	VectorNormalize(dir);

	G_SetMovedir(ent->s.angles, dir);
	ent->s.eventParm = DirToByte(dir);



}

/*QUAKED misc_portal_camera (0 0 1) (-8 -8 -8) (8 8 8) slowrotate fastrotate noswing
The target for a misc_portal_director.  You can set either angles or target another entity to determine the direction of view.
"roll" an angle modifier to orient the camera around the target vector;
*/
void SP_misc_portal_camera(gentity_t * ent)
{
	float           roll;

	VectorClear(ent->r.mins);
	VectorClear(ent->r.maxs);
	trap_LinkEntity(ent);

	G_SpawnFloat("roll", "0", &roll);

	ent->s.clientNum = roll / 360.0 * 256;
}

/*
======================================================================

  SHOOTERS

======================================================================
*/

void Use_Shooter(gentity_t * ent, gentity_t * other, gentity_t * activator)
{
	vec3_t          dir;
	float           deg;
	vec3_t          up, right;

	// see if we have a target
	if(ent->enemy)
	{
		VectorSubtract(ent->enemy->r.currentOrigin, ent->s.origin, dir);
		VectorNormalize(dir);
	}
	else
	{
		VectorCopy(ent->movedir, dir);
	}

	// randomize a bit
	PerpendicularVector(up, dir);
	CrossProduct(up, dir, right);

	deg = crandom() * ent->random;
	VectorMA(dir, deg, up, dir);

	deg = crandom() * ent->random;
	VectorMA(dir, deg, right, dir);

	VectorNormalize(dir);

	switch (ent->s.weapon)
	{
		case WP_GRENADE_LAUNCHER:
			fire_grenade(ent, ent->s.origin, dir);
			break;
		case WP_ROCKET_LAUNCHER:
			fire_rocket(ent, ent->s.origin, dir);
			break;
		case WP_PLASMAGUN:
			fire_plasma(ent, ent->s.origin, dir);
			break;
	}

	G_AddEvent(ent, EV_FIRE_WEAPON, 0);
}


static void InitShooter_Finish(gentity_t * ent)
{
	ent->enemy = G_PickTarget(ent->target);
	ent->think = 0;
	ent->nextthink = 0;
}

void InitShooter(gentity_t * ent, int weapon)
{
	ent->use = Use_Shooter;
	ent->s.weapon = weapon;

	RegisterItem(BG_FindItemForWeapon(weapon));

	G_SetMovedir(ent->s.angles, ent->movedir);

	if(!ent->random)
	{
		ent->random = 1.0;
	}
	ent->random = sin(M_PI * ent->random / 180);
	// target might be a moving object, so we can't set movedir for it
	if(ent->target)
	{
		ent->think = InitShooter_Finish;
		ent->nextthink = level.time + 500;
	}
	trap_LinkEntity(ent);
}

/*QUAKED shooter_rocket (1 0 0) (-16 -16 -16) (16 16 16)
Fires at either the target or the current direction.
"random" the number of degrees of deviance from the taget. (1.0 default)
*/
void SP_shooter_rocket(gentity_t * ent)
{
	InitShooter(ent, WP_ROCKET_LAUNCHER);
}

/*QUAKED shooter_plasma (1 0 0) (-16 -16 -16) (16 16 16)
Fires at either the target or the current direction.
"random" is the number of degrees of deviance from the taget. (1.0 default)
*/
void SP_shooter_plasma(gentity_t * ent)
{
	InitShooter(ent, WP_PLASMAGUN);
}

/*QUAKED shooter_grenade (1 0 0) (-16 -16 -16) (16 16 16)
Fires at either the target or the current direction.
"random" is the number of degrees of deviance from the taget. (1.0 default)
*/
void SP_shooter_grenade(gentity_t * ent)
{
	InitShooter(ent, WP_GRENADE_LAUNCHER);
}

/////////////////////////////
// FIRES AND EXPLOSION PROPS
/////////////////////////////

/*QUAKED props_FireColumn (.3 .2 .7) (-8 -8 -8) (8 8 8) CORKSCREW SMOKE GRAVITY HALFGRAVITY 
this entity will require a target use an infonotnull to specifiy its direction

defaults:
	will leave a flaming trail by default
	will not be affected by gravity

radius = distance flame will corkscrew from origin
speed = default is 0 so this needs to be set
duration = default is 3 sec
 
start_size = default is 5
end_size = defaults 7 thru 17
count = defaults 100 thru 500 

Pending:   
delay before it happens again use trigger_relay for now
assign a model
*/

void propsFireColumnUse(gentity_t * ent)
{
	gentity_t      *tent;
	vec3_t          dir;



	AngleVectors(ent->r.currentAngles, dir, NULL, NULL);

	tent = G_Spawn();
	trap_UnlinkEntity(tent);

	if(ent->spawnflags & 2)
	{
		tent->s.eType = ET_FIRE_COLUMN;
	}
	else
	{
		tent->s.eType = ET_FIRE_COLUMN;
	}
	tent->classname = "props_FireColumn";
	tent->s.clientNum = tent - g_entities;
	tent->r.svFlags = SVF_BROADCAST;
	if(ent->spawnflags & 4)
	{
		tent->s.pos.trType = TR_GRAVITY;
	}
	else if(ent->spawnflags & 8)
	{
		tent->s.pos.trType = TR_GRAVITY_LOW;
	}
	else
	{
		tent->s.pos.trType = TR_LINEAR;
	}

	if(ent->spawnflags & 1)
	{
		tent->s.otherEntityNum2 = ent->radius;	// corkscrew effect
	}

	VectorCopy(dir, tent->s.angles);

	tent->s.origin2[0] = ent->duration;

	tent->s.origin2[1] = ent->speed;

	tent->s.angles2[0] = ent->count;

	tent->s.angles2[1] = ent->start_size;

	tent->s.angles2[2] = ent->end_size;

	tent->think = G_FreeEntity;
	tent->nextthink = level.time + FRAMETIME;
//  ent->think = G_FreeEntity;
//  ent->nextthink = level.time + FRAMETIME;
	VectorCopy(ent->r.currentOrigin, tent->s.origin);
	VectorCopy(ent->r.currentOrigin, tent->r.currentOrigin);
	trap_LinkEntity(tent);
}

void SP_propsFireColumn(gentity_t * ent)
{
	gentity_t      *target;
	vec3_t          vec, angles;
	vec3_t          dir;
	int             i;
	gentity_t      *pent;

	G_SetOrigin(ent, ent->s.origin);

	if(ent->target)
	{
		target = G_Find(NULL, FOFS(targetname), ent->target);
		VectorSubtract(target->s.origin, ent->s.origin, vec);
		vectoangles(vec, angles);
		G_SetAngle(ent, angles);
	}
	else
	{
		// ok then just up
		VectorSet(vec, 0, 0, 1);
		vectoangles(vec, angles);
		G_SetAngle(ent, angles);
	}


	if(ent->spawnflags & 2)
	{
		ent->s.eType = ET_FIRE_COLUMN;
	}
	else
	{
		ent->s.eType = ET_FIRE_COLUMN;
	}
	ent->classname = "props_FireColumn";
	ent->s.clientNum = ent - g_entities;
	ent->r.svFlags = SVF_SINGLECLIENT;

	for(i = 0; i < MAX_CLIENTS; i++)
	{
		pent = &g_entities[i];
		if(!pent->inuse)
		{
			continue;
		}
		if(!pent->client)
		{
			continue;
		}
		if(pent->client->pers.connected == CON_CONNECTING)
		{
			continue;
		}
		ent->r.singleClient = i;

	}
	if(ent->spawnflags & 4)
	{
		ent->s.pos.trType = TR_GRAVITY;
	}
	else if(ent->spawnflags & 8)
	{
		ent->s.pos.trType = TR_GRAVITY_LOW;
	}
	else
	{
		ent->s.pos.trType = TR_LINEAR;
	}

	if(ent->spawnflags & 1)
	{
		ent->s.otherEntityNum2 = ent->radius;	// corkscrew effect
	}
	AngleVectors(ent->r.currentAngles, dir, NULL, NULL);
	VectorCopy(dir, ent->s.angles);

	ent->s.origin2[0] = ent->duration;

	ent->s.origin2[1] = ent->speed;

	ent->s.angles2[0] = ent->count;

	ent->s.angles2[1] = ent->start_size;

	ent->s.angles2[2] = ent->end_size;

//  tent->think = G_FreeEntity;
//  tent->nextthink = level.time + FRAMETIME;
	ent->think = G_FreeEntity;
	ent->nextthink = level.time + 1005;
	VectorCopy(ent->r.currentOrigin, ent->s.origin);
//  VectorCopy (ent->r.currentOrigin, tent->r.currentOrigin);



//  ent->think = propsFireColumnUse;
//  ent->nextthink = level.time + FRAMETIME;
	trap_LinkEntity(ent);
}

#ifdef MISSIONPACK
static void PortalDie(gentity_t * self, gentity_t * inflictor, gentity_t * attacker, int damage, int mod)
{
	G_FreeEntity(self);
	//FIXME do something more interesting
}


void DropPortalDestination(gentity_t * player)
{
	gentity_t      *ent;
	vec3_t          snapped;

	// create the portal destination
	ent = G_Spawn();
	ent->s.modelindex = G_ModelIndex("models/powerups/teleporter/tele_exit.md3");

	VectorCopy(player->s.pos.trBase, snapped);
	SnapVector(snapped);
	G_SetOrigin(ent, snapped);
	VectorCopy(player->r.mins, ent->r.mins);
	VectorCopy(player->r.maxs, ent->r.maxs);

	ent->classname = "hi_portal destination";
	ent->s.pos.trType = TR_STATIONARY;

	ent->r.contents = CONTENTS_CORPSE;
	ent->takedamage = qtrue;
	ent->health = 200;
	ent->die = PortalDie;

	VectorCopy(player->s.apos.trBase, ent->s.angles);

	ent->think = G_FreeEntity;
	ent->nextthink = level.time + 2 * 60 * 1000;

	trap_LinkEntity(ent);

	player->client->portalID = ++level.portalSequence;
	ent->count = player->client->portalID;

	// give the item back so they can drop the source now
	player->client->ps.stats[STAT_HOLDABLE_ITEM] = BG_FindItem("Portal") - bg_itemlist;
}


static void PortalTouch(gentity_t * self, gentity_t * other, trace_t * trace)
{
	gentity_t      *destination;

	// see if we will even let other try to use it
	if(other->health <= 0)
	{
		return;
	}
	if(!other->client)
	{
		return;
	}

	if(other->client->ps.powerups[PW_REDFLAG])
	{							// only happens in standard CTF
		Drop_Item(other, BG_FindItemForPowerup(PW_REDFLAG), 0);
		other->client->ps.powerups[PW_REDFLAG] = 0;
	}
	if(other->client->ps.powerups[PW_BLUEFLAG])
	{							// only happens in standard CTF
		Drop_Item(other, BG_FindItemForPowerup(PW_BLUEFLAG), 0);
		other->client->ps.powerups[PW_BLUEFLAG] = 0;
	}

	// find the destination
	destination = NULL;
	while((destination = G_Find(destination, FOFS(classname), "hi_portal destination")) != NULL)
	{
		if(destination->count == self->count)
		{
			break;
		}
	}

	// if there is not one, die!
	if(!destination)
	{
		if(self->pos1[0] || self->pos1[1] || self->pos1[2])
		{
			TeleportPlayer(other, self->pos1, self->s.angles);
		}
		G_Damage(other, other, other, NULL, NULL, 100000, DAMAGE_NO_PROTECTION, MOD_TELEFRAG);
		return;
	}

	TeleportPlayer(other, destination->s.pos.trBase, destination->s.angles);
}


static void PortalEnable(gentity_t * self)
{
	self->touch = PortalTouch;
	self->think = G_FreeEntity;
	self->nextthink = level.time + 2 * 60 * 1000;
}


void DropPortalSource(gentity_t * player)
{
	gentity_t      *ent;
	gentity_t      *destination;
	vec3_t          snapped;

	// create the portal source
	ent = G_Spawn();
	ent->s.modelindex = G_ModelIndex("models/powerups/teleporter/tele_enter.md3");

	VectorCopy(player->s.pos.trBase, snapped);
	SnapVector(snapped);
	G_SetOrigin(ent, snapped);
	VectorCopy(player->r.mins, ent->r.mins);
	VectorCopy(player->r.maxs, ent->r.maxs);

	ent->classname = "hi_portal source";
	ent->s.pos.trType = TR_STATIONARY;

	ent->r.contents = CONTENTS_CORPSE | CONTENTS_TRIGGER;
	ent->takedamage = qtrue;
	ent->health = 200;
	ent->die = PortalDie;

	trap_LinkEntity(ent);

	ent->count = player->client->portalID;
	player->client->portalID = 0;

//  ent->spawnflags = player->client->ps.persistant[PERS_TEAM];

	ent->nextthink = level.time + 1000;
	ent->think = PortalEnable;

	// find the destination
	destination = NULL;
	while((destination = G_Find(destination, FOFS(classname), "hi_portal destination")) != NULL)
	{
		if(destination->count == ent->count)
		{
			VectorCopy(destination->s.pos.trBase, ent->pos1);
			break;
		}
	}

}
#endif
