#pragma once

#include "CWorld.h"

#define WORLD			(GAMEWORLD->GetWorld())

#define WORLD_FORTRESS	(WORLD->GetWorldID() == WORLDID_Fortress)
#define WORLD_TEST		(WORLD->GetWorldID() == WORLDID_Test)

class CGameWorld
{
public:
	CGameWorld();
	virtual				 ~CGameWorld() {}

	BOOL				  Init();
	void				  Shutdown();

	void				  Apply();

	//Setters
	void				  SetWorld( EWorldID iWorldID );
	void				  SetWorld( std::string strWorld );

	//Getters
	CWorld				* GetWorld() { return pcWorld; }

private:
	std::vector<CWorld*>	  vWorlds;

	CWorld				* pcWorld;

};