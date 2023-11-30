#include "stdafx.h"
#include "CSesmicImpactHandler.h"


CSesmicImpactHandler::CSesmicImpactHandler()
{
}


CSesmicImpactHandler::~CSesmicImpactHandler()
{
}

bool CSesmicImpactHandler::OnCastAnimationStart( struct Skill * psSkill, UnitData * pcUnitData )
{
	if ( USESKILL( psSkill ) )
	{
		//Update Skill Data
		UNITDATA->sActiveSkill.cSkillTinyID	= SKILLPLAYID_T51;
		UNITDATA->sActiveSkill.cLevel		= psSkill->iLevel;

		//Animate...
		UNITDATA->Animate( ANIMATIONTYPE_Skill );

		//New FPS based on Weapon Speed
		UNITDATA->iNewFPS					= (int)((float)SKILLMANAGERHANDLER->GetAttackFrameSpeed( UNITDATA->sCharacterData.iAttackSpeed ) * 2.3f);
		UNITDATA->bOverrideFPS				= TRUE;

		return true;
	}

	return false;
}

BOOL CSesmicImpactHandler::OnBegin( UnitData * pcThis, int iSkillID, int iLevel, UnitData * pcUnitData, int iX, int iY, int iZ )
{


	return FALSE;
}

BOOL CSesmicImpactHandler::OnUse()
{

	return TRUE;
}

BOOL CSesmicImpactHandler::OnEvent( UnitData * pcUnitData )
{
	Point3D s;

	//Hitting Position
	if ( pcUnitData->GetAnimationAttackPosition( &s.iX, &s.iY, &s.iZ ) )
	{
		//Red light while perform skill...
		SetDynamicLight( s.iX, s.iY, s.iZ, 200, 0, 0, 0, 400 );

		//When Character back axe, miss sound will be place
		if ( pcUnitData->iAnimationEvent == 3 )
			SOUNDHANDLER->SetWeaponSound( WEAPONSOUND_TwoHandMiss, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );
		else
		{
			//When Character hit axe, Avenging crash sound will...
			SOUNDHANDLER->SetSkillSound( SKILLSOUND_AvengingCrash, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );

			if ( pcUnitData == UNITDATA )
			{
				if ( pcUnitData->pcTarget )
				{
					//Last Hit is AoE
					if ( pcUnitData->iAnimationEvent == 4 )
					{
						SKILLMANAGERHANDLER->SetUnitsRange( pcUnitData->pcTarget->sPosition, iaFighterT52Area[UNITDATA->sActiveSkill.cLevel - 1] );

						SKILLMANAGERHANDLER->DamageAreaHandle( pcUnitData->pcTarget, pcUnitData->pcTarget->sPosition, 0, 0, 0, 0, UNITDATA->sActiveSkill.iValue );
					}
					else
					{
						SKILLMANAGERHANDLER->DamageUnitHandle( pcUnitData->pcTarget, 0, 0, 0, 0, UNITDATA->sActiveSkill.iValue, TRUE );
					}
				}
			}

			//Last Hit is AoE
			if ( pcUnitData->iAnimationEvent == 4 )
			{
				CParticleScript * pc = PARTICLEFACTORY->LoadScript( "game\\scripts\\particles\\SkillFighterT5SesmicImpact.lua" );

				pc->SetOwner( new Engine::ObjectOwnerPosition( Point3D( s.iX, s.iY - (28 << 8), s.iZ ) ) );

				PARTICLEENGINE->AddScript( pc );
			}

			//Particle every hit
			PARTICLE->Start( iParticleID, s.iX, s.iY, s.iZ );
		}
	}

	return TRUE;
}

void CSesmicImpactHandler::LoadParticle()
{
	iParticleID = PARTICLE->Load( "game\\scripts\\particles\\R5SesmicImpact.part" );
}

BOOL CSesmicImpactHandler::CanUse( Skill * psSkill )
{
	SKILLMANAGERHANDLER->UseManaStamina( psSkill );

	return TRUE;
}
