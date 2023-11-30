#pragma once

#include "CParticleModifier.h"

enum EParticleModifierGroupType
{
	PARTICLEMODIFIERGROUPTYPE_Size		= 0,
	PARTICLEMODIFIERGROUPTYPE_Color		= 1,
};

class CParticleModifierGroup
{
public:
								  CParticleModifierGroup( EParticleModifierGroupType _eType );
								  CParticleModifierGroup( CParticleModifierGroup & c );
	virtual						 ~CParticleModifierGroup();

	void						  Apply( CParticle * pcParticle );

	void						  AddModifier( CParticleModifier * pcModifier ) { vModifiers.push_back( pcModifier ); }
	
	EParticleModifierGroupType	  GetType() { return eType; }

protected:
	EParticleModifierGroupType	  eType;

	std::vector<CParticleModifier*>	  vModifiers;

	CParticleModifier			* pcLast1;
	CParticleModifier			* pcLast2;
	int							  iLastDiffP;

};