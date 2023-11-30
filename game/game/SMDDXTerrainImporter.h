#pragma once
#include "DXTerrainImporter.h"
class SMDTerrainImporter : public DX::TerrainImporter
{
private:
	struct UVCoordinates
	{
		float fU;
		float fV;
	};

	std::string			_strFilePath;

	float				fMapCenterX = 0.0f;
	float				fMapCenterY = 0.0f;
	float				fMapCenterZ = 0.0f;

	const UINT			MAX_TERRAIN_FACES = 24000;

	const UINT			MAX_TERRAIN_BLOCKS = 8;
	UINT				uCurrentBlocksCount = 0;

	UINT				uMaxTerrainFaces = 0;

	SMDMeshImporter		* pcMeshImporter = nullptr;
	SMDMaterialImporter	* pcMaterialImporter = nullptr;

	DX::Octree_ptr		CreateOctree();

	X3D::Vector3		GetNodeCenter( const X3D::Vector3 vCenter, float fTerrainWidth, int iIndex );

	DX::Mesh_ptr		CreateMeshes( DWORD * paFaceList, int iFaceCount );

	void				SetTextureCoordinate( DX::Mesh_ptr pMesh, struct RawSMDTerrain * psRawTerrain, std::vector<std::vector<UVCoordinates>> & vTextureCoordinates );

	void				GetDimension( struct RawSMDTerrain * psRawTerrain, float & fCenterX, float & fCenterY, float & fCenterZ, float & fTerrainWidth );

	bool				IsInsideTriangle( struct RawSMDTerrain * psRawTerrain, int iIndex, const X3D::Vector3 vTerrainCenter, float fWidth );

    void				GetFaceTriangles( struct RawSMDTerrain * psRawTerrain, const X3D::Vector3 vTerrainCenter, float fWidth, std::vector<int> & vFaceID );
    int					GetFaceTrianglesCount( struct RawSMDTerrain * psRawTerrain, const X3D::Vector3 vTerrainCenter, float fWidth );

	void				CreateNode( struct RawSMDTerrain * psRawTerrain, DX::Octree_ptr pOctree, X3D::Vector3 vTerrainCenter, float fTerrainWidth, std::vector<SMDMeshImporter::SMDMeshVertexData> & vVertexData, std::vector<DX::Material_ptr> & vMaterials );

	void				CreateChildNode( int iIndexNode, struct RawSMDTerrain * psRawTerrain, DX::Octree_ptr pOctree, X3D::Vector3 vTerrainCenter, float fTerrainWidth, std::vector<bool> vVertexAdd, std::vector<SMDMeshImporter::SMDMeshVertexData> & vVertexData, std::vector<DX::Material_ptr> & vMaterials );

public:
	SMDTerrainImporter();
	virtual ~SMDTerrainImporter();

	bool Load( const std::string & strFilePath );

	DX::Terrain_ptr Build();
};

