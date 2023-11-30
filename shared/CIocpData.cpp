#include "stdafx.h"
#include "CIocpData.h"

namespace IOCP
{

CIocpData::CIocpData() : sShutdownEvent( INVALID_HANDLE_VALUE ), sIoCompletionPort( INVALID_HANDLE_VALUE ),
sSocket( INVALID_SOCKET ), cAcceptBase( INVALID_SOCKET, 0, CIocpBase::IocpType::IOCPTYPE_Accept, 4096 ),
iBufferSize( 0 ), iID( 0 ), pfnAcceptEx( nullptr )
{
}

CIocpData::~CIocpData()
{
}

INT64 CIocpData::GetNextID()
{
    std::scoped_lock<std::mutex> l( sMutex );
    iID++;
    return iID;
}

};
