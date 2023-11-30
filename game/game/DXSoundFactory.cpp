#include "stdafx.h"
#include "DXSoundFactory.h"

#include "DX.h"
#include "DXSoundDevice.h"

namespace DX
{

SoundFactory::SoundFactory( SoundDevice * pSoundDevice ) : pSoundDevice(pSoundDevice)
{
	vCache.reserve( 1000 );
}

Sound_ptr SoundFactory::MakeSound( std::string strFilePath, BOOL bGlobalFocus, size_t iSoundRedundancyCount )
{
	//No Sound Device?
	if( !pSoundDevice->HasDevice() )
		return nullptr;

	//In Cache?
	for( auto pSound : vCache )
	{
		//Same File?
		if( STRINGCOMPAREI( strFilePath.c_str(), pSound->GetFile().c_str()) )
			return pSound;
	}

	//Make New Sound Buffer from File
	LPDIRECTSOUNDBUFFER8 lpSoundBuffer = CreateSoundBufferFromFile( strFilePath, bGlobalFocus );

	//New Sound (adds Reference)
	Sound_ptr pSound = std::make_shared<Sound>( pSoundDevice->Get(), lpSoundBuffer, strFilePath, bGlobalFocus );

	//Add Duplicate Sound Buffers to Sound
	for( size_t i = 0; i < iSoundRedundancyCount - 1; i++ )
	{
		//Duplicate Sound Buffer
		LPDIRECTSOUNDBUFFER8 lpSoundBufferDuplicate = DuplicateSoundBuffer( lpSoundBuffer );

		//Add to Sound (adds Reference)
		pSound->AddBuffer( lpSoundBufferDuplicate );

		//Release our Reference
		RELEASE( lpSoundBufferDuplicate );
	}
	
	//Release our Reference
	RELEASE( lpSoundBuffer );

	//Cache New Sound
	vCache.push_back( pSound );

	return pSound;
}

Sound_ptr SoundFactory::MakeTemporarySound( std::string strFilePath, BOOL bGlobalFocus )
{
	//No Sound Device?
	if( !pSoundDevice->HasDevice() )
		return nullptr;

	//Make New Sound Buffer from File
	LPDIRECTSOUNDBUFFER8 lpSoundBuffer = CreateSoundBufferFromFile( strFilePath, bGlobalFocus );

	//New Sound (adds Reference)
	Sound_ptr pSound = std::make_shared<Sound>( pSoundDevice->Get(), lpSoundBuffer, strFilePath, bGlobalFocus );
	
	//Release our Reference
	RELEASE( lpSoundBuffer );

	return pSound;
}

SoundStream_ptr SoundFactory::MakeSoundStream( WAVEFORMATEX & WaveFormatEx, int iBufferSizeInSeconds, BOOL bGlobalFocus )
{
	//No Sound Device?
	if( !pSoundDevice->HasDevice() )
		return nullptr;

	//Make New Sound Buffer
	LPDIRECTSOUNDBUFFER8 lpSoundBuffer = CreateSoundBuffer( WaveFormatEx, iBufferSizeInSeconds, bGlobalFocus );

	//New Sound Stream (adds Reference)
	SoundStream_ptr pSoundStream = std::make_shared<SoundStream>( pSoundDevice->Get(), lpSoundBuffer );

	//Release our Reference
	RELEASE( lpSoundBuffer );

	return pSoundStream;
}

void SoundFactory::ReloadSound( Sound_ptr pSound )
{
	//Make New Sound Buffer from File
	LPDIRECTSOUNDBUFFER8 lpSoundBuffer = CreateSoundBufferFromFile( pSound->GetFile(), pSound->HasGlobalFocus() );

	//Get Sound Redundancy Count
	size_t iSoundRedundancyCount = pSound->GetListSize();

	//Remove Sound Buffers in Sound
	pSound->RemoveBuffers();

	//Add Sound Buffer to Sound (adds Reference)
	pSound->AddBuffer( lpSoundBuffer );
	
	//Add Duplicate Sound Buffers to Sound
	for( size_t i = 0; i < iSoundRedundancyCount - 1; i++ )
	{
		//Duplicate Sound Buffer
		LPDIRECTSOUNDBUFFER8 lpSoundBufferDuplicate = DuplicateSoundBuffer( lpSoundBuffer );

		//Add to Sound (adds Reference)
		pSound->AddBuffer( lpSoundBufferDuplicate );

		//Release our Reference
		RELEASE( lpSoundBufferDuplicate );
	}
	
	//Release our Reference
	RELEASE( lpSoundBuffer );
}

void SoundFactory::ReloadSounds()
{
	//Loop through Sounds in Cache
	for( auto pSound : vCache )
	{
		//Reload Sound
		ReloadSound( pSound );
	}
}

void SoundFactory::ClearCache()
{
	vCache.clear();
}

LPDIRECTSOUNDBUFFER8 SoundFactory::CreateSoundBufferFromFile( std::string strSoundFile, BOOL bGlobalFocus )
{
	//No Sound Device?
	if( !pSoundDevice->HasDevice() )
		return NULL;

	//File does not exist?
	if( PathFileExistsA( strSoundFile.c_str() ) == FALSE )
		return NULL;

	WAVEFORMATEX * pWaveFormatEx;
	HMMIO hMMIO;
	MMCKINFO mmckInfo;
	MMCKINFO mmckInfoParent;

	//Open the File
	if( WaveOpenFile( (char*)strSoundFile.c_str(), &hMMIO, &pWaveFormatEx, &mmckInfoParent ) != 0 )
		return NULL;

	//Prepare File for Reading
	if( WaveStartDataRead( &hMMIO, &mmckInfo, &mmckInfoParent ) != 0 )
	{
		WaveCloseReadFile( &hMMIO, &pWaveFormatEx );
		return NULL;
	}

	//Optional Flags
	DWORD dwOptionalFlags = 0;

	if( bGlobalFocus )
		dwOptionalFlags |= DSBCAPS_GLOBALFOCUS;
	
	//Set DirectSound Buffer Description
	DSBUFFERDESC DSBufferDesc;
	ZeroMemory( &DSBufferDesc, sizeof( DSBUFFERDESC ) );
	DSBufferDesc.dwSize				= sizeof( DSBUFFERDESC );
	DSBufferDesc.dwFlags			= dwOptionalFlags | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY; 
	DSBufferDesc.dwBufferBytes		= mmckInfo.cksize;
	DSBufferDesc.lpwfxFormat		= pWaveFormatEx;
	DSBufferDesc.dwReserved			= 0;
	DSBufferDesc.guid3DAlgorithm	= GUID_NULL;

	//Create Temporary DirectSound Buffer
	LPDIRECTSOUNDBUFFER lpTempSoundBuffer = NULL;
	if( DSFAILED( pSoundDevice->Get()->CreateSoundBuffer( &DSBufferDesc, &lpTempSoundBuffer, NULL ) ) )
	{
		WaveCloseReadFile( &hMMIO, &pWaveFormatEx );
		return NULL; 
	}
	
	//DirectSound Buffer to Return
	LPDIRECTSOUNDBUFFER8 lpSoundBuffer = NULL;
	
	//Query Interface for DirectSoundBuffer8
	if( FAILED( lpTempSoundBuffer->QueryInterface( IID_IDirectSoundBuffer8, (LPVOID *)&lpSoundBuffer ) ) )
	{
		RELEASE( lpTempSoundBuffer );

		WaveCloseReadFile( &hMMIO, &pWaveFormatEx );
		return NULL; 
	}
	
	//Release Temporary DirectSound Buffer
	RELEASE( lpTempSoundBuffer );

	void * pBuffer = NULL;
	DWORD dwBufferSize = 0;

	//Lock the DirectSound Buffer
	if( DSFAILED( lpSoundBuffer->Lock( 0, 0, &pBuffer, &dwBufferSize, NULL, NULL, DSBLOCK_ENTIREBUFFER ) ) )
	{
		RELEASE( lpSoundBuffer );

		WaveCloseReadFile( &hMMIO, &pWaveFormatEx );
		return NULL;
	}

	UINT uBytesRead = 0;

	//Read Audio Data to DirectSound Buffer
	if( WaveReadFile( hMMIO, dwBufferSize, (BYTE *)pBuffer, &mmckInfo, &uBytesRead ) != 0 )
	{
		RELEASE( lpSoundBuffer );

		WaveCloseReadFile( &hMMIO, &pWaveFormatEx );
		return NULL;
	}

	//Unlock the DirectSound Buffer
	lpSoundBuffer->Unlock( pBuffer, dwBufferSize, NULL, 0 );

	//Close File
	WaveCloseReadFile( &hMMIO, &pWaveFormatEx );

	return lpSoundBuffer;
}

LPDIRECTSOUNDBUFFER8 SoundFactory::CreateSoundBuffer( WAVEFORMATEX & WaveFormatEx, int iBufferSizeInSeconds, BOOL bGlobalFocus )
{
	//No Sound Device?
	if( !pSoundDevice->HasDevice() )
		return NULL;

	//Invalid Buffer Size In Seconds?
	if( iBufferSizeInSeconds <= 0 )
		return NULL;

	//Optional Flags
	DWORD dwOptionalFlags = 0;

	if( bGlobalFocus )
		dwOptionalFlags |= DSBCAPS_GLOBALFOCUS;

	//Set DirectSound Buffer Description
	DSBUFFERDESC DSBufferDesc;
	ZeroMemory( &DSBufferDesc, sizeof( DSBUFFERDESC ) );
	DSBufferDesc.dwSize				= sizeof( DSBUFFERDESC );
	DSBufferDesc.dwFlags			= dwOptionalFlags | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY; 
	DSBufferDesc.dwBufferBytes		= WaveFormatEx.nAvgBytesPerSec * iBufferSizeInSeconds;
	DSBufferDesc.lpwfxFormat		= &WaveFormatEx;
	DSBufferDesc.dwReserved			= 0;
	DSBufferDesc.guid3DAlgorithm	= GUID_NULL;
	
	//Create Temporary DirectSound Buffer
	LPDIRECTSOUNDBUFFER lpTempSoundBuffer = NULL;
	if( DSFAILED( pSoundDevice->Get()->CreateSoundBuffer( &DSBufferDesc, &lpTempSoundBuffer, NULL ) ) )
		return NULL;

	//Query Interface for DirectSoundBuffer8
	LPDIRECTSOUNDBUFFER8 lpSoundBuffer = NULL;
	if( FAILED( lpTempSoundBuffer->QueryInterface( IID_IDirectSoundBuffer8, (LPVOID *)&lpSoundBuffer ) ) )
	{
		RELEASE( lpTempSoundBuffer );

		return NULL;
	}

	RELEASE( lpTempSoundBuffer );

	//Silent the Buffer
	{
		void * pBuffer = NULL;
		DWORD dwBufferSize = 0;

		//Lock the Buffer
		if( DSSUCCEEDED( lpSoundBuffer->Lock( 0, 0, &pBuffer, &dwBufferSize, NULL, NULL, DSBLOCK_ENTIREBUFFER ) ) )
		{
			int iSilentValue = 0;

			//Choose Silent Byte based on Bits per Sample
			if( WaveFormatEx.wBitsPerSample == 8 )
				iSilentValue = 127;
			else if( WaveFormatEx.wBitsPerSample == 16 )
				iSilentValue = 0;

			//Silent the Buffer
			memset( pBuffer, iSilentValue, dwBufferSize );

			//Unlock the Buffer
			lpSoundBuffer->Unlock( pBuffer, dwBufferSize, NULL, 0 );
		}
	}

	return lpSoundBuffer;
}

LPDIRECTSOUNDBUFFER8 SoundFactory::DuplicateSoundBuffer( LPDIRECTSOUNDBUFFER8 lpSoundBuffer )
{
	//No Sound Device?
	if( !pSoundDevice->HasDevice() )
		return NULL;

	//Invalid Sound Buffer?
	if( lpSoundBuffer == NULL )
		return NULL;

	//Return Variable
	LPDIRECTSOUNDBUFFER8 lpSoundBufferDuplicate = NULL;

	//Duplicate Temporary DirectSound Buffer
	LPDIRECTSOUNDBUFFER lpTempSoundBuffer = NULL;
	if( DSSUCCEEDED( pSoundDevice->Get()->DuplicateSoundBuffer( lpSoundBuffer, &lpTempSoundBuffer ) ) )
	{
		//Query Interface for DirectSoundBuffer8
		if( FAILED( lpTempSoundBuffer->QueryInterface( IID_IDirectSoundBuffer8, (LPVOID *)&lpSoundBufferDuplicate ) ) )
		{
			RELEASE( lpTempSoundBuffer );

			return NULL;
		}
	}
	RELEASE( lpTempSoundBuffer );

	return lpSoundBufferDuplicate;
}

}