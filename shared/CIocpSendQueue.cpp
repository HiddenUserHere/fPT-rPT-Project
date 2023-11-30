#include "stdafx.h"
#include "CIocpSendQueue.h"

namespace IOCP
{

CIocpSendQueue::CIocpSendQueue()
{
}

CIocpSendQueue::~CIocpSendQueue()
{
    CloseAllSends();
}

void CIocpSendQueue::AddSend( std::shared_ptr<CIocpBase> cSend )
{
    std::scoped_lock<std::mutex> l( sMutex );

    sSendBaseMap.insert( std::make_pair( cSend.get(), cSend ) );
}

int CIocpSendQueue::RemoveSend( CIocpBase * pcSend )
{
    std::scoped_lock<std::mutex> l( sMutex );

    sSendBaseMap.erase( pcSend );

    return sSendBaseMap.size();
}

CIocpBase * CIocpSendQueue::GetNext()
{
    SendBaseMap_t::iterator itr = sSendBaseMap.begin();
    return itr->first;
}

void CIocpSendQueue::CloseAllSends()
{
    std::scoped_lock<std::mutex> l( sMutex );

    SendBaseMap_t::iterator itr = sSendBaseMap.begin();
    while ( itr != sSendBaseMap.end() )
    {
        if ( itr->second->hEvent != INVALID_HANDLE_VALUE )
        {
            CloseHandle( itr->second->hEvent );
            itr->second->hEvent = INVALID_HANDLE_VALUE;
        }
        ++itr;
    }
}

UINT CIocpSendQueue::NumOutstanding()
{
    std::scoped_lock<std::mutex> l( sMutex );

    return sSendBaseMap.size();
}

};