#include "stdafx.h"
#include "CParticleEmitter.h"


CParticleEmitter::CParticleEmitter( CParticleSystem * _pcParticleSystem )
{
	pcParticleSystem	= _pcParticleSystem;

	eType				= PARTICLEEMITTERTYPE_Follow;

	sCurDelay			= 0.0f;
	sMaxDelay			= 0.0f;

	sCurLifeTime		= 0.0f;
	sMaxLifeTime		= -1.0f;

	pOwner				= NULL;

	fLastEmit			= 0.0f;

	bActive				= FALSE;
}

CParticleEmitter::CParticleEmitter( CParticleSystem * _pcParticleSystem, CParticleEmitter & c )
{
	pcParticleSystem	= _pcParticleSystem;

	eType				= c.eType;

	sPosition			= c.sPosition;
	sVelocity			= c.sVelocity;

	sRotation			= c.sRotation;
	sAngularVelocity	= c.sAngularVelocity;

	sCurDelay			= c.sCurDelay;
	sMaxDelay			= c.sMaxDelay;

	sCurLifeTime		= c.sCurLifeTime;
	sMaxLifeTime		= c.sMaxLifeTime;

	sParticles			= c.sParticles;
	sInterval			= c.sInterval;

	pOwner				= (c.pOwner ? c.pOwner->Clone() : NULL);

	fLastEmit			= 0.0f;

	bActive				= FALSE;
}

CParticleEmitter::~CParticleEmitter()
{
	//WRITEDBG( "~CParticleEmitter()" );

	DELET( pOwner );
}

void CParticleEmitter::Birth()
{
	sCurDelay.Birth();
	sMaxDelay.Birth();

	sCurLifeTime.Birth();
	sMaxLifeTime.Birth();

	sParticles.Birth();
	sInterval.Birth();
}

BOOL CParticleEmitter::Update( double fTime )
{
	if( pOwner )
	{
		//Update Owner
		pOwner->Update();
	}

	double fCurDelay = sCurDelay.Get();
	double fMaxDelay = sMaxDelay.Get();

	if( fCurDelay < fMaxDelay )
	{
		fCurDelay += fTime;

		if( fCurDelay > fMaxDelay )
		{
			fTime += (fCurDelay - fMaxDelay);

			fCurDelay = fMaxDelay;
		}

		sCurDelay.Set( fCurDelay );
	}

	if( fCurDelay >= fMaxDelay )
	{
		//Handle Time Events
		double fCurLifeTime = sCurLifeTime.t.value;
		double fMaxLifeTime = sMaxLifeTime.t.value;

		double fNewCurLifeTime = fCurLifeTime + fTime;

		//Reached Max Life Time?
		if( fNewCurLifeTime > fMaxLifeTime )
			fNewCurLifeTime = fMaxLifeTime;

		//Update Emit
		if( (sParticles.value > 0) || (sParticles.value == -1) )
		{
			fLastEmit -= fTime;

			if( fLastEmit <= 0.0f )
			{
				double fOverTime = abs( fLastEmit );
				double fInterval = sInterval.value;

				do
				{
					//Emit with Update Over Time
					Emit( fOverTime );

					//Finite Particles?
					if( sParticles.value != -1 )
					{
						sParticles.value--;

						if( sParticles.value <= 0 )
							break;
					}

					//Another Particle?
					fOverTime -= fInterval;
				} while( fOverTime >= 0.0f );

				//Save Left Over Time
				fLastEmit = abs( fOverTime );
			}
		}

		fCurLifeTime = fNewCurLifeTime;

		sCurLifeTime.t.value = fCurLifeTime;

		//If the Max Life Time is negative, this Emitter never ends by time. Otherwise it does
		if( fMaxLifeTime >= 0.0f )
		{
			if( fCurLifeTime >= fMaxLifeTime )
				return TRUE;
		}
	}

	return FALSE;
}

void CParticleEmitter::SetName( std::string _strName )
{
	strName = _strName;
}

void CParticleEmitter::Emit( double fUpdateTime )
{
	//Create the Clone Particle
	CParticle * pcParticle = pcParticleSystem->pcParticle->Clone();

	//Fill Particle Attributes
	if( ((eType == PARTICLEEMITTERTYPE_Follow) && (pOwner != NULL)) && (pcParticle->eType != PARTICLETYPE_Follow) )
	{
		//Base position on owner
		CPoint<int> & sOwnerPosition = pOwner->GetPosition();

		pcParticle->sPosition.x		= CParticle::Int( sPosition.x.low + sOwnerPosition.GetX(), sPosition.x.high + sOwnerPosition.GetX() );
		pcParticle->sPosition.y		= CParticle::Int( sPosition.y.low + sOwnerPosition.GetY(), sPosition.y.high + sOwnerPosition.GetY() );
		pcParticle->sPosition.z		= CParticle::Int( sPosition.z.low + sOwnerPosition.GetZ(), sPosition.z.high + sOwnerPosition.GetZ() );
	}
	else
	{
		//No owner
		pcParticle->sPosition		= sPosition;
	}
	pcParticle->sVelocity			= sVelocity;

	pcParticle->sRotation			= sRotation;
	pcParticle->sAngularVelocity	= sAngularVelocity;

	//Birth Particle
	pcParticle->Birth();

	//Update Particle
	if( !pcParticle->Update( fUpdateTime ) )
	{
		//Add Particle to System
		pcParticleSystem->Add( pcParticle );
	}
	else
	{
		//Delete Particle
		DELET( pcParticle );
	}
}