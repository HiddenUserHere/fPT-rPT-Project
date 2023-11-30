#include "stdafx.h"
#include "LoginModel.h"

UI::Bitmapfont_ptr pBitmapFont = nullptr;

CLoginModel::CLoginModel()
{
}

CLoginModel::~CLoginModel()
{
}

void CLoginModel::RenderServerSelectBox()
{
	CLoginScreen * p = NULL;

	if ( GAMESCREEN->GetActiveScreenType() == SCREEN_Login )
		p = (CLoginScreen*)GAMESCREEN->GetActiveScreen();

	if ( !bLoaded )
	{
		pBoxImageHandler = UI::ImageLoader::LoadImage( "game\\images\\UI\\messagebox\\background_normal.png" );

		pBitmapFont = std::make_shared<UI::BitmapFont>( Rectangle2D( 0, 0, iWidthServerSelectBox, 0 ) );
		pBitmapFont->SetHorizontalAlign( ALIGN_Center );
		pBitmapFont->ReadFontData( "game\\fonts\\messagebox.dat" );
		pBitmapFont->SetFontImage( UI::ImageLoader::LoadImage( "game\\fonts\\messagebox.png" ) );
		pBitmapFont->SetText( "SELECT SERVER" );

		bLoaded = TRUE;
	}

	if ( p )
	{
		DWORD dwWidth = RESOLUTION_WIDTH;
		DWORD dwHeight = RESOLUTION_HEIGHT;

		if ( p->GetCurrentState() == 3 )
		{
			UI::ImageRender::Render( pBoxImageHandler, (dwWidth >> 1) - (iWidthServerSelectBox >> 1), (dwHeight >> 1) + (iHeightServerSelectBox / 3) - 10, iWidthServerSelectBox, iHeightServerSelectBox, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			pBitmapFont->Render( (dwWidth >> 1) - (iWidthServerSelectBox >> 1), (dwHeight >> 1) + (iHeightServerSelectBox / 3) + 4, iWidthServerSelectBox, RESOLUTION_HEIGHT, 0, 0 );
		}
	}
}

void CLoginModel::OnRender()
{
	
}

void CLoginModel::Init()
{
	
}

void CLoginModel::RenderLoginBG()
{
	static char * pszaLoginBG[] =
	{
		"game\\images\\login\\bg1.png",
		"game\\images\\login\\bg2.png",
		"game\\images\\login\\bg3.png",
		"game\\images\\login\\bg4.png",
		"game\\images\\login\\bg5.png",
		"game\\images\\login\\bg6.png",
		"game\\images\\login\\bg7.png",
	};
	if ( pImageBGLogin == NULL )
	{
		srand( (UINT)time( NULL ) );
		pImageBGLogin = UI::ImageLoader::LoadImageA( pszaLoginBG[rand() % _countof( pszaLoginBG )] );
	}

	if ( pImageBGLogin )
	{
		if ( ((CLoginScreen*)GAMESCREEN->GetActiveScreen())->IsLoginVideo() == FALSE )
			UI::ImageRender::Render( pImageBGLogin, (RESOLUTION_WIDTH >> 1) - (pImageBGLogin->GetWidth() >> 1), (RESOLUTION_HEIGHT >> 1) - (pImageBGLogin->GetHeight() >> 1), pImageBGLogin->GetWidth(), pImageBGLogin->GetHeight(), -1 );
	}
}
