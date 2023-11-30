#include "stdafx.h"
#include "Game.h"
#include "CSound.h"
#include "Controller.h"

char Game::szHardwareID[40];

UINT Game::uGameChecksum = 0;
UINT Game::uDLLChecksum = 0;

HANDLE Game::hEventLoader;

extern BOOL								bMouseCreatedHandler;

static BOOL								bKillFocus = FALSE;

// Window Procedure
static LRESULT							CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

extern GameCore										* pGameCoreHandler;

// Return Window Procedure of PT
typedef									LRESULT( CALLBACK *t_WndProcPT ) ( HWND, UINT, WPARAM, LPARAM );
t_WndProcPT								WndProcPT = ( t_WndProcPT )0x0041C330;

HANDLE hThreadLoader;

Game::Game()
{
}

Game::~Game()
{
	if ( hThreadLoader != 0 )
		TerminateThread( hThreadLoader, 0 );
}


EXEModelData * Game::ReadInx( char * pszFile )
{
	EXEModelDataHandler * pcModelDataHandler = ((EXEModelDataHandler *)0x00A79248);

	return pcModelDataHandler->LoadModelData( pszFile );
}

void Game::RenderInit()
{
	CALLT( 0x0047F2E0, 0x03245878 );
}

void Game::RenderClipStateSet( DWORD dw )
{
	typedef void( __thiscall *tfnP )(DWORD dwClass, DWORD p);
	tfnP fnp = (tfnP)0x0047F2F0;
	fnp( 0x03245878, dw );
}

void Game::RenderClip()
{
	typedef void( __thiscall *tfnP )(DWORD dwClass);
	tfnP fnp = (tfnP)0x00483D10;
	fnp( 0x03245878 );
}

void Game::RenderGeomVertex2D(DWORD dwColor)
{
	typedef void( __thiscall *tfnP )(DWORD dwClass, DWORD dw);
	tfnP fnp = (tfnP)0x00483EA0;
	fnp( 0x03245878, dwColor );
}

void Game::RenderD3D()
{
	typedef void( __thiscall *tfnP )(DWORD dwClass);
	tfnP fnp = (tfnP)0x004849B0;
	fnp( 0x03245878 );
}

void Game::AnimObjectTree( void * pObject, int iFrame, int iAX, int iAY, int iAZ )
{
	typedef void( __cdecl *tfnP )( void * pObject, int iFrame, int iAX, int iAY, int iAZ );
	tfnP fnP = (tfnP)0x0041E710;
	fnP( pObject, iFrame, iAX, iAY, iAZ );
}

void Game::CreateInx( char * pszFile )
{
	char szFileName[MAX_PATH] = { 0 };
	STRINGCOPY( szFileName, pszFile );

	typedef BOOL( __cdecl *tfnReadINX )(char * pszFile);
	tfnReadINX ReadINX = (tfnReadINX)0x00457BA0;

	char * pBuffer = NULL;
	DWORD dwAddr = 0;
	if ( ReadINX( pszFile ) )
	{
		DWORD dwCount = (*(DWORD*)0x0209EA48) - 1;
		dwAddr = (*(DWORD*)(0x02081628 + (dwCount * 4)));
		if ( dwAddr )
		{
			pBuffer = (char*)(*(DWORD*)(dwAddr + 0x54));
			if ( pBuffer )
			{
				ChangeFileExtension( pszFile, "inx", szFileName );
				FILE * fp;
				fopen_s( &fp, szFileName, "wb" );
				if ( fp )
				{
					fwrite( pBuffer, 95268, 1, fp );
					fclose( fp );
				}
			}
		}
	}
}

BOOL Game::Initialize()
{
	int iScreenWidth, iScreenHeight;

	// hInstance
	hInst = *( HINSTANCE* )0x0A1752C;

	// Initialize the width and height of the screen to zero before sending the variables into the function.
	SETTINGSHANDLER->GetModel()->ReadSettings();
	iScreenWidth = SETTINGSHANDLER->GetModel()->GetSettings().dwWidth;
	iScreenHeight = SETTINGSHANDLER->GetModel()->GetSettings().dwHeight;

	STRINGCOPYLEN( (char*)0x039033E8, 32, SETTINGSHANDLER->GetModel()->GetSettings().szLastAccount );

	// Create iSound
	DX::SoundOld_ptr pSound = std::make_shared<DX::SoundOld>();
	pSound->CreateDevices();

	DeleteClanTag();

	DX::Graphic::ResolutionChange( TRUE, iScreenWidth, iScreenHeight ); 

	// Initialize the windows api.
	InitializeWindow( iScreenWidth, iScreenHeight );

	// Create Edit Window
	HWND hWndEdt = CreateWindowEx( NULL, "EDIT", "", 0x40801004, 10, 400, 500, 40, hWindow, ( HMENU )101, hInst, NULL );

	SetWindowTextA( hWndEdt, (char*)0x039033E8 );

	// Copy hWndEdit for Executable
	*( UINT* )0x0A17544 = ( UINT )hWndEdt;
	*( UINT* )0x0A15DF4 = SetWindowLong( hWndEdt, GWL_WNDPROC, 0x416810 );
	*( UINT* )0x0A176DC = ( UINT )hWndEdt;
	*( UINT* )0x0A17418 = ( UINT )ImmGetContext( hWindow );
	*( UINT* )0x0A15DE0 = ( UINT )ImmGetContext( hWndEdt );

	// Initial Context
	CALL_WITH_ARG1( 0x00416780, ( DWORD )0 );

	// Initialize Font
	CALL( 0x0041A210 );

	// Initialization Game
	CALL_WITH_ARG1( 0x00417D70, ( DWORD )1 );

	// Set Window Timer
	SetTimer( hWindow, 0, 100, 0 );
	SetTimer( hWindow, 0x6A6A6A6A, 850, 0 );
	SetTimer( hWindow, 0x6A6A6A6B, 10, 0 );

	// Set sockets to 0
	WRITEBYTE( 0x38FED40, 0 );
	WRITEBYTE( 0x38FEE40, 0 );
	WRITEDWORD( 0x04C38C28, 27242 );

	// Time Sleep when game is minimized
	WRITEDWORD( 0x824F6C, 200 );

	// Test GM
	if ( FILEEXIST( "testgm.txt" ) )
		GM_MODE = TRUE;


	pGameCoreHandler->Init();

	//Multi Core?
	if ( SYSTEM->IsMultiCore() )
	{
		DWORD_PTR dw = 0x1;
		
		//Quad Core?
		if ( SYSTEM->GetNumberOfCores() == 4 )
		{
			//Update for 3 Cores, since 4 cores not working( ??? )
			dw = 0xE;
		}
		else
		{
			//Update Cores
			for ( int i = 1; i < SYSTEM->GetNumberOfCores(); i++ )
				dw |= 1 << i;
		}

		//Set Affinity
	//	SetProcessAffinityMask( GetCurrentProcess(), dw );
	}

	// RenderD3D
	CALL( 0x0041C2E0 );

	// Shutdown Game
	CALL( 0x0046D1E0 );
	CALL( 0x00417810 );
	CALL( 0x0045A410 );

	DEVMODE devMode;
	ZeroMemory( &devMode, sizeof( DEVMODE ) );
	MoveMemory( &devMode, ( void* )0x0A16268, sizeof( DEVMODE ) );

	// If is fullscreen
	if ( *( int* )0x009CB028 == 0 )
	{
		if ( devMode.dmDriverVersion != 0 )
			ChangeDisplaySettingsA( &devMode, CDS_UPDATEREGISTRY );
	}

	return TRUE;
}

HINTERNET WINAPI Game::InternetOpenUrlClan( HINTERNET hInternet, LPCTSTR lpszUrl, LPCTSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwFlags, DWORD_PTR dwContext )
{
	return InternetOpenUrlA( hInternet, lpszUrl, "Accept: */*\r\n\r\n", lstrlenA( "Accept: */*\r\n\r\n" ), INTERNET_FLAG_DONT_CACHE, NULL );
}

void Game::OnSetRGBATimer()
{
	static int isR, isG, isB;
	static DWORD dwTimerColorStage;
	int iHour	= (*(int*)0x00826D10);
	int iMinute = (*(int*)0x00CF4798);

	int & R = (*(int*)0x034DF460);
	int & G = (*(int*)0x034DF464);
	int & B = (*(int*)0x034DF468);
	int & D = (*(int*)0x00CF4774);

	R -= D;
	G -= D;
	B -= D;
	isR = R;
	isG = G;
	isB = B;
}

void Game::Shutdown()
{
	// Shutdown the window.
	ShutdownWindow();
}

void Game::Run()
{
	MSG msg;
	BOOL bDone;

	// Initialize the message structure.
	ZeroMemory( &msg, sizeof( MSG ) );

	// Loop until there is a quit message from the window or the user.
	bDone = FALSE;
	while ( !bDone )
	{
		// Handle the windows messages.
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		// If windows signals to end the application then exit out.
		if ( msg.message == WM_QUIT )
			bDone = TRUE;
		else
			bDone = TRUE;
	}
}

void Game::SetMousePos( int iX, int iY )
{
	*( int* )0x0A17518 = iX;
	*( int* )0x0A1751C = iY;
}

void Game::GetMousePos( D3DXVECTOR2 * pVec )
{
	// Is vector?
	if ( pVec )
	{
		// Set mouse pos in vector
		pVec->x = ( float )*( int* )0x0A17518;
		pVec->y = ( float )*( int* )0x0A1751C;
	}
}

BOOL Game::IsPoint2D( int iX, int iY, int iW, int iH )
{
	int iXMouse = *( int* )0x0A17518;
	int iYMouse = *( int* )0x0A1751C;

	if ( ( iXMouse >= iX ) && ( iXMouse <= ( iX + iW ) ) )
	{
		if ( ( iYMouse >= iY ) && ( iYMouse <= ( iY + iH ) ) )
			return TRUE;
	}
	return FALSE;
}

BOOL bNewLoader = FALSE;

std::vector<EXEDataLoader *> vDataQueueLoader;

CMutex * pcMutexLoader = nullptr;

struct PacketUnitInfoDataLoader
{
	PacketUnitInfo sPacket;

	UnitData * pcUnitData;
};

struct PacketUnitInfoQuickDataLoader
{
	PacketUnitInfoQuick sPacket;

	UnitData * pcUnitData;
};

std::vector<PacketUnitInfoDataLoader> vPacketsUnitInfo;
std::vector<PacketUnitInfoQuickDataLoader> vPacketsUnitInfoQuick;

std::vector<PacketUnitInfoDataLoader> vPacketsUnitInfoJob;
std::vector<PacketUnitInfoQuickDataLoader> vPacketsUnitInfoQuickJob;

std::vector<PacketUnitInfoDataLoader> vPacketsUnitInfoFlushJob;
std::vector<PacketUnitInfoQuickDataLoader> vPacketsUnitInfoQuickFlushJob;

CMutex * pcMutexUnitInfoLoader = nullptr;

void Game::LoaderThread()
{
	return;

	if ( pcMutexUnitInfoLoader == nullptr )
		pcMutexUnitInfoLoader = new CMutex( "Loader Unit Info" );

	std::vector<EXEDataLoader *> vDataQueueInJob;


	while ( TRUE )
	{
		//Copy Data to Job vector
		{
			pcMutexLoader->Lock();

			for ( auto ps : vDataQueueLoader )
				vDataQueueInJob.push_back( ps );

			vDataQueueLoader.clear();

			pcMutexLoader->Unlock();
		}

		//Copy Data to Job vectors
		{
			pcMutexUnitInfoLoader->Lock();

			for ( auto ps : vPacketsUnitInfo )
				vPacketsUnitInfoJob.push_back( ps );

			vPacketsUnitInfo.clear();

			for ( auto ps : vPacketsUnitInfoQuick )
				vPacketsUnitInfoQuickJob.push_back( ps );

			vPacketsUnitInfoQuick.clear();

			pcMutexUnitInfoLoader->Unlock();
		}
		//Load Data
		for ( auto ps : vDataQueueInJob )
		{
			Game::LoadDataFromLoader( ps );

			//Yield CPU
			Sleep( 1 );
		}

		vPacketsUnitInfoJob.clear();
		vPacketsUnitInfoQuickJob.clear();

		vDataQueueInJob.clear();

		Sleep( 250 );
	}
}

void Game::CreateLoaderThread( bool bFromPackets )
{
	EXEDataLoader * ps = ((EXEDataLoader *)0x00A197B0) + (READDWORD( 0x00A1CCB8 ) & 0x3F);

	Game::LoadDataFromLoader( ps );

	//Next
	WRITEINT( 0x00A1CCB8, READINT( 0x00A1CCB8 ) + 1 );
}

CMutex * pcMutexLoaderTime = nullptr;
CMutex * pcMutexFlushTime = nullptr;

void Game::LoadDataFromLoader( EXEDataLoader * psLoader )
{
	//Map?
	if ( psLoader->pcStage )
	{
		//Set Loading
		GRAPHICENGINE->ShowLoading();

		//Load Map
		if ( Stage::LoadStage( psLoader->szName, psLoader->pcStage ) )
		{
			//Load Textures
			((EXETextureManager *)psLoader->pcStage->pMaterial)->ReadTextures( 1 );

			//Done!
			psLoader->iResult = TRUE;
			psLoader->pcStage->bState = TRUE;
		}

		//Loaded Successful..
		GRAPHICENGINE->HideLoading();
	}

	//Item?
	if ( psLoader->pcItemDrop )
	{
		//Set Loading
		psLoader->pcItemDrop->bHidden = TRUE;

		//Get old Models
		auto pModelDataOld = psLoader->pcItemDrop->psModelData;

		//Load new Model
		if ( auto pNewModelData = Game::ReadInx( psLoader->szName ) )
		{
			//First?
			if ( pNewModelData->iCount == 1 )
			{
				//Model?
				if ( pNewModelData->pcModel )
				{
					//Read Textures
					pNewModelData->pcModel->pcTextureManager->ReadTextures();
				}
			}

			//Still Hidden
			psLoader->pcItemDrop->bVisible = FALSE;
			psLoader->pcItemDrop->pcModel = pNewModelData->pcModel;
			psLoader->pcItemDrop->psModelData = pNewModelData;

			//Get Model Name
			STRINGCOPY( psLoader->pcItemDrop->szModelName, pNewModelData->szModelName );

			//Show
			psLoader->pcItemDrop->bVisible = TRUE;
		}

		//Done!
		psLoader->iResult = TRUE;

		//Delete Old Model
		if ( pModelDataOld )
			CALL_WITH_ARG1( 0x0041EE70, (DWORD)pModelDataOld );

		//Isn't Loading anymore
		psLoader->pcItemDrop->bHidden = FALSE;
	}

	//Unit?
	if ( psLoader->pcUnitData )
	{
		//Set Loading
		psLoader->pcUnitData->bNoMove = TRUE;
		
		EXEModelData * pNewModelData = nullptr;
		EXEModelData * pNewSecondModelData = nullptr;

		EXEModelData * pNewModelDataOld = psLoader->pcUnitData->pcBody;
		EXEModelData * pNewSecondModelDataOld = psLoader->pcUnitData->pcHead;

		//Body
		if ( pNewModelData = Game::ReadInx( psLoader->szName ) )
		{
			//Hide
			psLoader->pcUnitData->bVisible = FALSE;

			//Set Model
			psLoader->pcUnitData->SetBodyModelData( pNewModelData );

			//Idle
			psLoader->pcUnitData->Animate( ANIMATIONTYPE_Idle );

			//Active Unit
			psLoader->pcUnitData->bActive = TRUE;
			STRINGCOPY( psLoader->pcUnitData->sCharacterData.Player.szBodyModel, pNewModelData->szModelName );

			//Clear Head
			psLoader->pcUnitData->sCharacterData.Player.szHeadModel[0] = 0;

			//Has Submodel?
			if ( (pNewModelData->psModelData->szDeathModelDataFilePath[0] != 0) || (psLoader->szNameLoader2[0] != 0) )
			{
				//Load it
				if ( pNewSecondModelData = Game::ReadInx( (pNewModelData->psModelData->szDeathModelDataFilePath[0] != 0) ? 
														  pNewModelData->psModelData->szDeathModelDataFilePath : psLoader->szNameLoader2 ) )
				{
					if ( psLoader->szNameLoader2[0] != 0 )
						STRINGCOPY( psLoader->pcUnitData->sCharacterData.Player.szHeadModel, pNewSecondModelData->szModelName );

					psLoader->pcUnitData->SetHeadModelData( pNewSecondModelData );
				}
			}

			//First Time?
			if ( (pNewModelData->iCount == 1) || (pNewSecondModelData && (pNewSecondModelData->iCount == 1)) )
			{
				if ( pNewModelData->pcModel && pNewModelData->pcModel->pcTextureManager )
					pNewModelData->pcModel->pcTextureManager->ReadTextures();

				if ( pNewSecondModelData && pNewSecondModelData->pcModel && pNewSecondModelData->pcModel->pcTextureManager )
					pNewSecondModelData->pcModel->pcTextureManager->ReadTextures();
			}

			psLoader->pcUnitData->bVisible = TRUE;
		}

		psLoader->iResult = TRUE;

		//Delete Old Model
		if ( pNewModelDataOld )
			CALL_WITH_ARG1( 0x0041EE70, (DWORD)pNewModelDataOld );

		//Delete Old Model
		if ( pNewSecondModelDataOld )
			CALL_WITH_ARG1( 0x0041EE70, (DWORD)pNewSecondModelDataOld );

		//Can be rendered
		psLoader->pcUnitData->bNoMove = FALSE;
	}
}

void Game::AddCharacterLoader( UnitData * pcUnitData, const std::string strBody, const std::string strHead )
{
	int i = READINT( 0x00A18094 ) & 0x3F;

	WRITEINT( 0x00A18094, READINT( 0x00A18094 ) + 1 );

	EXEDataLoader * ps = ((EXEDataLoader *)0x00A197B0) + i;

	ps->pcStage = nullptr;
	ps->pcUnitData = pcUnitData;
	ps->pcItemDrop = nullptr;
	ps->bCancel = FALSE;
	ps->iResult = 0;
	STRINGCOPY( ps->szName, strBody.c_str() );

	if ( strHead.length() > 0 )
		STRINGCOPY( ps->szNameLoader2, strHead.c_str() );
	else
		ps->szNameLoader2[0] = 0;

	ps->pcUnitData->bNoMove = TRUE;

	CreateLoaderThread( true );
}

void Game::HandlePacket( UnitData * pcUnitData, PacketUnitInfo * psPacket )
{
	if ( pcUnitData->bNoMove == FALSE )
	{
		pcUnitData->bVisible = FALSE;

		AddCharacterLoader( pcUnitData, psPacket->sCharacterData.Player.szBodyModel, psPacket->sCharacterData.Player.szHeadModel );

		if ( pcUnitData->bVisible )
			ProcessHandlePacket( pcUnitData, psPacket );
	}
}

void Game::HandlePacket( UnitData * pcUnitData, PacketUnitInfoQuick * psPacket )
{
	if ( pcUnitData->bNoMove == FALSE )
	{
		pcUnitData->bVisible = FALSE;

		AddCharacterLoader( pcUnitData, psPacket->sCharacterData.Player.szBodyModel, psPacket->sCharacterData.Player.szHeadModel );

		if ( pcUnitData->bVisible )
			ProcessHandlePacket( pcUnitData, psPacket );
	}
}

void Game::HandlePacket( PacketUnitInfo * psPacket )
{
	static ModelAnimation sDataSafe;
	auto pcUnitDataFound = UNITDATABYID(psPacket->iID);

	if (pcUnitDataFound)
	{
		CopyMemory(&pcUnitDataFound->sCharacterData, &psPacket->sCharacterData, sizeof(CharacterData));
	}
	else
	{
		if ( auto pcUnitData = UNITGAME->GetFreeUnitDataSlot() )
		{
			UNITGAME->RemoveCharacterIDRequest( psPacket->iID );

			pcUnitData->Init();
			pcUnitData->bActive = TRUE;
			pcUnitData->sCharacterData.szName[0] = 0;
			pcUnitData->eUpdateMode = EUpdateMode::UPDATEMODE_ClientUnit;
			pcUnitData->pcBody = nullptr;
			pcUnitData->pcHead = nullptr;
			pcUnitData->bRemoteControlled = TRUE;
			pcUnitData->iUnknown[0] = 0;
			pcUnitData->bVisible = FALSE;
			pcUnitData->psModelAnimation = &sDataSafe;

			pcUnitData->iID = psPacket->iID ^ 0x6A6A6A6A;

			//2 = GAME SERVER
			pcUnitData->iServerCode = 2;

			CALLT( 0x00422170, (DWORD)pcUnitData );

			Game::HandlePacket( pcUnitData, psPacket );

			CALL_WITH_ARG1( 0x0045C5C0, (DWORD)pcUnitData );

			ACHIEVEMENTHANDLER->ProcessCache( UNITDATATOUNIT( pcUnitData ) );
		}
	}
}

void Game::HandlePacket( PacketUnitInfoQuick * psPacket )
{
	static ModelAnimation sDataSafe;

	if ( UNITDATABYID( psPacket->iID ) == nullptr )
	{
		if ( auto pcUnitData = UNITGAME->GetFreeUnitDataSlot() )
		{
			UNITGAME->RemoveCharacterIDRequest( psPacket->iID );

			pcUnitData->Init();
			pcUnitData->bActive = TRUE;
			pcUnitData->sCharacterData.szName[0] = 0;
			pcUnitData->eUpdateMode = EUpdateMode::UPDATEMODE_ClientUnit;
			pcUnitData->pcBody = nullptr;
			pcUnitData->pcHead = nullptr;
			pcUnitData->bRemoteControlled = TRUE;
			pcUnitData->iUnknown[0] = 0;
			pcUnitData->bVisible = FALSE;
			pcUnitData->psModelAnimation = &sDataSafe;

			pcUnitData->iID = psPacket->iID ^ 0x6A6A6A6A;

			//2 = GAME SERVER
			pcUnitData->iServerCode = 2;

			CALLT( 0x00422170, (DWORD)pcUnitData );

			Game::HandlePacket( pcUnitData, psPacket );

			CALL_WITH_ARG1( 0x0045C5C0, (DWORD)pcUnitData );

			ACHIEVEMENTHANDLER->ProcessCache( UNITDATATOUNIT( pcUnitData ) );
		}
	}
}

void Game::ProcessHandlePacket( UnitData * pcUnitData, PacketUnitInfo * psPacket )
{
	CopyMemory( &pcUnitData->sCharacterData, &psPacket->sCharacterData, sizeof( CharacterData ) );
	
	//Reset Damage?
	CALLT( 0x00422170, (DWORD)pcUnitData );

	pcUnitData->sPosition = psPacket->sPosition;

	//Initial Player State
	CALLT( 0x00422B20, (DWORD)pcUnitData );

	if ( pcUnitData->sCharacterData.iClanID )
	{
		pcUnitData->iClanInfoIndex = CALL_WITH_ARG1( 0x0065B900, pcUnitData->sCharacterData.iClanID );
		pcUnitData->dwClanUpdateTime = TICKCOUNT;

		if ( BLESSCASTLEHANDLER->GetClanID( pcUnitData->sCharacterData.iClanID ) == CASTLEMASTERID )
			pcUnitData->bBlessCastleCrown = TRUE;
	}

	pcUnitData->iCharacterType = CHARACTERTYPE_None;

	pcUnitData->iID ^= 0x6A6A6A6A;

	pcUnitData->Animate( ANIMATIONTYPE_Idle );

	pcUnitData->bVisible = TRUE;
}

void Game::ProcessHandlePacket( UnitData * pcUnitData, PacketUnitInfoQuick * psPacket )
{
	int iLength = sizeof( PacketUnitInfoQuick ) - psPacket->iLength;

	ZeroMemory( &pcUnitData->sCharacterData, sizeof( CharacterData ) );
	pcUnitData->sCharacterData.sHP.sCur = 10;
	pcUnitData->sCharacterData.sHP.sMax = 50;

	CopyMemory( &pcUnitData->sCharacterData, &psPacket->sCharacterData, sizeof( CharacterData ) - iLength );

	//Reset Damage?
	CALLT( 0x00422170, (DWORD)pcUnitData );

	pcUnitData->sPosition = psPacket->sPosition;

	//Initial Player State
	CALLT( 0x00422B20, (DWORD)pcUnitData );

	if ( pcUnitData->sCharacterData.iClanID )
	{
		pcUnitData->iClanInfoIndex = CALL_WITH_ARG1( 0x0065B900, pcUnitData->sCharacterData.iClanID );
		pcUnitData->dwClanUpdateTime = TICKCOUNT;

		if ( BLESSCASTLEHANDLER->GetClanID( pcUnitData->sCharacterData.iClanID ) == CASTLEMASTERID )
			pcUnitData->bBlessCastleCrown = TRUE;
	}

	pcUnitData->iCharacterType = CHARACTERTYPE_None;

	pcUnitData->iID ^= 0x6A6A6A6A;

	pcUnitData->Animate( ANIMATIONTYPE_Idle );

	pcUnitData->bVisible = TRUE;
}

void Game::FlushLoaders()
{

}

void Game::InitializeWindow( int iWidth, int iHeight )
{
	WNDCLASSEXA wc;

	// Create Pointer DXGraphicEngine
	DX::Graphic_ptr GraphicEngine_ptr = std::make_shared<DX::Graphic>();

	// Give the application a name.
	lpAppName = GAME_NAME;

	// Level Cap
	*(int*)(0x04B06F14) = SERVER_LEVEL_MAX;

	// Clear out the window class for use
	ZeroMemory( &wc, sizeof( WNDCLASSEX ) );

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInst;
	wc.hIcon = LoadIconA( hInst, (LPCSTR)111 );
	wc.hCursor = LoadCursorA( NULL, IDC_ARROW );
	wc.hbrBackground = ( HBRUSH )GetStockObject( BLACK_BRUSH );
	wc.lpszClassName = lpAppName;
	wc.cbSize = sizeof( WNDCLASSEX );

	// Register the window class.
	RegisterClassExA( &wc );

	// Determine the resolution of the clients desktop screen.
	iWidth = GetSystemMetrics( SM_CXSCREEN );
	iHeight = GetSystemMetrics( SM_CYSCREEN );

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if ( *( BOOL* )0x9CB028 == FALSE )
	{
		// Create the window and use the result as the handle
		hWindow = CreateWindowExA( WS_POPUP, lpAppName, lpAppName, WS_POPUP, CW_USEDEFAULT, 0, iWidth, iHeight, NULL, NULL, hInst, NULL );
	}
	else
	{
		RECT srcRect = { 0, 0, (LONG)*( UINT* )0x824F70, (LONG)*( UINT* )0x824F74 };
		UINT uWindowStyle = WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

		// Adjust Window
		AdjustWindowRect( &srcRect, uWindowStyle, FALSE );

		srcRect.right -= srcRect.left;
		srcRect.bottom -= srcRect.top;

		// Create the window and use the result as the handle 0x80CF0000
		hWindow = CreateWindowExA( NULL, lpAppName, lpAppName, uWindowStyle, ( GetSystemMetrics( SM_CXSCREEN ) / 2 ) - ( srcRect.right / 2 ),
								   ( GetSystemMetrics( SM_CYSCREEN ) / 2 ) - ( srcRect.bottom / 2 ), srcRect.right, srcRect.bottom, NULL, NULL, hInst, NULL );
	}

	// HWND of Game
	*( UINT* )0x0A17540 = ( UINT )hWindow;

	// Create Unknown Thread
	CreateThread( NULL, NULL, ( LPTHREAD_START_ROUTINE )0x416750, NULL, NULL, NULL );

	// Bring the window up on the screen and set it as main focus.
	ShowWindow( hWindow, SW_SHOW );
	UpdateWindow( hWindow );

	// Hide the mouse cursor.
	//ShowCursor( FALSE );

	// Init D3D 
	GRAPHICENGINE->InitD3D( hWindow );
}

void Game::ShutdownWindow()
{
	// Show the mouse cursor.
	//ShowCursor( TRUE );

	// Fix the display settings if leaving full screen mode.
	if ( *( BOOL* )0x9CB028 == FALSE )
		ChangeDisplaySettings( NULL, 0 );

	// Destroy Window
	DestroyWindow( hWindow );
	hWindow = NULL; 

	// Remove the application instance.
	UnregisterClassA( lpAppName, hInst );
	hInst = NULL;
}

void Game::DeleteClanTag()
{
	WIN32_FIND_DATA sFindData;
	char szFileName[256] = { 0 };
	HANDLE hFile = NULL;
	STRINGCOPY( szFileName, "save\\clanDATA\\Regnum\\*.bmp" );

	char szPath[512] = { 0 };

	if ( hFile = FindFirstFileA( szFileName, &sFindData ) )
	{
		do
		{
			STRINGFORMAT( szPath, "save\\clanDATA\\Regnum\\%s", sFindData.cFileName );
			DeleteFileA( szPath );
		}
		while ( FindNextFileA( hFile, &sFindData ) );

		FindClose( hFile );
	}

	STRINGCOPY( szFileName, "save\\clanDATA\\Regnum\\*.rep" );

	if ( hFile = FindFirstFileA( szFileName, &sFindData ) )
	{
		do
		{
			STRINGFORMAT( szPath, "save\\clanDATA\\Regnum\\%s", sFindData.cFileName );
			DeleteFileA( szPath );
		}
		while ( FindNextFileA( hFile, &sFindData ) );

		FindClose( hFile );
	}
}

void Game::SetHardwareID( const char * pszHardwareID )
{
	STRINGCOPY( szHardwareID, pszHardwareID );
}

//Ugly, Remove...
LRESULT CALLBACK WndProc( HWND hWindow, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	std::shared_ptr<Controller> pController = std::shared_ptr<Controller>();
	std::shared_ptr<Game> pGame = std::shared_ptr<Game>();

	switch ( uMsg )
	{
		case WM_MOUSEMOVE:
	//		if ( GetForegroundWindow() != hWindow )
	//			return 0;

			*( WORD* )0x04B0B228 = ( WORD )round( LOWORD( lParam ) );
			*( WORD* )0x04B0B22A = ( WORD )round( HIWORD( lParam ) );
			*( int* )0x0A17518 = ( int )round( LOWORD( lParam ) );
			*( int* )0x0A1751C = ( int )round( HIWORD( lParam ) );
			*( int* )0x0A16194 = *( int* )0x0A17518;
			*( int* )0x0A16198 = *( int* )0x0A1751C;
			if ( bMouseCreatedHandler )
				MOUSEHANDLER->OnMouseMoveHandler( *(int*)0x0A17518, *(int*)0x0A1751C );
			break;

		case WM_MOUSEWHEEL:
			if ( bMouseCreatedHandler && MOUSEHANDLER->OnMouseScrollHandler( GET_WHEEL_DELTA_WPARAM( wParam ) ) )
				return 0;
			break;
			/*
		case WM_TIMER:
			if ( wParam == 0x6A6A6A6B )
			{
				if ( *(UINT*)0x3A97620 && *(UINT*)0x3A97624 && *(UINT*)0x3A97628 )
					CALL( 0x0046E3A0 );
				*(UINT*)0x0A17684 += 1;
				return 0;
			}
			break;
			*/

		case WM_KILLFOCUS:

			break;

		case WM_NCLBUTTONDOWN:
		case WM_NCRBUTTONDOWN:
		//	CHARACTERGAME->CancelAttack();
			break;

		case WM_LBUTTONDOWN:

			if ( GetForegroundWindow() != hWindow )
				return 0;

			if ( bMouseCreatedHandler && MOUSEHANDLER->OnMouseClickHandler( uMsg ) )
				return 0;
			break;
		case WM_LBUTTONUP:
			if ( GetForegroundWindow() != hWindow )
				return 0;

			if ( bMouseCreatedHandler && MOUSEHANDLER->OnMouseClickHandler( uMsg ) )
				return 0;
			break;

		case WM_RBUTTONDOWN:
			if ( GetForegroundWindow() != hWindow )
				return 0;

			if ( bMouseCreatedHandler && MOUSEHANDLER->OnMouseClickHandler( uMsg ) )
				return 0;
			break;

		case WM_RBUTTONUP:
			if ( GetForegroundWindow() != hWindow )
				return 0;

			if ( bMouseCreatedHandler && MOUSEHANDLER->OnMouseClickHandler( uMsg ) )
				return 0;
			break;

		case WM_CHAR:
			if ( GetForegroundWindow() != hWindow )
				return 0;
			if ( (*(UINT*)0x04B0D364) && bMouseCreatedHandler && KEYBOARDHANDLER->OnKeyChar( (char)wParam ) )
				return 0;
			break;

		case WM_KEYDOWN:
			if ( GetForegroundWindow() != hWindow )
				return 0;
			if ( (*(UINT*)0x04B0D364) && bMouseCreatedHandler && KEYBOARDHANDLER->OnKeyPress( wParam, TRUE ) )
				return 0;
			break;

		case WM_KEYUP:
			if ( GetForegroundWindow() != hWindow )
				return 0;
			if ( (*(UINT*)0x04B0D364) && bMouseCreatedHandler && KEYBOARDHANDLER->OnKeyPress( wParam, FALSE ) )
				return 0;
			break;

		default:
			break;
	}
	WndProcPT( hWindow, uMsg, wParam, lParam );

	// Keys?
	if ( uMsg >= WM_KEYFIRST && uMsg <= WM_KEYLAST )
		return 0;

	// Close?
	if ( uMsg == WM_CLOSE || uMsg == WM_QUIT )
		return 0;

	return DefWindowProcA( hWindow, uMsg, wParam, lParam );
}