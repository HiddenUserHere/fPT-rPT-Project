#pragma once

#include "CIocpData.h"

namespace IOCP
{

struct IocpConnInfo
{
    std::string     strIP;
    std::string     strHostName;
    int             iPort;
};

class CIocpUtils
{
public:
    static UINT StringIPToNumberIPV4( const std::string strIP );

    static std::string NumberIPToStringIPV4( const UINT uIP );

    static SOCKET   CreateOverlappedSocket();

    static IocpConnInfo GetConnectionInfo( SOCKET _s );

    static void     PostAccept( CIocpData & cIocpData );

    static int      PostReceive( CIocpBase & cBase );

    static int      PostSend( CIocpBase & cBase );
    static int      PostSendQueue( CIocpData & cIocpData, CIocpBase & cBase );

    static void     CreateCompletion( HANDLE _hHandle, CIocpData & cIocpData );

    static HANDLE   CreateIocp( int iNumThreads );

    static int      GetNumIocpThreads();

    static int      PostDisconnect( CIocpData & cIocpData, CIocpConnection & cConnection );

    static LPFN_ACCEPTEX    LoadAcceptEx( SOCKET _s );
};

};