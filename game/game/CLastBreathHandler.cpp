#include "stdafx.h"
#include "CLastBreathHandler.h"


CLastBreathHandler::CLastBreathHandler()
{
}


CLastBreathHandler::~CLastBreathHandler()
{
}


bool CLastBreathHandler::OnCastAnimationStart( Skill * psSkill, UnitData * pcUnitData )
{
	//if ( pcUnitData->iStunTimeLeft > 0 )
	{
		if ( USESKILL( psSkill ) )
		{
			//Update Skill Data
			UNITDATA->sActiveSkill.cSkillTinyID = SKILLPLAYID_T52;
			UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

			//Animate...
			UNITDATA->Animate( ANIMATIONTYPE_Skill );

			//New FPS based on Weapon Speed
			UNITDATA->iNewFPS = 140;
			UNITDATA->bOverrideFPS = TRUE;

			return true;
		}
	}

	return false;
}

BOOL CLastBreathHandler::OnEvent( UnitData * pcUnitData )
{
	Point3D s;

	//Hitting Position
	if ( pcUnitData->GetAnimationAttackPosition( &s.iX, &s.iY, &s.iZ ) && pcUnitData->iTargetCritical >= 0 )
	{
		Unit * pcTarget = UNITDATATOUNIT( pcUnitData->pcTarget );

		//Jump
		if ( pcUnitData->iAnimationEvent == 1 )
		{
		//	if ( pcTarget )
		//		KNOCKUPHANDLER->AddUnit( new CKnockupUnit( pcTarget, 600.0f, 250.0f, 50.0f, 28000.0f ) );

			SOUNDHANDLER->SetSkillSound( 0x1130, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );
		}
		else if ( (pcUnitData->iAnimationEvent == 2) || (pcUnitData->iAnimationEvent == 3) )
		{
			SOUNDHANDLER->SetSkillSound( 0x3240, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );

			if ( pcUnitData == UNITDATA )
				SKILLMANAGERHANDLER->DamageUnitHandle( pcUnitData->pcTarget, 0, 0, 0, 0, UNITDATA->sActiveSkill.iValue, TRUE );

			SetDynamicLight( s.iX, s.iY, s.iZ, 50, 50, 180, 0, 400 );
		}
		//Vortice
		else if ( pcUnitData->iAnimationEvent == 4 )
		{
			if ( pcTarget )
			{
				CParticleScript * pc = PARTICLEFACTORY->LoadScript( "game\\scripts\\particles\\SkillPikemanLastBreath.luac" );
				pc->SetOwner( new Engine::ObjectOwnerUnit( pcUnitData->pcTarget ) );
				PARTICLEENGINE->AddScript( pc );

				SetDynamicLight( s.iX, s.iY, s.iZ, 50, 50, 180, 0, 400, 30 );
				
				if ( pcUnitData == UNITDATA )
					SKILLMANAGERHANDLER->DamageUnitHandle( pcUnitData->pcTarget, 0, 0, 0, 0, UNITDATA->sActiveSkill.iValue, TRUE );

				SOUNDHANDLER->SetSkillSound( 0x3241, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );
				SOUNDHANDLER->SetSkillSound( 0x2130, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );
				SOUNDHANDLER->SetSkillSound( 0x2120, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );
			}
		}
	}

	return FALSE;
}

void CLastBreathHandler::AddTarget( Unit * pcUnit )
{
}
