#include "stdafx.h"
#include "Knight.h"
#include "HNSSkill.h"


CKnight::CKnight()
{
}

int CKnight::AddGodsBlessing( UserData * pcUserData, int TypeDamage )
{
	UINT uTime = pcUserData->GodsBlessingTime;
	if ( uTime >= TICKCOUNT )
	{
		int iValue = 0;
		if ( TypeDamage == 1 )
		{
			iValue = ((pcUserData->sCharacterData.iMinDamage * pcUserData->GodsBlessingParam) / 100);
			int iAtkMin = pcUserData->sCharacterData.iMinDamage + iValue;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> God's Blessing Damage Min [%d + %d] = %d (%d)", pcUserData->sCharacterData.iMinDamage, iValue, iAtkMin, pcUserData->GodsBlessingParam);
			return iValue;
		}
		else
		{
			iValue = ((pcUserData->sCharacterData.iMaxDamage * pcUserData->GodsBlessingParam) / 100);
			int iAtkMax = pcUserData->sCharacterData.iMaxDamage + iValue;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> God's Blessing Damage Max [%d + %d] = %d (%d)", pcUserData->sCharacterData.iMaxDamage, iValue, iAtkMax, pcUserData->GodsBlessingParam);
			return iValue;
		}
	}
	return 0;
}

int CKnight::AddHolyConvictionUndeadDamage( UserData * pcUserData, UnitData * pcUnitData, int TypeDamage )
{
	User * pcUser = USERDATATOUSER( pcUserData );
	if ( pcUser && pcUnitData && pcUnitData->sCharacterData.iMonsterType == MONSTERTYPE_Undead )
	{
		UINT uTime = pcUser->dwHolyConvictionTimeOut;
		if ( uTime >= TICKCOUNT )
		{
			int iValue = pcUser->uHolyConvictionLevel;
			iValue = iaKnightT51DMGBoost[pcUser->uHolyConvictionLevel - 1];
			if ( TypeDamage == 1 )
			{
				int iAtkMin = pcUserData->sCharacterData.iMinDamage + ((pcUserData->sCharacterData.iMinDamage * iValue) / 100);
				CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Holy Conviction Damage Min [%d + %d] = %d", pcUserData->sCharacterData.iMinDamage, ((pcUserData->sCharacterData.iMinDamage * iValue) / 100), iAtkMin );
				int iDamage = ((pcUserData->sCharacterData.iMinDamage * iValue) / 100);
				return iDamage;
			}
			else
			{
				int iAtkMax = pcUserData->sCharacterData.iMaxDamage + ((pcUserData->sCharacterData.iMaxDamage * iValue) / 100);
				CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Holy Conviction Damage Max [%d + %d] = %d", pcUserData->sCharacterData.iMaxDamage, ((pcUserData->sCharacterData.iMaxDamage * iValue) / 100), iAtkMax );
				int iDamage = ((pcUserData->sCharacterData.iMaxDamage * iValue) / 100);
				return iDamage;
			}
		}
	}
	return 0;
}



int CKnight::RecvSkillSingleTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageNormalEx * psPacketEx )
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
			int iDamageBoost = psPacket->iDamage;
			psPacket->iDamage = iDamageBoost;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Default Attack]" );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
		} break;


		case Skills::DoubleCrash:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Double Crash]" );
			int iCritical = SQLSKILL->GetSkillValueInt( "SELECT AddedCritical FROM DoubleCrash WHERE SkillLevel=%d", iLevelSkill );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM DoubleCrash WHERE SkillLevel=%d", iLevelSkill );
			psPacket->iDamage += ( psPacket->iDamage * iValue ) / 100;
			psPacketEx->saStatus[ 0 ] += iCritical;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", psPacketEx->saStatus[ 0 ], pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
		} break;

		case Skills::DivinePiercing:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Divine Piercing]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM DivinePiercing WHERE SkillLevel=%d", iLevelSkill );
			psPacket->iDamage += ( psPacket->iDamage * iValue ) / 100;
			int iCritical = SQLSKILL->GetSkillValueInt( "SELECT AddedCritical FROM DivinePiercing WHERE SkillLevel=%d", iLevelSkill );
			psPacketEx->saStatus[0] += iCritical;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", psPacketEx->saStatus[ 0 ], pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacket->iDamage );
		} break;

		case Skills::HolyIncantation:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Holy Incantation]" );
			if ( pcUnitData )
			{
				if ( pcUnitData->sCharacterData.iMonsterType == MONSTERTYPE_Undead )
				{

					// Is Incantation Alive?
					if ( pcUserData->dwHolyIncantationID )
					{
						UnitData * pcUnitDataIncantaten = UNITSERVER->GetUnit( pcUserData->dwHolyIncantationID );
						if ( pcUnitDataIncantaten && pcUnitDataIncantaten->sCharacterData.iOwnerID == pcUserData->iID && pcUnitDataIncantaten->sCharacterData.sHP.sCur != 0 &&
							pcUnitDataIncantaten->sHolyIncantation.sMax > 0 )
						{
							CHATSERVER->SendChatMessageBox( USERDATATOUSER( pcUserData ), "You can't enchant more than one monster!" );
							return 0;
						}
					}

					pcUserData->dwHolyIncantationID = 0;

					int iElement = pcUnitData->sCharacterData.sElementalDef[ ELEMENTID_Organic ];
					if ( iElement > 100 )
						iElement = 100;

					iValue = SQLSKILL->GetSkillValueInt( "SELECT SuccessRate FROM HolyIncantation WHERE SkillLevel=%d", iLevelSkill );
					int iPercent = ( ( 100 - iElement ) * iValue ) / 100;
					srand( ( UINT )time( NULL ) );
					
					int iChance = ( rand() % 99 ) + 1;
					if ( iChance > iPercent )
						return 0;

					iValue = SQLSKILL->GetSkillValueInt( "SELECT IncreaseUndeadHP FROM HolyIncantation WHERE SkillLevel=%d", iLevelSkill );

					pcUnitData->sCharacterData.cNewLoad++;

					int iMax = ((int)pcUnitData->sCharacterData.sHP.sMax * iValue) / 100;

					pcUnitData->sCharacterData.sHP.sMax = (short)iMax;
					if ( iMax <= 0 || iMax > 30000 )
						pcUnitData->sCharacterData.sHP.sMax = 30000;

					pcUnitData->sCharacterData.sHP.sCur = pcUnitData->sCharacterData.sHP.sMax;
					pcUnitData->sUnitInfo.dwHashCharacterName = 0;
					pcUnitData->pcOwner = pcUserData;
					pcUnitData->sHolyIncantation.sMin = ( short )pcUnitData->sCharacterData.iMonsterType;
					
					int iDuration = SQLSKILL->GetSkillValueInt( "SELECT Duration FROM HolyIncantation WHERE SkillLevel=%d", iLevelSkill );
					pcUnitData->sHolyIncantation.sMax = iDuration * 19;
					
					pcUnitData->sCharacterData.iOwnerID = pcUserData->iID;
					pcUnitData->sCharacterData.iMonsterType = MONSTERTYPE_Friendly;
					STRINGCOPYLEN( ( char* )pcUnitData->sCharacterData.Pet.szOwnerName, 59, pcUserData->sCharacterData.szName );

					ITEMSERVER->OnUseCrystal( pcUserData, pcUnitData->iID, SKILLID_HolyIncantation, (iDuration * 1000) + TICKCOUNT );
					
					pcUnitData->pcAttackTarget = 0;
					pcUnitData->pcFocusTarget = 0;

					//Disable Monster Spawn
					MAPSERVER->DisableMonsterSpawn(pcUnitData->pcMap, pcUnitData);
					UNITSERVER->UpdateCharacterData(USERDATATOUSER(pcUserData), pcUnitData);

					typedef void( __cdecl *t_SendMobEncanten )( UserData * pcUserData, void * packet );
					t_SendMobEncanten SendMobEncanten = ( t_SendMobEncanten )0x00552130;
					BYTE PacketPT[ 44 ];
					ZeroMemory( PacketPT, 44 );
					*( UINT* )( &PacketPT[ 8 ] )	= pcUnitData->iID;
					pcUserData->dwHolyIncantationID = pcUnitData->iID;
					*( UINT* )( &PacketPT[ 12 ] ) = UnitServer::MOTIONID_Death;
					*( UINT* )( &PacketPT[ 16 ] ) = iDuration;
					*( UINT* )( &PacketPT[ 20 ] ) = 0;
					SendMobEncanten( pcUserData, PacketPT );
				}
			}
			return 0;
		} break;

		case Skills::GrandCross:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Grand Cross]" );
			int iValueAdd = 50;
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM GrandCross WHERE SkillLevel=%d", iLevelSkill );
			if ( pcUnitData )
			{
				if ( pcUnitData->sCharacterData.iMonsterType == 0x90 )
				{
					iValue += iValueAdd;
					CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> +%d%% Monster", iValueAdd );
				}
			}
			psPacket->iDamage += ( psPacket->iDamage * iValue ) / 100;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", psPacketEx->saStatus[ 0 ], psPacketEx->iAttackRating );
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


int CKnight::RecvSkillAreaTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageRange * psPacketRange, PacketSkillDamageRangeEx * psPacketRangeEx )
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

		case Skills::SwordBlast:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Sword Blast]" );
			int iDamage = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM SwordBlast WHERE SkillLevel=%d", iLevelSkill );
			int iStaticDamage = SQLSKILL->GetSkillValueInt( "SELECT StaticDamage FROM SwordBlast WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iDamage += ((psPacketRange->iDamage * iDamage) / 100) + iStaticDamage;
			psPacketRange->iDamageState = 0;
			psPacketRange->iCount &= 0x000000FF;
		} break;

		case Skills::Brandish:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Brandish]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM Brandish WHERE SkillLevel=%d", iLevelSkill );
			int iRange = SQLSKILL->GetSkillValueInt( "SELECT Area FROM Brandish WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iDamage += ( psPacketRange->iDamage * iValue ) / 100;
			psPacketRange->iRange = iRange;
			psPacketRange->iDamageState = 0;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", iRange, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
			psPacketRange->iCount &= 0x000000FF;
		} break;

		case Skills::Piercing:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Piercing]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM Piercing WHERE SkillLevel=%d", iLevelSkill );
			int iRange = SQLSKILL->GetSkillValueInt( "SELECT Range FROM Piercing WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iDamage += ( psPacketRange->iDamage * iValue ) / 100;
			psPacketRange->iRange = iRange + 100;
			psPacketRange->iDamageState = 0;
			psPacketRange->iCount &= 0x000000FF;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", iRange, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );

		} break;

		case Skills::SwordofJustice:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Sword of Justice]" );
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM SwordofJustice WHERE SkillLevel=%d", iLevelSkill );
			int iRange = SQLSKILL->GetSkillValueInt( "SELECT Range FROM SwordofJustice WHERE SkillLevel=%d", iLevelSkill );
			psPacketRange->iDamage += ( psPacketRange->iDamage * iValue ) / 100;
			psPacketRange->iRange = iRange;
			psPacketRange->iDamageState = 0;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", iRange, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
			psPacketRange->iCount &= 0x000000FF;

		} break;

		case SKILLPLAYID_T52:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Whirl Wind]" );
			iValue = iaKnightT52DMGBoost[iLevelSkill - 1];
			int iRange = iaKnightT52Area[iLevelSkill - 1];
			psPacketRange->iDamage += (psPacketRange->iDamage * iValue) / 100;
			psPacketRange->iRange = iRange;
			psPacketRange->iDamageState = 0;
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", iRange, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
		} break;

		default:
			break;
	}

	return 1;
}

int CKnight::RecvBuffSkill( UserData * pcUserData, UINT packet )
{
	UINT uSkillID = *( UINT* )( packet + 0x0C ) & 0x000000FF;
	int iLevelSkill = ( ( *( int* )( packet + 0x0C ) / 256 ) & 0x0000000F ) - 1;

	User * pcUser = USERDATATOUSER( pcUserData );
	

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

		case Skills::HolyBody:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Holy Body]" );
			int iDuration = SQLSKILL->GetSkillValueInt( "SELECT Duration FROM HolyBody WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->dwHolyBodyEndTime = (iDuration * 1000) + TICKCOUNT;
			iValue = SQLSKILL->GetSkillValueInt( "SELECT AbsorptionRtg FROM HolyBody WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->iHolyBodyAbsorb = iValue;

			// Disable Skills
			pcUserData->dwHolyValorEndTime = 0;
			pcUserData->iHolyValorBoost = 0;

			pcUser->uHolyConvictionLevel = 0;
			pcUser->dwHolyConvictionTimeOut = 0;

		} break;

		case Skills::GodlyShield:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Godly Shield]" );
		}break;

		case Skills::DivineShield:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Divine Shield]" );
			int iDuration = SQLSKILL->GetSkillValueInt( "SELECT Duration FROM DivineShield WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->dwDivineShieldEndTime = ( iDuration * 1000 ) + TICKCOUNT;
			iValue = SQLSKILL->GetSkillValueInt( "SELECT AbsorbLife FROM DivineShield WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->sDivineShieldAbsorb = ( WORD )iValue;
			iValue = SQLSKILL->GetSkillValueInt( "SELECT AddedBlockRating FROM DivineShield WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->sDivineShieldAttackRating = ( WORD )iValue;

			// Disable Skills
			//	 *( UINT* )( Player + 0x0B330 ) = 0;
			//	 *( UINT* )( Player + 0x0B334 ) = 0;
		} break;

		case Skills::GodsBlessing:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [God's Blessing]" );
			pcUserData->GodsBlessingTime = ( 300 * 1000 ) + TICKCOUNT;
			iValue = SQLSKILL->GetSkillValueInt( "SELECT AddedAttackPower FROM GodsBlessing WHERE SkillLevel=%d", iLevelSkill );
			pcUserData->GodsBlessingParam = iValue;

			// Disable Skills
			//	 *( UINT* )( Player + 0x0B330 ) = 0;
			//	 *( UINT* )( Player + 0x0B334 ) = 0;
		} break;

		case Skills::HolyConviction:
		{
			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Holy Conviction]" );
			pcUserData->dwHolyValorEndTime = TICKCOUNT + (300 * 1000);
			iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM HolyValor WHERE SkillLevel=%d", pcUser->pData.iLevelSkill[5 - 1] );
			pcUserData->iHolyValorBoost = iValue;

			pcUserData->dwHolyBodyEndTime = TICKCOUNT + (300 * 1000);
			iValue = SQLSKILL->GetSkillValueInt( "SELECT AbsorptionRtg FROM HolyBody WHERE SkillLevel=%d", pcUser->pData.iLevelSkill[2 - 1] );
			pcUserData->iHolyBodyAbsorb = iValue;

			pcUser->uHolyConvictionLevel	= iLevelSkill;
			pcUser->dwHolyConvictionTimeOut = TICKCOUNT + (300 * 1000);

		} break;

		case SKILLPLAYID_T52:
			pcUser->bWhirlWind = iLevelSkill == 5 ? false : true;
			break;

		default:
			break;
	}
	return 1;
}

int CKnight::AddHolyValor( UserData * pcUserData, PacketSkillPartyData * packet )
{
	int iLevelSkill = packet->iLevel;

	CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "%d Effect", packet->iSkillID );

	int iValue = 0;

	if ( packet->iSkillID == Skills::HolyValor )
	{
		CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Holy Valor]" );
		int iDuration = SQLSKILL->GetSkillValueInt( "SELECT Duration FROM HolyValor WHERE SkillLevel=%d", iLevelSkill );
		pcUserData->dwHolyValorEndTime = ( iDuration * 1000 ) + TICKCOUNT;
		iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM HolyValor WHERE SkillLevel=%d", iLevelSkill );
		pcUserData->iHolyValorBoost = iValue;
		packet->iClass = 6;

		// Disable Skills
		pcUserData->dwHolyBodyEndTime = 0;
		pcUserData->iHolyBodyAbsorb = 0;
		
		User * pcUser = USERDATATOUSER( pcUserData );
		pcUser->uHolyConvictionLevel = 0;
		pcUser->dwHolyConvictionTimeOut = 0;
		return 1;
	}
	return 0;
}

int CKnight::AddHolyConviction( User * pcUser, User * pcTarget, PacketSkillPartyData * packet )
{
	int iLevelSkill = packet->iLevel;

	CHATSERVER->SendDebugChat( pcUser->pcUserData, CHATCOLOR_Error, "%d Effect", packet->iSkillID );
	CHATSERVER->SendDebugChat( pcTarget->pcUserData, CHATCOLOR_Error, "%d Effect", packet->iSkillID );

	int iValue = 0;

	if ( packet->iSkillID == Skills::HolyConviction )
	{
		CHATSERVER->SendDebugChat( pcUser->pcUserData, CHATCOLOR_Error, "> [Holy Valor]" );
		pcTarget->pcUserData->dwHolyValorEndTime = TICKCOUNT + (300 * 1000);
		iValue = SQLSKILL->GetSkillValueInt( "SELECT DamageBoost FROM HolyValor WHERE SkillLevel=%d", pcUser->pData.iLevelSkill[5 - 1] );
		pcTarget->pcUserData->iHolyValorBoost = iValue;
		packet->iClass = 6;

		CHATSERVER->SendDebugChat( pcUser->pcUserData, CHATCOLOR_Error, "> [Holy Body]" );
		pcTarget->pcUserData->dwHolyBodyEndTime = TICKCOUNT + (300 * 1000);
		iValue = SQLSKILL->GetSkillValueInt( "SELECT AbsorptionRtg FROM HolyBody WHERE SkillLevel=%d", pcUser->pData.iLevelSkill[2 - 1] );
		pcTarget->pcUserData->iHolyBodyAbsorb = iValue >> 1;

		pcTarget->uHolyConvictionLevel = iLevelSkill;
		pcTarget->dwHolyConvictionTimeOut = TICKCOUNT + (300 * 1000);

		return 1;
	}
	return 0;
}

void CKnight::OnDrasticSpirit( UserData * pcUserData )
{
	pcUserData->iHolyBodyAbsorb		= 0;
	pcUserData->dwHolyBodyEndTime	= 0;

	CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> [Drastic Spirit]" );
}

CKnight::~CKnight()
{
}
