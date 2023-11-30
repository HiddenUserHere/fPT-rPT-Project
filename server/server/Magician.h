#pragma once
class CMagician
{
private:
	enum Skills
	{
		// Tier 1 -----------------
		DefaultAttack = 0x0,
		FireBolt = 0x1B,
		FireBall = 0x1D,
		// DoubleCrash = 0x1C,
		// ------------------------
		// Tier 2 -----------------
		Watornado = 0x2A,
		EnchantWeapon = 0x2B,
		DeathRay = 0x2C,
		// ------------------------
		// Tier 3 -----------------
		EnergyShield = 0x3A,
		Diastrophism = 0x3B,
		SpiritElemental = 0x3C,
		DancingSword = 0x3D,
		// ------------------------
		// Tier 4 -----------------
		FireElemental = 0x4A,
		FlameWave = 0x4B,
		Distortion = 0x4C,
		Meteorite = 0x4D,
		// ------------------------
		// Tier 5 -----------------
		WizardTrance = 0x5A,
	};

public:
	CMagician();
	static int UseDistortionMonster( UserData * pcUserData, PacketSkillDamageRange * psPacket, int iLevelSkill );
	static int UseDistortionUser( UserData * pcUserData, int iTimeByLevel );
	int AddSpiritElemental( UserData * pcUserData, int TypeDamage );
	int AddEnchantWeapon( UserData * pcUserData, UINT ArraySkill );
	int RecvSkillSingleTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageNormalEx * psPacketEx );
	int RecvSkillAreaTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageRange * psPacketRange, PacketSkillDamageRangeEx * psPacketRangeEx );
	int RecvBuffSkill( UserData * pcUserData, UINT packet );

	static BOOL	OnEnchantWeapon( UserData * pcUserData, PacketSkillPartyData * psPacket );

	~CMagician();
};

