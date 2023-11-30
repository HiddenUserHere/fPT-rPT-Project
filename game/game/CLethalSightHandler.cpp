#include "stdafx.h"
#include "CLethalSightHandler.h"
#include "SkillManager.h"

CLethalSightHandler::CLethalSightHandler()
{
}

CLethalSightHandler::~CLethalSightHandler()
{
}

void CLethalSightHandler::AddTimer( int iTime, int iLevel )
{
	if( !iTimerID )
		iTimerID = TIMERSKILLHANDLER->LoadSkillTimerTexture( "game\\images\\skills\\timers\\Ac5_LethalSight.tga" );

	Skill * psSkill = TIMERSKILLHANDLER->FindSkillTimer( SKILLID_LethalSight );

	if( psSkill )
		ZeroMemory( psSkill, sizeof( Skill ) );

	if( !TIMERSKILLHANDLER->FindSkillTimer( SKILLID_LethalSight ) )
	{
		Skill sSkill;
		ZeroMemory( &sSkill, sizeof( sSkill ) );
		sSkill.iID = SKILLID_LethalSight;
		sSkill.iUseTime = iTime;
		sSkill.iLevel = iLevel;
		sSkill.MatIcon = iTimerID;
		TIMERSKILLHANDLER->AddSkillTimer( &sSkill );
	}
}

void CLethalSightHandler::LoadParticle()
{
	iParticleID		= PARTICLE->Load( "game\\scripts\\particles\\Ac5LethalSight.part" );
	iParticleKeepID = PARTICLE->Load( "game\\scripts\\particles\\Ac5LethalSightKeep.part" );
}

bool CLethalSightHandler::OnCastAnimationStart( Skill * psSkill, UnitData * pcUnitData )
{
	if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_LethalSight ) )
		return true;

	if( USESKILL( psSkill ) )
	{
		//Update Skill Data
		UNITDATA->sActiveSkill.cSkillTinyID = SKILLPLAYID_T51;
		UNITDATA->sActiveSkill.cLevel		= psSkill->iLevel;

		//Animate...
		UNITDATA->Animate( ANIMATIONTYPE_Skill );

		//New FPS
		UNITDATA->iNewFPS		= 70;
		UNITDATA->bOverrideFPS	= TRUE;

		PROCESSSKILLBUFF( UNITDATA );

		SOUNDHANDLER->SetSkillSound( 0x4340, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );

		OnCastEffect( UNIT, psSkill->iLevel, 300.0f );

		return true;
	}

	return false;
}

void CLethalSightHandler::OnCastEffect( Unit * pcUnit, UINT uSkillLevel, double fOverTime )
{
	if( pcUnit->bLethalSight == FALSE )
	{
		pcUnit->bLethalSight = TRUE;

		if( pcUnit == UNIT )
		{
			AddTimer( 300, uSkillLevel );
		}

		PARTICLE->Start( "Ac5LethalSight", pcUnit->pcUnitData->sPosition.iX, pcUnit->pcUnitData->sPosition.iY, pcUnit->pcUnitData->sPosition.iZ );

		int iID = PARTICLE->Start( "Ac5LethalSightKeep", pcUnit->pcUnitData->sPosition.iX, pcUnit->pcUnitData->sPosition.iY, pcUnit->pcUnitData->sPosition.iZ );
		PARTICLE->AddScript( iID, iParticleKeepID, pcUnit->pcUnitData, (float)fOverTime, TRUE, TRUE, 0.5f );
	}
}


void CLethalSightHandler::OnCastEnd( Unit * pcUnit )
{
	if ( pcUnit->bLethalSight )
	{
		if ( pcUnit == UNIT )
			TIMERSKILLHANDLER->KillTimer( SKILLID_LethalSight );

		PARTICLE->Delete( pcUnit->GetID(), iParticleKeepID );

		pcUnit->bLethalSight = FALSE;
	}
}
