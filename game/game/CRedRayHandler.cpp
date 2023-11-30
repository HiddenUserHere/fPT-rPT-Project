#include "stdafx.h"
#include "CRedRayHandler.h"

CRedRayHandler::CRedRayHandler()
{
}

CRedRayHandler::~CRedRayHandler()
{
}

bool CRedRayHandler::OnCastAnimationStart( Skill * psSkill, UnitData * pcUnitData )
{
	if( USESKILL( psSkill ) )
	{
		//Update Skill Data
		UNITDATA->sActiveSkill.cSkillTinyID = SKILLPLAYID_T52;
		UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

		//Animate...
		UNITDATA->Animate( ANIMATIONTYPE_Skill );

		//New FPS based on Weapon Speed
		UNITDATA->iNewFPS = SKILLMANAGERHANDLER->GetAttackFrameSpeed( UNITDATA->sCharacterData.iAttackSpeed );
		UNITDATA->bOverrideFPS = TRUE;

		return true;
	}

	return false;
}

BOOL CRedRayHandler::OnEvent( UnitData * pcUnitData )
{
	return TRUE;
}

BOOL CRedRayHandler::CanUse( Skill * psSkill )
{
	return TRUE;
}
