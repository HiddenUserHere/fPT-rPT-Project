#pragma once

#include "CMiraculousLight.h"
#include "CLethalSightHandler.h"
#include "CFierceWindHandler.h"
#include "RessurectionHandler.h"
#include "CSesmicImpactHandler.h"
#include "CStoneSkinHandler.h"
#include "CRuptureHandler.h"
#include "CIceElementalHandler.h"
#include "CHolyConvictionHandler.h"
#include "CSummonTigerHandler.h"
#include "CRedRayHandler.h"
#include "CRingsOfSpearsHandler.h"
#include "CWizardTrance.h"
#include "CShadowBlinkHandler.h"
#include "CCreedHandler.h"
#include "CChasingHuntHandler.h"
#include "CImpulsionHandler.h"
#include "CLastBreathHandler.h"
#include "CVirtualLifeHandler.h"
#include "CEnchantWeaponHandler.h"
#include "CWhirlWindHandler.h"
#include "CIceMeteoriteHandler.h"
#include "CMagneticDischargeHandler.h"
#include "CPressDeityHandler.h"
#include "CKissViperHandler.h"
#include "CFlameBallHandler.h"


//Skill IDs
#define PARTICLEID_MAGNETIC_DISCHARGE		4000
#define PARTICLEID_KISS_VIPER				4001
#define PARTICLEID_FLAME_BALL				4002
#define PARTICLEID_MONSTERS_CURSED_DESERT	4502

//Priestess T5
#define MIRACULOUSLIGHTHANDLER	(SKILLEFFECTHANDLER->GetMiraculousLight())
#define RESSURECTIONHANDLER		(SKILLEFFECTHANDLER->GetRessurection())
#define ICEELEMENTALHANDLER		(SKILLEFFECTHANDLER->GetIceElemental())
#define VIRTUALLIFEHANDLER		(SKILLEFFECTHANDLER->GetVirtualLife())
#define ICEMETEORITE			(SKILLEFFECTHANDLER->GetIceMeteorite())

//Archer T5
#define LETHALSIGHTHANDLER		(SKILLEFFECTHANDLER->GetLethalSight())
#define FIERCEWINDHANDLER		(SKILLEFFECTHANDLER->GetFierceWind())

//Pikeman T5
#define RINGOFSPEARSHANDLER		(SKILLEFFECTHANDLER->GetRingOfSpears())
#define LASTBREATHHANDLER		(SKILLEFFECTHANDLER->GetLastBreath())

//Fighter T5
#define	SESMICIMPACTHANDLER		(SKILLEFFECTHANDLER->GetSesmicImpact())

//Mechanician T5
#define IMPULSIONHANDLER		(SKILLEFFECTHANDLER->GetImpulsion())
#define RUPTUREHANDLER			(SKILLEFFECTHANDLER->GetRupture())
#define MAGNETICDISCHARGEHANDLER (SKILLEFFECTHANDLER->GetMagneticDischarge())

//Magician T5
#define STONESKINHANDLER		(SKILLEFFECTHANDLER->GetStoneSkin())
#define REDRAYHANDLER			(SKILLEFFECTHANDLER->GetRedRay())
#define WIZARDTRANCEHANDLER		(SKILLEFFECTHANDLER->GetWizardTrance())
#define ENCHANTWEAPONHANDLER	(SKILLEFFECTHANDLER->GetEnchantWeapon())
#define FLAMEBALLHANDLER		(SKILLEFFECTHANDLER->GetFlameBall())

//Knight T5
#define HOLYCONVICTIONHANDLER	(SKILLEFFECTHANDLER->GetHolyConviction())
#define WHIRLWINDHANDLER		(SKILLEFFECTHANDLER->GetWhirlWind())

//Atalanta T5
#define SUMMONTIGERHANDLER		(SKILLEFFECTHANDLER->GetSummonTiger())

//Assassin
#define SHADOWBLINKHANDLER		(SKILLEFFECTHANDLER->GetShadowBlink())
#define KISSVIPERHANDLER		(SKILLEFFECTHANDLER->GetKissViper())

//Shaman
#define CREEDHANDLER			(SKILLEFFECTHANDLER->GetCreed())
#define CHASINGHUNTHANDLER		(SKILLEFFECTHANDLER->GetChasingHunt())
#define PRESSDEITYHANDLER		(SKILLEFFECTHANDLER->GetPressDeity())

#define USESKILL(s)				(CALL((DWORD)((Skill*)s)->sSkillInfo.dwFunctionPointer))
#define PROCESSSKILLBUFFT(ud, target) (SKILLEFFECTHANDLER->ProcessSkillBuff(ud, target))
#define PROCESSSKILLBUFF(ud)	(SKILLEFFECTHANDLER->ProcessSkillBuff(ud, NULL))
#define PROCESSSKILLBUFFEX		SKILLEFFECTHANDLER->ProcessSkillBuffEx

class CSkillEffectHandler
{
public:
								CSkillEffectHandler();
	virtual					   ~CSkillEffectHandler();

	//Priestess T5
	CMiraculousLight			* GetMiraculousLight() { return pcMiraculousLight; };
	CRessurectionHandler		* GetRessurection() { return pcRessurection; }
	CIceElementalHandler		* GetIceElemental() { return pcIceElemental; }
	CVirtualLifeHandler			* GetVirtualLife() { return pcVirtualLife; }
	CIceMeteoriteHandler		* GetIceMeteorite() { return pcIceMeteorite; }

	//Archer T5
	CLethalSightHandler			* GetLethalSight() { return pcLethalSight; };
	CFierceWindHandler			* GetFierceWind() { return pcFierceWind; };

	//Fighter T5
	CSesmicImpactHandler		* GetSesmicImpact() { return pcSesmicImpact; }

	//Pikeman T5
	CRingsOfSpearsHandler		* GetRingOfSpears(){ return pcRingOfSpears; }
	CLastBreathHandler			* GetLastBreath() { return pcLastBreath; }

	//Mechanician T5
	CImpulsionHandler			* GetImpulsion() { return pcImpulsion; }
	CRuptureHandler				* GetRupture() { return pcRupture; }
	CMagneticDischargeHandler	* GetMagneticDischarge() { return pcMagneticDischarge; }

	//Magician T5
	CStoneSkinHandler			* GetStoneSkin() { return pcStoneSkin; }
	CRedRayHandler				* GetRedRay(){ return pcRedRay; }
	CWizardTrance				* GetWizardTrance(){ return pcWizardTrance; }
	CEnchantWeaponHandler		* GetEnchantWeapon() { return pcEnchantWeapon; }
	CFlameBallHandler			* GetFlameBall() { return pcFlameBall; }

	//Knight T5
	CHolyConvictionHandler		* GetHolyConviction() { return pcHolyConviction; }
	CWhirlWindHandler			* GetWhirlWind() { return pcWhirlWind; }

	//Atalanta
	CSummonTigerHandler			* GetSummonTiger(){ return pcSummonTiger; }

	//Assassin
	CShadowBlinkHandler			* GetShadowBlink() { return pcShadowBlink; }
	CKissViperHandler			* GetKissViper() { return pcKissViper; }

	//Shaman
	CCreedHandler				* GetCreed() { return pcCreed; }
	CChasingHuntHandler			* GetChasingHunt() { return pcChasingHunt; }
	CPressDeityHandler			* GetPressDeity() { return pcPressDeityHandler; }

	void						Init();

	void						ProcessSkillBuff( UnitData * pcUnitData, UnitData * pcTarget );
	static void					ProcessSkillBuffEx( int iSkillID, int iLevel, int iParam1 = 0, int iParam2 = 0 );

	void						Update();

	float						GetTimeAge() { return fTimeAgeItem; }

protected:
	//Priestess T5
	CMiraculousLight			* pcMiraculousLight;
	CRessurectionHandler		* pcRessurection;
	CIceElementalHandler		* pcIceElemental;
	CVirtualLifeHandler			* pcVirtualLife;
	CIceMeteoriteHandler		* pcIceMeteorite;

	//Archer T5
	CLethalSightHandler			* pcLethalSight;
	CFierceWindHandler			* pcFierceWind;

	//Fighter T5
	CSesmicImpactHandler		* pcSesmicImpact;

	//Pikeman T5
	CRingsOfSpearsHandler		* pcRingOfSpears;
	CLastBreathHandler			* pcLastBreath;

	//Mechanician T5
	CImpulsionHandler			* pcImpulsion;
	CRuptureHandler				* pcRupture;
	CMagneticDischargeHandler	* pcMagneticDischarge;

	//Magician T5
	CStoneSkinHandler			* pcStoneSkin;
	CRedRayHandler				* pcRedRay;
	CWizardTrance				* pcWizardTrance;
	CEnchantWeaponHandler		* pcEnchantWeapon;
	CFlameBallHandler			* pcFlameBall;

	//Knight T5
	CHolyConvictionHandler		* pcHolyConviction;
	CWhirlWindHandler			* pcWhirlWind;

	//Atalanta T5
	CSummonTigerHandler			* pcSummonTiger;

	//Assassin
	CShadowBlinkHandler			* pcShadowBlink;
	CKissViperHandler			* pcKissViper;

	//Shaman
	CCreedHandler				* pcCreed;
	CChasingHuntHandler			* pcChasingHunt;
	CPressDeityHandler			* pcPressDeityHandler;

	bool						bInverseAgeTime = false;
	float						fTimeAgeItem = 0.0f;

};

