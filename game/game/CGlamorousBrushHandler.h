#pragma once
class CGlamorousBrushHandler
{
public:
    CGlamorousBrushHandler();

    ~CGlamorousBrushHandler();

    void                        Init();

    bool                        IsGlamorousMouse();

    void                        OnItemBrush( ItemData * pcItemData );

    BOOL                        OnMouseClick( CMouse * pcMouse );

    void                        Update( float fTime );

    void                        HandlePacket( PacketGlamorousBrush * psPacket );

    void                        Render();

private:
    EItemID                     eGlamorousItemID;
    int                         iGlamorousChk1;
    int                         iGlamorousChk2;

    EItemID                     eItemID;
    int                         iChk1;
    int                         iChk2;

    float                       fTimeWait = 0.0f;
    float                       fTimeAnimation = 0.0f;

    Rectangle2D                 sRectangleItem;

    UI::Image                   * pcImageAnimation = nullptr;

    void                        OnAcceptItem( ItemData * pcItemData );

    void                        ProcessPacket( PacketGlamorousBrush * psPacket );

    bool                        IsValidItem( ItemData * pcItemData, bool bMessage );

};

