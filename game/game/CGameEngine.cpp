#include "stdafx.h"
#include "CGameEngine.h"


CGameEngine::CGameEngine()
{
	pcGameGraphic	= new CGameGraphic();

	pcGameAudio		= new CGameAudio();
}


CGameEngine::~CGameEngine()
{
	DELET( pcGameGraphic );
	DELET( pcGameAudio );
}

BOOL CGameEngine::Init()
{
	INIT( pcGameGraphic );
	INIT( pcGameAudio );
	return TRUE;
}

void CGameEngine::Shutdown()
{
	SHUTDOWN( pcGameGraphic );
	SHUTDOWN( pcGameAudio );
}

void CGameEngine::Frame()
{
	pcGameGraphic->Frame();
}

void CGameEngine::Update( float fTime )
{
	pcGameGraphic->Update( fTime );
	pcGameAudio->Update( fTime );
}

void CGameEngine::Render3D()
{
	pcGameGraphic->Render3D();
}

void CGameEngine::Render2D()
{
	pcGameGraphic->Render2D();
}
