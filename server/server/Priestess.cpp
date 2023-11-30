#include "stdafx.h"
#include "HNSSkill.h"
#include "Priestess.h"



typedef void( __cdecl *t_RestoreHPPacket ) ( void * packet );
t_RestoreHPPacket RestoreHPPacket = ( t_RestoreHPPacket )0x00551700;

typedef UINT( __cdecl *t_FindPlayerByIDCode ) ( UINT IDPlayer );
t_FindPlayerByIDCode FindPlayerByIDCode = ( t_FindPlayerByIDCode )0x0054CC10;

static void SendIceMeteorite( UserData * pcUserData, int iRange, UINT uMeteoriteCount )
{
	PacketIceMeteorite sPacket;
	sPacket.iLength = sizeof( PacketIceMeteorite );
	sPacket.iHeader = PKTHDR_IceMeteorite;
	sPacket.sBegin = pcUserData->sPosition;
	sPacket.sBegin.iY += 150000;
	sPacket.sEnd = pcUserData->sPosition;
	sPacket.uMeteoriteCount = uMeteoriteCount;
	sPacket.bSmall = FALSE;
	sPacket.iDelay = RandomI( 51, 250 );

	SENDPACKETRANGE( &sPacket, &pcUserData->sPosition, DISTANCE_UNIT_VIEWLIMIT );
}

CPriestess::CPriestess()
{
}

BOOL CPriestess::OnVirtualLife( UserData * pcUserData, PacketSkillPartyData * psPacket )
{
	psPacket->iClass = 8;
	return TRUE;
}

int CPriestess::AddSummonMuspellDamageBoost( UserData * pcUserData, int iType )
{
	User * pcUser = USERDATATOUSER( pcUserData );

	if ( pcUserData->SummonMuspellTime > TICKCOUNT )
	{
		if ( iType == 1 )
		{
			int iAtkMin = pcUserData->sCharacterData.iMinDamage + ((pcUserData->sCharacterData.iMinDamage * pcUser->sSummonMuspellDamageBoost.sMin) / 100);
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Summon Muspell Damage Min [%d + %d] = %d", pcUserData->sCharacterData.iMinDamage, ((pcUserData->sCharacterData.iMinDamage * pcUser->sSummonMuspellDamageBoost.sMin) / 100), iAtkMin );
			return ((pcUserData->sCharacterData.iMinDamage * pcUser->sSummonMuspellDamageBoost.sMin) / 100);
		}
		else
		{
			int iAtkMax = pcUserData->sCharacterData.iMaxDamage + ((pcUserData->sCharacterData.iMaxDamage * pcUser->sSummonMuspellDamageBoost.sMax) / 100);
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Summon Muspell Damage Max [%d + %d] = %d", pcUserData->sCharacterData.iMaxDamage, ((pcUserData->sCharacterData.iMaxDamage * pcUser->sSummonMuspellDamageBoost.sMax) / 100), iAtkMax );
			return ((pcUserData->sCharacterData.iMaxDamage * pcUser->sSummonMuspellDamageBoost.sMax) / 100);
		}
	}

	return 0;
}

void CPriestess::OnUseHolyMind( User * pcUser, PacketHolyMind * psPacket )
{
	UserData * pcUserDataTarget = NULL;
	UnitData * pcUnitDataTarget = NULL;

	int iTime		= psPacket->iTime;
	int iAbs		= psPacket->iAbsorbPercent;
	int iResistance = 0;

	if ( (pcUserDataTarget = USERDATABYID( psPacket->dwObjectID )) )
	{
		iResistance = pcUserDataTarget->sCharacterData.sElementalDef[0];

		if ( iResistance < 0 )
			iResistance = 0;
		if ( iResistance > 100 )
			iResistance = 100;

		iTime	= (iTime*(100 - iResistance)) / 100;
		iAbs	= (iAbs*(100 - iResistance)) / 100;

		CHATSERVER->SendDebugChat( pcUser->pcUserData, CHATCOLOR_Error, "> Holy Mind [%s] Time[%d] Abs[%d] Res[%d]", CHARACTERSERVER->GetCharacterName( pcUserDataTarget ), iTime, iAbs, iResistance );
		
		if ( iTime > 0 )
		{
			User * u = USERDATATOUSER( pcUserDataTarget );
			if ( u )
			{
				u->dwHolyMindTime	= (iTime * 1000) + TICKCOUNT;
				u->iHolyMindAbsorb	= iAbs;
			}
		}

	}
	else if ( (pcUnitDataTarget = UNITSERVER->GetUnit(psPacket->dwObjectID)) )
	{
		iResistance = pcUnitDataTarget->sCharacterData.sElementalDef[0];

		if ( iResistance < 0 )
			iResistance = 0;
		if ( iResistance > 100 )
			iResistance = 100;

		iTime	= iTime << 4;

		iTime	= (iTime*(100 - iResistance)) / 100;
		iAbs	= (iAbs*(100 - iResistance)) / 100;
		
		if ( iTime > 0 )
		{
			pcUnitDataTarget->sHolyMind.sMin = (short)iAbs;
			pcUnitDataTarget->sHolyMind.sMax = (short)iTime;
		}
	}
}

void CPriestess::OnHolyMindHandler( UserData * pcUserDataTarget, int * piDamage )
{
	if ( pcUserDataTarget )
	{
		User * u = USERDATATOUSER( pcUserDataTarget );
		if ( u )
		{
			if ( u->dwHolyMindTime > TICKCOUNT )
			{
				piDamage[0] -= ((piDamage[0]) * u->iHolyMindAbsorb) / 100;
			}
			else
			{
				u->dwHolyMindTime	= 0;
				u->iHolyMindAbsorb	= 0;
			}
			if ( u->dwWispTime > TICKCOUNT )
			{
				piDamage[0] -= ((piDamage[0]) * u->iWispAbsorb) / 100;
			}
			else
			{
				u->dwWispTime	= 0;
				u->iWispAbsorb	= 0;
			}
		}
	}
}

int CPriestess::RecvSkillSingleTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageNormalEx * psPacketEx )
{
	UINT uSkillID = psPacketEx->iSkillID & 0x000000FF;
	UINT uUnkV = ( psPacketEx->iSkillID / 4096 ) & 0x0000000F;
	int iLevelSkill = ( psPacketEx->iSkillID / 256 ) & 0x0000000F;

	*( UINT* )0x7ACA048 = psPacketEx->iSkillID;

	if ( uSkillID != 0 && iLevelSkill <= 0 || iLevelSkill > 10 )
		return 0;

	int iValue = 0;

	User * pcUser = USERDATATOUSER( pcUserData );

	switch ( uSkillID )
	{

		case Skills::DefaultAttack:
		{
			int iDamageBoost = psPacket->iDamage;
			psPacket->iDamage = iDamageBoost;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Default Attack]" );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
		} break;

		case Skills::Healing:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Healing]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT HPRestored FROM Healing WHERE SkillLevel=%d", iLevelSkill );
			int iStaticDamage = SQLSKILL->GetSkillValueInt( "SELECT StaticDamage FROM Healing WHERE SkillLevel=%d", iLevelSkill );
			int iDamageBoost = psPacket->iDamage;
			struct PacketRestoreHP
			{
				int size;
				int op;
				int null;
				int hpboost;
				int idmob;
				int IDPlayer;
			};
			struct PacketRestoreHP phpr;
			phpr.size = sizeof( phpr );
			phpr.op = 0x484700D2;
			phpr.null = 0;
			phpr.hpboost = ( ( iDamageBoost * iValue ) / 100 ) + iStaticDamage;
			if ( pcUnitData && pcUnitData->sCharacterData.iMonsterEffectID != MONSTEREFFECTID_CrystalTower )
			{
				phpr.hpboost -= ((phpr.hpboost * pcUnitData->sCharacterData.iAbsorbRating) / 100);

				phpr.idmob = psPacketEx->iID;
				phpr.IDPlayer = pcUserData->iID;
				RestoreHPPacket( &phpr );
			}
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> HP Restored: %d", phpr.hpboost );
			return 0;
		} break;



		case Skills::GrandHealing:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Grand Healing]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT HPRestored FROM GrandHealing WHERE SkillLevel=%d", iLevelSkill );
			int iStaticDamage = SQLSKILL->GetSkillValueInt( "SELECT StaticDamage FROM GrandHealing WHERE SkillLevel=%d", iLevelSkill );
			int iDamageBoost = psPacket->iDamage;			
			PacketRestoreHP phpr;
			phpr.size = sizeof( phpr );
			phpr.op = 0x484700D4;
			phpr.hpboost = ( ( iDamageBoost * iValue ) / 100 ) + iStaticDamage;
			phpr.SKILLLVL = iLevelSkill;
			iValue = SQLSKILL->GetSkillValueInt( "SELECT PartyRange FROM GrandHealing WHERE SkillLevel=%d", iLevelSkill );
			phpr.pRange = iValue;
			phpr.IDPlayer = pcUserData->iID;
			RestoreHPPacketParty( pcUserData, &phpr );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> HP Restored: %d", phpr.hpboost );
			return 0;
		} break;

		case Skills::HolyBolt:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Holy Bolt]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM HolyBolt WHERE SkillLevel=%d", iLevelSkill );
			int iStaticDamage = SQLSKILL->GetSkillValueInt( "SELECT StaticDamage FROM HolyBolt WHERE SkillLevel=%d", iLevelSkill );
			int iDamageBoost = psPacket->iDamage;
			psPacket->iDamage += iStaticDamage + ( ( iDamageBoost * iValue ) / 100 );
			psPacket->iDamageState = ( psPacket->iDamageState & 0xFFFF0000 ) + 2;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
			psPacket->dwDamageChkSum = 0;
		} break;

		case Skills::VigorBall:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Vigor Ball]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM VigorBall WHERE SkillLevel=%d", iLevelSkill );
			int iDamageBoost = psPacket->iDamage; 

			if ( pcUnitData && pcUnitData->sCharacterData.iMonsterType == MONSTERTYPE_Undead )
			{
				CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Vigor Ball +100%% Damage vs. Undead" );
				iValue += 100;
			}
			psPacket->iDamage = iDamageBoost + ( ( iDamageBoost * iValue ) / 100 );
			psPacket->iDamageState = ( psPacket->iDamageState & 0xFFFF0000 ) + 2;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
			psPacketEx->saStatus[ 0 ] = pcUserData->sCharacterData.iCritical;
		} break;

		case Skills::MultiSpark:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Multi Spark]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT Damage FROM MultiSpark WHERE SkillLevel=%d", iLevelSkill );
			int iStaticDamage = SQLSKILL->GetSkillValueInt( "SELECT StaticDamage FROM MultiSpark WHERE SkillLevel=%d", iLevelSkill );
			int iDamageBoost = psPacket->iDamage;
			int NSparks = SQLSKILL->GetSkillValueInt( "SELECT Damage FROM MultiSpark WHERE SkillLevel=%d", iLevelSkill );
			psPacket->iDamage = ( iStaticDamage + ( ( iDamageBoost * iValue ) / 100 ) ) * uUnkV;
			psPacket->iDamageState = ( psPacket->iDamageState & 0xFFFF0000 ) + 2;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
			psPacket->dwDamageChkSum = 0;
		} break;
		case Skills::SummonMuspell:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Summon Muspell]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT Damage FROM SummonMuspell WHERE SkillLevel=%d", pcUser->pData.iLevelSkill[ 16 - 1 ] );
			int iDamageBoost = psPacket->iDamage;
			psPacket->iDamage += ( ( iDamageBoost * iValue ) / 100 );
			psPacket->iDamageState = ( psPacket->iDamageState & 0xFFFF0000 ) + 2;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
			psPacket->dwDamageChkSum = 0;
		} break;

		//Magnetic Discharge
		case 0x8C:
			CMechanician::DoMagneticDischargeDamage( USERDATATOUSER( pcUserData ), pcUnitData, psPacket, psPacketEx );
			break;

		default:
			break;
	}
	return 1;
}

int CPriestess::RecvSkillAreaTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageRange * psPacketRange, PacketSkillDamageRangeEx * psPacketRangeEx )
{
	UINT uSkillID = psPacketRangeEx->iSkillID & 0x000000FF;
	UINT uUnkV = ( psPacketRangeEx->iSkillID / 4096 ) & 0x0000000F;
	int iLevelSkill = ( psPacketRangeEx->iSkillID / 256 ) & 0x0000000F;

	

	if ( iLevelSkill <= 0 || iLevelSkill > 10 )
		return 0;

	*( UINT* )0x7ACA048 = psPacketRangeEx->saStatus[ 0 ];
	int iValue = 0;

	switch ( uSkillID )
	{
		case Skills::DiviningLightning:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Divine Lightning]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM DivineLightning WHERE SkillLevel=%d", iLevelSkill );
		//  int iStaticDamage = SQLSKILL->GetSkillValueInt( "SELECT StaticDamage FROM DivineLightning WHERE SkillLevel=%d", iLevelSkill );
			int iDamageBoost = psPacketRange->iDamage;
			psPacketRange->iDamage += ( ( iDamageBoost * iValue ) / 100 );
			psPacketRange->iDamageState = 103;
			iValue = SQLSKILL->GetSkillValueInt( "SELECT Bolts FROM DivineLightning WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iCount = iValue;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> # Bolts = %d , Atk Rtg = %d", iValue, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
			psPacketRange->iCount &= 0x000000FF;
		}break;

		case Skills::ChainLightning:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Chain Lightning]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT LightningDamage FROM ChainLightning WHERE SkillLevel=%d", iLevelSkill );
			int iDamageBoost = psPacketRange->iDamage;
			psPacketRange->iDamage = ( iDamageBoost * iValue ) / 100;
			psPacketRange->iDamageState = 103;
			psPacketRange->iRange += 300;
			iValue = SQLSKILL->GetSkillValueInt( "SELECT Bolts FROM ChainLightning WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iCount = iValue;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> # Bolts[%d] Attack Rating[%d]", iValue, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
			*( UINT* )0x07ACA04C = -200;
			psPacketRange->iCount &= 0x000000FF;
		}break;

		case Skills::Extinction:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Extinction]" );
			psPacketRange->iDamage = iLevelSkill;
			psPacketRange->iDamageState = 6;
		} break;



		case Skills::GlacialSpike:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Glacial Spike]" );
			int iFreeze = SQLSKILL->GetSkillValueInt( "SELECT Duration FROM GlacialSpike WHERE SkillLevel=%d", iLevelSkill );
			int iRange = SQLSKILL->GetSkillValueInt( "SELECT Area FROM GlacialSpike WHERE SkillLevel=%d", iLevelSkill );
			int iDamageMin = SQLSKILL->GetSkillValueInt( "SELECT DamageMin FROM GlacialSpike WHERE SkillLevel=%d", iLevelSkill );
			int iDamageMax = SQLSKILL->GetSkillValueInt( "SELECT DamageMax FROM GlacialSpike WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iDamage = SQLSKILL->GetMinMaxInt( iDamageMin, iDamageMax, 0 );
			*( WORD* )0x08B80AF8 = ( WORD )iFreeze;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Damage [%d-%d][%d]", iDamageMin, iDamageMax, psPacketRange->iDamage );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Freeze Time [%d] Range [%d]", iFreeze, iRange );
			psPacketRange->iRange = iRange * 256;
			psPacketRange->iDamageState = 3;
			psPacketRange->iCount &= 0x000000FF;
		}
		break;

		case SKILLPLAYID_T52:
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Ice Meteorite]" );

			iValue = iaPrsT52DMGBoost[iLevelSkill - 1];
			psPacketRange->iDamage += ((psPacketRange->iDamage * iValue) / 100);

			iValue = iaPrsT52Area[iLevelSkill - 1] * iaPrsT52MeteoriteCount[iLevelSkill - 1];
			if ( iValue > 220 )
				iValue = 220;

			psPacketRange->iRange = iValue;

			psPacketRange->iDamageState = 0;

			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", psPacketRange->iRange, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );

			//SendIceMeteorite( pcUserData, psPacketRange->iRange, iaPrsT52MeteoriteCount[iLevelSkill - 1] );
			break;

		default:
			break;
	}
	return 1;
}

int CPriestess::RecvBuffSkill( UserData * pcUserData, UINT packet )
{
	UINT uSkillID = *( UINT* )( packet + 0x0C ) & 0x000000FF;
	int iLevelSkill = ( ( *( int* )( packet + 0x0C ) / 256 ) & 0x0000000F ) - 1;

	User * pcUser = USERDATATOUSER( pcUserData );

	if ( iLevelSkill < 0 || iLevelSkill > 10 )
	{
		iLevelSkill = *( int* )( packet + 0x08 ) - 1;
		if ( iLevelSkill < 0 || iLevelSkill > 10 )
			return 1;
	}
	iLevelSkill += 1;
	if ( iLevelSkill < 0 || iLevelSkill > 10 )
		return 1;

	int iValue = 0;

	switch ( uSkillID )
	{
		case Skills::HolyReflection:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Holy Reflection]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT Reflect FROM HolyReflection WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->iHolyReflectionDamage = iValue;
			iValue = SQLSKILL->GetSkillValueInt( "SELECT Duration FROM HolyReflection WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->dwHolyReflectionEndTime = ( iValue * 1000 ) + TICKCOUNT;
		}; break;

		case Skills::VirtualLife:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Virtual Life]" );
			UserData * pcUserDataTarget = USERSERVER->GetUserdata( *( UINT* )( packet + 0x10 ) );
			if ( pcUserDataTarget && !BATTLEROYALEHANDLER->IsPVPMap( pcUserDataTarget->iMapID ) )
			{
				*( UINT* )( packet + 8 ) = JOBCODEID_Priestess;
				SENDPACKET( USERDATATOUSER( pcUserDataTarget ), ( void* )packet );
			}
		}; break;

		case Skills::SummonMuspell:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Summon Muspell]" );
			int iDuration = SQLSKILL->GetSkillValueInt( "SELECT Duration FROM SummonMuspell WHERE SkillLevel=%d", pcUser->pData.iLevelSkill[ 16 - 1 ] );
			int iEvade = SQLSKILL->GetSkillValueInt( "SELECT AddedEvasion FROM SummonMuspell WHERE SkillLevel=%d", pcUser->pData.iLevelSkill[ 16 - 1 ] );
			pcUserData->SummonMuspellTime = ( iDuration * 1000 ) + TICKCOUNT;
			pcUserData->SummonMuspellParam = iEvade;
			pcUser->sSummonMuspellDamageBoost.sMin = iaSummonMuspellDamageBoostMin[iLevelSkill - 1];
			pcUser->sSummonMuspellDamageBoost.sMax = iaSummonMuspellDamageBoostMax[iLevelSkill - 1];
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Evade: %d   Duration: %d - %d", iEvade, iDuration, pcUser->pData.iLevelSkill[ 16 - 1 ] );
		}; break;

		case Skills::Resurrection:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Resurrection]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT SuccessRate FROM Resurrection WHERE SkillLevel=%d", iLevelSkill );
			srand( ( UINT )time( NULL ) );
			int PerSuc = ( rand() % 99 ) + 1;
			if ( PerSuc <= iValue )
			{
				UserData * pcUserDataDead = USERSERVER->GetUserdata( *( UINT* )( packet + 0x10 ) );
				if ( pcUserDataDead )
				{
					if ( pcUserDataDead->iMapID == MAPID_Bellatra )
						break;

					*( UINT* )( packet + 8 ) = 8;
					CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Target '%s' Ressurrected!", pcUserDataDead->sCharacterData.szName );
					SENDPACKET( USERDATATOUSER( pcUserDataDead ), ( void* )packet );
				}

			}
		}; break;

		case Skills::IceElemental:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Ice Elemental]" );

			if ( MAPSERVER->IsSummonMap( pcUser->GetMapID() ) == FALSE )
			{
				CHATSERVER->SendChatMessageBox( pcUser, "You can't summon a pet on this map" );
				break;
			}

			WRITEDWORD( 0x08B81830, TRUE );

			UnitData * pcUnitData = MAPSERVER->SpawnMonster( "Ice Elemental", pcUser->GetMapID(), Point3D( pcUserData->sPosition.iX, pcUserData->sPosition.iY, pcUserData->sPosition.iZ ), pcUser );

			if ( pcUnitData )
			{
				pcUnitData->sUnitInfo.dwHashCharacterName	= 0;
				pcUnitData->sCharacterData.iMonsterType		= MONSTERTYPE_Friendly;
				pcUnitData->sCharacterData.iOwnerID			= pcUserData->iID;

				pcUnitData->sCharacterData.sHP.sCur = (short)iaIceElementalHP[iLevelSkill - 1];
				pcUnitData->sCharacterData.sHP.sMax = (short)iaIceElementalHP[iLevelSkill - 1];
				pcUnitData->sCharacterData.iMinDamage = iaIceElementalAttackPower[iLevelSkill - 1];
				pcUnitData->sCharacterData.iMaxDamage = iaIceElementalAttackPower[iLevelSkill - 1];
				pcUnitData->sCharacterData.iAttackRating = iaIceElementalAttackRating[iLevelSkill - 1];

				pcUnitData->pcOwner				= pcUserData;
				pcUnitData->dwLastUpdateTime	= TICKCOUNT + (300 * 1000);

				pcUnitData->sCharacterData.sSize = 224 + iLevelSkill * 15;

				pcUnitData->saMoveStepCount[0] = 1;
				pcUnitData->saMoveStepCount[1] = 2;

				pcUserData->pcPetData		= pcUnitData;
				pcUserData->dwObjectPetID	= pcUnitData->iID;
				pcUnitData->bAggressive		= FALSE;
				pcUnitData->iPetID			= SKILLID_IceElemental;

				ITEMSERVER->OnUseCrystal( pcUserData, pcUnitData->iID, SKILLID_IceElemental, pcUnitData->dwLastUpdateTime );

				//User Warp Effect summon crystal
				PacketUserWarpEffect sPacketEffect;
				ZeroMemory( &sPacketEffect, sizeof( PacketUserWarpEffect ) );
				sPacketEffect.iID = 0x100;
				sPacketEffect.iX = pcUserData->sPosition.iX;
				sPacketEffect.iY = pcUserData->sPosition.iY;
				sPacketEffect.iZ = pcUserData->sPosition.iZ;
				CALL_WITH_ARG2( 0x00552130, (DWORD)pcUserData, (DWORD)&sPacketEffect );

				return TRUE;
			}
		}
		break;

		default:
			break;
	}
	return 1;
}

BOOL CPriestess::RestoreHPPacketParty( UserData * pcUserData, PacketRestoreHP * psPacket )
{
	PacketRestoreHP sPacket;
	CopyMemory( &sPacket, psPacket, sizeof(PacketRestoreHP) );

	User * pcUser = USERDATATOUSER( pcUserData );
	if( pcUser )
	{
		if( pcUser->bParty && pcUser->psParty )
		{
			std::vector<User*> vUsers = PARTYHANDLER->GetPartyMembers( pcUser->psParty );
			for( auto &p : vUsers )
			{
				if( p && p != pcUser )
				{
					if( pcUserData->dwVirtualLifeEndTime && pcUserData->dwVirtualLifeEndTime > TICKCOUNT )
					{
						sPacket.hpboost = psPacket->hpboost;
						sPacket.hpboost -= ( psPacket->hpboost * pcUserData->iVirtualLifeData ) / 100;
						SENDPACKET( p, &sPacket );
					}
					else
						SENDPACKET( p, psPacket );
				}
			}

			if ( pcUserData->dwVirtualLifeEndTime && pcUserData->dwVirtualLifeEndTime > TICKCOUNT )
			{
				sPacket.hpboost = psPacket->hpboost;
				sPacket.hpboost -= (psPacket->hpboost * pcUserData->iVirtualLifeData) / 100;
				SENDPACKET( pcUser, &sPacket );
			}
			else
				SENDPACKET( pcUser, psPacket );
		}
	}

	return TRUE;
}


CPriestess::~CPriestess()
{
}
