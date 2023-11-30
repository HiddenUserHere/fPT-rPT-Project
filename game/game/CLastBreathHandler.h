#pragma once
class CLastBreathHandler
{
public:
	CLastBreathHandler();
	virtual ~CLastBreathHandler();

	bool							OnCastAnimationStart( Skill * psSkill, UnitData * pcUnitData );

	BOOL							OnEvent( UnitData * pcUnitData );

	void							AddTarget( Unit * pcUnit );


private:
};

