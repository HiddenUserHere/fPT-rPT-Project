#include "stdafx.h"
#include "DXRenderer.h"

#include "DX.h"

static BOOL bHardwareSkinning = TRUE;

using namespace X3D;

namespace DX
{

Renderer::Renderer( GraphicsDevice_ptr pGraphicsDevice ) : pGraphicsDevice(pGraphicsDevice)
{
	D3DXMatrixIdentity( &mProjectionMatrix );
	D3DXMatrixIdentity( &mViewMatrix );
	D3DXMatrixIdentity( &mViewProjectionMatrix );
	D3DXMatrixIdentity( &mWorldViewProjectionMatrix );
	
	pDebugEffect = pGraphicsDevice->GetEffectFactory()->MakeEffect( "game\\scripts\\shaders\\Debug.fx" );

	D3DXCreateSphere( pGraphicsDevice->Get(), 1.0f, 32, 32, &lpDebugSphereMesh, NULL );

	D3DXCreateMatrixStack( 0, &lpWorldMatrixStack );
	lpWorldMatrixStack->Push();
	lpWorldMatrixStack->LoadIdentity();

	ShowMeshBoundingVolumes( FALSE );

	ShowOctreeBoundingVolumes( FALSE );

	bShowViewFrustum = FALSE;
}

Renderer::~Renderer()
{
	RELEASE( lpWorldMatrixStack );
	
	RELEASE( lpDebugSphereMesh );

	pDebugEffect = nullptr;
}

BOOL Renderer::IsDeviceReady()
{
	return TRUE;
}

BOOL Renderer::Begin()
{
	if( !pGraphicsDevice->Begin() )
		return FALSE;

	pGraphicsDevice->Clear( TRUE, TRUE, TRUE );

	D3DXMatrixIdentity( &mProjectionMatrix );
	D3DXMatrixIdentity( &mViewMatrix );
	D3DXMatrixIdentity( &mViewProjectionMatrix );
	D3DXMatrixIdentity( &mWorldViewProjectionMatrix );

	PushWorldMatrix( X3D::Matrix4() );

	return TRUE;
}

void Renderer::End()
{
	if( bShowViewFrustum )
		DrawDebugFrustum( cViewFrustum );

	PopWorldMatrix();

	pActiveCamera = nullptr;
	pActiveLens = nullptr;

	pGraphicsDevice->End();
}

void Renderer::ShowMeshBoundingVolumes( BOOL b )
{
	bShowMeshBoundingAABB = b;
	bShowMeshBoundingSphere = b;
}

void Renderer::ShowOctreeBoundingVolumes( BOOL b )
{
	bShowOctreeBoundingAABB = b;
	bShowOctreeBoundingSphere = b;
}

void Renderer::SetTime( float fNewTime )
{
	fTime = fNewTime;
}

void Renderer::SetLens( Lens_ptr pLens )
{
	pActiveLens = pLens;

	D3DXMatrixPerspectiveFovLH( &mProjectionMatrix, D3DXToRadian( pLens->GetFOVY() ), pLens->GetRatio(), pLens->GetZNear(), pLens->GetZFar() );


	//View Frustum isn't locked?
	if( !bLockViewFrustum )
	{
		//Set Scaling
		cViewFrustum.scale( 1.0f );

		//Set Perspective
		cViewFrustum.perspective( pLens->GetFOVY(), pLens->GetRatio(), pLens->GetZNear(), pLens->GetZFar() );
	}
}

void Renderer::SetCamera( Camera_ptr pCamera )
{
	pActiveCamera = pCamera;

	D3DXMatrixLookAtLH( &mViewMatrix, &X3DVector3ToD3DXVector3( pCamera->GetEye() ), &X3DVector3ToD3DXVector3( pCamera->GetLookAt() ), &X3DVector3ToD3DXVector3( pCamera->GetUp() ) );

	mViewProjectionMatrix = mViewMatrix * mProjectionMatrix;
	mWorldViewProjectionMatrix = *lpWorldMatrixStack->GetTop() * mViewProjectionMatrix;

	//View Frustum isn't locked?
	if ( !bLockViewFrustum )
	{
		//Set Look At
		cViewFrustum.lookAt( pCamera->GetEye(), pCamera->GetLookAt(), pCamera->GetUp() );

		//Build Frustum
		cViewFrustum.build();
	}
}


void Renderer::PushWorldMatrix( const X3D::Matrix4 & m )
{
	//X3D Matrix Stack
	{
		if( sWorldMatrixStack.empty() )
			sWorldMatrixStack.push( m );
		else
			sWorldMatrixStack.push( m * sWorldMatrixStack.top() );
	}

	//D3DX Matrix Stack
	{
		D3DXMATRIX mWorldMatrix = X3DMatrixToD3DXMatrix( m );

		lpWorldMatrixStack->Push();
		lpWorldMatrixStack->MultMatrixLocal( &mWorldMatrix );
	}

	//Update WorldViewProjection Matrix
	mWorldViewProjectionMatrix = *lpWorldMatrixStack->GetTop() * mViewProjectionMatrix;
}

void Renderer::PopWorldMatrix()
{
	//X3D Matrix Stack
	sWorldMatrixStack.pop();

	//D3DX Matrix Stack
	lpWorldMatrixStack->Pop();

	//Update WorldViewProjection Matrix
	mWorldViewProjectionMatrix = *lpWorldMatrixStack->GetTop() * mViewProjectionMatrix;
}

void Renderer::SetOverrideMaterialAlphaBlend( Material::EAlphaBlend eAlphaBlend )
{
	bOverrideMaterialAlphaBlend = TRUE;
	eOverrideMaterialAlphaBlend = eAlphaBlend;
}

void Renderer::ResetOverrideMaterialAlphaBlend()
{
	bOverrideMaterialAlphaBlend = FALSE;
}

void Renderer::DrawModel( Model_ptr pModel )
{
	//Update Effects
	pModel->UpdateEffects( [&]( Effect_ptr pEffect )
	{
		pEffect->SetMatrix( "WorldViewProjection", mWorldViewProjectionMatrix );
		pEffect->SetMatrix( "ViewProjection", mViewProjectionMatrix );
		pEffect->SetMatrix( "Projection", mProjectionMatrix );
		pEffect->SetMatrix( "View", mViewMatrix );
		pEffect->SetMatrix( "World", *lpWorldMatrixStack->GetTop() );
		pEffect->SetFloat( "Time", fTime );
		pEffect->SetFloatArray( "AmbientLight", vAmbientLight.f, 4 );

		if ( pModel->GetBoneModel() && bHardwareSkinning )
		{
			D3DLOCKED_RECT d3dRect;
			RECT rc;
			rc.left = 0;
			rc.top = 0;
			rc.right = GRAPHICENGINE->GetBoneTexture()->GetWidth();
			rc.bottom = GRAPHICENGINE->GetBoneTexture()->GetHeight();

			if ( SUCCEEDED( GRAPHICENGINE->GetBoneTexture()->Get()->LockRect( 0, &d3dRect, &rc, D3DLOCK_DISCARD ) ) )
			{
				BYTE * pbaData = (BYTE *)d3dRect.pBits;
				CopyMemory( pbaData, (D3DXMATRIX *)pModel->GetBoneMatrices().data(), sizeof( D3DXMATRIX ) * pModel->GetBoneMatrices().size() );
				GRAPHICENGINE->GetBoneTexture()->Get()->UnlockRect( 0 );
			}

			pEffect->SetInt( "BoneCount", pModel->GetBoneMatrices().size() );
			pEffect->SetBool( "HardwareSkinning", TRUE );

			HRESULT hr;
			if ( FAILED( hr = pEffect->Get()->SetTexture( "tAnimations", GRAPHICENGINE->GetBoneTexture()->Get() ) ) )
				WRITEDBG( "Effect Failed 0x%08X", hr );
		}
	});

	//Draw Meshes in Model
	for( auto pMesh : pModel->GetMeshes() )
	{
//		if ( !IsVisibleInFrustum( cViewFrustum, pMesh->GetBoundingSphere() ) )
//			continue;

		//Skin Mesh
		SkinMesh( pModel, pMesh );

		//Draw Mesh
		DrawMesh( pMesh, true, true, true );
	}
}

void Renderer::DrawTerrain( Terrain_ptr pTerrain )
{
	//Update Effects
	pTerrain->UpdateEffects([&]( Effect_ptr pEffect )
	{
		pEffect->SetMatrix( "WorldViewProjection", mWorldViewProjectionMatrix );
		pEffect->SetMatrix( "ViewProjection", mViewProjectionMatrix );
		pEffect->SetMatrix( "World", *lpWorldMatrixStack->GetTop() );
		pEffect->SetFloat( "Time", fTime );
		pEffect->SetFloatArray( "AmbientLight", vAmbientLight.f, 4 );
	});

	//Get the Root of the Terrain's Octree
	if( Octree_ptr pOctree = pTerrain->GetRootOctree() )
	{
		//There is no Octree on this Terrain?
		if( !pOctree )
			return;

		//Draw Root Octree
		DrawOctree( pOctree );

		//Do actual Rendering of Octree
		FlushOctreeDrawJobs();

		//Render Octree Debug Volumes
		DrawOctreeDebugVolumes( pOctree );
	}
}

void Renderer::DrawOctree( Octree_ptr pOctree )
{
	//Octree has a Mesh?
	if( auto pMesh = pOctree->pMesh )
	{
		//Mesh AABB not visible in Frustum?
		if( !IsVisibleInFrustum( cViewFrustum, pOctree->cMeshBoundingAABB ) )
			return;

		//Compute Transformed World Center of Mesh
		auto vCenter = sWorldMatrixStack.top() * pMesh->GetBoundingSphere().center;

		//Get Squared Distance to Center
		float fSquaredDistance = (vCenter - pActiveCamera->GetEye()).lengthSquared();

		SkinMesh( nullptr, pMesh );

		//Queue Mesh
		PushOctreeDrawJob( pMesh, fSquaredDistance );
	}
	else
	{
		//Draw Children of Octree
		for( int i = 0; i < _countof( pOctree->paChildren ); i++ )
		{
			//Draw Octree
			if( pOctree->paChildren[i] )
				DrawOctree( pOctree->paChildren[i] );
		}
	}
}

void Renderer::PushOctreeDrawJob( Mesh_ptr pMesh, float fDistance )
{
	//Add OctreeDraw Job to List
	OctreeDrawJob sOctreeDrawJob;
	sOctreeDrawJob.pMesh		= pMesh;
	sOctreeDrawJob.fDistance	= fDistance;
	lOctreeDrawJobs.push_back( sOctreeDrawJob );
}

void Renderer::FlushOctreeDrawJobs()
{
	//Sort Octree Draw Job List for Opaque Mesh Parts
	//lOctreeDrawJobs.sort( Compare_OctreeDrawJob_DistanceOrderFrontToBack );

	//Render ordered Meshes
	for( auto & sOctreeDrawJob : lOctreeDrawJobs )
	{
		auto pMesh = sOctreeDrawJob.pMesh;

		//Render Mesh
		DrawMesh( pMesh, true, false, false );
	}

	//Sort Octree Draw Job List for Transparent Mesh Parts
	//lOctreeDrawJobs.sort( Compare_OctreeDrawJob_DistanceOrderBackToFront );
	
	//Render ordered Meshes
	for( auto & sOctreeDrawJob : lOctreeDrawJobs )
	{
		auto pMesh = sOctreeDrawJob.pMesh;

		//Render Mesh
		DrawMesh( pMesh, false, true, false );
	}
	
	//Clear Octree Draw Job List
	lOctreeDrawJobs.clear();
}

void Renderer::DrawMesh( Mesh_ptr pMesh, bool bOpaqueParts, bool bTransparentParts, bool bDebugBoundingVolumes )
{
	int iStreamNumber = 0;

	//Vertex Buffer
	if( auto pVertexBuffer = bHardwareSkinning ? pMesh->GetVertexBuffer() : pMesh->GetDynamicVertexBuffer() )
	{
		GetGraphicsDevice()->Get()->SetStreamSource( iStreamNumber, pVertexBuffer->Get(), 0, pVertexBuffer->GetElementSize() );

		iStreamNumber++;
	}

	//Normal Buffer
	if( auto pNormalBuffer = bHardwareSkinning ? pMesh->GetNormalBuffer() : pMesh->GetDynamicNormalBuffer() )
	{
		GetGraphicsDevice()->Get()->SetStreamSource( iStreamNumber, pNormalBuffer->Get(), 0, pNormalBuffer->GetElementSize() );

		iStreamNumber++;
	}

	//Color Buffer
	if( auto pColorBuffer = pMesh->GetColorBuffer() )
	{
		GetGraphicsDevice()->Get()->SetStreamSource( iStreamNumber, pColorBuffer->Get(), 0, pColorBuffer->GetElementSize() );

		iStreamNumber++;
	}

	//Skin Indices Buffer
	if( auto pSkinIndices = pMesh->GetSkinIndicesBuffer() )
	{
		GetGraphicsDevice()->Get()->SetStreamSource( iStreamNumber, pSkinIndices->Get(), 0, pSkinIndices->GetElementSize() );

		iStreamNumber++;
	}

	//Draw Opaque Mesh Parts in Model
	if( bOpaqueParts )
	{
		for( auto pMeshPart : pMesh->GetOpaqueParts() )
		{
			//Is Invisible Material?
			if( pMeshPart->GetMaterial()->GetInvisible() )
				continue;

			//Apply Material
			ApplyMaterial( pMesh, pMeshPart, iStreamNumber );

			//Draw Mesh Part
			DrawMeshPart( pMeshPart, pMesh->GetVertexBuffer()->GetElementCount() );
		}
	}

	//Draw Transparent Mesh Parts in Model
	if( bTransparentParts )
	{
		for( auto pMeshPart : pMesh->GetTransparentParts() )
		{
			//Is Invisible Material?
			if( pMeshPart->GetMaterial()->GetInvisible() )
				continue;

			//Apply Material
			ApplyMaterial( pMesh, pMeshPart, iStreamNumber );

			//Draw Mesh Part
			DrawMeshPart( pMeshPart, pMesh->GetVertexBuffer()->GetElementCount() );
		}
	}
	
	//Draw Debug Bounding Volumes
	if( bDebugBoundingVolumes )
	{
		//Draw Bounding AABB?
		if( bShowMeshBoundingAABB )
			DrawDebugAABB( pMesh->GetBoundingAABB() );

		//Draw Bounding Sphere?
		if( bShowMeshBoundingSphere )
			DrawDebugSphere( pMesh->GetBoundingSphere() );
	}

	//Draw Normals?
	if( bShowNormals )
		DrawMeshDebugNormals( pMesh );
}

void Renderer::DrawMeshPart( MeshPart_ptr pMeshPart, UINT uNumVertices )
{
	auto pMaterial = pMeshPart->GetMaterial();

	if( !pMaterial )
		return;

	auto pEffect = pMaterial->GetEffect();

	if( !pEffect )
		return;

	if( !pEffect->SetTechnique( pMaterial->GetEffectTechnique() ) )
		return;

	if( pEffect->Begin() >= 0 )
	{
		for( UINT u = 0; u < pMaterial->GetEffectNumPasses(); u++ )
		{
			if( pEffect->BeginPass( u ) )
			{
				GetGraphicsDevice()->Get()->SetVertexDeclaration( pMeshPart->GetVertexDeclaration()->Get() );
				GetGraphicsDevice()->Get()->SetIndices( pMeshPart->GetIndexBuffer()->Get() );
				GetGraphicsDevice()->Get()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, uNumVertices, 0, pMeshPart->GetIndexBuffer()->GetPrimitiveCount() );

				pEffect->EndPass();
			}
		}

		pEffect->End();
	}
}

BOOL Renderer::IsVisibleInFrustum( const X3D::Frustum & rFrustum, const X3D::Sphere & rSphere ) const
{
	auto bVisible = rFrustum.contains( rSphere );

	//Debug?

	return bVisible;
}

BOOL Renderer::IsVisibleInFrustum( const X3D::Frustum & rFrustum, const X3D::AABB & rAABB ) const
{
	auto bVisible = rFrustum.contains( rAABB );

	//Debug?

	return bVisible;
}

void Renderer::DrawDebugFrustum( const X3D::Frustum & rFrustum )
{
	if( !pDebugEffect->SetTechnique( "Debug" ) )
		return;

	if( pDebugEffect->Begin() >= 0 )
	{
		pDebugEffect->SetMatrix( "WorldViewProjection", mWorldViewProjectionMatrix );
		pDebugEffect->SetMatrix( "ViewProjection", mViewProjectionMatrix );
		pDebugEffect->SetMatrix( "World", *lpWorldMatrixStack->GetTop() );
		pDebugEffect->SetFloat( "Alpha", 1.0f );


		if( pDebugEffect->BeginPass( 0 ) )
		{
			//Near Plane
			{
				X3D::Quad q;
				q.p1 = rFrustum.ntl;
				q.p2 = rFrustum.nbl;
				q.p3 = rFrustum.nbr;
				q.p4 = rFrustum.ntr;
				DrawDebugQuad( q );
			}
	
			//Far Plane
			{
				X3D::Quad q;
				q.p1 = rFrustum.ftl;
				q.p2 = rFrustum.fbl;
				q.p3 = rFrustum.fbr;
				q.p4 = rFrustum.ftr;
				DrawDebugQuad( q );
			}

			//Attach Corners
			DrawDebugLine( rFrustum.ntl, rFrustum.ftl );
			DrawDebugLine( rFrustum.nbl, rFrustum.fbl );
			DrawDebugLine( rFrustum.nbr, rFrustum.fbr );
			DrawDebugLine( rFrustum.ntr, rFrustum.ftr );
			
			pDebugEffect->EndPass();
		}

		pDebugEffect->End();
	}


}

void Renderer::DrawMeshDebugNormals( const Mesh_ptr pMesh )
{
	auto vVertices = pMesh->GetVertices();
	auto vNormals = pMesh->GetNormals();
	auto vSkinIndices = pMesh->GetVertices();

	if( vVertices.size() != vNormals.size() )
		return;

	if( vVertices.size() != vSkinIndices.size() )
		return;

	pDebugEffect->SetTechnique( "Debug" );
	pDebugEffect->Begin();
	{
		pDebugEffect->SetMatrix( "WorldViewProjection", mWorldViewProjectionMatrix );
		pDebugEffect->SetMatrix( "ViewProjection", mViewProjectionMatrix );
		pDebugEffect->SetMatrix( "World", *lpWorldMatrixStack->GetTop() );
		pDebugEffect->SetFloat( "Alpha", 1.0f );


		pDebugEffect->BeginPass( 0 );
		{
			for( size_t i = 0; i < vVertices.size(); i++ )
			{
                auto mMatrix = pMesh->GetUseLocalMatrix() ? pMesh->GetLocalMatrix() : pMesh->GetFrameMatrix();

				const auto & v = mMatrix * vVertices[i];
				const auto & n = (mMatrix * vNormals[i]) * 25.0f;

				DrawDebugLine( v, v + n );
			}
		}
		pDebugEffect->EndPass();

	}
	pDebugEffect->End();
}

void Renderer::DrawOctreeDebugVolumes( const Octree_ptr pOctree )
{
	//End of Octree?
	if( pOctree->pMesh )
	{
		//Draw Bounding AABB?
		if( bShowOctreeBoundingAABB )
			DrawDebugAABB( pOctree->cBoundingAABB );

		//Draw Bounding Sphere?
		if( bShowOctreeBoundingSphere )
			DrawDebugSphere( pOctree->cBoundingSphere );
	}
	else
	{
		//Draw Children of Octree
		for( int i = 0; i < _countof( pOctree->paChildren ); i++ )
		{
			//Draw Octree
			if( pOctree->paChildren[i] )
				DrawOctreeDebugVolumes( pOctree->paChildren[i] );
		}
	}
}

void Renderer::DrawDebugAABB( const X3D::AABB & cAABB )
{
	if( !pDebugEffect->SetTechnique( "Debug" ) )
		return;

	if( pDebugEffect->Begin() >= 0 )
	{
		pDebugEffect->SetMatrix( "WorldViewProjection", mWorldViewProjectionMatrix );
		pDebugEffect->SetMatrix( "ViewProjection", mViewProjectionMatrix );
		pDebugEffect->SetMatrix( "World", *lpWorldMatrixStack->GetTop() );
		pDebugEffect->SetFloat( "Alpha", 1.0f );

		if( pDebugEffect->BeginPass( 0 ) )
		{
			Vector3 corners[8];
			cAABB.computeCorners( corners );

			DrawDebugQuad( Quad( corners[0], corners[4], corners[7], corners[3] ) ); //Left
			DrawDebugQuad( Quad( corners[5], corners[1], corners[2], corners[6] ) ); //Right
			DrawDebugQuad( Quad( corners[7], corners[6], corners[2], corners[3] ) ); //Bottom
			DrawDebugQuad( Quad( corners[0], corners[1], corners[5], corners[4] ) ); //Top
			DrawDebugQuad( Quad( corners[4], corners[5], corners[6], corners[7] ) ); //Front
			DrawDebugQuad( Quad( corners[1], corners[0], corners[3], corners[2] ) ); //Back

			pDebugEffect->EndPass();
		}

		pDebugEffect->End();
	}
}

void Renderer::DrawDebugSphere( const X3D::Sphere & cSphere )
{
	X3D::Matrix4 scaling, translation;
	scaling.scale( X3D::Vector3( cSphere.radius, cSphere.radius, cSphere.radius ) );
	translation.translate( X3D::Vector3( cSphere.center.x, cSphere.center.y, cSphere.center.z ) );

	PushWorldMatrix( scaling * translation );
	{
		pDebugEffect->SetTechnique( "Debug" );
		pDebugEffect->Begin();
		{
			pDebugEffect->SetMatrix( "WorldViewProjection", mWorldViewProjectionMatrix );
			pDebugEffect->SetMatrix( "ViewProjection", mViewProjectionMatrix );
			pDebugEffect->SetMatrix( "World", *lpWorldMatrixStack->GetTop() );
			pDebugEffect->SetFloat( "Alpha", 0.25f );

			pDebugEffect->BeginPass( 0 );
			{
				lpDebugSphereMesh->DrawSubset( 0 );
			}
			pDebugEffect->EndPass();
		}
		pDebugEffect->End();
	}
	PopWorldMatrix();
}

void Renderer::DrawDebugQuad( const X3D::Quad & cQuad )
{
	DrawDebugLine( cQuad.p1, cQuad.p2 );
	DrawDebugLine( cQuad.p2, cQuad.p3 );
	DrawDebugLine( cQuad.p3, cQuad.p4 );
	DrawDebugLine( cQuad.p4, cQuad.p1 );
}

void Renderer::DrawDebugLine( const X3D::Vector3 & cPoint1, const X3D::Vector3 & cPoint2 )
{
	struct Vertex
	{
		float		  fX, fY, fZ;

		Vertex() {}
		Vertex( float fX, float fY, float fZ ) : fX(fX), fY(fY), fZ(fZ) {}
	};

	Vertex aVertex[2];
	aVertex[0] = Vertex( cPoint1.x, cPoint1.y, cPoint1.z );
	aVertex[1] = Vertex( cPoint2.x, cPoint2.y, cPoint2.z );

	GetGraphicsDevice()->Get()->SetFVF( D3DFVF_XYZ );
	GetGraphicsDevice()->Get()->DrawPrimitiveUP( D3DPT_LINESTRIP, 1, aVertex, sizeof( Vertex ) );
}

void Renderer::ApplyMaterial( Mesh_ptr pMesh, MeshPart_ptr pMeshPart, int iStreamNumber )
{
	auto pMaterial = pMeshPart->GetMaterial();

	if ( !pMaterial )
		return;

	auto pEffect = pMaterial->GetEffect();

	if ( !pEffect )
		return;

	//Textures and Texture Coordinates
	{
		size_t i = 0;
		size_t s = pMaterial->GetTextures().size();
		for ( i = 0; i < s; i++ )
		{
			if ( i == 1 )
				break;

			LPDIRECT3DTEXTURE9 lpTexture = pMaterial->GetTextures()[i]->Get();

			GetGraphicsDevice()->Get()->SetTexture( i, lpTexture );
			GetGraphicsDevice()->Get()->SetStreamSource( i + iStreamNumber, pMesh->GetTextureCoordBuffers()[i]->Get(), 0, pMesh->GetTextureCoordBuffers()[i]->GetElementSize() );

			char szTexNSet[32];
			STRINGFORMAT( szTexNSet, "Tex%dSet", i + 1 );

			pEffect->SetBool( szTexNSet, lpTexture ? TRUE : FALSE );
		}
		GetGraphicsDevice()->Get()->SetTexture( i, NULL );
		GetGraphicsDevice()->Get()->SetStreamSource( i + iStreamNumber, NULL, 0, 0 );

		for ( i = s; i < 8; i++ )
		{
			char szTexNSet[32];
			STRINGFORMAT( szTexNSet, "Tex%dSet", i + 1 );

			pEffect->SetBool( szTexNSet, FALSE );
		}
	}

	//Alpha Blending
	switch ( bOverrideMaterialAlphaBlend ? eOverrideMaterialAlphaBlend : pMaterial->GetAlphaBlend() )
	{
		case Material::ALPHABLEND_None:
			GetGraphicsDevice()->Get()->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
			GetGraphicsDevice()->Get()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			GetGraphicsDevice()->Get()->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
			break;

		case Material::ALPHABLEND_Alpha:
			GetGraphicsDevice()->Get()->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			GetGraphicsDevice()->Get()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
			GetGraphicsDevice()->Get()->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
			break;

		case Material::ALPHABLEND_Additive:
			GetGraphicsDevice()->Get()->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			GetGraphicsDevice()->Get()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			GetGraphicsDevice()->Get()->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
			break;
	}

	//Commit Local Matrix?
	if ( pMesh->GetUseLocalMatrix() )
		pEffect->SetMatrix( "Local", X3DMatrixToD3DXMatrix( pMesh->GetLocalMatrix() ) );

	//Two Sided?
	if ( pMaterial->GetTwoSided() )
		GetGraphicsDevice()->Get()->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	else
		GetGraphicsDevice()->Get()->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );

	//Diffuse Color
	pEffect->SetFloat( "DiffuseRed", pMaterial->GetDiffuseRed() );
	pEffect->SetFloat( "DiffuseGreen", pMaterial->GetDiffuseGreen() );
	pEffect->SetFloat( "DiffuseBlue", pMaterial->GetDiffuseBlue() );

	//Transparency
	pEffect->SetFloat( "Transparency", pMaterial->GetTransparency() );

	//Self Illumination
	pEffect->SetFloat( "SelfIllumination", pMaterial->GetSelfIllumination() );

	//Texture Coordination Transformation Type
	{
		const auto & vTextureTypes = pMaterial->GetTextureTypes();

		for ( size_t i = 0; i < vTextureTypes.size(); i++ )
		{
			char szHandle[32];
			STRINGFORMAT( szHandle, "TexCoordTransformationType%d", i + 1 );

			pEffect->SetInt( szHandle, vTextureTypes[i] );
		}
	}
}

void Renderer::SkinMesh( Model_ptr pModel, Mesh_ptr pMesh )
{
	if ( bHardwareSkinning )
		return;

	if ( pModel && pModel->GetBoneModel() )
	{
		if ( pModel->GetBoneMatrices().size() == 0 )
			return;

		if ( pMesh->GetSkinIndices().size() == 0 )
			return;

		if ( pMesh->GetSkinIndices().size() != pMesh->GetVertices().size() )
			return;

		X3D::Vector3 * psaDynamicVertex = (X3D::Vector3 *)pMesh->GetDynamicVertexBuffer()->Lock();
		X3D::Vector3 * psaDynamicNormal = (X3D::Vector3 *)pMesh->GetDynamicNormalBuffer()->Lock();
		for ( size_t i = 0; i < pMesh->GetSkinIndices().size(); i++ )
		{
			int iIndex = (int)pMesh->GetSkinIndices()[i];
			if ( (iIndex < 0) || (iIndex >= (int)pModel->GetBoneMatrices().size()) )
				continue;

			auto mMatrix = pModel->GetBoneMatrices()[iIndex];
			auto sVertex = pMesh->GetVertices()[i];
			auto sNormal = pMesh->GetNormals()[i];

			psaDynamicVertex[i] = mMatrix * sVertex;
			psaDynamicNormal[i] = mMatrix * sNormal;
		}
		pMesh->GetDynamicNormalBuffer()->Unlock();
		pMesh->GetDynamicVertexBuffer()->Unlock();
	}
	else
	{
		X3D::Vector3 * psaDynamicVertex = (X3D::Vector3 *)pMesh->GetDynamicVertexBuffer()->Lock();		
		X3D::Vector3 * psaDynamicNormal = (X3D::Vector3 *)pMesh->GetDynamicNormalBuffer()->Lock();
		for ( size_t i = 0; i < pMesh->GetVertices().size(); i++ )
		{
			auto mMatrix = pMesh->GetUseLocalMatrix() ? pMesh->GetLocalMatrix() : pMesh->GetFrameMatrix();
			auto sVertex = pMesh->GetVertices()[i];
			auto sNormal = pMesh->GetNormals()[i];

			psaDynamicVertex[i] = mMatrix * sVertex;
			psaDynamicNormal[i] = mMatrix * sNormal;
		}
		pMesh->GetDynamicNormalBuffer()->Unlock();
		pMesh->GetDynamicVertexBuffer()->Unlock();
	}
}

bool Compare_OctreeDrawJob_DistanceOrderBackToFront( const OctreeDrawJob & r1, const OctreeDrawJob & r2 )
{
	if( r1.fDistance >= r2.fDistance )
		return true;

	return false;
}

bool Compare_OctreeDrawJob_DistanceOrderFrontToBack( const OctreeDrawJob & r1, const OctreeDrawJob & r2 )
{
	if( r1.fDistance >= r2.fDistance )
		return false;

	return true;
}

}