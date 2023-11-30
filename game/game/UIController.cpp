#include "stdafx.h"
#include "UIController.h"
#include "UI.h"


CUIController::CUIController()
{

}


CUIController::~CUIController()
{
}

void CUIController::ReposRect()
{
	bool bHaveScroll = false;

	int iHeight = 0;

	for ( auto element : CUIElement::vElementsChild )
	{
		if ( element->GetHeight() > r.h )
		{
			if ( element->GetHeight() > iHeight )
				iHeight = element->GetHeight();
			bHaveScroll = true;
		}
	}

	if ( !bHaveScroll )
		return;

	if ( pScrollControl )
	{
		int iPercentScroll = -pScrollControl->GetScrollPercent();
		iYPosScroller = ( ( ( iHeight - r.h ) * iPercentScroll ) / 100 ) - 2;
		for ( auto element : CUIElement::vElementsChild )
			element->SetYMoveObject( iYPosScroller );
	}
}

void CUIController::SetControllerRect( Rectangle2D rectangle )
{
	r = rectangle;
	pElement = CUIElement::GetPtr();
}

void CUIController::SetScroller(std::shared_ptr<class CUIScroller> pScroller)
{
	pScrollControl = pScroller;
}

void CUIController::SetFreePosition()
{
	IsFreeMove = TRUE;
}

void CUIController::Draw()
{
	if ( !bIsShowing )
		return;

	D3DVIEWPORT9 pViewPortGet;
	D3DVIEWPORT9 pViewPortSet;
	
	DX::Graphic::GetDevice()->GetViewport( &pViewPortGet );
	
	// Set viewport to elements
	{

		ZeroMemory( &pViewPortSet, sizeof( pViewPortSet ) );
		pViewPortSet.X		= pViewPortGet.X + r.x;
		pViewPortSet.Y      = pViewPortGet.Y + r.y;
		pViewPortSet.Width  = r.w;
		pViewPortSet.Height = r.h;
		DX::Graphic::GetDevice()->SetViewport( &pViewPortSet );

		// Draw
		for ( auto element : CUIElement::vElementsChild )
			element->Draw();

		// View port line
		CUILine pLine;
		pLine.DrawLine( Rectangle2D{ 0, 0, r.w, r.h }, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	}

	// Render scroller out viewport
	if ( pScrollControl )
	{
		pScrollControl->Draw();
	}

	DX::Graphic::GetDevice()->SetViewport( &pViewPortGet );

}

BOOL CUIController::OnMouseClick( EButton eButton )
{
	if ( !bIsShowing )
		return FALSE;

	D3DVIEWPORT9 pViewPortGet;
	D3DVIEWPORT9 pViewPortSet;

	DX::Graphic::GetDevice()->GetViewport( &pViewPortGet );

	// Set viewport to elements
	{

		ZeroMemory( &pViewPortSet, sizeof( pViewPortSet ) );
		pViewPortSet.X = pViewPortGet.X + r.x;
		pViewPortSet.Y = pViewPortGet.Y + r.y;
		pViewPortSet.Width = r.w;
		pViewPortSet.Height = r.h;
		DX::Graphic::GetDevice()->SetViewport( &pViewPortSet );
		
		for ( auto element : CUIElement::vElementsChild )
		{
			if ( element->OnMouseClick( eButton ) )
			{
				DX::Graphic::GetDevice()->SetViewport( &pViewPortGet );
				return TRUE;
			}
		}
		
		if ( pScrollControl )
		{
			if ( pScrollControl->OnMouseClick( eButton ) )
			{
				DX::Graphic::GetDevice()->SetViewport( &pViewPortGet );
				return TRUE;
			}
		}

		if ( UI::pGame::IsMousePosition( r.x, r.y, r.w, r.h ) )
		{
			if ( eButton == WM_LBUTTONDOWN )
			{
				ZeroMemory( &vMousePosInitial, sizeof( D3DXVECTOR2 ) );
				UI::pGame::GetMousePos( &vMousePosInitial );
				vMousePosInitial.x -= ( float )pViewPortSet.X;
				vMousePosInitial.y -= ( float )pViewPortSet.Y;
				IsClicked = TRUE;
			}
		}
		if ( eButton == WM_LBUTTONUP )
			IsClicked = FALSE;

		if ( IsClicked )
		{
			DX::Graphic::GetDevice()->SetViewport( &pViewPortGet );
			return TRUE;
		}
	}
	DX::Graphic::GetDevice()->SetViewport( &pViewPortGet );
	return IsNotGlobalClick;
}

BOOL CUIController::OnMouseMove()
{
	if ( !bIsShowing )
		return FALSE;

	D3DVIEWPORT9 pViewPortGet;
	D3DVIEWPORT9 pViewPortSet;

	DX::Graphic::GetDevice()->GetViewport( &pViewPortGet );

	// Set viewport to elements
	{

		ZeroMemory( &pViewPortSet, sizeof( pViewPortSet ) );
		pViewPortSet.X = pViewPortGet.X + r.x;
		pViewPortSet.Y = pViewPortGet.Y + r.y;
		pViewPortSet.Width = r.w;
		pViewPortSet.Height = r.h;
		DX::Graphic::GetDevice()->SetViewport( &pViewPortSet );

		for ( auto element : CUIElement::vElementsChild )
		{
			if ( element->OnMouseMove() )
			{
				DX::Graphic::GetDevice()->SetViewport( &pViewPortGet );
				return TRUE;
			}
		}
		if ( pScrollControl )
		{
			if ( pScrollControl->OnMouseMove() )
			{
				ReposRect();
				DX::Graphic::GetDevice()->SetViewport( &pViewPortGet );
				return TRUE;
			}
		}
		if ( IsFreeMove && IsClicked )
		{
			ZeroMemory( &vMousePos, sizeof( D3DXVECTOR2 ) );
			UI::pGame::GetMousePos( &vMousePos );
			vMousePos.x -= ( float )pViewPortSet.X;
			vMousePos.y -= ( float )pViewPortSet.Y;
			r.x = ( int )vMousePos.x - ( int )vMousePosInitial.x + pViewPortSet.X;
			r.y = ( int )vMousePos.y - ( int )vMousePosInitial.y + pViewPortSet.Y;
			if ( r.x < 0 )
				r.x = 0;
			if ( r.y < 0 )
				r.y = 0;
			if ( pScrollControl )
				pScrollControl->SetRect( r );
		}
	}

	DX::Graphic::GetDevice()->SetViewport( &pViewPortGet );

	return FALSE;
}

BOOL CUIController::OnMouseWheel( int iDelta )
{
	if ( !bIsShowing )
		return FALSE;

	D3DVIEWPORT9 pViewPortGet;
	D3DVIEWPORT9 pViewPortSet;

	DX::Graphic::GetDevice()->GetViewport( &pViewPortGet );

	// Set viewport to elements
	{

		ZeroMemory( &pViewPortSet, sizeof( pViewPortSet ) );
		pViewPortSet.X		= pViewPortGet.X + r.x;
		pViewPortSet.Y		= pViewPortGet.Y + r.y;
		pViewPortSet.Width	= r.w;
		pViewPortSet.Height = r.h;
		DX::Graphic::GetDevice()->SetViewport( &pViewPortSet );

		if ( UI::pGame::IsMousePosition( pViewPortSet.X, pViewPortSet.Y, pViewPortSet.Width, pViewPortSet.Height ) )
		{
			for ( auto element : CUIElement::vElementsChild )
			{
				if ( element->OnMouseWheel( iDelta ) )
				{
					DX::Graphic::GetDevice()->SetViewport( &pViewPortGet );
					return TRUE;
				}
			}
			if ( pScrollControl )
			{
				pScrollControl->OnMouseWheel( iDelta );
				ReposRect();
				DX::Graphic::GetDevice()->SetViewport( &pViewPortGet );
				return TRUE;
			}
		}
	}
	DX::Graphic::GetDevice()->SetViewport( &pViewPortGet );
	return FALSE;
}

BOOL CUIController::OnKeyChar( char cChar )
{
	if ( !bIsShowing )
		return FALSE;

	for ( auto element : CUIElement::vElementsChild )
	{
		if ( element->OnKeyChar( cChar ) )
		{
			return TRUE;
		}
	}
	return FALSE;
}