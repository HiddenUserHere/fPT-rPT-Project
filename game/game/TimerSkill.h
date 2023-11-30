#pragma once

#define MAX_SKILLS_TIMER		80

enum ETimerSkillID
{
	TIMERSKILLID_QuestNew						= 0x096A0003,
	TIMERSKILLID_QuestNewT						= 0x096B0003,
	TIMERSKILLID_BellatraGold					= 0x096A0061,
	TIMERSKILLID_BellatraSilver					= 0x096A0062,
	TIMERSKILLID_BellatraBronze					= 0x096A0063,
	TIMERSKILLID_ChasingHunt					= 0x50020003,
	TIMERSKILLID_Compulsion						= 0x01080002,
	TIMERSKILLID_RollDice						= 0x096B0004,
};

class CTimerSkill
{
protected:
	DWORD										dwChasingHuntTimerTexture = 0;

private:
	static void										WritePointer( DWORD Addr, void * ptr );
	const char									* pszTimerDescription	= (char*)0x036762A0;
	const char									* pszTimerValue			= (char*)0x03677628;
public:
	CTimerSkill();
	virtual ~CTimerSkill();

	static void									Hooks();

	BOOL										KillTimer( DWORD dwCode );
	BOOL										KillTimerQuest( DWORD dwCode, int iQuestID );

	DWORD										LoadSkillTimerTexture( const char * pszFileName );

	BOOL										AddSkillTimer( struct Skill * psSkill );

	struct Skill								* FindSkillTimer( DWORD dwCode );
	struct Skill								* GetPointerSkillTimer();

	int											OnRenderTimer( Skill * psSkill );

	int											GetArraySize();

	BOOL										OnAddChasingHunt( int iTime, int iLevel );

	void										PacketHandlerCrystal( struct PacketCrystalUse * psPacket );
	void										PacketHandlerCrystal( struct PacketCrystalUseDie * psPacket );

	void										OnCharacterKilled();

	void										DrawTimer();
	void										DrawTimerBox();

	void										SwapSkillTimer( Skill * psSkill );

	void										Loop();
};

