#pragma once


namespace ultralight
{

class DXUltralightContext
{
public:
    DXUltralightContext();

    virtual ~DXUltralightContext();

    virtual void BeginDrawing();

    virtual void EndDrawing();

    virtual void PresentFrame();

    virtual void Resize( int iWidth, int iHeight );

    virtual LPDIRECT3DDEVICE9 device();

    virtual DX::RenderTarget_ptr render_target_view();

    virtual void EnableBlend();
    virtual void DisableBlend();

    virtual void EnableScissor();
    virtual void DisableScissor();

    virtual void set_scale( double scale );
    virtual double scale() const;

    virtual void set_screen_size( uint32_t width, uint32_t height );
    virtual void screen_size( uint32_t & width, uint32_t & height );

    UINT back_buffer_width();
    UINT back_buffer_height();

private:
    double scale_;
};

};
