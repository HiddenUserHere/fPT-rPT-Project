#pragma once

#include "skill.h"
#include "item.h"

enum EMapID;
struct RecordSkill;


enum EIntegrityUpdate : BYTE
{
	INTEGRITYUPDATE_None,

	INTEGRITYUPDATE_KilledMonster,
	INTEGRITYUPDATE_KilledUser,
};


enum EElementID : unsigned int
{
	ELEMENTID_Organic,
	ELEMENTID_Earth,
	ELEMENTID_Fire,
	ELEMENTID_Ice,
	ELEMENTID_Lighting,
	ELEMENTID_Poison,
	ELEMENTID_Water,
	ELEMENTID_Wind,
	ELEMENTID_None = 15,
};

enum ECharacterType : int
{
	CHARACTERTYPE_None				= 0,

	CHARACTERTYPE_NPC				= 0x00,
	CHARACTERTYPE_Monster			= 0x01,
	CHARACTERTYPE_Player			= 0x80,
};

enum EClassFlag
{
	CLASSFLAG_None					= 0,
	CLASSFLAG_Fighter				= 2,
	CLASSFLAG_Mechanician			= 1,
	CLASSFLAG_Archer				= 4,
	CLASSFLAG_Pikeman				= 3,
	CLASSFLAG_Atalanta				= 0x00020000,
	CLASSFLAG_Knight				= 0x00010000,
	CLASSFLAG_Magician				= 0x00040000,
	CLASSFLAG_Priestess				= 0x00030000,
	CLASSFLAG_Assassin				= 0x00050100,
	CLASSFLAG_Shaman				= 0x00050000,
};

enum ECharacterRank : int
{
	CHARACTERRANK_Error = -1,

	CHARACTERRANK_Rank1 = 0,
	CHARACTERRANK_Rank2 = 1,
	CHARACTERRANK_Rank3 = 2,
	CHARACTERRANK_Rank4 = 3,
	CHARACTERRANK_Rank5 = 4,
	CHARACTERRANK_Rank6 = 5,
	CHARACTERRANK_Rank7 = 6,
	CHARACTERRANK_Rank8 = 7,
};

enum ECharacterClass
{
	CHARACTERCLASS_None					= 0,

	CHARACTERCLASS_Fighter				= 1,
	CHARACTERCLASS_Mechanician			= 2,
	CHARACTERCLASS_Archer				= 3,
	CHARACTERCLASS_Pikeman				= 4,
	CHARACTERCLASS_Atalanta				= 5,
	CHARACTERCLASS_Knight				= 6,
	CHARACTERCLASS_Magician				= 7,
	CHARACTERCLASS_Priestess			= 8,
	CHARACTERCLASS_Assassin				= 9,
	CHARACTERCLASS_Shaman				= 10,
};

enum ECharacterRace
{
	CHARACTERRACE_Tempskron,
	CHARACTERRACE_Morion,
	CHARACTERRACE_Sopphetio,
};

enum EMonsterType
{
	MONSTERTYPE_Normal					= 0x00,
	MONSTERTYPE_Undead					= 0x90,
	MONSTERTYPE_Mutant					= 0x91,
	MONSTERTYPE_Demon					= 0x92,
	MONSTERTYPE_Mechanic				= 0x93,
	MONSTERTYPE_Friendly				= 0xA0
};

enum EMonsterNature
{
	MONSTERNATURE_Neutral				= 0x80,
	MONSTERNATURE_Good					= 0x81,
	MONSTERNATURE_Evil					= 0x82,
};

enum EMonsterEffectID
{
	MONSTEREFFECTID_None				= 0,

	MONSTEREFFECTID_CYCLOPS = 0x1000,
	MONSTEREFFECTID_HOBGOBLIN = 0x1010,
	MONSTEREFFECTID_IMP = 0x1020,
	MONSTEREFFECTID_MINIG = 0x1030,
	MONSTEREFFECTID_PLANT = 0x1040,
	MONSTEREFFECTID_SKELETON = 0x1050,
	MONSTEREFFECTID_ZOMBI = 0x1060,
	MONSTEREFFECTID_OBIT = 0x1070,

	MONSTEREFFECTID_HOPT = 0x1080,
	MONSTEREFFECTID_BARGON = 0x1090,
	MONSTEREFFECTID_LEECH = 0x10A0,
	MONSTEREFFECTID_MUSHROOM = 0x10B0,

	MONSTEREFFECTID_ARMA = 0x10C0,
	MONSTEREFFECTID_SCORPION = 0x10D0,

	MONSTEREFFECTID_HEADCUTTER = 0x1100,
	MONSTEREFFECTID_SANDLEM = 0x1110,
	MONSTEREFFECTID_WEB = 0x1120,
	MONSTEREFFECTID_HOPYKING = 0x1130,
	MONSTEREFFECTID_CRIP = 0x1140,
	MONSTEREFFECTID_BUMA = 0x1150,
	MONSTEREFFECTID_DECOY = 0x1160,
	MONSTEREFFECTID_DORAL = 0x1170,
	MONSTEREFFECTID_FIGON = 0x1180,
	MONSTEREFFECTID_STONEGIANT = 0x1190,
	MONSTEREFFECTID_GREVEN = 0x11A0,
	MONSTEREFFECTID_ILLUSIONKNIGHT = 0x11B0,
	MONSTEREFFECTID_SKELETONRANGE = 0x11C0,
	MONSTEREFFECTID_SKELETONMELEE = 0x11D0,
	MONSTEREFFECTID_WOLVERLIN = 0x11E0,

	MONSTEREFFECTID_RABIE = 0x1200,
	MONSTEREFFECTID_MUDY = 0x1210,
	MONSTEREFFECTID_SEN = 0x1220,
	MONSTEREFFECTID_EGAN = 0x1230,
	MONSTEREFFECTID_BEEDOG = 0x1240,
	MONSTEREFFECTID_MUTANTPLANT = 0x1250,
	MONSTEREFFECTID_MUTANTRABIE = 0x1260,
	MONSTEREFFECTID_MUTANTTREE = 0x1270,
	MONSTEREFFECTID_AVELISK = 0x1280,
	MONSTEREFFECTID_NAZ = 0x1290,
	MONSTEREFFECTID_MUMMY = 0x12A0,
	MONSTEREFFECTID_HULK = 0x12B0,
	MONSTEREFFECTID_SUCCUBUS = 0x12C0,
	MONSTEREFFECTID_DAWLIN = 0x12D0,
	MONSTEREFFECTID_SHADOW = 0x12E0,
	MONSTEREFFECTID_BERSERKER = 0x12F0,
	MONSTEREFFECTID_IRONGUARD = 0x1300,
	MONSTEREFFECTID_FURY = 0x1310,
	MONSTEREFFECTID_SLIVER = 0x1320,
	MONSTEREFFECTID_HUNGKY = 0x1330,

	MONSTEREFFECTID_RATOO = 0x1340,
	MONSTEREFFECTID_STYGIANLORD = 0x1350,
	MONSTEREFFECTID_OMICRON = 0x1360,
	MONSTEREFFECTID_DMACHINE = 0x1370,
	MONSTEREFFECTID_METRON = 0x1380,

	MONSTEREFFECTID_MRGHOST = 0x1390,

	MONSTEREFFECTID_VAMPIRICBAT = 0x13A0,
	MONSTEREFFECTID_MIREKEEPER = 0x13B0,
	MONSTEREFFECTID_MUFFIN = 0x13C0,
	MONSTEREFFECTID_SOLIDSNAIL = 0x13D0,
	MONSTEREFFECTID_BEEVIL = 0x13E0,
	MONSTEREFFECTID_DIREBEE = 0x13F0,
	MONSTEREFFECTID_NIGHTMARE = 0x1400,
	MONSTEREFFECTID_STONEGOLEM = 0x1410,
	MONSTEREFFECTID_THORNCRAWLER = 0x1420,
	MONSTEREFFECTID_HEAVYGOBLIN = 0x1430,
	MONSTEREFFECTID_EVILPLANT = 0x1440,
	MONSTEREFFECTID_HAUNTINGPLANT = 0x1450,
	MONSTEREFFECTID_DARKKNIGHT = 0x1460,
	MONSTEREFFECTID_GUARDIAN_SAINT = 0x1470,
	MONSTEREFFECTID_DarkGuardian = 0x1471,

	MONSTEREFFECTID_CHAINGOLEM = 0x1500,
	MONSTEREFFECTID_DEADZONE = 0x1510,
	MONSTEREFFECTID_GROTESQUE = 0x1520,
	MONSTEREFFECTID_HYPERMACHINE = 0x1530,
	MONSTEREFFECTID_IRONFIST = 0x1540,
	MONSTEREFFECTID_MORGON = 0x1550,
	MONSTEREFFECTID_MOUNTAIN = 0x1560,
	MONSTEREFFECTID_RAMPAGE = 0x1570,
	MONSTEREFFECTID_RUNICGUARDIAN = 0x1580,
	MONSTEREFFECTID_SADNESS = 0x1590,
	MONSTEREFFECTID_TOWERGOLEM = 0x15A0,
	MONSTEREFFECTID_VAMPRICBEE = 0x15B0,
	MONSTEREFFECTID_VAMPRICMACHINE = 0x15C0,
	MONSTEREFFECTID_OMU = 0x15D0,

	MONSTEREFFECTID_AVELIN_ARCHER = 0x15E0,
	MONSTEREFFECTID_AVELIN_QUEEN = 0x15F0,

	MONSTEREFFECTID_BABEL = 0x1600,

	MONSTEREFFECTID_MYSTIC = 0x1610,
	MONSTEREFFECTID_ICEGOBLIN = 0x1620,
	MONSTEREFFECTID_COLDEYE = 0x1630,
	MONSTEREFFECTID_FROZEN = 0x1640,
	MONSTEREFFECTID_ICEGOLEM = 0x1650,
	MONSTEREFFECTID_FROST = 0x1660,
	MONSTEREFFECTID_CHAOSCARA = 0x1670,
	MONSTEREFFECTID_DEATHKNIGHT = 0x1680,

	MONSTEREFFECTID_GREATE_GREVEN = 0x1700,
	MONSTEREFFECTID_LIZARDFOLK = 0x1710,
	MONSTEREFFECTID_M_LORD = 0x1720,
	MONSTEREFFECTID_SPIDER = 0x1730,
	MONSTEREFFECTID_STINGRAY = 0x1740,
	MONSTEREFFECTID_STRIDER = 0x1750,

	MONSTEREFFECTID_TURTLE_CANNON = 0x1800,
	MONSTEREFFECTID_DEVIL_BIRD = 0x1810,
	MONSTEREFFECTID_BLIZZARD_GIANT = 0x1820,
	MONSTEREFFECTID_KELVEZU = 0x1830,

	MONSTEREFFECTID_DARKPHALANX = 0x1840,
	MONSTEREFFECTID_BLOODYKNIGHT = 0x1850,
	MONSTEREFFECTID_CHIMERA = 0x1860,
	MONSTEREFFECTID_FIREWORM = 0x1870,
	MONSTEREFFECTID_HELLHOUND = 0x1880,
	MONSTEREFFECTID_DARKGUARD = 0x1890,
	MONSTEREFFECTID_DARKMAGE = 0x18A0,

	MONSTEREFFECTID_MOKOVA = 0x18B0,
	MONSTEREFFECTID_TEMPLEGUARD = 0x18C0,
	MONSTEREFFECTID_SETO = 0x18D0,
	MONSTEREFFECTID_KINGSPIDER = 0x18E0,

	MONSTEREFFECTID_REVIVED_KNIGHT = 0x18F0,
	MONSTEREFFECTID_REVIVED_MAGICIAN = 0x1900,
	MONSTEREFFECTID_REVIVED_ARCHER = 0x1910,
	MONSTEREFFECTID_REVIVED_ATALANTA = 0x1920,
	MONSTEREFFECTID_REVIVED_FIGTHER = 0x1930,
	MONSTEREFFECTID_REVIVED_MECANICIAN = 0x1940,
	MONSTEREFFECTID_REVIVED_PIKEMAN = 0x1950,
	MONSTEREFFECTID_REVIVED_PRIESTESS = 0x1960,
	MONSTEREFFECTID_DEADHOPT = 0x1970,
	MONSTEREFFECTID_DEADKINGHOPY = 0x1980,
	MONSTEREFFECTID_GORGON = 0x1990,
	MONSTEREFFECTID_HOBOGOLEM = 0x19A0,

	MONSTEREFFECTID_NIKEN = 0x19B0,
	MONSTEREFFECTID_MIMIC = 0x19C0,
	MONSTEREFFECTID_KINGBAT = 0x19D0,
	MONSTEREFFECTID_GOBLINSHAMAN = 0x19E0,
	MONSTEREFFECTID_HEST = 0x19F0,

	MONSTEREFFECTID_RUCA = 0x2100,
	MONSTEREFFECTID_NAZSENIOR = 0x2110,
	MONSTEREFFECTID_IGOLATION = 0x2120,
	MONSTEREFFECTID_KAKOA = 0x2130,
	MONSTEREFFECTID_SPRIN = 0x2140,
	MONSTEREFFECTID_UNDEADMAPLE = 0x2150,
	MONSTEREFFECTID_XETAN = 0x2160,
	MONSTEREFFECTID_DEVILSHY = 0x21D0,

	MONSTEREFFECTID_BEBECHICK = 0x2170,
	MONSTEREFFECTID_PAPACHICK = 0x2180,
	MONSTEREFFECTID_BEAR = 0x21A0,
	MONSTEREFFECTID_SNOWMAN = 0x21C0,
	MONSTEREFFECTID_BRAZILBEAR = 0x21F1,
	MONSTEREFFECTID_BOITATA = 0x21F2,
	MONSTEREFFECTID_ICESERPENT = 0x2200,
	MONSTEREFFECTID_ICEWORM = 0x2210,
	MONSTEREFFECTID_ICEBAT = 0x2220,
	MONSTEREFFECTID_MINEGOLEM = 0x2230,
	MONSTEREFFECTID_SEALCRASHER = 0x2240,
	MONSTEREFFECTID_TARANTULIKA = 0x2250,
	MONSTEREFFECTID_TULLA = 0x2260,
	MONSTEREFFECTID_UNDEADMINER = 0x2270,
	MONSTEREFFECTID_UNDEADMANAGER = 0x2280,

	MONSTEREFFECTID_Billy = 0x2290,
	MONSTEREFFECTID_Hound = 0x22A0,
	MONSTEREFFECTID_Lena = 0x22B0,
	MONSTEREFFECTID_Marionette = 0x22C0,
	MONSTEREFFECTID_UndeadStalker = 0x22D0,

	MONSTEREFFECTID_Greedy = 0x2340,
	MONSTEREFFECTID_Acero = 0x2342,
	MONSTEREFFECTID_Nihil = 0x2344,
	MONSTEREFFECTID_Chalybs = 0x2345,
	MONSTEREFFECTID_AncientGuardian = 0x2346,
	MONSTEREFFECTID_Tyrcus = 0x2347,


	MONSTEREFFECTID_Igniculus = 0x2358,
	MONSTEREFFECTID_LavaArmor = 0x2359,
	MONSTEREFFECTID_LavaArcher = 0x235A,
	MONSTEREFFECTID_LavaGolem = 0x235B,
	MONSTEREFFECTID_LavaGiant = 0x235C,
	MONSTEREFFECTID_Insec = 0x235D,
	MONSTEREFFECTID_Flamo = 0x235E,
	MONSTEREFFECTID_FlameMaiden = 0x235F,

	MONSTEREFFECTID_Freddy = 0x2360,
	MONSTEREFFECTID_Pelgia = 0x2361,
	MONSTEREFFECTID_GrassBeast = 0x2362,
	MONSTEREFFECTID_TropicalDemon = 0x2363,
	MONSTEREFFECTID_TropicalFlower = 0x2364,
	MONSTEREFFECTID_TropicalRanger = 0x2365,
	MONSTEREFFECTID_Yeti = 0x2366,

	MONSTEREFFECTID_WORLDCUP = 0x1A00,

	MONSTEREFFECTID_NPC_MORIF = 0x2010,
	MONSTEREFFECTID_NPC_MOLLYWOLF = 0x2012,
	MONSTEREFFECTID_NPC_SKILLMASTER = 0x2020,
	MONSTEREFFECTID_NPC_MAGICMASTER = 0x2030,


	MONSTEREFFECTID_S_WOLVERLIN = 0x3010,
	MONSTEREFFECTID_S_METALGOLEM = 0x3020,
	MONSTEREFFECTID_S_F_ELEMENTAL = 0x3030,


	MONSTEREFFECTID_CASTLE_DOOR = 0x5010,
	MONSTEREFFECTID_CASTLE_CRYSTAL_R = 0x5020,
	MONSTEREFFECTID_CASTLE_CRYSTAL_G = 0x5021,
	MONSTEREFFECTID_CASTLE_CRYSTAL_B = 0x5022,
	MONSTEREFFECTID_CASTLE_CRYSTAL_N = 0x5023,
	MONSTEREFFECTID_CASTLE_TOWER_B = 0x5030,

	MONSTEREFFECTID_CASTLE_SOLDER_A = 0x5100,
	MONSTEREFFECTID_CASTLE_SOLDER_B = 0x5110,
	MONSTEREFFECTID_CASTLE_SOLDER_C = 0x5120,

	MONSTEREFFECTID_WATERMELON = 0x2190,

	MONSTEREFFECTID_S_AVELIN = 0x5140,
	MONSTEREFFECTID_S_BAGON = 0x5150,
	MONSTEREFFECTID_S_BEEDOG = 0x5160,
	MONSTEREFFECTID_S_BEEVIL = 0x5170,
	MONSTEREFFECTID_S_BERSERKER = 0x5180,
	MONSTEREFFECTID_S_BUMA = 0x5190,
	MONSTEREFFECTID_S_COKRIS = 0x51A0,
	MONSTEREFFECTID_S_COLDEYE = 0x51B0,
	MONSTEREFFECTID_S_CRYPT = 0x51C0,
	MONSTEREFFECTID_S_CYCLOPS = 0x51D0,
	MONSTEREFFECTID_S_DEADZONE = 0x51E0,
	MONSTEREFFECTID_S_DECOY = 0x51F0,
	MONSTEREFFECTID_S_DMACHINE = 0x5200,
	MONSTEREFFECTID_S_EVILSNAIL = 0x5210,
	MONSTEREFFECTID_S_GREVEN = 0x5220,
	MONSTEREFFECTID_S_GROTESQUE = 0x5230,
	MONSTEREFFECTID_S_ICEGOBLIN = 0x5240,
	MONSTEREFFECTID_S_ICEGOLEM = 0x5250,
	MONSTEREFFECTID_S_INCUBUS = 0x5260,
	MONSTEREFFECTID_S_KINGHOPY = 0x5270,
	MONSTEREFFECTID_S_LEECH = 0x5280,
	MONSTEREFFECTID_S_LIZARDFOLK = 0x5290,
	MONSTEREFFECTID_S_MEPHIT = 0x52A0,
	MONSTEREFFECTID_S_METRON = 0x52B0,
	MONSTEREFFECTID_S_MUFFIN = 0x52C0,
	MONSTEREFFECTID_S_MUMMY = 0x52D0,
	MONSTEREFFECTID_S_NAZ = 0x52E0,
	MONSTEREFFECTID_S_OMEGA = 0x52F0,
	MONSTEREFFECTID_S_RAMPAGE = 0x5300,
	MONSTEREFFECTID_S_SADNESS = 0x5310,
	MONSTEREFFECTID_S_SLAUGHTER = 0x5320,
	MONSTEREFFECTID_S_SLAYON = 0x5330,
	MONSTEREFFECTID_S_SLIVER = 0x5340,
	MONSTEREFFECTID_S_TITAN = 0x5350,
	MONSTEREFFECTID_S_TOWERGOLEM = 0x5360,
	MONSTEREFFECTID_S_TYPHOON = 0x5370,
	MONSTEREFFECTID_S_VAMPIRICBAT = 0x5380,
	MONSTEREFFECTID_S_WITCH = 0x5390,
	MONSTEREFFECTID_S_ZOMBIE = 0x53A0,

	MONSTEREFFECTID_CrystalTower = 0x53A1,
	MONSTEREFFECTID_IceElemental = 0x53A2,
	MONSTEREFFECTID_SummonTiger = 0x53A3,
	
	MONSTEREFFECTID_LizardBigmama = 0x2313,
	MONSTEREFFECTID_LizardCommander = 0x230F,
	MONSTEREFFECTID_LizardDoctor = 0x230A,
	MONSTEREFFECTID_LizardElder = 0x230B,
	MONSTEREFFECTID_LizardPapa = 0x230C,
	MONSTEREFFECTID_LizardSoldier = 0x230D,
	
	MONSTEREFFECTID_Dey = 0x2350,
	MONSTEREFFECTID_Gurkob = 0x2351,
	MONSTEREFFECTID_Shogoth = 0x2352,
	MONSTEREFFECTID_Arhdyra = 0x2353,
	MONSTEREFFECTID_Sathla = 0x2354,
	MONSTEREFFECTID_Faugn = 0x2355,
	MONSTEREFFECTID_Koon = 0x2356,
	MONSTEREFFECTID_Yagditha = 0x2357,

	MONSTEREFFECTID_ITechnician = 0x2300,
	MONSTEREFFECTID_LTechnician = 0x2301,
	MONSTEREFFECTID_LEngineer = 0x2302,
	MONSTEREFFECTID_ISoldier = 0x2303,
	MONSTEREFFECTID_IBomber = 0x2304,
	MONSTEREFFECTID_LGuardian = 0x2305,
	MONSTEREFFECTID_IElite = 0x2306,
	MONSTEREFFECTID_Draxos = 0x2307,

	MONSTEREFFECTID_Volcanos = 0x2308,
	MONSTEREFFECTID_Hagios = 0x2309,

};

struct CharacterSoundEffect
{
	char					* szCodeName;
	EMonsterEffectID		iMonsterEffectID;
};

static CharacterSoundEffect	saCharacterSoundEffect[] = 
{
	{ "CYCLOPS", MONSTEREFFECTID_CYCLOPS },
	{ "HOBGOBLIN", MONSTEREFFECTID_HOBGOBLIN },
	{ "IMP", MONSTEREFFECTID_IMP },
	{ "MINIG", MONSTEREFFECTID_MINIG },
	{ "PLANT", MONSTEREFFECTID_PLANT },
	{ "SKELETON", MONSTEREFFECTID_SKELETON },
	{ "ZOMBI", MONSTEREFFECTID_ZOMBI },
	{ "OBIT", MONSTEREFFECTID_OBIT },

	{ "HOPT", MONSTEREFFECTID_HOPT },
	{ "BARGON", MONSTEREFFECTID_BARGON },
	{ "LEECH", MONSTEREFFECTID_LEECH },
	{ "MUSHROOM", MONSTEREFFECTID_MUSHROOM },

	{ "ARMA", MONSTEREFFECTID_ARMA },
	{ "ARMADIL", MONSTEREFFECTID_ARMA },
	{ "SCORPION", MONSTEREFFECTID_SCORPION },

	{ "HEADCUTTER", MONSTEREFFECTID_HEADCUTTER },
	{ "SANDLEM", MONSTEREFFECTID_SANDLEM },
	{ "WEB", MONSTEREFFECTID_WEB },
	{ "HOPYKING", MONSTEREFFECTID_HOPYKING },
	{ "CRIP", MONSTEREFFECTID_CRIP },
	{ "BUMA", MONSTEREFFECTID_BUMA },
	{ "DECOY", MONSTEREFFECTID_DECOY },
	{ "DORAL", MONSTEREFFECTID_DORAL },
	{ "FIGON", MONSTEREFFECTID_FIGON },
	{ "GOLEM", MONSTEREFFECTID_STONEGIANT },
	{ "GREVEN", MONSTEREFFECTID_GREVEN },
	{ "ILLUSIONKNIGHT", MONSTEREFFECTID_ILLUSIONKNIGHT },
	{ "SKELETONRANGE", MONSTEREFFECTID_SKELETONRANGE },
	{ "SKELETONMELEE", MONSTEREFFECTID_SKELETONMELEE },
	{ "WOLVERLIN", MONSTEREFFECTID_WOLVERLIN },

	{ "RABIE", MONSTEREFFECTID_RABIE },
	{ "MUDY", MONSTEREFFECTID_MUDY },
	{ "SEN", MONSTEREFFECTID_SEN },
	{ "EGAN", MONSTEREFFECTID_EGAN },
	{ "BEEDOG", MONSTEREFFECTID_BEEDOG },
	{ "MUTANTPLANT", MONSTEREFFECTID_MUTANTPLANT },
	{ "MUTANTRABIE", MONSTEREFFECTID_MUTANTRABIE },
	{ "MUTANTTREE", MONSTEREFFECTID_MUTANTTREE },
	{ "AVELISK", MONSTEREFFECTID_AVELISK },
	{ "NAZ", MONSTEREFFECTID_NAZ },
	{ "MUMMY", MONSTEREFFECTID_MUMMY },
	{ "HULK", MONSTEREFFECTID_HULK },
	{ "HUNGKY", MONSTEREFFECTID_HUNGKY },
	{ "SUCCUBUS", MONSTEREFFECTID_SUCCUBUS },
	{ "DAWLIN", MONSTEREFFECTID_DAWLIN },
	{ "SHADOW", MONSTEREFFECTID_SHADOW },
	{ "BERSERKER", MONSTEREFFECTID_BERSERKER },
	{ "IRONGUARD", MONSTEREFFECTID_IRONGUARD },
	{ "FURY", MONSTEREFFECTID_FURY },
	{ "SLIVER", MONSTEREFFECTID_SLIVER },

	{ "RATOO", MONSTEREFFECTID_RATOO },
	{ "STYGIANLORD", MONSTEREFFECTID_STYGIANLORD },
	{ "OMICRON", MONSTEREFFECTID_OMICRON },
	{ "D-MACHINE", MONSTEREFFECTID_DMACHINE },
	{ "METRON", MONSTEREFFECTID_METRON },
	{ "MRGHOST", MONSTEREFFECTID_MRGHOST },

	{ "VAMPIRICBAT", MONSTEREFFECTID_VAMPIRICBAT },
	{ "MIREKEEPER", MONSTEREFFECTID_MIREKEEPER },
	{ "MUFFIN", MONSTEREFFECTID_MUFFIN },
	{ "SOLIDSNAIL", MONSTEREFFECTID_SOLIDSNAIL },
	{ "BEEVIL", MONSTEREFFECTID_BEEVIL },
	{ "NIGHTMARE", MONSTEREFFECTID_NIGHTMARE },
	{ "STONEGOLEM", MONSTEREFFECTID_STONEGOLEM },
	{ "THORNCRAWLER", MONSTEREFFECTID_THORNCRAWLER },
	{ "HEAVYGOBLIN", MONSTEREFFECTID_HEAVYGOBLIN },
	{ "EVILPLANT", MONSTEREFFECTID_EVILPLANT },
	{ "HAUNTINGPLANT", MONSTEREFFECTID_HAUNTINGPLANT },
	{ "DARKKNIGHT", MONSTEREFFECTID_DARKKNIGHT },
	{ "GUARDIAN-SAINT", MONSTEREFFECTID_GUARDIAN_SAINT },
	{ "DARKGUARDIAN", MONSTEREFFECTID_DarkGuardian },

	{ "CHAINGOLEM", MONSTEREFFECTID_CHAINGOLEM },
	{ "DEADZONE", MONSTEREFFECTID_DEADZONE },
	{ "GROTESQUE", MONSTEREFFECTID_GROTESQUE },
	{ "HYPERMACHINE", MONSTEREFFECTID_HYPERMACHINE },
	{ "IRONFIST", MONSTEREFFECTID_IRONFIST },
	{ "MORGON", MONSTEREFFECTID_MORGON },
	{ "MOUNTAIN", MONSTEREFFECTID_MOUNTAIN },
	{ "RAMPAGE", MONSTEREFFECTID_RAMPAGE },
	{ "RUNICGUARDIAN", MONSTEREFFECTID_RUNICGUARDIAN },
	{ "SADNESS", MONSTEREFFECTID_SADNESS },
	{ "TOWERGOLEM", MONSTEREFFECTID_TOWERGOLEM },
	{ "VAMPIRICBEE", MONSTEREFFECTID_VAMPRICBEE },
	{ "VAMPIRICMACHINE", MONSTEREFFECTID_VAMPRICMACHINE },
	{ "AVELINARCHER", MONSTEREFFECTID_AVELIN_ARCHER },
	{ "AVELINQUEEN", MONSTEREFFECTID_AVELIN_QUEEN },
	{ "BABEL", MONSTEREFFECTID_BABEL },

	{ "MYSTIC", MONSTEREFFECTID_MYSTIC },
	{ "ICEGOBLIN", MONSTEREFFECTID_ICEGOBLIN },
	{ "COLDEYE", MONSTEREFFECTID_COLDEYE },
	{ "FROZEN", MONSTEREFFECTID_FROZEN },
	{ "ICEGOLEM", MONSTEREFFECTID_ICEGOLEM },
	{ "FROST", MONSTEREFFECTID_FROST },
	{ "CHAOSCARA", MONSTEREFFECTID_CHAOSCARA },
	{ "DEATHKNIGHT", MONSTEREFFECTID_DEATHKNIGHT },

	{ "GREATE_GREVEN", MONSTEREFFECTID_GREATE_GREVEN },
	{ "LIZARDFOLK", MONSTEREFFECTID_LIZARDFOLK },
	{ "M_LORD", MONSTEREFFECTID_M_LORD },
	{ "SPIDER", MONSTEREFFECTID_SPIDER },
	{ "STINGRAY", MONSTEREFFECTID_STINGRAY },
	{ "STRIDER", MONSTEREFFECTID_STRIDER },
	{ "OMU", MONSTEREFFECTID_OMU },

	{ "TURTLECANNON", MONSTEREFFECTID_TURTLE_CANNON },
	{ "DEVILBIRD", MONSTEREFFECTID_DEVIL_BIRD },
	{ "BLIZZARDGIANT", MONSTEREFFECTID_BLIZZARD_GIANT },
	{ "KELVEZU", MONSTEREFFECTID_KELVEZU },

	{ "DARKPHALANX", MONSTEREFFECTID_DARKPHALANX },
	{ "BLOODYKNIGHT", MONSTEREFFECTID_BLOODYKNIGHT },
	{ "CHIMERA", MONSTEREFFECTID_CHIMERA },
	{ "FIREWORM", MONSTEREFFECTID_FIREWORM },
	{ "HELLHOUND", MONSTEREFFECTID_HELLHOUND },
	{ "DARKGUARD", MONSTEREFFECTID_DARKGUARD },
	{ "DARKMAGE", MONSTEREFFECTID_DARKMAGE },

	{ "MONMOKOVA", MONSTEREFFECTID_MOKOVA },
	{ "MONTEMPLEGUARD", MONSTEREFFECTID_TEMPLEGUARD },
	{ "MONSETO", MONSTEREFFECTID_SETO },
	{ "MONKINGSPIDER", MONSTEREFFECTID_KINGSPIDER },

	{ "D_KN", MONSTEREFFECTID_REVIVED_KNIGHT },
	{ "D_MAGI", MONSTEREFFECTID_REVIVED_MAGICIAN },
	{ "D_ATAL", MONSTEREFFECTID_REVIVED_ATALANTA },
	{ "D_FI", MONSTEREFFECTID_REVIVED_FIGTHER },
	{ "D_AR", MONSTEREFFECTID_REVIVED_ARCHER },
	{ "D_MECA", MONSTEREFFECTID_REVIVED_MECANICIAN },
	{ "D_PA", MONSTEREFFECTID_REVIVED_PIKEMAN },
	{ "D_PR", MONSTEREFFECTID_REVIVED_PRIESTESS },
	{ "DEADHOPT", MONSTEREFFECTID_DEADHOPT },
	{ "DEADKINGHOPY", MONSTEREFFECTID_DEADKINGHOPY },
	{ "GORGON", MONSTEREFFECTID_GORGON },
	{ "HOBOGOLEM", MONSTEREFFECTID_HOBOGOLEM },

	{ "NIKEN", MONSTEREFFECTID_NIKEN },
	{ "MIMIC", MONSTEREFFECTID_MIMIC },
	{ "KINGBAT", MONSTEREFFECTID_KINGBAT },
	{ "GOBLINSHAMAN", MONSTEREFFECTID_GOBLINSHAMAN },
	{ "HEST", MONSTEREFFECTID_HEST },

	{ "RUCA", MONSTEREFFECTID_RUCA },
	{ "NAZSENIOR", MONSTEREFFECTID_NAZSENIOR },
	{ "IGOLATION", MONSTEREFFECTID_IGOLATION },
	{ "KAKOA", MONSTEREFFECTID_KAKOA },
	{ "SPRIN", MONSTEREFFECTID_SPRIN },
	{ "UNDEADMAPLE", MONSTEREFFECTID_UNDEADMAPLE },
	{ "XETAN", MONSTEREFFECTID_XETAN },
	{ "DEVILSHY", MONSTEREFFECTID_DEVILSHY },

	{ "BEBECHICK", MONSTEREFFECTID_BEBECHICK },
	{ "PAPACHICK", MONSTEREFFECTID_PAPACHICK },
	{ "BEAR", MONSTEREFFECTID_BEAR },
	{ "SNOWMAN", MONSTEREFFECTID_SNOWMAN },
	{ "BRAZILBEAR", MONSTEREFFECTID_BRAZILBEAR },
	{ "BOITATA", MONSTEREFFECTID_BOITATA },

	{ "ICESERPENT", MONSTEREFFECTID_ICESERPENT },
	{ "ICEWORM", MONSTEREFFECTID_ICEWORM },
	{ "ICEBAT", MONSTEREFFECTID_ICEBAT },
	{ "MINEGOLEM", MONSTEREFFECTID_MINEGOLEM },
	{ "SEALCRASHER", MONSTEREFFECTID_SEALCRASHER },
	{ "TARANTULIKA", MONSTEREFFECTID_TARANTULIKA },
	{ "TULLA", MONSTEREFFECTID_TULLA },
	{ "UNDEADMINER", MONSTEREFFECTID_UNDEADMINER },
	{ "UNDEADMANAGER", MONSTEREFFECTID_UNDEADMANAGER },

	{ "BILLY", MONSTEREFFECTID_Billy },
	{ "HOUND", MONSTEREFFECTID_Hound },
	{ "LENA", MONSTEREFFECTID_Lena },
	{ "MARIONETTE", MONSTEREFFECTID_Marionette },
	{"UNDEADSTALKER", MONSTEREFFECTID_UndeadStalker},

	{"IGNICULUS", MONSTEREFFECTID_Igniculus},
	{"LavaArmor", MONSTEREFFECTID_LavaArmor},
	{"LavaArcher", MONSTEREFFECTID_LavaArcher},
	{"LavaGolem", MONSTEREFFECTID_LavaGolem},
	{"LavaGiant", MONSTEREFFECTID_LavaGiant},
	{"Insec", MONSTEREFFECTID_Insec},
	{"Flamo", MONSTEREFFECTID_Flamo},
	{"FlameMaiden", MONSTEREFFECTID_FlameMaiden},

	{"Freddy", MONSTEREFFECTID_Freddy},
	{"Pelgia", MONSTEREFFECTID_Pelgia},
	{"GrassBeast", MONSTEREFFECTID_GrassBeast},

	{"TropicalDemon", MONSTEREFFECTID_TropicalDemon},
	{"TropicalFlower", MONSTEREFFECTID_TropicalFlower},
	{"TropicalRanger", MONSTEREFFECTID_TropicalRanger},
	{"Yeti", MONSTEREFFECTID_Yeti},

	{ "MORIF", MONSTEREFFECTID_NPC_MORIF },
	{ "MOLLYWOLF", MONSTEREFFECTID_NPC_MOLLYWOLF },
	{ "SKILLMASTER", MONSTEREFFECTID_NPC_SKILLMASTER },
	{ "MAGE", MONSTEREFFECTID_NPC_MAGICMASTER },
	{ "WORLDCUP", MONSTEREFFECTID_WORLDCUP },
	{ "WATERMELON", MONSTEREFFECTID_WATERMELON },

	{ "WOLVERIN", MONSTEREFFECTID_S_WOLVERLIN },
	{ "METALGOLEM", MONSTEREFFECTID_S_METALGOLEM },
	{ "FIREELEMENTAL", MONSTEREFFECTID_S_F_ELEMENTAL },

	{ "CASTLEDOOR", MONSTEREFFECTID_CASTLE_DOOR },
	{ "T_CRYSTAL_R", MONSTEREFFECTID_CASTLE_CRYSTAL_R },
	{ "T_CRYSTAL_G", MONSTEREFFECTID_CASTLE_CRYSTAL_G },
	{ "T_CRYSTAL_B", MONSTEREFFECTID_CASTLE_CRYSTAL_B },
	{ "T_CRYSTAL_N", MONSTEREFFECTID_CASTLE_CRYSTAL_N },
	{ "TOWER-B", MONSTEREFFECTID_CASTLE_TOWER_B },

	{ "SOLDIER_A", MONSTEREFFECTID_CASTLE_SOLDER_A },
	{ "SOLDIER_B", MONSTEREFFECTID_CASTLE_SOLDER_B },
	{ "SOLDIER_C", MONSTEREFFECTID_CASTLE_SOLDER_C },

	{ "S_AVELIN", MONSTEREFFECTID_S_AVELIN },
	{ "S_BAGON", MONSTEREFFECTID_S_BAGON },
	{ "S_BEEDOG", MONSTEREFFECTID_S_BEEDOG },
	{ "S_BEEVIL", MONSTEREFFECTID_S_BEEVIL },
	{ "S_BERSERKER", MONSTEREFFECTID_S_BERSERKER },
	{ "S_BUMA", MONSTEREFFECTID_S_BUMA },
	{ "S_COKRIS", MONSTEREFFECTID_S_COKRIS },
	{ "S_COLDEYE", MONSTEREFFECTID_S_COLDEYE },
	{ "S_CRYPT", MONSTEREFFECTID_S_CRYPT },
	{ "S_CYCLOPS", MONSTEREFFECTID_S_CYCLOPS },
	{ "S_DEADZONE", MONSTEREFFECTID_S_DEADZONE },
	{ "S_DECOY", MONSTEREFFECTID_S_DECOY },
	{ "S_D-Machine", MONSTEREFFECTID_S_DMACHINE },
	{ "S_EVILSNAIL", MONSTEREFFECTID_S_EVILSNAIL },
	{ "S_GREVEN", MONSTEREFFECTID_S_GREVEN },
	{ "S_GROTESQUE", MONSTEREFFECTID_S_GROTESQUE },
	{ "S_ICEGOBLIN", MONSTEREFFECTID_S_ICEGOBLIN },
	{ "S_ICEGOLEM", MONSTEREFFECTID_S_ICEGOLEM },
	{ "S_INCUBUS", MONSTEREFFECTID_S_INCUBUS },
	{ "S_KINGHOPY", MONSTEREFFECTID_S_KINGHOPY },
	{ "S_LEECH", MONSTEREFFECTID_S_LEECH },
	{ "S_LIZARDFOLK", MONSTEREFFECTID_S_LIZARDFOLK },
	{ "S_MEPHIT", MONSTEREFFECTID_S_MEPHIT },
	{ "S_METRON", MONSTEREFFECTID_S_METRON },
	{ "S_MUFFIN", MONSTEREFFECTID_S_MUFFIN },
	{ "S_MUMMY", MONSTEREFFECTID_S_MUMMY },
	{ "S_NAZ", MONSTEREFFECTID_S_NAZ },
	{ "S_OMEGA", MONSTEREFFECTID_S_OMEGA },
	{ "S_RAMPAGE", MONSTEREFFECTID_S_RAMPAGE },
	{ "S_SADNESS", MONSTEREFFECTID_S_SADNESS },
	{ "S_SLAUGHTER", MONSTEREFFECTID_S_SLAUGHTER },
	{ "S_SLAYON", MONSTEREFFECTID_S_SLAYON },
	{ "S_SLIVER", MONSTEREFFECTID_S_SLIVER },
	{ "S_TITAN", MONSTEREFFECTID_S_TITAN },
	{ "S_TOWERGOLEM", MONSTEREFFECTID_S_TOWERGOLEM },
	{ "S_TYPHOON", MONSTEREFFECTID_S_TYPHOON },
	{ "S_VAMPIRICBAT", MONSTEREFFECTID_S_VAMPIRICBAT },
	{ "S_WITCH", MONSTEREFFECTID_S_WITCH },
	{ "S_ZOMBIE", MONSTEREFFECTID_S_ZOMBIE },
	{ "LIZARDBIGMAMA", MONSTEREFFECTID_LizardBigmama },
	{ "LIZARDCOMMANDER", MONSTEREFFECTID_LizardCommander },
	{ "LIZARDDOCTOR", MONSTEREFFECTID_LizardDoctor },
	{ "LIZARDELDER", MONSTEREFFECTID_LizardElder },
	{ "LIZARDPAPA", MONSTEREFFECTID_LizardPapa },
	{ "LIZARDSOLDIER", MONSTEREFFECTID_LizardSoldier },
	{ "DEY", MONSTEREFFECTID_Dey },
	{ "GURKOB", MONSTEREFFECTID_Gurkob },
	{ "SHOGOTH", MONSTEREFFECTID_Shogoth },
	{ "ARHDYRA", MONSTEREFFECTID_Arhdyra },
	{ "SATHLA", MONSTEREFFECTID_Sathla },
	{ "FAUGN", MONSTEREFFECTID_Faugn },
	{ "KOON", MONSTEREFFECTID_Koon },
	{ "YAGDITHA", MONSTEREFFECTID_Yagditha },
	{ "CRYSTALTOWER", MONSTEREFFECTID_CrystalTower },
	{ "ICEELEMENTAL", MONSTEREFFECTID_IceElemental },
	{ "SUMMONTIGER", MONSTEREFFECTID_SummonTiger },

	{ "ISOLDIER", MONSTEREFFECTID_ISoldier },
	{ "IBOMBER", MONSTEREFFECTID_IBomber },
	{ "ITECHNICIAN", MONSTEREFFECTID_ITechnician },
	{ "LGUARDIAN", MONSTEREFFECTID_LGuardian },
	{ "LTECHNICIAN", MONSTEREFFECTID_LTechnician },
	{ "LENGINEER", MONSTEREFFECTID_LEngineer },
	{ "IELITE", MONSTEREFFECTID_IElite },
	{ "DRAXOS", MONSTEREFFECTID_Draxos },

	{ "ACERO", MONSTEREFFECTID_Acero },
	{ "GREEDY", MONSTEREFFECTID_Greedy },
	{ "NIHIL", MONSTEREFFECTID_Nihil },
	{ "CHALYBS", MONSTEREFFECTID_Chalybs },
	{ "ANCIENTGUARDIAN", MONSTEREFFECTID_AncientGuardian},
	{ "VOLCANOS", MONSTEREFFECTID_Volcanos},
	{ "TYRCUS", MONSTEREFFECTID_Tyrcus },
	{ "HAGIOS", MONSTEREFFECTID_Hagios },


	{ "", MONSTEREFFECTID_None }
};

struct SoundEffectData
{
	char						* pszFile;
	int							iID;
};

static const SoundEffectData saSoundEffectData[] =
{
	{ "wav\\Effects\\monster\\cyclops\\", 0x1000 },		//0
	{ "wav\\Effects\\monster\\hob goblin\\", 0x1010 },		//1
	{ "wav\\Effects\\monster\\Imp\\", 0x1020 },		//2
	{ "wav\\Effects\\monster\\miniG\\", 0x1030 },		//3
	{ "wav\\Effects\\monster\\plant\\", 0x1040 },		//4
	{ "wav\\Effects\\monster\\skeleton\\", 0x1050 },		//5
	{ "wav\\Effects\\monster\\zombi\\", 0x1060 },		//6
	{ "wav\\Effects\\monster\\obit\\", 0x1070 },		//7
	{ "wav\\Effects\\monster\\hopt\\", 0x1080 },		//8
	{ "wav\\Effects\\monster\\bargon\\", 0x1090 },		//9
	{ "wav\\Effects\\monster\\leech\\", 0x10A0 },		//10
	{ "wav\\Effects\\monster\\mushroom\\", 0x10B0 },		//11
	{ "wav\\Effects\\monster\\Arma\\", 0x10C0 },		//12
	{ "wav\\Effects\\monster\\scorpion\\", 0x10D0 },		//13
	{ "wav\\Effects\\monster\\HeadCutter\\", 0x1100 },		//14
	{ "wav\\Effects\\monster\\Sandlem\\", 0x1110 },		//15
	{ "wav\\Effects\\monster\\Web\\", 0x1120 },		//16
	{ "wav\\Effects\\monster\\HopyKing\\", 0x1130 },		//17
	{ "wav\\Effects\\monster\\Crip\\", 0x1140 },		//18
	{ "wav\\Effects\\monster\\Buma\\", 0x1150 },		//19
	{ "wav\\Effects\\monster\\Decoy\\", 0x1160 },		//20
	{ "wav\\Effects\\monster\\Doral\\", 0x1170 },		//21
	{ "wav\\Effects\\monster\\Figon\\", 0x1180 },		//22
	{ "wav\\Effects\\monster\\StoneGiant\\", 0x1190 },		//23
	{ "wav\\Effects\\monster\\Greven\\", 0x11A0 },		//24
	{ "wav\\Effects\\monster\\illusionknight\\", 0x11B0 },		//25
	{ "wav\\Effects\\monster\\SkeletonRange\\", 0x11C0 },		//26
	{ "wav\\Effects\\monster\\SkeletonMelee\\", 0x11D0 },		//27
	{ "wav\\Effects\\monster\\wolverin\\", 0x11E0 },		//28
	{ "wav\\Effects\\monster\\Rabie\\", 0x1200 },		//29
	{ "wav\\Effects\\monster\\Mudy\\", 0x1210 },		//30
	{ "wav\\Effects\\monster\\Sen\\", 0x1220 },		//31
	{ "wav\\Effects\\monster\\Egan\\", 0x1230 },		//32
	{ "wav\\Effects\\monster\\BeeDog\\", 0x1240 },		//33
	{ "wav\\Effects\\monster\\MutantPlant\\", 0x1250 },		//34
	{ "wav\\Effects\\monster\\MutantRabie\\", 0x1260 },		//35
	{ "wav\\Effects\\monster\\MutantTree\\", 0x1270 },		//36
	{ "wav\\Effects\\monster\\Avelisk\\", 0x1280 },		//37
	{ "wav\\Effects\\monster\\Naz\\", 0x1290 },		//38
	{ "wav\\Effects\\monster\\Mummy\\", 0x12A0 },		//39
	{ "wav\\Effects\\monster\\Hulk\\", 0x12B0 },		//40
	{ "wav\\Effects\\monster\\Hungky\\", 0x1330 },		//41
	{ "wav\\Effects\\monster\\Succubus\\", 0x12C0 },		//42
	{ "wav\\Effects\\monster\\Dawlin\\", 0x12D0 },		//43
	{ "wav\\Effects\\monster\\Shadow\\", 0x12E0 },		//44
	{ "wav\\Effects\\monster\\Berserker\\", 0x12F0 },		//45
	{ "wav\\Effects\\monster\\IronGuard\\", 0x1300 },		//46
	{ "wav\\Effects\\monster\\Fury\\", 0x1310 },		//47
	{ "wav\\Effects\\monster\\Sliver\\", 0x1320 },		//48
	{ "wav\\Effects\\monster\\Ratoo\\", 0x1340 },		//49
	{ "wav\\Effects\\monster\\Stygianlord\\", 0x1350 },		//50
	{ "wav\\Effects\\monster\\Omicron\\", 0x1360 },		//51
	{ "wav\\Effects\\monster\\D-Machine\\", 0x1370 },		//52
	{ "wav\\Effects\\monster\\Metron\\", 0x1380 },		//53
	{ "wav\\Effects\\monster\\MrGhost\\", 0x1390 },		//54
	{ "wav\\Effects\\monster\\VAMPIRICBAT\\", 0x13A0 },		//55
	{ "wav\\Effects\\monster\\MIREKEEPER\\", 0x13B0 },		//56
	{ "wav\\Effects\\monster\\MUFFIN\\", 0x13C0 },		//57
	{ "wav\\Effects\\monster\\SOLIDSNAIL\\", 0x13D0 },		//58
	{ "wav\\Effects\\monster\\BEEVIL\\", 0x13E0 },		//59
	{ "wav\\Effects\\monster\\NIGHTMARE\\", 0x1400 },		//60
	{ "wav\\Effects\\monster\\STONEGOLEM\\", 0x1410 },		//61
	{ "wav\\Effects\\monster\\THORNCRAWLER\\", 0x1420 },		//62
	{ "wav\\Effects\\monster\\HEAVYGOBLIN\\", 0x1430 },		//63
	{ "wav\\Effects\\monster\\EVILPLANT\\", 0x1440 },		//64
	{ "wav\\Effects\\monster\\HAUNTINGPLANT\\", 0x1450 },		//65
	{ "wav\\Effects\\monster\\DARKKNIGHT\\", 0x1460 },		//66
	{ "wav\\Effects\\monster\\Guardian-Saint\\", 0x1470 },		//67
	{ "wav\\Effects\\monster\\ChainGolem\\", 0x1500 },		//68
	{ "wav\\Effects\\monster\\DeadZone\\", 0x1510 },		//69
	{ "wav\\Effects\\monster\\Grotesque\\", 0x1520 },		//70
	{ "wav\\Effects\\monster\\Hypermachine\\", 0x1530 },		//71
	{ "wav\\Effects\\monster\\IronFist\\", 0x1540 },		//72
	{ "wav\\Effects\\monster\\Morgon\\", 0x1550 },		//73
	{ "wav\\Effects\\monster\\Mountain\\", 0x1560 },		//74
	{ "wav\\Effects\\monster\\Rampage\\", 0x1570 },		//75
	{ "wav\\Effects\\monster\\RunicGuardian\\", 0x1580 },		//76
	{ "wav\\Effects\\monster\\Sadness\\", 0x1590 },		//77
	{ "wav\\Effects\\monster\\TowerGolem\\", 0x15A0 },		//78
	{ "wav\\Effects\\monster\\VampiricBee\\", 0x15B0 },		//79
	{ "wav\\Effects\\monster\\VampiricMachine\\", 0x15C0 },		//80
	{ "wav\\Effects\\monster\\Omu\\", 0x15D0 },		//81
	{ "wav\\Effects\\monster\\AvelinArcher\\", 0x15E0 },		//82
	{ "wav\\Effects\\monster\\AvelinQueen\\", 0x15F0 },		//83
	{ "wav\\Effects\\monster\\Babel\\", 0x1600 },		//84
	{ "wav\\Effects\\monster\\Mystic\\", 0x1610 },		//85
	{ "wav\\Effects\\monster\\HEAVYGOBLIN\\", 0x1620 },		//86
	{ "wav\\Effects\\monster\\ColdEye\\", 0x1630 },		//87
	{ "wav\\Effects\\monster\\Frozen\\", 0x1640 },		//88
	{ "wav\\Effects\\monster\\STONEGOLEM\\", 0x1650 },		//89
	{ "wav\\Effects\\monster\\Frost\\", 0x1660 },		//90
	{ "wav\\Effects\\monster\\ChaosCara\\", 0x1670 },		//91
	{ "wav\\Effects\\monster\\DeathKnight\\", 0x1680 },		//92
	{ "wav\\Effects\\monster\\GreateGreven\\", 0x1700 },		//93
	{ "wav\\Effects\\monster\\LizardFolk\\", 0x1710 },		//94
	{ "wav\\Effects\\monster\\M_Lord\\", 0x1720 },		//95
	{ "wav\\Effects\\monster\\Spider\\", 0x1730 },		//96
	{ "wav\\Effects\\monster\\Stingray\\", 0x1740 },		//97
	{ "wav\\Effects\\monster\\Strider\\", 0x1750 },		//98
	{ "wav\\Effects\\monster\\TurtleCannon\\", 0x1800 },		//99
	{ "wav\\Effects\\monster\\DevilBird\\", 0x1810 },		//100
	{ "wav\\Effects\\monster\\BlizzardGiant\\", 0x1820 },		//101
	{ "wav\\Effects\\monster\\Kelvezu\\", 0x1830 },		//102
	{ "wav\\Effects\\monster\\DarkPhalanx\\", 0x1840 },		//103
	{ "wav\\Effects\\monster\\BloodyKnight\\", 0x1850 },		//104
	{ "wav\\Effects\\monster\\Chimera\\", 0x1860 },		//105
	{ "wav\\Effects\\monster\\FireWorm\\", 0x1870 },		//106
	{ "wav\\Effects\\monster\\HellHound\\", 0x1880 },		//107
	{ "wav\\Effects\\monster\\DarkGuard\\", 0x1890 },		//108
	{ "wav\\Effects\\monster\\DarkMage\\", 0x18A0 },		//109
	{ "wav\\Effects\\monster\\MonMokova\\", 0x18B0 },		//110
	{ "wav\\Effects\\monster\\MonTempleguard\\", 0x18C0 },		//111
	{ "wav\\Effects\\monster\\MonSeto\\", 0x18D0 },		//112
	{ "wav\\Effects\\monster\\MonKingSpider\\", 0x18E0 },		//113
	{ "wav\\Effects\\monster\\d_kn\\", 0x18F0 },		//114
	{ "wav\\Effects\\monster\\d_magi\\", 0x1900 },		//115
	{ "wav\\Effects\\monster\\d_atal\\", 0x1920 },		//116
	{ "wav\\Effects\\monster\\d_fi\\", 0x1930 },		//117
	{ "wav\\Effects\\monster\\d_ar\\", 0x1910 },		//118
	{ "wav\\Effects\\monster\\d_meca\\", 0x1940 },		//119
	{ "wav\\Effects\\monster\\d_pa\\", 0x1950 },		//120
	{ "wav\\Effects\\monster\\d_pr\\", 0x1960 },		//121
	{ "wav\\Effects\\monster\\deadhopt\\", 0x1970 },		//122
	{ "wav\\Effects\\monster\\deadkinghopy\\", 0x1980 },		//123
	{ "wav\\Effects\\monster\\gorgon\\", 0x1990 },		//124
	{ "wav\\Effects\\monster\\hobogolem\\", 0x19A0 },		//125
	{ "wav\\Effects\\monster\\Niken\\", 0x19B0 },		//126
	{ "wav\\Effects\\monster\\Mimic\\", 0x19C0 },		//127
	{ "wav\\Effects\\monster\\KingBat\\", 0x19D0 },		//128
	{ "wav\\Effects\\monster\\GoblinShaman\\", 0x19E0 },		//129
	{ "wav\\Effects\\monster\\Hest\\", 0x19F0 },		//130
	{ "wav\\Effects\\monster\\Ruca\\", 0x2100 },		//131
	{ "wav\\Effects\\monster\\NazSenior\\", 0x2110 },		//132
	{ "wav\\Effects\\monster\\Igolation\\", 0x2120 },		//133
	{ "wav\\Effects\\monster\\Kakoa\\", 0x2130 },		//134
	{ "wav\\Effects\\monster\\Sprin\\", 0x2140 },		//135
	{ "wav\\Effects\\monster\\UndeadMaple\\", 0x2150 },		//136
	{ "wav\\Effects\\monster\\Xetan\\", 0x2160 },		//137
	{ "wav\\Effects\\monster\\DevilShy\\", 0x21D0 },		//138
	{ "wav\\Effects\\monster\\Bebechick\\", 0x2170 },		//139
	{ "wav\\Effects\\monster\\PaPachick\\", 0x2180 },		//140
	{ "wav\\Effects\\monster\\watermelon\\", 0x2190 },		//141
	{ "wav\\Effects\\monster\\Bear\\", 0x21A0 },		//142
	{ "wav\\Effects\\monster\\PumpkinGhost\\", 0x21B0 },		//143
	{ "wav\\Effects\\monster\\Snowman\\", 0x21C0 },		//144
	{ "wav\\Effects\\monster\\sakura\\", 0x21E0 },		//145
	{ "wav\\Effects\\monster\\Cstone\\", 0x2410 },		//146
	{ "wav\\Effects\\monster\\Mountain\\", 0x2420 },		//147
	{ "wav\\Effects\\monster\\Morgon\\", 0x2430 },		//148
	{ "wav\\Effects\\monster\\ColdEye\\", 0x2440 },		//149
	{ "wav\\Effects\\monster\\TurtleCannon\\", 0x2450 },		//150
	{ "wav\\Effects\\monster\\HellHound\\", 0x2460 },		//151
	{ "wav\\Effects\\monster\\IceSerpent\\", 0x2200 },		//152
	{ "wav\\Effects\\monster\\IceWorm\\", 0x2210 },		//153
	{ "wav\\Effects\\monster\\Minebat\\", 0x2220 },		//154
	{ "wav\\Effects\\monster\\MineGolem\\", 0x2230 },		//155
	{ "wav\\Effects\\monster\\SealCrasher\\", 0x2240 },		//156
	{ "wav\\Effects\\monster\\Tarantulika\\", 0x2250 },		//157
	{ "wav\\Effects\\monster\\Tulla\\", 0x2260 },		//158
	{ "wav\\Effects\\monster\\UndeadManager\\", 0x2270 },		//159
	{ "wav\\Effects\\monster\\UndeadMiner\\", 0x2280 },		//160
	{ "wav\\Effects\\monster\\ITechnician\\", 0x2300 },		//161
	{ "wav\\Effects\\monster\\LTechnician\\", 0x2301 },		//162
	{ "wav\\Effects\\monster\\LEngineer\\", 0x2302 },		//163
	{ "wav\\Effects\\monster\\ISoldier\\", 0x2303 },		//164
	{ "wav\\Effects\\monster\\IBomber\\", 0x2304 },		//165
	{ "wav\\Effects\\monster\\LGuardian\\", 0x2305 },		//166
	{ "wav\\Effects\\monster\\IElite\\", 0x2306 },		//167
	{ "wav\\Effects\\monster\\Draxos\\", 0x2307 },		//168
	{ "wav\\Effects\\monster\\Lizard_Bigmama\\", MONSTEREFFECTID_LizardBigmama },		//169
	{ "wav\\Effects\\monster\\Lizard_Commander\\", MONSTEREFFECTID_LizardCommander },		//170
	{ "wav\\Effects\\monster\\Lizard_Doctor\\", MONSTEREFFECTID_LizardDoctor },		//171
	{ "wav\\Effects\\monster\\Lizard_Elder\\", MONSTEREFFECTID_LizardElder },		//172
	{ "wav\\Effects\\monster\\Lizard_Papa\\", MONSTEREFFECTID_LizardPapa },		//173
	{ "wav\\Effects\\monster\\Lizard_Soldier\\", MONSTEREFFECTID_LizardSoldier },		//174
	{ "wav\\Effects\\npc\\castledoor\\", 0x230E },		//175
	{ "wav\\Effects\\monster\\Billy\\", 0x2290 },		//176
	{ "wav\\Effects\\monster\\Hound\\", 0x22A0 },		//177
	{ "wav\\Effects\\monster\\Lena\\", 0x22B0 },		//178
	{ "wav\\Effects\\monster\\marionette\\", 0x22C0 },		//179
	{ "wav\\Effects\\monster\\UndeadStalker\\", 0x22D0 },		//180
	{ "wav\\Effects\\npc\\castledoor\\", 0x22E0 },		//181
	{ "wav\\Effects\\monster\\Hestian\\", 0x2470 },		//182
	{ "wav\\Effects\\npc\\morif\\", 0x2010 },		//183
	{ "wav\\Effects\\npc\\mollywolf\\", 0x2012 },		//184
	{ "wav\\Effects\\npc\\SkillMaster\\", 0x2020 },		//185
	{ "wav\\Effects\\npc\\MagicMaster\\", 0x2030 },		//186
	{ "wav\\Effects\\npc\\worldcup\\", 0x1A00 },		//187
	{ "wav\\Effects\\npc\\castledoor\\", 0x5010 },		//188
	{ "wav\\Effects\\npc\\crystal_r\\", 0x5020 },		//189
	{ "wav\\Effects\\npc\\crystal_g\\", 0x5021 },		//190
	{ "wav\\Effects\\npc\\crystal_b\\", 0x5022 },		//191
	{ "wav\\Effects\\npc\\crystal_n\\", 0x5023 },		//192
	{ "wav\\Effects\\npc\\tower-b\\", 0x5030 },		//193
	{ "wav\\Effects\\npc\\Soldier_A\\", 0x5100 },		//194
	{ "wav\\Effects\\npc\\Soldier_B\\", 0x5110 },		//195
	{ "wav\\Effects\\npc\\Soldier_C\\", 0x5120 },		//196
	{ "wav\\Effects\\npc\\Soldier_D\\", 0x5130 },		//197
	{ "wav\\Effects\\monster\\wolverin\\", 0x3010 },		//198
	{ "wav\\Effects\\monster\\MetalGolem\\", 0x3020 },		//199
	{ "wav\\Effects\\monster\\Figon\\", 0x3030 },		//200
	{ "wav\\Effects\\player\\fighter\\", 0x1 },		//201
	{ "wav\\Effects\\player\\Mechanician\\", 0x2 },		//202
	{ "wav\\Effects\\player\\archer\\", 0x3 },		//203
	{ "wav\\Effects\\player\\pikeman\\", 0x4 },		//204
	{ "wav\\Effects\\player\\Assassin\\", 0x9 },		//205
	{ "wav\\Effects\\player\\Knight\\", 0x6 },		//206
	{ "wav\\Effects\\player\\Atalanta\\", 0x5 },		//207
	{ "wav\\Effects\\player\\Priestess\\", 0x8 },		//208
	{ "wav\\Effects\\player\\Magician\\", 0x7 },		//209
	{ "wav\\Effects\\player\\Shaman\\", 0xA },		//210
	{ "wav\\Effects\\monster\\Coma\\", 0x21F0 },		//211
	{ "wav\\Effects\\monster\\Boitata\\", MONSTEREFFECTID_BOITATA },		//212
	{ "wav\\Effects\\monster\\BrazilBear\\", 0x21F1 },		//213
	{ "game\\sounds\\monsters\\Dey\\", MONSTEREFFECTID_Dey },		//214
	{ "game\\sounds\\monsters\\Gurkob\\", MONSTEREFFECTID_Gurkob },		//215
	{ "game\\sounds\\monsters\\Shogoth\\", MONSTEREFFECTID_Shogoth },		//216
	{ "game\\sounds\\monsters\\Arhdyra\\", MONSTEREFFECTID_Arhdyra },		//217
	{ "game\\sounds\\monsters\\Sathla\\", MONSTEREFFECTID_Sathla },		//218
	{ "game\\sounds\\monsters\\Faugn\\", MONSTEREFFECTID_Faugn },		//219
	{ "game\\sounds\\monsters\\Koon\\", MONSTEREFFECTID_Koon },		//220
	{ "game\\sounds\\monsters\\Yagditha\\", MONSTEREFFECTID_Yagditha },		//221
	{ "wav\\Effects\\monster\\Frozen\\", MONSTEREFFECTID_IceElemental },		//222
	{ "wav\\Effects\\monster\\Tiger\\", MONSTEREFFECTID_SummonTiger },		//223

	{ "wav\\Effects\\monster\\ISoldier\\", MONSTEREFFECTID_ISoldier },		//224
	{ "wav\\Effects\\monster\\IBomber\\", MONSTEREFFECTID_IBomber },		//225
	{ "wav\\Effects\\monster\\ITechnician\\", MONSTEREFFECTID_ITechnician },		//226
	{ "wav\\Effects\\monster\\LGuardian\\", MONSTEREFFECTID_LGuardian },		//227
	{ "wav\\Effects\\monster\\LTechnician\\", MONSTEREFFECTID_LTechnician },		//228
	{ "wav\\Effects\\monster\\LEngineer\\", MONSTEREFFECTID_LEngineer },		//229
	{ "wav\\Effects\\monster\\IElite\\", MONSTEREFFECTID_IElite },		//230
	{ "wav\\Effects\\monster\\Draxos\\", MONSTEREFFECTID_Draxos },		//231
	{"wav\\Effects\\monster\\Guardian-Saint\\", MONSTEREFFECTID_DarkGuardian},		//232
	{"wav\\Effects\\monster\\Igniculus\\", MONSTEREFFECTID_Igniculus},		//233
	{"wav\\Effects\\monster\\LavaArmor\\", MONSTEREFFECTID_LavaArmor},		//234
	{ "wav\\Effects\\monster\\LavaArcher\\", MONSTEREFFECTID_LavaArcher },  //235
	{ "wav\\Effects\\monster\\LavaGolem\\", MONSTEREFFECTID_LavaGolem },   //236
	{ "wav\\Effects\\monster\\LavaGiant\\", MONSTEREFFECTID_LavaGiant },   //237
	{ "wav\\Effects\\monster\\Insec\\", MONSTEREFFECTID_Insec },			//238
	{ "wav\\Effects\\monster\\Flamo\\", MONSTEREFFECTID_Flamo },			//239
	{ "wav\\Effects\\monster\\FlameMaiden\\", MONSTEREFFECTID_FlameMaiden },//240
	{"wav\\Effects\\monster\\rPT_Freddy\\", MONSTEREFFECTID_Freddy},//241
	{"wav\\Effects\\monster\\rPT_Pelgia\\", MONSTEREFFECTID_Pelgia},//242
	{"wav\\Effects\\monster\\rPT_GrassBeast\\", MONSTEREFFECTID_GrassBeast},//243
	{"wav\\Effects\\monster\\rPT_TropicalDemon\\", MONSTEREFFECTID_TropicalDemon},//244
	{"wav\\Effects\\monster\\rPT_TropicalFlower\\", MONSTEREFFECTID_TropicalFlower},//245
	{"wav\\Effects\\monster\\rPT_TropicalRanger\\", MONSTEREFFECTID_TropicalRanger},//246
	{"wav\\Effects\\monster\\rPT_Pelgia\\", MONSTEREFFECTID_Yeti},//247
	{"wav\\Effects\\monster\\Acero\\", MONSTEREFFECTID_Acero},//248
	{"wav\\Effects\\monster\\Nihil\\", MONSTEREFFECTID_Nihil},//249
	{"wav\\Effects\\monster\\Chalybs\\", MONSTEREFFECTID_Chalybs},//250
	{"wav\\Effects\\monster\\Greedy\\", MONSTEREFFECTID_Greedy},//251
	{"wav\\Effects\\monster\\Ancient_Guardian\\", MONSTEREFFECTID_AncientGuardian},//252
	{"wav\\Effects\\monster\\Volcanos\\", MONSTEREFFECTID_Volcanos},//253
	{ "wav\\Effects\\monster\\Tyrcus\\", MONSTEREFFECTID_Tyrcus},//254
	{ "wav\\Effects\\monster\\Volcanos\\", MONSTEREFFECTID_Hagios },//255

	{ NULL, 0 },		//256
};

struct ItemRequeriments
{
	DWORD	iClass;
	short	sLevel[2];
	short	sStrength[2];
	short	sSpirit[2];
	short	sTalent[2];
	short	sAgility[2];
	short	sHealth[2];
};

static ItemRequeriments saItemRequeriments[10+1]
{
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

	{ CLASSFLAG_Fighter, 0, 0, 20, 25, -20, -10, 0, 0, -20, -15, 0, 0 },
	{ CLASSFLAG_Mechanician, 0, 0, 15, 25, -20, -10, 0, 0, -20, -15, 0, 0 },
	{ CLASSFLAG_Archer, 0, 0, -25, -15, -20, -10, 0, 0, 20, 30, 0, 0 },
	{ CLASSFLAG_Pikeman, 0, 0, 20, 25, -20, -10, 0, 0, -20, -15, 0, 0, },

	{ CLASSFLAG_Atalanta, 0, 0, -20, -15, -20, -10, 0, 0, 20, 30, 0, 0 },
	{ CLASSFLAG_Knight,	 0, 0, 15, 25, -15, -10, 5, 10, -20, -15, 0, 0 },
	{ CLASSFLAG_Magician, 0, 0, -30, -25, 25, 30, -15, -10, -20, -15, 0, 0 },
	{ CLASSFLAG_Priestess, 0, 0, -30, -25, 25, 30, -15, -10, -20, -15, 0, 0 },
	{ CLASSFLAG_Assassin, 0, 0, 20, 25, -20, -10, 0, 0, -20, -15, 0, 0 },
	{ CLASSFLAG_Shaman, 0, 0, -30, -25, 25, 30, -15, -10, -20, -15, 0, 0 },
};

struct CharacterClassData
{
	char				* pszName;
	char				* pszNameTranslated;

	int					iClassBit;
	int					iClass;
	int					iRace;

	int					iSight;
	int					iHP;
	int					iMP;
	int					iSP;

	short				sDamage[2];

	short				sUnk;
}; //Size = 0x2C

static CharacterClassData saCharacterClassData[]
{
	//Tier 1 - Tempskrons
	{ "Mechanician", "Mechanician", CLASSFLAG_Mechanician, 2, 0, 65, 2, 2, 2, 2, 2, 0 },
	{ "Fighter", "Fighter", CLASSFLAG_Fighter, 1, 0, 70, 1, 3, 1, 1, 2, 0 },
	{ "Pikeman", "Pikeman", CLASSFLAG_Pikeman, 4, 0, 75, 1, 3, 1, 1, 2, 0 },
	{ "Archer", "Archer", CLASSFLAG_Archer, 3, 0, 80, 3, 3, 2, 3, 1, 0 },
	{ "Assassin", "Assassin", CLASSFLAG_Assassin, 9, 0, 80, 3, 3, 2, 3, 1, 0 },

	//Tier 2
	{ "Mechanic Master", "Mechanic Master", 0x00000010, 2, 1 },
	{ "Warrior", "Warrior", 0x00000020, 1, 1 },
	{ "Combatant", "Combatant", 0x00000040, 4, 1 },
	{ "Hunter Master", "Huntress Master", 0x00000080, 3, 1 },
	{ "Rogue", "Rogue", 0x00000050, 9, 1 },

	//Tier 3
	{ "Metal Leader", "Metal Leader", 0x00000100, 2, 1 },
	{ "Champion", "Champion", 0x00000200, 1, 1 },
	{ "Lancer", "Lancer", 0x00000400, 4, 1 },
	{ "Dion's Disciple", "Dion's Disciple", 0x00000800, 3, 1 },
	{ "Shadow", "Shadow", 0x00000500, 9, 1 },

	//Tier 4
	{ "Metallion", "Heavy Metal", 0x00001000, 2, 1 },
	{ "Immortal Warrior", "Immortal Warrior", 0x00002000, 1, 1 },
	{ "Lancelot", "Lancelot", 0x00004000, 4, 1 },
	{ "Sagittarion", "Sagittarius", 0x00008000, 3, 1 },
	{ "Shadow Walker", "Shadow Walker", 0x00005000, 9, 1 },

	//Tier 5
	{ "Metalrage", "The Engineer", 0x00010000, 2, 1 },
	{ "Warlord", "Zecram", 0x00020000, 1, 1 },
	{ "Baylolunt", "Reaper", 0x00040000, 4, 1 },
	{ "Iskar", "Wind Ranger", 0x00080000, 3, 1 },
	{ "Shadow Master", "Spectre", 0x00050000, 9, 1 },

	//Tier 1 - Morions
	{ "Knight", "Knight", CLASSFLAG_Knight, 6, 0, 190, 2, 2, 2, 1, 2, 0 },
	{ "Atalanta", "Atalanta", CLASSFLAG_Atalanta, 5, 0, 200, 2, 2, 2, 2, 1, 0 },
	{ "Priestess", "Priestess", CLASSFLAG_Priestess, 8, 0, 200, 4, 1, 3, 3, 2, 2 },
	{ "Magician", "Magician", CLASSFLAG_Magician, 7, 0, 190, 5, 1, 3, 3, 2, 1 },
	{ "Shaman", "Shaman", CLASSFLAG_Shaman, 10, 0, 190, 5, 1, 3, 3, 2, 1 },

	//Tier 2
	{ "Paladin", "Paladin", 0x00100000, 6, 2 },
	{ "Valkyrie", "Valkyrie", 0x00200000, 5, 2 },
	{ "Saintess", "Saintess", 0x00400000, 8, 2 },
	{ "Wizard", "Wizard", 0x00800000, 7, 2 },
	{ "Clairvoyant", "Clairvoyant", 0x00500000, 10, 2 },

	//Tier 3
	{ "HolyKnight", "Holy Knight", 0x01000000, 6, 2 },
	{ "Brunhild", "Brynhild", 0x02000000, 5, 2 },
	{ "Bishop", "Bishop", 0x04000000, 8, 2 },
	{ "RoyalWizard", "Royal Wizard", 0x08000000, 7, 2 },
	{ "Soul Mage", "Soul Mage", 0x05000000, 10, 2 },

	//Tier 4
	{ "SaintKnight", "Saint Knight", 0x10000000, 6, 2 },
	{ "Valhalla", "Valhalla", 0x20000000, 5, 2 },
	{ "Celestial", "Celestial", 0x40000000, 8, 2 },
	{ "ArchMage", "Arch Mage", 0x80000000, 7, 2 },
	{ "Necromancer", "Necromancer", 0x50000000, 10, 2 },

	//Tier 5
	{ "Valiant", "Guardian", 0x10010000, 6, 2 },
	{ "Queen of Valhalla", "Titanesses", 0x20020000, 5, 2 },
	{ "Arc Angel", "Lightbringer", 0x40040000, 8, 2 },
	{ "Elemental Master", "Elementalist", 0x80080000, 7, 2 },
	{ "Chaotic Oracle", "Occultist", 0x50050000, 10, 2 },

	{ "", "", 0, 10, 0, 190, 5, 1, 3, 3, 2, 1 },
};

struct CharacterBody
{
	EItemID				eItemID;
	ECharacterClass		iClass;
	char				* pszFileName;
};

static CharacterBody saCharacterBodyLoadDefault[]
{
	{ ITEMID_None, CHARACTERCLASS_Fighter, "char\\tmABCD\\B001.ini" },
	{ ITEMID_None, CHARACTERCLASS_Mechanician, "char\\tmABCD\\A001.ini" },
	{ ITEMID_None, CHARACTERCLASS_Archer, "char\\tmABCD\\D001.ini" },
	{ ITEMID_None, CHARACTERCLASS_Pikeman, "char\\tmABCD\\C001.ini" },
	{ ITEMID_None, CHARACTERCLASS_Atalanta, "char\\tmABCD\\mB001.ini" },
	{ ITEMID_None, CHARACTERCLASS_Knight, "char\\tmABCD\\mA001.ini" },
	{ ITEMID_None, CHARACTERCLASS_Magician, "char\\tmABCD\\mD001.ini" },
	{ ITEMID_None, CHARACTERCLASS_Priestess, "char\\tmABCD\\mC001.ini" },
	{ ITEMID_None, CHARACTERCLASS_Assassin, "char\\tmABCD\\E001.ini" },
	{ ITEMID_None, CHARACTERCLASS_Shaman, "char\\tmABCD\\mE001.ini" },
};

static CharacterBody saCharacterBodyList[]
{
	//Custom Armor
	{(EItemID)0x0201510A, CHARACTERCLASS_None, "char\\tmABCD\\Cb301FA.ini"},		//Stylish						
	{(EItemID)0x02015200, CHARACTERCLASS_None, "char\\tmABCD\\Cb302.ini"},		//Destroyer Armor				
	{(EItemID)0x02015300, CHARACTERCLASS_None, "char\\tmABCD\\ArmorGMRPT.ini"},	//Shemale					
//	{(EItemID)0x02015300, CHARACTERCLASS_None, "char\\tmABCD\\ar0nugly02.ini"},	//Shemale					

																				//Viper Armor
	{ ITEMID_ViperArmor, CHARACTERCLASS_Fighter, "char\\tmABCD\\b301.ini" },
	{ ITEMID_ViperArmor, CHARACTERCLASS_Mechanician, "char\\tmABCD\\a301.ini" },
	{ ITEMID_ViperArmor, CHARACTERCLASS_Archer, "char\\tmABCD\\d301.ini" },
	{ ITEMID_ViperArmor, CHARACTERCLASS_Pikeman, "char\\tmABCD\\c301.ini" },
	{ ITEMID_ViperArmor, CHARACTERCLASS_Atalanta, "char\\tmABCD\\mb301.ini" },
	{ ITEMID_ViperArmor, CHARACTERCLASS_Knight, "char\\tmABCD\\ma301.ini" },
	{ ITEMID_PhoenixRobe, CHARACTERCLASS_Magician, "char\\tmABCD\\md301.ini" },
	{ ITEMID_PhoenixRobe, CHARACTERCLASS_Priestess, "char\\tmABCD\\mc301.ini" },
	{ ITEMID_ViperArmor, CHARACTERCLASS_Assassin, "char\\tmABCD\\e301.ini" },
	{ ITEMID_PhoenixRobe, CHARACTERCLASS_Shaman,"char\\tmABCD\\me301.ini" },

	//Inferno Armor
	{ ITEMID_InfernoArmor, CHARACTERCLASS_Fighter, "char\\tmABCD\\b113.ini" },
	{ ITEMID_InfernoArmor, CHARACTERCLASS_Mechanician, "char\\tmABCD\\a113.ini" },
	{ ITEMID_InfernoArmor, CHARACTERCLASS_Archer, "char\\tmABCD\\d113.ini" },
	{ ITEMID_InfernoArmor, CHARACTERCLASS_Pikeman, "char\\tmABCD\\c113.ini" },
	{ ITEMID_InfernoArmor, CHARACTERCLASS_Atalanta, "char\\tmABCD\\mb113.ini" },
	{ ITEMID_InfernoArmor, CHARACTERCLASS_Knight, "char\\tmABCD\\ma113.ini" },
	{ ITEMID_InfernoRobe, CHARACTERCLASS_Magician, "char\\tmABCD\\md113.ini" },
	{ ITEMID_InfernoRobe, CHARACTERCLASS_Priestess, "char\\tmABCD\\mc113.ini" },
	{ ITEMID_InfernoArmor, CHARACTERCLASS_Assassin, "char\\tmABCD\\e113.ini" },
	{ ITEMID_InfernoRobe, CHARACTERCLASS_Shaman,"char\\tmABCD\\me113.ini" },

	//Hades Armor
	{ ITEMID_HadesArmor, CHARACTERCLASS_Fighter, "char\\tmABCD\\b114.ini" },
	{ ITEMID_HadesArmor, CHARACTERCLASS_Mechanician, "char\\tmABCD\\a114.ini" },
	{ ITEMID_HadesArmor, CHARACTERCLASS_Archer, "char\\tmABCD\\d114.ini" },
	{ ITEMID_HadesArmor, CHARACTERCLASS_Pikeman, "char\\tmABCD\\c114.ini" },
	{ ITEMID_HadesArmor, CHARACTERCLASS_Atalanta, "char\\tmABCD\\mb114.ini" },
	{ ITEMID_HadesArmor, CHARACTERCLASS_Knight, "char\\tmABCD\\ma114.ini" },
	{ ITEMID_ElfRobe, CHARACTERCLASS_Magician, "char\\tmABCD\\md114.ini" },
	{ ITEMID_ElfRobe, CHARACTERCLASS_Priestess, "char\\tmABCD\\mc114.ini" },
	{ ITEMID_HadesArmor, CHARACTERCLASS_Assassin, "char\\tmABCD\\e114.ini" },
	{ ITEMID_ElfRobe, CHARACTERCLASS_Shaman,"char\\tmABCD\\me114.ini" },

	//Mythril Armor
	{ ITEMID_MythrilArmor, CHARACTERCLASS_Fighter, "char\\tmABCD\\b115.ini" },
	{ ITEMID_MythrilArmor, CHARACTERCLASS_Mechanician, "char\\tmABCD\\a115.ini" },
	{ ITEMID_MythrilArmor, CHARACTERCLASS_Archer, "char\\tmABCD\\d115.ini" },
	{ ITEMID_MythrilArmor, CHARACTERCLASS_Pikeman, "char\\tmABCD\\c115.ini" },
	{ ITEMID_MythrilArmor, CHARACTERCLASS_Atalanta, "char\\tmABCD\\mb115.ini" },
	{ ITEMID_MythrilArmor, CHARACTERCLASS_Knight, "char\\tmABCD\\ma115.ini" },
	{ ITEMID_LunarRobe, CHARACTERCLASS_Magician, "char\\tmABCD\\md115.ini" },
	{ ITEMID_LunarRobe, CHARACTERCLASS_Priestess, "char\\tmABCD\\mc115.ini" },
	{ ITEMID_MythrilArmor, CHARACTERCLASS_Assassin, "char\\tmABCD\\e115.ini" },
	{ ITEMID_LunarRobe, CHARACTERCLASS_Shaman,"char\\tmABCD\\me115.ini" },

	//Imperial Armor
	{ ITEMID_ImperialArmor, CHARACTERCLASS_Fighter, "char\\tmABCD\\b116.ini" },
	{ ITEMID_ImperialArmor, CHARACTERCLASS_Mechanician, "char\\tmABCD\\a116.ini" },
	{ ITEMID_ImperialArmor, CHARACTERCLASS_Archer, "char\\tmABCD\\d116.ini" },
	{ ITEMID_ImperialArmor, CHARACTERCLASS_Pikeman, "char\\tmABCD\\c116.ini" },
	{ ITEMID_ImperialArmor, CHARACTERCLASS_Atalanta, "char\\tmABCD\\mb116.ini" },
	{ ITEMID_ImperialArmor, CHARACTERCLASS_Knight, "char\\tmABCD\\ma116.ini" },
	{ ITEMID_CherubRobe, CHARACTERCLASS_Magician, "char\\tmABCD\\md116.ini" },
	{ ITEMID_CherubRobe, CHARACTERCLASS_Priestess, "char\\tmABCD\\mc116.ini" },
	{ ITEMID_ImperialArmor, CHARACTERCLASS_Assassin, "char\\tmABCD\\e116.ini" },
	{ ITEMID_CherubRobe, CHARACTERCLASS_Shaman,"char\\tmABCD\\me116.ini" },

	//Mythology Armor
	{ ITEMID_MythologyArmor, CHARACTERCLASS_Fighter, "char\\tmABCD\\b027.ini" },
	{ ITEMID_MythologyArmor, CHARACTERCLASS_Mechanician, "char\\tmABCD\\a027.ini" },
	{ ITEMID_MythologyArmor, CHARACTERCLASS_Archer, "char\\tmABCD\\d027.ini" },
	{ ITEMID_MythologyArmor, CHARACTERCLASS_Pikeman, "char\\tmABCD\\c027.ini" },
	{ ITEMID_MythologyArmor, CHARACTERCLASS_Atalanta, "char\\tmABCD\\mb027.ini" },
	{ ITEMID_MythologyArmor, CHARACTERCLASS_Knight, "char\\tmABCD\\ma027.ini" },
	{ ITEMID_MythologyRobe, CHARACTERCLASS_Magician, "char\\tmABCD\\md027.ini" },
	{ ITEMID_MythologyRobe, CHARACTERCLASS_Priestess, "char\\tmABCD\\mc027.ini" },
	{ ITEMID_MythologyArmor, CHARACTERCLASS_Assassin, "char\\tmABCD\\e027.ini" },
	{ ITEMID_MythologyRobe, CHARACTERCLASS_Shaman,"char\\tmABCD\\me027.ini" },

	//Death Armor
	{ ITEMID_DeathArmor, CHARACTERCLASS_Fighter, "char\\tmABCD\\b028.ini" },
	{ ITEMID_DeathArmor, CHARACTERCLASS_Mechanician, "char\\tmABCD\\a028.ini" },
	{ ITEMID_DeathArmor, CHARACTERCLASS_Archer, "char\\tmABCD\\d028.ini" },
	{ ITEMID_DeathArmor, CHARACTERCLASS_Pikeman, "char\\tmABCD\\c028.ini" },
	{ ITEMID_DeathArmor, CHARACTERCLASS_Atalanta, "char\\tmABCD\\mb028.ini" },
	{ ITEMID_DeathArmor, CHARACTERCLASS_Knight, "char\\tmABCD\\ma028.ini" },
	{ ITEMID_DeathRobe, CHARACTERCLASS_Magician, "char\\tmABCD\\md028.ini" },
	{ ITEMID_DeathRobe, CHARACTERCLASS_Priestess, "char\\tmABCD\\mc028.ini" },
	{ ITEMID_DeathArmor, CHARACTERCLASS_Assassin, "char\\tmABCD\\e028.ini" },
	{ ITEMID_DeathRobe, CHARACTERCLASS_Shaman,"char\\tmABCD\\me028.ini" },

	//Sandurr Items
	{ ITEMID_QuantumArmor, CHARACTERCLASS_Fighter, "char\\tmABCD\\b031.ini" },
	{ ITEMID_QuantumArmor, CHARACTERCLASS_Mechanician, "char\\tmABCD\\a031.ini" },
	{ ITEMID_QuantumArmor, CHARACTERCLASS_Archer, "char\\tmABCD\\d031.ini" },
	{ ITEMID_QuantumArmor, CHARACTERCLASS_Pikeman, "char\\tmABCD\\c031.ini" },
	{ ITEMID_QuantumArmor, CHARACTERCLASS_Atalanta, "char\\tmABCD\\mb031.ini" },
	{ ITEMID_QuantumArmor, CHARACTERCLASS_Knight, "char\\tmABCD\\ma031.ini" },
	{ ITEMID_WarlockRobe, CHARACTERCLASS_Magician, "char\\tmABCD\\md031.ini" },
	{ ITEMID_WarlockRobe, CHARACTERCLASS_Priestess, "char\\tmABCD\\mc031.ini" },
	{ ITEMID_QuantumArmor, CHARACTERCLASS_Assassin, "char\\tmABCD\\e031.ini" },
	{ ITEMID_WarlockRobe, CHARACTERCLASS_Shaman,"char\\tmABCD\\me031.ini" },

	//Abyss Armor and Rober
	{ ITEMID_AbyssArmor, CHARACTERCLASS_Fighter, "char\\tmABCD\\b032.ini" },
	{ ITEMID_AbyssArmor, CHARACTERCLASS_Mechanician, "char\\tmABCD\\a032.ini" },
	{ ITEMID_AbyssArmor, CHARACTERCLASS_Archer, "char\\tmABCD\\d032.ini" },
	{ ITEMID_AbyssArmor, CHARACTERCLASS_Pikeman, "char\\tmABCD\\c032.ini" },
	{ ITEMID_AbyssArmor, CHARACTERCLASS_Atalanta, "char\\tmABCD\\mb032.ini" },
	{ ITEMID_AbyssArmor, CHARACTERCLASS_Knight, "char\\tmABCD\\ma032.ini" },
	{ ITEMID_AbyssRobe, CHARACTERCLASS_Magician, "char\\tmABCD\\md032.ini" },
	{ ITEMID_AbyssRobe, CHARACTERCLASS_Priestess, "char\\tmABCD\\mc032.ini" },
	{ ITEMID_AbyssArmor, CHARACTERCLASS_Assassin, "char\\tmABCD\\e032.ini" },
	{ ITEMID_AbyssRobe, CHARACTERCLASS_Shaman,"char\\tmABCD\\me032.ini" },

	//Costumes Halloween
	{ ITEMID_VampireSuit, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb031.ini" },
	{ ITEMID_VampireSuit, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca031.ini" },
	{ ITEMID_Succubus, CHARACTERCLASS_Archer, "char\\tmABCD\\cd031.ini" },
	{ ITEMID_VampireSuit, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc031.ini" },
	{ ITEMID_Succubus, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb031.ini" },
	{ ITEMID_VampireSuit, CHARACTERCLASS_Knight, "char\\tmABCD\\cma031.ini" },
	{ ITEMID_VampireSuit, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd031.ini" },
	{ ITEMID_Succubus, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc031.ini" },
	{ ITEMID_Succubus, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce031.ini" },
	{ ITEMID_VampireSuit, CHARACTERCLASS_Shaman,"char\\tmABCD\\cme031.ini" },

	//Soccer Suit
	{ ITEMID_SoccerShirt, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb005.ini" },
	{ ITEMID_SoccerShirt, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca005.ini" },
	{ ITEMID_SoccerShirt, CHARACTERCLASS_Archer, "char\\tmABCD\\cd005.ini" },
	{ ITEMID_SoccerShirt, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc005.ini" },
	{ ITEMID_SoccerShirt, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb005.ini" },
	{ ITEMID_SoccerShirt, CHARACTERCLASS_Knight, "char\\tmABCD\\cma005.ini" },
	{ ITEMID_SoccerShirt, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd005.ini" },
	{ ITEMID_SoccerShirt, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc005.ini" },
	{ ITEMID_SoccerShirt, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce005.ini" },
	{ ITEMID_SoccerShirt, CHARACTERCLASS_Shaman,"char\\tmABCD\\cme005.ini" },

	//Xmas
	{ ITEMID_XMasCloatCostumeTM, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb016.ini" },
	{ ITEMID_XMasCloatCostumeTM, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca016.ini" },
	{ ITEMID_XMasCloatCostumeTF, CHARACTERCLASS_Archer, "char\\tmABCD\\cd016.ini" },
	{ ITEMID_XMasCloatCostumeTM, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc016.ini" },
	{ ITEMID_XMasCloatCostumeTF, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb016.ini" },
	{ ITEMID_XMasCloatCostumeTM, CHARACTERCLASS_Knight, "char\\tmABCD\\cma016.ini" },
	{ ITEMID_XMasCloatCostumeTM, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd016.ini" },
	{ ITEMID_XMasCloatCostumeTF, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc016.ini" },
	{ ITEMID_XMasCloatCostumeTF, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce016.ini" },
	{ ITEMID_XMasCloatCostumeTM, CHARACTERCLASS_Shaman,"char\\tmABCD\\cme016.ini" },

	//Suits
	//Football Suit
	{ ITEMID_FootballM, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb011.ini" },
	{ ITEMID_FootballM, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca011.ini" },
	{ ITEMID_FootballF, CHARACTERCLASS_Archer, "char\\tmABCD\\cd011.ini" },
	{ ITEMID_FootballM, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc011.ini" },
	{ ITEMID_FootballF, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb011.ini" },
	{ ITEMID_FootballM, CHARACTERCLASS_Knight, "char\\tmABCD\\cma011.ini" },
	{ ITEMID_FootballM, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd011.ini" },
	{ ITEMID_FootballF, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc011.ini" },
	{ ITEMID_FootballF, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce011.ini" },
	{ ITEMID_FootballM, CHARACTERCLASS_Shaman,"char\\tmABCD\\cme011.ini" },

	//South Graceful Hanbok
	{ ITEMID_SouthGraceHanbokM, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb011.ini" },
	{ ITEMID_SouthGraceHanbokM, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca011.ini" },
	{ ITEMID_SouthGraceHanbokF, CHARACTERCLASS_Archer, "char\\tmABCD\\cd011.ini" },
	{ ITEMID_SouthGraceHanbokM, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc011.ini" },
	{ ITEMID_SouthGraceHanbokF, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb011.ini" },
	{ ITEMID_SouthGraceHanbokM, CHARACTERCLASS_Knight, "char\\tmABCD\\cma011.ini" },
	{ ITEMID_SouthGraceHanbokM, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd011.ini" },
	{ ITEMID_SouthGraceHanbokF, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc011.ini" },
	{ ITEMID_SouthGraceHanbokF, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce011.ini" },
	{ ITEMID_SouthGraceHanbokM, CHARACTERCLASS_Shaman,"char\\tmABCD\\cme011.ini" },

	//Magnificent Hanbok
	{ ITEMID_MagnificentHanbokM, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb028.ini" },
	{ ITEMID_MagnificentHanbokM, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca028.ini" },
	{ ITEMID_MagnificentHanbokF, CHARACTERCLASS_Archer, "char\\tmABCD\\cd028.ini" },
	{ ITEMID_MagnificentHanbokM, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc028.ini" },
	{ ITEMID_MagnificentHanbokF, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb028.ini" },
	{ ITEMID_MagnificentHanbokM, CHARACTERCLASS_Knight, "char\\tmABCD\\cma028.ini" },
	{ ITEMID_MagnificentHanbokM, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd028.ini" },
	{ ITEMID_MagnificentHanbokF, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc028.ini" },
	{ ITEMID_MagnificentHanbokF, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce028.ini" },
	{ ITEMID_MagnificentHanbokM, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme028.ini" },

	//Gray Suit/Dress
	{ ITEMID_GraySuitM, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb030.ini" },
	{ ITEMID_GraySuitM, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca030.ini" },
	{ ITEMID_GraySuitF, CHARACTERCLASS_Archer, "char\\tmABCD\\cd030.ini" },
	{ ITEMID_GraySuitM, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc030.ini" },
	{ ITEMID_GraySuitF, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb030.ini" },
	{ ITEMID_GraySuitM, CHARACTERCLASS_Knight, "char\\tmABCD\\cma030.ini" },
	{ ITEMID_GraySuitM, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd030.ini" },
	{ ITEMID_GraySuitF, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc030.ini" },
	{ ITEMID_GraySuitF, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce030.ini" },
	{ ITEMID_GraySuitM, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme030.ini" },

	//Romantic Suit
	{ ITEMID_RomanticSuitM, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb035.ini" },
	{ ITEMID_RomanticSuitM, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca035.ini" },
	{ ITEMID_RomanticSuitF, CHARACTERCLASS_Archer, "char\\tmABCD\\cd035.ini" },
	{ ITEMID_RomanticSuitM, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc035.ini" },
	{ ITEMID_RomanticSuitF, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb035.ini" },
	{ ITEMID_RomanticSuitM, CHARACTERCLASS_Knight, "char\\tmABCD\\cma035.ini" },
	{ ITEMID_RomanticSuitM, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd035.ini" },
	{ ITEMID_RomanticSuitF, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc035.ini" },
	{ ITEMID_RomanticSuitF, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce035.ini" },
	{ ITEMID_RomanticSuitM, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme035.ini" },

	//Mechanic Costume
	{ ITEMID_MechanicCostumeM, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb034.ini" },
	{ ITEMID_MechanicCostumeM, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca034.ini" },
	{ ITEMID_MechanicCostumeF, CHARACTERCLASS_Archer, "char\\tmABCD\\cd034.ini" },
	{ ITEMID_MechanicCostumeM, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc034.ini" },
	{ ITEMID_MechanicCostumeF, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb034.ini" },
	{ ITEMID_MechanicCostumeM, CHARACTERCLASS_Knight, "char\\tmABCD\\cma034.ini" },
	{ ITEMID_MechanicCostumeM, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd034.ini" },
	{ ITEMID_MechanicCostumeF, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc034.ini" },
	{ ITEMID_MechanicCostumeF, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce034.ini" },
	{ ITEMID_MechanicCostumeM, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme034.ini" },

	//Chinese Suit
	{ ITEMID_ChineseSuitM, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb017.ini" },
	{ ITEMID_ChineseSuitM, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca017.ini" },
	{ ITEMID_ChineseSuitF, CHARACTERCLASS_Archer, "char\\tmABCD\\cd017.ini" },
	{ ITEMID_ChineseSuitM, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc017.ini" },
	{ ITEMID_ChineseSuitF, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb017.ini" },
	{ ITEMID_ChineseSuitM, CHARACTERCLASS_Knight, "char\\tmABCD\\cma017.ini" },
	{ ITEMID_ChineseSuitM, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd017.ini" },
	{ ITEMID_ChineseSuitF, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc017.ini" },
	{ ITEMID_ChineseSuitF, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce017.ini" },
	{ ITEMID_ChineseSuitM, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme017.ini" },

	//Warriors Hanbok
	{ ITEMID_WarriorsHanbokM, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb014.ini" },
	{ ITEMID_WarriorsHanbokM, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca014.ini" },
	{ ITEMID_WarriorsHanbokF, CHARACTERCLASS_Archer, "char\\tmABCD\\cd014.ini" },
	{ ITEMID_WarriorsHanbokM, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc014.ini" },
	{ ITEMID_WarriorsHanbokF, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb014.ini" },
	{ ITEMID_WarriorsHanbokM, CHARACTERCLASS_Knight, "char\\tmABCD\\cma014.ini" },
	{ ITEMID_WarriorsHanbokM, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd014.ini" },
	{ ITEMID_WarriorsHanbokF, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc014.ini" },
	{ ITEMID_WarriorsHanbokF, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce014.ini" },
	{ ITEMID_WarriorsHanbokM, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme014.ini" },

	//Kinomo Costume
	{ ITEMID_KimonoCostumeM, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb015.ini" },
	{ ITEMID_KimonoCostumeM, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca015.ini" },
	{ ITEMID_KimonoCostumeF, CHARACTERCLASS_Archer, "char\\tmABCD\\cd015.ini" },
	{ ITEMID_KimonoCostumeM, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc015.ini" },
	{ ITEMID_KimonoCostumeF, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb015.ini" },
	{ ITEMID_KimonoCostumeM, CHARACTERCLASS_Knight, "char\\tmABCD\\cma015.ini" },
	{ ITEMID_KimonoCostumeM, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd015.ini" },
	{ ITEMID_KimonoCostumeF, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc015.ini" },
	{ ITEMID_KimonoCostumeF, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce015.ini" },
	{ ITEMID_KimonoCostumeM, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme015.ini" },

	{ ITEMID_GrumpyArmor, CHARACTERCLASS_Fighter, "char\\tmABCD\\b029.ini" },
	{ ITEMID_GrumpyArmor, CHARACTERCLASS_Mechanician, "char\\tmABCD\\a029.ini" },
	{ ITEMID_GrumpyArmor, CHARACTERCLASS_Archer, "char\\tmABCD\\d029.ini" },
	{ ITEMID_GrumpyArmor, CHARACTERCLASS_Pikeman, "char\\tmABCD\\c029.ini" },
	{ ITEMID_GrumpyArmor, CHARACTERCLASS_Atalanta, "char\\tmABCD\\mb029.ini" },
	{ ITEMID_GrumpyArmor, CHARACTERCLASS_Knight, "char\\tmABCD\\ma029.ini" },
	{ ITEMID_GrumpyRobe, CHARACTERCLASS_Magician, "char\\tmABCD\\md029.ini" },
	{ ITEMID_GrumpyRobe, CHARACTERCLASS_Priestess, "char\\tmABCD\\mc029.ini" },
	{ ITEMID_GrumpyArmor, CHARACTERCLASS_Assassin, "char\\tmABCD\\e029.ini" },
	{ ITEMID_GrumpyRobe, CHARACTERCLASS_Shaman, "char\\tmABCD\\me029.ini" },

	//Soccers
	//Peru
	{ ITEMID_PeruvianSoccer, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb018.ini" },
	{ ITEMID_PeruvianSoccer, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca018.ini" },
	{ ITEMID_PeruvianSoccer, CHARACTERCLASS_Archer, "char\\tmABCD\\cd018.ini" },
	{ ITEMID_PeruvianSoccer, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc018.ini" },
	{ ITEMID_PeruvianSoccer, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb018.ini" },
	{ ITEMID_PeruvianSoccer, CHARACTERCLASS_Knight, "char\\tmABCD\\cma018.ini" },
	{ ITEMID_PeruvianSoccer, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd018.ini" },
	{ ITEMID_PeruvianSoccer, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc018.ini" },
	{ ITEMID_PeruvianSoccer, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce018.ini" },
	{ ITEMID_PeruvianSoccer, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme018.ini" },

	//Brazil
	{ ITEMID_BrazilianSoccer, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb019.ini" },
	{ ITEMID_BrazilianSoccer, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca019.ini" },
	{ ITEMID_BrazilianSoccer, CHARACTERCLASS_Archer, "char\\tmABCD\\cd019.ini" },
	{ ITEMID_BrazilianSoccer, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc019.ini" },
	{ ITEMID_BrazilianSoccer, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb019.ini" },
	{ ITEMID_BrazilianSoccer, CHARACTERCLASS_Knight, "char\\tmABCD\\cma019.ini" },
	{ ITEMID_BrazilianSoccer, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd019.ini" },
	{ ITEMID_BrazilianSoccer, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc019.ini" },
	{ ITEMID_BrazilianSoccer, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce019.ini" },
	{ ITEMID_BrazilianSoccer, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme019.ini" },

	//Argentine
	{ ITEMID_ArgentineSoccer, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb020.ini" },
	{ ITEMID_ArgentineSoccer, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca020.ini" },
	{ ITEMID_ArgentineSoccer, CHARACTERCLASS_Archer, "char\\tmABCD\\cd020.ini" },
	{ ITEMID_ArgentineSoccer, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc020.ini" },
	{ ITEMID_ArgentineSoccer, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb020.ini" },
	{ ITEMID_ArgentineSoccer, CHARACTERCLASS_Knight, "char\\tmABCD\\cma020.ini" },
	{ ITEMID_ArgentineSoccer, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd020.ini" },
	{ ITEMID_ArgentineSoccer, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc020.ini" },
	{ ITEMID_ArgentineSoccer, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce020.ini" },
	{ ITEMID_ArgentineSoccer, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme020.ini" },

	//Chilean
	{ ITEMID_ChileanSoccer, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb021.ini" },
	{ ITEMID_ChileanSoccer, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca021.ini" },
	{ ITEMID_ChileanSoccer, CHARACTERCLASS_Archer, "char\\tmABCD\\cd021.ini" },
	{ ITEMID_ChileanSoccer, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc021.ini" },
	{ ITEMID_ChileanSoccer, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb021.ini" },
	{ ITEMID_ChileanSoccer, CHARACTERCLASS_Knight, "char\\tmABCD\\cma021.ini" },
	{ ITEMID_ChileanSoccer, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd021.ini" },
	{ ITEMID_ChileanSoccer, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc021.ini" },
	{ ITEMID_ChileanSoccer, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce021.ini" },
	{ ITEMID_ChileanSoccer, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme021.ini" },

	//Japan
	{ ITEMID_JapanSoccer, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb022.ini" },
	{ ITEMID_JapanSoccer, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca022.ini" },
	{ ITEMID_JapanSoccer, CHARACTERCLASS_Archer, "char\\tmABCD\\cd022.ini" },
	{ ITEMID_JapanSoccer, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc022.ini" },
	{ ITEMID_JapanSoccer, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb022.ini" },
	{ ITEMID_JapanSoccer, CHARACTERCLASS_Knight, "char\\tmABCD\\cma022.ini" },
	{ ITEMID_JapanSoccer, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd022.ini" },
	{ ITEMID_JapanSoccer, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc022.ini" },
	{ ITEMID_JapanSoccer, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce022.ini" },
	{ ITEMID_JapanSoccer, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme022.ini" },

	//Colombian
	{ ITEMID_ColombianSoccer, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb089.ini" },
	{ ITEMID_ColombianSoccer, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca089.ini" },
	{ ITEMID_ColombianSoccer, CHARACTERCLASS_Archer, "char\\tmABCD\\cd089.ini" },
	{ ITEMID_ColombianSoccer, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc089.ini" },
	{ ITEMID_ColombianSoccer, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb089.ini" },
	{ ITEMID_ColombianSoccer, CHARACTERCLASS_Knight, "char\\tmABCD\\cma089.ini" },
	{ ITEMID_ColombianSoccer, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd089.ini" },
	{ ITEMID_ColombianSoccer, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc089.ini" },
	{ ITEMID_ColombianSoccer, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce089.ini" },
	{ ITEMID_ColombianSoccer, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme089.ini" },

	//French
	{ ITEMID_FrenchSoccer, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb090.ini" },
	{ ITEMID_FrenchSoccer, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca090.ini" },
	{ ITEMID_FrenchSoccer, CHARACTERCLASS_Archer, "char\\tmABCD\\cd090.ini" },
	{ ITEMID_FrenchSoccer, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc090.ini" },
	{ ITEMID_FrenchSoccer, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb090.ini" },
	{ ITEMID_FrenchSoccer, CHARACTERCLASS_Knight, "char\\tmABCD\\cma090.ini" },
	{ ITEMID_FrenchSoccer, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd090.ini" },
	{ ITEMID_FrenchSoccer, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc090.ini" },
	{ ITEMID_FrenchSoccer, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce090.ini" },
	{ ITEMID_FrenchSoccer, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme090.ini" },

	//Germany
	{ ITEMID_GermanySoccer, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb091.ini" },
	{ ITEMID_GermanySoccer, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca091.ini" },
	{ ITEMID_GermanySoccer, CHARACTERCLASS_Archer, "char\\tmABCD\\cd091.ini" },
	{ ITEMID_GermanySoccer, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc091.ini" },
	{ ITEMID_GermanySoccer, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb091.ini" },
	{ ITEMID_GermanySoccer, CHARACTERCLASS_Knight, "char\\tmABCD\\cma091.ini" },
	{ ITEMID_GermanySoccer, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd091.ini" },
	{ ITEMID_GermanySoccer, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc091.ini" },
	{ ITEMID_GermanySoccer, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce091.ini" },
	{ ITEMID_GermanySoccer, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme091.ini" },

	//Holland
	{ ITEMID_HollandSoccer, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb092.ini" },
	{ ITEMID_HollandSoccer, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca092.ini" },
	{ ITEMID_HollandSoccer, CHARACTERCLASS_Archer, "char\\tmABCD\\cd092.ini" },
	{ ITEMID_HollandSoccer, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc092.ini" },
	{ ITEMID_HollandSoccer, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb092.ini" },
	{ ITEMID_HollandSoccer, CHARACTERCLASS_Knight, "char\\tmABCD\\cma092.ini" },
	{ ITEMID_HollandSoccer, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd092.ini" },
	{ ITEMID_HollandSoccer, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc092.ini" },
	{ ITEMID_HollandSoccer, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce092.ini" },
	{ ITEMID_HollandSoccer, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme092.ini" },

	//Italian
	{ ITEMID_ItalianSoccer, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb093.ini" },
	{ ITEMID_ItalianSoccer, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca093.ini" },
	{ ITEMID_ItalianSoccer, CHARACTERCLASS_Archer, "char\\tmABCD\\cd093.ini" },
	{ ITEMID_ItalianSoccer, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc093.ini" },
	{ ITEMID_ItalianSoccer, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb093.ini" },
	{ ITEMID_ItalianSoccer, CHARACTERCLASS_Knight, "char\\tmABCD\\cma093.ini" },
	{ ITEMID_ItalianSoccer, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd093.ini" },
	{ ITEMID_ItalianSoccer, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc093.ini" },
	{ ITEMID_ItalianSoccer, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce093.ini" },
	{ ITEMID_ItalianSoccer, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme093.ini" },

	//Spain
	{ ITEMID_SpanishSoccer, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb094.ini" },
	{ ITEMID_SpanishSoccer, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca094.ini" },
	{ ITEMID_SpanishSoccer, CHARACTERCLASS_Archer, "char\\tmABCD\\cd094.ini" },
	{ ITEMID_SpanishSoccer, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc094.ini" },
	{ ITEMID_SpanishSoccer, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb094.ini" },
	{ ITEMID_SpanishSoccer, CHARACTERCLASS_Knight, "char\\tmABCD\\cma094.ini" },
	{ ITEMID_SpanishSoccer, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd094.ini" },
	{ ITEMID_SpanishSoccer, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc094.ini" },
	{ ITEMID_SpanishSoccer, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce094.ini" },
	{ ITEMID_SpanishSoccer, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme094.ini" },

	//Portugal
	{ ITEMID_PortugueseSoccer, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb095.ini" },
	{ ITEMID_PortugueseSoccer, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca095.ini" },
	{ ITEMID_PortugueseSoccer, CHARACTERCLASS_Archer, "char\\tmABCD\\cd095.ini" },
	{ ITEMID_PortugueseSoccer, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc095.ini" },
	{ ITEMID_PortugueseSoccer, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb095.ini" },
	{ ITEMID_PortugueseSoccer, CHARACTERCLASS_Knight, "char\\tmABCD\\cma095.ini" },
	{ ITEMID_PortugueseSoccer, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd095.ini" },
	{ ITEMID_PortugueseSoccer, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc095.ini" },
	{ ITEMID_PortugueseSoccer, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce095.ini" },
	{ ITEMID_PortugueseSoccer, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme095.ini" },

	//Mexican
	{ ITEMID_MexicanSoccer, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb096.ini" },
	{ ITEMID_MexicanSoccer, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca096.ini" },
	{ ITEMID_MexicanSoccer, CHARACTERCLASS_Archer, "char\\tmABCD\\cd096.ini" },
	{ ITEMID_MexicanSoccer, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc096.ini" },
	{ ITEMID_MexicanSoccer, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb096.ini" },
	{ ITEMID_MexicanSoccer, CHARACTERCLASS_Knight, "char\\tmABCD\\cma096.ini" },
	{ ITEMID_MexicanSoccer, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd096.ini" },
	{ ITEMID_MexicanSoccer, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc096.ini" },
	{ ITEMID_MexicanSoccer, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce096.ini" },
	{ ITEMID_MexicanSoccer, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme096.ini" },

	//USA
	{ ITEMID_TeamUSASoccer, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb097.ini" },
	{ ITEMID_TeamUSASoccer, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca097.ini" },
	{ ITEMID_TeamUSASoccer, CHARACTERCLASS_Archer, "char\\tmABCD\\cd097.ini" },
	{ ITEMID_TeamUSASoccer, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc097.ini" },
	{ ITEMID_TeamUSASoccer, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb097.ini" },
	{ ITEMID_TeamUSASoccer, CHARACTERCLASS_Knight, "char\\tmABCD\\cma097.ini" },
	{ ITEMID_TeamUSASoccer, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd097.ini" },
	{ ITEMID_TeamUSASoccer, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc097.ini" },
	{ ITEMID_TeamUSASoccer, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce097.ini" },
	{ ITEMID_TeamUSASoccer, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme097.ini" },

	//Vietnamese
	{ ITEMID_VietnameseSoccer, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb098.ini" },
	{ ITEMID_VietnameseSoccer, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca098.ini" },
	{ ITEMID_VietnameseSoccer, CHARACTERCLASS_Archer, "char\\tmABCD\\cd098.ini" },
	{ ITEMID_VietnameseSoccer, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc098.ini" },
	{ ITEMID_VietnameseSoccer, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb098.ini" },
	{ ITEMID_VietnameseSoccer, CHARACTERCLASS_Knight, "char\\tmABCD\\cma098.ini" },
	{ ITEMID_VietnameseSoccer, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd098.ini" },
	{ ITEMID_VietnameseSoccer, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc098.ini" },
	{ ITEMID_VietnameseSoccer, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce098.ini" },
	{ ITEMID_VietnameseSoccer, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme098.ini" },

	//Philippines
	{ ITEMID_PinoySoccer, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb099.ini" },
	{ ITEMID_PinoySoccer, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca099.ini" },
	{ ITEMID_PinoySoccer, CHARACTERCLASS_Archer, "char\\tmABCD\\cd099.ini" },
	{ ITEMID_PinoySoccer, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc099.ini" },
	{ ITEMID_PinoySoccer, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb099.ini" },
	{ ITEMID_PinoySoccer, CHARACTERCLASS_Knight, "char\\tmABCD\\cma099.ini" },
	{ ITEMID_PinoySoccer, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd099.ini" },
	{ ITEMID_PinoySoccer, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc099.ini" },
	{ ITEMID_PinoySoccer, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce099.ini" },
	{ ITEMID_PinoySoccer, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme099.ini" },
	
	//Kronus
	{ ITEMID_KronusArmor, CHARACTERCLASS_Fighter, "char\\tmABCD\\b026.ini" },
	{ ITEMID_KronusArmor, CHARACTERCLASS_Mechanician, "char\\tmABCD\\a026.ini" },
	{ ITEMID_KronusArmor, CHARACTERCLASS_Archer, "char\\tmABCD\\d026.ini" },
	{ ITEMID_KronusArmor, CHARACTERCLASS_Pikeman, "char\\tmABCD\\c026.ini" },
	{ ITEMID_KronusArmor, CHARACTERCLASS_Atalanta, "char\\tmABCD\\mb026.ini" },
	{ ITEMID_KronusArmor, CHARACTERCLASS_Knight, "char\\tmABCD\\ma026.ini" },
	{ ITEMID_KronusRobe, CHARACTERCLASS_Magician, "char\\tmABCD\\md026.ini" },
	{ ITEMID_KronusRobe, CHARACTERCLASS_Priestess, "char\\tmABCD\\mc026.ini" },
	{ ITEMID_KronusArmor, CHARACTERCLASS_Assassin, "char\\tmABCD\\e026.ini" },
	{ ITEMID_KronusRobe, CHARACTERCLASS_Shaman, "char\\tmABCD\\me026.ini" },

	//Costumes
	{ ITEMID_PartyCostumeMale, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb001.ini" },
	{ ITEMID_PartyCostumeMale, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca001.ini" },
	{ ITEMID_PartyCostumeFemale, CHARACTERCLASS_Archer, "char\\tmABCD\\cd001.ini" },
	{ ITEMID_PartyCostumeMale, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc001.ini" },
	{ ITEMID_PartyCostumeFemale, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb001.ini" },
	{ ITEMID_PartyCostumeMale, CHARACTERCLASS_Knight, "char\\tmABCD\\cma001.ini" },
	{ ITEMID_PartyCostumeMale, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd001.ini" },
	{ ITEMID_PartyCostumeFemale, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc001.ini" },
	{ ITEMID_PartyCostumeFemale, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce001.ini" },
	{ ITEMID_PartyCostumeMale, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme001.ini" },

	{ ITEMID_Costume19MaleArmorFemale, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb006.ini" },
	{ ITEMID_Costume19MaleArmorFemale, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca006.ini" },
	{ ITEMID_Costume19MaleArmorFemale, CHARACTERCLASS_Archer, "char\\tmABCD\\cd006.ini" },
	{ ITEMID_Costume19MaleArmorFemale, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc006.ini" },
	{ ITEMID_Costume19MaleArmorFemale, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb006.ini" },
	{ ITEMID_Costume19MaleArmorFemale, CHARACTERCLASS_Knight, "char\\tmABCD\\cma006.ini" },
	{ ITEMID_Costume19MaleArmorFemale, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd006.ini" },
	{ ITEMID_Costume19MaleArmorFemale, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc006.ini" },
	{ ITEMID_Costume19MaleArmorFemale, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce006.ini" },
	{ ITEMID_Costume19MaleArmorFemale, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme006.ini" },

	{ ITEMID_IdasCostumeMale, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb002.ini" },
	{ ITEMID_IdasCostumeMale, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca002.ini" },
	{ ITEMID_IdasCostumeFemale, CHARACTERCLASS_Archer, "char\\tmABCD\\cd002.ini" },
	{ ITEMID_IdasCostumeMale, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc002.ini" },
	{ ITEMID_IdasCostumeFemale, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb002.ini" },
	{ ITEMID_IdasCostumeMale, CHARACTERCLASS_Knight, "char\\tmABCD\\cma002.ini" },
	{ ITEMID_MartanosRobeMale, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd002.ini" },
	{ ITEMID_MartanosRobeFemale, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc002.ini" },
	{ ITEMID_IdasCostumeFemale, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce002.ini" },
	{ ITEMID_MartanosRobeMale, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme002.ini" },

	{ ITEMID_ThalesArmorMale, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb004.ini" },
	{ ITEMID_ThalesArmorMale, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca004.ini" },
	{ ITEMID_ThalesArmorFemale, CHARACTERCLASS_Archer, "char\\tmABCD\\cd004.ini" },
	{ ITEMID_ThalesArmorMale, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc004.ini" },
	{ ITEMID_ThalesArmorFemale, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb004.ini" },
	{ ITEMID_ThalesArmorMale, CHARACTERCLASS_Knight, "char\\tmABCD\\cma004.ini" },
	{ ITEMID_MikalRobeMale, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd004.ini" },
	{ ITEMID_MikalRobeFemale, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc004.ini" },
	{ ITEMID_ThalesArmorFemale, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce004.ini" },
	{ ITEMID_MikalRobeMale, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme004.ini" },

	{ ITEMID_Costume4ArmorMale, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb012.ini" },
	{ ITEMID_Costume4ArmorMale, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca012.ini" },
	{ ITEMID_Costume4ArmorFemale, CHARACTERCLASS_Archer, "char\\tmABCD\\cd012.ini" },
	{ ITEMID_Costume4ArmorMale, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc012.ini" },
	{ ITEMID_Costume4ArmorFemale, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb012.ini" },
	{ ITEMID_Costume4ArmorMale, CHARACTERCLASS_Knight, "char\\tmABCD\\cma012.ini" },
	{ ITEMID_Costume4ArmorMale, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd012.ini" },
	{ ITEMID_Costume4ArmorFemale, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc012.ini" },
	{ ITEMID_Costume4ArmorFemale, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce012.ini" },
	{ ITEMID_Costume4ArmorMale, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme012.ini" },

	{ ITEMID_Costume5ArmorMale, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb027.ini" },
	{ ITEMID_Costume5ArmorMale, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca027.ini" },
	{ ITEMID_Costume5ArmorFemale, CHARACTERCLASS_Archer, "char\\tmABCD\\cd027.ini" },
	{ ITEMID_Costume5ArmorMale, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc027.ini" },
	{ ITEMID_Costume5ArmorFemale, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb027.ini" },
	{ ITEMID_Costume5ArmorMale, CHARACTERCLASS_Knight, "char\\tmABCD\\cma027.ini" },
	{ ITEMID_Costume5ArmorMale, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd027.ini" },
	{ ITEMID_Costume5ArmorFemale, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc027.ini" },
	{ ITEMID_Costume5ArmorFemale, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce027.ini" },
	{ ITEMID_Costume5ArmorMale, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme027.ini" },

	{ ITEMID_WingArmorMale, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb024.ini" },
	{ ITEMID_WingArmorMale, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca024.ini" },
	{ ITEMID_WingArmorFemale, CHARACTERCLASS_Archer, "char\\tmABCD\\cd024.ini" },
	{ ITEMID_WingArmorMale, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc024.ini" },
	{ ITEMID_WingArmorFemale, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb024.ini" },
	{ ITEMID_WingArmorMale, CHARACTERCLASS_Knight, "char\\tmABCD\\cma024.ini" },
	{ ITEMID_WingArmorMale, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd024.ini" },
	{ ITEMID_WingArmorFemale, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc024.ini" },
	{ ITEMID_WingArmorFemale, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce024.ini" },
	{ ITEMID_WingArmorMale, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme024.ini" },

	{ ITEMID_DarkWingArmorMale, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb025.ini" },
	{ ITEMID_DarkWingArmorMale, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca025.ini" },
	{ ITEMID_DarkWingArmorFemale, CHARACTERCLASS_Archer, "char\\tmABCD\\cd025.ini" },
	{ ITEMID_DarkWingArmorMale, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc025.ini" },
	{ ITEMID_DarkWingArmorFemale, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb025.ini" },
	{ ITEMID_DarkWingArmorMale, CHARACTERCLASS_Knight, "char\\tmABCD\\cma025.ini" },
	{ ITEMID_DarkWingArmorMale, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd025.ini" },
	{ ITEMID_DarkWingArmorFemale, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc025.ini" },
	{ ITEMID_DarkWingArmorFemale, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce025.ini" },
	{ ITEMID_DarkWingArmorMale, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme025.ini" },

	{ ITEMID_SummerClotheMale, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb026.ini" },
	{ ITEMID_SummerClotheMale, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca026.ini" },
	{ ITEMID_SummerClotheFemale, CHARACTERCLASS_Archer, "char\\tmABCD\\cd026.ini" },
	{ ITEMID_SummerClotheMale, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc026.ini" },
	{ ITEMID_SummerClotheFemale, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb026.ini" },
	{ ITEMID_SummerClotheMale, CHARACTERCLASS_Knight, "char\\tmABCD\\cma026.ini" },
	{ ITEMID_SummerClotheMale, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd026.ini" },
	{ ITEMID_SummerClotheFemale, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc026.ini" },
	{ ITEMID_SummerClotheFemale, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce026.ini" },
	{ ITEMID_SummerClotheMale, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme026.ini" },

	{ ITEMID_SpringCostumeMale, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb028.ini" },
	{ ITEMID_SpringCostumeMale, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca028.ini" },
	{ ITEMID_SpringCostumeFemale, CHARACTERCLASS_Archer, "char\\tmABCD\\cd028.ini" },
	{ ITEMID_SpringCostumeMale, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc028.ini" },
	{ ITEMID_SpringCostumeFemale, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb028.ini" },
	{ ITEMID_SpringCostumeMale, CHARACTERCLASS_Knight, "char\\tmABCD\\cma028.ini" },
	{ ITEMID_SpringCostumeMale, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd028.ini" },
	{ ITEMID_SpringCostumeFemale, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc028.ini" },
	{ ITEMID_SpringCostumeFemale, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce028.ini" },
	{ ITEMID_SpringCostumeMale, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme028.ini" },

	{ ITEMID_YellowSpringCostumeMale, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb029.ini" },
	{ ITEMID_YellowSpringCostumeMale, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca029.ini" },
	{ ITEMID_YellowSpringCostumeFemale, CHARACTERCLASS_Archer, "char\\tmABCD\\cd029.ini" },
	{ ITEMID_YellowSpringCostumeMale, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc029.ini" },
	{ ITEMID_YellowSpringCostumeFemale, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb029.ini" },
	{ ITEMID_YellowSpringCostumeMale, CHARACTERCLASS_Knight, "char\\tmABCD\\cma029.ini" },
	{ ITEMID_YellowSpringCostumeMale, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd029.ini" },
	{ ITEMID_YellowSpringCostumeFemale, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc029.ini" },
	{ ITEMID_YellowSpringCostumeFemale, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce029.ini" },
	{ ITEMID_YellowSpringCostumeMale, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme029.ini" },

	{ ITEMID_Costume11ArmorMale, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb030.ini" },
	{ ITEMID_Costume11ArmorMale, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca030.ini" },
	{ ITEMID_Costume11ArmorFemale, CHARACTERCLASS_Archer, "char\\tmABCD\\cd030.ini" },
	{ ITEMID_Costume11ArmorMale, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc030.ini" },
	{ ITEMID_Costume11ArmorFemale, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb030.ini" },
	{ ITEMID_Costume11ArmorMale, CHARACTERCLASS_Knight, "char\\tmABCD\\cma030.ini" },
	{ ITEMID_Costume11ArmorMale, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd030.ini" },
	{ ITEMID_Costume11ArmorFemale, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc030.ini" },
	{ ITEMID_Costume11ArmorFemale, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce030.ini" },
	{ ITEMID_Costume11ArmorMale, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme030.ini" },

	{ ITEMID_BlackSuit, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb035.ini" },
	{ ITEMID_BlackSuit, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca035.ini" },
	{ ITEMID_WhiteDress, CHARACTERCLASS_Archer, "char\\tmABCD\\cd035.ini" },
	{ ITEMID_BlackSuit, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc035.ini" },
	{ ITEMID_WhiteDress, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb035.ini" },
	{ ITEMID_BlackSuit, CHARACTERCLASS_Knight, "char\\tmABCD\\cma035.ini" },
	{ ITEMID_BlackSuit, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd035.ini" },
	{ ITEMID_WhiteDress, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc035.ini" },
	{ ITEMID_WhiteDress, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce035.ini" },
	{ ITEMID_BlackSuit, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme035.ini" },

	{ ITEMID_MechanicCostumeM, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb034.ini" },
	{ ITEMID_MechanicCostumeM, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca034.ini" },
	{ ITEMID_MechanicCostumeF, CHARACTERCLASS_Archer, "char\\tmABCD\\cd034.ini" },
	{ ITEMID_MechanicCostumeM, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc034.ini" },
	{ ITEMID_MechanicCostumeF, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb034.ini" },
	{ ITEMID_MechanicCostumeM, CHARACTERCLASS_Knight, "char\\tmABCD\\cma034.ini" },
	{ ITEMID_MechanicCostumeM, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd034.ini" },
	{ ITEMID_MechanicCostumeF, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc034.ini" },
	{ ITEMID_MechanicCostumeF, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce034.ini" },
	{ ITEMID_MechanicCostumeM, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme034.ini" },

	{ ITEMID_BroyaleCostumeMale, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb303.ini" },
	{ ITEMID_BroyaleCostumeMale, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca303.ini" },
	{ ITEMID_BroyaleCostumeFemale, CHARACTERCLASS_Archer, "char\\tmABCD\\cd303.ini" },
	{ ITEMID_BroyaleCostumeMale, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc303.ini" },
	{ ITEMID_BroyaleCostumeFemale, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb303.ini" },
	{ ITEMID_BroyaleCostumeMale, CHARACTERCLASS_Knight, "char\\tmABCD\\cma303.ini" },
	{ ITEMID_BroyaleCostumeMale, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd303.ini" },
	{ ITEMID_BroyaleCostumeFemale, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc303.ini" },
	{ ITEMID_BroyaleCostumeFemale, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce303.ini" },
	{ ITEMID_BroyaleCostumeMale, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme303.ini" },

	{ ITEMID_CasualSchoolUniformMale, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb036.ini" },
	{ ITEMID_CasualSchoolUniformMale, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca036.ini" },
	{ ITEMID_CasualSchoolUniformFemale, CHARACTERCLASS_Archer, "char\\tmABCD\\cd036.ini" },
	{ ITEMID_CasualSchoolUniformMale, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc036.ini" },
	{ ITEMID_CasualSchoolUniformFemale, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb036.ini" },
	{ ITEMID_CasualSchoolUniformMale, CHARACTERCLASS_Knight, "char\\tmABCD\\cma036.ini" },
	{ ITEMID_CasualSchoolUniformMale, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd036.ini" },
	{ ITEMID_CasualSchoolUniformFemale, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc036.ini" },
	{ ITEMID_CasualSchoolUniformFemale, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce036.ini" },
	{ ITEMID_CasualSchoolUniformMale, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme036.ini" },

	{ ITEMID_ElegantSchoolUniformMale, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb037.ini" },
	{ ITEMID_ElegantSchoolUniformMale, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca037.ini" },
	{ ITEMID_ElegantSchoolUniformFemale, CHARACTERCLASS_Archer, "char\\tmABCD\\cd037.ini" },
	{ ITEMID_ElegantSchoolUniformMale, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc037.ini" },
	{ ITEMID_ElegantSchoolUniformFemale, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb037.ini" },
	{ ITEMID_ElegantSchoolUniformMale, CHARACTERCLASS_Knight, "char\\tmABCD\\cma037.ini" },
	{ ITEMID_ElegantSchoolUniformMale, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd037.ini" },
	{ ITEMID_ElegantSchoolUniformFemale, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc037.ini" },
	{ ITEMID_ElegantSchoolUniformFemale, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce037.ini" },
	{ ITEMID_ElegantSchoolUniformMale, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme037.ini" },

	{ ITEMID_Costume16ArmorMale, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb038.ini" },
	{ ITEMID_Costume16ArmorMale, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca038.ini" },
	{ ITEMID_Costume16ArmorFemale, CHARACTERCLASS_Archer, "char\\tmABCD\\cd038.ini" },
	{ ITEMID_Costume16ArmorMale, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc038.ini" },
	{ ITEMID_Costume16ArmorFemale, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb038.ini" },
	{ ITEMID_Costume16ArmorMale, CHARACTERCLASS_Knight, "char\\tmABCD\\cma038.ini" },
	{ ITEMID_Costume16ArmorMale, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd038.ini" },
	{ ITEMID_Costume16ArmorFemale, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc038.ini" },
	{ ITEMID_Costume16ArmorFemale, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce038.ini" },
	{ ITEMID_Costume16ArmorMale, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme038.ini" },

	{ ITEMID_MarineCostumeMale, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb039.ini" },
	{ ITEMID_MarineCostumeMale, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca039.ini" },
	{ ITEMID_MarineCostumeFemale, CHARACTERCLASS_Archer, "char\\tmABCD\\cd039.ini" },
	{ ITEMID_MarineCostumeMale, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc039.ini" },
	{ ITEMID_MarineCostumeFemale, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb039.ini" },
	{ ITEMID_MarineCostumeMale, CHARACTERCLASS_Knight, "char\\tmABCD\\cma039.ini" },
	{ ITEMID_MarineCostumeMale, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd039.ini" },
	{ ITEMID_MarineCostumeFemale, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc039.ini" },
	{ ITEMID_MarineCostumeFemale, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce039.ini" },
	{ ITEMID_MarineCostumeMale, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme039.ini" },

	{ ITEMID_GaiaArmorMale, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb007.ini" },
	{ ITEMID_GaiaArmorMale, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca007.ini" },
	{ ITEMID_GaiaArmorFemale, CHARACTERCLASS_Archer, "char\\tmABCD\\cd008.ini" },
	{ ITEMID_GaiaArmorMale, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc007.ini" },
	{ ITEMID_GaiaArmorFemale, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb008.ini" },
	{ ITEMID_GaiaArmorMale, CHARACTERCLASS_Knight, "char\\tmABCD\\cma007.ini" },
	{ ITEMID_IriaRobeMale, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd008.ini" },
	{ ITEMID_IriaRobeFemale, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc008.ini" },
	{ ITEMID_GaiaArmorFemale, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce008.ini" },
	{ ITEMID_IriaRobeMale, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme008.ini" },

	{ ITEMID_DarkGaiaArmorMale, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb009.ini" },
	{ ITEMID_DarkGaiaArmorMale, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca009.ini" },
	{ ITEMID_DarkGaiaArmorFemale, CHARACTERCLASS_Archer, "char\\tmABCD\\cd010.ini" },
	{ ITEMID_DarkGaiaArmorMale, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc009.ini" },
	{ ITEMID_DarkGaiaArmorFemale, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb010.ini" },
	{ ITEMID_DarkGaiaArmorMale, CHARACTERCLASS_Knight, "char\\tmABCD\\cma009.ini" },
	{ ITEMID_DarkIriaRobeMale, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd009.ini" },
	{ ITEMID_DarkIriaRobeFemale, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc010.ini" },
	{ ITEMID_DarkGaiaArmorFemale, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce010.ini" },
	{ ITEMID_DarkIriaRobeMale, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme010.ini" },

	{ ITEMID_WeddingSuit, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb013.ini" },
	{ ITEMID_WeddingSuit, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca013.ini" },
	{ ITEMID_WeddingDress, CHARACTERCLASS_Archer, "char\\tmABCD\\cd013.ini" },
	{ ITEMID_WeddingSuit, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc013.ini" },
	{ ITEMID_WeddingDress, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb013.ini" },
	{ ITEMID_WeddingSuit, CHARACTERCLASS_Knight, "char\\tmABCD\\cma013.ini" },
	{ ITEMID_WeddingSuit, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd013.ini" },
	{ ITEMID_WeddingDress, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc013.ini" },
	{ ITEMID_WeddingDress, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce013.ini" },
	{ ITEMID_WeddingSuit, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme013.ini" },

	{ ITEMID_Costume23ArmorMale, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb014.ini" },
	{ ITEMID_Costume23ArmorMale, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca014.ini" },
	{ ITEMID_Costume23ArmorFemale, CHARACTERCLASS_Archer, "char\\tmABCD\\cd014.ini" },
	{ ITEMID_Costume23ArmorMale, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc014.ini" },
	{ ITEMID_Costume23ArmorFemale, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb014.ini" },
	{ ITEMID_Costume23ArmorMale, CHARACTERCLASS_Knight, "char\\tmABCD\\cma014.ini" },
	{ ITEMID_Costume23ArmorMale, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd014.ini" },
	{ ITEMID_Costume23ArmorFemale, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc014.ini" },
	{ ITEMID_Costume23ArmorFemale, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce014.ini" },
	{ ITEMID_Costume23ArmorMale, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme014.ini" },
					  
	{ ITEMID_Costume24ArmorMale, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb015.ini" },
	{ ITEMID_Costume24ArmorMale, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca015.ini" },
	{ ITEMID_Costume24ArmorFemale, CHARACTERCLASS_Archer, "char\\tmABCD\\cd015.ini" },
	{ ITEMID_Costume24ArmorMale, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc015.ini" },
	{ ITEMID_Costume24ArmorFemale, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb015.ini" },
	{ ITEMID_Costume24ArmorMale, CHARACTERCLASS_Knight, "char\\tmABCD\\cma015.ini" },
	{ ITEMID_Costume24ArmorMale, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd015.ini" },
	{ ITEMID_Costume24ArmorFemale, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc015.ini" },
	{ ITEMID_Costume24ArmorFemale, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce015.ini" },
	{ ITEMID_Costume24ArmorMale, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme015.ini" },
				
	{ ITEMID_Costume25ArmorMale, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb017.ini" },
	{ ITEMID_Costume25ArmorMale, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca017.ini" },
	{ ITEMID_Costume25ArmorFemale, CHARACTERCLASS_Archer, "char\\tmABCD\\cd017.ini" },
	{ ITEMID_Costume25ArmorMale, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc017.ini" },
	{ ITEMID_Costume25ArmorFemale, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb017.ini" },
	{ ITEMID_Costume25ArmorMale, CHARACTERCLASS_Knight, "char\\tmABCD\\cma017.ini" },
	{ ITEMID_Costume25ArmorMale, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd017.ini" },
	{ ITEMID_Costume25ArmorFemale, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc017.ini" },
	{ ITEMID_Costume25ArmorFemale, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce017.ini" },
	{ ITEMID_Costume25ArmorMale, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme017.ini" },

	//Hopy Costume
	{ ITEMID_HopyCostume, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb040.ini" },
	{ ITEMID_HopyCostume, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca040.ini" },
	{ ITEMID_CuepyCostume, CHARACTERCLASS_Archer, "char\\tmABCD\\cd040.ini" },
	{ ITEMID_HopyCostume, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc040.ini" },
	{ ITEMID_CuepyCostume, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb040.ini" },
	{ ITEMID_HopyCostume, CHARACTERCLASS_Knight, "char\\tmABCD\\cma040.ini" },
	{ ITEMID_HopyCostume, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd040.ini" },
	{ ITEMID_CuepyCostume, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc040.ini" },
	{ ITEMID_CuepyCostume, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce040.ini" },
	{ ITEMID_HopyCostume, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme040.ini" },
	{ ITEMID_HopyCostumeTime, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb040.ini" },
	{ ITEMID_HopyCostumeTime, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca040.ini" },
	{ ITEMID_CuepyCostumeTime, CHARACTERCLASS_Archer, "char\\tmABCD\\cd040.ini" },
	{ ITEMID_HopyCostumeTime, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc040.ini" },
	{ ITEMID_CuepyCostumeTime, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb040.ini" },
	{ ITEMID_HopyCostumeTime, CHARACTERCLASS_Knight, "char\\tmABCD\\cma040.ini" },
	{ ITEMID_HopyCostumeTime, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd040.ini" },
	{ ITEMID_CuepyCostumeTime, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc040.ini" },
	{ ITEMID_CuepyCostumeTime, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce040.ini" },
	{ ITEMID_HopyCostumeTime, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme040.ini" },

	{ITEMID_HopyCostumeIce, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb041.ini"},
	{ITEMID_HopyCostumeIce, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca041.ini"},
	{ITEMID_CuepyCostumeIce, CHARACTERCLASS_Archer, "char\\tmABCD\\cd041.ini"},
	{ITEMID_HopyCostumeIce, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc041.ini"},
	{ITEMID_CuepyCostumeIce, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb041.ini"},
	{ITEMID_HopyCostumeIce, CHARACTERCLASS_Knight, "char\\tmABCD\\cma041.ini"},
	{ITEMID_HopyCostumeIce, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd041.ini"},
	{ITEMID_CuepyCostumeIce, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc041.ini"},
	{ITEMID_CuepyCostumeIce, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce041.ini"},
	{ITEMID_HopyCostumeIce, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme041.ini"},

	{ ITEMID_ChemistCostumeMale, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb042.ini" },
	{ ITEMID_ChemistCostumeMale, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca042.ini" },
	{ ITEMID_ChemistCostumeFemale, CHARACTERCLASS_Archer, "char\\tmABCD\\cd042.ini" },
	{ ITEMID_ChemistCostumeMale, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc042.ini" },
	{ ITEMID_ChemistCostumeFemale, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb042.ini" },
	{ ITEMID_ChemistCostumeMale, CHARACTERCLASS_Knight, "char\\tmABCD\\cma042.ini" },
	{ ITEMID_ChemistCostumeMale, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd042.ini" },
	{ ITEMID_ChemistCostumeFemale, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc042.ini" },
	{ ITEMID_ChemistCostumeFemale, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce042.ini" },
	{ ITEMID_ChemistCostumeMale, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme042.ini" },

	{ ITEMID_DraculaDragonCostumeMale, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb043.ini" },
	{ ITEMID_DraculaDragonCostumeMale, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca043.ini" },
	{ ITEMID_DraculaDragonCostumeFemale, CHARACTERCLASS_Archer, "char\\tmABCD\\cd043.ini" },
	{ ITEMID_DraculaDragonCostumeMale, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc043.ini" },
	{ ITEMID_DraculaDragonCostumeFemale, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb043.ini" },
	{ ITEMID_DraculaDragonCostumeMale, CHARACTERCLASS_Knight, "char\\tmABCD\\cma043.ini" },
	{ ITEMID_DraculaDragonCostumeMale, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd043.ini" },
	{ ITEMID_DraculaDragonCostumeFemale, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc043.ini" },
	{ ITEMID_DraculaDragonCostumeFemale, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce043.ini" },
	{ ITEMID_DraculaDragonCostumeMale, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme043.ini" },

	//Silver Costume
	{ ITEMID_SilverCostumeMale, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb003.ini" },
	{ ITEMID_SilverCostumeMale, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca003.ini" },
	{ ITEMID_SilverCostumeFemale, CHARACTERCLASS_Archer, "char\\tmABCD\\cd003.ini" },
	{ ITEMID_SilverCostumeMale, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc003.ini" },
	{ ITEMID_SilverCostumeFemale, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb003.ini" },
	{ ITEMID_SilverCostumeMale, CHARACTERCLASS_Knight, "char\\tmABCD\\cma003.ini" },
	{ ITEMID_SilverCostumeMale, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd003.ini" },
	{ ITEMID_SilverCostumeFemale, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc003.ini" },
	{ ITEMID_SilverCostumeFemale, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce003.ini" },
	{ ITEMID_SilverCostumeMale, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme003.ini" },


	{ITEMID_IceCostumeMale, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb044.ini"},
	{ITEMID_IceCostumeMale, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca044.ini"},
	{ITEMID_IceCostumeFemale, CHARACTERCLASS_Archer, "char\\tmABCD\\cd044.ini"},
	{ITEMID_IceCostumeMale, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc044.ini"},
	{ITEMID_IceCostumeFemale, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb044.ini"},
	{ITEMID_IceCostumeMale, CHARACTERCLASS_Knight, "char\\tmABCD\\cma044.ini"},
	{ITEMID_IceCostumeMale, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd044.ini"},
	{ITEMID_IceCostumeFemale, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc044.ini"},
	{ITEMID_IceCostumeFemale, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce044.ini"},
	{ITEMID_IceCostumeMale, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme044.ini"},

	{ITEMID_GoldenHopyCostumeMale, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb045.ini"},
	{ITEMID_GoldenHopyCostumeMale, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca045.ini"},
	{ITEMID_GoldenHopyCostumeFemale, CHARACTERCLASS_Archer, "char\\tmABCD\\cd045.ini"},
	{ITEMID_GoldenHopyCostumeMale, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc045.ini"},
	{ITEMID_GoldenHopyCostumeFemale, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb045.ini"},
	{ITEMID_GoldenHopyCostumeMale, CHARACTERCLASS_Knight, "char\\tmABCD\\cma045.ini"},
	{ITEMID_GoldenHopyCostumeMale, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd045.ini"},
	{ITEMID_GoldenHopyCostumeFemale, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc045.ini"},
	{ITEMID_GoldenHopyCostumeFemale, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce045.ini"},
	{ITEMID_GoldenHopyCostumeMale, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme045.ini"},

	{ITEMID_GoldenHopyCostumeMaleTime, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb045.ini"},
	{ITEMID_GoldenHopyCostumeMaleTime, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca045.ini"},
	{ITEMID_GoldenHopyCostumeFemaleTime, CHARACTERCLASS_Archer, "char\\tmABCD\\cd045.ini"},
	{ITEMID_GoldenHopyCostumeMaleTime, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc045.ini"},
	{ITEMID_GoldenHopyCostumeFemaleTime, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb045.ini"},
	{ITEMID_GoldenHopyCostumeMaleTime, CHARACTERCLASS_Knight, "char\\tmABCD\\cma045.ini"},
	{ITEMID_GoldenHopyCostumeMaleTime, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd045.ini"},
	{ITEMID_GoldenHopyCostumeFemaleTime, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc045.ini"},
	{ITEMID_GoldenHopyCostumeFemaleTime, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce045.ini"},
	{ITEMID_GoldenHopyCostumeMaleTime, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme045.ini"},

	{ITEMID_SandurrRedCostumeMale, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb046.ini"},
	{ITEMID_SandurrRedCostumeMale, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca046.ini"},
	{ITEMID_SandurrRedCostumeFemale, CHARACTERCLASS_Archer, "char\\tmABCD\\cd046.ini"},
	{ITEMID_SandurrRedCostumeMale, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc046.ini"},
	{ITEMID_SandurrRedCostumeFemale, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb046.ini"},
	{ITEMID_SandurrRedCostumeMale, CHARACTERCLASS_Knight, "char\\tmABCD\\cma046.ini"},
	{ITEMID_SandurrRedCostumeMale, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd046.ini"},
	{ITEMID_SandurrRedCostumeFemale, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc046.ini"},
	{ITEMID_SandurrRedCostumeFemale, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce046.ini"},
	{ITEMID_SandurrRedCostumeMale, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme046.ini"},

	{ITEMID_SandurrPurpleCostumeMale, CHARACTERCLASS_Fighter, "char\\tmABCD\\cb047.ini"},
	{ITEMID_SandurrPurpleCostumeMale, CHARACTERCLASS_Mechanician, "char\\tmABCD\\ca047.ini"},
	{ITEMID_SandurrPurpleCostumeFemale, CHARACTERCLASS_Archer, "char\\tmABCD\\cd047.ini"},
	{ITEMID_SandurrPurpleCostumeMale, CHARACTERCLASS_Pikeman, "char\\tmABCD\\cc047.ini"},
	{ITEMID_SandurrPurpleCostumeFemale, CHARACTERCLASS_Atalanta, "char\\tmABCD\\cmb047.ini"},
	{ITEMID_SandurrPurpleCostumeMale, CHARACTERCLASS_Knight, "char\\tmABCD\\cma047.ini"},
	{ITEMID_SandurrPurpleCostumeMale, CHARACTERCLASS_Magician, "char\\tmABCD\\cmd047.ini"},
	{ITEMID_SandurrPurpleCostumeFemale, CHARACTERCLASS_Priestess, "char\\tmABCD\\cmc047.ini"},
	{ITEMID_SandurrPurpleCostumeFemale, CHARACTERCLASS_Assassin, "char\\tmABCD\\ce047.ini"},
	{ITEMID_SandurrPurpleCostumeMale, CHARACTERCLASS_Shaman, "char\\tmABCD\\cme047.ini"},

	{ ITEMID_ChaoticCostume, CHARACTERCLASS_Fighter, "char\\tmABCD\\rPT_Costume_4_FS.ini" },
	{ ITEMID_ChaoticCostume, CHARACTERCLASS_Mechanician, "char\\tmABCD\\rPT_Costume_4_MS.ini" },
	{ ITEMID_ChaoticCostumeFemale, CHARACTERCLASS_Archer, "char\\tmABCD\\rPT_Costume_4_AS.ini" },
	{ ITEMID_ChaoticCostume, CHARACTERCLASS_Pikeman, "char\\tmABCD\\rPT_Costume_4_PS.ini" },
	{ ITEMID_ChaoticCostumeFemale, CHARACTERCLASS_Atalanta, "char\\tmABCD\\rPT_Costume_4_ATA.ini" },
	{ ITEMID_ChaoticCostume, CHARACTERCLASS_Knight, "char\\tmABCD\\rPT_Costume_4_KS.ini" },
	{ ITEMID_ChaoticCostume, CHARACTERCLASS_Magician, "char\\tmABCD\\rPT_Costume_4_MG.ini" },
	{ ITEMID_ChaoticCostumeFemale, CHARACTERCLASS_Priestess, "char\\tmABCD\\rPT_Costume_4_PRS.ini" },
	{ ITEMID_ChaoticCostumeFemale, CHARACTERCLASS_Assassin, "char\\tmABCD\\rPT_Costume_4_ASS.ini" },
	{ ITEMID_ChaoticCostume, CHARACTERCLASS_Shaman, "char\\tmABCD\\rPT_Costume_4_SS.ini" },
	



	//Akatsuki
	{ITEMID_AkatsukiCostumeMale, CHARACTERCLASS_Fighter, "char\\tmABCD\\AkatsukiCostumeM1.ini"},
};

enum ESpecialUnitType
{
	SPECIALUNITTYPE_None			= 0,

	SPECIALUNITTYPE_QuestWolverine	= 100,
};

struct QuestCharacterSave
{
	WORD				waQuestID[2];

	DWORD				dwaData[7];
};

#define	LAST_QUEST_MAX		32

struct LastQuestSave
{
	WORD				waQuestID[LAST_QUEST_MAX];

	int					iCount;
};

struct CharacterSave
{
	DWORD				dwHeader;

	EMapID				iMapID;

	int					iCameraMode;
	int					iCameraPositionX;
	int					iCameraPositionZ;
	int					iLastGold;

	DWORD				dwChecksum;

	RecordSkill			sSkillInfo;

	int					iSaveTime;

	int					iPadding69[3];

	short				sPotionUpdate[2];
	short				sPotionCount[3][4];
	WORD				wForceOrbUsing[2];

	UINT				iQuestFlag;

	int					iShortKeyDefaultSkill;

	int					iBlessCastleTax;
	int					iBlessCastleClanID;

	int					iPadding72[13];

	int					iTotalSubExp;
	int					iTotalGold;
	int					iTotalExp;	

	char				szAccountName[32];

	QuestCharacterSave	sQuestInfo;
	LastQuestSave		sLastQuestInfo;

	int					iPadding08[35];
};

struct UnitInfo
{
	char					  szName[32];

	MinMax					  sSkillPower;
	int						  iSkillArea;
	int						  iSkillChance;
	int						  iSkillPierceRange;
	int						  iSkillBoxLeft;
	int						  iSkillBoxTop;
	int						  iSkillBoxRight;
	int						  iSkillBoxBottom;
	int						  iSkillType;

	int						  iAttackPattern; // 0x44
	int						  iActiveHour; // 0x48
	int						  iIntelligence; // 0x4C
	int						  iNature; // 0x50
	int						  iUndead; // 0x54

	int						  iGroupLimitMin;
	int						  iGroupLimitMax;
	int						  iExp;
	int						  iLureDistance;

	int						  iMovementRange;

	int						  iFlinch;

	int						  iMaxPotions;					//0x70
	int						  iPerPotions;					//0x74
	BOOL					  bPublicDrop;					//0x78

	int						  iDropCount;
	int						  iDropPercentNum;

	int						  iNumDrops;					//0x84
	int						  iDropPlusCount;

	int						  iRank;

	int						  iUnkj;

	int						  iSight;

	DropSettingsData		  saDropData[200];
	DropSettingsData		  saDropDataCustom[3];

	int						  iWeaponShopCounter;
	DWORD					  dwaWeaponShop[32];

	int						  iDefenseShopCounter;
	DWORD					  dwaDefenseShop[32];

	int						  iMiscShopCounter;
	DWORD					  dwaMiscShop[32];

	int						  iSkillMaster;
	int						  iSkillMasterRank;
	int						  iWarehouse;
	int						  iMixMaster;
	int						  iAgeMaster;
	int						  iSmelting;
	int						  iEventGirl;

	ESpecialUnitType		  eSpecialType;					//0x0BC4
	int						  eMapID;						//0x0BC8
	EItemID					  iQuestItemID;					//0x0BCC
	int						  iBellatraTypeNPC;				//0x0BD0

	int						  iClanController;
	int						  iItemBox;
	int						  iWarpgate;
	int						  iEventNPC;
	int						  iClanNPC;
	int						  iTeleport;
	int						  iCastleController;
	int						  iStarPointNpc;
	int						  iGiveMoneyNpc;
	int						  iTelePortNpc;
	int						  iQuestTypeID;
	int						  iQuestSubID;

	BOOL					  bGMOnly;

	DWORD					  dwHashCharacterName;

	char					* pszaMessage[20];
	int						  iNumMessage;

#if defined(_GAME)
	BYTE				  baUnk[252];
#endif

}; //Size = 0x0C60

struct CharacterData
{
	char				  szName[32];
	union
	{
		struct
		{
			char		  szBodyModel[64];
			char		  szHeadModel[64];
		} Player;
		struct
		{
			char		  szBodyModel[64];
			char		  szHeadModel[60];
			int			  iMonsterID;
		} Monster;
		struct
		{
			char		  szBodyModel[64];
			char		  szHeadModel[60];
			int			  iNPCID;
		} NPC;
		struct
		{
			char		  szBodyModel[65];
			char		  szOwnerName[63];
		} Pet;
		struct
		{
			char		  szBodyModel[64];
			char		  szHeadModel[4];
			char		  szCharacterNameOwner[56];
			int			  iNPCID;
		} XmasTree;
	};
	unsigned int		  iID;					//0xA0
	unsigned int		  iClanID;				//0xA4
	ECharacterType		  iType;				//0xA8
	int					  iShadowSize;			//0xAC
	EMonsterEffectID	  iMonsterEffectID;		//0xB0
	ECharacterClass		  iClass;				//0xB4
	int					  iLevel;				//0xB8
	int					  iStrength;			//0xBC
	int					  iSpirit;				//0xC0
	int					  iTalent;				//0xC4
	int					  iAgility;				//0xC8
	int					  iHealth;				//0xCC
	int					  iAccuracy;			//0xD0
	int					  iAttackRating;		//0xD4
	int					  iMinDamage;			//0xD8
	int					  iMaxDamage;			//0xDC
	int					  iAttackSpeed;			//0xE0
	int					  iAttackRange;			//0xE4
	int					  iCritical;			//0xE8
	int					  iDefenseRating;		//0xEC
	int					  iBlockRating;			//0xF0
	int					  iAbsorbRating;		//0xF4
	int					  iMovementSpeed;		//0xF8
	int					  iSight;				//0xFC
	CurMax				  sWeight;				//0x100
	short				  sElementalDef[8];		//0x104	(index 3 = ice, 4 = poison)
	short				  sElementalAtk[8];		//0x114
	CurMax				  sHP;					//0x124
	CurMax				  sMP;					//0x128
	CurMax				  sSP;					//0x12C
	float				  fHPRegen;				//0x130
	float				  fMPRegen;				//0x134
	float				  fSPRegen;				//0x138
	unsigned int		  iCurrentExpLow;		//0x13C
	int					  iOwnerID;				//0x140
	int					  iGold;				//0x144
	union
	{
		struct UnitInfo	* psUnitInfo;			//0x148
		int				 sBSAL;				//0x148
	};
	EMonsterType		  iMonsterType;			//0x14C
	int					  iStatPoints;			//0x150
	char				  cNewLoad;				//0x154
	char				  cUnknown389103821;	//0x155
	char				  cUnknown358923102;	//0x156
	char				  cUnknown869023021;	//0x157
	MinMax				  sViewBoxZoom;			//0x158

	CurMax				  sHPBlocks;
	int					  iaPadding03;			//0x15C
	int					  iMPType;				//0x164
	BOOL				  bSPType;				//0x168

	int					  iUnknown12;			//0x16C

	DWORD				  dwChecksum;			//0x170
	ECharacterRank		  iRank;				//0x174
	EClassFlag			  iFlag;				//0x178
	short				  sWarpHomeTown;		//0x17C
	short				  sUnused23841212352;	//0x17E
	short				  sGlow;				//0x180
	short				  sSize;				//0x182
	unsigned int		  iCurrentExpHigh;		//0x184
	BOOL				  bCustomHead;			//0x188

	BOOL				  bExclusiveBoss;
	BOOL				  bGrandFuryBoss;

	BYTE				sLevelAgeArmor;
	BYTE				baUnk004848[3];
	int					  iaUnused[13];
	short				  saUnused;
	short				  bResetStatistics;		//Used with Event Girl

}; //Size = 0x01D0

struct PacketUnitInfo : Packet
{
	CharacterData			sCharacterData;
	ID						iID;
	Point3D					sPosition;
	Point3D					sAngle;
	BOOL					bActive;
};

struct PacketUnitInfoQuick : Packet
{
	ID						iID;
	Point3D					sPosition;
	Point3D					sAngle;
	BOOL					bActive;
	CharacterData			sCharacterData;
};

struct	PacketCharacterRecordData : Packet
{
	char			szHeader[8];

	CharacterData	sCharacterData;
	CharacterSave	sCharacterSaveData;

	DropItemData	saDropItemData[64];
	int				iDropItemCount;

	int				iItemCount;
	int				iItemSubStart;

	int				iDataSize;
	BYTE			baData[sizeof(RecordItem) * 200];
};

#define MAX_PKTSIZE_SAVEDATA			(MAX_PKTSIZ - 0x20)

struct PacketCharacterRecordDataSplit : Packet
{
	int					iCurrentPacket;
	int					iMaxPackets;

	int					iSize;
	char				szData[MAX_PKTSIZE_SAVEDATA];
};

struct	CharacterSelectPacketData
{
	char				szName[32];
	char				szBodyModel[64];
	char				szHeadModel[64];
	ECharacterClass		iClass;
	int					iLevel;
	EMonsterType		iType;
	EItemID				iArmorID;
	int					iMapID;
	int					iX;
	int					iZ;
	DWORD				dwUnk001[13];
};


struct	PacketCharacterSelectUser : Packet
{
	char									szAccountName[32];
	int										iCount;

	CharacterSelectPacketData				saData[6];
};


class CharacterDataEx
{
public:
	char			szOldHead[64];
	int				iFSP;
	BYTE			baR5Skill[4];
	bool			bCostume;
};

struct PacketCharacterDataEx : Packet
{
	CharacterDataEx			sData;

	ItemData				cCostumeWeapon;
	ItemData				cCostumeShield;
	ItemData				cEarring1;
	ItemData				cEarring2;
};

struct PacketUpdateIntegrity : Packet
{
	EIntegrityUpdate		eUpdate;
};


class CCharacterSaveData
{
public:
	PacketCharacterRecordData	sPacket;

	int							iPacketCount;

	BYTE						sPacketBlockData[64];		//???

	DWORD						dwSaveTime;
	DWORD						dwSaveTimeNext;

	CCharacterSaveData() {};
	~CCharacterSaveData(){};

	BOOL						LoadInventoyItems( CharacterData & sCharacterData, ItemData * pcaInventory1, ItemData * pcaInventory2, ItemData * pcItemMouse = NULL );
	BOOL						SaveCharacter( CharacterData & sCharacterData, ItemData * pcaInventory1, ItemData * pcaInventory2, ItemData * pcItemMouse = NULL );

private:

};



EClassFlag				CharacterClassToClassFlag( ECharacterClass eClass );
ECharacterClass			ClassFlagToCharacterClass( EClassFlag eFlag );
const char				* GetCharacterClassByFlag( enum EClassFlag eFlag );
ECharacterRace			GetCharacterRace( ECharacterClass iClass );
const char				* GetCharacterRaceName( ECharacterRace eCharacterRace );
CharacterClassData	  * GetCharacterClassData( int iClass, int iRankup );