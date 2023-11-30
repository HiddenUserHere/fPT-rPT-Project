#include "stdafx.h"
#include "SkillTimerInfo.h"


CSkillTimerInfo::CSkillTimerInfo()
{
}


CSkillTimerInfo::~CSkillTimerInfo()
{
}

Skill * CSkillTimerInfo::FindFreeSlot()
{
	Skill * psSkillTimer = (Skill*)pdwTimerArray;
	for ( int i = 0; i < iTotalTimer; i++ )
	{
		if ( !psSkillTimer[i].bActive )
			return &psSkillTimer[i];
	}
	return NULL;
}

BOOL CSkillTimerInfo::AddNewTimer( Skill * psSkill )
{
	// Free slot found?
	Skill * psSlot = FindFreeSlot();
	if ( psSlot )
	{
		// Set timer into memory
		memcpy( psSlot, psSkill, sizeof( Skill ) );
		return TRUE;
	}
	return FALSE;
}
