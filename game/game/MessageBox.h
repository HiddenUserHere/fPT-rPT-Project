#pragma once

class CMessageBox
{
public:
	CMessageBox();
	virtual ~CMessageBox();

	enum
	{
		WINDOWID_Main,
		WINDOWID_RollDice,

		//Box Type
		BOXTYPEID_YesNo,
		BOXTYPEID_OkCancel,
		BOXTYPEID_RollDice,
		BOXTYPEID_PersonalShop,
		BOXTYPEID_PersonalShopMessage,
		BOXTYPEID_PersonalShopBuy,

		//Buttons
		BUTTONID_Yes,
		BUTTONID_No,

		//Texts
		TextID_Title,
		TextID_Text,
		TEXTID_AutoClose,
		TEXTID_Price,

		//Input
		INPUTID_ItemPrice,
		INPUTID_MessagePersonalShop,

		//Checkbox
		CHECKBOXID_CanBuyItem,
		CHECKBOXID_SellByCreditsItem,

		//Counter
		COUNTID_Quantity,
	};

	void												Init();

	void												SetBoxType( int iType = 1 );


	void												SetEvent( std::function<void( UIEventArgs const & )> const & f );

	void												SetTitle( std::string strText );
	void												SetDescription( std::string strText );

	void												SetAutoClose( int iDuration );

	void												SetItemPriceMax( int iMinPrice, int iMaxPrice = 0 );
	void												SetItemCounter( int iMax );
	BOOL												CanBuyItemPersonalShop();
	BOOL												IsSellByCreditsItemPersonalShop();
	int													GetPriceItem();
	int													GetCountItem();

	std::string											GetPersonalShopMessage();
	void												SetPersonalShopMessageLength( int iLength );

	void												AddExtraText( std::string strText, Rectangle2D sBox, BOOL bBold, UI::Color cColor );
	void												AddExtraElement( Element_ptr p ) { if ( pWindow1 )pWindow1->AddElement( p ); vExtraElements.push_back( p ); }

	BOOL												GetType(){ return bType; }

	BOOL												IsOpen(){ if( pWindow1 )return pWindow1->IsOpen(); return FALSE; }

	void												Show();

	void												OnMouseMove( class CMouse * pcMouse );
	BOOL												OnMouseClick( class CMouse * pcMouse );

	BOOL												OnKeyPress( class CKeyboard * pcKeyboard );
	BOOL												OnKeyChar( class CKeyboard * pcKeyboard );

	int													GetElapsedTime(){ return iElapsedTime; }

	void												Render();
	void												Update();

	void												ClearExtraElements();
protected:
	UI::Window_ptr										pWindow1	= NULL;

	UI::Window_ptr										pWindow1Main = NULL;
	UI::Window_ptr										pWindow1RollDice = NULL;
	UI::Window_ptr										pWindow1PersonalShop = NULL;
	UI::Window_ptr										pWindow1PersonalShopSeller = NULL;
	UI::Window_ptr										pWindow1PersonalShopBuyer = NULL;
	int													iBoxType	= 1;
	BOOL												bType		= FALSE;

	UI::Image										  * pAutoCloseBar[3];

	BOOL												bAutoClose	= FALSE;
	int													iElapsedTime = 0;
	DWORD												dwTickTime = 0;

	std::vector<Element_ptr>							vExtraElements;

	void												OnYesButtonClick( UIEventArgs eArgs );
	void												OnNoButtonClick( UIEventArgs eArgs );
	void												OnRollButtonClick( UIEventArgs eArgs );
	void												OnPassButtonClick( UIEventArgs eArgs );
};

