#pragma once

#include "CIocpBase.h"

namespace IOCP
{

class CIocpImpl;

struct IocpConnInfo;

class CIocpHandlerImpl
{
public:

    CIocpImpl * pcHandler = nullptr;

    virtual void        OnNewConnection( INT64 iID, const IocpConnInfo & cConnection ) {};

    virtual UINT        OnReceive( INT64 iID, std::vector<char> vData, UINT iLengthReceived ) { return iLengthReceived; };

    virtual void        OnSent( CIocpBase & cBase, INT64 iTotalSent ) {};

    virtual void        OnClientDisconnect( INT64 iID, UINT iErrorCode ) {};

    virtual void        OnDisconnect( INT64 iID, UINT iErrorCode ) {};

    virtual void        OnError( UINT iErrorCode ) {};

    virtual void        OnClose( UINT iErrorCode ) {};

    void                SetHandler( CIocpImpl * _pcHandler ) { pcHandler = _pcHandler; };
};

};