#pragma once

#include "DXSound.h"

class DXMusic
{
public:
	void				  Reset();

	void				  Fade();

	void				  Play( const char * pszPath, BOOL bSwift = FALSE );

	void				  Start( BOOL bFade = FALSE );
	void				  Stop( BOOL bFade = FALSE );

	void				  SetVolume( int i );

private:
	DX::Sound_ptr		  pFrontMusic;
	DX::Sound_ptr		  pBackMusic;

	BOOL				  bStopped = FALSE;
	int					  iVolume = DX::MAX_VOLUME;
};