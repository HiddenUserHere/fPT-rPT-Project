#include "stdafx.h"
#include "CEffekSeerParticleSystem.h"

CEffekSeerParticleSystem::CEffekSeerParticleSystem( Effekseer::Effect * _pcEffect ) : pcEffect( _pcEffect )
{

}

CEffekSeerParticleSystem::CEffekSeerParticleSystem( const CEffekSeerParticleSystem & c )
{
	pcEffect = c.pcEffect;

	pcOwner = (c.pcOwner ? c.pcOwner->Clone() : nullptr);

	iID = c.iID;

	bKill = c.bKill;

	iHandle = c.iHandle;
}

CEffekSeerParticleSystem::~CEffekSeerParticleSystem()
{
	DELET( pcOwner );
}

void CEffekSeerParticleSystem::SetOwner( Engine::ObjectOwner * _pcOwner )
{
	DELET( pcOwner );

	pcOwner = _pcOwner->Clone();
}

UINT CEffekSeerParticleSystem::GetOwnerID()
{
	if ( pcOwner )
	{
		if ( pcOwner->GetType() == Engine::EObjectOwnerType::OBJECTOWNERTYPE_Unit )
			return ((Engine::ObjectOwnerUnit *)pcOwner)->GetUnitID();
	}

	return 0xFFFFFFFF;
}

void CEffekSeerParticleSystem::Kill( BOOL bForce )
{
	//Set Kill State
	bKill = TRUE;

	//Force end the Particle System? Release it
	//if ( bForce )
	//	EFFEKSEERHANDLER->GetManager()->StopEffect( iHandle );
}

void CEffekSeerParticleSystem::Render( Effekseer::Manager * pcManager, BOOL bBack )
{
	if ( bKill )
		return;

	if ( bBack )
		pcManager->DrawHandleBack( iHandle );
	else
		pcManager->DrawHandleFront( iHandle );
}

BOOL CEffekSeerParticleSystem::Update( float fTime )
{
	//Already killed? So don't update
	if ( bKill )
		return FALSE;

	//Owner is Lost State? Kill particles
	if ( pcOwner )
	{
		if ( pcOwner->Lost( TICKCOUNT ) )
		{
			EFFEKSEERHANDLER->OnLost( this );

			Kill( TRUE );
		}
	}
	BOOL bRet = bKill;

	//Still Alive?
	if ( bRet == FALSE )
	{
		//Update Object if there's have one
		if ( pcOwner )
			pcOwner->Update();
	}

	return !bRet;
}
