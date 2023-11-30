#pragma once
class CGameSkillHandlerOld
{
    CGameSkillHandlerOld();
    virtual ~CGameSkillHandlerOld();

    int                 OpenPlaySkill( Skill * psSkill );

    BOOL                BeginSkill( int iSkillID, int iLevel, UnitData * pcTarget, int iX, int iY, int iZ );

    BOOL                AddTriumphOfValhalla( int iSkillLevel, int iLevel );
};

