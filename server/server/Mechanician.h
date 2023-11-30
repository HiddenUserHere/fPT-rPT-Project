#pragma once
class CMechanician
{
private:
	enum Skills
	{
		// Tier 1 -----------------
		DefaultAttack = 0x0,
		ExtremeShield = 0x1A,
		MechanicBomb = 0x1B,
		PhysicalAbsorption = 0x1C,
		// ------------------------
		// Tier 2 -----------------
		GreatSmash = 0x2A,
		Maximize = 0x2B,
		Automation = 0x2C,
		Spark = 0x2D,
		// ------------------------
		// Tier 3 -----------------
		MetalArmor = 0x3A,
		GrandSmash = 0x3B,
		SparkShield = 0x3C,
		// ------------------------
		// Tier 4 -----------------
		Impulsion = 0x4A,
		Compulsion = 0x4B,
		MagneticSphere = 0x4C,
		MetalGolem = 0x4D,
		// ------------------------
		// Tier 4 -----------------
		Rupture = 0x5A,
	};
	int recvSkillSingleTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageNormalEx * psPacketEx );
	int recvSkillAreaTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageRange * psPacketRange, PacketSkillDamageRangeEx * psPacketRangeEx );
	int recvBuffSkill( UserData * pcUserData, UINT packet );
public:
	CMechanician( );
	int AddMaximize( UserData * pcUserData, int TypeDamage );
	int AddAutomation( UserData * pcUserData, UnitData * pcUnitData, int TypeDamage, UINT packet );
	int RecvSkillSingleTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageNormalEx * psPacketEx );
	int RecvSkillAreaTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageRange * psPacketRange, PacketSkillDamageRangeEx * psPacketRangeEx );
	int RecvBuffSkill( UserData * pcUserData, UINT packet );

	static BOOL AddMagneticDischarge( User * pcUser, int iLevel, int iLevelMagneticSphere, bool bSelf = false );
	static BOOL DoMagneticDischargeDamage( User * pcUser, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageNormalEx * psPacketEx );

	~CMechanician( );
};
