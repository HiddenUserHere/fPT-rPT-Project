#pragma once
class CAtalanta
{
private:
	enum Skills
	{
		// Tier 1 -----------------
		DefaultAttack = 0x0,
		ShieldStrike = 0x1A,
		Farina = 0x1B,
		BiggerSpear = 0x1C,
		// ------------------------
		// Tier 2 -----------------
		Windy = 0x2A,
		TwistJavelin = 0x2B,
		SoulSucker = 0x2C,
		FireJavelin = 0x2D,
		// ------------------------
		// Tier 3 -----------------
		SplitJavelin = 0x3A,
		TriumphofValhalla = 0x3B,
		LightningJavelin = 0x3C,
		StormJavelin = 0x3D,
		// ------------------------
		// Tier 4 -----------------
		HallofValhalla = 0x4A,
		ExtremeRage = 0x4B,
		FrostJavelin = 0x4C,
		Vengeance = 0x4D,
		// ------------------------
		// Tier 5 -----------------
		SummonTiger = 0x5A,
	};

	int recvSkillSingleTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageNormalEx * psPacketEx );
	int recvSkillAreaTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageRange * psPacketRange, PacketSkillDamageRangeEx * psPacketRangeEx );
	int recvBuffSkill( UserData * pcUserData, UINT packet );
public:
	CAtalanta( );
	int AddFrostJavelin( UserData * pcUserData, int TypeDamage );
	int AddTriumphofValhalla( UserData * pcUserData, int TypeDamage );
	int RecvSkillSingleTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageNormalEx * psPacketEx );
	int RecvSkillAreaTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageRange * psPacketRange, PacketSkillDamageRangeEx * psPacketRangeEx );
	int RecvBuffSkill( UserData * pcUserData, UINT packet );
	~CAtalanta( );
};

