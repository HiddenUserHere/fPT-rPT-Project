#include "stdafx.h"
#include "Mechanician.h"
#include "Assassin.h"


CAssassin::CAssassin()
{
}


CAssassin::~CAssassin()
{
}


int CAssassin::RecvSkillSingleTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageNormalEx * psPacketEx )
{
	UINT uSkillID = psPacketEx->iSkillID & 0x000000FF;
	UINT uUnkV = ( psPacketEx->iSkillID / 4096 ) & 0x0000000F;
	int iLevelSkill = ( psPacketEx->iSkillID / 256 ) & 0x0000000F;

	

	if ( uSkillID != 0 && iLevelSkill <= 0 || iLevelSkill > 10 )
		return 0;

	*( UINT* )0x7ACA048 = uSkillID;

	int iValue = 0;

	User * pcUser = USERDATATOUSER( pcUserData );

	switch ( uSkillID )
	{
		case Skills::DefaultAttack:
		{
			int iDamageBoost = SQLSKILL->GetMinMaxInt( pcUserData->sCharacterData.iMinDamage, pcUserData->sCharacterData.iMaxDamage, 0 );
			psPacket->iDamage = psPacket->iDamage;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Default Attack]" );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
		} break;

		case Skills::Stinger:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Stinger]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM Stinger WHERE SkillLevel=%d", iLevelSkill );
			psPacket->iDamage += ( ( psPacket->iDamage * iValue ) / 100 );

			if ( pcUnitData )
			{
				if ( pcUnitData->sCharacterData.iMonsterType == MONSTERTYPE_Mutant )
				{
					psPacket->iDamage += ( ( psPacket->iDamage * 30 ) / 100 );
					CHATSERVER->SendChat( pcUser, CHATCOLOR_Error, "> +30%% Mutant" );
				}
				else if ( pcUnitData->sCharacterData.iMonsterType == MONSTERTYPE_Normal )
				{
					psPacket->iDamage += ( ( psPacket->iDamage * 30 ) / 100 );
					CHATSERVER->SendChat( pcUser, CHATCOLOR_Error, "> +30%% Normal" );
				}
			}

			psPacket->iDamageState = ( psPacket->iDamageState & 0xFFFF0000 ) + 2;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
		} break;

		case Skills::RunningHit:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Running Hit]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM RunningHit WHERE SkillLevel=%d", iLevelSkill );
			psPacket->iDamage += ( ( psPacket->iDamage * iValue ) / 100 );
			psPacket->iDamageState = ( psPacket->iDamageState & 0xFFFF0000 ) + 2;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
		} break;

		case Skills::VenomThorn:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Venom Thorn]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM VenomThorn WHERE SkillLevel=%d", iLevelSkill );
			psPacket->iDamage += ( ( psPacket->iDamage * iValue ) / 100 );
			psPacket->iDamageState = ( psPacket->iDamageState & 0xFFFF0000 ) + 2;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
		} break;

		case Skills::SoreBlade:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Sore Blade]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM SoreBlade WHERE SkillLevel=%d", iLevelSkill );
			psPacket->iDamage += ( ( psPacket->iDamage * iValue ) / 100 );
			psPacket->iDamageState = ( psPacket->iDamageState & 0xFFFF0000 ) + 2;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
		} break;

		case Skills::GustSlash:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Gust Slash]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM GustSlash WHERE SkillLevel=%d", iLevelSkill );
			psPacket->iDamage += ( ( psPacket->iDamage * iValue ) / 100 );
			psPacket->iDamageState = ( psPacket->iDamageState & 0xFFFF0000 ) + 2;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
		} break;

		case Skills::FrostWind:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Frost Wind]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM FrostWind WHERE SkillLevel=%d", iLevelSkill );
			psPacket->iDamage += ( ( psPacket->iDamage * iValue ) / 100 );
			psPacket->iDamageState = ( psPacket->iDamageState & 0xFFFF0000 ) + 2;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
		} break;

		case Skills::NinjaShadow:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Ninja Shadow]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM NinjaShadow WHERE SkillLevel=%d", iLevelSkill );
			psPacket->iDamage += ( ( psPacket->iDamage * iValue ) / 100 );
			psPacket->iDamageState = ( psPacket->iDamageState & 0xFFFF0000 ) + 2;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
		} break;
		case Skills::RisingSlash:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Rising Slash]" );
			iValue = iaDMGBoostASST5S2[iLevelSkill - 1];
			psPacket->iDamage += ( ( psPacket->iDamage * iValue ) / 100 );
			psPacket->iDamageState = ( psPacket->iDamageState & 0xFFFF0000 ) + 2;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
		} break;

		case Skills::ShadowBlink:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Shadow Blink]" );
			*(UINT*)0x08B80ACC = iaASNT51CritBoost[iLevelSkill - 1];
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
		} break;



		case Skills::ViolentStub:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Violent Stub]" );
			iValue = iaDMGBoostASST5S3[iLevelSkill - 1];
			psPacket->iDamage += ( ( psPacket->iDamage * iValue ) / 100 );
			psPacket->iDamageState = ( psPacket->iDamageState & 0xFFFF0000 ) + 2;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
		} break;
		case Skills::ShadowStorm:
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Shadow Storm]" );
			iValue = iaDMGBoostASST5S4[iLevelSkill - 1];
			psPacket->iDamage += ( ( psPacket->iDamage * iValue ) / 100 );
			psPacket->iDamageState = ( psPacket->iDamageState & 0xFFFF0000 ) + 2;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
			break;

		//Magnetic Discharge
		case 0x8C:
			CMechanician::DoMagneticDischargeDamage( USERDATATOUSER( pcUserData ), pcUnitData, psPacket, psPacketEx );
			break;

	}
	return 1;
}

int CAssassin::RecvSkillAreaTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageRange * psPacketRange, PacketSkillDamageRangeEx * psPacketRangeEx )
{
	UINT uSkillID = psPacketRangeEx->iSkillID & 0x000000FF;
	UINT uUnkV = ( psPacketRangeEx->iSkillID / 4096 ) & 0x0000000F;
	int iLevelSkill = ( psPacketRangeEx->iSkillID / 256 ) & 0x0000000F;

	

	if ( iLevelSkill <= 0 || iLevelSkill > 10 )
		return 0;

	*(UINT*)0x7ACA048 = psPacketRangeEx->saStatus[0];

	int iValue = 0;

	switch ( uSkillID )
	{
		case Skills::GustSlash:
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Gust Slash]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM GustSlash WHERE SkillLevel=%d", iLevelSkill );
			int iDamageBoost;
			iDamageBoost = psPacketRange->iDamage;
			psPacketRange->iDamage = iDamageBoost + ( ( iDamageBoost * iValue ) / 100 );
			psPacketRange->iDamageState = 100;
			psPacketRange->iRange = 100;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketRangeEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
			break;

		case Skills::SoulShock:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Soul Shock]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT Range FROM SoulShock WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iRange = iValue;
			iValue = SQLSKILL->GetSkillValueInt( "SELECT StunTime FROM SoulShock WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iDamageState = 2;
			psPacketRange->iDamage = iValue;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Range = %d , Time = %d", psPacketRange->iRange, psPacketRange->iDamage );
		} break;

		case Skills::Polluted:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Polluted]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM Polluted WHERE SkillLevel=%d", iLevelSkill );
			int iDuration = SQLSKILL->GetSkillValueInt( "SELECT Duration FROM Polluted WHERE SkillLevel=%d", iLevelSkill );
			int iSpears = SQLSKILL->GetSkillValueInt( "SELECT NumberofSpears FROM Polluted WHERE SkillLevel=%d", iLevelSkill );
			*( WORD* )0x08B80AF6 = ( WORD )iDuration * 17;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Damage [%d + %d][%d] Duration [%d]", psPacketRange->iDamage, ( ( psPacketRange->iDamage * iValue ) / 100 ),
									   psPacketRange->iDamage + ( ( psPacketRange->iDamage * iValue ) / 100 ), iDuration );

			psPacketRange->iDamage += ( ( psPacketRange->iDamage * iValue ) / 100 );
			psPacketRange->iCount = iSpears;
			*( WORD* )0x08B80AF4 = ( WORD )psPacketRange->iDamage;
			psPacketRange->iRange = psPacketRange->iRange;
			psPacketRange->iDamageState = 0x6A;
			psPacketRange->iCount &= 0x000000FF;
		} break;

	}
	return 1;
}

int CAssassin::RecvBuffSkill( UserData * pcUserData, UINT packet )
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

	int iValue = 0;

	switch ( uSkillID )
	{
		case Skills::Blind:
		{
			User * pcUser;
			pcUser = USERDATATOUSER( pcUserData );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Blind]" );
			int iDamageBoost = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM Blind WHERE SkillLevel=%d", iLevelSkill );
			pcUser->dwBlindDamage = ( DWORD )iDamageBoost;

			int iVisibility = SQLSKILL->GetSkillValueInt( "SELECT Visibility FROM Blind WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->saVanishParam[ 1 ]	= ( WORD )iVisibility;
			pcUserData->saVanishParam[0]		= 0;


			UINT uTime = SQLSKILL->GetSkillValueInt( "SELECT Duration FROM Blind WHERE SkillLevel=%d", iLevelSkill );
			pcUser->dwBlindTime = ( uTime * 1000 ) + TICKCOUNT;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Damage Boost [+%d%%] Visibility [%d]", iDamageBoost, iVisibility );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Duration [%d]", uTime );
		} break;

	}
	return TRUE;
}

void CAssassin::OnAddFinalDamage( UserData * pcUserData, PacketSkillDamageNormal * psPacket )
{
	if ( pcUserData )
	{
		User * pcUser = USERDATATOUSER( pcUserData );
		if ( pcUser->dwBlindTime > TICKCOUNT )
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Blind Damage [%d + %d]", psPacket->iDamage, (psPacket->iDamage * pcUser->dwBlindDamage) / 100 );
			psPacket->iDamage += (psPacket->iDamage * pcUser->dwBlindDamage) / 100;
		}
	}

	UINT uTime = 0;

	//Blind
	User * pcUser = USERDATATOUSER( pcUserData );
	if ( pcUser )
	{
		uTime = pcUser->dwBlindTime;
		if ( uTime > TICKCOUNT )
		{
			pcUser->dwBlindTime = 0;
			pcUser->dwBlindDamage = 0;
			pcUserData->saVanishParam[ 1 ] = 0;
			DAMAGEHANDLER->SendCancelSkill( pcUser, SKILLPLAYID_T34 );
		}
	}
}

BOOL CAssassin::AddAlas( User * pcUser, User * pcTarget, PacketSkillPartyData * packet )
{
	int iLevelSkill = packet->iLevel;

	CHATSERVER->SendDebugChat( pcUser->pcUserData, CHATCOLOR_Error, "%d Effect", packet->iSkillID );

	int iValue = 0;

	if ( packet->iSkillID == Skills::Alas )
	{
		CHATSERVER->SendDebugChat( pcTarget->pcUserData, CHATCOLOR_Error, "> [Alas]" );
		int iDuration = SQLSKILL->GetSkillValueInt( "SELECT Duration FROM Alas WHERE SkillLevel=%d", iLevelSkill );
		pcTarget->dwAlasTime = ( iDuration * 1000 ) + TICKCOUNT;
		iValue = SQLSKILL->GetSkillValueInt( "SELECT Evasion FROM Alas WHERE SkillLevel=%d", iLevelSkill );
		pcTarget->dwAlasEvasion = iValue;

		if ( pcTarget != pcUser )
			pcTarget->dwAlasEvasion >>= 1;

		packet->iClass = 9;

		// Disable Skills
		return TRUE;
	}
	return FALSE;
}

BOOL CAssassin::AddKissViper( User * pcUser, User * pcTarget, PacketSkillPartyData * psPacket )
{
	if ( psPacket->iSkillID == SKILLPLAYID_T52 )
	{
		CHATSERVER->SendDebugChat( pcTarget->pcUserData, CHATCOLOR_Error, "> [Kiss Viper]" );
		pcTarget->dwKissViperTimeOut = TICKCOUNT + (300 * 1000);
		pcTarget->uKissViperLevel = psPacket->iLevel;
		pcTarget->uKissViperCriticalAdd = iaASNT52CriticalAdd[psPacket->iLevel - 1];

		if ( pcTarget != pcUser )
			pcTarget->uKissViperCriticalAdd >>= 1;

		psPacket->iClass = CHARACTERCLASS_Assassin;

		// Disable Skills
		return TRUE;
	}

	return FALSE;
}

BOOL CAssassin::OnAlasEvade( UserData * pcUserData, DWORD dwObjectID )
{
	User * u = USERDATATOUSER( pcUserData );

	UserData * pcUserEnemyData = NULL;
	UnitData * pcUnitEnemyData = NULL;

	pcUserEnemyData = USERDATABYID( dwObjectID );

	char * pszName = NULL;

	if ( !pcUserEnemyData )
	{
		pcUnitEnemyData = UNITSERVER->GetUnit( dwObjectID ); 
		pszName = pcUnitEnemyData->sCharacterData.szName;
	}
	else
		pszName = CHARACTERSERVER->GetCharacterName( pcUserEnemyData );

	if ( u && ( pcUnitEnemyData || pcUserEnemyData ) )
	{
		if ( u->dwAlasTime > TICKCOUNT )
		{
			if ( (rand() % 100) < (int)u->dwAlasEvasion )
			{
				CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> %s Damage Evaded( Alas[%d] )", pszName, u->dwAlasEvasion );
				PacketEvasion sPacket;
				sPacket.iLength		= sizeof( PacketEvasion );
				sPacket.iHeader		= PKTHDR_EvasionSuccess;
				sPacket.iUnk		= 0;
				sPacket.iSkillID	= Skills::Alas;
				sPacket.dwObjectID	= dwObjectID;
				SENDPACKET( u, &sPacket );
				return TRUE;
			}
		}
		else
		{
			u->dwAlasEvasion	= 0;
			u->dwAlasTime		= 0;
		}
	}
	return FALSE;
}

void CAssassin::OnUseWisp( User * pcUser, PacketWisp * psPacket )
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
		
		if ( iTime > 0 )
		{
			User * u = USERDATATOUSER( pcUserDataTarget );
			if ( u )
			{
				u->dwWispTime	= (iTime * 1000) + TICKCOUNT;
				u->iWispAbsorb	= iAbs;
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
