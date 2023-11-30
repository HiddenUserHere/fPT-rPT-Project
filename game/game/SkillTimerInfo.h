#pragma once
class CSkillTimerInfo
{
private:
	struct Skill							* FindFreeSlot();
	const int								iTotalTimer		= 20;
	static const DWORD						pdwTimerArray	= 0x036789F8;

public:
	CSkillTimerInfo();
	~CSkillTimerInfo();

	void									Init();

	BOOL									AddNewTimer( struct Skill * psSkill );

	
};

