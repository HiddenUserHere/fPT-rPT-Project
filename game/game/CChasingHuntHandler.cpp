#include "stdafx.h"
#include "CChasingHuntHandler.h"


CChasingHuntHandler::CChasingHuntHandler()
{
}


CChasingHuntHandler::~CChasingHuntHandler()
{
}

bool CChasingHuntHandler::OnCastAnimationStart( Skill * psSkill )
{
	if ( USESKILL( psSkill ) )
	{
		//Update Skill Data
		UNITDATA->sActiveSkill.cSkillTinyID = SKILLPLAYID_T23;
		UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

		//Animate...
		UNITDATA->Animate( ANIMATIONTYPE_Skill );

		//New FPS based on Weapon Speed
		UNITDATA->iNewFPS = SKILLMANAGERHANDLER->GetAttackFrameSpeed( UNITDATA->sCharacterData.iAttackSpeed );
		UNITDATA->bOverrideFPS = TRUE;

		TIMERSKILLHANDLER->OnAddChasingHunt( ((int*)0x00998B10)[psSkill->iLevel - 1], psSkill->iLevel );

		//Buff
		PROCESSSKILLBUFF( UNITDATA );

		if ( UNITDATA->pcTarget )
		{
			//Area Hook?
			SKILLMANAGERHANDLER->SkillAnimationStart( UNITDATA, UNITDATA->sActiveSkill.iValue, 0, UNITDATA->pcTarget, 0, 0, 0 );

			//Left Side? Send Area Skill Hook
			if ( SKILL_LEFTSIDE == psSkill )
				CALL_WITH_ARG1( 0x0040F3F0, (DWORD)UNITDATA->pcTarget );
		}

		return true;
	}

	return false;
}
