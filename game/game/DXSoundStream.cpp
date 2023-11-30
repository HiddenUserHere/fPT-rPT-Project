#include "stdafx.h"
#include "DXSoundStream.h"

#include "DX.h"
//#include "CountTimer.h"
#include "Timer.h"

namespace DX
{

SoundStream::SoundStream( LPDIRECTSOUND8 lpDS, LPDIRECTSOUNDBUFFER8 lpDSoundBuffer ) : lpDS(lpDS), lpDSoundBuffer(lpDSoundBuffer)
{
	ADDREF( lpDS );
	ADDREF( lpDSoundBuffer );

	if( lpDSoundBuffer )
	{
		DSBCaps.dwSize = sizeof( DSBCaps );

		if( DSFAILED( lpDSoundBuffer->GetCaps( &DSBCaps ) ) )
		{
			RELEASE( lpDSoundBuffer );
			return;
		}

		if( DSFAILED( lpDSoundBuffer->GetFormat( &WaveFormatEx, sizeof( WAVEFORMATEX ), NULL ) ) )
		{
			RELEASE( lpDSoundBuffer );
			return;
		}

		//Set Default Position
		lpDSoundBuffer->SetCurrentPosition( 0 );

		//Set Default Volume
		lpDSoundBuffer->SetVolume( Sound::VolumeToDirectSoundVolume( iVolume ) );
	}

	//Create the Silent Buffer (empty)
	{
		dwSilentBufferSize = (WaveFormatEx.nAvgBytesPerSec * SOUNDSTREAM_SILENTBUFFER_SIZE_MS) / 1000;
		pSilentBuffer = new BYTE[dwSilentBufferSize];

		int iSilentValue = 0;

		if( WaveFormatEx.wBitsPerSample == 8 )
			iSilentValue = 127;
		else if( WaveFormatEx.wBitsPerSample == 16 )
			iSilentValue = 0;

		memset( pSilentBuffer, iSilentValue, dwSilentBufferSize );
	}

	//Create the Timer Thread
	tTimerThread = std::thread( &SoundStream::ThreadTimer, this );
}

SoundStream::~SoundStream()
{
	bTerminateTimerThread = TRUE;

	//Can Wait for Timer Thread?
	if( tTimerThread.joinable() )
	{
		//Wait for Timer Thread
		tTimerThread.join();
	}

	DELETA( pSilentBuffer );

	RELEASE( lpDSoundBuffer );
	RELEASE( lpDS );
}

void SoundStream::NewAudio( const BYTE * pBuffer, DWORD dwBufferSize )
{
	if( lpDSoundBuffer == NULL )
		return;

	//Add Audio Buffer
	AppendBuffer( pBuffer, dwBufferSize, TRUE );

	//Add Silent Buffer at the end as a Grace
	AppendBuffer( pSilentBuffer, dwSilentBufferSize, FALSE );
}

void SoundStream::Play()
{
	std::lock_guard<std::recursive_mutex> lg(rmMutex);

	if( lpDSoundBuffer == NULL )
		return;

	//Already Playing?
	if( IsPlaying() )
		return;

	//Nothing in Buffer to Play?
	if( dwBytesInBuffer == 0 )
		return;

	//Play!
	if( lpDSoundBuffer->Play( 0, 0, DSBPLAY_LOOPING ) == DSERR_BUFFERLOST )
	{
		//Restore if Buffer is Lost
		lpDSoundBuffer->Restore();

		//Restore Position
		lpDSoundBuffer->SetCurrentPosition( 0 );

		//Restore Write Positions
		dwWritePosition = 0;
		dwBytesInBuffer = 0;
	}
}

BOOL SoundStream::IsPlaying()
{
	std::lock_guard<std::recursive_mutex> lg(rmMutex);

	if( lpDSoundBuffer == NULL )
		return FALSE;

	//Get Status
	DWORD dwStatus = 0;
	if( DSFAILED( lpDSoundBuffer->GetStatus( &dwStatus ) ) )
		return FALSE;
	
	//Status marked as Playing?
	if( dwStatus & DSBSTATUS_PLAYING )
		return TRUE;

	return FALSE;
}

void SoundStream::Stop()
{
	std::lock_guard<std::recursive_mutex> lg(rmMutex);

	if( lpDSoundBuffer == NULL )
		return;

	//Reset Bytes in Buffer to Play Count
	dwBytesInBuffer = 0;

	//Stop!
	lpDSoundBuffer->Stop();

	//Get Buffer Write Cursor
	DWORD dwCurrentWriteCursor;
	if( DSSUCCEEDED( lpDSoundBuffer->GetCurrentPosition( NULL, &dwCurrentWriteCursor ) ) )
	{
		//Correct Write Position
		dwWritePosition = dwCurrentWriteCursor;
	}
}

BOOL SoundStream::HasGlobalFocus() const
{
	return (DSBCaps.dwFlags & DSBCAPS_GLOBALFOCUS);
}

const WAVEFORMATEX & SoundStream::GetWaveFormat() const
{
	return WaveFormatEx;
}

int SoundStream::GetVolume() const
{
	return iVolume;
}

void SoundStream::SetVolume( int iNewVolume )
{
	if( lpDSoundBuffer == NULL )
		return;

	//Update the Volume member variable
	iVolume = iNewVolume;

	//Set the Buffer Volume
	lpDSoundBuffer->SetVolume( Sound::VolumeToDirectSoundVolume( iVolume ) );
}

void SoundStream::AppendBuffer( const BYTE * pBuffer, DWORD dwBufferSize, BOOL bMoveWritePosition )
{
	std::lock_guard<std::recursive_mutex> lg(rmMutex);

	if( lpDSoundBuffer == NULL )
		return;

	if( dwBufferSize == 0 )
		return;

	if( DSBCaps.dwSize == 0 )
		return;

	//Get Play and Write Positions
	DWORD dwCurrentPlayCursor, dwCurrentWriteCursor;
	if( DSFAILED( lpDSoundBuffer->GetCurrentPosition( &dwCurrentPlayCursor, &dwCurrentWriteCursor ) ) )
		return;

	//Not the Silent Buffer?
	if( bMoveWritePosition )
	{
		//Is Playing?
		if( IsPlaying() )
		{
			//No Space Left in Buffer till Play Cursor to fit in the new Buffer?
			if( (dwWritePosition < dwCurrentPlayCursor) && (dwCurrentPlayCursor < (dwWritePosition + dwBufferSize)) )
			{
				//Start Replacing Data at the Write Cursor
				dwWritePosition = dwCurrentWriteCursor;

				//Reset Bytes in Buffer Count (data is lost)
				dwBytesInBuffer = 0;

				//Reset
				dwLastCurrentPlayCursor = dwCurrentPlayCursor;
			}
		}
		else
		{
			//Reset
			dwWritePosition = 0;
			
			//Start with No Bytes in the Buffer
			dwBytesInBuffer = 0;

			//Reset Play Cursor
			dwLastCurrentPlayCursor = 0;
			
			//Reset Buffer Positions
			lpDSoundBuffer->SetCurrentPosition( 0 );
		}
	}

	void * pBuffer1 = NULL, * pBuffer2 = NULL;
	DWORD dwBuffer1Size = 0, dwBuffer2Size = 0;

	//Lock DirectSound Buffer. Buffer Lost? Try to Restore and Try Again
	HRESULT hr;
	if( hr = lpDSoundBuffer->Lock( dwWritePosition, dwBufferSize, &pBuffer1, &dwBuffer1Size, &pBuffer2, &dwBuffer2Size, 0 ) == DSERR_BUFFERLOST )
	{
		//Restore
		lpDSoundBuffer->Restore();
		
		//Reset Write Position
		dwWritePosition = 0;

		//Start with No Bytes in the Buffer
		dwBytesInBuffer = 0;

		//Reset Play Cursor
		dwLastCurrentPlayCursor = 0;
			
		//Reset Buffer Positions
		lpDSoundBuffer->SetCurrentPosition( 0 );

		//Try to Lock once more
		hr = lpDSoundBuffer->Lock( dwWritePosition, dwBufferSize, &pBuffer1, &dwBuffer1Size, &pBuffer2, &dwBuffer2Size, 0 );
	}

	if( DSFAILED( hr ) )
		return;

	DWORD dwBytesWritten = 0;
	
	//Copy Data 1
	memcpy( pBuffer1, pBuffer, dwBuffer1Size );
	dwBytesWritten += dwBuffer1Size;

	//Copy Data 2
	if( (pBuffer2) && (dwBuffer2Size > 0) )
	{
		memcpy( pBuffer2, pBuffer + dwBuffer1Size, dwBuffer2Size );
		dwBytesWritten += dwBuffer2Size;
	}

	//Unlock DirectSound Buffer
	lpDSoundBuffer->Unlock( pBuffer1, dwBuffer1Size, pBuffer2, dwBuffer2Size );

	//Move Write Position
	if( bMoveWritePosition )
	{
		if( pBuffer2 )
			dwWritePosition = dwBuffer2Size;
		else
			dwWritePosition += dwBuffer1Size;

		dwWritePosition %= DSBCaps.dwBufferBytes;

		dwBytesInBuffer += dwBytesWritten;

		if( dwBytesInBuffer > DSBCaps.dwBufferBytes )
			dwBytesInBuffer = DSBCaps.dwBufferBytes;
	}
}

void SoundStream::ThreadTimer()
{
	/*
	Engine::Timer tTimer;
	Engine::CountTimer tCountTimer( SOUNDSTREAM_STOP_PRECISION_MS );

	while( !bTerminateTimerThread )
	{
		if( tCountTimer.Update( tTimer.Update() ) > 0 )
		{
			std::lock_guard<std::recursive_mutex> lg(rmMutex);

			//Sound Stream is Playing?
			if( IsPlaying() )
			{
				DWORD dwCurrentPlayCursor;
				if( DSFAILED( lpDSoundBuffer->GetCurrentPosition( &dwCurrentPlayCursor, NULL ) ) )
					return;

				DWORD dwBytesProcessed = 0;

				if( dwCurrentPlayCursor < dwLastCurrentPlayCursor )
				{
					dwBytesProcessed += DSBCaps.dwBufferBytes - dwLastCurrentPlayCursor;

					dwLastCurrentPlayCursor = 0;
				}

				dwBytesProcessed += dwCurrentPlayCursor - dwLastCurrentPlayCursor;
				dwLastCurrentPlayCursor = dwCurrentPlayCursor;

				if( dwBytesProcessed >= dwBytesInBuffer )
					dwBytesInBuffer = 0;
				else
					dwBytesInBuffer -= dwBytesProcessed;

				//No more Bytes in Buffer left to Play?
				if( dwBytesInBuffer == 0 )
				{
					//Stop Playing
					Stop();
				}
			}
		}

		//Sleep for a while
		Sleep( SOUNDSTREAM_STOP_PRECISION_MS >> 1 );
	}
	*/
}

}