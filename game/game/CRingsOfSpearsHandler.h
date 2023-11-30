#pragma once

class CRingsOfSpearsHandler
{
public:
											CRingsOfSpearsHandler();
	virtual								   ~CRingsOfSpearsHandler();

	void									LoadParticle();
	bool									OnCastAnimationStart( struct Skill * psSkill, UnitData * pcUnitData );

	BOOL									OnBeginSkillUse( class UnitData * pcThis, class UnitData * pcUnitData, int iX, int iY, int iZ );
	BOOL									OnEvent( UnitData * pcUnitData );

private:
	BOOL									CanUse( struct Skill * psSkill );
};