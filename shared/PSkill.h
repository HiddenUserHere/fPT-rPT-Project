#pragma once

//Fighter
const int iaSurvivalInstinctAddHP[10] = { 5, 10, 15, 20, 25, 30, 35, 40, 45, 50 };
const int iaSurvivalInstinctBoostHPRegen[10] = { 2, 4, 6, 8, 10, 12, 14, 16, 18, 20 };

const int iaSesmicImpactDamage[10] = { 70, 80, 90, 100, 120, 140, 160, 180, 190, 200 };

const int iaSesmicImpactArea[10] = { 50, 55, 60, 65, 70, 75, 80, 85, 90, 95 };

static float FST5A3_1[10]
{
	5.6f, 5.2f, 4.8f, 4.4f, 4.0f, 3.6f, 3.2f, 2.8f, 2.4f, 2.0f
};

static int FST5A3_2[10]
{
	6, 6, 6, 5, 5, 5, 4, 4, 4, 3
};

static int FST5A3_3[10]
{
	12, 14, 16, 18, 20, 22, 24, 26, 28, 30
};

static int FST5A4_1[10]
{
	17, 24, 31, 38, 45, 52, 59, 66, 73, 80
};

static int FST5A4_2[10]
{
	12, 14, 16, 18, 20, 22, 24, 26, 28, 30
};

static int FST5A4_3[10]
{
	17, 19, 21, 23, 25, 27, 29, 31, 33, 35
};

static int FST5A4_4[10]
{
	29, 31, 35, 37, 39, 41, 43, 45, 47, 50
};


const int iaFierceWindDamageBoost[10] = { 60, 70, 80, 90, 100, 110, 120, 130, 140 };

const int iaFierceWindTimeOut[10] = { 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 };


//Atalanta
const int iaSummonTigerAttackPower[10] = { 140, 160, 180, 200, 220, 240, 260, 280, 300, 320 };
const int iaSummonTigerAttackRating[10] = { 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000 };
const int iaSummonTigerHP[10] = { 10000, 12000, 14000, 16000, 18000, 20000, 22000, 24000, 26000, 28000 };

//Priestess
const int iaIceElementalAttackPower[10] = { 140, 160, 180, 200, 220, 240, 260, 280, 300, 320 };
const int iaIceElementalAttackRating[10] = { 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000 };
const int iaIceElementalHP[10] = { 10000, 12000, 14000, 16000, 18000, 20000, 22000, 24000, 26000, 28000 };

const int iaSummonMuspellDamageBoostMin[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
const int iaSummonMuspellDamageBoostMax[10] = { 2, 4, 6, 8, 10, 11, 12, 13, 14, 15 };

//Magician
const int iaStoneSkinImmortal[10] = { 2, 2, 2, 3, 3, 3, 4, 4, 4, 5 };

const int iaSpiritElementalMin[10] = { 2, 4, 6, 8, 10, 12, 14, 16, 18, 20 };
const int iaSpiritElementalMax[10] = { 4, 8, 12, 16, 20, 24, 28, 32, 36, 40 };

//Shaman
const int iaChasingHuntMin[10] = { 2, 4, 6, 8, 10, 12, 14, 16, 18, 20 };
const int iaChasingHuntMax[10] = { 3, 6, 9, 12, 15, 18, 21, 24, 27, 30 };


struct SkillValueDataTable
{
	int				iSkillID;

	DWORD			dwAddressGame;

	DWORD			dwAddressServer;

	struct SkillValueInt
	{
		int i[10];
	};

	struct SkillValueFloat
	{
		float f[10];
	};

	union
	{
		SkillValueInt		sValue;

		SkillValueFloat		fValue;
	};
};


static SkillValueDataTable saSkillValueData[300]
{
	{
	0,0,0,
	SkillValueDataTable::SkillValueInt{0,0,0,0,0,0,0,0,0,0}
	},
};

