#pragma once
class CHonorHandler
{
public:
    CHonorHandler();
    virtual ~CHonorHandler();

    void                    OnLoadUser( User * pcUser );
    void                    OnUnloadUser( User * pcUser );

    void                    SQLAddPvPHonor( int iCharacterID, PacketHonor::EHonorType eType, UINT uTimeLeft );
    void                    SQLAddPvPHonor( User * pcUser, PacketHonor::EHonorType eType, UINT uTimeLeft );
    void                    SQLUpdatePvPHonor( User * pcUser );
    void                    SQLDeletePvPHonor( User * pcUser );

    void                    SQLAddBellatraHonor( int iCharacterID, PacketHonor::EHonorType eType, UINT uTimeLeft );
    void                    SQLAddBellatraHonor( User * pcUser, PacketHonor::EHonorType eType, UINT uTimeLeft );
    void                    SQLUpdateBellatraHonor( User * pcUser );
    void                    SQLDeleteBellatraHonor( User * pcUser );

    void                    Update( ETickType eTick );

    void                    SendHonors( User * pcUser );
    void                    UpdateHonor();

    void                    NetHandlePacket( PacketNetUpdatePvPBellatraBuff * psPacket );

private:
    struct HonorTopClass3
    {
        int                 iaCharacterID[10][3];
    };

    void                    ProcessPacket( User * pcUser, PacketHonor * psPacket );

    bool                    SQLGetPvPTop( HonorTopClass3 & sCharacters );
    bool                    SQLGetBellatraTop( HonorTopClass3 & sCharacters );
    void                    SQLLog( int iCharacterID, PacketHonor::EHonorType eHonorType, bool bPvP );

    void                    SQLResetHonors();
    void                    SQLResetBellatraPlayer();

    WORD                    wDay = 0;
};

