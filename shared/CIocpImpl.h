#pragma once

#include "CIocpUtils.h"
#include "CIocpData.h"

namespace IOCP
{

class CIocpImpl
{
public:
    virtual void    Send( INT64 iID, std::vector<char> & vData );
    virtual void    Send( INT64 iID, char * pBuffer, UINT iLength );

    virtual void    Shutdown( INT64 iID, int iHow );

    virtual void    Disconnect( INT64 iID );

    CIocpData       cIocpData;
};

};