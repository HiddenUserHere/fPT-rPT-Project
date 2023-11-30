#pragma once
class CFlameBallHandler
{
public:
    CFlameBallHandler();
    virtual ~CFlameBallHandler();

    bool					OnCastAnimationStart( Skill * psSkill, UnitData * pcUnitData );

    BOOL					OnEvent( UnitData * pcUnitData );

};

