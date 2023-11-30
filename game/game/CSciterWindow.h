#pragma once

#ifdef _SCITER_LIB_

class CSciterWindow
{
public:
    CSciterWindow();

    virtual ~CSciterWindow();

    LRESULT                     WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

    void                        Destroy();

    HWND                        GetWindowHandler() { return hWindow; }

    void                        Init( HINSTANCE hInstance, HWND hMainWindow, Rectangle2D rRectangle );

    void                        UpdateSciterWindow( Rectangle2D rRectangle );

private:
    HWND                        hMain;
    HINSTANCE                   hInst;

    //Handle for Sciter
    HWND                        hWindow;

    bool                        bCreated = false;

    Rectangle2D                 rRect;

    void                        CreateSciteWindow( HINSTANCE hInstance, HWND hMainWindow, Rectangle2D rRectangle );
};

#endif
