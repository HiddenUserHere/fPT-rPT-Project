#include "stdafx.h"
#include "DXSound.h"

#include "DX.h"

#define MIN_DS_VOLUME	-4000
#define MAX_DS_VOLUME	0

namespace DX
{

Sound::Sound( LPDIRECTSOUND8 lpDS, LPDIRECTSOUNDBUFFER8 lpDSoundBuffer, std::string strSoundFile, BOOL bHasGlobalFocus ) : lpDS(lpDS), strSoundFile(strSoundFile), bHasGlobalFocus(bHasGlobalFocus)
{
	ADDREF( lpDS );

	AddBuffer( lpDSoundBuffer );
}

Sound::~Sound()
{
	RemoveBuffers();

	RELEASE( lpDS );
}

void Sound::AddBuffer( LPDIRECTSOUNDBUFFER8 lpDSoundBuffer )
{
	if( lpDSoundBuffer == NULL )
		return;

	ADDREF( lpDSoundBuffer );

	vSoundBuffers.push_back( lpDSoundBuffer );
}

void Sound::RemoveBuffers()
{
	for( auto lpDSoundBuffer : vSoundBuffers )
		RELEASE( lpDSoundBuffer );

	vSoundBuffers.clear();
}

int Sound::Play( BOOL bLoop, LONG lPlayVolume, DWORD dwFrequency )
{
	//No Sound Buffers?
	if( vSoundBuffers.size() == 0 )
		return -1; //Fatal Error

	//Set Global Volume
	lVolume = lPlayVolume;

	//Loop the Play Wheel
	if( uPlayWheel >= vSoundBuffers.size() )
		uPlayWheel = 0;

	//Get DirectSound Buffer to Play
	auto lpDSoundBuffer = vSoundBuffers[uPlayWheel++];

	//Stop DirectSound Buffer
	lpDSoundBuffer->Stop();

	//Reset Play Position of DirectSound Buffer to Play
	lpDSoundBuffer->SetCurrentPosition( 0 );

	//Set Volume of DirectSound Buffer to Play
	lpDSoundBuffer->SetVolume( VolumeToDirectSoundVolume( lVolume ) );

	//Set Frequency of DirectSound Buffer to Play
	lpDSoundBuffer->SetFrequency( dwFrequency );

	//Try to Play Sound. Failed and is Buffer Lost??
	if( lpDSoundBuffer->Play( 0, 0, bLoop ? DSBPLAY_LOOPING : 0 ) == DSERR_BUFFERLOST )
	{
		if( DSSUCCEEDED( lpDSoundBuffer->Restore() ) )
			return 0; //Require Sound Reload
	}

	return 1; //OK
}

BOOL Sound::IsPlaying()
{
	for( auto lpDSoundBuffer : vSoundBuffers )
	{
		DWORD dwStatus = 0;
		if( DSFAILED( lpDSoundBuffer->GetStatus( &dwStatus ) ) )
			continue;

		if( dwStatus & DSBSTATUS_PLAYING )
			return TRUE;
	}

	return FALSE;
}

void Sound::Stop()
{
	for( auto lpDSoundBuffer : vSoundBuffers )
		lpDSoundBuffer->Stop();
}

int Sound::GetVolume() const
{
	return lVolume;
}

void Sound::SetVolume( LONG lNewVolume )
{
	if( lVolume == lNewVolume )
		return;

	lVolume = lNewVolume;
	
	for( auto lpDSoundBuffer : vSoundBuffers )
		lpDSoundBuffer->SetVolume( VolumeToDirectSoundVolume( lVolume ) );
}

LONG Sound::VolumeToDirectSoundVolume( LONG lVolume )
{
	if( lVolume >= MAX_VOLUME )
		return MAX_DS_VOLUME;
	else if( lVolume <= MIN_VOLUME )
		return MIN_DS_VOLUME;

	return ((lVolume * (MAX_DS_VOLUME - MIN_DS_VOLUME)) / MAX_VOLUME) + MIN_DS_VOLUME;
}

}