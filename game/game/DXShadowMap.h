#pragma once

#define DXSHADOW_MAP_SIZE		512

namespace DX
{

class ShadowMap
{
public:
	void Init( Renderer_ptr _pRenderer );
	void Shutdown();

	BOOL Begin( BOOL bSmallPass );
	void End( BOOL bSmallPass );

	void Draw();

	void SetupFrame( D3DXVECTOR3 sEye, D3DXVECTOR3 sLookAt, D3DXVECTOR3 sUp );

	void UpdateShadowMapping();

	BOOL IsDisabled() { return bDisabled; }
	void SetDisabled( BOOL bValue ) { bDisabled = bValue; }

	Effect_ptr		GetEffect() { return pEffect; }

	float			* GetOffset() { return faOffset; }
	float			* GetWeight() { return faWeight; }

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

	D3DXMATRIX	m_LightViewMatrix;

	D3DXMATRIX	m_LightProjectionMatrix;

	D3DXMATRIX m_CameraProjectionMatrix;
	D3DXMATRIX m_CameraViewMatrix;
};

extern ShadowMap cShadowMap1;

};