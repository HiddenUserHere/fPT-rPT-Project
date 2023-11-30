#include "stdafx.h"
#include "UIDropdownMenu.h"

namespace UI
{
DropdownMenu::DropdownMenu()
{
}

DropdownMenu::DropdownMenu( Rectangle2D sBox ) : Element(sBox)
{
	bShow = FALSE;
}

DropdownMenu::~DropdownMenu()
{
}

void DropdownMenu::Open()
{
	bAnimate = TRUE;
	bOpen = TRUE;
	bClose = FALSE;
	fProgress = 0.0f;
	bShow = TRUE;

	if( pGroup )
		pGroup->SetIndexObject( GetID() );

	if( ePosition == DROPDOWNPOSITION_Left )
		sAnimateBox = Point2D( -GetWidth(), 0 );
}

void DropdownMenu::Close()
{
	bAnimate = TRUE;
	bOpen = FALSE;
	bClose = TRUE;
	fProgress = 1.0f;
}

void DropdownMenu::Clear()
{
	for( size_t i = 0; i < vOptions.size(); i++ )
	{
		vOptions[i].pText->Clear();
		vOptions[i].pText = nullptr;
	}

	vOptions.clear();
	iHeightMenu = 0;
}

void DropdownMenu::SetMenuBackground( std::string strImageTop, std::string strImageMiddle, std::string strImageBottom, std::string strImageMain )
{
	pImageTop = UI::ImageLoader::LoadImage( strImageTop.c_str() );
	pImageMiddle = UI::ImageLoader::LoadImage( strImageMiddle.c_str() );
	pImageBottom = UI::ImageLoader::LoadImage( strImageBottom.c_str() );
	pImageMain = UI::ImageLoader::LoadImage( strImageMain.c_str() );
}

Text_ptr DropdownMenu::MakeOption( const std::string & strText, const Rectangle2D & rBox, BOOL bBold, const DWORD dwColor, const DWORD dwHoverColor )
{
	Text_ptr pText = std::make_shared<Text>( strText, rBox, bBold, dwColor );
	pText->SetNoClip( TRUE );
	pText->SetColorHover( dwHoverColor );
	pText->SetSelectType( TRUE );

	return pText;
}

void DropdownMenu::AddOption( Text_ptr p, BOOL bChild )
{
	p->SetBox( Rectangle2D( p->GetX(), p->GetY() + (iHeightMenu), p->GetWidth(), p->GetHeight() ) );
	p->SetID( vOptions.size() + 1 );
	p->SetHorizontalAlign( ALIGN_Center );
	p->SetVerticalAlign( ALIGN_Middle );

	Option sOption;
	sOption.bChild = bChild;
	sOption.pText = p;

	vOptions.push_back( sOption );

	iHeightMenu += p->GetHeight();
}

void DropdownMenu::AddOption( Text_ptr p, BOOL bChild, int iID )
{
	p->SetBox( Rectangle2D( p->GetX(), p->GetY() + (iHeightMenu), p->GetWidth(), p->GetHeight() ) );
	p->SetID( iID );
	p->SetHorizontalAlign( ALIGN_Center );
	p->SetVerticalAlign( ALIGN_Middle );

	Option sOption;
	sOption.bChild = bChild;
	sOption.pText = p;

	vOptions.push_back( sOption );

	iHeightMenu += p->GetHeight();
}

void DropdownMenu::RemoveOption( int iID )
{
	for( std::vector<UI::Option>::iterator it = vOptions.begin(); it != vOptions.end(); )
	{
		if ( (*it).pText->GetID() == iID )
		{
			iHeightMenu -= (*it).pText->GetHeight();
			(*it).pText->Clear();
			(*it).pText = nullptr;
			it = vOptions.erase( it );
		}
		else
			it++;
	}
}

void DropdownMenu::Animate()
{
	if( bAnimate )
	{
		if( bOpen )
		{
			if( ePosition == DROPDOWNPOSITION_Left )
				sAnimateBox.iX += 5;

			if( sAnimateBox.iX > 0 || sAnimateBox.iY > 0 )
			{
				bAnimate = FALSE;
				bOpen = TRUE;
				bClose = FALSE;
			}

			fProgress += 0.05f;
			if( fProgress > 1.0f )
				fProgress = 1.0f;
		}
		else if( bClose )
		{
			if( ePosition == DROPDOWNPOSITION_Left )
				sAnimateBox.iX -= 5;

			if( sAnimateBox.iX < -GetWidth() )
			{
				bAnimate = FALSE;
				bOpen = FALSE;
				bClose = FALSE;
				bShow = FALSE;
			}

			fProgress -= 0.05f;
			if( fProgress < 0.0f )
				fProgress = 0.0f;
		}
	}
}

void DropdownMenu::Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	int iRenderX		= GetX() + iX;
	int iRenderY		= GetY() + iY;
	int iRenderWidth	= low( GetWidth(), iWidth );
	int iRenderHeight	= low( GetHeight(), iHeight );

	if( pImageTop && pImageMiddle && pImageBottom && pImageMain && IsOpen() && vOptions.size() > 0 )
	{
		Animate();

		if( vOptions.size() == 1 )
			UI::ImageRender::Render( pImageMain, iRenderX + sAnimateBox.iX, iRenderY + sAnimateBox.iY, pImageMain->GetWidth(), pImageMain->GetHeight(), iSourceX, iSourceY, D3DCOLOR_ARGB( (int)(float)( fProgress * 255 ), 255, 255, 255 ) );
		else
		{
			UI::ImageRender::Render( pImageTop, iRenderX + sAnimateBox.iX, iRenderY + sAnimateBox.iY, iRenderWidth, iRenderHeight, iSourceX, iSourceY, D3DCOLOR_ARGB( (int)(float)( fProgress * 255 ), 255, 255, 255 ) );
			UI::ImageRender::Render( pImageMiddle, iRenderX + sAnimateBox.iX, iRenderY + sAnimateBox.iY + iRenderHeight, iRenderWidth, iHeightMenu - iRenderHeight + vOptions[0].pText->GetY() + 7 - pImageBottom->GetHeight(), iSourceX, iSourceY, D3DCOLOR_ARGB( (int)(float)( fProgress * 255 ), 255, 255, 255 ) );
			UI::ImageRender::Render( pImageBottom, iRenderX + sAnimateBox.iX, iRenderY + sAnimateBox.iY + iHeightMenu - iRenderHeight + vOptions[0].pText->GetY() + 7, iRenderWidth, iRenderHeight, iSourceX, iSourceY, D3DCOLOR_ARGB( (int)(float)( fProgress * 255 ), 255, 255, 255 ) );
		}

		for( auto &v : vOptions )
		{
			D3DXCOLOR c = v.pText->GetColor();
			c.a = fProgress;

			v.pText->SetColor( c );
			v.pText->Render( iRenderX + sAnimateBox.iX, iRenderY + sAnimateBox.iY, iWidth, iHeight, iSourceX, iSourceY );
		}
	}
}

void DropdownMenu::Update( float fTime )
{
	if( bShow && bOpen && !bClose && !bAnimate )
	{
		if( pGroup )
			if( pGroup->GetIndexObject() != GetID() )
				Close();
	}
}

BOOL DropdownMenu::OnMouseClick( CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	if( !IsOpen() )
		return FALSE;

	if( pcMouse->GetEvent() != ClickDownL && pcMouse->GetEvent() != ClickDownR )
		return FALSE;

	int iRenderX		= GetX() + iX;
	int iRenderY		= GetY() + iY;
	int iRenderWidth	= low( GetWidth(), iWidth );
	int iRenderHeight	= low( GetHeight(), iHeight );

	for( auto &v : vOptions )
	{
		if( v.pText->OnMouseClick( pcMouse, iRenderX, iRenderY, iWidth, iHeight, iSourceX, iSourceY ) )
		{
			iSelectedOptionID = v.pText->GetID();
			UI::Event::Invoke( iEventID );
			iSelectedOptionID = -1;

			if( iEventID )
				Close();

			return TRUE;
		}
	}

	if( bShow && bOpen && !bClose && !bAnimate )
	{
		Close();
		return TRUE;
	}

	return FALSE;
}

void DropdownMenu::OnMouseMove( CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	if( !IsOpen() )
		return;

	int iRenderX		= GetX() + iX;
	int iRenderY		= GetY() + iY;
	int iRenderWidth	= low( GetWidth(), iWidth );
	int iRenderHeight	= low( GetHeight(), iHeight );

	for( auto &v : vOptions )
		v.pText->OnMouseMove( pcMouse, iRenderX, iRenderY, iWidth, iHeight, iSourceX, iSourceY );
}
}