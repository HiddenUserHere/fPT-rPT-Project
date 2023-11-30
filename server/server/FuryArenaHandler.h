#pragma once

#define _X_KINGFURY			-928739
#define _Y_KINGFURY			124461
#define _Z_KINGFURY			-11049304

#define _X_LEAVEFURYARENA	-928749
#define _Z_LEAVEFURYARENA	-10776389


#define _BOSS_FURYARENA					"Hagios"
#define _BOSS_FURYARENALOW				"King Fury"
#define _MINION_FURYARENA				"Fury Worm"
#define _MINION_FURYARENALOW			"B-Machine"
#define _MONSTEREVENT_FURYARENA			"Wanted Wolf"
#define _MONSTEREVENT_FURYARENALOW		"Wanted Wolf"
#define _MINION_FURYARENA_CNT 6

// Minions Position
static const Point3D MinionsFuryArena[6]
{
	// X,				Y,              Z
	{ -927741,			0,				-10975789 },
	{ -926409,			0,				-11125981 },
	{ -862940,			0,				-11013207 },
	{ -870972,			0,				-11100081 },
	{ -986155,			0,				-11094362 },
	{ -992901,			0,				-11017736 },
};

class CFuryArenaHandler
{
private:

	std::vector<SPrizeData*>							vaPrizesReward[4];
	int													iaTotalPercent[4];

	BOOL												KillUnits();

	SPrizeData											* GetRewardItem( int iID );

public:
	CFuryArenaHandler();
	virtual ~CFuryArenaHandler();

	int													GetEventID();

	UINT64												GetTimeLeft( SYSTEMTIME * ps );

	DWORD												GetTick() { return dwTickTime; }

	BOOL												InsideArena( Point3D * psPosition );

	int													GetTotalUsers();

	BOOL												IsStarted() { return bStarted; };
	BOOL												IsEnabled() { return bEnabled; };
	BOOL												IsBoss( UnitData * pcUnitData );
	void												SetEnable( BOOL bVal ) { bEnabled = bVal; };

	void												SQLLog( char * pszLog, ... );

	BOOL												ExchangeItem( User * pcUser, PacketFuryArenaRequest * psPacket );
	
	BOOL												SetBoss( const char * szMonsterName );
	BOOL												SetMinion( const char * szMonsterName );
	BOOL												SetMonsterEvent( const char * szMonsterName );

	BOOL												UpdateTime();
	BOOL												SetTimeMinion( int iMinute = 1 );

	BOOL												StartEvent();
	BOOL												EndEvent();
	BOOL												LeaveFromArena();
	BOOL												LeaveFromArena( User * pcUser );
	BOOL												OnDisconnected( User * pcUser );

	BOOL												OnUnitKilled( UserData * pcUserData, UnitData * pcUnitData );

	void												PacketHandlerData( User * pcUser, PacketFuryArenaRequest * psPacket );
	BOOL												EnterFromArena( User * pcUser );

	void												Meteorite();

	void												UpdateHPBoss();

	void												Tick();

	void												Init();

protected:
	int													iEventID = 0;
	int													iEventIDNow = 0;


	BOOL												bCanEnter		= FALSE;
	BOOL												bEnabled		= FALSE;
	BOOL												bStarted		= FALSE;
	BOOL												bAlert1Start	= FALSE;
	BOOL												bAlert2Start	= FALSE;
	BOOL												bAlert3Start	= FALSE;
	BOOL												bAlert4Start	= FALSE;
	WORD												wDayEvent = 0;
	WORD												wMinuteMinionSpawn = 0;
	int													iHourEvent		= 21;
	int													iMinuteEvent	= 30;
	int													iMinuteSpawnMinion = 1;

	DWORD												dwEventLeaveTime = 0;
	DWORD												dwTickTime = 0;
	DWORD												dwStarted = 0;
	DWORD												dwMeteor = 0;

	char												szBossName[2][32];
	char												szMinionName[2][32];
	char												szMonsterEventName[2][32];

	UnitData											* pcUnitDataMinions[_MINION_FURYARENA_CNT];
	UnitData											* pcUnitDataBoss = NULL;
};

