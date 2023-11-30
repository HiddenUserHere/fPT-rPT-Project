#pragma once
class CSesmicImpactHandler
{
public:
	CSesmicImpactHandler();
	virtual ~CSesmicImpactHandler();


	bool							OnCastAnimationStart( struct Skill * psSkill, UnitData * pcUnitData );

	BOOL							OnBegin( class UnitData * pcThis, int iSkillID, int iLevel, class UnitData * pcUnitData, int iX, int iY, int iZ );

	BOOL							OnUse();

	BOOL							OnEvent( UnitData * pcUnitData );

	void							LoadParticle();

private:
	BOOL							CanUse( struct Skill * psSkill );


	int								iParticleID = -1;
};

