/*
-----------------------------------------------------------------------------
This source file is part of spivi runtime.
For the latest info, see http://www.spiviTech.com

Copyright (c) SpiviTech Ltd
-----------------------------------------------------------------------------
*/

#ifdef _NOESIS_GUI

#ifndef __Spivi_NoesisGUIRenderTargetD3D9_H__
#define __Spivi_NoesisGUIRenderTargetD3D9_H__

#include <Noesis_pch.h>
#include "SpiviNoesisGUITextureD3D9.h"

#include <d3d9.h>

namespace Spivi
{
	//-----------------------------------------------------------------------------
	class NoesisGUIRenderTargetD3D9 final : public Noesis::RenderTarget
	{
	public:
		NoesisGUIRenderTargetD3D9();
		virtual ~NoesisGUIRenderTargetD3D9();
		NoesisGUIRenderTargetD3D9(const char* lpszLabel, uint32_t width, uint32_t height, uint32_t sampleCount, Noesis::Ptr<NoesisGUITextureD3D9> pTexture, IDirect3DSurface9* pDepthStencilSurface);

		/** Return the label of this render target. */
		const std::string & GetLabel() const { return m_strLabel; }

		/** @see Texture::GetWidth */
		uint32_t GetWidth() const { return m_nWidth;  }

		/** @see Texture::GetHeight */
		uint32_t GetHeight() const { return m_nHeight;  }

		/** @see Texture::GetSampleCount */
		uint32_t GetSampleCount() const { return m_nSampleCount; }

		/** @see RenderTarget::GetTexture */
		virtual Noesis::Texture* GetTexture() override { return m_pTexture.GetPtr(); }
		
		/** Return the render target surface interface. */
		IDirect3DSurface9* GetDepthStencilSurface() const { return m_pDepthStencilSurface; }

		/** Release the internal resources needed by this object. */
		void releaseResources();

		/** Acquire the internal resources needed by this object. */
		void acquireResources(IDirect3DSurface9* pDepthStencil);


	protected:
		std::string							m_strLabel;				// The label of this render target.
		uint32_t							m_nWidth;				// The width of this render target.
		uint32_t							m_nHeight;				// The height of this render target.
		uint32_t							m_nSampleCount;			// The samples count of this render target.
		Noesis::Ptr<NoesisGUITextureD3D9>	m_pTexture;				// The texture interface.
		IDirect3DSurface9*					m_pDepthStencilSurface;	// The depth & stencil surface.
		
	};

}

#endif
#endif