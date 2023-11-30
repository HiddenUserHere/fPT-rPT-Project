#include "stdafx.h"
#include "CharacterGame.h"

typedef int( __thiscall *tfnChangeMotion )(void * ptr, int iMotion, int iAnimation);
tfnChangeMotion fnChangeMotion = (tfnChangeMotion)0x0041F930;

typedef void( __thiscall *tfnMessageBoxTitle ) (DWORD dwClass, int iID, char * pszTitle);
tfnMessageBoxTitle MessageBoxTitle = (tfnMessageBoxTitle)0x004D1680;

NAKED INT64 CharacterGame::ExpXor()
{
	JMP( 0x00459820 );
}

CharacterGame::CharacterGame()
{
	ZeroMemory( &sCharacterDataEx, sizeof( CharacterDataEx ) );
}

CharacterGame::~CharacterGame()
{
}

void CharacterGame::OnCharacterUpdateData()
{
	CGameProtect::SetDecreaseMP( 0.0f );
	CGameProtect::SetDecreaseSP( 0.0f );

	//T5
	SetFSP( 0 );
	if ( CQUESTGAME->GetQuestInstance()->IsDoneQuest( QUESTID_T5QuestArena ) )
	{
		int iPoints = 0;

		if ( UNITDATA->sCharacterData.iLevel >= 100 )
			iPoints++;
		if ( CQUESTGAME->GetQuestInstance()->IsDoneQuest( QUESTID_KingHopyQuest ) )
			iPoints++;
		if ( CQUESTGAME->GetQuestInstance()->IsDoneQuest( QUESTID_AMatterOfStrenght ) )
			iPoints++;
		if ( CQUESTGAME->GetQuestInstance()->IsDoneQuest( QUESTID_CryMeARiver ) )
			iPoints++;
		if ( CQUESTGAME->GetQuestInstance()->IsDoneQuest( QUESTID_TheLastStage ) )
			iPoints++;
		if ( CQUESTGAME->GetQuestInstance()->IsDoneQuest( QUESTID_AnAncientThreat ) )
			iPoints++;
		if ( CQUESTGAME->GetQuestInstance()->IsDoneQuest( QUESTID_AnEndlessBattle ) )
			iPoints++;
		if ( CQUESTGAME->GetQuestInstance()->IsDoneQuest( QUESTID_AnEndlessSuffering ) )
			iPoints++;
		if ( CQUESTGAME->GetQuestInstance()->IsDoneQuest( QUESTID_ATestOfBravery ) )
			iPoints++;
		if ( CQUESTGAME->GetQuestInstance()->IsDoneQuest( QUESTID_GreenLiquidQuestStep2 ) )
			iPoints += 2;
		if ( CQUESTGAME->GetQuestInstance()->IsDoneQuest( QUESTID_MercyfortheLost2 ) )
			iPoints += 2;
		if ( CQUESTGAME->GetQuestInstance()->IsDoneQuest( QUESTID_HuntingTheIronGuardian ) )
			iPoints += 2;

		iPoints += (UNITDATA->sCharacterData.iLevel - 100) / 5;

		for ( int i = 16; i < 20; i++ )
			iPoints -= SKILLSCHARACTER[i + 1].iLevel;

		SetFSP( iPoints );
	}

	//Quest
	{
		QuestData * pc = CQUESTGAME->GetQuestInstance()->GetQuest( QUESTID_TheLoyaltyTestStep1 );
		if ( pc )
		{
			SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_Damage, CHARSTATUSCOLOR_Red );

			(*(int*)0x035EB600) -= (UNITDATA->sCharacterData.iMinDamage * 20) / 100;
			(*(int*)0x035EB604) -= (UNITDATA->sCharacterData.iMaxDamage * 20) / 100;
		}

		pc = CQUESTGAME->GetQuestInstance()->GetQuest( QUESTID_TheLoyaltyTestStep2 );
		if ( pc )
		{
			SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_Absorb, CHARSTATUSCOLOR_Red );
			SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_Defense, CHARSTATUSCOLOR_Red );

			UNITDATA->sCharacterData.iAbsorbRating -= (UNITDATA->sCharacterData.iAbsorbRating * 20) / 100;
			UNITDATA->sCharacterData.iDefenseRating -= (UNITDATA->sCharacterData.iDefenseRating * 20) / 100;
		}
	}

	//Weapon Using
	{
		if ( INVENTORYITEMSLOT[0].iItemIndex )
		{
			ItemData * pcItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[0].iItemIndex - 1];

			WRITEDWORD( 0x04B0DA14, 0 );
			if ( pcItemData->bValid )
			{
				WRITEDWORD( 0x04B0DA14, pcItemData->sItem.sItemID.ToInt() );
			}
		}
	}

	if ( (*(DWORD*)0x04B0719C) && !SKILLMANAGERHANDLER->IsActiveBuff( 0x05020004 ) )
	{
		__asm
		{
			PUSHAD;
			MOV EAX, DWORD PTR DS : [0x04B0719C];
			PUSH EAX;
			MOV ECX, 0x039032CC;
			MOV EAX, 0x00598B80;
			CALL EAX;

			MOV EAX, DWORD PTR DS : [0x04B0719C];
			PUSH EAX;
			MOV ECX, 0x039032CC;
			MOV EAX, 0x00598B40;
			CALL EAX;
			POPAD;
		};

		(*(DWORD*)0x04B0719C) = 0;
	}

	ITEMSETATTRIBUTEHANDLER->UpdateCharacterData();

	// Premium Data
	if ( Game::GetGameMode() == GAMEMODE_InGame && UNITDATA->bActive )
	{
		UNITDATA->sCharacterData.sLevelAgeArmor = 0;
		
		int iSlot = READDWORD( 0x03625718 ) ? ITEMSLOT_Costume : ITEMSLOT_Armor;

		if ( INVENTORYITEMSLOT[iSlot - 1].iItemIndex >= 0 )
		{
			ItemData * pcItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[iSlot - 1].iItemIndex - 1];

			if ( pcItemData )
				UNITDATA->sCharacterData.sLevelAgeArmor = (BYTE)pcItemData->sItem.sAgeLevel;
		}

		UNITGAME->HandleElementalWeapon( UNITDATATOUNIT( UNITDATA ) );

		if ( UNITDATA->bBlessCastleCrown == FALSE )
		{
		//	TIMERSKILLHANDLER->KillTimer( SKILLID_AbsorptionOfGlory );
		//	TIMERSKILLHANDLER->KillTimer( SKILLID_PowerSiege );
		//	TIMERSKILLHANDLER->KillTimer( SKILLID_EvasionAdaption );
		}

		if ( bHPUp )
			UNITDATA->sCharacterData.fHPRegen += 10.0f;

		if ( bMPUp )
			UNITDATA->sCharacterData.fMPRegen += 10.0f;

		WRITEDWORD( pdwMPDownTime, 0 );
		WRITEDWORD( pdwMPDownPercent, 0 );

		WRITEDWORD( pdwSPDownTime, 0 );
		WRITEDWORD( pdwSPDownPercent, 0 );

		if ( bMPDown )
		{
			WRITEDWORD( pdwMPDownTime, 1 );
			WRITEDWORD( pdwMPDownPercent, 25 );
		}

		if ( bSPDown )
		{
			WRITEDWORD( pdwSPDownTime, 1 );
			WRITEDWORD( pdwSPDownPercent, 25 );
		}

		if ( bTopPVP )
		{
			(*(int*)0x035EB600) += (UNITDATA->sCharacterData.iMinDamage * 8) / 100;
			(*(int*)0x035EB604) += (UNITDATA->sCharacterData.iMaxDamage * 8) / 100;
		}

		if ( bWeightStone )
			UNITDATA->sCharacterData.sWeight.sMax += 1000;

		//Hats
		ItemTimer * pcITM = ITEMTIMERHANDLER->GetHead();
		if ( pcITM )
		{
			// Cartola?
			switch ( pcITM->iType )
			{
				case ITEMTIMERTYPE_CartolaHat:
				case ITEMTIMERTYPE_WitchHat:
				case ITEMTIMERTYPE_HalloweenHat:
					UNITDATA->sCharacterData.fHPRegen += 10.0f;
					UNITDATA->sCharacterData.sHP.sMax += 10;
					break;

				case ITEMTIMERTYPE_XMasGreenHat:
					UNITDATA->sCharacterData.fHPRegen += 10.0f;
					UNITDATA->sCharacterData.sHP.sMax += 10;
					break;

				case ITEMTIMERTYPE_XMasRedHat:
					UNITDATA->sCharacterData.sMP.sMax += 50;
					UNITDATA->sCharacterData.sSP.sMax += 50;
					break;

				case ITEMTIMERTYPE_EasterHat:
					UNITDATA->sCharacterData.iMovementSpeed += 2;
					break;

				case ITEMTIMERTYPE_SoccerHat:
					UNITDATA->sCharacterData.iMovementSpeed += 2;
					break;

				case ITEMTIMERTYPE_SheepHat:
					UNITDATA->sCharacterData.iDefenseRating += 150;
					break;

				case ITEMTIMERTYPE_GiraffeHat:
					UNITDATA->sCharacterData.iAttackRating += 75;
					break;

				case ITEMTIMERTYPE_BigHeadHappiness:
					UNITDATA->sCharacterData.iAttackRating += 50;
					break;

				case ITEMTIMERTYPE_BigHeadLove:
					UNITDATA->sCharacterData.sHP.sMax += 10;
					UNITDATA->sCharacterData.fHPRegen += 10.0f;
					break;

				case ITEMTIMERTYPE_BigHeadSadness:
					UNITDATA->sCharacterData.sMP.sMax += 10;
					UNITDATA->sCharacterData.fMPRegen += 10.0f;
					break;

				case ITEMTIMERTYPE_BigHeadShyness:
					UNITDATA->sCharacterData.iDefenseRating += 100;
					break;

				case ITEMTIMERTYPE_BigHeadAngry:
					UNITDATA->sCharacterData.sHP.sMax += 50;
					UNITDATA->sCharacterData.sSP.sMax += 50;
					UNITDATA->sCharacterData.sMP.sMax += 50;
					break;

				case ITEMTIMERTYPE_BigHeadSurprised:
					UNITDATA->sCharacterData.iBlockRating += 2;
					break;

				case ITEMTIMERTYPE_BigHeadSensual:
					UNITDATA->sCharacterData.iCritical += 2;
					break;
				case ITEMTIMERTYPE_PatronumHat:
				case ITEMTIMERTYPE_GoldenHopyHat:
					UNITDATA->sCharacterData.iCritical += 3;
					break;

				case ITEMTIMERTYPE_ObscureHat:
					(*(int *)0x035EB600) += (UNITDATA->sCharacterData.iMinDamage * 5) / 100;
					(*(int *)0x035EB604) += (UNITDATA->sCharacterData.iMaxDamage * 5) / 100;
					break;
			}
		}

		if ( MOUNTHANDLER->IsRiding( UNITDATA ) )
			UNITDATA->sCharacterData.iMovementSpeed += 6;
		if ( CHARACTERGAME->bHPBooster )
			PLAYERDATA->sHP.sMax += (PLAYERDATA->sHP.sMax * 15) / 100;
		if ( CHARACTERGAME->bMPBooster )
			PLAYERDATA->sMP.sMax += (PLAYERDATA->sMP.sMax * 15) / 100;
		if ( CHARACTERGAME->bSTMBooster )
			PLAYERDATA->sSP.sMax += (PLAYERDATA->sSP.sMax * 15) / 100;
	}

	// Skill Timers
	{
	/*
		static int iaVLBackup[10];

		int * piVLReduction = (int*)(0x04B0DCB4);
		int * piVL = (int*)(0x00996E28);

		if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_SummonMuspell ) )
		{
			int iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_SummonMuspell );

			if ( iLevelSkill > 0 )
			{
				if ( iaVLBackup[9] == 0 )
					CopyMemory( iaVLBackup, piVL, sizeof( int ) * 10 );

				if ( iaVLBackup[9] == piVL[9] )
				{
					for ( int i = 0; i < 10; i++ )
						piVL[i] = piVL[i] - ((piVL[i] * abs( piVLReduction[i] )) / 100);
				}
			}
		}
		else
		{
			if ( iaVLBackup[9] != 0 && iaVLBackup[9] != piVL[9] )
				CopyMemory( piVL, iaVLBackup, sizeof( int ) * 10 );
		}
	*/
		// ABS BC
		if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_AbsorptionOfGlory ) )
		{
			if ( MAP_ID != MAPID_Bellatra )
				UNITDATA->sCharacterData.iAbsorbRating += (UNITDATA->sCharacterData.iAbsorbRating * 15) / 100;
		}

		//ABS Sod
		if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_AvertScroll ) )
		{
			if ( MAP_ID == MAPID_Bellatra )
				UNITDATA->sCharacterData.iAbsorbRating += (UNITDATA->sCharacterData.iAbsorbRating * 10) / 100;
		}
		// Bellatra Silver
		if ( SKILLMANAGERHANDLER->IsActiveBuff( TIMERSKILLID_BellatraSilver ) )
		{
			if ( (MAP_ID != MAPID_Bellatra) && (MAP_ID != MAPID_BlessCastle) )
				UNITDATA->sCharacterData.iDefenseRating += (UNITDATA->sCharacterData.iDefenseRating * 10) / 100;
		}
		// Bellatra Bronze
		else if ( SKILLMANAGERHANDLER->IsActiveBuff( TIMERSKILLID_BellatraBronze ) )
		{
			if ( (MAP_ID != MAPID_Bellatra) && (MAP_ID != MAPID_BlessCastle) )
				UNITDATA->sCharacterData.iAttackRating += (UNITDATA->sCharacterData.iAttackRating * 5) / 100;
		}
	}

	// Skills
	// Attack Mastery AtkRtg (Assassin)
	int iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( 0x10020004 );
	if ( iLevelSkill )
	{
		if ( (UNITDATA->sRightHandTool.eItemID & 0xFFFF0000) == 0x010A0000 )
		{
			ItemData * psItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[0].iItemIndex - 1];
			if ( psItemData )
			{
				int * iSkillTable = (int*)0x00998224;
				int iPercentSkill = iSkillTable[iLevelSkill];
				UNITDATA->sCharacterData.iAttackRating += (psItemData->sItem.iAttackRating * iPercentSkill) / 100;
			}
		}
	}

	iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_LethalSight );
	if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_LethalSight ) )
	{
		if ( iLevelSkill )
		{
			int iPercentSkill = iaArcherT51CritRate[iLevelSkill - 1];
			UNITDATA->sCharacterData.iCritical += (UNITDATA->sCharacterData.iCritical * iPercentSkill) / 100;
			iPercentSkill = iaArcherT51RangeAdd[iLevelSkill - 1];
			UNITDATA->sCharacterData.iAttackRange += (UNITDATA->sCharacterData.iAttackRange * iPercentSkill) / 100;
		}
	}

	iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_WizardTrance );
	if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_WizardTrance ) )
	{
		if ( INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex >= 0 )
		{
			ItemData * psItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex - 1];
			if ( psItemData && psItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Orb )
			{
				if ( iLevelSkill )
				{
					int iPercentSkill = iaMageT51BlockBoost[iLevelSkill - 1];
					UNITDATA->sCharacterData.iBlockRating += (UNITDATA->sCharacterData.iBlockRating * iPercentSkill) / 100;
				}
			}
		}
	}

	iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_Automation );
	if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_Automation ) )
	{
		if ( iLevelSkill )
		{
			ItemData * psItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex - 1];
			if ( psItemData && (psItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Bow || psItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Javelin) )
			{
				if ( (GetCurrentMP() > 0) && (GetCurrentSP() > 0) )
					UNITDATA->sCharacterData.iAttackSpeed += iaMechT23AtkSpeed[iLevelSkill - 1];
			}

			if ( iDecreaseAutomation > 0 )
			{
				CGameProtect::SetDecreaseMP( (CGameProtect::GetDecreaseMP() + faMechT23MPDrain[iLevelSkill - 1]) * 4.0f );
				CGameProtect::SetDecreaseSP( (CGameProtect::GetDecreaseSP() + faMechT23SPDrain[iLevelSkill - 1]) * 4.0f );
				iDecreaseAutomation--;
			}
		}
	}

	iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_Windy );
	if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_Windy ) )
	{
		if ( iLevelSkill )
		{
			int * iSkillTable = (int*)0x0099496C;
			int iPercentSkill = iSkillTable[iLevelSkill];
			UNITDATA->sCharacterData.iAttackRating += (UNITDATA->sCharacterData.iAttackRating * iPercentSkill) / 100;
			SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_AttackRating, CHARSTATUSCOLOR_Orange );
		}
	}

	iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_SummonMuspell );
	if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_SummonMuspell ) )
	{
		if ( iLevelSkill )
		{
			int * iSkillTable1 = (int*)0x04B0DC64;
			int * iSkillTable2 = (int*)0x04B0DC8C;

			int iPercentSkill = iSkillTable1[iLevelSkill - 1];
			(*(int*)0x035EB600) += (UNITDATA->sCharacterData.iMinDamage * iPercentSkill) / 100;

			iPercentSkill = iSkillTable2[iLevelSkill - 1];
			(*(int*)0x035EB604) += (UNITDATA->sCharacterData.iMaxDamage * iPercentSkill) / 100;
		}
	}

	iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_GodsBlessing );
	if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_GodsBlessing ) )
	{
		if ( iLevelSkill )
		{
			int iPercentSkill = iaKnightT43AddAtkPow[iLevelSkill - 1];
			(*(int*)0x035EB600) += (UNITDATA->sCharacterData.iMinDamage * iPercentSkill) / 100;

			iPercentSkill = iaKnightT43AddAtkPow[iLevelSkill - 1];
			(*(int*)0x035EB604) += (UNITDATA->sCharacterData.iMaxDamage * iPercentSkill) / 100;
		}
	}

	if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_Creed ) )
	{
		Skill * p = TIMERSKILLHANDLER->FindSkillTimer( SKILLID_Creed );
		if ( p && p->iLevel )
		{
			UNITDATA->sCharacterData.sMP.sMax += (UNITDATA->sCharacterData.sMP.sMax * iaShaT51MPBoost[p->iLevel - 1]) / 100;
			UNITDATA->sCharacterData.sSP.sMax += (UNITDATA->sCharacterData.sSP.sMax * iaShaT51STMBoost[p->iLevel - 1]) / 100;
			SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_MP, CHARSTATUSCOLOR_Blue );
			SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_SP, CHARSTATUSCOLOR_Green );
		}
	}

	if ( SKILLMANAGERHANDLER->IsActiveBuff( TIMERSKILLID_Compulsion ) )
	{
		SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_Absorb, CHARSTATUSCOLOR_Gold );
	}

	iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( 0x10080002 );
	if ( iLevelSkill )
	{
		if ( (UNITDATA->sRightHandTool.eItemID & 0xFFFF0000) == 0x010A0000 )
		{
			ItemData * psItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[0].iItemIndex - 1];
			if ( psItemData )
			{
				int * iSkillTable = (int*)0x00998454;
				int iPercentSkill = iSkillTable[iLevelSkill];
				UNITDATA->sCharacterData.iCritical += ((int)psItemData->sItem.iCritical * iPercentSkill) / 100;
			}
		}
	}

	iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_SurvivalInstinct );
	if ( iLevelSkill )
	{
		UNITDATA->sCharacterData.fHPRegen += (float)iaSurvivalInstinctBoostHPRegen[iLevelSkill - 1];

		int iLevelSkill2 = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_BoostHealth );
		if ( iLevelSkill2 )
		{
			int * piaSkill = ((int*)0x00995938);

			UNITDATA->sCharacterData.sHP.sMax += (piaSkill[iLevelSkill2 - 1] * iaFighterT51BoostHealthBoost[iLevelSkill - 1]) / 100;
		}
		SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_HP, CHARSTATUSCOLOR_Pink );
	}


	iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( TIMERSKILLID_ChasingHunt );
	if ( SKILLMANAGERHANDLER->IsActiveBuff( TIMERSKILLID_ChasingHunt ) )
	{
		if ( iLevelSkill )
		{
			if ( (UNITDATA->sRightHandTool.eItemID & 0xFFFF0000) == 0x01090000 )
			{
				ItemData * psItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[0].iItemIndex - 1];
				if ( psItemData )
				{
					int * iSkillTable1 = (int*)0x04B0DBA4;
					int * iSkillTable2 = (int*)0x04B0DBCC;

					int iPercentSkill = iSkillTable1[iLevelSkill - 1];
					(*(int*)0x035EB600) += (UNITDATA->sCharacterData.iMinDamage * iPercentSkill) / 100;

					iPercentSkill = iSkillTable2[iLevelSkill - 1];
					(*(int*)0x035EB604) += (UNITDATA->sCharacterData.iMaxDamage * iPercentSkill) / 100;
				}
			}
		}
	}

	static DWORD dwTimeUpdateKilled = 0;

	if ( UNITDATA && UNITDATA->psModelAnimation && UNITDATA->psModelAnimation->iType == ANIMATIONTYPE_Die )
	{
		if ( (dwTimeUpdateKilled + 7500) < TICKCOUNT )
		{
			dwTimeUpdateKilled = TICKCOUNT;

			TIMERSKILLHANDLER->OnCharacterKilled();
			CQUESTGAME->OnCharacterKilled();

			BATTLEROYALEHANDLER->OnDie();
		}
	}

	//Change Map?
	if ( GetLastStageID() != MAP_ID )
	{
		WRITEDWORD( 0x00A17650, 0 );
		WRITEDWORD( 0x00A17654, 0 );
		WRITEDWORD( 0x00A17658, 0 );
		WRITEDWORD( 0x00A1765C, 0 );

		//Update Handlers
		HUDHANDLER->GetMiniMapHandler()->OnChangeMap( MAP_ID );

		if ( IsStageVillage() )
		{
			SKILLMANAGERHANDLER->ResetSkillEffectParticle( SKILLEFFECTID_HolyValorAction );
			WIZARDTRANCEHANDLER->OnCastEnd( UNIT );
			MAGNETICDISCHARGEHANDLER->OnCastEnd( UNIT );
			WHIRLWINDHANDLER->OnCastEnd( UNIT );

			for ( int i = 0; i < MAX_UNITS; i++ )
			{
				auto pcUnitData = UNITGAME->pcaUnitData + i;
				if ( pcUnitData->bActive )
				{
					Unit * pcUnit = UNITDATATOUNIT( pcUnitData );

					WIZARDTRANCEHANDLER->OnCastEnd( pcUnit );
					MAGNETICDISCHARGEHANDLER->OnCastEnd( pcUnit );
					WHIRLWINDHANDLER->OnCastEnd( pcUnit );
				}
			}
		}

		if ( Game::GetGameMode() == GAMEMODE_InGame && UNITDATA->bActive )
		{
			if ( MAP_ID == MAPID_BlessCastle )
				if ( MOUNTHANDLER->IsRiding( UNITDATA ) )
					UNITDATA->Animate( ANIMATIONTYPE_Idle );

			CHARACTERPVPMODEHANDLER->OnUpdateMap( MAP_ID );
		}

		SetLastStageID( MAP_ID );
	}

	if ( Game::GetGameMode() == GAMEMODE_InGame && UNITDATA->bActive )
	{
		static bool bSwimming = false;
		if ( UNITDATA->iWaterHeight != (-32767) )
		{
			int iDifference = abs( UNITDATA->sPosition.iY - UNITDATA->iWaterHeight ) >> 8;

			if ( iDifference < 8 )
			{
				if ( bSwimming == false )
					UNITDATA->Animate( ANIMATIONTYPE_Idle );

				if ( UNITDATA->GetAnimation() == ANIMATIONTYPE_Walking )
				{
					UNITDATA->Animate( ANIMATIONTYPE_Idle );

					UNITDATA->bWalking = FALSE;
				}

				bSwimming = true;
			}
			else
			{
				if ( bSwimming )
					UNITDATA->Animate( ANIMATIONTYPE_Idle );

				bSwimming = false;
			}
		}
		else if ( bSwimming )
		{
			UNITDATA->Animate( ANIMATIONTYPE_Idle );
			bSwimming = false;
		}
	}

	CHECK_CHARACTER_CHECKSUM;
}

void CharacterGame::OnCharacterSetCharInfoEx()
{
	/*
	4B0C5EC CRIT
	4B0C4B0 BLOCK
	4B0C210 EVADE
	*/

	ITEMSETATTRIBUTEHANDLER->UpdateCharacterDataDefense();

	int * piCritical = (int*)0x04B0C5EC;
	int * piBlock = (int*)0x04B0C4B0;
	int * piEvade = (int*)0x04B0C210;

	extern int EVADE_GLOBAL_ITEM;
	if ( EVADE_GLOBAL_ITEM > 0 )
		*piEvade += EVADE_GLOBAL_ITEM;

	if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_EvasionAdaption ) )
	{
		if ( MAP_ID != MAPID_Bellatra )
			*piEvade += 10;
	}

	ItemTimer * pcITM = ITEMTIMERHANDLER->GetHead();
	if ( pcITM )
	{
		switch ( pcITM->iType )
		{
			case ITEMTIMERTYPE_GoldenHopyHat:
				piEvade += 2;
				break;
		}
	}

	if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_Vague ) )
	{
		int * piaSkill = ((int *)0x00997BE8);
		int iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_Vague );
		if ( iLevelSkill )
		{
			if ( INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex )
			{
				ItemData * pcItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex - 1];
				if ( pcItemData->bValid && pcItemData->IsTwoHandedWeapon() )
					*piEvade += piaSkill[iLevelSkill - 1];
			}
		}
	}

	// Attack Mastery Block (Assassin)
	int iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( 0x10020004 );
	if ( iLevelSkill )
	{
		if ( (UNITDATA->sRightHandTool.eItemID & 0xFFFF0000) == 0x010A0000 )
		{
			ItemData * psItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[0].iItemIndex - 1];
			if ( psItemData )
			{
				int * iSkillTable = (int*)0x009981FC;
				int iBlockPercentSkill = iSkillTable[iLevelSkill];
				*piBlock += ((int)psItemData->sItem.fBlockRating * iBlockPercentSkill) / 100;
			}
		}
	}

	if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_GodlyShield ) )
	{
		int iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_DivineShield );
		if ( iLevelSkill )
		{
			if ( ItemID( UNITDATA->sLeftHandTool.eItemID ).ToItemType() == ITEMTYPE_Shield )
			{
				if ( INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex )
				{
					ItemData * pcItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex - 1];
					if ( pcItemData )
					{
						*piBlock += ((int)pcItemData->sItem.fBlockRating * ((int *)0x009935E8)[iLevelSkill - 1]) / 100;
					}
				}
			}
		}
	}

	if ( SKILLMANAGERHANDLER->IsActiveBuff( 0x10020002 ) )
	{
		Skill * psSkill = TIMERSKILLHANDLER->FindSkillTimer( 0x10020002 );
		if ( psSkill )
		{
			iLevelSkill = psSkill->iLevel;
			if ( iLevelSkill )
			{
				int * iSkillTable = (int*)0x0099815C;
				int iPercentSkill = iSkillTable[iLevelSkill];
				
				if ( READDWORD( 0x04B0DA54 ) == 1 )
					iPercentSkill >>= 1;

				*piEvade += iPercentSkill;
			}
		}
	}
	if ( SKILLMANAGERHANDLER->IsActiveBuff( 0x50040001 ) )
	{
		iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( 0x50040001 );
		if ( iLevelSkill )
		{
			int * iSkillTable = (int*)0x04B06F4C;
			int iPercentSkill = iSkillTable[iLevelSkill];
			*piEvade += iPercentSkill;
		}
	}

	if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_SummonMuspell ) )
	{
		iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_SummonMuspell );
		if ( iLevelSkill )
			*piEvade += ((int *)0x00997170)[iLevelSkill - 1];
	}

	if ( SKILLMANAGERHANDLER->IsActiveBuff( 0x09020002 ) )
	{
		*piCritical += 5;
	}

	*piCritical += (*(int*)0x0362A2D0);

	if ( *piCritical > 50 )
		*piCritical = 50;
}

BOOL CharacterGame::IsStageVillage()
{
	if ( Game::GetGameMode() != GAMEMODE_InGame )
		return FALSE;

	if ( UNITDATA->iLoadedMapIndex >= 0 )
	{
		DWORD dwMapType = *(DWORD*)((UNITDATA->iLoadedMapIndex * 4) + 0x00CF4748);

		if ( dwMapType == 0 )
			return FALSE;

		if ( *(DWORD*)(dwMapType + 0x0C4) == 0x100 )
			return TRUE;
	}
	return FALSE;
}

int CharacterGame::GetStageID()
{
    if ( Game::GetGameMode() == GAMEMODE_InGame )
    {
        if ( UNITDATA->iLoadedMapIndex >= 0 )
            return FIELDGAME[UNITDATA->iLoadedMapIndex]->iMapID;
    }

    return MAPID_Invalid;
}

int CharacterGame::GetMapType()
{
    if ( Game::GetGameMode() == GAMEMODE_InGame )
    {
        if ( UNITDATA->iLoadedMapIndex >= 0 )
            return FIELDGAME[UNITDATA->iLoadedMapIndex]->iMapType;
    }

    return MAPTYPE_None;
}

int CharacterGame::GetCurrentMP()
{
	int iMana = 0;
	__asm
	{
		mov eax, 0x00507760;
		call eax;
		movsx eax, ax;
		mov iMana, eax;
	}
	return iMana;
}

int CharacterGame::GetCurrentHP()
{
	int iHP = 0;
	__asm
	{
		MOV EAX, 0x00507740;
		CALL EAX;
		MOVSX EAX, AX;
		MOV iHP, EAX;
	}
	return iHP;
}

int CharacterGame::GetCurrentSP()
{
	int iSP = 0;
	__asm
	{
		MOV EAX, 0x00507770;
		CALL EAX;
		MOVSX EAX, AX;
		MOV iSP, EAX;
	}
	return iSP;
}

void CharacterGame::SetCurrentMP( int iMP )
{
	CALL_WITH_ARG1( 0x00507820, (DWORD)iMP );
	CALL_WITH_ARG1( 0x0045D910, (DWORD)1 );
}

void CharacterGame::SetCurrentHP( int iHP )
{
	CALL_WITH_ARG1( 0x00507780, (DWORD)iHP );
	CALL_WITH_ARG1( 0x0045D910, (DWORD)0 );
}

void CharacterGame::SetCurrentSP( int iSP )
{
	CALL_WITH_ARG1( 0x005078B0, (DWORD)iSP );
	CALL_WITH_ARG1( 0x0045D910, (DWORD)2 );
}

void CharacterGame::SetPhoenix( BOOL bActive, int iPhoenixID )
{
	(*(DWORD*)piActivePhoenix) = iPhoenixID;

	if ( bActive )
		CALLT( pfnSetPhoenix, pcPhoenix );
	else
		CALLT( pfnDeletePhoenix, pcPhoenix );

	(*(BOOL*)pbShowPhoenixPet) = bActive;
}

void CharacterGame::Die()
{
	if ( UNITDATA && UNITDATA->psModelAnimation && UNITDATA->psModelAnimation->iType != ANIMATIONTYPE_Die )
	{
		UNITDATA->Animate( ANIMATIONTYPE_Die );

		SetCurrentHP( 0 );
		CHECK_CHARACTER_CHECKSUM;

		CALL( 0x0046CC4A );

		PLAYUNITSOUND( UNITDATA );
	}
}

void CharacterGame::ResetHead( BOOL bDefault )
{
	char szNewHeadModel[128] = { 0 };

	char cTier[2] = { UNITDATA->sCharacterData.iRank ? 0x60 + (char)UNITDATA->sCharacterData.iRank : 0, 0 };
	char cChar[2] = { UNITDATA->sCharacterData.iRank == 2 ? '_' : '-', 0 };

	switch ( UNITDATA->sCharacterData.iClass )
	{
		case CHARACTERCLASS_Fighter:
			STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Tmh%sB01%s.inf", cChar, cTier );
			break;

		case CHARACTERCLASS_Mechanician:
			STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Tmh%sA01%s.inf", cChar, cTier );
			break;

		case CHARACTERCLASS_Archer:
			STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Tfh%sD01%s.inf", cChar, cTier );
			break;

		case CHARACTERCLASS_Pikeman:
			STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Tmh%sC01%s.inf", cChar, cTier );
			if ( UNITDATA->sCharacterData.iRank == 3 )
				STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\TmhC-01c.inf" );
			break;

		case CHARACTERCLASS_Atalanta:
			STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Mfh%sB01%s.inf", cChar, cTier );
			break;

		case CHARACTERCLASS_Knight:
			STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Mmh%sA01%s.inf", cChar, cTier );
			break;

		case CHARACTERCLASS_Magician:
			STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Mmh%sD01%s.inf", cChar, cTier );
			break;

		case CHARACTERCLASS_Priestess:
			STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Mfh%sC01%s.inf", cChar, cTier );
			break;

		case CHARACTERCLASS_Assassin:
			STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Tfh%sE01%s.inf", cChar, cTier );
			break;

		case CHARACTERCLASS_Shaman:
			STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Mmh%sE01%s.inf", cChar, cTier );
			break;

		default:
			break;
	}

	//Old Head Not Null
	if ( (sCharacterDataEx.szOldHead[0] != 0) && ((sCharacterDataEx.szOldHead[0] == 'c') || (sCharacterDataEx.szOldHead[0] == 'C')) && bDefault == FALSE )
	{
		UNITGAME->SetCharacterHeadModel( sCharacterDataEx.szOldHead );
		return;
	}

	//Set Head
	UNITGAME->SetCharacterHeadModel( szNewHeadModel );
}

BOOL CharacterGame::Save()
{
	if ( SOCKETL == NULL )
		return FALSE;

	if ( TIME_LASTSAVE )
		return FALSE;

	if ( UNITDATA->sCharacterData.iClass == CHARACTERCLASS_None )
		return FALSE;

	//GM Disabled?
	if ( READDWORD( 0x00CF4870 ) != 0 )
		return FALSE;

	if ( TRADEHANDLER->IsWindowTradeOpen() )
		return FALSE;

	if ( WAREHOUSEHANDLER->IsWarehouseWindowOpen() )
		return FALSE;

	if ( GameCore::QuitAndSave() )
		return FALSE;

	if ( CHEAT_WARNING )
		return FALSE;

	if ( CGameProtect::ValidateCharacterCheck() == FALSE )
		return FALSE;

	//Skills Check
	if ( CALL( 0x00627BC0 ) == FALSE )
		return FALSE;

	SAVEEX;

	//Mounts
	MOUNTHANDLER->SaveMounts();

	//Xor Character Info
	CALL( 0x00620C10 );

	UPDATE_CHARACTER_CHECKSUM;

	CALLT_WITH_ARG4( 0x00463AF0, 0x00CF48E0, (DWORD)&UNITDATA->sCharacterData, (DWORD)INVENTORYITEMS, 0x035FEEF4, (DWORD)ITEMHANDLER->GetMouseItem() );

	CALLT_WITH_ARG2( 0x00464400, 0x00CF48E0, (DWORD)SOCKETL, NULL );

	TIME_LASTSAVE = TICKCOUNT;

	if ( DEBUG_GM )
		ChatGame::AddChatBoxText( "GameSave" );

	CALL( 0x00620C10 );

	UPDATE_CHARACTER_CHECKSUM;

	//Check Skill
	CALL( 0x0040FC70 );

	return TRUE;
}

void CharacterGame::SaveEx()
{
	PacketCharacterDataEx s;
	ZeroMemory( &s, sizeof( PacketCharacterDataEx ) );
	s.iLength = sizeof( PacketCharacterDataEx );
	s.iHeader = PKTHDR_CharacterDataEx;
	CopyMemory( &s.sData, &CHARACTERGAME->sCharacterDataEx, sizeof( CharacterDataEx ) );

	if ( auto pcItemData = HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->GetWeaponItem() )
		CopyMemory( &s.cCostumeWeapon, pcItemData, sizeof( ItemData ) );

	if ( auto pcItemData = HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->GetShieldItem() )
		CopyMemory( &s.cCostumeShield, pcItemData, sizeof( ItemData ) );

	if ( auto pcItemData = HUDHANDLER->GetInventoryWindow()->GetEarring1() )
		CopyMemory( &s.cEarring1, pcItemData, sizeof( ItemData ) );

	if ( auto pcItemData = HUDHANDLER->GetInventoryWindow()->GetEarring2() )
		CopyMemory( &s.cEarring2, pcItemData, sizeof( ItemData ) );

	SENDPACKETL( &s );
}

NAKED BOOL CharacterGame::ChecksumUpdate( CharacterData * psCharacterData )
{
	JMP( pfnChecksumUpdate );
}

BOOL __cdecl CharacterGame::ChecksumCheck()
{
	ANTICHEATHANDLER->CheckThread();

	return UPDATE_CHARACTER_CHECKSUM;
}

void CharacterGame::UpdateWeight()
{
	typedef void( __thiscall *tfnCheckWeight )(DWORD dwClass);
	tfnCheckWeight fnCheckWeight = (tfnCheckWeight)0x004AD2E0;
	fnCheckWeight( 0x035EBB20 );
}

BOOL CharacterGame::CheckInventorySpace( ItemData * psItemData, BOOL bUseTemporaryItem )
{
	ItemData sItemDataTemp;
	CopyMemory( &sItemDataTemp, psItemData, sizeof( ItemData ) );
	typedef BOOL( __thiscall *tfnCheckInventorySpace )(DWORD dwClass, ItemData * psItemData);
	tfnCheckInventorySpace fnCheckInventorySpace = (tfnCheckInventorySpace)0x004B0C60;

	if ( bUseTemporaryItem )
		return fnCheckInventorySpace( 0x035EBB20, &sItemDataTemp );

	return fnCheckInventorySpace( 0x035EBB20, psItemData );
}

BOOL CharacterGame::CheckInventoryWeight( ItemData * psItemData )
{
	EItemBase eItemBase = psItemData->sBaseItemID.ToItemBase();

	if ( eItemBase == ITEMBASE_Potion )
	{
		if ( (UNITDATA->sCharacterData.sWeight.sCur + psItemData->sItem.iPotionCount) > UNITDATA->sCharacterData.sWeight.sMax )
			return FALSE;
	}

	if ( (UNITDATA->sCharacterData.sWeight.sCur + psItemData->sItem.iWeight) > UNITDATA->sCharacterData.sWeight.sMax )
		return FALSE;

	if ( psItemData->sBaseItemID.ToItemID() == ITEMID_VampireSuit )
	{
		if ( (UNITDATA->sCharacterData.sWeight.sCur + 10) > UNITDATA->sCharacterData.sWeight.sMax )
			return FALSE;
	}

	return TRUE;
}

int CharacterGame::OnMotionChange( UnitData * pcUnitGame, int iMotion, int iAnimationID, EAnimationType eAnimationType, bool bRidingMount )
{
	bool bInWater = false;
	if ( (bRidingMount == false) && (pcUnitGame == UNITDATA) )
	{
		if ( (eAnimationType == ANIMATIONTYPE_Idle) || (eAnimationType == ANIMATIONTYPE_Running) )
		{
			pcUnitGame->Move( 256 );
			pcUnitGame->Move( -256 );
			if ( (pcUnitGame->iWaterHeight != (-32767)) && (pcUnitGame->sPosition.iY == pcUnitGame->iWaterHeight) )
				bInWater = true;
		}
	}

	Unit * pcUnit = UNITDATATOUNIT( pcUnitGame );
	if ( pcUnitGame )
	{
		// Assassin
		if ( pcUnitGame->sCharacterData.iClass == CHARACTERCLASS_Assassin )
		{
			if ( !IsStageVillage() && (ItemID( pcUnitGame->sRightHandTool.eItemID ).ToItemType() == ITEMTYPE_Dagger) )
			{
				switch ( eAnimationType )
				{
					case ANIMATIONTYPE_Idle:
						iMotion = 47;
						break;
					case ANIMATIONTYPE_Walking:
						iMotion = 16;
						break;
					case ANIMATIONTYPE_Running:
						iMotion = 36;
						break;
					case ANIMATIONTYPE_Attack:
						iMotion = 69;
						break;
				}
			}
		}

		if ( bInWater )
		{
			iMotion = EXECharacter::GetCharacterWaterAnimationData( pcUnitGame->sCharacterData.iClass, eAnimationType ).iAnimationID;
		}

		if ( (eAnimationType == ANIMATIONTYPE_FallLow) || (eAnimationType == ANIMATIONTYPE_FallHigh) )
		{
			if ( pcUnit )
			{
				if ( KNOCKUPHANDLER->CanChangeUnitY( pcUnit ) == FALSE )
					return TRUE;
			}
		}

		if ( iMotion < 0 || iMotion >= 512 )
			return TRUE;

		return fnChangeMotion( pcUnitGame, iMotion, iAnimationID );
	}
	return TRUE;
}

void CharacterGame::PHDamage( PacketSkillDamageNormal * psPacket )
{
	UnitData * pcUnitData = UNITDATABYID( psPacket->iTargetReceiveDamageID );
	if ( pcUnitData && (pcUnitData->sCharacterData.iType == CHARACTERTYPE_Monster && pcUnitData->iCharacterType == CHARACTERTYPE_Monster) )
	{
		if ( pcUnitData->dwLastDamageTime > TICKCOUNT )
			psPacket->iTargetReceiveDamageID = 0;
		else
			pcUnitData->dwLastActiveTime = TICKCOUNT + 400;
	}
}

BOOL CharacterGame::PHTeleport( struct PacketTeleportEvent * psPacket )
{
	BOOL bRet = FALSE;
	switch ( psPacket->iTeleportEventID )
	{
		case 65:
		{
			EMapID iMapTeleportID = MAPID_LandOfChaos;
			if ( MAP_ID == MAPID_LandOfChaos )
				iMapTeleportID = MAPID_DeathIsland;

			if ( UNITDATA->sCharacterData.iLevel >= MAPLEVEL( iMapTeleportID ) )
				MAPGAME->SetMessageBoxMap( iMapTeleportID );
			else
				TITLEBOX( "You must be level %d to teleport", MAPLEVEL( iMapTeleportID ) );

			bRet = TRUE;
		} break;
		case 1000:
			MAPGAME->SetMessageBoxMap( MAPID_RicartenTown );
			bRet = TRUE;
			break;
		case 1001:
			MAPGAME->SetMessageBoxMap( MAPID_Atlantis );
			bRet = TRUE;
			break;
        case 1002:
            if ( UNITDATA->sCharacterData.iLevel >= MAPLEVEL( MAPID_BattleTown ) )
                MAPGAME->SetMessageBoxMap( MAPID_BattleTown );
            else
                TITLEBOX( "You must be level %d to teleport", MAPLEVEL( MAPID_BattleTown ) );
            bRet = TRUE;
            break;
        case 1003:
            MAPGAME->TeleportMapPosition( MAPID_MysteryForest1, -37667, -14684 );
            bRet = TRUE;
            break;
        default:
			break;
	}
	return bRet;
}

void CharacterGame::CancelAttack()
{
	CALL( 0x004529B0 );
}

void CharacterGame::OnUseManaPotion( int iMP )
{
	int iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_Meditation );

	if ( iLevelSkill > 0 )
	{
		iLevelSkill--;

		int iValue = ((int*)0x04B0D8B0)[iLevelSkill];

		iMP += (iMP * iValue) / 100;
	}

	iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_MentalMastery );

	if ( iLevelSkill > 0 )
	{
		iLevelSkill--;

		int iValue = iaMageT21MPRecoveryBoost[iLevelSkill];

		iMP += (iMP * iValue) / 100;
	}

	iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_InnerPeace );

	if ( iLevelSkill > 0 )
	{
		iLevelSkill--;

		int iValue = ((int*)0x04B0D950)[iLevelSkill];

		iMP += (iMP * iValue) / 100;
	}

	if ( CGameCharacterStatus::SPEC_MPPOTION_EFFICIENCE > 0 )
		iMP += (iMP * CGameCharacterStatus::SPEC_MPPOTION_EFFICIENCE) / 100;

	CALL_WITH_ARG1( 0x00507820, (DWORD)iMP );
}

void CharacterGame::OnUseHPPotion( int iHP )
{
	int iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_SurvivalInstinct );

	if ( iLevelSkill > 0 )
	{
		iLevelSkill--;

		int iValue = iaFighterT51AddHPPotionBoost[iLevelSkill];

		iHP += (iHP * iValue) / 100;
	}

	if ( CGameCharacterStatus::SPEC_HPPOTION_EFFICIENCE > 0 )
		iHP += (iHP * CGameCharacterStatus::SPEC_HPPOTION_EFFICIENCE) / 100;

	CALL_WITH_ARG1( 0x00507780, (DWORD)iHP );
}

void CharacterGame::OnUseSPPotion( int iSP )
{
	if ( CGameCharacterStatus::SPEC_SPPOTION_EFFICIENCE > 0 )
		iSP += (iSP * CGameCharacterStatus::SPEC_SPPOTION_EFFICIENCE) / 100;

	CALL_WITH_ARG1( 0x005078B0, (DWORD)iSP );
}

BOOL CharacterGame::CanUseGold( int iGold )
{
	int i = 0;
	__asm
	{
		PUSH EAX;
		MOV EAX, 0x004E5640;
		PUSH iGold;
		CALL EAX;
		MOV i, EAX;
		POP EAX;
	}



	return (i >= 0);
}

void CharacterGame::ResetStats( int iNum )
{
	//Reset All Stats
	if ( iNum == 6 )
	{
		CALLT_WITH_ARG1( pfnResetAllStats, 0x035D0EA0, TRUE );
	}
	else
	{
		CALLT_WITH_ARG1( pfnResetStats, 0x035D0EA0, iNum );
	}
}

void CharacterGame::FocusTargetPet( int iID )
{
	if ( iLastFocusID != iID )
	{
		PacketUpdatePetDataEx sPacket;
		sPacket.iLength = sizeof( PacketUpdatePetDataEx );
		sPacket.iHeader = PKTHDR_UpdatePetDataEx;
		sPacket.iTargetID = iID;

		SENDPACKETG( &sPacket );

		iLastFocusID = iID;
	}
}

void CharacterGame::HandlePacket( PacketUpdatePetDataEx * psPacket )
{
	#define PARTICLEID_TARGETFOCUS		0x2002

	UnitData * pcUnitData = UNITDATABYID( psPacket->iTargetID );

	if( pcUnitData )
	{
		PARTICLEENGINE->Kill( PARTICLEID_TARGETFOCUS );
		CParticleScript * pc = PARTICLEFACTORY->LoadScript( "game\\scripts\\particles\\Target.luac" );

		if( pc )
		{
			pc->SetID( PARTICLEID_TARGETFOCUS );
			Engine::ObjectOwnerUnit * pcOwner = new Engine::ObjectOwnerUnit( pcUnitData );
			pcOwner->AddAnimationTypeLostCondition( ANIMATIONTYPE_Die );
			pc->SetOwner( pcOwner );
			PARTICLEENGINE->AddScript( pc );
		}
	}

	if ( psPacket->iTargetID == (-1) )
		iLastFocusID = -1;
}

void CharacterGame::HandlePacket( PacketCharacterDataEx * psPacket )
{
	CopyMemory( &sCharacterDataEx, &psPacket->sData, sizeof( CharacterDataEx ) );

	if ( psPacket->cCostumeWeapon.bValid )
		HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->SetWeaponCostume( &psPacket->cCostumeWeapon );
	if ( psPacket->cCostumeShield.bValid )
		HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->SetShieldCostume( &psPacket->cCostumeShield );

	if ( psPacket->cEarring1.bValid )
		HUDHANDLER->GetInventoryWindow()->SetEarring1( &psPacket->cEarring1 );
	if ( psPacket->cEarring2.bValid )
		HUDHANDLER->GetInventoryWindow()->SetEarring2( &psPacket->cEarring2 );
}

void CharacterGame::HandlePacket( PacketUpdateIntegrity * psPacket )
{
	UINT uIntegrityDecrease = 0;
	UINT uIntegrityValueNotify = 0;

	switch( psPacket->eUpdate )
	{
		case INTEGRITYUPDATE_KilledMonster:
			uIntegrityDecrease = 10;
			uIntegrityValueNotify = 30;
			break;
		case INTEGRITYUPDATE_KilledUser:
			uIntegrityDecrease = 5;
			uIntegrityValueNotify = 20;
			break;
	}

	//Update equipped Items
	for( int i = 0; i < 15; i++ )
	{
		if( INVENTORYITEMSLOT[i].iItemIndex )
		{
			ItemData * psItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[i].iItemIndex - 1];

			if( psItemData->bValid && psItemData->sItem.sIntegrity.sMax > 0 )
			{
				psItemData->sItem.sIntegrity.sCur -= uIntegrityDecrease;
				if( psItemData->sItem.sIntegrity.sCur <= 0 )
				{
					psItemData->sItem.sIntegrity.sCur = 0;
					psItemData->sItem.bCanNotUse = TRUE;
				}
				else if( psItemData->sItem.sIntegrity.sCur > psItemData->sItem.sIntegrity.sMax )
					psItemData->sItem.sIntegrity.sCur = psItemData->sItem.sIntegrity.sMax;
				else
				{
					if ( psItemData->sItem.sIntegrity.sCur <= uIntegrityValueNotify )
					{
						TITLEBOX( "%s's Integrity is too low!", psItemData->sItem.szItemName );
						ChatGame::AddChatBoxTextEx( CHATCOLOR_Notice, "> %s's Integrity is too low!", psItemData->sItem.szItemName );
					}
				}
			}
		}
	}
}

void CharacterGame::SetEXP( INT64 iEXP )
{
	LARGE_INTEGER li;
	li.QuadPart = iEXP;

	ExpXor();

	CALL_WITH_ARG3( 0x004599D0, (DWORD)&UNITDATA->sCharacterData, li.LowPart, li.HighPart );

	ExpXor();
}

INT64 CharacterGame::GetEXP()
{
	INT64 iEXP = ExpXor();
	ExpXor();
	return iEXP;
}

int CharacterGame::GetCanCarryGold()
{
	if ( UNITDATA->sCharacterData.iLevel <= 10 )
		return 200000;

	BOOL bGoldBag = ITEMHANDLER->GetItemInventoryByCode( ITEMID_GoldBag ) ? TRUE : FALSE;

	int iGoldCarryMax = 0;

	switch ( UNITDATA->sCharacterData.iRank )
	{
		case CHARACTERRANK_Rank1:
			iGoldCarryMax = (UNITDATA->sCharacterData.iLevel * 200000) - 1800000;
			break;
		case CHARACTERRANK_Rank2:
			iGoldCarryMax = 10000000;		//10kk
			break;
		case CHARACTERRANK_Rank3:
			iGoldCarryMax = 50000000;		//50kk
			break;
		case CHARACTERRANK_Rank4:
			iGoldCarryMax = 500000000;		//500kk
			break;
		case CHARACTERRANK_Rank5:
			iGoldCarryMax = 750000000;		//750kk
			break;
	}

	if ( bGoldBag )
		iGoldCarryMax *= 2;

	return iGoldCarryMax;
}

BOOL CharacterGame::CanCarryGold( int iGold )
{
	if ( (UNITDATA->sCharacterData.iGold + iGold) < 0 )
		return FALSE;

	if ( (UNITDATA->sCharacterData.iGold + iGold) <= GetCanCarryGold() )
		return TRUE;

	CTITLEBOX->SetText( 13 );

	return FALSE;
}

DWORD CharacterGame::GetCharacterColorByClass( int iClass )
{
	DWORD dwColor = 0;

	switch ( iClass )
	{
		case CHARACTERCLASS_Fighter:
			dwColor = D3DCOLOR_XRGB( 255, 140, 0 );		//Dark Orange
			break;
		case CHARACTERCLASS_Mechanician:
			dwColor = D3DCOLOR_XRGB( 0, 250, 154 );		//Med Spring Green
			break;
		case CHARACTERCLASS_Archer:
			dwColor = D3DCOLOR_XRGB( 255, 69, 0 );		//Orange Red
			break;
		case CHARACTERCLASS_Pikeman:
			dwColor = D3DCOLOR_XRGB( 30, 144, 255 );	//Dodger Blue
			break;
		case CHARACTERCLASS_Atalanta:
			dwColor = D3DCOLOR_XRGB( 218, 165, 32 );	//Goldenrod
			break;
		case CHARACTERCLASS_Knight:
			dwColor = D3DCOLOR_XRGB( 0, 191, 255 );		//Deep Sky Blue
			break;
		case CHARACTERCLASS_Magician:
			dwColor = D3DCOLOR_XRGB( 50, 205, 50 );		//Lime Green
			break;
		case CHARACTERCLASS_Priestess:
			dwColor = D3DCOLOR_XRGB( 238, 0, 238 );		//Magenta 2
			break;
		case CHARACTERCLASS_Assassin:
			dwColor = D3DCOLOR_XRGB( 125, 38, 205 );	//Purple 3
			break;
		case CHARACTERCLASS_Shaman:
			dwColor = D3DCOLOR_XRGB( 205, 0, 0 );		//Red 3
			break;
	}

	return dwColor;
}

void CharacterGame::UpdateCharacterPlayData()
{
	int iFramesSendCount = UNITGAME->GetFramesSendCount();
	int iPing = SOCKETGAME ? SOCKETGAME->GetAveragePingGame() : 400;

	if ( TRADEHANDLER->IsWindowTradeOpen() )
	{
		if ( (TRADEHANDLER->IsTradeSent() == FALSE) && TRADE_ACCEPT && TRADEUNIT_ACCEPT )
		{
			TRADEHANDLER->SendTradeCheckItem( TRADE_UNITID );
			TRADEHANDLER->IsTradeSent( TRUE );
		}
	}

	if ( TRADE_MASKTIME )
	{
		if ( TRADE_MASKTIME < TICKCOUNT )
			TRADE_MASKTIME = 0;
	}

	if ( (int)(CHARACTER_PLAYCOUNTER - CHARACTER_LASTPLAYCOUNTER) > iFramesSendCount )
	{
		UNITGAME->ClearFarUnits();

		CHARACTER_LASTPLAYCOUNTER = CHARACTER_PLAYCOUNTER;
		CHARACTER_LASTSENDPLAYCOUNTER = TICKCOUNT;
	}

	if ( SOCKETG )
	{
		DWORD dwTime = TICKCOUNT - CHARACTER_LASTTIMESENDPLAYDATA;

		if ( UNITDATA->iID )
		{
			if ( ((UNITDATA->iMoveSequenceCounter % iFramesSendCount) == 0) )
			{
				UNITDATA->HandleUnitDataBuffer( (char*)UNITDATA->baUnitInfo, iFramesSendCount, 1 );

				PacketPlayData * psPacket = (PacketPlayData *)UNITDATA->baUnitInfo;

				psPacket->iLastX = UNITDATA->sPosition.iX;
				psPacket->iLastZ = UNITDATA->sPosition.iZ;
				psPacket->bDie = (UNITDATA->GetAnimation() == ANIMATIONTYPE_Die);

				SENDPACKETG( psPacket );
				
				SOCKET_NETSTANDCOUNT = (psPacket->iPlayBuffCount > 1) ? 0 : (SOCKET_NETSTANDCOUNT + 1);

				SOCKETSEND_DEBUGCOUNT++;

				SOCKET_RECEIVEGAMESERVER = FALSE;
				CHARACTER_LASTTIMESENDPLAYDATA = TICKCOUNT;
			}

			//Damage Send
			if ( ((UNITDATA->iMoveSequenceCounter % (iFramesSendCount >> 1)) == 0) )
			{

			}
		}
	}

	if ( GAME_LOADINGTIME )
	{
		if ( TICKCOUNT > (GAME_LOADINGTIME + 10000) )
		{
			DISCONNECT_TIME = TICKCOUNT;
			GAME_LOADINGTIME = 0;
		}
	}

	if ( (CHARACTER_PLAYCOUNTER & 0x1FF) == 0x80 )
		CALL( 0x00628C00 );

	if ( CHARACTER_VISIBLECOUNT > 0 )
		CHARACTER_VISIBLECOUNT--;
}

void CharacterGame::OnCooldownSkillUpdate()
{
	auto ReduceSkillCooldownPercent = []( int iBeginSkillIndex, int iEndSkillIndex, int iCooldown ) -> void
	{
		if ( iCooldown > 0 )
		{
			for ( int i = iBeginSkillIndex; i <= iEndSkillIndex; i++ )
			{
				SKILLSCHARACTER[i].Mastery -= (SKILLSCHARACTER[i].Mastery * iCooldown) / 100;
				if ( SKILLSCHARACTER[i].Mastery < 2 )
					SKILLSCHARACTER[i].Mastery = 2;

				CALLT_WITH_ARG2( 0x004EC0B0, 0x0367E148, NULL, i );
			}
		}
	};

	ReduceSkillCooldownPercent( 1, 4, iCooldownT1 );
	ReduceSkillCooldownPercent( 5, 8, iCooldownT2 );
	ReduceSkillCooldownPercent( 9, 12, iCooldownT3 );
	ReduceSkillCooldownPercent( 13, 16, iCooldownT4 );
	ReduceSkillCooldownPercent( 17, 20, iCooldownT5 );
}
