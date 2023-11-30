#include "stdafx.h"
#include "DXSpriteFactory.h"

#include "DXGraphicsDevice.h"

namespace DX
{

SpriteFactory::SpriteFactory( GraphicsDevice * pGraphicsDevice ) : pGraphicsDevice(pGraphicsDevice)
{
	vCache.reserve( 10 );
}

void SpriteFactory::OnLostDevice()
{
	for( auto pSprite : vCache )
		pSprite->OnLostDevice();
}

void SpriteFactory::OnResetDevice()
{
	for( auto pSprite : vCache )
		pSprite->OnResetDevice();
}

Sprite_ptr SpriteFactory::MakeSprite( BOOL bSharable )
{
	//Sharable?
	if( bSharable )
	{
		//Find one in Cache that is also Sharable..
		for( auto pSprite : vCache )
		{
			//Also Sharable Sprite?
			if( pSprite->IsSharable() )
				return pSprite;
		}
	}

	//Load Sprite
	LPD3DXSPRITE lpSprite;
	if( FAILED( D3DXCreateSprite( pGraphicsDevice->Get(), &lpSprite ) ) )
		return nullptr;

	//New Sprite
	auto pSprite = std::make_shared<Sprite>( lpSprite, bSharable );

	//Remember Sprite
	vCache.push_back( pSprite );

	//Release Sprite (New Sprite took possession of a Reference)
	RELEASE( lpSprite );

	return pSprite;
}

}