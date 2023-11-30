#include "stdafx.h"
#include "UIButton.h"

namespace UI
{
Button::Button(Rectangle2D rRectangle) : Element(rRectangle)
{

}

Button::~Button()
{
}

void Button::Clear()
{
	CLEARPTR( pImage );
	CLEARPTR( pImageActive );
	CLEARPTR( pImageDisable );
	UI::ImageLoader::DeleteImage( pImage );
	UI::ImageLoader::DeleteImage( pImageActive );
	UI::ImageLoader::DeleteImage( pImageDisable );
}

void Button::SetSelected( BOOL b )
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

void Button::SetActiveImage( std::pair<class UI::Image *, class UI::Image *> pPair )
{
	pImage			= pPair.first;
	pImageActive	= pPair.second;
}

void Button::Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	if ( IsOpen() )
	{
		if( (iWidth	- GetX()) <= 0 )
			return;

		if( (iHeight - GetY()) <= 0 )
			return;

		int iRenderX		= GetX() + iX;
		int iRenderY		= GetY() + iY;
		int iRenderWidth	= low( GetWidth(), iWidth	- GetX() );
		int iRenderHeight	= low( GetHeight(), iHeight - GetY() );
	
		if ( bDisable )
		{
			UI::ImageRender::Render( pImageDisable, iRenderX, iRenderY, rRect.iWidth, rRect.iHeight, iSourceX, iSourceY, -1, Element::GetScale() );
			bHover	= FALSE;
			bSelect = FALSE;
			return;
		}

		if( bCanHold && bHolding )
			UI::Event::Invoke( iEventID );

		if ( pImageHold && bHoldingClick )
		{
			UI::ImageRender::Render( pImageHold, iRenderX, iRenderY, rRect.iWidth, rRect.iHeight, iSourceX, iSourceY, -1, Element::GetScale() );
			return;
		}

		DWORD dwColor = (iAlphaHover > 0 && !bHover) ? D3DCOLOR_ARGB( iAlphaHover, 255, 255, 255 ) : D3DCOLOR_ARGB( 255, 255, 255, 255 );

		UI::Image * pImageRender = pImage;

		if ( bHover || bSelect )
		{
			if ( pGroupMain )
			{
				if ( pGroupMain->GetIndexObject() != GetID() )
					bSelect = FALSE;
			}
			
			pImageRender = pImageActive ? pImageActive : pImage;
		}

		UI::ImageRender::Render( pImageRender, iRenderX, iRenderY, rRect.iWidth, rRect.iHeight, iSourceX, iSourceY, dwColor, Element::GetScale() );

		Element::Render( iRenderX, iRenderY, iRenderWidth, iRenderHeight, iSourceX, iSourceY );
	}
}



BOOL Button::OnMouseClick( CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	if( !IsOpen() || bDisable )
		return FALSE;

	Point2D * psPosition = pcMouse->GetPosition();

	int iRenderX		= GetX() + iX + iSourceX;
	int iRenderY		= GetY() + iY + iSourceY;
	int iRenderWidth	= low( GetWidth(), iWidth );
	int iRenderHeight	= low( GetHeight(), iHeight );

	if( bCanHold && bHolding && ( pcMouse->GetEvent() == ClickUpL || pcMouse->GetEvent() == ClickUpR ) )
		bHolding = FALSE;

	if ( bHoldingClick && (pcMouse->GetEvent() == ClickUpL || pcMouse->GetEvent() == ClickUpR) )
		bHoldingClick = FALSE;

	// Inside X?
	if ( (psPosition->iX >= iRenderX) && (psPosition->iX <= (iRenderX + iRenderWidth - iSourceX)) )
	{
		// Inside Y?
		if ( (psPosition->iY >= iRenderY) && (psPosition->iY <= (iRenderY + iRenderHeight - iSourceY)) )
		{
			//Dropdown Menu
			if( (pDropdownMenu) && !pDropdownMenu->IsOpen() && pDropdownMenu->GetMouseEvent() == pcMouse->GetEvent() )
			{
				pDropdownMenu->Open();
				return TRUE;
			}
			else if( (pDropdownMenu) && pDropdownMenu->IsOpen() && pDropdownMenu->GetMouseEvent() == pcMouse->GetEvent() )
			{
				pDropdownMenu->Close();
				return TRUE;
			}

			if ( bSelectType && eMouseEvent == pcMouse->GetEvent() )
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

			if( iEventID && eMouseEvent == pcMouse->GetEvent() || bHolding )
			{
				if( bCanHold )
					bHolding = TRUE;

				UI::Event::Invoke( iEventID );
				return TRUE;
			}

			if ( pImageHold && ((pcMouse->GetEvent() == ClickDownL) && (eMouseEvent == ClickUpL)) )
				bHoldingClick = TRUE;
		}
	}

	return FALSE;
}

void Button::OnMouseMove( CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	if ( !IsOpen() || bDisable )
		return; 

	Point2D * psPosition = pcMouse->GetPosition();

	int iRenderX		= GetX() + iX + iSourceX;
	int iRenderY		= GetY() + iY + iSourceY;
	int iRenderWidth	= low( GetWidth(), iWidth );
	int iRenderHeight	= low( GetHeight(), iHeight );

	bHover = FALSE;
	// Inside X?
	if ( (psPosition->iX >= iRenderX) && (psPosition->iX <= (iRenderX + iRenderWidth - iSourceX)) )
	{
		// Inside Y?
		if ( (psPosition->iY >= iRenderY) && (psPosition->iY <= (iRenderY + iRenderHeight - iSourceY)) )
		{
			bHover = TRUE;
		}
	}
}
}