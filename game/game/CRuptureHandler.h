#pragma once

class CRuptureHandler
{
public:
											CRuptureHandler();
	virtual								   ~CRuptureHandler();

	void									LoadParticle();
	bool									OnCastAnimationStart( struct Skill * psSkill, UnitData * pcUnitData );

	BOOL									OnEvent( UnitData * pcUnitData );

private:
	BOOL									CanUse( struct Skill * psSkill );
};