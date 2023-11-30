#include "stdafx.h"
#include "CIocpServerHandler.h"
#include "CIocpServer.h"

namespace IOCP
{

CIocpServerHandler::CIocpServerHandler()
{
}

CIocpServerHandler::~CIocpServerHandler()
{
}

void CIocpServerHandler::OnNewConnection( INT64 iID, const IocpConnInfo & cConnection )
{
    printf( "dae %s - %s\n", cConnection.strIP.c_str(), cConnection.strHostName.c_str() );

    for ( int i = 1; i <= 50; i++ )
    {

        char szBuffer[512] = {0};

        sprintf_s( szBuffer, 512, "%d\n", i );


        std::vector<char> v( szBuffer, szBuffer + strlen( szBuffer ) );

        pcHandler->Send( iID, v );
    }
}

UINT CIocpServerHandler::OnReceive( INT64 iID, std::vector<char> vData, UINT iLengthReceived )
{
    char szBuffer[512] = {0};

    sprintf_s( szBuffer, 512, "%s ----  %d\n", vData.data(), rand() );

    std::vector<char> v( szBuffer, szBuffer + strlen( szBuffer ) );

    printf( szBuffer );

    //pcHandler->Send( iID, v );

    return 1;
}

void CIocpServerHandler::OnSent( IOCP::CIocpBase & cBase, INT64 iTotalSent )
{
}

void CIocpServerHandler::OnClientDisconnect( INT64 iID, UINT iErrorCode )
{
    printf( "dc cside\n" );
}

void CIocpServerHandler::OnDisconnect( INT64 iID, UINT iErrorCode )
{
    printf( "dc\n" );
}

void CIocpServerHandler::OnError( UINT iErrorCode )
{
}

void CIocpServerHandler::OnClose( UINT iErrorCode )
{
}

};