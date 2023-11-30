#pragma once

class CDropItemAreaHandler
{
private:
    std::vector<ItemAreaDropData *> vItems;

public:
    CDropItemAreaHandler();
    virtual ~CDropItemAreaHandler();

    void                        Init();

    void                        OnDisconnected( User * pcUser );

    void                        RemoveItem( User * pcUser );

    BOOL                        IsExclusiveDrop( int iID, Map::Loot * psLoot, bool bParty );
    BOOL                        IsExclusiveDrop( Map::Loot * psLoot );

    void                        AddItem( User * pcUser, Map::Loot * psLoot, int iRadius, bool bParty );
    void                        AddItem( User * pcUser, Point3D sPosition, int iRadius, bool bParty );

    void					    OnLeaveParty( User * pcUser );

    void                        SendUserArea( User * pcUser );

private:
    void                        ProcessPacket( User * pcUser, PacketItemAreaDrop * psPacket );
};

