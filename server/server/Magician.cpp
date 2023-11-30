#include "stdafx.h"
#include "Magician.h"
#include "HNSSkill.h"

typedef UnitData*( __cdecl *t_SummonMobSkillMG ) ( int iSkillID, int X, int Y, int Z, UserData * pcUserData, int v0 );
t_SummonMobSkillMG SummonMobSkillMG = ( t_SummonMobSkillMG )0x0055E5A0;


CMagician::CMagician()
{
}

int CMagician::UseDistortionMonster( UserData * pcUserData, PacketSkillDamageRange * psPacket, int iLevelSkill )
{
	int iSpeed = 240 - (240 * SQLSKILL->GetSkillValueInt( "SELECT SpeedReduction FROM Distortion WHERE SkillLevel=%d", iLevelSkill )) / 100;
	int iDamage = SQLSKILL->GetSkillValueInt( "SELECT DamageReduction FROM Distortion WHERE SkillLevel=%d", iLevelSkill ) * 17;

	for ( UINT i = 0; i < UNITSERVER->uUnitsInGame; i++ )
	{
		Unit * pcUnit = UNITSERVER->pcaUnitInGame[i];

		if ( (pcUnit->pcUnitData->IsDead() == FALSE) && pcUnit->pcUnitData->sCharacterData.iType )
		{
			for ( int j = 0; j < psPacket->iCount; j++ )
			{
				if ( pcUnit->GetID() == psPacket->iaTargetID[j] )
				{
					pcUnit->pcUnitData->iStunTimeLeft = 0;

					pcUnit->pcUnitData->iFreezeTimeLeft = iDamage;
					pcUnit->pcUnitData->iUpdateMoveReduce = iSpeed;
					
					pcUnit->pcUnitData->bDistortion = 0;
					pcUnit->iInertiaCount = 0;

					if ( pcUserData->sCharacterData.iClass == CHARACTERCLASS_Shaman )
						pcUnit->iInertiaCount = iDamage;
					else
						pcUnit->pcUnitData->bDistortion = iDamage;

					if ( pcUnit->pcUnitData->sCharacterData.sHP.sCur )
					{
						//Move Unit
						int iAngle = MapServer::GetAngle( pcUnit->pcUnitData->sPosition.iX, pcUnit->pcUnitData->sPosition.iZ, pcUserData->sPosition.iX, pcUserData->sPosition.iZ );
						int iNewAngle = (iAngle + PTANGLE_180) & PTANGLE_Mask;
						pcUnit->pcUnitData->sAngle.iY = iNewAngle;
						pcUnit->pcUnitData->Move( 10 );
						pcUnit->pcUnitData->sAngle.iY = iAngle;

						if ( (pcUnit->pcUnitData->GetAnimation() != ANIMATIONTYPE_Flinch) && (pcUnit->pcUnitData->GetAnimation() != ANIMATIONTYPE_Potion) )
						{
							int iDamageHP = (pcUnit->pcUnitData->sCharacterData.sHP.sMax << 2) / 100;
							if ( psPacket->iDamage > iDamageHP )
								pcUnit->pcUnitData->Animate( ANIMATIONTYPE_Flinch );
						}
					}

					if ( pcUnit->pcUnitData->sCharacterData.sHP.sCur <= 0 )
					{
						pcUnit->pcUnitData->Animate( ANIMATIONTYPE_Die );
						if ( pcUnit->pcUnitData->pcKillerData == NULL )
							pcUnit->pcUnitData->pcKillerData = pcUserData;
					}
				}
			}
		}
	}

	return TRUE;
}

int CMagician::UseDistortionUser( UserData * pcUserData, int iTimeByLevel )
{
	User * pcUser = USERDATATOUSER( pcUserData );
	for ( int i = 0; i < PLAYERS_MAX; i++ )
	{
		UserData * ps = USERSDATA + i;
		if ( ps && ps->pcSocketData && ps->sCharacterData.iType && ps->iID && ps != pcUserData )
		{
			User * pcTarget = USERDATATOUSER( ps );

			BOOL bContinue = FALSE;

			EBattleRoyaleStateEnemy eState = BATTLEROYALEHANDLER->GetStateEnemy( pcUser, pcTarget );
			if ( pcUserData->iClanID && ps->iClanID && pcUserData->iClanID == ps->iClanID )
			{

				if ( pcUser->bInBattleRoyale )
				{
					if ( (eState == BATTLEROYALESTATEENEMY_Friendly) || (eState == BATTLEROYALESTATEENEMY_Enemy) || (eState == BATTLEROYALESTATEENEMY_None) )
						bContinue = TRUE;
				}
				else
					bContinue = TRUE;
			}

			if ( pcUser->bInBattleRoyale && ((eState == BATTLEROYALESTATEENEMY_Friendly) || (eState == BATTLEROYALESTATEENEMY_Enemy) || (eState == BATTLEROYALESTATEENEMY_None)) )
				bContinue = TRUE;

			if ( pcUser->bPvPMode && pcTarget->bPvPMode )
			{
				if ( MAPSERVER->IsPKMap( pcUser->GetMapID() ) == FALSE )
					bContinue = TRUE;
			}

			if ( bContinue )
				continue;

			int iX			= (ps->sPosition.iX - pcUserData->sPosition.iX) >> 8;
			int iZ			= (ps->sPosition.iZ - pcUserData->sPosition.iZ) >> 8;
			int iDistance	= abs((iX*iX) + (iZ*iZ));

			int iResistance = ps->sCharacterData.sElementalDef[0];
			int iTime = (((100 - iResistance) * iTimeByLevel) / 100) * 70;

			int iID = pcUserData->sCharacterData.iClass == CHARACTERCLASS_Magician ? 0x4C : 0x1C;

			if ( abs( iX ) <= (27 * 15) && abs( iZ ) <= (27 * 15) )
			{
				CHATSERVER->SendDebugChat( pcUserData, EChatColor::CHATCOLOR_Error, "> Distorted[%s] Time[%d] Res[%d]",
					ps->sCharacterData.szName, iTime, iResistance );
				__asm
				{
					PUSH iTime;
					MOV EAX, iID;
					NEG EAX;
					PUSH EAX;
					PUSH 0x80;
					PUSH 1;
					PUSH ps;
					PUSH pcUserData;
					MOV EAX, 0x00401FF4;
					CALL EAX;
					ADD ESP, 0x18;
				}
			}
		}
	}

	return 0;
}

int CMagician::AddEnchantWeapon( UserData * pcUserData, UINT ArraySkill )
{

	UINT uTime = pcUserData->dwEnchantWeaponEndTime;

	int iValue = 0;

	if ( uTime >= TICKCOUNT )
	{
		int iLevelSkill = pcUserData->iEnchantWeaponAttr & 0x000000FF;
		int iAttr = pcUserData->iEnchantWeaponAttr >> 8;
		if ( iAttr == 2 )
		{
			iValue = SQLSKILL->GetSkillValueInt( "SELECT FireDamage FROM EnchantWeapon WHERE SkillLevel=%d", iLevelSkill );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Enchant Weapon Fire Damage Min [%d + %d]", pcUserData->sCharacterData.iMinDamage, iValue );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Enchant Weapon Fire Damage Max [%d + %d]", pcUserData->sCharacterData.iMaxDamage, iValue );
			return iValue;
		}
		else
			if ( iAttr == 1 )
			{
				iValue = SQLSKILL->GetSkillValueInt( "SELECT LightningDamage FROM EnchantWeapon WHERE SkillLevel=%d", iLevelSkill );
				CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Enchant Weapon Light Damage Min [%d + %d]", pcUserData->sCharacterData.iMinDamage, iValue );
				CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Enchant Weapon Light Damage Max [%d + %d]", pcUserData->sCharacterData.iMaxDamage, iValue );
				return iValue;
			}
			else
			{
				iValue = SQLSKILL->GetSkillValueInt( "SELECT IceDamage FROM EnchantWeapon WHERE SkillLevel=%d", iLevelSkill );
				CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Enchant Weapon Ice Damage Min [%d + %d]", pcUserData->sCharacterData.iMinDamage, iValue );
				CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Enchant Weapon Ice Damage Max [%d + %d]", pcUserData->sCharacterData.iMaxDamage, iValue );
				return iValue;
			}
	}
	return 0;
}


int CMagician::AddSpiritElemental( UserData * pcUserData, int TypeDamage )
{
	UINT uTime = pcUserData->dwSpiritElementalEndTime;
	if ( uTime >= TICKCOUNT )
	{
		int iValue1 = pcUserData->sSpiritElementalBoost.sMin;
		int iValue2 = pcUserData->sSpiritElementalBoost.sMax;
		if ( TypeDamage == 1 )
		{
			int iAtkMin = pcUserData->sCharacterData.iMinDamage + ( ( pcUserData->sCharacterData.iMinDamage * iValue1 ) / 100 );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Spirit Elemental Damage Min [%d + %d] = %d", pcUserData->sCharacterData.iMinDamage, ( ( pcUserData->sCharacterData.iMinDamage * iValue1 ) / 100 ), iAtkMin );
			return ( ( pcUserData->sCharacterData.iMinDamage * iValue1 ) / 100 );
		}
		else
		{
			int iAtkMax = pcUserData->sCharacterData.iMaxDamage + ( ( pcUserData->sCharacterData.iMaxDamage * iValue2 ) / 100 );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Spirit Elemental Damage Max [%d + %d] = %d", pcUserData->sCharacterData.iMaxDamage, ( ( pcUserData->sCharacterData.iMaxDamage * iValue2 ) / 100 ), iAtkMax );
			return ( ( pcUserData->sCharacterData.iMaxDamage * iValue2 ) / 100 );
		}
	}
	return 0;
}

int CMagician::RecvSkillSingleTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageNormalEx * psPacketEx )
{
	UINT uSkillID = psPacketEx->iSkillID & 0x000000FF;
	UINT uUnkV = ( psPacketEx->iSkillID / 4096 ) & 0x0000000F;
	int iLevelSkill = ( psPacketEx->iSkillID / 256 ) & 0x0000000F;

	


	if ( uSkillID != 0 && iLevelSkill <= 0 || iLevelSkill > 10 )
		return 0;

	int iDamageBoost = psPacket->iDamage;

	int iValue = 0;

	switch ( uSkillID )
	{
		case Skills::DefaultAttack:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Default Attack]" );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );

		} break;

		case Skills::FireBolt:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Fire Bolt]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM FireBolt WHERE SkillLevel=%d", iLevelSkill );
			int iStaticDamage = SQLSKILL->GetSkillValueInt( "SELECT StaticDamage FROM FireBolt WHERE SkillLevel=%d", iLevelSkill );
			psPacket->iDamage += ( ( psPacket->iDamage * iValue ) / 100 ) + iStaticDamage;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );

		} break;

		case Skills::DeathRay:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Death Ray]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM DeathRay WHERE SkillLevel=%d", iLevelSkill );
			psPacket->iDamage += ( ( psPacket->iDamage * iValue ) / 100 );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			psPacketEx->saStatus[ 0 ] = pcUserData->sCharacterData.iCritical;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
		} break;

		case Skills::DancingSword:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Dancing Sword]" );
			UINT uTime = pcUserData->dwDancingSwordEndTime;
			if ( uTime >= TICKCOUNT )
			{
				iLevelSkill = pcUserData->iDancingSwordLevel;
				int iAttr = iLevelSkill;
				if ( iAttr > 0x00010000 )
				{
					iLevelSkill = iLevelSkill & 0x000000FF;
					CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Fire Damage Type" );
					iValue = SQLSKILL->GetSkillValueInt( "SELECT FireDamage FROM DancingSword WHERE SkillLevel=%d", iLevelSkill );
					psPacket->iDamage = ( ( psPacket->iDamage * iValue ) / 100 );
				}
				else
				{
					iLevelSkill = iLevelSkill & 0x000000FF;
					CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Ice Damage Type" );
					iValue = SQLSKILL->GetSkillValueInt( "SELECT IceDamage FROM DancingSword WHERE SkillLevel=%d", iLevelSkill );
					psPacket->iDamage = ( ( psPacket->iDamage * iValue ) / 100 );
				}

				if ( pcUnitData )
				{
					psPacket->iDamage += ((psPacket->iDamage * 40) / 100);
					CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> +40%% Monster" );
				}

				CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
				CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
			}
		} break;

		//Magnetic Discharge
		case 0x8C:
			CMechanician::DoMagneticDischargeDamage( USERDATATOUSER( pcUserData ), pcUnitData, psPacket, psPacketEx );
			break;

		case SKILLPLAYID_T52:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Flame Ball]" );
			iValue = iaMageT52DMGBoost[iLevelSkill - 1];
			int iDamageBoost = psPacket->iDamage;
			psPacket->iDamage = iDamageBoost + ((iDamageBoost * iValue) / 100);
			psPacket->iDamageState = (psPacket->iDamageState & 0xFFFF0000) + 2;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
			psPacketEx->saStatus[0] = pcUserData->sCharacterData.iCritical;
		} break;


		default:
			break;
	}
	return 1;
}

int CMagician::RecvSkillAreaTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageRange * psPacketRange, PacketSkillDamageRangeEx * psPacketRangeEx )
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
		case Skills::FireBall:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Fire Ball]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM FireBall WHERE SkillLevel=%d", iLevelSkill );
			int iStaticDamage = SQLSKILL->GetSkillValueInt( "SELECT StaticDamage FROM FireBall WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iDamage += iStaticDamage + ( ( psPacketRange->iDamage * iValue ) / 100 );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT Area FROM FireBall WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iRange = iValue;
			psPacketRange->iDamageState = 100;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", psPacketRange->iRange, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
		} break;

		case Skills::Watornado:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Watornado]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM Watornado WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iDamage += ( ( psPacketRange->iDamage * iValue ) / 100 );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT Area FROM Watornado WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iRange = iValue;
			psPacketRange->iDamageState = 104;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", psPacketRange->iRange, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
		} break;

		case Skills::Diastrophism:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Diastrophism]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM Diastrophism WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iDamage += ( ( psPacketRange->iDamage * iValue ) / 100 );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT Area FROM Diastrophism WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iRange = iValue;
			psPacketRange->iDamageState = 0;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", psPacketRange->iRange, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
		} break;

		case Skills::FlameWave:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Flame Wave]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM FlameWave WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iDamage += ( ( psPacketRange->iDamage * iValue ) / 100 );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT Range FROM FlameWave WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iRange = iValue;
			psPacketRange->iDamageState = 100;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", psPacketRange->iRange, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
			psPacketRange->iCount &= 0x000000FF;
		} break;

		case Skills::Meteorite:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Meteorite]" );
			
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM Meteorite WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iDamage += ( ( psPacketRange->iDamage * iValue ) / 100 );
			
			iValue = SQLSKILL->GetSkillValueInt( "SELECT Area FROM Meteorite WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iRange = iValue;
			
			psPacketRange->iDamageState = 0;
			
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", psPacketRange->iRange, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
		} break;

		case Skills::Distortion:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Distortion]" );
			psPacketRange->iDamage = 0;
			psPacketRange->iRange = 120;
			psPacketRange->iDamageState = 7;
			psPacketRange->iCount &= 0x000000FF;
		} break;

		default:
			break;
	}
	return 1;
}

int CMagician::RecvBuffSkill( UserData * pcUserData, UINT packet )
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

	if ( iLevelSkill < 0 || iLevelSkill > 10 )
		return 1;

	int iValue = 0;

	User * pcUser = USERDATATOUSER( pcUserData );

	switch ( uSkillID )
	{
		case Skills::EnchantWeapon:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Enchant Weapon]" );
			UserData * pcUserDataTarget = USERSERVER->GetUserdata( *( int* )( packet + 0x10 ) );
			if ( pcUserDataTarget && !BATTLEROYALEHANDLER->IsPVPMap( pcUserDataTarget->iMapID ) )
			{
				int iDuration = SQLSKILL->GetSkillValueInt( "SELECT Duration FROM EnchantWeapon WHERE SkillLevel=%d", iLevelSkill );
				pcUserDataTarget->dwEnchantWeaponEndTime = ( iDuration * 1000 ) + TICKCOUNT;
				pcUserDataTarget->iEnchantWeaponAttr = ( *( UINT* )( packet + 0x14 ) << 8 ) | iLevelSkill;
				*( UINT* )( packet + 0x8 ) = JOBCODEID_Magician;
				SENDPACKET( USERDATATOUSER(pcUserDataTarget), ( void* )packet );
			}
		} break;

		case Skills::EnergyShield:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Energy Shield]" );
			int iDuration = SQLSKILL->GetSkillValueInt( "SELECT Duration FROM EnergyShield WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->dwEnergyShieldEndTime = ( iDuration * 1000 ) + TICKCOUNT;
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageReduction FROM EnergyShield WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->iEnergyShieldDamage = iValue;
		} break;

		case Skills::SpiritElemental:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Spirit Elemental]" );
			int iDuration = SQLSKILL->GetSkillValueInt( "SELECT Duration FROM SpiritElemental WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->dwSpiritElementalEndTime = ( iDuration * 1000 ) + TICKCOUNT;
			pcUserData->sSpiritElementalBoost.sMin = iaSpiritElementalMin[iLevelSkill - 1];
			pcUserData->sSpiritElementalBoost.sMax = iaSpiritElementalMax[iLevelSkill - 1];
		} break;

		case Skills::DancingSword:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Dancing Sword][Buff]" );
			int iDuration = SQLSKILL->GetSkillValueInt( "SELECT Duration FROM DancingSword WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->dwDancingSwordEndTime = ( iDuration * 1000 ) + TICKCOUNT;
			int iValue = ( *( UINT* )( packet + 0x10 ) << 16 ) | iLevelSkill;
			pcUserData->iDancingSwordLevel = iValue;
		} break;

		case Skills::FireElemental:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Fire Elemental]" );

			if ( MAPSERVER->IsSummonMap( pcUser->GetMapID() ) == FALSE )
			{
				CHATSERVER->SendChatMessageBox( pcUser, "You can't summon a pet on this map" );
				break;
			}

			UnitData * pcUnitDataPet = SummonMobSkillMG( 0x8D, pcUserData->sPosition.iX, pcUserData->sPosition.iY, pcUserData->sPosition.iZ, pcUserData, 0 );

			if ( pcUnitDataPet )
			{
				pcUnitDataPet->dwLastUpdateTime = ( 300 * 1000 ) + TICKCOUNT;

				iValue = SQLSKILL->GetSkillValueInt( "SELECT AttackPower FROM FireElemental WHERE SkillLevel=%d", iLevelSkill );
				pcUnitDataPet->sCharacterData.iMinDamage = iValue;
				pcUnitDataPet->sCharacterData.iMaxDamage = iValue;

				iValue = SQLSKILL->GetSkillValueInt( "SELECT AttackRating FROM FireElemental WHERE SkillLevel=%d", iLevelSkill );
				pcUnitDataPet->sCharacterData.iAttackRating = iValue;

				iValue = SQLSKILL->GetSkillValueInt( "SELECT HP FROM FireElemental WHERE SkillLevel=%d", iLevelSkill );
				pcUnitDataPet->sCharacterData.sHP.sCur = ( WORD )iValue;
				pcUnitDataPet->sCharacterData.sHP.sMax = ( WORD )iValue;

				pcUnitDataPet->sCharacterData.sSize = 192 + iLevelSkill * 12;

				pcUnitDataPet->saMoveStepCount[ 0 ] = 1;
				pcUnitDataPet->saMoveStepCount[ 1 ] = 2;
			}
		} break;

		case SKILLPLAYID_T51:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Wizard Trance]" );

			pcUser->uWizardTranceLevel = iLevelSkill;
			pcUser->dwWizardTranceTimeOut = TICKCOUNT + (300 * 1000);

			PacketUseSkillNew s;
			s.iLength = sizeof( PacketUseSkillNew );
			s.iHeader = PKTHDR_UseSkillNew;
			s.iSkillID = SKILLID_WizardTrance;
			s.iSkillLevel = pcUser->uWizardTranceLevel;
			s.lCasterID = pcUser->GetID();
			s.lTargetID = pcUser->GetID();
			SENDPACKETRANGE( &s, &pcUserData->sPosition, DISTANCE_UNIT_VIEWLIMIT );
		} break;

		default:
			break;
	}
	return 1;
}

BOOL CMagician::OnEnchantWeapon( UserData * pcUserData, PacketSkillPartyData * psPacket )
{
	if ( (psPacket->iLevel > 0) && (psPacket->iLevel <= 10) )
	{
		if ( (psPacket->iLevel >= (pcUserData->iEnchantWeaponAttr & 0x000000FF)) || (pcUserData->dwEnchantWeaponEndTime < TICKCOUNT) )
		{
			int iDuration = SQLSKILL->GetSkillValueInt( "SELECT Duration FROM EnchantWeapon WHERE SkillLevel=%d", psPacket->iLevel );
			pcUserData->dwEnchantWeaponEndTime = (iDuration * 1000) + TICKCOUNT;

			psPacket->iParam1 = rand() % 3;

			//Fire
			int iElementID = 3;
			//Ice
			if ( psPacket->iParam1 == 1 )
				iElementID = 4;
			//Lightning
			else if ( psPacket->iParam1 == 2 )
				iElementID = 5;

			pcUserData->iEnchantWeaponAttr = (iElementID << 8) | psPacket->iLevel;

			psPacket->iClass = 7;

			return TRUE;
		}
	}
	return FALSE;
}


CMagician::~CMagician()
{
}
