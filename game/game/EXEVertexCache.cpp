#include "stdafx.h"
#include "EXEVertexCache.h"

EXEVertexCacheData EXEVertexCache::saVertices[TOTAL_CACHE_VERTICES];

EXEVertexCache::EXEVertexCache()
{
	for ( int i = 0; i < NUM_VERTEXMODEL_CACHE; i++ )
	{
	//	psaCache[i] = new EXEVertexCacheData;
	//	ClearCache( psaCache[i] );
	}
}


EXEVertexCache::~EXEVertexCache()
{
}

void EXEVertexCache::Update()
{
	return;

	for ( int i = 0; i < TOTAL_CACHE_VERTICES; i++ )
	{
		EXEVertexCacheData * ps = EXEVertexCache::saVertices + i;

		if ( (TICKCOUNT - ps->dwLastUpdateTime) > 1000 )
		{
			ps->x = 0;
			ps->y = 0;
			ps->z = 0;
			ps->iMaterialID = -1;
		}
	}
}

D3DVALUE * EXEVertexCache::GetVertex( D3DVALUE * psData, int iMaterialID, int & iCount )
{
	float x = psData[0];
	float y = psData[1];
	float z = psData[2];

	if ( iCount == 0 )
	{
		for ( int i = 0; i < TOTAL_CACHE_VERTICES; i++ )
		{
			EXEVertexCacheData * ps = EXEVertexCache::saVertices + i;

			if ( (ps->iMaterialID == iMaterialID) )
			{
				ps->dwLastUpdateTime = TICKCOUNT;
				iCount = ps->iCount;
				return ps->vertices;
			}
		}
	}
	else if ( iCount > 0 && iCount < 600 )
	{
		for ( int i = 0; i < TOTAL_CACHE_VERTICES; i++ )
		{
			EXEVertexCacheData * ps = EXEVertexCache::saVertices + i;

			if ( ps->iMaterialID == -1 )
			{
				CopyMemory( ps->vertices, psData, (64 * (iCount * 3)) );
				ps->x = x;
				ps->y = y;
				ps->z = z;
				ps->iMaterialID = iMaterialID;
				ps->dwLastUpdateTime = TICKCOUNT;
				ps->iCount = iCount;
				return ps->vertices;
			}
		}
	}

	return (D3DVALUE*)0x2E45830;
}

BOOL EXEMeshCache::Build( PTModel * pcModel )
{
	

	return TRUE;
}

void EXEMeshCache::Render()
{

	if ( FAILED( GRAPHICENGINE->GetDevice()->SetStreamSource( 0, pcVertexBuffer->Get(), 0, pcVertexBuffer->GetElementSize() ) ) )
	{
		return;
	}
	if ( FAILED( GRAPHICENGINE->GetDevice()->SetStreamSource( 1, pcTextureCoordBuffer->Get(), 0, pcTextureCoordBuffer->GetElementSize() ) ) )
	{
		return;
	}
	if ( FAILED( GRAPHICENGINE->GetDevice()->SetIndices( pcIndexBuffer->Get() ) ) )
	{
		return;
	}

	if ( FAILED( GRAPHICENGINE->GetDevice()->SetFVF( NULL ) ) )
	{
		return;
	}

	if ( FAILED( GRAPHICENGINE->GetDevice()->SetVertexDeclaration( pcVertexDeclaration->Get() ) ) )
	{
		return;
	}

	if ( FAILED( GRAPHICENGINE->GetDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, pcVertexBuffer->GetElementCount(), 0, pcIndexBuffer->GetPrimitiveCount() / 3 ) ) )
	{
		return;
	}
}

BOOL PTModelCache::Build()
{
	int iMeshes = 0;


	return TRUE;
}

void PTModelCache::SetWorld( EXEMatrixI & sMatrix )
{
	for ( int i = 0; i < 4; i++ )
		for ( int j = 0; j < 4; j++ )
			sWorld.m[i][j] = (float)sMatrix.i[i][j];
}

void PTModelCache::Render()
{
	
}
