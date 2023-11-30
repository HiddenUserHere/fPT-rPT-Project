#pragma once

static int iaCriticalJudment[10]
{
	3, 3, 3, 4, 4, 4, 5, 5, 5, 6
};

class CShaman
{
private:
	enum Skills
	{
		DefaultAttack = 0x0,
		// Tier 1 -----------------
		DarkBolt = 0x1A,
		DarkWave = 0x1B,
		Inertia = 0x1C,
		InnerPeace = 0x1D,
		// ------------------------
		// Tier 2 -----------------
		SpiritualFlare = 0x2A,
		SpiritualManacle = 0x2B,
		ChasingHunt = 0x2C,
		AdventMigal = 0x2D,
		// ------------------------
		// Tier 3 -----------------
		Rainmaker = 0x3A,
		PhantomCall = 0x3B,
		Haunt = 0x3C,
		Scratch = 0x3D,
		// ------------------------
		// Tier 4 -----------------
		RecallBloodyKnight = 0x4A,
		Judgement = 0x4B,
		AdventMidranda = 0x4C,
		MourningPray = 0x4D
		// ------------------------
	};

public:
	CShaman();
	virtual ~CShaman();

	int RecvSkillSingleTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageNormalEx * psPacketEx );
	int RecvSkillAreaTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageRange * psPacketRange, PacketSkillDamageRangeEx * psPacketRangeEx );
	int RecvBuffSkill( UserData * pcUserData, UINT packet );

	static BOOL AddAdventMigal( UserData * pcUserData, PacketSkillPartyData * packet );
	static BOOL OnRainMakerEvade( UserData * pcUserData, DWORD dwObjectID );
	int AddAdventMigalDamage( UserData * pcUserData );
	int AddChasingHuntAtkPow( UserData * pcUserData, int iType );
};

