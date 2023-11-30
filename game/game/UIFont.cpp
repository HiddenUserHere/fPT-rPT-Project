#include "stdafx.h"
#include "UIFont.h"


CUIFont::CUIFont()
{
}


CUIFont::~CUIFont()
{
}

void CUIFont::SetFontSet( const char * pszFaceName, BOOL bItalic, BOOL bBold, int iWidth, int iHeight, UINT uColor )
{
	pFont = std::make_shared<DX::Font>();
	iHandlerFont = pFont->CreateFont( pszFaceName, iHeight, iWidth, bItalic, bBold, -1 );
	uColorText = uColor;
}

void CUIFont::SetText( const char * pszText )
{
	strText = pszText;
}

void CUIFont::DeleteChar()
{
	if ( strText.size() > 0 )
		strText.erase( strText.size() - 1 );
}

void CUIFont::AddChar( char cChar )
{
	if ( cChar == VK_BACK )
	{
		DeleteChar();
		return;
	}
	else if ( cChar == VK_RETURN || cChar == VK_TAB || cChar == 0x0A || cChar == VK_ESCAPE )
		return;

	if ( (int)strText.size() < iMaxChar )
		strText.insert( strText.end(), cChar );
}

int CUIFont::GetWidth()
{
	if ( pFont )
		return pFont->GetWidthText( iHandlerFont, strText.c_str(), strText.length() );

	return 0;
}

void CUIFont::Draw( int iX, int iY )
{
	D3DVIEWPORT9 pViewPortGet;
	DX::Graphic::GetDevice()->GetViewport( &pViewPortGet );

	if ( pFont )
		pFont->Draw( iHandlerFont, pViewPortGet.X + iX, pViewPortGet.Y + iY, strText.c_str(), DT_WORDBREAK, uColorText );
}

void CUIFont::DrawBarText( int iX, int iY, bool bActive )
{
	D3DVIEWPORT9 pViewPortGet;
	DX::Graphic::GetDevice()->GetViewport( &pViewPortGet );

	if ( pFont )
	{
		if ( bActive )
			pFont->Draw( iHandlerFont, pViewPortGet.X + iX + 1 + GetWidth(), pViewPortGet.Y + iY, "|", DT_WORDBREAK, uColorText );
		pFont->Draw( iHandlerFont, pViewPortGet.X + iX, pViewPortGet.Y + iY, strText.c_str(), DT_WORDBREAK, uColorText );
	}
}

void CUIFont::Draw( RECT rBox )
{
	D3DVIEWPORT9 pViewPortGet;
	DX::Graphic::GetDevice()->GetViewport( &pViewPortGet );

	if ( pFont )
		pFont->Draw( iHandlerFont, rBox.left, rBox.top, strText.c_str(), DT_CENTER | DT_VCENTER, uColorText );
}

