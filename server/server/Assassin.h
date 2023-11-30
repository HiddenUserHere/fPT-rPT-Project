#pragma once

static int iaDMGBoostASST5S1[10]
{
	47, 52, 57, 62, 67, 72, 77, 82, 87, 92
};

static int iaDMGBoostASST5S2[10]
{
	60, 65, 70, 75, 80, 85, 90, 95, 100, 105
};

static int iaDMGBoostASST5S3[10]
{
	35, 40, 45, 50, 55, 60, 65, 70, 75, 80
};

static int iaDMGBoostASST5S4[10]
{
	60, 64, 68, 72, 76, 80, 84, 88, 92, 96
};
static int iaDMGBoostAreaASST5S4[10]
{
	76, 78, 80, 82, 84, 86, 88, 90, 92, 94
};
static int iaDMGBoostRangeASST5S4[10]
{
	53, 56, 59, 62, 65, 68, 71, 74, 77, 80
};
static int iaAreaASST5S4[10]
{
	86, 90, 94, 98, 102, 106, 110, 114, 118, 122
};
static int iaPushRangeASST5S4[10]
{
	85, 90, 95, 100, 105, 110, 115, 120, 125, 130
};

class CAssassin
{
private:
	enum Skills
	{
		DefaultAttack = 0x0,
		// Tier 1 -----------------
		Stinger = 0x1A,
		RunningHit = 0x1B,
		Wisp = 0x1D,
		// ------------------------
		// Tier 2 -----------------
		VenomThorn = 0x2A,
		Alas = 0x2B,
		SoulShock = 0x2C,
		AttackMastery = 0x2D,
		// ------------------------
		// Tier 3 -----------------
		SoreBlade = 0x3A,
		GustSlash = 0x3B,
		Inpes = 0x3C,
		Blind = 0x3D,
		// ------------------------
		// Tier 4 -----------------
		FrostWind = 0x4A,
		Polluted = 0x4C,
		NinjaShadow = 0x4D,
		// ------------------------
		// Tier 4 -----------------
		ShadowBlink = 0x5A,
		RisingSlash = 0x5B,
		ViolentStub = 0x5C,
		ShadowStorm = 0x5D
		// ------------------------
	};
public:

	int RecvSkillSingleTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageNormalEx * psPacketEx );
	int RecvSkillAreaTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageRange * psPacketRange, PacketSkillDamageRangeEx * psPacketRangeEx );
	int RecvBuffSkill( UserData * pcUserData, UINT packet );

	void OnAddFinalDamage( UserData * pcUserData, PacketSkillDamageNormal * psPacket );

	static BOOL AddAlas( User * pcUser, User * pcTarget, PacketSkillPartyData * packet );
	static BOOL AddKissViper( User * pcUser, User * pcTarget, PacketSkillPartyData * packet );
	static BOOL OnAlasEvade( UserData * pcUserData, DWORD dwObjectID );
	static void OnUseWisp( User * pcUser, PacketWisp * psPacket );

	CAssassin();
	virtual ~CAssassin();
};
