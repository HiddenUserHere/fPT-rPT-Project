#include "stdafx.h"
#include "HNSSkill.h"



typedef void( __cdecl *t_ForeOrbAdd ) ( UserData * pcUserData, int v, UINT SkillArray, UINT packet34, UINT packet28, UINT packet2A );
t_ForeOrbAdd  ForceOrbAdd = ( t_ForeOrbAdd )0x00577F10;

typedef void( __cdecl *t_SendSkillEffect ) ( UserData * pcUserData, UserData * pcUserDataTarget, UINT skCode, UINT skLevel );
t_SendSkillEffect SendPacketSkillEffect = ( t_SendSkillEffect )0x0045142C;

typedef UINT( __cdecl *t_FindByUniqueID ) ( UINT id );
t_FindByUniqueID FindPlayerByUniqueID = ( t_FindByUniqueID )0x0054CC10;
t_FindByUniqueID FindMonsterByUniqueID = ( t_FindByUniqueID )0x0054CC60;


CFighter										* pcFighter = NULL;
CMechanician									* pcMechanician = NULL;
CArcher											* pcArcher = NULL;
CPikeman										* pcPikeman = NULL;
CAtalanta										* pcAtalanta = NULL;
CKnight											* pcKnight = NULL;
CMagician										* pcMagician = NULL;
CPriestess										* pcPriestess = NULL;
CAssassin										* pcAssassin = NULL;
CShaman											* pcShaman = NULL;


void InitSkills()
{
	if ( !pcFighter )
	{
		pcFighter = new CFighter;
		pcMechanician = new CMechanician;
		pcArcher = new CArcher;
		pcPikeman = new CPikeman;
		pcAtalanta = new CAtalanta;
		pcKnight = new CKnight;
		pcMagician = new CMagician;
		pcPriestess = new CPriestess; 
		pcAssassin = new CAssassin;
		pcShaman = new CShaman;
	}
}

void __cdecl AddFinalDamage( UserData * pcUserData, void * pPacket )
{
	PacketSkillDamageNormal * psPacket = (PacketSkillDamageNormal*)pPacket;

	ASSASSINSKILL->OnAddFinalDamage( pcUserData, psPacket );
	ARCHERSKILL->OnAddFinalDamage( pcUserData, psPacket );

	if ( pcUserData->saBCBuff[0].dwItemEndTime > TICKCOUNT )
	{
		if ( pcUserData->saBCBuff[0].iItemID == ITEMID_FatalEdge )
		{
			if ( pcUserData->iMapID == MAPID_Bellatra )
			{
				int iAdd = (psPacket->iDamage * pcUserData->saBCBuff[0].iValueData) / 100;

				CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Fatal Edge[%d + %d[%d%%]] = %d", psPacket->iDamage, iAdd, pcUserData->saBCBuff[0].iValueData, psPacket->iDamage + iAdd );

				psPacket->iDamage += iAdd;
			}
		}
	}
}

int AddForceOrb( UserData * pcUserData, int TypeMath )
{
	UINT uItemID = pcUserData->ForceOrbCode;
	int iTime = (int)pcUserData->ForceOrbTime;
	UINT uDamage = pcUserData->ForceOrbDamage;

	if ( iTime > 0 )
	{
		if ( ( uItemID >= ITEMID_LucidyForce ) && ( uItemID <= ITEMID_SolForce ) )
		{
			int iForceT = ((uItemID & 0x0000FFFF) / 256) - 1;
			if ( TypeMath == 1 )
			{
				return ForceDamageTable[ iForceT ];
			}
			else if ( TypeMath == 2 )
			{
				return ForceDamagePercentTable[ iForceT ];
			}
		}
	}
	return 0;
}

int __cdecl BuffBlessCastle( UserData * pcUserData, int iTypeDamage )
{
	if ( pcUserData )
	{
		if ( pcUserData->iMapID == MAPID_Bellatra )
			return 0;

		if ( iTypeDamage == 1 )
		{
			if ( pcUserData->dwBlessSkillTime >= TICKCOUNT && pcUserData->dwBlessSkill_Code == 0x0A1 )
				return (pcUserData->sCharacterData.iMinDamage * 10) / 100;
		}
		else if ( iTypeDamage == 2 )
		{
			if ( pcUserData->dwBlessSkillTime >= TICKCOUNT && pcUserData->dwBlessSkill_Code == 0x0A1 )
			{
				CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> BC Skill (2) Damage[%d + %d][%d + %d]",
									  pcUserData->sCharacterData.iMinDamage, (pcUserData->sCharacterData.iMinDamage * 10) / 100, pcUserData->sCharacterData.iMaxDamage, (pcUserData->sCharacterData.iMaxDamage * 10) / 100);
				return (pcUserData->sCharacterData.iMaxDamage * 10) / 100;
			}
		}
	}
	return 0;
}

int __cdecl BuffBellatraGold( UserData * pcUserData, int iTypeDamage )
{
	User * pcUser = USERDATATOUSER( pcUserData );
	if ( pcUserData && pcUser && pcUserData->iMapID != MAPID_Bellatra && pcUserData->iMapID != MAPID_BlessCastle )
	{
		if ( iTypeDamage == 1 )
		{
			if ( pcUser->pData.iBellatraCrown == 1 )
				return (pcUserData->sCharacterData.iMinDamage * 15) / 100;
		}
		else if ( iTypeDamage == 2 )
		{
			if ( pcUser->pData.iBellatraCrown == 1 )
			{
				CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> SoD Crown Skill (1) Damage[%d + %d][%d + %d]",
									  pcUserData->sCharacterData.iMinDamage, (pcUserData->sCharacterData.iMinDamage * 15) / 100, pcUserData->sCharacterData.iMaxDamage, (pcUserData->sCharacterData.iMaxDamage * 15) / 100);
				return (pcUserData->sCharacterData.iMaxDamage * 15) / 100;
			}
		}
	}
	return 0;
}

int AddBuffPvP( User * pcUser, int iTypeDamage )
{
	if ( pcUser->uPvPHonorTimeOut > TICKCOUNT )
	{
		int iAddAttackPower = 1;
		switch ( (PacketHonor::EHonorType)pcUser->iPvPHonorType )
		{
			case PacketHonor::EHonorType::HONORTYPE_PvPBronze:
				iAddAttackPower = 3;
				break;
			case PacketHonor::EHonorType::HONORTYPE_PvPSilver:
				iAddAttackPower = 5;
				break;
			case PacketHonor::EHonorType::HONORTYPE_PvPGold:
				iAddAttackPower = 7;
				break;
		}

		if ( iTypeDamage == 1 )
		{
			CHATSERVER->SendDebugChat( pcUser->pcUserData, CHATCOLOR_Error, "> PvP Buff Damage[%d + %d][%d + %d]",
									   pcUser->pcUserData->sCharacterData.iMinDamage, (pcUser->pcUserData->sCharacterData.iMinDamage * iAddAttackPower) / 100, 
									   pcUser->pcUserData->sCharacterData.iMaxDamage, (pcUser->pcUserData->sCharacterData.iMaxDamage * iAddAttackPower) / 100 );

			return (pcUser->pcUserData->sCharacterData.iMinDamage * iAddAttackPower) / 100;
		}
		else
			return (pcUser->pcUserData->sCharacterData.iMaxDamage * iAddAttackPower) / 100;
	}
	else
	{
		pcUser->iPvPHonorType		= 0;
		pcUser->uPvPHonorTimeOut	= 0;
	}

	return 0;
}

// 0x00555BA0
BOOL __cdecl BuffReceive( User * pcUser, PacketSkillPartyData * pPacket )
{
	if ( pPacket->iCount <= 0 )
		return TRUE;

	//Add Buff Skill to User
	int iClass = pcUser->pcUserData->sCharacterData.iClass;
	for ( int i = 0; i < pPacket->iCount; i++ )
	{
		UserData * ud = USERDATABYID( pPacket->iaID[i] );
		if ( ud )
		{
			User * pcTarget = USERDATATOUSER( ud );
			if ( pcTarget )
			{
				bool bCanSend = false;

				if ( iClass == CHARACTERCLASS_Mechanician )
				{
					if ( pcTarget != pcUser )
					{
						if ( CMechanician::AddMagneticDischarge( pcTarget, pPacket->iLevel, pcUser->pData.iLevelSkill[15 - 1] ) )
							bCanSend = true;
					}
				}
				else if ( iClass == CHARACTERCLASS_Knight )
				{
					if ( ud != pcUser->pcUserData )
					{
						if ( CKnight::AddHolyValor( ud, pPacket ) == TRUE )
							bCanSend = true;

						if ( CKnight::AddHolyConviction( pcUser, pcTarget, pPacket ) == TRUE )
							bCanSend = true;
					}
				}
				else if ( iClass == CHARACTERCLASS_Magician )
				{
					if ( CMagician::OnEnchantWeapon( ud, pPacket ) == TRUE )
						bCanSend = true;
				}
				else if ( iClass == CHARACTERCLASS_Priestess )
				{
					if ( CPriestess::OnVirtualLife( ud, pPacket ) == TRUE )
						bCanSend = true;
				}
				else if ( iClass == CHARACTERCLASS_Assassin )
				{
					if ( CAssassin::AddAlas( pcUser, pcTarget, pPacket ) == TRUE )
						bCanSend = true;
					if ( CAssassin::AddKissViper( pcUser, pcTarget, pPacket ) == TRUE )
						bCanSend = true;
				}
				else if ( iClass == CHARACTERCLASS_Shaman )
				{
					if ( CShaman::AddAdventMigal( ud, pPacket ) == TRUE )
						bCanSend = true;
				}

				if ( bCanSend )
				{
					if ( pcTarget != pcUser )
					{
						if ( BATTLEROYALEHANDLER->IsPVPMap( pcUser->GetMapID() ) )
							bCanSend = false;
					}

					if ( bCanSend )
						SENDPACKET( pcTarget, pPacket );
				}
			}
		}
	}
	return TRUE;
}

void __cdecl SendAttackRatting( UserData * pcUserData, UINT iAttackRating, UINT idEnemy )
{
	if ( pcUserData )
	{
		PacketAttackRating sPacket;
		sPacket.iLength = sizeof( PacketAttackRating );
		sPacket.iHeader = PKTHDR_AttackRatingSend;
		sPacket.iEnemyID = idEnemy;
		sPacket.iAttackRating = iAttackRating;

		SENDPACKET( USERDATATOUSER(pcUserData), &sPacket );
	}
}


int __cdecl AddBuffsDamage( UserData * pcUserData, UINT ArraySkill, UINT ArraySkillArea, UINT packet, UnitData * pcUnitData )
{
	User * pcUser = USERDATATOUSER( pcUserData );

	int iDamageForce = AddForceOrb( pcUserData, 1 );
	int iDamageForcePer = AddForceOrb( pcUserData, 2 );

	int iDamageMin = pcUserData->sCharacterData.iMinDamage;
	int iDamageMax = pcUserData->sCharacterData.iMaxDamage;

	iDamageMin += BuffBlessCastle( pcUserData, 1 );
	iDamageMax += BuffBlessCastle( pcUserData, 2 );

	if ( ( iDamageForce != 0 ) || ( iDamageForcePer != 0 ) )
	{
		int iDamageRangePercent = ( pcUserData->sCharacterData.iMinDamage + pcUserData->sCharacterData.iMaxDamage ) / 2;
		iDamageRangePercent = ( iDamageRangePercent * iDamageForcePer ) / 100;
		iDamageRangePercent += iDamageForce;
		CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Force Orb Damage ([%d-%d] + %d)", pcUserData->sCharacterData.iMinDamage, pcUserData->sCharacterData.iMaxDamage, iDamageRangePercent );
		iDamageMin += iDamageRangePercent;
		iDamageMax += iDamageRangePercent;
	}
	int iMaximizeMin = MECHANICIANSKILL->AddMaximize( pcUserData, 1 );
	int iMaximizeMax = MECHANICIANSKILL->AddMaximize( pcUserData, 2 );
	int iAutomationMin = MECHANICIANSKILL->AddAutomation( pcUserData, pcUnitData, 1, packet );
	int iAutomationMax = MECHANICIANSKILL->AddAutomation( pcUserData, pcUnitData, 2, packet );
	iDamageMin += iMaximizeMin;
	iDamageMax += iMaximizeMax;
	iDamageMin += iAutomationMin;
	iDamageMax += iAutomationMax;


	int iBerserkerMin = FIGHTERSKILL->AddBerserker( pcUserData, 1 );
	int iBerserkerMax = FIGHTERSKILL->AddBerserker( pcUserData, 2 );
	iDamageMin += iBerserkerMin;
	iDamageMax += iBerserkerMax;

	int iSMuspellMin = PRIESTESSSKILL->AddSummonMuspellDamageBoost( pcUserData, 1 );
	int iSMuspellMax = PRIESTESSSKILL->AddSummonMuspellDamageBoost( pcUserData, 2 );
	iDamageMin += iSMuspellMin;
	iDamageMax += iSMuspellMax;

	int iEnchantWeapon = MAGICIANSKILL->AddEnchantWeapon( pcUserData, ArraySkill );
	int iSpiritElementalMin = MAGICIANSKILL->AddSpiritElemental( pcUserData, 1 );
	int iSpiritElementalMax = MAGICIANSKILL->AddSpiritElemental( pcUserData, 2 );
	iDamageMin += iEnchantWeapon;
	iDamageMax += iEnchantWeapon;
	iDamageMin += iSpiritElementalMin;
	iDamageMax += iSpiritElementalMax;
	
	int iForceOfNatureMin = ARCHERSKILL->AddForceOfNature( pcUserData, 1 );
	int iForceOfNatureMax = ARCHERSKILL->AddForceOfNature( pcUserData, 2 );
	iDamageMin += iForceOfNatureMin;
	iDamageMax += iForceOfNatureMax;

	int iGodsBlessingMin = KNIGHTSKILL->AddGodsBlessing( pcUserData, 1 );
	int iGodsBlessingMax = KNIGHTSKILL->AddGodsBlessing( pcUserData, 2 );
	iDamageMin += iGodsBlessingMin;
	iDamageMax += iGodsBlessingMax;

	int iHolyConvictionMin = KNIGHTSKILL->AddHolyConvictionUndeadDamage( pcUserData, pcUnitData, 1 );
	int iHolyConvictionMax = KNIGHTSKILL->AddHolyConvictionUndeadDamage( pcUserData, pcUnitData, 2 );
	iDamageMin += iHolyConvictionMin;
	iDamageMax += iHolyConvictionMax;

	int iTriumphofValhallaMax = ATALANTASKILL->AddTriumphofValhalla( pcUserData, 2 );
	int iFrostJavelinMin = ATALANTASKILL->AddFrostJavelin( pcUserData, 1 );
	int iFrostJavelinMax = ATALANTASKILL->AddFrostJavelin( pcUserData, 2 );
	iDamageMax += iTriumphofValhallaMax;
	iDamageMin += iFrostJavelinMin;
	iDamageMax += iFrostJavelinMax;

	int iDamageAdventMigal = SHAMANSKILL->AddAdventMigalDamage( pcUserData );
	iDamageMin += iDamageAdventMigal;
	iDamageMax += iDamageAdventMigal;

	int iDamageChasingHuntMin = SHAMANSKILL->AddChasingHuntAtkPow( pcUserData, 1 );
	int iDamageChasingHuntMax = SHAMANSKILL->AddChasingHuntAtkPow( pcUserData, 2 );
	iDamageMin += iDamageChasingHuntMin;
	iDamageMax += iDamageChasingHuntMax;

	int iPvPBuffDamageMin = AddBuffPvP( pcUser, 1 );
	int iPvPBuffDamageMax = AddBuffPvP( pcUser, 2 );
	iDamageMin += iPvPBuffDamageMin;
	iDamageMax += iPvPBuffDamageMax;

	if ( iFrostJavelinMin > 0 )
	{
		if ( ArraySkill != 0 )
		{
			int iFJavelinRange = ((SQLSKILL->GetMinMaxInt( iFrostJavelinMin, iFrostJavelinMax, 0 ) << 14) | 0x00040000);
			*( UINT* )( ArraySkill + 0x1C ) = *( UINT* )( ArraySkill + 0x1C ) | iFJavelinRange;
		}
	}

	if ( QUESTSERVER->IsActiveQuest( USERDATATOUSER( pcUserData ), QUESTID_TheLoyaltyTestStep1 ) )
	{
		iDamageMin -= (iDamageMin * 20) / 100;
		iDamageMax -= (iDamageMax * 20) / 100;
	}

	
	{
		ItemPremium * psPremium = ITEMSERVER->GetPremiumUse( pcUserData, ITEMTIMERTYPE_ObscureHat );
		if ( psPremium && psPremium->dwTimeLeft )
		{
			iDamageMin += (iDamageMin * 5) / 100;
			iDamageMax += (iDamageMax * 5) / 100;
		}
	}

	int iDamageRange = SQLSKILL->GetMinMaxInt( iDamageMin, iDamageMax, 0 );
	CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Base Damage [%d - %d] = %d", iDamageMin, iDamageMax, iDamageRange );

	return iDamageRange;
}


int __cdecl RecvSkillSingleTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageNormalEx * psPacketEx )
{
	psPacket->iDamage = AddBuffsDamage( pcUserData, ( UINT )psPacket, 0, (UINT)psPacketEx, pcUnitData );
	pcUserData->sCharacterData.iAttackRating = psPacketEx->iAttackRating;

	int iClass = pcUserData->sCharacterData.iClass;


	if ( iClass == 1 ) // FS
	{
		return FIGHTERSKILL->RecvSkillSingleTarget( pcUserData, pcUnitData, psPacket, psPacketEx );
	}
	else if ( iClass == 2 ) // MS
	{
		return MECHANICIANSKILL->RecvSkillSingleTarget( pcUserData, pcUnitData, psPacket, psPacketEx );
	}
	else if ( iClass == 3 ) // AS
	{
		return ARCHERSKILL->RecvSkillSingleTarget( pcUserData, pcUnitData, psPacket, psPacketEx );
	}
	else if ( iClass == 4 ) // PS
	{
		return PIKEMANSKILL->RecvSkillSingleTarget( pcUserData, pcUnitData, psPacket, psPacketEx );
	}
	else if ( iClass == 5 ) // ATA
	{
		return ATALANTASKILL->RecvSkillSingleTarget( pcUserData, pcUnitData, psPacket, psPacketEx );
	}
	else if ( iClass == 6 ) // KS
	{
		return KNIGHTSKILL->RecvSkillSingleTarget( pcUserData, pcUnitData, psPacket, psPacketEx );
	}
	else if ( iClass == 7 ) // MG
	{
		return MAGICIANSKILL->RecvSkillSingleTarget( pcUserData, pcUnitData, psPacket, psPacketEx );
	}
	else if ( iClass == 8 ) // PRS
	{
		return PRIESTESSSKILL->RecvSkillSingleTarget( pcUserData, pcUnitData, psPacket, psPacketEx );
	}
	else if ( iClass == 9 ) // ASS
	{
		return ASSASSINSKILL->RecvSkillSingleTarget( pcUserData, pcUnitData, psPacket, psPacketEx );
	}
	else if ( iClass == 10 ) // SHA
	{
		return SHAMANSKILL->RecvSkillSingleTarget( pcUserData, pcUnitData, psPacket, psPacketEx );
	}
	return 0;
}

int __cdecl RecvBuffSkill( UserData * pcUserData, UINT packet )
{
	int iClass = pcUserData->sCharacterData.iClass;
	if ( iClass == 1 ) // FS
	{
		return FIGHTERSKILL->RecvBuffSkill( pcUserData, packet );
	}
	else if ( iClass == 2 ) // MS
	{
		return MECHANICIANSKILL->RecvBuffSkill( pcUserData, packet );
	}
	else if ( iClass == 3 ) // AS
	{
		return ARCHERSKILL->RecvBuffSkill( pcUserData, packet );
	}
	else if ( iClass == 4 ) // PS
	{
		return PIKEMANSKILL->RecvBuffSkill( pcUserData, packet );
	}
	else if ( iClass == 5 ) // ATA
	{
		return ATALANTASKILL->RecvBuffSkill( pcUserData, packet );
	}
	else if ( iClass == 6 ) // KS
	{
		return KNIGHTSKILL->RecvBuffSkill( pcUserData, packet );
	}
	else if ( iClass == 7 ) // MG
	{
		return MAGICIANSKILL->RecvBuffSkill( pcUserData, packet );
	}
	else if ( iClass == 8 ) // PRS
	{
		return PRIESTESSSKILL->RecvBuffSkill( pcUserData, packet );
	}
	else if ( iClass == 9 ) // ASS
	{
		return ASSASSINSKILL->RecvBuffSkill( pcUserData, packet );
	}
	else if ( iClass == 10 ) // ASS
	{
		return SHAMANSKILL->RecvBuffSkill( pcUserData, packet );
	}
	return 0;
}

int __cdecl RecvSkillAreaTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageRange * psPacketRange, PacketSkillDamageRangeEx * psPacketRangeEx )
{
	pcUserData->sCharacterData.iAttackRating = psPacketRangeEx->iAttackRating;

	// Check for CTRL key
	if ( !MAPSERVER->IsPVPMap( pcUserData->iMapID ) )
	{
//		if ( USERDATABYID( psPacketRangeEx->dwChkSum ) )
//			return 0;
	}

	psPacketRange->iDamage = AddBuffsDamage( pcUserData, 0, 0, ( UINT )psPacketRangeEx, pcUnitData );

	psPacketRange->iCount |= 0xFF000000; // AoE

	int iClass = pcUserData->sCharacterData.iClass;
	if ( !MAPSERVER->IsPVPMap( pcUserData->iMapID ) )
	{
		User * pcUser = USERDATATOUSER( pcUserData );

		UINT uTime = pcUserData->dwVanishEndTime;
		if ( uTime > TICKCOUNT )
		{
			pcUserData->dwVanishEndTime = 0;
			pcUserData->saVanishParam[0] = 0;
			pcUserData->saVanishParam[1] = 0;
			DAMAGEHANDLER->SendCancelSkill( pcUser, SKILLPLAYID_T32 );
		}

		//Blind
		if ( pcUser )
		{
			uTime = pcUser->dwBlindTime;
			if ( uTime > TICKCOUNT )
			{
				pcUser->dwBlindTime = 0;
				pcUser->dwBlindDamage = 0;
				pcUserData->saVanishParam[1] = 0;
				DAMAGEHANDLER->SendCancelSkill( pcUser, SKILLPLAYID_T34 );
			}
		}
		
		//Lethal Sight
		if ( pcUser )
		{
			uTime = pcUser->dwLethalSightTimeOut;
			if ( uTime > TICKCOUNT )
			{
				pcUser->dwLethalSightTimeOut = 0;
				pcUser->uLethalSightLevel	= 0;
				pcUserData->dwAssassinEyeEndTime = 0;
				DAMAGEHANDLER->SendCancelSkill( pcUser, SKILLPLAYID_T51 );
			}
		}
	}

	BOOL bResult = FALSE;

	if ( iClass == 1 ) // FS
	{
		bResult = FIGHTERSKILL->RecvSkillAreaTarget( pcUserData, pcUnitData, psPacket, psPacketRange, psPacketRangeEx );
	}
	else if ( iClass == 2 ) // MS
	{
		bResult = MECHANICIANSKILL->RecvSkillAreaTarget( pcUserData, pcUnitData, psPacket, psPacketRange, psPacketRangeEx );
	}
	else if ( iClass == 3 ) // AS
	{
		bResult = ARCHERSKILL->RecvSkillAreaTarget( pcUserData, pcUnitData, psPacket, psPacketRange, psPacketRangeEx );
	}
	else if ( iClass == 4 ) // PS
	{
		bResult = PIKEMANSKILL->RecvSkillAreaTarget( pcUserData, pcUnitData, psPacket, psPacketRange, psPacketRangeEx );
	}
	else if ( iClass == 5 ) // ATA
	{
		bResult = ATALANTASKILL->RecvSkillAreaTarget( pcUserData, pcUnitData, psPacket, psPacketRange, psPacketRangeEx );
	}
	else if ( iClass == 6 ) // KS
	{
		bResult = KNIGHTSKILL->RecvSkillAreaTarget( pcUserData, pcUnitData, psPacket, psPacketRange, psPacketRangeEx );
	}
	else if ( iClass == 7 ) // MG
	{
		bResult = MAGICIANSKILL->RecvSkillAreaTarget( pcUserData, pcUnitData, psPacket, psPacketRange, psPacketRangeEx );
	}
	else if ( iClass == 8 ) // PRS
	{
		bResult = PRIESTESSSKILL->RecvSkillAreaTarget( pcUserData, pcUnitData, psPacket, psPacketRange, psPacketRangeEx );
	}
	else if ( iClass == 9 ) // ASS
	{
		bResult = ASSASSINSKILL->RecvSkillAreaTarget( pcUserData, pcUnitData, psPacket, psPacketRange, psPacketRangeEx );
	}
	else if ( iClass == 10 ) // SHA
	{
		bResult = SHAMANSKILL->RecvSkillAreaTarget( pcUserData, pcUnitData, psPacket, psPacketRange, psPacketRangeEx );
	}

	DWORD dwAnd;
	dwAnd = psPacketRange->iCount & 0xFF000000;
	if ( dwAnd == 0xFF000000 )
		psPacketRange->iCount = 256;
	else
		psPacketRange->iCount &= 0x000000FF;

	return bResult;
}

void __cdecl SendSkillEffect( UserData * pcUserData, UserData * TargetPlayer, UINT skCode )
{
	int iClass = pcUserData->sCharacterData.iClass;
	if ( iClass == 8 )
	{
		if ( skCode == CPriestess::Skills::GlacialSpike )
		{
			int iSKLevel = 0;// pPlayer->Memory->LevelSkill14;
			SendPacketSkillEffect( pcUserData, TargetPlayer, skCode, iSKLevel );
		}
	}
}