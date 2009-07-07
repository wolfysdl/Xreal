package xreal.server.game;

import javax.vecmath.Vector3f;

import xreal.ConsoleColorStrings;
import xreal.Engine;
import xreal.PlayerStateAccess;
import xreal.UserCommand;
import xreal.UserInfo;
import xreal.common.ConfigStrings;
import xreal.common.GameType;
import xreal.common.PlayerMovementType;
import xreal.common.Team;
import xreal.server.Server;

/**
 * Represents, uses and writes to a native gclient_t
 * 
 * @author Robert Beckebans
 */
public class Player extends GameEntity implements ClientListener, PlayerStateAccess {
	
	UserInfo _userInfo = new UserInfo();
	
	private ClientPersistant	_pers = new ClientPersistant();
	private ClientSession		_sess = new ClientSession();
	private int					_lastCmdTime; 
	
	private static native String	getUserInfo0(int clientNum);
	private static native void 		setUserInfo0(int clientNum, String s);
	
	
	
	
	// --------------------------------------------------------------------------------------------
	
	
	Player(int clientNum, boolean firstTime, boolean isBot) throws GameException
	{
		super(clientNum);
		
		_sess.sessionTeam = Team.SPECTATOR;
		
		String userinfo = getUserInfo0(clientNum);
		if(userinfo.length() == 0)
			userinfo = "\\name\\badinfo";
		
		clientUserInfoChanged(userinfo);
		
		// IP filtering
		// https://zerowing.idsoftware.com/bugzilla/show_bug.cgi?id=500
		// recommanding PB based IP / GUID banning, the builtin system is pretty limited
		// check to see if they are on the banned IP list
		String ip = _userInfo.get("ip");
		/*
		if(G_FilterPacket(ip))
		{
			throw new GameException("You are banned from this server.");
		}
		*/

		// we don't check password for bots and local client
		// NOTE: local client <-> "ip" "localhost"
		//   this means this client is not running in our current process
		if(!isBot && ip != null && !ip.equals("localhost"))
		{
			// check for a password
			String userPassword = _userInfo.get("password");
			String requiredPassword = CVars.g_password.getString();
			
			if(requiredPassword != null && requiredPassword.length() > 0 && !requiredPassword.equals(userPassword))
			{
				throw new GameException("Invalid password");
			}
		}

		_pers.connected = ClientConnectionState.CONNECTING;
		

		// read or initialize the session data
		/*
		if(firstTime || level.newSession)
		{
			G_InitSessionData(client, userinfo);
		}
		G_ReadSessionData(client);
		*/
		
		//throw new GameException("Connection refused");
	}

	@Override
	public void clientBegin() {
		Engine.print("xreal.server.game.Player.clientBegin(clientNum = " + getEntityIndex() + ")\n");
		
		_pers.connected = ClientConnectionState.CONNECTED;
		_pers.enterTime = Game.getLevelTime();
		
		//_pers.teamState.state = Team.BEGIN;

	}

	@Override
	public void clientCommand() {
		Engine.print("xreal.server.game.Player.clientCommand(clientNum = " + getEntityIndex() + ")\n");
	}
	
	@Override
	public void clientDisconnect() {
		Engine.print("xreal.server.game.Player.clientDisconnect(clientNum = " + getEntityIndex() + ")\n");
	}

	@Override
	public void clientThink(UserCommand ucmd) {
		//Engine.print("xreal.server.game.Player.clientThink(clientNum = " + getEntityIndex() + ")\n");
		
		//Engine.println(ucmd.toString());
		
		// mark the time we got info, so we can display the
		// phone jack if they don't get any for a while
		_lastCmdTime = Game.getLevelTime();
		
		if(CVars.g_synchronousClients.getBoolean())
		{
			//ClientThink_real(ent);
			
			// shut up client about outdated player states
			setPlayerState_commandTime(ucmd.serverTime);
			
			setPlayerState_pm_type(PlayerMovementType.SPECTATOR);
		}
	}

	/**
	 * Called from Player() when the player first connects and
	 * directly by the server system when the player updates a userinfo variable.
	 * 
	 * The game can override any of the settings and call Player.setUserinfo
	 * if desired.
	 * 
	 * @param userinfo
	 *            the userinfo string, formatted as:
	 *            "\keyword\value\keyword\value\....\keyword\value"
	 */
	@Override
	public void clientUserInfoChanged(String userinfo) {
		Engine.print("xreal.server.game.Player.clientUserInfoChanged(clientNum = " + getEntityIndex() + ")\n");

		if (userinfo == null)
			return;

		// fill and update the user info hash table
		_userInfo.read(userinfo);

		//Engine.println("Player.userinfo = " + _userInfo.toString());

		// check for local client
		String ip = _userInfo.get("ip");
		if (ip.equals("localhost")) {
			_pers.localClient = true;
		}

		// set name
		String oldname = _pers.netname;
		String name = _userInfo.get("name");

		// TODO _pers.netname = ClientCleanName(name);
		_pers.netname = name;

		if (_sess.sessionTeam == Team.SPECTATOR) {
			if (_sess.spectatorState == SpectatorState.SCOREBOARD) {
				_pers.netname = "scoreboard";
			}
		}

		if (_pers.connected == ClientConnectionState.CONNECTED) {
			if (!_pers.netname.equals(oldname)) {
				Server.broadcastServerCommand("print \"" + oldname + ConsoleColorStrings.WHITE + " renamed to " + _pers.netname + "\n\"");
			}
		}
		
		// set model
		String model = _userInfo.get("model");

		// bots set their team a few frames later
		Team team = _sess.sessionTeam;
		
		GameType gt = GameType.values()[CVars.g_gametype.getInteger()];
		if((gt == GameType.TEAM || gt == GameType.CTF || gt == GameType.ONEFLAG || gt == GameType.OBELISK || gt == GameType.HARVESTER) /* && g_entities[clientNum].r.svFlags & SVF_BOT */)
		{
			/*
			s = Info_ValueForKey(userinfo, "team");
			if(!Q_stricmp(s, "red") || !Q_stricmp(s, "r"))
			{
				team = TEAM_RED;
			}
			else if(!Q_stricmp(s, "blue") || !Q_stricmp(s, "b"))
			{
				team = TEAM_BLUE;
			}
			else
			{
				// pick the team with the least number of players
				team = PickTeam(clientNum);
			}
			*/
		}
		
		// team task (0 = none, 1 = offence, 2 = defence)
		String teamTask = _userInfo.get("teamtask");
		
		
		// team Leader (1 = leader, 0 is normal player)
		boolean teamLeader = _sess.teamLeader;
		

		// colors
		String c1 = _userInfo.get("color1");
		String c2 = _userInfo.get("color2");

		/*
		 * Com_sprintf(userinfo, sizeof(userinfo),
		 * "n\\%s\\t\\%i\\model\\%s\\hmodel\\%s\\g_redteam\\%s\\g_blueteam\\%s\\c1\\%s\\c2\\%s\\hc\\%i\\w\\%i\\l\\%i\\tt\\%d\\tl\\%d"
		 * , client->pers.netname, team, model, "", redTeam, blueTeam, c1, c2,
		 * client->pers.maxHealth, client->sess.wins, client->sess.losses,
		 * teamTask, teamLeader);
		 */
		
		// build new user info CG_NewClientInfo
		
		UserInfo uinfo = new UserInfo();
		uinfo.put("n", _pers.netname);
		uinfo.put("t",  team.toString());
		uinfo.put("model", model);
		uinfo.put("hmodel", "");
		uinfo.put("g_redteam", "");
		uinfo.put("g_redteam", "");
		uinfo.put("c1", c1);
		uinfo.put("c2", c2);
		uinfo.put("hc", _pers.maxHealth);
		uinfo.put("w", _sess.wins);
		uinfo.put("l", _sess.losses);
		uinfo.put("tt", teamTask);
		uinfo.put("tl", teamLeader);
		
		//Engine.println("CS_PLAYERS userinfo = '" + uinfo.toString() + "'");
		
		Server.setConfigString(ConfigStrings.PLAYERS + getEntityIndex(), uinfo.toString());
		
	}
	
	// ------------------- playerState_t:: fields in gclient_t::ps --------------------------------
	
	private synchronized static native int getPlayerState_commandTime(int clientNum);

	private synchronized static native void setPlayerState_commandTime(int clientNum, int commandTime);

	private synchronized static native int getPlayerState_pm_type(int clientNum);

	private synchronized static native void setPlayerState_pm_type(int clientNum, int pm_type);

	private synchronized static native int getPlayerState_pm_flags(int clientNum);

	private synchronized static native void setPlayerState_pm_flags(int clientNum, int pm_flags);
	
	private synchronized static native void addPlayerState_pm_flags(int clientNum, int pm_flags);
	
	private synchronized static native void delPlayerState_pm_flags(int clientNum, int pm_flags);
	
	private synchronized static native boolean hasPlayerState_pm_flags(int clientNum, int pm_flags);

	private synchronized static native int getPlayerState_pm_time(int clientNum);

	private synchronized static native void setPlayerState_pm_time(int clientNum, int pm_time);

	private synchronized static native int getPlayerState_bobCycle(int clientNum);

	private synchronized static native void setPlayerState_bobCycle(int clientNum, int bobCycle);

	private synchronized static native Vector3f getPlayerState_origin(int clientNum);

	private synchronized static native void setPlayerState_origin(int clientNum, Vector3f origin);

	private synchronized static native Vector3f getPlayerState_velocity(int clientNum);

	private synchronized static native void setPlayerState_velocity(int clientNum, Vector3f velocity);

	private synchronized static native int getPlayerState_weaponTime(int clientNum);

	private synchronized static native void setPlayerState_weaponTime(int clientNum, int weaponTime);

	private synchronized static native int getPlayerState_gravity(int clientNum);

	private synchronized static native void setPlayerState_gravity(int clientNum, int gravity);

	private synchronized static native int getPlayerState_speed(int clientNum);

	private synchronized static native void setPlayerState_speed(int clientNum, int speed);

	private synchronized static native int getPlayerState_deltaPitch(int clientNum);

	private synchronized static native void setPlayerState_deltaPitch(int clientNum, int deltaPitch);

	private synchronized static native int getPlayerState_deltaYaw(int clientNum);

	private synchronized static native void setPlayerState_deltaYaw(int clientNum, int deltaYaw);

	private synchronized static native int getPlayerState_deltaRoll(int clientNum);

	private synchronized static native void setPlayerState_deltaRoll(int clientNum, int deltaRoll);

	private synchronized static native int getPlayerState_groundEntityNum(int clientNum);

	private synchronized static native void setPlayerState_groundEntityNum(int clientNum, int groundEntityNum);

	private synchronized static native int getPlayerState_legsTimer(int clientNum);

	private synchronized static native void setPlayerState_legsTimer(int clientNum, int legsTimer);

	private synchronized static native int getPlayerState_legsAnim(int clientNum);

	private synchronized static native void setPlayerState_legsAnim(int clientNum, int legsAnim);

	private synchronized static native int getPlayerState_torsoTimer(int clientNum);

	private synchronized static native void setPlayerState_torsoTimer(int clientNum, int torsoTimer);

	private synchronized static native int getPlayerState_torsoAnim(int clientNum);

	private synchronized static native void setPlayerState_torsoAnim(int clientNum, int torsoAnim);

	private synchronized static native int getPlayerState_movementDir(int clientNum);

	private synchronized static native void setPlayerState_movementDir(int clientNum, int movementDir);

	private synchronized static native Vector3f getPlayerState_grapplePoint(int clientNum);

	private synchronized static native void setPlayerState_grapplePoint(int clientNum, Vector3f grapplePoint);

	private synchronized static native int getPlayerState_eFlags(int clientNum);

	private synchronized static native void setPlayerState_eFlags(int clientNum, int flags);

	private synchronized static native int getPlayerState_eventSequence(int clientNum);

	private synchronized static native void setPlayerState_eventSequence(int clientNum, int eventSequence);

	private synchronized static native int getPlayerState_externalEvent(int clientNum);

	private synchronized static native void setPlayerState_externalEvent(int clientNum, int externalEvent);

	private synchronized static native int getPlayerState_externalEventParm(int clientNum);

	private synchronized static native void setPlayerState_externalEventParm(int clientNum, int externalEventParm);

	private synchronized static native int getPlayerState_externalEventTime(int clientNum);

	private synchronized static native void setPlayerState_externalEventTime(int clientNum, int externalEventTime);

//	private synchronized static native int getPlayerState_clientNum(int clientNum);

//	private synchronized static native void setPlayerState_clientNum(int clientNum, int clientNum);

	private synchronized static native int getPlayerState_weapon(int clientNum);

	private synchronized static native void setPlayerState_weapon(int clientNum, int weapon);

	private synchronized static native int getPlayerState_weaponState(int clientNum);

	private synchronized static native void setPlayerState_weaponState(int clientNum, int weaponState);

	private synchronized static native Vector3f getPlayerState_viewAngles(int clientNum);

	private synchronized static native void setPlayerState_viewAngles(int clientNum, Vector3f viewAngles);
	
	private synchronized static native void setPlayerState_viewAngles(int clientNum, float pitch, float yaw, float roll);

	private synchronized static native int getPlayerState_viewHeight(int clientNum);

	private synchronized static native void setPlayerState_viewHeight(int clientNum, int viewHeight);

	private synchronized static native int getPlayerState_damageEvent(int clientNum);

	private synchronized static native void setPlayerState_damageEvent(int clientNum, int damageEvent);

	private synchronized static native int getPlayerState_damageYaw(int clientNum);

	private synchronized static native void setPlayerState_damageYaw(int clientNum, int damageYaw);

	private synchronized static native int getPlayerState_damagePitch(int clientNum);

	private synchronized static native void setPlayerState_damagePitch(int clientNum, int damagePitch);

	private synchronized static native int getPlayerState_damageCount(int clientNum);

	private synchronized static native void setPlayerState_damageCount(int clientNum, int damageCount);

	private synchronized static native int getPlayerState_generic1(int clientNum);

	private synchronized static native void setPlayerState_generic1(int clientNum, int generic1);

	private synchronized static native int getPlayerState_loopSound(int clientNum);

	private synchronized static native void setPlayerState_loopSound(int clientNum, int loopSound);

	private synchronized static native int getPlayerState_jumppad_ent(int clientNum);

	private synchronized static native void setPlayerState_jumppad_ent(int clientNum, int jumppad_ent);

	private synchronized static native int getPlayerState_ping(int clientNum);

	private synchronized static native void setPlayerState_ping(int clientNum, int ping);
	
	@Override
	public int getPlayerState_bobCycle() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public int getPlayerState_clientNum() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public int getPlayerState_commandTime() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public int getPlayerState_damageCount() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public int getPlayerState_damageEvent() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public int getPlayerState_damagePitch() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public int getPlayerState_damageYaw() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public int getPlayerState_deltaPitch() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public int getPlayerState_deltaRoll() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public int getPlayerState_deltaYaw() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public int getPlayerState_eFlags() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public int getPlayerState_eventSequence() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public int getPlayerState_externalEvent() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public int getPlayerState_externalEventParm() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public int getPlayerState_externalEventTime() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public int getPlayerState_generic1() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public Vector3f getPlayerState_grapplePoint() {
		// TODO Auto-generated method stub
		return null;
	}
	@Override
	public int getPlayerState_gravity() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public int getPlayerState_groundEntityNum() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public int getPlayerState_jumppad_ent() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public int getPlayerState_legsAnim() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public int getPlayerState_legsTimer() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public int getPlayerState_loopSound() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public int getPlayerState_movementDir() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public Vector3f getPlayerState_origin() {
		// TODO Auto-generated method stub
		return null;
	}
	@Override
	public int getPlayerState_ping() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public int getPlayerState_pm_flags() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public int getPlayerState_pm_time() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public PlayerMovementType getPlayerState_pm_type() {
		// TODO Auto-generated method stub
		return null;
	}
	@Override
	public int getPlayerState_speed() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public int getPlayerState_torsoAnim() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public int getPlayerState_torsoTimer() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public Vector3f getPlayerState_velocity() {
		// TODO Auto-generated method stub
		return null;
	}
	@Override
	public Vector3f getPlayerState_viewAngles() {
		// TODO Auto-generated method stub
		return null;
	}
	@Override
	public int getPlayerState_viewHeight() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public int getPlayerState_weapon() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public int getPlayerState_weaponState() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public int getPlayerState_weaponTime() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public void setPlayerState_bobCycle(int bobCycle) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void setPlayerState_clientNum(int clientNum) {
		// TODO Auto-generated method stub
		
	}
	
	@Override
	public void setPlayerState_commandTime(int time) {
		setPlayerState_commandTime(getEntityIndex(), time);
	}
	
	@Override
	public void setPlayerState_damageCount(int damageCount) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void setPlayerState_damageEvent(int damageEvent) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void setPlayerState_damagePitch(int damagePitch) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void setPlayerState_damageYaw(int damageYaw) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void setPlayerState_deltaPitch(int deltaPitch) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void setPlayerState_deltaRoll(int deltaRoll) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void setPlayerState_deltaYaw(int deltaYaw) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void setPlayerState_eFlags(int flags) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void setPlayerState_eventSequence(int eventSequence) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void setPlayerState_externalEvent(int externalEvent) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void setPlayerState_externalEventParm(int externalEventParm) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void setPlayerState_externalEventTime(int externalEventTime) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void setPlayerState_generic1(int generic1) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void setPlayerState_grapplePoint(Vector3f grapplePoint) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void setPlayerState_gravity(int gravity) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void setPlayerState_groundEntityNum(int groundEntityNum) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void setPlayerState_jumppad_ent(int jumppad_ent) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void setPlayerState_legsAnim(int legsAnim) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void setPlayerState_legsTimer(int legsTimer) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void setPlayerState_loopSound(int loopSound) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void setPlayerState_movementDir(int movementDir) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void setPlayerState_origin(Vector3f origin) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void setPlayerState_ping(int ping) {
		// TODO Auto-generated method stub
		
	}
	
	@Override
	public void setPlayerState_pm_flags(int flags) {
		setPlayerState_pm_flags(getEntityIndex(), flags);
	}

	@Override
	public void setPlayerState_pm_time(int time) {
		setPlayerState_pm_time(getEntityIndex(), time);
	}

	@Override
	public void setPlayerState_pm_type(PlayerMovementType type) {
		setPlayerState_pm_type(getEntityIndex(), type.ordinal());
	}
	
	@Override
	public void setPlayerState_speed(int speed) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void setPlayerState_torsoAnim(int torsoAnim) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void setPlayerState_torsoTimer(int torsoTimer) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void setPlayerState_velocity(Vector3f velocity) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void setPlayerState_viewAngles(Vector3f viewAngles) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void setPlayerState_viewAngles(float pitch, float yaw, float roll) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void setPlayerState_viewHeight(int viewHeight) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void setPlayerState_weapon(int weapon) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void setPlayerState_weaponState(int weaponState) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void setPlayerState_weaponTime(int weaponTime) {
		// TODO Auto-generated method stub
		
	}
	
	@Override
	public void addPlayerState_pm_flags(int pm_flags) {
		setPlayerState_pm_flags(getPlayerState_pm_flags() | pm_flags);
	}
	
	@Override
	public void delPlayerState_pm_flags(int pm_flags) {
		setPlayerState_pm_flags(getPlayerState_pm_flags() & ~pm_flags);
	}
	
	@Override
	public boolean hasPlayerState_pm_flags(int pm_flags) {
		return (getPlayerState_pm_flags() & pm_flags) > 0;
	}
	
	// --------------------------------------------------------------------------------------------
}