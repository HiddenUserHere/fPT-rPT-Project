#include "stdafx.h"
#include "CGameScreen.h"

CGameScreen::CGameScreen()
{
	pcActiveScreen = NULL;
	bActiveScreen = FALSE;
	iActiveScreen = SCREEN_None;
}

CGameScreen::~CGameScreen()
{
	if ( bActiveScreen )
		DELET( pcActiveScreen );
}

void CGameScreen::Init()
{
}

void CGameScreen::Shutdown()
{
	if ( bActiveScreen )
		SHUTDOWN( pcActiveScreen );
}

void CGameScreen::Frame()
{
	if ( bActiveScreen )
	{
		EScreen iNewActiveScreen = pcActiveScreen->Frame();

		if ( iNewActiveScreen != iActiveScreen )
			ChangeScreen( iNewActiveScreen );
	}
}

void CGameScreen::Update( float fTime )
{
	CAMERA->Update( (double)fTime );

	if( bActiveScreen )
		pcActiveScreen->Update( fTime );
}

void CGameScreen::Render3D()
{
	if ( bActiveScreen )
		pcActiveScreen->Render3D();
}

void CGameScreen::Render2D()
{
	if ( bActiveScreen )
		pcActiveScreen->Render2D();
}

void CGameScreen::OnMouseMove( CMouse * pcMouse )
{
	if ( bActiveScreen )
		pcActiveScreen->OnMouseMove( pcMouse );
}

BOOL CGameScreen::OnKeyPress( CKeyboard * pcKeyboard )
{
	if( bActiveScreen )
		return pcActiveScreen->OnKeyPress( pcKeyboard );

	return FALSE;
}

BOOL CGameScreen::OnMouseClick( CMouse * pcMouse )
{
	if ( bActiveScreen )
		return pcActiveScreen->OnMouseClick( pcMouse );

	return FALSE;
}

BOOL CGameScreen::OnMouseScroll( CMouse * pcMouse )
{
	if( bActiveScreen )
		return pcActiveScreen->OnMouseScroll( pcMouse );

	return FALSE;
}

BOOL CGameScreen::OnKeyChar( CKeyboard * pcKeyboard )
{
	if( bActiveScreen )
		return pcActiveScreen->OnKeyChar( pcKeyboard );

	return FALSE;
}

void CGameScreen::OnResolutionChanged()
{
	if( bActiveScreen )
		pcActiveScreen->OnResolutionChanged();
}

void CGameScreen::ChangeScreen( EScreen iNewScreen )
{
	bActiveScreen = FALSE;

	iActiveScreen = iNewScreen;

	SHUTDOWN( pcActiveScreen );
	DELET( pcActiveScreen );

	switch ( iNewScreen )
	{
		case SCREEN_Login:
			pcActiveScreen = new CLoginScreen();
			break;
		case SCREEN_Character:
			pcActiveScreen = new CCharacterScreen();
			break;
		case SCREEN_World:
			pcActiveScreen = new CWorldScreen();
			break;
		case SCREEN_Test:
			pcActiveScreen = new CTestScreen();
			break;
		default:
			pcActiveScreen = NULL;
			break;
	}

	if ( pcActiveScreen )
		bActiveScreen = pcActiveScreen->Init();
}
