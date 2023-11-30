#include "stdafx.h"
#include "CCharacterScreen.h"

PacketCharacterSelectUser CCharacterScreen::sPacketCharactersUser;

CCharacterScreen::CCharacterScreen()
{
	CALLT_WITH_ARG1( 0x005881A0, GetThis(), TRUE );

	pcCharacterScreenModel = new CCharacterScreenModel();
	pcWindow = new CCharacterScreenWindowHandler();
}


CCharacterScreen::~CCharacterScreen()
{
//	EFFEKSEERHANDLER->Kill( PARTICLEID_CHARACTERSELECT_FIRE );

	CALLT( 0x00589530, GetThis() );

	DELET( pcCharacterScreenModel );
}

const X3D::Vector3 saParticlesPosition[] =
{
	{-115.395f, 35.426f, -31.902f},
	{-115.832f, 35.426f, -99.014f},
	{115.598f, 35.426f, -99.014f},
	{116.035f, 35.426f, -31.902f}
};

BOOL CCharacterScreen::Init()
{
	iCurrentScreen = SCREEN_Character;

	CALLT( 0x005882F0, GetThis() );

	Sky::GetInstance()->SetSky( 128 );

	//Add Fires
	{	
		for ( int i = 0; i < _countof( saParticlesPosition ); i++ )
		{
			if ( auto pcScript = EFFEKSEERHANDLER->LoadScript( FormatString( "game\\scripts\\particles\\FireCharacterSelect%d.efk", i + 1 ) ) )
			{
				pcScript->SetID( PARTICLEID_CHARACTERSELECT_FIRE + i );

				auto pcOwner = new Engine::ObjectOwnerPosition( Point3D( (int)(saParticlesPosition[i].x * 256.0f), (int)(saParticlesPosition[i].y * 256.0f), (int)(saParticlesPosition[i].z * 256.0f) ) );
				pcScript->SetOwner( pcOwner );

				EFFEKSEERHANDLER->AddScript( pcScript );
			}
		}
	}

/*
	pcCharacterScreenModel->Init();
	pcWindow->Init();

	for ( int i = 0; i < sPacketCharactersUser.iCount; i++ )
	{
		pcCharacterScreenModel->AddUserCharacter( sPacketCharactersUser.saData[i].szName, sPacketCharactersUser.saData[i].szBodyModel, 
			sPacketCharactersUser.saData[i].szHeadModel, sPacketCharactersUser.saData[i].iClass, sPacketCharactersUser.saData[i].iLevel, 0 );
	}
	*/
	return TRUE;
}

void CCharacterScreen::Shutdown()
{
	for ( int i = 0; i < _countof( saParticlesPosition ); i++ )
		EFFEKSEERHANDLER->Kill( PARTICLEID_CHARACTERSELECT_FIRE + i );

	pcCharacterScreenModel->Shutdown();
}

void CCharacterScreen::Render3D()
{
	Sky::Draw( 0, 0, 0, 0, 0, 0 );

	Effekseer::Matrix44 mV;
	CopyMemory( &mV, &GRAPHICS->GetRenderer()->GetCamera()->View(), sizeof( Effekseer::Matrix44 ) );
	Effekseer::Matrix44 mP;
	CopyMemory( &mP, &GRAPHICS->GetRenderer()->GetCamera()->Projection(), sizeof( Effekseer::Matrix44 ) );

	EFFEKSEERHANDLER->GetRenderer()->SetProjectionMatrix( (Effekseer::Matrix44 &)GRAPHICENGINE->GetCamera()->Projection() );

	EFFEKSEERHANDLER->GetRenderer()->SetCameraMatrix( (Effekseer::Matrix44 &)GRAPHICENGINE->GetCamera()->View() );

    //GRAPHICENGINE->GetCamera()->SetPosition( Math::Vector3( sPosition.iX / 256.0f, sPosition.iY / 256.0f, sPosition.iZ / 256.0f ), Math::Vector3( sLookAt.iX / 256.0f, (sLookAt.iY / 256.0f), sLookAt.iZ / 256.0f ) );

    if ( GRAPHICENGINE->GetTerrainShader() )
    {
        GRAPHICENGINE->GetTerrainShader()->SetMatrix( "Projection", GRAPHICENGINE->GetCamera()->Projection().Get() );
        GRAPHICENGINE->GetTerrainShader()->SetTechnique( "Terrain" );
        GRAPHICENGINE->GetTerrainShader()->SetFloat( "FogEnd", 1500.f );
        GRAPHICENGINE->GetTerrainShader()->SetInt( "NumActiveLights", 0 );

        GRAPHICS->GetRenderer()->SetFogEnd( 1500.f );
    }

 //   EXERender::SetRenderWithShaders( TRUE );

    if ( EXERender::IsRenderingWithShaders() )
    {
        if ( GRAPHICENGINE->GetTerrainShader() )
        {
            GRAPHICS->GetRenderer()->PrepareLights( GRAPHICENGINE->GetTerrainShader() );
            GRAPHICS->GetRenderer()->PrepareShadowMap( GRAPHICENGINE->GetTerrainShader() );
        }
    }

    CALLT( 0x00589620, GetThis() );

//    EXERender::SetRenderWithShaders( FALSE );

	return;
	

	pcCharacterScreenModel->Render3D();

	Point3D sC;
	Point3D sA;

	RENDERER->GetCameraPosition( sC, sA );

	GetGraphicsDevice()->GetDefaultFont()->Draw( FormatString( "X: %d  Y: %d  Z: %d  AX: %d  AY: %d  AZ: %d", sC.iX, sC.iY, sC.iZ, sA.iX, sA.iY, sA.iZ ), 10, 10, 0,
		DX::Color( 255, 255, 255 ) );

	if ( bLoadingTime )
		GRAPHICENGINE->DrawLoadingImage();

	pcWindow->Render();
}

void CCharacterScreen::Render2D()
{
}

EScreen CCharacterScreen::Frame()
{
	//Keep
	//pcCharacterScreenModel->Frame();
	//pcCharacterScreenModel->Update( 1000.0f / 60.0f );

	
	DWORD dwRet = CALLT( 0x00588710, GetThis() );
	//CALLT( 0x00589500, GetThis() );


	if ( dwRet == 1 )
	{
		if ( bLoadingTime )
		{
			iCurrentScreen = SCREEN_World;
		}
	}

	if ( dwRet == 2 )
		iCurrentScreen = SCREEN_Login;

	if ( dwRet == 3 )
	{
		bLoadingTime = TRUE;
	}


	//New, Keep
	if ( bLoadingTime )
	{
		if ( (dwLoadingTime + 100) < TICKCOUNT )
			iCurrentScreen = SCREEN_World;
	}
	

	return iCurrentScreen;
}

void CCharacterScreen::OnMouseMove( CMouse * pcMouse )
{
	pcWindow->OnMouseMove( pcMouse );
}

BOOL CCharacterScreen::OnMouseClick( CMouse * pcMouse )
{
	//Old
	return FALSE;

	BOOL bRet = FALSE;

	if ( (pcMouse->GetEvent() == EMouseEvent::ClickDownL) || (pcMouse->GetEvent() == EMouseEvent::DoubleClickL) )
	{
		if ( pcCharacterScreenModel->CanSelectUserCharacters() )
		{
			if ( pcCharacterScreenModel->GetCharacterLocked() == false )
			{
				UnitData * pc = pcCharacterScreenModel->GetUserCharacterMouseOver( pcMouse->GetPosition() );
				if ( pc )
				{
					if ( pcMouse->GetEvent() == EMouseEvent::DoubleClickL )
					{
						pcCharacterScreenModel->SetCharacterLocked( true );

						MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
						MESSAGEBOX->SetTitle( "Character" );
						MESSAGEBOX->SetDescription( FormatString( "Do you want to select this character?" ) );
						MESSAGEBOX->SetEvent( std::bind( &CCharacterScreen::OnSelectedCharacterBox, this ) );
						MESSAGEBOX->SetAutoClose( 10 );
						MESSAGEBOX->Show();
					}
					else
						pcCharacterScreenModel->SetCharacterLocked( false );

					pcCharacterScreenModel->SetCharacter( pc );
					bRet = TRUE;
				}
				else
					pcCharacterScreenModel->ResetCharacterSelect();
			}
		}
	}

	if ( bRet == FALSE )
		bRet = pcWindow->OnMouseClick( pcMouse );

	return bRet;
}

void CCharacterScreen::OnResolutionChanged()
{
	pcWindow->OnResolutionChanged();
}

void CCharacterScreen::UpdateScreen( EScreen iScreen )
{
	iCurrentScreen = iScreen;
	if ( iCurrentScreen == SCREEN_Login )
		Disconnect();
}

void CCharacterScreen::HandlePacket( PacketCharacterSelectUser * psPacket )
{
	CopyMemory( &sPacketCharactersUser, psPacket, sizeof( PacketCharacterSelectUser ) );
}

CharacterSelectPacketData & CCharacterScreen::GetCharacterData( const char * pszName )
{
	CharacterSelectPacketData & s = CCharacterScreen::sPacketCharactersUser.saData[0];

	for ( int i = 0; i < sPacketCharactersUser.iCount; i++ )
	{
		if ( STRINGCOMPARE( sPacketCharactersUser.saData[i].szName, pszName ) )
		{
			s = sPacketCharactersUser.saData[i];

			break;
		}
	}

	return s;
}

void CCharacterScreen::OnSelectedCharacterBox()
{
	if ( MESSAGEBOX->GetType() )
		SetCharacterUser( GetCharacterData( pcCharacterScreenModel->GetCharacter()->GetName() ) );
	else
		pcCharacterScreenModel->SetCharacterLocked( false );
}

void CCharacterScreen::SetCharacterUser( CharacterSelectPacketData & sData )
{
	STRINGCOPYLEN( CHARACTER_SELECT_NAME, 32, sData.szName );
	STRINGCOPYLEN( CHARACTER_SELECT_BODYMODEL, 64, sData.szBodyModel );
	STRINGCOPYLEN( CHARACTER_SELECT_HEADMODEL, 64, sData.szHeadModel );
	CHARACTER_SELECT_CLASS = sData.iClass;
	CHARACTER_SELECT_MAPID = sData.iMapID;
	CHARACTER_SELECT_X = sData.iX;
	CHARACTER_SELECT_Z = sData.iZ;

	dwLoadingTime = TICKCOUNT;
	bLoadingTime = TRUE;
}

void CCharacterScreen::Disconnect()
{
	CALL( 0x0061E9B0 );
}
