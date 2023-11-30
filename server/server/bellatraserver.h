#pragma once

#define BELLATRASERVER_MAX_ROUNDS			8

static int iaBellatraRoundRoomTable[4][8] = {
	{ 0,	0,	0,	4,	4,	4,	6,	6 },
	{ 1,	1,	1,	4,	4,	4,	6,	6 },
	{ 2,	2,	2,	5,	5,	5,	6,	6 },
	{ 3,	3,	3,	5,	5,	5,	6,	6 }
};

static int iaBellatraRoundTable[8][2] = {
	{ 0,	4 },		//1
	{ 0,	4 },		//2
	{ 0,	4 },		//3
	{ 4,	6 },		//4
	{ 4,	6 },		//5
	{ 4,	6 },		//6
	{ 6,	7 },		//7
	{ 6,	7 }		//8
};

static Point3D saBellatraSpawnPoints[7][3] = {
	{	
		{ 18967,  1154,	3055 },
		{ 19995,  1154,	2134 },
		{ 19485,  1154,	2615 }	
	},
	{
		{ 20838,  1154,	3024 },
		{ 21671,  1154,	2193 },
		{ 21255,  1154,	2615 }	
	},
	{
		{ 22478,  1154,	3049 },
		{ 23437,  1154,	2135 },
		{ 22948,  1154,	2615 }
	},
	{
		{ 24204,  1154,	3094 },
		{ 25173,  1154,	2135 },
		{ 24674,  1154,	2615 }
	},
	{	
		{ 20509,  1621,	5284 },
		{ 21544,  1621,	4261 },
		{ 21012,  1621,	4773 }	
	},
	{
		{ 22575,  1621,	5253 },
		{ 23610,  1621,	4230 },
		{ 23077,  1621,	4742 }	
	},
	{	
		{ 21538,  1638,	7057 },
		{ 22527,  1638,	6079 },
		{ 22018,  1638,	6569 }	
	}
};


struct BellatraData
{
	struct BellatraUnitData
	{
		char			szName[32];
		CharacterData	* psCharacterData;
		int				iPercent;
	};

	BellatraUnitData	saMonster[4];
	BellatraUnitData	sBoss;

	int					iPercent;
	int					iHour;
	BOOL				iPoints;
	int					iMaxMonsters;
	int					iDelayTime;
};


struct BellatraMonsterSpawnHandle
{
	struct SSpawn
	{
		BOOL				iCount;
	};

	SSpawn					saRound[8];
};


class CBellatraRoom
{
private:
	int					iID;

	std::vector<User*>	vUsers;

	int					iKills;

	int					iTotalScore;

	int					iaItemsCount[8];
};


class BellatraServer
{
private:
	IMPFNC										pfnRoomScoreData			= 0x07AC9BB8;
	IMPFNC										pfnRoomScoreKill			= 0x08B810B0;
	IMPFNC										pfnUpdateBellatraUserScore	= 0x0054DBF0;
	IMPFNC										pfnHandleBellatraRoomItem	= 0x0054FFC0;
	IMPFNC										pfnHandleConfigNextRound	= 0x0054D750;
	IMPFNC										pfnCreateBellatraItem		= 0x005525E0;
	IMPFNC										pfnSendScore				= 0x0055B220;

public:
	BellatraServer();
	virtual ~BellatraServer();

	void										UpdateClanCrown();

	static BellatraData::BellatraUnitData		* GetBellatraMonster( int iIndex ) { return ((BellatraData::BellatraUnitData*)0x07AABE80) + iIndex; }
	static BellatraMonsterSpawnHandle			* GetBossSpawnHandle() { return (BellatraMonsterSpawnHandle*)0x07AB3054; }
	static BellatraMonsterSpawnHandle			* GetMonsterSpawnHandle() { return (BellatraMonsterSpawnHandle*)0x07AC9B98; }

	static PacketBellatraResult					* GetBellatraResultPacket() { return (PacketBellatraResult*)0x07AABFA8; }

	void										SetMaxMonsterInRooms( int i ) { (*(int*)0x07AABF48) = i; }
	int											GetMaxMonsterInRooms() { return (*(int*)0x07AABF48); }
	void										SetTimeout( DWORD dw ) { (*(DWORD*)0x07AB14F8) = dw; }
	DWORD										GetTimeout() { return (*(DWORD*)0x07AB14F8); }

	void										SetTimeUpdate( DWORD dw ) { WRITEDWORD( 0x07AC9E34, dw ); }
	DWORD										GetTimeUpdate() { return READDWORD( 0x07AC9E34 ); }

	void										SetDelayTime( DWORD dw ) { WRITEDWORD( 0x07AC9E38, dw ); }
	DWORD										GetDelayTime() { return READDWORD( 0x07AC9E38 ); }

	int											GetRankRoomID( int iRank ) { return (*(int*)(0x07AABF94 + (iRank * 4))); }

	int											GetUserRoomCount( int i );
	void										SetUserRoomCount( int iRoomID, int iCount );
	int											GetRoomScore( int i );

	int											GetTotalUsers() { return (*(int*)0x07AABD38); }
	void										SetTotalUsers(int i) { (*(int*)0x07AABD38) = i; }

	BellatraData								* GetBellatraDataNew( int iRound ) { return &saBellatraData[iRound - 1]; }
	void										SetBellatraDataNew( BellatraData * ps );

	static BellatraData							* GetBellatraData() { return *(BellatraData**)(0x006E469C); }
	static void									SetBellatraData( BellatraData * ps ) { *(BellatraData**)(0x006E469C) = ps; }

	int											GetRoundNew() { return iRound; }
	void										SetRoundNew(int i);

	int											GetRound() { return (*(int*)0x07AAC884) + 1; }
	void										SetRound( int i ) 
	{
		int * piaRoomKills = ((int*)pfnRoomScoreKill);
		ZeroMemory( piaRoomKills, sizeof( int ) * 8 );
//		ZeroMemory( baBossSpawnSOD, sizeof( BOOL ) * 8 ); 
		(*(int*)0x07AAC884) = i;
	}


	void										ResetBellatra();

	BOOL										PHGoldClanRecv( User * pcUser, struct PacketClanGoldReceive * psPacket );

	void										DeletePoint( char * pszUserID, char * pszCharName, char * pszIP, int iUnk, int iUnk2 );

	BOOL										AddGoldClan( int SodScore );

	int											GetClanPoint( UserData * pcUserData );

	BOOL										RecordPoint( UserData * pcUserData, int iLevel, int SodScore, int SodKillCount, int SodTotalScore, int SodTotalSucessUser, int SodTotalUser, int SodCode );

	BOOL										RecordClanPoint( const char * pszClanName, int iPoint, int iOldPoint );
	int											GetClanPoint( const char * pszClanName );
	BOOL										UpdateClanBellatra();

	BOOL										AddRoomScore( int iRoomID, int iScore );
	BOOL										UnitKilled( UnitData * pcUnitData );

	static BOOL									UpdateBellatraUserScore( UserData * pcUserData );

	static BOOL									HandleItemRoom( UnitData * pcUnitData, UserData * pcUserData );

	static void									HandleConfigurationNextRound();

	static void									SendScoreUser( UserData * pcUserData, int iScore, int iKills, int iTotalScore, int iUserCount, int iTotalUsers, int iCode );

	static BOOL									CreateBellatraItem( int iItemID, Map * pcStage, int iX, int iY, int iZ, int iQuantity );

	void										UpdateRound( int iNewRound );

	void										SpawnMonster( int iCount );

	void										UpdateIhin( bool bShow );

	void										UpdateUsersRoom();

	void										UpdateUsersScore();

	BOOL										UpdateServer( Map * pcStage, MonsterMapSpawnHandle * psMonsterData );

	void										SQLBellatraRewardLog( char * pszAccountName, char * pszName, char * pszItemName, int iQuantity );

	void										HandlePacket( User * pcUser, PacketReceiveItemIhin * psPacket );

	SPrizeData									* GetPrizeBellatraRewardItem();

	void										Init();

	BOOL										IsKilledUnitSameRoom( UserData * pcUserData, UnitData * pcUnitData );

	BOOL										SpawnRabieBellatra( UserData * pcUserData, int iSpawnID, Map * pcMap );
	
	void										HandlePacket( User * pcUser, PacketBellatraRequest * psPacket );
	void										HandleNetPacket( PacketNetBellatraRequest * psPacket );
	void										HandleNetPacket( PacketNetBellatraGiveGold * psPacket );


private:

	BellatraData								saBellatraData[BELLATRASERVER_MAX_ROUNDS];

	bool										bEventStart = false;
	int											iRound = 1;

	DWORD										dwFinishTime = 0;

	int											iIhinPosition = -1;
	int											iTotalPercent = 0;

	CBellatraRoom								caBellatraRoom[4];

	int											iaUserRoomCount[4];


	SPrizeData									saPrizes[50];

	int											iRoomTopScore = 0;

	BOOL										IsRoomInParty( std::vector<UserData *> & vUsersRoom );

	int											GetRoomTopScore();

	bool										IsEventTime();

	static BOOL									RequestSod( UserData * pcUserData, int iGold );
};

