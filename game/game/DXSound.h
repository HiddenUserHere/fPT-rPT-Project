#pragma once

namespace DX
{

const int MIN_VOLUME = 0;
const int MAX_VOLUME = 100;

typedef std::shared_ptr<class Sound> Sound_ptr;

class Sound
{
public:
										  Sound( LPDIRECTSOUND8 lpDS, LPDIRECTSOUNDBUFFER8 lpDSoundBuffer, std::string strSoundFile, BOOL bHasGlobalFocus );
	virtual								 ~Sound();

	size_t								  GetListSize() { return vSoundBuffers.size(); }
	const std::string					& GetFile() { return strSoundFile; }

	void								  AddBuffer( LPDIRECTSOUNDBUFFER8 lpDSoundBuffer );
	void								  RemoveBuffers();

	int									  Play( BOOL bLoop = FALSE, LONG lPlayVolume = MAX_VOLUME, DWORD dwFrequency = DSBFREQUENCY_ORIGINAL );
	BOOL								  IsPlaying();
	void								  Stop();

	BOOL								  HasGlobalFocus() const { return bHasGlobalFocus; }

	int									  GetVolume() const;
	void								  SetVolume( LONG lNewVolume );

public:
	static LONG							  VolumeToDirectSoundVolume( LONG lVolume );

private:
	LPDIRECTSOUND8						  lpDS;
	std::vector<LPDIRECTSOUNDBUFFER8>	  vSoundBuffers;
	std::string							  strSoundFile;
	BOOL								  bHasGlobalFocus;

	UINT								  uPlayWheel = 0;
	LONG								  lVolume = MIN_VOLUME;
};

}