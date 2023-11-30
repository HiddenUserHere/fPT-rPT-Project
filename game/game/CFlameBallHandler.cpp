#include "stdafx.h"
#include "CFlameBallHandler.h"

CFlameBallHandler::CFlameBallHandler()
{
}

CFlameBallHandler::~CFlameBallHandler()
{
}

bool CFlameBallHandler::OnCastAnimationStart( Skill * psSkill, UnitData * pcUnitData )
{
	if ( USESKILL( psSkill ) )
	{
		//Update Skill Data
		UNITDATA->sActiveSkill.cSkillTinyID = SKILLPLAYID_T42;
		UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

		//Animate...
		UNITDATA->Animate( ANIMATIONTYPE_Skill );

		//New FPS based on Weapon Speed
		UNITDATA->iNewFPS = 105;
		UNITDATA->bOverrideFPS = TRUE;

		UNITDATA->sActiveSkill.cSkillTinyID = SKILLPLAYID_T52;
		UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

		return true;
	}

	return false;
}

BOOL CFlameBallHandler::OnEvent( UnitData * pcUnitData )
{
	if ( pcUnitData->iAnimationEvent == 1 )
	{
		if ( UNITDATA == pcUnitData )
		{
			if ( pcUnitData->pcTarget )
			{
				int iAddAttackRating = ((UNITDATA->sCharacterData.iAttackRating * iaMageT52ATKRate[UNITDATA->sActiveSkill.cLevel - 1]) / 100);
				UNITDATA->sCharacterData.iAttackRating += iAddAttackRating;
				SKILLMANAGERHANDLER->DamageUnitHandle( pcUnitData->pcTarget, 0, 0, 0, 0, UNITDATA->sActiveSkill.iValue, TRUE );
				SKILLMANAGERHANDLER->DamageUnitHandle( pcUnitData->pcTarget, 0, 0, 0, 0, UNITDATA->sActiveSkill.iValue, TRUE );
				UNITDATA->sCharacterData.iAttackRating -= iAddAttackRating;
			}
		}

		
		if ( auto pcScript = EFFEKSEERHANDLER->LoadScript( "game\\scripts\\particles\\MG5FlameBall.efk" ) )
		{
			pcScript->SetID( PARTICLEID_FLAME_BALL );

			auto pcOwner = new Engine::ObjectOwnerUnit( pcUnitData );
			
			if ( pcUnitData->pcTarget )
			{
				auto s = pcUnitData->pcTarget->sPosition;
				s.iY += pcUnitData->pcTarget->iModelHeight >> 1;
				pcOwner->SetTargetPosition( s );
			}

			auto vRot = UNIT->GetAngle();
			vRot.iY += PTANGLE_180;
			pcOwner->SetRotation( vRot );

			pcOwner->AddAnimationTypeLostCondition( ANIMATIONTYPE_Die );
			pcScript->SetOwner( pcOwner );

			EFFEKSEERHANDLER->AddScript( pcScript );
		}
		

		SKILLMANAGERHANDLER->SkillPlaySound( 0x3610 + RandomI( 0, 1 ), pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );
	}

    return TRUE;
}
