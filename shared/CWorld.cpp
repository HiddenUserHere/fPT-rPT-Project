#include "stdafx.h"
#include "CWorld.h"

CWorld::CWorld( EWorldID _iWorldID, string _strName, unsigned long _lIP, USHORT _uPort, string _strGameName, string _strShortGameName, string _strWebsite, unsigned long _lIP2, unsigned long _lIP3 )
{
	iWorldID			= _iWorldID;
	strName				= _strName;
	lIP					= _lIP;
	uPort				= _uPort;

	strGameName			= _strGameName;
	strShortGameName	= _strShortGameName;
	strWebsite			= _strWebsite;

	lIP2				= _lIP2;
	lIP3				= _lIP3;

	char szIP[32];
	//IP longs to Strings
	STRINGCOPY( szIP, NumberIPToStringIPV4( lIP ).c_str() );
	strIP				= szIP;

	STRINGCOPY( szIP, NumberIPToStringIPV4( lIP2 ).c_str() );
	strIP2				= szIP;

	STRINGCOPY( szIP, NumberIPToStringIPV4( lIP3 ).c_str() );
	strIP3				= szIP;
}

CWorld::~CWorld()
{
}

void CWorld::SetIP( unsigned long _lIP )
{
	lIP			= _lIP;
	
	char szIP[32];
	//IP long to Strings
	STRINGCOPY( szIP, NumberIPToStringIPV4( lIP ).c_str() );
	strIP		= szIP;
}