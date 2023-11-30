#include "stdafx.h"
#include "ShamanHandler.h"

std::vector<struct RainMakerParticleBip*>			vUnitsRainMaker;

CShamanHandler::CShamanHandler()
{

}


CShamanHandler::~CShamanHandler()
{
}


void CShamanHandler::LoadParticles()
{
	iRainMakerBipParticleID = PARTICLE->Load( "game\\scripts\\particles\\BipPhantomRain.part" );
}

void CShamanHandler::OnRainMakerParticle( UnitData * pcUnitGame, int iTime )
{
	return;

	if ( iRainMakerBipParticleID == -1 )
		return;

	RainMakerParticleBip * p	= new RainMakerParticleBip;
	p->iID						= PARTICLE->Start( iRainMakerBipParticleID, pcUnitGame->sPosition.iX, pcUnitGame->sPosition.iY, pcUnitGame->sPosition.iZ );
	p->pcUnitGame				= pcUnitGame;
	p->iTime					= 5 * 1000;
	p->dwObjectID				= pcUnitGame->iID;
	vUnitsRainMaker.push_back( p );
}

void CShamanHandler::RenderParticle()
{
	
}

void CShamanHandler::Update( float fTime )
{
	
}