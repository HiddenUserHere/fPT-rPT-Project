#include "stdafx.h"
#include "CWareHouseWindow.h"


CWarehouseWindow::CWarehouseWindow()
{
	
}


CWarehouseWindow::~CWarehouseWindow()
{
}


void CWarehouseWindow::Render()
{
	if ( (*(BOOL*)(0x036932FC)) )
	{

		if ( COINHANDLER->GetWindow()->IsOpen() || (*(BOOL*)0x03642090) || (*(BOOL*)0x03653E40) )
		{
			pWindow1->Hide();

			(*(BOOL*)(0x036932FC)) = FALSE;

			(*(BOOL*)0x03653E40) = FALSE;

			(*(BOOL*)0x03642090) = FALSE;

			return;
		}

		if ( !pWindow1->IsOpen() )
			pWindow1->Show();
		pWindow1->SetPosition( WINDOW_WAREHOUSEMOVE - 384, 40 );
		pWindow1->Render( 0, 0, GRAPHICENGINE->GetWidthBackBuffer(), GRAPHICENGINE->GetHeightBackBuffer(), 0, 0 );
	}
	else
	{
		if ( pWindow1->IsOpen() )
		{
			WAREHOUSEHANDLER->SetPage( 1 );

			auto pButtonRight = pWindow1->GetElement<UI::Button>( BUTTONID_Right );
			pButtonRight->Enable();

			auto pButtonLeft = pWindow1->GetElement<UI::Button>( BUTTONID_Left );
			pButtonLeft->Disable();

			auto pText = pWindow1->GetElement<UI::Text>( TextID_Pages );
			pText->SetText( "1 / 5" );

			pWindow1->Hide();
		}
	}
	pWindowQuest1->Render( 0, 0, GRAPHICENGINE->GetWidthBackBuffer(), GRAPHICENGINE->GetHeightBackBuffer(), 0, 0 );
}

void CWarehouseWindow::OnMouseMove( class CMouse * pcMouse )
{
	if ( (*(BOOL*)(0x036932FC)) )
		pWindow1->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );

	pWindowQuest1->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

BOOL CWarehouseWindow::OnMouseClick( class CMouse * pcMouse )
{
	if ( (*(BOOL*)(0x036932FC)) )
	{
		bButtonClick = FALSE;

		if ( pWindow1->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		{
			if ( bButtonClick )
				return TRUE;
		}
	}

	if ( pWindowQuest1->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;

	return FALSE;
}

void CWarehouseWindow::OnButtonLeftClick( UIEventArgs eArgs )
{	
	WAREHOUSEHANDLER->SetPage( WAREHOUSEHANDLER->GetPage() - 1 );
	auto pText = pWindow1->GetElement<UI::Text>( TextID_Pages );

	char szBuf[32] = { 0 };
	STRINGFORMAT( szBuf, "%d / 5", WAREHOUSEHANDLER->GetPage() );
	pText->SetText( szBuf );

	if ( WAREHOUSEHANDLER->GetPage() == 1 )
	{
		auto pButtonLeft = pWindow1->GetElement<UI::Button>( BUTTONID_Left );
		pButtonLeft->Disable();
	}
	else if ( WAREHOUSEHANDLER->GetPage() < 5 )
	{
		auto pButtonRight = pWindow1->GetElement<UI::Button>( BUTTONID_Right );
		pButtonRight->Enable();
	}

	bButtonClick = TRUE;
}

void CWarehouseWindow::OnButtonRightClick( UIEventArgs eArgs )
{

	WAREHOUSEHANDLER->SetPage( WAREHOUSEHANDLER->GetPage() + 1 );
	
	auto pText = pWindow1->GetElement<UI::Text>( TextID_Pages );
	
	char szBuf[32] = { 0 };
	STRINGFORMAT( szBuf, "%d / 5", WAREHOUSEHANDLER->GetPage() );
	pText->SetText( szBuf );

	if ( WAREHOUSEHANDLER->GetPage() == 5 )
	{
		auto pButtonRight = pWindow1->GetElement<UI::Button>( BUTTONID_Right );
		pButtonRight->Disable();
	}
	else if ( WAREHOUSEHANDLER->GetPage() > 1 )
	{
		auto pButtonLeft = pWindow1->GetElement<UI::Button>( BUTTONID_Left );
		pButtonLeft->Enable();
	}

	bButtonClick = TRUE;
}

void CWarehouseWindow::OnAcceptQuest( UIEventArgs eArgs )
{
	QUESTGAMEHANDLER->StartQuest( QUESTID_Warehouse2Page );
}

void CWarehouseWindow::OnButtonQuestOkClick( UIEventArgs eArgs )
{
	pWindowQuest1->Hide();
}

void CWarehouseWindow::BuildWindow1()
{
	pWindow1 = std::make_shared<UI::Window>( Rectangle2D( 0, 40, 344, 264 ) );
	pWindowQuest1 = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 320, 374 ) );

	UI::Button_ptr pButtonLeft = std::make_shared<UI::Button>( Rectangle2D( 230, 152, 20, 20 ) );
	pButtonLeft->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\misc\\LeftArrow.png", "game\\images\\misc\\LeftArrow_.png" ) );
	pButtonLeft->SetDisabledImage( UI::ImageLoader::LoadImage( "game\\images\\misc\\LeftArrow.png" ) );
	pButtonLeft->SetID( BUTTONID_Left );
	pButtonLeft->Disable();
	pButtonLeft->SetEvent( UI::Event::Build( std::bind( &CWarehouseWindow::OnButtonLeftClick, this, std::placeholders::_1 ) ) );
	pWindow1->AddElement( pButtonLeft );

	UI::Button_ptr pButtonRight = std::make_shared<UI::Button>( Rectangle2D( 310, 152, 20, 20 ) );
	pButtonRight->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\misc\\RightArrow.png", "game\\images\\misc\\RightArrow_.png" ) );
	pButtonRight->SetDisabledImage( UI::ImageLoader::LoadImage( "game\\images\\misc\\RightArrow.png" ) );
	pButtonRight->SetID( BUTTONID_Right );
	pButtonRight->Disable();
	pButtonRight->SetEvent( UI::Event::Build( std::bind( &CWarehouseWindow::OnButtonRightClick, this, std::placeholders::_1 ) ) );
	pWindow1->AddElement( pButtonRight );

	UI::Text_ptr pTextPage = std::make_shared<UI::Text>( Rectangle2D( 268, 154, 20, 20 ) );
	pTextPage->SetID( TextID_Pages );
	pTextPage->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pTextPage->SetText( "1 / 5" );
	pWindow1->AddElement( pTextPage );

	{
		pWindowQuest1->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\window2.png" ) );

		UI::TextWindow_ptr pTextWindow = std::make_shared<UI::TextWindow>( Rectangle2D( 22, 32, 285, 305 ) );
		pTextWindow->SetID( TEXTWINDOW_Quest );
		pTextWindow->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
		pTextWindow->LoadTextFromFile( "game\\quests\\quest_wh\\Page2Begin.txt" );
		pWindowQuest1->AddElement( pTextWindow );

		UI::Button_ptr pButtonOKQuest = std::make_shared<UI::Button>( Rectangle2D( (320 >> 1) - (93 >> 1), 344, 93, 23 ) );
		pButtonOKQuest->SetID( BUTTONID_QuestOK );
		pButtonOKQuest->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\btnok.png" ) );
		pButtonOKQuest->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\btnok.png", "game\\images\\UI\\window\\btnok_.png" ) );
		pButtonOKQuest->SetEvent( UI::Event::Build( std::bind( &CWarehouseWindow::OnButtonQuestOkClick, this, std::placeholders::_1 ) ) );
		pWindowQuest1->AddElement( pButtonOKQuest );

		int iWindowWidth	= pWindowQuest1->GetWidth();
		int iWindowHeight	= pWindowQuest1->GetHeight();
		int iWindowX		= ( GRAPHICENGINE->GetWidthBackBuffer() >> 2 ) - ( iWindowWidth >> 1 ) + 30;
		int iWindowY		= ( GRAPHICENGINE->GetHeightBackBuffer() >> 2 ) - ( iWindowHeight >> 1 ) + 78;
		pWindowQuest1->SetPosition( iWindowX, iWindowY );

		pWindowQuest1->Hide();
	}
}

void CWarehouseWindow::OpenWindowQuest2Page()
{

	pWindowQuest1->Show();
}

void CWarehouseWindow::Init()
{
	BuildWindow1();
}
