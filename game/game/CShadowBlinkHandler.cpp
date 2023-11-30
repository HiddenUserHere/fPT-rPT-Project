#include "stdafx.h"
#include "CShadowBlinkHandler.h"

CShadowBlinkHandler::CShadowBlinkHandler()
{
}

CShadowBlinkHandler::~CShadowBlinkHandler()
{
}

void CShadowBlinkHandler::LoadParticle()
{
	PARTICLE->Load( "Effect\\Particle\\Script\\ShadowBomb.part" );
}

bool CShadowBlinkHandler::OnCastAnimationStart( Skill * psSkill, UnitData * pcUnitData )
{
	//if( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_Blind ) )
	{
		if( USESKILL( psSkill ) )
		{
			//Update Skill Data
			UNITDATA->sActiveSkill.cSkillTinyID = SKILLPLAYID_T51;
			UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

			//New FPS based on Weapon Speed
			UNITDATA->iNewFPS = SKILLMANAGERHANDLER->GetAttackFrameSpeed( UNITDATA->sCharacterData.iAttackSpeed );
			UNITDATA->bOverrideFPS = TRUE;

			//Compute Distance
			int iX = abs( UNITDATA->sPosition.iX - UNITDATA->pcTarget->sPosition.iX );
			int iZ = abs( UNITDATA->sPosition.iZ - UNITDATA->pcTarget->sPosition.iZ );

			if( iX > iZ )
				iMoveCount = iX;
			else
				iMoveCount = iZ;

			iMoveCount += ( 40 << 8 );

			bActive = TRUE;

			SKILLMANAGERHANDLER->DamageUnitHandle( pcUnitData, 0, 0, 0, 0, UNITDATA->sActiveSkill.iValue, TRUE );

			return true;
		}
	}

	return false;
}

BOOL CShadowBlinkHandler::OnEvent( UnitData * pcUnitData )
{
	if( pcUnitData->iAnimationEvent == 1 )
	{
		PARTICLE->Start( "ShadowBomb", UNITDATA->sPosition.iX, UNITDATA->sPosition.iY, UNITDATA->sPosition.iZ );
		SOUNDHANDLER->SetSkillSound( 0x4813, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );
	}

	return TRUE;
}

void CShadowBlinkHandler::Update()
{
	if ( bActive )
	{
		if ( UNITDATA->pcTarget )
		{
			UNITGAME->MoveUnit( UNITDATA, 0x1388 );

			UNITDATA->Animate( ANIMATIONTYPE_Running );
			CHARACTERGAME->OnMotionChange( UNITDATA, 36, 0 );

			iMoveCount -= 0x1388;

			if ( iMoveCount <= 0 )
			{
				if( UNITDATA->pcTarget )
				{
					//Animate...
					UNITDATA->sActiveSkill.cSkillTinyID = SKILLPLAYID_T51;
					UNITDATA->Animate( ANIMATIONTYPE_Skill );

					//New FPS based on Weapon Speed
					UNITDATA->iNewFPS = SKILLMANAGERHANDLER->GetAttackFrameSpeed( UNITDATA->sCharacterData.iAttackSpeed );
					UNITDATA->bOverrideFPS = TRUE;
				}

				iMoveCount = 0;
				bActive = FALSE;
			}
		}
	}
}

