#include "stdafx.h"
#include "CGameSkillHandlerOld.h"

CGameSkillHandlerOld::CGameSkillHandlerOld()
{
}

CGameSkillHandlerOld::~CGameSkillHandlerOld()
{
}


int CGameSkillHandlerOld::OpenPlaySkill( Skill * psSkill )
{
	
	BOOL bRet = FALSE;
	int cnt;
	MinMax sDamageWeapon;

	//Use Skill
	WRITEDWORD( 0x03673B78, (DWORD)psSkill );

	if ( (UNITDATA->iLoadedMapIndex) >= 0 && (FIELDGAME[UNITDATA->iLoadedMapIndex]->iMapType == MAPTYPE_Town) )
		return bRet;

	if ( SKILLMANAGERHANDLER->CanUseSkill( psSkill ) == FALSE )
		return bRet;
	
	if ( psSkill->iLevel > 10 )
		return bRet;

	sDamageWeapon = INVENTORYITEMS[INVENTORYITEMSLOT[0].iItemIndex - 1].sItem.sDamage;

	psSkill->dwSkillTarget = 0;

	switch ( psSkill->sSkillInfo.eSkillID )
	{
		case SKILLID_ExtremeShield:
			if ( UNITDATA->GetAnimation() < ANIMATIONTYPE_Attack )
			{
				if ( USESKILL( psSkill ) == TRUE )
				{
					UNITDATA->sActiveSkill.iValue = SKILLPLAYID_T11;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, 0, 0, 0, 0 );
					UNITDATA->Animate( ANIMATIONTYPE_Skill );
					UNITDATA->iFPS = 100;
					UNITDATA->bOverrideFPS = TRUE;
					bRet = TRUE;
				}
			}
			break;

		case SKILLID_PhysicalAbsorb:
			if ( UNITDATA->GetAnimation() < ANIMATIONTYPE_Attack )
			{
				if ( USESKILL( psSkill ) == TRUE )
				{

					UNITDATA->sActiveSkill.iValue = SKILLPLAYID_T13;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

					UNITDATA->Animate( ANIMATIONTYPE_Skill );
					UNITDATA->iFPS = 70;
					UNITDATA->bOverrideFPS = TRUE;

					PROCESSSKILLBUFFEX( SKILLPLAYID_T13, psSkill->iLevel, 0, 0 );

					SKILLMANAGERHANDLER->SkillPlaySound( SKILLSOUNDID_PHYSICAL_ABSORB3, UNITDATA->sPosition.iX, UNITDATA->sPosition.iY, UNITDATA->sPosition.iZ, TRUE );
					bRet = TRUE;
				}
			}
			break;

		case SKILLID_PoisonAttribute:
			if ( USESKILL( psSkill ) == TRUE )
			{
				//BeginSkill( SKILL_PLAY_POISON_ATTRIBUTE, 0, 0, 0, 0, 0 );
				bRet = TRUE;
			}
			break;


		case SKILLID_PikeWind:
			if ( USESKILL( psSkill ) == TRUE )
			{
				if ( psSkill->iLevel )
				{
					BeginSkill( SKILLPLAYID_T11, 0, 0, 0, 0, 0 );
					UNITDATA->sActiveSkill.iValue = SKILLPLAYID_T11;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);
					UNITDATA->Animate( ANIMATIONTYPE_Skill );

					UNITDATA->iFPS = 80;
					UNITDATA->bOverrideFPS = TRUE;

					SKILLMANAGERHANDLER->SetUnitsRange( UNITDATA->sPosition, sSkillTable.iaPikemanT11PushbackRange[psSkill->iLevel - 1], FALSE );

					SKILLMANAGERHANDLER->DamageAreaHandle( UNITDATA->pcTarget, UNITDATA->sPosition, 0, 0, 0, 0, UNITDATA->sActiveSkill.iValue );

					bRet = TRUE;
				}
			}
			break;

		case SKILLID_ScoutHawk:
			if ( UNITDATA->iPetID != SKILLID_ScoutHawk && USESKILL( psSkill ) == TRUE )
			{

				UNITDATA->sActiveSkill.iValue = SKILLPLAYID_T11;
				UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);
				BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );
				UNITDATA->Animate( ANIMATIONTYPE_Skill );

				//Skill Backup?
				memcpy( ((Skill*)0x00A15998), psSkill, sizeof( Skill ) );
				UNITDATA->bHideRightWeapon = TRUE;
				bRet = TRUE;
			}
			break;
		case SKILLID_Maximize:
			if ( USESKILL( psSkill ) == TRUE && psSkill->iLevel )
			{

				UNITDATA->sActiveSkill.iValue = SKILLPLAYID_T22;
				UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);
				BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

				UNITDATA->Animate( ANIMATIONTYPE_Skill );
				UNITDATA->iFPS = 70;
				UNITDATA->bOverrideFPS = TRUE;

				bRet = TRUE;
			}
			break;

		case SKILLID_Automation:
			if ( USESKILL( psSkill ) == TRUE && psSkill->iLevel )
			{

				UNITDATA->sActiveSkill.iValue = SKILLPLAYID_T13;
				UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);
				BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

				UNITDATA->Animate( ANIMATIONTYPE_Skill );
				UNITDATA->iFPS = 70;
				UNITDATA->bOverrideFPS = TRUE;

				SKILLMANAGERHANDLER->SkillPlaySound( SKILLSOUNDID_SKILL_AUTOMATION3, UNITDATA->sPosition.iX, UNITDATA->sPosition.iY, UNITDATA->sPosition.iZ, TRUE );

				bRet = TRUE;
			}
			break;

		case SKILLID_GroundPike:
			if ( USESKILL( psSkill ) == TRUE )
			{
				if ( psSkill->iLevel )
				{
					UNITDATA->sActiveSkill.iValue = SKILLPLAYID_T21;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );
					UNITDATA->Animate( ANIMATIONTYPE_Skill );

					UNITDATA->iFPS = 80;
					UNITDATA->bOverrideFPS = TRUE;

					SKILLMANAGERHANDLER->SetUnitsRange( UNITDATA->sPosition, sSkillTable.iaPikemanT21FreezeAttackRange[psSkill->iLevel - 1], FALSE );

					SKILLMANAGERHANDLER->DamageAreaHandle( UNITDATA->pcTarget, UNITDATA->sPosition, 0, 0, 0, 0, UNITDATA->sActiveSkill.iValue );

					bRet = TRUE;
				}
			}
			break;


		case SKILLID_VenomSpear:
			if ( USESKILL( psSkill ) == TRUE )
			{
				if ( psSkill->iLevel )
				{
					UNITDATA->sActiveSkill.iValue = SKILLPLAYID_T31;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );
					UNITDATA->Animate( ANIMATIONTYPE_Skill );

					UNITDATA->iFPS = 80;
					UNITDATA->bOverrideFPS = TRUE;
					bRet = TRUE;
				}
			}
			break;

		case SKILLID_Vanish:
			if ( USESKILL( psSkill ) == TRUE )
			{
				if ( psSkill->iLevel )
				{
					UNITDATA->sActiveSkill.iValue = SKILLPLAYID_T32;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );
					UNITDATA->Animate( ANIMATIONTYPE_Skill );

					UNITDATA->iFPS = 80;
					UNITDATA->bOverrideFPS = TRUE;

					bRet = TRUE;
				}
			}
			break;

		case SKILLID_Falcon:
			if ( UNITDATA->iPetID != SKILLID_Falcon && USESKILL( psSkill ) == TRUE )
			{
				if ( psSkill->iLevel )
				{
					UNITDATA->sActiveSkill.iValue = SKILLPLAYID_T21;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );
					UNITDATA->Animate( ANIMATIONTYPE_Skill );

					UNITDATA->iFPS = 80;
					UNITDATA->bOverrideFPS = TRUE;
					UNITDATA->bHideRightWeapon = TRUE;

					(*(DWORD*)0x00CDFF50) = (psSkill->iLevel - 1) ^ UNITDATA->iID ^ ((DWORD)0x00D16888);

					PROCESSSKILLBUFFEX( SKILLPLAYID_T21, psSkill->iLevel, 0, 0 );
				}
			}
			break;

		case SKILLID_GoldenFalcon:
			if ( UNITDATA->iPetID != SKILLID_GoldenFalcon && USESKILL( psSkill ) == TRUE )
			{
				if ( psSkill->iLevel )
				{
					UNITDATA->sActiveSkill.iValue = SKILLPLAYID_T32;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );
					UNITDATA->Animate( ANIMATIONTYPE_Skill );

					UNITDATA->iFPS = 80;
					UNITDATA->bOverrideFPS = TRUE;
					UNITDATA->bHideRightWeapon = TRUE;

					(*(DWORD *)0x00CDFF50) = (psSkill->iLevel - 1) ^ UNITDATA->iID ^ ((DWORD)0x00D16888);

					cnt = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_Falcon );
					PROCESSSKILLBUFFEX( SKILLPLAYID_T32, psSkill->iLevel, cnt, 0 );
				}
			}
			break;


		case SKILLID_Roar:
			if ( USESKILL( psSkill ) == TRUE )
			{
				if ( psSkill->iLevel )
				{
					UNITDATA->sActiveSkill.iValue = SKILLPLAYID_T23;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );
					UNITDATA->Animate( ANIMATIONTYPE_Skill );

					UNITDATA->iFPS = 80;
					UNITDATA->bOverrideFPS = TRUE;

					SKILLMANAGERHANDLER->SetUnitsRange( UNITDATA->sPosition, sSkillTable.iaFighterT23AreaofEffect[psSkill->iLevel - 1], FALSE );

					SKILLMANAGERHANDLER->DamageAreaHandle( UNITDATA->pcTarget, UNITDATA->sPosition, 0, 0, 0, 0, UNITDATA->sActiveSkill.iValue );

					bRet = TRUE;
				}
			}
			break;

		case SKILLID_Concentration:
			if ( USESKILL( psSkill ) == TRUE )
			{
				if ( psSkill->iLevel )
				{
					UNITDATA->sActiveSkill.iValue = SKILLSHORTID_Concentration;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );
					UNITDATA->Animate( ANIMATIONTYPE_Skill );

					UNITDATA->iFPS = 80;
					UNITDATA->bOverrideFPS = TRUE;

					bRet = TRUE;
				}
			}
			break;

		case SKILLID_SwiftAxe:
			if ( USESKILL( psSkill ) == TRUE )
			{
				if ( psSkill->iLevel )
				{
					UNITDATA->sActiveSkill.iValue = SKILLSHORTID_SwiftAxe;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );
					UNITDATA->Animate( ANIMATIONTYPE_Skill );

					UNITDATA->iFPS = 80;
					UNITDATA->bOverrideFPS = TRUE;

					bRet = TRUE;
				}
			}
			break;

		case SKILLID_MetalArmor:
			if ( UNITDATA->GetAnimation() < ANIMATIONTYPE_Attack )
			{
				if ( USESKILL( psSkill ) == TRUE )
				{

					UNITDATA->sActiveSkill.iValue = SKILLSHORTID_MetalArmor;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

					UNITDATA->Animate( ANIMATIONTYPE_Skill );
					UNITDATA->iFPS = 70;
					UNITDATA->bOverrideFPS = TRUE;

					cnt = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_PhysicalAbsorb );

					PROCESSSKILLBUFFEX( SKILLSHORTID_MetalArmor, psSkill->iLevel, cnt, 0 );

					SKILLMANAGERHANDLER->SkillPlaySound( SKILLSOUNDID_PHYSICAL_ABSORB3 , UNITDATA->sPosition.iX,UNITDATA->sPosition.iY,UNITDATA->sPosition.iZ, TRUE );
					bRet = TRUE;
				}
			}
			break;

		case SKILLID_SparkShield:
			if ( UNITDATA->GetAnimation() < ANIMATIONTYPE_Attack )
			{
				if ( USESKILL( psSkill ) == TRUE )
				{

					UNITDATA->sActiveSkill.iValue = SKILLSHORTID_SparkShield;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

					UNITDATA->Animate( ANIMATIONTYPE_Skill );
					UNITDATA->iFPS = 70;
					UNITDATA->bOverrideFPS = TRUE;

					cnt = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_Spark );

					PROCESSSKILLBUFFEX( SKILLSHORTID_SparkShield, psSkill->iLevel, cnt, 0 );

					SKILLMANAGERHANDLER->SkillPlaySound( SKILLSOUNDID_PHYSICAL_ABSORB3 , UNITDATA->sPosition.iX,UNITDATA->sPosition.iY,UNITDATA->sPosition.iZ, TRUE );
					bRet = TRUE;
				}
			}
			break;

		case SKILLID_HolyBody:
			if ( UNITDATA->GetAnimation() < ANIMATIONTYPE_Attack )
			{
				if ( USESKILL( psSkill ) == TRUE )
				{
					UNITDATA->sActiveSkill.iValue = SKILLSHORTID_HolyBody;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

					UNITDATA->Animate( ANIMATIONTYPE_Skill );
					UNITDATA->iFPS = 70;
					UNITDATA->bOverrideFPS = TRUE;

					PROCESSSKILLBUFFEX( SKILLSHORTID_HolyBody, psSkill->iLevel, 0, 0 );

					SKILLMANAGERHANDLER->SkillPlaySound( SKILLSOUNDID_PHYSICAL_ABSORB3, UNITDATA->sPosition.iX, UNITDATA->sPosition.iY, UNITDATA->sPosition.iZ, TRUE );
					bRet = TRUE;
				}
			}
			break;

		case SKILLID_HolyValor:
			if ( USESKILL( psSkill ) == TRUE )
			{
				UNITDATA->sActiveSkill.iValue = SKILLSHORTID_HolyValor;
				UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);
				BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

				UNITDATA->Animate( ANIMATIONTYPE_Skill );
				UNITDATA->iFPS = 70;
				UNITDATA->bOverrideFPS = TRUE;
				bRet = TRUE;
			}
			break;

		case SKILLID_DrasticSpirit:
			if ( USESKILL( psSkill ) == TRUE )
			{
				UNITDATA->sActiveSkill.iValue = SKILLSHORTID_DrasticSpirit;
				UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);
				BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

				UNITDATA->Animate( ANIMATIONTYPE_Skill );
				UNITDATA->iFPS = 70;
				UNITDATA->bOverrideFPS = TRUE;
				bRet = TRUE;
			}
			break;

		case SKILLID_DivineShield:
			if ( UNITDATA->GetAnimation() < ANIMATIONTYPE_Attack )
			{
				if ( USESKILL( psSkill ) == TRUE )
				{
					UNITDATA->sActiveSkill.iValue = SKILLSHORTID_DivineShield;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

					UNITDATA->Animate( ANIMATIONTYPE_Skill );
					UNITDATA->iFPS = 70;
					UNITDATA->bOverrideFPS = TRUE;

					PROCESSSKILLBUFFEX( SKILLSHORTID_DivineShield, psSkill->iLevel, 0, 0 );
					
					SKILLMANAGERHANDLER->SkillPlaySound( SKILLSOUNDID_PHYSICAL_ABSORB3 , UNITDATA->sPosition.iX,UNITDATA->sPosition.iY,UNITDATA->sPosition.iZ, TRUE );

					bRet = TRUE;
				}
			}
			break;

		case SKILLID_Windy:
			if ( USESKILL( psSkill ) == TRUE )
			{
				UNITDATA->sActiveSkill.iValue = SKILLSHORTID_Windy;
				UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);
				BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

				UNITDATA->Animate( ANIMATIONTYPE_Skill );
				bRet = TRUE;
			}
			break;

		case SKILLID_TriumphofJavelin:
			if ( (UNITDATA_SELECTED == NULL) &&
				 (EffectBaseFollow::IsActiveEffect( UNITDATA, SKILLID_TriumphofJavelin ) == FALSE) &&
				 (SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_TriumphofJavelin ) == FALSE) &&
				 (USESKILL( psSkill ) == TRUE) )
			{

				UNITDATA->sActiveSkill.iValue = SKILLSHORTID_TriumphOfValhalla;
				UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);
				BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

				UNITDATA->Animate( ANIMATIONTYPE_Skill );

				PROCESSSKILLBUFFEX( SKILLSHORTID_TriumphOfValhalla, psSkill->iLevel, 0, UNITDATA->sCharacterData.iLevel );
				UNITDATA->pcTarget = NULL;
				
				AddTriumphOfValhalla( psSkill->iLevel, UNITDATA->sCharacterData.iLevel );

				bRet = TRUE;
			}
			break;
/*
		case SKILLID_Healing:
			if ( UNITDATA->GetAnimation() < ANIMATIONTYPE_Attack && (UNITDATA_SELECTED == NULL) )
			{
				if ( USESKILL( psSkill ) == TRUE )
				{

					UNITDATA->sActiveSkill.iValue = SKILLSHORTID_Healing;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);				//스킬 포인트 상위 바이트에 저장
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

					UNITDATA->Animate( ANIMATIONTYPE_Skill );
					UNITDATA->iFPS = 70;
					UNITDATA->bOverrideFPS = TRUE;

					cnt = GetRandomPos( Healing_Heal[psSkill->iLevel - 1][0] + wpDamage[0], Healing_Heal[psSkill->iLevel - 1][1] + wpDamage[0] );

					sinSetLife( sinGetLife() + cnt );
					ResetEnergyGraph( 0 );
					bRet = TRUE;
				}
			}
			break;
		case SKILL_DIVINE_LIGHTNING:
			//디바인 라이트닝
			if ( USESKILL( psSkill ) == TRUE )
			{

				//SkillPlay_DivineLightning_Select( lpCurPlayer , psSkill->iLevel );
				UNITDATA->sActiveSkill.iValue = SKILL_PLAY_DIVINE_LIGHTNING;
				UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);				//스킬 포인트 상위 바이트에 저장
				BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

				UNITDATA->Animate( ANIMATIONTYPE_Skill );
				UNITDATA->iFPS = 85;
				UNITDATA->bOverrideFPS = TRUE;

				bRet = TRUE;
			}
			break;

		case SKILL_HOLY_REFLECTION:
			if ( USESKILL( psSkill ) == TRUE )
			{
				UNITDATA->sActiveSkill.iValue = SKILL_PLAY_HOLY_REFLECTION;
				UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);				//스킬 포인트 상위 바이트에 저장
				BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

				UNITDATA->Animate( ANIMATIONTYPE_Skill );
				bRet = TRUE;
			}
			break;
		case SKILL_GRAND_HEALING:
			if ( InterfaceParty.PartyMemberCount > 0 && USESKILL( psSkill ) == TRUE )
			{
				UNITDATA->sActiveSkill.iValue = SKILL_PLAY_GREAT_HEALING;
				UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);				//스킬 포인트 상위 바이트에 저장
				BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );
				UNITDATA->Animate( ANIMATIONTYPE_Skill );

				UNITDATA->iFPS = 70;
				UNITDATA->bOverrideFPS = TRUE;

				if ( AreaServerMode )
					dm_SendTransDamage( lpCurPlayer, wpDamage[0], wpDamage[1], 0, 0, UNITDATA->sActiveSkill.iValue, FALSE, smWsockServer );
				else
					dm_SendTransDamage( lpCurPlayer, wpDamage[0], wpDamage[1], 0, 0, UNITDATA->sActiveSkill.iValue, FALSE );

				bRet = TRUE;
			}
			break;

		case SKILL_RESURRECTION:		//죽은자 소생
			if ( USESKILL( psSkill ) == TRUE )
			{
				UNITDATA->sActiveSkill.iValue = SKILL_PLAY_RESURRECTION;
				UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);				//스킬 포인트 상위 바이트에 저장
				BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

				UNITDATA->Animate( ANIMATIONTYPE_Skill );
				bRet = TRUE;
			}
			break;

		case SKILL_EXTINCTION:		//언데드 소멸
			if ( USESKILL( psSkill ) == TRUE )
			{
				UNITDATA->sActiveSkill.iValue = SKILL_PLAY_EXTINCTION;
				UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);				//스킬 포인트 상위 바이트에 저장
				BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

				UNITDATA->Animate( ANIMATIONTYPE_Skill );

				//범위형 공격 범위 선정 (원)
				SKILLMANAGERHANDLER->SetUnitsRange( UNITDATA->sPosition, 160, FALSE );
				//범위공격 설정후 전송
				SKILLMANAGERHANDLER->DamageAreaHandle( UNITDATA->pcTarget, UNITDATA->sPosition, 0, 0, 0, 0, UNITDATA->sActiveSkill.iValue );

				bRet = TRUE;
			}
			break;

		case SKILL_VIRTUAL_LIFE:		//버츌얼 라이프
			if ( !lpCharSelPlayer && GetAssaUseEffect( lpCurPlayer, SKILL_VIRTUAL_LIFE ) == FALSE && USESKILL( psSkill ) == TRUE )
			{
				UNITDATA->sActiveSkill.iValue = SKILL_PLAY_VIRTUAL_LIFE;
				UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);				//스킬 포인트 상위 바이트에 저장
				BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

				UNITDATA->Animate( ANIMATIONTYPE_Skill );

				PROCESSSKILLBUFFEX( SKILL_PLAY_VIRTUAL_LIFE, psSkill->iLevel, 0, 0 );
				cSkill.SetVirtualLife( Virtual_Life_Time[psSkill->iLevel - 1], psSkill->iLevel );
				UNITDATA->chrAttackTarget = 0;

				bRet = TRUE;
			}
			break;

			//////////////////////////// 매지션 /////////////////////////////////////
		case SKILL_AGONY:
			if ( UNITDATA->GetAnimation() < ANIMATIONTYPE_Attack )
			{
				if ( USESKILL( psSkill ) == TRUE )
				{
					UNITDATA->sActiveSkill.iValue = SKILL_PLAY_AGONY;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);				//스킬 포인트 상위 바이트에 저장
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

					UNITDATA->Animate( ANIMATIONTYPE_Skill );
					UNITDATA->iFPS = 70;
					UNITDATA->bOverrideFPS = TRUE;
					bRet = TRUE;

					//에너지 그래프 첵크 초기화
					ResetEnergyGraph( 3 );
				}
			}
			break;

		case SKILL_ZENITH:
			if ( UNITDATA->GetAnimation() < ANIMATIONTYPE_Attack )
			{
				if ( USESKILL( psSkill ) == TRUE )
				{
					UNITDATA->sActiveSkill.iValue = SKILL_PLAY_ZENITH;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);				//스킬 포인트 상위 바이트에 저장
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

					UNITDATA->Animate( ANIMATIONTYPE_Skill );
					UNITDATA->iFPS = 70;
					UNITDATA->bOverrideFPS = TRUE;

					SendResistanceToServer();

					bRet = TRUE;
				}
			}
			break;

		case SKILL_ENCHANT_WEAPON:
			if ( UNITDATA->GetAnimation() < ANIMATIONTYPE_Attack && !lpCharSelPlayer && !UNITDATA->WeaponEffect )
			{
				if ( USESKILL( psSkill ) == TRUE )
				{
					UNITDATA->chrAttackTarget = 0;

					UNITDATA->sActiveSkill.iValue = SKILL_PLAY_ENCHANT_WEAPON;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);				//스킬 포인트 상위 바이트에 저장
					cnt = rand() % 3;
					UNITDATA->sActiveSkill.iValue |= (cnt << 12);
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

					UNITDATA->Animate( ANIMATIONTYPE_Skill );
					UNITDATA->iFPS = 70;
					UNITDATA->bOverrideFPS = TRUE;
					bRet = TRUE;
				}
			}
			break;


		case SKILL_ENERGY_SHIELD:
			if ( UNITDATA->GetAnimation() < ANIMATIONTYPE_Attack )
			{
				if ( USESKILL( psSkill ) == TRUE )
				{
					UNITDATA->sActiveSkill.iValue = SKILL_PLAY_ENERGY_SHIELD;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);				//스킬 포인트 상위 바이트에 저장
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

					UNITDATA->Animate( ANIMATIONTYPE_Skill );
					UNITDATA->iFPS = 80;
					UNITDATA->bOverrideFPS = TRUE;

					PROCESSSKILLBUFFEX( SKILL_PLAY_ENERGY_SHIELD, psSkill->iLevel, 0, 0 );

					bRet = TRUE;
				}
			}
			break;

		case SKILL_DIASTROPHISM:
			if ( UNITDATA->GetAnimation() < ANIMATIONTYPE_Attack )
			{
				if ( USESKILL( psSkill ) == TRUE )
				{

					UNITDATA->sActiveSkill.iValue = SKILL_PLAY_DIASTROPHISM;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);				//스킬 포인트 상위 바이트에 저장
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

					UNITDATA->Animate( ANIMATIONTYPE_Skill );
					UNITDATA->iFPS = 120;
					UNITDATA->bOverrideFPS = TRUE;
					bRet = TRUE;
				}
			}
			break;

		case SKILL_SPIRIT_ELEMENTAL:
			if ( UNITDATA->GetAnimation() < ANIMATIONTYPE_Attack )
			{
				if ( USESKILL( psSkill ) == TRUE )
				{

					UNITDATA->sActiveSkill.iValue = SKILL_PLAY_SPIRIT_ELEMENTAL;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);				//스킬 포인트 상위 바이트에 저장
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

					UNITDATA->Animate( ANIMATIONTYPE_Skill );
					UNITDATA->iFPS = 80;
					UNITDATA->bOverrideFPS = TRUE;

					PROCESSSKILLBUFFEX( SKILL_PLAY_SPIRIT_ELEMENTAL, psSkill->iLevel, 0, 0 );

					bRet = TRUE;
				}
			}
			break;

		case SKILL_DANCING_SWORD:
			if ( UNITDATA->GetAnimation() < ANIMATIONTYPE_Attack )
			{
				if ( USESKILL( psSkill ) == TRUE )
				{

					UNITDATA->sActiveSkill.iValue = SKILL_PLAY_DANCING_SWORD;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);				//스킬 포인트 상위 바이트에 저장
					cnt = rand() % 2;
					UNITDATA->sActiveSkill.iValue |= (cnt << 12);
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

					UNITDATA->Animate( ANIMATIONTYPE_Skill );
					UNITDATA->iFPS = 80;
					UNITDATA->bOverrideFPS = TRUE;

					PROCESSSKILLBUFFEX( SKILL_PLAY_DANCING_SWORD, psSkill->iLevel, cnt, 0 );

					bRet = TRUE;
				}

			}
			break;


			////////////////////// 4차 스킬 ////////////////////////
			///메카니션
		case SKILL_COMPULSION:
			if ( UNITDATA->GetAnimation() < ANIMATIONTYPE_Attack )
			{
				if ( USESKILL( psSkill ) == TRUE )
				{

					UNITDATA->sActiveSkill.iValue = SKILL_PLAY_COMPULSION;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);				//스킬 포인트 상위 바이트에 저장
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

					UNITDATA->Animate( ANIMATIONTYPE_Skill );
					UNITDATA->iFPS = 80;
					UNITDATA->bOverrideFPS = TRUE;

					//범위형 공격 범위 선정 (원)
					SKILLMANAGERHANDLER->SetUnitsRange( UNITDATA->sPosition, Compulsion_Area[psSkill->iLevel - 1], FALSE );
					//범위공격 설정후 전송
					SKILLMANAGERHANDLER->DamageAreaHandle( UNITDATA->pcTarget, UNITDATA->sPosition, 0, 0, 0, 0, UNITDATA->sActiveSkill.iValue );

					bRet = TRUE;
				}
			}
			break;

		case SKILL_MAGNETIC_SPHERE:		//댄싱소드랑 비슷하다고 함
			if ( UNITDATA->GetAnimation() < ANIMATIONTYPE_Attack )
			{
				if ( USESKILL( psSkill ) == TRUE )
				{

					UNITDATA->sActiveSkill.iValue = SKILL_PLAY_MAGNETIC_SPHERE;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);				//스킬 포인트 상위 바이트에 저장
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

					UNITDATA->Animate( ANIMATIONTYPE_Skill );
					UNITDATA->iFPS = 80;
					UNITDATA->bOverrideFPS = TRUE;

					PROCESSSKILLBUFFEX( SKILL_PLAY_MAGNETIC_SPHERE, psSkill->iLevel, 0, 0 );

					bRet = TRUE;
				}
			}
			break;

		case SKILL_METAL_GOLEM:
			if ( UNITDATA->GetAnimation() < ANIMATIONTYPE_Attack )
			{
				if ( USESKILL( psSkill ) == TRUE )
				{

					UNITDATA->sActiveSkill.iValue = SKILL_PLAY_METAL_GOLEM;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);				//스킬 포인트 상위 바이트에 저장
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

					UNITDATA->Animate( ANIMATIONTYPE_Skill );
					UNITDATA->iFPS = 80;
					UNITDATA->bOverrideFPS = TRUE;

					PROCESSSKILLBUFFEX( SKILL_PLAY_METAL_GOLEM, psSkill->iLevel, 0, 0 );

					bRet = TRUE;
				}
			}
			break;

			///파이터
		case SKILL_BERSERKER:
			if ( UNITDATA->GetAnimation() < ANIMATIONTYPE_Attack )
			{
				if ( USESKILL( psSkill ) == TRUE )
				{

					UNITDATA->sActiveSkill.iValue = SKILL_PLAY_BERSERKER;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);				//스킬 포인트 상위 바이트에 저장
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

					UNITDATA->Animate( ANIMATIONTYPE_Skill );
					UNITDATA->iFPS = 80;
					UNITDATA->bOverrideFPS = TRUE;

					PROCESSSKILLBUFFEX( SKILL_PLAY_BERSERKER, psSkill->iLevel, 0, 0 );

					bRet = TRUE;
				}
			}
			break;
		case SKILL_VAGUE:
			if ( UNITDATA->GetAnimation() < ANIMATIONTYPE_Attack )
			{
				if ( USESKILL( psSkill ) == TRUE )
				{

					UNITDATA->sActiveSkill.iValue = SKILL_PLAY_VAGUE;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);				//스킬 포인트 상위 바이트에 저장
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

					UNITDATA->Animate( ANIMATIONTYPE_Skill );
					UNITDATA->iFPS = 80;
					UNITDATA->bOverrideFPS = TRUE;

					PROCESSSKILLBUFFEX( SKILL_PLAY_VAGUE, psSkill->iLevel, 0, 0 );

					bRet = TRUE;
				}
			}
			break;

			//아처
		case SKILL_RECALL_WOLVERIN:
			if ( UNITDATA->GetAnimation() < ANIMATIONTYPE_Attack )
			{
				if ( USESKILL( psSkill ) == TRUE )
				{

					UNITDATA->sActiveSkill.iValue = SKILL_PLAY_RECALL_WOLVERIN;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);				//스킬 포인트 상위 바이트에 저장
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

					UNITDATA->Animate( ANIMATIONTYPE_Skill );
					UNITDATA->iFPS = 80;
					UNITDATA->bOverrideFPS = TRUE;
					UNITDATA->HideWeapon = TRUE;
					bRet = TRUE;

					PROCESSSKILLBUFFEX( SKILL_PLAY_RECALL_WOLVERIN, psSkill->iLevel, 0, 0 );
				}
			}
			break;

		case SKILL_FORCE_OF_NATURE:
			if ( UNITDATA->GetAnimation() < ANIMATIONTYPE_Attack )
			{
				if ( USESKILL( psSkill ) == TRUE )
				{

					UNITDATA->sActiveSkill.iValue = SKILL_PLAY_FORCE_OF_NATURE;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);				//스킬 포인트 상위 바이트에 저장
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

					UNITDATA->Animate( ANIMATIONTYPE_Skill );
					UNITDATA->iFPS = 80;
					UNITDATA->bOverrideFPS = TRUE;

					PROCESSSKILLBUFFEX( SKILL_PLAY_FORCE_OF_NATURE, psSkill->iLevel, 0, 0 );
					cSkill.ForeceOfNature( Force_Of_Nature_Time[psSkill->iLevel - 1], psSkill->iLevel );

					UNITDATA->dwForceOfNatureTime = dwPlayTime + Force_Of_Nature_Time[psSkill->iLevel - 1] * 1000;	//포스오브 네이쳐 유지시간

					bRet = TRUE;
				}
			}
			break;

			//나이트
		case SKILL_GODLY_SHIELD:
			if ( UNITDATA->GetAnimation() < ANIMATIONTYPE_Attack )
			{
				if ( USESKILL( psSkill ) == TRUE )
				{

					UNITDATA->sActiveSkill.iValue = SKILL_PLAY_GODLY_SHIELD;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);				//스킬 포인트 상위 바이트에 저장
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

					UNITDATA->Animate( ANIMATIONTYPE_Skill );
					UNITDATA->iFPS = 80;
					UNITDATA->bOverrideFPS = TRUE;

					PROCESSSKILLBUFFEX( SKILL_PLAY_GODLY_SHIELD, psSkill->iLevel, 0, 0 );
					bRet = TRUE;
				}
			}
			break;

		case SKILL_GOD_BLESS:
			if ( UNITDATA->GetAnimation() < ANIMATIONTYPE_Attack )
			{
				if ( USESKILL( psSkill ) == TRUE )
				{

					UNITDATA->sActiveSkill.iValue = SKILL_PLAY_GODS_BLESS;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);				//스킬 포인트 상위 바이트에 저장
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

					UNITDATA->Animate( ANIMATIONTYPE_Skill );
					UNITDATA->iFPS = 80;
					UNITDATA->bOverrideFPS = TRUE;

					PROCESSSKILLBUFFEX( SKILL_PLAY_GODS_BLESS, psSkill->iLevel, 0, 0 );
					bRet = TRUE;
				}
			}
			break;

			//아탈란타
		case SKILL_HALL_OF_VALHALLA:
			if ( UNITDATA->GetAnimation() < ANIMATIONTYPE_Attack )
			{
				if ( GetAssaUseEffect( lpCurPlayer, SKILL_TRIUMPH_OF_VALHALLA ) == FALSE && USESKILL( psSkill ) == TRUE )
				{

					UNITDATA->sActiveSkill.iValue = SKILL_PLAY_HALL_OF_VALHALLA;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);				//스킬 포인트 상위 바이트에 저장
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

					UNITDATA->Animate( ANIMATIONTYPE_Skill );
					UNITDATA->iFPS = 80;
					UNITDATA->bOverrideFPS = TRUE;

					cnt = SKILLMANAGERHANDLER->GetLevelSkill( SKILL_TRIUMPH_OF_VALHALLA );			//트라이엄프발할라의 포인트
					PROCESSSKILLBUFFEX( SKILL_PLAY_HALL_OF_VALHALLA, psSkill->iLevel, cnt, UNITDATA->smCharInfo.Level );

					cSkill.HellOfValhalla( Hall_Of_Valhalla_Time[psSkill->iLevel - 1], psSkill->iLevel, cnt, UNITDATA->smCharInfo.Level, 0 );

					UNITDATA->dwHallOfValhallaTime = dwPlayTime + Hall_Of_Valhalla_Time[psSkill->iLevel - 1] * 1000;	//홀오브발할라 유지시간

					bRet = TRUE;
				}
			}
			break;

		case SKILL_FROST_JAVELIN:
			if ( UNITDATA->GetAnimation() < ANIMATIONTYPE_Attack )
			{
				if ( USESKILL( psSkill ) == TRUE )
				{

					UNITDATA->sActiveSkill.iValue = SKILL_PLAY_FROST_JAVELIN;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);				//스킬 포인트 상위 바이트에 저장
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

					UNITDATA->Animate( ANIMATIONTYPE_Skill );
					UNITDATA->iFPS = 80;
					UNITDATA->bOverrideFPS = TRUE;

					PROCESSSKILLBUFFEX( SKILL_PLAY_FROST_JAVELIN, psSkill->iLevel, 0, 0 );
					bRet = TRUE;
				}
			}
			break;

			//프리스티스
		case SKILL_REGENERATION_FIELD:
			if ( UNITDATA->GetAnimation() < ANIMATIONTYPE_Attack )
			{
				if ( USESKILL( psSkill ) == TRUE )
				{ // && InterfaceParty.PartyMemberCount>0 && psSkill->PartyFlag && psSkill->SkillTaget_CODE != 0) {

					UNITDATA->sActiveSkill.iValue = SKILL_PLAY_REGENERATION_FIELD;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);				//스킬 포인트 상위 바이트에 저장
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

					UNITDATA->Animate( ANIMATIONTYPE_Skill );
					UNITDATA->iFPS = 80;
					UNITDATA->bOverrideFPS = TRUE;

					cSkill.Regeneration_Field( Regeneration_Field_Time[psSkill->iLevel - 1], psSkill->iLevel, 0 );		//재생력 추가
					bRet = TRUE;
				}
			}
			break;

		case SKILL_SUMMON_MUSPELL:
			if ( UNITDATA->GetAnimation() < ANIMATIONTYPE_Attack )
			{
				if ( USESKILL( psSkill ) == TRUE )
				{

					UNITDATA->sActiveSkill.iValue = SKILL_PLAY_SUMMON_MUSPELL;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);				//스킬 포인트 상위 바이트에 저장
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

					UNITDATA->Animate( ANIMATIONTYPE_Skill );
					UNITDATA->iFPS = 80;
					UNITDATA->bOverrideFPS = TRUE;

					PROCESSSKILLBUFFEX( SKILL_PLAY_SUMMON_MUSPELL, psSkill->iLevel, 0, 0 );

					bRet = TRUE;
				}
			}
			break;

			//매지션
		case SKILL_FIRE_ELEMENTAL:
			if ( UNITDATA->GetAnimation() < ANIMATIONTYPE_Attack )
			{
				if ( USESKILL( psSkill ) == TRUE )
				{

					UNITDATA->sActiveSkill.iValue = SKILL_PLAY_FIRE_ELEMENTAL;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);				//스킬 포인트 상위 바이트에 저장
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

					UNITDATA->Animate( ANIMATIONTYPE_Skill );
					UNITDATA->iFPS = 80;
					UNITDATA->bOverrideFPS = TRUE;

					PROCESSSKILLBUFFEX( SKILL_PLAY_FIRE_ELEMENTAL, psSkill->iLevel, 0, 0 );

					bRet = TRUE;
				}
			}
			break;
		case SKILL_DISTORTION:
			if ( UNITDATA->GetAnimation() < ANIMATIONTYPE_Attack )
			{
				if ( USESKILL( psSkill ) == TRUE )
				{

					UNITDATA->sActiveSkill.iValue = SKILL_PLAY_DISTORTION;
					UNITDATA->sActiveSkill.iValue |= (psSkill->iLevel << 8);				//스킬 포인트 상위 바이트에 저장
					BeginSkill( UNITDATA->sActiveSkill.iValue, 0, nullptr, 0, 0, 0 );

					UNITDATA->Animate( ANIMATIONTYPE_Skill );
					UNITDATA->iFPS = 80;
					UNITDATA->bOverrideFPS = TRUE;

					//범위형 공격 범위 선정 (원)
					SKILLMANAGERHANDLER->SetUnitsRange( UNITDATA->sPosition, Distortion_Area[psSkill->iLevel - 1], FALSE );
					//범위공격 설정후 전송
					SKILLMANAGERHANDLER->DamageAreaHandle( UNITDATA->pcTarget, UNITDATA->sPosition, 0, 0, 0, 0, UNITDATA->sActiveSkill.iValue );
					bRet = TRUE;
				}

			}
			break;

			*/
	}
	
	return 0;
}

BOOL CGameSkillHandlerOld::BeginSkill( int iSkillID, int iLevel, UnitData * pcTarget, int iX, int iY, int iZ )
{
	return 0;
}

BOOL CGameSkillHandlerOld::AddTriumphOfValhalla( int iSkillLevel, int iLevel )
{
	if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_TriumphofJavelin ) || SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_HallofValhalla ) )
		return FALSE;

	Skill sSkill;
	ZeroMemory( &sSkill, sizeof( sSkill ) );
	sSkill.bActive = TRUE;
	sSkill.iID = SKILLID_TriumphofJavelin;
	sSkill.iUseTime = sSkillTable.iaAtalantaT32Duration[iSkillLevel - 1];
	sSkill.iLevel = iSkillLevel;
	TIMERSKILLHANDLER->AddSkillTimer( &sSkill );

	return TRUE;
}

