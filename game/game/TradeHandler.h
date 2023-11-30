#pragma once

#define TRADEUNIT_ACCEPT					(*(BOOL*)0x03692400)
#define TRADEUNIT_WEIGHT					(*(int*)0x036923FC)
#define TRADE_ACCEPT						(*(BOOL*)0x0369D820)
#define TRADE_CHANGECOUNT					(*(int*)0x03A976E0)
#define TRADE_UNITID						(*(int*)0x03706F44)
#define TRADE_MASKTIME						(*(DWORD*)0x03A976DC)

class CTradeHandler
{
public:
	CTradeHandler();
	virtual ~CTradeHandler();


	void													Init();


	void													OnMouseMove( class CMouse * pcMouse );
	BOOL													OnMouseClick( class CMouse * pcMouse );

	BOOL													OnKeyChar( CKeyboard * pcKeyboard );

	void													Render();

	BOOL													IsValidTrade() { return CALLT( 0x0051DC10, 0x036EFD98 ); }

	BOOL													IsTradeSent() { return (*(BOOL*)0x03A976D8); }
	void													IsTradeSent(BOOL b) { (*(BOOL*)0x03A976D8) = b; }

	BOOL													IsWindowTradeOpen() { return (*(BOOL*)(0x036EFD98)); }
	void													IsWindowTradeOpen(BOOL b) { (*(BOOL*)(0x036EFD98)) = b; }

	BOOL													IsTradeMaskTime() { return (READDWORD( 0x03A976DC ) != 0); }
	BOOL													CanCloseTradeWindow() { return ((IsTradeMaskTime() == FALSE) || (READDWORD( 0x036EFCCC ) != 0)); }

	void													CloseTradeWindow();

	void													SetCheckTradeButton( BOOL bCheck );

	void													HandlePacket( struct PacketCoinsTraded * psPacket );
	void													HandlePacket( struct PacketTradeCoinSet * psPacket );

	void													ProcessPacket( struct PacketTradeCoinSet * psPacket );

	ItemData												* GetMouseItemHover();
	ItemData												* GetMouseItemHoverOther();

	BOOL													IsDelayTime();

	static BOOL												SendTradeCheckItem( int iID );

private:

	void									OnButtonCloseClick( UIEventArgs e );
	void									OnButtonCheckTradeClick( UIEventArgs e );
	void									OnButtonCoinClick( UIEventArgs e );
	void									OnInputCoinClick( UIEventArgs e );

	void									UpdateCoins();

	enum
	{
		WINDOW_Main,
		BUTTONID_Close,
		BUTTONID_TradeCheckSelf,

		IMAGEID_TradeCheckUnit,

		TEXTID_WeightUnit,
		TEXTID_CoinSelf,
		TEXTID_CoinUnit
	};

	UI::Window_ptr							pWindow = NULL;

	UI::Image								* pImageTradeCancel = NULL;
	UI::Image								* pImageTradeAccept = NULL;

	int										iMaxCoins	= 0;
	int										iSelfCoins	= 0;
	int										iUnitCoins	= 0;

	DWORD									dwTradeTimeDelay = 0;
};

