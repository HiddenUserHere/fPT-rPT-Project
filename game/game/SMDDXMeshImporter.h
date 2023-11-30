#pragma once
class SMDMeshImporter
{
private:
	struct UVCoordinates
	{
		float fU;
		float fV;

		UVCoordinates() { fU = 0.0f; fV = 0.0f; };
		UVCoordinates( float u, float v ) { fU = u; fV = v; };
		~UVCoordinates() {};
	};

	struct UVCoordinatesEx
	{
		int	  iVertexID;

		float fU;
		float fV;
	};

public:
	struct SMDMeshVertexData
	{
		int				iVertexID;
		X3D::Vector3	sVertex;
		X3D::Vector3	sNormal;
		D3DCOLOR		d3dColor;
		std::vector<UVCoordinates> vUV;
		int				iBoneID;
	};

	SMDMeshImporter();
	virtual ~SMDMeshImporter();

	void				SetVertices( DX::Model_ptr pModel, DX::Mesh_ptr pMesh, RawSMDObject * psObject, std::vector<SMDMeshVertexData> & vVertexData, std::vector<DX::Material_ptr> & vMaterials );

	void				SetSkinIndices( DX::Model_ptr pModel, DX::Mesh_ptr pMesh, RawSMDObject * psRawObject );

	void				CreateVertexDeclaration( DX::Mesh_ptr pMesh, DX::MeshPart_ptr pMeshPart );

	void				CreateIndices( DX::MeshPart_ptr pMeshPart, std::vector<unsigned short> & vIndices );

	void				CreateTextureCoordinates( DX::Mesh_ptr pMesh, RawSMDObject * psObject, std::vector<DX::Material_ptr> & vMaterials, std::vector<SMDMeshVertexData> & vVertexData );

	void				SetFaces( DX::Model_ptr pModel, DX::Mesh_ptr pMesh, struct RawSMDObject * psObject, std::vector<DX::Material_ptr> & vMaterials );

	void				SetKeys( DX::Mesh_ptr pMesh, struct RawSMDObject * psObject );

	void				SetMatrices( DX::Mesh_ptr pMesh, RawSMDObject * psObject );

	void				UpdateMeshParent( std::vector<DX::Mesh_ptr> & vMeshes, RawSMDObject * psaRawObject );

	void				SetVerticesBuffers( DX::Mesh_ptr pMesh, std::vector<D3DCOLOR> & vColors );

	void				SetTerrainVertices( DX::Mesh_ptr pMesh, struct RawSMDTerrain * psSMDTerrain, std::vector<SMDMeshVertexData> & vVertexData, std::vector<DX::Material_ptr> & vMaterials );
	void				CreateTerrainTextureCoordinates( DX::Mesh_ptr pMesh, RawSMDTerrain * psSMDTerrain, std::vector<DX::Material_ptr> & vMaterials, std::vector<SMDMeshVertexData> & vVertexData );
	void				SetTerrainFaces( DX::Mesh_ptr pMesh, RawSMDTerrain * psSMDTerrain, std::vector<DX::Material_ptr> & vMaterials, std::vector<SMDMeshVertexData> & vVertexData );

	void				LoadTerrainVertices( struct RawSMDTerrain * psSMDTerrain, std::vector<SMDMeshVertexData> & vVertexData, std::vector<DX::Material_ptr> & vMaterials );
};

