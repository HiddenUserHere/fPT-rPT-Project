#include "stdafx.h"
#include "CHolyConvictionHandler.h"

CHolyConvictionHandler::CHolyConvictionHandler()
{
}

CHolyConvictionHandler::~CHolyConvictionHandler()
{
}

void CHolyConvictionHandler::LoadParticle()
{
	iParticleID = PARTICLE->Load( "game\\scripts\\particles\\KS5HolyConviction.part" );
}

void CHolyConvictionHandler::AddTimer( int iTime, int iLevel )
{
	if ( !iTimerID )
		iTimerID = TIMERSKILLHANDLER->LoadSkillTimerTexture( "game\\images\\skills\\timers\\Kn5_HolyConviction.tga" );

	Skill * psSkill = TIMERSKILLHANDLER->FindSkillTimer( SKILLID_HolyConviction );

	if ( psSkill )
		ZeroMemory( psSkill, sizeof( Skill ) );

	if ( !TIMERSKILLHANDLER->FindSkillTimer( SKILLID_HolyConviction ) )
	{
		Skill sSkill;
		ZeroMemory( &sSkill, sizeof( sSkill ) );
		sSkill.iID = SKILLID_HolyConviction;
		sSkill.iUseTime = iTime;
		sSkill.iLevel = iLevel;
		sSkill.MatIcon = iTimerID;
		TIMERSKILLHANDLER->AddSkillTimer( &sSkill );
	}
}

bool CHolyConvictionHandler::OnCastAnimationStart( Skill * psSkill, UnitData * pcUnitData )
{
	if ( UNITDATA->IsLowAnimationType() && USESKILL( psSkill ) )
	{
		//Update Skill Data
		UNITDATA->sActiveSkill.cSkillTinyID = SKILLPLAYID_T43;
		UNITDATA->sActiveSkill.cLevel		= psSkill->iLevel;

		//Animate...
		UNITDATA->Animate( ANIMATIONTYPE_Skill );

		UNITDATA->sActiveSkill.cSkillTinyID = SKILLPLAYID_T51;

		//New FPS
		UNITDATA->iNewFPS		= 70;
		UNITDATA->bOverrideFPS	= TRUE;

		PROCESSSKILLBUFF( UNITDATA );

		SKILLMANAGERHANDLER->ProcessSkillInParty( SKILLPLAYID_T51, psSkill->iLevel, 300 );

		SOUNDHANDLER->SetSkillSound( 0x2510, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );
 
		OnCastEffect( UNIT, psSkill->iLevel, 300.0f );

		return true;
	}

	return false;
}

void CHolyConvictionHandler::OnCastEffect( Unit * pcUnit, UINT uSkillLevel, double fOverTime )
{
	if ( pcUnit->bHolyConviction == FALSE )
	{
		pcUnit->bHolyConviction = TRUE;

		if ( pcUnit == UNIT )
		{
			AddTimer( 300, uSkillLevel );
			TIMERSKILLHANDLER->KillTimer( SKILLID_HolyValor );
			TIMERSKILLHANDLER->KillTimer( SKILLID_HolyBody );

			SKILLMANAGERHANDLER->ResetSkillEffectParticle( SKILLEFFECTID_HolyValorAction );
			SKILLMANAGERHANDLER->ResetSkillTimer( SKILLID_HolyValor );
		}

		int iID = PARTICLE->Start( "KS5HolyConviction", pcUnit->pcUnitData->sPosition.iX, pcUnit->pcUnitData->sPosition.iY, pcUnit->pcUnitData->sPosition.iZ );
		PARTICLE->AddScript( iID, iParticleID, pcUnit->pcUnitData, 300.0f, TRUE );
	}
}


void CHolyConvictionHandler::OnCastEnd( Unit * pcUnit )
{
	if ( pcUnit->bHolyConviction )
	{
		PARTICLE->Delete( pcUnit->GetID(), iParticleID );

		pcUnit->bHolyConviction = FALSE;
	}
}
