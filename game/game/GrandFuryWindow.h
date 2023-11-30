#pragma once
class CGrandFuryWindow
{
protected:
	UI::Window_ptr											pWindow1 = NULL;
	UI::Window_ptr											pWindow2 = NULL;

	enum
	{
		WINDOWID_Main,
		WINDOWID_ChildMain,
		WINDOWID_ChildAbout,

		LISTID_Main,
		LISTID_About,

		IMAGEID_HeaderNPC,
		IMAGEID_Boss,
		IMAGEID_BackgroundBoss,
		IMAGEID_BarCountdownTime,
		IMAGEID_ArrowTeleport,
		IMAGEID_ArrowExchangeItem,

		TEXTID_Countdown,
		TEXTID_BossName,
		TEXTID_BossLevel,
		TEXTID_ReadMore,
		TEXTWINDOWID_About,
		TEXTID_TeleportCT2,
		TEXTID_ExchangeItem,
		TEXTID_RequestEnter,
	};

private:
	void													BuildWindow1();
	void													BuildWindow2();

	void													OnReadMoreButtonClick( UIEventArgs e );
	void													OnTeleportCT2ButtonClick( UIEventArgs e );
	void													OnTeleportFuryArenaButtonClick( UIEventArgs e );
	void													OnAcceptTeleportCT2ButtonClick( UIEventArgs e );
	void													OnAcceptTeleportFuryArenaButtonClick( UIEventArgs e );
	void													OnAcceptExchangeButtonClick( UIEventArgs e );
	void													OnExchangeButtonClick( UIEventArgs e );
	void													OnCloseButtonClick( UIEventArgs e ) { this->Close(); };
public:
	CGrandFuryWindow();
	virtual ~CGrandFuryWindow();


	void													OnMouseMove( class CMouse * pcMouse );
	BOOL													OnMouseClick( class CMouse * pcMouse );
	BOOL													OnMouseScroll( class CMouse * pcMouse );

	BOOL													OnKeyPress( class CKeyboard * pcKeyboard );
	BOOL													OnKeyChar( class CKeyboard * pcKeyboard );

	void													OnResolutionChanged();

	void													Open( int iNPC );
	void													Close();
	void													SwapTest();

	void													Init();

	void													Render();
};

