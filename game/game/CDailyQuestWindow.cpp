#include "stdafx.h"
#include "CDailyQuestWindow.h"


DailyQuestWindow::DailyQuestWindow()
{
	pWindow1					= std::make_shared<UI::Window>( Rectangle2D( 0, 0, 364, 374 ) );
	pWindow2					= std::make_shared<UI::Window>( Rectangle2D( 0, 0, 320, 374 ) );
	pWindow3					= std::make_shared<UI::Window>( Rectangle2D( 0, 0, 320, 374 ) );
}


DailyQuestWindow::~DailyQuestWindow()
{
	pWindow1					= nullptr;
}

void DailyQuestWindow::OnSelect80A(UIEventArgs eArgs)
{
	// Level?
	if ( UNITDATA->sCharacterData.iLevel < 80 )
	{
		TITLEBOX( "You must be level %d!", 80 );
		return;
	}
	
	// Open Second window
	OpenSubWindow( QUESTID_DailyQuest80A );
}

void DailyQuestWindow::OnSelect85A(UIEventArgs eArgs)
{
	QuestStatusData * psQuestData = QUESTGAMEHANDLER->GetData();
	// Level?
	if ( UNITDATA->sCharacterData.iLevel < 85 )
	{
		TITLEBOX( "You must be level %d!", 85 );
		return;
	}

	// Open Second window
	OpenSubWindow( QUESTID_DailyQuest85A );
}

void DailyQuestWindow::OnSelect90A(UIEventArgs eArgs)
{
	QuestStatusData * psQuestData = QUESTGAMEHANDLER->GetData();
	// Level?
	if ( UNITDATA->sCharacterData.iLevel < 90 )
	{
		TITLEBOX( "You must be level %d!", 90 );
		return;
	}

	// Open Second window
	OpenSubWindow( QUESTID_DailyQuest90A );
}

void DailyQuestWindow::OnSelect95A(UIEventArgs eArgs)
{
	QuestStatusData * psQuestData = QUESTGAMEHANDLER->GetData();
	// Level?
	if ( UNITDATA->sCharacterData.iLevel < 95 )
	{
		TITLEBOX( "You must be level %d!", 95 );
		return;
	}

	// Open Second window
	OpenSubWindow( QUESTID_DailyQuest95A );
}

void DailyQuestWindow::OnSelect100A(UIEventArgs eArgs)
{
	QuestStatusData * psQuestData = QUESTGAMEHANDLER->GetData();
	// Level?
	if ( UNITDATA->sCharacterData.iLevel < 100 )
	{
		TITLEBOX( "You must be level %d!", 100 );
		return;
	}

	// Open Second window
	OpenSubWindow( QUESTID_DailyQuest100A );
}

void DailyQuestWindow::OnSelect105A( UIEventArgs eArgs )
{
	QuestStatusData * psQuestData = QUESTGAMEHANDLER->GetData();
	// Level?
	if ( UNITDATA->sCharacterData.iLevel < 105 )
	{
		TITLEBOX( "You must be level %d!", 105 );
		return;
	}
	// Open Second window
	OpenSubWindow( QUESTID_DailyQuest105A );
}

void DailyQuestWindow::OnSelect110A(UIEventArgs eArgs)
{
	QuestStatusData * psQuestData = QUESTGAMEHANDLER->GetData();
	// Level?
	if ( UNITDATA->sCharacterData.iLevel < 110 )
	{
		TITLEBOX( "You must be level %d!", 110 );
		return;
	}

	// Open Second window
	OpenSubWindow( QUESTID_DailyQuest110A );
}

void DailyQuestWindow::OnSelect115A(UIEventArgs eArgs)
{
	QuestStatusData * psQuestData = QUESTGAMEHANDLER->GetData();
	// Level?
	if ( UNITDATA->sCharacterData.iLevel < 115 )
	{
		TITLEBOX( "You must be level %d!", 115 );
		return;
	}
	// Open Second window
	OpenSubWindow( QUESTID_DailyQuest115A );
}

void DailyQuestWindow::OnClickOK( UIEventArgs eArgs )
{
}

void DailyQuestWindow::Open()
{

	if ( CQUESTGAME->GetQuestInstance()->IsActiveQuest( QUESTID_DailyQuest80A ) )
		return;
	if ( CQUESTGAME->GetQuestInstance()->IsActiveQuest( QUESTID_DailyQuest85A ) )
		return;
	if ( CQUESTGAME->GetQuestInstance()->IsActiveQuest( QUESTID_DailyQuest90A ) )
		return;
	if ( CQUESTGAME->GetQuestInstance()->IsActiveQuest( QUESTID_DailyQuest95A ) )
		return;
	if ( CQUESTGAME->GetQuestInstance()->IsActiveQuest( QUESTID_DailyQuest100A ) )
		return;
	if ( CQUESTGAME->GetQuestInstance()->IsActiveQuest( QUESTID_DailyQuest105A ) )
		return;
	if ( CQUESTGAME->GetQuestInstance()->IsActiveQuest( QUESTID_DailyQuest110A ) )
		return;
	if ( CQUESTGAME->GetQuestInstance()->IsActiveQuest( QUESTID_DailyQuest115A ) )
		return;

	if ( !pWindow1->IsOpen() )
	{
		pWindow1->Show();
		MOUSEHANDLER->BlockMouse();
	}
}

void DailyQuestWindow::Close(UIEventArgs eArgs)
{
	if( pWindow1->IsOpen() )
	{
		HIDERIGHTNPCBOX;
		pWindow1->Hide();
		MOUSEHANDLER->UnblockMouse();
	}
}

void DailyQuestWindow::OpenSubWindow( DWORD dwQuestID )
{
	switch ( dwQuestID )
	{
		case QUESTID_DailyQuest80A:
			if ( !pWindow2->IsOpen() )
			{
				UI::TextWindow_ptr pWindowText = pWindow2->GetElement<UI::TextWindow>( TEXTWINDOWID_Sub );
				pWindowText->LoadTextFromFile( "game\\quests\\quest_ian\\80a_start.txt" );
				pWindow2->Show();
				dwCurrentQuestID = dwQuestID;
			}
			break;

		case QUESTID_DailyQuest85A:
			if ( !pWindow2->IsOpen() )
			{
				UI::TextWindow_ptr pWindowText = pWindow2->GetElement<UI::TextWindow>( TEXTWINDOWID_Sub );
				pWindowText->LoadTextFromFile( "game\\quests\\quest_ian\\85a_start.txt" );
				pWindow2->Show();
				dwCurrentQuestID = dwQuestID;
			}
			break;

		case QUESTID_DailyQuest90A:
			if ( !pWindow2->IsOpen() )
			{
				UI::TextWindow_ptr pWindowText = pWindow2->GetElement<UI::TextWindow>( TEXTWINDOWID_Sub );
				pWindowText->LoadTextFromFile( "game\\quests\\quest_ian\\90a_start.txt" );
				pWindow2->Show();
				dwCurrentQuestID = dwQuestID;
			}
			break;

		case QUESTID_DailyQuest95A:
			if ( !pWindow2->IsOpen() )
			{
				UI::TextWindow_ptr pWindowText = pWindow2->GetElement<UI::TextWindow>( TEXTWINDOWID_Sub );
				pWindowText->LoadTextFromFile( "game\\quests\\quest_ian\\95a_start.txt" );
				pWindow2->Show();
				dwCurrentQuestID = dwQuestID;
			}
			break;

		case QUESTID_DailyQuest100A:
			if ( !pWindow2->IsOpen() )
			{
				UI::TextWindow_ptr pWindowText = pWindow2->GetElement<UI::TextWindow>( TEXTWINDOWID_Sub );
				pWindowText->LoadTextFromFile( "game\\quests\\quest_ian\\100a_start.txt" );
				pWindow2->Show();
				dwCurrentQuestID = dwQuestID;
			}
			break;

		case QUESTID_DailyQuest105A:
			if ( !pWindow2->IsOpen() )
			{
				UI::TextWindow_ptr pWindowText = pWindow2->GetElement<UI::TextWindow>( TEXTWINDOWID_Sub );
				pWindowText->LoadTextFromFile( "game\\quests\\quest_ian\\105a_start.txt" );
				pWindow2->Show();
				dwCurrentQuestID = dwQuestID;
			}
			break;

		case QUESTID_DailyQuest110A:
			if ( !pWindow2->IsOpen() )
			{
				UI::TextWindow_ptr pWindowText = pWindow2->GetElement<UI::TextWindow>( TEXTWINDOWID_Sub );
				pWindowText->LoadTextFromFile( "game\\quests\\quest_ian\\110a_start.txt" );
				pWindow2->Show();
				dwCurrentQuestID = dwQuestID;
			}
			break;

		case QUESTID_DailyQuest115A:
			if ( !pWindow2->IsOpen() )
			{
				UI::TextWindow_ptr pWindowText = pWindow2->GetElement<UI::TextWindow>( TEXTWINDOWID_Sub );
				pWindowText->LoadTextFromFile( "game\\quests\\quest_ian\\115a_start.txt" );
				pWindow2->Show();
				dwCurrentQuestID = dwQuestID;
			}
			break;

		default:
			break;
	}

	HIDERIGHTNPCBOX;
}


void DailyQuestWindow::OnClickYesBox(UIEventArgs eArgs)
{
	PacketQuestNPCAccept s;
	s.iID = dwCurrentQuestID;

	if ( MESSAGEBOX->GetType() )
	{

		if ( dwCurrentQuestID == QUESTID_DailyQuest80A )
			CQUESTGAME->ProcessPacket( &s );

		if ( dwCurrentQuestID == QUESTID_DailyQuest85A )
			CQUESTGAME->ProcessPacket( &s );

		if ( dwCurrentQuestID == QUESTID_DailyQuest90A )
			CQUESTGAME->ProcessPacket( &s );

		if ( dwCurrentQuestID == QUESTID_DailyQuest95A )
			CQUESTGAME->ProcessPacket( &s );

		if ( dwCurrentQuestID == QUESTID_DailyQuest100A )
			CQUESTGAME->ProcessPacket( &s );

		if ( dwCurrentQuestID == QUESTID_DailyQuest105A )
			CQUESTGAME->ProcessPacket( &s );

		if ( dwCurrentQuestID == QUESTID_DailyQuest110A )
			CQUESTGAME->ProcessPacket( &s );

		if ( dwCurrentQuestID == QUESTID_DailyQuest115A )
			CQUESTGAME->ProcessPacket( &s );

		pWindow1->Hide();
	}
}

void DailyQuestWindow::OnClickClose( UIEventArgs eArgs )
{
	SHOWRIGHTNPCBOX;

	pWindow2->Hide();
	pWindow3->Hide();
}

void DailyQuestWindow::OnMouseMove( class CMouse * pcMouse )
{
	pWindow1->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
	pWindow2->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
	pWindow3->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

BOOL DailyQuestWindow::OnMouseClick( class CMouse * pcMouse )
{
	if ( pWindow3->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;
	if ( pWindow2->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;
	if ( pWindow1->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;

	return FALSE;
}

BOOL DailyQuestWindow::OnKeyPress( class CKeyboard * pcKeyboard )
{
	if ( pWindow1->OnKeyPress( pcKeyboard ) )
		return TRUE;

	return FALSE;
}

BOOL DailyQuestWindow::OnKeyChar( class CKeyboard * pcKeyboard )
{
	if ( pWindow1->OnKeyChar( pcKeyboard ) )
		return TRUE;

	return FALSE;
}

void DailyQuestWindow::Render()
{
	pWindow1->Render( 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
	pWindow2->Render( 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
	pWindow3->Render( 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

void DailyQuestWindow::OnResolutionChanged()
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
		pWindow3->SetPosition( iWindowX, iWindowY );
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

void DailyQuestWindow::Init()
{
	BuildWindow1();
}

void DailyQuestWindow::BuildWindow1()
{
	// Set ID
	pWindow1->SetID( WINDOWID_Main );

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
		pWindow3->SetPosition( iWindowX, iWindowY );

		//Base Rectangle
		iWindowWidth = pWindow2->GetWidth();
		iWindowHeight = pWindow2->GetHeight();
		iWindowX = ( RESOLUTION_WIDTH >> 2 ) + ( RESOLUTION_WIDTH >> 1 ) - ( iWindowWidth >> 1 ) - 10;
		iWindowY = ( RESOLUTION_HEIGHT >> 2 ) - ( iWindowHeight >> 1 ) + 78;

		//iItemSlot
		pWindow2->SetPosition( iWindowX, iWindowY );
	}

	// Background
	pWindow1->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\window.png" ) );
	pWindow2->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\window2.png" ) );
	pWindow3->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\window2.png" ) );

	// Header Background
	UI::ImageBox_ptr pHeaderBG = std::make_shared<UI::ImageBox>( Rectangle2D( (pWindow1->GetWidth() >> 1) - (181 >> 1), -13, 181, 32 ) );
	pHeaderBG->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\text\\header.png" ) );
	pWindow1->AddElement( pHeaderBG );

	// Header Title
	UI::ImageBox_ptr pHeaderTitle = std::make_shared<UI::ImageBox>( Rectangle2D( ( pWindow1->GetWidth() >> 1 ) - ( 47 >> 1 ), -4, 47, 16 ) );
	pHeaderTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\text\\header_quest.png" ) );
	pWindow1->AddElement( pHeaderTitle );

	// Button Close
	UI::Button_ptr pButtonClose = std::make_shared<UI::Button>( Rectangle2D( 136, 342, 93, 23 ) );
	pButtonClose->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\btnclose.png" ) );
	pButtonClose->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\btnclose.png", "game\\images\\UI\\window\\btnclose_.png" ) );
	pButtonClose->SetEvent( UI::Event::Build( std::bind( &DailyQuestWindow::Close, this, std::placeholders::_1 ) ) );
	pWindow1->AddElement( pButtonClose );

	UI::TextWindow_ptr pTextWindow = std::make_shared<UI::TextWindow>( Rectangle2D( 26, 30, 320, 304 ) );
	pTextWindow->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pTextWindow->LoadTextFromFile( "game\\quests\\quest_ian\\main.txt" );
	pWindow1->AddElement( pTextWindow );

	UI::TextWindow_ptr pTextWindowWait = std::make_shared<UI::TextWindow>( Rectangle2D( 26, 30, 280, 304 ) );
	pTextWindowWait->SetID( TEXTWINDOWID_WaitQuest );
	pTextWindowWait->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pTextWindowWait->LoadTextFromFile( "game\\quests\\quest_ian\\main.txt" );
	pWindow3->AddElement( pTextWindowWait );

	// Text Links
	int iY = 170;
	UI::Text_ptr pTextQuest80A = std::make_shared<UI::Text>( Rectangle2D( 44, iY, 200, 17 ) );
	pTextQuest80A->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pTextQuest80A->SetText( "The Babel's Army ( Req. Level 80 )" );
	pTextQuest80A->SetEvent( UI::Event::Build( std::bind( &DailyQuestWindow::OnSelect80A, this, std::placeholders::_1 ) ) );
	pTextQuest80A->SetColorHover( D3DCOLOR_ARGB( 255, 255, 165, 0 ) );
	pWindow1->AddElement( pTextQuest80A );

	iY += 20;
	UI::Text_ptr pTextQuest85A = std::make_shared<UI::Text>( Rectangle2D( 44, iY, 200, 17 ) );
	pTextQuest85A->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pTextQuest85A->SetText( "The Second Army ( Req. Level 85 )" );
	pTextQuest85A->SetEvent( UI::Event::Build( std::bind( &DailyQuestWindow::OnSelect85A, this, std::placeholders::_1 ) ) );
	pTextQuest85A->SetColorHover( D3DCOLOR_ARGB( 255, 255, 165, 0 ) );
	pWindow1->AddElement( pTextQuest85A );

	iY += 20;
	UI::Text_ptr pTextQuest90A = std::make_shared<UI::Text>( Rectangle2D( 44, iY, 200, 17 ) );
	pTextQuest90A->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pTextQuest90A->SetText( "Help Andrei ( Req. Level 90 )" );
	pTextQuest90A->SetEvent( UI::Event::Build( std::bind( &DailyQuestWindow::OnSelect90A, this, std::placeholders::_1 ) ) );
	pTextQuest90A->SetColorHover( D3DCOLOR_ARGB( 255, 255, 165, 0 ) );
	pWindow1->AddElement( pTextQuest90A );

	iY += 20;
	UI::Text_ptr pTextQuest95A = std::make_shared<UI::Text>( Rectangle2D( 44, iY, 200, 17 ) );
	pTextQuest95A->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pTextQuest95A->SetText( "The Bosses Army ( Req. Level 95 )" );
	pTextQuest95A->SetEvent( UI::Event::Build( std::bind( &DailyQuestWindow::OnSelect95A, this, std::placeholders::_1 ) ) );
	pTextQuest95A->SetColorHover( D3DCOLOR_ARGB( 255, 255, 165, 0 ) );
	pWindow1->AddElement( pTextQuest95A );

	iY += 20;
	UI::Text_ptr pTextQuest100A = std::make_shared<UI::Text>( Rectangle2D( 44, iY, 200, 17 ) );
	pTextQuest100A->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pTextQuest100A->SetText( "The New Island ( Req. Level 100 )" );
	pTextQuest100A->SetEvent( UI::Event::Build( std::bind( &DailyQuestWindow::OnSelect100A, this, std::placeholders::_1 ) ) );
	pTextQuest100A->SetColorHover( D3DCOLOR_ARGB( 255, 255, 165, 0 ) );
	pWindow1->AddElement( pTextQuest100A );

	iY += 20;
	UI::Text_ptr pTextQuest105A = std::make_shared<UI::Text>( Rectangle2D( 44, iY, 200, 17 ) );
	pTextQuest105A->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pTextQuest105A->SetText( "The Lost Temple ( Req. Level 105 )" );
	pTextQuest105A->SetEvent( UI::Event::Build( std::bind( &DailyQuestWindow::OnSelect105A, this, std::placeholders::_1 ) ) );
	pTextQuest105A->SetColorHover( D3DCOLOR_ARGB( 255, 255, 165, 0 ) );
	pWindow1->AddElement( pTextQuest105A );

	iY += 20;
	UI::Text_ptr pTextQuest110A = std::make_shared<UI::Text>( Rectangle2D( 44, iY, 200, 17 ) );
	pTextQuest110A->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pTextQuest110A->SetText( "Help Muriel ( Req. Level 110 )" );
	pTextQuest110A->SetEvent( UI::Event::Build( std::bind( &DailyQuestWindow::OnSelect110A, this, std::placeholders::_1 ) ) );
	pTextQuest110A->SetColorHover( D3DCOLOR_ARGB( 255, 255, 165, 0 ) );
	pWindow1->AddElement( pTextQuest110A );

	iY += 20;
	UI::Text_ptr pTextQuest115A = std::make_shared<UI::Text>( Rectangle2D( 44, iY, 200, 17 ) );
	pTextQuest115A->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pTextQuest115A->SetText( "The Gorgonyte Menace ( Req. Level 115 )" );
	pTextQuest115A->SetEvent( UI::Event::Build( std::bind( &DailyQuestWindow::OnSelect115A, this, std::placeholders::_1 ) ) );
	pTextQuest115A->SetColorHover( D3DCOLOR_ARGB( 255, 255, 165, 0 ) );
	pWindow1->AddElement( pTextQuest115A );

	// Quest Icons
	iY = 170;
	UI::ImageBox_ptr pImageQuest = std::make_shared<UI::ImageBox>( Rectangle2D( 40 - 24, iY - 8, 32, 32 ) );
	pImageQuest->SetImage( UI::ImageLoader::LoadImage("game\\images\\UI\\window\\quest.png") );
	pWindow1->AddElement( pImageQuest );

	iY += 20;
	UI::ImageBox_ptr pImageQuest1 = std::make_shared<UI::ImageBox>( Rectangle2D( 40 - 24, iY - 8, 32, 32 ) );
	pImageQuest1->SetImage( UI::ImageLoader::LoadImage("game\\images\\UI\\window\\quest.png") );
	pWindow1->AddElement( pImageQuest1 );

	iY += 20;
	UI::ImageBox_ptr pImageQuest2 = std::make_shared<UI::ImageBox>( Rectangle2D( 40 - 24, iY - 8, 32, 32 ) );
	pImageQuest2->SetImage( UI::ImageLoader::LoadImage("game\\images\\UI\\window\\quest.png") );
	pWindow1->AddElement( pImageQuest2 );

	iY += 20;
	UI::ImageBox_ptr pImageQuest3 = std::make_shared<UI::ImageBox>( Rectangle2D( 40 - 24, iY - 8, 32, 32 ) );
	pImageQuest3->SetImage( UI::ImageLoader::LoadImage("game\\images\\UI\\window\\quest.png") );
	pWindow1->AddElement( pImageQuest3 );

	iY += 20;
	UI::ImageBox_ptr pImageQuest4 = std::make_shared<UI::ImageBox>( Rectangle2D( 40 - 24, iY - 8, 32, 32 ) );
	pImageQuest4->SetImage( UI::ImageLoader::LoadImage("game\\images\\UI\\window\\quest.png") );
	pWindow1->AddElement( pImageQuest4 );

	iY += 20;
	UI::ImageBox_ptr pImageQuest5 = std::make_shared<UI::ImageBox>( Rectangle2D( 40 - 24, iY - 8, 32, 32 ) );
	pImageQuest5->SetImage( UI::ImageLoader::LoadImage("game\\images\\UI\\window\\quest.png") );
	pWindow1->AddElement( pImageQuest5 );

	iY += 20;
	UI::ImageBox_ptr pImageQuest6 = std::make_shared<UI::ImageBox>( Rectangle2D( 40 - 24, iY - 8, 32, 32 ) );
	pImageQuest6->SetImage( UI::ImageLoader::LoadImage("game\\images\\UI\\window\\quest.png") );
	pWindow1->AddElement( pImageQuest6 );

	iY += 20;
	UI::ImageBox_ptr pImageQuest7 = std::make_shared<UI::ImageBox>( Rectangle2D( 40 - 24, iY - 8, 32, 32 ) );
	pImageQuest7->SetImage( UI::ImageLoader::LoadImage("game\\images\\UI\\window\\quest.png") );
	pWindow1->AddElement( pImageQuest7 );

	// Child Window
	{
		UI::TextWindow_ptr pTextWindowSub = std::make_shared<UI::TextWindow>( Rectangle2D( 26, 30, 280, 304 ) );
		pTextWindowSub->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
		pTextWindowSub->SetID( TEXTWINDOWID_Sub );
		pWindow2->AddElement( pTextWindowSub );

		UI::Button_ptr pButtonOk = std::make_shared<UI::Button>( Rectangle2D( 62, 342, 93, 23 ) );
		pButtonOk->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\btnok.png" ) );
		pButtonOk->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\btnok.png", "game\\images\\UI\\window\\btnok_.png" ) );
		pButtonOk->SetEvent( UI::Event::Build( std::bind( &DailyQuestWindow::OnClickOK, this, std::placeholders::_1 ) ) );
		pWindow2->AddElement( pButtonOk );

		UI::Button_ptr pButtonOk2 = std::make_shared<UI::Button>( Rectangle2D( ( pWindow3->GetWidth() >> 1 ) - ( 93 >> 1 ), 342, 93, 23 ) );
		pButtonOk2->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\btnok.png" ) );
		pButtonOk2->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\btnok.png", "game\\images\\UI\\window\\btnok_.png" ) );
		pButtonOk2->SetEvent( UI::Event::Build( std::bind( &DailyQuestWindow::OnClickOK, this, std::placeholders::_1 ) ) );
		pWindow3->AddElement( pButtonOk2 );

		UI::Button_ptr pButtonClose = std::make_shared<UI::Button>( Rectangle2D( 162, 342, 93, 23 ) );
		pButtonClose->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\btnclose.png" ) );
		pButtonClose->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\btnclose.png", "game\\images\\UI\\window\\btnclose_.png" ) );
		pButtonClose->SetEvent( UI::Event::Build( std::bind( &DailyQuestWindow::OnClickClose, this, std::placeholders::_1 ) ) );
		pWindow2->AddElement( pButtonClose );
	}
	
	pWindow1->Hide();

	pWindow2->Hide();
	pWindow3->Hide();
}