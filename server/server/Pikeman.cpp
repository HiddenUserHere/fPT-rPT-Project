#include "stdafx.h"
#include "Pikeman.h"
#include "HNSSkill.h"


CPikeman::CPikeman()
{
}

int CPikeman::RecvSkillSingleTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageNormalEx * psPacketEx )
{
	UINT uSkillID = psPacketEx->iSkillID & 0x000000FF;
	UINT uUnkV = ( psPacketEx->iSkillID / 4096 ) & 0x0000000F;
	int iLevelSkill = ( psPacketEx->iSkillID / 256 ) & 0x0000000F;

	*( UINT* )0x7ACA048 = psPacketEx->iSkillID;

	if ( uSkillID != 0 && iLevelSkill <= 0 || iLevelSkill > 10 )
		return 0;

	int iValue = 0;

	switch ( uSkillID )
	{
		case Skills::DefaultAttack:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Default Attack]" );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical [%d] AttackRating [%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
		} break;

		case Skills::CriticalHit:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Critical Hit]" );
			int iCritical = SQLSKILL->GetSkillValueInt( "SELECT CriticalDamage FROM CriticalHit WHERE SkillLevel=%d", iLevelSkill );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical Damage [+%d%%]", iCritical );
			*( UINT* )0x08B80ACC = iCritical;
		} break;

		case Skills::JumpingCrash:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Jumping Crash]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM JumpingCrash WHERE SkillLevel=%d", iLevelSkill );
			if ( pcUnitData )
			{
				if ( pcUnitData->sCharacterData.iMonsterType == 0x92 )
				{
					iValue += 30;
					CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> +30%% Demon Monster" );
				}
			}
			int iDamageBoost = ( psPacket->iDamage * iValue ) / 100;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical [%d] AttackRating [%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage [%d + %d] (%d)", psPacket->iDamage, iDamageBoost, ( iDamageBoost + psPacket->iDamage ) );
			psPacket->iDamage += iDamageBoost;
		} break;

		case Skills::ChainLance:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Chain Lance]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM ChainLance WHERE SkillLevel=%d", iLevelSkill );
			int iDamageBoost = ( psPacket->iDamage * iValue ) / 100;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical [%d] AttackRating [%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage [%d + %d] (%d)", psPacket->iDamage, iDamageBoost, ( iDamageBoost + psPacket->iDamage ) );
			psPacket->iDamage += iDamageBoost;
		} break;

		case Skills::ShadowMaster:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Shadow Master]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM ShadowMaster WHERE SkillLevel=%d", iLevelSkill );
			int iDamageBoost = ( psPacket->iDamage * iValue ) / 100;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical [%d] AttackRating [%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage [%d + %d] (%d)", psPacket->iDamage, iDamageBoost, ( iDamageBoost + psPacket->iDamage ) );
			psPacket->iDamage += iDamageBoost;
		} break;

		case Skills::ChargingStrike:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Charging Strike]" );
			int iValue = 0;
			if ( psPacketEx->WeaponAttackPower[0] == 0 )
			{
				iValue = SQLSKILL->GetSkillValueInt( "SELECT ChargedDamage FROM ChargingStrike WHERE SkillLevel=%d", iLevelSkill );
				CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Charged Skill" );
			}
			else
				iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM ChargingStrike WHERE SkillLevel=%d", iLevelSkill );
			int iDamageBoost = ( psPacket->iDamage * iValue ) / 100;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical [%d] AttackRating [%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage [%d + %d] (%d)", psPacket->iDamage, iDamageBoost, ( iDamageBoost + psPacket->iDamage ) );
			psPacket->iDamage += iDamageBoost;
		} break;

		case Skills::AssasinsEye:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Assasin's Eye]" );
			int iCriticalDamage = SQLSKILL->GetSkillValueInt( "SELECT CriticalDamage FROM AssasinsEye WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->AssassinEyeParam = iCriticalDamage;

			UINT uTime = SQLSKILL->GetSkillValueInt( "SELECT Duration FROM AssasinsEye WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->dwAssassinEyeEndTime = ( uTime * 1000 ) + TICKCOUNT;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical [+%d%%] Duration [%d]", iCriticalDamage, uTime );
			if ( pcUnitData )
			{
				pcUnitData->dwAssassinEyeEndTime = ( uTime * 1000 ) + TICKCOUNT;
				int iCritMonster = SQLSKILL->GetSkillValueInt( "SELECT CriticalMonster FROM AssasinsEye WHERE SkillLevel=%d", iLevelSkill );
				pcUnitData->iAssassinEyeValue = iCritMonster;
				CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> %s Target", pcUnitData->sCharacterData.szName );
				CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical [%d] Duration [%d]", iCritMonster, uTime );
			}
			return 0;
		} break;

		case Skills::RingOfSpears:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [RingOfSpears]" );
			iValue = iaPikeT51Duration[iLevelSkill - 1];
			if ( pcUnitData )
			{
				if ( (pcUnitData->sCharacterData.sHP.sCur > 0) && (pcUnitData->pcKillerData == NULL) && (pcUnitData->GetAnimation() != ANIMATIONTYPE_Die) )
				{
					UNITSERVER->SetMotionFromCode( pcUnitData, 0x110 );
					int iResistance = pcUnitData->sCharacterData.sElementalDef[0];
					if ( iResistance < 0 )
						iResistance = 0;
					if ( iResistance >= 100 )
						iResistance = 100;

					iResistance = 100 - iResistance;
					pcUnitData->iStunTimeLeft = iValue << 4;
					pcUnitData->iStunTimeLeft = (pcUnitData->iStunTimeLeft * iResistance) / 100;
					psPacket->iDamage = 0;
					psPacket->iRange = 0;
					return 0;
				}
			}
			else
			{
				UserData * ud = USERDATABYID( psPacketEx->iID );

				if ( ud )
				{
					int iResistance = ud->sCharacterData.sElementalDef[0];
					if ( iResistance < 0 )
						iResistance = 0;
					if ( iResistance >= 100 )
						iResistance = 100;

					iResistance = 100 - iResistance;

					int iStun = ((iValue * 70) * iResistance) / 100;

					DAMAGEHANDLER->SendDamagePvPData( pcUserData, ud, 1, 0x80, -0x5A, iStun );

					psPacket->iDamage = 0;
					psPacket->iRange = 0;
					return 0;
				}
			}
		} break;

		case Skills::LastBreath:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Last Breath]" );
			iValue = iaPikeT52DamageBoost[iLevelSkill - 1];
			int iDamageBoost = (psPacket->iDamage * iValue) / 100;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical [%d] AttackRating [%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage [%d + %d] (%d)", psPacket->iDamage, iDamageBoost, (iDamageBoost + psPacket->iDamage) );
			psPacket->iDamage += iDamageBoost;
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

int CPikeman::RecvSkillAreaTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageRange * psPacketRange, PacketSkillDamageRangeEx * psPacketRangeEx )
{
	UINT uSkillID = psPacketRangeEx->iSkillID & 0x000000FF;
	UINT uUnkV = ( psPacketRangeEx->iSkillID / 4096 ) & 0x0000000F;
	int iLevelSkill = ( psPacketRangeEx->iSkillID / 256 ) & 0x0000000F;


	


	if ( uSkillID != 0 && iLevelSkill <= 0 || iLevelSkill > 10 )
		return 0;

	*( UINT* )0x7ACA048 = psPacketRangeEx->saStatus[0];

	int iValue = 0;

	switch ( uSkillID )
	{

		case Skills::PikeWind:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Pike Wind]" );
			int iDamageMin = SQLSKILL->GetSkillValueInt( "SELECT DamageMin FROM PikeWind WHERE SkillLevel=%d", iLevelSkill );
			int iDamageMax = SQLSKILL->GetSkillValueInt( "SELECT DamageMax FROM PikeWind WHERE SkillLevel=%d", iLevelSkill );
			int iRange = SQLSKILL->GetSkillValueInt( "SELECT Range FROM PikeWind WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iDamage = SQLSKILL->GetMinMaxInt( iDamageMin, iDamageMax, 0 );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Range [%d] Damage [%d-%d][%d]", iRange, iDamageMin, iDamageMax, psPacketRange->iDamage );
			psPacketRange->iRange = iRange;
			psPacketRange->iDamageState = 1;
		} break;

		case Skills::GroundPike:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Ground Pike]" );
			int iDamageMin = SQLSKILL->GetSkillValueInt( "SELECT DamageMin FROM GroundPike WHERE SkillLevel=%d", iLevelSkill );
			int iDamageMax = SQLSKILL->GetSkillValueInt( "SELECT DamageMax FROM GroundPike WHERE SkillLevel=%d", iLevelSkill );
			int iFreeze = SQLSKILL->GetSkillValueInt( "SELECT FreezeTime FROM GroundPike WHERE SkillLevel=%d", iLevelSkill );
			int iRange = SQLSKILL->GetSkillValueInt( "SELECT Range FROM GroundPike WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iDamage = SQLSKILL->GetMinMaxInt( iDamageMin, iDamageMax, 0 );
			*( WORD* )0x08B80AF2 = ( WORD )iFreeze;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Damage [%d-%d][%d]", iDamageMin, iDamageMax, psPacketRange->iDamage );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Freeze Time [%d] Range [%d]", iFreeze, iRange );
			psPacketRange->iRange = iRange * 256;
			psPacketRange->iDamageState = 3;
		} break;

		case Skills::VenomSpear:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Venom Spear]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM VenomSpear WHERE SkillLevel=%d", iLevelSkill );
			int iDuration = SQLSKILL->GetSkillValueInt( "SELECT Duration FROM VenomSpear WHERE SkillLevel=%d", iLevelSkill );
			int iSpears = SQLSKILL->GetSkillValueInt( "SELECT NumberofSpears FROM VenomSpear WHERE SkillLevel=%d", iLevelSkill );
			*( WORD* )0x08B80AF6 = ( WORD )iDuration * 17;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Damage [%d + %d][%d] Duration [%d]", psPacketRange->iDamage, ( ( psPacketRange->iDamage * iValue ) / 100 ),
									   psPacketRange->iDamage + ( ( psPacketRange->iDamage * iValue ) / 100 ), iDuration );

			psPacketRange->iDamage += ( ( psPacketRange->iDamage * iValue ) / 100 );
			psPacketRange->iCount = iSpears;
			*( WORD* )0x08B80AF4 = ( WORD )psPacketRange->iDamage;
			psPacketRange->iRange = iaSkillVenomSpear[iLevelSkill - 1];
			psPacketRange->iDamageState = 0x6A;
			psPacketRange->iCount &= 0x000000FF;
		} break;

		case Skills::Tornado:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Tornado]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM Tornado WHERE SkillLevel=%d", iLevelSkill );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Damage [%d + %d][%d]", psPacketRange->iDamage, ( ( psPacketRange->iDamage * iValue ) / 100 ),
									   psPacketRange->iDamage + ( ( psPacketRange->iDamage * iValue ) / 100 ) );
			psPacketRange->iDamage += ( ( psPacketRange->iDamage * iValue ) / 100 );
			psPacketRange->iRange = 120; //iaSkillTornadoRange[iLevelSkill - 1];
			//   CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Range [%d]", iRange );
			psPacketRange->iDamageState = 0;
		} break;

		case Skills::Expansion:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Expansion]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM Expansion WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iRange = 70;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Damage [%d + %d][%d] Range [%d]", psPacketRange->iDamage, ( ( psPacketRange->iDamage * iValue ) / 100 ),
									   psPacketRange->iDamage + ( ( psPacketRange->iDamage * iValue ) / 100 ), 180 );

			psPacketRange->iDamage += ( ( psPacketRange->iDamage * iValue ) / 100 );
			psPacketRange->iDamageState = 0;
			psPacketRange->iCount &= 0x000000FF;

			if ( pcUnitData )
			{
				if ( *( int* )( (DWORD)psPacketRangeEx + 0x5C ) > 0 )
				{
					int iMonsters = ( (DWORD)psPacketRangeEx + 0x60 );
					int iCount = *( int* )( (DWORD)psPacketRangeEx + 0x5C );
					for ( int i = 0; i < iCount; i++ )
					{
						if ( *( int* )iMonsters == *( UINT* )( (DWORD)pcUnitData + 0x10 ) )
						{
							psPacket->iDamage = 0;
							break;
						}
						iMonsters += 4;
					}
				}
			}

		} break;

		default:
			break;
	}

	return 1;
}

int CPikeman::RecvBuffSkill( UserData * pcUserData, UINT packet )
{
	UINT uSkillID = *( UINT* )( packet + 0x0C ) & 0x000000FF;
	int iLevelSkill = ( ( *( int* )( packet + 0x0C ) / 256 ) & 0x0000000F ) - 1;

	

	if ( iLevelSkill < 0 || iLevelSkill > 10 )
	{
		iLevelSkill = *( int* )( packet + 0x08 ) - 1;
		if ( iLevelSkill < 0 || iLevelSkill > 10 )
			return 1;
	}
	iLevelSkill += 1;

	switch ( uSkillID )
	{

		case Skills::Vanish:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Vanish]" );
			int iDamageBoost = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM Vanish WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->saVanishParam[ 0 ] = ( WORD )iDamageBoost;

			int iVisibility = SQLSKILL->GetSkillValueInt( "SELECT Visibility FROM Vanish WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->saVanishParam[ 1 ] = ( WORD )iVisibility;

			UINT uTime = SQLSKILL->GetSkillValueInt( "SELECT Duration FROM Vanish WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->dwVanishEndTime = ( uTime * 1000 ) + TICKCOUNT;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Damage Boost [+%d%%] Visibility [%d]", iDamageBoost, iVisibility );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Duration [%d]", uTime );
		} break;

		case Skills::AssasinsEye:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Assasin's Eye]" );
			int iCriticalDamage = SQLSKILL->GetSkillValueInt( "SELECT CriticalDamage FROM AssasinsEye WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->AssassinEyeParam = iCriticalDamage;

			UINT uTime = SQLSKILL->GetSkillValueInt( "SELECT Duration FROM AssasinsEye WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->dwAssassinEyeEndTime = ( uTime * 1000 ) + TICKCOUNT;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical [+%d%%] Duration [%d]", iCriticalDamage, uTime );
		} break;

		case Skills::Vague:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Vague]" );
			int iEvasion = SQLSKILL->GetSkillValueInt( "SELECT Evasion FROM Vague WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->sVagueEvade = (short)iEvasion;
			pcUserData->sVagueLevel = iLevelSkill;

			UINT uTime = SQLSKILL->GetSkillValueInt( "SELECT Duration FROM Vague WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->dwVagueEndTime = ( uTime * 1000 ) + TICKCOUNT;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Evasion [%d%%] Duration [%d]", iEvasion, uTime );
		} break;

		default:
			break;
	}
	return 1;
}


CPikeman::~CPikeman()
{
}
