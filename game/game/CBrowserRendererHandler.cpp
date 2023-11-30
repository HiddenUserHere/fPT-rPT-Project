#include "stdafx.h"
#include "CBrowserRendererHandler.h"

#ifdef CEF_UI_BROWSER

CBrowserRendererHandler::CBrowserRendererHandler( int _iWidth, int _iHeight )
{
	iWidth = _iWidth;
	iHeight = _iHeight;
}


CBrowserRendererHandler::~CBrowserRendererHandler()
{
}


void CBrowserRendererHandler::Init()
{
	LPDIRECT3DTEXTURE9 lpTexture;
	HRESULT hr;
	if ( FAILED( hr = DX::Graphic::GetDevice()->CreateTexture( iWidth, iHeight, 1, 0, GRAPHICENGINE->Get3DPP().BackBufferFormat == D3DFMT_X4R4G4B4 ? D3DFMT_A4R4G4B4 : D3DFMT_A8R8G8B8, 
		DXGRAPHICENGINE_D3DPOOL, &lpTexture, NULL ) ) )
	{
		WRITEDBG( "CreateTexture Browser [0x%08X]", hr );
	}

	pTerxture = GetGraphicsDevice()->GetTextureFactory()->MakeTemporaryTexture( lpTexture );
	pSprite = GRAPHICENGINE->GetRenderer()->GetGraphicsDevice()->GetSpriteFactory()->MakeSprite( TRUE );
	pRenderTarget = GetGraphicsDevice()->MakeRenderTarget( 200010, iWidth, iHeight );
}

void CBrowserRendererHandler::Shutdown()
{
	pRenderTarget = nullptr;
	pTerxture = nullptr;
	pSprite = nullptr;
}

bool CBrowserRendererHandler::GetViewRect( CefRefPtr<CefBrowser> browser, CefRect & rect )
{
	rect = CefRect( 0, 0, iWidth, iHeight );
	return true;
}

void CBrowserRendererHandler::OnAfterCreated( CefRefPtr<CefBrowser> browser )
{
	browser->GetHost()->WasResized();
}

void CBrowserRendererHandler::OnPaint( CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList & dirtyRects, const void * buffer, int width, int height )
{
	D3DLOCKED_RECT d3dRect;
	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = width;
	rc.bottom = height;
	
	HRESULT hr = pTerxture->Get()->LockRect( 0, &d3dRect, &rc, D3DLOCK_DISCARD );
	CopyMemory( d3dRect.pBits, buffer, width * height * 4 );
	
	pTerxture->Get()->UnlockRect( 0 );
}

void CBrowserRendererHandler::Render()
{
	CefDoMessageLoopWork();

	//Draw Blank Texture to Render Target 1 and only color the parts marked in the low resolution Stencil Buffer
	if ( GetGraphicsDevice()->SetRenderTarget( pRenderTarget ) )
	{
		//Clear Render Target
		GetGraphicsDevice()->Clear( TRUE, FALSE, FALSE );

		if ( pSprite->Begin( TRUE ) )
		{
			pSprite->Draw( pTerxture, DX::Color( 255, 255, 255, 255 ), pTerxture->GetWidth(), pTerxture->GetHeight() );

			pSprite->End();
		}

		GetGraphicsDevice()->UnsetRenderTarget();

		if ( pSprite->Begin( TRUE ) )
		{
			pSprite->Draw( pRenderTarget );

			pSprite->End();
		}
	}
}

void CBrowserRendererHandler::Frame()
{
	CefDoMessageLoopWork();
}

#endif
