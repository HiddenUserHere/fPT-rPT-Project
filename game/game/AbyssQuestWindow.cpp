#include "stdafx.h"
#include "AbyssQuestWindow.h"


CAbyssQuestWindow::CAbyssQuestWindow()
{
}


CAbyssQuestWindow::~CAbyssQuestWindow()
{
}

void CAbyssQuestWindow::OnResolutionChanged()
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

		//Base Rectangle
		iWindowWidth = pWindow2->GetWidth();
		iWindowHeight = pWindow2->GetHeight();
		iWindowX = ( RESOLUTION_WIDTH >> 2 ) + ( RESOLUTION_WIDTH >> 1 ) - ( iWindowWidth >> 1 ) - 10;
		iWindowY = ( RESOLUTION_HEIGHT >> 2 ) - ( iWindowHeight >> 1 ) + 78;

		pWindow2->SetPosition( iWindowX, iWindowY );
	}
}

void CAbyssQuestWindow::BuildWindow()
{
	pWindow1 = std::make_shared<UI::Window>( Rectangle2D( 20, 20, 364, 374 ) );
	pWindow1->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\window.png" ) );

	pWindow2 = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 320, 374 ) );
	pWindow2->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\window2.png" ) );

	pWindow3 = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 320, 374 ) );
	pWindow3->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\window2.png" ) );

	// Child Window
	{
		UI::TextWindow_ptr pTextWindowSub = std::make_shared<UI::TextWindow>( Rectangle2D( 26, 30, 280, 304 ) );
		pTextWindowSub->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
		pTextWindowSub->SetID( TEXTWINDOWID_Sub );
		pWindow2->AddElement( pTextWindowSub );

		UI::TextWindow_ptr pTextWindowSub1 = std::make_shared<UI::TextWindow>( Rectangle2D( 26, 30, 280, 304 ) );
		pTextWindowSub1->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
		pTextWindowSub1->SetID( TEXTWINDOWID_Sub );
		pWindow3->AddElement( pTextWindowSub1 );

		UI::Button_ptr pButtonOk = std::make_shared<UI::Button>( Rectangle2D( 62, 342, 93, 23 ) );
		pButtonOk->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\btnok.png" ) );
		pButtonOk->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\btnok.png", "game\\images\\UI\\window\\btnok_.png" ) );
		pButtonOk->SetEvent( UI::Event::Build( std::bind( &CAbyssQuestWindow::OnClickOK, this, std::placeholders::_1 ) ) );
		pWindow2->AddElement( pButtonOk );

		UI::Button_ptr pButtonOk2 = std::make_shared<UI::Button>( Rectangle2D( (pWindow3->GetWidth() >> 1) - (93 >> 1), 342, 93, 23 ) );
		pButtonOk2->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\btnok.png" ) );
		pButtonOk2->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\btnok.png", "game\\images\\UI\\window\\btnok_.png" ) );
		pButtonOk2->SetEvent( UI::Event::Build( std::bind( &CAbyssQuestWindow::OnClickOKFinish, this, std::placeholders::_1 ) ) );
		pWindow3->AddElement( pButtonOk2 );

		UI::Button_ptr pButtonClose = std::make_shared<UI::Button>( Rectangle2D( 162, 342, 93, 23 ) );
		pButtonClose->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\btnclose.png" ) );
		pButtonClose->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\btnclose.png", "game\\images\\UI\\window\\btnclose_.png" ) );
		pButtonClose->SetEvent( UI::Event::Build( std::bind( &CAbyssQuestWindow::OnClickClose, this, std::placeholders::_1 ) ) );
		pWindow2->AddElement( pButtonClose );

		pWindow2->Hide();
		pWindow3->Hide();
	}

	UI::Button_ptr pButtonCloseMain = std::make_shared<UI::Button>( Rectangle2D( (pWindow1->GetWidth() >> 1) - (93 >> 1), 342, 93, 23 ) );
	pButtonCloseMain->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\btnclose.png" ) );
	pButtonCloseMain->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\btnclose.png", "game\\images\\UI\\window\\btnclose_.png" ) );
	pButtonCloseMain->SetEvent( UI::Event::Build( std::bind( &CAbyssQuestWindow::OnClickCloseMain, this, std::placeholders::_1 ) ) );
	pWindow1->AddElement( pButtonCloseMain );

	//Left Window
	if ( pWindow1 )
	{
		//Base Rectangle
		int iWindowWidth = pWindow1->GetWidth();
		int iWindowHeight = pWindow1->GetHeight();
		int iWindowX = (RESOLUTION_WIDTH >> 2) - (iWindowWidth >> 1) + 30;
		int iWindowY = (RESOLUTION_HEIGHT >> 2) - (iWindowHeight >> 1) + 78;

		//iItemSlot
		pWindow1->SetPosition( iWindowX, iWindowY );
		pWindow3->SetPosition( iWindowX, iWindowY );

		//Base Rectangle
		iWindowWidth = pWindow2->GetWidth();
		iWindowHeight = pWindow2->GetHeight();
		iWindowX = (RESOLUTION_WIDTH >> 2) + (RESOLUTION_WIDTH >> 1) - (iWindowWidth >> 1) - 10;
		iWindowY = (RESOLUTION_HEIGHT >> 2) - (iWindowHeight >> 1) + 78;

		pWindow2->SetPosition( iWindowX, iWindowY );
	}

	UI::ImageBox_ptr pHeaderBG = std::make_shared<UI::ImageBox>( Rectangle2D( (pWindow1->GetWidth() >> 1) - (181 >> 1), -13, 181, 32 ) );
	pHeaderBG->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\text\\header.png" ) );
	pWindow1->AddElement( pHeaderBG );

	//Header Title
	UI::ImageBox_ptr pHeaderTitle = std::make_shared<UI::ImageBox>( Rectangle2D( (pWindow1->GetWidth() >> 1) - (47 >> 1), -4, 47, 16 ) );
	pHeaderTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\text\\header_quest.png" ) );
	pWindow1->AddElement( pHeaderTitle );

	//Text About
	UI::TextWindow_ptr pTextAbout = std::make_shared<UI::TextWindow>( Rectangle2D( 26, 38, 330, 200 ) );
	pTextAbout->LoadTextFromFile( "game\\quests\\quest_abyss\\hermit_main.txt" );
	pTextAbout->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pWindow1->AddElement( pTextAbout );

	//Label Select
	UI::Text_ptr pTextQuestLabel = std::make_shared<UI::Text>( Rectangle2D( 26, 200, 200, 20 ) );
	pTextQuestLabel->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextQuestLabel->SetText( "Please select a quest below for more information:" );
	pWindow1->AddElement( pTextQuestLabel );

	int iY = 225;

	//Label Quest 1
	UI::Text_ptr pTextQuest1 = std::make_shared<UI::Text>( Rectangle2D( 44, iY, 200, 20 ) );
	pTextQuest1->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pTextQuest1->SetText( "The Elite of Fear" );
	pTextQuest1->SetColorHover( D3DCOLOR_ARGB( 255, 255, 165, 0 ) );
	pTextQuest1->SetEvent( UI::Event::Build( std::bind( &CAbyssQuestWindow::OnButtonOpenQuestWindow, this, WINDOW_Quest1, std::placeholders::_1 ) ) );
	pWindow1->AddElement( pTextQuest1 );

	//Label Quest 2
	iY += 20;
	UI::Text_ptr pTextQuest2 = std::make_shared<UI::Text>( Rectangle2D( 44, iY, 200, 20 ) );
	pTextQuest2->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pTextQuest2->SetText( "The Return of Mutants" );
	pTextQuest2->SetColorHover( D3DCOLOR_ARGB( 255, 255, 165, 0 ) );
	pTextQuest2->SetEvent( UI::Event::Build( std::bind( &CAbyssQuestWindow::OnButtonOpenQuestWindow, this, WINDOW_Quest2, std::placeholders::_1 ) ) );
	pWindow1->AddElement( pTextQuest2 );

	//Label Quest 3
	iY += 20;
	UI::Text_ptr pTextQuest3 = std::make_shared<UI::Text>( Rectangle2D( 44, iY, 200, 20 ) );
	pTextQuest3->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pTextQuest3->SetText( "The Abyss Hunter" );
	pTextQuest3->SetColorHover( D3DCOLOR_ARGB( 255, 255, 165, 0 ) );
	pTextQuest3->SetEvent( UI::Event::Build( std::bind( &CAbyssQuestWindow::OnButtonOpenQuestWindow, this, WINDOW_Quest3, std::placeholders::_1 ) ) );
	pWindow1->AddElement( pTextQuest3 );

	//Label Quest 4
	iY += 20;
	UI::Text_ptr pTextQuest4 = std::make_shared<UI::Text>( Rectangle2D( 44, iY, 200, 20 ) );
	pTextQuest4->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pTextQuest4->SetText( "A Test of Loneliness" );
	pTextQuest4->SetColorHover( D3DCOLOR_ARGB( 255, 255, 165, 0 ) );
	pTextQuest4->SetEvent( UI::Event::Build( std::bind( &CAbyssQuestWindow::OnButtonOpenQuestWindow, this, WINDOW_Quest4, std::placeholders::_1 ) ) );
	pWindow1->AddElement( pTextQuest4 );

	//Label Quest 5
	iY += 20;
	UI::Text_ptr pTextQuest5 = std::make_shared<UI::Text>( Rectangle2D( 44, iY, 200, 20 ) );
	pTextQuest5->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pTextQuest5->SetText( "Under the Abyss" );
	pTextQuest5->SetColorHover( D3DCOLOR_ARGB( 255, 255, 165, 0 ) );
	pTextQuest5->SetEvent( UI::Event::Build( std::bind( &CAbyssQuestWindow::OnButtonOpenQuestWindow, this, WINDOW_Quest5, std::placeholders::_1 ) ) );
	pWindow1->AddElement( pTextQuest5 );

	iY = 225;
	UI::ImageBox_ptr pImageQuest = std::make_shared<UI::ImageBox>( Rectangle2D( 40 - 24, iY - 8, 32, 32 ) );
	pImageQuest->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\quest.png" ) );
	pWindow1->AddElement( pImageQuest );

	iY += 20;
	UI::ImageBox_ptr pImageQuest1 = std::make_shared<UI::ImageBox>( Rectangle2D( 40 - 24, iY - 8, 32, 32 ) );
	pImageQuest1->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\quest.png" ) );
	pWindow1->AddElement( pImageQuest1 );

	iY += 20;
	UI::ImageBox_ptr pImageQuest2 = std::make_shared<UI::ImageBox>( Rectangle2D( 40 - 24, iY - 8, 32, 32 ) );
	pImageQuest2->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\quest.png" ) );
	pWindow1->AddElement( pImageQuest2 );

	iY += 20;
	UI::ImageBox_ptr pImageQuest3 = std::make_shared<UI::ImageBox>( Rectangle2D( 40 - 24, iY - 8, 32, 32 ) );
	pImageQuest3->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\quest.png" ) );
	pWindow1->AddElement( pImageQuest3 );

	iY += 20;
	UI::ImageBox_ptr pImageQuest4 = std::make_shared<UI::ImageBox>( Rectangle2D( 40 - 24, iY - 8, 32, 32 ) );
	pImageQuest4->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\quest.png" ) );
	pWindow1->AddElement( pImageQuest4 );


	pWindow1->Hide();
}

void CAbyssQuestWindow::Init()
{
	BuildWindow();
}

void CAbyssQuestWindow::Render()
{
	return;

	pWindow1->Render( 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
	pWindow2->Render( 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
	pWindow3->Render( 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

void CAbyssQuestWindow::Open()
{
	if ( CanOpenNPC() )
		pWindow1->Show();
	else
	{
		UI::TextWindow_ptr pTextWindow = pWindow3->GetElement<UI::TextWindow>( TEXTWINDOWID_Sub );

		if ( iCurrentQuestID == QUESTID_TheEliteOfFear )
		{
			if ( IsFinishedQuest( iCurrentQuestID ) )
			{
				pTextWindow->LoadTextFromFile( "game\\quests\\quest_abyss\\1700_end.txt" );
				MOUSEHANDLER->BlockMouse();
			}
		}

		if ( iCurrentQuestID == QUESTID_TheReturnOfMutants )
		{
			if ( IsFinishedQuest( iCurrentQuestID ) )
			{
				pTextWindow->LoadTextFromFile( "game\\quests\\quest_abyss\\1701_end.txt" );
				MOUSEHANDLER->BlockMouse();
			}
		}

		if ( iCurrentQuestID == QUESTID_TheAbyssHunter )
		{
			if ( IsFinishedQuest( iCurrentQuestID ) )
			{
				pTextWindow->LoadTextFromFile( "game\\quests\\quest_abyss\\1702_end.txt" );
				MOUSEHANDLER->BlockMouse();
			}
		}

		if ( iCurrentQuestID == QUESTID_ATestofLoneliness )
		{
			if ( IsFinishedQuest( iCurrentQuestID ) )
			{
				pTextWindow->LoadTextFromFile( "game\\quests\\quest_abyss\\2013_end.txt" );
				MOUSEHANDLER->BlockMouse();
			}
		}
		if ( iCurrentQuestID == QUESTID_UndertheAbyss )
		{
			if ( IsFinishedQuest( iCurrentQuestID ) )
			{
				pTextWindow->LoadTextFromFile( "game\\quests\\quest_abyss\\2015_end.txt" );
				MOUSEHANDLER->BlockMouse();
			}
		}
	}
}

void CAbyssQuestWindow::Close()
{
	pWindow1->Hide();
	pWindow2->Hide();
	pWindow3->Hide();

	MOUSEHANDLER->UnblockMouse();
}

void CAbyssQuestWindow::OnMouseMove( CMouse * pcMouse )
{
	pWindow1->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
	pWindow2->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
	pWindow3->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

BOOL CAbyssQuestWindow::OnMouseClick( CMouse * pcMouse )
{
	if ( pWindow3->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;
	if ( pWindow2->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;
	if ( pWindow1->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;

	return FALSE;
}

BOOL CAbyssQuestWindow::OnMouseScroll( CMouse * pcMouse )
{
	if ( pWindow3->OnMouseScroll( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;
	if ( pWindow2->OnMouseScroll( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;
	if ( pWindow1->OnMouseScroll( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;

	return FALSE;
}

void CAbyssQuestWindow::OnButtonOpenQuestWindow( int iID, UIEventArgs e )
{
	//A Test of Loneliness
	if ( iID == WINDOW_Quest4 || iID == WINDOW_Quest5 )
	{
		if ( UNITDATA->sCharacterData.iLevel < 125 )
		{
			TITLEBOX( "You must be level %d!", 125 );
			return;
		}
	}
	else if ( UNITDATA->sCharacterData.iLevel < 125 )
	{
		TITLEBOX( "You must be level %d!", 125 );
		return;
	}

	if ( IsActiveQuests() )
	{
		TITLEBOX( "You already are in quest!" );
		return;
	}

	UI::TextWindow_ptr pTextWindow = pWindow2->GetElement<UI::TextWindow>( TEXTWINDOWID_Sub );

	switch ( iID )
	{
		case WINDOW_Quest1:
			pTextWindow->LoadTextFromFile( "game\\quests\\quest_abyss\\1700_start.txt" );
			pWindow2->Show();
			HIDERIGHTNPCBOX;
			iCurrentQuestID = QUESTID_TheEliteOfFear;
			break;

		case WINDOW_Quest2:
			pTextWindow->LoadTextFromFile( "game\\quests\\quest_abyss\\1701_start.txt" );
			pWindow2->Show();
			HIDERIGHTNPCBOX;
			iCurrentQuestID = QUESTID_TheReturnOfMutants;
			break;

		case WINDOW_Quest3:
			pTextWindow->LoadTextFromFile( "game\\quests\\quest_abyss\\1702_start.txt" );
			pWindow2->Show();
			HIDERIGHTNPCBOX;
			iCurrentQuestID = QUESTID_TheAbyssHunter;
			break;

		case WINDOW_Quest4:
			pTextWindow->LoadTextFromFile( "game\\quests\\quest_abyss\\2013_start.txt" );
			pWindow2->Show();
			HIDERIGHTNPCBOX;
			iCurrentQuestID = QUESTID_ATestofLoneliness;
			break;

		case WINDOW_Quest5:
			pTextWindow->LoadTextFromFile( "game\\quests\\quest_abyss\\2015_start.txt" );
			pWindow2->Show();
			HIDERIGHTNPCBOX;
			iCurrentQuestID = QUESTID_UndertheAbyss;
			break;

		default:
			break;
	}
}

void CAbyssQuestWindow::OnClickOK( UIEventArgs e )
{
	if ( CQUESTGAME->GetQuestInstance()->IsActiveQuest( iCurrentQuestID ) )
	{
		SHOWRIGHTNPCBOX;
		pWindow2->Hide();

		TITLEBOX( "You already have this quest!" );

		return;
	}

	if ( iCurrentQuestID == QUESTID_TheEliteOfFear )
	{
		// Show Box to accept the quest
		MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
		MESSAGEBOX->SetTitle( "Quest" );
		MESSAGEBOX->SetDescription( "Do you want to get the The Elite of Fear quest?" );
		MESSAGEBOX->SetEvent( std::bind( &CAbyssQuestWindow::OnClickYesBox, this, std::placeholders::_1 ) );
		MESSAGEBOX->Show();
	}

	if ( iCurrentQuestID == QUESTID_TheReturnOfMutants )
	{
		// Show Box to accept the quest
		MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
		MESSAGEBOX->SetTitle( "Quest" );
		MESSAGEBOX->SetDescription( "Do you want to get the The Return of Mutants quest?" );
		MESSAGEBOX->SetEvent( std::bind( &CAbyssQuestWindow::OnClickYesBox, this, std::placeholders::_1 ) );
		MESSAGEBOX->Show();
	}


	if ( iCurrentQuestID == QUESTID_TheAbyssHunter )
	{
		// Show Box to accept the quest
		MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
		MESSAGEBOX->SetTitle( "Quest" );
		MESSAGEBOX->SetDescription( "Do you want to get the The Abyss Hunter quest?" );
		MESSAGEBOX->SetEvent( std::bind( &CAbyssQuestWindow::OnClickYesBox, this, std::placeholders::_1 ) );
		MESSAGEBOX->Show();
	}

	if ( iCurrentQuestID == QUESTID_ATestofLoneliness )
	{
		// Show Box to accept the quest
		MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
		MESSAGEBOX->SetTitle( "Quest" );
		MESSAGEBOX->SetDescription( "Do you want to get the A Test of Loneliness quest?" );
		MESSAGEBOX->SetEvent( std::bind( &CAbyssQuestWindow::OnClickYesBox, this, std::placeholders::_1 ) );
		MESSAGEBOX->Show();
	}

	if ( iCurrentQuestID == QUESTID_UndertheAbyss )
	{
		// Show Box to accept the quest
		MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
		MESSAGEBOX->SetTitle( "Quest" );
		MESSAGEBOX->SetDescription( "Do you want to get the Under the Abyss quest?" );
		MESSAGEBOX->SetEvent( std::bind( &CAbyssQuestWindow::OnClickYesBox, this, std::placeholders::_1 ) );
		MESSAGEBOX->Show();
	}

	SHOWRIGHTNPCBOX;
	pWindow2->Hide();
}

void CAbyssQuestWindow::OnClickClose( UIEventArgs e )
{
	SHOWRIGHTNPCBOX;
	pWindow2->Hide();
}

void CAbyssQuestWindow::OnClickCloseMain( UIEventArgs e )
{
	Close();
	HIDERIGHTNPCBOX;
}

void CAbyssQuestWindow::OnClickYesBox( UIEventArgs e )
{
	if ( MESSAGEBOX->GetType() )
	{
		PacketQuestNPCAccept s;
		s.iID = iCurrentQuestID;

		CQUESTGAME->ProcessPacket( &s );
	}

	MOUSEHANDLER->BlockMouse();
}

BOOL CAbyssQuestWindow::CanOpenNPC()
{
	iCurrentQuestID = QUESTID_TheEliteOfFear;
	if ( CQUESTGAME->GetQuestInstance()->IsActiveQuest( iCurrentQuestID ) )
	{
		QuestData * pc = CQUESTGAME->GetQuestInstance()->GetQuest( iCurrentQuestID );
		if ( pc )
			return FALSE;
	}
	iCurrentQuestID = QUESTID_TheReturnOfMutants;
	if ( CQUESTGAME->GetQuestInstance()->IsActiveQuest( iCurrentQuestID ) )
	{
		QuestData * pc = CQUESTGAME->GetQuestInstance()->GetQuest( iCurrentQuestID );
		if ( pc )
			return FALSE;
	}
	iCurrentQuestID = QUESTID_TheAbyssHunter;
	if ( CQUESTGAME->GetQuestInstance()->IsActiveQuest( iCurrentQuestID ) )
	{
		QuestData * pc = CQUESTGAME->GetQuestInstance()->GetQuest( iCurrentQuestID );
		if ( pc )
			return FALSE;
	}
	iCurrentQuestID = QUESTID_ATestofLoneliness;
	if ( CQUESTGAME->GetQuestInstance()->IsActiveQuest( iCurrentQuestID ) )
	{
		QuestData * pc = CQUESTGAME->GetQuestInstance()->GetQuest( iCurrentQuestID );
		if ( pc )
			return FALSE;
	}
	iCurrentQuestID = QUESTID_UndertheAbyss;
	if ( CQUESTGAME->GetQuestInstance()->IsActiveQuest( iCurrentQuestID ) )
	{
		QuestData * pc = CQUESTGAME->GetQuestInstance()->GetQuest( iCurrentQuestID );
		if ( pc )
			return FALSE;
	}

	return TRUE;
}

BOOL CAbyssQuestWindow::IsActiveQuests()
{
	if ( CQUESTGAME->GetQuestInstance()->IsActiveQuest( QUESTID_TheEliteOfFear ) )
		return TRUE;

	if ( CQUESTGAME->GetQuestInstance()->IsActiveQuest( QUESTID_TheReturnOfMutants ) )
		return TRUE;

	if ( CQUESTGAME->GetQuestInstance()->IsActiveQuest( QUESTID_TheAbyssHunter ) )
		return TRUE;

	if ( CQUESTGAME->GetQuestInstance()->IsActiveQuest( QUESTID_ATestofLoneliness ) )
		return TRUE;

	if ( CQUESTGAME->GetQuestInstance()->IsActiveQuest( QUESTID_UndertheAbyss ) )
		return TRUE;

	return FALSE;
}

BOOL CAbyssQuestWindow::IsFinishedQuest( int iID )
{
	if ( CQUESTGAME->GetQuestInstance()->IsActiveQuest( iID ) )
	{
		QuestData * pc = CQUESTGAME->GetQuestInstance()->GetQuest( iID );
		if ( pc && pc->IsKillsFinished() )
			return TRUE;
	}

	return FALSE;
}

void CAbyssQuestWindow::OnClickOKFinish( UIEventArgs e )
{
	PacketQuestNPCFinish s;
	s.iID = iCurrentQuestID;
	CQUESTGAME->ProcessPacket( &s );

	Close();
}
