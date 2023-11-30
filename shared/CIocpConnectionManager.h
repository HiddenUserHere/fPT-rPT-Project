#pragma once

#include "CIocpConnection.h"

namespace IOCP
{

class CIocpConnectionManager
{
public:
    void    AddConnection( std::shared_ptr<CIocpConnection> cConnection );

    bool    RemoveConnection( INT64 iID );

    std::shared_ptr<CIocpConnection> GetConnection( INT64 iID );

    void    CloseAllConnections();

private:
    typedef std::map<INT64, std::shared_ptr<CIocpConnection>> ConnectionMap_t;

    ConnectionMap_t sConnectionMap;

    std::mutex  sMutex;
};

};