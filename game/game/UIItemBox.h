#pragma once

namespace UI
{ 
typedef std::shared_ptr<class ItemBox> ItemBox_ptr;
class ItemBox : public Element
{
public:
	/**
	 * Item Box Slot
	 * @param iX X position of slot
	 * @param iY Y position of slot
	 * @param iSlotCountX number of slots by column	= 4 Default
	 * @param iSlotCountY number of slots by row = 6 Default
	 */

	enum ERule
	{
		RULE_None,
		RULE_Allow,
		RULE_Disallow,
	};

	enum ETypeRule
	{
		TYPERULE_None,
		TYPERULE_ItemID,
		TYPERULE_ItemBase,
		TYPERULE_ItemType,
		TYPERULE_QuestItem,
		TYPERULE_AgeItem,
		TYPERULE_ItemPerfect,
		TYPERULE_EquippedItem,
		TYPERULE_Level,
		TYPERULE_ItemTime,
		TYPERULE_CostumeFlag,
		TYPERULE_LockItemGM,
	};

	struct SRuleData
	{
		ERule								eRule;
		ETypeRule							eType;
		int									iParam;

		BOOL								bMessage;
		char								szMessage[128];
	};


											ItemBox( int iX, int iY, int _iSlotCountX = 4, int _iSlotCountY = 6 );
	virtual								   ~ItemBox();


	void									SetMaxItems( int iMax ) { iMaxItems = iMax; }
	void									SetFreePlace( BOOL b ) { bFreeSlotPlace = b; }
	void									SetKeepItemValid( BOOL b ) { bKeepItemValid = b; }
	void									SetDisableItemSound( BOOL b ) { bDisableSound = b; }

	int										GetItemsMax() { return iMaxItems; };

	ItemData								* GetLastRenderedItem() { return pcLastRenderedItem; }

	void									SetEventOnPutItem( int iID ) { iPutItemEventID = iID; }
	void									SetEventOnGetItem( int iID ) { iGetItemEventID = iID; }
	void									SetEventOnDeniedItem( int iID ) { iDeniedItemEventID = iID; }
	void									SetEventOnAllowedItem( int iID ) { iAllowedItemEventID = iID; }
	void									SetEventBeforeGetItem( int iID ) { iBeforeGetItemEventID = iID; }
	void									SetEventBeforePutItem( int iID ) { iBeforePutItemEventID = iID; }

	void									SetEventOnExpireItem( int iID ) { iBeforeExpiredItem = iID; }

	void									AddRule( ERule eRule, ETypeRule eType, int iParam );

	void									HandleEvent( ItemData * pcItemData, int iEventID );

	void									ClearItems();

	void									Clear();

	ItemData								* GetEventItemData(){return pcLastItemDataPtr;}

	//Render
	void									Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );

	void									Update( float fTime );

	//Event Handlers
	BOOL									OnMouseClick( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );
	void									OnMouseMove( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );

	void									Enable() { bDisable = FALSE; };
	void									Disable() { bDisable = TRUE; };
	void									EnableMouse() { bDisableClick = FALSE; };
	void									DisableMouse() { bDisableClick = TRUE; };

	BOOL									AddItem( ItemData * pcItemData );
	BOOL									AddItem( ItemData * pcItemData, int iX, int iY, int iMouseX = 0, int iMouseY = 0 );
	BOOL									GetItem( Point2D * psPosition, int iX, int iY, BOOL bAutomatic = FALSE );
	BOOL									GetLastItemOnMouse( BOOL bAutomatic = FALSE );
	BOOL									RemoveItem( int iItemID, int iChk1, int iChk2 );

	ItemData								* GetItemData( int iItemID, int iChk1, int iChk2 );
	ItemData								* GetItemOnPosition( Point2D * psPosition, int iX, int iY );

	std::vector<ItemData*>					& GetItems() { return vItems; }

	void									ResetLastRender() { iLastRenderX = 0; iLastRenderY = 0; }

	void									SetNoCheckSlot( BOOL b ) { bNoCheckSlot = b; }

	void									SetEnableRightClick( BOOL b ) { bEnableRightClick = b; }

	void									SetSwapItemSlot( BOOL b ) { bSwapItemSlot = b; }

private:

	BOOL									IsAllowedItem( ItemData * pcItemData );

	BOOL									bDisable = FALSE;

	BOOL									bFreeSlotPlace = FALSE;

	BOOL									bNoCheckSlot = FALSE;

	Point2D									sPosition;
	int										iSlotCountX, iSlotCountY;
	int										iMaxItems;

	BOOL									bDisableClick = FALSE;

	BOOL									bKeepItemValid = FALSE;
	BOOL									bNoPlaceItemUse = FALSE;
	BOOL									bDisableSound = FALSE;

	std::vector<ItemData*>					vItems;
	std::vector<SRuleData*>					vRules;


	int										iPutItemEventID		= -1;
	int										iGetItemEventID		= -1;
	int										iDeniedItemEventID	= -1;
	int										iAllowedItemEventID = -1;

	int										iBeforePutItemEventID = -1;
	int										iBeforeGetItemEventID = -1;

	int										iBeforeExpiredItem = -1;

	ItemData								* pcLastItemDataPtr = NULL;
	ItemData								* pcLastRenderedItem = NULL;

	int										iLastRenderX = 0;
	int										iLastRenderY = 0;

	int										iLastRenderGetItemX = 0;
	int										iLastRenderGetItemY = 0;

	Point2D									sLastItemOnMouse;

	BOOL									bEnableRightClick = TRUE;
	BOOL									bSwapItemSlot = FALSE;
};
}