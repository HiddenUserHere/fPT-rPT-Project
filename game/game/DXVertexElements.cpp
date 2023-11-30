#include "stdafx.h"
#include "DXVertexElements.h"

namespace DX
{

void VertexElements::AddElement( WORD Stream, WORD Offset, BYTE Type, BYTE Method, BYTE Usage, BYTE UsageIndex )
{
	D3DVERTEXELEMENT9 v;
	v.Stream		= Stream;
	v.Offset		= Offset;
	v.Type			= Type;
	v.Method		= Method;
	v.Usage			= Usage;
	v.UsageIndex	= UsageIndex;
	vElements.push_back( v );
}

std::vector<D3DVERTEXELEMENT9> VertexElements::GetElements()
{
	std::vector<D3DVERTEXELEMENT9> v;

	for( const auto c : vElements )
		v.push_back( c );
	v.push_back( D3DDECL_END() );

	return v;
}

}