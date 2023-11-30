#pragma once

#include "AbyssQuestWindow.h"

enum EElementsID
{
	//Main Window
	WINDOWID_Main,
	BUTTONID_Ok,
	LISTID_TextWindow,
	WINDOWID_Text,
	TEXTID_Main,

	BUTTONID_LeftArrow,
	BUTTONID_RightArrow,

	LISTID_Items,

	//Ice Mine Window
	LISTID_IM1_Main,
	LISTID_IM1_Menu,
	WINDOWID_IM1_Main,
	WINDOWID_IM1_MineFears,
	WINDOWID_IM1_MineAlone,
	WINDOWID_IM1_Tulla,
	IMAGEID_IM1_Body,
	BUTTONID_IM1_Easy,
	BUTTONID_IM1_Medium,
	BUTTONID_IM1_Hard,
	BUTTONID_IM1_Start,
	GROUPID_IM1_Difficulties,
	TEXTID_IM1_Reward,
	TEXTID_IM1_Difficulty,
	TEXTID_IM1_TextAbout,

	BUTTONID_IM1_TullaStart,
	BUTTONID_IM1_MineAloneStart,

	//Dynamic Quest Window
	WINDOWID_DM_Main,
	LISTID_DM_Main,
};

class CQuestWindow
{
private:
	UI::Window_ptr					pWindow;
	UI::Window_ptr					pWindowItems;
	UI::Window_ptr					pWindowIceMine;
	UI::Window_ptr					pWindowT5Quest;

	UI::Window_ptr					pDynamicWindow;

	std::vector<PacketDynamicQuestOpen::QuestData*> vDynamicQuests;

	int								iCurrentID = 0;
	int								iNPCType = 0;

	int								iItemIndex = 0;

	char							szName[64];

	BOOL							bBlockedQuest = FALSE;


	UI::List_ptr					GetListMineFears();
	UI::Window_ptr					GetWindowMineFears();
	UI::Window_ptr					GetWindowTullaMenace();
	UI::Window_ptr					GetWindowMineAlone();

	UI::List_ptr					GetListMenu();
	UI::Window_ptr					GetWindowMain();

	CAbyssQuestWindow			  * pcAbyssWindow = NULL;

	void							OnButtonAcceptClick( UIEventArgs e );
	void							OnButtonOKClick( UIEventArgs e );
	void							OnButtonOKMineClick( UIEventArgs e );
	void							OnButtonOKT5QuestClick( UIEventArgs e );
	void							OnMessageBoxT5QuestStartClick( UIEventArgs e );

	void							OnButtonLeftArrowClick( UIEventArgs e );
	void							OnButtonRightArrowClick( UIEventArgs e );
	void							OnButtonOKItemClick( UIEventArgs e );
	void							OnButtonCloseClick( UIEventArgs e );
	void							OnButtonCloseMineClick( UIEventArgs e );
	void							OnButtonDifficultyClick( int iID, UIEventArgs e );
	void							OnButtonOpenQuestWindow( int iID, UIEventArgs e );
	void							OnButtonOpenDynamicQuestWindow( int iID, UIEventArgs e );


public:
									CQuestWindow();
	virtual						   ~CQuestWindow();

	void							BuildWindowDQTullaMenace( UI::List_ptr pList );
	void							BuildWindowDQTheMinersFear( UI::List_ptr pList );
	void							BuildWindowSQMineAlone( UI::List_ptr pList );
	void							BuildWindowIceMine();
	void							BuildWindowT5Quest();

	void							BuildDynamicWindow();

	void							Init();

	void							Open( int iID, char * pszName, char * pszDescription, int iNPCType );
	void							ClearDynamicQuest();
	void							SetDynamicQuestMain( const char * pszImageMain, const char * pszTextMain );
	void							AddDynamicQuestMainMenu( PacketDynamicQuestOpen::QuestData * psQuestData );
	void							OpenDynamicQuestWindow();
	void							CloseDynamicQuestWindow();
	void							OpenItems( int iID );

	void							OpenLarry();

	void							OpenT5QuestWindow( BOOL bStart );
	void							CloseT5QuestWindow();

	void							AddItem( ItemData * pcItemData );

	void							Close();

	void							Render();

	void							OnResolutionChanged();

	void							OnMouseMove( class CMouse * pcMouse );
	BOOL							OnMouseClick( class CMouse * pcMouse );
	BOOL							OnMouseScroll( class CMouse * pcMouse );

	CAbyssQuestWindow				* GetAbyssWindow() { return pcAbyssWindow; };
};

