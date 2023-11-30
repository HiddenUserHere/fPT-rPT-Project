#include "stdafx.h"
#include "CPressDeityHandler.h"

CPressDeityHandler::CPressDeityHandler()
{
}

CPressDeityHandler::~CPressDeityHandler()
{
}

void CPressDeityHandler::LoadParticle()
{
	PARTICLE->Load( "game\\scripts\\particles\\PressOfDeity1.part" );
	PARTICLE->Load( "game\\scripts\\particles\\PressOfDeity2.part" );
}

bool CPressDeityHandler::OnCastAnimationStart( Skill * psSkill, UnitData * pcUnitData )
{
	if ( USESKILL( psSkill ) )
	{
		//Update Skill Data
		UNITDATA->sActiveSkill.cSkillTinyID = SKILLPLAYID_T52;
		UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

		//Animate...
		UNITDATA->Animate( ANIMATIONTYPE_Skill );

		UNITDATA->iNewFPS = SKILLMANAGERHANDLER->GetAttackFrameSpeed( UNITDATA->sCharacterData.iAttackSpeed );
		UNITDATA->bOverrideFPS = TRUE;

		return true;
	}

    return false;
}

BOOL CPressDeityHandler::OnEvent( UnitData * pcUnitData )
{
	if ( pcUnitData->iAnimationEvent == 1 )
	{
		SpawnEffect( pcUnitData, false );

		if ( UNITDATA == pcUnitData )
		{
			Point3D sPosition = UNITDATA->pcTarget ? UNITDATA->pcTarget->sPosition : UNITDATA->sPosition;

			SKILLMANAGERHANDLER->SetUnitsRange( sPosition, (iaShaT52Area[pcUnitData->sActiveSkill.cLevel - 1] * iaShaT52MeteoriteCount[pcUnitData->sActiveSkill.cLevel - 1]) >> 1 );
			SKILLMANAGERHANDLER->DamageAreaHandle( UNITDATA, sPosition, 0, 0, 0, 0, UNITDATA->sActiveSkill.iValue );
		}

		return TRUE;
	}

	return TRUE;
}

void CPressDeityHandler::SpawnEffect( UnitData * pcUnitData, bool bFinalEffect )
{
	Point3D sPosition = pcUnitData->sPosition;
	
	if ( pcUnitData->pcTarget )
		sPosition = pcUnitData->pcTarget->sPosition;

	if ( bFinalEffect == false )
		EffectGame::ShamanMeteorite( sPosition, 4 );
}
