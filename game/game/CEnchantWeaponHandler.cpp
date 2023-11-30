#include "stdafx.h"
#include "CEnchantWeaponHandler.h"


CEnchantWeaponHandler::CEnchantWeaponHandler()
{
}


CEnchantWeaponHandler::~CEnchantWeaponHandler()
{
}

bool CEnchantWeaponHandler::OnCastAnimationStart( Skill * psSkill, UnitData * pcUnitData )
{
	if ( ((pcUnitData->sCharacterData.iType == CHARACTERTYPE_Player) || (pcUnitData->iCharacterType == CHARACTERTYPE_Player)) && (UNITDATA_SELECTED == pcUnitData) )
	{
		if ( USESKILL( psSkill ) )
		{
            SKILLMANAGERHANDLER->SkillPlaySound( 0x1800, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );

			//Update Skill Data
			UNITDATA->sActiveSkill.cSkillTinyID = SKILLPLAYID_T22;
			UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;
			
			int iElementID = rand() % 3;
			UNITDATA->sActiveSkill.iValue |= (iElementID) << 12;

			//Animate...
			UNITDATA->Animate( ANIMATIONTYPE_Skill );

			PROCESSSKILLBUFFT( UNITDATA, pcUnitData );

            SKILLMANAGERHANDLER->SkillPlaySound( 0x2820, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );
			return true;
		}
	}
	return false;
}

bool CEnchantWeaponHandler::OnCastAnimationStart( Skill * psSkill )
{
	if ( (UNITDATA->GetAnimation() < ANIMATIONTYPE_Attack) && (UNITDATA_SELECTED == NULL) )
	{
		if ( USESKILL( psSkill ) )
		{
			//Update Skill Data
			UNITDATA->sActiveSkill.cSkillTinyID = SKILLPLAYID_T22;
			UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

			int iElementID = rand() % 3;
			UNITDATA->sActiveSkill.iValue |= (iElementID) << 12;

			//Animate...
			UNITDATA->Animate( ANIMATIONTYPE_Skill );

			PROCESSSKILLBUFF( UNITDATA );
			
			CastMembers( iElementID );

			SKILLMANAGERHANDLER->ProcessSkillInParty( SKILLPLAYID_T22, psSkill->iLevel, 300, iElementID );
			return true;
		}
	}

    if ( UNITDATA_SELECTED && ((UNITDATA_SELECTED->sCharacterData.iType == CHARACTERTYPE_Player) || (UNITDATA_SELECTED->iCharacterType == CHARACTERTYPE_Player)) )
	    return false;

    return true;
}

void CEnchantWeaponHandler::OnCastEffect( Unit * pcUnit, UINT uSkillLevel, int iElementID, double fOverTime )
{
	if ( pcUnit == UNIT )
		AddTimer( UNITDATA, uSkillLevel, iElementID, 300 );
		
	AddEnchantWeaponEffect( pcUnit, iElementID );
}

void CEnchantWeaponHandler::OnCastEnd( UnitData * pcUnitData )
{
	SKILLMANAGERHANDLER->ResetSkillEffectParticle( SKILLEFFECTID_EnchantWeaponFire );
	SKILLMANAGERHANDLER->ResetSkillEffectParticle( SKILLEFFECTID_EnchantWeaponIce );
	SKILLMANAGERHANDLER->ResetSkillEffectParticle( SKILLEFFECTID_EnchantWeaponLightning );

	SKILLMANAGERHANDLER->ResetSkillEffectParticle( SKILLID_EnchantWeapon );
	SKILLMANAGERHANDLER->ResetSkillTimer( SKILLID_EnchantWeapon );
}

void CEnchantWeaponHandler::CastMembers( int iElementID )
{
	if ( PARTYHANDLER->GetPartyData().iMembersCount > 0 )
	{
		auto v = PARTYHANDLER->GetPartyMembersArea( 300 );
		for ( auto & p : v )
		{
			if ( p != UNITDATA )
			{
				CEnchantWeaponHandler::AddEnchantWeaponEffect( UNITDATATOUNIT( p ), iElementID );
			}
		}
	}
}

void CEnchantWeaponHandler::CastMembers( PacketSkillPartyData * psPacket )
{
	for ( int i = 0; i < psPacket->iCount; i++ )
	{
		Unit * pcUnit = UNITDATATOUNIT( UNITDATABYID( psPacket->iaID[i] ) );
		if ( pcUnit )
		{
			if ( pcUnit != UNIT )
				CEnchantWeaponHandler::AddEnchantWeaponEffect( pcUnit, psPacket->iParam1 );
		}
	}
}

void CEnchantWeaponHandler::AddEnchantWeaponEffect( Unit * pcUnit, int iElementID )
{
	Point3D sPosition = pcUnit->GetPosition();
	SKILLMANAGERHANDLER->SkillPlaySound( 0x1800, sPosition.iX, sPosition.iY, sPosition.iZ );
	CALL_WITH_ARG3( 0x00531E00, (DWORD)&sPosition, 2, 0 );

	switch ( iElementID )
	{
		case 0:
			AddEnchantWeaponFire( pcUnit->pcUnitData, 300 );
			break;

		case 1:
			AddEnchantWeaponIce( pcUnit->pcUnitData, 300 );
			break;

		case 2:
			AddEnchantWeaponLight( pcUnit->pcUnitData, 300 );
			break;

		default:
			break;
	}

	SKILLMANAGERHANDLER->SkillPlaySound( 0x2820, sPosition.iX, sPosition.iY, sPosition.iZ );
}

void CEnchantWeaponHandler::AddEnchantWeaponFire( UnitData * pcUnitData, int iTime )
{
	CALL_WITH_ARG2( 0x005B8AB0, (DWORD)pcUnitData, iTime );
	pcUnitData->iEnchantWeaponType		= 3;
	pcUnitData->dwEnchantWeaponTimeOut	= TICKCOUNT + (iTime * 1000);
}

void CEnchantWeaponHandler::AddEnchantWeaponIce( UnitData * pcUnitData, int iTime )
{
	CALL_WITH_ARG2( 0x005B8940, (DWORD)pcUnitData, iTime );
	pcUnitData->iEnchantWeaponType = 4;
	pcUnitData->dwEnchantWeaponTimeOut = TICKCOUNT + (iTime * 1000);
}

void CEnchantWeaponHandler::AddEnchantWeaponLight( UnitData * pcUnitData, int iTime )
{
	CALL_WITH_ARG2( 0x005B8B80, (DWORD)pcUnitData, iTime );
	pcUnitData->iEnchantWeaponType = 5;
	pcUnitData->dwEnchantWeaponTimeOut = TICKCOUNT + (iTime * 1000);
}

void CEnchantWeaponHandler::AddTimer( UnitData * pcUnitData, UINT uSkillLevel, int iElementID, double fOverTime )
{
	CALLT_WITH_ARG2( 0x005047E0, 0x0367E148, (int)fOverTime, uSkillLevel );
}
