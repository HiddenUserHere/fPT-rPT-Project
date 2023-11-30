#pragma once

namespace DX
{

typedef std::shared_ptr<class Shadow> Shadow_ptr;


class Shadow
{
public:
	Shadow( std::weak_ptr<GraphicsDevice> pGraphicsDevice, int _iSharableID, UINT _uWidth, UINT _uHeight );
	virtual ~Shadow();

	bool					Init();

	bool					Begin( Effect_ptr pEffectShadow, Point3D sCamera, Point3D sAngle, D3DXMATRIX & sLightViewMatrix, D3DXMATRIX & sLightProjectionMatrix );

	void					End( Effect_ptr pEffectShadow );
		
	void					Render( PTModel * pcModel, Effect_ptr pEffectShadow );

	int						GetSharableID() { return iSharableID; }

	int						GetWidth() { return uWidth; }
	int						GetHeight() { return uHeight; }

	RenderTarget_ptr		GetRenderTarget() { return pRenderTargetShadow; }

private:
	std::weak_ptr<GraphicsDevice>	  pGraphicsDevice;
	int						iSharableID;

	RenderTarget_ptr		pRenderTargetShadow;
	DepthStencilBuffer_ptr	pDepthStencilShadow;

	UINT					uWidth;
	UINT					uHeight;

	void					UpdateMatrix( Effect_ptr pEffectShadow, D3DXMATRIX & sLightViewMatrix, D3DXMATRIX & sLightProjectionMatrix );
};

}