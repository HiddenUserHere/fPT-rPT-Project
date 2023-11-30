#pragma once
class CFighter
{
private:
	enum Skills
	{
		DefaultAttack = 0x0,
		// Tier 1 -----------------
		Raving = 0x1A,
		Impact = 0x1B,
		// ------------------------
		// Tier 2 -----------------
		TripleImpact = 0x2A,
		BrutalSwing = 0x2B,
		Roar = 0x2C,
		RageOfZecram = 0x2D,
		// ------------------------
		// Tier 3 -----------------
		AvengingCrash = 0x3B,
		BoneCrash = 0x3D,
		// ------------------------
		// Tier 4 -----------------
		Destroyer = 0x4A,
		Berserker = 0x4B,
		CycloneStrike = 0x4C,
		// ------------------------
		// Tier 5 -----------------
		SesmicImpact = 0x5A,
	};
	int recvSkillSingleTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageNormalEx * psPacketEx );
	int recvSkillAreaTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageRange * psPacketRange, PacketSkillDamageRangeEx * psPacketRangeEx );
	int recvBuffSkill( UserData * pcUserData, UINT packet );
public:
	CFighter();
	int AddBerserker( UserData * pcUserData, int TypeDamage );
	int RecvSkillSingleTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageNormalEx * psPacketEx );
	int RecvSkillAreaTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageRange * psPacketRange, PacketSkillDamageRangeEx * psPacketRangeEx );
	int RecvBuffSkill( UserData * pcUserData, UINT packet );
	~CFighter();
};

