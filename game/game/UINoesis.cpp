#include "stdafx.h"
#include "UINoesis.h"

namespace UI
{

#ifdef _NOESIS_GUI
typedef std::shared_ptr<Spivi::NoesisGUIRenderDeviceD3D9> RenderDeviceNoesis_ptr;

RenderDeviceNoesis_ptr pNoesisRenderDevice = nullptr;
#endif

UINoesis::UINoesis( Rectangle2D rRectangle ) : Element( rRectangle )
{
#ifdef _NOESIS_GUI
    if ( pNoesisRenderDevice == nullptr )
    {
        pNoesisRenderDevice = std::make_shared<Spivi::NoesisGUIRenderDeviceD3D9>( GetGraphicsDevice()->Get() );

        Noesis::GUI::Init( nullptr );
    }

    pNoesisRenderDevice->AddReference();

    static UINT uWheelID = 12000;

    pRenderTarget = GetGraphicsDevice()->MakeRenderTarget( uWheelID++ );
    pSprite = GetGraphicsDevice()->GetSpriteFactory()->MakeSprite( TRUE );
#endif
}

UINoesis::~UINoesis()
{
#ifdef _NOESIS_GUI
    pView.Reset();

    pNoesisRenderDevice->Release();
#endif
}

void UINoesis::LoadFromFile( const std::string _strFile )
{
#ifdef _NOESIS_GUI
    static char szBufferXMLLoader[16385];

    if ( FileExist( _strFile ) )
    {
        //Open File
        FILE * pFile = nullptr;
        fopen_s( &pFile, _strFile.c_str(), "rb" );
        if ( pFile )
        {
            fseek( pFile, 0, SEEK_END );
            int iFileSize = ftell( pFile );
            fseek( pFile, 0, SEEK_SET );

            if ( iFileSize > (_countof( szBufferXMLLoader ) - 1) )
                iFileSize = (_countof( szBufferXMLLoader ) - 1);

            fread( szBufferXMLLoader, iFileSize, 1, pFile );
            fclose( pFile );

            szBufferXMLLoader[iFileSize] = 0;

            pView.Reset();

            strFile = _strFile;

            Noesis::Ptr<Noesis::UserControl> xaml( Noesis::GUI::ParseXaml<Noesis::UserControl>( szBufferXMLLoader ) );


            pView = Noesis::GUI::CreateView( xaml );
            pView->SetSize( 1024, 768 );
            pView->GetRenderer()->Init( pNoesisRenderDevice.get() );

            pView->SetIsPPAAEnabled( false );
        }
    }
#endif
}

void UINoesis::Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
#ifdef _NOESIS_GUI
    int iRenderX = GetX() + iX + iSourceX;
    int iRenderY = GetY() + iY + iSourceY;
    int iRenderWidth = low( GetWidth(), iWidth );
    int iRenderHeight = low( GetHeight(), iHeight );

    pView->GetRenderer()->UpdateRenderTree();

    if ( GetGraphicsDevice()->SetRenderTarget( pRenderTarget ) )
    {
        GetGraphicsDevice()->Clear( TRUE, TRUE, TRUE );

        pView->GetRenderer()->RenderOffscreen();

        pView->GetRenderer()->Render();

        GetGraphicsDevice()->UnsetRenderTarget();
    }

    if ( pSprite->Begin( TRUE ) )
    {
        pSprite->Draw( pRenderTarget );

        pSprite->End();
    }
    
    GetGraphicsDevice()->Get()->SetVertexShader( nullptr );
    GetGraphicsDevice()->Get()->SetPixelShader( nullptr );
#endif
}

void UINoesis::Update( float fTime )
{
#ifdef _NOESIS_GUI
    static uint64_t startTime = Noesis::HighResTimer::Ticks();

    // Update (layout, animations). Note that global time is used, not a delta
    uint64_t time = Noesis::HighResTimer::Ticks();
    pView->Update( Noesis::HighResTimer::Seconds( startTime - time ) );
#endif
}

BOOL UINoesis::OnMouseClick( CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
#ifdef _NOESIS_GUI
    if ( !IsOpen() )
        return FALSE;

    if ( pView == nullptr )
        return FALSE;

    Point2D * psPosition = pcMouse->GetPosition();

    int iRenderX = GetX() + iX + iSourceX;
    int iRenderY = GetY() + iY + iSourceY;
    int iRenderWidth = low( GetWidth(), iWidth );
    int iRenderHeight = low( GetHeight(), iHeight );

    //Inside X?
    if ( (psPosition->iX >= iRenderX) && (psPosition->iX <= (iRenderX + iRenderWidth - iSourceX)) )
    {
        //Inside Y?
        if ( (psPosition->iY >= iRenderY) && (psPosition->iY <= (iRenderY + iRenderHeight - iSourceY)) )
        {
            Noesis::MouseButton eBtn = (pcMouse->GetEvent() == EMouseEvent::ClickDownL) || (pcMouse->GetEvent() == EMouseEvent::ClickDownL) ?
                Noesis::MouseButton::MouseButton_Left : Noesis::MouseButton::MouseButton_Right;

            bool bDown = (pcMouse->GetEvent() == EMouseEvent::ClickDownL) || (pcMouse->GetEvent() == EMouseEvent::ClickDownR);

            if ( bDown )
                return pView->MouseButtonDown( iRenderX, iRenderY, eBtn ) ? TRUE : FALSE;
            else
                return pView->MouseButtonUp( iRenderX, iRenderY, eBtn ) ? TRUE : FALSE;
        }
    }
#endif
    return FALSE;
}

void UINoesis::OnMouseMove( CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
#ifdef _NOESIS_GUI
    if ( pView == nullptr )
        return;

    if ( !IsOpen() )
        return;

    Point2D * psPosition = pcMouse->GetPosition();

    int iRenderX = GetX() + iX + iSourceX;
    int iRenderY = GetY() + iY + iSourceY;
    int iRenderWidth = low( GetWidth(), iWidth );
    int iRenderHeight = low( GetHeight(), iHeight );

    //Inside X?
    if ( (psPosition->iX >= iRenderX) && (psPosition->iX <= (iRenderX + iRenderWidth - iSourceX)) )
    {
        //Inside Y?
        if ( (psPosition->iY >= iRenderY) && (psPosition->iY <= (iRenderY + iRenderHeight - iSourceY)) )
            pView->MouseMove( pcMouse->GetPosition()->iX, pcMouse->GetPosition()->iY );
    }
#endif
}

void UINoesis::OnLostDevice()
{
#ifdef _NOESIS_GUI
    if ( pNoesisRenderDevice )
        pNoesisRenderDevice->handleDeviceLost();
#endif
}

void UINoesis::OnResetDevice()
{
#ifdef _NOESIS_GUI
    if ( pNoesisRenderDevice )
        pNoesisRenderDevice->handleDeviceReset();
#endif
}

};