#pragma once
class CAkatsukiHandler
{
public:
    CAkatsukiHandler();
    virtual ~CAkatsukiHandler();

    void                        Init();

    void                        Update( float fTime );

    void                        VerifyUnit( UnitData * pcUnitData );

    bool                        IsActiveMusicTheme() { return (iBackgroundMusicID == BACKGROUNDMUSICID_AkatsukiTheme); }

    void                        UpdateVerify( bool bReset );

private:
    bool                        bHasUserCostume = false;

    DWORD                       dwTimeUpdate = 0;

    int                         iBackgroundMusicID = BACKGROUNDMUSICID_Login;
};

