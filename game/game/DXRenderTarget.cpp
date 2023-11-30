#include "stdafx.h"
#include "DXRenderTarget.h"

#include "DXGraphicsDevice.h"

namespace DX
{

RenderTarget::RenderTarget( std::weak_ptr<GraphicsDevice> pGraphicsDevice, int iSharableID, UINT uSizeShiftRight ) : pGraphicsDevice( pGraphicsDevice ), iSharableID( iSharableID ), uSizeShiftRight( uSizeShiftRight ), uWidth( 0 ), uHeight( 0 )
{
	lpTexture	= NULL;
	lpSurface	= NULL;
}

RenderTarget::RenderTarget( std::weak_ptr<GraphicsDevice> pGraphicsDevice, int iSharableID, UINT uWidth, UINT uHeight ) : pGraphicsDevice( pGraphicsDevice ), iSharableID( iSharableID ), uSizeShiftRight( 0 ), uWidth( uWidth ), uHeight( uHeight )
{
	lpTexture = NULL;
	lpSurface = NULL;
}

RenderTarget::~RenderTarget()
{
	RELEASE( lpSurface );
	RELEASE( lpTexture );
}

void RenderTarget::OnLostDevice()
{
	RELEASE( lpSurface );
	RELEASE( lpTexture );
}

void RenderTarget::OnResetDevice()
{
	pGraphicsDevice.lock()->BuildRenderTarget( shared_from_this() );
}

UINT RenderTarget::GetWidth()
{
	if ( uWidth == 0 )
		return pGraphicsDevice.lock()->GetBackBufferWidth();

	return uWidth;
}

UINT RenderTarget::GetHeight()
{
	if ( uHeight == 0 )
		return pGraphicsDevice.lock()->GetBackBufferHeight();

	return uHeight;
}

}