#include "stdafx.h"
#include "CServerWindow.h"

#include "CServer.h"
#include "CConfigFileReader.h"

CServerWindow::CServerWindow() : CWindow()
{
	pConfig = new CServerConfig( "server.ini" );
	pServer = new CServer();
}

CServerWindow::~CServerWindow()
{
	DELET( pServer );
	DELET( pConfig );
}

UINT CServerWindow::Init()
{
	//LOGEx( "SERVER", "NOTICE : Init" );

	if( !Register( "Server", CS_HREDRAW | CS_VREDRAW, IDI_ICON, (HBRUSH)( COLOR_WINDOW ) ) )
	{
		WRITEERR( "Register() failed!\n\nGetLastError returned %d", GetLastError() );
		return 0;
	}

	if( !MakeWindow( "Fortress World Server", TRUE, FALSE, FALSE, WINDOW_WIDTH, WINDOW_HEIGHT ) )
	{
		WRITEERR( "MakeWindow() failed!" );
		return 0;
	}
	
	Server::CreateInstance();
	Server::GetInstance()->Init( hWnd );
	Server::GetInstance()->Load();

	if( !pServer->Init() )
		return FALSE;
	
	Server::GetInstance()->Begin();

	bExit = FALSE;

	//Load Settings
	CConfigFileReader * pcConfigFileReader = new CConfigFileReader( "server.ini" );
	if( pcConfigFileReader->Open() )
	{
		dwUpdateTimeInterval = 128;
		if ( (LOGIN_SERVER == FALSE) && GAME_SERVER )
			dwUpdateTimeInterval = 16;


		pcConfigFileReader->Close();
	}
	DELET( pcConfigFileReader );

	return 1;
}

BOOL CServerWindow::Shutdown()
{
	//LOGEx( "SERVER", "NOTICE : Shutdown" );

	bExit = TRUE;

	Server::GetInstance()->End();

	SHUTDOWN( pServer );

	Server::GetInstance()->UnLoad();
	Server::GetInstance()->UnInit();
	Server::DeleteInstance();

	RemoveWindow();
	Unregister();

	return FALSE;
}

BOOL CServerWindow::Run()
{
	//LOGEx( "SERVER", "NOTICE : Run" );

	LoopWindowMessages();

	bExit = TRUE;

	return TRUE;
}

LRESULT CServerWindow::WndProc( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	/* System Defined Message */
	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;

	case WM_PAINT:
		Render();
		break;

	/* User Defined Messages */
	case WM_UPDATE:
		Update( *(double*)wParam );
		break;

	case WM_SOCKETACCEPT:
		SOCKETACCEPT( (SOCKET)wParam, (SocketServerAccept *)lParam );
		break;
		
	case WM_SOCKETPACKET:
		SOCKETPACKET( (SocketData *)wParam, (PacketReceiving *)lParam );
		break;

	case WM_SOCKETCLOSE:
		SOCKETCLOSE( (SocketData *)wParam );
		break;

	/* Unhandled Messages */
	default:
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
		break;
	}

	return FALSE;
}

void CServerWindow::Update( double fTime )
{
	static double fTick = (1000.0f / ((double)64));
	static double fOffs = 0.0f;

	SERVER_MUTEX->Lock( 3000 );

	//Split time into frames
	fOffs += fTime;
	if( fOffs >= fTick )
	{
		do
		{
			GSERVER->Loop();
			pServer->Update();

			fOffs -= fTick;
		} while( fOffs >= fTick );
	}

	GSERVER->Time( fTime );

	SERVER_MUTEX->Unlock();
}

void CServerWindow::Render()
{
	static HFONT hFont = NULL;

	PAINTSTRUCT sPS;
	HDC hDC;
	RECT sRect;

	hDC = BeginPaint( hWnd, &sPS );

	if( hFont == NULL )
		hFont = CreateFontA( 18, 0, 0, 0, FW_ULTRALIGHT, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH, "Arial" );

	SetBkMode( hDC, TRANSPARENT );
	SetTextColor( hDC, RGB( 0, 0, 0 ) );
	SelectObject( hDC, (HGDIOBJ)hFont );

	switch( SERVER_TYPE )
	{
	case SERVERTYPE_Login:
		sRect.left		= 4;
		sRect.top		= 6;
		sRect.right		= WINDOW_WIDTH;
		sRect.bottom	= sRect.top + 20;
		DrawTextA( hDC, "[Login Server] ONLINE!", -1, &sRect, DT_LEFT );
		break;
	case SERVERTYPE_Game:
		sRect.left		= 4;
		sRect.top		= 6;
		sRect.right		= WINDOW_WIDTH;
		sRect.bottom	= sRect.top + 20;
		DrawTextA( hDC, "[Game Server] ONLINE!", -1, &sRect, DT_LEFT );
		break;
	case SERVERTYPE_Multi:
		sRect.left		= 4;
		sRect.top		= 6;
		sRect.right		= WINDOW_WIDTH;
		sRect.bottom	= sRect.top + 20;
		DrawTextA( hDC, "[Multi Server] ONLINE!", -1, &sRect, DT_LEFT );
		break;
	default:
		sRect.left		= 4;
		sRect.top		= 6;
		sRect.right		= WINDOW_WIDTH;
		sRect.bottom	= sRect.top + 20;
		DrawTextA( hDC, "Loading Server...", -1, &sRect, DT_LEFT );
		break;
	};

	EndPaint( hWnd, &sPS );
}