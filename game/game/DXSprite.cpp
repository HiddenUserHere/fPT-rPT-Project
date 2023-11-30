#include "stdafx.h"
#include "DXSprite.h"

namespace DX
{

Sprite::Sprite( LPD3DXSPRITE lpSprite, BOOL bSharable ) : lpSprite(lpSprite), bSharable(bSharable)
{
	ADDREF( lpSprite );

	uUseCount = 0;
	
	D3DXCreateMatrixStack( 0, &lpTransformMatrixStack );

	PushTransform( X3D::Matrix4() );

	Reset();
}

Sprite::Sprite( const Sprite & c ) : lpSprite(c.lpSprite), bSharable(c.bSharable)
{
	ADDREF( lpSprite );

	uUseCount = 0;
	
	D3DXCreateMatrixStack( 0, &lpTransformMatrixStack );
	
	PushTransform( X3D::Matrix4() );

	Reset();
}

Sprite::~Sprite()
{
	RELEASE( lpTransformMatrixStack );
	RELEASE( lpSprite );
}

void Sprite::OnLostDevice()
{
	if( lpSprite )
		lpSprite->OnLostDevice();
}

void Sprite::OnResetDevice()
{
	if( lpSprite )
		lpSprite->OnResetDevice();
}

void Sprite::Reset()
{
	bStencilEnable = FALSE;
}

BOOL Sprite::Begin( BOOL bAlphaBlending )
{
	if( !lpSprite )
		return FALSE;

	BOOL bRet = TRUE;

	if( uUseCount == 0 )
	{
		bRet = SUCCEEDED( lpSprite->Begin( bAlphaBlending ? D3DXSPRITE_ALPHABLEND : 0 ) );

		if( bRet )
			uUseCount++;

		if( bStencilEnable )
			GetGraphicsDevice()->SetRenderState( RENDERSTATE_StencilEnable, TRUE );
	}
	else
		uUseCount++;

	return bRet;
}

void Sprite::Draw( Texture_ptr pTexture, const Color & cColor )
{
	if( !lpSprite )
		return;

	if( uUseCount > 0 )
	{
		lpSprite->Draw( pTexture->Get(), NULL, NULL, NULL, cColor.Get() );
		lpSprite->Flush();
	}
}

void Sprite::Draw( Texture_ptr pTexture, const Color & cColor, int iWidth, int iHeight )
{
	if( !lpSprite )
		return;

	if( uUseCount > 0 )
	{
		RECT rSourceRect;
		rSourceRect.left	= 0;
		rSourceRect.top		= 0;
		rSourceRect.right	= iWidth;
		rSourceRect.bottom	= iHeight;

		//Scale Vetor
		D3DXVECTOR2 scale( 1.0f, 1.0f );
		D3DXVECTOR2 trans( (float)0, (float)0 );
		D3DXVECTOR2 center( (float)((iWidth) / 2), (float)((iHeight) / 2) );
		D3DXMATRIX matrix;
		D3DXMatrixTransformation2D( &matrix, NULL, 0, &scale, &center, 0.0f, &trans );
		lpSprite->SetTransform( &matrix );
		lpSprite->Draw( pTexture->Get(), &rSourceRect, NULL, NULL, cColor.Get() );
		lpSprite->Flush();
	}
}

void Sprite::Draw( Texture_ptr pTexture, const Color & cColor, const RECT & rSourceRect, int iX, int iY )
{
	if( !lpSprite )
		return;

	if( uUseCount > 0 )
	{
		lpSprite->Draw( pTexture->Get(), &rSourceRect, NULL, &D3DXVECTOR3( (float)iX, (float)iY, 0.0f ), cColor.Get() );
		lpSprite->Flush();
	}
}

void Sprite::Draw( RenderTarget_ptr pRenderTarget )
{
	if( !lpSprite )
		return;

	if( uUseCount > 0 )
	{
		BOOL bPopTransform = FALSE;

		if( (pRenderTarget->GetWidth() != GetGraphicsDevice()->GetRenderTargetWidth()) || (pRenderTarget->GetHeight() != GetGraphicsDevice()->GetRenderTargetHeight()) )
		{
			PushScaling2DTransform( X3D::Vector2( (float)GetGraphicsDevice()->GetRenderTargetWidth() / (float)pRenderTarget->GetWidth(), (float)GetGraphicsDevice()->GetRenderTargetHeight() / (float)pRenderTarget->GetHeight() ) );

			bPopTransform = TRUE;
		}

		//Scale Vetor
		D3DXVECTOR2 scale( 1.0f, 1.0f );
		D3DXVECTOR2 trans( (float)0, (float)0 );
		D3DXVECTOR2 center( (float)((pRenderTarget->GetWidth()) / 2), (float)((pRenderTarget->GetHeight()) / 2) );
		D3DXMATRIX matrix;
		D3DXMatrixTransformation2D( &matrix, NULL, 0, &scale, &center, 0.0f, &trans );
		lpSprite->SetTransform( &matrix );

		lpSprite->Draw( pRenderTarget->GetTexture(), NULL, NULL, NULL, D3DCOLOR_XRGB( 255, 255, 255 ) );
		lpSprite->Flush();

		if( bPopTransform )
			PopTransform();
	}
}

void Sprite::Flush()
{
	if( lpSprite )
		return;

	if( uUseCount > 0 )
		lpSprite->Flush();
}

void Sprite::End()
{
	if( !lpSprite )
		return;

	if( uUseCount > 0 )
		uUseCount--;

	if( uUseCount == 0 )
		lpSprite->End();
	else
		lpSprite->Flush();
}

void Sprite::PushScaling2DTransform( const X3D::Vector2 & vScaling, const X3D::Vector2 & vScalingCenter )
{
	D3DXVECTOR2 vXScaling;
	vXScaling.x = vScaling.x;
	vXScaling.y = vScaling.y;
	
	D3DXVECTOR2 vXScalingCenter;
	vXScalingCenter.x = vScalingCenter.x;
	vXScalingCenter.y = vScalingCenter.y;
	
	D3DXMATRIX mMatrix;
	D3DXMatrixTransformation2D( &mMatrix, &vXScalingCenter, 0.0f, &vXScaling, NULL, 0.0f, NULL );

	PushTransform( D3DXMatrixToX3DMatrix( mMatrix ) );
}

void Sprite::PushRotation2DTransform( float fRotation, const X3D::Vector2 & vRotationCenter )
{
	D3DXVECTOR2 vXRotationCenter;
	vXRotationCenter.x = vRotationCenter.x;
	vXRotationCenter.y = vRotationCenter.y;
	
	D3DXMATRIX mMatrix;
	D3DXMatrixTransformation2D( &mMatrix, NULL, 0.0f, NULL, &vXRotationCenter, fRotation, NULL );

	PushTransform( D3DXMatrixToX3DMatrix( mMatrix ) );
}

void Sprite::PushTranslation2DTransform( const X3D::Vector2 & vTranslation )
{
	D3DXVECTOR2 vXTranslation;
	vXTranslation.x = vTranslation.x;
	vXTranslation.y = vTranslation.y;
	
	D3DXMATRIX mMatrix;
	D3DXMatrixTransformation2D( &mMatrix, NULL, 0.0f, NULL, NULL, 0.0f, &vXTranslation );

	PushTransform( D3DXMatrixToX3DMatrix( mMatrix ) );
}

void Sprite::PushTransform( const X3D::Matrix4 & mMatrix )
{
	lpTransformMatrixStack->Push();
	lpTransformMatrixStack->MultMatrix( &X3DMatrixToD3DXMatrix( mMatrix ) );

	if( lpSprite )
		lpSprite->SetTransform( lpTransformMatrixStack->GetTop() );
}

void Sprite::PopTransform( int iPopCount )
{
	for( int i = 0; i < iPopCount; i++ )
		lpTransformMatrixStack->Pop();

	if( lpSprite )
		lpSprite->SetTransform( lpTransformMatrixStack->GetTop() );
}

}