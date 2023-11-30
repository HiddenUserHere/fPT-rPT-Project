#pragma once
class DailyQuestWindow
{
public:
												DailyQuestWindow();
	virtual										~DailyQuestWindow(); 

	void										OnMouseMove( class CMouse * pcMouse );
	BOOL										OnMouseClick( class CMouse * pcMouse );

	BOOL										OnKeyPress( class CKeyboard * pcKeyboard );
	BOOL										OnKeyChar( class CKeyboard * pcKeyboard );

	void										Render();
	void										OnResolutionChanged();

	void										Init();
	void										Open();
	void										Close( UIEventArgs eArgs );
	void										OpenSubWindow( DWORD dwQuestID );

	enum
	{
		// Windows
		WINDOWID_Main,
		TEXTWINDOWID_Sub,

		TEXTWINDOWID_WaitQuest,
	};
protected:
	UI::Window_ptr								pWindow1 = NULL;
	UI::Window_ptr								pWindow2 = NULL;
	UI::Window_ptr								pWindow3 = NULL;
	DWORD										dwCurrentQuestID = 0;
	void										BuildWindow1();

	void										OnSelect80A( UIEventArgs eArgs );
	void										OnSelect85A( UIEventArgs eArgs );
	void										OnSelect90A( UIEventArgs eArgs );
	void										OnSelect95A( UIEventArgs eArgs );
	void										OnSelect100A( UIEventArgs eArgs );
	void										OnSelect105A( UIEventArgs eArgs );
	void										OnSelect110A( UIEventArgs eArgs );
	void										OnSelect115A( UIEventArgs eArgs );
	void										OnClickOK( UIEventArgs eArgs );
	void										OnClickYesBox(UIEventArgs eArgs);
	void										OnClickClose( UIEventArgs eArgs );
};

