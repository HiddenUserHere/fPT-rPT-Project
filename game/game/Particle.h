#pragma once

struct ParticleDataOld
{
	int						iID;
	int						iParticleUseID;
	int						iParticleID;

	BOOL					bFixed;
	BOOL					bWeapon;

	float					fScale;

	UnitData				* pcUnitData;

	float					fTime;
};

class CParticleOld
{
private:
	IMPFNC						pfnSetParticleSkillEffect = 0x005B6F60;

	std::vector<ParticleDataOld*>   vParticleData;


	int							load( const char * File );
	int							start( const int ParticleID, int X, int Y, int Z );
public:
	CParticleOld();
	int							Load( const char * File );

	int							Start( const int ParticleID, int X, int Y, int Z );
	int							Start( const char * pszName, int X, int Y, int Z );
	int							StartPTMode( const char * pszName, Point3D sPosition, float fStartDelay = 0.0f );
	void 						SetAttachPosition( int PtrIDParticle, D3DVECTOR pos );
	void 						SetAttachPosition( int PtrIDParticle, X3D::Vector3 pos );
	void 						SetPosition( int PtrIDParticle, D3DVECTOR pos );
	void 						SetPosition( int PtrIDParticle, X3D::Vector3 pos );
	void						SetAngle( int iPtr, Point3D angle );

	void						SetRendering( int iPtr, int iShow );

	bool						IsRunning( int iPtr );

	void						Stop( int iStartID );

	void						SetFastStop( int iID );

	BOOL						IsLost( ParticleDataOld * ps );

	void						Update( float fTime );

	void						AddScript( int iID, int iParticleID, UnitData * pcUnitData = NULL, float fTimeLeft = 1.0f, BOOL bFixed = FALSE, BOOL bWeapon = FALSE, float fScale = 0.0f );

	BOOL						Delete( int iUnitDataID, int iParticleID );

	static void					StartEffectParticleSkill( char * pszParticle, UnitData * pcTarget, int iSkillID = 0 );

	~CParticleOld();
};

typedef std::shared_ptr<CParticleOld> ParticleOld_ptr;
typedef CParticleOld ParticleOld;