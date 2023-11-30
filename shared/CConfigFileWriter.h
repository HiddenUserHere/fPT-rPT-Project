#pragma once

#include "CConfigFileReader.h"

class CConfigFileWriter : public CConfigFileReader
{
public:
					  CConfigFileWriter( string _filepath );
	virtual			 ~CConfigFileWriter() {}

	void			  WriteString( string section, string key, string value );

protected:
	void			  SetValue( string key, string value, int line_key );

};