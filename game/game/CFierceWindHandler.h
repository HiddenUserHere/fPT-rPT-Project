#pragma once
class CFierceWindHandler
{
private:
	IMPFNC													pfnSetStormProjectile = 0x005BE560;

	int														iParticleID;
	int														iParticleAID;
	int														iParticleProjectileID;

	static BOOL												SetProjectile( Point3D * psPosition, Point3D * psTargetPosition );

public:
	CFierceWindHandler();
	virtual ~CFierceWindHandler();

	void													LoadParticle();

	void													RenderEffect( class UnitData * pcUnitData );

	void													OnKeep( class UnitData * pcUnitData );

	bool													OnCastAnimationStart( struct Skill * psSkill, class UnitData * pcUnitData );
	BOOL													OnCastMember( class UnitData * pcUnitData );
	BOOL													OnCastEffect( class Unit * pcUnit, short sTimeLeft );
	BOOL													OnCastEnd( Unit * pcUnit );

	BOOL													OnEvent( UnitData * pcUnitData );

	BOOL													OnAnimationEnd( UnitData * pcUnitData );
};

