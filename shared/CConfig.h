#pragma once

#include <string>
#include <sstream>

#include "CConfigFileReader.h"
#include "CConfigFileWriter.h"

class CConfig
{
public:
						  CConfig( string _file );
	virtual				 ~CConfig() {}

	CConfigFileReader	* GetReader();
	CConfigFileWriter	* GetWriter();

	void				  CloseReader();
	void				  CloseWriter();

protected:
	string				  file;

	CConfigFileReader	* pcReader;
	CConfigFileWriter	* pcWriter;

};