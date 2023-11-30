#include "stdafx.h"
#include "GrandFuryWindow.h"


CGrandFuryWindow::CGrandFuryWindow()
{
}


CGrandFuryWindow::~CGrandFuryWindow()
{
}

void CGrandFuryWindow::OnMouseMove( class CMouse * pcMouse )
{
	pWindow1->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
	pWindow2->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

BOOL CGrandFuryWindow::OnMouseClick( class CMouse * pcMouse )
{
	if ( pWindow2->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;
	if ( pWindow1->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;
	return FALSE;
}

BOOL CGrandFuryWindow::OnMouseScroll( class CMouse * pcMouse )
{
	if ( pWindow2->OnMouseScroll( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;
	if ( pWindow1->OnMouseScroll( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;
	return FALSE;
}

BOOL CGrandFuryWindow::OnKeyPress( class CKeyboard * pcKeyboard )
{
	if ( pWindow1->OnKeyPress( pcKeyboard ) )
		return TRUE;
	return FALSE;
}

BOOL CGrandFuryWindow::OnKeyChar( class CKeyboard * pcKeyboard )
{
	if ( pWindow1->OnKeyChar( pcKeyboard ) )
		return TRUE;
	return FALSE;
}

void CGrandFuryWindow::BuildWindow1()
{
	pWindow1 = std::make_shared<UI::Window>( Rectangle2D( 20, 20, 365, 375 ) );
	pWindow1->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\window.png" ) );

	//Base Rectangle
	int iWindowWidth = pWindow1->GetWidth();
	int iWindowHeight = pWindow1->GetHeight();
	int iWindowX = ( RESOLUTION_WIDTH >> 2 ) - ( iWindowWidth >> 1 ) + 30;
	int iWindowY = ( RESOLUTION_HEIGHT >> 2 ) - ( iWindowHeight >> 1 ) + 78;

	//iItemSlot
	pWindow1->SetPosition( iWindowX, iWindowY );
	
	// Header Background
	UI::ImageBox_ptr pHeaderBG = std::make_shared<UI::ImageBox>( Rectangle2D( (pWindow1->GetWidth() >> 1) - (181 >> 1), -13, 181, 32 ) );
	pHeaderBG->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\text\\header.png" ) );
	pWindow1->AddElement( pHeaderBG );

	// Header Title
	UI::ImageBox_ptr pHeaderTitle = std::make_shared<UI::ImageBox>( Rectangle2D( ( pWindow1->GetWidth() >> 1 ) - ( 47 >> 1 ), -4, 47, 16 ) );
	pHeaderTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\text\\header_event.png" ) );
	pWindow1->AddElement( pHeaderTitle );

	//List Main
	UI::List_ptr pList = std::make_shared<UI::List>( Rectangle2D( 20, 154, 316, 185 ) );
	pList->SetID( LISTID_Main );

	UI::Window_ptr pWindowList = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 316, 1000 ) );
	pWindowList->SetID( WINDOWID_ChildMain );
	pList->AddWindow( pWindowList );
	pWindow1->AddElement( pList );

	// Image Header
	UI::ImageBox_ptr pImageHeader = std::make_shared<UI::ImageBox>( Rectangle2D( 22, 65, 321, 120 ) );
	pImageHeader->SetID( IMAGEID_HeaderNPC );
	pImageHeader->SetImage( UI::ImageLoader::LoadImage( "game\\images\\events\\grandfury\\logoheader.png" ) );
	pWindow1->AddElement( pImageHeader );

	// Bar Time Countdown
	UI::ImageBox_ptr pImageCountdownBar = std::make_shared<UI::ImageBox>( Rectangle2D( 16, 35, 332, 21 ) );
	pImageCountdownBar->SetID( IMAGEID_BarCountdownTime );
	pImageCountdownBar->SetImage( UI::ImageLoader::LoadImage( "game\\images\\events\\grandfury\\bar.png" ) );
	pWindow1->AddElement( pImageCountdownBar );

	// Time Countdown
	UI::Text_ptr pTextTime = std::make_shared<UI::Text>( Rectangle2D( 30, 39, 100, 20 ) );
	pTextTime->SetText( "Countdown Time: 00:00:00" );
	pTextTime->SetID( TEXTID_Countdown );
	pTextTime->SetFont( "Arial", 14, 0, TRUE, FALSE, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	pWindow1->AddElement( pTextTime );

	// Read More
	UI::Text_ptr pTextReadMore = std::make_shared<UI::Text>( Rectangle2D( 260, 39, 100, 20 ) );
	pTextReadMore->SetText( "Read More..." );
	pTextReadMore->SetID( TEXTID_ReadMore );
	pTextReadMore->SetEvent( UI::Event::Build( std::bind( &CGrandFuryWindow::OnReadMoreButtonClick, this, std::placeholders::_1 ) ) );
	pTextReadMore->SetFont( "Arial", 14, 0, FALSE, TRUE, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	pTextReadMore->SetColorHover( D3DCOLOR_ARGB( 255, 255, 165, 0 ) );
	pWindow1->AddElement( pTextReadMore );

	// Next Boss Text
	UI::Text_ptr pTextBoss = std::make_shared<UI::Text>( Rectangle2D( 30, 190, 100, 20 ) );
	pTextBoss->SetText( "Next Boss:" );
	pTextBoss->SetFont( "Arial", 14, 0, FALSE, TRUE, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	pWindow1->AddElement( pTextBoss );

	// Background Boss Image
	UI::ImageBox_ptr pImageBackgroundBoss = std::make_shared<UI::ImageBox>( Rectangle2D( 30, 207, 157, 49 ) );
	pImageBackgroundBoss->SetID( IMAGEID_BackgroundBoss );
	pImageBackgroundBoss->SetImage( UI::ImageLoader::LoadImage( "game\\images\\events\\grandfury\\monbg.png" ) );
	pWindow1->AddElement( pImageBackgroundBoss );

	// Boss Image
	UI::ImageBox_ptr pImageBoss = std::make_shared<UI::ImageBox>( Rectangle2D( 35, 212, 38, 38 ) );
	pImageBoss->SetID( IMAGEID_Boss );
	pWindow1->AddElement( pImageBoss );

	// Boss Name 
	UI::Text_ptr pTextBossName = std::make_shared<UI::Text>( Rectangle2D( 90, 215, 100, 20 ) );
	pTextBossName->SetID( TEXTID_BossName );
	pTextBossName->SetText( "" );
	pTextBossName->SetFont( "Arial", 15, 0, FALSE, TRUE, D3DCOLOR_ARGB( 255, 210, 170, 120 ) );
	pWindow1->AddElement( pTextBossName );

	// Boss Level 
	UI::Text_ptr pTextBossLevel = std::make_shared<UI::Text>( Rectangle2D( 90, 230, 100, 20 ) );
	pTextBossLevel->SetID( TEXTID_BossLevel );
	pTextBossLevel->SetText( "Level: 0" );
	pTextBossLevel->SetFont( "Arial", 15, 0, FALSE, FALSE, D3DCOLOR_ARGB( 220, 255, 255, 255 ) );
	pWindow1->AddElement( pTextBossLevel );

	// Line
	UI::ImageBox_ptr pImageLine = std::make_shared<UI::ImageBox>( Rectangle2D( 28, 273, 316, 1 ) );
	pImageLine->SetImage( UI::ImageLoader::LoadImage( "game\\images\\events\\grandfury\\line.png" ) );
	pWindow1->AddElement( pImageLine );


	// Image Teleport
	UI::ImageBox_ptr pImageTeleport = std::make_shared<UI::ImageBox>( Rectangle2D( 53, 293, 4, 4 ) );
	pImageTeleport->SetID( IMAGEID_ArrowTeleport );
	pImageTeleport->SetImage( UI::ImageLoader::LoadImage( "game\\images\\events\\grandfury\\arrow.png" ) );
	pWindow1->AddElement( pImageTeleport );

	// Teleport to CT2
	UI::Text_ptr pTextTeleportCT2 = std::make_shared<UI::Text>( Rectangle2D( 64, 287, 220, 20 ) );
	pTextTeleportCT2->SetID( TEXTID_TeleportCT2 );
	pTextTeleportCT2->SetText( "Teleport to Boss Arena Entrance" );
	pTextTeleportCT2->SetEvent( UI::Event::Build( std::bind( &CGrandFuryWindow::OnTeleportCT2ButtonClick, this, std::placeholders::_1 ) ) );
	pTextTeleportCT2->SetFont( "Arial", 16, 0, FALSE, FALSE, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	pTextTeleportCT2->SetColorHover( D3DCOLOR_ARGB( 255, 255, 165, 0 ) );
	pWindow1->AddElement( pTextTeleportCT2 );

	// Teleport to TGF
	UI::Text_ptr pTextTeleportFA = std::make_shared<UI::Text>( Rectangle2D( 64, 287, 220, 20 ) );
	pTextTeleportFA->SetID( TEXTID_RequestEnter );
	pTextTeleportFA->SetText( "Teleport to Boss Arena" );
	pTextTeleportFA->SetEvent( UI::Event::Build( std::bind( &CGrandFuryWindow::OnTeleportFuryArenaButtonClick, this, std::placeholders::_1 ) ) );
	pTextTeleportFA->SetFont( "Arial", 16, 0, FALSE, FALSE, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	pTextTeleportFA->SetColorHover( D3DCOLOR_ARGB( 255, 255, 165, 0 ) );
	pTextTeleportFA->Hide();
	pWindow1->AddElement( pTextTeleportFA );

	// Image Exchange Item
	UI::ImageBox_ptr pImageExchange = std::make_shared<UI::ImageBox>( Rectangle2D( 53, 312, 4, 4 ) );
	pImageExchange->SetID( IMAGEID_ArrowExchangeItem );
	pImageExchange->SetImage( UI::ImageLoader::LoadImage( "game\\images\\events\\grandfury\\arrow.png" ) );
	pWindow1->AddElement( pImageExchange );

	// Exchange
	UI::Text_ptr pTextExchange = std::make_shared<UI::Text>( Rectangle2D( 64, 305, 100, 20 ) );
	pTextExchange->SetID( TEXTID_ExchangeItem );
	pTextExchange->SetText( "Exchange Item" );
	pTextExchange->SetEvent( UI::Event::Build( std::bind( &CGrandFuryWindow::OnExchangeButtonClick, this, std::placeholders::_1 ) ) );
	pTextExchange->SetFont( "Arial", 16, 0, FALSE, FALSE, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	pTextExchange->SetColorHover( D3DCOLOR_ARGB( 255, 255, 165, 0 ) );
	pWindow1->AddElement( pTextExchange );

	// Close Button
	UI::Button_ptr pButtonClose = std::make_shared<UI::Button>( Rectangle2D( 136, 342, 93, 23 ) );
	pButtonClose->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\btnclose.png", "game\\images\\UI\\window\\btnclose_.png" ) );
	pButtonClose->SetEvent( UI::Event::Build( std::bind( &CGrandFuryWindow::OnCloseButtonClick, this, std::placeholders::_1 ) ) );
	pWindow1->AddElement( pButtonClose );

	pWindow1->Hide();
}

void CGrandFuryWindow::BuildWindow2()
{
	pWindow2 = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 320, 374 ) );
	pWindow2->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\window2.png" ) );

	//Base Rectangle
	int iWindowWidth = pWindow2->GetWidth();
	int iWindowHeight = pWindow2->GetHeight();
	int iWindowX = ( RESOLUTION_WIDTH >> 2 ) + ( RESOLUTION_WIDTH >> 1 ) - ( iWindowWidth >> 1 ) - 10;
	int iWindowY = ( RESOLUTION_HEIGHT >> 2 ) - ( iWindowHeight >> 1 ) + 78;

	//iItemSlot
	pWindow2->SetPosition( iWindowX, iWindowY );

	// List Text
	UI::List_ptr pListText = std::make_shared<UI::List>( Rectangle2D( 20, 24, 270, 310 ) );
	pListText->SetID( LISTID_About );
	pListText->MakeScrollBar( Rectangle2D( 274, 4, 6, 300 ), "game\\images\\UI\\scroll\\barbg_v.png", "game\\images\\UI\\scroll\\icon_2.png" );
	pWindow2->AddElement( pListText );

	UI::Window_ptr pWindowList = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 270, 1000 ) );
	pWindowList->SetID( WINDOWID_ChildAbout );

	// Text About
	UI::TextWindow_ptr pTextAbout = std::make_shared<UI::TextWindow>( Rectangle2D( 0, 0, 270, 1000 ) );
	pTextAbout->SetID( TEXTWINDOWID_About );
	pTextAbout->SetFont( "Tahoma", 16, 0, FALSE, FALSE, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	pTextAbout->LoadTextFromFile( "game\\npcs\\grandfury\\about.txt" );
	pTextAbout->SetBox( Rectangle2D( 0, 0, 270, pTextAbout->GetHeightText() ) );
	pWindowList->SetBox( Rectangle2D( 0, 0, 270, pTextAbout->GetHeightText() ) );
	pWindowList->AddElement( pTextAbout );

	pListText->AddWindow( pWindowList );
	pWindow2->Hide();
}

void CGrandFuryWindow::OnResolutionChanged()
{
	//Left Window
	if( pWindow1 )
	{
		//Base Rectangle
		int iWindowWidth = pWindow1->GetWidth();
		int iWindowHeight = pWindow1->GetHeight();
		int iWindowX = ( RESOLUTION_WIDTH >> 2 ) - ( iWindowWidth >> 1 ) + 30;
		int iWindowY = ( RESOLUTION_HEIGHT >> 2 ) - ( iWindowHeight >> 1 ) + 78;

		//iItemSlot
		pWindow1->SetPosition( iWindowX, iWindowY );
	}

	//Right Window
	if( pWindow2 )
	{
		//Base Rectangle
		int iWindowWidth = pWindow2->GetWidth();
		int iWindowHeight = pWindow2->GetHeight();
		int iWindowX = ( RESOLUTION_WIDTH >> 2 ) + ( RESOLUTION_WIDTH >> 1 ) - ( iWindowWidth >> 1 ) - 30;
		int iWindowY = ( RESOLUTION_HEIGHT >> 2 ) - ( iWindowHeight >> 1 ) + 78;

		//iItemSlot
		pWindow2->SetPosition( iWindowX, iWindowY );
	}
}

void CGrandFuryWindow::OnReadMoreButtonClick( UIEventArgs e )
{
	UI::Text_ptr pText = pWindow1->GetElement<UI::Text>( TEXTID_ReadMore );
	if ( pText )
	{
		if ( !pWindow2->IsOpen() )
		{
			HIDERIGHTNPCBOX;
			pText->SetColor( D3DCOLOR_ARGB( 255, 255, 165, 0 ) );
			pWindow2->Show();
		}
		else
		{
			SHOWRIGHTNPCBOX;
			pText->SetColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			pWindow2->Hide();
		}
	}
}

void CGrandFuryWindow::OnTeleportCT2ButtonClick( UIEventArgs e )
{
	char szTempText[32] = { 0 };
	int iPrice			= (UNITDATA->sCharacterData.iLevel * 2000);
	CALL_WITH_ARG2( 0x00509880, iPrice, (DWORD)szTempText );

	if ( iPrice <= UNITDATA->sCharacterData.iGold )
	{
		// Level
		int iLevel = MAPLEVEL( MAPID_CursedTempleF2 );
		if ( iLevel <= UNITDATA->sCharacterData.iLevel )
		{
			MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
			MESSAGEBOX->SetTitle( "Are you sure?" );
			MESSAGEBOX->SetDescription( FormatString( "Do you want to teleport for Boss Arena?\nPrice: %s", szTempText ) );
			MESSAGEBOX->SetEvent( std::bind( &CGrandFuryWindow::OnAcceptTeleportCT2ButtonClick, this, std::placeholders::_1 ) );
			MESSAGEBOX->Show();
		}
		else
			TITLEBOX( "You must be level %d or above", iLevel );
	}
	else
		TITLEBOX( "Not enough gold" );
}

void CGrandFuryWindow::OnTeleportFuryArenaButtonClick( UIEventArgs e )
{
	if ( MOUNTHANDLER->IsRiding( UNITDATA ) )
	{
		MOUNTHANDLER->ShowMessage( MOUNTMESSAGE_ErrorEnterNPC );
		return;
	}

	DWORD dw = GRANDFURYHANDLER->GetTimeLeft();
	if ( dw >= (30) && dw <= (15 * 60) )
	{
		MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
		MESSAGEBOX->SetTitle( "Are you sure?" );
		MESSAGEBOX->SetDescription( "Do you want to teleport?" );
		MESSAGEBOX->SetEvent( std::bind( &CGrandFuryWindow::OnAcceptTeleportFuryArenaButtonClick, this, std::placeholders::_1 ) );
		MESSAGEBOX->Show();
	}
	else
		TITLEBOX( "You cannot enter at the Arena" );
}

void CGrandFuryWindow::OnAcceptTeleportCT2ButtonClick( UIEventArgs e )
{
	if ( MESSAGEBOX->GetType() == TRUE )
	{
		PacketFuryArenaRequest sPacket;
		sPacket.iLength = sizeof( PacketFuryArenaRequest );
		sPacket.iHeader = PKTHDR_FuryArena;
		sPacket.eState = FURYARENASTATEID_TeleportCT2;
		sPacket.p1 = UNITDATA->sCharacterData.iLevel;
		SENDPACKETL( &sPacket, TRUE );
		Close();
	}
}

void CGrandFuryWindow::OnAcceptTeleportFuryArenaButtonClick( UIEventArgs e )
{
	if ( MESSAGEBOX->GetType() == TRUE )
	{
		PacketFuryArenaRequest sPacket;
		sPacket.iLength = sizeof( PacketFuryArenaRequest );
		sPacket.iHeader = PKTHDR_FuryArena;
		sPacket.eState	= FURYARENASTATEID_RequestEnterArena;
		sPacket.p1		= 0;
		SENDPACKETG( &sPacket, TRUE );
		Close();
	}
}

void CGrandFuryWindow::OnAcceptExchangeButtonClick( UIEventArgs e )
{
	if ( MESSAGEBOX->GetType() == TRUE )
	{
		if ( TRADEHANDLER->IsWindowTradeOpen() )
		{
			TRADEHANDLER->CloseTradeWindow();
		}

		ItemData * ps = ITEMHANDLER->GetItemInventoryByCode( ITEMID_KingStone );
		if ( ps )
		{
			// Check iWeight
			if ( (UNITDATA->sCharacterData.sWeight.sCur + 200) > UNITDATA->sCharacterData.sWeight.sMax )
			{
				TITLEBOX( "Weight limit exceeded" );
				return;
			}

			// Check Space
			if ( !CHARACTERGAME->CheckInventorySpace( ITEMHANDLER->FindItemTableByCode( ITEMID_BrazilianSoccer ) ) )
			{
				TITLEBOX( "Not enough space in inventory" );
				return;
			}
			PacketFuryArenaRequest sPacket;
			sPacket.iLength = sizeof( PacketFuryArenaRequest );
			sPacket.iHeader = PKTHDR_FuryArena;
			sPacket.eState	= FURYARENASTATEID_Exchange;
			sPacket.p1		= 100;
			sPacket.p4		= ps->sItem.eMixTypeName;
			sPacket.p5		= ps->sItem.sQuestMonsters.sCur;
			sPacket.iLevel	= UNITDATA->sCharacterData.iLevel ^ 0x5767;
			SENDPACKETL( &sPacket, TRUE );
			ITEMHANDLER->DeleteItem( ps );
		}
		else
			TITLEBOX( "You don't have items to exchange" );
	}
}

void CGrandFuryWindow::OnExchangeButtonClick( UIEventArgs e )
{
	if ( ITEMHANDLER->GetItemAmountByCode( ITEMID_KingStone ) > 0 )
	{
		MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
		MESSAGEBOX->SetTitle( "Are you sure?" );
		MESSAGEBOX->SetDescription( "Do you want to make the Exchange?" );
		MESSAGEBOX->SetEvent( std::bind( &CGrandFuryWindow::OnAcceptExchangeButtonClick, this, std::placeholders::_1 ) );
		MESSAGEBOX->Show();
	}
	else
		TITLEBOX( "You don't have items to exchange" );
}

void CGrandFuryWindow::Open( int iNPC )
{
	if ( !pWindow1->IsOpen() )
	{
		UI::Text_ptr pTextTeleportCT2	= pWindow1->GetElement<UI::Text>( TEXTID_TeleportCT2 );
		UI::Text_ptr pTextTeleportFA	= pWindow1->GetElement<UI::Text>( TEXTID_RequestEnter );
		UI::Text_ptr pTextExchange		= pWindow1->GetElement<UI::Text>( TEXTID_ExchangeItem );
		UI::Text_ptr pTextBossName		= pWindow1->GetElement<UI::Text>( TEXTID_BossName );
		UI::Text_ptr pTextBossLevel		= pWindow1->GetElement<UI::Text>( TEXTID_BossLevel );
		UI::ImageBox_ptr pImageBoss		= pWindow1->GetElement<UI::ImageBox>( IMAGEID_Boss );

		if ( pTextBossName && pImageBoss && pTextTeleportCT2 && pTextTeleportFA && pTextExchange )
		{
			pTextBossName->SetText( FormatString( "%s", GRANDFURYHANDLER->GetEventID() == 0 ? "Hagios" : "King Fury" ) );
			pTextBossLevel->SetText( FormatString( "%s", GRANDFURYHANDLER->GetEventID() == 0 ? "Level: 130+" : "Level: 110+" ) );

			if ( GRANDFURYHANDLER->GetEventID() == 0 )
			{
				pImageBoss->SetImage( UI::ImageLoader::LoadImage( "game\\images\\events\\grandfury\\hagios.png" ) );
			}
			else
			{
				pImageBoss->SetImage( UI::ImageLoader::LoadImage( "game\\images\\events\\grandfury\\fury.png" ) );
			}

			// NPC ID
			if ( iNPC == 1 )
			{
				pTextTeleportCT2->Hide();
				pTextTeleportFA->Show();
				pTextExchange->SetDisabled( TRUE );

				// Text Read More
				UI::List_ptr pList = pWindow2->GetElement<UI::List>( LISTID_About );
				if ( pList )
				{
					UI::Window_ptr pWindow = pList->GetWindow( WINDOWID_ChildAbout );
					if ( pWindow )
					{
						UI::TextWindow_ptr pTextWindow = pWindow->GetElement<UI::TextWindow>( TEXTWINDOWID_About );
						if ( pTextWindow )
							pTextWindow->LoadTextFromFile( "game\\npcs\\grandfury\\entrance.txt" );
					}
				}
			}
			else
			{
				pTextTeleportCT2->Show();
				pTextTeleportFA->Hide();
				pTextExchange->SetDisabled( FALSE );

				// Text Read More
				UI::List_ptr pList = pWindow2->GetElement<UI::List>( LISTID_About );
				if ( pList )
				{
					UI::Window_ptr pWindow = pList->GetWindow( WINDOWID_ChildAbout );
					if ( pWindow )
					{
						UI::TextWindow_ptr pTextWindow = pWindow->GetElement<UI::TextWindow>( TEXTWINDOWID_About );
						if ( pTextWindow )
							pTextWindow->LoadTextFromFile( "game\\npcs\\grandfury\\about.txt" );
					}
				}
			}
		}
		pWindow1->Show();

		MOUSEHANDLER->BlockMouse();
	}
}

void CGrandFuryWindow::Close()
{
	if ( pWindow1->IsOpen() )
	{
		pWindow1->Hide();
		MOUSEHANDLER->UnblockMouse();
	}
	if ( pWindow2->IsOpen() )
		OnReadMoreButtonClick( UIEventArgs{} );

	HIDERIGHTNPCBOX;
}

void CGrandFuryWindow::SwapTest()
{
	UI::List_ptr pList = pWindow1->GetElement<UI::List>( LISTID_Main );
	if ( pList )
	{
		UI::Window_ptr pWindow = pList->GetWindow( WINDOWID_ChildMain );
		if ( pWindow )
		{
			UI::ImageBox_ptr pImage = pWindow->GetElement<UI::ImageBox>( IMAGEID_HeaderNPC );
			pImage->Swap();
		}
	}
}

void CGrandFuryWindow::Init()
{
	BuildWindow1();
	BuildWindow2();
}

void CGrandFuryWindow::Render()
{
	UI::Text_ptr pTextTime = pWindow1->GetElement<UI::Text>( TEXTID_Countdown );
	if ( pTextTime )
	{
		DWORD dw	= GRANDFURYHANDLER->GetTimeLeft();
		
		int iHour	= (dw / 3600);
		int iMinute	= (dw / 60) % 60;
		int iSecond = (dw % 60);

		pTextTime->SetText( FormatString( "Countdown Time: %02d:%02d:%02d", iHour, iMinute, iSecond ) );
	}
	pWindow1->Render( 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
	pWindow2->Render( 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}
