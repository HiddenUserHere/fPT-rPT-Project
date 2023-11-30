#include "stdafx.h"
#include "CIocpServer.h"

namespace IOCP
{

CIocpServer::CIocpServer( int iPort, CIocpHandlerImpl * pIocpImpl, UINT iAddrToListen, UINT iBufferSize, UINT iNumThreads )
{
	cIocpData.pHandler = pIocpImpl;

	pIocpImpl->SetHandler( this );

	Init( iPort, iAddrToListen, iBufferSize, iNumThreads );
}

CIocpServer::~CIocpServer()
{
	UnInit();
}

void CIocpServer::Init( int iPort, UINT iAddrToListen, UINT iBufferSize, UINT iNumThreads )
{
	if ( iBufferSize == 0 )
		cIocpData.iBufferSize = 8192;
	else
		cIocpData.iBufferSize = iBufferSize;

	cIocpData.sShutdownEvent = CreateEvent( NULL, TRUE, FALSE, NULL );

	InitializeWinsock();

	InitializeIocp();

	InitializeThreadPool( iNumThreads );

	InitializeSocket( iAddrToListen, iPort );

	InitializeAcceptEvent();
}

void CIocpServer::UnInit()
{
	if ( cIocpData.sShutdownEvent != INVALID_HANDLE_VALUE )
		SetEvent( cIocpData.sShutdownEvent );

	cIocpData.cConnectionManager.CloseAllConnections();

	for ( ThreadPool_t::iterator it = vThreadPool.begin(); it != vThreadPool.end(); it++ )
		PostQueuedCompletionStatus( cIocpData.sIoCompletionPort, 0, NULL, NULL );

	vThreadPool.clear();

	if ( cIocpData.sSocket != INVALID_SOCKET )
	{
		closesocket( cIocpData.sSocket );
		cIocpData.sSocket = INVALID_SOCKET;
	}

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

void CIocpServer::Send( INT64 iID, std::vector<char> & vData )
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

void CIocpServer::Send( INT64 iID, char * pBuffer, UINT iLength )
{
	std::vector<char> vData( pBuffer, pBuffer + iLength );
	Send( iID, vData );
}

void CIocpServer::Shutdown( INT64 iID, int iHow )
{
	if ( auto pConnection = cIocpData.cConnectionManager.GetConnection( iID ) )
		::shutdown( pConnection->sSocket, iHow );
}

void CIocpServer::Disconnect( INT64 iID )
{
	if ( auto pConnection = cIocpData.cConnectionManager.GetConnection( iID ) )
	{
		Shutdown( iID, SD_BOTH );

		::InterlockedIncrement( &pConnection->iDisconnecting );

		CIocpUtils::PostDisconnect( cIocpData, (*pConnection) );
	}
}

void CIocpServer::InitializeThreadPool( UINT iNumThreads )
{
	if ( iNumThreads == 0 )
		iNumThreads = CIocpUtils::GetNumIocpThreads();

	vThreadPool.reserve( iNumThreads );

	for ( UINT i = 0; i < iNumThreads; i++ )
		vThreadPool.push_back( std::shared_ptr<CIocpWorkerThread>( new CIocpWorkerThread( cIocpData ) ) );
}

void CIocpServer::InitializeIocp()
{
	cIocpData.sIoCompletionPort = CIocpUtils::CreateIocp( 0 );
}

void CIocpServer::InitializeWinsock()
{
	WSADATA wsaData;
	WSAStartup( MAKEWORD( 2, 2 ), &wsaData );
}

void CIocpServer::InitializeSocket( UINT iAddrToListen, int iPort )
{
	if ( cIocpData.sSocket != INVALID_SOCKET )
		closesocket( cIocpData.sSocket );

	cIocpData.sSocket = CIocpUtils::CreateOverlappedSocket();

	ULONG iNonBlocking = 1;
	ioctlsocket( cIocpData.sSocket, FIONBIO, &iNonBlocking );

	sockaddr_in sServerAddr;
	ZeroMemory( &sServerAddr, sizeof( sockaddr_in ) );

	sServerAddr.sin_family = AF_INET;
	sServerAddr.sin_addr.s_addr = iAddrToListen;
	sServerAddr.sin_port = htons( (WORD)iPort );

	if ( ::bind( cIocpData.sSocket, (sockaddr *)&sServerAddr, sizeof( sockaddr_in ) ) == SOCKET_ERROR )
	{
		closesocket( cIocpData.sSocket );
		cIocpData.sSocket = INVALID_SOCKET;
	}

	if ( listen( cIocpData.sSocket, SOMAXCONN ) == SOCKET_ERROR )
	{
		closesocket( cIocpData.sSocket );
		cIocpData.sSocket = INVALID_SOCKET;
	}

	cIocpData.pfnAcceptEx = CIocpUtils::LoadAcceptEx( cIocpData.sSocket );

	CIocpUtils::CreateCompletion( (HANDLE)cIocpData.sSocket, cIocpData );
}

void CIocpServer::InitializeAcceptEvent()
{
	cIocpData.cAcceptBase.sSocket = CIocpUtils::CreateOverlappedSocket();

	ULONG iNonBlocking = 1;
	ioctlsocket( cIocpData.cAcceptBase.sSocket, FIONBIO, &iNonBlocking );

	CIocpUtils::PostAccept( cIocpData );
}

};