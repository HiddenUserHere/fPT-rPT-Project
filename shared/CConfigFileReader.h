#pragma once

#include "CConfigFile.h"

class CConfigFileReader : public CConfigFile
{
public:
					  CConfigFileReader( string _filepath );
	virtual			 ~CConfigFileReader() {}

	string			  ReadString( string section, string key );
	int				  ReadInt( string section, string key );
	long			  ReadLong( string section, string key );
	bool			  ReadBool( string section, string key, string value );

protected:
					  CConfigFileReader( string _filepath, EConfigFileMode _filemode );

protected:
	string			  GetValue( int line_key );

};