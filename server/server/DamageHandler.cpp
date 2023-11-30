#include "stdafx.h"
#include "DamageHandler.h"
#include "Assassin.h"
#include "Priestess.h"


extern int __cdecl RecvSkillSingleTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageNormalEx * psPacketEx );
extern void __cdecl SendAttackRatting( UserData * pcUserData, UINT iAttackRating, UINT idEnemy );

NAKED BOOL CDamageHandler::SkillRangeMonsterSet( UnitData * pcUnitData, UserData * pcTarget, int iRange, int iDamagePercentDecrease )
{
	JMP( pfnSkillRangeMonsterSet );
}

BOOL CDamageHandler::SendDamageBySocket( UnitData * pcThis, UnitData * pcUnitData, SocketData * pcSocketData, int iAttackID, int iDamageAdd, int iResistance )
{
	return (BOOL)CALLT_WITH_ARG5( pfnSendDamageBySocket, (DWORD)pcThis, (DWORD)pcUnitData, (DWORD)pcSocketData, iAttackID, iDamageAdd, iResistance );
}

NAKED BOOL CDamageHandler::DecryptPacket( UserData * pcUserData, PacketSkillDamageNormalEx * psPacket )
{
	JMP( 0x005798F0 );
}

NAKED DWORD CDamageHandler::GetPacketChecksum( PacketSkillDamageNormalEx * psPacket )
{
	JMP( 0x004050A0 );
}

NAKED int CDamageHandler::GetCriticalChance( UserData * pcUserData, UnitData * pcUnitData, int iCritical )
{
	JMP( 0x00577E60 );
}

NAKED int CDamageHandler::GetCriticalChanceUser( UserData * pcUserData, UserData * pcTarget, int iCritical )
{
	JMP( 0x00577ED0 );
}

BOOL CDamageHandler::ProcessDamage( UserData * pcUserData, UserData * pcTarget, PacketSkillDamageNormal * psPacket )
{
	User * pcUser = USERDATATOUSER( pcUserData );
	User * pcUserTarget = USERDATATOUSER( pcTarget );

	if ( (pcUser == nullptr) || (pcUserTarget == nullptr) )
		return FALSE;

	if ( pcUserData->iGameLevel == GAMELEVEL_None )
	{
		if ( pcUserData->iMapID == MAPID_BlessCastle )
		{
			//Not Massacre or in Bless castle?
			if ( (READDWORD( 0x08B86750 ) != 2) || (READDWORD( 0x07AB1EE8 ) == 0) )
			{
				//Not Inside Area? 1 = Inside Area, 2 = target still in area + caution zone
				if ( (pcUserData->sPosition.iZ > 0xFFA21500) || (pcTarget->sPosition.iZ > 0xFFA3E1F8) )
					return FALSE;
			}
		}
		else
		{
			BOOL bPvPMap = MAPSERVER->IsPVPMap( pcUserData->iMapID );
			if ( (bPvPMap == FALSE) && MAPSERVER->IsPKMap( pcUserData->iMapID ) )
			{
				if ( (pcUser->bPvPMode == false) || (pcUserTarget->bPvPMode == false) )
					return FALSE;

				if ( pcUser->bParty && pcUserTarget->bParty && (pcUser->psParty == pcUserTarget->psParty) )
					return FALSE;

				bPvPMap = TRUE;
			}

			if ( bPvPMap == FALSE )
				return FALSE;
		}
	}

	//Debug
	if ( pcUserData->iGameLevel > GAMELEVEL_Two )
		CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> %s Damage ( %d -> %d )", pcTarget->sCharacterData.szName, psPacket->iDamage, (psPacket->iDamage / 5) );

	int iSkillID	= 0;
	int iTime		= 0;

	//Resistances
	int iResistanceID = (psPacket->iDamageState >> 16) & 0xF;

	if ( iResistanceID && (iResistanceID < 15) )
	{
		int iValue = (psPacket->iDamageState >> 20);
		if ( iValue == 0 )
			iValue = psPacket->iDamage;

		int iResistanceValue = pcTarget->sCharacterData.sElementalDef[iResistanceID - 1];
		if ( iResistanceValue )
		{
			if ( iResistanceValue > 100 )
				iResistanceValue = 100;
			else if ( iResistanceValue < (-100) )
				iResistanceValue = (-100);

            int iValueToReduce = (iValue * iResistanceValue) / 100;

            //Debug
            if ( pcUserData->iGameLevel > GAMELEVEL_Two )
            {
                CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> %s Resistance Reduce ( [%d - %d] -> %d )", psPacket->iDamage, iValueToReduce,
                    (psPacket->iDamage - iValueToReduce) < 0 ? 0 : (psPacket->iDamage - iValueToReduce) );

                CHATSERVER->SendDebugChat( pcTarget, CHATCOLOR_Error, "> %s Resistance Reduce ( [%d - %d] -> %d )", psPacket->iDamage, iValueToReduce,
                    (psPacket->iDamage - iValueToReduce) < 0 ? 0 : (psPacket->iDamage - iValueToReduce) );
            }

            psPacket->iDamage -= iValueToReduce;
            if ( psPacket->iDamage < 0 )
                psPacket->iDamage = 0;
		}

		switch ( iResistanceID - 1 )
		{
			//Ice
			case 3:
				//Enchant Weapon
				if ( pcUserData->dwEnchantWeaponEndTime > TICKCOUNT )
				{
					if ( (pcUserData->iEnchantWeaponAttr >> 8) == 0 )
					{
						iSkillID	= (-0x2C);
						iTime		= 230 - ((pcUserData->iEnchantWeaponAttr & 0xFF) * 10);
					}
				}
				//Dancing Sword
				if ( pcUserData->dwDancingSwordEndTime > TICKCOUNT )
				{
					if ( (pcUserData->iDancingSwordLevel & 0xFF) == 0 )
					{
						iSkillID	= (-0x3D);
						iTime		= 230 - ((pcUserData->iDancingSwordLevel >> 16) * 10);
					}
				}
				//Frost Javelin
				if ( pcUserData->FrostJavelinTime > TICKCOUNT )
				{
					iSkillID	= (-0x4C);
					iTime		= 240 - ((((int*)0x006BA728)[pcUserData->FrostJavelinParam] * 240) / 100);
				}
				break;

			default:
				break;
		}
	}

	return SendDamagePvPData( pcUserData, pcTarget, (psPacket->iDamage / 5), CHARACTERTYPE_Player, iSkillID, iTime );
}

BOOL CDamageHandler::SendDamagePvP( UserData * pcUserData, UserData * pcTarget, int iDamage, int iID, int iParam1, int iParam2 )
{
	User * pcUser = USERDATATOUSER( pcUserData );
	User * pcUserTarget = USERDATATOUSER( pcTarget );

	if ( (pcUser == nullptr) || (pcUserTarget == nullptr) )
		return FALSE;

	if ( pcUserData->iGameLevel == GAMELEVEL_None )
	{
		if ( pcUserData->iMapID == MAPID_BlessCastle )
		{
			//Not Massacre or in Bless castle?
			if ( (READDWORD( 0x08B86750 ) != 2) || (READDWORD( 0x07AB1EE8 ) == 0) )
			{
				//Not Inside Area? 1 = Inside Area, 2 = target still in area + caution zone
				if ( (pcUserData->sPosition.iZ > 0xFFA21500) || (pcTarget->sPosition.iZ > 0xFFA3E1F8) )
					return FALSE;
			}
		}
		else
		{
			BOOL bPvPMap = MAPSERVER->IsPVPMap( pcUserData->iMapID );
			if ( (bPvPMap == FALSE) && MAPSERVER->IsPKMap( pcUserData->iMapID ) )
			{
				if ( (pcUser->bPvPMode == false) || (pcUserTarget->bPvPMode == false) )
					return FALSE;

				if ( pcUser->bParty && pcUserTarget->bParty && (pcUser->psParty == pcUserTarget->psParty) )
					return FALSE;

				bPvPMap = TRUE;
			}

			if ( bPvPMap == FALSE )
				return FALSE;
		}
	}

	int iKillerAttackRating = pcUserData->sCharacterData.iAttackRating;

	//Battle Royale Handicap
	{
		float fDamage = BATTLEROYALEHANDLER->GetHandicap( USERDATATOUSER( pcUserData ), BattleRoyale::EHandicapType::HANDICAPTYPE_Damage );
		if ( fDamage > 0.0f )
			iDamage = (int)((float)iDamage * fDamage);

		float fAttackRating = BATTLEROYALEHANDLER->GetHandicap( USERDATATOUSER( pcUserData ), BattleRoyale::EHandicapType::HANDICAPTYPE_AttackRating );
		if ( fAttackRating > 0.0f )
			iKillerAttackRating = (int)((float)iKillerAttackRating * fAttackRating);
	}

	SendAttackRatting( pcTarget, iKillerAttackRating, pcUserData->iID );

	PacketSkillDamageNormal sPacketSkill;
	sPacketSkill.iLength = sizeof( PacketSkillDamageNormal );
	sPacketSkill.iHeader = PKTHDR_DamageData;

	sPacketSkill.sPosition = pcTarget->sPosition;

	CPriestess::OnHolyMindHandler( pcUserData, &sPacketSkill.iDamage );

	sPacketSkill.iDamageState	= iID;
	sPacketSkill.iRange			= 32 << 8;
	sPacketSkill.iDamage		= iDamage;

	//Bless Castle Items Reset
	for ( int i = 0; i < _countof( pcUserData->saBCBuff ); i++ )
	{
		if ( pcUserData->saBCBuff[i].dwItemEndTime )
		{
			if ( pcUserData->saBCBuff[i].dwItemEndTime < TICKCOUNT )
			{
				pcUserData->saBCBuff[i].dwItemEndTime	= 0;
				pcUserData->saBCBuff[i].iItemID			= 0;
				pcUserData->saBCBuff[i].iValueData		= 0;
			}
		}
	}

	//Athanasia
	if ( pcTarget->saBCBuff[0].iItemID == ITEMID_Athanasia )
		if ( pcTarget->iMapID == MAPID_BlessCastle )
			return FALSE;

	//Eternal Life
	if ( pcTarget->saBCBuff[0].iItemID == ITEMID_EternalLife )
		return FALSE;

	//Avail of Evade
	if ( pcTarget->saBCBuff[0].iItemID == ITEMID_AvailOfEvade )
	{
		if ( pcTarget->iMapID == MAPID_BlessCastle )
		{
			if ( pcTarget->saBCBuff[0].iValueData > (rand() % 100) )
			{
				PacketSimple sPacketEvade;
				sPacketEvade.iLength	= sizeof( PacketSimple );
				sPacketEvade.iHeader	= PKTHDR_EvasionSuccess;
				sPacketEvade.iUnk		= pcTarget->saBCBuff[0].iItemID;
				sPacketEvade.iUnk3		= pcUserData->iID;
				SENDPACKET( USERDATATOUSER( pcTarget ), &sPacketEvade );

				CHATSERVER->SendDebugChat( pcTarget, CHATCOLOR_Error, "> %s Damage Evaded [%s]", pcUserData->sCharacterData.szName, "Avail of Evade" );
				return FALSE;
			}
		}
	}

	//Avert Scroll
	if ( pcTarget->saBCBuff[0].iItemID == ITEMID_AvertScroll )
	{
		if ( pcTarget->saBCBuff[0].iValueData > (rand() % 100) )
		{
			PacketSimple sPacketEvade;
			sPacketEvade.iLength	= sizeof( PacketSimple );
			sPacketEvade.iHeader	= PKTHDR_EvasionSuccess;
			sPacketEvade.iUnk		= pcTarget->saBCBuff[0].iItemID;
			sPacketEvade.iUnk3		= pcUserData->iID;
			SENDPACKET( USERDATATOUSER( pcTarget ), &sPacketEvade );

			CHATSERVER->SendDebugChat( pcTarget, CHATCOLOR_Error, "> %s Damage Evaded [%s]", pcUserData->sCharacterData.szName, "Avert Scroll" );
			return FALSE;
		}
	}

	//Physical Absoption
	if ( pcTarget->dwPhysicalAbsorbEndTime )
	{
		if ( pcTarget->dwPhysicalAbsorbEndTime > TICKCOUNT )
		{
			sPacketSkill.iDamage -= pcTarget->iPhysicalAbsorbBoost;
			if ( sPacketSkill.iDamage <= 0 )
				return FALSE;
		}
		else
		{
			pcTarget->iPhysicalAbsorbBoost		= 0;
			pcTarget->dwPhysicalAbsorbEndTime	= 0;
		}
	}

	//Extreme Shield
	if ( pcTarget->dwExtremeShieldEndTime )
	{
		if ( pcTarget->dwExtremeShieldEndTime > TICKCOUNT )
		{
		//	sPacketSkill.iDamageState |= pcTarget->iExtremeShield << 16;
		}
		else
		{
			pcTarget->iExtremeShield			= 0;
			pcTarget->dwExtremeShieldEndTime	= 0;
		}
	}

	//Metal Armor
	if ( pcTarget->dwMetalArmorEndTime )
	{
		if ( pcTarget->dwMetalArmorEndTime > TICKCOUNT )
		{
			sPacketSkill.iDamage -= pcTarget->iMetalArmorParam;
			if ( sPacketSkill.iDamage <= 0 )
				return FALSE;
		}
		else
		{
			pcTarget->iMetalArmorParam		= 0;
			pcTarget->dwMetalArmorEndTime	= 0;
		}
	}

	//Virtual Life
	if ( pcTarget->dwVirtualLifeEndTime )
	{
		if ( pcTarget->dwVirtualLifeEndTime > TICKCOUNT )
		{
			sPacketSkill.iDamage -= (sPacketSkill.iDamage * pcTarget->iVirtualLifeData) / 100;
			if ( sPacketSkill.iDamage <= 0 )
				return FALSE;
		}
		else
		{
			pcTarget->iVirtualLifeData		= 0;
			pcTarget->dwVirtualLifeEndTime	= 0;
		}
	}

	//Energy Shield
	if ( pcTarget->dwEnergyShieldEndTime )
	{
		if ( pcTarget->dwEnergyShieldEndTime > TICKCOUNT )
		{
			int iReduce = (sPacketSkill.iDamage * pcTarget->iEnergyShieldDamage) / 100;
			sPacketSkill.iDamageState	|= (iReduce << 16);

            CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> %s Energy Shield ( [%d - %d] -> %d )", pcTarget->sCharacterData.szName, sPacketSkill.iDamage, iReduce,
                (sPacketSkill.iDamage - iReduce) < 0 ? 0 : sPacketSkill.iDamage - iReduce );

            CHATSERVER->SendDebugChat( pcTarget, CHATCOLOR_Error, "> %s Energy Shield ( [%d - %d] -> %d )", pcTarget->sCharacterData.szName, sPacketSkill.iDamage, iReduce,
                (sPacketSkill.iDamage - iReduce) < 0 ? 0 : sPacketSkill.iDamage - iReduce );

            sPacketSkill.iDamage -= iReduce;

			if ( sPacketSkill.iDamage <= 0 )
				return FALSE;            
		}
		else
		{
			pcTarget->iEnergyShieldDamage	= 0;
			pcTarget->dwEnergyShieldEndTime = 0;
		}
	}

	//Compulsion
	if ( pcTarget->dwHookedTargetEndTime )
	{
		if ( pcTarget->dwHookedTargetEndTime > TICKCOUNT )
		{
			sPacketSkill.iDamage -= pcTarget->iCompulsionParam;
			if ( sPacketSkill.iDamage <= 0 )
				return FALSE;
		}
		else
		{
			pcTarget->iCompulsionParam		= 0;
			pcTarget->dwHookedTargetEndTime = 0;
		}
	}

	//Godly Shield
	if ( pcTarget->dwGodlyShiedEndTime )
	{
		if ( pcTarget->dwGodlyShiedEndTime > TICKCOUNT )
		{
			sPacketSkill.iDamage -= (sPacketSkill.iDamage * ((int*)0x006BA480)[pcTarget->iGodlyShiedParam]) / 100;
			if ( sPacketSkill.iDamage <= 0 )
				return FALSE;
		}
		else
		{
			pcTarget->iGodlyShiedParam		= 0;
			pcTarget->dwGodlyShiedEndTime	= 0;
		}
	}

	//Alas
	if ( CAssassin::OnAlasEvade( pcTarget, pcUserData->iID ) )
		return FALSE;

	//Vague
	if ( pcTarget->dwVagueEndTime )
	{
		if ( pcTarget->dwVagueEndTime > TICKCOUNT )
		{
			if ( (pcTarget->sVagueEvade > (rand() % 100)) && ((pcTarget->sVagueLevel > 0) && (pcTarget->sVagueLevel <= 10)) )
			{
				PacketSimple sPacketEvade;
				sPacketEvade.iLength = sizeof( PacketSimple );
				sPacketEvade.iHeader = PKTHDR_EvasionSuccess;
				sPacketEvade.iUnk = 0x4C;
				sPacketEvade.iUnk3 = pcUserData->iID;
				SENDPACKET( USERDATATOUSER( pcTarget ), &sPacketEvade );

				CHATSERVER->SendDebugChat( pcTarget, CHATCOLOR_Error, "> %s Damage Evaded [%s]", pcUserData->sCharacterData.szName, "Vague" );
				return FALSE;
			}
		}
		else
		{
			pcTarget->sVagueEvade		= 0;
			pcUserData->sVagueLevel		= 0;
			pcTarget->dwVagueEndTime	= 0;
		}
	}

	//Hall of Valhalla
	if ( pcTarget->HallOfValhallaTime )
	{
		if ( pcTarget->HallOfValhallaTime > TICKCOUNT )
		{
			if ( pcTarget->HallOfValhallaParam[0] > (rand() % 100) )
			{
				PacketSimple sPacketEvade;
				sPacketEvade.iLength = sizeof( PacketSimple );
				sPacketEvade.iHeader = PKTHDR_EvasionSuccess;
				sPacketEvade.iUnk = 0x6D;
				sPacketEvade.iUnk3 = pcUserData->iID;
				SENDPACKET( USERDATATOUSER( pcTarget ), &sPacketEvade );

				CHATSERVER->SendDebugChat( pcTarget, CHATCOLOR_Error, "> %s Damage Evaded [%s]", pcUserData->sCharacterData.szName, "Hall Of Valhalla" );
				return FALSE;
			}
		}
		else
		{
			pcTarget->HallOfValhallaParam[0]	= 0;
			pcTarget->HallOfValhallaParam[1]	= 0;
			pcTarget->HallOfValhallaTime		= 0;
		}
	}

	//Summon Muspell
	if ( pcTarget->SummonMuspellTime )
	{
		if ( pcTarget->SummonMuspellTime > TICKCOUNT )
		{
			if ( pcTarget->SummonMuspellParam > (DWORD)(rand() % 100) )
			{
				PacketSimple sPacketEvade;
				sPacketEvade.iLength = sizeof( PacketSimple );
				sPacketEvade.iHeader = PKTHDR_EvasionSuccess;
				sPacketEvade.iUnk = 0x6D;
				sPacketEvade.iUnk3 = pcUserData->iID;
				SENDPACKET( USERDATATOUSER( pcTarget ), &sPacketEvade );

				CHATSERVER->SendDebugChat( pcTarget, CHATCOLOR_Error, "> %s Damage Evaded [%s]", pcUserData->sCharacterData.szName, "Summon Muspell" );
				return FALSE;
			}
		}
		else
		{
			pcTarget->SummonMuspellParam	= 0;
			pcTarget->SummonMuspellTime		= 0;
		}
	}

	sPacketSkill.sRating[0] = (short)iParam1;
	sPacketSkill.sRating[1] = (short)iParam2;

	sPacketSkill.iTargetReceiveDamageID = pcUserData->iID;
	sPacketSkill.iID					= pcTarget->iID;

	sPacketSkill.dwChkSum = (sPacketSkill.iDamage * 2002) + (sPacketSkill.sPosition.iX * sPacketSkill.sPosition.iY * sPacketSkill.sPosition.iZ);

    CHATSERVER->SendDebugChat( pcTarget, CHATCOLOR_Error, "> %s Final Damage [%d]", pcUserData->sCharacterData.szName, sPacketSkill.iDamage );
    CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> %s Final Damage [%d]", pcUserData->sCharacterData.szName, sPacketSkill.iDamage );

	if ( pcTarget->pcSocketData )
	{
		DAMAGEHANDLER->UpdateDamageDataPVP( pcUserData, pcTarget, iDamage );

		sPacketSkill.dwDamageChkSum = CALL_WITH_ARG1( 0x00405120, (DWORD)&sPacketSkill );

		SENDPACKET( USERDATATOUSER( pcTarget ), &sPacketSkill );
	}

	return TRUE;
}

CDamageHandler::CDamageHandler()
{
}

CDamageHandler::~CDamageHandler()
{
}

NAKED BOOL CDamageHandler::SkillRangeHandler( UserData * pcUserData, UnitData * psUnitData, PacketSkillDamageNormal * psSkillNormal, PacketSkillDamageRange * psRangeData, PacketSkillDamageRangeEx * psRangeEx )
{
	JMP( pfnSetSkillRange );
}

NAKED BOOL CDamageHandler::SingleDamageRangeHandler( UserData * pcUserData, PacketSkillDamageNormal * psSkillNormal, UnitData * psUnitData, int iType )
{
	JMP( pfnSingleDamageRangeHandler );
}

NAKED BOOL CDamageHandler::DamageRangeHandler( UserData * pcUserData, PacketSkillDamageRange * psRangeData, int iMapID, int iSkillID )
{
	JMP( pfnDamageRangeHandler );
}

NAKED BOOL CDamageHandler::DamageRangeUserHandler( UserData * pcUserData, PacketSkillDamageRange * psRangeData, int iMapID, int iSkillID )
{
	JMP( pfnDamageRangeUserHandler );
}

NAKED BOOL CDamageHandler::SkillSingleHandler( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacketDamageNormal, PacketSkillDamageNormalEx * psPacket )
{
	JMP( 0x00577980 );
}

BOOL CDamageHandler::HandlePacket( User * pcUser, PacketSkillDamageNormalEx * psPacket )
{
	UserData * pcUserData = pcUser->pcUserData;

	PacketSkillDamageNormal sPacketSkillD;

	UnitData * pcUnitData = nullptr;

	//Decrypt Packet
	DecryptPacket( pcUserData, psPacket );

	if ( GetPacketChecksum( psPacket ) != psPacket->dwCheckSum )
	{
		PacketLogCheat s;
		s.iCheatID = CHEATLOGID_DamagePacketError;
		s.LParam = 1;
		s.SParam = 0;
		s.EParam = 0;
		LOGSERVER->OnLogCheat( pcUserData, &s );
		return FALSE;
	}

	//Damage Timer
	pcUserData->iPacketDamageCount++;
	if ( pcUserData->dwPacketDamageTime < TICKCOUNT )
	{
		if ( pcUserData->iPacketDamageCount > MAX_DAMAGETIME_COUNT )
		{
			//Error, Log it...
			PacketLogCheat s;
			s.iCheatID = CHEATLOGID_LimitDamageTime;
			s.LParam = pcUserData->iPacketDamageCount;
			s.SParam = MAX_DAMAGETIME_COUNT;
			s.EParam = MAX_DAMAGETIME_TIME;
			LOGSERVER->OnLogCheat( pcUserData, &s );
		}

		pcUserData->iPacketDamageCount = 0;
		pcUserData->dwPacketDamageTime = TICKCOUNT + MAX_DAMAGETIME_TIME;
	}

	//User Dead? Not Handle Damage...
	if ( pcUserData->IsDead() )
		return FALSE;

	//Damage Limit Overpower?
	if ( psPacket->sAttackPower.sMin > pcUserData->sDamageLimit.sMin ||
		 psPacket->sAttackPower.sMax > pcUserData->sDamageLimit.sMax ||
		 psPacket->saStatus[0] > pcUserData->sCriticalLimit.sMin )
	{
		//Error, Log it...
		PacketLogCheat s;
		s.iCheatID = CHEATLOGID_LimitDamageOver;
		s.WxParam = TRUE;
		s.LParam = psPacket->sAttackPower.sMin;
		s.SParam = psPacket->sAttackPower.sMax;
		s.EParam = psPacket->saStatus[0];
		s.LxParam = pcUserData->sDamageLimit.sMin;
		s.SxParam = pcUserData->sDamageLimit.sMax;
		s.ExParam = pcUserData->sCriticalLimit.sMin;
		LOGSERVER->OnLogCheat( pcUserData, &s );


		//Set Minimal Damage
		psPacket->sAttackPower = pcUserData->sDamageLimit;
		psPacket->saStatus[0] = pcUserData->sCriticalLimit.sMin;
	}

	//Update Position
	sPacketSkillD.sPosition = psPacket->sPosition;

	//Range
	sPacketSkillD.iRange = psPacket->iRange;

	//IDs
	sPacketSkillD.iID						= psPacket->iID;
	sPacketSkillD.iTargetReceiveDamageID	= psPacket->iTargetReceiveDamageID;

	//New Damage
	sPacketSkillD.iDamage		= (int)psPacket->sAttackPower.RandomValue();
	sPacketSkillD.iDamageState	= psPacket->iDamageState;

	//Get Unit
	if ( psPacket->Area[0] >= 0 )
	{
		if ( (pcUnitData = UNITDATABYIDMAP( psPacket->iID, psPacket->Area[0] )) == NULL )
			pcUnitData = UNITSERVER->GetUnit( psPacket->iID );
	}
	else
		pcUnitData = UNITSERVER->GetUnit( psPacket->iID );

	if ( User * pcTarget = USERDATATOUSER( USERDATABYID( psPacket->iID ) ) )
	{
		EBattleRoyaleStateEnemy eState = BATTLEROYALEHANDLER->GetStateEnemy( pcUser, pcTarget );
		if ( (eState == BATTLEROYALESTATEENEMY_InBattleEnemy) || (eState == BATTLEROYALESTATEENEMY_None) )
		{
		}
		else if ( pcUser->bPvPMode && pcTarget->bPvPMode && MAPSERVER->IsPKMap( pcUser->GetMapID() ) )
		{

		}
		else
			psPacket->iID = -1;
	}

	//Handle Damage
	if ( RecvSkillSingleTarget( pcUserData, pcUnitData, &sPacketSkillD, psPacket ) == FALSE )
		return FALSE;

	//Handle More
	if ( SkillSingleHandler( pcUserData, pcUnitData, &sPacketSkillD, psPacket ) == FALSE )
		return FALSE;

	//Curse Skill (Quest)
	if ( pcUserData->dwCurseQuestTimeOut )
	{
		//On Time? Reduce damage
		if ( pcUserData->dwCurseQuestTimeOut > TICKCOUNT )
			sPacketSkillD.iDamage -= (sPacketSkillD.iDamage * pcUserData->dwCurseDamage) / 100;
		else
			pcUserData->dwCurseQuestTimeOut = 0;
	}

	//Unit Normal
	if ( pcUnitData )
	{
		//Critical
		if ( psPacket->saStatus[0] )
		{
			//Have Chance?
			if ( GetCriticalChance( pcUserData, pcUnitData, (int)psPacket->saStatus[0] ) > (rand() % 100) )
			{
				int iCriticalValueBoost = 70;

				//Assassin Eye
				if ( pcUserData->dwAssassinEyeEndTime )
				{
					if ( pcUserData->dwAssassinEyeEndTime > TICKCOUNT )
						iCriticalValueBoost += pcUserData->AssassinEyeParam;
					else
					{
						pcUserData->dwAssassinEyeEndTime = 0;
						pcUserData->AssassinEyeParam = 0;
					}
				}

				//Kiss Viper
				if ( pcUser->dwKissViperTimeOut )
				{
					if ( pcUser->dwKissViperTimeOut > TICKCOUNT )
						iCriticalValueBoost += pcUser->uKissViperCriticalAdd;
					else
					{
						pcUser->dwKissViperTimeOut		= 0;
						pcUser->uKissViperLevel			= 0;
						pcUser->uKissViperCriticalAdd	= 0;

						SendCancelSkill( pcUser, SKILLID_KissViper );
					}
				}

				//Other Skills
				iCriticalValueBoost += READDWORD( 0x08B80ACC );
				WRITEDWORD( 0x08B80ACC, 0 );

				int iDamageOld		= sPacketSkillD.iDamage;
				int iCriticalAdd	= (sPacketSkillD.iDamage * iCriticalValueBoost) / 100;

				//Add
				sPacketSkillD.iDamageState	 = (sPacketSkillD.iDamageState & 0xFFFF0000) + 2;
				sPacketSkillD.iDamage		+= iCriticalAdd;

				//Debug
				CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> %s Critical Hit [%d + %d][%d]", pcUnitData->sCharacterData.szName, iDamageOld, iCriticalAdd, sPacketSkillD.iDamage );

				//???
				WRITEDWORD( 0x08B81260, TRUE );

				//Result
				PacketSimple sPacketResult;
				sPacketResult.iLength = sizeof( PacketSimple );
				sPacketResult.iHeader = PKTHDR_DamageResult;
				sPacketResult.iUnk2 = TRUE;
				SENDPACKET( USERDATATOUSER( pcUserData ), &sPacketResult );
			}
		}

		//Super Power Event
		int iSuperPowerDamage = 0;
		if ( EVENT_SUPERPOWER > 0 )
		{
			iSuperPowerDamage = (sPacketSkillD.iDamage * EVENT_SUPERPOWER) / 100;
			sPacketSkillD.iDamage += iSuperPowerDamage;
		}

		SingleDamageRangeHandler( pcUserData, &sPacketSkillD, pcUnitData, TRUE );

		sPacketSkillD.iDamage -= iSuperPowerDamage;

		MAPSERVER->OnSendDebugDamageUnit( pcUnitData, pcUnitData->iDamageLastAttacker );

		//Lock on Target?
		if ( pcUnitData->dwHookedTargetEndTime == 0 )
		{
			if ( (pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_DEATHKNIGHT) ||
				 (pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_BABEL) ||
				 (pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CASTLE_CRYSTAL_R) ||
				 (pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CASTLE_CRYSTAL_G) ||
				 (pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CASTLE_CRYSTAL_B) )
			{
				if ( pcUserData->sPosition.WithinPTDistance( &pcUnitData->sPosition, pcUnitData->sCharacterData.iAttackRange ) )
				{
					pcUnitData->pcHookedTarget			= pcUserData;
					pcUnitData->dwHookedTargetEndTime	= TICKCOUNT + 4000;
				}
			}
		}
	}
	//Player PvP
	else
	{
		if ( UserData * pcTarget = USERDATABYID( psPacket->iID ) )
		{
			//Critical
			if ( psPacket->saStatus[0] )
			{
				//Have Chance?
				if ( GetCriticalChanceUser( pcUserData, pcTarget, (int)psPacket->saStatus[0] ) > (rand() % 100) )
				{
					int iCriticalValueBoost = 70;

					//Assassin Eye
					if ( pcUserData->dwAssassinEyeEndTime )
					{
						if ( pcUserData->dwAssassinEyeEndTime > TICKCOUNT )
							iCriticalValueBoost += pcUserData->AssassinEyeParam;
						else
						{
							pcUserData->dwAssassinEyeEndTime = 0;
							pcUserData->AssassinEyeParam = 0;
						}
					}

					//Kiss Viper
					if ( pcUser->dwKissViperTimeOut )
					{
						if ( pcUser->dwKissViperTimeOut > TICKCOUNT )
							iCriticalValueBoost += pcUser->uKissViperCriticalAdd;
						else
						{
							pcUser->dwKissViperTimeOut		= 0;
							pcUser->uKissViperLevel			= 0;
							pcUser->uKissViperCriticalAdd	= 0;

							SendCancelSkill( pcUser, SKILLID_KissViper );
						}
					}

					int iDamageOld = sPacketSkillD.iDamage;
					int iCriticalAdd = (sPacketSkillD.iDamage * iCriticalValueBoost) / 100;

					//Add
					sPacketSkillD.iDamageState = (sPacketSkillD.iDamageState & 0xFFFF0000) + 2;
					sPacketSkillD.iDamage += iCriticalAdd;

					//Debug
					CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> %s Critical Hit [%d + %d][%d]", pcTarget->sCharacterData.szName, iDamageOld, iCriticalAdd, sPacketSkillD.iDamage );

					//Result
					PacketSimple sPacketResult;
					sPacketResult.iLength = sizeof( PacketSimple );
					sPacketResult.iHeader = PKTHDR_DamageResult;
					sPacketResult.iUnk2 = TRUE;
					SENDPACKET( USERDATATOUSER( pcUserData ), &sPacketResult );
				}
			}
			ProcessDamage( pcUserData, pcTarget, &sPacketSkillD );
		}
		
	}

	//GM Mode?
	if ( pcUserData->iGameLevel )
	{
		//Send Last Damage to User
		PacketSimple s;
		s.iLength = sizeof( PacketSimple );
		s.iHeader = PKTHDR_LastAttackDamage;
		s.iUnk2 = sPacketSkillD.iDamage;
		s.iUnk = s.iUnk3 = s.iUnk4 = 0;
		SENDPACKET( USERDATATOUSER( pcUserData ), &s, TRUE );
	}

	return TRUE;
}

BOOL CDamageHandler::HandlePacket( User * pcUser, PacketSkillDamageRangeEx * psPacket )
{
	UserData * pcUserData = pcUser->pcUserData;

	PacketSkillDamageNormal sPacketSkillD;
	PacketSkillDamageRange	sPacketRange;
	
	int iSkillID			= 0;
	
	UnitData * pcUnitData	= NULL;

	//Damage Timer
	pcUserData->iPacketDamageCount++;
	if ( pcUserData->dwPacketDamageTime < TICKCOUNT )
	{
		if ( pcUserData->iPacketDamageCount > MAX_DAMAGETIME_COUNT )
		{
			//Error, Log it...
			PacketLogCheat s;
			s.iCheatID = CHEATLOGID_LimitDamageTime;
			s.LParam = pcUserData->iPacketDamageCount;
			s.SParam = MAX_DAMAGETIME_COUNT;
			s.EParam = MAX_DAMAGETIME_TIME;
			LOGSERVER->OnLogCheat( pcUserData, &s );
		}

		pcUserData->iPacketDamageCount = 0;
		pcUserData->dwPacketDamageTime = TICKCOUNT + MAX_DAMAGETIME_TIME;
	}

	//User Dead? Not Handle Damage...
	if ( pcUserData->IsDead() )
		return FALSE;

	//Damage Limit Overpower?
	if ( psPacket->sAttackPower.sMin > pcUserData->sDamageLimit.sMin ||
		psPacket->sAttackPower.sMax > pcUserData->sDamageLimit.sMax ||
		psPacket->saStatus[0] > pcUserData->sCriticalLimit.sMin )
	{
		//Error, Log it...
		PacketLogCheat s;
		s.iCheatID	= CHEATLOGID_LimitDamageOver;
		s.WxParam	= TRUE;		
		s.LParam	= psPacket->sAttackPower.sMin;
		s.SParam	= psPacket->sAttackPower.sMax;
		s.EParam	= psPacket->saStatus[0];
		s.LxParam	= pcUserData->sDamageLimit.sMin;
		s.SxParam	= pcUserData->sDamageLimit.sMax;
		s.ExParam	= pcUserData->sCriticalLimit.sMin;
		LOGSERVER->OnLogCheat( pcUserData, &s );


		//Set Minimal Damage
		psPacket->sAttackPower	= pcUserData->sDamageLimit;
		psPacket->saStatus[0] = pcUserData->sCriticalLimit.sMin;
	}

	sPacketSkillD.iDamage = 0;
	sPacketRange.iDamage = 0;

	//Have Single Target?
	if ( psPacket->iTargetID )
	{
		//Find
		pcUnitData = UNITDATABYIDMAP( psPacket->iTargetID, (int)psPacket->sMapID );

		sPacketSkillD.iDamageState = 0;
		sPacketSkillD.iRange = (64 * 64);
		sPacketSkillD.iTargetReceiveDamageID = psPacket->iTargetID;
		sPacketSkillD.sPosition = psPacket->sPosition;
	}

	//???
	sPacketRange.iRange = 200;
	sPacketRange.iTargetReceiveDamageID = psPacket->iTargetReceiveDamageID;
	sPacketRange.sPosition = psPacket->sPosition;

	if ( User * pcTarget = USERDATATOUSER( USERDATABYID( psPacket->iTargetReceiveDamageID ) ) )
	{
		EBattleRoyaleStateEnemy eState = BATTLEROYALEHANDLER->GetStateEnemy( pcUser, pcTarget );
		if ( (eState == BATTLEROYALESTATEENEMY_InBattleEnemy) || (eState == BATTLEROYALESTATEENEMY_None) )
		{
		}
		else if ( pcUser->bPvPMode && pcTarget->bPvPMode && MAPSERVER->IsPKMap( pcUser->GetMapID() ) )
		{

		}
		else
			psPacket->iTargetReceiveDamageID = -1;
	}


	//Check For Overflow
	if ( psPacket->iCount < 0 )
		psPacket->iCount = 0;
	else if ( psPacket->iCount > 128 )
		psPacket->iCount = 128;

	for ( int i = 0; i < psPacket->iCount; i++ )
	{
		if ( User * pcTarget = USERDATATOUSER( USERDATABYID( psPacket->iaTargetID[i] ) ) )
		{
			EBattleRoyaleStateEnemy eState = BATTLEROYALEHANDLER->GetStateEnemy( pcUser, pcTarget );
			if ( (eState == BATTLEROYALESTATEENEMY_InBattleEnemy) || (eState == BATTLEROYALESTATEENEMY_None) )
				continue;

			if ( pcUser->bPvPMode && pcTarget->bPvPMode && MAPSERVER->IsPKMap( pcUser->GetMapID() ) )
				continue;

			psPacket->iaTargetID[i] = -1;
		}
	}

	//Copy Data
	CopyMemory( sPacketRange.iaTargetID, psPacket->iaTargetID, psPacket->iCount * sizeof( int ) );
	sPacketRange.iCount = psPacket->iCount;

	//Not Handled Skill? Fail...
	if ( CDamageHandler::SkillRangeHandler( pcUserData, pcUnitData, &sPacketSkillD, &sPacketRange, psPacket ) == FALSE )
		return FALSE;

	//Debug
	CHATSERVER->SendDebugChat( pcUserData, EChatColor::CHATCOLOR_Error, "> Range Damage: Count[%d]", psPacket->iCount );

	//Have Target and Damage?
	if ( pcUnitData && sPacketSkillD.iDamage )
	{
		//Set Damage to Unit
		CDamageHandler::SingleDamageRangeHandler( pcUserData, &sPacketSkillD, pcUnitData, TRUE );
	}


	//Have Targets?
	if ( psPacket->iCount > 0 )
	{

		//Super Power Event
		int iSuperPowerDamage = 0;
		if ( EVENT_SUPERPOWER > 0 )
		{
			iSuperPowerDamage = (sPacketRange.iDamage * EVENT_SUPERPOWER) / 100;
			sPacketRange.iDamage += iSuperPowerDamage;
		}

		//Handle Damage to Targets
		CDamageHandler::DamageRangeHandler( pcUserData, &sPacketRange, (int)psPacket->sMapID, psPacket->iSkillID );

		//Remove Damage for Users
		sPacketRange.iDamage -= iSuperPowerDamage;

		//PVP Map? Handle Damage to Users
		if ( MAPSERVER->IsPVPMap( pcUserData->iMapID ) || (pcUser->bPvPMode && MAPSERVER->IsPKMap( pcUser->GetMapID() )) )
		{
			DAMAGEHANDLER->SetIsAreaDamage( TRUE );

			//Handle Damage to Users
			CDamageHandler::DamageRangeUserHandler( pcUserData, &sPacketRange, (int)psPacket->sMapID, psPacket->iSkillID );

			DAMAGEHANDLER->SetIsAreaDamage( FALSE );
		}
	}

	//GM Mode?
	if ( pcUserData->iGameLevel )
	{
		//Send Last Damage to User
		PacketSimple s;
		s.iLength = sizeof( PacketSimple );
		s.iHeader = PKTHDR_LastAttackDamage;
		s.iUnk2 = sPacketSkillD.iDamage + sPacketRange.iDamage;
		s.iUnk = s.iUnk3 = s.iUnk4 = 0;
		SENDPACKET( USERDATATOUSER(pcUserData), &s, TRUE );
	}

	return TRUE;
}

BOOL CDamageHandler::HandlePacket( User * pcUser, PacketCancelSkill * psPacket )
{
	if ( pcUser->pcUserData->sCharacterData.iClass == CHARACTERCLASS_Shaman )
	{
		if ( psPacket->iID == SKILLID_Rainmaker )
			pcUser->pcUserData->dwEnergyShieldEndTime = 0;
	}
	else if ( pcUser->pcUserData->sCharacterData.iClass == CHARACTERCLASS_Pikeman )
	{
		if ( psPacket->iID == SKILLID_Vague )
			pcUser->pcUserData->sVagueLevel = psPacket->iParam1;
	}
	return TRUE;
}

BOOL CDamageHandler::UpdateDamageDataPVP( UserData * pcKiller, UserData * pcVictim, int iDamage )
{
	AttackDamageData * psaDamageData = NULL;

	BlessCastleStatusData * psBCStatus = BLESSCASTLESERVER->psBlessCastleStatusData;

	if ( (pcKiller->iClanID == 0) || (pcKiller->iClanID == pcVictim->iClanID) )
	{
		if ( (pcKiller->iClanID != psBCStatus->iClanID) && (pcKiller->iClanID != psBCStatus->iClanID) )
			return FALSE;
	}

	if ( psBCStatus->psaDamageData )
		psaDamageData = psBCStatus->psaDamageData;
	else if ( psBCStatus->pcUnitDataTower )
	{
		if ( psBCStatus->pcUnitDataTower->bActive &&
			(psBCStatus->pcUnitDataTower->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CASTLE_TOWER_B) &&
			(psBCStatus->pcUnitDataTower->sCharacterData.sHP.sCur > 0) )
		{
			psaDamageData = psBCStatus->psaDamageData;
		}
	}

	if ( psaDamageData == NULL )
		return FALSE;

	iDamage -= pcVictim->sCharacterData.iAbsorbRating;

	iDamage /= 10;

	//Area Damage Reduction
	if ( IsAreaDamage() )
		iDamage /= 8;

	if ( iDamage <= 0 )
		iDamage = 1;

	pcKiller->sNumDeath += iDamage;

	if ( pcKiller->iClanID && pcKiller->iMapID == MAPID_BlessCastle )
	{
		BOOL bSuccess = FALSE;

		int iSlot = -1;

		for ( int i = 0; i < ATTACK_DAMAGE_MAX; i++ )
		{
			if ( psaDamageData[i].iID )
			{
				int iClanID = BLESSCASTLESERVER->GetClanCodeByID( psaDamageData[i].iID );

				if ( iClanID == pcKiller->iClanID )
				{
					psaDamageData[i].iDamage += iDamage;
					psaDamageData[i].iCounter++;
					bSuccess = TRUE;
					break;
				}
			}

			if ( (iSlot < 0) && (psaDamageData[i].iID == 0) )
				iSlot = i;
		}

		if ( (bSuccess == FALSE) && (iSlot >= 0) )
		{
			psaDamageData[iSlot].iID = pcKiller->sCharacterData.iClanID;
			psaDamageData[iSlot].iDamage = iDamage;
			psaDamageData[iSlot].iCounter++;
		}
	}

	return TRUE;
}

BOOL CDamageHandler::IsMonsterAreaDamage( UnitData * pcUnitData )
{
	if ( pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_Faugn )
	{
		if ( pcUnitData->sPosition.WithinPTDistance( &pcUnitData->pcFocusTarget->sPosition, (200 * 200) ) == FALSE )
		{
			CDamageHandler::SendDamageBySocket( pcUnitData, NULL, pcUnitData->pcFocusTarget->pcSocketData );
		}
	}

	return FALSE;
}

BOOL CDamageHandler::SendDamagePvPData( UserData * pcUserData, UserData * pcTarget, int iDamage, int iID, int iParam1, int iParam2 )
{
	if ( bNewPvPHandler )
	{
		for ( int i = 0; i < MAX_PVP_DAMAGE; i++ )
		{
			DamagePvPDataHandler & sDamagePvP = pcUserData->saPvPDamageData[i];
			if ( sDamagePvP.bActive == FALSE )
			{
				sDamagePvP.bActive = TRUE;

				sDamagePvP.iDamage = iDamage;
				sDamagePvP.iSkillID = iID;
				sDamagePvP.iParam1 = iParam1;
				sDamagePvP.iParam2 = iParam2;
				sDamagePvP.pcTarget = pcTarget;
				sDamagePvP.iTargetID = pcTarget->iID;

				int iDelay = 140;

				if ( pcUserData->pcSocketData && pcTarget->pcSocketData )
					iDelay = (pcUserData->pcSocketData->iPing + pcTarget->pcSocketData->iPing) >> 1;

				if ( iDelay > 200 )
					iDelay = 200;

				sDamagePvP.dwTimeUpdate = TICKCOUNT + iDelay + 180;

				break;
			}
		}

		return TRUE;
	}

	return CDamageHandler::SendDamagePvP( pcUserData, pcTarget, iDamage, iID, iParam1, iParam2 );
}

BOOL CDamageHandler::UpdateDamagePvPData( User * pcUser )
{
	if ( bNewPvPHandler )
	{
		for ( int i = 0; i < MAX_PVP_DAMAGE; i++ )
		{
			DamagePvPDataHandler & sDamagePvP = pcUser->pcUserData->saPvPDamageData[i];
			if ( sDamagePvP.bActive )
			{
				if ( sDamagePvP.dwTimeUpdate <= TICKCOUNT )
				{
					if ( (TICKCOUNT - sDamagePvP.dwTimeUpdate) < 500 )
					{
						if ( sDamagePvP.pcTarget && (sDamagePvP.pcTarget->iID != 0) && (sDamagePvP.pcTarget->iID == sDamagePvP.iTargetID) && (sDamagePvP.pcTarget->pcSocketData) )
							CDamageHandler::SendDamagePvP( pcUser->pcUserData, sDamagePvP.pcTarget, sDamagePvP.iDamage, sDamagePvP.iSkillID, sDamagePvP.iParam1, sDamagePvP.iParam2 );
					}

					sDamagePvP.bActive = FALSE;
				}
			}
		}          
	}

	return TRUE;
}

void CDamageHandler::SendCancelSkill( User * pcUser, int iPlayID )
{
	PacketSkillCancel s;
	s.iLength	= sizeof( PacketSkillCancel );
	s.iHeader	= PKTHDR_SkillCast;
	s.iPlayID	= iPlayID;
	s.iUnk		= 0;
	s.iUnk2		= 0;
	s.iUnk3		= 0;
	SENDPACKET( pcUser, &s );
}

BOOL CDamageHandler::CanHitTargetAreaDamage( UserData * pcUserData, UnitData * pcUnitData )
{
	User * pcUser = USERDATATOUSER( pcUserData );

	if ( pcUser->GetMapID() != pcUnitData->pcMap->pcBaseMap->iMapID )
		return FALSE;

	if ( pcUser->bInT5QuestArena )
	{
		if ( pcUser->GetMapID() == MAPID_T5QuestArena )
		{
			if ( pcUnitData->lExclusiveTargetID )
			{
				if ( pcUnitData->lExclusiveTargetID != pcUser->GetID() )
					return FALSE;
			}
		}
	}
	
	return ROOMINSTANCEHANDLER->CanSee( pcUserData->iID, pcUnitData->iID );
}
