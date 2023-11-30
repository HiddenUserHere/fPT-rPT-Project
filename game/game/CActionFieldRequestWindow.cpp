#include "stdafx.h"
#include "CActionFieldRequestWindow.h"


CActionFieldRequestWindow::CActionFieldRequestWindow()
{
}


CActionFieldRequestWindow::~CActionFieldRequestWindow()
{
	for ( auto & sLogo : vLogos )
		UI::ImageLoader::DeleteImage( sLogo.pcImage );

	vLogos.clear();
}

void CActionFieldRequestWindow::Init()
{
	BuildWindow();
}

void CActionFieldRequestWindow::Render()
{
	pWindow->Render( 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

void CActionFieldRequestWindow::Open()
{
	pWindow->Show();
	MOUSEHANDLER->BlockMouse();
}

void CActionFieldRequestWindow::Close()
{
	pWindow->Hide();
	MOUSEHANDLER->UnblockMouse();
}

void CActionFieldRequestWindow::OnResolutionChanged()
{
	//Base Rectangle
	int iWindowWidth = pWindow->GetWidth();
	int iWindowHeight = pWindow->GetHeight();
	int iWindowX = (RESOLUTION_WIDTH >> 2) - (iWindowWidth >> 1) + 30;
	int iWindowY = (RESOLUTION_HEIGHT >> 2) - (iWindowHeight >> 1) + 78;
	pWindow->SetPosition( iWindowX, iWindowY );
}

void CActionFieldRequestWindow::OnMouseMove( CMouse * pcMouse )
{
	pWindow->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

BOOL CActionFieldRequestWindow::OnMouseClick( CMouse * pcMouse )
{
	return pWindow->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );;
}

void CActionFieldRequestWindow::AddModeList( std::string strName, IMinMax sRequiredLevel, int iInstanceCount, int iInstanceLimitPerDay, EActionFieldModeID eMode )
{
	auto pMainList = pWindow->GetElement<UI::List>( LIST_Main );

	UI::Window_ptr pWindowMode = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 290, 24 ) );

	UI::Text_ptr pText = std::make_shared<UI::Text>( Rectangle2D( 28, 0, 290, 24 ) );
	pText->SetFont( "Arial", 16, 0, FALSE, FALSE, D3DCOLOR_XRGB( 255, 255, 255 ) );

	int iEntranceLeft = iInstanceLimitPerDay - iInstanceCount;

	pText->SetText( FormatString( "%s (%d-%d) Entrances left: %d/%d", strName.c_str(), sRequiredLevel.iMin, sRequiredLevel.iMax, iEntranceLeft, iInstanceLimitPerDay ) );

	pText->SetEvent( UI::Event::Build( std::bind( &CActionFieldRequestWindow::OnButtonRequestEventClick, this, strName, eMode, std::placeholders::_1 ) ) );
	pText->SetColorHover( D3DCOLOR_ARGB( 255, 255, 165, 0 ) );
	pWindowMode->AddElement( pText );
	pMainList->AddWindow( pWindowMode );

	if ( pMainList->GetWindowList().size() == 1 )
	{
		for ( auto & sLogo : vLogos )
		{
			if ( sLogo.eModeID == eMode )
			{
				UI::ImageBox_ptr pImageHeader = pWindow->GetElement<UI::ImageBox>( IMAGE_ModeAFLogo );
				pImageHeader->SetImage( sLogo.pcImage );
				break;
			}
		}
	}
}

void CActionFieldRequestWindow::ClearList()
{
	auto pMainList = pWindow->GetElement<UI::List>( LIST_Main );
	pMainList->Clear();
}

void CActionFieldRequestWindow::BuildWindow()
{
	vLogos.push_back( ImageLogoEventMode( EActionFieldModeID::ACTIONFIELDID_ForestDungeon1, UI::ImageLoader::LoadImage( "game\\images\\events\\ghostcastle\\tropicalisland.png" ) ) );
	vLogos.push_back( ImageLogoEventMode( EActionFieldModeID::ACTIONFIELDID_ForestDungeon2, UI::ImageLoader::LoadImage( "game\\images\\events\\ghostcastle\\tropicalisland.png" ) ) );
	vLogos.push_back( ImageLogoEventMode( EActionFieldModeID::ACTIONFIELDID_ForestDungeon3, UI::ImageLoader::LoadImage( "game\\images\\events\\ghostcastle\\tropicalisland.png" ) ) );
	vLogos.push_back( ImageLogoEventMode( EActionFieldModeID::ACTIONFIELDID_DragonsDungeon1, UI::ImageLoader::LoadImage( "game\\images\\events\\ghostcastle\\dragonsdungeon.png" ) ) );
	vLogos.push_back( ImageLogoEventMode( EActionFieldModeID::ACTIONFIELDID_DragonsDungeon2, UI::ImageLoader::LoadImage( "game\\images\\events\\ghostcastle\\dragonsdungeon.png" ) ) );
	vLogos.push_back( ImageLogoEventMode( EActionFieldModeID::ACTIONFIELDID_DragonsDungeon3, UI::ImageLoader::LoadImage( "game\\images\\events\\ghostcastle\\dragonsdungeon.png" ) ) );
	vLogos.push_back( ImageLogoEventMode( EActionFieldModeID::ACTIONFIELDID_GhostCastle1, UI::ImageLoader::LoadImage( "game\\images\\events\\ghostcastle\\main.png" ) ) );
	vLogos.push_back( ImageLogoEventMode( EActionFieldModeID::ACTIONFIELDID_GhostCastle2, UI::ImageLoader::LoadImage( "game\\images\\events\\ghostcastle\\main.png" ) ) );

	pWindow = std::make_shared<UI::Window>( Rectangle2D( 20, 20, 364, 374 ) );

	//Base Rectangle
	int iWindowWidth = pWindow->GetWidth();
	int iWindowHeight = pWindow->GetHeight();
	int iWindowX = (RESOLUTION_WIDTH >> 2) - (iWindowWidth >> 1) + 30;
	int iWindowY = (RESOLUTION_HEIGHT >> 2) - (iWindowHeight >> 1) + 78;

	pWindow->SetPosition( iWindowX, iWindowY );

	pWindow->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\window.png" ) );

	UI::ImageBox_ptr pHeaderBG = std::make_shared<UI::ImageBox>( Rectangle2D( (pWindow->GetWidth() >> 1) - (181 >> 1), -13, 181, 32 ) );
	pHeaderBG->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\text\\header.png" ) );
	pWindow->AddElement( pHeaderBG );

	//Header Title
	UI::ImageBox_ptr pHeaderTitle = std::make_shared<UI::ImageBox>( Rectangle2D( (pWindow->GetWidth() >> 1) - (47 >> 1), -4, 47, 16 ) );
	pHeaderTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\text\\header_event.png" ) );
	pWindow->AddElement( pHeaderTitle );

	UI::List_ptr pMainList = std::make_shared<UI::List>( Rectangle2D( 24, 260, 320, 96 ) );
	pMainList->SetID( LIST_Main );
	pWindow->AddElement( pMainList );

	//Image Header
	UI::ImageBox_ptr pImageHeader = std::make_shared<UI::ImageBox>( Rectangle2D( 20, 30, 321, 120 ) );
	pImageHeader->SetImage( vLogos[3].pcImage );
	pImageHeader->SetID( IMAGE_ModeAFLogo );
	pWindow->AddElement( pImageHeader );

	UI::TextWindow_ptr pTextWindow = std::make_shared<UI::TextWindow>( Rectangle2D( 30, 165, 290, 160 ) );
	pTextWindow->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pTextWindow->LoadTextFromFile( "game\\npcs\\ghostcastle\\main.txt" );
	pWindow->AddElement( pTextWindow );

	UI::Button_ptr pButtonOK = std::make_shared<UI::Button>( Rectangle2D( (pWindow->GetWidth() >> 1) - (93 >> 1), 342, 93, 23 ) );
	pButtonOK->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\btnclose.png", "game\\images\\UI\\window\\btnclose_.png" ) );
	pButtonOK->SetEvent( UI::Event::Build( std::bind( &CActionFieldRequestWindow::OnButtonCloseClick, this, std::placeholders::_1 ) ) );
	pWindow->AddElement( pButtonOK );

	pWindow->Hide();
}

void CActionFieldRequestWindow::OnButtonCloseClick( UIEventArgs eArgs )
{
	Close();
}

void CActionFieldRequestWindow::OnButtonYesClick( EActionFieldModeID eModeID, UIEventArgs eArgs )
{
	if ( MESSAGEBOX->GetType() )
		ACTIONFIELDHANDLER->ProcessPacket( &PacketActionFieldRequest( eModeID ) );

	Close();
}

void CActionFieldRequestWindow::OnButtonRequestEventClick( std::string strName, EActionFieldModeID eModeID, UIEventArgs eArgs )
{
	MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
	MESSAGEBOX->SetTitle( strName.c_str() );
	MESSAGEBOX->SetDescription( "Do you want to participate?" );
	MESSAGEBOX->SetEvent( std::bind( &CActionFieldRequestWindow::OnButtonYesClick, this, eModeID, std::placeholders::_1 ) );
	MESSAGEBOX->Show();
}
