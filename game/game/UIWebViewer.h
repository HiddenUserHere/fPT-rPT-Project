#pragma once
#include "UIElement.h"

#ifdef _ULTRALIGHT_WEB

class UIWebViewerApp : public ultralight::LoadListener
{
protected:
    ultralight::RefPtr<ultralight::Renderer> renderer_;
    UINT               uRef = 0;
public:
    UIWebViewerApp()
    {
        renderer_ = ultralight::Renderer::Create();
    }

    virtual ~UIWebViewerApp() { renderer_ = nullptr; }

    ultralight::RefPtr<ultralight::Renderer> & GetRenderer() { return renderer_; }

    void Update()
    {
        //renderer_->Update();
    }

    static UIWebViewerApp * GetApp();

    static void            CreateApp();


    void AddRef() { uRef++; }
    UINT DecRef() { uRef--; return uRef; }
};

namespace UI
{
typedef std::shared_ptr<class WebViewer> WebViewer_ptr;
class WebViewer : public Element, private ultralight::ViewListener, private ultralight::LoadListener
{
public:
    WebViewer( Rectangle2D rRectangle );

    virtual                 ~WebViewer();


    void                    LoadFromHTMLFile( const std::string strFile );

    void                    CallFunction( const std::string strFunction );

    void                    SetEventOnDOMReady( int iEventID ) { iEventOnDOMReady = iEventID; };

    void                    Update( float fTime );

    void                    Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );

    //Event Handlers
    void					OnMouseMove( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );
    BOOL				    OnMouseClick( CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );
    BOOL					OnMouseScroll( CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );
    BOOL					OnKeyChar( class CKeyboard * pcKeyboard );
    BOOL					OnKeyPress( class CKeyboard * pcKeyboard );

    void                    OnDOMReady( ultralight::View * caller ) override;

    bool                    CanRender() { return bCanRender; }

    void OnAddConsoleMessage( ultralight::View * caller,
                              ultralight::MessageSource source,
                              ultralight::MessageLevel level,
                              const ultralight::String & message,
                              uint32_t line_number,
                              uint32_t column_number,
                              const ultralight::String & source_id ) override;

    static void             CloseApp();

    static void             OnLostDevice();

    static void             OnResetDevice();

private:

    DX::RenderTarget_ptr    pRenderTarget = nullptr;

    DX::Effect_ptr          pEffect = nullptr;

    bool                    bCanRender = false;

    int                     iEventOnDOMReady = -1;
};
};

#endif