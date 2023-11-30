#include "stdafx.h"
#include "QuestWindow.h"

UI::List_ptr CQuestWindow::GetListMineFears()
{
	return pWindowIceMine->GetElement<UI::List>( LISTID_IM1_Main );
}

UI::Window_ptr CQuestWindow::GetWindowMineFears()
{
	return pWindowIceMine->GetElement<UI::Window>( WINDOWID_IM1_MineFears );
}

UI::Window_ptr CQuestWindow::GetWindowTullaMenace()
{
	return pWindowIceMine->GetElement<UI::Window>( WINDOWID_IM1_Tulla );
}

UI::Window_ptr CQuestWindow::GetWindowMineAlone()
{
	return pWindowIceMine->GetElement<UI::Window>( WINDOWID_IM1_MineAlone );
}

UI::List_ptr CQuestWindow::GetListMenu()
{
	return pWindowIceMine->GetElement<UI::List>( LISTID_IM1_Menu );
}

UI::Window_ptr CQuestWindow::GetWindowMain()
{
	return GetListMenu()->GetWindow( WINDOWID_IM1_Main );
}

void CQuestWindow::OnButtonAcceptClick( UIEventArgs e )
{
	if ( MESSAGEBOX->GetType() )
	{
		PacketQuestNPCAccept s;
		s.iID	= iCurrentID;

		CQUESTGAME->ProcessPacket( &s );
	}

	Close();
	GetWindowMain()->Show();
	HIDERIGHTNPCBOX;
	OnButtonCloseMineClick( UIEventArgs{} );
}

void CQuestWindow::OnButtonOKClick( UIEventArgs e )
{
	if ( CQUESTGAME->IsFinishing() == FALSE )
	{
		QuestData * pcQuestData = CQUESTGAME->GetQuestInstance()->GetQuest( iCurrentID );
		if ( pcQuestData == NULL )
		{
			MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
			MESSAGEBOX->SetTitle( "Quest" );
			MESSAGEBOX->SetDescription( FormatString("Do you want accept the %s quest?", szName) );
			MESSAGEBOX->SetEvent( std::bind( &CQuestWindow::OnButtonAcceptClick, this, std::placeholders::_1 ) );
			MESSAGEBOX->Show();
		}
		else
		{
			if ( pcQuestData->IsFinished() && (iNPCType == 3 || iNPCType == 4) )
			{
				if ( pcQuestData->CanFinishQuestWaitTime() == FALSE )
				{
					TITLEBOX( "You need to wait a time to finish this quest!" );
				}
				else
				{
					PacketQuestNPCFinish s;
					s.iID = pcQuestData->GetID();
					s.iItemIndex = 0;
					CQUESTGAME->ProcessPacket( &s );
				}
			}

			Close();
		}
	}
	else
		TITLEBOX( "You are in finishing quest process!" );
}

void CQuestWindow::OnButtonOKMineClick( UIEventArgs e )
{
	QuestData * pcQuestData = CQUESTGAME->GetQuestInstance()->GetQuest( iCurrentID );
	if ( pcQuestData == NULL )
	{
		MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
		MESSAGEBOX->SetTitle( "Quest" );
		MESSAGEBOX->SetDescription( FormatString("Do you want accept the %s quest?", szName) );
		MESSAGEBOX->SetEvent( std::bind( &CQuestWindow::OnButtonAcceptClick, this, std::placeholders::_1 ) );
		MESSAGEBOX->Show();
	}
	else
	{
		TITLEBOX( "You are doing this quest!" );
	}
}

void CQuestWindow::OnButtonOKT5QuestClick( UIEventArgs e )
{
	if ( CQUESTGAME->GetQuestInstance()->IsActiveQuest( QUESTID_T5QuestArena ) )
	{
		CQUESTGAME->OnQuestT5OpenProgress();
	}
	else
	{
		MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
		MESSAGEBOX->SetTitle( "Rank Up" );
		MESSAGEBOX->SetDescription( "Do you wanna get the quest?" );
		MESSAGEBOX->SetEvent( std::bind( &CQuestWindow::OnMessageBoxT5QuestStartClick, this, std::placeholders::_1 ) );
		MESSAGEBOX->Show();
	}
}

void CQuestWindow::OnMessageBoxT5QuestStartClick( UIEventArgs e )
{
	if ( MESSAGEBOX->GetType() == TRUE )
	{
		BOOL bContinue = FALSE;
		if ( PARTYHANDLER->GetPartyData().iMembersCount > 0 )
			TITLEBOX( "You cannot start this quest in party!" );
		else
			bContinue = TRUE;

		if( bContinue )
		{
			PacketQuestNPCAccept s;
			s.iID = QUESTID_T5QuestArena;
			CQUESTGAME->ProcessPacket( &s );

			PacketQuestArenaTier5 s1;
			s1.iLength	= sizeof( PacketQuestArenaTier5 );
			s1.iHeader	= PKTHDR_T5QuestArena;
			s1.iType	= PacketQuestArenaTier5::EType::TYPEID_EnterArena;
			SENDPACKETG( &s1 );
		}
	}

	pWindowT5Quest->Hide();
}

void CQuestWindow::OnButtonLeftArrowClick( UIEventArgs e )
{
	UI::List_ptr pListItems = pWindowItems->GetElement<UI::List>( LISTID_Items );
	if ( iItemIndex > 1 )
	{
		pListItems->GetWindow( iItemIndex )->Hide();
		iItemIndex--;
		pListItems->AddAxis( 233, 0 );
		pListItems->GetWindow( iItemIndex )->Show();

	}
}

void CQuestWindow::OnButtonRightArrowClick( UIEventArgs e )
{
	UI::List_ptr pListItems = pWindowItems->GetElement<UI::List>( LISTID_Items );
	if ( iItemIndex < pListItems->GetCountItemsList() )
	{
		pListItems->GetWindow( iItemIndex )->Hide();
		iItemIndex++;
		pListItems->AddAxis( -233, 0 );
		pListItems->GetWindow( iItemIndex )->Show();
	}
}

void CQuestWindow::OnButtonOKItemClick( UIEventArgs e )
{
	PacketQuestNPCFinish s;
	s.iID			= iCurrentID;
	s.iItemIndex	= iItemIndex;
	CQUESTGAME->ProcessPacket( &s );

	Close();
}

void CQuestWindow::OnButtonCloseClick( UIEventArgs e )
{
	if ( pWindow->IsOpen() == FALSE )
		Close();
}

void CQuestWindow::OnButtonCloseMineClick( UIEventArgs e )
{
	if ( GetWindowMain()->IsOpen() == FALSE )
	{
		OnButtonDifficultyClick( BUTTONID_IM1_Easy, UIEventArgs{} );
		GetWindowMineFears()->GetElement<UI::Group>( GROUPID_IM1_Difficulties )->SetIndexObject( BUTTONID_IM1_Easy );
		GetWindowMineFears()->Hide();
		GetWindowTullaMenace()->Hide();
		GetWindowMineAlone()->Hide();

		iCurrentID = 0;

		GetWindowMain()->Show();
	}
	else
	{
		OnButtonDifficultyClick( BUTTONID_IM1_Easy, UIEventArgs{} );
		GetWindowMineFears()->GetElement<UI::Group>( GROUPID_IM1_Difficulties )->SetIndexObject( BUTTONID_IM1_Easy );
		GetWindowMineFears()->Hide();
		GetWindowTullaMenace()->Hide();
		GetWindowMineAlone()->Hide();
		pWindowIceMine->Hide();

		Close();
		HIDERIGHTNPCBOX;
	}
}

void CQuestWindow::OnButtonDifficultyClick( int iID, UIEventArgs e )
{
	UI::Text_ptr pTextDifficulty = GetWindowMineFears()->GetElement<UI::Text>( TEXTID_IM1_Difficulty );
	UI::Text_ptr pTextRW = GetWindowMineFears()->GetElement<UI::Text>( TEXTID_IM1_Reward );
	UI::TextWindow_ptr pTextAbout = GetWindowMineFears()->GetElement<UI::TextWindow>( TEXTID_IM1_TextAbout );

	if ( iID == BUTTONID_IM1_Easy )
	{
		pTextDifficulty->SetText( "Easy" );
		pTextRW->SetTextEx( "%s EXP", FormatNumber( 12000000000 ) );
		iCurrentID = QUESTID_MinersFearsEasy;
		STRINGCOPY( szName, "Fear's Embrace(Easy)" );

		pTextAbout->SetParam( "{kill_count}", "100" );
	}
	else if ( iID == BUTTONID_IM1_Medium )
	{
		pTextDifficulty->SetText( "Medium" );
		pTextRW->SetTextEx( "%s EXP", FormatNumber( 16000000000 ) );
		iCurrentID = QUESTID_MinersFearsMedium;
		STRINGCOPY( szName, "Fear's Embrace(Medium)" );

		pTextAbout->SetParam( "{kill_count}", "140" );
	}
	else if ( iID == BUTTONID_IM1_Hard )
	{
		pTextDifficulty->SetText( "Hard" );
		pTextRW->SetTextEx( "%s EXP", FormatNumber( 20000000000 ) );
		iCurrentID = QUESTID_MinersFearsHard;
		STRINGCOPY( szName, "Fear's Embrace(Hard)" );

		pTextAbout->SetParam( "{kill_count}", "180" );
	}
}

void CQuestWindow::OnButtonOpenQuestWindow( int iID, UIEventArgs e )
{
	GetWindowMain()->Hide();

	if ( iID == WINDOWID_IM1_MineFears )
	{
		OnButtonDifficultyClick( BUTTONID_IM1_Easy, UIEventArgs{} );
		GetWindowMineFears()->GetElement<UI::Group>( GROUPID_IM1_Difficulties )->SetIndexObject( BUTTONID_IM1_Easy );
		GetWindowMineFears()->Show();
	}
	if ( iID == WINDOWID_IM1_Tulla )
	{
		GetWindowTullaMenace()->Show();
		iCurrentID = QUESTID_TullaMenace;
		STRINGCOPY( szName, "The Tulla Menace" );
	}
	if ( iID == WINDOWID_IM1_MineAlone )
	{
		if ( UNITDATA->sCharacterData.iLevel < 130 )
		{
			GetWindowMineAlone()->Show();
			iCurrentID = QUESTID_MineAlone;
			STRINGCOPY( szName, "Mine Alone" );
		}
		else
		{
			TITLEBOX( "You must be level 120-129!" );
			GetWindowMain()->Show();
		}
	}
}

void CQuestWindow::OnButtonOpenDynamicQuestWindow( int iID, UIEventArgs e )
{
	if ( pWindow->IsOpen() == FALSE )
	{
		PacketDynamicQuestNPCOpen s;
		s.iQuestID = iID;
		CQUESTGAME->ProcessPacket( &s );
	}
}

CQuestWindow::CQuestWindow()
{
	pcAbyssWindow = new CAbyssQuestWindow();
}


CQuestWindow::~CQuestWindow()
{
}

void CQuestWindow::BuildWindowDQTullaMenace( UI::List_ptr pList )
{
	//Body
	{
		UI::Window_ptr pWindowMain = std::make_shared<UI::Window>( Rectangle2D( 16, 26, 330, 1000 ) );
		pWindowMain->SetID( WINDOWID_IM1_Tulla );

		//Text About
		UI::TextWindow_ptr pTextAbout = std::make_shared<UI::TextWindow>( Rectangle2D( 10, 10, 330, 250 ) );
		pTextAbout->LoadTextFromFile( "game\\npcs\\larry_mine\\main_tullamenace.txt" );
		pTextAbout->SetFont( "Arial", 16, 0, FALSE, FALSE, D3DCOLOR_RGBA( 230, 230, 230, 255 ) );
		pWindowMain->AddElement( pTextAbout );

		//Background Image
		UI::ImageBox_ptr pImageBackgroundReward = std::make_shared<UI::ImageBox>( Rectangle2D( 2, 270, 248, 30 ) );
		pImageBackgroundReward->SetImage( UI::ImageLoader::LoadImage( "game\\images\\misc\\bgselect.png" ) );
		pWindowMain->AddElement( pImageBackgroundReward );

		//Start Button
		UI::Button_ptr pButtonStart = std::make_shared<UI::Button>( Rectangle2D( 250, 270, 80, 30 ) );
		pButtonStart->SetID( BUTTONID_IM1_TullaStart );
		pButtonStart->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\start_q.png", "game\\images\\UI\\window\\start_q_.png" ) );
		pButtonStart->SetEvent( UI::Event::Build( std::bind( &CQuestWindow::OnButtonOKMineClick, this, std::placeholders::_1 ) ) );
		pWindowMain->AddElement( pButtonStart );

		//Reward Label Text
		UI::Text_ptr pTextRWLabel = std::make_shared<UI::Text>( Rectangle2D( 36, 276, 100, 16 ) );
		pTextRWLabel->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
		pTextRWLabel->SetText( "Reward:" );
		pWindowMain->AddElement( pTextRWLabel );

		//Reward Text
		UI::Text_ptr pTextRW = std::make_shared<UI::Text>( Rectangle2D( 100, 278, 100, 16 ) );
		pTextRW->SetFont( "Arial", 14, 0, FALSE, FALSE, D3DCOLOR_RGBA( 230, 245, 230, 255 ) );
		pTextRW->SetText( "30,000,000,000 EXP" );
		pWindowMain->AddElement( pTextRW );

		pWindowMain->Hide();

		pWindowIceMine->AddElement( pWindowMain );
	}
}

void CQuestWindow::BuildWindowDQTheMinersFear( UI::List_ptr pList )
{
	//Body
	{
		UI::Window_ptr pWindowMain = std::make_shared<UI::Window>( Rectangle2D( 16, 26, 330, 1000 ) );
		pWindowMain->SetID( WINDOWID_IM1_MineFears );

		//Group Buttons
		UI::Group_ptr pGroupDifficulties = std::make_shared<UI::Group>();
		pGroupDifficulties->SetID( GROUPID_IM1_Difficulties );
		pWindowMain->AddElement( pGroupDifficulties );

		//Text About
		UI::TextWindow_ptr pTextAbout = std::make_shared<UI::TextWindow>( Rectangle2D( 10, 10, 330, 200 ) );
		pTextAbout->LoadTextFromFile( "game\\npcs\\larry_mine\\main_minefears.txt" );
		pTextAbout->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
		pTextAbout->SetID( TEXTID_IM1_TextAbout );
		pWindowMain->AddElement( pTextAbout );

		//Choose Text
		UI::Text_ptr pTextChoose = std::make_shared<UI::Text>( Rectangle2D( 10, 165, 100, 16 ) );
		pTextChoose->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
		pTextChoose->SetText( "Choose the difficulty:" );
		pWindowMain->AddElement( pTextChoose );

		//Difficulty Label
		UI::Text_ptr pTextDifficultyLabel = std::make_shared<UI::Text>( Rectangle2D( 10, 240, 100, 16 ) );
		pTextDifficultyLabel->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
		pTextDifficultyLabel->SetText( "Difficulty:" );
		pWindowMain->AddElement( pTextDifficultyLabel );

		//Difficulty Value
		UI::Text_ptr pTextDifficulty = std::make_shared<UI::Text>( Rectangle2D( 75, 243, 100, 16 ) );
		pTextDifficulty->SetFont( "Arial", 14, 0, FALSE, FALSE, D3DCOLOR_RGBA( 230, 230, 230, 255 ) );
		pTextDifficulty->SetText( "" );
		pTextDifficulty->SetID( TEXTID_IM1_Difficulty );
		pWindowMain->AddElement( pTextDifficulty );

		//Background Image
		UI::ImageBox_ptr pImageBackgroundReward = std::make_shared<UI::ImageBox>( Rectangle2D( 2, 270, 248, 30 ) );
		pImageBackgroundReward->SetImage( UI::ImageLoader::LoadImage( "game\\images\\misc\\bgselect.png" ) );
		pWindowMain->AddElement( pImageBackgroundReward );

		//Reward Label Text
		UI::Text_ptr pTextRWLabel = std::make_shared<UI::Text>( Rectangle2D( 36, 276, 100, 16 ) );
		pTextRWLabel->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
		pTextRWLabel->SetText( "Reward:" );
		pWindowMain->AddElement( pTextRWLabel );

		//Reward Text
		UI::Text_ptr pTextRW = std::make_shared<UI::Text>( Rectangle2D( 100, 278, 100, 16 ) );
		pTextRW->SetFont( "Arial", 14, 0, FALSE, FALSE, D3DCOLOR_RGBA( 230, 230, 230, 255 ) );
		pTextRW->SetID( TEXTID_IM1_Reward );
		pTextRW->SetText( "" );
		pWindowMain->AddElement( pTextRW );

		//Easy
		UI::Button_ptr pButtonEasy = std::make_shared<UI::Button>( Rectangle2D( 10, 185, 34, 34 ) );
		pButtonEasy->SetID( BUTTONID_IM1_Easy );
		pButtonEasy->SetGroup( pGroupDifficulties );
		pButtonEasy->SetSelect( TRUE );
		pButtonEasy->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\IM_Easy.png", "game\\images\\UI\\window\\IM_Easy_.png" ) );
		pButtonEasy->SetEvent( UI::Event::Build( std::bind( &CQuestWindow::OnButtonDifficultyClick, this, pButtonEasy->GetID(), std::placeholders::_1 ) ) );
		pWindowMain->AddElement( pButtonEasy );

		UI::Text_ptr pTextEasy = std::make_shared<UI::Text>( Rectangle2D( 14, 219, 34, 34 ) );
		pTextEasy->SetFont( "Tahoma", 12, 0, FALSE, TRUE, -1 );
		pTextEasy->SetText( "Easy" );
		pWindowMain->AddElement( pTextEasy );

		//Medium
		UI::Button_ptr pButtonMedium = std::make_shared<UI::Button>( Rectangle2D( 50, 185, 34, 34 ) );
		pButtonMedium->SetID( BUTTONID_IM1_Medium );
		pButtonMedium->SetGroup( pGroupDifficulties );
		pButtonMedium->SetSelect( TRUE );
		pButtonMedium->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\IM_Medium.png", "game\\images\\UI\\window\\IM_Medium_.png" ) );
		pButtonMedium->SetEvent( UI::Event::Build( std::bind( &CQuestWindow::OnButtonDifficultyClick, this, pButtonMedium->GetID(), std::placeholders::_1 ) ) );
		pWindowMain->AddElement( pButtonMedium );

		UI::Text_ptr pTextMedium = std::make_shared<UI::Text>( Rectangle2D( 48, 219, 34, 34 ) );
		pTextMedium->SetFont( "Tahoma", 12, 0, FALSE, TRUE, -1 );
		pTextMedium->SetText( "Medium" );
		pWindowMain->AddElement( pTextMedium );

		//Hard
		UI::Button_ptr pButtonHard = std::make_shared<UI::Button>( Rectangle2D( 88, 185, 34, 34 ) );
		pButtonHard->SetID( BUTTONID_IM1_Hard );
		pButtonHard->SetGroup( pGroupDifficulties );
		pButtonHard->SetSelect( TRUE );
		pButtonHard->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\IM_Hard.png", "game\\images\\UI\\window\\IM_Hard_.png" ) );
		pButtonHard->SetEvent( UI::Event::Build( std::bind( &CQuestWindow::OnButtonDifficultyClick, this, pButtonHard->GetID(), std::placeholders::_1 ) ) );
		pWindowMain->AddElement( pButtonHard );

		UI::Text_ptr pTextHard = std::make_shared<UI::Text>( Rectangle2D( 96, 219, 34, 34 ) );
		pTextHard->SetFont( "Tahoma", 12, 0, FALSE, TRUE, -1 );
		pTextHard->SetText( "Hard" );
		pWindowMain->AddElement( pTextHard );

		//Start Button
		UI::Button_ptr pButtonStart = std::make_shared<UI::Button>( Rectangle2D( 250, 270, 80, 30 ) );
		pButtonStart->SetID( BUTTONID_IM1_Start );
		pButtonStart->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\start_q.png", "game\\images\\UI\\window\\start_q_.png" ) );
		pButtonStart->SetEvent( UI::Event::Build( std::bind( &CQuestWindow::OnButtonOKMineClick, this, std::placeholders::_1 ) ) );
		pWindowMain->AddElement( pButtonStart );

		pWindowMain->Hide();

		pWindowIceMine->AddElement( pWindowMain );

	}
}

void CQuestWindow::BuildWindowSQMineAlone( UI::List_ptr pList )
{
	//Body
	{
		UI::Window_ptr pWindowMain = std::make_shared<UI::Window>( Rectangle2D( 16, 26, 330, 1000 ) );
		pWindowMain->SetID( WINDOWID_IM1_MineAlone );

		//Text About
		UI::TextWindow_ptr pTextAbout = std::make_shared<UI::TextWindow>( Rectangle2D( 10, 10, 330, 250 ) );
		pTextAbout->LoadTextFromFile( "game\\npcs\\larry_mine\\main_minealone.txt" );
		pTextAbout->SetFont( "Arial", 16, 0, FALSE, FALSE, D3DCOLOR_RGBA( 230, 230, 230, 255 ) );
		pWindowMain->AddElement( pTextAbout );

		//Background Image
		UI::ImageBox_ptr pImageBackgroundReward = std::make_shared<UI::ImageBox>( Rectangle2D( 2, 270, 248, 30 ) );
		pImageBackgroundReward->SetImage( UI::ImageLoader::LoadImage( "game\\images\\misc\\bgselect.png" ) );
		pWindowMain->AddElement( pImageBackgroundReward );

		//Start Button
		UI::Button_ptr pButtonStart = std::make_shared<UI::Button>( Rectangle2D( 250, 270, 80, 30 ) );
		pButtonStart->SetID( BUTTONID_IM1_MineAloneStart );
		pButtonStart->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\start_q.png", "game\\images\\UI\\window\\start_q_.png" ) );
		pButtonStart->SetEvent( UI::Event::Build( std::bind( &CQuestWindow::OnButtonOKMineClick, this, std::placeholders::_1 ) ) );
		pWindowMain->AddElement( pButtonStart );

		//Reward Label Text
		UI::Text_ptr pTextRWLabel = std::make_shared<UI::Text>( Rectangle2D( 36, 276, 100, 16 ) );
		pTextRWLabel->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
		pTextRWLabel->SetText( "Reward:" );
		pWindowMain->AddElement( pTextRWLabel );

		//Reward Text
		UI::Text_ptr pTextRW = std::make_shared<UI::Text>( Rectangle2D( 100, 278, 100, 16 ) );
		pTextRW->SetFont( "Arial", 14, 0, FALSE, FALSE, D3DCOLOR_RGBA( 230, 245, 230, 255 ) );
		pTextRW->SetText( "15,000,000,000 EXP" );
		pWindowMain->AddElement( pTextRW );

		pWindowMain->Hide();

		pWindowIceMine->AddElement( pWindowMain );
	}
}

void CQuestWindow::BuildWindowIceMine()
{
	pWindowIceMine = std::make_shared<UI::Window>( Rectangle2D( 20, 20, 364, 374 ) );
	pWindowIceMine->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\window.png" ) );

	int iWindowWidth = pWindowIceMine->GetWidth();
	int iWindowHeight = pWindowIceMine->GetHeight();
	int iWindowX = ( RESOLUTION_WIDTH >> 2 ) - ( iWindowWidth >> 1 ) + 30;
	int iWindowY = ( RESOLUTION_HEIGHT >> 2 ) - ( iWindowHeight >> 1 ) + 78;

	pWindowIceMine->SetPosition( iWindowX, iWindowY );

	UI::Window_ptr pWindowMain = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 330, 1000 ) );
	pWindowMain->SetID( WINDOWID_IM1_Main );

	UI::ImageBox_ptr pHeaderBG = std::make_shared<UI::ImageBox>( Rectangle2D( (pWindowIceMine->GetWidth() >> 1) - (181 >> 1), -13, 181, 32 ) );
	pHeaderBG->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\text\\header.png" ) );
	pWindowIceMine->AddElement( pHeaderBG );

	// Header Title
	UI::ImageBox_ptr pHeaderTitle = std::make_shared<UI::ImageBox>( Rectangle2D( (pWindowIceMine->GetWidth() >> 1) - (47 >> 1), -4, 47, 16 ) );
	pHeaderTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\text\\header_quest.png" ) );
	pWindowIceMine->AddElement( pHeaderTitle );

	//Text About
	UI::TextWindow_ptr pTextAbout = std::make_shared<UI::TextWindow>( Rectangle2D( 10, 10, 330, 200 ) );
	pTextAbout->LoadTextFromFile( "game\\npcs\\larry_mine\\main.txt" );
	pTextAbout->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pWindowMain->AddElement( pTextAbout );

	//Label Select
	UI::Text_ptr pTextQuestLabel = std::make_shared<UI::Text>( Rectangle2D( 10, 214, 200, 20 ) );
	pTextQuestLabel->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextQuestLabel->SetText( "Please select a quest below for more information:" );
	pWindowMain->AddElement( pTextQuestLabel );

	//Image Quest 1
	UI::ImageBox_ptr pImageQuest1 = std::make_shared<UI::ImageBox>( Rectangle2D( 0, 230, 32, 32 ) );
	pImageQuest1->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\quest.png" ) );
	pWindowMain->AddElement( pImageQuest1 );

	//Image Quest 2
	UI::ImageBox_ptr pImageQuest2 = std::make_shared<UI::ImageBox>( Rectangle2D( 0, 250, 32, 32 ) );
	pImageQuest2->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\quest.png" ) );
	pWindowMain->AddElement( pImageQuest2 );

	//Image Quest 3
	UI::ImageBox_ptr pImageQuest3 = std::make_shared<UI::ImageBox>( Rectangle2D( 0, 270, 32, 32 ) );
	pImageQuest3->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\quest.png" ) );
	pWindowMain->AddElement( pImageQuest3 );

	//Label Quest 1
	UI::Text_ptr pTextQuestTulla = std::make_shared<UI::Text>( Rectangle2D( 30, 238, 200, 20 ) );
	pTextQuestTulla->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pTextQuestTulla->SetText( "The Tulla Menace (Lv. 120)" );
	pTextQuestTulla->SetColorHover( D3DCOLOR_ARGB( 255, 255, 165, 0 ) );
	pTextQuestTulla->SetEvent( UI::Event::Build( std::bind( &CQuestWindow::OnButtonOpenQuestWindow, this, WINDOWID_IM1_Tulla, std::placeholders::_1 ) ) );
	pWindowMain->AddElement( pTextQuestTulla );

	//Label Quest 2
	UI::Text_ptr pTextQuestMineFears = std::make_shared<UI::Text>( Rectangle2D( 30, 258, 200, 20 ) );
	pTextQuestMineFears->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pTextQuestMineFears->SetText( "Fear's Embrace (Lv. 120)" );
	pTextQuestMineFears->SetColorHover( D3DCOLOR_ARGB( 255, 255, 165, 0 ) );
	pTextQuestMineFears->SetEvent( UI::Event::Build( std::bind( &CQuestWindow::OnButtonOpenQuestWindow, this, WINDOWID_IM1_MineFears, std::placeholders::_1 ) ) );
	pWindowMain->AddElement( pTextQuestMineFears );

	//Label Quest 3
	UI::Text_ptr pTextQuestMineAlone = std::make_shared<UI::Text>( Rectangle2D( 30, 278, 200, 20 ) );
	pTextQuestMineAlone->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pTextQuestMineAlone->SetText( "Mine Alone (Lv. 120)" );
	pTextQuestMineAlone->SetColorHover( D3DCOLOR_ARGB( 255, 255, 165, 0 ) );
	pTextQuestMineAlone->SetEvent( UI::Event::Build( std::bind( &CQuestWindow::OnButtonOpenQuestWindow, this, WINDOWID_IM1_MineAlone, std::placeholders::_1 ) ) );
	pWindowMain->AddElement( pTextQuestMineAlone );

	//Button OK
	UI::Button_ptr pButtonOK = std::make_shared<UI::Button>( Rectangle2D( (pWindowIceMine->GetWidth() >> 1) - (93 >> 1), 344, 93, 23 ) );
	pButtonOK->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\btnok.png", "game\\images\\UI\\window\\btnok_.png" ) );
	pButtonOK->SetEvent( UI::Event::Build( std::bind( &CQuestWindow::OnButtonCloseMineClick, this, std::placeholders::_1 ) ) );
	pWindowIceMine->AddElement( pButtonOK );

	UI::List_ptr pListMain = std::make_shared<UI::List>( Rectangle2D( 16, 26, 330, 310 ) );
	pListMain->SetID( LISTID_IM1_Main );

	UI::List_ptr pListMenu = std::make_shared<UI::List>( Rectangle2D( 16, 26, 330, 310 ) );
	pListMenu->SetID( LISTID_IM1_Menu );

	BuildWindowDQTheMinersFear( pListMain );
	BuildWindowDQTullaMenace( pListMain );
	BuildWindowSQMineAlone( pListMain );

	pListMenu->AddWindow( pWindowMain );

	pWindowIceMine->AddElement( pListMain );
	pWindowIceMine->AddElement( pListMenu );

	OnButtonDifficultyClick( BUTTONID_IM1_Easy, UIEventArgs{} );
	GetWindowMineFears()->GetElement<UI::Group>(GROUPID_IM1_Difficulties)->SetIndexObject( BUTTONID_IM1_Easy );

	pWindowIceMine->Hide();
}

void CQuestWindow::BuildWindowT5Quest()
{
	pWindowT5Quest = std::make_shared<UI::Window>( Rectangle2D( 20, 20, 364, 374 ) );

	//Base Rectangle
	int iWindowWidth = pWindowT5Quest->GetWidth();
	int iWindowHeight = pWindowT5Quest->GetHeight();
	int iWindowX = (RESOLUTION_WIDTH >> 2) - (iWindowWidth >> 1) + 30;
	int iWindowY = (RESOLUTION_HEIGHT >> 2) - (iWindowHeight >> 1) + 78;

	pWindowT5Quest->SetPosition( iWindowX, iWindowY );

	pWindowT5Quest->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\window.png" ) );

	UI::ImageBox_ptr pHeaderBG = std::make_shared<UI::ImageBox>( Rectangle2D( (pWindowT5Quest->GetWidth() >> 1) - (181 >> 1), -13, 181, 32 ) );
	pHeaderBG->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\text\\header.png" ) );
	pWindowT5Quest->AddElement( pHeaderBG );

	// Header Title
	UI::ImageBox_ptr pHeaderTitle = std::make_shared<UI::ImageBox>( Rectangle2D( (pWindowT5Quest->GetWidth() >> 1) - (47 >> 1), -4, 47, 16 ) );
	pHeaderTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\text\\header_quest.png" ) );
	pWindowT5Quest->AddElement( pHeaderTitle );

	UI::Button_ptr pButtonOK = std::make_shared<UI::Button>( Rectangle2D( (pWindowT5Quest->GetWidth() >> 1) - (93 >> 1), 342, 93, 23 ) );
	pButtonOK->SetID( BUTTONID_Ok );
	pButtonOK->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\btnok.png", "game\\images\\UI\\window\\btnok_.png" ) );
	pButtonOK->SetEvent( UI::Event::Build( std::bind( &CQuestWindow::OnButtonOKT5QuestClick, this, std::placeholders::_1 ) ) );
	pWindowT5Quest->AddElement( pButtonOK );

	UI::List_ptr pTextList = std::make_shared<UI::List>( Rectangle2D( 24, 34, 305, 296 ) );
	pTextList->SetID( LISTID_TextWindow );
	{
		UI::Window_ptr pWindowText = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 305, 296 ) );
		pWindowText->SetID( WINDOWID_Text );
		{
			UI::TextWindow_ptr pTextWindowMain = std::make_shared<UI::TextWindow>( Rectangle2D( 0, 0, 305, 296 ) );
			pTextWindowMain->SetID( TEXTID_Main );
			pTextWindowMain->SetText( "" );
			pTextWindowMain->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
			pWindowText->AddElement( pTextWindowMain );
		}
		pTextList->AddWindow( pWindowText );
		pTextList->MakeScrollBar( Rectangle2D( 314, 0, 6, 300 ), "game\\images\\UI\\scroll\\barbg_v.png", "game\\images\\UI\\scroll\\icon_2.png" );
	}

	pWindowT5Quest->AddElement( pTextList );

	pWindowT5Quest->Hide();
}

void CQuestWindow::BuildDynamicWindow()
{
	//Main Window
	{
		pDynamicWindow = std::make_shared<UI::Window>( Rectangle2D( 20, 20, 364, 374 ) );

		//Base Rectangle
		int iWindowWidth = pDynamicWindow->GetWidth();
		int iWindowHeight = pDynamicWindow->GetHeight();
		int iWindowX = (RESOLUTION_WIDTH >> 2) - (iWindowWidth >> 1) + 30;
		int iWindowY = (RESOLUTION_HEIGHT >> 2) - (iWindowHeight >> 1) + 78;

		pDynamicWindow->SetPosition( iWindowX, iWindowY );

		pDynamicWindow->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\window.png" ) );

		UI::ImageBox_ptr pHeaderBG = std::make_shared<UI::ImageBox>( Rectangle2D( (pDynamicWindow->GetWidth() >> 1) - (181 >> 1), -13, 181, 32 ) );
		pHeaderBG->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\text\\header.png" ) );
		pDynamicWindow->AddElement( pHeaderBG );

		// Header Title
		UI::ImageBox_ptr pHeaderTitle = std::make_shared<UI::ImageBox>( Rectangle2D( (pDynamicWindow->GetWidth() >> 1) - (47 >> 1), -4, 47, 16 ) );
		pHeaderTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\text\\header_quest.png" ) );
		pDynamicWindow->AddElement( pHeaderTitle );

		UI::List_ptr pMainList = std::make_shared<UI::List>( Rectangle2D( 24, 28, 314, 310 ) );
		pMainList->SetID( LISTID_DM_Main );

		pMainList->MakeScrollBar( Rectangle2D( 314, 0, 6, 300 ), "game\\images\\UI\\scroll\\barbg_v.png", "game\\images\\UI\\scroll\\icon_2.png" );

		pDynamicWindow->AddElement( pMainList );

		UI::Button_ptr pButtonOK = std::make_shared<UI::Button>( Rectangle2D( (pDynamicWindow->GetWidth() >> 1) - (93 >> 1), 342, 93, 23 ) );
		pButtonOK->SetID( BUTTONID_Ok );
		pButtonOK->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\btnok.png", "game\\images\\UI\\window\\btnok_.png" ) );
		pButtonOK->SetEvent( UI::Event::Build( std::bind( &CQuestWindow::OnButtonCloseClick, this, std::placeholders::_1 ) ) );
		pDynamicWindow->AddElement( pButtonOK );

		pDynamicWindow->Hide();
	}
}

void CQuestWindow::Init()
{
	//Main Window
	{
		pWindow = std::make_shared<UI::Window>( Rectangle2D( 20, 20, 364, 374 ) );

		//Base Rectangle
		int iWindowWidth = pWindow->GetWidth();
		int iWindowHeight = pWindow->GetHeight();
		int iWindowX = ( RESOLUTION_WIDTH >> 2 ) - ( iWindowWidth >> 1 ) + 30;
		int iWindowY = ( RESOLUTION_HEIGHT >> 2 ) - ( iWindowHeight >> 1 ) + 78;

		pWindow->SetPosition( iWindowX, iWindowY );

		pWindow->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\window.png" ) );

		UI::ImageBox_ptr pHeaderBG = std::make_shared<UI::ImageBox>( Rectangle2D( (pWindow->GetWidth() >> 1) - (181 >> 1), -13, 181, 32 ) );
		pHeaderBG->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\text\\header.png" ) );
		pWindow->AddElement( pHeaderBG );

		// Header Title
		UI::ImageBox_ptr pHeaderTitle = std::make_shared<UI::ImageBox>( Rectangle2D( (pWindow->GetWidth() >> 1) - (47 >> 1), -4, 47, 16 ) );
		pHeaderTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\text\\header_quest.png" ) );
		pWindow->AddElement( pHeaderTitle );

		UI::Button_ptr pButtonOK = std::make_shared<UI::Button>( Rectangle2D( (pWindow->GetWidth() >> 1) - (93 >> 1), 342, 93, 23 ) );
		pButtonOK->SetID( BUTTONID_Ok );
		pButtonOK->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\btnok.png", "game\\images\\UI\\window\\btnok_.png" ) );
		pButtonOK->SetEvent( UI::Event::Build( std::bind( &CQuestWindow::OnButtonOKClick, this, std::placeholders::_1 ) ) );
		pWindow->AddElement( pButtonOK );

		UI::List_ptr pTextList = std::make_shared<UI::List>( Rectangle2D( 24, 34, 320, 296 ) );
		pTextList->SetID( LISTID_TextWindow );
		{
			UI::Window_ptr pWindowText = std::make_shared<UI::Window>( Rectangle2D( 12, 12, 320, 0 ) );
			pWindowText->SetID( WINDOWID_Text );
			{
				UI::TextWindow_ptr pTextWindowMain = std::make_shared<UI::TextWindow>( Rectangle2D( 0, 0, 305, 0 ) );
				pTextWindowMain->SetID( TEXTID_Main );
				pTextWindowMain->SetText( "" );
				pTextWindowMain->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
				pWindowText->AddElement( pTextWindowMain );
			}
			pTextList->AddWindow( pWindowText );
			pTextList->MakeScrollBar( Rectangle2D( 314, 0, 6, 300 ), "game\\images\\UI\\scroll\\barbg_v.png", "game\\images\\UI\\scroll\\icon_2.png" );
		}

		pWindow->AddElement( pTextList );

		pWindow->Hide();
	}

	//Items Window
	{
		pWindowItems = std::make_shared<UI::Window>( Rectangle2D( 20, 20, 364, 374 ) );

		int iWindowWidth = pWindowItems->GetWidth();
		int iWindowHeight = pWindowItems->GetHeight();
		int iWindowX = ( RESOLUTION_WIDTH >> 2 ) - ( iWindowWidth >> 1 ) + 30;
		int iWindowY = ( RESOLUTION_HEIGHT >> 2 ) - ( iWindowHeight >> 1 ) + 78;

		pWindowItems->SetPosition( iWindowX, iWindowY );

		pWindowItems->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\window.png" ) );

		UI::ImageBox_ptr pHeaderBG = std::make_shared<UI::ImageBox>( Rectangle2D( (pWindowItems->GetWidth() >> 1) - (181 >> 1), -13, 181, 32 ) );
		pHeaderBG->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\text\\header.png" ) );
		pWindowItems->AddElement( pHeaderBG );

		// Header Title
		UI::ImageBox_ptr pHeaderTitle = std::make_shared<UI::ImageBox>( Rectangle2D( (pWindowItems->GetWidth() >> 1) - (47 >> 1), -4, 47, 16 ) );
		pHeaderTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\text\\header_quest.png" ) );
		pWindowItems->AddElement( pHeaderTitle );

		//List
		UI::List_ptr pListItems = std::make_shared<UI::List>( Rectangle2D( 20, 20, 340, 308 ) );
		pListItems->SetID( LISTID_Items );
		pListItems->SetHorizontal( TRUE );
		pListItems->SetPaddingSeparator( 2 );
		pListItems->SetCountAxisHidden( TRUE );
		pWindowItems->AddElement( pListItems );

		//Button Left
		UI::Button_ptr pButtonLeftArrow = std::make_shared<UI::Button>( Rectangle2D( 20, 151, 29, 61 ) );
		pButtonLeftArrow->SetID( BUTTONID_LeftArrow );
		pButtonLeftArrow->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\left.png", "game\\images\\UI\\window\\left_.png" ) );
		pButtonLeftArrow->SetEvent( UI::Event::Build( std::bind( &CQuestWindow::OnButtonLeftArrowClick, this, std::placeholders::_1 ) ) );
		pWindowItems->AddElement( pButtonLeftArrow );
		//Button Right
		UI::Button_ptr pButtonRightArrow = std::make_shared<UI::Button>( Rectangle2D( 314, 151, 29, 61 ) );
		pButtonRightArrow->SetID( BUTTONID_RightArrow );
		pButtonRightArrow->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\right.png", "game\\images\\UI\\window\\right_.png" ) );
		pButtonRightArrow->SetEvent( UI::Event::Build( std::bind( &CQuestWindow::OnButtonRightArrowClick, this, std::placeholders::_1 ) ) );
		pWindowItems->AddElement( pButtonRightArrow );

		//Button OK
		UI::Button_ptr pButtonOk = std::make_shared<UI::Button>( Rectangle2D( 86, 342, 93, 23 ) );
		pButtonOk->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\btnok.png" ) );
		pButtonOk->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\btnok.png", "game\\images\\UI\\window\\btnok_.png" ) );
		pButtonOk->SetEvent( UI::Event::Build( std::bind( &CQuestWindow::OnButtonOKItemClick, this, std::placeholders::_1 ) ) );
		pWindowItems->AddElement( pButtonOk );

		//Button Close
		UI::Button_ptr pButtonClose = std::make_shared<UI::Button>( Rectangle2D( 186, 342, 93, 23 ) );
		pButtonClose->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\btnclose.png" ) );
		pButtonClose->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\btnclose.png", "game\\images\\UI\\window\\btnclose_.png" ) );
		pButtonClose->SetEvent( UI::Event::Build( std::bind( &CQuestWindow::OnButtonCloseClick, this, std::placeholders::_1 ) ) );
		pWindowItems->AddElement( pButtonClose );

		pWindowItems->Hide();
	}

	BuildWindowIceMine();
	pcAbyssWindow->Init();

	BuildWindowT5Quest();

	BuildDynamicWindow();
}

void CQuestWindow::Open( int iID, char * pszName, char * pszDescription, int iNPCType )
{
	if ( pWindow->IsOpen() == FALSE )
	{
		iCurrentID = iID;
		this->iNPCType = iNPCType;

		STRINGCOPY( szName, pszName );

		UI::List_ptr pList = pWindow->GetElement<UI::List>( LISTID_TextWindow );
		UI::Window_ptr pWindow1 = pList->GetWindow( WINDOWID_Text );
		UI::TextWindow_ptr pText = pWindow1->GetElement<UI::TextWindow>( TEXTID_Main );
		pText->SetText( pszDescription );
		pText->SetBox( Rectangle2D( 0, 0, 320, 3000 ) );
		pText->SetBox( Rectangle2D( 0, 0, 320, pText->GetHeightText() ) );

		pWindow1->SetBox( Rectangle2D( 0, 0, 320, pText->GetHeightText() ) );
		pWindow->Show();

		MOUSEHANDLER->BlockMouse();
	}
}

void CQuestWindow::ClearDynamicQuest()
{
	//Clear Quest Vector
	{
		for ( std::vector<PacketDynamicQuestOpen::QuestData*>::iterator it = vDynamicQuests.begin(); it != vDynamicQuests.end(); it++ )
		{
			PacketDynamicQuestOpen::QuestData * ps = (*it);

			DELET( ps );
		}

		vDynamicQuests.clear();
	}

	//Reset Main Text TODO

	//Clear List
	{
		UI::List_ptr pList = pDynamicWindow->GetElement<UI::List>( LISTID_DM_Main );

		pList->Clear();
	}
}

void CQuestWindow::SetDynamicQuestMain( const char * pszImageMain, const char * pszTextMain )
{
	UI::List_ptr pList = pDynamicWindow->GetElement<UI::List>( LISTID_DM_Main );

	//Image Header
	if ( pszImageMain[0] != 0 )
	{
		UI::Image * pcImage = UI::ImageLoader::LoadImage( pszImageMain );

		if ( pcImage )
		{
			UI::Window_ptr pWindowImageHeader = std::make_shared<UI::Window>( Rectangle2D( 0, 0, pcImage->GetWidth(), pcImage->GetHeight() ) );
			pWindowImageHeader->SetImage( pcImage );
			pList->AddWindow( pWindowImageHeader );
		}
	}

	//Text Main
	if ( pszTextMain[0] != 0 )
	{
		UI::TextWindow_ptr pTextWindow = std::make_shared<UI::TextWindow>( Rectangle2D( 0, 0, pList->GetWidth(), 3000 ) );
		pTextWindow->SetFont( "Arial", 16, 0, FALSE, FALSE, D3DCOLOR_XRGB( 255, 255, 255 ) );
		pTextWindow->SetText( pszTextMain );
		pTextWindow->SetBox( Rectangle2D( 0, 0, pList->GetWidth(), pTextWindow->GetHeightText() ) );
		
		UI::Window_ptr pWindowText = std::make_shared<UI::Window>( Rectangle2D( 0, 0, pList->GetWidth(), pTextWindow->GetHeight() ) );
		pWindowText->AddElement( pTextWindow );

		pList->AddWindow( pWindowText );
	}
}

void CQuestWindow::AddDynamicQuestMainMenu( PacketDynamicQuestOpen::QuestData * psQuestData )
{
	PacketDynamicQuestOpen::QuestData * ps = new PacketDynamicQuestOpen::QuestData;
	ps->iQuestID		= psQuestData->iQuestID;
	ps->sRequiredLevel	= psQuestData->sRequiredLevel;
	ps->bDone			= psQuestData->bDone;
	STRINGCOPY( ps->szQuestName, psQuestData->szQuestName );
	vDynamicQuests.push_back( ps );

	//Add Menu
	UI::List_ptr pList = pDynamicWindow->GetElement<UI::List>( LISTID_DM_Main );
	UI::Window_ptr pMenuWindow = std::make_shared<UI::Window>( Rectangle2D( 0, 0, pDynamicWindow->GetWidth(), 26 ) );
	{
		//Image Quest
		UI::ImageBox_ptr pImageQuest = std::make_shared<UI::ImageBox>( Rectangle2D( -8, -3, 26, 26 ) );
		pImageQuest->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\quest.png" ) );
		pMenuWindow->AddElement( pImageQuest );

		UI::Text_ptr pTextMenu = std::make_shared<UI::Text>( Rectangle2D( 18, 6, 250, 20 ) );
		pTextMenu->SetFont( "Arial", 16, 0, FALSE, FALSE, D3DCOLOR_XRGB( 255, 255, 255 ) );

		if ( (ps->sRequiredLevel.iMin > 0) || (ps->sRequiredLevel.iMax > 0) )
		{
			if ( ps->sRequiredLevel.iMax > 0 )
				pTextMenu->SetTextEx( "%s (Req. Level: %d-%d)", ps->szQuestName, ps->sRequiredLevel.iMin, ps->sRequiredLevel.iMax );
			else
				pTextMenu->SetTextEx( "%s (Req. Level: %d)", ps->szQuestName, ps->sRequiredLevel.iMin );
		}
		else
			pTextMenu->SetText( ps->szQuestName );

		//pTextMenu->SetDisabled( ps->bDone ? TRUE : FALSE );
		pTextMenu->SetColorHover( D3DCOLOR_XRGB( 255, 165, 0 ) );
		pTextMenu->SetEvent( UI::Event::Build( std::bind( &CQuestWindow::OnButtonOpenDynamicQuestWindow, this, ps->iQuestID, std::placeholders::_1 ) ) );
		pMenuWindow->AddElement( pTextMenu );
		pList->AddWindow( pMenuWindow );
	}
}

void CQuestWindow::OpenDynamicQuestWindow()
{
	pDynamicWindow->Show();

	OnResolutionChanged();

	MOUSEHANDLER->BlockMouse();
}

void CQuestWindow::CloseDynamicQuestWindow()
{
	pDynamicWindow->Hide();
}

void CQuestWindow::OpenItems(int iID)
{
	iCurrentID = iID;
	iItemIndex = 1;
	pWindowItems->Show();
}

void CQuestWindow::OpenLarry()
{
	if ( CQUESTGAME->GetQuestInstance()->IsActiveQuest( QUESTID_TullaMenace ) || CQUESTGAME->GetQuestInstance()->IsActiveQuest( QUESTID_MinersFearsEasy ) ||
		CQUESTGAME->GetQuestInstance()->IsActiveQuest( QUESTID_MinersFearsMedium ) || CQUESTGAME->GetQuestInstance()->IsActiveQuest( QUESTID_MinersFearsHard ) ||
		CQUESTGAME->GetQuestInstance()->IsActiveQuest( QUESTID_MineAlone ) )
		return;

	if ( pWindowIceMine->IsOpen() == FALSE )
		pWindowIceMine->Show();

	GetWindowMain()->Show();
	MOUSEHANDLER->BlockMouse();
}

void CQuestWindow::OpenT5QuestWindow( BOOL bStart )
{
	UI::List_ptr pList = pWindowT5Quest->GetElement<UI::List>( LISTID_TextWindow );

	UI::Window_ptr pWindowText = pList->GetWindow( WINDOWID_Text );
	UI::TextWindow_ptr pText = pWindowText->GetElement<UI::TextWindow>( TEXTID_Main );

	if ( bStart )
		pText->LoadTextFromFile( "game\\quests\\quest_t5\\2105_start.txt" );
	else
		pText->LoadTextFromFile( "game\\quests\\quest_t5\\2105_finish.txt" );

	pWindowT5Quest->Show();
}

void CQuestWindow::CloseT5QuestWindow()
{
	pWindowT5Quest->Hide();
}

void CQuestWindow::AddItem( ItemData * pcItemData )
{
	UI::List_ptr pListItems = pWindowItems->GetElement<UI::List>( LISTID_Items );

	UI::Window_ptr pWindow = std::make_shared<UI::Window>( Rectangle2D( 50, 10, 230, 308 ) );
	pWindow->SetID( pListItems->GetCountItemsList() + 1 );

	UI::ItemBox_ptr pItemBox = std::make_shared<UI::ItemBox>( (pWindow->GetWidth() >> 1) - ((22*4) >> 1), (pWindow->GetHeight() >> 1) - ((22*6) >> 1), 4, 6 );

	int iX = pWindowItems->GetX() + pWindow->GetX() + pListItems->GetX();
	int iY = pWindowItems->GetY() + pWindow->GetY() + pListItems->GetY();

	pcItemData->bValid = TRUE;

	pItemBox->AddItem( pcItemData, iX, iY );

	pItemBox->DisableMouse();

	pWindow->AddElement( pItemBox );
	
	pListItems->AddWindow( pWindow );

	if ( pWindow->GetID() == 1 )
		pWindow->Show();
	else
		pWindow->Hide();
}

void CQuestWindow::Close()
{
	iCurrentID = 0;
	iItemIndex = 0;
	pWindow->Hide();
	MOUSEHANDLER->UnblockMouse();
	pWindowItems->Hide();

	UI::List_ptr pListItems = pWindowItems->GetElement<UI::List>( LISTID_Items );
	pListItems->Clear();

	pDynamicWindow->Hide();
}

void CQuestWindow::Render()
{
	if ( pWindow->IsOpen() )
		pWindow->Render( 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );

	if ( pWindowItems->IsOpen() )
		pWindowItems->Render( 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );

	if( pWindowIceMine->IsOpen() )
		pWindowIceMine->Render( 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );

	if ( pWindowT5Quest->IsOpen() )
		pWindowT5Quest->Render( 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );

	if ( pDynamicWindow->IsOpen() )
		pDynamicWindow->Render( 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );

	pcAbyssWindow->Render();
}

void CQuestWindow::OnResolutionChanged()
{
	int iX = RESOLUTION_WIDTH / 3;
	int iY = RESOLUTION_HEIGHT / 3;

	int iWindowWidth = pWindow->GetWidth();
	int iWindowHeight = pWindow->GetHeight();
	int iWindowX = ( RESOLUTION_WIDTH >> 2 ) - ( iWindowWidth >> 1 ) + 30;
	int iWindowY = ( RESOLUTION_HEIGHT >> 2 ) - ( iWindowHeight >> 1 ) + 78;

	pWindow->SetPosition( iWindowX, iWindowY );
	pWindowItems->SetPosition( iWindowX, iWindowY );
	pWindowIceMine->SetPosition( iWindowX, iWindowY );
	GetAbyssWindow()->OnResolutionChanged();
	pDynamicWindow->SetPosition( iWindowX, iWindowY );
	pWindowT5Quest->SetPosition( iWindowX, iWindowY );

	if ( pDynamicWindow->IsOpen() )
	{
		//Base Rectangle
		iWindowWidth = pWindow->GetWidth();
		iWindowHeight = pWindow->GetHeight();
		iWindowX = (RESOLUTION_WIDTH >> 2) + (RESOLUTION_WIDTH >> 1) - (iWindowWidth >> 1) - 10;
		iWindowY = (RESOLUTION_HEIGHT >> 2) - (iWindowHeight >> 1) + 78;

		pWindow->SetPosition( iWindowX, iWindowY );
	}
}

void CQuestWindow::OnMouseMove( CMouse * pcMouse )
{
	pWindow->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
	pWindowItems->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
	pWindowIceMine->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
	pWindowT5Quest->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
	pDynamicWindow->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
	GetAbyssWindow()->OnMouseMove( pcMouse );
}

BOOL CQuestWindow::OnMouseClick( CMouse * pcMouse )
{
	if ( pWindow->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;
	if ( pWindowItems->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;
	if ( pWindowIceMine->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;
	if ( GetAbyssWindow()->OnMouseClick( pcMouse ) )
		return TRUE;
	if ( pWindowT5Quest->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;
	if ( pDynamicWindow->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;

	return FALSE;
}

BOOL CQuestWindow::OnMouseScroll( CMouse * pcMouse )
{
	if ( pWindow->OnMouseScroll( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;
	if ( pWindowItems->OnMouseScroll( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;
	if ( pWindowIceMine->OnMouseScroll( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;
	if ( GetAbyssWindow()->OnMouseScroll( pcMouse ) )
		return TRUE;
	if ( pWindowT5Quest->OnMouseScroll( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;
	if ( pDynamicWindow->OnMouseScroll( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;

	return FALSE;
}
