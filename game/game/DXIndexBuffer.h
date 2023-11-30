#pragma once

namespace DX
{

typedef std::shared_ptr<class IndexBuffer> IndexBuffer_ptr;

class IndexBuffer
{
public:
								  IndexBuffer( LPDIRECT3DINDEXBUFFER9 lpIndexBuffer, UINT uPrimitiveSize, UINT uPrimitiveCount );
	virtual						 ~IndexBuffer();

	void						* Lock();
	void						  Unlock();

	LPDIRECT3DINDEXBUFFER9		  Get() { return lpIndexBuffer; }

	UINT						  GetPrimitiveSize() { return uPrimitiveSize; }
	UINT						  GetPrimitiveCount() { return uPrimitiveCount; }

private:
	LPDIRECT3DINDEXBUFFER9		  lpIndexBuffer;

	UINT						  uPrimitiveSize;
	UINT						  uPrimitiveCount;
};

}