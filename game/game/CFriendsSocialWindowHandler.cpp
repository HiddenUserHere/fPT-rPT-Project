#include "stdafx.h"
#include "CFriendsSocialWindowHandler.h"


CFriendsSocialWindowHandler::CFriendsSocialWindowHandler()
{
	pWindow = std::make_shared<UI::Window>( Rectangle2D( 34, 72, 184, 342 ) );
}

CFriendsSocialWindowHandler::~CFriendsSocialWindowHandler()
{
	pWindow = nullptr;
}

void CFriendsSocialWindowHandler::Init()
{
	UI::List_ptr pListRecents = std::make_shared<UI::List>( Rectangle2D( 0, 0, 202, 278 ) );
	pListRecents->SetID( LIST_Recent );
	pListRecents->MakeScrollBar( Rectangle2D( 196, 0, 4, 269 ), "game\\images\\hud\\social\\scroll.png", "game\\images\\hud\\social\\scroller.png" );

	pWindow->AddElement( pListRecents );

	UI::Group_ptr pGroupRecents = std::make_shared<UI::Group>();
	pGroupRecents->SetID( GROUP_Name );
	pWindow->AddElement( pGroupRecents );

	UI::List_ptr pListFriends = std::make_shared<UI::List>( Rectangle2D( 0, 0, 202, 278 ) );
	pListFriends->SetID( LIST_Friends );
	pListFriends->Hide();
	pWindow->AddElement( pListFriends );

	UI::List_ptr pListIgnored = std::make_shared<UI::List>( Rectangle2D( 0, 0, 202, 278 ) );
	pListIgnored->SetID( LIST_Ignored );
	pListIgnored->Hide();
	pWindow->AddElement( pListIgnored );

	//Bottom
	{
		UI::ImageBox_ptr pOptionsImage = std::make_shared<UI::ImageBox>( Rectangle2D( 28, 282, 147, 35 ) );
		pOptionsImage->SetImage( UI::ImageLoader::LoadImage( "game\\images\\hud\\social\\options_bg.png" ) );
		pWindow->AddElement( pOptionsImage );

		UI::Group_ptr pGroupTabBottom = std::make_shared<UI::Group>();
		pGroupTabBottom->SetID( GROUP_TabBottom );
		pWindow->AddElement( pGroupTabBottom );

		UI::Button_ptr pButtonAddFriend = std::make_shared<UI::Button>( Rectangle2D( 30, 282, 32, 32 ) );
		pButtonAddFriend->SetID( BUTTON_AddFriend );
		pButtonAddFriend->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\hud\\social\\addfriendbtn.png", "game\\images\\hud\\social\\addfriendbtn_.png" ) );
		pButtonAddFriend->SetEvent( UI::Event::Build( std::bind( &CFriendsSocialWindowHandler::OnButtonAddFriendClick, this, std::placeholders::_1 ) ) );
		pWindow->AddElement( pButtonAddFriend );

		UI::Tooltip_ptr pTooltipAddFriend = std::make_shared<UI::Tooltip>( Rectangle2D() );
		pTooltipAddFriend->Init();
		pTooltipAddFriend->SetLinkObject( pButtonAddFriend );
		pTooltipAddFriend->SetFont( "Arial", 16, -1 );
		pTooltipAddFriend->SetPosition( TOOLTIPPOSITION_Top );
		pTooltipAddFriend->SetFollowMouse( FALSE );
		pTooltipAddFriend->SetText( "Add a friend" );
		pWindow->AddElement( pTooltipAddFriend );

		UI::ImageBox_ptr pSeparatorVerticalImage1 = std::make_shared<UI::ImageBox>( Rectangle2D( 64, 284, 1, 28 ) );
		pSeparatorVerticalImage1->SetImage( UI::ImageLoader::LoadImage( "game\\images\\hud\\social\\tab_separator.png" ) );
		pWindow->AddElement( pSeparatorVerticalImage1 );

		UI::Button_ptr pButtonRecent = std::make_shared<UI::Button>( Rectangle2D( 67, 282, 32, 32 ) );
		pButtonRecent->SetID( BUTTON_Recent );
		pButtonRecent->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\hud\\social\\recentbtn.png", "game\\images\\hud\\social\\recentbtn_.png" ) );
		pButtonRecent->SetGroup( pGroupTabBottom );
		pButtonRecent->SetSelect( TRUE );
		pButtonRecent->SetSelected( TRUE );
		pButtonRecent->SetEvent( UI::Event::Build( std::bind( &CFriendsSocialWindowHandler::OnButtonRecentClick, this, std::placeholders::_1 ) ) );
		pWindow->AddElement( pButtonRecent );

		UI::Tooltip_ptr pTooltipRecent = std::make_shared<UI::Tooltip>( Rectangle2D() );
		pTooltipRecent->Init();
		pTooltipRecent->SetLinkObject( pButtonRecent );
		pTooltipRecent->SetFont( "Arial", 16, -1 );
		pTooltipRecent->SetPosition( TOOLTIPPOSITION_Top );
		pTooltipRecent->SetFollowMouse( FALSE );
		pTooltipRecent->SetText( "Recent list" );
		pWindow->AddElement( pTooltipRecent );

		UI::ImageBox_ptr pSeparatorVerticalImage2 = std::make_shared<UI::ImageBox>( Rectangle2D( 101, 284, 1, 28 ) );
		pSeparatorVerticalImage2->SetImage( UI::ImageLoader::LoadImage( "game\\images\\hud\\social\\tab_separator.png" ) );
		pWindow->AddElement( pSeparatorVerticalImage2 );

		UI::Button_ptr pButtonBlocked = std::make_shared<UI::Button>( Rectangle2D( 104, 282, 32, 32 ) );
		pButtonBlocked->SetID( BUTTON_Blocked );
		pButtonBlocked->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\hud\\social\\blockedbtn.png", "game\\images\\hud\\social\\blockedbtn_.png" ) );
		pButtonBlocked->SetGroup( pGroupTabBottom );
		pButtonBlocked->SetSelect( TRUE );
		pButtonBlocked->SetEvent( UI::Event::Build( std::bind( &CFriendsSocialWindowHandler::OnButtonBlockedClick, this, std::placeholders::_1 ) ) );
		pWindow->AddElement( pButtonBlocked );

		UI::Tooltip_ptr pTooltipBlocked = std::make_shared<UI::Tooltip>( Rectangle2D() );
		pTooltipBlocked->Init();
		pTooltipBlocked->SetLinkObject( pButtonBlocked );
		pTooltipBlocked->SetFont( "Arial", 16, -1 );
		pTooltipBlocked->SetPosition( TOOLTIPPOSITION_Top );
		pTooltipBlocked->SetFollowMouse( FALSE );
		pTooltipBlocked->SetText( "Blocked list" );
		pWindow->AddElement( pTooltipBlocked );

		UI::ImageBox_ptr pSeparatorVerticalImage3 = std::make_shared<UI::ImageBox>( Rectangle2D( 138, 284, 1, 28 ) );
		pSeparatorVerticalImage3->SetImage( UI::ImageLoader::LoadImage( "game\\images\\hud\\social\\tab_separator.png" ) );
		pWindow->AddElement( pSeparatorVerticalImage3 );

		UI::Button_ptr pButtonFavorite = std::make_shared<UI::Button>( Rectangle2D( 141, 282, 32, 32 ) );
		pButtonFavorite->SetID( BUTTON_Favorite );
		pButtonFavorite->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\hud\\social\\favoritebtn.png", "game\\images\\hud\\social\\favoritebtn_.png" ) );
		pButtonFavorite->SetGroup( pGroupTabBottom );
		pButtonFavorite->SetSelect( TRUE );
		pButtonFavorite->SetEvent( UI::Event::Build( std::bind( &CFriendsSocialWindowHandler::OnButtonFavoriteClick, this, std::placeholders::_1 ) ) );
		pWindow->AddElement( pButtonFavorite );

		UI::Tooltip_ptr pTooltipFavorite = std::make_shared<UI::Tooltip>( Rectangle2D() );
		pTooltipFavorite->Init();
		pTooltipFavorite->SetLinkObject( pButtonFavorite );
		pTooltipFavorite->SetFont( "Arial", 16, -1 );
		pTooltipFavorite->SetPosition( TOOLTIPPOSITION_Top );
		pTooltipFavorite->SetFollowMouse( FALSE );
		pTooltipFavorite->SetText( "Friend list" );
		pWindow->AddElement( pTooltipFavorite );

		pGroupTabBottom->SetIndexObject( BUTTON_Recent );
	}
}

void CFriendsSocialWindowHandler::Update( float fTime )
{
//	GetWindow()->Update( fTime );
}

void CFriendsSocialWindowHandler::Open( EWindowFriendsID iWindowID )
{
	pWindow->GetElement<UI::List>( LIST_Recent )->Hide();
	pWindow->GetElement<UI::List>( LIST_Friends )->Hide();
	pWindow->GetElement<UI::List>( LIST_Ignored )->Hide();

	pWindow->GetElement<UI::Button>( BUTTON_Favorite )->SetSelected( FALSE );
	pWindow->GetElement<UI::Button>( BUTTON_Blocked )->SetSelected( FALSE );

	pWindow->GetElement<UI::Button>( BUTTON_Recent )->SetSelected( FALSE );

	switch ( iWindowID )
	{
		case CFriendsSocialWindowHandler::WINDOWID_Recents:
			pWindow->GetElement<UI::List>( LIST_Recent )->Show();
			pWindow->GetElement<UI::Button>( BUTTON_Recent )->SetSelected( TRUE );
			break;
		case CFriendsSocialWindowHandler::WINDOWID_Friends:
			pWindow->GetElement<UI::List>( LIST_Friends )->Show();
			pWindow->GetElement<UI::Button>( BUTTON_Favorite )->SetSelected( TRUE );
			break;
		case CFriendsSocialWindowHandler::WINDOWID_Ignored:
			pWindow->GetElement<UI::List>( LIST_Ignored )->Show();
			pWindow->GetElement<UI::Button>( BUTTON_Blocked )->SetSelected( TRUE );
			break;
		default:
			break;
	}

	if ( pWindow->IsOpen() == FALSE )
		pWindow->Show();
}

void CFriendsSocialWindowHandler::Close()
{
	pWindow->Hide();
}

void CFriendsSocialWindowHandler::AddRecent( const std::string strName )
{
	Remove( strName, true, false, true );

	Add( LIST_Recent, strName );

	SOCIALHANDLER->SaveData();
}

void CFriendsSocialWindowHandler::AddFriend( const std::string strName )
{
	Remove( strName, false, true, true );

	Add( LIST_Friends, strName );

	SOCIALHANDLER->SaveData();
}

void CFriendsSocialWindowHandler::AddIgnored( const std::string strName )
{
	Remove( strName, true, true, true );

	Add( LIST_Ignored, strName );

	SOCIALHANDLER->SaveData();
}

bool CFriendsSocialWindowHandler::IsBlocked( const std::string strName )
{
	auto pList = pWindow->GetElement<UI::List>( LIST_Ignored );
	for ( auto pWindow1 : pList->GetWindowList() )
	{
		if ( tolower( pWindow1->GetElement<UI::Text>( TEXT_Name )->GetText() ).compare( tolower( strName ) ) == 0 )
			return true;
	}

	return false;
}

std::vector<std::string> CFriendsSocialWindowHandler::GetFriendNames()
{
	std::vector<std::string> v;
	auto pList = pWindow->GetElement<UI::List>( LIST_Friends );
	for ( auto pWindow1 : pList->GetWindowList() )
		v.push_back( pWindow1->GetElement<UI::Text>( TEXT_Name )->GetText() );

	return v;
}

std::vector<std::string> CFriendsSocialWindowHandler::GetBlockedNames()
{
	std::vector<std::string> v;
	auto pList = pWindow->GetElement<UI::List>( LIST_Ignored );
	for ( auto pWindow1 : pList->GetWindowList() )
		v.push_back( pWindow1->GetElement<UI::Text>( TEXT_Name )->GetText() );

	return v;
}

void CFriendsSocialWindowHandler::OnButtonBlockClick( UI::Text_ptr pText, UIEventArgs eArgs )
{
	std::string strName = pText->GetText();

	Remove( strName, true, true, true );

	AddIgnored( strName );

	SOCIALHANDLER->SaveData();
}

void CFriendsSocialWindowHandler::OnButtonRemoveClick( UI::Text_ptr pText, UIEventArgs eArgs )
{
	std::string strName = pText->GetText();

	Remove( strName, true, true, true );

	SOCIALHANDLER->SaveData();
}

void CFriendsSocialWindowHandler::OnButtonAddFriendClick( UIEventArgs eArgs )
{
	if ( strLastName.length() > 0 )
	{
		Remove( strLastName, false, true, true );

		AddFriend( strLastName );

		strLastName = "";

		Open( WINDOWID_Friends );
	}
}

void CFriendsSocialWindowHandler::OnButtonRecentClick( UIEventArgs eArgs )
{
	Open( WINDOWID_Recents );
}

void CFriendsSocialWindowHandler::OnButtonBlockedClick( UIEventArgs eArgs )
{
	Open( WINDOWID_Ignored );
}

void CFriendsSocialWindowHandler::OnButtonFavoriteClick( UIEventArgs eArgs )
{
	Open( WINDOWID_Friends );
}

void CFriendsSocialWindowHandler::OnNameClick( UI::Text_ptr pText, int iListID, UIEventArgs eArgs )
{
	if ( iListID != LIST_Friends )
		strLastName = pText->GetText();
}

void CFriendsSocialWindowHandler::OnNameButtonDoubleClick( UI::Text_ptr pText, int iListID, UIEventArgs eArgs )
{
	if ( iListID != LIST_Friends )
		strLastName = pText->GetText();

	pText->SetSelected( FALSE );

	CHATBOX->OnEnableChatBox();
	CHATBOX->SetInputFieldText( FormatString( "/:%s ", pText->GetText().c_str() ) );
}

void CFriendsSocialWindowHandler::Add( int iListID, const std::string strName )
{
	UI::Window_ptr pWindow1 = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 195, 24 ) );

	UI::ImageBox_ptr pImageSeparator = std::make_shared<UI::ImageBox>( Rectangle2D( 10, 18, 174, 1 ) );
	pImageSeparator->SetImage( UI::ImageLoader::LoadImage( "game\\images\\hud\\social\\separator.png" ) );
	pWindow1->AddElement( pImageSeparator );

	UI::Text_ptr pTextName = std::make_shared<UI::Text>( Rectangle2D( 10, 2, 138, 17 ) );
	pTextName->SetID( TEXT_Name );
	pTextName->SetFont( "Arial", 14, 0, FALSE, TRUE, -1 );
	pTextName->SetText( strName );
	pTextName->SetColor( D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
	pTextName->SetColorHover( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	pTextName->SetHorizontalAlign( ALIGN_Center );
	pTextName->SetSelectType( TRUE );
	pTextName->SetGroup( pWindow->GetElement<UI::Group>( GROUP_Name ) );
	pTextName->SetEvent( UI::Event::Build( std::bind( &CFriendsSocialWindowHandler::OnNameClick, this, pTextName, iListID, std::placeholders::_1 ) ) );
	pWindow1->AddElement( pTextName );

	UI::Button_ptr pButtonName = std::make_shared<UI::Button>( Rectangle2D( 10, 2, 138, 17 ) );
	pButtonName->SetMouseEvent( DoubleClickL );
	pButtonName->SetEvent( UI::Event::Build( std::bind( &CFriendsSocialWindowHandler::OnNameButtonDoubleClick, this, pTextName, iListID, std::placeholders::_1 ) ) );
	pWindow1->AddElement( pButtonName );

	if ( iListID != LIST_Ignored )
	{
		UI::Button_ptr pBlock = std::make_shared<UI::Button>( Rectangle2D( 150, 0, 16, 16 ) );
		pBlock->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\hud\\social\\blockbtn.png", "game\\images\\hud\\social\\blockbtn_.png" ) );
		pBlock->SetEvent( UI::Event::Build( std::bind( &CFriendsSocialWindowHandler::OnButtonBlockClick, this, pTextName, std::placeholders::_1 ) ) );
		pWindow1->AddElement( pBlock );
	}

	UI::Button_ptr pRemove = std::make_shared<UI::Button>( Rectangle2D( 168, -2, 20, 20 ) );
	pRemove->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\hud\\social\\removebtn.png", "game\\images\\hud\\social\\removebtn_.png" ) );
	pRemove->SetEvent( UI::Event::Build( std::bind( &CFriendsSocialWindowHandler::OnButtonRemoveClick, this, pTextName, std::placeholders::_1 ) ) );
	pWindow1->AddElement( pRemove );

	pWindow->GetElement<UI::List>( iListID )->AddWindow( pWindow1, iListID == LIST_Recent ? true : false );
}

void CFriendsSocialWindowHandler::Remove( const std::string strName, bool bRemoveRecents, bool bRemoveFriends, bool bRemoveIgnored )
{
	auto RemoveWindowFriends = [&strName]( UI::Window_ptr pWindow, int iListID )-> bool
	{
		bool bRet = false;

		auto pList = pWindow->GetElement<UI::List>( iListID );
		for ( auto pWindow1 : pList->GetWindowList() )
		{
			if ( tolower( pWindow1->GetElement<UI::Text>( TEXT_Name )->GetText() ).compare( tolower( strName ) ) == 0 )
			{
				pList->DelWindow( pWindow1 );
				bRet = true;
				break;
			}
		}

		return bRet;
	};

	if ( bRemoveFriends )
		RemoveWindowFriends( pWindow, LIST_Friends );
	
	if ( bRemoveIgnored )
		RemoveWindowFriends( pWindow, LIST_Ignored );

	if ( bRemoveRecents )
		RemoveWindowFriends( pWindow, LIST_Recent );

}

