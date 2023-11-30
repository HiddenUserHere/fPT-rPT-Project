#include "stdafx.h"
#include "CRingsOfSpearsHandler.h"

CRingsOfSpearsHandler::CRingsOfSpearsHandler()
{
}

CRingsOfSpearsHandler::~CRingsOfSpearsHandler()
{
}

void CRingsOfSpearsHandler::LoadParticle()
{
	PARTICLE->Load( "game\\scripts\\particles\\PS5RingOfSpears.part" );
}

bool CRingsOfSpearsHandler::OnCastAnimationStart( Skill * psSkill, UnitData * pcUnitData )
{
	if( USESKILL( psSkill ) )
	{
		//Update Skill Data
		UNITDATA->sActiveSkill.cSkillTinyID = SKILLPLAYID_T41;
		UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

		//Animate...
		UNITDATA->Animate( ANIMATIONTYPE_Skill );

		UNITDATA->sActiveSkill.cSkillTinyID = SKILLPLAYID_T51;

		//New FPS based on Weapon Speed
		UNITDATA->iNewFPS = SKILLMANAGERHANDLER->GetAttackFrameSpeed( UNITDATA->sCharacterData.iAttackSpeed );
		UNITDATA->bOverrideFPS = TRUE;

		OnBeginSkillUse( UNITDATA, UNITDATA->pcTarget, 0, 0, 0 );

		SKILLMANAGERHANDLER->DamageUnitHandle( UNITDATA->pcTarget, 0, 0, 0, 0, UNITDATA->sActiveSkill.iValue, TRUE );

		return true;
	}

	return false;
}

BOOL CRingsOfSpearsHandler::OnBeginSkillUse( UnitData * pcThis, UnitData * pcUnitData, int iX, int iY, int iZ )
{
	//Lua Effect
	CEffectGroupLua * pEffectGroup = CLuaParticleOld::GetInstance()->Load( "game\\scripts\\particles\\PS5RingOfSpears.lua" );
	if ( pEffectGroup )
	{
		CEffectMainBaseLua * pMainBase = new CEffectMainBaseLua();
		pMainBase->SetID( 46464 );
		pMainBase->Init( pEffectGroup, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ, 0, 0, 0 );
		CEffectManagerHandlerLua::GetInstance()->AddEffect( pMainBase );
	}

	PARTICLE->Start( "PS5RingOfSpears", pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );
	
	return TRUE;
}

BOOL CRingsOfSpearsHandler::OnEvent( UnitData * pcUnitData )
{
	if( pcUnitData->iAnimationEvent == 1 )
	{
		SOUNDHANDLER->SetSkillSound( 0x4857, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );
	}

	return TRUE;
}

BOOL CRingsOfSpearsHandler::CanUse( Skill * psSkill )
{
	return TRUE;
}