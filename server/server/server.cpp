#include "stdafx.h"
#include "server.h"

#include "CServer.h"

BOOL g_bLoginServer = FALSE;
BOOL g_bGameServer = FALSE;

Server * Server::pcInstance = NULL;



void Server::CreateSQLConnection( EDatabaseID eID )
{
	SQLConnection * pcSQL = new SQLConnection;
	pcSQL->Init( eID );
}

Server::Server()
{
	GetLocalTime( &sLocalTime );

	iServers = 0;
	ZeroMemory( &sPacketServerList, sizeof( sPacketServerList ) );

	bMaintenanceCountdown = false;
	iMaintenanceCountdown = 0;

	InitializeCriticalSection( (CRITICAL_SECTION *)0x07AC3E7C );
	pcMutex = new CMutex( "Server", (CRITICAL_SECTION *)0x07AC3E7C );

	SocketServer::CreateInstance();
	pcSocketServer			= SocketServer::GetInstance();
	pcPacketServer			= new PacketServer();
	pcAccountServer			= new AccountServer();
	pcSQLServerConn			= new SQLConnection();
	pcCharacterServer		= new CharacterServer();
	pcChatServer			= new ChatServer();
	pcServerCommand			= new ServerCommand();
	pcUnitServer			= new UnitServer();
	pcUserServer			= new UserServer();
	pcSQLSkill				= new CSQLSkill();
	pcNetServer				= new NetServer();
	pcItemServer			= new ItemServer();
	pcLogServer				= new LogServer();
	pcCheatServer			= new CheatServer();
	pcMapServer				= new MapServer();
	pcQuestServer			= new QuestServer();
	pcUnitInfoServer		= new UnitInfoServer();
	pcBellatraServer		= new BellatraServer();
	pcBlessCastleServer		= new BlessCastleServer();
	pcPvPServer				= new PvPServer();
}

Server::~Server()
{
	DELET( pcPacketServer );
	DELET( pcAccountServer );
	DELET( pcSQLServerConn );
	DELET( pcCharacterServer );
	DELET( pcChatServer );
	DELET( pcServerCommand );
	DELET( pcUnitServer );
	DELET( pcUserServer );
	DELET( pcSQLSkill );
	DELET( pcNetServer );
	DELET( pcItemServer );
	DELET( pcLogServer );
	DELET( pcCheatServer );
	DELET( pcMapServer );
	DELET( pcQuestServer );
	DELET( pcUnitInfoServer );
	DELET( pcBellatraServer );
	DELET( pcBlessCastleServer );
	DELET( pcPvPServer );
	DELET( pcSocketServer );

	DELET( pcMutex );
}

void Server::Init( HWND hWnd )
{
	this->hWnd = hWnd;

	ShowWindow( hWnd, SW_SHOW );
	UpdateWindow( hWnd );
}

void Server::UnInit()
{
}

void Server::Load()
{
	iGameVersion = SERVERCONFIG->GetVersionGame();
	iServerVersion = SERVERCONFIG->GetVersionServer();

	iServers = 1;

	int iID = 0;



	CConfigFileReader * pcConfigFileReader = new CConfigFileReader( "server.ini" );
	if ( pcConfigFileReader->Open() )
	{
		iServers = pcConfigFileReader->ReadInt( "Server", "GameServers" ) + 1;

		iID = pcConfigFileReader->ReadInt( pcConfigFileReader->ReadString( "Server", "Type" ), "ID" );
		SERVER_CODE = iID;

		if ( iID >= 0 )
		{
			LoadServerInfo( pcConfigFileReader, saServerInfo + 0, SERVERTYPE_Login, "LoginServer" );

			for ( int i = 1; i < iServers; i++ )
			{
				char szGameServer[32];
				StringCbPrintfA( szGameServer, _countof( szGameServer ), "GameServer%d", i );

				LoadServerInfo( pcConfigFileReader, saServerInfo + i, SERVERTYPE_Game, szGameServer );
			}
		}
		else
		{
			iID = 0;
			SERVER_CODE = 0;

			LoadServerInfo( pcConfigFileReader, saServerInfo + 0, SERVERTYPE_Multi, "LoginServer" );

			for ( int i = 1; i < iServers; i++ )
			{
				char szGameServer[32];
				StringCbPrintfA( szGameServer, _countof( szGameServer ), "GameServer%d", i );

				LoadServerInfo( pcConfigFileReader, saServerInfo + i, SERVERTYPE_Game, szGameServer );
			}
		}

		for ( int i = 0; i < iServers; i++ )
		{
			ServerInfo * p = saServerInfo + i;

			if ( i != iID )
				NETSERVER->AddConnection( p->szNetIP, p->iPort, i );
		}

		pcConfigFileReader->Close();
	}
	DELET( pcConfigFileReader );

	psServerInfo = saServerInfo + iID;
	iServerType = psServerInfo->iType;

	switch ( iServerType )
	{
		case SERVERTYPE_Login:
			g_bLoginServer = TRUE;
			g_bGameServer = FALSE;
			break;
		case SERVERTYPE_Game:
			g_bLoginServer = FALSE;
			g_bGameServer = TRUE;
			break;
		case SERVERTYPE_Multi:
			g_bLoginServer = TRUE;
			g_bGameServer = TRUE;
			break;
	}

	CreateSQLConnection( DATABASEID_GameDB );
	CreateSQLConnection( DATABASEID_UserDB );
	CreateSQLConnection( DATABASEID_ServerDB );
	CreateSQLConnection( DATABASEID_LogDB );
	CreateSQLConnection( DATABASEID_SkillDB );
	CreateSQLConnection( DATABASEID_EventDB );
	CreateSQLConnection( DATABASEID_ClanDB );
	CreateSQLConnection( DATABASEID_SkillDBNew );

	LoadDirty();

	(*(BOOL*)0x0084585C) = GAME_SERVER;
	(*(BOOL*)0x07AC9D3C) = GAME_SERVER;

	SERVER_VERSION_MIN = iGameVersion;
	SERVER_VERSION_MAX = iGameVersion;
	SERVER_VERSION = iServerVersion;
	MAX_USERS = psServerInfo->iMaxUsers;

	CALL( 0x0056B960 ); //Load All
	CALL( 0x0056BA90 );

	SOCKETSERVER->Init( hWnd );
	
	if ( GAME_SERVER )
		NETSERVER->Init();
	
	BLESSCASTLESERVER->SQLReadBlessCastleStatus();

	BELLATRASERVER->Init();

    PVPSERVER->Init();


	AccountServer::SQLUserOnline( "", "", "", 0, USERSONLINE_DeleteAll, 0, 0 );
}

void Server::UnLoad()
{
	AccountServer::SQLUserOnline( "", "", "", 0, USERSONLINE_DeleteAll, 0, 0 );
/*
	ServerNet::GetInstance()->DisconnectAll( true );

	SocketServer::GetInstance()->UnLoad();
	BlessCastleServer::GetInstance()->UnLoad();
	MapServer::GetInstance()->UnLoad();
	UserServer::GetInstance()->UnLoad();
	SQLManager::GetInstance()->UnLoad();
*/
	UnLoadDirty();
}

void Server::Begin()
{
	ShowWindow( hWnd, SW_SHOW );
	UpdateWindow( hWnd );

	InvalidateRect( hWnd, NULL, TRUE );
	UpdateWindow( hWnd );

	if ( LOGIN_SERVER )
	{
//		CreateThread( NULL, KB256, (LPTHREAD_START_ROUTINE)BellatraServer::ProcessBellatraScores, NULL, STACK_SIZE_PARAM_IS_A_RESERVATION, NULL );
//		CreateThread( NULL, KB256, (LPTHREAD_START_ROUTINE)ChatServer::ProcessGlobalTradeChats, NULL, STACK_SIZE_PARAM_IS_A_RESERVATION, NULL );
		CreateThread( NULL, KB256, (LPTHREAD_START_ROUTINE)AccountServer::ProcessAccountLoginQueue, NULL, STACK_SIZE_PARAM_IS_A_RESERVATION, NULL );
	}
}

void Server::End()
{
	InvalidateRect( hWnd, NULL, TRUE );
	UpdateWindow( hWnd );

	ShowWindow( hWnd, SW_HIDE );
	UpdateWindow( hWnd );
}

void Server::Tick( ETickType iTickType )
{
	switch ( iTickType )
	{
		case TICKTYPE_500MilliSecond:
			CALL( 0x00560520 );
			_time32( (__time32_t *)0x07AC9D6C );
			break;
		case TICKTYPE_1Second:
			AnnounceMaintenanceCountdown();

			GetLocalTime( &sLocalTime );

			for( UINT i = 0; i < USERSERVER->uUsersInGame; i++ )
			{
				CHARACTERSERVER->UpdateForceOrb( USERSERVER->pcaUserInGame[i]->pcUserData );
				CHARACTERSERVER->UpdatePremiumTimer( USERSERVER->pcaUserInGame[i]->pcUserData );
			}

			PVPSERVER->Update();
			FURYARENAHANDLER->Tick();
			TRADEHANDLER->Tick();
			LOGSERVER->OnLogUsersOnline();

			if( GAME_SERVER )
				ROLLDICEHANDLER->Tick();

			if ( LOGIN_SERVER )
				ServerCommand::SQLRead();

			BELLATRASERVER->UpdateClanBellatra();

			break;
		case TICKTYPE_1Minute:
			NETSERVER->Tick();
			//ShellExecuteA( 0, "runas", "netsh", "advfirewall firewall delete rule name=\"BlockPT\"", NULL, SW_HIDE );
			break;

        case TICKTYPE_1Hour:
            PVPSERVER->Tick( iTickType );
            break;
	}
}

void Server::Loop()
{
	//Update Global TickCount
	TickCount( timeGetTime() );

	//Update Users
	USERSERVER->Update();

	//Update Units
	UNITSERVER->Update();

	//Update Maps
	MAPSERVER->Update();

	//Update Alive Users
	USERSERVER->Loop();
}

void Server::Time( double fTime )
{
	static double fCount_100MS = 0.0f; static double f100MS = 100.0f;
	static double fCount_500MS = 0.0f; static double f500MS = 500.0f;
	static double fCount_1SECO = 0.0f; static double f1SECO = 1000.0f;
	static double fCount_1MINU = 0.0f; static double f1MINU = 60000.0f;
	static double fCount_1HOUR = 0.0f; static double f1HOUR = 3600000.0f;

	fCount_100MS += fTime;
	fCount_500MS += fTime;
	fCount_1SECO += fTime;
	fCount_1MINU += fTime;
	fCount_1HOUR += fTime;

	while ( fCount_100MS >= f100MS )
	{
		SERVERHANDLER->Update( TICKTYPE_100MilliSecond );

		fCount_100MS -= f100MS;
	}

	while ( fCount_500MS >= f500MS )
	{
		Server::GetInstance()->Tick( TICKTYPE_500MilliSecond );

		SERVERHANDLER->Update( TICKTYPE_500MilliSecond );

		fCount_500MS -= f500MS;
	}

	while ( fCount_1SECO >= f1SECO )
	{
		Server::GetInstance()->Tick( TICKTYPE_1Second );

		SERVERHANDLER->Update( TICKTYPE_1Second );

		fCount_1SECO -= f1SECO;
	}

	while ( fCount_1MINU >= f1MINU )
	{
		Server::GetInstance()->Tick( TICKTYPE_1Minute );

		SERVERHANDLER->Update( TICKTYPE_1Minute );

		fCount_1MINU -= f1MINU;
	}

	while ( fCount_1HOUR >= f1HOUR )
	{
		Server::GetInstance()->Tick( TICKTYPE_1Hour );

		SERVERHANDLER->Update( TICKTYPE_1Hour );
			
		fCount_1HOUR -= f1HOUR;
	}
}

void Server::LoadServerInfo( CConfigFileReader * pcConfigFileReader, ServerInfo * si, EServerType eServerType, string strSection )
{
	//Server Type
	si->iType = eServerType;

	//Name
	StringCchCopyA( si->szName, _countof( si->szName ), pcConfigFileReader->ReadString( strSection, "Name" ).c_str() );

	//IP 1
	si->lIP.s_addr = StringIPToNumberIPV4( pcConfigFileReader->ReadString( strSection, "IP" ).c_str() );
	StringCchCopyA( si->szIP, _countof( si->szIP ), NumberIPToStringIPV4( si->lIP.S_un.S_addr ).c_str() );

	//IP 2
	si->lIP2.s_addr = StringIPToNumberIPV4( pcConfigFileReader->ReadString( strSection, "IP" ).c_str() );
	StringCchCopyA( si->szIP2, _countof( si->szIP2 ), NumberIPToStringIPV4( si->lIP2.S_un.S_addr ).c_str() );

	//IP 3
	si->lIP3.s_addr = StringIPToNumberIPV4( pcConfigFileReader->ReadString( strSection, "IP" ).c_str() );
	StringCchCopyA( si->szIP3, _countof( si->szIP3 ), NumberIPToStringIPV4( si->lIP3.S_un.S_addr ).c_str() );

	//IP Net
	si->lNetIP.s_addr = StringIPToNumberIPV4( pcConfigFileReader->ReadString( strSection, "NetIP" ).c_str() );
	StringCchCopyA( si->szNetIP, _countof( si->szNetIP ), NumberIPToStringIPV4( si->lNetIP.S_un.S_addr ).c_str() );

	//Port
	si->iPort = pcConfigFileReader->ReadInt( strSection, "Port" );

	si->iMaxUsers = pcConfigFileReader->ReadInt( strSection, "MaxUsers" );
}

char * Server::GetServerName( int iIndex )
{
	if ( (iIndex >= 0) && (iIndex < MAX_SERVERINFO) )
		return saServerInfo[iIndex].szName;

	return "";
}

int Server::GetServerIndex( char * pszIP, int iPort )
{
	for ( int i = 0; i < MAX_SERVERINFO; i++ )
	{
		ServerInfo * si = saServerInfo + i;

		if ( si->iPort == iPort )
		{
			if ( STRINGCOMPAREI( si->szIP, pszIP ) )
				return i;

			if ( STRINGCOMPAREI( si->szIP2, pszIP ) )
				return i;

			if ( STRINGCOMPAREI( si->szIP3, pszIP ) )
				return i;
		}
	}

	return -1;
}

void Server::StartMaintenanceCountdown( int iSeconds )
{
	bMaintenanceCountdown = true;
	iMaintenanceCountdown = iSeconds;
	dwLastAnnounce = 0;

	AnnounceMaintenanceCountdown();

	SERVER->SetMaintenance( TRUE );
}

void Server::StopMaintenanceCountdown()
{
	bMaintenanceCountdown = false;

	CHATSERVER->SendChatAll( CHATCOLOR_Global, "Maintenance Countdown Cancelled" );

	SERVER->SetMaintenance( FALSE );
}

void Server::AnnounceMaintenanceCountdown()
{
	if ( bMaintenanceCountdown )
	{
		iMaintenanceCountdown--;

		if ( iMaintenanceCountdown > 0 )
		{
			int iMinutes = iMaintenanceCountdown / 60;
			int iSeconds = iMaintenanceCountdown - (iMinutes * 60);

			if ( iMaintenanceCountdown == 10 )
			{
				Packet s;
				s.iLength = sizeof( s );
				s.iHeader = PKTHDR_Save;
				for ( int i = 0; i < PLAYERS_MAX; i++ )
				{
					UserData * pcCur = USERSDATA + i;

					if ( (pcCur->pcSocketData != NULL) && pcCur->iID )
						SENDPACKET( USERDATATOUSER( pcCur ), &s );
				}
			}

			bool bAnnounce = false;
			if ( iMaintenanceCountdown <= 5 )
			{
				bAnnounce = true;
			}
			else if ( iMaintenanceCountdown <= 15 )
			{
				if ( (iMaintenanceCountdown % 2) == 0 )
				{
					bAnnounce = true;
				}
			}
			else if ( iMaintenanceCountdown <= 60 )
			{
				if ( (iMaintenanceCountdown % 10) == 0 )
				{
					bAnnounce = true;
				}
			}
			else if ( iMaintenanceCountdown <= 120 )
			{
				if ( (iMaintenanceCountdown % 20) == 0 )
				{
					bAnnounce = true;
				}
			}
			else
			{
				if ( (iMaintenanceCountdown % 30) == 0 )
				{
					bAnnounce = true;
				}
			}

			if ( bAnnounce )
				CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "Maintenance Countdown> %dmin %dsec", iMinutes, iSeconds );

			if ( iMaintenanceCountdown == 1 )
			{
				for ( int i = 0; i < PLAYERS_MAX; i++ )
				{
					UserData * pcCur = USERSDATA + i;

					if ( (pcCur->pcSocketData != NULL) && (pcCur->iGameLevel < GAMELEVEL_Two) && (pcCur->pcSocketData->bKeepAlive == FALSE) )
						SENDPACKETBLANK( USERDATATOUSER( pcCur ), PKTHDR_Disconnect, TRUE );
				}
			}
		}
		else
		{
			for ( int i = 0; i < PLAYERS_MAX; i++ )
			{
				UserData * pcCur = USERSDATA + i;

				if ( (pcCur->pcSocketData != NULL) && (pcCur->iGameLevel < GAMELEVEL_Two) && (pcCur->pcSocketData->bKeepAlive == FALSE) )
					WNDPROC_SOCKETCLOSE( SOCKETSERVER->GetHWND(), pcCur->pcSocketData );
			}

			bMaintenanceCountdown = false;
		}
	}
}

BOOL Server::SQLGetMaintenace()
{
	BOOL bRet = FALSE;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT IP FROM Maintenance WHERE (Mode=3)" ) )
		{
			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_Integer, &bRet );
			}
		}
		pcDB->Close();
	}

	return bRet;
}

ID Server::GetNextID()
{
	static ID lBeginID = 0x00100000;

	ID lNextID = (lBeginID++);

	if ( GAME_SERVER )
		lNextID += 0x3FF00000;

	return lNextID;
}

void Server::PHServerList( SocketData * pcSocketData, int iTicket, EAccountShare eAccountShare )
{
	if ( sPacketServerList.iHeader != PKTHDR_ServerList )
	{
		sPacketServerList.iLength = sizeof( PacketServerList );
		sPacketServerList.iHeader = PKTHDR_ServerList;

		StringCchCopyA( sPacketServerList.sHeader.szServerName, _countof( sPacketServerList.sHeader.szServerName ), psServerInfo->szName );

		int j = 0;
		for ( int i = 0; (i < iServers) && (j < (_countof( sPacketServerList.sServers ) - 1)); i++ )
		{
			ServerInfo * p = saServerInfo + i;

			if ( p->iType == SERVERTYPE_Game )
			{
				StringCchCopyA( sPacketServerList.sServers[j].szName, _countof( sPacketServerList.sServers[j].szName ), p->szName );

				StringCchCopyA( sPacketServerList.sServers[j].szaIP[0], _countof( sPacketServerList.sServers[j].szaIP[0] ), p->szIP );
				StringCchCopyA( sPacketServerList.sServers[j].szaIP[1], _countof( sPacketServerList.sServers[j].szaIP[1] ), p->szIP2 );
				StringCchCopyA( sPacketServerList.sServers[j].szaIP[2], _countof( sPacketServerList.sServers[j].szaIP[2] ), p->szIP3 );

				sPacketServerList.sServers[j].iaPort[0] = p->iPort;
				sPacketServerList.sServers[j].iaPort[1] = p->iPort;
				sPacketServerList.sServers[j].iaPort[2] = p->iPort;
				sPacketServerList.sServers[j].iaPort[3] = 0;

				j++;
			}
		}

		sPacketServerList.sHeader.iGameServers = j;
		sPacketServerList.sHeader.iClanServerIndex = j;

		StringCchCopyA( sPacketServerList.sServers[j].szName, _countof( sPacketServerList.sServers[j].szName ), "clan" );

		StringCchCopyA( sPacketServerList.sServers[j].szaIP[0], _countof( sPacketServerList.sServers[j].szaIP[0] ), saServerInfo[0].szIP );
		StringCchCopyA( sPacketServerList.sServers[j].szaIP[1], _countof( sPacketServerList.sServers[j].szaIP[1] ), saServerInfo[0].szIP2 );
		StringCchCopyA( sPacketServerList.sServers[j].szaIP[2], _countof( sPacketServerList.sServers[j].szaIP[2] ), saServerInfo[0].szIP3 );

		sPacketServerList.sServers[j].iaPort[0] = 80;
		sPacketServerList.sServers[j].iaPort[1] = saServerInfo[0].iPort;
		sPacketServerList.sServers[j].iaPort[2] = saServerInfo[0].iPort;
		sPacketServerList.sServers[j].iaPort[3] = 0;
	}

	sPacketServerList.sHeader.iTicket = iTicket;
	sPacketServerList.eAccountShare = eAccountShare;

	sPacketServerList.sHeader.dwTime = READDWORD( 0x07AC9D6C );

	SENDPACKETSOCKET( pcSocketData, &sPacketServerList );
}
