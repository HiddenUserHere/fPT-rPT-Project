#include "stdafx.h"
#include "packetserver.h"
#include "HNSSkill.h"

CRITICAL_SECTION CriticalSectionPacketSend;
BOOL bPacketCritical = FALSE;

extern BOOL __cdecl BuffReceive( User * pcUser, PacketSkillPartyData * pPacket );

PacketServer::PacketServer()
{
}

PacketServer::~PacketServer()
{
}



BOOL PacketServer::AnalyzePacket( User * pcUser, PacketReceiving * p )
{
	Packet * psPacket = (Packet*)p->baPacket;

	UserData * pcUserData = pcUser->pcUserData;

	NETSERVER->OnReceivePacket( pcUserData, psPacket );

	switch ( psPacket->iHeader )
	{
		case PKTHDR_Ping:
			if ( ((PacketPing*)psPacket)->iLength != sizeof( PacketPing ) )
				break;

			if ( pcUserData->pcSocketData )
				pcUserData->pcSocketData->SetPing( ((PacketPing *)psPacket)->iPing );

			((PacketPing*)psPacket)->dwTick = GetTickCount();
			SENDPACKET( pcUser, psPacket, TRUE );

			if ( pcUserData->dwNoticeCounter < READDWORD( 0x07AC9BF0 ) )
				CALL_WITH_ARG1( 0x00550930, (DWORD)pcUserData );
			break;

		case PKTHDR_Crash:
			CHEATSERVER->HandlePacket( pcUser, (PacketCrash*)psPacket );
			break;

		case PKTHDR_CrashData:
			CHEATSERVER->HandlePacket( pcUser, (PacketCrashData*)psPacket );
			break;

		case PKTHDR_CharacterDataEx:
			CHARACTERSERVER->HandlePacket( pcUser, (PacketCharacterDataEx*)psPacket );
			break;

		case PKTHDR_CreateCharacter:
			CHARACTERSERVER->CharacterCreate( pcUserData, (PacketCreateCharacter*)psPacket );
			break;

		case PKTHDR_LoginUser:
			if ( pcUserData->iTicket == 0 )
			{
				if ( pcUserData->dwVerifiedLoginTimeOut < TICKCOUNT )
				{
					PacketLoginUser * p;
					p = (PacketLoginUser *)psPacket;

					ACCOUNTSERVER->PHAccountLogin( pcUser, p );

					WRITEDWORD( 0x07AC9D80, READDWORD( 0x07AC9D80 ) + 1 );

					CHEATSERVER->OnLoginUserPacket( pcUserData );

					pcUserData->dwVerifiedLoginTimeOut = TICKCOUNT + 750;
				}
				else
					WNDPROC_SOCKETCLOSE( SOCKETSERVER->GetHWND(), pcUserData->pcSocketData );
			}
			else
				WNDPROC_SOCKETCLOSE( SOCKETSERVER->GetHWND(), pcUserData->pcSocketData );
			break;

		case PKTHDR_SaveData:
			ACCOUNTSERVER->SQLCharacterSave( pcUserData );
			CHARACTERSERVER->HandlePacket( pcUser, (PacketSaveData *)psPacket );
			break;

		case PKTHDR_SelectCharacter:
			CHARACTERSERVER->HandlePacket( pcUser, (PacketSelectCharacter *)psPacket );
			break;

		case PKTHDR_Version:
			break;

		case PKTHDR_LogCheat:
			LOGSERVER->OnLogCheat( pcUserData, (PacketLogCheat*)psPacket );
			break;

		case PKTHDR_Reconnect:
			RECONNECTSERVERHANDLER->HandlePacket( pcUser, (PacketReconnectUser*)psPacket );
			break;

		case PKTHDR_ChatGame:
			if ( SERVERCOMMAND->OnReceiveChat( pcUserData, (PacketChat*)psPacket ) )
				return TRUE;
			break;

		case PKTHDR_ChatItemLink:
			CHATSERVER->HandlePacket( pcUser, (PacketItemLinkChat*)psPacket );
			break;

		case PKTHDR_PlayDataEx:
			CHARACTERSERVER->OnCharacterSyncDataEx( pcUserData, (PacketPlayDataEx*)psPacket );
			break;

		case PKTHDR_LoseExp:
			CHARACTERSERVER->OnLoseExp( pcUserData, (PacketLoseExperience*)psPacket );
			break;

		case PKTHDR_UserOnline:
			SERVERCOMMAND->CheckUserOnline( pcUserData, (PacketUserOnline*)psPacket );
			break;

		case PKTHDR_DeleteCharacter:
			ACCOUNTSERVER->DeleteCharacter( pcUserData, (PacketDeleteCharacter*)psPacket );
			break;


		case PKTHDR_TradeItems:
			TRADEHANDLER->HandlePacket( pcUser, (PacketTradeSuccess*)psPacket );
			break;

			//Item
		case PKTHDR_BuyItemNPCShop:
			ITEMSERVER->HandlePacket( pcUser, (PacketBuyNPCShop*)psPacket );
			break;

		case PKTHDR_PerfectItemData:
			ITEMSERVER->PHGetPerfectItem( pcUser, (PacketGetItemPerfectView*)psPacket );
			break;

		case PKTHDR_MixItem:
			ITEMSERVER->OnMixingItem( pcUser, (PacketMixServer*)psPacket );
			return TRUE;

		case PKTHDR_SocketItemDataToServer:
			ITEMSERVER->OnSocketItem( pcUser, (PacketSocketSystem*)psPacket );
			break;

		case PKTHDR_ResetItem:
			ITEMSERVER->OnResetItem( pcUser, (PacketResetItem*)psPacket );
			break;

		case PKTHDR_AgingItem:
			ITEMSERVER->OnAgingItem( pcUserData, (PacketAgingItem*)psPacket );
			break;

		case PKTHDR_ManufactureItem:
			MANUFACTUREHANDLER->HandlePacket( pcUser, (PacketManufactureItem*)psPacket );
			break;

		case PKTHDR_QuestItemSwap:
			QUESTSERVER->HandlePacket( pcUser, (PacketQuestItemSwap*)psPacket );
			break;

		case PKTHDR_UpdateLevelClan:
			CHARACTERSERVER->OnCharacterUpdateLevelClan( pcUser, (PacketUpdateClanLevel*)psPacket );
			break;

		case PKTHDR_UpgradeAgingItem:
			ITEMSERVER->OnUpgradeAgingItem( pcUserData, (PacketItem*)psPacket );
			break;

		case PKTHDR_RecvWarehouse:
			WAREHOUSEHANDLER->HandlePacket( pcUser, (PacketWarehouseOpen*)psPacket );
			break;

		case PKTHDR_SendWarehouse:
			WAREHOUSEHANDLER->HandlePacket( pcUser, (PacketWarehouse*)psPacket );
			break;

		case PKTHDR_NPCItemBox:
			ITEMSERVER->OpenItemBox( pcUser, (PacketNPCItemBox*)psPacket );
			break;
		case PKTHDR_NPCItemBoxSend:
			ITEMSERVER->HandlePacket( pcUser, (PacketNPCItemBoxSend*)psPacket );
			break;

		//Caravan
		case PKTHDR_NPCCaravanOpen:
			FOLLOWUNITHANDLER->HandlePacket( pcUser, (PacketCaravanOpen*)psPacket );
			break;
		case PKTHDR_NPCCaravan:
			FOLLOWUNITHANDLER->HandlePacket( pcUser, (PacketCaravan*)psPacket );
			break;
		case PKTHDR_NPCCaravanUpdate:
			FOLLOWUNITHANDLER->HandlePacket( pcUser, (PacketCaravanUpdate*)psPacket );
			break;

		//Beginners
		case PKTHDR_NPCBeginnerItemTimer:
			ITEMSERVER->HandlePacket( pcUser, (PacketRequestBeginnerItemsTimer *)psPacket );
			break;

		case PKTHDR_ChemistHatItemTimer:
			ITEMSERVER->HandlePacket( pcUser, (PacketRequestChemistItemTimer *)psPacket );
			break;

			//Quest Handlers
		case PKTHDR_QuestNPCAccept:
			QUESTSERVER->HandlePacket( pcUser, (PacketQuestNPCAccept*)psPacket );
			break;
		case PKTHDR_QuestCancel:
			QUESTSERVER->HandlePacket( pcUser, (PacketQuestHandleCancel*)psPacket );
			break;

		case PKTHDR_QuestUpdateData:
			QUESTSERVER->HandlePacket( pcUser, (PacketQuestUpdateData*)psPacket );
			break;

		case PKTHDR_QuestNPCFinish:
			QUESTSERVER->HandlePacket( pcUser, (PacketQuestNPCFinish*)psPacket );
			break;

		case PKTHDR_CustomQuestUpdate:
			QUESTSERVER->HandlePacket( pcUser, (PacketCustomQuestValue *)psPacket );
			break;

			//Bellatra
		case PKTHDR_GoldBellatraClan:
			BELLATRASERVER->PHGoldClanRecv( pcUser, (PacketClanGoldReceive*)psPacket );
			break;

			//Coin Shop
		case PKTHDR_CoinShopAmount:
			COINSHOPHANDLER->HandlePacket( pcUser, (PacketCoinAmount*)psPacket );
			break;

		case PKTHDR_CoinShopNPC:
			COINSHOPHANDLER->HandlePacket( pcUser, (PacketCoinShopNPC*)psPacket );
			break;

		case PKTHDR_CoinShopOpenTab:
			COINSHOPHANDLER->HandlePacket( pcUser, (PacketCoinOpenTab*)psPacket );
			break;

		case PKTHDR_CoinShopOpenItemData:
			COINSHOPHANDLER->HandlePacket( pcUser, (PacketCoinOpenItemData*)psPacket );
			break;

		case PKTHDR_CoinShopBuy:
			COINSHOPHANDLER->HandlePacket( pcUser, (PacketCoinBuy*)psPacket );
			break;

		case PKTHDR_CoinServiceBuy:
			COINSHOPHANDLER->HandlePacket( pcUser, (PacketCoinServiceBuy*)psPacket );
			break;

		case PKTHDR_KillCharacter:
			USERSERVER->OnKilled( pcUser, (PacketDeadCharacter*)psPacket );
			break;

		case PKTHDR_SettingsData:
			USERSERVER->SettingsDataSet( pcUser, (PacketSettingsData*)psPacket );
			break;

		case PKTHDR_FuryArena:
			FURYARENAHANDLER->PacketHandlerData( pcUser, (PacketFuryArenaRequest*)psPacket );
			break;

			//Event
		case PKTHDR_NPCEasterEgg:
			EVENTSERVER->HandlePacket( pcUserData, (PacketNPCEasterEgg*)psPacket );
			break;

		case PKTHDR_NPCRudolphXmas:
			EVENTSERVER->HandlePacket( pcUserData, (PacketNPCRudolphXmas*)psPacket );
			break;

		case PKTHDR_ItemSlotHandle:
			ITEMSERVER->HandlePacket( pcUser, (PacketSlotItemHandle*)psPacket );
			break;

		case PKTHDR_DamageDebugInfo:
			MAPSERVER->OnSendDebugDamage( pcUser, (PacketDebugDamageInfo*)psPacket );
			break;

		case PKTHDR_SingleDamage:
			DAMAGEHANDLER->HandlePacket( pcUser, (PacketSkillDamageNormalEx*)psPacket );
			break;

		case PKTHDR_RangeDamage:
			DAMAGEHANDLER->HandlePacket( pcUser, (PacketSkillDamageRangeEx*)psPacket );
			break;

		case PKTHDR_DisableGM:
			pcUserData->iGameLevel = GAMELEVEL_None;
			NETSERVER->SendGameMaster( pcUserData );
			CHATSERVER->SendChat( USERDATATOUSER( pcUserData ), CHATCOLOR_Error, "> GM Disabled!" );
			break;

		case PKTHDR_DeleteFlagPoint:
			MAPSERVER->DeleteFlagPoint( pcUserData, (PacketDeleteFlagPoint*)psPacket );
			break;

		case PKTHDR_GetItemData:
			if ( pcUserData->iGameLevel > GAMELEVEL_Three )
			{
				// Set Temp Item
				CopyMemory( &pcUser->sTempItem, ((PacketItemData*)psPacket), sizeof( PacketItemData ) );
				CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Item Temp Getted!" );
			}
			break;

		case PKTHDR_DynamicQuestOpenNPC:
			QUESTSERVER->HandlePacket( pcUser, (PacketDynamicQuestNPCOpen*)psPacket );
			break;

		case PKTHDR_PlayDataChar:
			USERSERVER->HandlePacket( pcUser, (PacketPlayData*)psPacket );
			break;

		case PKTHDR_TradeRequest:
			USERSERVER->OnTradeRequest( pcUserData, (PacketSimple*)psPacket );
			TRADEHANDLER->HandlePacket( pcUser, (PacketTradeRequest*)psPacket );
			break;

		case PKTHDR_TradeData:
			TRADEHANDLER->HandlePacket( pcUser, (PacketTradeData*)psPacket );
			break;

		case PKTHDR_CoinTradeSet:
			COINSHOPHANDLER->HandlePacket( pcUser, (PacketTradeCoinSet*)psPacket );
			break;

		case PKTHDR_BuffSkillsParty:
			BuffReceive( pcUser, (PacketSkillPartyData*)psPacket );
			break;

		case PKTHDR_UpdatePetData:
			USERSERVER->UpdatePetData( pcUser, (PacketUpdatePetData*)psPacket );
			break;

		case PKTHDR_UpdatePetDataEx:
			USERSERVER->UpdatePetDataEx( pcUser, (PacketUpdatePetDataEx*)psPacket );
			break;

		case PKTHDR_HolyMind:
			PRIESTESSSKILL->OnUseHolyMind( pcUser, (PacketHolyMind*)psPacket );
			break;

		case PKTHDR_Wisp:
			ASSASSINSKILL->OnUseWisp( pcUser, (PacketWisp*)psPacket );
			break;

		case PKTHDR_GetUnitInfoData:
			BOTSERVER->OnGetUnitInfo( pcUser, (PacketSimple*)psPacket );
			ACHIEVEMENTHANDLER->HandlePacket( pcUser, (PacketGetUnitData*)psPacket );
			break;

		case PKTHDR_CancelForceOrb:
			USERSERVER->CancelForceOrb( pcUser );
			break;

		case PKTHDR_ThrowItem:
			if ( MAPSERVER->OnThrowItem( pcUserData, (PacketThrowItem*)psPacket ) )
				*(UINT*)((DWORD)psPacket + 4) = 0;
			break;

		case PKTHDR_CheckItemUseSlot:
			ITEMSERVER->PHCheckItemUsing( pcUserData, (PacketListUsingItem*)psPacket );
			break;

		case PKTHDR_CheckItemUseSlotEx:
			ITEMSERVER->PHCheckItemUsingEx( pcUserData, (PacketListUsingItem*)psPacket );
			break;

		case PKTHDR_UseUnionCore:
			MAPSERVER->UseUnionCore( pcUserData, (PacketUseUnionCore*)psPacket );
			break;

		case PKTHDR_CancelDrasticSpirit:
			KNIGHTSKILL->OnDrasticSpirit( pcUserData );
			break;

		case PKTHDR_NPCIhinRewardRequest:
			BELLATRASERVER->HandlePacket( pcUser, (PacketReceiveItemIhin*)psPacket );
			break;

			//Item
		case PKTHDR_SetItemTimer:
			ITEMSERVER->HandlePacket( pcUser, (PacketNewItemTimer*)psPacket );
			break;
		case PKTHDR_CancelItemTimer:
			ITEMSERVER->HandlePacket( pcUser, (PacketCancelItemTimer*)psPacket );
			break;
		case PKTHDR_CharacterHeadOldUse:
			ITEMSERVER->HandlePacket( pcUser, (PacketOldHeadUse*)psPacket );
			break;
		case PKTHDR_T5QuestArena:
			QUESTARENAHANDLER->HandlePacket( pcUser, (PacketQuestArenaTier5*)psPacket );
			break;
		case PKTHDR_NewItem:
			ITEMSERVER->HandlePacket( pcUser, (PacketGetItem*)psPacket );
			break;
	
		case PKTHDR_RollDiceDrop:
			ROLLDICEHANDLER->HandlePacket( pcUser, (PacketRollDiceDrop*)psPacket );
			break;

			//Party
		case PKTHDR_RequestParty:
			PARTYHANDLER->HandlePacket( pcUser, (PacketRequestParty*)psPacket );
			break;
		case PKTHDR_JoinParty:
			PARTYHANDLER->HandlePacket( pcUser, (PacketJoinParty*)psPacket );
			break;
		case PKTHDR_ActionParty:
			PARTYHANDLER->HandlePacket( pcUser, (PacketActionParty*)psPacket );
			break;
		case PKTHDR_RequestRaid:
			PARTYHANDLER->HandlePacket( pcUser, (PacketRequestRaid*)psPacket );
			break;
		case PKTHDR_JoinRaid:
			PARTYHANDLER->HandlePacket( pcUser, (PacketJoinRaid *)psPacket );
			break;
		case PTKHDR_PartyCore:
			PARTYHANDLER->HandlePacket( pcUser, (PacketPartyCore *)psPacket );
			break;

		case PKTHDR_BossTimeUpdate:
			MAPSERVER->HandlePacket( pcUser, (PacketBossTimeUpdate*)psPacket );
			break;

		case PKTHDR_ActionFieldExchange:
			ACTIONFIELDHANDLER->HandlePacket( pcUser, (PacketActionFieldExchange*)psPacket );
			break;
		case PKTHDR_ActionFieldNPCEnter:
			ACTIONFIELDHANDLER->HandlePacket( pcUser, (PacketActionFieldRequest*)psPacket );
			break;

		//Personal Shop
		case PKTHDR_NewPersonalShop:
			PERSONALSHOPHANDLER->HandlePacket( pcUser, (PacketChatBoxMessage*)psPacket );
			break;
		case PKTHDR_PersonalShopItemList:
			PERSONALSHOPHANDLER->HandlePacket( pcUser, (PacketPersonalShopItemList*)psPacket );
			break;
		case PKTHDR_PersonalShopTradeItems:
			PERSONALSHOPHANDLER->HandlePacket( pcUser, (PacketPersonalShopTradeItems*)psPacket );
			break;
		case PKTHDR_PersonalShopRequestView:
			PERSONALSHOPHANDLER->HandlePacket( pcUser, (PacketPersonalShopRequestView*)psPacket );
			break;

		//Xmas Tree
		case PKTHDR_XmasTree:
			XMASTREEHANDLER->HandlePacket( pcUser, (PacketXmasTree*)psPacket );
			break;
		case PKTHDR_XmasTreeNPCGift:
			XMASTREEHANDLER->HandlePacket( pcUser, (PacketXmasTreeItemNPC*)psPacket );
			break;
		case PKTHDR_XmasTreeItemUse:
			XMASTREEHANDLER->HandlePacket( pcUser, (PacketXmasTreeItemUse*)psPacket );
			break;

		case PKTHDR_CharacterDataSyncExGM:
			CHARACTERSERVER->HandlePacket( pcUser, (PacketCharacterDataSyncEx*)psPacket );
			break;

		case PKTHDR_RequestBellatra:
			BELLATRASERVERHANDLER->HandlePacket( pcUser, (PacketBellatraRequest *)psPacket );
			break;

		case PKTHDR_BattleRoyaleDeath:
			BATTLEROYALEHANDLER->HandlePacket( pcUser, (PacketBattleRoyaleGasDeath *)psPacket );
			break;
		case PKTHDR_BattleRoyaleRequest:
			BATTLEROYALEHANDLER->HandlePacket( pcUser, (PacketBattleRoyaleRequest *)psPacket );
			break;

		case PKTHDR_CancelSkill:
			DAMAGEHANDLER->HandlePacket( pcUser, (PacketCancelSkill *)psPacket );
			break;

		case PKTHDR_GlamorousBrush:
			GLAMOROUSBRUSHHANDLER->HandlePacket( pcUser, (PacketGlamorousBrush *)psPacket );
			break;

		//Carnival
		case PKTHDR_CarnivalNPC:
			EVENTSERVER->HandlePacket( pcUser, (PacketNPCCarnival *)psPacket );
			break;

		case PKTHDR_MountsList:
			MOUNTSERVERHANDLER->HandlePacket( pcUser, (PacketMountUserList *)psPacket );
			break;
	}


	/* TEMPORARILY */
	char packettemp[0x202C];
	char * pointertotmp = packettemp;
	int len = psPacket->iLength;

	if ( len > 0x2000 )
		return TRUE;

	memcpy( packettemp + 0x2C, psPacket, len );

	WRITEDWORD( packettemp, (DWORD)pcUserData->pcSocketData );
	WRITEDWORD( (DWORD)(packettemp + 4), (DWORD)pcUserData );

	CALL_WITH_ARG1( 0x0056F300, (DWORD)packettemp );

	return TRUE;
}

void PacketServer::Send( User * pcUser, void * pPacket, BOOL bEncrypted )
{
	if ( pcUser )
		CServerSocketHandler::Send( pcUser, (Packet *)pPacket, bEncrypted );
}

void PacketServer::SendPacketSocket( SocketData * pcSocketData, void * pPacket, BOOL bEncrypted )
{
	CServerSocketHandler::Send( pcSocketData, (Packet *)pPacket, bEncrypted );
}

void PacketServer::SendPacketSocketConnect( SocketData * pcSocketData, void * pPacket, BOOL bEncrypted )
{
	CServerSocketHandler::Send( pcSocketData, (Packet *)pPacket, bEncrypted );
}

void PacketServer::SendPacketBlank( User * pcUser, int iPacket, BOOL bEncrypted )
{
	struct PacketBlank : Packet
	{
		int						iUnk[4];
	};
	PacketBlank sPacket;
	sPacket.iLength	= sizeof( PacketBlank );
	sPacket.iHeader = iPacket;

	SENDPACKET( pcUser, &sPacket, bEncrypted );
}

void PacketServer::SendPacketStage( void * pPacket, int iStageID, BOOL bEncrypt )
{
	for ( int i = 0; i < PLAYERS_MAX; i++ )
	{
		User * u = USERDATATOUSER( &USERSDATA[i] );
		if ( u && u->pcUserData && u->IsConnected() && u->IsInGame() && u->pcUserData->iMapID == iStageID )
		{
			SENDPACKET( u, pPacket, bEncrypt );
		}
	}
}

void PacketServer::SendPacketRange( void * pPacket, Point3D * psPos, int iDistance, BOOL bEncrypt )
{
	for ( int i = 0; i < PLAYERS_MAX; i++ )
	{
		User * u = USERDATATOUSER( &USERSDATA[i] );
		if ( u && u->pcUserData && u->IsConnected() && u->IsInGame() )
		{
			int iShrDistance = iDistance / 72;
			UserData * ud = u->pcUserData;
			int iX = (psPos->iX - u->pcUserData->sPosition.iX) >> 8;
			int iZ = (psPos->iZ - u->pcUserData->sPosition.iZ) >> 8;
			int iDist = (iX*iX + iZ*iZ);
			if ( iDist < iDistance && abs( iX ) < iShrDistance && abs( iZ ) < iShrDistance )
				SENDPACKET( u, pPacket, bEncrypt );
		}
	}
}