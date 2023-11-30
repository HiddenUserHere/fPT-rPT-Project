#pragma once

#define D3DFVF_GEOMETRY		(D3DFVF_XYZRHW | D3DFVF_TEX0)
#define D3DFVF_INSTANCE		(D3DFVF_DIFFUSE | D3DFVF_SPECULAR)

struct VertexGeometry
{
	float			fX;
	float			fY;
	float			fZ;
	float			fRHW;

	float			fU;
	float			fV;
};

struct VertexInstance
{
	D3DCOLOR	Color;
	D3DCOLOR	Specular;
};

class DXGeometryInstance
{
public:
	DXGeometryInstance();
	virtual ~DXGeometryInstance();

	BOOL					Init();
	void					Shutdown();

	void					HandleVertices( void * pVertices, int iPrimitiveCount, UINT uVertexSize );

	void					HandleIndices( WORD * psaIndices, int iIndicesCount );

	void					Render( D3DPRIMITIVETYPE d3dPrimitiveType );

private:
	LPDIRECT3DINDEXBUFFER9			lpIBRenderer;
	
	LPDIRECT3DVERTEXBUFFER9			lpVBRendererModelData;
	LPDIRECT3DVERTEXBUFFER9			lpVBRendererInstanceData;
	LPDIRECT3DVERTEXDECLARATION9	lpVDModelData;
	LPDIRECT3DVERTEXDECLARATION9	lpVDInstanceData;

	int								iPrimitiveCount;
	UINT							uVertexSize;

	int								iIndicesCount;

	static VertexGeometry			saGeometry[16384];
	static VertexInstance			saInstance[16384];

};

