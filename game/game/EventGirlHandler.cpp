#include "stdafx.h"
#include "EventGirlHandler.h"

CEventGirlHandler::CEventGirlHandler()
{
}

CEventGirlHandler::~CEventGirlHandler()
{
}

void CEventGirlHandler::Init()
{
	pWindow = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 364, 374 ) );

	//Base Rectangle
	int iWindowWidth = pWindow->GetWidth();
	int iWindowHeight = pWindow->GetHeight();
	int iWindowX = ( RESOLUTION_WIDTH >> 2 ) - ( iWindowWidth >> 1 ) + 30;
	int iWindowY = ( RESOLUTION_HEIGHT >> 2 ) - ( iWindowHeight >> 1 ) + 78;

	pWindow->SetPosition( iWindowX, iWindowY );
	pWindow->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\window.png" ) );

	//Header
	UI::ImageBox_ptr pHeaderBG = std::make_shared<UI::ImageBox>( Rectangle2D( ( pWindow->GetWidth() >> 1 ) - ( 181 >> 1 ), -13, 181, 32 ) );
	pHeaderBG->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\text\\header.png" ) );
	pWindow->AddElement( pHeaderBG );

	//Header Title
	UI::ImageBox_ptr pHeaderTitle = std::make_shared<UI::ImageBox>( Rectangle2D( ( pWindow->GetWidth() >> 1 ) - ( 47 >> 1 ), -4, 47, 16 ) );
	pHeaderTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\text\\header_event.png" ) );
	pWindow->AddElement( pHeaderTitle );

	//Intro Text
	UI::TextWindow_ptr pTextWindow = std::make_shared<UI::TextWindow>( Rectangle2D( 26, 37, 313, 100 ) );
	pTextWindow->SetText( "Hi there! Here you can reset your Stats or your Skill Points, choose wisely and make sure you are not overwheight when picking up an item." );
	pTextWindow->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pWindow->AddElement( pTextWindow );

	//All Stats
	UI::Button_ptr pAllStats = std::make_shared<UI::Button>( Rectangle2D( 40, 109, 135, 29 ) );
	pAllStats->SetImage( UI::ImageLoader::LoadImage( "game\\images\\events\\event_girl\\allstats.png" ) );
	pAllStats->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\events\\event_girl\\allstats.png", "game\\images\\events\\event_girl\\allstats_.png" ) );
	pAllStats->SetEvent( UI::Event::Build( std::bind( &CEventGirlHandler::OnAllStatsClick, this, std::placeholders::_1 ) ) );
	pWindow->AddElement( pAllStats );

	//All Skills
	UI::Button_ptr pAllSkills = std::make_shared<UI::Button>( Rectangle2D( 187, 109, 135, 29 ) );
	pAllSkills->SetImage( UI::ImageLoader::LoadImage( "game\\images\\events\\event_girl\\allskills.png" ) );
	pAllSkills->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\events\\event_girl\\allskills.png", "game\\images\\events\\event_girl\\allskills_.png" ) );
	pAllSkills->SetEvent( UI::Event::Build( std::bind( &CEventGirlHandler::OnAllSkillsClick, this, std::placeholders::_1 ) ) );
	pWindow->AddElement( pAllSkills );

	UI::TextWindow_ptr pTextWindow1 = std::make_shared<UI::TextWindow>( Rectangle2D( 26, 162, 313, 36 ) );
	pTextWindow1->SetText( "Choose the specific stats to be reseted, each one has a cost of 200.000 gold" );
	pTextWindow1->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pWindow->AddElement( pTextWindow1 );

	int iY = 200;
	UI::CheckBox_ptr pCheckBoxStrenght = std::make_shared<UI::CheckBox>( Rectangle2D( 26, iY, 13, 13 ) );
	pCheckBoxStrenght->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxStrenght->SetText( "Strength" );
	pCheckBoxStrenght->SetID( CHECKBOXID_Strength );
	pCheckBoxStrenght->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindow->AddElement( pCheckBoxStrenght );

	iY += 20;
	UI::CheckBox_ptr pCheckBoxSpirit = std::make_shared<UI::CheckBox>( Rectangle2D( 26, iY, 13, 13 ) );
	pCheckBoxSpirit->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxSpirit->SetText( "Spirit" );
	pCheckBoxSpirit->SetID( CHECKBOXID_Spirit );
	pCheckBoxSpirit->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindow->AddElement( pCheckBoxSpirit );

	iY += 20;
	UI::CheckBox_ptr pCheckBoxTalent = std::make_shared<UI::CheckBox>( Rectangle2D( 26, iY, 13, 13 ) );
	pCheckBoxTalent->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxTalent->SetText( "Talent" );
	pCheckBoxTalent->SetID( CHECKBOXID_Talent );
	pCheckBoxTalent->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindow->AddElement( pCheckBoxTalent );

	iY += 20;
	UI::CheckBox_ptr pCheckBoxAgility = std::make_shared<UI::CheckBox>( Rectangle2D( 26, iY, 13, 13 ) );
	pCheckBoxAgility->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxAgility->SetText( "Agility" );
	pCheckBoxAgility->SetID( CHECKBOXID_Agility );
	pCheckBoxAgility->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindow->AddElement( pCheckBoxAgility );

	iY += 20;
	UI::CheckBox_ptr pCheckBoxHealth = std::make_shared<UI::CheckBox>( Rectangle2D( 26, iY, 13, 13 ) );
	pCheckBoxHealth->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxHealth->SetText( "Health" );
	pCheckBoxHealth->SetID( CHECKBOXID_Health );
	pCheckBoxHealth->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindow->AddElement( pCheckBoxHealth );

	//Background Value
	UI::ImageBox_ptr pBackgroundValue = std::make_shared<UI::ImageBox>( Rectangle2D( 132, 220, 187, 69 ) );
	pBackgroundValue->SetImage( UI::ImageLoader::LoadImage( "game\\images\\events\\event_girl\\bgvalue.png" ) );
	pWindow->AddElement( pBackgroundValue );

	//Button Reset Stats
	UI::Button_ptr pResetStats = std::make_shared<UI::Button>( Rectangle2D( 142, 252, 167, 22 ) );
	pResetStats->SetImage( UI::ImageLoader::LoadImage( "game\\images\\events\\event_girl\\reset.png" ) );
	pResetStats->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\events\\event_girl\\reset.png", "game\\images\\events\\event_girl\\reset_.png" ) );
	pResetStats->SetEvent( UI::Event::Build( std::bind( &CEventGirlHandler::OnResetButtonClick, this, std::placeholders::_1 ) ) );
	pWindow->AddElement( pResetStats );

	//Value Text
	UI::Text_ptr pTextValue = std::make_shared<UI::Text>( Rectangle2D( 145, 233, 180, 50 ) );
	pTextValue->SetText( "Total Price:" );
	pTextValue->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pWindow->AddElement( pTextValue );

	//Value Text
	UI::Text_ptr pTextValueGP = std::make_shared<UI::Text>( Rectangle2D( 147, 254, 180, 50 ) );
	pTextValueGP->SetID( TEXTID_StatusValue );
	pTextValueGP->SetText( "0 gp" );
	pTextValueGP->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pWindow->AddElement( pTextValueGP );

	//Button Close
	UI::Button_ptr pButtonClose = std::make_shared<UI::Button>( Rectangle2D( ( pWindow->GetWidth() >> 1) - (93 >> 1), 344, 93, 23 ) );
	pButtonClose->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\btnclose.png" ) );
	pButtonClose->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\btnclose.png", "game\\images\\UI\\window\\btnclose_.png" ) );
	pButtonClose->SetEvent( UI::Event::Build( std::bind( &CEventGirlHandler::OnCloseButtonClick, this, std::placeholders::_1 ) ) );
	pWindow->AddElement( pButtonClose );

	pWindow->Hide();
}


BOOL CEventGirlHandler::OnMouseClick( CMouse * pcMouse )
{
	return pWindow->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

void CEventGirlHandler::OnAllStatsClick( UIEventArgs e )
{
	int iGold = EVENTGIRL_ALLSTATS_PRICE;

	if ( IsFreeGold() )
		iGold = 0;

	// Show Box to accept the reset
	MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
	MESSAGEBOX->SetTitle( "Event Girl" );
	MESSAGEBOX->SetDescription( FormatString( "Do you want to Reset All your Stats?\nPrice: %s gp", FormatNumber( iGold ) ) );
	MESSAGEBOX->SetEvent( std::bind( &CEventGirlHandler::OnAcceptAllStats, this, std::placeholders::_1 ) );
	MESSAGEBOX->Show();
}

void CEventGirlHandler::OnAllSkillsClick( UIEventArgs e )
{
	int iGold = EVENTGIRL_ALLSKILLS_PRICE;

	if ( IsFreeGold() )
		iGold = 0;

	// Show Box to accept the reset
	MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
	MESSAGEBOX->SetTitle( "Event Girl" );
	MESSAGEBOX->SetDescription( FormatString( "Do you want to Reset All your Skills?\nPrice: %s gp", FormatNumber( iGold ) ) );
	MESSAGEBOX->SetEvent( std::bind( &CEventGirlHandler::OnAcceptAllSkills, this, std::placeholders::_1 ) );
	MESSAGEBOX->Show();

}

void CEventGirlHandler::OnResetButtonClick( UIEventArgs e )
{
	if( (iResetPrice > 0) || IsFreeGold() )
	{
		MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
		MESSAGEBOX->SetTitle( "Event Girl" );
		MESSAGEBOX->SetDescription( FormatString( "Do you want to Reset these stats?\nPrice: %s gp", FormatNumber( iResetPrice ) ) );
		MESSAGEBOX->SetEvent( std::bind( &CEventGirlHandler::OnAcceptResetStats, this, std::placeholders::_1 ) );
		MESSAGEBOX->Show();
	}
	else
		TITLEBOX( "Before proceeding, select a stat to be reseted" );
}
void CEventGirlHandler::OnCloseButtonClick( UIEventArgs e )
{
	ResetCheckBox();
	Close();
}

void CEventGirlHandler::OnAcceptAllSkills(UIEventArgs e )
{
	if ( MESSAGEBOX->GetType() )
	{
		int iGold = EVENTGIRL_ALLSKILLS_PRICE;

		if ( IsFreeGold() )
			iGold = 0;

		if ( UNITDATA->sCharacterData.iGold >= iGold )
		{
			if ( SKILLMANAGERHANDLER->ResetSkills( TRUE ) )
			{
				UNITDATA->sCharacterData.iGold -= iGold;
				CHECK_CHARACTER_CHECKSUM;

				PLAYMINISOUND( 18 );

				TITLEBOX( "All skills resetted!" );

				Close();
				HIDERIGHTNPCBOX;
				MOUSEHANDLER->UnblockMouse();
				return;
			}
		}
		else
			TITLEBOX( "Not enough gold" );
	}
}

void CEventGirlHandler::OnAcceptAllStats( UIEventArgs e )
{
	if ( MESSAGEBOX->GetType() )
	{
		int iGold = EVENTGIRL_ALLSTATS_PRICE;

		if ( IsFreeGold() )
			iGold = 0;

		if ( UNITDATA->sCharacterData.iGold >= iGold )
		{
			CHARACTERGAME->ResetStats( 6 );

			UNITDATA->sCharacterData.iGold -= iGold;
			CHECK_CHARACTER_CHECKSUM;

			PLAYMINISOUND( 18 );

			TITLEBOX( "All stats resetted!" );

			Close();
			HIDERIGHTNPCBOX;
			MOUSEHANDLER->UnblockMouse();
			return;
		}
		else
			TITLEBOX( "Not enough gold" );
	}
}

void CEventGirlHandler::OnAcceptResetStats( UIEventArgs e )
{
	BOOL baReset[5] = { FALSE, FALSE, FALSE, FALSE, FALSE };

	if ( MESSAGEBOX->GetType() )
	{
		int iGold = 0;

		UI::CheckBox_ptr pCheckBox = pWindow->GetElement<UI::CheckBox>( CHECKBOXID_Strength );
		if ( pCheckBox->IsChecked() )
		{
			baReset[0] = TRUE;
			iGold += EVENTGIRL_RESETSTATS_PRICE;
		}

		pCheckBox = pWindow->GetElement<UI::CheckBox>( CHECKBOXID_Spirit );
		if ( pCheckBox->IsChecked() )
		{
			baReset[1] = TRUE;
			iGold += EVENTGIRL_RESETSTATS_PRICE;
		}

		pCheckBox = pWindow->GetElement<UI::CheckBox>( CHECKBOXID_Talent );
		if ( pCheckBox->IsChecked() )
		{
			baReset[2] = TRUE;
			iGold += EVENTGIRL_RESETSTATS_PRICE;
		}

		pCheckBox = pWindow->GetElement<UI::CheckBox>( CHECKBOXID_Agility );
		if ( pCheckBox->IsChecked() )
		{
			baReset[3] = TRUE;
			iGold += EVENTGIRL_RESETSTATS_PRICE;
		}

		pCheckBox = pWindow->GetElement<UI::CheckBox>( CHECKBOXID_Health );
		if ( pCheckBox->IsChecked() )
		{
			baReset[4] = TRUE;
			iGold += EVENTGIRL_RESETSTATS_PRICE;
		}

		if( IsFreeGold() )
			iGold = 0;

		if ( UNITDATA->sCharacterData.iGold >= iGold )
		{
			for ( int i = 0; i < 5; i++ )
			{
				if ( baReset[i] )
					CHARACTERGAME->ResetStats( i + 1 );
			}

			UNITDATA->sCharacterData.iGold -= iGold;
			CHECK_CHARACTER_CHECKSUM;

			PLAYMINISOUND( 18 );

			ResetCheckBox();

			TITLEBOX( "Stats resetted!" );

			Close();
			HIDERIGHTNPCBOX;
			MOUSEHANDLER->UnblockMouse();
			return;
		}
		else
			TITLEBOX( "Not enough gold" );
	}
}

void CEventGirlHandler::ResetCheckBox()
{
	UI::CheckBox_ptr pCheckBox = pWindow->GetElement<UI::CheckBox>( CHECKBOXID_Strength );
	pCheckBox->SetCheck( FALSE );

	pCheckBox = pWindow->GetElement<UI::CheckBox>( CHECKBOXID_Spirit );
	pCheckBox->SetCheck( FALSE );

	pCheckBox = pWindow->GetElement<UI::CheckBox>( CHECKBOXID_Talent );
	pCheckBox->SetCheck( FALSE );

	pCheckBox = pWindow->GetElement<UI::CheckBox>( CHECKBOXID_Agility );
	pCheckBox->SetCheck( FALSE );

	pCheckBox = pWindow->GetElement<UI::CheckBox>( CHECKBOXID_Health );
	pCheckBox->SetCheck( FALSE );
}

BOOL CEventGirlHandler::IsFreeGold()
{
	if ( UNITDATA->sCharacterData.iLevel <= 100 )
		return TRUE;

	if ( bFreeGold )
		return TRUE;

	return FALSE;
}

void CEventGirlHandler::OnMouseMove( CMouse * pcMouse )
{
	pWindow->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

BOOL CEventGirlHandler::OnMouseScroll( CMouse * pcMouse )
{
	return pWindow->OnMouseScroll( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

void CEventGirlHandler::OnResolutionChanged()
{
	int iWindowWidth = pWindow->GetWidth();
	int iWindowHeight = pWindow->GetHeight();
	int iWindowX = ( RESOLUTION_WIDTH >> 2 ) - ( iWindowWidth >> 1 ) + 30;
	int iWindowY = ( RESOLUTION_HEIGHT >> 2 ) - ( iWindowHeight >> 1 ) + 78;

	pWindow->SetPosition( iWindowX, iWindowY );
}

void CEventGirlHandler::Render()
{
	pWindow->Render( 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );

	if ( pWindow->IsOpen() )
	{
		int iGold = 0;

		UI::CheckBox_ptr pCheckBox = pWindow->GetElement<UI::CheckBox>( CHECKBOXID_Strength );
		if ( pCheckBox->IsChecked() )
			iGold += EVENTGIRL_RESETSTATS_PRICE;

		pCheckBox = pWindow->GetElement<UI::CheckBox>( CHECKBOXID_Spirit );
		if ( pCheckBox->IsChecked() )
			iGold += EVENTGIRL_RESETSTATS_PRICE;

		pCheckBox = pWindow->GetElement<UI::CheckBox>( CHECKBOXID_Talent );
		if ( pCheckBox->IsChecked() )
			iGold += EVENTGIRL_RESETSTATS_PRICE;

		pCheckBox = pWindow->GetElement<UI::CheckBox>( CHECKBOXID_Agility );
		if ( pCheckBox->IsChecked() )
			iGold += EVENTGIRL_RESETSTATS_PRICE;

		pCheckBox = pWindow->GetElement<UI::CheckBox>( CHECKBOXID_Health );
		if ( pCheckBox->IsChecked() )
			iGold += EVENTGIRL_RESETSTATS_PRICE;

		if ( IsFreeGold() )
			iGold = 0;

		UI::Text_ptr pText = pWindow->GetElement<UI::Text>( TEXTID_StatusValue );
		pText->SetTextEx( "%s gp", FormatNumber( iGold ) );

		iResetPrice = iGold;
	}
}

void CEventGirlHandler::Open( PacketEventGirl * psPacket )
{
	bFreeGold = psPacket->bFreeGold;
	pWindow->Show();
	MOUSEHANDLER->BlockMouse();
}


void CEventGirlHandler::Close()
{
	pWindow->Hide();
	MOUSEHANDLER->UnblockMouse();
}