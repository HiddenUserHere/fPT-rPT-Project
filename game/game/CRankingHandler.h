#pragma once

#include "CRankingLevelWindowHandler.h"

class CRankingHandler
{
private:
    CRankingLevelWindowHandler      * pcWindowRankingLevel = nullptr;

public:
    CRankingHandler();
    virtual ~CRankingHandler();

    void                            Init();

    void                            Update( float fTime );

    void                            Render2D();

    void							OnMouseMove( class CMouse * pcMouse );
    BOOL							OnMouseClick( class CMouse * pcMouse );
    BOOL							OnMouseScroll( class CMouse * pcMouse );

    BOOL							OnKeyPress( class CKeyboard * pcKeyboard );
    BOOL							OnKeyChar( class CKeyboard * pcKeyboard );

    void                            OnResolutionChanged();

    void                            HandlePacket( PacketRankingLevel * psPacket );
};

