#pragma once
class CPriestess
{
struct PacketRestoreHP
{
	int size;
	int op;
	int hpboost;
	int SKILLLVL;
	int pRange;
	int IDPlayer;
};
private:

	int recvSkillSingleTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageNormalEx * psPacketEx );
	int recvSkillAreaTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageRange * psPacketRange, PacketSkillDamageRangeEx * psPacketRangeEx );
	int recvBuffSkill( UserData * pcUserData, UINT packet );
public:
	enum Skills
	{
		// Tier 1 -----------------
		DefaultAttack = 0x0,
		Healing = 0x1A,
		HolyBolt = 0x1B,
		MultiSpark = 0x1C,
		HolyMind = 0x1D,
		// ------------------------
		// Tier 2 -----------------
		DiviningLightning = 0x2A,
		HolyReflection = 0x2B,
		GrandHealing = 0x2C,
		// ------------------------
		// Tier 3 -----------------
		VigorBall = 0x3A,
		Resurrection = 0x3B,
		Extinction = 0x3C,
		VirtualLife = 0x3D,
		// ------------------------
		// Tier 4 -----------------
		GlacialSpike = 0x4A,
		RegenerationField = 0x4B,
		ChainLightning = 0x4C,
		SummonMuspell = 0x4D,
		// ------------------------
		// Tier 5 -----------------
		IceElemental = 0x5A,

	};
	CPriestess();

	static BOOL OnVirtualLife( UserData * pcUserData, PacketSkillPartyData * psPacket );
	int AddSummonMuspellDamageBoost( UserData * pcUserData, int iType );

	void OnUseHolyMind( User * pcUser, PacketHolyMind * psPacket );
	static void OnHolyMindHandler( UserData * pcUserDataTarget, int * piDamage );
	int RecvSkillSingleTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageNormalEx * psPacketEx );
	int RecvSkillAreaTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageRange * psPacketRange, PacketSkillDamageRangeEx * psPacketRangeEx );
	int RecvBuffSkill( UserData * pcUserData, UINT packet );
	BOOL RestoreHPPacketParty( UserData * pcUserData, PacketRestoreHP * psPacket );
	~CPriestess();
};

