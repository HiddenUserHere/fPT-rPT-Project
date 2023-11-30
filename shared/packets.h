#include <Windows.h>
#include "premiumstruct.h"


#define MAX_PKTSIZ				0x4000
#define PKTHDR_ENCMAX			0x10

#define PKTLENBFRHDR			4
#define PKTLEN					WORD
#define PKTENC					BYTE
#define PKTCRC					BYTE
#define PKTHDR					DWORD

#define SERVER_LEVEL_MAX				155

#define DISTANCE_UNIT_FAR				0x12C000
#define DISTANCE_UNIT_VIEWLIMIT			(64*64)
#define DISTANCE_UNIT_NEAR				((10*64)*(10*64))
#define DISTANCE_UNIT_PARTY_MAX			((17*64))

#define MAX_UNIT_PLAYDATA				75

#define MAX_QUESTPACKETDATA			15

enum EItemID;

enum EAccountShare;

struct HPStruct
{
	short								sMin;
	short								sMax;
};

struct StructFuryArenaBoss
{
	int      iCurrentBossHP;
	int      iMaxBossHP;
};

//                              Enumeratos
enum PacketsHeader
{
	//Net Server
	PKTHDR_NetIdentifier							= 0x7F000001,
	PKTHDR_NetExp									= 0x7F000002,
	PKTHDR_NetGetExp								= 0x7F000003,
	PKTHDR_NetGameMaster							= 0x7F000004,
	PKTHDR_NetGetGameMaster							= 0x7F000005,
	PKTHDR_NetClan									= 0x7F000006,
	PKTHDR_NetDisconnectUser						= 0x7F000007,
	PKTHDR_NetForceOrb								= 0x7F000008,
	PKTHDR_NetVersion								= 0x7F000009,
	PKTHDR_NetCharacterSync							= 0x7F00000A,
	PKTHDR_NetGiveExp								= 0x7F00000B,
	PKTHDR_NetPingPong								= 0x7F00000C,
	PKTHDR_NetBCStatusReq							= 0x7F00000D,
	PKTHDR_NetBCStatusSnd							= 0x7F00000E,
	PKTHDR_NetBCNewOwnerClanID						= 0x7F00000F,
	PKTHDR_NetBotAdd								= 0x7F000010,
	PKTHDR_NetPremiumEXP							= 0x7F000011,
	PKTHDR_NetUnloadUser							= 0x7F000012,
	PKTHDR_NetEXPEvent								= 0x7F000013,
	PKTHDR_NetTradeCoin								= 0x7F000014,
	PKTHDR_NetUsersOnline							= 0x7F000015,
	PKTHDR_NetQuestT5Data							= 0x7F000016,
	PKTHDR_NetQuestUpdateData						= 0x7F000017,
	PKTHDR_NetHardwareID							= 0x7F000018,
	PKTHDR_NetActionField							= 0x7F000019,
	PKTHDR_NetPvPBuff								= 0x7F000020,
	PKTHDR_NetPvPHandle								= 0x7F000021,
	PKTHDR_NetBellatraRequest						= 0x7F000022,
	PKTHDR_NetBattleRoyaleStatus					= 0x7F000023,
	PKTHDR_NetBellatraGiveGold						= 0x7F000024,
	PKTHDR_NetBattleRoyaleEnter						= 0x7F000025,
	PKTHDR_NetActionFieldRequest					= 0x7F000026,
	PKTHDR_NetUpdateEventMessage					= 0x7F000027,
	PKTHDR_NetResetPvPLogin							= 0x7F000028,
	PKTHDR_NetUpdatePvPGameServer					= 0x7F000029,
	PKTHDR_NetGetItem								= 0x7F00002A,
	PKTHDR_NetQuestEXPFreeEvent						= 0x7F00002B,

	//Packets Item Data
	PKTHDR_PerfectItemData							= 0x5A320002,
	PKTHDR_MixDescription							= 0x5A320001,
	PKTHDR_RecvWarehouse							= 0x48470048,
	PKTHDR_SendWarehouse							= 0x48470047,
	PKTHDR_TradeData								= 0x48470041,
	PKTHDR_TradeRequest								= 0x48470042,
	PKTHDR_TradeItems								= 0x48470043, //Trade Time...( when trade your inventory item to player... )
	PKTHDR_ThrowItem								= 0x48470053,
	PKTHDR_PlaceItem								= 0x4847005A,
	PKTHDR_CheckItemUseSlot							= 0x48478930,
	PKTHDR_CheckItemUseSlotEx						= 0x48478931,
	PKTHDR_ItemShopList								= 0x48470054,

	PKTHDR_KeySet									= 0x4847FFFF,
	PKTHDR_Crash									= 0x4847FFFE,
	PKTHDR_CrashData								= 0x4847FFFD,

	//Packets Character Data
	PKTHDR_PacketFun								= 0x43550001,
	PKTHDR_SetExp									= 0x43550002,
	PKTHDR_AttackRatingSend							= 0x43550003,
	PKTHDR_DamageDebugInfo							= 0x43550004,
	PKTHDR_ServerInfo								= 0x43550005,
	PKTHDR_PlayDataEx								= 0x43550006,
	PKTHDR_PremiumData								= 0x43550007,
	PKTHDR_SetItemTimer								= 0x43550008,
	PKTHDR_DisableGM								= 0x43550009,
	PKTHDR_QuestData								= 0x4355000A,
	PKTHDR_UpdateQuestData							= 0x4355000B,
	PKTHDR_FinishQuest								= 0x4355000C,
	PKTHDR_RestartQuest								= 0x4355000D,
	PKTHDR_BellatraCrown							= 0x4355000E,
	PKTHDR_PVPDataUpdate							= 0x4355000F,
	PKTHDR_UpdateSWPhase							= 0x43550010,
	PKTHDR_CancelQuest								= 0x43550011,
	PKTHDR_CrystalUse								= 0x43550012,
	PKTHDR_CrystalUseDie							= 0x43550013,
	PKTHDR_UserOnline								= 0x43550014,
	PKTHDR_CancelForceOrb							= 0x43550015,
	PKTHDR_FireMeteorite							= 0x43550016,
	PKTHDR_NPCEasterEgg								= 0x43550017,
	PKTHDR_PvPKill									= 0x43550018,
	PKTHDR_NPCIhinRewardRequest						= 0x43550019,
	PKTHDR_CharacterHeadOldUse						= 0x4355001A,
	PKTHDR_BlessCastleIndividualScore				= 0x4355001B,
	PKTHDR_BlessCastleUserScoreUpdate				= 0x4355001C,
	PKTHDR_NPCRudolphXmas							= 0x4355001D,
	PKTHDR_CancelItemTimer							= 0x4355001E,
	PKTHDR_Achievement								= 0x4355001F,
	PKTHDR_AchievementUser							= 0x43550020,
	PKTHDR_ActionFieldEnter							= 0x43550021,
	PKTHDR_ActionFieldFail							= 0x43550022,
	PKTHDR_ActionFieldUpdate						= 0x43550023,
	PKTHDR_ActionFieldExchange						= 0x43550024,
	PKTHDR_ActionFieldNPCExchange					= 0x43550025,
	PKTHDR_ActionFieldNPCEnter						= 0x43550026,
	PKTHDR_ActionFieldStart							= 0x43550027,
	PKTHDR_ActionFieldEnd							= 0x43550028,
	PKTHDR_NPCCaravanOpen							= 0x43550029,
	PKTHDR_NPCCaravan								= 0x4355002A,
	PKTHDR_NPCCaravanUpdate							= 0x4355002B,
	PKTHDR_NPCBeginnerItemTimer						= 0x4355002C,
	PKTHDR_XmasTreeOpen								= 0x4355002D,
	PKTHDR_XmasTree									= 0x4355002E,
	PKTHDR_XmasTreeNPCGift							= 0x4355002F,
	PKTHDR_XmasTreeNPCGiftOpen						= 0x43550030,
	PKTHDR_XmasTreeNPCResponse						= 0x43550031,
	PKTHDR_XmasTreeItemUse							= 0x43550032,
	PKTHDR_IceMeteorite								= 0x43550033,
	PKTHDR_EventsData								= 0x43550034,
	PKTHDR_GlamorousBrush							= 0x43550035,
	PKTHDR_DropItemAreaUpdate						= 0x43550036,
	PKTHDR_CustomQuestUpdate						= 0x43550037,
	PKTHDR_CarnivalNPC								= 0x43550038,
	PKTHDR_HonorUpdate								= 0x43550039,
	PKTHDR_ChemistHatItemTimer						= 0x4355003A,
	PKTHDR_RankingLevelRequest						= 0x4355003B,
	PKTHDR_RankingLevel								= 0x4355003C,
	PKTHDR_MountsList								= 0x4355003D,
	PKTHDR_MountDelete								= 0x4355003E,
	PKTHDR_MountNew									= 0x4355003F,


	//Packets Protect
	PKTHDR_LogCheat									= 0x6F6A0001,
	PKTHDR_DCAccount								= 0x6A6A0001,
	PKTHDR_ReconnectedServer						= 0x6A6A0002,
	PKTHDR_SendToLoginServer						= 0x6A6A0003,
	PKTHDR_Microphone								= 0x6A6A0004,
	PKTHDR_LogDataSimple							= 0x6A6A0005,
	PKTHDR_CheckSum									= 0x6A6A0006,
	PKTHDR_LogDebugger								= 0x6A6A0008,
	PKTHDR_UndentifiedPacket						= 0x6A6A0007,
	PKTHDR_AddExp									= 0x6A6A0009,
	PKTHDR_LoseExp									= 0x6A6A000A,
	PKTHDR_GetItemData								= 0x6A6A000B,
	PKTHDR_SetItemData								= 0x6A6A000C,
	PKTHDR_SetGold									= 0x6A6A000D,
	PKTHDR_WindowList								= 0x6A6A000E,
	PKTHDR_SettingsData								= 0x6A6A000F,
	PKTHDR_RetryWarehouse							= 0x6A6A0010,
	PKTHDR_WarehouseSaveSuccess						= 0x6A6A0011,
	PKTHDR_DamageQuick								= 0x6A6A0012,
	PKTHDR_QuestNPCOpen								= 0x6A6A0013,
	PKTHDR_QuestNPCAccept							= 0x6A6A0014,
	PKTHDR_QuestStartData							= 0x6A6A0015,
	PKTHDR_QuestUpdateData							= 0x6A6A0016,
	PKTHDR_QuestCancel								= 0x6A6A0017,
	PKTHDR_QuestNPCFinish							= 0x6A6A0018,
	PKTHDR_QuestNPCItemList							= 0x6A6A0019,
	PKTHDR_QuestHandleFinished						= 0x6A6A001A,
	PKTHDR_CharacterDataEx							= 0x6A6A001B,
	PKTHDR_ItemSlotHandle							= 0x6A6A001C,
	PKTHDR_T5QuestArena								= 0x6A6A001D,
	PKTHDR_DynamicQuestOpen							= 0x6A6A001F,
	PKTHDR_DynamicQuestOpenNPC						= 0x6A6A0020,
	PKTHDR_QuestItemSwap							= 0x6A6A0027,
	PKTHDR_BossTimeUpdate							= 0x6A6A0028,
	PKTHDR_CharacterDataSyncExGM					= 0x6A6A0031,
	PKTHDR_BattleRoyaleUpdate						= 0x6A6A0032,
	PKTHDR_BattleRoyaleDeath						= 0x6A6A0033,
	PKTHDR_BattleRoyaleBattleTime					= 0x6A6A0034,
	PKTHDR_BattleRoyaleRequest						= 0x6A6A0035,
	PKTHDR_BattleRoyaleList							= 0x6A6A0036,
	PKTHDR_BattleRoyaleRewardList					= 0x6A6A0037,
	PKTHDR_BattleRoyaleResponse						= 0x6A6A0038,
	PKTHDR_DeleteItemData							= 0x6A6A0039,
	PKTHDR_SkillTable								= 0x6A6A003A,
	PKTHDR_BattleRoyaleTeleports					= 0x6A6A003B,
	
	//Clan
	PKTHDR_GetClanInfo								= 0x64640001,
	PKTHDR_ClanInfoData								= 0x64640002,

	//Party
	PKTHDR_RequestParty								= 0x6A6A001E,
	PKTHDR_JoinParty								= 0x6A6A001F,
	PKTHDR_UpdateParty								= 0x6A6A0020,
	PKTHDR_UpdatePartyData							= 0x6A6A0021,
	PKTHDR_ActionParty								= 0x6A6A0022,
	PKTHDR_RequestRaid								= 0x6A6A0023,
	PKTHDR_JoinRaid									= 0x6A6A0024,
	PTKHDR_PartyCore								= 0x6A6A002A,

	//Coin Shop
	PKTHDR_CoinShopNPC								= 0x48476A01,
	PKTHDR_CoinShopAmount							= 0x48476A02,
	PKTHDR_CoinShopOpenTab							= 0x48476A03,
	PKTHDR_CoinShopTab								= 0x48476A04,
	PKTHDR_CoinShopOpenItemData						= 0x48476A05,
	PKTHDR_CoinShopItemData							= 0x48476A06,
	PKTHDR_CoinShopBuy								= 0x48476A07,
	PKTHDR_OpenCoinShop								= 0x48476A08,
	PKTHDR_CoinServiceOpen							= 0x48476A09,
	PKTHDR_CoinServiceBuy							= 0x48476A0A,
	PKTHDR_CoinServiceAgingRecoveryOpen				= 0x48476A0B,
	PKTHDR_CoinTraded								= 0x48476A0C,
	PKTHDR_CoinTradeSet								= 0x48476A0D,

	//Packets Event
	PKTHDR_KillsInSod								= 0x503220B1,
	PKTHDR_AgingFreeEvent							= 0x50326020,

	//Packets Sync Data
	PKTHDR_ChatGame									= 0x48471001,
	PKTHDR_ChatMessage								= 0x48471005,
	PKTHDR_ChatItemLink								= 0x48471007,
	PKTHDR_OpenClanMaster							= 0x48478A00,
	PKTHDR_PersonalShopMessage						= 0x48478AC0,
	PKTHDR_SyncData1								= 0x4847008B,
	PKTHDR_SyncData2								= 0x4847008E,
	PKTHDR_CharacterData							= 0x48470020,
	PKTHDR_Ping										= 0x435A0007,
	PKTHDR_PingLoginServer							= 0x435A0008,
	PKTHDR_FuryArena								= 0x435A0009,
	PKTHDR_FuryArenaItemData						= 0x435A000A,
	PKTHDR_FuryArenaBossData						= 0x435A000B,
	PKTHDR_NPCLarryOpen								= 0x435A000C,
	PKTHDR_SkillStatusContainer						= 0x435A000D,
	PKTHDR_SkillStatus								= 0x435A000E,
	PKTHDR_UseSkillNew								= 0x435A000F,
	PKTHDR_NPCMarinaOpen							= 0x435A0010,
	PKTHDR_SaveAndClose								= 0x435A0011,
	PKTHDR_PhoenixPet								= 0x484700B0,
	PKTHDR_Warehouse								= 0x48470047,
	PKTHDR_NPCBellatra								= 0x4847004A,
	PKTHDR_RequestBellatra							= 0x4847004B,
	PKTHDR_Save										= 0x484700E8,
	PKTHDR_Disconnect								= 0x484700E6,
	PKTHDR_Reconnect								= 0x48478010,
	PKTHDR_ReconnectLogin							= 0x48478011,
	PKTHDR_UpdateLevelClan							= 0x48470038,
	PKTHDR_ChatMessageBox							= 0x48478100,
	PKTHDR_PlayDataChar								= 0x48470013,
	PKTHDR_UnitStatusContainer						= 0x48470014,
	PKTHDR_GameStatus								= 0x48470018,
	PKTHDR_CharacterDataShort						= 0x4847004C,
	PKTHDR_BellatraRoomScore						= 0x50320901,
	PKTHDR_KillCharacter							= 0x50320E00,
	PKTHDR_RegenChar								= 0x50320B00,
	PKTHDR_UpdatePetData							= 0x50320A40,
	PKTHDR_UpdatePetDataEx							= 0x6A6A0030,
	PKTHDR_DamageData								= 0x48470030,
	PKTHDR_GoldBellatraClan							= 0x48478A18,
	PKTHDR_GetUnitInfoData							= 0x48470021,
	PKTHDR_LastAttackDamage							= 0x50322030,
	PKTHDR_SingleDamage								= 0x50322000,
	PKTHDR_RangeDamage								= 0x50322010,
	PKTHDR_DamageResult								= 0x50322020,
	PKTHDR_BuyItemNPCShop							= 0x50320210,
	PKTHDR_NewPersonalShop							= 0x48478A90,
	PKTHDR_PersonalShopRequestView					= 0x48478AA0,
	PKTHDR_PersonalShopItemList						= 0x48478AB0,
	PKTHDR_PersonalShopMessageUpdate				= 0x48478AC0,
	PKTHDR_PersonalShopTradeItems					= 0x48478AD0,

	//Packets Login
	PKTHDR_ServerList								= 0x484700C0,
	PKTHDR_SaveData									= 0x48470081,
	PKTHDR_SaveDataFail								= 0x48470083,
	PKTHDR_SelectCharacter							= 0x48470082,
	PKTHDR_SaveSuccess								= 0x48470084,
	PKTHDR_UserInfo									= 0x48470086,
	PKTHDR_CreateCharacter							= 0x48470088,
	PKTHDR_DeleteCharacter							= 0x48470087,
	PKTHDR_Version									= 0x4847008A,
	PKTHDR_AccountLoginCode							= 0x48470023,

	PKTHDR_LoginUser								= 0x4848FFFF,

	//Packet Skills
	PKTHDR_BuffSkillsParty							= 0x50320A00,
	PKTHDR_CancelDrasticSpirit						= 0x50320A11,
	PKTHDR_CancelSkill								= 0x50320A10,
	PKTHDR_SkillCast								= 0x50320A20,
	PKTHDR_SkillCastData							= 0x50320A30,
	PKTHDR_EvasionSuccess							= 0x50320A50,
	PKTHDR_PartyUsePot								= 0x484700D0,
	PKTHDR_HolyMind									= 0x484700D3,
	PKTHDR_Wisp										= 0x484700C3,

	//Packets Unit
	PKTHDR_OpenNPC									= 0x48470024,
	PKTHDR_NpcData									= 0x48470070,
	PKTHDR_SetExpOld								= 0x48470031,
	PKTHDR_TeleportEvent							= 0x4847005D,
	PKTHDR_UnitStatusMove							= 0x48470010,
	PKTHDR_PlayData									= 0x48470013,
	PKTHDR_UnitInfoQuick							= 0x4847004C,
	PTKHDR_OpenEventGirl							= 0x4847004F,

	//Packets Map
	PKTHDR_WarpGateField							= 0x48470040,
	PKTHDR_DeleteFlagPoint							= 0x48470061,
	PKTHDR_TheDeath									= 0x48478800,
	PKTHDR_PlayItem									= 0x48470050,
	PKTHDR_UseUnionCore								= 0x50320206,
	PKTHDR_BellatraInformation						= 0x503220B0,
	PKTHDR_EffectSpawn								= 0x4847005C,

	//Packets Item
	PKTHDR_SocketItemDataToServer					= 0x48470025,
	PKTHDR_SocketItemDataToClient					= 0x48470026,
	PKTHDR_DeleteItem								= 0x48470051,
	PKTHDR_NewItem									= 0x48470052,
	PKTHDR_UpgradeAgingItem							= 0x50320204,
	PKTHDR_AgingItem								= 0x50320202,
	PKTHDR_OpenSmelting								= 0x50325000,
	PKTHDR_OpenResetItem							= 0x484700A6,
	PKTHDR_OpenManufacture							= 0x50326000,
	PKTHDR_ManufactureItem							= 0x50326001,
	PKTHDR_MixItem									= 0x50320200,
	PKTHDR_ResetItem								= 0x50326006,
	PKTHDR_NPCItemBox								= 0x48478A80,
	PKTHDR_NPCItemBoxSend							= 0x48478A8A,
	PKTHDR_NPCItemBoxSendResponse					= 0x48478A8B,
	PKTHDR_NPCItemBoxOpen							= 0x48478A8C,
	PKTHDR_RollDiceDrop								= 0x6A6A0025,
	PKTHDR_RollDiceRanking							= 0x6A6A0026,
	PKTHDR_IntegrityUpdateItem						= 0x6A6A0029,
	PKTHDR_YurinaSkipQuests							= 0x6A6A002B,

	//Packets Buff
	PKTHDR_StunBuff									= 0x48478004,
	PKTHDR_IceDistortionBuff						= 0x48478008,
};

struct Packet //Header
{
	PKTLEN	  iLength;				//Packet Length
	PKTENC	  iEncKeyIndex = 0;		//Packet Encryption Key Index
	PKTENC	  iEncrypted = 0;		//Packet Encrypted
	PKTHDR	  iHeader;				//Packet Header
}; //Size = 0x08

struct PacketSettingsData : Packet
{
	BOOL									bNoTradeChat;
	BOOL									bNoPartyRequest;
	BOOL									bNoTradeRequest;
	BOOL									bNoWhisper;

	BOOL									bHideCaravans;

	BOOL									cPadding001[0x10];
};

struct PacketCrash : Packet
{
	DWORD									dwExceptionCode;
	DWORD									dwExceptionFlags;
	PVOID									pvExceptionAddress;

	DWORD									EAX;
	DWORD									ECX;
	DWORD									EDX;
	DWORD									EBX;
	DWORD									ESP;
	DWORD									EBP;
	DWORD									ESI;
	DWORD									EDI;
	DWORD									EIP;

	DWORD									dwVersion;

	char									szAccountName[32];
};

#define CRASHDATA_SIZE 0x1F00

struct PacketCrashData : Packet
{
	BOOL									bFirst;
	char									szAccountName[32];
	int										iDataLen;
	DWORD									dwTick;
	BYTE									baData[CRASHDATA_SIZE];
};

// Save
struct	PacketSaveData : Packet
{
	int										iCount;
	int										iTotal;
	int										iDataSize;

	char									szData[8160];
};

struct PacketSaveSuccess : Packet
{
	int				iUnknown;
	BOOL			bSaved;
	int				iBlessCastleTax;
	int				iBlessCastleClanID;
};

// Effects
struct PacketFireMeteorite : Packet
{
	Point3D									sBegin;
	Point3D									sEnd;
	int										iDelay;
	BOOL									bSmallMeteor;
	int										iCount;
};

struct PacketIceMeteorite : Packet
{
	Point3D									sBegin;
	Point3D									sEnd;
	int										iDelay;
	BOOL									bSmall;
	UINT									uMeteoriteCount;
};

// Packets TGF
struct PacketFuryArenaBossData : Packet
{
	StructFuryArenaBoss						sHP;
};

#define EASTER_ITEMS_MAX			3

struct PacketNPCEasterEgg : Packet
{
	enum EAction
	{
		ACTION_None,
		ACTION_GetReward,
	};

	int										iAction;

	int										iaItemID[EASTER_ITEMS_MAX];
	DWORD									dwaHead[EASTER_ITEMS_MAX];
	DWORD									dwaCheckSum[EASTER_ITEMS_MAX];
};

#define CARNIVAL_ITEMS_MAX			8

struct PacketNPCCarnival : Packet
{
	bool									bDeleteItems;

	int										iCount;
	int										iaItemID[CARNIVAL_ITEMS_MAX];
	int										iaChk1[CARNIVAL_ITEMS_MAX];
	int										iaChk2[CARNIVAL_ITEMS_MAX];
};



#define PUZZLE_ITEMS_MAX	3

struct PacketNPCPuzzle : Packet
{
	int										iaItemID[EASTER_ITEMS_MAX];
	DWORD									dwaHead[EASTER_ITEMS_MAX];
	DWORD									dwaCheckSum[EASTER_ITEMS_MAX];
};

struct PacketNPCRudolphXmas : Packet
{

	int										iItemID;
	DWORD									dwHead;
	DWORD									dwCheckSum;
};


struct PacketDamageQuick : Packet
{
	BOOL											bPercentile;
	BOOL											bBaseCurrentHP;
	int												iDamage;

	BOOL											bDie = TRUE;
};


struct PacketStageItemData : Packet
{
	struct StageItemData
	{
		BOOL								bActive;
		int									iType;
		int									iItemID;
		int									iX;
		int									iY;
		int									iZ;
		char								szName[32];
		int									iClass;
	};
	BOOL									bActive;
	int										iCount;
	StageItemData							saItems[50];
};

// Party
struct PartyUserData
{
	char									szName[32];
	char									szModel[64];
	char									szHead[64];

	DWORD									dwObjectID;
	int										iLevel;
	CurMax									sHP;
	int										iJobCode;
	int										iGold;
	int										iX;
	int										iZ;
};
struct PartyUserInfo
{
	DWORD									dwObjectID;
	int										iLevel;
	CurMax									sHP;
	int										iJobCode;
	int										iGold;
	int										iX;
	int										iZ;
};

struct PacketPartyPlayInfo : Packet
{
	DWORD									dwObjectID;
	DWORD									dwTemp;

	int										iUsersCount;
	PartyUserInfo							sData[8];

	struct
	{
		CurMax								sHP;
		CurMax								sMP;
		CurMax								sSP;
	} New[8];
};
struct PacketPartyUsers : Packet
{
	DWORD									dwObjectID;
	DWORD									dwTemp[8];

	int										iUsersCount;
	PartyUserData							sData[8];

	struct
	{
		CurMax								sHP;
		CurMax								sMP;
		CurMax								sSP;
	} New[8];
};

struct PacketPartyPotion : Packet
{
	int										iMP;
	int										iHP;
	int										iSTM;
	int										iUnk;
};

//			LoginServer
struct _sTRANS_SERVER_INFO
{
	char									szServerName[ 0x20 ]; // 0x0
	char									szServerIp1[ 0x14 ]; // 0x20
	char									szServerIp2[ 0x14 ]; // 0x34
	char									szServerIp3[ 0x14 ]; // 0x48
	DWORD									dwServerPort1; // 0x5C
	DWORD									dwServerPort2; // 0x60
	DWORD									dwServerPort3; // 0x64
	DWORD									dwUnknown; // 0x64

};

struct PacketLoginChannel : Packet
{
	char									szServerName[ 0x10 ]; // 0x8
	DWORD									dwServerTime; // 0x18
	int										iClanTicket; // 0x1C
	DWORD									dwTemp; // 0x20
	int										iClanServerCount; // 0x24
	int										iServerCount; // 0x28
	_sTRANS_SERVER_INFO						ServerInfo[ 0x4 ]; // 0x2C

};

struct PacketServerList : Packet
{
	struct Header
	{
		char				szServerName[16];
		DWORD				dwTime;
		int					iTicket;
		DWORD				dwUnknown;
		int					iClanServerIndex;
		int					iGameServers;
	};

	struct Server
	{
		char				szName[32];
		char				szaIP[3][20];
		int					iaPort[4];
	};

	Header					sHeader;
	Server					sServers[4];

	EAccountShare			eAccountShare;
};

struct PacketTradeData : Packet
{
	int										iSenderID;
	int										iReceiverID;

	DWORD									dwCheckSum;

	DWORD									dwaTempData[4];

	BYTE									baData[4000];

};

struct PacketTradeSuccess : Packet
{
	int										iSenderID;
	int										iReceiverID;

	DWORD									dwCheckSum;

	DWORD									dwaTempData[4];

	BYTE									baData[4000];
};

struct PacketTradeRequest : Packet
{
	int										iType;

	int										iUnitID;

	int										iID;
};

struct PacketClanGoldReceive : Packet
{
	int										iGoldSend;
	int										iGold;
};

struct PacketReceiveItemIhin : Packet
{
	int										iCount; // Count + 80 : Security
};

struct PacketBellatraRequest : Packet
{
	int				iUnsaveGold;
	int				iGold;
};

struct PacketLoginUser : Packet
{

	DWORD									dwUnk[3];

	char									szUserID[32];
	char									szPassword[32];
	char									szMacAddr[20];
	char									szPCName[32];
	DWORD									dwSerialHD;

	UINT									uUniqueFolderID;

	char									szVideoName[256];
	UINT									uUniqueVideoID;

	char									szHardwareID[40];

	UINT									uWidthScreen;
	UINT									uHeightScreen;

	int										iSystemOS;

	int										iGameVersion;

	UINT									uGameChecksum;
	UINT									uDLLChecksum;
};

struct PacketSyncTimeData : Packet
{
	DWORD									dwNetCount;
	DWORD									dwTime;
	int										iUnk;
	int										iUnk1;
};

struct PacketUserWarpEffect : Packet
{
	int										iX;
	int										iID;
	int										iY;
	int										iZ;
};

struct PacketSendUseUnionCore : Packet
{
	int										iY;
	int										iX;
	int										iZ;
	int										iMapID;

	DWORD									dwItemHead;
	DWORD									dwItemID;
	DWORD									dwItemChecksum;
	DWORD									dwUnk;
};


struct PacketPersonalShopMessageEffect : Packet
{
	DWORD									dwMSGCode;

	int										ColorBlink = 0;
	short									sColors[4];
	DWORD									DispEffect = 0;
	DWORD									BlinkScale = 0;
};

struct PacketDebugDamageInfo : Packet
{

	DWORD									dwObjectID;
	int										iTypeAction;
	int										iValue;

//	int										iElementType;
};

struct PacketReconnectUser : Packet
{
	int										iUnk;
	int										iID;
	char									szUserID[32];
};

struct PacketEvasion : Packet
{
	int										iSkillID;
	int										iUnk;
	DWORD									dwObjectID;
};

struct CharacterFile : Packet
{

	char									szHeader[8];

	void									* sCharInfo;
};

#define _XMIN_FURYARENA		-1050946
#define _XMAX_FURYARENA		-806751
#define _ZMIN_FURYARENA		-11170306
#define _ZMAX_FURYARENA		-10928372

enum EFuryArenaState
{
	FURYARENASTATEID_OpenNPC,
	FURYARENASTATEID_RequestEnterArena,
	FURYARENASTATEID_Enter,
	FURYARENASTATEID_TeleportCT2,
	FURYARENASTATEID_OpenNPCEntrance,
	FURYARENASTATEID_Status,
	FURYARENASTATEID_Exchange,
	FURYARENASTATEID_EXP,
	FURYARENASTATEID_Gold,

};

struct PacketFuryArenaRequest : Packet
{
	EFuryArenaState							eState;
	int										iEventID;
	DWORD									dwTimeLeft;
	int										p1;
	int										p2;
	int										p3;
	int										p4;
	int										p5;

	int										iLevel;
};

struct SPrizeData
{
	BOOL										bActive;
	char										szCode[32];
	int											iQuantity;
	int											iPercent;
};

struct PacketBuffData : Packet
{
	union
	{
		struct SIce
		{
			int				iStunCount;
			int				iIceOverlay;
			int				iaUnk[2];
			int				iDistortion;
			int				iCurseLazy;
		} Ice;
	};
};

struct PlayBuffer
{
	int										iX;
	int										iY;
	int										iZ;
	DWORD									dwFrame;
	short									saAngle[4];
	short									sStickItems[4];
	DWORD									dwTargetSerial;
};

struct PlayBufferData
{
	int										iCount;
	PlayBuffer								sPlayBuff;
};

struct PacketPlayDataMinor : Packet
{
	DWORD									dwObjectID;	
	int										iX;
	int										iY;
	int										iZ;
	short									saAngle[4];		
	DWORD									dwFrame;
	DWORD									dwAutoCharCode;		
};

struct PacketPlayData : Packet
{
	DWORD									dwObjectID;
	DWORD									dwTarget;

	int										iPlayBuffCount;
	int										iStartPosition;

	short									sHp[2];
	DWORD									dwAutoCharCode;
	short									sMapID;
	short									sUnk;
	BYTE									bUpdateInfo[4];
	BYTE									bEventInfo[4];

	CurMax									sMP;
	CurMax									sSP;

	bool									bPartyLeader;

	short									sSecondMapID;

	short									saSpeed[2];

	IMinMax									sHPLong;

	short									sAngleY;
	short									sSize;

	int										iLevel;

	short									sTitleID;
	short									sFPS;

	int										iLastX;
	int										iLastZ;

	BYTE									sLevelAgeArmor;

	bool									bDie;

	bool									bCaravan;

	bool									bPvPMode;

    int                                     iPvPKills;
    int                                     iPvPExperience;

	int										iMountType;

	BYTE									baPadding[0x10];
};

struct PacketPlayDataGroup : Packet
{
	int										iPlayDataCount;
	DWORD									dwSendCode;
};
struct PacketPlayDataGroupBot : Packet
{
	int										iPlayDataCount;
	DWORD									dwSendCode;
	PacketPlayData							sPlayData;
	PlayBufferData							sPlayBuffData;

	struct PacketItemShopBot : Packet
	{
		int									iCheckSum;
		int									iUnk[5];
	};

	PacketItemShopBot						sPacket;
};

struct PacketPlayDataGroupBot2 : Packet
{
	int										iPlayDataCount;
	DWORD									dwSendCode;
	PacketPlayDataMinor						sPlayDataMinor;
};

struct PacketCheatLog : Packet
{

	int										iUnk;
	int										iLogCode;

	int										iOldCode1;
	int										iOldCode2;

	int										iExUnk;
	int										iExLogCode;
	int										iCode1;
	int										iCode2;
};

struct PacketCommand : Packet
{
	int										p2;
	int										p1;
	int										p3;
	int										p4;
};

struct PacketCommandEx : Packet
{
	int										p2;
	int										p1;
	int										p3;
	int										p4;

	int										p6;
	int										p5;
	int										p7;
	int										p8;
};

struct PacketCreateCharacter : Packet
{
	int										iUnknown;
	int										iCode;
	int										iUnk;
	char									szUserID[ 32 ];
	char									szCharname[ 32 ];
};

struct PacketSelectCharacter : Packet
{

	int dwReserved1;
	int dwReserved2;
	int dwReserved3;
	char									szName[32];
};
struct PacketVersion : Packet
{

	BOOL									bServerFull;
	int										iVersion;
	int										iUnk2;
};

struct PacketBellatraCrown : Packet
{
	int										iBellatraCrown;
};

struct PacketDeleteCharacter : Packet
{
	int										iUnknown;
	int										iCode;
	int										iUnk;
	char									szUserID[ 32 ];
	char									szCharname[ 32 ];
};



#define SKILL_COUNT_CHARACTER 16

struct CharacterDataPacket
{
	int										iStrength;			//힘 
	int										iSpirit;			//정신력 
	int										iTalent;			//재능 
	int										iAgility;
	int										iHealth;			//건강 
	int										iAccuracy;			//상대적 명중률
	int										iAttackRating;	//명중률
	int										iMinDamage;	//타격수치 ( 최소 / 최대 )
	int										iMaxDamage;	//타격수치 ( 최소 / 최대 )
	int										iAttackSpeed;		//공격속도			
};

struct PacketCharacterDataSyncEx : Packet
{
	int										iID;

	CharacterDataPacket						sData;
};

struct PacketSetCharacterGold : Packet
{
	DWORD									dwGold;
};

struct PacketUpdateClanLevel : Packet
{
	DWORD									dwLevel;
	DWORD									dwCodeOP;
	DWORD									dwClanCode;
	DWORD									dwUpdateInfo;
};

struct PacketPlayDataEx : Packet
{
	int										iLevelSkill[ SKILL_COUNT_CHARACTER ];
	CharacterDataPacket						sCharacterData;
	int										iCritical;
	int										iLevelCharacter;
	BOOL									DebugInfo;
	BOOL									bGameMaster;
	char									szMacAddr[ 20 ];
	DWORD									dwServerID;
	DWORD									dwQuestLevelLog;
	DWORD									dwGold;
	DWORD									dwSpeed;
	int										iBellatraCrown;
	int										iMapID;
	UINT									uSerialHD;
	UINT									uChecksumModule;
};

struct ChecksumFunction
{
	DWORD										dwSize;
	DWORD										dwAddress;
	DWORD										dwCheckSum;
};

#define CHECKSUM_FUNCTION_TOTAL					400

struct PacketChecksumFunctionList : Packet
{
	BYTE										bKey;
	ChecksumFunction							sCheckSum[CHECKSUM_FUNCTION_TOTAL];
};

#define WINDOW_CHEATLIST_TOTAL					100

struct PacketWindowList : Packet
{
	int											iCount;
	char										szaWindowName[WINDOW_CHEATLIST_TOTAL][64];
};

struct PacketAttackRating : Packet
{
	int										iEnemyID;
	int										iAttackRating;
};

struct PacketWarpGateField : Packet
{
	int										iZ;
	int										iStage;
	int										iX;
};

struct PacketServerInfo : Packet
{
	int										iUsers;
	int										iMaxUsers;
	int										iBossTime;
	int										iServerVersion;
	short									sSiegeWarDay;
	short									sSiegeWarHour;
	BYTE									bSiegeWarType;
	BOOL									bGameMaster;
	SYSTEMTIME								sServerTime;
};

enum EPhoenixType
{
	PHOENIXTYPEID_None,
	PHOENIXTYPEID_Fire,
	PHOENIXTYPEID_Ice,
	PHOENIXTYPEID_Lightning,
	PHOENIXTYPEID_Healing,
	PHOENIXTYPEID_ElementHealing	= 2,
	PHOENIXTYPEID_DeleteMask		= 4855374,
};

struct PacketPhoenixPet : Packet
{
	int										iElementType;			// 2 = Healing
	EPhoenixType							eType;
	int										iUnk;
	DWORD									dwMask;					// 4855374 = Delete Phoenix
};

struct PacketDeleteFlagPoint : Packet
{
	int										iX;
	int										iZ;
};

struct PacketSetPremium : Packet
{
	DWORD									dwCode;
};

struct PacketExperience : Packet
{
	INT64									iAddExp;
};

struct PacketExperienceOld : Packet
{
	int										iMonsterID;
	int										WParam;
	int										SParam;
	int										EParam;

	int										iMapID;
};


struct PacketLoseExperience : Packet
{
	INT64									iSubExp;
	int										iLevel;
	int										iMapID;

	BOOL									bRessurectionItem;
};

struct PacketMixDescription : Packet
{
	char									szMixDescription[ 150 ];
};

struct PacketCrystalUse : Packet
{
	DWORD									dwObjectID;
	DWORD									dwItemID;
	DWORD									dwTime;
};

struct PacketCrystalUseDie : Packet
{
	DWORD									dwObjectID;
};

struct BlessCastleStatusShort
{
	DWORD									iClanID;
	int										iTax;
	int										iSkill;
	int										iaTowerType[6];
	int										iaGuardAmount[3];
};

struct BlessCastleStatusSettingsData
{
	int										iUnk[6];
	int										iTax;
	short									saTowerType[12];	//1 = ice, 2 = lightning, 3 = fire
	unsigned char							caGuardAmount[4];	//max = 20
	int										iSkill;				//1 = absorb, 2 = damage, 3 = evasion
	int										iPrice;
	DWORD									iClanID;
	DWORD									dwUnk[4];
};

#define MAX_TOP_DAMAGEDATA		10
#define MAX_DAMAGEDATA			100

enum ESiegeWarMode : int
{
	SIEGEWARMODE_Normal,
	SIEGEWARMODE_TheConquest,
	SIEGEWARMODE_TheMassacre,
};

struct PacketBlessCastleTopIndividual : Packet
{
	char				szCharacterName[3][32];

	int					iClass[3];

	int					iDamage[3];

	int					iMode;
};

struct PacketBlessCastleUserScoreUpdate : Packet
{
	int					iUserScore;

	int					iMode;
};

struct PacketBlessCastleTopData : Packet
{
	int					iUnk01;

	int					iID;
	DWORD				dwUnk02;

	Point3D				sPosition;

	DWORD				dwUnk03;
	DWORD				dwUpdateTime;
	DWORD				dwTotalDamage;

	BYTE				baCrystalTowerCount[4];
	DWORD				dwFree[2];

	union
	{
		int				iUserID[MAX_TOP_DAMAGEDATA];
		int				iClanID[MAX_TOP_DAMAGEDATA];
	};

	int					iDamage[MAX_TOP_DAMAGEDATA];
	int					iCounter[MAX_TOP_DAMAGEDATA];
};

struct BlessCastleStatusData
{
	int										iCastleMode;
	DWORD									iClanID;
	int										iDefenseLevel;
	DWORD									dwTimeLimit;
	int										iCounter;
	
	int										iTax;
	int										iTaxNow;
	
	DWORD									dwBattleOverTime;

	int										iFountainPotionCount;
	DWORD									dwFountainTime;
	DWORD									dwFountainLastUseTime;
	int										iFountainCountUsers;

	DWORD									dwStartTime;
	union
	{
		short								saTimeSec[2];

		int									iTimeSec;
	};
	int										iSkill;
	int										iUnk[10];
	BlessCastleStatusSettingsData			sSettings;
	PacketBlessCastleTopData				sPacketTopData;
	struct AttackDamageData					* psaDamageData;

	union
	{
		BYTE								baSoldierCount[4];

		int									iSoldierCount;
	};

	BYTE									baCrystalTowerCount[4];
	class UnitData							* pcUnitDataTower;
	DWORD									dwScoreTime;
};

struct BlessCastlePVPLeague
{
	int						iMode;
	DWORD					dwBeginTime;
	DWORD					dwEndTime;
	DWORD					dwRestartTime;
};

struct PacketNetBCStatus : Packet
{
	BlessCastleStatusShort					sBlessCastleStatus;
};

struct PacketBlessCastleStatus : Packet
{
	BlessCastleStatusShort					sBlessCastleStatus;
};

struct PacketSkillDamageNormal : Packet
{
	int		iTargetReceiveDamageID;	//공격하는 캐릭터 고유번호
	int		iID;	//공격대상 캐릭터 고유번호

	Point3D			sPosition;

	int iDamageState;	// 속성
	int iRange;		// 공격 범위
	int iDamage;			// 공격 파워
	DWORD	dwChkSum;

	short	sRating[ 2 ];	//[0]-레벨 [1]-명중력
	DWORD	dwDamageChkSum;
};

struct PacketHolyMind : Packet
{
	int										iTime;
	int										iAbsorbPercent;
	DWORD									dwObjectID;
};

struct PacketTeleportEvent : Packet
{
	int										iTeleportEventID;
};

struct PacketWisp : Packet
{
	int										iTime;
	int										iAbsorbPercent;
	DWORD									dwObjectID;
};

struct PacketSkillCancel : Packet
{
	int				iUnk;
	int				iPlayID;
	int				iUnk2;
	int				iUnk3;
};

struct PacketSkillDamageNormalEx : Packet
{
	DWORD									dwCheckSum;

	DWORD									iTargetReceiveDamageID;	
	int										iID;	

	Point3D									sPosition;

	int										iDamageState;	// 속성
	int										iRange;		// 공격 범위
	MinMax									sAttackPower;		// Attack power of character
	short									WeaponAttackPower[ 2 ];		// Attack power of weapon
	short									saStatus[ 2 ];	// iSpirit/critical/{...}
	int										iSkillID;		// 스킬코드
	DWORD									dwTime;		
	int										iDamageCount;
	short									MotionCount[ 2 ];	// 공격 모션 카운터
	DWORD									dwWeaponCode;	// 주 공격 무기
	short									Area[ 2 ];		// 지역
	int										Temp[ 3 ];		// 예비
	int										iAttackRating;
};

struct PacketSkillDamageRange : Packet
{
	int										iTargetReceiveDamageID;	//공격하는 캐릭터 고유번호

	Point3D									sPosition;

	int										iDamageState;	// 속성
	int										iRange;		// 공격 범위
	int										iDamage;			// 공격 파워
	DWORD									dwChkSum;

	int										iCount;
	int										iaTargetID[ 128 ];	//공격대상 캐릭터 고유번호
};


struct PacketSkillDamageRangeEx : Packet
{
	int										iTargetReceiveDamageID;
	
	DWORD									dwChkSum;

	Point3D									sPosition;

	int										iDamageState;	
	int										iRange;
	MinMax									sAttackPower;
	short									WeaponAttackPower[2];
	short									saStatus[2];	//iSpirit/critical/{...}
	int										iSkillID;
	DWORD									dwTime;		
	int										iDamageCount;
	short									MotionCount[ 2 ];
	DWORD									dwWeaponCode;
	
	short									sMapID;
	short									sUnka01;

	int										Temp[ 3 ];	
	int										iAttackRating;
	ID										iTargetID;

	int										iCount;
	int										iaTargetID[ 128 ];
};

struct PacketUserOnline : Packet
{
	int										iType;
	int										iID;
	BOOL									bOnline;
	char									szName[32];
};

struct PacketSimple : Packet
{
	int										iUnk;
	int										iUnk2;
	int										iUnk3;
	int										iUnk4;
};

struct PacketSimpleEx : Packet
{
	int										iUnk;
	int										iUnk2;
	int										iUnk3;
	int										iUnk4;

	int										iUnk5;
	int										iUnk6;
	int										iUnk7;
	int										iUnk8;
};

struct PacketCancelSkill : Packet
{
	int				iParam1;
	int				iID;
	int				iParam2;
};

struct PacketGetUnitData : Packet
{
	int										iUnknown;
	int										iID;
};

struct PacketEventGirl : Packet
{
	BOOL									bFreeGold;
};

struct PacketDeadCharacter : Packet
{
	int										iUnk;
	int										dwObjectID;
};

struct PacketAgingFreeEvent : Packet
{
	int				iReduceTax;
	BOOL			bFree;
};

struct PacketPvPData : Packet
{
	int										iPvPKills;
	int										iPvPDeaths;
	int										iPvPStreak;
    int                                     iPvPExperience;
	BOOL									bKill;
	int										iID;
};

enum EPvPKillType
{
	PVPKILLTYPE_None,
	PVPKILLTYPE_DoubleKill,
	PVPKILLTYPE_TripleKill,
	PVPKILLTYPE_QuadraKill,
	PVPKILLTYPE_PentaKill,
	PVPKILLTYPE_Shutdown,
	PVPKILLTYPE_Rampage,
	PVPKILLTYPE_Unstoppable,
	PVPKILLTYPE_COUNT,

};

struct PacketPvPKillType : Packet
{
	char				szKillerName[32];
	char				szVictimName[32];

	int					iKillerClass;
	int					iVictimClass;

	int					iKillType;
};

struct PacketHalloweenEventStatus : Packet
{
	BOOL									bStatus;
};

struct PacketUpdatePetData : Packet
{
	BOOL									bAutoAttack;
	int										dwSkillID;
};

struct PacketUpdatePetDataEx : Packet
{
	int										iTargetID;
	int										iLastTargetID;
};

struct PacketLogCheat : Packet
{
	int										LParam;
	int										iCheatID;
	int										SParam;
	int										EParam;

	int										LxParam;
	int										WxParam;
	int										SxParam;
	int										ExParam;

#if defined(_SERVER)
	char									szBuffer1[256] = { 0 };
	char									szBuffer2[32] = { 0 };
#endif
};

struct PacketLogCheatEx : Packet
{
	int										LParam;
	int										iCheatID;
	int										SParam;
	int										EParam;

	int										LxParam;
	int										WxParam;
	int										SxParam;
	int										ExParam;

	char									szParam[256];
};

//                      Siege War

struct PacketSiegeWarPhase : Packet
{
	int										iPhaseID;
};

enum ELogCheat
{
	CHEATLOGID_DelaySkillHack				= 52,
	CHEATLOGID_ValidateCharacterError		= 81,
	CHEATLOGID_CopiedItemError				= 99,
	CHEATLOGID_CopiedItem					= 1000,
	CHEATLOGID_CopiedItemWarehouse			= 1010,
	CHEATLOGID_WarehouseGold				= 1011,
	CHEATLOGID_WarehouseNewClone			= 1012,
	CHEATLOGID_WarehouseBug					= 1013,
	CHEATLOGID_HackDetected					= 1100,
	CHEATLOGID_FocusChanged					= 1101,

	CHEATLOGID_GoldLimit					= 1150,
	CHEATLOGID_SpeedHack					= 1200,
	CHEATLOGID_TimeErrorSpeedHack			= 1250,
	CHEATLOGID_TimeMismatch					= 1400,
	CHEATLOGID_AttackRatioError				= 1500,
	CHEATLOGID_PotionCheckError				= 1510,
	CHEATLOGID_PotionCountError				= 1530,

	CHEATLOGID_CharInfoSaveError			= 1600,
	CHEATLOGID_DefaultAttackRatingError		= 1820,
	CHEATLOGID_DefaultAttackSizeError		= 1821,
	CHEATLOGID_ContinuousAttackError		= 1823,
	CHEATLOGID_SkillAttackRatingError		= 1830,
	CHEATLOGID_SkillContinuousAttackError	= 1833,
	CHEATLOGID_RestrictedAreaTrespassed		= 1840,
	CHEATLOGID_WeightError					= 1900,

	CHEATLOGID_ItemError					= 1950,
	CHEATLOGID_CopiedItemRecall				= 1960,
	CHEATLOGID_ForcedPenaltyBoot			= 2000,
	CHEATLOGID_EditLevelError				= 2300,
	CHEATLOGID_SavedItemError				= 2400,
	CHEATLOGID_ContinuousSaveFailedError	= 2700,
	CHEATLOGID_AccountCharacterError		= 2800,
	CHEATLOGID_TradeAuthorizationError		= 4000,
	CHEATLOGID_MoneyTransferError			= 4100,
	CHEATLOGID_MultipleConnectionsIP		= 5000,
	CHEATLOGID_TooManyPackets				= 5100,
	CHEATLOGID_GameServerIPError			= 5200,
	CHEATLOGID_StartCharacterError			= 6000,
	CHEATLOGID_ServerMoneyError1			= 6610,
	CHEATLOGID_ServerMoneyError2			= 6611,
	CHEATLOGID_ServerMoneyError3			= 6612,
	CHEATLOGID_ItemCodeError				= 6800,
	CHEATLOGID_ItemTempCodeError			= 6810,
	CHEATLOGID_CharacterStateError1			= 6900,
	CHEATLOGID_CharacterSkillPointError		= 6910,
	CHEATLOGID_CharacterWeightError			= 6920,
	CHEATLOGID_ClientProcessTimeOut			= 7000,
	CHEATLOGID_InventoryItemError			= 7010,
	CHEATLOGID_CharacterModelError			= 7020,
	CHEATLOGID_JobCodeError					= 7030,

	CHEATLOGID_ClientAttackDefenseError		= 7100,
	CHEATLOGID_ClientEnergyBarError			= 7110,
	CHEATLOGID_CopiedItemFromFloor			= 7130,
	CHEATLOGID_TriedConnectDisableIP		= 7140,
	CHEATLOGID_ItemErrorInventory			= 7150,
	CHEATLOGID_MoneyErrorInventory			= 7160,

	CHEATLOGID_ServerItemErrorInventory		= 8000,
	CHEATLOGID_ItemErrorInventoryRecord		= 8001,
	CHEATLOGID_ServerMoneyErrorInventory	= 8010,
	CHEATLOGID_ServerToServerItemError		= 8020,
	CHEATLOGID_WrongSavingCharacterName		= 8030,
	CHEATLOGID_ItemPositionError			= 8040,
	CHEATLOGID_ServerInventoryUsedFull		= 8050,
	CHEATLOGID_UsedItemCodeWarning			= 8060,
	CHEATLOGID_ServerInventoryCopiedItem	= 8070,
	CHEATLOGID_ServerCopiedItemWarehouse	= 8071,
	CHEATLOGID_MemoryBufferSavingError		= 8080,
	CHEATLOGID_WarningAutoMouse				= 8100,
	CHEATLOGID_WarningMacroMouse			= 8102,
	CHEATLOGID_WarningAutoClick				= 8103,
	CHEATLOGID_WarningAvgDamageDefense		= 8110,
	CHEATLOGID_WarningAvgDamageAttack		= 8120,
	CHEATLOGID_AgingFailedCopiedItemSheltom	= 8200,
	CHEATLOGID_AgingFailedCopiedItem		= 8210,
	CHEATLOGID_ReconnectServer				= 8300,
	CHEATLOGID_CheckedInventoryData			= 8400,
	CHEATLOGID_TeleportFieldHack			= 8401,
	CHEATLOGID_DamagePacketError			= 8500,
	CHEATLOGID_LimitDamageOver				= 8510,
	CHEATLOGID_LimitDamageTime				= 8511,
	CHEATLOGID_ClientWarningMotionSpeed		= 8520,
	CHEATLOGID_ClientWarningSkillAttack		= 8530,
	CHEATLOGID_ClientWarningSkillForgery	= 8540,
	CHEATLOGID_ClientWarningSkillForgery2	= 8550,
	CHEATLOGID_InitialCharacterLevelError	= 8560,
	CHEATLOGID_TooManyUpdatedCharInfo		= 8570,
	CHEATLOGID_ClientDamagePacketError		= 8580,
	CHEATLOGID_WarningInvincibleMode		= 8600,
	CHEATLOGID_FindThreadHack				= 8720,
	CHEATLOGID_SuspendThreadHack			= 8721,
	CHEATLOGID_CharacterStateError2			= 8730,
	CHEATLOGID_ServerPotionError			= 8800,
	CHEATLOGID_ServerPotionMovingError		= 8810,
	CHEATLOGID_LevelQuestCodeWarning		= 8820,
	CHEATLOGID_ShopBuyForgeryItem			= 8830,
	CHEATLOGID_ServerMoneyOverflow			= 8840,
	CHEATLOGID_SkillUsedLevelError			= 8850,
	CHEATLOGID_CheckPlayFieldWarning		= 8860,
	CHEATLOGID_CompareClanCodeError			= 8870,
	CHEATLOGID_ModelFileError				= 8880,
	CHEATLOGID_WarningCharacterReloading	= 8890,
	CHEATLOGID_FieldNPCWarning				= 8900,
	CHEATLOGID_LevelHack					= 8901,
	CHEATLOGID_ItemMeshError				= 8910,
	CHEATLOGID_IllegallyServerConnect		= 8920,

	CHEATLOGID_MatureHack					= 8950,
	CHEATLOGID_AgingHack					= 8951,
	CHEATLOGID_FakeGM						= 8952,
	CHEATLOGID_RunSpeedHack					= 8953,
	CHEATLOGID_CheckSumFunctionError		= 8954,
	CHEATLOGID_WindowHack					= 8955,
	CHEATLOGID_StateCharacterHack			= 8956,
	CHEATLOGID_DebuggerHack					= 8957,
	CHEATLOGID_GMReason						= 8958,
	CHEATLOGID_BellatraGoldHackLeader		= 8959,
	CHEATLOGID_DefenseMultHack				= 8960,
	CHEATLOGID_RegenFormulaHack				= 8961,
	CHEATLOGID_EasterEggFakeItem			= 8962,
	CHEATLOGID_RespecFakeItem				= 8963,
	CHEATLOGID_NPCItemShopFakeItem			= 8964,
	CHEATLOGID_ItemTimerFakeInventory		= 8965,
	CHEATLOGID_ManufactureItemFake			= 8966,
	CHEATLOGID_SkillCooldown				= 8967,
	CHEATLOGID_XmasRudolphFakeBox			= 8968,
	CHEATLOGID_ActionFieldFakeItem			= 8969,
	CHEATLOGID_LevelErrorSyncEx				= 8970,
	CHEATLOGID_PerfectizeItemDupe			= 8971,
	CHEATLOGID_SwapperItemDupe				= 8972,
	CHEATLOGID_ItemBoxItemDupe				= 8973,
	CHEATLOGID_ModuleSyncError				= 8974,
	CHEATLOGID_ItemFakeBoughtShopID			= 8975,
	CHEATLOGID_ItemFakeBoughtShopItem		= 8976,
	CHEATLOGID_ProcessHook					= 8977,
	CHEATLOGID_GetTickCountHack				= 8978,
	CHEATLOGID_QueryPerformanceHack			= 8979,
	CHEATLOGID_MultipleWindowHackProcess	= 8980,
	CHEATLOGID_EXEModuleSyncError			= 8981,
	CHEATLOGID_CRC32ChecksumError			= 8982,
	CHEATLOGID_CaravanItemAddError			= 8983,
	CHEATLOGID_CaravanItemDelError			= 8984,
	CHEATLOGID_BellatraRequestGoldError		= 8988,
	CHEATLOGID_WeaponCostumeError			= 8989,
	CHEATLOGID_ShieldCostumeError			= 8990,
	CHEATLOGID_PersonalShopGoldReceiveError	= 8991,
	CHEATLOGID_CharacterQuestInvalid		= 8992,
	CHEATLOGID_GlamorousItemError			= 8993,
	CHEATLOGID_CarnivalFakePuzzleItem		= 8994,
	CHEATLOGID_EarringItemError				= 8995,


	CHEATLOGID_MemorySaveNameError1			= 10000,
	CHEATLOGID_MemorySaveNameError2			= 10001,
	CHEATLOGID_MemorySaveNameError3			= 10002,
	CHEATLOGID_MemorySaveNameError4			= 10003,
	CHEATLOGID_MemorySaveNameError5			= 10005,
	CHEATLOGID_InitialSaveMemoryError		= 10006,
	CHEATLOGID_SaveBufferPointError			= 10007,
	CHEATLOGID_CharacterAccountMismatch		= 10010,
	CHEATLOGID_AuthenticationError			= 10020,
	CHEATLOGID_ShutdownService				= 11000,
	CHEATLOGID_EventHighScore				= 21000,

	CHEATLOGID_GoldHackNPC					= 11037,
	CHEATLOGID_GoldHackWH					= 11034,
	CHEATLOGID_MixingItemHack				= 99000,

};

/* Net Server */
struct PacketNetIdentifier : Packet
{
	int										iServerID;
	int										iPort;

	UINT									uPasswordNUM; //= 0x33445566
};
struct PacketNetUnloadUser : Packet
{
	DWORD									dwObjectID;
};
struct PacketNetDisconnectUser : Packet
{
	DWORD									dwObjectID;
};
struct PacketNetForceOrbTime : Packet
{
	DWORD									dwObjectID;
	BOOL									bUpdate;
	DWORD									dwTime;
};
struct PacketNetExp : Packet
{
	UINT									uObjectID;
	INT64									iExp;
};

struct PacketNetExpEvent : Packet
{
	int						iPercent;
};

struct PacketNetUsersOnline : Packet
{
	int						iID;
	int						iUsersOnline;
};

struct PacketNetEXPPotion : Packet
{
	DWORD									dwObjectID;
	BOOL									bExp;
	int										iPercent;
};

struct PacketNetClan : Packet
{
	UINT									uObjectID;
	DWORD									dwClan;
};
struct PacketNetGetExp : Packet
{
	UINT									uObjectID;
};
struct PacketNetPingPong : Packet
{
	int										iNothing;
};
struct PacketNetCharacterSync : Packet
{
	DWORD									dwObjectID;
	char									szCharName[32];
};
struct PacketNetGameMaster : Packet
{
	UINT									uObjectID;
	int										iGameMasterLevel;
};
struct PacketNetGetGameMaster : Packet
{
	UINT									uObjectID;
	int										iGameMasterLevel;
	char									szName[32];

	EAccountShare							eAccountShare;

	BOOL									bFirst = FALSE;
};

struct PacketNetVersion : Packet
{
	int										iVersion;
};

struct PacketNetBCNewOwnerClanID : Packet
{
	DWORD									dwNewClanID;
};

struct PacketNetTradeCoin : Packet
{
	int										iID;
	int										iReceiverID;
};

struct PacketNetT5QuestData : Packet
{
	int										iID;

	UINT									uQuestID;
	USHORT									sQuestValue[2];
};

struct PacketNetHardwareID : Packet
{
	int										iID;

	char									szHardwareID[40];
};

struct PacketNetActionField : Packet
{
	BOOL									bEvent;
};


struct PacketNetPvPBuff : Packet
{
	int					iCount;
	int					iaUsersID[128];
};

struct PacketNetPvPLoginHandle : Packet
{
	bool				bNotifyReset;
	bool				bAddBuff;
};

struct PacketNetBellatraRequest : Packet
{
	int					iID;
	int					iRoomID;
	int					iGold;
};

struct PacketNetBellatraGiveGold : Packet
{
	int					iID;
	int					iGold;
};

struct PacketNetResetPvPLogin : Packet
{
	bool				bReset;
};

struct PacketNetUpdatePvPBellatraBuff : Packet
{
	int					iaBellatraCharacterID[10][3];
	int					iaBellatraHonorType[10][3];

	int					iaPvPCharacterID[10][3];
	int					iaPvPHonorType[10][3];
};

#define MAX_ACTIONFIELD_USERS_NET		12

enum EActionFieldModeID;

struct PacketNetActionFieldRequest : Packet
{
	int					iID;
	EActionFieldModeID	iModeID;

	int					iCount;
	int					iaUsersPartyID[MAX_ACTIONFIELD_USERS_NET];

	BOOL				bEnter;
	int					iUserErrorID;
};

struct PacketNetQuestEXPFree : Packet
{
	BOOL					bActive;
	int						iExpEvent;

	PacketNetQuestEXPFree(BOOL _bEvent, int _iExp)
	{
		bActive = _bEvent;

		iExpEvent = _iExp;
	};

	~PacketNetQuestEXPFree() {};
};

struct PacketGetItemPerfectView : Packet
{
	DWORD		iItemID;
	int			iAgeLevel;
	int			iMixID;
};

struct PacketListUsingItem : Packet
{
	struct ItemUsing
	{
		int									iItemID;
		short								sPerformance[8];
	};
	int										iItemListCount;
	ItemUsing								sItemList[16];
};

//Timers

struct PacketOldHeadUse : Packet
{
	char				szHeadModel[64];

	BOOL				bUpdate;

	PacketOldHeadUse() {};
	PacketOldHeadUse( BOOL bUpdateHead ) { bUpdate = bUpdateHead; }
};

struct PacketNewItemTimer : Packet
{
	EItemTimerType		iType; 
	
	EItemID				iItemID;

	int					iChk1;
	int					iChk2;

	DWORD				dwTimeLeft;
	DWORD				dwTimeTotal; 

	PacketNewItemTimer( EItemID iItemID, int iChk1, int iChk2 ) { this->iItemID = iItemID; this->iChk1 = iChk1; this->iChk2 = iChk2; }
	PacketNewItemTimer() { }
};

struct PacketCancelItemTimer : Packet
{
	EItemTimerType		iType;

	EItemID				iItemID;
};

//Record
struct PacketRecordGameDataUpdate : Packet
{
	BOOL				bUpdate;
};


//Quest Arena T5
struct PacketQuestArenaTier5 : Packet
{
	enum EType
	{
		TYPEID_None,
		TYPEID_EnterArena,
		TYPEID_Round,
		TYPEID_FinishQuest,
		TYPEID_Lost,
	};

	int						iType;

	int						iRound;
	DWORD					dwTimeLeft;

	PacketQuestArenaTier5() { this->iType = 0; this->iRound = 0; this->dwTimeLeft = 0; };
	PacketQuestArenaTier5( int iType, int iRound, DWORD dwTimeLeft ) { this->iType = iType; this->iRound = iRound; this->dwTimeLeft = dwTimeLeft; };
	~PacketQuestArenaTier5() {};
};

struct PacketEffectSpawn : Packet
{
	int						iX;
	int						iEffectID;
	int						iY;
	int						iZ;

	PacketEffectSpawn() { iEffectID = 256; iX = iY = iZ = 0; };
	PacketEffectSpawn( Point3D sPosition )
	{
		iEffectID	= 256;
		iX			= sPosition.iX;
		iY			= sPosition.iY;
		iZ			= sPosition.iZ;
	};
};

#define MAX_EVENTSERVER_TEXT        10

struct PacketEventServer : Packet
{
    struct EventData
    {
        bool            bEXP;
        int             iEXP;

        bool            bExtraDrop;
        int             iExtraDrop;

        bool            bPvP;

        bool            bWantedMoriph;
        bool            bWantedWolf;

        bool            bHalloween;
        bool            bXmas;

        int             iEventCount;
        char            szaMessageEventLeft[MAX_EVENTSERVER_TEXT][32];
        BYTE            baTextColorLeft[MAX_EVENTSERVER_TEXT][4];
        BOOL            baBoldLeft[MAX_EVENTSERVER_TEXT];
        char            szaMessageEventRight[MAX_EVENTSERVER_TEXT][32];
        BYTE            baTextColorRight[MAX_EVENTSERVER_TEXT][4];
        BOOL            baBoldRight[MAX_EVENTSERVER_TEXT];

    };

    EventData           sEventData;
};

#define SKILLTABLE_LEN	(MAX_PKTSIZ - 0x40)

struct PacketSkillTable : Packet
{
	MinMax					sPacketCount;


	int						iSize;
	char					baBuffer[SKILLTABLE_LEN];

	bool					IsFirst() { return sPacketCount.sMin == 1 ? true : false; }
	bool					IsOnly() { return sPacketCount.sMin == sPacketCount.sMax ? true : false; }
};

struct PacketHonor : Packet
{
	enum EHonorType : int
	{
		HONORTYPE_None,
		HONORTYPE_PvPGold,
		HONORTYPE_PvPSilver,
		HONORTYPE_PvPBronze,

		HONORTYPE_BellatraGold = 51,
		HONORTYPE_BellatraSilver,
		HONORTYPE_BellatraBronze,
	};

	EHonorType		ePvPHonor;
	UINT64			uPvPTime;

	EHonorType		eBellatraHonor;
	UINT64			uBellatraTime;
};

struct PacketBellatraNPC : Packet
{
	int				iPadding001 = 0;
	int				iBellatraNPCID;

	int				iaPadding002[2] = {0};

	PacketBellatraNPC() {};
	PacketBellatraNPC( int iNPCID ) { iBellatraNPCID = iNPCID; };
	~PacketBellatraNPC() {};
};

struct PacketChallengeRequest : Packet
{
	int					iID;
	char				szCharacterName[32];
	int					iAmount;
	bool				bAccepted;

	PacketChallengeRequest( int _iID, const std::string strName, int _iAmount, bool bGold )
	{
		iID = _iID;
		STRINGCOPY( szCharacterName, strName.c_str() );
		iAmount = _iAmount;
		bAccepted = false;
	};

	PacketChallengeRequest() {};

	~PacketChallengeRequest() {};
};

struct PacketChallengeUpdate : Packet
{
	int					iaID[2];
	int					iaScore[2];
	char				szaCharacterName[2][32];

	int					iTimeLeft;
};

