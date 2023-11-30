#pragma once

#include "DXSound.h"
#include "DXSoundStream.h"

namespace DX
{

class SoundDevice;

typedef std::shared_ptr<class SoundManager> SoundManager_ptr;

class SoundManager
{
public:
					  SoundManager( SoundDevice * pSoundDevice );

	void			  SoundPlay( Sound_ptr pSound, int iVolume = MAX_VOLUME );

	void			  SoundStreamPlay( SoundStream_ptr pSoundStream, int iVolume = MAX_VOLUME );
	void			  SoundStreamUpdate( SoundStream_ptr pSoundStream, int iVolume = MAX_VOLUME );

	BOOL			  GetMasterMusic() { return bMasterMusic; }
	void			  SetMasterMusic( BOOL bValue );
	int				  GetMasterMusicVolume() { return iMasterSoundVolume; }
	void			  SetMasterMusicVolume( int iValue );

	BOOL			  GetMasterSound() { return bMasterSound; }
	void			  SetMasterSound( BOOL bValue );
	int				  GetMasterSoundVolume() { return iMasterSoundVolume; }
	void			  SetMasterSoundVolume( int iValue );

	BOOL			  GetMasterVoice() { return bMasterVoice; }
	void			  SetMasterVoice( BOOL bValue );
	BOOL			  GetGlobalVoice() { return bGlobalVoice; }
	void			  SetGlobalVoice( BOOL bValue );
	BOOL			  GetDisableMic() { return bDisableMic; }
	void			  SetDisableMic( BOOL bValue );
	int				  GetMasterVoiceVolume() { return iMasterVoiceVolume; }
	void			  SetMasterVoiceVolume( int iValue );

private:
	SoundDevice		* pSoundDevice = NULL;

	BOOL			  bMasterMusic = TRUE;
	int				  iMasterMusicVolume = MAX_VOLUME;
	BOOL			  bMasterSound = TRUE;
	int				  iMasterSoundVolume = MAX_VOLUME;
	BOOL			  bMasterVoice = TRUE;
	BOOL			  bGlobalVoice = TRUE;
	BOOL			  bDisableMic = FALSE;
	int				  iMasterVoiceVolume = MAX_VOLUME;
};

}