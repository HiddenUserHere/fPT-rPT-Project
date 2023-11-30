#pragma once

class CDialog
{
public:
							  CDialog( USHORT _iResourceID );
	virtual					 ~CDialog();

	virtual BOOL			  Init() = 0;
	virtual void			  Shutdown() = 0;

	HINSTANCE				  GetProcessInstance() { return hInstance; }
	HWND					  GetWindowHandle() { return hWnd; }

protected:
	BOOL					  MakeDialog();

	static INT_PTR CALLBACK	  StaticDlgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual INT_PTR			  DlgProc( UINT uMsg, WPARAM wParam, LPARAM lParam ) = 0;

	void					  RemoveDialog();

protected:
	HINSTANCE				  hInstance;
	HWND					  hWnd;

	USHORT					  iResourceID;

};