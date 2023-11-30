#include "stdafx.h"
#include "SocketGame.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment (lib, "Ws2_32.lib")


SocketGame * SocketGame::pcInstance;

SocketGame::SocketGame()
{
	bActive = false;

	hWnd = NULL;

	pcaSocketData = new SocketData[MAX_CONNECTIONS];

	pcSocketDataL = (SocketData**)0x03A97624;
	pcSocketDataG = (SocketData**)0x03A97620;
	pcSocketDataG2 = (SocketData**)0x03A97628;
	pcSocketDataG3 = (SocketData**)0x03A9762C;

	iDisconnectCode = -1;

	bReservePackets = FALSE;
	bHandlingReservedPackets = FALSE;

}

SocketGame::~SocketGame()
{
	bActive = false;

	hWnd = NULL;

	for ( auto pc : vIocpClients )
		DELET( pc );

	vIocpClients.clear();

	DELETA( pcaSocketData );
}

void SocketGame::Load( HWND hWnd )
{
	this->bActive = true;
	this->hWnd = hWnd;

	int iMaxSendPacket = IOCP_SERVER_TYPE ? 1 : 500;
	int iMaxReceivePacket = IOCP_SERVER_TYPE ? 1 : 500;

	for ( int i = 0; i < MAX_CONNECTIONS; i++ )
	{
		SocketData * sd = pcaSocketData + i;

		sd->pcThis = sd;
		sd->bInUse = false;

		sd->Prepare( iMaxSendPacket, iMaxReceivePacket );

		if ( IOCP_SERVER_TYPE == FALSE )
		{
			sd->hReceiveThread = CAntiDebuggerHandler::CreateHiddenThread( (LPTHREAD_START_ROUTINE)SocketGame::Receiver, sd, KB16 );
			sd->hSendThread = CAntiDebuggerHandler::CreateHiddenThread( (LPTHREAD_START_ROUTINE)SocketGame::Sender, sd, KB16 );
		}
	}

	sPingLogin	= MinMax( 0, 0 );
	sPingGame	= MinMax( 0, 0 );

	bReservePackets = FALSE;
}

void SocketGame::UnLoad()
{
	bReservePackets = FALSE;

	bActive = false;

	for ( int i = 0; i < MAX_CONNECTIONS; i++ )
	{
		SocketData * sd = pcaSocketData + i;

		SOCKETCLOSE( sd );

		SetEvent( sd->hReceiveThreadSignal );
		WaitForSingleObject( sd->hReceiveThread, INFINITE );
		CloseHandle( sd->hReceiveThread );

		SetEvent( sd->hSendThreadSignal );
		SetEvent( sd->hSendEvent );
		WaitForSingleObject( sd->hSendThread, INFINITE );
		CloseHandle( sd->hSendThread );
	}
}

void SocketGame::Loop()
{
	if ( Game::GetGameMode() == GAMEMODE_InGame )
	{
		static bool bFirst = false;
		if ( !bFirst )
		{
			if ( SOCKETL && SOCKETG )
			{
				ZeroMemory( (void*)0x03A46600, 0x8E * 4 );
				WRITEDWORD( 0x03A975F4, 0 );
				WRITEDWORD( 0x03A97648, 0 );
				WRITEDWORD( 0x03A97664, 0 );
				WRITEDWORD( 0x03A3E3BC, READDWORD( 0x00CF4824 ) );
				WRITEDWORD( 0x03A46844, 0 );
				WRITEDWORD( 0x00CF47C0, 0 );
				WRITEDWORD( 0x03A976E4, 0 );
				WRITEDWORD( 0x039C8710, 0 );
				WRITEDWORD( 0x00A1781C, 0 );
				WRITEDWORD( 0x03A975F8, -1 );
				ZeroMemory( (void*)0x039F0308, 0x80 * 4 );
				ZeroMemory( (void*)0x0397A708, 0x6 * 4 );
				ZeroMemory( (void*)0x03A46AA0, 0x6 * 4 );

				UnitData * ud = UNITDATA;
				if ( ud )
				{
					PacketSelectCharacter s;
					s.iLength = sizeof( PacketSelectCharacter );
					s.iHeader = PKTHDR_SelectCharacter;
					s.dwReserved1 = 0;
					s.dwReserved2 = 0;
					s.dwReserved3 = 0;
					STRINGCOPY( s.szName, ud->sCharacterData.szName );
					SENDPACKETL( &s );
				}
			}
			bFirst = true;
		}
		else
		{
			CHARACTERGAME->UpdateCharacterPlayData(); //Update Character... 48470013

			if ( DISCONNECT_TYPE != (-2) )
			{
				if ( SOCKETL == NULL )
				{
					if ( DISCONNECT_TYPE == -1 )
					{
						DISCONNECT_TYPE = 1;
						ChatGame::AddChatBoxTextEx( CHATCOLOR_Error, "Connection lost with %s", "Login Server" );
					}
				}
				else if ( SOCKETG == NULL )
				{
					if ( DISCONNECT_TYPE == -1 )
					{
						DISCONNECT_TYPE = 2;
						ChatGame::AddChatBoxTextEx( CHATCOLOR_Error, "Connection lost with %s", "Game Server" );
					}
				}
			}
		}

		CALL( 0x0061ECF0 );
		CALL( 0x00408DF0 );
	}

}

void SocketGame::Tick( ETickType iTickType )
{
	switch ( iTickType )
	{
		case TICKTYPE_100MilliSecond:
			PingConnections();
			break;
	}
}


void SocketGame::PHPing( PacketPing * psPacket, SocketData * pcSocketData )
{
	pcSocketData->SetPing( TICKCOUNT - psPacket->dwTime );

	int iPing = pcSocketData->GetPing( TICKCOUNT );

	MinMax & sPing = (pcSocketData == SOCKETL) ? PingLogin() : PingGame();

	if ( (iPing < sPing.sMin) || (sPing.sMin == 0) )
		sPing.sMin = iPing;

	if ( (iPing > sPing.sMax) || (sPing.sMax == 0) )
		sPing.sMax = iPing;
}

void SocketGame::PingSocket( SocketData * sd, DWORD dwTime )
{
	sd->bPing = true;
	sd->dwPingTime = dwTime;

	PacketPing p;
	p.iLength = sizeof( p );
	p.iHeader = PKTHDR_Ping;
	p.dwTime = dwTime;
	p.iPing = sd->iPing;

	if ( SOCKETL == sd )
		SENDPACKETL( &p );
	else
		SENDPACKETG( &p );
}


void SocketGame::PingConnections()
{
	static int iCounter = 0;

	iCounter++;

	for ( int i = 0; i < MAX_CONNECTIONS; i++ )
	{
		SocketData * sd = pcaSocketData + i;

		if ( (sd->bInUse) && (sd->bConnected) )
		{
			if ( (sd->bPing == false) )
			{
				if ( (sd->dwTimeLastPacketReceived + 2500) < TICKCOUNT )
				{
					//Keep Alive every 2,5 seconds
					PingSocket( sd, TICKCOUNT );

					MinMax & sPing = (sd == SOCKETL) ? PingLogin() : PingGame();
					sPing.sMin = sPing.sMax = sd->GetPing( TICKCOUNT );
				}
				else if ( iCounter >= 100 )
				{
					//Ping every 10 seconds
					PingSocket( sd, TICKCOUNT );

					MinMax & sPing = (sd == SOCKETL) ? PingLogin() : PingGame();
					sPing.sMin = sPing.sMax = sd->GetPing( TICKCOUNT );
				}
			}
			else
			{
				//Reset Pinging if taking longer than 60 seconds
				if ( sd->GetPing( TICKCOUNT ) > 60000 )
					sd->bPing = false;
			}
		}
	}

	if ( iCounter >= 100 )
		iCounter = 0;
}

char * SocketGame::GetServerTypeName( SocketData * sd, int iOptionalCode )
{
	static char szLoginServer[] = "Login Server";
	static char szGameServer[] = "Game Server";
	static char szUnknown[] = "Unknown";

	if ( (sd == SOCKETL) || (iOptionalCode == 1) )
		return szLoginServer;
	else if ( (sd == SOCKETG) || (iOptionalCode == 2) )
		return szGameServer;

	return szUnknown;
}

BOOL SocketGame::IsReservePackets()
{
	return bReservePackets;
}

void SocketGame::SetReservePackets( BOOL b )
{
	//WRITEDBG( "SetReservePackets( BOOL %s ) (current: %s)", b ? "true" : "false", bReservePackets ? "true" : "false" );

	bReservePackets = b;
}

BOOL SocketGame::IsHandlingReservedPackets()
{
	return bHandlingReservedPackets;
}

void SocketGame::HandlePacket( SocketData * sd, PacketReceiving * p )
{
	sd->iCountIN++;

	if ( (TICKCOUNT - sd->dwTimeIN) > 5000 )
	{
		sd->iCountIN = 0;
		sd->iCountOUTB = 0;
		sd->dwTimeIN = TICKCOUNT;
	}

	Packet * psPacket = (Packet*)p->baPacket;
	int len = psPacket->iLength;
	if ( len > MAX_PKTSIZ )
		return;
	PACKETHANDLER->AnalyzePacket( sd, psPacket );
	/* TEMPORARILY */
	static char * packettemp = NULL;
	if ( packettemp == NULL )
		packettemp = new char[MAX_PKTSIZ + 0x2C];
	char * pointertotmp = packettemp;
	memcpy( packettemp + 0x2C, psPacket, len );
	WRITEDWORD( pointertotmp, sd );
	if ( psPacket->iHeader == PKTHDR_KeySet )
	{
		//    sd->KeyObfuscatorSet( ((PacketSendKey*)&p->sPacket)->bKey );
	}
	else
	{
		__asm
		{
			PUSHAD;
			MOV EAX, packettemp;
			PUSH EAX;
			MOV EAX, 0x00632A50;
			CALL EAX;
			ADD ESP, 4;
			POPAD;
		}
	}
	sd->dwTimeLastPacketReceived = TICKCOUNT;
	sd->FreePacketReceiving( p );

}

void SocketGame::SocketPacket( SocketData * sd, PacketReceiving * p )
{
	if ( bReservePackets )
	{
		//WRITEDBG( "Reserve Packet (%d) (%d) [%d][0x%08X] [%s]", p->bInUse, p->bDelete, ((Packet *)p->baPacket)->iLength, ((Packet *)p->baPacket)->iHeader, sd->szIP );

		//Create new Reserved Packet
		PacketReserved * pr = new PacketReserved( sd, p );

		//Add Reserved Packet to Queue
		vReservedPackets.push_back( pr );
	}
	else
	{
		//Any Reserved Packets?
		if ( vReservedPackets.size() > 0 )
		{
			//WRITEDBG( "Reserved Packets count: %d", vReservedPackets.size() );

			//Set Handling of Reserved Packets is in Progress
			bHandlingReservedPackets = TRUE;

			//Handle each Reserved Packet
			for ( vector<PacketReserved*>::iterator it = vReservedPackets.begin(); it != vReservedPackets.end(); it++ )
			{
				PacketReserved * pr = *it;

				//Handle Reserved Packet
				HandlePacket( pr->pSocketData, pr->pPacketReceiving );

				//Delete Reserved Packet
				delete pr;
			}

			//Clear Reserved Packet Queue
			vReservedPackets.clear();

			//Done Handling Reserved Packets
			bHandlingReservedPackets = FALSE;

			//WRITEDBG( "Reserved Packets cleared" );
		}

		//Handle Packet
		HandlePacket( sd, p );
	}
}

void SocketGame::SetIPPort( const char * pszIP, int iPort )
{
	char * ip = (char*)pszIP;
	if ( (pszIP[0] < '0') || (pszIP[0] > '9') )
	{
		hostent * record = gethostbyname( pszIP );
		in_addr * address = (in_addr *)record->h_addr;
		ip = inet_ntoa( *address );
	}
	// IP
	STRINGCOPYLEN( (char*)0x0CDE160, 16, ip );
	STRINGCOPYLEN( (char*)0x0CDE13C, 16, ip );
	STRINGCOPYLEN( (char*)0x0CDE1A8, 16, ip );
	STRINGCOPYLEN( (char*)0x0CDE184, 16, ip );
	PORT_GAME = iPort;
	// PORT
	*(UINT*)0x0CDE180 = PORT_GAME;
	*(UINT*)0x0CDE15C = PORT_GAME;
	*(UINT*)0x0CDE1A4 = PORT_GAME;
	*(UINT*)0x0CDE1C8 = PORT_GAME;
}


void SocketGame::SocketClose( SocketData * sd )
{
	if ( (sd) && (sd->bInUse) )
	{
		WRITEDBG( "Close %s:%d", sd->szIP, sd->iPort );

		if ( sd == SOCKETL )
			*(DWORD*)0x03A97624 = NULL;
		if ( sd == SOCKETG )
			*(DWORD*)0x03A97620 = NULL;

		while ( sd->bConnecting )
			Sleep( 2500 );

		if ( IOCP_SERVER_TYPE == FALSE )
			sd->Close();
		else
		{
			if ( auto pc = GetIocpHandler( sd ) )
			{
				pc->pcClient->Disconnect( 1 );

				RemoveIocpHandler( sd );
			}
		}

		sd->UnInit();
	}
}

SocketData * SocketGame::GetFreeSocketData()
{
	SocketData * r = NULL;

	for ( int i = 0; i < MAX_CONNECTIONS; i++ )
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

SocketData * SocketGame::GetSocketData( const char * pszIP, int iPort )
{
	SocketData * r = NULL;

	for ( int i = 0; i < MAX_CONNECTIONS; i++ )
	{
		SocketData * sd = pcaSocketData + i;

		if ( sd->bInUse )
		{
			if ( (sd->iPort == iPort) && (lstrcmpiA( sd->szIP, pszIP ) == 0) )
			{
				r = sd;
				break;
			}
		}
	}

	return r;
}

SocketData * SocketGame::GetSocketData( INT64 iID )
{
	SocketData * r = NULL;

	for ( int i = 0; i < MAX_CONNECTIONS; i++ )
	{
		SocketData * sd = pcaSocketData + i;

		if ( sd->bInUse )
		{
			if ( sd->iConnectionID == iID )
			{
				r = sd;
				break;
			}
		}
	}

	return r;
}

CIocpGameHandler * SocketGame::GetIocpHandler( SocketData * sd )
{
	for ( auto pc : vIocpClients )
		if ( pc->pcSocketData == sd )
			return pc;

	return nullptr;
}

void SocketGame::AddIocpHandler( SocketData * sd )
{
	CIocpGameHandler * pcIocp = new CIocpGameHandler();

	pcIocp->Init( sd );

	vIocpClients.push_back( pcIocp );
}

void SocketGame::RemoveIocpHandler( SocketData * sd )
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

void SocketGame::ConnectServerL( const char * pszIP, int iPort )
{
	CloseServerL();

	SetSocketL( Connect( pszIP, iPort ) );
}

void SocketGame::ConnectServerG( const char * pszIP, int iPort )
{
	CloseServerG();

	SetSocketG( Connect( pszIP, iPort ) );
}

void SocketGame::CloseServerL()
{
	if ( SOCKETL != NULL )
	{
		if ( IOCP_SERVER_TYPE == FALSE )
			SOCKETL->Close();
		else
		{
			if ( auto pc = GetIocpHandler( SOCKETL ) )
			{
				pc->pcClient->Disconnect( 1 );

				RemoveIocpHandler( SOCKETL );
			}
		}

		SOCKETL->UnInit();

		SetSocketL( NULL );
	}
}

void SocketGame::CloseServerG()
{
	if ( SOCKETG != NULL )
	{
		if ( IOCP_SERVER_TYPE == FALSE )
			SOCKETG->Close();
		else
		{
			if ( auto pc = GetIocpHandler( SOCKETG ) )
			{
				pc->pcClient->Disconnect( 1 );

				RemoveIocpHandler( SOCKETG );
			}
		}

		SOCKETG->UnInit();

		SetSocketG( NULL );
	}
}

void SocketGame::CloseConnections()
{
	CloseServerL();
	CloseServerG();
}

int SocketGame::GetConnectionsAlive()
{
	int ret = 0;

	for ( int i = 0; i < MAX_CONNECTIONS; i++ )
	{
		SocketData * sd = pcaSocketData + i;

		if ( sd->bInUse )
			ret++;
	}

	return ret;
}

bool SocketGame::ConnectSocket( SocketData * sd, const char * pszIP, int iPort )
{
	bool bRet = false;

	SOCKETGAME->AddIocpHandler( sd );

	//Remove comment to back
	//bRet = sd->Connect( pszIP, iPort );
	if ( auto pc = SOCKETGAME->GetIocpHandler( sd ) )
	{
		if ( bRet = pc->pcClient->Connect( pszIP, iPort ) )
		{
			sockaddr_in sSockAddr;
			ZeroMemory( &sSockAddr, sizeof( sockaddr_in ) );
			sSockAddr.sin_family = AF_INET;
			sSockAddr.sin_addr.s_addr = IOCP::CIocpUtils::StringIPToNumberIPV4( pszIP );
			sSockAddr.sin_port = htons( (WORD)iPort );
			sd->Open( pc->pcClient->cIocpData.sSocket, &sSockAddr );
		}
	}
	return bRet;
}

SocketData * SocketGame::Connect( const char * pszIP, int iPort )
{ //conn thread: 521230
	WRITEDBG( "Connect %s:%d", pszIP, iPort );

	SocketData * sd = NULL;

	if ( (sd = GetInstance()->GetSocketData( pszIP, iPort )) == NULL )
	{
		sd = GetInstance()->GetFreeSocketData();

		if ( sd )
		{
			if ( !GetInstance()->ConnectSocket( sd, pszIP, iPort ) )
			{
				sd->UnInit();

				return NULL;
			}

			if ( iPort == PORT_GAME )
				SocketGame::GetInstance()->SetSocketL( sd );
			else
				SocketGame::GetInstance()->SetSocketG( sd );
		}
	}
	else
	{
		if ( SOCKETL )
			SocketGame::GetInstance()->SetSocketG( sd );

		PacketReceiving * p = new PacketReceiving;
		p->bInUse = TRUE;
		p->bDelete = TRUE;
		//Connection already Confirmed
		PacketVersion sPacket;
		sPacket.iLength = sizeof( PacketVersion );
		sPacket.iHeader = PKTHDR_Version;
		sPacket.bServerFull = FALSE;
		sPacket.iUnk2 = 0;
		sPacket.iVersion = READDWORD( 0x04AF7FFC );
		CopyMemory( p->baPacket, &sPacket, sizeof( PacketVersion ) );
		SOCKETGAME->SocketPacket( sd, p );
	}

	return sd;
}

DWORD WINAPI SocketGame::Receiver( SocketData * sd )
{
	SocketGame * m = GetInstance();

	HWND hWnd = m->GetHWND();
	do
	{
		WaitForSingleObject( sd->hReceiveThreadSignal, INFINITE );

		if ( !m->Active() )
			break;

		WRITEDBG( "[Receiver] Active (%s:%d)", sd->szIP, sd->iPort );

		PacketReceiving * p;
		while ( (p = sd->ReceivePacket()) != NULL )
		{
			//Post Packet Message
			WNDPROC_SOCKETPACKET( hWnd, sd, p );
		}

		sd->bBlockSend = TRUE;

		WRITEDBG( "[Receiver] Done" );

		if ( m->Active() )
		{
			if ( sd->bConnected )
			{
				//Post Close Message
				WNDPROC_SOCKETCLOSE( hWnd, sd );
			}
		}

		WRITEDBG( "[Receiver] Wait (%d) (%d)", sd->iRecvRet, sd->iRecvEC );
	}
	while ( m->Active() );

	return TRUE;
}

DWORD WINAPI SocketGame::Sender( SocketData * sd )
{
	SocketGame * m = GetInstance();

	DWORD dwTime = TICKCOUNT + 1000;

	do
	{
		WaitForSingleObject( sd->hSendThreadSignal, INFINITE );

		if ( !m->Active() )
			break;

		WRITEDBG( "[Sender] Active (%s:%d)", sd->szIP, sd->iPort );

		PacketSending * p;
		while ( (p = sd->NextPacketSending()) != NULL )
		{
			if ( sd->Send( p ) == FALSE )
			{
				sd->FreePacketSending( p );
				break;
			}

			sd->FreePacketSending( p );
		}

		WRITEDBG( "[Sender] Done" );
		WRITEDBG( "[Sender] Wait (%d) (%d)", sd->iSendRet, sd->iSendEC );
	}
	while ( m->Active() );

	return TRUE;
}

void SocketGame::SetDisconnectCode( int i )
{
	iDisconnectCode = i;

	BOOL bDC = (i >= 0) ? TRUE : FALSE;

	DISCONNECT_TIME = TICKCOUNT;
	DISCONNECT_TYPE = i;
}

void SocketGame::RequestOpenConnection( bool bGameRequest, int iSleep )
{
	return;

	SOCKET s = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

	if ( s != INVALID_SOCKET )
	{
		PacketSyncLoginGame p;
		ZeroMemory( &p, sizeof( PacketSyncLoginGame ) );
		p.iLength = sizeof( PacketSyncLoginGame );
		p.dwReserved1 = 0x64656A5A;
		p.dwReserved2 = 0x74656A5A;

		STRINGCOPY( p.szAccountName, (char *)0x039033E8 );
		STRINGCOPY( p.szPassword, (char *)0x039032E8 );

		int iLengthStr = STRLEN( p.szAccountName );
		for ( int i = 0; i < iLengthStr; i++ )
			p.szAccountName[i] ^= 0x7;

		iLengthStr = STRLEN( p.szPassword );
		for ( int i = 0; i < iLengthStr; i++ )
			p.szPassword[i] ^= 0x9;

		p.bGameRequest = bGameRequest;

		sockaddr_in sin;
		sin.sin_family = AF_INET;
		sin.sin_addr.s_addr = inet_addr( IP_FORTRESS_WORLD );
		sin.sin_port = htons( 6005 );

		if ( connect( s, (SOCKADDR *)& sin, sizeof( sin ) ) != SOCKET_ERROR )
		{
			LINGER l;
			l.l_onoff = TRUE;
			l.l_linger = 5;
			setsockopt( s, SOL_SOCKET, SO_LINGER, (char *)& l, sizeof( l ) );

			send( s, (char *)&p, p.iLength, 0 );
		}

		Sleep( iSleep );

		closesocket( s );
	}
}

BOOL SocketGame::SendPacketLogin( Packet * psPacket, BOOL bEncrypted )
{
	if ( SOCKETL == NULL )
		return TRUE;

	if ( auto pc = SOCKETGAME->GetIocpHandler( SOCKETL ) )
		pc->Send( SOCKETL, psPacket, bEncrypted );

	//SOCKETL->SendPacket( psPacket, bEncrypted );

	return TRUE;
}

BOOL SocketGame::SendPacketGame( Packet * psPacket, BOOL bEncrypted )
{
	if ( SOCKETG == NULL )
		return TRUE;

	//SOCKETG->SendPacket( psPacket, bEncrypted );

	if ( auto pc = SOCKETGAME->GetIocpHandler( SOCKETG ) )
		pc->Send( SOCKETG, psPacket, bEncrypted );

	return TRUE;
}

BOOL SocketGame::SendPacket( SocketData * pcSocketData, Packet * psPacket, BOOL bEncrypted )
{
	BOOL bRet = FALSE;

	if ( pcSocketData == SOCKETL )
		bRet = SendPacketLogin( psPacket, bEncrypted );
	else
		bRet = SendPacketGame( psPacket, bEncrypted );

	return bRet;
}
