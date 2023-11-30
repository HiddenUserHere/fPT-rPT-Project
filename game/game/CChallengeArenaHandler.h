#pragma once
class CChallengeArenaHandler
{
public:
    CChallengeArenaHandler();
    virtual ~CChallengeArenaHandler();

    void                        Init();

    void                        Update( float fTime );

    void                        HandlePacket( PacketChallengeRequest * psPacket );
};

