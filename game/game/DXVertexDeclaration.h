#pragma once

namespace DX
{

typedef std::shared_ptr<class VertexDeclaration> VertexDeclaration_ptr;

class VertexDeclaration
{
public:
									  VertexDeclaration( LPDIRECT3DVERTEXDECLARATION9 lpVertexDeclaration );
	virtual							 ~VertexDeclaration();

	LPDIRECT3DVERTEXDECLARATION9	  Get() { return lpVertexDeclaration; }

private:
	LPDIRECT3DVERTEXDECLARATION9	  lpVertexDeclaration;
};

}