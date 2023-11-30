#pragma once

#include "CParticleFactory.h"
#include "CParticleDebug.h"

#include "DXGraphicEngine.h"

#define PARTICLEFACTORY			(PARTICLEENGINE->GetParticleFactory())
#define PARTICLEDEBUG			(PARTICLEENGINE->GetParticleDebug())

class CParticleEngine
{
	friend class CParticleDebug;

public:
								  CParticleEngine();
	virtual						 ~CParticleEngine();

	BOOL						  Init();
	void						  Shutdown();

	void						  Update( double fTime );

	void						  Render3D( DXGraphicEngine * pGraphicEngine, Point3D sCameraPos, Point3D sCameraAngle );
	void						  Render2D( DXGraphicEngine * pGraphicEngine );

	CParticleFactory			* GetParticleFactory() { return pcParticleFactory; }
	CParticleDebug				* GetParticleDebug() { return pcParticleDebug; }

	UINT						  Add( CParticleSystem * pcParticleSystem, double fOverTime = 0.0f );
	void						  AddScript( CParticleScript * pcParticleScript, double fOverTime = 0.0f );

	void						  Pause( UINT iID );
	void						  Continue( UINT iID );

	void						  Kill( UINT uParticleID );
	void						  Kill( UINT uParticleID, UINT uOwnerID );
	void						  KillAll();

	UINT						  NumberOfActiveParticleSystems() { return vParticleSystems.size(); }

protected:
	CParticleFactory			* pcParticleFactory;
	CParticleDebug				* pcParticleDebug;

	class EXETextureManager		* pcTextureManager;

	std::vector<CParticleSystem*>	  vParticleSystems;

private:
	CMutex						* pcMutex;

};