#pragma once

#ifdef _ULTRALIGHT_WEB
#include "DXUltralightContext.h"


static void DXUltralightCopyBitmapToTexture( LPDIRECT3DTEXTURE9 lpTexture, ultralight::RefPtr<ultralight::Bitmap> bitmap )
{
    bitmap->LockPixels();
    void * pData = bitmap->raw_pixels();

    D3DLOCKED_RECT d3dRect;
    RECT rc;
    rc.left = 0;
    rc.top = 0;
    rc.right = bitmap->width();
    rc.bottom = bitmap->height();

    HRESULT hr = lpTexture->LockRect( 0, &d3dRect, &rc, D3DLOCK_DISCARD );

    BYTE * pbaDest = (BYTE *)d3dRect.pBits;
    BYTE * pbaSource = (BYTE *)pData;

    auto rows = low( bitmap->row_bytes(), d3dRect.Pitch );
    auto height = bitmap->height();

    for ( unsigned int y = 0; y < height; ++y )
    {
        memcpy( pbaDest + d3dRect.Pitch * y,
                pbaSource + bitmap->row_bytes() * y,
                rows );
    }

    lpTexture->UnlockRect( 0 );


    bitmap->UnlockPixels();
}

namespace ultralight
{

class DXUltralightDriver : public GPUDriverImpl
{
public:
    DXUltralightDriver();

    virtual ~DXUltralightDriver();

    virtual void CreateTexture( uint32_t texture_id,
                                Ref<Bitmap> bitmap ) override;

    virtual void UpdateTexture( uint32_t texture_id,
                                Ref<Bitmap> bitmap ) override;

    virtual void DestroyTexture( uint32_t texture_id ) override;

    virtual void CreateRenderBuffer( uint32_t render_buffer_id,
                                     const RenderBuffer & buffer ) override;

    virtual void DestroyRenderBuffer( uint32_t render_buffer_id ) override;

    virtual void CreateGeometry( uint32_t geometry_id,
                                 const VertexBuffer & vertices,
                                 const IndexBuffer & indices ) override;

    virtual void UpdateGeometry( uint32_t geometry_id,
                                 const VertexBuffer & vertices,
                                 const IndexBuffer & indices ) override;

    virtual void DestroyGeometry( uint32_t geometry_id ) override;

    // Inherited from GPUDriverImpl

    virtual const char * name() override { return "Direct3D 9"; }

    virtual void BeginDrawing() override {}

    virtual void EndDrawing() override {}

    virtual void BindTexture( uint8_t texture_unit,
                              uint32_t texture_id ) override;

    virtual void BindRenderBuffer( uint32_t render_buffer_id ) override;

    virtual void ClearRenderBuffer( uint32_t render_buffer_id ) override;

    virtual void DrawGeometry( uint32_t geometry_id,
                               uint32_t indices_count,
                               uint32_t indices_offset,
                               const GPUState & state ) override;

protected:
    void LoadShaders();
    void BindShader( uint8_t shader );
    void BindVertexLayout( VertexBufferFormat format );
    void BindGeometry( uint32_t id );
    void SetViewport( uint32_t width, uint32_t height );
    void UpdateConstantBuffer( const GPUState & state );
    Matrix ApplyProjection( const Matrix4x4 & transform, float screen_width, float screen_height );

    DXUltralightContext * context_;


    struct GeometryEntry { VertexBufferFormat format; DX::VertexBuffer_ptr vertexBuffer; DX::IndexBuffer_ptr indexBuffer; };
    typedef std::map<uint32_t, GeometryEntry> GeometryMap;
    GeometryMap geometry_;

    struct TextureEntry
    {
        DX::Texture_ptr texture;

        // These members are only used when MSAA is enabled
        bool is_msaa_render_target = false;
        bool needs_resolve = false;
        DX::Texture_ptr resolve_texture;
    };

    typedef std::map<uint32_t, TextureEntry> TextureMap;
    TextureMap textures_;

    struct RenderTargetEntry
    {
        DX::RenderTarget_ptr render_target_view;
        uint32_t render_target_texture_id;
    };

    typedef std::map<uint32_t, RenderTargetEntry> RenderTargetMap;
    RenderTargetMap render_targets_;

    //typedef std::map<ShaderType, std::pair<ComPtr<ID3D11VertexShader>, ComPtr<ID3D11PixelShader>>> ShaderMap;
    //ShaderMap shaders_;
};

};
#endif