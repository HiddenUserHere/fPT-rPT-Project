#pragma once

#include "CWindow.h"

#include "CServer.h"
#include "CServerConfig.h"

#define SERVERWINDOW	((CServerWindow*)WINDOW)

#define SERVERCONFIG	(SERVERWINDOW->GetConfig())
#define SERVER			(SERVERWINDOW->GetServer())

class CServer;

class CServerWindow : public CWindow
{
public:
	CServerWindow();
	virtual					 ~CServerWindow();

	UINT					  Init();
	BOOL					  Shutdown();
	BOOL					  Run();

	CServer					* GetServer() { return pServer; }
	CServerConfig			* GetConfig() { return pConfig; }

private:
	LRESULT					  WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void					  Update(double fTime);
	void					  Render();

protected:
	CServer					* pServer;
	CServerConfig			* pConfig;

};