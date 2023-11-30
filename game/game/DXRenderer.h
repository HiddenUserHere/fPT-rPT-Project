#pragma once

#include "DXGraphicsDevice.h"
#include "DXLens.h"
#include "DXCamera.h"
#include "DXEffect.h"
#include "DXModel.h"
#include "DXMesh.h"
#include "DXMeshPart.h"
#include "DXTerrain.h"

#include "X3DFrustum.h"
#include "X3DAABB.h"
#include "X3DQuad.h"
#include "X3DVector3.h"

namespace DX
{

struct OctreeDrawJob
{
	Mesh_ptr	  pMesh;
	float		  fDistance;
};

typedef std::shared_ptr<class Renderer> Renderer_ptr;

class Renderer : public std::enable_shared_from_this<Renderer>
{
public:
							  Renderer( GraphicsDevice_ptr pGraphicsDevice );
	virtual					 ~Renderer();

	GraphicsDevice_ptr		  GetGraphicsDevice() { return pGraphicsDevice; }

	BOOL					  IsDeviceReady();
	BOOL					  Begin();
	void					  End();

	void					  ShowMeshBoundingVolumes( BOOL b );
	void					  ShowOctreeBoundingVolumes( BOOL b );

	BOOL					  GetLockViewFrustum() { return bLockViewFrustum; }
	void					  SetLockViewFrustum( BOOL b ) { bLockViewFrustum = b; }

	BOOL					  GetShowViewFrustm() { return bShowViewFrustum; }
	void					  SetShowViewFrustum( BOOL b ) { bShowViewFrustum = b; }

	BOOL					  GetShowNormals() { return bShowNormals; }
	void					  SetShowNormals( BOOL b ) { bShowNormals = b; }
	
	void					  SetTime( float fNewTime );

	void					  SetLens( Lens_ptr pLens );
	Lens_ptr				  GetLens() { return pActiveLens; }

	void					  SetCamera( Camera_ptr pCamera );
	Camera_ptr				  GetActiveCamera() { return pActiveCamera; }

	void					  PushWorldMatrix( const X3D::Matrix4 & m );
	void					  PopWorldMatrix();

	const X3D::Frustum		& GetViewFrustum() const { return cViewFrustum; }

	const X3D::Vector4		& GetAmbientLight() { return vAmbientLight; }
	void					  SetAmbientLight( const X3D::Vector4 & vLight ) { vAmbientLight = vLight; }

	void					  SetOverrideMaterialAlphaBlend( Material::EAlphaBlend eAlphaBlend );
	void					  ResetOverrideMaterialAlphaBlend();

	void					  DrawModel( Model_ptr pModel );

	void					  DrawTerrain( Terrain_ptr pTerrain );

	D3DXMATRIX				  & GetViewMatrix() { return mViewMatrix; }
	D3DXMATRIX				  & GetProjectionMatrix() { return mProjectionMatrix; }
	
protected:
	void					  DrawOctree( Octree_ptr pOctree );
	void					  PushOctreeDrawJob( Mesh_ptr pMesh, float fDistance );
	void					  FlushOctreeDrawJobs();

	void					  DrawMesh( Mesh_ptr pMesh, bool bOpaqueParts, bool bTransparentParts, bool bDebugBoundingVolumes );
	void					  DrawMeshPart( MeshPart_ptr pMesh, UINT uNumVertices );

	BOOL					  IsVisibleInFrustum( const X3D::Frustum & rFrustum, const X3D::Sphere & rSphere ) const;
	BOOL					  IsVisibleInFrustum( const X3D::Frustum & rFrustum, const X3D::AABB & rAABB ) const;

	void					  DrawDebugFrustum( const X3D::Frustum & rFrustum );
	
	void					  DrawMeshDebugNormals( const Mesh_ptr pMesh );
	void					  DrawOctreeDebugVolumes( const Octree_ptr pOctree );

	void					  DrawDebugAABB( const X3D::AABB & cAABB );
	void					  DrawDebugSphere( const X3D::Sphere & cSphere );

	void					  DrawDebugQuad( const X3D::Quad & cQuad );
	void					  DrawDebugLine( const X3D::Vector3 & cPoint1, const X3D::Vector3 & cPoint2 );

	void					  ApplyMaterial( Mesh_ptr pMesh, MeshPart_ptr pMeshPart, int iStreamNumber );

	void					  SkinMesh( Model_ptr pModel, Mesh_ptr pMesh );

protected:
	GraphicsDevice_ptr		  pGraphicsDevice;

	Effect_ptr				  pDebugEffect;
	LPD3DXMESH				  lpDebugSphereMesh;

	BOOL					  bShowMeshBoundingAABB = FALSE;
	BOOL					  bShowMeshBoundingSphere = FALSE;

	BOOL					  bShowOctreeBoundingAABB = FALSE;
	BOOL					  bShowOctreeBoundingSphere = FALSE;

	BOOL					  bLockViewFrustum = FALSE;
	BOOL					  bShowViewFrustum = FALSE;

	BOOL					  bShowNormals = FALSE;

	float					  fTime = 0.0f;

	Lens_ptr				  pActiveLens;
	Camera_ptr				  pActiveCamera;

	D3DXMATRIX				  mProjectionMatrix;
	D3DXMATRIX				  mViewMatrix;
	D3DXMATRIX				  mViewProjectionMatrix;
	D3DXMATRIX				  mWorldViewProjectionMatrix;

	LPD3DXMATRIXSTACK		  lpWorldMatrixStack;
	std::stack<X3D::Matrix4>  sWorldMatrixStack;

	X3D::Frustum			  cViewFrustum;

	X3D::Vector4			  vAmbientLight;

	BOOL					  bOverrideMaterialAlphaBlend = FALSE;
	Material::EAlphaBlend	  eOverrideMaterialAlphaBlend;

	std::list<OctreeDrawJob>	  lOctreeDrawJobs;
};

bool Compare_OctreeDrawJob_DistanceOrderBackToFront( const OctreeDrawJob & r1, const OctreeDrawJob & r2 );
bool Compare_OctreeDrawJob_DistanceOrderFrontToBack( const OctreeDrawJob & r1, const OctreeDrawJob & r2 );

}