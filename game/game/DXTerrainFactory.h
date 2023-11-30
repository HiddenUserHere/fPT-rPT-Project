#pragma once

#include "DXTerrain.h"

namespace DX
{

class GraphicsDevice;

typedef std::shared_ptr<class TerrainFactory> TerrainFactory_ptr;

class TerrainFactory
{
public:
								  TerrainFactory( GraphicsDevice * pGraphicsDevice );

	void						  OnLostDevice();
	void						  OnResetDevice();
	
	Terrain_ptr					  MakeTerrain( std::string strTerrainFile );

	void						  RemoveTerrain( Terrain_ptr pTerrainRemove );

private:
	GraphicsDevice				* pGraphicsDevice;

	std::vector<Terrain_ptr>	  vTerrains;

	void						  OnLostDevice( Octree_ptr pOctree );
	void						  OnResetDevice( Octree_ptr pOctree );
};

}