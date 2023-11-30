#include "stdafx.h"
#include "WindowTest.h"


CWindowTest::CWindowTest()
{
}


CWindowTest::~CWindowTest()
{
}

void CWindowTest::Init()
{
	pWindow = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 652, 547 ) );
	//pWindow->SetCanMove( Rectangle2D( 0, 0, 560, 450 ), TRUE );
	//pWindow->SetEvent( UI::Event::Build( std::bind( &CWindowTest::Open, this, std::placeholders::_1 ) ) );
	//MOUSEHANDLER->BlockMouse();

	//Base Rectangle
	int iWindowWidth = pWindow->GetWidth();
    int iWindowHeight = pWindow->GetHeight();
	int iWindowX = (RESOLUTION_WIDTH >> 1) - (iWindowWidth >> 1);
	int iWindowY = (RESOLUTION_HEIGHT >> 1) - (iWindowHeight >> 1);

	//Usando settings window por agora
	pWindow->SetPosition( iWindowX, iWindowY );
	pWindow->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\window3.png" ) );

	Clear();

	//Button Close
	UI::Button_ptr pButtonClose = std::make_shared<UI::Button>( Rectangle2D( 589, 14, 30, 28 ) );
	pButtonClose->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\hud\\social\\closebtn.png", "game\\images\\hud\\social\\closebtn_.png" ) );
	pButtonClose->SetEvent( UI::Event::Build( std::bind( &CWindowTest::Close, this, std::placeholders::_1 ) ) );
	pWindow->AddElement( pButtonClose );

	pWindow->Hide();
}


BOOL CWindowTest::OnMouseClick( CMouse * pcMouse )
{
	return pWindow->OnMouseClick(pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

void CWindowTest::OnAllStatsClick( UIEventArgs e )
{
	// Show Box to accept the reset
	MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
	MESSAGEBOX->SetTitle( "Event Girl" );
	MESSAGEBOX->SetDescription( "Do you want to Reset All your Stats?\nPrice: 1,000,000 Gold" );
	//MESSAGEBOX->SetEvent( UI::Event::Build( std::bind( &CWindowTest::Close, this, std::placeholders::_1 ) ) );
	MESSAGEBOX->Show();
}

void CWindowTest::OnAllSkillsClick( UIEventArgs e )
{
	// Show Box to accept the reset
	MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
	MESSAGEBOX->SetTitle( "Event Girl" );
	MESSAGEBOX->SetDescription( "Do you want to Reset All your Skills?\nPrice: 1,000,000 Gold" );
	//MESSAGEBOX->SetEvent( UI::Event::Build( std::bind( &CWindowTest::Close, this, std::placeholders::_1 ) ) );
	MESSAGEBOX->Show();

}

void CWindowTest::OnResetButtonClick( UIEventArgs e )
{
	// Show Box to accept the reset
	MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
	MESSAGEBOX->SetTitle( "Event Girl" );
	MESSAGEBOX->SetDescription( "Do you want to Reset these stats?\nPrice: %d" );
	//MESSAGEBOX->SetEvent( UI::Event::Build( std::bind( &CWindowTest::Close, this, std::placeholders::_1 ) ) );
	MESSAGEBOX->Show();
}
void CWindowTest::OnMouseMove( CMouse * pcMouse )
{
	pWindow->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

BOOL CWindowTest::OnMouseScroll( CMouse * pcMouse )
{
	return pWindow->OnMouseScroll( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

void CWindowTest::Render()
{
	pWindow->Render( 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

void CWindowTest::Update( float fTime )
{
	pWindow->Update( fTime );
}

void CWindowTest::Open()
{
	if ( pWindow->IsOpen() == FALSE )
	{
		pWindow->Show();
		MOUSEHANDLER->BlockMouse();
	}

	Clear();
}

void CWindowTest::Close( UIEventArgs e )
{
	if ( pWindow->IsOpen() )
	{
		pWindow->Hide();
		MOUSEHANDLER->UnblockMouse();
	}
}

void CWindowTest::OnDOMReady( UIEventArgs e )
{
	//if ( auto pWebView = pWindow->GetElement<UI::WebViewer>( WEBVIEW_Test ) )
	//	pWebView->CallFunction( "appendRow([100, \"PROGUDO\", \"<img src = 'img/class/1.gif'>\", \"Mechanician\", \"IMPACT\", \"145\", \"0.000 % \"]);" );
}

void CWindowTest::Clear()
{
	//if ( auto pWebView = pWindow->GetElement<UI::WebViewer>( WEBVIEW_Test ) )
	//	pWindow->DelElement( pWebView );

}
