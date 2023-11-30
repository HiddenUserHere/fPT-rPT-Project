/*
-----------------------------------------------------------------------------
This source file is part of spivi runtime.
For the latest info, see http://www.spiviTech.com

Copyright (c) SpiviTech Ltd
-----------------------------------------------------------------------------
*/
#include "stdafx.h"

#ifdef _NOESIS_GUI
#include "SpiviNoesisGUIRenderTargetD3D9.h"

#include <d3d9.h>
#include <DxErr.h>

namespace Spivi
{
	//-----------------------------------------------------------------------------
	NoesisGUIRenderTargetD3D9::NoesisGUIRenderTargetD3D9() : m_pTexture(nullptr), m_pDepthStencilSurface(nullptr)
	{

	}

	//-----------------------------------------------------------------------------
	NoesisGUIRenderTargetD3D9::NoesisGUIRenderTargetD3D9(const char* lpszLabel, uint32_t width, uint32_t height, uint32_t sampleCount, 
		Noesis::Ptr<NoesisGUITextureD3D9> pTexture, IDirect3DSurface9* pDepthStencilSurface) :
		m_strLabel(lpszLabel), 
		m_nWidth(width),
		m_nHeight(height),
		m_nSampleCount(sampleCount),
		m_pTexture(pTexture),
		m_pDepthStencilSurface(pDepthStencilSurface)
	{
		
	}

	//-----------------------------------------------------------------------------
	NoesisGUIRenderTargetD3D9::~NoesisGUIRenderTargetD3D9()
	{
		releaseResources();
	}
	
	//-----------------------------------------------------------------------------
	void NoesisGUIRenderTargetD3D9::releaseResources()
	{
		if (m_pDepthStencilSurface != nullptr)
		{
			m_pDepthStencilSurface->Release();
			m_pDepthStencilSurface = nullptr;
		}
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderTargetD3D9::acquireResources(IDirect3DSurface9* pDepthStencil)
	{
		m_pDepthStencilSurface = pDepthStencil;
	}

}

#endif