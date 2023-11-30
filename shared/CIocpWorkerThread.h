#pragma once

#include "CIocpData.h"
#include "CIocpUtils.h"

namespace IOCP
{

class CIocpWorkerThread
{
public:
    CIocpWorkerThread( CIocpData & cSharedData );

    ~CIocpWorkerThread();

    void            Run();

private:
    void            HandleBase( CIocpBase & cBase, DWORD dwTransferred );

    void            HandleFailure( OVERLAPPED * psOverlapped, DWORD dwTransferred, UINT iErrorCode );

    void            HandleReceive( CIocpBase & cBase, DWORD dwTransferred );

    void            HandleSend( CIocpBase & cBase, DWORD dwTransferred );
    void            HandleSent( CIocpBase & cBase, DWORD dwTransferred );

    void            HandleAccept( CIocpBase & cBase, DWORD dwTransferred );

    void            HandleDisconnect( CIocpBase & cBase );

    std::thread     sThread;

    CIocpData & cIocpData;
};

};