#pragma once

#include <filesystem>

#include "CPoint.h"
#include "ObjectOwner.h"

#include "CEffekSeerParticleScript.h"

class CEffekseerDistortion : public EffekseerRenderer::DistortingCallback
{
public:
	CEffekseerDistortion()
	{

	}

	virtual ~CEffekseerDistortion()
	{

	}

	bool			CopyRenderTarget();

	bool			OnDistorting() override;
};

class CEffekseerTextureLoader : public ::Effekseer::TextureLoader
{
public:
	CEffekseerTextureLoader() {};
	virtual ~CEffekseerTextureLoader() {};

	Effekseer::TextureData			* Load( const EFK_CHAR * path, Effekseer::TextureType textureType ) override;
	void							Unload( Effekseer::TextureData * data ) override;

};

class CEffekseerHandler
{
private:
	static const int				MAX_EFFEKSEER_SPRITES = 2000;

public:
	CEffekseerHandler();
	~CEffekseerHandler();

	EffekseerRendererDX9::Renderer	* GetRenderer() { return pcRenderer; }
	Effekseer::Manager				* GetManager() { return pcManager; };

	LPDIRECT3DSURFACE9				GetDistortionRenderTarget() { return lpRenderSurface; }
	DX::RenderTarget_ptr			GetRenderTarget(){ return pRenderTargetDistortion; }

	void							Init();

	CEffekSeerParticleScript		* LoadScript( const std::string strFileName );

	bool							AddScript( CEffekSeerParticleScript * pcScript );

	void							Kill( UINT uID );
	void							Kill( UINT uID, UINT uOwnerID );
	void							KillHandle( Effekseer::Handle iHandle );

	void							Update( float fTime );

	void							Render();

	void							OnLost( CEffekSeerParticleSystem * pcParticleSystem );

	void							OnLostDevice();
	void							OnResetDevice();

private:
	EffekseerRendererDX9::Renderer	* pcRenderer = nullptr;
	Effekseer::Manager				* pcManager = nullptr;

	DX::RenderTarget_ptr			pRenderTargetDistortion = nullptr;

	LPDIRECT3DSURFACE9				lpRenderSurface = nullptr;

	std::vector<CEffekSeerParticleScript *> vParticleScriptCache;
	std::vector<CEffekSeerParticleSystem *> vParticleSystems;

	CEffekSeerParticleScript		* GetScriptFromCache( const std::string strFileName );

	void							AddScriptToCache( CEffekSeerParticleScript * pcParticleScript );

	void							CreateRenderTarget();
};

