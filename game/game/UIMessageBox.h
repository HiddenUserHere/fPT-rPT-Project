#pragma once
#include "UIController.h"
#include "UIEvent.h"
//#include "UI.h"

class CUIMessageBox
{
public:
	CUIMessageBox( int iType, const char * pszTitle, const char * pszText );
	virtual ~CUIMessageBox();

	enum
	{
		IDTYPE_YesNo,
		IDTYPE_OkCancel,


		// IDS
		WINDOW_ID,
		EBUTTON_1,
		EBUTTON_2,

	};

	void												SetAutomaticCloseTimer( float fTime, EButton eButton );
	void												SetEvent( int iEvent );
	void												SetCloseEvent( int iEvent );

	BOOL												OnMouseClick( EButton eButton );

	void												Draw();

private:
	void												OnAutomaticClose( UIEventArgs eArgs );
	void												OnEventClose();

	void												AddButton( Rectangle2D rect, EImage eImage1, EImage eImage2, EButton eButton );

	void												CreateType();

	int													GetWidthButton( EButton eButton );
	int													GetHeightButton( EButton eButton );
	int													GetXButton( EButton eButton );
	int													GetYButton( EButton eButton );

protected:

	int													eButtonAutomaticFire = EBUTTON_2;

	int													iTypeBox = 0;

	int													iEventMsgBox = -1;
	int													iEventCloseMsgBox = -1;

	std::shared_ptr<class CUIFont>						pFontTitle = NULL;
	std::shared_ptr<class CUIFont>						pFontText = NULL;

	int													iHandlerButton1_1 = -1;
	int													iHandlerButton1_2 = -1;
	int													iHandlerButton2_1 = -1;
	int													iHandlerButton2_2 = -1;

	std::shared_ptr<class CUIController>				pControllerCheckBox = NULL;

	Timer_ptr											pTimer = NULL;
	BOOL												bStartedTime = FALSE;

	BOOL												bClosedMessageBox = FALSE;

	std::string											strText;
	std::string											strTitle;
	float												fTimeBox = 0.0f;

};

