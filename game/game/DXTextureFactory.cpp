#include "stdafx.h"
#include "DXTextureFactory.h"

#include "DX.h"

namespace DX
{

TextureFactory::TextureFactory( GraphicsDevice * pGraphicsDevice ) : pGraphicsDevice(pGraphicsDevice)
{
	vCache.reserve( 1000 );
}

void TextureFactory::OnLostDevice()
{
}

void TextureFactory::OnResetDevice()
{
}

Texture_ptr TextureFactory::MakeTexture( const std::string & strFilePath, BOOL bUse3D )
{
	//In Cache?
	for( auto pTexture : vCache )
	{
		//Not same File?
		if( !STRINGCOMPAREI( strFilePath.c_str(), pTexture->GetFile().c_str()) )
			continue;

		//Not same Use 3D?
		if( pTexture->GetUse3D() != bUse3D )
			continue;

		return pTexture;
	}

	//Make New Texture
	BOOL bTransparent = FALSE;
	LPDIRECT3DTEXTURE9 lpTexture = CreateTextureFromFile( strFilePath, bUse3D, &bTransparent );

	//New Texture (adds Reference)
	Texture_ptr pTexture = std::make_shared<Texture>( lpTexture, strFilePath, bUse3D );

	//Set Texture Transparent setting
	pTexture->SetTransparent( bTransparent );

	//Release our Reference
	RELEASE( lpTexture );

	//Cache New Texture
	vCache.push_back( pTexture );

	return pTexture;
}

Texture_ptr TextureFactory::MakeTemporaryTexture( const std::string & strFilePath, BOOL bUse3D )
{
	//Make New Texture
	BOOL bTransparent = FALSE;
	LPDIRECT3DTEXTURE9 lpTexture = CreateTextureFromFile( strFilePath, bUse3D, &bTransparent );

	//New Texture (adds Reference)
	auto pTexture = std::make_shared<Texture>( lpTexture, strFilePath, bUse3D );

	//Set Texture Transparent setting
	pTexture->SetTransparent( bTransparent );

	//Release our Reference
	RELEASE( lpTexture );

	return pTexture;
}

Texture_ptr TextureFactory::MakeTemporaryTexture( LPDIRECT3DTEXTURE9 lpTexture )
{
	//New Texture
	return std::make_shared<Texture>( lpTexture );
}

Texture_ptr TextureFactory::MakeBlankTexture()
{
	//New Blank Texture
	return MakeTexture( "game\\textures\\misc\\white.bmp" );
}

void TextureFactory::ReloadTextures()
{
	//Loop through Textures in Cache
	for( auto pTexture : vCache )
	{
		//Create Texture
		LPDIRECT3DTEXTURE9 lpTexture = CreateTextureFromFile( pTexture->GetFile(), pTexture->GetUse3D() );

		//Renew Texture (adds Reference)
		pTexture->Renew( lpTexture );

		//Release our Reference
		RELEASE( lpTexture );
	}
}

LPDIRECT3DTEXTURE9 TextureFactory::CreateTextureFromFile( const std::string & strTextureFile, BOOL bUseQualityLevel, BOOL * pbTransparent )
{
	LPDIRECT3DTEXTURE9 lpTexture = NULL;
	HRESULT hr = 0;

	//Load Texture from File...
	FILE *pFile;
	if( fopen_s( &pFile, strTextureFile.c_str(), "rb" ) == 0 )
	{
		//Get File Size
		fseek( pFile, 0, SEEK_END );
		unsigned int uFileSize = ftell( pFile );
		rewind( pFile );

		//Invalid File Size?
		if( uFileSize <= 0 )
		{
			fclose( pFile );
			return NULL;
		}

		//Create Buffer for File contents
		char * pBuffer = new char[uFileSize];

		//Read File into Buffer
		fread( pBuffer, uFileSize, 1, pFile );
		fclose( pFile );

		//Get MipMap Count
		UINT uMipLevels = 0;

		if( bUseQualityLevel )
		{
			char szFilePath[MAX_PATH] = { 0 };
			STRINGCOPY( szFilePath, strTextureFile.c_str() );

			const char * iDot = strrchr( szFilePath, '.' );

			if( iDot != NULL )
			{
				char szBaseFile[MAX_PATH], szFileExtension[MAX_PATH];

				int iLenToDot = iDot - szFilePath;

				//Get Base File
				STRINGCOPY( szBaseFile, szFilePath );
				szBaseFile[iLenToDot] = 0;

				//Get File Extension
				STRINGCOPY( szFileExtension, szBaseFile + iLenToDot + 1 );

				//Find MipMap Levels
				for( int i = 1; i <= 4; i++ )
				{
					char szBuffer[MAX_PATH];
					STRINGFORMAT( szBuffer, "%s_mm%d.%s", szBaseFile, i, szFileExtension );

					uMipLevels += PathFileExistsA( szBuffer ) ? 1 : 0;
				}
			}
		}

		//Get File Extension
		std::string strFileExtension = GetFileExtension( strTextureFile );

		if( (pBuffer[0] == 'B') && (pBuffer[1] == 'M') )
			strFileExtension = "bmp";

		//Load BMP?
		if( strFileExtension.compare( "bmp" ) == 0 )
		{
			if( pbTransparent )
				*pbTransparent = FALSE;

			//BMP (Decrypt first)
			if( DecryptBMP( pBuffer, uFileSize ) )
				lpTexture = CreateTextureFromFileInMemory( hr, pBuffer, uFileSize, uMipLevels == 0 ? D3DX_FROM_FILE : uMipLevels + 1, bUseQualityLevel ? D3DCOLOR_ARGB(0, 0, 0, 0) : D3DCOLOR_ARGB(255, 0, 0, 0), bUseQualityLevel );
		}

		//Load JPG?
		if( strFileExtension.compare( "jpg" ) == 0 )
		{
			if( pbTransparent )
				*pbTransparent = FALSE;

			//JPG (No encryption)
			lpTexture = CreateTextureFromFileInMemory( hr, pBuffer, uFileSize, uMipLevels == 0 ? D3DX_FROM_FILE : uMipLevels + 1, bUseQualityLevel ? D3DCOLOR_ARGB(0, 0, 0, 0) : D3DCOLOR_ARGB(255, 0, 0, 0), bUseQualityLevel );
		}
		
		//Load TGA?
		if( strFileExtension.compare( "tga" ) == 0 )
		{
			if( pbTransparent )
				*pbTransparent = TRUE;

			//TGA (Decrypt first)
			if( DecryptTGA( pBuffer, uFileSize ) )
				lpTexture = CreateTextureFromFileInMemory( hr, pBuffer, uFileSize, uMipLevels == 0 ? D3DX_FROM_FILE : uMipLevels + 1, D3DCOLOR_ARGB(0, 0, 0, 0), bUseQualityLevel );
		}

		//Load PNG?
		if( strFileExtension.compare( "png" ) == 0 )
		{
			if( pbTransparent )
				*pbTransparent = TRUE;

			//PNG (No encryption)
			lpTexture = CreateTextureFromFileInMemory( hr, pBuffer, uFileSize, uMipLevels == 0 ? D3DX_FROM_FILE : uMipLevels + 1, D3DCOLOR_ARGB(0, 0, 0, 0), bUseQualityLevel );
		}

		//Delete Buffer
		delete[] pBuffer;

		//Error?
		if( (FAILED( hr )) || (!lpTexture) )
		{
			//Log
//			//Logger::GetInstance().PrintEx( Logger::Release, hr, "Could not create Texture from File (%s)", strTextureFile.c_str() );
		}
	}

	return lpTexture;
}

LPDIRECT3DTEXTURE9 TextureFactory::CreateTextureFromFileInMemory( HRESULT & hOutResult, const char * pBuffer, unsigned int uBufferSize, unsigned int uMipLevels, D3DCOLOR d3dColorKey, BOOL bUseQualityLevel )
{
	UINT uWidth = D3DX_DEFAULT_NONPOW2;
	UINT uHeight = D3DX_DEFAULT_NONPOW2;
	
	D3DXIMAGE_INFO ii;
	ZeroMemory( &ii, sizeof( D3DXIMAGE_INFO ) );

	//Reduce Texture Dimensions if necessary
	if( (bUseQualityLevel) && (iQualityLevel > 0) )
	{
		hOutResult = D3DXGetImageInfoFromFileInMemory( pBuffer, uBufferSize, &ii );

		if( FAILED( hOutResult ) )
			return NULL;

		uWidth = ii.Width >> iQualityLevel;
		uHeight = ii.Height >> iQualityLevel;
	}
	
	LPDIRECT3DTEXTURE9 lpTexture = NULL;

	//Create Texture
	hOutResult = D3DXCreateTextureFromFileInMemoryEx( pGraphicsDevice->Get(), 
		pBuffer, 
		uBufferSize, 
		uWidth, 
		uHeight, 
		uMipLevels, 
		0, 
		D3DFMT_FROM_FILE, 
		DXGRAPHICENGINE_D3DPOOL, 
		D3DX_DEFAULT, 
		D3DX_DEFAULT, 
		d3dColorKey, 
		&ii, 
		NULL, 
		&lpTexture );

	if( FAILED( hOutResult ) )
		return NULL;

	return lpTexture;
}

BOOL TextureFactory::DecryptBMP( char * pBuffer, unsigned int uBufferSize )
{
	if( uBufferSize < 14 )
		return FALSE;

	if( (pBuffer[0] != 'B') || (pBuffer[1] != 'M') )
	{
		pBuffer[0] = 'B';
		pBuffer[1] = 'M';

		for( unsigned char c = 2; c < 14; c++ )
			pBuffer[c] -= (c * c);
	}

	pBuffer[2] = 0;
	pBuffer[3] = 0;
	pBuffer[4] = 0;
	pBuffer[5] = 0;

	return TRUE;
}

BOOL TextureFactory::DecryptTGA( char * pBuffer, unsigned int uBufferSize )
{
	if( uBufferSize < 18 )
		return FALSE;

	if( (pBuffer[0] == 'G') && (pBuffer[1] == '8') )
	{
		pBuffer[0] = 0;
		pBuffer[1] = 0;

		for( unsigned char c = 2; c < 18; c++ )
			pBuffer[c] -= (c * c);
	}

	return TRUE;
}

std::string TextureFactory::GetFileExtension( const std::string & strFilePath )
{
	std::string::size_type stSlashIndex;
	std::string::size_type stBackslashIndex;
	std::string::size_type stDotIndex;
	
	stSlashIndex = strFilePath.rfind('/');
	stBackslashIndex = strFilePath.rfind('\\');
	stDotIndex = strFilePath.rfind('.');
	
	if( stDotIndex != std::string::npos )
	{
		if( stSlashIndex != std::string::npos )
		{
			if( stSlashIndex > stDotIndex )
				return "";
		}

		if( stBackslashIndex != std::string::npos )
		{
			if( stBackslashIndex > stDotIndex )
				return "";
		}

		char szFileExtension[MAX_PATH] = { 0 };
		STRINGCOPY( szFileExtension, strFilePath.substr( stDotIndex + 1 ).c_str() );

		for( int i = 0, j = STRLEN( szFileExtension ); i < j; i++ )
			szFileExtension[i] = tolower( szFileExtension[i] );

		return szFileExtension;
	}

	return "";
}

}