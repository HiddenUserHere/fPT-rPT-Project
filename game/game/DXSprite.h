#pragma once

#include "DXTexture.h"
#include "DXRenderTarget.h"
#include "DXColor.h"

namespace DX
{

typedef std::shared_ptr<class Sprite> Sprite_ptr;

class Sprite
{
public:
							  Sprite( LPD3DXSPRITE lpSprite, BOOL bSharable );
							  Sprite( const Sprite & c );
	virtual					 ~Sprite();

	LPD3DXSPRITE			  Get() { return lpSprite; }

	void					  OnLostDevice();
	void					  OnResetDevice();
	
	void					  Reset();

	BOOL					  Begin( BOOL bAlphaBlending );
	
	void					  Draw( Texture_ptr pTexture, const Color & cColor );
	void					  Draw( Texture_ptr pTexture, const Color & cColor, int iWidth, int iHeight );
	void					  Draw( Texture_ptr pTexture, const Color & cColor, const RECT & rSourceRect, int iX, int iY );
	void					  Draw( RenderTarget_ptr pRenderTarget );
	
	void					  Flush();
	void					  End();

	void					  PushScaling2DTransform( const X3D::Vector2 & vScaling, const X3D::Vector2 & vScalingCenter = X3D::Vector2( 0.0f, 0.0f ) );
	void					  PushRotation2DTransform( float fRotation, const X3D::Vector2 & vRotationCenter = X3D::Vector2( 0.0f, 0.0f ) );
	void					  PushTranslation2DTransform( const X3D::Vector2 & vTranslation );

	void					  PushTransform( const X3D::Matrix4 & mMatrix );
	void					  PopTransform( int iPopCount = 1 );

	BOOL					  IsSharable() { return bSharable; }

	void					  SetStencilEnable( BOOL bEnable ) { bStencilEnable = bEnable; }
	
protected:
	LPD3DXSPRITE			  lpSprite;
	BOOL					  bSharable;

	UINT					  uUseCount;

	BOOL					  bStencilEnable;
	
	LPD3DXMATRIXSTACK		  lpTransformMatrixStack;
};

}