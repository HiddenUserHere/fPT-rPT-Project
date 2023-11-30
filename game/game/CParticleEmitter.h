#pragma once

#include "CParticle.h"

class CParticleSystem;
class CParticleEmitter;

enum EParticleEmitterType
{
	PARTICLEEMITTERTYPE_Follow,
	PARTICLEEMITTERTYPE_Free,
};

class CParticleEmitter
{
	friend class CParticleSystem;
	friend class CParticleFactory;

public:
							  CParticleEmitter( CParticleSystem * _pcParticleSystem );
							  CParticleEmitter( CParticleSystem * _pcParticleSystem, CParticleEmitter & c );
	virtual					 ~CParticleEmitter();

	void					  Birth();

	BOOL					  Update( double fTime );

	void					  SetOwner( Engine::ObjectOwner * pNewOwner ) { pOwner = pNewOwner->Clone(); }

	void					  SetName( std::string _strName );

	void					  SetActive( BOOL b ) { bActive = b; }

protected:
	void					  Emit( double fUpdateTime );

protected:

	BOOL					  bActive;

	std::string				  strName;

	CParticleSystem			* pcParticleSystem;

	EParticleEmitterType	  eType;

	CParticle::Point		  sPosition;
	CParticle::Point		  sVelocity;

	CParticle::Rotation		  sRotation;
	CParticle::Rotation		  sAngularVelocity;

	CParticle::Time			  sCurDelay;
	CParticle::Time			  sMaxDelay;

	CParticle::Time			  sCurLifeTime;
	CParticle::Time			  sMaxLifeTime;

	CParticle::Int			  sParticles;
	CParticle::Dbl			  sInterval;

	Engine::ObjectOwner		* pOwner;

	double					  fLastEmit;
};