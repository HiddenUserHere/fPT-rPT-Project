#include "stdafx.h"
#include "battleroyale.h"

using namespace BattleRoyale;

CBattleRoyale::CBattleRoyale() : iID( -1 ), eMapID( MAPID_Invalid )
{
	strName = "";
}

CBattleRoyale::CBattleRoyale( int _iID, const std::string _strName, EMapID _eMapID ) : iID( _iID ), strName( _strName ), eMapID( _eMapID )
{
}

CBattleRoyale::~CBattleRoyale()
{
	for ( auto ps : vTimes )
		DELET( ps );

	vTimes.clear();

	for ( auto & vDropData : vDrops )
	{
		for ( auto ps : vDropData )
			DELET( ps );

		vDropData.clear();
	}
	vDrops.clear();

	for ( auto psCoordinate : vSpawnCoordinates )
		DELET( psCoordinate );

	vSpawnCoordinates.clear();

	for ( auto psHandicap : vBattleHandicaps )
		DELET( psHandicap );

	vBattleHandicaps.clear();
}

void CBattleRoyale::AddTime( BattleTimes * psBattleTimes )
{
	vTimes.push_back( psBattleTimes );
}

bool CBattleRoyale::IsTime( int iDayWeek, int iHour, int iMinute, int iLeftMinute )
{
	for ( auto psTimes : vTimes )
	{
		if ( psTimes->iDayWeek == iDayWeek )
		{
			for ( auto ps : psTimes->vTimes )
			{
				int iSeconds = ps.first * 60 * 60;
				iSeconds += ps.second * 60;

				int iSecoundsTime = iHour * 60 * 60;
				iSecoundsTime += iMinute * 60;

				if ( ((iSeconds - iSecoundsTime) - (iLeftMinute * 60)) == 0 )
					return true;
			}
		}
	}
	return false;
}

INT64 CBattleRoyale::GetTimeLeft( SYSTEMTIME sServerTime )
{
	SYSTEMTIME sTimeBroyale;
	CopyMemory( &sTimeBroyale, &sServerTime, sizeof( SYSTEMTIME ) );
	sTimeBroyale.wMilliseconds = 0;
	sTimeBroyale.wSecond = 0;

	INT64 iTimeLeft = INT64_MAX;
	for ( auto psTimes : vTimes )
	{
		if ( (WORD)psTimes->iDayWeek == sServerTime.wDayOfWeek )
		{
			sTimeBroyale.wDayOfWeek = (WORD)psTimes->iDayWeek;
			for ( auto ps : psTimes->vTimes )
			{
				sTimeBroyale.wHour = ps.first;
				sTimeBroyale.wMinute = ps.second;

				INT64 iTimeStart = SystemTimeToQWORD( &sServerTime );
				INT64 iTimeEnd = SystemTimeToQWORD( &sTimeBroyale ) - iTimeStart;

				if ( (iTimeEnd > 0) && (iTimeEnd < iTimeLeft) )
					iTimeLeft = iTimeEnd;
			}
		}
	}

	return iTimeLeft;
}


