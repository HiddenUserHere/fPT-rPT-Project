#include "stdafx.h"
#include "UIScroller.h"
#include "UIElement.h"
#include "UIGroup.h"
#include "UI.h"

/*
		Global Vars
*/
int					iScrollBarImageUI = -1;
int					iScrollTagImageUI = -1;
BOOL				bCreatedScrollImagesUI = FALSE;

CUIScroller::CUIScroller( Rectangle2D rectangle, BOOL bVertical )
{
	ZeroMemory( &rectScroll, sizeof( Rectangle2D ) );
	IsVertical	= bVertical;
	r			= rectangle;
}


CUIScroller::~CUIScroller()
{
}

void CUIScroller::SetMinMax( float fMin, float fMax )
{
	fMinScroll = fMin;
	fMaxScroll = fMax;
}

void CUIScroller::SetRect( Rectangle2D rectangle )
{
	r = rectangle;
}

void CUIScroller::SetImage( EImage eImageScrollBar, EImage eImageScroller, int iWidth, int iHeight )
{
	eImageScrollBarHandler	= eImageScrollBar;
	eImageScrollerHandler	= eImageScroller;
	iWidthScroller			= iWidth;
	iHeightScroller			= iHeight;
}

void CUIScroller::SetImageBackground( EImage eImageScrollBarBack )
{
	eImageScrollBarBackHandler = eImageScrollBarBack;
	bBackground = TRUE;
}

void CUIScroller::Draw()
{

	D3DVIEWPORT9 pViewPortGet;
	DX::Graphic::GetDevice()->GetViewport( &pViewPortGet );

	int iPercent = (IsVertical ? rectScroll.y : rectScroll.x);
	int iXScroller = 0;
	int iYScroller = 0;

	UI::ImageRender::Render( eImageScrollBarHandler, r.x, r.y, r.w, r.h, -1 );
	if ( bBackground )
	{
		D3DVIEWPORT9 pViewPortSet;
		ZeroMemory( &pViewPortSet, sizeof( pViewPortSet ) );
		pViewPortSet.X		= r.x + pViewPortGet.X;
		pViewPortSet.Y		= r.y + pViewPortGet.Y;
		pViewPortSet.Width	= IsVertical ? r.w : (iPercent + (iWidthScroller >> 1));
		pViewPortSet.Height = IsVertical ? (iPercent + (iHeightScroller >> 1)) : r.h;
		GRAPHICENGINE->GetDevice()->SetViewport( &pViewPortSet );
		
		UI::ImageRender::Render( eImageScrollBarBackHandler, 0, 0, r.w, r.h, -1 );
		
		GRAPHICENGINE->GetDevice()->SetViewport( &pViewPortGet );
	}

	if ( IsVertical )
	{
		iXScroller = (r.w >> 1) - (iWidthScroller >> 1);
		UI::ImageRender::Render( eImageScrollerHandler, r.x + iXScroller, r.y + iYScroller + iPercent, iWidthScroller, iHeightScroller, -1 );
	}
	else
	{
		iYScroller = (r.h >> 1) - (iHeightScroller >> 1);
		UI::ImageRender::Render( eImageScrollerHandler, r.x + iXScroller + iPercent, r.y + iYScroller, iWidthScroller, iHeightScroller, -1 );
	}
	char szText[64] = { 0 };
	wsprintfA( szText, "P: %d", GetScrollPercent() );
	FONTHANDLER->Draw( 0, r.x+ pViewPortGet.X, r.y + pViewPortGet.Y, szText, DT_WORDBREAK, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
}

BOOL CUIScroller::OnMouseClick( EButton eButton )
{
	D3DVIEWPORT9 pViewPortGet;
	DX::Graphic::GetDevice()->GetViewport( &pViewPortGet );

	int iXScrollBar = pViewPortGet.X + r.x;
	int iYScrollBar = pViewPortGet.Y + r.y;

	if ( eButton == WM_LBUTTONUP )
	{
		IsClicked = FALSE;
		return FALSE;
	}
	else if ( UI::pGame::IsMousePosition( iXScrollBar, iYScrollBar, r.w, r.h ) )
	{
		IsClicked = TRUE;
		return TRUE;
	}

	return FALSE;
}

BOOL CUIScroller::OnMouseMove()
{
	if ( !IsClicked )
		return FALSE;

	D3DVIEWPORT9 pViewPortGet;
	DX::Graphic::GetDevice()->GetViewport( &pViewPortGet );

	int iXScrollBar = pViewPortGet.X + r.x + (iWidthScroller >> 1);
	int iYScrollBar = pViewPortGet.Y + r.y + (iHeightScroller >> 1);

	D3DXVECTOR2 pVec;
	UI::pGame::GetMousePos( &pVec );

	if ( IsVertical )
	{
		rectScroll.y = (int)pVec.y - iYScrollBar;
		if ( rectScroll.y > (r.h - (iHeightScroller >> 1)) )
			rectScroll.y = (r.h - (iHeightScroller >> 1));
		else if ( rectScroll.y < 0 )
			rectScroll.y = 0;
	}
	else
	{
		rectScroll.x = (int)pVec.x - iXScrollBar;
		if ( rectScroll.x > (r.w - (iWidthScroller >> 1)) )
			rectScroll.x = (r.w - (iWidthScroller >> 1));
		else if ( rectScroll.x < 0 )
			rectScroll.x = 0;
	}
	return TRUE;
}

BOOL CUIScroller::OnMouseWheel( int iDelta )
{
	if ( !IsVertical )
		return FALSE;

	if ( iDelta == -120 )
		rectScroll.y += 15;
	else
		rectScroll.y -= 15;

	if ( rectScroll.y < 0 )
		rectScroll.y = 0;
	else if ( rectScroll.y > (r.h - (iHeightScroller >> 1)) )
		rectScroll.y = (r.h - (iHeightScroller >> 1));

	return TRUE;

}

int CUIScroller::GetScrollPercent()
{
	int iMinMax = (int)fMaxScroll - (int)fMinScroll;

	if ( IsVertical )
		return ((rectScroll.y * iMinMax) / (r.h - (iHeightScroller >> 1))) + (int)fMinScroll;
	return ((rectScroll.x * iMinMax) / (r.w - (iWidthScroller >> 1))) + (int)fMinScroll;
}
