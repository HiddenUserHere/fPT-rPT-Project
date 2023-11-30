#include "stdafx.h"
#include "HUDController.h"

bool RollNumberIsGreater( RollDiceRankingUser & pcRollUser1, RollDiceRankingUser & pcRollUser2 ) { return pcRollUser1.uNumber > pcRollUser2.uNumber; }

HUDController::HUDController()
{
	bRenderTarget				= FALSE;
	pWindowTarget				= std::make_shared<UI::Window>( Rectangle2D( 0, 0, 425, 80 ) );
	pWindowDisplay				= std::make_shared<UI::Window>( Rectangle2D( 0, 0, 370, 150 ) );
	pWindowRollDiceRanking		= std::make_shared<UI::Window>( Rectangle2D( 0, 0, 379, 124 ) );
	pcTooltipWorldTime			= std::make_shared<UI::Tooltip>( Rectangle2D() );

	for( int i = 0; i < 3; i++ )
		pcTooltipBars[i] = std::make_shared<UI::Tooltip>( Rectangle2D() );

	pcPartyHandler				= new CPartyHandler();
	pcChatBox					= new CChatBox();
	pcInventory					= new CInventory();
	pcMiniMapHandler			= new CMiniMapHandler();
	pcAchievementHandler		= new CAchievementHandler();

	pcInventoryWindowHandler	= new CInventoryWindowHandler();
}

HUDController::~HUDController()
{
	DELET( pcInventoryWindowHandler );

	DELET( pcPartyHandler );
	DELET( pcChatBox );
	DELET( pcInventory );
	DELET( pcMiniMapHandler );
	DELET( pcAchievementHandler );
}

void HUDController::OnSelectTargetMenu()
{
	if ( BATTLEROYALEHANDLER->InWaiting() || BATTLEROYALEHANDLER->InBattle() )
	{
		TITLEBOX( "You cannot use the menu in Battle Royale!" );
		return;
	}

	auto pMenu = pWindowTarget->GetElement<UI::DropdownMenu>( DROPDOWNMENU_More );
	if( pMenu )
	{
		int iMenuID = pMenu->GetSelectedOptionID();
		if( iMenuID == 1 )		//Request Trade
		{
			CALL_WITH_ARG2( 0x000628290, TARGET_UNITDATA->sCharacterData.iID, 0 );
			CHATGAMEHANDLER->AddChatBoxTextEx( CHATCOLOR_Error, "> Sent Trade Request to %s", TARGET_UNITDATA->sCharacterData.szName );
		}
		else if( iMenuID == 2 )	//Add Friend
		{
			SOCIALHANDLER->GetFriends()->AddRecent( TARGET_UNITDATA->sCharacterData.szName );
			SOCIALHANDLER->GetFriends()->AddFriend( TARGET_UNITDATA->sCharacterData.szName );
			SOCIALHANDLER->Open();

			/*
			CALLT_WITH_ARG1( 0x005A57D0, 0x39114D0, (DWORD)TARGET_UNITDATA->sCharacterData.szName );		//Recents
			CALLT_WITH_ARG1( 0x005A57E0, 0x39114D0, (DWORD)TARGET_UNITDATA->sCharacterData.szName );		//Friends
			CALLT_WITH_ARG1( 0x005A5860, 0x39114D0, 2 );
			CALLT_WITH_ARG1( 0x005A5820, 0x39114D0, 1 );
			CALL( 0x005075C0 );
			*/
		}
		else if( iMenuID == 3 )	//Request Party
			PARTYHANDLER->ProcessPacket( &PacketRequestParty( TARGET_UNITDATA->sCharacterData.iID ) );
		else if( iMenuID == 4 )	//Request Raid
			PARTYHANDLER->ProcessPacket( &PacketRequestRaid( TARGET_UNITDATA->sCharacterData.iID ) );
		else if( iMenuID == 5 )	//Invite Clan
			CALL_WITH_ARG1( 0x0065CFA0, (DWORD)TARGET_UNITDATA );
	}
}

BOOL HUDController::IsLockItems()
{
	return pcInventoryWindowHandler->IsLockItems();
}

void HUDController::RenderHPCrystalTower( int iX, int iY, int iMin, int iMax )
{
	if ( pcaImageCrystalTowerHPBar[0] && pcaImageCrystalTowerHPBar[1] )
	{
		UI::Image * pc = pcaImageCrystalTowerHPBar[0];
		UI::Image * pc1 = pcaImageCrystalTowerHPBar[1];

		int iWidth = (iMin * pc->GetWidth()) / iMax;

		UI::ImageRender::Render( pc1, iX - (pc->GetWidth() >> 1) + 60, iY, pc1->GetWidth(), pc1->GetHeight(), 0, 0, -1, 0.75f );
		UI::ImageRender::Render( pc, iX - (pc->GetWidth() >> 1) + 60, iY, iWidth, pc->GetHeight(), 0, 0, -1, 0.75f );

		char szHP[64] = { 0 };
		STRINGFORMAT( szHP, "%d/%d", iMin, iMax );
		int iWidthText = GETWIDTHTEXT( szHP );

		GetGraphicsDevice()->GetDefaultFont()->Draw( szHP, iX - (iWidthText >> 1) + 18, iY + 1, 0, DX::Color( 255, 255, 255 ) );
	}
}

void HUDController::UpdateObjectsSettings()
{
	//Interface Settings
	if ( PARTYHANDLER->GetPartyWindow()->GetWindowParty() )
		PARTYHANDLER->GetPartyWindow()->GetWindowParty()->SetCanMove( !SETTINGSHANDLER->GetModel()->GetSettings().bLockUI );

	if ( PARTYHANDLER->GetPartyWindow()->GetWindowRaid() )
		PARTYHANDLER->GetPartyWindow()->GetWindowRaid()->SetCanMove( !SETTINGSHANDLER->GetModel()->GetSettings().bLockUI );

	if ( HUDHANDLER->GetTargetWindow() )
	{
		HUDHANDLER->GetTargetWindow()->SetCanMove( !SETTINGSHANDLER->GetModel()->GetSettings().bLockUI );

		auto pTargetDistance = HUDHANDLER->GetTargetWindow()->GetElement<UI::Text>( TEXTID_Range );
		if ( pTargetDistance )
		{
			if ( SETTINGSHANDLER->GetModel()->GetSettings().bTargetDistance )
				pTargetDistance->Show();
			else
				pTargetDistance->Hide();
		}

		auto pTargetHPValue = HUDHANDLER->GetTargetWindow()->GetElement<UI::Text>( TEXTID_Life );
		if ( pTargetHPValue )
		{
			if ( SETTINGSHANDLER->GetModel()->GetSettings().bTargetHPValue )
				pTargetHPValue->Show();
			else
				pTargetHPValue->Hide();
		}

		auto pTargetHPPercent = HUDHANDLER->GetTargetWindow()->GetElement<UI::Text>( TEXTID_LifePercent );
		if ( pTargetHPPercent )
		{
			if ( SETTINGSHANDLER->GetModel()->GetSettings().bTargetHPPercent )
				pTargetHPPercent->Show();
			else
				pTargetHPPercent->Hide();
		}
	}

	if( CHATBOX->GetWindow() )
	{
		CHATBOX->GetWindow()->SetCanMove( !SETTINGSHANDLER->GetModel()->GetSettings().bLockUI );
		CHATBOX->ToggleNotice( SETTINGSHANDLER->GetModel()->GetSettings().bShowNotice );
	}

	SOCIALHANDLER->UpdateObjectSettings();
}

BOOL HUDController::UpdateTargetInfo( UnitData * pcUnitData )
{
	Unit * pcUnit = UNITDATATOUNIT( UNITDATABYID( pcUnitData->iID ) );
	if( !pcUnit || !pcUnitData )
		return FALSE;

	//Update Everytime the Life
	auto pImageLifeBar = pWindowTarget->GetElement<UI::ImageBox>( IMAGEID_LifeBar );
	if ( pImageLifeBar )
		pImageLifeBar->SetWidth( (pcUnitData->sCharacterData.sHP.sCur * 208) / (pcUnitData->sCharacterData.sHP.sMax == 0 ? 1 : pcUnitData->sCharacterData.sHP.sMax) );

    //Update Range Text
    auto pTextRange = pWindowTarget->GetElement<UI::Text>( TEXTID_Range );
    if ( pTextRange )
    {
        int iDistanceA = 0, iDistanceB = 0, iDistanceC = 0;
        pcUnitData->sPosition.GetPTDistanceXZ( &UNITDATA->sPosition, iDistanceA, iDistanceB, iDistanceC );
        pTextRange->SetTextEx( "%d (%dm)", (int)sqrt( iDistanceA ), (int)sqrt( iDistanceA ) / 27 );
    }

    UpdateBattleInfo( pcUnit );

    auto pTextLife = pWindowTarget->GetElement<UI::Text>( TEXTID_Life );
	if( pTextLife )
	{
		pTextLife->SetTextEx( "%d/%d", pcUnit->sHPLong.iMin, pcUnit->sHPLong.iMax );

		//Percent
		auto pTextLifePercent = pWindowTarget->GetElement<UI::Text>( TEXTID_LifePercent );
		if ( pTextLifePercent )
		{
			if ( pcUnit->sHPLong.iMax > 0 )
			{
				int iPercent = (pcUnit->sHPLong.iMin * 100) / pcUnit->sHPLong.iMax;
				pTextLifePercent->SetTextEx( "%d%%", iPercent );
			}
		}

		if( pcUnit->sHPLong.iMin <= 0 || pcUnitData->sCharacterData.sHP.sCur <= 0 )
		{
			pcLastUnitDataTarget = NULL;
			iTargetID = 0;
			return FALSE;
		}
	}

	BOOL bSameTarget = FALSE;
	//Same Target? So don't update it
	if ( (((pcUnitData->sCharacterData.iType == CHARACTERTYPE_Player || pcUnitData->iCharacterType == CHARACTERTYPE_Player) || pcUnitData->sCharacterData.iType == CHARACTERTYPE_NPC) && iTargetID == pcUnitData->iID) || (pcUnitData->iCharacterType != CHARACTERTYPE_Player && pcUnitData->sCharacterData.iType == CHARACTERTYPE_Monster && iTargetID == pcUnitData->sCharacterData.iMonsterEffectID) )
		bSameTarget = TRUE;


	ClanInfo * psClanInfo = (ClanInfo*)0x03AA4608;

	if ( bSameTarget == FALSE )
	{
		bLoadClan16 = FALSE;
		bLoadClan32 = FALSE;
	}

	BOOL bBattleRoyale = BATTLEROYALEHANDLER->InWaiting() || BATTLEROYALEHANDLER->InBattle();

	//Clan Name and Icon
	auto pWindowTargetInfo = pWindowTarget->GetElement<UI::Window>( WINDOWID_TargetInfo );
	if ( pcUnitData->sCharacterData.iClanID && pcUnitData->iClanInfoIndex >= 0 && pWindowTargetInfo && (bLoadClan16 == FALSE) )
	{
		pWindowTargetInfo->Show();

		if ( bBattleRoyale )
			pWindowTargetInfo->Hide();

		//Clan Name
		auto pTextTargetInfo = pWindowTargetInfo->GetElement<UI::Text>( TEXTID_TargetInfo );
		if ( pTextTargetInfo )
			pTextTargetInfo->SetText( psClanInfo[pcUnitData->iClanInfoIndex].sHeader.szClanName );

		//Clan Icon
		auto pImageClanIcon = pWindowTargetInfo->GetElement<UI::ImageBox>( IMAGEID_ClanIcon );
		if ( pImageClanIcon )
		{
			if ( psClanInfo[pcUnitData->iClanInfoIndex].psTexture32 )
			{
				if ( FILEEXIST( psClanInfo[pcUnitData->iClanInfoIndex].psTexture32->sTexture.szName ) )
				{
					pImageClanIcon->SetImage( UI::ImageLoader::LoadImage( psClanInfo[pcUnitData->iClanInfoIndex].psTexture32->sTexture.szName ) );

					bLoadClan16 = TRUE;
				}
			}
		}
	}
	else if ( pWindowTargetInfo && bLoadClan16 == FALSE )
		pWindowTargetInfo->Hide();

	//Clan Info
	auto pCharacterInfoWindow = pWindowTarget->GetElement<UI::Window>( WINDOWID_CharacterInfo );
	if ( pCharacterInfoWindow && (bLoadClan32 == FALSE) )
	{
		auto pImageClanIcon = pCharacterInfoWindow->GetElement<UI::ImageBox>( IMAGEID_ClanIcon2 );
		if ( pImageClanIcon )
		{
			if ( psClanInfo[pcUnitData->iClanInfoIndex].psTexture32 )
			{
				if ( FILEEXIST( psClanInfo[pcUnitData->iClanInfoIndex].psTexture32->sTexture.szName ) )
				{
					pImageClanIcon->SetImage( UI::ImageLoader::LoadImage( psClanInfo[pcUnitData->iClanInfoIndex].psTexture32->sTexture.szName ) );

					bLoadClan32 = TRUE;
				}
			}
		}

		auto pTextClanInfo = pCharacterInfoWindow->GetElement<UI::Text>( TEXTID_ClanInfo );
		if ( pTextClanInfo )
			pTextClanInfo->SetText( trim( std::string( psClanInfo[pcUnitData->iClanInfoIndex].sHeader.szClanText ) ) );
	}

	//Same Target? So don't update it
	if( bSameTarget )
		return FALSE;

	//Update Achievements
	if ( pcUnit->vAchievement.size() > 0 )
	{
		auto pButtonAchievement = pWindowTarget->GetElement<UI::Button>( BUTTONID_Achievement );
		if ( pButtonAchievement )
			pButtonAchievement->Show();

		auto pImageAchievement = pWindowTarget->GetElement<UI::ImageBox>( IMAGEID_Achievement );
		if ( pImageAchievement )
			pImageAchievement->Show();

		auto pTooltipAchievement = pWindowTarget->GetElement<UI::Tooltip>( TOOLTIP_Achievement );
		if ( pTooltipAchievement )
			pTooltipAchievement->Show();
	}
	else
	{
		auto pButtonAchievement = pWindowTarget->GetElement<UI::Button>( BUTTONID_Achievement );
		if ( pButtonAchievement )
			pButtonAchievement->Hide();

		auto pImageAchievement = pWindowTarget->GetElement<UI::ImageBox>( IMAGEID_Achievement );
		if ( pImageAchievement )
			pImageAchievement->Hide();

		auto pTooltipAchievement = pWindowTarget->GetElement<UI::Tooltip>( TOOLTIP_Achievement );
		if ( pTooltipAchievement )
			pTooltipAchievement->Hide();
	}


	//Update Target Name
	auto pTextName = pWindowTarget->GetElement<UI::Text>( TEXTID_Name );
	if ( pTextName )
	{
		char * pszCharacterName = pcUnitData->sCharacterData.szName;
		if ( BATTLEROYALEHANDLER->InWaiting() || BATTLEROYALEHANDLER->InBattle() )
			pszCharacterName = "[Battle Royale Hidden Name]";

		pTextName->SetText( pszCharacterName );
	}

	//Update Target Level
	auto pTextLevel = pWindowTarget->GetElement<UI::Text>( TEXTID_Level );
	if( pTextLevel )
	{
		std::string strLevel = std::to_string( pcUnitData->sCharacterData.iLevel );
		bool bLevelType = strLevel.size() > 1 ? true : false;

		if( pcUnitData->sCharacterData.iType == CHARACTERTYPE_Player || pcUnitData->iCharacterType == CHARACTERTYPE_Player )
			pTextLevel->SetTextEx( "%s%c", bLevelType ? strLevel.substr( 0, strLevel.size() - 1 ).c_str() : strLevel.c_str(), bLevelType ? 'x' : NULL );
		else
			pTextLevel->SetTextEx( "%s", strLevel.c_str() );
	}

	//Show button more if it's a Player
	auto pButtonMore = pWindowTarget->GetElement<UI::Button>( BUTTONID_More );
	if( pButtonMore )
	{
		if( pcUnitData->sCharacterData.iType == CHARACTERTYPE_Player || pcUnitData->iCharacterType == CHARACTERTYPE_Player )
			pButtonMore->Show();
		else
			pButtonMore->Hide();
	}

	//Update Dropdown Menu
	auto pDropdownMenu = pWindowTarget->GetElement<UI::DropdownMenu>( DROPDOWNMENU_More );
	if( pDropdownMenu )
	{
		if( pcUnitData->iID != iTargetID )
			pDropdownMenu->Close();

		pDropdownMenu->Clear();

		//Enemy 
		if( pcUnitData->sCharacterData.iType != CHARACTERTYPE_Monster )
			pDropdownMenu->AddOption( pDropdownMenu->MakeOption( "Trade", Rectangle2D( 13, 5, 77, 18 ), FALSE, -1, D3DCOLOR_XRGB( 230, 180, 150 ) ), FALSE, 1 );

		//Others Options
		pDropdownMenu->AddOption( pDropdownMenu->MakeOption( "Add Friend", Rectangle2D( 13, 5, 77, 18 ), FALSE, -1, D3DCOLOR_XRGB( 230, 180, 150 ) ), FALSE, 2 );
		pDropdownMenu->AddOption( pDropdownMenu->MakeOption( "Party", Rectangle2D( 13, 5, 77, 18 ), FALSE, -1, D3DCOLOR_XRGB( 230, 180, 150 ) ), FALSE, 3 );

		//Raid
		if ( pcUnit && pcUnit->bPartyLeader )
		{
			if ( UNIT->bPartyLeader && pcUnit->bPartyLeader )
				pDropdownMenu->AddOption( pDropdownMenu->MakeOption( "Raid", Rectangle2D( 13, 5, 77, 18 ), FALSE, -1, D3DCOLOR_XRGB( 230, 180, 150 ) ), FALSE, 4 );
		}
		//It's a Clan Leader? So show the option to invite for clan
		if ( (READDWORD( 0x3B17830 ) == 101 || READDWORD( 0x3B17830 ) == 104) && !pcUnitData->sCharacterData.iClanID )
			pDropdownMenu->AddOption( pDropdownMenu->MakeOption( "Clan", Rectangle2D( 13, 5, 77, 18 ), FALSE, -1, D3DCOLOR_XRGB( 230, 180, 150 ) ), FALSE, 5 );
	}

	//Update Tooltip of Unit type
	auto pTooltipType = pWindowTarget->GetElement<UI::Tooltip>( TOOLTIP_TargetType );
	auto pImageType = pWindowTarget->GetElement<UI::ImageBox>( IMAGEID_Type );
	if( pImageType && pTooltipType )
	{
		if( pcUnitData->sCharacterData.iType == CHARACTERTYPE_Monster )
		{
			if( pcUnitData->sCharacterData.iMonsterType == MONSTERTYPE_Demon )
			{
				pImageType->SetImage( pcaImageTargetType[TARGETTYPE_Demon] );
				pTooltipType->SetText( "Demon" );
			}
			else if( pcUnitData->sCharacterData.iMonsterType == MONSTERTYPE_Mechanic )
			{
				pImageType->SetImage( pcaImageTargetType[TARGETTYPE_Mechanic] );
				pTooltipType->SetText( "Mechanic" );
			}
			else if( pcUnitData->sCharacterData.iMonsterType == MONSTERTYPE_Mutant )
			{
				pImageType->SetImage( pcaImageTargetType[TARGETTYPE_Mutant] );
				pTooltipType->SetText( "Mutant" );
			}
			else if( pcUnitData->sCharacterData.iMonsterType == MONSTERTYPE_Undead )
			{
				pImageType->SetImage( pcaImageTargetType[TARGETTYPE_Undead] );
				pTooltipType->SetText( "Undead" );
			}
			else if( pcUnitData->sCharacterData.iMonsterType == MONSTERTYPE_Normal )
			{
				pImageType->SetImage( pcaImageTargetType[TARGETTYPE_Normal] );
				pTooltipType->SetText( "Normal" );
			}
		}
		else if ( pcUnitData->sCharacterData.iMonsterType == MONSTERTYPE_Friendly )
		{
			pImageType->SetImage( pcaImageTargetType[TARGETTYPE_NPC] );
			pTooltipType->SetText( "Friendly" );
		}
		else if ( pcUnitData->sCharacterData.iType == CHARACTERTYPE_NPC )
		{
			pImageType->SetImage( pcaImageTargetType[TARGETTYPE_NPC] );
			pTooltipType->SetText( "NPC" );
		}
		else if( pcUnitData->sCharacterData.iType == CHARACTERTYPE_Player || pcUnitData->iCharacterType == CHARACTERTYPE_Player )
		{
			pImageType->SetImage( pcaImageTargetType[TARGETTYPE_Player] );
			pTooltipType->SetText( "Player" );
		}
	}


	//Update Last Target
	iTargetID = pcUnitData->iCharacterType == CHARACTERTYPE_Monster ? pcUnitData->sCharacterData.iMonsterEffectID : pcUnitData->iID;
	pcLastUnitDataTarget = pcUnitData;

	return TRUE;
}

void HUDController::UpdateBattleInfo( Unit * pcUnit )
{
    //Update Battle Text
    if ( (MAP_ID == MAPID_BlessCastle) && (BLESSCASTLEHANDLER->InSiegeWar() == FALSE) &&
        ((pcUnit->pcUnitData->sCharacterData.iType == CHARACTERTYPE_Player) || (pcUnit->pcUnitData->iCharacterType == CHARACTERTYPE_Player)) )
    {
        auto pTextBattlePoints = pWindowTarget->GetElement<UI::Text>( TEXTID_BattlePoints );
        if ( pTextBattlePoints )
        {
            pTextBattlePoints->SetTextEx( "%d Battle Points", pcUnit->iPvPKills );
            pTextBattlePoints->Show();
        }

        auto pTextBattleEXP = pWindowTarget->GetElement<UI::Text>( TEXTID_BattleEXP );
        if ( pTextBattleEXP )
        {
            pTextBattleEXP->SetTextEx( "%d Battle EXP", pcUnit->iPvPExperience );
            pTextBattleEXP->Show();
        }
    }
    else
    {
        auto pTextBattlePoints = pWindowTarget->GetElement<UI::Text>( TEXTID_BattlePoints );
        if ( pTextBattlePoints )
            pTextBattlePoints->Hide();

        auto pTextBattleEXP = pWindowTarget->GetElement<UI::Text>( TEXTID_BattleEXP );
        if ( pTextBattleEXP )
            pTextBattleEXP->Hide();
    }
}

void HUDController::SetRenderTarget( BOOL b )
{
	bRenderTarget = b;
	if ( b == FALSE )
		pcLastUnitDataTarget = NULL;
}

void HUDController::HandlePacket( PacketRollDiceRanking * psPacket )
{
	if( !psPacket )
		return;

	if( psPacket->szCharacterName[0] != 0 )
	{
		vRollDiceRanking.push_back( RollDiceRankingUser( psPacket->szCharacterName, psPacket->uNumber ) );
		std::sort( vRollDiceRanking.begin(), vRollDiceRanking.end(), RollNumberIsGreater );
	
		auto pListRankingLeft = pWindowRollDiceRanking->GetElement<UI::List>( LISTID_RankingLeft );
		auto pListRankingRight = pWindowRollDiceRanking->GetElement<UI::List>( LISTID_RankingRight );

		if( pListRankingLeft && pListRankingRight )
		{
			ClearRollDiceRanking();

			for( size_t i = 0; i < vRollDiceRanking.size(); i++ )
			{
				auto pWindowPlayer = i < 5 ? pListRankingLeft->GetWindow( WINDOWID_Player1 + i ) : pListRankingRight->GetWindow( WINDOWID_Player1 + ( i - 5 ) );

				if( pWindowPlayer )
				{
					auto pTextCharacterName = i < 5 ? pWindowPlayer->GetElement<UI::Text>( TEXTID_CharacterNick1 + i ) : pWindowPlayer->GetElement<UI::Text>( TEXTID_CharacterNick1 + i - 5 );
					auto pTextNumber = i < 5 ? pWindowPlayer->GetElement<UI::Text>( TEXTID_Number1 + i ) : pWindowPlayer->GetElement<UI::Text>( TEXTID_Number1 + i - 5 );

					if( pTextCharacterName && pTextNumber )
					{
						pTextCharacterName->SetText( vRollDiceRanking[i].szCharacterName );
						pTextNumber->SetTextEx( "%d", vRollDiceRanking[i].uNumber );
					}
				}
			}
		}
	}
}

void HUDController::ClearRollDiceRanking( bool bResetVetor )
{
	if( bResetVetor )
		vRollDiceRanking.clear();

	auto pListRankingLeft = pWindowRollDiceRanking->GetElement<UI::List>( LISTID_RankingLeft );
	auto pListRankingRight = pWindowRollDiceRanking->GetElement<UI::List>( LISTID_RankingRight );

	if( pListRankingLeft && pListRankingRight )
	{
		for( size_t i = 0; i < 10; i++ )
		{
			auto pWindowPlayer = i < 5 ? pListRankingLeft->GetWindow( WINDOWID_Player1 + i ) : pListRankingRight->GetWindow( WINDOWID_Player1 + ( i - 5 ) );

			if( pWindowPlayer )
			{
				auto pTextCharacterName = i < 5 ? pWindowPlayer->GetElement<UI::Text>( TEXTID_CharacterNick1 + i ) : pWindowPlayer->GetElement<UI::Text>( TEXTID_CharacterNick1 + i - 5 );
				auto pTextNumber = i < 5 ? pWindowPlayer->GetElement<UI::Text>( TEXTID_Number1 + i ) : pWindowPlayer->GetElement<UI::Text>( TEXTID_Number1 + i - 5 );

				if( pTextCharacterName && pTextNumber )
				{
					pTextCharacterName->SetText( "" );
					pTextNumber->SetText( "" );
				}
			}
		}
	}
}

void HUDController::BuildTargetWindow1()
{
	pWindowTarget->SetMoveBox( pWindowTarget->GetRect() );
	pWindowTarget->SetCanMove( !SETTINGSHANDLER->GetModel()->GetSettings().bLockUI );

	//Dropdown Menu
	UI::DropdownMenu_ptr pDropdownMenu = std::make_shared<UI::DropdownMenu>( Rectangle2D( 301, 16, 94, 24 ) );
	pDropdownMenu->SetID( DROPDOWNMENU_More );
	pDropdownMenu->SetDropdownPosition( UI::DROPDOWNPOSITION_Left );
	pDropdownMenu->SetEvent( UI::Event::Build( std::bind( &HUDController::OnSelectTargetMenu, this ) ) );
	pDropdownMenu->SetMenuBackground( "game\\images\\UI\\menu\\topleft.png", "game\\images\\UI\\menu\\middle.png", "game\\images\\UI\\menu\\bottom.png", "game\\images\\UI\\menu\\main.png" );
	pWindowTarget->AddElement( pDropdownMenu );

	//Background Target Info
	UI::Window_ptr pWindowTargetInfo = std::make_shared<UI::Window>( Rectangle2D( 41, 52, 229, 22 ) );
	pWindowTargetInfo->SetID( WINDOWID_TargetInfo );
	{
		UI::ImageBox_ptr pImageBackgroundTargetInfo = std::make_shared<UI::ImageBox>( Rectangle2D( 0, 0, 229, 22 ) );
		pImageBackgroundTargetInfo->SetID( IMAGEID_TargetInfo );
		pImageBackgroundTargetInfo->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\target\\targetinfo.png" ) );
		pWindowTargetInfo->AddElement( pImageBackgroundTargetInfo );

		UI::Text_ptr pTextTargetInfo = std::make_shared<UI::Text>( Rectangle2D( 81, 4, 150, 18 ) );
		pTextTargetInfo->SetFont( "Arial", 15, 0, FALSE, FALSE, D3DCOLOR_XRGB( 180, 180, 125 ) );
		pTextTargetInfo->SetID( TEXTID_TargetInfo );
		pTextTargetInfo->SetDropdownShadow( TRUE );
		pTextTargetInfo->SetDropdownColor( D3DCOLOR_XRGB( 0, 0, 0 ) );
		pTextTargetInfo->SetNoClip( TRUE );
		pWindowTargetInfo->AddElement( pTextTargetInfo );

		UI::ImageBox_ptr pImageIconClan = std::make_shared<UI::ImageBox>( Rectangle2D( 61 + 64, 8, 32, 32 ) );
		pImageIconClan->SetScale( 0.5f );
		pImageIconClan->SetUseSelfScale( true );
		pImageIconClan->SetID( IMAGEID_ClanIcon );
		pWindowTargetInfo->AddElement( pImageIconClan );
	}
	pWindowTargetInfo->Hide();
	pWindowTarget->AddElement( pWindowTargetInfo );

	//Character Info
	UI::Window_ptr pWindowCharacterInfo = std::make_shared<UI::Window>( Rectangle2D( 52, 76, 210, 59 ) );
	pWindowCharacterInfo->SetID( WINDOWID_CharacterInfo );
	pWindowCharacterInfo->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\target\\infobackground.png" ) );
	{
		UI::ImageBox_ptr pImageIconClan2 = std::make_shared<UI::ImageBox>( Rectangle2D( 13, 15, 32, 32 ) );
		pImageIconClan2->SetID( IMAGEID_ClanIcon2 );
		pWindowCharacterInfo->AddElement( pImageIconClan2 );

		UI::Text_ptr pTextClanInfo = std::make_shared<UI::Text>( Rectangle2D( 55, 16, 140, 32 ) );
		pTextClanInfo->SetFont( "Arial", 15, 0, FALSE, FALSE, -1 );
		pTextClanInfo->SetID( TEXTID_ClanInfo );
		pTextClanInfo->SetMultiLine( TRUE );
		pTextClanInfo->SetWordWrap( TRUE );
		pTextClanInfo->SetNoClip( TRUE );
		pWindowCharacterInfo->AddElement( pTextClanInfo );
	}
	pWindowCharacterInfo->Hide();
	pWindowTarget->AddElement( pWindowCharacterInfo );

	//Background Image
	UI::ImageBox_ptr pImageBackgroundHead = std::make_shared<UI::ImageBox>( Rectangle2D( 7, 11, 59, 59 ) );
	pImageBackgroundHead->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\target\\targetbackground.png" ) );
	pWindowTarget->AddElement( pImageBackgroundHead );

	//Overlay
	UI::ImageBox_ptr pImageOverlayTarget = std::make_shared<UI::ImageBox>( Rectangle2D( 0, 0, 301, 80 ) );
	pImageOverlayTarget->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\target\\targetoverlay.png" ) );
	pWindowTarget->AddElement( pImageOverlayTarget );

	//Life
	UI::ImageBox_ptr pImageLifeBar = std::make_shared<UI::ImageBox>( Rectangle2D( 82, 39, 208, 12 ) );
	pImageLifeBar->SetID( IMAGEID_LifeBar );
	pImageLifeBar->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\target\\targethp.png" ) );
	pWindowTarget->AddElement( pImageLifeBar );

	//Type
	UI::ImageBox_ptr pImageType = std::make_shared<UI::ImageBox>( Rectangle2D( 62, 34, 16, 16 ) );
	pImageType->SetID( IMAGEID_Type );
	pImageType->SetImage( pcaImageTargetType[TARGETTYPE_NPC] );
	pWindowTarget->AddElement( pImageType );

	//Name of Unit
	UI::Text_ptr pTextName = std::make_shared<UI::Text>( Rectangle2D( 88, 21, 195, 18 ) );
	pTextName->SetFont( "Arial", 15, 0, FALSE, FALSE, -1 );
	pTextName->SetID( TEXTID_Name );
	pTextName->SetHorizontalAlign( ALIGN_Center );
	pTextName->SetDropdownShadow( TRUE );
	pTextName->SetDropdownColor( D3DCOLOR_XRGB( 0, 0, 0 ) );
	pTextName->SetNoClip( TRUE );
	pWindowTarget->AddElement( pTextName );

	//Life of Unit
	UI::Text_ptr pTextLife = std::make_shared<UI::Text>( Rectangle2D( 84, 38, 204, 14 ) );
	pTextLife->SetFont( "Arial", 13, 0, FALSE, FALSE, -1 );
	pTextLife->SetID( TEXTID_Life );
	pTextLife->SetHorizontalAlign( ALIGN_Center );
	pTextLife->SetNoClip( TRUE );
	pWindowTarget->AddElement( pTextLife );

	//Level of Unit
	UI::Text_ptr pTextLevel = std::make_shared<UI::Text>( Rectangle2D( 10, 29, 52, 30 ) );
	pTextLevel->SetFont( "Frutiger LT 55 Roman", 26, 0, FALSE, TRUE, -1 );
	pTextLevel->SetID( TEXTID_Level );
	pTextLevel->SetColor( D3DCOLOR_XRGB( 227, 191, 152 ) );
	pTextLevel->SetHorizontalAlign( ALIGN_Center );
	pTextLevel->SetNoClip( TRUE );
	pWindowTarget->AddElement( pTextLevel );

	//Life Percent
	UI::Text_ptr pTextLifePercent = std::make_shared<UI::Text>( Rectangle2D( 0, 37, 288, 20 ) );
	pTextLifePercent->SetFont( "Arial", 14, 0, FALSE, FALSE, -1 );
	pTextLifePercent->SetID( TEXTID_LifePercent );
	pTextLifePercent->SetHorizontalAlign( ALIGN_Right );
	pTextLifePercent->SetNoClip( TRUE );
	pWindowTarget->AddElement( pTextLifePercent );

    //Range
    UI::Text_ptr pTextRange = std::make_shared<UI::Text>( Rectangle2D( 0, 56, 293, 20 ) );
    pTextRange->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
    pTextRange->SetID( TEXTID_Range );
    pTextRange->SetHorizontalAlign( ALIGN_Right );
    pTextRange->SetNoClip( TRUE );
    pWindowTarget->AddElement( pTextRange );

    //Battle Points
    UI::Text_ptr pTextBattlePoints = std::make_shared<UI::Text>( Rectangle2D( 0, 76, 148, 20 ) );
    pTextBattlePoints->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
    pTextBattlePoints->SetID( TEXTID_BattlePoints );
    pTextBattlePoints->SetHorizontalAlign( ALIGN_Center );
    pTextBattlePoints->SetNoClip( TRUE );
    pWindowTarget->AddElement( pTextBattlePoints );

    //Battle EXP
    UI::Text_ptr pTextBattleEXP = std::make_shared<UI::Text>( Rectangle2D( 148, 76, 148, 20 ) );
    pTextBattleEXP->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
    pTextBattleEXP->SetID( TEXTID_BattleEXP );
    pTextBattleEXP->SetHorizontalAlign( ALIGN_Center );
    pTextBattleEXP->SetNoClip( TRUE );
    pWindowTarget->AddElement( pTextBattleEXP );

    //More Button
	UI::Button_ptr pButtonMore = std::make_shared<UI::Button>( Rectangle2D( 262, 16, 25, 25 ) );
	pButtonMore->SetID( BUTTONID_More );
	pButtonMore->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\hud\\target\\more.png", "game\\images\\UI\\hud\\target\\more_.png" ) );
	pButtonMore->SetDropdownMenu( pDropdownMenu );
	pWindowTarget->AddElement( pButtonMore );

	//Button Achievement
	UI::Button_ptr pButtonAchievement = std::make_shared<UI::Button>( Rectangle2D( 80, 56, 16, 16 ) );
	pButtonAchievement->SetID( BUTTONID_Achievement );
	pButtonAchievement->SetEvent( UI::Event::Build( std::bind( &HUDController::OnAchievementButtonClick, this, std::placeholders::_1 ) ) );
	pWindowTarget->AddElement( pButtonAchievement );

	UI::ImageBox_ptr pImageAchievement = std::make_shared<UI::ImageBox>( Rectangle2D( 80 + 80, 56 + 56, 32, 32 ) );
	pImageAchievement->SetID( IMAGEID_Achievement );
	pImageAchievement->SetImage( UI::ImageLoader::LoadImage( "game\\images\\achievements\\achievement_btn.png" ) );
	pImageAchievement->SetScale( 0.5f );
	pImageAchievement->SetUseSelfScale( true );
	pWindowTarget->AddElement( pImageAchievement );

	//Tooltip of Target Type
	UI::Tooltip_ptr pTooltipAchievement = std::make_shared<UI::Tooltip>( Rectangle2D() );
	pTooltipAchievement->Init();
	pTooltipAchievement->SetID( TOOLTIP_Achievement );
	pTooltipAchievement->SetLinkObject( pButtonAchievement );
	pTooltipAchievement->SetFont( "Arial", 16, -1 );
	pTooltipAchievement->SetText( "Achievements" );
	pTooltipAchievement->SetPosition( TOOLTIPPOSITION_Top );
	pTooltipAchievement->SetFollowMouse( FALSE );
	pWindowTarget->AddElement( pTooltipAchievement );

	//Tooltip of Target Type
	UI::Tooltip_ptr pTooltipType = std::make_shared<UI::Tooltip>( Rectangle2D() );
	pTooltipType->Init();
	pTooltipType->SetID( TOOLTIP_TargetType );
	pTooltipType->SetLinkObject( pImageType );
	pTooltipType->SetFont( "Arial", 16, -1 );
	pTooltipType->SetPosition( TOOLTIPPOSITION_Top );
	pTooltipType->SetFollowMouse( FALSE );
	pWindowTarget->AddElement( pTooltipType );
}

void HUDController::BuildDisplayWindow1()
{
	pWindowDisplay->SetMoveBox( pWindowDisplay->GetRect() );
	pWindowDisplay->SetCanMove( TRUE );

	UI::ImageBox_ptr pImageLeftSkill = std::make_shared<UI::ImageBox>( Rectangle2D( 145, 0, 35, 35 ) );
	pImageLeftSkill->SetID( 1 );
	pWindowDisplay->AddElement( pImageLeftSkill );

	for( size_t i = 0; i < 8; i++ )
	{
		UI::ImageBox_ptr pImageSkill = std::make_shared<UI::ImageBox>( Rectangle2D( (i == 4 ? 230 : 0) + (35 * i), 0, 35, 35 ) );
		pImageSkill->SetID( 2 + i );
		pWindowDisplay->AddElement( pImageSkill );
	}

	pWindowDisplay->Hide();
}

int HUDController::GetShortcutSkill( int iShortcut )
{
	for( size_t i = 0; i < 20; i++ )
	{
		if( USESKILLS.saUseSkill[i].ShortKey == iShortcut )
			return i;
	}

	return -1;
}

void HUDController::BuildDiceRollRankingWindow1()
{
	pWindowRollDiceRanking->SetImage( UI::ImageLoader::LoadImage( "game\\images\\diceroll\\background.png" ) );

	UI::ImageBox_ptr pDivRankingImage = std::make_shared<UI::ImageBox>( Rectangle2D( 7, 23, 365, 95 ) );
	pDivRankingImage->SetImage( UI::ImageLoader::LoadImage( "game\\images\\diceroll\\div.png" ) );
	pWindowRollDiceRanking->AddElement( pDivRankingImage );

	UI::Text_ptr pTextLabel = std::make_shared<UI::Text>( Rectangle2D( 0, 4, 379, 23 ) );
	pTextLabel->SetText( "TOP ROLLS" );
	pTextLabel->SetFont( "Arial", 15, 0, FALSE, TRUE, D3DCOLOR_XRGB( 200, 190, 150 ) );
	pTextLabel->SetNoClip( TRUE );
	pTextLabel->SetHorizontalAlign( ALIGN_Center );
	pWindowRollDiceRanking->AddElement( pTextLabel );

	//Labels Ranking
	for( size_t i = 0; i < 2; i++ )
	{
		UI::Text_ptr pTextID = std::make_shared<UI::Text>( Rectangle2D( 7 + (192 * i), 24, 20, 15 ) );
		pTextID->SetText( "#" );
		pTextID->SetFont( "Arial", 13, 0, FALSE, TRUE, -1 );
		pTextID->SetNoClip( TRUE );
		pTextID->SetHorizontalAlign( ALIGN_Center );
		pWindowRollDiceRanking->AddElement( pTextID );

		UI::Text_ptr pTextCharacterNick = std::make_shared<UI::Text>( Rectangle2D( 33 + (192 * i), 24, 0, 0 ) );
		pTextCharacterNick->SetText( "Character Name" );
		pTextCharacterNick->SetFont( "Arial", 13, 0, FALSE, TRUE, -1 );
		pTextCharacterNick->SetNoClip( TRUE );
		pWindowRollDiceRanking->AddElement( pTextCharacterNick );

		UI::Text_ptr pTextNumber = std::make_shared<UI::Text>( Rectangle2D( 134 + (192 * i), 24, 0, 0 ) );
		pTextNumber->SetText( "Number" );
		pTextNumber->SetFont( "Arial", 13, 0, FALSE, TRUE, -1 );
		pTextNumber->SetNoClip( TRUE );
		pWindowRollDiceRanking->AddElement( pTextNumber );
	}

	UI::List_ptr pRankingListLeft = std::make_shared<UI::List>( Rectangle2D( 7, 38, 173, 95 ) );
	pRankingListLeft->SetID( LISTID_RankingLeft );
	pRankingListLeft->SetPaddingSeparator( 1 );
	pWindowRollDiceRanking->AddElement( pRankingListLeft );

	UI::List_ptr pRankingListRight = std::make_shared<UI::List>( Rectangle2D( 199, 38, 173, 95 ) );
	pRankingListRight->SetID( LISTID_RankingRight );
	pRankingListRight->SetPaddingSeparator( 1 );
	pWindowRollDiceRanking->AddElement( pRankingListRight );

	//Ranking Player
	for( size_t i = 0; i < 5; i++ )
	{
		UI::Window_ptr pWindowPlayer = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 174, 15 ) );
		pWindowPlayer->SetID( WINDOWID_Player1 + i );

		UI::Text_ptr pTextID = std::make_shared<UI::Text>( Rectangle2D( 0, 1, 20, 15 ) );
		pTextID->SetText( std::to_string( i + 1 ) );
		pTextID->SetFont( "Arial", 13, 0, FALSE, TRUE, D3DCOLOR_XRGB( 192, 144, 97 ) );
		pTextID->SetNoClip( TRUE );
		pTextID->SetHorizontalAlign( ALIGN_Center );
		pWindowPlayer->AddElement( pTextID );

		UI::Text_ptr pTextCharacterNick = std::make_shared<UI::Text>( Rectangle2D( 26, 1, 0, 0 ) );
		pTextCharacterNick->SetID( TEXTID_CharacterNick1 + i );
		pTextCharacterNick->SetText( "" );
		pTextCharacterNick->SetFont( "Arial", 13, 0, FALSE, TRUE, D3DCOLOR_XRGB( 192, 144, 97 ) );
		pTextCharacterNick->SetNoClip( TRUE );
		pWindowPlayer->AddElement( pTextCharacterNick );

		UI::Text_ptr pTextNumber = std::make_shared<UI::Text>( Rectangle2D( 127, 1, 0, 0 ) );
		pTextNumber->SetID( TEXTID_Number1 + i );
		pTextNumber->SetText( "" );
		pTextNumber->SetFont( "Arial", 13, 0, FALSE, TRUE, D3DCOLOR_XRGB( 192, 144, 97 ) );
		pTextNumber->SetNoClip( TRUE );
		pWindowPlayer->AddElement( pTextNumber );

		pRankingListLeft->AddWindow( pWindowPlayer );
	}

	//Ranking Player
	for( size_t i = 0; i < 5; i++ )
	{
		UI::Window_ptr pWindowPlayer = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 174, 15 ) );
		pWindowPlayer->SetID( WINDOWID_Player1 + i );

		UI::Text_ptr pTextID = std::make_shared<UI::Text>( Rectangle2D( 0, 1, 20, 15 ) );
		pTextID->SetText( std::to_string( i + 6 ) );
		pTextID->SetFont( "Arial", 13, 0, FALSE, TRUE, D3DCOLOR_XRGB( 192, 144, 97 ) );
		pTextID->SetNoClip( TRUE );
		pTextID->SetHorizontalAlign( ALIGN_Center );
		pWindowPlayer->AddElement( pTextID );

		UI::Text_ptr pTextCharacterNick = std::make_shared<UI::Text>( Rectangle2D( 26, 1, 0, 0 ) );
		pTextCharacterNick->SetID( TEXTID_CharacterNick1 + i );
		pTextCharacterNick->SetText( "" );
		pTextCharacterNick->SetFont( "Arial", 13, 0, FALSE, TRUE, D3DCOLOR_XRGB( 192, 144, 97 ) );
		pTextCharacterNick->SetNoClip( TRUE );
		pWindowPlayer->AddElement( pTextCharacterNick );

		UI::Text_ptr pTextNumber = std::make_shared<UI::Text>( Rectangle2D( 127, 1, 0, 0 ) );
		pTextNumber->SetID( TEXTID_Number1 + i );
		pTextNumber->SetText( "" );
		pTextNumber->SetFont( "Arial", 13, 0, FALSE, TRUE, D3DCOLOR_XRGB( 192, 144, 97 ) );
		pTextNumber->SetNoClip( TRUE );
		pWindowPlayer->AddElement( pTextNumber );

		pRankingListRight->AddWindow( pWindowPlayer );
	}

	UI::Text_ptr pTextClose = std::make_shared<UI::Text>( Rectangle2D( 360, 4, 16, 16 ) );
	pTextClose->SetText( "X" );
	pTextClose->SetFont( "Arial", 15, 0, FALSE, TRUE, UI::Color( 255, 175, 175, 175 ).Get() );
	pTextClose->SetEvent( UI::Event::Build( std::bind( &HUDController::CloseDiceRollRanking, this ) ) );
	pTextClose->SetColorHover( -1 );
	pTextClose->SetNoClip( TRUE );
	pWindowRollDiceRanking->AddElement( pTextClose );
}

void HUDController::OnAchievementButtonClick( UIEventArgs eArgs )
{
	Unit * pcUnit = UNITDATATOUNIT( GetTargetUnitData() );

	if ( pcUnit )
	{
		pcAchievementHandler->GetWindow()->UpdateWindowList( pcUnit, CAchievementWindow::EAchievementWindow::ACHIEVEMENTWINDOW_Target );
	}
}

void HUDController::OnAcceptTradeRequest( int iTargetID )
{
	if( MESSAGEBOX->GetType() )
		CALL_WITH_ARG2( 0x000628290, iTargetID, 1 );
}

BOOL HUDController::IsWindowOldHUDOpen()
{
	if ( MIXWINDOW_OPEN || CRAFTWINDOW_OPEN || BLACKSMITHWINDOW_OPEN || SMELTWINDOW_OPEN || RESETMIXWINDOW_OPEN || 
		BELLATRARANKINGWINDOW_OPEN || CLANMANAGERWINDOW_OPEN || PERSONALSHOPHANDLER->GetWindow()->IsOpen() || CARAVANHANDLER->GetWindow()->IsOpen() ||
	    ITEMBOXHANDLER->GetWindow()->IsOpen() )
		return TRUE;


	struct WindowQuestOldData
	{
		BOOL		bOpen;

		char		cPadding001[0xE64];
	};

	WindowQuestOldData * psaQuestWindowOld = ((WindowQuestOldData*)0x035D1178);
	for ( int i = 0; i < 10; i++ )
	{
		WindowQuestOldData * ps = psaQuestWindowOld + i;
		if ( ps->bOpen )
			return TRUE;
	}


	return FALSE;
}

BOOL HUDController::OnMouseClick( class CMouse * pcMouse )
{
	BOOL bWindowOldOpen = IsWindowOldHUDOpen();

	if ( bWindowOldOpen == FALSE )
	{
		if ( pcPartyHandler->OnMouseClick( pcMouse ) )
			return TRUE;

		if ( pWindowTarget->OnMouseClick( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 ) )
			return TRUE;

		if ( pcChatBox->OnMouseClick( pcMouse ) )
			return TRUE;

		if ( pcAchievementHandler->GetWindow()->OnMouseClick( pcMouse ) )
			return TRUE;
	}
	else
	{
		if ( PERSONALSHOPHANDLER->GetWindow()->IsOpen() )
		{
			if ( pcChatBox->OnMouseClick( pcMouse ) )
				return TRUE;
		}
	}

	if( pWindowDisplay->OnMouseClick( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 ) )
		return TRUE;
	
	if( pcInventory->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcInventoryWindowHandler->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( bWindowOldOpen == FALSE )
	{
		if ( pWindowTarget->IsHover() )
			return TRUE;
	}

	if( pcMiniMapHandler->OnMouseClick( pcMouse ) )
		return TRUE;

	if( pWindowRollDiceRanking->OnMouseClick( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 ) )
		return TRUE;

	return FALSE;
}

void HUDController::OnMouseMove( class CMouse * pcMouse )
{
	pcInventoryWindowHandler->OnMouseMove( pcMouse );

	pcChatBox->OnMouseMove( pcMouse );
	pcPartyHandler->OnMouseMove( pcMouse );
	pWindowTarget->OnMouseMove( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
	pWindowDisplay->OnMouseMove( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
	pcInventory->OnMouseMove( pcMouse );
	pcMiniMapHandler->OnMouseMove( pcMouse );
	pcAchievementHandler->GetWindow()->OnMouseMove( pcMouse );
	pWindowRollDiceRanking->OnMouseMove( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );

	Rectangle2D sBox = Rectangle2D( 364, RESOLUTION_HEIGHT - 12, 72, 12 );

	Rectangle2D sBoxBar[3];
	sBoxBar[0] = Rectangle2D( 305, RESOLUTION_HEIGHT - 80, 8, 82 );
	sBoxBar[1] = Rectangle2D( 320, RESOLUTION_HEIGHT - 100, 15, 100 );
	sBoxBar[2] = Rectangle2D( 465, RESOLUTION_HEIGHT - 100, 15, 100 );

	if( sBox.Inside( pcMouse->GetPosition() ) )
	{
		if( !pcTooltipWorldTime->IsHover() )
		{
			pcTooltipWorldTime->SetBox( sBox );
			pcTooltipWorldTime->SetHover( TRUE );
		}
	}
	else
		pcTooltipWorldTime->SetHover( FALSE );

	for( int i = 0; i < 3; i++ )
	{
		if( sBoxBar[i].Inside( pcMouse->GetPosition() ) )
		{
			if( !pcTooltipBars[i]->IsHover() )
			{
				pcTooltipBars[i]->SetBox( sBoxBar[i] );
				pcTooltipBars[i]->SetHover( TRUE );
			}
		}
		else
			pcTooltipBars[i]->SetHover( FALSE );
	}
}

BOOL HUDController::OnKeyPress( class CKeyboard * pcKeyboard )
{
	if ( (*(BOOL*)0x03642090) )
	{
		if ( pcKeyboard->GetKey() == VK_ESCAPE )
		{
			ZeroMemory( (void*)0x03642094, 0x267A * 4 );
			(*(BOOL*)0x03642090) = FALSE;
			return TRUE;
		}
	}

	if( pcChatBox->OnKeyPress( pcKeyboard ) )
		return TRUE;

	return FALSE;
}

BOOL HUDController::OnMouseScroll( CMouse * pcMouse )
{
	if( pcChatBox->OnMouseScroll( pcMouse ) )
		return TRUE;

	if ( pcAchievementHandler->GetWindow()->OnMouseScroll( pcMouse ) )
		return TRUE;

	return FALSE;
}

BOOL HUDController::OnKeyChar( CKeyboard * pcKeyboard )
{
	if( pcChatBox->OnKeyChar( pcKeyboard ) )
		return TRUE;

	return FALSE;
}

void HUDController::RenderFriendStatus()
{
}

void HUDController::Render()
{
	//Render Inventory
	pcInventoryWindowHandler->Render();

	//Render
	if ( ISSHOW_INTERFACE )
	{
		ITEMTIMERHANDLER->Render();
		pcMiniMapHandler->Render();
		pcPartyHandler->Render();
		pcChatBox->Render();
		pcInventory->Render();
		pWindowTarget->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
		pWindowDisplay->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
		pWindowRollDiceRanking->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
		pcTooltipWorldTime->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
		pcAchievementHandler->Render();
		ACTIONFIELDHANDLER->Render();


		for ( int i = 0; i < 3; i++ )
			pcTooltipBars[i]->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
	}

	//Render FPS
	if( SETTINGSHANDLER->GetModel()->GetSettings().bFPS )
	{
		int iFPS = *(int*)0x0A1755C;
		DWORD dwColor = D3DCOLOR_RGBA( 50, 205, 50, 255 );

		if( iFPS < 60 )
		{
			dwColor = D3DCOLOR_RGBA( 255, 255, 0, 255 );

			if( iFPS < 41 )
				dwColor = D3DCOLOR_RGBA( 255, 0, 0, 255 );
		}

		pFont->Draw( FormatString( "%d", iFPS ), RECT{ 0,0,RESOLUTION_WIDTH - 5,30 }, DT_NOCLIP | DT_RIGHT | DT_VCENTER, dwColor );
	}
}

void HUDController::Init()
{
	pFont = GetGraphicsDevice()->GetFontFactory()->MakeFont( nullptr, "Arial", 20, 0, TRUE, FALSE );

	pcTooltipWorldTime->Init();
	pcPartyHandler->Init();	
	pcChatBox->Init();
	pcInventory->Init();
	pcMiniMapHandler->Init();
	pcAchievementHandler->Init();

	HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->LoadCacheItems();

	BuildTargetWindow1();
	BuildDisplayWindow1();
	BuildDiceRollRankingWindow1();

	//Tooltip World Time
	pcTooltipWorldTime->SetFont( "Arial", 16, -1 );
	pcTooltipWorldTime->SetPosition( TOOLTIPPOSITION_Top );
	pcTooltipWorldTime->SetFollowMouse( FALSE );
	pcTooltipWorldTime->SetHover( FALSE );
	pcTooltipWorldTime->Open();

	for( int i = 0; i < 3; i++ )
	{
		pcTooltipBars[i]->Init();
		pcTooltipBars[i]->SetFont( "Arial", 16, -1 );
		pcTooltipBars[i]->SetPosition( TOOLTIPPOSITION_Right );
		pcTooltipBars[i]->SetFollowMouse( FALSE );
		pcTooltipBars[i]->SetHover( FALSE );
		pcTooltipBars[i]->Open();
	}

	pcaImageCrystalTowerHPBar[0] = UI::ImageLoader::LoadImageA( "game\\images\\UI\\hud\\hpbar_crystal.png" );
	pcaImageCrystalTowerHPBar[1] = UI::ImageLoader::LoadImageA( "game\\images\\UI\\hud\\hpbar_crystal_.png" );

	//Load Images from Skills
	if( UNITDATA )
	{
		for( size_t i = 1; i < 20; i++ )
			pcaImageSkillsIcon[0][i] = UI::ImageLoader::LoadImage( "image\\Sinimage\\skill\\%s\\Button\\%s.bmp", GetCharacterClassByFlag(UNITDATA->sCharacterData.iFlag), USESKILLS.saUseSkill[i].szFileName );
	
		for( size_t i = 1; i < 20; i++ )
			pcaImageSkillsIcon[1][i] = UI::ImageLoader::LoadImage( "image\\Sinimage\\skill\\%s\\Button\\%s_.bmp", GetCharacterClassByFlag(UNITDATA->sCharacterData.iFlag), USESKILLS.saUseSkill[i].szFileName );
	}

	//Load Images from Target Type
	pcaImageTargetType[TARGETTYPE_Demon] = UI::ImageLoader::LoadImageA( "game\\images\\UI\\hud\\target\\type\\demon.png" );
	pcaImageTargetType[TARGETTYPE_Mechanic] = UI::ImageLoader::LoadImageA( "game\\images\\UI\\hud\\target\\type\\mechanic.png" );
	pcaImageTargetType[TARGETTYPE_Mutant] = UI::ImageLoader::LoadImageA( "game\\images\\UI\\hud\\target\\type\\mutant.png" );
	pcaImageTargetType[TARGETTYPE_Normal] = UI::ImageLoader::LoadImageA( "game\\images\\UI\\hud\\target\\type\\normal.png" );
	pcaImageTargetType[TARGETTYPE_Undead] = UI::ImageLoader::LoadImageA( "game\\images\\UI\\hud\\target\\type\\undead.png" );
	pcaImageTargetType[TARGETTYPE_NPC] = UI::ImageLoader::LoadImageA( "game\\images\\UI\\hud\\target\\type\\npc.png" );
	pcaImageTargetType[TARGETTYPE_Player] = UI::ImageLoader::LoadImageA( "game\\images\\UI\\hud\\target\\type\\player.png" );

	UpdateObjectsSettings();
}

void HUDController::OnResolutionChanged()
{
	//Update Target Position
	pWindowTarget->SetPosition( (RESOLUTION_WIDTH >> 1) - 150, 0 );

	//Update Display Position
	pWindowDisplay->SetPosition( (RESOLUTION_WIDTH >> 1) - (pWindowDisplay->GetWidth() >> 1), RESOLUTION_HEIGHT - 150 );

	//Update Dice Roll Ranking Position
	pWindowRollDiceRanking->SetPosition( (RESOLUTION_WIDTH >> 1) - (pWindowRollDiceRanking->GetWidth() >> 1), (RESOLUTION_HEIGHT >> 1) + 95 );

	pcChatBox->OnResolutionChanged();
	pcMiniMapHandler->OnResolutionChanged();
	pcAchievementHandler->GetWindow()->OnResolutionChanged();

	pcPartyHandler->GetPartyCore()->OnResolutionChanged();

	//Update Potion Slot Position
	/*for( size_t i = 0; i < 3; i++ )
	{
		INVENTORYITEMSLOT[10+i].sBox = Rectangle2D( (RESOLUTION_WIDTH >> 1) + 71 + (30 * i), 472, (RESOLUTION_WIDTH >> 1) + 71 + (30 * i) + 26, 472 + 26 );

		INVENTORYITEMS[INVENTORYITEMSLOT[10+i].iItemIndex - 1].sPosition.iX = INVENTORYITEMSLOT[10+i].sBox.iX;
		INVENTORYITEMS[INVENTORYITEMSLOT[10+i].iItemIndex - 1].sPosition.iY = INVENTORYITEMSLOT[10+i].sBox.iY;
	}

	if( GAMESCREEN->GetActiveScreenType() == SCREEN_World )
	{
		for( size_t i = 0; i < 21; i++ )
		{
			//USESKILLS.pLeftSkill->szFileName
			if( USESKILLS.saUseSkill[i].ShortKey )
				WRITEDBG( "%s %d", USESKILLS.saUseSkill[i].szFileName, USESKILLS.saUseSkill[i].ShortKey );
		}
	}*/
}

void HUDController::Update( float fTime )
{
	GLOBALWINDOW_BLOCKACTION = IsWindowOldHUDOpen();

	//Check if need to render the Target
	if( bRenderTarget && !pWindowTarget->IsOpen() )
		pWindowTarget->Show();
	else if( !bRenderTarget && pWindowTarget->IsOpen() )
		pWindowTarget->Hide();

	if ( BATTLEROYALEHANDLER->InWaiting() || BATTLEROYALEHANDLER->InBattle() )
		pWindowTarget->Hide();

	//Check if need to show the Character Info Window
	auto pTargetInfoWindow = pWindowTarget->GetElement<UI::Window>( WINDOWID_TargetInfo );
	auto pCharacterInfoWindow = pWindowTarget->GetElement<UI::Window>( WINDOWID_CharacterInfo );
	if( (pTargetInfoWindow && pCharacterInfoWindow) && !pCharacterInfoWindow->IsOpen() && pTargetInfoWindow->IsOpen() && pTargetInfoWindow->IsHover() )
		pCharacterInfoWindow->Show();
	else if( (pTargetInfoWindow && pCharacterInfoWindow) && pCharacterInfoWindow->IsOpen() && pTargetInfoWindow->IsOpen() && !pTargetInfoWindow->IsHover()  )
		pCharacterInfoWindow->Hide();
	else if( ( pTargetInfoWindow && pCharacterInfoWindow ) && pCharacterInfoWindow->IsOpen() && !pTargetInfoWindow->IsOpen() )
		pCharacterInfoWindow->Hide();

	//Check if can close roll dice ranking
	if( pWindowRollDiceRanking->IsOpen() && TICKCOUNT - dwLastUpdateRollDice > (MAX_ROLLDICEDROP_TIME + 3) * 1000 )
		pWindowRollDiceRanking->Hide();

	//Update Objects
	pcInventoryWindowHandler->Update( fTime );
	pcPartyHandler->Update( fTime );
	pcInventory->Update( fTime );
	pWindowTarget->Update( fTime );
	pcMiniMapHandler->Update();

	if( pcTooltipWorldTime->IsHover() )
		pcTooltipWorldTime->SetText( FormatString( "World Time: %02d:%02d", ( *(DWORD*)0x00826D10 ), ( *(DWORD*)0x00CF4798 ) ) );

	if( pcTooltipBars[0]->IsHover() )
		pcTooltipBars[0]->SetText( FormatString( "STM: %d/%d", UNITDATA->sCharacterData.sSP.sCur, UNITDATA->sCharacterData.sSP.sMax) );

	if( pcTooltipBars[1]->IsHover() )
		pcTooltipBars[1]->SetText( FormatString( "HP: %d/%d", CHARACTERGAME->GetCurrentHP(), UNITDATA->sCharacterData.sHP.sMax) );

	if( pcTooltipBars[2]->IsHover() )
		pcTooltipBars[2]->SetText( FormatString( "MP: %d/%d", UNITDATA->sCharacterData.sMP.sCur, UNITDATA->sCharacterData.sMP.sMax ) );

	/*if( pWindowDisplay )
	{
		auto pImageLeftSkill = pWindowDisplay->GetElement<UI::ImageBox>( 1 );
		if( pImageLeftSkill )
			pImageLeftSkill->SetImage( pcaImageSkillsIcon[0][USESKILLS.pLeftSkill->Position - 2] );

		for( size_t i = 0; i < 8; i++ )
		{
			auto pImageSkill = pWindowDisplay->GetElement<UI::ImageBox>( 2 + i );
			if( pImageSkill )
				pImageSkill->SetImage( pcaImageSkillsIcon[0][USESKILLS.saUseSkill[GetShortcutSkill(i+1)].Position - 2] );
		}
	}*/
}