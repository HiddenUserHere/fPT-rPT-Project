#include "stdafx.h"
#include "CBattleRoyaleWindowHandler.h"

#define BATTLEROYALE_ROLLMASK			0x5AFDE0
#define BATTLEROYALE_ROLLITEMCOLUMN		2

CBattleRoyaleWindowHandler::CBattleRoyaleWindowHandler()
{
	pWindow1 = make_shared<UI::Window>( Rectangle2D( 10, 10, 652, 547 ) );
}


CBattleRoyaleWindowHandler::~CBattleRoyaleWindowHandler()
{
	pWindow1 = nullptr;
}

void CBattleRoyaleWindowHandler::Init()
{
	BuildWindow();

	pWindow1->SetPosition( (RESOLUTION_WIDTH >> 1) - (pWindow1->GetWidth() >> 1), (RESOLUTION_HEIGHT >> 1) - (pWindow1->GetHeight() >> 1) );
}

void CBattleRoyaleWindowHandler::Open( EWindowOpenID eWindowID, bool bClear )
{
	if ( fTimeSending > 0.0f )
	{
		TITLEBOX( "You need to wait your reward!" );
		return;
	}

	pWindow1->GetElement<UI::List>( LIST_Main )->Hide();
	pWindow1->GetElement<UI::List>( LIST_Reward )->Hide();
	pWindow1->GetElement<UI::List>( LIST_Rules )->Hide();

	pWindow1->GetElement<UI::Button>( BUTTON_Back )->Hide();
	pWindow1->GetElement<UI::Button>( BUTTON_Reward )->Hide();
	pWindow1->GetElement<UI::Button>( BUTTON_Rules )->Hide();

	pWindow1->GetElement<UI::List>( LIST_Reward )->GetWindow( WINDOW_Reward )->GetElement<UI::Text>( TEXT_NoRewards )->Hide();
	pWindow1->GetElement<UI::List>( LIST_Reward )->GetWindow( WINDOW_Reward )->GetElement<UI::Button>( BUTTON_Roll )->Show();
	pWindow1->GetElement<UI::List>( LIST_Reward )->GetWindow( WINDOW_Reward )->GetElement<UI::Button>( BUTTON_Claim )->Hide();
	pWindow1->GetElement<UI::List>( LIST_Reward )->GetWindow( WINDOW_Reward )->GetElement<UI::ImageBox>( IMAGE_GoldExp )->Hide();

	switch ( eWindowID )
	{
		case WINDOWOPENID_Main:
			pWindow1->GetElement<UI::List>( LIST_Main )->Show();
			pWindow1->GetElement<UI::Button>( BUTTON_Reward )->Show();
			pWindow1->GetElement<UI::Button>( BUTTON_Rules )->Show();
			break;
		case WINDOWOPENID_Reward:
			pWindow1->GetElement<UI::List>( LIST_Reward )->Show();
			pWindow1->GetElement<UI::Button>( BUTTON_Back )->Show();

			if ( GetRewardWindowList()->GetWindowList().size() == 0 )
			{
				pWindow1->GetElement<UI::List>( LIST_Reward )->GetWindow( WINDOW_Reward )->GetElement<UI::Text>( TEXT_NoRewards )->Show();
				pWindow1->GetElement<UI::List>( LIST_Reward )->GetWindow( WINDOW_Reward )->GetElement<UI::Button>( BUTTON_Roll )->Hide();
			}
			else if ( GetRewardWindowList()->GetWindowList().size() == 1 )
			{
				pWindow1->GetElement<UI::List>( LIST_Reward )->GetWindow( WINDOW_Reward )->GetElement<UI::Button>( BUTTON_Roll )->Hide();
				pWindow1->GetElement<UI::List>( LIST_Reward )->GetWindow( WINDOW_Reward )->GetElement<UI::Button>( BUTTON_Claim )->Show();
				pWindow1->GetElement<UI::List>( LIST_Reward )->GetWindow( WINDOW_Reward )->GetElement<UI::ImageBox>( IMAGE_GoldExp )->Show();
			}
			break;
		case WINDOWOPENID_Rules:
			pWindow1->GetElement<UI::List>( LIST_Rules )->Show();
			pWindow1->GetElement<UI::Button>( BUTTON_Back )->Show();
			break;
	}

	if ( pWindow1->IsOpen() == FALSE )
	{
		SAVE;

		pWindow1->Show();

		if ( bClear )
			Clear();

		MOUSEHANDLER->BlockMouse();
	}
}

void CBattleRoyaleWindowHandler::Close()
{
	if ( (fTimeSending > 0.0f) || bRoll )
	{
		TITLEBOX( "You need to wait your reward!" );
		return;
	}

	if ( pWindow1->IsOpen() )
	{
		Clear();
		pWindow1->Hide();

		MOUSEHANDLER->UnblockMouse();
	}
}

void CBattleRoyaleWindowHandler::Clear()
{
	std::vector<UI::Window_ptr> vWindowsDelete;

	auto pListMain = pWindow1->GetElement<UI::List>( LIST_Main );
	for ( auto pWindow : pListMain->GetWindowList() )
		if ( pWindow->GetID() >= WINDOW_BattleID )
			vWindowsDelete.push_back( pWindow );

	for ( auto pWindow : vWindowsDelete )
		pListMain->DelWindow( pWindow );

	vWindowsDelete.clear();

	auto pListRewardWindow = GetRewardWindowList();
	for ( auto pWindow : pListRewardWindow->GetWindowList() )
		vWindowsDelete.push_back( pWindow );

	for ( auto pWindow : vWindowsDelete )
		pListRewardWindow->DelWindow( pWindow );

	vWindowsDelete.clear();

	vRewardChances.clear();
	vBattleStatus.clear();
	iTotalChance = 0;
}

void CBattleRoyaleWindowHandler::AddBattle( int iBattleID, int iMapID, bool bOpen, IMinMax sRequiredLevel, INT64 iTimeLeft, const std::string strName )
{
	auto pListMain = pWindow1->GetElement<UI::List>( LIST_Main );

	UI::Window_ptr pWindowBattle = std::make_shared<UI::Window>( Rectangle2D( 20, 0, pListMain->GetWidth(), 20 ) );
	pWindowBattle->SetID( WINDOW_BattleID + pListMain->GetWindowList().size() );

	UI::ImageBox_ptr pImageStatus = std::make_shared<UI::ImageBox>( Rectangle2D( 0, 0, 32, 15 ) );
	pImageStatus->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\%s_broyale.png", bOpen ? "open" : "closed" ) );
	pWindowBattle->AddElement( pImageStatus );

	UI::Text_ptr pTextBattle = std::make_shared<UI::Text>( Rectangle2D( 36, 0, pListMain->GetWidth(), 20 ) );
	pTextBattle->SetFont( "Arial", 16, 0, FALSE, FALSE, D3DCOLOR_ARGB( 255, 200, 200, 200 ) );
	if ( iTimeLeft != INT64_MAX )
	{
		if ( bOpen == false )
		{
			iTimeLeft -= (MAX_BATTLEROYALE_WAITTIME / 1000);
			if ( iTimeLeft < 0 )
				iTimeLeft = 0;

			int iHour		= (int)((iTimeLeft / 60) / 60);
			int iMinutes	= (int)((iTimeLeft / 60) % 60);

			if ( iHour > 0 )
				pTextBattle->SetTextEx( "%s (%s) (%d-%d) will open in %02d hour%s and %02d minute%s!", strName.c_str(), MAPNAME( iMapID ), sRequiredLevel.iMin, sRequiredLevel.iMax, iHour, iHour > 1 ? "s" : "", iMinutes, iMinutes > 1 ? "s" : "" );
			else
				pTextBattle->SetTextEx( "%s (%s) (%d-%d) will open in %02d minute%s!", strName.c_str(), MAPNAME( iMapID ), sRequiredLevel.iMin, sRequiredLevel.iMax, iMinutes, iMinutes > 1 ? "s" : "" );
		}
		else
			pTextBattle->SetTextEx( "%s (%s) (%d-%d)", strName.c_str(), MAPNAME( iMapID ), sRequiredLevel.iMin, sRequiredLevel.iMax );
	}
	else
		pTextBattle->SetTextEx( "%s (%s) (%d-%d) will not open today, back tomorrow!", strName.c_str(), MAPNAME( iMapID ), sRequiredLevel.iMin, sRequiredLevel.iMax );

	pTextBattle->SetColorHover( D3DCOLOR_XRGB( 238, 201, 0 ) );
	pTextBattle->SetEvent( UI::Event::Build( std::bind( &CBattleRoyaleWindowHandler::OnButtonRequestClick, this, iBattleID, std::placeholders::_1 ) ) );
	pWindowBattle->AddElement( pTextBattle );

	pListMain->AddWindow( pWindowBattle );

	vBattleStatus.push_back( BattleStatus( iBattleID, bOpen ) );
}

void CBattleRoyaleWindowHandler::AddReward( int iID, int iChance, const std::string strName, const std::string strImagePath )
{
	auto pListRewardWindow = GetRewardWindowList();

	UI::Window_ptr pWindowReward = std::make_shared<UI::Window>( Rectangle2D( 0, 15, 88, 112 ) );
	pWindowReward->SetID( iID );
	pListRewardWindow->AddWindow( pWindowReward );

	UI::Image * pImage = nullptr;
	if ( strImagePath.length() > 0 )
		pImage = UI::ImageLoader::LoadImage( strImagePath.c_str() );
	else
		pImage = UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\goldexp_broyale.png" );

	UI::ImageBox_ptr pImageBox = std::make_shared<UI::ImageBox>( Rectangle2D( 44 - (pImage->GetWidth() >> 1), 44 - (pImage->GetHeight() >> 1), pImage->GetWidth(), pImage->GetHeight() ) );
	pImageBox->SetUseSelfScale( true );
	pImageBox->SetImage( pImage );
	pWindowReward->AddElement( pImageBox );

	UI::Text_ptr pTextNumber = std::make_shared<UI::Text>( Rectangle2D( 0, 92, 88, 20 ) );
	pTextNumber->SetFont( "Arial", 16, 0, TRUE, TRUE, -1 );
	pTextNumber->SetTextEx( "#%d", pListRewardWindow->GetWindowList().size() );
	pTextNumber->SetHorizontalAlign( EAlign::ALIGN_Center );
	pWindowReward->AddElement( pTextNumber );

	if ( (pListRewardWindow->GetWindowList().size() == 1) || (pListRewardWindow->GetWindowList().size() == 3) )
		ScaleImage( pWindowReward, 0.5f );

	if ( pListRewardWindow->GetWindowList().size() <= 3 )
		pWindowReward->Show();
	else
		pWindowReward->Hide();

	if ( strImagePath.length() == 0 )
		pWindowReward->Hide();

	vRewardChances.push_back( RewardChance( iID, iChance ) );
	iTotalChance += iChance;
}

void CBattleRoyaleWindowHandler::SetDataID( int iInstanceID, int iBattleID )
{
	iInstanceServerID = iInstanceID;
	iBattleServerID = iBattleID;
}

void CBattleRoyaleWindowHandler::Render()
{
	pWindow1->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

void CBattleRoyaleWindowHandler::Update( float fTime )
{
	static int iCountToReward = 0;

	if ( bRoll )
	{
		fUpdateRoll += fTime;

		if ( fUpdateRoll >= fDurationRoll )
		{
			if ( iCountToReward >= 15 )
				MoveRewardList( 22 );
			else if ( iCountToReward >= 10 )
				MoveRewardList( 11 );
			else
				MoveRewardList( 6 );

			if ( GetRewardWindowList()->GetAddAxisX() == 0 )
			{
				if ( GetCurrentRewardID() == GetRewardID() )
				{
					//Send to Server
					BATTLEROYALEHANDLER->ProcessPacket( &PacketBattleRoyaleRequest( iInstanceServerID, iBattleServerID, TRUE, GetRewardID() ) );

					fTimeSending = 5000.0f;

					bRoll = false;
				}

				iCountToReward--;
			}
		}
		else
		{
			MoveRewardList( 22 );

			iCountToReward = GetCountToReward();
		}
	}
	else
	{
		if ( fTimeSending > 0.0f )
		{
			fTimeSending -= fTime;

			if ( fTimeSending <= 0.0f )
				fTimeSending = 0.0f;
		}
	}
}

BOOL CBattleRoyaleWindowHandler::OnMouseClick( CMouse * pcMouse )
{
	return pWindow1->OnMouseClick( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

void CBattleRoyaleWindowHandler::OnMouseMove( CMouse * pcMouse )
{
	pWindow1->OnMouseMove( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

BOOL CBattleRoyaleWindowHandler::OnMouseScroll( CMouse * pcMouse )
{
	return pWindow1->OnMouseScroll( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

void CBattleRoyaleWindowHandler::UpdateReward()
{
	OnButtonRewardClick( UIEventArgs{} );

	fTimeSending = 0.0f;
}

void CBattleRoyaleWindowHandler::OnResolutionChanged()
{
	pWindow1->SetPosition( (RESOLUTION_WIDTH >> 1) - (pWindow1->GetWidth() >> 1), (RESOLUTION_HEIGHT >> 1) - (pWindow1->GetHeight() >> 1) );
}

void CBattleRoyaleWindowHandler::BuildWindow()
{
	pWindow1->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\window3.png" ) );

	//Header
	UI::ImageBox_ptr pHeaderImage = std::make_shared<UI::ImageBox>( Rectangle2D( 160, 0, 337, 51 ) );
	pHeaderImage->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\text\\header_broyale.png" ) );
	pWindow1->AddElement( pHeaderImage );

	//Main List
	UI::List_ptr pListMain = std::make_shared<UI::List>( Rectangle2D( 38, 56, 568, 423 ) );
	pListMain->SetID( LIST_Main );
	pListMain->MakeScrollBar( Rectangle2D( 570, 0, 4, 416 ), "game\\images\\hud\\social\\scroll.png", "game\\images\\hud\\social\\scroller.png" );
	pWindow1->AddElement( pListMain );

	//Text Main
	UI::Window_ptr pWindowMain = std::make_shared<UI::Window>( Rectangle2D( 10, 0, 548, 3000 ) );
	UI::TextWindow_ptr pTextWindowMain = std::make_shared<UI::TextWindow>( Rectangle2D( 0, 0, 546, 3000 ) );
	pTextWindowMain->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pTextWindowMain->LoadTextFromFile( "game\\npcs\\broyale\\main.txt" );
	pWindowMain->SetBox( Rectangle2D( 10, 0, 548, pTextWindowMain->GetHeightText() ) );
	pTextWindowMain->SetBox( Rectangle2D( 0, 0, 546, pTextWindowMain->GetHeightText() ) );
	pWindowMain->AddElement( pTextWindowMain );
	pListMain->AddWindow( pWindowMain );

	//Reward List
	UI::List_ptr pListReward = std::make_shared<UI::List>( Rectangle2D( 38, 56, 568, 423 ) );
	pListReward->SetID( LIST_Reward );
	pListReward->MakeScrollBar( Rectangle2D( 570, 0, 4, 416 ), "game\\images\\hud\\social\\scroll.png", "game\\images\\hud\\social\\scroller.png" );
	pListReward->Hide();
	pWindow1->AddElement( pListReward );

	//Reward Window List
	UI::Window_ptr pWindowReward = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 568, 423 ) );
	pWindowReward->SetID( WINDOW_Reward );
	pListReward->AddWindow( pWindowReward );

	UI::ImageBox_ptr pBorderImage = std::make_shared<UI::ImageBox>( Rectangle2D( (568 >> 1) - (304 >> 1), 80, 304, 136 ) );
	pBorderImage->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\itemborder_broyale.png" ) );
	pWindowReward->AddElement( pBorderImage );

	UI::Text_ptr pTextNoReward = std::make_shared<UI::Text>( Rectangle2D( (568 >> 1) - (568 >> 1), 110, 568, 76 ) );
	pTextNoReward->SetID( TEXT_NoRewards );
	pTextNoReward->SetFont( "Arial", 48, 0, TRUE, TRUE, -1 );
	pTextNoReward->SetText( "NO REWARDS!" );
	pTextNoReward->SetHorizontalAlign( ALIGN_Center );
	pTextNoReward->SetVerticalAlign( ALIGN_Center );
	pTextNoReward->Hide();
	pWindowReward->AddElement( pTextNoReward );

	UI::ImageBox_ptr pGoldEXPImage = std::make_shared<UI::ImageBox>( Rectangle2D( (568 >> 1) - (200 >> 1), 100, 200, 100 ) );
	pGoldEXPImage->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\goldexp_broyale.png" ) );
	pGoldEXPImage->SetID( IMAGE_GoldExp );
	pWindowReward->AddElement( pGoldEXPImage );

	UI::List_ptr pListRules = std::make_shared<UI::List>( Rectangle2D( 38, 56, 568, 423 ) );
	pListRules->SetID( LIST_Rules );
	pListRules->MakeScrollBar( Rectangle2D( 570, 0, 4, 416 ), "game\\images\\hud\\social\\scroll.png", "game\\images\\hud\\social\\scroller.png" );
	pListRules->Hide();
	pWindow1->AddElement( pListRules );

	UI::Window_ptr pWindowRules = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 568, 423 ) );
	pWindowRules->SetID( WINDOW_Rules );
	pListRules->AddWindow( pWindowRules );

	UI::TextWindow_ptr pTextWindowRules = std::make_shared<UI::TextWindow>( Rectangle2D( 0, 0, 546, 3000 ) );
	pTextWindowRules->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pTextWindowRules->LoadTextFromFile( "game\\npcs\\broyale\\rules.txt" );
	pWindowRules->SetBox( Rectangle2D( 10, 0, 548, pTextWindowRules->GetHeightText() ) );
	pTextWindowRules->SetBox( Rectangle2D( 0, 0, 546, pTextWindowRules->GetHeightText() ) );
	pWindowRules->AddElement( pTextWindowRules );

	//Button Roll
	UI::Button_ptr pButtonRoll = std::make_shared<UI::Button>( Rectangle2D( (568 >> 1) - (140 >> 1), 254, 140, 60 ) );
	pButtonRoll->SetID( BUTTON_Roll );
	pButtonRoll->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\roll_broyale.png", "game\\images\\UI\\window\\roll_broyale_.png" ) );
	pButtonRoll->SetEvent( UI::Event::Build( std::bind( &CBattleRoyaleWindowHandler::OnButtonRollClick, this, std::placeholders::_1 ) ) );
	pWindowReward->AddElement( pButtonRoll );

	//Button Claim
	UI::Button_ptr pButtonClaim = std::make_shared<UI::Button>( Rectangle2D( (568 >> 1) - (140 >> 1), 254, 140, 60 ) );
	pButtonClaim->SetID( BUTTON_Claim );
	pButtonClaim->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\claimbtn.png", "game\\images\\UI\\window\\claimbtn_.png" ) );
	pButtonClaim->SetEvent( UI::Event::Build( std::bind( &CBattleRoyaleWindowHandler::OnButtonClaimClick, this, std::placeholders::_1 ) ) );
	pWindowReward->AddElement( pButtonClaim );

	UI::List_ptr pListWindowReward = std::make_shared<UI::List>( Rectangle2D( (568 >> 1) - (304 >> 1), 80, 304, 200 ) );
	pListWindowReward->SetID( LIST_RewardWindow );
	pListWindowReward->SetHorizontal( TRUE );
	pListWindowReward->SetPaddingSeparator( 20 );
	pWindowReward->AddElement( pListWindowReward );


	//Button Close
	UI::Button_ptr pButtonClose = std::make_shared<UI::Button>( Rectangle2D( 589, 14, 30, 28 ) );
	pButtonClose->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\hud\\social\\closebtn.png", "game\\images\\hud\\social\\closebtn_.png" ) );
	pButtonClose->SetEvent( UI::Event::Build( std::bind( &CBattleRoyaleWindowHandler::OnButtonCloseClick, this, std::placeholders::_1 ) ) );
	pWindow1->AddElement( pButtonClose );

	//Button Back
	UI::Button_ptr pButtonBack = std::make_shared<UI::Button>( Rectangle2D( (pWindow1->GetWidth() >> 1) - (93 >> 1), pWindow1->GetHeight() - 65, 93, 40 ) );
	pButtonBack->SetID( BUTTON_Back );
	pButtonBack->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\btnback.png", "game\\images\\UI\\window\\btnback_.png" ) );
	pButtonBack->SetEvent( UI::Event::Build( std::bind( &CBattleRoyaleWindowHandler::OnButtonBackClick, this, std::placeholders::_1 ) ) );
	pWindow1->AddElement( pButtonBack );

	//Button Reward
	UI::Button_ptr pButtonReward = std::make_shared<UI::Button>( Rectangle2D( (pWindow1->GetWidth() >> 1) - 103, pWindow1->GetHeight() - 65, 93, 40 ) );
	pButtonReward->SetID( BUTTON_Reward );
	pButtonReward->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\rewardbtn_broyale.png", "game\\images\\UI\\window\\rewardbtn_broyale_.png" ) );
	pButtonReward->SetEvent( UI::Event::Build( std::bind( &CBattleRoyaleWindowHandler::OnButtonRewardClick, this, std::placeholders::_1 ) ) );
	pButtonReward->Hide();
	pWindow1->AddElement( pButtonReward );

	//Button Rules
	UI::Button_ptr pButtonRules = std::make_shared<UI::Button>( Rectangle2D( (pWindow1->GetWidth() >> 1) + 10, pWindow1->GetHeight() - 65, 93, 40 ) );
	pButtonRules->SetID( BUTTON_Rules );
	pButtonRules->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\rules_broyale.png", "game\\images\\UI\\window\\rules_broyale_.png" ) );
	pButtonRules->SetEvent( UI::Event::Build( std::bind( &CBattleRoyaleWindowHandler::OnButtonRulesClick, this, std::placeholders::_1 ) ) );
	pButtonRules->Hide();
	pWindow1->AddElement( pButtonRules );

	pWindow1->Hide();
}

void CBattleRoyaleWindowHandler::OnButtonCloseClick( UIEventArgs eArgs )
{
	Close();
}

void CBattleRoyaleWindowHandler::OnButtonRequestClick( int iBattleID, UIEventArgs eArgs )
{
	if ( MOUNTHANDLER->IsRiding( UNITDATA ) )
	{
		MOUNTHANDLER->ShowMessage( MOUNTMESSAGE_ErrorEnterNPC );
		return;
	}

	for ( auto & sBattle : vBattleStatus )
	{
		if ( sBattle.iBattleID == iBattleID )
		{
			if ( sBattle.bOpen )
			{
				MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
				MESSAGEBOX->SetTitle( "BATTLE ROYALE" );
				MESSAGEBOX->SetDescription( "Do you want to enter?" );
				MESSAGEBOX->SetEvent( std::bind( &CBattleRoyaleWindowHandler::OnAcceptRequest, this, iBattleID ) );
				MESSAGEBOX->SetAutoClose( 10 );
				MESSAGEBOX->Show();
			}
			else
				TITLEBOX( "This battle is closed for now!" );

			break;
		}
	}
}

void CBattleRoyaleWindowHandler::OnButtonRollClick( UIEventArgs eArgs )
{
	if ( (fTimeSending > 0.0f) || bRoll )
	{
		TITLEBOX( "You need to wait your reward!" );
		return;
	}

	auto pListRewardWindow = GetRewardWindowList();
	if ( pListRewardWindow->GetWindowList().size() > 0 )
	{
		MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
		MESSAGEBOX->SetTitle( "Roll" );
		MESSAGEBOX->SetDescription( "Do you want to roll the rewards?" );
		MESSAGEBOX->SetEvent( std::bind( &CBattleRoyaleWindowHandler::OnAcceptRoll, this ) );
		MESSAGEBOX->SetAutoClose( 10 );
		MESSAGEBOX->Show();
	}
}

void CBattleRoyaleWindowHandler::OnButtonClaimClick( UIEventArgs eArgs )
{
	if ( fTimeSending > 0.0f )
	{
		TITLEBOX( "You need to wait your reward!" );
		return;
	}

	//Send to Server
	BATTLEROYALEHANDLER->ProcessPacket( &PacketBattleRoyaleRequest( iInstanceServerID, iBattleServerID, TRUE, 0, TRUE ) );

	fTimeSending = 5000.0f;
}

void CBattleRoyaleWindowHandler::OnButtonBackClick( UIEventArgs eArgs )
{
	if ( (fTimeSending > 0.0f) || bRoll )
	{
		TITLEBOX( "You need to wait your reward!" );
		return;
	}

	BATTLEROYALEHANDLER->ProcessPacket( &PacketBattleRoyaleRequest( -1, -1, FALSE ) );
}

void CBattleRoyaleWindowHandler::OnButtonRewardClick( UIEventArgs eArgs )
{
	BATTLEROYALEHANDLER->ProcessPacket( &PacketBattleRoyaleRequest( -1, -1, TRUE ) );
}

void CBattleRoyaleWindowHandler::OnButtonRulesClick( UIEventArgs eArgs )
{
	Open( WINDOWOPENID_Rules, true );
}

void CBattleRoyaleWindowHandler::OnAcceptRequest( int iBattleID )
{
	if ( MESSAGEBOX->GetType() )
	{
		bool bRequest = true;

		if ( ITEMHANDLER->GetMouseItem()->bValid )
		{
			TITLEBOX( "Please, put your mouse item on inventory before request!" );
			bRequest = false;
		}

		if ( bRequest )
			BATTLEROYALEHANDLER->ProcessPacket( &PacketBattleRoyaleRequest( -1, iBattleID, FALSE ) );
	}
}

void CBattleRoyaleWindowHandler::OnAcceptRoll()
{
	auto pListRewardWindow = GetRewardWindowList();

	if ( MESSAGEBOX->GetType() )
	{
		if ( !bRoll )
		{
			fDurationRoll = RandomF( 2500.0f, 3000.0f );
			fUpdateRoll = 0.0f;
			fLastEase = 0.0f;
			iRollCount = RandomI( pListRewardWindow->GetWindowList().size(), (pListRewardWindow->GetWindowList().size() * 15) );
			bRoll = true;

			iRewardID = GetNewRewardID();
		}
	}
}

void CBattleRoyaleWindowHandler::MoveRewardList( int iSpeed )
{
	//Get Reward List
	auto pListRewardWindow = GetRewardWindowList();
	if ( pListRewardWindow->GetWindowList().size() > 3 )
	{
		//Add Axis
		pListRewardWindow->AddAxis( -iSpeed, 0 );

		//Next Item?
		if ( pListRewardWindow->GetAddAxisX() <= (-88) )
		{
			//Reset Axis
			pListRewardWindow->SetAxis( 0, 0 );

			//Get the reward
			auto pWindowFirstOld = pListRewardWindow->GetWindowList()[0];

			//Add Ref for no ptr delete
			pWindowFirstOld->AddRef();

			//Erase Window from the iterator list
			pListRewardWindow->DelWindow( pWindowFirstOld );

			//Dec Ref to back to the normal
			pWindowFirstOld->DecRef();

			//Add window to the end of the list
			pListRewardWindow->AddWindow( pWindowFirstOld );

			//Reset Scale
			ScaleImage( pWindowFirstOld, 1.0f );

			//Hide
			pWindowFirstOld->Hide();

			//Get the rewards
			auto pWindowFirst = pListRewardWindow->GetWindowList()[0];
			auto pWindowSecond = pListRewardWindow->GetWindowList()[1];
			auto pWindowThird = pListRewardWindow->GetWindowList()[2];

			//Scale
			ScaleImage( pWindowFirst, 0.5f );
			ScaleImage( pWindowSecond, 1.0f );
			ScaleImage( pWindowThird, 0.5f );

			//Show
			pWindowFirst->Show();
			pWindowSecond->Show();
			pWindowThird->Show();
		}
		else
		{
			//Get the rewards
			auto pWindowSecond = pListRewardWindow->GetWindowList()[1];
			auto pWindowThird = pListRewardWindow->GetWindowList()[2];
			auto pWindowFourth = pListRewardWindow->GetWindowList()[3];

			float fScaleSecond = (((88.0f + (float)pListRewardWindow->GetAddAxisX()) * 0.5f) / 88.0f) + 0.5f;
			float fScaleThird = ((abs( pListRewardWindow->GetAddAxisX() ) * 0.5f) / 88.0f) + 0.5f;

			//Scale
			ScaleImage( pWindowSecond, fScaleSecond );
			ScaleImage( pWindowThird, fScaleThird );
			ScaleImage( pWindowFourth, 0.5f );

			if ( pListRewardWindow->GetAddAxisX() < (-22) )
				pWindowFourth->Show();
		}
	}
}

void CBattleRoyaleWindowHandler::ScaleImage( UI::Window_ptr pWindowReward, float fScale )
{
	auto pImageBox = pWindowReward->GetElements()[0];

	pImageBox->SetScale( fScale );
	pImageBox->SetPosition( (88 >> 1) - ((int)((float)pImageBox->GetWidth() * fScale) >> 1), (88 >> 1) - ((int)((float)pImageBox->GetHeight() * fScale) >> 1) );
}

int CBattleRoyaleWindowHandler::GetNewRewardID()
{
	int iRet = 0;

	int iRandom = RandomI( 0, iTotalChance );
	int iRandomCount = 0;
	for ( auto & sChance : vRewardChances )
	{
		if ( (iRandom >= iRandomCount) && (iRandom < (iRandomCount + sChance.iChance)) )
		{
			iRet = sChance.iID;
			break;
		}

		iRandomCount += sChance.iChance;
	}

	return iRet;
}

int CBattleRoyaleWindowHandler::GetCurrentRewardID()
{
	//Get Reward List
	auto pListRewardWindow = GetRewardWindowList();
	if ( pListRewardWindow->GetWindowList().size() > 3 )
		return pListRewardWindow->GetWindowList()[1]->GetID();

	return 0;
}

int CBattleRoyaleWindowHandler::GetCountToReward()
{
	int iRet = 0;

	auto pListRewardWindow = GetRewardWindowList();
	if ( pListRewardWindow->GetWindowList().size() > 3 )
	{
		int iCountID = 0;
		for ( auto pWindow : pListRewardWindow->GetWindowList() )
		{
			if ( pWindow->GetID() == GetRewardID() )
				break;

			iCountID++;
		}

		if ( 1 <= iCountID )
			iRet = iCountID - 1;
		else
			iRet = (pListRewardWindow->GetWindowList().size() - 1) + iCountID;
	}

	return iRet;
}
