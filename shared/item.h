#pragma once

#define ITEM_SERVER_MAX				1500
#define INVENTORYSERVER_MAX			100
#define	MAX_ITEMSINITEMBOX			50

#define MAX_AGING				24

enum EWeaponClass
{
	WEAPONCLASS_None,
	WEAPONCLASS_Melee,
	WEAPONCLASS_Ranged,
	WEAPONCLASS_Magic,
};

static int iaAgingChance[MAX_AGING] =
{
	0,0,0,0,0,0,0,0,4,8,12,16,18,20,24,28,32,36,40,44,48,52,56,60
};

static DWORD iaSheltomAgingList[MAX_AGING][12]
{
	{3, 3, 4, 4, 5, 0, 0, 0, 0, 0, 0, 0},					// 1
	{3, 3, 4, 4, 5, 5, 0, 0, 0, 0, 0, 0},					// 2
	{3, 3, 4, 4, 5, 5, 6, 0, 0, 0, 0, 0},					// 3
	{3, 3, 4, 4, 5, 5, 6, 6, 0, 0, 0, 0},					// 4
	{3, 3, 4, 4, 5, 5, 6, 6, 7, 0, 0, 0},					// 5
	{3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 0, 0},					// 6
	{3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 0},					// 7
	{3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8},					// 8
	{4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 0},					// 9
	{4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9},					// 10
	{5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 0},					// 11
	{5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10},					// 12
	{6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 0},				// 13
	{6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11},				// 14
	{7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 0},				// 15
	{7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12},				// 16
	{8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 0},			// 17
	{8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13},			// 18
	{9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 0},			// 19
	{9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14},			// 20
	{10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 0},		// 21
	{10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15},		// 22
	{11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 16, 0},		// 23
	{11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 16, 16},		// 24
};




enum EPotionPosition : int
{
	POTIONPOSITION_HP,
	POTIONPOSITION_MP,
	POTIONPOSITION_SP,
};

enum EMatureType
{
	MATURETYPE_Attack = 100,
	MATURETYPE_Critical = 101,
	MATURETYPE_Block = 102,
	MATURETYPE_Orb = 103,
	MATURETYPE_Armor = 104,
	MATURETYPE_Boots = 105,
	MATURETYPE_Gauntlets = 106,
	MATURETYPE_Bracelets = 107,
};

enum EItemLockedType : unsigned char
{
	ITEMLOCKEDTYPE_None,
	ITEMLOCKEDTYPE_1Hour,
	ITEMLOCKEDTYPE_2Hours,
	ITEMLOCKEDTYPE_4Hours,
	ITEMLOCKEDTYPE_8Hours,
	ITEMLOCKEDTYPE_12Hours,
	ITEMLOCKEDTYPE_24Hours,
	ITEMLOCKEDTYPE_36Hours,
	ITEMLOCKEDTYPE_48Hours,
	ITEMLOCKEDTYPE_72Hours,
	ITEMLOCKEDTYPE_OneWeek,
	ITEMLOCKEDTYPE_15Days,
	ITEMLOCKEDTYPE_30Days,

};

enum EItemID
{
	ITEMID_None											= 0,

	ITEMID_RecipeCode									= 0x0A020000,
	ITEMID_RuneCodeManufacture1							= 0x03100000,

	ITEMID_HopyCrystal									= 0x08020100,
	ITEMID_HobgoblinCrystal								= 0x08020200,
	ITEMID_DecoyCrystal									= 0x08020300,
	ITEMID_BargonCrystal								= 0x08020400,
	ITEMID_HeadCutterCrystal							= 0x08020500,
	ITEMID_FigonCrystal									= 0x08020600,
	ITEMID_KingHopyCrystal								= 0x08020700,
	ITEMID_HulkCrystal									= 0x08020800,
	ITEMID_MysticCrystal								= 0x08020900,
	ITEMID_GuardianSaintCrystal							= 0x08020A00,
	ITEMID_WebCrystal									= 0x08020B00,
	ITEMID_DarkSpecterCrystal							= 0x08020C00,
	ITEMID_IronGuardCrystal								= 0x08020D00,
	ITEMID_RicartenMilitiaGuardCrystal					= 0x08020E00,
	ITEMID_RicartenGuardCrystal							= 0x08020F00,
	ITEMID_RoyalBlessGuardCrystal						= 0x08021000,
	ITEMID_OccultCrystal								= 0x08021100,
	ITEMID_DoomedCrystal								= 0x08021200,
	ITEMID_DuskyCrystal									= 0x08021300,
	ITEMID_AbysmalCrystal								= 0x08021800, //GP124
	ITEMID_ArcticCrystal								= 0x08021900, //GP125
	ITEMID_ValentoCrystal								= 0x08021E00, //GP130
	ITEMID_KelvezuCrystal								= 0x08021A00, //GP126
	ITEMID_TullaCrystal									= 0x08022000, //GP132
	ITEMID_CaravanHopyCrystal							= 0x08022500, //GP137
	ITEMID_SoulCrystal									= 0x08022600, //GP138
	ITEMID_SoulSummonCrystal							= 0x08022700, //GP139
	ITEMID_CaravanArmaCrystal							= 0x08022A00, //GP142
	ITEMID_CaravanBumaCrystal							= 0x08022B00, //GP143
	ITEMID_CaravanCuepyCrystal							= 0x08022C00, //GP144
	ITEMID_CaravanHungkyCrystal							= 0x08022D00, //GP145
	ITEMID_CaravanRabieCrystal							= 0x08022E00, //GP146
	ITEMID_CaravanTobieCrystal							= 0x08022F00, //GP147
	ITEMID_CaravanRudolphCrystal						= 0x08023000, //GP148
	ITEMID_CaravanSirKnightCrystal						= 0x08023100, //GP149
	ITEMID_CaravanPinguCrystal							= 0x08023200, //GP150
	ITEMID_CaravanHopyCheapCrystal						= 0x08023300, //GP151
	ITEMID_CaravanYetiCrystal							= 0x08023400, //GP152
	ITEMID_CaravanGoldenHopyCrystal						= 0x08023500, //GP153

	ITEMID_MatureStoneA									= 0x080B3400,
	ITEMID_MatureStoneB									= 0x080B3500,
	ITEMID_MatureStoneC									= 0x080B3600,

	ITEMID_BlueStone									= 0x080B0100,
	ITEMID_RedStone										= 0x080B0200,
	ITEMID_GreenStone									= 0x080B0300,

	ITEMID_SapphireStone								= 0x080B3900,
	ITEMID_RubyStone									= 0x080B3700,

	//Cores
	ITEMID_RicartenCore									= 0x06010100,
	ITEMID_NaviskoCore									= 0x06010200,
	ITEMID_AtlantisCore									= 0x06010300,
	ITEMID_PillaiCore									= 0x06010400,
	ITEMID_UnionCore									= 0x06010500,
	ITEMID_PartyCore									= 0x06010700,

	ITEMID_Athanasia									= 0x080A0100, //BC101
	ITEMID_DeadlyEdge									= 0x080A0200, //BC102
	ITEMID_AvailOfEvade									= 0x080A0300, //BC103
	ITEMID_BolsterRecovery								= 0x080A0400, //BC104
	ITEMID_Restoration									= 0x080A0500, //BC105
	ITEMID_DefianceStoneFire							= 0x080A0600, //BC106
	ITEMID_DefianceStoneIce								= 0x080A0700, //BC107
	ITEMID_DefianceStoneLight							= 0x080A0800, //BC108
	ITEMID_MightPotionFighter							= 0x080A0900, //BC109
	ITEMID_MightPotionMechanician						= 0x080A0A00, //BC110
	ITEMID_MightPotionPikeman							= 0x080A0B00, //BC111
	ITEMID_MightPotionArcher							= 0x080A0C00, //BC112
	ITEMID_MightPotionKnight							= 0x080A0D00, //BC113
	ITEMID_MightPotionAtalanta							= 0x080A0E00, //BC114
	ITEMID_MightPotionMagician							= 0x080A0F00, //BC115
	ITEMID_MightPotionPriestess							= 0x080A1000, //BC116

	ITEMID_HPBooster									= 0x080A1500, //BC121
	ITEMID_MPBooster									= 0x080A1800, //BC124
	ITEMID_STMBooster									= 0x080A1B00, //BC127

	ITEMID_EternalLife									= 0x080B0500, //BI105
	ITEMID_FatalEdge									= 0x080B0600, //BI106
	ITEMID_AvertScroll									= 0x080B0700, //BI107

	//Quest Items
	ITEMID_Roitra										= 0x07010700, //QT107
	ITEMID_TearsOfKalia									= 0x07010800, //QT108
	ITEMID_DemonHunter									= 0x01010800, //WA108 (Quest)
	ITEMID_AncientRevenge								= 0x01020800, //WC108 (Quest)
	ITEMID_DarkSkull									= 0x01040900, //WM109 (Quest)
	ITEMID_DevilScythe									= 0x01050900, //WP109 (Quest)
	ITEMID_Mist											= 0x01060A00, //WS110 (Quest)
	ITEMID_PlatinumSword								= 0x01070A00, //WS210 (Quest)
	ITEMID_Anaconda										= 0x01080900, //WT109 (Quest)
	ITEMID_PlatinumDagger								= 0x010A0900, //WD109 (Quest)
	ITEMID_HutanPhantom									= 0x01090900, //WN109 (Quest)

	ITEMID_RoyalAmulet									= 0x07010D00, //QT113
	ITEMID_MagicRune									= 0x07011300, //QT119 (Frozen Quest)
	ITEMID_BlessOfKalia									= 0x07011900, //QT125 (Frozen Quest)
	ITEMID_GoldBag										= 0x07011700, //QT123
	ITEMID_ValentoEye									= 0x07013200, //QT134 (Tier5)
	ITEMID_PrinceEye									= 0x07013300, //QT135 (Tier5)
	ITEMID_DiamondTears									= 0x07013400, //QT136 (Tier5)
	ITEMID_GreenLiquidTree								= 0x07013500, //QT137 (Green Liquid Frozen Sanctuary Tree)
	ITEMID_KingHopyHorn									= 0x07013600, //QT138 (King Hopy Quest)

	//EXP Items
	ITEMID_RebirthScroll								= 0x080B0400, //BI104

	ITEMID_ExpUp1H										= 0x080B0F00,
	ITEMID_ExpUp3H										= 0x080B1A00,
	ITEMID_ExpUp24H										= 0x080B0D00,
	ITEMID_ExpUp48H										= 0x080B1800,
	ITEMID_ExpUp72H										= 0x080B1900,

	ITEMID_ThirdEye1H									= 0x080F1100,
	ITEMID_ThirdEye3H									= 0x080B1B00,
	ITEMID_ThirdEye24H									= 0x080B0C00,
	ITEMID_ThirdEye48H									= 0x080B0E00,
	ITEMID_ThirdEye72H									= 0x080B5E00,

	ITEMID_PhoenixEgg									= 0x080E0100,
	ITEMID_PhoenixFire									= 0x080B4400,
	ITEMID_PhoenixIce									= 0x080B4500,
	ITEMID_PhoenixLightning								= 0x080B4600,
	ITEMID_PhoenixHealing								= 0x080B4700,

	ITEMID_HPUp12H										= 0x080B1300,
	ITEMID_MPUp12H										= 0x080B1500,
	ITEMID_MPDown12H									= 0x080B1C00,
	ITEMID_SPDown12H									= 0x080B1D00,

	ITEMID_HPUp24H										= 0x080B1400,
	ITEMID_MPUp24H										= 0x080B1600,
	ITEMID_MPDown24H									= 0x080C2400,
	ITEMID_SPDown24H									= 0x080C2700,

	ITEMID_HPUp48H										= 0x080B1E00,
	ITEMID_MPUp48H										= 0x080B2000,
	ITEMID_MPDown48H									= 0x080C2500,
	ITEMID_SPDown48H									= 0x080C2B00,

	ITEMID_HPUp72H										= 0x080B1F00,
	ITEMID_MPUp72H										= 0x080B2100,
	ITEMID_MPDown72H									= 0x080C2600,
	ITEMID_SPDown72H									= 0x080C2C00,

	ITEMID_GravityStone72H								= 0x080B6400,

	ITEMID_TeleportCore									= 0x080B0800,

	ITEMID_AgingStone									= 0x080B0A00,
	ITEMID_CopperOre									= 0x080B0B00,
	ITEMID_RipeStone									= 0x080B4E00,
	ITEMID_FluenceStone									= 0x080C2E00,
	ITEMID_ArcaneStone									= 0x080C2D00,
	ITEMID_MaxAge										= 0x080C5200,

	ITEMID_FeatherTier1									= 0x080C1F00, //BI231
	ITEMID_FeatherTier2									= 0x080C2000, //BI232
	ITEMID_FeatherTier3									= 0x080C2100, //BI233
	ITEMID_FeatherTier4									= 0x080C2200, //BI234
	ITEMID_FeatherTier5									= 0x080C2300, //BI235

	ITEMID_ActionFieldPass								= 0x080C5700, //BI283
	ITEMID_ActionFieldPassFD							= 0x080C5800, //BI284
	ITEMID_ActionFieldPassDD							= 0x080C5900, //BI285

	ITEMID_BigHeadHappyness								= 0x080B6F00,
	ITEMID_BigHeadLove									= 0x080B7000,
	ITEMID_BigHeadSadness								= 0x080B7100,
	ITEMID_BigHeadShyness								= 0x080B7200,
	ITEMID_BigHeadAngry									= 0x08110900,
	ITEMID_BigHeadSurprised								= 0x08110A00,
	ITEMID_BigHeadSensual								= 0x08110B00,

	ITEMID_HairPotionA									= 0x080B1000,
	ITEMID_HairPotionB									= 0x080B1100,
	ITEMID_HairPotionC									= 0x080B1200,
	ITEMID_HairPotionD									= 0x080C0D00,
	ITEMID_HairPotionE									= 0x080C0E00,
	ITEMID_HairPotionF									= 0x080C0F00,
	ITEMID_HairPotionG									= 0x080C1000,
	ITEMID_HairPotionH									= 0x080C1100,
	ITEMID_HairPotionI									= 0x080C1200,
	ITEMID_HairPotionJ									= 0x080C1300,
	ITEMID_HairPotionK									= 0x080C1400,
	ITEMID_HairPotionL									= 0x080C1500,
	ITEMID_HairPotionM									= 0x080C1600,
	ITEMID_HairPotionN									= 0x08110300,
	ITEMID_HairPotionO									= 0x08110400,
	ITEMID_HairPotionP									= 0x08110500,
	ITEMID_HairPotionQ									= 0x08110600,
	ITEMID_HairPotionR									= 0x08110700,
	ITEMID_HairPotionS									= 0x08110C00,
	ITEMID_HairPotionT									= 0x08110D00,

	ITEMID_SheepHat										= 0x080C3800,
	ITEMID_GiraffeHat									= 0x080C3900,
	ITEMID_SoccerHat									= 0x080B6410,

	ITEMID_XMasGreenHat									= 0x080B6415,
	ITEMID_XMasRedHat									= 0x080B6420,
	ITEMID_BCBuff										= 0x080B6416,
	ITEMID_CaravanHopy									= 0x080B6417,
	ITEMID_CaravanRudolf								= 0x080B6418,

	ITEMID_RomanticSuit									= 0x02111200,
	ITEMID_RomanticDress								= 0x02111300,

	ITEMID_CartolaHat									= 0x080B6500,
	ITEMID_WitchHat										= 0x080B6600,
	ITEMID_HalloweenHat									= 0x080B6800,
	ITEMID_TuttiFruttiHat								= 0x080B6C00, //BI192
	ITEMID_EasterHat									= 0x080B6D00, //BI193
	ITEMID_TropicalScroll								= 0x080B6E00, //BI194
	ITEMID_ChemistHat									= 0x080C0100, //BI201
	ITEMID_PatronumHat									= 0x080C0200, //BI202
	ITEMID_SharinganEye									= 0x080C0300, //BI203
	ITEMID_IceHair										= 0x080C0400, //BI204
	ITEMID_SamuraiHair									= 0x080C0500, //BI205
	ITEMID_GoldenHopyHat								= 0x080C0600, //BI206
	ITEMID_ObscureHat									= 0x080C0700, //BI207

	ITEMID_PinguMountMysterious							= 0x080F1200, //BI318
	ITEMID_MountObscureHog								= 0x080F1300, //BI319
	ITEMID_MountSnowflakeUnicorn						= 0x080F1400, //BI320
	ITEMID_MountHog										= 0x080F1500, //BI321
	ITEMID_MountHog1									= 0x080F1600, //BI322
	ITEMID_MountHopy									= 0x080F1700, //BI323
	ITEMID_MountHopy1									= 0x080F1800, //BI324
	ITEMID_MountHopy2									= 0x080F1900, //BI325
	ITEMID_MountHorse									= 0x080F1A00, //BI326
	ITEMID_MountHorse1									= 0x080F1B00, //BI327
	ITEMID_MountHorse2									= 0x080F1C00, //BI328
	ITEMID_MountLion1									= 0x080F1D00, //BI329
	ITEMID_MountLion5									= 0x080F1E00, //BI330
	ITEMID_MountUnicorn3								= 0x080F1F00, //BI331
	ITEMID_MountPingu1									= 0x080F2000, //BI332
	ITEMID_MountGhostWolf								= 0x080F2100, //BI333
	ITEMID_MountHorsePTM								= 0x080F2200, //BI334
	ITEMID_MountSnowdeer								= 0x080F2300, //BI335
	ITEMID_MountRudolf									= 0x080F2400, //BI336
	ITEMID_MountWhiteDeer								= 0x080F2500, //BI337
	ITEMID_MountIronCharger								= 0x080F2600, //BI338
	ITEMID_MountMecaballus								= 0x080F2700, //BI339
	ITEMID_MountZira									= 0x080F2800, //BI340
	ITEMID_MountIceTiger								= 0x080F2900, //BI341
	ITEMID_MountUnicorn6								= 0x080F2A00, //BI342
	ITEMID_MountRabie									= 0x080F2B00, //BI343
	ITEMID_MountRaptor									= 0x080F2C00, //BI344
	ITEMID_MountRaptor1									= 0x080F2D00, //BI345
	ITEMID_MountRaptor2									= 0x080F2E00, //BI346
	ITEMID_MountRaptor3									= 0x080F2F00, //BI347


	ITEMID_TurquoiseStone								= 0x080B5000,
	ITEMID_TopazStone									= 0x080B4F00,

	ITEMID_EmeraldStone									= 0x080B3800,

	ITEMID_VampireSuit									= 0x02100900,
	ITEMID_Succubus										= 0x02100B00,

	ITEMID_Jera											= 0x03070100,
	ITEMID_Nied											= 0x03070200,
	ITEMID_Gyfu											= 0x03070300,
	ITEMID_SuperJera									= 0x03070400,

	ITEMID_ValentoRing									= 0x03050500,
	ITEMID_KelvezuAmulet								= 0x03012E00,
	ITEMID_TullaAmulet									= 0x03013C00,
	ITEMID_ThroneAmulet									= 0x03013D00, //OA145
	ITEMID_FallenRing									= 0x03054100,
	ITEMID_MokovaBoots									= 0x02022E00,
	ITEMID_AbyssRing									= 0x03054000, //OR210
	ITEMID_GuidanceAmulet								= 0x03011900, //OA125
	ITEMID_GuidanceAmuletPlus1							= 0x03011A00, //OA126
	ITEMID_DraxosBoots									= 0x02024200,
	ITEMID_DarkGuardianSheltom							= 0x02352E00,
	ITEMID_DevilShyRing									= 0x03054200, //OR248
	ITEMID_CeruleanRing									= 0x03054300, //OR249
	ITEMID_GhostCastleBracelets							= 0x03022F00,

	ITEMID_ManaMystic									= 0x04010400,

	//Halloween
	ITEMID_CoreHW										= 0x0A026001,
	ITEMID_RottenLantern								= 0x080B6001,
	ITEMID_EvilLantern									= 0x080B6002,
	ITEMID_InfernalLantern								= 0x080B6003,
	ITEMID_ToiletPaper									= 0x03106004,
	ITEMID_HopyToy										= 0x03106005,
	ITEMID_SpiderPlastic								= 0x03106006,
	ITEMID_GriffenEgg									= 0x03106007,

	//Sheltom
	ITEMID_Devine										= 0x02350800, //OS108
	ITEMID_Celesto										= 0x02350900, //OS109
	ITEMID_Mirage										= 0x02350A00, //OS110
	ITEMID_Inferna										= 0x02350B00, //OS111
	ITEMID_Enigma										= 0x02350C00, //OS112
	ITEMID_Bellum										= 0x02350D00, //OS113
	ITEMID_Oredo										= 0x02350E00, //OS114
	ITEMID_Sapphire										= 0x02350F00, //OS115
	ITEMID_Sol											= 0x02351000, //OS116
	ITEMID_FurySheltom									= 0x02351500, //OS121
	ITEMID_ThroneSheltom								= 0x02351600, //OS122

	//ForceOrb
	ITEMID_LucidyForce									= 0x03060100,
	ITEMID_SerenoForce									= 0x03060200,
	ITEMID_FadeoForce									= 0x03060300,
	ITEMID_SparkyForce									= 0x03060400,
	ITEMID_RaidentForce									= 0x03060500,
	ITEMID_TransparoForce								= 0x03060600,
	ITEMID_MurkyForce									= 0x03060700,
	ITEMID_DevineForce									= 0x03060800,
	ITEMID_CelestoForce									= 0x03060900,
	ITEMID_MirageForce									= 0x03060A00,
	ITEMID_InfernaForce									= 0x03060B00,
	ITEMID_EnigmaForce									= 0x03060C00,
	ITEMID_BellumForce									= 0x03060D00,
	ITEMID_OredoForce									= 0x03060E00,
	ITEMID_SapphireForce								= 0x03060F00,
	ITEMID_SolForce										= 0x03061000,

	//Potion
	ITEMID_PotionMiniHP									= 0x04020100,
	ITEMID_PotionMiniMP									= 0x04010100,
	ITEMID_PotionMiniSP									= 0x04030100,

	ITEMID_PotionMediumHP								= 0x04020200,
	ITEMID_PotionMediumMP								= 0x04010200,
	ITEMID_PotionMediumSP								= 0x04030200,

	ITEMID_PotionGreatHP								= 0x04020300,
	ITEMID_PotionGreatMP								= 0x04010300,
	ITEMID_PotionGreatSP								= 0x04030300,

	ITEMID_PotionMysticHP								= 0x04020400,
	ITEMID_PotionMysticMP								= 0x04010400,
	ITEMID_PotionMysticSP								= 0x04030400,

	ITEMID_PotionSuperbHP								= 0x04020500,
	ITEMID_PotionSuperbMP								= 0x04010500,
	ITEMID_PotionSuperbSP								= 0x04030500,


	//Socket System
	ITEMID_RuneDrill									= 0x080C4F00, //BI263
	ITEMID_RedRuneDrill									= 0x080C5000, //BI264
	ITEMID_YellowRuneDrill								= 0x080C5100, //BI265
	ITEMID_GreenRuneDrill								= 0x080C5200, //BI266
	ITEMID_BlueRuneDrill								= 0x080C5300, //BI267
	ITEMID_PurpleRuneDrill1								= 0x080C5400, //BI268
	ITEMID_PurpleRuneDrill2								= 0x080C5500, //BI269
	ITEMID_PurpleRuneDrill3								= 0x080C5600, //BI270
	ITEMID_TitaniumDrill								= 0x080C4000, //BI248
	ITEMID_DiamondDrill									= 0x080C4100, //BI249
	ITEMID_IronDrill									= 0x080C4200, //BI250

	ITEMID_RedTriangularGem								= 0x03130100, //PR501
	ITEMID_RedSquareGem									= 0x03130200, //PR502
	ITEMID_RedMarquiseGem								= 0x03130300, //PR503
	ITEMID_YellowTriangularGem							= 0x03130400, //PR504
	ITEMID_YellowSquareGem								= 0x03130500, //PR505
	ITEMID_YellowMarquiseGem							= 0x03130600, //PR506
	ITEMID_GreenTriangularGem							= 0x03130700, //PR507
	ITEMID_GreenSquareGem								= 0x03130800, //PR508
	ITEMID_GreenMarquiseGem								= 0x03130900, //PR509
	ITEMID_BlueTriangularGem							= 0x03130A00, //PR510
	ITEMID_BlueSquareGem								= 0x03130B00, //PR511
	ITEMID_BlueMarquiseGem								= 0x03130C00, //PR512
	ITEMID_PurpleTriangularGem							= 0x03130D00, //PR513
	ITEMID_PurpleSquareGem								= 0x03130E00, //PR514
	ITEMID_PurpleMarquiseGem							= 0x03130F00, //PR515

	ITEMID_GreenGemPiece								= 0x03131500, //PR521
	ITEMID_RedGemPiece									= 0x03131600, //PR522
	ITEMID_BlueGemPiece									= 0x03131700, //PR523
	ITEMID_PurpleGemPiece								= 0x03131800, //PR524
	ITEMID_YellowGemPiece								= 0x03131900, //PR525

	ITEMID_RedElaboratedGem								= 0x03131000, //PR516
	ITEMID_YellowElaboratedGem							= 0x03131100, //PR517
	ITEMID_GreenElaboratedGem							= 0x03131200, //PR518
	ITEMID_BlueElaboratedGem							= 0x03131300, //PR519
	ITEMID_PurpleElaboratedGem							= 0x03131400, //PR520

	ITEMID_CuepyRune									= 0x03131A00, //PR526
	ITEMID_HopyRune										= 0x03131B00, //PR527
	ITEMID_GoldenHopyRune								= 0x03131C00, //PR528
	ITEMID_MysteriousBookRecipe							= 0x0A026004, //DR214
	ITEMID_SecretRecipe									= 0x0A026005, //DR215

	ITEMID_PurpleDiamond								= 0x03090100,
	ITEMID_SilverDiamond								= 0x03090200,
	ITEMID_GoldDiamond									= 0x03090300,

	//Earrings
	ITEMID_GorgonyteEarring								= 0x03140500, //OE105
	ITEMID_VolcanosEarring								= 0x03140600, //OE106

	//Respec
	ITEMID_SpecStoneCode								= 0x03076000,
	ITEMID_SpecStoneFS									= 0x03076001,
	ITEMID_SpecStoneMS									= 0x03076002,
	ITEMID_SpecStoneAS									= 0x03076003,
	ITEMID_SpecStonePS									= 0x03076004,
	ITEMID_SpecStoneATA									= 0x03076005,
	ITEMID_SpecStoneKS									= 0x03076006,
	ITEMID_SpecStoneMG									= 0x03076007,
	ITEMID_SpecStonePRS									= 0x03076008,
	ITEMID_SpecStoneASS									= 0x03076009,
	ITEMID_SpecStoneSHA									= 0x0307600A,
	ITEMID_DeluxeSpecStoneFS							= 0x0307600B,
	ITEMID_DeluxeSpecStoneMS							= 0x0307600C,
	ITEMID_DeluxeSpecStoneAS							= 0x0307600D,
	ITEMID_DeluxeSpecStonePS							= 0x0307600E,
	ITEMID_DeluxeSpecStoneATA							= 0x0307600F,
	ITEMID_DeluxeSpecStoneKS							= 0x03076010,
	ITEMID_DeluxeSpecStoneMG							= 0x03076011,
	ITEMID_DeluxeSpecStonePRS							= 0x03076012,
	ITEMID_DeluxeSpecStoneASS							= 0x03076013,
	ITEMID_DeluxeSpecStoneSHA							= 0x03076014,

	ITEMID_CleanItem									= 0x080B6900,
	ITEMID_GlamorousAttireGlaze							= 0x080B6A00, //BI190
	ITEMID_GlamorousArmamentVarnish						= 0x080B6B00, //BI191

	ITEMID_Fireworks									= 0x080B1700,

	//Suits
	ITEMID_SoccerShirt									= 0x02053F00,
	ITEMID_SouthGraceHanbokM							= 0x02100100,
	ITEMID_SouthGraceHanbokF							= 0x02100200,
	ITEMID_MagnificentHanbokM							= 0x02100300,
	ITEMID_MagnificentHanbokF							= 0x02100500,
	ITEMID_GraySuitM									= 0x02100700,
	ITEMID_GraySuitF									= 0x02100800,
	ITEMID_RomanticSuitM								= 0x02101100,
	ITEMID_RomanticSuitF								= 0x02101200,
	ITEMID_MechanicCostumeM								= 0x02101500,
	ITEMID_MechanicCostumeF								= 0x02101700,
	ITEMID_ChineseSuitM									= 0x02015000,
	ITEMID_ChineseSuitF									= 0x02015100,
	ITEMID_WarriorsHanbokM								= 0x02014A00,
	ITEMID_WarriorsHanbokF								= 0x02014B00,
	ITEMID_KimonoCostumeM								= 0x02014C00,
	ITEMID_KimonoCostumeF								= 0x02014D00,
	ITEMID_FootballM									= 0x02101900,
	ITEMID_FootballF									= 0x02101B00,
	ITEMID_PeruvianSoccer								= 0x02111900,
	ITEMID_BrazilianSoccer								= 0x02111A00,
	ITEMID_ArgentineSoccer								= 0x02111B00,
	ITEMID_ChileanSoccer								= 0x02111C00,
	ITEMID_PinoySoccer									= 0x02111D00,
	ITEMID_VietnameseSoccer								= 0x02111E00,
	ITEMID_FrenchSoccer									= 0x02111F00,
	ITEMID_GermanySoccer								= 0x02112000,
	ITEMID_HollandSoccer								= 0x02112100,
	ITEMID_ItalianSoccer								= 0x02112200,
	ITEMID_MexicanSoccer								= 0x02112300,
	ITEMID_PortugueseSoccer								= 0x02112400,
	ITEMID_SpanishSoccer								= 0x02112500,
	ITEMID_TeamUSASoccer								= 0x02112600,
	ITEMID_ColombianSoccer								= 0x02112700,
	ITEMID_JapanSoccer									= 0x02112800,

	//Armors
	ITEMID_ViperArmor									= 0x02011600,
	ITEMID_KronusArmor									= 0x02012B00,
	ITEMID_DestroyerArmor								= 0x02011A00,

	//Robes
	ITEMID_PhoenixRobe									= 0x02051600,
	ITEMID_KronusRobe									= 0x02052B00,

	ITEMID_Gold											= 0x05010100,
	ITEMID_Experience									= 0x05010200,

	ITEMID_PotionAgingQuestBee							= 0x09010100, //MA101

	//XMas
	ITEMID_RecipeXMas01									= 0x0A026101,
	ITEMID_RecipeXMas02									= 0x0A026102,
	ITEMID_SnowManCrystal								= 0x08022900,
	ITEMID_Cotton										= 0x0310602E,
	ITEMID_RedButton									= 0x0310602F,
	ITEMID_GreenCloth									= 0x03106031,
	ITEMID_XMasElegantCostumeM							= 0x02100F00,
	ITEMID_XMasElegantCostumeF							= 0x02101000,
	ITEMID_XMasCloatCostumeTM							= 0x02014E00,
	ITEMID_XMasCloatCostumeTF							= 0x02014F00,
	ITEMID_XMasCloatCostumeMM							= 0x02054E00,
	ITEMID_XMasCloatCostumeMF							= 0x02054F00,
	ITEMID_Gingerbread									= 0x080E4200,
	ITEMID_GiftBox1										= 0x08010700, //SP107
	ITEMID_GiftBox2										= 0x08011100, //SP117
	ITEMID_SnowyXmasTree								= 0x080B6100, //BI181
	ITEMID_ColorfulXmasTree								= 0x080B6200, //BI182

	
	//Easter
	ITEMID_RedEasterEgg									= 0x080E4600,
	ITEMID_GreenEasterEgg								= 0x080E4700,
	ITEMID_PinkEasterEgg								= 0x080E4800,

	//Bellatra
	ITEMID_BrilliantStone								= 0x08017100,
	
	ITEMID_QuakeSeal									= 0x08090100,
	ITEMID_StunSeal										= 0x08090200,
	ITEMID_FreezeSeal									= 0x08090300,
	ITEMID_RabieSeal									= 0x08090400,
	ITEMID_StygianSeal									= 0x08090500,
	ITEMID_GuardianSaintSeal							= 0x08090600,
	ITEMID_ScoreSeal									= 0x08090700,

	ITEMID_GrumpyArmor									= 0x0201EE01,
	ITEMID_GrumpyRobe									= 0x0205EE01,


	//Carnival Puzzle
	ITEMID_CarnivalLetterA								= 0x080E4900, //PZ120
	ITEMID_CarnivalLetterC								= 0x080E4A00, //PZ121
	ITEMID_CarnivalLetterI								= 0x080E4B00, //PZ122
	ITEMID_CarnivalLetterL								= 0x080E4C00, //PZ123
	ITEMID_CarnivalLetterN								= 0x080E4D00, //PZ124
	ITEMID_CarnivalLetterR								= 0x080E4E00, //PZ125
	ITEMID_CarnivalLetterV								= 0x080E4F00, //PZ126
	ITEMID_CarnivalApple								= 0x080E5000, //PZ127
	ITEMID_CarnivalBanana								= 0x080E5100, //PZ128
	ITEMID_CarnivalJabuticaba							= 0x080E5200, //PZ129
	ITEMID_CarnivalMaracuja								= 0x080E5300, //PZ130
	ITEMID_CarnivalPitanga								= 0x080E5400, //PZ131
	ITEMID_CarnivalStrawberry							= 0x080E5500, //PZ132

	ITEMID_CandySheltom									= 0x02355A00, //OS190

	//TGF
	ITEMID_KingStone									= 0x07013100,

	//Runes
	ITEMID_LightBlueRune								= 0x03100400,
	ITEMID_LightRedRune									= 0x03100600, //PR306
	ITEMID_DarkRedRune									= 0x03100700, //PR307
	ITEMID_LightGreenRune								= 0x03100800, //PR308
	ITEMID_LightSilverRune								= 0x03100E00, //PR314
	ITEMID_GreenCrystalRune								= 0x03100F00, //PR315
	ITEMID_RedCrystalRune								= 0x03101000, //PR316
	ITEMID_BlueCrystalRune								= 0x03101100, //PR317
	ITEMID_RedXmasGiftRune								= 0x03101200, //PR318
	ITEMID_GreenXmasGiftRune							= 0x03101300, //PR319
	ITEMID_BlueXmasGiftRune								= 0x03101400, //PR320
	ITEMID_IceRune										= 0x03101500, //PR321
	ITEMID_GoldCrystalRune								= 0x03101600, //PR322
	ITEMID_SilverCrystalRune							= 0x03101700, //PR323
	ITEMID_BronzeCrystalRune							= 0x03101800, //PR324
	ITEMID_ThroneRedRune								= 0x03101900, //PR325
	ITEMID_ThroneYellowRune								= 0x03101A00, //PR326
	ITEMID_ThroneGreenRune								= 0x03101B00, //PR327
	ITEMID_LightPurpleRune								= 0x03100100, //PR301
	ITEMID_LightOrangeRune								= 0x03110600, //PR406

	ITEMID_XmasRecipeGift								= 0x0A026002, //DR211
	ITEMID_IceRecipe									= 0x0A026003, //DR213

	//Ghost Castle
	ITEMID_SilverLockedChest							= 0x080E0300, //SP202
	ITEMID_BlueLockedChest								= 0x080E0400, //SP204
	ITEMID_PurpleLockedChest							= 0x080E0500, //SP205
	ITEMID_GoldenLockedChest							= 0x080E0600, //SP206
	ITEMID_ChestKey										= 0x08015F00, //SP179


	//Beginner
	ITEMID_BeginnerAxe									= 0x01012E00, //WA130
	ITEMID_BeginnerClaw									= 0x01022D00, //WC129
	ITEMID_BeginnerHammer								= 0x01032F00, //WH131
	ITEMID_BeginnerSword								= 0x01073100, //WS233
	ITEMID_BeginnerStaff								= 0x01042F00, //WM131
	ITEMID_BeginnerJavelin								= 0x01082E00, //WT130
	ITEMID_BeginnerBow									= 0x01062F00, //WS131
	ITEMID_BeginnerScythe								= 0x01052E00, //WP130
	ITEMID_BeginnerPhantom								= 0x01092D00, //WN129
	ITEMID_BeginnerDagger								= 0x010A2D00, //WD129
	ITEMID_BeginnerGauntlet								= 0x02031B00, //DG127
	ITEMID_BeginnerBoots								= 0x02021B00, //DB127
	ITEMID_BeginnerBracelet								= 0x03021B00, //OA227

	//Costumes
	ITEMID_PartyCostumeMale								= 0x02062F00, //CA131
	ITEMID_PartyCostumeFemale							= 0x02063100, //CA133
	ITEMID_IdasCostumeMale								= 0x02063300, //CA135
	ITEMID_IdasCostumeFemale							= 0x02063500, //CA137
	ITEMID_ThalesArmorMale								= 0x02063B00, //CA143
	ITEMID_ThalesArmorFemale							= 0x02063D00, //CA145
	ITEMID_Costume4ArmorMale							= 0x02064600, //CA154
	ITEMID_Costume4ArmorFemale							= 0x02064700, //CA155
	ITEMID_Costume5ArmorMale							= 0x02065200, //CA166
	ITEMID_Costume5ArmorFemale							= 0x02065400, //CA168
	ITEMID_WingArmorMale								= 0x02066500, //CA185
	ITEMID_WingArmorFemale								= 0x02066700, //CA187
	ITEMID_DarkWingArmorMale							= 0x02066900, //CA189
	ITEMID_DarkWingArmorFemale							= 0x02066B00, //CA191
	ITEMID_SummerClotheMale								= 0x02066D00, //CA193
	ITEMID_SummerClotheFemale							= 0x02066F00, //CA195
	ITEMID_SpringCostumeMale							= 0x02100100, //CA501
	ITEMID_SpringCostumeFemale							= 0x02100200, //CA502
	ITEMID_YellowSpringCostumeMale						= 0x02100300, //CA503
	ITEMID_YellowSpringCostumeFemale					= 0x02100500, //CA505
	ITEMID_Costume11ArmorMale							= 0x02100700, //CA507
	ITEMID_Costume11ArmorFemale							= 0x02100800, //CA508
	ITEMID_BlackSuit									= 0x02101100, //CA517
	ITEMID_WhiteDress									= 0x02101300, //CA519
	ITEMID_BroyaleCostumeMale							= 0x02101600, //CA522
	ITEMID_BroyaleCostumeFemale							= 0x02101800, //CA524
	ITEMID_CasualSchoolUniformMale						= 0x02101F00, //CA531
	ITEMID_CasualSchoolUniformFemale					= 0x02102100, //CA533
	ITEMID_ElegantSchoolUniformMale						= 0x0207A900, //CA353
	ITEMID_ElegantSchoolUniformFemale					= 0x02102500, //CA537
	ITEMID_Costume16ArmorMale							= 0x02102700, //CA539
	ITEMID_Costume16ArmorFemale							= 0x02102900, //CA541
	ITEMID_MarineCostumeMale							= 0x02102B00, //CA543
	ITEMID_MarineCostumeFemale							= 0x02102D00, //CA545
	ITEMID_HopyCostume									= 0x02102F00, //CA547
	ITEMID_CuepyCostume									= 0x02103000, //CA548
	ITEMID_HopyCostumeTime								= 0x02103100, //CA549
	ITEMID_CuepyCostumeTime								= 0x02103200, //CA550
	ITEMID_HopyCostumeIce								= 0x02103300, //CA551
	ITEMID_CuepyCostumeIce								= 0x02103400, //CA552
	ITEMID_ChemistCostumeMale							= 0x02103500, //CA553
	ITEMID_ChemistCostumeFemale							= 0x02103600, //CA554
	ITEMID_DraculaDragonCostumeMale						= 0x02103700, //CA555
	ITEMID_DraculaDragonCostumeFemale					= 0x02103800, //CA556
	ITEMID_AkatsukiCostumeMale							= 0x02103900, //CA557
	ITEMID_AkatsukiCostumeFemale						= 0x02103A00, //CA558
	ITEMID_IceCostumeMale								= 0x02103B00, //CA559
	ITEMID_IceCostumeFemale								= 0x02103C00, //CA560
	ITEMID_GoldenHopyCostumeMale						= 0x02103D00, //CA561
	ITEMID_GoldenHopyCostumeFemale						= 0x02103E00, //CA562
	ITEMID_GoldenHopyCostumeMaleTime					= 0x02103F00, //CA563
	ITEMID_GoldenHopyCostumeFemaleTime					= 0x02104000, //CA564
	ITEMID_SandurrRedCostumeMale						= 0x02104100, //CA565
	ITEMID_SandurrRedCostumeFemale						= 0x02104200, //CA566
	ITEMID_SandurrPurpleCostumeMale						= 0x02104300, //CA567
	ITEMID_SandurrPurpleCostumeFemale					= 0x02104400, //CA568
	ITEMID_ChaoticCostume								= 0x02104500, //CA569
	ITEMID_ChaoticCostumeFemale							= 0x02104600, //CA570
	ITEMID_Costume19MaleArmorFemale						= 0x02014000, //DA148
	ITEMID_GaiaArmorMale								= 0x02014100, //DA149
	ITEMID_GaiaArmorFemale								= 0x02014200, //DA150
	ITEMID_IriaRobeMale									= 0x02054100, //DA249
	ITEMID_IriaRobeFemale								= 0x02054200, //DA250
	ITEMID_MartanosRobeMale								= 0x02073300, //CA235
	ITEMID_MartanosRobeFemale							= 0x02073500, //CA237
	ITEMID_MikalRobeMale								= 0x02073B00, //CA243
	ITEMID_MikalRobeFemale								= 0x02073D00, //CA245
	ITEMID_SilverCostumeMale							= 0x02013700, //DA139
	ITEMID_SilverCostumeFemale							= 0x02013900, //DA141
	ITEMID_DarkGaiaArmorMale							= 0x02014300, //DA151
	ITEMID_DarkGaiaArmorFemale							= 0x02014400, //DA152
	ITEMID_DarkIriaRobeMale								= 0x02054300, //DA251
	ITEMID_DarkIriaRobeFemale							= 0x02054400, //DA252
	ITEMID_WeddingSuit									= 0x02014800, //DA156
	ITEMID_WeddingDress									= 0x02014900, //DA157
	ITEMID_Costume23ArmorMale							= 0x02014A00, //DA158
	ITEMID_Costume23ArmorFemale							= 0x02014B00, //DA159
	ITEMID_Costume24ArmorMale							= 0x02014C00, //DA160
	ITEMID_Costume24ArmorFemale							= 0x02014D00, //DA161
	ITEMID_Costume25ArmorMale							= 0x02015000, //DA164
	ITEMID_Costume25ArmorFemale							= 0x02015100, //DA165

	//Costume Skin Default
	ITEMID_DefaultCostumeSkinAxe						= 0x01013500, //CW1
	ITEMID_DefaultCostumeSkinClaw						= 0x01023300, //CW1
	ITEMID_DefaultCostumeSkinDagger						= 0x010A3200, //CW1
	ITEMID_DefaultCostumeSkinHammer						= 0x01033400, //CW1
	ITEMID_DefaultCostumeSkinWand						= 0x01043500, //CW1
	ITEMID_DefaultCostumeSkinPhantom					= 0x01093200, //CW1
	ITEMID_DefaultCostumeSkinScythe						= 0x01053400, //CW1
	ITEMID_DefaultCostumeSkinBow						= 0x01063300, //CW1
	ITEMID_DefaultCostumeSkinSword						= 0x01073800, //CW1
	ITEMID_DefaultCostumeSkinJavelin					= 0x01083200, //CW1
	ITEMID_DefaultCostumeSkinShield						= 0x02042000, //CS1


	//Costume Skin Default
	ITEMID_DefaultCostumeSkinAxeTime					= 0x01013600, //CW3
	ITEMID_DefaultCostumeSkinClawTime					= 0x01023400, //CW3
	ITEMID_DefaultCostumeSkinDaggerTime					= 0x010A3300, //CW3
	ITEMID_DefaultCostumeSkinHammerTime					= 0x01033500, //CW3
	ITEMID_DefaultCostumeSkinWandTime					= 0x01043600, //CW3
	ITEMID_DefaultCostumeSkinPhantomTime				= 0x01093300, //CW3
	ITEMID_DefaultCostumeSkinScytheTime					= 0x01053500, //CW3
	ITEMID_DefaultCostumeSkinBowTime					= 0x01063400, //CW3
	ITEMID_DefaultCostumeSkinSwordTime					= 0x01073900, //CW3
	ITEMID_DefaultCostumeSkinJavelinTime				= 0x01083300, //CW3
	ITEMID_DefaultCostumeSkinShieldTime					= 0x02042100, //CS3


	//Halloween Costumes
	ITEMID_HalloweenAxe									= 0x01013501, //CW101
	ITEMID_HalloweenClaw								= 0x01023301, //CW102
	ITEMID_HalloweenDagger								= 0x010A3201, //CW103
	ITEMID_HalloweenHammer								= 0x01033401, //CW104
	ITEMID_HalloweenStaff								= 0x01043501, //CW105
	ITEMID_HalloweenWand								= 0x01043502, //CW106
	ITEMID_HalloweenPhantom								= 0x01093201, //CW107
	ITEMID_HalloweenScythe								= 0x01053401, //CW108
	ITEMID_HalloweenBow									= 0x01063301, //CW109
	ITEMID_HalloweenSword								= 0x01073801, //CW110
	ITEMID_HalloweenBladeSword							= 0x01073802, //CW111
	ITEMID_HalloweenJavelin								= 0x01083201, //CW112
	ITEMID_HalloweenShield								= 0x02042001, //CS101

	ITEMID_XmasCostumeOldAxe							= 0x01013502, //CW113
	ITEMID_XmasCostumeOldAxe1Hand						= 0x01013503, //CW114
	ITEMID_XmasCostumeOldClaw							= 0x01023302, //CW115
	ITEMID_XmasCostumeOldDagger							= 0x010A3202, //CW116
	ITEMID_XmasCostumeOldHammer							= 0x01033402, //CW117
	ITEMID_XmasCostumeOldStaff							= 0x01043503, //CW118
	ITEMID_XmasCostumeOldPhantom						= 0x01093202, //CW119
	ITEMID_XmasCostumeOldScythe							= 0x01053402, //CW120
	ITEMID_XmasCostumeOldBow							= 0x01063302, //CW121
	ITEMID_XmasCostumeOldSword							= 0x01073803, //CW122
	ITEMID_XmasCostumeOldSword1Hand						= 0x01073804, //CW123
	ITEMID_XmasCostumeOldJavelin						= 0x01083202, //CW124

	ITEMID_XmasCostumeAxe								= 0x01013504, //CW125
	ITEMID_XmasCostumeAxe1Hand							= 0x01013505, //CW126
	ITEMID_XmasCostumeClaw								= 0x01023303, //CW127
	ITEMID_XmasCostumeDagger							= 0x010A3203, //CW128
	ITEMID_XmasCostumeHammer							= 0x01033403, //CW129
	ITEMID_XmasCostumeStaff								= 0x01043504, //CW130
	ITEMID_XmasCostumeWand								= 0x01043505, //CW131
	ITEMID_XmasCostumePhantom							= 0x01093203, //CW132
	ITEMID_XmasCostumeScythe							= 0x01053403, //CW133
	ITEMID_XmasCostumeBow								= 0x01063303, //CW134
	ITEMID_XmasCostumeSword								= 0x01073805, //CW135
	ITEMID_XmasCostumeSword1Hand						= 0x01073806, //CW136
	ITEMID_XmasCostumeJavelin							= 0x01083203, //CW137
	ITEMID_XmasCostumeShield							= 0x02042002, //CS102

	//Candy
	ITEMID_CandyAxe										= 0x01013506, //CW138
	ITEMID_CandyAxe1Hand								= 0x01013507, //CW139
	ITEMID_CandyClaw									= 0x01023304, //CW140
	ITEMID_CandyDagger									= 0x010A3204, //CW141
	ITEMID_CandyHammer									= 0x01033404, //CW142
	ITEMID_CandyStaff									= 0x01043506, //CW143
	ITEMID_CandyWand									= 0x01043507, //CW144
	ITEMID_CandyPhantom									= 0x01093204, //CW145
	ITEMID_CandyScythe									= 0x01053404, //CW146
	ITEMID_CandyBow										= 0x01063304, //CW147
	ITEMID_CandySword									= 0x01073807, //CW148
	ITEMID_CandySword1Hand								= 0x01073808, //CW149
	ITEMID_CandyJavelin									= 0x01083204, //CW150
	ITEMID_CandyShield									= 0x02042003, //CS103

	//CandyPerm
	ITEMID_PCandyAxe									= 0x0101350E, //CW189
	ITEMID_PCandyClaw									= 0x01023308, //CW190
	ITEMID_PCandyDagger									= 0x010A3208, //CW191
	ITEMID_PCandyHammer									= 0x01033408, //CW192
	ITEMID_PCandyWand									= 0x0104350C, //CW193
	ITEMID_PCandyPhantom								= 0x01093208, //CW194
	ITEMID_PCandyScythe									= 0x01053409, //CW195
	ITEMID_PCandyBow									= 0x01063308, //CW196
	ITEMID_PCandySword									= 0x0107380F, //CW197
	ITEMID_PCandyJavelin								= 0x01083208, //CW198
	ITEMID_PCandyShield									= 0x02042007, //CS107

	//Obscure
	ITEMID_ObscureAxe									= 0x01013508, //CW151
	ITEMID_ObscureAxe1Hand								= 0x01013509, //CW152
	ITEMID_ObscureClaw									= 0x01023305, //CW153
	ITEMID_ObscureDagger								= 0x010A3205, //CW154
	ITEMID_ObscureHammer								= 0x01033405, //CW155
	ITEMID_ObscureStaff									= 0x01043508, //CW156
	ITEMID_ObscureWand									= 0x01043509, //CW157
	ITEMID_ObscurePhantom								= 0x01093205, //CW158
	ITEMID_ObscureScythe								= 0x01053405, //CW159
	ITEMID_ObscureBow									= 0x01063305, //CW160
	ITEMID_ObscureSword									= 0x01073809, //CW161
	ITEMID_ObscureSword1Hand							= 0x0107380A, //CW162
	ITEMID_ObscureJavelin								= 0x01083205, //CW163
	ITEMID_ObscureShield								= 0x02042004, //CS104

	//Sandurr for now...
	ITEMID_SandurrRedAxe								= 0x0101350A, //CW164
	ITEMID_SandurrRedClaw								= 0x01023306, //CW165
	ITEMID_SandurrRedDagger								= 0x010A3206, //CW166
	ITEMID_SandurrRedHammer								= 0x01033406, //CW167
	ITEMID_SandurrRedStaff								= 0x0104350A, //CW168
	ITEMID_SandurrRedPhantom							= 0x01093206, //CW169
	ITEMID_SandurrRedScythe								= 0x01053406, //CW170
	ITEMID_SandurrRedBow								= 0x01063306, //CW171
	ITEMID_SandurrRedSword								= 0x0107380B, //CW172
	ITEMID_SandurrRedJavelin							= 0x01083206, //CW173
	ITEMID_SandurrRedShield								= 0x02042005, //CS105

	ITEMID_SandurrPurpleAxe								= 0x0101350B, //CW174
	ITEMID_SandurrPurpleClaw							= 0x01023307, //CW175
	ITEMID_SandurrPurpleDagger							= 0x010A3207, //CW176
	ITEMID_SandurrPurpleHammer							= 0x01033407, //CW177
	ITEMID_SandurrPurpleStaff							= 0x0104350B, //CW178
	ITEMID_SandurrPurplePhantom							= 0x01093207, //CW179
	ITEMID_SandurrPurpleScythe							= 0x01053407, //CW180
	ITEMID_SandurrPurpleBow								= 0x01063307, //CW181
	ITEMID_SandurrPurpleSword							= 0x0107380C, //CW182
	ITEMID_SandurrPurpleJavelin							= 0x01083207, //CW183
	ITEMID_SandurrPurpleShield							= 0x02042006, //CS106

	ITEMID_SandurrRedAxe1Hand							= 0x0101350C, //CW184
	ITEMID_SandurrPurpleAxe1Hand						= 0x0101350D, //CW185

	ITEMID_SandurrRedSword1Hand							= 0x0107380D, //CW186
	ITEMID_SandurrPurpleSword1Hand						= 0x0107380E, //CW187

	ITEMID_SandurrPurple2Scythe							= 0x01053408, //CW188

	//Toys
	ITEMID_LoveSword									= 0x01073600, //LI101
	ITEMID_Rose											= 0x01023200, //LI102
	ITEMID_PickAxe										= 0x01013300, //LI103
	ITEMID_Guitar										= 0x01053300, //LI104
	ITEMID_XmasCane										= 0x01013400, //LI105
	ITEMID_TigersClaw									= 0x01023201, //LI106
	ITEMID_PanFun										= 0x01033600, //LI107
	ITEMID_GoldenPanFun									= 0x01033601, //LI108
	ITEMID_GoldenTigersClaw								= 0x01023202, //LI109


	//Axes
	ITEMID_ZecramAxe									= 0x01011600, //WA122
	ITEMID_MythologyAxe									= 0x01012B00, //WA127
	ITEMID_DeathAxe										= 0x01012C00, //WA128
	ITEMID_InfernoAxe									= 0x01012F00, //WA303
	ITEMID_BlazingAxe									= 0x01013000, //WA304
	ITEMID_DemolitionAxe								= 0x01013100, //WA305
	ITEMID_GladiatorAxe									= 0x01013200, //WA306
	ITEMID_ZealousAxe									= 0x01013700, //WA131
	ITEMID_FerociousAxe									= 0x01013800, //WA132
	ITEMID_AbyssAxe										= 0x01013900, //WA133

	//Claws
	ITEMID_InjusticeTalon								= 0x01021600, //WC122
	ITEMID_MythologyTalon								= 0x01022A00, //WC126
	ITEMID_DeathTalon									= 0x01022B00, //WC127
	ITEMID_InfernoClaw									= 0x01022E00, //WC303
	ITEMID_ImperialTalon								= 0x01022F00, //WC304
	ITEMID_RagingClaw									= 0x01023000, //WC305
	ITEMID_AvengerClaw									= 0x01023100, //WC306
	ITEMID_FerociousClaw								= 0x01023500, //WC130
	ITEMID_AbyssClaw									= 0x01023600, //WC131


	//Hammers
	ITEMID_BaneHammer									= 0x01031700, //WH123
	ITEMID_MythologyHammer								= 0x01032C00, //WH128
	ITEMID_DeathHammer									= 0x01032D00, //WH129
	ITEMID_InfernoHammer								= 0x01033000, //WH303
	ITEMID_LegendaryHammer								= 0x01033100, //WH304
	ITEMID_DestructiveHammer							= 0x01033200, //WH305
	ITEMID_MoltenHammer									= 0x01033300, //WH306
	ITEMID_TaurusHammer									= 0x01033700, //WH132
	ITEMID_AbyssHammer									= 0x01033800, //WH133

	//Staffs
	ITEMID_CelestialStaff								= 0x01041700, //WM123
	ITEMID_MythologyStaff								= 0x01042C00, //WM128
	ITEMID_DeathStaff									= 0x01042D00, //WM129
	ITEMID_BlessedStaff									= 0x01043000, //WM303
	ITEMID_MysticStaff									= 0x01043100, //WM304
	ITEMID_BrutalWand									= 0x01043200, //WM305
	ITEMID_TwilightStaff								= 0x01043300, //WM306
	ITEMID_CrystalWand									= 0x01043400, //WM307
	ITEMID_DeviousStaff									= 0x01043700, //WM132
	ITEMID_EternalWand									= 0x01043800, //WM133
	ITEMID_AbyssStaff									= 0x01043900, //WM134


	//Scythes
	ITEMID_ReaperScythe									= 0x01051800, //WP123
	ITEMID_MythologySpear								= 0x01052B00, //WP127
	ITEMID_DiabolicScythe								= 0x01052C00, //WP128
	ITEMID_InfernoScythe								= 0x01052F00, //WP303
	ITEMID_DeathScythe									= 0x01053000, //WP304
	ITEMID_TwinScythe									= 0x01053100, //WP305
	ITEMID_DoubleSidedSpear								= 0x01053200, //WP306
	ITEMID_TormentorScythe								= 0x01053600, //WP131
	ITEMID_AbyssScythe									= 0x01053700, //WP132

	//Bows
	ITEMID_ImmortalBow									= 0x01061800, //WS124
	ITEMID_MythologyBow									= 0x01062C00, //WS128
	ITEMID_DeathBow										= 0x01062D00, //WS129
	ITEMID_EnigmaBow									= 0x01062F00, //WS303
	ITEMID_SupremeBow									= 0x01063000, //WS304
	ITEMID_RogueBow										= 0x01063100, //WS305
	ITEMID_FatalHandCrossBow							= 0x01063200, //WS306
	ITEMID_LuciferBow									= 0x01063500, //WS132
	ITEMID_AbyssBow										= 0x01063600, //WS133

	//Swords
	ITEMID_ExtremeSword									= 0x01071900, //WS225
	ITEMID_MythologySword								= 0x01072E00, //WS230
	ITEMID_DeathSword									= 0x01072F00, //WS231
	ITEMID_InfernoSword									= 0x01073200, //WS403
	ITEMID_ImperialSword								= 0x01073300, //WS404
	ITEMID_EragonSword									= 0x01073400, //WS405
	ITEMID_EmperorSword									= 0x01073500, //WS406
	ITEMID_RelicSword									= 0x01073700, //WS408
	ITEMID_HellbringerSword								= 0x01073A00, //WS234
	ITEMID_FerociousSword								= 0x01073B00, //WS235
	ITEMID_AbyssSword									= 0x01073C00, //WS236

	//Javelins
	ITEMID_HerecticJavelin								= 0x01081700, //WT123
	ITEMID_MythologyJavelin								= 0x01082B00, //WT127
	ITEMID_DeathJavelin									= 0x01082C00, //WT128
	ITEMID_InfernoJavelin								= 0x01082E00, //WT303
	ITEMID_FlameJavelin									= 0x01082F00, //WT304
	ITEMID_BlessedJavelin								= 0x01083000, //WT305
	ITEMID_RoyalJavelin									= 0x01083100, //WT306
	ITEMID_OphidianJavelin								= 0x01083400, //WT131
	ITEMID_AbyssJavelin									= 0x01083500, //WT132

	//Phantoms
	ITEMID_AcalaPhantom									= 0x01091700, //WN123
	ITEMID_MythologyPhantom								= 0x01092A00, //WN126
	ITEMID_DeathPhantom									= 0x01092B00, //WN127
	ITEMID_LutaPhantom									= 0x01092E00, //WN303
	ITEMID_KahsehthaPhantom								= 0x01092F00, //WN304
	ITEMID_NiyahaPhantom								= 0x01093000, //WN305
	ITEMID_WakinyanPhantom								= 0x01093100, //WN306
	ITEMID_PsionicPhantom								= 0x01093400, //WN130
	ITEMID_AbyssPhantom									= 0x01093500, //WN131

	//Daggers
	ITEMID_DevilianDagger								= 0x010A1700, //WD126
	ITEMID_MythologyDagger								= 0x010A2A00, //WD126
	ITEMID_DeathDagger									= 0x010A2B00, //WD127
	ITEMID_InfernoDagger								= 0x010A2E00, //WD303
	ITEMID_EnigmaDagger									= 0x010A2F00, //WD304
	ITEMID_FlamingDagger								= 0x010A3000, //WD305
	ITEMID_VenomousDagger								= 0x010A3100, //WD306
	ITEMID_DeviousDagger								= 0x010A3400, //WD130
	ITEMID_AbyssDagger									= 0x010A3500, //WD131

	//Armors
	ITEMID_FrenzyArmor									= 0x02011800, //DA124
	ITEMID_MythologyArmor								= 0x02012C00, //DA128
	ITEMID_DeathArmor									= 0x02012D00, //DA129
	ITEMID_InfernoArmor									= 0x02012F00, //DA303
	ITEMID_HadesArmor									= 0x02013000, //DA304
	ITEMID_MythrilArmor									= 0x02013100, //DA305
	ITEMID_ImperialArmor								= 0x02013200, //DA306
	ITEMID_QuantumArmor									= 0x02013300, //DA130
	ITEMID_AbyssArmor									= 0x02013400, //DA307

	//Shields
	ITEMID_DreadnaughtShield							= 0x02041600, //DS122
	ITEMID_MythologyShield								= 0x02041900, //DS125
	ITEMID_DeathShield									= 0x02041A00, //DS126
	ITEMID_CrystalShield								= 0x02041C00, //DS303
	ITEMID_RunicShield									= 0x02041D00, //DS304
	ITEMID_AegisShield									= 0x02041E00, //DS305
	ITEMID_ImperialShield								= 0x02041B00, //DS127
	ITEMID_AbyssShield									= 0x02041F00, //DS128

	//Orbs
	ITEMID_AvernusBeads									= 0x03031700, //OM123
	ITEMID_MythologyOrb									= 0x03031A00, //OM126
	ITEMID_DeathOrb										= 0x03031B00, //OM127
	ITEMID_KelvezuClaw									= 0x03031D00, //OM303
	ITEMID_DynastySphere								= 0x03031E00, //OM304
	ITEMID_RunicOrb										= 0x03031F00, //OM305
	ITEMID_AeonSphere									= 0x03032000, //OM306
	ITEMID_SentinelOrb									= 0x03031C00, //OM128
	ITEMID_AbyssOrb										= 0x03032100, //OM129

	//Robes
	ITEMID_EternalRobe									= 0x02051800, //DA224
	ITEMID_MythologyRobe								= 0x02052C00, //DA228
	ITEMID_DeathRobe									= 0x02052D00, //DA229
	ITEMID_InfernoRobe									= 0x02052F00, //DA403
	ITEMID_ElfRobe										= 0x02053000, //DA404
	ITEMID_LunarRobe									= 0x02053100, //DA405
	ITEMID_CherubRobe									= 0x02053200, //DA406
	ITEMID_WarlockRobe									= 0x02053300, //DA230
	ITEMID_AbyssRobe									= 0x02053400, //DA407

	//Boots
	ITEMID_PhoenixBoots									= 0x02021600, //DB122
	ITEMID_DeathBoots									= 0x02021A00, //DB126
	ITEMID_GoldenBoots									= 0x02021C00, //DB303
	ITEMID_HighlanderBoots								= 0x02021D00, //DB304
	ITEMID_EragonBoots									= 0x02021E00, //DB305
	ITEMID_DryadBoots									= 0x02021F00, //DB306
	ITEMID_GhostBoots									= 0x02022000, //DB128
	ITEMID_MercurialBoots								= 0x02022100, //DB129
	ITEMID_AbyssBoots									= 0x02022200, //DB132

	//Gauntlets
	ITEMID_PhoenixGauntlets								= 0x02031600, //DG122
	ITEMID_DeathGauntlets								= 0x02031A00, //DG126
	ITEMID_GoldenGauntlets								= 0x02031C00, //DG303
	ITEMID_HighlanderGauntlets							= 0x02031D00, //DG304
	ITEMID_EragonGauntlets								= 0x02031E00, //DG305
	ITEMID_DryadGauntlets								= 0x02031F00, //DG306
	ITEMID_GhostGauntlets								= 0x02032000, //DG128
	ITEMID_GhostGauntlets111							= 0x02032100, //DG129
	ITEMID_GreedyGauntlets								= 0x02032200, //DG130
	ITEMID_ArgonianGauntlets							= 0x02032300, //DG131
	ITEMID_AbyssGauntlets								= 0x02032400, //DG132

	//Bracelets
	ITEMID_PhoenixBracelets								= 0x03021600, //OA222
	ITEMID_DeathBracelets								= 0x03021A00, //OA226
	ITEMID_GoldenBracelets								= 0x03021C00, //OA303
	ITEMID_HighlanderBracelets							= 0x03021D00, //OA304
	ITEMID_EragonBracelets								= 0x03021E00, //OA305
	ITEMID_DryadBracelets								= 0x03021F00, //OA306
	ITEMID_GhostBracelets								= 0x03022000, //OA228
	ITEMID_TropicalBracelets							= 0x03022100, //OA308
	ITEMID_MercurialBracelets							= 0x03022200, //OA229
	ITEMID_AbyssBracelets								= 0x03022300, //OA232


	//Rings
	ITEMID_SpiritRing									= 0x03041B00, //OR127
	ITEMID_SpiritRing1									= 0x03041C00, //OR128
	ITEMID_SpiritRing2									= 0x03041D00, //OR129
	ITEMID_EnigmaRing									= 0x03042000, //OR132
	ITEMID_ThroneRing									= 0x03042100, //OR250

	//Amulets
	ITEMID_SpiritAmulet									= 0x03011B00, //OA127
	ITEMID_SpiritAmulet1								= 0x03011C00, //OA128
	ITEMID_EnigmaAmulet									= 0x03012000, //OA132
};

enum EItemType : unsigned int
{
	ITEMTYPE_Any			= 0xFFFFFFFF,
	ITEMTYPE_None			= 0x00000000,

	/* Weapons */
	ITEMTYPE_Axe			= 0x01010000, //WA
	ITEMTYPE_Claw			= 0x01020000, //WC
	ITEMTYPE_Hammer			= 0x01030000, //WH
	ITEMTYPE_Wand			= 0x01040000, //WM
	ITEMTYPE_Scythe			= 0x01050000, //WP
	ITEMTYPE_Bow			= 0x01060000, //WS1
	ITEMTYPE_Sword			= 0x01070000, //WS2
	ITEMTYPE_Javelin		= 0x01080000, //WT
	ITEMTYPE_Phantom		= 0x01090000, //WN
	ITEMTYPE_Dagger			= 0x010A0000, //WD

	/* Defenses */
	ITEMTYPE_Armor			= 0x02010000, //DA1
	ITEMTYPE_Costume		= 0x02100000, //CA
	ITEMTYPE_Costume2		= 0x02110000, //CA
	ITEMTYPE_Costume3		= 0x02060000, //CA
	ITEMTYPE_Costume4		= 0x02070000, //CA
	ITEMTYPE_Boots			= 0x02020000, //DB
	ITEMTYPE_Gauntlets		= 0x02030000, //DG
	ITEMTYPE_Shield			= 0x02040000, //DS
	ITEMTYPE_Robe			= 0x02050000, //DA2
	ITEMTYPE_Sheltom		= 0x02350000, //OS

	/* Accessories */
	ITEMTYPE_Amulet			= 0x03010000, //OA1
	ITEMTYPE_Bracelets		= 0x03020000, //OA2
	ITEMTYPE_Orb			= 0x03030000, //OM
	ITEMTYPE_Ring			= 0x03040000, //OR1
	ITEMTYPE_ForceOrb		= 0x03060000, //FO
	ITEMTYPE_Respec			= 0x03070000, //SE or JV
	ITEMTYPE_Smelting		= 0x03090000, //PR2
	ITEMTYPE_Crafting		= 0x03100000, //PR3
	ITEMTYPE_Crafting2		= 0x03110000, //PR4
	ITEMTYPE_EventPR5		= 0x03130000, //PR5
	ITEMTYPE_Ring2			= 0x03050000, //OR1
	ITEMTYPE_Earring		= 0x03140000, //OE1


	/* Potions */
	ITEMTYPE_ManaPotion		= 0x04010000, //PM
	ITEMTYPE_HealthPotion	= 0x04020000, //PL
	ITEMTYPE_StaminaPotion	= 0x04030000, //PS

	/* Special */
	ITEMTYPE_GoldAndExp		= 0x05010000, //GG
	ITEMTYPE_BellatoStone	= 0x05020000, //BS

	/* Other */
	ITEMTYPE_EtherCore		= 0x06010000, //EC

	/* Quest */
	ITEMTYPE_Quest			= 0x07010000, //QT
	ITEMTYPE_MorionT2Quest	= 0x09020000, 

	/* Event */
	ITEMTYPE_Cookie			= 0x08010000, 
	ITEMTYPE_MonsterCrystal	= 0x08020000, //GP
	ITEMTYPE_Wing			= 0x08030000, //QW
	ITEMTYPE_Gift			= 0x08040000, 
	ITEMTYPE_Puzzle1		= 0x08060000, 
	ITEMTYPE_Puzzle2		= 0x08070000, 
	ITEMTYPE_Chocolate		= 0x08080000, 
	ITEMTYPE_Bellatra		= 0x08090000, //SD2
	ITEMTYPE_BlessCastle	= 0x080A0000, //BC
	ITEMTYPE_Premium1		= 0x080B0000, //BI1
	ITEMTYPE_Premium2		= 0x080C0000, //BI2
	ITEMTYPE_Puzzle3		= 0x080D0000, 
	ITEMTYPE_Puzzle4		= 0x080E0000, 
	ITEMTYPE_Premium3		= 0x080F0000, 
	ITEMTYPE_Premium4		= 0x08100000, 
	ITEMTYPE_Premium5		= 0x08110000, 

	/* Custom */
	ITEMTYPE_MixStone		= 0x08A00000, //RS
	ITEMTYPE_MatureStone	= 0x08B00000, //MS
	ITEMTYPE_Crystal		= 0x08C00000, //SC
	ITEMTYPE_Bars			= 0x08D00000, //GB
	ITEMTYPE_PetEggs		= 0x08E00000, //PI
	ITEMTYPE_Event			= 0x08F00000, //EV
};

enum EItemBase : unsigned int
{
	ITEMBASE_Any			= 0xFFFFFFFF,
	ITEMBASE_None			= 0x00000000,

	ITEMBASE_Weapon			= 0x01000000, //W
	ITEMBASE_Defense		= 0x02000000, //D
	ITEMBASE_Accessory		= 0x03000000, //O
	ITEMBASE_Potion			= 0x04000000, //P
	ITEMBASE_Special		= 0x05000000, //G
	ITEMBASE_Core			= 0x06000000, //E
	ITEMBASE_Quest1			= 0x07000000, //Q
	ITEMBASE_Premium		= 0x08000000, //BI
	ITEMBASE_Quest2			= 0x09000000,
	ITEMBASE_Recipe			= 0x0A000000, //SD
};

enum EItemCraftType : int
{
	ITEMCRAFTTYPE_NPC				= -1,
	ITEMCRAFTTYPE_None				= 0,
	ITEMCRAFTTYPE_Mixing			= 1,
	ITEMCRAFTTYPE_Aging				= 2,
	ITEMCRAFTTYPE_Quest				= 3,
	ITEMCRAFTTYPE_Event				= 4,
	ITEMCRAFTTYPE_Bottle			= 5,
	ITEMCRAFTTYPE_BottleElement		= 6,
	ITEMCRAFTTYPE_QuestWeapon		= 7,
	ITEMCRAFTTYPE_Special			= 8,
	ITEMCRAFTTYPE_TGFKingFury		= 9,
	ITEMCRAFTTYPE_TGFLordBabel		= 10,
};

enum EMixTypeName : unsigned int
{
	MIXTYPENAME_None,

	MIXTYPENAME_Devine,
	MIXTYPENAME_Celesto,
	MIXTYPENAME_Mirage,
	MIXTYPENAME_Inferna,
	MIXTYPENAME_Enigma,
	MIXTYPENAME_Bellum,
	MIXTYPENAME_Oredo,

	MIXTYPENAME_NextMixFlag = 50,
	MIXTYPENAME_Devine2,
	MIXTYPENAME_Celesto2,
	MIXTYPENAME_Mirage2,
	MIXTYPENAME_Inferna2,
	MIXTYPENAME_Enigma2,
	MIXTYPENAME_Bellum2,
	MIXTYPENAME_Oredo2,

	MIXTYPENAME_OredoSwiftness = 150,
	MIXTYPENAME_OredoPower,
	MIXTYPENAME_OredoCritical,
	MIXTYPENAME_OredoDefense,
	MIXTYPENAME_OredoBlock,
	MIXTYPENAME_OredoRating,
	MIXTYPENAME_OredoVitality,
	MIXTYPENAME_OredoVelocity,
	MIXTYPENAME_OredoEfficiency1,
	MIXTYPENAME_OredoEfficiency2,

	MIXTYPENAME_NextMixFlag2 = 560,
	MIXTYPENAME_BellumDefense,
	MIXTYPENAME_BellumBlock,
	MIXTYPENAME_BellumVelocity,
	MIXTYPENAME_BellumPower,
	MIXTYPENAME_BellumCritical,
	MIXTYPENAME_BellumRating,
	MIXTYPENAME_BellumVitality,

};

enum EMixAttributeType
{
	MIXATTRTYPE_Fire			= 1,
	MIXATTRTYPE_Ice				= 2,
	MIXATTRTYPE_Lightning		= 4,
	MIXATTRTYPE_Poison			= 8,
	MIXATTRTYPE_Organic			= 16,
	MIXATTRTYPE_Critical		= 32,
	MIXATTRTYPE_AttackRating	= 64,
	MIXATTRTYPE_DamageMin		= 128,
	MIXATTRTYPE_DamageMax		= 256,
	MIXATTRTYPE_AttackSpeed		= 512,
	MIXATTRTYPE_Absorption		= 1024,
	MIXATTRTYPE_Defense			= 2048,
	MIXATTRTYPE_BlockRating		= 4096,
	MIXATTRTYPE_WalkSpeed		= 8192,
	MIXATTRTYPE_HP				= 16384,
	MIXATTRTYPE_MP				= 32768,
	MIXATTRTYPE_SP				= 65536,
	MIXATTRTYPE_HPRegen			= 131072,
	MIXATTRTYPE_MPRegen			= 262144,
	MIXATTRTYPE_SPRegen			= 524288,
	MIXATTRTYPE_PotionCount		= 1048576,


};

enum EItemSlotFlag : unsigned int
{
	ITEMSLOTFLAG_None					= 0,
	ITEMSLOTFLAG_Inventory				= 1 << 0,					//1
	ITEMSLOTFLAG_LeftHand				= 1 << 1,					//2
	ITEMSLOTFLAG_RightHand				= 1 << 2,					//4
	ITEMSLOTFLAG_LeftRightHand			= (1 << 2) | (1 << 1),		//6
	ITEMSLOTFLAG_Armor					= 1 << 3,					//8
	ITEMSLOTFLAG_Boots					= 1 << 4,					//16
	ITEMSLOTFLAG_Gauntlets				= 1 << 5,					//32
	ITEMSLOTFLAG_RingLeft				= 1 << 6,					//64
	ITEMSLOTFLAG_RingRight				= 1 << 7,					//128
	ITEMSLOTFLAG_Sheltom				= 1 << 8,					//256
	ITEMSLOTFLAG_Amulet					= 1 << 9,					//512
	ITEMSLOTFLAG_Bracelets				= 1 << 11,					//2048
	ITEMSLOTFLAG_Potion					= 1 << 13,					//8192
	ITEMSLOTFLAG_Costume				= 1 << 14,					//16384
	ITEMSLOTFLAG_CostumeLeft			= 1 << 15,					//32768
	ITEMSLOTFLAG_CostumeRight			= 1 << 16,					//65536
	ITEMSLOTFLAG_CostumeLeftRight		= (1 << 15) | (1 << 16),	//98304
	ITEMSLOTFLAG_Earrings				= (1 << 17),				//131072
};

enum EMixEffect : unsigned int
{
	MIXEFFECT_None				= 0x00000000,
	MIXEFFECT_Flame				= 0x00000001,
	MIXEFFECT_Frost				= 0x00000002,
	MIXEFFECT_Light				= 0x00000004,
	MIXEFFECT_Poison			= 0x00000008,
	MIXEFFECT_Organic			= 0x00000010,
	MIXEFFECT_Critical			= 0x00000020,
	MIXEFFECT_AttackRating		= 0x00000040,
	MIXEFFECT_MinAttackPower	= 0x00000080,
	MIXEFFECT_MaxAttackPower	= 0x00000100,
	MIXEFFECT_WeaponSpeed		= 0x00000200,
	MIXEFFECT_AbsorbRating		= 0x00000400,
	MIXEFFECT_DefenseRating		= 0x00000800,
	MIXEFFECT_Block				= 0x00001000,
	MIXEFFECT_WalkSpeed			= 0x00002000,
	MIXEFFECT_HP				= 0x00004000,
	MIXEFFECT_MP				= 0x00008000,
	MIXEFFECT_SP				= 0x00010000,
	MIXEFFECT_HPRegen			= 0x00020000,
	MIXEFFECT_MPRegen			= 0x00040000,
	MIXEFFECT_SPRegen			= 0x00080000,
	MIXEFFECT_PotionCount		= 0x00100000,
};

enum EItemInvSound
{
	ITEMINVSOUND_None			= 0,
	ITEMINVSOUND_Axe			= 1,
	ITEMINVSOUND_Claw			= 2,
	ITEMINVSOUND_Armor			= 9,
};

enum EItemSlot : int
{
	ITEMSLOT_None				= 0,

	ITEMSLOT_RightHand			= 1,
	ITEMSLOT_LeftHand			= 2,
	ITEMSLOT_Armor				= 3,
	ITEMSLOT_Amulet				= 4,
	ITEMSLOT_RingRight			= 5,
	ITEMSLOT_RingLeft			= 6,
	ITEMSLOT_Sheltom			= 7,
	ITEMSLOT_Bracelets			= 8,
	ITEMSLOT_Gauntlets			= 9,
	ITEMSLOT_Boots				= 10,
	ITEMSLOT_PotionOne			= 11,
	ITEMSLOT_PotionTwo			= 12,
	ITEMSLOT_PotionThree		= 13,
	ITEMSLOT_Costume			= 14,
};

enum ESocketType : unsigned char
{
	SOCKETTYPE_None,

	SOCKETTYPE_Prismatic,
	SOCKETTYPE_Emerald,
	SOCKETTYPE_Ruby,
	SOCKETTYPE_Aquamarine,
	SOCKETTYPE_Amethyst,
	SOCKETTYPE_Topaz,

	SOCKETTYPE_Count = 7,
};

enum EGemType : unsigned char
{
	GEMTYPE_None,

	GEMTYPE_Triangular,
	GEMTYPE_Square,
	GEMTYPE_Marquise,
};

enum EGemItem : unsigned char
{
	GEMITEM_None,

	/*Normal Gems*/
	GEMITEM_Emerald,
	GEMITEM_Ruby,
	GEMITEM_Aquamarine,
	GEMITEM_Amethyst,
	GEMITEM_Topaz,

	/*Special Gems*/
	GEMITEM_RainbowOpal,
	GEMITEM_DemonSlayer,
	GEMITEM_BloodyPerl,
	GEMITEM_NavyPerl,
	GEMITEM_TempskronMartyr,
	GEMITEM_MorionMartyr,
	GEMITEM_RoyalDiamond,
	GEMITEM_PolishedCitrine,
	GEMITEM_ElementalTourmaline,
	GEMITEM_DivineHeart,
	GEMITEM_LuckyCirclet,
};

enum EGemSpecial
{
	GEMSPECIAL_None,
	GEMSPECIAL_WPNMaxAtkPowAdd1,				//+1 Max Atk Pow on Weapon
	GEMSPECIAL_WPNMaxAtkPowAdd2,				//+2 Max Atk Pow on Weapon
	GEMSPECIAL_WPNReqDec5PercentStrenght,		//-5% Strenght on Weapon
	GEMSPECIAL_WPNAtkPowAdd1,					//+1 Min & Max Atk Pow on Weapon
	GEMSPECIAL_HPAdd10,							//+10 HP
	GEMSPECIAL_WeightAdd100,					//+100 iWeight
	GEMSPECIAL_AbsorbAdd5,						//+5 fAbsorbRating
	GEMSPECIAL_BlockAdd10Percent,				//+10% Block
	GEMSPECIAL_WPNReqDec5PercentSpirit,			//-5% Spirit
	GEMSPECIAL_HPRegen1,						//1.0 Regen HP
	GEMSPECIAL_FireResistance10,				//+10 Fire saElementalDef
	GEMSPECIAL_MPAdd100,						//+100 MP
	GEMSPECIAL_MPRegen2,						//2.0 Regen MP
	GEMSPECIAL_SPAdd200,						//+200 SP
	GEMSPECIAL_IceResistance10,					//+10 Ice saElementalDef
	GEMSPECIAL_WPNReqDec5PercentTalent,			//-5% Talent
	GEMSPECIAL_AtkRtgAdd20,						//+20 Attack Rating
	GEMSPECIAL_DefenseAdd30,					//+30 Defense
	GEMSPECIAL_WPNRangeAdd10,					//+10 Range on Weapon
	GEMSPECIAL_OrganicResistanceAdd10,			//+10 Organic saElementalDef
	GEMSPECIAL_WPNReqDec5PercentAgility,		//-5% Agility on Weapon
	GEMSPECIAL_AtkRtgAdd30,						//+30 Attack Rating
	GEMSPECIAL_DefenseAdd40,					//+40 Defense
	GEMSPECIAL_RunSpeedAdd1,					//+1 Run fMovementSpeed
	GEMSPECIAL_LightningResistanceAdd10,		//+10 Lightning saElementalDef
	GEMSPECIAL_BlockAdd1Percent,				//+1% Block
	GEMSPECIAL_HPAdd25,							//+25 HP
	GEMSPECIAL_AbsorbAdd8,						//+8 fAbsorbRating
	GEMSPECIAL_AllStatsDec2Percent,				//-2% All Stats
	GEMSPECIAL_HPRegen2,						//2.0 HP Regen
	GEMSPECIAL_DefenseAdd50,					//+50 Defense
	GEMSPECIAL_PoisonResistanceAdd10,			//+10 Poison saElementalDef
};

struct AgingColorStructure
{
	short								sRed;
	short								sGreen;
	short								sBlue;
	short								sAlpha;
	short								iUnk;
	short								sSpeed;
};

static AgingColorStructure saAgingColorTable[MAX_AGING - 3]
{
	{ 100,	 40,	 90,	0,	 -1,	0	},		// 4
	{ 120,	100,	 10,	0,	 -1,	0	},		// 5
	{   0,	110,	 30,	0,	 -1,	0	},		// 6
	{   0,	 50,	140,	0,	 -1,	0	},		// 7
	{ 100,	  0,	 90,	0,	 -1,	0	},		// 8
	{ 150,	 60,	  0,	0,	 -1,	0	},		// 9
	{ 150,	 10,	  0,	0,	 -1,	0	},		// 10
	{  20,	220,	190,	0,	 -1,	0	},		// 11
	{  10,	220,	 30,	0,	 -1,	0	},		// 12
	{ 170,	 40,	170,	0,	 -1,	0	},		// 13
	{ 250,	 30,	160,	0,	 -1,	0	},		// 14
	{  30,	190,	255,	0,	 -1,	0	},		// 15
	{ 250,	130,	 30,	0,	 -1,	0	},		// 16
	{ 120,	 30,	 30,	0,	 -1,	0	},		// 17
	{ 130,	  0,	255,	0,	 -1,	0	},		// 18
	{ 255,	255,	 0,		0,	 -1,	0	},		// 19
	{ 255,	255,	255,	0,	 -1,	0	},		// 20
	{ 1024,	256,	256,	0,	 2,		14	},		// 21
	{ 256,	256,	640,	0,	 2,		14	},		// 22
	{ 1024,	256,	256,	0,	 2,		14	},		// 23
	{ 256,	256,	640,	0,	 2,		14	},		// 24
};

static AgingColorStructure saMixingColorTable[10]
{
	{ 13,	 0,	 5,		0,	  9,	8	},		// 4
	{ 13,	 0,	 6,		0,	  5,	8	},		// 5
	{ 13,	 0,	 7,		0,	  6,	9	},		// 6
	{ 13,	 0,	 8,		0,	  7,	9	},		// 7
	{ 13,	 0,	 9,		0,	  8,	9	},		// 8
	{ 13,	 0,	 10,	0,	  9,	9	},		// 9  
	{ 13,	 0,	 11,	0,	  0,	9	},		// 11 // 
	{ 1,	 0,	 1,		0,	  12,	9 },		// 12 // Enigma
	{ 2,	 0,	 2,		0,	  13,	9 },		// 13 // Bellum
	{ 3,	 0,	 2,		0,	  14,	9 },		// 13 // Oredo

};


struct SocketItem
{
	ESocketType 	eSocketType;
	EGemType		eGemType;
	EGemItem		eGemItem;
};

struct SpecItemData
{
	float									fSpecAbsorbRating;
	int										iSpecDefenseRating;
	float									fSpecMovementSpeed;
	float									fSpecBlockRating;
	int										iSpecAttackSpeed;
	int										iSpecCritical;
	int										iSpecAttackRange;
	float									fSpecMagicMastery;
	short									saSpecElementalDef[8];
	short									saSpecElementalAtkDiv[8];
	int										iSpecAddMPDiv;
	int										iSpecAddHPDiv;
	int										iSpecAttackRatingDiv;
	short									iSpecAttackPowerDivMin;
	short									iSpecAttackPowerDiv;
	float									fSpecMPRegen;
	float									fSpecHPRegen;
	float									fSpecSPRegen;
	int										iSpecEvade;

	short									sSpecPenetration;
	short									sSpecHPPotionEfficience;
	short									sSpecMPPotionEfficience;
	short									sSpecSPPotionEfficience;


	int										iPadding99[29];
};

class ItemID
{
public:
	ItemID() {};
	ItemID( int i ) { *(int *)(this) = i; }

	char									iIDHigh;	//always zero
	char									iIDLow;		//++ counter
	short									sItemType;

	inline unsigned int						ToInt() { return *(unsigned int *)(this); }
	inline EItemID							ToItemID() { return *(EItemID*)(this); }
	inline EItemType						ToItemType() { return (EItemType)(*(unsigned int *)(this) & 0xFFFF0000); }
	inline EItemBase						ToItemBase() { return (EItemBase)(*(unsigned int *)(this) & 0xFF000000); }

	inline void								SetItemID( EItemID iItemID ) { *(EItemID*)this = iItemID; }

	static inline ItemID					* ToItemID( EItemID * i ) { return (ItemID *)i; }
};

struct Item
{
	//Attributes
	DWORD									dwSize;
	ITEMCHECKSUM							iChk1;
	DWORD									dwVersion;
	DWORD									dwTime;
	ITEMCHECKSUM							iChk2;

	//Statistics
	CurMax									sIntegrity;
	ItemID									sItemID;
	char									szItemName[32];
	int										iWeight;
	int										iSalePrice;
	int										iIndex;
	int										iPotionCount;
	short									saElementalDef[8];
	int										iSight;
	int										iUnknown02;
	MinMax									sDamage;
	int										iAttackRange;
	int										iAttackSpeed;
	int										iAttackRating;
	int										iCritical;
	float									fAbsorbRating;
	int										iDefenseRating;
	float									fBlockRating;
	float									fMovementSpeed;
	int										iPotionStorage;
	float									fMagicMastery;
	float									fMPRegen;
	float									fHPRegen;
	float									fSPRegen;
	float									fAddHP;
	float									fAddMP;
	float									fAddSP;
	int										iLevel;
	int										iStrength;
	int										iSpirit;
	int										iTalent;
	int										iAgility;
	int										iHealth;
	MinMax									sMPRecovery;
	MinMax									sHPRecovery;
	MinMax									sSPRecovery;
	int										iGold;
		
	BOOL									bCanNotUse;

	DWORD									iBackupKey;
	DWORD									iBackupChk;

	short									iScaleBlink[2];
	int										iItemUniqueID;
	short									sEffectBlink[2];
	short									sEffectColor[4];
	BOOL									bEnableEffect;

	int										eSpecialization;
	SpecItemData							sSpecData;

	EItemCraftType							eCraftType;
	int										eMixEffect;
	union
	{
		USHORT								sAgeLevel;
		USHORT								sMixID;
	};
	USHORT									bMaturing;
	UCurMax									sMatureBar;
	
	int										iUnknown;

	short									sSpecialItemType[2];

	DWORD									tTime;
	SocketItem								sSocketData[2];

	BYTE									bCostumeItem;
	BYTE									bEvade;
	BYTE									bLockedItemType;
	char									cPadding[11];

	CurMax									sQuestMonsters;

	EMixTypeName							eMixTypeName;
	BOOL									bPerfectItem;

	int										iPadding666;
};

class ItemData
{
public:
	ItemID									sBaseItemID;
	char									szBaseName[32];
	char									szInventoryName[16];
	int										iWidth;
	int										iHeight;
	char									szCategory[64];
	EItemSlotFlag							iItemSlotFlag;
	char									szModelName[64];
	EItemSlotFlag							iVisibleItemType;
	EItemInvSound							iInvItemSound;
	int										iItemWpnDmg;
	BOOL									bValid;
	Point2D									sPosition;
	Point2D									sPlacePosition;
	void									* pcInventoryTexture;
	int										iItemSlot;
	int										iPotionCount;
	int										iNotUse;
	int										iSalePrice;
	Point2D									sOldPosition;
	void									* pcTempInventoryTexture;
	Item								sItem;

	BOOL									IsOneHandedWeapon();
	BOOL									IsTwoHandedWeapon();
};

struct DefinitionItem
{
	struct Item								sItem; // 0x0
	short									IntegrityMin; // 0x214
	short									IntegrityMax; // 0x216
	short									OrganicMin; // 0x218
	short									OrganicMax; // 0x21A
	short									UnknownResistanseMin; // 0x21C
	short									UnknownResistanseMax; // 0x21E
	short									FireMin; // 0x220
	short									FireMax; // 0x222
	short									FrostMin; // 0x224
	short									FrostMax; // 0x226
	short									LightningMin; // 0x228
	short									LightningMax; // 0x22A
	short									PoisonMin; // 0x22C
	short									PoisonMax; // 0x22E
	short									sUnknownResistance[4]; // 0x230
	short									AttackPower1Min; // 0x238
	short									AttackPower2Min; // 0x23A
	short									AttackPower1Max; // 0x23C
	short									AttackPower2Max; // 0x23E
	short									AttackRatingMin; // 0x240
	short									AttackRatingMax; // 0x242
	short									DefenseMin; // 0x244
	short									DefenseMax; // 0x246
	float									BlockRatingMin; // 0x248
	float									BlockRatingMax; // 0x24C
	float									AbsorbMin; // 0x250
	float									AbsorbMax; // 0x254
	float									RunSpeedMin; // 0x258
	float									RunSpeedMax; // 0x25C
	int										AddHPMin; // 0x260
	int										AddHPMax; // 0x264
	int										AddMPMin; // 0x268
	int										AddMPMax; // 0x26C
	int										AddSTMMin; // 0x270
	int										AddSTMMax; // 0x274
	float									MPRegenMin; // 0x278
	float									MPRegenMax; // 0x27C
	float									HPRegenMin; // 0x280
	float									HPRegenMax; // 0x284
	float									STMRegenMin; // 0x288
	float									STMRegenMax; // 0x28C
	short									AddSpecDefenseMin; // 0x290
	short									AddSpecDefenseMax; // 0x292
	float									AddSpecAbsorbMin; // 0x294
	float									AddSpecAbsorbMax; // 0x298
	float									AddSpecRunSpeedMin; // 0x29C
	float									AddSpecRunSpeedMax; // 0x2A0
	float									SpecialMagicMastery[0x2]; // 0x2A4
	float									AddSpecMPRegenMin; // 0x2AC
	float									AddSpecMPRegenMax; // 0x2B0
	int										SpecAttackRatingLevelMin; // 0x2B4
	int										SpecAttackRatingLevelMax; // 0x2B8
	DWORD									JobBitCodeRandom[0xC]; // 0x2BC
	int										JobBitCodeRandomCount; // 0x2EC
	short									sGenDay[0x2]; // 0x2F0
	unsigned char *							DefCompressData[0x214]; // 0x2F4
	int										DefCompressDataLen; // 0xB44
};

struct DefinitionItemEx
{
	EItemID									eItemID;

	MinMax									sSpecPenetration;
	MinMax									sSpecHPPotionEfficience;
	MinMax									sSpecMPPotionEfficience;
	MinMax									sSpecSPPotionEfficience;
};

struct DropSettingsData
{
	int					iItemID;	//0x00
	int					iPercent;	//0x04
	short				sGoldMin;	//0x08
	short				sGoldMax;	//0x0A
};

struct DropItemData
{
	int										iItemID;
	int										iChk1;
	DWORD									iChk2;
};

struct ItemBoxItem
{
	BOOL									bValid;

	int										iID;

	char									szSenderName[32];

	BOOL									bItem;
	SYSTEMTIME								sDate;

	int										iItemID;
	int										iSpecialization;

	Item									sItem;
};

struct ItemBoxSlot
{
	int										iNumItems;
	ItemBoxItem								saItemBox[MAX_ITEMSINITEMBOX];
};

struct RecordItem
{
	int			iItemCount;

	Point2D		sItemPosition;
	int			iItemPosition;

	Item	sItem;
};

struct InventoryItemData
{
	int				iPosition;
	Rectangle2D		sBox;
	int				iItemIndex;
	int				iWeaponClass;
};

struct SellItemShopData
{
	EItemID				eItemID;
	DWORD				dwHead;
	DWORD				dwChecksum;
	int					iPrice;
};

struct ItemLoadData
{
	BOOL				bState;
	DWORD				dwSerial;
	Item				sItem;
};

struct RespecStone
{
	EItemID				eItemID;
	int					iMinLevel;
	int					iMaxLevel;
	int					iRequiredStones;
	int					iPrice;
};

struct PriceItem
{
	int					iRealPrice;
	int					iPrice;
	int					iRepairPrice;
};


struct SocketItemData
{
	BOOL					bOpenNPC;
	ItemData				sItemData;
	ItemData				sRune[5];
	ItemData				sStone;
	int						iSocketWorkType;
	char					cPadding[32];
	int						iItemCounter;
};

struct ItemLogBox
{
	int										iSize;
	int										iID;
	struct SPlayer
	{
		char								szAccountName[32];
		char								szCharacterName[32];
		IN_ADDR								iIP;
	};

	struct SItem
	{
		int									iItemID;
		int									iAmount;
		int									iChecksum1;
		int									iChecksum2;
	};

	int										iActionCode;

	SPlayer									sPlayer;
	SItem									sItem;
};

struct CompressedItem
{
	ItemID									sItemID;
	char									szBaseName[32];
	char									szInventoryName[16];
	int										iWidth;
	int										iHeight;
	char									szDropFolder[16];
	UINT									iClass;
	char									szDropName[16];
	UINT									iModelPosition;
	UINT									iSound;
	UINT									iWeaponClass;
};

struct DropItemView
{
	BOOL									bFormatted;
	BOOL									bVisible;
	BOOL									bActive;

	Point3D									sPosition;

	char									szModelName[64];
	char									szName[64];

	int										iClass;

	class PTModel							* pcModel;
	struct EXEModelData						* psModelData;

	BOOL									bHidden;

	int										iItemID;

	DWORD									dwLastUpdate;

	int										iServerID;

	Point3D									sRenderPosition;
	Point3D									sSelfPosition;
	Point3D									sAngle;
};

struct ItemInventoryData
{
	int										iItemSlot;
	Rectangle2D								sBox;
	int										iItemIndex;
	int										iWeaponClass;
};

#define ITEM_PERSONALSHOP_MAX	32
#define ITEM_PERSONALSHOP_QUEUE 8

struct PacketPersonalBotShopData : Packet
{
	char									szMessage[256];
	ItemData								cItemData[ITEM_PERSONALSHOP_MAX];
	int										iPrice[ITEM_PERSONALSHOP_MAX];
	int										iaItems[3];
};

struct PricePersonalShopData
{
	ItemID				sItemID;
	int					iChk1;
	int					iChk2;

	int					iPrice;
	bool				bCanBuy;
	bool				bCredits;
	
	int					iCount;
};

struct PacketPersonalShopItemList : Packet
{
	int						iSenderID;
	int						iRequesterID;

	int						iItemCount;
	ItemData				caItemData[ITEM_PERSONALSHOP_QUEUE];
	PricePersonalShopData	saItemPrice[ITEM_PERSONALSHOP_QUEUE];

	short					sPacketNumber;
	short					sPacketMax;

	BOOL					IsFirst() { return sPacketNumber == 1 ? TRUE : FALSE; }
	BOOL					IsOnly() { return sPacketNumber == sPacketMax ? TRUE : FALSE; }
};

struct PacketPersonalShopTradeItems : Packet
{
	int						iSellerID;
	int						iBuyerID;

	bool					bBuy;

	PricePersonalShopData	sItemPrice;

	ItemData				cItemData;

	int						iItemCount;
};

struct PacketPersonalShopRequestView : Packet
{
	PacketPersonalShopRequestView( int _iSenderID, int _iRequesterID ) { iSenderID = _iSenderID; iRequesterID = _iRequesterID; };
	~PacketPersonalShopRequestView() {};
	int						iSenderID;
	int						iRequesterID;
};

enum ESlotTypeHandle
{
	ITEMSLOTHANDLE_Get,
	ITEMSLOTHANDLE_Put,
};
enum ESlotTypeHandleWhere
{
	ITEMSLOTHANDLEWHERE_Perfectize,
	ITEMSLOTHANDLEWHERE_Swap,
};

struct PacketSlotItemHandle : Packet
{

	ESlotTypeHandle			iTypeHandle;
	ESlotTypeHandleWhere	iTypeWhere;

	int					iItemID;

	int					iChk1;
	int					iChk2;
};

struct NPCItemBoxMessageBox
{
	BOOL				bActive;
	ItemData			cItemData;
	char				szMessage[128];
	char				szItemName[64];

	BOOL				bYesButtonHover;
	BOOL				bNoButtonHover;

	DWORD				dwReserved[4];
};

struct PacketNPCItemBox : Packet
{
	char					szSenderName[32];
	SYSTEMTIME				sDate;

	union
	{
		struct
		{
			LARGE_INTEGER	liEXP;
		} EXP;

		struct
		{
			int				iValue;
			int				iReserved;
		} Gold;
	};

	short					sWeight;

	BOOL					bItem;
	BOOL					bAccepted;

	ItemData				cItemData;
};

#define MAX_ITEMBOX_SEND			25
#define MAX_ITEMBOX_SEND_QUEUE		8

struct PacketNPCItemBoxSend : Packet
{
	char					szCharacterName[32];

	int						iCount;
	ItemData				cItemData[MAX_ITEMBOX_SEND_QUEUE];
};

struct PacketNPCItemBoxSendResponse : Packet
{
	int						iResult;

	int						iCount;

	int						iaItemID[MAX_ITEMBOX_SEND_QUEUE];
	int						iaChk1[MAX_ITEMBOX_SEND_QUEUE];
	int						iaChk2[MAX_ITEMBOX_SEND_QUEUE];
};

struct PacketMixServer : Packet
{
	int										iDocIndex;
	int										iResult;
	ItemData								sItemData;
	DWORD									dwCode[12];
	DWORD									dwHead[12];
	DWORD									dwCheckSum[12];
	int										iIndex;
	int										iGold;

	DWORD									dwStoneItemCode;
	DWORD									dwStoneHead;
	DWORD									dwStoneCheckSum;
};

struct PacketItemData : Packet
{
	int										iItemIndex;
	ItemData								sItem;
};

struct PacketUseUnionCore : Packet
{
	Item								sItem;
	Point3D									sPosition;
};

struct PacketThrowItem : Packet
{
	Item								sItem;
	Point3D									sPosition;
	DWORD									dwSecCode[4];
};

struct PacketResetItem : Packet
{
	int										iDocIndex;
	int										iResult;
	int										iIndex;

	ItemData								sItemData;

	DWORD									dwCode;
	DWORD									dwHead;
	DWORD									dwCheckSum;
};

struct PacketQuestNPCItemList : Packet
{
	int										iID;

	int										iIndex;

	class ItemData							cItemData;

	BOOL									bLast;
};

#define MAX_QUESTDYNAMICMENU			30

struct PacketDynamicQuestOpen : Packet
{
	char									szImagePath[260];

	char									szMainText[4096];

	int										iQuestCount;

	struct QuestData
	{
		int									iQuestID;
		char								szQuestName[64];
		IMinMax								sRequiredLevel;

		bool								bDone;
	};

	QuestData								saQuestData[MAX_QUESTDYNAMICMENU];
};

struct PacketFuryArenaItemData : Packet
{
	ItemData								sItemData;
};

struct PacketSocketSystem : Packet
{
	ItemData								sItemData;
	ItemData								sRune[5];
	ItemData								sItemStone;
	int										iSocketWorkType; //0x0E = Drill, 0x0F = Stone
}; //sSize = 0x1598

struct PacketItemPerfectView : Packet
{
	ItemData								cItemData;
};

struct PacketItem : Packet
{
	struct Item							sItem;
	int										iX;
	int										iY;
	int										iZ;
	DWORD									dwSecCode[4];
};

struct PacketManufactureItem : Packet
{
	int								iUunknown;
	int								iResult;
	ItemData						sItemData;
	EItemID							iRecipeID;
	EItemID							iaRuneID[3];
	int								iRecipeChk1;
	int								iaChk1[3];
	int								iRecipeChk2;
	int								iaChk2[3];
	int								iIndex;
	int								iGold;

	DWORD							iExStoneItemCode;
	DWORD							iExStoneHead;
	DWORD							iExStoneCheckSum;
};

struct PacketAgingItem : Packet
{
	int						DocIndex;
	int						Result;
	ItemData				sItem;
	DWORD					SheltomCode[12];
	DWORD					Head[12];
	DWORD					CheckSum[12];
	int						Index;
	int						Money;

	DWORD					StoneItemCode;
	DWORD					StoneHead;
	DWORD					StoneCheckSum;
};

struct PacketAgingUpgradeItem : Packet
{
	Item					sItem;

	Point3D					sPosition;

	DWORD					dwaSecCode[4];
};

struct PacketBuyNPCShop : Packet
{
	ItemData				sItemData;
	int						iCount;

	int						iUniqueID;
	int						iNPCID;
};

struct PacketItemShopList : Packet
{
	int						iShopType;
	int						iItemCount;
	int						iUniqueID;
	int						iNPCID;
	int						iUnk001[2];
	char					szBuffer[0x1FE0];
};

struct PacketGetItem : Packet
{
	int						iState;
	Point3D					sPosition;

	class Map			  * pcMap;
};

struct PacketNetGetItem : Packet
{
	int						iID;

	PacketItem				sPacketItem;
};


struct PacketItemLinkChat : Packet
{
	char					szCharacterName[32];

	char					szMessage[256];

	int						eChatColor;

	ItemData				cItemData;
};

struct PacketRequestBeginnerItemsTimer : Packet
{
	int						iUnk;
};

struct PacketRequestChemistItemTimer : Packet
{
	int						iUnk;
};

struct PacketPlaceItem : Packet
{
	int				iChk1;
	EItemID			iItemID;
	int				iChk2;
	int				iUnk;
};

struct PacketDeleteItem : Packet
{
	EItemID			iItemID;
	int				iChk1;
	int				iChk2;
};


struct PacketGlamorousBrush : Packet
{
	EItemID					eGlamorousItemID;
	int						iChk1;
	int						iChk2;

	ItemData				cItemData;

	BOOL					bResult;
};


static int ForceDamageTable[]
{
		2,		// Lucidy
		4,		// Sereneo
		7,		// Fadeo
		10,		// Sparky
		15,		// Raident
		25,		// Transparo
		40,		// Murky
		60,		// Devine
		80,		// Celesto
		100,	// Mirage
		120,	// Inferna
		140,	// Enigma
		160,	// Bellum	
		180,	// Oredo	
		200,	// Sapphire
		220,	// Sol
};


static int ForceDamagePercentTable[]
{
		0,		// Lucidy
		0,		// Sereneo
		0,		// Fadeo
		0,		// Sparky
		0,		// Raident
		0,		// Transparo
		10,		// Murky
		10,		// Devine
		10,		// Celesto
		10,		// Mirage
		10,		// Inferna
		10,		// Enigma
		10,		// Bellum	
		10,		// Oredo	
		10,		// Sapphire	
		10,		// Sol
};

static int ForceDurationTable[]
{
		500,		// Lucidy
		500,		// Sereneo
		500,		// Fadeo
		500,		// Sparky
		700,		// Raident
		900,		// Transparo
		1800,		// Murky
		2400,		// Devine
		3600,		// Celesto
		4800,		// Mirage
		6000,		// Inferna
		7200,		// Enigma
		8400,		// Bellum	
		9600,		// Oredo	
		10800,		// Sapphire	
		12000,		// Sol
};

struct ItemAreaDropData
{
	int                 iUserID;

	EItemID             iItemID;
	int                 iChk1;
	int                 iChk2;

	Point3D             sPosition;

	int                 iRadius;

	bool                bParty;

	bool                bIsItem;
};

#define MAX_DROPITEM_AREA	5

struct PacketItemAreaDrop : Packet
{
	int					iCount;
	ItemAreaDropData	saArea[MAX_DROPITEM_AREA];
};

struct PacketItemEffect : Packet
{
	int				iItemID;

	int				iColor;
	short			saColors[4];
	UINT			uShowEffect;
	UINT			uScale;
};

