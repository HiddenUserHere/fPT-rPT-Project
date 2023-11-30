#include "stdafx.h"
#include "CIceElementalHandler.h"

CIceElementalHandler::CIceElementalHandler()
{
}

CIceElementalHandler::~CIceElementalHandler()
{
}

void CIceElementalHandler::AddTimer( int iTime, int iLevel )
{
	if ( !iTimerID )
		iTimerID = TIMERSKILLHANDLER->LoadSkillTimerTexture( "game\\images\\skills\\timers\\Pr5_IceElemental.tga" );

	Skill * psSkill = TIMERSKILLHANDLER->FindSkillTimer( SKILLID_IceElemental );

	if ( psSkill )
		ZeroMemory( psSkill, sizeof( Skill ) );

	if ( !TIMERSKILLHANDLER->FindSkillTimer( SKILLID_IceElemental ) )
	{
		Skill sSkill;
		ZeroMemory( &sSkill, sizeof( sSkill ) );
		sSkill.iID = SKILLID_IceElemental;
		sSkill.iUseTime = iTime;
		sSkill.iLevel = iLevel;
		sSkill.MatIcon = iTimerID;
		TIMERSKILLHANDLER->AddSkillTimer( &sSkill );
	}
}

bool CIceElementalHandler::OnCastAnimationStart( Skill * psSkill, UnitData * pcUnitData )
{
	if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_IceElemental ) )
		return true;

	if ( USESKILL( psSkill ) )
	{
		//Update Skill Data
		UNITDATA->sActiveSkill.cSkillTinyID = SKILLPLAYID_T44;
		UNITDATA->sActiveSkill.cLevel		= psSkill->iLevel;

		//Animate...
		UNITDATA->Animate( ANIMATIONTYPE_Skill );

		UNITDATA->sActiveSkill.cSkillTinyID = SKILLPLAYID_T51;

		//New FPS
		UNITDATA->iNewFPS		= 70;
		UNITDATA->bOverrideFPS	= TRUE;

		SOUNDHANDLER->SetSkillSound( 0x4710, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );

		PROCESSSKILLBUFF( UNITDATA );

		//Timer
	//	AddTimer( 300, psSkill->iLevel );
		return true;
	}

	return false;
}
