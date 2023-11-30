#pragma once

struct MountMonster
{
    int                     iID;

    std::string             strName;

    std::string             strImageFile;

    EMountType              eMountType;

    EMountRareType          eRareType;
};




class CMountServerHandler
{
private:
    std::vector<MountMonster *>     vMountsList;

public:
    CMountServerHandler();
    virtual ~CMountServerHandler();

    void                    Clear();

    void                    Init();

    void                    OnLoadUser( User * pcUser );

    void                    OnUnloadUser( User * pcUser );

    void                    OnUpdate( User * pcUser );

    void                    HandlePacket( User * pcUser, PacketMountUserList * psPacket );

    bool                    HandleItem( User * pcUser, EItemID eItemID, int iChk1, int iChk2 );

private:
    void                    SQLLoadMounts();

    bool                    SQLGetUserMounts( User * pcUser, std::vector<MountUserListData> & vMounts );
    bool                    SQLAddUserMounts( User * pcUser, std::vector<MountUserListData> & vMounts );
    bool                    SQLDeleteUserMount( User * pcUser, EMountType eMountType );
    bool                    SQLHasUserMount( int iCharacterID, EMountType eMountType );

    MountMonster            * GetMount( EMountType eMountType );

    void                    ProcessPacket( User * pcUser, PacketMountUserList * psPacket );
    void                    ProcessPacket( User * pcUser, PacketMountDelete * psPacket );
    void                    ProcessPacket( User * pcUser, PacketNewMount * psPacket );
};

