#pragma once

#include "CParticle.h"

class CParticleModifier
{
public:
								  CParticleModifier( double f );
								  CParticleModifier( CParticleModifier & c );
	virtual						 ~CParticleModifier();

	virtual CParticleModifier	* Clone() = 0;

	virtual void				  Apply( CParticle * pcParticle ) = 0;
	virtual void				  Apply( CParticle * pcParticle, CParticleModifier * pcOther, int iDiffP ) = 0;

	double						  GetTime() { return fTime; }

protected:
	double						  fTime;

};

class CParticleModifierSize : public CParticleModifier
{
public:
								  CParticleModifierSize( CParticle::Size s, double f );
								  CParticleModifierSize( CParticleModifierSize & c );
	virtual						 ~CParticleModifierSize();

	CParticleModifierSize		* Clone();

	void						  Apply( CParticle * pcParticle );
	void						  Apply( CParticle * pcParticle, CParticleModifier * pcOther, int iDiffP );

protected:
	CParticle::Size				  sSize;

};

class CParticleModifierColor : public CParticleModifier
{
public:
								  CParticleModifierColor( CParticle::Color c, double f );
								  CParticleModifierColor( CParticleModifierColor & c );
	virtual						 ~CParticleModifierColor();

	CParticleModifierColor		* Clone();

	void						  Apply( CParticle * pcParticle );
	void						  Apply( CParticle * pcParticle, CParticleModifier * pcOther, int iDiffP );

protected:
	CParticle::Color			  sColor;

};