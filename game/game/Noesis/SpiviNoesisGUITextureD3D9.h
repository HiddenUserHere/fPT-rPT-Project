/*
-----------------------------------------------------------------------------
This source file is part of spivi runtime.
For the latest info, see http://www.spiviTech.com

Copyright (c) SpiviTech Ltd
-----------------------------------------------------------------------------
*/

#ifdef _NOESIS_GUI


#ifndef __Spivi_NoesisGUITextureD3D9_H__
#define __Spivi_NoesisGUITextureD3D9_H__

#include <string>
#include <vector>

#include <Noesis_pch.h>

#include <d3d9.h>

namespace Ogre
{
	class D3D9Texture;
}

namespace Spivi
{
	//-----------------------------------------------------------------------------
	class NoesisGUITextureD3D9 final : public Noesis::Texture
	{
	public:
		NoesisGUITextureD3D9();
		virtual ~NoesisGUITextureD3D9();
		NoesisGUITextureD3D9(const char* lpszLabel, IDirect3DTexture9* pTexture);

		/** Return the label of this texture. */
		const std::string & GetLabel() const { return m_strLabel;  }

		/** Return the texture interface of this texture. */
		IDirect3DTexture9* GetTexture() const { return m_pTexture;  }

		/** Return the surface interface of this texture. */
		IDirect3DSurface9* GetSurface() const { return m_pSurface; }

		/** @see Texture::GetWidth */
		virtual uint32_t GetWidth() const override;

		/** @see Texture::GetHeight */
		virtual uint32_t GetHeight() const override;

		/** @see Texture::GetWidth */
		virtual bool HasMipMaps() const override;

		/** @see Texture::GetWidth */
		virtual bool IsInverted() const override;

		/** Update the content of this texture. */
		void UpdateContent(uint32_t level, uint32_t x, uint32_t y, uint32_t width, uint32_t height, const void* data);

		/** Release the internal resources needed by this object. */
		void releaseResources();

		/** Acquire the internal resources needed by this object. */
		void acquireResources();

		/** Acquire the internal resources needed by this object. */
		void acquireResources(IDirect3DTexture9* pTexture);

		typedef std::vector<NoesisGUITextureD3D9*> TextureArray;
		static TextureArray			s_Instances;			// The instances list.

	protected:
		std::string					m_strLabel;				// The label of this texture.
		IDirect3DTexture9*			m_pTexture;				// The texture interface.
		IDirect3DSurface9*			m_pSurface;				// The surface interface.
		D3DSURFACE_DESC				m_oSurfaceDesc;			// The surface desc.
		uint32_t					m_nLevels;				// The levels count of this texture.
		Noesis::TextureFormat::Enum m_eFormat;				// The format of this texture.
	};

}

#endif
#endif