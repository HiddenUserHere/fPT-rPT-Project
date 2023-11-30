#include "stdafx.h"
#include "DXEffectFactory.h"

#include "DX.h"

//#define YDEBUG

namespace DX
{

EffectFactory::EffectFactory( GraphicsDevice * pGraphicsDevice ) : pGraphicsDevice(pGraphicsDevice)
{
	vCache.reserve( 25 );
}

void EffectFactory::OnLostDevice()
{
	for( auto pEffect : vCache )
		pEffect->OnLostDevice();
}

void EffectFactory::OnResetDevice()
{
	for( auto pEffect : vCache )
		pEffect->OnResetDevice();
}

Effect_ptr EffectFactory::MakeEffect( std::string strEffectFile )
{
	//Find same Effect in Cache..
	for( auto pEffect : vCache )
	{
		//Same Effect? Share it
		if( pEffect->GetFile().compare( strEffectFile ) == 0 )
			return pEffect;
	}

	//Create Effect
	LPD3DXEFFECT lpEffect = CreateEffectFromFile( strEffectFile );

	//Failed to load?
	if( !lpEffect )
		return nullptr;

	//New Effect (new Reference)
	auto pEffect = std::make_shared<Effect>( lpEffect, strEffectFile );

	//Remember Effect
	vCache.push_back( pEffect );

	//Release our Reference
	RELEASE( lpEffect );

	//Return Effect
	return pEffect;
}

void EffectFactory::ReloadEffects()
{
	//Loop through Effects in Cache
	for( auto pEffect : vCache )
	{
		//Create Effect
		LPD3DXEFFECT lpEffect = CreateEffectFromFile( pEffect->GetFile() );

		//Renew Effect (adds Reference)
		pEffect->Renew( lpEffect );

		//Release our Reference
		RELEASE( lpEffect );
	}
}

LPD3DXEFFECT EffectFactory::CreateEffectFromFile( std::string strEffectFile )
{
	DWORD dwFlags = 0;

#ifdef YDEBUG
	dwFlags |= D3DXSHADER_DEBUG;
	dwFlags |= D3DXSHADER_SKIPOPTIMIZATION;
#endif

	//Create Effect
	HRESULT hr;
	LPD3DXEFFECT lpEffect;
	LPD3DXBUFFER lpErrorBuffer = NULL;
	if( FAILED( hr = D3DXCreateEffectFromFile( pGraphicsDevice->Get(), strEffectFile.c_str(), NULL, NULL, dwFlags, NULL, &lpEffect, &lpErrorBuffer ) ) )
	{
		//Logger::GetInstance().PrintEx( Logger::Release, hr, "Could not create Effect from File (%s):\n%s", strEffectFile.c_str(), lpErrorBuffer ? lpErrorBuffer->GetBufferPointer() : "NULL" );

		if( lpErrorBuffer )
			RELEASE( lpErrorBuffer );

		return NULL;
	}
	if( lpErrorBuffer )
		RELEASE( lpErrorBuffer );

	return lpEffect;
}

}