#pragma once

#include "DXMaterial.h"
#include "DXVertexDeclaration.h"
#include "DXIndexBuffer.h"

namespace DX
{

typedef std::shared_ptr<class MeshPart> MeshPart_ptr;

class MeshPart
{
public:
											  MeshPart();
	virtual									 ~MeshPart();

	void									  SetVertexDeclaration( VertexDeclaration_ptr _pVertexDeclaration ) { pVertexDeclaration = _pVertexDeclaration; }
	VertexDeclaration_ptr					  GetVertexDeclaration() { return pVertexDeclaration; }

	void									  SetIndexBuffer( IndexBuffer_ptr _pIndexBuffer ) { pIndexBuffer = _pIndexBuffer; }
	IndexBuffer_ptr							  GetIndexBuffer() { return pIndexBuffer; }

	void									  SetMaterial( Material_ptr pNewMaterial ) { pMaterial = pNewMaterial; }
	Material_ptr							  GetMaterial() { return pMaterial; }

	void									  SetIndices( const std::vector<unsigned short> & v ) { vIndices = v; }
	const std::vector<unsigned short>		& GetIndices() const { return vIndices; }

private:
	VertexDeclaration_ptr					  pVertexDeclaration;
	IndexBuffer_ptr							  pIndexBuffer;

	Material_ptr							  pMaterial;
	
	std::vector<unsigned short>				  vIndices;
};

}