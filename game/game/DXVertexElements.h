#pragma once

#include <vector>

namespace DX
{

typedef std::shared_ptr<class VertexElements> VertexElements_ptr;

class VertexElements
{
public:
	void								  AddElement( WORD Stream, WORD Offset, BYTE Type, BYTE Method, BYTE Usage, BYTE UsageIndex );
	std::vector<D3DVERTEXELEMENT9>		  GetElements();

private:
	std::vector<D3DVERTEXELEMENT9>		  vElements;
};

}