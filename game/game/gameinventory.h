#pragma once

#define AGEPACKET_SENDING		(*(BOOL*)0x039C8710)
#define NOTCHANGE_ITEM			(*(BOOL*)0x035EB6C4)
#define SHOWSPECIAL_ITEM		(*(BOOL*)0x035EB6C0)

#define ARROWINVENTORY_LEFT		(*(BOOL*)0x035EB664)
#define ARROWINVENTORY_RIGHT	(*(BOOL*)0x035EB668)

#define INVENTORY_DATACOUNT			100
#define INVENTORYSLOT_WHEEL			97
#define INVENTORYSLOT_WHEELCOUNT	99

#define INVENTORY_OVERWEIGHT	(*(int*)0x035EB618)

struct ItemPriceData
{
	int				iPurePrice;
	int				iSellPrice;
	int				iRepairCost;
};


class CGameInventory
{
protected:
	IMPFNC						pfnCheckMousePotionCount	= 0x004B08B0;
	IMPFNC						pfnOnMouseLClickDown		= 0x004C5940;
	IMPFNC						pfnGetInventoryTwoHandItem	= 0x004BA060;
	IMPFNC						pfnLoadItemImageEXPGold		= 0x004B0D60;
	IMPFNC						pfnSetInventoryItemItem		= 0x004BB6D0;
	IMPFNC						pfnResetPotion				= 0x00461250;

    IMPFNC                      pfnInventorySideFlag        = 0x035EB68C;

public:
	BOOL						bOpen;
	ItemData					saInventoryItems[INVENTORY_DATACOUNT];
	ItemData					saInventoryItemTemp[INVENTORY_DATACOUNT];
	ItemData					saBackupInventoryItemTemp[INVENTORY_DATACOUNT];

								CGameInventory();
							   ~CGameInventory();

	static CGameInventory		* GetInstance() { return (CGameInventory*)0x035EBB20; }

    static int                  GetInventorySideIndex() { return READINT( pfnInventorySideFlag ); }
    static void                 SetInventorySideIndex( int iIndex ) { WRITEINT( pfnInventorySideFlag, iIndex ); }


	int							CheckMousePotionCount();
	void						OnMouseLDownClick( int iX, int iY );

	BOOL						CanChangeSpecialItem( BOOL b = FALSE );

	BOOL						ChangeInventorySide( int iType );

	int							GetInventoryPersonalShopItem( ItemData * pcItemData );
	int							GetInventoryItemIndex( ItemData * pcItemData );
	ItemData					* GetInventoryItemIndexEx( int iItemID, int iChk1, int iChk2 );

	BOOL						GetInventoryItem( int iX, int iY, int iPickup );
	BOOL						GetInventoryTwoHandItem( ItemData * pcItem );
	
	BOOL						SetInventoryItem( ItemData * psItem );
	BOOL						SetInventoryItem( Item * psItem );

	BOOL						PlaceItemInventory( Item * psItem );

	BOOL						IsValidRequirements( ItemData * pcItemData );

	BOOL						LoadItemImageEXPGold( ItemData * pcItemData );

	static BOOL					ResetPotion();

	static void					RenderForceEffect( Point2D sPosition );

	static void					RenderUnionCoreEffect( Point2D sPosition );

	static BOOL					PlaceQuestItem( Item * psItem );

	void						DropInventoryItemsOnMap();

	void						GetItemPrice( ItemPriceData * psItemPrice, ItemData * pcItemData );

	BOOL						RepairItem( ItemData * pcItemData, int iRepairCost );
};

