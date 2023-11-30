#include "stdafx.h"
#include "BellatraReward.h"


void CBellatraReward::OnButtonOKClick( UIEventArgs e )
{
	int iAmount = ITEMHANDLER->GetItemAmountByCode( ITEMID_BrilliantStone );

	if ( iAmount >= 15 )
	{
		ITEMHANDLER->DeleteItemQuest( ITEMID_BrilliantStone, 15 );

		PacketReceiveItemIhin s;
		s.iLength	= sizeof( PacketReceiveItemIhin );
		s.iHeader	= PKTHDR_NPCIhinRewardRequest;
		s.iCount	= iAmount + 80;
		SENDPACKETG( &s, TRUE );
	}
	else
	{
		TITLEBOX( "You need 15 Brilliant Stones to get reward!" );
	}

	pWindow->Hide();
	MOUSEHANDLER->UnblockMouse();
}

void CBellatraReward::OnButtonCloseClick( UIEventArgs e )
{
	pWindow->Hide();
	MOUSEHANDLER->UnblockMouse();
}

CBellatraReward::CBellatraReward()
{
}


CBellatraReward::~CBellatraReward()
{
}

void CBellatraReward::BuildWindow()
{
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

	// Header Title
	UI::ImageBox_ptr pHeaderTitle = std::make_shared<UI::ImageBox>( Rectangle2D( (pWindow->GetWidth() >> 1) - (46 >> 1), -4, 46, 16 ) );
	pHeaderTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\text\\header_event.png" ) );
	pWindow->AddElement( pHeaderTitle );

	//List
	UI::List_ptr pList = std::make_shared<UI::List>( Rectangle2D( 20, 20, 340, 308 ) );
	pList->SetID( LIST_Main );
	pWindow->AddElement( pList );

	//Window Main
	UI::Window_ptr pWindowMain = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 340, 600 ) );
	pWindowMain->SetID( WINDOW_Main );

	//Text Window
	UI::TextWindow_ptr pTextWindow = std::make_shared<UI::TextWindow>( Rectangle2D( 0, 10, 340, 600 ) );
	pTextWindow->SetID( TEXTWINDOW_Main );
	pTextWindow->SetFont("Arial", 16, 0, TRUE, FALSE, D3DCOLOR_RGBA( 230, 230, 230, 255 ) );
	pTextWindow->LoadTextFromFile( "game\\npcs\\ihin_bellatra\\main.txt" );
	pWindowMain->AddElement( pTextWindow );

	pList->AddWindow( pWindowMain );

	//Button OK
	UI::Button_ptr pButtonOk = std::make_shared<UI::Button>( Rectangle2D( 86, 342, 93, 23 ) );
	pButtonOk->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\btnok.png" ) );
	pButtonOk->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\btnok.png", "game\\images\\UI\\window\\btnok_.png" ) );
	pButtonOk->SetEvent( UI::Event::Build( std::bind( &CBellatraReward::OnButtonOKClick, this, std::placeholders::_1 ) ) );
	pWindow->AddElement( pButtonOk );

	//Button Close
	UI::Button_ptr pButtonClose = std::make_shared<UI::Button>( Rectangle2D( 186, 342, 93, 23 ) );
	pButtonClose->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\btnclose.png" ) );
	pButtonClose->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\btnclose.png", "game\\images\\UI\\window\\btnclose_.png" ) );
	pButtonClose->SetEvent( UI::Event::Build( std::bind( &CBellatraReward::OnButtonCloseClick, this, std::placeholders::_1 ) ) );
	pWindow->AddElement( pButtonClose );

	pWindow->Hide();
}

void CBellatraReward::Init()
{
	BuildWindow();
}

void CBellatraReward::Render()
{
	if ( pWindow->IsOpen() )
		pWindow->Render( 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

void CBellatraReward::Open()
{
	pWindow->Show();
	MOUSEHANDLER->BlockMouse();
}

void CBellatraReward::OnResolutionChanged()
{
	int iWindowWidth = pWindow->GetWidth();
	int iWindowHeight = pWindow->GetHeight();
	int iWindowX = (RESOLUTION_WIDTH >> 2) - (iWindowWidth >> 1) + 30;
	int iWindowY = (RESOLUTION_HEIGHT >> 2) - (iWindowHeight >> 1) + 78;
	pWindow->SetPosition( iWindowX, iWindowY );
}

void CBellatraReward::OnMouseMove( CMouse * pcMouse )
{
	pWindow->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

BOOL CBellatraReward::OnMouseClick( CMouse * pcMouse )
{
	if ( pWindow->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;

	return FALSE;
}

BOOL CBellatraReward::OnMouseScroll( CMouse * pcMouse )
{
	if ( pWindow->OnMouseScroll( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;

	return FALSE;
}

