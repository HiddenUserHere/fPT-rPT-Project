#include "stdafx.h"
#include "SMDDXModelImporter.h"
#include "ImportSMDFile.h"



std::vector<DX::Mesh_ptr> SMDModelImporter::CreateMeshes( DX::Model_ptr pModel, struct RawSMDHeader * psRawHeader, RawSMDObjectInfo * psaRawObjectInfo, RawSMDObject * psaRawObject, std::vector<DX::Material_ptr> & vMaterials )
{
	std::vector<DX::Mesh_ptr> vMeshes;

	std::vector<SMDMeshImporter::SMDMeshVertexData> vVertexData;

	//Meshes
	for ( int i = 0; i < psRawHeader->iObjectCount; i++ )
	{
		//New Mesh
		RawSMDObject * psRawObject	= psaRawObject + i;
		auto pMesh					= std::make_shared<DX::Mesh>( psaRawObjectInfo[i].szObjectName );

		//Add Vertices
		pcMeshImporter->SetVertices( pModel, pMesh, psRawObject, vVertexData, vMaterials );

		//Set Texture Coordinates
		pcMeshImporter->CreateTextureCoordinates( pMesh, psRawObject, vMaterials, vVertexData );
	
		//Set Faces
		pcMeshImporter->SetFaces( pModel, pMesh, psRawObject, vMaterials );

		//Add Keys
		pcMeshImporter->SetKeys( pMesh, psRawObject );

		//Set Matrices
		pcMeshImporter->SetMatrices( pMesh, psRawObject );

		//Store Mesh
		vMeshes.push_back( pMesh );

		for ( auto & sVertexData : vVertexData )
			sVertexData.vUV.clear();

		vVertexData.clear();
	}

	//Update Meshes's Parent
	pcMeshImporter->UpdateMeshParent( vMeshes, psaRawObject );

	return vMeshes;
}

bool SMDModelImporter::Load( const std::string & strFilePath )
{
	_strFilePath = strFilePath;

	//Replace ASE with SMD
	if ( auto it = _strFilePath.find( ".ase" ); it != std::string::npos )
		_strFilePath = _strFilePath.replace( it, 4, ".smd" );

	if ( FileExist( _strFilePath ) )
		return true;

	if ( auto it = _strFilePath.find( ".smd" ); it != std::string::npos )
		_strFilePath = _strFilePath.replace( it, 4, ".smb" );

	if ( FileExist( _strFilePath ) )
		return true;

	return false;
}

DX::Model_ptr SMDModelImporter::Build( DX::Model_ptr pBoneModel )
{
	std::string strTemp = _strFilePath;

	//Replace ASE with SMD
	if ( auto it = strTemp.find( ".smd" ); it != std::string::npos )
		strTemp = strTemp.replace( it, 4, ".ase" );

	if ( auto it = strTemp.find( ".smb" ); it != std::string::npos )
		strTemp = strTemp.replace( it, 4, ".ase" );

	Import::SMDFile impSMDFile( _strFilePath );
	if ( !impSMDFile.Load() )
		return nullptr;

	//New Model
	DX::Model_ptr pModel = std::make_shared<DX::Model>( strTemp );
	
	//Load Materials if it's a Model (otherwise, Bone)
	std::vector<DX::Material_ptr> vMaterials;
	if ( impSMDFile.sRawHeader.iMaterialCount > 0 )
		vMaterials = pcMaterialImporter->GetLoadMaterials( &impSMDFile.sRawMaterialHeader );

	//Update Bone if have one
	pModel->SetBoneModel( pBoneModel );

	//Load Meshes data (vertices, indices...)
	auto vMeshes = CreateMeshes( pModel, &impSMDFile.sRawHeader, impSMDFile.psaRawObjectInfo, impSMDFile.psaRawObject, vMaterials );

	//Add Meshes to Model
	for ( auto & pMesh : vMeshes )
		pModel->AddMesh( pMesh );

	//Reorder Meshes
	pModel->Modified();

	//Clear Materials
	vMaterials.clear();

	return pModel;
}

SMDModelImporter::SMDModelImporter()
{
	pcMeshImporter = new SMDMeshImporter();
	pcMaterialImporter = new SMDMaterialImporter();
}


SMDModelImporter::~SMDModelImporter()
{
	DELET( pcMaterialImporter );
	DELET( pcMeshImporter );
}
