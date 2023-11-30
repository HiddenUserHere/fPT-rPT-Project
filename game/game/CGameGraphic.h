#pragma once

#include "CParticleEngine.h"
#include "CUserInterface.h"

#include "CLuaParticleOld.h"

#define PARTICLEENGINE				(GAMEGRAPHIC->GetParticleEngine())
#define GRAPHICENGINE				(GAMEGRAPHIC->GetGraphicEngine())
#define GRAPHICS					Delta3D::Graphics::Graphics::GetInstance()


class CGameGraphic
{
public:
	CGameGraphic();
	virtual ~CGameGraphic();

	BOOL				  Init();
	void				  Shutdown();

	void				  Frame();
	void				  Update( float fTime );

	void				  Render3D();
	void				  Render2D();

	DXGraphicEngine		* GetGraphicEngine() { return pGraphicEngine; }

	CUserInterface		* GetUserInterface() { return pcUserInterface; }

	CParticleEngine		* GetParticleEngine() { return pcParticleEngine; }

private:
	DXGraphicEngine		* pGraphicEngine;

	CUserInterface		* pcUserInterface;

	CParticleEngine		* pcParticleEngine;

};

