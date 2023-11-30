#include "stdafx.h"
#include "UIScroll.h"

namespace UI
{
Scroll::Scroll(Rectangle2D rRectangle) : Element(rRectangle)
{
	rScroll = new Rectangle2D( 0, 0, 0, 0 );
}

Scroll::~Scroll()
{
	DELET( rScroll );
}

void Scroll::Clear()
{
	CLEARPTR( pImage );
	CLEARPTR( pImageScroller );
	CLEARPTR( pImageBackground );
	UI::ImageLoader::DeleteImage( pImage );
	UI::ImageLoader::DeleteImage( pImageScroller );
	UI::ImageLoader::DeleteImage( pImageBackground );

	DELET( rScroll );
}

void Scroll::Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	if ( IsOpen() )
	{
		int iRenderX		= GetX() + iX;
		int iRenderY		= GetY() + iY;
		int iRenderWidth	= (int)((float)GetWidth() * Element::GetScale());
		int iRenderHeight	= (int)((float)GetHeight() * Element::GetScale());

		int iScaleWidthScroller = (int)((float)iWidthScroller * Element::GetScale());
		int iScaleHeightScroller = (int)((float)iHeightScroller * Element::GetScale());

		int iHalfWidthScroller = iScaleWidthScroller >> 1;
		int iHalfHeightScroller = iScaleHeightScroller >> 1;

		UI::ImageRender::Render( pImage, iRenderX + iSourceX, iRenderY + iSourceY, GetWidth(), GetHeight(), iSourceX, iSourceY, -1, Element::GetScale() );

		int iPercent		= (bVertical ? rScroll->iY : rScroll->iX);
		
		// Background render?
		if ( pImageBackground )
		{
			int iWidthBackground	= bVertical ? iRenderWidth : (iPercent + iHalfWidthScroller);
			int iHeightBackground	= bVertical ? (iPercent + iHalfHeightScroller) : iRenderHeight;
			UI::ImageRender::Render( pImageBackground, iRenderX + iSourceX, iRenderY + iSourceY, iWidthBackground, iHeightBackground, iSourceX, iSourceY, -1, Element::GetScale() );
		}

		// Scroller render?
		if ( pImageScroller )
		{
			int iXScroller	= bVertical ? ((iRenderWidth >> 1) - iHalfWidthScroller) : 0;
			int iYScroller	= bVertical ? 0 : ((iRenderHeight >> 1) - iHalfHeightScroller);
			UI::ImageRender::Render( pImageScroller, iRenderX + iSourceX + iXScroller + rScroll->iX, iRenderY + iSourceY + iYScroller + rScroll->iY, iScaleWidthScroller, iScaleHeightScroller, iSourceX, iSourceY, -1, Element::GetScale() );
		}

		Element::Render( iRenderX, iRenderY, iWidth, iHeight, iSourceX, iSourceY );
	}
}

BOOL Scroll::OnMouseClick( CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	if ( !IsOpen() )
		return FALSE;

	EMouseEvent eEvent = pcMouse->GetEvent();

	if ( EMouseEvent::ClickDownL != eEvent && EMouseEvent::ClickUpL != eEvent )
		return FALSE;
	if ( eEvent == EMouseEvent::ClickUpL && bScrollable )
	{
		bScrollable = FALSE;
		return TRUE;
	}

	Point2D * psPosition = pcMouse->GetPosition();

	int iRenderX		= GetX() + iX + iSourceX;
	int iRenderY		= GetY() + iY + iSourceY;
	int iRenderWidth = (int)((float)GetWidth() * Element::GetScale());
	int iRenderHeight = (int)((float)GetHeight() * Element::GetScale());

	// Inside X?
	if ( (psPosition->iX >= iRenderX) && (psPosition->iX <= (iRenderX + iRenderWidth - iSourceX)) )
	{
		// Inside Y?
		if ( (psPosition->iY >= iRenderY) && (psPosition->iY <= (iRenderY + iRenderHeight - iSourceY)) )
		{
			if ( eEvent == ClickDownL )
				bScrollable = TRUE;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL Scroll::OnMouseScroll( CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	if ( !IsOpen() )
		return FALSE;

	Point2D * psPosition = pcMouse->GetPosition();

	int iRenderX		= GetX() + iX + iSourceX;
	int iRenderY		= GetY() + iY + iSourceY;
	int iRenderWidth	= (int)((float)GetWidth() * Element::GetScale());
	int iRenderHeight	= (int)((float)GetHeight() * Element::GetScale());

	int iScaleWidthScroller = (int)((float)iWidthScroller * Element::GetScale());
	int iScaleHeightScroller = (int)((float)iHeightScroller * Element::GetScale());

	int iHalfWidthScroller = iScaleWidthScroller >> 1;
	int iHalfHeightScroller = iScaleHeightScroller >> 1;

	int iXScrollBar = iRenderX + iHalfWidthScroller;
	int iYScrollBar = iRenderY + iHalfHeightScroller;

	if ( bVertical )
	{
		rScroll->iY = rScroll->iY - (pcMouse->GetDistance() * 16);
		if ( rScroll->iY > (iRenderHeight - iHalfHeightScroller) )
			rScroll->iY = (int)((float)iRenderHeight - (float)iHalfHeightScroller);
		else if ( rScroll->iY < 0 )
			rScroll->iY = 0;
	}

	return TRUE;
}

void Scroll::OnMouseMove( CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	if ( !IsOpen() || !bScrollable )
		return; 

	Point2D * psPosition = pcMouse->GetPosition();

	int iRenderX		= GetX() + iX + iSourceX;
	int iRenderY		= GetY() + iY + iSourceY;
	int iRenderWidth	= (int)((float)GetWidth() * Element::GetScale());
	int iRenderHeight	= (int)((float)GetHeight() * Element::GetScale());

	int iScaleWidthScroller = (int)((float)iWidthScroller * Element::GetScale());
	int iScaleHeightScroller = (int)((float)iHeightScroller * Element::GetScale());

	int iHalfWidthScroller = iScaleWidthScroller >> 1;
	int iHalfHeightScroller = iScaleHeightScroller >> 1;

	int iXScrollBar = iRenderX + iHalfWidthScroller;
	int iYScrollBar = iRenderY + iHalfHeightScroller;

	if ( bVertical )
	{
		rScroll->iY = psPosition->iY - iYScrollBar;
		if ( rScroll->iY > (iRenderHeight - iHalfHeightScroller) )
			rScroll->iY = (int)((float)iRenderHeight - (float)iHalfHeightScroller);
		else if ( rScroll->iY < 0 )
			rScroll->iY = 0;
	}
	else
	{
		rScroll->iX = psPosition->iX - iXScrollBar;
		if ( rScroll->iX > (iRenderWidth - iHalfWidthScroller) )
			rScroll->iX = (int)((float)iRenderWidth - (float)iHalfWidthScroller);
		else if ( rScroll->iX < 0 )
			rScroll->iX = 0;
	}
}

int Scroll::GetPercent()
{
	float fMinMax = fMaxScroll - fMinScroll;

	float fRet = 0;

	int iRenderWidth = (int)((float)GetWidth() * Element::GetScale());
	int iRenderHeight = (int)((float)GetHeight() * Element::GetScale());

	int iScaleWidthScroller = (int)((float)iWidthScroller * Element::GetScale());
	int iScaleHeightScroller = (int)((float)iHeightScroller * Element::GetScale());

	int iHalfWidthScroller = iScaleWidthScroller >> 1;
	int iHalfHeightScroller = iScaleHeightScroller >> 1;
	
	if ( bVertical )
		fRet = (((float)rScroll->iY * fMinMax) / ((float)iRenderHeight - (float)iHalfHeightScroller)) + fMinScroll;
	else
		fRet = (((float)rScroll->iX * fMinMax) / ((float)iRenderWidth - (float)iHalfWidthScroller)) + fMinScroll;

	return (int)round( fRet );
}

void Scroll::SetPercent(int iCur)
{
	float fMinMax = fMaxScroll - fMinScroll;
	if ( iCur >= (int)fMinScroll && iCur <= (int)fMaxScroll )
	{
		int iRenderWidth = (int)((float)GetWidth() * Element::GetScale());
		int iRenderHeight = (int)((float)GetHeight() * Element::GetScale());

		int iScaleWidthScroller = (int)((float)iWidthScroller * Element::GetScale());
		int iScaleHeightScroller = (int)((float)iHeightScroller * Element::GetScale());

		int iHalfWidthScroller = iScaleWidthScroller >> 1;
		int iHalfHeightScroller = iScaleHeightScroller >> 1;

		iCurrentPercent = iCur;
		if ( bVertical )
		{
			rScroll->iY = (int)round( abs( (((float)iRenderHeight - (float)iHalfHeightScroller) * (float)iCur) / fMinMax ) + fMinScroll );
			if ( rScroll->iY < 0 )
				rScroll->iY = 0;
		}
		else
		{
			rScroll->iX = (int)round( abs( (((float)iRenderWidth - (float)iHalfWidthScroller) * (float)iCur) / fMinMax ) + fMinScroll );
			if ( rScroll->iX < 0 )
				rScroll->iX = 0;
		}
	}
}
}