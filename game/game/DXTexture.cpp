#include "stdafx.h"
#include "DXTexture.h"

namespace DX
{

Texture::Texture( LPDIRECT3DTEXTURE9 lpTexture ) : lpTexture(lpTexture)
{
	ADDREF( lpTexture );

	LoadDescription();
}

Texture::Texture( LPDIRECT3DTEXTURE9 lpTexture, BOOL bUse3D ) : lpTexture(lpTexture), bUse3D(bUse3D)
{
	ADDREF( lpTexture );

	LoadDescription();
}

Texture::Texture( LPDIRECT3DTEXTURE9 lpTexture, std::string strTextureFile, BOOL bUse3D ) : lpTexture(lpTexture), strTextureFile(strTextureFile), bUse3D(bUse3D)
{
	ADDREF( lpTexture );

	LoadDescription();
}

Texture::Texture( const Texture & c ) : lpTexture(c.lpTexture), strTextureFile(c.strTextureFile), bUse3D(c.bUse3D)
{
	ADDREF( lpTexture );

	LoadDescription();
}

Texture::~Texture()
{
	RELEASE( lpTexture );
}

void Texture::Renew( LPDIRECT3DTEXTURE9 _lpTexture )
{
	RELEASE( lpTexture );

	lpTexture = _lpTexture;

	ADDREF( lpTexture );

	LoadDescription();
}

UINT Texture::GetWidth()
{
	if( !lpTexture )
		return 0;

	return d3dSurfaceDesc.Width;
}

UINT Texture::GetHeight()
{
	if( !lpTexture )
		return 0;

	return d3dSurfaceDesc.Height;
}

void Texture::LoadDescription()
{
	if( lpTexture )
		lpTexture->GetLevelDesc( 0, &d3dSurfaceDesc );
}

}