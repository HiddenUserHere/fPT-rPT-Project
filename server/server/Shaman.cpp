#include "stdafx.h"
#include "Mechanician.h"
#include "Shaman.h"


CShaman::CShaman()
{
}


CShaman::~CShaman()
{
}

typedef UnitData*( __cdecl *t_SummonMobSkillSHA ) ( int iSkillID, int X, int Y, int Z, UserData * pcUserData, int v0 );
t_SummonMobSkillSHA SummonMobSkillSHA = ( t_SummonMobSkillSHA )0x0055E5A0;


int CShaman::RecvSkillSingleTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageNormalEx * psPacketEx )
{
	UINT uSkillID = psPacketEx->iSkillID & 0x000000FF;
	UINT uUnkV = ( psPacketEx->iSkillID / 4096 ) & 0x0000000F;
	int iLevelSkill = ( psPacketEx->iSkillID / 256 ) & 0x0000000F;

	

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

		case Skills::DarkBolt:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Dark Bolt]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM DarkBolt WHERE SkillLevel=%d", iLevelSkill );
			psPacket->iDamage += ((psPacket->iDamage * iValue) / 100);
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );

		} break;

		case Skills::Scratch:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Scratch]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM Scratch WHERE SkillLevel=%d", iLevelSkill );

			if ( pcUnitData )
			{
				iValue += 30;
				CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> +30% Monster" );
			}

			psPacket->iDamage += ((psPacket->iDamage * iValue) / 100);
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );

		} break;

		case Skills::SpiritualManacle:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Spiritual Manacle]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT StunTime FROM SpiritualManacle WHERE SkillLevel=%d", iLevelSkill );
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

					DAMAGEHANDLER->SendDamagePvPData( pcUserData, ud, 1, 0x80, -0x2B, iStun );

					psPacket->iDamage = 0;
					psPacket->iRange = 0;
					return 0;
				}
			}
		} break;

		case Skills::Haunt:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Haunt]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT HPAbsorption FROM Haunt WHERE SkillLevel=%d", iLevelSkill );
			psPacket->iDamage = 0;
			psPacket->iDamageState = 4;

			if ( pcUnitData && (pcUnitData->sCharacterData.iMonsterType != MONSTERTYPE_Friendly) && (pcUnitData->sCharacterData.sGlow != TRUE) && (pcUnitData->GetAnimation() != ANIMATIONTYPE_Die) &&
				(pcUnitData->sCharacterData.sHP.sCur > 0) )
			{
				int iTotalHP = (int)pcUnitData->sCharacterData.sHP.sCur;
				int iPercent = (iTotalHP * iValue) / 100;
				pcUnitData->sCharacterData.sHP.sCur -= (short)iPercent;
				if ( pcUnitData->sCharacterData.sHP.sCur <= 0 )
				{
					pcUnitData->sCharacterData.sHP.sCur = 0;
					pcUnitData->Animate( ANIMATIONTYPE_Die );
					if ( pcUnitData->pcKillerData == NULL )
						pcUnitData->pcKillerData = pcUserData;
				}

				PacketSimple sPacket;
				sPacket.iLength = sizeof( PacketSimple );
				sPacket.iHeader = PKTHDR_RegenChar;
				sPacket.iUnk = 0;
				sPacket.iUnk2 = iPercent;
				sPacket.iUnk3 = 0;
				sPacket.iUnk4 = 0;
				SENDPACKET( USERDATATOUSER(pcUserData), &sPacket );
			}
		} break;

		case Skills::Judgement:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Judgement]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM Judgement WHERE SkillLevel=%d", iLevelSkill );
			psPacket->iDamage += ((psPacket->iDamage * iValue) / 100);
			iValue = SQLSKILL->GetSkillValueInt( "SELECT Range FROM Judgement WHERE SkillLevel=%d", iLevelSkill );
			psPacket->iRange = iValue;
			psPacket->iDamageState = 0;
			int iCritBoost = pcUserData->sCharacterData.iCritical + iaCriticalJudment[iLevelSkill - 1];
			psPacketEx->saStatus[0] = iCritBoost;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Range[%d] Attack Rating[%d]", iCritBoost, psPacket->iRange, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
		} break;
		
		//Magnetic Discharge
		case 0x8C:
			CMechanician::DoMagneticDischargeDamage( USERDATATOUSER( pcUserData ), pcUnitData, psPacket, psPacketEx );
			break;

	}
	return TRUE;
}

int CShaman::RecvSkillAreaTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageRange * psPacketRange, PacketSkillDamageRangeEx * psPacketRangeEx )
{
	UINT uSkillID = psPacketRangeEx->iSkillID & 0x000000FF;
	UINT uUnkV = ( psPacketRangeEx->iSkillID / 4096 ) & 0x0000000F;
	int iLevelSkill = ( psPacketRangeEx->iSkillID / 256 ) & 0x0000000F;

	

	if ( iLevelSkill <= 0 || iLevelSkill > 10 )
		return 0;

	*( UINT* )0x7ACA048 = psPacketRangeEx->saStatus[0];

	int iValue = 0;



	switch ( uSkillID )
	{
		case Skills::DarkWave:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Dark Wave]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM DarkWave WHERE SkillLevel=%d", iLevelSkill );
			int iRange = SQLSKILL->GetSkillValueInt( "SELECT Range FROM DarkWave WHERE SkillLevel=%d", iLevelSkill );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Damage [%d + %d][%d] Range [%d]", psPacketRange->iDamage, ( ( psPacketRange->iDamage * iValue ) / 100 ),
									   psPacketRange->iDamage + ( ( psPacketRange->iDamage * iValue ) / 100 ), iRange );
			psPacketRange->iDamage += ( ( psPacketRange->iDamage * iValue ) / 100 );
			psPacketRange->iRange = iRange;
			psPacketRange->iDamageState = 0;
		} break;

		case Skills::SpiritualFlare:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Spiritual Flare]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM SpiritualFlare WHERE SkillLevel=%d", iLevelSkill );
			int iRange = SQLSKILL->GetSkillValueInt( "SELECT Range FROM SpiritualFlare WHERE SkillLevel=%d", iLevelSkill );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Damage [%d + %d][%d] Range [%d]", psPacketRange->iDamage, ( ( psPacketRange->iDamage * iValue ) / 100 ),
									   psPacketRange->iDamage + ( ( psPacketRange->iDamage * iValue ) / 100 ), iRange );
			psPacketRange->iDamage += ( ( psPacketRange->iDamage * iValue ) / 100 );
			psPacketRange->iRange = iValue + 60;
			psPacketRange->iDamageState = 0;
		} break;

		case Skills::Inertia:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Inertia]" );
			psPacketRange->iDamage = 0;
			psPacketRange->iRange = 120;
			psPacketRange->iDamageState = 7;
			psPacketRange->iCount &= 0x000000FF;
		} break;

		case Skills::ChasingHunt:
		{
			User * u;
			u = USERDATATOUSER( pcUserData );
			if ( u )
			{
				CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Chasing Hunt]" );
				iValue = SQLSKILL->GetSkillValueInt( "SELECT MagneticRange FROM ChasingHunt WHERE SkillLevel=%d", iLevelSkill );
				psPacketRange->iRange = iValue;
				psPacketRange->iDamageState = 201;
				iValue = SQLSKILL->GetSkillValueInt( "SELECT Duration FROM ChasingHunt WHERE SkillLevel=%d", iLevelSkill );
				pcUserData->dwHookedTargetEndTime = (iValue * 1000) + TICKCOUNT;
				pcUserData->iCompulsionParam = 0;
			}
		} break;

		case Skills::PhantomCall:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Phantom Call]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM PhantomCall WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iDamage += ( ( psPacketRange->iDamage * iValue ) / 100 );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT Range FROM PhantomCall WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iRange = iValue;
			psPacketRange->iDamageState = 0;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", psPacketRange->iRange, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
		} break;

		case Skills::MourningPray:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Mourning Pray]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM MourningPray WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iDamage += ((psPacketRange->iDamage * iValue) / 100);
			iValue = SQLSKILL->GetSkillValueInt( "SELECT Range FROM MourningPray WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iRange = iValue;
			psPacketRange->iDamageState = 0;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", psPacketRange->iRange, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
		} break;

		case SKILLPLAYID_T52:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Press Deity]" );
			iValue = iaShaT52DMGBoost[iLevelSkill - 1];
			psPacketRange->iDamage += ((psPacketRange->iDamage * iValue) / 100);
			
			iValue = iaShaT52Area[iLevelSkill - 1] * iaShaT52MeteoriteCount[iLevelSkill - 1];
			if ( iValue > 220 )
				iValue = 220;

			psPacketRange->iRange = iValue;
			psPacketRange->iDamageState = 0;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", psPacketRange->iRange, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
		} break;

	}
	return TRUE;
}

int CShaman::RecvBuffSkill( UserData * pcUserData, UINT packet )
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
		case Skills::RecallBloodyKnight:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Recall Bloody Knight]" );

			if ( MAPSERVER->IsSummonMap( pcUser->GetMapID() ) == FALSE )
			{
				CHATSERVER->SendChatMessageBox( pcUser, "You can't summon a pet on this map" );
				break;
			}

			UnitData * pcUnitDataPet = SummonMobSkillSHA( 0x10, pcUserData->sPosition.iX, pcUserData->sPosition.iY, pcUserData->sPosition.iZ, pcUserData, 0 );

			if ( pcUnitDataPet )
			{
				pcUnitDataPet->dwLastUpdateTime = ( 300 * 1000 ) + TICKCOUNT;

				iValue = SQLSKILL->GetSkillValueInt( "SELECT AttackPower FROM RecallHestian WHERE SkillLevel=%d", iLevelSkill );
				pcUnitDataPet->sCharacterData.iMinDamage = iValue;
				pcUnitDataPet->sCharacterData.iMaxDamage = iValue;

				iValue = SQLSKILL->GetSkillValueInt( "SELECT AttackRating FROM RecallHestian WHERE SkillLevel=%d", iLevelSkill );
				pcUnitDataPet->sCharacterData.iAttackRating = iValue;

				iValue = SQLSKILL->GetSkillValueInt( "SELECT Defense FROM RecallHestian WHERE SkillLevel=%d", iLevelSkill );
				pcUnitDataPet->sCharacterData.iDefenseRating = iValue;

				iValue = SQLSKILL->GetSkillValueInt( "SELECT HP FROM RecallHestian WHERE SkillLevel=%d", iLevelSkill );
				pcUnitDataPet->sCharacterData.sHP.sCur = ( WORD )iValue;
				pcUnitDataPet->sCharacterData.sHP.sMax = ( WORD )iValue;

				pcUnitDataPet->sCharacterData.sSize = 192 + iLevelSkill * 12;

				pcUnitDataPet->saMoveStepCount[ 0 ] = 1;
				pcUnitDataPet->saMoveStepCount[ 1 ] = 2;
			}

		} break;

		case Skills::Rainmaker:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Rainmaker]" );
			int iDuration = SQLSKILL->GetSkillValueInt( "SELECT Duration FROM Rainmaker WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->dwEnergyShieldEndTime = ( iDuration * 1000 ) + TICKCOUNT;
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageReduction FROM Rainmaker WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->iEnergyShieldDamage = iValue;
		} break;

		case Skills::ChasingHunt:
			iValue = SQLSKILL->GetSkillValueInt( "SELECT Duration FROM ChasingHunt WHERE SkillLevel=%d", iLevelSkill );
			pcUser->dwChasingHuntTime = (iValue * 1000) + TICKCOUNT;
			pcUser->sChasingHuntDamageBoost.sMin = iaChasingHuntMin[iLevelSkill - 1];
			pcUser->sChasingHuntDamageBoost.sMax = iaChasingHuntMax[iLevelSkill - 1];
			break;

		case SKILLPLAYID_T51:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Creed]" );
			UserData * pcUserDataTarget = USERSERVER->GetUserdata( *(UINT*)(packet + 0x10) );
			if ( pcUserDataTarget )
			{
				*(UINT*)(packet + 8) = JOBCODEID_Shaman;
				SENDPACKET( USERDATATOUSER( pcUserDataTarget ), (void*)packet );
			}

			//In Party?
			if ( pcUser->psParty && pcUser->bParty )
			{
				if ( auto vUsersParty = PARTYHANDLER->GetPartyMembers( pcUser->psParty ); vUsersParty.size() > 0 )
				{
					for ( auto pcTarget : vUsersParty )
					{
						if ( pcTarget->pcUserData != pcUserDataTarget )
						{
							if ( pcTarget->pcUserData->sPosition.WithinPTDistance( &pcUser->pcUserData->sPosition, DISTANCE_UNIT_PARTY_MAX ) )
							{
								*(UINT *)(packet + 8) = JOBCODEID_Shaman;
								SENDPACKET( pcTarget, (void *)packet );
							}
						}
					}
				}
			}
		}; break;

	}
	return TRUE;
}

BOOL CShaman::AddAdventMigal( UserData * pcUserData, PacketSkillPartyData * packet )
{
	int iLevelSkill = packet->iLevel;

	CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "%d Effect", packet->iSkillID );

	int iValue = 0;

	if ( packet->iSkillID == Skills::AdventMigal )
	{
		User * pcUser = USERDATATOUSER( pcUserData );
		CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Advent Migal]" );
		int iDuration = SQLSKILL->GetSkillValueInt( "SELECT Duration FROM AdventMigal WHERE SkillLevel=%d", iLevelSkill );
		pcUser->dwAdventMigalTime = ( iDuration * 1000 ) + TICKCOUNT;
		int iDamage = SQLSKILL->GetSkillValueInt( "SELECT Damage FROM AdventMigal WHERE SkillLevel=%d", iLevelSkill );
		pcUser->dwAdventMigalDamage = iDamage;
		packet->iClass = 10;
		return TRUE;
	}
	return FALSE;
}

int CShaman::AddAdventMigalDamage( UserData * pcUserData )
{
	User * u = USERDATATOUSER( pcUserData );
	if ( u )
	{
		UINT uTime = u->dwAdventMigalTime;

		int iValue = 0;

		if ( uTime >= TICKCOUNT )
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Advent Migal Damage Min [%d + %d]", pcUserData->sCharacterData.iMinDamage, u->dwAdventMigalDamage );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Advent Migal Damage Max [%d + %d]", pcUserData->sCharacterData.iMaxDamage, u->dwAdventMigalDamage );
			return u->dwAdventMigalDamage;
		}
		else
		{
			u->dwAdventMigalTime = 0;
			u->dwAdventMigalDamage = 0;
		}
	}
	return 0;
}

int CShaman::AddChasingHuntAtkPow( UserData * pcUserData, int iType )
{
	User * u = USERDATATOUSER( pcUserData );
	if( u )
	{
		UINT uTime = u->dwChasingHuntTime;

		int iValue = 0;

		if ( uTime >= TICKCOUNT )
		{
			int iPercent = 0;
			if( iType == 1 )
			{
				iPercent = (pcUserData->sCharacterData.iMinDamage * u->sChasingHuntDamageBoost.sMin) / 100;
				CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Chasing Hunt Damage Min [%d + %d]", pcUserData->sCharacterData.iMinDamage, iPercent );
			}
			else
			{				
				iPercent = (pcUserData->sCharacterData.iMaxDamage * u->sChasingHuntDamageBoost.sMax) / 100;
				CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Chasing Hunt Damage Max [%d + %d]", pcUserData->sCharacterData.iMaxDamage, iPercent );
			}
			return iPercent;
		}
		else
		{
			u->dwChasingHuntTime = 0;
			u->sChasingHuntDamageBoost.sMin = 0;
			u->sChasingHuntDamageBoost.sMax = 0;
		}
	}
	return 0;
}
