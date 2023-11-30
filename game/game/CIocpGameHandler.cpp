#include "stdafx.h"
#include "CIocpGameHandler.h"


CIocpGameHandler::CIocpGameHandler()
{
	pcClient = new IOCP::CIocpClient();
}

CIocpGameHandler::~CIocpGameHandler()
{
	DELET( pcClient );
}

void CIocpGameHandler::Init( SocketData * sd )
{
	pcClient->Init( IOCP::MAX_IOCP_PKTSIZE, 0 );
	pcClient->cIocpData.pHandler = this;

	pcSocketData = sd;
}

void CIocpGameHandler::UnInit()
{
	pcClient->UnInit();
}

void CIocpGameHandler::Send( SocketData * pcSocketData, Packet * psPacket, BOOL bEncrypted )
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

	pcClient->Send( 1, (char*)psNewPacket, psNewPacket->iLength );

	DELET( p );
}

UINT CIocpGameHandler::OnReceive( INT64 iID, std::vector<char> vData, UINT iLengthReceived )
{
	auto GetPackets = []( SocketData * pcSocketData, std::vector<PacketReceiving *> & vPackets, std::vector<char> & vData, UINT iLengthReceived ) -> UINT
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

			vPackets.push_back( p );

			uNext += uSize;
		}

		return vPackets.size() > 0 ? uNext : 0;
	};

	UINT uTotalProcessed = 0;

	std::vector<PacketReceiving *> vPackets;
	if ( (uTotalProcessed = GetPackets( pcSocketData, vPackets, vData, iLengthReceived )) > 0 )
	{
		for ( auto p : vPackets )
			PostMessageA( SOCKETGAME->GetHWND(), WM_SOCKETPACKET, (WPARAM)pcSocketData, (LPARAM)p );
	}
	else
	{
		for ( auto p : vPackets )
			DELET( p );
	}

	vPackets.clear();

	return uTotalProcessed;
}

void CIocpGameHandler::OnSent( IOCP::CIocpBase & cBase, INT64 iTotalSent )
{
	
}

void CIocpGameHandler::OnClientDisconnect( INT64 iID, UINT iErrorCode )
{
	pcClient->Shutdown( iID, SD_SEND );

	pcClient->Disconnect( iID );
}

void CIocpGameHandler::OnDisconnect( INT64 iID, UINT iErrorCode )
{
	
}

void CIocpGameHandler::OnError( UINT iErrorCode )
{
//	WRITEERR( "%d", iErrorCode );
}

void CIocpGameHandler::OnClose( UINT iErrorCode )
{
}
