#pragma once

#include "PSystem.h"

class CSystem
{
protected:
	OSVERSIONINFO		  os_version_info;
	OSVERSIONINFOEX		  os_version_info_ex;
	SYSTEM_INFO			  sys_info;
	LANGID				  lang_id;

public:
						  CSystem();
	virtual				 ~CSystem() { }

	ESystemOS			  GetSystemOS();
	ESystemLanguage		  GetSystemLanguage();

	BOOL				  IsMultiCore();
	int					  GetNumberOfCores();

	UINT				  GetRAMAmount();

	int					  ComputeFrameDelayTime();

	BOOL				  IsVistaOrLater();
	BOOL				  IsElevated();

	BOOL				  OpenURL( char * pszURL );
	BOOL				  OpenURLEx( char * pszFormatURL, ... );

	std::vector<std::string> ListFilesInDirectory( const std::string & strFolderPath, const std::string & strFileExtension, bool bIncludeSubDirectories = true );

};