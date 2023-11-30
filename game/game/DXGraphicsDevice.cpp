#include "stdafx.h"
#include "DXGraphicsDevice.h"

namespace DX
{

GraphicsDevice::GraphicsDevice( LPDIRECT3DDEVICE9 lpD3DDevice ) : lpD3DDevice(lpD3DDevice)
{
	ADDREF( lpD3DDevice );
	
	pSpriteFactory = std::make_shared<SpriteFactory>( this );
	pTextureFactory = std::make_shared<TextureFactory>( this );
	pFontFactory = std::make_shared<FontFactory>( this );
	pEffectFactory = std::make_shared<EffectFactory>( this );
	pModelFactory = std::make_shared<ModelFactory>( this );
	pTerrainFactory = std::make_shared<TerrainFactory>( this );

	pDefaultSprite = pSpriteFactory->MakeSprite( FALSE );
	pDefaultTexture = pTextureFactory->MakeBlankTexture();
	pDefaultFont = pFontFactory->MakeFont( pDefaultSprite, DEFAULT_FONT, DEFAULT_FONT_HEIGHT, DEFAULT_FONT_WIDTH, FALSE, FALSE );

	Setup();
}

GraphicsDevice::~GraphicsDevice()
{
	vDepthStencilBuffers.clear();
	vRenderTargets.clear();
	// vShadows.clear();

	pDefaultFont = nullptr;
	pDefaultTexture = nullptr;
	pDefaultSprite = nullptr;

	pTerrainFactory = nullptr;
	pModelFactory = nullptr;
	pEffectFactory = nullptr;
	pFontFactory = nullptr;
	pTextureFactory = nullptr;
	pSpriteFactory = nullptr;

	RELEASE( lpD3DDevice );
}

void GraphicsDevice::OnLostDevice()
{
	pTerrainFactory->OnLostDevice();
	pModelFactory->OnLostDevice();
	pFontFactory->OnLostDevice();
	pTextureFactory->OnLostDevice();
	pEffectFactory->OnLostDevice();
	pSpriteFactory->OnLostDevice();

	for( auto pRenderTarget : vRenderTargets )
		pRenderTarget->OnLostDevice();

	for( auto pDepthStencilBuffer : vDepthStencilBuffers )
		pDepthStencilBuffer->OnLostDevice();

	//UI::WebViewer::OnLostDevice();
}

void GraphicsDevice::OnResetDevice()
{
	Setup();
	
	pTerrainFactory->OnResetDevice();
	pModelFactory->OnResetDevice();
	pFontFactory->OnResetDevice();
	pTextureFactory->OnResetDevice();
	pEffectFactory->OnResetDevice();
	pSpriteFactory->OnResetDevice();

	for( auto pRenderTarget : vRenderTargets )
		pRenderTarget->OnResetDevice();

	for( auto pDepthStencilBuffer : vDepthStencilBuffers )
		pDepthStencilBuffer->OnResetDevice();

//	UI::WebViewer::OnResetDevice();
}

Viewport GraphicsDevice::GetViewport() const
{
	Viewport vViewport;

	vViewport.dwX		= 0;
	vViewport.dwY		= 0;
	vViewport.dwWidth	= GetBackBufferWidth();
	vViewport.dwHeight	= GetBackBufferHeight();
	vViewport.fMinZ		= 0.0f;
	vViewport.fMaxZ		= 1.0f;

	D3DVIEWPORT9 d3dViewport;
	if( FAILED( lpD3DDevice->GetViewport( &d3dViewport ) ) )
		return vViewport;

	vViewport.dwX		= d3dViewport.X;
	vViewport.dwY		= d3dViewport.Y;
	vViewport.dwWidth	= d3dViewport.Width;
	vViewport.dwHeight	= d3dViewport.Height;
	vViewport.fMinZ		= d3dViewport.MinZ;
	vViewport.fMaxZ		= d3dViewport.MaxZ;

	return vViewport;
}

BOOL GraphicsDevice::SetViewport( const Viewport & vViewport )
{
	D3DVIEWPORT9 d3dViewport;
	d3dViewport.X		= vViewport.dwX;
	d3dViewport.Y		= vViewport.dwY;
	d3dViewport.Width	= vViewport.dwWidth;
	d3dViewport.Height	= vViewport.dwHeight;
	d3dViewport.MinZ	= vViewport.fMinZ;
	d3dViewport.MaxZ	= vViewport.fMaxZ;

	if( FAILED( lpD3DDevice->SetViewport( &d3dViewport ) ) )
		return FALSE;

	return TRUE;
}

BOOL GraphicsDevice::Clear( BOOL bTarget, BOOL bZBuffer, BOOL bStencil, D3DCOLOR d3dColor )
{
	DWORD dwFlags = 0;

	if( bTarget )
		dwFlags |= D3DCLEAR_TARGET;

	if( bZBuffer )
		dwFlags |= D3DCLEAR_ZBUFFER;

	if( bStencil )
		dwFlags |= D3DCLEAR_STENCIL;

	if( FAILED( lpD3DDevice->Clear( 0, NULL, dwFlags, d3dColor, 1.0f, 0 ) ) )
		return FALSE;

	return TRUE;
}

BOOL GraphicsDevice::Begin()
{
	return SUCCEEDED( lpD3DDevice->BeginScene() );
}

BOOL GraphicsDevice::End()
{
	if( FAILED( lpD3DDevice->EndScene() ) )
		return FALSE;

	return SUCCEEDED( lpD3DDevice->Present( NULL, NULL, NULL, NULL ) );
}

void GraphicsDevice::SetRenderState( RenderState eState, DWORD dwValue )
{
	//Set Render State
	switch( eState )
	{
	case RENDERSTATE_ScissorTest:
		lpD3DDevice->SetRenderState( D3DRS_SCISSORTESTENABLE, dwValue );
		break;

	case RENDERSTATE_StencilEnable:
		lpD3DDevice->SetRenderState( D3DRS_STENCILENABLE, dwValue );
		break;
	}
}

void GraphicsDevice::SetScissorRect( const RECT & rRect )
{
	//Set Clipping Rect
	lpD3DDevice->SetScissorRect( &rRect );
}

BOOL GraphicsDevice::SetRenderTarget( RenderTarget_ptr pRenderTarget )
{
	if( !pRenderTarget )
		return FALSE;

	if( !pRenderTarget->GetSurface() )
		return FALSE;

	//Push Render Target to stack
	PushRenderTarget();

	//Set Render Target
	if( FAILED( lpD3DDevice->SetRenderTarget( 0, pRenderTarget->GetSurface() ) ) )
	{
		//Pop Render Target back from stack
		PopRenderTarget();
		
		return FALSE;
	}
	
	//Render Target
	UpdateRenderTargetInfo();

	return TRUE;
}

void GraphicsDevice::UnsetRenderTarget()
{
	//Pop Render Target from stack
	PopRenderTarget();
	
	//Render Target
	UpdateRenderTargetInfo();
}

BOOL GraphicsDevice::SetDepthStencilBuffer( DepthStencilBuffer_ptr pDepthStencilBuffer )
{
	if( !pDepthStencilBuffer )
		return FALSE;

	if( !pDepthStencilBuffer->GetSurface() )
		return FALSE;

	//Push Depth Stencil Buffer to stack
	PushDepthStencilBuffer();

	//Set Depth Stencil Buffer
	if( FAILED( lpD3DDevice->SetDepthStencilSurface( pDepthStencilBuffer->GetSurface() ) ) )
	{
		//Pop Depth Stencil Buffer back from stack
		PopDepthStencilBuffer();

		return FALSE;
	}

	return TRUE;
}

void GraphicsDevice::UnsetDepthStencilBuffer()
{
	//Pop Depth Stencil Buffer from stack
	PopDepthStencilBuffer();
}

void GraphicsDevice::Draw2DLine( const POINT & p1, const POINT & p2, const DX::Color & cColor )
{
	struct TransformedLitVertex
	{
		float		  fX, fY, fZ;
		float		  fRHW;
		D3DCOLOR	  d3dColor;

		TransformedLitVertex() {}
		TransformedLitVertex( float fX, float fY, float fZ, float fRHW, D3DCOLOR d3dColor ) : fX(fX), fY(fY), fZ(fZ), fRHW(fRHW), d3dColor(d3dColor) {}
	};

	TransformedLitVertex aVertex[2];
	aVertex[0] = TransformedLitVertex( (float)(p1.x), (float)(p1.y), 0.0f, 1.0f, cColor.Get() );
	aVertex[1] = TransformedLitVertex( (float)(p2.x), (float)(p2.y), 0.0f, 1.0f, cColor.Get() );

	lpD3DDevice->SetTexture( 0, NULL );
	lpD3DDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
	lpD3DDevice->DrawPrimitiveUP( D3DPT_LINESTRIP, 1, aVertex, sizeof( TransformedLitVertex ) );
}

void GraphicsDevice::Draw2DRectangle( const RECT & rRect, const DX::Color & cColor )
{
	struct TransformedLitVertex
	{
		float		  fX, fY, fZ;
		float		  fRHW;
		D3DCOLOR	  d3dColor;

		TransformedLitVertex() {}
		TransformedLitVertex( float fX, float fY, float fZ, float fRHW, D3DCOLOR d3dColor ) : fX(fX), fY(fY), fZ(fZ), fRHW(fRHW), d3dColor(d3dColor) {}
	};

	TransformedLitVertex aVertex[5];
	aVertex[0] = TransformedLitVertex( (float)(rRect.left), (float)(rRect.top), 0.0f, 1.0f, cColor.Get() );
	aVertex[1] = TransformedLitVertex( (float)(rRect.right), (float)(rRect.top), 0.0f, 1.0f, cColor.Get() );
	aVertex[2] = TransformedLitVertex( (float)(rRect.right), (float)(rRect.bottom), 0.0f, 1.0f, cColor.Get() );
	aVertex[3] = TransformedLitVertex( (float)(rRect.left), (float)(rRect.bottom), 0.0f, 1.0f, cColor.Get() );
	aVertex[4] = TransformedLitVertex( (float)(rRect.left), (float)(rRect.top), 0.0f, 1.0f, cColor.Get() );

	lpD3DDevice->SetTexture( 0, NULL );
	lpD3DDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
	lpD3DDevice->DrawPrimitiveUP( D3DPT_LINESTRIP, 4, aVertex, sizeof( TransformedLitVertex ) );
}

VertexDeclaration_ptr GraphicsDevice::CreateVertexDeclaration( VertexElements_ptr pVertexElements )
{
	LPDIRECT3DVERTEXDECLARATION9 lpVertexDeclaration;
	if( FAILED( lpD3DDevice->CreateVertexDeclaration( pVertexElements->GetElements().data(), &lpVertexDeclaration ) ) )
	{
		WRITEDBG( "Could not create Vertex Declaration" );

		return nullptr;
	}

	//Create Vertex Declaration holder (Adds Reference)
	auto pVertexDeclaration = std::make_shared<VertexDeclaration>( lpVertexDeclaration );

	//Remove our Reference to Vertex Declaration
	RELEASE( lpVertexDeclaration );

	//Return Vertex Declaration
	return pVertexDeclaration;
}

VertexBuffer_ptr GraphicsDevice::CreateVertexBuffer( UINT uElementSize, UINT uElementCount, BOOL bDynamic )
{
	D3DPOOL d3dPool = bDynamic ? D3DPOOL_DEFAULT : DXGRAPHICENGINE_D3DPOOL;
	DWORD dwUsage	= bDynamic ? D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC : D3DUSAGE_WRITEONLY;

	LPDIRECT3DVERTEXBUFFER9 lpVertexBuffer;
	if( FAILED( lpD3DDevice->CreateVertexBuffer( uElementSize * uElementCount, dwUsage, 0, d3dPool, &lpVertexBuffer, NULL ) ) )
	{
		WRITEDBG( "Could not create Vertex Buffer of size %d*%d", uElementSize, uElementCount );

		return nullptr;
	}

	//Create Vertex Buffer holder (Adds Reference)
	auto pVertexBuffer = std::make_shared<VertexBuffer>( lpVertexBuffer, uElementSize, uElementCount, bDynamic );

	//Remove our Reference to Vertex Buffer
	RELEASE( lpVertexBuffer );

	//Return Vertex Buffer
	return pVertexBuffer;
}

IndexBuffer_ptr GraphicsDevice::CreateIndexBuffer( UINT uPrimitiveSize, UINT uPrimitiveCount )
{
	LPDIRECT3DINDEXBUFFER9 lpIndexBuffer;
	if( FAILED( lpD3DDevice->CreateIndexBuffer( uPrimitiveSize * uPrimitiveCount, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, DXGRAPHICENGINE_D3DPOOL, &lpIndexBuffer, NULL ) ) )
	{
		WRITEDBG( "Could not create Index Buffer of size %d*%d", uPrimitiveSize, uPrimitiveCount );

		return nullptr;
	}

	//Create Index Buffer holder (Adds Reference)
	auto pIndexBuffer = std::make_shared<IndexBuffer>( lpIndexBuffer, uPrimitiveSize, uPrimitiveCount );

	//Remove our Reference to Index Buffer
	RELEASE( lpIndexBuffer );

	//Return Index Buffer
	return pIndexBuffer;
}

RenderTarget_ptr GraphicsDevice::MakeRenderTarget( int iSharableID, UINT uSizeShiftRight )
{
	//Anything but -1 is a Sharable ID
	if( iSharableID != -1 )
	{
		//Find Render Target in Cache with same Sharable ID..
		for( auto pRenderTarget : vRenderTargets )
		{
			//Same Render Target specifics? Share it
			if( (pRenderTarget->GetSharableID() == iSharableID) && (pRenderTarget->GetSizeShiftRight() == uSizeShiftRight) )
				return pRenderTarget;
		}
	}

	//New Render Target
	auto pRenderTarget = std::make_shared<RenderTarget>( shared_from_this(), iSharableID, uSizeShiftRight );

	//Build Render Target
	if( !BuildRenderTarget( pRenderTarget ) )
		return nullptr;

	//Remember Render Target
	vRenderTargets.push_back( pRenderTarget );

	//Return Render Target
	return pRenderTarget;
}

RenderTarget_ptr GraphicsDevice::MakeRenderTarget( int iSharableID, UINT uWidth, UINT uHeight )
{
	//Anything but -1 is a Sharable ID
	if ( iSharableID != -1 )
	{
		//Find Render Target in Cache with same Sharable ID..
		for ( auto pRenderTarget : vRenderTargets )
		{
			//Same Render Target specifics? Share it
			if ( (pRenderTarget->GetSharableID() == iSharableID) && (pRenderTarget->GetWidth() == uWidth) && (pRenderTarget->GetHeight() == uHeight) )
				return pRenderTarget;
		}
	}

	//New Render Target
	auto pRenderTarget = std::make_shared<RenderTarget>( shared_from_this(), iSharableID, uWidth, uHeight );

	//Build Render Target
	if ( !BuildRenderTarget( pRenderTarget ) )
		return nullptr;

	//Remember Render Target
	vRenderTargets.push_back( pRenderTarget );

	//Return Render Target
	return pRenderTarget;
}

DepthStencilBuffer_ptr GraphicsDevice::MakeDepthStencilBuffer( int iSharableID, UINT uSizeShiftRight )
{
	//Anything but -1 is a Sharable ID
	if( iSharableID != -1 )
	{
		//Find Depth Stencil Buffer in Cache with same Sharable ID..
		for( auto pDepthStencilBuffer : vDepthStencilBuffers )
		{
			//Same Depth Stencil Buffer specifics? Share it
			if( (pDepthStencilBuffer->GetSharableID() == iSharableID) && (pDepthStencilBuffer->GetSizeShiftRight() == uSizeShiftRight) )
				return pDepthStencilBuffer;
		}
	}

	//New Depth Stencil Buffer
	auto pDepthStencilBuffer = std::make_shared<DepthStencilBuffer>( shared_from_this(), iSharableID, uSizeShiftRight );

	//Build Depth Stencil Buffer
	if( !BuildDepthStencilBuffer( pDepthStencilBuffer ) )
		return nullptr;

	//Remember Depth Stencil Buffer
	vDepthStencilBuffers.push_back( pDepthStencilBuffer );

	//Return Depth Stencil Buffer
	return pDepthStencilBuffer;
}

DepthStencilBuffer_ptr GraphicsDevice::MakeDepthStencilBuffer( int iSharableID, UINT uWidth, UINT uHeight )
{
	//Anything but -1 is a Sharable ID
	if ( iSharableID != -1 )
	{
		//Find Depth Stencil Buffer in Cache with same Sharable ID..
		for ( auto pDepthStencilBuffer : vDepthStencilBuffers )
		{
			//Same Depth Stencil Buffer specifics? Share it
			if ( (pDepthStencilBuffer->GetSharableID() == iSharableID) && (pDepthStencilBuffer->GetWidth() == uWidth) && (pDepthStencilBuffer->GetHeight() == uHeight) )
				return pDepthStencilBuffer;
		}
	}

	//New Depth Stencil Buffer
	auto pDepthStencilBuffer = std::make_shared<DepthStencilBuffer>( shared_from_this(), iSharableID, uWidth, uHeight );

	//Build Depth Stencil Buffer
	if ( !BuildDepthStencilBuffer( pDepthStencilBuffer ) )
		return nullptr;

	//Remember Depth Stencil Buffer
	vDepthStencilBuffers.push_back( pDepthStencilBuffer );

	//Return Depth Stencil Buffer
	return pDepthStencilBuffer;
}

Shadow_ptr GraphicsDevice::MakeShadow( int iSharableID, UINT uWidth, UINT uHeight )
{
	//Anything but -1 is a Sharable ID
	if ( iSharableID != -1 )
	{
		//Find Depth Stencil Buffer in Cache with same Sharable ID..
		for ( auto pShadow : vShadows )
		{
			//Same Shadow specifics? Share it
			if ( (pShadow->GetSharableID() == iSharableID) && (pShadow->GetWidth() == uWidth) && (pShadow->GetHeight() == uHeight) )
				return pShadow;
		}
	}

	//New Depth Stencil Buffer
	auto pShadow = std::make_shared<Shadow>( shared_from_this(), iSharableID, uWidth, uHeight );

	//Build Depth Stencil Buffer
	if ( !pShadow->Init() )
		return nullptr;

	//Remember Depth Stencil Buffer
	vShadows.push_back( pShadow );

	//Return Depth Stencil Buffer
	return pShadow;
}

void GraphicsDevice::PushScissorRect( const RECT & rRect )
{
	SetRenderState( DX::RENDERSTATE_ScissorTest, TRUE );

	RECT * pRect = new RECT;
	CopyMemory( pRect, &rRect, sizeof( RECT ) );
	vScissorRect.push_back( pRect );

	DX::Graphic::GetDevice()->SetScissorRect( &rRect );
}

void GraphicsDevice::PopScissorRect()
{
	if ( vScissorRect.size() > 0 )
	{
		DELET( vScissorRect[vScissorRect.size() - 1] );

		vScissorRect.pop_back();

		if ( vScissorRect.size() == 0 )
			SetRenderState( DX::RENDERSTATE_ScissorTest, FALSE );
		else
			DX::Graphic::GetDevice()->SetScissorRect( vScissorRect[vScissorRect.size() - 1] );
	}
}

RECT * GraphicsDevice::GetScissorRect()
{
	if ( vScissorRect.size() > 0 )
		return vScissorRect[vScissorRect.size() - 1];

	return nullptr;
}

void GraphicsDevice::GetScissorRectInside( RECT & rRect )
{
	if ( HaveScissorRect() )
	{
		RECT * p = GetScissorRect();

		if ( rRect.top < p->bottom )
		{
			if ( rRect.bottom > p->bottom )
				rRect.bottom = (p->bottom - rRect.top) + rRect.top;
		}
		else
		{
			rRect.top		= p->bottom;
			rRect.bottom	= p->bottom;
		}

		if ( rRect.left < p->right )
		{
			if ( rRect.right > p->right )
				rRect.right = (p->right - rRect.left) + rRect.left;
		}
		else
		{
			rRect.left	= p->right;
			rRect.right = p->right;
		}
	}
}

void GraphicsDevice::Setup()
{
	//Clear Info
	memset( &sInfo, 0, sizeof( sInfo ) );

	//Back Buffer
	{
		LPDIRECT3DSURFACE9 lpSurface;
		if( SUCCEEDED( lpD3DDevice->GetRenderTarget( 0, &lpSurface ) ) )
		{
			D3DSURFACE_DESC d3dSurfaceDesc;
			if( SUCCEEDED( lpSurface->GetDesc( &d3dSurfaceDesc ) ) )
			{
				sInfo.uBackBufferWidth		= d3dSurfaceDesc.Width;
				sInfo.uBackBufferHeight		= d3dSurfaceDesc.Height;
				sInfo.d3dBackBufferFormat	= d3dSurfaceDesc.Format;
			}

			lpSurface->Release();
		}
	}

	//Depth Stencil Buffer
	{
		LPDIRECT3DSURFACE9 lpSurface;
		if( SUCCEEDED( lpD3DDevice->GetDepthStencilSurface( &lpSurface ) ) )
		{
			D3DSURFACE_DESC d3dSurfaceDesc;
			if( SUCCEEDED( lpSurface->GetDesc( &d3dSurfaceDesc ) ) )
			{
				sInfo.uDepthStencilBufferWidth		= d3dSurfaceDesc.Width;
				sInfo.uDepthStencilBufferHeight		= d3dSurfaceDesc.Height;
				sInfo.d3dDepthStencilBufferFormat	= d3dSurfaceDesc.Format;
			}

			lpSurface->Release();
		}
	}

	//Render Target
	UpdateRenderTargetInfo();
}

void GraphicsDevice::UpdateRenderTargetInfo()
{
	LPDIRECT3DSURFACE9 lpSurface;
	if( SUCCEEDED( lpD3DDevice->GetRenderTarget( 0, &lpSurface ) ) )
	{
		D3DSURFACE_DESC d3dSurfaceDesc;
		if( SUCCEEDED( lpSurface->GetDesc( &d3dSurfaceDesc ) ) )
		{
			sInfo.uRenderTargetWidth	= d3dSurfaceDesc.Width;
			sInfo.uRenderTargetHeight	= d3dSurfaceDesc.Height;
			sInfo.d3dRenderTargetFormat	= d3dSurfaceDesc.Format;
		}

		lpSurface->Release();
	}
}

BOOL GraphicsDevice::BuildRenderTarget( RenderTarget_ptr pRenderTarget )
{
	RELEASE( pRenderTarget->lpSurface );
	RELEASE( pRenderTarget->lpTexture );

	UINT uWidth			= pRenderTarget->GetWidth();
	UINT uHeight		= pRenderTarget->GetHeight();
	D3DFORMAT d3dFormat = sInfo.d3dBackBufferFormat == D3DFMT_X4R4G4B4 ? D3DFMT_A4R4G4B4 : D3DFMT_A8R8G8B8;

	//Reduce Size?
	if( pRenderTarget->GetSizeShiftRight() > 0 )
	{
		uWidth	>>= pRenderTarget->GetSizeShiftRight();
		uHeight	>>= pRenderTarget->GetSizeShiftRight();
	}

	//Create Render Target Texture
	LPDIRECT3DTEXTURE9 lpTexture = nullptr;
	if( FAILED( lpD3DDevice->CreateTexture( uWidth, uHeight, 1, D3DUSAGE_RENDERTARGET, d3dFormat, D3DPOOL_DEFAULT, &lpTexture, NULL ) ) )
	{
		WRITEDBG( "Could not create Render Target Texture" );

		return FALSE;
	}

	//Create Render Target Surface
	LPDIRECT3DSURFACE9 lpSurface = nullptr;
	if( FAILED( lpTexture->GetSurfaceLevel( 0, &lpSurface ) ) )
	{
		RELEASE( lpTexture );

		WRITEDBG( "Could not create Render Target Surface" );

		return FALSE;
	}

	pRenderTarget->lpTexture	= lpTexture;
	pRenderTarget->lpSurface	= lpSurface;

	return TRUE;
}

BOOL GraphicsDevice::BuildDepthStencilBuffer( DepthStencilBuffer_ptr pDepthStencilBuffer )
{
	RELEASE( pDepthStencilBuffer->lpSurface );

	UINT uWidth			= pDepthStencilBuffer->GetWidth();
	UINT uHeight		= pDepthStencilBuffer->GetHeight();
	D3DFORMAT d3dFormat = sInfo.d3dDepthStencilBufferFormat;

	//Reduce Size?
	if( pDepthStencilBuffer->GetSizeShiftRight() > 0 )
	{
		uWidth	>>= pDepthStencilBuffer->GetSizeShiftRight();
		uHeight	>>= pDepthStencilBuffer->GetSizeShiftRight();
	}

	//Create Depth Stencil Buffer Surface
	LPDIRECT3DSURFACE9 lpSurface;
	if( FAILED( lpD3DDevice->CreateDepthStencilSurface( uWidth, uHeight, d3dFormat, D3DMULTISAMPLE_NONE, 0, FALSE, &lpSurface, NULL ) ) )
	{
		WRITEDBG( "Could not create Depth Stencil Buffer Surface" );

		return FALSE;
	}

	pDepthStencilBuffer->lpSurface	= lpSurface;

	return TRUE;
}

void GraphicsDevice::PushRenderTarget()
{
	LPDIRECT3DSURFACE9 lpSurface;
	lpD3DDevice->GetRenderTarget( 0, &lpSurface );

	sRenderTargets.push( lpSurface );
}

void GraphicsDevice::PopRenderTarget()
{
	LPDIRECT3DSURFACE9 lpSurface = sRenderTargets.top();

	lpD3DDevice->SetRenderTarget( 0, lpSurface );
	lpSurface->Release();

	sRenderTargets.pop();
}

void GraphicsDevice::PushDepthStencilBuffer()
{
	LPDIRECT3DSURFACE9 lpSurface;
	lpD3DDevice->GetDepthStencilSurface( &lpSurface );

	sDepthStencilBuffers.push( lpSurface );
}

void GraphicsDevice::PopDepthStencilBuffer()
{
	LPDIRECT3DSURFACE9 lpSurface = sDepthStencilBuffers.top();

	lpD3DDevice->SetDepthStencilSurface( lpSurface );
	lpSurface->Release();

	sDepthStencilBuffers.pop();
}

}