#include "stdafx.h"
#include "CWhirlWindHandler.h"


CWhirlWindHandler::CWhirlWindHandler()
{
}


CWhirlWindHandler::~CWhirlWindHandler()
{
}

bool CWhirlWindHandler::OnCastAnimationStart( Skill * psSkill, UnitData * pcUnitData )
{
	if ( !SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_WhirlWind ) )
	{
		if ( USESKILL( psSkill ) )
		{
			//Update Skill Data
			UNITDATA->sActiveSkill.cSkillTinyID = SKILLPLAYID_T52;
			UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

			//Animate...
			UNITDATA->Animate( ANIMATIONTYPE_Skill );
			//New FPS based on Weapon Speed
			UNITDATA->iNewFPS = 80;//SKILLMANAGERHANDLER->GetAttackFrameSpeed( UNITDATA->sCharacterData.iAttackSpeed );
			UNITDATA->bOverrideFPS = TRUE;

			AddTimer( iaKnightT52Duration[psSkill->iLevel - 1], psSkill->iLevel );

			UNITDATA->sActiveSkill.cLevel = 1;

			PROCESSSKILLBUFF( UNITDATA );

			UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

			return true;
		}
	}

	return false;
}

BOOL CWhirlWindHandler::OnEvent( UnitData * pcUnitData )
{
	if ( pcUnitData->iAnimationEvent == 1 )
	{
		if ( pcUnitData == UNITDATA )
			OnCastEffect( UNIT );
	}

	return TRUE;
}

BOOL CWhirlWindHandler::OnAnimationEnd( UnitData * pcUnitData )
{
	if ( pcUnitData == UNITDATA )
		TIMERSKILLHANDLER->KillTimer( SKILLID_WhirlWind );

	OnCastEnd( UNITDATATOUNIT( pcUnitData ) );
	return TRUE;
}

void CWhirlWindHandler::OnCastEffect( Unit * pcUnit )
{
	if ( pcUnit->bWhirlWind == false )
	{
		CParticleScript * pc = PARTICLEFACTORY->LoadScript( "game\\scripts\\particles\\KS5WhirlWind.lua" );
		if ( pc )
		{
			pc->SetID( SKILLID_WhirlWind );
			Engine::ObjectOwnerUnit * pcOwner = new Engine::ObjectOwnerUnit( pcUnit->pcUnitData );
			pcOwner->AddAnimationTypeLostCondition( ANIMATIONTYPE_Die );
			pc->SetOwner( pcOwner );
			PARTICLEENGINE->AddScript( pc );
		}

		pcUnit->bWhirlWind = true;
	}
}

void CWhirlWindHandler::OnCastEnd( Unit * pcUnit )
{
	if ( pcUnit->bWhirlWind )
	{
		PARTICLEENGINE->Kill( SKILLID_WhirlWind, pcUnit->pcUnitData->iID );

		if ( pcUnit == UNIT )
		{
			ActiveSkill s = pcUnit->pcUnitData->sActiveSkill;
			UNITDATA->sActiveSkill.cSkillTinyID = SKILLPLAYID_T52;
			UNITDATA->sActiveSkill.cLevel = 5;
			PROCESSSKILLBUFF( pcUnit->pcUnitData );
			UNITDATA->sActiveSkill = s;
		}

		pcUnit->bWhirlWind = false;
	}
}

void CWhirlWindHandler::Update()
{
	if ( Game::GetGameMode() == EGameMode::GAMEMODE_InGame )
	{
		if ( UNITDATA->GetAnimation() == ANIMATIONTYPE_Skill )
		{
			if ( UNITDATA->sCharacterData.iClass == CHARACTERCLASS_Knight )
			{
				if ( UNITDATA->sActiveSkill.cSkillTinyID == SKILLPLAYID_T52 )
				{
					if ( UNITDATA->iFrame >= (UNITDATA->iStartFrame + (18 * 160)) )
					{
						if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_WhirlWind ) )
						{
							//Inventory Side Changed?
							if ( READDWORD( 0x035EB68C ) || READDWORD( 0x035EB690 ) )
							{
								OnAnimationEnd( UNITDATA );
								UNITDATA->Animate( ANIMATIONTYPE_Idle );
							}

							if ( UNITDATA->GetAnimation() == ANIMATIONTYPE_Flinch )
								OnAnimationEnd( UNITDATA );

							if ( MOUSEHANDLER->GetEvent() == EMouseEvent::ClickDownR )
							{
								UNITDATA->sAngle.iY = CALL( 0x00417110 ); //Mouse
								UNITDATA->Move( ((250 + (10 * UNITDATA->sCharacterData.iMovementSpeed)) * 575) >> 8 );

								if ( (dwTimeUpdate + 600) < TICKCOUNT )
								{
									SKILLMANAGERHANDLER->SetUnitsRange( UNITDATA->sPosition, iaKnightT52Area[UNITDATA->sActiveSkill.cLevel - 1] );
									SKILLMANAGERHANDLER->DamageAreaHandle( UNITDATA, UNITDATA->sPosition, 0, 0, 0, 0, UNITDATA->sActiveSkill.iValue );
									dwTimeUpdate = TICKCOUNT;
								}
							}
						}
					}
				}
			}
		}
	}
}

int CWhirlWindHandler::GetMouseAngle()
{
	int iAX = MOUSEHANDLER->GetPosition()->iX - (RESOLUTION_WIDTH >> 1);
	int iAZ = MOUSEHANDLER->GetPosition()->iY - (RESOLUTION_HEIGHT >> 1);
	

	return 0;
}

void CWhirlWindHandler::AddTimer( int iTime, int iLevel )
{
	if ( !iTimerID )
		iTimerID = TIMERSKILLHANDLER->LoadSkillTimerTexture( "game\\images\\skills\\timers\\Kn5_WhirlWind.tga" );

	Skill * psSkill = TIMERSKILLHANDLER->FindSkillTimer( SKILLID_WhirlWind );

	if ( psSkill )
		ZeroMemory( psSkill, sizeof( Skill ) );

	if ( !TIMERSKILLHANDLER->FindSkillTimer( SKILLID_WhirlWind ) )
	{
		Skill sSkill;
		ZeroMemory( &sSkill, sizeof( sSkill ) );
		sSkill.iID = SKILLID_WhirlWind;
		sSkill.iUseTime = iTime;
		sSkill.iLevel = iLevel;
		sSkill.MatIcon = iTimerID;
		TIMERSKILLHANDLER->AddSkillTimer( &sSkill );
	}
}
