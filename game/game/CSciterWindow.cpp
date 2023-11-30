#include "stdafx.h"
#include "CSciterWindow.h"

#ifdef _SCITER_LIB_

namespace sciter
{
class dib32
{
    const unsigned _width;
    const unsigned _height;
    void * _bits;
    mutable HBITMAP    _old_bitmap;
    mutable HDC        _dc;
    HBITMAP    _bitmap;

    BITMAPINFO _bitmap_info;

public:

    dib32( unsigned w, unsigned h ) :
        _width( w ),
        _height( h ),
        _bits( 0 ),
        _old_bitmap( 0 ),
        _dc( 0 )
    {

        memset( &_bitmap_info, 0, sizeof( _bitmap_info ) );
        _bitmap_info.bmiHeader.biSize = sizeof( _bitmap_info.bmiHeader );
        _bitmap_info.bmiHeader.biWidth = _width;
        _bitmap_info.bmiHeader.biHeight = 0 - int( _height );
        _bitmap_info.bmiHeader.biPlanes = 1;
        _bitmap_info.bmiHeader.biBitCount = 32;
        _bitmap_info.bmiHeader.biCompression = BI_RGB;

        _bitmap = ::CreateDIBSection(
            NULL, // device context
            &_bitmap_info,
            DIB_RGB_COLORS,
            &_bits,
            0, // file mapping object
            0 ); // file offset

        if ( 0 == _bits )
        {
            return;
            //throw std::bad_alloc();
        }

        memset( _bits, 0, _width * _height * 4 );

    }

    ~dib32()
    {
        if ( _dc )
        {
            ::SelectObject( _dc, _old_bitmap );
            ::DeleteDC( _dc );
        }
        if ( _bitmap )
            ::DeleteObject( _bitmap );
    }

    void set_white()
    {
        memset( _bits, 0xff, _width * _height * 4 );
    }

    unsigned  width() const { return _width; }
    unsigned  height() const { return _height; }
    void * bits() const { return _bits; }
    BYTE * bytes() const { return (BYTE *)_bits; }
    HDC   DC() const
    {
        if ( !_dc )
        {
            _dc = ::CreateCompatibleDC( NULL );
            if ( _dc )
                _old_bitmap = (HBITMAP)::SelectObject( _dc, _bitmap );
        }
        return _dc;
    }

    HBITMAP   detach() { auto r = _bitmap; _bitmap = 0; return r; }

    const BITMAPINFO & get_info() const { return _bitmap_info; }
};



};

CSciterWindow::CSciterWindow()
{
    // Enable these features, to be available from script 
    SciterSetOption( NULL, SCITER_SET_SCRIPT_RUNTIME_FEATURES,
                     ALLOW_FILE_IO |
                     ALLOW_SOCKET_IO |
                     ALLOW_EVAL |
                     ALLOW_SYSINFO );

    SciterSetOption( NULL, SCITER_SET_DEBUG_MODE, TRUE );
    
}

CSciterWindow::~CSciterWindow()
{
}

LRESULT CSciterWindow::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    BOOL    bHandled = FALSE;

    switch ( message )
    {
        case WM_PAINT:
            PAINTSTRUCT ps;
            BeginPaint( GetWindowHandler(), &ps );
            EndPaint( GetWindowHandler(), &ps );

            BOOL handled;
            sciter::dib32 bitmap( rRect.iWidth, rRect.iHeight );

            SciterProcND( hWnd, WM_PRINTCLIENT, (WPARAM)bitmap.DC(), 0, &handled );

            break;
    }

    LRESULT lResult = SciterProcND( hWnd, message, wParam, lParam, &bHandled );
    if ( bHandled )
        return lResult;

    return LRESULT();
}

void CSciterWindow::Destroy()
{
    if ( bCreated )
    {
        BOOL    bHandled = FALSE;
        SciterProcND( hMain, WM_DESTROY, 0, 0, &bHandled );

        if ( bHandled )
            bCreated = false;
    }
}

void CSciterWindow::Init( HINSTANCE hInstance, HWND hMainWindow, Rectangle2D rRectangle )
{
    hMain = hMainWindow;
    hInst = hInstance;
    rRect = rRectangle;

    CreateSciteWindow( hInst, hMain, rRectangle );

    SciterLoadFile( GetWindowHandler(), L"file://D:/RPT/Projects/sciter/samples/video/video-controls.htm" );
}

void CSciterWindow::UpdateSciterWindow( Rectangle2D rRectangle )
{
    rRect = rRectangle;

    CreateSciteWindow( hInst, hMain, rRectangle );
}

void CSciterWindow::CreateSciteWindow( HINSTANCE hInstance, HWND hMainWindow, Rectangle2D rRectangle )
{
    Destroy();

    if ( !bCreated )
    {
        if ( (hWindow = CreateWindowExW( 0, SciterClassName(), L"PT", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, rRectangle.iWidth, rRectangle.iHeight, hMain, NULL, 0, NULL )) == 0 )
            ERRORBOX_INT( GetLastError() );

        

        ShowWindow( hWindow, SW_SHOW );

        bCreated = true;
    }
}


#endif
