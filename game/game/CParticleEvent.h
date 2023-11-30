#pragma once

#include "CParticle.h"

class CParticleEvent
{
public:
							  CParticleEvent( double f );
							  CParticleEvent( CParticleEvent & c );
	virtual					 ~CParticleEvent();

	virtual CParticleEvent	* Clone() = 0;

	virtual void			  Apply( CParticle * pcParticle ) = 0;

	double					  GetTime() { return fTime; }

protected:
	double					  fTime;

};

class CParticleEventVelocity : public CParticleEvent
{
public:
							  CParticleEventVelocity( CParticle::Point v, double f );
							  CParticleEventVelocity( CParticleEventVelocity & c );
	virtual					 ~CParticleEventVelocity();

	CParticleEventVelocity	* Clone();

	void					  Apply( CParticle * pcParticle );

protected:
	CParticle::Point		  sVelocity;

};

class CParticleEventSize : public CParticleEvent
{
public:
							  CParticleEventSize( CParticle::Size s, double f );
							  CParticleEventSize( CParticleEventSize & c );
	virtual					 ~CParticleEventSize();

	CParticleEventSize		* Clone();

	void					  Apply( CParticle * pcParticle );

protected:
	CParticle::Size			  sSize;

};