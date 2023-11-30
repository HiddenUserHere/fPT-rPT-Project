#pragma once

#include "CPersonalShopWindowHandler.h"

#define MAX_PERSONALSHOP_VISITED		50

class CPersonalShopHandler
{
public:
	CPersonalShopHandler();
	virtual ~CPersonalShopHandler();

	CPersonalShopWindowHandler			* GetWindow() { return pcWindow; }

	void								Init();

	void								HandlePacket( PacketPersonalShopItemList * psPacket );
	void								HandlePacket( PacketPersonalShopTradeItems * psPacket );
	void								HandlePacket( PacketPersonalShopRequestView * psPacket );

	void								RequestPersonalShop( UnitData * pcUnitData );

	int									GetCanCarryGold();
	int									GetGoldSelling();

	bool								CanUseGoldShop( PricePersonalShopData * psPriceData );
	std::tuple<bool, PricePersonalShopData *>	GetPrice( int iItemID, int iChk1, int iChk2 );

	ItemData							* GetLastRenderedItem() { return pcLastBoughtItem; }

	bool								GetVisitedShop( int iID );
	void								AddVisitedShop( int iID );
	void								RemoveVisitedShop( int iID );
	bool								GetPinnedShop( int iID );
	void								AddPinnedShop( int iID );
	void								RemovePinnedShop( int iID );

private:
	CPersonalShopWindowHandler			* pcWindow;

	std::vector<PricePersonalShopData*> vPriceData;


	void								AddPrice( PricePersonalShopData * psPriceData );
	void								RemovePrice( int iItemID, int iChk1, int iChk2 );

	void								ClearShop();

	int									GetSellerID() { return iLastUserSellerID; }

	void								ProcessPacket( PacketChatBoxMessage * psPacket );
	void								ProcessPacket( PacketPersonalShopItemList * psPacket );
	void								ProcessPacket( PacketPersonalShopTradeItems * psPacket, bool bSell );
	void								ProcessPacket( PacketPersonalShopRequestView * psPacket );

	void								SetLastBoughtItem( ItemData * pcLast ) { pcLastBoughtItem = pcLast; }

	friend class CPersonalShopWindowHandler;

	std::vector< PacketPersonalShopItemList*> vReceivePackets;

protected:
	int									iLastUserSellerID = 0;

	ItemData							* pcLastBoughtItem = nullptr;

	std::vector<int>					vVisitedShop;
	std::vector<int>					vPinnedShop;
};

