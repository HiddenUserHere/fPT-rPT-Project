#include "stdafx.h"
#include "CParticleScript.h"

CParticleScript::CParticleScript( string _strFilePath )
{
	strFilePath			= _strFilePath;

	strScript			= "";
	strAuthor			= "";

	pcParticleSystem	= NULL;
}

CParticleScript::CParticleScript( CParticleScript & c )
{
	strFilePath			= c.strFilePath;

	strScript			= c.strScript;
	strAuthor			= c.strAuthor;

	pcParticleSystem	= NULL;

	for( vector<CParticleSystem*>::iterator it = c.vParticleSystems.begin(); it != c.vParticleSystems.end(); it++ )
	{
		CParticleSystem * pcReadyParticleSystem = *it;

		vParticleSystems.push_back( new CParticleSystem( *pcReadyParticleSystem ) );
	}
}

CParticleScript::~CParticleScript()
{
	//WRITEDBG( "~CParticleScript()" );

	for( vector<CParticleSystem*>::iterator it = vParticleSystems.begin(); it != vParticleSystems.end(); it++ )
	{
		CParticleSystem * pcReadyParticleSystem = *it;

		DELET( pcReadyParticleSystem );
	}
	vParticleSystems.clear();

	DELET( pcParticleSystem );
}

void CParticleScript::Add( CParticleSystem * pcReadyParticleSystem )
{
	vParticleSystems.push_back( pcReadyParticleSystem );
}

void CParticleScript::Clear()
{
	vParticleSystems.clear();
}

void CParticleScript::SetOwner( Engine::ObjectOwner * pNewOwner )
{
	for( vector<CParticleSystem*>::iterator it = vParticleSystems.begin(); it != vParticleSystems.end(); it++ )
	{
		CParticleSystem * pcReadyParticleSystem = *it;

		pcReadyParticleSystem->SetOwner( pNewOwner );
	}

	DELET( pNewOwner );
}

void CParticleScript::SetID( UINT uID )
{
	for( vector<CParticleSystem*>::iterator it = vParticleSystems.begin(); it != vParticleSystems.end(); it++ )
	{
		CParticleSystem * pcReadyParticleSystem = *it;

		pcReadyParticleSystem->SetID( uID );
	}
}

void CParticleScript::RemoveParticleSystem( string strParticleSystem )
{
	for( vector<CParticleSystem*>::iterator it = vParticleSystems.begin(); it != vParticleSystems.end(); )
	{
		CParticleSystem * pcReadyParticleSystem = *it;

		if( strParticleSystem.compare( pcReadyParticleSystem->GetName() ) == 0 )
		{
			DELET( pcReadyParticleSystem );

			it = vParticleSystems.erase( it );
		}
		else
			it++;
	}
}