#pragma once
class CKissViperHandler
{
public:
    CKissViperHandler();
    virtual ~CKissViperHandler();

    void                    AddTimer( int iTime, int iLevel );

    bool					OnCastAnimationStart( Skill * psSkill );

    BOOL					OnBeginSkillUse( UnitData * pcThis, class UnitData * pcUnitData, int iX, int iY, int iZ );

    void					OnCastEffect( Unit * pcUnit, UINT uSkillLevel, double fOverTime, bool bSelfCast );
    void					OnCastEnd( Unit * pcUnit );

    bool                    IsOwner() { return _bSelfCast; }

private:
    int                     iTimerID = 0;

    bool                    _bSelfCast = false;
};

