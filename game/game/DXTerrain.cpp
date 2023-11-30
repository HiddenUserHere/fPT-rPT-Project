#include "stdafx.h"
#include "DXTerrain.h"

namespace DX
{

Terrain::Terrain( std::string strTerrainFile ) : strTerrainFile(strTerrainFile)
{
}

Terrain::~Terrain()
{
}

void Terrain::Modified()
{
	vEffects.clear();
	
	if( pRootOctree )
		ModifiedParseOctree( pRootOctree );
}

void Terrain::UpdateEffects( std::function<void( Effect_ptr pEffect )> f )
{
	//Update Effects
	for( auto pEffect : vEffects )
	{
		//Update Effect
		f( pEffect );
	}
}

void Terrain::ModifiedParseOctree( Octree_ptr pOctree )
{
	if( pOctree->pMesh )
	{
		for( auto pMeshPart : pOctree->pMesh->GetOpaqueParts() )
		{
			auto pMaterial = pMeshPart->GetMaterial();

			//Add Effect to Cache
			if( auto pEffect = pMaterial->GetEffect() )
				vEffects.push_back( pEffect );
		}

		for( auto pMeshPart : pOctree->pMesh->GetTransparentParts() )
		{
			auto pMaterial = pMeshPart->GetMaterial();

			//Add Effect to Cache
			if( auto pEffect = pMaterial->GetEffect() )
				vEffects.push_back( pEffect );
		}

		pOctree->pMesh->UpdateBoundingVolumes();

		pOctree->cMeshBoundingAABB = pOctree->pMesh->GetBoundingAABB();
		pOctree->cBoundingAABB = pOctree->cMeshBoundingAABB;
		pOctree->cMeshBoundingSphere = pOctree->pMesh->GetBoundingSphere();
	}
	else
	{
		for( int i = 0; i < _countof( pOctree->paChildren ); i++ )
		{
			if( pOctree->paChildren[i]  )
				ModifiedParseOctree( pOctree->paChildren[i] );
		}
	}
}

}