#include "stdafx.h"
#include "CClanSocialWindowHandler.h"


CClanSocialWindowHandler::CClanSocialWindowHandler()
{
	pWindow = std::make_shared<UI::Window>( Rectangle2D( 34, 72, 184, 278 ) );
}


CClanSocialWindowHandler::~CClanSocialWindowHandler()
{
	pWindow = nullptr;
}

void CClanSocialWindowHandler::Init()
{
	UI::List_ptr pListMembers = std::make_shared<UI::List>( Rectangle2D( 0, 0, 202, 278 ) );
	pListMembers->SetID( LIST_Members );
	pListMembers->MakeScrollBar( Rectangle2D( 196, 0, 4, 269 ), "game\\images\\hud\\social\\scroll.png", "game\\images\\hud\\social\\scroller.png" );
	pWindow->AddElement( pListMembers );

	pWindow->Hide();
}

void CClanSocialWindowHandler::Update()
{
	if ( (uClanTimerWheel % 10) == 0 )
	{
		struct SChatListClan
		{
			int			iUnk;
			char		szMemberName[100][30];
		};

		pWindow->GetElement<UI::List>( LIST_Members )->Clear();

		const SChatListClan * psClanList = ((SChatListClan*)0x03B17C08);
		for ( int i = 0; i < _countof( psClanList->szMemberName ); i++ )
		{
			if ( psClanList->szMemberName[i][0] == 0 )
				break;

			if ( i == 0 )
				AddMember( psClanList->szMemberName[i], CLANMEMBERTYPE_Leader );
			else 
			{
				AddMember( psClanList->szMemberName[i], CLANMEMBERTYPE_Member );
			}
		}
	}

	uClanTimerWheel++;
}


void CClanSocialWindowHandler::AddMember( const std::string strName, EClanMemberType eMemberType )
{
	UI::Window_ptr pWindow1 = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 195, 24 ) );

	UI::ImageBox_ptr pImageSeparator = std::make_shared<UI::ImageBox>( Rectangle2D( 15, 18, 174, 1 ) );
	pImageSeparator->SetImage( UI::ImageLoader::LoadImage( "game\\images\\hud\\social\\separator.png" ) );
	pWindow1->AddElement( pImageSeparator );

	UI::Text_ptr pTextName = std::make_shared<UI::Text>( Rectangle2D( 10, 2, 172, 17 ) );
	pTextName->SetID( TEXT_MemberName );
	pTextName->SetFont( "Arial", 14, 0, FALSE, eMemberType != CLANMEMBERTYPE_Member ? TRUE : FALSE, -1 );
	pTextName->SetText( strName );
	
	if ( eMemberType == CLANMEMBERTYPE_Leader )
	{
		pTextName->SetColor( D3DCOLOR_ARGB( 210, 255, 215, 0 ) );
		pTextName->SetColorHover( D3DCOLOR_ARGB( 255, 255, 215, 0 ) );
	}
	else if ( eMemberType == CLANMEMBERTYPE_ViceLeader )
	{
		pTextName->SetColor( D3DCOLOR_ARGB( 140, 255, 215, 0 ) );
		pTextName->SetColorHover( D3DCOLOR_ARGB( 255, 255, 215, 0 ) );
	}
	else
	{
		pTextName->SetColor( D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
		pTextName->SetColorHover( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	}

	pTextName->SetHorizontalAlign( ALIGN_Center );
	pTextName->SetSelectType( TRUE );
	pTextName->SetGroup( pWindow->GetElement<UI::Group>( GROUP_Member ) );
	pTextName->SetMouseEvent( DoubleClickL );
	pTextName->SetEvent( UI::Event::Build( std::bind( &CClanSocialWindowHandler::OnButtonNameClick, this, pTextName, std::placeholders::_1 ) ) );
	pWindow1->AddElement( pTextName );

	pWindow->GetElement<UI::List>( LIST_Members )->AddWindow( pWindow1 );
}


void CClanSocialWindowHandler::RemoveMember( const std::string strName )
{
	auto pList = pWindow->GetElement<UI::List>( LIST_Members );
	for ( auto pWindow1 : pList->GetWindowList() )
	{
		if ( tolower( pWindow1->GetElement<UI::Text>( TEXT_MemberName )->GetText() ).compare( tolower( strName ) ) == 0 )
		{
			pList->DelWindow( pWindow1 );
			break;
		}
	}
}

void CClanSocialWindowHandler::UpdateMemberStatus( const std::string strName, bool bOnline )
{
}

bool CClanSocialWindowHandler::MemberExists( const std::string strName )
{
	auto pList = pWindow->GetElement<UI::List>( LIST_Members );
	for ( auto pWindow1 : pList->GetWindowList() )
	{
		if ( tolower( pWindow1->GetElement<UI::Text>( TEXT_MemberName )->GetText() ).compare( tolower( strName ) ) == 0 )
			return true;
	}

	return false;
}

void CClanSocialWindowHandler::OnButtonNameClick( UI::Text_ptr pText, UIEventArgs eArgs )
{
	pText->SetSelected( FALSE );

	CHATBOX->OnEnableChatBox();
	CHATBOX->SetInputFieldText( FormatString( "/:%s ", pText->GetText().c_str() ) );
}
