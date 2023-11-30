#include "stdafx.h"
#include "Atalanta.h"
#include "HNSSkill.h"

typedef void( __cdecl *tfnSendSkillDebuff )(UserData * pcUserData, UserData * pcUserDataTarget, int iUnk, int iu, int iSkillID, int iTime);
tfnSendSkillDebuff fnSendSkillDebuff = (tfnSendSkillDebuff)0x00401FF4;

CAtalanta::CAtalanta()
{
}

int CAtalanta::AddFrostJavelin( UserData * pcUserData, int TypeDamage )
{
	UINT uTime = pcUserData->FrostJavelinTime;
	if ( uTime >= TICKCOUNT )
	{
		int iLevelSkill = pcUserData->FrostJavelinParam + 1;
		if ( TypeDamage == 1 )
		{
			int iDamageMin = SQLSKILL->GetSkillValueInt( "SELECT DamageMin FROM FrostJavelin WHERE SkillLevel=%d", iLevelSkill );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Frost Javelin Damage Min [%d + %d]", pcUserData->sCharacterData.iMinDamage, iDamageMin );
			return iDamageMin;
		}
		else
		{
			int iDamageMax = SQLSKILL->GetSkillValueInt( "SELECT DamageMax FROM FrostJavelin WHERE SkillLevel=%d", iLevelSkill );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Frost Javelin Damage Max [%d + %d]", pcUserData->sCharacterData.iMaxDamage, iDamageMax );
			return iDamageMax;
		}
	}
	return 0;
}

int CAtalanta::AddTriumphofValhalla( UserData * pcUserData, int TypeDamage )
{

	

	UINT uTime = pcUserData->dwTriumphOfValhallaEndTime;
	if ( uTime >= TICKCOUNT )
	{
		int iValue = pcUserData->iTriumphOfValhallaDamage;
		if ( TypeDamage == 1 )
		{
			int iAtkMin = pcUserData->sCharacterData.iMinDamage + iValue;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Triumph of Valhalla Damage Min [%d + %d]", pcUserData->sCharacterData.iMinDamage, iValue );
			return iValue;
		}
		else
		{
			int iAtkMax = pcUserData->sCharacterData.iMaxDamage + iValue;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Triumph of Valhalla Damage Max [%d + %d]", pcUserData->sCharacterData.iMaxDamage, iValue );
			return iValue;
		}
	}

	uTime = pcUserData->HallOfValhallaTime;
	if ( uTime >= TICKCOUNT )
	{
		int iValue = pcUserData->HallOfValhallaParam[1];
		if ( TypeDamage == 2 )
		{
			int iAtkMax = pcUserData->sCharacterData.iMaxDamage + iValue;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Triumph of Valhalla Damage Max [%d + %d]", pcUserData->sCharacterData.iMaxDamage, iValue );
			return iValue;
		}
	}

	return 0;
}

int CAtalanta::RecvSkillSingleTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageNormalEx * psPacketEx )
{
	UINT uSkillID = psPacketEx->iSkillID & 0x000000FF;
	UINT uUnkV = ( psPacketEx->iSkillID / 4096 ) & 0x0000000F;
	int iLevelSkill = ( psPacketEx->iSkillID / 256 ) & 0x0000000F;

	UserData * ud = NULL;
	if ( !pcUnitData )
		ud = USERDATABYID( psPacketEx->iID );
	

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
			UINT uTime = pcUserData->FrostJavelinTime;
			if ( uTime >= TICKCOUNT )
			{
				int iTime = 5 << 4;
				int iRes = 0;
				if ( ud )
				{
					iRes = ud->sCharacterData.sElementalDef[3]; // ice
					if ( iRes > 100 )
						iRes = 100;
					if ( iRes < 0 )
						iRes = 0;

					iTime = (iTime*(100 - iRes)) / 100;
					fnSendSkillDebuff( pcUserData, ud, 1, 128, -0x4C, iTime );
				}
				else if ( pcUnitData && pcUnitData->sCharacterData.iType != CHARACTERTYPE_NPC )
				{
					iRes = pcUnitData->sCharacterData.sElementalDef[3]; // ice
					if ( iRes > 100 )
						iRes = 100;
					if ( iRes < 0 )
						iRes = 0;

					iTime = (iTime*(100 - iRes)) / 100;

					pcUnitData->iUpdateMoveReduce = 200;
					pcUnitData->iFreezeTimeLeft = iTime;
					pcUnitData->iIceOverlay = TRUE;
				}

			}
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Default Attack]" );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
		} break;

		case Skills::Farina:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Farina]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM Farina WHERE SkillLevel=%d", iLevelSkill );
			int iDamageBoost = psPacket->iDamage;
			psPacket->iDamage = iDamageBoost + ( ( iDamageBoost * iValue ) / 100 );
			if ( pcUnitData )
			{
				if ( pcUnitData->sCharacterData.iMonsterType == 0 )
				{
					psPacket->iDamage += ( ( iDamageBoost * 20 ) / 100 );
					CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> +20%% Normal Monster" );
				}
			}
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
		} break;

		case Skills::BiggerSpear:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Bigger Spear]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM BiggerSpear WHERE SkillLevel=%d", iLevelSkill );
			int iDamageBoost = psPacket->iDamage;
			psPacket->iDamage = iDamageBoost + ( ( iDamageBoost * iValue ) / 100 );
			if ( pcUnitData )
			{
				if ( pcUnitData->sCharacterData.iMonsterType == 0x93 )
				{
					psPacket->iDamage += ( ( iDamageBoost * 30 ) / 100 );
					CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> +30%% Machine Monster" );
				}
			}
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
		} break;


		case Skills::TwistJavelin:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Twist Javelin]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM TwistJavelin WHERE SkillLevel=%d", iLevelSkill );
			int iDamageBoost = psPacket->iDamage;
			psPacket->iDamage = iDamageBoost + ( ( iDamageBoost * iValue ) / 100 );
			if ( pcUnitData )
			{
				if ( pcUnitData->sCharacterData.iMonsterType == 0x92 )
				{
					psPacket->iDamage += ( ( iDamageBoost * 30 ) / 100 );
					CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> +30%% Demon Monster" );
				}
			}
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
		} break;

		case Skills::FireJavelin:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Fire Javelin]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM FireJavelin WHERE SkillLevel=%d", iLevelSkill );
			int iDamageBoost = psPacket->iDamage;
			psPacket->iDamage = iDamageBoost + ( ( iDamageBoost * iValue ) / 100 );
			if ( pcUnitData )
			{
				if ( pcUnitData->sCharacterData.iMonsterType == 0x91 )
				{
					psPacket->iDamage += ( ( iDamageBoost * 30 ) / 100 );
					CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> +30%% Mutant Monster" );
				}
			}
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
		} break;

		case Skills::SplitJavelin:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Split Javelin]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM SplitJavelin WHERE SkillLevel=%d", iLevelSkill );
			int iDamageBoost = psPacket->iDamage;
			psPacket->iDamage = iDamageBoost + ( ( iDamageBoost * iValue ) / 100 );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
		} break;


		case Skills::LightningJavelin:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Lightning Javelin]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM LightningJavelin WHERE SkillLevel=%d", iLevelSkill );
			int iDamageBoost = psPacket->iDamage;
			psPacket->iDamage = iDamageBoost + ( ( iDamageBoost * iValue ) / 100 );
			if ( pcUnitData )
			{
				if ( pcUnitData->sCharacterData.iMonsterType == 0x90 )
				{
					psPacket->iDamage += ( ( iDamageBoost * 30 ) / 100 );
					CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> +30%% Undead Monster" );
				}
			}
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
		} break;

		case Skills::Vengeance:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Vengeance]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM Vengeance WHERE SkillLevel=%d", iLevelSkill );
			int iDamageBoost = psPacket->iDamage;
			psPacket->iDamage = iDamageBoost + ( ( iDamageBoost * iValue ) / 100 );

			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );

			psPacket->iDamageState = 0;
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

int CAtalanta::RecvSkillAreaTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageRange * psPacketRange, PacketSkillDamageRangeEx * psPacketRangeEx )
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
		case Skills::ShieldStrike:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Shield Strike]" );
			int iDamageMin = SQLSKILL->GetSkillValueInt( "SELECT DamageMin FROM ShieldStrike WHERE SkillLevel=%d", iLevelSkill );
			int iDamageMax = SQLSKILL->GetSkillValueInt( "SELECT DamageMax FROM ShieldStrike WHERE SkillLevel=%d", iLevelSkill );
			int iDamageBoost = SQLSKILL->GetMinMaxInt( iDamageMin, iDamageMax, 0 );
			psPacketRange->iDamage = iDamageBoost;
			psPacketRange->iDamageState = 5;
			psPacketRange->iCount &= 0x000000FF;
		} break;

		case Skills::SoulSucker:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Soul Sucker]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT HPAbsorption FROM SoulSucker WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iDamage = iValue;
			psPacketRange->iDamageState = 4;
			psPacketRange->iCount &= 0x000000FF;
		} break;

		case Skills::StormJavelin:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Storm Javelin]" );
			psPacketRange->iRange = 300;
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM StormJavelin WHERE SkillLevel=%d", iLevelSkill );
			int iDamageBoost = psPacketRange->iDamage;
			psPacketRange->iDamage = iDamageBoost + ( ( iDamageBoost * iValue ) / 100 );
			psPacketRange->iDamageState = 0;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketRangeEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
			psPacketRange->iCount &= 0x000000FF;
		}break;

		case Skills::ExtremeRage:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Extreme Rage]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM ExtremeRage WHERE SkillLevel=%d", iLevelSkill );
			int iDamageBoost = psPacketRange->iDamage;
			psPacketRange->iDamage = iDamageBoost + ( ( iDamageBoost * iValue ) / 100 );
			psPacketRange->iDamageState = 0;
			iValue = SQLSKILL->GetSkillValueInt( "SELECT Area FROM ExtremeRage WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iRange = iValue;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", iValue, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
		}break;

		case Skills::LightningJavelin:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Lightning Javelin]" );
			psPacketRange->iRange = 100;
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM LightningJavelin WHERE SkillLevel=%d", iLevelSkill );
			int iDamageBoost = psPacketRange->iDamage;
			psPacketRange->iDamage = iDamageBoost + ( ( iDamageBoost * iValue ) / 100 );
			psPacketRange->iDamageState = 101;
			psPacket->iDamage = psPacketRange->iDamage;
			if ( pcUnitData )
			{
				if ( pcUnitData->sCharacterData.iMonsterType == MONSTERTYPE_Undead )
				{
					psPacket->iDamage += ( ( iDamageBoost * 30 ) / 100 );
					psPacket->iDamageState = 0;
					CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> +30%% Undead Monster" );

					int iCritical = psPacketRangeEx->saStatus[0];
					if ( iCritical != 0 )
					{
						typedef int( __cdecl *t_GetCriticalMonster ) ( UserData * pcUserData, UnitData * Enemy, int Critical );
						t_GetCriticalMonster GetCriticalMonster = ( t_GetCriticalMonster )0x00577E60;
						int iCritValue = GetCriticalMonster( pcUserData, pcUnitData, iCritical );
						srand( ( UINT )time( NULL ) );
						int iRandChance = ( rand() % 99 ) + 1;
						if ( iRandChance < iCritValue )
						{
							int iCriticalAdd = ( psPacket->iDamage * 70 ) / 100;
							typedef int( __cdecl *t_SendMessageToPlayer ) ( UserData * pcUserData, UINT Buf );
							t_SendMessageToPlayer SendMessageToPlayer = ( t_SendMessageToPlayer )0x00576110;
							STRINGFORMATLEN( ( char* )( 0x7AC9FC8 ), 128, "> %s Critical [%d + %d]", pcUnitData->sCharacterData.szName, psPacket->iDamage, iCriticalAdd );
							psPacket->iDamage += iCriticalAdd;
							SendMessageToPlayer( pcUserData, 0x7AC9FC8 );
						}
					}
				}
			}
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketRangeEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
			psPacket->iDamageState = 0;

		}

		default:
			break;
	}

	return 1;
}

int CAtalanta::RecvBuffSkill( UserData * pcUserData, UINT packet )
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

	User * pcUser = USERDATATOUSER( pcUserData );

	switch ( uSkillID )
	{

		case Skills::TriumphofValhalla:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Triumph of Valhalla]" );
			UINT IDEnemy = *( UINT* )( packet + 0x10 );
			int iDamage = SQLSKILL->GetSkillValueInt( "SELECT AddedMaxAtkPower FROM TriumphofValhalla WHERE SkillLevel=%d", iLevelSkill );
			int iDuration = SQLSKILL->GetSkillValueInt( "SELECT Duration FROM TriumphofValhalla WHERE SkillLevel=%d", iLevelSkill );
			if ( IDEnemy != 0 )
			{

				UserData * pcUserDataEnemy = USERSERVER->GetUserdata( IDEnemy );
				if ( pcUserDataEnemy )
				{
					if ( pcUserDataEnemy->HallOfValhallaTime < TICKCOUNT )
					{
						pcUserDataEnemy->dwTriumphOfValhallaEndTime = ( iDuration * 1000 ) + TICKCOUNT;
						pcUserDataEnemy->iTriumphOfValhallaDamage = iDamage / 2;
						*( UINT* )( packet + 0x14 ) = pcUserData->sCharacterData.iLevel;
						*( UINT* )( packet + 0x08 ) = pcUserData->sCharacterData.iClass;
						SENDPACKET( USERDATATOUSER(pcUserDataEnemy), ( void* )packet );
					}
				}
			}
			else
			{
				pcUserData->HallOfValhallaTime = 0;
				pcUserData->dwTriumphOfValhallaEndTime = ( iDuration * 1000 ) + TICKCOUNT;
				pcUserData->iTriumphOfValhallaDamage = iDamage;
			}
		} break;

		case Skills::HallofValhalla:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Hall of Valhalla]" );
			pcUserData->dwTriumphOfValhallaEndTime = 0;
			pcUserData->iTriumphOfValhallaDamage = 0;
			int iTime = SQLSKILL->GetSkillValueInt( "SELECT Duration FROM HallofValhalla WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->HallOfValhallaTime = ( iTime * 1000 ) + TICKCOUNT;
			iValue = SQLSKILL->GetSkillValueInt( "SELECT AddedEvasionRating FROM HallofValhalla WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->HallOfValhallaParam[ 0 ] = iValue;
			int iLevelTOH = *( UINT* )( packet + 0x10 );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT AddedMaxAtkPower FROM TriumphofValhalla WHERE SkillLevel=%d", iLevelTOH );
			pcUserData->HallOfValhallaParam[ 1 ] = iValue;
			
			//Party it's active?
			if( pcUser->bParty && pcUser->psParty )
			{
				std::vector<User*> vUsers = PARTYHANDLER->GetPartyMembers( pcUser->psParty );
				for( auto &p : vUsers )
				{
					if( p )
					{
						if( p == pcUser )
							continue;

						if( p->GetMapID() == pcUser->GetMapID() )
						{
							//Calculate Range of buff
							int iXPosition = ( pcUserData->sPosition.iX - p->pcUserData->sPosition.iX ) >> 8;
							int iZPosition = ( pcUserData->sPosition.iZ - p->pcUserData->sPosition.iZ ) >> 8;
							int iYPosition = abs( pcUserData->sPosition.iY - p->pcUserData->sPosition.iY ) >> 8;
							int iXZCalc = abs( ( iXPosition * iXPosition ) - ( iZPosition * iZPosition ) );

							//Is in Area?
							if( ( iXZCalc > 90000 ) || ( iYPosition > 80 ) )
								continue;

							//Debug
							CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "XZ: %d, Y: %d, Target: %s", iXZCalc, iYPosition, p->pcUserData->sCharacterData.szName );

							//Send Buff to Client
							*(UINT*)( packet + 8 ) = JOBCODEID_Atalanta;
							p->pcUserData->HallOfValhallaTime = (iTime * 1000) + TICKCOUNT;
							p->pcUserData->HallOfValhallaParam[0] = pcUserData->HallOfValhallaParam[0] / 2;
							p->pcUserData->HallOfValhallaParam[1] = pcUserData->HallOfValhallaParam[1] / 2;
							SENDPACKET( p, (void*)packet );
						}
					}
				}
			}
		} break;

		case Skills::FrostJavelin:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Frost Javelin]" );
			int iTime = SQLSKILL->GetSkillValueInt( "SELECT Duration FROM HallofValhalla WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->FrostJavelinTime = ( iTime * 1000 ) + TICKCOUNT;
			pcUserData->FrostJavelinParam = iLevelSkill - 1;
		} break;

		case Skills::SummonTiger:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Summon Tiger]" );

			if ( MAPSERVER->IsSummonMap( pcUser->GetMapID() ) == FALSE )
			{
				CHATSERVER->SendChatMessageBox( pcUser, "You can't summon a pet on this map" );
				break;
			}

			WRITEDWORD( 0x08B81830, TRUE );

			UnitData * pcUnitData = MAPSERVER->SpawnMonster( "Summon Tiger", pcUser->GetMapID(), pcUserData->sPosition, pcUser );

			if ( pcUnitData )
			{
				pcUnitData->sUnitInfo.dwHashCharacterName = 0;
				pcUnitData->sCharacterData.iMonsterType = MONSTERTYPE_Friendly;
				pcUnitData->sCharacterData.iOwnerID = pcUserData->iID;

				pcUnitData->sCharacterData.sHP.sCur			= iaSummonTigerHP[iLevelSkill - 1];
				pcUnitData->sCharacterData.sHP.sMax			= iaSummonTigerHP[iLevelSkill - 1];
				pcUnitData->sCharacterData.iMinDamage		= iaSummonTigerAttackPower[iLevelSkill - 1];
				pcUnitData->sCharacterData.iMaxDamage		= iaSummonTigerAttackPower[iLevelSkill - 1];
				pcUnitData->sCharacterData.iAttackRating	= iaSummonTigerAttackRating[iLevelSkill - 1];

				pcUnitData->pcOwner = pcUserData;
				pcUnitData->dwLastUpdateTime = TICKCOUNT + (300 * 1000);

				pcUnitData->sCharacterData.sSize = 224 + iLevelSkill * 15;

				pcUnitData->saMoveStepCount[0] = 1;
				pcUnitData->saMoveStepCount[1] = 2;

				pcUserData->pcPetData = pcUnitData;
				pcUserData->dwObjectPetID = pcUnitData->iID;
				pcUnitData->bAggressive = FALSE;
				pcUnitData->iPetID = SKILLID_SummonTiger;

				ITEMSERVER->OnUseCrystal( pcUserData, pcUnitData->iID, SKILLID_SummonTiger, pcUnitData->dwLastUpdateTime );

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


CAtalanta::~CAtalanta()
{
}
