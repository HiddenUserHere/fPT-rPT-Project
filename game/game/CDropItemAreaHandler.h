#pragma once

#define DROPAREACIRCLEID_ForestDungeon          0x1001

class CDropItemAreaHandler
{
public:
    CDropItemAreaHandler();
    virtual ~CDropItemAreaHandler();

    void                    Init();

    void                    Clear();

    void                    AddCircle( int iID, Point3D sPosition, int iRadius, bool bNoTime );

    void                    Render3D();

    void                    Update( float fTime );

    void                    HandlePacket( PacketItemAreaDrop * psPacket );

    void                    KillCircle( int iID );

private:
    struct CircleArea
    {
        int                 iID;
        Point3D             sPosition;
        int                 iRadius;

        float               fTimeUpdate;

        float               fTimeEase;

        bool                bNoTime;
    };

    CircleArea              * GetCircle( int iID );

    std::vector<CircleArea *> vCircles;

    bool                   bCanRender = false;

    PTModel                * pcModel = nullptr;

    int                    iRadius = 0;
};

