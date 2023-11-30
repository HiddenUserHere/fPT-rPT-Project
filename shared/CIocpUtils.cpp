#include "stdafx.h"
#include "CIocpUtils.h"

namespace IOCP
{

static std::vector<std::string> split_local( const std::string & s, char delim )
{
	std::stringstream ss( s.c_str() );
	std::string item;

	std::vector<std::string> tokens;

	while ( std::getline( ss, item, delim ) )
		tokens.push_back( item );

	return tokens;
}


UINT CIocpUtils::StringIPToNumberIPV4( const std::string strIP )
{
	struct SIPDataConverter
	{
		union
		{
			BYTE baIP[4];
			UINT uIP;
		};
	};

	SIPDataConverter sIP;
	sIP.uIP = 0;

	auto vIP = split_local( strIP, '.' );
	if ( vIP.size() == 4 )
	{
		for ( int i = 0; i < 4; i++ )
		{
			if ( (atoi( vIP[i].c_str() ) < 0) || (atoi( vIP[i].c_str() ) > 255) )
			{
				sIP.uIP = 0;
				break;
			}

			sIP.baIP[i] = atoi( vIP[i].c_str() );
		}
	}
	vIP.clear();

	return sIP.uIP;
};

std::string CIocpUtils::NumberIPToStringIPV4( const UINT uIP )
{
	in_addr sAddr;
	sAddr.S_un.S_addr = uIP;

	char szIP[32] = {0};
	int iLength = snprintf( szIP, 32, "%d.%d.%d.%d", uIP & 0xFF, (uIP >> 8) & 0xFF, (uIP >> 16) & 0xFF, (uIP >> 24) & 0xFF );

	if ( (iLength >= 0) && (iLength < 32) )
		return std::string( szIP );

	return std::string( "" );
};


SOCKET CIocpUtils::CreateOverlappedSocket()
{
	return WSASocket( AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED );
}

IocpConnInfo CIocpUtils::GetConnectionInfo( SOCKET _s )
{
	IocpConnInfo sConnInfo;
	sConnInfo.iPort = 0;
	sConnInfo.strHostName = "";
	sConnInfo.strIP = "";

	sockaddr_in sAddrIn;
	ZeroMemory( &sAddrIn, sizeof( sockaddr_in ) );
	int iLength = sizeof( sockaddr_in );

	if ( ::getpeername( _s, (sockaddr *)&sAddrIn, &iLength ) != 0 )
		return sConnInfo;

	sConnInfo.strIP = NumberIPToStringIPV4( sAddrIn.sin_addr.S_un.S_addr );
	sConnInfo.iPort = (int)ntohs( sAddrIn.sin_port );

	char szHostName[NI_MAXHOST] = {0};
	char szServInfo[NI_MAXSERV] = {0};
	if ( getnameinfo( (sockaddr *)&sAddrIn, sizeof( sockaddr ), szHostName, NI_MAXHOST,
					  szServInfo, NI_MAXSERV, NI_NUMERICSERV ) != 0 )
		return sConnInfo;

	sConnInfo.strHostName = szHostName;

	return sConnInfo;
}

int CALLBACK ConditionAcceptFunc(
	LPWSABUF lpCallerId,
	LPWSABUF lpCallerData,
	LPQOS pQos,
	LPQOS lpGQOS,
	LPWSABUF lpCalleeId,
	LPWSABUF lpCalleeData,
	GROUP FAR * g,
	DWORD_PTR dwCallbackData
)
{
	if ( (pQos != NULL) && (pQos->SendingFlowspec.MinimumPolicedSize == 7) )
	{
		RtlZeroMemory( pQos, sizeof( QOS ) );
		return CF_ACCEPT;
	}
	

	return CF_ACCEPT;
}

void CIocpUtils::PostAccept( CIocpData & cIocpData )
{
	DWORD dwReceived = 0;

	INT dwAddrSize = sizeof( sockaddr_in ) + 16;

	ULONG iNonBlocking = 1;
	ioctlsocket( cIocpData.sSocket, FIONBIO, &iNonBlocking );

	
	if ( cIocpData.pfnAcceptEx( cIocpData.sSocket, cIocpData.cAcceptBase.sSocket, cIocpData.cAcceptBase.vData.data(),
								0, dwAddrSize, dwAddrSize, &dwReceived, &cIocpData.cAcceptBase ) == FALSE )
	{
		UINT iErrorCode = GetLastError();
		if ( iErrorCode != ERROR_IO_PENDING )
			if ( cIocpData.pHandler )
				cIocpData.pHandler->OnError( iErrorCode );
	}
	else
		PostQueuedCompletionStatus( cIocpData.sIoCompletionPort, 0, (ULONG_PTR)&cIocpData, &cIocpData.cAcceptBase );

	

	/*
	if ( (cIocpData.cAcceptBase.sSocket = WSAAccept( cIocpData.sSocket, (SOCKADDR *)cIocpData.cAcceptBase.vData.data(), &dwAddrSize, &ConditionAcceptFunc, NULL )) == INVALID_SOCKET )
	{
		UINT iErrorCode = WSAGetLastError();
		if ( iErrorCode != ERROR_IO_PENDING )
			if ( cIocpData.pHandler )
				cIocpData.pHandler->OnError( iErrorCode );
	}
	*/
}

int CIocpUtils::PostReceive( CIocpBase & cBase )
{
	DWORD dwFlags = 0;

	if ( WSARecv( cBase.sSocket, &cBase.sWSABuffer, 1, NULL, &dwFlags, &cBase, NULL ) == SOCKET_ERROR )
		return WSAGetLastError();

	return WSA_IO_PENDING;
}

int CIocpUtils::PostSend( CIocpBase & cBase )
{
	DWORD dwBytes = 0;

	if ( WSASend( cBase.sSocket, &cBase.sWSABuffer, 1, &dwBytes, 0, &cBase, NULL ) == SOCKET_ERROR )
		return WSAGetLastError();

	return WSA_IO_PENDING;
}

int CIocpUtils::PostSendQueue( CIocpData & cIocpData, CIocpBase & cBase )
{
	if ( PostQueuedCompletionStatus( cIocpData.sIoCompletionPort, 0, (ULONG_PTR)&cIocpData, (LPOVERLAPPED)&cBase ) == FALSE )
		return GetLastError();

	return NO_ERROR;
}

void CIocpUtils::CreateCompletion( HANDLE _hHandle, CIocpData & cIocpData )
{
	if ( ::CreateIoCompletionPort( _hHandle, cIocpData.sIoCompletionPort, (ULONG_PTR)&cIocpData, 0 ) != cIocpData.sIoCompletionPort )
		if ( cIocpData.pHandler )
			cIocpData.pHandler->OnError( GetLastError() );
}

HANDLE CIocpUtils::CreateIocp( int iNumThreads )
{
	return CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, iNumThreads );
}

int CIocpUtils::GetNumIocpThreads()
{
	SYSTEM_INFO si;
	GetSystemInfo( &si );
	return si.dwNumberOfProcessors * 2;
}

int CIocpUtils::PostDisconnect( CIocpData & cIocpData, CIocpConnection & cConnection )
{
	CIocpBase * pcIocpDisconnectBase = new CIocpBase( cConnection.sSocket, cConnection.iID, CIocpBase::IocpType::IOCPTYPE_Disconnect, 0 );

	if ( PostQueuedCompletionStatus( cIocpData.sIoCompletionPort, 0, (ULONG_PTR)&cIocpData, (LPOVERLAPPED)pcIocpDisconnectBase ) == FALSE )
		return GetLastError();

	return NO_ERROR;
}

LPFN_ACCEPTEX CIocpUtils::LoadAcceptEx( SOCKET _s )
{
	LPFN_ACCEPTEX lpfnAcceptEx = NULL;
	DWORD dwBytes = 0;

	GUID GuidAcceptEx = WSAID_ACCEPTEX;

	if ( WSAIoctl( _s, SIO_GET_EXTENSION_FUNCTION_POINTER, &GuidAcceptEx, sizeof( GUID ), &lpfnAcceptEx, sizeof( lpfnAcceptEx ), &dwBytes, NULL, NULL ) == SOCKET_ERROR )
		return NULL;

	return lpfnAcceptEx;
}

};