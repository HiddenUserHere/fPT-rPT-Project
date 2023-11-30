#pragma once
class CStoneSkinHandler
{
public:
	CStoneSkinHandler();
	virtual ~CStoneSkinHandler();


	void						LoadParticle();

	bool						OnCast( struct Skill * psSkill, UnitData * pcUnitData );

	BOOL						OnAnimationEnd( class UnitData * pcUnitData );

private:

	int							iParticleID = -1;
};

