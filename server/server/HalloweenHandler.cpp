#include "stdafx.h"
#include "HalloweenHandler.h"


CHalloweenHandler::CHalloweenHandler()
{
	dwRuneEvil[0]		= ITEMID_SpiderPlastic;
	dwRuneEvil[1]		= ITEMID_GriffenEgg;
	dwRuneEvil[2]		= ITEMID_ToiletPaper;

	dwRuneInfernal[0]	= ITEMID_GriffenEgg;
	dwRuneInfernal[1]	= ITEMID_HopyToy;
	dwRuneInfernal[2]	= ITEMID_ToiletPaper;
	iCombinationID		= 0;
}


CHalloweenHandler::~CHalloweenHandler()
{
}

void CHalloweenHandler::Init()
{
}

BOOL CHalloweenHandler::OnManufactureItem( User * pcUser, struct PacketManufactureItem * psPacket )
{
	/*
	if ( !EVENT_HALLOWEEN && pcUser->pcUserData->iGameLevel == GAMELEVEL_None )
		return FALSE;

	// Checksum
	for ( int i = 0; i < 3; i++ )
	{
		if ( ITEMSERVER->DeleteItemInventory( pcUser->pcUserData, psPacket->dwRuneCode[i], psPacket->dwRuneHead[i], psPacket->dwRuneCheckSum[i] ) < 0 )
		{
			// TODO
		}
	}

		if ( ITEMSERVER->DeleteItemInventory( pcUser->pcUserData, psPacket->dwRecipeCode, psPacket->dwRecipeHead, psPacket->dwRecipeChecksum ) < 0 )
		{
			// TODO
		}


	if ( psPacket->dwRecipeCode != ITEMID_CoreHW )
		return FALSE;

	DWORD dwRune[3];
	dwRune[0] = psPacket->dwRuneCode[0];
	dwRune[1] = psPacket->dwRuneCode[1];
	dwRune[2] = psPacket->dwRuneCode[2];

	DWORD dwCreateItem = ITEMID_RottenLantern;

	if ( dwRune[0] == dwRuneInfernal[0] && dwRune[1] == dwRuneInfernal[1] && dwRune[2] == dwRuneInfernal[2] )
	{
		dwCreateItem = ITEMID_InfernalLantern;
	}
	else if ( dwRune[0] == dwRuneEvil[0] && dwRune[1] == dwRuneEvil[1] && dwRune[2] == dwRuneEvil[2] )
	{
		dwCreateItem = ITEMID_EvilLantern;
	}

	DefinitionItem * psItemDef = ITEMSERVER->FindItemDefByCode( dwCreateItem );

	if ( !psItemDef )
		return FALSE;

	ITEMSERVER->CreateItem( &psPacket->sItemData.sItem, psItemDef );
	psPacket->iResult = TRUE;
	CHARACTERSERVER->GiveGOLD( pcUser, -psPacket->sItemData.sItem.iSalePrice );
	ITEMSERVER->AddItemInventory( pcUser->pcUserData, psPacket->sItemData.sItem.sItemID.ToItemID(), psPacket->sItemData.sItem.iChk1, psPacket->sItemData.sItem.iChk2 );
	psPacket->iLength = sizeof( PacketManufactureItem );
	psPacket->iHeader = PKTHDR_ManufactureItem;
	SENDPACKET( pcUser, psPacket );
	*/
	return TRUE;

}

void CHalloweenHandler::OnSpawnEventMonster( CharacterData * psCharacterData )
{
	if ( GAME_SERVER && EVENT_HALLOWEEN )
	{
		int iRand = rand() % 1000;
		if ( iRand < RATE_HALLOWEEN )
		{
			iRand = rand() % 100;
			if ( iRand < 65 )
			{
				if ( CharacterData * psEventCharacterData = UNITSERVER->GetCharacterDataByEffect( MONSTEREFFECTID_BEAR ) )
				{
					LARGE_INTEGER liEXP, liEXPEvent;
					liEXP.QuadPart = UNITSERVER->GetExp( psCharacterData );
					liEXPEvent.QuadPart = UNITSERVER->GetExp( psEventCharacterData );

					CopyMemory( psCharacterData, psEventCharacterData, sizeof( CharacterData ) );

					if ( liEXP.QuadPart > liEXPEvent.QuadPart )
					{
						psCharacterData->iCurrentExpLow = liEXP.LowPart;
						psCharacterData->iCurrentExpHigh = liEXP.HighPart;
					}
				}
			}
			else
			{
				if ( CharacterData * psEventCharacterData = UNITSERVER->GetCharacterDataByEffect( MONSTEREFFECTID_BRAZILBEAR ) )
				{
					LARGE_INTEGER liEXP, liEXPEvent;
					liEXP.QuadPart = UNITSERVER->GetExp( psCharacterData );
					liEXPEvent.QuadPart = UNITSERVER->GetExp( psEventCharacterData );

					CopyMemory( psCharacterData, psEventCharacterData, sizeof( CharacterData ) );

					if ( liEXP.QuadPart > liEXPEvent.QuadPart )
					{
						psCharacterData->iCurrentExpLow = liEXP.LowPart;
						psCharacterData->iCurrentExpHigh = liEXP.HighPart;
					}
				}
			}
		}
	}
}

BOOL CHalloweenHandler::OnUnitKilled( UnitData * pcUnitData )
{
	if ( pcUnitData )
	{
		if ( pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_BRAZILBEAR )
		{
			int iRand = rand() % 100;
			if ( iRand < 15 )
			{
				CharacterData * psChar = UNITSERVER->GetCharacterDataByEffect( MONSTEREFFECTID_MRGHOST, 300 );
				if ( psChar )
				{
					if ( pcUnitData->pcMap && pcUnitData->pcMap->pcBaseMap )
						MAPSERVER->SpawnMonster( psChar->szName, pcUnitData->pcMap->pcBaseMap->GetID(), Point3D( pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ ) );
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

void CHalloweenHandler::SetCombinationID( int iID )
{
	if ( iCombinationID != iID )
	{
		iCombinationID = iID;

		if ( iCombinationID > 3 )
			iCombinationID = 0;
		
		if ( iCombinationID == 1 )
		{
			dwRuneEvil[0] = ITEMID_SpiderPlastic;
			dwRuneEvil[1] = ITEMID_GriffenEgg;
			dwRuneEvil[2] = ITEMID_SpiderPlastic;

			dwRuneInfernal[0] = ITEMID_GriffenEgg;
			dwRuneInfernal[1] = ITEMID_SpiderPlastic;
			dwRuneInfernal[2] = ITEMID_ToiletPaper;
		}
		else if ( iCombinationID == 2 )
		{
			dwRuneEvil[0] = ITEMID_SpiderPlastic;
			dwRuneEvil[1] = ITEMID_GriffenEgg;
			dwRuneEvil[2] = ITEMID_HopyToy;

			dwRuneInfernal[0] = ITEMID_HopyToy;
			dwRuneInfernal[1] = ITEMID_SpiderPlastic;
			dwRuneInfernal[2] = ITEMID_ToiletPaper;
		}
		else if ( iCombinationID == 3 )
		{
			dwRuneEvil[0] = ITEMID_SpiderPlastic;
			dwRuneEvil[1] = ITEMID_GriffenEgg;
			dwRuneEvil[2] = ITEMID_HopyToy;

			dwRuneInfernal[0] = ITEMID_GriffenEgg;
			dwRuneInfernal[1] = ITEMID_ToiletPaper;
			dwRuneInfernal[2] = ITEMID_HopyToy;
		}
		else
		{
			dwRuneEvil[0] = ITEMID_HopyToy;
			dwRuneEvil[1] = ITEMID_GriffenEgg;
			dwRuneEvil[2] = ITEMID_ToiletPaper;

			dwRuneInfernal[0] = ITEMID_GriffenEgg;
			dwRuneInfernal[1] = ITEMID_SpiderPlastic;
			dwRuneInfernal[2] = ITEMID_ToiletPaper;
		}
		if ( EVENT_HALLOWEEN )
			CHATSERVER->SendChatAll( CHATCOLOR_Global, "Halloween> Rune Combination Changed!" );
	}
}

void CHalloweenHandler::Tick()
{
	if ( GAME_SERVER )
	{
		return;

		if ( dwDropTime == 0 )
			dwDropTime = TICKCOUNT + (3 * 60 * 60 * 1000);

		if ( dwDropTime < TICKCOUNT )
		{
			Map::Loot::LootItem sLootItem;

			Map * pcMap = MAPSDATA + MAPID_RicartenTown;

			const EItemID iaItems[] = { ITEMID_HopyToy, ITEMID_GriffenEgg, ITEMID_SpiderPlastic, ITEMID_ToiletPaper };

			//Get Item Data
			DefinitionItem * ps = ITEMSERVER->FindItemDefByCode( iaItems[rand() % _countof( iaItems )] );
			if ( ps )
			{
				//Create Item with non-spec
				ITEMSERVER->CreateItem( &sLootItem.sItem, ps, 0 );

				if ( pcMap )
				{
					static const Point3D sTombPosition( 263, 211, -17222 );

					//Add Item to Map
					Map::Loot * psItemD = pcMap->AddItem( &sLootItem, sTombPosition.iX, sTombPosition.iY, sTombPosition.iZ, TRUE );

					//Added? Send It to near users...
					if ( psItemD )
					{
						MAPSERVER->SendItemStageNearUsers( pcMap, psItemD );
						CHATSERVER->SendChatAllUsersInStage( MAPID_RicartenTown, CHATCOLOR_Global, "Halloween Soul> The item from darkness!" );
					}
				}
			}

			dwDropTime = TICKCOUNT + (3 * 60 * 60 * 1000);
		}
	}
	else
	{
		if ( !dwUpdateCombinationTime )
			dwUpdateCombinationTime = TICKCOUNT + (60 * 60 * 8 * 1000);

		if ( TICKCOUNT > dwUpdateCombinationTime )
		{
			MANUFACTUREHANDLER->SetHalloweenCombination( MANUFACTUREHANDLER->GetHalloweenCombination() + 1 );
			dwUpdateCombinationTime = TICKCOUNT + (60 * 60 * 8 * 1000);
		}
	}
}
