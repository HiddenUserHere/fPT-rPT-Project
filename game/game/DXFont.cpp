#include "stdafx.h"
#include "DXFont.h"

namespace DX
{

const std::string DEFAULT_FONT = "Arial";
const int DEFAULT_FONT_HEIGHT = 16;
const int DEFAULT_FONT_WIDTH = 6;

DXFont::DXFont( Sprite_ptr pSprite, LPD3DXFONT lpFont, D3DXFONT_DESC FontDesc ) : pSprite( pSprite ), lpFont( lpFont ), FontDesc( FontDesc )
{
	ADDREF( lpFont );
}

DXFont::DXFont( const DXFont & c ) : pSprite( c.pSprite ), lpFont( c.lpFont ), FontDesc( c.FontDesc )
{
	ADDREF( lpFont );
}

DXFont::~DXFont()
{
	RELEASE( lpFont );
}

void DXFont::OnLostDevice()
{
	if ( lpFont )
		lpFont->OnLostDevice();
}

void DXFont::OnResetDevice()
{
	if ( lpFont )
		lpFont->OnResetDevice();
}

void DXFont::Draw( const char * pszText, int iTextLength, RECT & rRect, UINT uFormat, const Color & cColor )
{
	if ( !lpFont )
		return;

	if ( pSprite )
		pSprite->Begin( TRUE );

	Draw( pSprite ? pSprite->Get() : NULL, lpFont, pszText, iTextLength, &rRect, uFormat, cColor.Get() );

	if ( pSprite )
		pSprite->End();
}

void DXFont::Draw( const std::string & strText, RECT & rRect, UINT uFormat, const Color & cColor )
{
	if ( !lpFont )
		return;

	if ( pSprite )
		pSprite->Begin( TRUE );

	Draw( pSprite ? pSprite->Get() : NULL, lpFont, strText.c_str(), strText.length(), &rRect, uFormat, cColor.Get() );

	if ( pSprite )
		pSprite->End();
}

void DXFont::Draw( const std::string & strText, Point2D & sPosition, UINT uFormat, const Color & cColor )
{
	RECT rRect;
	rRect.left = sPosition.iX;
	rRect.top = sPosition.iY;
	rRect.right = rRect.left + RESOLUTION_WIDTH;
	rRect.bottom = rRect.top + RESOLUTION_HEIGHT;

	Draw( strText, rRect, uFormat, cColor );
}

void DXFont::Draw( const std::string & strText, INT iX, INT iY, UINT uFormat, const Color & cColor )
{
	RECT rRect;
	rRect.left = iX;
	rRect.top = iY;
	rRect.right = rRect.left + RESOLUTION_WIDTH;
	rRect.bottom = rRect.top + RESOLUTION_HEIGHT;

	Draw( strText, rRect, uFormat, cColor );
}

void DXFont::Calc( const std::string & strText, UINT uFormat, int & iWidth, int & iHeight )
{
	if ( !lpFont )
		return;

	RECT rRect;
	rRect.left = 0;
	rRect.top = 0;
	rRect.right = rRect.left + iWidth;
	rRect.bottom = rRect.top + iHeight;

	if ( pSprite )
		pSprite->Begin( TRUE );

	iHeight = Draw( pSprite ? pSprite->Get() : NULL, lpFont, strText.c_str(), strText.length(), &rRect, uFormat | DT_CALCRECT, D3DCOLOR_XRGB( 255, 255, 255 ) );

	if ( pSprite )
		pSprite->End();

	iWidth = rRect.right - rRect.left;
}

int DXFont::Calc( const std::string & strText, UINT uFormat )
{
	int iWidth = RESOLUTION_WIDTH;
	int iHeight = RESOLUTION_HEIGHT;

	Calc( strText, uFormat, iWidth, iHeight );

	return iWidth;
}

int DXFont::CalcHeight( const std::string & strText, UINT uFormat )
{
	int iWidth = RESOLUTION_WIDTH;
	int iHeight = RESOLUTION_HEIGHT;

	Calc( strText, uFormat, iWidth, iHeight );

	return iHeight;
}

INT DXFont::Draw( LPD3DXSPRITE pSprite, LPD3DXFONT pFont, LPCTSTR pString, INT Count, LPRECT pRect, DWORD Format, D3DCOLOR Color )
{
	static const int UNICODE_BUFFER_SIZE = 8192;
	static const int UTF8_BUFFER_SIZE = UNICODE_BUFFER_SIZE * 2;

	//Empty Text? Don't bother drawing
	if ( Count <= 0 )
	{
		if ( Format & DT_CALCRECT )
			pRect->left = pRect->top = pRect->right = pRect->bottom = 0;

		return 0;
	}

	//Static Arrays for Local Buffers
	static WCHAR szUNICODE[UNICODE_BUFFER_SIZE];

	//Zero the Buffer
	//ZeroMemory( szUTF8, sizeof( szUTF8 ) );
	//ZeroMemory( szUNICODE, sizeof( szUNICODE ) );

	//Set Max Size of Count if it's exceeding the Max
	if ( Count >= UTF8_BUFFER_SIZE )
		Count = UTF8_BUFFER_SIZE - 1;


	//Copy base text to UTF8 before conversion
//	memcpy( szUTF8, pString, Count );

	int iLength = Count;

	//Convert to Unicode
	if ( iLength = MultiByteToWideChar( CP_UTF8, MB_ERR_INVALID_CHARS, pString, Count, szUNICODE, UNICODE_BUFFER_SIZE ); iLength == 0 )
		iLength = MultiByteToWideChar( CP_ACP, 0, pString, Count, szUNICODE, UNICODE_BUFFER_SIZE );

	szUNICODE[iLength] = 0;

	//Draw as Unicode
	return pFont->DrawTextW( pSprite, szUNICODE, iLength, pRect, Format, Color );
}

}
