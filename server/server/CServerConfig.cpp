#include "stdafx.h"
#include "CServerConfig.h"

CServerConfig::CServerConfig(std::string _file) : CConfig(_file)
{
}

int CServerConfig::GetVersionGame()
{
	CConfigFileReader * pcConfigFileReader = GetReader();

	int iReturn = pcConfigFileReader->ReadInt( "Server", "Version" );

	CloseReader();

	return iReturn;
}

int CServerConfig::GetVersionServer()
{
	CConfigFileReader * pcConfigFileReader = GetReader();

	int iReturn = pcConfigFileReader->ReadInt( "Server", "Version" );

	CloseReader();

	return iReturn;
}

string CServerConfig::GetServerWorld()
{
	CConfigFileReader * pcConfigFileReader = GetReader();

	string strReturn = pcConfigFileReader->ReadString("Server", "World");

	CloseReader();

	return strReturn;
}

BOOL CServerConfig::GetDebugNoMonsters()
{
	CConfigFileReader * pcConfigFileReader = GetReader();

	bool bReturn = pcConfigFileReader->ReadBool("Debug", "NoMonsters", "True");

	CloseReader();

	return bReturn;
}