#pragma once

#include "CIocpBase.h"

namespace IOCP
{

class CIocpSendQueue
{
public:
    CIocpSendQueue();
    ~CIocpSendQueue();

    void            AddSend( std::shared_ptr<CIocpBase> cSend );
    int             RemoveSend( CIocpBase * pcSend );

    CIocpBase       * GetNext();

    void            CloseAllSends();

    UINT            NumOutstanding();

private:
    typedef std::map<CIocpBase *, std::shared_ptr<CIocpBase>> SendBaseMap_t;

    SendBaseMap_t   sSendBaseMap;

    std::mutex      sMutex;
};

};