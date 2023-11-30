#pragma once
class CGlamorousBrushHandler
{
public:
    CGlamorousBrushHandler();
    virtual ~CGlamorousBrushHandler();

    void                    HandlePacket( User * pcUser, PacketGlamorousBrush * psPacket );

private:
    void                    ProcessPacket( User * pcUser, PacketGlamorousBrush * psPacket );

    void                    ClearItemStats( Item & sItem );
};

