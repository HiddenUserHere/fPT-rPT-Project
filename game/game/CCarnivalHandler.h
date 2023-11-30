#pragma once

#include "CCarnivalWindowHandler.h"

class CCarnivalHandler
{
private:
    CCarnivalWindowHandler * pcWindow = nullptr;

public:
    CCarnivalHandler();
    virtual ~CCarnivalHandler();

    CCarnivalWindowHandler * GetWindow() { return pcWindow; }

    void                Init();

    bool                HaveItems();
    bool                HaveFruits();

    void                ExchangeItems();

    void                HandlePacket( PacketNPCCarnival * psPacket );

    void                Update( float fTime );

private:
    void                ProcessPacket( PacketNPCCarnival * psPacket );

    bool                bActiveSound = false;

    float               fTimeSound = 0.0f;

};

