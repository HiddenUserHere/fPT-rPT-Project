#pragma once
class SMDMaterialImporter
{

public:
	SMDMaterialImporter();
	virtual ~SMDMaterialImporter();

	std::vector<DX::Material_ptr>	GetLoadMaterials( struct RawSMDMaterialHeader * psRawMaterialHeader, BOOL bTerrain = FALSE );
};

