#include "stdafx.h"
#include "DXTerrainFactory.h"

#include "SMDDXTerrainImporter.h"
#include "ImportFile.h"

namespace DX
{

TerrainFactory::TerrainFactory( GraphicsDevice * pGraphicsDevice ) : pGraphicsDevice(pGraphicsDevice)
{
}

void TerrainFactory::OnLostDevice()
{
	for ( auto pTerrain : vTerrains )
	{
		if ( pTerrain->GetRootOctree() )
			OnLostDevice( pTerrain->GetRootOctree() );
	}
}

void TerrainFactory::OnResetDevice()
{
	for ( auto pTerrain : vTerrains )
	{
		if ( pTerrain->GetRootOctree() )
			OnResetDevice( pTerrain->GetRootOctree() );
	}
}

Terrain_ptr TerrainFactory::MakeTerrain( std::string strTerrainFile )
{
	//Get File Extension
	auto strFileExtension = Import::File( strTerrainFile ).GetFileExtension();

	//Terrain Importer
	std::shared_ptr<TerrainImporter> pTerrainImporter;

	//Choose Terrain Importer based on File Extension
	if( strFileExtension == "smd" )
	{
		//SMD Terrain Importer
		pTerrainImporter = std::make_shared<SMDTerrainImporter>();
	}

	//Terrain
	Terrain_ptr pTerrain;

	//Importer chosen?
	if( pTerrainImporter )
	{
		//Load File
		if( pTerrainImporter->Load( strTerrainFile ) )
		{
			//Build Terrain
			pTerrain = pTerrainImporter->Build();
		}
	}
	else
	{
		//Error
		WRITEDBG( "[Terrain Factory] No Importer for File Extension: %s", strFileExtension.c_str() );
	}

	//Terrain Loaded successfully?
	if( pTerrain )
	{
		//Do Nothing...
		vTerrains.push_back( pTerrain );
	}
	else
	{
		//Error
		WRITEDBG( "[Terrain Factory] Failed to Load: %s", strTerrainFile.c_str() );
	}

	//Return Terrain
	return pTerrain;
}

void TerrainFactory::RemoveTerrain( Terrain_ptr pTerrainRemove )
{
	for ( auto it = vTerrains.begin(); it != vTerrains.end(); )
	{
		auto pTerrain = (*it);
		if ( pTerrain == pTerrainRemove )
		{
			pTerrain = nullptr;
			it = vTerrains.erase( it );
		}
		else
			it++;
	}
}

void TerrainFactory::OnLostDevice( Octree_ptr pOctree )
{
	if ( pOctree == nullptr )
		return;

	if ( pOctree->pMesh )
	{
		if ( pOctree->pMesh->GetVertices().size() > 0 )
			pOctree->pMesh->SetDynamicVertexBuffer( nullptr );

		if ( pOctree->pMesh->GetNormals().size() > 0 )
			pOctree->pMesh->SetDynamicNormalBuffer( nullptr );
	}

	for ( auto pChild : pOctree->paChildren )
		OnLostDevice( pChild );
}

void TerrainFactory::OnResetDevice( Octree_ptr pOctree )
{
	if ( pOctree == nullptr )
		return;

	if ( pOctree->pMesh )
	{
		if ( pOctree->pMesh->GetVertices().size() > 0 )
			pOctree->pMesh->SetDynamicVertexBuffer( GetGraphicsDevice()->CreateVertexBuffer( sizeof( X3D::Vector3 ), pOctree->pMesh->GetVertices().size(), TRUE ) );

		if ( pOctree->pMesh->GetNormals().size() > 0 )
			pOctree->pMesh->SetDynamicNormalBuffer( GetGraphicsDevice()->CreateVertexBuffer( sizeof( X3D::Vector3 ), pOctree->pMesh->GetNormals().size(), TRUE ) );
	}

	for ( auto pChild : pOctree->paChildren )
		OnResetDevice( pChild );
}

}