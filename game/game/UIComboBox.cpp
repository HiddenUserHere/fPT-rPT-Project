#include "stdafx.h"
#include "UIComboBox.h"

namespace UI
{
ComboBox::ComboBox( Rectangle2D rRect ) : Element( rRect )
{
	pTextValue = std::make_shared<Text>( "", Rectangle2D( 0, 0, rRect.iWidth - 22, rRect.iHeight ), FALSE, -1 );
	pTextValue->SetVerticalAlign( ALIGN_Middle );
	pTextValue->SetHorizontalAlign( ALIGN_Center );
}

ComboBox::~ComboBox()
{
}

Text_ptr ComboBox::MakeOption( const std::string & strText, const Rectangle2D & rBox, BOOL bBold, const DWORD dwColor )
{
	Text_ptr pText = std::make_shared<Text>( strText, rBox, bBold, dwColor );
	pText->SetNoClip( TRUE );

	return pText;
}

void ComboBox::AddOption( Text_ptr p )
{
	p->SetBox( Rectangle2D( p->GetX(), p->GetY() + GetHeight() + ( vTextOptions.size() * p->GetHeight() ), p->GetWidth(), p->GetHeight() ) );
	p->SetID( vTextOptions.size() + 1 );
	p->SetHorizontalAlign( ALIGN_Center );
	p->SetVerticalAlign( ALIGN_Middle );
	vTextOptions.push_back( p );

	iHeightMenu += p->GetHeight();
}

void ComboBox::AddOption( Text_ptr p, int iID )
{
	p->SetBox( Rectangle2D( p->GetX(), p->GetY() + GetHeight() + (vTextOptions.size() * p->GetHeight()), p->GetWidth(), p->GetHeight() ) );
	p->SetID( iID );
	p->SetHorizontalAlign( ALIGN_Center );
	p->SetVerticalAlign( ALIGN_Middle );
	vTextOptions.push_back( p );

	iHeightMenu += p->GetHeight();
}

void ComboBox::SetBackgroundImage( std::pair<class Image *, class Image *> pPair )
{
	pBackgroundImage		= pPair.first;
	pBackgroundActiveImage	= pPair.second;
}

void ComboBox::SetBackgroundActiveImage( std::pair<class Image *, class Image *> pPair )
{
	pBackgroundActiveMiddleImage = pPair.first;
	pBackgroundActiveBottomImage = pPair.second;
}

void ComboBox::Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	int iRenderX		= GetX() + iX;
	int iRenderY		= GetY() + iY;
	int iRenderWidth	= low( GetWidth(), iWidth );
	int iRenderHeight	= low( GetHeight(), iHeight );

	//Render Background Image
	if( !bShowMenu )
	{
		if( bHover )
		{
			if( pBackgroundActiveImage )
				UI::ImageRender::Render( pBackgroundActiveImage, iRenderX, iRenderY, iRenderWidth, iRenderHeight, iSourceX, iSourceY, -1 );
		}
		else
		{
			if( pBackgroundImage )
				UI::ImageRender::Render( pBackgroundImage, iRenderX, iRenderY, iRenderWidth, iRenderHeight, iSourceX, iSourceY, -1 );
		}
	}
	else
	{
		if( pBackgroundActiveTopImage )
			UI::ImageRender::Render( pBackgroundActiveTopImage, iRenderX, iRenderY, iRenderWidth, iRenderHeight, iSourceX, iSourceY, -1 );

		if( pBackgroundActiveMiddleImage )
			UI::ImageRender::Render( pBackgroundActiveMiddleImage, iRenderX, iRenderY + pBackgroundActiveImage->GetHeight(), iRenderWidth, iHeightMenu - pBackgroundActiveBottomImage->GetHeight() + 5, iSourceX, iSourceY, -1 );
	
		if( pBackgroundActiveBottomImage )
			UI::ImageRender::Render( pBackgroundActiveBottomImage, iRenderX, iRenderY + pBackgroundActiveImage->GetHeight() + iHeightMenu - pBackgroundActiveBottomImage->GetHeight() + 5, iRenderWidth, pBackgroundActiveBottomImage->GetHeight(), iSourceX, iSourceY, -1 );
	}

	//Render Value Text
	if( pTextValue )
		pTextValue->Render( iRenderX, iRenderY, iRenderWidth, iRenderHeight, iSourceX, iSourceY );

	//Show Options from Menu
	if( bShowMenu )
	{
		GRAPHICENGINE->DrawRectangle2D( rOptionPosition, dwHoverColor );

		for( auto &v : vTextOptions )
		{
			v->Render( GetX() + iX, GetY() + iY, iWidth, iHeight, iSourceX, iSourceY );
		}
	}

	Element::Render( iRenderX, iRenderY, iRenderWidth, iRenderHeight, iSourceX, iSourceY );
}

BOOL ComboBox::OnMouseClick( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	if( !IsOpen() )
		return FALSE;

	int iRenderX		= GetX() + iX;
	int iRenderY		= GetY() + iY;
	int iRenderWidth	= low( GetWidth(), iWidth );
	int iRenderHeight	= low( GetHeight(), iHeight );

	Rectangle2D rBox = { iRenderX, iRenderY, iRenderWidth, iRenderHeight };
	Rectangle2D rMenu = { iRenderX, iRenderY + iRenderHeight, iRenderWidth, iHeightMenu };

	if( pcMouse->GetEvent() == ClickDownL && rBox.Inside( pcMouse->GetPosition() ) )
	{
		bShowMenu = TRUE;
		return TRUE;
	}
	else if( pcMouse->GetEvent() == ClickDownL && bShowMenu && rMenu.Inside( pcMouse->GetPosition() ) )
	{
		//Invoke Change Event
		if( iEventID != -1 )
			UI::Event::Invoke( iEventID );

		//Update Value
		pTextValue->SetText( GetTextOption( iSelectedID ) );

		bShowMenu = FALSE;
		return TRUE;
	}
	else if( pcMouse->GetEvent() == ClickDownL )
		bShowMenu = FALSE;

	return FALSE;
}

void ComboBox::OnMouseMove( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	int iRenderX = GetX() + iX;
	int iRenderY = GetY() + iY;
	int iRenderWidth = low( GetWidth(), iWidth );
	int iRenderHeight = low( GetHeight(), iHeight );

	//Show Options from Menu
	if( bShowMenu )
	{
		//Set SelecteID to default
		iSelectedID = -1;
		rOptionPosition = { 0, 0, 0, 0 };

		for( auto &v : vTextOptions )
		{
			Rectangle2D rBox = { iRenderX + v->GetX(), iRenderY + v->GetY(), v->GetWidth(), v->GetHeight() };
			if( rBox.Inside( pcMouse->GetPosition() ) )
			{
				rOptionPosition = rBox;
				iSelectedID = v->GetID();
				break;
			}
		}
	}
	else
	{
		Rectangle2D rBox = { iRenderX, iRenderY, iRenderWidth, iRenderHeight };

		if( rBox.Inside( pcMouse->GetPosition() ) )
			bHover = TRUE;
		else
			bHover = FALSE;
	}
}
void ComboBox::Clear()
{
	for ( std::vector<Text_ptr>::iterator it = vTextOptions.begin(); it != vTextOptions.end(); it++ )
	{
		(*it)->Clear();
		(*it) = nullptr;
	}

	vTextOptions.clear();


	CLEARPTR( pBackgroundImage );
	CLEARPTR( pBackgroundActiveImage );
	CLEARPTR( pBackgroundActiveTopImage );
	CLEARPTR( pBackgroundActiveMiddleImage );
	CLEARPTR( pBackgroundActiveBottomImage );
	UI::ImageLoader::DeleteImage( pBackgroundImage );
	UI::ImageLoader::DeleteImage( pBackgroundActiveImage );
	UI::ImageLoader::DeleteImage( pBackgroundActiveTopImage );
	UI::ImageLoader::DeleteImage( pBackgroundActiveMiddleImage );
	UI::ImageLoader::DeleteImage( pBackgroundActiveBottomImage );

	CLEARPTR( pTextValue );

	pTextValue = nullptr;
}
}