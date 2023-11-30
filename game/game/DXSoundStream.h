#pragma once

#include "DXSound.h"

namespace DX
{

const int SOUNDSTREAM_SILENTBUFFER_SIZE_MS	= 100;	//Silent Buffer Size in Seconds based on Sound Buffer Wave Format
const int SOUNDSTREAM_STOP_PRECISION_MS		= 30;	//Timer Precision in Timer Thread
	
typedef std::shared_ptr<class SoundStream> SoundStream_ptr;
	
class SoundStream
{
public:
							  SoundStream( LPDIRECTSOUND8 lpDS, LPDIRECTSOUNDBUFFER8 lpDSoundBuffer );
	virtual					 ~SoundStream();

	void					  NewAudio( const BYTE * pBuffer, DWORD dwBufferSize );

	void					  Play();
	BOOL					  IsPlaying();
	void					  Stop();

	BOOL					  HasGlobalFocus() const;

	const WAVEFORMATEX		& GetWaveFormat() const;

	int						  GetVolume() const;
	void					  SetVolume( int iNewVolume );

private:
	void					  AppendBuffer( const BYTE * pBuffer, DWORD dwBufferSize, BOOL bMoveWritePosition );

	void					  ThreadTimer();

private:
	LPDIRECTSOUND8			  lpDS;
	LPDIRECTSOUNDBUFFER8	  lpDSoundBuffer;

	DSBCAPS					  DSBCaps;
	WAVEFORMATEX			  WaveFormatEx;

	BYTE					* pSilentBuffer = NULL;
	DWORD					  dwSilentBufferSize = 0;
	
	std::recursive_mutex	  rmMutex;

	DWORD					  dwWritePosition = 0;
	DWORD					  dwBytesInBuffer = 0;
	DWORD					  dwLastCurrentPlayCursor = 0;

	int						  iVolume = MAX_VOLUME;

	std::thread				  tTimerThread;
	BOOL					  bTerminateTimerThread = FALSE;
};

}