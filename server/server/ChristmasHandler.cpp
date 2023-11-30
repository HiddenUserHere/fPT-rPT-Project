#include "stdafx.h"
#include "ChristmasHandler.h"


CChristmasHandler::CChristmasHandler()
{
}


CChristmasHandler::~CChristmasHandler()
{
}

BOOL CChristmasHandler::IsChristmasRune( DWORD dwCode )
{
	BOOL bResult = FALSE;
	switch ( dwCode )
	{
		case ITEMID_RedButton:
		case ITEMID_GreenCloth:
		case ITEMID_Cotton:
			bResult = TRUE;
			break;

		default:
			break;
	}
	return bResult;
}

void CChristmasHandler::OnSpawnMonsterEvent( struct CharacterData * psCharacterData, Map * pcMap )
{
	if ( GAME_SERVER && EVENT_XMAS )
	{
		int iRate = RATE_XMAS;
		if ( MAPSERVER->IsEXPMap( pcMap->pcBaseMap->GetID() ) )
			iRate = RATE_XMAS_EXPMAP;


		int iRand = (rand() | TICKCOUNT) % 1000;
		if ( iRand < iRate )
		{
			if ( CharacterData * psEventCharacterData = UNITSERVER->GetCharacterDataByName( "Santa Skeleton" ) )
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
		iRand = ((rand() + iRand) | TICKCOUNT) % 1000;
		if ( iRand < iRate )
		{
			if ( CharacterData * psEventCharacterData = UNITSERVER->GetCharacterDataByName( "Santa Goblin" ) )
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

BOOL CChristmasHandler::OnManufactureItem( User * pcUser, struct PacketManufactureItem * psPacket )
{
	/*
	if ( GAME_SERVER || !EVENT_XMAS )
		return FALSE;


	DWORD dwRune[3];
	dwRune[0] = psPacket->dwRuneCode[0];
	dwRune[1] = psPacket->dwRuneCode[1];
	dwRune[2] = psPacket->dwRuneCode[2];

	if ( IsChristmasRune( dwRune[0] ) && IsChristmasRune( dwRune[1] ) && IsChristmasRune( dwRune[2] ) )
	{
		if ( (dwRune[0] == dwRune[1]) || (dwRune[0] == dwRune[2]) ||
			 (dwRune[1] == dwRune[0]) || (dwRune[1] == dwRune[2]) ||
			 (dwRune[2] == dwRune[0]) || (dwRune[2] == dwRune[1]) )
		{
			psPacket->iResult = FALSE;
			SENDPACKET( pcUser, psPacket );
			return FALSE;
		}

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

		DWORD dwCreateItem = 0;

		// Cloat
		if ( psPacket->dwRecipeCode == ITEMID_RecipeXMas01 )
		{
			int iRand = rand() % 100;
			if ( iRand < 50 )
			{
				if ( iRand < 25 )
					dwCreateItem = ITEMID_XMasCloatCostumeTM;
				else
					dwCreateItem = ITEMID_XMasCloatCostumeTF;
			}
			else
			{
				dwCreateItem = ITEMID_XMasRedHat;
			}
		}
		// Elegant
		else if ( psPacket->dwRecipeCode == ITEMID_RecipeXMas02 )
		{
			int iRand = rand() % 100;
			if ( iRand < 75 )
			{
				if ( iRand < 25 )
					dwCreateItem = ITEMID_XMasElegantCostumeM;
				else
					dwCreateItem = ITEMID_XMasElegantCostumeF;
			}
			else
			{
				dwCreateItem = ITEMID_XMasRedHat;
			}
		}

		if ( dwCreateItem )
		{

			DefinitionItem * psItemDef = ITEMSERVER->FindItemDefByCode( dwCreateItem );

			if ( !psItemDef )
				return FALSE;

			CHARACTERSERVER->GiveGOLD( pcUser, -psPacket->sItemData.sItem.iSalePrice );

			ITEMSERVER->CreateItem( &psPacket->sItemData.sItem, psItemDef );
			psPacket->iResult = TRUE;
			ITEMSERVER->AddItemInventory( pcUser->pcUserData, psPacket->sItemData.sItem.sItemID.ToItemID(), psPacket->sItemData.sItem.iChk1, psPacket->sItemData.sItem.iChk2 );
			psPacket->iLength = sizeof( PacketManufactureItem );
			psPacket->iHeader = PKTHDR_ManufactureItem;
			SENDPACKET( pcUser, psPacket );

			return TRUE;
		}
	}
	return FALSE;

	*/

	return FALSE;
}

void CChristmasHandler::OnSayTime( UnitData * pcUnitData )
{
	if ( pcUnitData->dwMessageEndTime < TICKCOUNT )
	{
		if ( STRINGCOMPAREI( pcUnitData->sCharacterData.szName, "Santa Skeleton" ) )
		{
			if ( (*(UINT*)pcUnitData->psModelAnimation) == 0x120 )
			{
				char * pszaSkeletonXMASPhrase[6] =
				{
					"I Kidnaped Santa Claus and smashed him with a stick, hahaha",
					"You will be killed this XMas! muahahahaha",
						"See you in the next life (:",
						"I know that I'm a computed data turned into a monster!",
						"GM-Prog's PC can't handle me! buggy pc",
						"Merry XMas in hell ((:",
				};
				int iRand = rand() % 6;
				pcUnitData->dwMessageEndTime = TICKCOUNT + 60000;
				CHATSERVER->SendUserBoxChatRange( pcUnitData->sPosition.iX, pcUnitData->sPosition.iZ, 0x4B000, pcUnitData->iID, "%s: %s", pcUnitData->sCharacterData.szName, pszaSkeletonXMASPhrase[iRand] );
			}
		}
		else if ( pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_HOBGOBLIN && pcUnitData->sCharacterData.iLevel == 60 )
		{
			if ( (*(UINT*)pcUnitData->psModelAnimation) == 0x120 )
			{
				char * pszaSantaGoblinPhrase[6] =
				{
						"",
						"I'm your father! Merry XMas (;",
						"Turn back on musics of XMas, have something for you! HEHEHE",
						"Do you like Christmas? I like to kill you!",
						"Are you a good boy? hehehehe 666",
						"Your parents are liers, Santa Claus doesn't exist!",
				};
				int iRand = rand() % 6;
				pcUnitData->dwMessageEndTime = TICKCOUNT + 60000;

				if ( iRand == 0 )
				{
					if ( pcUnitData->pcKillerData )
						CHATSERVER->SendUserBoxChatRange( pcUnitData->sPosition.iX, pcUnitData->sPosition.iZ, 0x4B000, pcUnitData->iID, "%s: %s, I will catch you this night! MUAHAHA", pcUnitData->sCharacterData.szName, CHARACTERSERVER->GetCharacterName( pcUnitData->pcKillerData ) );
				}
				else
					CHATSERVER->SendUserBoxChatRange( pcUnitData->sPosition.iX, pcUnitData->sPosition.iZ, 0x4B000, pcUnitData->iID, "%s: %s", pcUnitData->sCharacterData.szName, pszaSantaGoblinPhrase[iRand] );
			}
		}
	}
}
