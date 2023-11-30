#pragma once

#ifdef CEF_UI_BROWSER

class CBrowserRendererHandler : public CefRenderHandler
{
public:
	CBrowserRendererHandler( int _iWidth, int _iHeight );
	virtual ~CBrowserRendererHandler();

	void					Init();
	void					Shutdown();

	//CefRenderHandler interface
	bool GetViewRect( CefRefPtr<CefBrowser> browser, CefRect &rect );

	void OnAfterCreated( CefRefPtr<CefBrowser> browser );

	void OnPaint( CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height );

	void					Render();

	void					Frame();

private:
	DX::Sprite_ptr			pSprite;
	DX::Texture_ptr			pTerxture;
	DX::RenderTarget_ptr	pRenderTarget;

	int						iWidth;
	int						iHeight;

	//CefBase interface
	IMPLEMENT_REFCOUNTING( CBrowserRendererHandler );
};

#endif