#include "stdafx.h"
#include "DXIndexBuffer.h"

namespace DX
{

IndexBuffer::IndexBuffer( LPDIRECT3DINDEXBUFFER9 lpIndexBuffer, UINT uPrimitiveSize, UINT uPrimitiveCount ) : lpIndexBuffer(lpIndexBuffer), uPrimitiveSize(uPrimitiveSize), uPrimitiveCount(uPrimitiveCount)
{
	ADDREF( lpIndexBuffer );
}

IndexBuffer::~IndexBuffer()
{
	RELEASE( lpIndexBuffer );
}

void * IndexBuffer::Lock()
{
	void * ptr;
	if( FAILED( lpIndexBuffer->Lock( 0, 0, &ptr, 0 ) ) )
		return NULL;

	return ptr;
}

void IndexBuffer::Unlock()
{
	lpIndexBuffer->Unlock();
}

}