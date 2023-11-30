#include "stdafx.h"
#include "CIocpClient.h"

namespace IOCP
{

CIocpClient::CIocpClient()
{
}

CIocpClient::~CIocpClient()
{
	UnInit();
}

void CIocpClient::Init( UINT iBufferSize, UINT iNumThreads )
{
	if ( iBufferSize == 0 )
		cIocpData.iBufferSize = 8192;
	else
		cIocpData.iBufferSize = iBufferSize;

	cIocpData.sShutdownEvent = CreateEvent( NULL, TRUE, FALSE, NULL );

	InitializeWinsock();

	InitializeIocp();

	InitializeThreadPool( iNumThreads );
}

void CIocpClient::UnInit()
{
	if ( cIocpData.sShutdownEvent != INVALID_HANDLE_VALUE )
		SetEvent( cIocpData.sShutdownEvent );

	cIocpData.cConnectionManager.CloseAllConnections();

	for ( ThreadPool_t::iterator it = vThreadPool.begin(); it != vThreadPool.end(); it++ )
		PostQueuedCompletionStatus( cIocpData.sIoCompletionPort, 0, NULL, NULL );

	vThreadPool.clear();

	if ( cIocpData.sShutdownEvent != INVALID_HANDLE_VALUE )
	{
		CloseHandle( cIocpData.sShutdownEvent );
		cIocpData.sShutdownEvent = INVALID_HANDLE_VALUE;
	}

	if ( cIocpData.sIoCompletionPort != INVALID_HANDLE_VALUE )
	{
		CloseHandle( cIocpData.sIoCompletionPort );
		cIocpData.sIoCompletionPort = INVALID_HANDLE_VALUE;
	}

	if ( cIocpData.pHandler )
	{
		cIocpData.pHandler->OnClose( 0 );
		DELET( cIocpData.pHandler );
	}
}

void CIocpClient::Send( INT64 iID, std::vector<char> & vData )
{
	if ( auto pConnection = cIocpData.cConnectionManager.GetConnection( iID ) )
	{
		if ( auto pBaseSend = pConnection->CreateSend() )
		{
			pBaseSend->vData.swap( vData );
			pBaseSend->Reset();

			if ( CIocpUtils::PostSend( (*pBaseSend) ) != WSA_IO_PENDING )
			{
				pConnection->cSendQueue.RemoveSend( pBaseSend.get() );
				vData.swap( pBaseSend->vData );
			}
		}
	}
}

void CIocpClient::Send( INT64 iID, char * pBuffer, UINT iLength )
{
	std::vector<char> vData( pBuffer, pBuffer + iLength );
	Send( iID, vData );
}

void CIocpClient::Shutdown( INT64 iID, int iHow )
{
	if ( auto pConnection = cIocpData.cConnectionManager.GetConnection( iID ) )
		::shutdown( pConnection->sSocket, iHow );
}

void CIocpClient::Disconnect( INT64 iID )
{
	if ( auto pConnection = cIocpData.cConnectionManager.GetConnection( iID ) )
	{
		Shutdown( iID, SD_BOTH );

		::InterlockedIncrement( &pConnection->iDisconnecting );

		CIocpUtils::PostDisconnect( cIocpData, (*pConnection) );
	}
}

bool CIocpClient::Connect( const std::string strIP, int iPort )
{
	cIocpData.sSocket = WSASocket( AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED );
	if ( cIocpData.sSocket == INVALID_SOCKET )
		return false;

	sockaddr_in sSockAddr;
	ZeroMemory( &sSockAddr, sizeof( sockaddr_in ) );
	sSockAddr.sin_family = AF_INET;
	sSockAddr.sin_addr.s_addr = CIocpUtils::StringIPToNumberIPV4( strIP );
	sSockAddr.sin_port = htons( (WORD)iPort );

	//QOS Data test for AntiDDoS
	QOS sQOSData;
	sQOSData.SendingFlowspec.DelayVariation = QOS_NOT_SPECIFIED;
	sQOSData.SendingFlowspec.Latency = QOS_NOT_SPECIFIED;
	sQOSData.SendingFlowspec.MaxSduSize = QOS_NOT_SPECIFIED;
	sQOSData.SendingFlowspec.MinimumPolicedSize = 7;
	sQOSData.SendingFlowspec.PeakBandwidth = QOS_NOT_SPECIFIED;
	sQOSData.SendingFlowspec.ServiceType = SERVICETYPE_BESTEFFORT;
	sQOSData.SendingFlowspec.TokenBucketSize = QOS_NOT_SPECIFIED;
	sQOSData.SendingFlowspec.TokenRate = SERVICETYPE_BESTEFFORT;

	sQOSData.ReceivingFlowspec.DelayVariation = QOS_NOT_SPECIFIED;
	sQOSData.ReceivingFlowspec.Latency = QOS_NOT_SPECIFIED;
	sQOSData.ReceivingFlowspec.MaxSduSize = QOS_NOT_SPECIFIED;
	sQOSData.ReceivingFlowspec.MinimumPolicedSize = QOS_NOT_SPECIFIED;
	sQOSData.ReceivingFlowspec.PeakBandwidth = QOS_NOT_SPECIFIED;
	sQOSData.ReceivingFlowspec.ServiceType = SERVICETYPE_BESTEFFORT;
	sQOSData.ReceivingFlowspec.TokenBucketSize = QOS_NOT_SPECIFIED;
	sQOSData.ReceivingFlowspec.TokenRate = SERVICETYPE_BESTEFFORT;

	if ( WSAConnect( cIocpData.sSocket, (SOCKADDR *)&sSockAddr, sizeof( sockaddr_in ), NULL, NULL, NULL, NULL ) == SOCKET_ERROR )
		return false;

	ULONG iNonBlocking = 1;
	if ( ioctlsocket( cIocpData.sSocket, FIONBIO, &iNonBlocking ) == SOCKET_ERROR )
		return false;

	std::shared_ptr<CIocpConnection> pConnection( new CIocpConnection( cIocpData.sSocket, cIocpData.GetNextID(), cIocpData.iBufferSize ) );

	cIocpData.cConnectionManager.AddConnection( pConnection );

	CIocpUtils::CreateCompletion( (HANDLE)pConnection->sSocket, cIocpData );

	if ( CIocpUtils::PostReceive( pConnection->sReceive ) != WSA_IO_PENDING )
		if ( pConnection->CloseReceive() )
			CIocpUtils::PostDisconnect( cIocpData, (*pConnection) );

	const int RCVWND_SIZEL = 0x800000;
	int SNDWND_SIZEL = 0;

	DWORD dwUnused = 0;

	BOOL SOCKET_NODELAYL = TRUE;

	WSAIoctl( pConnection->sSocket, SIO_IDEAL_SEND_BACKLOG_QUERY, NULL, NULL, &SNDWND_SIZEL, sizeof( int ), &dwUnused, NULL, NULL );

	//Set Receive and Send Window sizes
	setsockopt( pConnection->sSocket, IPPROTO_TCP, TCP_NODELAY, (char *)&SOCKET_NODELAYL, sizeof( BOOL ) );
	setsockopt( pConnection->sSocket, SOL_SOCKET, SO_RCVBUF, (char *)&RCVWND_SIZEL, sizeof( int ) );
	setsockopt( pConnection->sSocket, SOL_SOCKET, SO_SNDBUF, (char *)&SNDWND_SIZEL, sizeof( int ) );

	return true;
}

void CIocpClient::InitializeThreadPool( UINT iNumThreads )
{
	if ( iNumThreads == 0 )
		iNumThreads = CIocpUtils::GetNumIocpThreads();

	vThreadPool.reserve( iNumThreads );

	for ( UINT i = 0; i < iNumThreads; i++ )
		vThreadPool.push_back( std::shared_ptr<CIocpWorkerThread>( new CIocpWorkerThread( cIocpData ) ) );
}

void CIocpClient::InitializeIocp()
{
	cIocpData.sIoCompletionPort = CIocpUtils::CreateIocp( 0 );
}

void CIocpClient::InitializeWinsock()
{
	WSADATA wsaData;
	WSAStartup( MAKEWORD( 2, 2 ), &wsaData );
}

void CIocpClient::InitializeSocket( UINT iAddrToListen, int iPort )
{

}

};