#include "stdafx.h"
#include "DXVideoRenderer.h"


DXVideoRenderer::DXVideoRenderer()
{

}


DXVideoRenderer::~DXVideoRenderer()
{
	
}

// GetUnconnectedPin   
//    Finds an unconnected pin on a filter in the desired direction   
HRESULT GetUnconnectedPin(
	IBaseFilter *pFilter,   // Pointer to the filter.   
	PIN_DIRECTION PinDir,   // Direction of the pin to find.   
	IPin **ppPin )           // Receives a pointer to the pin.   
{
	*ppPin = 0;
	IEnumPins *pEnum = 0;
	IPin *pPin = 0;
	HRESULT hr = pFilter->EnumPins( &pEnum );
	if ( FAILED( hr ) )
	{
		return hr;
	}
	while ( pEnum->Next( 1, &pPin, NULL ) == S_OK )
	{
		PIN_DIRECTION ThisPinDir;
		pPin->QueryDirection( &ThisPinDir );
		if ( ThisPinDir == PinDir )
		{
			IPin *pTmp = 0;
			hr = pPin->ConnectedTo( &pTmp );
			if ( SUCCEEDED( hr ) )  // Already connected, not the pin we want.   
			{
				pTmp->Release();
			}
			else  // Unconnected, this is the pin we want.   
			{
				pEnum->Release();
				*ppPin = pPin;
				return S_OK;
			}
		}
		pPin->Release();
	}
	pEnum->Release();
	// Did not find a matching pin.   
	return E_FAIL;
}

// Disconnect any connections to the filter.   
HRESULT DisconnectPins( IBaseFilter *pFilter )
{
	IEnumPins *pEnum = 0;
	IPin *pPin = 0;
	HRESULT hr = pFilter->EnumPins( &pEnum );
	if ( FAILED( hr ) )
	{
		return hr;
	}

	while ( pEnum->Next( 1, &pPin, NULL ) == S_OK )
	{
		pPin->Disconnect();
		pPin->Release();
	}
	pEnum->Release();

	// Did not find a matching pin.   
	return S_OK;
}

// ConnectFilters   
//    Connects a pin of an upstream filter to the pDest downstream filter   
HRESULT ConnectFilters(
	IGraphBuilder *pGraph, // Filter Graph Manager.   
	IPin *pOut,            // Output pin on the upstream filter.   
	IBaseFilter *pDest )    // Downstream filter.   
{
	if ( (pGraph == NULL) || (pOut == NULL) || (pDest == NULL) )
	{
		return E_POINTER;
	}
#ifdef debug   
	PIN_DIRECTION PinDir;
	pOut->QueryDirection( &PinDir );
	_ASSERTE( PinDir == PINDIR_OUTPUT );
#endif   

	// Find an input pin on the downstream filter.   
	IPin *pIn = 0;
	HRESULT hr = GetUnconnectedPin( pDest, PINDIR_INPUT, &pIn );
	if ( FAILED( hr ) )
	{
		return hr;
	}
	// Try to connect them.   
	hr = pGraph->Connect( pOut, pIn );
	pIn->Release();
	return hr;
}



// ConnectFilters   
//    Connects two filters   
HRESULT ConnectFilters(
	IGraphBuilder *pGraph,
	IBaseFilter *pSrc,
	IBaseFilter *pDest )
{
	if ( (pGraph == NULL) || (pSrc == NULL) || (pDest == NULL) )
	{
		return E_POINTER;
	}

	// Find an output pin on the first filter.   
	IPin *pOut = 0;
	HRESULT hr = GetUnconnectedPin( pSrc, PINDIR_OUTPUT, &pOut );
	if ( FAILED( hr ) )
	{
		return hr;
	}
	hr = ConnectFilters( pGraph, pOut, pDest );
	pOut->Release();
	return hr;
}

STDMETHODIMP BufferCB( double SampleTime, BYTE *pBuffer, long BufferLen )
{
	WRITEERR( "%lf", SampleTime );

	return 0;
}


BOOL DXVideoRenderer::Init( std::string _strFile )
{
	if ( bInit )
		return FALSE;

	CoInitialize( NULL );

	pcGraphBuilder	= NULL;
	pcMediaControl	= NULL;
	pcVideoWindow	= NULL;
	pcMediaEvents	= NULL;
	pcMediaPosition	= NULL;
	pcMediaSeeking	= NULL;
	pcBaseFilter	= NULL;
	pcSampleGrabber = NULL;
	pcNullRender	= NULL;
	pcSourceFilter	= NULL;

	uWidth			= 0;
	uHeight			= 0;

	uFPS			= 60;

	strFile = _strFile;

	HRESULT hr;

	
	//Create object base for graphic media
	hr = CoCreateInstance( CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (LPVOID *)&pcGraphBuilder );
	if ( (hr != S_OK) || (pcGraphBuilder == NULL) )
	{
		WRITEDBG( "GraphBuilder Video Render [0x%08X]", hr );
		return FALSE;
	}

	//Media control, volume, speed, etc...
	hr = pcGraphBuilder->QueryInterface( IID_IMediaControl, (LPVOID *)&pcMediaControl );
	if ( (hr != S_OK) || (pcMediaControl == NULL) )
	{
		WRITEDBG( "MediaControl Video Render [0x%08X]", hr );
		return FALSE;
	}
	
	//Window, only used for hide window video...
	hr = pcGraphBuilder->QueryInterface( IID_IVideoWindow, (LPVOID *)&pcVideoWindow );
	if ( (hr != S_OK) || (pcVideoWindow == NULL) )
	{
		WRITEDBG( "VideoWindow Video Render [0x%08X]", hr );
		return FALSE;
	}
	
	//Open video file to the object memory
	WCHAR wPath[MAX_PATH];
	MultiByteToWideChar( CP_ACP, 0, strFile.c_str(), -1, wPath, MAX_PATH );
	hr = pcGraphBuilder->RenderFile( wPath, NULL );
	if ( hr != S_OK )
	{
		WRITEDBG( "RenderFile Video Render [0x%08X][%s]", hr, strFile.c_str() );
		return FALSE;
	}

	//Data seek...
	hr = pcGraphBuilder->QueryInterface( IID_IMediaSeeking, (LPVOID *)&pcMediaSeeking );
	if ( (hr != S_OK) || (pcMediaSeeking == NULL) )
	{
		WRITEDBG( "MediaSeeking Video Render [0x%08X]", hr );
		return FALSE;
	}

	//Create Grabber object, will be used to read the video file
	hr = CoCreateInstance( CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (LPVOID *)&pcBaseFilter );
	if ( (hr != S_OK) || (pcBaseFilter == NULL) )
	{
		WRITEDBG( "BaseFilter Video Render [0x%08X]", hr );
		return FALSE;
	}

	//Add Grabber to graphic
	hr = pcGraphBuilder->AddFilter( pcBaseFilter, L"Sample Grabber" );
	if ( hr != S_OK )
	{
		WRITEDBG( "AddFilter Video Render [0x%08X]", hr );
		return FALSE;
	}

	//Grabber for buffers
	hr = pcBaseFilter->QueryInterface( IID_ISampleGrabber, (LPVOID *)&pcSampleGrabber );
	if ( (hr != S_OK) || (pcBaseFilter == NULL) )
	{
		WRITEDBG( "SampleGrabber Video Render [0x%08X]", hr );
		return FALSE;
	}

	//Set Video Format that we want use on
	AM_MEDIA_TYPE mt;
	ZeroMemory( &mt, sizeof( AM_MEDIA_TYPE ) );
	mt.majortype	= MEDIATYPE_Video;
	mt.subtype		= MEDIASUBTYPE_RGB32;
	mt.formattype	= FORMAT_VideoInfo;
	hr = pcSampleGrabber->SetMediaType( &mt );
	if ( hr != S_OK )
	{
		WRITEDBG( "SetMediaType Video Render [0x%08X]", hr );
		return FALSE;
	}

	//Create Filters
	hr = pcGraphBuilder->AddSourceFilter( wPath, L"Source", &pcSourceFilter );
	if ( (hr != S_OK) || (pcSourceFilter == NULL) )
	{
		WRITEDBG( "AddSourceFilter Video Render [0x%08X]", hr );
		return FALSE;
	}

	//Link Filters to the graphic
	hr = ConnectFilters( pcGraphBuilder, pcSourceFilter, pcBaseFilter );
	if ( FAILED( hr ) )
	{
		WRITEDBG( "ConnectFilters Video Render [0x%08X]", hr );
		return FALSE;
	}

	//Null Render
	hr = CoCreateInstance( CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&pcNullRender );
	if ( (hr != S_OK) || (pcNullRender == NULL) )
	{
		WRITEDBG( "CoCreateInstance NullRender Video Render [0x%08X]", hr );
		return FALSE;
	}

	//Add Render to Graphic
	hr = pcGraphBuilder->AddFilter( pcNullRender, L"Render" );
	if ( (hr != S_OK) )
	{
		WRITEDBG( "AddFilter Render Video Render [0x%08X]", hr );
		return FALSE;
	}

	//Link render object to others objects
	hr = ConnectFilters( pcGraphBuilder, pcBaseFilter, pcNullRender );
	if ( (hr != S_OK) )
	{
		WRITEDBG( "ConnectFilters Link Video Render [0x%08X]", hr );
		return FALSE;
	}

	//Not used yet... need implement callback...
	hr = pcGraphBuilder->QueryInterface( IID_IMediaEventEx, (LPVOID *)&pcMediaEvents );
	if ( (hr != S_OK) || (pcMediaEvents == NULL) )
	{
		WRITEDBG( "MediaEvent Video Render [0x%08X]", hr );
		return FALSE;
	}


	//Get Video Resolution on header data
	AM_MEDIA_TYPE mtt;
	hr = pcSampleGrabber->GetConnectedMediaType( &mtt );
	if ( (hr != S_OK) )
	{
		WRITEDBG( "GetConnectedMediaType Video Render [0x%08X]", hr );
		return FALSE;
	}

	VIDEOINFOHEADER *vih = (VIDEOINFOHEADER*)mtt.pbFormat;
	uWidth	= vih->bmiHeader.biWidth;
	uHeight = vih->bmiHeader.biHeight;

	//Create texture to receive the video bitmap
	if ( FAILED( hr = DX::Graphic::GetDevice()->CreateTexture( uWidth, uHeight, 1, 0, GRAPHICS->presentParams.BackBufferFormat, DXGRAPHICENGINE_D3DPOOL, &lpTexture, NULL ) ) )
	{
		WRITEDBG( "CreateTexture Video Render [0x%08X]", hr );
		return FALSE;
	}

	dDuration = 0;

	//Create Position Handler
	hr = pcGraphBuilder->QueryInterface( IID_IMediaPosition, (LPVOID *)&pcMediaPosition );
	if ( (hr != S_OK) || (pcMediaPosition == NULL) )
	{
		WRITEDBG( "MediaPosition Video Render [0x%08X]", hr );
		return FALSE;
	}

	//Set continuous rendering and set buffer usage
	pcSampleGrabber->SetOneShot( FALSE );
	pcSampleGrabber->SetBufferSamples( TRUE );

	//Hide Window
	pcVideoWindow->put_AutoShow( OAFALSE );

	pcMediaPosition->get_Duration( &dDuration );

	bInit = TRUE;

	bFirstUpdate = TRUE;

	return TRUE;
}

void DXVideoRenderer::Shutdown()
{
	if ( pcMediaControl )
		pcMediaControl->Stop();
	
	RELEASE( pcGraphBuilder );
	RELEASE( pcMediaControl );
	RELEASE( pcVideoWindow );
	RELEASE( pcMediaEvents );
	RELEASE( pcMediaPosition );
	RELEASE( pcMediaSeeking );
	RELEASE( pcBaseFilter );
	RELEASE( pcSampleGrabber );
	RELEASE( pcNullRender );
	RELEASE( pcSourceFilter );

	CoUninitialize();

	RELEASE( lpTexture );

	DELETA( pBufferData );
	lBufferSize = 0;

	bInit = FALSE;
}

void DXVideoRenderer::Run()
{
	HRESULT hr;

	if ( pcMediaControl )
		hr = pcMediaControl->Run();

	OAFilterState oa;

	//Stay while not running state...
	pcMediaControl->GetState( 1000, &oa );

	bInit = TRUE;
}

void DXVideoRenderer::Update()
{
	if ( bInit == FALSE )
		return;

	if ( !lpTexture )
		return;

	HRESULT hr;

	if ( !pcGraphBuilder )
		return;

	if ( pBufferData == NULL )
	{
		//Get Buffer Size
		hr = pcSampleGrabber->GetCurrentBuffer( &lBufferSize, NULL );
		if ( lBufferSize <= 0 )
		{
			return;
		}

		//New buffer data
		pBufferData = new char[lBufferSize];
		if ( pBufferData == NULL )
		{
			WRITEDBG( "Could not allocate video Memory[%d]", lBufferSize );
			return;
		}
	}

	//Update?
	if ( (GetPosition() - dOldPosition) < (1.0 / (double)GetFPS()) )
	{
		if ( bFirstUpdate == FALSE )
		{
			if ( (dDuration - GetPosition()) > (1.0 / (double)GetFPS()) )
				return;
		}

		//If First Update time, then copy buffer to texture ( black screen prevention )
		bFirstUpdate = FALSE;
	}

	//Get Current Bitmap buffer
	hr = pcSampleGrabber->GetCurrentBuffer( &lBufferSize, (long*)pBufferData );
	if ( hr == E_INVALIDARG || hr == VFW_E_NOT_CONNECTED || hr == VFW_E_WRONG_STATE )
	{
		WRITEDBG( "[DSHOW] Failed at GetCurrentBuffer!" );
		return;
	}

	BYTE * pVideo = (BYTE*)pBufferData;

	//Rectangle all window
	D3DLOCKED_RECT d3dRect;
	RECT rc;
	rc.left		= 0;
	rc.top		= 0;
	rc.right	= uWidth;
	rc.bottom	= uHeight;
	hr = lpTexture->LockRect( 0, &d3dRect, &rc, D3DLOCK_DISCARD );

	BYTE * pDest = (BYTE*)d3dRect.pBits;

	if ( pDest == NULL )
	{
		lpTexture->UnlockRect( 0 );
		return;
	}

	//Inverse Loop, flipped bitmap...
	for ( int i = uHeight-1; i >= 0; i-- )
	{
		//Copy into texture buffer, based on RBG32 (4Bytes)
		CopyMemory( pDest, pVideo + (uWidth * 4 * i), uWidth * 4 );
		pDest += d3dRect.Pitch;
	}

	lpTexture->UnlockRect( 0 );

	dOldPosition = GetPosition();

	//Loop render?
	if ( bLoop )
	{
		//Max duration? back to initial position
		if ( GetPosition() >= dDuration )
		{
			double d = 0.0;
			if ( dRepeat > 0.0 )
				d = dRepeat;

			SetPosition( d );
		}

		//Run Time
		Run();
	}
}

void DXVideoRenderer::Render()
{
	if ( bInit == FALSE )
		return;

	UI::ImageRender::Render( lpTexture, 0, 0, uWidth, uHeight, 0, 0, -1, TRUE );
}

void DXVideoRenderer::Loop( BOOL b )
{
	bLoop = b;
}

double DXVideoRenderer::GetPosition()
{
	REFTIME dTime = 0.0;

	if ( pcMediaPosition )
	{
		pcMediaPosition->get_CurrentPosition( &dTime );
		return dTime;
	}

	return 0.0;
}

void DXVideoRenderer::SetPosition( double d )
{
	if ( pcMediaPosition )
		pcMediaPosition->put_CurrentPosition( d );

	dOldPosition = d;
}

void DXVideoRenderer::SetRepeatBegin( double d )
{
	dRepeat = d;
}

BOOL DXVideoRenderer::CanSeek()
{
	if ( pcMediaPosition )
	{
		LONG lCanSeek = 0;
		pcMediaPosition->CanSeekForward( &lCanSeek );

		if ( lCanSeek == OATRUE )
			return TRUE;
	}

	return FALSE;
}

void DXVideoRenderer::OnResetDevice()
{

}

void DXVideoRenderer::OnLostDevice()
{

}

