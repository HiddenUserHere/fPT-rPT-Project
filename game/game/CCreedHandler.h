#pragma once
class CCreedHandler
{
private:
	void							AddCreedKeep( UnitData * pcUnitData );
	void							AddCreedTimer( int iTime, int iLevel );

public:
	CCreedHandler();
	virtual ~CCreedHandler();


	bool							OnCastAnimationStart( Skill * psSkill, UnitData * pcUnitData );
	bool							OnCastAnimationStart( Skill * psSkill );
	BOOL							OnCast( int iLevel );
	BOOL							OnCastEnd();

	BOOL							OnBeginSkillUse( UnitData * pcThis, UnitData * pcUnitData, int iX, int iY, int iZ );

};

