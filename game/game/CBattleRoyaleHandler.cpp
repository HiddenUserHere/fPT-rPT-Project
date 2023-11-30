#include "stdafx.h"
#include "CBattleRoyaleHandler.h"

using namespace BattleRoyale;


CBattleRoyaleHandler::CBattleRoyaleHandler()
{
	pcWindow = new CBattleRoyaleWindowHandler();

	pWindow = std::make_shared<UI::Window>( Rectangle2D( 100, 150, 200, 64 ) );

	pWindowTop = std::make_shared<UI::Window>( Rectangle2D( 200, 270, 400, 400 ) );
}


CBattleRoyaleHandler::~CBattleRoyaleHandler()
{
	DELET( pcWindow );

	DELETA( pcaItemData );
}

void CBattleRoyaleHandler::Init()
{
	pcWindow->Init();

	OnResolutionChanged();

	//Players
	{
		UI::ImageBox_ptr pImagePlayer = std::make_shared<UI::ImageBox>( Rectangle2D( 2, 0, 32, 32 ) );
		pImagePlayer->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\minimap\\player_broyale.png" ) );
		pImagePlayer->SetScale( 0.75f );
		pWindow->AddElement( pImagePlayer );

		UI::Text_ptr pTextPlayersCount = std::make_shared<UI::Text>( Rectangle2D( 0, 0, 128, 32 ) );
		pTextPlayersCount->SetID( TEXT_PlayersCount );
		pTextPlayersCount->SetHorizontalAlign( ALIGN_Right );
		pTextPlayersCount->SetVerticalAlign( ALIGN_Center );
		pTextPlayersCount->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
		pTextPlayersCount->SetText( "1" );
		pWindow->AddElement( pTextPlayersCount );
	}

	//Gas
	{
		UI::ImageBox_ptr pImageSmoke = std::make_shared<UI::ImageBox>( Rectangle2D( 0, 26, 32, 32 ) );
		pImageSmoke->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\minimap\\smoke_broyale.png" ) );
		pWindow->AddElement( pImageSmoke );

		UI::Text_ptr pTextSmokeTime = std::make_shared<UI::Text>( Rectangle2D( 0, 26, 128, 32 ) );
		pTextSmokeTime->SetID( TEXT_GasTime );
		pTextSmokeTime->SetHorizontalAlign( ALIGN_Right );
		pTextSmokeTime->SetVerticalAlign( ALIGN_Center );
		pTextSmokeTime->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
		pTextSmokeTime->SetText( "01:00" );
		pWindow->AddElement( pTextSmokeTime );
	}

	//State
	{
		UI::Text_ptr pTextState = std::make_shared<UI::Text>( Rectangle2D( (pWindowTop->GetWidth() >> 1) - 50, 10, 100, 100 ) );
		pTextState->SetID( TEXT_State );
		pTextState->SetFont( "Arial", 28, 0, FALSE, TRUE, D3DCOLOR_XRGB( 84, 173, 63 ) );
		pTextState->SetDropdownShadow( TRUE );
		pTextState->SetDropdownColor( D3DCOLOR_ARGB( 200, 0, 0, 0 ) );
		pTextState->SetText( "Waiting for players..." );
		pWindowTop->AddElement( pTextState );
	}

	//Team
	{
		UI::Text_ptr pTextTeam = std::make_shared<UI::Text>( Rectangle2D( (pWindowTop->GetWidth() >> 1) - 50, 32, 100, 100 ) );
		pTextTeam->SetID( TEXT_Team );
		pTextTeam->SetFont( "Arial", 28, 0, FALSE, TRUE, D3DCOLOR_XRGB( 84, 173, 63 ) );
		pTextTeam->SetDropdownShadow( TRUE );
		pTextTeam->SetDropdownColor( D3DCOLOR_ARGB( 200, 0, 0, 0 ) );
		pTextTeam->SetText( "" );
		pWindowTop->AddElement( pTextTeam );
	}

	//Teams Score
	{
		UI::Text_ptr pTextTeamsScoreGreen = std::make_shared<UI::Text>( Rectangle2D( 0, 64, 200, 120 ) );
		pTextTeamsScoreGreen->SetID( TEXT_TeamGreenKills );
		pTextTeamsScoreGreen->SetFont( "Arial", 28, 0, FALSE, TRUE, D3DCOLOR_XRGB( 84, 173, 63 ) );
		pTextTeamsScoreGreen->SetDropdownShadow( TRUE );
		pTextTeamsScoreGreen->SetDropdownColor( D3DCOLOR_ARGB( 200, 0, 0, 0 ) );
		pTextTeamsScoreGreen->SetText( "" );
		pTextTeamsScoreGreen->SetHorizontalAlign( ALIGN_Center );
		pWindowTop->AddElement( pTextTeamsScoreGreen );

		UI::Text_ptr pTextTeamsScoreWhite = std::make_shared<UI::Text>( Rectangle2D( 200, 64, 200, 120 ) );
		pTextTeamsScoreWhite->SetID( TEXT_TeamWhiteKills );
		pTextTeamsScoreWhite->SetFont( "Arial", 28, 0, FALSE, TRUE, D3DCOLOR_XRGB( 255, 255, 255 ) );
		pTextTeamsScoreWhite->SetDropdownShadow( TRUE );
		pTextTeamsScoreWhite->SetDropdownColor( D3DCOLOR_ARGB( 200, 0, 0, 0 ) );
		pTextTeamsScoreWhite->SetText( "" );
		pTextTeamsScoreWhite->SetHorizontalAlign( ALIGN_Center );
		pWindowTop->AddElement( pTextTeamsScoreWhite );
	}

	//Countdown
	{
		UI::Text_ptr pTextCountdown = std::make_shared<UI::Text>( Rectangle2D( (pWindowTop->GetWidth() >> 1) - 200, 120, 400, 120 ) );
		pTextCountdown->SetID( TEXT_Countdown );
		pTextCountdown->SetFont( "Arial", 72, 0, FALSE, TRUE, D3DCOLOR_XRGB( 255, 255, 255 ) );
		pTextCountdown->SetDropdownShadow( TRUE );
		pTextCountdown->SetDropdownColor( D3DCOLOR_ARGB( 200, 0, 0, 0 ) );
		pTextCountdown->SetText( "" );
		pTextCountdown->SetHorizontalAlign( ALIGN_Center );
		pWindowTop->AddElement( pTextCountdown );
	}

	//Revive
	{
		UI::Text_ptr pTextRevive = std::make_shared<UI::Text>( Rectangle2D( (pWindowTop->GetWidth() >> 1) - 200, 120, 400, 120 ) );
		pTextRevive->SetID( TEXT_TimeRevive );
		pTextRevive->SetFont( "Arial", 36, 0, FALSE, TRUE, D3DCOLOR_XRGB( 255, 20, 20 ) );
		pTextRevive->SetDropdownShadow( TRUE );
		pTextRevive->SetDropdownColor( D3DCOLOR_ARGB( 200, 0, 0, 0 ) );
		pTextRevive->SetText( "" );
		pTextRevive->SetHorizontalAlign( ALIGN_Center );
		pWindowTop->AddElement( pTextRevive );
	}

	pWindowTop->Hide();

	pcModelSmoke = EXEModel::ReadModel( "game\\objects\\maps\\broyale\\broyale_smoke.ASE" );

	pcImageArrow = UI::ImageLoader::LoadImage( "game\\images\\hud\\arrowdownhead.png" );
}

void CBattleRoyaleHandler::HandlePacket( PacketBattleRoyaleUpdate * psPacket )
{
	CopyMemory( &sState, &psPacket->sState, sizeof( State ) );

	eTeamID = PLAYERTEAM_None;
	if ( sState.bTeam )
	{
		for ( int i = 0; i < sState.iPlayerTeamCount; i++ )
		{
			if ( sState.saPlayerTeams[i].iID == UNITDATA->iID )
			{
				eTeamID = sState.saPlayerTeams[i].eTeamID;
				break;
			}
		}
	}

	if ( InWaiting() || InBattle() )
	{
		if ( InBattle() )
		{
			pWindowTop->GetElement<UI::Text>( TEXT_State )->SetText( "In Battle" );
			pWindowTop->GetElement<UI::Text>( TEXT_State )->SetColor( D3DCOLOR_XRGB( 255, 20, 20 ) );

			if ( sState.bTeam )
			{
				pWindowTop->GetElement<UI::Text>( TEXT_Team )->SetText( FormatString( "%s Team", BroyaleTeamToString( eTeamID ).c_str() ) );

				DWORD dwColor = -1;
				switch ( eTeamID )
				{
					case PLAYERTEAM_Red:
						dwColor = RGBA( 255, 255, 30, 0 );
						break;
					case PLAYERTEAM_Green:
						dwColor = RGBA( 255, 0, 255, 100 );
						break;
					case PLAYERTEAM_Blue:
						dwColor = RGBA( 255, 0, 128, 255 );
						break;
				}

				pWindowTop->GetElement<UI::Text>( TEXT_Team )->SetColor( dwColor );


				if ( CBattleRoyale::IsTeamRoyalTickets( sState.iBattleID ) )
				{
					for ( int i = 0; i < _countof( sState.eaTeamID ); i++ )
					{
						if ( sState.eaTeamID[i] == EPlayerTeam::PLAYERTEAM_Green )
							pWindowTop->GetElement<UI::Text>( TEXT_TeamGreenKills )->SetTextEx( "Green (%d/%d)", sState.iaTeamKills[i], sState.iKillsNeeded );
						else
							pWindowTop->GetElement<UI::Text>( TEXT_TeamWhiteKills )->SetTextEx( "White (%d/%d)", sState.iaTeamKills[i], sState.iKillsNeeded );
					}
				}
			}
			else
				pWindowTop->GetElement<UI::Text>( TEXT_Team )->SetText( "" );
		}
		else
		{
			if ( sState.eGameState == BattleRoyale::EGameState::GAMESTATE_WaitingForPlayers )
			{
				int iDifferenceTime = (MAX_BATTLEROYALE_WAITTIME - psPacket->sState.iTimeUpdate) / 1000;
				int iMinutes = iDifferenceTime / 60;
				int iSeconds = iDifferenceTime % 60;
				pWindowTop->GetElement<UI::Text>( TEXT_State )->SetTextEx( "Waiting for players... (%02d:%02d)", iMinutes, iSeconds );

				if ( sState.bCountdown )
					pWindowTop->GetElement<UI::Text>( TEXT_State )->SetText( "" );

				pWindowTop->GetElement<UI::Text>( TEXT_Team )->SetText( "" );

				pWindowTop->GetElement<UI::Text>( TEXT_TeamWhiteKills )->SetText( "" );
				pWindowTop->GetElement<UI::Text>( TEXT_TeamGreenKills )->SetText( "" );

				pWindowTop->GetElement<UI::Text>( TEXT_TimeRevive )->SetText( "" );
			}
			else if ( sState.eGameState == BattleRoyale::EGameState::GAMESTATE_WaitingPvP )
			{
				int iDifferenceTime = (MAX_BATTLEROYALE_WAITPVP - psPacket->sState.iTimeUpdate) / 1000;
				int iMinutes = iDifferenceTime / 60;
				int iSeconds = iDifferenceTime % 60;
				pWindowTop->GetElement<UI::Text>( TEXT_State )->SetTextEx( "Drop time... (%02d:%02d)", iMinutes, iSeconds );
				if ( sState.bCountdown )
					pWindowTop->GetElement<UI::Text>( TEXT_State )->SetText( "" );
			}

			pWindowTop->GetElement<UI::Text>( TEXT_State )->SetColor( D3DCOLOR_XRGB( 84, 173, 63 ) );
		}

		if ( psPacket->sState.eGameState == GAMESTATE_End )
		{
			pWindowTop->GetElement<UI::Text>( TEXT_State )->SetText( "" );
			pWindowTop->GetElement<UI::Text>( TEXT_Team )->SetText( "" );

			pWindowTop->GetElement<UI::Text>( TEXT_TeamWhiteKills )->SetText( "" );
			pWindowTop->GetElement<UI::Text>( TEXT_TeamGreenKills )->SetText( "" );

			pWindowTop->GetElement<UI::Text>( TEXT_TimeRevive )->SetText( "" );
		}
	}
	else
	{
		pWindowTop->GetElement<UI::Text>( TEXT_State )->SetText( "" );
		pWindowTop->GetElement<UI::Text>( TEXT_Team )->SetText( "" );

		pWindowTop->GetElement<UI::Text>( TEXT_TeamWhiteKills )->SetText( "" );
		pWindowTop->GetElement<UI::Text>( TEXT_TeamGreenKills )->SetText( "" );

		pWindowTop->GetElement<UI::Text>( TEXT_TimeRevive )->SetText( "" );
	}

	if ( pWindowTop->IsOpen() == FALSE )
		pWindowTop->Show();
}

void CBattleRoyaleHandler::HandlePacket( PacketBattleRoyaleBattleTime * psPacket )
{
	if ( bStart && psPacket->bStart )
		return;

	bStart = psPacket->bStart;
	if ( bStart == TRUE )
		bWaitingTime = TRUE;

	if ( psPacket->bRemoveItems )
	{
		bWaitingTime = TRUE;

		DELETA( pcaItemData );
		pcaItemData = new ItemData[(INVENTORY_DATACOUNT * 3) + 1];

		DELETA( psaInventorySlot );
		psaInventorySlot = new InventoryItemData[15];

		CopyMemory( pcaItemData, CGameInventory::GetInstance()->saInventoryItems, sizeof( ItemData ) * INVENTORY_DATACOUNT );
		CopyMemory( pcaItemData + INVENTORY_DATACOUNT, CGameInventory::GetInstance()->saInventoryItemTemp, sizeof( ItemData ) * INVENTORY_DATACOUNT );
		CopyMemory( pcaItemData + (INVENTORY_DATACOUNT * 2), CGameInventory::GetInstance()->saBackupInventoryItemTemp, sizeof( ItemData ) * INVENTORY_DATACOUNT );

		CopyMemory( pcaItemData + (INVENTORY_DATACOUNT * 3), ITEMHANDLER->GetMouseItem(), sizeof( ItemData ) );

		CopyMemory( psaInventorySlot, INVENTORYITEMSLOT, sizeof( InventoryItemData ) * 14 );

		if ( INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex > 0 )
			saItemID[0] = INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex - 1].sItem.sItemID;
		else
			saItemID[0].SetItemID( ITEMID_None );

		if ( INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex > 0 )
			saItemID[1] = INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex - 1].sItem.sItemID;
		else
			saItemID[1].SetItemID( ITEMID_None );

		if ( INVENTORYITEMSLOT[ITEMSLOT_Armor - 1].iItemIndex > 0 )
			saItemID[2] = INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_Armor - 1].iItemIndex - 1].sItem.sItemID;
		else
			saItemID[2].SetItemID( ITEMID_None );

		STRINGCOPY( szBodyModel, UNITDATA->sCharacterData.Player.szBodyModel );

		pcUnknownItem = (ItemData *)READDWORD( 0x035EB67C );
		WRITEDWORD( 0x035EB67C, 0 );

		ZeroMemory( CGameInventory::GetInstance()->saInventoryItems, sizeof( ItemData ) * INVENTORY_DATACOUNT );
		ZeroMemory( CGameInventory::GetInstance()->saInventoryItemTemp, sizeof( ItemData ) * INVENTORY_DATACOUNT );
		ZeroMemory( CGameInventory::GetInstance()->saBackupInventoryItemTemp, sizeof( ItemData ) * INVENTORY_DATACOUNT );
		ZeroMemory( ITEMHANDLER->GetMouseItem(), sizeof( ItemData ) );

		for ( int i = 0; i < 14; i++ )
			INVENTORYITEMSLOT[i].iItemIndex = 0;

		ITEMHANDLER->SetCharacterItem( saItemID[1].ToItemID(), ITEMSLOTFLAG_LeftHand, FALSE );
		ITEMHANDLER->SetCharacterItem( saItemID[0].ToItemID(), ITEMSLOTFLAG_RightHand, FALSE );
		ITEMHANDLER->SetCharacterItem( saItemID[2].ToItemID(), ITEMSLOTFLAG_Costume, FALSE );
		ITEMHANDLER->SetCharacterItem( saItemID[2].ToItemID(), ITEMSLOTFLAG_Armor, FALSE );

		WRITEDWORD( 0x035EB690, FALSE );
		WRITEDWORD( 0x03625718, FALSE );

		SOUNDHANDLER->PlayMusic( BACKGROUNDMUSICID_Broyale );

		pcaCrowns[0] = (EXEModel *)READDWORD( 0x00CF47F8 );
		pcaCrowns[1] = (EXEModel *)READDWORD( 0x00CF47FC );
		WRITEDWORD( 0x00CF47F8, 0 );
		WRITEDWORD( 0x00CF47FC, 0 );

		HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->SwapInventory( FALSE );

		HUDHANDLER->GetInventoryWindow()->SetEarringsEnabled( false );

		HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->SetBlockSwap( true );
	}
	else if ( psPacket->bStart == FALSE )
	{
		bWaitingTime = FALSE;

		if ( pcaItemData )
		{
			CopyMemory( CGameInventory::GetInstance()->saInventoryItems, pcaItemData, sizeof( ItemData ) * INVENTORY_DATACOUNT );
			CopyMemory( CGameInventory::GetInstance()->saInventoryItemTemp, pcaItemData + INVENTORY_DATACOUNT, sizeof( ItemData ) * INVENTORY_DATACOUNT );
			CopyMemory( CGameInventory::GetInstance()->saBackupInventoryItemTemp, pcaItemData + (INVENTORY_DATACOUNT * 2), sizeof( ItemData ) * INVENTORY_DATACOUNT );
			CopyMemory( ITEMHANDLER->GetMouseItem(), pcaItemData + (INVENTORY_DATACOUNT * 3), sizeof( ItemData ) );

			DELETA( pcaItemData );

			CopyMemory( INVENTORYITEMSLOT, psaInventorySlot, sizeof( InventoryItemData ) * 14 );
			DELETA( psaInventorySlot );

			ITEMHANDLER->SetCharacterItem( saItemID[0].ToItemID(), ITEMSLOTFLAG_RightHand, (saItemID[0].ToItemID() != ITEMID_None) ? TRUE : FALSE );
			ITEMHANDLER->SetCharacterItem( saItemID[1].ToItemID(), ITEMSLOTFLAG_LeftHand, (saItemID[1].ToItemID() != ITEMID_None) ?	((saItemID[1].ToItemID() != saItemID[0].ToItemID()) ? TRUE : FALSE) : FALSE );
			ITEMHANDLER->SetCharacterItem( saItemID[2].ToItemID(), ITEMSLOTFLAG_Armor, (saItemID[2].ToItemID() != ITEMID_None) ? TRUE : FALSE );

			UNITDATA->bNoMove = TRUE;

			STRINGCOPY( UNITDATA->sCharacterData.Player.szBodyModel, szBodyModel );
			SETPLAYERPATTERN( UNITDATA, UNITDATA->sCharacterData.Player.szBodyModel, UNITDATA->sCharacterData.Player.szHeadModel );

			WRITEDWORD( 0x035EB690, FALSE );
			WRITEDWORD( 0x03625718, FALSE );

			WRITEDWORD( 0x035EB67C, (DWORD)pcUnknownItem );

			pcUnknownItem = nullptr;

			WRITEDWORD( 0x00CF47F8, (DWORD)pcaCrowns[0] );
			WRITEDWORD( 0x00CF47FC, (DWORD)pcaCrowns[1] );
			pcaCrowns[0] = nullptr;
			pcaCrowns[1] = nullptr;

			HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->SetBlockSwap( false );

			HUDHANDLER->GetInventoryWindow()->SetEarringsEnabled( true );

			HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->SwapInventory( (*(BOOL *)0x03625718) );
		}

		pWindowTop->Hide();
	}

	ITEMHANDLER->ValidateInventoryItems();
	ITEMHANDLER->UpdateItemCharacterStatus();
	CHARACTERGAME->UpdateWeight();

	if ( bStart == FALSE )
	{
		UNITDATA->sCharacterData.cNewLoad++;
		CHECK_CHARACTER_CHECKSUM;
		UNITGAME->UpdateObjectID( UNITDATA->iID );

		fTimeCharacter = 2000.0f;
	}
	else
		CHECK_CHARACTER_CHECKSUM;
}

void CBattleRoyaleHandler::HandlePacket( PacketBattleRoyaleList * psPacket )
{
	if ( MOUNTHANDLER->IsRiding( UNITDATA ) )
	{
		MOUNTHANDLER->ShowMessage( MOUNTMESSAGE_ErrorEnterNPC );
		return;
	}

	GetWindow()->Clear();

	for ( int i = 0; i < psPacket->iCount; i++ )
		GetWindow()->AddBattle( psPacket->saList[i].iID, psPacket->saList[i].iMapID, psPacket->saList[i].bOpen, psPacket->saList[i].sRequiredLevel, psPacket->saList[i].iTimeLeft, psPacket->saList[i].szName );

	GetWindow()->Open( CBattleRoyaleWindowHandler::EWindowOpenID::WINDOWOPENID_Main );
}

void CBattleRoyaleHandler::HandlePacket( PacketBattleRoyaleRewardList * psPacket )
{
	if ( psPacket->IsFirst() )
		GetWindow()->Clear();

	for ( int i = 0; i < psPacket->iCount; i++ )
		GetWindow()->AddReward( psPacket->saRewards[i].iID, psPacket->saRewards[i].iChance, psPacket->saRewards[i].szName, psPacket->saRewards[i].szImagePath );

	if ( psPacket->IsOnly() )
	{
		GetWindow()->SetDataID( psPacket->iInstanceID, psPacket->iBattleID );
		GetWindow()->Open( CBattleRoyaleWindowHandler::EWindowOpenID::WINDOWOPENID_Reward );
	}
}

void CBattleRoyaleHandler::HandlePacket( PacketBattleRoyaleResponse * psPacket )
{
	if ( psPacket->iRetCode == 1 )
		GetWindow()->UpdateReward();
	else if ( psPacket->iRetCode == 2 )
		GetWindow()->Close();
}

void CBattleRoyaleHandler::HandlePacket( PacketBattleRoyaleTeleports * psPacket )
{
	iSpawnCoordinatesCount = psPacket->iCount;

	CopyMemory( saCoordinates, psPacket->saCoordinates, sizeof( BattleRoyale::SpawnCoordinate ) * iSpawnCoordinatesCount );
}

void CBattleRoyaleHandler::ProcessPacket( PacketBattleRoyaleRequest * psPacket )
{
	psPacket->iLength = sizeof( PacketBattleRoyaleRequest );
	psPacket->iHeader = PKTHDR_BattleRoyaleRequest;
	if ( psPacket->bGiveGoldEXP )
		SENDPACKETL( psPacket, TRUE );
	else
		SENDPACKETG( psPacket, TRUE );
}

BOOL CBattleRoyaleHandler::IsInsideRadius( Point3D sPosition )
{
	return BattleRoyale::IsBattleRoyaleInsideRadius( sPosition, sState.sPosition, sState.iRadius );
}

BOOL CBattleRoyaleHandler::IsInsideNextRadius( Point3D sPosition )
{
	return BattleRoyale::IsBattleRoyaleInsideRadius( sPosition, sState.sPosition, sState.iNextRadius );
}

BOOL CBattleRoyaleHandler::IsInsideMeteorite( Point3D sPosition )
{
	if ( sState.iMeteoriteTime == 0 )
		return FALSE;

	return BattleRoyale::IsBattleRoyaleInsideRadius( sPosition, sState.sPosition, RADIUS_BATTLEROYALE_METEORITE );
}

void CBattleRoyaleHandler::Update( float fTime )
{
	GetWindow()->Update( fTime );

	//return;
	if ( Game::GetGameMode() == GAMEMODE_InGame )
	{
		if ( InBattle() )
		{
			fTimeUpdateDecreaseHP += fTime;
			sState.iTimeUpdate += (int)fTime;

			if ( (sState.iDuration - sState.iTimeUpdate) <= 4000 )
				sState.iTimeUpdate = sState.iDuration - 4000;

			//Update Radius
			sState.iRadius = ComputeBattleRoyaleRadius( sState.iTimeUpdate, sState.iDuration );

			//Update Next Radius
			if ( (sState.iNextRadius >= sState.iRadius) || (sState.iNextRadius == 0) )
				sState.iNextRadius = (sState.iRadius * RADIUS_BATTLEROYALE_NEXT) / 100;

			if ( fTimeUpdateDecreaseHP >= 1000.0f )
			{
				int iTime = (sState.iDuration - sState.iTimeUpdate) / 1000;
				int iMinute = iTime / 60;
				int iSecond = iTime % 60;
				pWindow->GetElement<UI::Text>( TEXT_GasTime )->SetTextEx( "%02d:%02d", iMinute, iSecond );

				pWindow->GetElement<UI::Text>( TEXT_PlayersCount )->SetTextEx( "%d", sState.iPlayersCount );

				if ( UNITDATA->GetAnimation() != ANIMATIONTYPE_Die )
				{
					if ( UNITDATA->bNoDeath == 0 )
					{
						if ( IsInsideRadius( UNIT->GetPosition() ) == FALSE )
						{
							PacketDamageQuick sPacket;
							sPacket.bPercentile = TRUE;
							sPacket.bBaseCurrentHP = FALSE;
							sPacket.iDamage = (sState.iTimeUpdate * 80) / (sState.iDuration > 0 ? sState.iDuration : 1);
							DAMAGEHANDLER->HandlePacket( &sPacket );
						}
					}
				}

				fTimeUpdateDecreaseHP = 0.0f;
			}
		}

		if ( sState.bCountdown )
		{
			static float fAlphaCountdown = 0.0f;
			static int iOldTimeCountdown = 0;

			if ( InBattle() == FALSE )
			{
				UINT iNewTimeCountdown = (UINT)((sState.iCountdownEnd - sState.iTimeUpdate) / 1000);
				if ( iNewTimeCountdown <= 10 )
				{
					if ( iNewTimeCountdown != iOldTimeCountdown )
					{
						iOldTimeCountdown = iNewTimeCountdown;

						pWindowTop->GetElement<UI::Text>( TEXT_Countdown )->SetTextEx( "%d", iNewTimeCountdown );
						fAlphaCountdown = 1000.0f;
					}
				}
			}
			else
			{
				if ( iOldTimeCountdown != 1000 )
				{
					iOldTimeCountdown = 1000;

					pWindowTop->GetElement<UI::Text>( TEXT_Countdown )->SetText( "START" );

					fAlphaCountdown = 1000.0f;
				}
			}

			if ( (fAlphaCountdown -= fTime) <= 0.0f )
				fAlphaCountdown = 0.0f;

			int iAlpha			= (int)((fAlphaCountdown * 255.0f) / 1000.0f);
			int iAlphaShadow	= (int)((fAlphaCountdown * 200.0f) / 1000.0f);
			auto pTextCountdown = pWindowTop->GetElement<UI::Text>( TEXT_Countdown );
			pTextCountdown->SetColor( D3DCOLOR_ARGB( iAlpha, 255, 255, 255 ) );
			pTextCountdown->SetDropdownColor( D3DCOLOR_ARGB( iAlphaShadow, 0, 0, 0 ) );
		}

		//Revive Time
		if ( READDWORD( 0x0209EA4C ) )
		{
			int iTime = RESTARTCOUNTER / 70;
			if ( iTime < 0 )
				iTime = 0;

			pWindowTop->GetElement<UI::Text>( TEXT_TimeRevive )->SetTextEx( "Revive in %d", iTime );

			fTimeToRevive -= fTime;
		}
		else
		{
			pWindowTop->GetElement<UI::Text>( TEXT_TimeRevive )->SetText( "" );

			fTimeToRevive = 0.0f;
		}

		if ( fTimeCharacter > 0.0f )
		{
			fTimeCharacter -= fTime;

			if ( fTimeCharacter <= 0.0f )
			{
				UNITDATA->sCharacterData.cNewLoad++;
				CHECK_CHARACTER_CHECKSUM;
				UNITGAME->UpdateObjectID( UNITDATA->iID );

				fTimeCharacter = 0.0f;
			}
		}
	}
}

void CBattleRoyaleHandler::OnResolutionChanged()
{
	pWindowTop->SetPosition( (RESOLUTION_WIDTH >> 1) - (pWindowTop->GetWidth() >> 1), 70 );
}

void CBattleRoyaleHandler::Render()
{
	if ( InBattle() )
		pWindow->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );

	pWindowTop->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

void CBattleRoyaleHandler::Render3D()
{
	if ( InBattle() )
	{
		Point3D sCameraPosition;
		sCameraPosition.iX = UNITTARGETPOSITION_X;
		sCameraPosition.iY = UNITTARGETPOSITION_Y;
		sCameraPosition.iZ = UNITTARGETPOSITION_Z;

		Point3D sCameraAngle;
		sCameraAngle.iX = UNITTARGETANGLE_X;
		sCameraAngle.iY = UNITTARGETANGLE_Y;
		sCameraAngle.iZ = UNITTARGETANGLE_Z;

		Point3D sPosition = sState.sPosition;

		pcModelSmoke->SetPosition( &sPosition, &Point3D( 0, 0, 0 ) );

		//Get Difference
		int iDifferenceRadius = 0;
		{
			int iX = (sPosition.iX - UNIT->GetPosition().iX) >> 8;
			int iZ = (sPosition.iZ - UNIT->GetPosition().iZ) >> 8;

			iDifferenceRadius = (int)abs( sqrtf( (float)(iX * iX + iZ * iZ) ) - (float)sState.iRadius );
		}

		int iAlphaOld = RENDERER->a;
		RENDERER->a = -160;

		int iObjectScale = RENDERER->iObjectVertexScale;

		RENDERER->iObjectVertexScale = sState.iRadius;
		if ( RENDERER->iObjectVertexScale < 64 )
			RENDERER->iObjectVertexScale = 64;

		for ( UINT i = 0; i < pcModelSmoke->uMeshCount; i++ )
			pcModelSmoke->paMesh[i]->dwUnused = 0x3F;

		RENDERER->bNoClipFaceRender = TRUE;

		if ( iDifferenceRadius <= 3000 )
			pcModelSmoke->Render( &sCameraPosition, &sCameraAngle );

		RENDERER->bNoClipFaceRender = FALSE;

		RENDERER->iObjectVertexScale = iObjectScale;

		RENDERER->a = iAlphaOld;
	}
}

void CBattleRoyaleHandler::RenderUnitArrow( UnitData * pcUnitData )
{
	if ( InBattle() )
	{
		if ( sState.bTeam )
		{
			if ( IsEnemy( pcUnitData ) == false )
			{
				int iX = pcUnitData->rRenderRectangle.left;
				int iY = pcUnitData->rRenderRectangle.top;
				int iWidth = pcUnitData->rRenderRectangle.right - iX;
				int iHeight = pcUnitData->rRenderRectangle.bottom - iY;

				DWORD dwColor = -1;
				switch ( eTeamID )
				{
					case PLAYERTEAM_Red:
						dwColor = RGBA( 255, 255, 30, 0 );
						break;
					case PLAYERTEAM_Green:
						dwColor = RGBA( 255, 0, 255, 100 );
						break;
					case PLAYERTEAM_Blue:
						dwColor = RGBA( 255, 0, 128, 255 );
						break;
				}

				UI::ImageRender::Render( pcImageArrow, iX + (iWidth >> 1) - (pcImageArrow->GetWidth() >> 1), iY - pcImageArrow->GetHeight() - 16, pcImageArrow->GetWidth(), pcImageArrow->GetHeight(), dwColor );
			}
		}
	}
}

void CBattleRoyaleHandler::Test()
{
	sState.sPosition = UNIT->GetPosition();
	sState.sPosition.iY = 0;
	sState.iDuration = (120 * 1000);
	sState.iTimeUpdate = 100 * 1000;

	sState.iRadius = MAX_BATTLEROYALE_RADIUS;
	sState.iNextRadius = (sState.iRadius * RADIUS_BATTLEROYALE_NEXT) / 100;
	sState.eGameState = BattleRoyale::GAMESTATE_InBattle;
	sState.iMeteoriteTime = sState.iTimeUpdate + 20000;
	sState.sMeteoritePosition = sState.sPosition;
}

void CBattleRoyaleHandler::OnDie()
{
	if ( InBattle() )
	{
		if ( CBattleRoyale::IsTeamRoyalTickets( sState.iBattleID ) == false )
		{
			PacketBattleRoyaleGasDeath sPacket;
			sPacket.iLength = sizeof( PacketBattleRoyaleGasDeath );
			sPacket.iHeader = PKTHDR_BattleRoyaleDeath;
			sPacket.bMeteorite = IsInsideMeteorite( UNIT->GetPosition() );
			sPacket.iTime = sState.iTimeUpdate;
			SENDPACKETG( &sPacket );

			if ( INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex > 0 )
				ITEMHANDLER->SetCharacterItem( INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex - 1].sItem.sItemID.ToItemID(), ITEMSLOTFLAG_RightHand, FALSE );

			if ( INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex > 0 )
				ITEMHANDLER->SetCharacterItem( INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex - 1].sItem.sItemID.ToItemID(), ITEMSLOTFLAG_LeftHand, FALSE );

			if ( INVENTORYITEMSLOT[ITEMSLOT_Costume - 1].iItemIndex > 0 )
				ITEMHANDLER->SetCharacterItem( INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_Costume - 1].iItemIndex - 1].sItem.sItemID.ToItemID(), ITEMSLOTFLAG_Costume, FALSE );

			if ( INVENTORYITEMSLOT[ITEMSLOT_Armor - 1].iItemIndex > 0 )
				ITEMHANDLER->SetCharacterItem( INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_Armor - 1].iItemIndex - 1].sItem.sItemID.ToItemID(), ITEMSLOTFLAG_Armor, FALSE );

			CGameInventory::GetInstance()->DropInventoryItemsOnMap();
		}
		else
		{
			if ( fTimeToRevive == 0.0f )
			{
				WRITEDWORD( 0x0209EA4C, 1 );

				RESTARTCOUNTER = (70 * 10);

				fTimeToRevive = 10000.0f;
			}
		}
	}
}

bool CBattleRoyaleHandler::IsEnemy( UnitData * pcUnitData )
{
	bool bRet = false;

	if ( InBattle() )
	{
		bRet = true;

		if ( sState.bTeam )
		{
			for ( int i = 0; i < sState.iPlayerTeamCount; i++ )
			{
				if ( sState.saPlayerTeams[i].iID == pcUnitData->iID )
				{
					if ( eTeamID == sState.saPlayerTeams[i].eTeamID )
						bRet = false;

					break;
				}
			}
		}
	}

	return bRet;
}

bool CBattleRoyaleHandler::OnRespawn( Point3D & sPosition )
{
	bool bRet = false;
	if ( iSpawnCoordinatesCount > 0 )
	{
		if ( InBattle() )
		{
			if ( CBattleRoyale::IsTeamRoyalTickets( sState.iBattleID ) )
			{
				std::vector<Point3D> vCoordList;

				for ( int i = 0; i < iSpawnCoordinatesCount; i++ )
				{
					auto & sCoordinate = saCoordinates[i];

					Point3D s;

					std::random_device rd;
					std::uniform_int_distribution<int> cGenX( sCoordinate.iWest, sCoordinate.iEast );
					std::uniform_int_distribution<int> cGenZ( sCoordinate.iSouth, sCoordinate.iNorth );

					s.iX = cGenX( rd ) << 8;
					s.iZ = cGenZ( rd ) << 8;
					s.iY = 0;

					if ( IsInsideRadius( s ) )
						vCoordList.push_back( s );
				}

				vCoordList.push_back( sState.sPosition );

				sPosition = vCoordList[RandomI( 1, vCoordList.size() ) - 1];
				
				if ( (STAGE1 != NULL) && (UNITDATA->iLoadedMapIndex == 0) )
					sPosition.iY = STAGE1->GetHighestPoint( sPosition.iX, sPosition.iZ );
				if ( (STAGE2 != NULL) && (UNITDATA->iLoadedMapIndex == 1) )
					sPosition.iY = STAGE2->GetHighestPoint( sPosition.iX, sPosition.iZ );

				vCoordList.clear();

				bRet = true;
			}
		}
	}
	return bRet;
}
