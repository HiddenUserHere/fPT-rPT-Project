#pragma once
class CGameSkill
{
protected:
	IMPFNC						pfnOnMouseLClickDown = 0x004993B0;

public:
	CGameSkill();
	~CGameSkill();

	static CGameSkill			* GetInstance() { return (CGameSkill*)0x0367E148; }

	void						OnMouseLDownClick( int iX, int iY );

	static void					FrozenMistHit( UnitData * pcUnitData, UnitData * pcTarget );
	static void					FrozenMistSkillHit( UnitData * pcUnitData, UnitData * pcTarget );
	static void					DarkGuardianHit( UnitData * pcUnitData );
	static void					DarkGuardianSkillHit( UnitData * pcUnitData );

	static void					GoblinShamanSkill( UnitData * pcUnitData, int iRange );

	static BOOL					IceGolemHit( UnitData * pcUnitData );
};

