#pragma once
class CPressDeityHandler
{
public:
    CPressDeityHandler();
    virtual ~CPressDeityHandler();

    void                    LoadParticle();

    bool					OnCastAnimationStart( Skill * psSkill, UnitData * pcUnitData );
    BOOL					OnEvent( UnitData * pcUnitData );

private:
    int                     iParticleOneID = -1;
    int                     iParticleTwoID = -1;

    void                    SpawnEffect( UnitData * pcUnitData, bool bFinalEffect );
};

