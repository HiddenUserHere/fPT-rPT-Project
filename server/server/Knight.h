#pragma once
class CKnight
{
private:
	enum Skills
	{
		// Tier 1 -----------------
		DefaultAttack = 0x0,
		SwordBlast = 0x1A,
		HolyBody = 0x1B,
		DoubleCrash = 0x1C,
		// ------------------------
		// Tier 2 -----------------
		HolyValor = 0x2A,
		Brandish = 0x2B,
		Piercing = 0x2C,
		// ------------------------
		// Tier 3 -----------------
		DivineShield = 0x3A,
		HolyIncantation = 0x3B,
		GrandCross = 0x3C,
		// ------------------------
		// Tier 4 -----------------
		SwordofJustice = 0x4A,
		GodlyShield = 0x4B,
		GodsBlessing = 0x4C,
		DivinePiercing = 0x4D,
		// ------------------------
		// Tier 5 -----------------
		HolyConviction = 0x5A,
	};


public:
	CKnight( );
	int AddGodsBlessing( UserData * pcUserData, int TypeDamage );
	int AddHolyConvictionUndeadDamage( UserData * pcUserData, UnitData * pcUnitData, int TypeDamage );
	int RecvSkillSingleTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageNormalEx * psPacketEx );
	int RecvSkillAreaTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageRange * psPacketRange, PacketSkillDamageRangeEx * psPacketRangeEx );
	int RecvBuffSkill( UserData * pcUserData, UINT packet );
	static int AddHolyValor( UserData * pcUserData, PacketSkillPartyData * packet );
	static int AddHolyConviction( User * pcUser, User * pcTarget, PacketSkillPartyData * packet );
	static void OnDrasticSpirit( UserData * pcUserData );

	~CKnight( );
};

