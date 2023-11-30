#include "stdafx.h"
#include "Archer.h"
#include "HNSSkill.h"

typedef UnitData*( __cdecl *t_SummonMobSkill ) ( int iSkillID, int X, int Y, int Z, UserData * pcUserData, int v0 );
t_SummonMobSkill SummonMobSkill = ( t_SummonMobSkill )0x0055E5A0;

DWORD dwLastSkillArcherID = 0;

CArcher::CArcher()
{
}


int CArcher::AddForceOfNature( UserData * pcUserData, int TypeDamage )
{
	if ( pcUserData->dwForceOfNatureEndTime >= TICKCOUNT )
	{
		int iValue = pcUserData->saForceOfNatureParam[ 0 ];
		if ( TypeDamage == 1 )
		{
			int iAtkMin = pcUserData->sCharacterData.iMinDamage + iValue;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Force Of Nature Damage Min [%d + %d]", pcUserData->sCharacterData.iMinDamage, iValue );
			return iValue;
		}
		else
		{
			int iAtkMax = pcUserData->sCharacterData.iMaxDamage + iValue;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Force Of Nature Damage Max [%d + %d]", pcUserData->sCharacterData.iMaxDamage, iValue );
			return iValue;
		}
	}
	return 0;
}

int CArcher::RecvSkillSingleTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageNormalEx * psPacketEx )
{
	UINT uSkillID = psPacketEx->iSkillID & 0x000000FF;
	UINT uUnkV = ( psPacketEx->iSkillID / 4096 ) & 0x0000000F;
	int iLevelSkill = ( psPacketEx->iSkillID / 256 ) & 0x0000000F;

	

	if ( uSkillID != 0 && iLevelSkill <= 0 || iLevelSkill > 10 )
		return 0;

	int iValue = 0;

	dwLastSkillArcherID = uSkillID;

	User * pcUser = USERDATATOUSER( pcUserData );

	//Lethal Sight
	if ( pcUser->dwLethalSightTimeOut > TICKCOUNT )
	{
	//	if ( (dwLastSkillArcherID != Skills::Falcon) && (dwLastSkillArcherID != Skills::GoldenFalcon) )
	//		*(UINT*)0x08B80ACC = pcUser->uLethalSightCriticalBoost;
	}

	switch ( uSkillID )
	{
		case Skills::DefaultAttack:
		{
			int iDamageBoost = psPacket->iDamage;
			psPacket->iDamage = iDamageBoost;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Default Attack]" );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
		} break;
		case Skills::WindArrow:
		{

			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Wind Arrow]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM WindArrow WHERE SkillLevel=%d", iLevelSkill );
			int iDamageBoost = psPacket->iDamage;
			if ( pcUnitData )
			{
				if ( pcUnitData->sCharacterData.iMonsterType == MONSTERTYPE_Normal )
					iValue += 30;
			}
			psPacket->iDamage = iDamageBoost + ( ( iDamageBoost * iValue ) / 100 );
			psPacket->iDamageState = ( psPacket->iDamageState & 0xFFFF0000 ) + 2;

			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );

		} break;

		case Skills::PerfectAim:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Perfect Aim]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM PerfectAim WHERE SkillLevel=%d", iLevelSkill );
			int iDamageBoost = psPacket->iDamage;
			if ( pcUnitData )
			{
				if ( pcUnitData->sCharacterData.iMonsterType == MONSTERTYPE_Demon )
					iValue += 30;
			}
			psPacket->iDamage = iDamageBoost + ( ( iDamageBoost * iValue ) / 100 );
			psPacket->iDamageState = ( psPacket->iDamageState & 0xFFFF0000 ) + 2;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );

		}break;

		case Skills::Falcon:
		{
			if ( pcUserData->dwFalconEndTime > TICKCOUNT )
			{
				CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Falcon]" );
				iValue = SQLSKILL->GetSkillValueInt( "SELECT Damage FROM Falcon WHERE SkillLevel=%d", iLevelSkill );
				int iStaticDamage = SQLSKILL->GetSkillValueInt( "SELECT StaticDamage FROM Falcon WHERE SkillLevel=%d", iLevelSkill );
				int iDamageBoost = psPacket->iDamage;
				psPacket->iDamage = iStaticDamage + ( ( iDamageBoost * iValue ) / 100 );
				psPacket->iDamageState = ( psPacket->iDamageState & 0xFFFF0000 ) + 2;
				if ( pcUserData->saFalconDamage[0] == 2 )
				{
					iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM GoldenFalcon WHERE SkillLevel=%d", iLevelSkill );
					iDamageBoost = psPacket->iDamage;
					psPacket->iDamage = ( ( iDamageBoost * iValue ) / 100 );
					CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Golden Falcon Damage: %d", ( ( iDamageBoost * iValue ) / 100 ) );
				}
				CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
				CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );

				*(UINT*)0x08B80ACC = 0;
			}
		}break;

		case Skills::ArrowofRage: // Area Skill

		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Arrow of Rage]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM ArrowofRage WHERE SkillLevel=%d", iLevelSkill );
			int iDamageBoost = psPacket->iDamage;
			psPacket->iDamage = ( ( iDamageBoost * iValue ) / 100 );
			psPacket->iDamageState = ( psPacket->iDamageState & 0xFFFF0000 ) + 2;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );

		}break;

		case Skills::ElementalShot: //Area no elemento Raio
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Elemental Shot]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT Damage FROM ElementalShot WHERE SkillLevel=%d", iLevelSkill );
			int iStaticDamage = SQLSKILL->GetSkillValueInt( "SELECT StaticDamage FROM ElementalShot WHERE SkillLevel=%d", iLevelSkill );
			int iDamageBoost = psPacket->iDamage;
			psPacket->iDamage = ( ( iDamageBoost * iValue ) / 100 ) + iStaticDamage;
		//	psPacket->AttackState = ( psPacket->AttackState & 0xFFFF0000 ) + 2;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );

		}break;

		case Skills::BombShot: // Area skill
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Bomb Shot]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM BombShot WHERE SkillLevel=%d", iLevelSkill );
			int iValueDemon = SQLSKILL->GetSkillValueInt( "SELECT DamageBoostVsDemons FROM BombShot WHERE SkillLevel=%d", iLevelSkill );
			int iDamageBoost = psPacket->iDamage;
			if ( pcUnitData )
			{
				if ( pcUnitData->sCharacterData.iMonsterType == MONSTERTYPE_Demon )
				{
					psPacket->iDamage = iDamageBoost + ( ( iDamageBoost * iValueDemon ) / 100 );
					psPacket->iDamageState = ( psPacket->iDamageState & 0xFFFF0000 ) + 2;
				}
				else
				{
					psPacket->iDamage = iDamageBoost + ( ( iDamageBoost * iValue ) / 100 );
					psPacket->iDamageState = ( psPacket->iDamageState & 0xFFFF0000 ) + 2;
				}
			}
			else
			{
				psPacket->iDamage = iDamageBoost + ( ( iDamageBoost * iValue ) / 100 );
				psPacket->iDamageState = ( psPacket->iDamageState & 0xFFFF0000 ) + 2;
			}
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
		} break;

		case Skills::Perforation: // Area Skill
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Perforation]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM Perforation WHERE SkillLevel=%d", iLevelSkill );
			int iValueCritical = SQLSKILL->GetSkillValueInt( "SELECT AddedCritical FROM Perforation WHERE SkillLevel=%d", iLevelSkill );
			int iCritBoost = pcUserData->sCharacterData.iCritical;
			psPacketEx->saStatus[ 0 ] = iCritBoost;
			int iDamageBoost = psPacket->iDamage;
			psPacket->iDamage = ( ( iDamageBoost * iValue ) / 100 );
			psPacket->iDamageState = ( psPacket->iDamageState & 0xFFFF0000 ) + 2;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
			*( UINT* )0x07ACA04C = psPacketEx->WeaponAttackPower[ 0 ];
		}break;

		case Skills::PhoenixShot: // Area Skill qnd Chargeada
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Phoenix Shot]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM PhoenixShot WHERE SkillLevel=%d", iLevelSkill );
			int iDamageBoost = psPacket->iDamage;
			psPacket->iDamage = iDamageBoost + ( ( iDamageBoost * iValue ) / 100 );
			psPacket->iDamageState = ( psPacket->iDamageState & 0xFFFF0000 ) + 2;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );

		}break;

		case Skills::Avalanche:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Avalanche]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM Avalanche WHERE SkillLevel=%d", iLevelSkill );
			int iDamageBoost = psPacket->iDamage;
			psPacket->iDamage = iDamageBoost + ( ( iDamageBoost * iValue ) / 100 );
			psPacketEx->saStatus[ 0 ] = psPacketEx->saStatus[ 0 ] >> 1;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );

		}break;

		//Magnetic Discharge
		case 0x8C:
			CMechanician::DoMagneticDischargeDamage( USERDATATOUSER( pcUserData ), pcUnitData, psPacket, psPacketEx );
			break;

		default:
			break;
	}
	return 1;
}

int CArcher::RecvSkillAreaTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageRange * psPacketRange, PacketSkillDamageRangeEx * psPacketRangeEx )
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
		case Skills::ArrowofRage:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Arrow of Rage]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM ArrowofRage WHERE SkillLevel=%d", iLevelSkill );
			int iDamageBoost = psPacketRange->iDamage;
			psPacketRange->iDamage = ( ( iDamageBoost * iValue ) / 100 );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT Area FROM ArrowofRage WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iRange = iValue;
			psPacketRange->iDamageState = 0;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", iValue, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
		} break;
		case Skills::ElementalShot:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Elemental Shot]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT Damage FROM ElementalShot WHERE SkillLevel=%d", iLevelSkill );
			int iStaticDamage = SQLSKILL->GetSkillValueInt( "SELECT StaticDamage FROM ElementalShot WHERE SkillLevel=%d", iLevelSkill );
			int iDamageBoost = psPacketRange->iDamage;
			psPacketRange->iDamage = ( ( iDamageBoost * iValue ) / 100 ) + iStaticDamage;
			iValue = SQLSKILL->GetSkillValueInt( "SELECT Range FROM ElementalShot WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iRange = iValue;
			psPacketRange->iDamageState = 0x69;
			psPacket->iDamageState |= 0x00050000;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", iValue, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
		} break;

		case Skills::BombShot:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Bomb Shot]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM BombShot WHERE SkillLevel=%d", iLevelSkill );
			int iValueDemon = SQLSKILL->GetSkillValueInt( "SELECT DamageBoostVsDemons FROM BombShot WHERE SkillLevel=%d", iLevelSkill );
			int iDamageBoost = psPacketRange->iDamage;
			if ( pcUnitData )
			{
				if ( pcUnitData->sCharacterData.iMonsterType == MONSTERTYPE_Demon )
				{
					psPacket->iDamage = iDamageBoost + ( ( iDamageBoost * iValueDemon ) / 100 );
					psPacket->iDamageState = ( psPacket->iDamageState & 0xFFFF0000 ) + 2;
				}
				else
				{
					psPacket->iDamage = iDamageBoost + ( ( iDamageBoost * iValue ) / 100 );
					psPacket->iDamageState = ( psPacket->iDamageState & 0xFFFF0000 ) + 2;
				}
			}
			else
			{
				psPacket->iDamage = iDamageBoost + ( ( iDamageBoost * iValue ) / 100 );
				psPacket->iDamageState = ( psPacket->iDamageState & 0xFFFF0000 ) + 2;
			}
			psPacketRange->iDamage = psPacket->iDamage;
			psPacketRange->iRange = 100;
			psPacketRange->iDamageState = 0;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", iValue, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
		} break;

		case Skills::Perforation:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Perforation]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM Perforation WHERE SkillLevel=%d", iLevelSkill );
			int iValueCritical = SQLSKILL->GetSkillValueInt( "SELECT AddedCritical FROM Perforation WHERE SkillLevel=%d", iLevelSkill );
			int iCritBoost = pcUserData->sCharacterData.iCritical;
			psPacketRangeEx->saStatus[0] = iCritBoost;
			int iDamageBoost = psPacketRange->iDamage;
			psPacketRange->iDamage = iDamageBoost + ( ( iDamageBoost * iValue ) / 100 );
			psPacketRange->iDamageState = 0;
			iValue = SQLSKILL->GetSkillValueInt( "SELECT Area FROM Perforation WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iRange = iValue + 340;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", psPacketRangeEx->saStatus[0], pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d] Range[%d]", psPacketRange->iDamage, iValue );
			*( UINT* )0x07ACA04C = psPacketRangeEx->WeaponAttackPower[ 0 ];
			psPacketRange->iCount &= 0x000000FF;
		}break;

		case Skills::PhoenixShot: // Area Skill qnd Chargeada
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Phoenix Shot]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM PhoenixShot WHERE SkillLevel=%d", iLevelSkill );
			int iDamageBoost = psPacketRange->iDamage;
			psPacketRange->iDamage = iDamageBoost + ((iDamageBoost * iValue) / 100);
			psPacketRange->iDamageState = 0;
			psPacketRange->iRange += 120;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
			psPacketRange->iCount &= 0x000000FF;

		}break;
		case Skills::FierceWind: //Same as PHShot...
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Fierce Wind]" );
			iValue = iaFierceWindDamageBoost[iLevelSkill-1];
			int iDamageBoost = psPacketRange->iDamage;
			psPacketRange->iDamage = iDamageBoost + ((iDamageBoost * iValue) / 100);
			psPacketRange->iDamageState = 0;
			psPacketRange->iRange = 120 * 2;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
			psPacketRange->iCount &= 0x000000FF;

		}break;


		default:
			break;
	}
	return 1;
}

int CArcher::RecvBuffSkill( UserData * pcUserData, UINT packet )
{
	UINT uSkillID = *( UINT* )( packet + 0x0C ) & 0x000000FF;
	int iLevelSkill = ( ( *( int* )( packet + 0x0C ) / 256 ) & 0x0000000F ) - 1;

	PacketSkillCast * psPacket = (PacketSkillCast *)packet;

	User * pcUser = USERDATATOUSER( pcUserData );
	User * pcUserTarget = USERDATATOUSER( USERDATABYID( psPacket->iID ) );
	Unit * pcUnitTarget = UNITDATATOUNIT( UNITDATABYIDMAP( psPacket->iID, pcUserData->iMapID ) );

	if ( iLevelSkill < 0 || iLevelSkill > 10 )
	{
		iLevelSkill = *( int* )( packet + 0x08 ) - 1;
		if ( iLevelSkill < 0 || iLevelSkill > 10 )
			return 1;
	}
	iLevelSkill += 1;

	int iValue = 0;

	PacketUseSkillNew sPacketSkillNew;

	switch ( uSkillID )
	{
		case Skills::GoldenFalcon:
		{
			if ( ( *( UINT* )( packet + 0x10 ) < 0 ) || ( *( UINT* )( packet + 0x10 ) > 10 ) )
				break;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Golden Falcon]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT Duration FROM GoldenFalcon WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->dwFalconEndTime = ( iValue * 1000 ) + TICKCOUNT;
			pcUserData->saFalconDamage[ 0 ] = 2;
		} break;

		case Skills::Falcon:
		{
			if ( ( *( UINT* )( packet + 0x10 ) < 0 ) || ( *( UINT* )( packet + 0x10 ) > 10 ) )
				break;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Falcon]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT Duration FROM Falcon WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->dwFalconEndTime = ( iValue * 1000 ) + TICKCOUNT;
			pcUserData->saFalconDamage[ 0 ] = 1;
		} break;


		case Skills::ForceofNature:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Force of Nature]" );
			int iTime = SQLSKILL->GetSkillValueInt( "SELECT Duration FROM ForceofNature WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->dwForceOfNatureEndTime = ( iTime * 1000 ) + TICKCOUNT;
			iValue = SQLSKILL->GetSkillValueInt( "SELECT AddedAtkPow FROM ForceofNature WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->saForceOfNatureParam[0] = iValue;
			iValue = SQLSKILL->GetSkillValueInt( "SELECT AddedRange FROM ForceofNature WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->saForceOfNatureParam[1] = iValue;
			
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
							int iXPosition = (pcUserData->sPosition.iX - p->pcUserData->sPosition.iX) >> 8;
							int iZPosition = (pcUserData->sPosition.iZ - p->pcUserData->sPosition.iZ) >> 8;
							int iYPosition = abs(pcUserData->sPosition.iY - p->pcUserData->sPosition.iY) >> 8;
							int iXZCalc = abs((iXPosition * iXPosition) - (iZPosition * iZPosition ));

							//Is in Area?
							if( (iXZCalc > 90000) || (iYPosition > 80) )
								continue;

							//Debug
							CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "XZ: %d, Y: %d, Target: %s", iXZCalc, iYPosition, p->pcUserData->sCharacterData.szName );

							//Send Buff to Client
							*(UINT*)( packet + 8 ) = JOBCODEID_Archer;
							p->pcUserData->dwForceOfNatureEndTime = (iTime * 1000) + TICKCOUNT;
							p->pcUserData->saForceOfNatureParam[0] = pcUserData->saForceOfNatureParam[0] / 2;
							p->pcUserData->saForceOfNatureParam[1] = pcUserData->saForceOfNatureParam[0] / 2;
							SENDPACKET( p, (void*)packet );
						}
					}
				}
			}
		} break;

		case Skills::RecallWolverine:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Recall Wolverine]" );

			if ( MAPSERVER->IsSummonMap( pcUser->GetMapID() ) == FALSE )
			{
				CHATSERVER->SendChatMessageBox( pcUser, "You can't summon a pet on this map" );
				break;
			}

			UnitData * pcUnitDataPet = SummonMobSkill( 0x4D, pcUserData->sPosition.iX, pcUserData->sPosition.iY, pcUserData->sPosition.iZ, pcUserData, 0 );
			
			if ( pcUnitDataPet )
			{
				pcUnitDataPet->dwLastUpdateTime = ( 300 * 1000 ) + TICKCOUNT;
				
				iValue = SQLSKILL->GetSkillValueInt( "SELECT AttackPower FROM RecallWolverine WHERE SkillLevel=%d", iLevelSkill );
				pcUnitDataPet->sCharacterData.iMinDamage = iValue;
				pcUnitDataPet->sCharacterData.iMaxDamage = iValue;
				
				iValue = SQLSKILL->GetSkillValueInt( "SELECT AttackRating FROM RecallWolverine WHERE SkillLevel=%d", iLevelSkill );
				pcUnitDataPet->sCharacterData.iAttackRating = iValue;
				
				iValue = SQLSKILL->GetSkillValueInt( "SELECT Defense FROM RecallWolverine WHERE SkillLevel=%d", iLevelSkill );
				pcUnitDataPet->sCharacterData.iDefenseRating = iValue + ( pcUserData->sCharacterData.iAgility * 2 );
				
				iValue = SQLSKILL->GetSkillValueInt( "SELECT HP FROM RecallWolverine WHERE SkillLevel=%d", iLevelSkill );
				pcUnitDataPet->sCharacterData.sHP.sCur = ( WORD )iValue;
				pcUnitDataPet->sCharacterData.sHP.sMax = ( WORD )iValue;
				
				pcUnitDataPet->sCharacterData.sSize = 192 + iLevelSkill * 12;
				
				pcUnitDataPet->saMoveStepCount[ 0 ] = 1;
				pcUnitDataPet->saMoveStepCount[ 1 ] = 2;
			}
		} break;

		case SKILLPLAYID_T51:
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Lethal Sight]" );

			pcUser->uLethalSightLevel = iLevelSkill;
			pcUser->dwLethalSightTimeOut = (300 * 1000) + TICKCOUNT;
			pcUser->uLethalSightCritical = iaArcherT51CritRate[iLevelSkill - 1];
			pcUser->uLethalSightDamageBoost = iaArcherT51DMGBoost[iLevelSkill - 1];

			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Time[%d] Level[%d]", 300, pcUser->uLethalSightLevel );

			sPacketSkillNew.iLength = sizeof( PacketUseSkillNew );
			sPacketSkillNew.iHeader = PKTHDR_UseSkillNew;
			sPacketSkillNew.iSkillID = SKILLID_LethalSight;
			sPacketSkillNew.iSkillLevel = pcUser->uLethalSightLevel;
			sPacketSkillNew.lCasterID = pcUser->GetID();
			sPacketSkillNew.lTargetID = pcUser->GetID();
			SENDPACKETRANGE( &sPacketSkillNew, &pcUserData->sPosition, DISTANCE_UNIT_VIEWLIMIT );
			break;

		case SKILLPLAYID_T52:
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Fierce Wind]" );

			if ( pcUserTarget )
			{
				pcUserTarget->uFierceWindLevel		= iLevelSkill;
				pcUserTarget->dwFierceWindTimeOut	= (iaFierceWindTimeOut[iLevelSkill - 1] * 1000) + TICKCOUNT;

				CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Time[%d] Level[%d]", iaFierceWindTimeOut[iLevelSkill - 1], pcUserTarget->uFierceWindLevel );

				sPacketSkillNew.iLength = sizeof( PacketUseSkillNew );
				sPacketSkillNew.iHeader = PKTHDR_UseSkillNew;
				sPacketSkillNew.iSkillID = SKILLID_FierceWind;
				sPacketSkillNew.iSkillLevel = pcUserTarget->uFierceWindLevel;
				sPacketSkillNew.lCasterID = pcUserData->iID;
				sPacketSkillNew.lTargetID = pcUserTarget->GetID();
				SENDPACKETRANGE( &sPacketSkillNew, &pcUserData->sPosition, DISTANCE_UNIT_VIEWLIMIT );
			}
			if ( pcUnitTarget )
			{
				DWORD dwTimeOut = (iaFierceWindTimeOut[iLevelSkill - 1] * 1000) + TICKCOUNT;

				CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Time[%d] Level[%d]", dwTimeOut, iLevelSkill );

				sPacketSkillNew.iLength = sizeof( PacketUseSkillNew );
				sPacketSkillNew.iHeader = PKTHDR_UseSkillNew;
				sPacketSkillNew.iSkillID = SKILLID_FierceWind;
				sPacketSkillNew.iSkillLevel = iLevelSkill;
				sPacketSkillNew.lCasterID = pcUserData->iID;
				sPacketSkillNew.lTargetID = pcUnitTarget->GetID();
				SENDPACKETRANGE( &sPacketSkillNew, &pcUserData->sPosition, DISTANCE_UNIT_VIEWLIMIT );
			}
			break;

		default:
			break;
	}
	return 1;
}

void CArcher::OnAddFinalDamage( UserData * pcUserData, PacketSkillDamageNormal * psPacket )
{
	if ( (dwLastSkillArcherID == Skills::Falcon) || (dwLastSkillArcherID == Skills::GoldenFalcon) )
	{
		dwLastSkillArcherID = 0;
		return;
	}

	if ( dwLastSkillArcherID > 0x6D )
	{
		dwLastSkillArcherID = 0;
		return;
	}

	if ( pcUserData )
	{
		User * pcUser = USERDATATOUSER( pcUserData );
		if ( pcUser )
		{
			if ( pcUser->dwLethalSightTimeOut > TICKCOUNT )
			{
				CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Lethal Sight Damage [%d + %d]", psPacket->iDamage, (psPacket->iDamage * pcUser->uLethalSightDamageBoost) / 100 );
				psPacket->iDamage += (psPacket->iDamage * pcUser->uLethalSightDamageBoost) / 100;

				pcUser->dwLethalSightTimeOut = 0;
				pcUser->uLethalSightLevel = 0;
				pcUserData->saVanishParam[1] = 0;
				pcUserData->dwAssassinEyeEndTime = 0;
				DAMAGEHANDLER->SendCancelSkill( pcUser, SKILLPLAYID_T51 );
			}
		}
	}

	dwLastSkillArcherID = 0;
}

CArcher::~CArcher()
{
}
