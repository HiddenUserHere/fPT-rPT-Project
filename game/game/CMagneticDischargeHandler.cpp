#include "stdafx.h"
#include "CMagneticDischargeHandler.h"

bool CMagneticDischargeHandler::OnCastAnimationStart( Skill * psSkill )
{
	if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_MagneticDischarge ) )
		return true;

	if ( (UNITDATA->GetAnimation() < ANIMATIONTYPE_Attack) && (UNITDATA_SELECTED == NULL) )
	{
		if ( USESKILL( psSkill ) )
		{
			//Update Skill Data
			UNITDATA->sActiveSkill.cSkillTinyID = SKILLPLAYID_T43;
			UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

			//Animate...
			UNITDATA->Animate( ANIMATIONTYPE_Skill );

			UNITDATA->sActiveSkill.cSkillTinyID = SKILLPLAYID_T52;

			//New FPS
			UNITDATA->iNewFPS = 70;
			UNITDATA->bOverrideFPS = TRUE;

			//Set Event Frame
			if ( UNITDATA->psModelAnimation )
				UNITDATA->iaEventFrame[0] = (UNITDATA->psModelAnimation->iEndFrame - 20) * 160;

			PROCESSSKILLBUFF( UNITDATA );

			SKILLMANAGERHANDLER->ProcessSkillInParty( SKILLPLAYID_T52, psSkill->iLevel, 300, 300 );
			return true;
		}
	}
	return false;
}

void CMagneticDischargeHandler::CastMembers( PacketSkillPartyData * psPacket )
{
	for ( int i = 0; i < psPacket->iCount; i++ )
	{
		Unit * pcUnit = UNITDATATOUNIT( UNITDATABYID( psPacket->iaID[i] ) );
		if ( pcUnit )
		{
			if ( pcUnit != UNIT )
				OnCastEffect( pcUnit, psPacket->iLevel, psPacket->iParam1 );
		}
	}
}

NAKED void CMagneticDischargeHandler::AddMagneticSphere( UnitData * pcUnitData, float fTime, float fDamageDelay )
{
	JMP( 0x005D3700 );
}

BOOL CMagneticDischargeHandler::OnEvent( UnitData * pcUnitData )
{
	if ( pcUnitData->iAnimationEvent == 1 )
	{
		OnCastEffect( UNITDATATOUNIT( pcUnitData ), pcUnitData->sActiveSkill.cLevel, 300 );

		SKILLMANAGERHANDLER->SkillPlaySound( 0x2032, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );
	}

	return TRUE;
}

void CMagneticDischargeHandler::OnCastEnd( Unit * pcUnit )
{
	if ( pcUnit->bMagneticDischarge )
	{
		pcUnit->bMagneticDischarge = false;

		EFFEKSEERHANDLER->Kill( PARTICLEID_MAGNETIC_DISCHARGE, pcUnit->GetID() );

		CEffectManagerHandlerLua::KillEffect( SKILLID_MagneticSphere, pcUnit->GetID() );
	}
}

void CMagneticDischargeHandler::OnCastEffect( Unit * pcUnit, int iLevel, int iTime )
{
	if ( pcUnit->bMagneticDischarge == false )
	{
		CEffectManagerHandlerLua::KillEffect( SKILLID_MagneticSphere, pcUnit->GetID() );

		if ( pcUnit == UNIT )
			TIMERSKILLHANDLER->KillTimer( SKILLID_MagneticSphere );

		CMagneticDischargeHandler::AddMagneticSphere( pcUnit->pcUnitData, (float)iTime, 5.0f );

		
		if ( auto pcScript = EFFEKSEERHANDLER->LoadScript( "game\\scripts\\particles\\MS5MagneticDischarge.efk" ) )
		{
			pcScript->SetID( PARTICLEID_MAGNETIC_DISCHARGE );

			auto pcOwner = new Engine::ObjectOwnerUnit( pcUnit->pcUnitData );
			pcOwner->AddAnimationTypeLostCondition( ANIMATIONTYPE_Die );
			pcScript->SetOwner( pcOwner );

			EFFEKSEERHANDLER->AddScript( pcScript );
		}
		

		if ( pcUnit == UNIT )
			AddTimer( iTime, iLevel );

		pcUnit->bMagneticDischarge = true;
	}
}

BOOL CMagneticDischargeHandler::Damage( UnitData * pcUnitData )
{
	if ( pcUnitData == nullptr )
		return FALSE;

	IMinMax sWeaponDamage = IMinMax( 0, 0 );

	if ( auto pcItemData = ITEMHANDLER->GetItemEquippedByBase( ITEMBASE_Weapon ) )
	{
		sWeaponDamage.iMin = pcItemData->sItem.sDamage.sMin;
		sWeaponDamage.iMax = pcItemData->sItem.sDamage.sMax;
	}
	
	ActiveSkill sActiveSkill;
	sActiveSkill.cSkillTinyID = SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_MagneticDischarge ) ? 0x8C : SKILLPLAYID_T43;
	sActiveSkill.cLevel = 1;
	sActiveSkill.sValue = 0;

	SKILLMANAGERHANDLER->DamageUnitHandle( pcUnitData, sWeaponDamage.iMin, sWeaponDamage.iMax, 0, 0, sActiveSkill.iValue, FALSE );

	if ( RandomI( 1, 2 ) == 2 )
		SKILLMANAGERHANDLER->SkillPlaySound( 0x3741, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );
	else
		SKILLMANAGERHANDLER->SkillPlaySound( 0x3742, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );

	return TRUE;
}

void CMagneticDischargeHandler::AddTimer( int iTime, int iLevel )
{
	if ( iTimerID == -1 )
		iTimerID = TIMERSKILLHANDLER->LoadSkillTimerTexture( "game\\images\\skills\\timers\\Ms5_MagneticDischarge.tga" );

	Skill * psSkill = TIMERSKILLHANDLER->FindSkillTimer( SKILLID_MagneticDischarge );

	if ( psSkill )
		ZeroMemory( psSkill, sizeof( Skill ) );

	if ( !TIMERSKILLHANDLER->FindSkillTimer( SKILLID_MagneticDischarge ) )
	{
		Skill sSkill;
		ZeroMemory( &sSkill, sizeof( sSkill ) );
		sSkill.iID = SKILLID_MagneticDischarge;
		sSkill.iUseTime = iTime;
		sSkill.iLevel = iLevel;
		sSkill.MatIcon = iTimerID;
		TIMERSKILLHANDLER->AddSkillTimer( &sSkill );
	}
}

