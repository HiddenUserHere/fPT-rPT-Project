#include "stdafx.h"
#include "CSummonTigerHandler.h"

CSummonTigerHandler::CSummonTigerHandler()
{
}

CSummonTigerHandler::~CSummonTigerHandler()
{
}

void CSummonTigerHandler::AddTimer( int iTime, int iLevel )
{
	if ( !iTimerID )
		iTimerID = TIMERSKILLHANDLER->LoadSkillTimerTexture( "game\\images\\skills\\timers\\Ata5_SummonTiger.tga" );

	Skill * psSkill = TIMERSKILLHANDLER->FindSkillTimer( SKILLID_SummonTiger );

	if ( psSkill )
		ZeroMemory( psSkill, sizeof( Skill ) );

	if ( !TIMERSKILLHANDLER->FindSkillTimer( SKILLID_SummonTiger ) )
	{
		Skill sSkill;
		ZeroMemory( &sSkill, sizeof( sSkill ) );
		sSkill.iID = SKILLID_SummonTiger;
		sSkill.iUseTime = iTime;
		sSkill.iLevel = iLevel;
		sSkill.MatIcon = iTimerID;
		TIMERSKILLHANDLER->AddSkillTimer( &sSkill );
		PLAYMINISOUND( 25 );
	}
}

bool CSummonTigerHandler::OnCastAnimationStart( Skill * psSkill, UnitData * pcUnitData )
{
	if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_SummonTiger ) )
		return true;

	if ( USESKILL( psSkill ) )
	{
		//Update Skill Data
		UNITDATA->sActiveSkill.cSkillTinyID = SKILLPLAYID_T41;
		UNITDATA->sActiveSkill.cLevel		= psSkill->iLevel;

		//Animate...
		UNITDATA->Animate( ANIMATIONTYPE_Skill );

		UNITDATA->sActiveSkill.cSkillTinyID = SKILLPLAYID_T51;

		//New FPS
		UNITDATA->iNewFPS		= 70;
		UNITDATA->bOverrideFPS	= TRUE;

		PROCESSSKILLBUFF( UNITDATA );

		//Timer
	//	AddTimer( 300, psSkill->iLevel );
		return true;
	}

	return false;
}
