#pragma once


class DXGlow
{
public:
	DXGlow();
	virtual ~DXGlow();


	void						CreateRenderBuffers();
	void						Init();

	BOOL						BeginRender();

	void						EndRender();

	void						Begin();

	BOOL						BeginPass();

	void						EndPass();

	void						End();

	void						Render2D();
	void						Render2D2();
	void						RenderGlow();

	BOOL						SetTechnique();

	void						Test();
	void						Test2();
	void						AllOff();

	void						GetTextures();

	void						SetTextures();

	LPDIRECT3DTEXTURE9			GetGlowTexture() { return lpRenderTextureGlow; }

	LPDIRECT3DTEXTURE9			lpRenderTexture = NULL;
	LPDIRECT3DTEXTURE9			lpRenderTextureGlow = NULL;



	LPDIRECT3DSURFACE9			lpRenderSurface = NULL;
	LPDIRECT3DSURFACE9			lpRenderSurfaceGlow = NULL;
	LPDIRECT3DSURFACE9			lpBackBuffer = NULL;
	LPD3DXEFFECT				lpRenderEffect = NULL;
	LPD3DXSPRITE				lpRenderSprite = NULL;

	LPDIRECT3DBASETEXTURE9		lpaTextures[8];

protected:

	BOOL						bNewRenderTarget = FALSE;

	UINT						uPasses = 0;

	UINT						uPass = 0;

	BOOL						bInit = FALSE;
};
