#include "stdafx.h"
#include "gameskill.h"


CGameSkill::CGameSkill()
{
}


CGameSkill::~CGameSkill()
{
}

NAKED void CGameSkill::OnMouseLDownClick( int iX, int iY )
{
	JMP( pfnOnMouseLClickDown );
}

NAKED void CGameSkill::FrozenMistHit( UnitData * pcUnitData, UnitData * pcTarget )
{
	JMP( 0x005B08C0 );
}

NAKED void CGameSkill::FrozenMistSkillHit( UnitData * pcUnitData, UnitData * pcTarget )
{
	JMP( 0x005B0950 );
}

NAKED void CGameSkill::DarkGuardianHit( UnitData * pcUnitData )
{
	JMP( 0x004095E8 );
}

NAKED void CGameSkill::DarkGuardianSkillHit( UnitData * pcUnitData )
{
	JMP( 0x005BB7D0 );
}

NAKED void CGameSkill::GoblinShamanSkill( UnitData * pcUnitData, int iRange )
{
	JMP( 0x00623C80 );
}

NAKED BOOL CGameSkill::IceGolemHit( UnitData * pcUnitData )
{
	JMP( 0x005B0760 );
}
