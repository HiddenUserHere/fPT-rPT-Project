#include "stdafx.h"
#include "CEffekSeerParticleScript.h"

CEffekSeerParticleScript::CEffekSeerParticleScript( const std::string _strName, Effekseer::Effect * _pcEffect ) : strFileName( _strName ), pcEffect( _pcEffect )
{
	bCache = TRUE;

	vParticleSystems.push_back( new CEffekSeerParticleSystem( _pcEffect ) );
}

CEffekSeerParticleScript::CEffekSeerParticleScript( const CEffekSeerParticleScript & c )
{
	strFileName = c.strFileName;

	pcEffect = c.pcEffect;

	for ( auto pcReadyParticleSystem : c.vParticleSystems )
		vParticleSystems.push_back( new CEffekSeerParticleSystem( *pcReadyParticleSystem ) );
}

CEffekSeerParticleScript::~CEffekSeerParticleScript()
{
	if ( bCache )
		ES_SAFE_RELEASE( pcEffect );

	for ( auto pcParticleSystem : vParticleSystems )
		DELET( pcParticleSystem );

	vParticleSystems.clear();
}

void CEffekSeerParticleScript::Clear()
{
	vParticleSystems.clear();
}

void CEffekSeerParticleScript::SetOwner( Engine::ObjectOwner * _pcOwner )
{
	for ( auto pcParticleSystem : vParticleSystems )
		pcParticleSystem->SetOwner( _pcOwner );

	DELET( _pcOwner );
}

void CEffekSeerParticleScript::SetID( UINT _uID )
{
	uID = _uID;

	for ( auto pcParticleSystem : vParticleSystems )
		pcParticleSystem->SetID( _uID );
}
