#pragma once

#include "CList.h"
#include "CDialog.h"

class CApplication;

#define WS_SERVER			(WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX)
#define WS_EX_SERVER		0 //(WS_EX_TOPMOST)

#define WS_WINDOWED			(WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)
#define WS_EX_WINDOWED		0 //(WS_EX_TOPMOST)

#define WS_FULLSCREEN		(WS_POPUP)
#define WS_EX_FULLSCREEN	0 //(WS_EX_TOPMOST)

class CWindow
{
public:
							  CWindow();
	virtual					 ~CWindow();

	CApplication			* GetApplication() { return pApplication; }
	void					  SetApplication( CApplication * _pApplication ) { pApplication = _pApplication; }

	virtual UINT			  Init() = 0;
	virtual BOOL			  Shutdown() = 0;
	virtual BOOL			  Run() = 0;

	HWND					  GetWindowHandle() { return hWnd; }

	void					  LoadWindowMessages();

	void					  AddDialog( CDialog * pcDialog );
	void					  RemoveDialog( CDialog * pcDialog );

	void					  Exit( BOOL b ) { bExit = b; }
	BOOL					  Exit() { return bExit; }

	void					  Minimized( BOOL b ) { bMinimized = b; }
	BOOL					  Minimized() { return bMinimized; }

protected:
	BOOL					  Register( const char * pszClassName, UINT uStyle, UINT uIcon, HBRUSH hbrBackground );
	void					  Unregister();

	BOOL					  MakeWindow( LPCSTR lpszTitle, BOOL bServer = FALSE, BOOL bWindowed = FALSE, BOOL bMaximized = FALSE, int iWidth = 0, int iHeight = 0 );

	BOOL					  PumpWindowMessages();
	void					  LoopWindowMessages();

	BOOL					  IsActive( BOOL bIncludeDialogs = TRUE );

	BOOL					  IsDialog( HWND hWnd );
	BOOL					  ProcessMessage( MSG & msg );

	static LRESULT CALLBACK	  StaticWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual LRESULT			  WndProc( UINT uMsg, WPARAM wParam, LPARAM lParam ) = 0;

	virtual void			  Update( double fTime ) = 0;
	static DWORD WINAPI		  UpdaterThread( CWindow * pThis );

	void					  RemoveDialogs();
	void					  RemoveWindow();

protected:
	CApplication			* pApplication;

	BOOL					  bRegisterSuccess;
	char					  szClassName[256];

	HWND					  hWnd;

	std::vector<CDialog*>	  vDialogs;

	DWORD					  dwUpdateTimeInterval;

	BOOL					  bExit;
	BOOL					  bRunning;
	BOOL					  bMinimized;

};