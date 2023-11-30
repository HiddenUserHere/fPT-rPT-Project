#include "stdafx.h"
#include "DXGlow.h"


DXGlow::DXGlow()
{
}


DXGlow::~DXGlow()
{
}

void DXGlow::CreateRenderBuffers()
{
	if ( !lpRenderTexture )
	{
		HRESULT hr;

		LPD3DXBUFFER lpD3DErrorBuffer = NULL;

		if ( FAILED( hr = D3DXCreateEffectFromFileA( DX::Graphic::GetDevice(), "game\\scripts\\shaders\\SelectGlow.fx", NULL, NULL, D3DXSHADER_DEBUG, NULL, &lpRenderEffect, &lpD3DErrorBuffer ) ) )
		{
			GRAPHICENGINE->Log( "Could not create Post Process Effect:\n%s", lpD3DErrorBuffer ? lpD3DErrorBuffer->GetBufferPointer() : "NULL" );

			if ( lpD3DErrorBuffer )
				RELEASE( lpD3DErrorBuffer );

			return;
		}


		//Render Target
		if ( FAILED( hr = DX::Graphic::GetDevice()->CreateTexture( GRAPHICS->presentParams.BackBufferWidth, GRAPHICS->presentParams.BackBufferHeight, 1, D3DUSAGE_RENDERTARGET, GRAPHICS->presentParams.BackBufferFormat, D3DPOOL_DEFAULT, &lpRenderTexture, NULL ) ) )
		{
			GRAPHICENGINE->Log( "Could not create Render Target" );
			return;
		}

		//Render Surface
		if ( FAILED( hr = lpRenderTexture->GetSurfaceLevel( 0, &lpRenderSurface ) ) )
		{
			GRAPHICENGINE->Log( "Could not get Render Target Surface" );
			return;
		}

		//Render Target Glow
		if ( FAILED( hr = DX::Graphic::GetDevice()->CreateTexture( GRAPHICS->presentParams.BackBufferWidth, GRAPHICS->presentParams.BackBufferHeight, 1, D3DUSAGE_RENDERTARGET, GRAPHICS->presentParams.BackBufferFormat, D3DPOOL_DEFAULT, &lpRenderTextureGlow, NULL ) ) )
		{
			GRAPHICENGINE->Log( "Could not create Render Target" );
			return;
		}

		//Render Surface Glow
		if ( FAILED( hr = lpRenderTextureGlow->GetSurfaceLevel( 0, &lpRenderSurfaceGlow ) ) )
		{
			GRAPHICENGINE->Log( "Could not get Render Target Surface" );
			return;
		}

		//Sprite
		if ( FAILED( hr = D3DXCreateSprite( DX::Graphic::GetDevice(), &lpRenderSprite ) ) )
		{
			GRAPHICENGINE->Log( "Could not create Sprite Render" );
			return;
		}
	}
}

void DXGlow::Init()
{
	CreateRenderBuffers();
}

BOOL DXGlow::BeginRender()
{
	if ( lpRenderSurface && bNewRenderTarget == FALSE )
	{

		//Save Back Buffers
		DX::Graphic::GetDevice()->GetRenderTarget( 0, &lpBackBuffer );

		//Set the proj textures render surface
		DX::Graphic::GetDevice()->SetRenderTarget( 0, lpRenderSurface );
		if ( !bInit )
			DX::Graphic::GetDevice()->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 0, 0, 0 ), 1.0f, 0 );
		else
		{
			DX::Graphic::GetDevice()->Clear( 0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 0, 0, 0 ), 1.0f, 0 );
		}
		bInit = TRUE;

		//Set
		bNewRenderTarget = TRUE;

		return TRUE;
	}

	return FALSE;
}

void DXGlow::EndRender()
{
	if ( lpRenderSurface && bNewRenderTarget )
	{
	
		//Back to Old Back Buffer
		DX::Graphic::GetDevice()->SetRenderTarget( 0, lpBackBuffer );

		bNewRenderTarget = FALSE;
	}
}

void DXGlow::Begin()
{
	SetTechnique();

	uPasses = 0;
	uPass = 0;
	lpRenderEffect->Begin( &uPasses, D3DXFX_DONOTSAVESAMPLERSTATE );
	
}

BOOL DXGlow::BeginPass()
{
	if ( uPass >= uPasses )
		return FALSE;

	lpRenderEffect->BeginPass( uPass );

	return TRUE;
}

void DXGlow::EndPass()
{
	lpRenderEffect->EndPass();
	
	uPass++;
}

void DXGlow::End()
{
	lpRenderEffect->End();
}

void DrawTexture2D( LPDIRECT3DTEXTURE9 pTextureContainer, D3DCOLOR d3dColor, Rectangle2DF & rDestinationRectangle, Rectangle2DF & rSourceRectangle, BOOL bBitmapTransparency, D3DCOLOR d3dColorSpecular )
{
	//Don't draw texture if it's size is 0x0
	if ( (rDestinationRectangle.fWidth == 0) && (rDestinationRectangle.fHeight == 0) )
		return;

	//If the destination size is -1 x -1 then it was requested to use the texture width and height
	if ( (rDestinationRectangle.fWidth == -1) && (rDestinationRectangle.fHeight == -1) )
	{
		if ( pTextureContainer == NULL )
			return;

		rDestinationRectangle.fWidth = float( 800 );
		rDestinationRectangle.fHeight = float( 60 );
	}

	float u, v, u2, v2;

	//Store texture sizes
	float w = float( 800 );
	float h = float( 800 );

	//If Width and Height of the source rectangle is zero, use texture sizes
	if ( rSourceRectangle.fWidth == 0 )
		rSourceRectangle.fWidth = w;
	if ( rSourceRectangle.fHeight == 0 )
		rSourceRectangle.fHeight = h;

	//Compute texture coordinates
	u = (float( rSourceRectangle.fX ) + 0.5f) / w;
	v = (float( rSourceRectangle.fY ) + 0.5f) / h;
	u2 = u + float( rSourceRectangle.fWidth ) / w;
	v2 = v + float( rSourceRectangle.fHeight ) / h;

	EXEDrawVertex rVertex[4];
	rVertex[0] = { float( rDestinationRectangle.fX ), float( rDestinationRectangle.fY ), 0, 1, d3dColor, d3dColorSpecular, u, v };
	rVertex[1] = { float( rDestinationRectangle.fX + rDestinationRectangle.fWidth ), float( rDestinationRectangle.fY ), 0, 1, d3dColor, d3dColorSpecular, u2, v };
	rVertex[2] = { float( rDestinationRectangle.fX ), float( rDestinationRectangle.fY + rDestinationRectangle.fHeight ), 0, 1, d3dColor, d3dColorSpecular, u, v2 };
	rVertex[3] = { float( rDestinationRectangle.fX + rDestinationRectangle.fWidth ), float( rDestinationRectangle.fY + rDestinationRectangle.fHeight ), 0, 1, d3dColor, d3dColorSpecular, u2, v2 };

	GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	DX::Graphic::GetDevice()->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );

	//Textures
	GRAPHICENGINE->GetDevice()->SetTexture( 0, NULL );
	GRAPHICENGINE->GetDevice()->SetTexture( 1, NULL );
	GRAPHICENGINE->GetDevice()->SetTexture( 2, NULL );
	GRAPHICENGINE->GetDevice()->SetTexture( 3, NULL );
	GRAPHICENGINE->GetDevice()->SetTexture( 4, NULL );
	GRAPHICENGINE->GetDevice()->SetTexture( 5, NULL );
	GRAPHICENGINE->GetDevice()->SetTexture( 6, NULL );
	GRAPHICENGINE->GetDevice()->SetTexture( 7, NULL );

	GRAPHICENGINE->GetDevice()->SetTexture( 0, pTextureContainer );

	
	GRAPHICENGINE->DrawPrimitive( D3DPT_TRIANGLESTRIP, 452, 4, rVertex );



}
void DXGlow::Render2D()
{

	Begin();

	Rectangle2DF s;
	s.fX = 0;
	s.fY = 0;
	s.fWidth = 0;
	s.fHeight = 0;

	Rectangle2DF d;
	d.fX = 0;
	d.fY = 0;
	d.fWidth = 800;
	d.fHeight = 600;

	GetTextures();

	for ( UINT u = 0; u < uPasses; u++ )
	{
		lpRenderEffect->BeginPass( u );
		DrawTexture2D( lpRenderTexture, -1, d, s, FALSE, 0 );
		lpRenderEffect->EndPass();
	}

	SetTextures();


	End();

	bInit = FALSE;
}

void DXGlow::Render2D2()
{

	Rectangle2DF s;
	s.fX = 0;
	s.fY = 0;
	s.fWidth = 0;
	s.fHeight = 0;

	Rectangle2DF d;
	d.fX = 0;
	d.fY = 0;
	d.fWidth = 800;
	d.fHeight = 600;

	GetTextures();

	DrawTexture2D( lpRenderTexture, -1, d, s, FALSE, 0 );
		
	SetTextures();
}

void DXGlow::RenderGlow()
{
	Begin();

	Rectangle2DF s;
	s.fX = 0;
	s.fY = 0;
	s.fWidth = 0;
	s.fHeight = 0;

	Rectangle2DF d;
	d.fX = 0;
	d.fY = 0;
	d.fWidth = 800;
	d.fHeight = 600;

	GetTextures();

	for ( UINT u = 0; u < uPasses; u++ )
	{
		lpRenderEffect->BeginPass( u );
		DrawTexture2D( lpRenderTextureGlow, -1, d, s, FALSE, 0 );
		lpRenderEffect->EndPass();
	}

	SetTextures();


	End();
}

BOOL DXGlow::SetTechnique()
{
	lpRenderEffect->SetTechnique( "Glow" );

	return TRUE;
}

void DXGlow::Test()
{
	DX::Graphic::GetDevice()->SetRenderState( D3DRS_STENCILENABLE, TRUE );

	DX::Graphic::GetDevice()->SetRenderState( D3DRS_STENCILFUNC, D3DCMP_GREATEREQUAL );

	DX::Graphic::GetDevice()->SetRenderState( D3DRS_STENCILREF, 0x1 );

	DX::Graphic::GetDevice()->SetRenderState( D3DRS_STENCILMASK, -1 );
	DX::Graphic::GetDevice()->SetRenderState( D3DRS_STENCILWRITEMASK, -1 );

	DX::Graphic::GetDevice()->SetRenderState( D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP );
	DX::Graphic::GetDevice()->SetRenderState( D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP );
	DX::Graphic::GetDevice()->SetRenderState( D3DRS_STENCILPASS, D3DSTENCILOP_KEEP );

	DX::Graphic::GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	DX::Graphic::GetDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

	DX::Graphic::GetDevice()->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
	DX::Graphic::GetDevice()->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_NOTEQUAL );
	DX::Graphic::GetDevice()->SetRenderState( D3DRS_ALPHAREF, 0x1 );


	DX::Graphic::GetDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );
	DX::Graphic::GetDevice()->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
}

void DXGlow::Test2()
{
	DX::Graphic::GetDevice()->SetRenderState( D3DRS_STENCILENABLE, TRUE );

	DX::Graphic::GetDevice()->SetRenderState( D3DRS_STENCILFUNC, D3DCMP_EQUAL );

	DX::Graphic::GetDevice()->SetRenderState( D3DRS_STENCILREF, 0x0 );

	DX::Graphic::GetDevice()->SetRenderState( D3DRS_STENCILMASK, -1 );
	DX::Graphic::GetDevice()->SetRenderState( D3DRS_STENCILWRITEMASK, -1 );

	DX::Graphic::GetDevice()->SetRenderState( D3DRS_STENCILZFAIL, D3DSTENCILOP_INCR );
	DX::Graphic::GetDevice()->SetRenderState( D3DRS_STENCILFAIL, D3DSTENCILOP_INCR );
	DX::Graphic::GetDevice()->SetRenderState( D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE );

	DX::Graphic::GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	DX::Graphic::GetDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
	DX::Graphic::GetDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );
}

void DXGlow::AllOff()
{
	DX::Graphic::GetDevice()->SetRenderState( D3DRS_STENCILENABLE, FALSE );

	DX::Graphic::GetDevice()->SetRenderState( D3DRS_STENCILREF, 0x0 );

	DX::Graphic::GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
}

void DXGlow::GetTextures()
{
	//Save Textures...
	DX::Graphic::GetDevice()->GetTexture( 0, &lpaTextures[0] );
	DX::Graphic::GetDevice()->GetTexture( 1, &lpaTextures[1] );
	DX::Graphic::GetDevice()->GetTexture( 2, &lpaTextures[2] );
	DX::Graphic::GetDevice()->GetTexture( 3, &lpaTextures[3] );
	DX::Graphic::GetDevice()->GetTexture( 4, &lpaTextures[4] );
	DX::Graphic::GetDevice()->GetTexture( 5, &lpaTextures[5] );
	DX::Graphic::GetDevice()->GetTexture( 6, &lpaTextures[6] );
	DX::Graphic::GetDevice()->GetTexture( 7, &lpaTextures[7] );
}

void DXGlow::SetTextures()
{
	//Load Textures...
	DX::Graphic::GetDevice()->SetTexture( 0, lpaTextures[0] );
	DX::Graphic::GetDevice()->SetTexture( 1, lpaTextures[1] );
	DX::Graphic::GetDevice()->SetTexture( 2, lpaTextures[2] );
	DX::Graphic::GetDevice()->SetTexture( 3, lpaTextures[3] );
	DX::Graphic::GetDevice()->SetTexture( 4, lpaTextures[4] );
	DX::Graphic::GetDevice()->SetTexture( 5, lpaTextures[5] );
	DX::Graphic::GetDevice()->SetTexture( 6, lpaTextures[6] );
	DX::Graphic::GetDevice()->SetTexture( 7, lpaTextures[7] );
}
