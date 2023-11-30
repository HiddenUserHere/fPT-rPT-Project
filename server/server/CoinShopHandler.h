#pragma once

static char * pszaNotAllowedNamesChangeNick[]
{
	"[gm]",
	"[adm]",
	"sandurr",
	"realmpt",
	"gm-",
	"adm-",
	"gamemaster"
};


class CCoinShopHandler
{
protected:
	//class CCoinShop										* pcCoinShop = NULL;

	std::vector<CCoinShop *>							vCoinShops;

	int													iChangeNickPrice = 0;
	int													iAgingRecoveryPrice = 0;

public:

	static const int											COINSHOP_DEFAULT_ID		= 1;
	static const int											COINSHOP_TIMESHOP_ID	= 2;

	CCoinShopHandler();
	virtual ~CCoinShopHandler();

	void												SQLCoinLog( CCoinShop * pcCoinShop, char * pszAccountName, const std::string strDescription );

	void												LoadCoinShop( int iCoinShopID = -1 );

	void												ReloadCoinShops();

	void												Init();

	int													GetCoinAmount( CCoinShop * pcCoinShop, char * pszAccountName );

	int													GetCoinTradeAmount( char * pszAccountName );

	int													GetBulkValue( CCoinShopItem * pcItem, int iQuantity );

	int													UseCoin( CCoinShop * pcCoinShop, char * pszAccountName, int iCoinAmount );
	
	void												AddTradeCoin( char * pszAccountName, int iCoinAmount );
	int													AddCoin( CCoinShop * pcCoinShop, char * pszAccountName, int iCoinAmount );
	int													HasCoinAmount( CCoinShop * pcCoinShop, char * pszAccountName, int iCoinAmount );

	BOOL												PerfectizeItem( User * pcUser, struct PacketCoinServiceBuy * psPacket );
	BOOL												ItemSwapper( User * pcUser, struct PacketCoinServiceBuy * psPacket );
	BOOL												ChangeNick( User * pcUser, struct PacketCoinServiceBuy * psPacket );
	BOOL												LevelUP( User * pcUser, struct PacketCoinServiceBuy * psPacket );
	BOOL												AgingRecovery( User * pcUser, struct PacketCoinServiceBuy * psPacket );
	BOOL												ChangeClass( User * pcUser, struct PacketCoinServiceBuy * psPacket );
	BOOL												NewItemTimer( User * pcUser, struct PacketCoinBuy * psPacket );

	void												ProcessPacket( User * pcUser, PacketCoinServiceBuy * psPacket );
	void												ProcessPacket( User * pcUser, struct PacketCoinShopNPC * psPacket );
	void												ProcessPacket( User * pcUser, struct PacketCoinAmount * psPacket );
	void												ProcessPacket( User * pcUser, struct PacketCoinOpenTab * psPacket );
	void												ProcessPacket( User * pcUser, struct PacketCoinTab * psPacket );
	void												ProcessPacket( User * pcUser, struct PacketCoinItemData * psPacket );

	void												HandlePacket( User * pcUser, struct PacketCoinShopNPC * psPacket );
	void												HandlePacket( User * pcUser, struct PacketCoinOpenTab * psPacket );
	void												HandlePacket( User * pcUser, struct PacketCoinAmount * psPacket );
	void												HandlePacket( User * pcUser, struct PacketCoinOpenItemData * psPacket );
	void												HandlePacket( User * pcUser, struct PacketCoinBuy * psPacket );
	void												HandlePacket( User * pcUser, struct PacketCoinServiceBuy * psPacket );

	void												HandlePacket( User * pcUser, struct PacketTradeCoinSet * psPacket );

	void												SendUserCoinAmount( User * pcUser );

	CCoinShopTab										* GetTabCoinShopByID( int iTabID );
	
	CCoinShop											* GetCoinShopByTabID( int iTabID );
	CCoinShop											* GetCoinShop( int iID = COINSHOP_DEFAULT_ID );

};

