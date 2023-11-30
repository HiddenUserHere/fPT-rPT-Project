#include "stdafx.h"
#include "CLoginScreen.h"

CLoginScreen::LoginServerList * CLoginScreen::psLoginServerList = ((CLoginScreen::LoginServerList *)0x038FEF40);
CLoginScreen::LoginServerCharacterInfo * CLoginScreen::psLoginServerCharacterInfo = ((CLoginScreen::LoginServerCharacterInfo *)0x038FF358);
int CLoginScreen::GAMESERVER_SERVERID = 0;

static LoginScreenMovementData saLoginMovements[] =
{
	{ Point3D( 948 << 8, 440 << 8, -17474 << 8 ), Point3D( 948 << 8, 440 << 8, -17474 << 8 ), Point3D( 966 << 8, 364 << 8, -18073 << 8 ), 100.0f },
};


static const char * pszaModelLoginCharacters[] = 
{
	"char\\tmABCD\\B023.ini",
	"char\\tmABCD\\A023.ini",
	"char\\tmABCD\\D023.ini",
	"char\\tmABCD\\C023.ini",
	"char\\tmABCD\\mB023.ini",
	"char\\tmABCD\\mA023.ini",
	"char\\tmABCD\\mD023.ini",
	"char\\tmABCD\\mC023.ini",
	"char\\tmABCD\\E023.ini",
	"char\\tmABCD\\mE023.ini",
};


CLoginScreen::CLoginScreen()
{
	//CALLT( 0x0059E490, GetThis() );
	pWindowLoginScreenSelector = std::make_shared<UI::Window>( Rectangle2D( (RESOLUTION_WIDTH >> 2) - 120, RESOLUTION_HEIGHT - 120, 200, 75 ) );
	pWindowLoginScreenSelector->SetImage( UI::ImageLoader::LoadImage( "game\\images\\login\\bg_selector.png" ) );

	pWindowServerSelector = std::make_shared<UI::Window>( Rectangle2D( ((RESOLUTION_WIDTH >> 2) * 3) + 120, RESOLUTION_HEIGHT - 120, 200, 75 ) );
	pWindowServerSelector->SetImage( UI::ImageLoader::LoadImage( "game\\images\\login\\bg_selector.png" ) );

#ifdef CEF_UI_BROWSER
	pcBrowser = new CBrowserWindowHandler( 800, 600 );
#endif

    pcWindow = new CLoginScreenWindowHandler();
}

CLoginScreen::~CLoginScreen()
{
    DELET( pcWindow );

//	CALLT( 0x0059F8F0, GetThis() );
	pWindowLoginScreenSelector = nullptr;
	pWindowServerSelector = nullptr;
}

BOOL CLoginScreen::Init()
{
//	UnblockBMPBPT();

	//EXETexture::EncryptDecryptTextures( "C:\\Patch", true );

	bLoginVideo = FALSE;

#ifdef CEF_UI_BROWSER
	pcBrowser->Init();
#endif

	CALLT( 0x0059E570, GetThis() );
	CALL( 0x0066D060 );

	SOUNDHANDLER->LoadMusic( XMAS_EVENT_THINGS ? BACKGROUNDMUSICID_XmasLogin : BACKGROUNDMUSICID_Login );
	SOUNDHANDLER->SetVolumeMusic( SETTINGSHANDLER->GetModel()->GetSettings().bNoMusic ? 0 : SETTINGSHANDLER->GetModel()->GetSettings().iMusicVolume );

	EXETexture::GetInstance()->ReadTextures();

	//Check Game Resolution
	if( RESOLUTION_WIDTH < 1024 && RESOLUTION_HEIGHT < 768 )
	{
		MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_OkCancel );
		MESSAGEBOX->SetTitle( "Warning" );
		MESSAGEBOX->SetDescription( "Deprecated resolution! We will update your resolution." );
		MESSAGEBOX->SetEvent( std::bind( &CLoginScreen::OnUpdateResolution, this ) );
		MESSAGEBOX->Show();
	}
	
	//Video Selector
	{
		int iY = 10;
		UI::CheckBox_ptr pCheckBoxImage = std::make_shared<UI::CheckBox>( Rectangle2D( 10, iY, 13, 13 ) );
		pCheckBoxImage->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxImage->SetText( "Image Only" );
		pCheckBoxImage->SetID( CHECKBOXID_ImageOnly );
		pCheckBoxImage->SetEvent( UI::Event::Build( std::bind( &CLoginScreen::OnCheckScreen, this, CHECKBOXID_ImageOnly, std::placeholders::_1 ) ) );
		pCheckBoxImage->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowLoginScreenSelector->AddElement( pCheckBoxImage );

		iY += 20;
		UI::CheckBox_ptr pCheckBoxVideoLow = std::make_shared<UI::CheckBox>( Rectangle2D( 10, iY, 13, 13 ) );
		pCheckBoxVideoLow->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxVideoLow->SetText( "Video Low Quality" );
		pCheckBoxVideoLow->SetID( CHECKBOXID_VideoLow );
		pCheckBoxVideoLow->SetCheck( TRUE );
		pCheckBoxVideoLow->SetEvent( UI::Event::Build( std::bind( &CLoginScreen::OnCheckScreen, this, CHECKBOXID_VideoLow, std::placeholders::_1 ) ) );
		pCheckBoxVideoLow->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowLoginScreenSelector->AddElement( pCheckBoxVideoLow );

		iY += 20;
		UI::CheckBox_ptr pCheckBoxVideoHigh = std::make_shared<UI::CheckBox>( Rectangle2D( 10, iY, 13, 13 ) );
		pCheckBoxVideoHigh->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxVideoHigh->SetText( "Video High Quality" );
		pCheckBoxVideoHigh->SetID( CHECKBOXID_VideoHigh );
		pCheckBoxVideoHigh->SetEvent( UI::Event::Build( std::bind( &CLoginScreen::OnCheckScreen, this, CHECKBOXID_VideoHigh, std::placeholders::_1 ) ) );
		pCheckBoxVideoHigh->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowLoginScreenSelector->AddElement( pCheckBoxVideoHigh );
	}

	//Server Selector
	{
		int iY = 10;
		UI::Text_ptr pTextServerSelector = std::make_shared<UI::Text>( Rectangle2D( 10, iY, 120, 20 ) );
		pTextServerSelector->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
		pTextServerSelector->SetText( "Select World:" );
		pWindowServerSelector->AddElement( pTextServerSelector );

		iY += 20;
		UI::CheckBox_ptr pCheckBoxOfficialServer = std::make_shared<UI::CheckBox>( Rectangle2D( 10, iY, 13, 13 ) );
		pCheckBoxOfficialServer->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxOfficialServer->SetText( "PT" );
		pCheckBoxOfficialServer->SetID( CHECKBOXID_OfficialServer );
		pCheckBoxOfficialServer->SetCheck( TRUE );
		pCheckBoxOfficialServer->SetEvent( UI::Event::Build( std::bind( &CLoginScreen::OnCheckServer, this, CHECKBOXID_OfficialServer, std::placeholders::_1 ) ) );
		pCheckBoxOfficialServer->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowServerSelector->AddElement( pCheckBoxOfficialServer );
		iOldCheckBoxServer = CHECKBOXID_OfficialServer;

		EWorldID eWorldID = FileExist( "testserver.ini" ) ? WORLDID_TestLocal : WORLDID_Fortress;

		GAMEWORLD->SetWorld( eWorldID );
		GAMEWORLD->Apply();

		iY += 20;
		UI::CheckBox_ptr pCheckBoxTestServer = std::make_shared<UI::CheckBox>( Rectangle2D( 10, iY, 13, 13 ) );
		pCheckBoxTestServer->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxTestServer->SetText( "BETA" );
		pCheckBoxTestServer->SetID( CHECKBOXID_TestServer );
		pCheckBoxTestServer->SetEvent( UI::Event::Build( std::bind( &CLoginScreen::OnCheckServer, this, CHECKBOXID_TestServer, std::placeholders::_1 ) ) );
		pCheckBoxTestServer->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowServerSelector->AddElement( pCheckBoxTestServer );
	}

	iOldCheckBox = CHECKBOXID_ImageOnly;
	OnCheckScreen( CHECKBOXID_VideoLow, UIEventArgs{} );

	sPositionCamera = Point3D( 948 << 8, 440 << 8, -17474 << 8 );
	sPositionPoint.iX = 966 << 8;
	sPositionPoint.iY = 364 << 8;
	sPositionPoint.iZ = -18073 << 8;
	sAngleCamera = Point3D( 0, 0, 0 );

	pcStage = Stage::LoadStage( XMAS_EVENT_THINGS ? "Field\\RicartenXmas\\village-2.ASE" : "Field\\Ricarten\\village-2.ASE" );
	pcStage->bState = TRUE;

	time_t t = time( NULL );
	tm * psTime = localtime( &t );

	Sky::SetSky( 1 );

	/*
	Sky::SetSky( 35 );

	if ( psTime )
	{
		if ( (psTime->tm_hour >= 16) && (psTime->tm_hour < 19) )
			Sky::SetSky( 36 );
		else if ( (psTime->tm_hour >= 19) || (psTime->tm_hour < 6) )
			Sky::SetSky( 37 );
	}
*/
	fTransparency = -1600.0f;

	//Update Units
	for ( int i = 0; i < _countof( pcaUnitData ); i++ )
	{
		pcaUnitData[i] = new UnitData();
		UnitData * pcUnitData = pcaUnitData[i];

		pcUnitData->Init();
		pcUnitData->pcStage = pcStage;

		ECharacterClass iClass = (ECharacterClass)(i + 1);

		if ( (iClass != CHARACTERCLASS_Fighter) && (iClass != CHARACTERCLASS_Archer) && (iClass != CHARACTERCLASS_Atalanta) )
			continue;

		pcUnitData->sCharacterData.iRank = CHARACTERRANK_Rank5;

		//Hair
		char szNewHeadModel[128] = { 0 };

		char cTier[2] = { pcUnitData->sCharacterData.iRank ? 0x60 + (char)pcUnitData->sCharacterData.iRank : 0, 0 };
		char cChar[2] = { pcUnitData->sCharacterData.iRank == 2 ? '_' : '-', 0 };

		switch ( iClass )
		{
			case CHARACTERCLASS_Fighter:
				STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Tmh%sB01%s.inf", cChar, cTier );
				break;

			case CHARACTERCLASS_Mechanician:
				STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Tmh%sA01%s.inf", cChar, cTier );
				break;

			case CHARACTERCLASS_Archer:
				STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Tfh%sD01%s.inf", cChar, cTier );
				break;

			case CHARACTERCLASS_Pikeman:
				STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Tmh%sC01%s.inf", cChar, cTier );
				if ( UNITDATA->sCharacterData.iRank == 3 )
					STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\TmhC-01c.inf" );
				break;

			case CHARACTERCLASS_Atalanta:
				STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Mfh%sB01%s.inf", cChar, cTier );
				break;

			case CHARACTERCLASS_Knight:
				STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Mmh%sA01%s.inf", cChar, cTier );
				break;

			case CHARACTERCLASS_Magician:
				STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Mmh%sD01%s.inf", cChar, cTier );
				break;

			case CHARACTERCLASS_Priestess:
				STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Mfh%sC01%s.inf", cChar, cTier );
				break;

			case CHARACTERCLASS_Assassin:
				STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Tfh%sE01%s.inf", cChar, cTier );
				break;

			case CHARACTERCLASS_Shaman:
				STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Mmh%sE01%s.inf", cChar, cTier );
				break;

			default:
				break;
		}

		Point3D sPosition = Point3D( (37990 << 8) -  (2048 * i), 1166 << 8, (49120 << 8) - (3072 * i) );
		Point3D sAngle = Point3D( 0, 2048, 0 );

		pcUnitData->sCharacterData.iClass = iClass;

		LoadUnitData( pcUnitData, pszaModelLoginCharacters[i], szNewHeadModel, &sPosition, &sAngle );
		pcUnitData->eUpdateMode = UPDATEMODE_ClientSelf;

		pcUnitData->sCharacterData.sWarpHomeTown = 0;
		pcUnitData->sCharacterData.iClass = iClass;

		pcUnitData->SetPosition( sPosition.iX, pcStage->GetHighestPoint( sPosition.iX, sPosition.iZ ), sPosition.iZ, 0, sAngle.iY, 0 );

		pcUnitData->bActive = TRUE;
		pcUnitData->bVisible = TRUE;

		pcUnitData->ChangeAnimationID( 0 );
	}

	CALL( 0x0048A260 );

    pcWindow->Init();

	return TRUE;
}

void CLoginScreen::Shutdown()
{
	DXVIDEORENDERER->Shutdown();

#ifdef CEF_UI_BROWSER
	pcBrowser->Shutdown();
#endif


//	pcRenderLogin.reset();
}

void CLoginScreen::Render3D()
{
//	DXVIDEORENDERER->Render();

	//Sky::SetSky( 37 );

	SetCamera( sPositionCamera, sPositionPoint );

	SetCharacterCamera( sPositionCamera.iX, sPositionCamera.iY, sPositionCamera.iZ, sAngleCamera.iX, sAngleCamera.iY, sAngleCamera.iZ );

	for ( int i = 0; i < _countof( pcaUnitData ); i++ )
	{
		UnitData * pcUnitData = pcaUnitData[i];
		if ( pcUnitData->bActive )
			pcUnitData->Render();
	}

    EXERender::SetRenderWithShaders( TRUE );

    if ( EXERender::IsRenderingWithShaders() )
    {
        if ( GRAPHICENGINE->GetTerrainShader() )
        {
            GRAPHICS->GetRenderer()->PrepareLights( GRAPHICENGINE->GetTerrainShader() );
            GRAPHICS->GetRenderer()->PrepareShadowMap( GRAPHICENGINE->GetTerrainShader() );
        }
    }

	pcStage->Render( sPositionCamera.iX, sPositionCamera.iY, sPositionCamera.iZ, sAngleCamera.iX, sAngleCamera.iY, 0 );

	EXERender::SetRenderWithShaders( FALSE );

	Sky::GetInstance()->Render( sPositionCamera.iX, 5000, sPositionCamera.iZ, sAngleCamera.iX, sAngleCamera.iY + 6000, sAngleCamera.iZ );

//	Point3D sPositionSky = sPositionCamera;
//	sPositionSky.iY -= 1;

//	SetCamera( sPositionSky, sPositionPoint );


	LOGINHANDLER->RenderServerSelectBox();

//	CALLT( 0x0059E7A0, GetThis() );

    RENDERER->CloseCamera();

#ifdef CEF_UI_BROWSER
	pcBrowser->GetClient()->GetWindowRenderer()->Render();
#endif
}

void CLoginScreen::Render2D()
{
	pcWindow->Render();

//	pWindowLoginScreenSelector->SetPosition( (RESOLUTION_WIDTH >> 2) - 120, RESOLUTION_HEIGHT - 120 );
//	pWindowServerSelector->SetPosition( ((RESOLUTION_WIDTH >> 2) * 3) - 60, RESOLUTION_HEIGHT - 120 );

//	pWindowLoginScreenSelector->Render( 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
//	pWindowServerSelector->Render( 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );

	//CALLT( 0x0059D790, GetThis() );

    //pcWindow->Render();


}

EScreen CLoginScreen::Frame()
{
	Sky::GetInstance()->Update();

	SOUNDHANDLER->SetVolumeMusic( SETTINGSHANDLER->GetModel()->GetSettings().bNoMusic ? 0 : SETTINGSHANDLER->GetModel()->GetSettings().iMusicVolume );

	//CALLT( 0x0059D860, GetThis() );

	for ( int i = 0; i < _countof( pcaUnitData ); i++ )
	{
		UnitData * pcUnitData = pcaUnitData[i];
		if ( pcUnitData->bActive )
		{
			pcUnitData->Main();
		}
	}

#ifdef CEF_UI_BROWSER
	pcBrowser->GetClient()->GetWindowRenderer()->Frame();
#endif

	//DXVIDEORENDERER->Update();

	UpdateCameraMovement();

    pcWindow->Update( 1000.0f / 60.0f );

	if ( NewScreen() )
		return SCREEN_Character;

	return SCREEN_Login;
}

void CLoginScreen::OnMouseMove( CMouse * pcMouse )
{
	SETTINGSHANDLER->GetWindow()->OnMouseMove( pcMouse );
	pWindowLoginScreenSelector->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
	pWindowServerSelector->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
#ifdef CEF_UI_BROWSER
	pcBrowser->OnMouseMove( pcMouse );
#endif

    pcWindow->OnMouseMove( pcMouse );
}

BOOL CLoginScreen::OnMouseClick( CMouse * pcMouse )
{
	/*
	if ( pWindowLoginScreenSelector->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;

	if ( pWindowServerSelector->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;
*/
	if ( pcMouse->GetEvent() == EMouseEvent::ClickDownL )
		bShowNotify = FALSE;

#ifdef CEF_UI_BROWSER
	pcBrowser->OnMouseClick( pcMouse );
#endif

    if ( pcWindow->OnMouseClick( pcMouse ) )
        return TRUE;

	return FALSE;
}

BOOL CLoginScreen::OnMouseScroll( CMouse * pcMouse )
{
    if ( pcWindow->OnMouseScroll( pcMouse ) )
        return TRUE;

    return FALSE;
}

BOOL CLoginScreen::OnKeyPress( CKeyboard * pcKeyboard )
{
    if ( pcWindow->OnKeyPress( pcKeyboard ) )
        return TRUE;

    return FALSE;
}

BOOL CLoginScreen::OnKeyChar( CKeyboard * pcKeyboard )
{
    if ( pcWindow->OnKeyChar( pcKeyboard ) )
        return TRUE;

    return FALSE;
}

void CLoginScreen::OnResolutionChanged()
{
    pcWindow->OnResolutionChanged();
}

void CLoginScreen::SetCamera( Point3D sPosition, Point3D sLookAt )
{
	struct eCAMERA_TRACE
	{
		int x, y, z;
		int tx, ty, tz;

		EXEMatrixE	eRotMatrix;
		int AngX, AngY;
	};


	eCAMERA_TRACE eTrace;

	/*
	const float CAMERA_SPEED_FACTOR = 0.125f;

	sPosition.iX += (int)((-((float)RESOLUTION_WIDTH * CAMERA_SPEED_FACTOR) * 0.5f) + (((float)MOUSEHANDLER->GetPosition()->iX * CAMERA_SPEED_FACTOR)));
	sPosition.iZ += (int)((-((float)RESOLUTION_WIDTH * CAMERA_SPEED_FACTOR) * 0.5f) + (((float)MOUSEHANDLER->GetPosition()->iX * CAMERA_SPEED_FACTOR)));
	sPosition.iY += (int)((-((float)RESOLUTION_HEIGHT * CAMERA_SPEED_FACTOR) * 0.5f) + (((float)MOUSEHANDLER->GetPosition()->iY * CAMERA_SPEED_FACTOR)));
*/
	CALL_WITH_ARG7( 0x0048B540, (DWORD)&eTrace, sPosition.iX, sPosition.iY, sPosition.iZ, sLookAt.iX, sLookAt.iY, sLookAt.iZ );
	CALLT_WITH_ARG4( 0x0047F1A0, (DWORD)RENDERER, sPosition.iX, sPosition.iY, sPosition.iZ, (DWORD)&eTrace.eRotMatrix );


	RENDERER->SetCameraPosition( sPosition.iX, sPosition.iY, sPosition.iZ, 0, 0, 0 );
    GRAPHICENGINE->GetCamera()->SetPosition( Math::Vector3( sPosition.iX / 256.0f, sPosition.iY / 256.0f, sPosition.iZ / 256.0f ), Math::Vector3( sLookAt.iX / 256.0f, (sLookAt.iY / 256.0f), sLookAt.iZ / 256.0f ) );

    if ( GRAPHICENGINE->GetTerrainShader() )
    {
        GRAPHICENGINE->GetTerrainShader()->SetMatrix( "Projection", GRAPHICENGINE->GetCamera()->Projection().Get() );
        GRAPHICENGINE->GetTerrainShader()->SetTechnique( "Terrain" );
        GRAPHICENGINE->GetTerrainShader()->SetFloat( "FogEnd", 1500.f );
        GRAPHICENGINE->GetTerrainShader()->SetInt( "NumActiveLights", 0 );

        GRAPHICS->GetRenderer()->SetFogEnd( 1500.f );
    }

	sAngleCamera.iX = eTrace.AngX;
	sAngleCamera.iY = eTrace.AngY;

	(*(int*)0x00A17520) = sPosition.iX;
	(*(int*)0x00A17510) = sPosition.iY;
	(*(int*)0x00A1619C) = sPosition.iZ;

	(*(int*)0x00824FA8) = eTrace.AngX;
	(*(int*)0x00A17630) = eTrace.AngY;

	D3DXVECTOR3 vPosition;
	vPosition.x = (float)sPosition.iX;
	vPosition.y = (float)sPosition.iY;
	vPosition.z = (float)sPosition.iZ;

	D3DXVECTOR3 vLookAt;
	vLookAt.x = (float)sLookAt.iX;
	vLookAt.y = (float)sLookAt.iY;
	vLookAt.z = (float)sLookAt.iZ;

	D3DXMATRIX matView;

	D3DXMatrixLookAtLH( &matView, &vPosition, &vLookAt, &D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );

	GRAPHICENGINE->GetDevice()->SetTransform( D3DTS_VIEW, &matView );    // set the view transform to matView

	D3DXMATRIX mat;
	D3DXMatrixIdentity( &mat );

	D3DXMatrixTranslation( &mat, vPosition.x, vPosition.y, 10.0f );


	GRAPHICENGINE->GetDevice()->SetTransform( D3DTS_WORLD, &mat );

	D3DXMATRIX matProjection;     // the projection transform matrix

	D3DXMatrixPerspectiveFovLH( &matProjection,
		D3DXToRadian( 45 ),    // the horizontal field of view
		(FLOAT)800 / (FLOAT)600, // aspect ratio
		1.0f,    // the near view-plane
		100000.0f );    // the far view-plane

	GRAPHICENGINE->GetDevice()->SetTransform( D3DTS_PROJECTION, &matProjection );    // set the projection
}

void CLoginScreen::UpdateCameraMovement()
{
	LoginScreenMovementData * ps = saLoginMovements + iCameraMovementID;
	if ( fTimeCameraMovement < ps->fTime )
	{
		fTimeCameraMovement += (1000.0f / 60.0f);

		sPositionPoint = ps->sLookAt;

		float fX = ((float)ps->sEndPosition.iX - (float)ps->sBeginPosition.iX) / ps->fTime;
		float fY = ((float)ps->sEndPosition.iY - (float)ps->sBeginPosition.iY) / ps->fTime;
		float fZ = ((float)ps->sEndPosition.iZ - (float)ps->sBeginPosition.iZ) / ps->fTime;

		sPositionCamera.iX = ps->sBeginPosition.iX + (int)roundf( fX * fTimeCameraMovement );
		sPositionCamera.iY = ps->sBeginPosition.iY + (int)roundf( fY * fTimeCameraMovement );
		sPositionCamera.iZ = ps->sBeginPosition.iZ + (int)roundf( fZ * fTimeCameraMovement );
	}
	else
	{
		if ( iCameraMovementID < (_countof( saLoginMovements ) - 1) )
		{
			iCameraMovementID++;

			fTimeCameraMovement = 0.0f;
		}
		else
		{
			fTransparency += 1.0f;
			if ( fTransparency > 0.0f )
				fTransparency = 0.0f;
		}
	}
}

void CLoginScreen::DisconnectAllServers()
{
    SOCKETGAME->CloseConnections();
}

void CLoginScreen::ConnectLoginServer( const std::string strAccountName, const std::string strPassword )
{
}

void CLoginScreen::ConnectGameServer( int iIndex )
{
}

void CLoginScreen::HandlePacket( SocketData * pcSocketData, PacketAccountLoginCode * psPacket )
{
    switch ( psPacket->iCode )
    {
        case ACCOUNTLOGIN_ConnectionFailed:
            GetWindow()->AddLoginMessage( "Connecting failed", 3000 );
            break;

        case ACCOUNTLOGIN_IncorrectName:
            GetWindow()->AddLoginMessage( "Account does not exist", 3000 );
            break;

        case ACCOUNTLOGIN_IncorrectPassword:
            GetWindow()->AddLoginMessage( "Incorrect Password", 3000 );
            break;

        case ACCOUNTLOGIN_Banned:
            GetWindow()->AddLoginMessage( psPacket->szMessage, 3000 );
            break;

        case ACCOUNTLOGIN_LoggedIn:
            GetWindow()->AddLoginMessage( "Account is already logged in", 3000 );
            break;

        case ACCOUNTLOGIN_IPBanned:
            GetWindow()->AddLoginMessage( "Your IP is banned", 3000 );
            break;

        case ACCOUNTLOGIN_YouAreBanned:
            GetWindow()->AddLoginMessage( "You are banned", 3000 );
            break;

        case ACCOUNTLOGIN_Maintenance:
            GetWindow()->AddLoginMessage( "Server is in Maintenance", 3000 );
            break;

        case ACCOUNTLOGIN_AccountNotActive:
            GetWindow()->AddLoginMessage( "Account not active, see User Management Panel", 3000 );
            break;

        case ACCOUNTLOGIN_WrongVersion:
            GetWindow()->AddLoginMessage( "Version does not Match", 3000 );
            break;

        case ACCOUNTLOGIN_TempBanned:
            GetWindow()->AddLoginMessage( "Temporarily Banned", 3000 );
            break;

        default:
            break;
    }

    if ( psPacket->iCode <= 0 )
    {
        if ( pcSocketData == SOCKETL )
        {
            SOCKETGAME->CloseServerL();

            GetWindow()->UnblockWindowLoginMouse();
        }
        else
        {
            SOCKETGAME->CloseServerG();

            GetWindow()->UnblockWindowSubServerMouse();
        }
    }

    psPacket->iHeader = 0;
}

void CLoginScreen::HandlePacket( PacketCharacterSelectUser * psPacket )
{
    if ( STRINGCOMPARE( psPacket->szAccountName, (char *)0x039033E8 ) )
    {
        int iCharacterCount = 0;

        ZeroMemory( psLoginServerCharacterInfo, sizeof( LoginServerCharacterInfo ) * MAX_LOGINSERVER_CHARACTER_LIST );

        for ( int i = 0; i < psPacket->iCount; i++ )
        {
            STRINGCOPY( psLoginServerCharacterInfo[i].szName, psPacket->saData[i].szName );

            STRINGCOPY( psLoginServerCharacterInfo[i].szBodyModel, psPacket->saData[i].szBodyModel );
            STRINGCOPY( psLoginServerCharacterInfo[i].szHeadModel, psPacket->saData[i].szHeadModel );

            psLoginServerCharacterInfo[i].iLevel = psPacket->saData[i].iLevel;
            psLoginServerCharacterInfo[i].iClass = psPacket->saData[i].iClass;

            psLoginServerCharacterInfo[i].iMapID = psPacket->saData[i].iMapID;

            psLoginServerCharacterInfo[i].iX = psPacket->saData[i].iX;
            psLoginServerCharacterInfo[i].iZ = psPacket->saData[i].iZ;

            iCharacterCount = i + 1;
        }

        WRITEINT( 0x038FF350, iCharacterCount );
    }

    psPacket->iHeader = 0;
}

void CLoginScreen::HandlePacket( PacketServerList * psPacket )
{
    int iServerCount = 0;
    ZeroMemory( psLoginServerList, sizeof( LoginServerList ) * MAX_LOGINSERVER_LIST );

    STRINGCOPYLEN( ((char*)0x03A46A7C), 16, psPacket->sHeader.szServerName );

    GetWindow()->ClearServerList();

    for ( int i = 0; i < psPacket->sHeader.iGameServers; i++ )
    {
        STRINGCOPY( psLoginServerList[i].szName, psPacket->sServers[i].szName );

        STRINGCOPY( psLoginServerList[i].szIP1, psPacket->sServers[i].szaIP[0] );
        STRINGCOPY( psLoginServerList[i].szIP2, psPacket->sServers[i].szaIP[1] );
        STRINGCOPY( psLoginServerList[i].szIP3, psPacket->sServers[i].szaIP[2] );

        psLoginServerList[i].iPort1 = psPacket->sServers[i].iaPort[0];
        psLoginServerList[i].iPort2 = psPacket->sServers[i].iaPort[1];
        psLoginServerList[i].iPort3 = psPacket->sServers[i].iaPort[2];

        GetWindow()->AddSubServer( psLoginServerList[i].szName, (i == 1), i );

        iServerCount = i + 1;
    }

    WRITEINT( 0x038FF354, iServerCount );

	if ( psPacket->sHeader.iClanServerIndex )
	{
		SetClanData( psPacket->sServers[psPacket->sHeader.iGameServers].szaIP[0], psPacket->sServers[psPacket->sHeader.iGameServers].iaPort[0],
					 psPacket->sServers[psPacket->sHeader.iGameServers].szaIP[1], psPacket->sServers[psPacket->sHeader.iGameServers].iaPort[1],
					 psPacket->sHeader.iTicket );
	}

    ACCOUNTHANDLER->HandlePacket( psPacket );

    GetWindow()->ShowServerList();

	WRITEDWORD( 0x03628700, psPacket->sHeader.dwTime );
	WRITEDWORD( 0x03A97618, psPacket->sHeader.dwTime );

	WRITEDWORD( 0x03A9761C, GetTickCount() );

    psPacket->iHeader = 0;
}

void CLoginScreen::HandlePacket( SocketData * pcSocketData, PacketVersion * psPacket )
{
    if ( psPacket->iVersion > READDWORD( 0x04AF7FFC ) )
    {
        if ( pcSocketData == SOCKETL )
            SOCKETGAME->CloseServerL();
        else
            SOCKETGAME->CloseServerG();

        GetWindow()->AddLoginMessage( "Version does not Match", 3000 );
    }
    else if ( psPacket->bServerFull )
    {
        if ( pcSocketData == SOCKETL )
        {
            SOCKETGAME->CloseServerL();

            GetWindow()->UnblockWindowLoginMouse();
        }
        else
        {
            SOCKETGAME->CloseServerG();

            GetWindow()->UnblockWindowSubServerMouse();
        }

        GetWindow()->AddLoginMessage( "Server is Full", 3000 );
    }
    else if ( pcSocketData == SOCKETG )
        iCurrentStateLogin = LOGINSTATE_Finish;
}

void CLoginScreen::CreateThreadLogin()
{
    CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)&CLoginScreen::ThreadLogin, NULL, 0, NULL );
}

bool CLoginScreen::CreateThreadGame( int iIndex )
{
    CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)&CLoginScreen::ThreadGame, (int*)iIndex, 0, NULL );

    return true;
}

void CLoginScreen::OnUpdateResolution()
{
	if( MESSAGEBOX->GetType() )
	{
		SETTINGSHANDLER->GetModel()->GetSettings().dwWidth = 1024;
		SETTINGSHANDLER->GetModel()->GetSettings().dwHeight = 768;
		SETTINGSHANDLER->GetModel()->WriteSettings();

		GRAPHICENGINE->ResolutionChange( FALSE, 1024, 768 );
		GRAPHICENGINE->Reset( SETTINGSHANDLER->GetModel()->GetSettings().bWindowed, SETTINGSHANDLER->GetModel()->GetSettings().dwWidth, SETTINGSHANDLER->GetModel()->GetSettings().dwHeight, TRUE );

		DXGraphicEngine::ResizeWindow();
	}
}

void CLoginScreen::OnCheckScreen( int iID, UIEventArgs e )
{
	if ( iOldCheckBox == iID )
	{
		pWindowLoginScreenSelector->GetElement<UI::CheckBox>( iID )->SetCheck( TRUE );
		return;
	}

	bLoginVideo = FALSE;

	double dPosition = DXVIDEORENDERER->GetPosition();

	DXVIDEORENDERER->Shutdown();

	if ( (iID == CHECKBOXID_VideoLow) || (iID == CHECKBOXID_VideoHigh) )
	{
		char * pszLogin = "game\\textures\\misc\\login_low.dat";

		UINT uVideoMemory = GRAPHICENGINE->GetVideoMemory() / 1024000;

		//2GB Video+ & Quad Core+ & 8GB Ram+? login high...
		if ( (uVideoMemory > 1792) && (SYSTEM->GetNumberOfCores() >= 4) && (SYSTEM->GetRAMAmount() >= 7168) && (iID == CHECKBOXID_VideoHigh) )
			pszLogin = "game\\textures\\misc\\login.dat";

		if ( DXVIDEORENDERER->Init( pszLogin ) )
		{
			DXVIDEORENDERER->SetFPS( 60 );
			DXVIDEORENDERER->Loop( TRUE );
//			DXVIDEORENDERER->SetRepeatBegin( 3.97 );
			DXVIDEORENDERER->SetPosition( dPosition );
			DXVIDEORENDERER->Run();

			bLoginVideo = TRUE;
		}
	}

	//Reset Checkbox
	pWindowLoginScreenSelector->GetElement<UI::CheckBox>( CHECKBOXID_ImageOnly )->SetCheck( iID == CHECKBOXID_ImageOnly ? TRUE : FALSE );
	pWindowLoginScreenSelector->GetElement<UI::CheckBox>( CHECKBOXID_VideoLow )->SetCheck( iID == CHECKBOXID_VideoLow ? TRUE : FALSE );
	pWindowLoginScreenSelector->GetElement<UI::CheckBox>( CHECKBOXID_VideoHigh )->SetCheck( iID == CHECKBOXID_VideoHigh ? TRUE : FALSE );

	iOldCheckBox = iID;
}

void CLoginScreen::OnCheckServer( int iID, UIEventArgs e )
{
	if ( iOldCheckBoxServer == iID )
	{
		pWindowServerSelector->GetElement<UI::CheckBox>( iID )->SetCheck( TRUE );
		return;
	}

	EWorldID eWorld = (iID == CHECKBOXID_OfficialServer) ? WORLDID_Fortress : WORLDID_Test;

	GAMEWORLD->SetWorld( eWorld );
	GAMEWORLD->Apply();

	//Reset Checkbox
	pWindowServerSelector->GetElement<UI::CheckBox>( CHECKBOXID_OfficialServer )->SetCheck( iID == CHECKBOXID_OfficialServer ? TRUE : FALSE );
	pWindowServerSelector->GetElement<UI::CheckBox>( CHECKBOXID_TestServer )->SetCheck( iID == CHECKBOXID_TestServer ? TRUE : FALSE );

	iOldCheckBoxServer = iID;
}

NAKED void CLoginScreen::SetClanData( const char * pszIP1, int iPort1, const char * pszIP2, int iPort2, int iTicket )
{
	JMP( 0x00661A50 );
}

DWORD WINAPI CLoginScreen::ThreadLogin()
{
    SOCKETGAME->ConnectServerL( GAMEWORLD->GetWorld()->GetIP().c_str(), PORT_GAME );

    if ( SOCKETL )
        GAMECORE->SendLoginData();
    else
    {
        ((CLoginScreen *)GAMESCREEN->GetActiveScreen())->GetWindow()->AddLoginMessage( "Connecting failed", 3000 );

        ((CLoginScreen *)GAMESCREEN->GetActiveScreen())->GetWindow()->UnblockWindowLoginMouse();
    }

    ExitThread( 0 );

    return 0;
}

DWORD __stdcall CLoginScreen::ThreadGame( int * piIndex )
{
	CLoginScreen::GAMESERVER_SERVERID = (int)piIndex + 1;

    SOCKETGAME->ConnectServerG( psLoginServerList[(int)piIndex].szIP1, psLoginServerList[(int)piIndex].iPort1 );

    if ( SOCKETG == NULL )
    {
        ((CLoginScreen *)GAMESCREEN->GetActiveScreen())->GetWindow()->AddLoginMessage( "Connecting failed", 3000 );

        ((CLoginScreen *)GAMESCREEN->GetActiveScreen())->GetWindow()->UnblockWindowSubServerMouse();

		CLoginScreen::GAMESERVER_SERVERID = 0;
    }

    ExitThread( 0 );

    return 0;
}
