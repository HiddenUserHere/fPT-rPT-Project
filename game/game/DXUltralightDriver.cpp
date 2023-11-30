#include "stdafx.h"
#include "DXUltralightDriver.h"

#ifdef _ULTRALIGHT_WEB
namespace ultralight
{

DXUltralightDriver::DXUltralightDriver()
{
}

DXUltralightDriver::~DXUltralightDriver()
{
}

void DXUltralightDriver::CreateTexture( uint32_t texture_id, Ref<Bitmap> bitmap )
{
    auto i = textures_.find( texture_id );
    if ( i != textures_.end() )
    {
        MessageBoxW( nullptr,
                     L"GPUDriverD3D11::CreateTexture, texture id already exists.", L"Error", MB_OK );
        return;
    }

    if ( !(bitmap->format() == kBitmapFormat_BGRA8_UNORM_SRGB || bitmap->format() == kBitmapFormat_A8_UNORM) )
    {
        MessageBoxW( nullptr,
                     L"GPUDriverD3D11::CreateTexture, unsupported format.", L"Error", MB_OK );
    }


    auto & texture_entry = textures_[texture_id];
    HRESULT hr;

    if ( bitmap->IsEmpty() )
    {
        LPDIRECT3DTEXTURE9 lpTexture;
        HRESULT hr;
        D3DFORMAT d3dFormat = GetGraphicsDevice()->GetInfo().d3dBackBufferFormat == D3DFMT_X4R4G4B4 ? D3DFMT_A4R4G4B4 : D3DFMT_A8R8G8B8;
        if ( FAILED( hr = DX::Graphic::GetDevice()->CreateTexture( bitmap->width(), bitmap->height(), 1, 0, d3dFormat,
                                                                   DXGRAPHICENGINE_D3DPOOL, &lpTexture, NULL ) ) )
        {
            WRITEDBG( "CreateTexture Browser [0x%08X]", hr );
        }

        texture_entry.texture = GetGraphicsDevice()->GetTextureFactory()->MakeTemporaryTexture( lpTexture );
    }
    else
    {
        LPDIRECT3DTEXTURE9 lpTexture;
        HRESULT hr;
        D3DFORMAT d3dFormat = GetGraphicsDevice()->GetInfo().d3dBackBufferFormat == D3DFMT_X4R4G4B4 ? D3DFMT_A4R4G4B4 : D3DFMT_A8R8G8B8;
        if ( FAILED( hr = DX::Graphic::GetDevice()->CreateTexture( bitmap->width(), bitmap->height(), 1, 0, d3dFormat,
                                                                   DXGRAPHICENGINE_D3DPOOL, &lpTexture, NULL ) ) )
        {
            WRITEDBG( "CreateTexture Browser [0x%08X]", hr );
        }

        texture_entry.texture = GetGraphicsDevice()->GetTextureFactory()->MakeTemporaryTexture( lpTexture );

        DXUltralightCopyBitmapToTexture( lpTexture, bitmap );
    }



}

void DXUltralightDriver::UpdateTexture( uint32_t texture_id, Ref<Bitmap> bitmap )
{
    auto i = textures_.find( texture_id );
    if ( i == textures_.end() )
    {
        MessageBoxW( nullptr,
                     L"GPUDriverD3D11::UpdateTexture, texture id doesn't exist.", L"Error", MB_OK );
        return;
    }

    auto & entry = i->second;

    auto lpTexture = entry.texture->Get();
    
    DXUltralightCopyBitmapToTexture( lpTexture, bitmap );
}

void DXUltralightDriver::DestroyTexture( uint32_t texture_id )
{
    auto i = textures_.find( texture_id );
    if ( i != textures_.end() )
    {
        textures_.erase( i );
    }
}

void DXUltralightDriver::CreateRenderBuffer( uint32_t render_buffer_id, const RenderBuffer & buffer )
{
    
    if ( render_buffer_id == 0 )
    {
        MessageBoxW( nullptr,
                     L"GPUDriverD3D11::CreateRenderBuffer, render buffer ID 0 is reserved for default render target view.", L"Error", MB_OK );
        return;
    }

    auto i = render_targets_.find( render_buffer_id );
    if ( i != render_targets_.end() )
    {
        MessageBoxW( nullptr,
                     L"GPUDriverD3D11::CreateRenderBuffer, render buffer id already exists.", L"Error", MB_OK );
        return;
    }

    auto tex_entry = textures_.find( buffer.texture_id );
    if ( tex_entry == textures_.end() )
    {
        MessageBoxW( nullptr,
                     L"GPUDriverD3D11::CreateRenderBuffer, texture id doesn't exist.", L"Error", MB_OK );
        return;
    }


    auto & render_target_entry = render_targets_[render_buffer_id];
    
    auto pRenderTarget = GetGraphicsDevice()->MakeRenderTarget( 7000 + render_buffer_id );

    render_target_entry.render_target_texture_id = buffer.texture_id;

    render_target_entry.render_target_view = pRenderTarget;
}

void DXUltralightDriver::DestroyRenderBuffer( uint32_t render_buffer_id )
{
}

void DXUltralightDriver::CreateGeometry( uint32_t geometry_id, const VertexBuffer & vertices, const IndexBuffer & indices )
{
    if ( geometry_.find( geometry_id ) != geometry_.end() )
        return;

    GeometryEntry geometry;
    geometry.format = vertices.format;
    
    auto pVertexBuffer = GetGraphicsDevice()->CreateVertexBuffer( vertices.size, 1, TRUE );
    auto pData = pVertexBuffer->Lock();
    CopyMemory( pData, vertices.data, vertices.size );
    pVertexBuffer->Unlock();

    auto pIndexBuffer = GetGraphicsDevice()->CreateIndexBuffer( indices.size, 1 );
    auto pDataIdx = pIndexBuffer->Lock();
    CopyMemory( pDataIdx, indices.data, indices.size );
    pIndexBuffer->Unlock();

    geometry.indexBuffer = pIndexBuffer;
    geometry.vertexBuffer = pVertexBuffer;

    geometry_.insert( {geometry_id, std::move( geometry )} );
}

void DXUltralightDriver::UpdateGeometry( uint32_t geometry_id, const VertexBuffer & vertices, const IndexBuffer & indices )
{
    auto i = geometry_.find( geometry_id );
    if ( i == geometry_.end() )
    {
        MessageBoxW( nullptr,
                     L"GPUDriverD3D11::UpdateGeometry, geometry id doesn't exist.", L"Error", MB_OK );
        return;
    }

    auto & entry = i->second;

    auto pData = entry.vertexBuffer->Lock();
    CopyMemory( pData, vertices.data, vertices.size );
    entry.vertexBuffer->Unlock();

    auto pDataIdx = entry.indexBuffer->Lock();
    CopyMemory( pDataIdx, indices.data, indices.size );
    entry.indexBuffer->Unlock();
}

void DXUltralightDriver::DestroyGeometry( uint32_t geometry_id )
{
}

void DXUltralightDriver::BindTexture( uint8_t texture_unit, uint32_t texture_id )
{
}

void DXUltralightDriver::BindRenderBuffer( uint32_t render_buffer_id )
{
}

void DXUltralightDriver::ClearRenderBuffer( uint32_t render_buffer_id )
{
}

void DXUltralightDriver::DrawGeometry( uint32_t geometry_id, uint32_t indices_count, uint32_t indices_offset, const GPUState & state )
{
}

};

#endif