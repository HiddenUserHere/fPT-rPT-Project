#include "stdafx.h"
#include "CChatBox.h"

CChatBox::CChatBox()
{
}

CChatBox::~CChatBox()
{
}

void CChatBox::Init()
{
	BuildWindow();

	//Initialize Friendly Commands
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/sweat", "e1" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/love", "e2" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/angry", "e3" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/bigeye", "e4" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/sad", "e5" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/cry", "e6" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/ops", "e7" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/nerd", "e8" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/craving", "e9" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/hurt", "e10" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/silence", "e11" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/crazy", "e12" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/hello", "e13" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/tired", "e14" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/shame", "e15" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/brokeheart", "e16" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/abused", "e17" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/kappa", "e18" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/kappapride", "e19" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/pogchamp", "e20" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/highthomas", "e21" ) );

	for ( int i = 0; i < 7; i++ )
		saItemLink[i].iID = LISTID_ChatAll + i;

	ZeroMemory( &cItemLinkDataWrapper, sizeof( ItemData ) );
}

void CChatBox::Render()
{
	pWindow1->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );

	/*auto pInputField = pWindow1->GetElement<UI::InputField>( INPUTFIELDID_Message );
	auto pButtonSendMessage = pWindow1->GetElement<UI::Button>( BUTTONID_SendMessage );
	BOOL bActive = FALSE;

	if( pWindow1->IsHover() )
		bActive = TRUE;
	if( pInputField->IsSelected() )
		bActive = TRUE;

	for( size_t i = 0; i < 6; i++ )
	{
		auto pButtonTabs = pWindow1->GetElement<UI::Button>( BUTTONID_TabAll + i );
		if( pButtonTabs )
			bActive ? pButtonTabs->Show() : pButtonTabs->Hide();
	}

	if( bActive )
		pWindow1->SetColorBackground( UI::Color( 70, 0, 0, 0 ) );
	else
		pWindow1->SetColorBackground( UI::Color( 0, 0, 0, 0 ) );

	if( pInputField )
		bActive ? pInputField->Show() : pInputField->Hide();

	if( pButtonSendMessage )
		bActive ? pButtonSendMessage->Show() : pButtonSendMessage->Hide();*/

	if ( cItemLinkDataWrapper.bValid )
		ITEMHANDLER->ShowItemInfo( &cItemLinkDataWrapper, FALSE );

	CheckCollideWindow( FALSE );
}

void CChatBox::AddMessage( std::string strMessage, EChatColor eChatColor, ItemData * pcItemData )
{
	if( !pWindow1 )
		return;

	if ( eChatColor == CHATCOLOR_Blue )
		if ( SOCIALHANDLER->GetFriends()->IsBlocked( GetPlayerName( strMessage ) ) )
			return;

	if ( (eChatColor != CHATCOLOR_Global) && (eChatColor != CHATCOLOR_Trade) && (eChatColor != CHATCOLOR_Notice) )
		if ( BATTLEROYALEHANDLER->InWaiting() || BATTLEROYALEHANDLER->InBattle() )
			return;

	std::vector<UI::List_ptr> vListToAdd;

	if( eChatColor != CHATCOLOR_Notice )
		vListToAdd.push_back( pWindow1->GetElement<UI::List>(LISTID_ChatAll) );
	else if( eChatColor == CHATCOLOR_Notice )
		vListToAdd.push_back( pWindow1->GetElement<UI::List>( LISTID_ChatNotice ) );

	if( eChatColor == CHATCOLOR_Party )
		vListToAdd.push_back( pWindow1->GetElement<UI::List>(LISTID_ChatParty) );
	else if( eChatColor == CHATCOLOR_Trade )
		vListToAdd.push_back( pWindow1->GetElement<UI::List>( LISTID_ChatTrade ) );
	else if( eChatColor == CHATCOLOR_Clan || eChatColor == CHATCOLOR_ClanLeader )
		vListToAdd.push_back( pWindow1->GetElement<UI::List>( LISTID_ChatClan ) );
	else if( eChatColor == CHATCOLOR_Whisper || eChatColor == CHATCOLOR_Blue || eChatColor == CHATCOLOR_Error )
		vListToAdd.push_back( pWindow1->GetElement<UI::List>( LISTID_ChatPM ) );
	else if( eChatColor == CHATCOLOR_Raid )
		vListToAdd.push_back( pWindow1->GetElement<UI::List>( LISTID_ChatRaid ) );

	if( vListToAdd.size() > 0 )
	{
		for( auto &v : vListToAdd )
		{
			if( v )
			{
				UI::Window_ptr pBaseWindow = std::make_shared <UI::Window>( Rectangle2D( 5, 0, 275, 0 ) );
				UI::Text_ptr pTextMessage = std::make_shared<UI::Text>( Rectangle2D( 0, 0, 0, 0 ) );

				//Add Item Link
				ItemData * pcItemLinkWrapper = new ItemData();
				if ( pcItemData )
					CopyMemory( pcItemLinkWrapper, pcItemData, sizeof( ItemData ) );
				else
					ZeroMemory( pcItemLinkWrapper, sizeof( ItemData ) );

				for ( int i = 0; i < MAX_LISTCHAT; i++ )
				{
					if ( saItemLink[i].iID == v->GetID() )
					{
						saItemLink[i].vItemData.push_back( pcItemLinkWrapper );
						break;
					}
				}

				if( eChatColor == CHATCOLOR_Normal && strMessage.find( ":" ) != std::string::npos )
				{
					pTextMessage->SetHighlightText( TRUE );
					pTextMessage->SetHighlightTextColor( D3DCOLOR_XRGB( 100, 255, 50 ) );
				}

				pTextMessage->SetFont( FONTNAME_DEFAULT, 16, 0, FALSE, FALSE, -1 );
				pTextMessage->SetColor( GetMessageColorFromType( eChatColor ) );

				if ( pcItemData )
				{
					pTextMessage->SetColor( D3DCOLOR_ARGB( 255, 240, 240, 0 ) );
					pBaseWindow->SetHoverEvent( UI::Event::Build( std::bind( &CChatBox::OnItemLinkHover, this, pcItemLinkWrapper, pBaseWindow, std::placeholders::_1 ) ) );

					if ( eChatColor != CHATCOLOR_Normal )
					{
						pTextMessage->SetHighlightText( TRUE );
						pTextMessage->SetHighlightTextColor( GetMessageColorFromType( eChatColor ) );
					}
				}
				
				pTextMessage->SetDropdownShadow( TRUE );
				pTextMessage->SetDropdownColor( UI::Color( 150, 0, 0, 0 ).Get() );
				pTextMessage->SetMultiLine( TRUE );
				pTextMessage->SetMaxTextWidth( 275 );
				pTextMessage->SetText( strMessage );
				pTextMessage->SetNoClip( TRUE );
				pBaseWindow->SetBox( Rectangle2D( 5, 0, 275, pTextMessage->GetHeightText() ) );
				
				//Emote
				{
					std::string strReplaceEmote = "";
					int iEmoteIndex = -1;
					if ( IsEmoteMessage( strMessage, strReplaceEmote, iEmoteIndex, eChatColor ) )
					{
						std::string strNewMessage = strMessage;
						strNewMessage.erase( strNewMessage.find( strReplaceEmote ), strReplaceEmote.length() );

						pTextMessage->SetText( strNewMessage );
						pBaseWindow->SetBox( Rectangle2D( 5, 0, 275, pTextMessage->GetHeightText() ) );

						UI::ImageBox_ptr pEmoteIcon = std::make_shared<UI::ImageBox>( Rectangle2D( (pTextMessage->GetTextWidth() * 7) - 4, 0, 103, 103 ) );
						pEmoteIcon->SetImage( UI::ImageLoader::LoadImage( "game\\textures\\particles\\emote%d.png", iEmoteIndex ) );
						pEmoteIcon->SetScale( 0.16f );
						pEmoteIcon->SetUseSelfScale( true );
						pBaseWindow->AddElement( pEmoteIcon );
					}
				}

				pBaseWindow->AddElement( pTextMessage );

				v->AddWindow( pBaseWindow );

				//Analyze
				if ( eChatColor == CHATCOLOR_Blue )
				{
					AddRecentPlayer( strMessage );
				}

				//Max list count?
				if ( v->GetCountItemsList() > MAX_CHATTEXTLIST )
				{
					//Get Window
					auto pTextWindow = v->GetWindowList()[0];

					//Old Position
					int iHeight = pTextWindow->GetHeight();

					v->DelWindow( pTextWindow );

					for ( int i = 0; i < MAX_LISTCHAT; i++ )
					{
						if ( saItemLink[i].iID == v->GetID() )
						{
							if ( saItemLink[i].vItemData.size() > 0 )
							{
								ItemData * pc = saItemLink[i].vItemData[0];
								if ( (cItemLinkDataWrapper.sItem.iChk1 == pc->sItem.iChk1) && (cItemLinkDataWrapper.sItem.iChk2 == pc->sItem.iChk2) )
									cItemLinkDataWrapper.bValid = FALSE;

								auto pIteratorItem = saItemLink[i].vItemData.begin();
								DELET( saItemLink[i].vItemData[0] );
								pIteratorItem = saItemLink[i].vItemData.erase( pIteratorItem );
								break;
							}
						}
					}
				}
			}
		}
	}
}

void CChatBox::SendMessageChat( std::string strMessage )
{
	if( strMessage.size() > 0 )
	{
		BOOL bSendToLoginServer = FALSE;

		std::string strEmote = strMessage;
		BOOL bEmote = OnEmoteCommand( strEmote );

		if ( iTabID == CHATBOXTABID_All )
		{
			if ( bEmote )
			{
				AddMessage( FormatString( "%s: %s", UNITDATA->sCharacterData.szName, strMessage.c_str() ), CHATCOLOR_Normal );
				CHATGAMEHANDLER->SendChat( FALSE, strEmote.c_str() );
				return;
			}
		}

		int iRetChat = CHATGAMEHANDLER->AnalyzeChat( (char*)strMessage.c_str() );

		BOOL bContinue = TRUE;

		if( strMessage[0] == '/' || strMessage[0] == '@' || strMessage[0] == '#' || strMessage[0] == '$' )
		{
			bContinue = FALSE;

			if ( bEmote )
				bContinue = TRUE;

			if ( strMessage[0] == '/' && (strMessage[1] == ';' || strMessage[1] == ':') )
			{
				strMessage = trim( strMessage );
				bSendToLoginServer = TRUE;
			}
			else if ( strMessage.size() > 1 && strMessage[1] == '!' )
				bSendToLoginServer = TRUE;
			else if ( strMessage.compare( 0, 6, "/CLAN>" ) == 0 )
				bSendToLoginServer = TRUE;
			else if ( (strMessage.compare( 0, 7, "/TRADE>" ) == 0 && strMessage.length() > 8) || (strMessage[0] == '$' && strMessage.length() > 2) )
			{
				if ( strMessage[0] == '$' )
				{
					strMessage.erase( 0, 1 );
					strMessage.insert( 0, "/TRADE>" );
				}

				bSendToLoginServer = TRUE;
			}
		}

		if( bContinue )
		{
			switch( iTabID )
			{
				case CHATBOXTABID_All:
				case CHATBOXTABID_PM:
					if( iRetChat == 0 )
						break;

					if( true )
					{
						int iLastMessageTick = (TICKCOUNT - dwLastMessageTick);
						if( dwLastMessageTick && iLastMessageTick < 2000 )
						{
							iTotalMessages++;

							if( iTotalMessages > 10 )
							{
								if( iLastMessageTick > 0 )
								{
									lstrcpy( UNITDATA->szMessage, FormatString( "%s: %s", UNITDATA->sCharacterData.szName, "My tongue froze because I talk too much -_-" ) );
									UNITDATA->dwMessageEndTime = TICKCOUNT + 1000 * 7;
									dwLastMessageTick = TICKCOUNT + 1000 * 7;
									
									OnEmoteCommand( std::string("e6"), UNITDATA );
								}

								return;
							}
						}
						else if( iLastMessageTick > 2000 )
							iTotalMessages = 0;

						dwLastMessageTick = TICKCOUNT;
					}

					strMessage = FormatString( "%s: %s", UNITDATA->sCharacterData.szName, strMessage.c_str() );
					STRINGCOPY( UNITDATA->szMessage, strMessage.c_str() );
					UNITDATA->dwMessageEndTime = TICKCOUNT + 1000 * 7;
					AddMessage( strMessage, CHATCOLOR_Normal );
					break;
				case CHATBOXTABID_Clan:
					strMessage = FormatString( "/CLAN> %s", strMessage.c_str() );
					bSendToLoginServer = TRUE;
					break;
				case CHATBOXTABID_Party:
					strMessage = FormatString( "@%s", strMessage.c_str() );
					break;
				case CHATBOXTABID_Raid:
					strMessage = FormatString( "#%s", strMessage.c_str() );
					break;
				case CHATBOXTABID_Trade:
					strMessage = FormatString( "/TRADE>%s", strMessage.c_str() );
					bSendToLoginServer = TRUE;
					break;
			}
		}

		if( iRetChat == 1 )
		{
			CHATGAMEHANDLER->SendChat( bSendToLoginServer, strMessage.c_str() );
		}
	}
}

void CChatBox::OnEnableChatBox()
{
	auto pInputField = pWindow1->GetElement<UI::InputField>( INPUTFIELDID_Message );

	if( pInputField )
	{
		pInputField->SetSelected( TRUE );
		pInputField->SetSelect( TRUE );
	}
}

void CChatBox::OnEnterKeyPressed( UIEventArgs eArgs )
{
	auto pInputField = pWindow1->GetElement<UI::InputField>( INPUTFIELDID_Message );

	if( pInputField )
	{
		if( pInputField->GetText().length() > 0 )
		{
			AddSaveCommand( pInputField->GetText() );
			SendMessageChat( pInputField->GetText() );

			//PM?
			if ( (pInputField->GetText().length() > 2) && (pInputField->GetText()[0] == '/') && ((pInputField->GetText()[1] == ':') || (pInputField->GetText()[1] == ';')) )
			{
				//Set Name
				if ( pInputField->GetText().find( ' ' ) != std::string::npos )
				{
					BOOL bAddBack = (pInputField->GetText().find( ' ' ) + 1) == pInputField->GetText().length() ? FALSE : TRUE;

					std::string str = pInputField->GetText().substr( 0, pInputField->GetText().find( ' ' ) + 1 );

					//Add it back
					pInputField->ClearInput();
					if ( bAddBack )
						pInputField->AddInput( str );

					return;
				}
			}

			if( (pInputField->GetText().length() > 7) && (pInputField->GetText()[0] == '/' && pInputField->GetText()[5] == '>') && pInputField->GetText().find( "/CLAN>" ) != std::string::npos )
			{
				pInputField->ClearInput();
				pInputField->AddInput( "/CLAN> " );
				return;
			}
		}
		else
			pInputField->SetSelected( FALSE );

		pInputField->ClearInput();
	}
}

BOOL CChatBox::IsInputFieldActive()
{
	auto pInputField = pWindow1->GetElement<UI::InputField>( INPUTFIELDID_Message );
	if( pInputField )
		return pInputField->IsSelected();

	return FALSE;
}

void CChatBox::SetInputFieldText( std::string strText )
{
	auto pInputField = pWindow1->GetElement<UI::InputField>( INPUTFIELDID_Message );
	if( pInputField )
		pInputField->AddInput( strText );
}

void CChatBox::OnMouseMove( class CMouse * pcMouse )
{
	pWindow1->OnMouseMove( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

BOOL CChatBox::OnMouseClick( class CMouse * pcMouse )
{
	auto pInputField = pWindow1->GetElement<UI::InputField>( INPUTFIELDID_Message );
	if( pInputField )
	{
		if( !pWindow1->IsHover() && pInputField->IsSelected() && !pInputField->GetText().length() )
		{
			pInputField->SetSelected( FALSE );
			pInputField->ClearInput();
		}
	}

	if ( (pcMouse->GetEvent() == ClickDownL) && KEYBOARDHANDLER->IsKeyDown( VK_CONTROL ) && pWindow1->IsOpen() )
	{
		if ( TICKCOUNT > dwDelayItemLinkTime )
		{
			SendItemLink( ITEMHANDLER->GetMouseItemHover() );
			dwDelayItemLinkTime = TICKCOUNT + 2000;
			pInputField->ClearInput();
			return TRUE;
		}
	}

	if ( pWindow1->OnMouseClick( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 ) )
		return TRUE;

	return FALSE;
}

BOOL CChatBox::OnKeyPress( class CKeyboard * pcKeyboard )
{
	if( READDWORD( 0x362B1E0 ) )
		return FALSE;

	if( pcKeyboard->GetEvent() == KeyDown && pcKeyboard->GetKey() == VK_ESCAPE && !pcKeyboard->IsKeyDown(VK_SHIFT) && pWindow1->IsOpen() )
	{
		auto pInputField = pWindow1->GetElement<UI::InputField>( INPUTFIELDID_Message );
		if( pInputField )
		{
			pInputField->SetSelected( FALSE );
			pInputField->ClearInput();
		}

		pWindow1->Hide();
		return TRUE;
	}

	if( pcKeyboard->GetEvent() == KeyDown && pcKeyboard->GetKey() == VK_RETURN && !pWindow1->IsOpen() && !MESSAGEBOX->IsOpen() )
	{
		OnEnableChatBox();
		pWindow1->Show();
		return TRUE;
	}

	if( pcKeyboard->GetEvent() == KeyDown && pcKeyboard->GetKey() == VK_RETURN && !IsInputFieldActive() && !MESSAGEBOX->IsOpen() )
	{
		OnEnableChatBox();
		return TRUE;
	}

	if ( pcKeyboard->GetEvent() == KeyDown && pcKeyboard->GetKey() == VK_TAB && IsInputFieldActive() )
	{
		auto pInputField = pWindow1->GetElement<UI::InputField>( INPUTFIELDID_Message );
		if ( pInputField )
		{
			pInputField->ClearInput();
			pInputField->AddInput( "/CLAN> " );
		}
		return TRUE;
	}

	if ( pcKeyboard->GetEvent() == KeyDown && pcKeyboard->GetKey() == VK_UP && IsInputFieldActive() )
	{
		auto pInputField = pWindow1->GetElement<UI::InputField>( INPUTFIELDID_Message );
		if ( pInputField )
		{
			pInputField->ClearInput();
			pInputField->AddInput( GetSaveCommand( TRUE ) );
		}
		return TRUE;
	}

	if ( pcKeyboard->GetEvent() == KeyDown && pcKeyboard->GetKey() == VK_DOWN && IsInputFieldActive() )
	{
		auto pInputField = pWindow1->GetElement<UI::InputField>( INPUTFIELDID_Message );
		if ( pInputField )
		{
			pInputField->ClearInput();
			pInputField->AddInput( GetSaveCommand( FALSE ) );
		}
		return TRUE;
	}

	if ( pcKeyboard->GetEvent() == KeyDown && pcKeyboard->GetKey() == VK_PRIOR && IsInputFieldActive() )
	{
		for ( size_t i = 0; i < 6; i++ )
		{
			auto pListChat = pWindow1->GetElement<UI::List>( LISTID_ChatAll + i );
			if ( pListChat )
			{
				if ( pListChat->IsOpen() )
				{
					auto pScroll = pListChat->GetScrollBar();
					if ( pScroll )
					{
						int iPercent = pScroll->GetPercent() - 5;
						if ( iPercent < 0 )
							iPercent = 0;

						pScroll->SetPercent( iPercent );
					}
					break;
				}
			}
		}

		return TRUE;
	}

	if ( pcKeyboard->GetEvent() == KeyDown && pcKeyboard->GetKey() == VK_NEXT && IsInputFieldActive() )
	{
		for ( size_t i = 0; i < 6; i++ )
		{
			auto pListChat = pWindow1->GetElement<UI::List>( LISTID_ChatAll + i );
			if ( pListChat )
			{
				if ( pListChat->IsOpen() )
				{
					auto pScroll = pListChat->GetScrollBar();
					if ( pScroll )
					{
						int iPercent = pScroll->GetPercent() + 5;
						if ( iPercent > 100 )
							iPercent = 100;

						pScroll->SetPercent( iPercent );
					}
					break;
				}
			}
		}

		return TRUE;
	}

	if( pcKeyboard->GetEvent() == KeyDown && pcKeyboard->IsKeyDown(VK_CONTROL) && (pcKeyboard->GetKey() >= VK_F1 && pcKeyboard->GetKey() <= VK_F10) )
	{
		char * pszMacroText = SETTINGSHANDLER->GetModel()->GetSettings().szaMacro[(pcKeyboard->GetKey() & 0x0F)];

		if( pszMacroText[0] != NULL )
		{
			OnEnableChatBox();
			auto pInputField = pWindow1->GetElement<UI::InputField>( INPUTFIELDID_Message );
			if ( pInputField )
			{
				pInputField->ClearInput();

				pInputField->AddInput( SETTINGSHANDLER->GetModel()->GetSettings().szaMacro[(pcKeyboard->GetKey() & 0x0F)] );
			}

			pWindow1->Show();
			return TRUE;
		}
	}

	if( pWindow1->OnKeyPress( pcKeyboard ) )
		return TRUE;

	return FALSE;
}

BOOL CChatBox::OnMouseScroll( CMouse * pcMouse )
{
	if( pWindow1->OnMouseScroll( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 ) )
		return TRUE;

	return FALSE;
}

BOOL CChatBox::OnKeyChar( CKeyboard * pcKeyboard )
{
	if( pWindow1->OnKeyChar( pcKeyboard ) )
		return TRUE;

	return FALSE;
}

void CChatBox::ToggleNotice( BOOL bOpen )
{
	int iY = 0;
	int iHeight = 0;

	auto pImageBoxChatBar = pWindow1->GetElement<UI::ImageBox>( IMAGEID_ChatBoxBar );
	auto pListChatNotice = pWindow1->GetElement<UI::List>( LISTID_ChatNotice );
	if( !pImageBoxChatBar || !pListChatNotice )
		return;

	if( bOpen )
	{
		iY = 74;
		iHeight = 100;
		pImageBoxChatBar->Show();
		pListChatNotice->Show();
	}
	else
	{
		iY = 8;
		iHeight = 166;
		pImageBoxChatBar->Hide();
		pListChatNotice->Hide();
	}

	for( size_t i = 0; i < 6; i++ )
	{
		auto pListChat = pWindow1->GetElement<UI::List>( LISTID_ChatAll + i );
		if( pListChat )
		{
			pListChat->SetBox( Rectangle2D( pListChat->GetX(), iY, pListChat->GetWidth(), iHeight ) );
			pListChat->GetScrollBar()->SetHeight( iHeight + 5 );
			pListChat->GetScrollBar()->SetPercent( 100 );
		}
	}
}

void CChatBox::HandlePacket( PacketChatBoxMessage * psPacket )
{
	if ( psPacket->iChatColor == CHATCOLOR_Trade )
	{
		if ( SETTINGSHANDLER->GetModel()->GetSettings().bNoTradeChat )
			return;
	}

	UnitData * pcUnitData = NULL;

	BOOL bEmote = FALSE;

	if( psPacket->lID )
		pcUnitData = UNITDATABYID( psPacket->lID );

	if ( pcUnitData )
		bEmote = OnEmoteCommand( std::string( psPacket->szChatBoxMessage ), pcUnitData );

	if ( ((pcUnitData && (pcUnitData->sCharacterData.iType == CHARACTERTYPE_Player || pcUnitData->iCharacterType == CHARACTERTYPE_Player)) || psPacket->lID == 0) )
		AddMessage( psPacket->szChatBoxMessage, psPacket->lID ? CHATCOLOR_Normal : psPacket->iChatColor );
	
	if ( pcUnitData && (psPacket->iChatColor != CHATCOLOR_Party) )
	{
		if ( bEmote == FALSE )
		{
			STRINGCOPY( pcUnitData->szMessage, psPacket->szChatBoxMessage );
			pcUnitData->dwMessageEndTime = TICKCOUNT + 5000;
		}
	}
}

void CChatBox::HandlePacket( PacketItemLinkChat * psPacket )
{
	if ( psPacket->eChatColor == CHATCOLOR_Trade )
	{
		if ( SETTINGSHANDLER->GetModel()->GetSettings().bNoTradeChat )
			return;
	}

	AddMessage( psPacket->szMessage, (EChatColor)psPacket->eChatColor, &psPacket->cItemData );
}

void CChatBox::ProcessPacket( PacketItemLinkChat * psPacket, BOOL bLoginServer )
{
	psPacket->iLength = sizeof( PacketItemLinkChat );
	psPacket->iHeader = PKTHDR_ChatItemLink;

	if ( bLoginServer )
		SENDPACKETL( psPacket );
	else
		SENDPACKETG( psPacket );
}

DWORD CChatBox::GetMessageColorFromType( EChatColor eType )
{
	switch( eType )
	{
		case CHATCOLOR_Error:
			return D3DCOLOR_ARGB( 255, 255, 128, 96 );
		case CHATCOLOR_Whisper:
			return D3DCOLOR_ARGB( 255, 150, 190, 255 );
		case CHATCOLOR_Blue:
			return D3DCOLOR_ARGB( 255, 150, 190, 255 );
		case CHATCOLOR_Global:
			return D3DCOLOR_ARGB( 255, 230, 160, 255 );
		case CHATCOLOR_Party:
			return D3DCOLOR_ARGB( 255, 200, 255, 0 );
		case CHATCOLOR_Clan:
			return D3DCOLOR_ARGB( 255, 255, 255, 80 );
		case CHATCOLOR_Trade:
			return D3DCOLOR_ARGB( 255, 200, 255, 255 );
		case CHATCOLOR_Notice:
			return D3DCOLOR_ARGB( 255, 200, 128, 128 );
		case CHATCOLOR_ClanLeader:
			return D3DCOLOR_ARGB( 255, 255, 180, 70 );
		case CHATCOLOR_Normal:
			return D3DCOLOR_ARGB( 255, 255, 255, 128 );
		case CHATCOLOR_Raid:
			return D3DCOLOR_ARGB( 255, 40, 240, 230 );
	}

	return D3DCOLOR_XRGB( 255, 255, 255 );
}

void CChatBox::OnSelectChatBoxTab( int iID )
{
	for ( size_t i = 0; i < 6; i++ )
	{
		auto pListChat = pWindow1->GetElement<UI::List>( LISTID_ChatAll + i );
		if ( pListChat )
		{
			pListChat->Hide();

			auto pScroll = pListChat->GetScrollBar();
			if ( pScroll )
				pScroll->SetPercent( 100 );
		}
	}

	auto pListChat = pWindow1->GetElement<UI::List>( LISTID_ChatAll + (iID - BUTTONID_TabAll) );
	if( pListChat )
	{
		iTabID = iID - BUTTONID_TabAll;
		pListChat->Show();
	}
}

void CChatBox::BuildWindow()
{
	pWindow1 = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 295, 218 ) );
	pWindow1->SetPosition( 4, RESOLUTION_HEIGHT - pWindow1->GetHeight() - 4 );

	pWindow1->SetMoveBox( Rectangle2D( 0, 0, pWindow1->GetWidth() - 15, pWindow1->GetHeight() - 40 ) );
	pWindow1->SetCanMove( !SETTINGSHANDLER->GetModel()->GetSettings().bLockUI );

	pWindow1->SetColorBackground( UI::Color( 70, 0, 0, 0 ) );

	UI::ImageBox_ptr pImageChatBoxBar = std::make_shared<UI::ImageBox>( Rectangle2D( 0, 66, 295, 3 ) );
	pImageChatBoxBar->SetID( IMAGEID_ChatBoxBar );
	pImageChatBoxBar->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\chatbox\\chatbar.png" ) );
	pWindow1->AddElement( pImageChatBoxBar );

	for( size_t i = 0; i < 6; i++ )
	{
		UI::List_ptr pListChat = std::make_shared<UI::List>( Rectangle2D( 0, 74, 280, 100 ) );
		pListChat->SetID( LISTID_ChatAll + i );
		pListChat->MakeScrollBar( Rectangle2D( 285, 0, 6, 105 ), "game\\images\\UI\\scroll\\barbg_v.png", "game\\images\\UI\\scroll\\icon_2.png" );
		pListChat->SetForceDown( TRUE );
		pListChat->SetScrollFirst( TRUE );

		if ( pListChat->GetID() != LISTID_ChatAll )
			pListChat->Hide();

		iTabID = CHATBOXTABID_All;

		pWindow1->AddElement( pListChat );
	}

	UI::List_ptr pListChatNotice = std::make_shared<UI::List>( Rectangle2D( 0, 2, 280, 62 ) );
	pListChatNotice->SetID( LISTID_ChatNotice );
	pListChatNotice->MakeScrollBar( Rectangle2D( 285, 2, 6, 54 ), "game\\images\\UI\\scroll\\barbg_v.png", "game\\images\\UI\\scroll\\icon_2.png" );
	pListChatNotice->SetForceDown( TRUE );
	pWindow1->AddElement( pListChatNotice );

	UI::Group_ptr p = std::make_shared<UI::Group>();
	pWindow1->AddElement( p );

	//Chat Tabs
	{
		UI::Group_ptr pGroupTabs = std::make_shared<UI::Group>();
		pWindow1->AddElement( pGroupTabs );

		int iBaseX = 3;
		int iBaseY = 177;

		UI::Button_ptr pButtonTabAll = std::make_shared<UI::Button>( Rectangle2D( iBaseX, iBaseY, 43, 15 ) );
		pButtonTabAll->SetGroup( pGroupTabs );
		pButtonTabAll->SetID( BUTTONID_TabAll );
		pButtonTabAll->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\hud\\chatbox\\all.png", "game\\images\\UI\\hud\\chatbox\\all_.png" ) );
		pButtonTabAll->SetEvent( UI::Event::Build( std::bind( &CChatBox::OnSelectChatBoxTab, this, pButtonTabAll->GetID() ) ));
		pButtonTabAll->SetSelect( TRUE );
		pButtonTabAll->SetSelected( TRUE );
		pWindow1->AddElement( pButtonTabAll );

		UI::Button_ptr pButtonTabClan = std::make_shared<UI::Button>( Rectangle2D( iBaseX + 45, iBaseY, 43, 15 ) );
		pButtonTabClan->SetGroup( pGroupTabs );
		pButtonTabClan->SetID( BUTTONID_TabClan );
		pButtonTabClan->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\hud\\chatbox\\clan.png", "game\\images\\UI\\hud\\chatbox\\clan_.png" ) );
		pButtonTabClan->SetEvent( UI::Event::Build( std::bind( &CChatBox::OnSelectChatBoxTab, this, pButtonTabClan->GetID() ) ) );
		pButtonTabClan->SetSelect( TRUE );
		pWindow1->AddElement( pButtonTabClan );

		UI::Button_ptr pButtonTabParty = std::make_shared<UI::Button>( Rectangle2D( iBaseX + 90, iBaseY, 43, 15 ) );
		pButtonTabParty->SetGroup( pGroupTabs );
		pButtonTabParty->SetID( BUTTONID_TabParty );
		pButtonTabParty->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\hud\\chatbox\\party.png", "game\\images\\UI\\hud\\chatbox\\party_.png" ) );
		pButtonTabParty->SetEvent( UI::Event::Build( std::bind( &CChatBox::OnSelectChatBoxTab, this, pButtonTabParty->GetID() ) ) );
		pButtonTabParty->SetSelect( TRUE );
		pWindow1->AddElement( pButtonTabParty );

		UI::Button_ptr pButtonTabRaid = std::make_shared<UI::Button>( Rectangle2D( iBaseX + 135, iBaseY, 43, 15 ) );
		pButtonTabRaid->SetGroup( pGroupTabs );
		pButtonTabRaid->SetID( BUTTONID_TabRaid );
		pButtonTabRaid->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\hud\\chatbox\\raid.png", "game\\images\\UI\\hud\\chatbox\\raid_.png" ) );
		pButtonTabRaid->SetEvent( UI::Event::Build( std::bind( &CChatBox::OnSelectChatBoxTab, this, pButtonTabRaid->GetID() ) ) );
		pButtonTabRaid->SetSelect( TRUE );
		pWindow1->AddElement( pButtonTabRaid );

		UI::Button_ptr pButtonTabTrade = std::make_shared<UI::Button>( Rectangle2D( iBaseX + 180, iBaseY, 43, 15 ) );
		pButtonTabTrade->SetGroup( pGroupTabs );
		pButtonTabTrade->SetID( BUTTONID_TabTrade );
		pButtonTabTrade->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\hud\\chatbox\\trade.png", "game\\images\\UI\\hud\\chatbox\\trade_.png" ) );
		pButtonTabTrade->SetEvent( UI::Event::Build( std::bind( &CChatBox::OnSelectChatBoxTab, this, pButtonTabTrade->GetID() ) ) );
		pButtonTabTrade->SetSelect( TRUE );
		pWindow1->AddElement( pButtonTabTrade );

		UI::Button_ptr pButtonTabPM = std::make_shared<UI::Button>( Rectangle2D( iBaseX + 225, iBaseY, 43, 15 ) );
		pButtonTabPM->SetGroup( pGroupTabs );
		pButtonTabPM->SetID( BUTTONID_TabPM );
		pButtonTabPM->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\hud\\chatbox\\pm.png", "game\\images\\UI\\hud\\chatbox\\pm_.png" ) );
		pButtonTabPM->SetEvent( UI::Event::Build( std::bind( &CChatBox::OnSelectChatBoxTab, this, pButtonTabPM->GetID() ) ) );
		pButtonTabPM->SetSelect( TRUE );
		pWindow1->AddElement( pButtonTabPM );
	}

	UI::InputField_ptr pInputMessage = std::make_shared<UI::InputField>( Rectangle2D( 0, 195, 270, 23 ) );
	pInputMessage->SetID( INPUTFIELDID_Message );
	pInputMessage->SetGroup( p );
	pInputMessage->SetFont( FONTNAME_DEFAULT, 16, 6, FALSE, FALSE, -1 );
	pInputMessage->SetText( "" );
	pInputMessage->SetSelect( TRUE );
	pInputMessage->SetMaxLength( MAX_CHATLENGTH );
	pInputMessage->SetTextAlign( EAlign::ALIGN_Left, EAlign::ALIGN_Middle );
	pInputMessage->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\chatbox\\input.png" ) );
	pInputMessage->BindActionKeyboard( UI::ActionKeyboard( VK_RETURN, UI::Event::Build( std::bind( &CChatBox::OnEnterKeyPressed, this, std::placeholders::_1 ) ) ) );
	pWindow1->AddElement( pInputMessage );

	UI::Button_ptr pButtonSendMessage = std::make_shared<UI::Button>( Rectangle2D( 270, 195, 25, 23 ) );
	pButtonSendMessage->SetID( BUTTONID_SendMessage );
	pButtonSendMessage->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\hud\\chatbox\\send.png", "game\\images\\UI\\hud\\chatbox\\send_.png" ) );
	pButtonSendMessage->SetEvent( UI::Event::Build( std::bind( &CChatBox::OnEnterKeyPressed, this, std::placeholders::_1 ) ) );
	pButtonSendMessage->SetSelect( TRUE );
	pWindow1->AddElement( pButtonSendMessage );
}

BOOL CChatBox::IsEmoteMessage( std::string strMessage, std::string & strReplace, int & iEmoteIndex, EChatColor eChatColor )
{
	BOOL bRet = FALSE;

	iEmoteIndex = -1;

	if ( strMessage.length() >= 2 )
	{
		iEmoteIndex = -1;

		std::string strCommand = strMessage;

		if ( (eChatColor == CHATCOLOR_Trade) || (eChatColor == CHATCOLOR_Global) )
		{
			if ( strMessage.find( ">" ) != std::string::npos )
				strCommand = trim(strMessage.substr( strMessage.find( ">" ) + 1 ));
		}
		else
		{
			if ( strMessage.find( ":" ) != std::string::npos )
				strCommand = trim(strMessage.substr( strMessage.find( ":" ) + 1 ));
		}

		if ( strCommand[0] == 'e' )
		{
			strReplace = strCommand;
			iEmoteIndex = atoi( strCommand.substr( 1 ).c_str() );
		}
		else if ( strCommand[0] == '/' )
		{
			for ( std::map<std::string, std::string>::iterator it = mFriendlyCommands.begin(); it != mFriendlyCommands.end(); it++ )
			{
				if ( STRINGCOMPAREI( it->first.c_str(), strCommand.c_str() ) )
				{
					strReplace = strCommand;
					iEmoteIndex = atoi( it->second.substr( 1 ).c_str() );
					break;
				}
			}
		}

		if ( (iEmoteIndex > 0) && (iEmoteIndex <= MAX_EMOTELIST) )
			bRet = TRUE;
	}
	return bRet;
}

void CChatBox::OnResolutionChanged()
{
	pWindow1->SetPosition( 4, RESOLUTION_HEIGHT - pWindow1->GetHeight() - 4 );

	CheckCollideWindow( TRUE );
}

BOOL CChatBox::OnEmoteCommand( std::string & strMessage, UnitData * pcUnitData )
{
	BOOL bRet = FALSE;
	std::string strEffectPath;
	int iEmoteIndex = 0;

	for( std::map<std::string, std::string>::iterator it = mFriendlyCommands.begin(); it != mFriendlyCommands.end(); it++ )
	{
		if( STRINGCOMPAREI( it->first.c_str(), strMessage.c_str() ) )
		{
			strMessage = it->second;
			break;
		}
	}

	size_t uPos = strMessage.find_first_of( ":" );
	if( uPos != std::string::npos )
		if( uPos + 1 <= strMessage.length() )
			strMessage = trim( strMessage.substr( strMessage.find_first_of( ":" ) + 1 ) );

	if( (strMessage.length() <= 3) && (strMessage.length() > 0) )
	{
		iEmoteIndex = atoi(strMessage.substr(1).c_str());

		if( strMessage[0] == 'e' && isdigit(strMessage[1]) && iEmoteIndex > 0 && iEmoteIndex <= MAX_EMOTELIST )
		{
			bRet			= TRUE;
			strEffectPath	= FormatString("game\\scripts\\particles\\Emote%d.luac", iEmoteIndex );
		}
	}

	if( pcUnitData && bRet )
	{
		CParticleScript * pc = PARTICLEFACTORY->LoadScript( strEffectPath, 0, TRUE );

		if( pc )
		{
			pc->SetID( iEmoteIndex + 1000 );
			Engine::ObjectOwnerUnit * pcOwner = new Engine::ObjectOwnerUnit( pcUnitData );
			pcOwner->AddAnimationTypeLostCondition( ANIMATIONTYPE_Die );
			pc->SetOwner( pcOwner );
			PARTICLEENGINE->AddScript( pc );
		}
	}

	return bRet;
}

void CChatBox::CheckCollideWindow( BOOL bResolutionChanged )
{
	if ( (((pWindow1->GetY() + pWindow1->GetHeight()) > (RESOLUTION_HEIGHT - 175)) && (pWindow1->GetX() < 800)) || bResolutionChanged )
	{
		//Open Inventory/Skills/Character Info Window
		if ( WINDOW_INVENTORY && WINDOW_YINVENTORYMOVE > 0 && (WINDOW_YINVENTORYMOVE != 256 || bResolutionChanged) )
			pWindow1->SetPosition( pWindow1->GetX(), (205 + (256 - WINDOW_YINVENTORYMOVE)) + (RESOLUTION_HEIGHT - 600) );

		if ( WINDOW_SKILLS && WINDOW_SKILLMOVE > 0 && (WINDOW_SKILLMOVE != 256 || bResolutionChanged) )
			pWindow1->SetPosition( pWindow1->GetX(), (205 + (256 - WINDOW_SKILLMOVE)) + (RESOLUTION_HEIGHT - 600) );

		if ( WINDOW_CHARACTERINFO && WINDOW_CHARACTERINFOMOVE > 0 && (WINDOW_CHARACTERINFOMOVE != 256 || bResolutionChanged) )
			pWindow1->SetPosition( pWindow1->GetX(), (205 + (256 - WINDOW_CHARACTERINFOMOVE)) + (RESOLUTION_HEIGHT - 600) );
	}

	//Closing
	if ( !WINDOW_INVENTORY && !WINDOW_SKILLS && !WINDOW_CHARACTERINFO )
	{
		if ( pWindow1->GetY() + pWindow1->GetHeight() > RESOLUTION_HEIGHT - 180 && pWindow1->GetX() + pWindow1->GetWidth() < 300 )
		{
			if ( WINDOW_YINVENTORYMOVE > 0 && WINDOW_YINVENTORYMOVE < 256 )
				pWindow1->SetPosition( pWindow1->GetX(), RESOLUTION_HEIGHT - pWindow1->GetHeight() - 4 );

			if ( WINDOW_SKILLMOVE > 0 && WINDOW_SKILLMOVE < 256 )
				pWindow1->SetPosition( pWindow1->GetX(), RESOLUTION_HEIGHT - pWindow1->GetHeight() - 4 );

			if ( WINDOW_CHARACTERINFOMOVE > 0 && WINDOW_CHARACTERINFOMOVE < 256 )
				pWindow1->SetPosition( pWindow1->GetX(), RESOLUTION_HEIGHT - pWindow1->GetHeight() - 4 );
		}
	}
}

void CChatBox::AddRecentPlayer( std::string strMessage )
{
	std::string strName = GetPlayerName( strMessage );
	if ( strName.length() > 0 )
	{
		SOCIALHANDLER->GetFriends()->AddRecent( strName );
	}
}

void CChatBox::AddSaveCommand( std::string strMessage )
{
	if ( (strMessage.length() > 2) && (strMessage[0] == '/') )
	{
		std::string str = strMessage.substr( 0, strMessage.find( ' ' ) );

		BOOL bFound = FALSE;
		for ( std::vector<std::string>::iterator it = vSaveCommand.begin(); it != vSaveCommand.end(); it++ )
		{
			std::string s = (*it);
			if ( STRINGCOMPARE( s.c_str(), str.c_str() ) )
			{
				bFound = TRUE;
				break;
			}
		}

		if ( bFound == FALSE )
		{
			vSaveCommand.push_back( str );
			if ( vSaveCommand.size() > MAX_SAVECOMMANDSCHAT )
			{
				auto it = vSaveCommand.begin();
				it = vSaveCommand.erase( it );
			}
		}
	}
}

std::string CChatBox::GetSaveCommand( BOOL bUP )
{
	std::string str = "";

	if ( vSaveCommand.size() > 0 )
	{
		if ( bUP == FALSE )
			str = vSaveCommand[(uSaveCommandWheel--) % vSaveCommand.size()];
		else
			str = vSaveCommand[(uSaveCommandWheel++) % vSaveCommand.size()];

		str.insert( str.length(), " " );
	}
	return str;
}

void CChatBox::SendItemLink( ItemData * pcItemData )
{
	if ( pcItemData )
	{
		PacketItemLinkChat sPacket;
		ZeroMemory( &sPacket, sizeof( PacketItemLinkChat ) );

		sPacket.eChatColor = CHATCOLOR_Normal;

		BOOL bLoginServer = FALSE;

		//Whisper?
		auto pInputField = pWindow1->GetElement<UI::InputField>( INPUTFIELDID_Message );
		if ( pInputField )
		{
			std::string strMessage = pInputField->GetText();

			if ( strMessage.length() > 2 )
			{
				if ( (strMessage[0] == '/') && ((strMessage[1] == ':') || (strMessage[1] == ';')) )
				{
					size_t iFind = strMessage.find( " " );
					std::string strName = "";
					strName = strMessage.substr( 2, iFind != std::string::npos ? (iFind - 2) : std::string::npos );
					if ( strName.length() > 0 )
					{
						STRINGCOPY( sPacket.szCharacterName, strName.c_str() );
						sPacket.eChatColor = CHATCOLOR_Whisper;
						AddRecentPlayer( strMessage );
						pInputField->ClearInput();
					}
				}
			}

			if ( strMessage[0] == '/' || strMessage[0] == '$' )
			{
				if ( (strMessage.compare( 0, 7, "/TRADE>" ) == 0 && strMessage.length() > 8) || (strMessage[0] == '$' && strMessage.length() > 2) )
				{
					sPacket.eChatColor = CHATCOLOR_Trade;

					bLoginServer = TRUE;

					if ( strMessage[0] == '$' )
					{
						strMessage.replace( 0, 1, "/TRADE>" );
					}
				}

				else if ( (strMessage.compare( 0, 6, "/CLAN>" ) == 0 && strMessage.length() > 7) )
				{
					sPacket.eChatColor = CHATCOLOR_Clan;
					bLoginServer = TRUE;
				}
			}
			else
			{
				if ( iTabID == CHATBOXTABID_Clan )
				{
					strMessage.insert( 0, "/CLAN>" );
					sPacket.eChatColor = CHATCOLOR_Clan;
					bLoginServer = TRUE;
				}
				else if ( iTabID == CHATBOXTABID_Party )
					sPacket.eChatColor = CHATCOLOR_Party;
				else if ( iTabID == CHATBOXTABID_Raid )
					sPacket.eChatColor = CHATCOLOR_Raid;
				else if ( iTabID == CHATBOXTABID_Trade )
				{
					strMessage.insert( 0, "/TRADE>" );
					sPacket.eChatColor = CHATCOLOR_Trade;
					bLoginServer = TRUE;
				}
			}

			STRINGCOPY( sPacket.szMessage, strMessage.c_str() );
		}

		CopyMemory( &sPacket.cItemData, pcItemData, sizeof( ItemData ) );
		ProcessPacket( &sPacket, bLoginServer );
	}
}

void CChatBox::OnItemLinkHover( ItemData * pcItemData, UI::Window_ptr pWindowHover, UIEventArgs eArgs )
{
	if ( pWindowHover->IsHover() )
	{
		if ( (cItemLinkDataWrapper.sItem.iChk1 != pcItemData->sItem.iChk1) || (cItemLinkDataWrapper.sItem.iChk2 != pcItemData->sItem.iChk2) )
		{
			CopyMemory( &cItemLinkDataWrapper, pcItemData, sizeof( ItemData ) );

			cItemLinkDataWrapper.sPosition.iX = pWindow1->GetX() + (pWindow1->GetWidth() >> 1);
			cItemLinkDataWrapper.sPosition.iY = MOUSEHANDLER->GetPosition()->iY;

			cItemLinkDataWrapper.iItemSlot = ITEMSLOTFLAG_None;
		}

		cItemLinkDataWrapper.bValid = TRUE;
	}
	else
		cItemLinkDataWrapper.bValid = FALSE;
}

std::string CChatBox::GetPlayerName( std::string strMessage )
{
	std::string strName = "";

	if ( strMessage.find( "From> " ) == 0 )
		strName = strMessage.substr( STRLEN( "From> " ), strMessage.find( ':' ) - STRLEN( "From> " ) );
	else if ( strMessage.find( "To> " ) == 0 )
		strName = strMessage.substr( STRLEN( "To> " ), strMessage.find( ':' ) - STRLEN( "To> " ) );
	else
	{
		if ( strMessage.length() > 2 )
		{
			size_t iFind = strMessage.find( ' ' );

			if ( (strMessage[0] == '/') && ((strMessage[1] == ':') || (strMessage[1] == ';')) )
				strName = strMessage.substr( 2, iFind != std::string::npos ? (iFind - 2) : 0 );
		}
	}

	return strName;
}
