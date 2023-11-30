#pragma once

namespace DX
{
	
typedef std::shared_ptr<class RenderTarget> RenderTarget_ptr;

class RenderTarget : public std::enable_shared_from_this<RenderTarget>
{
	friend class GraphicsDevice;

public:
									  RenderTarget( std::weak_ptr<GraphicsDevice> pGraphicsDevice, int iSharableID, UINT uSizeShiftRight );
									  RenderTarget( std::weak_ptr<GraphicsDevice> pGraphicsDevice, int iSharableID, UINT uWidth, UINT uHeight );
	virtual							 ~RenderTarget();

	LPDIRECT3DTEXTURE9				  GetTexture() { return lpTexture; }
	LPDIRECT3DSURFACE9				  GetSurface() { return lpSurface; }

	void							  OnLostDevice();
	void							  OnResetDevice();

	BOOL							  GetSharableID() { return iSharableID; }
	UINT							  GetSizeShiftRight() { return uSizeShiftRight; }

	UINT							  GetWidth();
	UINT							  GetHeight();

private:
	std::weak_ptr<GraphicsDevice>	  pGraphicsDevice;
	int								  iSharableID;
	UINT							  uSizeShiftRight;

	LPDIRECT3DTEXTURE9				  lpTexture;
	LPDIRECT3DSURFACE9				  lpSurface;

	UINT							  uWidth;
	UINT							  uHeight;
};

}