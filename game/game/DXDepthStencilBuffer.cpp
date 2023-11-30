#include "stdafx.h"
#include "DXDepthStencilBuffer.h"

#include "DXGraphicsDevice.h"

namespace DX
{

DepthStencilBuffer::DepthStencilBuffer( std::weak_ptr<GraphicsDevice> pGraphicsDevice, int iSharableID, UINT uSizeShiftRight ) : pGraphicsDevice(pGraphicsDevice), iSharableID(iSharableID), uSizeShiftRight(uSizeShiftRight), uWidth( 0 ), uHeight( 0 )
{
	lpSurface	= NULL;
}

DepthStencilBuffer::DepthStencilBuffer( std::weak_ptr<GraphicsDevice> pGraphicsDevice, int iSharableID, UINT uWidth, UINT uHeight ) : pGraphicsDevice( pGraphicsDevice ), iSharableID( iSharableID ), uSizeShiftRight( 0 ), uWidth( uWidth ), uHeight( uHeight )
{
}

DepthStencilBuffer::~DepthStencilBuffer()
{
	RELEASE( lpSurface );
}

void DepthStencilBuffer::OnLostDevice()
{
	RELEASE( lpSurface );
}

void DepthStencilBuffer::OnResetDevice()
{
	pGraphicsDevice.lock()->BuildDepthStencilBuffer( shared_from_this() );
}

UINT DepthStencilBuffer::GetWidth()
{
	if ( uWidth == 0 )
		return pGraphicsDevice.lock()->GetInfo().uDepthStencilBufferWidth;

	return uWidth;
}

UINT DepthStencilBuffer::GetHeight()
{
	if ( uHeight == 0 )
		return pGraphicsDevice.lock()->GetInfo().uDepthStencilBufferHeight;

	return uHeight;
}

}