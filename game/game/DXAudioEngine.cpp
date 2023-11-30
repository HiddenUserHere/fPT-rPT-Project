#include "stdafx.h"
#include "DXAudioEngine.h"

#include "DLL.h"

using namespace DX;

DXAudioEngine::DXAudioEngine()
{
	iSoundVolume	= MAX_VOLUME;
	fSoundVolumeD	= 1.0f;
	iVolumeDivider	= GAME_VOLUME_MAX / 100;
	
	ZeroMemory( szaSound, sizeof( szaSound ) );
}

void DXAudioEngine::Shutdown()
{
	for( int i = 0; i < NUM_SOUNDS; i++ )
		paSound[i] = nullptr;

	cMusic.Reset();
}

void DXAudioEngine::PrepareSound( UINT iIndex, const char * pszFilePath )
{
	if( iIndex < NUM_SOUNDS )
		STRINGCOPY( szaSound[iIndex], pszFilePath );
}

void DXAudioEngine::LoadAllSounds()
{
	for( int i = 0; i < NUM_SOUNDS; i++ )
	{
		char * pszSound = szaSound[i];

		if( pszSound[0] != 0 )
			LoadSound( i, pszSound );
	}
}

BOOL DXAudioEngine::LoadSound( UINT iIndex, const char * pszFilePath )
{
	if( iIndex < NUM_SOUNDS )
	{
		//Replace Sound
		paSound[iIndex] = DX::GetSoundDevice()->GetSoundFactory()->MakeSound( pszFilePath );
	}

	return FALSE;
}

void DXAudioEngine::Update( double fTime )
{
	cMusic.Fade();
}

void DXAudioEngine::SetVolume( int iNewSoundVolume )
{
	iSoundVolume = iNewSoundVolume;
	if( iNewSoundVolume != 0 )
		fSoundVolumeD = (float)MAX_VOLUME / (float)iNewSoundVolume;
	else
		fSoundVolumeD = 0.0f;

	//Update Sounds Volume
	for( UINT i = 0; i < NUM_SOUNDS; i++ )
		SetSoundVolume( i, iSoundVolume );
}

void DXAudioEngine::PlaySound( UINT iIndex, BOOL bLoop, int iVolume, DWORD dwFrequency )
{
	if( (bSound != FALSE) && (fSoundVolumeD != 0.0f) )
	{
		if( iIndex < NUM_SOUNDS )
		{
			DX::Sound_ptr pSound = paSound[iIndex];

			if( pSound )
			{
				int iSetVolume = (int)((float)((iVolume / iVolumeDivider)) / fSoundVolumeD);

				pSound->Play( bLoop, iSetVolume, dwFrequency );
			}
		}
	}
}

void DXAudioEngine::StopSound( UINT iIndex )
{
	if( iIndex < NUM_SOUNDS )
	{
		DX::Sound_ptr pSound = paSound[iIndex];

		if( pSound )
			pSound->Stop();
	}
}

BOOL DXAudioEngine::IsSoundPlaying( UINT iIndex )
{
	if( iIndex < NUM_SOUNDS )
	{
		DX::Sound_ptr pSound = paSound[iIndex];

		if( pSound )
			return pSound->IsPlaying();
	}

	return FALSE;
}

void DXAudioEngine::SetSoundVolume( UINT iIndex, int iVolume )
{
	if( iIndex < NUM_SOUNDS )
	{
		DX::Sound_ptr pSound = paSound[iIndex];

		if( pSound )
		{
			int iSetVolume = (int)((float)((iVolume / iVolumeDivider)) / fSoundVolumeD);

			pSound->SetVolume( iSetVolume );
		}
	}
}