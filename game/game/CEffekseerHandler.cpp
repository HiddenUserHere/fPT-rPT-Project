#include "stdafx.h"
#include "CEffekseerHandler.h"


void __stdcall RemoveCallBack( Effekseer::Manager * manager, Effekseer::Handle handle, bool isRemovingManager )
{
	if ( EFFEKSEERHANDLER )
		EFFEKSEERHANDLER->KillHandle( handle );
}


Effekseer::TextureData * CEffekseerTextureLoader::Load( const EFK_CHAR * path, ::Effekseer::TextureType textureType )
{
	std::filesystem::path p = (const WCHAR *)path;

	std::string strFile = std::string( "game\\textures\\particles\\" ) + std::string( p.filename().string() );

	if ( auto pTexture = GetGraphicsDevice()->GetTextureFactory()->CreateTextureFromFile( strFile, FALSE ) )
	{
		//Get Surface Description
		D3DSURFACE_DESC sDescription;
		pTexture->GetLevelDesc( 0, &sDescription );

		auto pcTextureData = new Effekseer::TextureData();
		pcTextureData->UserPtr = pTexture;
		pcTextureData->UserID = 0;
		pcTextureData->TextureFormat = Effekseer::TextureFormatType::ABGR8;
		pcTextureData->Width = sDescription.Width;
		pcTextureData->Height = sDescription.Height;

		return pcTextureData;
	}

	return nullptr;
}

void CEffekseerTextureLoader::Unload( Effekseer::TextureData * psTextureData )
{
	if ( psTextureData )
	{
		auto pTexture = (LPDIRECT3DTEXTURE9)psTextureData->UserPtr;
		RELEASE( pTexture );
	}
}


CEffekseerHandler::CEffekseerHandler()
{
}

CEffekseerHandler::~CEffekseerHandler()
{
	//Delete Manager
	if ( pcManager )
		pcManager->Destroy();

	//Delete Renderer
	if ( pcRenderer )
		pcRenderer->Destroy();

	pRenderTargetDistortion = nullptr;

	for ( auto pcParticleSystem : vParticleSystems )
		DELET( pcParticleSystem );

	vParticleSystems.clear();

	for ( auto pcParticleScript : vParticleScriptCache )
		DELET( pcParticleScript );

	vParticleScriptCache.clear();
}

void CEffekseerHandler::Init()
{
	//Create Renderer
	pcRenderer = EffekseerRendererDX9::Renderer::Create( GetGraphicsDevice()->Get(), CEffekseerHandler::MAX_EFFEKSEER_SPRITES );

	//Create Manager
	pcManager = Effekseer::Manager::Create( CEffekseerHandler::MAX_EFFEKSEER_SPRITES );

	//Create Renderers
	pcManager->SetSpriteRenderer( pcRenderer->CreateSpriteRenderer() );
	pcManager->SetRibbonRenderer( pcRenderer->CreateRibbonRenderer() );
	pcManager->SetRingRenderer( pcRenderer->CreateRingRenderer() );
	pcManager->SetTrackRenderer( pcRenderer->CreateTrackRenderer() );
	pcManager->SetModelRenderer( pcRenderer->CreateModelRenderer() );
	
	//Create Texture Loader (Create a custom Loader since we can load from a new directory)
	pcManager->SetTextureLoader( new CEffekseerTextureLoader() );
	pcManager->SetModelLoader( pcRenderer->CreateModelLoader() );

	//Set Coordinate System
	pcManager->SetCoordinateSystem( Effekseer::CoordinateSystem::LH );

	//Set Distortion
	pRenderTargetDistortion = GetGraphicsDevice()->MakeRenderTarget( 777, 0 );

	//Set Distortion
	pcRenderer->SetDistortingCallback( new CEffekseerDistortion() );

	//Create Culling
	//pcManager->CreateCullingWorld( 1000.0f, 1000.0f, 1000.0f, 6 );

	CreateRenderTarget();
}

CEffekSeerParticleScript * CEffekseerHandler::LoadScript( const std::string strFileName )
{
	CEffekSeerParticleScript * pcRet = nullptr;

	CEffekSeerParticleScript * pcParticleScript = GetScriptFromCache( strFileName );
	if ( pcParticleScript == nullptr )
	{
		//Convert File Name to UTF-16
		char16_t szFileName[MAX_PATH] = { 0 };
		Effekseer::ConvertUtf8ToUtf16( (int16_t *)szFileName, MAX_PATH, (const int8_t *)strFileName.c_str() );

		FILE * pFile = nullptr;
		if ( fopen_s( &pFile, strFileName.c_str(), "rb" ) == 0 )
		{
			//Get File Size
			fseek( pFile, 0, SEEK_END );
			int32_t iFileSize = ftell( pFile );

			//Reset
			fseek( pFile, 0, SEEK_SET );

			//Read Buffer
			BYTE * pbBuffer = new BYTE[iFileSize];
			fread( pbBuffer, iFileSize, 1, pFile );

			//Close
			fclose( pFile );

			//Create Effect
			if ( auto pcEffect = Effekseer::Effect::Create( pcManager, pbBuffer, iFileSize, 14.0f ) )
			{
				//New Script Cache
				pcParticleScript = new CEffekSeerParticleScript( strFileName, pcEffect );

				//Add to Cache
				AddScriptToCache( pcParticleScript );
			}

			//Delete Buffer
			DELETA( pbBuffer );
		}
	}

	//Has a Script?
	if ( pcParticleScript )
	{
		//Add New One
		pcRet = new CEffekSeerParticleScript( *pcParticleScript );
	}

	return pcRet;
}

bool CEffekseerHandler::AddScript( CEffekSeerParticleScript * pcScript )
{
#if defined(_DEBUG)
	//return false;
#endif
	bool bRet = false;

	CPoint<int> sPosition = CPoint<int>( 0, 0, 0 );
	if ( pcScript->GetParticleSystems().size() > 0 )
	{
		if ( pcScript->GetParticleSystems()[0]->GetOwner() )
		{
			pcScript->GetParticleSystems()[0]->GetOwner()->Update();

			sPosition = pcScript->GetParticleSystems()[0]->GetOwner()->GetPosition();
		}
	}

	if ( auto iHandle = pcManager->Play( pcScript->GetEffect(), (float)sPosition.GetX() / 256.0f, (float)sPosition.GetY() / 256.0f, (float)sPosition.GetZ() / 256.0f ); iHandle >= 0 )
	{
		for ( auto pcParticleSystem : pcScript->GetParticleSystems() )
		{
			//Set Handle
			pcParticleSystem->SetHandle( iHandle );

			//Add Particle System
			vParticleSystems.push_back( pcParticleSystem );
		}

		if ( pcScript->GetParticleSystems().size() > 0 )
		{
			if ( pcScript->GetParticleSystems()[0]->GetOwner() )
			{
				auto sRot = pcScript->GetParticleSystems()[0]->GetOwner()->GetRotation();
				X3D::Vector3 vRot = X3D::Vector3( PTAngleToRadians( sRot.iX ), PTAngleToRadians( sRot.iY ), PTAngleToRadians( sRot.iZ ) );
				pcManager->SetRotation( iHandle, vRot.x, vRot.y, vRot.z );
			}
		}

		pcManager->SetRemovingCallback( iHandle, RemoveCallBack );

		bRet = true;
	}

	pcScript->Clear();
	
	DELET( pcScript );

	return bRet;
}

void CEffekseerHandler::Kill( UINT uID )
{
	for ( auto pcParticleSystem : vParticleSystems )
	{
		if ( pcParticleSystem->GetID() == uID )
			pcParticleSystem->Kill( TRUE );
	}
}

void CEffekseerHandler::Kill( UINT uID, UINT uOwnerID )
{
	for ( auto pcParticleSystem : vParticleSystems )
	{
		if ( (pcParticleSystem->GetID() == uID) && (pcParticleSystem->GetOwnerID() == uOwnerID) )
			pcParticleSystem->Kill( TRUE );
	}
}

void CEffekseerHandler::KillHandle( Effekseer::Handle iHandle )
{
	for ( auto it = vParticleSystems.begin(); it != vParticleSystems.end(); )
	{
		auto pcParticleSystem = (*it);
		if ( pcParticleSystem->GetHandle() == iHandle )
		{
			DELET( pcParticleSystem );

			it = vParticleSystems.erase( it );
		}
		else
			it++;
	}
}

void CEffekseerHandler::Update( float fTime )
{
	for ( auto it = vParticleSystems.begin(); it != vParticleSystems.end(); )
	{
		auto pcParticleSystem = (*it);

		//Can Update?
		if ( pcParticleSystem->Update( fTime ) )
		{
			//Have an Owner?
			if ( pcParticleSystem->GetOwner() )
			{
				auto sOwnerPosition = pcParticleSystem->GetOwner()->GetPosition();
				Effekseer::Vector3D sPosition = Effekseer::Vector3D( (float)sOwnerPosition.GetX() / 256.0f, (float)sOwnerPosition.GetY() / 256.0f, (float)sOwnerPosition.GetZ() / 256.0f );

				//Move Particle
				pcManager->SetLocation( pcParticleSystem->GetHandle(), sPosition );

				//Has Target?
				if ( pcParticleSystem->GetOwner()->HasTarget() )
				{
					auto sTargetPosition = pcParticleSystem->GetOwner()->GetTargetPosition();
					
					Effekseer::Vector3D sTPosition = Effekseer::Vector3D( (float)sTargetPosition.iX / 256.0f, (float)sTargetPosition.iY / 256.0f, (float)sTargetPosition.iZ / 256.0f );
					pcManager->SetTargetLocation( pcParticleSystem->GetHandle(), sTPosition );
				}
			}

			it++;
		}
		else
		{
			EFFEKSEERHANDLER->GetManager()->StopEffect( pcParticleSystem->GetHandle() );

			DELET( pcParticleSystem );

			it = vParticleSystems.erase( it );
		}
	}

	//Update Particles Effects
	pcManager->Update();
}

void CEffekseerHandler::Render()
{
	//Begin Renderer
	if ( pcRenderer->BeginRendering() )
	{
		//Reset Render States
		pcRenderer->ResetRenderState();

		//Draw Particle Back
		for ( auto pcParticleSystem : vParticleSystems )
			pcParticleSystem->Render( pcManager, TRUE );

		//Draw Particle Front
		for ( auto pcParticleSystem : vParticleSystems )
			pcParticleSystem->Render( pcManager, FALSE );

		//End Renderer
		pcRenderer->EndRendering();
	}
}

void CEffekseerHandler::OnLost( CEffekSeerParticleSystem * pcParticleSystem )
{
	if ( pcParticleSystem->GetID() == PARTICLEID_MAGNETIC_DISCHARGE )
	{
		if ( auto pcUnit = UNITDATATOUNIT( UNITDATABYID( pcParticleSystem->GetOwnerID() ) ) )
			MAGNETICDISCHARGEHANDLER->OnCastEnd( pcUnit );
	}
}

void CEffekseerHandler::OnLostDevice()
{
	((EffekseerRendererDX9::Renderer *)EFFEKSEERHANDLER->GetRenderer())->SetBackground( NULL );

	pcRenderer->OnLostDevice();

	for ( auto pcParticleSystem : vParticleSystems )
		pcParticleSystem->GetEffect()->UnloadResources();

	RELEASE( lpRenderSurface );
}

void CEffekseerHandler::OnResetDevice()
{
	pcRenderer->OnResetDevice();

	for ( auto pcParticleSystem : vParticleSystems )
		pcParticleSystem->GetEffect()->ReloadResources();

	CreateRenderTarget();
}

CEffekSeerParticleScript * CEffekseerHandler::GetScriptFromCache( const std::string strFileName )
{
	for ( auto pcParticleScript : vParticleScriptCache )
	{
		//Same file in Cache? So return it
		if ( pcParticleScript->GetFileName().compare( strFileName ) == 0 )
			return pcParticleScript;
	}

	return nullptr;
}

void CEffekseerHandler::AddScriptToCache( CEffekSeerParticleScript * pcParticleScript )
{
	vParticleScriptCache.push_back( pcParticleScript );
}

void CEffekseerHandler::CreateRenderTarget()
{
	RELEASE( lpRenderSurface );

	if ( SUCCEEDED( DX::Graphic::GetDevice()->CreateRenderTarget( GRAPHICS->presentParams.BackBufferWidth, GRAPHICS->presentParams.BackBufferHeight,
		 GRAPHICS->presentParams.BackBufferFormat == D3DFMT_X4R4G4B4 ? D3DFMT_A4R4G4B4 : D3DFMT_A8R8G8B8,
		 GRAPHICS->presentParams.MultiSampleType, GRAPHICS->presentParams.MultiSampleQuality, FALSE, &lpRenderSurface, NULL ) ) )
	{

	}
}

bool CEffekseerDistortion::CopyRenderTarget()
{
	bool bRet = false;
	IDirect3DSurface9 * pBackBuffer				= nullptr;
	IDirect3DSurface9 * pDepthStencilSurface	= nullptr;

	if ( SUCCEEDED( GetGraphicsDevice()->Get()->GetRenderTarget( 0, &pBackBuffer ) ) )
	{
		if ( SUCCEEDED( GetGraphicsDevice()->Get()->GetDepthStencilSurface( &pDepthStencilSurface ) ) )
		{
			GetGraphicsDevice()->Get()->SetRenderTarget( 0, EFFEKSEERHANDLER->GetDistortionRenderTarget() );
			GetGraphicsDevice()->Get()->SetDepthStencilSurface( nullptr );

			D3DSURFACE_DESC desc;
			pBackBuffer->GetDesc( &desc );

			GetGraphicsDevice()->Get()->StretchRect( pBackBuffer, nullptr, EFFEKSEERHANDLER->GetDistortionRenderTarget(), nullptr, D3DTEXF_POINT );

			GetGraphicsDevice()->Get()->SetRenderTarget( 0, pBackBuffer );
			GetGraphicsDevice()->Get()->SetDepthStencilSurface( pDepthStencilSurface );

			GetGraphicsDevice()->Get()->StretchRect( EFFEKSEERHANDLER->GetDistortionRenderTarget(), nullptr, EFFEKSEERHANDLER->GetRenderTarget()->GetSurface(), nullptr, D3DTEXF_POINT );


			ES_SAFE_RELEASE( pDepthStencilSurface );

			bRet = true;
		}

		ES_SAFE_RELEASE( pBackBuffer );
	}

	return bRet;
}

bool CEffekseerDistortion::OnDistorting()
{
	if ( EFFEKSEERHANDLER->GetRenderTarget()->GetTexture() == NULL )
	{
		((EffekseerRendererDX9::Renderer *)EFFEKSEERHANDLER->GetRenderer())->SetBackground( NULL );
		return false;
	}

	CopyRenderTarget();

	((EffekseerRendererDX9::Renderer *)EFFEKSEERHANDLER->GetRenderer())->SetBackground( EFFEKSEERHANDLER->GetRenderTarget()->GetTexture() );
	return true;
}
