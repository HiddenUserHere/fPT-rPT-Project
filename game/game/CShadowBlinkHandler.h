#pragma once
class CShadowBlinkHandler
{
private:
	BOOL							bActive = FALSE;

	int								iMoveCount = 0;

public:
									CShadowBlinkHandler();
	virtual						   ~CShadowBlinkHandler();

	void							LoadParticle();
	bool							OnCastAnimationStart( Skill * psSkill, UnitData * pcUnitData );
	BOOL							OnEvent( UnitData * pcUnitData );

	void							Update();
};

