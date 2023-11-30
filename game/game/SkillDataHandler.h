#pragma once
class CSkillDataHandler
{
private:
	int							OnFighterSkill( Skill * psSkill, int iLevel );


	int							OnAtalantaSkill( Skill * psSkill, int iLevel );
	int							OnKnightSkill( Skill * psSkill, int iLevel );
	int							OnPriestessSkill( Skill * psSkill, int iLevel );

public:
	CSkillDataHandler();
	virtual ~CSkillDataHandler();

	BOOL						IsSkillLocked( int iID );

	int							OnInformationHandler( Skill * psSkill, int iLevel );
};

