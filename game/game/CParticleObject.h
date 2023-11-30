#pragma once

#include "CParticleEmitter.h"

class CParticleObject
{
	friend class CParticleSystem;

public:
	CParticleObject();
	virtual ~CParticleObject();

	void									AddObject( CParticle * pcParticleEmitter );

private:

	std::vector<CParticle*>			vParticles;
};

