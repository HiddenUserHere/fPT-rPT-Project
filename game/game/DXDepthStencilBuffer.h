#pragma once

namespace DX
{
	
typedef std::shared_ptr<class DepthStencilBuffer> DepthStencilBuffer_ptr;

class DepthStencilBuffer : public std::enable_shared_from_this<DepthStencilBuffer>
{
	friend class GraphicsDevice;

public:
									  DepthStencilBuffer( std::weak_ptr<GraphicsDevice> pGraphicsDevice, int iSharableID, UINT uSizeShiftRight );
									  DepthStencilBuffer( std::weak_ptr<GraphicsDevice> pGraphicsDevice, int iSharableID, UINT uWidth, UINT uHeight );
	virtual							 ~DepthStencilBuffer();

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

	LPDIRECT3DSURFACE9				  lpSurface;

	UINT							  uWidth;
	UINT							  uHeight;
};

}