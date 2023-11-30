#pragma once

#include "CIocpHandlerImpl.h"
#include "CIocpImpl.h"
#include "CIocpWorkerThread.h"

namespace IOCP
{

class CIocpWorkerThread;

class CIocpServer : public CIocpImpl
{
public:
    CIocpServer( int iPort, CIocpHandlerImpl * pIocpImpl, UINT iAddrToListen, UINT iBufferSize, UINT iNumThreads );

    virtual ~CIocpServer();

    void            Init( int iPort, UINT iAddrToListen, UINT iBufferSize, UINT iNumThreads );
    void            UnInit();

    void            Send( INT64 iID, std::vector<char> & vData );
    void            Send( INT64 iID, char * pBuffer, UINT iLength );

    void            Shutdown( INT64 iID, int iHow );

    void            Disconnect( INT64 iID );

private:
    void            InitializeThreadPool( UINT iNumThreads );
    void            InitializeIocp();
    void            InitializeWinsock();
    void            InitializeSocket( UINT iAddrToListen, int iPort );
    void            InitializeAcceptEvent();

    typedef std::vector<std::shared_ptr<CIocpWorkerThread>> ThreadPool_t;
    ThreadPool_t vThreadPool;

    std::vector<char> vOutputBuffer;
};

};
