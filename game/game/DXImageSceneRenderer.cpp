#include "stdafx.h"
#include "DXImageSceneRenderer.h"

namespace DX
{

DXImageSceneRenderer::DXImageSceneRenderer( std::string _strFileName )
{
	strFileName = _strFileName;

	ZeroMemory( &sHeader, sizeof( DXImageSceneHeader ) );

	iBlockWheel = 0;

	iFramesPerSecond = 30;

	iWidth = 0;
	iHeight = 0;

	D3DXCreateSprite( DX::Graphic::GetDevice(), &lpSprite );
}


DXImageSceneRenderer::~DXImageSceneRenderer()
{
	for ( std::vector<LPDIRECT3DTEXTURE9>::iterator it = vTextures.begin(); it != vTextures.end(); it++ )
	{
		LPDIRECT3DTEXTURE9 p = (*it);

		RELEASE( p );
	}

	vTextures.clear();

	RELEASE( lpSprite );
}

int DXImageSceneRenderer::GetFileSize( FILE * fp )
{
	fseek( fp, 0, SEEK_END );
	int iSize = ftell( fp );
	return iSize;
}

void DXImageSceneRenderer::SetNextTexture( DXImageSceneData * ps )
{
	LPDIRECT3DTEXTURE9 lpTexture;

	D3DXIMAGE_INFO SrcInfo;

	// Use a magenta colourkey
	D3DCOLOR colorkey = 0xFFFF00FF;

	// Get Image Info
	D3DXGetImageInfoFromFileInMemory( &ps->baData, ps->iSize, &SrcInfo );

	if ( SrcInfo.ImageFileFormat == D3DXIFF_BMP )
		colorkey = 0xFF000000;


	// Load image from file.
	if ( FAILED( D3DXCreateTextureFromFileInMemoryEx( DX::Graphic::GetDevice(), &ps->baData, ps->iSize, SrcInfo.Width, SrcInfo.Height, 1, 0, D3DFMT_A8B8G8R8, DXGRAPHICENGINE_D3DPOOL, D3DX_FILTER_NONE, D3DX_DEFAULT, colorkey, &SrcInfo, NULL, &lpTexture ) ) )
	{

	}

	if ( (iWidth == 0) && (iHeight == 0) )
	{
		iWidth	= SrcInfo.Width;
		iHeight = SrcInfo.Height;
	}

//	vTextures.push_back( lpTexture );
}


BOOL DXImageSceneRenderer::Create( const char * pszDir, const char * pszPrefix, int iCount )
{
	FILE * pFileToRead	= NULL;
	FILE * pFileToWrite = NULL;


	char szFilePath[MAX_PATH]	= { 0 };
	char szFileFormat[MAX_PATH] = { 0 };

	STRINGFORMAT( szFileFormat, "%s\\%s", pszDir, pszPrefix );

	//Try open file to write
	fopen_s( &pFileToWrite, strFileName.c_str(), "wb" );

	if ( pFileToWrite )
	{
		int iNext = sizeof( DXImageSceneHeader );

		int iBlockCount = 0;

		for ( int i = 0; i < iCount; i++ )
		{
			STRINGFORMAT( szFilePath, szFileFormat, i+1 );

			fopen_s( &pFileToRead, szFilePath, "rb" );

			if ( pFileToRead )
			{
				//Get File size
				int iSize = GetFileSize( pFileToRead );
				fseek( pFileToRead, 0, SEEK_SET );

				BYTE * pbData = new BYTE[iSize];

				fread( pbData, iSize, 1, pFileToRead );

				fseek( pFileToWrite, iNext, SEEK_SET );
				iSize += 4;
				fwrite( &iSize, sizeof( int ), 1, pFileToWrite );
				iSize -= 4;


				fseek( pFileToWrite, iNext + sizeof( int ), SEEK_SET );
				fwrite( pbData, iSize, 1, pFileToWrite );

				iNext += (iSize + sizeof( int ));

				delete pbData;

				fclose( pFileToRead );

				iBlockCount++;
			}
		}

		//Header
		STRINGCOPY( sHeader.szHeader, "Scene Data 0.1" );
		sHeader.iFileSize	= iNext;
		sHeader.iBlockCount = iBlockCount;
		fseek( pFileToWrite, 0, SEEK_SET );
		fwrite( &sHeader, sizeof( DXImageSceneHeader ), 1, pFileToWrite );

		fclose( pFileToWrite );
		return TRUE;
	}

	return FALSE;
}

BOOL DXImageSceneRenderer::Load()
{
	FILE * pFile = NULL;

	fopen_s( &pFile, strFileName.c_str(), "rb" );

	if ( pFile )
	{
		int iSize = GetFileSize( pFile ) - sizeof( DXImageSceneHeader );
		fseek( pFile, 0, SEEK_SET );

		fread( &sHeader, sizeof( DXImageSceneHeader ), 1, pFile );

		if ( STRINGCOMPARE( sHeader.szHeader, "Scene Data 0.1" ) == FALSE )
		{
			fclose( pFile );
			return FALSE;
		}

		BYTE * pbDataFile = new BYTE[iSize];

		fseek( pFile, sizeof( DXImageSceneHeader ), SEEK_SET );
		fread( pbDataFile, iSize, 1, pFile );

		int iCurrentBlockSize = 0;

		for ( int i = 0; i < sHeader.iBlockCount; i++ )
		{
			DXImageSceneData * ps = (DXImageSceneData *)(&pbDataFile[iCurrentBlockSize]);
			iCurrentBlockSize += ps->iSize;

			SetNextTexture( ps );
		}

		delete pbDataFile;

		fclose( pFile );

		return TRUE;
	}

	return FALSE;
}

void DXImageSceneRenderer::Update( float fTime )
{

	static DWORD dwTest = 0;

	if ( (TICKCOUNT - dwTest) >= 33 )
	{
		dwTest = TICKCOUNT;
		iBlockWheel++;

		if ( iBlockWheel >= sHeader.iBlockCount )
		{
			iBlockWheel = 0;
		}
	}
}

void DXImageSceneRenderer::Render()
{
	//Sprite Rectangle
	RECT rSourceRect;
	rSourceRect.left	= 0;
	rSourceRect.top		= 0;
	rSourceRect.right	= rSourceRect.left + iWidth;
	rSourceRect.bottom	= rSourceRect.top + iHeight;

	LPDIRECT3DTEXTURE9 lp = vTextures[iBlockWheel];

	//Scale Vetor
	D3DXVECTOR2 scale( 1.0f, 1.0f );

	scale.x = (float)RESOLUTION_WIDTH / (float)iWidth;

	scale.y = (float)RESOLUTION_HEIGHT / (float)iHeight;

	D3DXVECTOR2 trans( (float)0, (float)0 );
	D3DXVECTOR2 center( (float)((RESOLUTION_WIDTH) / 2), (float)((RESOLUTION_HEIGHT) / 2) );
	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D( &matrix, NULL, 0, &scale, &center, 0.0f, &trans );


	lpSprite->Begin( D3DXSPRITE_ALPHABLEND );
	lpSprite->SetTransform( &matrix );
	lpSprite->Draw( vTextures[iBlockWheel], &rSourceRect, NULL, NULL, -1 );
	lpSprite->End();
}

void DXImageSceneRenderer::Close()
{
}

};