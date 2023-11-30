#pragma once

class CIceElementalHandler
{
private:
	int							iTimerID = 0;

	void						AddTimer( int iTime, int iLevel );

public:
								CIceElementalHandler();
	virtual					   ~CIceElementalHandler();

	bool						OnCastAnimationStart( Skill * psSkill, UnitData * pcUnitData );
};