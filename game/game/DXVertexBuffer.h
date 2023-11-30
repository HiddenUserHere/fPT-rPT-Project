#pragma once

namespace DX
{

typedef std::shared_ptr<class VertexBuffer> VertexBuffer_ptr;

class VertexBuffer
{
public:
								  VertexBuffer( LPDIRECT3DVERTEXBUFFER9 lpVertexBuffer, UINT uElementSize, UINT uElementCount, BOOL bDynamic );
	virtual						 ~VertexBuffer();

	void						* Lock();
	void						  Unlock();

	LPDIRECT3DVERTEXBUFFER9		  & Get() { return lpVertexBuffer; }

	UINT						  GetElementSize() { return uElementSize; }
	UINT						  GetElementCount() { return uElementCount; }

private:
	LPDIRECT3DVERTEXBUFFER9		  lpVertexBuffer;

	UINT						  uElementSize;
	UINT						  uElementCount;

	BOOL						  bDynamic;
};

}