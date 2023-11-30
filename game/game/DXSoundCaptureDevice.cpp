#include "stdafx.h"
#include "DXSoundCaptureDevice.h"

#include "DX.h"
#include "DXLogger.h"
//#include "CountTimer.h"
#include "Timer.h"

namespace DX
{

WAVEFORMATEX RecordingWaveFormatEx = { WAVE_FORMAT_PCM, 1, 8000, 16000, 2, 16, 0 };

SoundCaptureDevice::SoundCaptureDevice()
{
}

SoundCaptureDevice::~SoundCaptureDevice()
{
}

BOOL SoundCaptureDevice::Init( CWindow * _pWindow )
{
	pWindow = _pWindow;

	HRESULT hr;
	
	//Create
	{
		hr = DirectSoundCaptureCreate8( NULL, &lpDSoundCapture, NULL );

		if( DSFAILED( hr ) )
		{
			lpDSoundCapture = NULL;

			//Log Error
			//Logger::GetInstance().Print( Logger::Release, hr, "[Sound Capture Device] DirectSoundCaptureCreate8 ERROR" );

			return FALSE;
		}
	}

	//Get Caps
	{
		ZeroMemory( &DSCCaps, sizeof( DSCCAPS ) );
		DSCCaps.dwSize = sizeof( DSCCAPS );

		hr = lpDSoundCapture->GetCaps( &DSCCaps );

		if( DSFAILED( hr ) )
		{
			RELEASE( lpDSoundCapture );

			//Log Error
			//Logger::GetInstance().Print( Logger::Release, hr, "[Sound Capture Device] IDirectSoundCapture8::GetCaps ERROR" );

			return FALSE;
		}
	}

	//Capture Buffer Creation
	{
		//Set Capture FX Descriptions
		DSCEFFECTDESC DSCEffectDesc[2];
		ZeroMemory( DSCEffectDesc, sizeof( DSCEFFECTDESC ) * _countof( DSCEffectDesc ) );
		DSCEffectDesc[0].dwSize				= sizeof( DSCEFFECTDESC );
		DSCEffectDesc[0].dwFlags			= DSCFX_LOCSOFTWARE;
		DSCEffectDesc[0].guidDSCFXClass		= GUID_DSCFX_CLASS_AEC;
		DSCEffectDesc[0].guidDSCFXInstance	= GUID_DSCFX_MS_AEC;
		DSCEffectDesc[0].dwReserved1		= 0;
		DSCEffectDesc[0].dwReserved2		= 0;
		DSCEffectDesc[1].dwSize				= sizeof( DSCEFFECTDESC );
		DSCEffectDesc[1].dwFlags			= DSCFX_LOCSOFTWARE;
		DSCEffectDesc[1].guidDSCFXClass		= GUID_DSCFX_CLASS_NS;
		DSCEffectDesc[1].guidDSCFXInstance	= GUID_DSCFX_MS_NS;
		DSCEffectDesc[1].dwReserved1		= 0;
		DSCEffectDesc[1].dwReserved2		= 0;

		//Set Capture Buffer Description
		ZeroMemory( &DSCBufferDesc, sizeof( DSCBUFFERDESC ) );
		DSCBufferDesc.dwSize		= sizeof( DSCBUFFERDESC );
		DSCBufferDesc.dwFlags		= DSCBCAPS_CTRLFX;
		DSCBufferDesc.dwBufferBytes	= RecordingWaveFormatEx.nAvgBytesPerSec * iRecordingBufferSizeInSeconds;
		DSCBufferDesc.dwReserved	= 0;
		DSCBufferDesc.lpwfxFormat	= &RecordingWaveFormatEx;
		DSCBufferDesc.dwFXCount		= _countof( DSCEffectDesc );
		DSCBufferDesc.lpDSCFXDesc	= DSCEffectDesc;

		//Try to Create the Capture Buffer
		LPDIRECTSOUNDCAPTUREBUFFER lpDSoundCaptureBufferTemp = NULL;
		hr = lpDSoundCapture->CreateCaptureBuffer( &DSCBufferDesc, &lpDSoundCaptureBufferTemp, NULL );

		//Failed?
		if( DSFAILED( hr ) )
		{
			//Unset FX
			DSCBufferDesc.dwFlags		= 0;
			DSCBufferDesc.dwFXCount		= 0;
			DSCBufferDesc.lpDSCFXDesc	= NULL;

			//Try again (without FX)
			hr = lpDSoundCapture->CreateCaptureBuffer( &DSCBufferDesc, &lpDSoundCaptureBufferTemp, NULL );

			//Failed again?
			if( DSFAILED( hr ) )
			{
				RELEASE( lpDSoundCapture );

				//Log Error
				//Logger::GetInstance().Print( Logger::Release, hr, "[Sound Capture Device] IDirectSoundCapture8::CreateCaptureBuffer ERROR" );

				return FALSE;
			}
		}

		//Query Interface of Direct Sound Capture Buffer 8
		hr = lpDSoundCaptureBufferTemp->QueryInterface( IID_IDirectSoundCaptureBuffer8, (LPVOID*)&lpDSoundCaptureBuffer );

		//Failed?
		if( FAILED( hr ) )
		{
			RELEASE( lpDSoundCaptureBufferTemp );
			RELEASE( lpDSoundCapture );

			//Log Error
			//Logger::GetInstance().Print( Logger::Release, hr, "[Sound Capture Device] IDirectSoundCaptureBuffer::QueryInterface ERROR" );

			return FALSE;
		}

		RELEASE( lpDSoundCaptureBufferTemp );
	}
	
	//Create Timer Thread
	bTerminateTimerThread = FALSE;
	tTimerThread = std::thread( &SoundCaptureDevice::ThreadTimer, this );
	
	//Log Success
	////Logger::GetInstance().Print( Logger::Release, "[Sound Capture Device] Initialized Successfully" );

	return TRUE;
}

void SoundCaptureDevice::Shutdown()
{
	bTerminateTimerThread = TRUE;

	//Stop Recording
	Stop();

	//Can Wait for Timer Thread?
	if( tTimerThread.joinable() )
	{
		//Wait for Timer Thread
		tTimerThread.join();
	}

	RELEASE( lpDSoundCaptureBuffer );
	RELEASE( lpDSoundCapture );
}

BOOL SoundCaptureDevice::HasDevice() const
{
	//Capture not available?
	if( (lpDSoundCapture == NULL) || (lpDSoundCaptureBuffer == NULL) )
		return FALSE;

	//Capture lost?
	DWORD dwCapturePosition = 0;
	if( DSFAILED( lpDSoundCaptureBuffer->GetCurrentPosition( &dwCapturePosition, NULL ) ) )
		return FALSE;

	return TRUE;
}

BOOL SoundCaptureDevice::Start()
{
	//Capture not available?
	if( (lpDSoundCapture == NULL) || (lpDSoundCaptureBuffer == NULL) )
		return FALSE;

	//Get Status
	DWORD dwStatus = 0;
	if( DSFAILED( lpDSoundCaptureBuffer->GetStatus( &dwStatus ) ) )
		return FALSE;

	//Already Capturing?
	if( (dwStatus & DSCBSTATUS_CAPTURING) != 0 )
	{
		bCapturing = TRUE;
		return FALSE;
	}

	//Start Capture
	bCapturing = FALSE; //DSSUCCEEDED( lpDSoundCaptureBuffer->Start( DSCBSTART_LOOPING ) );

	return bCapturing;
}

BOOL SoundCaptureDevice::IsCapturing()
{
	return bCapturing;
}

void SoundCaptureDevice::Stop()
{
	//Capture not available?
	if( (lpDSoundCapture == NULL) || (lpDSoundCaptureBuffer == NULL) )
		return;

	//Get Status
	DWORD dwStatus = 0;
	if( DSFAILED( lpDSoundCaptureBuffer->GetStatus( &dwStatus ) ) )
		return;

	//Not Capturing?
	if( (dwStatus & DSCBSTATUS_CAPTURING) == 0 )
	{
		bCapturing = FALSE;
		return;
	}

	//Stop Capture
	lpDSoundCaptureBuffer->Stop();

	bCapturing = FALSE;
}

void SoundCaptureDevice::ThreadTimer()
{
	DWORD dwLastReadPosition = 0;
	DWORD dwBufferSize = DSCBufferDesc.dwBufferBytes;

	/*
	Engine::Timer tTimer;
	Engine::CountTimer tCountTimer( CAPTURE_STOP_PRECISION_MS );

	while( !bTerminateTimerThread )
	{
		if( tCountTimer.Update( tTimer.Update() ) > 0 )
		{
			DWORD dwCurrentReadPosition;
			if( DSFAILED( lpDSoundCaptureBuffer->GetCurrentPosition( NULL, &dwCurrentReadPosition ) ) )
			{
				//Broken...
				break;
			}

			if( dwCurrentReadPosition == dwLastReadPosition )
				continue;

			DWORD dwReadBlockSize = 0;
			if( dwCurrentReadPosition < dwLastReadPosition )
			{
				dwReadBlockSize += dwBufferSize - dwLastReadPosition;
				dwReadBlockSize += dwCurrentReadPosition;
			}
			else
			{
				dwReadBlockSize += dwCurrentReadPosition - dwLastReadPosition;
			}

			//Zero Size?
			if( dwReadBlockSize == 0 )
				continue;

			void * pBuffer1 = NULL, * pBuffer2 = NULL;
			DWORD dwBuffer1Size = 0, dwBuffer2Size = 0;
			if( DSFAILED( lpDSoundCaptureBuffer->Lock( dwLastReadPosition, dwReadBlockSize, &pBuffer1, &dwBuffer1Size, &pBuffer2, &dwBuffer2Size, 0 ) ) )
			{
				//Retry
				continue;
			}

			//Invalid?
			if( pBuffer1 == NULL )
			{
				//Try to Unlock this weirdness
				lpDSoundCaptureBuffer->Unlock( pBuffer1, dwBuffer1Size, pBuffer2, dwBuffer2Size );

				//Retry
				continue;
			}

			//Compute the Temporary Buffer Size that we will use to put the Audio data in
			DWORD dwTemporaryBufferSize = dwBuffer1Size + (pBuffer2 ? dwBuffer2Size : 0);

			//Zero?
			if( dwTemporaryBufferSize == 0 )
			{
				//Try to Unlock this weirdness
				lpDSoundCaptureBuffer->Unlock( pBuffer1, dwBuffer1Size, pBuffer2, dwBuffer2Size );

				//Retry
				continue;
			}

			//Create the Temporary Buffer
			BYTE * pTemporaryBuffer = new BYTE[dwTemporaryBufferSize];

			//Copy Memory from Audio Buffer 1
			memcpy( pTemporaryBuffer, pBuffer1, dwBuffer1Size );

			//Copy Memory from Audio Buffer 2
			if( (pBuffer2) && (dwBuffer2Size > 0) )
				memcpy( pTemporaryBuffer + dwBuffer1Size, pBuffer2, dwBuffer2Size );

			//Unlock Capture Buffer
			lpDSoundCaptureBuffer->Unlock( pBuffer1, dwBuffer1Size, pBuffer2, dwBuffer2Size );

			//Notify Callback of new Audio Data
			if( fCallbackCapturedAudio )
				fCallbackCapturedAudio( pTemporaryBuffer, dwTemporaryBufferSize );

			//Delete the Buffer
			delete[] pTemporaryBuffer;

			//Set new Last Read Position
			dwLastReadPosition = dwCurrentReadPosition;
		}

		//Sleep for a short while
		Sleep( CAPTURE_STOP_PRECISION_MS >> 1 );
	}

	*/
}

}