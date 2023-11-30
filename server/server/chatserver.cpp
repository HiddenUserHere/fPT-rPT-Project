#include "stdafx.h"
#include "chatserver.h"


ChatServer::ChatServer()
{
}


ChatServer::~ChatServer()
{
}

void ChatServer::SQLLogChat( int iFromAccountID, int iFromCharacterID, int iToAccountID, int iToCharacterID, EChatColor eColor, const std::string & strMessage, const std::string & strFromIP, const std::string & strToIP )
{
    auto pcDB = SQLCONNECTION( DATABASEID_LogDB );
    if ( pcDB->Open() )
    {
        if ( pcDB->Prepare( "INSERT INTO ChatLog([FromAccountID],[FromCharacterID],[ToAccountID],[ToCharacterID],[ChatType],[Message],[FromIP],[ToIP],[Date]) VALUES(?,?,?,?,?,?,?,?,GETDATE())" ) )
        {
            pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iFromAccountID );
            pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iFromCharacterID );
            pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &iToAccountID );
            pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &iToCharacterID );
            pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &eColor );
            pcDB->BindParameterInput( 6, PARAMTYPE_String, (void *)strMessage.c_str(), strMessage.length() );
            pcDB->BindParameterInput( 7, PARAMTYPE_String, (void *)strFromIP.c_str(), strFromIP.length() );
            pcDB->BindParameterInput( 8, PARAMTYPE_String, (void *)strToIP.c_str(), strToIP.length() );
            pcDB->Execute();
        }

        pcDB->Close();
    }
}

void ChatServer::SendDebugChat( UserData * pcUserData, EChatColor eColor, const char * pszFormat, ... )
{
	User * pcUser = USERDATATOUSER( pcUserData );
	if ( pcUserData )
	{
		if ( pcUserData->iGameLevel > GAMELEVEL_One && pcUser->bDebugChat )
		{
			va_list ap;

			PacketChatBoxMessage sPacket;
			ZeroMemory( &sPacket, sizeof(PacketChatBoxMessage) );
			sPacket.iLength = sizeof(PacketChatBoxMessage);
			sPacket.iHeader = PKTHDR_ChatMessage;
			sPacket.iChatColor = eColor;

			va_start( ap, pszFormat );
			vsnprintf_s( sPacket.szChatBoxMessage, 255, pszFormat, ap );
			va_end( ap );

			SENDPACKET( USERDATATOUSER(pcUserData), &sPacket );
		}
	}
}

void ChatServer::SendChatAllUsersInStage( int iStage, EChatColor eColor, const char * pszFormat, ... )
{
	va_list ap;

	PacketChatBoxMessage sPacket;
	ZeroMemory( &sPacket, sizeof(PacketChatBoxMessage) );
	sPacket.iLength = sizeof(PacketChatBoxMessage);
	sPacket.iHeader = PKTHDR_ChatMessage;
	sPacket.iChatColor = eColor;

	va_start( ap, pszFormat );
	vsnprintf_s( sPacket.szChatBoxMessage, 255, pszFormat, ap );
	va_end( ap );

	for ( int i = 0; i < PLAYERS_MAX; i++ )
	{
		if ( USERSDATA[i].pcSocketData )
		{
			if ( USERSDATA[i].iMapID == iStage )
				SENDPACKET( USERDATATOUSER(&USERSDATA[i]), &sPacket );
		}
	}
}

void ChatServer::SendChatAll( EChatColor eColor, const char * pszText )
{
	PacketChatBoxMessage sPacket;
	ZeroMemory( &sPacket, sizeof( PacketChatBoxMessage ) );
	sPacket.iLength = sizeof( PacketChatBoxMessage );
	sPacket.iHeader = PKTHDR_ChatMessage;
	sPacket.iChatColor = eColor;

	STRINGCOPY( sPacket.szChatBoxMessage, pszText );

	for ( int i = 0; i < PLAYERS_MAX; i++ )
	{
		if ( USERSDATA[i].pcSocketData )
		{
			SENDPACKET( USERDATATOUSER( &USERSDATA[i] ), &sPacket );
		}
	}
}

void ChatServer::SendChatAllEx( EChatColor eColor, const char * pszFormat, ... )
{
	va_list ap;

	PacketChatBoxMessage sPacket;
	ZeroMemory( &sPacket, sizeof( PacketChatBoxMessage ) );
	sPacket.iLength = sizeof( PacketChatBoxMessage );
	sPacket.iHeader = PKTHDR_ChatMessage;
	sPacket.iChatColor = eColor;

	va_start( ap, pszFormat );
	vsnprintf_s( sPacket.szChatBoxMessage, 255, pszFormat, ap );
	va_end( ap );

	for ( UINT i = 0; i < USERSERVER->uUsersInGame; i++ )
	{
		User * pcUser = USERSERVER->pcaUserInGame[i];
		SENDPACKET( pcUser, &sPacket );
	}
}

void ChatServer::SendChatAllUsersInRange( int iX, int iZ, int iRange, EChatColor eColor, const char * pszFormat, ... )
{
	va_list ap;

	PacketChatBoxMessage sPacket;
	ZeroMemory( &sPacket, sizeof(PacketChatBoxMessage) );
	sPacket.iLength = sizeof(PacketChatBoxMessage);
	sPacket.iHeader = PKTHDR_ChatMessage;
	sPacket.iChatColor = eColor;

	va_start( ap, pszFormat );
	vsnprintf_s( sPacket.szChatBoxMessage, 255, pszFormat, ap );
	va_end( ap );

	for ( int i = 0; i < PLAYERS_MAX; i++ )
	{
		if ( USERSDATA[i].pcSocketData )
		{
			int iXP = ( iX - USERSDATA[i].sPosition.iX ) >> 8;
			int iZP = ( iZ - USERSDATA[i].sPosition.iZ ) >> 8;
			int iDist = iXP*iXP + iZP*iZP;
			if ( iDist < iRange && abs( iXP ) < 4096 && abs( iZP ) < 4096 )
				SENDPACKET( USERDATATOUSER(&USERSDATA[i]), &sPacket );
		}
	}
}

void ChatServer::SendChatAllUsersInClan( UserData * pcUserData, EChatColor eColor, const char * pszFormat, ... )
{
	if ( pcUserData && pcUserData->pcSocketData )
	{
		va_list ap;

		PacketChatBoxMessage sPacket;
		ZeroMemory( &sPacket, sizeof(PacketChatBoxMessage) );
		sPacket.iLength = sizeof(PacketChatBoxMessage);
		sPacket.iHeader = PKTHDR_ChatMessage;
		sPacket.iChatColor = eColor;

		va_start( ap, pszFormat );
		vsnprintf_s( sPacket.szChatBoxMessage, 256, pszFormat, ap );
		va_end( ap );

		for ( int i = 0; i < PLAYERS_MAX; i++ )
		{
			if ( USERSDATA[i].pcSocketData )
			{
				if ( USERSDATA[i].iClanID == pcUserData->iClanID && USERSDATA[i].pcSocketData != pcUserData->pcSocketData )
					SENDPACKET( USERDATATOUSER(&USERSDATA[i]), &sPacket );
			}
		}
	}
}


void ChatServer::SendChat( User * pcUser, EChatColor eColor, const char * pszText )
{
	if ( pcUser )
	{

		PacketChatBoxMessage sPacket;
		ZeroMemory( &sPacket, sizeof( PacketChatBoxMessage ) );
		sPacket.iHeader = PKTHDR_ChatMessage;
		sPacket.iChatColor = eColor;

		STRINGCOPY( sPacket.szChatBoxMessage, pszText );

		sPacket.iLength = 16 + lstrlenA( sPacket.szChatBoxMessage ) + 12;

		SENDPACKET( pcUser, &sPacket );
	}
}

void ChatServer::SendChatEx( User * pcUser, EChatColor eColor, const char * pszFormat, ... )
{
	if ( pcUser )
	{
		va_list ap;

		PacketChatBoxMessage sPacket;
		ZeroMemory( &sPacket, sizeof( PacketChatBoxMessage ) );
		sPacket.iHeader = PKTHDR_ChatMessage;
		sPacket.iChatColor = eColor;

		va_start( ap, pszFormat );
		vsnprintf_s( sPacket.szChatBoxMessage, 255, pszFormat, ap );
		va_end( ap );

		sPacket.iLength = 16 + lstrlenA( sPacket.szChatBoxMessage ) + 12;

		SENDPACKET( pcUser, &sPacket );
	}
}

void ChatServer::SendPersonalShopChat( UserData * pcUserData, DWORD dwObjectID, const char * pszFormat, ... )
{
	if ( pcUserData )
	{
		va_list ap;

		PacketChatBoxMessage sPacket;
		ZeroMemory( &sPacket, sizeof(PacketChatBoxMessage) );
		sPacket.iHeader = PKTHDR_PersonalShopMessage;
		sPacket.iChatColor = CHATCOLOR_Error;
		sPacket.lID = dwObjectID;

		va_start( ap, pszFormat );
		vsnprintf_s( sPacket.szChatBoxMessage, 255, pszFormat, ap );
		va_end( ap );

		sPacket.iLength = 16 + lstrlenA( sPacket.szChatBoxMessage ) + 12;

		SENDPACKET( USERDATATOUSER(pcUserData), &sPacket );
	}
}

void ChatServer::SendUserBoxChat( UserData * pcUserData, DWORD dwObjectID, const char * pszFormat, ... )
{
	if ( pcUserData )
	{
		va_list ap;

		PacketChatBoxMessage sPacket;
		ZeroMemory( &sPacket, sizeof(PacketChatBoxMessage) );
		sPacket.iHeader = PKTHDR_ChatMessage;
		sPacket.iChatColor = CHATCOLOR_Error;
		sPacket.lID = dwObjectID;

		va_start( ap, pszFormat );
		vsnprintf_s( sPacket.szChatBoxMessage, 255, pszFormat, ap );
		va_end( ap );

		sPacket.iLength = 16 + lstrlenA( sPacket.szChatBoxMessage ) + 12;

		SENDPACKET( USERDATATOUSER(pcUserData), &sPacket );
	}
}

void ChatServer::SendUserBoxChatRange( int iX, int iZ, int iRange, DWORD dwObjectID, const char * pszFormat, ... )
{
	va_list ap;

	PacketChatBoxMessage sPacket;
	ZeroMemory( &sPacket, sizeof(PacketChatBoxMessage) );
	sPacket.iHeader = PKTHDR_ChatMessage;
	sPacket.iChatColor = CHATCOLOR_Error;
	sPacket.lID = dwObjectID;

	va_start( ap, pszFormat );
	vsnprintf_s( sPacket.szChatBoxMessage, 255, pszFormat, ap );
	va_end( ap );

	sPacket.iLength = 16 + lstrlenA( sPacket.szChatBoxMessage ) + 12;

	for ( int i = 0; i < PLAYERS_MAX; i++ )
	{
		if ( USERSDATA[i].pcSocketData )
		{
			int iXP = ( iX - USERSDATA[i].sPosition.iX ) >> 8;
			int iZP = ( iZ - USERSDATA[i].sPosition.iZ ) >> 8;
			int iDist = iXP*iXP + iZP*iZP;
			int im = iRange / 1000;
			if ( iDist < iRange && abs( iXP ) < 4096 && abs( iZP ) < 4096 )
				SENDPACKET( USERDATATOUSER(&USERSDATA[i]), &sPacket );
		}
	}
}

void ChatServer::SendChatMessageBox( User * pcUser, const char * pszFormat, ... )
{
	va_list ap;

	PacketChatBoxMessage sPacket;
	ZeroMemory( &sPacket, sizeof(PacketChatBoxMessage) );
	sPacket.iHeader = PKTHDR_ChatMessageBox;
	sPacket.iChatColor = CHATCOLOR_Error;

	va_start( ap, pszFormat );
	vsnprintf_s( sPacket.szChatBoxMessage, 255, pszFormat, ap );
	va_end( ap );
	sPacket.iLength = 16 + lstrlenA( sPacket.szChatBoxMessage ) + 12;

	SENDPACKET( pcUser, &sPacket );
}

void ChatServer::SendChatSpyMember( UserData * pcUserData, void * pPacket )
{
	User * pcUser = USERDATATOUSER( pcUserData );
	SENDPACKET( USERDATATOUSER(pcUserData), pPacket );
	if ( pcUserData && pcUser->pcUserDataSpyoner )
	{
		SendChatEx( pcUser, EChatColor::CHATCOLOR_Error, "> [%s User]", pcUserData->sCharacterData.szName );
		SENDPACKET( USERDATATOUSER(pcUser->pcUserDataSpyoner), pPacket );
	}
}

void ChatServer::SendChatTrade( UserData * pcUserData, std::string strMessage )
{
	if ( pcUserData )
	{
		User * pcUser = USERDATATOUSER( pcUserData );

		if ( CanSendMessage( pcUser, strMessage, CHATCOLOR_Trade ) )
		{
			strMessage.erase( 0, 7 );
			SendChatAll( CHATCOLOR_Trade, FormatString( "[%c]%s: %s", GSERVER->GetServerName( pcUser->pcUserData->bServerIndexID )[0], CHARACTERSERVER->GetCharacterName( pcUserData ), strMessage.c_str() ) );
			pcUser->dwChatTradeTime = TICKCOUNT + ((1 * 60) * 1000);

            SQLLogChat( pcUser->iAccountID, pcUser->iCharacterID, 0, 0,
                (EChatColor)CHATCOLOR_Trade, strMessage, pcUser->pcUserData->pcSocketData->szIP, "127.0.0.1" );
		}
	}
}

void ChatServer::AddMute( const char * pszName )
{
	vMutedNames.push_back( pszName );
}

void ChatServer::ClearMute()
{
	vMutedNames.clear();
}

std::string ChatServer::GetFormatWhisper( User * pcUser, const char * pszMessage, BOOL bReceiverTime )
{
	return FormatString( "%s> %s: %s", bReceiverTime ? "From" : "To", CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), pszMessage );
}

void ChatServer::SendWhisper( User * pcUserSender, User * pcUserReceiver, const char * pszMessage )
{
	if ( pszMessage[0] != 0 )
	{
		if ( !pcUserReceiver->pcUserData->bDisabledWhisper || pcUserSender->GetGameLevel() >= GAMELEVEL_Two )
		{
			if ( CanSendMessage( pcUserSender, pszMessage, CHATCOLOR_Whisper ) )
			{
				//To Receiver
				CHATSERVER->SendChat( pcUserReceiver, CHATCOLOR_Blue, GetFormatWhisper( pcUserSender, pszMessage, TRUE ).c_str() );

				//To Sender
				CHATSERVER->SendChat( pcUserSender, CHATCOLOR_Blue, GetFormatWhisper( pcUserReceiver, pszMessage, FALSE ).c_str() );

                //Log it...
                SQLLogChat( pcUserSender->iAccountID, pcUserSender->iCharacterID, pcUserReceiver->iAccountID, pcUserReceiver->iCharacterID,
                    CHATCOLOR_Blue, pszMessage, pcUserSender->pcUserData->pcSocketData->szIP, pcUserReceiver->pcUserData->pcSocketData->szIP );
			}
		}
		else
			CHATSERVER->SendChatEx( pcUserSender, CHATCOLOR_Error, "%s disabled private message!", CHARACTERSERVER->GetCharacterName( pcUserReceiver->pcUserData ) );
	}
	else
		CHATSERVER->SendChatEx( pcUserSender, CHATCOLOR_Error, "%s is online at %s%s", CHARACTERSERVER->GetCharacterName( pcUserReceiver->pcUserData ), GSERVER->GetServerName( pcUserReceiver->pcUserData->bServerIndexID ), pcUserReceiver->pcUserData->bDisabledWhisper ? "(No Whisper)" : "" );
}

void ChatServer::SendWhisperItemLink( User * pcUserSender, User * pcUserReceiver, PacketItemLinkChat * psPacket )
{
	if ( !pcUserReceiver->pcUserData->bDisabledWhisper )
	{
		std::string strMessage = psPacket->szMessage;

		if ( strMessage.length() == 0 )
			strMessage = psPacket->cItemData.sItem.szItemName;

		if ( strMessage.find( "<item>" ) != std::string::npos )
			strMessage.replace( strMessage.find( "<item>" ), STRLEN( "<item>" ), psPacket->cItemData.sItem.szItemName );

		if ( CanSendMessage( pcUserSender, strMessage, CHATCOLOR_Whisper ) )
		{
			//To Receiver
			STRINGCOPY( psPacket->szMessage, GetFormatWhisper( pcUserSender, strMessage.c_str(), TRUE ).c_str() );
			SENDPACKET( pcUserReceiver, psPacket );

			//To Sender
			STRINGCOPY( psPacket->szMessage, GetFormatWhisper( pcUserReceiver, strMessage.c_str(), FALSE ).c_str() );
			SENDPACKET( pcUserSender, psPacket );

            SQLLogChat( pcUserSender->iAccountID, pcUserSender->iCharacterID, pcUserReceiver->iAccountID, pcUserReceiver->iCharacterID,
                CHATCOLOR_Blue, strMessage, pcUserSender->pcUserData->pcSocketData->szIP, pcUserReceiver->pcUserData->pcSocketData->szIP );
		}
	}
	else
		CHATSERVER->SendChatEx( pcUserSender, CHATCOLOR_Error, "%s disabled private message!", CHARACTERSERVER->GetCharacterName( pcUserReceiver->pcUserData ) );
}

void ChatServer::HandlePacket( User * pcUser, PacketItemLinkChat * psPacket )
{
	psPacket->cItemData.sItem.iChk1 += RandomI( 1000, 20000 );
	psPacket->cItemData.sItem.iChk2 += RandomI( 2000, 10000 );

	if ( psPacket->szCharacterName[0] != 0 )
	{
		User * pcUserReceiver = USERDATATOUSER( USERDATABYNAME( psPacket->szCharacterName ) );

		if ( pcUserReceiver )
			SendWhisperItemLink( pcUser, pcUserReceiver, psPacket );
		else
			CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> %s is offline or not found!", psPacket->szCharacterName );
	}
	else
	{
		std::string strMessage = GetMessageString( psPacket->szMessage, psPacket->eChatColor );

		if ( strMessage.length() == 0 )
			strMessage = psPacket->cItemData.sItem.szItemName;

		if ( strMessage.find( "<item>" ) != std::string::npos )
			strMessage.replace( strMessage.find( "<item>" ), STRLEN( "<item>" ), psPacket->cItemData.sItem.szItemName );

		if ( psPacket->eChatColor == CHATCOLOR_Normal )
		{
			if ( CanSendMessage( pcUser, psPacket->szMessage, psPacket->eChatColor ) )
			{
				STRINGFORMAT( psPacket->szMessage, "%s: %s", CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), strMessage.c_str() );

				for ( UINT i = 0; i < USERSERVER->uUsersInGame; i++ )
				{
					User * pcUserReceiver = USERSERVER->pcaUserInGame[i];
					if ( pcUserReceiver && (pcUserReceiver->bNetServer == FALSE) && (pcUser != pcUserReceiver) )
					{
						if ( pcUser->pcUserData->sPosition.WithinPTDistance( &pcUserReceiver->pcUserData->sPosition, USER_TALKRANGE ) )
							SENDPACKET( pcUserReceiver, psPacket );
					}
				}

				SENDPACKET( pcUser, psPacket );

                SQLLogChat( pcUser->iAccountID, pcUser->iCharacterID, 0, 0,
                    (EChatColor)psPacket->eChatColor, psPacket->szMessage, pcUser->pcUserData->pcSocketData->szIP, "127.0.0.1" );
			}
		}
		else if ( psPacket->eChatColor == CHATCOLOR_Trade )
		{
			if ( CanSendMessage( pcUser, psPacket->szMessage, psPacket->eChatColor ) )
			{
				STRINGFORMAT( psPacket->szMessage, "[%c]%s: %s", GSERVER->GetServerName( pcUser->pcUserData->bServerIndexID )[0], CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), strMessage.c_str() );

				for ( UINT i = 0; i < USERSERVER->uUsersInGame; i++ )
				{
					User * pcUserReceiver = USERSERVER->pcaUserInGame[i];

					if ( pcUserReceiver && (pcUserReceiver->bNetServer == FALSE) && (pcUser != pcUserReceiver) )
					{
						if ( pcUser->pcUserData->sPosition.WithinPTDistance( &pcUserReceiver->pcUserData->sPosition, USER_TALKRANGE ) )
							SENDPACKET( pcUserReceiver, psPacket );
					}
				}

				pcUser->dwChatTradeTime = TICKCOUNT + ((1 * 60) * 1000);

				SENDPACKET( pcUser, psPacket );

                SQLLogChat( pcUser->iAccountID, pcUser->iCharacterID, 0, 0,
                    (EChatColor)psPacket->eChatColor, psPacket->szMessage, pcUser->pcUserData->pcSocketData->szIP, "127.0.0.1" );
			}
		}
		else if ( (psPacket->eChatColor == CHATCOLOR_Party) || (psPacket->eChatColor == CHATCOLOR_Raid) )
		{
			STRINGFORMAT( psPacket->szMessage, "[%c]%s: %s", GSERVER->GetServerName( pcUser->pcUserData->bServerIndexID )[0], CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), strMessage.c_str() );

			if ( CanSendMessage( pcUser, psPacket->szMessage, psPacket->eChatColor ) )
			{			
				if ( pcUser->bParty && pcUser->psParty )
				{
					std::vector<User*> vUsers = PARTYHANDLER->GetPartyMembers( pcUser->psParty, (psPacket->eChatColor == CHATCOLOR_Raid) ? true : false );

					for ( auto &v : vUsers )
					{
						if ( v != pcUser )
							SENDPACKET( v, psPacket );
					}
				}

				SENDPACKET( pcUser, psPacket );

                SQLLogChat( pcUser->iAccountID, pcUser->iCharacterID, 0, 0,
                    (EChatColor)psPacket->eChatColor, psPacket->szMessage, pcUser->pcUserData->pcSocketData->szIP, "127.0.0.1" );
			}
		}
		else if ( psPacket->eChatColor == CHATCOLOR_Clan )
		{
			if ( CHARACTERSERVER->IsCharacterLeader( pcUser ) )
				psPacket->eChatColor = CHATCOLOR_ClanLeader;

			STRINGFORMAT( psPacket->szMessage, "[%c]%s: %s", GSERVER->GetServerName( pcUser->pcUserData->bServerIndexID )[0], CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), strMessage.c_str() );

			if ( pcUser->pcUserData->iClanID )
			{
				for ( UINT i = 0; i < USERSERVER->uUsersInGame; i++ )
				{
					User * pcUserReceiver = USERSERVER->pcaUserInGame[i];

					if ( pcUserReceiver && (pcUserReceiver->bNetServer == FALSE) && (pcUser != pcUserReceiver) )
					{
						if ( pcUserReceiver->pcUserData->iClanID && (pcUser->pcUserData->iClanID == pcUserReceiver->pcUserData->iClanID) )
							SENDPACKET( pcUserReceiver, psPacket );
					}
				}

				SENDPACKET( pcUser, psPacket );

                SQLLogChat( pcUser->iAccountID, pcUser->iCharacterID, 0, 0,
                    (EChatColor)psPacket->eChatColor, psPacket->szMessage, pcUser->pcUserData->pcSocketData->szIP, "127.0.0.1" );
			}
		}
	}
}

std::string ChatServer::GetMessageString( std::string strMessage, int iChatColor )
{
	std::string str = strMessage;

	if ( iChatColor == CHATCOLOR_Trade )
	{
		if ( strMessage.length() >= 7 )
		{
			if ( strMessage.find( "TRADE>" ) != std::string::npos )
			{
				str = strMessage.substr( strMessage.find_first_of( ">" ) + 1 );
				str = trim( str );
			}
		}
	}
	else if ( iChatColor == CHATCOLOR_Clan )
	{
		if ( strMessage.length() >= 6 )
		{
			if ( strMessage.find( "CLAN>" ) != std::string::npos )
			{
				str = strMessage.substr( strMessage.find_first_of( ">" ) + 1 );
				str = trim( str );
			}
		}
	}
	else if ( (iChatColor == CHATCOLOR_Party) || (iChatColor == CHATCOLOR_Raid) )
	{
		if ( (strMessage.length() >= 1) && ((strMessage[0] == '@') || (strMessage[0] == '#')) )
		{
			str = strMessage.substr( 1 );
			str = trim( str );
		}
	}

	return str;
}

BOOL ChatServer::CanSendMessage( User * pcUser, std::string strMessage, int iChatColor )
{
	if ( iChatColor == CHATCOLOR_Trade )
	{
		if ( pcUser->IsAccountShareDenied( ACCOUNTSHARE_DenyUseTradeChat ) )
		{
			CHATSERVER->SendChatMessageBox( pcUser, "You can't use Trade Chat, because you're logged in with the Share PW!" );
			return FALSE;
		}
	}
	else if ( (iChatColor == CHATCOLOR_Blue) || (iChatColor == CHATCOLOR_Whisper) )
	{
		if ( pcUser->IsAccountShareDenied( ACCOUNTSHARE_DenyChat ) )
		{
			CHATSERVER->SendChatMessageBox( pcUser, "You can't use Personal Chat, because you're logged in with the Share PW!" );
			return FALSE;
		}
	}

	BOOL bRet = TRUE;

	if ( iChatColor == CHATCOLOR_Trade )
	{
		if ( strMessage.length() >= 7 )
		{
			strMessage = strMessage.substr( strMessage.find_first_of( ">" ) + 1 );
			strMessage = trim( strMessage );

			std::string strFind = strMessage;

			//Can talk?
			if ( pcUser->bMuted == FALSE )
			{
				BOOL bMutedPlayer = FALSE;
				for ( const auto v : vMutedNames )
				{
					if ( STRINGCOMPAREI( CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), v.c_str() ) )
					{
						bMutedPlayer = TRUE;
						bRet = FALSE;
						break;
					}
				}

				if ( bMutedPlayer == FALSE )
				{
					//To lower
					for ( UINT i = 0; i < strMessage.length(); i++ )
						strFind[i] = tolower( strFind[i] );

					char cFirstLetter = strMessage[0];
					char cSecondLetter = strMessage[1];

					BOOL bFreeTrade = TRUE;

					//Not GM and on time delay?
					if ( pcUser->pcUserData->iGameLevel == GAMELEVEL_None && pcUser->dwChatTradeTime > TICKCOUNT )
					{
						if ( !FREE_TRADECHAT )
						{
							//Warning user
							SendChatEx( pcUser, CHATCOLOR_Error, "> Wait %d seconds to write again", (pcUser->dwChatTradeTime - TICKCOUNT) / 1000 );
							bFreeTrade = FALSE;
							bRet = FALSE;
						}
					}

					if ( bFreeTrade )
					{
						/*
						//Not correct first letters?
						if ( (cFirstLetter != 'S' && cFirstLetter != 'C' && cFirstLetter != 'B' && cFirstLetter != 'P' && cFirstLetter != 'T') || cSecondLetter != '>' )
						{
							//Notify Users
							SendChat( pcUser, CHATCOLOR_Error, "> You can only start your phrase with" );
							SendChat( pcUser, CHATCOLOR_Error, "S> For selling" );
							SendChat( pcUser, CHATCOLOR_Error, "B> For buying" );
							SendChat( pcUser, CHATCOLOR_Error, "T> For trading" );
							SendChat( pcUser, CHATCOLOR_Error, "P> Looking for party" );
							SendChat( pcUser, CHATCOLOR_Error, "C> Looking for clan" );
							SendChat( pcUser, CHATCOLOR_Error, "Example: P> Need party in Iron2" );
							bRet = FALSE;
						}
						*/
					}

					if ( bRet )
					{
						for ( int i = 0; i < _countof( pszaWordsTrade ); i++ )
						{
							//Is found words? don't write in chat
							if ( strstr( strFind.c_str(), pszaWordsTrade[i] ) )
							{
								bRet = FALSE;
								break;
							}
						}
					}
				}
			}
		}
		else
			bRet = FALSE;
	}

	return bRet;
}
