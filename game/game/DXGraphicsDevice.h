#pragma once

#include "DXVertexElements.h"
#include "DXVertexDeclaration.h"
#include "DXVertexBuffer.h"
#include "DXIndexBuffer.h"

#include "DXViewport.h"
#include "DXRenderTarget.h"
#include "DXDepthStencilBuffer.h"

#include "DXFontFactory.h"
#include "DXTextureFactory.h"
#include "DXEffectFactory.h"
#include "DXSpriteFactory.h"
#include "DXModelFactory.h"
#include "DXTerrainFactory.h"

#include "DXShadow.h"

namespace DX
{

enum RenderState
{
	RENDERSTATE_ScissorTest,

	RENDERSTATE_StencilEnable,
	RENDERSTATE_StencilFail,
	RENDERSTATE_StencilZFail,
	RENDERSTATE_StencilPass,
	RENDERSTATE_StencilFunc,
	RENDERSTATE_StencilRef,
	RENDERSTATE_StencilMask,
	RENDERSTATE_StencilWriteMask,
};

enum CompareFunction
{
	COMPAREFUNCTION_Never,
	COMPAREFUNCTION_Less,
	COMPAREFUNCTION_Equal,
	COMPAREFUNCTION_LessEqual,
	COMPAREFUNCTION_Greater,
	COMPAREFUNCTION_NotEqual,
	COMPAREFUNCTION_GreaterEqual,
	COMPAREFUNCTION_Always,
};

enum StencilOperation
{
	STENCILOPERATION_Keep,
	STENCILOPERATION_Zero,
	STENCILOPERATION_Replace,
	STENCILOPERATION_IncrSat,
	STENCILOPERATION_DecrSat,
	STENCILOPERATION_Invert,
	STENCILOPERATION_Incr,
	STENCILOPERATION_Decr,
};

typedef std::shared_ptr<class GraphicsDevice> GraphicsDevice_ptr;

class GraphicsDevice : public std::enable_shared_from_this<GraphicsDevice>
{
	friend class RenderTarget;
	friend class DepthStencilBuffer;

private:
	struct Info
	{
		UINT		  uBackBufferWidth;
		UINT		  uBackBufferHeight;
		D3DFORMAT	  d3dBackBufferFormat;

		UINT		  uDepthStencilBufferWidth;
		UINT		  uDepthStencilBufferHeight;
		D3DFORMAT	  d3dDepthStencilBufferFormat;

		UINT		  uRenderTargetWidth;
		UINT		  uRenderTargetHeight;
		D3DFORMAT	  d3dRenderTargetFormat;
	};

public:
										  GraphicsDevice( LPDIRECT3DDEVICE9 lpD3DDevice );
	virtual								 ~GraphicsDevice();

	LPDIRECT3DDEVICE9					  Get() { return lpD3DDevice; }
	
	void								  OnLostDevice();
	void								  OnResetDevice();

	Viewport							  GetViewport() const;
	BOOL								  SetViewport( const Viewport & vViewport );

	BOOL								  Clear( BOOL bTarget, BOOL bZBuffer, BOOL bStencil, D3DCOLOR d3dColor = D3DCOLOR_ARGB( 0, 0, 0, 0 ) );
	
	BOOL								  Begin();
	BOOL								  End();

	void								  SetRenderState( RenderState eState, DWORD dwValue );

	void								  SetScissorRect( const RECT & rRect );

	BOOL								  SetRenderTarget( RenderTarget_ptr pRenderTarget );
	void								  UnsetRenderTarget();
	
	BOOL								  SetDepthStencilBuffer( DepthStencilBuffer_ptr pDepthStencilBuffer );
	void								  UnsetDepthStencilBuffer();
	
	void								  Draw2DLine( const POINT & p1, const POINT & p2, const DX::Color & cColor = DX::Color() );
	void								  Draw2DRectangle( const RECT & rRect, const DX::Color & cColor = DX::Color() );

	VertexDeclaration_ptr				  CreateVertexDeclaration( VertexElements_ptr pVertexElements );
	VertexBuffer_ptr					  CreateVertexBuffer( UINT uElementSize, UINT uElementCount, BOOL bDynamic = FALSE );
	IndexBuffer_ptr						  CreateIndexBuffer( UINT uPrimitiveSize, UINT uPrimitiveCount );

	RenderTarget_ptr					  MakeRenderTarget( int iSharableID = 0, UINT uSizeShiftRight = 0 );
	RenderTarget_ptr					  MakeRenderTarget( int iSharableID, UINT uWidth, UINT uHeight );
	DepthStencilBuffer_ptr				  MakeDepthStencilBuffer( int iSharableID = 0, UINT uSizeShiftRight = 0 );
	DepthStencilBuffer_ptr				  MakeDepthStencilBuffer( int iSharableID, UINT uWidth, UINT uHeight );

	Shadow_ptr							  MakeShadow( int iSharableID, UINT uWidth, UINT uHeight );

	void								  PushScissorRect( const RECT & rRect );
	void								  PopScissorRect();
	BOOL								  HaveScissorRect() { return (vScissorRect.size() > 0); }
	RECT								  * GetScissorRect();
	void								  GetScissorRectInside( RECT & rRect );

	UINT								  GetBackBufferWidth() const { return sInfo.uBackBufferWidth; }
	UINT								  GetBackBufferHeight() const { return sInfo.uBackBufferHeight; }

	UINT								  GetRenderTargetWidth() const { return sInfo.uRenderTargetWidth; }
	UINT								  GetRenderTargetHeight() const { return sInfo.uRenderTargetHeight; }

	const Info							& GetInfo() const { return sInfo; }

	SpriteFactory_ptr					  GetSpriteFactory() const { return pSpriteFactory; }
	TextureFactory_ptr					  GetTextureFactory() const { return pTextureFactory; }
	FontFactory_ptr						  GetFontFactory() const { return pFontFactory; }
	EffectFactory_ptr					  GetEffectFactory() const { return pEffectFactory; }
	ModelFactory_ptr					  GetModelFactory() const { return pModelFactory; }
	TerrainFactory_ptr					  GetTerrainFactory() const { return pTerrainFactory; }

	Sprite_ptr							  GetDefaultSprite() const { return pDefaultSprite; }
	Texture_ptr							  GetDefaultTexture() const { return pDefaultTexture; }
	Font_ptr							  GetDefaultFont() const { return pDefaultFont; }

private:
	void								  Setup();
	void								  UpdateRenderTargetInfo();

	BOOL								  BuildRenderTarget( RenderTarget_ptr pRenderTarget );
	BOOL								  BuildDepthStencilBuffer( DepthStencilBuffer_ptr pDepthStencilBuffer );

	void								  PushRenderTarget();
	void								  PopRenderTarget();

	void								  PushDepthStencilBuffer();
	void								  PopDepthStencilBuffer();

private:
	LPDIRECT3DDEVICE9					  lpD3DDevice = NULL;

	Info								  sInfo;
	
	SpriteFactory_ptr					  pSpriteFactory;
	TextureFactory_ptr					  pTextureFactory;
	FontFactory_ptr						  pFontFactory;
	EffectFactory_ptr					  pEffectFactory;
	ModelFactory_ptr					  pModelFactory;
	TerrainFactory_ptr					  pTerrainFactory;
	
	Sprite_ptr							  pDefaultSprite;
	Texture_ptr							  pDefaultTexture;

	std::vector<RenderTarget_ptr>		  vRenderTargets;
	std::vector<DepthStencilBuffer_ptr>	  vDepthStencilBuffers;
	std::vector<Shadow_ptr>				  vShadows;

	std::stack<LPDIRECT3DSURFACE9>		  sRenderTargets;
	std::stack<LPDIRECT3DSURFACE9>		  sDepthStencilBuffers;

	std::vector<RECT*>					  vScissorRect;

	Font_ptr							  pDefaultFont;
};

}