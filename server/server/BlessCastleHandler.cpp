#include "stdafx.h"
#include "BlessCastleHandler.h"


CBlessCastleHandler::CBlessCastleHandler()
{
	SYSTEMTIME * ps = GetServerTime();
	sLastDay = ps->wDay;
}


CBlessCastleHandler::~CBlessCastleHandler()
{
}

void CBlessCastleHandler::Init()
{
	if ( !bInit && *(UnitData**)0x08B64054 != 0 )
	{
		pcUnitDataDoor	= NULL;
		bTowers			= TRUE;
		bCastleDoor		= FALSE;
		bPhase3			= FALSE;
		bPhase4			= FALSE;
		pcUnitDataDoor	= *(UnitData**)0x08B64054;
		bCastleDoor		= TRUE;
		iPhaseID		= 1;
		iOldPhaseID		= 0;
		bInit = TRUE;
	}
}


int CBlessCastleHandler::UpdateTower()
{
	iCountTower = 0;
	BYTE * pbTowerCount = (BYTE*)0x07AB2058;
	if ( pbTowerCount[0] )
		iCountTower += (int)pbTowerCount[0];
	if ( pbTowerCount[1] )
		iCountTower += (int)pbTowerCount[1];
	if ( pbTowerCount[2] )
		iCountTower += (int)pbTowerCount[2];
	if ( pbTowerCount[3] )
		iCountTower += (int)pbTowerCount[3];

	return iCountTower;
}

void CBlessCastleHandler::UpdatePhase( int iPhase )
{
	PacketSiegeWarPhase sPacket;
	sPacket.iLength		= sizeof( PacketSiegeWarPhase );
	sPacket.iHeader		= PKTHDR_UpdateSWPhase;
	sPacket.iPhaseID	= iPhase;
	SENDPACKETSTAGE( &sPacket, MAPID_BlessCastle, TRUE );
}

void CBlessCastleHandler::UpdateSiegeWar()
{
	BLESSCASTLESERVER->Main();

	if ( GAME_SERVER && SERVER_CODE == 1 )
	{

		//Bless Castle Type 1.
		if ( BLESSCASTLESERVER->GetSiegeWarMode() == SIEGEWARMODE_TheConquest || BLESSCASTLESERVER->GetSiegeWarMode() == SIEGEWARMODE_TheMassacre )
		{
			//Reset
			SYSTEMTIME * ps = GetServerTime();

			if ( (ps->wDayOfWeek == 0) && (sLastDay != ps->wDay) )
			{
				CHATSERVER->SendChatAll( CHATCOLOR_Global, "BlessCastle> Siege War Crown resetted!" );
				if ( BLESSCASTLESERVER->GetSiegeWarMode() == SIEGEWARMODE_TheConquest )
					CHATSERVER->SendChatAll( CHATCOLOR_Global, "BlessCastle> Mode: The Conquest!" );
				else if ( BLESSCASTLESERVER->GetSiegeWarMode() == SIEGEWARMODE_TheMassacre )
					CHATSERVER->SendChatAll( CHATCOLOR_Global, "BlessCastle> Mode: The Massacre!" );

				BlessCastleStatusData * psBC = BLESSCASTLESERVER->psBlessCastleStatusData;

				psBC->iClanID = 0;
				psBC->sSettings.iSkill = RandomI( 1, 3 );
				psBC->sSettings.caGuardAmount[0] = 20;
				psBC->sSettings.caGuardAmount[1] = 20;
				psBC->sSettings.caGuardAmount[2] = 20;
				psBC->sSettings.saTowerType[0] = RandomI( 1, 3 );
				psBC->sSettings.saTowerType[1] = RandomI( 1, 3 );
				psBC->sSettings.saTowerType[2] = RandomI( 1, 3 );
				psBC->sSettings.saTowerType[3] = RandomI( 1, 3 );
				psBC->sSettings.saTowerType[4] = RandomI( 1, 3 );
				psBC->sSettings.saTowerType[5] = RandomI( 1, 3 );

				BLESSCASTLESERVER->NetSendBlessCastleStatus( NETSERVER->GetLoginServerConnection() );

				sLastDay = ps->wDay;
			}
		}

		if ( *(int*)(0x07AB1EE8) != 0 )
		{
			Init();
			if ( bInit )
			{
				int iTowers = UpdateTower();
				if ( pcUnitDataDoor && pcUnitDataDoor->IsDead() && bCastleDoor )
				{
					if ( BLESSCASTLESERVER->GetSiegeWarMode() != SIEGEWARMODE_TheMassacre )
					{
						if ( iTowers == 6 )
							CHATSERVER->SendChatAll( CHATCOLOR_Global, "BlessCastle> Siege War has entered Phase 2!" );
						pcUnitDataDoor	= NULL;
						bCastleDoor		= FALSE;
						iPhaseID		= 2;
					}
				}

				if ( bTowers && !bCastleDoor )
				{
					if ( iTowers == 4 && !bPhase3 )
					{
						CHATSERVER->SendChatAll( CHATCOLOR_Global, "BlessCastle> Siege War has entered Phase 3!" );
						bPhase3		= TRUE;
						iPhaseID	= 3;
					}
					else if ( iTowers == 2 && !bPhase4 )
					{
						CHATSERVER->SendChatAll( CHATCOLOR_Global, "BlessCastle> Siege War has entered Phase 4!" );
						bPhase4		= TRUE;
						iPhaseID	= 4;

					}
					else if ( iTowers == 0 )
					{
						CHATSERVER->SendChatAll( CHATCOLOR_Global, "BlessCastle> Siege War has entered the Final Phase!" );
						bTowers		= FALSE;
						iPhaseID	= 5;
					}
				}

				// Update Users
				if ( iPhaseID != iOldPhaseID )
				{
					UpdatePhase( iPhaseID );
					iOldPhaseID = iPhaseID;
				}
				else if ( TICKCOUNT > dwTickUpdate )
				{
					UpdatePhase( iPhaseID );
					dwTickUpdate = TICKCOUNT + 5000;
				}

			}
		}
		else
		{
			*(UINT*)0x08B64054 = 0;
			bInit = FALSE;
		}
	}

}

