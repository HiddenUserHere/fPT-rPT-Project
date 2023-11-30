#pragma once
class CItemBoxWindowHandler
{
private:

	friend class CItemBoxHandler;

public:


	enum EWindowItemBoxID
	{
		WINDOWID_Main,
		WINDOWID_Receive,
		WINDOWID_Send,
	};

	CItemBoxWindowHandler();
	virtual ~CItemBoxWindowHandler();

	void							Init();

	void							Open( EWindowItemBoxID iWindowID );
	void							Close();

	BOOL							IsOpen() { return pWindow1->IsOpen(); }


	void							OnMouseMove( CMouse * pcMouse );
	BOOL							OnMouseClick( CMouse * pcMouse );

	BOOL							OnKeyPress( CKeyboard * pcKeyboard );
	BOOL							OnKeyChar( CKeyboard * pcKeyboard );

	void							Render();
	void							Update( float fTime );
	void							OnResolutionChanged();

	void							ClearReceiveWindow();
	void							ShowEmptyReceiveWindow();

	void							AddItemReceive( const char * szSenderName, SYSTEMTIME sDate, ItemData * pcItemData );
	void							RemoveItem( int iItemID, int iChk1, int iChk2 );

	void							ItemBoxSendBlock( BOOL bBlock );

private:

	enum
	{
		WINDOW_Main,
		WINDOW_Receive,
		WINDOW_ReceiveEmpty,
		WINDOW_ReceiveItems,
		WINDOW_Send,

		BUTTON_Send,
		BUTTON_Receive,

		BUTTON_SendItem,
		BUTTON_AcceptItem,

		INPUT_CharacterName,

		ITEMBOX_Items,

		TEXT_Sender,
		TEXT_GoldEXP,
	};

	UI::Window_ptr					pWindow1;

	EWindowItemBoxID				iWindowID;

	float							fItemBoxSendBlockTime = 0.0f;

	void							BuildWindow1();

	void							BuildWindowMain();
	void							BuildWindowReceive();
	void							BuildWindowSend();

	void							OnButtonSendItemWindowClick( UIEventArgs eArgs );
	void							OnButtonRetriveItemWindowClick( UIEventArgs eArgs );
	void							OnButtonBackClick( UIEventArgs eArgs );
	void							OnButtonCloseClick( UIEventArgs eArgs );

	void							OnButtonGetItemClick( UIEventArgs eArgs );
	void							OnButtonDiscardItemClick( UIEventArgs eArgs );
	void							OnAcceptDiscardItemClick( UIEventArgs eArgs );

	void							OnButtonSendItemClick( UIEventArgs eArgs );
	void							OnAcceptSendItemClick( UIEventArgs eArgs );

	void							HandleReceiveItemReply( BOOL bGetItem );
};

