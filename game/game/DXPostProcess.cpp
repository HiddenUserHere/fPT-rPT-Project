#include "stdafx.h"
#include "DXPostProcess.h"

BOOL DXPostProcess::FakeHDR = FALSE;

DXPostProcess::DXPostProcess()
{
	bPostProcess = FALSE;
	bPostProcessInitialized = FALSE;
	lpRenderEffect = NULL;
	lpRenderSprite = NULL;
	lpRenderTexture = NULL;
	lpRenderSurfaceTexture = NULL;
	lpRenderSurface = NULL;
	lpBackBuffer = NULL;
	hr = NULL;
}

DXPostProcess::~DXPostProcess()
{
}

void DXPostProcess::PostProcessON( EPostEffects eTechnique )
{
	bPostProcess = TRUE;

	char szTechnique[ 32 ] = { 0 };

	if ( lpRenderEffect )
	{
		switch ( eTechnique )
		{
			case POSTPROCESSID_Dead:
				STRINGCOPY( szTechnique, "Dead" );
				break;
			case POSTPROCESSID_Poison:
				STRINGCOPY( szTechnique, "Poison" );
				break;
			case POSTPROCESSID_BattleRoyale:
				STRINGCOPY( szTechnique, "BattleRoyaleGas" );
				break;
			case POSTPROCESSID_FakeHDR:
				STRINGCOPY( szTechnique, "FakeHDR" );
				break;
			default:
				break;
		}
		if ( lstrlenA( szTechnique ) > 0 )
			lpRenderEffect->SetTechnique( szTechnique );
	}
}

void DXPostProcess::PostProcessOFF()
{
	fDeadIntensity = 0.1f;
	bPostProcess = FALSE;
}

void DXPostProcess::SetPostProcessFloat( const char * pszVariable, const float fValue )
{
	if ( bPostProcess && bPostProcessInitialized )
		if ( lpRenderEffect )
			lpRenderEffect->SetFloat( pszVariable, fValue );
}

void DXPostProcess::PostProcess3D()
{
	if ( bPostProcess && bPostProcessInitialized )
	{
		if ( lpRenderEffect && lpRenderSprite )
		{
			//Set Dynamic Shader variables
			lpRenderEffect->SetFloat( "Time", fTime );
			lpRenderEffect->SetFloat( "DeadIntensity", fDeadIntensity );
			lpRenderEffect->SetFloat( "PixelSize", (1.0f / (float)(RESOLUTION_WIDTH  / RESOLUTION_HEIGHT)) );

			fTime += 0.4f;
			if ( fTime == 4.0f )
				fTime = 0.4f;

			fDeadIntensity += 0.007f;
			if ( fDeadIntensity >= 1.0f )
				fDeadIntensity = 1.0f;

			DX::Graphic::GetDevice()->StretchRect( lpRenderSurface, NULL, lpRenderSurfaceTexture, NULL, D3DTEXF_NONE );

			//Begin Post Process Shader Pass
			UINT uPasses = 0;
			lpRenderEffect->Begin( &uPasses, D3DXFX_DONOTSAVESAMPLERSTATE );
			lpRenderSprite->Begin( 0 );
			lpRenderEffect->BeginPass( 0 );
			lpRenderSprite->Draw( lpRenderTexture, NULL, NULL, NULL, D3DCOLOR_XRGB( 255, 255, 255 ) );
			lpRenderEffect->EndPass();
			lpRenderSprite->End();
			lpRenderEffect->End();
		}
	}
}


BOOL DXPostProcess::CreatePostProcessBuffers()
{
	bPostProcessInitialized = FALSE;

	RELEASE( lpRenderTexture );
	RELEASE( lpRenderSurface );
	RELEASE( lpRenderSurfaceTexture );

	if ( FAILED( hr = DX::Graphic::GetDevice()->CreateRenderTarget( GRAPHICS->presentParams.BackBufferWidth, GRAPHICS->presentParams.BackBufferHeight,
		 GRAPHICS->presentParams.BackBufferFormat == D3DFMT_X4R4G4B4 ? D3DFMT_A4R4G4B4 : D3DFMT_A8R8G8B8,
		 GRAPHICS->presentParams.MultiSampleType, GRAPHICS->presentParams.MultiSampleQuality, FALSE, &lpRenderSurface, NULL ) ) )
	{
		GRAPHICENGINE->Log( "Could not create Render Target" );
		return TRUE;
	}

	//Render Target
	if ( FAILED( hr = DX::Graphic::GetDevice()->CreateTexture( GRAPHICS->presentParams.BackBufferWidth, GRAPHICS->presentParams.BackBufferHeight, 1, D3DUSAGE_RENDERTARGET, GRAPHICS->presentParams.BackBufferFormat == D3DFMT_X4R4G4B4 ? D3DFMT_A4R4G4B4 : D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &lpRenderTexture, NULL ) ) )
	{
		GRAPHICENGINE->Log( "Could not create Render Target" );
		return TRUE;
	}

	//Render Surface
	if ( FAILED( hr = lpRenderTexture->GetSurfaceLevel( 0, &lpRenderSurfaceTexture ) ) )
	{
		GRAPHICENGINE->Log( "Could not get Render Target Surface" );
		return TRUE;
	}

	bPostProcessInitialized = TRUE;

	return TRUE;
}

BOOL DXPostProcess::CreatePostProcess()
{
	LPD3DXBUFFER lpD3DErrorBuffer = NULL;

	//Post Process Effect
	RELEASE( lpRenderEffect );
	if ( FAILED( hr = D3DXCreateEffectFromFile( DX::Graphic::GetDevice(), "game\\scripts\\shaders\\PostProcess.fx", NULL, NULL, 0, NULL, &lpRenderEffect, &lpD3DErrorBuffer ) ) )
	{
		GRAPHICENGINE->Log( "Could not create Post Process Effect:\n%s", lpD3DErrorBuffer ? lpD3DErrorBuffer->GetBufferPointer() : "NULL" );

		if ( lpD3DErrorBuffer )
			RELEASE( lpD3DErrorBuffer );

		return TRUE;
	}

	if ( lpD3DErrorBuffer )
		RELEASE( lpD3DErrorBuffer );

	//Create Post Process Buffers
	if ( !CreatePostProcessBuffers() )
	{
		GRAPHICENGINE->Log( "Could not create Post Process buffers" );
		return TRUE;
	}

	bPostProcessInitialized = FALSE;

	RELEASE( lpRenderSprite );
	if ( FAILED( hr = D3DXCreateSprite( DX::Graphic::GetDevice(), &lpRenderSprite ) ) )
	{
		GRAPHICENGINE->Log( "Could not create Sprite" );
		return TRUE;
	}

	bPostProcessInitialized = TRUE;

	GRAPHICENGINE->Log( "Post Process Initialized Successfully" );
	return TRUE;
}

void DXPostProcess::OnBegin3D()
{
	if( bPostProcess && bPostProcessInitialized )
	{
		if( lpRenderSurface )
		{
			//Save Back Buffers
			DX::Graphic::GetDevice()->GetRenderTarget( 0, &lpBackBuffer );

			//Set Render Buffers
			DX::Graphic::GetDevice()->SetRenderTarget( 0, lpRenderSurface );

			// Clear
			DX::Graphic::GetDevice()->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_ARGB( 0, 0, 0, 0 ), 1.0f, 0 );

			GRAPHICENGINE->StateSetup();
		}
	}
}

void DXPostProcess::OnEnd3D()
{
	if ( bPostProcess && bPostProcessInitialized )
	{
		if ( lpBackBuffer )
		{
			//Reset Back Buffer
			DX::Graphic::GetDevice()->SetRenderTarget( 0, lpBackBuffer );
			RELEASE( lpBackBuffer );
		}
	}

	if ( UNITDATA && UNITDATA->psModelAnimation && UNITDATA->psModelAnimation->iType == ANIMATIONTYPE_Die )
	{
		PostProcessON( POSTPROCESSID_Dead );
	}
	else if ( BATTLEROYALEHANDLER->InBattle() && (BATTLEROYALEHANDLER->IsInsideRadius( UNIT->GetPosition() ) == FALSE) )
	{
		PostProcessON( POSTPROCESSID_BattleRoyale );
	}
	else
	{
		if ( DXPostProcess::FakeHDR )
			PostProcessON( POSTPROCESSID_FakeHDR );
		else
			PostProcessOFF();
	}
	PostProcess3D();
}

void DXPostProcess::OnResetDevice()
{
	CreatePostProcessBuffers();

	//Objects
	if( lpRenderSprite )
		lpRenderSprite->OnResetDevice();
	if( lpRenderEffect )
		lpRenderEffect->OnResetDevice();
}

void DXPostProcess::OnLostDevice()
{
	bPostProcessInitialized = FALSE;

	RELEASE( lpRenderTexture );
	RELEASE( lpRenderSurfaceTexture );
	RELEASE( lpRenderSurface );

	//Objects
	if( lpRenderSprite )
		lpRenderSprite->OnLostDevice();
	if( lpRenderEffect )
		lpRenderEffect->OnLostDevice();

}

BOOL DXPostProcess::Init()
{
	if ( !CreatePostProcess() )
	{
	//	GRAPHICENGINE->Log( "Could not create Post Process" );
		return FALSE;
	}
	return TRUE;
}