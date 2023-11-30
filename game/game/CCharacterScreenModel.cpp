#include "stdafx.h"
#include "CCharacterScreenModel.h"

static const Point3D saFireParticles[] =
{
	Point3D( -117 << 8, 35 << 8, -98 << 8 ),
	Point3D( -117 << 8, 35 << 8, -30 << 8 ),
	Point3D( 117 << 8, 35 << 8, -98 << 8 ),
	Point3D( 117 << 8, 35 << 8, -30 << 8 )
};


CCharacterScreenModel::CCharacterScreenModel()
{
	sCameraPosition		= Point3D( 0 << 8, 75 << 8, -350 << 8 );
	sCameraAngle		= Point3D( 0, 0, 0 );

	sNextCameraPosition = sCameraPosition;

	sUserCharactersPosition = Point3D( 0 << 8, 10 << 8, -99 << 8 );

	sLookAt = Point3D( 0 << 8, 32 << 8, -99 << 8 );
}


CCharacterScreenModel::~CCharacterScreenModel()
{
	ClearUserCharacters();
}

void CCharacterScreenModel::Init()
{
	pcStage = Stage::LoadStage( "game\\maps\\newchrselect\\scene.ASE" );
	//pcStage = Stage::LoadStage( "Field\\castle\\castle.smd" );
	pcStage->bState = TRUE;

	for ( int i = 0; i < _countof( saFireParticles ); i++ )
	{
		CParticleScript * pc = PARTICLEFACTORY->LoadScript( "game\\scripts\\particles\\CharacterSelectFire.lua" );
		pc->SetOwner( new Engine::ObjectOwnerPosition( saFireParticles[i] ) );
		PARTICLEENGINE->AddScript( pc );
	}

	UpdateTextures();
}

BOOL CCharacterScreenModel::Shutdown()
{
	//DELET( pcStage );

	return TRUE;
}

void CCharacterScreenModel::Render3D()
{
	SetCharacterCamera( sCameraPosition.iX, sCameraPosition.iY, sCameraPosition.iZ, sCameraAngle.iX, sCameraAngle.iY, sCameraAngle.iZ );

	SetCamera( sCameraPosition, sLookAt );

	for ( int i = 0; i < _countof( saFireParticles ); i++ )
	{
		SetDynamicLight( saFireParticles[i].iX, saFireParticles[i].iY, saFireParticles[i].iZ, 200, 75, 0, 255, 15 );
	}

	SetDynamicLights();

	pcStage->Render( sCameraPosition.iX, sCameraPosition.iY, sCameraPosition.iZ, sCameraAngle.iX, sCameraAngle.iY, sCameraAngle.iZ );

	RENDERER->ClearLight();
	RENDERER->r = 0;
	RENDERER->g = 0;
	RENDERER->b = 0;
	RENDERER->a = 0;

	PARTICLEENGINE->Render3D( NULL, sCameraPosition, sCameraAngle );

	EFFEKSEERHANDLER->Render();

	RenderUserCharacters();

	CALLT( 0x0047F200, (DWORD)RENDERER );

	DX::cSelectGlow1.Draw();
}

void CCharacterScreenModel::AddCameraAxis( Point3D sPosition )
{
	sCameraPosition += sPosition;
}

void CCharacterScreenModel::AddNextCameraAxis( Point3D sPosition )
{
	sNextCameraPosition += sPosition;

	fCameraTimeUpdate = 0.0f;
}

void CCharacterScreenModel::Frame()
{
	FrameUserCharacters();
}

void CCharacterScreenModel::Update( float fTime )
{
	UpdateCamera( fTime );

	UpdateUserCharacters( fTime );
}

void CCharacterScreenModel::ClearUserCharacters()
{
	for ( auto & pc : vUserCharacters )
	{
		pc->Free();
		DELET( pc );
	}

	vUserCharacters.clear();
}

void CCharacterScreenModel::AddUserCharacter( const char * pszName, const char * pszModelFile, const char * pszHeadFile, ECharacterClass iClass, int iLevel, int iClanID )
{
	UnitData * pcUnitData = new UnitData;

	pcUnitData->Init();
	pcUnitData->pcStage = pcStage;

	LoadUnitData( pcUnitData, pszModelFile, pszHeadFile, &sUserCharactersPosition, &Point3D( 0, 2048, 0 ) );
	pcUnitData->eUpdateMode = UPDATEMODE_ClientSelf;


	STRINGCOPY( pcUnitData->sCharacterData.szName, pszName );
	pcUnitData->sCharacterData.iClanID = iClanID;
	pcUnitData->sCharacterData.iClass = iClass;
	pcUnitData->sCharacterData.iLevel = iLevel;
	pcUnitData->sCharacterData.sWarpHomeTown = 0;

	pcUnitData->SetPosition( sUserCharactersPosition.iX, pcStage->GetHighestPoint( sUserCharactersPosition.iX, sUserCharactersPosition.iZ ), sUserCharactersPosition.iZ, 0, 2048, 0 );

	pcUnitData->ChangeAnimationID( 10 );

	pcUnitData->bActive = TRUE;
	pcUnitData->bVisible = TRUE;

	vUserCharacters.push_back( pcUnitData );

	UpdateTextures();

	UpdateUserCharactersPosition();
}

void CCharacterScreenModel::ResetCharacterSelect()
{
	pcCharacter = nullptr;

	fCharacterSelectTimeUpdate = 0.0f;
}

void CCharacterScreenModel::SetCharacter( UnitData * pcUnitData )
{
	pcCharacter = pcUnitData;

	//MoveCharacter( pcCharacter );
	
	fCharacterSelectTimeUpdate = 0.0f;
}

void CCharacterScreenModel::MoveCharacter( UnitData * pcUnitData )
{
	pcUnitData->sCharacterData.sSP.sCur = 1000;
	pcUnitData->sCharacterData.sSP.sMax = 1000;
	pcUnitData->sCharacterData.sWeight.sMax = 30000;
	pcUnitData->sCharacterData.sWeight.sMax = 30000;

	pcUnitData->SetTarget( pcUnitData->sPosition.iX >> 8, (pcUnitData->sPosition.iZ >> 8) - 8 );
	pcUnitData->bRunMode = TRUE;
	pcUnitData->sCharacterData.iMovementSpeed = 1;
	pcUnitData->iMoveCount = 54;
}

bool CCharacterScreenModel::CanSelectUserCharacters()
{
	bool bRet = false;

	if ( (GetStep() == CHARACTERSCREENSTEP_None) && (GetNewStep() == CHARACTERSCREENSTEP_None) )
		bRet = true;

	return bRet;
}

UnitData * CCharacterScreenModel::GetUserCharacterMouseOver( Point2D * psPoint )
{
	UnitData * pcRet = nullptr;
	for ( auto & pc : vUserCharacters )
	{
		if ( pc->bVisible )
		{
			RECT * psRect2D = &pc->rRenderRectangle;
			if ( (psRect2D->left < psPoint->iX) && (psRect2D->right > psPoint->iX) )
			{
				if ( (psRect2D->top < psPoint->iY) && (psRect2D->bottom > psPoint->iY) )
				{
					pcRet = pc;
					break;
				}
			}
		}
	}

	return pcRet;
}

bool CCharacterScreenModel::CanBackLoginScreen()
{
	if ( (GetStep() == CHARACTERSCREENSTEP_None) && (GetNewStep() == CHARACTERSCREENSTEP_None) )
		return true;

	return false;
}

void CCharacterScreenModel::UpdateCamera( float fTime )
{
	if ( sNextCameraPosition != sCameraPosition )
	{
		if ( fCameraTimeChangeDuration > 0.0f )
		{
			float f = X3D::easeInQuadf( 0.0f, fCameraTimeChangeDuration, fCameraTimeUpdate );

			Point3D sPosition	= sNextCameraPosition - sCameraPosition;
			sPosition.iX		= (int)round( ((float)sPosition.iX * f) / fCameraTimeChangeDuration );
			sPosition.iY		= (int)round( ((float)sPosition.iY * f) / fCameraTimeChangeDuration );
			sPosition.iZ		= (int)round( ((float)sPosition.iZ * f) / fCameraTimeChangeDuration );

			sCameraPosition += sPosition;

			fCameraTimeUpdate += fTime;

			if ( fCameraTimeUpdate >= fCameraTimeChangeDuration )
			{
				sNextCameraPosition = sCameraPosition;

				fCameraTimeUpdate = 0.0f;
			}
		}
	}
}

void CCharacterScreenModel::SetCamera( Point3D sPosition, Point3D sLookAt )
{
	struct eCAMERA_TRACE
	{
		int x, y, z;
		int tx, ty, tz;

		EXEMatrixE	eRotMatrix;
		int AngX, AngY;
	};

	eCAMERA_TRACE eTrace;

	CALL_WITH_ARG7( 0x0048B540, (DWORD)&eTrace, sPosition.iX, sPosition.iY, sPosition.iZ, sLookAt.iX, sLookAt.iY, sLookAt.iZ );
	CALLT_WITH_ARG4( 0x0047F1A0, (DWORD)RENDERER, sPosition.iX, sPosition.iY, sPosition.iZ, (DWORD)&eTrace.eRotMatrix );


	RENDERER->SetCameraPosition( sPosition.iX, sPosition.iY, sPosition.iZ, 0, 0, 0 );

	sCameraAngle.iX = eTrace.AngX;
	sCameraAngle.iY = eTrace.AngY;

	(*(int*)0x00A17520) = sPosition.iX;
	(*(int*)0x00A17510) = sPosition.iY;
	(*(int*)0x00A1619C) = sPosition.iZ;

	(*(int*)0x00824FA8) = eTrace.AngX;
	(*(int*)0x00A17630) = eTrace.AngY;
}

bool CCharacterScreenModel::CanRenderUserCharacters()
{
	bool bRender = false;

	if ( (GetStep() == CHARACTERSCREENSTEP_None) || (GetNewStep() == CHARACTERSCREENSTEP_None) ||
		(GetStep() == CHARACTERSCREENSTEP_NewCharacterClass) || (GetNewStep() == CHARACTERSCREENSTEP_NewCharacterClass) ||
		(GetStep() == CHARACTERSCREENSTEP_NewCharacterHair) || (GetNewStep() == CHARACTERSCREENSTEP_NewCharacterHair) )
		bRender = true;

	return bRender;
}

void CCharacterScreenModel::UpdateUserCharactersPosition()
{
	Point3D sPosition = Point3D( 0, 0, 0 );

	int iIndex = 0;
	for ( auto & pc : vUserCharacters )
	{
		if ( pc->bVisible )
		{
			pc->sPosition = sUserCharactersPosition;

			//Left
			if ( (iIndex % 2) == 0 )
				pc->sPosition.iX += sPosition.iX;
			else
				pc->sPosition.iX -= sPosition.iX;

			pc->sPosition.iZ += sPosition.iZ;

			if ( ((iIndex % 2) == 0) )
			{
				sPosition.iX -= 34 << 8; //2.5meters	
				sPosition.iZ += 8 << 8; //0.5meter				
			}
		}

		iIndex++;
	}
}

void CCharacterScreenModel::RenderUserCharacters()
{
	if ( CanRenderUserCharacters() )
	{
		auto pcUnitHover = GetUserCharacterMouseOver( MOUSEHANDLER->GetPosition() );

		for ( auto & pc : vUserCharacters )
		{
			if ( pc->bVisible )
			{
				bool bColor = false;

				if ( pcCharacter )
				{
					if ( pcCharacter == pc )
					{
						RENDERER->r += bCharacterLocked ? 150 : 50;
						RENDERER->g += bCharacterLocked ? 150 : 50;
						RENDERER->b += bCharacterLocked ? 150 : 50;
						bColor = true;
					}
				}

				//Glow if its on hover
				bool bGlowCharacter = false;
				if ( (pcUnitHover == pc) || (pcCharacter && (pcCharacter == pc)) )
				{
					bGlowCharacter = true;

					if ( (pcUnitHover == pc) )
						if ( bCharacterLocked && (pcCharacter != pc) )
							bGlowCharacter = false;
				}

				if ( bGlowCharacter )
				{
					if ( DX::cSelectGlow1.Begin( D3DCOLOR_XRGB( 255, 255, 255 ), TRUE ) )
					{
						pc->Render();
						DX::cSelectGlow1.End( TRUE );

						if ( DX::cSelectGlow1.Begin( D3DCOLOR_XRGB( 255, 255, 255 ), FALSE ) )
						{
							pc->Render();
							DX::cSelectGlow1.End( FALSE );
						}
					}
				}

				pc->Render();

				if ( bColor )
				{
					RENDERER->r -= bCharacterLocked ? 150 : 50;
					RENDERER->g -= bCharacterLocked ? 150 : 50;
					RENDERER->b -= bCharacterLocked ? 150 : 50;
				}
			}
		}
	}
}

void CCharacterScreenModel::UpdateUserCharacters( float fTime )
{
	return;

	if ( CanRenderUserCharacters() )
	{
		bool bUpdateTime = false;

		//Reset Transparency
		if ( pcCharacter == nullptr )
		{
			for ( auto & pc : vUserCharacters )
			{
				if ( pc->iTransparencySelf < -30 )
				{
					float f = X3D::easeInQuadf( 0.0f, 500.0f, fCharacterSelectTimeUpdate );

					pc->iTransparencySelf = (int)round( (-150.0f *fCharacterSelectTimeUpdate) / 500.0f );

					bUpdateTime = true;
				}
			}
		}
		else
		{
			for ( auto & pc : vUserCharacters )
			{
				if ( pc != pcCharacter )
				{
					if ( pc->iTransparencySelf > (-150) )
					{
						float f = X3D::easeInQuadf( 0.0f, 500.0f, fCharacterSelectTimeUpdate );

						pc->iTransparencySelf = -(int)round( (150.0f * fCharacterSelectTimeUpdate) / 500.0f );

						bUpdateTime = true;
					}
				}
				else
				{
					if ( pc->iTransparencySelf < -30 )
					{
						float f = X3D::easeInQuadf( 0.0f, 500.0f, fCharacterSelectTimeUpdate );

						pc->iTransparencySelf = (int)round( (-150.0f *fCharacterSelectTimeUpdate) / 500.0f );

						bUpdateTime = true;
					}
				}
			}
		}

		if ( bUpdateTime )
			fCharacterSelectTimeUpdate += fTime;
	}
}

void CCharacterScreenModel::FrameUserCharacters()
{
	if ( CanRenderUserCharacters() )
	{
		for ( auto & pc : vUserCharacters )
		{
			if ( pc->bVisible )
				pc->Main();
		}
	}
}

void CCharacterScreenModel::UpdateTextures()
{
	//function EXETexture::ReadTextures()
	CALL( 0x0048A260 );
}
