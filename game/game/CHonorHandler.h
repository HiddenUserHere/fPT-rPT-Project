#pragma once
class CHonorHandler
{
private:
    void                    AddTimer( ESkillID eSkillID, PacketHonor::EHonorType eType, UINT uTime );
public:
    CHonorHandler();
    virtual ~CHonorHandler();

    void                    HandlePacket( PacketHonor * psPacket );

    void                    Test();

private:
    int                     iTimerPvPHonorID        = -1;
    int                     iTimerBellatraHonorID   = -1;
};

