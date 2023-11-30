#include "stdafx.h"
#include "UIText.h"
#include "UI.h"

namespace UI
{
Text::Text( Rectangle2D rRectangle ) : Element( rRectangle )
{
	pFont = GetGraphicsDevice()->GetDefaultFont();
}

Text::Text( const std::string & strText, const Rectangle2D & rBox, BOOL bBold, const DWORD dwColor ) : Element( rBox )
{
	pFont = GetGraphicsDevice()->GetFontFactory()->MakeFont( nullptr, "Arial", 16, 6, bBold, FALSE );

	SetColor( dwColor );
	SetText( strText );
}

Text::Text( const Rectangle2D & rBox, BOOL bBold, const DWORD dwColor ) : Element( rBox )
{
	pFont = GetGraphicsDevice()->GetFontFactory()->MakeFont( nullptr, "Arial", 16, 6, bBold, FALSE );
	SetColor( dwColor );
}

Text::~Text()
{
}

void Text::Clear()
{
}

void Text::SetFont( const char * pszFont, int iHeight, int iWidth, BOOL bItalic, BOOL bBold, DWORD dwColor )
{
	pFont = GetGraphicsDevice()->GetFontFactory()->MakeFont( nullptr, pszFont, iHeight, iWidth, bBold, FALSE );
	dwColorText		= dwColor;
}

int Text::GetHeightFont()
{
	return pFont->GetDesc().Height;
}

void Text::UpdateText()
{
	auto IsSpaceLambda = []( const int c ) -> bool
	{
		if ( (c < 0) || (c > 255) )
			return false;

		return std::isspace( c );
	};

	strHighlightText = "";

	if( iMaxTextWidth > 0 )
	{
		int iTextWidth = 0;
		const char * pszText = strText.c_str();

		for( UINT i = 0; i < strText.size(); i++ )
		{
			char c[2] = { pszText[i], 0 };

			int iWidth = pFont->Calc( c, BuildFormat() );

			if ( iWidth == 0 )
			{
				if ( IsSpaceLambda( c[0] ) )
				{
					char c2[4] = { 'i', c[0], 'i', 0 };

					iWidth = pFont->Calc( c2, BuildFormat() );

					c2[1] = 0;
					c2[2] = 0;

					iWidth -= pFont->Calc( c2, BuildFormat() ) * 2;
				}
			}
			
			iTextWidth += iWidth;

			if( iTextWidth >= iMaxTextWidth )
			{
				strText.insert( strText.begin() + i, '\n' );
				iTextWidth = 0;
			}
		}
	}

	if( bHighlightText )
	{
		if( strText.find( ":" ) != std::string::npos )
		{
			strHighlightText = strText.substr( 0, strText.find( ":" ) + 1 );
			strText = strText.substr( strText.find( ":" ) + 1 );
			iHighlightTextWidth = pFont->Calc( strHighlightText, BuildFormat() );
		}
	}
}

int Text::GetCountLines()
{
	int iLengthText = strText.length();

	if( iLengthText <= 0 )
		return 0;

	int iCountLines = 1;

	for( int i = 0; i < iLengthText; i++ )
	{
		if( strText[ i ] == '\n' )
			iCountLines++;
	}

	return iCountLines;
}

void Text::SetTextEx( char * pszFormat, ... )
{
	va_list vl;

	char szText[ 32768 ];

	va_start( vl, pszFormat );
	_vsnprintf_s( szText, _countof( szText ), _TRUNCATE, pszFormat, vl );
	va_end( vl );

	SetText( szText );
}

void Text::SetSelected( BOOL b )
{
	bSelect = b;
	if ( pGroupMain )
	{
		if ( b )
			pGroupMain->SetIndexObject( GetID() );
		else
		{
			if ( pGroupMain->GetIndexObject() == GetID() )
				pGroupMain->SetIndexObject( -1 );
		}
	}
}

int Text::GetTextWidth()
{
	if( bHighlightText )
	{
		int iMaxWidth = iMaxTextWidth > 0 ? iMaxTextWidth : GetWidth(), iMaxHeight = GetHeight();

		if ( iMaxHeight == 0 )
			iMaxHeight = RESOLUTION_HEIGHT;
		if ( iMaxWidth == 0 )
			iMaxWidth = RESOLUTION_WIDTH;

		//Has line?
		pFont->Calc( strText, BuildFormat(), iMaxWidth, iMaxHeight );
		std::vector<std::string> vLines = split( strText, '\n' );
		if ( vLines.size() > 1 )
		{
			iMaxWidth = iMaxTextWidth + 1;
		}
		else
			iMaxWidth += pFont->Calc( strHighlightText, BuildFormat() );

		return iMaxWidth;
	}

	return pFont->Calc( strText.c_str(), BuildFormat() );
}

int Text::GetHeightText()
{
	int iRenderX		= GetX();
	int iRenderY		= GetY();
	int iRenderWidth	= GetWidth();
	int iRenderHeight	= GetHeight();

	RECT rRect;
	rRect.left		= iRenderX;
	rRect.top		= iRenderY;
	rRect.right		= rRect.left + GetWidth();
	rRect.bottom	= rRect.top + GetHeight();

	pFont->Calc( strText, BuildFormat(), iRenderWidth, iRenderHeight );

	return iRenderHeight;
}

void Text::Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	// Is activated Button?
	int iRenderX		= GetX() + iX;
	int iRenderY		= GetY() + iY;
	int iRenderWidth	= low( GetWidth(), iWidth );
	int iRenderHeight	= low( GetHeight(), iHeight );

	DWORD dwColor = dwColorText;

	if ( dwColorTextLink != 0 && (bHover || (bSelectType && bSelect)) && !bDisable )
		dwColor = dwColorTextLink;

	if ( pGroupMain && (pGroupMain->GetIndexObject() != GetID()) )
		bSelect = FALSE;

	RECT rRect;
	rRect.left		= iRenderX;
	rRect.top		= iRenderY;
	rRect.right		= rRect.left + GetWidth();
	rRect.bottom	= rRect.top + GetHeight();

	if( bHighlightText && strHighlightText.length() > 0 && IsOpen() && !bDisable )
	{
		//Vector Delimiter
		std::vector<std::string> vLines = split( strText, '\n' );

		for( size_t i = 0; i < vLines.size(); i++ )
		{
			if( i == 0 )
			{
				if( bShadow )
				{
					RECT rRect1 = RECT{ rRect.left + iShadowDistance, rRect.top + iShadowDistance, rRect.right, rRect.bottom };
					RECT rRect2 = RECT{ rRect.left + iHighlightTextWidth + iShadowDistance, rRect.top + iShadowDistance, rRect.right, rRect.bottom };

					pFont->Draw( strHighlightText, rRect1, BuildFormat(), DX::Color( dwColorTextShadow != -1 ? dwColorTextShadow : D3DCOLOR_XRGB( 74, 74, 74 ) ) );
					pFont->Draw( vLines[i], rRect2, BuildFormat(), DX::Color( dwColorTextShadow != -1 ? dwColorTextShadow : D3DCOLOR_XRGB( 74, 74, 74 ) ) );
				}
				else if( dwColorTextOutline )
				{
					pFont->Draw( strHighlightText, RECT{ rRect.left + 1, rRect.top, rRect.right, rRect.bottom }, BuildFormat(), DX::Color( dwColorTextOutline ) );
					pFont->Draw( vLines[i], RECT{ rRect.left + iHighlightTextWidth + 1, rRect.top, rRect.right, rRect.bottom }, BuildFormat(), DX::Color( dwColorTextOutline ) );

					pFont->Draw( strHighlightText, RECT{ rRect.left, rRect.top + 1, rRect.right, rRect.bottom }, BuildFormat(), DX::Color( dwColorTextOutline ) );
					pFont->Draw( vLines[i], RECT{ rRect.left + iHighlightTextWidth, rRect.top + 1, rRect.right, rRect.bottom }, BuildFormat(), DX::Color( dwColorTextOutline ) );

					pFont->Draw( strHighlightText, RECT{ rRect.left - 1, rRect.top, rRect.right, rRect.bottom }, BuildFormat(), DX::Color( dwColorTextOutline ) );
					pFont->Draw( vLines[i], RECT{ rRect.left + iHighlightTextWidth - 1, rRect.top, rRect.right, rRect.bottom }, BuildFormat(), DX::Color( dwColorTextOutline ) );

					pFont->Draw( strHighlightText, RECT{ rRect.left, rRect.top - 1, rRect.right, rRect.bottom }, BuildFormat(), DX::Color( dwColorTextOutline ) );
					pFont->Draw( vLines[i], RECT{ rRect.left + iHighlightTextWidth, rRect.top - 1, rRect.right, rRect.bottom }, BuildFormat(), DX::Color( dwColorTextOutline ) );
				}
				
				pFont->Draw( strHighlightText, rRect, BuildFormat(), DX::Color( dwColorTextHighlight ) );
				pFont->Draw( vLines[i], RECT{ rRect.left + iHighlightTextWidth,rRect.top,rRect.right + iHighlightTextWidth,rRect.bottom }, BuildFormat(), DX::Color( dwColor ) );
			}
			else
			{
				UINT uYLine = GetHeightFont() * i;

				if( bShadow )
					pFont->Draw( vLines[i], RECT{ rRect.left + iShadowDistance,(LONG)(rRect.top + uYLine + iShadowDistance),rRect.right,(LONG)(rRect.bottom + uYLine) }, BuildFormat(), DX::Color( dwColorTextShadow != -1 ? dwColorTextShadow : D3DCOLOR_XRGB( 74, 74, 74 ) ) );
				else if( dwColorTextOutline )
				{
					pFont->Draw( vLines[i], RECT{ rRect.left + 1, (LONG)(rRect.top + uYLine), rRect.right, (LONG)(rRect.bottom + uYLine) }, BuildFormat(), DX::Color( dwColorTextOutline ) );
					pFont->Draw( vLines[i], RECT{ rRect.left, (LONG)(rRect.top + uYLine) + 1, rRect.right, (LONG)(rRect.bottom + uYLine) }, BuildFormat(), DX::Color( dwColorTextOutline ) );
					pFont->Draw( vLines[i], RECT{ rRect.left - 1, (LONG)(rRect.top + uYLine), rRect.right, (LONG)(rRect.bottom + uYLine) }, BuildFormat(), DX::Color( dwColorTextOutline ) );
					pFont->Draw( vLines[i], RECT{ rRect.left, (LONG)(rRect.top + uYLine - 1), rRect.right, (LONG)(rRect.bottom + uYLine) }, BuildFormat(), DX::Color( dwColorTextOutline ) );
				}

				pFont->Draw( vLines[i], RECT{ rRect.left,(LONG)(rRect.top + uYLine),rRect.right,(LONG)(rRect.bottom + uYLine) }, BuildFormat(), DX::Color( dwColor ) );
			}
		}
	}
	else if( !bHighlightText && IsOpen() && !bDisable )
	{
		if( bShadow )
			pFont->Draw( strText, RECT{ rRect.left + iShadowDistance, (LONG)(rRect.top + iShadowDistance), rRect.right, (LONG)(rRect.bottom) }, BuildFormat(), DX::Color( dwColorTextShadow != -1 ? dwColorTextShadow : D3DCOLOR_XRGB( 74, 74, 74 ) ) );
		else if( dwColorTextOutline )
		{
			pFont->Draw( strText, RECT{ rRect.left + 1, (LONG)(rRect.top), rRect.right, (LONG)(rRect.bottom) }, BuildFormat(), DX::Color( dwColorTextOutline ) );
			pFont->Draw( strText, RECT{ rRect.left, (LONG)(rRect.top + 1), rRect.right, (LONG)(rRect.bottom) }, BuildFormat(), DX::Color( dwColorTextOutline ) );
			pFont->Draw( strText, RECT{ rRect.left - 1, (LONG)(rRect.top), rRect.right, (LONG)(rRect.bottom) }, BuildFormat(), DX::Color( dwColorTextOutline ) );
			pFont->Draw( strText, RECT{ rRect.left, (LONG)(rRect.top - 1), rRect.right, (LONG)(rRect.bottom) }, BuildFormat(), DX::Color( dwColorTextOutline ) );
		}

		pFont->Draw( strText, rRect, BuildFormat(), DX::Color( dwColor ) );
	}
	else if( IsOpen() && bDisable )
		pFont->Draw( strText, rRect, BuildFormat(), DX::Color( D3DCOLOR_ARGB( 255, 135, 135, 135 ) ) );


	Element::Render( iRenderX, iRenderY, GetWidth(), GetHeight(), iSourceX, iSourceY );
}

void Text::Update( float fTime )
{
	
}

UINT Text::BuildFormat() const
{
	UINT uFormat = DT_EXPANDTABS;

	switch( eHorizontalAlign )
	{
	default:
	case ALIGN_Left:
		uFormat |= DT_LEFT;
		break;
	case ALIGN_Center:
		uFormat |= DT_CENTER;
		break;
	case ALIGN_Right:
		uFormat |= DT_RIGHT;
		break;
	}

	switch( eVerticalAlign )
	{
	default:
	case ALIGN_Top:
		uFormat |= DT_TOP;
		break;
	case ALIGN_Middle:
		uFormat |= DT_VCENTER;
		break;
	case ALIGN_Bottom:
		uFormat |= DT_BOTTOM;
		break;
	}

	if( !bMultiLine )
		uFormat |= DT_SINGLELINE;

	if( bWordWrap )
		uFormat |= DT_WORDBREAK;

	if( bNoClip )
		uFormat |= DT_NOCLIP;

	return uFormat;
}

BOOL Text::OnMouseClick( CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	if ( !IsOpen() || bDisable )
		return FALSE;

	if ( eMouseEvent != pcMouse->GetEvent() )
		return FALSE;

	Point2D * psPosition = pcMouse->GetPosition();

	int iRenderX		= GetX() + iX + iSourceX;
	int iRenderY		= GetY() + iY + iSourceY;
	int iRenderWidth	= bParentChild ? low( GetWidth(), iWidth	- GetX() ) : GetWidth();
	int iRenderHeight	= bParentChild ? low( GetHeight(), iHeight - GetY() ) : GetHeight();

	// Inside X?
	if ( (psPosition->iX >= iRenderX) && (psPosition->iX <= (iRenderX + iRenderWidth - iSourceX)) )
	{
		// Inside Y?
		if ( (psPosition->iY >= iRenderY) && (psPosition->iY <= (iRenderY + iRenderHeight - iSourceY)) )
		{
			if ( bSelectType )
			{
				if ( pGroupMain )
				{
					if ( !bSelect )
						pGroupMain->SetIndexObject( GetID() );
					bSelect = TRUE;

				}
				else
					bSelect = bSelect ? FALSE : TRUE;
			}
			if ( bSelectType || iEventID != (-1) )
			{
				UI::Event::Invoke( iEventID );
				return TRUE;
			}
		}
	}
	return FALSE;
}

void Text::OnMouseMove( CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	if ( !IsOpen() || bDisable )
		return; 

	Point2D * psPosition = pcMouse->GetPosition();

	int iRenderX		= GetX() + iX + iSourceX;
	int iRenderY		= GetY() + iY + iSourceY;
	int iRenderWidth	= bParentChild ? low( GetWidth(), iWidth	- GetX() ) : GetWidth();
	int iRenderHeight	= bParentChild ? low( GetHeight(), iHeight - GetY() ) : GetHeight();


	BOOL bOldHover = bHover;

	bHover = FALSE;

	//Inside X?
	if ( (psPosition->iX >= iRenderX) && (psPosition->iX <= (iRenderX + iRenderWidth - iSourceX)) )
	{
		//Inside Y?
		if ( (psPosition->iY >= iRenderY) && (psPosition->iY <= (iRenderY + iRenderHeight - iSourceY)) )
		{
			bHover = TRUE;

			if ( bHover != bOldHover && (iHoverEventID != (-1)) )
			{
				UI::Event::Invoke( iHoverEventID );
			}
		}
	}
}
}
