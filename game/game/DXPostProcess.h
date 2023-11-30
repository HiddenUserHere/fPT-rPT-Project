#pragma once

enum EPostEffects
	{
		POSTPROCESSID_None,
		POSTPROCESSID_Dead,
		POSTPROCESSID_Poison,
		POSTPROCESSID_ControlColor,
		POSTPROCESSID_BattleRoyale,
		POSTPROCESSID_FakeHDR,
};

class DXPostProcess
{
protected:
	float							fDeadIntensity	= 0.1f;
	float							fTime			= 0.1f;
	BOOL							bPostProcess;
	BOOL							bPostProcessInitialized;
	LPD3DXEFFECT					lpRenderEffect;
	LPD3DXSPRITE					lpRenderSprite;
	LPDIRECT3DTEXTURE9				lpRenderTexture;
	LPDIRECT3DSURFACE9				lpRenderSurfaceTexture;
	LPDIRECT3DSURFACE9				lpRenderSurface;
	LPDIRECT3DSURFACE9				lpBackBuffer;

	HRESULT							hr;
private:

	void							PostProcessON( EPostEffects eTechnique );
	void							PostProcessOFF();
	void							SetPostProcessFloat( const char * pszVariable, const float fValue );
	void							PostProcess3D();

	BOOL							CreatePostProcessBuffers();
	BOOL							CreatePostProcess();

public:

	static BOOL						FakeHDR;

	DXPostProcess();
	virtual ~DXPostProcess();

	void										OnBegin3D();
	void										OnEnd3D();
	void										OnResetDevice();
	void										OnLostDevice();
//	void										Reload

	BOOL										Init();
};

