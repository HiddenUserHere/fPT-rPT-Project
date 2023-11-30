#pragma once

#include "CBrowserRendererHandler.h"

#ifdef CEF_UI_BROWSER

class CBrowserClient : public CefClient
{
public:
	CBrowserClient( CBrowserRendererHandler * renderHandler )
		: m_renderHandler( renderHandler ), pRenderer( renderHandler )
	{
	}

	virtual CefRefPtr<CefRenderHandler> GetRenderHandler()
	{
		return m_renderHandler;
	}

	CBrowserRendererHandler		* GetWindowRenderer() { return pRenderer; }

	CefRefPtr<CefRenderHandler> m_renderHandler;

	CBrowserRendererHandler		* pRenderer;

	IMPLEMENT_REFCOUNTING( CBrowserClient );
};



class CBrowserWindowHandler
{
public:
	CBrowserWindowHandler( int iWidth, int iHeight );
	virtual ~CBrowserWindowHandler();

	void						Init();

	void						Shutdown();

	void						OnMouseMove( CMouse * pcMouse );
	
	BOOL						OnMouseClick( CMouse * pcMouse );

	CefRefPtr<CBrowserClient>	& GetClient() { return pcBrowserClient; }

private:
	CefRefPtr<CefBrowser>		pcBrowser;
	CefRefPtr<CBrowserClient>	pcBrowserClient;

	CefBrowserSettings			sSettings;
	CefWindowInfo				sWindowInfo;
};

#endif