#include "stdafx.h"
#include "CGameGraphic.h"

CGameGraphic::CGameGraphic()
{
	pGraphicEngine = new DXGraphicEngine();
	pcUserInterface = new CUserInterface();
	pcParticleEngine = new CParticleEngine();
}

CGameGraphic::~CGameGraphic()
{
	DELET( pGraphicEngine );
	DELET( pcUserInterface );
	DELET( pcParticleEngine );
}

BOOL CGameGraphic::Init()
{
	INIT( pcUserInterface );
	INIT( pcParticleEngine );
	return TRUE;
}

void CGameGraphic::Shutdown()
{
	SHUTDOWN( pcUserInterface );
	SHUTDOWN( pcParticleEngine );
}

void CGameGraphic::Frame()
{
}

void CGameGraphic::Update( float fTime )
{
	pcParticleEngine->Update( fTime );
}

void CGameGraphic::Render3D()
{
//	pcParticleEngine->Render3D( pGraphicEngine );
}

void CGameGraphic::Render2D()
{
	pcParticleEngine->Render2D( pGraphicEngine );
}
