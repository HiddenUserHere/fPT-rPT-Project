#pragma once

#include "CConfig.h"

class CServerConfig : public CConfig
{
public:
	CServerConfig(std::string _file);
	virtual				 ~CServerConfig() {}

	void				  Init() {};
	void				  Shutdown() {};

	//Getters
	int					  GetVersionGame();
	int					  GetVersionServer();

	std::string			  GetServerWorld();

	BOOL				  GetDebugNoMonsters();
};