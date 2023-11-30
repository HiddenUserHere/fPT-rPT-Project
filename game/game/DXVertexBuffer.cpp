#include "stdafx.h"
#include "DXVertexBuffer.h"

namespace DX
{

VertexBuffer::VertexBuffer( LPDIRECT3DVERTEXBUFFER9 lpVertexBuffer, UINT uElementSize, UINT uElementCount, BOOL bDynamic ) : lpVertexBuffer(lpVertexBuffer), uElementSize(uElementSize), uElementCount(uElementCount), bDynamic(bDynamic)
{
	ADDREF( lpVertexBuffer );
}

VertexBuffer::~VertexBuffer()
{
	RELEASE( lpVertexBuffer );
}

void * VertexBuffer::Lock()
{
	void * ptr;
	if( FAILED( lpVertexBuffer->Lock( 0, 0, &ptr, bDynamic ? D3DLOCK_DISCARD | D3DLOCK_NOOVERWRITE : 0 ) ) )
		return NULL;

	return ptr;
}

void VertexBuffer::Unlock()
{
	lpVertexBuffer->Unlock();
}

}