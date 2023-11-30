#pragma once

#include "DXMesh.h"


namespace DX
{

const int MAX_OCTREE_TRIANGLES = 1000;

typedef std::shared_ptr<struct Octree> Octree_ptr;

struct Octree
{
	X3D::AABB			  cBoundingAABB;
	X3D::Sphere			  cBoundingSphere;
	
	X3D::AABB			  cMeshBoundingAABB;
	X3D::Sphere			  cMeshBoundingSphere;
	Mesh_ptr			  pMesh;

	Octree_ptr			  paChildren[8];
};

typedef std::shared_ptr<class Terrain> Terrain_ptr;

class Terrain
{
public:

public:
								  Terrain( std::string strTerrainFile );
	virtual						 ~Terrain();

	Octree_ptr					  GetRootOctree() { return pRootOctree; }
	void						  SetRootOctree( Octree_ptr _pRootOctree ) { pRootOctree = _pRootOctree; }

	void						  Modified();
	void						  UpdateEffects( std::function<void( Effect_ptr pEffect )> f );

private:
	void						  ModifiedParseOctree( Octree_ptr pOctree );

private:
	std::string					  strTerrainFile;
	
	/**
	 * Octree of the Terrain
	 */
	Octree_ptr					  pRootOctree;

	/**
	 * Cache of all Effects used by the Materials of Mesh Parts inside this Model
	 * This list is updated when Modified() is called
	 * This list must be updated when a Mesh or MeshPart is added, modified or removed from this Model
	 */
	std::vector<Effect_ptr>		  vEffects;
};

}