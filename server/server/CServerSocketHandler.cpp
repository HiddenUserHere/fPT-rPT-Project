#include "stdafx.h"
#include "CServerSocketHandler.h"

IOCP::CIocpServer * CServerSocketHandler::pcServer = nullptr;

BOOL CServerSocketHandler::bInit = FALSE;

BOOL bTestingSend = FALSE;

CServerSocketHandler::CServerSocketHandler()
{
}

CServerSocketHandler::~CServerSocketHandler()
{
	DELET( pcServer );
}

void CServerSocketHandler::Init()
{
	pcServer = new IOCP::CIocpServer( SERVER_PORT, this, INADDR_ANY, IOCP::MAX_IOCP_PKTSIZE, 64 );

	CServerSocketHandler::bInit = TRUE;
}

void CServerSocketHandler::Send( User * pcUser, Packet * psPacket, BOOL bEncrypted )
{
	//Send With User
	Send( pcUser->pcUserData->pcSocketData, psPacket, bEncrypted );
}

void CServerSocketHandler::Send( SocketData * pcSocketData, Packet * psPacket, BOOL bEncrypted )
{
	if ( pcSocketData )
	{
		if ( CServerSocketHandler::bInit )
		{
			PacketSending * p = new PacketSending;
			CopyMemory( p->baPacket, psPacket, psPacket->iLength );

			Packet * psNewPacket = (Packet *)p->baPacket;

			if ( psNewPacket->iHeader != PKTHDR_KeySet )
			{
				psNewPacket->iEncKeyIndex = 0;
				psNewPacket->iEncrypted = bEncrypted ? 1 : 0;
			}

			if ( psNewPacket->iEncrypted == 1 )
				pcSocketData->EncryptPacket( psNewPacket );

			if ( auto pcIocpClient = SOCKETSERVER->GetIocpHandler( pcSocketData ) )
			{
				if ( pcIocpClient->pcClient )
					pcIocpClient->pcClient->Send( 1, (char *)psNewPacket, psNewPacket->iLength );
			}
			else
				CServerSocketHandler::pcServer->Send( pcSocketData->iConnectionID, (char *)psNewPacket, psNewPacket->iLength );

			DELET( p );
		}
		else
			pcSocketData->SendPacket( psPacket, bEncrypted );
	}
}

void CServerSocketHandler::OnNewConnection( INT64 iID, const IOCP::IocpConnInfo & cConnection )
{
	if ( auto pConnection = pcServer->cIocpData.cConnectionManager.GetConnection( iID ) )
	{
		auto psAccept = new SocketServerAccept;
		ZeroMemory( &psAccept->sAddr, sizeof( sockaddr_in ) );
		psAccept->sAddr.sin_family = AF_INET;
		psAccept->sAddr.sin_addr.S_un.S_addr = IOCP::CIocpUtils::StringIPToNumberIPV4( cConnection.strIP );
		psAccept->sAddr.sin_port = htons( (WORD)SERVER_PORT );
		psAccept->iID = iID;

		const int RCVWND_SIZEL = 0x800000;
		const int SNDWND_SIZEL = 0;

		BOOL SOCKET_NODELAYL = TRUE;

		//Set Receive and Send Window sizes
		setsockopt( pConnection->sSocket, IPPROTO_TCP, TCP_NODELAY, (char *)&SOCKET_NODELAYL, sizeof( BOOL ) );
		setsockopt( pConnection->sSocket, SOL_SOCKET, SO_RCVBUF, (char *)&RCVWND_SIZEL, sizeof( int ) );
		setsockopt( pConnection->sSocket, SOL_SOCKET, SO_SNDBUF, (char *)&SNDWND_SIZEL, sizeof( int ) );

		//Send Accept Message
		SendMessageA( SOCKETSERVER->GetHWND(), WM_SOCKETACCEPT, pConnection->sSocket, (LPARAM)psAccept );
	}
}

UINT CServerSocketHandler::OnReceive( INT64 iID, std::vector<char> vData, UINT iLengthReceived )
{
	auto GetPackets = [iID]( SocketData * pcSocketData, std::vector<PacketReceiving *> & vPackets, std::vector<char> & vData, UINT iLengthReceived ) -> UINT
	{
		UINT uNext = 0;
		while ( uNext < iLengthReceived )
		{
			UINT uSize = READWORD( &vData.data()[uNext] );
			if ( (uSize + uNext) > IOCP::MAX_IOCP_PKTSIZE )
				break;

			if ( (uSize + uNext) > iLengthReceived )
				break;

			if ( uSize == 0 )
				break;

			if ( (uSize < 8) || (uSize > MAX_PKTSIZ) )
				break;

			PacketReceiving * p = new PacketReceiving;
			p->bDelete = TRUE;
			p->bInUse = TRUE;
			CopyMemory( p->baPacket, &vData.data()[uNext], uSize );

			Packet * psPacket = (Packet *)p->baPacket;
			if ( pcSocketData->PacketEncrypted( psPacket ) )
				pcSocketData->DecryptPacket( psPacket );

#if defined(_SERVER)
			bool bAllowed = false;
			switch ( psPacket->iHeader & 0xFFFF0000 )
			{
				case 0x7F000000:
				case 0x5A320000:
				case 0x48470000:
				case 0x43550000:
				case 0x6F6A0000:
				case 0x6A6A0000:
				case 0x64640000:
				case 0x50320000:
				case 0x435A0000:
				case 0x48480000:
					bAllowed = true;
					break;
			}

			if ( bAllowed == false )
			{
				DELET( p );

				WNDPROC_SOCKETCLOSE( SOCKETSERVER->GetHWND(), pcSocketData );

				pcServer->Disconnect( iID );

				break;
			}
#endif


			vPackets.push_back( p );

			uNext += uSize;
		}

		return vPackets.size() > 0 ? uNext : 0;
	};

	UINT uTotalProcessed = 0;

	if ( auto pcSocketData = SOCKETSERVER->GetSocketDataByID( iID ) )
	{
		std::vector<PacketReceiving *> vPackets;
		if ( (uTotalProcessed = GetPackets( pcSocketData, vPackets, vData, iLengthReceived )) > 0 )
		{
			for ( auto p : vPackets )
				PostMessageA( SOCKETSERVER->GetHWND(), WM_SOCKETPACKET, (WPARAM)pcSocketData, (LPARAM)p );
		}
		else
		{
			for ( auto p : vPackets )
				DELET( p );
		}

		vPackets.clear();
	}

/*
	if ( LOGIN_SERVER )
	{
		static INT64 iRRR = 0;
		iRRR++;

		std::string str = "C:\\Test" + std::to_string( iRRR ) + ".dat";

		FILE * pFile = nullptr;
		fopen_s( &pFile, str.c_str(), "wb" );
		if ( pFile )
		{
			fwrite( vData.data(), iLengthReceived, 1, pFile );
			fclose( pFile );
		}
	}
*/
	return uTotalProcessed;
}

void CServerSocketHandler::OnSent( IOCP::CIocpBase & cBase, INT64 iTotalSent )
{
	bTestingSend = FALSE;
}

void CServerSocketHandler::OnClientDisconnect( INT64 iID, UINT iErrorCode )
{
	if ( auto pcSocketData = SOCKETSERVER->GetSocketDataByID( iID ) )
		WNDPROC_SOCKETCLOSE( SOCKETSERVER->GetHWND(), pcSocketData );

	pcServer->Shutdown( iID, SD_SEND );

	pcServer->Disconnect( iID );
}

void CServerSocketHandler::OnDisconnect( INT64 iID, UINT iErrorCode )
{
	if ( auto pcSocketData = SOCKETSERVER->GetSocketDataByID( iID ) )
		WNDPROC_SOCKETCLOSE( SOCKETSERVER->GetHWND(), pcSocketData );
}

void CServerSocketHandler::OnError( UINT iErrorCode )
{

}

void CServerSocketHandler::Disconnect( SocketData * pcSocketData )
{
	pcServer->Disconnect( pcSocketData->iConnectionID );
}
