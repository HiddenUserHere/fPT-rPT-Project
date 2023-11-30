#pragma once

#define MAX_ACTIONFIELD_UNITS_SPAWN		10

#define TIME_ACTIONFIELD_LEAVE_FINISHED	20

enum EActionFieldUnitType : int
{
	ACTIONFIELDUNIT_None,
	ACTIONFIELDUNIT_Friendly	= 500,
	ACTIONFIELDUNIT_Enemy		= 750,
};

struct SQLActionFieldInstance
{
	int						iID;

	int						iInstanceID;
	EActionFieldModeID		iModeID;

	int						iAccountID;
	int						iCharacterID;
	int						iCharacterClass;

	char					szIP[32];

	SYSTEMTIME				sDate;

	int						iTotalScore;
	int						iTotalKills;

	int						iLastRound;
};

class CActionFieldInstance
{
public:
	int						iTimeUpdate;
	int						iTimeFinished;

	CActionFieldInstance();
	virtual ~CActionFieldInstance();

	CActionField			* GetActionField() { return pcActionField; }
	void					SetActionField( CActionField * pc ) { pcActionField = pc; }

	CRoomInstance			* GetRoom() { return pcRoom; }
	void					SetRoom( CRoomInstance * pc ) { pcRoom = pc; }

	BOOL					IsStarted() { return bStarted; }

	BOOL					Delete() { return bDelete; }
	void					Delete( BOOL b ) { bDelete = b; }


	void					Start() { bStarted = TRUE; }
	void					Stop() { bStarted = FALSE; iMonstersAlive = 0; iMonstersKilledRound = 0; }

	int						GetRound() { return iRound; }
	void					SetRound( int i ) { iRound = i; }

	int						GetTotalKills() { return iMonstersKilled; }
	void					SetTotalKills( int i ) { iMonstersKilled = i; }

	int						GetRoundKills() { return iMonstersKilledRound; }
	void					SetRoundKills( int i ) { iMonstersKilledRound = i; }

	int						GetMonstersAlive() { return iMonstersAlive; }
	void					SetMonstersAlive( int i );

	void					SetBossSpawn( BOOL b ) { bSpawnBoss = b; }
	int						GetBossSpawn() { return bSpawnBoss; }

	void					SetSentPrizes( BOOL b ) { bSentPrizes = b; }
	BOOL					IsSentPrizes() { return bSentPrizes; }

private:
	BOOL					bStarted;

	BOOL					bDelete;

	CActionField			* pcActionField;
	CRoomInstance			* pcRoom;

	int						iRound;

	int						iMonstersKilled;
	int						iMonstersKilledRound;
	int						iMonstersAlive;

	BOOL					bSpawnBoss;

	BOOL					bSentPrizes;

	int						iUsersCount;
	ActionFieldUser			saUsers[6];
};

class CActionFieldHandler
{
public:
	CActionFieldHandler();
	virtual ~CActionFieldHandler();

	CActionFieldInstance	* GetInstanceRoom( CRoomInstance * pcRoom );

	void					Init();

	void					LoadActionField( EActionFieldModeID iModeID );

	void					Update();

	BOOL					OnUnitKilled( User * pcUser, Unit * pcUnit );

	void					HandlePacket( User * pcUser, PacketActionFieldRequest * psPacket );
	void					HandlePacket( User * pcUser, PacketActionFieldExchange * psPacket );
	void					HandleNetPacket( PacketNetActionFieldRequest * psPacket );

	void					RequestTest( User * pcUser );

	void					OnDisconnectedUser( User * pcUser );

	void					OnEnterParty( User * pcUser );
	void					OnLeaveParty( User * pcUser );
	void					OnLeaveMap( User * pcUser, int iMapID );

	BOOL					IsOnInstance( Unit * pcUnit );

	CActionField			* GetActionField( EActionFieldModeID iModeID );

	int						SQLGetInstanceTodayCount( User * pcUser, EActionFieldModeID iModeID );

private:
	const int				MAX_CHEST_ITEMBOX = 1;


	std::vector<CActionField*>			vActionFields;
	std::vector<CActionFieldInstance*>	vInstances;

	UINT					iActionFieldWheel;

	void					Delete( EActionFieldModeID iModeID );

	BOOL					SQLRewardLog( EActionFieldModeID iModeID, int iChestTypeID, User * pcUser, ActionFieldReward * psReward );
	BOOL					SQLInstanceLog( EActionFieldModeID iModeID, User * pcUser, const char * pszDescription );
	BOOL					SQLGetInstance( User * pcUser, SQLActionFieldInstance * psInstance );


	EMapID					GetMapID( EActionFieldModeID iModeID );

	bool					GetEntranceXZ( EActionFieldModeID iModeID, Point3D & sEntrance );

	void					UpdateMonsterSpawn( CActionFieldInstance * pcInstance );
	void					UpdatePrizes( CActionFieldInstance * pcInstance );
	void					UpdateRound( CActionFieldInstance * pcInstance );

	void					ProcessPacket( CActionFieldInstance * pcInstance, PacketActionFieldEnter * psPacket );
	void					ProcessPacket( CActionFieldInstance * pcInstance, PacketActionFieldFail * psPacket );
	void					ProcessPacket( CActionFieldInstance * pcInstance, PacketActionFieldUpdate * psPacket );
	void					ProcessPacket( CActionFieldInstance * pcInstance, PacketActionFieldStart * psPacket );
	void					ProcessPacket( CActionFieldInstance * pcInstance, PacketActionFieldEnd * psPacket );

	void					LeaveUsers( CActionFieldInstance * pcInstance );

	bool					HaveChestItemBox( User * pcUser, EItemID eItemID );

	void					Request( User * pcUser, EActionFieldModeID iModeID );

	bool					CanEnter( User * pcUser, EActionFieldModeID iModeID );

	void					SaveInstance( CActionFieldInstance * pcInstance );

	void					SpawnMonster( CActionFieldInstance * pcInstance, ActionFieldMonster * psMonster, int iIndex = 0 );

	void					KillUnits( CActionFieldInstance * pcInstance );

	void					SendChat( CActionFieldInstance * pcInstance, EChatColor eColor, const char * pszText );
};

