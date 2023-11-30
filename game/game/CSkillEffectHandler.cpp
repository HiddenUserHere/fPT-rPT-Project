#include "stdafx.h"
#include "CSkillEffectHandler.h"

CSkillEffectHandler::CSkillEffectHandler()
{
	//Priestess T5
	pcMiraculousLight		= new CMiraculousLight();
	pcIceElemental			= new CIceElementalHandler();
	pcVirtualLife			= new CVirtualLifeHandler();
	pcIceMeteorite			= new CIceMeteoriteHandler();

	//Archer T5
	pcLethalSight			= new CLethalSightHandler();
	pcFierceWind			= new CFierceWindHandler();

	//Fighter T5
	pcSesmicImpact			= new CSesmicImpactHandler();

	//Pikeman T5
	pcRingOfSpears			= new CRingsOfSpearsHandler();
	pcLastBreath			= new CLastBreathHandler();

	//Mechanician T5
	pcImpulsion				= new CImpulsionHandler();
	pcRupture				= new CRuptureHandler();
	pcMagneticDischarge		= new CMagneticDischargeHandler();

	//Magician T5
	pcStoneSkin				= new CStoneSkinHandler();
	pcRedRay				= new CRedRayHandler();
	pcWizardTrance			= new CWizardTrance();
	pcEnchantWeapon			= new CEnchantWeaponHandler();
	pcFlameBall				= new CFlameBallHandler();

	//Knight T5
	pcHolyConviction		= new CHolyConvictionHandler();
	pcWhirlWind				= new CWhirlWindHandler();

	//Atalanta T5
	pcSummonTiger			= new CSummonTigerHandler();

	//Assassin
	pcShadowBlink			= new CShadowBlinkHandler();
	pcKissViper				= new CKissViperHandler();

	//Shaman
	pcCreed					= new CCreedHandler();
	pcChasingHunt			= new CChasingHuntHandler();
	pcPressDeityHandler		= new CPressDeityHandler();
}

CSkillEffectHandler::~CSkillEffectHandler()
{
	//Priestess T5
	DELET( pcMiraculousLight );
	DELET( pcIceElemental );
	DELET( pcIceMeteorite );
	DELET( pcVirtualLife );

	//Archer T5
	DELET( pcLethalSight );
	DELET( pcFierceWind );

	//Mechanician T5
	DELET( pcImpulsion );
	DELET( pcRupture );
	DELET( pcMagneticDischarge );

	//Fighter T5
	DELET( pcSesmicImpact );

	//Pikeman T5
	DELET( pcRingOfSpears );
	DELET( pcLastBreath );

	//Magician T5
	DELET( pcStoneSkin );
	DELET( pcRedRay );
	DELET( pcWizardTrance );
	DELET( pcEnchantWeapon );
	DELET( pcFlameBall );

	//Knight T5
	DELET( pcHolyConviction );
	DELET( pcWhirlWind );

	//Atalanta T5
	DELET( pcSummonTiger );

	//Assassin
	DELET( pcShadowBlink );
	DELET( pcKissViper );

	//Shaman
	DELET( pcCreed );
	DELET( pcChasingHunt );
	DELET( pcPressDeityHandler );
}

void CSkillEffectHandler::Init()
{
}

void CSkillEffectHandler::ProcessSkillBuff( UnitData * pcUnitData, UnitData * pcTarget )
{
	int iID = pcTarget != NULL ? pcTarget->iID : 0;

	CALL_WITH_ARG4( 0x00623790, pcUnitData->sActiveSkill.cSkillTinyID, pcUnitData->sActiveSkill.cLevel, iID, 0 );
}

NAKED void CSkillEffectHandler::ProcessSkillBuffEx( int iSkillID, int iLevel, int iParam1, int iParam2 )
{
	JMP( 0x00623790 );
}

void CSkillEffectHandler::Update()
{
	pcShadowBlink->Update();
	pcWhirlWind->Update();

	if ( bInverseAgeTime )
	{
		fTimeAgeItem -= (100.0f / 35.0f);
		if ( fTimeAgeItem <= 0.0f )
		{
			bInverseAgeTime = false;
			fTimeAgeItem = 0.0f;
		}
	}
	else
	{
		fTimeAgeItem += (100.0f / 35.0f);
		if ( fTimeAgeItem >= 60.0f )
		{
			bInverseAgeTime = true;
			fTimeAgeItem = 60.0f;
		}
	}
}
