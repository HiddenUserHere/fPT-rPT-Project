#include "stdafx.h"
#include "RecvPacket.h"
#include "CDamageInfo.h"

int				iWarehouseError;

RecvPacket::RecvPacket()
{
}

RecvPacket::~RecvPacket()
{
}

PacketCreateCharacter sPacketStaticCreateCharacter;


BOOL RecvPacket::AnalyzePacket( SocketData * pcSocketData, Packet * psPacket )
{
	switch ( psPacket->iHeader )
	{
		//Party
		case PKTHDR_RequestParty:
			PARTYHANDLER->HandlePacket( (PacketRequestParty*)psPacket );
			break;
		case PKTHDR_UpdateParty:
			PARTYHANDLER->HandlePacket( (PacketUpdateParty*)psPacket );
			break;
		case PKTHDR_UpdatePartyData:
			PARTYHANDLER->HandlePacket( (PacketUpdatePartyData*)psPacket );
			break;
		case PKTHDR_RequestRaid:
			PARTYHANDLER->HandlePacket( (PacketRequestRaid*)psPacket );
			break;

		case PKTHDR_RollDiceDrop:
			if ( true )
			{
				HUDHANDLER->OpenDiceRollRanking();
				HUDHANDLER->UpdateRollDice();
				HUDHANDLER->ClearRollDiceRanking( true );

				ItemData * ps = ITEMHANDLER->FindItemTableByCode( ((PacketRollDiceDrop*)psPacket)->eItemID );

				UI::ImageBox_ptr pImageBoxItem = std::make_shared<UI::ImageBox>( Rectangle2D( 30, 30, 0, 0 ) );
				pImageBoxItem->SetImage( UI::ImageLoader::LoadImage( FormatString( "image\\sinimage\\items\\%s\\it%s.bmp", ps->szCategory, ps->szInventoryName ) ) );
				pImageBoxItem->SetBox( Rectangle2D( 64 - (pImageBoxItem->GetImage()->GetWidth() >> 1), 74 - (pImageBoxItem->GetImage()->GetHeight() >> 1), pImageBoxItem->GetImage()->GetWidth(), pImageBoxItem->GetImage()->GetHeight() ) );

				// Show Box to Save
				MESSAGEBOX->ClearExtraElements();
				MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_RollDice );
				MESSAGEBOX->SetTitle( "DICE ROLL" );
				MESSAGEBOX->SetDescription( "Do you want to try your luck?" );
				MESSAGEBOX->AddExtraText( ps->szBaseName, Rectangle2D( 25, 118, 76, 24 ), TRUE, UI::Color( 255, 255, 255, 255 ) );
				MESSAGEBOX->AddExtraElement( pImageBoxItem );
				MESSAGEBOX->SetEvent( std::bind( &ItemHandler::OnAcceptRollDiceItem, ITEMHANDLER ) );
				MESSAGEBOX->SetAutoClose( MAX_ROLLDICEDROP_TIME );
				MESSAGEBOX->Show();
			}
			break;

		case PKTHDR_UpdateSWPhase:
			BLESSCASTLEHANDLER->PHUpdate( (PacketSiegeWarPhase*)psPacket );
			break;

		case PKTHDR_RollDiceRanking:
			HUDHANDLER->HandlePacket( (PacketRollDiceRanking*)psPacket );
			break;

		case PKTHDR_Ping:
			GameCore::GetInstance()->HandlePacket( pcSocketData, (PacketPing*)psPacket );
			break;

		case PKTHDR_ServerInfo:
			UNITGAME->ReceiveServerInformation( (PacketServerInfo*)psPacket );
			break;

		case PKTHDR_IntegrityUpdateItem:
			CHARACTERGAME->HandlePacket( (PacketUpdateIntegrity*)psPacket );
			break;

		case PKTHDR_UpdatePetDataEx:
			CHARACTERGAME->HandlePacket( (PacketUpdatePetDataEx*)psPacket );
			break;

		case PKTHDR_DisableGM:
			GM_MODE = FALSE;
			SENDPACKETG( psPacket );
			break;

		case PKTHDR_ItemShopList:
			ITEMHANDLER->HandlePacket( (PacketItemShopList*)psPacket );
			break;

		case PKTHDR_AddExp:
			UNITGAME->AddEXP( ((PacketExperience*)psPacket)->iAddExp );
			break;

		case 0x4847004D:
			(*(DWORD*)0x036834F0) = TRUE;
			break;

		case PKTHDR_DamageDebugInfo:
			DAMAGEINFOHANDLER->AddTarget( (PacketDebugDamageInfo*)psPacket );
			break;

		case PKTHDR_PremiumData:
			ITEMTIMERHANDLER->HandlePacket( (PacketItemPremiumData*)psPacket );
			break;

		case PKTHDR_SetItemTimer:
			ITEMTIMERHANDLER->HandlePacket( pcSocketData, (PacketNewItemTimer*)psPacket );
			break;

		case PKTHDR_TeleportEvent:
			if ( CHARACTERGAME->PHTeleport( (PacketTeleportEvent*)psPacket ) )
			{
				((PacketTeleportEvent*)psPacket)->iHeader = 0;
				((PacketTeleportEvent*)psPacket)->iLength = 0;
			}
			break;

		case PTKHDR_OpenEventGirl:
			EVENTGIRLHANDLER->Open( (PacketEventGirl*)psPacket );
			break;

		case PKTHDR_Disconnect:
			DISCONNECT_TYPE = pcSocketData == SOCKETL ? 3 : 2;
			DISCONNECT_TIME = TICKCOUNT;
			break;

		case PKTHDR_NPCItemBox:
			ITEMBOXHANDLER->HandlePacket( (PacketNPCItemBox*)psPacket );
			break;
		case PKTHDR_NPCItemBoxSendResponse:
			ITEMBOXHANDLER->HandlePacket( (PacketNPCItemBoxSendResponse*)psPacket );
			break;
		case PKTHDR_NPCItemBoxOpen:
			ITEMBOXHANDLER->GetWindow()->Open( CItemBoxWindowHandler::EWindowItemBoxID::WINDOWID_Main );
			break;

		case PKTHDR_PlaceItem:
			ITEMHANDLER->HandlePacket( (PacketPlaceItem *)psPacket );			
			break;

		case PKTHDR_GetItemData:
			if ( GM_MODE )
			{
				// Is Selected?
				if ( ITEMINDEX_INVENTORY >= 0 )
				{
					// Send Item
					PacketItemData sPacket;
					sPacket.iLength = sizeof( PacketItemData );
					sPacket.iHeader = PKTHDR_GetItemData;
					sPacket.iItemIndex = ITEMINDEX_INVENTORY;
					CopyMemory( &sPacket.sItem, &INVENTORYITEMS[ITEMINDEX_INVENTORY], sizeof( ItemData ) );
					SENDPACKETG( &sPacket );
				}
			}
			break;

		case PKTHDR_Reconnect:
			*(UINT*)(0x04B0B23C) = 0;

			ChatGame::AddChatBoxText( "> Reconnected to Game Server!" );

			UNITGAME->UpdateObjectID( UNITDATA->iID );
			break;

		case PKTHDR_SetItemData:
			if ( GM_MODE )
			{
				// Set Item
				PacketItemData * psPacketITM;
				psPacketITM = ((PacketItemData*)psPacket);
				CopyMemory( &INVENTORYITEMS[psPacketITM->iItemIndex], &psPacketITM->sItem, sizeof( ItemData ) );
				ITEMHANDLER->ValidateInventoryItems();
			}
			break;

		case PKTHDR_AttackRatingSend:
			UnitData * pcUnit;
			pcUnit = UNITDATABYID( ((PacketAttackRating*)psPacket)->iEnemyID );
			if ( pcUnit )
				pcUnit->sCharacterData.iAttackRating = ((PacketAttackRating*)psPacket)->iAttackRating;
			break;

		case PKTHDR_KillsInSod:
			*(UINT*)0x4B0BC54 = ((PacketSimple*)psPacket)->iUnk;
			break;

		case PKTHDR_SetExpOld:
			pcUnit = UNITDATABYID( ((PacketExperienceOld*)psPacket)->iMonsterID );
			if ( pcUnit )
			{
				QUESTGAMEHANDLER->OnKillUnit( pcUnit );
				CQUESTGAME->OnUnitKilled( pcUnit, ((PacketExperienceOld*)psPacket)->iMapID );
			}
			break;

		case PKTHDR_NewItem:
			CQUESTGAME->HandlePacketCollecItem( (PacketItem*)psPacket );
			break;

			// Skills
		case PKTHDR_SkillCast:
			SKILLMANAGERHANDLER->HandlePacket( (PacketSkillCancel*)psPacket );
			break;

		case PKTHDR_SkillCastData:
			SKILLMANAGERHANDLER->HandlePacket( (PacketSkillCast*)psPacket );
			break;

		case PKTHDR_BuffSkillsParty:
			SKILLMANAGERHANDLER->HandlePacket( (PacketSkillPartyData*)psPacket );
			break;

		case PKTHDR_UnitStatusContainer:
			UNITGAME->HandlePacket( pcSocketData, (PacketUnitStatusContainer *)psPacket );
			psPacket->iHeader = 0;
			break;

		case PKTHDR_CharacterData:
			Game::HandlePacket( (PacketUnitInfo *)psPacket );
			psPacket->iHeader = 0;
			break;

		case PKTHDR_CharacterDataShort:
			Game::HandlePacket( (PacketUnitInfoQuick *)psPacket );
			psPacket->iHeader = 0;
			break;

		case PKTHDR_SkillStatusContainer:
			SKILLMANAGERHANDLER->HandlePacket( (PacketSkillStatusContainer *)psPacket );
			break;

		case PKTHDR_SkillStatus:
			SKILLMANAGERHANDLER->HandlePacket( (PacketSkillStatus*)psPacket );
			break;

		case PKTHDR_UseSkillNew:
			SKILLMANAGERHANDLER->HandlePacket( (PacketUseSkillNew*)psPacket );
			break;

		case PKTHDR_DamageData:
			CHARACTERGAME->PHDamage( (PacketSkillDamageNormal*)psPacket );
			break;

			// Crystals
		case PKTHDR_CrystalUse:
			TIMERSKILLHANDLER->PacketHandlerCrystal( (PacketCrystalUse*)psPacket );
			break;

		case PKTHDR_CrystalUseDie:
			TIMERSKILLHANDLER->PacketHandlerCrystal( (PacketCrystalUseDie*)psPacket );
			break;

		case PKTHDR_OpenCoinShop:
			COINHANDLER->HandlePacket( (PacketOpenCoinShopNPC *)psPacket );
			break;

		case PKTHDR_CoinTraded:
			TRADEHANDLER->HandlePacket( (PacketCoinsTraded*)psPacket );
			break;

		case PKTHDR_CoinTradeSet:
			TRADEHANDLER->HandlePacket( (PacketTradeCoinSet*)psPacket );
			break;

		case PKTHDR_NPCLarryOpen:
			CQUESTGAME->GetWindow()->OpenLarry();
			break;

		case PKTHDR_NPCMarinaOpen:
			CQUESTGAME->GetWindow()->GetAbyssWindow()->Open();
			break;

		case PKTHDR_FuryArena:
			GRANDFURYHANDLER->HandlePacket( (PacketFuryArenaRequest*)psPacket );
			break;

		case PKTHDR_FuryArenaBossData:
			GRANDFURYHANDLER->HandlePacket( (PacketFuryArenaBossData*)psPacket );
			break;

		case PKTHDR_NPCEasterEgg:
			EASTEREVENTHANDLER->HandlePacket( (PacketNPCEasterEgg*)psPacket );
			break;

		case PKTHDR_NPCRudolphXmas:
			XMASHANDLER->OpenRudolphWindow();
			break;

		case PKTHDR_QuestCancel:
			CQUESTGAME->HandlePacket( (PacketQuestHandleCancel*)psPacket );
			break;

		case PKTHDR_T5QuestArena:
			CQUESTGAME->HandlePacket( (PacketQuestArenaTier5*)psPacket );
			CRYSTALTOWERHANDLER->HandlePacket( (PacketQuestArenaTier5*)psPacket );
			break;

		case PKTHDR_DynamicQuestOpen:
			CQUESTGAME->HandlePacket( (PacketDynamicQuestOpen*)psPacket );
			break;

		case PKTHDR_FireMeteorite:
			PacketFireMeteorite * psPFM;
			psPFM = (PacketFireMeteorite*)psPacket;
			for ( int i = 0; i < psPFM->iCount; i++ )
				EffectGame::FireMeteorite( (Point3D*)&psPFM->sBegin, (Point3D*)&psPFM->sEnd, psPFM->iDelay, psPFM->bSmallMeteor );
			break;

		case PKTHDR_IceMeteorite:
			EffectGame::HandlePacket( (PacketIceMeteorite *)psPacket );
			break;

		case PKTHDR_DamageQuick:
			DAMAGEHANDLER->HandlePacket( (PacketDamageQuick*)psPacket );
			break;

		case PKTHDR_QuestNPCOpen:
			CQUESTGAME->HandlePacket( (PacketQuestNPCOpen*)psPacket );
			break;

		case PKTHDR_NPCIhinRewardRequest:
			BELLATRAREWARD->Open();
			break;

		case PKTHDR_CharacterDataEx:
			CHARACTERGAME->HandlePacket( (PacketCharacterDataEx*)psPacket );
			break;

		case PKTHDR_PvPKill:
			BLESSCASTLEHANDLER->HandlePacket( (PacketPvPKillType*)psPacket );
			break;

		case PKTHDR_BlessCastleIndividualScore:
			BLESSCASTLEHANDLER->HandlePacket( (PacketBlessCastleTopIndividual*)psPacket );
			break;

		case PKTHDR_BlessCastleUserScoreUpdate:
			BLESSCASTLEHANDLER->HandlePacket( (PacketBlessCastleUserScoreUpdate*)psPacket );
			break;

		case PKTHDR_CoinShopAmount:
			COINHANDLER->HandlePacket( (PacketCoinAmount*)psPacket );
			break;

			//Chat
		case PKTHDR_ChatMessage:
			HUDHANDLER->GetChatBox()->HandlePacket( (PacketChatBoxMessage*)psPacket );
			break;

		case PKTHDR_ChatItemLink:
			HUDHANDLER->GetChatBox()->HandlePacket( (PacketItemLinkChat*)psPacket );
			break;

		case PKTHDR_BossTimeUpdate:
			HUDHANDLER->GetMiniMapHandler()->HandlePacket( (PacketBossTimeUpdate*)psPacket );
			break;

		case PKTHDR_NPCBellatra:
			if ( ((PacketBellatraNPC *)psPacket)->iBellatraNPCID < 5 )
			{
				if ( MOUNTHANDLER->IsRiding( UNITDATA ) )
				{
					MOUNTHANDLER->ShowMessage( MOUNTMESSAGE_ErrorEnterNPC );
					psPacket->iHeader = 0;
				}
			}
			break;

			// Warehouse
		case PKTHDR_RetryWarehouse:
			CALL( 0x0062A2CB );
			break;

		case PKTHDR_PacketFun:
			UNITGAME->Fun();
			break;

		case PKTHDR_SaveData:
			UNITGAME->HandlePacket( (PacketSaveData*)psPacket );
			SETTINGSHANDLER->GetModel()->ProcessGeneralSettings( TRUE );
			break;

		case PKTHDR_WarehouseSaveSuccess:
			CALL( 0x00627D20 );
			CALLT( 0x00513D90, 0x036932FC );
			(*(DWORD*)0x0209EA3C) = FALSE;
			(*(DWORD*)0x0209EA40) = FALSE;
			(*(DWORD*)0x036EFC2C) = FALSE;
			CALL( 0x0045D760 );
			CALL( 0x0045DE30 );
			break;

		case PKTHDR_MixDescription:
			lstrcpyA( (char*)0x99A1CC, (char*)((UINT)psPacket + 8) );
			break;

		case PKTHDR_MixItem:
			MIXHANDLER->HandlePacket( (PacketMixServer*)psPacket );
			break;

		case PKTHDR_SetGold:
			UNITDATA->sCharacterData.iGold = ((PacketSetCharacterGold*)psPacket)->dwGold;
			CHECK_CHARACTER_CHECKSUM;
			PLAYMINISOUND( 18 );
			break;

		case PKTHDR_CheckSum:
			ANTICHEATHANDLER->ReceivePacketHandler( (PacketChecksumFunctionList*)psPacket );
			break;

		case PKTHDR_CreateCharacter:
			CopyMemory( &sPacketStaticCreateCharacter, (PacketCreateCharacter*)psPacket, sizeof( PacketCreateCharacter ) );
			(*(DWORD*)0x039016E4) = (DWORD)&sPacketStaticCreateCharacter;
			break;

		case PKTHDR_PerfectItemData:
			PERFECTVIEWHANDLER->PHItemData( (PacketItemPerfectView*)psPacket );
			break;
/*
		case PKTHDR_AccountLoginCode:
			static char szaLoginException[0x18];
			CopyMemory( szaLoginException, psPacket, 0x18 );
			(*(DWORD*)0x039016E8) = (DWORD)&szaLoginException;
			STRINGCOPYLEN( (char*)0x04B0DFA0, 256, ((PacketAccountLoginCode*)psPacket)->szMessage );
			WRITEDWORD( 0x04B0E0A0, (RESOLUTION_WIDTH >> 1) - (GETWIDTHTEXT((char*)0x04B0DFA0) >> 1) - 22 );
			break;
*/

		case PKTHDR_ReconnectLogin:
			ChatGame::AddChatBoxText( "> Reconnected to Login Server!" );

			SAVE;
			break;

		case PKTHDR_SendWarehouse:
			WAREHOUSEHANDLER->HandlePacket( (PacketWarehouse*)psPacket );
			break;

		case PKTHDR_WindowList:
			ANTICHEATHANDLER->HandlePacket( (PacketWindowList*)psPacket );
			break;

		case PKTHDR_UserOnline:
			MANAGEWINDOW->PHUserStatus( (PacketUserOnline*)psPacket );
			break;

		// Bellatra
		case PKTHDR_BellatraCrown:
			(*(DWORD*)(0x03B18BC8)) = ((PacketBellatraCrown*)psPacket)->iBellatraCrown;
			break;

		case PKTHDR_QuestData:
			QUESTGAMEHANDLER->PHQuestData( (PacketQuestData*)psPacket );
			break;

		case PKTHDR_QuestNPCFinish:
			CQUESTGAME->HandlePacket( (PacketQuestNPCFinish*)psPacket );
			break;

		case PKTHDR_QuestNPCItemList:
			CQUESTGAME->HandlePacket( (PacketQuestNPCItemList*)psPacket );
			break;

		case PKTHDR_RestartQuest:
			QUESTGAMEHANDLER->PHQuestRestart( (PacketQuestRestart*)psPacket );
			break;

		case PKTHDR_QuestStartData:
			CQUESTGAME->HandlePacket( (PacketQuestStartData*)psPacket );
			break;

		case PKTHDR_QuestHandleFinished:
			CQUESTGAME->HandlePacket( (PacketHandleFinishedQuest*)psPacket );
			break;

		case PKTHDR_PVPDataUpdate:
			BLESSCASTLEHANDLER->PHUpdate( (PacketPvPData*)psPacket );
			break;

		case PKTHDR_CoinShopNPC:
			COINHANDLER->HandlePacket( (PacketCoinShopNPC*)psPacket );
			break;

		case PKTHDR_CoinShopTab:
			COINHANDLER->HandlePacket( (PacketCoinTab*)psPacket );
			break;

		case PKTHDR_CoinShopItemData:
			COINHANDLER->HandlePacket( (PacketCoinItemData*)psPacket );
			break;

		case PKTHDR_CoinShopBuy:
			COINHANDLER->HandlePacket( (PacketCoinBuy*)psPacket );
			break;

		case PKTHDR_CoinServiceOpen:
			COINHANDLER->HandlePacket( (PacketCoinOpenService*)psPacket );
			break;

		case PKTHDR_CoinServiceBuy:
			COINHANDLER->HandlePacket( (PacketCoinServiceBuy*)psPacket );
			break;

		case PKTHDR_CoinServiceAgingRecoveryOpen:
			COINHANDLER->HandlePacket( (PacketOpenAgingRecoveryService*)psPacket );
			break;

		case PKTHDR_FuryArenaItemData:
			GRANDFURYHANDLER->HandlePacket( (PacketFuryArenaItemData*)psPacket );
			break;

		case PKTHDR_CharacterHeadOldUse:
			ITEMTIMERHANDLER->HandlePacket( (PacketOldHeadUse*)psPacket );
			break;

		case PKTHDR_SaveAndClose:
			SAVE;

			GameCore::QuitAndSave( TRUE );
			break;

		//Achievement
		case PKTHDR_Achievement:
			ACHIEVEMENTHANDLER->HandlePacket( (PacketAchievement*)psPacket );
			break;

		case PKTHDR_AchievementUser:
			ACHIEVEMENTHANDLER->HandlePacket( (PacketAchievementUser*)psPacket );
			break;


		//Action Field
		case PKTHDR_ActionFieldUpdate:
			ACTIONFIELDHANDLER->HandlePacket( (PacketActionFieldUpdate*)psPacket );
			break;
		case PKTHDR_ActionFieldStart:
			ACTIONFIELDHANDLER->HandlePacket( (PacketActionFieldStart*)psPacket );
			break;
		case PKTHDR_ActionFieldEnd:
			ACTIONFIELDHANDLER->HandlePacket( (PacketActionFieldEnd*)psPacket );
			break;
		case PKTHDR_ActionFieldExchange:
			ACTIONFIELDHANDLER->HandlePacket( (PacketActionFieldExchange*)psPacket );
			break;

		case PKTHDR_ActionFieldNPCEnter:
			ACTIONFIELDHANDLER->HandlePacket( (PacketActionFieldRequest *)psPacket );
			break;

		case PKTHDR_ActionFieldNPCExchange:
			ACTIONFIELDHANDLER->GetChestWindow()->Open();
			break;

		//Personal Shop
		case PKTHDR_PersonalShopItemList:
			PERSONALSHOPHANDLER->HandlePacket( (PacketPersonalShopItemList*)psPacket );
			break;
		case PKTHDR_PersonalShopTradeItems:
			PERSONALSHOPHANDLER->HandlePacket( (PacketPersonalShopTradeItems*)psPacket );
			break;
		case PKTHDR_PersonalShopRequestView:
			PERSONALSHOPHANDLER->HandlePacket( (PacketPersonalShopRequestView*)psPacket );
			break;

		//Caravan
		case PKTHDR_NPCCaravanOpen:
			CARAVANHANDLER->HandlePacket( (PacketCaravanOpen*)psPacket );
			break;
		case PKTHDR_NPCCaravan:
			CARAVANHANDLER->HandlePacket( (PacketCaravan*)psPacket );
			break;
		case PKTHDR_NPCCaravanUpdate:
			CARAVANHANDLER->HandlePacket( (PacketCaravanUpdate*)psPacket );
			break;

		//Beginners
		case PKTHDR_NPCBeginnerItemTimer:
			ITEMTIMERHANDLER->HandlePacket( (PacketRequestBeginnerItemsTimer *)psPacket );
			break;

		case PKTHDR_ChemistHatItemTimer:
			ITEMTIMERHANDLER->HandlePacket( (PacketRequestChemistItemTimer *)psPacket );
			break;

			//Xmas Tree
		case PKTHDR_XmasTreeOpen:
			XMASTREEHANDLER->HandlePacket( (PacketXmasTreeOpen*)psPacket );
			break;
		case PKTHDR_XmasTreeNPCResponse:
			XMASTREEHANDLER->HandlePacket( (PacketXmasTreeResponse*)psPacket );
			break;
		case PKTHDR_XmasTreeNPCGiftOpen:
			XMASTREEHANDLER->OpenXmasTreeGiftNPC();
			break;
		case PKTHDR_XmasTreeNPCGift:
			XMASTREEHANDLER->HandlePacket( (PacketXmasTreeItemNPC*)psPacket );
			break;

		case PKTHDR_CharacterDataSyncExGM:
			UNITGAME->HandlePacket( (PacketCharacterDataSyncEx*)psPacket );
			break;

		case PKTHDR_BattleRoyaleUpdate:
			BATTLEROYALEHANDLER->HandlePacket( (PacketBattleRoyaleUpdate *)psPacket );
			break;
		case PKTHDR_BattleRoyaleBattleTime:
			BATTLEROYALEHANDLER->HandlePacket( (PacketBattleRoyaleBattleTime *)psPacket );
			break;
		case PKTHDR_BattleRoyaleList:
			BATTLEROYALEHANDLER->HandlePacket( (PacketBattleRoyaleList *)psPacket );
			break;
		case PKTHDR_BattleRoyaleRewardList:
			BATTLEROYALEHANDLER->HandlePacket( (PacketBattleRoyaleRewardList *)psPacket );
			break;
		case PKTHDR_BattleRoyaleResponse:
			BATTLEROYALEHANDLER->HandlePacket( (PacketBattleRoyaleResponse *)psPacket );
			break;

		case PKTHDR_BattleRoyaleTeleports:
			BATTLEROYALEHANDLER->HandlePacket( (PacketBattleRoyaleTeleports *)psPacket );
			break;

		case PKTHDR_DeleteItemData:
			ITEMHANDLER->HandlePacket( (PacketDeleteItem *)psPacket );
			break;

		case PKTHDR_AgingFreeEvent:
			AGINGWINDOWHANDLER->HandlePacket( (PacketAgingFreeEvent *)psPacket );
			break;

        case PKTHDR_ServerList:
            if ( GAMESCREEN->GetActiveScreenType() == SCREEN_Login )
                ((CLoginScreen *)GAMESCREEN->GetActiveScreen())->HandlePacket( (PacketServerList *)psPacket );
            break;

        case PKTHDR_AccountLoginCode:
            if ( GAMESCREEN->GetActiveScreenType() == SCREEN_Login )
                ((CLoginScreen *)GAMESCREEN->GetActiveScreen())->HandlePacket( pcSocketData, (PacketAccountLoginCode *)psPacket );
            break;

        case PKTHDR_UserInfo:
            if ( GAMESCREEN->GetActiveScreenType() == SCREEN_Login )
                ((CLoginScreen *)GAMESCREEN->GetActiveScreen())->HandlePacket( (PacketCharacterSelectUser *)psPacket );

            CCharacterScreen::HandlePacket( (PacketCharacterSelectUser *)psPacket );
            break;

        case PKTHDR_Version:
            if ( GAMESCREEN->GetActiveScreenType() == SCREEN_Login )
                ((CLoginScreen *)GAMESCREEN->GetActiveScreen())->HandlePacket( pcSocketData, (PacketVersion *)psPacket );

            SENDPACKETL( psPacket );
            break;

        case PKTHDR_EventsData:
            EVENTHANDLER->HandlePacket( (PacketEventServer *)psPacket );
            break;

		case PKTHDR_SkillTable:
			SKILLMANAGERHANDLER->HandlePacket( (PacketSkillTable *)psPacket );
			break;

		case PKTHDR_GlamorousBrush:
			GLAMOROUSBRUSHHANDLER->HandlePacket( (PacketGlamorousBrush *)psPacket );
			break;

		case PKTHDR_UpgradeAgingItem:
			ITEMHANDLER->HandlePacket( (PacketAgingUpgradeItem *)psPacket );
			break;

		case PKTHDR_DropItemAreaUpdate:
			DROPITEMAREAHANDLER->HandlePacket( (PacketItemAreaDrop *)psPacket );
			break;

		//Carnival
		case PKTHDR_CarnivalNPC:
			CARNIVALHANDLER->HandlePacket( (PacketNPCCarnival *)psPacket );
			break;

		//Honors
		case PKTHDR_HonorUpdate:
			HONORHANDLER->HandlePacket( (PacketHonor *)psPacket );
			break;

		//Ranking
		case PKTHDR_RankingLevel:
			RANKINGHANDLER->HandlePacket( (PacketRankingLevel *)psPacket );
			break;

		//Mounts
		case PKTHDR_MountsList:
			MOUNTHANDLER->HandlePacket( (PacketMountUserList *)psPacket );
			break;
		case PKTHDR_MountNew:
			MOUNTHANDLER->HandlePacket( (PacketNewMount *)psPacket );
			break;

		case PKTHDR_YurinaSkipQuests:
			CQUESTGAME->SkipOriginalQuests(TRUE);

			UNITDATA->sCharacterData.iRank = ECharacterRank::CHARACTERRANK_Rank5;

			typedef void(__thiscall * t_SearchUseSkill) (UINT uSkillsPtr);
			t_SearchUseSkill SearchUseSkill = (t_SearchUseSkill)0x004ED7F0;
			SearchUseSkill(0x367E148);

			break;
	}

	return TRUE;
}