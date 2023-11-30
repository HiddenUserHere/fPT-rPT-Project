#pragma once

#include "DXVertexBuffer.h"
#include "DXMeshPart.h"

#include "X3DQuaternion.h"
#include "X3DAABB.h"
#include "X3DSphere.h"
#include "X3DMatrix4.h"

namespace DX
{

struct KeyScaling
{
	int				  iFrame;
	X3D::Vector3	  vScaling;
};

struct KeyRotation
{
	int				  iFrame;
	X3D::Quaternion	  qRotation;
	X3D::Matrix4	  mRotation;

	KeyRotation() : qRotation( 0.0f, 0.0f, 0.0f, 0.0f ){ };
	~KeyRotation() {};
};

struct KeyTranslation
{
	int				  iFrame;
	X3D::Vector3	  vTranslation;
};

typedef std::shared_ptr<class Mesh> Mesh_ptr;

class Mesh
{
	friend class Model;
	friend class Terrain;

public:
											  Mesh( std::string strName );
	virtual									 ~Mesh();

	const std::string						& GetName() const { return strName; }

	void									  SetDynamicVertexBuffer( VertexBuffer_ptr _pVertexBuffer ) { pDynamicVertexBuffer = _pVertexBuffer; }
	VertexBuffer_ptr						  GetDynamicVertexBuffer() { return pDynamicVertexBuffer; }

	void									  SetDynamicNormalBuffer( VertexBuffer_ptr _pVertexBuffer ) { pDynamicNormalBuffer = _pVertexBuffer; }
	VertexBuffer_ptr						  GetDynamicNormalBuffer() { return pDynamicNormalBuffer; }

	void									  SetVertexBuffer( VertexBuffer_ptr _pVertexBuffer ) { pVertexBuffer = _pVertexBuffer; }
	VertexBuffer_ptr						  GetVertexBuffer() { return pVertexBuffer; }

	void									  SetNormalBuffer( VertexBuffer_ptr _pNormalBuffer ) { pNormalBuffer = _pNormalBuffer; }
	VertexBuffer_ptr						  GetNormalBuffer() { return pNormalBuffer; }

	void									  SetColorBuffer( VertexBuffer_ptr _pColorBuffer ) { pColorBuffer = _pColorBuffer; }
	VertexBuffer_ptr						  GetColorBuffer() { return pColorBuffer; }

	void									  SetSkinIndicesBuffer( VertexBuffer_ptr _pSkinIndicesBuffer ) { pSkinIndicesBuffer = _pSkinIndicesBuffer; }
	VertexBuffer_ptr						  GetSkinIndicesBuffer() { return pSkinIndicesBuffer; }

	void									  AddTextureCoordBuffer( VertexBuffer_ptr pTextureCoordBuffer ) { vTextureCoordBuffers.push_back( pTextureCoordBuffer ); }
	const std::vector<VertexBuffer_ptr>		& GetTextureCoordBuffers() const { return vTextureCoordBuffers; }

	void									  AddOpaquePart( MeshPart_ptr pPart );
	const std::list<MeshPart_ptr>			& GetOpaqueParts() const { return lOpaqueParts; }

	void									  AddTransparentPart( MeshPart_ptr pPart, BOOL bEnd = FALSE );
	const std::list<MeshPart_ptr>			& GetTransparentParts() const { return lTransparentParts; }

	void									  SetKeyScalings( const std::vector<KeyScaling> & v ) { vKeyScalings = v; }
	const std::vector<KeyScaling>			& GetKeyScalings() const { return vKeyScalings; }

	void									  SetKeyRotations( const std::vector<KeyRotation> & v ) { vKeyRotations = v; }
	const std::vector<KeyRotation>			& GetKeyRotations() const { return vKeyRotations; }

	void									  SetKeyTranslations( const std::vector<KeyTranslation> & v ) { vKeyTranslations = v; }
	const std::vector<KeyTranslation>		& GetKeyTranslations() const { return vKeyTranslations; }

	const X3D::AABB							& GetBoundingAABB() const { return cBoundingAABB; }
	const X3D::Sphere						& GetBoundingSphere() const { return cBoundingSphere; }

	void									  SetVertices( const std::vector<X3D::Vector3> & v ) { vVertices = v; }
	const std::vector<X3D::Vector3>			& GetVertices() const { return vVertices; }
	
	void									  SetNormals( const std::vector<X3D::Vector3> & v ) { vNormals = v; }
	const std::vector<X3D::Vector3>			& GetNormals() const { return vNormals; }
	
	void									  SetSkinIndices( const std::vector<float> & v ) { vSkinIndices = v; }
	const std::vector<float>				& GetSkinIndices() const { return vSkinIndices; }

	void									  SetTransformation( const X3D::Matrix4 & m ) { mTransformation = m; }
	const X3D::Matrix4						& GetTransformation() const { return mTransformation; }

	void									  SetTransformationRotation( const X3D::Matrix4 & m ) { mTransformationRotation = m; }
	const X3D::Matrix4						& GetTransformationRotation() const { return mTransformationRotation; }

	void									  SetTransformationTranslation( const X3D::Matrix4 & m ) { mTransformationTranslation = m; }
	const X3D::Matrix4						& GetTransformationTranslation() const { return mTransformationTranslation; }

	void									  SetTransformationInverse( const X3D::Matrix4 & m ) { mTransformationInverse = m; }
	const X3D::Matrix4						& GetTransformationInverse() const { return mTransformationInverse; }

	const X3D::Matrix4						& GetFrameMatrix() const { return mFrame; }
	const X3D::Matrix4						& GetLocalMatrix() const { return mLocal; }
	const X3D::Matrix4						& GetWorldMatrix() const { return bUseLocalMatrix ? mLocal : mFrame; }


	void									  SetUseLocalMatrix( BOOL b ) { bUseLocalMatrix = b; }
	BOOL									  GetUseLocalMatrix() const { return bUseLocalMatrix; }

	int										  GetFrame() const { return iFrame; }

	void									  SetMaxFrame( int i ) { iMaxFrame = i; }
	int										  GetMaxFrame() const { return iMaxFrame; }

	void									  SetParent( Mesh_ptr pMesh ) { pParent = pMesh; }
	Mesh_ptr								  GetParent() { return pParent; }
	
protected:
	void									  SetFrame( int iNewFrame );
	void									  UpdateBoundingVolumes( const std::vector<X3D::Matrix4> * pvBoneMatrices = NULL );

	X3D::Matrix4							  GetScaling( int iFrame );
	X3D::Matrix4							  GetRotation( int iFrame );
	X3D::Matrix4							  GetTranslation( int iFrame );

protected:
	std::string								  strName;

	VertexBuffer_ptr						  pDynamicVertexBuffer;
	VertexBuffer_ptr						  pDynamicNormalBuffer;

	VertexBuffer_ptr						  pVertexBuffer;
	VertexBuffer_ptr						  pNormalBuffer;
	VertexBuffer_ptr						  pColorBuffer;
	std::vector<VertexBuffer_ptr>			  vTextureCoordBuffers;
	VertexBuffer_ptr						  pSkinIndicesBuffer;

	std::list<MeshPart_ptr>					  lOpaqueParts;
	std::list<MeshPart_ptr>					  lTransparentParts;
	
	std::vector<KeyScaling>					  vKeyScalings;
	std::vector<KeyRotation>				  vKeyRotations;
	std::vector<KeyTranslation>				  vKeyTranslations;

	X3D::AABB								  cBoundingAABB;
	X3D::Sphere								  cBoundingSphere;

	std::vector<X3D::Vector3>				  vVertices;
	std::vector<X3D::Vector3>				  vNormals;
	std::vector<float>						  vSkinIndices;

	X3D::Matrix4							  mTransformation;
	X3D::Matrix4							  mTransformationRotation;
	X3D::Matrix4							  mTransformationTranslation;
	X3D::Matrix4							  mTransformationInverse;

	X3D::Matrix4							  mFrame;
	X3D::Matrix4							  mLocal;

	BOOL									  bUseLocalMatrix = FALSE;

	int										  iFrame = -1;
	int										  iMaxFrame = -1;

	Mesh_ptr								  pParent;
};

}