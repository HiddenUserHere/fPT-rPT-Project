#include "stdafx.h"
#include "CSocialWindowHandler.h"


CSocialWindowHandler::CSocialWindowHandler()
{
	pWindow = std::make_shared<UI::Window>( Rectangle2D( 0, 40, 270, 415 ) );
	
	OnResolutionChanged();

	pcClanWindow = new CClanSocialWindowHandler();
	pcFriendsWindow = new CFriendsSocialWindowHandler();
}


CSocialWindowHandler::~CSocialWindowHandler()
{
	DELET( pcFriendsWindow );
	DELET( pcClanWindow );

	pWindow = nullptr;
}

void CSocialWindowHandler::Init()
{
	BuildWindow();

	INIT( pcClanWindow );
	INIT( pcFriendsWindow );

	pWindow->AddElement( pcFriendsWindow->GetWindow() );
	pWindow->AddElement( pcClanWindow->GetWindow() );

	pWindow->Hide();
}

void CSocialWindowHandler::Update( float fTime )
{
	pWindow->Update( fTime );

	if ( bLoaded == false )
	{
		if ( Game::GetGameMode() == GAMEMODE_InGame )
		{
			LoadData();

			bLoaded = true;
		}
	}
}

void CSocialWindowHandler::Render()
{
	pWindow->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

void CSocialWindowHandler::OnMouseMove( CMouse * pcMouse )
{
	pWindow->OnMouseMove( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

BOOL CSocialWindowHandler::OnMouseClick( CMouse * pcMouse )
{
	return pWindow->OnMouseClick( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

BOOL CSocialWindowHandler::OnMouseScroll( CMouse * pcMouse )
{
	return pWindow->OnMouseScroll( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

void CSocialWindowHandler::OnResolutionChanged()
{
	int iWindowX = RESOLUTION_WIDTH - pWindow->GetWidth() - 32;
	int iWindowY = (RESOLUTION_HEIGHT >> 1) - (pWindow->GetHeight() >> 1);
	pWindow->SetPosition( iWindowX, iWindowY );
}

void CSocialWindowHandler::Open()
{
	OnButtonFriendsClick( UIEventArgs{} );
	pWindow->Show();
}

void CSocialWindowHandler::Close()
{
	pWindow->Hide();
}

void CSocialWindowHandler::LoadData()
{
	std::string strFile = FormatString( "save\\data\\RegnumWorld\\%s.ini", (char*)0x39033E8 );
	if ( FileExist( strFile ) )
	{
		INI::CReader cReader( (char*)strFile.c_str() );

		for ( int i = 0; i <= 100; i++ )
		{
			std::string strFriend = cReader.ReadString( "FRIEND", FormatString( "ID%02d", i ) );
			std::string strBlocked = cReader.ReadString( "DENY", FormatString( "ID%02d", i ) );

			if ( strFriend.length() > 0 )
				GetFriends()->AddFriend( strFriend );

			if ( strBlocked.length() > 0 )
				GetFriends()->AddIgnored( strBlocked );
		}
	}
	else
		SaveData();
}

void CSocialWindowHandler::SaveData()
{
	std::string strFile = FormatString( "save\\data\\RegnumWorld\\%s.ini", (char*)0x39033E8 );
	INI::CReader cReader( (char*)strFile.c_str() );

	auto vFriends = GetFriends()->GetFriendNames();
	auto vBlockeds = GetFriends()->GetBlockedNames();

	if ( vFriends.size() > 0 )
	{
		for ( UINT i = 0; i < vFriends.size(); i++ )
			cReader.WriteString( "FRIEND", FormatString( "ID%02d", i ), vFriends[i] );
	}
	for ( UINT i = vFriends.size(); i <= 100; i++ )
		cReader.WriteString( "FRIEND", FormatString( "ID%02d", i ), "" );

	vFriends.clear();

	if ( vBlockeds.size() > 0 )
	{
		for ( UINT i = 0; i < vBlockeds.size(); i++ )
			cReader.WriteString( "FRIEND", FormatString( "ID%02d", i ), vBlockeds[i] );
	}
	for ( UINT i = vBlockeds.size(); i <= 100; i++ )
		cReader.WriteString( "DENY", FormatString( "ID%02d", i ), "" );

	vBlockeds.clear();
}

void CSocialWindowHandler::UpdateObjectSettings()
{
	pWindow->SetCanMove( !SETTINGSHANDLER->GetModel()->GetSettings().bLockUI );
}

void CSocialWindowHandler::BuildWindow()
{
	pWindow->SetImage( UI::ImageLoader::LoadImage( "game\\images\\hud\\social\\window.png" ) );

	UI::Group_ptr pGroupTabTop = std::make_shared<UI::Group>();
	pWindow->AddElement( pGroupTabTop );

	UI::Button_ptr pButtonFriends = std::make_shared<UI::Button>( Rectangle2D( 35, 30, 103, 29 ) );
	pButtonFriends->SetID( BUTTON_FriendsTab );
	pButtonFriends->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\hud\\social\\friendsbtn.png", "game\\images\\hud\\social\\friendsbtn_.png" ) );
	pButtonFriends->SetGroup( pGroupTabTop );
	pButtonFriends->SetSelect( TRUE );
	pButtonFriends->SetEvent( UI::Event::Build( std::bind( &CSocialWindowHandler::OnButtonFriendsClick, this, std::placeholders::_1 ) ) );
	pWindow->AddElement( pButtonFriends );

	UI::Button_ptr pButtonClan = std::make_shared<UI::Button>( Rectangle2D( 138, 30, 96, 29 ) );
	pButtonClan->SetID( BUTTON_ClanTab );
	pButtonClan->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\hud\\social\\clanbtn.png", "game\\images\\hud\\social\\clanbtn_.png" ) );
	pButtonClan->SetGroup( pGroupTabTop );
	pButtonClan->SetSelect( TRUE );
	pButtonClan->SetEvent( UI::Event::Build( std::bind( &CSocialWindowHandler::OnButtonClanClick, this, std::placeholders::_1 ) ) );
	pWindow->AddElement( pButtonClan );

	pGroupTabTop->SetIndexObject( BUTTON_FriendsTab );

	UI::ImageBox_ptr pTitleImage = std::make_shared<UI::ImageBox>( Rectangle2D( 26, -24, 216, 66 ) );
	pTitleImage->SetImage( UI::ImageLoader::LoadImage( "game\\images\\hud\\social\\title.png" ) );
	pWindow->AddElement( pTitleImage );

	UI::Button_ptr pButtonClose = std::make_shared<UI::Button>( Rectangle2D( 212, 2, 30, 28 ) );
	pButtonClose->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\hud\\social\\closebtn.png", "game\\images\\hud\\social\\closebtn_.png" ) );
	pButtonClose->SetEvent( UI::Event::Build( std::bind( &CSocialWindowHandler::OnButtonCloseClick, this, std::placeholders::_1 ) ) );
	pWindow->AddElement( pButtonClose );
	
	pWindow->SetMoveBox( Rectangle2D( 0, 0, 211, 50 ) );
	pWindow->SetCanMove( !SETTINGSHANDLER->GetModel()->GetSettings().bLockUI );
}

void CSocialWindowHandler::BuildFriendsWindow()
{
}

void CSocialWindowHandler::OnButtonFriendsClick( UIEventArgs eArgs )
{
	pcFriendsWindow->Open( CFriendsSocialWindowHandler::EWindowFriendsID::WINDOWID_Recents );
	pcClanWindow->Close();
}

void CSocialWindowHandler::OnButtonClanClick( UIEventArgs eArgs )
{
	pcClanWindow->Open();
	pcFriendsWindow->Close();
}

void CSocialWindowHandler::OnButtonCloseClick( UIEventArgs eArgs )
{
	Close();
}