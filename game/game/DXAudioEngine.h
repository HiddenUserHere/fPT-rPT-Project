#pragma once

#include "DXSound.h"
#include "DXMusic.h"

#define NUM_SOUNDS			3000
#define GAME_VOLUME_MAX		400

#define PLAY_SOUND(i)		(AUDIOENGINE->PlaySound( i ))
#define SOUND_VOLUME(x)		(AUDIOENGINE->SetVolume( x ))

#define PLAY_MUSIC(p)		(AUDIOENGINE->PlayMusic( p ))
#define PLAY_MUSIC_SWIFT(p)	(AUDIOENGINE->PlayMusic( p, TRUE ))
#define START_MUSIC(b)		(AUDIOENGINE->StartMusic( b ))
#define STOP_MUSIC(b)		(AUDIOENGINE->StopMusic( b ))
#define MUSIC_VOLUME(x)		(AUDIOENGINE->SetMusicVolume( x ))

class DXAudioEngine
{
public:
							  DXAudioEngine();

	void					  Shutdown();

	void					  PrepareSound( UINT iIndex, const char * pszFilePath );

	void					  LoadAllSounds();
	BOOL					  LoadSound( UINT iIndex, const char * pszFilePath );

	void					  Update( double fTime );

	void					  StartSound() { bSound = TRUE; }
	void					  StopSound() { bSound = FALSE; }
	BOOL					  HasSound() { return bSound; }

	int						  GetVolume() { return iSoundVolume; }
	void					  SetVolume( int iNewSoundVolume );

	void					  PlaySound( UINT iIndex, BOOL bLoop = FALSE, int iVolume = GAME_VOLUME_MAX, DWORD dwFrequency = DSBFREQUENCY_ORIGINAL );
	void					  StopSound( UINT iIndex );
	BOOL					  IsSoundPlaying( UINT iIndex );
	void					  SetSoundVolume( UINT iIndex, int iVolume );

	void					  PlayMusic( const char * pszPath, BOOL bSwift = FALSE ) { cMusic.Play( pszPath, bSwift ); }
	void					  StartMusic( BOOL bFade ) { cMusic.Start( bFade ); }
	void					  StopMusic( BOOL bFade ) { cMusic.Stop( bFade ); }
	void					  SetMusicVolume( int iNewMusicVolume ) { cMusic.SetVolume( iNewMusicVolume ); }

private:
	BOOL					  bSound;
	int						  iSoundVolume;
	float					  fSoundVolumeD;
	int						  iVolumeDivider;

	char					  szaSound[NUM_SOUNDS][MAX_PATH];
	DX::Sound_ptr			  paSound[NUM_SOUNDS];

	BOOL					  bMusic = FALSE;
	DXMusic					  cMusic;
};