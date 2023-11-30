#pragma once

struct ResolutionStruct
{
	UINT										uWidth;
	UINT										uHeight;
};

//#define _DXGRAPHICENGINE_USE3DX

#ifdef _DXGRAPHICENGINE_USE3DX
#define DXGRAPHICENGINE_D3DPOOL		D3DPOOL_DEFAULT
#else
#define DXGRAPHICENGINE_D3DPOOL		(GRAPHICS->IsDeviceEx() ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED)
#endif

#include "DXGeometryInstance.h"
#include "DXVideoRenderer.h"
#include "DXRenderer.h"
#include "EXEVertexCache.h"

#define DXVIDEORENDERER				(GRAPHICENGINE->GetVideoRenderer())
#define VERTEXCACHE					(GRAPHICENGINE->GetVertexCache())

class DXGraphicEngine
{
protected:
	// Post Processing
	class DXPostProcess				* lpPostProcess;
	class DXGlow					* lpGlow = NULL;


	// Light
	class CLightEffect				* lpLightEffect;

	//Test Geometry
	DXGeometryInstance				* pcGeometryEngine;

	EXEVertexCache					* pcVertexCache;

public:

	int								iLastPrimitiveCount = 0;
	int								iLastPrimitiveType = 0;
	DWORD							dwLastVertexType = 0;
	PVOID							pLastRenderPtr = NULL;

	ResolutionStruct				sResolution;
									DXGraphicEngine();
	virtual							~DXGraphicEngine();

	void							Log( char * pszFormat, ... );
	void							Error( char * pszFormat, ... );

	// Initialization
	BOOL							InitD3D( HWND hWindow );

	BOOL							CreateVertexBuffer();

	// Getters 
	static LPDIRECT3DDEVICE9		GetDevice(){return *( LPDIRECT3DDEVICE9* )0x3A9A318;};
	static int						GetWidthBackBuffer(){return *( int* )WidthWindow;};
	static int						GetHeightBackBuffer(){return *( int* )HeightWindow;};
	static HWND						GetWindowHandler() { return *(HWND*)HandleWindow; };
	LPDIRECT3DVERTEXBUFFER9			GetVertexBuffer(int i) { return lpVertexBuffer[i]; }
	LPDIRECT3DINDEXBUFFER9			GetIndexBuffer(int i) { return lpIndexBuffer[i]; }

	DXVideoRenderer					* GetVideoRenderer() { return pcVideoRenderer; };
	EXEVertexCache					* GetVertexCache() { return pcVertexCache; }
	DXPostProcess				    * GetPostProcess(){ return lpPostProcess; }
	DX::Texture_ptr					GetBoneTexture() { return pTextureBones; }
	DX::Renderer_ptr				GetSRenderer() { return pRenderer; }

	Graphics::Material			    * GetAgingMaterial( int iAgeLevel ) const { if( iAgeLevel <= 3 || iAgeLevel > MAX_AGING ) return nullptr; return pAgingMaterial[iAgeLevel - 4]; }
	Graphics::Material			    * GetMixingMaterial( int iMixID ) const { if( (iMixID < 0) || (iMixID >= _countof( pMixingMaterial )) ) return nullptr; return pMixingMaterial[iMixID]; }
	Graphics::Material				* GetDropEffectMaterial() { return pDropEffectMaterial; }
	Graphics::Material				* GetRainbowMaterial() { return pRainbowMaterial; }
	Delta3D::Graphics::Camera	    * GetCamera() { return pCamera; }
	Delta3D::Graphics::Camera	    * GetFrameCamera() { return pFrameCamera; }

	class DXGlow					* GetGlow() { return lpGlow; };

	void							DrawRectangle2D( class Rectangle2D rRect, DWORD dwColor );
	void							DrawRectangle2D( class Rectangle2D sBox, DWORD dwColor1, DWORD dwColor2 );

	// Scene
	static void						BeginScene( D3DXMATRIX mView );
	static void						EndScene();
	void							StateSetup();
	BOOL							Reset( BOOL bResetWindowed, UINT uResetScreenWidth, UINT uResetScreenHeight, BOOL bResetWindow );
	BOOL							SaveRenderTargetToFile( const char * pszFilePath );

	UINT							GetVideoMemory();
	std::string						GetVideoName();
	UINT							GetVideoGUID();

	UINT							GetVertexSize( DWORD dwVertexTypeDesc );

	BOOL							DrawPrimitive( D3DPRIMITIVETYPE d3dPrimitiveType, DWORD dwVertexTypeDesc, int iPrimitiveCount, LPVOID pVertices );

	BOOL							DrawIndexedPrimitive( D3DPRIMITIVETYPE d3dPrimitiveType, DWORD dwVertexTypeDesc, int iPrimitiveCount, LPVOID pVertices );
	BOOL							DrawIndexedPrimitiveGeo( D3DPRIMITIVETYPE d3dPrimitiveType, DWORD dwVertexTypeDesc, int iPrimitiveCount, LPVOID pVertices );

	BOOL							DrawLast();

	DWORD							GetTextureAverageColor( LPDIRECT3DTEXTURE9 lpTexture, UINT uWidth, UINT uHeight, BOOL bHighColor );

	void							OnResetDevice();
	void							OnLostDevice();
	void							ReloadPostProcess();						

	BOOL							CheckDevice();

	// 3D
	void							Begin3D();
	void							End3D();

	std::shared_ptr<Graphics::Shader> GetTerrainShader() const{ return pTerrainShader; }

	// Bitmaps and loading
	static DWORD WINAPI				ThreadLoadingBar();
	static void WINAPI				UpdateLoading( int iCurrent, int iMax );
	static void WINAPI				ShowLoading();
	static void WINAPI				HideLoading();
	static DWORD WINAPI				DrawLoadingImage();

	static DWORD __cdecl			TakenScreenShot(DWORD dw1, DWORD dwType);
	static DWORD WINAPI				GetScreenShot();

	static BOOL						ResolutionChange( BOOL bFirstTime, int iWidth, int iHeight );
	static BOOL						ResizeWindow();
private:
	LPDIRECT3DVERTEXBUFFER9			lpVertexBuffer[5];
	LPDIRECT3DINDEXBUFFER9			lpIndexBuffer[5];

	Delta3D::Graphics::Camera	  * pCamera = nullptr;
	Delta3D::Graphics::Camera	  * pFrameCamera = nullptr;
	Graphics::Material			  * pAgingMaterial[MAX_AGING-3];
	Graphics::Material			  * pMixingMaterial[20];
	Graphics::Material			  * pDropEffectMaterial;
	Graphics::Material			  * pRainbowMaterial;

	DX::Renderer_ptr				pRenderer = NULL;
	DX::GraphicsDevice_ptr			pGraphicsDevice = NULL;

	std::shared_ptr<Graphics::Shader> pTerrainShader = nullptr;

	DXVideoRenderer					* pcVideoRenderer = NULL;

	ID3DXLine					  * pRectangleUI;
	BOOL							bCreatedRectangle = FALSE;

	BOOL							bWindowed = FALSE;

	HRESULT							hr;

	// Loading Bar
	int								iCurrentLoading;
	int								iMaxLoading;
	HANDLE							hLoadingThread;
	BOOL							bQuitLoading;
	BOOL							bShowLoading;

	DX::Texture_ptr					pTextureBones = nullptr;

	enum DXSettings
	{
		DistanceCamera				= 0x82BF0C,					// Distance of camera general
		WidthCameraView				= 0x2E45828,				// iWidth of view Direct3D
		HeightCameraView			= 0x324585C,				// iHeight of view Direct3D
		HandleWindow				= 0x0A17540,				// Handle Window
		BitDepth					= 0x824F78,					// Bit depth 
		WindowedConfig				= 0x0CDC4C8,				// Windowed Config
		WindowedRender				= 0x9CB028,					// Windowed Render

		// iWidth and iHeight
		WidthWindow					= 0x824F70,					// iWidth Window
		HeightWindow				= 0x824F74,					// iHeight Window
		WidthWindowConfig			= 0x0CDC4CC,				// iWidth config
		HeightWindowConfig			= 0x0CDC4D0,				// iHeight config
		WidthBBDevice				= 0x3A9A30C,				// iWidth BackBuffer device
		HeightBBDevice				= 0x3A9A310,				// iHeight BackBuffer device
		WidthDevice					= 0x3A9A2D0,				// iWidth device
		HeightDevice				= 0x3A9A2D4,				// iHeight device
		WidthRenderDimension		= 0x82BF04,					// iWidth of render
		HeightRenderDimension		= 0x82BF08,					// iHeight of render
	};
};

DX::GraphicsDevice_ptr GetGraphicsDevice();