#pragma once

class Map;

struct SpawnDataInfo
{
	struct SpawnData
	{
		int					iID;
		char				szName[32];
		int					iX;
		int					iZ;
	};

	std::vector<SpawnData*>	vSpawnData;

	int						iMapID;

	SpawnDataInfo() { iMapID = -1; }
	SpawnDataInfo( int _iMapID ) { iMapID = _iMapID; }
	~SpawnDataInfo() {}
};

struct MapBossPositionData
{
	char				szName[32];

	int					iEffectID;

	EMapID				eMapID;

	Point3D				sPosition;
};

class MapServer
{

private:
	
	typedef int( __thiscall *tfnFindEmptyIndexUnitData )(Map * pcMap);

	IMPFNC									pfnAddItemStage			= 0x0054E370;
	IMPFNC									pfnFindIndexUnitData	= 0x0054EFA0;

	IMPFNC									pfnQuestUnitMapHandler  = 0x0055BA00;

	IMPFNC									pfnGetMinuteOfDay		= 0x0054CBC0;

	IMPFNC									pfnGetAngle				= 0x0046EB00;

	BaseMap									* pcaBaseMap;

	std::vector<SpawnDataInfo*>				vSpawnMapsInfo;

	std::vector<MapBossPositionData *>		vBossMapPosition;

public:
	//Constructor
											MapServer();
	virtual									~MapServer();

	void									AddMapSpawnData( int iMapID, const char * pszName, int iID, int iX, int iZ );
	void									DelMapSpawnData( int iMapID, int iX, int iZ );
	SpawnDataInfo::SpawnData				* GetMapSpawnData( int iMapID, int iX, int iZ );

	Map										* GetMap( int iMapID );

	static UINT								GetMinuteOfDay();

	static BOOL								HandleUnitQuestMap( void * pcMap, UnitData * pcUnitData );

	void									SetFlagPoint( UserData * pcUserData, const char * pszDescription );
	void									DeleteFlagPoint( UserData * pcUserData, PacketDeleteFlagPoint * psPacket );

	void									OnSendDebugDamage( User * pcUser, PacketDebugDamageInfo * psPacket );
	void									OnSendDebugDamageUnit( UnitData * pcUnitData, int iDamage );

	BOOL									AddMonsterSpawn( Map * pcMap, UnitData * pcUnitData );
	BOOL									DelMonsterSpawn( Map * pcMap, UnitData * pcUnitData );
	BOOL									DisableMonsterSpawn( Map * pcMap, UnitData * pcUnitData );
	BOOL									DelItem( Map * pcMap, Map::Loot * psItem );

	void									Load();

	BOOL									IsNight() { return (*(BOOL*)0x07AC5654); }
	void									IsNight(BOOL b) { (*(BOOL*)0x07AC5654) = b; }

	UINT									GetTime() { return (*(UINT*)0x07AAC88C); }
	void									SetTime(UINT i) { (*(UINT*)0x07AAC88C) = i; }

	BOOL									IsPVPMap( int iMapID );
	BOOL									IsPKMap( int iMapID );
	BOOL									IsEXPMap( int iMapID );
	BOOL									IsBellatraMapArea( int iX, int iY, int iZ );
	BOOL									IsSummonMap( int iMapID );
	BOOL									IsValidMap( Map * pcMap );

	int										GetEmptyIndexUnitData( Map * pcMap );


	static void								* FindItemStage( Map * pcMap, int iX, int iY, int iZ );

	BOOL									SendItemStageNearUsers( Map * pcMap, Map::Loot * psItemStage );
	void									SendItemStageUser( Map * pcMap, UserData * pcUserData, Map::Loot * psItemStage );

	void									SendStageItem( Map * pcMap, UserData * pcUserData );
	void									SendStageSpawn( Map * pcMap, UserData * pcUserData );
	BOOL									SetSpawnMonster( Map * pcMap, UnitData * pcUnitData );

	BOOL									OnThrowItem( UserData * pcUserData, struct PacketThrowItem * psPacket );

	BOOL									UseUnionCore( UserData * pcUserData, struct PacketUseUnionCore * psPacket );

	BOOL									SetBossPosition( Map * pcMap, CharacterData * pcUnitData, Point3D * psPosition );
	BOOL									SpawnMonsterBoss( Map * pcMap, Map::SpawnSetting::Boss * psBoss );

	BOOL									UpdateUnitPosition( UnitData * pcUnitData, int iMapID, int iX, int iY, int iZ );
	BOOL									UpdateUnitAngle( UnitData * pcUnitData, int iMapID, int iAngle);

	UnitData							  * CreateUnit( Map * pcMap, MonsterMapSpawnHandle * pcMonster, int iGroup );

	void									UpdateMap( Map * pcMap );

	void									Update();

	void									UpdateTime();

	void									SpawnNPC( Map * pcMap, PacketUnitInfo * psPacket );
	BOOL									SpawnCrystal( Item * psItem, int iX, int iY, int iZ, UserData * pcUserData );
	UnitData								* SpawnMonsterBC( Map * pcMap, CharacterData * psCharacterData, int iX, int iY, int iZ );
	UnitData								* SpawnMonster( const char * pszMonsterName, int iMapID, Point3D sPosition, User * pcUser = nullptr );

	UINT									GetWorldTimeHour();

	int										MakeFaceArea( Stage * pcThis, int iX, int iZ, int iWidth, int iHeight, int iTop, int iBottom );
	BOOL									SetupPolygonArea( Stage * pcThis );
	int										GetHeightArea( Stage * pcThis, int iAX, int iAZ, int iX, int iZ );

	void									UpdateUserBossTime( User * pcUser, int iMapID );

	void									UpdateUsersBossTime();

	void									HandlePacket( User * pcUser, PacketBossTimeUpdate * psPacket );

	static int								GetAngle( int iX, int iZ, int iTargetX, int iTargetZ );

	static EMapID							* piBabelStormMapID;
	static DWORD							* pdwBabelStormEndTime;

	void									SQLUpdateBossTime( int iMinute, int iServerID, int iDay, BOOL bGameMaster = FALSE );

	void									SQLLoadMapBossPosition();

	bool									IsTownMap( int iMapID );
};

