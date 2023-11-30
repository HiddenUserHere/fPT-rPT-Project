#pragma once

#include "ImportFile.h"
#include "RawSMDMaterialHeader.h"

namespace Import
{

class SMDTerrainImporter : public File
{
public:
						SMDTerrainImporter( std::string strFileName );
	virtual				~SMDTerrainImporter();

	BOOL				Load();

private:
	std::string			strFile;

};

}