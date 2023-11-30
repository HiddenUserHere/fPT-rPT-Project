#include "stdafx.h"
#include "DXModelFactory.h"

#include "DXGraphicsDevice.h"
#include "SMDDXModelImporter.h"
#include "ImportFile.h"

namespace DX
{

ModelFactory::ModelFactory( GraphicsDevice * pGraphicsDevice ) : pGraphicsDevice(pGraphicsDevice)
{
	vCache.reserve( 500 );
	vCacheBone.reserve( 250 );
}

void ModelFactory::OnLostDevice()
{
	for ( auto pModel : vCache )
	{
		for ( auto pMesh : pModel->GetMeshes() )
		{
			if ( pMesh->GetDynamicVertexBuffer() )
				pMesh->SetDynamicVertexBuffer( nullptr );

			if ( pMesh->GetDynamicNormalBuffer() )
				pMesh->SetDynamicNormalBuffer( nullptr );
		}
	}
}

void ModelFactory::OnResetDevice()
{
	for ( auto pModel : vCache )
	{
		for ( auto pMesh : pModel->GetMeshes() )
		{
			if ( pMesh->GetVertices().size() > 0 )
				pMesh->SetDynamicVertexBuffer( GetGraphicsDevice()->CreateVertexBuffer( sizeof( X3D::Vector3 ), pMesh->GetVertices().size(), TRUE ) );

			if ( pMesh->GetNormals().size() > 0 )
				pMesh->SetDynamicNormalBuffer( GetGraphicsDevice()->CreateVertexBuffer( sizeof( X3D::Vector3 ), pMesh->GetNormals().size(), TRUE ) );
		}
	}
}

Model_ptr ModelFactory::GetModelCache( std::string strModelFile, std::string strBoneFile )
{
	//Build LowerCase Model File
	std::string strLowerModelFile;
	for ( auto c : strModelFile )
		strLowerModelFile.append( 1, tolower( c ) );

	//Replace ASE with SMD
	if ( auto it = strLowerModelFile.find( ".smd" ); it != std::string::npos )
		strModelFile = strLowerModelFile.replace( it, 4, ".ase" );

	//Build LowerCase Model File
	std::string strLowerBoneFile;
	for ( auto c : strBoneFile )
		strLowerBoneFile.append( 1, tolower( c ) );

	//Replace ASE with SMD
	if ( auto it = strLowerBoneFile.find( ".smb" ); it != std::string::npos )
		strBoneFile = strLowerBoneFile.replace( it, 4, ".ase" );


	//Find same Model in Cache..
	for ( auto pModel : vCache )
	{
		//With Bones?
		if ( strBoneFile.length() != 0 )
		{
			//Same Model File?
			if ( pModel->GetFile().compare( strModelFile ) == 0 )
			{
				//Has Bone Model?
				if ( auto pBoneModel = pModel->GetBoneModel() )
				{
					//Same Bone Model File? Share it
					if ( pBoneModel->GetFile().compare( strBoneFile ) == 0 )
						return pModel;
				}
			}
		}
		else
		{
			//Same Model File? Share it
			if ( pModel->GetFile().compare( strModelFile ) == 0 )
				return pModel;
		}
	}

	return nullptr;
}

Model_ptr ModelFactory::GetBoneCache( std::string strBoneFile )
{
	//Build LowerCase Model File
	std::string strLowerBoneFile;
	for ( auto c : strBoneFile )
		strLowerBoneFile.append( 1, tolower( c ) );

	//Replace ASE with SMD
	if ( auto it = strLowerBoneFile.find( ".smb" ); it != std::string::npos )
		strBoneFile = strLowerBoneFile.replace( it, 4, ".ase" );

	//Find same Model in Cache..
	for ( auto pBone : vCacheBone )
	{
		//Same Model File? Share it
		if ( pBone->GetFile().compare( strBoneFile ) == 0 )
			return pBone;
	}

	return nullptr;
}

Model_ptr ModelFactory::MakeModel( std::string strModelFile, std::string strBoneFile )
{
	//Find same Model in Cache..
	if ( auto pModel = GetModelCache( strModelFile, strBoneFile ) )
		return pModel;

	//Get File Extension
	auto strFileExtension = Import::File( strModelFile ).GetFileExtension();

	//Model Importer
	std::shared_ptr<ModelImporter> pModelImporter;

	//Choose Model Importer based on File Extension
	if( strFileExtension == "ase" )
	{
		//Build LowerCase Model File
		std::string strLowerModelFile;
		for( auto c : strModelFile )
			strLowerModelFile.append( 1, tolower( c ) );

		//Replace ASE with SMD
		if ( auto it = strLowerModelFile.find( ".ase" ); it != std::string::npos )
			strModelFile = strLowerModelFile.replace( it, 4, ".smd" );

		//Build LowerCase Model File
		if ( strBoneFile.length() != 0 )
		{
			std::string strLowerBoneFile;
			for ( auto c : strBoneFile )
				strLowerBoneFile.append( 1, tolower( c ) );

			//Replace ASE with SMD
			if ( auto it = strLowerBoneFile.find( ".ase" ); it != std::string::npos )
				strBoneFile = strLowerBoneFile.replace( it, 4, ".smb" );
		}

		//SMD Model Importer
		pModelImporter = std::make_shared<SMDModelImporter>();
	}
	else if( strFileExtension == "smd" )
	{
		//SMD Model Importer
		pModelImporter = std::make_shared<SMDModelImporter>();
	}

	//Model
	Model_ptr pModel;

	//Importer chosen?
	if( pModelImporter )
	{
		//With Bones?
		if( strBoneFile.length() != 0 )
		{
			//Bone isn't in Cache?
			auto pBoneModel = GetBoneCache( strBoneFile );
			if ( pBoneModel == nullptr )
			{
				//Load Bone File
				if ( pModelImporter->Load( strBoneFile ) )
				{
					//Build Bone Model
					pBoneModel = pModelImporter->Build();

					//Bone Loaded?
					if ( pBoneModel )
					{
						//Remember Bone
						vCacheBone.push_back( pBoneModel );
					}
				}
			}

			//Bone Model Loaded?
			if ( pBoneModel )
			{
				//Load Model File
				if ( pModelImporter->Load( strModelFile ) )
				{
					//Load Model
					pModel = pModelImporter->Build( pBoneModel );
				}
			}
		}
		else
		{
			//Load Model File
			if( pModelImporter->Load( strModelFile ) )
			{
				//Build Model
				pModel = pModelImporter->Build();
			}
		}
	}
	else
	{
		//Error
		WRITEDBG( "[Model Factory] No Importer for File Extension: %s", strFileExtension.c_str() );
	}

	//Model Loaded successfully?
	if( pModel )
	{
		//Remember Model
		vCache.push_back( pModel );
	}
	else
	{
		//Error
		WRITEDBG( "[Model Factory] Failed to Load: %s (%s)", strModelFile.c_str(), strBoneFile.c_str() );
	}
	
	//Return Model
	return pModel;
}

void ModelFactory::AddModel( Model_ptr pNewModel )
{
	//Find same Model in Cache..
	for ( auto pModel : vCache )
	{
		//Same Model File? then OK!
		if ( pModel->GetFile().compare( pNewModel->GetFile() ) == 0 )
			return;
	}

	//Remember Model
	vCache.push_back( pNewModel );
}

struct UnitNewModel
{
	int			iID;
	Model_ptr	pcBodyModel;

	UnitNewModel( int _iID, Model_ptr pcModel ) { iID = _iID; pcBodyModel = pcModel; }
	UnitNewModel() { iID = -1; pcBodyModel = nullptr; };
	~UnitNewModel() {};
};

std::vector<UnitNewModel>	vNewUnitModel;

Model_ptr ModelFactory::MakeModel( UnitData * pcUnitData )
{
	return nullptr;
	Model_ptr pModel = nullptr;
	for ( auto & sUnitModel : vNewUnitModel )
	{
		if ( sUnitModel.iID == pcUnitData->iID )
		{
			pModel = sUnitModel.pcBodyModel;
			break;
		}
	}

	if ( pModel == nullptr )
	{
		static int iModelCount = 0;
		if ( (iModelCount++) > 100 )
			return nullptr;


		if ( pModel = GetGraphicsDevice()->GetModelFactory()->MakeModel( tolower( pcUnitData->pcBody->psModelData->szModelPath ), tolower( pcUnitData->pcBody->psModelData->szBoneModelFilePath ) ) )
			vNewUnitModel.push_back( UnitNewModel( pcUnitData->iID, pModel ) );
	}

	return pModel;
}

void ModelFactory::FreeModel( UnitData * pcUnitData )
{
	for ( auto it = vNewUnitModel.begin(); it != vNewUnitModel.end(); )
	{
		auto & sUnitModel = (*it);

		if ( sUnitModel.iID == pcUnitData->iID )
		{
			it = vNewUnitModel.erase( it );
		}
		else
			it++;
	}
}

}