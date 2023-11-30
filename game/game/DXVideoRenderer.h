#pragma once

#define MAX_AUDIO_STREAMS		5
#define MAX_VIDEO_STREAMS		5

class DXVideoRenderer
{
public:
	DXVideoRenderer();
	virtual ~DXVideoRenderer();

	BOOL					Init( std::string _strFile );
	void					Shutdown();

	void					Run();

	void					Update();

	void					Render();

	void					Loop( BOOL b );

	double					GetPosition();
	void					SetPosition( double d );

	void					SetRepeatBegin( double d );

	double					GetDuration() { return dDuration; }

	BOOL					CanSeek();

	UINT					GetFPS() { return uFPS; }
	void					SetFPS( UINT u ) { uFPS = u; }

	void					OnResetDevice();
	void					OnLostDevice();

protected:
	std::string				strFile;

	IGraphBuilder			* pcGraphBuilder	= NULL;
	IMediaControl			* pcMediaControl	= NULL;
	IVideoWindow			* pcVideoWindow		= NULL;
	IMediaEventEx			* pcMediaEvents		= NULL;
	IMediaPosition			* pcMediaPosition	= NULL;
	IMediaSeeking			* pcMediaSeeking	= NULL;
	IBaseFilter				* pcBaseFilter		= NULL;
	ISampleGrabber			* pcSampleGrabber	= NULL;
	IBaseFilter				* pcNullRender		= NULL;
	IBaseFilter				* pcSourceFilter	= NULL;

	LPDIRECT3DTEXTURE9		lpTexture			= NULL;

	double					dDuration			= 0.0;

	double					dRepeat				= 0.0;

	double					dOldPosition		= 0.0;

	BOOL					bFirstUpdate		= TRUE;

	UINT					uWidth				= 0;
	UINT					uHeight				= 0;

	UINT					uFPS				= 60;

	BOOL					bLoop				= FALSE;

	BOOL					bInit				= FALSE;

	char					* pBufferData		= NULL;
	long					lBufferSize			= 0;

private:
};

