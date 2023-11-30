#include "stdafx.h"
#include "UITooltip.h"

namespace UI
{
Tooltip::Tooltip()
{
}

Tooltip::Tooltip( Rectangle2D sBox ) : Element(sBox)
{
	pLinkObject		= NULL;
	bHover			= FALSE;
	sObjectBox		= Rectangle2D();
	pText			= NULL;
	bTransition		= FALSE;

	SetType( TYPE_Tooltip );
}

Tooltip::~Tooltip()
{
}

void Tooltip::Init()
{
	pBorderImages[0] = ImageLoader::LoadImage( "game\\images\\UI\\tooltip\\bottomleft.png" );
	pBorderImages[1] = ImageLoader::LoadImage( "game\\images\\UI\\tooltip\\bottomright.png" );
	pBorderImages[2] = ImageLoader::LoadImage( "game\\images\\UI\\tooltip\\left.png" );
	pBorderImages[3] = ImageLoader::LoadImage( "game\\images\\UI\\tooltip\\top.png" );
	pBorderImages[4] = ImageLoader::LoadImage( "game\\images\\UI\\tooltip\\topleft.png" );
	pBorderImages[5] = ImageLoader::LoadImage( "game\\images\\UI\\tooltip\\topright.png" );
}

void Tooltip::SetLinkObject( Element_ptr pObject )
{
	//Link Object
	pLinkObject = pObject;
}

void Tooltip::SetFont( char * pszFontName, int iFontSize, DWORD dwColor )
{
	pText = std::make_shared<UI::Text>( Rectangle2D( 0, 0, rRect.iWidth, rRect.iHeight ) );
	pText->SetFont( pszFontName, iFontSize, 0, FALSE, FALSE, dwColor );
	pText->SetMultiLine( TRUE );
	pText->SetWordWrap( TRUE );
}

void Tooltip::SetText( std::string strText )
{
	if( pText )
	{
		pText->SetText( strText );

		iWidthText = pText->GetTextWidth();
		iHeightText = pText->GetHeightText();
	}
}

void Tooltip::Open()
{
	bTransition = TRUE;
	fProgress = 1.0f;

	iFadeType = 1;
}

void Tooltip::Close()
{
	bTransition = TRUE;
	fProgress = 0.0f;

	iFadeType = 2;
}

void Tooltip::Update( float fTime )
{
	/*if( bTransition )
	{
		if( iFadeType == 1 )
		{
			fProgress += fTime;

			if( fProgress > 1.0f )
			{
				fProgress = 1.0f;
				iFadeType = 0;
				bTransition = FALSE;
			}
		}
		else
		{
			fProgress -= fTime;

			if( fProgress < 0.0f )
			{
				fProgress = 0.0f;
				iFadeType = 0;
				bTransition = FALSE;
			}
		}
	}*/
}

void Tooltip::Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	if ( !IsOpen() )
		return;

	if ( pLinkObject )
		if ( !pLinkObject->IsOpen() )
			return;

	int iRenderX = GetX() + iX + (pLinkObject ? pLinkObject->GetX() : 0);
	int iRenderY = GetY() + iY + (pLinkObject ? pLinkObject->GetY() : 0);
	int iRenderWidth = low( (pLinkObject ? pLinkObject->GetWidth() : GetWidth() ), iWidth - GetX() );
	int iRenderHeight = low( (pLinkObject ? pLinkObject->GetHeight() : GetHeight() ), iHeight - GetY() );

	sObjectBox = Rectangle2D( iRenderX, iRenderY, iRenderWidth, iRenderHeight );

	if( !bHover )
		return;

	int iAddX = 0;
	int iAddY = 0;

	if( ePosition == TOOLTIPPOSITION_Top )
	{
		iAddY -= iHeightText + 8;
		iAddX = (iRenderWidth >> 1) - (iWidthText >> 1);
	}
	else if( ePosition == TOOLTIPPOSITION_Bottom )
	{
		iAddY = iRenderHeight;
		iAddX = (iRenderWidth >> 1) - (iWidthText >> 1);
	}
	else if( ePosition == TOOLTIPPOSITION_Right )
		iAddX = iRenderWidth + 2;

	if( pText )
	{
		if( bFollowMouse )
		{
			GRAPHICENGINE->DrawRectangle2D( Rectangle2D( MOUSEHANDLER->GetPosition()->iX - ( iWidthText >> 1 ), MOUSEHANDLER->GetPosition()->iY + ( iAddY * (int)fProgress ), iWidthText, iHeightText ), D3DCOLOR_ARGB( 200, 0, 0, 0 ) );
			pText->Render( MOUSEHANDLER->GetPosition()->iX - ( iWidthText >> 1 ), MOUSEHANDLER->GetPosition()->iY + ( iAddY * (int)fProgress ), iWidth, iHeight, 0, 0 );
		}
		else
		{
			int iX = sObjectBox.iX + iAddX;
			int iY = sObjectBox.iY + (iAddY * (int)fProgress);

			iX += iAdditionalX;
			iY += iAdditionalY;

			if ( iX <= 0 )
				iX = 2;

			if ( iY <= 0 )
				iY = 2;

			if ( (iX + iWidthText) > RESOLUTION_WIDTH )
				iX = RESOLUTION_WIDTH - iWidthText - 2;

			if( pBorderImages[0] )
				UI::ImageRender::Render( pBorderImages[0], iX - pBorderImages[0]->GetWidth(), iY + iHeightText, pBorderImages[0]->GetWidth(), pBorderImages[0]->GetHeight(), -1 );

			if( pBorderImages[1] )
				UI::ImageRender::Render( pBorderImages[1], iX + iWidthText, iY + iHeightText, pBorderImages[1]->GetWidth(), pBorderImages[1]->GetHeight(), -1 );

			if( pBorderImages[2] )
				UI::ImageRender::Render( pBorderImages[2], iX - pBorderImages[2]->GetWidth(), iY, pBorderImages[2]->GetWidth(), iHeightText, -1 );

			if( pBorderImages[2] )
				UI::ImageRender::Render( pBorderImages[2], iX + iWidthText, iY, pBorderImages[2]->GetWidth(), iHeightText, -1 );

			if( pBorderImages[3] )
				UI::ImageRender::Render( pBorderImages[3], iX, iY - pBorderImages[3]->GetHeight(), iWidthText, pBorderImages[3]->GetHeight(), -1 );

			if( pBorderImages[3] )
				UI::ImageRender::Render( pBorderImages[3], iX, iY + iHeightText, iWidthText, pBorderImages[3]->GetHeight(), -1 );

			if( pBorderImages[4] )
				UI::ImageRender::Render( pBorderImages[4], iX - pBorderImages[4]->GetWidth(), iY - pBorderImages[4]->GetHeight(), pBorderImages[4]->GetWidth(), pBorderImages[4]->GetHeight(), -1 );

			if( pBorderImages[5] )
				UI::ImageRender::Render( pBorderImages[5], iX + iWidthText, iY - pBorderImages[5]->GetHeight(), pBorderImages[5]->GetWidth(), pBorderImages[5]->GetHeight(), -1 );

			GRAPHICENGINE->DrawRectangle2D( Rectangle2D( iX - 1, iY - 1, iWidthText + 1, iHeightText + 1 ), D3DCOLOR_ARGB( 200, 0, 0, 0 ) );
			pText->Render( iX, iY, iWidth, iHeight, 0, 0 );
		}
	}
}

BOOL Tooltip::OnMouseClick( CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	return FALSE;
}

void Tooltip::OnMouseMove( CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	if( sObjectBox.Inside( pcMouse->GetPosition() ) )
	{
		bHover = TRUE;

		if( fProgress == 0.0f )
			Open();

		return;
	}

	if( fProgress == 1.0f )
		Close();

	if( fProgress == 0.0f )
		bHover = FALSE;
}
}