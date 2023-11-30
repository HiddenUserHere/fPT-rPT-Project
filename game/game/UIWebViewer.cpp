#include "stdafx.h"
#include "UIWebViewer.h"

#ifdef _ULTRALIGHT_WEB

#include "UltralightFileSystemWin.h"

UIWebViewerApp * pcWebViewerApp = nullptr;

ultralight::RefPtr<ultralight::View> pcView = nullptr;

DX::Texture_ptr    pTextureUltralight = nullptr;

Rectangle2D rRectangleTexture;

void ReleaseUltralightTexture()
{
    if ( pTextureUltralight )
        pTextureUltralight->Get()->Release();

    pTextureUltralight = nullptr;
}


void CreateUltralightTexture()
{
    ReleaseUltralightTexture();

    LPDIRECT3DTEXTURE9 lpTexture;
    HRESULT hr;
    D3DFORMAT d3dFormat = GetGraphicsDevice()->GetInfo().d3dBackBufferFormat == D3DFMT_X4R4G4B4 ? D3DFMT_A4R4G4B4 : D3DFMT_A8R8G8B8;
    if ( FAILED( hr = DX::Graphic::GetDevice()->CreateTexture( rRectangleTexture.iWidth, rRectangleTexture.iHeight, 1, D3DUSAGE_DYNAMIC, d3dFormat,
                                                               D3DPOOL_DEFAULT, &lpTexture, NULL ) ) )
    {
        WRITEDBG( "CreateTexture Browser [0x%08X]", hr );
    }

    pTextureUltralight = GetGraphicsDevice()->GetTextureFactory()->MakeTemporaryTexture( lpTexture );
}


namespace UI
{
WebViewer::WebViewer( Rectangle2D rRectangle ) : Element( rRectangle )
{
    pcView->Resize( rRectangle.iWidth, rRectangle.iHeight );

    pcView->set_view_listener( this );
    pcView->set_load_listener( this );

    rRectangleTexture = rRectangle;

    CreateUltralightTexture();

    pEffect = GetGraphicsDevice()->GetEffectFactory()->MakeEffect( "game\\scripts\\shaders\\UI\\UltralightColor.fx" );
}

WebViewer::~WebViewer()
{
    pcView->set_view_listener( nullptr );
    pcView->set_load_listener( nullptr );

    LPDIRECT3DTEXTURE9 lpTexture = pTextureUltralight->Get();
    RELEASE( lpTexture );

    //pcWebViewerApp->GetRenderer()->Update();
}

void WebViewer::LoadFromHTMLFile( const std::string strFile )
{
    std::string strFile_ = std::string( "file:///" ) + strFile;

    std::replace( strFile_.begin(), strFile_.end(), '\\', '/' );

    pcView->LoadURL( strFile_.c_str() );
    //pcView->LoadURL( "https://www.google.com/" );

    //pcView->LoadHTML( "<h1>Hello!</h1><p>Welcome!</p>" );
}

void WebViewer::CallFunction( const std::string strFunction )
{
    pcView->EvaluateScript( ultralight::String( strFunction.c_str() ) );
}

void WebViewer::Update( float fTime )
{
    //pcWebViewerApp->GetRenderer()->Update();
}

void WebViewer::Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
    pcWebViewerApp->GetRenderer()->Update();

    if ( CanRender() == false )
        return;

    if ( IsOpen() )
    {
        ultralight::Platform::instance().gpu_driver()->BeginSynchronize();
        pcWebViewerApp->GetRenderer()->Render();
        ultralight::Platform::instance().gpu_driver()->EndSynchronize();
    }

    if ( ultralight::Platform::instance().gpu_driver()->HasCommandsPending() )
        ultralight::Platform::instance().gpu_driver()->DrawCommandList();

    int iRenderX = GetX() + iX;
    int iRenderY = GetY() + iY;
    int iRenderWidth = low( GetWidth(), iWidth );
    int iRenderHeight = low( GetHeight(), iHeight );


    if ( IsOpen() && pcView->is_bitmap_dirty() )
    {
        ultralight::RefPtr<ultralight::Bitmap> bitmap = pcView->bitmap();

        if ( Element::GetScale() != 1.0f )
        {
            iRenderWidth = GetWidth();
            iRenderHeight = GetHeight();

            iRenderX = GetX() + iX;
            iRenderY = GetY() + iY;
        }
        bitmap->LockPixels();
        void * pData = bitmap->raw_pixels();

        D3DLOCKED_RECT d3dRect;
        RECT rc;
        rc.left = 0;
        rc.top = 0;
        rc.right = rRect.iWidth;
        rc.bottom = rRect.iHeight;

        HRESULT hr = pTextureUltralight->Get()->LockRect( 0, &d3dRect, &rc, D3DLOCK_DISCARD );

        BYTE * pbaDest = (BYTE *)d3dRect.pBits;
        BYTE * pbaSource = (BYTE *)pData;

        auto rows = low( bitmap->row_bytes(), d3dRect.Pitch );
        auto height = low( bitmap->height(), iRenderHeight );

        for ( unsigned int y = 0; y < height; ++y )
        {
            memcpy( pbaDest + d3dRect.Pitch * y,
                    pbaSource + bitmap->row_bytes() * y,
                    rows );
        }

        pTextureUltralight->Get()->UnlockRect( 0 );
        

        bitmap->UnlockPixels();
    }

    UI::ImageRender::SetEffect( pEffect );
    pEffect->SetTechnique( "SwapUTColor" );

    if ( pEffect->Begin() >= 0 )
    {
        UI::ImageRender::Render( pTextureUltralight->Get(), iRenderX, iRenderY, iRenderWidth, iRenderHeight, iSourceX, iSourceY, -1, Element::GetScale() );
        //UI::ImageRender::RenderQuad( pTexture->Get(), iRenderX, iRenderY, iRenderWidth, iRenderHeight, -1 );

        pEffect->End();
    }

    UI::ImageRender::SetEffect( NULL );
}

void WebViewer::OnMouseMove( CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
    if ( !IsOpen() )
        return;

    Point2D * psPosition = pcMouse->GetPosition();

    int iRenderX = GetX() + iX + iSourceX;
    int iRenderY = GetY() + iY + iSourceY;
    int iRenderWidth = low( GetWidth(), iWidth );
    int iRenderHeight = low( GetHeight(), iHeight );

    // Inside X?
    if ( (psPosition->iX >= iRenderX) && (psPosition->iX <= (iRenderX + iRenderWidth - iSourceX)) )
    {
        // Inside Y?
        if ( (psPosition->iY >= iRenderY) && (psPosition->iY <= (iRenderY + iRenderHeight - iSourceY)) )
        {
            ultralight::MouseEvent sEvent;
            sEvent.button = ultralight::MouseEvent::Button::kButton_Left;
            if ( (pcMouse->GetEvent() == EMouseEvent::ClickDownR) || (pcMouse->GetEvent() == EMouseEvent::ClickUpR) )
                sEvent.button = ultralight::MouseEvent::Button::kButton_Right;

            sEvent.type = ultralight::MouseEvent::Type::kType_MouseMoved;

            sEvent.x = psPosition->iX - iRenderX;
            sEvent.y = psPosition->iY - iRenderY;

            pcView->FireMouseEvent( sEvent );
        }
    }
}

BOOL WebViewer::OnMouseClick( CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
    if ( !IsOpen() )
        return FALSE;

    Point2D * psPosition = pcMouse->GetPosition();

    int iRenderX = GetX() + iX + iSourceX;
    int iRenderY = GetY() + iY + iSourceY;
    int iRenderWidth = low( GetWidth(), iWidth );
    int iRenderHeight = low( GetHeight(), iHeight );

    // Inside X?
    if ( (psPosition->iX >= iRenderX) && (psPosition->iX <= (iRenderX + iRenderWidth - iSourceX)) )
    {
        // Inside Y?
        if ( (psPosition->iY >= iRenderY) && (psPosition->iY <= (iRenderY + iRenderHeight - iSourceY)) )
        {
            ultralight::MouseEvent sEvent;
            sEvent.button = ultralight::MouseEvent::Button::kButton_Left;
            if ( (pcMouse->GetEvent() == EMouseEvent::ClickDownR) || (pcMouse->GetEvent() == EMouseEvent::ClickUpR) )
                sEvent.button = ultralight::MouseEvent::Button::kButton_Right;

            sEvent.type = ultralight::MouseEvent::Type::kType_MouseDown;
            if ( (pcMouse->GetEvent() == EMouseEvent::ClickUpL) || (pcMouse->GetEvent() == EMouseEvent::ClickUpR) )
                sEvent.type = ultralight::MouseEvent::Type::kType_MouseUp;

            sEvent.x = psPosition->iX - iRenderX;
            sEvent.y = psPosition->iY - iRenderY;

            pcView->FireMouseEvent( sEvent );

            return TRUE;
        }
    }

    return FALSE;
}

BOOL WebViewer::OnMouseScroll( CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
    if ( !IsOpen() )
        return FALSE;

    Point2D * psPosition = pcMouse->GetPosition();

    int iRenderX = GetX() + iX + iSourceX;
    int iRenderY = GetY() + iY + iSourceY;
    int iRenderWidth = low( GetWidth(), iWidth );
    int iRenderHeight = low( GetHeight(), iHeight );

    // Inside X?
    if ( (psPosition->iX >= iRenderX) && (psPosition->iX <= (iRenderX + iRenderWidth - iSourceX)) )
    {
        // Inside Y?
        if ( (psPosition->iY >= iRenderY) && (psPosition->iY <= (iRenderY + iRenderHeight - iSourceY)) )
        {
            pcView->FireScrollEvent( ultralight::ScrollEvent{ultralight::ScrollEvent::Type::kType_ScrollByPixel, 0, (int)(pcMouse->GetDistance() * 20)} );

            return TRUE;
        }
    }
    return FALSE;
}

BOOL WebViewer::OnKeyChar( CKeyboard * pcKeyboard )
{
    pcView->FireKeyEvent( ultralight::KeyEvent( ultralight::KeyEvent::Type::kType_Char, pcKeyboard->GetChar(), pcKeyboard->GetLParam(), false ) );

    return 0;
}

BOOL WebViewer::OnKeyPress( CKeyboard * pcKeyboard )
{
    ultralight::KeyEvent sEvent = ultralight::KeyEvent(
        pcKeyboard->GetEvent() == EKeyboardEvent::KeyDown ? ultralight::KeyEvent::Type::kType_RawKeyDown : ultralight::KeyEvent::Type::kType_KeyUp,
        pcKeyboard->GetKey(), pcKeyboard->GetLParam(), false );

    sEvent.virtual_key_code = pcKeyboard->GetKey();
    sEvent.native_key_code = pcKeyboard->GetKey();

    pcView->FireKeyEvent( sEvent );

    return 0;
}
void WebViewer::OnDOMReady( ultralight::View * caller )
{
    bCanRender = true;

    if ( iEventOnDOMReady >= 0 )
        UI::Event::Invoke( iEventOnDOMReady );
}
void WebViewer::OnAddConsoleMessage( ultralight::View * caller, ultralight::MessageSource source, ultralight::MessageLevel level, const ultralight::String & message, uint32_t line_number, uint32_t column_number, const ultralight::String & source_id )
{
    auto ToUTF8 = []( const ultralight::String & str ) -> std::string
    {
        ultralight::String8 utf8 = str.utf8();
        return std::string( utf8.data(), utf8.length() );
    };

    auto Stringify = []( ultralight::MessageSource source ) -> const char *
    {
        switch ( source )
        {
            case ultralight::kMessageSource_XML: return "XML";
            case ultralight::kMessageSource_JS: return "JS";
            case ultralight::kMessageSource_Network: return "Network";
            case ultralight::kMessageSource_ConsoleAPI: return "ConsoleAPI";
            case ultralight::kMessageSource_Storage: return "Storage";
            case ultralight::kMessageSource_AppCache: return "AppCache";
            case ultralight::kMessageSource_Rendering: return "Rendering";
            case ultralight::kMessageSource_CSS: return "CSS";
            case ultralight::kMessageSource_Security: return "Security";
            case ultralight::kMessageSource_ContentBlocker: return "ContentBlocker";
            case ultralight::kMessageSource_Other: return "Other";
            default: return "";
        }
    };

    auto StringifyLevel = []( ultralight::MessageLevel level ) -> const char *
    {
        switch ( level )
        {
            case ultralight::kMessageLevel_Log: return "Log";
            case ultralight::kMessageLevel_Warning: return "Warning";
            case ultralight::kMessageLevel_Error: return "Error";
            case ultralight::kMessageLevel_Debug: return "Debug";
            case ultralight::kMessageLevel_Info: return "Info";
            default: return "";
        }
    };

    ultralight::String str = ultralight::String ( "[Console]: [" ) + Stringify( source ) + "] [" +
        StringifyLevel( level ) + "] " + ultralight::String(ToUTF8( message ).c_str());

    //WRITEERR( "%s", ToUTF8( str ).c_str() );
}
void WebViewer::CloseApp()
{
    DELET( pcWebViewerApp );
}
void WebViewer::OnLostDevice()
{
    ReleaseUltralightTexture();
}
void WebViewer::OnResetDevice()
{
    CreateUltralightTexture();
}
};

UIWebViewerApp * UIWebViewerApp::GetApp()
{
    return pcWebViewerApp;
}



void UIWebViewerApp::CreateApp()
{
    if ( pcWebViewerApp == nullptr )
    {
        WCHAR szPath[MAX_PATH] = {0};
        GetCurrentDirectoryW( MAX_PATH, szPath );

        ultralight::Platform & platform = ultralight::Platform::instance();

        ultralight::Config sConfig;
        CopyMemory( &sConfig, &platform.config(), sizeof( ultralight::Config ) );

        sConfig.use_bgra_for_offscreen_rendering = false;

        platform.set_config( sConfig );

        platform.set_file_system( new ultralight::FileSystemWin( szPath ) );

        pcWebViewerApp = new UIWebViewerApp();

        if ( pcWebViewerApp->GetRenderer() )
            pcView = pcWebViewerApp->GetRenderer()->CreateView( 800, 600, true );
    }
}
#endif
