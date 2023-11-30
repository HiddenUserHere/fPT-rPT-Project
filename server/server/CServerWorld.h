#pragma once

#include "CWorld.h"

#define WORLD			(SERVER->GetServerWorld()->GetWorld())

#define WORLD_REALM		(WORLD->GetWorldID() == WORLDID_Realm)
#define WORLD_UNIQUE	(WORLD->GetWorldID() == WORLDID_Unique)

class CServerWorld
{
public:
	CServerWorld();
	virtual				 ~CServerWorld() {}

	BOOL				  Init();
	void				  Shutdown();

	//Setters
	void				  SetWorld(EWorldID iWorldID);
	void				  SetWorld(std::string strWorld);

	//Getters
	CWorld				* GetWorld() { return pcWorld; }

private:
	std::vector<CWorld*>		  vWorlds;

	CWorld				* pcWorld;

};