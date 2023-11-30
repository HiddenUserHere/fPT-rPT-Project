#pragma once

static int iaSkillTornadoRange[10]
{
	44, 48, 52, 56, 60, 64, 68, 72, 76, 120
};

static int iaSkillVenomSpear[10]
{
	90, 94, 98, 102, 106, 110, 114, 118, 122, 126
};

class CPikeman
{
private:
	enum Skills
	{
		// Tier 1 -----------------
		DefaultAttack = 0x0,
		PikeWind = 0x1A,
		CriticalHit = 0x1B,
		JumpingCrash = 0x1C,
		// ------------------------
		// Tier 2 -----------------
		GroundPike = 0x2A,
		Tornado = 0x2B,
		Expansion = 0x2C,
		// ------------------------
		// Tier 3 -----------------
		VenomSpear = 0x3A,
		Vanish = 0x3B,
		ChainLance = 0x3C,
		// ------------------------
		// Tier 4 -----------------
		AssasinsEye = 0x4A,
		ChargingStrike = 0x4B,
		Vague = 0x4C,
		ShadowMaster = 0x4D,
		// ------------------------
		// Tier 5 -----------------
		RingOfSpears = 0x5A,
		LastBreath = 0x5B,
	};

	int recvSkillSingleTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageNormalEx * psPacketEx );
	int recvSkillAreaTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageRange * psPacketRange, PacketSkillDamageRangeEx * psPacketRangeEx );
	int recvBuffSkill( UserData * pcUserData, UINT packet );
public:
	CPikeman( );
	int RecvSkillSingleTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageNormalEx * psPacketEx );
	int RecvSkillAreaTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageRange * psPacketRange, PacketSkillDamageRangeEx * psPacketRangeEx );
	int RecvBuffSkill( UserData * pcUserData, UINT packet );
	~CPikeman( );
};

