#pragma once

#include "DXTexture.h"

namespace DX
{

class GraphicsDevice;

typedef std::shared_ptr<class TextureFactory> TextureFactory_ptr;

class TextureFactory
{
public:
								  TextureFactory( GraphicsDevice * pGraphicsDevice );

	void						  OnLostDevice();
	void						  OnResetDevice();

	Texture_ptr					  MakeTexture( const std::string & strFilePath, BOOL bUse3D = FALSE );

	Texture_ptr					  MakeTemporaryTexture( const std::string & strFilePath, BOOL bUse3D = FALSE );
	Texture_ptr					  MakeTemporaryTexture( LPDIRECT3DTEXTURE9 lpTexture );

	Texture_ptr					  MakeBlankTexture();

	void						  ReloadTextures();

	void						  SetQuality( int _iQualityLevel ) { iQualityLevel = _iQualityLevel; }
	
	LPDIRECT3DTEXTURE9			  CreateTextureFromFile( const std::string & strTextureFile, BOOL bUseQualityLevel, BOOL * pbTransparent = NULL );

private:
	LPDIRECT3DTEXTURE9			  CreateTextureFromFileInMemory( HRESULT & hOutResult, const char * pBuffer, unsigned int uBufferSize, unsigned int uMipLevels, D3DCOLOR d3dColorKey, BOOL bUseQualityLevel );

	BOOL						  DecryptBMP( char * pBuffer, unsigned int uBufferSize );
	BOOL						  DecryptTGA( char * pBuffer, unsigned int uBufferSize );

	std::string					  GetFileExtension( const std::string & strFilePath );

private:
	GraphicsDevice				* pGraphicsDevice;

	std::vector<Texture_ptr>	  vCache;

	int							  iQualityLevel = 0;
};

}