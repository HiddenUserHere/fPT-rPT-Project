#pragma once

#define QUESTT5_TIME_START			(20*1000)
#define QUESTT5_TIME_ROUND			(3*60*1000)
#define QUESTT5_TIME_SPAWN			(20*1000)
#define QUESTT5_MAXROUND			5
#define QUESTT5_MAXPOSITIONSPAWN	5

#define QUESTT5ARENA_X_				(488537<<8)
#define QUESTT5ARENA_Z_				0

#define QUESTT5ARENA_X_LEAVE		(-40254<<8)
#define QUESTT5ARENA_Z_LEAVE		(-17511<<8)

static const Point3D saT5QuestArenaSpawnPosition[QUESTT5_MAXPOSITIONSPAWN] =
{
	{ 489124, 3, 190 },
	{ 488532, 3, 614 },
	{ 487949, 3, 187 },
	{ 488178, 3, -503 },
	{ 488903, 3, -499 },
};

static const Point3D saT5QuestCrymeARiver[10] = 
{
	{ 11847, 689, -11369 },
	{ 12656, 680, -11186 },
	{ 12292, 666, -11969 },
	{ 12710, 704, -12841 },
	{ 12490, 750, -13400 },
	{ 12050, 772, -12850 },
	{ 11476, 674, -12603 },
	{ 11022, 680, -12027 },
	{ 11034, 683, -11426 },
	{ 11492, 685, -10823 },
};

struct MonsterDataT5QuestArena
{
	char				* pszName;
	MinMax				sDamage;
	int					iHP;
};

static const MonsterDataT5QuestArena saMonstersT5QuestArena[QUESTT5_MAXROUND] =
{
	{ "Aprisioned Iron Guard", { 50, 50 }, 1000 },
	{ "Aprisioned Morgon", { 50, 50 }, 1200 },
	{ "Aprisioned Ice Goblin", { 50, 50 }, 1500 },
	{ "Aprisioned Cyclops Warrior", { 50, 50 }, 1700 },
	{ "Aprisioned Dark Phalanx", { 50, 50 }, 2000 },
};


class CQuestArenaHandler
{
private:
	void					UpdateQuestT5Unit();
	void					UpdateQuestT5Users();

	void					KillUnitsCrystalT5Quest( UnitData * pcUnitData );
	void					KillUnitsUserT5Quest( User * pcUser, BOOL bDisconnected = FALSE );
	void					SpawnUnitsUserT5Quest( User * pcUser );
	void					SpawnUnitCrystalT5Quest( User * pcUser );

	WORD					wLastMinute = 0xFFFF;

public:
	CQuestArenaHandler();
	virtual ~CQuestArenaHandler();

	BOOL					IsExclusiveUnit( UnitData * pcUnitData, UserData * pcUserData );

	BOOL					IsCrystalTower( UnitData * pcUnitData );

	BOOL					IsUpdatedUnit( UnitData * pcUnitData );

	BOOL					EnterArenaT5Quest( User * pcUser );

	void					HandlePacket( User * pcUser, PacketQuestArenaTier5 * psPacket );
	void					HandlePacket( User * pcUser, PacketSkillDamageRangeEx * psPacket );

	void					ProcessPacket( User * pcUser, PacketQuestArenaTier5 * psPacket ); 

	void					Update();

	BOOL					OnUnitKilled( UserData * pcUserData, UnitData * pcUnitData, BOOL bDamageUsers );
	BOOL					OnUnitKilled( UnitData * pcUnitData );

	BOOL					OnDisconnected( User * pcUser );
	BOOL					OnReconnect( User * pcUser );
};

