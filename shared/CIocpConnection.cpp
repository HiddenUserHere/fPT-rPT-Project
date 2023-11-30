
#include "stdafx.h"
#include "CIocpConnection.h"

namespace IOCP
{

CIocpConnection::CIocpConnection( SOCKET _s, INT64 _iID, UINT _iBufferSize ) :
    sSocket( _s ), iID( _iID ), iBufferSize( _iBufferSize ),
    iDisconnecting( 0 ), iReceiveClosed( 0 ), iSendClosing( 0 ),
    sReceive( _s, _iID, CIocpBase::IOCPTYPE_Receive, iBufferSize ),
    sDisconnect( _s, _iID, CIocpBase::IOCPTYPE_Disconnect, 0 )
{
}

CIocpConnection::~CIocpConnection()
{
    closesocket( sSocket );
}

bool CIocpConnection::CloseReceive()
{
    if ( ::InterlockedExchange( &iReceiveClosed, 1 ) == 0 )
    {
        if ( sReceive.hEvent != INVALID_HANDLE_VALUE )
        {
            CloseHandle( sReceive.hEvent );
            sReceive.hEvent = INVALID_HANDLE_VALUE;
        }

        return true;
    }

    return false;
}

std::shared_ptr<CIocpBase> CIocpConnection::CreateSend()
{
    std::shared_ptr<CIocpBase> c( new CIocpBase( sSocket, iID, CIocpBase::IOCPTYPE_Send, 0 ) );

    cSendQueue.AddSend( c );

    return c;
}

bool CIocpConnection::HasOutstanding()
{
    if ( ::InterlockedExchangeAdd( &iReceiveClosed, 0 ) == 0 )
        return true;

    return cSendQueue.NumOutstanding() > 0 ? true : false;
}

};