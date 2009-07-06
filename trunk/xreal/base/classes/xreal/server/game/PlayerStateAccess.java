package xreal.server.game;

import xreal.common.PlayerMovementType;

//playerState_t is the information needed by both the client and server
//to predict player motion and actions
//nothing outside of pmove should modify these, or some degree of prediction error
//will occur

//you can't add anything to this without modifying the code in msg.c

//playerState_t is a full superset of entityState_t as it is used by players,
//so if a playerState_t is transmitted, the entityState_t can be fully derived
//from it.
/*
 typedef struct playerState_s
 {
 int             commandTime;	// cmd->serverTime of last executed command
 int             pm_type;
 int             bobCycle;	// for view bobbing and footstep generation
 int             pm_flags;	// ducked, jump_held, etc
 int             pm_time;

 vec3_t          origin;
 vec3_t          velocity;
 int             weaponTime;
 int             gravity;
 int             speed;
 int             delta_angles[3];	// add to command angles to get view direction
 // changed by spawns, rotating objects, and teleporters

 int             groundEntityNum;	// ENTITYNUM_NONE = in air

 int             legsTimer;	// don't change low priority animations until this runs out
 int             legsAnim;	// mask off ANIM_TOGGLEBIT

 int             torsoTimer;	// don't change low priority animations until this runs out
 int             torsoAnim;	// mask off ANIM_TOGGLEBIT

 int             movementDir;	// a number 0 to 7 that represents the relative angle
 // of movement to the view angle (axial and diagonals)
 // when at rest, the value will remain unchanged
 // used to twist the legs during strafing

 vec3_t          grapplePoint;	// location of grapple to pull towards if PMF_GRAPPLE_PULL

 int             eFlags;		// copied to entityState_t->eFlags

 int             eventSequence;	// pmove generated events
 int             events[MAX_PS_EVENTS];
 int             eventParms[MAX_PS_EVENTS];

 int             externalEvent;	// events set on player from another source
 int             externalEventParm;
 int             externalEventTime;

 int             clientNum;	// ranges from 0 to MAX_CLIENTS-1
 int             weapon;		// copied to entityState_t->weapon
 int             weaponstate;

 vec3_t          viewangles;	// for fixed views
 int             viewheight;

 // damage feedback
 int             damageEvent;	// when it changes, latch the other parms
 int             damageYaw;
 int             damagePitch;
 int             damageCount;

 int             stats[MAX_STATS];
 int             persistant[MAX_PERSISTANT];	// stats that aren't cleared on death
 int             powerups[MAX_POWERUPS];	// level.time that the powerup runs out
 int             ammo[MAX_WEAPONS];

 int             generic1;
 int             loopSound;
 int             jumppad_ent;	// jumppad entity hit this frame

 // not communicated over the net at all
 int             ping;		// server to game info for scoreboard
 int             pmove_framecount;	// FIXME: don't transmit over the network
 int             jumppad_frame;
 int             entityEventSequence;
 } playerState_t;
 */

public interface PlayerStateAccess {
	public void setPlayerState_commandTime(int time);

	public void setPlayerState_pm_type(PlayerMovementType type);

	public void setPlayerState_pm_flags(int flags);

	public void setPlayerState_pm_time(int time);
}
