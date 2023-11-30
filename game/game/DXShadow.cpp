#include "stdafx.h"
#include "DXShadow.h"

namespace DX
{

Shadow::Shadow( std::weak_ptr<GraphicsDevice> pGraphicsDevice, int _iSharableID, UINT _uWidth, UINT _uHeight ) : pGraphicsDevice( pGraphicsDevice ), iSharableID( _iSharableID ), uWidth( _uWidth ), uHeight( _uHeight )
{
}


Shadow::~Shadow()
{
}

bool Shadow::Init()
{
	pRenderTargetShadow = pGraphicsDevice.lock()->MakeRenderTarget( 212 );
	pDepthStencilShadow = pGraphicsDevice.lock()->MakeDepthStencilBuffer( 212 );

	return true;
}

bool Shadow::Begin( Effect_ptr pEffectShadow, Point3D sCamera, Point3D sAngle, D3DXMATRIX & sLightViewMatrix, D3DXMATRIX & sLightProjectionMatrix )
{
	bool bRet = false;

	UpdateMatrix( pEffectShadow, sLightViewMatrix, sLightProjectionMatrix );

	//Set New Render Target
	if ( pGraphicsDevice.lock()->SetRenderTarget( pRenderTargetShadow ) )
	{
		if ( pGraphicsDevice.lock()->SetDepthStencilBuffer( pDepthStencilShadow ) )
		{
			//Clear scene
			pGraphicsDevice.lock()->Clear( TRUE, TRUE, FALSE );

			D3DXMATRIX matp;
			GRAPHICENGINE->GetDevice()->GetTransform( D3DTS_WORLD, &matp );

			//Update Light Matrix
			pEffectShadow->SetMatrix( "LightWorldViewProjection", (matp * sLightViewMatrix * sLightProjectionMatrix) );

			//Render Scene
			pEffectShadow->SetTechnique( "RenderShadowMap" );
			if ( pEffectShadow->Begin() > 0 )
			{
				pEffectShadow->BeginPass( 0 );
				bRet = true;
			}
		}
		else
		{
			pGraphicsDevice.lock()->UnsetDepthStencilBuffer();
			pGraphicsDevice.lock()->UnsetRenderTarget();
		}
	}
	else
		pGraphicsDevice.lock()->UnsetRenderTarget();

	return bRet;
}

void Shadow::End( Effect_ptr pEffectShadow )
{
	pEffectShadow->EndPass();
	pEffectShadow->End();

	GRAPHICENGINE->SaveRenderTargetToFile( "D:\\PROG.PNG" );

	pGraphicsDevice.lock()->UnsetDepthStencilBuffer();
	pGraphicsDevice.lock()->UnsetRenderTarget();
}

void Shadow::UpdateMatrix( Effect_ptr pEffectShadow, D3DXMATRIX & sLightViewMatrix, D3DXMATRIX & sLightProjectionMatrix )
{
	D3DXMATRIX textureMatrix;
	D3DXMATRIX cameraInverse;

	float fOffsetX = 0.5f + (0.5f / uWidth);
	float fOffsetY = 0.5f + (0.5f / uHeight);

	//compute bias matrix
	D3DXMATRIX biasMatrix( 0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		fOffsetX, fOffsetY, 0.0f, 1.0f );

	D3DXMATRIX matp;
	GRAPHICENGINE->GetDevice()->GetTransform( D3DTS_WORLD, &matp );

	//compute camera's view inverse
	D3DXMatrixInverse( &cameraInverse, NULL, &CAMERA->GetViewMatrix() );
	D3DXMatrixTranspose( &cameraInverse, &cameraInverse );

	//concatenate matrices to compute texture matrix
	textureMatrix = matp * sLightViewMatrix * sLightProjectionMatrix * biasMatrix;

	//set matrix in effect shaders
	pEffectShadow->SetMatrix( "matTexture", textureMatrix );
}


}