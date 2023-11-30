#pragma once

class CLethalSightHandler
{
private:
	int							iTimerID = 0;
	int							iParticleID = -1;
	int							iParticleKeepID = -1;

	void						AddTimer( int iTime, int iLevel );
public:
	//Constructor
								CLethalSightHandler();
	virtual					   ~CLethalSightHandler();

	void						LoadParticle();

	bool						OnCastAnimationStart( Skill * psSkill, UnitData * pcUnitData );

	void						OnCastEffect( Unit * pcUnit, UINT uSkillLevel, double fOverTime );
	void						OnCastEnd( Unit * pcUnit );
};