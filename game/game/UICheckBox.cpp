#include "stdafx.h"
#include "UICheckBox.h"

namespace UI
{
CheckBox::CheckBox( Rectangle2D rRectangle ) : Element( rRectangle )
{
}

CheckBox::~CheckBox()
{
}

void CheckBox::Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	if ( IsOpen() )
	{
		if ( pGroupMain )
		{
			if ( !pGroupMain->IsOpen() )
				return;

			if ( pGroupMain->GetIndexObject() != GetID() )
				bSelect = FALSE;
			else
				bSelect = TRUE;
		}
		int iRenderX		= GetX() + iX;
		int iRenderY		= GetY() + iY;
		UI::ImageRender::Render( pImage, iRenderX, iRenderY, GetWidth(), GetHeight(), -1 );
		if ( bSelect )
			UI::ImageRender::Render( pImage2, iRenderX, iRenderY, GetWidth(), GetHeight(), -1 );

		pFont->Draw( strText, iRenderX + GetWidth() + 5, iRenderY + ((GetHeight() >> 1) - (iFontHeight >> 1)), DT_WORDBREAK, dwColorText );
	
		Element::Render( iRenderX, iRenderY, GetWidth(), GetHeight(), iSourceX, iSourceY );
	}
}

BOOL CheckBox::OnMouseClick( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	if ( !IsOpen() || bDisable )
		return FALSE;

	if ( pGroupMain )
	{
		if ( !pGroupMain->IsOpen() )
			return FALSE;
	}

	if ( EMouseEvent::ClickDownL != pcMouse->GetEvent() )
		return FALSE;

	Point2D * psPosition = pcMouse->GetPosition();

	int iRenderX		= GetX() + iX + iSourceX;
	int iRenderY		= GetY() + iY + iSourceY;
	int iRenderWidth	= bParentChild ? low( GetWidth(), iWidth	- GetX() ) : GetWidth();
	int iRenderHeight	= bParentChild ? low( GetHeight(), iHeight - GetY() ) : GetHeight();

	// Inside X?
	int iWidthText = pFont->Calc( strText );
	if ( (psPosition->iX >= iRenderX) && (psPosition->iX <= (iRenderX + iWidthText + iRenderWidth - iSourceX)) )
	{
		// Inside Y?
		if ( (psPosition->iY >= iRenderY) && (psPosition->iY <= (iRenderY + iRenderHeight - iSourceY)) )
		{
			if ( pGroupMain )
			{
				if ( !bSelect )
					pGroupMain->SetIndexObject( GetID() );
				bSelect = TRUE;

			}
			else
				bSelect = bSelect ? FALSE : TRUE;

			UI::Event::Invoke( iEventID );
			return TRUE;
		}
	}
	return FALSE;
}

void CheckBox::Clear()
{
	CLEARPTR( pImage );
	CLEARPTR( pImage2 );
	UI::ImageLoader::DeleteImage( pImage );
	UI::ImageLoader::DeleteImage( pImage2 );
}

void CheckBox::SetFont( const char * pszFont, int iHeight, int iWidth, BOOL bItalic, BOOL bBold, DWORD dwColor )
{
	pFont			= GetGraphicsDevice()->GetFontFactory()->MakeFont( nullptr, pszFont, iHeight, iWidth, bBold, bItalic );
	iFontHeight		= iHeight;
	dwColorText		= dwColor;
}
}