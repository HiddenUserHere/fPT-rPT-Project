/*
-----------------------------------------------------------------------------
This source file is part of spivi runtime.
For the latest info, see http://www.spiviTech.com

Copyright (c) SpiviTech Ltd
-----------------------------------------------------------------------------
*/

#ifndef __Spivi_NoesisGUIRendererD3D9_H__
#define __Spivi_NoesisGUIRendererD3D9_H__

#include <Noesis_pch.h>


#include <d3d9.h>


namespace Spivi
{
	/** 
	NoesisGUI renderer D3D9 implementation class.
	*/
	class NoesisGUIRendererD3D9 : public Noesis::NoesisGUIRenderer, public Ogre::RenderSystem::Listener
	{

		// Interface.
	public:
		NoesisGUIRendererD3D9(Ogre::RenderSystem* pRenderSystem);
		virtual ~NoesisGUIRendererD3D9();

		/** @see NoesisGUIRenderer::registerRenderWindow */
		virtual void registerRenderWindow(Ogre::RenderWindow* pRenderWindow);

		/** @see NoesisGUIRenderer::unregisterRenderWindow */
		virtual void unregisterRenderWindow(Ogre::RenderWindow* pRenderWindow);
		
		/** @see NoesisGUIRenderer::renderGUIControls */
		virtual void renderGUIControls(NoesisGUIControlArray& aGUIControls);

		/** @see NoesisGUIRenderer::GetRenderDevice */
		virtual Noesis::RenderDevice* getRenderDevice() const;

		/** @see NoesisGUIRenderer::createImageSource */
		virtual NsImageSourcePtr createImageSource(Ogre::Texture* pTexture);
		
		/** @see Ogre::RenderSystem::Listener::eventOccurred */
		virtual void eventOccurred(const Ogre::String& eventName, const Ogre::NameValuePairList* parameters);

		// Protected types.
	protected:

		/** Device state. */
		enum DeviceState
		{
			DS_Valid,
			DS_Lost,			
		};
	

		// Protected methods.
	protected:

		/** D3D9 Device lost handle method. */
		void handleDeviceLost();

		/** D3D9 Device reset handle method. */
		void handleDeviceReset();

		
		// Attributes.
	protected:
		Ogre::RenderSystem*					m_pOgreRenderSystemD3D9;		// The underlying Ogre D3D9 Render System.
		DeviceState							m_eDeviceState;					// The current state of the active device.
		Noesis::Ptr<Noesis::RenderDevice>	m_pRenderDeviceD3D9;				// The render device implementation.
	};

}

#endif