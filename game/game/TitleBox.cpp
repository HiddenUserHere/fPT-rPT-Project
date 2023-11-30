#include "stdafx.h"
#include "TitleBox.h"

TitleBox::TitleBox()
{
}

TitleBox::~TitleBox()
{
}

void TitleBox::Render()
{
	int i = 0;
	for( auto &v : vTitles )
	{
		if( i == 1 )
			v.d3dcolor.a = 127;
		else if( i == 2 )
			v.d3dcolor.a = 70;

		v.iOpacity = (int)(float)(v.d3dcolor.a * 1.0f);

		pFont->Draw( v.strText, RECT{ 0, v.sPosition.iY + 2, RESOLUTION_WIDTH, 24 }, DT_CENTER | DT_EXPANDTABS | DT_NOCLIP, DX::Color( (int)v.iOpacity, 0, 0, 0 ) );
		pFont->Draw( v.strText, RECT{ 0, v.sPosition.iY, RESOLUTION_WIDTH, 24 }, DT_CENTER | DT_EXPANDTABS | DT_NOCLIP, DX::Color( (int)v.iOpacity, (int)v.d3dcolor.r, (int)v.d3dcolor.g, (int)v.d3dcolor.b ) );

		i++;
	}
}

void TitleBox::SetText( char * pszText, int iTime )
{
	if ( pszText && pszText[0] != 0 )
		AddTitle( Title( Point2D( 0, 180 ), trim((std::string)pszText), D3DXCOLOR(255,185,50,255), MAX_DURATION_TITLEBOX ) );
}

void TitleBox::SetText( int iID )
{
	struct MessageBox1
	{
		char				szMessage1[64];
		char				szMessage2[64];
		BOOL				bActive;
		int					iLength;
		int					iX;
		int					iY;
		int					iLine;
	};

	MessageBox1 * ps = ((MessageBox1*)0x009588D0);

	if( ps[iID].szMessage1[0] != 0 )
		AddTitle( Title( Point2D( 0, 100 ), trim((std::string)ps[iID].szMessage1), D3DXCOLOR(255,185,50,255), MAX_DURATION_TITLEBOX ) );
}

void TitleBox::RenderTextMiddle( const char * pszText )
{
	if( pImageLeft && pImageCenter && pImageRight && STRLEN( pszText ) > 0 )
	{
		std::string str = trim( ( std::string )pszText ).c_str();

		int iW = GETWIDTHTEXT( str.c_str() );
		int iX = (RESOLUTION_WIDTH >> 1) - ( (iW + 64) >> 1 );
		int iY = (RESOLUTION_HEIGHT >> 1) - (pImageLeft->GetHeight() >> 1);

		UI::ImageRender::Render( pImageLeft, iX, iY, pImageLeft->GetWidth(), pImageLeft->GetHeight(), -1 );
		UI::ImageRender::Render( pImageCenter, iX + 32, iY, iW, pImageLeft->GetHeight(), -1 );
		UI::ImageRender::Render( pImageRight, iX + 32 + iW, iY, pImageLeft->GetWidth(), pImageLeft->GetHeight(), -1 );

		GetGraphicsDevice()->GetDefaultFont()->Draw( str, iX + 32, iY + 13, 0, DX::Color( 255, 255, 255 ) );
	}
}

void TitleBox::AddTitle( Title psTitle )
{
	BOOL bFound = FALSE;
	//Found?
	for ( auto &v : vTitles )
	{
		if ( v.strText.compare( psTitle.strText ) == 0 )
		{
			bFound = TRUE;
			break;
		}
	}

	if ( bFound == FALSE )
	{
		vTitles.insert( vTitles.begin(), psTitle );

		for ( auto &v : vTitles )
		{
			if ( !v.strText.empty() )
				v.bMoveToDown = TRUE;
		}

		if ( vTitles.size() > MAX_MESSAGES )
			vTitles.pop_back();
	}
}

void TitleBox::Update( float fTime )
{
	int i = 0;
	for( auto &v : vTitles )
	{
		if( v.bMoveToDown )
		{
			float fTimeUpdate = v.fTime / MAX_DURATION_ANIM;
			float f = X3D::easeInOutCircf( 0.0f, 1.0f, fTimeUpdate );

			if( v.iBasePositionY == 0 )
				v.iBasePositionY = v.sPosition.iY;

			if( i == 0 )
				v.sPosition.iY = (int)( v.iBasePositionY * ( 1.0f - f ) );
			else
				v.sPosition.iY = (int)( v.iBasePositionY + ( 24 * ( 1.0f - f ) ) );
		}

		if( v.fTime > 0.0f )
			v.fTime -= fTime;
		else
		{
			v.bMoveToDown = FALSE;
			v.fTime = 100.0f;
			v.iBasePositionY = 0;
		}

		i++;
	}
}

void TitleBox::Tick()
{
	for( auto &v : vTitles )
	{
		v.sDuration--;

		if( v.sDuration <= 3 && !v.bCanHide )
		{
			v.bCanHide = TRUE;
			v.fTime = 100.0f;
		}

		if( v.sDuration <= 0 )
			vTitles.pop_back();
	}
}

void TitleBox::Init()
{
	pFont = GetGraphicsDevice()->GetFontFactory()->MakeFont( nullptr, "Frutiger LT 55 Roman", 24, 0, FALSE, FALSE );

	pImageLeft = UI::ImageLoader::LoadImage( "game\\images\\UI\\title\\left.png" );
	pImageCenter = UI::ImageLoader::LoadImage( "game\\images\\UI\\title\\center.png" );
	pImageRight = UI::ImageLoader::LoadImage( "game\\images\\UI\\title\\right.png" );
}