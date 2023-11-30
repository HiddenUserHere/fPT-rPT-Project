#include "stdafx.h"
#include "DXFontFactory.h"

#include "DX.h"
#include "CSystem.h"

namespace DX
{

FontFactory::FontFactory( GraphicsDevice * pGraphicsDevice ) : pGraphicsDevice(pGraphicsDevice)
{
	vCache.reserve( 10 );

	LoadFontFiles();
}

void FontFactory::LoadFontFiles()
{
	auto vFiles = SYSTEM->ListFilesInDirectory( "game\\fonts\\", "ttf" );

	for( auto vFile : vFiles )
		LoadFontFile( vFile.c_str() );
}

void FontFactory::LoadFontFile( const std::string & strFontFile )
{
	if( AddFontResourceEx( strFontFile.c_str(), FR_PRIVATE, 0 ) == 0 )
	{
		//Debug Log
		WRITEDBG( "Failed to Load Font File: %s", strFontFile.c_str() );
	}
}

void FontFactory::OnLostDevice()
{
	for( auto pFont : vCache )
		pFont->OnLostDevice();
}

void FontFactory::OnResetDevice()
{
	for( auto pFont : vCache )
		pFont->OnResetDevice();
}

Font_ptr FontFactory::MakeFont( Sprite_ptr pSprite, std::string strFontName, int Height, int Width, BOOL bBold, BOOL bItalic )
{
	//New Font Description
	D3DXFONT_DESC d3dxFontDesc;
	ZeroMemory( &d3dxFontDesc, sizeof( D3DXFONT_DESC ) );
	
	//Build Font Description
	d3dxFontDesc.Height				= Height;
	d3dxFontDesc.Width				= Width;
	d3dxFontDesc.Weight				= bBold ? FW_BOLD : FW_NORMAL;
	d3dxFontDesc.MipLevels			= 1;
	d3dxFontDesc.Italic				= bItalic;
	d3dxFontDesc.CharSet			= DEFAULT_CHARSET;
	d3dxFontDesc.OutputPrecision	= OUT_DEFAULT_PRECIS;
	d3dxFontDesc.Quality			= CLEARTYPE_QUALITY;
	d3dxFontDesc.PitchAndFamily		= VARIABLE_PITCH;
	STRINGCOPY( d3dxFontDesc.FaceName, strFontName.c_str() );

	//Font already loaded before? Then return the same one and share that
	for( auto pFont : vCache )
	{
		if( memcmp( &d3dxFontDesc, &(pFont->GetDesc()), sizeof( D3DXFONT_DESC ) ) == 0 )
			return pFont;
	}

	//Load Font
	LPD3DXFONT lpD3DXFont;
	if( FAILED( D3DXCreateFontIndirect( pGraphicsDevice->Get(), &d3dxFontDesc, &lpD3DXFont ) ) )
		return nullptr;

	//Preload Characters
	lpD3DXFont->PreloadCharacters( 'A', 'Z' );
	lpD3DXFont->PreloadCharacters( 'a', 'z' );
	lpD3DXFont->PreloadCharacters( '0', '9' );

	//New Font
	auto pFont = std::make_shared<DXFont>( pSprite, lpD3DXFont, d3dxFontDesc );

	//Remember Font
	vCache.push_back( pFont );

	//Release Font (New Font took possession of reference)
	RELEASE( lpD3DXFont );

	return pFont;
}

}