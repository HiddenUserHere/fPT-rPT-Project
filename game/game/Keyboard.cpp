#include "stdafx.h"
#include "Keyboard.h"

CKeyboard::CKeyboard()
{
	for( int i = 0; i < 256; i++ )
		baKeyDown[ i ] = FALSE;
}

CKeyboard::~CKeyboard()
{
}

void CKeyboard::SetEvent( EKeyboardEvent eKeyEvent )
{
	eEvent = eKeyEvent;
}

BOOL CKeyboard::IsKeyDown( int iKey )
{
	if( ( iKey < 0 ) || ( iKey >= 256 ) )
		return FALSE;

	return baKeyDown[ iKey ];
}

BOOL CKeyboard::OnKeyPress( int iKey, BOOL bKeyDown )
{
	if ( bKeyDown )
		SetEvent( KeyDown );
	else
		SetEvent( KeyUp );
	
	if( ( iKey < 0 ) || ( iKey >= 256 ) )
		return FALSE;

	if ( bKeyDown )
	{
		baKeyDown[iKey] = TRUE;
	}
	else
	{
		baKeyDown[iKey] = FALSE;
		ISPRESSEDKEY( iKey ) = FALSE;
	}

	iKeyPress = iKey;

	return GAME->OnKeyPress( this );
}

BOOL CKeyboard::OnKeyChar( wchar_t cKey )
{
	SetEvent( EKeyboardEvent::KeyChar );
	cKeyChar = cKey;
	return GAME->OnKeyChar( this );
}

