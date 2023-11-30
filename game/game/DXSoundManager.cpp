#include "stdafx.h"
#include "DXSoundManager.h"

#include "DXSoundDevice.h"

namespace DX
{

SoundManager::SoundManager( SoundDevice * pSoundDevice ) : pSoundDevice(pSoundDevice)
{
}

void SoundManager::SoundPlay( Sound_ptr pSound, int iVolume )
{
	//No Sound Device?
	if( !pSoundDevice->HasDevice() )
		return;

	//Master Sound disabled?
	if( !bMasterSound )
		return;

	//Invalid Sound Volume
	if( (iMasterSoundVolume < MIN_VOLUME) || (iVolume < MIN_VOLUME) )
		return;

	//Invalid Sound Volume
	if( (iMasterSoundVolume > MAX_VOLUME) || (iVolume > MAX_VOLUME) )
		return;

	//Window not Active?
//	if( (pSound->HasGlobalFocus() == FALSE) && ((pSoundDevice->GetWindow()->IsOnForeground() == FALSE) || (pSoundDevice->GetWindow()->IsMinimized())) )
//		return;

	//Final Volume
	int iFinalVolume = (iMasterSoundVolume * iVolume) / MAX_VOLUME;

	//Play Sound
	if( pSound->Play( FALSE, iFinalVolume ) == 0 )
	{
		//Reload Required
		pSoundDevice->GetSoundFactory()->ReloadSound( pSound );

		//Try once more
		pSound->Play( FALSE, iFinalVolume );
	}
}

void SoundManager::SoundStreamPlay( SoundStream_ptr pSoundStream, int iVolume )
{
	//No Sound Device?
	if( !pSoundDevice->HasDevice() )
		return;

	//Master Voice disabled?
	if( !bMasterVoice )
		return;

	//Invalid Voice Volume
	if( (iMasterVoiceVolume < MIN_VOLUME) || (iVolume < MIN_VOLUME) )
		return;

	//Invalid Voice Volume
	if( (iMasterVoiceVolume > MAX_VOLUME) || (iVolume > MAX_VOLUME) )
		return;

	//Final Volume
	int iFinalVolume = (iMasterVoiceVolume * iVolume) / MAX_VOLUME;

	//Set Sound Stream Volume
	pSoundStream->SetVolume( iFinalVolume );
	
	//Play Sound Stream
	pSoundStream->Play();
}

void SoundManager::SoundStreamUpdate( SoundStream_ptr pSoundStream, int iVolume )
{
	//No Sound Device?
	if( !pSoundDevice->HasDevice() )
		return;

	//Invalid Voice Volume
	if( (iMasterVoiceVolume < MIN_VOLUME) || (iVolume < MIN_VOLUME) )
		return;

	//Invalid Voice Volume
	if( (iMasterVoiceVolume > MAX_VOLUME) || (iVolume > MAX_VOLUME) )
		return;

	//Final Volume
	int iFinalVolume = (iMasterVoiceVolume * iVolume) / MAX_VOLUME;

	//Set Sound Stream Volume
	pSoundStream->SetVolume( iFinalVolume );
}

void SoundManager::SetMasterMusic( BOOL bValue )
{
	bMasterMusic = bValue;
}

void SoundManager::SetMasterMusicVolume( int iValue )
{
	iMasterMusicVolume = iValue;
}

void SoundManager::SetMasterSound( BOOL bValue )
{
	bMasterSound = bValue;
}

void SoundManager::SetMasterSoundVolume( int iValue )
{
	iMasterSoundVolume = iValue;
}

void SoundManager::SetMasterVoice( BOOL bValue )
{
	bMasterVoice = bValue;
}

void SoundManager::SetGlobalVoice( BOOL bValue )
{
	bGlobalVoice = bValue;
}

void SoundManager::SetDisableMic( BOOL bValue )
{
	bDisableMic = bValue;
}

void SoundManager::SetMasterVoiceVolume( int iValue )
{
	iMasterVoiceVolume = iValue;
}

}