#include "stdafx.h"
#include "DXGeometryInstance.h"

VertexGeometry DXGeometryInstance::saGeometry[16384];

VertexInstance DXGeometryInstance::saInstance[16384];

/*
D3DVERTEXELEMENT9 g_VertexElemHardware[] =
{
	{ 0, 0,     D3DDECLTYPE_FLOAT3,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_POSITION,  0 },
	{ 0, 3 * 4, D3DDECLTYPE_FLOAT3,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_NORMAL,    0 },
	{ 0, 6 * 4, D3DDECLTYPE_FLOAT2,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_TEXCOORD,  0 },
	{ 1, 0,     D3DDECLTYPE_D3DCOLOR,   D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_COLOR,     0 },
	{ 1, 4,     D3DDECLTYPE_D3DCOLOR,   D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_COLOR,     1 },
	D3DDECL_END()
};
*/

D3DVERTEXELEMENT9 g_VertexElementGeometry[] =
{
	{ 0, 0,     D3DDECLTYPE_FLOAT4,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_POSITIONT,  0 },
	{ 0, 4*4,     D3DDECLTYPE_FLOAT2,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_TEXCOORD,  0 },
	D3DDECL_END()
};

D3DVERTEXELEMENT9 g_VertexElementInstance[] =
{
	{ 1, 0,     D3DDECLTYPE_D3DCOLOR,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_COLOR,  0 },
	{ 1, 4,     D3DDECLTYPE_D3DCOLOR,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_COLOR,  1 },
	D3DDECL_END()
};

DXGeometryInstance::DXGeometryInstance()
{
	lpIBRenderer				= NULL;
	lpVBRendererModelData		= NULL;
	lpVBRendererInstanceData	= NULL;
}


DXGeometryInstance::~DXGeometryInstance()
{
}

BOOL DXGeometryInstance::Init()
{
	HRESULT hr;

	LPDIRECT3DDEVICE9 lp = GRAPHICENGINE->GetDevice();

	if ( (hr = lp->CreateVertexBuffer( sizeof( VertexGeometry ) * 16384, 0, 1220, DXGRAPHICENGINE_D3DPOOL, &lpVBRendererModelData, NULL )) != D3D_OK )
	{
		return FALSE;
	}

	if ( (hr = lp->CreateVertexBuffer( sizeof( VertexInstance ) * 16384, 0, 1220, DXGRAPHICENGINE_D3DPOOL, &lpVBRendererInstanceData, NULL )) != D3D_OK )
	{
		return FALSE;
	}

	if ( (hr = lp->CreateIndexBuffer( 3 * 18000 * sizeof( WORD ), 0, D3DFMT_INDEX16, DXGRAPHICENGINE_D3DPOOL, &lpIBRenderer, NULL )) != D3D_OK )
	{
		return FALSE;
	}

	//Vertex Declaration
	if ( (hr = lp->CreateVertexDeclaration( g_VertexElementGeometry, &lpVDModelData )) != S_OK )
	{
		return FALSE;
	}

	//Vertex Declaration
	if ( (hr = lp->CreateVertexDeclaration( g_VertexElementInstance, &lpVDInstanceData )) != S_OK )
	{
		return FALSE;
	}

	return TRUE;
}

void DXGeometryInstance::Shutdown()
{
	if ( lpIBRenderer )
		lpIBRenderer->Release();

	if ( lpVBRendererModelData )
		lpVBRendererModelData->Release();

	if ( lpVBRendererInstanceData )
		lpVBRendererInstanceData->Release();
}

void DXGeometryInstance::HandleVertices( void * pVertices, int iPrimitiveCount, UINT uVertexSize )
{
	void * pVoid;

	CONST DWORD dwLockFlags = D3DLOCK_DISCARD;

	this->iPrimitiveCount	= iPrimitiveCount;
	this->uVertexSize		= uVertexSize;

	for ( int i = 0; i < (iPrimitiveCount * 3); i++ )
	{
		EXEDrawVertex * p		= ((EXEDrawVertex*)pVertices) + i;

		saGeometry[i].fX		= p->x;
		saGeometry[i].fY		= p->y;
		saGeometry[i].fZ		= p->z;
		saGeometry[i].fRHW		= p->rhw;
		saGeometry[i].fU		= p->u;
		saGeometry[i].fV		= p->v;

		saInstance[i].Color		= p->d3dColor;
		saInstance[i].Specular	= p->d3dSpecular;
	}

	lpVBRendererModelData->Lock( 0, 0, &pVoid, dwLockFlags );

	CopyMemory( pVoid, saGeometry, sizeof( VertexGeometry ) * (iPrimitiveCount * 3) );

	lpVBRendererModelData->Unlock();
	
	lpVBRendererInstanceData->Lock( 0, 0, &pVoid, dwLockFlags );

	CopyMemory( pVoid, saInstance, sizeof( VertexInstance ) * (iPrimitiveCount * 3) );

	lpVBRendererInstanceData->Unlock();
}

void DXGeometryInstance::HandleIndices( WORD * psaIndices, int iIndicesCount )
{
	void * pVoid;

	CONST DWORD dwLockFlags = D3DLOCK_DISCARD;

	this->iIndicesCount		= iIndicesCount;

	lpIBRenderer->Lock( 0, 0, &pVoid, dwLockFlags );
	
	CopyMemory( pVoid, psaIndices, sizeof( WORD ) * iIndicesCount );
	
	lpIBRenderer->Unlock();
}

void DXGeometryInstance::Render( D3DPRIMITIVETYPE d3dPrimitiveType )
{
	LPDIRECT3DDEVICE9 lpD3DDev = GRAPHICENGINE->GetDevice();

	//Stream Sources
	{
		//Stream 0, model data...
		{
			//Vertex Buffer
			if ( FAILED( lpD3DDev->SetStreamSource( 0, lpVBRendererModelData, 0, sizeof( VertexGeometry ) ) ) )
			{
				//Error
				WRITEERR( "ERROR - VERTEX SetStreamSource(0)" );
			}

			//Set Indices Count
			if ( FAILED( lpD3DDev->SetStreamSourceFreq( 0, D3DSTREAMSOURCE_INDEXEDDATA | iIndicesCount ) ) )
			{
				//Error
				WRITEERR( "ERROR - VERTEX SetStreamSource(0)" );
			}
		}

		//Stream 1, instance data...
		{
			//Vertex Buffer
			if ( FAILED( lpD3DDev->SetStreamSource( 1, lpVBRendererInstanceData, 0, sizeof( VertexInstance ) ) ) )
			{
				//Error
				WRITEERR( "ERROR - VERTEX SetStreamSource(1)" );
			}

			//Set Instances, 1 = Triangle per drawn timer
			if ( FAILED( lpD3DDev->SetStreamSourceFreq( 1, D3DSTREAMSOURCE_INSTANCEDATA | 1 ) ) )
			{
				//Error
				WRITEERR( "ERROR - VERTEX SetStreamSource(1)" );
			}
		}
	}
	
	if ( FAILED( lpD3DDev->SetIndices( lpIBRenderer ) ) )
	{
		WRITEDBG( "ERROR - SetIndices" );
	}

	//Vertex Declaration
	if ( FAILED( lpD3DDev->SetVertexDeclaration( lpVDModelData ) ) )
	{
		//Error
		WRITEERR( "ERROR - VERTEX DECLARATION" );
	}

	//Draw
	lpD3DDev->DrawIndexedPrimitive( d3dPrimitiveType, 0, 0, (iPrimitiveCount * 3), 0, iPrimitiveCount );

	//Vertex Declaration
	if ( FAILED( lpD3DDev->SetVertexDeclaration( lpVDInstanceData ) ) )
	{
		//Error
		WRITEERR( "ERROR - VERTEX DECLARATION" );
	}

	//Draw
	lpD3DDev->DrawIndexedPrimitive( d3dPrimitiveType, 0, 0, (iPrimitiveCount * 3), 0, iPrimitiveCount );

	//Reset
	lpD3DDev->SetStreamSourceFreq( 0, 1 );
	lpD3DDev->SetStreamSourceFreq( 1, 1 );
	lpD3DDev->SetVertexDeclaration( NULL );
}
