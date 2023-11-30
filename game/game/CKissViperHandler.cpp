#include "stdafx.h"
#include "CKissViperHandler.h"

CKissViperHandler::CKissViperHandler()
{
}

CKissViperHandler::~CKissViperHandler()
{
}

void CKissViperHandler::AddTimer( int iTime, int iLevel )
{
	if ( !iTimerID )
		iTimerID = TIMERSKILLHANDLER->LoadSkillTimerTexture( "game\\images\\skills\\timers\\Asn5_KissViper.tga" );

	Skill * psSkill = TIMERSKILLHANDLER->FindSkillTimer( SKILLID_KissViper );

	if ( psSkill )
		ZeroMemory( psSkill, sizeof( Skill ) );

	if ( !TIMERSKILLHANDLER->FindSkillTimer( SKILLID_KissViper ) )
	{
		Skill sSkill;
		ZeroMemory( &sSkill, sizeof( sSkill ) );
		sSkill.iID = SKILLID_KissViper;
		sSkill.iUseTime = iTime;
		sSkill.iLevel = iLevel;
		sSkill.MatIcon = iTimerID;
		TIMERSKILLHANDLER->AddSkillTimer( &sSkill );
	}
}

bool CKissViperHandler::OnCastAnimationStart( Skill * psSkill )
{
	if ( (UNITDATA->GetAnimation() < ANIMATIONTYPE_Attack) && (UNITDATA_SELECTED == NULL) )
	{
		if ( USESKILL( psSkill ) )
		{
			//Update Skill Data
			UNITDATA->sActiveSkill.cSkillTinyID = SKILLPLAYID_T52;
			UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

			//Animate...
			UNITDATA->Animate( ANIMATIONTYPE_Skill );

			SKILLMANAGERHANDLER->ProcessSkillInParty( SKILLPLAYID_T52, psSkill->iLevel, 300 );

			OnBeginSkillUse( UNITDATA, NULL, 0, 0, 0 );

			return true;
		}
	}

    return false;
}

BOOL CKissViperHandler::OnBeginSkillUse( UnitData * pcThis, UnitData * pcUnitData, int iX, int iY, int iZ )
{
	OnCastEffect( UNITDATATOUNIT( pcThis ), (int)pcThis->sActiveSkill.cLevel, 300.0f, (pcThis == UNITDATA) );

	SOUNDHANDLER->SetSkillSound( 0x3211, pcThis->sPosition.iX, pcThis->sPosition.iY, pcThis->sPosition.iZ );
	SOUNDHANDLER->SetSkillSound( 0x4230, pcThis->sPosition.iX, pcThis->sPosition.iY, pcThis->sPosition.iZ );

	return TRUE;
}

void CKissViperHandler::OnCastEffect( Unit * pcUnit, UINT uSkillLevel, double fOverTime, bool bSelfCast )
{
	if ( pcUnit->bKissViper == false )
	{
		pcUnit->bKissViper = true;

		_bSelfCast = bSelfCast;

		
		if ( auto pcScript = EFFEKSEERHANDLER->LoadScript( "game\\scripts\\particles\\ASN5KissViper.efk" ) )
		{
			pcScript->SetID( PARTICLEID_KISS_VIPER );

			auto pcOwner = new Engine::ObjectOwnerUnit( pcUnit->pcUnitData );
			pcOwner->AddAnimationTypeLostCondition( ANIMATIONTYPE_Die );
			pcScript->SetOwner( pcOwner );

			EFFEKSEERHANDLER->AddScript( pcScript );
		}
		

		if ( pcUnit == UNIT )
			AddTimer( 300, uSkillLevel );
	}
}

void CKissViperHandler::OnCastEnd( Unit * pcUnit )
{
	if ( pcUnit->bKissViper )
	{
		EFFEKSEERHANDLER->Kill( PARTICLEID_KISS_VIPER, pcUnit->GetID() );

		pcUnit->bKissViper = false;
	}
}
