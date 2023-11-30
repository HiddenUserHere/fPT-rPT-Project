#pragma once

#include "ExportFile.h"

#include "RawSMDHeader.h"
#include "RawSMDObjectInfo.h"
#include "RawSMDMaterialHeader.h"
#include "RawSMDMaterial.h"
#include "RawSMDObject.h"
#include "RawSMDTerrain.h"

namespace Export
{

class SMDFile : public File
{
public:
							  SMDFile( const std::string & strFilePath );

	bool					  SaveModel( const RawSMDHeader * psRawHeader, const RawSMDMaterialHeader * psRawMaterialHeader, const RawSMDObject * psaRawObject, UglyRefactor::PTModelVersion & sModelVersion );
	bool					  SaveTerrain( const RawSMDHeader * psRawHeader, const RawSMDMaterialHeader * psRawMaterialHeader, const RawSMDTerrain * psRawTerrain );

private:
	unsigned int			  ComputeMaterialsBlockSize( const RawSMDMaterialHeader * psRawMaterialHeader ) const;
	void					  SaveMaterials( const RawSMDMaterialHeader * psRawMaterialHeader );

	unsigned int			  ComputeObjectBlockSize( const RawSMDObject * psRawObject, UglyRefactor::PTModelVersion & sModelVersion ) const;
	void					  SaveObject( const RawSMDObject * psRawObject, UglyRefactor::PTModelVersion & sModelVersion );

};

}