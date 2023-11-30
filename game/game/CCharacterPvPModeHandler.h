#pragma once
class CCharacterPvPModeHandler
{
public:
    CCharacterPvPModeHandler();
    virtual ~CCharacterPvPModeHandler();

    void                Init();

    BOOL                OnKeyChar( CKeyboard * pcKeyboard );

    void                Kill( bool bForce = false );

    int                 OnRenderTime( Skill * psSkill );

    int                 GetTimeLeft( bool bEnable = false );

    BOOL                IsPKMap( int iMapID );

    void                OnUpdateMap( int iMapID );

    void                RenderHeaderTopPK( Unit * pcUnit, int iLastHeight );

private:
    const UINT          MAX_PKMODE_DISABLE_TIME = 1 * 60 * 1000;
    const UINT          MAX_PKMODE_ENABLE_TIME  = 1 * 60 * 1000;

    void                AddTimer();

    DWORD               dwTimeUpdate = 0;

    int                 iTimerID = 0;

    UI::Image           * pcImage = nullptr;
};

