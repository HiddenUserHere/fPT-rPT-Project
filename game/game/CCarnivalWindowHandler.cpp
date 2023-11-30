#include "stdafx.h"
#include "CCarnivalWindowHandler.h"

CCarnivalWindowHandler::CCarnivalWindowHandler()
{
    pWindow1 = std::make_shared<UI::Window>( Rectangle2D( 10, 10, 408, 417 ) );
}

CCarnivalWindowHandler::~CCarnivalWindowHandler()
{
    pWindow1 = nullptr;
}

void CCarnivalWindowHandler::Init()
{
	BuildWindow1();

	OnResolutionChanged();
}

void CCarnivalWindowHandler::OnResolutionChanged()
{
	pWindow1->SetPosition( (RESOLUTION_WIDTH >> 1) - (pWindow1->GetWidth() >> 1), (RESOLUTION_HEIGHT >> 1) - (pWindow1->GetHeight() >> 1) );
}

void CCarnivalWindowHandler::Open()
{
	if ( pWindow1->IsOpen() == FALSE )
	{
		pWindow1->Show();

		MOUSEHANDLER->BlockMouse();
	}
}

void CCarnivalWindowHandler::Close()
{
	if ( pWindow1->IsOpen() )
	{
		pWindow1->Hide();

		MOUSEHANDLER->UnblockMouse();
	}
}

void CCarnivalWindowHandler::BuildWindow1()
{
	pWindow1->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\window4.png" ) );

	//Header
	UI::ImageBox_ptr pHeaderImage = std::make_shared<UI::ImageBox>( Rectangle2D( 20, -13, 337, 51 ) );
	pHeaderImage->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\text\\header_event2.png" ) );
	pWindow1->AddElement( pHeaderImage );

	//Main List
	UI::List_ptr pListMain = std::make_shared<UI::List>( Rectangle2D( 22, 36, 350, 324 ) );
	pListMain->MakeScrollBar( Rectangle2D( 348, 0, 4, 320 ), "game\\images\\hud\\social\\scroll.png", "game\\images\\hud\\social\\scroller.png" );
	pWindow1->AddElement( pListMain );

	//Text Main
	UI::Window_ptr pWindowMain = std::make_shared<UI::Window>( Rectangle2D( 10, 0, 344, 3000 ) );
	UI::TextWindow_ptr pTextWindowMain = std::make_shared<UI::TextWindow>( Rectangle2D( 0, 0, 342, 3000 ) );
	pTextWindowMain->SetFont( "Constantia", 16, 0, TRUE, FALSE, DX::Color( 180, 255, 255, 255 ).Get() );
	pTextWindowMain->LoadTextFromFile( "game\\npcs\\carnival\\main.txt" );
	pWindowMain->SetBox( Rectangle2D( 10, 0, 344, pTextWindowMain->GetHeightText() ) );
	pTextWindowMain->SetBox( Rectangle2D( 0, 0, 342, pTextWindowMain->GetHeightText() ) );
	pWindowMain->AddElement( pTextWindowMain );
	pListMain->AddWindow( pWindowMain );

	//Button Close
	UI::Button_ptr pButtonClose = std::make_shared<UI::Button>( Rectangle2D( 348, 0, 30, 28 ) );
	pButtonClose->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\hud\\social\\closebtn.png", "game\\images\\hud\\social\\closebtn_.png" ) );
	pButtonClose->SetEvent( UI::Event::Build( std::bind( &CCarnivalWindowHandler::OnButtonCloseClick, this, std::placeholders::_1 ) ) );
	pWindow1->AddElement( pButtonClose );

	//Button Reward
	UI::Button_ptr pButtonReward = std::make_shared<UI::Button>( Rectangle2D( (pWindow1->GetWidth() >> 1) - (93 >> 1), pWindow1->GetHeight() - 58, 93, 40 ) );
	pButtonReward->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\rewardbtn_broyale.png", "game\\images\\UI\\window\\rewardbtn_broyale_.png" ) );
	pButtonReward->SetEvent( UI::Event::Build( std::bind( &CCarnivalWindowHandler::OnButtonRewardClick, this, std::placeholders::_1 ) ) );
	pWindow1->AddElement( pButtonReward );

	pWindow1->Hide();
}

void CCarnivalWindowHandler::OnButtonCloseClick( UIEventArgs eArgs )
{
	if ( dwCloseTime < TICKCOUNT )
		Close();
	else
		TITLEBOX( "Still in progress... wait %d seconds", (dwCloseTime - TICKCOUNT) / 1000 );
}

void CCarnivalWindowHandler::OnButtonRewardClick( UIEventArgs eArgs )
{
	if ( dwCloseTime < TICKCOUNT )
	{
		if ( CARNIVALHANDLER->HaveFruits() )
		{
			MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
			MESSAGEBOX->SetTitle( "EXCHANGE ITEMS" );
			MESSAGEBOX->SetDescription( "Do you want to exchange items?" );
			MESSAGEBOX->SetEvent( std::bind( &CCarnivalWindowHandler::OnAcceptRewardClick, this ) );
			MESSAGEBOX->SetAutoClose( 10 );
			MESSAGEBOX->Show();
		}
		else
			TITLEBOX( AY_OBFUSCATE( "You must have all fruits to exchange!" ) );
	}
	else
		TITLEBOX( "Still in progress... wait %d seconds", (dwCloseTime - TICKCOUNT) / 1000 );
}

void CCarnivalWindowHandler::OnAcceptRewardClick()
{
	if ( MESSAGEBOX->GetType() )
	{
		if ( CARNIVALHANDLER->HaveFruits() )
		{
			CARNIVALHANDLER->ExchangeItems();
			dwCloseTime = TICKCOUNT + 3000;
		}
	}
	else
		Close();
}


UIWINDOW_DEFAULT_SOURCE_CREATOR( CCarnivalWindowHandler, CCarnivalWindowHandler::pWindow1 );