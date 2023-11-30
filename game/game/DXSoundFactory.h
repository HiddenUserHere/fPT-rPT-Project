#pragma once

#include "DXSound.h"
#include "DXSoundStream.h"

namespace DX
{

class SoundDevice;

typedef std::shared_ptr<class SoundFactory> SoundFactory_ptr;

class SoundFactory
{
	friend SoundDevice;

public:
								  SoundFactory( SoundDevice * pSoundDevice );

	Sound_ptr					  MakeSound( std::string strFilePath, BOOL bGlobalFocus = FALSE, size_t iSoundRedundancyCount = 3 );

	Sound_ptr					  MakeTemporarySound( std::string strFilePath, BOOL bGlobalFocus = FALSE );

	SoundStream_ptr				  MakeSoundStream( WAVEFORMATEX & WaveFormatEx, int iBufferSizeInSeconds, BOOL bGlobalFocus = FALSE );

	void						  ReloadSound( Sound_ptr pSound );
	void						  ReloadSounds();

	void						  ClearCache();

private:
	LPDIRECTSOUNDBUFFER8		  CreateSoundBufferFromFile( std::string strSoundFile, BOOL bGlobalFocus );
	LPDIRECTSOUNDBUFFER8		  CreateSoundBuffer( WAVEFORMATEX & WaveFormatEx, int iBufferSizeInSeconds, BOOL bGlobalFocus );
	LPDIRECTSOUNDBUFFER8		  DuplicateSoundBuffer( LPDIRECTSOUNDBUFFER8 lpSoundBuffer );

private:
	SoundDevice					* pSoundDevice = NULL;

	std::vector<Sound_ptr>		  vCache;
};

}