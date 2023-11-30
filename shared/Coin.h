#pragma once

#define MAXCOINSHOPITEMS								300
#define MAX_COINSHOPITEMS								30
#define	MAX_COINSHOPTABS								32

const int MAX_LEVEL_COINSHOP = 140;


#define ALLOWED_CHARS_NICK								"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ[]Çç-_=!@#$%&ºª~{}."

enum EItemCoinType
{
	ITEMCOIN_Normal,
	ITEMCOIN_Data,
	ITEMCOIN_LevelUP,
	ITEMCOIN_ItemPerfect,
	ITEMCOIN_ItemSwapper, 
	ITEMCOIN_ChangeNick,
	ITEMCOIN_AgingRecovery,
	ITEMCOIN_ChangeClass,
};

namespace Base
{
struct SCoinShopItem
{
	int													iID;
	char												szName[32];
	char												szDescription[128];
	char												szCode[8];
	char												szImage[64];
	int													iValue;
	int													iDiscount;
};

class Coin
{
public:

	static int								GetLevelItemEachAge( Item * psItem );
	static int								GetPerfectizePrice( Item * psItem );
	static int								GetSwapperPrice( Item * psItem, int iItemType );
	static int								GetAgingRecoveryPrice( Item * psItem );

	static int								GetRealItemLevel( Item * psItem );

	static int								GetLevelUPPrice( int iLevelOld, int iNewLevel );

	static bool								CanLevelUP( int iLevel );
};
};


struct PacketCoinsTraded : Packet
{
	int										iCoinsTraded;
};

struct PacketTradeCoinSet : Packet
{
	PacketTradeCoinSet( int i, int lID ) { iCoins = i; iID = lID; }

	int										iID;
	int										iCoins;
};

struct PacketOpenCoinShopNPC : Packet
{
	int										iID;
};

struct PacketCoinShopNPC : Packet
{
	struct Tab
	{
		int										iID;
		char									szName[32];
		int										iParentTabID;
		int										iDiscount;
		int										iBulk;
		int										iMaxBulk;
	};

	short									sPacketNumber;
	short									sPacketMax;

	int										iID;
	char									szName[32];
	char									szMessage[128];
	int										iDiscount;

	BOOL									bIsTimeShop;

	int										iTabs;
	Tab										aTabs[MAX_COINSHOPTABS];

	BOOL									IsFirst() { return sPacketNumber == 1 ? TRUE : FALSE; }
	BOOL									IsOnly() { return sPacketNumber == sPacketMax ? TRUE : FALSE; }
};

struct PacketCoinOpenTab : Packet
{
	int										iID;
	int										iReturnCode;
};

struct PacketCoinTab : Packet
{
	struct Item
	{
		int										iID;
		char									szName[32];
		char									szDescription[128];
		char									szCode[8];
		char									szImage[64];
		int										iValue;
		int										iDiscount;
		int										iBulk;
		int										iMaxBulk;
	};

	short									sPacketNumber;
	short									sPacketMax;

	int										iID;
	int										iItems;
	Item									aItems[MAX_COINSHOPITEMS];

	BOOL									IsFirst() { return sPacketNumber == 1 ? TRUE : FALSE; }
	BOOL									IsOnly() { return sPacketNumber == sPacketMax ? TRUE : FALSE; }
};

struct PacketCoinAmount : Packet
{
	int										iCoinAmount;
	int										iTimePoints;

	PacketCoinAmount( int iCoin, int iTimeCoin ) { iCoinAmount = iCoin; iTimePoints = iTimeCoin; }
	PacketCoinAmount() { iCoinAmount = 0; iTimePoints = 0; }
};

struct PacketCoinOpenItemData : Packet
{
	int												iID;
	int												iTabID;
};

struct PacketCoinOpenService : Packet
{
	int												iID;

	int												iPrice;
};

#define MAX_AGINGRECOVERY_PER_PACKET 20

struct AgingRecoveryDataHandle
{
	int					iID;

	SYSTEMTIME			sDate;

	char				szItemName[32];

	int					iPrice;

	int					iAgeNumber;
};


struct PacketOpenAgingRecoveryService : Packet
{
	int						iCount;

	AgingRecoveryDataHandle	saData[MAX_AGINGRECOVERY_PER_PACKET];

	short					sPacketNumber;
	short					sPacketMax;

	int						iPrice;

	BOOL					IsFirst() { return sPacketNumber == 1 ? TRUE : FALSE; }
	BOOL					IsOnly() { return sPacketNumber == sPacketMax ? TRUE : FALSE; }
};

struct PacketCoinItemData : Packet
{
	int												iID;
	int												iTabID;
	int												iType;

	BOOL											bSpec;
	BOOL											bQuantity;

	DefinitionItem									sItemDefinition;
};

struct PacketCoinBuy : Packet
{
	int												iID;
	int												iTabID;

	int												iParam1;
	int												iParam2;

	int												iReturnCode;
};

struct PacketCoinServiceBuy : Packet
{
	int							iType;
	
	int							iRetCode;

	union
	{
		struct SLevelUP
		{
			int						iLevel;

			char					szPadding[0x318];
		} LevelUP;

		struct SItemSwapper
		{
			int						iSpec;
			int						iItemBase;

			ItemData				cItemData;
		} ItemSwapper;

		struct SItemPerfect
		{
			int						iSpec;

			int						iUnk;

			ItemData				cItemData;
		} ItemPerfect;

		struct SChangeNick
		{
			char					szNewCharacterName[32];

			char					cPadding[0x2FC];
		} ChangeNick;

		struct SAgingRecovery
		{
			int						iID;

			int						iPadding;

			ItemData				cItemData;
		} AgingRecovery;

		struct SChangeClass
		{
			char					szNewCharacterName[32];
			BOOL					bPremiumChangeClass;
			int						iClass;

			char					cPadding[0x2F4];

		} ChangeClass;
	};
	
	PacketCoinServiceBuy() {};
	~PacketCoinServiceBuy() {};
};
