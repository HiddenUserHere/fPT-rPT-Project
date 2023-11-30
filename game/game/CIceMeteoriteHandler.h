#pragma once
class CIceMeteoriteHandler
{
public:
	CIceMeteoriteHandler();
	~CIceMeteoriteHandler();

	bool						OnCastAnimationStart( Skill * psSkill, UnitData * pcUnitData );

	BOOL						OnEvent( UnitData * pcUnitData );

	BOOL						OnAnimationEnd( UnitData * pcUnitData );
};

