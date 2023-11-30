#include "stdafx.h"
#include "DXVertexDeclaration.h"

namespace DX
{

VertexDeclaration::VertexDeclaration( LPDIRECT3DVERTEXDECLARATION9 lpVertexDeclaration ) : lpVertexDeclaration(lpVertexDeclaration)
{
	ADDREF( lpVertexDeclaration );
}

VertexDeclaration::~VertexDeclaration()
{
	RELEASE( lpVertexDeclaration );
}

}