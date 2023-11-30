#pragma once

#include "ExportFile.h"

#include "RawINXFile.h"


namespace Export
{

class INXFile : public File
{
public:
							  INXFile( const std::string & strFilePath );

	bool					  Save( const RawINXFile * pRawFile );

private:
	void					  Encode();

protected:
	RawINXFile				  sRawFile;
};

}
