#pragma once

namespace DX
{

class SoundDevice;

const int iRecordingBufferSizeInSeconds = 4;

extern WAVEFORMATEX RecordingWaveFormatEx;

const int CAPTURE_STOP_PRECISION_MS = 250;	//Timer Precision in Timer Thread
	
typedef std::shared_ptr<class SoundCaptureDevice> SoundCaptureDevice_ptr;

class SoundCaptureDevice : public std::enable_shared_from_this<SoundCaptureDevice>
{
public:
										  SoundCaptureDevice();
	virtual								 ~SoundCaptureDevice();

	BOOL								  Init( CWindow * _pWindow );
	void								  Shutdown();

	BOOL								  Start();
	BOOL								  IsCapturing();
	void								  Stop();

	BOOL								  HasDevice() const;

	CWindow *							  GetWindow() { return pWindow; }

	LPDIRECTSOUNDCAPTURE8				  Get() { return lpDSoundCapture; }
	const DSCCAPS						& GetCaps() { return DSCCaps; }

	std::function<void(BYTE*,DWORD)>	  GetCallback() { return fCallbackCapturedAudio; }
	void								  SetCallback( std::function<void(BYTE*,DWORD)> f ) { fCallbackCapturedAudio = f; }

private:
	void								  ThreadTimer();

private:
	CWindow *							  pWindow;

	LPDIRECTSOUNDCAPTURE8				  lpDSoundCapture = NULL;
	DSCCAPS								  DSCCaps;

	DSCBUFFERDESC						  DSCBufferDesc;
	LPDIRECTSOUNDCAPTUREBUFFER8			  lpDSoundCaptureBuffer = NULL;
	
	std::thread							  tTimerThread;
	BOOL								  bTerminateTimerThread = FALSE;

	BOOL								  bCapturing = FALSE;

	std::function<void(BYTE*,DWORD)>	  fCallbackCapturedAudio;
};

}