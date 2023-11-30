#pragma once
class CTimePointsHandler
{
protected:
    const int                   TIMECOINS_POINTS_MINUTES = 1;
public:
    CTimePointsHandler();
    virtual ~CTimePointsHandler();

    void                        OnLoadUser( User * pcUser );
    void                        OnUnloadUser( User * pcUser );

    void                        Update( ETickType eTick );

private:
    bool                        SQLHasUser( User * pcUser );
};

