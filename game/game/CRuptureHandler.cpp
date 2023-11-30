#include "stdafx.h"
#include "CRuptureHandler.h"

CRuptureHandler::CRuptureHandler()
{
}

CRuptureHandler::~CRuptureHandler()
{
}

void CRuptureHandler::LoadParticle()
{
	PARTICLE->Load( "game\\scripts\\particles\\MC5RuptureHit.part" );
	PARTICLE->Load( "game\\scripts\\particles\\MC5RuptureFinalHit.part" );
}

bool CRuptureHandler::OnCastAnimationStart( Skill * psSkill, UnitData * pcUnitData )
{
	if ( USESKILL( psSkill ) )
	{
		//Update Skill Data
		UNITDATA->sActiveSkill.cSkillTinyID = SKILLPLAYID_T51;
		UNITDATA->sActiveSkill.cLevel		= psSkill->iLevel;

		//Animate...
		UNITDATA->Animate( ANIMATIONTYPE_Skill );

		//New FPS based on Weapon Speed
		UNITDATA->iNewFPS		= (int)((float)SKILLMANAGERHANDLER->GetAttackFrameSpeed( UNITDATA->sCharacterData.iAttackSpeed ) * 1.2f);
		UNITDATA->bOverrideFPS	= TRUE;

		return true;
	}

	return false;
}

BOOL CRuptureHandler::OnEvent( UnitData * pcUnitData )
{
	Point3D s;

	const int * piaAttackRatingGrandSmash = ((int*)0x00992B20);

	//Hitting Position
	if ( pcUnitData->GetAnimationAttackPosition( &s.iX, &s.iY, &s.iZ ) && pcUnitData->iTargetCritical >= 0 )
	{
		if( pcUnitData->iAnimationEvent == 1 )
			SOUNDHANDLER->SetSkillSound( 0x4855, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );
		else if ( pcUnitData->iAnimationEvent == 2 )
		{
			PARTICLE->Start( "MC5RuptureHit", s.iX, s.iY, s.iZ );

			if ( UNITDATA == pcUnitData )
			{
				int iAddAttackRating	= ((UNITDATA->sCharacterData.iAttackRating * piaAttackRatingGrandSmash[UNITDATA->sActiveSkill.cLevel - 1]) / 100);
				iAddAttackRating		= (iAddAttackRating * 70) / 100;

				if ( pcUnitData->pcTarget )
				{
					UNITDATA->sCharacterData.iAttackRating += iAddAttackRating;
					SKILLMANAGERHANDLER->DamageUnitHandle( pcUnitData->pcTarget, 0, 0, 0, 0, UNITDATA->sActiveSkill.iValue, TRUE );
					UNITDATA->sCharacterData.iAttackRating -= iAddAttackRating;
				}
			}
		}
		else if ( pcUnitData->iAnimationEvent == 3 )
		{
			//Green light while perform skill...
			SetDynamicLight( s.iX, s.iY, s.iZ, 0, 140, 0, 0, 200 );

			SOUNDHANDLER->SetSkillSound( 0x4856, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );

			PARTICLE->Start( "MC5RuptureFinalHit", s.iX, s.iY, s.iZ );		

			if ( pcUnitData == UNITDATA )
			{
				if ( pcUnitData->pcTarget )
				{
					int iAddAttackRating = (UNITDATA->sCharacterData.iAttackRating * piaAttackRatingGrandSmash[UNITDATA->sActiveSkill.cLevel - 1]) / 100;

					UNITDATA->sCharacterData.iAttackRating += iAddAttackRating;
					SKILLMANAGERHANDLER->DamageUnitHandle( pcUnitData->pcTarget, 0, 0, 0, 0, UNITDATA->sActiveSkill.iValue, TRUE );
					UNITDATA->sCharacterData.iAttackRating -= iAddAttackRating;
				}
			}
		}
	}

	return TRUE;
}

BOOL CRuptureHandler::CanUse( Skill * psSkill )
{
	SKILLMANAGERHANDLER->UseManaStamina( psSkill );

	return TRUE;
}
