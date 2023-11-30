#pragma once
class CChasingHuntHandler
{
public:
	CChasingHuntHandler();
	virtual ~CChasingHuntHandler();

	bool						OnCastAnimationStart( Skill * psSkill );
};

