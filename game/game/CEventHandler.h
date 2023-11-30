#pragma once
class CEventHandler
{
public:
    CEventHandler();
    virtual ~CEventHandler();

    void                    Init();

    void                    HandlePacket( PacketEventServer * psPacket );

    PacketEventServer::EventData & GetData() { return sEventData; };

private:
    PacketEventServer::EventData    sEventData;

    bool                    bPlaceTimer = false;

    int iTimerID            = 0;

    void                    AddTimer();

    bool                    HaveActiveEvent();
};

