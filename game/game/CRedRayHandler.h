#pragma once

class CRedRayHandler
{
public:
											CRedRayHandler();
	virtual								   ~CRedRayHandler();

	bool									OnCastAnimationStart( struct Skill * psSkill, UnitData * pcUnitData );

	BOOL									OnEvent( UnitData * pcUnitData );

private:
	BOOL									CanUse( struct Skill * psSkill );
};