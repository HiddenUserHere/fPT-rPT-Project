#pragma once

#include "CIocpHandlerImpl.h"
#include "CIocpImpl.h"
#include "CIocpWorkerThread.h"

namespace IOCP
{

class CIocpClient : public CIocpImpl
{
public:
    CIocpClient();
    ~CIocpClient();

    void            Init( UINT iBufferSize, UINT iNumThreads );
    void            UnInit();

    void            Send( INT64 iID, std::vector<char> & vData ) override;
    void            Send( INT64 iID, char * pBuffer, UINT iLength ) override;

    void            Shutdown( INT64 iID, int iHow );

    void            Disconnect( INT64 iID );

    bool            Connect( const std::string strIP, int iPort );

private:
    void            InitializeThreadPool( UINT iNumThreads );
    void            InitializeIocp();
    void            InitializeWinsock();
    void            InitializeSocket( UINT iAddrToListen, int iPort );


    typedef std::vector<std::shared_ptr<CIocpWorkerThread>> ThreadPool_t;
    ThreadPool_t vThreadPool;

    std::vector<char> vOutputBuffer;
};

};