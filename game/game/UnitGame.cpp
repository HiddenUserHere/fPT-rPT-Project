#include "stdafx.h"
#include "UnitGame.h"
#include "LevelTable.h"

UnitGame * UnitGame::pcInstance = NULL;
int UnitGame::iSpeedHack = 0;
DWORD UnitGame::dwUpdatePacket = 0;
PacketServerInfo sServerInfo;

Unit  UnitGame::cViewUnit;

float fSZHPBar = 15.0f;

extern BOOL __cdecl AnimateMeshTreeNew( PTMesh * pcMesh, int iFrame, int iAX, int iAY, int iAZ );

UnitGame::UnitGame()
{
	pcaUnitData = (UnitData*)0x0D16888;

	pcUnitData = new UnitData();
	pcUnit = new Unit();
	pcaUnit = new Unit[MAX_UNITS];

	for ( UINT i = 0; i < MAX_UNITS; i++ )
	{
		pcaUnit[i].uIndex = i;
		pcaUnit[i].pcUnitData = pcaUnitData + i;
	}

	WRITEDWORD( 0x00CF481C, pcUnitData );

	WRITEDWORD( 0x0362A3B8, &pcUnitData->sCharacterData );

	TEST( "UnitData", sizeof( UnitData ), 0x4D98 );

	pcUnit->Load( pcUnitData );
}

UnitGame::~UnitGame()
{
	DELET( pcUnitData );
	DELET( pcUnit );
	DELETA( pcaUnit );
}

UnitData * UnitGame::GetUnitDataByID( ID lID )
{
	if ( lID == UNITDATA->iID )
		return UNITDATA;

	typedef UnitData*( __cdecl *t_GetUnitDataByID )( UINT uID );
	t_GetUnitDataByID GetUnitDataByID = (t_GetUnitDataByID)pfnGetUnitData;

	return GetUnitDataByID( lID );
}

Unit * UnitGame::UnitDataToUnit( UnitData * pcUnitData )
{
	if ( pcUnitData == NULL )
		return NULL;

	//Find matching Unit for this UnitData
	Unit * pcUnit = NULL;

	if ( UNITDATA == pcUnitData )
	{
		//UnitData is self
		pcUnit = this->pcUnit;

		if ( pcUnit->iID != pcUnitData->iID )
			pcUnit->iID = pcUnitData->iID;
	}
	else if ( (UnitData*)(0x0207C888) == pcUnitData )
	{
		//The View Unit
		pcUnit = &cViewUnit;
	}
	else if ( ((pcUnitData - pcaUnitData) >= 0) && ((pcUnitData - pcaUnitData) < MAX_UNITS) )
	{
		//Withing the Unit Array
		pcUnit = pcaUnit + (pcUnitData - pcaUnitData);
	}

	//Nothing? Then use a Static Unit
	if ( pcUnit == NULL )
	{
		static Unit t;
		pcUnit = &t;
	}

	//Load Unit if ID mismatch between Unit and UnitData
	if ( (pcUnit->pcUnitData != pcUnitData) || (pcUnit->iID != pcUnitData->iID) )
		pcUnit->Load( pcUnitData );

	return pcUnit;
}

void UnitGame::DrawLifeBar( int iX, int iY )
{
	// Vars
	int iPercent = 0, iPercent2 = 0, iPercent3 = 0;
	DWORD dwColor = 0;

	UnitData * pc = UNITDATABYID( *(DWORD*)( 0x207C888 + 0x10 ) );

	if( pc->sCharacterData.iType == 1 )
		pc->bShowHP = TRUE;
	else
		pc->bShowHP = FALSE;

	// HP Max is 0? no need show
	if( pc->sCharacterData.sHP.sMax == 0 || ( !DEBUG_GM && !pc->bShowHP ) )
		return;

	// Treepass screen? no show
	if( iX < 0 || ( iX + 100 ) >= ( *(int*)0x03A9A30C ) )
		return;

	// Treepass screen? no show
	if( iY < 0 || ( iY + 100 ) >= ( *(int*)0x03A9A310 ) )
		return;

	// HP Percent
	iPercent = ( pc->sCharacterData.sHP.sCur * 92 ) / pc->sCharacterData.sHP.sMax;
	iPercent2 = ( pc->sCharacterData.sHP.sCur * 511 ) / pc->sCharacterData.sHP.sMax;
	iPercent2 = ( iPercent2 * iPercent2 ) / 512;
	iPercent3 = iPercent2 & 0xFF;

	// Color based to percent HP
	if( iPercent2 < 256 )
		dwColor = D3DCOLOR_RGBA( 255, iPercent3, 0, 255 );
	else
		dwColor = D3DCOLOR_RGBA( 255 - iPercent3, 255, 0, 255 );

	// Calculate Bar Size relative to Resolution
	int iHeight = (RESOLUTION_WIDTH / 4) * 3;
	float fSize = 0, fSizeW = 0;
	fSizeW = (float)RESOLUTION_WIDTH * (*(float*)0x007AFD8C);

	if( iHeight == RESOLUTION_HEIGHT )
		fSize = ((float)RESOLUTION_WIDTH / 800.0f);
	else
		fSize = ((float)RESOLUTION_WIDTH / (float)( ( RESOLUTION_HEIGHT / 3 ) * 4 ));

	// Texture 0 = Bar, 1 = Color
	int * piImage = ( (int*)0x00CDFF54 );

	// Functions
	typedef void( __cdecl *tfnDrawTexImageFloat )( int Mat, float x, float y, float w, float h, float surW, float surH, float tLeft, float tTop, float tRight, float tBottom, int Transp );
	typedef void( __cdecl *tfnDrawTexImageFloatColor )( int Mat, float x, float y, float w, float h, float surW, float surH, float tLeft, float tTop, float tRight, float tBottom, DWORD dwColor );
	tfnDrawTexImageFloat fnDrawTexImageFloat = (tfnDrawTexImageFloat)0x00409F40;
	tfnDrawTexImageFloatColor fnDrawTexImageFloatColor = (tfnDrawTexImageFloatColor)0x0040A240;

	// Draw Bar model
	fnDrawTexImageFloat( piImage[0], (float)iX, (float)iY, 100 * fSizeW, DXGraphicEngine::GetWidthBackBuffer() > 1024 ? 22.5f : 16.0f*fSize, 128, 16, 0, 0, 100, 15, 255 );

	// Have HP? Draw inside Bar model
	if( pc->sCharacterData.sHP.sCur > 0 )
		fnDrawTexImageFloatColor( piImage[1], (float)iX, (float)iY, (float)(iPercent + 3)*fSizeW, DXGraphicEngine::GetWidthBackBuffer() > 1024 ? 22.5f : 16.0f*fSize, 128, DXGraphicEngine::GetWidthBackBuffer() == 800 ? 18.0f : 16.0f, 0, 0, (float)iPercent + 3, 15, dwColor );
}

BOOL UnitGame::SetCharacterHeadModel( const char * pszHeadModel, BOOL bEffect )
{
	// Model not exists?
	char szModelName[64] = { 0 };

	STRINGCOPY( szModelName, pszHeadModel );
	int iLen = STRLEN( szModelName );
	if( iLen > 3 )
	{
		szModelName[iLen - 3] = 'i';
		szModelName[iLen - 2] = 'n';
		szModelName[iLen - 1] = 'x';
	}

	if( !FILEEXIST( szModelName ) && !FILEEXIST( pszHeadModel ) )
		return FALSE;

	if( STRINGCOMPARE( pszHeadModel, UNITDATA->sCharacterData.Player.szHeadModel ) )
		return TRUE;

	STRINGCOPY( UNITDATA->sCharacterData.Player.szHeadModel, pszHeadModel );
	UNITDATA->bNoMove = TRUE;

	SetPlayerPattern( UNITDATA, UNITDATA->sCharacterData.Player.szBodyModel, UNITDATA->sCharacterData.Player.szHeadModel );

	UNITDATA->sCharacterData.cNewLoad++;
	CHECK_CHARACTER_CHECKSUM;
	UpdateObjectID( UNITDATA->iID );

	SAVE;

	if( bEffect )
	{
		SKILLMANAGERHANDLER->StartSkillEffect( UNITDATA->sPosition.iX, UNITDATA->sPosition.iY, UNITDATA->sPosition.iZ, 0, 0, 0, SKILL_UP1 );
		SKILLMANAGERHANDLER->SkillPlaySound( 0x1000, UNITDATA->sPosition.iX, UNITDATA->sPosition.iY, UNITDATA->sPosition.iZ );
	}

	return TRUE;
}

void UnitGame::ClearFarUnits()
{
	// This Function has been rewrite to reduce lag while playing actions
	static int iUnitMonstersCount;
	static BOOL bCountNowUnitMonsters;

	BOOL bCanClose;
	int iX = 0, iZ = 0;


	for( int i = 0; i < MAX_UNITS; i++ )
	{
		UnitData * p = pcaUnitData + i;
		// Unit Active?
		if( p->bActive )
		{
			bCanClose = FALSE;

			if( ( p->dwLastActiveTime + 8000 ) < TICKCOUNT )
			{
				bCanClose = TRUE;
			}

			iX = ( UNITDATA->sPosition.iX - p->sPosition.iX ) >> 8;
			iZ = ( UNITDATA->sPosition.iZ - p->sPosition.iZ ) >> 8;
			int d = ( iX*iX ) + ( iZ*iZ );

			// Distance is far from player or can delete it?
			if( bCanClose || (p->bVisible && d > 0x320000) )
			{
				UnitData * pp = (*(UnitData**)0x00CF48D0);

				p->bVisible = FALSE;

				// Clear monster on memory
				p->Free();
			}
		}
	}
}

void UnitGame::AddEXP( INT64 iEXP )
{
	LARGE_INTEGER s;
	s.QuadPart = iEXP;

	if( UNITDATA->sCharacterData.iLevel < *(int*)( 0x04B06F14 ) )
		CALL_WITH_ARG2( pfnAddCharacterEXP, s.LowPart, s.HighPart );
}

INT64 UnitGame::GetEXPFromLevel( int iLevel )
{
	if ( iLevel > 0 && iLevel <= (*(int*)(0x04B06F14) + 1) )
	{
		return LevelTable[iLevel-1];
	}

	return 0;
}

BOOL UnitGame::OnLoseEXP()
{
	if( CHARACTERGAME->IsStageVillage() || MAP_ID == MAPID_BlessCastle || MAP_ID == MAPID_QuestArena || MAP_ID == MAPID_T5QuestArena ||
		MAP_ID == MAPID_Bellatra || (BATTLEROYALEHANDLER->InBattle()) )
		return FALSE;

	INT64 iEXPCharacter = CHARACTERGAME->GetEXP();
	
	LARGE_INTEGER liEXPLevel;
	liEXPLevel.QuadPart = GetEXPFromLevel( UNITDATA->sCharacterData.iLevel );

	INT64 iEXP = GetEXPFromLevel( UNITDATA->sCharacterData.iLevel + 1 ) - liEXPLevel.QuadPart;

	int iPercent = 3;

	BOOL bRessurectionItem = FALSE;

	if ( UNITDATA->sCharacterData.iLevel > 10 )
	{
		DWORD dwQuestFlag = READDWORD( 0x0362BA68 );

		if ( ((dwQuestFlag & 0x10) != 0) && (UNITDATA->sCharacterData.iLevel >= 85) )
			iPercent = 2;

		//Bless of Kalia?
		if ( CQUESTGAME->GetQuestInstance()->IsDoneQuest( QUESTID_GreenLiquidQuestStep2 ) && ITEMHANDLER->GetItemInventoryByCode( ITEMID_BlessOfKalia ) )
			iPercent = 1;

		ItemData * pcItemData = nullptr;
		if ( MAP_ID == MAPID_ForestDungeon )
			pcItemData = ITEMHANDLER->GetItemInventoryByCode( ITEMID_TropicalScroll );
		else
			pcItemData = ITEMHANDLER->GetItemInventoryByCode( ITEMID_RebirthScroll );

		if ( pcItemData )
		{
			bRessurectionItem = TRUE;

			ITEMHANDLER->UseItem( &pcItemData->sItem );
			ITEMHANDLER->DeleteItem( pcItemData );
			iPercent = 0;

			PacketLoseExperience sPacket;
			ZeroMemory( &sPacket, sizeof( PacketLoseExperience ) );
			sPacket.iLength = sizeof( PacketLoseExperience );
			sPacket.iHeader = PKTHDR_LoseExp;
			sPacket.iSubExp = 0;
			sPacket.bRessurectionItem = bRessurectionItem;
			sPacket.iLevel = UNITDATA->sCharacterData.iLevel;
			sPacket.iMapID = MAP_ID;
			SENDPACKETL( &sPacket );

			WRITEDWORD( 0x0209EA4C, READDWORD( 0x0CF4750 ) + 180000 );
		}

        if ( BATTLEROYALEHANDLER->InBattle() || BATTLEROYALEHANDLER->InWaiting() )
            iPercent = 0;

		if ( iPercent > 0 )
		{
			INT64 iExpLose = (INT64)round( ((double)iEXP * iPercent) / (double)100 );

			iEXP = iEXPCharacter - iExpLose;

			if ( iEXP < liEXPLevel.QuadPart )
			{
				iEXP = liEXPLevel.QuadPart;
				iExpLose = iEXPCharacter - liEXPLevel.QuadPart;
			}

			if ( iExpLose > 0 )
			{
				PacketLoseExperience sPacket;
				ZeroMemory( &sPacket, sizeof( PacketLoseExperience ) );
				sPacket.iLength = sizeof( PacketLoseExperience );
				sPacket.iHeader = PKTHDR_LoseExp;
				sPacket.iSubExp = iExpLose;
				sPacket.bRessurectionItem = bRessurectionItem;
				sPacket.iLevel = UNITDATA->sCharacterData.iLevel;
				sPacket.iMapID = MAP_ID;
				SENDPACKETL( &sPacket );

				LARGE_INTEGER li;

				li.QuadPart = iEXPCharacter - iExpLose;

				CHARACTERGAME->SetEXP( li.QuadPart );
			}

			int iGold = UNITDATA->sCharacterData.iGold / 100;

			if ( CHARACTERGAME->CanUseGold( iGold ) )
			{
				UNITDATA->sCharacterData.iGold -= iGold;

				ITEMHANDLER->SendGoldUse( iGold );
				ITEMHANDLER->ProcessGold();
				PLAYMINISOUND( 18 );
			}
		}

		UPDATE_CHARACTER_CHECKSUM;

		CALL( 0x0045DE30 );
	}

	return TRUE;
}

int UnitGame::GetCharacterAbsorption()
{
	// Get Absorption Character
	int iUnitAbsorption = UNITDATA->sCharacterData.iAbsorbRating;

	// Is Active?
	if( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_Zenith ) )
	{
		int iSkillLevel = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_Zenith );

		// Is Set skill?
		if( iSkillLevel )
		{
			int * iSkillTable = (int*)SKILLMANAGERHANDLER->SKILLTABLEID_ZenithAbs;
			int iPercentAbsorption = iSkillTable[iSkillLevel];
			iUnitAbsorption += iPercentAbsorption;
		}
	}

	// Is Active?
	if( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_GodlyShield ) )
	{
		int iSkillLevel = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_GodlyShield );

		// Is Set skill?
		if( iSkillLevel )
		{
			// Is Shield using?
			if( ( UNITDATA->sLeftHandTool.eItemID & 0xFFFF0000 ) == ITEMTYPE_Shield )
			{
				int * iSkillTable = (int*)SKILLMANAGERHANDLER->SKILLTABLEID_GodlyShieldAborsb;
				int iPercentAbsorption = iSkillTable[iSkillLevel - 1];

				if ( INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex )
				{
					auto pcItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex - 1];
					if ( pcItemData->bValid )
						iUnitAbsorption += ((int)pcItemData->sItem.fAbsorbRating * iPercentAbsorption) / 100;
				}
			}
		}
	}

	iUnitAbsorption += *(int*)0x035EB608;
	iUnitAbsorption += *(int*)0x035D0CDC;

	return iUnitAbsorption;
}

void UnitGame::ReceiveServerInformation( PacketServerInfo * psPacket )
{
	CopyMemory( &sServerInfo, psPacket, sizeof(PacketServerInfo) );
	GM_MODE = psPacket->bGameMaster;

	if( GM_MODE )
		*(int*)( 0x04B06F14 ) = 155;

	CopyMemory( &GameCore::GetInstance()->sServerTime, &sServerInfo.sServerTime, sizeof(SYSTEMTIME) );
}

void UnitGame::SetCharacterFormula( ItemData * pItem, ItemInventoryData * pInventory, CharacterDataLocal & sCharacterDataLocal )
{
	int iItemIndex = pInventory[0].iItemIndex - 1;
	int iItemClass = pItem[iItemIndex].iItemWpnDmg;
	int iItemSpec = pItem[iItemIndex].sItem.eSpecialization;

	int iGauntlets = pInventory[ITEMSLOT_Gauntlets - 1].iItemIndex - 1;
	int iGauntletSpec = pItem[iGauntlets].sItem.eSpecialization;

	const DWORD									pdwInventory = 0x035EBB20;
	const DWORD									pdwStrength = pdwInventory + 0x39C4C;
	const DWORD									pdwSpirit = pdwInventory + 0x39C58;
	const DWORD									pdwTalent = pdwInventory + 0x39C54;
	const DWORD									pdwAgility = pdwInventory + 0x39C5C;
	const DWORD									pdwHealth = pdwInventory + 0x39C50;

	int iStrength = PLAYERDATA->iStrength + ( *(int*)pdwStrength );
	int iSpirit = PLAYERDATA->iSpirit + ( *(int*)pdwSpirit );
	int iTalent = PLAYERDATA->iTalent + ( *(int*)pdwTalent );
	int iAgility = PLAYERDATA->iAgility + ( *(int*)pdwAgility );
	int iHealth = PLAYERDATA->iHealth + ( *(int*)pdwHealth );

#ifdef _NEW_DMG__

	PLAYERDATA->iMinDamage = 1 + (((iStrength)+130) / 130) + ((iTalent + iAgility) / 40);
	PLAYERDATA->iMaxDamage = 2 + ((iStrength + 130) / 130) + ((iTalent + iAgility) / 40);

	switch( PLAYERDATA->iClass )
	{
		case CHARACTERCLASS_Fighter:
		if( iItemClass == WEAPONCLASS_Melee )
		{
			PLAYERDATA->iMinDamage = 1 + ( ( sCharacterDataLocal.iMinDamage * ( iStrength + 260 ) / 260 ) ) + ((sCharacterDataLocal.iMinDamage * (iSpirit + 520) / 520)) + ((sCharacterDataLocal.iMinDamage * (iTalent + 400) / 400)) + ((sCharacterDataLocal.iMinDamage * (iAgility + 520) / 520)) - ((sCharacterDataLocal.iMinDamage * (iHealth + 500) / 400));
			PLAYERDATA->iMaxDamage = 3 + ((sCharacterDataLocal.iMaxDamage * (iStrength + 260) / 260)) + ((sCharacterDataLocal.iMaxDamage * (iSpirit + 520) / 520)) + ((sCharacterDataLocal.iMaxDamage * (iTalent + 400) / 400)) + ((sCharacterDataLocal.iMaxDamage * (iAgility + 520) / 520)) - ((sCharacterDataLocal.iMaxDamage * (iHealth + 500) / 400));
			if( iItemSpec == CLASSFLAG_Fighter )
			{
				PLAYERDATA->iMinDamage += ( sCharacterDataLocal.iMinDamage + sCharacterDataLocal.iMaxDamage ) / 16;
				if( pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv > 0 )
					PLAYERDATA->iMaxDamage += PLAYERDATA->iLevel / pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv;
			}
		}
		break;

		case CHARACTERCLASS_Mechanician:
		if( iItemClass == WEAPONCLASS_Melee || iItemClass == WEAPONCLASS_Ranged )
		{
			if( iItemClass == WEAPONCLASS_Ranged )
			{
				PLAYERDATA->iMinDamage = 1 + ((sCharacterDataLocal.iMinDamage * (iStrength + 560) / 560)) + ((sCharacterDataLocal.iMinDamage * (iSpirit + 600) / 600)) + ((sCharacterDataLocal.iMinDamage * (iTalent + 560) / 560)) + ((sCharacterDataLocal.iMinDamage * (iAgility + 320) / 320)) - ((sCharacterDataLocal.iMinDamage * (iHealth + 500) / 400));
				PLAYERDATA->iMaxDamage = 3 + ((sCharacterDataLocal.iMaxDamage * (iStrength + 560) / 560)) + ((sCharacterDataLocal.iMaxDamage * (iSpirit + 600) / 600)) + ((sCharacterDataLocal.iMaxDamage * (iTalent + 560) / 560)) + ((sCharacterDataLocal.iMaxDamage * (iAgility + 320) / 320)) - ((sCharacterDataLocal.iMaxDamage * (iHealth + 500) / 400));
			}
			else
			{
				PLAYERDATA->iMinDamage = 1 + ((sCharacterDataLocal.iMinDamage * (iStrength + 320) / 320)) + ((sCharacterDataLocal.iMinDamage * (iSpirit + 600) / 600)) + ((sCharacterDataLocal.iMinDamage * (iTalent + 560) / 560)) + ((sCharacterDataLocal.iMinDamage * (iAgility + 560) / 560)) - ((sCharacterDataLocal.iMinDamage * (iHealth + 500) / 400));
				PLAYERDATA->iMaxDamage = 3 + ((sCharacterDataLocal.iMaxDamage * (iStrength + 320) / 320)) + ((sCharacterDataLocal.iMaxDamage * (iSpirit + 600) / 600)) + ((sCharacterDataLocal.iMaxDamage * (iTalent + 560) / 560)) + ((sCharacterDataLocal.iMaxDamage * (iAgility + 560) / 560)) - ((sCharacterDataLocal.iMaxDamage * (iHealth + 500) / 400));
			}

			if( iItemSpec == CLASSFLAG_Mechanician )
			{
				PLAYERDATA->iMinDamage += ( sCharacterDataLocal.iMinDamage + sCharacterDataLocal.iMaxDamage ) / 16;
				if( pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv > 0 )
					PLAYERDATA->iMaxDamage += PLAYERDATA->iLevel / pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv;
			}
		}
		break;

		case CHARACTERCLASS_Archer:
		if( iItemClass == WEAPONCLASS_Ranged )
		{
			PLAYERDATA->iMinDamage = 2 + ((sCharacterDataLocal.iMinDamage * (iStrength + 400) / 400)) + ((sCharacterDataLocal.iMinDamage * (iSpirit + 520) / 520)) + ((sCharacterDataLocal.iMinDamage * (iTalent + 460) / 460)) + ((sCharacterDataLocal.iMinDamage * (iAgility + 260) / 260)) - ((sCharacterDataLocal.iMinDamage * (iHealth + 480) / 380));
			PLAYERDATA->iMaxDamage = 3 + ((sCharacterDataLocal.iMaxDamage * (iStrength + 400) / 400)) + ((sCharacterDataLocal.iMaxDamage * (iSpirit + 520) / 520)) + ((sCharacterDataLocal.iMaxDamage * (iTalent + 460) / 460)) + ((sCharacterDataLocal.iMaxDamage * (iAgility + 260) / 260)) - ((sCharacterDataLocal.iMaxDamage * (iHealth + 480) / 380));
			if( iItemSpec == CLASSFLAG_Archer )
			{
				PLAYERDATA->iMinDamage += ( sCharacterDataLocal.iMinDamage + sCharacterDataLocal.iMaxDamage ) / 16;
				if( pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv > 0 )
					PLAYERDATA->iMaxDamage += PLAYERDATA->iLevel / pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv;
			}
		}
		break;

		case CHARACTERCLASS_Pikeman:
		if( iItemClass == WEAPONCLASS_Melee )
		{
			PLAYERDATA->iMinDamage = 1 + ((sCharacterDataLocal.iMinDamage * (iStrength + 240) / 240)) + ((sCharacterDataLocal.iMinDamage * (iSpirit + 480) / 480)) + ((sCharacterDataLocal.iMinDamage * (iTalent + 420) / 420)) + ((sCharacterDataLocal.iMinDamage * (iAgility + 400) / 400)) - ((sCharacterDataLocal.iMinDamage * (iHealth + 500) / 400));
			PLAYERDATA->iMaxDamage = 3 + ((sCharacterDataLocal.iMaxDamage * (iStrength + 240) / 240)) + ((sCharacterDataLocal.iMaxDamage * (iSpirit + 480) / 480)) + ((sCharacterDataLocal.iMaxDamage * (iTalent + 420) / 420)) + ((sCharacterDataLocal.iMaxDamage * (iAgility + 400) / 400)) - ((sCharacterDataLocal.iMaxDamage * (iHealth + 500) / 400));
			if( iItemSpec == CLASSFLAG_Pikeman )
			{
				PLAYERDATA->iMinDamage += ( sCharacterDataLocal.iMinDamage + sCharacterDataLocal.iMaxDamage ) / 16;
				if( pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv > 0 )
					PLAYERDATA->iMaxDamage += PLAYERDATA->iLevel / pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv;
			}
		}
		break;

		case CHARACTERCLASS_Atalanta:
		if( iItemClass == WEAPONCLASS_Ranged )
		{
			PLAYERDATA->iMinDamage = 2 + ((sCharacterDataLocal.iMinDamage * (iStrength + 400) / 400)) + ((sCharacterDataLocal.iMinDamage * (iSpirit + 460) / 460)) + ((sCharacterDataLocal.iMinDamage * (iTalent + 520) / 520)) + ((sCharacterDataLocal.iMinDamage * (iAgility + 260) / 260)) - ((sCharacterDataLocal.iMinDamage * (iHealth + 480) / 380));
			PLAYERDATA->iMaxDamage = 3 + ((sCharacterDataLocal.iMaxDamage * (iStrength + 400) / 400)) + ((sCharacterDataLocal.iMaxDamage * (iSpirit + 460) / 460)) + ((sCharacterDataLocal.iMaxDamage * (iTalent + 520) / 520)) + ((sCharacterDataLocal.iMaxDamage * (iAgility + 260) / 260)) - ((sCharacterDataLocal.iMaxDamage * (iHealth + 480) / 380));
			if( iItemSpec == CLASSFLAG_Atalanta )
			{
				PLAYERDATA->iMinDamage += ( sCharacterDataLocal.iMinDamage + sCharacterDataLocal.iMaxDamage ) / 16;
				if( pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv > 0 )
					PLAYERDATA->iMaxDamage += PLAYERDATA->iLevel / pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv;
			}
		}
		break;

		case CHARACTERCLASS_Knight:
		if( iItemClass == WEAPONCLASS_Melee )
		{
			PLAYERDATA->iMinDamage = 1 + ((sCharacterDataLocal.iMinDamage * (iStrength + 300) / 300)) + ((sCharacterDataLocal.iMinDamage * (iSpirit + 520) / 520)) + ((sCharacterDataLocal.iMinDamage * (iTalent + 440) / 440)) + ((sCharacterDataLocal.iMinDamage * (iAgility + 600) / 600)) - ((sCharacterDataLocal.iMinDamage * (iHealth + 500) / 400));
			PLAYERDATA->iMaxDamage = 3 + ((sCharacterDataLocal.iMaxDamage * (iStrength + 300) / 300)) + ((sCharacterDataLocal.iMaxDamage * (iSpirit + 520) / 520)) + ((sCharacterDataLocal.iMaxDamage * (iTalent + 440) / 440)) + ((sCharacterDataLocal.iMaxDamage * (iAgility + 600) / 600)) - ((sCharacterDataLocal.iMaxDamage * (iHealth + 500) / 400));
			if( iItemSpec == CLASSFLAG_Knight )
			{
				PLAYERDATA->iMinDamage += ( sCharacterDataLocal.iMinDamage + sCharacterDataLocal.iMaxDamage ) / 16;
				if( pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv > 0 )
					PLAYERDATA->iMaxDamage += PLAYERDATA->iLevel / pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv;
			}
		}
		break;

		case CHARACTERCLASS_Magician:
		if( iItemClass == WEAPONCLASS_Magic )
		{
			PLAYERDATA->iMinDamage = 2 + ((sCharacterDataLocal.iMinDamage * (iStrength + 520) / 520)) + ((sCharacterDataLocal.iMinDamage * (iSpirit + 260) / 260)) + ((sCharacterDataLocal.iMinDamage * (iTalent + 440) / 440)) + ((sCharacterDataLocal.iMinDamage * (iAgility + 480) / 480)) - ((sCharacterDataLocal.iMinDamage * (iHealth + 300) / 300));
			PLAYERDATA->iMaxDamage = 3 + ((sCharacterDataLocal.iMaxDamage * (iStrength + 520) / 520)) + ((sCharacterDataLocal.iMaxDamage * (iSpirit + 260) / 260)) + ((sCharacterDataLocal.iMaxDamage * (iTalent + 440) / 440)) + ((sCharacterDataLocal.iMaxDamage * (iAgility + 480) / 480)) - ((sCharacterDataLocal.iMaxDamage * (iHealth + 300) / 300));
			if( iItemSpec == CLASSFLAG_Magician )
			{
				PLAYERDATA->iMinDamage += ( sCharacterDataLocal.iMinDamage + sCharacterDataLocal.iMaxDamage ) / 16;
				if( pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv > 0 )
					PLAYERDATA->iMaxDamage += PLAYERDATA->iLevel / pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv;
			}
		}
		break;

		case CHARACTERCLASS_Priestess:
		if( iItemClass == WEAPONCLASS_Magic )
		{
			PLAYERDATA->iMinDamage = 2 + ((sCharacterDataLocal.iMinDamage * (iStrength + 480) / 480)) + ((sCharacterDataLocal.iMinDamage * (iSpirit + 240) / 240)) + ((sCharacterDataLocal.iMinDamage * (iTalent + 400) / 400)) + ((sCharacterDataLocal.iMinDamage * (iAgility + 480) / 480)) - ((sCharacterDataLocal.iMinDamage * (iHealth + 600) / 500));
			PLAYERDATA->iMaxDamage = 3 + ((sCharacterDataLocal.iMaxDamage * (iStrength + 480) / 480)) + ((sCharacterDataLocal.iMaxDamage * (iSpirit + 240) / 240)) + ((sCharacterDataLocal.iMaxDamage * (iTalent + 400) / 400)) + ((sCharacterDataLocal.iMaxDamage * (iAgility + 480) / 480)) - ((sCharacterDataLocal.iMaxDamage * (iHealth + 600) / 500));
			if( iItemSpec == CLASSFLAG_Priestess )
			{
				PLAYERDATA->iMinDamage += ( sCharacterDataLocal.iMinDamage + sCharacterDataLocal.iMaxDamage ) / 16;
				if( pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv > 0 )
					PLAYERDATA->iMaxDamage += PLAYERDATA->iLevel / pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv;
			}
		}
		break;

		case CHARACTERCLASS_Assassin:
		if( iItemClass == WEAPONCLASS_Melee )
		{
			PLAYERDATA->iMinDamage = 1 + ((sCharacterDataLocal.iMinDamage * (iStrength + 240) / 240)) + ((sCharacterDataLocal.iMinDamage * (iSpirit + 480) / 480)) + ((sCharacterDataLocal.iMinDamage * (iTalent + 420) / 420)) + ((sCharacterDataLocal.iMinDamage * (iAgility + 400) / 400)) - ((sCharacterDataLocal.iMinDamage * (iHealth + 500) / 400));
			PLAYERDATA->iMaxDamage = 3 + ((sCharacterDataLocal.iMaxDamage * (iStrength + 240) / 240)) + ((sCharacterDataLocal.iMaxDamage * (iSpirit + 480) / 480)) + ((sCharacterDataLocal.iMaxDamage * (iTalent + 420) / 420)) + ((sCharacterDataLocal.iMaxDamage * (iAgility + 400) / 400)) - ((sCharacterDataLocal.iMaxDamage * (iHealth + 500) / 400));
			if( iItemSpec == CLASSFLAG_Assassin )
			{
				PLAYERDATA->iMinDamage += ( sCharacterDataLocal.iMinDamage + sCharacterDataLocal.iMaxDamage ) / 16;
				if( pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv > 0 )
					PLAYERDATA->iMaxDamage += PLAYERDATA->iLevel / pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv;
			}
		}
		break;

		case CHARACTERCLASS_Shaman:
		if( iItemClass == WEAPONCLASS_Magic )
		{
			PLAYERDATA->iMinDamage = 2 + ((sCharacterDataLocal.iMinDamage * (iStrength + 520) / 520)) + ((sCharacterDataLocal.iMinDamage * (iSpirit + 260) / 260)) + ((sCharacterDataLocal.iMinDamage * (iTalent + 440) / 440)) + ((sCharacterDataLocal.iMinDamage * (iAgility + 480) / 480)) - ((sCharacterDataLocal.iMinDamage * (iHealth + 480) / 380));
			PLAYERDATA->iMaxDamage = 3 + ((sCharacterDataLocal.iMaxDamage * (iStrength + 520) / 520)) + ((sCharacterDataLocal.iMaxDamage * (iSpirit + 260) / 260)) + ((sCharacterDataLocal.iMaxDamage * (iTalent + 440) / 440)) + ((sCharacterDataLocal.iMaxDamage * (iAgility + 480) / 480)) - ((sCharacterDataLocal.iMaxDamage * (iHealth + 480) / 380));
			if( iItemSpec == CLASSFLAG_Shaman )
			{
				PLAYERDATA->iMinDamage += ( sCharacterDataLocal.iMinDamage + sCharacterDataLocal.iMaxDamage ) / 16;
				if( pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv > 0 )
					PLAYERDATA->iMaxDamage += PLAYERDATA->iLevel / pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv;
			}
		}
		break;

		default:
		break;
	}

	if ( CharacterClassToClassFlag( (ECharacterClass)PLAYERDATA->iClass ) == iGauntletSpec )
	{
		if ( pItem[iGauntlets].sItem.sSpecData.iSpecAttackPowerDiv > 0 )
			PLAYERDATA->iMaxDamage += PLAYERDATA->iLevel / pItem[iGauntlets].sItem.sSpecData.iSpecAttackPowerDiv;
	}


#else

	PLAYERDATA->iMinDamage = 1 + (((iStrength)+130) / 130) + ((iTalent + iAgility) / 40);
	PLAYERDATA->iMaxDamage = 2 + ((iStrength + 130) / 130) + ((iTalent + iAgility) / 40);


	switch ( PLAYERDATA->iClass )
	{
		case CHARACTERCLASS_Fighter:
			if ( iItemClass == WEAPONCLASS_Melee )
			{
				PLAYERDATA->iMinDamage = 1 + ((sCharacterDataLocal.iMinDamage * (iStrength + 130) / 130)) + ((iTalent + iAgility) / 40);
				PLAYERDATA->iMaxDamage = 3 + ((sCharacterDataLocal.iMaxDamage * (iStrength + 130) / 130)) + ((iTalent + iAgility) / 35);
				if ( iItemSpec == CLASSFLAG_Fighter )
				{
					PLAYERDATA->iMinDamage += (sCharacterDataLocal.iMinDamage + sCharacterDataLocal.iMaxDamage) / 16;
					if ( pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv > 0 )
						PLAYERDATA->iMaxDamage += PLAYERDATA->iLevel / pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv;
				}
			}
			break;

		case CHARACTERCLASS_Mechanician:
			if ( iItemClass == WEAPONCLASS_Melee || iItemClass == WEAPONCLASS_Ranged )
			{
				if ( iItemClass == WEAPONCLASS_Ranged )
				{
					PLAYERDATA->iMinDamage = 1 + ((sCharacterDataLocal.iMinDamage * (iAgility + 130) / 130)) + ((iTalent + iAgility) / 40);
					PLAYERDATA->iMaxDamage = 3 + ((sCharacterDataLocal.iMaxDamage * (iAgility + 130) / 130)) + ((iTalent + iAgility) / 35);
				}
				else
				{
					PLAYERDATA->iMinDamage = 1 + ((sCharacterDataLocal.iMinDamage * (iStrength + 160) / 160)) + ((iTalent + iAgility) / 40);
					PLAYERDATA->iMaxDamage = 3 + ((sCharacterDataLocal.iMaxDamage * (iStrength + 160) / 160)) + ((iTalent + iAgility) / 35);
				}

				if ( iItemSpec == CLASSFLAG_Mechanician )
				{
					PLAYERDATA->iMinDamage += (sCharacterDataLocal.iMinDamage + sCharacterDataLocal.iMaxDamage) / 16;
					if ( pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv > 0 )
						PLAYERDATA->iMaxDamage += PLAYERDATA->iLevel / pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv;
				}
			}
			break;

		case CHARACTERCLASS_Archer:
			if ( iItemClass == WEAPONCLASS_Ranged )
			{
				PLAYERDATA->iMinDamage = 2 + ((sCharacterDataLocal.iMinDamage * (iAgility + 130) / 130)) + ((iTalent + iAgility) / 50);
				PLAYERDATA->iMaxDamage = 3 + ((sCharacterDataLocal.iMaxDamage * (iAgility + 130) / 130)) + ((iTalent + iAgility) / 45);
				if ( iItemSpec == CLASSFLAG_Archer )
				{
					PLAYERDATA->iMinDamage += (sCharacterDataLocal.iMinDamage + sCharacterDataLocal.iMaxDamage) / 16;
					if ( pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv > 0 )
						PLAYERDATA->iMaxDamage += PLAYERDATA->iLevel / pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv;
				}
			}
			break;

		case CHARACTERCLASS_Pikeman:
			if ( iItemClass == WEAPONCLASS_Melee )
			{
				PLAYERDATA->iMinDamage = 1 + ((sCharacterDataLocal.iMinDamage * (iStrength + 120) / 120)) + ((iTalent + iAgility) / 40);
				PLAYERDATA->iMaxDamage = 3 + ((sCharacterDataLocal.iMaxDamage * (iStrength + 120) / 120)) + ((iTalent + iAgility) / 35);
				if ( iItemSpec == CLASSFLAG_Pikeman )
				{
					PLAYERDATA->iMinDamage += (sCharacterDataLocal.iMinDamage + sCharacterDataLocal.iMaxDamage) / 16;
					if ( pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv > 0 )
						PLAYERDATA->iMaxDamage += PLAYERDATA->iLevel / pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv;
				}
			}
			break;

		case CHARACTERCLASS_Atalanta:
			if ( iItemClass == WEAPONCLASS_Ranged )
			{
				PLAYERDATA->iMinDamage = 2 + ((sCharacterDataLocal.iMinDamage * (iAgility + 130) / 130)) + ((iTalent + iAgility) / 50);
				PLAYERDATA->iMaxDamage = 3 + ((sCharacterDataLocal.iMaxDamage * (iAgility + 130) / 130)) + ((iTalent + iAgility) / 45);
				if ( iItemSpec == CLASSFLAG_Atalanta )
				{
					PLAYERDATA->iMinDamage += (sCharacterDataLocal.iMinDamage + sCharacterDataLocal.iMaxDamage) / 16;
					if ( pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv > 0 )
						PLAYERDATA->iMaxDamage += PLAYERDATA->iLevel / pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv;
				}
			}
			break;

		case CHARACTERCLASS_Knight:
			if ( iItemClass == WEAPONCLASS_Melee )
			{
				PLAYERDATA->iMinDamage = 1 + ((sCharacterDataLocal.iMinDamage * (iStrength + 150) / 150)) + ((iTalent + iAgility) / 40);
				PLAYERDATA->iMaxDamage = 3 + ((sCharacterDataLocal.iMaxDamage * (iStrength + 150) / 150)) + ((iTalent + iAgility) / 35);
				if ( iItemSpec == CLASSFLAG_Knight )
				{
					PLAYERDATA->iMinDamage += (sCharacterDataLocal.iMinDamage + sCharacterDataLocal.iMaxDamage) / 16;
					if ( pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv > 0 )
						PLAYERDATA->iMaxDamage += PLAYERDATA->iLevel / pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv;
				}
			}
			break;

		case CHARACTERCLASS_Magician:
			if ( iItemClass == WEAPONCLASS_Magic )
			{
				PLAYERDATA->iMinDamage = 2 + ((sCharacterDataLocal.iMinDamage * (iSpirit + 130) / 130)) + ((iTalent + iAgility) / 50);
				PLAYERDATA->iMaxDamage = 3 + ((sCharacterDataLocal.iMaxDamage * (iSpirit + 130) / 130)) + ((iTalent + iAgility) / 45);
				if ( iItemSpec == CLASSFLAG_Magician )
				{
					PLAYERDATA->iMinDamage += (sCharacterDataLocal.iMinDamage + sCharacterDataLocal.iMaxDamage) / 16;
					if ( pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv > 0 )
						PLAYERDATA->iMaxDamage += PLAYERDATA->iLevel / pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv;
				}
			}
			break;

		case CHARACTERCLASS_Priestess:
			if ( iItemClass == WEAPONCLASS_Magic )
			{
				PLAYERDATA->iMinDamage = 2 + ((sCharacterDataLocal.iMinDamage * (iSpirit + 120) / 120)) + ((iTalent + iAgility) / 50);
				PLAYERDATA->iMaxDamage = 3 + ((sCharacterDataLocal.iMaxDamage * (iSpirit + 120) / 120)) + ((iTalent + iAgility) / 45);
				if ( iItemSpec == CLASSFLAG_Priestess )
				{
					PLAYERDATA->iMinDamage += (sCharacterDataLocal.iMinDamage + sCharacterDataLocal.iMaxDamage) / 16;
					if ( pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv > 0 )
						PLAYERDATA->iMaxDamage += PLAYERDATA->iLevel / pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv;
				}
			}
			break;

		case CHARACTERCLASS_Assassin:
			if ( iItemClass == WEAPONCLASS_Melee )
			{
				PLAYERDATA->iMinDamage = 1 + ((sCharacterDataLocal.iMinDamage * (iStrength + 130) / 130)) + ((iTalent + iAgility) / 40);
				PLAYERDATA->iMaxDamage = 3 + ((sCharacterDataLocal.iMaxDamage * (iStrength + 130) / 130)) + ((iTalent + iAgility) / 35);
				if ( iItemSpec == CLASSFLAG_Assassin )
				{
					PLAYERDATA->iMinDamage += (sCharacterDataLocal.iMinDamage + sCharacterDataLocal.iMaxDamage) / 16;
					if ( pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv > 0 )
						PLAYERDATA->iMaxDamage += PLAYERDATA->iLevel / pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv;
				}
			}
			break;

		case CHARACTERCLASS_Shaman:
			if ( iItemClass == WEAPONCLASS_Magic )
			{
				PLAYERDATA->iMinDamage = 2 + ((sCharacterDataLocal.iMinDamage * (iSpirit + 130) / 130)) + ((iTalent + iAgility) / 50);
				PLAYERDATA->iMaxDamage = 3 + ((sCharacterDataLocal.iMaxDamage * (iSpirit + 130) / 130)) + ((iTalent + iAgility) / 45);
				if ( iItemSpec == CLASSFLAG_Shaman )
				{
					PLAYERDATA->iMinDamage += (sCharacterDataLocal.iMinDamage + sCharacterDataLocal.iMaxDamage) / 16;
					if ( pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv > 0 )
						PLAYERDATA->iMaxDamage += PLAYERDATA->iLevel / pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv;
				}
			}
			break;

		default:
			break;
	}

	if ( CharacterClassToClassFlag( (ECharacterClass)PLAYERDATA->iClass ) == iGauntletSpec )
	{
		if ( pItem[iGauntlets].sItem.sSpecData.iSpecAttackPowerDiv > 0 )
			PLAYERDATA->iMaxDamage += PLAYERDATA->iLevel / pItem[iGauntlets].sItem.sSpecData.iSpecAttackPowerDiv;
	}

#endif

	PLAYERDATA->iMinDamage += sCharacterDataLocal.iMinDamageSkill + ( PLAYERDATA->iLevel / 6 );
	PLAYERDATA->iMaxDamage += sCharacterDataLocal.iMaxDamageSkill + ( PLAYERDATA->iLevel / 6 );
}

int UnitGame::DefendChance( int iDefenseRating, int iAttackRating )
{
	//Chance to block = ((def - atr) / def) * 100
	if( iDefenseRating <= iAttackRating )
		return 10;

	double def = iDefenseRating;
	double def_rtg = iDefenseRating - iAttackRating;
	double mul = 100.0f;
	double ret = ( def_rtg / def ) * mul;

	if( ret > 90.0f )
		return 90;
	if( ret < 10.0f )
		return 10;

	return (int)ret;
}

int UnitGame::BlockChance( int iBlockRating, int iBlock )
{
	//Extreme Shield
	if( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_ExtremeShield ) )
	{
		// Extreme Shield LVL
		int iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_ExtremeShield );
		if( iLevelSkill )
		{
			int * iSkillTable = (int*)SKILLMANAGERHANDLER->SKILLTABLEID_ExtremeShieldBlock;
			int iBlockPercentSkill = iSkillTable[iLevelSkill - 1];
			iBlock += ( *(int*)0x4B0B140 * iBlockPercentSkill ) / 100;
		}
	}

	// Attack Mastery Block (Assassin)
	int iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( 0x10020004 );
	if( iLevelSkill )
	{
		if( ( UNITDATA->sRightHandTool.eItemID & 0xFFFF0000 ) == 0x010A0000 )
		{
			ItemData * psItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[0].iItemIndex - 1];
			if( psItemData )
			{
				int * iSkillTable = (int*)0x009981FC;
				int iBlockPercentSkill = iSkillTable[iLevelSkill];
				iBlock += ( (int)psItemData->sItem.fBlockRating * iBlockPercentSkill ) / 100;
			}
		}
	}

	return iBlock;
}

void UnitGame::RenderDebugText()
{
	const int iXDebug = 8;
	const int iYDebug = 2;
	int iYPos = 0;

	auto pFont = GetGraphicsDevice()->GetDefaultFont();

	std::vector<std::string> vServerInfo;
	std::vector<std::string> vUserInfo;
	std::vector<std::string> vCharacterInfo;

	//Server Name
	vServerInfo.push_back( FormatString( "Server Name: %s", (char *)(0x038FEF40 + ((*(int *)0x4B06960 - 1) * 0x68)) ) );

	//Server Version
	vServerInfo.push_back( FormatString( "Server Version: %d", sServerInfo.iServerVersion ) );

	//Users Online
	vServerInfo.push_back( FormatString( "Users Online: %d/%d", sServerInfo.iUsers, sServerInfo.iMaxUsers ) );

	//Boss Time
	vServerInfo.push_back( FormatString( "Boss Time: xx:%02d", sServerInfo.iBossTime ) );

	iYPos += 30;

	//Account
	vUserInfo.push_back( FormatString( "Account: %s", (char*)0x39033E8 ) );

	//Character Name
	vUserInfo.push_back( FormatString( "Character Name: %s [%d]", (char*)*(UINT*)0x362A3B8, UNITDATA->iID ) );

	//FPS
	vUserInfo.push_back( FormatString( "FPS: %d", *(int*)0x0A1755C ) );

	//X
	vUserInfo.push_back( FormatString( "X: %d [%d] AX(%d) CX(%.2f)", UNITDATA->sPosition.iX, UNITDATA->sPosition.iX >> 8, UNITDATA->sAngle.iX & PTANGLE_Mask,
		GRAPHICENGINE->GetSRenderer()->GetActiveCamera()->GetEye().x ) );

	//Y
	vUserInfo.push_back( FormatString( "Y: %d [%d] AY(%d) CY(%.2f)", UNITDATA->sPosition.iY, UNITDATA->sPosition.iY >> 8, UNITDATA->sAngle.iY & PTANGLE_Mask,
		GRAPHICENGINE->GetSRenderer()->GetActiveCamera()->GetEye().y ) );

	//Z
	vUserInfo.push_back( FormatString( "Z: %d [%d] AZ(%d) CZ(%.2f)", UNITDATA->sPosition.iZ, UNITDATA->sPosition.iZ >> 8, UNITDATA->sAngle.iZ & PTANGLE_Mask,
		GRAPHICENGINE->GetSRenderer()->GetActiveCamera()->GetEye().z ) );

	//Stage Area
	extern int STAGE_DRAWS_COUNT;

	static int LASTTEXTUREFRAME = 0;
	static DWORD dwTimeTextureFrame = 0;

	static int TEXTUREFRAME_PER_SECOND = 0;
	if ( dwTimeTextureFrame < TICKCOUNT )
	{
		TEXTUREFRAME_PER_SECOND = TEXTUREFRAME - LASTTEXTUREFRAME;
		LASTTEXTUREFRAME = TEXTUREFRAME;
		dwTimeTextureFrame = TICKCOUNT + 1000;
	}

	vUserInfo.push_back( FormatString( "Stage Area: %d Draws[%d] TextureFrame[%d] PerSecond[%d]", CHARACTERGAME->GetStageID(), STAGE_DRAWS_COUNT, TEXTUREFRAME, TEXTUREFRAME_PER_SECOND ) );

	//Stage Area
	vUserInfo.push_back( FormatString( "Packets Info: [%d][%d][%d] | [%d][%d][%d]", SOCKETL ? SOCKETL->iCountIN : 0, SOCKETL ? SOCKETL->iCountOUTB : 0, SOCKETL ? (TICKCOUNT - SOCKETL->dwTimeIN) : 0,
									   SOCKETG ? SOCKETG->iCountIN : 0, SOCKETG ? SOCKETG->iCountOUTB : 0, SOCKETG ? (TICKCOUNT - SOCKETG->dwTimeIN) : 0 ) );

	//Weapon
	vCharacterInfo.push_back( FormatString( "Weapon ID: %04X (%d)", UNITDATA->eWeaponItemID, UNITDATA->bIsWeaponTwoHanded ) );

	vCharacterInfo.push_back( FormatString( "Weapon Equip Mode: %04X", UNITDATA->eToolEquipModeTown ) );

	//Animation
	vCharacterInfo.push_back( FormatString( "Animation ID: 0x%02X FPS(%d)", UNITDATA->iAnimationID, UNITDATA->iFPS ) );

	vCharacterInfo.push_back( FormatString( "Animation Frame: %d", UNITDATA->iFrame ) );

	vCharacterInfo.push_back( FormatString( "Animation Type: 0x%02X", UNITDATA->psModelAnimation->iType ) );

	vCharacterInfo.push_back( FormatString( "Enviroment Color: [%d][%d][%d]", READDWORD( 0x00CF4784 ), READDWORD( 0x00CF4788 ), READDWORD( 0x00CF478C ) ) );

	DWORD dwParticles = ((*(DWORD*)(0x039032CC + 0x0C)) - (*(DWORD*)(0x039032CC + 0x08))) / 0x08C;

	vCharacterInfo.push_back( FormatString( "# Particles: %d", dwParticles ) );

	for ( auto str : vServerInfo )
	{
		pFont->Draw( str, iXDebug, iYDebug + iYPos, 0, DX::Color( 255, 255, 255, 255 ) );

		iYPos += 15;
	}

	iYPos += 15;
	for ( auto str : vUserInfo )
	{
		pFont->Draw( str, iXDebug, iYDebug + iYPos, 0, DX::Color( 255, 255, 255, 255 ) );

		iYPos += 15;
	}

	iYPos += 15;
	for ( auto str : vCharacterInfo )
	{
		pFont->Draw( str, iXDebug, iYDebug + iYPos, 0, DX::Color( 255, 255, 255, 255 ) );

		iYPos += 15;
	}

	struct GMSeeUnitInfo
	{
		char			  szName[64];
		int				  iTimes;
		BOOL			  bPlayer;
		BOOL			  bFriendly; //TRUE is blue else red
		int				  iDeaths;
	};

	std::vector<GMSeeUnitInfo> vUnits;
	GMSeeUnitInfo * unit;
	GMSeeUnitInfo nunit;

	int iTotalUnits = 0, iTotalUnitsDeath = 0, iTotalPlayers = 0, iTotalPlayersDeath = 0, iTotalFriendly = 0, iTotalFriendlyDeath = 0;

	for( int i = 0; i < MAX_UNITS; i++ )
	{
		UnitData * ud = pcaUnitData + i;

		if( ud->bActive )
		{
			bool bNew = true;
			for( std::vector<GMSeeUnitInfo>::iterator it = vUnits.begin(); it != vUnits.end(); it++ )
			{
				GMSeeUnitInfo * cur = &( *it );

				if( lstrcmpiA( ud->sCharacterData.szName, cur->szName ) == 0 )
				{
					unit = cur;
					bNew = false;
					break;
				}
			}

			if( bNew )
			{
				unit = &nunit;

				StringCchCopyA( unit->szName, _countof( unit->szName ), ud->sCharacterData.szName );
				unit->iTimes = 1;
				unit->bPlayer = FALSE;
				unit->bFriendly = FALSE;
				unit->iDeaths = (ud->IsDead() || (ud->sCharacterData.sHP.sCur == 0)) ? 1 : 0;

				if( ud->sCharacterData.iType == CHARACTERTYPE_Monster )
				{
					if ( ud->sCharacterData.iType == CHARACTERTYPE_Player )
					{
						unit->bPlayer = TRUE;
						iTotalPlayers++;
						iTotalPlayersDeath += (ud->IsDead() || (ud->sCharacterData.sHP.sCur == 0)) ? 1 : 0;
					}
					else
					{
						iTotalUnits++;
						iTotalUnitsDeath += (ud->IsDead() || (ud->sCharacterData.sHP.sCur == 0)) ? 1 : 0;
					}
				}
				else if( ud->sCharacterData.iType == CHARACTERTYPE_Player )
				{
					unit->bPlayer = TRUE;
					unit->bFriendly = TRUE;

					iTotalPlayers++;
					iTotalPlayersDeath += (ud->IsDead() || (ud->sCharacterData.sHP.sCur == 0)) ? 1 : 0;
				}
				else if( ud->sCharacterData.iType == CHARACTERTYPE_NPC )
					unit->bFriendly = TRUE;

				vUnits.push_back( *unit );
			}
			else
			{
				unit->iTimes++;
				unit->iDeaths += (ud->IsDead() || (ud->sCharacterData.sHP.sCur == 0)) ? 1 : 0;

				if ( ud->sCharacterData.iType == CHARACTERTYPE_Monster )
				{
					if ( ud->sCharacterData.iType == CHARACTERTYPE_Player )
					{
						iTotalPlayers++;
						iTotalPlayersDeath += (ud->IsDead() || (ud->sCharacterData.sHP.sCur == 0)) ? 1 : 0;
					}
					else
					{
						iTotalUnits++;
						iTotalUnitsDeath += (ud->IsDead() || (ud->sCharacterData.sHP.sCur == 0)) ? 1 : 0;
					}
				}
				else if ( ud->sCharacterData.iType == CHARACTERTYPE_Player )
				{
					iTotalPlayers++;
					iTotalPlayersDeath += (ud->IsDead() || (ud->sCharacterData.sHP.sCur == 0)) ? 1 : 0;
				}
			}
		}
	}

	int iLeft = 0, iRight = 0;

	pFont->Draw( FormatString( "Total Units: %d (%d)[%d]", iTotalUnits, iTotalUnitsDeath, iTotalUnits - iTotalUnitsDeath ), 350, 4 + (iLeft++ * 15), DT_WORDBREAK, DX::Color( 255, 255, 255, 255 ) );
	pFont->Draw( FormatString( "Total Players: %d (%d)[%d]", iTotalPlayers, iTotalPlayersDeath, iTotalPlayers - iTotalPlayersDeath ), 350 + 200, 4 + (iRight++ * 15), DT_WORDBREAK, DX::Color( 255, 255, 255, 255 ) );

	for( std::vector<GMSeeUnitInfo>::iterator it = vUnits.begin(); it != vUnits.end(); it++ )
	{
		GMSeeUnitInfo cur = *it;

		if( !cur.bPlayer )
		{
			if( cur.bFriendly )
				pFont->Draw( FormatString( "%s x %d (%d)[%d]", cur.szName, cur.iTimes, cur.iDeaths, cur.iTimes - cur.iDeaths ), 350, 4 + (iLeft++ * 15), 0, DX::Color( 255, 104, 202, 230 ) );
			else
				pFont->Draw( FormatString( "%s x %d (%d)[%d]", cur.szName, cur.iTimes, cur.iDeaths, cur.iTimes - cur.iDeaths ), 350, 4 + (iLeft++ * 15), 0, DX::Color( 255, 255, 128, 96 ) );
		}
		else
			pFont->Draw( FormatString( "%s x %d (%d)[%d]", cur.szName, cur.iTimes, cur.iDeaths, cur.iTimes - cur.iDeaths ), 350 + 200, 4 + (iRight++ * 15), DT_WORDBREAK, DX::Color( 255, 255, 255, 255 ) );
	}
}

void UnitGame::RenderCharacterDebugText()
{
	int iXDebug = RESOLUTION_WIDTH - 80;
	int iYDebug = *(int*)0x82BF08;

	const int iLabelPosition = 180;
	const int iValuePosition = 65;

	int iPaddingBottom = (RESOLUTION_HEIGHT >> 2) - 30;

	//Update UnitData Pointer
	if( iLastObjectID == 0 )
		pcViewUnitData = UNITDATABYID( *(DWORD*)( 0x207C888 + 0x10 ) );
	else if( *(DWORD*)( 0x207C888 + 0x10 ) != iLastObjectID )
		pcViewUnitData = UNITDATABYID( *(DWORD*)( 0x207C888 + 0x10 ) );

	if ( pcViewUnitData == NULL || pcViewUnitData->bActive == FALSE || pcViewUnitData->bVisible == FALSE )
		return;

	UnitData * pcTargetView = (UnitData*)(0x0207C888);
	
	if ( pcTargetView->sRightHandTool.eItemID != iLastUnitDataViewItemID )
	{
		iLastUnitDataViewItemID = pcTargetView->sRightHandTool.eItemID;

		CopyMemory( pcTargetView, pcViewUnitData, sizeof( UnitData ) );

		pcTargetView->Animate( ANIMATIONTYPE_Idle );
	}

	std::vector<std::string> vCharacterInfoDataLeft, vCharacterInfoDataRight;
	
	//Level
	vCharacterInfoDataLeft.push_back( "Level: " );
	vCharacterInfoDataRight.push_back( FormatString( "%d", pcViewUnitData->sCharacterData.iLevel ) );
	
	//HP
	vCharacterInfoDataLeft.push_back( "HP: " );
	vCharacterInfoDataRight.push_back( FormatString( "%d / %d", pcViewUnitData->sCharacterData.sHP.sCur, pcViewUnitData->sCharacterData.sHP.sMax ) );
	
	//MP
	vCharacterInfoDataLeft.push_back( "MP: " );
	vCharacterInfoDataRight.push_back( FormatString( "%d / %d", pcViewUnitData->sCharacterData.sMP.sCur, pcViewUnitData->sCharacterData.sMP.sMax ) );
	
	//SP
	vCharacterInfoDataLeft.push_back( "SP: " );
	vCharacterInfoDataRight.push_back( FormatString( "%d / %d", pcViewUnitData->sCharacterData.sSP.sCur, pcViewUnitData->sCharacterData.sSP.sMax ) );
	
	//Defense
	vCharacterInfoDataLeft.push_back( "Defense: " );
	vCharacterInfoDataRight.push_back( FormatString( "%d", pcViewUnitData->sCharacterData.iDefenseRating ) );
	
	//fAbsorbRating
	vCharacterInfoDataLeft.push_back( "Absorb: " );
	vCharacterInfoDataRight.push_back( FormatString( "%d", pcViewUnitData->sCharacterData.iAbsorbRating ) );
	
	//Block
	vCharacterInfoDataLeft.push_back( "Block: " );
	vCharacterInfoDataRight.push_back( FormatString( "%d", pcViewUnitData->sCharacterData.iBlockRating ) );
	
	//sDamage
	vCharacterInfoDataLeft.push_back( "Damage: " );
	vCharacterInfoDataRight.push_back( FormatString( "%d - %d", pcViewUnitData->sCharacterData.iMinDamage, pcViewUnitData->sCharacterData.iMaxDamage ) );
	
	//Attack Rating
	vCharacterInfoDataLeft.push_back( "Attack Rating: " );
	vCharacterInfoDataRight.push_back( FormatString( "%d", pcViewUnitData->sCharacterData.iAttackRating ) );
	
	//Range
	vCharacterInfoDataLeft.push_back( "Range: " );
	vCharacterInfoDataRight.push_back( FormatString( "%d", pcViewUnitData->sCharacterData.iAttackRange ) );
	
	//Attack fMovementSpeed
	vCharacterInfoDataLeft.push_back( "Attack Speed: " );
	vCharacterInfoDataRight.push_back( FormatString( "%d", pcViewUnitData->sCharacterData.iAttackSpeed ) );
	
	//Critical
	vCharacterInfoDataLeft.push_back( "Critical: " );
	vCharacterInfoDataRight.push_back( FormatString( "%d", pcViewUnitData->sCharacterData.iCritical ) );

	//Owner ID
	vCharacterInfoDataLeft.push_back("Owner ID: ");
	vCharacterInfoDataRight.push_back(FormatString("%d", pcViewUnitData->sCharacterData.iOwnerID));
	
	//Regen
	vCharacterInfoDataLeft.push_back( "Regen: " );
	vCharacterInfoDataRight.push_back( FormatString( "%.2f - %.2f - %.2f", pcViewUnitData->sCharacterData.fHPRegen, pcViewUnitData->sCharacterData.fMPRegen, pcViewUnitData->sCharacterData.fSPRegen ) );
	
	//Movement fMovementSpeed
	vCharacterInfoDataLeft.push_back( "Movement Speed: " );
	vCharacterInfoDataRight.push_back( FormatString( "%d", pcViewUnitData->sCharacterData.iMovementSpeed ) );
	
	//iWeight
	vCharacterInfoDataLeft.push_back( "Weight: " );
	vCharacterInfoDataRight.push_back( FormatString( "%d", pcViewUnitData->sCharacterData.sWeight ) );
	
	//Attributes
	vCharacterInfoDataLeft.push_back( "Attributes: " );
	vCharacterInfoDataRight.push_back( FormatString( "%d - %d - %d - %d - %d (%d)",
		pcViewUnitData->sCharacterData.iStrength, pcViewUnitData->sCharacterData.iSpirit, pcViewUnitData->sCharacterData.iTalent, pcViewUnitData->sCharacterData.iAgility, pcViewUnitData->sCharacterData.iHealth, pcViewUnitData->sCharacterData.iStatPoints ) );
	
	//Gold
	vCharacterInfoDataLeft.push_back( "Gold: " );
	vCharacterInfoDataRight.push_back( FormatString( "%s gp", FormatNumber( pcViewUnitData->sCharacterData.iGold ) ) );
	
	//EXP
	vCharacterInfoDataLeft.push_back( "EXP: " );
	LARGE_INTEGER li;
	li.LowPart = pcViewUnitData->sCharacterData.iCurrentExpLow;
	li.HighPart = pcViewUnitData->sCharacterData.iCurrentExpHigh;
	vCharacterInfoDataRight.push_back( FormatString( "%s", FormatNumber( li.QuadPart ) ) );
	
	//Unique ID
	vCharacterInfoDataLeft.push_back( "Unique ID: " );
	vCharacterInfoDataRight.push_back( FormatString( "%d", pcViewUnitData->sCharacterData.iID ) );
	
	//Clan ID
	vCharacterInfoDataLeft.push_back( "Clan ID: " );
	vCharacterInfoDataRight.push_back( FormatString( "%d", pcViewUnitData->sCharacterData.iClanID ) );
	
	//Type
	std::string strType;

	if ( pcViewUnitData->sCharacterData.iType == CHARACTERTYPE_Monster )
		strType = "[Monster]";
	else if ( pcViewUnitData->sCharacterData.iType == CHARACTERTYPE_NPC )
		strType = "[NPC]";
	else
		strType = "[Player]";

	vCharacterInfoDataLeft.push_back( "Type: " );
	vCharacterInfoDataRight.push_back( strType );
	
	//Animation ID
	vCharacterInfoDataLeft.push_back( "Animation ID: " );
	vCharacterInfoDataRight.push_back( FormatString( "0x%02X", pcViewUnitData->iAnimationID ) );
	
	//Animation Type
	vCharacterInfoDataLeft.push_back( "Animation Type: " );
	vCharacterInfoDataRight.push_back( FormatString( "0x%02X", pcViewUnitData->GetAnimation() ) );
	
	//FPS
	vCharacterInfoDataLeft.push_back( "FPS: " );
	vCharacterInfoDataRight.push_back( FormatString( "%d", pcViewUnitData->iFPS ) );
	
	//Animation Frame
	vCharacterInfoDataLeft.push_back( "Animation Frame: " );
	vCharacterInfoDataRight.push_back( FormatString( "%d", pcViewUnitData->iFrame ) );
	
	//Position
	vCharacterInfoDataLeft.push_back( "Position: " );
	vCharacterInfoDataRight.push_back( FormatString( "X [%d (%d)]\nY [%d(%d)]\nZ [%d(%d)]",
		pcViewUnitData->sPosition.iX, pcViewUnitData->sAngle.iX & PTANGLE_Mask, pcViewUnitData->sPosition.iY, pcViewUnitData->sAngle.iY & PTANGLE_Mask, 
													 pcViewUnitData->sPosition.iZ, pcViewUnitData->sAngle.iZ & PTANGLE_Mask ) );

	if ( (vCharacterInfoDataLeft.size() == vCharacterInfoDataRight.size()) && (vCharacterInfoDataLeft.size() > 0) )
	{
		for ( UINT i = 0; i < vCharacterInfoDataLeft.size(); i++ )
		{
			GetGraphicsDevice()->GetDefaultFont()->Draw( vCharacterInfoDataLeft[i], iXDebug - iLabelPosition, iPaddingBottom, 0, DX::Color( 255, 255, 255 ) );
			GetGraphicsDevice()->GetDefaultFont()->Draw( vCharacterInfoDataRight[i], iXDebug - iValuePosition, iPaddingBottom, 0, DX::Color( 255, 255, 255 ) );
			iPaddingBottom += 15;
		}
	}

	iLastObjectID = *(DWORD*)( 0x207C888 + 0x10 );
}


void UnitGame::SendUnitDataEx()
{
	//Prepare Packet
	PacketPlayDataEx sPacket;
	ZeroMemory( &sPacket, sizeof( PacketPlayDataEx ) );
	sPacket.iLength = sizeof( PacketPlayDataEx );
	sPacket.iHeader = PKTHDR_PlayDataEx;
	sPacket.bGameMaster = GM_MODE;
	sPacket.iMapID = MAP_ID;

	// Skills levels
	for( int i = 0; i < SKILL_COUNT_CHARACTER; i++ )
		sPacket.iLevelSkill[i] = SKILLSCHARACTER[i + 1].iLevel;

	// Mac Address
	{
		UUID uuid;

		// Ask OS to create UUID
		UuidCreateSequential( &uuid );

		// Bytes 2 through 7 inclusive 
		sprintf_s( sPacket.szMacAddr, 20, "%02X-%02X-%02X-%02X-%02X-%02X",
			uuid.Data4[2], uuid.Data4[3], uuid.Data4[4], uuid.Data4[5], uuid.Data4[6], uuid.Data4[7] );
	}

	// Player Data
	CopyMemory( &sPacket.sCharacterData, &UNITDATA->sCharacterData.iStrength, sizeof( CharacterDataPacket ) );
	sPacket.iLevelCharacter = UNITDATA->sCharacterData.iLevel;
	sPacket.iCritical = UNITDATA->sCharacterData.iCritical;

	// ServerID
	sPacket.dwServerID = CLoginScreen::GAMESERVER_SERVERID;

	// Quest Log
	sPacket.dwQuestLevelLog = ( *(DWORD*)0x0362BA68 );

	//Bellatra Crown
	sPacket.iBellatraCrown = ( *(int*)( 0x03B18BC8 ) );

	//HD Serial
	{
		char szHD[4];

		szHD[0] = 'C';
		szHD[1] = ':';
		szHD[2] = '\\';
		szHD[3] = 0;

		DWORD dw = 0;

		GetVolumeInformationA( szHD, NULL, 0, &dw, NULL, NULL, NULL, 0 );

		sPacket.uSerialHD = dw;
	}

	if( iSpeedHack <= 3 )
	{
		sPacket.dwSpeed = 0;
		iSpeedHack++;
	}
	else
		sPacket.dwSpeed = 0;

	// Gold
	sPacket.dwGold = UNITDATA->sCharacterData.iGold;

	sPacket.uChecksumModule = CAntiCheat::GetChecksumDLL();

	if( dwUpdatePacket < TICKCOUNT || CHARACTERGAME->GetStageID() == 33 )
	{
		dwUpdatePacket = TICKCOUNT + 5000;
		SENDPACKETL( &sPacket );
		SENDPACKETG( &sPacket );
	}

	//Normal...
	PacketPlayData * psPacketPlay = (PacketPlayData*)UNITDATA->baUnitInfo;
	psPacketPlay->sMP.sCur = CHARACTERGAME->GetCurrentMP();
	psPacketPlay->sMP.sMax = UNITDATA->sCharacterData.sMP.sMax;
	psPacketPlay->sSP.sCur = CHARACTERGAME->GetCurrentSP();
	psPacketPlay->sSP.sMax = UNITDATA->sCharacterData.sSP.sMax;
	psPacketPlay->bPartyLeader = UNIT->bPartyLeader;
	psPacketPlay->saSpeed[0] = UNITDATA->sCharacterData.iAttackSpeed;
	psPacketPlay->saSpeed[1] = UNITDATA->sCharacterData.iMovementSpeed;
	psPacketPlay->sFPS = pcUnitData->iFPS;
	psPacketPlay->sHPLong.iMin = CHARACTERGAME->GetCurrentHP();
	psPacketPlay->sHPLong.iMax = UNITDATA->sCharacterData.sHP.sMax;
	psPacketPlay->sAngleY = UNITDATA->sAngle.iY;
	psPacketPlay->sSize = UNITDATA->sCharacterData.sSize;

    psPacketPlay->sLevelAgeArmor = UNITDATA->sCharacterData.sLevelAgeArmor;

	psPacketPlay->bCaravan = false;

    psPacketPlay->iPvPKills         = UNIT->iPvPKills;
    psPacketPlay->iPvPExperience    = UNIT->iPvPExperience;

	psPacketPlay->bPvPMode = UNIT->bPvPMode;

	psPacketPlay->iMountType = (int)MOUNTHANDLER->GetMountType( UNITDATA );

	int iSecondMapIndex = (UNITDATA->iLoadedMapIndex == 0) ? 1 : 0;
	psPacketPlay->sSecondMapID = -1;
	if ( FIELDGAME[iSecondMapIndex] )
		psPacketPlay->sSecondMapID = FIELDGAME[iSecondMapIndex]->iMapID;


	//Update Unit Status
	{
		static DWORD dwTimeUpdateGM = 0;

		if ( (dwTimeUpdateGM + 3000) < TICKCOUNT )
		{
			if ( GM_MODE && DEBUG_GM )
			{
				if ( TARGET_UNITDATA->bActive )
				{
					if ( (TARGET_UNITDATA->sCharacterData.iType == CHARACTERTYPE_Player) || (TARGET_UNITDATA->iCharacterType == CHARACTERTYPE_Player) )
					{
						PacketCharacterDataSyncEx sPacketGM;
						sPacketGM.iLength = sizeof( PacketCharacterDataSyncEx );
						sPacketGM.iHeader = PKTHDR_CharacterDataSyncExGM;
						sPacketGM.iID = TARGET_UNITDATA->iID;
						SENDPACKETG( &sPacketGM, TRUE );
						dwTimeUpdateGM = TICKCOUNT;
					}
				}
			}
		}
	}
}

NAKED void UnitGame::UpdateObjectID( ID lID )
{
	JMP( pfnUpdateObjectID );
}

#define BLURITEM_MAX 32

EXEVertex sVertexBlur[(BLURITEM_MAX * 2) * 3];
EXEFace sFaceBlur[BLURITEM_MAX * 2];

BOOL UnitGame::RenderItemMotionBlur( UnitData * pcUnitData, UnitTool * psHand )
{
	if ( !pcUnitData->bVisible || !pcUnitData->eWeaponItemID )
		return FALSE;

	if( (pcUnitData->sLeftHandTool.eItemID & 0xFFFF0000) == ITEMTYPE_Shield )
		return FALSE;

	if ( pcUnitData != UNITDATA )
	{
		if ( READDWORD( 0x0A1755C ) <= 45 )
			return FALSE;
	}

	const int iTotalFrame	= 80;

	int iCos = 0, iSin = 0;

	//Position
	Point3D sTop( 0, 0, 0 );
	Point3D sBottom( 0, 0, psHand->iSizeMax );

	//Angle
	Point3D sAngle	= pcUnitData->sAngle;
	sAngle.iY		= (-sAngle.iY + PTANGLE_180) & PTANGLE_Mask;

	static BOOL bHandleFaces = FALSE;

	if ( bHandleFaces == FALSE )
	{
		USHORT iV = 0;
		int iB = 0;

		EXETextureLink * psa = ((EXETextureLink*)0x00A1BCB8);

		for ( int i = 0; i < BLURITEM_MAX; i++ )
		{
			sFaceBlur[iB].pcTextureLink = &psa[i % 128];
			sFaceBlur[iB].uVertex1	= iV;
			sFaceBlur[iB].uVertex2	= iV+1;
			sFaceBlur[iB].uVertex3	= iV+2;
			sFaceBlur[iB].uMatID	= (USHORT)READDWORD( 0x00A1CCBC );
			iB++;

			sFaceBlur[iB].pcTextureLink = &psa[i % 128];
			sFaceBlur[iB].uVertex1	= iV+2;
			sFaceBlur[iB].uVertex2	= iV+1;
			sFaceBlur[iB].uVertex3	= iV+3;
			sFaceBlur[iB].uMatID	= (USHORT)READDWORD( 0x00A1CCBC );
			iB++;

			iV += 2;
		}

		bHandleFaces = TRUE;
	}

	//Vertex and Face
	EXEVertex * psVertex	= sVertexBlur;
	EXEFace * psFace		= sFaceBlur;

	int iLevelBlurO		= BLURITEM_MAX << 1;
	int iSizeLevelBlur	= sBottom.iZ / (BLURITEM_MAX << 1);

	//Update position if weapon is Two Hand
	if ( pcUnitData->bIsWeaponTwoHanded )
	{
		int iAngle	= (-PTANGLE_45 / 5) & PTANGLE_Mask;

		iSin		= PTSEN[iAngle] >> 8;
		iCos		= PTCOS[iAngle] >> 8;

		sTop.iY		= -(sTop.iZ * iSin) >> 8;
		sTop.iZ		= (sTop.iZ * iCos) >> 8;

		sBottom.iY	= -(sBottom.iZ * iSin) >> 8;
		sBottom.iZ	= (sBottom.iZ * iCos) >> 8;
	}

	//Using Skill?
	if ( ACTIONMODE )
	{
		//Update Angle
		if ( sAngle.iY > PTANGLE_180 )
			sAngle.iY = PTANGLE_270;
		else
			sAngle.iY = PTANGLE_90;
	}

	if ( psHand->pModel && psHand->pBone )
	{
		EXEMatrixI & sMatrix = psHand->pBone->GetWorldMatrix();

		for ( int i = 0; i < BLURITEM_MAX; i++ )
		{
			int iFrame = pcUnitData->iFrame - (i * iTotalFrame >> 2);
			
			if ( iFrame < pcUnitData->psModelAnimation->iBeginFrame )
				iFrame = pcUnitData->psModelAnimation->iBeginFrame;

			AnimateMeshTreeNew( psHand->pBone, iFrame, sAngle.iX, sAngle.iY, sAngle.iZ );

			int iVertexCount = i << 1;

			Point3D sRadian;
			Point3D sPositionOut;

			//Get Top Position
			{
				sPositionOut = sTop;

				//Radian
				sRadian.iX = sPositionOut.iX * sMatrix.i[0][0] + sPositionOut.iY * sMatrix.i[1][0] + sPositionOut.iZ * sMatrix.i[2][0];
				sRadian.iY = sPositionOut.iX * sMatrix.i[0][1] + sPositionOut.iY * sMatrix.i[1][1] + sPositionOut.iZ * sMatrix.i[2][1];
				sRadian.iZ = sPositionOut.iX * sMatrix.i[0][2] + sPositionOut.iY * sMatrix.i[1][2] + sPositionOut.iZ * sMatrix.i[2][2];

				//Get Weapon Position
				psVertex[iVertexCount].sPosition.iX = (pcUnitData->sPosition.iX + (sRadian.iX >> 8) + sMatrix.i[3][0]);
				psVertex[iVertexCount].sPosition.iZ = (pcUnitData->sPosition.iZ + (sRadian.iY >> 8) + sMatrix.i[3][1]);
				psVertex[iVertexCount].sPosition.iY = (pcUnitData->sPosition.iY + (sRadian.iZ >> 8) + sMatrix.i[3][2]);
			}

			iVertexCount++;

			//Get Bottom Position
			{
				sPositionOut = sBottom;

				//Radian
				sRadian.iX = sPositionOut.iX * sMatrix.i[0][0] + sPositionOut.iY * sMatrix.i[1][0] + sPositionOut.iZ * sMatrix.i[2][0];
				sRadian.iY = sPositionOut.iX * sMatrix.i[0][1] + sPositionOut.iY * sMatrix.i[1][1] + sPositionOut.iZ * sMatrix.i[2][1];
				sRadian.iZ = sPositionOut.iX * sMatrix.i[0][2] + sPositionOut.iY * sMatrix.i[1][2] + sPositionOut.iZ * sMatrix.i[2][2];

				//Get Weapon Position
				psVertex[iVertexCount].sPosition.iX = (pcUnitData->sPosition.iX + (sRadian.iX >> 8) + sMatrix.i[3][0]);
				psVertex[iVertexCount].sPosition.iZ = (pcUnitData->sPosition.iZ + (sRadian.iY >> 8) + sMatrix.i[3][1]);
				psVertex[iVertexCount].sPosition.iY = (pcUnitData->sPosition.iY + (sRadian.iZ >> 8) + sMatrix.i[3][2]);
			}

			//Update Position
			sTop.iZ		+= iSizeLevelBlur;
			sBottom.iZ	-= iSizeLevelBlur;
		}
	}

	RENDERER->Init();

	RENDERER->SetCameraPosition( UNITTARGETPOSITION_X, UNITTARGETPOSITION_Y, UNITTARGETPOSITION_Z, UNITTARGETANGLE_X, UNITTARGETANGLE_Y, UNITTARGETANGLE_Z );

	//Add Vertex to the render
	for ( int i = 0; i < iLevelBlurO; i++ )
		RENDERER->AddObjectVertex( &psVertex[i] );

	//Add Faces to the render
	for ( int i = 0; i < (iLevelBlurO - 2); i += 2 )
	{
		EXEVertex * v0 = &psVertex[i];
		EXEVertex * v1 = &psVertex[i + 1];
		EXEVertex * v2 = &psVertex[i + 2];
		EXEVertex * v3 = &psVertex[i + 3];

		if ( v0->sPosition.iX != v2->sPosition.iX || v0->sPosition.iY != v2->sPosition.iY || v0->sPosition.iZ != v2->sPosition.iZ ||
			 v1->sPosition.iX != v3->sPosition.iX || v1->sPosition.iY != v3->sPosition.iY || v1->sPosition.iZ != v3->sPosition.iZ )
		{
			RENDERER->AddObjectFace( &psFace[i] );
			RENDERER->AddObjectFace( &psFace[i + 1] );
		}
	}

	int r, g, b;

	r = RENDERER->r;
	g = RENDERER->g;
	b = RENDERER->b;

	RENDERER->r -= 64;
	RENDERER->g -= 64;
	RENDERER->b += 256;

	BOOL bPass = FALSE;

	if ( pcUnitData->sActiveSkill.iValue != 0 )
	{
		switch ( pcUnitData->sCharacterData.iClass )
		{
			case CHARACTERCLASS_Fighter:
				switch ( pcUnitData->sActiveSkill.cSkillTinyID )
				{
					case SKILLPLAYID_T11:
						RENDERER->r += 256;
						RENDERER->g -= 64;
						RENDERER->b -= 64;
						bPass = TRUE;
						break;

					case SKILLPLAYID_T12:
						RENDERER->r += 256;
						RENDERER->g += 256;
						RENDERER->b -= 64;
						bPass = TRUE;
						break;

					case SKILLPLAYID_T22:
						RENDERER->r -= 64;
						RENDERER->g += 256;
						RENDERER->b += 128;
						bPass = TRUE;
						break;

					default:
						break;
				}
				break;

			case CHARACTERCLASS_Pikeman:
				switch ( pcUnitData->sActiveSkill.cSkillTinyID )
				{
					case SKILLPLAYID_T12:
						RENDERER->r += 256;
						RENDERER->g -= 64;
						RENDERER->b += 256;
						bPass = TRUE;
						break;

					case SKILLPLAYID_T52:
					case SKILLPLAYID_T33:
						RENDERER->r += 256;
						RENDERER->g -= 64;
						RENDERER->b -= 64;
						bPass = TRUE;
						break;

					default:
						break;
				}
				break;

			default:
				break;
		}
	}

	if ( !bPass && psHand->iBlinkSpeed )
	{
		RENDERER->r += psHand->saBlinkColor[2];
		RENDERER->g += psHand->saBlinkColor[1];
		RENDERER->b += psHand->saBlinkColor[0];
	}

	if ( RENDERER->iFaceCount )
	{
		RENDERER->SetRenderClip( 0x3F );
		RENDERER->ClipFaceRender();
		RENDERER->Vertex2DGeometry( 0 );
		RENDERER->Render();
	}

	RENDERER->r = r;
	RENDERER->g = g;
	RENDERER->b = b;

	return TRUE;
}

BOOL UnitGame::IsBlockedRangeTarget()
{
	UnitData * pcUnitData = (*(UnitData**)0x0CF48D0);

	if ( pcUnitData )
	{
	//	if ( pcUnitData->sCharacterData.iType == CHARACTERTYPE_Player )
	//		return TRUE;
	}

	return FALSE;
}

BOOL UnitGame::SetPVPArea( UnitData * pcUnitData )
{
	BOOL bRet = FALSE;

	Unit * pcTarget = UNITDATATOUNIT( pcUnitData );

	int iMapType = UNITDATA->iLoadedMapIndex >= 0 ? FIELDGAME[UNITDATA->iLoadedMapIndex]->iMapType : -1;
	int iMapID = UNITDATA->iLoadedMapIndex >= 0 ? FIELDGAME[UNITDATA->iLoadedMapIndex]->iMapID : -1;

	if ( UNITDATA->iLoadedMapIndex >= 0 && 
		(iMapType == MAPTYPE_BlessCastle && (int)READDWORD( 0x008250CC ) > UNITDATA->sPosition.iZ) ||
		(iMapType == MAPTYPE_BlessCastle && BLESSCASTLEHANDLER->InSiegeWar() && BLESSCASTLEHANDLER->GetSiegeWarMode() == SIEGEWARMODE_TheMassacre) ||
		(iMapID == MAP_ID_PVP && READDWORD( 0x03A975F4 ) > TICKCOUNT && pcUnitData->dwExclusiveNum == 0) )
	{
		if ( pcUnitData->iCharacterType == CHARACTERTYPE_None && pcUnitData->sCharacterData.iType == CHARACTERTYPE_Player &&
			(pcUnitData->sCharacterData.iClanID == 0 || pcUnitData->sCharacterData.iClanID != UNITDATA->sCharacterData.iClanID) )
		{
			if ( iMapID != MAP_ID_PVP || pcUnitData->dwExclusiveNum == 0 )
			{
				pcUnitData->iCharacterType = pcUnitData->sCharacterData.iType;
				pcUnitData->sCharacterData.iType = CHARACTERTYPE_Monster;
			}
		}

		if ( pcUnitData->IsPlayer() )
		{
			if ( BATTLEROYALEHANDLER->InBattle() )
			{
				if ( BATTLEROYALEHANDLER->IsEnemy( pcUnitData ) )
				{
					if ( pcUnitData->sCharacterData.iType != CHARACTERTYPE_Monster )
					{
						pcUnitData->iCharacterType = CHARACTERTYPE_Player;
						pcUnitData->sCharacterData.iType = CHARACTERTYPE_Monster;
					}
				}
				else
				{
					pcUnitData->sCharacterData.iType = pcUnitData->iCharacterType;
					pcUnitData->iCharacterType = CHARACTERTYPE_None;
				}
			}
		}
	}
	else if ( BATTLEROYALEHANDLER->InBattle() )
	{
		if ( pcUnitData->IsPlayer() )
		{
			if ( BATTLEROYALEHANDLER->IsEnemy( pcUnitData ) )
			{
				if ( pcUnitData->sCharacterData.iType != CHARACTERTYPE_Monster )
				{
					pcUnitData->iCharacterType = CHARACTERTYPE_Player;
					pcUnitData->sCharacterData.iType = CHARACTERTYPE_Monster;
				}
			}
			else
			{
				pcUnitData->sCharacterData.iType = pcUnitData->iCharacterType;
				pcUnitData->iCharacterType = CHARACTERTYPE_None;
			}
		}
	}
	else if ( UNIT->bPvPMode && pcTarget->bPvPMode && (pcUnitData->dwExclusiveNum == 0) )
	{
		if ( pcUnitData->IsPlayer() )
		{
			if ( CHARACTERPVPMODEHANDLER->IsPKMap( MAP_ID ) )
			{
				if ( pcUnitData->sCharacterData.iType != CHARACTERTYPE_Monster )
				{
					pcUnitData->iCharacterType = CHARACTERTYPE_Player;
					pcUnitData->sCharacterData.iType = CHARACTERTYPE_Monster;
				}
			}
			else
			{
				pcUnitData->sCharacterData.iType = pcUnitData->iCharacterType;
				pcUnitData->iCharacterType = CHARACTERTYPE_None;
			}
		}
	}
	else
	{
		if ( pcUnitData->iCharacterType )
		{
			pcUnitData->sCharacterData.iType = pcUnitData->iCharacterType;
			pcUnitData->iCharacterType = CHARACTERTYPE_None;
		}
	}

	return bRet;
}

BOOL UnitGame::Fun()
{
	int i = (rand() % 8192) * 2;

	DWORD dw = CALL_WITH_ARG1( 0x005C1B80, i );

	CALL_WITH_ARG6( 0x0045CFB0, 0x12000010, (*(int*)0x00CF4750), (dw >> 24) & 0xFF, 0, 0, (DWORD)UNITDATA );
	CALL_WITH_ARG6( 0x0045CFB0, 0x12000010, (*(int*)0x00CF4750) + 500, (dw >> 16) & 0xFF, 0, 0, (DWORD)UNITDATA );
	CALL_WITH_ARG6( 0x0045CFB0, 0x12000010, (*(int*)0x00CF4750) + 1000, (dw >> 8) & 0xFF, 0, 0, (DWORD)UNITDATA );
	CALL_WITH_ARG6( 0x0045CFB0, 0x12000010, (*(int*)0x00CF4750) + 1500, dw & 0xFF, 0, 0, (DWORD)UNITDATA );

	UNITDATA->eNextAnimationType = ANIMATIONTYPE_Yahoo;

	CALL_WITH_ARG4( 0x00623390, 0x160, i, 0, 0 );

	return TRUE;
}

void UnitGame::MoveUnit( UnitData * pcUnitData, int iSpeed )
{
	CALLT_WITH_ARG1( 0x0044FFB4, (DWORD)pcUnitData, iSpeed );
}

void UnitGame::HandlePacket( PacketSaveData * psPacket )
{
	static int iCurrentBlock = 0;

	if ( psPacket->iCount > (iCurrentBlock) )
	{
	//	DISCONNECT_TYPE = 0;
	//	DISCONNECT_TIME = TRUE;
	}
	else
		iCurrentBlock++;

}

void UnitGame::HandlePacket( PacketCharacterDataSyncEx * psPacket )
{
	if ( UnitData * pcUnitData = UNITDATABYID( psPacket->iID ) )
	{
		CopyMemory( &pcUnitData->sCharacterData.iStrength, &psPacket->sData, sizeof( CharacterDataPacket ) );

		if ( TARGET_UNITDATA->bActive )
		{
			if ( TARGET_UNITDATA->iID == psPacket->iID )
				CopyMemory( &TARGET_UNITDATA->sCharacterData.iStrength, &psPacket->sData, sizeof( CharacterDataPacket ) );
		}
	}
}

void UnitGame::HandlePacketUnitStatus( UnitData * pcUnitData, Packet * psPacket )
{
	PacketPlayData * psPacketPlayData = (PacketPlayData*)psPacket;

	Unit * pcUnit = UNITDATATOUNIT( pcUnitData );

	switch ( psPacket->iHeader )
	{
		case PKTHDR_PlayData:
			pcUnitData->sCharacterData.sMP		= psPacketPlayData->sMP;
			pcUnitData->sCharacterData.sSP		= psPacketPlayData->sSP;

			pcUnitData->iFPS = 80;

			if ( (pcUnitData->sCharacterData.iType == CHARACTERTYPE_Player) || (pcUnitData->iCharacterType == CHARACTERTYPE_Player) )
			{
				pcUnitData->sCharacterData.iAttackSpeed = psPacketPlayData->saSpeed[0];
				pcUnitData->sCharacterData.iMovementSpeed = psPacketPlayData->saSpeed[1];

				pcUnitData->iFPS = psPacketPlayData->sFPS;

				pcUnitData->sAngle.iY = psPacketPlayData->sAngleY % 4096;
				pcUnitData->sCharacterData.sSize = psPacketPlayData->sSize;
				pcUnitData->sCharacterData.iLevel = psPacketPlayData->iLevel;

                pcUnitData->sCharacterData.sLevelAgeArmor = psPacketPlayData->sLevelAgeArmor;
				pcUnit->bFollowUnitType = psPacketPlayData->bCaravan;
                pcUnit->iPvPKills       = psPacketPlayData->iPvPKills;
                pcUnit->iPvPExperience  = psPacketPlayData->iPvPExperience;
				pcUnit->bPvPMode		= psPacketPlayData->bPvPMode;

                if ( UNITDATA_SELECTED && (UNITDATA_SELECTED == pcUnitData) )
                    if ( HUDHANDLER )
                        HUDHANDLER->UpdateBattleInfo( pcUnit );
            }
			else
				pcUnitData->sCharacterData.sLevelAgeArmor = 0;
			
			pcUnit->sHPLong = psPacketPlayData->sHPLong;
			pcUnit->bPartyLeader = psPacketPlayData->bPartyLeader;

			if ( psPacketPlayData->iMountType != (int)EMountType::MOUNTTYPE_None )
				MOUNTHANDLER->AddMount( pcUnitData, (EMountType)psPacketPlayData->iMountType );
			else if ( pcUnit->iMountType != psPacketPlayData->iMountType )
				MOUNTHANDLER->RemoveMount( pcUnitData );

			pcUnit->iMountType = psPacketPlayData->iMountType;

			break;

		default:
			break;
	}

	if ( pcUnitData->psModelAnimation == nullptr )
		return;

	//Old...
	CALLT_WITH_ARG1( 0x00434AF0, (DWORD)pcUnitData, (DWORD)psPacket );
}

void UnitGame::HandlePacketUnitBuff( UnitData * pcUnitData, PacketBuffData * psPacket )
{
	pcUnitData->iStunTimeLeft = 0;
	switch ( psPacket->iHeader )
	{
		case PKTHDR_IceDistortionBuff:
			if ( psPacket->Ice.iDistortion )
			{
				pcUnitData->bDistortion = 120;
			}
			else
				pcUnitData->bDistortion = 0;

			if ( psPacket->Ice.iIceOverlay )
			{
				if ( pcUnitData->iIceOverlay <= 0 )
					pcUnitData->iIceOverlay = -200;
			}
			else
			{
				if ( pcUnitData->iIceOverlay > 0 )
					pcUnitData->iIceOverlay = (-pcUnitData->iIceOverlay);
			}

			if ( psPacket->Ice.iCurseLazy > 0 )
			{
				pcUnitData->iCurseLazy = 120;
			}
			else
			{
				pcUnitData->iCurseLazy = 0;
			}

			pcUnitData->iFreezeTimeLeft = psPacket->Ice.iStunCount;
			break;

		case PKTHDR_StunBuff:
			pcUnitData->iStunTimeLeft = 4 * 16;
			break;

		default:
			break;
	}
}

int UnitGame::GetFramesSendCount()
{
	int iFrames = 64;

	if ( SOCKETGAME )
	{
		int iPing = SOCKETGAME->GetAveragePingGame();
		 if ( iPing <= 192 )
			iFrames = 16;
		else
			iFrames = 32;
	}

	return 64;
}

void UnitGame::HandleElementalWeapon( Unit * pcUnit )
{
	return;

	if ( !SETTINGSHANDLER->GetModel()->GetSettings().bElementEffect )
		return;

	UnitData * pcUnitData = pcUnit->pcUnitData;

	BOOL bElementWeapon = FALSE;

	if ( (pcUnitData->sCharacterData.iType == CHARACTERTYPE_Player) || (pcUnitData->iCharacterType == CHARACTERTYPE_Player) )
	{
		if ( pcUnitData->iID )
		{
			//Check Effects
			if ( pcUnitData->sRightHandTool.eItemID )
			{
				if ( pcUnitData->sRightHandTool.saBlinkColor[0] || pcUnitData->sRightHandTool.saBlinkColor[1] || pcUnitData->sRightHandTool.saBlinkColor[2] )
				{
					int iAgeNumber = GameCore::GetAgingNumberColor( pcUnitData->sRightHandTool.saBlinkColor[2], pcUnitData->sRightHandTool.saBlinkColor[1], pcUnitData->sRightHandTool.saBlinkColor[0] );
					if ( iAgeNumber == 21 )
					{
						if ( pcUnit->uElementWeaponID != PARTICLEID_FIREELEMENTWEAPON )
						{
							if ( pcUnit->uElementWeaponID )
								PARTICLEENGINE->Kill( pcUnit->uElementWeaponID, pcUnit->GetID() );

							CParticleScript * pcParticleScript = PARTICLEFACTORY->LoadScript( "game\\scripts\\particles\\FireElementWeapon.lua", 0, TRUE );
							if ( pcParticleScript )
							{
								pcParticleScript->SetID( PARTICLEID_FIREELEMENTWEAPON );

								Engine::ObjectOwnerUnit * pcOwner = new Engine::ObjectOwnerUnit( pcUnit->pcUnitData );
								pcOwner->IsWeapon( TRUE );
								pcParticleScript->SetOwner( pcOwner );
								PARTICLEENGINE->AddScript( pcParticleScript );

								pcUnit->uElementWeaponID = PARTICLEID_FIREELEMENTWEAPON;
							}
						}

						bElementWeapon = TRUE;
					}
					else if ( iAgeNumber == 22 )
					{
						if ( pcUnit->uElementWeaponID != PARTICLEID_ICEELEMENTWEAPON )
						{
							if ( pcUnit->uElementWeaponID )
								PARTICLEENGINE->Kill( pcUnit->uElementWeaponID, pcUnit->GetID() );

							CParticleScript * pcParticleScript = PARTICLEFACTORY->LoadScript( "game\\scripts\\particles\\IceElementWeapon.lua", 0, TRUE );
							if ( pcParticleScript )
							{
								pcParticleScript->SetID( PARTICLEID_ICEELEMENTWEAPON );

								Engine::ObjectOwnerUnit * pcOwner = new Engine::ObjectOwnerUnit( pcUnit->pcUnitData );
								pcOwner->IsWeapon( TRUE );
								pcParticleScript->SetOwner( pcOwner );
								PARTICLEENGINE->AddScript( pcParticleScript );

								pcUnit->uElementWeaponID = PARTICLEID_ICEELEMENTWEAPON;
							}
						}

						bElementWeapon = TRUE;
					}
				}
			}

			if ( bElementWeapon == FALSE )
			{
				if ( pcUnit->uElementWeaponID )
				{
					PARTICLEENGINE->Kill( pcUnit->uElementWeaponID, pcUnit->GetID() );
					pcUnit->uElementWeaponID = 0;
				}
			}
		}
	}
}

void UnitGame::UpdateElementalWeaponSettings()
{
	if ( !SETTINGSHANDLER->GetModel()->GetSettings().bElementEffect )
	{
		PARTICLEENGINE->Kill( PARTICLEID_FIREELEMENTWEAPON );
		PARTICLEENGINE->Kill( PARTICLEID_ICEELEMENTWEAPON );

		for ( UINT i = 0; i < MAX_UNITS; i++ )
			pcaUnit[i].uElementWeaponID = 0;
	}
}

void UnitGame::HandlePacket( SocketData * pcSocketData, PacketUnitStatusContainer * psPacket )
{
	int iReceiveCount = 0;

	UINT uBufferWheel = 0;
	for ( int i = 0; i < psPacket->iAmount; i++ )
	{
		PacketUnitStatusMove * ps = (PacketUnitStatusMove *)(psPacket->baBuffer + uBufferWheel);

		if ( auto pcUnitData = UNITDATABYID( ps->lID ) )
		{
			HandlePacketUnitStatus( pcUnitData, ps );

			if ( pcUnitData->IsPlayer() == FALSE )
			{
				if ( pcUnitData->iMoveSequenceCounter < (pcUnitData->iMoveSequenceEnd - 64) )
					pcUnitData->iMoveSequenceCounter = (pcUnitData->iMoveSequenceEnd - 64);
			}
		}
		else
		{
			if ( (iReceiveCount < 8) || ((iReceiveCount < 16) && (ps->iHeader == PKTHDR_PlayData)) )
			{
				iReceiveCount++;

				//Has Free Slot?
				if ( GetFreeUnitDataSlot() )
				{
					//Theres no need to create an unit, just request the data...
					if ( HasActiveCharacterIDRequest( ps->lID ) == false )
					{
						PacketSimple sPacket;
						sPacket.iLength = sizeof( PacketSimple );
						sPacket.iHeader = PKTHDR_GetUnitInfoData;
						sPacket.iUnk2 = ps->lID;
						if ( pcSocketData == SOCKETL )
							SENDPACKETL( &sPacket );
						else
							SENDPACKETG( &sPacket );

						AddCharacterIDRequest( ps->lID );
					}
				}
			}
		}

		uBufferWheel += ps->iLength;
		if ( uBufferWheel >= _countof( psPacket->baBuffer ) )
			break;
	}

	if ( uBufferWheel < (_countof( psPacket->baBuffer ) - sizeof( PacketSimple )) )
	{
		PacketSimple * psPacketOther = (PacketSimple *)(psPacket->baBuffer + uBufferWheel);
		if ( (psPacketOther->iHeader == PKTHDR_GameStatus) && (psPacketOther->iUnk2 == UNITDATA->iID) )
		{
			switch ( psPacketOther->iUnk )
			{
				//Babel Time
				case 0x200:
					WRITEDWORD( 0x03933FF8, (TICKCOUNT + 5000) );
					WRITEDWORD( 0x009C6428, psPacketOther->iUnk3 );
					break;

				//Pk Mode
				case 0x220:
					WRITEDWORD( 0x03A975F4, (TICKCOUNT + 10000) );
					WRITEDWORD( 0x03A975F8, psPacketOther->iUnk3 );
					break;

				//Bless Castle Leader
				case 0x210:
					if ( uBufferWheel < (_countof( psPacket->baBuffer ) - sizeof( PacketSimpleEx )) )
					{
						PacketSimpleEx * psPacketOtherEx = (PacketSimpleEx *)(psPacket->baBuffer + uBufferWheel);
						BlessCastleStatusData * psBlessCastleDataBackup = ((BlessCastleStatusData *)0x039ECFA0);
						BlessCastleStatusData * psBlessCastleData		= ((BlessCastleStatusData *)0x0392E088);

						//Backup BC Data
						memcpy( psBlessCastleDataBackup, psBlessCastleData, sizeof( BlessCastleStatusData ) );

						psBlessCastleData->iCastleMode = psPacketOtherEx->iUnk3 >> 16;
						psBlessCastleData->iDefenseLevel = psPacketOtherEx->iUnk3 & 0xFFFF;
						psBlessCastleData->iClanID = psPacketOtherEx->iUnk4;
						psBlessCastleData->iTimeSec = psPacketOtherEx->iUnk6;
						psBlessCastleData->iSoldierCount = psPacketOtherEx->iUnk5;

						//Show BC Time
						CALLT_WITH_ARG1( 0x004DE330, 0x0362BEA8, (DWORD)(psBlessCastleData->saTimeSec[1] - psBlessCastleData->saTimeSec[0]) );

						WRITEDWORD( 0x04B06BB0, READDWORD( 0x04B06BB0 ) + (psPacketOtherEx->iUnk7 & 0xFFFF) );

						if ( psBlessCastleData->iCastleMode == 3 )
						{
							if ( psBlessCastleDataBackup->iCastleMode == 1 )
							{
								RESTARTCOUNTER = 400;

								//Stop BGM
								CALL( 0x00542B30 );

								//Play Mini Sound
								CALL_WITH_ARG1( 0x00543D30, 14 );

								//Show Exit Message
								CALLT( 0x004DFF30, 0x0362BEA8 );

								//Winner? Yahoo!
								if ( psBlessCastleData->iClanID && (psBlessCastleData->iClanID == BLESSCASTLEHANDLER->GetClanID( UNITDATA->sCharacterData.iClanID )) )
									UNITDATA->Animate( ANIMATIONTYPE_Yahoo );
							}
						}
						else if ( psBlessCastleDataBackup->iCastleMode && (psBlessCastleData->iCastleMode == 0) )
						{
							//Loser or doesn't have a Clan? Teleport back
							if ( (UNITDATA->sCharacterData.iClanID == 0) || (psBlessCastleData->iClanID != BLESSCASTLEHANDLER->GetClanID( UNITDATA->sCharacterData.iClanID )) )
								TELEPORTPLAYER( MAPID_BlessCastle );
						}

						//Update Resistance (maybe crown?)
						if ( (READDWORD( 0x03A976EC ) + 60000) < TICKCOUNT )
							CALL( 0x00627420 );

						//Update Clan
						if ( psBlessCastleData->iClanID != psBlessCastleDataBackup->iClanID )
							CALL_WITH_ARG1( 0x00433690, psBlessCastleData->iClanID );
					}
					break;

				default:
					break;
			}
		}
	}
}

UnitData * UnitGame::GetFreeUnitDataSlot()
{
	for ( int i = 0; i < MAX_UNITS; i++ )
		if ( UNITGAME->pcaUnitData[i].bActive == FALSE )
			return UNITGAME->pcaUnitData + i;

	return nullptr;
}

bool UnitGame::HasActiveCharacterIDRequest( int iID )
{
	bool bRet = false;
	for ( auto & sRequest : saCharacterRequestID )
	{
		if ( sRequest.iID == iID )
		{
			if ( (sRequest.dwTime + MAX_CHARACTERID_REQUEST_TIME) < TICKCOUNT )
			{
				sRequest.iID = -1;
				sRequest.dwTime = 0;
				break;
			}
			else
			{
				bRet = true;
				break;
			}
		}
	}

	return bRet;
}

void UnitGame::AddCharacterIDRequest( int iID )
{
	auto psRequest = saCharacterRequestID + (iCharacterRequestIDWheel++ % MAX_CHARACTERID_REQUEST);

	psRequest->iID		= iID;
	psRequest->dwTime	= TICKCOUNT;
}

void UnitGame::RemoveCharacterIDRequest( int iID )
{
	for ( auto & sRequest : saCharacterRequestID )
	{
		if ( sRequest.iID == iID )
		{
			sRequest.iID = -1;
			sRequest.dwTime = 0;
			break;
		}
	}
}
