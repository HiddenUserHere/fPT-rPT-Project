/*
-----------------------------------------------------------------------------
This source file is part of spivi runtime.
For the latest info, see http://www.spiviTech.com

Copyright (c) SpiviTech Ltd
-----------------------------------------------------------------------------
*/
#include "stdafx.h"

#include "SpiviNoesisGUIRendererD3D9.h"
#include "SpiviNoesisGUIRenderDeviceD3D9.h"
#include "SpiviNoesisGUITextureD3D9.h"




namespace Spivi
{
	
	//-----------------------------------------------------------------------------
	NoesisGUIRendererD3D9::NoesisGUIRendererD3D9(Ogre::RenderSystem* pRenderSystem) : m_pOgreRenderSystemD3D9(pRenderSystem)
	{
		m_pOgreRenderSystemD3D9->addListener(static_cast<Ogre::RenderSystem::Listener*>(this));
		m_eDeviceState = DS_Valid;
	}

	//-----------------------------------------------------------------------------
	NoesisGUIRendererD3D9::~NoesisGUIRendererD3D9()
	{
		m_pOgreRenderSystemD3D9->removeListener(static_cast<Ogre::RenderSystem::Listener*>(this));
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRendererD3D9::registerRenderWindow(Ogre::RenderWindow* pRenderWindow)
	{
		IDirect3DDevice9* pDeviceD3D9 = NULL;

		pRenderWindow->getCustomAttribute("D3DDEVICE", &pDeviceD3D9);

		if (pDeviceD3D9 != NULL)
		{
			m_pRenderDeviceD3D9 = *new NoesisGUIRenderDeviceD3D9(pDeviceD3D9);
		}
		else
		{
			//spivi_log_error("NoesisGUIRendererD3D9::registerRenderWindow Could not get D3D9DEVICE !!");
		}
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRendererD3D9::unregisterRenderWindow(Ogre::RenderWindow* pRenderWindow)
	{
		IDirect3DDevice9* pDeviceD3D9 = NULL;

		pRenderWindow->getCustomAttribute("D3DDEVICE", &pDeviceD3D9);

		if (pDeviceD3D9 != NULL)
		{
			//destroyDeviceResources(pDeviceD3D9);
		}
		else
		{
			spivi_log_error("NoesisGUIRendererD3D9::unregisterRenderWindow Could not get D3D9DEVICE !!");
		}
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRendererD3D9::renderGUIControls(NoesisGUIControlArray& aGUIControls)
	{
		NoesisGUIRenderDeviceD3D9* pRenderDeviceImpl = static_cast<NoesisGUIRenderDeviceD3D9*>(m_pRenderDeviceD3D9.GetPtr());

		if (pRenderDeviceImpl->canRender())
		{
			// Capture the current target surfaces.
			pRenderDeviceImpl->captureRenderSurfaces();

			// Capture the render state.
			pRenderDeviceImpl->captureRenderState();

			NoesisGUIControlArrayConstIterator it;
			NoesisGUIControlArrayConstIterator itEnd;

			// Update the render tree.
			it = aGUIControls.begin();
			itEnd = aGUIControls.end();
			for (; it != itEnd; ++it)
			{
				NoesisGUIControl* pControl = *it;
				NsIRendererPtr pRenderer = pControl->getRenderer();

				if (pControl->getActive())
				{
					pRenderer->UpdateRenderTree();
				}
			}

			// Do the first phase of off-screen rendering.
			it = aGUIControls.begin();
			itEnd = aGUIControls.end();
			for (; it != itEnd; ++it)
			{
				NoesisGUIControl* pControl = *it;

				if (pControl->getActive())
				{
					NsIRendererPtr pRenderer = pControl->getRenderer();

					if (pRenderer->NeedsOffscreen())
					{
						pRenderer->RenderOffscreen();
					}
				}
			}

			// Restore render surfaces.
			pRenderDeviceImpl->restoreRenderSurfaces();

			// Do the second phase of rendering to target surfaces.
			it = aGUIControls.begin();
			itEnd = aGUIControls.end();
			for (; it != itEnd; ++it)
			{
				NoesisGUIControl* pControl = *it;

				if (pControl->getActive())
				{
					NsIRendererPtr pRenderer = pControl->getRenderer();

					pRenderer->Render();
				}
			}

			// Restore the render state.
			pRenderDeviceImpl->restoreRenderState();
		}
	}

	//-----------------------------------------------------------------------------
	NsImageSourcePtr NoesisGUIRendererD3D9::createImageSource(Ogre::Texture* pTexture)
	{
		if (pTexture != NULL)
		{
			// Grab the native D3D9 Texture pointer.
			Ogre::D3D9Texture* pOgreD3D9Texture = static_cast<Ogre::D3D9Texture*>(pTexture);			
 			if (pOgreD3D9Texture != NULL)
 			{	
				Noesis::Ptr<NoesisGUITextureD3D9> pNsD3D9Texture = *new NoesisGUITextureD3D9(pOgreD3D9Texture->getName().c_str(), pOgreD3D9Texture);
 				NsImageSourcePtr pImageSource = *new Noesis::TextureSource(pNsD3D9Texture);
 				return pImageSource;	
			}
		}

		return NsImageSourcePtr();	
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRendererD3D9::eventOccurred( const Ogre::String& eventName, const Ogre::NameValuePairList* parameters )
	{
		if (eventName == "DeviceLost")
		{
			if (m_eDeviceState == DS_Valid)
			{
				handleDeviceLost();
				m_eDeviceState = DS_Lost;
			}
			
		}
		else if(eventName == "DeviceRestored")
		{
			handleDeviceReset();
			m_eDeviceState = DS_Valid;
		}
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRendererD3D9::handleDeviceLost()
	{
		NoesisGUIRenderDeviceD3D9* pRenderDeviceImpl = static_cast<NoesisGUIRenderDeviceD3D9*>(m_pRenderDeviceD3D9.GetPtr());

		pRenderDeviceImpl->handleDeviceLost();
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRendererD3D9::handleDeviceReset()
	{
		NoesisGUIRenderDeviceD3D9* pRenderDeviceImpl = static_cast<NoesisGUIRenderDeviceD3D9*>(m_pRenderDeviceD3D9.GetPtr());

		pRenderDeviceImpl->handleDeviceReset();
	}

	//-----------------------------------------------------------------------------
	Noesis::RenderDevice* NoesisGUIRendererD3D9::getRenderDevice() const
	{
		return m_pRenderDeviceD3D9.GetPtr();
	}
}

