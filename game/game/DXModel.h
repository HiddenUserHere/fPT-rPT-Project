#pragma once

#include "DXMesh.h"

namespace DX
{

typedef std::shared_ptr<class Model> Model_ptr;

class Model
{
public:
									  Model( std::string strModelFile );
	virtual							 ~Model();

	const std::string				& GetFile() const { return strModelFile; }

	void							  SetBoneModel( Model_ptr pModel ) { pBoneModel = pModel; }
	Model_ptr						  GetBoneModel() { return pBoneModel; }

	const std::vector<X3D::Matrix4>	& GetBoneMatrices() const { return vBoneMatrices; }

	void							  AddMesh( Mesh_ptr pMesh );
	const std::vector<Mesh_ptr>		& GetMeshes() const { return vMeshes; }

	const Mesh_ptr					  GetMesh( const std::string strName );

	int								  GetMaxFrame() const;

	void							  Modified();
	void							  UpdateEffects( std::function<void( Effect_ptr pEffect )> f );

	void							  SetFrame( int iFrame );
	void							  CacheBoneMatrices();

protected:
	void							  ReorderMeshes();

protected:
	std::string						  strModelFile;

	/**
	 * Optional Link to Model with this Model's Bones
	 */
	Model_ptr						  pBoneModel;

	/**
	 * List of Bone Matrices for the current Frame of each Bone Mesh in the Bone Model
	 */
	std::vector<X3D::Matrix4>		  vBoneMatrices;

	/**
	 * List of all Meshes used in this Model
	 */
	std::vector<Mesh_ptr>			  vMeshes;

	/**
	 * List of all Meshes ordered by Parenthood (Parents first in List)
	 */
	std::vector<Mesh_ptr>			  vOrderedMeshes;

	/**
	 * Cache of all Effects used by the Materials of Mesh Parts inside this Model
	 * This list is updated when Modified() is called
	 * This list must be updated when a Mesh or MeshPart is added, modified or removed from this Model
	 */
	std::vector<Effect_ptr>			  vEffects;

	/**
	 * Highest Possible Frame of Meshes inside
	 */
	int								  iMaxFrame = 0;
};

}