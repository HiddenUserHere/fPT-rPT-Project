#pragma once
class CSummonTigerHandler
{
private:
	int							iTimerID = 0;

	void						AddTimer( int iTime, int iLevel );

public:
	CSummonTigerHandler();
	virtual ~CSummonTigerHandler();

	bool						OnCastAnimationStart( Skill * psSkill, UnitData * pcUnitData );
};

