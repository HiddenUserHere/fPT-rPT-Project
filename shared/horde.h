#pragma once

#include "user.h"
#include "character.h"
#include "item.h"

#define NAME_MONSTERHORDE_PREFIX		"_Horde"

class CMonsterHorde
{
private:
	CharacterData			sCharacterData;
	UnitInfo				sUnitInfo;

	BOOL					bIsPercentageStats;

	int						iChance;
public:
	CMonsterHorde() {};

	~CMonsterHorde() {};

	CharacterData				& GetCharacterData() { return sCharacterData; };
	UnitInfo					& GetUnitInfo() { return sUnitInfo; };

	BOOL						IsPercentageStats() { return bIsPercentageStats; }
	void						SetPercentageStats( BOOL _bIsPercentageStats ) { bIsPercentageStats = _bIsPercentageStats; }

	int							GetChance() { return iChance; }
	void						SetChance( int _iChance ) { iChance = _iChance; }
};

class CHordeInstance
{
public:
	CHordeInstance();

	virtual ~CHordeInstance();

	void						Init( int iID, std::string strName );

	int							GetID() { return iID; }

	BOOL						IsActive() { return bActive; }
	void						SetActive( BOOL b ) { bActive = b; }

	std::string					GetName() { return strName; }
	void						SetName( std::string _strName ) { strName = _strName; }

	std::vector<EMapID>			& GetMapIDs() { return vMapIDs; }
	void						AddMapID( EMapID eMapID ) { vMapIDs.push_back( eMapID ); }

	std::vector<std::pair<EMapID, int>> & GetExtraDrops() { return vExtraDrops; }
	void						AddExtraDrop( EMapID eMapID, int iExtraDrop ) { vExtraDrops.push_back( std::make_pair( eMapID, iExtraDrop ) ); }

	void						AddHour( int iHour );

	bool						IsHour( int iHour );

	std::vector<CMonsterHorde*>	& GetMonsters() { return vMonsters; }

	void						AddMonster( CMonsterHorde * pcMonster ) { vMonsters.push_back( pcMonster ); }

	float						GetEXPMultiplier() { return fEXPMultiplier; }
	void						SetEXPMultiplier( float _fEXPMultiplier ) { fEXPMultiplier = _fEXPMultiplier; }

	BOOL						GetUseEXPMap() { return bUseEXPMap; }
	void						SetUseEXPMap( BOOL _bUseEXPMap ) { bUseEXPMap = _bUseEXPMap; }

	BOOL						GetUseDropMap() { return bUseDropMap; }
	void						SetUseDropMap( BOOL _bUseDropMap ) { bUseDropMap = _bUseDropMap; }

private:
	int							iID = -1;

	std::string					strName;

	std::vector<CMonsterHorde*>	vMonsters;

	std::vector<int>			vHours;

	std::vector<EMapID>			vMapIDs;

	std::vector<std::pair<EMapID, int>>	vExtraDrops;

	BOOL						bUseEXPMap = FALSE;
	float						fEXPMultiplier = 0.0f;

	BOOL						bUseDropMap = FALSE;

	BOOL						bActive = FALSE;
};