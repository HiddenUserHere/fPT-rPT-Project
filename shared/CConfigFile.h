#pragma once

#include <iostream>
using namespace std;

enum EConfigFileMode
{
	CONFIGFILEMODE_Read				= 1,
	CONFIGFILEMODE_Write			= 2,
	CONFIGFILEMODE_ReadWrite		= 3,

	CONFIGFILEMODE_ReadBinary		= 11,
	CONFIGFILEMODE_WriteBinary		= 12,
	CONFIGFILEMODE_ReadWriteBinary	= 13,
};

class CConfigFile
{
public:
					  CConfigFile( string _filepath, EConfigFileMode _filemode );
	virtual			 ~CConfigFile();

	BOOL			  Open();
	void			  Close();

	void			  Remove();

protected:
	int				  FindSection( string section );
	int				  FindKey( string key, int line_section );

	static string	  FileModeToString( EConfigFileMode filemode );

protected:
	string			  filepath;
	EConfigFileMode	  filemode;

	BOOL			  open;
	FILE			* file;

};