#pragma once

#include "roll.h"

#define MESSAGEBOX_ITEMBOX				((NPCItemBoxMessageBox*)0x0362AAC0)

#define ITEMTABLE_ITEM_COUNT			(*(UINT *)0x04B06B98)

enum ETypeItemUse
{
	HAIRSTYLETYPEID_None,
	HAIRSTYLETYPEID_MaleOnly,
	HAIRSTYLETYPEID_FemaleOnly,
};

enum EItemCheckInventory
{
	ITEMCHECKINVENTORY_None,
	ITEMCHECKINVENTORY_Potion,
	ITEMCHECKINVENTORY_Special,
	ITEMCHECKINVENTORY_Requirements,
};

struct sHairPotion
{
	DWORD										dwCode;
	ETypeItemUse								eUseType;
	const char									* pszFile;

};

struct WeaponParticlePosition
{
	int			iCount;
	Point3D		saPosition[4];
};

class ItemHandler
{
private:
	ModelAnimation								* psaModelDataDefault[10];

	int											iUniqueShopID = 0;
	int											iNPCID = 0;

public:
												ItemHandler();
	virtual										~ItemHandler();

	class ItemData *							FindItemTable( UINT uPosition );
	int											FindItemTablePosition( int iItemID );
	class ItemData *							FindItemTableByCode( DWORD dwCode );
	class ItemData *							GetMouseItemHover();
	class ItemData *							GetItemEquippedByType( enum EItemType eType );
	class ItemData *							GetItemEquippedByBase( EItemBase eItemBase );
	int											GetItemEquippedCountByBase( EItemBase eType, ECharacterClass iClass = CHARACTERCLASS_None );
	int											GetItemEquippedCountByType( EItemType eType, ECharacterClass iClass = CHARACTERCLASS_None );
	int											GetItemEquippedCountByCode( EItemID eType, ECharacterClass iClass = CHARACTERCLASS_None );
	ItemData *									GetItemInventoryByCode( EItemID eID );
	ItemData *									GetItemInventory( EItemID eID, int iChk1, int iChk2 );
	int											GetItemAmountByCode( EItemID eID, EItemCraftType eCraft = ITEMCRAFTTYPE_None );
	char *										GetArmorPatternName( class UnitData * pcUnit, DWORD dwCode );
	BOOL										GetItemPrice( class ItemData * pcItemData, PriceItem * psSellData );

	static BOOL									DropItemToMap( ItemData * pcItemData );
	static BOOL									DropItemToMap( Item * psItem, Point3D sPosition );
	static BOOL									UseItem( Item * psItem );

	int											DeleteItemQuest( EItemID eID, int iCount = 0 );
	void										DeleteItem( ItemData * psItemData );

	BOOL										SetCharacterItem( EItemID iItemID, int iPosition, BOOL bActive );

	void										CheckPuzzleItemEvent();

	void										CheckRequirements();

	void										ResetPotionGate();

	BOOL										LoadItemImage( Item * psIn, ItemData * psOut );

	void										OnLoadArmorCharacter();

	void										CheckDamageItems();
	void										UpdateItemCharacterStatus( BOOL bHackCheck = FALSE );

	int											OnItemPivot( int iPosition, UINT uItemID );

	BOOL										OnSetArmorUnit( class UnitData * psUnit, UINT uItemCode );
	BOOL										OnSetArmorTargetUnit( class UnitData * psUnit, UINT uPosition );

	ECharacterGender							GetItemGender( int iItemID );

	BOOL										CanEquipItem( void * pcClassItemData, ItemData * pcItemData );

	BOOL										EquipItem( UnitData * pcUnitData, EItemID iItemID, int iPositionID );
	BOOL										EquipBody( UnitData * pcUnitData, int iPositionID );

	BOOL										OnSetHairModel( DWORD dwCode );
	const char *								GetHeadModel( DWORD dwCode );

	BOOL										OnHoverItem( DWORD dwCode );
	char *										OnDescriptionItem( DWORD dwCode );
	void										ValidateInventoryItems();
	void										ResetInventoryItemChecksum();
	void										ResetItemMouse();
	BOOL										BackItemToInventory( ItemData * psItem, EItemCheckInventory eCheck = ITEMCHECKINVENTORY_None, bool bSlot = true );

	BOOL										DeleteItemForServer( EItemID eItemID, int iChk1, int iChk2 );


	void										ShowItemInfo( ItemData * pItem, BOOL bBottom = TRUE );
	ItemData									* GetMouseItem() { return (ItemData*)0x036283D8; };
	void										DrawColorBox( DWORD Color, int iX, int iY, int iW, int iH );
	void										DrawSprite( int winX, int winY, int lpdds, int x, int y, int width, int height );
	bool										DelayCristal();
	BOOL										OnSendUpgradeAgingItem( ItemData * psItem );
	BOOL										CheckQuestItemAging( Item * psItemInfo );
	BOOL										OnMatureStone( DWORD dwCode );
	BOOL										OnSkillStone( DWORD dwCode );

	void										OnAcceptRollDiceItem();

	BOOL										OnCheckCanItemDrill( ItemData * pcItemData, int iUnk );

	BOOL										OnRenderRightWeapon( UnitData * pcUnitData, PTModel * pcModel, EXEMatrixI * psSourceMatrix, EXEMatrixI * psDestMatrix );
	BOOL										OnRenderSecondWeapon( UnitData * pcUnitData, PTModel * pcModel, EXEMatrixI * psSourceMatrix, EXEMatrixI * psDestMatrix, BOOL bRight );

	int											OnUseItem( ItemData * pcItemData );

	void										ProcessItemSlotAction( ESlotTypeHandle iAction, ESlotTypeHandleWhere iWhere, int iItemID, int iChk1, int iChk2 );

	void										SendGoldUse( int iGold );
	void										ProcessGold();

	void										FormatDropItemGold( DropItemView * ps );
	BOOL										CanViewDropItem( DropItemView * ps );
	BOOL										SameSpecFilterDropItem( DropItemView * ps );

	BOOL										IsImportantDrop( DropItemView * ps );

	static PacketItem							* GetItemQueue( EItemID eItemID, int iChk1, int iChk2 );

	void										HandlePacket( PacketNPCItemBox * psPacket );
	void										HandlePacket( PacketItemShopList * psPacket );
	void										HandlePacket( PacketPlaceItem * psPacket );
	void										HandlePacket( PacketDeleteItem * psPacket );

	void										LoadDefaultModels();

	ModelAnimation								* GetModelDefault( int iClass );

	void										ShowDebugCloneItemWarehouse( ItemData * pcItemData );

	void										DropSpecialItem( ItemData * pcItemData );

	int											EncodeItemsData( void * psItems, void * psDest, int iLength );
	int											DecodeItemsData( void * psItems, void * psDest, int iLength );

	bool										IsForceEffect( Item & sItem );

	BOOL										BuyItemShop( ItemData * pcItemData, int iItemCount );

	static BOOL									IsValidItem( Item * psItem );

	static bool									GetWeaponParticlePosition( UnitData * pcUnitData, WeaponParticlePosition * psWeaponPosition );

	BOOL										MatureItem( EMatureType eMatureType, bool bInstantMature );

	void										HandlePacket( PacketAgingUpgradeItem * psPacket );

	BOOL										SellItem( ItemData * pcItemData, int iCount = 1 );
};

