#include "stdafx.h"
#include "DXUltralightContext.h"


namespace ultralight
{

DXUltralightContext::DXUltralightContext()
{
}

DXUltralightContext::~DXUltralightContext()
{
}

void DXUltralightContext::BeginDrawing()
{
}

void DXUltralightContext::EndDrawing()
{
}

void DXUltralightContext::PresentFrame()
{
}

void DXUltralightContext::Resize( int iWidth, int iHeight )
{
}

LPDIRECT3DDEVICE9 DXUltralightContext::device()
{
    return LPDIRECT3DDEVICE9();
}

DX::RenderTarget_ptr DXUltralightContext::render_target_view()
{
    return DX::RenderTarget_ptr();
}

void DXUltralightContext::EnableBlend()
{
}

void DXUltralightContext::DisableBlend()
{
}

void DXUltralightContext::EnableScissor()
{
}

void DXUltralightContext::DisableScissor()
{
}

void DXUltralightContext::set_scale( double scale )
{
    scale_ = scale;
}

double DXUltralightContext::scale() const
{
    return scale_;
}

void DXUltralightContext::set_screen_size( uint32_t width, uint32_t height )
{
}

void DXUltralightContext::screen_size( uint32_t & width, uint32_t & height )
{
}

UINT DXUltralightContext::back_buffer_width()
{
    return 0;
}

UINT DXUltralightContext::back_buffer_height()
{
    return 0;
}

};

