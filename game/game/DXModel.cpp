#include "stdafx.h"
#include "DXModel.h"

namespace DX
{

Model::Model( std::string strModelFile ) : strModelFile(strModelFile)
{
}

Model::~Model()
{
}

void Model::AddMesh( Mesh_ptr pMesh )
{
	//Add Mesh to List
	vMeshes.push_back( pMesh );

	//Set new Max Frame of Model if new Mesh has higher Max Frame
	if( pMesh->GetMaxFrame() > iMaxFrame )
		iMaxFrame = pMesh->GetMaxFrame();
}

const Mesh_ptr Model::GetMesh( const std::string strName )
{
	for ( auto pcMesh : vMeshes )
		if ( pcMesh->GetName().compare( strName ) == 0 )
			return pcMesh;

	return nullptr;
}

int Model::GetMaxFrame() const
{
	if( pBoneModel )
		return pBoneModel->GetMaxFrame();

	return iMaxFrame;
}

void Model::Modified()
{
	//Clear List of Effects
	vEffects.clear();

	//Add Effects of Materials in this Model's Meshes to the List of Effects
	for( auto pMesh : vMeshes )
	{
		for( auto pMeshPart : pMesh->GetOpaqueParts() )
		{
			auto pMaterial = pMeshPart->GetMaterial();

			//Get Effect of Material
			if( auto pEffect = pMaterial->GetEffect() )
			{
				//Not Yet in Cache?
				if( std::find( vEffects.begin(), vEffects.end(), pEffect ) == vEffects.end() )
				{
					//Add Effect to Cache
					vEffects.push_back( pEffect );
				}
			}
		}

		for( auto pMeshPart : pMesh->GetTransparentParts() )
		{
			auto pMaterial = pMeshPart->GetMaterial();
			
			//Get Effect of Material
			if( auto pEffect = pMaterial->GetEffect() )
			{
				//Not Yet in Cache?
				if( std::find( vEffects.begin(), vEffects.end(), pEffect ) == vEffects.end() )
				{
					//Add Effect to Cache
					vEffects.push_back( pEffect );
				}
			}
		}
	}

	//Build list of Meshes ordered by Parenthood
	ReorderMeshes();
}

void Model::UpdateEffects( std::function<void( Effect_ptr pEffect )> f )
{
	//Update Effects
	for( auto pEffect : vEffects )
	{
		//Update Effect
		f( pEffect );
	}
}

void Model::SetFrame( int iFrame )
{
	//Set Frame of Bone Model
	if( pBoneModel )
	{
		//Set Bone Model Frame
		pBoneModel->SetFrame( iFrame );

		//Copy Bone Matrices
		CacheBoneMatrices();

		//Update Bounding Volumes of Meshes
		for( auto pMesh : vOrderedMeshes )
		{
			//Update Bounding Volumes
			pMesh->UpdateBoundingVolumes( &vBoneMatrices );
		}
	}
	else
	{
		//Set Frame of Meshes
		for( auto pMesh : vOrderedMeshes )
		{
			//Set Frame
			pMesh->SetFrame( iFrame );
		}

		//Update Bounding Volumes of Meshes
		for( auto pMesh : vOrderedMeshes )
		{
			//Update Bounding Volumes
			pMesh->UpdateBoundingVolumes();
		}
	}
}

void Model::CacheBoneMatrices()
{
	//Reset Bone Matrices List
	vBoneMatrices.clear();

	//Fill Bone Matrices List
	vBoneMatrices.reserve( pBoneModel->GetMeshes().size() );
	for( auto pMesh : pBoneModel->GetMeshes() )
		vBoneMatrices.push_back( pMesh->GetLocalMatrix() );
}

void Model::ReorderMeshes()
{
	//Clear List of Ordered Meshes
	vOrderedMeshes.clear();

	//Build a Reserve
	vOrderedMeshes.reserve( vMeshes.size() );

	//Put Root Meshes on Top of the List
	for( auto pMesh : vMeshes )
	{
		if( pMesh->GetParent() == nullptr )
			vOrderedMeshes.push_back( pMesh );
	}

	//Put Child Meshes in the List
	for( auto pMesh : vMeshes )
	{
		if( pMesh->GetParent() == nullptr )
			continue;

		std::list<Mesh_ptr> vParentList;

		auto pParent = pMesh->GetParent();
		while( pParent )
		{
			bool bExistsInList = false;

			for( auto pOrderedMesh : vOrderedMeshes )
			{
				if( pOrderedMesh == pParent )
				{
					bExistsInList = true;
					break;
				}
			}

			if( bExistsInList )
				break;

			vParentList.push_front( pParent );

			pParent = pParent->GetParent();
		}

		for( auto pParent : vParentList )
			vOrderedMeshes.push_back( pParent );

		vOrderedMeshes.push_back( pMesh );
	}
}

}