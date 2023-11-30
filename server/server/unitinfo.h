#pragma once

class UnitInfoServer
{
public:
	UnitInfoServer();
	virtual ~UnitInfoServer();

	void												Init();

	UINT												GetTotalUnitEnemy();
	CharacterData *										GetEnemyCharInfoPtr();
	UnitInfo *										GetEnemyDataInfoPtr();

	DefinitionItemEx									* GetItemDefinitionExByItemID( EItemID eItemID );

	ItemData *												FindItemPointerTable( const char * szCode );
	ItemData *												FindItemPointerTable( UINT uCode );

	CharacterData *										GetCharacterDataByMonsterName( const char * pszName );
	CharacterData *										GetCharacterDataByMonsterID( int iID );

	int													GetModelID( const char * pszModelFile );
	int													GetModelDeathID( const char * pszModelFile );
	UINT												ReadModelSize( const char * pszModelFile );
	BOOL												ReadUnitAnimation( char * pszModelFile, ModelDataOld * pModelData );
	int													ReadInx( char * pszName );
	int													ReadInxOld( char * pszName );

	void												CreateUnitEnemyData();
	void												ReadUnitSpawnData( Map * pStageData );
	void												ReadUnitEnemyStage( Map::SpawnSetting * pMonsterStageList );

	void												CreateItemMemoryTable();

	void												ReadUnitNpcStand( Map * pStageData );
	void												ReadUnitNpcData( CharacterData * pCharInfo, UnitInfo *pMonInfo, char *pDialogMessage );

	void												OnDescriptionSpawn( int iMapID, int iX, int iZ, char * pszDescription );

	void												SetupMonsterBellatra( UINT BellatraMemory, UINT ConfigMemory );

private:
	int													GetEnemyEffect( char * pszEffect );
	
	void												CompressItemDataInTable( ItemData * pItem );

	BOOL												EnemyDataItem( UnitInfo * pMonsterInfo, const char * pszItems, int iChance, int iGoldMin, int iGoldMax );

	void												ReadDropItem();

protected:
	CompressedItem								* pTableNewItem = NULL;

	DefinitionItemEx							* pDefinitionItemExTable = nullptr;
};
