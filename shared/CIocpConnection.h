#pragma once

#include "CIocpBase.h"
#include "CIocpSendQueue.h"

namespace IOCP
{

class CIocpConnection
{
public:
    CIocpConnection( SOCKET _s, INT64 _iID, UINT _iBufferSize );
    ~CIocpConnection();

    bool        CloseReceive();

    std::shared_ptr<CIocpBase>  CreateSend();

    bool        HasOutstanding();

    SOCKET      sSocket;

    INT64       iID;

    UINT        iBufferSize;

    LONG        iDisconnecting;
    LONG        iSendClosing;
    LONG        iReceiveClosed;

    CIocpBase   sReceive;
    CIocpBase   sDisconnect;

    CIocpSendQueue  cSendQueue;

    std::mutex  sMutexReceive;
    std::mutex  sMutexSend;
};

};