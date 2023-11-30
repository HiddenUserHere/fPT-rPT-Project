#pragma once
#include <Windows.h>
#include <strsafe.h>
#include <vector>

struct IMinMax;

#define QUEST_MONSTERITEM_MAX				10

enum EQuestType
{
	QUESTTYPE_None,
	QUESTTYPE_Solo,
	QUESTTYPE_Party,
	QUESTTYPE_Daily,
	QUESTTYPE_DailyMidNight,
	QUESTTYPE_DailyWeek,
	QUESTTYPE_DailyMonth,
	QUESTTYPE_Repeatable,
	QUESTTYPE_Weekend,
	QUESTTYPE_WeekendDaily,
	QUESTTYPE_WeekendDailyMidNight,
	QUESTTYPE_WeekendRepeatable,
	QUESTTYPE_WeekFriday,
};

enum EQuestDurationType
{
	QUESTDURATIONTYPE_None,
	QUESTDURATIONTYPE_Running,
	QUESTDURATIONTYPE_Amount,
};

enum EQuestAreaType
{
	QUESTAREATYPE_None,
	QUESTAREATYPE_Bound,
	QUESTAREATYPE_Radius,

};

enum EQuestExtraRewardType
{
	QUESTREWARDTYPE_None,
	QUESTREWARDTYPE_Gold,
	QUESTREWARDTYPE_WarehousePage,
	QUESTREWARDTYPE_EXPPotBuff,
	QUESTREWARDTYPE_ThirdEyeBuff,
	QUESTREWARDTYPE_HPUpBuff,
	QUESTREWARDTYPE_MPUpBuff,
};

enum EQuestID
{
	QUESTID_None,

	QUESTID_General					= 1500,
	QUESTID_Warehouse2Page			= QUESTID_General + 1,
	QUESTID_WarehousePageFinish		= QUESTID_General + 2,
	QUESTID_DailyQuestsIan			= 1503,
	QUESTID_DailyQuest80A			= 1504,
	QUESTID_DailyQuest85A			= 1505,
	QUESTID_DailyQuest90A			= 1506,
	QUESTID_DailyQuest95A			= 1507,
	QUESTID_DailyQuest100A			= 1508,
	QUESTID_DailyQuest105A			= 1509,
	QUESTID_DailyQuest110A			= 1510,
	QUESTID_DailyQuest115A			= 1511,
	QUESTID_DailyQuestLimit			= 1550,

	//Normal Quests
	QUESTID_BlacksmithGodenGold		= 1551,
	QUESTID_WarehouseGold			= 1552,
	QUESTID_Quest110A				= 1553,
	QUESTID_AnAncientThreat			= 2001,
	QUESTID_AnEndlessBattle			= 2006,
	QUESTID_AnEndlessSuffering		= 2007,
	QUESTID_ATestOfBravery			= 2012,
	QUESTID_GreenLiquidQuestStep1	= 2049,
	QUESTID_GreenLiquidQuestStep2	= 2050,
	QUESTID_TheLoyaltyTestStep1		= 2068,
	QUESTID_TheLoyaltyTestStep2		= 2069,

	QUESTID_MercyfortheLost2		= 3030,
	QUESTID_HuntingTheIronGuardian	= 3031,

	QUESTID_TheDemonAngel			= 1669,

	//Ice Mine Quests
	QUESTID_MinersFearsEasy			= 1512,
	QUESTID_MinersFearsMedium		= 1514,
	QUESTID_MinersFearsHard			= 1515,
	QUESTID_TullaMenace				= 1513,
	QUESTID_MineAlone				= 2014,

	//Abyss Quests
	QUESTID_TheEliteOfFear			= 1700,
	QUESTID_TheReturnOfMutants		= 1701,
	QUESTID_TheAbyssHunter			= 1702,
	QUESTID_ATestofLoneliness		= 2013,
	QUESTID_UndertheAbyss			= 2015,

	//Secret Laboratory Quests
	QUESTID_TheSecretRevealed		= 2024,


	//T5 Rankup
	QUESTID_AMatterOfStrenght		= 2100,
	QUESTID_CryMeARiver				= 2101,
	QUESTID_TheLastStage			= 2102,
	QUESTID_T5QuestArena			= 2105,

	QUESTID_KingHopyQuest			= 3032,

	QUESTID_TheMineCommander		= 4060,
	QUESTID_Retrievingbackthelight	= 4064,
	QUESTID_TheGreedy				= 4065,
	QUESTID_VestigesOfAncientWar	= 4066,
	QUESTID_CommanderSaxhleel		= 4067,
	QUESTID_TheStoneGod				= 4068,
	QUESTID_TheReturnOfTheLizards	= 4069,
	QUESTID_TheunknownplanofHagios  = 4070,
	QUESTID_SeeknDestroyYagditha	= 4071,
	QUESTID_TheEasterChef			= 60608,

	QUESTID_FlamoQuest				= 8524,

	//Dance
	QUESTID_AnimationDance			= 60617,

	//CoinShop
	QUESTID_CoinShopNPC				= 15000,
	QUESTID_EasterNPC				= 15001,
	QUESTID_RudolphNPC				= 15002,

	QUESTID_LordSantaGoblin			= 60606,
	QUESTID_LordSantaSkeleton		= 60607,
	QUESTID_PelgiaQuest				= 60616,
	QUESTID_Pumpking				= 60618,
};


enum EQuestMonsterID
{
	QUESTMONSTERID_Ratoo		= 10,
	QUESTMONSTERID_IronGuard	= 24,
	QUESTMONSTERID_CrystalTower	= 2105,
};

enum EQuestStatus
{
	QUESTSTATUSID_None,
	QUESTSTATUSID_Killing,
	QUESTSTATUSID_GetItem,
	QUESTSTATUSID_EndQuest,
	QUESTSTATUSID_Finished		= 2000,

	// Add From Here
	QUESTSTATUSID_T5Quest1
};

enum EQuestSaveFlag : int
{
	QUESTSAVEFLAG_TheLoyaltyTest = 256,
};

#define MAX_QUESTDATA		5

struct QuestDataNew
{
	int										iaQuestID[MAX_QUESTDATA];
	int										iaKillCount[MAX_QUESTDATA];
	int										iaItemCount[MAX_QUESTDATA];
};

struct PacketQuestFinish : Packet
{
	DWORD									dwQuestID;
	EQuestStatus							eQuestStatus;
};

struct PacketQuestRestart : Packet
{
	DWORD									dwQuestID;
};
struct PacketQuestCancel : Packet
{
	DWORD									dwQuestID;
};

struct PacketQuestNPCOpen : Packet
{
	int										iID;
	int										iType;

	char									szName[64];
	char									szDescription[2048];
	char									szProgressText[2048];
	char									szConclusion[2048];
};

struct PacketQuestNPCAccept : Packet
{
	int										iID;

	PacketQuestNPCAccept() { iID = 0; }
	PacketQuestNPCAccept( int i ) { iID = i; }
};


struct PacketQuestNPCFinish : Packet
{
	int										iID;

	int										iItemIndex;

	short									sWeight[2];

	BOOL									bHaveSpace;
};


struct PacketQuestHandleCancel : Packet
{
	int										iID;
};

struct PacketHandleFinishedQuest : Packet
{
	int										iID;

	int										iType;
	
	SYSTEMTIME								sStartDate;
	SYSTEMTIME								sEndDate;

	int										iaExtraReward[QUEST_MONSTERITEM_MAX];
	int										iaExtraRewardValue[QUEST_MONSTERITEM_MAX];

	BOOL									bLoginTime;
};

struct PacketQuestUpdateData : Packet
{
	int										iID;

	DWORD									dwTimeLeft;

	int										iMonsterCount;
	int										iaMonsterKilled[QUEST_MONSTERITEM_MAX];
	int										iItemsCount;
	int										iaItemsCollected[QUEST_MONSTERITEM_MAX];
};

struct PacketNetQuestUpdateData : Packet
{
	int										iID;
	struct PacketQuestUpdateData			saPacket[MAX_QUESTPACKETDATA];
};

#define MAX_QUEST_MAPDATA		5

struct PacketQuestStartData : Packet
{
	int										iID;
	char									szName[64];
	char									szShortDescription[128];

	int										iType;

	int										iLevel;

	BOOL									bTime;
	DWORD									dwTimeLeft;
	DWORD									dwWaitingTime;
	DWORD									dwTimeTotal;

	int										iMapCount;
	int										iaMapID[MAX_QUEST_MAPDATA];

	int										iAreaType;

	int										iXMin;
	int										iXMax;
	int										iZMin;
	int										iZMax;

	int										iRadius;

	int										iParty;

	BOOL									bPvP;

	BOOL									bMultiple;

	int										iaMonsterID[QUEST_MONSTERITEM_MAX];
	int										iaMonsterCounter[QUEST_MONSTERITEM_MAX];
	int										iaMonsterKilled[QUEST_MONSTERITEM_MAX];
	char									szaMonsterName[QUEST_MONSTERITEM_MAX][32];

	int										iaItemID[QUEST_MONSTERITEM_MAX];
	int										iaItemCounter[QUEST_MONSTERITEM_MAX];
	int										iaItemsCollected[QUEST_MONSTERITEM_MAX];

};


struct PacketDynamicQuestNPCOpen : Packet
{
	int										iQuestID;
};

struct PacketQuestItemSwap : Packet
{
	int					iQuestID;

	int					iItemID;
	int					iChk1;
	int					iChk2;
};

struct PacketCustomQuestValue : Packet
{
	int					iQuestID;

	int					iValue1;
	int					iValue2;
	int					iValue3;
};

struct QuestFinishedData
{
	int										iID;

	int										iType;

	SYSTEMTIME								sStartDate;
	SYSTEMTIME								sEndDate;

	int										iaExtraReward[QUEST_MONSTERITEM_MAX];
	int										iaExtraRewardValue[QUEST_MONSTERITEM_MAX];
};



class QuestData
{
private:
	int							iID;

	int							iaMonsterID[QUEST_MONSTERITEM_MAX];
	int							iaMonsterCounter[QUEST_MONSTERITEM_MAX];
	int							iaMonsterKilled[QUEST_MONSTERITEM_MAX];

	char						szaMonsterName[QUEST_MONSTERITEM_MAX][32];

	int							iaItemID[QUEST_MONSTERITEM_MAX];
	int							iaItemCounter[QUEST_MONSTERITEM_MAX];
	int							iaItemsCollected[QUEST_MONSTERITEM_MAX];

	char						szaItemName[QUEST_MONSTERITEM_MAX][32];

	char						szName[64];
	char						szDescription[128];

	int							iType;

	int							iLevel;

	BOOL						bPvP;
	
	std::vector<EMapID>			vMapIDs;

	int							iAreaType;

	BOOL						bTime;
	DWORD						dwTimeLeft;
	DWORD						dwWaitingTime;
	DWORD						dwTimeTotal;

	int							iXMin, iXMax;
	int							iZMin, iZMax;

	int							iRadius;

	int							iParty;
	BOOL						bMultiple;

	BOOL						bKillsFinished = FALSE;
	BOOL						bItemsFinished = FALSE;

	DWORD						pfnFinished = 0;

	BOOL						bUpdate = FALSE;

	UINT						uQuestValidateID = 0;
	UINT						uValidateWheel = 0;

	void						HandleFinish();

public:
	//Constructor
	QuestData( PacketQuestStartData * psPacket );
	virtual ~QuestData();

	void						SetName( char * pszQuestName );
	void						SetDescription( char * pszQuestDescription );

	void						SetFunctionFinishHandle( void * pFunction ) { pfnFinished = (DWORD)pFunction; }

	char						* GetName() { return szName; }
	char						* GetDescription() { return szDescription; }

	int							GetID() { return iID; }
	std::vector<EMapID>			& GetMapIDs() { return vMapIDs; }
	int							GetAreaType() { return iAreaType; }
	int							GetType() { return iType; }
	int							GetLevel() { return iLevel; }

	BOOL						IsPVP() { return bPvP; }

	int							GetXMin() { return iXMin; }
	int							GetXMax() { return iXMax; }
	int							GetZMin() { return iZMin; }
	int							GetZMax() { return iZMax; }
	int							GetRadius() { return iRadius; }

	BOOL						IsKillsFinished() { return bKillsFinished; }
	BOOL						IsItemsFinished() { return bItemsFinished; }
	BOOL						IsFinished() { return (bKillsFinished && bItemsFinished); }
	BOOL						CanFinishQuestWaitTime();

	BOOL						IsInArea( int iMapID, int iX, int iZ );
	
	int							GetMonsterID( int i );
	int							GetMonsterCounter( int i );

	int							GetMonsterCount();
	int							GetItemCount();

	int							* GetMonsterKilledData() { return iaMonsterKilled; }
	int							* GetItemsCollectData() { return iaItemsCollected; }

	int							GetMonsterKilledAmount( int i );
	char						* GetMonsterName( int i );
	char						* GetItemName( int i );
	int							GetItemID( int i );
	int							GetItemCounter( int i );
	int							GetItemsCollected( int i );

	BOOL						IsInParty() { return ((iParty == 1) || (iParty == 2) || (iParty == 3) || (iParty == 4)) ? TRUE : FALSE; }
	BOOL						IsInRaid() { return ((iParty == 3) || (iParty == 4)) ? TRUE : FALSE; }

	int							GetPartyType() { return iParty; }

	BOOL						IsMultiple() { return bMultiple; }

	BOOL						IsTimeQuest() { return bTime; }
	DWORD						GetTimeQuest() { return dwTimeLeft; }
	DWORD						GetWaitTime() { return dwWaitingTime; }

	BOOL						AddMonsterKill( int i );
	BOOL						AddItemCollected( int i );

	void						SetID( int i ) { iID = i; }
	void						AddMapID( int i ) { vMapIDs.push_back( (EMapID)i ); }

	BOOL						Update();
	void						Update( BOOL b ) { bUpdate = b; };

	//Functions
	BOOL						HandleKill( int iTargetID, int iMapID, int iX, int iZ );

	BOOL						HandleCollectedItem( int iItemID, int iMapID, int iX, int iZ );

	void						Update( float fTime );

	UINT						GetQuestValidate();
	UINT						GetCurrentValidateQuestCode() { return uQuestValidateID; }
	void						ValidateQuest() { uValidateWheel = (rand() % 9000) + 1000; uQuestValidateID = GetQuestValidate(); };

	bool						IsValidQuest() { return uQuestValidateID == GetQuestValidate(); }


};

class Quest
{
public:

	Quest();
	virtual ~Quest();

	std::vector<QuestData*>		& GetData() { return vData; }
	QuestData*					GetQuest( int iID );

	BOOL						IsActiveQuest( int iID );

	BOOL						IsFinishedQuestKills( int iID );
	BOOL						IsFinishedQuestItems( int iID );

	BOOL						AddQuest( QuestData * psQuestData );

	BOOL						AddFinishedQuest( PacketHandleFinishedQuest * psPacket );

	BOOL						IsDoneQuest( int iQuestID );

	BOOL						HandleKill( int iTargetID, int iMapID, int iX, int iZ, BOOL bParty, BOOL bRaid, BOOL bPvP );

	BOOL						HandleCollectedItem( int iItemID, int iMapID, int iX, int iZ, BOOL bParty, BOOL bRaid );

	BOOL						DeleteQuest( int iID );
	void						DeleteAllQuests();

	int							GetRewardValue( int iRewardType );

private:
	std::vector<QuestData*>		vData;

	std::vector<QuestFinishedData*> vFinishedData;
};