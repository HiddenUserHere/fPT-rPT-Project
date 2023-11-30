#pragma once

class CAbyssQuestWindow
{
public:
	CAbyssQuestWindow();
	virtual ~CAbyssQuestWindow();

	void							BuildWindow();

	void							Init();

	void							Render();

	void							Open();
	void							Close();

	void							OnMouseMove( class CMouse * pcMouse );
	BOOL							OnMouseClick( class CMouse * pcMouse );
	BOOL							OnMouseScroll( class CMouse * pcMouse );

	void							OnResolutionChanged();
private:
	void							OnButtonOpenQuestWindow( int iID, UIEventArgs e );

	void							OnClickOK( UIEventArgs e );
	void							OnClickClose( UIEventArgs e );
	void							OnClickCloseMain( UIEventArgs e );
	void							OnClickYesBox( UIEventArgs e );

	BOOL							CanOpenNPC();

	BOOL							IsActiveQuests();

	BOOL							IsFinishedQuest( int iID );

	void							OnClickOKFinish( UIEventArgs e );


protected:
	UI::Window_ptr					pWindow1;
	UI::Window_ptr					pWindow2;
	UI::Window_ptr					pWindow3;


	int								iCurrentQuestID = 0;

	enum
	{
		WINDOW_Main,

		WINDOW_ChildMain,

		WINDOW_Quest1,
		WINDOW_Quest2,
		WINDOW_Quest3,
		WINDOW_Quest4,
		WINDOW_Quest5,
		WINDOW_Quest6,
		WINDOW_Quest7,

		TEXTWINDOWID_Sub,
	};
};

