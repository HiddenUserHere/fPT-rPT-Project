#include "stdafx.h"
#include "netserver.h"
#include "UserServer.h"
#include "userdataserver.h"

int												iListServers = 0;

#define RECVNET_BUFFER_LEN						512

extern BOOL bFreeExpEvent;
extern int iExpFreeExp;

NetServer::NetServer()
{
}


NetServer::~NetServer()
{

}

void NetServer::Reconnect( class SocketData * pcSocketData )
{

}


void NetServer::Connect( NetConnection * nc )
{
	nc->pcSocketData = SOCKETSERVER->Connect( nc->szIP, nc->iPort );

	if ( nc->pcSocketData )
	{
		User * u = nc->pcSocketData->u;
		if ( u )
		{
			u->bNetServer					= TRUE;
			nc->pcSocketData->bKeepAlive	= TRUE;
		}
		PacketNetIdentifier sPacket;
		sPacket.iLength		= sizeof( PacketNetIdentifier );
		sPacket.iHeader		= PKTHDR_NetIdentifier;
		sPacket.iServerID	= SERVER_CODE;
		sPacket.iPort		= SERVER_PORT;
		sPacket.uPasswordNUM = 0x6A44556A;
		SendPacket( nc->pcSocketData, &sPacket );
	}
}

void NetServer::AddConnection( char * pszIP, int iPort, int iServerType )
{
	NetConnection * psClient	= new NetConnection;
	STRINGCOPY( psClient->szIP, pszIP );
	psClient->iPort				= iPort;
	psClient->iServerType		= iServerType;
	psClient->pcSocketData		= NULL;
	vNetConnections.push_back( psClient );
}

NetConnection * NetServer::AddNetConnection( class SocketData * pcSocketData )
{
	NetConnection * psClient	= new NetConnection;
	STRINGCOPY( psClient->szIP, pcSocketData->szIP );
	psClient->iPort				= 0;
	psClient->iServerType		= GAME_SERVER == TRUE ? 0 : -1;
	psClient->pcSocketData		= pcSocketData;
	vNetConnections.push_back( psClient );
	return psClient;
}

BOOL NetServer::Disconnected( class SocketData * sd )
{
	for( std::vector<NetConnection*>::iterator it = vNetConnections.begin(); it != vNetConnections.end(); it++ )
	{
		NetConnection * nc = *it;

		if( nc->pcSocketData == sd )
		{
			SOCKETSERVER->RemoveIocpHandler( sd );

			nc->pcSocketData = NULL;
			Connect( nc );

			return TRUE;
		}
	}
	return FALSE;
}


void NetServer::ConnectAll()
{
	for ( std::vector<NetConnection*>::iterator it = vNetConnections.begin(); it != vNetConnections.end(); it++ )
	{
		NetConnection * nc = *it;
		Connect( nc );
	}
}

void NetServer::DisconnectAll()
{
	for ( std::vector<NetConnection*>::iterator it = vNetConnections.begin(); it != vNetConnections.end(); it++ )
	{
		NetConnection * nc = *it;
		CALL_WITH_ARG1( 0x0055C390, (DWORD)nc->pcSocketData );
	}
}

NetConnection * NetServer::GetLoginServerConnection()
{
	for ( std::vector<NetConnection*>::iterator it = vNetConnections.begin(); it != vNetConnections.end(); it++ )
	{
		NetConnection * nc = *it;
		if ( nc->iServerType == 0 )
			return nc;
	}
	return NULL;
}

void NetServer::SendPacketGameServers( void * packet )
{
	for ( std::vector<NetConnection*>::iterator it = vNetConnections.begin(); it != vNetConnections.end(); it++ )
	{
		NetConnection * nc = *it;
		if ( nc->iServerType != 0 && nc->iServerType != -1 )
			SendPacket( nc->pcSocketData, packet );
	}
}

void NetServer::SendBotAdd( void * pPacket )
{
	if ( GAME_SERVER )
	{
		NetConnection * nc = GetLoginServerConnection();
		if ( nc )
			SendPacket( nc->pcSocketData, pPacket );
	}
}

void NetServer::SendExp( UserData *pcUserData, INT64 iExp )
{
	if ( pcUserData && GAME_SERVER )
	{
		PacketNetExp sPacket;
		sPacket.iLength = sizeof( PacketNetExp );
		sPacket.iHeader = PKTHDR_NetExp;
		sPacket.uObjectID = pcUserData->iID;
		sPacket.iExp = iExp;
		NetConnection * nc = GetLoginServerConnection();
		if ( nc )
			SendPacket( nc->pcSocketData, &sPacket );
	}
}

void NetServer::GiveEXP( UserData *pcUserData, INT64 iExp )
{
	if ( pcUserData && GAME_SERVER )
	{
		PacketNetExp sPacket;
		sPacket.iLength = sizeof( PacketNetExp );
		sPacket.iHeader = PKTHDR_NetGiveExp;
		sPacket.uObjectID = pcUserData->iID;
		sPacket.iExp = iExp;
		NetConnection * nc = GetLoginServerConnection();
		if ( nc )
			SendPacket( nc->pcSocketData, &sPacket );
	}
}

void NetServer::SendExpLogin( UserData *pcUserData, INT64 iExp )
{
	if ( pcUserData )
	{
		PacketNetExp sPacket;
		sPacket.iLength = sizeof( PacketNetExp );
		sPacket.iHeader = PKTHDR_NetExp;
		sPacket.uObjectID = pcUserData->iID;
		sPacket.iExp = iExp;
		SendPacketGameServers( &sPacket );
	}
}

void NetServer::SendForceOrb( UserData * pcUserData, BOOL bCity )
{
	if ( pcUserData && GAME_SERVER )
	{
		PacketNetForceOrbTime sPacket;
		sPacket.iLength		= sizeof( PacketNetForceOrbTime );
		sPacket.iHeader		= PKTHDR_NetForceOrb;
		sPacket.dwObjectID	= pcUserData->iID;
		sPacket.bUpdate		= bCity;
		sPacket.dwTime		= pcUserData->ForceOrbTime;
		NetConnection * nc = GetLoginServerConnection();
		if ( nc )
			SendPacket( nc->pcSocketData, &sPacket );
	}
}

void NetServer::SendEXPPotion( UserData * pcUserData, int iPercent )
{
	if ( pcUserData && GAME_SERVER )
	{
		PacketNetEXPPotion sPacket;
		sPacket.iLength		= sizeof( PacketNetEXPPotion );
		sPacket.iHeader		= PKTHDR_NetPremiumEXP;
		sPacket.dwObjectID	= pcUserData->iID;
		sPacket.iPercent	= iPercent;
		NetConnection * nc = GetLoginServerConnection();
		if ( nc )
			SendPacket( nc->pcSocketData, &sPacket );
	}
}

void NetServer::SendEXPEventLogin( int iPercent )
{
	PacketNetExpEvent s;
	s.iLength	= sizeof( PacketNetExpEvent );
	s.iHeader	= PKTHDR_NetEXPEvent;
	s.iPercent	= iPercent;
	SendPacketGameServers( &s );
}

void NetServer::SendClan( UserData *pcUserData )
{
	if ( pcUserData && GAME_SERVER )
	{
		PacketNetClan sPacket;
		sPacket.iLength = sizeof( PacketNetClan );
		sPacket.iHeader = PKTHDR_NetClan;
		sPacket.uObjectID = pcUserData->iID;
		sPacket.dwClan = pcUserData->iClanID;
		NetConnection * nc = GetLoginServerConnection();
		if ( nc )
			SendPacket( nc->pcSocketData, &sPacket );
	}
}
void NetServer::SendSyncChar( UserData *pcUserData )
{
	if ( pcUserData && GAME_SERVER )
	{
		PacketNetCharacterSync sPacket;
		sPacket.iLength = sizeof( PacketNetCharacterSync );
		sPacket.iHeader = PKTHDR_NetCharacterSync;
		sPacket.dwObjectID = pcUserData->iID;
		STRINGCOPY( sPacket.szCharName, pcUserData->sCharacterData.szName );
		NetConnection * nc = GetLoginServerConnection();
		if ( nc )
			SendPacket( nc->pcSocketData, &sPacket );
	}
}

void NetServer::DisconnectUser( UserData *pcUserData )
{
	if ( pcUserData )
	{
		PacketNetDisconnectUser sPacket;
		sPacket.iLength = sizeof( PacketNetDisconnectUser );
		sPacket.iHeader = PKTHDR_NetDisconnectUser;
		sPacket.dwObjectID = pcUserData->iID;
		if ( GAME_SERVER )
		{
			NetConnection * nc = GetLoginServerConnection();
			if ( nc )
				SendPacket( nc->pcSocketData, &sPacket );
		}
		else
			SendPacketGameServers( &sPacket );
	}
}

void NetServer::OnUnloadUser( UserData * pcUserData )
{
	if ( pcUserData && LOGIN_SERVER )
	{
		PacketNetUnloadUser sPacket;
		sPacket.iLength = sizeof( PacketNetUnloadUser );
		sPacket.iHeader = PKTHDR_NetUnloadUser;
		sPacket.dwObjectID = pcUserData->iID;
		SendPacketGameServers( &sPacket );
	}
}

void NetServer::SendGameMaster( UserData * pcUserData )
{
	if ( pcUserData )
	{
		PacketNetGameMaster sPacket;
		sPacket.iLength = sizeof( PacketNetGameMaster );
		sPacket.iHeader = PKTHDR_NetGameMaster;
		sPacket.uObjectID = pcUserData->iID;
		sPacket.iGameMasterLevel = pcUserData->iGameLevel;
		NetConnection * nc = GetLoginServerConnection();
		if ( nc )
			SendPacket( nc->pcSocketData, &sPacket );
	}
}
void NetServer::OnGetGameMaster( UserData * pcUserData )
{
	if ( pcUserData && GAME_SERVER )
	{
		PacketNetGetGameMaster sPacket;
		sPacket.iLength = sizeof( PacketNetGetGameMaster );
		sPacket.iHeader = PKTHDR_NetGetGameMaster;
		sPacket.uObjectID = pcUserData->iID;
		sPacket.iGameMasterLevel = pcUserData->iGameLevel;
		STRINGCOPY( sPacket.szName, pcUserData->sCharacterData.szName );
		NetConnection * nc = GetLoginServerConnection();
		if ( nc )
			SendPacket( nc->pcSocketData, &sPacket );
	}
}

void NetServer::GetExp( UserData * pcUserData )
{
	PacketNetGetExp sPacket;
	sPacket.iLength = sizeof( PacketNetGetExp );
	sPacket.iHeader = PKTHDR_NetGetExp;
	sPacket.uObjectID = pcUserData->iID;
	SendPacketGameServers( &sPacket );
}

void NetServer::SendVersion( int iVersion )
{
	PacketNetVersion sPacket;
	sPacket.iLength		= sizeof( PacketNetVersion );
	sPacket.iHeader		= PKTHDR_NetVersion;
	sPacket.iVersion	= iVersion;
	SendPacketGameServers( &sPacket );
}

void NetServer::SendTradeCoin( int iID, int iReceiverID )
{
	PacketNetTradeCoin s;
	s.iLength = sizeof( PacketNetTradeCoin );
	s.iHeader = PKTHDR_NetTradeCoin;
	s.iID = iID;
	s.iReceiverID = iReceiverID;
	SendPacketGameServers( &s );
}

void NetServer::SendQuestData( User * pcUser )
{
	PacketNetT5QuestData s;
	s.iLength			= sizeof( PacketNetT5QuestData );
	s.iHeader			= PKTHDR_NetQuestT5Data;
	s.iID				= pcUser->GetID();
	s.uQuestID			= pcUser->uQuestT5ID;
	s.sQuestValue[0]	= pcUser->sQuestT5ProgressValue[0];
	s.sQuestValue[1]	= pcUser->sQuestT5ProgressValue[1];
	SendPacketGameServers( &s );
}

void NetServer::SendQuestUpdateData( User * pcUser )
{
	PacketNetQuestUpdateData s;
	s.iLength			= sizeof( PacketNetQuestUpdateData );
	s.iHeader			= PKTHDR_NetQuestUpdateData;
	s.iID				= pcUser->GetID();
	CopyMemory( s.saPacket, pcUser->saPacketQuestData, sizeof( PacketQuestUpdateData ) * MAX_QUESTPACKETDATA );
	SendPacketGameServers( &s );
}

void NetServer::SendHardwareID( User * pcUser )
{
	if ( LOGIN_SERVER )
	{
		PacketNetHardwareID sPacket;
		sPacket.iLength = sizeof( PacketNetHardwareID );
		sPacket.iHeader = PKTHDR_NetHardwareID;
		sPacket.iID = pcUser->pcUserData->iID;
		STRINGCOPY( sPacket.szHardwareID, pcUser->szHardwareID );
		SendPacketGameServers( &sPacket );
	}
}

void NetServer::SendUpdateMessageEvent()
{
    if ( LOGIN_SERVER )
    {
        PacketSimple sPacket;
        sPacket.iLength = sizeof( PacketSimple );
        sPacket.iHeader = PKTHDR_NetUpdateEventMessage;

        SendPacketGameServers( &sPacket );
    }
}

void NetServer::SendPvPBuff( std::vector<int> vID )
{
	if ( LOGIN_SERVER )
	{
		PacketNetPvPBuff sPacket;
		sPacket.iLength = sizeof( PacketNetPvPBuff );
		sPacket.iHeader = PKTHDR_NetPvPBuff;
		sPacket.iCount	= 0;
		for ( const auto iID : vID )
			sPacket.iaUsersID[sPacket.iCount++] = iID;

		SendPacketGameServers( &sPacket );
	}
}

void NetServer::ReceivePvPBuff( PacketNetPvPBuff * psPacket )
{
	std::vector<int> v;

	for ( int i = 0; i < psPacket->iCount; i++ )
		v.push_back( psPacket->iaUsersID[i] );

	PVPSERVER->SetPvPBuffs( v );

	v.clear();
}

void NetServer::SendPvPResetLogin()
{
	PacketNetResetPvPLogin sPacket;
	sPacket.iLength = sizeof( PacketNetResetPvPLogin );
	sPacket.iHeader = PKTHDR_NetResetPvPLogin;
	sPacket.bReset = true;
	NetConnection * nc = GetLoginServerConnection();
	if ( nc )
		SendPacket( nc->pcSocketData, &sPacket );
}

void NetServer::SendPvPUpdateGameServer( PacketNetUpdatePvPBellatraBuff * psPacket )
{
	psPacket->iLength = sizeof( PacketNetUpdatePvPBellatraBuff );
	psPacket->iHeader = PKTHDR_NetUpdatePvPGameServer;
	SendPacketGameServers( psPacket );
}

void NetServer::SendBellatraRequest( User * pcUser, int iGold )
{
	PacketNetBellatraRequest sPacket;
	sPacket.iLength = sizeof( PacketNetBellatraRequest );
	sPacket.iHeader = PKTHDR_NetBellatraRequest;
	sPacket.iGold	= iGold;
	sPacket.iID		= pcUser->GetID();
	SendPacketGameServers( &sPacket );
}

void NetServer::SendBellatraGiveGold( User * pcUser, int iGold )
{
	PacketNetBellatraGiveGold sPacket;
	sPacket.iLength = sizeof( PacketNetBellatraGiveGold );
	sPacket.iHeader = PKTHDR_NetBellatraGiveGold;
	sPacket.iGold = iGold;
	sPacket.iID = pcUser->GetID();
	NetConnection * nc = GetLoginServerConnection();
	if ( nc )
		SendPacket( nc->pcSocketData, &sPacket );
}

void NetServer::HandlePvPBufferLogin( bool bNotifyReset, bool bAddBuff )
{
	PacketNetPvPLoginHandle sPacket;
	sPacket.iLength = sizeof( PacketNetPvPLoginHandle );
	sPacket.iHeader = PKTHDR_NetPvPHandle;
	sPacket.bNotifyReset = bNotifyReset;
	sPacket.bAddBuff = bAddBuff;
	NetConnection * nc = GetLoginServerConnection();
	if ( nc )
		SendPacket( nc->pcSocketData, &sPacket );
}

void NetServer::ReceiveHandlePvPBuffer( PacketNetPvPLoginHandle * psPacket )
{
	PVPSERVER->HandlePvPBuffers( psPacket->bNotifyReset, psPacket->bAddBuff );
}

void NetServer::SendActionFieldEvent( BOOL b )
{
	if ( LOGIN_SERVER )
	{
		PacketNetActionField sPacket;
		sPacket.iLength = sizeof( PacketNetActionField );
		sPacket.iHeader = PKTHDR_NetActionField;
		sPacket.bEvent = b;
		SendPacketGameServers( &sPacket );
	}
}

void NetServer::SendBattleRoyaleStatus( User * pcUser )
{
	if ( GAME_SERVER )
	{
		PacketNetBattleRoyaleStatus sPacket;
		sPacket.iLength			= sizeof( PacketNetBattleRoyaleStatus );
		sPacket.iHeader			= PKTHDR_NetBattleRoyaleStatus;
		sPacket.iID				= pcUser->GetID();
		sPacket.bInBattleRoyale = pcUser->bInBattleRoyale;
		NetConnection * nc		= GetLoginServerConnection();
		if ( nc )
			SendPacket( nc->pcSocketData, &sPacket );
	}
}

void NetServer::SendBattleRoyaleEnter( User * pcUser )
{
	if ( GAME_SERVER )
	{
		PacketNetBattleRoyaleEnter sPacket;
		sPacket.iLength = sizeof( PacketNetBattleRoyaleEnter );
		sPacket.iHeader = PKTHDR_NetBattleRoyaleEnter;
		sPacket.iID = pcUser->GetID();
		STRINGCOPY( sPacket.szCharacterName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );
		NetConnection * nc = GetLoginServerConnection();
		if ( nc )
			SendPacket( nc->pcSocketData, &sPacket );
	}
}

void NetServer::SendActionFieldRequest( PacketNetActionFieldRequest * psPacket, BOOL bSendToLogin )
{
	psPacket->iLength = sizeof( PacketNetActionFieldRequest );
	psPacket->iHeader = PKTHDR_NetActionFieldRequest;

	if ( bSendToLogin )
	{
		if ( NetConnection * nc = GetLoginServerConnection() )
			SendPacket( nc->pcSocketData, psPacket );
	}
	else
		SendPacketGameServers( psPacket );
}

void NetServer::SendEXPEventQuestFree(PacketNetQuestEXPFree * psPacket)
{
	psPacket->iLength = sizeof(PacketNetQuestEXPFree);
	psPacket->iHeader = PKTHDR_NetQuestEXPFreeEvent;

	SendPacketGameServers(psPacket);
}

void NetServer::SendPacket( class SocketData * pcSocketData, void * packet )
{
	if ( pcSocketData )
		SENDPACKETSOCKET( pcSocketData, packet, FALSE );
}

// Login Server receive from gameserver
void NetServer::OnReceiveServer( UserData * pcUserDataServer, void * pPacket )
{
	UserData * pcUserData = NULL;
	User * pcUser = NULL;

	User * us = USERDATATOUSER( pcUserDataServer );

	int iPacketID = ((Packet*)pPacket)->iHeader;

	NetConnection * nc = NULL;

	if( pcUserDataServer )
	{
		nc = GetNetConnectionBySocketData( pcUserDataServer->pcSocketData );

		if( nc == NULL )
			nc = AddNetConnection( pcUserDataServer->pcSocketData );

		if( nc == NULL )
			return;
	}

	if ( nc == NULL )
		return;

	if ( iPacketID == PKTHDR_NetIdentifier )
	{
		for ( std::vector<NetConnection*>::iterator it = vNetConnections.begin(); it != vNetConnections.end(); it++ )
		{
			NetConnection * ncit = *it;
			if ( (ncit->iServerType == ((PacketNetIdentifier*)pPacket)->iServerID) && (((PacketNetIdentifier*)pPacket)->uPasswordNUM == 0x6A44556A) )
			{
				nc->iPort			= ((PacketNetIdentifier*)pPacket)->iPort;
				ncit->pcSocketData	= nc->pcSocketData;
				us->bNetServer		= TRUE;
				us->pcUserData->pcSocketData->bKeepAlive = TRUE;
				vNetConnections.pop_back();

				WRITEERR( "NET SERVER2" );
				return;
			}
		}
		nc->iServerType	= ((PacketNetIdentifier*)pPacket)->iServerID;
		nc->iPort		= ((PacketNetIdentifier*)pPacket)->iPort;
		us->bNetServer = TRUE;
		us->pcUserData->bKeepAlive = TRUE;
		us->pcUserData->pcSocketData->bKeepAlive = TRUE;
		return;
	}

	switch ( iPacketID )
	{
		//Bless Castle
		case PKTHDR_NetBCStatusReq:
			BLESSCASTLESERVER->NetSendBlessCastleStatus( nc );
			break;
		case PKTHDR_NetBCStatusSnd:
			BLESSCASTLESERVER->NetRecvBlessCastleStatus( (PacketNetBCStatus*)pPacket );
			break;
		case PKTHDR_NetBCNewOwnerClanID:
			BLESSCASTLESERVER->SetBlessCastleOwnerClanID( ((PacketNetBCNewOwnerClanID*)pPacket)->dwNewClanID );
			break;

		//Experience
		case PKTHDR_NetExp:
			pcUserData = USERSERVER->GetUserdata( ((PacketNetExp*)pPacket)->uObjectID );
			pcUser = USERDATATOUSER( pcUserData );
			if ( pcUserData && USERDATATOUSER(pcUserData) )
			{
				CHARACTERSERVER->SetExp( pcUserData, ((PacketNetExp*)pPacket)->iExp );
				pcUser->bReceivedExp = TRUE;
			}
			break;
		case PKTHDR_NetGiveExp:
			pcUserData = USERSERVER->GetUserdata( ((PacketNetExp*)pPacket)->uObjectID );
			pcUser = USERDATATOUSER( pcUserData );
			if ( pcUserData && pcUser )
			{
				CHARACTERSERVER->GiveEXP( pcUser, ((PacketNetExp*)pPacket)->iExp );
			}
			break;

		case PKTHDR_NetGameMaster:
			pcUserData = USERSERVER->GetUserdata( ((PacketNetGameMaster*)pPacket)->uObjectID );
			if ( pcUserData )
				pcUserData->iGameLevel = (EGameLevel)((PacketNetGameMaster*)pPacket)->iGameMasterLevel;
			break;

		case PKTHDR_NetGetGameMaster:
			pcUserData = USERSERVER->GetUserdata( ((PacketNetGetGameMaster*)pPacket)->szName );
			pcUser = USERDATATOUSER( pcUserData );
			if ( pcUserData && pcUser )
			{
				((PacketNetGetGameMaster*)pPacket)->iGameMasterLevel = pcUserData->iGameLevel;
				((PacketNetGetGameMaster *)pPacket)->eAccountShare = pcUser->eAccountShare;
				((PacketNetGetGameMaster *)pPacket)->bFirst = TRUE;
				SendPacket( nc->pcSocketData, pPacket );
				SendHardwareID( USERDATATOUSER( pcUserData ) );

				NETSERVER->SendQuestData( USERDATATOUSER( pcUserData ) );
			}
			break;

		case PKTHDR_NetDisconnectUser:
			pcUserData = USERSERVER->GetUserdata( ((PacketNetGameMaster*)pPacket)->uObjectID );
			pcUser = USERDATATOUSER( pcUserData );
			if ( pcUserData && pcUserData->pcSocketData && pcUser )
			{
				ACCOUNTSERVER->OnDisconnectUser( pcUserData );
				LOGSERVER->LogSocketClose( pcUserData->pcSocketData, "NetServer::OnReceiveServer(PKTHDR_NetDisconnectUser)", _ReturnAddress() );
				WNDPROC_SOCKETCLOSE( SOCKETSERVER->GetHWND(), pcUserData->pcSocketData );
			}
			break;

		case PKTHDR_NetForceOrb:
			pcUserData = USERSERVER->GetUserdata( ((PacketNetForceOrbTime*)pPacket)->dwObjectID );
			if ( pcUserData && pcUserData->pcSocketData )
			{
				pcUserData->dwForceOrb_SaveTime		= ((PacketNetForceOrbTime*)pPacket)->dwTime;
			}
			break;

		case PKTHDR_NetPremiumEXP:
			pcUserData = USERSERVER->GetUserdata( ((PacketNetEXPPotion*)pPacket)->dwObjectID );
			pcUser = USERDATATOUSER( pcUserData );
			if ( pcUserData && pcUserData->pcSocketData && pcUser )
			{
				pcUser->iEXPPotion = ((PacketNetEXPPotion*)pPacket)->iPercent;
			}
			break;

		case PKTHDR_NetCharacterSync:
			pcUserData = USERSERVER->GetUserdata( ((PacketNetCharacterSync*)pPacket)->dwObjectID );
			if ( pcUserData && pcUserData->pcSocketData )
			{
				STRINGCOPY( pcUserData->szCharacterName, ((PacketNetCharacterSync*)pPacket)->szCharName );
			}
			break;

		case PKTHDR_NetBotAdd:
			BOTSERVER->AddGameBot( (PacketBotLogin*)pPacket );
			break;

		case PKTHDR_NetClan:
			pcUserData = USERSERVER->GetUserdata( ((PacketNetGameMaster*)pPacket)->uObjectID );
			pcUser = USERDATATOUSER( pcUserData );
			if ( pcUserData && pcUser && pcUser->dwTimeSync )
			{
				DWORD dwOldClan;
				dwOldClan = pcUserData->iClanID;

				if ( pcUserData )
					pcUserData->iClanID = ((PacketNetClan*)pPacket)->dwClan;

				if ( LOGIN_SERVER && pcUserData->iClanID && pcUserData->iClanID != dwOldClan )
				{
					int iServerID = pcUser->pData.dwServerID;
					if ( iServerID < 0 || iServerID > 3 )
						iServerID = 0;

					CHATSERVER->SendChatAllUsersInClan( pcUserData, CHATCOLOR_Clan, "> %s is now online at %s", pcUserData->sCharacterData.szName, GSERVER->GetServerName( iServerID ) );
				}
			}
			break;

		case PKTHDR_NetUsersOnline:
			LOGSERVER->HandlePacket( (PacketNetUsersOnline*)pPacket );
			break;

		case PKTHDR_NetQuestT5Data:
			QUESTSERVER->HandlePacket( (PacketNetT5QuestData*)pPacket );
			break;

		case PKTHDR_NetPvPHandle:
			ReceiveHandlePvPBuffer( (PacketNetPvPLoginHandle*)pPacket );
			break;

		case PKTHDR_NetBattleRoyaleStatus:
			BATTLEROYALEHANDLER->HandlePacket( (PacketNetBattleRoyaleStatus *)pPacket );
			break;

		case PKTHDR_NetBattleRoyaleEnter:
			BATTLEROYALEHANDLER->HandlePacket( (PacketNetBattleRoyaleEnter *)pPacket );
			break;

		case PKTHDR_NetBellatraGiveGold:
			BELLATRASERVERHANDLER->HandleNetPacket( (PacketNetBellatraGiveGold *)pPacket );
			break;

		case PKTHDR_NetActionFieldRequest:
			ACTIONFIELDHANDLER->HandleNetPacket( (PacketNetActionFieldRequest *)pPacket );
			break;

		case PKTHDR_NetResetPvPLogin:
			PVPSERVER->ResetPvPBuffer();
			break;

		default:
			break;
	}
}

void NetServer::OnReceiveClient( UserData * pcUserDataServer, void * pPacket )
{
	UserData * pcUserData = NULL;

	User * us = USERDATATOUSER( pcUserDataServer );

	NetConnection * nc = NULL;

	User * pcUser = NULL;

	int iPacketID = ((Packet*)pPacket)->iHeader;

	if( pcUserDataServer )
	{
		nc = GetNetConnectionBySocketData( pcUserDataServer->pcSocketData );

		if( nc == NULL )
			nc = AddNetConnection( pcUserDataServer->pcSocketData );

		if( nc == NULL )
			return;
	}

	if ( nc == NULL )
		return;

	if ( iPacketID == PKTHDR_NetIdentifier )
	{
		for ( std::vector<NetConnection*>::iterator it = vNetConnections.begin(); it != vNetConnections.end(); it++ )
		{
			NetConnection * ncit = *it;
			if ( (ncit->iServerType == ((PacketNetIdentifier*)pPacket)->iServerID) && (((PacketNetIdentifier*)pPacket)->uPasswordNUM == 0x6A44556A) )
			{
				nc->iPort			= ((PacketNetIdentifier*)pPacket)->iPort;
				ncit->pcSocketData	= nc->pcSocketData;
				us->bNetServer		= TRUE;
				us->pcUserData->pcSocketData->bKeepAlive = TRUE;
				vNetConnections.pop_back();

				WRITEERR( "NET SERVER1" );
				return;
			}
		}
		nc->iServerType	= ((PacketNetIdentifier*)pPacket)->iServerID;
		nc->iPort		= ((PacketNetIdentifier*)pPacket)->iPort;
		us->bNetServer = TRUE;
		us->pcUserData->pcSocketData->bKeepAlive = TRUE;
		return;
	}

	switch ( iPacketID )
	{
		// BC
		case PKTHDR_NetBCStatusReq:
			BLESSCASTLESERVER->NetSendBlessCastleStatus( nc );
			break;
		case PKTHDR_NetBCStatusSnd:
			BLESSCASTLESERVER->NetRecvBlessCastleStatus( (PacketNetBCStatus*)pPacket );
			break;
		case PKTHDR_NetBCNewOwnerClanID:
			BLESSCASTLESERVER->SetBlessCastleOwnerClanID( ((PacketNetBCNewOwnerClanID*)pPacket)->dwNewClanID );
			break;

		case PKTHDR_NetDisconnectUser:
			pcUserData = USERSERVER->GetUserdata( ((PacketNetGameMaster*)pPacket)->uObjectID );
			if ( pcUserData && pcUserData->pcSocketData )
			{
				pcUser = USERDATATOUSER( pcUserData );
				if ( pcUser )
					pcUser->bDisconnectNet = TRUE;

				ACCOUNTSERVER->OnDisconnectUser( pcUserData );
				LOGSERVER->LogSocketClose( pcUserData->pcSocketData, "NetServer::OnReceiveServer(PKTHDR_NetDisconnectUser)", _ReturnAddress() );
				WNDPROC_SOCKETCLOSE( SOCKETSERVER->GetHWND(), pcUserData->pcSocketData );
			}
			break;

		case PKTHDR_NetActionField:
			SERVERCOMMAND->SetActionFieldEvent( ((PacketNetActionField*)pPacket)->bEvent );
			break;

		case PKTHDR_NetTradeCoin:
			TRADEHANDLER->HandleTradeCoin( USERDATATOUSER( USERDATABYID( ((PacketNetTradeCoin*)pPacket)->iID ) ), USERDATATOUSER( USERDATABYID( ((PacketNetTradeCoin*)pPacket)->iReceiverID ) ) );
			break;

		case PKTHDR_NetUnloadUser:
			pcUserData = USERSERVER->GetUserdata( ((PacketNetUnloadUser*)pPacket)->dwObjectID );
			pcUser = USERDATATOUSER( pcUserData );
			if ( pcUserData && pcUser )
			{
				USERSERVER->OnUnLoadUser( pcUser );
			}
			break;

		case PKTHDR_NetGetExp:
			pcUserData = USERSERVER->GetUserdata( ((PacketNetGetExp*)pPacket)->uObjectID );
			if ( pcUserData )
			{
				NETSERVER->SendExp( pcUserData, CHARACTERSERVER->GetExp( pcUserData ) );
				NETSERVER->SendClan( pcUserData );
			}
			break;

		case PKTHDR_NetHardwareID:
			pcUserData = USERSERVER->GetUserdata( ((PacketNetHardwareID*)pPacket)->iID );
			pcUser = USERDATATOUSER( pcUserData );
			if ( pcUserData && pcUser )
			{
				STRINGCOPY( pcUser->szHardwareID, ((PacketNetHardwareID*)pPacket)->szHardwareID );
			}
			break;

		case PKTHDR_NetEXPEvent:
			*(int*)(0x0084601C) = ((PacketNetExpEvent*)pPacket)->iPercent;
			break;

		case PKTHDR_NetVersion:
			SERVERCOMMAND->SetVersion( ((PacketNetVersion*)pPacket)->iVersion );
			break;

		case PKTHDR_NetExp:
			pcUserData = USERSERVER->GetUserdata( ( ( PacketNetExp* )pPacket )->uObjectID );
			if ( pcUserData )
			{
				CHARACTERSERVER->SetExp( pcUserData, ((PacketNetExp*)pPacket)->iExp );
			}
			break;

		case PKTHDR_NetGetGameMaster:
			pcUserData = USERSERVER->GetUserdata( ((PacketNetGetGameMaster*)pPacket)->szName );
			pcUser = USERDATATOUSER( pcUserData );
			if ( pcUserData && pcUser )
			{
				pcUser->SetAccountShare( ((PacketNetGetGameMaster *)pPacket)->eAccountShare );
				pcUserData->iGameLevel = (EGameLevel)((PacketNetGetGameMaster*)pPacket)->iGameMasterLevel;

				//Share PW
				if ( pcUser->eAccountShare != ACCOUNTSHARE_None )
					CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "> You're logged in with the Share PW!" );

				if ( pcUserData->iGameLevel > GAMELEVEL_None )
				{
					if( pcUserData->iGameLevel >= GAMELEVEL_Five )
						CHATSERVER->SendChat( USERDATATOUSER( pcUserData ), CHATCOLOR_Error, "> You are Admin Developer (O_o)!" );
					else
						CHATSERVER->SendChatEx( USERDATATOUSER( pcUserData ), CHATCOLOR_Error, "> GM Level Sync [%d] Activated!", pcUserData->iGameLevel );

					if ( (pcUserData->iGameLevel >= GAMELEVEL_Three) && (((PacketNetGetGameMaster *)pPacket)->bFirst) )
					{
						pcUserData->bCloak = TRUE;
						CHATSERVER->SendChat( USERDATATOUSER( pcUserData ), CHATCOLOR_Error, "> Hidden (GM)!" );
					}
				}
			}
			break;

		case PKTHDR_NetQuestT5Data:
			QUESTSERVER->HandlePacket( (PacketNetT5QuestData*)pPacket );
			break;

		case PKTHDR_NetQuestUpdateData:
			QUESTSERVER->HandlePacket( USERDATATOUSER( USERDATABYID( ((PacketNetQuestUpdateData*)pPacket)->iID ) ), (PacketNetQuestUpdateData*)pPacket );
			break;

		case PKTHDR_NetPvPBuff:
			ReceivePvPBuff( (PacketNetPvPBuff*)pPacket );
			break;

		case PKTHDR_NetBellatraRequest:
			BELLATRASERVERHANDLER->HandleNetPacket( (PacketNetBellatraRequest*)pPacket );
			break;

		case PKTHDR_NetActionFieldRequest:
			ACTIONFIELDHANDLER->HandleNetPacket( (PacketNetActionFieldRequest *)pPacket );
			break;

        case PKTHDR_NetUpdateEventMessage:
            EVENTSERVER->UpdateEventMessages();
            break;

		case PKTHDR_NetUpdatePvPGameServer:
			HONORHANDLER->NetHandlePacket( (PacketNetUpdatePvPBellatraBuff *)pPacket );
			break;

		case PKTHDR_NetQuestEXPFreeEvent:
			bFreeExpEvent = ((PacketNetQuestEXPFree *)pPacket)->bActive;
			iExpFreeExp = ((PacketNetQuestEXPFree *)pPacket)->iExpEvent;
			break;

		default:
			break;
	}
}

NetConnection * NetServer::GetNetConnectionBySocketData( class SocketData * sd )
{
	for( std::vector<NetConnection*>::iterator it = vNetConnections.begin(); it != vNetConnections.end(); it++ )
	{
		NetConnection * nc = *it;

		if( nc->pcSocketData == sd )
			return nc;
	}

	return NULL;
}


void NetServer::OnReceivePacket( UserData * pcUserData, void * pPacket )
{
	UserData * pcUserD = pcUserData;
	int iPacketID = ((Packet*)pPacket)->iHeader;

	if ( (iPacketID & 0xFF000000) == 0x7F000000 )
	{
		if ( LOGIN_SERVER )
			OnReceiveServer( pcUserD, pPacket );
		else
			OnReceiveClient( pcUserD, pPacket );
	}
}

void NetServer::Init()
{
	if ( !bInit )
	{
		if ( SERVER_PORT == 10004 )
		{
			
		}
		else
		{
			
		}

		if ( LOGIN_SERVER != GAME_SERVER )
			ConnectAll();
		bInit = TRUE;

		bReceivedBack = TRUE;
	}
}

void NetServer::Tick()
{
	if ( GAME_SERVER )
	{
		SYSTEMTIME * ps = GetServerTime();

		if ( wTimeMinute != ps->wMinute )
		{
			PacketNetUsersOnline s;
			s.iLength = sizeof( PacketNetUsersOnline );
			s.iHeader = PKTHDR_NetUsersOnline;
			s.iID = SERVER_CODE - 1;
			s.iUsersOnline = (*(DWORD*)0x07AC9BC8);
			
			NetConnection * nc = GetLoginServerConnection();
			if ( nc )
				SendPacket( nc->pcSocketData, &s );

			wTimeMinute = ps->wMinute;
		}
	}
}
