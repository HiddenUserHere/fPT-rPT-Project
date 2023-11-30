#include "stdafx.h"
#include "CGameWindow.h"

CGameWindow * CGameWindow::pcInstance;

BOOLEAN BlockAPI( HANDLE hProcess, CHAR * libName, CHAR * apiName )
{
	BYTE pRet[5] = { 0x31, 0xC0, // XOR eax, eax
			  0xC2, 0x10, 0x00 };    // RET
	HINSTANCE hLib = NULL;
	VOID * pAddr = NULL;
	BOOL bRet = FALSE;
	DWORD dwRet = 0;

	hLib = LoadLibrary( libName );
	if ( hLib )
	{
		pAddr = (VOID *)GetProcAddress( hLib, apiName );
		if ( pAddr )
		{
			if ( WriteProcessMemory( hProcess,
				(LPVOID)pAddr,
				(LPCVOID)pRet,
				sizeof( pRet ),
				&dwRet ) )
			{
				if ( dwRet )
				{
					bRet = TRUE;
				}
			}
		}
		FreeLibrary( hLib );
	}
	return bRet;
}

void AntiInject()
{
	HANDLE hProc = GetCurrentProcess();
	while ( TRUE )
	{
		BlockAPI( hProc, AY_OBFUSCATE( "NTDLL.DLL" ), AY_OBFUSCATE( "LdrLoadDll" ) );
		Sleep( 100 );
	}
}


CGameWindow::CGameWindow() : CWindow()
{
	CGameWindow::pcInstance = this;

	pcGame = new CGame();

	pcTimer = NULL;
	uTimerID = 0;

#ifdef _SCITER_LIB_
	pcSciter = new CSciterWindow();
#endif
}


CGameWindow::~CGameWindow()
{
#ifdef _SCITER_LIB_
	DELET( pcSciter );
#endif

	DELET( pcGame );
}

UINT CGameWindow::Init()
{
	SETTINGSHANDLER->GetModel()->ReadSettings();

	//Apply Camera Range
	if ( SETTINGSHANDLER->GetModel()->GetSettings().iCameraRange == 0 )
	{
		WRITEDWORD( 0x03245894, 0 );
	}
	else if ( SETTINGSHANDLER->GetModel()->GetSettings().iCameraRange == 1 )
	{
		WRITEDWORD( 0x03245894, 22 );
	}
	else if ( SETTINGSHANDLER->GetModel()->GetSettings().iCameraRange == 2 )
	{
		WRITEDWORD( 0x03245894, 64 );
	}

	CSettingsModel * p = SETTINGSHANDLER->GetModel();

	DX::Graphic::ResolutionChange( TRUE, p->GetSettings().dwWidth, p->GetSettings().dwHeight );

	if ( !Register( GAME_NAME, CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, 111, (HBRUSH)(GetStockObject( BLACK_BRUSH )) ) )
	{
		ERRORBOX( "Register() failed!\n\nGetLastError returned %d", GetLastError() );
		return 0;
	}

	if ( !MakeWindow( GAME_NAME, FALSE, p->GetSettings().bWindowed, FALSE, p->GetSettings().dwWidth, p->GetSettings().dwHeight ) )
	{
		ERRORBOX( "MakeWindow() failed!\n\nGetLastError returned %d", GetLastError() );
		return 0;
	}

	// Create Edit Window
	HWND hWndEdt = CreateWindowEx( NULL, "EDIT", "", 0x40801004, 10, 400, 500, 40, hWnd, (HMENU)101, GetApplication()->GetProcess()->GetInstanceHandle(), NULL );

	SetWindowTextA( hWndEdt, (char*)0x039033E8 );

	// Copy hWndEdit for Executable
	*(UINT*)0x0A17544 = (UINT)hWndEdt;
	*(UINT*)0x0A15DF4 = SetWindowLong( hWndEdt, GWL_WNDPROC, 0x416810 );
	*(UINT*)0x0A176DC = (UINT)hWndEdt;
	hImc = ImmGetContext( hWnd );
	*(UINT*)0x0A17418 = (UINT)hImc;
	*(UINT*)0x0A15DE0 = (UINT)ImmGetContext( hWndEdt );

	SetTimer( hWnd, 0, 100, 0 );
	SetTimer( hWnd, 0x6A6A6A6A, 850, 0 );
	SetTimer( hWnd, 0x6A6A6A6B, 10, 0 );

	// Initial Context
	CALL_WITH_ARG1( 0x00416780, (DWORD)0 );

	//Audio
	DX::SoundOld_ptr pSound = std::make_shared<DX::SoundOld>();
	pSound->CreateDevices();

	WRITEDWORD( 0x0A17540, hWnd );

	GRAPHICENGINE->InitD3D( hWnd );

	//UglyRefactor::EncryptAll();

	// Init Socket
	SOCKETGAME->Load( hWnd );

	if ( !pcGame->Init() )
		return 0;

	ANTICHEATHANDLER->AddNewThreadException( GetCurrentThreadId() );

	pcGame->Begin();

	if ( FILEEXIST( "testgm.txt" ) )
		GM_MODE = TRUE;

#ifdef _DEBUG
	if( FILEEXIST("testscreen.ini") )
		GAMESCREEN->ChangeScreen( SCREEN_Test );
	else
		GAMESCREEN->ChangeScreen( SCREEN_Login );
#else
	GAMESCREEN->ChangeScreen( SCREEN_Login );
#endif

	bExit		= FALSE;
	bRunning	= FALSE;
	bRender		= TRUE;

	pcTimer		= GAMETIMER;
	uTimerID	= RandomI( 500, 2500 );

	iWindowInactiveFrameSleepTime = GetApplication()->GetSystem()->ComputeFrameDelayTime();

	pcTimer->SetMultiCore( GetApplication()->GetSystem()->IsMultiCore() );
	pcTimer->Update();

	if ( GetApplication()->GetSystem()->IsMultiCore() )
	{
		DWORD_PTR dw = (1 << GetApplication()->GetSystem()->GetNumberOfCores()) - 1;

//		SetProcessAffinityMask( GetCurrentProcess(), dw );
	}

#ifdef _SCITER_LIB_
	pcSciter->Init( GetApplication()->GetProcess()->GetInstanceHandle(), hWnd, Rectangle2D( 0, 0, 800, 600 ) );
#endif

	return 1;
}

BOOL CGameWindow::Shutdown()
{
	SHUTDOWN( pcGame );
	return TRUE;
}

BOOL CGameWindow::Run()
{
	//CGameWindow::HookMouseClicks();

	MEMORYSTATUSEX sGlobalMem;
	sGlobalMem.dwLength = sizeof( MEMORYSTATUSEX );
	if ( GlobalMemoryStatusEx( &sGlobalMem ) )
	{
	//	if ( (sGlobalMem.ullAvailPhys / 1048576) < 1536 )
	//		MessageBoxA( 0, FormatString( "You need more than 1.5GB RAM memory to run the game without any issue!\n\nAvailable: %.1lfGB of %.1lfGB",
	//		((double)sGlobalMem.ullAvailPhys / (double)1073741824), ((double)sGlobalMem.ullTotalPhys / (double)1073741824) ), "Caution!", MB_ICONEXCLAMATION | MB_OK );
	}

	SocketGame::RequestOpenConnection( false, 3000 );

	HWND h = FindWindowA( "TFrmRPTLauncher", NULL );
	if ( h != 0 )
		PostMessageA( h, WM_WINDOWREADY, 0, 0 );

	if( SETTINGSHANDLER->GetModel()->GetSettings().bStartMaximized )
		ShowWindow( hWnd, SW_MAXIMIZE );
	else
		ShowWindow( hWnd, SW_SHOWNORMAL );
	
	WRITEDBG( "Run" );


	//DEP
	SetProcessDEPPolicy( 0 );

	bExit = FALSE;
	bRunning = TRUE;

	//CAntiDebuggerHandler::CreateHiddenThread( (LPTHREAD_START_ROUTINE)&AntiInject, NULL, 0 );

	while ( PumpWindowMessages() )
	{
		//Game Loop
		Loop();

		//Render Lock if Window Inactive
		if ( (!IsActive()) )
		{
			SetIME( false );

			Sleep( iWindowInactiveFrameSleepTime );
		}
		else
			SetIME( true );
	}

	bRunning = FALSE;
	bExit = TRUE;

	return TRUE;
}

LRESULT CGameWindow::WndProc( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	BOOL bUseOld = FALSE;

//	if ( uMsg == WM_NCRBUTTONDOWN || uMsg == WM_NCLBUTTONDOWN )
//		CHARACTERGAME->CancelAttack();

#ifdef _SCITER_LIB_
	pcSciter->WndProc( hWnd, uMsg, wParam, lParam );
#endif

	switch ( uMsg )
	{
		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;

		case WM_SETCURSOR:
			if ( (HWND)wParam == hWnd )
			{
				MOUSEHANDLER->SetCursorGame( MOUSE_ICON );
				return TRUE;
			}
			break;

		case WM_SIZE:
			if ( wParam == SIZE_MAXIMIZED || wParam == SIZE_RESTORED )
				MINIMIZED_GAME = FALSE;
			else
			{
				if ( wParam == SIZE_MINIMIZED )
				{
					MINIMIZED_GAME = TRUE;
				}
			}
			break;


		case WM_KILLFOCUS:
			SetFocus( hWnd );
			return TRUE;

		case WM_CLOSE:
			CALL( 0x0045C35A );
			return 0;

		case WM_MOVE:
			if ( Game::GetGameMode() == GAMEMODE_InGame )
				Loop();

			bUseOld = TRUE;
			break;

		case WM_MOUSEMOVE:
			int iX;
			iX = (int)round( LOWORD( lParam ) );
			int iY;
			iY = (int)round( HIWORD( lParam ) );

			if ( SETTINGSHANDLER->GetModel()->GetSettings().bWindowed && SETTINGSHANDLER->GetModel()->GetSettings().bAutoAdjust == FALSE )
			{
				RECT rc;
				rc.left = 0;
				rc.top = 0;
				rc.right = SETTINGSHANDLER->GetModel()->GetOldSettings().dwWidth;
				rc.bottom = SETTINGSHANDLER->GetModel()->GetOldSettings().dwHeight;
				GetWindowRect( DX::Graphic::GetWindowHandler(), &rc );

				UINT uWindowStyle = GetWindowLongA( DX::Graphic::GetWindowHandler(), GWL_STYLE );

				// Adjust Window
				AdjustWindowRect( &rc, uWindowStyle, FALSE );
				DWORD dwHeight = (rc.bottom + rc.top);

				WINDOWPLACEMENT wc;

				GetWindowPlacement( DX::Graphic::GetWindowHandler(), &wc );

				if ( wc.showCmd == SW_MAXIMIZE )
				{
					iX = (iX * SETTINGSHANDLER->GetModel()->GetOldSettings().dwWidth) / GetSystemMetrics( SM_CXSCREEN );
					iY = (iY * SETTINGSHANDLER->GetModel()->GetOldSettings().dwHeight) / dwHeight;
				}
			}

			*(WORD*)0x04B0B228 = (WORD)iX;
			*(WORD*)0x04B0B22A = (WORD)iY;
			*(int*)0x0A17518 = (int)iX;
			*(int*)0x0A1751C = (int)iY;
			*(int*)0x0A16194 = *(int*)0x0A17518;
			*(int*)0x0A16198 = *(int*)0x0A1751C;

			MOUSEHANDLER->OnMouseMoveHandler( iX, iY );
			
			struct BIG_WORD
			{
				union
				{
					DWORD	dwAddr;
					WORD	wAddr[2];
				};
			};

			BIG_WORD * bwAddr; 
			bwAddr = (BIG_WORD*)&lParam;

			bwAddr->wAddr[0] = iX;
			bwAddr->wAddr[1] = iY;

			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof( TRACKMOUSEEVENT );
			tme.dwFlags = TME_LEAVE;
			tme.hwndTrack = GetWindowHandle();
			if ( TrackMouseEvent( &tme ) )
			{
				bMouseInsideWindow = true;
			//	ANTICHEATHANDLER->AutoClickUpdate( GetWindowHandle(), bMouseInsideWindow );
			}

			bUseOld = TRUE;
			break;

		case WM_MOUSELEAVE:
			bMouseInsideWindow = false;
			if ( Game::GetGameMode() == EGameMode::GAMEMODE_InGame )
			{
			//	ANTICHEATHANDLER->AutoClickUpdate( GetWindowHandle(), bMouseInsideWindow );
			}
			break;

		case WM_MOUSEWHEEL:
			if ( MOUSEHANDLER->OnMouseScrollHandler( GET_WHEEL_DELTA_WPARAM( wParam ) ) )
				return 0;

			bUseOld = TRUE;
			break;


		case WM_LBUTTONDOWN:
			if ( GetForegroundWindow() != hWnd )
				return 0;

			if ( bMouseInsideWindow == false )
				return 0;

			//Regen Fied
			WRITEDWORD( 0x04B0E264, 0 );

			if ( MOUSEHANDLER->OnMouseClickHandler( uMsg ) )
				return 0;

			bUseOld = TRUE;
			break;

		case WM_LBUTTONUP:
			if ( GetForegroundWindow() != hWnd )
				return 0;

			if ( bMouseInsideWindow == false )
				return 0;

			if ( MOUSEHANDLER->OnMouseClickHandler( uMsg ) )
				return 0;

			bUseOld = TRUE;
			break;

		case WM_RBUTTONDOWN:
			if ( GetForegroundWindow() != hWnd )
				return 0;

			if ( bMouseInsideWindow == false )
				return 0;

			//Regen Fied
			WRITEDWORD( 0x04B0E264, 0 );

			if ( MOUSEHANDLER->OnMouseClickHandler( uMsg ) )
				return 0;

			bUseOld = TRUE;
			break;

		case WM_RBUTTONUP:
			if ( GetForegroundWindow() != hWnd )
				return 0;

			if ( bMouseInsideWindow == false )
				return 0;

			if ( MOUSEHANDLER->OnMouseClickHandler( uMsg ) )
				return 0;

			bUseOld = TRUE;
			break;

		case WM_LBUTTONDBLCLK:
			if ( GetForegroundWindow() != hWnd )
				return 0;

			if ( bMouseInsideWindow == false )
				return 0;

			if ( MOUSEHANDLER->OnMouseClickHandler( uMsg ) )
				return 0;

			bUseOld = TRUE;
			break;

		case WM_RBUTTONDBLCLK:
			if ( GetForegroundWindow() != hWnd )
				return 0;

			if ( bMouseInsideWindow == false )
				return 0;

			if ( MOUSEHANDLER->OnMouseClickHandler( uMsg ) )
				return 0;

			bUseOld = TRUE;
			break;

		case WM_IME_SETCONTEXT:
			return DefWindowProcW( hWnd, uMsg, wParam, 0 );
			break;

		case WM_IME_CHAR:
			if ( GetForegroundWindow() == hWnd )
			{
				KEYBOARDHANDLER->OnKeyChar( (wchar_t)wParam );
			}

			return DefWindowProcW( hWnd, uMsg, wParam, lParam );
			break;

		case WM_CHAR:
		//	ERRORBOX_INT( IsWindowUnicode( hWnd ) );
			if ( GetForegroundWindow() == hWnd )
			{
                KEYBOARDHANDLER->SetLParam( lParam );
				if ( KEYBOARDHANDLER->OnKeyChar( (char)wParam ) )
					return 0;
			}
			bUseOld = TRUE;
			break;

		case WM_UNICHAR:
			if ( wParam == UNICODE_NOCHAR )
				return TRUE;

			if ( GetForegroundWindow() == hWnd )
			{
                KEYBOARDHANDLER->SetLParam( lParam );
				if ( KEYBOARDHANDLER->OnKeyChar( (wchar_t)wParam ) )
					return 0;
			}
			return TRUE;
			break;

		case WM_KEYDOWN:
			if ( GetForegroundWindow() == hWnd )
			{
                KEYBOARDHANDLER->SetLParam( lParam );
				if ( KEYBOARDHANDLER->OnKeyPress( wParam, TRUE ) )
					return 0;
			}
			bUseOld = TRUE;
			break;

		case WM_KEYUP:
			if ( GetForegroundWindow() == hWnd )
			{
                KEYBOARDHANDLER->SetLParam( lParam );
				if ( KEYBOARDHANDLER->OnKeyPress( wParam, FALSE ) )
					return 0;
			}

			bUseOld = TRUE;
			break;

		case WM_TIMER:
			bUseOld = TRUE;
			break;

		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
			if ( wParam == VK_F10 )
			{
				KEYBOARDHANDLER->OnKeyPress( wParam, (uMsg == WM_SYSKEYDOWN) );
				return 0;
			}
			break;

		case WM_SOCKETPACKET:
			SOCKETGAME->SocketPacket( (SocketData*)wParam, (PacketReceiving*)lParam );
			return 0;
			break;

		case WM_IME_NOTIFY:
			return TRUE;
			break;

		case WM_NCACTIVATE:
			if ( Game::GetGameMode() == GAMEMODE_InGame )
			{
				UnitData * p = UNITDATA;

				if ( (p) && (p->iID != 0) )
				{
					CALL( 0x0045BC90 );
				}
			}
			return DefWindowProcW( hWnd, uMsg, wParam, lParam );

		default:
			return DefWindowProcW( hWnd, uMsg, wParam, lParam );
	}

	if ( bUseOld )
	{
		DWORD dwRet = 0;
		
		__asm
		{
			PUSH lParam;
			PUSH wParam;
			PUSH uMsg;
			PUSH hWnd;
			MOV EAX, 0x0041C330;
			CALL EAX;
			MOV dwRet, EAX;
		}

		return dwRet;
	}

	return FALSE;
}

void CGameWindow::Loop()
{
	Update( pcTimer->Update() );

	Render();

	if ( Quit() )
		bExit = TRUE;

	CALL( 0x004165B0 );
}

NAKED bool CGameWindow::ResetCheck()
{
	JMP( 0x0063B4E0 );
}

void CGameWindow::Render()
{
	if ( bRender == FALSE )
		return;

	if ( GRAPHICENGINE->CheckDevice() == FALSE )
		return;

	GRAPHICENGINE->BeginScene( CAMERA->GetViewMatrix() );

	pcGame->Render3D();

	pcGame->Render2D();


	GRAPHICENGINE->EndScene();
}

void CGameWindow::Update( double fTime )
{
	static double fTick = (1000.0f / ((double)60));
	static double fOffs = 0.0f;

	//Split time into frames
	fOffs += fTime;
	if ( fOffs >= fTick )
	{
		do
		{
			pcGame->Frame();

			fOffs -= fTick;
		}
		while ( fOffs >= fTick );
	}

	//Update
	pcGame->Update( (float)fTime );

	if ( fOffs < fTick && GAMESCREEN->GetActiveScreenType() == SCREEN_Login )
		Sleep( (DWORD)(fTick - fOffs) );
}

BOOL CGameWindow::Quit()
{
	if ( GameCore::QuitGame() == TRUE && TRADEHANDLER->IsTradeMaskTime() == FALSE )
	{
		if ( Game::GetGameMode() == GAMEMODE_InGame && SOCKETG )
		{
			if ( GameCore::QuitAndSave() )
			{
				if ( GameCore::IsSaved() )
					return TRUE;

				if ( ((*(DWORD*)0x00CF4750) - (*(DWORD*)0x00D0FC34)) > 15000 )
					return TRUE;
					
				return FALSE;
			}

			if ( TRADEHANDLER->IsWindowTradeOpen() )
			{
				//Send Cancel Trade
				CALL_WITH_ARG2( 0x000628290, TRADE_UNITID, 3 );
				//Close
				CALLT( 0x0050C310, 0x036EFD98 );
			}

			if ( WAREHOUSEHANDLER->IsWarehouseWindowOpen() )
				WAREHOUSEHANDLER->RestoreItems();

			ITEMHANDLER->ResetItemMouse();

			SAVE;

			GameCore::QuitAndSave( TRUE );
		}
		else
			return TRUE;
	}
	return FALSE;
}

void CGameWindow::SetIME( bool b )
{
	return;

	if ( b == false )
	{
		if ( bActiveIME )
		{
			if ( hImc = ImmGetContext( hWnd ) )
			{
				ImmSetOpenStatus( hImc, FALSE );
				ImmReleaseContext( hWnd, hImc );

				bActiveIME = FALSE;
			}
		}
	}
	else
	{
		if ( bActiveIME == FALSE )
		{
			if ( ImmGetOpenStatus( hImc ) == FALSE )
			{
				if ( hImc = ImmGetContext( hWnd ) )
				{
					ImmSetOpenStatus( hImc, TRUE );

					ImmSetConversionStatus( hImc, IME_CMODE_NATIVE, IME_CMODE_NATIVE );

					ImmReleaseContext( hWnd, hImc );

					//LoadKeyboardLayoutW()

					bActiveIME = TRUE;
				}
			}
		}
	}
}
