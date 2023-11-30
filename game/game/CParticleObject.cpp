#include "stdafx.h"
#include "CParticleObject.h"


CParticleObject::CParticleObject()
{
}


CParticleObject::~CParticleObject()
{
}

void CParticleObject::AddObject( CParticle * pcParticleEmitter )
{
	vParticles.push_back( pcParticleEmitter );
}
