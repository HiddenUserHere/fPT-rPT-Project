#pragma once

namespace DX
{

typedef std::shared_ptr<class Texture> Texture_ptr;
	
class Texture
{
public:
						  Texture( LPDIRECT3DTEXTURE9 lpTexture );
						  Texture( LPDIRECT3DTEXTURE9 lpTexture, BOOL bUse3D );
						  Texture( LPDIRECT3DTEXTURE9 lpTexture, std::string strTextureFile, BOOL bUse3D );
						  Texture( const Texture & c );
	virtual				 ~Texture();

	LPDIRECT3DTEXTURE9	  Get() { return lpTexture; }
	const std::string	& GetFile() const { return strTextureFile; }
	BOOL				  GetUse3D() const { return bUse3D; }

	void				  Renew( LPDIRECT3DTEXTURE9 _lpTexture );

	UINT				  GetWidth();
	UINT				  GetHeight();

	void				  SetTransparent( BOOL b ) { bIsTransparent = b; }
	BOOL				  GetTransparent() const { return bIsTransparent; }

protected:
	void				  LoadDescription();

protected:
	LPDIRECT3DTEXTURE9	  lpTexture = NULL;
	std::string			  strTextureFile;
	BOOL				  bUse3D = FALSE;

	D3DSURFACE_DESC		  d3dSurfaceDesc;

	BOOL				  bIsTransparent = FALSE;
};

}