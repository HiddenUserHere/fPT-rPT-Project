#include "stdafx.h"
#include "CMiniMapHandler.h"

CMiniMapHandler::CMiniMapHandler()
{
	bSelected	= FALSE;
	sPointMouse = Point2D();
}

CMiniMapHandler::~CMiniMapHandler()
{
}

void CMiniMapHandler::Init()
{
	pcBattleRoyaleBackground = UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\minimap\\broyale.tga" );
	pcMaskBattleRoyaleBackground = UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\minimap\\mask_broyale.png" );
	pcMaskBattleRoyaleMeteorite = UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\minimap\\meteorite_broyale.png" );
	pcMaskBackground = UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\minimap\\mask_minimap.tga" );
	pcMaskBackgroundActive = UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\minimap\\mask_activeminimap.tga" );
	pcBorderImage = UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\minimap\\border.png" );
	pcUserPoint = UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\minimap\\arrow.png" );
	pcNPCPoint = UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\minimap\\npc.png" );
	pcPKChar = UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\minimap\\pk_char.png" );
	pEffectMask = GetGraphicsDevice()->GetEffectFactory()->MakeEffect( "game\\scripts\\shaders\\UI\\Minimap.fx" );
	pRenderTargetMask = GetGraphicsDevice()->MakeRenderTarget( 128, MINIMAP_SIZE, MINIMAP_SIZE );
	pcImageRaidIcon = UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\party\\raidmember.png" );

	pTooltipUnit = std::make_shared<UI::Tooltip>( Rectangle2D() );
	pTooltipUnit->Init();
	pTooltipUnit->SetFont( "Arial", 16, -1 );
	pTooltipUnit->SetPosition( TOOLTIPPOSITION_Top );
	pTooltipUnit->SetFollowMouse( FALSE );
	pTooltipUnit->SetHover( TRUE );
	pTooltipUnit->Open();

	pcImageEnemyIcon = UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\minimap\\enemy.png" );
	pcImageFriendlyIcon = UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\minimap\\friendly.png" );
	pcImageNextBoss = UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\minimap\\nextboss.png" );

	pFontMap = GetGraphicsDevice()->GetFontFactory()->MakeFont( nullptr, "Arial", 14, 0, FALSE, FALSE );

	for( size_t i = 0; i < MAX_PARTY_MEMBERS; i++ )
		pcaImagesPartyMemberIcon[i] = UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\party\\member%d.png", i + 1 );

	SetMiniMapPostion( Point2D( ( RESOLUTION_WIDTH - MINIMAP_SIZE ) - 16, ( RESOLUTION_HEIGHT - MINIMAP_SIZE ) - 50 ) );
}

void CMiniMapHandler::Render()
{
	if( Game::GetGameMode() == GAMEMODE_InGame && ISSHOW_MINIMAP && (MAP_ID != MAPID_ChessRoom && MAP_ID != MAPID_QuestArena && MAP_ID != MAPID_GhostCastle && MAP_ID != MAPID_ForestDungeon && MAP_ID != MAPID_DragonsDungeon) )
	{
		bIsHoverUnit = FALSE;

		//New Render Target to render the minimap
		if ( GetGraphicsDevice()->SetRenderTarget( pRenderTargetMask ) )
		{
			//Clear Render Target
			GetGraphicsDevice()->Clear( TRUE, TRUE, FALSE );

			//Render Minimaps
			RenderMap( UNIT->GetPosition() );

			//Back Old Render Target
			GetGraphicsDevice()->UnsetRenderTarget();
		}

		RenderEffect();

		//Render user Point
		RenderUser( UNITDATA->sAngle );

		//Render Border
		if( bActive || SETTINGSHANDLER->GetModel()->GetSettings().bMinimapAlwaysActive )
		{
			UI::ImageRender::Render( pcBorderImage, GetMiniMapPostion().iX - 9, GetMiniMapPostion().iY - 24, 144, 158, -1 );
			
			if ( UNITDATA->iLoadedMapIndex >= 0 )
			{
				if( GetMiniMapData( UNITDATA->iLoadedMapIndex ) )
					UI::ImageRender::Render( GetMiniMapData( UNITDATA->iLoadedMapIndex )->psTextureTitle->pcD3DTexture, GetMiniMapPostion().iX - 1, GetMiniMapPostion().iY - GetMiniMapData( UNITDATA->iLoadedMapIndex )->psTextureTitle->iHeight, GetMiniMapData( UNITDATA->iLoadedMapIndex )->psTextureTitle->iWidth, GetMiniMapData( UNITDATA->iLoadedMapIndex )->psTextureTitle->iHeight, 0, 0, -1, FALSE );
			}
			else
			{
				if( GetMiniMapData( 0 ) )
					UI::ImageRender::Render( GetMiniMapData( 0 )->psTextureTitle->pcD3DTexture, GetMiniMapPostion().iX - 1, GetMiniMapPostion().iY - GetMiniMapData( 0 )->psTextureTitle->iHeight, GetMiniMapData( 0 )->psTextureTitle->iWidth, GetMiniMapData( 0 )->psTextureTitle->iHeight, 0, 0, -1, FALSE );
			}

			if( bShowBossTime && SETTINGSHANDLER->GetModel()->GetSettings().bShowNextBossTimer )
			{
				int iHour	= (dwNextBossTime / 3600) % 60;
				int iMinute = (dwNextBossTime / 60) % 60;
				int iSecond = dwNextBossTime % 60;

				UI::ImageRender::Render( pcImageNextBoss, GetMiniMapPostion().iX - 5, GetMiniMapPostion().iY - 44, 130, 23, -1 );
				//FONTHANDLER->Draw( iFontID, GetMiniMapPostion().iX + 20, GetMiniMapPostion().iY - 40, FormatString( "Next Boss: %02d:%02d:%02d", iHour, iMinute, iSecond ), DT_NOCLIP, D3DCOLOR_XRGB( 255, 180, 180 ) );
				pFontMap->Draw( FormatString( "Next Boss: %02d:%02d:%02d", iHour, iMinute, iSecond ), GetMiniMapPostion().iX + 20, GetMiniMapPostion().iY - 40, DT_NOCLIP, DX::Color( D3DCOLOR_XRGB( 255, 180, 180 ) ) );
			}
		}

		if( bIsHoverUnit )
		{
			pTooltipUnit->SetText( strHoverUnit );
			pTooltipUnit->SetTextColor( cHoverColor );
			pTooltipUnit->Render( sHoverUnitBox.iX, sHoverUnitBox.iY, 0, 0, 0, 0 );
		}
	}
}

void CMiniMapHandler::Update()
{
	dwTickTime++;
	
	//1 Second?
	if ( (dwTickTime % 10) == 0 )
	{
		//Decrease Boss Time
		if ( dwNextBossTime > 0 )
		{
			dwNextBossTime--;
			//Boss Time End? Hide.
			if ( dwNextBossTime == 0 )
			{
				bShowBossTime = FALSE;
				
				//Get new Boss Time
				ProcessPacket( &PacketBossTimeUpdate( MAP_ID ) );
			}
		}
	}

	BATTLEROYALEHANDLER->GetWindowMinimapHUD()->SetPosition( GetMiniMapPostion().iX, GetMiniMapPostion().iY - BATTLEROYALEHANDLER->GetWindowMinimapHUD()->GetHeight() - 42 );
}

BOOL CMiniMapHandler::OnMouseClick( CMouse * pcMouse )
{
	BOOL bRet = FALSE;

	if( bSelected )
	{
		if( pcMouse->GetEvent() == ClickUpL )
			bSelected = FALSE;
	}
	else if( !bSelected && !SETTINGSHANDLER->GetModel()->GetSettings().bLockUI && pcMouse->GetEvent() == ClickDownL && pcMouse->GetPosition()->Inside( Rectangle2D( GetMiniMapPostion().iX, GetMiniMapPostion().iY - 16, MINIMAP_SIZE, MINIMAP_SIZE + 16 ) ) )
	{
		int iExtraAdd = 0;
		bSelected = TRUE;

		if( pcMouse->GetPosition()->Inside( Rectangle2D( GetMiniMapPostion().iX, GetMiniMapPostion().iY - 16, MINIMAP_SIZE, 16 ) ) )
			iExtraAdd += 15;

		sPointMouse.iX = abs( pcMouse->GetPosition()->iX - GetMiniMapPostion().iX );
		sPointMouse.iY = abs( pcMouse->GetPosition()->iY - GetMiniMapPostion().iY ) - iExtraAdd;
		bRet = TRUE;
	}

	if( bActive && !bSelected )
		bRet = TRUE;

	return bRet;
}

void CMiniMapHandler::OnMouseMove( CMouse * pcMouse )
{
	if( pcMouse->GetPosition()->Inside( Rectangle2D( GetMiniMapPostion().iX, GetMiniMapPostion().iY - 16, MINIMAP_SIZE, MINIMAP_SIZE + 16 ) ) )
		bActive = TRUE;
	else
		bActive = FALSE;

	if( bSelected && (MAP_ID != MAPID_ChessRoom && MAP_ID != MAPID_QuestArena) )
	{
		int iX = pcMouse->GetPosition()->iX - sPointMouse.iX;
		int iY = pcMouse->GetPosition()->iY - sPointMouse.iY;

		SetMiniMapPostion( Point2D(iX < 0 ? 0 : iX, iY < 0 ? 0 : iY) );

		BATTLEROYALEHANDLER->GetWindowMinimapHUD()->SetPosition( GetMiniMapPostion().iX, GetMiniMapPostion().iY - BATTLEROYALEHANDLER->GetWindowMinimapHUD()->GetHeight() - 42 );
	}
}

void CMiniMapHandler::RenderEffect()
{
	UI::ImageRender::SetEffect( pEffectMask );
	pEffectMask->SetTechnique( "MaskScale" );

	if ( pEffectMask->Begin() >= 0 )
	{
		//Set Texture Mask Sampler
		D3DXHANDLE hTexture = pEffectMask->Get()->GetParameterByName( 0, "MaskTexture" );

		pEffectMask->Get()->SetTexture( hTexture, (bActive || SETTINGSHANDLER->GetModel()->GetSettings().bMinimapAlwaysActive) ? pcMaskBackgroundActive->GetTexture() : pcMaskBackground->GetTexture() );

		//Render the Render Target and mask it on effect
		UI::ImageRender::Render( pRenderTargetMask->GetTexture(), GetMiniMapPostion().iX, GetMiniMapPostion().iY, pRenderTargetMask->GetWidth(), pRenderTargetMask->GetHeight(), 0, 0, D3DCOLOR_XRGB( 255, 255, 255 ), FALSE );

		pEffectMask->End();
	}

	UI::ImageRender::SetEffect( NULL );
}

void CMiniMapHandler::RenderMap( Point3D sUserPosition )
{
	float fMapSizeScale = 15.0f - ((MINIMAP_TEXTURE_SCALE * 10.0f) / 100.0f);

	//Render mask to the Render Target
	SetMinimapSize( MINIMAP_SIZE );

	int iMinimapSize = 256 * 256 * (int)fMapSizeScale;

	//Draw MiniMap
	if ( GetMiniMapData( 1 ) )
		DrawMinimap( GetMiniMapData( 1 ), 0, 0, iMinimapSize );

	if ( GetMiniMapData( 0 ) )
		DrawMinimap( GetMiniMapData( 0 ), 0, 0, iMinimapSize );

	//Units
	for ( int i = 0; i < MAX_UNITS; i++ )
	{
		UnitData * pc = UNITGAME->pcaUnitData + i;

		if ( pc->bActive )
		{
			if ( SETTINGSHANDLER->GetModel()->GetSettings().bShowNPCs )
				if ( RenderNPC( pc, sUserPosition, iMinimapSize ) )
					continue;

			if ( RenderPlayer( pc, sUserPosition, UNITDATA->sCharacterData.iClanID, iMinimapSize ) )
				continue;
		}
	}

	RenderParty( sUserPosition, UNITDATA->iID, iMinimapSize );

	if ( BATTLEROYALEHANDLER->InBattle() )
	{
		float fRadiusScale		= (float)(BATTLEROYALEHANDLER->GetState().iRadius) / (float)((MINIMAP_TEXTURE_SCALE * MINIMAP_TEXTURE_SCALE) >> 3);
		float fNextRadiusScale	= (float)(BATTLEROYALEHANDLER->GetState().iNextRadius) / (float)((MINIMAP_TEXTURE_SCALE * MINIMAP_TEXTURE_SCALE) >> 3);

		Point2D sPosition = GetMinimapPositionFromCenter( sUserPosition, BATTLEROYALEHANDLER->GetState().sPosition, 1, 1, iMinimapSize );

		BOOL bInsideRadius = BATTLEROYALEHANDLER->IsInsideRadius( sUserPosition );

		UI::ImageRender::Render( pcMaskBattleRoyaleBackground->GetTexture(), sPosition.iX - (int)(34.0f * fRadiusScale), sPosition.iY - (int)(34.0f * fRadiusScale),
			pcMaskBattleRoyaleBackground->GetWidth(), pcMaskBattleRoyaleBackground->GetHeight(), 0, 0,
			bInsideRadius ? D3DCOLOR_XRGB( 0, 160, 255 ) : D3DCOLOR_XRGB( 70, 70, 255 ), fRadiusScale, fRadiusScale, 0.0f );

		UI::ImageRender::Render( pcMaskBattleRoyaleBackground->GetTexture(), sPosition.iX - (int)(34.0f * fNextRadiusScale), sPosition.iY - (int)(34.0f * fNextRadiusScale),
			pcMaskBattleRoyaleBackground->GetWidth(), pcMaskBattleRoyaleBackground->GetHeight(), 0, 0, D3DCOLOR_XRGB( 255, 255, 255 ), fNextRadiusScale, fNextRadiusScale, 0.0f );

		//Have Meteorite?
		if ( BATTLEROYALEHANDLER->GetState().iMeteoriteTime > 0 )
		{
			float fMeteoriteScale = (float)(RADIUS_BATTLEROYALE_METEORITE) / (float)((MINIMAP_TEXTURE_SCALE * MINIMAP_TEXTURE_SCALE) >> 3);

			Point2D sPosition = GetMinimapPositionFromCenter( sUserPosition, BATTLEROYALEHANDLER->GetState().sMeteoritePosition, 1, 1, iMinimapSize );

			UI::ImageRender::Render( pcMaskBattleRoyaleMeteorite->GetTexture(), sPosition.iX - (int)(33.5f * fMeteoriteScale), sPosition.iY - (int)(33.5f * fMeteoriteScale),
				pcMaskBattleRoyaleMeteorite->GetWidth(), pcMaskBattleRoyaleMeteorite->GetHeight(), 0, 0, D3DCOLOR_ARGB( 76, 255, 0, 0 ), fMeteoriteScale, fMeteoriteScale, 0.0f );
		}
	}

	SetMinimapSize( 128 );

	if ( bUIDebug )
	{
		GRAPHICENGINE->DrawRectangle2D( Rectangle2D( GetMiniMapPostion().iX, GetMiniMapPostion().iY, MINIMAP_SIZE, 1 ), -1 );
		GRAPHICENGINE->DrawRectangle2D( Rectangle2D( GetMiniMapPostion().iX, GetMiniMapPostion().iY + MINIMAP_SIZE - 1, MINIMAP_SIZE, 1 ), -1 );
		GRAPHICENGINE->DrawRectangle2D( Rectangle2D( GetMiniMapPostion().iX, GetMiniMapPostion().iY, 1, MINIMAP_SIZE ), -1 );
		GRAPHICENGINE->DrawRectangle2D( Rectangle2D( GetMiniMapPostion().iX + MINIMAP_SIZE - 1, GetMiniMapPostion().iY, 1, MINIMAP_SIZE ), -1 );
	}
}

void CMiniMapHandler::RenderUser( Point3D sUserAngle )
{
	//Angle of User Point
	int iAngle = ((sUserAngle.iY * 360) / PTANGLE_360) + 180;
	UI::ImageRender::Render( pcUserPoint, GetMiniMapPostion().iX + (MINIMAP_SIZE >> 1) - 8, GetMiniMapPostion().iY + (MINIMAP_SIZE >> 1) - 8, pcUserPoint->GetWidth(), pcUserPoint->GetHeight(), 0, 0, -1, 0.7f, (float)iAngle );
}

BOOL CMiniMapHandler::RenderNPC( UnitData * pcUnitData, Point3D sUserPosition, int iSize )
{
	if ( pcUnitData->sCharacterData.iType == CHARACTERTYPE_NPC )
	{
		if ( pcUnitData->sPosition.WithinPTDistance( &sUserPosition, DISTANCE_UNIT_VIEWLIMIT ) )
		{
			Point2D sPosition = GetMinimapPositionFromCenter( sUserPosition, pcUnitData->sPosition, pcNPCPoint->GetWidth() >> 1, pcNPCPoint->GetHeight() >> 1, iSize );
			Rectangle2D sBox = Rectangle2D( sPosition.iX + GetMiniMapPostion().iX, sPosition.iY + GetMiniMapPostion().iY, pcNPCPoint->GetWidth(), pcNPCPoint->GetHeight() );

			if ( sPosition.iX < (MINIMAP_SIZE - 10) )
				if ( sPosition.iY < (MINIMAP_SIZE - 10) )
					UI::ImageRender::Render( pcNPCPoint, sPosition.iX, sPosition.iY, pcNPCPoint->GetWidth(), pcNPCPoint->GetHeight(), 0, 0, -1 );

			if( MOUSEHANDLER->GetPosition()->Inside(sBox) )
			{
				bIsHoverUnit = TRUE;
				sHoverUnitBox = sBox;
				strHoverUnit = pcUnitData->sCharacterData.szName;
				cHoverColor = UI::Color( 255, 255, 255, 255 );
			}
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CMiniMapHandler::RenderPlayer( UnitData * pcUnitData, Point3D sUserPosition, int iUserClanID, int iSize )
{
	if ( BATTLEROYALEHANDLER->InWaiting() || BATTLEROYALEHANDLER->InBattle() )
		return TRUE;

	if ( Unit * pcUnit = UNITDATATOUNIT( pcUnitData ) )
	{
		if ( pcUnitData->iCharacterType == CHARACTERTYPE_Player || pcUnitData->sCharacterData.iType == CHARACTERTYPE_Player )
		{
			//Allied Clan
			if ( pcUnitData->sCharacterData.iClanID > 0 && pcUnitData->sCharacterData.iClanID == iUserClanID && SETTINGSHANDLER->GetModel()->GetSettings().bShowClanMembers )
			{
				if ( pcUnitData->sPosition.WithinPTDistance( &sUserPosition, DISTANCE_UNIT_VIEWLIMIT ) )
				{
					Point2D sPosition = GetMinimapPositionFromCenter( sUserPosition, pcUnitData->sPosition, pcImageFriendlyIcon->GetWidth() >> 1, pcImageFriendlyIcon->GetHeight() >> 1, iSize );
					Rectangle2D sBox = Rectangle2D( sPosition.iX + GetMiniMapPostion().iX, sPosition.iY + GetMiniMapPostion().iY, pcImageFriendlyIcon->GetWidth(), pcImageFriendlyIcon->GetHeight() );


					if ( sPosition.iX < (MINIMAP_SIZE - 10) )
						if ( sPosition.iY < (MINIMAP_SIZE - 10) )
							UI::ImageRender::Render( pcImageFriendlyIcon, sPosition.iX, sPosition.iY, pcImageFriendlyIcon->GetWidth(), pcImageFriendlyIcon->GetHeight(), 0, 0, -1 );

					if ( MOUSEHANDLER->GetPosition()->Inside( sBox ) )
					{
						bIsHoverUnit = TRUE;
						sHoverUnitBox = sBox;
						strHoverUnit = pcUnitData->sCharacterData.szName;
						cHoverColor = UI::Color( 255, 255, 245, 245 );
					}
				}
			}
			else if ( pcUnit->bPvPMode && SETTINGSHANDLER->GetModel()->GetSettings().bShowEnemies )
			{
				if ( pcUnitData->sPosition.WithinPTDistance( &sUserPosition, DISTANCE_UNIT_VIEWLIMIT ) )
				{
					Point2D sPosition = GetMinimapPositionFromCenter( sUserPosition, pcUnitData->sPosition, pcPKChar->GetWidth() >> 1, pcPKChar->GetHeight() >> 1, iSize );
					Rectangle2D sBox = Rectangle2D( sPosition.iX + GetMiniMapPostion().iX, sPosition.iY + GetMiniMapPostion().iY, pcPKChar->GetWidth(), pcPKChar->GetHeight() );

					if ( sPosition.iX < (MINIMAP_SIZE - 10) )
						if ( sPosition.iY < (MINIMAP_SIZE - 10) )
							UI::ImageRender::Render( pcPKChar, sPosition.iX, sPosition.iY, pcPKChar->GetWidth(), pcPKChar->GetHeight(), 0, 0, -1 );

					if ( MOUSEHANDLER->GetPosition()->Inside( sBox ) )
					{
						bIsHoverUnit = TRUE;
						sHoverUnitBox = sBox;
						strHoverUnit = pcUnitData->sCharacterData.szName;
						cHoverColor = UI::Color( 255, 255, 255, 255 );
					}
				}
			}
			else if ( MAP_ID == MAPID_BlessCastle && SETTINGSHANDLER->GetModel()->GetSettings().bShowEnemies )
			{
				//Enemy Clan
				if ( pcUnitData->sPosition.WithinPTDistance( &sUserPosition, DISTANCE_UNIT_VIEWLIMIT ) )
				{
					Point2D sPosition = GetMinimapPositionFromCenter( sUserPosition, pcUnitData->sPosition, pcImageEnemyIcon->GetWidth() >> 1, pcImageEnemyIcon->GetHeight() >> 1, iSize );

					if ( sPosition.iX < (MINIMAP_SIZE - 10) )
						if ( sPosition.iY < (MINIMAP_SIZE - 10) )
							UI::ImageRender::Render( pcImageEnemyIcon, sPosition.iX, sPosition.iY, pcImageEnemyIcon->GetWidth(), pcImageEnemyIcon->GetHeight(), 0, 0, -1 );
				}
			}

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CMiniMapHandler::RenderParty( Point3D sUserPosition, int iUserID, int iSize )
{
	if( PARTYHANDLER->GetPartyData().iMembersCount > 0 && SETTINGSHANDLER->GetModel()->GetSettings().bShowPartyMembers )
	{
		for( int i = 0; i < PARTYHANDLER->GetPartyData().iMembersCount; i++ )
		{
			PartyMember sMember = PARTYHANDLER->GetPartyData().saMembers[i];
			UI::Image * pImage = pcaImagesPartyMemberIcon[i];

			if( sMember.sData.sLevel > 0 && (sMember.sData.iID != iUserID) && pImage )
			{
				Point2D sPosition = GetMinimapPositionFromCenter( sUserPosition, sMember.sData.sPosition, pImage->GetWidth() >> 1, pImage->GetHeight() >> 1, iSize );
				Rectangle2D sBox = Rectangle2D( sPosition.iX + GetMiniMapPostion().iX, sPosition.iY + GetMiniMapPostion().iY, pImage->GetWidth(), pImage->GetHeight() );

				if ( sPosition.iX < (MINIMAP_SIZE - 10) )
					if ( sPosition.iY < (MINIMAP_SIZE - 10) )
						UI::ImageRender::Render( pImage, sPosition.iX, sPosition.iY, pImage->GetWidth(), pImage->GetHeight(), 0, 0, -1 );

				if( MOUSEHANDLER->GetPosition()->Inside( sBox ) )
				{
					bIsHoverUnit = TRUE;
					sHoverUnitBox = sBox;
					strHoverUnit = sMember.szCharacterName;
					cHoverColor = UI::Color( 255, 175, 230, 175 );
				}
			}
		}
	}

	if( PARTYHANDLER->GetPartyData().iRaidCount > 0 && SETTINGSHANDLER->GetModel()->GetSettings().bShowRaidMembers )
	{
		for( int i = 0; i < PARTYHANDLER->GetPartyData().iRaidCount; i++ )
		{
			for( size_t j = 0; j < MAX_PARTY_MEMBERS; j++ )
			{
				PartyRaidMember sMember = PARTYHANDLER->GetPartyData().saRaid[i].saMembers[j];

				if( sMember.sData.iID && sMember.szCharacterName[0] != 0 )
				{
					Point2D sPosition = GetMinimapPositionFromCenter( sUserPosition, sMember.sData.sPosition, pcImageRaidIcon->GetWidth() >> 1, pcImageRaidIcon->GetHeight() >> 1, iSize );
					Rectangle2D sBox = Rectangle2D( sPosition.iX + GetMiniMapPostion().iX, sPosition.iY + GetMiniMapPostion().iY, pcImageRaidIcon->GetWidth(), pcImageRaidIcon->GetHeight() );

					if ( sPosition.iX < (MINIMAP_SIZE - 10) )
						if ( sPosition.iY < (MINIMAP_SIZE - 10) )
							UI::ImageRender::Render( pcImageRaidIcon, sPosition.iX, sPosition.iY, pcImageRaidIcon->GetWidth(), pcImageRaidIcon->GetHeight(), 0, 0, -1 );

					if( MOUSEHANDLER->GetPosition()->Inside(sBox) )
					{
						bIsHoverUnit = TRUE;
						sHoverUnitBox = sBox;
						strHoverUnit = sMember.szCharacterName;
						cHoverColor = UI::Color( 255, 40, 240, 230 );
					}
				}
			}
		}
	}

	return TRUE;
}

BOOL CMiniMapHandler::RenderMapTime()
{
	return TRUE;
}

BOOL CMiniMapHandler::IsCurrentUserMap( MiniMapData * psMap )
{
	if ( UNITDATA->iLoadedMapIndex >= 0 )
	{
		if ( FIELDGAME[UNITDATA->iLoadedMapIndex] == psMap->pcBaseMap )
			return TRUE;
	}

	return FALSE;
}

void CMiniMapHandler::BeginViewport()
{
	RECT r = { GetMiniMapPostion().iX, GetMiniMapPostion().iY, GetMiniMapPostion().iX + MINIMAP_SIZE, GetMiniMapPostion().iY + MINIMAP_SIZE };
	GetGraphicsDevice()->PushScissorRect( r );
}

void CMiniMapHandler::EndViewport()
{
	GetGraphicsDevice()->PopScissorRect();
}

NAKED void CMiniMapHandler::DrawMinimap( MiniMapData * psMap, int iX, int iY, int iSize )
{
	JMP( 0x00460D50 );
}

void CMiniMapHandler::SetMinimapSize( float fSize )
{
	(*(float*)0x04B0DF44) = fSize / 2.0f;
	(*(float*)0x04B0DF48) = fSize - 2.0f;
}

Point2D CMiniMapHandler::GetMinimapPositionFromCenter( Point3D sCenterPosition, Point3D sObjectPosition, int iWidthPoint, int iHeightPoint, int iSize )
{
	//Compute position from center to object
	int iX = sObjectPosition.iX - sCenterPosition.iX;
	int iZ = sObjectPosition.iZ - sCenterPosition.iZ;

	//Divide points to inside of minimap
	iX /= (iSize / (MINIMAP_SIZE / 2));
	iZ /= (iSize / (MINIMAP_SIZE / 2));
	
	//Negative Z, since it will up...
	iZ = -iZ;

	return Point2D( iX + (MINIMAP_SIZE >> 1) - iWidthPoint, iZ + (MINIMAP_SIZE >> 1) - iHeightPoint );
}

void CMiniMapHandler::OnResolutionChanged()
{
	SetMiniMapPostion( Point2D( (RESOLUTION_WIDTH - MINIMAP_SIZE) - 16, (RESOLUTION_HEIGHT - MINIMAP_SIZE) - 50 ) );
}

void CMiniMapHandler::HandlePacket( PacketBossTimeUpdate * psPacket )
{
	strNextBossName = psPacket->szBossName;
	dwNextBossTime	= psPacket->dwTime;
	
	if ( dwNextBossTime )
		bShowBossTime = TRUE;
	else
		bShowBossTime = FALSE;
}

void CMiniMapHandler::ProcessPacket( PacketBossTimeUpdate * psPacket )
{
	psPacket->iLength = sizeof( PacketBossTimeUpdate );
	psPacket->iHeader = PKTHDR_BossTimeUpdate;
	SENDPACKETG( psPacket );
}

void CMiniMapHandler::OnChangeMap( int iMapID )
{
	if ( CHARACTERGAME->GetLastStageID() == MAPID_Invalid )
		ProcessPacket( &PacketBossTimeUpdate( iMapID ) );
}