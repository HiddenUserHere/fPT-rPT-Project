#include "stdafx.h"
#include "eventserver.h"


CEventServer::CEventServer()
{
	wLastHourOnlineReward = 26;
	iTotalPercent = 0;
	iTotalPercentEaster = 0;
	iXmasTotalPercent = 0;
}

CEventServer::~CEventServer()
{
}

void CEventServer::GiveFriendGifts()
{
	std::vector<PrizeXmasAccount *> vAccounts;

	if ( SQLGetAccountGifts( vAccounts ) )
	{
		for ( auto psAccount : vAccounts )
		{
			SPrizeData * ps = GetPrizeXmasRewardItem();

			ITEMSERVER->AddItemOpenBox( psAccount->szAccountName, ps->szCode, 0, ps->iQuantity );

			LOGSERVER->OnLogItemEx( psAccount->szAccountName, psAccount->szAccountIP, ITEMLOGID_XmasGiftDaily, "AccountName[%s] Character[%s] Won Daily Gift ItemCode[%s] Count[%d]",
									psAccount->szAccountName, psAccount->szCharacterName, ps->szCode, ps->iQuantity );
		}

		SQLClearAccountGifts();
	}

	for ( auto ps : vAccounts )
		DELET( ps );

	vAccounts.clear();

	CHATSERVER->SendChatAll( CHATCOLOR_Global, "Santa Clause> Friendship Xmas Gifts has been sent, visit Item Distributor to get yours!" );
}

void CEventServer::GiveRudolphPrizes()
{
	if ( EVENT_XMAS == FALSE )
		return;

	for ( std::vector<PrizeEventRudolphUser*>::iterator it = vXmasEventRudolph.begin(); it != vXmasEventRudolph.end(); it++ )
	{
		PrizeEventRudolphUser * psc = (*it);

		if ( psc->szAccountName[0] != 0 )
		{
			SPrizeData * ps = GetPrizeXmasRewardItem();

			ITEMSERVER->AddItemOpenBox( psc->szAccountName, ps->szCode, 0, ps->iQuantity );

			LOGSERVER->OnLogItemEx( psc->szAccountName, psc->szAccountIP, ITEMLOGID_RewardXmasRudolphSet, "AccountName[%s] Character[%s] Trade Xmas Rudolph Reward Set ID[%u] Box[%s] ItemCode[%s] Count[%d]",
				psc->szAccountName, psc->szCharacterName, psc->iID, psc->iItemGiftID == ITEMID_GiftBox1 ? "Red" : "Green", ps->szCode, ps->iQuantity );
		}

		delete psc;
	}

	vXmasEventRudolph.clear();

	CHATSERVER->SendChatAll( CHATCOLOR_Global, "Santa Clause> Ho Ho Ho!!" );
	CHATSERVER->SendChatAll( CHATCOLOR_Global, "Santa Clause> New presents under the Xmas Tree, visit Item Distributor to get yours!" );
}

void CEventServer::HandlePacket( UserData * pcUserData, PacketNPCEasterEgg * psPacket )
{
	switch ( psPacket->iAction )
	{
		case PacketNPCEasterEgg::ACTION_None:

			//Welcome Message when open NPC...
			CHATSERVER->SendChat( USERDATATOUSER( pcUserData ), EChatColor::CHATCOLOR_Global, "> Easter Event:" );
			CHATSERVER->SendChat( USERDATATOUSER( pcUserData ), EChatColor::CHATCOLOR_Global, "> Welcome, let's get some prizes? ^^'" );

			break;
		case PacketNPCEasterEgg::ACTION_GetReward:
			int iCount;
			iCount = 0;
			
			//Check if get Items needed....
			for ( int i = 0; i < EASTER_ITEMS_MAX; i++ )
			{
				if ( psPacket->iaItemID[i] == ITEMID_RedEasterEgg || psPacket->iaItemID[i] == ITEMID_GreenEasterEgg || psPacket->iaItemID[i] == ITEMID_PinkEasterEgg )
					iCount++;
			}

			//Get all Items?
			if ( iCount == EASTER_ITEMS_MAX )
			{
				//Check for Valid Items...
				for ( int i = 0; i < EASTER_ITEMS_MAX; i++ )
				{
					//Fake?
					if ( ITEMSERVER->UseItem( pcUserData, psPacket->iaItemID[i], psPacket->dwaHead[i], psPacket->dwaCheckSum[i] ) < 0 )
					{
						//Log
						PacketLogCheat s;
						s.iCheatID	= CHEATLOGID_EasterEggFakeItem;
						s.LParam	= psPacket->iaItemID[i];
						s.SParam	= psPacket->dwaHead[i];
						s.EParam	= psPacket->dwaCheckSum[i];
						LOGSERVER->OnLogCheat( pcUserData, &s );
						return;
					}
					
					//Get Item Name
					char * pszItemName	= "Red Easter Egg";
					if ( psPacket->iaItemID[i] == ITEMID_GreenEasterEgg )
						pszItemName		= "Green Easter Egg";
					else if ( psPacket->iaItemID[i] == ITEMID_PinkEasterEgg )
						pszItemName		= "Pink Easter Egg";


					//Log it...
					LOGSERVER->OnLogItemEx( USERDATATOUSER( pcUserData ), ITEMLOGID_RewardEasterGet, "Character[%s] Put Easter Egg ([%s][%08X][%d x %d])",
						CHARACTERSERVER->GetCharacterName( pcUserData ), pszItemName, psPacket->iaItemID[i], psPacket->dwaHead[i], psPacket->dwaCheckSum[i] );

				}

				//Get Reward
				SPrizeData * ps = GetPrizeRewardItemEaster();
				if ( ps )
				{
					char szItemName[32] = { 0 };

					//Find Item Name to put on Log...
					if ( ITEMSERVER->FindItemName( ps->szCode, szItemName ) )
					{
						//Find Item by Code to get ID to use on Log...
						ItemData * pcItemData = ITEMSERVER->FindItemPointerTable( ps->szCode );			
						if ( pcItemData )
						{
							//Send To ItemBox
							ITEMSERVER->AddItemOpenBox( pcUserData, ps->szCode, 0, ps->iQuantity );
						
							//Log it...
							LOGSERVER->OnLogItemEx( USERDATATOUSER( pcUserData ), ITEMLOGID_RewardEasterGet, "Character[%s] Received Easter Reward ([%s][%08X] Count[%d])",
								CHARACTERSERVER->GetCharacterName(pcUserData), szItemName, pcItemData->sBaseItemID.ToItemID(), ps->iQuantity );

							//Send Message
							CHATSERVER->SendChatMessageBox( USERDATATOUSER( pcUserData ), "Check Item Distributor to get your reward!" );
							CHATSERVER->SendChat( USERDATATOUSER( pcUserData ), EChatColor::CHATCOLOR_Global, "> Check Item Distributor to get your reward!" );
						}
					}
				}
			}
			break;
	}
}

void CEventServer::HandlePacket( UserData * pcUserData, PacketNPCRudolphXmas * psPacket )
{
	if ( (psPacket->iItemID == ITEMID_GiftBox1) || (psPacket->iItemID == ITEMID_GiftBox2) )
	{
		static UINT uRewardCounter = 0;

		BOOL bContinue = TRUE;

		//Fake?
		if ( ITEMSERVER->UseItem( pcUserData, psPacket->iItemID, psPacket->dwHead, psPacket->dwCheckSum ) < 0 )
		{
			//Log
			PacketLogCheat s;
			s.iCheatID = CHEATLOGID_XmasRudolphFakeBox;
			s.LParam = psPacket->iItemID;
			s.SParam = psPacket->dwHead;
			s.EParam = psPacket->dwCheckSum;
			LOGSERVER->OnLogCheat( pcUserData, &s );
			bContinue = FALSE;
		}

		if ( bContinue )
		{
			uRewardCounter++;

			LOGSERVER->OnLogItemEx( USERDATATOUSER( pcUserData ), ITEMLOGID_RewardXmasRudolphGet, "AccountName[%s] Character[%s] Trade Xmas Rudolph Reward ID[%u] Box[%s]", pcUserData->szAccountName, CHARACTERSERVER->GetCharacterName( pcUserData ), uRewardCounter, psPacket->iItemID == ITEMID_GiftBox1 ? "Red" : "Green" );

			vXmasEventRudolph.push_back( new PrizeEventRudolphUser( uRewardCounter, (EItemID)psPacket->iItemID, pcUserData->szAccountName, CHARACTERSERVER->GetCharacterName( pcUserData ), pcUserData->pcSocketData->szIP ) );

			CHATSERVER->SendChat( USERDATATOUSER( pcUserData ), CHATCOLOR_Global, "Rudolph> Gift Box received! Wait for Santa Clause!" );
			CHATSERVER->SendChatMessageBox( USERDATATOUSER( pcUserData ), "Gift Box received! Wait for Santa Clause!" );
		}
	}
}

void CEventServer::HandlePacket( User * pcUser, PacketNPCCarnival * psPacket )
{
	if ( EVENT_CARNIVAL == FALSE )
		return;

	if ( psPacket->iCount == 6 )
	{
		if ( (psPacket->iaItemID[0] == ITEMID_CarnivalApple) && (psPacket->iaItemID[1] == ITEMID_CarnivalBanana) && (psPacket->iaItemID[2] == ITEMID_CarnivalJabuticaba) &&
			(psPacket->iaItemID[3] == ITEMID_CarnivalMaracuja) && (psPacket->iaItemID[4] == ITEMID_CarnivalPitanga) && (psPacket->iaItemID[5] == ITEMID_CarnivalStrawberry) )
		{
			bool bCanReceiveReward = true;

			for ( int i = 0; i < 6; i++ )
			{
				if ( ITEMSERVER->UseItem( pcUser, psPacket->iaItemID[i], psPacket->iaChk1[i], psPacket->iaChk2[i] ) < 0 )
				{
					//Log
					PacketLogCheat s;
					s.iCheatID = CHEATLOGID_CarnivalFakePuzzleItem;
					s.LParam = psPacket->iaItemID[i];
					s.SParam = psPacket->iaChk1[i];
					s.EParam = psPacket->iaChk2[i];
					LOGSERVER->OnLogCheat( pcUser->pcUserData, &s );

					bCanReceiveReward = false;
					break;
				}

				ItemData * pcItemData = ITEMSERVER->FindItemPointerTable( psPacket->iaItemID[i] );
				if ( pcItemData )
				{
					//Log it...
					LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_RewardEasterGet, "Character[%s] Put Carnival Puzzle ([%s][0x%08X][%d x %d])",
											CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), pcItemData->szBaseName, psPacket->iaItemID[i], psPacket->iaChk1[i], psPacket->iaChk2[i] );
				}
			}

			if ( bCanReceiveReward )
			{
				//Get Reward
				SPrizeData * ps = GetPrizeRewardCarnivalPuzzle();
				if ( ps )
				{
					char szItemName[32] = {0};

					//Find Item Name to put on Log...
					if ( ITEMSERVER->FindItemName( ps->szCode, szItemName ) )
					{
						//Find Item by Code to get ID to use on Log...
						ItemData * pcItemData = ITEMSERVER->FindItemPointerTable( ps->szCode );
						if ( pcItemData )
						{
							if ( auto psDefinitionItem = ITEMSERVER->FindItemDefByCode( ps->szCode ) )
							{
								for ( int i = 0; i < ps->iQuantity; i++ )
								{
									Item sItem;

									//Create Item
									ITEMSERVER->CreateItem( &sItem, psDefinitionItem );

									//Send To ItemBox
									ITEMSERVER->AddItemOpenBox( pcUser->pcUserData->szAccountName, &sItem );
								}
							}

							//Send Message
							CHATSERVER->SendChatMessageBox( pcUser, "Check Item Distributor to get your reward!" );
							CHATSERVER->SendChat( pcUser, EChatColor::CHATCOLOR_Global, "> Check Item Distributor to get your reward!" );

							//Log it...
							LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_RewardCarnivalPuzzleReceive, "Character[%s] Received Carnival Puzzle Reward ([%s][0x%08X] Count[%d])",
													CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), szItemName, pcItemData->sBaseItemID.ToItemID(), ps->iQuantity );

							psPacket->bDeleteItems = true;

							SENDPACKET( pcUser, psPacket );
						}
					}
				}
			}
		}
	}
}

void CEventServer::EasterEventUnitKilled( UnitData * pcUnitData )
{
	if ( pcUnitData == NULL || EVENT_EASTER == FALSE )
		return;

	if ( STRINGCOMPARE( pcUnitData->sCharacterData.szName, EVENT_MONSPAWN_EASTER ) == FALSE )
		return;

	//Drop Easter Egg
	if ( RandomI( 0, 1000 ) < 500 )
	{
		//Random Egg
		int iItemID				= ITEMID_RedEasterEgg;

		//Generate Random
		int iRand				= RandomI( 1, 3 );
		if ( iRand == 1 )
			iItemID = ITEMID_GreenEasterEgg;
		else if ( iRand == 2 )
			iItemID = ITEMID_PinkEasterEgg;

		Map::Loot::LootItem sLootItem;

		//Get Item Data
		DefinitionItem * ps		= ITEMSERVER->FindItemDefByCode( iItemID );

		if ( ps )
		{
			//Create Item with non-spec
			ITEMSERVER->CreateItem( &sLootItem.sItem, ps, 0 );

			//Get Position of Unit on Map
			int iX				= pcUnitData->sPosition.iX >> 8;
			int iY				= pcUnitData->sPosition.iY >> 8;
			int iZ				= pcUnitData->sPosition.iZ >> 8;

			//Get Map Data from Unit
			Map * pcMap = (Map*)pcUnitData->pcMap;
			
			if ( pcMap )
			{
				//Add Item to Map
				Map::Loot * psItemD = pcMap->AddItem( &sLootItem, iX, iY, iZ, TRUE );
				
				//Added? Send It to near users...
				if ( psItemD )
					MAPSERVER->SendItemStageNearUsers( pcMap, psItemD );
			}
		}
	}
}

void CEventServer::OnUnitKilled( UnitData * pcUnitData )
{
	EasterEventUnitKilled( pcUnitData );
}

SPrizeData * CEventServer::GetPrizeRewardItem()
{
	int iRand	= (rand() << 7) | ((GetCurrentTime() >> 2) & 0x7F);
	iRand		= iRand % iTotalPercent;
	
	int iTotal = 0;

	for ( const auto v : vPrizesReward )
	{
		SPrizeData * p = v;
		if ( p->bActive )
		{
			if ( (iRand >= iTotal) && (iRand < (iTotal + p->iPercent)) )
				return p;

			iTotal += p->iPercent;
		}
	}
	return vPrizesReward[0];
}

SPrizeData * CEventServer::GetPrizeXmasRewardItem()
{
	int iRand = (rand() << 7) | ((GetCurrentTime() >> 2) & 0x7F);
	iRand = iRand % iXmasTotalPercent;

	int iTotal = 0;

	for ( const auto v : vPrizesXmasReward )
	{
		SPrizeData * p = v;
		if ( p->bActive )
		{
			if ( (iRand >= iTotal) && (iRand < (iTotal + p->iPercent)) )
				return p;

			iTotal += p->iPercent;
		}
	}
	return vPrizesXmasReward[0];
}

SPrizeData * CEventServer::GetPrizeRewardItemDaily()
{
	int iRand = (rand() << 7) | ((GetCurrentTime() >> 2) & 0x7F);
	iRand = iRand % iTotalPercentDaily;

	int iTotal = 0;

	for ( const auto v : vPrizesDailyReward )
	{
		SPrizeData * p = v;
		if ( p->bActive )
		{
			if ( (iRand >= iTotal) && (iRand < (iTotal + p->iPercent)) )
				return p;

			iTotal += p->iPercent;
		}
	}
	return vPrizesDailyReward[0];
}

SPrizeData * CEventServer::GetPrizeRewardItemEaster()
{
	int iRand = (rand() << 7) | ((GetCurrentTime() >> 2) & 0x7F);
	iRand = iRand % iTotalPercentEaster;

	int iTotal = 0;

	for ( const auto v : vPrizesRewardEaster )
	{
		SPrizeData * p = v;
		if ( p->bActive )
		{
			if ( (iRand >= iTotal) && (iRand < (iTotal + p->iPercent)) )
				return p;

			iTotal += p->iPercent;
		}
	}
	return vPrizesRewardEaster[0];
}

SPrizeData * CEventServer::GetPrizeRewardCarnivalPuzzle()
{
	int iRand = (rand() << 7) | ((GetCurrentTime() >> 2) & 0x7F);
	iRand = iRand % iTotalPercentCarnivalPuzzle;

	int iTotal = 0;

	for ( const auto v : vPrizesCarnivalPuzzleReward )
	{
		SPrizeData * p = v;
		if ( p->bActive )
		{
			if ( (iRand >= iTotal) && (iRand < (iTotal + p->iPercent)) )
				return p;

			iTotal += p->iPercent;
		}
	}
	return vPrizesCarnivalPuzzleReward[0];
}

void CEventServer::SQLOnlineRewardLog( char * pszAccountName, char * pszName, char * pszItemName, int iQuantity, bool bDailyReward )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "INSERT INTO OnlineRewardLog([AccountName],[Name],[ItemName],[Quantity],[IsDailyReward],[Date]) VALUES (?,?,?,?,?,GETDATE())" ) )
		{
			BOOL bDaily = bDailyReward ? TRUE : FALSE;
			pcDB->BindParameterInput( 1, PARAMTYPE_String, pszAccountName, 32 );
			pcDB->BindParameterInput( 2, PARAMTYPE_String, pszName, 32 );
			pcDB->BindParameterInput( 3, PARAMTYPE_String, pszItemName, 32 );
			pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &iQuantity );
			pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &bDaily );
			pcDB->Execute();
		}
		pcDB->Close();
	}
}

bool CEventServer::SQLGetAccountGifts( std::vector<PrizeXmasAccount *> & vAccounts )
{
	bool bRet = false;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT AccountName, AccountIP, CharacterName FROM ChristmasTreeGift" ) )
		{
			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
					PrizeXmasAccount * ps = new PrizeXmasAccount;

					pcDB->GetData( 1, PARAMTYPE_String, ps->szAccountName, _countof( ps->szAccountName ) );
					pcDB->GetData( 2, PARAMTYPE_String, ps->szAccountIP, _countof( ps->szAccountIP ) );
					pcDB->GetData( 3, PARAMTYPE_String, ps->szCharacterName, _countof( ps->szCharacterName ) );

					vAccounts.push_back( ps );

					bRet = true;
				}
			}
		}
		pcDB->Close();
	}

	return bRet;
}

void CEventServer::SQLAddAccountGift( char * pszAccountName, char * pszAccountIP, char * pszCharacterName )
{
	if ( (pszAccountName == nullptr) || (pszAccountName[0] == 0) )
		return;

	if ( (pszAccountIP == nullptr) || (pszAccountIP[0] == 0) )
		return;

	if ( (pszCharacterName == nullptr) || (pszCharacterName[0] == 0) )
		return;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		bool bAdd = true;

		if ( pcDB->Prepare( "SELECT ID FROM ChristmasTreeGift WHERE AccountName=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, pszAccountName, STRLEN( pszAccountName ) );

			if ( pcDB->Execute() )
				if ( pcDB->Fetch() )
					bAdd = false;
		}

		if ( bAdd )
		{
			pcDB->Clear();

			if ( pcDB->Prepare( "INSERT INTO ChristmasTreeGift([AccountName],[AccountIP],[CharacterName],[Date]) VALUES(?,?,?,GETDATE())" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_String, pszAccountName, STRLEN( pszAccountName ) );
				pcDB->BindParameterInput( 2, PARAMTYPE_String, pszAccountIP, STRLEN( pszAccountIP ) );
				pcDB->BindParameterInput( 3, PARAMTYPE_String, pszCharacterName, STRLEN( pszCharacterName ) );
				pcDB->Execute();
			}
		}

		pcDB->Close();
	}
}

void CEventServer::SQLClearAccountGifts()
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "TRUNCATE TABLE ChristmasTreeGift" ) )
			pcDB->Execute();

		pcDB->Close();
	}
}

void CEventServer::SetPrizeOnlineReward()
{
	std::vector<User *>	vUsers;
	for ( UINT i = 0; i < USERSERVER->uUsersInGame; i++ )
	{
		User * pcUser = USERSERVER->pcaUserInGame[i];		
		if ( (pcUser->bNetServer == FALSE) && (pcUser->GetGameLevel() == GAMELEVEL_None) )
			vUsers.push_back( pcUser );
	}

	char szItemName[32] = { 0 };
	if ( vUsers.size() > 0 )
	{
		// Get Random and set prize
		if ( User * pcUser = vUsers[rand() % vUsers.size()] )
		{
			SPrizeData * p = GetPrizeRewardItem();
			if ( p )
			{
				ITEMSERVER->AddItemOpenBox( pcUser->pcUserData, p->szCode, 0, p->iQuantity );
				if ( ITEMSERVER->FindItemName( p->szCode, szItemName ) )
					SQLOnlineRewardLog( pcUser->pcUserData->szAccountName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), szItemName, p->iQuantity, false );
				CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "OnlineReward> %s won this time!", CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );
			}
		}
	}

	vUsers.clear();
}

void CEventServer::SetPrizeDailyReward()
{
	std::vector<User *>	vUsers;
	for ( UINT i = 0; i < USERSERVER->uUsersInGame; i++ )
	{
		User * pcUser = USERSERVER->pcaUserInGame[i];
		if ( (pcUser->bNetServer == FALSE) && (pcUser->GetGameLevel() == GAMELEVEL_None) )
			vUsers.push_back( pcUser );
	}

	if ( vUsers.size() > 0 )
	{
		if ( User * pcUser = vUsers[rand() % vUsers.size()] )
		{
			SPrizeData * p = GetPrizeRewardItemDaily();
			if ( p )
			{
				ITEMSERVER->AddItemOpenBox( pcUser->pcUserData, p->szCode, 0, p->iQuantity );

				char szItemName[32] = { 0 };
				if ( ITEMSERVER->FindItemName( p->szCode, szItemName ) )
					SQLOnlineRewardLog( pcUser->pcUserData->szAccountName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), szItemName, p->iQuantity, true );

				CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "DailyReward> %s won this time!", CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );
			}
		}
	}

	vUsers.clear();
}

void CEventServer::OnSpawnMonsterEvent( struct CharacterData * psCharacterData, Map * pcMap )
{
	if ( EVENT_WANTEDMORIPH )
	{
		if ( pcMap && pcMap->pcBaseMap )
		{
			int iPercent = EVENT_WANTEDMORIPH;
			if ( MAPSERVER->IsEXPMap( pcMap->pcBaseMap->GetID() ) )
				iPercent = EVENT_WANTEDMORIPH_EXPMAP;

			int iRand = (rand() | TICKCOUNT) % 1000;
			if ( iRand <= iPercent )
			{
				CharacterData * psEventCharacterData = UNITSERVER->GetCharacterDataByEffect( MONSTEREFFECTID_NPC_MORIF );
				if ( psEventCharacterData )
				{
					LARGE_INTEGER liEXP, liEXPEvent;
					liEXP.QuadPart		= UNITSERVER->GetExp( psCharacterData );
					liEXPEvent.QuadPart = UNITSERVER->GetExp( psEventCharacterData );

					CopyMemory( psCharacterData, psEventCharacterData, sizeof( CharacterData ) );

					if ( liEXP.QuadPart > liEXPEvent.QuadPart )
					{
						psCharacterData->iCurrentExpLow		= liEXP.LowPart;
						psCharacterData->iCurrentExpHigh	= liEXP.HighPart;
					}
				}
			}
		}
	}
	if ( EVENT_WANTEDWOLF )
	{
		if ( pcMap && pcMap->pcBaseMap )
		{
			int iPercent = EVENT_WANTEDWOLF;

			if ( MAPSERVER->IsEXPMap( pcMap->pcBaseMap->GetID() ) )
				iPercent = EVENT_WANTEDWOLF_EXPMAP;

			int iRand = (rand() | TICKCOUNT) % 1000;
			if ( iRand <= iPercent )
			{
				CharacterData * psEventCharacterData = UNITSERVER->GetCharacterDataByEffect( MONSTEREFFECTID_NPC_MOLLYWOLF );
				if ( psEventCharacterData )
				{
					LARGE_INTEGER liEXP, liEXPEvent;
					liEXP.QuadPart		= UNITSERVER->GetExp( psCharacterData );
					liEXPEvent.QuadPart = UNITSERVER->GetExp( psEventCharacterData );

					CopyMemory( psCharacterData, psEventCharacterData, sizeof( CharacterData ) );

					if ( liEXP.QuadPart > liEXPEvent.QuadPart )
					{
						psCharacterData->iCurrentExpLow		= liEXP.LowPart;
						psCharacterData->iCurrentExpHigh	= liEXP.HighPart;
					}
				}
			}
		}
	}

	if ( EVENT_EASTER )
	{
		if ( pcMap && pcMap->pcBaseMap )
		{
			int iPercent = EVENT_EASTER_RATE;
			if ( MAPSERVER->IsEXPMap( pcMap->pcBaseMap->GetID() ) )
				iPercent = EVENT_EASTER_RATEMAPEXP;

			int iRand = (rand() | TICKCOUNT) % 1000;
			if ( iRand <= iPercent )
			{
				CharacterData * psEventCharacterData = UNITSERVER->GetCharacterDataByName( EVENT_MONSPAWN_EASTER );
				if ( psEventCharacterData )
				{
					LARGE_INTEGER liEXP, liEXPEvent;
					liEXP.QuadPart		= UNITSERVER->GetExp( psCharacterData );
					liEXPEvent.QuadPart = UNITSERVER->GetExp( psEventCharacterData );

					CopyMemory( psCharacterData, psEventCharacterData, sizeof( CharacterData ) );

					if ( liEXP.QuadPart > liEXPEvent.QuadPart )
					{
						psCharacterData->iCurrentExpLow		= liEXP.LowPart;
						psCharacterData->iCurrentExpHigh	= liEXP.HighPart;
					}
				}
			}
		}

	}
}

void CEventServer::OnSayTime( UnitData * pcUnitData )
{
	EMonsterEffectID eEffectID = (EMonsterEffectID)pcUnitData->sCharacterData.iMonsterEffectID;

	if ( EVENT_WANTEDMORIPH && eEffectID == MONSTEREFFECTID_NPC_MORIF )
	{
		if ( pcUnitData->dwMessageEndTime < TICKCOUNT )
		{
			char * pszaMoriphMessage[5] =
			{
				"When I back, you will know who is the BOSS!",
				"Let me know about your family!",
				"You are so ugly, bubbly!",
				"My sheltoms!!!! NOOO",
				"You need to know the darkside!"
			};
			int iRand = rand() % 5;
			pcUnitData->dwMessageEndTime = TICKCOUNT + 60000;
			CHATSERVER->SendUserBoxChatRange( pcUnitData->sPosition.iX, pcUnitData->sPosition.iZ, 0x4B000, pcUnitData->iID, "%s: %s", pcUnitData->sCharacterData.szName, pszaMoriphMessage[iRand] );
		}
	}
}

void CEventServer::SendEvents( User * pcUser )
{
    ProcessPacket( pcUser, &sPacketEventData );
}

void CEventServer::SendEventsAll()
{
    for ( UINT i = 0; i < USERSERVER->uUsersInGame; i++ )
        SendEvents( USERSERVER->pcaUserInGame[i] );
}

void CEventServer::UpdateEventMessages()
{
    LoadEventsData();

    SendEventsAll();
}

void CEventServer::SendUpdateEventMessages()
{
    NETSERVER->SendUpdateMessageEvent();
}

void CEventServer::AddXmasPrize( User * pcUser )
{
	if ( (GetServerTime()->wMonth == 12) && ((GetServerTime()->wDay == 24) || (GetServerTime()->wDay == 25) || (GetServerTime()->wDay == 26)) )
	{
		char * pszIP = pcUser->pcUserData->pcSocketData ? pcUser->pcUserData->pcSocketData->szIP : "127.0.0.1";

		SQLAddAccountGift( pcUser->pcUserData->szAccountName, pszIP, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );
	}
}

void CEventServer::SendNPCCarnival( User * pcUser )
{
	PacketNPCCarnival sPacket;
	sPacket.iLength = sizeof( PacketNPCCarnival );
	sPacket.iHeader = PKTHDR_CarnivalNPC;
	sPacket.bDeleteItems = false;
	SENDPACKET( pcUser, &sPacket );
}

void CEventServer::LoadPrizesOnlineReward()
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT ItemCode, Quantity, [Percent] FROM OnlineReward ORDER BY [Percent] DESC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					SPrizeData * p = new SPrizeData;
					pcDB->GetData( 1, PARAMTYPE_String, p->szCode, 32 );
					pcDB->GetData( 2, PARAMTYPE_Integer, &p->iQuantity );
					pcDB->GetData( 3, PARAMTYPE_Integer, &p->iPercent );
					p->bActive = TRUE;
					iTotalPercent += p->iPercent;

					vPrizesReward.push_back( p );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT ItemCode, Quantity, [Percent] FROM DailyReward ORDER BY [Percent] DESC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					SPrizeData * p = new SPrizeData;
					pcDB->GetData( 1, PARAMTYPE_String, p->szCode, 32 );
					pcDB->GetData( 2, PARAMTYPE_Integer, &p->iQuantity );
					pcDB->GetData( 3, PARAMTYPE_Integer, &p->iPercent );
					p->bActive = TRUE;
					iTotalPercentDaily += p->iPercent;

					vPrizesDailyReward.push_back( p );
					i++;
				}
			}
		}
		pcDB->Close();
	}
}

void CEventServer::LoadPrizesXmasReward()
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT ItemCode, Quantity, [Percent] FROM ChristmasReward ORDER BY [Percent] DESC" ) )
		{
			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
					SPrizeData * p = new SPrizeData;
					pcDB->GetData( 1, PARAMTYPE_String, p->szCode, 32 );
					pcDB->GetData( 2, PARAMTYPE_Integer, &p->iQuantity );
					pcDB->GetData( 3, PARAMTYPE_Integer, &p->iPercent );
					p->bActive = TRUE;
					iXmasTotalPercent += p->iPercent;

					vPrizesXmasReward.push_back( p );
				}
			}
		}

		pcDB->Close();
	}
}

void CEventServer::LoadPrizesEasterReward()
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT ItemCode, Quantity, [Percent] FROM EasterReward ORDER BY [Percent] DESC" ) )
		{
			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
					SPrizeData * p = new SPrizeData;
					pcDB->GetData( 1, PARAMTYPE_String, p->szCode, 32 );
					pcDB->GetData( 2, PARAMTYPE_Integer, &p->iQuantity );
					pcDB->GetData( 3, PARAMTYPE_Integer, &p->iPercent );
					p->bActive = TRUE;
					iTotalPercentEaster += p->iPercent;

					vPrizesRewardEaster.push_back( p );
				}
			}
		}

		pcDB->Close();
	}
}

void CEventServer::LoadPrizesCarnivalPuzzleReward()
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT ItemCode, Quantity, [Percent] FROM CarnivalPuzzleReward ORDER BY [Percent] DESC" ) )
		{
			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
					SPrizeData * p = new SPrizeData;
					pcDB->GetData( 1, PARAMTYPE_String, p->szCode, 32 );
					pcDB->GetData( 2, PARAMTYPE_Integer, &p->iQuantity );
					pcDB->GetData( 3, PARAMTYPE_Integer, &p->iPercent );
					p->bActive = TRUE;
					iTotalPercentCarnivalPuzzle += p->iPercent;

					vPrizesCarnivalPuzzleReward.push_back( p );
				}
			}
		}

		pcDB->Close();
	}
}

void CEventServer::LoadEventsData()
{
    ZeroMemory( &sPacketEventData, sizeof( PacketEventServer ) );

    SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
    if ( pcDB->Open() )
    {
        if ( pcDB->Prepare( "SELECT MessageLeft, RedMessageColorLeft, GreenMessageColorLeft, BlueMessageColorLeft, AlphaMessageColorLeft, IsMessageLeftBold, MessageRight, RedMessageColorRight, GreenMessageColorRight, BlueMessageColorRight, AlphaMessageColorRight, IsMessageRightBold FROM EventMessage WHERE (Enabled=1) AND ((ServerID=?) OR (ServerID=0))" ) )
        {
            pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &SERVER_CODE );
            if ( pcDB->Execute() )
            {
                while ( pcDB->Fetch() )
                {
                    int i = sPacketEventData.sEventData.iEventCount;

                    int iaColorLeft[4]     = {0,0,0,0};
                    int iaColorRight[4]    = {0,0,0,0};

                    pcDB->GetData( 1, PARAMTYPE_String, sPacketEventData.sEventData.szaMessageEventLeft[i], _countof( sPacketEventData.sEventData.szaMessageEventLeft[i] ) );
                    pcDB->GetData( 2, PARAMTYPE_Integer, &iaColorLeft[1] );
                    pcDB->GetData( 3, PARAMTYPE_Integer, &iaColorLeft[2] );
                    pcDB->GetData( 4, PARAMTYPE_Integer, &iaColorLeft[3] );
                    pcDB->GetData( 5, PARAMTYPE_Integer, &iaColorLeft[0] );
                    pcDB->GetData( 6, PARAMTYPE_Integer, &sPacketEventData.sEventData.baBoldLeft[i] );
                    pcDB->GetData( 7, PARAMTYPE_String, sPacketEventData.sEventData.szaMessageEventRight[i], _countof( sPacketEventData.sEventData.szaMessageEventRight[i] ) );
                    pcDB->GetData( 8, PARAMTYPE_Integer, &iaColorRight[1] );
                    pcDB->GetData( 9, PARAMTYPE_Integer, &iaColorRight[2] );
                    pcDB->GetData( 10, PARAMTYPE_Integer, &iaColorRight[3] );
                    pcDB->GetData( 11, PARAMTYPE_Integer, &iaColorRight[0] );
                    pcDB->GetData( 12, PARAMTYPE_Integer, &sPacketEventData.sEventData.baBoldRight[i] );

                    sPacketEventData.sEventData.baTextColorLeft[i][0] = iaColorLeft[0];
                    sPacketEventData.sEventData.baTextColorLeft[i][1] = iaColorLeft[1];
                    sPacketEventData.sEventData.baTextColorLeft[i][2] = iaColorLeft[2];
                    sPacketEventData.sEventData.baTextColorLeft[i][3] = iaColorLeft[3];

                    sPacketEventData.sEventData.baTextColorRight[i][0] = iaColorRight[0];
                    sPacketEventData.sEventData.baTextColorRight[i][1] = iaColorRight[1];
                    sPacketEventData.sEventData.baTextColorRight[i][2] = iaColorRight[2];
                    sPacketEventData.sEventData.baTextColorRight[i][3] = iaColorRight[3];

                    sPacketEventData.sEventData.iEventCount++;

                    if ( sPacketEventData.sEventData.iEventCount == MAX_EVENTSERVER_TEXT )
                        break;
                }
            }
        }
        pcDB->Close();
    }
}

void CEventServer::ProcessPacket( User * pcUser, PacketEventServer * psPacket )
{
    psPacket->iLength = sizeof( PacketEventServer );
    psPacket->iHeader = PKTHDR_EventsData;
    SENDPACKET( pcUser, psPacket );
}



void CEventServer::Tick()
{
	if ( LOGIN_SERVER )
	{
		if ( wLastHourOnlineReward == 26 )
		{
			// Set Prizes
			LoadPrizesOnlineReward();
			LoadPrizesXmasReward();
			LoadPrizesEasterReward();
			LoadPrizesCarnivalPuzzleReward();
            LoadEventsData();
			wLastHourOnlineReward	= GetServerTime()->wHour;
			wNextHourDailyReward = (GetServerTime()->wHour + 1) % 24;
			bDeliveredDailyReward = false;
		}

		if ( bDeliveredDailyReward == false )
		{
			if ( wNextHourDailyReward == GetServerTime()->wHour )
			{
				SetPrizeDailyReward();
		
				bDeliveredDailyReward = true;
			}
		}
		else
		{
			if ( GetServerTime()->wHour == 0 )
			{
				wNextHourDailyReward = (GetServerTime()->wHour + 1) % 24;

				bDeliveredDailyReward = false;
			}
		}

		if ( wLastHourOnlineReward != GetServerTime()->wHour )
		{
			SetPrizeOnlineReward();
			wLastHourOnlineReward = GetServerTime()->wHour;
		}

		if ( (wLastHourXmas != GetServerTime()->wHour) || (bResetRudolph == TRUE) )
		{
			GiveRudolphPrizes();

			wLastHourXmas = GetServerTime()->wHour;

			if ( (GetServerTime()->wMonth == 12) && ((GetServerTime()->wDay == 24) || (GetServerTime()->wDay == 25) || (GetServerTime()->wDay == 26)) )
				if ( (wLastHourXmas == 0) || (wLastHourXmas == 6) || (wLastHourXmas == 12) || (wLastHourXmas == 18) )
					GiveFriendGifts();

			bResetRudolph = FALSE;
		}
	}
}

void CEventServer::SetDailyReward( int iHour )
{
	wNextHourDailyReward = iHour;
	
	bDeliveredDailyReward = false;
}
