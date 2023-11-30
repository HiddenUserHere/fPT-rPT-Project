#pragma once

#define PERSONALSHOP_WINDOW_WIDTH			450
#define PERSONALSHOP_WINDOW_HEIGHT			264
#define PERSONALSHOP_WINDOW_TIMEUPDATE		500.0f

class CPersonalShopWindowHandler
{
public:
	CPersonalShopWindowHandler();
	virtual ~CPersonalShopWindowHandler();

	void						Init();

	BOOL						IsOpen();

	void						Open( bool bSelfShop );
	void						Close();
	void						ClearItems();
	
	void						Render();
	
	void						Update( float fTime );

	void						OnMouseMove( CMouse * pcMouse );

	BOOL						OnMouseClick( CMouse * pcMouse );

	BOOL						OnKeyChar( CKeyboard * pcKeyboard );


	void						AddItem( ItemData * pcItemData, bool bSelf = true );

	void						RemoveItem( int iItemID, int iChk1, int iChk2 );
	ItemData					* GetItem( int iItemID, int iChk1, int iChk2 );
	ItemData					* GetLastRenderedItem();

	std::vector<ItemData*>		& GetItems();

	BOOL						IsSellingTime() { return bIsSellingTime; }

	bool						IsSeller() { return (iShopType == PERSONALSHOPTYPE_Seller); }

private:
	enum
	{
		WINDOW_Main,

		ITEMBOX_Shop,

		BUTTON_StartSell,
		BUTTON_StopSell,

		BUTTON_Buy,

		BUTTON_Close,
		BUTTON_State,

		TEXT_Gold1,
		TEXT_Gold2,

	};

	enum EWindowState
	{
		WINDOWSTATE_None,

		WINDOWSTATE_Opening,
		WINDOWSTATE_Minimizing,
		WINDOWSTATE_Closing,
	};

	enum EPersonalShopType
	{
		PERSONALSHOPTYPE_None,
		PERSONALSHOPTYPE_Seller,
		PERSONALSHOPTYPE_Buyer,
	};

	void						BuildWindow();

	void						OnButtonStateClick( UIEventArgs eArgs );
	void						OnButtonCloseClick( UIEventArgs eArgs );
	void						OnButtonStartSellClick( UIEventArgs eArgs );
	void						OnButtonStopSellClick( UIEventArgs eArgs );

	void						OnButtonBuyClick( UIEventArgs eArgs );

	void						SetWindowState( EWindowState iState );

	void						OnBeforePutItemEvent( UIEventArgs eArgs );
	void						OnBeforeGetItemEvent( UIEventArgs eArgs );
	void						OnGetItemEvent( UIEventArgs eArgs );
	void						OnAcceptSellItem( ItemData * pcItemData, UIEventArgs eArgs );
	void						OnAcceptGetItem( UIEventArgs eArgs );

	void						OnAcceptBuyItem( ItemData * pcItemData, UIEventArgs eArgs );

	void						OnAcceptStartSelling( UIEventArgs eArgs );

	UI::Window_ptr				pWindow1;

	EWindowState				iWindowState = WINDOWSTATE_None;

	EPersonalShopType			iShopType = PERSONALSHOPTYPE_None;

	float						fTimeUpdate = 0.0f;

	DWORD						dwTimeProcessBuy = 0;

	BOOL						bIsSellingTime = FALSE;
};

