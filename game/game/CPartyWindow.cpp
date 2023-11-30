#include "stdafx.h"
#include "CPartyWindow.h"

CPartyWindow::CPartyWindow()
{
	pWindowParty = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 290, 345 ) );
	pWindowRaid = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 115, 185 ) );
}

CPartyWindow::~CPartyWindow()
{
}

void CPartyWindow::Init()
{
	pImagePartyMode[0] = UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\party\\normalmode.png" );
	pImagePartyMode[1] = UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\party\\expmode.png" );
	pImagePartyMode[2] = UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\party\\huntmode.png" );

	UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\party\\minilifebar.png" );
	UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\party\\minilifebar_.png" );
	UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\party\\background.png" );

	for ( int i = 0; i < 6; i++ )
		UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\party\\member%d.png", i + 1 );

	for ( int i = 0; i < 10; i++ )
	{
		const char * pszClass = GetCharacterClassByFlag( CharacterClassToClassFlag( (ECharacterClass)(i + 1) ) );
		UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\party\\%s.png", pszClass );
		UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\party\\%s_.png", pszClass );
	}

	UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\party\\lifebar.png" );
	UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\party\\manabar.png" );


	UI::ImageLoader::LoadImage( "game\\images\\UI\\menu\\topleft.png" );
	UI::ImageLoader::LoadImage( "game\\images\\UI\\menu\\middle.png" );
	UI::ImageLoader::LoadImage( "game\\images\\UI\\menu\\bottom.png" );
	UI::ImageLoader::LoadImage( "game\\images\\UI\\menu\\main.png" );

	UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\party\\raidleader.png" );
	UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\party\\leader.png" );


	BuildWindowParty();
	BuildWindowRaid();
}

void CPartyWindow::Render()
{
	pWindowRaid->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
	pWindowParty->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

void CPartyWindow::Update( float fTime )
{
	UpdatePartyData( &PARTYHANDLER->GetPartyData() );

	pWindowParty->Update( fTime );
	pWindowRaid->Update( fTime );
}

void CPartyWindow::OnMouseMove( CMouse * pcMouse )
{
	pWindowParty->OnMouseMove( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
	pWindowRaid->OnMouseMove( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

BOOL CPartyWindow::OnMouseClick( CMouse * pcMouse )
{
	if( CALL(0x004933B0) == false )
		return FALSE;

	if( (bool)CALLT(0x0049CD90, 0x35E05D8) == true )
		return FALSE;

	if( pWindowParty->OnMouseClick( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 ) )
		return TRUE;

	if( pWindowRaid->OnMouseClick( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 ) )
		return TRUE;

	return FALSE;
}

void CPartyWindow::ShowPartyNotification( char * pszCharacterName, int iType )
{
	if ( PERSONALSHOPHANDLER->GetWindow()->IsOpen() )
		return;

	iPartyRequestType = iType;

	MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
	MESSAGEBOX->SetTitle( "Party Request" );
	MESSAGEBOX->SetDescription( FormatString("Do you want to accept %s's Party Request?", pszCharacterName) );
	MESSAGEBOX->SetEvent( std::bind( &CPartyWindow::OnAcceptPartyButtonClick, this ) );
	MESSAGEBOX->SetAutoClose( 10 );
	MESSAGEBOX->Show();
}

void CPartyWindow::ShowRaidNotification( char * pszCharacterName )
{
	if ( PERSONALSHOPHANDLER->GetWindow()->IsOpen() )
		return;

	MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
	MESSAGEBOX->SetTitle( "Raid Request" );
	MESSAGEBOX->SetDescription( FormatString( "Do you want to accept %s's Raid Request?", pszCharacterName ) );
	MESSAGEBOX->SetEvent( std::bind( &CPartyWindow::OnAcceptRaidButtonClick, this ) );
	MESSAGEBOX->SetAutoClose( 10 );
	MESSAGEBOX->Show();
}

void CPartyWindow::UpdateParty( CPartyData * pcParty )
{
	bool bPartyIsRaidLeader = false;
	pWindowParty->Hide();

	auto pImageParty = pWindowParty->GetElement<UI::ImageBox>( IMAGEID_PartyType );
	auto pTooltipParty = pWindowParty->GetElement<UI::Tooltip>( TOOLTIPID_PartyType );
	if( pImageParty && pTooltipParty )
	{
		pImageParty->SetImage( pImagePartyMode[pcParty->eMode] );
		pTooltipParty->SetText( GetPartyMode(pcParty->eMode) );
	}

	for( size_t i = 0; i < MAX_RAID-1; i++ )
	{
		auto pListPartyRaid = pWindowRaid->GetElement<UI::List>( LISTID_PartyRaid + i );
		if( pListPartyRaid )
		{
			pListPartyRaid->Clear();
			pListPartyRaid->Hide();
			pWindowRaid->Hide();
			
			if( pcParty->iRaidCount > 0 )
			{
				bPartyIsRaidLeader = true;

				PartyRaid * pPartyRaid = &pcParty->saRaid[i];
				UI::Window_ptr p = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 115, 16 ) );

				UI::Text_ptr pText = std::make_shared<UI::Text>( FormatString( "Party #%d", (int)pPartyRaid->cRaidID ), Rectangle2D( 0, 0, 110, 20 ), TRUE, UI::Color(255,0,180,255).Get() );
				pText->SetFont( "Noto Sans", 16, 0, FALSE, FALSE, UI::Color(255,0,180,255).Get() );
				pText->SetDropdownShadow( TRUE );
				pText->SetNoClip( TRUE );
				p->AddElement( pText );

				pListPartyRaid->AddWindow( p );

				if( pPartyRaid->cRaidID == 1 )
					bPartyIsRaidLeader = false;

				for( size_t j = 0; j < MAX_PARTY_MEMBERS; j++ )
				{
					PartyRaidMember * pMemberRaid = &pPartyRaid->saMembers[j];

					if( pMemberRaid->sData.iID )
					{
						UI::Window_ptr pWindowRaidMember = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 115, 20 ) );
						pWindowRaidMember->SetID( WINDOWID_Member1 + j );

						//Character Name
						UI::Text_ptr pTextCharacterName = std::make_shared<UI::Text>( pPartyRaid->saMembers[j].szCharacterName, Rectangle2D( 0, 0, 110, 15 ), TRUE, -1 );
						pTextCharacterName->SetID( TEXTID_Name );
						pTextCharacterName->SetFont( "Noto Sans", 14, 0, FALSE, FALSE, -1 );
						pTextCharacterName->SetDropdownShadow( TRUE );
						pTextCharacterName->SetNoClip( TRUE );
						pWindowRaidMember->AddElement( pTextCharacterName );

						//Image of Life Bar
						UI::ImageBox_ptr pImageLifeBar = std::make_shared<UI::ImageBox>( Rectangle2D( 0, 14, 89, 4 ) );
						pImageLifeBar->SetID( IMAGEID_Background );
						pImageLifeBar->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\party\\minilifebar.png" ) );
						pWindowRaidMember->AddElement( pImageLifeBar );

						UI::ImageBox_ptr pImageLifeBarActive = std::make_shared<UI::ImageBox>( Rectangle2D( 0, 14, 89, 4 ) );
						pImageLifeBarActive->SetID( IMAGEID_LifeBar );
						pImageLifeBarActive->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\party\\minilifebar_.png" ) );
						pWindowRaidMember->AddElement( pImageLifeBarActive );

						pListPartyRaid->AddWindow( pWindowRaidMember );
					}
				}

				if( !SETTINGSHANDLER->GetModel()->GetSettings().bLockUI )
				{
					pWindowRaid->SetMoveBox( Rectangle2D( 0, 0, 175, 15 + pListPartyRaid->GetListHeight() ) );
					pWindowRaid->SetCanMove( TRUE );
				}
				else
					pWindowRaid->SetCanMove( FALSE );

				pListPartyRaid->Show();
				pWindowRaid->Show();
			}
		}
	}

	auto pListPartyMembers = pWindowParty->GetElement<UI::List>( LISTID_PartyMembers );
	if( pListPartyMembers )
	{
		pListPartyMembers->Clear();

		UI::Group_ptr pGroupDropdown = std::make_shared<UI::Group>();
		pWindowParty->AddElement( pGroupDropdown );

		//Clear Dropdown
		for ( int i = 0; i < MAX_PARTY_MEMBERS; i++ )
		{
			auto p = pWindowParty->GetElement<UI::DropdownMenu>( DROPDOWNMENUID_Options + (i * 6) );
			if ( p )
				pWindowParty->DelElement( p );
		}

		//Add Party Members
		for( size_t i = 0; i < MAX_PARTY_MEMBERS; i++ )
		{
			PartyMember sPartyMember = pcParty->saMembers[i];

			if( sPartyMember.sData.iID )
			{
				UI::Window_ptr p = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 290, 49 ) );
				p->SetID( WINDOWID_Member1 + i );

				UI::ImageBox_ptr pImageBackground = std::make_shared<UI::ImageBox>( Rectangle2D( 0, 0, 173, 49 ) );
				pImageBackground->SetID( IMAGEID_Background );
				pImageBackground->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\party\\background.png" ) );
				p->AddElement( pImageBackground );

				//Icon of Member Party
				UI::ImageBox_ptr pImageIconPartyMember = std::make_shared<UI::ImageBox>( Rectangle2D( 34, 40, 9, 9 ) );
				pImageIconPartyMember->SetID( IMAGEID_Icon );
				pImageIconPartyMember->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\party\\member%d.png", i + 1 ));
				p->AddElement( pImageIconPartyMember );

				//Text Character Name
				UI::Text_ptr pTextCharacterName = std::make_shared<UI::Text>( sPartyMember.szCharacterName, Rectangle2D( 70, 7, 220, 20 ), TRUE, -1 );
				pTextCharacterName->SetID( TEXTID_Name );
				pTextCharacterName->SetFont( "Noto Sans", 15, 0, FALSE, FALSE, -1 );
				pTextCharacterName->SetDropdownShadow( TRUE );
				pTextCharacterName->SetNoClip( TRUE );
				p->AddElement( pTextCharacterName );

				//Text Character Level
				UI::Text_ptr pTextCharacterLevel = std::make_shared<UI::Text>( "", Rectangle2D( 43, 8, 20, 12 ), TRUE, D3DCOLOR_XRGB( 255, 238, 100 ) );
				pTextCharacterLevel->SetID( TEXTID_CharacterLevel );
				pTextCharacterLevel->SetFont( "Arial", 13, 0, FALSE, FALSE, D3DCOLOR_XRGB( 255, 238, 100 ) );
				pTextCharacterLevel->SetDropdownShadow( TRUE );
				pTextCharacterLevel->SetDropdownColor( D3DCOLOR_XRGB( 30, 30, 30 ) );
				pTextCharacterLevel->SetNoClip( TRUE );
				pTextCharacterLevel->SetHorizontalAlign( ALIGN_Center );
				p->AddElement( pTextCharacterLevel );

				//Image of Character Class
				const char * pszClass = GetCharacterClassByFlag( CharacterClassToClassFlag( (ECharacterClass)sPartyMember.cCharacterClass ) );

				UI::ImageBox_ptr pImageCharacterClass = std::make_shared<UI::ImageBox>( Rectangle2D( 6, 8, 34, 34 ) );
				pImageCharacterClass->SetID( IMAGEID_Class );
				pImageCharacterClass->SetActiveImage( UI::ImageLoader::LoadActiveImage( (char*)(boost::format( "game\\images\\UI\\hud\\party\\%s.png" ) % pszClass ).str().c_str(), (char*)(boost::format( "game\\images\\UI\\hud\\party\\%s_.png" ) % pszClass ).str().c_str() ) );
				p->AddElement( pImageCharacterClass );

				//Image of Life Bar
				UI::ImageBox_ptr pImageLifeBar = std::make_shared<UI::ImageBox>( Rectangle2D( 46, 25, 119, 6 ) );
				pImageLifeBar->SetID( IMAGEID_LifeBar );
				pImageLifeBar->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\party\\lifebar.png" ) );
				p->AddElement( pImageLifeBar );

				//Image of Mana Bar
				UI::ImageBox_ptr pImageManaBar = std::make_shared<UI::ImageBox>( Rectangle2D( 42, 37, 123, 3 ) );
				pImageManaBar->SetID( IMAGEID_ManaBar );
				pImageManaBar->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\party\\manabar.png" ) );
				p->AddElement( pImageManaBar );

				//Dropdown Menu
				UI::DropdownMenu_ptr pDropdownMenu = std::make_shared<UI::DropdownMenu>( Rectangle2D( 175, 60 + pListPartyMembers->GetListHeight(), 94, 24 ) );
				pDropdownMenu->SetID( DROPDOWNMENUID_Options + (i * 6) );
				pDropdownMenu->SetGroup( pGroupDropdown );
				pDropdownMenu->SetDropdownPosition( UI::DROPDOWNPOSITION_Left );
				pDropdownMenu->SetMouseEvent( ClickDownR );
				pDropdownMenu->SetEvent( UI::Event::Build( std::bind( &CPartyWindow::OnSelectMenuParty, this, i ) ) );
				pDropdownMenu->SetMenuBackground( "game\\images\\UI\\menu\\topleft.png", "game\\images\\UI\\menu\\middle.png", "game\\images\\UI\\menu\\bottom.png", "game\\images\\UI\\menu\\main.png" );

				//Indicate it's a Party Leader
				if( i == 0 )
				{
					if( bPartyIsRaidLeader )
					{
						UI::ImageBox_ptr pImageIconRaidLeader = std::make_shared<UI::ImageBox>( Rectangle2D( 5, -18, 37, 30 ) );
						pImageIconRaidLeader->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\party\\raidleader.png" ) );
						p->AddElement( pImageIconRaidLeader );
					}

					UI::ImageBox_ptr pImageIconLeader = std::make_shared<UI::ImageBox>( Rectangle2D( 13, -10, 21, 14 ) );
					pImageIconLeader->SetID( IMAGEID_IconLeader );
					pImageIconLeader->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\party\\leader.png" ) );
					p->AddElement( pImageIconLeader );
				}

				//Party Leader
				if( pcParty->saMembers[0].sData.iID == UNITDATA->iID )
				{
					if( i != 0 )
					{
						//Options
						pDropdownMenu->AddOption( pDropdownMenu->MakeOption( "Delegate", Rectangle2D( 13, 5, 77, 18 ), FALSE, -1, D3DCOLOR_XRGB( 230, 180, 150 ) ), FALSE, 1 );
						pDropdownMenu->AddOption( pDropdownMenu->MakeOption( "Kick", Rectangle2D( 13, 5, 77, 18 ), FALSE, -1, D3DCOLOR_XRGB( 230, 180, 150 ) ), FALSE, 2 );
					}
					else
					{
						//Options
						pDropdownMenu->AddOption( pDropdownMenu->MakeOption( "Disband", Rectangle2D( 13, 5, 77, 18 ), FALSE, -1, D3DCOLOR_XRGB( 230, 180, 150 ) ), FALSE, 3 );
						pDropdownMenu->AddOption( pDropdownMenu->MakeOption( "Leave", Rectangle2D( 13, 5, 77, 18 ), FALSE, -1, D3DCOLOR_XRGB( 230, 180, 150 ) ), FALSE, 4 );
						pDropdownMenu->AddOption( pDropdownMenu->MakeOption( "Change Mode", Rectangle2D( 13, 5, 77, 18 ), FALSE, -1, D3DCOLOR_XRGB( 230, 180, 150 ) ), FALSE, 5 );
					}
				}
				else
				{
					if( sPartyMember.sData.iID == UNITDATA->iID )
					{
						//Options
						pDropdownMenu->AddOption( pDropdownMenu->MakeOption( "Leave", Rectangle2D( 13, 5, 77, 18 ), FALSE, -1, D3DCOLOR_XRGB( 230, 180, 150 ) ), FALSE, 6 );
					}
				}

				pWindowParty->AddElement( pDropdownMenu );

				//Button to open Dropdown Menu
				UI::Button_ptr pButtonDropdownMenu = std::make_shared<UI::Button>( Rectangle2D( 0, 0, 173, 49 ) );
				pButtonDropdownMenu->SetMouseEvent( ClickDownR );
				pButtonDropdownMenu->SetDropdownMenu( pDropdownMenu );
				p->AddElement( pButtonDropdownMenu );

				pListPartyMembers->AddWindow( p );
			}
		}

		if( !SETTINGSHANDLER->GetModel()->GetSettings().bLockUI )
		{
			pWindowParty->SetMoveBox( Rectangle2D( 0, 0, 175, 50 + pListPartyMembers->GetListHeight() ) );
			pWindowParty->SetCanMove( TRUE );
		}
		else
			pWindowParty->SetCanMove( FALSE );

		pWindowParty->Show();
	}
}

void CPartyWindow::UpdatePartyData( CPartyData * pcParty )
{
	if( pcParty->iRaidCount > 0 )
	{
		for( size_t i = 0; i < MAX_RAID - 1; i++ )
		{
			auto pListPartyRaid = pWindowRaid->GetElement<UI::List>( LISTID_PartyRaid + i );
			if( pListPartyRaid )
			{
				for( size_t j = 0; j < MAX_PARTY_MEMBERS; j++ )
				{
					PartyRaidMember psMember = pcParty->saRaid[i].saMembers[j];
					auto pWindowMember = pListPartyRaid->GetWindow( WINDOWID_Member1 + j );
					if( pWindowMember )
					{
						BOOL bDead = psMember.sData.sHP.sCur <= 0 ? TRUE : FALSE;
						BOOL bFar = !psMember.sData.sPosition.WithinPTDistance( &UNITDATA->sPosition, DISTANCE_UNIT_PARTY_MAX );

						auto pBackgroundImage = pWindowMember->GetElement<UI::ImageBox>( IMAGEID_Background );
						if( pBackgroundImage )
							pBackgroundImage->SetColor( bFar ? D3DCOLOR_ARGB( 120, 255, 255, 255 ) : -1 );

						auto pLifeBarImage = pWindowMember->GetElement<UI::ImageBox>( IMAGEID_LifeBar );
						if( pLifeBarImage )
						{
							if( psMember.sData.sHP.sMax > 0 )
								pLifeBarImage->SetWidth( (psMember.sData.sHP.sCur * 89) / psMember.sData.sHP.sMax );

							pLifeBarImage->SetColor( bFar ? D3DCOLOR_ARGB( 120, 255, 255, 255 ) : -1 );
						}

						auto pTextCharacterName = pWindowMember->GetElement<UI::Text>( TEXTID_Name );
						if( pTextCharacterName )
						{
							if( bDead )
								pTextCharacterName->SetColor( D3DCOLOR_XRGB( 255, 45, 45 ) );
							else if( bFar )
								pTextCharacterName->SetColor( D3DCOLOR_XRGB( 185, 175, 155 ) );
							else
								pTextCharacterName->SetColor( -1 );
						}
					}
				}

			}
		}
	}

	auto pListPartyMembers = pWindowParty->GetElement<UI::List>( LISTID_PartyMembers );
	if( pListPartyMembers )
	{
		for( int i = 0; i < pcParty->iMembersCount; i++ )
		{
			PartyMember psMember = pcParty->saMembers[i];

			auto pWindowMember = pListPartyMembers->GetWindow( WINDOWID_Member1 + i );
			if( pWindowMember )
			{
				BOOL bDead = psMember.sData.sHP.sCur <= 0 ? TRUE : FALSE;
				BOOL bFar = !psMember.sData.sPosition.WithinPTDistance( &UNITDATA->sPosition, DISTANCE_UNIT_PARTY_MAX );

				auto pBackgroundImage = pWindowMember->GetElement<UI::ImageBox>( IMAGEID_Background );
				if( pBackgroundImage )
					pBackgroundImage->SetColor( bFar ? D3DCOLOR_ARGB( 150, 255, 255, 255 ) : -1 );

				auto pIconImage = pWindowMember->GetElement<UI::ImageBox>( IMAGEID_Icon );
				if( pIconImage )
					pIconImage->SetColor( bFar ? D3DCOLOR_ARGB( 150, 255, 255, 255 ) : -1 );

				auto pLifeBarImage = pWindowMember->GetElement<UI::ImageBox>( IMAGEID_LifeBar );
				if( pLifeBarImage )
				{
					if( psMember.sData.sHP.sMax > 0 )
						pLifeBarImage->SetWidth( (psMember.sData.sHP.sCur * 119) / psMember.sData.sHP.sMax );

					pLifeBarImage->SetColor( bFar ? D3DCOLOR_ARGB( 150, 255, 255, 255 ) : -1 );
				}

				auto pManaBarImage = pWindowMember->GetElement<UI::ImageBox>( IMAGEID_ManaBar );
				if( pManaBarImage )
				{
					if( psMember.sData.sMP.sMax > 0 )
						pManaBarImage->SetWidth( (psMember.sData.sMP.sCur * 123) / psMember.sData.sMP.sMax );

					pManaBarImage->SetColor( bFar ? D3DCOLOR_ARGB( 150, 255, 255, 255 ) : -1 );
				}

				auto pTextCharacterLevel = pWindowMember->GetElement<UI::Text>( TEXTID_CharacterLevel );
				if ( pTextCharacterLevel )
				{
					int iLevel = psMember.sData.sLevel;
					if ( psMember.sData.iID == UNITDATA->iID )
						iLevel = UNITDATA->sCharacterData.iLevel;

					pTextCharacterLevel->SetText( FormatString( "%d", iLevel ) );
				}

				auto pCharacterClassImage = pWindowMember->GetElement<UI::ImageBox>( IMAGEID_Class );
				if( pCharacterClassImage )
				{
					pCharacterClassImage->Swap( bDead ? TRUE : FALSE );
					pCharacterClassImage->SetColor( bFar ? D3DCOLOR_ARGB( 150, 255, 255, 255 ) : -1 );
				}

				auto pLeaderIconImage = pWindowMember->GetElement<UI::ImageBox>( IMAGEID_IconLeader );
				if( pLeaderIconImage )
					pLeaderIconImage->SetColor( bFar ? D3DCOLOR_ARGB( 150, 255, 255, 255 ) : -1 );

				auto pTextCharacterName = pWindowMember->GetElement<UI::Text>( TEXTID_Name );
				if( pTextCharacterName )
				{
					if( bDead )
						pTextCharacterName->SetColor( D3DCOLOR_XRGB( 255, 45, 45 ) );
					else if( bFar )
						pTextCharacterName->SetColor( D3DCOLOR_XRGB( 185, 175, 155 ) );
					else
						pTextCharacterName->SetColor( -1 );
				}
			}
		}
	}
}

void CPartyWindow::LeaveParty()
{
	auto pListPartyMembers = pWindowParty->GetElement<UI::List>( LISTID_PartyMembers );
	if( pListPartyMembers )
	{
		pListPartyMembers->Clear();
		pWindowParty->Hide();
		pWindowRaid->Hide();
	}
}

void CPartyWindow::ToggleWindow()
{
	auto pListPartyMembers = pWindowParty->GetElement<UI::List>( LISTID_PartyMembers );
	if( pListPartyMembers )
	{
		if( pListPartyMembers->IsOpen() )
		{
			pListPartyMembers->Hide();
			pWindowParty->SetHeight( 15 );
			pWindowParty->SetCanMove( FALSE );
			pWindowRaid->Hide();
		}
		else
		{
			pListPartyMembers->Show();
			pWindowParty->SetHeight( 50 + pListPartyMembers->GetListHeight() );
			pWindowParty->SetCanMove( !SETTINGSHANDLER->GetModel()->GetSettings().bLockUI );
			pWindowParty->SetMoveBox( Rectangle2D( 0, 0, pWindowParty->GetWidth(), pWindowParty->GetHeight() ) );
			pWindowRaid->Show();
		}
	}
}

void CPartyWindow::BuildWindowParty()
{
	pWindowParty->SetPosition( 0, (RESOLUTION_HEIGHT >> 1) - (345 >> 1) );

	UI::Button_ptr pToggleParty = std::make_shared<UI::Button>( Rectangle2D( 14, 0, 18, 14 ) );
	pToggleParty->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\hud\\party\\expand.png", "game\\images\\UI\\hud\\party\\expand_.png" ) );
	pToggleParty->SetEvent( UI::Event::Build( std::bind( &CPartyWindow::ToggleWindow, this )) );
	pWindowParty->AddElement( pToggleParty );

	UI::ImageBox_ptr pImageParty = std::make_shared<UI::ImageBox>( Rectangle2D( 36, 0, 15, 15 ) );
	pImageParty->SetID( IMAGEID_PartyType );
	pImageParty->SetImage( pImagePartyMode[0] );
	pImageParty->SetColor( UI::Color( 220, 255, 255, 255 ).Get() );
	pWindowParty->AddElement( pImageParty );

	UI::List_ptr pListPartyMembers = std::make_shared<UI::List>( Rectangle2D( 0, 40, 290, 345 ) );
	pListPartyMembers->SetNoClip( TRUE );
	pListPartyMembers->SetID( LISTID_PartyMembers );
	pWindowParty->AddElement( pListPartyMembers );

	UI::Tooltip_ptr pTooltipHideParty = std::make_shared<UI::Tooltip>( Rectangle2D() );
	pTooltipHideParty->Init();
	pTooltipHideParty->SetFont( "Arial", 16, -1 );
	pTooltipHideParty->SetText( "Hide/Show Party" );
	pTooltipHideParty->SetLinkObject( pToggleParty );
	pTooltipHideParty->SetPosition( TOOLTIPPOSITION_Top );
	pTooltipHideParty->SetFollowMouse( FALSE );
	pWindowParty->AddElement( pTooltipHideParty );

	//Tooltip of Party Mode
	UI::Tooltip_ptr pTooltipPartyType = std::make_shared<UI::Tooltip>( Rectangle2D() );
	pTooltipPartyType->Init();
	pTooltipPartyType->SetID( TOOLTIPID_PartyType );
	pTooltipPartyType->SetLinkObject( pImageParty );
	pTooltipPartyType->SetFont( "Arial", 16, -1 );
	pTooltipPartyType->SetPosition( TOOLTIPPOSITION_Top );
	pTooltipPartyType->SetFollowMouse( FALSE );
	pWindowParty->AddElement( pTooltipPartyType );

	pWindowParty->Hide();
}

void CPartyWindow::BuildWindowRaid()
{
	pWindowRaid->SetPosition( 180, (RESOLUTION_HEIGHT >> 1) - (345 >> 1) );

	for( size_t i = 0; i < MAX_RAID - 1; i++ )
	{
		UI::List_ptr pListPartyRaid = std::make_shared<UI::List>( Rectangle2D( 0, 0, 115, 180 ) );
		pListPartyRaid->SetID( LISTID_PartyRaid + i );
		pWindowRaid->AddElement( pListPartyRaid );
	}

	pWindowRaid->Hide();
}

void CPartyWindow::ShowMessageBox( int iType, int iIndex )
{
	MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
	MESSAGEBOX->SetTitle( "Are you sure?" );

	if( iType == 1 )
	{
		MESSAGEBOX->SetDescription( "Do you want to disband the Party?" );
		MESSAGEBOX->SetEvent( std::bind( &CPartyWindow::OnDisbandPartyButtonClick, this ) );
	}
	else if( iType == 2 )
	{
		MESSAGEBOX->SetDescription( FormatString("Do you want  to expel %s?", PARTYHANDLER->GetPartyData().saMembers[iIndex].szCharacterName) );
		MESSAGEBOX->SetEvent( std::bind( &CPartyWindow::OnKickPartyButtonClick, this, iIndex ) );
	}
	else if( iType == 3 )
	{
		MESSAGEBOX->SetDescription( "Do you want to leave the party?" );
		MESSAGEBOX->SetEvent( std::bind( &CPartyWindow::OnLeavePartyButtonClick, this ) );
	}
	else if( iType == 4 )
	{
		MESSAGEBOX->SetDescription( FormatString( "Do you want to delegate %s as party leader?", PARTYHANDLER->GetPartyData().saMembers[iIndex].szCharacterName ) );
		MESSAGEBOX->SetEvent( std::bind( &CPartyWindow::OnDelegatePartyButtonClick, this, iIndex ) );
	}
	else if( iType == 5 )
	{ 
		MESSAGEBOX->SetDescription( FormatString("Do you want to change Party to %s?", GetPartyMode((EPartyMode)((PARTYHANDLER->GetPartyData().eMode+1)%3))) );
		MESSAGEBOX->SetEvent( std::bind( &CPartyWindow::OnChangePartyModeButtonClick, this ) );
	}

	MESSAGEBOX->Show();
}

void CPartyWindow::OnSelectMenuParty( int iIndex )
{
	auto pListPartyMembers = pWindowParty->GetElement<UI::List>( LISTID_PartyMembers );
	if( pListPartyMembers )
	{
		auto pWindowMember = pListPartyMembers->GetWindow( WINDOWID_Member1 + iIndex );
		if( pWindowMember )
		{
			UI::DropdownMenu_ptr pDropdownMenu = pWindowParty->GetElement<UI::DropdownMenu>( DROPDOWNMENUID_Options + (iIndex * 6) );

			if( pDropdownMenu )
			{
				int iOptionID = pDropdownMenu->GetSelectedOptionID();
				if( iOptionID == 1 )
					ShowMessageBox( 4, iIndex );
				else if( iOptionID == 2 )
					ShowMessageBox( 2, iIndex );
				else if( iOptionID == 3 )
					ShowMessageBox( 1, iIndex );
				else if( iOptionID == 4 )
					ShowMessageBox( 3, iIndex );
				else if( iOptionID == 5 )
					ShowMessageBox( 5, iIndex );
				else if( iOptionID == 6 )
					ShowMessageBox( 3, iIndex );
			}
		}
	}
}

void CPartyWindow::OnAcceptPartyButtonClick()
{
	if( MESSAGEBOX->GetType() )
	{
		if( iPartyRequestType > 0 )
		{
			if( iPartyRequestType == 1 )
				PARTYHANDLER->JoinPartyRequest();
			else if( iPartyRequestType == 2 )
				PARTYHANDLER->AcceptPartyRequest();
		}
	}
}

void CPartyWindow::OnAcceptRaidButtonClick()
{
	if( MESSAGEBOX->GetType() )
		PARTYHANDLER->JoinRaidRequest();
}

void CPartyWindow::OnDelegatePartyButtonClick( int iIndex )
{
	if( MESSAGEBOX->GetType() )
		PARTYHANDLER->ProcessPacket( &PacketActionParty( PARTYACTION_Delegate, PARTYHANDLER->GetPartyData().saMembers[iIndex].sData.iID ));
}

void CPartyWindow::OnKickPartyButtonClick( int iIndex )
{
	if( MESSAGEBOX->GetType() )
		PARTYHANDLER->ProcessPacket( &PacketActionParty( PARTYACTION_Kick, PARTYHANDLER->GetPartyData().saMembers[iIndex].sData.iID ));
}

void CPartyWindow::OnLeavePartyButtonClick()
{
	if( MESSAGEBOX->GetType() )
		PARTYHANDLER->ProcessPacket( &PacketActionParty( PARTYACTION_Leave ));
}

void CPartyWindow::OnDisbandPartyButtonClick()
{
	if( MESSAGEBOX->GetType() )
		PARTYHANDLER->ProcessPacket( &PacketActionParty( PARTYACTION_Disband ));
}

void CPartyWindow::OnChangePartyModeButtonClick()
{
	if( MESSAGEBOX->GetType() )
		PARTYHANDLER->ProcessPacket( &PacketActionParty( PARTYACTION_ChangeMode ) );
}
