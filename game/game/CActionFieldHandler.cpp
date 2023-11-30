#include "stdafx.h"
#include "CActionFieldHandler.h"


CActionFieldHandler::CActionFieldHandler()
{
	pcChestWindow = new CActionFieldChestWindow();
	pcRequestWindow = new CActionFieldRequestWindow();
	pGhostCastleInfo = std::make_shared<UI::Window>( Rectangle2D( (RESOLUTION_WIDTH >> 1) - (262 >> 1), 85, 262, 72 ) );
}


CActionFieldHandler::~CActionFieldHandler()
{
	DELET( pcChestWindow );
	DELET( pcRequestWindow );
}

void CActionFieldHandler::HandlePacket( PacketActionFieldRequest * psPacket )
{
	if ( MOUNTHANDLER->IsRiding( UNITDATA ) )
	{
		MOUNTHANDLER->ShowMessage( MOUNTMESSAGE_ErrorEnterNPC );
		return;
	}

	Clear();
	pcRequestWindow->ClearList();

	for ( int i = 0; i < psPacket->iCount; i++ )
		pcRequestWindow->AddModeList( psPacket->szaName[i], psPacket->saRequiredLevel[i], psPacket->iaInstanceCountToday[i], psPacket->iaInstanceLimitPerDay[i], psPacket->iaModeID[i] );

	pcRequestWindow->Open();
}

void CActionFieldHandler::HandlePacket( PacketActionFieldEnter * psPacket )
{
	Clear();
}

void CActionFieldHandler::HandlePacket( PacketActionFieldFail * psPacket )
{
	PARTICLEENGINE->Kill( PARTICLEID_GHOSTCASTLEPORTAL );
	DROPITEMAREAHANDLER->KillCircle( DROPAREACIRCLEID_ForestDungeon );
}

void CActionFieldHandler::HandlePacket( PacketActionFieldUpdate * psPacket )
{
	UI::Text_ptr pTextName = pGhostCastleInfo->GetElement<UI::Text>( TEXTID_Name );
	pTextName->SetText( psPacket->szName );

	if ( psPacket->iRound != iRound )
	{
		if ( psPacket->bStarted )
		{
			iAlphaRound = 0;
			fTimeImageRound = 0.0f;
			bShowImageRound = TRUE;

			iRound = psPacket->iRound;

			if ( iRound > 1 )
				AddParticleNextRound();

			AddParticleLimitRange();
		}
	}

	ACTIONFIELD_CAMERAX = (iRound * 0x35E0E) + 0x00189C00;
	BaseMap * pcBaseMap = MAPGAME->GetBaseMapGame() + MAP_ID;
	if ( pcBaseMap )
		ACTIONFIELD_CAMERAX = (iRound * 0x35E0E) + ((pcBaseMap->GetCenter().iX + 500) << 8);

	UI::Text_ptr pTextRound = pGhostCastleInfo->GetElement<UI::Text>( TEXTID_Round );
	pTextRound->SetText( psPacket->bStarted ? FormatString( "Round %d", iRound ) : "" );

	UI::Text_ptr pTextTime = pGhostCastleInfo->GetElement<UI::Text>( TEXTID_Time );
	if ( psPacket->bFinished )
		pTextTime->SetText( "" );
	else if ( psPacket->bStarted )
		pTextTime->SetTextEx( "Time Left: %02d:%02d", psPacket->iTime / 60, psPacket->iTime % 60 );
	else
		pTextTime->SetTextEx( "Time to start: %02d:%02d", psPacket->iTime / 60, psPacket->iTime % 60 );

	//Update Users
	for ( int i = 0; i < psPacket->iUsersCount; i++ )
	{
		UI::Text_ptr pName = pGhostCastleInfo->GetElement<UI::Text>( TEXTID_PlayerNameOne + i );
		UI::Text_ptr pKills = pGhostCastleInfo->GetElement<UI::Text>( TEXTID_PlayerKillsOne + i );
		UI::Text_ptr pPoints = pGhostCastleInfo->GetElement<UI::Text>( TEXTID_PlayerPointsOne + i );
		if ( pName && pKills && pPoints )
		{
			Unit * pcUnit = UNITDATATOUNIT( UNITDATABYID( psPacket->saUsers[i].iID ) );
			if ( pcUnit )
			{
				pName->SetText( pcUnit->GetName() );
				pKills->SetTextEx( "%d", psPacket->saUsers[i].iKills );
				pPoints->SetText( FormatNumber( (INT64)psPacket->saUsers[i].iScore ) );
			}
		}
	}
}

void CActionFieldHandler::HandlePacket( PacketActionFieldExchange * psPacket )
{
	if ( psPacket->bSuccess )
		GetChestWindow()->Close();

	pcChestWindow->EnableSlots();
}

void CActionFieldHandler::HandlePacket( PacketActionFieldStart * psPacket )
{
}

void CActionFieldHandler::HandlePacket( PacketActionFieldEnd * psPacket )
{
	PARTICLEENGINE->Kill( PARTICLEID_GHOSTCASTLEPORTAL );
	DROPITEMAREAHANDLER->KillCircle( DROPAREACIRCLEID_ForestDungeon );
}

void CActionFieldHandler::ProcessPacket( PacketActionFieldExchange * psPacket )
{
	psPacket->iLength = sizeof( PacketActionFieldExchange );
	psPacket->iHeader = PKTHDR_ActionFieldExchange;
	psPacket->bSuccess = FALSE;
	SENDPACKETL( psPacket, TRUE );
}

void CActionFieldHandler::ProcessPacket( PacketActionFieldRequest * psPacket )
{
	psPacket->iLength = sizeof( PacketActionFieldRequest );
	psPacket->iHeader = PKTHDR_ActionFieldNPCEnter;
	SENDPACKETG( psPacket );
}

void CActionFieldHandler::Init()
{
	pcChestWindow->Init();
	pcRequestWindow->Init();

	ACTIONFIELD_CAMERAX = (5 * 0x35E0E) + 0x00189C00;

	BuildWindow();

	pcaImageRound[0] = UI::ImageLoader::LoadImage( "game\\images\\events\\ghostcastle\\round1.png" );
	pcaImageRound[1] = UI::ImageLoader::LoadImage( "game\\images\\events\\ghostcastle\\round2.png" );
	pcaImageRound[2] = UI::ImageLoader::LoadImage( "game\\images\\events\\ghostcastle\\round3.png" );
	pcaImageRound[3] = UI::ImageLoader::LoadImage( "game\\images\\events\\ghostcastle\\round4.png" );
	pcaImageRound[4] = UI::ImageLoader::LoadImage( "game\\images\\events\\ghostcastle\\round5.png" );
}

void CActionFieldHandler::Render()
{
	pcChestWindow->Render();
	pcRequestWindow->Render();

	//Is Action Field Map?
	if ( IsInActionFieldMap( MAP_ID ) )
	{
		//Render
		pGhostCastleInfo->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );

		//Show?
		if ( bShowImageRound )
		{
			UI::Image * pc = pcaImageRound[iRound - 1];
			if ( pc )
			{
				int iX = (RESOLUTION_WIDTH >> 1) - (pc->GetWidth() >> 1);
				int iY = (RESOLUTION_HEIGHT >> 2) - (pc->GetHeight() >> 1) + 16;
				UI::ImageRender::Render( pc, iX, iY, pc->GetWidth(), pc->GetHeight(), D3DCOLOR_RGBA( 255, 255, 255, iAlphaRound ) );
			}
		}
	}
}

void CActionFieldHandler::Update( float fTime )
{
	if ( bShowImageRound )
	{
		fTimeImageRound += fTime;

		if ( fTimeImageRound < 500.0f )
		{
			float f = X3D::easeInQuadf( 0.0f, 500.0f, fTimeImageRound );
			iAlphaRound = (int)((f * 255.0f) / 500.0f);
		}
		else if ( fTimeImageRound >= 1500.0f )
		{
			float f = X3D::easeOutQuadf( 0.0f, 500.0f, fTimeImageRound - 1500.0f );
			iAlphaRound = 255 - (int)((f * 255.0f) / 500.0f);
		}

		if ( fTimeImageRound >= 2000.0f )
			bShowImageRound = FALSE;
	}
}

void CActionFieldHandler::OnResolutionChanged()
{
	pGhostCastleInfo->SetPosition( (RESOLUTION_WIDTH >> 1) - (262 >> 1), 85 );
	pcChestWindow->OnResolutionChanged();
	pcRequestWindow->OnResolutionChanged();
}

void CActionFieldHandler::OnMouseMove( CMouse * pcMouse )
{
	pcChestWindow->OnMouseMove( pcMouse );
	pcRequestWindow->OnMouseMove( pcMouse );
}

BOOL CActionFieldHandler::OnMouseClick( CMouse * pcMouse )
{
	if ( pcChestWindow->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcRequestWindow->OnMouseClick( pcMouse ) )
		return TRUE;

	return FALSE;
}

BOOL CActionFieldHandler::IsInActionFieldMap( EMapID iMapID )
{
	return ((iMapID == MAPID_DistortedForest) || 
		    (iMapID == MAPID_Swamp) || 
		    (iMapID == MAPID_GhostCastle) ||
			(iMapID == MAPID_DragonsDungeon) ||
			(iMapID == MAPID_ForestDungeon)) ? TRUE : FALSE;
}

int CActionFieldHandler::GetXRange()
{
	int iX = ((iRound > 0 ? iRound : 1) * 0x35E0E);

	BaseMap * pcBaseMap = MAPGAME->GetBaseMapGame() + MAP_ID;
	if ( pcBaseMap )
		if ( IsInActionFieldMap( MAP_ID ) )
			iX += (pcBaseMap->GetCenter().iX - 319) << 8;

	return iX;
}

void CActionFieldHandler::Clear()
{
	UI::Text_ptr pTextName = pGhostCastleInfo->GetElement<UI::Text>( TEXTID_Name );
	pTextName->SetText( "" );

	UI::Text_ptr pTextRound = pGhostCastleInfo->GetElement<UI::Text>( TEXTID_Round );
	pTextRound->SetText( "" );

	UI::Text_ptr pTextTime = pGhostCastleInfo->GetElement<UI::Text>( TEXTID_Time );
	pTextTime->SetText( "" );

	iRound = 0;

	ACTIONFIELD_CAMERAX = (iRound * 0x35E0E) + 0x00189C00;

	//Update Users
	for ( int i = 0; i < 3; i++ )
	{
		UI::Text_ptr pName = pGhostCastleInfo->GetElement<UI::Text>( TEXTID_PlayerNameOne + i );
		UI::Text_ptr pKills = pGhostCastleInfo->GetElement<UI::Text>( TEXTID_PlayerKillsOne + i );
		UI::Text_ptr pPoints = pGhostCastleInfo->GetElement<UI::Text>( TEXTID_PlayerPointsOne + i );
		if ( pName && pKills && pPoints )
		{
			pName->SetText( "" );
			pKills->SetText( "" );
			pPoints->SetText( "" );
		}
	}
}

EMapID CActionFieldHandler::GetMapID( EActionFieldModeID iModeID )
{
	if ( iModeID == ACTIONFIELDID_Distorted )
		return MAPID_DistortedForest;
	else if ( iModeID == ACTIONFIELDID_Swamp )
		return MAPID_Swamp;
	else if ( (iModeID == ACTIONFIELDID_GhostCastle1) || (iModeID == ACTIONFIELDID_GhostCastle2) )
		return MAPID_GhostCastle;

	return MAPID_Invalid;
}

void CActionFieldHandler::AddParticleNextRound()
{
	bool bAdd = false;
	Point3D sPosition = UNITDATA->sPosition;
	if ( MAP_ID == MAPID_GhostCastle )
	{
		sPosition.iY = 875 << 8;
		sPosition.iZ = 36757 << 8;

		bAdd = true;
	}

	if ( bAdd )
	{
		CParticleScript * pc = PARTICLEFACTORY->LoadScript( "game\\scripts\\particles\\GhostCastleNextRound.lua" );
		pc->SetOwner( new Engine::ObjectOwnerPosition( Point3D( sPosition.iX, sPosition.iY, sPosition.iZ ) ) );
		PARTICLEENGINE->AddScript( pc );
	}
}

void CActionFieldHandler::AddParticleLimitRange()
{
	Point3D sPosition = Point3D( 0, 0, 0 );
	if ( MAP_ID == MAPID_GhostCastle )
	{
		sPosition.iX = GetXRange() + (8 << 8);
		sPosition.iY = 875 << 8;
		sPosition.iZ = 36755 << 8;

		PARTICLEENGINE->Kill( PARTICLEID_GHOSTCASTLEPORTAL );
		CParticleScript * pc = PARTICLEFACTORY->LoadScript( "game\\scripts\\particles\\GhostCastlePortal.lua" );
		pc->SetID( PARTICLEID_GHOSTCASTLEPORTAL );
		pc->SetOwner( new Engine::ObjectOwnerPosition( Point3D( sPosition.iX, sPosition.iY, sPosition.iZ ) ) );
		PARTICLEENGINE->AddScript( pc );
	}
	else if ( MAP_ID == MAPID_ForestDungeon )
	{
		int iRange = 1400;
		if ( iRound == 1 )
		{
			sPosition.iX = -110128 << 8;
			sPosition.iY = 1400 << 8;
			sPosition.iZ = 28400 << 8;
		}
		else if ( iRound == 2 )
		{
			sPosition.iX = -112911 << 8;
			sPosition.iY = 1321 << 8;
			sPosition.iZ = 28263 << 8;

			iRange = 800;
		}
		else
		{
			sPosition.iX = -113066 << 8;
			sPosition.iY = 1546 << 8;
			sPosition.iZ = 30813 << 8;
			iRange = 1000;
		}

		DROPITEMAREAHANDLER->KillCircle( DROPAREACIRCLEID_ForestDungeon );
		DROPITEMAREAHANDLER->AddCircle( DROPAREACIRCLEID_ForestDungeon, sPosition, iRange, true );
	}
}

void CActionFieldHandler::BuildWindow()
{
	pGhostCastleInfo = std::make_shared<UI::Window>( Rectangle2D( (RESOLUTION_WIDTH >> 1) - (390 >> 1), 85, 390, 180 ) );

	//Name
	UI::Text_ptr pTextName = std::make_shared<UI::Text>( Rectangle2D( 0, 0, 390, 22 ) );
	pTextName->SetID( TEXTID_Name );
	pTextName->SetFont( "Frutiger LT 55 Roman", 24, 0, FALSE, TRUE, D3DCOLOR_XRGB( 255, 255, 255 ) );
	pTextName->SetText( "Ghost Castle" );
	pTextName->SetHorizontalAlign( EAlign::ALIGN_Center );
	pGhostCastleInfo->AddElement( pTextName );

	//Round
	UI::Text_ptr pTextRound = std::make_shared<UI::Text>( Rectangle2D( 40, 28, 350, 28 ) );
	pTextRound->SetID( TEXTID_Round );
	pTextRound->SetFont( "Frutiger LT 55 Roman", 28, 0, FALSE, TRUE, D3DCOLOR_XRGB( 30, 255, 30 ) );
	pTextRound->SetText( "Round 1" );
	pTextRound->SetHorizontalAlign( EAlign::ALIGN_Left );
	pGhostCastleInfo->AddElement( pTextRound );

	//Time
	UI::Text_ptr pTextTime = std::make_shared<UI::Text>( Rectangle2D( 0, 28, 350, 28 ) );
	pTextTime->SetID( TEXTID_Time );
	pTextTime->SetFont( "Frutiger LT 55 Roman", 28, 0, FALSE, TRUE, D3DCOLOR_XRGB( 255, 255, 30 ) );
	pTextTime->SetText( "Time Left: 23:59" );
	pTextTime->SetHorizontalAlign( EAlign::ALIGN_Right );
	pGhostCastleInfo->AddElement( pTextTime );

	//User 1
	{
		UI::Text_ptr pTextUserPlaceOne = std::make_shared<UI::Text>( Rectangle2D( 0, 60, 32, 22 ) );
		pTextUserPlaceOne->SetFont( "Frutiger LT 55 Roman", 22, 0, FALSE, FALSE, D3DCOLOR_XRGB( 255, 255, 255 ) );
		pTextUserPlaceOne->SetText( "#1" );
		pGhostCastleInfo->AddElement( pTextUserPlaceOne );

		UI::Text_ptr pTextUserNameOne = std::make_shared<UI::Text>( Rectangle2D( 32, 60, 224, 22 ) );
		pTextUserNameOne->SetID( TEXTID_PlayerNameOne );
		pTextUserNameOne->SetFont( "Frutiger LT 55 Roman", 22, 0, FALSE, FALSE, D3DCOLOR_XRGB( 255, 255, 255 ) );
		pTextUserNameOne->SetText( "" );
		pGhostCastleInfo->AddElement( pTextUserNameOne );

		UI::Text_ptr pTextUserKillsOne = std::make_shared<UI::Text>( Rectangle2D( 256, 60, 40, 22 ) );
		pTextUserKillsOne->SetID( TEXTID_PlayerKillsOne );
		pTextUserKillsOne->SetFont( "Frutiger LT 55 Roman", 22, 0, FALSE, FALSE, D3DCOLOR_XRGB( 255, 255, 255 ) );
		pTextUserKillsOne->SetText( "" );
		pGhostCastleInfo->AddElement( pTextUserKillsOne );

		UI::Text_ptr pTextUserPointsOne = std::make_shared<UI::Text>( Rectangle2D( 296, 60, 94, 22 ) );
		pTextUserPointsOne->SetID( TEXTID_PlayerPointsOne );
		pTextUserPointsOne->SetFont( "Frutiger LT 55 Roman", 22, 0, FALSE, FALSE, D3DCOLOR_XRGB( 255, 255, 255 ) );
		pTextUserPointsOne->SetText( "" );
		pGhostCastleInfo->AddElement( pTextUserPointsOne );
	}

	//User 2
	{
		UI::Text_ptr pTextUserPlaceTwo = std::make_shared<UI::Text>( Rectangle2D( 0, 86, 32, 22 ) );
		pTextUserPlaceTwo->SetFont( "Frutiger LT 55 Roman", 22, 0, FALSE, FALSE, D3DCOLOR_XRGB( 255, 255, 255 ) );
		pTextUserPlaceTwo->SetText( "#2" );
		pGhostCastleInfo->AddElement( pTextUserPlaceTwo );

		UI::Text_ptr pTextUserNameTwo = std::make_shared<UI::Text>( Rectangle2D( 32, 86, 224, 22 ) );
		pTextUserNameTwo->SetID( TEXTID_PlayerNameTwo );
		pTextUserNameTwo->SetFont( "Frutiger LT 55 Roman", 22, 0, FALSE, FALSE, D3DCOLOR_XRGB( 255, 255, 255 ) );
		pTextUserNameTwo->SetText( "" );
		pGhostCastleInfo->AddElement( pTextUserNameTwo );

		UI::Text_ptr pTextUserKillsTwo = std::make_shared<UI::Text>( Rectangle2D( 256, 86, 40, 22 ) );
		pTextUserKillsTwo->SetID( TEXTID_PlayerKillsTwo );
		pTextUserKillsTwo->SetFont( "Frutiger LT 55 Roman", 22, 0, FALSE, FALSE, D3DCOLOR_XRGB( 255, 255, 255 ) );
		pTextUserKillsTwo->SetText( "" );
		pGhostCastleInfo->AddElement( pTextUserKillsTwo );

		UI::Text_ptr pTextUserPointsTwo = std::make_shared<UI::Text>( Rectangle2D( 296, 86, 94, 22 ) );
		pTextUserPointsTwo->SetID( TEXTID_PlayerPointsTwo );
		pTextUserPointsTwo->SetFont( "Frutiger LT 55 Roman", 22, 0, FALSE, FALSE, D3DCOLOR_XRGB( 255, 255, 255 ) );
		pTextUserPointsTwo->SetText( "" );
		pGhostCastleInfo->AddElement( pTextUserPointsTwo );
	}

	//User 3
	{
		UI::Text_ptr pTextUserPlaceThree = std::make_shared<UI::Text>( Rectangle2D( 0, 112, 32, 22 ) );
		pTextUserPlaceThree->SetFont( "Frutiger LT 55 Roman", 22, 0, FALSE, FALSE, D3DCOLOR_XRGB( 255, 255, 255 ) );
		pTextUserPlaceThree->SetText( "#3" );
		pGhostCastleInfo->AddElement( pTextUserPlaceThree );

		UI::Text_ptr pTextUserNameThree = std::make_shared<UI::Text>( Rectangle2D( 32, 112, 224, 22 ) );
		pTextUserNameThree->SetID( TEXTID_PlayerNameThree );
		pTextUserNameThree->SetFont( "Frutiger LT 55 Roman", 22, 0, FALSE, FALSE, D3DCOLOR_XRGB( 255, 255, 255 ) );
		pTextUserNameThree->SetText( "" );
		pGhostCastleInfo->AddElement( pTextUserNameThree );

		UI::Text_ptr pTextUserKillsThree = std::make_shared<UI::Text>( Rectangle2D( 256, 112, 40, 22 ) );
		pTextUserKillsThree->SetID( TEXTID_PlayerKillsThree );
		pTextUserKillsThree->SetFont( "Frutiger LT 55 Roman", 22, 0, FALSE, FALSE, D3DCOLOR_XRGB( 255, 255, 255 ) );
		pTextUserKillsThree->SetText( "" );
		pGhostCastleInfo->AddElement( pTextUserKillsThree );

		UI::Text_ptr pTextUserPointsThree = std::make_shared<UI::Text>( Rectangle2D( 296, 112, 94, 22 ) );
		pTextUserPointsThree->SetID( TEXTID_PlayerPointsThree );
		pTextUserPointsThree->SetFont( "Frutiger LT 55 Roman", 22, 0, FALSE, FALSE, D3DCOLOR_XRGB( 255, 255, 255 ) );
		pTextUserPointsThree->SetText( "" );
		pGhostCastleInfo->AddElement( pTextUserPointsThree );
	}

	//pGhostCastleInfo->Hide();
}
