#include "stdafx.h"
#include "DXTextureEngine.h"

namespace DX
{
	/*
class DX::TextureContainer * TextureEngine::Load( std::string strFileName )
{
	if( !FILEEXIST( strFileName ) )
		return NULL;

	DX::TextureContainer * pcTextureContainer = new DX::TextureContainer( strFileName );

	// Get File Data Info
	D3DXIMAGE_INFO sImageInfo;
	D3DXGetImageInfoFromFileA( strFileName.c_str(), &sImageInfo );

	// Add to Container data
	pcTextureContainer->SetType( sImageInfo.ImageFileFormat );
	pcTextureContainer->SetWidth( sImageInfo.Width );
	pcTextureContainer->SetHeight( sImageInfo.Height );
	pcTextureContainer->SetDepth( sImageInfo.Depth );

	// Use a Magenta color key
	D3DCOLOR dwColor = 0xFFFF00FF;

	// Bitmap Mask Alpha
	if( pcTextureContainer->GetType() == TEXTURETYPE_BMP )
		dwColor = 0xFF000000;

	IDirect3DTexture9 * psTexture = NULL;
	if( FAILED( D3DXCreateTextureFromFileExA( DX::Graphic::GetDevice(), strFileName.c_str(), pcTextureContainer->GetWidth(), pcTextureContainer->GetHeight(), 1, 0, D3DFMT_A8B8G8R8, DXGRAPHICENGINE_D3DPOOL, D3DX_FILTER_NONE, D3DX_DEFAULT, dwColor, &sImageInfo, NULL, &psTexture ) ) )
	{
		// TODO: Log Error
	}
	return nullptr;
}

void TextureEngine::DrawTexture2D( LPDIRECT3DTEXTURE9 lpTexture, D3DCOLOR d3dColor, Rectangle2D & rDestinationRectangle, Rectangle2D & rSourceRectangle, BOOL bBitmapTransparency )
{
	//Don't draw texture if it's size is 0x0
	if( ( rDestinationRectangle.iWidth == 0 ) && ( rDestinationRectangle.iHeight == 0 ) )
		return;

	DX::Graphic::GetDevice()->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
	DX::Graphic::GetDevice()->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	DX::Graphic::GetDevice()->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

	float u, v, u2, v2;

	if( lpTexture )
	{
		//Store texture sizes
		float w = float( rDestinationRectangle.iWidth );
		float h = float( rDestinationRectangle.iHeight );

		//Compute texture coordinates
		u = ( float( rSourceRectangle.iX ) + 0.5f ) / w;
		v = ( float( rSourceRectangle.iY ) + 0.5f ) / h;
		u2 = u + float( rSourceRectangle.iWidth ) / w;
		v2 = v + float( rSourceRectangle.iHeight ) / h;
	}
	else
	{
		//Compute texture coordinates, it's easy
		u = 0.0f;
		v = 0.0f;
		u2 = 1.0f;
		v2 = 1.0f;
	}

	EXEDrawVertexMinor rVertex[4];
	rVertex[0] = { float( rDestinationRectangle.iX ), float( rDestinationRectangle.iY ), 0.0f, 1, d3dColor, u, v };
	rVertex[1] = { float( rDestinationRectangle.iX + rDestinationRectangle.iWidth ), float( rDestinationRectangle.iY ), 0, 1, d3dColor, u2, v };
	rVertex[2] = { float( rDestinationRectangle.iX ), float( rDestinationRectangle.iY + rDestinationRectangle.iHeight ), 0, 1, d3dColor, u, v2 };
	rVertex[3] = { float( rDestinationRectangle.iX + rDestinationRectangle.iWidth ), float( rDestinationRectangle.iY + rDestinationRectangle.iHeight ), 0, 1, d3dColor, u2, v2 };

	if( lpTexture )
	{
		DX::Graphic::GetDevice()->SetTexture( 0, lpTexture );
		DX::Graphic::GetDevice()->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 );
		DX::Graphic::GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, rVertex, sizeof(EXEDrawVertexMinor) );
	}
}
*/
}