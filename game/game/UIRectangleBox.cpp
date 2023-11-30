#include "stdafx.h"
#include "UIRectangleBox.h"

namespace UI
{

RectangleBox::RectangleBox( Rectangle2D rRectangle ) : Element( rRectangle )
{
	//Load Border Images
	pImageBorderLeftTop		= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\topleft.png" );
	pImageBorderRightTop	= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\topright.png" );
	pImageBorderTop			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\top.png" );
	pImageBorderRightBottom	= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\bottomright.png" );
	pImageBorderLeftBottom	= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\bottomleft.png" );
	pImageBorderBottom		= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\bottom.png" );
	pImageBorderLeft		= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\left.png" );
	pImageBorderRight		= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\right.png" );

	//Load Images
	pImageMaskLeftTop		= ImageLoader::LoadImage( "game\\images\\UI\\box\\mask\\topleft.png" );
	pImageMaskRightTop		= ImageLoader::LoadImage( "game\\images\\UI\\box\\mask\\topright.png" );
	pImageMaskLeftBottom	= ImageLoader::LoadImage( "game\\images\\UI\\box\\mask\\bottomleft.png" );
	pImageMaskRightBottom	= ImageLoader::LoadImage( "game\\images\\UI\\box\\mask\\bottomright.png" );

	pRenderTargetMask		= GetGraphicsDevice()->MakeRenderTarget();
	pEffectMask				= GetGraphicsDevice()->GetEffectFactory()->MakeEffect( "game\\scripts\\shaders\\UI\\InfoBox.fx" );

	cColor1 = DX::Color( 255, 255, 255, 255 );
	cColor2 = DX::Color( 255, 255, 255, 255 );
}

RectangleBox::~RectangleBox()
{
}

void RectangleBox::Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	if ( IsOpen() )
	{
		int iRenderX = GetX() + iX;
		int iRenderY = GetY() + iY;
		int iRenderWidth = low( GetWidth(), iWidth );
		int iRenderHeight = low( GetHeight(), iHeight );

		DrawInfoBoxMask( iRenderX, iRenderY, iRenderWidth, iRenderHeight );

		//Drawing Border
		UI::ImageRender::Render( pImageBorderLeftTop, iRenderX, iRenderY - 10, pImageBorderLeftTop->GetWidth(), pImageBorderLeftTop->GetHeight(), -1 );
		UI::ImageRender::Render( pImageBorderTop, iRenderX + pImageBorderLeftTop->GetWidth(), iRenderY - 10, iRenderWidth - pImageBorderLeftTop->GetWidth() - pImageBorderRightTop->GetWidth(), pImageBorderTop->GetHeight(), -1 );
		UI::ImageRender::Render( pImageBorderRightTop, iRenderX + pImageBorderLeftTop->GetWidth() + iRenderWidth - pImageBorderLeftTop->GetWidth() - pImageBorderRightTop->GetWidth(), iRenderY - 10, pImageBorderRightTop->GetWidth(), pImageBorderRightTop->GetHeight(), -1 );
		UI::ImageRender::Render( pImageBorderLeft, iRenderX, iRenderY - 10 + pImageBorderLeftTop->GetHeight(), pImageBorderLeft->GetWidth(), iRenderHeight - pImageBorderLeftTop->GetWidth() - pImageBorderLeftBottom->GetWidth(), -1 );
		UI::ImageRender::Render( pImageBorderRight, iRenderX + iRenderWidth - pImageBorderRight->GetWidth(), iRenderY - 10 + pImageBorderLeftTop->GetHeight(), pImageBorderRight->GetWidth(), iRenderHeight - pImageBorderLeftTop->GetWidth() - pImageBorderLeftBottom->GetWidth(), -1 );
		UI::ImageRender::Render( pImageBorderLeftBottom, iRenderX, iRenderY - 10 + iRenderHeight - pImageBorderLeftBottom->GetHeight(), pImageBorderLeftBottom->GetWidth(), pImageBorderLeftBottom->GetHeight(), -1 );
		UI::ImageRender::Render( pImageBorderRightBottom, iRenderX + iRenderWidth - pImageBorderRightBottom->GetWidth(), iRenderY - 10 + iRenderHeight - pImageBorderRightBottom->GetHeight(), pImageBorderRightBottom->GetWidth(), pImageBorderRightBottom->GetHeight(), -1 );
		UI::ImageRender::Render( pImageBorderBottom, iRenderX + pImageBorderLeftBottom->GetWidth(), iRenderY - 10 + iRenderHeight - pImageBorderBottom->GetHeight(), iRenderWidth - pImageBorderLeftTop->GetWidth() - pImageBorderRightTop->GetWidth(), pImageBorderBottom->GetHeight(), -1 );

		Element::Render( iRenderX, iRenderY - 10, iRenderWidth, iRenderHeight, iSourceX, iSourceY );
	}
}

void RectangleBox::SetColor( DX::Color c1, DX::Color c2 )
{
	cColor1.Set( c1.Get() );
	cColor2.Set( c2.Get() );
}


void RectangleBox::DrawInfoBoxMask( int iX, int iY, int iWidth, int iHeight )
{
	//Draw Item Info Box Background
	if ( pRenderTargetMask )
	{
		if ( GetGraphicsDevice()->SetRenderTarget( pRenderTargetMask ) )
		{
			//Clear Render Target
			GetGraphicsDevice()->Clear( TRUE, TRUE, FALSE );

			//Draw Background
			GRAPHICENGINE->DrawRectangle2D( Rectangle2D( 0, 0, iWidth, iHeight ), cColor1.Get(), cColor2.Get() );

			//Drawing Mask Border
			UI::ImageRender::Render( pImageMaskLeftTop, 0, 0, pImageMaskLeftTop->GetWidth(), pImageMaskLeftTop->GetHeight(), -1 );
			UI::ImageRender::Render( pImageMaskRightTop, iWidth - pImageMaskRightTop->GetWidth(), 0, pImageMaskRightTop->GetWidth(), pImageMaskRightTop->GetHeight(), -1 );
			UI::ImageRender::Render( pImageMaskLeftBottom, 0, iHeight - pImageMaskLeftBottom->GetHeight(), pImageMaskLeftBottom->GetWidth(), pImageMaskLeftBottom->GetHeight(), -1 );
			UI::ImageRender::Render( pImageMaskRightBottom, iWidth - pImageMaskRightTop->GetWidth(), iHeight - pImageMaskLeftBottom->GetHeight(), pImageMaskRightBottom->GetWidth(), pImageMaskRightBottom->GetHeight(), -1 );

			//Back Old Render Target
			GetGraphicsDevice()->UnsetRenderTarget();
		}

		//Set Sprite Effect...
		UI::ImageRender::SetEffect( pEffectMask );
		pEffectMask->SetTechnique( "Mask" );

		if ( pEffectMask->Begin() >= 0 )
		{
			//Render the Render Target and mask it on effect
			UI::ImageRender::Render( pRenderTargetMask->GetTexture(), iX, iY - 10, pRenderTargetMask->GetWidth(), pRenderTargetMask->GetHeight(), 0, 0, D3DCOLOR_XRGB( 255, 255, 255 ), FALSE );

			pEffectMask->End();
		}

		UI::ImageRender::SetEffect( NULL );
	}
}

};