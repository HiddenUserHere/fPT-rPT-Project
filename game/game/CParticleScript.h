#pragma once

#include "CParticleSystem.h"

class CParticleEngine;

class CParticleScript
{
	friend class CParticleEngine;
	friend class CParticleFactory;

public:
								  CParticleScript( std::string _strFilePath );
								  CParticleScript( CParticleScript & c );
	virtual						 ~CParticleScript();

	std::string						  GetFilePath() { return strFilePath; }

	std::string						  GetScript() { return strScript; }
	std::string						  GetAuthor() { return strAuthor; }

	std::vector<CParticleSystem*>	& GetParticleSystems() { return vParticleSystems; }

	void						  Add( CParticleSystem * pcReadyParticleSystem );
	void						  Clear();

	void						  SetOwner( Engine::ObjectOwner * pNewOwner );

	void						  SetID( UINT uID );

	void						  RemoveParticleSystem( std::string strParticleSystem );

private:
	std::string						  strFilePath;

	std::string						  strScript;
	std::string						  strAuthor;

	CParticleSystem				* pcParticleSystem;
	std::vector<CParticleSystem*>	  vParticleSystems;

};