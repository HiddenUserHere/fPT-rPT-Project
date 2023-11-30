#include "stdafx.h"
#include "DXSelectGlow.h"


namespace DX
{

void SelectGlow::Init( Renderer_ptr _pRenderer )
{
	static UINT uWheelID = 0;

	pRenderer = _pRenderer;

	pEffect = pRenderer->GetGraphicsDevice()->GetEffectFactory()->MakeEffect( "game\\scripts\\shaders\\SelectGlow.fx" );

	pDepthStencilBuffer = pRenderer->GetGraphicsDevice()->MakeDepthStencilBuffer( 11001 + uWheelID, 0 );
	pRenderTarget1 = pRenderer->GetGraphicsDevice()->MakeRenderTarget( 11001 + uWheelID );
	pRenderTarget2 = pRenderer->GetGraphicsDevice()->MakeRenderTarget( 11022 + uWheelID );
	uWheelID++;

	pTexture = pRenderer->GetGraphicsDevice()->GetTextureFactory()->MakeBlankTexture();

	pSprite = pRenderer->GetGraphicsDevice()->GetSpriteFactory()->MakeSprite( TRUE );

	GaussianFunc();

	bGotSomething = FALSE;

	bFirstTime = TRUE;
}

void SelectGlow::Shutdown()
{
	pSprite = nullptr;

	pTexture = nullptr;

	pRenderTarget2 = nullptr;
	pRenderTarget1 = nullptr;
	pDepthStencilBuffer = nullptr;

	pEffect = nullptr;
}

BOOL SelectGlow::Begin( D3DCOLOR d3dColor, BOOL bSmallPass )
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

		if ( bFirstTime )
		{
			pRenderer->GetGraphicsDevice()->Clear( FALSE, FALSE, TRUE );

			bFirstTime = FALSE;
		}
	}

	cColor = Color( d3dColor );
	//cColor.SetAlpha( 255 );

	pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_STENCILENABLE, TRUE );
	pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_STENCILFAIL, D3DSTENCILOP_INCRSAT );
	pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_STENCILZFAIL, D3DSTENCILOP_INCRSAT );
	pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_STENCILPASS, D3DSTENCILOP_INCRSAT );
	pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_STENCILFUNC, D3DCMP_ALWAYS );
	pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_STENCILWRITEMASK, 0xFFFFFFFF );

	pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_COLORWRITEENABLE, 0 );
	pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
	pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_ZENABLE, FALSE );

	return TRUE;
}

void SelectGlow::End( BOOL bSmallPass )
{
	pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_ZENABLE, TRUE );
	pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_RED );

	pRenderer->GetGraphicsDevice()->Get()->SetRenderState( D3DRS_STENCILENABLE, FALSE );

	if ( bSmallPass )
	{
		pRenderer->GetGraphicsDevice()->UnsetDepthStencilBuffer();
		pRenderer->GetGraphicsDevice()->UnsetRenderTarget();
		pRenderer->GetGraphicsDevice()->SetViewport( vOldViewport );
	}

	bGotSomething = TRUE;
}

void SelectGlow::Draw()
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

void SelectGlow::GaussianFunc()
{
	faWeight[0] = Gaussian( 0.0f );
	faOffset[0] = 0.0f;

	float totalWeight = faWeight[0];

	for ( int i = 0; i < _countof( faWeight ) / 2; i++ )
	{
		float weight = Gaussian( (float)(i + 1) );
		faWeight[(i * 2) + 1] = weight;
		faWeight[(i * 2) + 2] = weight;
		totalWeight += weight * 2.0f;

		float offset = ((float)i * 2.0f) + 1.5f;
		faOffset[(i * 2) + 1] = +offset;
		faOffset[(i * 2) + 2] = -offset;
	}

	for ( int i = 0; i < _countof( faWeight ); i++ )
		faWeight[i] /= totalWeight;
}

float SelectGlow::Gaussian( float x )
{
	float blur2 = fSizeGlow;
	float invSq = 1.0f / (float)sqrt( PI * blur2 );
	float exp2 = (float)exp( -(x * x) / blur2 );
	return invSq * exp2;
}

SelectGlow cSelectGlow1;
SelectGlow cSelectGlow2;
SelectGlow cSelectGlow3;
SelectGlow cSelectGlow4;

}