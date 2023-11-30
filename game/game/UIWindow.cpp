#include "stdafx.h"
#include "UIWindow.h"

namespace UI
{
Window::Window( Rectangle2D rRectangle ) : Element( rRectangle ), rRectViewPort( rRectangle )
{
}

Window::~Window()
{
}

void Window::Clear()
{
	for ( std::vector<Element_ptr>::iterator it = vElementsChild.begin(); it != vElementsChild.end(); it++ )
	{
		Element_ptr p = *it;
		if ( p )
		{
			p->DecRef();
			if ( p->GetRef() == 0 )
			{
				p->Clear();
				p = nullptr;
			}
		}
	}
	
	vElementsChild.clear();

	UI::ImageLoader::DeleteImage( pImage );

	pImage = NULL;
}

void Window::SetBox( Rectangle2D sBox )
{
	rRectViewPort = sBox;
	rRect = sBox;
}

BOOL Window::IsHover()
{
	if ( !IsOpen() )
		bIsHover = FALSE;

	return bIsHover;
}

void Window::SetWindowScale( float fWindowScale )
{
	if ( GetUseSelfScale() )
		return;

	Element::SetScale( fWindowScale );
	for ( auto & v : vElementsChild )
	{
		if ( v->GetType() == TYPE_Window )
			std::dynamic_pointer_cast<UI::Window>(v)->SetWindowScale( fWindowScale );
		else if( v->GetType() == TYPE_List )
			std::dynamic_pointer_cast<UI::List>(v)->SetListScale( fWindowScale );
		else if ( v->GetUseSelfScale() == false )
			v->SetScale( fWindowScale );
	}
}

void Window::Update( float fTime )
{
	if( IsOpen() )
	{
		// Render Objects
		for( auto &v : vElementsChild )
		{
			if( v->GetType() == TYPE_Window )
				v->Update( fTime );
			else
				v->Update( fTime );
		}
	}
}

void Window::Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY, int iSourceWidth, int iSourceHeight )
{
	if ( IsOpen() )
	{
		//Fix Source iWidth and iHeight
		if ( iSourceWidth == -1 )
			iSourceWidth = (int)((float)GetWidth() * Element::GetScale());
		if ( iSourceHeight == -1 )
			iSourceHeight = (int)((float)GetHeight() * Element::GetScale());

		if ( iWidth <= 0 )
			return;

		if ( iHeight <= 0 )
			return;

		int iRenderX		= GetX() + iX;
		int iRenderY		= GetY() + iY;
		int iRenderWidth	= low( GetWidth(), iWidth );
		int iRenderHeight	= low( GetHeight(), iHeight );

		if( cColorBackground.Get() != 0 )
			GRAPHICENGINE->DrawRectangle2D( Rectangle2D( iRenderX, iRenderY, iRenderWidth, iRenderHeight ), cColorBackground.Get(), cColorBackground.Get() );

		if ( pImage )
		{
			UI::ImageRender::Render( pImage, iRenderX, iRenderY, pImage->GetWidth(), pImage->GetHeight(), iSourceX, iSourceY, -1, Element::GetScale() );
		}
		// Render Objects
		for ( const auto &v : vElementsChild )
		{
			if ( v->IsParentChild() )
			{
				if ( v->GetType() == TYPE_Window )
					v->Render( iRenderX, iRenderY, iRenderWidth, iRenderHeight, iSourceX, iSourceY, iSourceWidth, iSourceHeight );
				else
					v->Render( iRenderX, iRenderY, iRenderWidth, iRenderHeight, iSourceX, iSourceY );
			}
			else
			{
				if ( v->GetType() == TYPE_Window )
					v->Render( iRenderX, iRenderY, iRenderWidth, iRenderHeight, 0, 0, iSourceWidth, iSourceHeight );
				else
					v->Render( iRenderX, iRenderY, iRenderWidth, iRenderHeight, 0, 0 );
			}
		}

		// Scroll
		if ( pScrollMain )
			pScrollMain->Render( iRenderX, iRenderY, iRenderWidth, iRenderHeight, iSourceX, iSourceY );

		Element::Render( iRenderX, iRenderY, iRenderWidth, iRenderHeight, iSourceX, iSourceY );
	}
}


BOOL Window::OnMouseClick( CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	if ( !IsOpen() || !bMouseClickEnabled )
		return FALSE;

	Point2D * psPosition = pcMouse->GetPosition();

	BOOL bRet = FALSE;

	if( IsHover() && bFocusWindow )
		bRet = TRUE;

	int iRenderX		= GetX() + iX;
	int iRenderY		= GetY() + iY;
	int iRenderWidth	= low( GetWidth(), iWidth );
	int iRenderHeight	= low( GetHeight(), iHeight );
	Rectangle2D rBox	= { iRenderX, iRenderY, iRenderWidth, iRenderHeight };
	Rectangle2D rSelBox = { iRenderX + rRectSelectMove.iX, iRenderY + rRectSelectMove.iY, rRectSelectMove.iWidth, rRectSelectMove.iHeight };

	//Event
	if ( pcMouse->GetEvent() == eMouseEvent && rBox.Inside( psPosition ) )
		UI::Event::Invoke( iEventID );

	for ( UINT u = vElementsChild.size(); u > 0; u-- )
	{
		const auto &v = vElementsChild[u - 1];
		if ( v->OnMouseClick( pcMouse, iRenderX, iRenderY, iRenderWidth, iRenderHeight, 0, 0 ) )
		{
			bRet = TRUE;
			break;
		}
	}

	if ( pScrollMain )
		bRet = pScrollMain->OnMouseClick( pcMouse, iRenderX, iRenderY, iWidth, iHeight, 0, 0 );

	if ( bCanMove )
	{
		if ( bSelected )
		{
			if ( pcMouse->GetEvent() == ClickUpL )
				bSelected = FALSE;
		}
		else if ( rSelBox.Inside( psPosition ) )
		{
			if ( pcMouse->GetEvent() == ClickDownL )
			{
				bSelected = TRUE;
				sPointMouse.iX = abs( pcMouse->GetPosition()->iX - GetX() );
				sPointMouse.iY = abs( pcMouse->GetPosition()->iY - GetY() );
				bRet = TRUE;
			}
		}
	}

	return bRet;
}

BOOL Window::OnMouseScroll( CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	if ( !IsOpen() )
		return FALSE;

	//Compute iWidth and iHeight
	iWidth	-= GetX();
	iHeight	-= GetY();

	int iRenderX		= GetX() + iX;
	int iRenderY		= GetY() + iY;
	int iRenderWidth	= low( GetWidth(), iWidth );
	int iRenderHeight	= low( GetHeight(), iHeight );

	for ( UINT u = vElementsChild.size(); u > 0; u-- )
	{
		const auto &v = vElementsChild[u - 1];
		if ( v->OnMouseScroll( pcMouse, iRenderX + iSourceX, iRenderY + iSourceY, iRenderWidth - iSourceX, iRenderHeight - iSourceY, iSourceX, iSourceY ) )
			return TRUE;
	}

	return FALSE;
}

void Window::OnMouseMove( CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	if( !IsOpen() )
		return;

	//Compute iWidth and iHeight
	iWidth	-= GetX();
	iHeight	-= GetY();

	int iRenderX		= GetX() + iX;
	int iRenderY		= GetY() + iY;
	int iRenderWidth	= low( GetWidth(), iWidth );
	int iRenderHeight	= low( GetHeight(), iHeight );

	BOOL bOldHover = bIsHover;

	if( Rectangle2D( iRenderX, iRenderY, iRenderWidth, iRenderHeight ).Inside( pcMouse->GetPosition() ) )
		bIsHover = TRUE;
	else
		bIsHover = FALSE;

	for ( UINT u = vElementsChild.size(); u > 0; u-- )
	{
		const auto &v = vElementsChild[u - 1];
		if ( v->IsParentChild() )
		{
			v->OnMouseMove( pcMouse, iRenderX + iSourceX, iRenderY + iSourceY, iRenderWidth - iSourceX, iRenderHeight - iSourceY, iSourceX, iSourceY );
		}
		else
		{
			v->OnMouseMove( pcMouse, iRenderX, iRenderY, iWidth, iWidth, 0, 0 );
		}
	}

	if ( pScrollMain )
		pScrollMain->OnMouseMove( pcMouse, iRenderX, iRenderY, iWidth, iWidth, 0, 0 );

	if( bSelected )
	{
		int iX = pcMouse->GetPosition()->iX - sPointMouse.iX;
		int iY = pcMouse->GetPosition()->iY - sPointMouse.iY;

		SetPosition( iX < 0 ? 0 : iX, iY < 0 ? 0 : iY );
	}

	if ( bIsHover != bOldHover && (iHoverEventID != (-1)) )
	{
		UI::Event::Invoke( iHoverEventID );
	}
}

BOOL Window::OnKeyChar( class CKeyboard * pcKeyboard )
{
	if ( !IsOpen() )
		return FALSE;

	for ( UINT u = vElementsChild.size(); u > 0; u-- )
	{
		const auto &v = vElementsChild[u - 1];
		if ( v->OnKeyChar( pcKeyboard ) )
			return TRUE;
	}
	return FALSE;
}

BOOL Window::OnKeyPress( class CKeyboard * pcKeyboard )
{
	if( !IsOpen() )
		return FALSE;

	BOOL bRet = FALSE;

	for ( UINT u = vElementsChild.size(); u > 0; u-- )
	{
		const auto &v = vElementsChild[u - 1];
		if ( v->OnKeyPress( pcKeyboard ) )
			bRet = TRUE;
	}

	return bRet;
}

void Window::AddElement(Element_ptr pElement)
{
	pElement->AddRef();

	if ( pElement->GetType() == TYPE_Window )
		std::dynamic_pointer_cast<UI::Window>(pElement)->SetWindowScale( Element::GetScale() );
	else if ( pElement->GetUseSelfScale() == false )
		pElement->SetScale( Element::GetScale() );

	vElementsChild.push_back( pElement );
}
BOOL Window::DelElement( Element_ptr pElement )
{
	for ( std::vector<Element_ptr>::iterator it = vElementsChild.begin(); it != vElementsChild.end(); )
	{
		Element_ptr p = (*it);
		if ( p == pElement )
		{
			p->DecRef();
			if ( p->GetRef() == 0 )
			{
				p->Clear();
				p = nullptr;
			}

			it = vElementsChild.erase( it );
		}
		else
			it++;
	}

	return FALSE;
}
}