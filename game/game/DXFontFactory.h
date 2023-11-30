#pragma once

#include "DXFont.h"

namespace DX
{

class GraphicsDevice;

typedef std::shared_ptr<class FontFactory> FontFactory_ptr;

class FontFactory
{
public:
								  FontFactory( GraphicsDevice * pGraphicsDevice );

	void						  LoadFontFiles();
	void						  LoadFontFile( const std::string & strFontFile );

	void						  OnLostDevice();
	void						  OnResetDevice();

	Font_ptr					  MakeFont( Sprite_ptr pSprite, std::string strFontName, int Height, int Width, BOOL bBold, BOOL bItalic );

private:
	GraphicsDevice				* pGraphicsDevice;

	std::vector<Font_ptr>		  vCache;
};

}