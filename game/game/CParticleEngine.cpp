#include "stdafx.h"
#include "CParticleEngine.h"


CParticleEngine::CParticleEngine()
{
	pcMutex				= new CMutex( "Particle Engine" );

	pcParticleFactory	= new CParticleFactory();
	pcParticleDebug		= new CParticleDebug( this );

	pcTextureManager	= new EXETextureManager( 1000 );
}

CParticleEngine::~CParticleEngine()
{
	//WRITEDBG( "~CParticleEngine()" );

	DELET( pcTextureManager );

	DELET( pcParticleDebug );
	DELET( pcParticleFactory );

	DELET( pcMutex );
}

BOOL CParticleEngine::Init()
{
	INITP( pcParticleFactory, pcTextureManager );

	return TRUE;
}

void CParticleEngine::Shutdown()
{
	for( vector<CParticleSystem*>::iterator it = vParticleSystems.begin(); it != vParticleSystems.end(); it++ )
	{
		CParticleSystem * pcParticleSystem = *it;

		DELET( pcParticleSystem );
	}
	vParticleSystems.clear();

	SHUTDOWN( pcParticleFactory );
}

void CParticleEngine::Update( double fTime )
{
	for( vector<CParticleSystem*>::iterator it = vParticleSystems.begin(); it != vParticleSystems.end(); )
	{
		CParticleSystem * pcParticleSystem = *it;

		if( pcParticleSystem->Update( fTime ) )
		{
			DELET( pcParticleSystem );

			it = vParticleSystems.erase( it );
		}
		else
			it++;
	}
}

void CParticleEngine::Render3D( DXGraphicEngine * pGraphicEngine, Point3D sCameraPos, Point3D sCameraAngle )
{
	//Render States
	DWORD dw1 = 0, dw2 = 0;
	GRAPHICENGINE->GetDevice()->GetRenderState( D3DRS_DITHERENABLE, &dw1 );
	GRAPHICENGINE->GetDevice()->GetRenderState( D3DRS_SPECULARENABLE, &dw2 );

	GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_DITHERENABLE, FALSE );
	GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
	GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

	RENDERER->SetTextureManager( pcTextureManager );
	RENDERER->SetCameraPosition( sCameraPos.iX, sCameraPos.iY, sCameraPos.iZ, sCameraAngle.iX, sCameraAngle.iY, sCameraAngle.iZ );

	//Loop through each Particle System and render each Particle
	for( int i = 0, j = vParticleSystems.size(); i < j; i++ )
		vParticleSystems[i]->Render( pcTextureManager );

	GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );

	GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_DITHERENABLE, dw1 );
	GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_SPECULARENABLE, dw2 );
}

void CParticleEngine::Render2D( DXGraphicEngine * pGraphicEngine )
{
	//Only for Particle Debug
	pcParticleDebug->Render();
}

UINT CParticleEngine::Add( CParticleSystem * pcParticleSystem, double fOverTime )
{
	pcMutex->Lock();

	pcParticleSystem->Birth();

	if( fOverTime > 0.0f )
	{
		if( pcParticleSystem->Update( fOverTime ) )
		{
			DELET( pcParticleSystem );

			pcMutex->Unlock();

			return -1;
		}
	}

	UINT iID = vParticleSystems.size();

	vParticleSystems.push_back( pcParticleSystem );

	pcMutex->Unlock();

	return iID;
}

void CParticleEngine::AddScript( CParticleScript * pcParticleScript, double fOverTime )
{
	pcMutex->Lock();

	for( vector<CParticleSystem*>::iterator it = pcParticleScript->vParticleSystems.begin(); it != pcParticleScript->vParticleSystems.end(); it++ )
	{
		CParticleSystem * pcParticleSystem = *it;

		UINT iID = Add( pcParticleSystem, fOverTime );
	}

	pcParticleScript->Clear();

	DELET( pcParticleScript );

	pcMutex->Unlock();
}

void CParticleEngine::Pause( UINT iID )
{
	if( (iID >= 0) && (iID < vParticleSystems.size()) )
	{
		CParticleSystem * pcParticleSystem = vParticleSystems[iID];

		pcParticleSystem->Pause();
	}
}

void CParticleEngine::Continue( UINT iID )
{
	if( (iID >= 0) && (iID < vParticleSystems.size()) )
	{
		CParticleSystem * pcParticleSystem = vParticleSystems[iID];

		pcParticleSystem->Continue();
	}
}

void CParticleEngine::Kill( UINT uParticleID )
{
	for( vector<CParticleSystem*>::iterator it = vParticleSystems.begin(); it != vParticleSystems.end(); it++ )
	{
		CParticleSystem * pcParticleSystem = *it;
	
		if( pcParticleSystem->GetID() == uParticleID )
		{
			//Let Particle System Die
			pcParticleSystem->Kill( TRUE );
		}
	}
}

void CParticleEngine::Kill( UINT uParticleID, UINT uOwnerID )
{
	for( vector<CParticleSystem*>::iterator it = vParticleSystems.begin(); it != vParticleSystems.end(); it++ )
	{
		CParticleSystem * pcParticleSystem = *it;
	
		if( (pcParticleSystem->GetID() == uParticleID) && (pcParticleSystem->GetOwnerID() == uOwnerID) )
		{
			//Let Particle System Die
			pcParticleSystem->Kill( FALSE );
		}
	}
}

void CParticleEngine::KillAll()
{
	for( vector<CParticleSystem*>::iterator it = vParticleSystems.begin(); it != vParticleSystems.end(); it++ )
	{
		CParticleSystem * pcParticleSystem = *it;

		DELET( pcParticleSystem );
	}
	vParticleSystems.clear();
}