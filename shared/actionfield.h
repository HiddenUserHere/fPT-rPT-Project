#pragma once

enum EActionFieldModeID : int
{
	ACTIONFIELDID_None				= 0,
	ACTIONFIELDID_Distorted			= 100,
	ACTIONFIELDID_GhostCastle1		= 200,
	ACTIONFIELDID_GhostCastle2		= 201,
	ACTIONFIELDID_Swamp				= 300,
	ACTIONFIELDID_ForestDungeon1	= 400,
	ACTIONFIELDID_ForestDungeon2	= 401,
	ACTIONFIELDID_ForestDungeon3	= 402,
	ACTIONFIELDID_DragonsDungeon1	= 500,
	ACTIONFIELDID_DragonsDungeon2	= 501,
	ACTIONFIELDID_DragonsDungeon3	= 502,
};

enum EActionFieldFail : int
{
	ACTIONFIELDFAIL_None,
	ACTIONFIELDFAIL_Disconnected,
	ACTIONFIELDFAIL_EndTime,
	ACTIONFIELDFAIL_Killed,
	ACTIONFIELDFAIL_LeaveParty,
	ACTIONFIELDFAIL_LeaveMap,
};

#define MAX_ACTIONFIELD_MONSTERS	20

#define MAX_ACTIONFIELD_USERS		6

#define MAX_ACTIONFIELD_NPCMODE		5

struct ActionFieldChest
{
	int					iID;

	EItemID				eItemID;
	char				szItemCode[10];

	char				szRequiredItemCode[10];
	int					iRequiredItemCount;
};

struct ActionFieldMonster
{
	CharacterData		sCharacterData;
	UnitInfo			sUnitInfo;

	int					iPoints;

	BOOL				bIsBoss;
};

struct ActionFieldReward
{
	int					iChestID;

	char				szItemCode[10];
	int					iQuantity;
	int					iPercentChance;
};

struct ActionFieldRound
{
	int					iID;

	int					iLevel;

	int					iBeginTime;
	int					iEndTime;

	int					iBossSpawnKills;
	
	int					iKillsNeeded;

	int					iMonsterCount;
	ActionFieldMonster	saMonsters[MAX_ACTIONFIELD_MONSTERS];

	std::vector<Point3D> vSpawnMonsterPosition;
	std::vector<Point3D> vSpawnBossPosition;
};

struct ActionFieldSettings
{
	IMinMax				sRequiredLevel;
	IMinMax				sRequiredUsers;

	int					iGoldEntranceLevelMultiplier;
	int					iGoldEntranceExtra;

	int					iDurationTime;

	int					iInstanceDayLimit;
};

struct ActionFieldUser
{
	int					iID;

	int					iKills;
	int					iScore;
};

struct PacketActionFieldEnter : Packet
{
	PacketActionFieldEnter( EActionFieldModeID iModeID ) { this->iModeID = iModeID; }
	~PacketActionFieldEnter() {};

	EActionFieldModeID	iModeID;
};

struct PacketActionFieldFail : Packet
{
	PacketActionFieldFail( EActionFieldFail i ) { iCode = i; };
	~PacketActionFieldFail() {};

	EActionFieldFail	iCode;
};

struct PacketActionFieldUpdate : Packet
{
	int					iUsersCount;
	ActionFieldUser		saUsers[MAX_ACTIONFIELD_USERS];

	char				szName[64];

	int					iRound;

	int					iTime;

	BOOL				bStarted;
	BOOL				bFinished;
};

struct PacketActionFieldStart : Packet
{
	PacketActionFieldStart( int i ) { iTime = i; }
	~PacketActionFieldStart() {};

	int					iTime;
};

struct PacketActionFieldEnd : Packet
{
	PacketActionFieldEnd( int i ) { iTime = i; }
	~PacketActionFieldEnd() {};

	int					iTime;
};


struct PacketActionFieldRequest : Packet
{
	PacketActionFieldRequest() {};
	PacketActionFieldRequest( EActionFieldModeID i ) { iaModeID[0] = i; };
	~PacketActionFieldRequest() {};


	int					iCount;
	EActionFieldModeID	iaModeID[MAX_ACTIONFIELD_NPCMODE];
	char				szaName[MAX_ACTIONFIELD_NPCMODE][32];
	IMinMax				saRequiredLevel[MAX_ACTIONFIELD_NPCMODE];
	int					iaInstanceCountToday[MAX_ACTIONFIELD_NPCMODE];
	int					iaInstanceLimitPerDay[MAX_ACTIONFIELD_NPCMODE];
};

struct PacketActionFieldExchange : Packet
{
	BOOL					bSuccess;

	EItemID					iItemID;
	int						iChk1;
	int						iChk2;

	int						iItemCount;
	EItemID					iaKeyItemID[12];
	int						iaKeyChk1[12];
	int						iaKeyChk2[12];
};

class CActionField
{
public:
	CActionField( int iID, const char szName[64], EActionFieldModeID iModeID );
	virtual ~CActionField();

	int								GetID() { return iID; }
	char							* GetName() { return szName; }
	EActionFieldModeID				GetModeID() { return iModeID; }

	ActionFieldSettings				& GetSettings() { return sSettings; }

	std::vector<ActionFieldChest*>	& GetChests() { return vChests; }
	std::vector<ActionFieldReward*> & GetRewards() { return vRewards; }
	std::vector<ActionFieldRound*>	& GetRounds() { return vRounds; }

	std::vector<ActionFieldReward*> GetRewards( int iChestID );


	ActionFieldRound				* GetRound( int iRoundLevel );

	void							Clear();

	void							SetSettings( ActionFieldSettings & sSettings );
	void							AddChest( ActionFieldChest & sChest );
	void							AddReward( ActionFieldReward & sReward );
	void							AddRound( ActionFieldRound & sRound );

	ActionFieldChest				* GetChestItem( const char * pszCode );

private:
	int								iID;
	char							szName[64];
	EActionFieldModeID				iModeID;

	ActionFieldSettings				sSettings;

	std::vector<ActionFieldChest*>	vChests;
	std::vector<ActionFieldReward*> vRewards;
	std::vector<ActionFieldRound*>	vRounds;
};
