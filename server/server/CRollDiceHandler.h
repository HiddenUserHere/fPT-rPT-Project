#pragma once

#include "roll.h"

class RollDiceUser
{
public:
	int							iID;
	User					  * pcUser;
	int							iRandomNumber;

								RollDiceUser( int _iID, User * p ){ iID = _iID; pcUser = p; iRandomNumber = -1; }
};

class RollDiceDrop
{
public:
	std::vector<RollDiceUser*>	vListUsers;
	Map::Loot					* psLoot;
	DWORD						dwEndTime;
	int							iMapID;
	int							iMonsterID;

	RollDiceDrop( std::vector<RollDiceUser*> v, Map::Loot * _psLoot, int _iMapID, int _iMonsterID ) { vListUsers = v; psLoot = _psLoot; dwEndTime = -1; iMapID = _iMapID; iMonsterID = _iMonsterID; }
};

class RollDiceMap
{
public:
	std::vector<RollDiceDrop*>	vListDrop;
	int							iMapID;

	RollDiceMap( int _iMapID ) { iMapID = _iMapID; }
};

class CRollDiceHandler
{
public:
	//Constructor
								CRollDiceHandler();
							   ~CRollDiceHandler();

	void						ProcessPacket( User * pcUser, PacketRollDiceDrop * psPacket );
	void						ProcessPacket( User * pcUser, PacketRollDiceRanking * psPacket );

	void						HandlePacket( User * pcUser, PacketRollDiceDrop * psPacket );

	void						Tick();

	BOOL						OnExchangeItemDrop( UnitData * pcUnitData, Map * pcMap, Map::Loot * psLoot, int iDropIndex );

	void						AddItemDrop( RollDiceDrop  * p, int iMapID ){ if( p )vListMap[iMapID]->vListDrop.push_back(p); }
	void						DeleteItemDrop( RollDiceDrop * p, int iMapID );
private:
	std::vector<RollDiceMap*>	vListMap;

	void						HandleQuest( User * pcUser, UnitData * pcUnitData );
};