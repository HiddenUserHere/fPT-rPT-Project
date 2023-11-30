#include "stdafx.h"
#include "horde.h"

CHordeInstance::CHordeInstance()
{
}

CHordeInstance::~CHordeInstance()
{
	for ( auto & p : vMonsters )
		DELET( p );

	vMonsters.clear();
	vHours.clear();
	vMapIDs.clear();
	vExtraDrops.clear();
}

void CHordeInstance::Init( int iID, std::string strName )
{
	this->iID		= iID;
	this->strName	= strName;
}

void CHordeInstance::AddHour( int iHour )
{
	vHours.push_back( iHour );
}

bool CHordeInstance::IsHour( int iHour )
{
	for ( auto & i : vHours )
		if ( i == iHour )
			return true;

	return false;
}
