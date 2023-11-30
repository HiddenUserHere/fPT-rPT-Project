#pragma once

#include "ImportFile.h"

#include "RawINXFile.h"



namespace Import
{

class INXFile : public File
{
public:
							  INXFile( const std::string & strFilePath );
	virtual					 ~INXFile();

	bool					  Load();
	void					  Unload();

	bool					  IsLoaded() const { return bLoaded; }

private:
	void					  Decode();

public:
	RawINXFile				  sRawFile;

private:
	bool					  bLoaded = false;
};

}
