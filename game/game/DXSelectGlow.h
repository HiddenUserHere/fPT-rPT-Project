#pragma once

#include "DXRenderer.h"
#include "DXEffect.h"
#include "DXDepthStencilBuffer.h"
#include "DXRenderTarget.h"
#include "DXRenderer.h"
#include "DXSprite.h"
#include "DXColor.h"
#include "DXViewport.h"

namespace DX
{

class SelectGlow
{
public:
	void Init( Renderer_ptr _pRenderer );
	void Shutdown();

	BOOL Begin( D3DCOLOR d3dColor, BOOL bSmallPass );
	void End( BOOL bSmallPass );

	void Draw();

	BOOL IsDisabled() { return bDisabled; }
	void SetDisabled( BOOL bValue ) { bDisabled = bValue; }

	void		SetSizeGlow( float f ) { fSizeGlow = f; GaussianFunc(); }

	Effect_ptr		GetEffect() { return pEffect; }

	float			* GetOffset() { return faOffset; }
	float			* GetWeight() { return faWeight; }

private:
	void GaussianFunc();
	float Gaussian(float x);

private:
	Renderer_ptr pRenderer;

	Effect_ptr pEffect;

	DepthStencilBuffer_ptr pDepthStencilBuffer;
	RenderTarget_ptr pRenderTarget1;
	RenderTarget_ptr pRenderTarget2;

	Texture_ptr pTexture;

	Sprite_ptr pSprite;
	
	float faOffset[15];
	float faWeight[15];

	Color cColor;
	BOOL bGotSomething = FALSE;

	BOOL bFirstTime = TRUE;

	BOOL bDisabled = FALSE;

	Viewport vOldViewport;

	float			fSizeGlow = 4.0f;
};

extern SelectGlow cSelectGlow1;		//Characters
extern SelectGlow cSelectGlow2;		//Loot
extern SelectGlow cSelectGlow3;		//Circle
extern SelectGlow cSelectGlow4;		//Broyale Safe
}