#include "stdafx.h"
#include "DXMusic.h"

#include "DX.h"

using namespace DX;

void DXMusic::Reset()
{
	pBackMusic = nullptr;
	pFrontMusic = nullptr;
}

void DXMusic::Fade()
{
	static int iFadeWheel = 0;

	iFadeWheel++;

	if( (iFadeWheel % 3) == 0 )
	{
		if( pFrontMusic )
		{
			int iVol = pFrontMusic->GetVolume();

			if( (iVol < MAX_VOLUME) && (iVol < iVolume) )
			{
				iVol++;
				pFrontMusic->SetVolume( iVol );
			}
		}
		
		if( pBackMusic )
		{
			int iVol = pBackMusic->GetVolume();

			if( iVol > MIN_VOLUME )
			{
				iVol--;
				pBackMusic->SetVolume( iVol );
			}
			else
				pBackMusic = nullptr;
		}
	}
}

void DXMusic::Play( const char * pszPath, BOOL bSwift )
{
	//WRITEDBG( "DXMusic::Play(%s,%s)", pszPath ? pszPath : "NULL", bSwift ? "TRUE" : "FALSE" );

	if( pszPath == NULL )
	{
		pFrontMusic = nullptr;
		pBackMusic = nullptr;
		return;
	}

	DX::Sound_ptr sound;

	if( pFrontMusic && STRINGCOMPAREI( pFrontMusic->GetFile().c_str(), pszPath ) )
	{
		sound = pFrontMusic;
	}
	else if( pBackMusic && STRINGCOMPAREI( pBackMusic->GetFile().c_str(), pszPath ) )
	{
		sound = pBackMusic;
		pBackMusic = pFrontMusic;
		pFrontMusic = sound;
	}
	else
	{
		sound = DX::GetSoundDevice()->GetSoundFactory()->MakeTemporarySound( pszPath );
		if( sound )
		{
			if( bStopped )
			{
				sound->SetVolume( MIN_VOLUME );
				sound->Stop();
			}
			else
				sound->Play( TRUE, MIN_VOLUME );

			pBackMusic = pFrontMusic;
			pFrontMusic = sound;
		}
	}

	if( bSwift )
	{
		pFrontMusic->SetVolume( iVolume );
		pBackMusic->SetVolume( MIN_VOLUME );
	}
}

void DXMusic::Start( BOOL bFade )
{
	//WRITEDBG( "DXMusic::Start(%s)", bFade ? "TRUE" : "FALSE" );

	if( bStopped )
	{
		bStopped = FALSE;

		if( bFade == FALSE )
		{
			if( pFrontMusic )
				pFrontMusic->Play( TRUE, pFrontMusic->GetVolume() );

			if( pBackMusic )
				pBackMusic->Play( TRUE, pBackMusic->GetVolume() );
		}
	}
}

void DXMusic::Stop( BOOL bFade )
{
	//WRITEDBG( "DXMusic::Stop(%s)", bFade ? "TRUE" : "FALSE" );

	if( bStopped == FALSE )
	{
		bStopped = TRUE;

		if( bFade == FALSE )
		{
			if( pFrontMusic )
				pFrontMusic->Stop();

			if( pBackMusic )
				pBackMusic->Stop();
		}
	}
}

void DXMusic::SetVolume( int i )
{
	//WRITEDBG( "DXMusic::SetVolume(%d)", i );

	iVolume = i;

	if( pFrontMusic )
		pFrontMusic->SetVolume( iVolume );

	if( pBackMusic )
		pBackMusic->SetVolume( iVolume );
}