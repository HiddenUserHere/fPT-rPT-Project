#include "stdafx.h"
#include "CCreedHandler.h"


void CCreedHandler::AddCreedKeep( UnitData * pcUnitData )
{
	CALL_WITH_ARG4( 0x005B6EC0, (DWORD)"CreedKeep", (DWORD)pcUnitData, 300, SKILLID_Creed );
}

void CCreedHandler::AddCreedTimer( int iTime, int iLevel )
{
	CALLT_WITH_ARG2( 0x004ED680, 0x0367E148, iTime, iLevel );
}

CCreedHandler::CCreedHandler()
{
}


CCreedHandler::~CCreedHandler()
{
}

bool CCreedHandler::OnCastAnimationStart( Skill * psSkill, UnitData * pcUnitData )
{
	if ( pcUnitData->sCharacterData.iType == CHARACTERTYPE_Player )
	{
		if ( USESKILL( psSkill ) )
		{
			//Update Skill Data
			UNITDATA->sActiveSkill.cSkillTinyID = SKILLPLAYID_T51;
			UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

			//Animate...
			UNITDATA->Animate( ANIMATIONTYPE_Skill );

			//New FPS based on Weapon Speed
			UNITDATA->iNewFPS = SKILLMANAGERHANDLER->GetAttackFrameSpeed( UNITDATA->sCharacterData.iAttackSpeed );
			UNITDATA->bOverrideFPS = TRUE;

			OnBeginSkillUse( UNITDATA, pcUnitData, 0, 0, 0 );

			PROCESSSKILLBUFFT( UNITDATA, pcUnitData );

			PARTICLE->Start( "CreedStart", UNITDATA->sPosition.iX, UNITDATA->sPosition.iY, UNITDATA->sPosition.iZ );

			return true;
		}
	}
	return false;
}

bool CCreedHandler::OnCastAnimationStart( Skill * psSkill )
{
	if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_Creed ) )
	{
		if ( UNITDATA_SELECTED && UNITDATA_SELECTED->sCharacterData.iType == CHARACTERTYPE_Player )
			return false;
	}

	if ( USESKILL( psSkill ) )
	{
		//Update Skill Data
		UNITDATA->sActiveSkill.cSkillTinyID = SKILLPLAYID_T51;
		UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

		//Animate...
		UNITDATA->Animate( ANIMATIONTYPE_Skill );

		//New FPS based on Weapon Speed
		UNITDATA->iNewFPS = SKILLMANAGERHANDLER->GetAttackFrameSpeed( UNITDATA->sCharacterData.iAttackSpeed );
		UNITDATA->bOverrideFPS = TRUE;

		PROCESSSKILLBUFF( UNITDATA );

		OnCastEnd();

		AddCreedTimer( 300, psSkill->iLevel );

		AddCreedKeep( UNITDATA );

		PARTICLE->Start( "CreedStart", UNITDATA->sPosition.iX, UNITDATA->sPosition.iY, UNITDATA->sPosition.iZ );

		return true;
	}

	return false;
}

BOOL CCreedHandler::OnCast( int iLevel )
{
	OnCastEnd();
	AddCreedTimer( 300, iLevel );
	AddCreedKeep( UNITDATA );
	return TRUE;
}

BOOL CCreedHandler::OnCastEnd()
{
	TIMERSKILLHANDLER->KillTimer( SKILLID_Creed );
	return 0;
}

BOOL CCreedHandler::OnBeginSkillUse( UnitData * pcThis, UnitData * pcUnitData, int iX, int iY, int iZ )
{
	AddCreedKeep( pcUnitData );
	return TRUE;
}


