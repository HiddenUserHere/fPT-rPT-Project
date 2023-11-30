#pragma once
class CArcher
{
private:
	enum Skills
	{
		// Tier 1 -----------------
		DefaultAttack = 0x0,
		WindArrow = 0x1B,
		PerfectAim = 0x1C,
		// ------------------------
		// Tier 2 -----------------
		Falcon = 0x2A,
		ArrowofRage = 0x2B,
		Avalanche = 0x2C,
		// ------------------------
		// Tier 3 -----------------
		ElementalShot = 0x3A,
		GoldenFalcon = 0x3B,
		BombShot = 0x3C,
		Perforation = 0x3D,
		// ------------------------
		// Tier 4 -----------------
		RecallWolverine = 0x4A,
		PhoenixShot = 0x4B,
		ForceofNature = 0x4C,
		// ------------------------
		// Tier 5 -----------------
		FierceWind = 0x5A,
	};

public:
	CArcher( );
	int AddForceOfNature( UserData * pcUserData, int TypeDamage );
	int RecvSkillSingleTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageNormalEx * psPacketEx );
	int RecvSkillAreaTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageRange * psPacketRange, PacketSkillDamageRangeEx * psPacketRangeEx );
	int RecvBuffSkill( UserData * pcUserData, UINT packet );

	void OnAddFinalDamage( UserData * pcUserData, PacketSkillDamageNormal * psPacket );

	~CArcher();
};

