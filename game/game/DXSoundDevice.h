#pragma once


#include "DXSoundFactory.h"
#include "DXSoundManager.h"

namespace DX
{

typedef std::shared_ptr<class SoundDevice> SoundDevice_ptr;

class SoundDevice : public std::enable_shared_from_this<SoundDevice>
{
public:
							  SoundDevice();
	virtual					 ~SoundDevice();

	BOOL					  Init( CWindow * _pWindow );
	void					  Shutdown();

	BOOL					  HasDevice() const;

	CWindow *				  GetWindow() { return pWindow; }

	LPDIRECTSOUND8			  Get() { return lpDSoundDevice; }

	SoundFactory_ptr		  GetSoundFactory() { return pSoundFactory; }
	SoundManager_ptr		  GetSoundManager() { return pSoundManager; }

private:
	CWindow *				  pWindow;

	LPDIRECTSOUND8			  lpDSoundDevice = NULL;

	SoundFactory_ptr		  pSoundFactory;
	SoundManager_ptr		  pSoundManager;
};

}