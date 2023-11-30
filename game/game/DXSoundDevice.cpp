#include "stdafx.h"
#include "DXSoundDevice.h"

#include "DX.h"
#include "DXLogger.h"

namespace DX
{

SoundDevice::SoundDevice()
{
	pSoundFactory = std::make_shared<SoundFactory>( this );
	pSoundManager = std::make_shared<SoundManager>( this );
}

SoundDevice::~SoundDevice()
{
	pSoundManager = nullptr;
	pSoundFactory = nullptr;
}

BOOL SoundDevice::Init( CWindow * _pWindow )
{
	pWindow = _pWindow;

	HRESULT hr;

	//Create Direct Sound 8
	if( DSFAILED( hr = DirectSoundCreate8( NULL, &lpDSoundDevice, NULL ) ) )
	{
		lpDSoundDevice = NULL;

		//Log Error
		//Logger::GetInstance().Print( Logger::Release, hr, "[Sound Device] DirectSoundCreate8 ERROR" );

		return FALSE;
	}

	//Set the Cooperative Level
	if( DSFAILED( hr = lpDSoundDevice->SetCooperativeLevel( pWindow->GetWindowHandle(), DSSCL_PRIORITY ) ) )
	{
		RELEASE( lpDSoundDevice );
		
		//Log Error
		//Logger::GetInstance().Print( Logger::Release, hr, "[Sound Device] IDirectSound8::SetCooperativeLevel ERROR" );

		return FALSE;
	}
	
	//Log Success
	//Logger::GetInstance().Print( Logger::Release, "[Sound Device] Initialized Successfully" );

	return TRUE;
}

void SoundDevice::Shutdown()
{
	//Remove Sounds from Cache
	pSoundFactory->ClearCache();

	//Remove our Sound Device Reference
	RELEASE( lpDSoundDevice );
}

BOOL SoundDevice::HasDevice() const
{
	//Sound not available?
	if( lpDSoundDevice == NULL )
		return FALSE;

	return TRUE;
}

}