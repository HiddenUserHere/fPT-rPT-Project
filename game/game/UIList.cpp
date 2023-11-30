#include "stdafx.h"
#include "UIList.h"

namespace UI
{
List::List()
{
}

List::List( Rectangle2D rRect ) : Element( rRect )
{
}

List::~List()
{
}

void List::Clear()
{
	for ( std::deque<UI::Window_ptr>::iterator it = vWindows.begin(); it != vWindows.end(); it++ )
	{
		UI::Window_ptr pWindow = *it;
		if ( pWindow )
		{
			pWindow->DecRef();
			if ( pWindow->GetRef() == 0 )
			{
				CLEARPTR( pWindow );
				pWindow = nullptr;
			}
		}
	}

	vWindows.clear();

	if ( pScrollBar )
		pScrollBar->SetPercent( 0 );
}

Scroll_ptr List::MakeScrollBar( Rectangle2D sBox, std::string strScrollBarPath, std::string strScrollerPath )
{
	pScrollBar = std::make_shared<Scroll>( sBox );
	pScrollBar->SetVertical( TRUE );
	pScrollBar->SetImage( UI::ImageLoader::LoadImage( strScrollBarPath.c_str() ) );
	auto pImageScroller = UI::ImageLoader::LoadImage( strScrollerPath.c_str() );
	pScrollBar->SetScroller( pImageScroller, pImageScroller->GetWidth(), pImageScroller->GetHeight() );

	if( bForceDown )
	{
		pScrollBar->SetMinMax( 100, 0 );
	}
	else
	{
		pScrollBar->SetMinMax( 0, 100 );
	}

	return pScrollBar;
}

int List::GetListWidth()
{
	int iReturn = 0;

	int iLastWidth = 0;

	//Go through List Items
	for( UINT i = 0; i < vWindows.size(); i++ )
	{
		Window_ptr pWindow = vWindows[ i ];

		if ( pWindow->IsOpen() || bCountAxisHidden )
		{
			//Add Element iWidth
			int iWidth = pWindow->GetWidth() + (i == 0 ? 0 : iPaddingSeparator);

			if ( bNoUpdatePosition )
			{
				if ( iWidth > iLastWidth )
					iReturn = iWidth;
			}
			else
				iReturn += iWidth;

			iLastWidth = iWidth;
		}
	}

	return iReturn;
}

int List::GetListHeight()
{
	int iReturn = 0;

	int iLastHeight = 0;

	//Go through List Items
	for( UINT i = 0; i < vWindows.size(); i++ )
	{
		Window_ptr pWindow = vWindows[ i ];

		if ( pWindow->IsOpen() || bCountAxisHidden )
		{
			//Add Element iHeight
			int iHeight = pWindow->GetHeight() + (i == 0 ? 0 : iPaddingSeparator);

			if ( bNoUpdatePosition )
			{
				if ( iHeight > iLastHeight )
					iReturn = iHeight;
			}
			else
				iReturn += iHeight;

			iLastHeight = iHeight;
		}
	}

	return iReturn;
}

void List::SetForceDown( BOOL b )
{
	bForceDown = b;

	if ( bForceDown )
	{
		if ( pScrollBar )
			pScrollBar->SetPercent( 100 );
	}
}

BOOL List::OnMouseClick( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	BOOL bRet = FALSE;

	if ( !IsOpen() )
		return bRet;

	int iRenderX		= GetX() + iX;
	int iRenderY		= GetY() + iY;
	int iRenderWidth	= low( GetWidth(), iWidth );
	int iRenderHeight	= low( GetHeight(), iHeight );
	BOOL bIsForceDown	= IsForceDown();

	int iScroll = 0;

	if( pScrollBar )	
		iScroll = ((GetListHeight() - iRenderHeight) * (bIsForceDown ? pScrollBar->GetPercent() - 100 : pScrollBar->GetPercent())) / 100;

	if( GetListHeight() < GetHeight() )
		iScroll = 0;

	if( pScrollBar )
		pScrollBar->OnMouseClick( pcMouse, iRenderX, iRenderY, iWidth, iHeight, iSourceX, iSourceY );

	int iAddExtra = 0;
	Rectangle2D rBox = { iRenderX, iRenderY, iRenderWidth, iRenderHeight };
	if ( rBox.Inside( pcMouse->GetPosition() ) )
	{
		for( auto &v : vWindows )
		{
			if( bNoUpdatePosition )
			{
				bRet = v->OnMouseClick( pcMouse, iRenderX, iRenderY - iScroll, iRenderWidth, iRenderHeight + iScroll, 0, 0 );
				continue;
			}

			Rectangle2D rBoxWindow( iRenderX + iAddAxisX + ( bHorizontalPadding ? iAddExtra : 0 ), iRenderY + iAddAxisY - (iScroll)+( !bHorizontalPadding ? iAddExtra : 0 ) + ( bIsForceDown ? GetHeight() - v->GetHeight() : 0 ), v->GetWidth(), v->GetHeight() );
			if( rBoxWindow.Inside( pcMouse->GetPosition() ) )
			{
				for( auto &p : vWindows )
					p->OnMouseClick( pcMouse, 0, 0, 0, 0, iSourceX, iSourceY );

				bRet = v->OnMouseClick( pcMouse, rBoxWindow.iX, rBoxWindow.iY, rBoxWindow.iWidth, rBoxWindow.iHeight, iSourceX, iSourceY );
				break;
			}

			if( bHorizontalPadding )
			{
				if( bIsForceDown )
					iAddExtra -= v->GetWidth() + iPaddingSeparator;
				else
					iAddExtra += v->GetWidth() + iPaddingSeparator;
			}
			else
			{
				if( bIsForceDown )
					iAddExtra -= v->GetHeight() + iPaddingSeparator;
				else
					iAddExtra += v->GetHeight() + iPaddingSeparator;
			}
		}
	}
	else
	{
		for ( auto &v : vWindows )
			bRet = v->OnMouseClick( pcMouse, 0,0,0,0, iSourceX, iSourceY );
	}

	return bRet;
}

BOOL List::OnMouseScroll( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	if ( !IsOpen() )
		return FALSE;

	Rectangle2D rBox = { GetX() + iX, GetY() + iY, GetWidth(), GetHeight() };

	if ( pScrollBar && rBox.Inside( pcMouse->GetPosition() ) && bScrollFirst == TRUE )
		return pScrollBar->OnMouseScroll( pcMouse, GetX() + iX, GetY() + iY, iWidth, iHeight, iSourceX, iSourceY );

	if ( rBox.Inside( pcMouse->GetPosition() ) )
	{
		for ( UINT u = vWindows.size(); u > 0; u-- )
		{
			const auto &v = vWindows[u - 1];
			if ( v->OnMouseScroll( pcMouse, GetX() + iX, GetY() + iY - iScroll, GetWidth(), GetListHeight(), iSourceX, iSourceY ) )
				return TRUE;
		}
	}

	if ( pScrollBar && rBox.Inside( pcMouse->GetPosition() ) && bScrollFirst == FALSE )
		return pScrollBar->OnMouseScroll( pcMouse, GetX() + iX, GetY() + iY, iWidth, iHeight, iSourceX, iSourceY );

	return FALSE;
}

void List::OnMouseMove( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	if ( !IsOpen() )
		return;

	int iRenderX		= GetX() + iX;
	int iRenderY		= GetY() + iY;
	int iRenderWidth	= low( GetWidth(), iWidth );
	int iRenderHeight	= low( GetHeight(), iHeight );
	BOOL bIsForceDown	= IsForceDown();

	int iScroll = 0;

	if( pScrollBar )	
		iScroll = ((GetListHeight() - GetHeight()) * (bIsForceDown ? pScrollBar->GetPercent() - 100 : pScrollBar->GetPercent())) / 100;

	if( GetListHeight() < GetHeight() )
		iScroll = 0;

	if( pScrollBar )
		pScrollBar->OnMouseMove( pcMouse, GetX() + iX, GetY() + iY, iWidth, iHeight, iSourceX, iSourceY );

	int iAddExtra = 0;
	Rectangle2D rBox = { GetX() + iX, GetY() + iY, GetWidth(), GetHeight() };
	if ( rBox.Inside( pcMouse->GetPosition() ) )
	{
		for ( UINT i = 0; i < vWindows.size(); i++ )
		{
			Window_ptr pWindow = vWindows[bIsForceDown ? vWindows.size() - 1 - i : i];

			int iWindowWidth = pWindow->GetWidth();
			int iWindowHeight = pWindow->GetHeight();

			if( bNoUpdatePosition )
			{
				pWindow->OnMouseMove( pcMouse, iRenderX, iRenderY - iScroll, iRenderWidth, iRenderHeight + iScroll, 0, 0 );
				continue;
			}

			Rectangle2D rBoxWindow( GetX() + iX + iAddAxisX + (bHorizontalPadding ? iAddExtra : 0), GetY() + iY + iAddAxisY - (iScroll)+(!bHorizontalPadding ? iAddExtra : 0) + (bIsForceDown ? iRenderHeight - iWindowHeight : 0), iWindowWidth, iWindowHeight );
			if( rBoxWindow.Inside( pcMouse->GetPosition() ) )
			{
				for( auto &p : vWindows )
					p->OnMouseMove( pcMouse, 0, 0, 0, 0, iSourceX, iSourceY );

				pWindow->OnMouseMove( pcMouse, rBoxWindow.iX, rBoxWindow.iY, rBoxWindow.iWidth, rBoxWindow.iHeight, iSourceX, iSourceY );
				break;
			}

			if( bHorizontalPadding )
			{
				if( bIsForceDown )
					iAddExtra -= iWindowWidth + iPaddingSeparator;
				else
					iAddExtra += iWindowWidth + iPaddingSeparator;
			}
			else
			{
				if( bIsForceDown )
					iAddExtra -= iWindowHeight + iPaddingSeparator;
				else
					iAddExtra += iWindowHeight + iPaddingSeparator;
			}
		}
	}
	else
	{
		for ( auto &v : vWindows )
			v->OnMouseMove( pcMouse, 0,0,0,0, iSourceX, iSourceY );
	}
}

BOOL List::OnKeyChar( CKeyboard * pcKeyboard )
{
	if ( !IsOpen() )
		return FALSE;

	for ( UINT i = vWindows.size(); i > 0; i-- )
	{
		auto v = vWindows[i - 1];

		if ( v->OnKeyChar( pcKeyboard ) )
			return TRUE;
	}

	return FALSE;
}

BOOL List::OnKeyPress( CKeyboard * pcKeyboard )
{
	if ( !IsOpen() )
		return FALSE;

	for ( UINT i = vWindows.size(); i > 0; i-- )
	{
		auto v = vWindows[i - 1];

		if ( v->OnKeyPress( pcKeyboard ) )
			return TRUE;
	}

	return FALSE;
}

void List::Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	if ( !IsOpen() )
		return;

	BOOL bIsForceDown	= IsForceDown();

	int iRenderX		= GetX() + iX;
	int iRenderY		= GetY() + iY;
	int iRenderWidth	= low( GetWidth(), iWidth );
	int iRenderHeight	= low( GetHeight(), iHeight );

	int iScroll = 0;

	if ( pScrollBar )	
		iScroll = ((GetListHeight() - GetHeight()) * (bIsForceDown ? pScrollBar->GetPercent() - 100 : pScrollBar->GetPercent())) / 100;

	if( GetListHeight() < GetHeight() )
		iScroll = 0;

	if( cColorBackground.Get() != 0 )
		GRAPHICENGINE->DrawRectangle2D( Rectangle2D( iRenderX, iRenderY, iRenderWidth, iRenderHeight ), cColorBackground.Get(), cColorBackground.Get() );

	//Set Clipping
	RECT r = { iRenderX, iRenderY, iRenderWidth + iRenderX, iRenderHeight + iRenderY };
	if ( !bNoClip )
		GetGraphicsDevice()->PushScissorRect( r );

	int iAddExtra = 0;
	for( UINT i = 0; i < vWindows.size(); i++ )
	{
		Window_ptr pWindow = vWindows[bIsForceDown ? vWindows.size() - 1 - i : i];

		int iWindowWidth = pWindow->GetWidth();
		int iWindowHeight = pWindow->GetHeight();

		if( bNoUpdatePosition )
			iAddExtra = 0;

		Rectangle2D rBox( iRenderX, iRenderY, GetWidth(), GetHeight() );
		Rectangle2D rBoxWindow( iRenderX + iAddAxisX + (bHorizontalPadding ? iAddExtra : 0), iRenderY + iAddAxisY - (iScroll) + (!bHorizontalPadding ? iAddExtra : 0) + (bIsForceDown ? iRenderHeight - iWindowHeight : 0 ), iWindowWidth, iWindowHeight );

		if ( rBox.Overlaps( &rBoxWindow ) )
			pWindow->Render( rBoxWindow.iX, rBoxWindow.iY, rBoxWindow.iWidth, rBoxWindow.iHeight, iSourceX, iSourceY );

		if( bHorizontalPadding )
		{
			if( bIsForceDown )
				iAddExtra -= iWindowWidth + iPaddingSeparator;
			else
				iAddExtra += iWindowWidth + iPaddingSeparator;
		}
		else
		{
			if( bIsForceDown )
				iAddExtra -= iWindowHeight + iPaddingSeparator;
			else
				iAddExtra += iWindowHeight + iPaddingSeparator;
		}
	}

	if ( !bNoClip )
		GetGraphicsDevice()->PopScissorRect();

	if( pScrollBar && GetListHeight() > GetHeight() )
		pScrollBar->Render( iRenderX, iRenderY, pScrollBar->GetRect().iWidth, pScrollBar->GetRect().iHeight, 0, 0 );
}

void List::Update( float fTime )
{
	if ( pScrollBar )
		pScrollBar->SetScale( Element::GetScale() );

	for( UINT i = 0; i < vWindows.size(); i++ )
	{
		Window_ptr pWindow = vWindows[i];

		if( pWindow )
			pWindow->Update( fTime );
	}
}

void List::AddAxis( int iX, int iY )
{
	iAddAxisX += iX;
	iAddAxisY += iY;
}

void List::SetAxis( int iX, int iY )
{
	iAddAxisX = iX;
	iAddAxisY = iY;
}

void List::SetListScale( float fScale )
{
	Element::SetScale( fScale );

	for ( auto p : vWindows )
		p->SetWindowScale( fScale );
}

void List::AddWindow( Window_ptr pWindow, bool bFront )
{
	pWindow->AddRef();

	pWindow->SetWindowScale( Element::GetScale() );

	if ( bHorizontalPadding )
		pWindow->SetPosition( pWindow->GetX(), pWindow->GetY() );
	else
		pWindow->SetPosition( pWindow->GetX(), pWindow->GetY() );
		
	if ( bFront )
		vWindows.push_front( pWindow );
	else
		vWindows.push_back( pWindow );
}
void List::DelWindow( Window_ptr pWindow )
{
	for ( std::deque<UI::Window_ptr>::iterator it = vWindows.begin(); it != vWindows.end(); )
	{
		UI::Window_ptr p = (*it);
		if ( p )
		{
			if ( p == pWindow )
			{
				p->DecRef();
				if ( p->GetRef() == 0 )
				{
					CLEARPTR( p );
					p = nullptr;
				}

				it = vWindows.erase( it );
			}
			else
				it++;
		}
		else
			it++;
	}
}
}