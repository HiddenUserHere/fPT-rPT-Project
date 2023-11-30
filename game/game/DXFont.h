#pragma once

#include "DXColor.h"
#include "DXSprite.h"

namespace DX
{

extern const std::string DEFAULT_FONT;
extern const int DEFAULT_FONT_HEIGHT;
extern const int DEFAULT_FONT_WIDTH;

typedef std::shared_ptr<class DXFont> Font_ptr;

class DXFont
{
public:
	DXFont( Sprite_ptr pSprite, LPD3DXFONT lpFont, D3DXFONT_DESC FontDesc );
	DXFont( const DXFont & c );
	virtual				 ~DXFont();

	LPD3DXFONT			  Get() { return lpFont; }
	D3DXFONT_DESC & GetDesc() { return FontDesc; }

	void				  OnLostDevice();
	void				  OnResetDevice();

	void				  Draw( const char * pszText, int iTextLength, RECT & rRect, UINT uFormat, const Color & cColor );
	void				  Draw( const std::string & strText, RECT & rRect, UINT uFormat, const Color & cColor );
	void				  Draw( const std::string & strText, Point2D & sPosition, UINT uFormat, const Color & cColor );
	void				  Draw( const std::string & strText, INT iX, INT iY, UINT uFormat, const Color & cColor );
	void				  Calc( const std::string & strText, UINT uFormat, int & iWidth, int & iHeight );
	int					  Calc( const std::string & strText, UINT uFormat = 0 );
	int					  CalcHeight( const std::string & strText, UINT uFormat = 0 );

	Sprite_ptr			  GetSprite() { return pSprite; }

protected:
	static INT			  Draw( LPD3DXSPRITE pSprite, LPD3DXFONT pFont, LPCTSTR pString, INT Count, LPRECT pRect, DWORD Format, D3DCOLOR Color );

protected:
	Sprite_ptr			  pSprite;
	LPD3DXFONT			  lpFont;
	D3DXFONT_DESC		  FontDesc;
};

};