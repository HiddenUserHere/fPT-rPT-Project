#pragma once


namespace UI
{
typedef std::shared_ptr<class UINoesis> Noesis_ptr;
class UINoesis : public Element
{
public:
    UINoesis( Rectangle2D rRectangle );
    virtual ~UINoesis();

    void                        LoadFromFile( const std::string _strFile );

    void						Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );
    void						Update( float fTime );

    //Event Handlers
    BOOL						OnMouseClick( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );
    void						OnMouseMove( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );

    static void                 OnLostDevice();
    static void                 OnResetDevice();

private:
    std::string                 strFile;

#ifdef _NOESIS_GUI
    Noesis::Ptr<Noesis::IView>  pView;
#endif
    double                      fTimeNoesis = 0.0;

    DX::RenderTarget_ptr        pRenderTarget = nullptr;
    DX::Sprite_ptr              pSprite = nullptr;
};
};
