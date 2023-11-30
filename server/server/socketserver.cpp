#include "stdafx.h"
#include "SocketServer.h"

SocketServer * SocketServer::pcInstance = NULL;

UINT SocketServer::uBytePacketFlow;

CRITICAL_SECTION                                        sSectionAccept;
CMutex                            * pcSocketCloseMutex = NULL;


SocketServer::SocketServer()
{
	ZeroMemory( pcaReconnectedUsers, sizeof( User * ) * _countof( pcaReconnectedUsers ) );
	sListenSocket = INVALID_SOCKET;
	iMaxConnections = 0;
	pcMutex = new CMutex();

	MAX_PACKETBYTESFLOW = (65535 * 32);

	pcSocketServerHandler = new CServerSocketHandler();
}


SocketServer::~SocketServer()
{
	CloseHandle( hEventSocket );

	DELET( pcSocketServerHandler );

	for ( auto pc : vIocpClients )
		DELET( pc );

	vIocpClients.clear();
}

int SocketServer::GetNumFreeSlots()
{
	int iFreeSlots = 0;

	for ( int i = 0; i < iMaxConnections; i++ )
	{
		SocketData * sd = pcaSocketData + i;

		if ( !sd->bInUse )
			iFreeSlots++;
	}

	return iFreeSlots;
}

SocketData * SocketServer::GetFreeSocketData()
{
	SocketData * r = NULL;

	for ( int i = 0; i < iMaxConnections; i++ )
	{
		SocketData * sd = pcaSocketData + i;

		if ( !sd->bInUse )
		{
			sd->Init();

			r = sd;
			break;
		}
	}

	return r;
}

SocketData * SocketServer::GetSocketData( const char * pszIP, int iPort )
{
	for ( int i = 0; i < iMaxConnections; i++ )
	{
		SocketData * sd = pcaSocketData + i;
		if ( sd->bInUse && sd->bConnected && STRINGCOMPARE( sd->szIP, pszIP ) && sd->iPort == iPort )
			return sd;
	}
	return NULL;
}

SocketData * SocketServer::GetSocketData( SOCKET s )
{
	for ( int i = 0; i < iMaxConnections; i++ )
	{
		SocketData * sd = pcaSocketData + i;
		if ( sd->bInUse && sd->bConnected && s == sd->s )
			return sd;
	}

	return nullptr;
}

SocketData * SocketServer::GetSocketDataByID( INT64 iID )
{
	for ( int i = 0; i < iMaxConnections; i++ )
	{
		SocketData * sd = pcaSocketData + i;
		if ( sd->bInUse && sd->bConnected && iID == sd->iConnectionID )
			return sd;
	}

	return nullptr;
}

CIocpGameHandler * SocketServer::GetIocpHandler( SocketData * sd )
{
	for ( auto pc : vIocpClients )
		if ( pc->pcSocketData == sd )
			return pc;

	return nullptr;
}

void SocketServer::AddIocpHandler( SocketData * sd )
{
	CIocpGameHandler * pcIocp = new CIocpGameHandler();

	pcIocp->Init( sd );

	vIocpClients.push_back( pcIocp );
}

void SocketServer::RemoveIocpHandler( SocketData * sd )
{
	for ( auto it = vIocpClients.begin(); it != vIocpClients.end(); )
	{
		auto pc = (*it);
		if ( pc->pcSocketData == sd )
		{
			pc->UnInit();

			DELET( pc );

			it = vIocpClients.erase( it );
		}
		else
			it++;
	}
}


int SocketServer::GetConnectionCount( UINT lIP )
{
	int iRet = 0;

	for ( int i = 0; i < iMaxConnections; i++ )
	{
		SocketData * sd = pcaSocketData + i;
		if ( sd->bInUse && sd->bKeepAlive == FALSE && sd->lIP == lIP )
		{
			iRet++;
			if ( sd->u && sd->u->GetGameLevel() >= GAMELEVEL_Two )
				iRet--;
		}
	}

	return iRet;
}

void SocketServer::DisconnectIP( UINT lIP )
{
	if ( pcaSocketData )
	{
		for ( int i = 0; i < iMaxConnections; i++ )
		{
			SocketData * sd = pcaSocketData + i;
			if ( sd->bInUse && sd->bConnected && sd->bKeepAlive == FALSE && sd->lIP == lIP )
				WNDPROC_SOCKETCLOSE( hWnd, sd );
		}
	}
}

SocketData * SocketServer::Connect( const char * pszIP, int iPort )
{
	auto ConnectServer = [this]( SocketData * pcSocketData, const char * _pszIP, int _iPort )-> bool
	{
		bool bRet = false;

		SOCKETSERVER->AddIocpHandler( pcSocketData );

		//Remove comment to back
		//bRet = sd->Connect( pszIP, iPort );
		if ( auto pc = SOCKETSERVER->GetIocpHandler( pcSocketData ) )
		{
			if ( bRet = pc->pcClient->Connect( _pszIP, _iPort ) )
			{
				sockaddr_in sSockAddr;
				ZeroMemory( &sSockAddr, sizeof( sockaddr_in ) );
				sSockAddr.sin_family = AF_INET;
				sSockAddr.sin_addr.s_addr = IOCP::CIocpUtils::StringIPToNumberIPV4( _pszIP );
				sSockAddr.sin_port = htons( (WORD)_iPort );
				pcSocketData->Open( pc->pcClient->cIocpData.sSocket, &sSockAddr );
			}
			else
				SOCKETSERVER->RemoveIocpHandler( pcSocketData );
		}

		return bRet;
	};

	WRITEERR( "Connect %s:%d", pszIP, iPort );

	SocketData * sd = NULL;

	if ( (sd = GetInstance()->GetSocketData( pszIP, iPort )) == NULL )
	{
		sd = GetInstance()->GetFreeSocketData();

		if ( sd )
		{
			if ( !ConnectServer( sd, pszIP, iPort ) )
			{
				sd->UnInit();

				return NULL;
			}

			if ( (sd->u = USERSERVER->GetFreeUser()) != NULL )
			{
				sd->ud = sd->u->pcUserData;

				sd->u->Start( sd );
			}
		}
	}

	return sd;
}


void SocketServer::SocketAccept( SOCKET s, SocketServerAccept * psAccept )
{
	SERVER_MUTEX->Lock( 5000 );

	SocketData * sd = NULL;
	BOOL bAccepted = FALSE;

	if ( IsFull() == FALSE )
	{
		BOOL bPass = (USERS_ONLINE < MAX_USERS) ? TRUE : FALSE;

		if ( bPass )
		{
			if ( (sd = GetFreeSocketData()) != NULL )
			{
				sd->u = NULL;
				sd->ud = NULL;

				if ( (sd->u = USERSERVER->GetFreeUser()) != NULL )
				{
					sd->ud = sd->u->pcUserData;

					sd->u->Start( sd );
					sd->Open( s, &psAccept->sAddr );

					sd->iConnectionID = psAccept->iID;

					bAccepted = TRUE;
				}
			}
		}
	}

	if ( bAccepted )
	{
		//Send the Byte Encryption Set
		//sd->SendKeySet( GetPacketObfuscatorByte() );

		//Update Keys
		{
			BYTE bObfuscator = GetPacketObfuscatorByte();

			for ( int i = 0; i < NUM_ENCKEYS; i++ )
				sd->baKeySet[i] = RandomI( 0, 256 );

			PacketKeySet s;
			s.iLength = sizeof( s );
			s.iEncKeyIndex = RandomI( 0, 256 );
			s.iEncrypted = RandomI( 2, 256 );
			s.iHeader = PKTHDR_KeySet;

			bObfuscator += (s.iEncKeyIndex + s.iEncrypted);

			WRITEDBG( "SendKeySet bObfuscator: %d [%d][%d]", bObfuscator, s.iEncKeyIndex, s.iEncrypted );

			for ( int i = 0; i < NUM_ENCKEYS; i++ )
				s.baKeySet[i] = (sd->baKeySet[i] ^ bObfuscator);

			//WRITEDBG( "First Key[0]: %d", baKeySet[0] );

			SENDPACKETSOCKET( sd, &s );

			sd->bKeySet = true;
		}

		// Send Version
		PacketVersion sPacket;
		sPacket.iLength		= sizeof( PacketVersion );
		sPacket.iHeader		= PKTHDR_Version;
		sPacket.bServerFull = FALSE;
		sPacket.iUnk2		= 0;
		sPacket.iVersion	= (*(int*)0x006E4F5C);
		SENDPACKETSOCKET( sd, &sPacket );
	}
	else
	{
		if ( sd )
		{
			sd->UnInit();

			sd = NULL;
		}

		if ( !AddDeniedSocket( s ) )
			closesocket( s );
	}

	SERVER_MUTEX->Unlock();

	DELET( psAccept );
}

void SocketServer::SocketAcceptUDP( SOCKET s, sockaddr_in * addr, UINT uSocketID, UINT uPacketID )
{
	
}

void SocketServer::SocketPacket( SocketData * sd, PacketReceiving * p )
{
	if ( sd )
	{
		sd->iCountIN++;
		sd->iAmountIN += ((Packet *)p->baPacket)->iLength;

		User * u = sd->u;
		UserData * ud = u->pcUserData;

		if ( (u) && (ud) && (ud->pcSocketData) )
		{
			DWORD dwTickCount;

			if ( ((dwTickCount = UserServer::AllowPacket( u )) != 0) && AllowPacket( sd ) )
			{
				SERVER_MUTEX->Lock( 5100 );

				PACKETSERVER->AnalyzePacket( u, p );

				SERVER_MUTEX->Unlock();
			}
			else
				WNDPROC_SOCKETCLOSE( hWnd, sd );
		}
		else
			WNDPROC_SOCKETCLOSE( hWnd, sd );

		sd->FreePacketReceiving( p );
		sd->dwTimeLastPacketReceived = TICKCOUNT;
	}
	else
	{
		if ( p && p->bDelete )
			DELET( p );
	} 
}

void SocketServer::SocketClose( SocketData * sd )
{
	SERVER_MUTEX->Lock( 5200 );

	if ( sd && sd->bInUse )
	{
		User * u = sd->u;
		UserData * ud = u->pcUserData;

		if ( u && ud )
		{
			if ( !u->bNetServer )
			{
				if ( GAME_SERVER )
					PARTYHANDLER->LeaveParty( u );

				if ( SERVEREVENTHANDLER->CannotSave( u ) == FALSE )
				{
					CharacterFile * cf = sd->u->pcUserData->psCharacterFile;
					if ( cf && ud->szAccountName[0] && ud->szCharacterName[0] && cf->iLength > 0 )
					{
						CharacterData * ch = (CharacterData *)& cf->sCharInfo;
						ch->szName[31] = 0;
						if ( STRINGCOMPAREI( ch->szName, ud->szCharacterName ) )
						{
							CALL_WITH_ARG1( 0x0044D070, (DWORD)ud );
							USERSERVER->InventoryServerCheck( ud, 1 );
							if ( LOGIN_SERVER )
							{
								CHARACTERSERVER->SetCharacterEXPOnDisconnect( ud->szCharacterName, ud->liEXP.QuadPart );
								ch->iCurrentExpLow = ud->liEXP.LowPart;
								ch->iCurrentExpHigh = ud->liEXP.HighPart;
							}

							char * pszCharacterName = ud->szCharacterName;
							DWORD safecheck = 0;
							__asm
							{
								PUSH cf;
								PUSH pszCharacterName;
								PUSH ud;
								MOV EAX, 0x0044EED0;
								CALL EAX; //Save?
								ADD ESP, 0x0C;
								MOV safecheck, EAX;
							}
							if ( safecheck != TRUE )
								WRITEERR( "ERR: Save Failed %s", ud->szCharacterName );

							cf->iLength = 0;
						}
					}
				}


				CALLT( 0x00576040, (DWORD)ud );
				CALL_WITH_ARG1( 0x00559000, (DWORD)ud );

				if ( ud->pcWarehouse )
				{
					WAREHOUSEHANDLER->Close( ud->pcWarehouse );
					ud->pcWarehouse = NULL;
				}
				if ( ud->psUnknown1 )
					DELET( ud->psUnknown1 );
			}

			LOGSERVER->LogSocketClose( sd, "SocketServer::SocketClose()", _ReturnAddress() );

			if ( sd )
				SOCKETSERVER->AddDisconnectedUser( sd->lIP );

			if ( NETSERVER->Disconnected( sd ) )
				WRITEDBG( "NET SERVER DC" );
			
			if ( GAME_SERVER )
			{
				ACTIONFIELDHANDLER->OnDisconnectedUser( u );
				QUESTARENAHANDLER->OnDisconnected( u );
				FOLLOWUNITHANDLER->OnDisconnected( u );
			}

			SERVEREVENTHANDLER->OnDisconnected( u );

			DROPAREAHANDLER->OnDisconnected( u );

			u->Stop();
			u->UnInit();
		}
		CServerSocketHandler::Disconnect( sd );

		if ( IOCP_SERVER_TYPE == FALSE )
			sd->Close();

		sd->UnInit();
	}

	SERVER_MUTEX->Unlock();
}

void SocketServer::Load()
{
	iMaxConnections = 350;

	if ( SERVER_PORT == 10004 || SERVER_PORT == 10008 )
	{
		iMaxConnections = 15;
	}
	pcaSocketData = new SocketData[iMaxConnections];

	hEventSocket = CreateEvent( NULL, FALSE, FALSE, NULL );
	for ( int i = 0; i < MAX_DENYCONNECTIONS; i++ )
		saSocket[i] = INVALID_SOCKET;
	
	iNextSocket = 0;

	for ( int i = 0; i < iMaxConnections; i++ )
	{
		SocketData * sd = pcaSocketData + i;
		sd->pcThis = sd;
		sd->bInUse = FALSE;

		if ( IOCP_SERVER_TYPE )
			sd->Prepare( 1, 1 );
		else
		{
			sd->Prepare( 128, 128 );
			sd->hReceiveThread = CreateThread( NULL, 16384, (LPTHREAD_START_ROUTINE)& SocketServer::Receiver, sd, STACK_SIZE_PARAM_IS_A_RESERVATION, NULL );
			sd->hSendThread = CreateThread( NULL, 16384, (LPTHREAD_START_ROUTINE)& SocketServer::Sender, sd, STACK_SIZE_PARAM_IS_A_RESERVATION, NULL );
		}
	}
	bActive = TRUE;
}

DWORD WINAPI SocketServer::Sender( SocketData * sd )
{
	SocketServer * m = GetInstance();

	HWND hWnd = m->GetHWND();

	do
	{
		WaitForSingleObject( sd->hSendThreadSignal, INFINITE );

		if ( !m->IsActive() )
			break;

		PacketSending * p;
		while ( (p = sd->NextPacketSending()) != NULL )
		{
			//Post Packet Message
			if ( !sd->Send( p ) )
			{
				sd->FreePacketSending( p );
				break;
			}
			sd->FreePacketSending( p );
		}

	}
	while ( m->IsActive() );

	return TRUE;
}

DWORD WINAPI SocketServer::Receiver( SocketData * sd )
{
	SocketServer * m = GetInstance();

	HWND hWnd = m->GetHWND();

	do
	{
		WaitForSingleObject( sd->hReceiveThreadSignal, INFINITE );

		if ( !m->IsActive() )
			break;

		PacketReceiving * p;
		while ( (p = sd->ReceivePacket()) != NULL )
		{
			PostMessageA( GetInstance()->GetHWND(), WM_SOCKETPACKET, (WPARAM)sd, (LPARAM)p );
		}

		sd->bBlockSend = TRUE;

		if ( m->IsActive() )
		{
			if ( sd->bConnected )
			{
				long lIP = 0;
				//Anti-DDoS
				if ( sd->iRecvEC == 0x6A6A )
				{
					lIP = sd->lIP;
				}
				else
				{
					static char szLogDC[64] = { 0 };
					STRINGFORMAT( szLogDC, "SocketServer::Receiver() [%d]", sd->iRecvEC );
					LOGSERVER->LogSocketClose( sd, szLogDC, _ReturnAddress() );
				}
				WNDPROC_SOCKETCLOSE( m->GetHWND(), sd );

				if ( lIP != 0 )
					m->AddBanIP( lIP );
			}
		}
	}
	while ( m->IsActive() );

	return TRUE;
}

DWORD WINAPI SocketServer::Listener( SOCKET * ls )
{
	SOCKET s = *ls;
	SocketServer * pc = GetInstance();

	sockaddr_in * sout = new sockaddr_in;
	int len = sizeof( sockaddr_in );

	INT64 iConnID = 1;

	while ( pc->IsActive() )
	{
		SOCKET n = INVALID_SOCKET;
		if ( (n = accept( s, (SOCKADDR *)sout, &len )) != INVALID_SOCKET )
		{
			//WRITEERR( "%s", NumberIPToStringIPV4( sout->sin_addr.s_addr ).c_str() );

			BOOL bContinue = TRUE;
			if ( pc->GetConnectionCount( sout->sin_addr.s_addr ) >= 20 )
			{
				closesocket( n );
				bContinue = FALSE;
			}

			//If GameServer, check if its allowed to login


			if ( bContinue )
			{
				auto psAccept = new SocketServerAccept;
				CopyMemory( &psAccept->sAddr, sout, sizeof( sockaddr_in ) );
				psAccept->iID = iConnID++;

				//Send Accept Message
				SendMessageA( GetInstance()->GetHWND(), WM_SOCKETACCEPT, n, (LPARAM)psAccept );
			}
		}
	}
	delete sout;

	return TRUE;
}

DWORD SocketServer::DeniedSocketsHandler()
{
	SocketServer * m = GetInstance();

	do
	{
		m->HandleBanFirewall();

		m->HandleReconnectedUsers();

		m->HandleDeniedSockets();

		WaitForSingleObject( m->GetHandleEventSocket(), 2500 );
	}
	while ( m->IsActive() );

	return 0;
}

void SocketServer::HandleDeniedSockets()
{
	SOCKET arr[MAX_DENYCONNECTIONS];
	int iNumSockets = 0;

	pcMutex->Lock( 6100 );

	for ( int i = 0; i < iNextSocket; i++ )
	{
		SOCKET s = saSocket[i];

		if ( s != INVALID_SOCKET )
		{
			arr[iNumSockets] = s;
			iNumSockets++;
		}

		saSocket[i] = INVALID_SOCKET;
	}
	iNextSocket = 0;

	pcMutex->Unlock();

	for ( int i = 0; i < iNumSockets; i++ )
	{
		SOCKET s = arr[i];

		if ( s != INVALID_SOCKET )
		{
			LINGER l;
			l.l_onoff = TRUE;
			l.l_linger = 30;
			setsockopt( s, SOL_SOCKET, SO_LINGER, (char*)&l, sizeof( l ) );

			// Send Version
			PacketVersion sPacket;
			sPacket.iLength = sizeof( PacketVersion );
			sPacket.iHeader = PKTHDR_Version;
			sPacket.bServerFull = TRUE;
			sPacket.iUnk2 = 0;
			sPacket.iVersion = (*(int*)0x006E4F5C);

			send( s, (char*)&sPacket, sPacket.iLength, 0 );
			closesocket( s );
		}
	}
}

void SocketServer::HandleBanFirewall()
{
	pcMutex->Lock( 6101 );

	char szIP[32] = { 0 };

	for ( int i = 0; i < MAX_BANCONNECTIONS; i++ )
	{
		if ( dwaIPListBan[i] != 0 )
		{
			DWORD dwIP = dwaIPListBan[i];
			STRINGFORMAT( szIP, "%d.%d.%d.%d", dwIP & 0xFF, (dwIP >> 8) & 0xFF, (dwIP >> 16) & 0xFF, (dwIP >> 24) & 0xFF );
			LOGSERVER->AddFirewallBan( szIP );
			dwaIPListBan[i] = 0;
		}
	}

	pcMutex->Unlock();
}

void SocketServer::HandleReconnectedUsers()
{
	for ( int i = 0; i < MAX_RECONNECTCONNECTIONS; i++ )
	{
		if ( pcaReconnectedUsers[i] )
		{
			SENDPACKETBLANK( pcaReconnectedUsers[i], LOGIN_SERVER ? PKTHDR_ReconnectLogin : PKTHDR_Reconnect );
			pcaReconnectedUsers[i] = NULL;
		}
	}
}

void SocketServer::HandleConnectionCount()
{
	if ( pcaSocketData )
	{
		const int MAX_ACCEPTED = 20;
		for ( int i = 0; i < iMaxConnections; i++ )
		{
			SocketData * sd = pcaSocketData + i;
			if ( sd->bInUse && sd->bConnected && sd->bKeepAlive == FALSE )
			{
				int iConn = 0;
				if ( (iConn = GetConnectionCount( sd->lIP )) >= MAX_ACCEPTED )
				{
					DisconnectIP( sd->lIP );
					WRITEERR( "Ban IP: [%d][%d]", sd->lIP, iConn );
				}
			}
		}
	}
}

BOOL SocketServer::AllowPacket( SocketData * sd )
{
	return TRUE;

	BOOL bRet = TRUE;

	//Flood protection
	if ( sd->iAmountIN > MAX_PACKETBYTESFLOW )
		bRet = FALSE;

	if ( (TICKCOUNT - sd->dwTimeIN) > 1000 )
	{
		sd->iAmountIN = 0;
		sd->dwTimeIN = TICKCOUNT;
	}

	if ( sd->bKeepAlive )
		bRet = TRUE;

	return bRet;
}

BOOL SocketServer::AddDeniedSocket( SOCKET s )
{
	BOOL bRet = FALSE;

	pcMutex->Lock( 6000 );

	if ( iNextSocket < MAX_DENYCONNECTIONS )
	{
		saSocket[iNextSocket] = s;
		iNextSocket++;

		bRet = TRUE;
	}

	pcMutex->Unlock();

	return bRet;
}

BOOL SocketServer::AddBanIP( DWORD dwIP )
{
	BOOL bRet = FALSE;

	pcMutex->Lock( 6101 );

	int iIndex = -1;

	for ( int i = 0; i < MAX_BANCONNECTIONS; i++ )
	{
		if ( dwaIPListBan[i] != 0 )
		{
			if ( dwaIPListBan[i] == dwIP )
			{
				iIndex = -1;
				bRet = TRUE;
				break;
			}
		}
		else
			iIndex = i;
	}

	if ( iIndex >= 0 )
		dwaIPListBan[iIndex] = dwIP;


	char szIP[32] = { 0 };
	WRITEDBG( "Add IP Ban: %%d.%d.%d.%d", dwIP & 0xFF, (dwIP >> 8) & 0xFF, (dwIP >> 16) & 0xFF, (dwIP >> 24) & 0xFF );

	pcMutex->Unlock();

	return bRet;
}

BOOL SocketServer::IsBannedIP( UINT lIP )
{
	BOOL bRet = FALSE;

	pcMutex->Lock( 6101 );

	for ( int i = 0; i < MAX_BANCONNECTIONS; i++ )
	{
		if ( dwaIPListBan[i] != 0 )
		{
			if ( dwaIPListBan[i] == lIP )
			{
				bRet = TRUE;
				break;
			}
		}
	}

	pcMutex->Unlock();

	return bRet;
}

BOOL SocketServer::AddDisconnectedUser( UINT lIP )
{
	BOOL bFound = FALSE;
	for ( int i = 0; i < MAX_DISCONNECT_CONNECTIONS; i++ )
	{
		if ( saDisconnectUser[i].lIP && saDisconnectUser[i].lIP == lIP )
		{
			saDisconnectUser[i].iDisconnectTimes++;
			if ( (saDisconnectUser[i].dwTime + MAX_DISCONNECT_TIME) >= TICKCOUNT )
			{
				if ( saDisconnectUser[i].iDisconnectTimes >= MAX_DISCONNECT_TICK )
				{
					AddBanIP( lIP );
					ZeroMemory( &saDisconnectUser[i], sizeof( DisconnectedUser ) );
				}
			}
			else
				saDisconnectUser[i].iDisconnectTimes = 1;

			bFound = TRUE;

			break;
		}
	}

	if ( bFound == FALSE )
	{
		for ( int i = 0; i < MAX_DISCONNECT_CONNECTIONS; i++ )
		{
			if ( (saDisconnectUser[i].lIP == 0) || ((saDisconnectUser[i].dwTime + MAX_DISCONNECT_TIME) < TICKCOUNT) )
			{
				saDisconnectUser[i].lIP = lIP;
				saDisconnectUser[i].dwTime = TICKCOUNT;
				saDisconnectUser[i].iDisconnectTimes = 1;
				break;
			}
		}
	}

	return TRUE;
}

BOOL SocketServer::AddReconnectedUser( User * pcUser )
{
	for ( int i = 0; i < MAX_RECONNECTCONNECTIONS; i++ )
	{
		if ( pcaReconnectedUsers[i] == NULL )
		{
			pcaReconnectedUsers[i] = pcUser;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL SocketServer::Init( HWND hwnd )
{
	hWnd = hwnd;

	SetPacketObfuscatorByte( SERVER_PORT );

	if ( IOCP_SERVER_TYPE )
	{
		pcSocketServerHandler->Init();

		Load();
	}
	else
		Listen( SERVER_PORT );

	return TRUE;
}

void SocketServer::Listen( int iPort )
{
	int iStatus = 0;
	// Start
	if ( (iStatus = WSAStartup( MAKEWORD( 2, 2 ), &sWsaData )) != 0 )
	{
		WRITEDBG( "SocketServer::Init()WSAStartup():: %d", iStatus );
		return;
	}

	if ( (sListenSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP )) != INVALID_SOCKET )
	{
		sockaddr_in sin;
		sin.sin_family = AF_INET;
		sin.sin_addr.s_addr = INADDR_ANY;
		sin.sin_port = htons( iPort );

		if ( ::bind( sListenSocket, (SOCKADDR *)&sin, sizeof( sin ) ) != SOCKET_ERROR )
		{
			if ( listen( sListenSocket, SOMAXCONN ) == SOCKET_ERROR )
			{
				WRITEDBG( "SocketServer::Init()listen():: %d", WSAGetLastError() );
				closesocket( sListenSocket );
				sListenSocket = INVALID_SOCKET;
				return;
			}
		}
		else
		{
			WRITEDBG( "SocketServer::Init()bind():: %d", WSAGetLastError() );
			closesocket( sListenSocket );
			sListenSocket = INVALID_SOCKET;
			return;
		}
	}
	else
	{
		WRITEDBG( "SocketServer::Init()socket():: %d", WSAGetLastError() );
		return;
	}

	bActive = TRUE;

	InitializeCriticalSection( &sSectionAccept );


	// Success
	hListener = CreateThread( NULL, 16384, (LPTHREAD_START_ROUTINE)Listener, &sListenSocket, STACK_SIZE_PARAM_IS_A_RESERVATION, NULL );
	CreateThread( NULL, 16384, (LPTHREAD_START_ROUTINE)DeniedSocketsHandler, NULL, STACK_SIZE_PARAM_IS_A_RESERVATION, NULL );
	Load();
}


void SocketServer::OnReadHandler( SOCKET s, DWORD dwParam )
{
	// Find Socket
	SocketData * sd = NULL;
	for ( int i = 0; i < iMaxConnections; i++ )
	{
		SocketData * p = pcaSocketData + i;
		if ( p->bInUse && p->s == s )
		{
			sd = p;
			break;
		}
	}

	if ( sd )
	{
		switch ( dwParam )
		{
			case FD_READ:
				//    SetEvent( sd->hReceiveThreadSignal );
				break;

			case FD_CLOSE:
				WNDPROC_SOCKETCLOSE( SOCKETSERVER->GetHWND(), sd );
				break;

			default:
				break;
		}
	}
}

SocketData * SocketServer::GetClientSocket( UINT uSocketID, UINT uPacketID )
{

	return nullptr;
}
