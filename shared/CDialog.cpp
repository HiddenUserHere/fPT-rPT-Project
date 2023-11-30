#include "stdafx.h"
#include "CDialog.h"

#include "CCore.h"

CDialog::CDialog( USHORT _iResourceID )
{
	hWnd		= NULL;

	iResourceID	= _iResourceID;
}

CDialog::~CDialog()
{
}

BOOL CDialog::MakeDialog()
{
	hInstance = GetModuleHandle( "game.dll" );

	hWnd = CreateDialogParam( hInstance, 
							  MAKEINTRESOURCE( iResourceID ), 
							  NULL, 
							  StaticDlgProc, 
							  (LPARAM)this );

	if( hWnd )
	{
		ShowWindow( hWnd, SW_SHOW );
		UpdateWindow( hWnd );

		ShowCursor( TRUE );
	}

	return hWnd != NULL;
}

void CDialog::RemoveDialog()
{
	if( hWnd )
		DestroyWindow( hWnd );

	hWnd = NULL;
}

INT_PTR CALLBACK CDialog::StaticDlgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	CDialog * pcParent = NULL;

	if( uMsg == WM_INITDIALOG )
	{
		pcParent = (CDialog *)lParam;
		pcParent->hWnd = hWnd;

		SetWindowLong( hWnd, GWL_USERDATA, (LONG)pcParent );

		//Set Icon
		SendMessage( hWnd, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon( pcParent->GetProcessInstance(), MAKEINTRESOURCE( IDI_ICON2 ) ) );
		SendMessage( hWnd, WM_SETICON, ICON_BIG,   (LPARAM)LoadIcon( pcParent->GetProcessInstance(), MAKEINTRESOURCE( IDI_ICON2 ) ) );
	}

	if( pcParent == NULL )
		pcParent = (CDialog *)GetWindowLongPtr( hWnd, GWL_USERDATA );

	if( pcParent == NULL )
		return DefWindowProc( hWnd, uMsg, wParam, lParam );

	return pcParent->DlgProc( uMsg, wParam, lParam );
}