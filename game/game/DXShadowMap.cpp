#include "stdafx.h"
#include "DXShadowMap.h"


namespace DX
{

void ShadowMap::Init( Renderer_ptr _pRenderer )
{
	pRenderer = _pRenderer;

	pEffect = pRenderer->GetGraphicsDevice()->GetEffectFactory()->MakeEffect( "game\\scripts\\shaders\\Shadow.fx" );

	pDepthStencilBuffer = pRenderer->GetGraphicsDevice()->MakeDepthStencilBuffer( 500, 0 );
	pRenderTarget1 = pRenderer->GetGraphicsDevice()->MakeRenderTarget( 11021, 0 );
	pRenderTarget2 = pRenderer->GetGraphicsDevice()->MakeRenderTarget( 11022, 0 );

	pTexture = pRenderer->GetGraphicsDevice()->GetTextureFactory()->MakeBlankTexture();

	pSprite = pRenderer->GetGraphicsDevice()->GetSpriteFactory()->MakeSprite( TRUE );

	bGotSomething = FALSE;

	bFirstTime = TRUE;
}

void ShadowMap::Shutdown()
{
	pSprite = nullptr;

	pTexture = nullptr;

	pRenderTarget2 = nullptr;
	pRenderTarget1 = nullptr;
	pDepthStencilBuffer = nullptr;

	pEffect = nullptr;
}

BOOL ShadowMap::Begin( BOOL bSmallPass )
{
	//Select Glow disabled?
	if ( bDisabled )
		return FALSE;

	//Is everything there?
	if ( !pEffect || !pDepthStencilBuffer || !pRenderTarget1 || !pRenderTarget2 || !pTexture || !pSprite )
		return FALSE;

	if ( bSmallPass )
	{
		vOldViewport = pRenderer->GetGraphicsDevice()->GetViewport();
		if ( !pRenderer->GetGraphicsDevice()->SetViewport( Viewport( 0, 0, pRenderTarget1->GetWidth() - 1, pRenderTarget1->GetHeight() - 1, 0.0f, 1.0f ) ) )
			return FALSE;

		if ( !pRenderer->GetGraphicsDevice()->SetRenderTarget( pRenderTarget1 ) )
		{
			pRenderer->GetGraphicsDevice()->SetViewport( vOldViewport );

			return FALSE;
		}

		if ( !pRenderer->GetGraphicsDevice()->SetDepthStencilBuffer( pDepthStencilBuffer ) )
		{
			pRenderer->GetGraphicsDevice()->UnsetRenderTarget();
			pRenderer->GetGraphicsDevice()->SetViewport( vOldViewport );

			return FALSE;
		}
		
		pRenderer->GetGraphicsDevice()->Clear( TRUE, TRUE, FALSE );

		pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_LIGHTING, FALSE );
		pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA );

		D3DXMATRIX sWorld;
		pRenderer->GetGraphicsDevice()->Get()->GetTransform( D3DTS_WORLD, &sWorld );

		D3DXMATRIX lightWVP = sWorld * m_LightViewMatrix * m_LightProjectionMatrix;
		pEffect->SetMatrix( "LightWorldViewProjection", lightWVP );

		pEffect->SetTechnique( "RenderShadowMap" );
		pEffect->Begin();
		pEffect->BeginPass(0);
	}
	else
	{
		pRenderer->GetGraphicsDevice()->Clear( TRUE, TRUE, FALSE );

		D3DXMATRIX sWorld;
		pRenderer->GetGraphicsDevice()->Get()->GetTransform( D3DTS_WORLD, &sWorld );

		D3DXMATRIX cameraWVP = sWorld * m_CameraViewMatrix * m_CameraProjectionMatrix;
		pEffect->SetMatrix( "CameraWorldViewProjection", cameraWVP );

		pEffect->SetTechnique( "RenderScene" );
		pEffect->Get()->SetTexture( "shadowMapTexture", pRenderTarget1->GetTexture() );
		pEffect->Begin();
		pEffect->BeginPass( 0 );

	}

	return TRUE;
}

void ShadowMap::End( BOOL bSmallPass )
{
	if ( bSmallPass )
	{
		pEffect->EndPass();
		pEffect->End();

		pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_RED );
		pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_LIGHTING, TRUE );


		pRenderer->GetGraphicsDevice()->UnsetDepthStencilBuffer();
		GRAPHICENGINE->SaveRenderTargetToFile( "d:\\prog.png" );
		pRenderer->GetGraphicsDevice()->UnsetRenderTarget();
		pRenderer->GetGraphicsDevice()->SetViewport( vOldViewport );


		D3DXMATRIX textureMatrix;
		D3DXMATRIX cameraInverse;

		float fOffsetX = 0.5f + (0.5f / 512);
		float fOffsetY = 0.5f + (0.5f / 512);

		//compute bias matrix
		D3DXMATRIX biasMatrix( 0.5f, 0.0f, 0.0f, 0.0f,
			0.0f, -0.5f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			fOffsetX, fOffsetY, 0.0f, 1.0f );

		D3DXMATRIX sWorld;
		pRenderer->GetGraphicsDevice()->Get()->GetTransform( D3DTS_WORLD, &sWorld );


		//concatenate matrices to compute texture matrix
		textureMatrix = sWorld * m_LightViewMatrix * m_LightProjectionMatrix * biasMatrix;

		//set matrix in effect shaders
		pEffect->SetMatrix( "matTexture", textureMatrix );
	}
	else
	{
		pEffect->EndPass();
		pEffect->End();

		GRAPHICENGINE->SaveRenderTargetToFile( "d:\\prog2.png" );
	}

	bGotSomething = TRUE;
}

void ShadowMap::Draw()
{
	if ( !bGotSomething )
		return;

	bGotSomething = FALSE;

	//Is everything there?
	if ( !pEffect || !pDepthStencilBuffer || !pRenderTarget1 || !pRenderTarget2 || !pTexture || !pSprite )
		return;

	BOOL bRenderOK = FALSE;

	//Resize Viewport to our Render Target
	vOldViewport = pRenderer->GetGraphicsDevice()->GetViewport();
	if ( pRenderer->GetGraphicsDevice()->SetViewport( Viewport( 0, 0, pRenderTarget1->GetWidth(), pRenderTarget1->GetHeight(), 0.0f, 1.0f ) ) )
	{
		//Set small Depth Stencil Buffer
		if ( pRenderer->GetGraphicsDevice()->SetDepthStencilBuffer( pDepthStencilBuffer ) )
		{
			//Compute Texel Size
			float faTexelSize[2] = { 0.0f, 0.0f };

			if ( pRenderTarget1 )
			{
				faTexelSize[0] = 1.0f / (float)pRenderTarget1->GetWidth();
				faTexelSize[1] = 1.0f / (float)pRenderTarget1->GetHeight();
			}

			//Draw Blank Texture to Render Target 1 and only color the parts marked in the low resolution Stencil Buffer
			if ( pRenderer->GetGraphicsDevice()->SetRenderTarget( pRenderTarget1 ) )
			{
				//Clear Render Target
				pRenderer->GetGraphicsDevice()->Clear( TRUE, FALSE, FALSE );

				if ( pSprite->Begin( TRUE ) )
				{
					pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_STENCILENABLE, TRUE );
					pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP );
					pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP );
					pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_STENCILPASS, D3DSTENCILOP_KEEP );
					pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_STENCILFUNC, D3DCMP_LESS );
					pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_STENCILREF, 0 );
					pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_STENCILMASK, 0xFFFFFFFF );
					pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_STENCILWRITEMASK, 0xFFFFFFFF );

					pSprite->Draw( pTexture, cColor, pRenderTarget1->GetWidth(), pRenderTarget1->GetHeight() );

					pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_STENCILENABLE, FALSE );

					pSprite->End();
				}

				//Clear low resolution Stencil Buffer
				pRenderer->GetGraphicsDevice()->Clear( FALSE, FALSE, TRUE );

				pRenderer->GetGraphicsDevice()->UnsetRenderTarget();
			}

			//Effect
			{
				//Glow Technique
				pEffect->SetTechnique( "Glow" );

				//Begin Effect
				if ( pEffect->Begin() >= 0 )
				{
					//Set Effect Parameters
					pEffect->SetFloatArray( "texelSize", faTexelSize, _countof( faTexelSize ) );
					pEffect->SetFloatArray( "offsets", faOffset, _countof( faOffset ) );
					pEffect->SetFloatArray( "weights", faWeight, _countof( faWeight ) );

					//Blur Horizontally the first Render Target (silhouette becomes horizontally blurred)
					if ( pRenderer->GetGraphicsDevice()->SetRenderTarget( pRenderTarget2 ) )
					{
						//Clear Render Target
						pRenderer->GetGraphicsDevice()->Clear( TRUE, FALSE, FALSE );

						if ( pSprite->Begin( TRUE ) )
						{
							if ( pEffect->BeginPass( 0 ) )
							{
								pSprite->Draw( pRenderTarget1 );

								pEffect->EndPass();
							}

							pSprite->End();
						}

						pRenderer->GetGraphicsDevice()->UnsetRenderTarget();
					}

					//Blur Vertically the second Render Target (horizontally blurred silhouette becomes fully blurred)
					if ( pRenderer->GetGraphicsDevice()->SetRenderTarget( pRenderTarget1 ) )
					{
						//Clear Render Target
						pRenderer->GetGraphicsDevice()->Clear( TRUE, FALSE, FALSE );

						if ( pSprite->Begin( TRUE ) )
						{
							if ( pEffect->BeginPass( 1 ) )
							{
								pSprite->Draw( pRenderTarget2 );

								pEffect->EndPass();

								//Mark Done
								bRenderOK = TRUE;
							}

							pSprite->End();
						}

						pRenderer->GetGraphicsDevice()->UnsetRenderTarget();
					}

					pEffect->End();
				}
			}

			//Unset small Depth Stencil Buffer
			pRenderer->GetGraphicsDevice()->UnsetDepthStencilBuffer();
		}

		//Use normal Viewport
		pRenderer->GetGraphicsDevice()->SetViewport( vOldViewport );
	}

	//Render OK?
	if ( bRenderOK )
	{
		//Stretch Render the blurred Render Target to the Back Buffer, mask out the silhouette that resides in the high resolution Stencil Buffer of the target
		if ( pSprite->Begin( TRUE ) )
		{
			pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

			pRenderer->GetGraphicsDevice()->Get()->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
			pRenderer->GetGraphicsDevice()->Get()->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

			pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_STENCILENABLE, TRUE );
			pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP );
			pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP );
			pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_STENCILPASS, D3DSTENCILOP_KEEP );
			pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_STENCILFUNC, D3DCMP_GREATEREQUAL );
			pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_STENCILREF, 0 );
			pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_STENCILMASK, 0xFFFFFFFF );
			pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_STENCILWRITEMASK, 0xFFFFFFFF );

			pSprite->Draw( pRenderTarget1 );

			pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_STENCILENABLE, FALSE );

			pSprite->End();
		}
	}

	bFirstTime = TRUE;
}

void ShadowMap::SetupFrame( D3DXVECTOR3 sEye, D3DXVECTOR3 sLookAt, D3DXVECTOR3 sUp )
{
	D3DXVECTOR3 sLight = sEye;
	sLight.z += (100.0f * 256.0f);

	//set camera matrices
	D3DXMatrixPerspectiveFovLH( &m_CameraProjectionMatrix, D3DXToRadian( 45.0f ), (float)RESOLUTION_WIDTH / (float)RESOLUTION_HEIGHT, 1.0f, 4000.0f );
	D3DXMatrixLookAtLH( &m_CameraViewMatrix,
		&sEye, //Eye-vector 
		&sLookAt,		//At-vector
		&D3DXVECTOR3( 0.0, 1.0, 0.0 ) );	//Up-vector

	//set light matrices
	D3DXMatrixPerspectiveFovLH( &m_LightProjectionMatrix, D3DXToRadian( 45.0f ), 1.0, 1.0f, 100.0f );
	D3DXMatrixLookAtLH( &m_LightViewMatrix,
		&sLight,	//Eye-vector 
		&D3DXVECTOR3( 0.0, 0.0, 0.0 ),		//At-vector
		&D3DXVECTOR3( 0.0, 1.0, 0.0 ) );	//Up-vector

	//setup our D3D Device initial states
//	pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );

	//set effect's light & camera positions for lighting calculations
	pEffect->Get()->SetVector( "lightPosition", (D3DXVECTOR4 *)&sLight );
	pEffect->Get()->SetVector( "cameraPosition", (D3DXVECTOR4 *)&sEye );
}

void ShadowMap::UpdateShadowMapping()
{
	if ( RENDERER->bShadowMapping )
	{
		LPDIRECT3DBASETEXTURE9 lpTexture;
		pRenderer->GetGraphicsDevice()->Get()->GetTexture( 0, &lpTexture );
		pEffect->Get()->SetTexture( "sceneTexture", lpTexture );
		pEffect->CommitChanges();
	}
}

ShadowMap cShadowMap1;

};