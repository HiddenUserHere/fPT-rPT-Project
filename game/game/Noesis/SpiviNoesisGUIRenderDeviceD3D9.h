/*
-----------------------------------------------------------------------------
This source file is part of spivi runtime.
For the latest info, see http://www.spiviTech.com

Copyright (c) SpiviTech Ltd
-----------------------------------------------------------------------------
*/

#ifdef _NOESIS_GUI
#ifndef __Spivi_NoesisGUIRenderDeviceD3D9_H__
#define __Spivi_NoesisGUIRenderDeviceD3D9_H__

#include <Noesis_pch.h>

#include <d3d9.h>
#include <d3dx9.h>

namespace Spivi
{

	/** 
	NoesisGUI D3D9 render device implementation class.
	*/
	class NoesisGUIRenderDeviceD3D9 final: public Noesis::RenderDevice
	{

		// Interface.
	public:
		NoesisGUIRenderDeviceD3D9(IDirect3DDevice9* pDeviceD3D9);
		virtual ~NoesisGUIRenderDeviceD3D9();

		/** Return true if the device can render. */
		bool canRender() const;

		/** D3D9 Device lost handle method. */
		void handleDeviceLost();

		/** D3D9 Device reset handle method. */
		void handleDeviceReset();
	

		// Protected types.
	protected:



		// Protected methods.
	protected:

		
		

		// Interface implementation.
	protected:

		/** @see Noesis::RenderDevice::GetCaps */
		virtual const Noesis::DeviceCaps& GetCaps() const override;

		/** @see Noesis::RenderDevice::CreateRenderTarget */
		virtual Noesis::Ptr<Noesis::RenderTarget> CreateRenderTarget(const char* label, UINT width, UINT height, UINT sampleCount) override;

		/** @see Noesis::RenderDevice::CloneRenderTarget */
		virtual Noesis::Ptr<Noesis::RenderTarget> CloneRenderTarget(const char* label, Noesis::RenderTarget* surface) override;

		/** @see Noesis::RenderDevice::CreateTexture */
		virtual Noesis::Ptr<Noesis::Texture> CreateTexture(const char* label, UINT width, UINT height, UINT numLevels, Noesis::TextureFormat::Enum format) override;

		/** @see Noesis::RenderDevice::UpdateTexture */
		virtual void UpdateTexture(Noesis::Texture* texture, UINT level, UINT x, UINT y, UINT width, UINT height, const void* data) override;

		/** @see Noesis::RenderDevice::BeginRender */
		virtual void BeginRender(bool offscreen) override;

		/** @see Noesis::RenderDevice::SetRenderTarget */
		virtual void SetRenderTarget(Noesis::RenderTarget* surface) override;

		/** @see Noesis::RenderDevice::BeginTile */
		virtual void BeginTile(const Noesis::Tile& tile, UINT surfaceWidth, UINT surfaceHeight) override;

		/** @see Noesis::RenderDevice::EndTile */
		virtual void EndTile() override;

		/** @see Noesis::RenderDevice::ResolveRenderTarget */
		virtual void ResolveRenderTarget(Noesis::RenderTarget* surface, const Noesis::Tile* tiles, UINT numTiles) override;

		/** @see Noesis::RenderDevice::EndRender */
		virtual void EndRender() override;

		/** @see Noesis::RenderDevice::MapVertices */
		virtual void* MapVertices(UINT bytes) override;

		/** @see Noesis::RenderDevice::UnmapVertices */
		virtual void UnmapVertices() override;

		/** @see Noesis::RenderDevice::MapIndices */
		virtual void* MapIndices(UINT bytes) override;

		/** @see Noesis::RenderDevice::UnmapIndices */
		virtual void UnmapIndices() override;

		/** @see Noesis::RenderDevice::DrawBatch */
		virtual void DrawBatch(const Noesis::Batch& batch) override;

		// Protected types.
	protected:

		/** Base rendering buffer data structure. */
		struct BaseRenderBuffer
		{
			UINT size;
			UINT pos;
			UINT drawPos;
		};

		/** Vertex buffer data structure. */
		struct VertexBuffer : public BaseRenderBuffer
		{
			IDirect3DVertexBuffer9* pVertexBuffer;
		};

		/** Index buffer data structure. */
		struct IndexBuffer : public BaseRenderBuffer
		{
			IDirect3DIndexBuffer9* pIndexBuffer;
		};

		/** Base shader data structure. */
		struct BaseShader
		{
			BaseShader();
			~BaseShader();

			LPD3DXBUFFER lpMicroCode;
			LPD3DXCONSTANTTABLE lpConstantTable;
		};

		/** Vertex shader data structure. */
		struct VertexShader : public BaseShader
		{
			VertexShader();
			~VertexShader();

			IDirect3DVertexShader9* pVertexShader;
			D3DXHANDLE hProjMatrix;

		};

		/** Pixel shader data structure. */
		struct PixelShader : public BaseShader
		{
			PixelShader();
			~PixelShader();

			IDirect3DPixelShader9* pPixelShader;
			D3DXHANDLE hColor;
			D3DXHANDLE hOpacity;
			D3DXHANDLE hRadialGrad;
		};

		/** Vertex declaration data structure. */
		struct VertexDeclaration
		{
			VertexDeclaration();
			~VertexDeclaration();

			IDirect3DVertexDeclaration9* pVertexDeclaration;
			UINT nStride;
		};

		typedef std::vector<Noesis::RenderTarget*> RenderTargetArray;

	protected:

		/** Check the given operation result and report the error if occurred. */
		void checkOperationResult(HRESULT hr, const char* lpszMethod, const char* lpszOperationName);

		/** Configure render device caps. */
		void configureRenderDeviceCaps();

		/** Create/Release dynamic vertex buffer. */
		void createDynamicVertexBuffer();
		void releaseDynamicVertexBuffer();

		/** Create/Release dynamic index buffer. */
		void createDynamicIndexBuffer();
		void releaseDynamicIndexBuffer();

		/** Create/Release vertex and pixel shaders. */
		void createShaders();
		void releaseShaders();

		/** Compile a given shader using the embedded source and given entry point and profile. */
		bool compileShader(const char* lpszEntryPoint, const char* lpszProfile, BaseShader* pShader);
		void createVertexShader(VertexShader* pShader);
		void createPixelShader(PixelShader* pShader);

		/** Set the relevant shaders to the given batch.  */
		void setShaders(const Noesis::Batch& batch);

		/** Set the given vertex shader. */
		void setVertexShader(const Noesis::Batch& batch, VertexShader* pShader);

		/** Set the given pixel shader. */
		void setPixelShader(const Noesis::Batch& batch, PixelShader* pShader);

		/** Create/Release vertex declarations. */
		void createVertexDeclarations();
		void createVertexDeclaration(const UINT nFlags, VertexDeclaration* pVertexDecl);
		void releaseVertexDeclarations();

		/** Set the vertex stage processing resources for the given batch. */
		void setVertexStageResources(const Noesis::Batch& batch);

		/** Set the given vertex declaration. */
		void setVertexDeclaration(VertexDeclaration* pVertexDecl);

		/** Set the render state for the given batch. */
		void setRenderState(const Noesis::RenderState& rkRenderState, uint8_t uStencilRef);
		void setFillModeRenderState(const Noesis::RenderState& rkRenderState);
		void setScissorRenderState(const Noesis::RenderState& rkRenderState);
		void setStencilModeRenderState(const Noesis::RenderState& rkRenderState, uint8_t uStencilRef);
		void setBlendModeRenderState(const Noesis::RenderState& rkRenderState);
		void setColorWriteRenderState(const Noesis::RenderState& rkRenderState);

		/** Set the textures for the given batch. */
		void setTextures(const Noesis::Batch & batch);
		void setTexture(UINT nStage, Noesis::Texture* pTexture);
		void setSampleState(UINT nStage, const Noesis::SamplerState& rkSampleState);

		/** Render state capture/restore methods. */
		void captureRenderState();
		void restoreRenderState();

		/** State block creation/release methods. */
		void createStateBlock();
		void releaseStateBlock();

		/** Render surfaces capture/restore/release methods. */
		void captureRenderSurfaces();
		void restoreRenderSurfaces();
		void releaseRenderSurfaces();

		/** Invalidates the render state. */
		void invalidateRenderStateCache();

		/** Create render target internal resources. */
		void createRenderTargetResources(UINT width, UINT height, UINT sampleCount, IDirect3DTexture9** ppTextureImpl, IDirect3DSurface9** ppDepthStencilSurfaceImpl);
		Noesis::Ptr<Noesis::RenderTarget> createRenderTarget(const char* label, UINT width, UINT height, UINT sampleCount, IDirect3DTexture9* pTextureImpl, IDirect3DSurface9* pDepthStencilSurfaceImpl);

		
	private:
		IDirect3DDevice9*				m_pDevice;				// The source rendering system device.
		D3DCAPS9						m_oDeviceCaps;			// The source rendering system device caps.
		IDirect3DStateBlock9*			m_pStateBlock;			// The state block used to save and restore original scene state.
		IDirect3DSurface9*				m_pColorBuffer;			// The main target color buffer.
		IDirect3DSurface9*				m_pDepthBuffer;			// The main target depth buffer.	
		VertexBuffer					m_oVertexBuffer;		// The main vertex buffer resource.
		IndexBuffer						m_oIndexBuffer;			// The main index buffer resource.
		Noesis::DeviceCaps				m_oCaps;				// The render device caps.
		VertexShader					m_aVertexShaders[7];	// The vertex shaders.
		PixelShader						m_aPixelShaders[18];	// The pixel shaders.
		VertexDeclaration				m_aVertexDecls[7];		// The vertex declarations.
		VertexShader*					m_pCurVertexShader;		// The current used vertex shader.
		PixelShader*					m_pCurPixelShader;		// The current used pixel shader.
		VertexDeclaration*				m_pCurVertexDecl;		// The current used vertex declaration.
		UINT						m_nVertexCBHash;		// The current vertex shader constant buffer hash.
		UINT						m_nPixelCBHash;			// The current vertex shader constant buffer hash.
		Noesis::RenderState				m_oCurRenderState;		// The current render state.
		uint8_t							m_uCurStencilRef;		// The current stencil ref value.
		RenderTargetArray				m_aRenderTargets;		// The render targets created by this device.

	private:
		friend class NoesisGUIRendererD3D9;

	};

}

#endif
#endif