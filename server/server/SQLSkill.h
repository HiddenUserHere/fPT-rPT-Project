#pragma once
class CSQLSkill
{
public:
	CSQLSkill();
	int								GetSkillValueInt( const char * SQLCommand, ... );
	float							GetSkillValueFloat( const char * SQLCommand, ... );
	int								GetMinMaxInt( int iMin, int iMax, int iPercent );
	~CSQLSkill();

};

