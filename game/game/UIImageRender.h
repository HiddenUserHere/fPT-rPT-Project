#pragma once

struct Vertex2D
{
	float x, y, z, rhw;
	DWORD Color;
	DWORD Specular;
	float u, v;
};

namespace UI
{
class ImageRender
{
private:
	static DX::Effect_ptr								pEffect;

public:
														ImageRender();
	virtual												~ImageRender();

	static DX::Effect_ptr								GetEffect();
	static void											SetEffect( DX::Effect_ptr p );

	static void											Render( LPDIRECT3DTEXTURE9 lpTexture, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY, DWORD dwColor, float fScaleX, float fScaleY, float fRotation = 0.0f );
	static void											Render( LPDIRECT3DTEXTURE9 lpTexture, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY, DWORD dwColor, float fScale = 1.0f, float fRotation = 0.0f );
	static void											Render( LPDIRECT3DTEXTURE9 lpTexture, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY, DWORD dwColor, float fScale, float fRotation, Point2D sRotationPoint, DWORD dwFlags = D3DXSPRITE_ALPHABLEND );
	static void											Render( LPDIRECT3DTEXTURE9 lpTexture, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY, DWORD dwColor, BOOL bScaleResolution );
	static void											Render( class Image * pImage, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY, DWORD dwColor, float fScale = 1.0f, float fRotation = 0.0f, DWORD dwFlags = D3DXSPRITE_ALPHABLEND );
	static void											Render( class Image * pImage, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY, DWORD dwColor, float fScale, float fRotation, Point2D sRotationPoint );
	static void											Render( class Image * pImage, int iX, int iY, int iWidth, int iHeight, DWORD dwColor );
	static void											RenderQuad( class Image * pImage, int iX, int iY, int iWidth, int iHeight, DWORD dwColor );
	static void											RenderQuad( LPDIRECT3DTEXTURE9 lpTexture, int iX, int iY, int iWidth, int iHeight, DWORD dwColor );
	static void											OnResetDevice();
	static void											OnLostDevice();
};
}