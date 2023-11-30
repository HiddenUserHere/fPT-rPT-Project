#pragma once
class CCoinShopWindow
{
protected:

	enum
	{
		WINDOWID_ItemData,
		WINDOWID_ItemInfo,
		WINDOWID_ItemStatus,
		WINDOWID_ItemPreview,

		BUTTONID_Credits,
		BUTTONID_Close,
		BUTTONID_Help,
		BUTTONID_Buy,

		LABELID_Name,
		LABELID_Price,
		LABELID_DiscountBulk,

		LISTID_Items,
		LISTID_Tabs,
		LISTID_ChildTabs,

		GROUPID_TabButtons,
		GROUPID_ChildTabButtons,

		TEXTID_Credits,
		INPUTID_ChangeNick,

		COUNTID_Quantity,

		COMBOBOX_Spec,
		COMBOBOX_Swap,
		COMBOBOX_ClassChange,

		CHECKBOX_ChangeClassPremium,

		ITEMBOX_Perfectize,
		ITEMBOX_Swapper,

		UNITMESHID_HeadPreview,
	};

	enum EMoveChildTab
	{
		CHILDTABMOVE_Left,
		CHILDTABMOVE_Right,
	};

	UI::Image										* pcImageCoinShopBackground = nullptr;
	UI::Image										* pcImageTimeShopBackground = nullptr;

	int												iCurChildTab = 0;
	int												iCountChildTab = 0;
	int												iSelectedSpec = 0;
	int												iSelectedSwap = 0;
	int												iItemAgingRecoveryID = 0;

	BOOL											bBuying = FALSE;

	struct DefinitionItem							sItemDefinition;

	UI::Window_ptr									pWindow1 = NULL;

	std::vector<UI::Text_ptr>						vLeftText, vRightText;

	void											AddString( int iSide, const std::string & strString, DWORD dwColor = -1, BOOL bBold = FALSE );

	BOOL											CheckSlots();

	void											OnBuyCredits( UIEventArgs eArgs );
	void											OnBuyAgingRecovery( int iPrice, UIEventArgs eArgs );
	void											OnItemNotAllowedPerfectize( UIEventArgs eArgs );
	void											OnItemNotAllowedSwapper( UIEventArgs eArgs );
	void											OnPutItemPerfectize( UIEventArgs eArgs );
	void											OnPutItemSwapper( UIEventArgs eArgs );
	void											OnGetItemPerfectize( UIEventArgs eArgs );
	void											OnGetItemSwapper( UIEventArgs eArgs );


	void											OnCreditsButtonClick( UIEventArgs eArgs );
	void											OnCloseButtonClick( UIEventArgs eArgs );
	void											OnHelpButtonClick( UIEventArgs eArgs );
	void											OnItemButtonClick( class CCoinShopItem * pcItem, UIEventArgs eArgs );
	void											OnAgingRecoveryItemButtonClick( int iID, int iPrice, UIEventArgs eArgs );
	void											OnChangeSpecSelect( class CCoinShopItem * pcItem, UIEventArgs eArgs );
	void											OnChangeSwapSelect( class CCoinShopItem * pcItem, UIEventArgs eArgs );
	void											OnChangeQuantityItem( class CCoinShopItem * pcItem, UIEventArgs eArgs );
	void											OnChangeLevelUPItem( UIEventArgs eArgs );
	void											OnBuyButtonClick( class CCoinShopItem * pcItem, UIEventArgs eArgs );
	void											OnBuyServiceButtonClick( int iType, UIEventArgs eArgs );
	void											OnCheckPremiumClass( int iOldPrice, UIEventArgs eArgs );
	void											OnParentTabButtonClick( class CCoinShopTab * pcTab, UIEventArgs eArgs );
	void											OnChildTabButtonClick( class CCoinShopTab * pcTab, UIEventArgs eArgs );
	void											OnChildMoveButtonClick( EMoveChildTab eType, UIEventArgs eArgs );
	void											OnRotateUnitMesh( int iRotateType );

	void											BuildWindow1();
public:
	BOOL											bItemData = FALSE;
	void											Init();
	void											Render();

	BOOL											IsBuying() { return bBuying; }
	void											SetBuying( BOOL b ) { bBuying = b; }

	struct DefinitionItem 			    		  * GetItemDefinition() { return &sItemDefinition; }

	void											ClearParentTabList();
	void											ClearChildTabList();
	void											ClearItemsList();
	void											ClearString();

	void											ClearLists();
	void											ClearQuantity();

	void											DeleteAgingRecoveryItem( int iID );

	void											SetItemStatus( struct DefinitionItem * psItemDefinition, char * pszName, char * pszDescription );

	BOOL											IsOpen() { return pWindow1->IsOpen(); }

	void											Open();
	void											Close() { pWindow1->Hide(); };

	void											AddChildTab( class CCoinShopTab * pcTab );
	void											AddTab( class CCoinShopTab * pcTab );

	void											Load( class CCoinShopTab * pcTab );
	void											Load( class CCoinShop * pcCoinShop );
	void											BuildItemNormal( class CCoinShopItem * pcItem, BOOL bSpec = FALSE, BOOL bQuantity = FALSE );
	void											BuildPerfectize( BOOL bSpec = FALSE );
	void											BuildSwapper( BOOL bSpec = FALSE );
	void											BuildLevelUP();
	void											BuildChangeNick( int iPrice );
	void											BuildAgingRecovery( struct AgingRecoveryDataHandle * psaData, int iCount, int iPrice );
	void											BuildPreviewCostume( UI::Window_ptr pcWindowItem );
	void											BuildPreviewHead( UI::Window_ptr pcWindowItem );
	void											BuildChangeClass( int iPrice );

	void											HandleItemService( struct PacketCoinServiceBuy * psPacket, ItemData * pcItemData );

	void											SetCoinAmount( int iValue, BOOL bCoins );

	void											OnCheckPreviewCostume( UI::CheckBox_ptr p );
	void											OnResolutionChanged();

	void											OnMouseMove( class CMouse * pcMouse );
	BOOL											OnMouseClick( class CMouse * pcMouse );
	BOOL											OnMouseScroll( class CMouse * pcMouse );

	BOOL											OnKeyPress( class CKeyboard * pcKeyboard );
	BOOL											OnKeyChar( class CKeyboard * pcKeyboard );

									
};

