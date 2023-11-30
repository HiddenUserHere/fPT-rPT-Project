#pragma once

#include "DXModelImporter.h"
#include "SMDDXMeshImporter.h"
#include "SMDDXMaterialImporter.h"

class SMDModelImporter : public DX::ModelImporter
{
private:
	SMDMeshImporter		* pcMeshImporter = nullptr;
	SMDMaterialImporter	* pcMaterialImporter = nullptr;

	std::string _strFilePath;

	std::vector<DX::Mesh_ptr> CreateMeshes( DX::Model_ptr pModel, struct RawSMDHeader * psRawHeader, struct RawSMDObjectInfo * psaRawObjectInfo, struct RawSMDObject * psaRawObject, std::vector<DX::Material_ptr> & vMaterials );


public:
	bool Load( const std::string & strFilePath );

	DX::Model_ptr Build( DX::Model_ptr pBoneModel = nullptr );


	SMDModelImporter();
	virtual ~SMDModelImporter();
};

