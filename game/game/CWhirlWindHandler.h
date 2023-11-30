#pragma once
class CWhirlWindHandler
{
public:
	CWhirlWindHandler();
	virtual ~CWhirlWindHandler();

	bool					OnCastAnimationStart( Skill * psSkill, UnitData * pcUnitData );

	BOOL					OnEvent( UnitData * pcUnitData );
	BOOL					OnAnimationEnd( UnitData * pcUnitData );
	void					OnCastEffect( Unit * pcUnit );
	void					OnCastEnd( Unit * pcUnit );

	void					Update();

private:
	int						iTimerID = 0;

	DWORD					dwTimeUpdate = 0;

	int						GetMouseAngle();

	void					AddTimer( int iTime, int iLevel );
};

