#pragma once

#include "PSkill.h"

#include "item.h"

#define SKILLCONTAINER_LENGTH	(MAX_PKTSIZ - 0x20)
#define MAX_SKILLS				230

#define MAX_MASTERY				1000

struct TwoArraySkill
{
	union
	{
		int				iaValue[10][2];
		int				faValue[10][2];
	};
};

struct SkillTable
{
	int			iaFighterT11WeaponAttackPowerAdd[10];

	int			iaFighterT12FireResistanceIncrease[10];

	int			iaFighterT13DamageBoost[10];
	int			iaFighterT13NumberofHits[10];
	float			faFighterT13HPDecrease[10];
	int			iaFighterT13MPCost[10];
	int			iaFighterT13SPCost[10];

	int			iaFighterT14AddAttackRating[10];
	int			iaFighterT14DamageBoost[10];
	int			iaFighterT14MPCost[10];
	int			iaFighterT14SPCost[10];

	int			iaFighterT21DamageBoost[10];
	int			iaFighterT21NumberofHits[10];
	int			iaFighterT21MPCost[10];
	int			iaFighterT21SPCost[10];

	int			iaFighterT22DamageBoost[10];
	int			iaFighterT22AddCritical[10];
	int			iaFighterT22MPCost[10];
	int			iaFighterT22SPCost[10];

	int			iaFighterT23AreaofEffect[10];
	int			iaFighterT23Duration[10];
	int			iaFighterT23MPCost[10];
	int			iaFighterT23SPCost[10];

	int			iaFighterT24DamageBoost[10];
	int			iaFighterT24AreaofEffect[10];
	int			iaFighterT24MPCost[10];
	int			iaFighterT24SPCost[10];

	int			iaFighterT31AddAttackRating[10];
	int			iaFighterT31Duration[10];
	int			iaFighterT31MPCost[10];
	int			iaFighterT31SPCost[10];

	int			iaFighterT32AddAttackRating[10];
	int			iaFighterT32DamageBoost[10];
	int			iaFighterT32MPCost[10];
	int			iaFighterT32SPCost[10];

	int			iaFighterT33AttackSpeedBoost[10];
	int			iaFighterT33Duration[10];
	int			iaFighterT33MPCost[10];
	int			iaFighterT33SPCost[10];

	int			iaFighterT34DamageBoost[10];
	int			iaFighterT34DamageBoostvsDemons[10];
	int			iaFighterT34MPCost[10];
	int			iaFighterT34SPCost[10];

	int			iaFighterT41DamageBoost[10];
	int			iaFighterT41AddCritical[10];
	int			iaFighterT41MPCost[10];
	int			iaFighterT41SPCost[10];

	int			iaFighterT42AddBaseAttackPower[10];
	int			iaFighterT42DecreaseAbsorption[10];
	int			iaFighterT42Duration[10];
	int			iaFighterT42MPCost[10];
	int			iaFighterT42SPCost[10];

	int			iaFighterT43DamageBoost[10];
	int			iaFighterT43MaximumHits[10];
	int			iaFighterT43Range[10];
	int			iaFighterT43MPCost[10];
	int			iaFighterT43SPCost[10];

	int			iaFighterT44HPIncrease[10];

	float			faFighterT51AddHPRegen[10];
	int			iaFighterT51BoostHPPotionRecovery[10];
	int			iaFighterT51BoostHPCapacity[10];

	int			iaFighterT52DamageBoost[10];
	int			iaFighterT52ImpactArea[10];
	int			iaFighterT52MPCost[10];
	int			iaFighterT52SPCost[10];

	int			iaFighterT53HPDecrease[10];
	int			iaFighterT53HitsNeededtoTrigger[10];
	int			iaFighterT53FinalDamageBoostIncrease[10];
	int			iaFighterT53Duration[10];
	int			iaFighterT53MPCost[10];
	int			iaFighterT53SPCost[10];

	int			iaFighterT54DamageBoost[10];
	int			iaFighterT54Duration[10];
	int			iaFighterT54MPDrain[10];
	int			iaFighterT54SPDrain[10];

	//Mechanician
	int			iaMechanicianT11ShieldBlockBoost[10];
	int			iaMechanicianT11Duration[10];
	int			iaMechanicianT11MPCost[10];
	int			iaMechanicianT11SPCost[10];

	int			iaMechanicianT12StaticDamage[10];
	int			iaMechanicianT12AddAttackPower[10];
	int			iaMechanicianT12AreaofEffect[10];
	int			iaMechanicianT12MPCost[10];
	int			iaMechanicianT12SPCost[10];

	int			iaMechanicianT13PoisonResistanceIncrease[10];

	int			iaMechanicianT14AbsorptionBoost[10];
	int			iaMechanicianT14Duration[10];
	int			iaMechanicianT14MPCost[10];
	int			iaMechanicianT14SPCost[10];

	int			iaMechanicianT21CalculatedDamage[10];
	int			iaMechanicianT21AreaofEffect[10];
	int			iaMechanicianT21MPCost[10];
	int			iaMechanicianT21SPCost[10];

	int			iaMechanicianT22AddAttackPower[10];
	int			iaMechanicianT22AddAttackRating[10];
	int			iaMechanicianT22Duration[10];
	int			iaMechanicianT22MPCost[10];
	int			iaMechanicianT22SPCost[10];

	int			iaMechanicianT23DamageBoost[10];
	int			iaMechanicianT23AddAttackSpeed[10];
	float			faMechanicianT23MPDrainperhit[10];
	float			faMechanicianT23SPDrainperhit[10];
	int			iaMechanicianT23MPCost[10];
	int			iaMechanicianT23SPCost[10];

	int			iaMechanicianT24DamageBoost[10];
	int			iaMechanicianT24AreaofEffect[10];
	int			iaMechanicianT24MPCost[10];
	int			iaMechanicianT24SPCost[10];

	int			iaMechanicianT31ArmorDefenseRatingBoost[10];
	int			iaMechanicianT31Duration[10];
	int			iaMechanicianT31MPCost[10];
	int			iaMechanicianT31SPCost[10];

	int			iaMechanicianT32AttackRatingBoost[10];
	int			iaMechanicianT32DamageBoost[10];
	int			iaMechanicianT32MPCost[10];
	int			iaMechanicianT32SPCost[10];

	int			iaMechanicianT33BaseAttackPowerIncrease[10];

	int			iaMechanicianT34SparkDamage[10];
	int			iaMechanicianT34AddDefenseRating[10];
	int			iaMechanicianT34Duration[10];
	int			iaMechanicianT34MPCost[10];
	int			iaMechanicianT34SPCost[10];

	int			iaMechanicianT41DamageBoost[10];
	int			iaMechanicianT41AreaofEffect[10];
	int			iaMechanicianT41MaxHits[10];
	int			iaMechanicianT41MPCost[10];
	int			iaMechanicianT41SPCost[10];

	int			iaMechanicianT42IncreaseAbsorption[10];
	int			iaMechanicianT42BoostAbsorption[10];
	int			iaMechanicianT42MonsterSightRangeIncrease[10];
	int			iaMechanicianT42Duration[10];
	int			iaMechanicianT42MPCost[10];
	int			iaMechanicianT42SPCost[10];

	int			iaMechanicianT43SpheresDamage[10];
	int			iaMechanicianT43SpheresAttackRange[10];
	int			iaMechanicianT43SpheresAttackDelay[10];
	int			iaMechanicianT43Duration[10];
	int			iaMechanicianT43MPCost[10];
	int			iaMechanicianT43SPCost[10];

	int			iaMechanicianT44SummonDuration[10];
	int			iaMechanicianT44SummonAttackPowerMin[10];
	int			iaMechanicianT44SummonAttackPowerMax[10];
	int			iaMechanicianT44SummonAttackRating[10];
	int			iaMechanicianT44SummonDefense[10];
	int			iaMechanicianT44SummonHP[10];
	int			iaMechanicianT44MPCost[10];
	int			iaMechanicianT44SPCost[10];

	int			iaMechanicianT51DamageBoost[10];
	int			iaMechanicianT51MPCost[10];
	int			iaMechanicianT51SPCost[10];

	int			iaMechanicianT52DamageBoost[10];
	int			iaMechanicianT52MPCost[10];
	int			iaMechanicianT52SPCost[10];

	int			iaMechanicianT53ReceivedDamageReduction[10];
	int			iaMechanicianT53AttackRatingReduction[10];
	int			iaMechanicianT53Duration[10];
	int			iaMechanicianT53MPCost[10];
	int			iaMechanicianT53SPCost[10];

	int			iaMechanicianT54DamageBoost[10];
	int			iaMechanicianT54PoisonDuration[10];
	int			iaMechanicianT54MPCost[10];
	int			iaMechanicianT54SPCost[10];

	//Archer
	int			iaArcherT11AddAttackRating[10];
	int			iaArcherT11HawkRotations[10];
	int			iaArcherT11MPCost[10];
	int			iaArcherT11SPCost[10];

	int			iaArcherT12AddAttackPower[10];

	int			iaArcherT13AddAttackSpeed[10];
	int			iaArcherT13DamageBoost[10];
	int			iaArcherT13MPCost[10];
	int			iaArcherT13SPCost[10];

	int			iaArcherT14AddAttackRating[10];
	int			iaArcherT14DamageBoost[10];
	int			iaArcherT14MPCost[10];
	int			iaArcherT14SPCost[10];

	int			iaArcherT21AddAttackRating[10];

	int			iaArcherT22StaticDamage[10];
	int			iaArcherT22FormulaDamage[10];
	int			iaArcherT22Duration[10];
	int			iaArcherT22MPCost[10];
	int			iaArcherT22SPCost[10];

	int			iaArcherT23DamageBoost[10];
	int			iaArcherT23AreaofEffect[10];
	int			iaArcherT23MPCost[10];
	int			iaArcherT23SPCost[10];

	int			iaArcherT24AddAttackSpeed[10];
	int			iaArcherT24DamageBoost[10];
	int			iaArcherT24NumberofArrows[10];
	int			iaArcherT24MPCost[10];
	int			iaArcherT24SPCost[10];

	int			iaArcherT31FireDamageStatic[10];
	int			iaArcherT31FireDamageCalculated[10];
	int			iaArcherT31LightningDamageStatic[10];
	int			iaArcherT31LightningDamageCalculated[10];
	int			iaArcherT31MPCost[10];
	int			iaArcherT31SPCost[10];

	int			iaArcherT32FalconDamage[10];
	float			faArcherT32HPRegen[10];
	int			iaArcherT32Duration[10];
	int			iaArcherT32MPCost[10];
	int			iaArcherT32SPCost[10];

	int			iaArcherT33DamageBoost[10];
	int			iaArcherT33DamageBoostvsDemonMonsters[10];
	int			iaArcherT33AreaofEffect[10];
	int			iaArcherT33MPCost[10];
	int			iaArcherT33SPCost[10];

	int			iaArcherT34IncreasedShotRange[10];
	int			iaArcherT34DamageBoost[10];
	int			iaArcherT34MPCost[10];
	int			iaArcherT34SPCost[10];

	int			iaArcherT41Duration[10];
	int			iaArcherT41SummonAttackPowerMin[10];
	int			iaArcherT41SummonAttackPowerMax[10];
	int			iaArcherT41SummonAttackRating[10];
	int			iaArcherT41SummonDefense[10];
	int			iaArcherT41SummonHP[10];
	int			iaArcherT41MPCost[10];
	int			iaArcherT41SPCost[10];

	int			iaArcherT42IncreaseEvasion[10];

	int			iaArcherT43ShotRangeIncrease[10];
	int			iaArcherT43DamageBoost[10];
	float			faArcherT43ChargeRaiseSpeed[10];
	int			iaArcherT43MPCost[10];
	int			iaArcherT43SPCost[10];

	int			iaArcherT44AddAttackPower[10];
	int			iaArcherT44AddAttackRating[10];
	int			iaArcherT44Duration[10];
	int			iaArcherT44MPCost[10];
	int			iaArcherT44SPCost[10];

	int			iaArcherT51AddRange[10];
	int			iaArcherT51CriticalRateIncrease[10];
	int			iaArcherT51AfterDamage[10];
	int			iaArcherT51MPCost[10];
	int			iaArcherT51SPCost[10];

	int			iaArcherT52DamageBoost[10];
	int			iaArcherT52CriticalDamageBoost[10];
	int			iaArcherT52MPCost[10];
	int			iaArcherT52SPCost[10];

	int			iaArcherT53EffectDuration[10];
	int			iaArcherT53MPCost[10];
	int			iaArcherT53SPCost[10];

	int			iaArcherT54DamageBoost[10];
	int			iaArcherT54EffectDuration[10];
	int			iaArcherT54MPDrain[10];
	int			iaArcherT54SPDrain[10];

	//Pikeman
	int			iaPikemanT11AttackPowerMin[10];
	int			iaPikemanT11AttackPowerMax[10];
	int			iaPikemanT11PushbackRange[10];
	int			iaPikemanT11MPCost[10];
	int			iaPikemanT11SPCost[10];

	int			iaPikemanT12FrostResistanceIncrease[10];

	int			iaPikemanT13AddCriticalDamage[10];
	int			iaPikemanT13MPCost[10];
	int			iaPikemanT13SPCost[10];

	int			iaPikemanT14WeaponAttackRatingBoost[10];
	int			iaPikemanT14DamageBoost[10];
	int			iaPikemanT14MPCost[10];
	int			iaPikemanT14SPCost[10];

	int			iaPikemanT21FreezeDuration[10];
	int			iaPikemanT21FreezeAttackPowerMin[10];
	int			iaPikemanT21FreezeAttackPowerMax[10];
	int			iaPikemanT21FreezeAttackRange[10];
	int			iaPikemanT21MPCost[10];
	int			iaPikemanT21SPCost[10];

	int			iaPikemanT22DamageBoost[10];
	int			iaPikemanT22Range[10];
	int			iaPikemanT22MPCost[10];
	int			iaPikemanT22SPCost[10];

	int			iaPikemanT23WeaponCurrentBlockBoost[10];

	int			iaPikemanT24AddSize[10];
	int			iaPikemanT24DamageBoost[10];
	int			iaPikemanT24MPCost[10];
	int			iaPikemanT24SPCost[10];

	int			iaPikemanT31DamageBoost[10];
	int			iaPikemanT31NumberofSpears[10];
	int			iaPikemanT31EffectDuration[10];
	int			iaPikemanT31EffectRange[10];
	int			iaPikemanT31MPCost[10];
	int			iaPikemanT31SPCost[10];

	int			iaPikemanT32AfterDamage[10];
	int			iaPikemanT32MonsterSightDecrease[10];
	int			iaPikemanT32Duration[10];
	int			iaPikemanT32MPCost[10];
	int			iaPikemanT32SPCost[10];

	int			iaPikemanT33WeaponCriticalRatingBoost[10];

	int			iaPikemanT34DamageBoost[10];
	int			iaPikemanT34MPCost[10];
	int			iaPikemanT34SPCost[10];

	int			iaPikemanT41AddCriticalDamage[10];
	int			iaPikemanT41TargetCriticalDecrease[10];
	int			iaPikemanT41DebuffDuration[10];
	int			iaPikemanT41MPCost[10];
	int			iaPikemanT41SPCost[10];

	int			iaPikemanT42DamageBoost[10];
	int			iaPikemanT42ChargedDamageBoost[10];
	int			iaPikemanT42RaiseChargingSpeed[10];
	int			iaPikemanT42MPCost[10];
	int			iaPikemanT42SPCost[10];

	int			iaPikemanT43AddEvasion[10];
	int			iaPikemanT43Duration[10];
	int			iaPikemanT43MPCost[10];
	int			iaPikemanT43SPCost[10];

	int			iaPikemanT44DamageBoost[10];
	int			iaPikemanT44NumberofHits[10];
	int			iaPikemanT44MPCost[10];
	int			iaPikemanT44SPCost[10];

	int			iaPikemanT51Duration[10];
	int			iaPikemanT51MPCost[10];
	int			iaPikemanT51SPCost[10];

	int			iaPikemanT52DamageBoost[10];
	float			faPikemanT52Cooldown[10];
	int			iaPikemanT52MPCost[10];
	int			iaPikemanT52SPCost[10];

	int			iaPikemanT53DamageBoost[10];
	int			iaPikemanT53WeaponAttackRatingBoost[10];
	int			iaPikemanT53MPCost[10];
	int			iaPikemanT53SPCost[10];

	int			iaPikemanT54DamageBoost[10];
	int			iaPikemanT54MPCost[10];
	int			iaPikemanT54SPCost[10];

	//Atalanta
	int			iaAtalantaT11AttackPowerMin[10];
	int			iaAtalantaT11AttackPowerMax[10];
	int			iaAtalantaT11MPCost[10];
	int			iaAtalantaT11SPCost[10];

	int			iaAtalantaT12DamageBoost[10];
	int			iaAtalantaT12AddAttackSpeed[10];
	int			iaAtalantaT12AddAttackRating[10];
	int			iaAtalantaT12MPCost[10];
	int			iaAtalantaT12SPCost[10];

	int			iaAtalantaT13AddAttackPower[10];

	int			iaAtalantaT14DamageBoost[10];
	int			iaAtalantaT14MPCost[10];
	int			iaAtalantaT14SPCost[10];

	int			iaAtalantaT21AttackRatingBoost[10];
	int			iaAtalantaT21AddRange[10];
	int			iaAtalantaT21Duration[10];
	int			iaAtalantaT21MPCost[10];
	int			iaAtalantaT21SPCost[10];

	int			iaAtalantaT22DamageBoost[10];
	int			iaAtalantaT22AttackRatingBoost[10];
	int			iaAtalantaT22MPCost[10];
	int			iaAtalantaT22SPCost[10];

	int			iaAtalantaT23MonsterLifeAbsorption[10];
	int			iaAtalantaT23MPCost[10];
	int			iaAtalantaT23SPCost[10];

	int			iaAtalantaT24DamageBoost[10];
	int			iaAtalantaT24MPCost[10];
	int			iaAtalantaT24SPCost[10];

	int			iaAtalantaT31NumberofHits[10];
	int			iaAtalantaT31DamageBoost[10];
	int			iaAtalantaT31AttackRatingBoost[10];
	int			iaAtalantaT31MPCost[10];
	int			iaAtalantaT31SPCost[10];

	int			iaAtalantaT32MaxAttackPower[10];
	int			iaAtalantaT32Duration[10];
	int			iaAtalantaT32MPCost[10];
	int			iaAtalantaT32SPCost[10];

	int			iaAtalantaT33DamageBoost[10];
	int			iaAtalantaT33MPCost[10];
	int			iaAtalantaT33SPCost[10];

	int			iaAtalantaT34DamageBoost[10];
	int			iaAtalantaT34MPCost[10];
	int			iaAtalantaT34SPCost[10];

	int			iaAtalantaT41AddEvasion[10];
	int			iaAtalantaT41Duration[10];
	int			iaAtalantaT41MPCost[10];
	int			iaAtalantaT41SPCost[10];

	int			iaAtalantaT42DamageBoost[10];
	int			iaAtalantaT42AreaofEffect[10];
	int			iaAtalantaT42MPCost[10];
	int			iaAtalantaT42SPCost[10];

	int			iaAtalantaT43AddAttackPowerMin[10];
	int			iaAtalantaT43AddAttackPowerMax[10];
	int			iaAtalantaT43TargetRunningSpeedReduction[10];
	int			iaAtalantaT43TargetFreezeEffectDuration[10];
	int			iaAtalantaT43Duration[10];
	int			iaAtalantaT43MPCost[10];
	int			iaAtalantaT43SPCost[10];

	int			iaAtalantaT44DamageBoost[10];
	int			iaAtalantaT44AddRange[10];
	int			iaAtalantaT44MPCost[10];
	int			iaAtalantaT44SPCost[10];

	int			iaAtalantaT51SummonAttackPowerMin[10];
	int			iaAtalantaT51SummonAttackPowerMax[10];
	int			iaAtalantaT51SummonAttackRating[10];
	int			iaAtalantaT51SummonHP[10];
	int			iaAtalantaT51SummonDuration[10];
	int			iaAtalantaT51MPCost[10];
	int			iaAtalantaT51SPCost[10];

	int			iaAtalantaT52SpeedIncrease[10];
	int			iaAtalantaT52AddEvasion[10];

	int			iaAtalantaT53DamageBoost[10];
	int			iaAtalantaT53Targets[10];
	int			iaAtalantaT53PoisonDuration[10];
	int			iaAtalantaT53MPCost[10];
	int			iaAtalantaT53SPCost[10];

	int			iaAtalantaT54DamageBoost[10];
	int			iaAtalantaT54MPCost[10];
	int			iaAtalantaT54SPCost[10];

	//Knight
	int			iaKnightT11StaticDamage[10];
	int			iaKnightT11DamageBoost[10];
	int			iaKnightT11LinearAreaofEffect[10];
	int			iaKnightT11MPCost[10];
	int			iaKnightT11SPCost[10];

	int			iaKnightT12DamageBoostvsUndeadMonsters[10];
	int			iaKnightT12Duration[10];
	int			iaKnightT12MPCost[10];
	int			iaKnightT12SPCost[10];

	int			iaKnightT13SPBoost[10];

	int			iaKnightT14DamageBoost[10];
	int			iaKnightT14AddCritical[10];
	int			iaKnightT14MPCost[10];
	int			iaKnightT14SPCost[10];

	int			iaKnightT21PartyArea[10];
	int			iaKnightT21FinalDamageBoostvsUndeadEnemies[10];
	int			iaKnightT21Duration[10];
	int			iaKnightT21MPCost[10];
	int			iaKnightT21SPCost[10];

	int			iaKnightT22AreaofEffect[10];
	int			iaKnightT22DamageBoost[10];
	int			iaKnightT22MPCost[10];
	int			iaKnightT22SPCost[10];

	int			iaKnightT23DamageBoost[10];
	int			iaKnightT23AreaofEffect[10];
	int			iaKnightT23PierceRange[10];
	int			iaKnightT23MPCost[10];
	int			iaKnightT23SPCost[10];

	int			iaKnightT24DefenseBoost[10];
	int			iaKnightT24Duration[10];
	int			iaKnightT24MPCost[10];
	int			iaKnightT24SPCost[10];

	int			iaKnightT31BaseAttackPowerIncrease[10];

	int			iaKnightT32AddBlockRating[10];
	int			iaKnightT32MonsterDamageIntoHP[10];
	int			iaKnightT32Duration[10];
	int			iaKnightT32MPCost[10];
	int			iaKnightT32SPCost[10];

	int			iaKnightT33SuccessRate[10];
	int			iaKnightT33MonsterLifeIncrease[10];
	int			iaKnightT33EnslaveEffectDuration[10];
	int			iaKnightT33MPCost[10];
	int			iaKnightT33SPCost[10];

	int			iaKnightT34AttackRatingBoost[10];
	int			iaKnightT34DamageBoost[10];
	int			iaKnightT34DamageBoostvsUndeadEnemies[10];
	int			iaKnightT34MPCost[10];
	int			iaKnightT34SPCost[10];

	int			iaKnightT41DamageBoost[10];
	int			iaKnightT41AreaofEffect[10];
	int			iaKnightT41MPCost[10];
	int			iaKnightT41SPCost[10];

	int			iaKnightT42ShieldAbsorptionBoost[10];
	int			iaKnightT42Duration[10];
	int			iaKnightT42MPCost[10];
	int			iaKnightT42SPCost[10];

	int			iaKnightT43AddAttackPower[10];
	int			iaKnightT43Duration[10];
	int			iaKnightT43MPCost[10];
	int			iaKnightT43SPCost[10];

	int			iaKnightT44AddCritical[10];
	int			iaKnightT44DamageBoost[10];
	int			iaKnightT44NumberofHits[10];
	int			iaKnightT44MPCost[10];
	int			iaKnightT44SPCost[10];

	int			iaKnightT51UndeadDamageBoost[10];
	int			iaKnightT51MPCost[10];
	int			iaKnightT51SPCost[10];

	int			iaKnightT52DamageBoost[10];
	int			iaKnightT52AreaofEffect[10];
	int			iaKnightT52AddAtkRtgBoost[10];
	int			iaKnightT52Duration[10];
	int			iaKnightT52MPCost[10];
	int			iaKnightT52SPCost[10];
	float			faKnightT52Cooldown[10];

	int			iaKnightT53ShieldDefenseBoost[10];
	int			iaKnightT53MPCost[10];
	int			iaKnightT53SPCost[10];

	int			iaKnightT54DamageBoost[10];
	int			iaKnightT54MPCost[10];
	int			iaKnightT54SPCost[10];

	//Magician
	int			iaMagicianT11HPSacrificed[10];
	int			iaMagicianT11MPIncreased[10];
	int			iaMagicianT11MPCost[10];
	int			iaMagicianT11SPCost[10];

	int			iaMagicianT12StaticDamage[10];
	int			iaMagicianT12DamageBoost[10];
	int			iaMagicianT12AttackRange[10];
	int			iaMagicianT12MPCost[10];
	int			iaMagicianT12SPCost[10];

	int			iaMagicianT13AddAbsorption[10];
	int			iaMagicianT13AddElementalResistance[10];
	int			iaMagicianT13Duration[10];
	int			iaMagicianT13MPCost[10];
	int			iaMagicianT13SPCost[10];

	int			iaMagicianT14StaticDamage[10];
	int			iaMagicianT14DamageBoost[10];
	int			iaMagicianT14AttackRange[10];
	int			iaMagicianT14AreaofEffect[10];
	int			iaMagicianT14MPCost[10];
	int			iaMagicianT14SPCost[10];

	int			iaMagicianT21MPPotionRecoveryBoost[10];
	int			iaMagicianT21MPCapacityIncrease[10];

	int			iaMagicianT22DamageBoost[10];
	int			iaMagicianT22AttackRange[10];
	int			iaMagicianT22AreaofEffect[10];
	int			iaMagicianT22MPCost[10];
	int			iaMagicianT22SPCost[10];

	int			iaMagicianT23FireAttackPowerMin[10];
	int			iaMagicianT23FireAttackPowerMax[10];
	int			iaMagicianT23IceAttackPowerMin[10];
	int			iaMagicianT23IceAttackPowerMax[10];
	int			iaMagicianT23LightningAttackPowerMin[10];
	int			iaMagicianT23LightningAttackPowerMax[10];
	int			iaMagicianT23Duration[10];
	int			iaMagicianT23MPCost[10];
	int			iaMagicianT23SPCost[10];

	int			iaMagicianT24DamageBoost[10];
	int			iaMagicianT24AttackRange[10];
	int			iaMagicianT24MPCost[10];
	int			iaMagicianT24SPCost[10];

	int			iaMagicianT31ReduceDamage[10];
	int			iaMagicianT31Duration[10];
	int			iaMagicianT31MPCost[10];
	int			iaMagicianT31SPCost[10];

	int			iaMagicianT32DamageBoost[10];
	int			iaMagicianT32AreaofEffect[10];
	int			iaMagicianT32MPCost[10];
	int			iaMagicianT32SPCost[10];

	int			iaMagicianT33AddAttackPowerMin[10];
	int			iaMagicianT33AddAttackPowerMax[10];
	int			iaMagicianT33Duration[10];
	int			iaMagicianT33MPCost[10];
	int			iaMagicianT33SPCost[10];

	int			iaMagicianT34FireDamage[10];
	int			iaMagicianT34IceDamage[10];
	int			iaMagicianT34SwordAttackDelay[10];
	int			iaMagicianT34Duration[10];
	int			iaMagicianT34MPCost[10];
	int			iaMagicianT34SPCost[10];

	int			iaMagicianT41SummonDuration[10];
	int			iaMagicianT41SummonAttackPowerMin[10];
	int			iaMagicianT41SummonAttackPowerMax[10];
	int			iaMagicianT41SummonAttackRating[10];
	int			iaMagicianT41SummonHP[10];
	int			iaMagicianT41MPCost[10];
	int			iaMagicianT41SPCost[10];

	int			iaMagicianT42DamageBoost[10];
	int			iaMagicianT42AreaofEffect[10];
	int			iaMagicianT42MPCost[10];
	int			iaMagicianT42SPCost[10];

	int			iaMagicianT43TargetSpeedReduction[10];
	int			iaMagicianT43AreaofEffect[10];
	int			iaMagicianT43EffectDuration[10];
	int			iaMagicianT43MPCost[10];
	int			iaMagicianT43SPCost[10];

	int			iaMagicianT44DamageBoost[10];
	int			iaMagicianT44AreaofEffect[10];
	int			iaMagicianT44MPCost[10];
	int			iaMagicianT44SPCost[10];

	int			iaMagicianT51OrbBlockBoost[10];
	int			iaMagicianT51AttackRateReduced[10];
	int			iaMagicianT51MPCost[10];
	int			iaMagicianT51SPCost[10];

	int			iaMagicianT52EffectDuration[10];
	int			iaMagicianT52RunSpeedReduction[10];
	int			iaMagicianT52MPCost[10];
	int			iaMagicianT52SPCost[10];

	int			iaMagicianT53DamageBoost[10];
	int			iaMagicianT53IncreasedAttackRange[10];
	int			iaMagicianT53MPCost[10];
	int			iaMagicianT53SPCost[10];

	int			iaMagicianT54DamageBoost[10];
	int			iaMagicianT54AttackDuration[10];
	int			iaMagicianT54MPDrain[10];
	int			iaMagicianT54SPDrain[10];

	//Priestess
	int			iaPriestessT11FormulaBasedLifeRecovery[10];
	int			iaPriestessT11AttackPowerBasedLifeRecovery[10];
	int			iaPriestessT11MPCost[10];
	int			iaPriestessT11SPCost[10];

	int			iaPriestessT12FormulaBasedDamage[10];
	int			iaPriestessT12DamageBoost[10];
	int			iaPriestessT12MPCost[10];
	int			iaPriestessT12SPCost[10];

	int			iaPriestessT13FormulaBasedDamageperHit[10];
	int			iaPriestessT13AttackPowerBasedDamageperHit[10];
	int			iaPriestessT13MinNumberofSparks[10];
	int			iaPriestessT13MaxNumberofSparks[10];
	int			iaPriestessT13MPCost[10];
	int			iaPriestessT13SPCost[10];

	int			iaPriestessT14TargetReduceDamage[10];
	int			iaPriestessT14EffectDuration[10];
	int			iaPriestessT14MPCost[10];
	int			iaPriestessT14SPCost[10];

	int			iaPriestessT21AddMPPotionBoostRecovery[10];
	int			iaPriestessT21AddMPRegeneration[10];

	int			iaPriestessT22AttackRange[10];
	int			iaPriestessT22DamageBoost[10];
	int			iaPriestessT22NumberofBolts[10];
	int			iaPriestessT22MPCost[10];
	int			iaPriestessT22SPCost[10];

	int			iaPriestessT23MonsterDamageReflected[10];
	int			iaPriestessT23Duration[10];
	int			iaPriestessT23MPCost[10];
	int			iaPriestessT23SPCost[10];

	int			iaPriestessT24FormulaBasedLifeRecovery[10];
	int			iaPriestessT24AttackPowerBasedLifeRecovery[10];
	int			iaPriestessT24Range[10];
	int			iaPriestessT24MPCost[10];
	int			iaPriestessT24SPCost[10];

	int			iaPriestessT31AttackRatingBoost[10];
	int			iaPriestessT31DamageBoost[10];
	int			iaPriestessT31MPCost[10];
	int			iaPriestessT31SPCost[10];

	int			iaPriestessT32AttainableSuccessRate[10];
	int			iaPriestessT32MaxPlayersResurrected[10];
	int			iaPriestessT32MPCost[10];
	int			iaPriestessT32SPCost[10];

	int			iaPriestessT33AttainableSuccessRate[10];
	int			iaPriestessT33MonsterHealthReduction[10];
	int			iaPriestessT33MPCost[10];
	int			iaPriestessT33SPCost[10];

	int			iaPriestessT34IncreaseLife[10];
	int			iaPriestessT34Duration[10];
	int			iaPriestessT34MPCost[10];
	int			iaPriestessT34SPCost[10];

	int			iaPriestessT41AttackPowerMin[10];
	int			iaPriestessT41AttackPowerMax[10];
	int			iaPriestessT41AttackRange[10];
	int			iaPriestessT41FreezeDuration[10];
	int			iaPriestessT41MPCost[10];
	int			iaPriestessT41SPCost[10];

	int			iaPriestessT42AddHPRegen[10];
	int			iaPriestessT42AddMPRegen[10];
	int			iaPriestessT42Duration[10];
	int			iaPriestessT42Range[10];
	int			iaPriestessT42MPCost[10];
	int			iaPriestessT42SPCost[10];

	int			iaPriestessT43DamageBoost[10];
	int			iaPriestessT43NumberofBolts[10];
	int			iaPriestessT43Range[10];
	int			iaPriestessT43MPCost[10];
	int			iaPriestessT43SPCost[10];

	int			iaPriestessT44MuspellDamage[10];
	int			iaPriestessT44AddAttackPowerMin[10];
	int			iaPriestessT44AddAttackPowerMax[10];
	int			iaPriestessT44VirtualLifeEffectReduction[10];
	int			iaPriestessT44MuspellAttackDelay[10];
	int			iaPriestessT44AddUndeadAbsorption[10];
	int			iaPriestessT44AddEvasion[10];
	int			iaPriestessT44MPCost[10];
	int			iaPriestessT44SPCost[10];

	int			iaPriestessT51SummonAttackPowerMin[10];
	int			iaPriestessT51SummonAttackPowerMax[10];
	int			iaPriestessT51SummonAttackRating[10];
	int			iaPriestessT51SummonHealthPoints[10];
	int			iaPriestessT51SummonDuration[10];
	int			iaPriestessT51MPCost[10];
	int			iaPriestessT51SPCost[10];

	int			iaPriestessT52DamageBoost[10];
	int			iaPriestessT52AreaofEffect[10];
	int			iaPriestessT52Meteorites[10];
	int			iaPriestessT52MPCost[10];
	int			iaPriestessT52SPCost[10];

	int			iaPriestessT53TargetAttackRatingReduction[10];
	int			iaPriestessT53DebuffEffectDuration[10];
	int			iaPriestessT53ChargedArea[10];
	int			iaPriestessT53MPCost[10];
	int			iaPriestessT53SPCost[10];

	int			iaPriestessT54IncreaseDefense[10];
	int			iaPriestessT54IncreaseAbsorption[10];
	int			iaPriestessT54MPCost[10];
	int			iaPriestessT54SPCost[10];

	//Assassin
	int			iaAssassinT11DamageBoost[10];
	int			iaAssassinT11MPCost[10];
	int			iaAssassinT11SPCost[10];

	int			iaAssassinT12DamageBoost[10];
	int			iaAssassinT12MPCost[10];
	int			iaAssassinT12SPCost[10];

	int			iaAssassinT13WeaponAttackPowerAdd[10];

	int			iaAssassinT14ReduceReceivedDamageFromTarget[10];
	int			iaAssassinT14AttackRange[10];
	int			iaAssassinT14EffectDuration[10];
	int			iaAssassinT14MPCost[10];
	int			iaAssassinT14SPCost[10];

	int			iaAssassinT21DamageBoost[10];
	int			iaAssassinT21MPCost[10];
	int			iaAssassinT21SPCost[10];

	int			iaAssassinT22AddEvasion[10];
	int			iaAssassinT22Duration[10];
	int			iaAssassinT22MPCost[10];
	int			iaAssassinT22SPCost[10];

	int			iaAssassinT23RadiusAreaofEffect[10];
	int			iaAssassinT23StunEffectDuration[10];
	int			iaAssassinT23MPCost[10];
	int			iaAssassinT23SPCost[10];

	int			iaAssassinT24WeaponAttackRatingBoost[10];
	int			iaAssassinT24WeaponBlockRatingBoost[10];

	int			iaAssassinT31DamageBoost[10];
	int			iaAssassinT31MPCost[10];
	int			iaAssassinT31SPCost[10];

	int			iaAssassinT32AreaofEffect[10];
	int			iaAssassinT32DamageBoost[10];
	int			iaAssassinT32MPCost[10];
	int			iaAssassinT32SPCost[10];

	int			iaAssassinT33AttackSpeedBoost[10];
	int			iaAssassinT33Duration[10];
	int			iaAssassinT33MPCost[10];
	int			iaAssassinT33SPCost[10];

	int			iaAssassinT34AfterDamage[10];
	int			iaAssassinT34MonsterSightDecrease[10];
	int			iaAssassinT34Duration[10];
	int			iaAssassinT34MPCost[10];
	int			iaAssassinT34SPCost[10];

	int			iaAssassinT41DamageBoost[10];
	int			iaAssassinT41MPCost[10];
	int			iaAssassinT41SPCost[10];

	int			iaAssassinT42WeaponCriticalRatingBoost[10];

	int			iaAssassinT43DamageBoost[10];
	int			iaAssassinT43MaximumHits[10];
	int			iaAssassinT43EffectDuration[10];
	int			iaAssassinT43EffectRange[10];
	int			iaAssassinT43MPCost[10];
	int			iaAssassinT43SPCost[10];

	int			iaAssassinT44DamageBoost[10];
	int			iaAssassinT44MPCost[10];
	int			iaAssassinT44SPCost[10];

	int			iaAssassinT51CriticalDamageBoost[10];
	int			iaAssassinT51AttainableMovementTeleportRange[10];
	int			iaAssassinT51MPCost[10];
	int			iaAssassinT51SPCost[10];

	int			iaAssassinT52DamageBoost[10];
	int			iaAssassinT52PoisonDamage[10];
	int			iaAssassinT52PoisonDuration[10];
	int			iaAssassinT52MPCost[10];
	int			iaAssassinT52SPCost[10];

	int			iaAssassinT53DamageBoost[10];
	int			iaAssassinT53MPCost[10];
	int			iaAssassinT53SPCost[10];

	int			iaAssassinT54DamageBoost[10];
	int			iaAssassinT54MPCost[10];
	int			iaAssassinT54SPCost[10];

	//Shaman
	int			iaShamanT11DamageBoost[10];
	int			iaShamanT11AttackRange[10];
	int			iaShamanT11MPCost[10];
	int			iaShamanT11SPCost[10];

	int			iaShamanT12DamageBoost[10];
	int			iaShamanT12AttackRange[10];
	int			iaShamanT12AttackArea[10];
	int			iaShamanT12MPCost[10];
	int			iaShamanT12SPCost[10];

	int			iaShamanT13EnemySpeedReduction[10];
	int			iaShamanT13EffectArea[10];
	int			iaShamanT13EffectDuration[10];
	int			iaShamanT13MPCost[10];
	int			iaShamanT13SPCost[10];

	int			iaShamanT14MPPotionRecoveryBoost[10];
	int			iaShamanT14MPCapacityBoost[10];

	int			iaShamanT21DamageBoost[10];
	int			iaShamanT21AttackRange[10];
	int			iaShamanT21AttackArea[10];
	int			iaShamanT21MPCost[10];
	int			iaShamanT21SPCost[10];

	int			iaShamanT22StunDuration[10];
	int			iaShamanT22MPCost[10];
	int			iaShamanT22SPCost[10];

	int			iaShamanT23BaseAddAttackPowerMin[10];
	int			iaShamanT23BaseAddAttackPowerMax[10];
	int			iaShamanT23IncreaseMonsterSightRange[10];
	int			iaShamanT23Duration[10];
	int			iaShamanT23MPCost[10];
	int			iaShamanT23SPCost[10];

	int			iaShamanT24AddAttackPowerMin[10];
	int			iaShamanT24AddAttackPowerMax[10];
	int			iaShamanT24Duration[10];
	int			iaShamanT24MPCost[10];
	int			iaShamanT24SPCost[10];

	int			iaShamanT31ReduceReceivedDamage[10];
	int			iaShamanT31AddEvasionRating[10];
	int			iaShamanT31ReduceMPPoints[10];
	int			iaShamanT31Duration[10];
	int			iaShamanT31MPCost[10];
	int			iaShamanT31SPCost[10];

	int			iaShamanT32DamageBoost[10];
	int			iaShamanT32AreaofEffect[10];
	int			iaShamanT32MPCost[10];
	int			iaShamanT32SPCost[10];

	int			iaShamanT33MonsterLifeAbsorption[10];
	int			iaShamanT33MPCost[10];
	int			iaShamanT33SPCost[10];

	int			iaShamanT34DamageBoost[10];
	int			iaShamanT34MPCost[10];
	int			iaShamanT34SPCost[10];

	int			iaShamanT41SummonDuration[10];
	int			iaShamanT41SummonAttackPowerMin[10];
	int			iaShamanT41SummonAttackPowerMax[10];
	int			iaShamanT41SummonAttackRating[10];
	int			iaShamanT41SummonHP[10];
	int			iaShamanT41MPCost[10];
	int			iaShamanT41SPCost[10];

	int			iaShamanT42DamageBoost[10];
	int			iaShamanT42AddCriticalChance[10];
	int			iaShamanT42AttackRange[10];
	int			iaShamanT42MPCost[10];
	int			iaShamanT42SPCost[10];

	int			iaShamanT43AttackSpeedBoost[10];
	int			iaShamanT43Duration[10];
	int			iaShamanT43MPCost[10];
	int			iaShamanT43SPCost[10];

	int			iaShamanT44DamageBoost[10];
	int			iaShamanT44AreaofEffect[10];
	int			iaShamanT44MPCost[10];
	int			iaShamanT44SPCost[10];

	int			iaShamanT51MPBoost[10];
	int			iaShamanT51SPBoost[10];
	int			iaShamanT51Duration[10];
	int			iaShamanT51MPCost[10];
	int			iaShamanT51SPCost[10];

	int			iaShamanT52DamageBoostMainTarget[10];
	int			iaShamanT52DamageBoostSurroundings[10];
	int			iaShamanT52SurroundingImpactArea[10];
	int			iaShamanT52MPCost[10];
	int			iaShamanT52SPCost[10];

	int			iaShamanT53DamageBoost[10];
	int			iaShamanT53AttackRange[10];
	int			iaShamanT53MPCost[10];
	int			iaShamanT53SPCost[10];

	int			iaShamanT54HPIncrease[10];
	int			iaShamanT54MPDecrease[10];
};

extern SkillTable sSkillTable;

	//Fighter
	const int iaFighterT11DMGBoost[10] = {13, 16, 19, 22, 25, 28, 31, 34, 37, 40};
	const int iaFighterT33AttackSpeed[10] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
	const int iaFighterT33Duration[10] = {300, 300, 300, 300, 300, 300, 300, 300, 300, 300};
	static const int * iaFighterT24DMGBoost = ((int *)0x00995500);
	const int iaFighterT24Area[10] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
	const float faFighterT51AddHPRegen[10] = {2.0f, 4.0f, 6.0f, 8.0f, 10.0f, 12.0f, 14.0f, 16.0f, 18.0f, 20.0f};
	const int iaFighterT51AddHPPotionBoost[10] = {4, 8, 12, 16, 20, 24, 28, 32, 36, 40};
	const int iaFighterT51BoostHealthBoost[10] = {13, 26, 39, 52, 65, 78, 91, 104, 117, 130};
	const int iaFighterT52DMGBoost[10] = {45, 50, 55, 60, 65, 70, 75, 80, 85, 90};
	const int iaFighterT52Area[10] = {50, 55, 60, 65, 70, 80, 85, 90, 95, 100};
	const float faFighterT53HPDec[10] = {5.6f, 5.2f, 4.8f, 4.4f, 4.0f, 3.6f, 3.2f, 2.8f, 2.4f, 2.0f};
	const int iaFighterT53HitsBoost[10] = {6, 6, 6, 5, 5, 5, 4, 4, 4, 3};
	const int iaFighterT53DMGBoost[10] = {12, 14, 16, 18, 20, 22, 24, 26, 28, 30};
	const int iaFighterT54DMGBoost[10] = {17, 24, 31, 38, 45, 52, 59, 66, 73, 80};
	const int iaFighterT54Duration[10] = {12, 14, 16, 18, 20, 22, 24, 26, 28, 30};
	const int iaFighterT54MPDrain[10] = {17, 19, 21, 23, 25, 27, 29, 31, 33, 35};
	const int iaFighterT54SPDrain[10] = {29, 31, 35, 37, 39, 41, 43, 45, 47, 50};

	//Mechanician
	static const int * iaMechT21Damage = ((int *)0x009928A0);
	const int iaMechT21Area[10] = {70, 70, 70, 70,70, 70, 70, 70};
	static const int * iaMechT23DamageBoost = ((int *)0x00992968);
	static const int * iaMechT23AtkSpeed = ((int *)0x00992990);
	static const int * iaMechT23Duration = ((int *)0x009929B8);
	const float faMechT23MPDrain[10] = {7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f};
	const float faMechT23SPDrain[10] = {5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f};
	static const int * iaMechT31DefenseRtg = ((int *)0x00992AA8);
	static const int * iaMechT31Duration = ((int *)0x00992AD0);
	const int iaMechT51DMGBoost[10] = {80, 90, 100, 110, 120, 130, 140, 150, 160, 170};
	const int iaMechT52DMGBoost[10] = {12, 14, 16, 18, 20, 22, 24, 26, 28, 30};
	const int iaMechT53DMGReduction[10] = {7, 8, 9, 10, 12, 14, 16, 18, 20};
	const int iaMechT53WPAtkRateRed[10] = {20, 22, 24, 26, 27, 28, 29, 30, 31};
	const int iaMechT54DMGBoost[10] = {20, 30, 40, 50, 60, 70, 80, 90, 100, 120};

	//Archer
	const int iaArcherT51RangeAdd[10] = {10, 15, 20, 25, 30, 35, 40, 45, 50, 55};
	const int iaArcherT51CritRate[10] = {3, 6, 9, 12, 15, 18, 21, 24, 27, 30};
	const int iaArcherT51DMGBoost[10] = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50};
	const int iaArcherT52DMGBoost[10] = {60, 70, 80, 90, 100, 110, 120, 130, 140};
	const int iaArcherT52CDMGBoost[10] = {2, 3, 4, 5, 6, 8, 10, 12, 15};
	const int iaArcherT53Duration[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
	const int iaArcherT54DMGBoost[10] = {86, 92, 98, 104, 110, 116, 122, 128, 134, 140};
	const int iaArcherT54Duration[10] = {12, 14, 16, 18, 20, 22, 24, 26, 28, 30};
	const int iaArcherT54MPDrain[10] = {17, 19, 21, 23, 25, 27, 29, 31, 33, 35};
	const int iaArcherT54SPDrain[10] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40};

	//Pikeman
	const int iaPikeT51Duration[10] = {8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
	const int iaPikeT52DamageBoost[10] = {60, 70, 80, 90, 100, 110, 120, 130, 140, 150};
	const int iaPikeT52MinATKRed[10] = {20, 40, 60, 80, 100, 120, 140, 160, 180, 200};
	const int iaPikeT52MaxATKInc[10] = {30, 60, 90, 120, 150, 180, 210, 240, 270, 300};
	const int iaPikeT53DMGBoost[10] = {4, 8, 12, 16, 20, 24, 28, 32, 36, 40};
	const int iaPikeT53ATKRate[10] = {24, 28, 32, 36, 40, 44, 48, 52, 56, 60};
	const int iaPikeT54DMGBoost[10] = {18, 26, 34, 42, 50, 58, 66, 74, 82, 90};

	//Atalanta
	const int iaAtaT51DMGBoost[10] = {39, 41, 44, 47, 50, 53, 56, 59, 62, 75};
	const int iaAtaT51Targets[10] = {3, 3, 4, 4, 5, 6, 7, 8, 9, 10};
	const int iaAtaT51PoisonDuration[10] = {2, 2, 3, 3, 4, 4, 5, 5, 6, 7};
	const int iaAtaT52Speed[10] = {8, 10, 12, 14, 16, 18, 20, 22, 24, 26};
	const int iaAtaT52Evade[10] = {1, 1, 1, 2, 2, 2, 3, 3, 3, 4};

	//Knight
	static const int * iaKnightT43AddAtkPow = (int *)0x009938B8;
	static const int * iaKnightT43Duration = (int *)0x009938E0;
	const int iaKnightT51DMGBoost[10] = {14, 16, 18, 20, 22, 24, 26, 28, 30, 32};
	const int iaKnightT52DMGBoost[10] = {35, 45, 55, 65, 75, 85, 95, 105, 115, 125};
	const int iaKnightT52Area[10] = {80, 85, 90, 95, 100, 105, 110, 115, 120, 125};
	const int iaKnightT52AtkRtg[10] = {20, 30, 40, 50, 60, 70, 80, 90, 100, 110};
	const int iaKnightT52Duration[10] = {2, 2, 3, 3, 4, 4, 5, 5, 6, 6};
	const int iaKnightT53DefBoost[10] = {40, 46, 52, 58, 64, 70, 76, 82, 96, 100};
	const int iaKnightT54DMGBoost[10] = {4,8,12,16,20,24,28,32,36,40};

	static const TwoArraySkill * iaMageT23FireDamage = (TwoArraySkill *)0x00995F50;
	static const TwoArraySkill * iaMageT23IceDamage = (TwoArraySkill *)0x00995F00;
	static const TwoArraySkill * iaMageT23LightDamage = (TwoArraySkill *)0x00995FA0;
	static const int * iaMageT23Duration = (int *)0x00995FF0;

	const int iaMageT21MPRecoveryBoost[10] = {60, 70, 80, 90, 100, 110, 120, 130, 140, 150};
	const int iaMageT21MPCapacityBoost[10] = {4, 8, 12, 16, 20, 24, 28, 32, 36, 40};
	const int iaMageT24DMGBoost[10] = {110, 115, 120, 125, 130, 135, 140, 145, 150, 155};
	const int iaMageT24AttackRatingBoost[10] = {6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	const int iaMageT24AttackRange[10] = {185, 190, 195, 200, 205, 210, 215, 220, 225, 230};
	const int iaMageT51BlockBoost[10] = {4, 8, 12, 16, 20, 24, 28, 32, 36, 40};
	const int iaMageT51ATKRateRed[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
	const int iaMageT52DMGBoost[10] = {60, 70, 80, 90, 100, 110, 120, 130, 140, 150};
	const int iaMageT52ATKRate[10] = {3, 6, 9, 12, 15, 18, 21, 24, 27, 30};
	const int iaMageT52Duration[10] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
	const int iaMageT52RunSpeedRed[10] = {80, 76, 72, 68, 64, 56, 52, 48, 44};
	const int iaMageT53DMGBoost[10] = {26, 32, 38, 44, 50, 56, 62, 68, 74, 80};
	const int iaMageT53AddRange[10] = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50};
	const int iaMageT54DMGBoost[10] = {65, 80, 95, 110, 125, 140, 155, 170, 185, 200};
	const int iaMageT54Duration[10] = {12, 14, 16, 18, 20, 22, 24, 26, 28, 30};
	const int iaMageT54MPDrain[10] = {75, 80, 85, 90, 95, 100, 105, 110, 115, 120};
	const int iaMageT54SPDrain[10] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40};


	//Priestess
	static const int * iaPrsT34Duration = (int *)0x00996E50;
	const int iaPrsT52DMGBoost[10] = {58, 66, 74, 82, 90, 98, 106, 114, 122, 130};
	const int iaPrsT52Area[10] = {141, 142, 143, 144, 145, 146, 147, 148, 149, 150};
	const int iaPrsT52AreaFake[10] = {130, 140, 150, 160, 170, 180, 190, 200, 210, 220};
	const int iaPrsT52MeteoriteCount[10] = {1, 1, 1, 1, 2, 3, 4, 5, 6, 7};
	const int iaPrsT53ATKRate[10] = {6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	const int iaPrsT53Duration[10] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
	const int iaPrsT53Area[10] = {80, 85, 90, 95, 100, 105, 110, 115, 120, 125};
	const int iaPrsT54Defense[10] = {70, 80, 90, 100, 110, 120, 130, 140, 150, 160};
	const int iaPrsT54Absorption[10] = {6, 8, 10, 12, 14, 16, 18, 20, 22, 24};

	//Assassin
	const int iaASNT33AttackSpeed[10] = {7, 9, 11, 13, 15, 17, 19, 21, 23, 25};
	const int iaASNT33Duration[10] = {300, 300, 300, 300, 300, 300, 300, 300, 300, 300};
	const int iaASNT51CritBoost[10] = {4, 8, 12, 16, 20, 24, 28, 32, 36, 40};
	const int iaASNT51Range[10] = {60, 80, 100, 120, 140, 160, 180, 200, 220, 240};
	const int iaASNT52CriticalAdd[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	const int iaASNT52Duration[10] = {300, 300, 300, 300, 300, 300, 300, 300, 300, 300};

	//Shaman
	const int iaShaT43AttackSpeed[10] = {7, 9, 11, 13, 15, 17, 19, 21, 23, 25};
	const int iaShaT43Duration[10] = {300, 300, 300, 300, 300, 300, 300, 300, 300, 300};
	const int iaShaT51MPBoost[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
	const int iaShaT51STMBoost[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
	const int iaShaT52DMGBoost[10] = {58, 66, 74, 82, 90, 98, 106, 114, 122, 130};
	const int iaShaT52Area[10] = {141, 142, 143, 144, 145, 146, 147, 148, 149, 150};
	const int iaShaT52AreaFake[10] = {130, 140, 150, 160, 170, 180, 190, 200, 210, 220};
	const int iaShaT52MeteoriteCount[10] = {1, 1, 1, 1, 2, 3, 4, 5, 6, 7};


enum ESkillType
{
	SKILLTYPE_Undefined			= 0,

	SKILLTYPE_Right				= 0x01000000,
	SKILLTYPE_Left				= 0x02000000,
	SKILLTYPE_All				= 0x03000000,
	SKILLTYPE_Passive			= 0x04000000,
};

enum ESkillPlayID
{
	SKILLPLAYID_Undefined		= 0x00,

	//Tier1
	SKILLPLAYID_T11				= 0x1A,
	SKILLPLAYID_T12				= 0x1B,
	SKILLPLAYID_T13				= 0x1C,
	SKILLPLAYID_T14				= 0x1D,

	//Tier2
	SKILLPLAYID_T21				= 0x2A,
	SKILLPLAYID_T22				= 0x2B,
	SKILLPLAYID_T23				= 0x2C,
	SKILLPLAYID_T24				= 0x2D,

	//Tier3
	SKILLPLAYID_T31				= 0x3A,
	SKILLPLAYID_T32				= 0x3B,
	SKILLPLAYID_T33				= 0x3C,
	SKILLPLAYID_T34				= 0x3D,

	//Tier4
	SKILLPLAYID_T41				= 0x4A,
	SKILLPLAYID_T42				= 0x4B,
	SKILLPLAYID_T43				= 0x4C,
	SKILLPLAYID_T44				= 0x4D,

	//Tier5
	SKILLPLAYID_T51				= 0x5A,		//90
	SKILLPLAYID_T52				= 0x5B,		//91
	SKILLPLAYID_T53				= 0x5C,		//92
	SKILLPLAYID_T54				= 0x5D,		//93
};


enum ESkillSound : int
{
	SKILLSOUND_PhysicalAbsorb3		= 0x00001032,
	SKILLSOUND_Automation3			= 0x00002012,
	SKILLSOUND_AvengingCrash		= 0x00003120,

	
	SKILLSOUNDID_LEARN				 = 0x1000,
	SKILLSOUNDID_EXTREME_SHIELD				 = 0x1010,
	SKILLSOUNDID_MECHANIC_BOMB1				 = 0x1020,
	SKILLSOUNDID_MECHANIC_BOMB2				 = 0x1021,
	SKILLSOUNDID_PHYSICAL_ABSORB1				 = 0x1030,
	SKILLSOUNDID_PHYSICAL_ABSORB2				 = 0x1031,
	SKILLSOUNDID_PHYSICAL_ABSORB3				 = 0x1032,
	SKILLSOUNDID_JUMPING_CRASH1				 = 0x1130,
	SKILLSOUNDID_JUMPING_CRASH2				 = 0x1131,
	SKILLSOUNDID_PIKE_WIND				 = 0x1200,
	SKILLSOUNDID_SCOUT_HAWK1				 = 0x1310,
	SKILLSOUNDID_SCOUT_HAWK2				 = 0x1311,
	SKILLSOUNDID_WIND_ARROW				 = 0x1320,
	SKILLSOUNDID_PERFECT_AIM				 = 0x1330,
	SKILLSOUNDID_ARROW_HIT1				 = 0x1301,
	SKILLSOUNDID_ARROW_HIT2				 = 0x1302,
	SKILLSOUNDID_SKILL_SWING1				 = 0x2000,
	SKILLSOUNDID_SKILL_SWING2				 = 0x2001,
	SKILLSOUNDID_SKILL_AUTOMATION1				 = 0x2010,
	SKILLSOUNDID_SKILL_AUTOMATION2				 = 0x2011,
	SKILLSOUNDID_SKILL_AUTOMATION3				 = 0x2012,
	SKILLSOUNDID_SKILL_AUTOMATION4				 = 0x2013,
	SKILLSOUNDID_SKILL_AUTOMATION5				 = 0x2014,
	SKILLSOUNDID_SKILL_MAXIMIZE1				 = 0x2020,
	SKILLSOUNDID_SKILL_MAXIMIZE2				 = 0x2021,
	SKILLSOUNDID_SKILL_MAXIMIZE3				 = 0x2022,
	SKILLSOUNDID_SKILL_SPARK1				 = 0x2030,
	SKILLSOUNDID_SKILL_SPARK2				 = 0x2031,
	SKILLSOUNDID_SKILL_SPARK3				 = 0x2032,
	SKILLSOUNDID_SKILL_GROUND_PIKE				 = 0x2110,
	SKILLSOUNDID_SKILL_TORNADO1				 = 0x2120,
	SKILLSOUNDID_SKILL_EXPANSION1				 = 0x2130,
	SKILLSOUNDID_SKILL_EXPANSION2				 = 0x2131,
	SKILLSOUNDID_SKILL_ROAR1				 = 0x2201,
	SKILLSOUNDID_SKILL_ROAR2				 = 0x2202,
	SKILLSOUNDID_SKILL_RAGE_OF_ZECRAM1				 = 0x2211,
	SKILLSOUNDID_SKILL_RAGE_OF_ZECRAM2				 = 0x2212,
	SKILLSOUNDID_SKILL_RAGE_OF_ZECRAM3				 = 0x2213,
	SKILLSOUNDID_SKILL_FALCON				 = 0x2310,
	SKILLSOUNDID_SKILL_FALCON_ATTACK1				 = 0x2311,
	SKILLSOUNDID_SKILL_FALCON_ATTACK2				 = 0x2312,
	SKILLSOUNDID_SKILL_AVALANCHE1				 = 0x2322,
	SKILLSOUNDID_SKILL_AVALANCHE2				 = 0x2322,
	SKILLSOUNDID_SKILL_AVALANCHE3				 = 0x2322,
	SKILLSOUNDID_SKILL_ARROW_OF_RANGE				 = 0x2332,
	SKILLSOUNDID_SKILL_CASTING_MAGICIAN				 = 0x1800,
	SKILLSOUNDID_SKILL_CASTING_PRIEST				 = 0x1700,
	SKILLSOUNDID_SKILL_SWORDBLAST1				 = 0x1510,
	SKILLSOUNDID_SKILL_SWORDBLAST2				 = 0x1511,
	SKILLSOUNDID_SKILL_HOLYBODY				 = 0x1520,
	SKILLSOUNDID_SKILL_SHIELDSTRIKE				 = 0x1610,
	SKILLSOUNDID_SKILL_FARINA				 = 0x1620,
	SKILLSOUNDID_SKILL_VIGORSPEAR				 = 0x1640,
	SKILLSOUNDID_SKILL_HEALING				 = 0x1710,
	SKILLSOUNDID_SKILL_HOLYBOLT				 = 0x1720,
	SKILLSOUNDID_SKILL_MULTISPARK				 = 0x1730,
	SKILLSOUNDID_SKILL_HOLYMIND				 = 0x1740,
	SKILLSOUNDID_SKILL_AGONY				 = 0x1810,
	SKILLSOUNDID_SKILL_FIREBOLT1				 = 0x1820,
	SKILLSOUNDID_SKILL_FIREBOLT2				 = 0x1821,
	SKILLSOUNDID_SKILL_ZENITH				 = 0x1830,
	SKILLSOUNDID_SKILL_FIREBALL1				 = 0x1840,
	SKILLSOUNDID_SKILL_FIREBALL2				 = 0x1841,
	SKILLSOUNDID_SKILL_HOLY_VALOR1				 = 0x2510,
	SKILLSOUNDID_SKILL_HOLY_VALOR3				 = 0x2513,
	SKILLSOUNDID_SKILL_BLANDISH1				 = 0x2520,
	SKILLSOUNDID_SKILL_BLANDISH2				 = 0x2521,
	SKILLSOUNDID_SKILL_PIERCING				 = 0x2530,
	SKILLSOUNDID_SKILL_DRASTICSPIRIT1				 = 0x2540,
	SKILLSOUNDID_SKILL_DRASTICSPIRIT3				 = 0x2543,
	SKILLSOUNDID_SKILL_WINDY1				 = 0x2610,
	SKILLSOUNDID_SKILL_WINDY3				 = 0x2613,
	SKILLSOUNDID_SKILL_TWISTJAVELIN				 = 0x2620,
	SKILLSOUNDID_SKILL_SOULSUCKER2				 = 0x2622,
	SKILLSOUNDID_SKILL_SOULSUCKER3				 = 0x2623,
	SKILLSOUNDID_SKILL_SOULSUCKER4				 = 0x2624,
	SKILLSOUNDID_SKILL_FIREJAVELIN				 = 0x2630,
	SKILLSOUNDID_SKILL_DIVINELIGHTNING1				 = 0x2710,
	SKILLSOUNDID_SKILL_DIVINELIGHTNING2				 = 0x2711,
	SKILLSOUNDID_SKILL_DIVINELIGHTNING3				 = 0x2712,
	SKILLSOUNDID_SKILL_HOLYREFLECTION1				 = 0x2720,
	SKILLSOUNDID_SKILL_HOLYREFLECTION2				 = 0x2722,
	SKILLSOUNDID_SKILL_GRANDHEALING				 = 0x2730,
	SKILLSOUNDID_SKILL_WATORNADO1				 = 0x2810,
	SKILLSOUNDID_SKILL_WATORNADO2				 = 0x2811,
	SKILLSOUNDID_SKILL_ENCHANTWEAPON_CAST				 = 0x2820,
	SKILLSOUNDID_SKILL_ENCHANTWEAPON_ICE1				 = 0x2821,
	SKILLSOUNDID_SKILL_ENCHANTWEAPON_ICE2				 = 0x2822,
	SKILLSOUNDID_SKILL_ENCHANTWEAPON_FIRE1				 = 0x2823,
	SKILLSOUNDID_SKILL_ENCHANTWEAPON_FIRE2				 = 0x2824,
	SKILLSOUNDID_SKILL_ENCHANTWEAPON_LIGHTNING1				 = 0x2825,
	SKILLSOUNDID_SKILL_ENCHANTWEAPON_LIGHTNING2				 = 0x2826,
	SKILLSOUNDID_SKILL_ENCHANTWEAPON_MAGIC_LIGHTNING1				 = 0x2827,
	SKILLSOUNDID_SKILL_ENCHANTWEAPON_MAGIC_LIGHTNING2				 = 0x2828,
	SKILLSOUNDID_SKILL_ENCHANTWEAPON_MAGIC_ICE1				 = 0x2840,
	SKILLSOUNDID_SKILL_ENCHANTWEAPON_MAGIC_ICE2				 = 0x2841,
	SKILLSOUNDID_SKILL_DEATHRAY				 = 0x2830,
	SKILLSOUNDID_SKILL_METAL_ARMOR				 = 0x3010,
	SKILLSOUNDID_SKILL_SPARK_SHIELD				 = 0x3040,
	SKILLSOUNDID_SKILL_CONCENTRATION1				 = 0x3110,
	SKILLSOUNDID_SKILL_CONCENTRATION2				 = 0x3111,
	SKILLSOUNDID_SKILL_AVANGING_CRASH				 = 0x3120,
	SKILLSOUNDID_SKILL_SWIFT_AXE1				 = 0x3130,
	SKILLSOUNDID_SKILL_SWIFT_AXE2				 = 0x3131,
	SKILLSOUNDID_SKILL_BONE_CRASH1				 = 0x3140,
	SKILLSOUNDID_SKILL_VENOM_SPEAR1				 = 0x3210,
	SKILLSOUNDID_SKILL_VENOM_SPEAR2				 = 0x3211,
	SKILLSOUNDID_SKILL_VANISH1				 = 0x3220,
	SKILLSOUNDID_SKILL_VANISH2				 = 0x3221,
	SKILLSOUNDID_SKILL_CHAIN_LANCE1				 = 0x3240,
	SKILLSOUNDID_SKILL_CHAIN_LANCE2				 = 0x3241,
	SKILLSOUNDID_SKILL_CHAIN_LANCE3				 = 0x3242,
	SKILLSOUNDID_SKILL_ELEMENTAL_SHOT				 = 0x3310,
	SKILLSOUNDID_SKILL_GOLDEN_FALCON				 = 0x3320,
	SKILLSOUNDID_SKILL_PERFORATION				 = 0x3330,
	SKILLSOUNDID_SKILL_BOMB_SHOT1				 = 0x3340,
	SKILLSOUNDID_SKILL_BOMB_SHOT2				 = 0x3341,
	SKILLSOUNDID_SKILL_FLAME_BRANDISH				 = 0x3410,
	SKILLSOUNDID_SKILL_DIVINE_INHALATION				 = 0x3420,
	SKILLSOUNDID_SKILL_HOLY_INCANTATION				 = 0x3430,
	SKILLSOUNDID_SKILL_GRAND_CROSS				 = 0x3440,
	SKILLSOUNDID_SKILL_SPLIT_JAVELIN1				 = 0x3510,
	SKILLSOUNDID_SKILL_SPLIT_JAVELIN2				 = 0x3511,
	SKILLSOUNDID_SKILL_TRIUMPH_OF_VALHALLA1				 = 0x3520,
	SKILLSOUNDID_SKILL_TRIUMPH_OF_VALHALLA2				 = 0x3521,
	SKILLSOUNDID_SKILL_LIGHTNING_JAVELIN1				 = 0x3530,
	SKILLSOUNDID_SKILL_LIGHTNING_JAVELIN2				 = 0x3531,
	SKILLSOUNDID_SKILL_STORM_JAVELIN1				 = 0x3540,
	SKILLSOUNDID_SKILL_STORM_JAVELIN2				 = 0x3541,
	SKILLSOUNDID_SKILL_VIGOR_BALL1				 = 0x3610,
	SKILLSOUNDID_SKILL_VIGOR_BALL2				 = 0x3611,
	SKILLSOUNDID_SKILL_RESURRECTION				 = 0x3620,
	SKILLSOUNDID_SKILL_EXTINTION				 = 0x3630,
	SKILLSOUNDID_SKILL_VIRTUAL_LIFE				 = 0x3640,
	SKILLSOUNDID_SKILL_ENERGY_SHIELD				 = 0x3710,
	SKILLSOUNDID_SKILL_DIASTROPHISM				 = 0x3720,
	SKILLSOUNDID_SKILL_SPIRIT_ELEMENTAL				 = 0x3730,
	SKILLSOUNDID_SKILL_DANCING_SWORD				 = 0x3740,
	SKILLSOUNDID_SKILL_DANCING_SWORD_ATK1				 = 0x3741,
	SKILLSOUNDID_SKILL_DANCING_SWORD_ATK2				 = 0x3742,
	SKILLSOUNDID_SKILL_IMPULSION				 = 0x4010,
	SKILLSOUNDID_SKILL_COMPULSION1				 = 0x4021,
	SKILLSOUNDID_SKILL_COMPULSION2				 = 0x4022,
	SKILLSOUNDID_SKILL_COMPULSION3				 = 0x4023,
	SKILLSOUNDID_SKILL_METAL_GOLEM				 = 0x4040,
	SKILLSOUNDID_SKILL_BERSERKER				 = 0x4120,
	SKILLSOUNDID_SKILL_CYCLONE_STRIKE				 = 0x4130,
	SKILLSOUNDID_SKILL_ASSASSIN_EYE				 = 0x4210,
	SKILLSOUNDID_SKILL_CHARGING				 = 0x4220,
	SKILLSOUNDID_SKILL_CHARGING_STRIKE				 = 0x4221,
	SKILLSOUNDID_SKILL_VAGUE				 = 0x4230,
	SKILLSOUNDID_SKILL_SHADOW_STRIKE1				 = 0x4241,
	SKILLSOUNDID_SKILL_SHADOW_STRIKE2				 = 0x4242,
	SKILLSOUNDID_SKILL_SHADOW_STRIKE3				 = 0x4243,
	SKILLSOUNDID_SKILL_SHADOW_STRIKE4				 = 0x4244,
	SKILLSOUNDID_SKILL_RECALL_WOLVERIN				 = 0x4310,
	SKILLSOUNDID_SKILL_PHOENIX_SHOT				 = 0x4330,
	SKILLSOUNDID_SKILL_FORCE_OF_NATURE				 = 0x4340,
	SKILLSOUNDID_SKILL_DVINE_PIERCING				 = 0x4410,
	SKILLSOUNDID_SKILL_GODLY_SHIELD				 = 0x4420,
	SKILLSOUNDID_SKILL_GODS_BLESS				 = 0x4430,
	SKILLSOUNDID_SKILL_SWORD_OF_JUSTICE				 = 0x4440,
	SKILLSOUNDID_SKILL_HALL_OF_VALHALLA				 = 0x4510,
	SKILLSOUNDID_SKILL_EXTREM_RAGE1				 = 0x4521,
	SKILLSOUNDID_SKILL_EXTREM_RAGE2				 = 0x4522,
	SKILLSOUNDID_SKILL_EXTREM_RAGE3				 = 0x4523,
	SKILLSOUNDID_SKILL_FROST_JAVELIN				 = 0x4530,
	SKILLSOUNDID_SKILL_VENGEANCE1				 = 0x4541,
	SKILLSOUNDID_SKILL_VENGEANCE2				 = 0x4542,
	SKILLSOUNDID_SKILL_GLACIAL_SPIKE1				 = 0x4611,
	SKILLSOUNDID_SKILL_GLACIAL_SPIKE2				 = 0x4612,
	SKILLSOUNDID_SKILL_REGEN_FIELD1				 = 0x4621,
	SKILLSOUNDID_SKILL_REGEN_FIELD2				 = 0x4622,
	SKILLSOUNDID_SKILL_CHAIN_LIGHTNING1				 = 0x4631,
	SKILLSOUNDID_SKILL_CHAIN_LIGHTNING2				 = 0x4632,
	SKILLSOUNDID_SKILL_MUSPELL1				 = 0x4641,
	SKILLSOUNDID_SKILL_MUSPELL2				 = 0x4642,
	SKILLSOUNDID_SKILL_FIRE_ELEMENTAL				 = 0x4710,
	SKILLSOUNDID_SKILL_FLAME_WAVE1				 = 0x4721,
	SKILLSOUNDID_SKILL_FLAME_WAVE2				 = 0x4722,
	SKILLSOUNDID_SKILL_FLAME_WAVE3				 = 0x4723,
	SKILLSOUNDID_SKILL_DISTORTION				 = 0x4730,
	SKILLSOUNDID_SKILL_METEO1				 = 0x4741,
	SKILLSOUNDID_SKILL_METEO2				 = 0x4742,
	SKILLSOUNDID_SKILL_METEO3				 = 0x4743
};


enum ESkillMask
{
	SKILLMASK_ExtremeShield			= 0x00000001,
	SKILLMASK_PhysicalAbsorb		= 0x00000002,
	SKILLMASK_Automation			= 0x00000004,
	SKILLMASK_CriticalHit			= 0x00000008,
	SKILLMASK_SparkShield			= 0x00000010,
	SKILLMASK_GodlyShield			= 0x00000020,

	SKILLMASK_HolyBody				= 0x00010000,
};

enum ESkillEffectID
{
	//Knight
	SKILLEFFECTID_HolyValorAction			= 700,

	//Magician
	SKILLEFFECTID_EnchantWeaponFire			= 100,
	SKILLEFFECTID_EnchantWeaponIce			= 200,
	SKILLEFFECTID_EnchantWeaponLightning	= 300,
};

enum ESkillID
{
	SKILLID_None					= 0x00000000,
	SKILLID_DefaultAttack			= 0x11111111,

	//Fighter
	SKILLID_MeleeMastery			= 0x02010001,
	SKILLID_FireAttribute			= 0x02010002,
	SKILLID_Raving					= 0x02010003,
	SKILLID_Impact					= 0x02010004,

	SKILLID_TripleImpact			= 0x02020001,
	SKILLID_BrutalSwing				= 0x02020002,
	SKILLID_Roar					= 0x02020003,
	SKILLID_RageofZecram			= 0x02020004,

	SKILLID_Concentration			= 0x02040001,
	SKILLID_AvengingCrash			= 0x02040002,
	SKILLID_SwiftAxe				= 0x02040003,
	SKILLID_BoneCrash				= 0x02040004,

	SKILLID_Destroyer				= 0x02080001,
	SKILLID_Berserker				= 0x02080002,
	SKILLID_CycloneStrike			= 0x02080003,
	SKILLID_BoostHealth				= 0x02080004,

	SKILLID_SurvivalInstinct		= 0x02100001,
	SKILLID_SeismicImpact			= 0x02100002,
	SKILLID_RuthlessofZecram		= 0x02100003,
	SKILLID_WhirlwindFS01			= 0x02100004,

	//Mechanician
	SKILLID_ExtremeShield			= 0x01010001,
	SKILLID_MechanicBomb			= 0x01010002,
	SKILLID_PhysicalAbsorb			= 0x01010003,
	SKILLID_PoisonAttribute			= 0x01010004,

	SKILLID_GreatSmash				= 0x01020001,
	SKILLID_Maximize				= 0x01020002,
	SKILLID_Automation				= 0x01020003,
	SKILLID_Spark					= 0x01020004,
	
	SKILLID_MetalArmor				= 0x01040001,
	SKILLID_GrandSmash				= 0x01040002,
	SKILLID_MechanicWeaponMastery	= 0x01040003,
	SKILLID_SparkShield				= 0x01040004,
	
	SKILLID_Impulsion				= 0x01080001,
	SKILLID_Compulsion				= 0x01080002,
	SKILLID_MagneticSphere			= 0x01080003,
	SKILLID_MetalGolem				= 0x01080004,

	SKILLID_Rupture					= 0x01100001,
	SKILLID_MagneticDischarge		= 0x01100002,
	SKILLID_HardeningShield			= 0x01100003,
	SKILLID_ParasitShot				= 0x01100004,

	//Archer
	SKILLID_ScoutHawk				= 0x04010001,
	SKILLID_ShootingMastery			= 0x04010002,
	SKILLID_WindArrow				= 0x04010003,
	SKILLID_PerfectAim				= 0x04010004,
										 
	SKILLID_DionsEye				= 0x04020001,
	SKILLID_Falcon					= 0x04020002,
	SKILLID_ArrowofRage				= 0x04020003,
	SKILLID_Avalanche				= 0x04020004,
										 
	SKILLID_ElementalShot			= 0x04040001,
	SKILLID_GoldenFalcon			= 0x04040002,
	SKILLID_BombShot				= 0x04040003,
	SKILLID_Perforation				= 0x04040004,
										 
	SKILLID_RecallWolverine			= 0x04080001,
	SKILLID_EvasionMastery			= 0x04080002,
	SKILLID_PhoenixShot				= 0x04080003,
	SKILLID_ForceofNature			= 0x04080004,

	SKILLID_LethalSight				= 0x04100001,
	SKILLID_FierceWind				= 0x04100002,
	SKILLID_EntalingRoots			= 0x04100003,
	SKILLID_Bombardment				= 0x04100004,

	//Pikeman
	SKILLID_PikeWind				= 0x03010001,
	SKILLID_IceAttribute			= 0x03010002,
	SKILLID_CriticalHit				= 0x03010003,
	SKILLID_JumpingCrash			= 0x03010004,

	SKILLID_GroundPike				= 0x03020001,
	SKILLID_Tornado					= 0x03020002,
	SKILLID_WeaponDefenseMastery	= 0x03020003,
	SKILLID_Expansion				= 0x03020004,
	
	SKILLID_VenomSpear				= 0x03040001,
	SKILLID_Vanish					= 0x03040002,
	SKILLID_CriticalMastery			= 0x03040003,
	SKILLID_ChainLance				= 0x03040004,

	SKILLID_AssassinsEye			= 0x03080001,
	SKILLID_ChargingStrike			= 0x03080002,
	SKILLID_Vague					= 0x03080003,
	SKILLID_ShadowMaster			= 0x03080004,

	SKILLID_RingofSpears			= 0x03100001,
	SKILLID_LastBreath				= 0x03100002,
	SKILLID_DeathMaster				= 0x03100003,
	SKILLID_Twister					= 0x03100004,

	//Atalanta
	SKILLID_ShieldStrike			= 0x06010001,
	SKILLID_Farina					= 0x06010002,
	SKILLID_ThrowingMaster			= 0x06010003,
	SKILLID_BiggerSpear				= 0x06010004,
										 
	SKILLID_Windy					= 0x06020001,
	SKILLID_TwistJavelin			= 0x06020002,
	SKILLID_SoulSucker				= 0x06020003,
	SKILLID_FireJavelin				= 0x06020004,
										 
	SKILLID_SplitJavelin			= 0x06040001,
	SKILLID_TriumphofJavelin		= 0x06040002,
	SKILLID_LightningJavelin		= 0x06040003,
	SKILLID_StormJavelin			= 0x06040004,
										 
	SKILLID_HallofValhalla			= 0x06080001,
	SKILLID_ExtremeRage				= 0x06080002,
	SKILLID_FrostJavelin			= 0x06080003,
	SKILLID_Vengeance				= 0x06080004,

	SKILLID_SummonTiger				= 0x06100001,
	SKILLID_GoldenApple				= 0x06100002,
	SKILLID_PlagueJavelin			= 0x06100003,
	SKILLID_ComboJavelin			= 0x06100004,

	//Knight
	SKILLID_SwordBlast				= 0x05010001,
	SKILLID_HolyBody				= 0x05010002,
	SKILLID_PhysicalTraining		= 0x05010003,
	SKILLID_DoubleCrash				= 0x05010004,

	SKILLID_HolyValor				= 0x05020001,
	SKILLID_Brandish				= 0x05020002,
	SKILLID_Piercing				= 0x05020003,
	SKILLID_DrasticSpirit			= 0x05020004,
	
	SKILLID_SwordMastery			= 0x05040001,
	SKILLID_DivineShield			= 0x05040002,
	SKILLID_HolyIncantation			= 0x05040003,
	SKILLID_GrandCross				= 0x05040004,

	SKILLID_SwordofJustice			= 0x05080001,
	SKILLID_GodlyShield				= 0x05080002,
	SKILLID_GodsBlessing			= 0x05080003,
	SKILLID_DivinePiercing			= 0x05080004,

	SKILLID_HolyConviction			= 0x05100001,
	SKILLID_WhirlWind				= 0x05100002,
	SKILLID_GloriousShield			= 0x05100003,
	SKILLID_DivineCross				= 0x05100004,

	//Magician
	SKILLID_Agony					= 0x08010001,
	SKILLID_FireBolt				= 0x08010002,
	SKILLID_Zenith					= 0x08010003,
	SKILLID_FireBall				= 0x08010004,

	SKILLID_MentalMastery			= 0x08020001,
	SKILLID_Watornado				= 0x08020002,
	SKILLID_EnchantWeapon			= 0x08020003,
	SKILLID_DeathRay				= 0x08020004,
	
	SKILLID_EnergyShield			= 0x08040001,
	SKILLID_Diastrophism			= 0x08040002,
	SKILLID_SpiritElemental			= 0x08040003,
	SKILLID_DancingSword			= 0x08040004,

	SKILLID_FireElemental			= 0x08080001,
	SKILLID_FlameWave				= 0x08080002,
	SKILLID_Distortion				= 0x08080003,
	SKILLID_Meteorite				= 0x08080004,

	SKILLID_WizardTrance			= 0x08100001,
	SKILLID_StoneSkin				= 0x08100002,
	SKILLID_RedRay					= 0x08100003,
	SKILLID_Cataclysm				= 0x08100004,

	//Priestess
	SKILLID_Healing					= 0x07010001,
	SKILLID_HolyBolt				= 0x07010002,
	SKILLID_MultiSpark				= 0x07010003,
	SKILLID_HolyMind				= 0x07010004,

	SKILLID_Meditation				= 0x07020001,
	SKILLID_DivineLightning			= 0x07020002,
	SKILLID_HolyReflection			= 0x07020003,
	SKILLID_GrandHealing			= 0x07020004,

	SKILLID_VigorBall				= 0x07040001,
	SKILLID_Resurrection			= 0x07040002,
	SKILLID_Extinction				= 0x07040003,
	SKILLID_VirtualLife				= 0x07040004,

	SKILLID_GlacialSpike			= 0x07080001,
	SKILLID_RegenerationField		= 0x07080002,
	SKILLID_ChainLightning			= 0x07080003,
	SKILLID_SummonMuspell			= 0x07080004,

	SKILLID_IceElemental			= 0x07100001,
	SKILLID_IceMeteorite			= 0x07100002,
	SKILLID_HeavenlyLight			= 0x07100003,
	SKILLID_Consecration			= 0x07100004,

	//Assassin
	SKILLID_Stinger					= 0x10010001,
	SKILLID_RunningHit				= 0x10010002,
	SKILLID_DualWieldMastery		= 0x10010003,
	SKILLID_Wisp					= 0x10010004,

	SKILLID_VenomThorn				= 0x10020001,
	SKILLID_Alas					= 0x10020002,
	SKILLID_SoulShock				= 0x10020003,
	SKILLID_AttackMastery			= 0x10020004,

	SKILLID_SoreBlade				= 0x10040001,
	SKILLID_GustSlash				= 0x10040002,
	SKILLID_Inpes					= 0x10040003,
	SKILLID_Blind					= 0x10040004,

	SKILLID_FrostWind				= 0x10080001,
	SKILLID_FatalMastery			= 0x10080002,
	SKILLID_Polluted				= 0x10080003,
	SKILLID_NinjaShadow				= 0x10080004,

	SKILLID_ShadowBlink				= 0x10100001,
	SKILLID_KissViper				= 0x10100002,
	SKILLID_ViolentStub				= 0x10100003,
	SKILLID_ShadowStorm				= 0x10100004,

	//Shaman
	SKILLID_DarkBolt				= 0x50010001,
	SKILLID_DarkWave				= 0x50010002,
	SKILLID_Inertia					= 0x50010003,
	SKILLID_InnerPeace				= 0x50010004,

	SKILLID_SpiritualFlare			= 0x50020001,
	SKILLID_SpiritualManacle		= 0x50020002,
	SKILLID_ChasingHunt				= 0x50020003,
	SKILLID_AdventMigal				= 0x50020004,
	
	SKILLID_Rainmaker				= 0x50040001,
	SKILLID_PhantomCall				= 0x50040002,
	SKILLID_Haunt					= 0x50040003,
	SKILLID_Scratch					= 0x50040004,

	SKILLID_RecallHestian			= 0x50080001,
	SKILLID_Judgement				= 0x50080002,
	SKILLID_AdventMidranda			= 0x50080003,
	SKILLID_MourningPray			= 0x50080004,

	SKILLID_Creed					= 0x50100001,
	SKILLID_PressDeity				= 0x50100002,
	SKILLID_PhantomNail				= 0x50100003,
	SKILLID_OccultLife				= 0x50100004,

	//Stones
	SKILLID_FighterStone			= 0x09040001,
	SKILLID_MechanicianStone		= 0x09040002,
	SKILLID_PikemanStone			= 0x09040003,
	SKILLID_ArcherStone				= 0x09040004,
	SKILLID_KnightStone				= 0x09040005,
	SKILLID_AtalantaStone			= 0x09040006,
	SKILLID_PriestessStone			= 0x09040007,
	SKILLID_MagicianStone			= 0x09040008,
	SKILLID_ShamanStone				= 0x09040009,
	SKILLID_AssassinStone			= 0x0904000A,

	//Timers
	SKILLID_AvertScroll				= 0x09080003,
	SKILLID_AbsorptionOfGlory		= 0x09010004,
	SKILLID_PowerSiege				= 0x09010005,
	SKILLID_EvasionAdaption			= 0x09010006,
	SKILLID_Force					= 0x09010001,
	SKILLID_QUEST1					= 0x09010002,
	SKILLID_QUEST2					= 0x09010003,
	SKILLID_QUEST3					= 0x09010010,
	SKILLID_QUEST4					= 0x09010020,
	SKILLID_QUESTNew				= 0x096A0003,
	SKILLID_QUESTNewTime			= 0x096B0003,
	SKILLID_EventTimer				= 0x096A0004,
	SKILLID_Atanasia				= 0x09020001,
	SKILLID_DeadlyEdge				= 0x09020002,
	SKILLID_AvailofEvasion			= 0x09020003,
	SKILLID_DefianceStoneA			= 0x09020004,
	SKILLID_DefianceStoneB			= 0x09020005,
	SKILLID_DefianceStoneC			= 0x09020006,
	SKILLID_MightStone				= 0x09040010,
	SKILLID_EternalLife				= 0x09080001,
	SKILLID_FatalEdge				= 0x09080002,
	SKILLID_Rivera					= 0x09080004,

	SKILLID_BellatraGold			= 0x096A0061,
	SKILLID_BellatraSilver			= 0x096A0062,
	SKILLID_BellatraBronze			= 0x096A0063,

	SKILLID_PvPHonor				= 0x096B0051,
	SKILLID_BellatraHonor			= 0x096B0052,
	
	SKILLID_PKMode					= 0x096A0051,
};

enum ESkillShortID
{
		//Mechanician
	SKILLSHORTID_ExtremeShield				=  SKILLPLAYID_T11,				
	SKILLSHORTID_MechanicBomb				=  SKILLPLAYID_T12,				
	SKILLSHORTID_PhysicalAbsorption			=  SKILLPLAYID_T13,			
	SKILLSHORTID_PoisonAttribute			=  SKILLPLAYID_Undefined,		

	SKILLSHORTID_GreatSmash					=  SKILLPLAYID_T21,				
	SKILLSHORTID_Maximize					=  SKILLPLAYID_T22,					
	SKILLSHORTID_Automation					=  SKILLPLAYID_T23,					
	SKILLSHORTID_Spark						=  SKILLPLAYID_T24,						
	
	SKILLSHORTID_MetalArmor					=  SKILLPLAYID_T31,					
	SKILLSHORTID_GrandSmash					=  SKILLPLAYID_T32,					
	SKILLSHORTID_MechanicMastery			=  SKILLPLAYID_Undefined,		
	SKILLSHORTID_SparkShield				=  SKILLPLAYID_T33,				
	
	SKILLSHORTID_Impulsion					=  SKILLPLAYID_T41,					
	SKILLSHORTID_Compulsion					=  SKILLPLAYID_T42,					
	SKILLSHORTID_MagneticSphere				=  SKILLPLAYID_T43,				
	SKILLSHORTID_MetalGolem					=  SKILLPLAYID_T44,					
	
	SKILLSHORTID_Rupture					=  SKILLPLAYID_T51,					
	SKILLSHORTID_MagneticDischarge			=  SKILLPLAYID_T52,			
	SKILLSHORTID_HardeningShield			=  SKILLPLAYID_T53,			
	SKILLSHORTID_ParasitShot				=  SKILLPLAYID_T54,				

	//Fighter
	SKILLSHORTID_MeleeMastery				=  SKILLPLAYID_Undefined,			
	SKILLSHORTID_FireAttribute				=  SKILLPLAYID_Undefined,			
	SKILLSHORTID_Raving						=  SKILLPLAYID_T11,						
	SKILLSHORTID_Impact						=  SKILLPLAYID_T12,						

	SKILLSHORTID_TripleImpact				=  SKILLPLAYID_T21,				
	SKILLSHORTID_BrutalSwing				=  SKILLPLAYID_T22,				
	SKILLSHORTID_Roar						=  SKILLPLAYID_T23,						
	SKILLSHORTID_RageofZecram				=  SKILLPLAYID_T24,				
	
	SKILLSHORTID_Concentration				=  SKILLPLAYID_T31,				
	SKILLSHORTID_AvengingCrash				=  SKILLPLAYID_T32,				
	SKILLSHORTID_SwiftAxe					=  SKILLPLAYID_T33,					
	SKILLSHORTID_BoneCrash					=  SKILLPLAYID_T34,					
	
	SKILLSHORTID_Destroyer					=  SKILLPLAYID_T41,					
	SKILLSHORTID_Berserker					=  SKILLPLAYID_T42,					
	SKILLSHORTID_CycloneStrike				=  SKILLPLAYID_T43,				
	SKILLSHORTID_BoostHealth				=  SKILLPLAYID_Undefined,			
	
	SKILLSHORTID_SesmicImpact				=  SKILLPLAYID_T51,				
	SKILLSHORTID_POWERDASH					=  SKILLPLAYID_T52,					
	SKILLSHORTID_MOTALBLOW					=  SKILLPLAYID_T53,					
	SKILLSHORTID_BLOODYBERSERKER			=  SKILLPLAYID_T54,			

	//Pikeman
	SKILLSHORTID_PikeWind					=  SKILLPLAYID_T11,					
	SKILLSHORTID_IceAttribute				=  SKILLPLAYID_Undefined,			
	SKILLSHORTID_CriticalHit				=  SKILLPLAYID_T12,				
	SKILLSHORTID_JumpingCrash				=  SKILLPLAYID_T13,				
	
	SKILLSHORTID_GroundPike					=  SKILLPLAYID_T21,					
	SKILLSHORTID_Tornado					=  SKILLPLAYID_T22,					
	SKILLSHORTID_WeaponDefenceMastery		=  SKILLPLAYID_Undefined,	
	SKILLSHORTID_Expansion					=  SKILLPLAYID_T23,					
	
	SKILLSHORTID_VenomSpear					=  SKILLPLAYID_T31,					
	SKILLSHORTID_Vanish						=  SKILLPLAYID_T32,						
	SKILLSHORTID_CriticalMastery			=  SKILLPLAYID_Undefined,		
	SKILLSHORTID_ChainLance					=  SKILLPLAYID_T33,					
	
	SKILLSHORTID_AssassinsEye				=  SKILLPLAYID_T41,				
	SKILLSHORTID_ChargingStrike				=  SKILLPLAYID_T42,				
	SKILLSHORTID_Vague						=  SKILLPLAYID_T43,						
	SKILLSHORTID_ShadowMaster				=  SKILLPLAYID_T44,				
	
	SKILLSHORTID_DANCINGREAPER				=  SKILLPLAYID_T51,				
	SKILLSHORTID_FINALSPEAR					=  SKILLPLAYID_T52,					
	SKILLSHORTID_AMPLIFIED					=  SKILLPLAYID_T53,					
	SKILLSHORTID_SIDESPINATTACK				=  SKILLPLAYID_T54,				
	
	//Archer
	SKILLSHORTID_ScoutHawk					=  SKILLPLAYID_T11,					
	SKILLSHORTID_ShootingMastery			=  SKILLPLAYID_Undefined,		
	SKILLSHORTID_WindArrow					=  SKILLPLAYID_T12,					
	SKILLSHORTID_PerfectAim					=  SKILLPLAYID_T13,					
	
	SKILLSHORTID_DionsEye					=  SKILLPLAYID_Undefined,				
	SKILLSHORTID_Falcon						=  SKILLPLAYID_T21,						
	SKILLSHORTID_ArrowofRage				=  SKILLPLAYID_T22,				
	SKILLSHORTID_Avalanche					=  SKILLPLAYID_T23,					
	
	SKILLSHORTID_ElementalShot				=  SKILLPLAYID_T31,				
	SKILLSHORTID_GoldenFalcon				=  SKILLPLAYID_T32,				
	SKILLSHORTID_BombShot					=  SKILLPLAYID_T33,					
	SKILLSHORTID_Perforation				=  SKILLPLAYID_T34,				
	
	SKILLSHORTID_RecallWolverine			=  SKILLPLAYID_T41,				
	SKILLSHORTID_EvasionMastery				=  SKILLPLAYID_Undefined,			
	SKILLSHORTID_PhoenixShot				=  SKILLPLAYID_T42,				
	SKILLSHORTID_ForceOfNature				=  SKILLPLAYID_T43,				
	
	SKILLSHORTID_LethalSight				=  SKILLPLAYID_T51,				
	SKILLSHORTID_FierceWind					=  SKILLPLAYID_T52,					
	SKILLSHORTID_NATURESPLASH				=  SKILLPLAYID_T53,				
	SKILLSHORTID_CIRCLETRAP					=  SKILLPLAYID_T54,					

	//Knight
	SKILLSHORTID_SwordBlast					=  SKILLPLAYID_T11,					
	SKILLSHORTID_HolyBody					=  SKILLPLAYID_T12,					
	SKILLSHORTID_PhysicalTraining			=  SKILLPLAYID_Undefined,		
	SKILLSHORTID_DoubleCrash				=  SKILLPLAYID_T13,				

	SKILLSHORTID_HolyValor					=  SKILLPLAYID_T21,					
	SKILLSHORTID_Brandish					=  SKILLPLAYID_T22,					
	SKILLSHORTID_Piercing					=  SKILLPLAYID_T23,					
	SKILLSHORTID_DrasticSpirit				=  SKILLPLAYID_T24,				

	SKILLSHORTID_SwordMastery				=  SKILLPLAYID_Undefined,			
	SKILLSHORTID_DivineShield				=  SKILLPLAYID_T31,			
	SKILLSHORTID_HolyIncantation			=  SKILLPLAYID_T32,			
	SKILLSHORTID_GrandCross					=  SKILLPLAYID_T33,					

	SKILLSHORTID_DivinePiercing				=  SKILLPLAYID_T44,				
	SKILLSHORTID_DIVINEPIERCING2			=  0x90,						
	SKILLSHORTID_DIVINEPIERCING3			=  0x91,						
	SKILLSHORTID_GodlyShield				=  SKILLPLAYID_T42,				
	SKILLSHORTID_GodsBlessing				=  SKILLPLAYID_T43,					
	SKILLSHORTID_SwordOfJustice				=  SKILLPLAYID_T41,				

	SKILLSHORTID_HolyConviction				=  SKILLPLAYID_T51,				
	SKILLSHORTID_DivineInquisition			=  SKILLPLAYID_T52,			
	SKILLSHORTID_SAINTBLADE					=  SKILLPLAYID_T53,					
	SKILLSHORTID_HOLYBENEDIC				=  SKILLPLAYID_T54,				

	//Atalanta
	SKILLSHORTID_ShieldStrike				 =  SKILLPLAYID_T11,				
	SKILLSHORTID_Farina				 =  SKILLPLAYID_T12,						
	SKILLSHORTID_ThrowingMastery				 =  SKILLPLAYID_Undefined,		
	SKILLSHORTID_VigorSpear				 =  SKILLPLAYID_T13,					

	SKILLSHORTID_Windy				 =  SKILLPLAYID_T21,						
	SKILLSHORTID_TwistJavelin				 =  SKILLPLAYID_T22,				
	SKILLSHORTID_SoulSucker				 =  SKILLPLAYID_T23,					
	SKILLSHORTID_FireJavelin				 =  SKILLPLAYID_T24,				

	SKILLSHORTID_SplitJavelin				 =  SKILLPLAYID_T31,				
	SKILLSHORTID_TriumphOfValhalla				 =  SKILLPLAYID_T32,			
	SKILLSHORTID_LightningJavelin				 =  SKILLPLAYID_T33,			
	SKILLSHORTID_StormJavelin				 =  SKILLPLAYID_T34,				

	SKILLSHORTID_HALLOFVALHALLA				 =  SKILLPLAYID_T41,				
	SKILLSHORTID_XRAGE				 =  SKILLPLAYID_T42,						
	SKILLSHORTID_FROSTJAVELIN				 =  SKILLPLAYID_T43,				
	SKILLSHORTID_VENGEANCE				 =  SKILLPLAYID_T44,					

	SKILLSHORTID_ComboJavelin				 =  SKILLPLAYID_T51,				
	SKILLSHORTID_GALAXYCOUP				 =  SKILLPLAYID_T52,					
	SKILLSHORTID_SUMMONARCUDA				 =  SKILLPLAYID_T53,				
	SKILLSHORTID_SNIPPYFEAR				 =  SKILLPLAYID_T54,					

	//Priestess
	SKILLSHORTID_Healing				 =  SKILLPLAYID_T11,					
	SKILLSHORTID_HolyBolt				 =  SKILLPLAYID_T12,					
	SKILLSHORTID_MultiSpark				 =  SKILLPLAYID_T13,					
	SKILLSHORTID_HolyMind				 =  SKILLPLAYID_T14,					

	SKILLSHORTID_Meditation				 =  SKILLPLAYID_Undefined,				
	SKILLSHORTID_DivineLightning				 =  SKILLPLAYID_T21,			
	SKILLSHORTID_HolyReflection				 =  SKILLPLAYID_T22,				
	SKILLSHORTID_GrandHealing				 =  SKILLPLAYID_T23,				

	SKILLSHORTID_VigorBall				 =  SKILLPLAYID_T31,					
	SKILLSHORTID_Resurrection				 =  SKILLPLAYID_T32,				
	SKILLSHORTID_Extinction				 =  SKILLPLAYID_T33,					
	SKILLSHORTID_VirtualLife				 =  SKILLPLAYID_T34,				

	SKILLSHORTID_GLACIALSPIKE				 =  SKILLPLAYID_T41,				
	SKILLSHORTID_REGENERATIONFIELD				 =  SKILLPLAYID_T42,			
	SKILLSHORTID_CHAINLIGHTNING				 =  SKILLPLAYID_T43,				
	SKILLSHORTID_SUMMONMUSPELL				 =  SKILLPLAYID_T44,				

	SKILLSHORTID_SpiritImpact				 =  SKILLPLAYID_T51,				
	SKILLSHORTID_PiercingIce				 =  SKILLPLAYID_T52,				
	SKILLSHORTID_SummonRamiel				 =  SKILLPLAYID_T53,				
	SKILLSHORTID_BlessingKrishna				 =  SKILLPLAYID_T54,			

	//Magician
	SKILLSHORTID_Agony				 =  SKILLPLAYID_T11,						
	SKILLSHORTID_FireBolt				 =  SKILLPLAYID_T12,					
	SKILLSHORTID_Zenith				 =  SKILLPLAYID_T13,						
	SKILLSHORTID_FireBall				 =  SKILLPLAYID_T14,					

	SKILLSHORTID_MentalMastery				 =  SKILLPLAYID_Undefined,			
	SKILLSHORTID_ColumnOfWater				 =  SKILLPLAYID_T21,				
	SKILLSHORTID_EnchantWeapon				 =  SKILLPLAYID_T22,				
	SKILLSHORTID_DeadRay				 =  SKILLPLAYID_T23,					

	SKILLSHORTID_EnergyShield				 =  SKILLPLAYID_T31,				
	SKILLSHORTID_Diastrophism				 =  SKILLPLAYID_T32,				
	SKILLSHORTID_SpiritElemental				 =  SKILLPLAYID_T33,			
	SKILLSHORTID_DancingSword				 =  SKILLPLAYID_T34,				

	SKILLSHORTID_FIREELEMENTAL				 =  SKILLPLAYID_T41,				
	SKILLSHORTID_FLAMEWAVE				 =  SKILLPLAYID_T42,					
	SKILLSHORTID_DISTORTION				 =  SKILLPLAYID_T43,					
	SKILLSHORTID_METEO				 =  SKILLPLAYID_T44,						

	SKILLSHORTID_StoneSkin				 =  SKILLPLAYID_T51,					
	SKILLSHORTID_RedRay				 =  SKILLPLAYID_T52,						
	SKILLSHORTID_Primaignis				 =  SKILLPLAYID_T53,					
	SKILLSHORTID_Thirdanima				 =  SKILLPLAYID_T54,					

	//Assassin
	SKILLSHORTID_Stingger				 =  SKILLPLAYID_T11,					
	SKILLSHORTID_RunningHit				 =  SKILLPLAYID_T12,					
	SKILLSHORTID_DualSwordMastery				 =  SKILLPLAYID_Undefined,		
	SKILLSHORTID_Wisp				 =  SKILLPLAYID_T14,						
	
	SKILLSHORTID_VenomThrone				 =  SKILLPLAYID_T21,				
	SKILLSHORTID_Alas				 =  SKILLPLAYID_T22,						
	SKILLSHORTID_SoulShock				 =  SKILLPLAYID_T23,					
	SKILLSHORTID_AttackMastery				 =  SKILLPLAYID_Undefined,			
	
	SKILLSHORTID_SoreSword				 =  SKILLPLAYID_T31,					
	SKILLSHORTID_BeatUp				 =  SKILLPLAYID_T32,						
	SKILLSHORTID_Inpes				 =  SKILLPLAYID_T33,						
	SKILLSHORTID_Blind				 =  SKILLPLAYID_T34,						
	
	SKILLSHORTID_FrostWind				 =  SKILLPLAYID_T41,					
	SKILLSHORTID_AsnCriticalMastery				 =  SKILLPLAYID_Undefined,		
	SKILLSHORTID_Polluted				 =  SKILLPLAYID_T43,					
	SKILLSHORTID_PastingShadow				 =  SKILLPLAYID_T44,				
	
	SKILLSHORTID_ShadowBomb				 =  SKILLPLAYID_T51,					
	SKILLSHORTID_RisingSlash				 =  SKILLPLAYID_T52,				
	SKILLSHORTID_ViolenceStab				 =  SKILLPLAYID_T53,				
	SKILLSHORTID_Storm				 =  SKILLPLAYID_T54,						

	//Shaman
	SKILLSHORTID_DarkBolt				 =  SKILLPLAYID_T11,					
	SKILLSHORTID_DarkWave				 =  SKILLPLAYID_T12,					
	SKILLSHORTID_CurseLazy				 =  SKILLPLAYID_T13,					
	SKILLSHORTID_InnerPeace				 =  SKILLPLAYID_Undefined,				
	
	SKILLSHORTID_SpiritualFlare				 =  SKILLPLAYID_T21,				
	SKILLSHORTID_SoulManacle				 =  SKILLPLAYID_T22,				
	SKILLSHORTID_ChasingHunt				 =  SKILLPLAYID_T23,				
	SKILLSHORTID_AdventMigal				 =  SKILLPLAYID_T24,				
	
	SKILLSHORTID_RainMaker				 =  SKILLPLAYID_T31,					
	SKILLSHORTID_LandOfGhost				 =  SKILLPLAYID_T32,				
	SKILLSHORTID_Haunt				 =  SKILLPLAYID_T33,						
	SKILLSHORTID_Scratch				 =  SKILLPLAYID_T34,					
	
	SKILLSHORTID_RecallBloodyKnight				 =  SKILLPLAYID_T41,			
	SKILLSHORTID_Judgement				 =  SKILLPLAYID_T42,					
	SKILLSHORTID_AdventMidranda				 =  SKILLPLAYID_T43,				
	SKILLSHORTID_MourningOfPray				 =  SKILLPLAYID_T44,				
	
	SKILLSHORTID_Creed				 =  SKILLPLAYID_T51,						
	SKILLSHORTID_PressOfDeity				 =  SKILLPLAYID_T52,				
	SKILLSHORTID_GhostyNail				 =  SKILLPLAYID_T53,					
	SKILLSHORTID_HighRegeneration				 =  SKILLPLAYID_Undefined,		
};

struct ActiveSkill
{
	union
	{
		struct
		{
			unsigned char		cSkillTinyID;
			unsigned char		cLevel;
			unsigned short		sValue;
		};

		int iValue;
	};

	ActiveSkill() {}
	ActiveSkill( int iValue ) : iValue(iValue) {}
};

struct RecordSkill
{
	BYTE		baSkillPoints[20];
	WORD		waSkillMastery[20];
	BYTE		baShortKey[20];
	WORD		waSelectSkill[2];
};

struct SkillBox
{
	RECT								BoxRect;
	RECT								GageRect;
};

struct SkillBase
{
	char                                szName[64];
	char                                szDescription[150];
	int                                 iSkillLevelRequired;
	int                                 iaSkillUseStamina[2];		// 0 = stamina value static, 1 = increase value each level skill
	int                                 iaSkillMastery[2];
	int                                 iaSkillElements[3];
	EItemType                           eSkillItemAllowed[8];
	DWORD								dwFunctionPointer;
	ESkillID				            eSkillID;
	ESkillType                          eSkillType;
	DWORD							    dwUseManaPointer;
	int                                 iSkillIndex;
};

struct Skill
{
	char                                szName[32];
	DWORD                               iID;
	char                                szFileName[32];
	char                                szIconName[32];
	int                                 bActive;
	BOOL                                bCanUse;
	int                                 iLevel;
	int                                 ShortKey;
	int                                 MousePosi;
	int                                 Position;
	int                                 iUseTime;
	int                                 iCheckTime;
	int                                 GageLength;
	int                                 GageLength2;
	int                                 Mastery;
	int                                 UseSKillIncreCount;
	int                                 UseSkillCount;
	int 								UseSkillMastery;
	int                                 UseSkillMasteryGage;
	int                                 UseSkillFlag;
	int                                 PlusState[5];
	int                                 Time[2];
	int                                 PartyFlag;
	int                                 ElementIndex;
	int                                 MatIcon;

	int                                 IconAlpha;
	int                                 IconPosiX;
	int                                 IconTime;
	int                                 IconFlag;

	DWORD                               dwSkillTarget;
	int                                 SummonFlag;

	SkillBase							sSkillInfo;
};

struct UseSkill
{
	Skill		saUseSkill[20];
	SkillBox	saSkillBox[21];
	Skill	  * pLeftSkill;
	Skill	  * pRightSkill;
	int			iSkillPoint;
	int			iSkillPointTier4;
};

struct PacketSkillStatus : Packet
{
	int									iID;

	//Mechanician
	short								sMagneticDischargeLevel;
	DWORD								dwMagneticDischargeTimeLeft;

	//Archer T5
	short								sLethalSightLevel;
	DWORD								dwLethalSightTimeLeft;
	short								sFierceWindLevel;
	DWORD								dwFierceWindTimeLeft;

	//Knight T5
	short								sHolyConvictionLevel;
	DWORD								dwHolyConvictionTimeLeft;
	bool								bWhirlWind;

	//Mage T5
	short								sWizardTranceLevel;
	DWORD								dwWizardTranceTimeLeft;

	//Assassin
	short								sKissViperLevel;
	DWORD								dwKissViperTimeLeft;
};

struct PacketSkillStatusContainer : Packet
{
	int				  iAmount;

	BYTE			  baBuffer[SKILLCONTAINER_LENGTH];
};

struct PacketSkillCast : Packet
{
	union
	{
		int						iClass;
		int						iSkillLevel;
	};

	ActiveSkill					sActiveSkill;
	int							iID;
};

struct PacketUseSkillNew : Packet
{
	ESkillID		  iSkillID;
	unsigned int	  iSkillLevel;
	int				  lCasterID;
	int				  lTargetID;
};

const Rectangle2D saSkillPage2Button[4]
{
	//Skill T5A1
	{ 23, 108, 39, 36 },
	//Skill T5A2
	{ 62, 152, 39, 36 },
	//Skill T5A3
	{ 72, 72, 39, 36 },
	//Skill T5A4
	{ 132, 82, 39, 36 },
};

const Skill saSkill[MAX_SKILLS] = 
{
	{ "³ë¸Ö¾îÅÃ ", 0x11111111, "Skill_Normal" },

	//Mechanician Skills - Tier 1
	{ "ÀÍ½ºÆ®¸²½¯µå", SKILLID_ExtremeShield, "TM10 Ex_shield", "Mc1_E_SHIELD.tga" },
	{ "¸ÞÄ«´Ð¹ü", SKILLID_MechanicBomb, "TM12 M_bomb" },
	{ "ÇÇÁöÄÃ ¾÷¼Ù", SKILLID_PoisonAttribute, "TM14 P_attribute" },
	{ "Æ÷ÀÌÁð ¾îÆ®¸®ºäÆ®", SKILLID_PhysicalAbsorb, "TM17 Ph_absorb", "Mc1_P_ABSORB.tga" },

	//Tier 2
	{ "¶ó¿îµå ÀÓÆåÆ®", SKILLID_GreatSmash, "TM20 G_Smash" },
	{ "¸Æ½Ã¸¶ÀÌÁî ", SKILLID_Maximize, "TM23 Maximize", "Mc2_MAXIMIZE.tga" },
	{ "¿ÀÅä¸ÞÀÌ¼Ç", SKILLID_Automation, "TM26 Automation", "Mc2_AUTOMATION.tga" },
	{ "½ºÆÄÅ©", SKILLID_Spark, "TM30 Spark" },

	//Tier 3
	{ "¸ÞÅ» ¾Æ¸Ó", SKILLID_MetalArmor, "TM40 M_Armor", "Mc3_M_ARMOR.tga" },
	{ "±×·£µå ½º¸Þ½¬", SKILLID_GrandSmash, "TM43 Grand_Smash" },
	{ "¸ÞÄ«´Ð ¿þÆÝ", SKILLID_MechanicWeaponMastery, "TM44 M_Weapon" },
	{ "½ºÆÄÆ® ½¯µå", SKILLID_SparkShield, "TM50 S_Shield", "Mc3_S_SHIELD.tga" },

	//Tier 4
	{ "ÀÓÇÃ·¹ÀÌ¼Ç", SKILLID_Impulsion, "TM60 Impulsion" },
	{ "ÄÄÇÃ·¹ÀÌ¼Ç", SKILLID_Compulsion, "TM63 Compulsion", "Mc4_COMPULSION.tga" },
	{ "¸Þ³×Æ½ ÇÇ¾î", SKILLID_MagneticSphere, "TM66 M_Sphere", "Mc4_M_SPHERE.tga" },
	{ "¸ÞÅ» °ñ·½", SKILLID_MetalGolem, "TM70 M_Golem", "Mc4_M_GOLEM.tga" },

	//Tier 5
	{ "·Ñ¸µ ½º¸Å½¬", SKILLID_Rupture, "TM100 Rupture" },
	{ "ÆÄ¿ö ÀÎÇÚ½º", SKILLID_MagneticDischarge, "TM103 Magnetic_Discharge" },
	{ "ÇÏÀÌÆÛ ¼Ò´Ð", SKILLID_HardeningShield, "TM106 Hardening_Shield" },
	{ "·£µå ¸¶ÀÌ´×", SKILLID_ParasitShot, "TM110 Parasit_Shot" },

	//Fighter Skills - Tier 1
	{ "¹èÆ² ¸¶½ºÅÍ¸®", SKILLID_MeleeMastery, "TF10 M_mastery" },
	{ "ÆÄÀÌÅÍ ¾îÆ®¸®ºäÆ®", SKILLID_FireAttribute, "TF12 F_attribute" },
	{ "·¹ÀÌºù", SKILLID_Raving, "TF14 raving" },
	{ "ÀÓÆÑÆ®", SKILLID_Impact, "TF17 impact" },

	//Tier 2
	{ "Æ®¸®ÇÃ ÀÓÆÑÆ®", SKILLID_TripleImpact, "TF20 T_Impact" },
	{ "ºÎ·çÆ² ½ºÀ®", SKILLID_BrutalSwing, "TF23 B_Swing" },
	{ "·Î¾î", SKILLID_Roar, "TF26 Roar" },
	{ "·¹ÀÌÁö¿Àºê Á¦Å©¶÷", SKILLID_RageofZecram, "TF30 R_Zecram" },

	//Tier 3
	{ "ÄÁ¼¾Æ®·¹ÀÌ¼Ç", SKILLID_Concentration, "TF40 Concentration", "Ft3_CONCENTRATION.tga" },
	{ "¾îº¥Â¡ Å©·¡½¬", SKILLID_AvengingCrash, "TF43 A_Crash" },
	{ "½ºÀ§ÇÁ¿¢½º", SKILLID_SwiftAxe, "TF46 S_Axe", "Ft3_S_AXE.tga" },
	{ "º» Å©·¡½¬", SKILLID_BoneCrash, "TF50 B_Crash" },

	//Tier 4
	{ "µð½ºÅä¸®¾î", SKILLID_Destroyer, "TF60 Destoryer" },
	{ "¹ö¼­Ä¿", SKILLID_Berserker, "TF63 Berserker", "Ft4_BERSERKER.tga" },
	{ "½ÎÀÌÅ¬·Ð ½ºÆÄÀÌÅ©", SKILLID_CycloneStrike, "TF66 C_Strike" },
	{ "ºÎ½ºÆ® Çï½º", SKILLID_BoostHealth, "TF70 B_Health" },

	//Tier 5
	{ "´Ù¿î Èý", SKILLID_SurvivalInstinct, "TF100 Survival_Instinct" },
	{ "ÆÄ¿ö ´ë½Ã", SKILLID_SeismicImpact, "TF103 Sesmic_Impact" },
	{ "¸ðÅ» ºí·Î¿ì", SKILLID_RuthlessofZecram, "TF106 Ruthless_Zecram" },
	{ "ºí·¯µð ¹ö¼­Ä¿", SKILLID_WhirlwindFS01, "TF110 Whirlwind" },

	//Pikeman Skills - Tier 1
	{ "ÆÄÀÌÅ©À©µå", SKILLID_PikeWind, "TP10 P_wind" },
	{ "¾ÆÀÌ½º ¾îÆ®¸®ºäÆ®", SKILLID_IceAttribute, "TP12 I_attribute" },
	{ "Å©¸®Æ¼ÄÃ Èý", SKILLID_CriticalHit, "TP14 Cri_hit" },
	{ "Á¡ÇÎÅ©·¡½¬", SKILLID_JumpingCrash, "TP17 J_Crash" },

	//Tier 2
	{ "±×¶ó¿îµåÆÄÀÌÅ©", SKILLID_GroundPike, "TP20 G_Pike" },
	{ "Åä³×ÀÌµµ", SKILLID_Tornado, "TP23 Tornado" },
	{ "¿þÆÝµðÆæ½º¸¶½ºÅÍ¸®", SKILLID_WeaponDefenseMastery, "TP26 W_D_Mastery" },
	{ "ÀÍ½ºÆæ¼Ç", SKILLID_Expansion, "TP30 Expasion" },

	//Tier 3
	{ "º£³ð ½ºÇÇ¾î", SKILLID_VenomSpear, "TP40 V_Spear" },
	{ "ºí·¹ÀÌµå¿Àºêºí·¹ÀÌÁî", SKILLID_Vanish, "TP43 Vanish", "Pk3_VANISH.tga" },
	{ "»þÇÁ¸¶½ºÅÍ¸®", SKILLID_CriticalMastery, "TP46 C_Mastery" },
	{ "Ã¼ÀÎ·£½º", SKILLID_ChainLance, "TP50 C_Lance" },

	//Tier 4
	{ "¾î½Ø½Å ¾ÆÀÌ", SKILLID_AssassinsEye, "TP60 A_Eye", "Pk4_A_EYE.tga" },
	{ "Â÷Â¡ ½ºÆ®¶óÀÌÅ©", SKILLID_ChargingStrike, "TP63 C_Strike" },
	{ "º£ÀÌ±×", SKILLID_Vague, "TP66 Vague", "Pk4_VAGUE.tga" },
	{ "»õµÍ¿ì ¸¶½ºÅÍ", SKILLID_ShadowMaster, "TP70 S_Master" },

	//Tier 5
	{ "´í½Ì ¸®ÆÛ", SKILLID_RingofSpears, "TP100 Ring_Spears" },
	{ "ÆÄÀÌ³Î ½ºÇÇ¾î", SKILLID_LastBreath, "TP103 Focus" },
	{ "¿¥ÇÃ¸®ÆÄÀÌµå", SKILLID_DeathMaster, "TP106 Death_Master" },
	{ "»çÀÌµå ½ºÇÉ ¾îÅÃ", SKILLID_Twister, "TP110 Twister" },

	//Archer Skills - Tier 1
	{ "½ºÄ«¿ô È£Å©", SKILLID_ScoutHawk, "TA10 S_hawk", "Ac1_S_HAWK.tga" },
	{ "½´ÆÃ ¸¶½ºÅÍ¸®", SKILLID_ShootingMastery, "TA12 S_mastery" },
	{ "À©µå ¾Ö·Î¿ì", SKILLID_WindArrow, "TA14 W_arrow" },
	{ "ÆÛÆåÆ® ¿¡ÀÓ", SKILLID_PerfectAim, "TA17 P_aim" },

	//Tier 2
	{ "µð¿Â½º ¾ÆÀÌ", SKILLID_DionsEye, "TA20 D_Eye" },
	{ "ÆÈÄÜ", SKILLID_Falcon, "TA23 Falcon", "Ac2_FALCON.tga" },
	{ "¾Ö·Î¿ì ¿Àºê·¹ÀÌÁö", SKILLID_ArrowofRage, "TA26 A_Rage" },
	{ "¾Æ¹ß¶õÃ¼", SKILLID_Avalanche, "TA30 Avalanchie" },

	//Tier 3
	{ "¿¤·¹¸àÅ»¼¦", SKILLID_ElementalShot, "TA40 E_Shot" },
	{ "°ñµçÆÈÄÜ", SKILLID_GoldenFalcon, "TA43 G_Falcon", "Ac3_G_FALCON.tga" },
	{ "¹ü ¼¦", SKILLID_BombShot, "TA46 B_Shot" },
	{ "ÆÛÆ÷·¹ÀÌ¼Ç", SKILLID_Perforation, "TA50 Perforation" },

	//Tier 4
	{ "¸®ÄÝ ¿ï¹ö¸°", SKILLID_RecallWolverine, "TA60 R_Wolverin", "Ac4_R_WOLVERIN.tga" },
	{ "ÀÌº£ÀÌ¼Ç ¸¶½ºÅÍ¸®", SKILLID_EvasionMastery, "TA63 E_Mastery" },
	{ "ÇÇ´Ð½º ¼¦", SKILLID_PhoenixShot, "TA66 P_Shot" },
	{ "Æ÷½º ¿Àºê ³»Ãò·²", SKILLID_ForceofNature, "TA70 F_O_Nature", "Ac4_F_O_NATURE.tga" },

	//Tier 5
	{ "½ã´õ·çÇÁ", SKILLID_LethalSight, "TA100 Lethal_Sight" },
	{ "ÀÌº£ÀÌµå¼¦", SKILLID_FierceWind, "TA103 Fierce_Wind" },
	{ "³×ÀÌÃÄ ½ºÇÃ·¡½¬", SKILLID_EntalingRoots, "TA106 Entailing_Roots" },
	{ "¼­Å¬Æ®·¦", SKILLID_Bombardment, "TA110 Bombardment" },

	//Assassin Skills - Tier 1
	{ "½ºÆÃ°Å", SKILLID_Stinger, "TA10 Stingger" },
	{ "·¯´× Èý", SKILLID_RunningHit, "TA12 R_Hit" },
	{ "µà¾ó¼Òµå ¸¶½ºÅÍ¸®", SKILLID_DualWieldMastery, "TA14 D_Mastery" },
	{ "À§½ºÇÁ", SKILLID_Wisp, "TA17 Wisp", "As1_WISP.tga" },

	//Tier 2
	{ "º£³ð ½ð", SKILLID_VenomThorn, "TA20 V_Throne" },
	{ "¾Ë¶ó½º", SKILLID_Alas, "TA23 Alas", "As2_ALAS.tga" },
	{ "¼Ò¿ï ¼îÅ©", SKILLID_SoulShock, "TA26 S_Shock" },
	{ "¾îÅÃ ¸¶½ºÅÍ¸®", SKILLID_AttackMastery, "TA30 A_Mastery" },

	//Tier 3
	{ "¼Ò¾î ¼Òµå", SKILLID_SoreBlade, "TA40 S_Sword" },
	{ "ºñÆ® ¾÷", SKILLID_GustSlash, "TA43 B_Up" },
	{ "ÀÎÆä½º", SKILLID_Inpes, "TA46 Inpes", "As3_INPES.tga" },
	{ "ºí¶óÀÎµå", SKILLID_Blind, "TA50 Blind", "As3_BLIND.tga" },
	
	//Tier 4
	{ "ÇÁ·Î½ºÆ® À©µå", SKILLID_FrostWind, "TA60 F_Wind" },
	{ "ÆäÀÌÆ² ¸¶½ºÅÍ¸®", SKILLID_FatalMastery, "TA63 F_Mastery" },
	{ "Æú·çÆ¼µå", SKILLID_Polluted, "TA66 Polluted" },
	{ "ÆäÀÌ½ºÆÃ ½¦µµ¿ì", SKILLID_NinjaShadow, "TA70 P_Shadow" },

	//Tier 5
	{ "½¦µµ¿ì ¹ü", SKILLID_ShadowBlink, "TS80 J_Bomb" },
	{ "¶óÀÌÂ¡ ½½·¡½¬", SKILLID_KissViper, "TS83 R_Slash" },
	{ "¹ÙÀÌ¿Ã·±½º ½ºÅÇ", SKILLID_ViolentStub, "TS86 V_Stab" },
	{ "½ºÅè", SKILLID_ShadowStorm, "TS90 Storm" },

	//Knight Skills - Tier 1
	{ "½º¿ö½ººí·¹½ºÆ®", SKILLID_SwordBlast, "MN10 S_Blast" },
	{ "È¦¸® ¹Ùµð", SKILLID_HolyBody, "MN12 H_Body", "Kt1_H_BODY.tga" },
	{ "ÇÇÁöÄÃÆ®·¹ÀÌ´×", SKILLID_PhysicalTraining, "MN14 P_Traning" },
	{ "´õºí Å©·¡½¬", SKILLID_DoubleCrash, "MN17 D_Crash" },

	//Tier 2
	{ "È¦¸® ¹ë·¯", SKILLID_HolyValor, "MN20 H_Valor", "Kt2_H_VALOR.tga" },
	{ "ºê·£µð½¬", SKILLID_Brandish, "MN23 Brandish" },
	{ "ÇÇ¾î½Ì", SKILLID_Piercing, "MN26 Piercing" },
	{ "µå·¹½ºÆ½ ½ºÇÇ¸´", SKILLID_DrasticSpirit, "MN30 D_Spirit", "Kt2_D_SPIRIT.tga" },

	//Tier 3
	{ "ÇÁ·¹ÀÓ ºê·£µð½¬", SKILLID_SwordMastery, "MN40 S_Mastery" },
	{ "µð¹ÙÀÎÀÎÈ¦·¹ÀÌ¼Ç", SKILLID_DivineShield, "MN43 D_Inhalation", "Kt3_D_INHALATION.tga" },
	{ "È¦¸®ÀÎÄµÅ×ÀÌ¼Ç", SKILLID_HolyIncantation, "MN46 H_Incantation" },
	{ "±×·£µå Å©·Î½º", SKILLID_GrandCross, "MN50 G_Cross" },

	//Tier 4
	{ "½º¿öµå ¿Àºê Àú½ºÆ½½º", SKILLID_SwordofJustice, "MN60 S_O_Justice" },
	{ "°«µé¸® ½¯µå", SKILLID_GodlyShield, "MN63 G_Shield", "Kt4_G_SHIELD.tga" },
	{ "°« ºí·¹½º", SKILLID_GodsBlessing, "MN66 G_Bless", "Kt4_G_BLESS.tga" },
	{ "µð¹ÙÀÎ ÇÇ¾î½Ì", SKILLID_DivinePiercing, "MN70 D_Piercing" },

	//Tier 5
	{ "¼Ò¿ï ºê·¹ÀÌÄ¿", SKILLID_HolyConviction, "MN100 Holy_Conviction" },
	{ "Å©·¹¼¾Æ® ¹®", SKILLID_WhirlWind, "MN103 Divine_Inquisiton" },
	{ "¼¼ÀÎÆ® ºí·¹ÀÌµå", SKILLID_GloriousShield, "MN106 Glorious_Shield" },
	{ "È¦¸® º£³×µñ", SKILLID_DivineCross, "MN110 Divine_Cross" },

	//Atalanta Skills - Tier 1
	{ "½¯µå ½ºÆ®¶óÀÌÅ©", SKILLID_ShieldStrike, "MA10 S_Strike" },
	{ "ÆÄ¸®³ª ", SKILLID_Farina, "MA12 Farina" },
	{ "µå·ÎÀ×¸¶½ºÅÍ¸®", SKILLID_ThrowingMaster, "MA14 T_Mastery" },
	{ "ºñ°Å ½ºÇÇ¾î", SKILLID_BiggerSpear, "MA17 V_Spear" },

	//Tier 2
	{ "À©µð", SKILLID_Windy, "MA20 Windy", "At2_WINDY.tga" },
	{ "Æ®À§½ºÆ® Àçºí¸°", SKILLID_TwistJavelin, "MA23 T_Javelin" },
	{ "¼Ò¿ï ½âÄ¿", SKILLID_SoulSucker, "MA26 S_Sucker" },
	{ "ÆÄÀÌ¾î Àçºí¸°", SKILLID_FireJavelin, "MA30 F_Javelin" },

	//Tier 3
	{ "½ºÇÃ¸´ ÀÚº§¸°", SKILLID_SplitJavelin, "MA40 Split_Javelin" },
	{ "Æ®¶óÀÌ¾öÇÁ¿Àºê¹ßÇÒ¶ó", SKILLID_TriumphofJavelin, "MA43 T_Of_Valhalla", "At3_T_O_VALHALLA.tga" },
	{ "¶óÀÌÆ®´×Àçºí¸°", SKILLID_LightningJavelin, "MA46 L_Javelin" },
	{ "½ºÅè Àçºí¸°", SKILLID_StormJavelin, "MA50 Storm_Javelin" },

	//Tier 4
	{ "Çï ¿Àºê ¹ßÇÒ¶ó", SKILLID_HallofValhalla, "MA60 H_O_Valhalla", "At4_H_O_VALHALLA.tga" },
	{ "¿¢½º ¶óÁö", SKILLID_ExtremeRage, "MA63 X_Rage" },
	{ "ÇÁ·Î½ºÆ® Àçºí¸°", SKILLID_FrostJavelin, "MA66 F_Javelin", "At4_F_JAVELIN.tga" },
	{ "º¥Ã¾", SKILLID_Vengeance, "MA70 Vengeance" },

	//Tier 5
	{ "¼­¸Õ ¾Æ¸£Äí´Ù", SKILLID_SummonTiger, "MA100 S_Tiger" },
	{ "Å»¶ó¸®¾Æ", SKILLID_GoldenApple, "MA103 Golden_Apple" },
	{ "°¶·°½Ã Äò", SKILLID_PlagueJavelin, "MA106 Plague_Javelin" },
	{ "½º´ÏÇÇ ÇÇ¾î", SKILLID_ComboJavelin, "MA110 Combo_Javelin" },

	//Priestess Skills - Tier 1
	{ "Èú¸µ", SKILLID_Healing, "MP10 Healing" },
	{ "È¦¸®º¼Æ® ", SKILLID_HolyBolt, "MP12 HolyBolt" },
	{ "¸ÖÆ¼½ºÆÄÅ©", SKILLID_MultiSpark, "MP14 M_Spark" },
	{ "È¦¸®¸¶ÀÎµå", SKILLID_HolyMind, "MP17 HolyMind" },

	//Tier 2
	{ "¸ÞµðÅ×ÀÌ¼Ç", SKILLID_Meditation, "MP20 Meditation" },
	{ "µð¹ÙÀÎ¶óÀÌÆ®´×", SKILLID_DivineLightning, "MP23 D_Lightning" },
	{ "È¦¸® ¸®ÇÃ·º¼Ç", SKILLID_HolyReflection, "MP26 H_Reflection", "Pr2_H_REFLECTION.tga" },
	{ "±×·¹ÀÌÆ® Èú¸µ", SKILLID_GrandHealing, "MP30 G_Healing" },

	//Tier 3
	{ "ºñ°Å º¼", SKILLID_VigorBall, "MP40 V_Ball" },
	{ "¸®Àú·º¼Ç", SKILLID_Resurrection, "MP43 Resurrection" },
	{ "ÀÍ½ºÆÃ¼Ç", SKILLID_Extinction, "MP46 Extinction" },
	{ "¹öÃò¾ó ¶óÀÌÇÁ", SKILLID_VirtualLife, "MP50 V_Life", "Pr3_V_LIFE.tga" },

	//Tier 4
	{ "±Û·¡¼Å ½ºÆÄÀÌÅ©", SKILLID_GlacialSpike, "MP60 G_Spike" },
	{ "¸®Á¦³Ê·¹ÀÌ¼Ç ÇÊµå", SKILLID_RegenerationField, "MP63 R_Field", "Pr4_R_FIELD.tga" },
	{ "Ã¼ÀÎ ¶óÀÌÆ®´×", SKILLID_ChainLightning, "MP66 C_Lightning" },
	{ "¼­¸Õ ¹«½ºÆç", SKILLID_SummonMuspell, "MP70 S_Muspell", "Pr4_S_MUSPELL.tga" },

	//Tier 5
	{ "½ºÇÇ¸´ ÀÓÆÑÆ®", SKILLID_IceElemental, "MP100 Ice_Elemental" },
	{ "ÇÇ¾î½Ì ¾ÆÀÌ½º", SKILLID_IceMeteorite, "MP103 Ice_Meteorite" },
	{ "¼­¸Õ ¶ó¹Ì¿¤", SKILLID_HeavenlyLight, "MP106 Heavenly_Light" },
	{ "ºí·¹½Ì Å©¸®½´³ª", SKILLID_Consecration, "MP110 Consecration" },

	//Magician Skills - Tier 1
	{ "¾î°í´Ï", SKILLID_Agony, "MM10 Agony" },
	{ "ÆÄÀÌ¾îº¼Æ® ", SKILLID_FireBolt, "MM12 FireBolt" },
	{ "Á¦´Ï¾²", SKILLID_Zenith, "MM14 Zenith", "Mg1_ZENITH.tga" },
	{ "ÆÄÀÌ¾îº¼", SKILLID_FireBall, "MM17 FireBall" },

	//Tier 2
	{ "¸ÇÅ» ¸¶½ºÅÍ¸®", SKILLID_MentalMastery, "MM20 M_Mastery" },
	{ "ÄÝ·ë¿Àºê¿öÅÍ", SKILLID_Watornado, "MM23 Watornado" },
	{ "ÀÎÃ¦Æ® ¿þÆù", SKILLID_EnchantWeapon, "MM26 E_Weapon", "Mg2_E_WEAPON.tga" },
	{ "µ¥µå·¹ÀÌ", SKILLID_DeathRay, "MM30 D_Ray" },

	//Tier 3
	{ "¿¡³ÊÁö ½¯µå", SKILLID_EnergyShield, "MM40 E_Shield", "Mg3_E_SHIELD.tga" },
	{ "´ÙÀÌ¾Æ½ºÆ®·ÎÇÇÁò", SKILLID_Diastrophism, "MM43 Diastrophism" },
	{ "½ºÇÇ¸´ ¿¤·¹¸àÅ»", SKILLID_SpiritElemental, "MM46 S_Elemental", "Mg3_S_ELEMENTAL.tga" },
	{ "´í½Ì½º¿öµå", SKILLID_DancingSword, "MM50 D_Sword", "Mg3_D_SWORD.tga" },

	//Tier 4
	{ "ÆÄÀÌ¾î ¿¤·¹¸àÅ»", SKILLID_FireElemental, "MM60 F_Elemental", "Mg4_F_ELEMENTAL.tga" },
	{ "ÇÃ¶óÀÓ ¿þÀÌºê", SKILLID_FlameWave, "MM63 F_Wave" },
	{ "µð½ºÆ®·Ï¼Ç", SKILLID_Distortion, "MM66 Distortion", "Mg4_DISTORTION.tga" },
	{ "¸ÞÅ×¿À", SKILLID_Meteorite, "MM70 Meteo" },

	//Tier 5
	{ "ÇÁ¸®¸¶ ÀÌ±×´Ï½º", SKILLID_WizardTrance, "MM100 WizardTrance" },
	{ "½Ç¶óÇË", SKILLID_StoneSkin, "MM103 Stone_Skin" },
	{ "ºñ½º Å×´©½º", SKILLID_RedRay, "MM106 RedRay" },
	{ "½áµå ¾Æ´Ï¸¶", SKILLID_Cataclysm, "MM110 Cataclysm" },

	//Shaman Skills - Tier 1
	{ "´ÙÅ© º¼Æ®", SKILLID_DarkBolt, "MS10 Darkbolt" },
	{ "´ÙÅ© ¿þÀÌºê", SKILLID_DarkWave, "MS12 Darkwave" },
	{ "Ä¿½º ·¹ÀÌÁö", SKILLID_Inertia, "MS14 Curselazy" },
	{ "ÀÌ³Ê ÇÇ½º", SKILLID_InnerPeace, "MS17 I_peace" },

	//Tier 2
	{ "½ºÇÇ¸®Ãò¾ó ÇÃ·¹¾î", SKILLID_SpiritualFlare, "MS20 S_Flare" },
	{ "¼Ò¿ï ¸Å³ÊÅ¬", SKILLID_SpiritualManacle, "MS23 S_Manacle" },
	{ "Ã¼ÀÌ½Ì ÇåÆ®", SKILLID_ChasingHunt, "MS26 C_Hunt" },
	{ "¾îµåº¥Æ®:¹Ì°¥", SKILLID_AdventMigal, "MS30 A_Migal", "Sh2_A_MIGAL.tga" },

	//Tier 3
	{ "·¹ÀÎ¸ÞÀÌÄ¿", SKILLID_Rainmaker, "MS40 R_Maker", "Sh3_R_MAKER.tga" },
	{ "·£µå ¿Àºê °í½ºÆ®", SKILLID_PhantomCall, "MS43 L_Ghost" },
	{ "È¥Æ®", SKILLID_Haunt, "MS46 Haunt" },
	{ "½ºÅ©·¡Ä¡", SKILLID_Scratch, "MS50 Scratch" },

	//Tier 4
	{ "¸®ÄÝ ºí·¯µð³ªÀÌÆ®", SKILLID_RecallHestian, "MS60 R_Knight", "Sh4_R_KNIGHT.tga" },
	{ "ÀúÁö¸ÕÆ®", SKILLID_Judgement, "MS63 Judge" },
	{ "¾îµåº¥Æ®:¹Ìµå¶õ´Ù", SKILLID_AdventMidranda, "MS66 A_Midranda", "Sh4_A_MIDRANDA.tga" },
	{ "¸ð´× ¿Àºê ÇÁ·¹ÀÌ", SKILLID_MourningPray, "MS70 M_pray" },

	//Tier 5
	{ "Å©¸®µå", SKILLID_Creed, "MS80 Creed", "Sh5_CREED.tga" },
	{ "ÇÁ·¹½º ¿Àºê µ¥ÀÌ¾îÆ¼", SKILLID_PressDeity, "MS83 P_Deity" },
	{ "°í½ºÆ¼ ³×ÀÏ", SKILLID_PhantomNail, "MS86 G_Nail" },
	{ "ÇÏÀÌ ¸®Á¦³×·¹ÀÌ¼Ç", SKILLID_OccultLife, "MS90 H_Regene" },

	//Other Skills
	{ "Èí¼ö·Â »ó½Â", SKILLID_AbsorptionOfGlory, "", "wa1-1.tga" },
	{ "°ø°Ý·Â »ó½Â", SKILLID_PowerSiege, "", "wa1-3.tga" },
	{ "È¸ÇÇÀ² »ó½Â", SKILLID_EvasionAdaption, "", "wa1-2.tga" },
	{ "¾ÆÅ¸³ª½Ã¾Æ", SKILLID_Atanasia, "", "S_Ath.tga" },
	{ "µ¥µé¸® ¿¡Áö", SKILLID_DeadlyEdge, "", "S_Dea.tga" },
	{ "¾îº£ÀÏ ¿Àºê ÀÌº£ÀÌµå", SKILLID_AvailofEvasion, "", "S_Eva.tga" },
	{ "µðÆÄÀÌ¾ð½º ½ºÅæ", SKILLID_DefianceStoneA, "", "F_C_M.tga" },
	{ "µðÆÄÀÌ¾ð½º ½ºÅæ", SKILLID_DefianceStoneB, "", "I_C_M.tga" },
	{ "µðÆÄÀÌ¾ð½º ½ºÅæ", SKILLID_DefianceStoneC, "", "L_C_M.tga" },
	{ "¸¶ÀÌÆ® ½ºÅæ", SKILLID_FighterStone, "", "Ft_R.tga" },
	{ "¸¶ÀÌÆ® ½ºÅæ", SKILLID_MechanicianStone, "", "Mc_R.tga" },
	{ "¸¶ÀÌÆ® ½ºÅæ", SKILLID_PikemanStone, "", "Pk_R.tga" },
	{ "¸¶ÀÌÆ® ½ºÅæ", SKILLID_ArcherStone, "", "Ac_R.tga" },
	{ "¸¶ÀÌÆ® ½ºÅæ", SKILLID_KnightStone, "", "Kt_R.tga" },
	{ "¸¶ÀÌÆ® ½ºÅæ", SKILLID_AtalantaStone, "", "At_R.tga" },
	{ "¸¶ÀÌÆ® ½ºÅæ", SKILLID_PriestessStone, "", "Mg_R.tga" },
	{ "¸¶ÀÌÆ® ½ºÅæ", SKILLID_MagicianStone, "", "Pt_R.tga" },
	{ "¸¶ÀÌÆ® ½ºÅæ", SKILLID_ShamanStone, "", "As_R.tga" },
	{ "¸¶ÀÌÆ® ½ºÅæ", SKILLID_MightStone, "", "Sh_R.tga" },
	{ "ÀÌÅÍ³Î ¶óÀÌÇÁ", SKILLID_EternalLife, "", "S_Ath.tga" },
	{ "ÆäÀÌÆ² ¿¡Áö", SKILLID_FatalEdge, "", "S_Dea.tga" },
	{ "¾î¹öÆ® ½ºÅ©·Ñ", SKILLID_AvertScroll, "", "S_Eva.tga" },
	{ "¸®º£¶ó", SKILLID_Rivera, "", "S_Lib.tga" },
	{ "»ý¸í·Â ºÎ½ºÅÍ", 0x09010007, "", "Booster_01.tga" },
	{ "±â·Â ºÎ½ºÅÍ", 0x09010008, "", "Booster_02.tga" },
	{ "±Ù·Â ºÎ½ºÅÍ", 0x09010009, "", "Booster_03.tga" },
	{ "¸ÖÆ¼ ºÎ½ºÅÍ", 0x09010030, "", "Booster_04.tga" },
};

const SkillBase saSkillBase[MAX_SKILLS] =
{
	{ "Extreme Shield", "Temporarily increases your block rating when a shield is being used",
	50, 31, 1, 128, -1,
	{ 0, 0, 0 }, { ITEMTYPE_Shield, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00522DB0, SKILLID_ExtremeShield, SKILLTYPE_Right, 0x009926E8, 0 },

	{ "Mechanic Bomb", "Throws a bomb that causes an explosion damaging enemies in that area",
	12, 28, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00522F70, SKILLID_MechanicBomb, SKILLTYPE_All, 0x00992788, 0 },

	{ "Poison Attribute", "Permanently increases resistance to all poison property attacks",
	14, 0, 0, 0, 0,
	{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00523220, SKILLID_PoisonAttribute, SKILLTYPE_Passive, 0x00000000, 0 },

	{ "Physical Absorption", "Boosts your absorption rating for a period of time",
	50, 31, 1, 128, -1,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00523090, SKILLID_PhysicalAbsorb, SKILLTYPE_Right, 0x00992828, 0 },

	{ "Great Smash", "Delivers a powerful smash with some area damage",
	20, 38, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Axe, ITEMTYPE_Claw, ITEMTYPE_Hammer, ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00523240, SKILLID_GreatSmash, SKILLTYPE_All, 0x009928C8, 0 },

	{ "Maximize", "Increases your attack rating and attack power for a period of time",
	50, 31, 1, 128, -1,
	{ 0, 0, 0 }, { ITEMTYPE_Axe, ITEMTYPE_Claw, ITEMTYPE_Hammer, ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_Bow, ITEMTYPE_Javelin, ITEMTYPE_None },
	0x00523390, SKILLID_Maximize, SKILLTYPE_Right, 0x00992940, 0 },

	{ "Automation", "Increases your attack power and attack speed when using bows and javelins for a period of time",
	50, 31, 1, 128, -1,
	{ 0, 0, 0 }, { ITEMTYPE_Bow, ITEMTYPE_Javelin, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00523510, SKILLID_Automation, SKILLTYPE_Right, 0x009929E0, 0 },

	{ "Spark", "Creates a spark of pure energy and releases it to damage enemies",
	30, 40, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Axe, ITEMTYPE_Claw, ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_Hammer, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005236C0, SKILLID_Spark, SKILLTYPE_All, 0x00992A80, 0 },

	{ "Metal Armor", "Temporarily increases defense rating of your armor if it's mechanician spec.",
	50, 31, 1, 128, -1,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00524FB0, SKILLID_MetalArmor, SKILLTYPE_Right, 0x00992AF8, 0 },

	{ "Grand Smash", "Unleashes two devastating blows to a single target with increased attack power and attack rating",
	43, 44, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Axe, ITEMTYPE_Claw, ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_Hammer, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525000, SKILLID_GrandSmash, SKILLTYPE_All, 0x00992B70, 0 },

	{ "Mechanic Weapon Mastery", "Permanently increases base attack power when using a mechanician spec weapon",
	46, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525020, SKILLID_MechanicWeaponMastery, SKILLTYPE_Passive, 0x00000000, 0 },

	{ "Spark Shield", "Increases your defense rating and deal damage to monsters when your shield blocks a hit successfully, based on damage of your Spark skill",
	50, 31, 1, 128, -1,
	{ 0, 0, 0 }, { ITEMTYPE_Shield, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525030, SKILLID_SparkShield, SKILLTYPE_Right, 0x00992C38, 0 },

	{ "Impulsion", "Creates a powerful chain of sparks that causes eletric damage against multiple targets",
	60, 55, 2, 70, 3,
	{ 1, 0, 0 }, { ITEMTYPE_Claw, ITEMTYPE_Hammer, ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005252A0, SKILLID_Impulsion, SKILLTYPE_All, 0x00992D00, 0 },

	{ "Compulsion", "Creates a magnetic field that pulls enemies towards the player while temporarily boosting absorption",
	50, 31, 1, 128, -1,
	{ 1, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005252C0, SKILLID_Compulsion, SKILLTYPE_Right, 0x00992DA0, 0 },

	{ "Magnetic Sphere", "Generates three magnetic spheres that attack nearby enemies",
	50, 31, 1, 128, -1,
	{ 1, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525300, SKILLID_MagneticSphere, SKILLTYPE_Right, 0x00992E90, 0 },

	{ "Metal Golem", "Summons a huge metallic golem to your aid, attacking nearby enemies",
	70, 71, 2, 169, 0,
	{ 1, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525340, SKILLID_MetalGolem, SKILLTYPE_Right, 0x00992F80, 0 },

	{ "Rupture", "Attacks the Enemy with a great force",
	100, 71, 2, 180, 4,
	{ 2, 0, 0 },{ ITEMTYPE_Axe, ITEMTYPE_Claw, ITEMTYPE_Hammer, ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525670, SKILLID_Rupture, SKILLTYPE_All, 0x00993110, 0 },

	{ "Magnetic Discharge", "Releases a mechanical discharge of magnetics spheres damaging enemies around you",
	50, 31, 1, 128, -1,
	{ 2, 0, 0 }, { ITEMTYPE_Axe, ITEMTYPE_Claw, ITEMTYPE_Hammer, ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_Bow, ITEMTYPE_Javelin, ITEMTYPE_None },
	0x00525690, SKILLID_MagneticDischarge, SKILLTYPE_Right, 0x00993160, 0 },

	{ "Hardened Ward", "Shield strenght is hardened by reducing your attack rating",
	103, 90, 2, 203, 0,
	{ 2, 0, 0 }, { ITEMTYPE_Axe, ITEMTYPE_Claw, ITEMTYPE_Hammer, ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_Bow, ITEMTYPE_Javelin, ITEMTYPE_None },
	0x00525630, SKILLID_HardeningShield, SKILLTYPE_Right, 0x00993098, 0 },

	{ "Swarm Shot", "Shots an arrow filled with parasites",
	103, 65, 2, 93, 3,
	{ 2, 0, 0 }, { ITEMTYPE_Axe, ITEMTYPE_Claw, ITEMTYPE_Hammer, ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525610, SKILLID_ParasitShot, SKILLTYPE_Right, 0x00992FA8, 0 },

	{ "Melee Mastery", "Permanently increases base attack power when using melee weapons",
	10, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_Axe, ITEMTYPE_Claw, ITEMTYPE_Hammer, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005237E0, SKILLID_MeleeMastery, SKILLTYPE_Passive, 0x00000000, 0 },

	{ "Fire Attribute", "Permanently increases your resistance against fire attacks",
	12, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00523800, SKILLID_FireAttribute, SKILLTYPE_Passive, 0x00000000, 0 },

	{ "Raving", "Goes berserk and attacks a single target with increased damage, losing HP when attacking",
	14, 35, 2, 73, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Axe, ITEMTYPE_Claw, ITEMTYPE_Hammer, ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00523820, SKILLID_Raving, SKILLTYPE_All, 0x009952F8, 0 },

	{ "Impact", "Delivers two powerful swings on a single target",
	17, 37, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Axe, ITEMTYPE_Claw, ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005239C0, SKILLID_Impact, SKILLTYPE_All, 0x00995370, 0 },

	{ "Triple Impact", "Delivers devastating attack combos inflicting huge damage to the target",
	20, 39, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Axe, ITEMTYPE_Claw, ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00523B10, SKILLID_TripleImpact, SKILLTYPE_All, 0x009953E8, 0 },

	{ "Brutal Swing", "Unleashes a massive attack with improved critical chances",
	23, 41, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Axe, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00523C60, SKILLID_BrutalSwing, SKILLTYPE_All, 0x00995460, 0 },

	{ "Roar", "Lets out a deafening roar to momentarily stun enemies",
	26, 39, 2, 129, 4,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00523DB0, SKILLID_Roar, SKILLTYPE_Right, 0x009954D8, 0 },

	{ "Rage of Zecram", "Jumps into the air smashing the axe into the ground to unleash a powerful eruption of fire that inflicts area damage",
	30, 45, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Axe, ITEMTYPE_Hammer, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00523ED0, SKILLID_RageofZecram, SKILLTYPE_All, 0x00995528, 0 },

	{ "Concentration", "Temporarily increases base attack rating by concentrating intensly",
	50, 31, 1, 128, -1,
	{ 0, 0, 0 }, { ITEMTYPE_Axe, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525080, SKILLID_Concentration, SKILLTYPE_Right, 0x009955A0, 0 },

	{ "Avenging Crash", "Inflicts great damage with the use of two deadly uppercuts",
	43, 48, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Axe, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005250D0, SKILLID_AvengingCrash, SKILLTYPE_All, 0x00995618, 0 },

	{ "Swift Axe", "Greatly increases your attacking speed with axe weapons",
	50, 31, 1, 128, -1,
	{ 0, 0, 0 }, { ITEMTYPE_Axe, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005250F0, SKILLID_SwiftAxe, SKILLTYPE_Right, 0x00995690, 0 },

	{ "Bone Crash", "Jumps into the air and delivers a smashing blow",
	50, 54, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Axe, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525140, SKILLID_BoneCrash, SKILLTYPE_All, 0x00995708, 0 },

	{ "Destroyer", "Delivers a powerful combo with massive damage and increased critical rating",
	60, 45, 2, 180, 4,
	{ 1, 0, 0 }, { ITEMTYPE_Axe, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525390, SKILLID_Destroyer, SKILLTYPE_All, 0x00995780, 0 },

	{ "Berserker", "Decreases absorb rating for an increased base attack power",
	50, 31, 1, 128, -1,
	{ 1, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005253B0, SKILLID_Berserker, SKILLTYPE_Right, 0x00995820, 0 },

	{ "Cyclone Strike", "Rotates and creates a huge cyclone damaging surrounding targets",
	66, 55, 2, 70, 3,
	{ 1, 0, 0 }, { ITEMTYPE_Axe, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525400, SKILLID_CycloneStrike, SKILLTYPE_All, 0x00995910, 0 },

	{ "Boost Health", "Permanently raises your maximum heath capacity",
	70, 0, 0, 0, 0,
	{ 1, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525420, SKILLID_BoostHealth, SKILLTYPE_Passive, 0x00000000, 0 },

	{ "Survival Instinct", "Permanently raises your rejuvenation capabilities",
	100, 0, 0, 0, 0,
	{ 2, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005256F0, SKILLID_SurvivalInstinct, SKILLTYPE_Passive, 0, 0 },

	{ "Seismic Impact", "Deals a huge damage on the target that shakes the ground",
	103, 48, 2, 70, 3,
	{ 2, 0, 0 }, { ITEMTYPE_Axe, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005256D0, SKILLID_SeismicImpact, SKILLTYPE_Right, 0x00995960, 0 },

	{ "Might of Zecram", "Infuse your weapon with fire energy increasing damage after a number of hits. Reduces your HP with each hit",
	106, 70, 2, 203, 0,
	{ 2, 0, 0 }, { ITEMTYPE_Axe, ITEMTYPE_Hammer, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525710, SKILLID_RuthlessofZecram, SKILLTYPE_Right, 0x00995A78, 0 },

	{ "Raging Strikes", "Use your axe to rotate furiously damaging nearby targets",
	110, 93, 2, 200, 0,
	{ 2, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525730, SKILLID_WhirlwindFS01, SKILLTYPE_Right, 0x00995B68, 0 },

	{ "Pike Wind", "Creates a whirwind with the use of a scythe, pushing the enemies away",
	10, 27, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Scythe, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00524020, SKILLID_PikeWind, SKILLTYPE_Right, 0x00997558, 0 },

	{ "Ice Attribute", "Permanently increases your resistance against ice attacks",
	12, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00524160, SKILLID_IceAttribute, SKILLTYPE_Passive, 0x00000000, 0 },

	{ "Critical Hit", "Hits the enemy twice on their weak points causing increased critical damage",
	14, 31, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Scythe, ITEMTYPE_Hammer, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00524180, SKILLID_CriticalHit, SKILLTYPE_All, 0x009975D0, 0 },

	{ "Jumping Crash", "Leaps up into the air to deliver a blow with massive power and accuracy",
	17, 34, 2, 112, 4,
	{ 0, 0, 0 }, { ITEMTYPE_Scythe, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005242A0, SKILLID_JumpingCrash, SKILLTYPE_All, 0x009975F8, 0 },

	{ "Ground Pike", "Thrusts spear into the ground freezing and slowing down enemies within range",
	20, 36, 2, 115, 4,
	{ 0, 0, 0 }, { ITEMTYPE_Scythe, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005243F0, SKILLID_GroundPike, SKILLTYPE_Right, 0x009976E8, 0 },

	{ "Tornado", "Brandishes scythe and creates a tornado, dealing serious damage to enemies within range",
	23, 38, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Scythe, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00524530, SKILLID_Tornado, SKILLTYPE_All, 0x009977B0, 0 },

	{ "Weapon Block Mastery", "Permanently increases block rating when using two-handed scythes",
	26, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_Hammer, ITEMTYPE_Scythe, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00524670, SKILLID_WeaponDefenseMastery, SKILLTYPE_Passive, 0x00000000, 0 },

	{ "Expansion", "Increases size of scythe and unleashes a devastating attack to strike the enemy",
	30, 42, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Scythe, ITEMTYPE_Hammer, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00524690, SKILLID_Expansion, SKILLTYPE_All, 0x00997850, 0 },

	{ "Venom Spear", "Thrusts spear into the ground causing poisoned spears to consume surrounding enemies",
	40, 50, 2, 124, 0,
	{ 0, 0, 0 }, { ITEMTYPE_Scythe, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525160, SKILLID_VenomSpear, SKILLTYPE_Right, 0x00997940, 0 },

	{ "Vanish", "Vanishes into thin air, getting invisible to enemies eyes and increasing final damage to first hit",
	43, 53, 2, 203, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525180, SKILLID_Vanish, SKILLTYPE_Right, 0x00997A08, 0 },

	{ "Critical Mastery", "Permanently increases chance to inflict critical damage when using scythes",
	46, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_Scythe, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005251C0, SKILLID_CriticalMastery, SKILLTYPE_Passive, 0x00000000, 0 },

	{ "Chain Lancer", "Jumps into the air slashing a single opponent with a fast and devastating three hits combo",
	50, 51, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Scythe, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005251D0, SKILLID_ChainLance, SKILLTYPE_All, 0x00997A80, 0 },

	{ "Assassin's Eye", "Curses the target, increasing your critical damage against it temporarily",
	60, 61, 2, 135, 4,
	{ 1, 0, 0 }, { ITEMTYPE_Scythe, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525430, SKILLID_AssassinsEye, SKILLTYPE_All, 0x00997B20, 0 },

	{ "Charging Strike", "Charges at an enemy delivering three powerful blows on a single target",
	63, 75, 2, 180, 4,
	{ 1, 0, 0 }, { ITEMTYPE_Scythe, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525450, SKILLID_ChargingStrike, SKILLTYPE_All, 0x00997BC0, 0 },

	{ "Vague", "Increases evasion rating when using two handed scythes temporarily",
	50, 31, 1, 128, -1,
	{ 1, 0, 0 }, { ITEMTYPE_Scythe, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525470, SKILLID_Vague, SKILLTYPE_Right, 0x00997C38, 0 },

	{ "Shadow Master", "Strikes a single target with a barrage of powerful devastating piercing attacks",
	70, 51, 2, 180, 4,
	{ 1, 0, 0 }, { ITEMTYPE_Scythe, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005254B0, SKILLID_ShadowMaster, SKILLTYPE_All, 0x00997CD8, 0 },

	{ "Ring of Spears", "Surround the target with spears stunning it with fear",
	100, 75, 2, 180, 0,
	{ 2, 0, 0 }, { ITEMTYPE_Claw, ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525570, SKILLID_RingofSpears, SKILLTYPE_Right, 0x00997D00, 0 },

	{ "Last Breath", "Jumps into target and deal damage",
	103, 70, 2, 203, 0,
	{ 2, 0, 0 }, { ITEMTYPE_Scythe, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525590, SKILLID_LastBreath, SKILLTYPE_Right, 0x00997D50, 0 },

	{ "Ex-Death", "Becomes the master of death by involving your weapon with the power of the shadows",
	106, 93, 2, 180, 4,
	{ 2, 0, 0 }, { ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005255B0, SKILLID_DeathMaster, SKILLTYPE_Right, 0x00997E18, 0 },

	{ "Sinister Spin", "Spins vigorously to create a deadly twister damaging nearby enemies",
	110, 81, 2, 100, 0,
	{ 2, 0, 0 }, { ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005255F0, SKILLID_Twister, SKILLTYPE_Right, 0x00997EE0, 0 },

	{ "Scout Hawk", "Summons a Hawk to scout the surrounding area raising your base attack rating temporarily",
	50, 31, 1, 128, -1,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005247E0, SKILLID_ScoutHawk, SKILLTYPE_Right, 0x00993C00, 0 },

	{ "Shooting Mastery", "Permanently raises your base attack power when using bow-type weapons",
	12, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_Bow, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00524920, SKILLID_ShootingMastery, SKILLTYPE_Passive, 0x00000000, 0 },

	{ "Wind Arrow", "Fires a wind arrow causing additional damage to normal monsters",
	14, 25, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Bow, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00524940, SKILLID_WindArrow, SKILLTYPE_All, 0x00993CA0, 0 },

	{ "Perfect Aim", "Fires an enhanced arrow that cause additional damage to demon monsters",
	17, 29, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Bow, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00524A80, SKILLID_PerfectAim, SKILLTYPE_All, 0x00993D40, 0 },

	{ "Dion's Eye", "Increases your attack rating permanently, based on your weapon",
	20, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_Bow, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00524BC0, SKILLID_DionsEye, SKILLTYPE_Passive, 0x00000000, 0 },

	{ "Falcon", "Summons a fierce falcon to your aid attacking all enemies that you hit with your weapon",
	50, 31, 1, 128, -1,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00524BE0, SKILLID_Falcon, SKILLTYPE_Right, 0x00993E08, 0 },

	{ "Arrow of Rage", "Launches a handful of arrows in midair that causes splash damage to certain targets",
	26, 38, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Bow, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00524D30, SKILLID_ArrowofRage, SKILLTYPE_All, 0x00993EA8, 0 },

	{ "Avalanche", "Shoots a series of multiple arrows to a single target causing massive damage",
	30, 41, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Bow, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00524E70, SKILLID_Avalanche, SKILLTYPE_All, 0x00993F48, 0 },

	{ "Elemental Shot", "Fires a shot of fire or lightning arrow causing damage",
	40, 43, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Bow, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005251F0, SKILLID_ElementalShot, SKILLTYPE_All, 0x00994060, 0 },

	{ "Golden Falcon", "Summons a falcon to help you by attacking and healing",
	50, 31, 1, 128, -1,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525210, SKILLID_GoldenFalcon, SKILLTYPE_Right, 0x00994100, 0 },

	{ "Bomb Shot", "Fires two explosive arrows causing splash damage within the blast radius",
	46, 46, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Bow, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525260, SKILLID_BombShot, SKILLTYPE_All, 0x009941F0, 0 },

	{ "Perforation", "Fires an arrow that pierces through the hardest obstacles and does splash damage",
	50, 48, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Bow, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525280, SKILLID_Perforation, SKILLTYPE_All, 0x00994268, 0 },

	{ "Wolverine", "Summons the fierce beast Wolverine to aid attack your enemies",
	60, 58, 2, 169, 0,
	{ 1, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005254D0, SKILLID_RecallWolverine, SKILLTYPE_Right, 0x00994380, 0 },

	{ "Evasion Mastery", "Permanently increases your evasion rating when using a two-handed bow to improve capability of dodging attacks",
	63, 0, 0, 0, 0,
	{ 1, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525520, SKILLID_EvasionMastery, SKILLTYPE_Passive, 0x00000000, 0 },

	{ "Phoenix Shot", "Fires an arrow enchanted with the flames of Phoenix",
	66, 43, 2, 70, 3,
	{ 1, 0, 0 }, { ITEMTYPE_Bow, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525530, SKILLID_PhoenixShot, SKILLTYPE_All, 0x00994420, 0 },

	{ "Force of Nature", "Amplifies and improves your offensive abilities with the use of nature's energy",
	50, 31, 1, 128, -1,
	{ 1, 0, 0 }, { ITEMTYPE_Bow, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525550, SKILLID_ForceofNature, SKILLTYPE_Right, 0x00994510, 0 },

	{ "Lethal Sight", "Focus your aim on a target at a far distance, making the next attack fatal",
	100, 52, 2, 203, 0,
	{ 2, 0, 0 }, { ITEMTYPE_Bow, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005257A0, SKILLID_LethalSight, SKILLTYPE_Right, 0x009945D8, 0 },

	{ "Mistral's Arrow", "Harness the power of wind to create an arrow that penetrates enemies and deals a heavy damage",
	103, 52, 2, 82, 2,
	{ 2, 0, 0 }, { ITEMTYPE_Bow, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525780, SKILLID_FierceWind, SKILLTYPE_Right, 0x00994538, 0 },

	{ "Blooming Lotus", "Roots the enemy on the floor with the Nature's Power",
	106, 57, 2, 87, 3,
	{ 2, 0, 0 }, { ITEMTYPE_Bow, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005257C0, SKILLID_EntalingRoots, SKILLTYPE_Right, 0x00994678, 0 },

	{ "Saggitarius Rain", "Bombards the enemy with a strong blast of continuous arrows",
	110, 95, 2, 100, 0,
	{ 2, 0, 0 }, { ITEMTYPE_Bow, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005257E0, SKILLID_Bombardment, SKILLTYPE_Right, 0x009946F0, 0 },

	{ "Stinger", "Runs straight towards the enemy and hits him with two quick attacks",
	10, 22, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Dagger, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525800, SKILLID_Stinger, SKILLTYPE_All, 0x00997F58, 0 },

	{ "Running Hit", "Deals continuous damage to the enemy while spinning",
	12, 27, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Dagger, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525820, SKILLID_RunningHit, SKILLTYPE_All, 0x00997FD0, 0 },

	{ "Dual Wield Mastery", "Increase permanently your base attack power when using daggers",
	14, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_Dagger, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525840, SKILLID_DualWieldMastery, SKILLTYPE_Passive, 0x00000000, 0 },

	{ "Wisp", "Reduces targeted enemy damage for a period of time",
	17, 37, 2, 159, 0,
	{ 0, 0, 0 }, { ITEMTYPE_Dagger, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525850, SKILLID_Wisp, SKILLTYPE_All, 0x00998048, 0 },

	{ "Venom Thorn", "Distributes dexterity of an Assassin increasing evasion rating to all party members",
	20, 38, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Dagger, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525870, SKILLID_VenomThorn, SKILLTYPE_All, 0x009980C0, 0 },

	{ "Alas", "Increases evasion rating of party for a certain duration",
	50, 31, 1, 128, -1,
	{ 0, 0, 0 }, { ITEMTYPE_Dagger, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525890, SKILLID_Alas, SKILLTYPE_Right, 0x00998110, 0 },

	{ "Soul Shock", "Performs a hard hit to the ground with daggers and stun all enemies around",
	26, 39, 2, 129, 4,
	{ 0, 0, 0 }, { ITEMTYPE_Dagger, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005258D0, SKILLID_SoulShock, SKILLTYPE_Right, 0x00998188, 0 },

	{ "Attack Mastery", "Increases your base attack rating and block rating permanently when using daggers",
	30, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005258F0, SKILLID_AttackMastery, SKILLTYPE_Passive, 0x00000000, 0 },

	{ "Sore Blade", "Using your agility performs a continous, sharp and fast attack to the enemy",
	40, 45, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Dagger, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525900, SKILLID_SoreBlade, SKILLTYPE_All, 0x00998250, 0 },

	{ "Gust Slash", "Jumps and performs a fast spinning attack, dealing massive damage to nearby enemies",
	43, 48, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Dagger, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525920, SKILLID_GustSlash, SKILLTYPE_All, 0x009982A0, 0 },

	{ "Inpes", "Increases your attack speed temporarily",
	50, 31, 1, 128, -1,
	{ 0, 0, 0 }, { ITEMTYPE_Dagger, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525940, SKILLID_Inpes, SKILLTYPE_Right, 0x009982F0, 0 },

	{ "Blind", "Hides into your own shadow, turning invisible to enemies eyes for a short duration until you attack",
	50, 53, 2, 203, 0,
	{ 0, 0, 0 }, { ITEMTYPE_Dagger, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525980, SKILLID_Blind, SKILLTYPE_Right, 0x00998368, 0 },

	{ "Frost Wind", "Moves quickly, causing a fatal damage to the enemy",
	60, 53, 2, 140, 2,
	{ 1, 0, 0 }, { ITEMTYPE_Dagger, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005259C0, SKILLID_FrostWind, SKILLTYPE_All, 0x00998408, 0 },

	{ "Fatal Mastery", "Increases permanently chance to inflict critical damage when using daggers",
	63, 0, 0, 0, 0,
	{ 1, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005259E0, SKILLID_FatalMastery, SKILLTYPE_Passive, 0x00000000, 0 },

	{ "Polluted", "Poison all nearby enemies and decrease their health for a period of time",
	66, 55, 2, 124, 0,
	{ 1, 0, 0 }, { ITEMTYPE_Dagger, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005259F0, SKILLID_Polluted, SKILLTYPE_Right, 0x00998480, 0 },

	{ "Ninja Shadow", "Moves fast to deal two powerful hits to the enemy with shadows",
	70, 51, 2, 180, 4,
	{ 1, 0, 0 }, { ITEMTYPE_Dagger, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525A10, SKILLID_NinjaShadow, SKILLTYPE_All, 0x00998570, 0 },

	{ "Shunpo", "Swiftly teleports onto your enemies",
	100, 65, 2, 180, 4,
	{ 2, 0, 0 }, { ITEMTYPE_Dagger, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525A30, SKILLID_ShadowBlink, SKILLTYPE_Right, 0x009985C0, 0 },

	{ "Kiss of the Viper", "Catalyze the power and add critical damage boost",
	103, 71, 2, 135, 0,
	{ 2, 0, 0 }, { ITEMTYPE_Dagger, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525A50, SKILLID_KissViper, SKILLTYPE_Right, 0x009986B0, 0 },

	{ "Shade Kick", "Violently kicks your enemies dealing damage",
	106, 90, 2, 70, 3,
	{ 2, 0, 0 }, { ITEMTYPE_Dagger, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525A70, SKILLID_ViolentStub, SKILLTYPE_Right, 0x00998700, 0 },

	{ "Shadow Storm", "Engulfs your enemies into a powerfull shadow storm dealing damage",
	110, 95, 2, 160, 0,
	{ 2, 0, 0 }, { ITEMTYPE_Dagger, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00525A90, SKILLID_ShadowStorm, SKILLTYPE_Right, 0x00998750, 0 },

	{ "Sword Blast", "Charges sword with divine energy, and blasts it away until a certain distance",
	10, 22, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521320, SKILLID_SwordBlast, SKILLTYPE_All, 0x00993200, 0 },

	{ "Holy Body", "Gathers holy energy to strengthen the knight against undead monsters temporarily",
	50, 31, 1, 128, -1,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521340, SKILLID_HolyBody, SKILLTYPE_Right, 0x009932A0, 0 },

	{ "Physical Training", "Physically trains body to permanently increase stamina points",
	14, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521380, SKILLID_PhysicalTraining, SKILLTYPE_Passive, 0x00000000, 0 },

	{ "Double Crash", "Performs a two hit crashing attack with increased critical chance",
	17, 35, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Axe, ITEMTYPE_Claw, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521390, SKILLID_DoubleCrash, SKILLTYPE_All, 0x00993340, 0 },

	{ "Holy Valor", "Increases party damage against undead enemies temporarily",
	50, 31, 1, 128, -1,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521620, SKILLID_HolyValor, SKILLTYPE_Right, 0x00993408, 0 },

	{ "Brandish", "Brandishes the weapon causing area damage against multiple targets in front of player",
	23, 38, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521660, SKILLID_Brandish, SKILLTYPE_All, 0x00993480, 0 },

	{ "Piercing", "Pierces a sword through the targeted enemy with a powerful attack",
	26, 41, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521680, SKILLID_Piercing, SKILLTYPE_All, 0x009934F8, 0 },

	{ "Drastic Spirit", "Replenishes the knight's spirit to increase his defense for a period of time",
	50, 31, 1, 128, -1,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005216A0, SKILLID_DrasticSpirit, SKILLTYPE_Right, 0x00993570, 0 },

	{ "Sword Mastery", "Increases base attack power when using swords permanently",
	40, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521890, SKILLID_SwordMastery, SKILLTYPE_Passive, 0x00000000, 0 },

	{ "Divine Shield", "Boosts your block rating and convert dealt undead damage into health points while using shield",
	50, 31, 1, 128, -1,
	{ 0, 0, 0 }, { ITEMTYPE_Shield, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005218A0, SKILLID_DivineShield, SKILLTYPE_Right, 0x00993638, 0 },

	{ "Holy Incantation", "Transforms the targeted monster into a pet that will fight for you in battle for a period of time",
	46, 50, 2, 128, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005218F0, SKILLID_HolyIncantation, SKILLTYPE_All, 0x009936D8, 0 },

	{ "Grand Cross", "Attacks the enemy with a deadly cross, causing extra damage against undead enemies",
	50, 45, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521910, SKILLID_GrandCross, SKILLTYPE_All, 0x00993778, 0 },

	{ "Sword of Justice", "Smash the ground with the sword to create a wave that damages all enemies within range",
	60, 45, 2, 70, 3,
	{ 1, 0, 0 }, { ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521BD0, SKILLID_SwordofJustice, SKILLTYPE_All, 0x00993980, 12 },

	{ "Godly Shield", "Forms a Holy Shield blessed by God, increasing absorption rating while a shield is being used",
	50, 31, 1, 128, -1,
	{ 1, 0, 0 }, { ITEMTYPE_Shield, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521B50, SKILLID_GodlyShield, SKILLTYPE_All, 0x00993890, 0 },

	{ "God's Blessing", "Temporarily increases your base attack power with the divine blessings of God",
	50, 31, 1, 128, -1,
	{ 1, 0, 0 }, { ITEMTYPE_Axe, ITEMTYPE_Claw, ITEMTYPE_Hammer, ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_Bow, ITEMTYPE_Javelin, ITEMTYPE_None },
	0x00521B90, SKILLID_GodsBlessing, SKILLTYPE_Right, 0x00993908, 0 },

	{ "Divine Piercing", "With the help of God's power, attacks the enemy unleashing a devastating strike with increased critical chance",
	70, 51, 2, 180, 4,
	{ 1, 0, 0 }, { ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521B30, SKILLID_DivinePiercing, SKILLTYPE_All, 0x00993818, 15 },

	{ "Holy Conviction", "Gleams sacred belief to strengthen allies souls",
	50, 31, 1, 128, -1,
	{ 2, 0, 0 }, { ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521DE0, SKILLID_HolyConviction, SKILLTYPE_Right, 0x009939A8, 0 },

	{ "Whirl Wind", "Use your sword to rotate furiously damaging nearby targets",
	103, 70, 2, 169, 0,
	{ 2, 0, 0 }, { ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521E00, SKILLID_WhirlWind, SKILLTYPE_Right, 0x00993A48, 0 },

	{ "Glorious shield", "Forms the final shield to protect Knight",
	106, 75, 2, 203, 0,
	{ 2, 0, 0 }, { ITEMTYPE_Sword, ITEMTYPE_Hammer, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521E20, SKILLID_GloriousShield, SKILLTYPE_Right, 0x00993AC0, 0 },

	{ "Divine Cross", "Perform the final cross with divine power",
	110, 93, 2, 180, 4,
	{ 2, 0, 0 }, { ITEMTYPE_Axe, ITEMTYPE_Claw, ITEMTYPE_Hammer, ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_Bow, ITEMTYPE_Javelin, ITEMTYPE_None },
	0x00521E40, SKILLID_DivineCross, SKILLTYPE_Right, 0x00993B38, 0 },

	{ "Shield Strike", "Smites down and temporarily stuns enemies in linear range by smashing them with a shield",
	10, 23, 2, 152, -1,
	{ 0, 0, 0 }, { ITEMTYPE_Shield, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005213B0, SKILLID_ShieldStrike, SKILLTYPE_All, 0x009947B8, 0 },

	{ "Farina", "Throws a javelin thrusting at a target's body with increased attack power and accuracy",
	12, 26, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Javelin, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005213D0, SKILLID_Farina, SKILLTYPE_All, 0x00994858, 0 },

	{ "Throwing Mastery", "Increases base attack power permanently when using Javelins",
	14, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_Javelin, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005213F0, SKILLID_ThrowingMaster, SKILLTYPE_Passive, 0x00000000, 0 },

	{ "Bigger Spear", "Strikes down an enemy with immensely enhanced force and thrusting power",
	17, 32, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Scythe, ITEMTYPE_Javelin, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521400, SKILLID_BiggerSpear, SKILLTYPE_All, 0x009948F8, 0 },

	{ "Windy", "Temporarily increases base attack rating and attack range while using Javelin weapons",
	50, 31, 1, 128, -1,
	{ 0, 0, 0 }, { ITEMTYPE_Javelin, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005216F0, SKILLID_Windy, SKILLTYPE_Right, 0x009949C0, 0 },

	{ "Twist Javelin", "Throws a Javelin with increases attack power and rating, causing extra damage against demon monsters",
	23, 36, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Javelin, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521730, SKILLID_TwistJavelin, SKILLTYPE_All, 0x00994A38, 0 },

	{ "Soul Sucker", "Absorbs the HP of monster and add it the casters's health points, but only when monster health points is higher than 50%",
	26, 42, 2, 180, 4,
	{ 0, 0, 0 }, { ITEMTYPE_Shield, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521750, SKILLID_SoulSucker, SKILLTYPE_All, 0x00994A88, 0 },

	{ "Fire Javelin", "Throws a Javelin with the properties of fire causing extra damage against mutant monsters",
	30, 40, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Javelin, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521770, SKILLID_FireJavelin, SKILLTYPE_All, 0x00994B00, 0 },

	{ "Split Javelin", "Throws a Javelin that separates in mid-air and falls down damaging a single target",
	40, 43, 2, 180, 4,
	{ 0, 0, 0 }, { ITEMTYPE_Javelin, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521930, SKILLID_SplitJavelin, SKILLTYPE_All, 0x00994BA0, 0 },

	{ "Triumph of Valhalla", "Temporarily increases maximum attack power",
	50, 31, 1, 128, -1,
	{ 0, 0, 0 }, { ITEMTYPE_Javelin, ITEMTYPE_Sword, ITEMTYPE_Bow, ITEMTYPE_Scythe, ITEMTYPE_Hammer, ITEMTYPE_Axe, ITEMTYPE_Claw, ITEMTYPE_Wand },
	0x00521950, SKILLID_TriumphofJavelin, SKILLTYPE_Right, 0x00994C18, 0 },

	{ "Lightning Javelin", "Throws a javelin with the properties of lightning causing extra damage against undead monsters",
	46, 46, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Javelin, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521980, SKILLID_LightningJavelin, SKILLTYPE_All, 0x00994C90, 0 },

	{ "Storm Javelin", "Throws two javelins with massive strength, damaging all within its path",
	50, 48, 2, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Javelin, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005219A0, SKILLID_StormJavelin, SKILLTYPE_All, 0x00994CE0, 0 },

	{ "Hall of Valhalla", "Temporarily raises evasion rating, automatically activating properties of Triumph of Valhalla to all friends in range",
	50, 31, 1, 128, -1,
	{ 1, 0, 0 }, { ITEMTYPE_Axe, ITEMTYPE_Claw, ITEMTYPE_Hammer, ITEMTYPE_Scythe, ITEMTYPE_Bow, ITEMTYPE_Javelin, ITEMTYPE_None, ITEMTYPE_None },
	0x00521BF0, SKILLID_HallofValhalla, SKILLTYPE_Right, 0x00994D58, 0 },

	{ "Extreme Rage", "Leaps into the air throwing three explosive Javelins, damaging all enemies within the area",
	63, 48, 2, 70, 3,
	{ 1, 0, 0 }, { ITEMTYPE_Javelin, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521C10, SKILLID_ExtremeRage, SKILLTYPE_All, 0x00994DD0, 0 },

	{ "Frost Javelin", "Temporarily enchants the Javelin with the properties of ice, reducing enemy speed when hit",
	50, 31, 1, 128, -1,
	{ 1, 0, 0 }, { ITEMTYPE_Javelin, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521C30, SKILLID_FrostJavelin, SKILLTYPE_Right, 0x00994EC0, 0 },

	{ "Vengeance", "Throws two javelins with massive strength at a single target",
	70, 45, 2, 70, 3,
	{ 1, 0, 0 }, { ITEMTYPE_Javelin, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521C70, SKILLID_Vengeance, SKILLTYPE_All, 0x00994F60, 0 },

	{ "Nature's Call", "Summons the predator of the jungle to your aid",
	100, 95, 2, 169, 0,
	{ 2, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00522000, SKILLID_SummonTiger, SKILLTYPE_Right, 0x00995168, 0 },

	{ "Golden Apple", "Eats a sacred apple that boosts your speed and evade",
	103, 52, 2, 0, 0,
	{ 2, 0, 0 }, { ITEMTYPE_Javelin, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521C70, SKILLID_GoldenApple, SKILLTYPE_Passive, 0x00995000, 0 },

	{ "Plague Javelin", "Infuses your weapong with a distinct jungle poison",
	106, 68, 2, 100, 4,
	{ 2, 0, 0 }, { ITEMTYPE_Axe, ITEMTYPE_Claw, ITEMTYPE_Hammer, ITEMTYPE_Scythe, ITEMTYPE_Bow, ITEMTYPE_Javelin, ITEMTYPE_None, ITEMTYPE_None },
	0x00521FA0, SKILLID_PlagueJavelin, SKILLTYPE_Right, 0x00994F88, 0 },

	{ "Triple Threat", "Launcher a serious attack with increased power to dealing continuous damage",
	110, 57, 2, 180, 4,
	{ 2, 0, 0 }, { ITEMTYPE_Javelin, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521C70, SKILLID_ComboJavelin, SKILLTYPE_Right, 0x00995190, 0 },

	{ "Healing", "Restore lost health points to a single target, based on your attack power",
	10, 15, 1, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521420, SKILLID_Healing, SKILLTYPE_All, 0x009968D8, 0 },

	{ "Holy Bolt", "Shoots a bolt made of divine energy to disintegrate a single target",
	12, 11, 1, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521440, SKILLID_HolyBolt, SKILLTYPE_All, 0x00996950, 0 },

	{ "Multi Spark", "Disperses multiple sparks which stacks and hits a single target",
	14, 14, 1, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521460, SKILLID_MultiSpark, SKILLTYPE_All, 0x009969C8, 0 },

	{ "Holy Mind", "Casts a spell on target, reducing its damage against you for a period of time",
	17, 19, 1, 159, 0,
	{ 0, 0, 0 }, { ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521480, SKILLID_HolyMind, SKILLTYPE_All, 0x00996A68, 0 },

	{ "Meditation", "Increases MP regen and boosts MP potion recovery permanently",
	20, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521790, SKILLID_Meditation, SKILLTYPE_Passive, 0x00000000, 0 },

	{ "Divine Lightning", "Summons lightning bolts from the clouds to strike enemies below",
	23, 21, 1, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005217A0, SKILLID_DivineLightning, SKILLTYPE_Right, 0x00996B80, 0 },

	{ "Holy Reflection", "Creates a holy shield that reflects attacks from Undead Monsters",
	50, 31, 1, 128, -1,
	{ 0, 0, 0 }, { ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005217C0, SKILLID_HolyReflection, SKILLTYPE_Right, 0x00996BF8, 0 },

	{ "Grand Healing", "Restore health to multiple allies at once with the same group party at range, based on your attack power",
	30, 27, 1, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521800, SKILLID_GrandHealing, SKILLTYPE_Right, 0x00996C98, 0 },

	{ "Vigor Ball", "Launches two glowing spheres of powerful energy against a single target with increased damage and attack rating",
	40, 24, 1, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005219C0, SKILLID_VigorBall, SKILLTYPE_All, 0x00996D10, 0 },

	{ "Resurrection", "With holy magic, revives dead players near you, not preventing them to lose gold or EXP",
	43, 31, 1, 115, 4,
	{ 0, 0, 0 }, { ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005219E0, SKILLID_Resurrection, SKILLTYPE_Right, 0x00996D88, 0 },

	{ "Extinction", "Cast a spell to quickly reduce the health of undead monsters",
	46, 28, 1, 180, 4,
	{ 0, 0, 0 }, { ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521A00, SKILLID_Extinction, SKILLTYPE_Right, 0x00996E00, 0 },

	{ "Virtual Life", "Increases health points of the targeted friend for a period of time",
	50, 31, 1, 128, -1,
	{ 0, 0, 0 }, { ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521A20, SKILLID_VirtualLife, SKILLTYPE_All, 0x00996E78, 0 },

	{ "Glacial Spike", "Projects a sharp ice spikes on the ground damaging and freezing all enemies within range",
	60, 35, 1, 124, 0,
	{ 1, 0, 0 }, { ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521C90, SKILLID_GlacialSpike, SKILLTYPE_All, 0x00996F18, 0 },

	{ "Regeneration Field", "Creates a Regeneration Field for a period of time that HP and MP regen of all friends in range",
	63, 41, 1, 203, 0,
	{ 1, 0, 0 }, { ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521CB0, SKILLID_RegenerationField, SKILLTYPE_Right, 0x00996FE0, 0 },

	{ "Chain Lightning", "Creates a chain of lightning bolts that can hit multiple enemies at once",
	66, 41, 1, 140, 2,
	{ 1, 0, 0 }, { ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521CD0, SKILLID_ChainLightning, SKILLTYPE_All, 0x009970A8, 0 },

	{ "Summon Muspell", "Summons a Muspell who enhances priestess defensive and offensive abilities and helps by attacking nearby enemies",
	50, 31, 1, 128, -1,
	{ 1, 0, 0 }, { ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521CF0, SKILLID_SummonMuspell, SKILLTYPE_Right, 0x009971C0, 0 },

	{ "Ice Elemental", "Summons an Ice Elemental to help you in battle",
	100, 45, 2, 169, 0,
	{ 2, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521F00, SKILLID_IceElemental, SKILLTYPE_Right, 0x009971E8, 0 },

	{ "Ice Meteorite", "Calls in an ice cold meteorite storm which will damage and freeze opponents near the strike area of each meteorite",
	103, 33, 1, 70, 3,
	{ 2, 0, 0 }, { ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521F20, SKILLID_IceMeteorite, SKILLTYPE_Right, 0x00997288, 0 },

	{ "Heavenly Light", "Summons a divine light to purify allies and blind enemies",
	106, 54, 2, 203, 0,
	{ 2, 0, 0 }, { ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521F40, SKILLID_HeavenlyLight, SKILLTYPE_Right, 0x00997300, 0 },

	{ "Consecration", "Blesses party members with God's protection",
	110, 60, 2, 203, 0,
	{ 2, 0, 0 }, { ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521F70, SKILLID_Consecration, SKILLTYPE_Right, 0x00997418, 0 },

	{ "Agony", "Sacrifices your own health points to be converted to mana points",
	10, 12, 1, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005214A0, SKILLID_Agony, SKILLTYPE_Right, 0x00995C08, 0 },

	{ "Fire Bolt", "Fires a small flaming bolt of hot lava to a single target",
	12, 11, 1, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521540, SKILLID_FireBolt, SKILLTYPE_All, 0x00995C58, 0 },

	{ "Zenith", "Temporarily raises your resistance to all type of elemental attacks or spells",
	50, 31, 1, 128, -1,
	{ 0, 0, 0 }, { ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521560, SKILLID_Zenith, SKILLTYPE_Right, 0x00995CD0, 0 },

	{ "Fire Ball", "Creates a giant ball of flaming hot rocks to attack targeted enemies",
	17, 19, 1, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521600, SKILLID_FireBall, SKILLTYPE_All, 0x00995D48, 0 },

	{ "Mental Mastery", "Increases your mana capacity and MP potion recovery permanently",
	20, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521820, SKILLID_MentalMastery, SKILLTYPE_Passive, 0x00000000, 0 },

	{ "Watornado", "Creates a water tornado, causing a devastating damage to all enemies in range",
	23, 21, 1, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521830, SKILLID_Watornado, SKILLTYPE_All, 0x00995ED8, 0 },

	{ "Enchant Weapon", "Temporarily increases your attack power with different elemental properties",
	26, 28, 1, 128, -1,
	{ 0, 0, 0 }, { ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521850, SKILLID_EnchantWeapon, SKILLTYPE_Right, 0x00996018, 0 },

	{ "Death Ray", "Fires a ray of pure energy to disintegrate a single target",
	30, 26, 1, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521870, SKILLID_DeathRay, SKILLTYPE_All, 0x009960B8, 0 },

	{ "Energy Shield", "Generates a shield of energy, reducing damage by absorbing a portion of damage to mana points",
	50, 31, 1, 128, -1,
	{ 0, 0, 0 }, { ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521A40, SKILLID_EnergyShield, SKILLTYPE_Right, 0x00996130, 0 },

	{ "Diastrophism", "Strong geographical attack to opponents within range. Diastrophism hits each enemy 2 times per cast.",
	43, 33, 1, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521A80, SKILLID_Diastrophism, SKILLTYPE_Right, 0x009961D0, 0 },

	{ "Spirit Elemental", "Summons a ghost to temporarily raises your base attack power",
	50, 31, 1, 128, -1,
	{ 0, 0, 0 }, { ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521AA0, SKILLID_SpiritElemental, SKILLTYPE_Right, 0x00996248, 0 },

	{ "Dancing Sword", "Summons a magical sword to attack nearby enemies attributed with Ice or Fire element",
	50, 31, 1, 128, -1,
	{ 0, 0, 0 }, { ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521AF0, SKILLID_DancingSword, SKILLTYPE_Right, 0x00996388, 0 },

	{ "Fire Elemental", "Summons a monster made of fire to help attacking neaby enemies",
	60, 120, 2, 169, 0,
	{ 1, 0, 0 }, { ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521D30, SKILLID_FireElemental, SKILLTYPE_Right, 0x00996450, 0 },

	{ "Flame Wave", "Creates a giant wave of fire, damaging all enemies within range",
	63, 32, 1, 70, 3,
	{ 1, 0, 0 }, { ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521D70, SKILLID_FlameWave, SKILLTYPE_All, 0x009964F0, 0 },

	{ "Distortion", "Generates a pulse of enegy, temporarily slowing down the movements of all enemies within the area",
	66, 43, 1, 130, 6,
	{ 1, 0, 0 }, { ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521D90, SKILLID_Distortion, SKILLTYPE_Right, 0x009965B8, 0 },

	{ "Meteorite", "Summons a meteor shower to fall from the sky, damaging all enemies within the area",
	70, 39, 1, 180, 4,
	{ 1, 0, 0 }, { ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521DC0, SKILLID_Meteorite, SKILLTYPE_All, 0x00996658, 0 },

	{ "Wizard's Trance", "Increase the Orb Effectiveness by concentrating in defenses but reduces attack rate. Only works when using Orb",
	50, 31, 1, 128, -1,
	{ 2, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521EA0, SKILLID_WizardTrance, SKILLTYPE_Right, 0x009967C0, 0 },

	{ "Flame Ball", "Launches two spheres of powerful flame against a single target with increased damage and attack rating",
	103, 45, 2, 70, 3,
	{ 2, 0, 0 }, { ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521E70, SKILLID_StoneSkin, SKILLTYPE_All, 0x00996680, 0 },

	{ "Elemental Ray", "Launch a ray that defeat the enemy",
	106, 50, 2, 70, 3,
	{ 2, 0, 0 }, { ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521E90, SKILLID_RedRay, SKILLTYPE_Right, 0x00996720, 0 },

	{ "Cataclysm", "Invokes the Wrath of Nature to form a series of powerful attacks",
	110, 60, 2, 180, 4,
	{ 2, 0, 0 }, { ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00521EC0, SKILLID_Cataclysm, SKILLTYPE_Right, 0x00996838, 0 },

	{ "Dark Bolt", "Hits the enemy with an almighty bolt of dark magic created with sorcery",
	10, 8, 1, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Phantom, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00522070, SKILLID_DarkBolt, SKILLTYPE_All, 0x009987A0, 0 },

	{ "Dark Wave", "Performs a consecutive area attack with almighty bolts of dark magic",
	12, 19, 1, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Phantom, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00522090, SKILLID_DarkWave, SKILLTYPE_All, 0x00998818, 0 },

	{ "Inertia", "Throws a curse onto the enemies, slowing them down temporarily",
	14, 20, 1, 130, 6,
	{ 0, 0, 0 }, { ITEMTYPE_Phantom, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005220B0, SKILLID_Inertia, SKILLTYPE_Right, 0x009988E0, 0 },

	{ "Inner Peace", "Through mental training, increases MP potion recovery and mana capacity permanently",
	17, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005220E0, SKILLID_InnerPeace, SKILLTYPE_Passive, 0x00000000, 0 },

	{ "Spiritual Flare", "Uses powerful sorcery to deal damage to all enemies nearby in the area",
	20, 21, 1, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Phantom, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005220F0, SKILLID_SpiritualFlare, SKILLTYPE_All, 0x009989D0, 0 },

	{ "Spiritual Manacle", "Binds the target's soul with sorcery that results in a stun of the target",
	23, 23, 1, 114, 4,
	{ 0, 0, 0 }, { ITEMTYPE_Phantom, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00522110, SKILLID_SpiritualManacle, SKILLTYPE_All, 0x00998A98, 0 },

	{ "Chasing Hunt", "Temporarily increases your base attack power and sight, and gatherers nearby enemies close by pulling them towards you",
	50, 31, 1, 128, -1,
	{ 0, 0, 0 }, { ITEMTYPE_Phantom, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00522140, SKILLID_ChasingHunt, SKILLTYPE_Right, 0x00998AE8, 0 },

	{ "Advent Migal", "Calls forth Migal to temporarily increases attack power of yourself and your party members",
	50, 31, 1, 128, -1,
	{ 0, 0, 0 }, { ITEMTYPE_Phantom, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00522160, SKILLID_AdventMigal, SKILLTYPE_Right, 0x00998B60, 0 },

	{ "Rainmaker", "While using Phantoms, invoke the Gods of Rain to reduce received damage and increases your evasion rating temporarily",
	50, 31, 1, 128, -1,
	{ 0, 0, 0 }, { ITEMTYPE_Phantom, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005221A0, SKILLID_Rainmaker, SKILLTYPE_Right, 0x00998BD8, 0 },

	{ "Phantom Call", "Controlling cursed ghosts, invokes them to deal massive damage to enemies in range",
	43, 33, 1, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Phantom, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005221C0, SKILLID_PhantomCall, SKILLTYPE_Right, 0x00998C78, 0 },

	{ "Haunt", "With help of Phantom's strength, strikes a forceful blow and absorbs damage into health points",
	46, 35, 1, 169, 0,
	{ 0, 0, 0 }, { ITEMTYPE_Phantom, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005221E0, SKILLID_Haunt, SKILLTYPE_All, 0x00998D18, 0 },

	{ "Scratch", "Claw the opponent fiercely with huge damage",
	50, 32, 1, 70, 3,
	{ 0, 0, 0 }, { ITEMTYPE_Phantom, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00522200, SKILLID_Scratch, SKILLTYPE_All, 0x00998DB8, 0 },

	{ "Summon Hestian", "Summons a Hestian to your aid, attacking nearby enemies",
	60, 41, 1, 169, 0,
	{ 1, 0, 0 }, { ITEMTYPE_Phantom, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00522220, SKILLID_RecallHestian, SKILLTYPE_Right, 0x00998E30, 0 },

	{ "Judgement", "Attacks with increased damage and critical chance, transforming the souls of enemies in a powerful energy beam",
	63, 41, 1, 140, 2,
	{ 1, 0, 0 }, { ITEMTYPE_Phantom, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00522260, SKILLID_Judgement, SKILLTYPE_All, 0x00998EF8, 0 },

	{ "Advent Midranda", "Invokes Midranda to temporarily increase your attack speed",
	50, 31, 1, 128, -1,
	{ 1, 0, 0 }, { ITEMTYPE_Phantom, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00522280, SKILLID_AdventMidranda, SKILLTYPE_Right, 0x00998F98, 0 },

	{ "Mourning Pray", "Using the sacred powers of gods, inflicts a damage to all nearby enemies",
	70, 47, 1, 160, 2,
	{ 1, 0, 0 }, { ITEMTYPE_Phantom, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005222C0, SKILLID_MourningPray, SKILLTYPE_Right, 0x00999010, 0 },

	{ "Creed", "Increases MP and STM of target temporarily",
	50, 31, 1, 128, -1,
	{ 2, 0, 0 }, { ITEMTYPE_Phantom, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x005222E0, SKILLID_Creed, SKILLTYPE_Right, 0x009990B0, 0 },

	{ "Press Deity", "Summons a powerful force from the darkness to damage your enemies in area",
	103, 45, 2, 70, 3,
	{ 2, 0, 0 }, { ITEMTYPE_Phantom, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00522300, SKILLID_PressDeity, SKILLTYPE_All, 0x00999150, 0 },

	{ "Phantom Nail", "Calls upon ghosts to scratch your enemies with powerful dark force",
	106, 54, 2, 70, 3,
	{ 2, 0, 0 }, { ITEMTYPE_Phantom, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00522320, SKILLID_PhantomNail, SKILLTYPE_Right, 0x009991F0, 0 },

	{ "Occult Life", "Harness powers of the occult increasing maximum health permanently",
	110, 0, 0, 0, 0,
	{ 2, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00522340, SKILLID_OccultLife, SKILLTYPE_Passive, 0x00000000, 0 },

	{ "Force", "Increases your damage temporarily",
	0, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00000000, SKILLID_Force, SKILLTYPE_Undefined, 0x00000000, 0 },

	{ "QUEST", "Hunt Monsters!",
	0, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00000000, SKILLID_QUEST1, SKILLTYPE_Undefined, 0x00000000, 0 },

	{ "QUEST", "Hunt Monsters!",
	0, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00000000, SKILLID_QUEST2, SKILLTYPE_Undefined, 0x00000000, 0 },

	{ "QUEST", "Hunt Monsters!",
	0, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00000000, SKILLID_QUEST3, SKILLTYPE_Undefined, 0x00000000, 0 },

	{ "QUEST", "Hunt Monsters!",
	0, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00000000, SKILLID_QUEST4, SKILLTYPE_Undefined, 0x00000000, 0 },

	{ "Absorption of Glory", "Increases Absorption Rating of all Clan Members by 15%",
	0, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00000000, SKILLID_AbsorptionOfGlory, SKILLTYPE_Undefined, 0x00000000, 0 },

	{ "Power Siege", "Increases Attack Power of all Clan Members by 10%",
	0, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00000000, SKILLID_PowerSiege, SKILLTYPE_Undefined, 0x00000000, 0 },

	{ "Evasion Adaption", "Increases Evasion Rating of all Clan Members by 10%",
	0, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00000000, SKILLID_EvasionAdaption, SKILLTYPE_Undefined, 0x00000000, 0 },

	{ "Atanasia", "Decrease ATK Power by half and God Mode for a while",
	0, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00000000, SKILLID_Atanasia, SKILLTYPE_Undefined, 0x00000000, 0 },

	{ "Deadly Edge", "Add critical rating by +5% in PVP areas",
	0, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00000000, SKILLID_DeadlyEdge, SKILLTYPE_Undefined, 0x00000000, 0 },

	{ "Avail of Evasion", "Add evasion rating by +5% in PVP areas",
	0, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00000000, SKILLID_AvailofEvasion, SKILLTYPE_Undefined, 0x00000000, 0 },

	{ "Defiance Stone", "Decrease Damage by 50% of Fire Crystal Tower",
	0, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00000000, SKILLID_DefianceStoneA, SKILLTYPE_Undefined, 0x00000000, 0 },

	{ "Defiance Stone", "Decrease Damage by 50% of Ice Crystal Tower",
	0, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00000000, SKILLID_DefianceStoneB, SKILLTYPE_Undefined, 0x00000000, 0 },

	{ "Defiance Stone", "Decrease Damage by 50% of Lightning Crystal Tower",
	0, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00000000, SKILLID_DefianceStoneC, SKILLTYPE_Undefined, 0x00000000, 0 },

	{ "Might Stone", "Increases Final DMG Boost by +5% against Fighters",
	0, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00000000, SKILLID_FighterStone, SKILLTYPE_Undefined, 0x00000000, 0 },

	{ "Might Stone", "Increases Final DMG Boost by +5% against Mechanicians",
	0, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00000000, SKILLID_MechanicianStone, SKILLTYPE_Undefined, 0x00000000, 0 },

	{ "Might Stone", "Increases Final DMG Boost by +5% against Pikemans",
	0, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00000000, SKILLID_PikemanStone, SKILLTYPE_Undefined, 0x00000000, 0 },

	{ "Might Stone", "Increases Final DMG Boost by +5% against Archers",
	0, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00000000, SKILLID_ArcherStone, SKILLTYPE_Undefined, 0x00000000, 0 },

	{ "Might Stone", "Increases Final DMG Boost by +5% against Knights",
	0, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00000000, SKILLID_KnightStone, SKILLTYPE_Undefined, 0x00000000, 0 },

	{ "Might Stone", "Increases Final DMG Boost by +5% against Atalantas",
	0, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00000000, SKILLID_AtalantaStone, SKILLTYPE_Undefined, 0x00000000, 0 },

	{ "Might Stone", "Increases Final DMG Boost by +5% against Magicians",
	0, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00000000, SKILLID_PriestessStone, SKILLTYPE_Undefined, 0x00000000, 0 },

	{ "Might Stone", "Increases Final DMG Boost by +5% against Priestesses",
	0, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00000000, SKILLID_MagicianStone, SKILLTYPE_Undefined, 0x00000000, 0 },

	{ "Might Stone", "Increases Final DMG Boost by +5% against Shamans",
	0, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00000000, SKILLID_ShamanStone, SKILLTYPE_Undefined, 0x00000000, 0 },

	{ "Might Stone", "Increases Final DMG Boost by +5% against Assassins",
	0, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00000000, SKILLID_MightStone, SKILLTYPE_Undefined, 0x00000000, 0 },

	{ "Eternal Life", "Becomes invincible for the set time. The attack power decrease in 1/2",
	0, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00000000, SKILLID_EternalLife, SKILLTYPE_Undefined, 0x00000000, 0 },

	{ "Fatal Edge", "+10% Final Damage Boost against Bellatra Monsters",
	0, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00000000, SKILLID_FatalEdge, SKILLTYPE_Undefined, 0x00000000, 0 },

	{"Avert Scroll", "+10% of Absorption against Bellatra Monsters",
	0, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00000000, SKILLID_AvertScroll, SKILLTYPE_Undefined, 0x00000000, 0},

	{"Rivera", "Invincible character attcak. The attack power decrease in 1/2",
	0, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00000000, SKILLID_Rivera, SKILLTYPE_Undefined, 0x00000000, 0},

	{"PvP Honor", "You received this Honor since you're under the best PvP Players",
	0, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00000000, SKILLID_PvPHonor, SKILLTYPE_Undefined, 0x00000000, 0},

	{"Bellatra Honor", "You received this Honor since you're under the best Bellatra Players",
	0, 0, 0, 0, 0,
	{ 0, 0, 0 }, { ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
	0x00000000, SKILLID_BellatraHonor, SKILLTYPE_Undefined, 0x00000000, 0},
};