#pragma once

class CServerManager
{
public:
	CServerManager();
	virtual				 ~CServerManager();

	BOOL				  Init();
	void				  Shutdown();

private:
};