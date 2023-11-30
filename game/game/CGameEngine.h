#pragma once

#include "CGameAudio.h"
#include "CGameGraphic.h"

#define GAMEGRAPHIC					(GAMEENGINE->GetGameGraphic())
#define GAMEAUDIO					(GAMEENGINE->GetGameAudio())

class CGameEngine
{
public:
	CGameEngine();
	virtual ~CGameEngine();

	BOOL			  Init();
	void			  Shutdown();

	void			  Frame();
	void			  Update( float fTime );

	void			  Render3D();
	void			  Render2D();

	CGameAudio		* GetGameAudio() { return pcGameAudio; }
	CGameGraphic	* GetGameGraphic() { return pcGameGraphic; }

private:
	CGameAudio		* pcGameAudio;
	CGameGraphic	* pcGameGraphic;

};

