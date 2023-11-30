#include "stdafx.h"
#include "UICounter.h"

namespace UI
{
Counter::Counter( Rectangle2D rRectangle ) : Element(rRectangle)
{
	pInputField = std::make_shared<InputField>( Rectangle2D( 0, 0, rRectangle.iWidth, rRectangle.iHeight ) );
	pInputField->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pInputField->SetIsNumber( TRUE, iMin, iMax );
}

Counter::~Counter()
{
}

void Counter::Clear()
{
	CLEARPTR( pImage );
	CLEARPTR( pImageBtn1 );
	CLEARPTR( pImageBtn2 );
	UI::ImageLoader::DeleteImage( pImage );
	UI::ImageLoader::DeleteImage( pImageBtn1 );
	UI::ImageLoader::DeleteImage( pImageBtn2 );

	CLEARPTR( pInputField );

	pInputField = nullptr;
}

void Counter::SetCurrent( int i )
{
	iCurrent = i;
	if ( iCurrent > iMax )
		iCurrent = iMax;
	if ( iCurrent < iMin )
		iCurrent = iMin;

	pInputField->SetText( FormatString( "%d", iCurrent ) );
	UI::Event::Invoke( iEventID );
}

void Counter::Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
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

		UI::ImageRender::Render( pImage, iRenderX, iRenderY, iRenderWidth, iRenderHeight, iSourceX, iSourceY, -1 );
	
		pInputField->Render( iRenderX, iRenderY, iRenderWidth, iRenderHeight, iSourceX, iSourceY );

		if ( bHover || bSelect )
		{
			if ( pGroupMain )
			{
				if ( pGroupMain->GetIndexObject() != GetID() )
					bSelect = FALSE;
			}
			if ( bHoverBtn1 )
				UI::ImageRender::Render( pImageBtn1, iRenderX + rButton1.iX, iRenderY + rButton1.iY, rButton1.iWidth, rButton1.iHeight, -1 );
			if ( bHoverBtn2 )
				UI::ImageRender::Render( pImageBtn2, iRenderX + rButton2.iX, iRenderY + rButton2.iY, rButton2.iWidth, rButton2.iHeight, -1 );
		}

		Element::Render( iRenderX, iRenderY, iRenderWidth, iRenderHeight, iSourceX, iSourceY );
	}
}



BOOL Counter::OnMouseClick( CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	if ( !IsOpen() || bDisable )
		return FALSE;

	if ( eMouseEvent != pcMouse->GetEvent() )
		return FALSE;

	Point2D * psPosition = pcMouse->GetPosition();

	int iRenderX		= GetX() + iX + iSourceX;
	int iRenderY		= GetY() + iY + iSourceY;
	int iRenderWidth	= low( GetWidth(), iWidth );
	int iRenderHeight	= low( GetHeight(), iHeight );


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
			Rectangle2D rBox;
			rBox = Rectangle2D( iRenderX + rButton1.iX, iRenderY + rButton1.iY, rButton1.iWidth, rButton1.iHeight );
			if ( rBox.Inside( psPosition ) )
			{
				SetCurrent( GetCurrent() + 1 );
			}

			rBox = Rectangle2D( iRenderX + rButton2.iX, iRenderY + rButton2.iY, rButton2.iWidth, rButton2.iHeight );
			if ( rBox.Inside( psPosition ) )
			{
				SetCurrent( GetCurrent() - 1 );
			}
			pInputField->OnMouseClick( pcMouse, iX, iY, iWidth, iHeight, iSourceX, iSourceY );
			return TRUE;
		}
	}
	return FALSE;
}

void Counter::OnMouseMove( CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
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
			Rectangle2D rBox;
			rBox = Rectangle2D( iRenderX + rButton1.iX, iRenderY + rButton1.iY, rButton1.iWidth, rButton1.iHeight );
			if ( rBox.Inside( psPosition ) )
			{
				bHoverBtn1 = TRUE;
			}
			else
				bHoverBtn1 = FALSE;

			rBox = Rectangle2D( iRenderX + rButton2.iX, iRenderY + rButton2.iY, rButton2.iWidth, rButton2.iHeight );
			if ( rBox.Inside( psPosition ) )
			{
				bHoverBtn2 = TRUE;
			}
			else
				bHoverBtn2 = FALSE;

			bHover = TRUE;
			pInputField->OnMouseMove( pcMouse, iX, iY, iWidth, iHeight, iSourceX, iSourceY );
		}
	}
}

BOOL Counter::OnMouseScroll( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	if ( !IsOpen() )
		return FALSE;

	Point2D * psPosition = pcMouse->GetPosition();

	if ( bHover )
	{
		SetCurrent( GetCurrent() + pcMouse->GetDistance() );
		return TRUE;
	}
	return FALSE;
}

}
