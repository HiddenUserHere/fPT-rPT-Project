#pragma once

class CWizardTrance
{
private:
	int							iTimerID = 0;
	int							iParticleID = -1;
	int							iParticleKeepID = -1;

	void						AddTimer( int iTime, int iLevel );
public:
	//Constructor
								CWizardTrance();
	virtual					   ~CWizardTrance();

	void						LoadParticle();

	bool						OnCastAnimationStart( Skill * psSkill, UnitData * pcUnitData );

	BOOL						OnEvent( UnitData * pcUnitData );
	void						OnCastEffect( Unit * pcUnit, UINT uSkillLevel, double fOverTime );
	void						OnCastEnd( Unit * pcUnit );
};