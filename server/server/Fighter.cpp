#include "stdafx.h"
#include "Fighter.h"
#include "HNSSkill.h"


CFighter::CFighter()
{
}


int CFighter::AddBerserker( UserData * pcUserData, int TypeDamage )
{

	

	UINT uTime = pcUserData->dwBerserkerEndTime;
	if ( uTime >= TICKCOUNT )
	{
		int iValue = pcUserData->iBerserkerParam;
		if ( TypeDamage == 1 )
		{
			int iAtkMin = (pcUserData->sCharacterData.iMinDamage * iValue) / 100;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Berserker Damage Min [%d + %d]", pcUserData->sCharacterData.iMinDamage, iAtkMin );
			return iAtkMin;
		}
		else
		{
			int iAtkMax = (pcUserData->sCharacterData.iMaxDamage * iValue) / 100;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Berserker Damage Max [%d + %d]", pcUserData->sCharacterData.iMaxDamage, iAtkMax );
			return iAtkMax;
		}
	}
	return 0;
}

int CFighter::RecvSkillSingleTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageNormalEx * psPacketEx )
{
	UINT uSkillID = psPacketEx->iSkillID & 0x000000FF;
	UINT uUnkV = ( psPacketEx->iSkillID / 4096 ) & 0x0000000F;
	int iLevelSkill = ( psPacketEx->iSkillID / 256 ) & 0x0000000F;

	User * pcUser = USERDATATOUSER( pcUserData );

	if ( uSkillID != 0 && iLevelSkill <= 0 || iLevelSkill > 10 )
		return 0;

	*( UINT* )0x7ACA048 = uSkillID;

	int iValue = 0;

	switch ( uSkillID )
	{
		case Skills::DefaultAttack:
		{
			int iDamageBoost = SQLSKILL->GetMinMaxInt( pcUserData->sCharacterData.iMinDamage, pcUserData->sCharacterData.iMaxDamage, 0 );
			psPacket->iDamage = psPacket->iDamage;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Default Attack]" );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
		} break;
		case Skills::Raving:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Raving]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM Raving WHERE SkillLevel=%d", iLevelSkill );
			psPacket->iDamage		+= ((psPacket->iDamage * iValue) / 100);
			psPacket->iDamageState	= (psPacket->iDamageState & 0xFFFF0000) + 2;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
		} break;



		case Skills::Impact:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Impact]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM Impact WHERE SkillLevel=%d", iLevelSkill );
			psPacket->iDamage += ( ( psPacket->iDamage * iValue ) / 100 );
			psPacket->iDamageState = ( psPacket->iDamageState & 0xFFFF0000 ) + 2;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
		} break;

		case Skills::TripleImpact:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Triple Impact]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM TripleImpact WHERE SkillLevel=%d", iLevelSkill );
			psPacket->iDamage += ( ( psPacket->iDamage * iValue ) / 100 );
			psPacket->iDamageState = ( psPacket->iDamageState & 0xFFFF0000 ) + 2;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );


		} break;

		case Skills::BrutalSwing:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Brutal Swing]" );
			int iValueCritical = SQLSKILL->GetSkillValueInt( "SELECT AddedCritical FROM BrutalSwing WHERE SkillLevel=%d", iLevelSkill );
			int iCritBoost = pcUserData->sCharacterData.iCritical + ( ( pcUserData->sCharacterData.iCritical * iValueCritical ) / 100 );
			psPacketEx->saStatus[ 0 ] = iCritBoost;
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM BrutalSwing WHERE SkillLevel=%d", iLevelSkill );
			if ( pcUnitData )
			{
				if ( pcUnitData->sCharacterData.iMonsterType == MONSTERTYPE_Demon )
				{
					iValue += 40;
					CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> +40%% Demon Monster" );
				}
			}
			psPacket->iDamage += ( ( psPacket->iDamage * iValue ) / 100 );
			psPacket->iDamageState = ( psPacket->iDamageState & 0xFFFF0000 ) + 2;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", iCritBoost, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );

		} break;

		case Skills::RageOfZecram:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Rage Of Zecram]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM RageofZecram WHERE SkillLevel=%d", iLevelSkill );
			psPacket->iDamage += ( ( psPacket->iDamage * iValue ) / 100 );
			psPacket->iDamageState = ( psPacket->iDamageState & 0xFFFF0000 ) + 2;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
		}break;

		case Skills::AvengingCrash:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Avenging Crash]" );
			int iValueCritical = SQLSKILL->GetSkillValueInt( "SELECT AddedCritical FROM BrutalSwing WHERE SkillLevel=%d", pcUser->pData.iLevelSkill[6 - 1] );
			int iCritBoost = pcUserData->sCharacterData.iCritical + (((pcUserData->sCharacterData.iCritical * (iValueCritical / 2)) / 100));
			psPacketEx->saStatus[0] = iCritBoost;
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM AvengingCrash WHERE SkillLevel=%d", iLevelSkill );
			psPacket->iDamage += ((psPacket->iDamage * iValue) / 100);
			psPacket->iDamageState = (psPacket->iDamageState & 0xFFFF0000) + 2;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", iCritBoost, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );

		} break;

		case Skills::SesmicImpact:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Sesmic Impact]" );
			psPacketEx->saStatus[0] = pcUserData->sCharacterData.iCritical;
			iValue = iaFighterT52DMGBoost[iLevelSkill-1];
			if ( pcUnitData )
			{
				iValue += 30;

				CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> +30%% Monster" );
			}

			int iValueCritical = SQLSKILL->GetSkillValueInt( "SELECT AddedCritical FROM BrutalSwing WHERE SkillLevel=%d", pcUser->pData.iLevelSkill[6 - 1] );
			int iCritBoost = pcUserData->sCharacterData.iCritical + (((pcUserData->sCharacterData.iCritical * (iValueCritical / 2)) / 100));
			psPacketEx->saStatus[0] = iCritBoost;

			psPacket->iDamage += ((psPacket->iDamage * iValue) / 100);
			psPacket->iDamageState = (psPacket->iDamageState & 0xFFFF0000) + 2;

			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", psPacketEx->saStatus[0], psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );

		} break;

		case Skills::BoneCrash:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Bone Crash]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM BoneCrash WHERE SkillLevel=%d", iLevelSkill );
			int iValueDemon = SQLSKILL->GetSkillValueInt( "SELECT DamageBoostVsDemons FROM BoneCrash WHERE SkillLevel=%d", iLevelSkill );
			int iDamageBoost = psPacket->iDamage;
			if ( pcUnitData )
			{
				if ( pcUnitData->sCharacterData.iMonsterType == 0x92 )
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

		case Skills::Destroyer:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Destroyer]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM Destroyer WHERE SkillLevel=%d", iLevelSkill );
			int iValueCritical = SQLSKILL->GetSkillValueInt( "SELECT AddedCritical FROM Destroyer WHERE SkillLevel=%d", iLevelSkill );
			int iCritBoost = pcUserData->sCharacterData.iCritical + ( ( pcUserData->sCharacterData.iCritical * iValueCritical ) / 100 );
			psPacketEx->saStatus[ 0 ] = iCritBoost;
			int iDamageBoost = psPacket->iDamage;
			if ( pcUnitData )
			{
				if ( pcUnitData->sCharacterData.iMonsterType == 0x92 )
				{
					psPacket->iDamage = iDamageBoost + ( ( iDamageBoost * ( 100 + iValue ) / 100 ) );
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


			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", iCritBoost, pcUserData->sCharacterData.iAttackRating );
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

int CFighter::RecvSkillAreaTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageRange * psPacketRange, PacketSkillDamageRangeEx * psPacketRangeEx )
{
	UINT uSkillID = psPacketRangeEx->iSkillID & 0x000000FF;
	UINT uUnkV = ( psPacketRangeEx->iSkillID / 4096 ) & 0x0000000F;
	int iLevelSkill = ( psPacketRangeEx->iSkillID / 256 ) & 0x0000000F;

	User * pcUser = USERDATATOUSER( pcUserData );

	if ( iLevelSkill <= 0 || iLevelSkill > 10 )
		return 0;

	*(UINT*)0x7ACA048 = psPacketRangeEx->saStatus[0];

	int iValue = 0;

	switch ( uSkillID )
	{
		case Skills::Roar:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Roar]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT Range FROM Roar WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iRange = iValue;
			iValue = SQLSKILL->GetSkillValueInt( "SELECT StunTime FROM Roar WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iDamageState = 2;
			psPacketRange->iDamage = iValue;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Range = %d , Time = %d", psPacketRange->iRange, psPacketRange->iDamage );
		} break;

		case Skills::RageOfZecram:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Rage Of Zecram]" );
			psPacketRange->iRange = 0x46;
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM RageofZecram WHERE SkillLevel=%d", iLevelSkill );
			int iDamageBoost = psPacketRange->iDamage;
			psPacketRange->iDamage = iDamageBoost + ((iDamageBoost * iValue) / 100);
			psPacketRange->iDamageState = 100;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketRangeEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
			psPacket->iDamageState = psPacket->iDamageState | 0x00030000;
		}break;

		case Skills::SesmicImpact: //Area Last Hit
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Sesmic Impact]" );
			psPacketRange->iRange = iaFighterT52Area[iLevelSkill-1];
			iValue = iaFighterT52DMGBoost[iLevelSkill-1];
			int iDamageBoost = psPacketRange->iDamage;
			psPacketRange->iDamage = iDamageBoost + ((iDamageBoost * iValue) / 100);
			psPacketRange->iDamageState = 100;

			int iValueCritical = SQLSKILL->GetSkillValueInt( "SELECT AddedCritical FROM BrutalSwing WHERE SkillLevel=%d", pcUser->pData.iLevelSkill[6 - 1] );
			int iCritBoost = pcUserData->sCharacterData.iCritical + (((pcUserData->sCharacterData.iCritical * (iValueCritical / 2)) / 100));
			psPacketRangeEx->saStatus[0] = iCritBoost;

			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketRangeEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
			psPacket->iDamageState = psPacket->iDamageState | 0x00030000;
		}break;

		case Skills::CycloneStrike:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Cyclone Strike]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM CycloneStrike WHERE SkillLevel=%d", iLevelSkill );
			int iDamageBoost = psPacketRange->iDamage;
			psPacket->iDamage = iDamageBoost + ( ( iDamageBoost * iValue ) / 100 );
			psPacketRange->iDamage = psPacket->iDamage;
			iValue = SQLSKILL->GetSkillValueInt( "SELECT Range FROM CycloneStrike WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iRange = iValue;
			iValue = SQLSKILL->GetSkillValueInt( "SELECT Enemies FROM CycloneStrike WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iCount = iValue;
			psPacketRange->iDamageState = 0;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage = %d, Range = %d", psPacket->iDamage, psPacketRange->iRange );
			*( UINT* )0x07ACA04C = -200;
			psPacketRange->iCount &= 0x000000FF;
		}break;

		default:
			break;
	}

	return 1;
}

int CFighter::RecvBuffSkill( UserData * pcUserData, UINT packet )
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
		case Skills::Berserker:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Berserker]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT AddedAtkPower FROM Berserker WHERE SkillLevel=%d", iLevelSkill );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "%d", iValue );
			pcUserData->iBerserkerParam = iValue;
			iValue = SQLSKILL->GetSkillValueInt( "SELECT Duration FROM Berserker WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->dwBerserkerEndTime = ( iValue * 1000 ) + TICKCOUNT;
		} break;

		default:
			break;
	}
	return 1;
}


CFighter::~CFighter()
{
}
