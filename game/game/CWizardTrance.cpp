#include "stdafx.h"
#include "CWizardTrance.h"

CWizardTrance::CWizardTrance()
{
}

CWizardTrance::~CWizardTrance()
{
}

void CWizardTrance::AddTimer( int iTime, int iLevel )
{
	if( !iTimerID )
		iTimerID = TIMERSKILLHANDLER->LoadSkillTimerTexture( "game\\images\\skills\\timers\\Mg5_WizardTrance.tga" );

	Skill * psSkill = TIMERSKILLHANDLER->FindSkillTimer( SKILLID_WizardTrance );

	if( psSkill )
		ZeroMemory( psSkill, sizeof( Skill ) );

	if( !TIMERSKILLHANDLER->FindSkillTimer( SKILLID_WizardTrance ) )
	{
		Skill sSkill;
		ZeroMemory( &sSkill, sizeof( sSkill ) );
		sSkill.iID = SKILLID_WizardTrance;
		sSkill.iUseTime = iTime;
		sSkill.iLevel = iLevel;
		sSkill.MatIcon = iTimerID;
		TIMERSKILLHANDLER->AddSkillTimer( &sSkill );
	}
}

void CWizardTrance::LoadParticle()
{
	iParticleID = PARTICLE->Load( "game\\scripts\\particles\\MG5OrbMastery.part" );
}

bool CWizardTrance::OnCastAnimationStart( Skill * psSkill, UnitData * pcUnitData )
{
	int iIndex1 = INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex;
	int iIndex2 = INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex;

	BOOL bCanUse = FALSE;

	if ( iIndex1 > 0 && iIndex2 > 0 )
	{
		ItemData * psItemData1 = &INVENTORYITEMS[iIndex1 - 1];
		ItemData * psItemData2 = &INVENTORYITEMS[iIndex2 - 1];

		if ( (psItemData1 && psItemData1->sItem.sItemID.ToItemType() == ITEMTYPE_Wand) && (psItemData2 && psItemData2->sItem.sItemID.ToItemType() == ITEMTYPE_Orb) )
			bCanUse = TRUE;
	}


	if( bCanUse && USESKILL( psSkill ) )
	{
		//Update Skill Data
		UNITDATA->sActiveSkill.cSkillTinyID = SKILLPLAYID_T33;
		UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

		//Animate...
		UNITDATA->Animate( ANIMATIONTYPE_Skill );

		UNITDATA->sActiveSkill.cSkillTinyID = SKILLPLAYID_T51;

		//New FPS
		UNITDATA->iNewFPS = 70;
		UNITDATA->bOverrideFPS = TRUE;

		PROCESSSKILLBUFF( UNITDATA );

		OnCastEffect( UNIT, psSkill->iLevel, 300.0f );

		return true;
	}

	return false;
}

BOOL CWizardTrance::OnEvent( UnitData * pcUnitData )
{
	if( pcUnitData->iAnimationEvent == 1 )
	{
		CParticleScript * pc = PARTICLEFACTORY->LoadScript( "game\\scripts\\particles\\MG5WizardTrance.luac" );
		if( pc )
		{
			pc->SetID( SKILLID_WizardTrance );
			Engine::ObjectOwnerUnit * pcOwner = new Engine::ObjectOwnerUnit( pcUnitData );
			pcOwner->AddAnimationTypeLostCondition( ANIMATIONTYPE_Die );
			pc->SetOwner( pcOwner );
			PARTICLEENGINE->AddScript( pc );
		}

		SOUNDHANDLER->SetSkillSound( 0x3710, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );
	}

	return TRUE;
}

void CWizardTrance::OnCastEffect( Unit * pcUnit, UINT uSkillLevel, double fOverTime )
{
	if( pcUnit->bWizardTrance == FALSE )
	{
		pcUnit->bWizardTrance = TRUE;

		if( pcUnit == UNIT )
		{
			AddTimer( 300, uSkillLevel );
		}
	}
}

void CWizardTrance::OnCastEnd( Unit * pcUnit )
{
	if ( pcUnit->bWizardTrance )
	{
		pcUnit->bWizardTrance = FALSE;

		PARTICLEENGINE->Kill( SKILLID_WizardTrance, pcUnit->GetID() );
	}
}
