#pragma once

#include "server.h"

#include "CCore.h"

#include "CServerManager.h"
#include "CServerHandler.h"
#include "CServerWorld.h"

#include "CServerWindow.h"

#define SERVERMANAGER	(SERVER->GetServerManager())
#define SERVERHANDLER	(SERVER->GetServerHandler())
#define SERVERWORLD		(SERVER->GetServerWorld())

class CServer
{
public:
	CServer();
	virtual				 ~CServer();

	BOOL				  Init();
	void				  Shutdown();

	void				  Update();

	//Getters
	CServerManager		* GetServerManager() { return pcServerManager; }
	CServerHandler		* GetServerHandler() { return pcServerHandler; }
	CServerWorld		* GetServerWorld() { return pcServerWorld; }

	BOOL				  GetMaintenance() { return bMaintenance; }

	//Setters
	void				  SetMaintenance(BOOL bOn) { bMaintenance = bOn; }

private:
	CServerManager		* pcServerManager;
	CServerHandler		* pcServerHandler;
	CServerWorld		* pcServerWorld;

	BOOL				  bMaintenance;

};