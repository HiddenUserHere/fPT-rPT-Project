#include "stdafx.h"
#include "CIocpWorkerThread.h"

namespace IOCP
{

CIocpWorkerThread::CIocpWorkerThread( CIocpData & cSharedData ) : cIocpData( cSharedData )
{
    sThread = std::thread( std::bind( &CIocpWorkerThread::Run, this ) );
}

CIocpWorkerThread::~CIocpWorkerThread()
{
    sThread.join();
}

void CIocpWorkerThread::Run()
{
    while ( true )
    {
        void * pKey = nullptr;

        OVERLAPPED * pOverlapped = nullptr;

        DWORD dwTransferred = 0;

        if ( GetQueuedCompletionStatus( cIocpData.sIoCompletionPort, &dwTransferred, (LPDWORD)&pKey, &pOverlapped, INFINITE ) == FALSE )
        {
            HandleFailure( pOverlapped, dwTransferred, WSAGetLastError() );
            continue;
        }

        if ( pKey == nullptr )
            break;

        HandleBase( (*(CIocpBase *)pOverlapped), dwTransferred );
    }
}

void CIocpWorkerThread::HandleBase( CIocpBase & cBase, DWORD dwTransferred )
{
    switch ( cBase.eType )
    {
        case CIocpBase::IocpType::IOCPTYPE_Receive:
            HandleReceive( cBase, dwTransferred );
            break;
        case CIocpBase::IocpType::IOCPTYPE_Send:
            //HandleSend( cBase, dwTransferred );
            HandleSent( cBase, dwTransferred );
            break;
        case CIocpBase::IocpType::IOCPTYPE_Sent:
            HandleSent( cBase, dwTransferred );
            break;
        case CIocpBase::IocpType::IOCPTYPE_Accept:
            HandleAccept( cBase, dwTransferred );
            break;
        case CIocpBase::IocpType::IOCPTYPE_Disconnect:
            HandleDisconnect( cBase );
            break;

        default:
            break;
    }
}

void CIocpWorkerThread::HandleFailure( OVERLAPPED * psOverlapped, DWORD dwTransferred, UINT iErrorCode )
{
    if ( psOverlapped )
        HandleBase( (*(CIocpBase *)psOverlapped), dwTransferred );
    else if ( cIocpData.pHandler )
        cIocpData.pHandler->OnError( iErrorCode );
}

void CIocpWorkerThread::HandleReceive( CIocpBase & cBase, DWORD dwTransferred )
{
    if ( auto cConnection = cIocpData.cConnectionManager.GetConnection( cBase.iID ) )
    {
        std::scoped_lock<std::mutex> l( cConnection->sMutexReceive );

        if ( dwTransferred )
        {
            if ( cIocpData.pHandler )
            {
                if ( UINT uProcessed = cIocpData.pHandler->OnReceive( cBase.iID, cBase.vData, dwTransferred + cBase.iTransferred ); uProcessed < (dwTransferred + cBase.iTransferred) )
                {
                    if ( uProcessed == 0 )
                        cBase.Add( dwTransferred );
                    else
                        cBase.BackTop( uProcessed, dwTransferred + cBase.iTransferred );
                }
                else
                {
                    cBase.vData.resize( cBase.iBufferSize );
                    cBase.Reset( true );
                }
            }
        }
        else
        {
            cBase.vData.resize( cBase.iBufferSize );
            cBase.Reset( true );
        }

        UINT iErrorCode = 0;

        if ( (dwTransferred == 0) || ((iErrorCode = CIocpUtils::PostReceive( cBase )) != WSA_IO_PENDING) )
        {
            if ( cConnection->CloseReceive() )
            {
                ::shutdown( cConnection->sSocket, SD_RECEIVE );

                if ( cIocpData.pHandler )
                    cIocpData.pHandler->OnClientDisconnect( cBase.iID, iErrorCode );
            }
        }
    }
}

void CIocpWorkerThread::HandleSend( CIocpBase & cBase, DWORD dwTransferred )
{
    if ( auto pConnection = cIocpData.cConnectionManager.GetConnection( cBase.iID ) )
    {
        std::scoped_lock<std::mutex> l( pConnection->sMutexSend );

        cBase.eType = CIocpBase::IocpType::IOCPTYPE_Sent;

        if ( CIocpUtils::PostSend( cBase ) != WSA_IO_PENDING )
            pConnection->cSendQueue.RemoveSend( &cBase );
    }
}

void CIocpWorkerThread::HandleSent( CIocpBase & cBase, DWORD dwTransferred )
{
    if ( auto pConnection = cIocpData.cConnectionManager.GetConnection( cBase.iID ) )
    {
        if ( dwTransferred > 0 )
        {
            if ( cIocpData.pHandler )
                cIocpData.pHandler->OnSent( cBase, dwTransferred );
        }

        //std::scoped_lock<std::mutex> l( pConnection->sMutexSend );

        /*
        if ( cBase.Add( dwTransferred, false ) < cBase.vData.size() )
        {
            if ( CIocpUtils::PostSend( cBase ) != WSA_IO_PENDING )
                pConnection->cSendQueue.RemoveSend( &cBase );
        }
        else */
        if ( pConnection->cSendQueue.RemoveSend( &cBase ) == 0 )
        {
            if ( (::InterlockedExchangeAdd( &pConnection->iReceiveClosed, 0 ) > 0) &&
                (::InterlockedExchangeAdd( &pConnection->iDisconnecting, 0 ) > 0) )
                CIocpUtils::PostDisconnect( cIocpData, (*pConnection) );
        }
    }
}

void CIocpWorkerThread::HandleAccept( CIocpBase & cBase, DWORD dwTransferred )
{
    //if ( dwTransferred > 0 )
    {
        if ( setsockopt( cBase.sSocket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char *)&cIocpData.sSocket, sizeof( SOCKET ) ) != 0 )
        {
            if ( cIocpData.pHandler )
                cIocpData.pHandler->OnError( WSAGetLastError() );
        }
        else
        {
            IocpConnInfo sConnInfo = CIocpUtils::GetConnectionInfo( cBase.sSocket );

            std::shared_ptr<CIocpConnection> pConnection( new CIocpConnection( cBase.sSocket, cIocpData.GetNextID(), cIocpData.iBufferSize ) );

            cIocpData.cConnectionManager.AddConnection( pConnection );

            CIocpUtils::CreateCompletion( (HANDLE)pConnection->sSocket, cIocpData );

            if ( cIocpData.pHandler )
                cIocpData.pHandler->OnNewConnection( pConnection->iID, sConnInfo );

            if ( CIocpUtils::PostReceive( pConnection->sReceive ) != WSA_IO_PENDING )
                if ( pConnection->CloseReceive() )
                    CIocpUtils::PostDisconnect( cIocpData, (*pConnection) );
        }

        //cBase.sSocket = INVALID_SOCKET;
        //CIocpUtils::PostAccept( cIocpData );

        
        if ( (cBase.sSocket = CIocpUtils::CreateOverlappedSocket()) != INVALID_SOCKET )
            CIocpUtils::PostAccept( cIocpData );
        else if ( cIocpData.pHandler )
            cIocpData.pHandler->OnError( WSAGetLastError() );
        


    }
}

void CIocpWorkerThread::HandleDisconnect( CIocpBase & cBase )
{
    INT64 iBaseID = cBase.iID;

    delete & cBase;

    if ( auto pConnection = cIocpData.cConnectionManager.GetConnection( iBaseID ) )
    {
        if ( pConnection->HasOutstanding() == false )
        {
            if ( cIocpData.cConnectionManager.RemoveConnection( iBaseID ) )
                if ( cIocpData.pHandler )
                    cIocpData.pHandler->OnDisconnect( iBaseID, 0 );
        }
    }
}

};