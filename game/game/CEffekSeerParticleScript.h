#pragma once

#include "CEffekSeerParticleSystem.h"

class CEffekSeerParticleScript
{
private:
	UINT					uID = 0xFFFFFFFF;

	BOOL					bCache = FALSE;

	std::string				strFileName;

	Effekseer::Effect		* pcEffect = nullptr;

	std::vector<CEffekSeerParticleSystem *> vParticleSystems;

public:
	CEffekSeerParticleScript( const std::string _strFileName, Effekseer::Effect * _pcEffect );
	CEffekSeerParticleScript( const CEffekSeerParticleScript & c );
	~CEffekSeerParticleScript();

	void					Clear();

	Effekseer::Effect		* GetEffect() { return pcEffect; }
	void					SetEffect( Effekseer::Effect * _pcEffect ) { pcEffect = _pcEffect; }

	void					SetOwner( Engine::ObjectOwner * _pcOwner );

	UINT					GetID() { return uID; }
	void					SetID( UINT _uID );

	std::string				GetFileName() { return strFileName; }
	void					SetFileName(const std::string _strFileName ) { strFileName = _strFileName; }

	std::vector<CEffekSeerParticleSystem *> & GetParticleSystems() { return vParticleSystems; }
};

