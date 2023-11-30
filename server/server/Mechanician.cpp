#include "stdafx.h"
#include "Mechanician.h"
#include "HNSSkill.h"


typedef void( __cdecl *t_SendDeBuffToPlayers ) ( UserData * pcUserData, void * packet );
t_SendDeBuffToPlayers SendDeBuffToPlayers = ( t_SendDeBuffToPlayers )0x00552210;

typedef UnitData*( __cdecl *t_SummonMobSkillMS ) ( int iSkillID, int X, int Y, int Z, UserData * pcUserData, int v0 );
t_SummonMobSkillMS SummonMobSkillMS = ( t_SummonMobSkillMS )0x0055E5A0;


CMechanician::CMechanician()
{
}

int CMechanician::AddMaximize( UserData * pcUserData, int TypeDamage )
{

	UINT uTime = pcUserData->dwMaximizeEndTime;
	if ( uTime >= TICKCOUNT )
	{
		int iValue = pcUserData->iMaximizeMaxDamageBoost;
		if ( TypeDamage == 1 )
		{
			int iAtkMin = pcUserData->sCharacterData.iMinDamage + ( ( pcUserData->sCharacterData.iMinDamage * iValue ) / 100 );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Maximize Damage Min [%d + %d] = %d", pcUserData->sCharacterData.iMinDamage, ( ( pcUserData->sCharacterData.iMinDamage * iValue ) / 100 ), iAtkMin );
			return ( ( pcUserData->sCharacterData.iMinDamage * iValue ) / 100 );
		}
		else
		{
			int iAtkMax = pcUserData->sCharacterData.iMaxDamage + ( ( pcUserData->sCharacterData.iMaxDamage * iValue ) / 100 );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Maximize Damage Max [%d + %d] = %d", pcUserData->sCharacterData.iMaxDamage, ( ( pcUserData->sCharacterData.iMaxDamage * iValue ) / 100 ), iAtkMax );
			return ( ( pcUserData->sCharacterData.iMaxDamage * iValue ) / 100 );
		}
	}
	return 0;
}

int CMechanician::AddAutomation( UserData * pcUserData, UnitData * pcUnitData, int TypeDamage, UINT packet )
{

	UINT uTime = pcUserData->dwAutomationEndTime;
	if ( uTime >= TICKCOUNT )
	{
		int iItemID = *( UINT* )( packet + 0x44 ) & 0xFFFF0000;

		if ( ( iItemID != 0x01060000 ) && ( iItemID != 0x01080000 ) )
			return 0;

		int iValue = pcUserData->iAutomationDamageBoost;
		if ( TypeDamage == 1 )
		{
			int iAtkMin = pcUserData->sCharacterData.iMinDamage + ( ( pcUserData->sCharacterData.iMinDamage * iValue ) / 100 );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Automation Damage Min [%d + %d] = %d", pcUserData->sCharacterData.iMinDamage, ( ( pcUserData->sCharacterData.iMinDamage * iValue ) / 100 ), iAtkMin );
			int iDamage = ((pcUserData->sCharacterData.iMinDamage * iValue) / 100);
			return iDamage;
		}
		else
		{
			int iAtkMax = pcUserData->sCharacterData.iMaxDamage + ( ( pcUserData->sCharacterData.iMaxDamage * iValue ) / 100 );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Automation Damage Max [%d + %d] = %d", pcUserData->sCharacterData.iMaxDamage, ( ( pcUserData->sCharacterData.iMaxDamage * iValue ) / 100 ), iAtkMax );
			int iDamage = ((pcUserData->sCharacterData.iMaxDamage * iValue) / 100);
			return iDamage;
		}
	}
	return 0;
}


int CMechanician::RecvSkillSingleTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageNormalEx * psPacketEx )
{
	UINT uSkillID = psPacketEx->iSkillID & 0x000000FF;
	UINT uUnkV = ( psPacketEx->iSkillID / 4096 ) & 0x0000000F;
	int iLevelSkill = ( psPacketEx->iSkillID / 256 ) & 0x0000000F;

	*( UINT* )0x7ACA048 = psPacketEx->iSkillID;

	if ( uSkillID != 0 && iLevelSkill <= 0 || iLevelSkill > 10 )
		return 0;

	int iDamageBoost = psPacket->iDamage;

	int iValue = 0;

	switch ( uSkillID )
	{
		case Skills::DefaultAttack:
		{
			UINT uTime = pcUserData->dwAutomationEndTime;
			if ( uTime >= TICKCOUNT && pcUnitData && pcUnitData->sCharacterData.iType == CHARACTERTYPE_Monster )
			{
				CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Automation +150%% Damage vs. Monster" );
				iDamageBoost += (iDamageBoost * 150) / 100;
			}
			psPacket->iDamage = iDamageBoost;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Default Attack]" );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
		} break;

		case Skills::GrandSmash:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Grand Smash]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM GrandSmash WHERE SkillLevel=%d", iLevelSkill );
			if ( pcUnitData )
			{
				psPacket->iDamage += ( ( iDamageBoost * ( iValue + 45 ) ) / 100 );
				CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> +45%% Monster" );
			}
			else
				psPacket->iDamage += ( ( iDamageBoost * iValue ) / 100 );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
		} break;


		case Skills::Impulsion:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Impulsion]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM Impulsion WHERE SkillLevel=%d", iLevelSkill );
			if ( pcUnitData )
			{
				if ( pcUnitData->sCharacterData.iMonsterType == 0x93 )
				{
					psPacket->iDamage += ( ( iDamageBoost * ( iValue + 30 ) ) / 100 );
					CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> +30%% Machine Monster" );
				}
				else
					psPacket->iDamage += ( ( iDamageBoost * iValue ) / 100 );
			}
			else
				psPacket->iDamage += ( ( iDamageBoost * iValue ) / 100 );
			psPacketEx->saStatus[ 0 ] = 0;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
		} break;

		case Skills::MagneticSphere:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Magnetic Sphere]" );
			UINT uTime = pcUserData->dwMagneticSphereEndTime;
			if ( uTime >= TICKCOUNT )
			{
				int iSkillLvlBuff = pcUserData->iMagneticSphereParam;
				iValue = SQLSKILL->GetSkillValueInt( "SELECT SphereDmg FROM MagneticSphere WHERE SkillLevel=%d", iSkillLvlBuff );
				psPacket->iDamage += ( ( iDamageBoost * iValue ) / 100 );
				if ( pcUnitData )
				{
					if ( pcUnitData->sCharacterData.iMonsterType == 0x93 )
					{
						psPacket->iDamage += ( (psPacket->iDamage * 30 ) / 100 );
						CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> +30%% Machine Monster" );
					}
				}
				psPacketEx->saStatus[ 0 ] = 0;
				psPacket->iDamageState = psPacket->iDamageState | 0x00050000;
			}
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
		} break;


		case Skills::SparkShield:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Spark Shield]" );
			UINT uTime = pcUserData->dwSparkShieldEndTime;
			if ( uTime >= TICKCOUNT )
			{
				int iDamagePercent = pcUserData->iSparkShieldDamage;
				psPacket->iDamage = iDamageBoost + ((iDamageBoost * iDamagePercent) / 100);
				if ( pcUnitData )
				{
					if ( pcUnitData->sCharacterData.iMonsterType == 0x93 )
					{
						psPacket->iDamage += ( psPacket->iDamage * 50 ) / 100;
					}
					psPacketEx->saStatus[ 0 ] = 0;
					psPacket->iDamageState = psPacket->iDamageState | 0x00050000;
					struct PacketBuff
					{
						int size;
						int op;
						int IDCodePlayer;
						int vSkillBuff;
						int IDCodeMonster;
						int null3;
					};
					struct PacketBuff pBuff;
					pBuff.size = sizeof( pBuff );
					pBuff.op = 0x50320A20;
					pBuff.IDCodePlayer =  pcUserData->iID;
					pBuff.IDCodeMonster = pcUnitData->iID;
					pBuff.null3 = 0;
					pBuff.vSkillBuff = 0x3C;
					SendDeBuffToPlayers( pcUserData, &pBuff );
					CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
				}
			}
		} break;

		case Skills::Rupture:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Rupture]" );
			
			iValue = iaMechT51DMGBoost[iLevelSkill-1];
			
			psPacket->iDamage += ((iDamageBoost * iValue) / 100);

			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
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

int CMechanician::RecvSkillAreaTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageRange * psPacketRange, PacketSkillDamageRangeEx * psPacketRangeEx )
{
	UINT uSkillID = psPacketRangeEx->iSkillID & 0x000000FF;
	UINT uUnkV = ( psPacketRangeEx->iSkillID / 4096 ) & 0x0000000F;
	int iLevelSkill = ( psPacketRangeEx->iSkillID / 256 ) & 0x0000000F;

	

	if ( iLevelSkill <= 0 || iLevelSkill > 10 )
		return 0;

	psPacket->iDamage = 0;

	int iDamageBoost = psPacketRange->iDamage;

	*( UINT* )0x7ACA048 = psPacketRangeEx->saStatus[0];

	int iValue = 0;

	switch ( uSkillID )
	{

		case Skills::MechanicBomb:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Mechanic Bomb]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT Damage FROM MechanicBomb WHERE SkillLevel=%d", iLevelSkill );
			int iStaticDamage = SQLSKILL->GetSkillValueInt( "SELECT StaticDamage FROM MechanicBomb WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iDamage = iStaticDamage + ( ( iDamageBoost * iValue ) / 100 );
			psPacket->iDamage = iStaticDamage + ( ( iDamageBoost * iValue ) / 100 );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT AreaRange FROM MechanicBomb WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iRange = iValue;
			psPacketRange->iDamageState = 0;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", iValue, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
		} break;

		case Skills::GreatSmash:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Great Smash]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM GreatSmash WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iDamage = ( ( iDamageBoost * iValue ) / 100 );
			psPacket->iDamage = ( ( iDamageBoost * iValue ) / 100 );
			psPacketRange->iRange = 70;
			psPacketRange->iDamageState = 0;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", 70, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
		} break;

		case Skills::Spark:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Spark]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT SparkDmg FROM Spark WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iDamage = iDamageBoost + ( ( iDamageBoost * iValue ) / 100 );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT AreaRange FROM Spark WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iRange = iValue;
			psPacketRange->iDamageState = 0;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", psPacketRange->iRange, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
		} break;

		case Skills::Impulsion:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Impulsion]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM Impulsion WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iDamage = iDamageBoost + ( ( iDamageBoost * iValue ) / 100 );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT Range FROM Impulsion WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iRange = iValue;
			psPacketRange->iDamageState = 101;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", psPacketRange->iRange, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
			psPacketRange->iCount &= 0x000000FF;
		} break;

		case Skills::Compulsion:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Compulsion]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT MagneticRange FROM Compulsion WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iRange = iValue;
			psPacketRange->iDamageState = 201;
			iValue = SQLSKILL->GetSkillValueInt( "SELECT Duration FROM Compulsion WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->dwHookedTargetEndTime = ( iValue * 1000 ) + TICKCOUNT;
			pcUserData->iCompulsionParam = 0;

		} break;

		case Skills::Rupture:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Rupture]" );

			iValue = iaMechT51DMGBoost[iLevelSkill-1];
			psPacketRange->iDamage = iDamageBoost + ((iDamageBoost * iValue) / 100);
			iValue = 90;
			psPacketRange->iRange = iValue;
			psPacketRange->iDamageState = 0;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", psPacketRange->iRange, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
		} break;

		default:
			break;
	}
	return 1;
}

int CMechanician::RecvBuffSkill( UserData * pcUserData, UINT packet )
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
		case Skills::ExtremeShield:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Extreme Shield]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT AddedBlockRtg FROM ExtremeShield WHERE SkillLevel=%d", iLevelSkill );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Add Block Rtg [%d%%] of shield", iValue );
		} break;

		case Skills::PhysicalAbsorption:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Physical Absorption]" );
			pcUserData->dwMaximizeEndTime = 0;
			pcUserData->iMaximizeMaxDamageBoost = 0;
			pcUserData->dwAutomationEndTime = 0;
			pcUserData->iAutomationDamageBoost = 0;
		} break;

		case Skills::Maximize:
		{
			if ( *(UINT*)(packet + 0x10) == 0 )
			{
				CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Maximize]" );
				iValue = SQLSKILL->GetSkillValueInt( "SELECT Duration FROM Maximize WHERE SkillLevel=%d", iLevelSkill );
				pcUserData->dwMaximizeEndTime = (iValue * 1000) + TICKCOUNT;
				iValue = SQLSKILL->GetSkillValueInt( "SELECT MaxDamageBoost FROM Maximize WHERE SkillLevel=%d", iLevelSkill );
				pcUserData->iMaximizeMaxDamageBoost = iValue;

				// Automation OFF
				pcUserData->dwAutomationEndTime = 0;
				pcUserData->iAutomationDamageBoost = 0;
			}
		} break;

		case Skills::Automation:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Automation]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT Duration FROM Automation WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->dwAutomationEndTime = ( iValue * 1000 ) + TICKCOUNT;
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM Automation WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->iAutomationDamageBoost = iValue;

			// Maximize OFF
			pcUserData->dwMaximizeEndTime = 0;
			pcUserData->iMaximizeMaxDamageBoost = 0;
		} break;

		case Skills::MetalArmor:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Metal Armor]" );
			pcUserData->dwMaximizeEndTime = 0;
			pcUserData->iMaximizeMaxDamageBoost = 0;
			pcUserData->dwAutomationEndTime = 0;
			pcUserData->iAutomationDamageBoost = 0;

		} break;

		case Skills::SparkShield:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Spark Shield]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT Duration FROM SparkShield WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->dwSparkShieldEndTime = ( iValue * 1000 ) + TICKCOUNT;
			iValue = SQLSKILL->GetSkillValueInt( "SELECT ReflectDmg FROM SparkShield WHERE SkillLevel=%d", iLevelSkill );
			int iSparkLevel = *( int* )( packet + 0x10 );
			if ( iSparkLevel <= 0 )
			{
				pcUserData->dwSparkShieldEndTime = 0;
				break;
			}
			int iSparkDmg = SQLSKILL->GetSkillValueInt( "SELECT SparkDmg FROM Spark WHERE SkillLevel=%d", iSparkLevel );
			pcUserData->iSparkShieldDamage = ( iSparkDmg * iValue ) / 100;
		} break;

		case Skills::MagneticSphere:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Magnetic Sphere]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT Duration FROM MagneticSphere WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->dwMagneticSphereEndTime = ( iValue * 1000 ) + TICKCOUNT;
			pcUserData->iMagneticSphereParam = iLevelSkill;
		} break;

		case Skills::MetalGolem:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Metal Golem]" );

			if ( MAPSERVER->IsSummonMap( pcUser->GetMapID() ) == FALSE )
			{
				CHATSERVER->SendChatMessageBox( pcUser, "You can't summon a pet on this map" );
				break;
			}

			UnitData * pcUnitDataPet = SummonMobSkillMS( 0x0F, pcUserData->sPosition.iX, pcUserData->sPosition.iY, pcUserData->sPosition.iZ, pcUserData, 0 );

			if ( pcUnitDataPet )
			{
				pcUnitDataPet->dwLastUpdateTime = ( 300 * 1000 ) + TICKCOUNT;

				iValue = SQLSKILL->GetSkillValueInt( "SELECT AttackPower FROM MetalGolem WHERE SkillLevel=%d", iLevelSkill );
				pcUnitDataPet->sCharacterData.iMinDamage = iValue;
				pcUnitDataPet->sCharacterData.iMaxDamage = iValue;

				iValue = SQLSKILL->GetSkillValueInt( "SELECT AttackRating FROM MetalGolem WHERE SkillLevel=%d", iLevelSkill );
				pcUnitDataPet->sCharacterData.iAttackRating = iValue;

				iValue = SQLSKILL->GetSkillValueInt( "SELECT Defense FROM MetalGolem WHERE SkillLevel=%d", iLevelSkill );
				pcUnitDataPet->sCharacterData.iDefenseRating = iValue;

				iValue = SQLSKILL->GetSkillValueInt( "SELECT HP FROM MetalGolem WHERE SkillLevel=%d", iLevelSkill );
				pcUnitDataPet->sCharacterData.sHP.sCur = ( WORD )iValue;
				pcUnitDataPet->sCharacterData.sHP.sMax = ( WORD )iValue;

				pcUnitDataPet->sCharacterData.sSize = 224 + iLevelSkill * 15;

				pcUnitDataPet->saMoveStepCount[ 0 ] = 1;
				pcUnitDataPet->saMoveStepCount[ 1 ] = 2;
			}

		} break;

		case SKILLPLAYID_T52:
		{
			CMechanician::AddMagneticDischarge( pcUser, iLevelSkill, pcUser->pData.iLevelSkill[15 - 1], true );
		} break;



		default:
			break;
	}
	return 1;
}

BOOL CMechanician::AddMagneticDischarge( User * pcUser, int iLevel, int iLevelMagneticSphere, bool bSelf )
{
	CHATSERVER->SendDebugChat( pcUser->pcUserData, CHATCOLOR_Error, "> [Magnetic Discharge]" );
	pcUser->dwMagneticDischargeTimeOut = (300 * 1000) + TICKCOUNT;
	pcUser->uMagneticDischargeLevel = iLevel;
	pcUser->bMagneticDischargeSelf = bSelf;


	pcUser->pcUserData->iMagneticSphereParam = iLevelMagneticSphere;
	pcUser->pcUserData->dwMagneticSphereEndTime = 0;

	return TRUE;
}

BOOL CMechanician::DoMagneticDischargeDamage( User * pcUser, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageNormalEx * psPacketEx )
{
	CHATSERVER->SendDebugChat( pcUser->pcUserData, CHATCOLOR_Error, "> [Magnetic Discharge]" );
	if ( pcUser->dwMagneticDischargeTimeOut >= TICKCOUNT )
	{
		int iValue = SQLSKILL->GetSkillValueInt( "SELECT SphereDmg FROM MagneticSphere WHERE SkillLevel=%d", pcUser->pcUserData->iMagneticSphereParam );
		iValue += iaMechT52DMGBoost[pcUser->uMagneticDischargeLevel - 1];

		if ( pcUser->bMagneticDischargeSelf == false )
			iValue >>= 1;

		psPacket->iDamage += ((psPacket->iDamage * iValue) / 100);
		if ( pcUnitData )
		{
			if ( pcUnitData->sCharacterData.iMonsterType == 0x93 )
			{
				psPacket->iDamage += ((psPacket->iDamage * 30) / 100);
				CHATSERVER->SendDebugChat( pcUser->pcUserData, CHATCOLOR_Error, "> +30%% Machine Monster" );
			}
		}
		psPacketEx->saStatus[0] = 0;
		psPacket->iDamageState = psPacket->iDamageState | 0x00050000;
	}
	CHATSERVER->SendDebugChat( pcUser->pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUser->pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
	CHATSERVER->SendDebugChat( pcUser->pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );

	return TRUE;
}


CMechanician::~CMechanician()
{
}
