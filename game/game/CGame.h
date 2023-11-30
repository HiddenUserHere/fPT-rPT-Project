#pragma once

#include "GameCore.h"
#include "CGameScreen.h"
#include "CGameEngine.h"
#include "CGameTimer.h"
#include "CGameWorld.h"

#define GAMEENGINE				(GAME->GetGameEngine())
#define GAMECORE				(GAME->GetGameCore())
#define GAMESCREEN				(GAME->GetGameScreen())
#define GAMETIMER				(GAME->GetGameTimer())
#define GAMEWORLD				(GAME->GetGameWorld())

class CGame
{
public:
	CGame();
	virtual ~CGame();

	BOOL					Init();
	void					Shutdown();

	void					Begin();
	void					End();

	void					Frame();
	void					Update( float fTime );

	void					Render3D();
	void					Render2D();

	BOOL					OnKeyPress( class CKeyboard * pcKeyboard );
	BOOL					OnKeyChar( class CKeyboard * pcKeyboard );

	void					OnMouseMove( class CMouse * pcMouse );
	BOOL					OnMouseClick( class CMouse * pcMouse );
	BOOL					OnMouseScroll( class CMouse * pcMouse );

	void					OnResolutionChanged();

	//Getters
	CGameEngine				* GetGameEngine() { return pcGameEngine; }
	GameCore				* GetGameCore() { return pcGameCore; }
	CGameScreen				* GetGameScreen() { return pcGameScreen; }
	CGameTimer				* GetGameTimer() { return pcGameTimer; }
	CGameWorld				* GetGameWorld() { return pcGameWorld; }

public:
	CGameEngine				* pcGameEngine;
	GameCore				* pcGameCore;
	CGameScreen				* pcGameScreen;
	CGameTimer				* pcGameTimer;
	CGameWorld				* pcGameWorld;
};

