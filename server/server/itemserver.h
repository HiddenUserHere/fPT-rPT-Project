#pragma once
#include "SocketSystemHandler.h"

enum AgeStoneList
{
	AgingStone = 0x080B0A00,
	CooperOre = 0x080B0B00,
	MaxAge = 0x080C5200,
};

static const int CRC_ITEMSHOP_UNIQUEID = 0x684871;

struct LoadItemDataHandler
{
	BOOL				bLoaded;
	int					iID;
	ItemLoadData		sItem;

	char				szCode[64];
};


#define SOCKETSYSTEMHANDLER						ItemServer::pcSocketSystemHandler			

class ItemServer
{
protected:

	static const DWORD								pfnCheckItem			= 0x00445FA0;
	static const DWORD								pfnDeleteItem			= 0x00550EA0;
	static const DWORD								pfnAddItem				= 0x00568410;
	static const DWORD								pfnIncreaseAgingState	= 0x004BD540;
	static const DWORD								pfnSaveItemData			= 0x005555B0;
	static const DWORD								pfnRecordAgingLog		= 0x00553D70;
	static const DWORD								pfnRecordItemChangeLog	= 0x00554C90;
	static const DWORD								pfnCreateDefItem		= 0x0043C580;
	static const DWORD								pfnCreateItemCount		= 0x00552890;
	static const DWORD								pfnIncreaseAging		= 0x004BD540;
	static const DWORD								pfnSetAgingChecksum		= 0x004461C0;

	typedef											INT( __cdecl *tfnDeleteItem )(UserData * pcUserData, DWORD dwCode, DWORD dwHead, DWORD dwChkSum);
	typedef											INT( __cdecl *tfnAddItem )(UserData * pcUserData, DWORD dwCode, DWORD dwHead, DWORD dwChkSum);
	typedef											BOOL( __cdecl *tfnCheckItem )(Item * psItem);
	typedef											BOOL( __cdecl *tfnIncreaseAgingState )(Item * psItem);
	typedef											BOOL( __cdecl *tfnSaveItemData )(Item * psItem);
	typedef											void( __cdecl *tfnRecordAgingLog )(UserData * pcUserData, Item * psItemSource, Item * psItemDest );
	typedef											void( __cdecl *tfnRecordItemChangeLog )(UserData * pcUserData, DWORD dwItemCode, DWORD	dwHead, DWORD dwChkSum, DWORD dwNewItemCode, DWORD dwNewHead, DWORD dwNewChkSum, int Flag);
	typedef											void( __cdecl *tfnCreateDefItem )(Item * psItem, DefinitionItem * psDefItem, int iSpeck);
	typedef											void( __cdecl *tfnCreateItemCount )(UserData * pcUserData, DWORD dwItemCode, int iCount);
	typedef											void( __cdecl *tfnIncreaseAging )(Item * psItem);
	typedef											void( __cdecl *tfnSetAgingChecksum )(Item * psItem);

private:

	EItemTimerType								GetPremiumType( DWORD dwCode );
	PremiumItem									* GetPremiumInfo( DWORD dwCode );
	void										SendItemTimer( User * pcUser, EItemID iItemID, EItemTimerType iType, DWORD dwTimeLeft, DWORD dwTimeTotal );

	static LoadItemDataHandler					* psRecoveryQueue;
	static HANDLE								hThreadRecovery;
	static HANDLE								hEventRecoveryQueue;

public:
	ItemServer();
	virtual ~ItemServer();

	static CSocketSystemHandler					* pcSocketSystemHandler;

	BOOL										CheckQuestItemAging( const char * pszItemName, int iItemID = 0 );

	ItemData *										FindItemPointerTable( const char * szCode );
	ItemData *										FindItemPointerTable( UINT uCode );
	DefinitionItem *								FindItemDefByCode( DWORD dwCode );
	DefinitionItem *								FindItemDefByCode( const char * pszCode );
	DefinitionItem *									FindItemDefByLevelBase( int iItemBase, int iLevel );
	int											FindItemPositionTable( char * pszName );
	BOOL 										FindItemName( UINT uCode, char * szBufName );
	BOOL 										FindItemName( char * pszCode, char * szBufName );
	int											GetItemIDByItemCode( char * pszCode );

	BOOL										IsDropableItem( int iItemID );

	int											SpecStoneToCharacterSpec( int iItemID );

	BOOL										IsDeluxeStone( int iItemID );

	static CharacterClassData					* GetItemSpecClassJob( int iClass, DefinitionItem * psDefItem );

	int											UseItem( UserData * pcUserData, DWORD dwCode, DWORD dwHead, DWORD dwChkSum );
	int											UseItem( User * pcUser, DWORD dwCode, DWORD dwHead, DWORD dwChkSum );
	int											AddItemInventory( UserData * pcUserData, DWORD dwCode, DWORD dwHead, DWORD dwChkSum );
	int 										FindItemInventory( UserData * pcUserData, DWORD dwCode, DWORD dwHead, DWORD dwChkSum );
	BOOL 										GetItemInventoryCheksum( User * pcUser, EItemID iItemID, int & iChk1, int & iChk2 );

	void										SendDeleteItem( User * pcUser, EItemID iItemID, int iChk1, int iChk2 );

	BOOL										IsItemSafe( UserData * pcUserData, int iItemID, int iChk1, int iChk2, int iGold, DWORD dwSec1, DWORD dwSec2, DWORD dwSec3, DWORD dwSec4, int * piaPotionCount );
	static BOOL									IsItemSafeServer( UserData * pcUserData, int iItemID, int iChk1, int iChk2, int iGold, DWORD dwSec1, DWORD dwSec2, DWORD dwSec3, DWORD dwSec4, int * piaPotionCount );

	void										SaveItemData( Item * psItem );
	BOOL										CheckValidItem(Item * psItem);
	void										CreateItem( Item * psItem, DefinitionItem * psDefItem, int iSpeck = 0 );
	void										CreateItem( UserData * pcUserdata, EItemID eItem, int iCount );
	void										CreatePerfectItem( Item * psOut, DefinitionItem * psIn, int iSpeck = 0 );

	int											AddPotion( UserData * pcUserData, EItemID iItemID, int iCount );

	BOOL										GetDropPotion( UserData * pcUserData, int iItemID );
	BOOL										AddDropPotion( UserData * pcUserData, int iItemID );

	BOOL										CreatePremium( UserData * pcUserData, EItemID iItemID, int iType, DWORD dwTime );

	void										SetAgingItem( ItemData * psItemData );

	int											DecodeItemsData( void * pItemData, void * pItemDest, int iSize );
	int											EncodeItemsData( void * pItemData, void * pItemDest, int iSize );
	int											EncodeItemsData( void * pItemData, void * pItemDest, int iSize, int iSizeDest );

	void										OnSetItemPerfect( Item * pItem );

	void										SendMixDescription( UserData * pcUserData, UINT puMix );

	int											GetAgingType( UserData * pcUserData );

	void										CreateMixingList();
	BOOL										OnUpgradeAgingItem( UserData * pcUserData, PacketItem * psPacket );
	BOOL										OnAgingItem( UserData * pcUserData, PacketAgingItem * psPacket );

	void										ProcessIntegrity( User * pcUser, EIntegrityUpdate eUpdate );
	void										UpdateIntegrity( Item * psItem, int iValueCur, int iValueMax = 0 );

	DWORD										OnPhoenixPet( UserData * pcUserData, DWORD dwCode, BOOL bDelete, int iElementType = PHOENIXTYPEID_None );
	BOOL										OnPremiumItem( DWORD dwCode, int iX, int iY, int iZ, UserData * pcUserData );

	BOOL										ValidateItem( ItemData * psItem );
	BOOL										ValidateItem( Item * psItem );
	void										ValidateItemTime( Item * psItem );
	void										RegisterItemSecCode( UserData * pcUserData, PacketItem * psItem );

	void										SendItemData( UserData * pcUserData, Item * psItem );

	BOOL										OnSaveItemData( Item * psItem );
	BOOL										OnLoadItemData( ItemLoadData * psItem, const char * pszCode );

	ItemPremium									* GetPremiumUse( UserData * pcUserData, EItemTimerType eType );
	ItemPremium									* GetPremiumUse( UserData * pcUserData, EItemID eID );

	void										OnLoadUser( User * pcUser );
	void										OnUnLoadUser( User * pcUser );
	void										UpdateItemTimer( User * pcUser );

	void										OnUseCrystal( UserData * pcUserData, int iID, DWORD dwItemID, DWORD dwTime );
	void										OnUseCrystalDie( UserData * pcUserData, int iID );

	BOOL										SQLGetItemTimer( User * pcUser, int iType, EItemID & iItemID, DWORD & dwTimeLeft, DWORD & dwTimeTotal );
	void										SQLUpdateItemTimer( User * pcUser, int iType, DWORD dwTimeLeft );
	void										SQLDeleteItemTimer( User * pcUser, int iItemID );
	BOOL										SQLAddItemTimer( User * pcUser, int iType, EItemID iItemID, DWORD dwTimeTotal );

	void										UpdateItemTimerEXP( User * pcUser, int iType, bool bRemove );

	BOOL										OnMixingItem( User * pcUser, struct PacketMixServer * psPacket );

	BOOL										OnResetItem( User * pcUser, struct PacketResetItem * psPacket );

	BOOL										OnSocketItem( User * pcUser, struct PacketSocketSystem * psPacket );

	void										OnManufactureItem( User * pcUser, struct PacketManufactureItem * psPacket );
	void										OnManufactureWeightPrice( User * pcUser );

	void										PHGetPerfectItem( User * pcUser, struct PacketGetItemPerfectView * psPacket );

	void										HandlePacket( User * pcUser, PacketGetItem * psPacket );

	BOOL										SQLLoadItemBox( User * pcUser );
	void										SQLItemLogBox( char * pszAccountName, ItemLogBox * psItemLogBox, BOOL bAccepted );
	void										SQLItemLogBox( User * pcUser, Item * psItem, int iID, int iAmount, BOOL bAccepted );
	int											SQLItemBoxCount( char * pszAccountName );
	int											SQLItemBoxCount( char * pszAccountName, EItemID eItemID );
	BOOL										OpenItemBox( User * pcUser, PacketNPCItemBox * psPacket );

	BOOL										AddItemOpenBox( UserData * pcUserData, char * pszItemName, int iSpec = 0, int iQuantity = 1 );
	BOOL										AddItemOpenBox( char * pszAccountName, char * pszItemName, int iSpec = 0, int iQuantity = 1 );
	BOOL										AddItemOpenBox( char * pszAccountName, Item * psItem, const char * pszSender = "Server" );

	void										PHCheckItemUsing( UserData * pcUserData, PacketListUsingItem * psPacket );
	void										PHCheckItemUsingEx( UserData * pcUserData, PacketListUsingItem * psPacket );

	void										UpgradeNewItem( Item * psItemOld, Item * psItemNew );

	void										HandlePacket( User * pcUser, PacketBuyNPCShop * psPacket );

	void										HandlePacket( User * pcUser, PacketNewItemTimer * psPacket );
	void										HandlePacket( User * pcUser, PacketCancelItemTimer * psPacket );

	void										HandlePacket( User * pcUser, PacketOldHeadUse * psPacket );

	void										HandlePacket( User * pcUser, PacketSlotItemHandle * psPacket );

	void										HandlePacket( User * pcUser, PacketNPCItemBoxSend * psPacket );
	
	void										HandlePacket( User * pcUser, PacketRequestBeginnerItemsTimer * psPacket );
	void										HandlePacket( User * pcUser, PacketRequestChemistItemTimer * psPacket );

	static void									UpdateQueueRecovery();

	static void									DeleteQueue();

	void										CheckItemRecovery( User * pcUser );
	void										AddItemRecovery( User * pcUser, const char * pszCode );

	bool										IsItemTimerFreezedMap( EMapID iMapID, EItemTimerType iType );

	BOOL										SendItemShopList( User * pcUser, UnitData * pcUnitData );

};

