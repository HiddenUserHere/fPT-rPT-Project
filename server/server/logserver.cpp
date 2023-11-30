#include "stdafx.h"
#include "logserver.h"
#include <ShellAPI.h>


LogServer::LogServer()
{
	iaUsersOnlineServers[0] = 0;
	iaUsersOnlineServers[1] = 0;
	iaUsersOnlineServers[2] = 0;
}


LogServer::~LogServer()
{
}

void LogServer::SetLogString( char * pszLog, ELogItem eType )
{
	switch ( eType )
	{
		case ITEMLOGID_Get:
			STRINGCOPYLEN( pszLog, 256, "[%s](%s) Got Item ( [%s][0x%08X] %d x %d )" );
			break;
		case ITEMLOGID_Put:
			STRINGCOPYLEN( pszLog, 256, "[%s](%s) Dropped Item ( [%s][0x%08X] %d x %d )" );
			break;
		case ITEMLOGID_Mixing:
			STRINGCOPYLEN( pszLog, 256, "[%s](%s) Mixed Item ( [%s][0x%08X] %d x %d )" );
			break;
		case ITEMLOGID_MixLost:
			STRINGCOPYLEN( pszLog, 256, "[%s](%s) Failed Mix on Item ( [%s][0x%08X] %d x %d )" );
			break;
		case ITEMLOGID_Aging:
			STRINGCOPYLEN( pszLog, 256, "[%s](%s) Aged Item ( [%s][0x%08X] %d x %d )" );
			break;
		case ITEMLOGID_AgingLost:
			STRINGCOPYLEN( pszLog, 256, "[%s](%s) Lost Item on Age ( [%s][0x%08X] %d x %d )" );
			break;
		case ITEMLOGID_AgingUp:
			STRINGCOPYLEN( pszLog, 256, "[%s](%s) Age Item Sucess ( [%s][0x%08X] %d x %d )" );
			break;
		case ITEMLOGID_BuyShop:
			STRINGCOPYLEN( pszLog, 256, "[%s](%s) Bought Item from Shop ( [%s][0x%08X] %d x %d )" );
			break;
		case ITEMLOGID_SellShop:
			STRINGCOPYLEN( pszLog, 256, "[%s](%s) Sold Item on Shop ( [%s][0x%08X] %d x %d )" );
			break;
		case ITEMLOGID_Trade:
			STRINGCOPYLEN( pszLog, 256, "[%s](%s) Traded Item to [%s](%s) ( [%s][0x%08X] %d x %d )" );
			break;
		case ITEMLOGID_Used:
			STRINGCOPYLEN( pszLog, 256, "[%s](%s) Used Item ( [%s][0x%08X] %d x %d )" );
			break;
		case ITEMLOGID_Express:
			STRINGCOPYLEN( pszLog, 256, "[%s](%s) Express Item ( [%s][0x%08X] %d x %d )" );
			break;
		case ITEMLOGID_GetWing:
			STRINGCOPYLEN( pszLog, 256, "[%s](%s) Got Wing ( [%s][0x%08X] %d x %d )" );
			break;
		case ITEMLOGID_WingLost:
			STRINGCOPYLEN( pszLog, 256, "[%s](%s) Lost Wing ( [%s][0x%08X] %d x %d )" );
			break;
		case ITEMLOGID_BuyStar:
			STRINGCOPYLEN( pszLog, 256, "[%s](%s) Bought Star ( [%s][0x%08X] %d x %d )" );
			break;
		case ITEMLOGID_Aging2Up:
			STRINGCOPYLEN( pszLog, 256, "[%s](%s) Age Item +2 Sucess ( [%s][0x%08X] %d x %d )" );
			break;
		case ITEMLOGID_Aging2Down:
			STRINGCOPYLEN( pszLog, 256, "[%s](%s) Age Item -2 Fail ( [%s][0x%08X] %d x %d )" );
			break;
		case ITEMLOGID_Aging1Down:
			STRINGCOPYLEN( pszLog, 256, "[%s](%s) Age Item -1 Fail ( [%s][0x%08X] %d x %d )" );
			break;
		case ITEMLOGID_AgingKeep:
			STRINGCOPYLEN( pszLog, 256, "[%s](%s) Age Fail Same Status Keep ( [%s][0x%08X] %d x %d )" );
			break;
		case ITEMLOGID_Present:
			STRINGCOPYLEN( pszLog, 256, "[%s](%s) Got Start Item ( [%s][0x%08X] %d x %d )" );
			break;
		case ITEMLOGID_PersonalShop:
	//		STRINGCOPYLEN( pszLog, 256, "[%s](%s) Put Item on Personal Shop ( [%s][0x%08X] %d x %d )" );
			break;
		case ITEMLOGID_UseAlchmy:
			STRINGCOPYLEN( pszLog, 256, "[%s](%s) Used Alchemy on Item ( [%s][0x%08X] %d x %d )" );
			break;
		case ITEMLOGID_LostAlchmy:
			STRINGCOPYLEN( pszLog, 256, "[%s](%s) Lost Alchemy on Item ( [%s][0x%08X] %d x %d )" );
			break;
		case ITEMLOGID_GetAlchmy:
			STRINGCOPYLEN( pszLog, 256, "[%s](%s) Get Alchemy on Item ( [%s][0x%08X] %d x %d )" );
			break;
		case ITEMLOGID_GetRecon:
			STRINGCOPYLEN( pszLog, 256, "[%s](%s) Respec Item ( [%s][0x%08X] %d x %d -> %d x %d )" );
			break;
		case ITEMLOGID_LostRecon:
			STRINGCOPYLEN( pszLog, 256, "[%s](%s) Respec Item Fail ( [%s][0x%08X] %d x %d )" );
			break;
		case ITEMLOGID_SmeltingLost:
			STRINGCOPYLEN( pszLog, 256, "[%s](%s) Smelting Item Fail ( [%s][0x%08X] %d x %d )" );
			break;
		case ITEMLOGID_SmeltingGet:
			STRINGCOPYLEN( pszLog, 256, "[%s](%s) Smelting Item Sucess ( [%s][0x%08X] %d x %d )" );
			break;
		case ITEMLOGID_ManufactureLost:
			STRINGCOPYLEN( pszLog, 256, "[%s](%s) Craft Item Lost ( [%s][0x%08X] %d x %d )" );
			break;
		case ITEMLOGID_ManufactureGet:
			STRINGCOPYLEN( pszLog, 256, "[%s](%s) Craft Item Sucess ( [%s][0x%08X] %d x %d )" );
			break;
		case ITEMLOGID_ManufactureFail:
			STRINGCOPYLEN( pszLog, 256, "[%s](%s) Craft Item Fail ( [%s][0x%08X] %d x %d )" );
			break;
		case ITEMLOGID_MixtureResetLost:
			STRINGCOPYLEN( pszLog, 256, "[%s](%s) Reset Mix Item Lost ( [%s][0x%08X] %d x %d )" );
			break;
		case ITEMLOGID_MixtureResetFail:
			STRINGCOPYLEN( pszLog, 256, "[%s](%s) Reset Mix Item Fail ( [%s][0x%08X] %d x %d )" );
			break;
		case ITEMLOGID_MixtureResetGet:
			STRINGCOPYLEN( pszLog, 256, "[%s](%s) Reset Mix Item Sucess ( [%s][0x%08X] %d x %d )" );
			break;

		default:
			break;
	}
}

BOOL LogServer::OnLogCheat( UserData * pcUserData, void * pCheatLog )
{
	if ( pcUserData )
	{
		char szCheatLog[512] = { 0 };

		char szDateTime[20];
		GetDateTime( szDateTime );

		PacketLogCheat * psPacket = (PacketLogCheat*)pCheatLog;

		BOOL Banned = FALSE;
		BOOL Disconnect = FALSE;
		int iDays = 0;

		char * pszIP = "";
		DWORD dwIP = 0;
		if ( pcUserData->pcSocketData )
		{
			pszIP = pcUserData->pcSocketData->szIP;
			dwIP = pcUserData->pcSocketData->lIP;
		}
		char * pszCharacterName = CHARACTERSERVER->GetCharacterName(pcUserData);

		switch ( psPacket->iCheatID )
		{

			case CHEATLOGID_TooManyPackets:
				//SOCKETSERVER->AddBanIP( dwIP );
				if ( pcUserData->pcSocketData && pcUserData->iID && (pcUserData->iMapID != MAPID_OldRuinen2) && (pcUserData->iMapID != MAPID_RoyalDesert) )
					STRINGFORMAT( szCheatLog, "IP[%s] Packets[%d/%d] Time[%d] IP Blocked", pszIP, psPacket->LParam, psPacket->SParam, psPacket->EParam );
				//Disconnect = TRUE;
				break;

			case CHEATLOGID_CopiedItem:
				STRINGFORMAT( szCheatLog, "Character[%s] Copied Item[%d]", pszCharacterName, psPacket->LParam );
				Banned = TRUE;
				break;

			case CHEATLOGID_ValidateCharacterError:
				STRINGFORMAT( szCheatLog, "Character[%s] Validate Character Error" );
				Disconnect = TRUE;
				break;

			case 100:
			case 50:
				if ( pcUserData->iGameLevel )
					return TRUE;
				STRINGFORMAT( szCheatLog, "Character[%s] Hack Warning", pszCharacterName );
				Banned = TRUE;
				break;

			case CHEATLOGID_DelaySkillHack:
				STRINGFORMAT( szCheatLog, "Character[%s] Delay Skill Hack", pszCharacterName );
				Banned = TRUE;
				break;

			case CHEATLOGID_EditLevelError:
				STRINGFORMAT( szCheatLog, "Character[%s] Level Save Error [Game[%d] != Server[%d]]", pszCharacterName, psPacket->LParam, psPacket->SParam );
				Banned = TRUE;
				break;

			case CHEATLOGID_EasterEggFakeItem:
				STRINGFORMAT( szCheatLog, "Character[%s] Fake Item Easter Reward[0x%08X][%d x %d]", pszCharacterName, psPacket->LParam, psPacket->SParam, psPacket->EParam );
				break;
			case CHEATLOGID_CarnivalFakePuzzleItem:
				STRINGFORMAT( szCheatLog, "Character[%s] Fake Item Carnival Puzzle[0x%08X][%d x %d]", pszCharacterName, psPacket->LParam, psPacket->SParam, psPacket->EParam );
				break;
			case CHEATLOGID_ActionFieldFakeItem:
				STRINGFORMAT( szCheatLog, "Character[%s] Fake Item Action Field Exchange[0x%08X][%d x %d]", pszCharacterName, psPacket->LParam, psPacket->SParam, psPacket->EParam );
				break;
			case CHEATLOGID_XmasRudolphFakeBox:
				STRINGFORMAT( szCheatLog, "Character[%s] Fake Item Xmas Rudolph Reward[0x%08X][%d x %d]", pszCharacterName, psPacket->LParam, psPacket->SParam, psPacket->EParam );
				Banned = TRUE;
				break;

			case CHEATLOGID_RespecFakeItem:
				STRINGFORMAT( szCheatLog, "Character[%s] Fake Item Respec[%s][0x%08X][%d x %d]", pszCharacterName, psPacket->LxParam, psPacket->LParam, psPacket->SParam, psPacket->EParam );
				Banned = TRUE;
				break;

			case CHEATLOGID_NPCItemShopFakeItem:
				STRINGFORMAT( szCheatLog, "Character[%s] Fake Item Buy NPC Shop[%s][%08X x %08X]", pszCharacterName, psPacket->EParam, psPacket->LParam, psPacket->SParam );
				Banned = TRUE;
				break;

			case CHEATLOGID_ItemTimerFakeInventory:
				STRINGFORMAT( szCheatLog, "Character[%s] Fake Item Timer Server[0x%08X][%s][%d x %d]", pszCharacterName, psPacket->LParam, psPacket->LxParam, psPacket->SParam, psPacket->EParam );
				Disconnect = TRUE;
				break;

			case 55:
				if ( pcUserData->iGameLevel )
					return TRUE;
				STRINGFORMAT( szCheatLog, "Character[%s] Skill Info Error", pszCharacterName );
			//	Banned = TRUE;
				break;

			case CHEATLOGID_CopiedItemWarehouse:
				STRINGFORMAT( szCheatLog, "Character[%s] Copied Item in Warehouse[%d]", pszCharacterName, psPacket->LParam );
				Banned = TRUE;
				break;

			case CHEATLOGID_WarehouseNewClone:
				STRINGFORMAT( szCheatLog, "Character[%s] Warehouse Clone Hack (ItemID[0x%08X] Code[%d x %d])", pszCharacterName, psPacket->LParam, psPacket->SParam, psPacket->EParam );
				Disconnect = TRUE;
				break;

			case CHEATLOGID_WarehouseGold:
				STRINGFORMAT( szCheatLog, "Character[%s] Gold Fake in Warehouse[%d]", pszCharacterName, psPacket->LParam );
				Banned = TRUE;
				break;
			case CHEATLOGID_WarehouseBug:
				STRINGFORMAT( szCheatLog, "Character[%s] Bug Warehouse (cannot saved)", pszCharacterName, psPacket->LParam );
				break;
			case CHEATLOGID_HackDetected:
				STRINGFORMAT( szCheatLog, "Character[%s] Hack Detected[%s]", pszCharacterName, (char*)psPacket->LParam );
				Disconnect = TRUE;
				break;
			case CHEATLOGID_GoldLimit:
				STRINGFORMAT( szCheatLog, "Character[%s] Gold Hack (Potion Copy[%d][%d])", pszCharacterName, psPacket->SParam, psPacket->LParam );
				Disconnect = TRUE;
				break;
			case CHEATLOGID_SpeedHack:
				STRINGFORMAT( szCheatLog, "Character[%s] Speed Hack (Velocity[%d])", pszCharacterName, psPacket->LParam );
				Banned = TRUE;
				break;
			case CHEATLOGID_TimeErrorSpeedHack:
				STRINGFORMAT( szCheatLog, "Character[%s] Speed Time Hack (Velocity[%d] Time[%d])", pszCharacterName, psPacket->LParam, psPacket->SParam );
				break;
			case CHEATLOGID_TimeMismatch:
				STRINGFORMAT( szCheatLog, "Character[%s] Time Mismatch (Time[%d])", pszCharacterName, psPacket->LParam );
				break;
			case CHEATLOGID_AttackRatioError:
				STRINGFORMAT( szCheatLog, "Character[%s] Attack Rating error (AttackRating[%d])", pszCharacterName, psPacket->LParam );
				Disconnect = TRUE;
				break;
			case CHEATLOGID_PotionCheckError:
				STRINGFORMAT( szCheatLog, "Character[%s] Potion error Check (Count[%d])", pszCharacterName, psPacket->LParam );
				Disconnect = TRUE;
				break;
			case CHEATLOGID_PotionCountError:
				STRINGFORMAT( szCheatLog, "Character[%s] Potion Count Error (Count[%d])", pszCharacterName, psPacket->LParam );
//				Disconnect = TRUE;
				break;
			case CHEATLOGID_CharInfoSaveError:
				STRINGFORMAT( szCheatLog, "Character[%s] Saving Error (Code[%d])", pszCharacterName, psPacket->LParam );
				Banned = TRUE;
				break;
			case CHEATLOGID_DefaultAttackRatingError:
				STRINGFORMAT( szCheatLog, "Character[%s] Attack Rating Error (AttackRating[%d])", pszCharacterName, psPacket->LParam );
				Banned = TRUE;
				break;
			case CHEATLOGID_DefaultAttackSizeError:
				STRINGFORMAT( szCheatLog, "Character[%s] Attack Size Error (AttackSize[%d])", pszCharacterName, psPacket->LParam );
				Banned = TRUE;
				break;
			case CHEATLOGID_ContinuousAttackError:
			//	STRINGFORMAT( szCheatLog, "Character[%s] Attack Continuous Error (AttackError[%d])", pszCharacterName, psPacket->LParam );
			//	pcUserData->BadPlayer = TRUE;
				break;
			case CHEATLOGID_SkillAttackRatingError:
				STRINGFORMAT( szCheatLog, "Character[%s] Attack Rating Error (AttackRating[%d])", pszCharacterName, psPacket->LParam );
				Banned = TRUE;
				break;
			case CHEATLOGID_SkillContinuousAttackError:
				STRINGFORMAT( szCheatLog, "Character[%s] Skill Continuous Error (Error[%d])", pszCharacterName, psPacket->LParam );
				Banned = TRUE;
				break;
			case CHEATLOGID_RestrictedAreaTrespassed:
				if ( psPacket->SParam > 0 )
				{
					STRINGFORMAT( szCheatLog, "Character[%s] Trespass Stage (Level[%d] StageLevel[%d] Stage[%d])", pszCharacterName, psPacket->SParam, psPacket->LParam, pcUserData->iMapID );
					Banned = TRUE;
				}
				break;
			case CHEATLOGID_ItemErrorInventory:
				STRINGFORMAT( szCheatLog, "Character[%s] Inventory Game Item Error ([0x%08X] %d x %d)", pszCharacterName, psPacket->LParam, psPacket->SParam, psPacket->EParam );
				break;
			case CHEATLOGID_ServerItemErrorInventory:
				if ( ((psPacket->LParam & 0xFFFF0000) == ITEMTYPE_EtherCore) || ((psPacket->LParam & 0xFF000000) == ITEMBASE_Potion) || (psPacket->WxParam == 50) )
				{

				}
				else
				{
					if ( ITEMSERVER->CheckQuestItemAging( NULL, psPacket->LParam ) == FALSE )
						Banned = FALSE;
				}

				if ( Banned )
					STRINGFORMAT( szCheatLog, "Character[%s] Item Error [%s]([0x%08X] %d x %d)", pszCharacterName, psPacket->LxParam, psPacket->LParam, psPacket->SParam, psPacket->EParam );

				break;
			case CHEATLOGID_ItemErrorInventoryRecord:
				if ( ((psPacket->LParam & 0xFFFF0000) == ITEMTYPE_EtherCore) || 
					((psPacket->LParam & 0xFF000000) == ITEMBASE_Potion) ||
					((psPacket->LParam & 0xFF000000) == ITEMBASE_Premium) ||
					((psPacket->LParam & 0xFF000000) == ITEMBASE_Recipe) ||
					((psPacket->LParam & 0xFFFF0000) == ITEMTYPE_Crafting) )
				{

				}
				else
				{
					if ( ITEMSERVER->CheckQuestItemAging( NULL, psPacket->LParam ) == FALSE )
						Banned = FALSE;
				}

				if ( Banned )
					STRINGFORMAT( szCheatLog, "Character[%s] Item Error [%s]([0x%08X] %d x %d)", pszCharacterName, psPacket->LxParam, psPacket->LParam, psPacket->SParam, psPacket->EParam );

				break;
			case CHEATLOGID_WarningAutoClick:
				STRINGFORMAT( szCheatLog, "Character[%s] Auto Click Detected (Field[%d])", pszCharacterName, psPacket->LParam );
		//		Disconnect = TRUE;
				break;
			case CHEATLOGID_TeleportFieldHack:
				STRINGFORMAT( szCheatLog, "Character[%s] Teleport Field Hack (Field[%d -> %d])", pszCharacterName, psPacket->LParam, psPacket->SParam );
				Banned = TRUE;
				break;
			case CHEATLOGID_FindThreadHack:
				STRINGFORMAT( szCheatLog, "Character[%s] Find Thread Hack (ThreadID[%d])", pszCharacterName, psPacket->LParam );
				Banned = TRUE;
				break;
			case CHEATLOGID_SuspendThreadHack:
				STRINGFORMAT( szCheatLog, "Character[%s] Suspended Thread Antihack", pszCharacterName );
				Banned = TRUE;
				break;
			case CHEATLOGID_ServerMoneyOverflow:
				STRINGFORMAT( szCheatLog, "Character[%s] Level[%d] Tier[%d] Gold Overflow NPC ((%s) Gold(%d) ActionGold(%d) NewGold(%d))", pszCharacterName, pcUserData->sCharacterData.iLevel, pcUserData->sCharacterData.iRank + 1, psPacket->LxParam, psPacket->LParam, psPacket->SParam, psPacket->EParam );
				Banned = TRUE;
				break;
			case CHEATLOGID_LevelHack:
				STRINGFORMAT( szCheatLog, "Character[%s] Level Hack (Level(%d -> %d) [%d]))", pszCharacterName, psPacket->LParam, psPacket->SParam, psPacket->EParam );
				Banned = TRUE;
				break;

			case CHEATLOGID_MatureHack:
				STRINGFORMAT( szCheatLog, "Character[%s] Mature Hack (CodeVerify[%d] Mature[%d / %d] AgeLvl[%d] ([0x%08X][%d x %d]))", pszCharacterName, psPacket->SParam, psPacket->EParam, psPacket->LxParam, psPacket->WxParam,
					psPacket->LParam, psPacket->SxParam, psPacket->ExParam );
				Banned = TRUE;
				break;

			case CHEATLOGID_DefenseMultHack:
				STRINGFORMAT( szCheatLog, "Character[%s] Defense Mult Hack (Num[%f])", pszCharacterName, *(float*)&psPacket->SParam );
				Banned = TRUE;
				break;

			case CHEATLOGID_RegenFormulaHack:
				STRINGFORMAT( szCheatLog, "Character[%s] Regen Formula Hack (Num[%lf])", pszCharacterName, *(double*)&psPacket->SParam );
				Banned = TRUE;
				break;

			case CHEATLOGID_SkillCooldown:
				STRINGFORMAT( szCheatLog, "Character[%s] Skill Cooldown Hack Index[%d] [%d] < [%d]", pszCharacterName, psPacket->SParam, psPacket->LxParam, psPacket->EParam );
				Banned = TRUE;
				break;

			case CHEATLOGID_AgingHack:
				STRINGFORMAT( szCheatLog, "Character[%s] Age Hack (AgeLvl[%d] Sheltoms[%d] ([0x%08X][%d x %d]))", pszCharacterName, psPacket->LParam, psPacket->SParam, psPacket->EParam, psPacket->LxParam, psPacket->WxParam );
				Banned = TRUE;
				break;

			case CHEATLOGID_FakeGM:
				STRINGFORMAT( szCheatLog, "Character[%s] Fake GameMaster", pszCharacterName );
				Banned = TRUE;
				break;

			case CHEATLOGID_RunSpeedHack:
				STRINGFORMAT( szCheatLog, "Character[%s] Run Speed Hack (Move[%d] Speed[%d])", pszCharacterName, psPacket->LParam, (((psPacket->LParam << 8) / 460) - 250) / 10 );
				Banned = TRUE;
				break;

			case CHEATLOGID_CheckSumFunctionError:
				STRINGFORMAT( szCheatLog, "Character[%s] Checksum Function Error (Size[%d] Addr[0x%08X] Checksum[%d] ChecksumError[%d])", pszCharacterName, psPacket->LParam, psPacket->SParam, psPacket->EParam, psPacket->LxParam );
				Banned = TRUE;
				break;

			case CHEATLOGID_WindowHack:
				STRINGFORMAT( szCheatLog, "Character[%s] Window Hack Found (WindowName[%s])", pszCharacterName, ((PacketLogCheatEx*)psPacket)->szParam );
				Banned = TRUE;

				if ( STRINGCOMPAREI( ((PacketLogCheatEx *)psPacket)->szParam, "form1" ) || STRINGCOMPAREI( ((PacketLogCheatEx *)psPacket)->szParam, "project1" ) ||
					STRINGCOMPAREI( ((PacketLogCheatEx *)psPacket)->szParam, "form2" ) || STRINGCOMPAREI( ((PacketLogCheatEx *)psPacket)->szParam, "project2" ) )
				{
					Banned = FALSE;

					Disconnect = TRUE;
				}

				break;

			case CHEATLOGID_StateCharacterHack:
				STRINGFORMAT( szCheatLog, "Character[%s] State Character Hack", pszCharacterName );
				Banned = TRUE;
				break;

			case CHEATLOGID_DebuggerHack:
				STRINGFORMAT( szCheatLog, "Character[%s] Debugger Hack Found", pszCharacterName );
				Banned = TRUE;
				break;

			case CHEATLOGID_LevelErrorSyncEx:
				STRINGFORMAT( szCheatLog, "Character[%s] Level SyncEx Error (Game[%d] != Server[%d])", pszCharacterName, psPacket->SParam, psPacket->LParam );
				//Banned = TRUE;
				Disconnect = TRUE;
				break;

			case CHEATLOGID_ModuleSyncError:
				STRINGFORMAT( szCheatLog, "Character[%s] Module Code Sync Error ([%d] != [%d])", pszCharacterName, psPacket->LParam, psPacket->SParam );
				Banned = TRUE;
				break;

			case CHEATLOGID_EXEModuleSyncError:
				STRINGFORMAT( szCheatLog, "Character[%s] EXE Module Code Sync Error ([%d] != [%d])", pszCharacterName, psPacket->LParam, psPacket->SParam );
				Banned = TRUE;
				break;

			case CHEATLOGID_CRC32ChecksumError:
				STRINGFORMAT( szCheatLog, "CRC32 Checksum Error Version[%d] EXE(Game[%d] Server[%d]) DLL(Game[%d] Server[%d])", psPacket->LxParam, psPacket->LParam, psPacket->SParam, psPacket->EParam, psPacket->SxParam );
				Disconnect = TRUE;
				break;

			case CHEATLOGID_CaravanItemAddError:
				STRINGFORMAT( szCheatLog, "Character[%s] Add Caravan Fake Item [%s]([0x%08X] %d x %d)", pszCharacterName, psPacket->szBuffer1, psPacket->LParam, psPacket->SParam, psPacket->EParam );
				Banned = TRUE;
				break;

			case CHEATLOGID_CaravanItemDelError:
				STRINGFORMAT( szCheatLog, "Character[%s] Removed Caravan Fake Item [%s]([0x%08X] %d x %d)", pszCharacterName, psPacket->szBuffer1, psPacket->LParam, psPacket->SParam, psPacket->EParam );
				Banned = TRUE;
				break;

			case CHEATLOGID_BellatraRequestGoldError:
				STRINGFORMAT( szCheatLog, "Character[%s] Bellatra Request Gold Error (Gold[%d])", pszCharacterName, psPacket->LParam );
				Banned = TRUE;
				break;

			case CHEATLOGID_ItemFakeBoughtShopID:
				STRINGFORMAT( szCheatLog, "Character[%s] Bought Fake Item Unique ID[%d != %d] [%s]([0x%08X] %d x %d)", pszCharacterName, psPacket->WxParam, psPacket->SxParam, psPacket->szBuffer1, psPacket->LParam, psPacket->SParam, psPacket->EParam );
				Banned = TRUE;
				break;

			case CHEATLOGID_ItemFakeBoughtShopItem:
				STRINGFORMAT( szCheatLog, "Character[%s] Bought Fake Item [%s]([0x%08X] %d x %d)", pszCharacterName, psPacket->szBuffer1, psPacket->LParam, psPacket->SParam, psPacket->EParam );
				Banned = TRUE;
				break;

			case CHEATLOGID_PerfectizeItemDupe:
				STRINGFORMAT( szCheatLog, "Character[%s] Perfectize Item Dupe [%s]([0x%08X] %d x %d)", pszCharacterName, psPacket->szBuffer1, psPacket->LParam, psPacket->SParam, psPacket->EParam );
				Banned = TRUE;
				break;
			case CHEATLOGID_WeaponCostumeError:
				STRINGFORMAT( szCheatLog, "Character[%s] Weapon Costume Error [%s]([0x%08X] %d x %d)", pszCharacterName, psPacket->szBuffer1, psPacket->LParam, psPacket->SParam, psPacket->EParam );
				Banned = TRUE;
				break;
			case CHEATLOGID_ShieldCostumeError:
				STRINGFORMAT( szCheatLog, "Character[%s] Shield Costume Error [%s]([0x%08X] %d x %d)", pszCharacterName, psPacket->szBuffer1, psPacket->LParam, psPacket->SParam, psPacket->EParam );
				Banned = TRUE;
				break;
			case CHEATLOGID_EarringItemError:
				STRINGFORMAT( szCheatLog, "Character[%s] Earring Item Error [%s]([0x%08X] %d x %d)", pszCharacterName, psPacket->szBuffer1, psPacket->LParam, psPacket->SParam, psPacket->EParam );
				Banned = TRUE;
				break;
			case CHEATLOGID_PersonalShopGoldReceiveError:
				STRINGFORMAT( szCheatLog, "Character[%s] Gold Receive Error From[%s][%s] Gold[%d]", pszCharacterName, psPacket->szBuffer1, psPacket->szBuffer2, psPacket->LParam );
				Banned = TRUE;
				break;
			case CHEATLOGID_CharacterQuestInvalid:
			{
				PacketLogCheatEx * psPacketEx = ((PacketLogCheatEx *)psPacket);
				STRINGFORMAT( szCheatLog, "Character[%s] Quest Invalid [%d][%s][%d != %d]", pszCharacterName, psPacketEx->LParam, psPacketEx->szParam, psPacketEx->SParam, psPacketEx->EParam );
				Banned = TRUE;
			}	
			break;
			case CHEATLOGID_GlamorousItemError:
				STRINGFORMAT( szCheatLog, "Character[%s] Glamorous Item Error [%s]([0x%08X] %d x %d)", pszCharacterName, psPacket->szBuffer1, psPacket->LParam, psPacket->SParam, psPacket->EParam );
				Banned = TRUE;
				break;
			case CHEATLOGID_SwapperItemDupe:
				STRINGFORMAT( szCheatLog, "Character[%s] Swapper Item Dupe [%s]([0x%08X] %d x %d)", pszCharacterName, psPacket->szBuffer1, psPacket->LParam, psPacket->SParam, psPacket->EParam );
				Banned = TRUE;
				break;
			case CHEATLOGID_ItemBoxItemDupe:
				STRINGFORMAT( szCheatLog, "Character[%s] Item Box Item Dupe [%s]([0x%08X] %d x %d)", pszCharacterName, psPacket->szBuffer1, psPacket->LParam, psPacket->SParam, psPacket->EParam );
				Banned = TRUE;
				break;

			case CHEATLOGID_GetTickCountHack:
				STRINGFORMAT( szCheatLog, "Character[%s] Speed Timer(GetTickCount)", pszCharacterName );
				Banned = TRUE;
				break;

			case CHEATLOGID_QueryPerformanceHack:
				STRINGFORMAT( szCheatLog, "Character[%s] Speed Timer(QueryPerformanceCounter)", pszCharacterName );
				Banned = TRUE;
				break;

			case CHEATLOGID_ProcessHook:
			{
				std::string strProcessName = lower( ((PacketLogCheatEx *)psPacket)->szParam );
				if ( (strProcessName.find( "gbclientapp" ) != std::string::npos) )
					break;

				STRINGFORMAT( szCheatLog, "Character[%s] Process Hook Name(%s)", pszCharacterName, ((PacketLogCheatEx *)psPacket)->szParam );
				//Disconnect = TRUE;

				if ( (strProcessName.find( "project1" ) != std::string::npos) || (strProcessName.find( "project2" ) != std::string::npos) || 
					(strProcessName.find( "cheat" ) != std::string::npos) || (strProcessName.find( "engine" ) != std::string::npos) )
					Disconnect = TRUE;

			}break;

			case CHEATLOGID_MultipleWindowHackProcess:
				STRINGFORMAT( szCheatLog, "Character[%s] Multiple Window Process Hook[%d]", pszCharacterName, psPacket->LParam );
				break;

			case CHEATLOGID_MixingItemHack:
				STRINGFORMAT( szCheatLog, "Character[%s] Mixing Hack Item[0x%08X]", pszCharacterName, psPacket->LParam );
				break;

			case CHEATLOGID_GoldHackNPC:
				STRINGFORMAT( szCheatLog, "Character[%s] Gold Hack NPC", pszCharacterName );
				Banned = TRUE;
				break;

			case CHEATLOGID_GoldHackWH:
				STRINGFORMAT( szCheatLog, "Character[%s] Gold Hack Warehouse", pszCharacterName );
				Disconnect = TRUE;
				break;

			case CHEATLOGID_ItemPositionError:
				STRINGFORMAT( szCheatLog, "Character[%s] Item Position Error[0x%08X][0x%08X][%d]", pszCharacterName, psPacket->LParam, psPacket->SParam, psPacket->EParam );
				Banned = TRUE;
				break;

			case CHEATLOGID_ManufactureItemFake:
				STRINGFORMAT( szCheatLog, "Character[%s] Manufacture Item Fake ([0x%08X][%d x %d]))", pszCharacterName, psPacket->LParam, psPacket->SParam, psPacket->EParam );
				Banned = TRUE;
				break;

			case CHEATLOGID_LimitDamageOver:
			case CHEATLOGID_ClientEnergyBarError:
			case CHEATLOGID_ServerMoneyErrorInventory:
			case CHEATLOGID_ClientWarningSkillForgery2:
			case CHEATLOGID_CopiedItemError:
			case CHEATLOGID_ServerInventoryCopiedItem:
			case CHEATLOGID_WarningInvincibleMode:
			case CHEATLOGID_ClientDamagePacketError:
			case CHEATLOGID_DamagePacketError:
			case CHEATLOGID_WarningMacroMouse:
			case CHEATLOGID_ServerToServerItemError:
			case CHEATLOGID_WeightError:
				// no need
				break;

			case CHEATLOGID_LimitDamageTime:
				STRINGFORMAT( szCheatLog, "Character[%s] Damage Limit Time Limit[%d/%d] Time[%d]", pszCharacterName, psPacket->LParam, psPacket->SParam, psPacket->EParam );
				Disconnect = TRUE;
				break;

			case CHEATLOGID_ContinuousSaveFailedError:
				Disconnect = TRUE;
				break;

			case CHEATLOGID_GMReason:
				STRINGFORMAT( szCheatLog, "Character[%s] %s", pszCharacterName, ((PacketLogCheatEx*)psPacket)->szParam );
				Banned = TRUE;
				break;

			default:
				STRINGFORMAT( szCheatLog, "Character[%s] Unknown Log[%d]", pszCharacterName, psPacket->iCheatID );
				break;
		}

		if ( IsNull( szCheatLog ) )
			return TRUE;

		int iActionType = 0;

		if ( Banned && pcUserData->iGameLevel == GAMELEVEL_None )
		{
			iActionType = 2;
			DWORD dwTime = 0;
			if ( iDays )
				dwTime = TICKCOUNT + (60 * 60 * (24 * iDays));

			SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "UPDATE UserInfo SET BanStatus=1, UnbanDate=NULL WHERE AccountName=?" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_String, pcUserData->szAccountName );
					pcDB->Execute();
				}
				pcDB->Close();
			}
		}
		else if ( Disconnect && pcUserData->iGameLevel == GAMELEVEL_None )
		{
			iActionType = 1;
		}

		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );
		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "INSERT INTO CheatLog VALUES (?,?,?,?,?,?,?)" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_String, pszIP );
				pcDB->BindParameterInput( 2, PARAMTYPE_String, pcUserData->szAccountName );
				pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &iActionType );
				pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &psPacket->iCheatID );
				pcDB->BindParameterInput( 5, PARAMTYPE_String, szCheatLog );
				pcDB->BindParameterInput( 6, PARAMTYPE_String, szDateTime );
				pcDB->BindParameterInput( 7, PARAMTYPE_Integer, &GAME_SERVER );
				pcDB->Execute();
			}
			pcDB->Close();
		}

		if ( (Banned || Disconnect) && pcUserData->iGameLevel == GAMELEVEL_None )
		{
			SENDPACKETBLANK( USERDATATOUSER(pcUserData), PKTHDR_Disconnect, TRUE );
			pcUserData->dwDisconnectTime = TICKCOUNT + 1500;
		}

	}
	return TRUE;
}

void LogServer::OnLogAging( UserData * pcUserData, int iLogID, const char * pszLog )
{
	if ( pcUserData )
	{
		// Set to database
		char szDateTime[ 20 ];
		GetDateTime( szDateTime );

		char * pszIP = "";
		if ( pcUserData->pcSocketData )
			pszIP = pcUserData->pcSocketData->szIP;

//		SQLSERVER->Select( "LogDB", "INSERT INTO ItemLog VALUES ('%s','%s',%d,'%s','%s',%d)", pszIP, pcUserData->szAccountName, iLogID, pszLog, szDateTime, GAME_SERVER );
//		SQLSERVER->Close();
	}
}

void LogServer::OnLogMixtrue( UserData * pcUserData, int iLogID, const char * pszLog )
{
	if ( pcUserData )
	{
		// Set to database
		char szDateTime[ 20 ];
		GetDateTime( szDateTime );

		char * pszIP = "";
		if ( pcUserData->pcSocketData )
			pszIP = pcUserData->pcSocketData->szIP;

//		SQLSERVER->Select( "LogDB", "INSERT INTO ItemLog VALUES ('%s','%s',%d,'%s','%s',%d)", pszIP, pcUserData->szAccountName, iLogID, pszLog, szDateTime, GAME_SERVER );
//		SQLSERVER->Close();
	}
}

void LogServer::OnLogItemGeneral( UserData * pcUserData, int iLogID, const char * pszLog )
{
	if ( pcUserData )
	{
		// Set to database
		char szDateTime[ 20 ];
		GetDateTime( szDateTime );

		char * pszIP = "";
		if ( pcUserData->pcSocketData )
			pszIP = pcUserData->pcSocketData->szIP;

//		SQLSERVER->Select( "LogDB", "INSERT INTO ItemLog VALUES ('%s','%s',%d,'%s','%s',%d)", pszIP, pcUserData->szAccountName, iLogID, pszLog, szDateTime, GAME_SERVER );
//		SQLSERVER->Close();
	}
}

void LogServer::OnLogWrite( UserData * pcUserData, int iLogID, const char * pszLog, ELogServerType eType )
{
	switch ( eType )
	{
		case ELogServerType::LOGTYPEID_Aging:
			OnLogAging( pcUserData, iLogID, pszLog );
			break;

		case ELogServerType::LOGTYPEID_Mixtrue:
			OnLogMixtrue( pcUserData, iLogID, pszLog );
			break;
		case ELogServerType::LOGTYPEID_Item:
			OnLogItemGeneral( pcUserData, iLogID, pszLog );
			break;

		default:
			break;
	}
}

void LogServer::OnLogItemData( LogItem * psLog )
{
	ELogItem eType = (ELogItem)psLog->Flag;

	if ( eType == ITEMLOGID_Get )
	{
		if ( psLog->Item[0].dwCode == ITEMID_Gold )
			return;
		if ( (psLog->Item[0].dwCode & 0xFF000000) == ITEMBASE_Potion )
			return;
	}

	char szLog[ 256 ] = { 0 };

	// Set log format
	char szLogFormat[ 256 ] = { 0 };
	SetLogString( szLogFormat, eType );
	
	int iIP = psLog->IP;
	
	char szDateTime[ 20 ];
	GetDateTime( szDateTime );
	
	// Convert IP to string
	char szIP[ 20 ] = { 0 };
	STRINGFORMAT( szIP, "%d.%d.%d.%d",iIP & 0xFF, (iIP >> 8) & 0xFF, (iIP >> 16) & 0xFF, (iIP >> 24) & 0xFF );

	BOOL bStop = FALSE;

	// Set log items
	char szItemName[32] = { 0 };
	for ( int i = 0; i < psLog->ItemCount; i++ )
	{
		ITEMSERVER->FindItemName( psLog->Item[i].dwCode, szItemName );
		if ( eType == ITEMLOGID_GetRecon )
		{
			STRINGFORMAT( szLog, szLogFormat, psLog->UserID, psLog->CharName, szItemName, psLog->Item[0].dwCode, psLog->Item[0].dwHead, psLog->Item[0].dwCheckSum,
				psLog->Item[1].dwHead, psLog->Item[1].dwCheckSum );
			bStop = TRUE;
		}
		else
			STRINGFORMAT( szLog, szLogFormat, psLog->UserID, psLog->CharName, szItemName, psLog->Item[i].dwCode, psLog->Item[i].dwHead, psLog->Item[i].dwCheckSum );

		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );
		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "INSERT INTO ItemLog VALUES (?,?,?,?,?,?,?,?)" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_String, szIP );
				pcDB->BindParameterInput( 2, PARAMTYPE_String, psLog->UserID );
				pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &psLog->Flag );
				pcDB->BindParameterInput( 4, PARAMTYPE_String, szLog );
				pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &psLog->Item[ i ].dwHead );
				pcDB->BindParameterInput( 6, PARAMTYPE_Integer, &psLog->Item[ i ].dwCheckSum );
				pcDB->BindParameterInput( 7, PARAMTYPE_String, szDateTime );
				pcDB->BindParameterInput( 8, PARAMTYPE_Integer, &GAME_SERVER );
				pcDB->Execute();
			}
			pcDB->Close();
		}
		if ( bStop == TRUE )
			break;
	}
}

void LogServer::OnLogItemTradeData( LogItemTrade * psLog )
{
	ELogItem eType = (ELogItem)psLog->Flag;

	char szLog[ 256 ] = { 0 };

	// Set log format
	char szLogFormat[ 256 ] = { 0 };
	SetLogString( szLogFormat, eType );
	
	int iIP = psLog->IP;
	
	char szDateTime[ 20 ];
	GetDateTime( szDateTime );
	
	// Convert IP to string
	char szIP[ 20 ] = { 0 };
	STRINGFORMAT( szIP, "%d.%d.%d.%d",iIP & 0xFF, (iIP >> 8) & 0xFF, (iIP >> 16) & 0xFF, (iIP >> 24) & 0xFF );

	// Set log items
	char szItemName[32] = { 0 };

	int iNull = 0;

	if ( eType == ITEMLOGID_PersonalShop )
	{
		if ( psLog->Money > 0 )
		{
			iIP = psLog->TIP;

			STRINGFORMAT( szIP, "%d.%d.%d.%d", iIP & 0xFF, (iIP >> 8) & 0xFF, (iIP >> 16) & 0xFF, (iIP >> 24) & 0xFF );

			STRINGFORMAT( szLog, "[%s](%s) Received Personal Shop Gold from [%s](%s) ( %d )", psLog->UserID, psLog->CharName, psLog->TUserID, psLog->TCharName, psLog->Money );

			SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );
			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "INSERT INTO ItemLog VALUES (?,?,?,?,?,?,?,?)" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_String, szIP );
					pcDB->BindParameterInput( 2, PARAMTYPE_String, psLog->UserID );
					pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &psLog->Flag );
					pcDB->BindParameterInput( 4, PARAMTYPE_String, szLog );
					pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &psLog->Money );
					pcDB->BindParameterInput( 6, PARAMTYPE_Integer, &iNull );
					pcDB->BindParameterInput( 7, PARAMTYPE_String, szDateTime );
					pcDB->BindParameterInput( 8, PARAMTYPE_Integer, &GAME_SERVER );
					pcDB->Execute();
				}
				pcDB->Close();
			}
		}
		else
		{
			ITEMSERVER->FindItemName( psLog->Item[0].dwCode, szItemName );

			STRINGFORMAT( szLog, "[%s](%s) Sold Personal Shop Item to [%s](%s) ( [%s][0x%08X] %d x %d )", psLog->TUserID, psLog->TCharName, psLog->UserID, psLog->CharName, szItemName, psLog->Item[0].dwCode, psLog->Item[0].dwHead, psLog->Item[0].dwCheckSum );

			SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );
			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "INSERT INTO ItemLog VALUES (?,?,?,?,?,?,?,?)" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_String, szIP );
					pcDB->BindParameterInput( 2, PARAMTYPE_String, psLog->TUserID );
					pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &psLog->Flag );
					pcDB->BindParameterInput( 4, PARAMTYPE_String, szLog );
					pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &psLog->Item[0].dwHead );
					pcDB->BindParameterInput( 6, PARAMTYPE_Integer, &psLog->Item[0].dwCheckSum );
					pcDB->BindParameterInput( 7, PARAMTYPE_String, szDateTime );
					pcDB->BindParameterInput( 8, PARAMTYPE_Integer, &GAME_SERVER );
					pcDB->Execute();
				}
				pcDB->Close();
			}

			STRINGFORMAT( szLog, "[%s](%s) Bought Personal Shop Item from [%s](%s) ( [%s][0x%08X] %d x %d )", psLog->UserID, psLog->CharName, psLog->TUserID, psLog->TCharName, szItemName, psLog->Item[0].dwCode, psLog->Item[0].dwHead, psLog->Item[0].dwCheckSum );

			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "INSERT INTO ItemLog VALUES (?,?,?,?,?,?,?,?)" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_String, szIP );
					pcDB->BindParameterInput( 2, PARAMTYPE_String, psLog->UserID );
					pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &psLog->Flag );
					pcDB->BindParameterInput( 4, PARAMTYPE_String, szLog );
					pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &psLog->Item[0].dwHead );
					pcDB->BindParameterInput( 6, PARAMTYPE_Integer, &psLog->Item[0].dwCheckSum );
					pcDB->BindParameterInput( 7, PARAMTYPE_String, szDateTime );
					pcDB->BindParameterInput( 8, PARAMTYPE_Integer, &GAME_SERVER );
					pcDB->Execute();
				}
				pcDB->Close();
			}
		}
	}

	if ( (eType == ITEMLOGID_Trade) )
	{
		int iLogIDT = ITEMLOGID_TradeItemSuccessReceive;

		for ( int i = 0; i < psLog->ItemCount; i++ )
		{
			ITEMSERVER->FindItemName( psLog->Item[i].dwCode, szItemName );

			STRINGFORMAT( szLog, "[%s](%s) Received Trade item from [%s](%s) ( [%s][0x%08X] %d x %d )", psLog->TUserID, psLog->TCharName, psLog->UserID, psLog->CharName, szItemName, psLog->Item[0].dwCode, psLog->Item[i].dwHead, psLog->Item[i].dwCheckSum );

			SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );
			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "INSERT INTO ItemLog VALUES (?,?,?,?,?,?,?,?)" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_String, szIP );
					pcDB->BindParameterInput( 2, PARAMTYPE_String, psLog->TUserID );
					pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &iLogIDT );
					pcDB->BindParameterInput( 4, PARAMTYPE_String, szLog );
					pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &psLog->Item[0].dwHead );
					pcDB->BindParameterInput( 6, PARAMTYPE_Integer, &psLog->Item[0].dwCheckSum );
					pcDB->BindParameterInput( 7, PARAMTYPE_String, szDateTime );
					pcDB->BindParameterInput( 8, PARAMTYPE_Integer, &GAME_SERVER );
					pcDB->Execute();
				}
				pcDB->Close();
			}
		}
	}


	for ( int i = 0; i < psLog->ItemCount; i++ )
	{
		if ( psLog->Flag == ITEMLOGID_PersonalShop )
			break;

		ITEMSERVER->FindItemName( psLog->Item[i].dwCode, szItemName );

		STRINGFORMAT( szLog, szLogFormat, psLog->UserID, psLog->CharName, psLog->TUserID, psLog->TCharName, szItemName, psLog->Item[ i ].dwCode, psLog->Item[ i ].dwHead, psLog->Item[ i ].dwCheckSum );	
		
		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );
		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "INSERT INTO ItemLog VALUES (?,?,?,?,?,?,?,?)" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_String, szIP );
				pcDB->BindParameterInput( 2, PARAMTYPE_String, psLog->UserID );
				pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &psLog->Flag );
				pcDB->BindParameterInput( 4, PARAMTYPE_String, szLog );
				pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &psLog->Item[ i ].dwHead );
				pcDB->BindParameterInput( 6, PARAMTYPE_Integer, &psLog->Item[ i ].dwCheckSum );
				pcDB->BindParameterInput( 7, PARAMTYPE_String, szDateTime );
				pcDB->BindParameterInput( 8, PARAMTYPE_Integer, &GAME_SERVER );
				pcDB->Execute();
			}
			pcDB->Close();
		}
	}
}

void LogServer::OnLogItemExpressData(LogItemGift * psLog)
{
	ELogItem eType = (ELogItem)psLog->Flag;

	char szLog[ 256 ] = { 0 };

	// Set log format
	char szLogFormat[ 256 ] = { 0 };
	SetLogString( szLogFormat, eType );
	
	int iIP = psLog->IP;
	
	char szDateTime[ 20 ];
	GetDateTime( szDateTime );
	
	// Convert IP to string
	char szIP[ 20 ] = { 0 };
	STRINGFORMAT( szIP, "%d.%d.%d.%d",iIP & 0xFF, (iIP >> 8) & 0xFF, (iIP >> 16) & 0xFF, (iIP >> 24) & 0xFF );

	// Set log item
	char szItemName[32] = { 0 };
	ITEMSERVER->FindItemName( psLog->Item.dwCode, szItemName );
	STRINGFORMAT( szLog, szLogFormat, psLog->UserID, psLog->CharName, szItemName, psLog->Item.dwCode, psLog->Item.dwHead, psLog->Item.dwCheckSum );

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "INSERT INTO ItemLog VALUES (?,?,?,?,?,?,?,?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, szIP );
			pcDB->BindParameterInput( 2, PARAMTYPE_String, psLog->UserID );
			pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &psLog->Flag );
			pcDB->BindParameterInput( 4, PARAMTYPE_String, szLog );
			pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &psLog->Item.dwHead );
			pcDB->BindParameterInput( 6, PARAMTYPE_Integer, &psLog->Item.dwCheckSum );
			pcDB->BindParameterInput( 7, PARAMTYPE_String, szDateTime );
			pcDB->BindParameterInput( 8, PARAMTYPE_Integer, &GAME_SERVER );
			pcDB->Execute();
		}
		pcDB->Close();
	}
}

void LogServer::OnLogItem( int eType, int iSize, void * psLogItem )
{
	switch ( (ELogServerType)eType )
	{
		case LogServer::LOGTYPEID_LogItem:
			OnLogItemData( (LogItem*)psLogItem );
			break;
		case LogServer::LOGTYPEID_LogItemTrade:
			OnLogItemTradeData( (LogItemTrade*)psLogItem );
			break;
		case LogServer::LOGTYPEID_LogItemExpress:
			OnLogItemExpressData( (LogItemGift*)psLogItem );
			break;
		default:
			break;
	}
}

void LogServer::OnLogLoadItemCharacter( UserData * pcUserData )
{

}

void LogServer::OnLogUnloadItemCharacter( UserData * pcUserData )
{
	
}

void LogServer::OnLog( const char * pszLogName, const char * pszLog )
{

}

void LogServer::OnLog( User * pcUser, const char * pszLogName, const char * pszLog )
{

}

void LogServer::OnLogEx( const char * pszLogName, const char * pszLog, ... )
{

}

void LogServer::OnLogEx( User * pcUser, int iLogID, const char * pszLog, ... )
{
	char szLogDescription[512] = { 0 };
	char * pszIP = NULL;
	char szDateTime[20] = { 0 };
	GetDateTime( szDateTime );

	if ( pcUser )
	{
		if ( pcUser->pcUserData->pcSocketData )
			pszIP = pcUser->pcUserData->pcSocketData->szIP;

		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );

		// Set log item
		va_list ap;
		va_start( ap, pszLog );
		vsnprintf_s( szLogDescription, 512, pszLog, ap );
		va_end( ap );

		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "INSERT INTO CharacterLog VALUES (?,?,?,?,?,?)" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_String, pszIP );
				pcDB->BindParameterInput( 2, PARAMTYPE_String, pcUser->pcUserData->szAccountName );
				pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &iLogID );
				pcDB->BindParameterInput( 4, PARAMTYPE_String, szLogDescription, 512 );
				pcDB->BindParameterInput( 5, PARAMTYPE_String, szDateTime );
				pcDB->BindParameterInput( 6, PARAMTYPE_Integer, &GAME_SERVER );
				pcDB->Execute();
			}
			pcDB->Close();
		}
	}
}

void LogServer::OnLogEx( const char * pszAccountName, const char * pszIP, int iLogID, const char * pszLog, ... )
{
	char szLogDescription[512] = {0};
	const char * pszLIP = "127.0.0.1";
	char szDateTime[20] = {0};
	GetDateTime( szDateTime );

	if ( pszAccountName && (pszAccountName[0] != 0) )
	{
		if ( pszIP && (pszLIP[0] != 0) )
			pszLIP = pszIP;

		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );

		// Set log item
		va_list ap;
		va_start( ap, pszLog );
		vsnprintf_s( szLogDescription, 512, pszLog, ap );
		va_end( ap );

		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "INSERT INTO CharacterLog VALUES (?,?,?,?,?,?)" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_String, (void *)pszLIP, STRLEN( pszLIP ) );
				pcDB->BindParameterInput( 2, PARAMTYPE_String, (void *)pszAccountName, STRLEN( pszAccountName ) );
				pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &iLogID );
				pcDB->BindParameterInput( 4, PARAMTYPE_String, szLogDescription, STRLEN( szLogDescription ) );
				pcDB->BindParameterInput( 5, PARAMTYPE_String, szDateTime );
				pcDB->BindParameterInput( 6, PARAMTYPE_Integer, &GAME_SERVER );
				pcDB->Execute();
			}
			pcDB->Close();
		}
	}
}

void LogServer::OnLogUsersOnline()
{
	SYSTEMTIME * ps = GetServerTime();

	if ( wLastHour == (-1) )
		wLastHour = ps->wHour;

	if ( LOGIN_SERVER && wLastHour != ps->wHour )
	{
		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );
		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "INSERT INTO UsersOnlineRecord([UsersOnlineSubServer1],[UsersOnlineSubServer2],[UsersOnlineSubServer3],[TotalUsersOnline],[Date]) VALUES (?,?,?,?,GETDATE())" ) )
			{
				int iTotal = iaUsersOnlineServers[0] + iaUsersOnlineServers[1] + iaUsersOnlineServers[2];

				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iaUsersOnlineServers[0] );
				pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iaUsersOnlineServers[1] );
				pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &iaUsersOnlineServers[2] );
				pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &iTotal );
				pcDB->Execute();
			}
			pcDB->Close();
		}

		wLastHour = ps->wHour;
	}
}

void LogServer::HandlePacket( PacketNetUsersOnline * psPacket )
{
	if ( (psPacket->iID >= 0) && (psPacket->iID < 3) )
	{
		iaUsersOnlineServers[psPacket->iID] = psPacket->iUsersOnline;
	}
}

void LogServer::AddFirewallBan( const char * pszIP )
{
	return;
	if ( pszIP && pszIP[0] != 0 )
	{
		static char szBlockFirewall[256] = { 0 };
		STRINGFORMAT( szBlockFirewall, "advfirewall firewall add rule name=\"BlockPT\" protocol=TCP dir=in action=block remoteip=%s localport=10000-10050", pszIP );
		ShellExecuteA( 0, "runas", "netsh", szBlockFirewall, NULL, SW_HIDE );
	}
}

void LogServer::OnLogItemEx( User * pcUser, int iLogID, const char * pszLog, ... )
{
	if ( pcUser && pcUser->pcUserData && pcUser->pcUserData->pcSocketData )
	{
		static char szLog[512] = { 0 };
		char szDateTime[20];
		GetDateTime( szDateTime );

		// Convert IP to string
		char szIP[20] = { 0 };
		STRINGFORMAT( szIP, "%s", pcUser->pcUserData->pcSocketData->szIP );

		// Set log item
		va_list ap;
		va_start( ap, pszLog );
		vsnprintf_s( szLog, 512, pszLog, ap );
		va_end( ap );

		int iUnk = 0;

		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );
		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "INSERT INTO ItemLog VALUES (?,?,?,?,?,?,?,?)" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_String, szIP, 20 );
				pcDB->BindParameterInput( 2, PARAMTYPE_String, pcUser->pcUserData->szAccountName, 32 );
				pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &iLogID );
				pcDB->BindParameterInput( 4, PARAMTYPE_String, szLog, 512 );
				pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &iUnk );
				pcDB->BindParameterInput( 6, PARAMTYPE_Integer, &iUnk );
				pcDB->BindParameterInput( 7, PARAMTYPE_String, szDateTime, 20 );
				pcDB->BindParameterInput( 8, PARAMTYPE_Integer, &GAME_SERVER );
				pcDB->Execute();
			}
			pcDB->Close();
		}
	}
}

void LogServer::OnLogItemEx( const char * pszAccountName, const char * pszIP, int iLogID, const char * pszLog, ... )
{
	static char szLog[512] = { 0 };
	char szDateTime[20];
	GetDateTime( szDateTime );

	// Convert IP to string
	char szIP[20] = { 0 };
	STRINGFORMAT( szIP, "%s", pszIP );

	// Set log item
	va_list ap;
	va_start( ap, pszLog );
	vsnprintf_s( szLog, 512, pszLog, ap );
	va_end( ap );

	int iUnk = 0;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "INSERT INTO ItemLog VALUES (?,?,?,?,?,?,?,?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, szIP, 20 );
			pcDB->BindParameterInput( 2, PARAMTYPE_String, (char*)pszAccountName, 32 );
			pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &iLogID );
			pcDB->BindParameterInput( 4, PARAMTYPE_String, szLog, 512 );
			pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &iUnk );
			pcDB->BindParameterInput( 6, PARAMTYPE_Integer, &iUnk );
			pcDB->BindParameterInput( 7, PARAMTYPE_String, szDateTime, 20 );
			pcDB->BindParameterInput( 8, PARAMTYPE_Integer, &GAME_SERVER );
			pcDB->Execute();
		}
		pcDB->Close();
	}
}

void LogServer::LogSocketClose( SocketData * pcSocketData, char * pszLocation, void * p )
{

	// Log It
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );
	if ( pcSocketData && pcDB->Open() )
	{
		char * pszAccountName = pcSocketData->ud != 0 ? pcSocketData->ud->szAccountName : "";
		BOOL bNetServer = pcSocketData->u != 0 ? pcSocketData->u->bNetServer : FALSE;
		BOOL bKeepAlive = pcSocketData->bKeepAlive;

		char szRetAddr[32] = { 0 };
		STRINGFORMAT( szRetAddr, "%p", p );
		char szRetAddr2[32] = { 0 };
		STRINGFORMAT( szRetAddr2, "%p", _ReturnAddress() );

		char * pszServer = GAME_SERVER != 0 ? "GameServer" : "LoginServer";

		if ( pcDB->Prepare( "INSERT INTO Disconnects([IP],[AccountName],[NetServer],[KeepAlive],[ServerType],[Location],[ReturnAddress],[ReturnAddressCall],[Date]) VALUES(?,?,?,?,?,?,?,?,GETDATE())" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, pcSocketData->szIP, 16 );
			pcDB->BindParameterInput( 2, PARAMTYPE_String, pszAccountName, STRLEN( pszAccountName ) );
			pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &bNetServer );
			pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &pcSocketData->bKeepAlive );
			pcDB->BindParameterInput( 5, PARAMTYPE_String, pszServer, STRLEN( pszServer ) );
			pcDB->BindParameterInput( 6, PARAMTYPE_String, pszLocation, STRLEN( pszLocation ) );
			pcDB->BindParameterInput( 7, PARAMTYPE_String, szRetAddr, 32 );
			pcDB->BindParameterInput( 8, PARAMTYPE_String, szRetAddr2, 32 );
			pcDB->Execute();
		}
		pcDB->Close();
	}
}
