// GFX.SHADER
// 
// this file contains shaders that are used by the programmers to
// generate special effects not attached to specific geometry.  This
// also has 2D shaders such as fonts, etc.
//



white
{
	{
		map _white
		blend	GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}





menuBanner
{
	nopicmip
	nomipmaps

	{
		blend blend
		map menu/art/xreal_highlight.tga
		//scroll (sinTable[ time * 0.1] * 0.01), (cosTable[ time * 0.1] * -0.01)
	}
	{
		blend blend
		map menu/art/xreal_redglow.tga
		alpha 0.5 + (sinTable[ time * 0.5] * 0.5)
	}
	{
		blend blend
		map menu/art/xreal_chrome2.tga
		//alpha 0.5 + (sinTable[ time * 0.2] * -0.5)
	}
	/*
	{
		blend blend
		map menu/art/xreal_chrome.tga
		alpha 0.5 + (sinTable[ time * 0.2] * 0.5)
	}
	*/
}



levelShotDetail
// used to break up the blur on levelshots
{
	nopicmip
	{
		map gfx/misc/detail_strip.tga
		blendFunc GL_DST_COLOR GL_SRC_COLOR
		rgbgen identity
	}
}

//
// special effects as seen on players
//
powerups/battleSuit
{
	deformVertexes wave 100 sin 1 0 0 0
	{
		map textures/effects/envmapgold2.tga
        //map textures/sfx/specular.tga
		tcGen environment
		tcMod turb 0 0.15 0 0.3
        tcmod rotate 333
        tcmod scroll .3 .3
		blendfunc GL_ONE GL_ONE
	}
}

powerups/battleWeapon
{
	deformVertexes wave 100 sin 0.5 0 0 0
	{
		map textures/effects/envmapgold2.tga
        //map textures/sfx/specular.tga
		tcGen environment
		tcMod turb 0 0.15 0 0.3
        tcmod rotate 333
        tcmod scroll .3 .3
		blendfunc GL_ONE GL_ONE
	}
}

powerups/invisibility
{
	sort postProcess
	
	{
		if 1
		map gfx/effect/invismap.png	
        blue	0.3 + sinTable[time * 0.2] * 0.2
      	scroll sinTable[time * 0.2] * 0.5 + 0.5, 0
		blend add
	}
	{
		stage	heathazemap
		map		gfx/effect/Reptile_Skin_normal.png
		scroll	time * 0.05 , time * 0.05
		//scale	0.3, 0.3
		deformMagnitude	1.7
    }
}

powerups/quad
{
	//deformVertexes wave 100 sin 3 0 0 0
	{
		map gfx/effect/quad_fx.tga
		blend add
		rotate time * 30
		scroll time * 1, time * .1
	}
}

powerups/quadWeapon
{
	//deformVertexes wave 100 sin 0.5 0 0 0
	{
		map gfx/effect/quad_fx.tga
		blend add
		rotate time * 30
		scroll time * 1, time * .1
	}
}

powerups/regen
{
	deformVertexes wave 100 sin 3 0 0 0
	{
		map textures/effects/regenmap2.tga
				blendfunc GL_ONE GL_ONE
				tcGen environment
				tcmod rotate 30
		//tcMod turb 0 0.2 0 .2
				tcmod scroll 1 .1
	}
}

powerups/blueflag
{
	deformVertexes wave 100 sin 3 0 0 0
	{
		map textures/effects/blueflagmap.tga
				blendfunc GL_ONE GL_ONE
				tcGen environment
				tcMod turb 0 0.2 0 1
	}
}

powerups/redflag
{
	deformVertexes wave 100 sin 3 0 0 0
	{
		map textures/effects/redflagmap.tga
				blendfunc GL_ONE GL_ONE
				tcGen environment
				tcMod turb 0 0.2 0 1
	}
}


//=========================

//
// holdable item icons
//
icons/teleporter
{
	nopicmip
	{
		map icons/teleporter.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

icons/medkit
{
	nopicmip
	{
		map icons/medkit.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

icons/kamikaze
{
	nopicmip
	{
		map icons/kamikaze.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

icons/portal
{
	nopicmip
	{
		map icons/portal.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

//
// powerup icons
//
icons/envirosuit
{
	nopicmip
	{
		map icons/envirosuit.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
icons/quad
{
	nopicmip
	{
		map icons/quad.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
icons/haste
{
	nopicmip
	{
		map icons/haste.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
icons/invis
{
	nopicmip
	{
		map icons/invis.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
icons/regen
{
	nopicmip
	{
		map icons/regen.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
icons/flight
{
	nopicmip
	{
		map icons/flight.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

//
// persistant powerup icons
//
icons/scout
{
	nopicmip
	{
		map icons/scout.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
icons/guard
{
	nopicmip
	{
		map icons/guard.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
icons/doubler
{
	nopicmip
	{
		map icons/doubler.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
icons/radar
{
	nopicmip
	{
		map icons/radar.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
icons/ammo_regen
{
	nopicmip
	{
		map icons/ammo_regen.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
icons/invulnerability
{
	nopicmip
	{
		map icons/invulnerability.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}


// reward medals used in the game floating over client's heads
medal_impressive
{
	nopicmip
	{
		clampmap menu/medals/medal_impressive.png
		blend blend
	}
}

medal_excellent
{
	nopicmip
	{
		clampmap menu/medals/medal_excellent.png
		blend blend
	}
}

medal_gauntlet
{
	nopicmip
	{
		clampmap menu/medals/medal_gauntlet.png
		blend blend
	}
}

medal_assist
{
	nopicmip
	{
		clampmap menu/medals/medal_assist.png
		blend blend
	}
}

medal_defend
{
	nopicmip
	{
		clampmap menu/medals/medal_defend.png
		blend blend
	}
}

medal_capture
{
	nopicmip
	{
		clampmap menu/medals/medal_capture.png
		blend blend
	}
}

medal_telefrag
{
	nopicmip
	{
		clampmap menu/medals/medal_telefrag.png
		blend blend
	}
}



//
// weapon icons
//
icons/weapon_gauntlet
{
	nopicmip
	{
		map icons/weapon_gauntlet.tga
		blend blend
		rgbGen vertex
	}
}
icons/weapon_machinegun
{
	nopicmip
	{
		map icons/weapon_machinegun.tga
		blend blend
		rgbGen vertex
	}
}
icons/weapon_rocket
{
	nopicmip
	{
		map icons/weapon_rocket.tga
		blend blend
		rgbGen vertex
	}
}

icons/weapon_shotgun
{
	nopicmip
	{
		map icons/weapon_shotgun.tga
		blend blend
		rgbGen vertex
	}
}

icons/weapon_grenade
{
	nopicmip
	{
		map icons/weapon_grenade.tga
		blend blend
		rgbGen vertex
	}
}

icons/weapon_lightning
{
	nopicmip
	{
		map icons/weapon_lightning.tga
		blend blend
		rgbGen vertex
	}
}

icons/weapon_plasma
{
	nopicmip
	{
		map icons/weapon_plasma.tga
		blend blend
		rgbGen vertex
	}
}

icons/weapon_railgun
{
	nopicmip
	{
		map icons/weapon_railgun.tga
		blend blend
		rgbGen vertex
	}
}

icons/weapon_bfg
{
	nopicmip
	{
		map icons/weapon_bfg.tga
		blend blend
		rgbGen vertex
	}
}

icons/weapon_grapple
{
	nopicmip
	{
		map icons/weapon_grapple.tga
		blend blend
		rgbGen vertex
	}
}

icons/weapon_nailgun
{
	nopicmip
	{
		map icons/weapon_nailgun.tga
		blend blend
		rgbGen vertex
	}
}

icons/weapon_chaingun
{
	nopicmip
	{
		map icons/weapon_chaingun.tga
		blend blend
		rgbGen vertex
	}
}

icons/weapon_proxlauncher
{
	nopicmip
	{
		map icons/weapon_proxmine.tga
		blend blend
		rgbGen vertex
	}
}

//
// ammo icons
//
icons/ammo_machinegun
{
	nopicmip
	{
		map icons/ammo_machinegun.tga
		blend blend
		rgbGen vertex
	}
}
icons/ammo_rocket
{
	nopicmip
	{
		map icons/ammo_rocket.tga
		blend blend
		rgbGen vertex
	}
}

icons/ammo_shotgun
{
	nopicmip
	{
		map icons/ammo_shotgun.tga
		blend blend
		rgbGen vertex
	}
}

icons/ammo_grenade
{
	nopicmip
	{
		map icons/ammo_grenade.tga
		blend blend
		rgbGen vertex
	}
}

icons/ammo_lightning
{
	nopicmip
	{
		map icons/ammo_lightning.tga
		blend blend
		rgbGen vertex
	}
}

icons/ammo_plasma
{
	nopicmip
	{
		map icons/ammo_plasma.tga
		blend blend
		rgbGen vertex
	}
}

icons/ammo_railgun
{
	nopicmip
	{
		map icons/ammo_railgun.tga
		blend blend
		rgbGen vertex
	}
}

icons/ammo_bfg
{
	nopicmip
	{
		map icons/ammo_bfg.tga
		blend blend
		rgbGen vertex
	}
}

icons/ammo_nailgun
{
	nopicmip
	{
		map icons/ammo_nailgun.tga
		blend blend
		rgbGen vertex
	}
}

icons/ammo_chaingun
{
	nopicmip
	{
		map icons/ammo_chaingun.tga
		blend blend
		rgbGen vertex
	}
}

icons/ammo_proxlauncher
{
	nopicmip
	{
		map icons/ammo_proxmine.tga
		blend blend
		rgbGen vertex
	}
}


//
// armor icons
//
icons/iconr_shard
{
	nopicmip
	{
		map icons/iconr_shard.tga
		blend blend
		rgbGen vertex
	}
}

icons/iconr_yellow
{
	nopicmip
	{
		map icons/iconr_yellow.tga
		blend blend
		rgbGen vertex
	}
}

icons/iconr_red
{
	nopicmip
	{
		map icons/iconr_red.tga
		blend blend
		rgbGen vertex
	}
}

//
// health icons
//
icons/iconh_green
{
	nopicmip
	{
		map icons/iconh_green.tga
		blend blend
		rgbGen vertex
	}
}

icons/iconh_yellow
{
	nopicmip
	{
		map icons/iconh_yellow.tga
		blend blend
		rgbGen vertex
	}
}

icons/iconh_red
{
	nopicmip
	{
		map icons/iconh_red.tga
		blend blend
		rgbGen vertex
	}
        //{
	//	clampmap icons/iconh_red2.tga
	//	blendFunc blend
        //        tcMod stretch sin .04 1 0 1
	//}
}

icons/iconh_mega
{
	nopicmip
	{
		map icons/iconh_mega.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

//
// CTF icons
//
icons/iconf_red
{
	nopicmip
	{
		map icons/iconf_red.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

icons/iconf_blu
{
	nopicmip
	{
		map icons/iconf_blu.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}


//===================================================

gfx/2d/menuinfo
{
	nopicmip
	{
		map gfx/2d/menuinfo.tga
	}
}

gfx/2d/menuinfo2
{
	nopicmip
	{
		map gfx/2d/menuinfo2.tga
	}
}

gfx/2d/quit
{
	nopicmip
	nomipmaps
	{
		map gfx/2d/quit.tga
	}
}

gfx/2d/cursor
{
    nopicmip
	nomipmaps
    {
        map gfx/2d/cursor.tga
    }
}

//==========================================================================

sprites/balloon3
{
	nomipmaps
	nopicmip
	{
		map sprites/balloon4.png
		centerScale 1.1 - (0.3 *  (  (   (sinTable[time * .7] + 1.0) / 2.0 ) )), 1.1 - (0.3 *  (  (   (sinTable[time * .7] + 1.0) / 2.0 ) ))

		blend blend
		alpha 0.6 + 0.3 * ( 1.0 -  (   (sinTable[time * .5] + 1.0) / 2.0 ) )
	}
}

// friend and foe icons floating over client's heads in teamplay games
sprites/friend
{
	nomipmaps
	nopicmip
	{
		map sprites/friend.png
		centerScale 1.1 - (0.3 *  (  (   (sinTable[time * .7] + 1.0) / 2.0 ) )), 1.1 - (0.3 *  (  (   (sinTable[time * .7] + 1.0) / 2.0 ) ))

		blend blend
		alpha 0.6 + 0.3 * ( 1.0 -  (   (sinTable[time * .5] + 1.0) / 2.0 ) )
	}
}

sprites/foe
{
	nomipmaps
	nopicmip
	{
		map gfx/misc/foe.png
		blend GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA
	}
}

sprites/plasma1
{
	twoSided
	{
		clampmap gfx/misc/plasmaa.png
		blend add
		rotate time * 3
	}
}

teleportEffect
{
	twosided

	{
		map gfx/misc/teleportEffect.tga
		blend GL_ONE , GL_ONE
		rgbGen entity
		scale 1 , 4
		scroll time * 1, time * 1
	}
}

// markShadow is the very cheap blurry blob underneat the player
markShadow
{
	polygonOffset
	{
		map gfx/misc/shadow.tga
		blend GL_ZERO, GL_ONE_MINUS_SRC_COLOR
		vertexColor
	}
}

// projectionShadow is used for cheap squashed model shadows
projectionShadow
{
	polygonOffset
	deformVertexes projectionShadow
	{
		map			*white
		blendFunc GL_ONE GL_ZERO
		rgbGen wave square 0 0 0 0				// just solid black
	}	
}


// wake is the mark on water surfaces for paddling players
wake
{
	{
		map gfx/misc/splash.tga
		clamp
		blend blend
		rgbGen vertex
        rotate 250 * time
        //tcMod stretch sin .9 0.1 0 0.7
		centerScale 1.3 + sinTable[time] * 0.1, 1.3 + cosTable[time] * 0.1
		rgbGen wave sin .7 .3 .25 .5
	}
    {
		map gfx/misc/splash.tga
		clamp
		blend blend
		rgbGen vertex
        rotate -230 * time
        //tcMod stretch sin .9 0.05 0 0.9
		rgbGen wave sin .7 .3 .25 .4
	}
	/*
	{       
      	blend	gl_zero, gl_one_minus_src_color
		map gfx/misc/splash.png
		clamp
		vertexColor
	}
	*/
}


// viewBloodBlend gives the blended directional cue when you get hit
viewBloodBlend
{
	nopicmip			// make sure a border remains
	sort	nearest
	{
		//map models/weaphits/blood201.tga
        	//map gfx/damage/blood_screen.tga
		clampmap textures/decals/blood_splat02.tga
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		blendFunc GL_ZERO GL_ONE_MINUS_SRC_COLOR
		vertexColor
	}
}

waterBubble
{
	sort underwater
	twoSided
	entityMergable		// allow all the sprites to be merged together
	{
		map gfx/misc/bubble.png
		blend GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA
		vertexColor
	}
	/*
	{
		stage	heathazemap
		map		gfx/misc/bubble_n.tga
		deformMagnitude .1
		alphaTest 0.5
	}
	*/
}

smokePuff
{
	twoSided
	entityMergable		// allow all the sprites to be merged together
	{
		map gfx/misc/smoke1.png
		blend GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA
		vertexColor
	}
}

hasteSmokePuff			// drops behind player's feet when speeded
{
	twoSided
	entityMergable		// allow all the sprites to be merged together
	{
		map gfx/misc/smoke4.png
		blend GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA
		vertexColor
	}
}

shotgunSmokePuff
{
	twoSided
	{
		map gfx/misc/smoke2.png
		blend GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA
		vertexColor
	}
}

nailTrail
{
	sort nearest
	twoSided
	
	{
		clamp
		map gfx/misc/smoke2.png 
		blend GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA
		vertexColor
        //rotate time * -30
	}

}

flareShader
{
	twoSided
	{
		map gfx/misc/flare.png
		blend add
		vertexColor
	}
}

sun
{
	twoSided
	{
		map gfx/misc/sun.png
		blend add
		vertexColor
	}
}

railDisc
{
	twoSided
	//deformVertexes wave 100 sin 0 .5 0 2.4
	translucent
	sort postProcess
	{
		stage	heathazemap
		map		gfx/misc/raildisc_mono2_n.png
		clamp
		//highQuality
		deformMagnitude 1
		//tcMod stretch sawtooth 1.4 -10 0 .1
		centerScale 0.6 + (time + parm4) * 2.3 * (1 - parm0) , 0.6 + (time + parm4) * 2.3 * (1 - parm0)
		blend GL_ONE, GL_ZERO
		alphaTest 0.5
		maskDepth	// don't write to depth buffer
					// it would mess up depth buffer based post process effects
      }
}

railRing
{
	twoSided
	translucent
	sort postProcess
	{
		map gfx/misc/raildisc_mono2.png
		clamp
		blend add
      }
}

railCore
{
	sort nearest
	twoSided
	{
		map gfx/misc/railcorethin_mono.tga
		blend add
		//vertexColor
		colored
		scroll time * -2,  0
	}
}

lightningBolt
{
	twoSided
	{
		map gfx/misc/lightning3.tga
		blend GL_ONE , GL_ONE
		rgbGen wave sin 1 0.5 0 7.1
		scale  2 , 1
		scroll time * -5 , 0
	}
	{
		map gfx/misc/lightning3.tga
		blend GL_ONE , GL_ONE
		rgbGen wave sin 1 0.8 0 8.1
		scale  -1.3 , -1
		scroll time * -7.2 , 0
	}
}

// shader used on the occasional machinegun bullet tracers
gfx/misc/tracer
{
	cull none
	{
		map	gfx/misc/tracer.tga
		blend add
	}
}

burnMark
{
	polygonOffset
	{
		map textures/decals/explo_burn01.tga
		blendFunc GL_ZERO GL_ONE_MINUS_SRC_COLOR
		vertexColor
	}
}

gfx/damage/bullet_mrk
{
	polygonOffset

	{
		map gfx/damage/bullet_mrk.tga
		blendFunc GL_ZERO GL_ONE_MINUS_SRC_COLOR
		vertexColor
	}
}

gfx/damage/burn_med_mrk
{
	polygonOffset
	{
		map textures/decals/explo_burn01.tga
		blendFunc GL_ZERO GL_ONE_MINUS_SRC_COLOR
		vertexColor
	}
}

gfx/damage/hole_lg_mrk
{
	polygonOffset
	{
		map gfx/damage/hole_lg_mrk.tga
		blendFunc GL_ZERO GL_ONE_MINUS_SRC_COLOR
		vertexColor
	}
}

gfx/damage/plasma_mrk
{
	polygonOffset
	{
		//map gfx/damage/plasma_mrk.tga
		map textures/decals/particle_burn01.png
		blendFunc GL_ZERO GL_ONE_MINUS_SRC_COLOR
		vertexColor
	}
}


//===============================================================

// scoreboard header tabs

scoreboardName
{
	nopicmip
	nomipmaps
	{
		clampmap menu/tab/name.tga
		blend blend
	}
}

scoreboardScore
{
	nopicmip
	nomipmaps
	{
		clamp
		map menu/tab/score.tga
		blend blend
	}
}

scoreboardTime
{
	nopicmip
	nomipmaps
	{
		clamp
		map menu/tab/time.tga
		blend blend
	}
}

scoreboardPing
{
	nopicmip
	nomipmaps
	{
		clamp
		map menu/tab/ping.tga
		blend blend
	}
}

//===============================================================

gfx/2d/crosshaira
{
	nopicmip
	{
		map gfx/2d/crosshaira.tga          
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA                
        rgbGen identity
	}
}

gfx/2d/crosshairb
{
	nopicmip
	{
		map gfx/2d/crosshairb.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
}

gfx/2d/crosshairc
{
	nopicmip
	{
		map gfx/2d/crosshairc.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
}

gfx/2d/crosshaird
{
	nopicmip
	{
		map gfx/2d/crosshaird.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
}

gfx/2d/crosshaire
{
	nopicmip
	{
		map gfx/2d/crosshaire.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
}

gfx/2d/crosshairf
{
	nopicmip
	{
		map gfx/2d/crosshairf.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
}

gfx/2d/crosshairg
{
	nopicmip
	{
		map gfx/2d/crosshairg.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
}

gfx/2d/crosshairh
{
	nopicmip
	{
		map gfx/2d/crosshairh.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
}

gfx/2d/crosshairi
{
	nopicmip
	{
		map gfx/2d/crosshairi.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}

}
gfx/2d/crosshairj
{
	nopicmip
	{
		map gfx/2d/crosshairj.tga       
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
}
gfx/2d/crosshairk
{
	nopicmip
	{
		map gfx/2d/crosshairk.tga       
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
}




gfx/2d/bigchars
{
	nopicmip
	nomipmaps
	{
		map gfx/2d/charset-bezerk-plain-rc2.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

gfx/2d/bigchars_noDepthTest
{
	nopicmip
	nomipmaps
	{
		map gfx/2d/charset-bezerk-plain-rc2.png
		blend GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
		
	}
}


gfx/2d/chromechars
{
	nopicmip
	nomipmaps
	{
		map gfx/2d/chromechars.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

gfx/2d/weapon_select
{
	nopicmip
	{
		map gfx/2d/weapon_select.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
		rgbgen vertex
	}
}

gfx/2d/select
{
	nopicmip
	{
		map gfx/2d/select.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
		rgbgen vertex
	}
}


gfx/2d/assault1d
{
	nopicmip
	{
		map gfx/2d/assault1d.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
gfx/2d/armor1h
{
	nopicmip
	{
		map gfx/2d/armor1h.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
gfx/2d/health
{
	nopicmip
	{
		map gfx/2d/health.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
gfx/2d/blank
{
	nopicmip
	{
		map gfx/2d/blank.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
gfx/2d/numbers/zero_32b
{
	nopicmip
	{
		map gfx/2d/numbers/zero_32b.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}
gfx/2d/numbers/one_32b
{
	nopicmip
	{
		map gfx/2d/numbers/one_32b.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}
gfx/2d/numbers/two_32b
{
	nopicmip
	{
		map gfx/2d/numbers/two_32b.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}
gfx/2d/numbers/three_32b
{
	nopicmip
	{
		map gfx/2d/numbers/three_32b.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}
gfx/2d/numbers/four_32b
{
	nopicmip
	{
		map gfx/2d/numbers/four_32b.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}
gfx/2d/numbers/five_32b
{
	nopicmip
	{
		map gfx/2d/numbers/five_32b.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}
gfx/2d/numbers/six_32b
{
	nopicmip
	{
		map gfx/2d/numbers/six_32b.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}
gfx/2d/numbers/seven_32b
{
	nopicmip
	{
		map gfx/2d/numbers/seven_32b.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}
gfx/2d/numbers/eight_32b
{
	nopicmip
	{
		map gfx/2d/numbers/eight_32b.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}
gfx/2d/numbers/nine_32b
{
	nopicmip
	{
		map gfx/2d/numbers/nine_32b.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}
gfx/2d/numbers/minus_32b
{
	nopicmip
	{
		map gfx/2d/numbers/minus_32b.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}


//
// weapon hits
// All of these will be put on entities with a shaderTime offset, so they
// can be expected to start at time 0 for each separate explosion
//
// Many of these are use smoothly morphed animating textures by pairing
// sawtooth and inverse sawtooth rgbGens
plasmaExplosion
{
	cull disable
	{
		clampmap models/weaphits/plasmaboom.tga
		blendfunc add
		tcMod stretch triangle .6 0.1 0 8
		tcmod rotate 999
		rgbGen wave inversesawtooth 0 1 0 1.5
	}
}

railExplosion
{
	cull disable
    	{
		animmap 5 models/weaphits/ring02_1.tga  models/weaphits/ring02_2.tga  models/weaphits/ring02_3.tga models/weaphits/ring02_4.tga _black
		alphaGen wave inversesawtooth 0 1 0 5
		blendfunc blend
	}
	{
		animmap 5 models/weaphits/ring02_2.tga  models/weaphits/ring02_3.tga models/weaphits/ring02_4.tga _black _black
		alphaGen wave sawtooth 0 1 0 5
		blendfunc blend
	}
}

lightningExplosion
{
	cull disable
	{
		animmap 5 models/weaphits/ring02_1.tga  models/weaphits/ring02_3.tga  models/weaphits/ring02_3.tga _black
		rgbGen wave inversesawtooth 0 1 0 5
		blendfunc add
	}
	{
		animmap 5 models/weaphits/ring02_2.tga  models/weaphits/ring02_3.tga  _black _black
		rgbGen wave sawtooth 0 1 0 5
		blendfunc add
	}
}

bulletExplosion
{
	cull disable
	{
		animmap 5 models/weaphits/bullet1.tga  models/weaphits/bullet2.tga  models/weaphits/bullet3.tga _black
		rgbGen wave inversesawtooth 0 1 0 5
		blendfunc add
	}
	{
		animmap 5 models/weaphits/bullet2.tga  models/weaphits/bullet3.tga  _black _black
		rgbGen wave sawtooth 0 1 0 5
		blendfunc add
	}
}

rocketExplosion
{
	twoSided
	
	{
		if((time * 13) % 13 == 0)
		map models/weaphits/rlboom_0.png
		blend add
	}
	{
		if((time * 13) % 13 == 1)
		map models/weaphits/rlboom_1.png
		blend add
	}
	{
		if((time * 13) % 13 == 2)
		map models/weaphits/rlboom_2.png
		blend add
	}
	{
		if((time * 13) % 13 == 3)
		map models/weaphits/rlboom_3.png
		blend add
	}
	{
		if((time * 13) % 13 == 4)
		map models/weaphits/rlboom_4.png
		blend add
	}
	{
		if((time * 13) % 13 == 5)
		map models/weaphits/rlboom_5.png
		blend add
	}
	{
		if((time * 13) % 13 == 6)
		map models/weaphits/rlboom_6.png
		blend add
	}
	{
		if((time * 13) % 13 == 7)
		map models/weaphits/rlboom_7.png
		blend add
	}
	{
		if((time * 13) % 13 == 8)
		map models/weaphits/rlboom_8.png
		blend add
	}
	{
		if((time * 13) % 13 == 9)
		map models/weaphits/rlboom_9.png
		blend add
	}
	{
		if((time * 13) % 13 == 10)
		map models/weaphits/rlboom_10.png
		blend add
	}
	{
		if((time * 13) % 13 == 11)
		map models/weaphits/rlboom_11.png
		blend add
	}
	{
		if((time * 13) % 13 == 12)
		map models/weaphits/rlboom_12.png
		blend add
	}
}

grenadeExplosion
{
	twoSided

	{
		if((time * 13) % 13 == 0)
		map models/weaphits/rlboom_0.png
		blend add
	}
	{
		if((time * 13) % 13 == 1)
		map models/weaphits/rlboom_1.png
		blend add
	}
	{
		if((time * 13) % 13 == 2)
		map models/weaphits/rlboom_2.png
		blend add
	}
	{
		if((time * 13) % 13 == 3)
		map models/weaphits/rlboom_3.png
		blend add
	}
	{
		if((time * 13) % 13 == 4)
		map models/weaphits/rlboom_4.png
		blend add
	}
	{
		if((time * 13) % 13 == 5)
		map models/weaphits/rlboom_5.png
		blend add
	}
	{
		if((time * 13) % 13 == 6)
		map models/weaphits/rlboom_6.png
		blend add
	}
	{
		if((time * 13) % 13 == 7)
		map models/weaphits/rlboom_7.png
		blend add
	}
	{
		if((time * 13) % 13 == 8)
		map models/weaphits/rlboom_8.png
		blend add
	}
	{
		if((time * 13) % 13 == 9)
		map models/weaphits/rlboom_9.png
		blend add
	}
	{
		if((time * 13) % 13 == 10)
		map models/weaphits/rlboom_10.png
		blend add
	}
	{
		if((time * 13) % 13 == 11)
		map models/weaphits/rlboom_11.png
		blend add
	}
	{
		if((time * 13) % 13 == 12)
		map models/weaphits/rlboom_12.png
		blend add
	}
}

bfgExplosion
{
	cull disable
	{
		animmap 5 models/weaphits/bfgboom/bfgboom_1.tga  models/weaphits/bfgboom/bfgboom_2.tga models/weaphits/bfgboom/bfgboom_3.tga
		rgbGen wave inversesawtooth 0 1 0 5
		blendfunc add
	}
	{
		animmap 5 models/weaphits/bfgboom/bfgboom_2.tga models/weaphits/bfgboom/bfgboom_3.tga _black
		rgbGen wave sawtooth 0 1 0 5
		blendfunc add
	}
}

bloodExplosion		// spurt of blood at point of impact
{
	cull disable
	{
		animmap 5 models/weaphits/blood201.tga models/weaphits/blood202.tga models/weaphits/blood203.tga models/weaphits/blood204.tga models/weaphits/blood205.tga
		blendfunc blend
	}
}

nailTrail
{
	sort nearest
	cull none
	{
		clampmap models/weaphits/nailtrail.tga 
		blendFunc Add
		rgbGen vertex
        tcMod rotate -30
	}
}

debugPlayerAABB
{
	nopicmip

	{
		map gfx/misc/aabb.tga
		blend add
		rgbGen vertex
	}
}

debugPlayerAABB_twoSided
{
	nopicmip
	twoSided

	{
		map gfx/misc/aabb.tga
		blend add
		rgbGen vertex
	}
}

powerups/redflag
{
	deformVertexes wave 100 sin 3 0 0 0
	{
		map textures/effects/redflagmap.tga
		blendfunc GL_ONE GL_ONE
		tcGen environment
		tcMod turb 0 0.2 0 1
	}
}

powerups/blueflag
{
	deformVertexes wave 100 sin 3 0 0 0
	{
		map textures/effects/blueflagmap.tga
		blendfunc GL_ONE GL_ONE
		tcGen environment
		tcMod turb 0 0.2 0 1
	}
}


