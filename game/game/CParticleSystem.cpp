#include "stdafx.h"
#include "CParticleSystem.h"


CParticleSystem::CParticleSystem()
{
	strName					= "";

	pcParticleEmitter		= new CParticleEmitter( this );
	pcParticle				= NULL;

	pOwner					= NULL;

	bKill					= FALSE;

	bPaused					= FALSE;

	uID						= 0xFFFFFFFF;
}

CParticleSystem::CParticleSystem( CParticle * _pcParticle )
{
	strName					= "";

	pcParticleEmitter		= new CParticleEmitter( this );
	pcParticle				= _pcParticle;

	pOwner					= NULL;

	bKill					= FALSE;

	bPaused					= FALSE;

	uID						= 0xFFFFFFFF;
}

CParticleSystem::CParticleSystem( CParticleSystem & c )
{
	strName					= c.strName;

	pcParticleEmitter		= new CParticleEmitter( this, *c.pcParticleEmitter );
	pcParticle				= (c.pcParticle ? c.pcParticle->Clone() : NULL);
	
	pOwner					= (c.pOwner ? c.pOwner->Clone() : NULL);

	bKill					= c.bKill;

	bPaused					= c.bPaused;

	uID						= c.uID;
}

CParticleSystem::~CParticleSystem()
{
	for( vector<CParticle*>::iterator it = vActiveParticles.begin(); it != vActiveParticles.end(); it++ )
	{
		CParticle * pcActiveParticle = *it;

		DELET( pcActiveParticle );
	}
	vActiveParticles.clear();

	DELET( pcParticle );
	DELET( pcParticleEmitter );
}

void CParticleSystem::Birth()
{
	pcParticleEmitter->Birth();
}

BOOL CParticleSystem::Update( double fTime )
{
	if( bPaused )
		return FALSE;

	if( pOwner )
	{
		if( pOwner->Lost( TICKCOUNT ) )
		{
			//Owner is Lost, force System end
			Kill( TRUE );
		}
		else
		{
			//Check if an ending Condition has met
			//TODO
		}
	}

	//Update Alive Particles
	for( vector<CParticle*>::iterator it = vActiveParticles.begin(); it != vActiveParticles.end(); )
	{
		CParticle * pcActiveParticle = *it;

		if( pcActiveParticle->Update( fTime ) )
		{
			DELET( pcActiveParticle );

			it = vActiveParticles.erase( it );
		}
		else
			it++;
	}

	//Emit Vitality
	BOOL bRet = bKill;
	
	//System Ending?
	if( bRet == FALSE )
	{
		//Emit New Particles
		bRet = pcParticleEmitter->Update( fTime );
	}

	//Is the Emitter done or is System ending?
	if( bRet )
	{
		if( ParticlesAlive() > 0 )
		{
			//Let's not delete this Particle System yet if there's active particles
			return FALSE;
		}
	}

	return bRet;
}

void CParticleSystem::Render( EXETextureManager * pcTextureManager )
{
	//Prepare for Rendering
	RENDERER->Init();

	//Loop through each Active Particle and Render it
	for( int i = 0, j = vActiveParticles.size(); i < j; i++ )
	{
		CParticle * pcActiveParticle = vActiveParticles[i];

		//Set the Source Blend Mode of the Texture
		EXETextureMaterial * pTextureWrapper	= pcTextureManager->GetTexture( pcActiveParticle->uTextureID );
		pTextureWrapper->bInUse				= TRUE;
		pTextureWrapper->bTwoSided			= TRUE;
		pTextureWrapper->iBlendType			= pcActiveParticle->eSourceBlendMode;

		RENDERER->SetTextureManager( pcTextureManager );

		//Render the Particle
		pcActiveParticle->Render();

		//Depracated for now pTextureWrapper->bInUse = FALSE;
	}

	//Prepare to finalize Rendering
	RENDERER->SetRenderClip( 0x3C );
	RENDERER->ClipFaceRender();
	RENDERER->Vertex2DGeometry( 0 );

	//Finalize Rendering
	RENDERER->Render();
}

void CParticleSystem::Kill( BOOL bForce )
{
	bKill = TRUE;
	
	if( bForce )
	{
		for( vector<CParticle*>::iterator it = vActiveParticles.begin(); it != vActiveParticles.end(); it++ )
		{
			CParticle * pcActiveParticle = *it;

			DELET( pcActiveParticle );
		}
		vActiveParticles.clear();
	}
	else
	{
		for( vector<CParticle*>::iterator it = vActiveParticles.begin(); it != vActiveParticles.end(); it++ )
		{
			CParticle * pcActiveParticle = *it;

			pcActiveParticle->bLoop = FALSE;
		}
	}
}

UINT CParticleSystem::GetOwnerID()
{
	if( pOwner )
	{
		if( pOwner->GetType() == Engine::OBJECTOWNERTYPE_Unit )
		{
			//Return Unit ID
			return ((Engine::ObjectOwnerUnit*)pOwner)->GetUnitID();
		}
	}

	return 0xFFFFFFFF;
}

void CParticleSystem::SetOwner( Engine::ObjectOwner * pNewOwner )
{
	pOwner = pNewOwner->Clone();

	pcParticleEmitter->SetOwner( pNewOwner );
	pcParticle->SetOwner( pNewOwner );
}

void CParticleSystem::Add( CParticle * pcParticle )
{
	vActiveParticles.push_back( pcParticle );
}