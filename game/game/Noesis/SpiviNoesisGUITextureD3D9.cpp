/*
-----------------------------------------------------------------------------
This source file is part of spivi runtime.
For the latest info, see http://www.spiviTech.com

Copyright (c) SpiviTech Ltd
-----------------------------------------------------------------------------
*/

#include "stdafx.h"

#ifdef _NOESIS_GUI

#include "SpiviNoesisGUITextureD3D9.h"


#include <d3d9.h>
#include <DxErr.h>

namespace Spivi
{
	NoesisGUITextureD3D9::TextureArray NoesisGUITextureD3D9::s_Instances;

	//-----------------------------------------------------------------------------
	NoesisGUITextureD3D9::NoesisGUITextureD3D9() : m_pTexture(nullptr), m_pSurface(nullptr), m_nLevels(0), m_eFormat(Noesis::TextureFormat::Count)
	{
		s_Instances.push_back(this);
	}

	//-----------------------------------------------------------------------------
	NoesisGUITextureD3D9::NoesisGUITextureD3D9(const char* lpszLabel, IDirect3DTexture9* pTexture) : m_strLabel(lpszLabel), m_pTexture(pTexture), m_pSurface(nullptr)
	{
		HRESULT hr;
		
		// Get the main surface descriptor.
		hr = m_pTexture->GetLevelDesc(0, &m_oSurfaceDesc);
		if (FAILED(hr))
		{
			std::string strErrorMessage = DXGetErrorDescription(hr);
			//OutCStringStream os;
			//spivi_log_error(os << "NoesisGUITextureD3D9 Failed to GetLevelDesc: " << strErrorMessage);
		}

		switch (m_oSurfaceDesc.Format)
		{
		case D3DFMT_A8R8G8B8:
			m_eFormat = Noesis::TextureFormat::RGBA8;
			break;
		case D3DFMT_X8R8G8B8:
			m_eFormat = Noesis::TextureFormat::RGBA8;
			break;

		case D3DFMT_L8:
			m_eFormat = Noesis::TextureFormat::R8;
			break;

		default:
			break;
		}

		// Get the levels count.
		m_nLevels = m_pTexture->GetLevelCount();

		acquireResources();

		s_Instances.push_back(this);
	}

	//-----------------------------------------------------------------------------
	NoesisGUITextureD3D9::~NoesisGUITextureD3D9()
	{
		releaseResources();

		std::vector<NoesisGUITextureD3D9*>::iterator itFind = std::find(s_Instances.begin(), s_Instances.end(), this);
		if (itFind != s_Instances.end())
		{
			s_Instances.erase(itFind);
		}
	}

	//-----------------------------------------------------------------------------
	uint32_t NoesisGUITextureD3D9::GetWidth() const
	{
		return m_oSurfaceDesc.Width;
	}

	//-----------------------------------------------------------------------------
	uint32_t NoesisGUITextureD3D9::GetHeight() const
	{
		return m_oSurfaceDesc.Height;
	}


	//-----------------------------------------------------------------------------
	bool NoesisGUITextureD3D9::HasMipMaps() const
	{
		return m_nLevels > 1;
	}

	//-----------------------------------------------------------------------------
	bool NoesisGUITextureD3D9::IsInverted() const
	{
		return false;
	}

	//-----------------------------------------------------------------------------
	void NoesisGUITextureD3D9::acquireResources()
	{
		acquireResources(m_pTexture);
	}

	//-----------------------------------------------------------------------------
	void NoesisGUITextureD3D9::acquireResources(IDirect3DTexture9* pTexture)
	{
		m_pTexture = pTexture;

		if (m_pTexture != nullptr)
		{
			HRESULT hr = m_pTexture->GetSurfaceLevel(0, &m_pSurface);

			if (FAILED(hr))
			{
				//CString strErrorMessage = DXGetErrorDescription(hr);
				//OutCStringStream os;
				//spivi_log_error(os << "NoesisGUITextureD3D9 Failed to GetSurfaceLevel: " << strErrorMessage);
			}
		}
	}

	//-----------------------------------------------------------------------------
	void NoesisGUITextureD3D9::releaseResources()
	{
		ULONG ulRefCount = 0;

		if (m_pSurface != nullptr)
		{
			ulRefCount = m_pSurface->Release();
			m_pSurface = nullptr;
		}

		if (m_pTexture != nullptr)
		{
			ulRefCount = m_pTexture->Release();
			m_pTexture = nullptr;
		}
	}

	//-----------------------------------------------------------------------------
	void NoesisGUITextureD3D9::UpdateContent(uint32_t level, uint32_t x, uint32_t y, uint32_t width, uint32_t height, const void * data)
	{
		if (m_pSurface != nullptr)
		{
			D3DLOCKED_RECT oLockedRect;
			RECT oTargetRect;

			oTargetRect.left = x;
			oTargetRect.top = y;
			oTargetRect.right = x + width;
			oTargetRect.bottom = y + height;

			HRESULT hr;

			hr = m_pSurface->LockRect(&oLockedRect, &oTargetRect, D3DLOCK_DISCARD);
			if (SUCCEEDED(hr))
			{
				UINT nBytesPerPixel = m_eFormat == Noesis::TextureFormat::R8 ? 1 : 4;
				if (oLockedRect.Pitch == width * nBytesPerPixel)
				{
					//memcpy(oLockedRect.pBits, data, width * height * nBytesPerPixel);

					BYTE * pDstMemory = (BYTE *)oLockedRect.pBits;
					BYTE * pSrcMemory = (BYTE *)data;

					for ( UINT nRow = 0; nRow < height; nRow++ )
					{
						for ( UINT i = 0; i < width; i++ )
						{
							UINT u = (nRow * width) + (i * 4);

							pDstMemory[u + 2] = pSrcMemory[u];
							pDstMemory[u + 1] = pSrcMemory[u + 1];
							pDstMemory[u] = pSrcMemory[u + 2];
							pDstMemory[u + 3] = pSrcMemory[u + 3];
						}
					}
				}
				else
				{
					BYTE * pDstMemory = (BYTE *)oLockedRect.pBits;
					BYTE * pSrcMemory = (BYTE *)data;

					for (UINT nRow = 0; nRow < height; nRow++ )
					{
						/*
						for ( UINT i = 0; i < width; i++ )
						{
							UINT u = (nRow * width) + (i * 4);

							pDstMemory[u + 2] = pSrcMemory[u];
							pDstMemory[u + 1] = pSrcMemory[u + 1];
							pDstMemory[u] = pSrcMemory[u + 2];
							pDstMemory[u + 3] = pSrcMemory[u + 3];
						}
						*/
						BYTE * pDstMemory = (BYTE *)(oLockedRect.pBits) + nRow * oLockedRect.Pitch;
						BYTE * pSrcMemory = (BYTE *)(data) + nRow * width * nBytesPerPixel;

						memcpy(pDstMemory, pSrcMemory, width * nBytesPerPixel);
						
					}
				}
				
				hr = m_pSurface->UnlockRect();
				if (FAILED(hr))
				{
					//CString strErrorMessage = DXGetErrorDescription(hr);
					//OutCStringStream os;
					//spivi_log_error(os << "NoesisGUITextureD3D9 Failed to LockRect: " << strErrorMessage);
				}
			}

			else
			{
				//CString strErrorMessage = DXGetErrorDescription(hr);
				//OutCStringStream os;
				//spivi_log_error(os << "NoesisGUITextureD3D9 Failed to LockRect: " << strErrorMessage);
			}
		}
	}
}

#endif