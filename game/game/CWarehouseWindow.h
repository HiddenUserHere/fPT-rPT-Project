#pragma once
class CWarehouseWindow
{
private:
	enum
	{
		BUTTONID_Left,
		BUTTONID_Right,
		
		TextID_Pages,
		TEXTWINDOW_Quest,

		BUTTONID_QuestOK,
		
	};

	UI::Window_ptr								pWindow1		= NULL;
	UI::Window_ptr								pWindowQuest1	= NULL;

	BOOL										bButtonClick = FALSE;

	void										BuildWindow1();
	void										OnButtonLeftClick(UIEventArgs eArgs);
	void										OnButtonRightClick(UIEventArgs eArgs);
	void										OnButtonQuestOkClick(UIEventArgs eArgs);
	void										OnAcceptQuest(UIEventArgs eArgs);
public:
	CWarehouseWindow();
	virtual ~CWarehouseWindow();

	void										Init();

	void										Render();

	void										OnMouseMove( class CMouse * pcMouse );
	BOOL										OnMouseClick( class CMouse * pcMouse );

	void										OpenWindowQuest2Page();

	void										BuildWindowQuest();

};

