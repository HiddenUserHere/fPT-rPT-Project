#include "stdafx.h"
#include "CRollDiceHandler.h"

CRollDiceHandler::CRollDiceHandler()
{
	for ( int i = 0; i < 200; i++ )
	{
		vListMap.push_back( new RollDiceMap( i ) );
	}
}

CRollDiceHandler::~CRollDiceHandler()
{
}

void CRollDiceHandler::ProcessPacket( User * pcUser, PacketRollDiceDrop * psPacket )
{
	psPacket->iHeader = PKTHDR_RollDiceDrop;
	psPacket->iLength = sizeof(PacketRollDiceDrop);

	SENDPACKET( pcUser, psPacket, TRUE );
}

void CRollDiceHandler::ProcessPacket( User * pcUser, PacketRollDiceRanking * psPacket )
{
	psPacket->iHeader = PKTHDR_RollDiceRanking;
	psPacket->iLength = sizeof(PacketRollDiceRanking);

	SENDPACKET( pcUser, psPacket, TRUE );
}

void CRollDiceHandler::HandlePacket( User * pcUser, PacketRollDiceDrop * psPacket )
{
	if( !pcUser || !psPacket )
		return;

	if ( (psPacket->iID == pcUser->GetID()) && ((pcUser->GetMapID() >= MAPID_AcasiaForest) && (pcUser->GetMapID() < (int)vListMap.size())) )
	{
		std::vector<RollDiceDrop*> & vItemDrops = vListMap[pcUser->GetMapID()]->vListDrop;

		//Check if have item to roll
		if( vItemDrops.size() > 0 )
		{
			RollDiceDrop * pcItemDrop = vItemDrops[vItemDrops.size()-1];

			//Item drop valid for usage?
			if( (pcItemDrop) && pcItemDrop->dwEndTime > TICKCOUNT && pcItemDrop->vListUsers.size() > 0 && pcItemDrop->psLoot->sLootItem.sItem.sItemID.ToItemID() != ITEMID_None )
			{
				int iNumber = 1;
				for( auto &pcRollUser : pcItemDrop->vListUsers )
				{
					//Set Number by Index
					if( pcRollUser->iID == psPacket->iID && pcRollUser->iRandomNumber == 0 && ((pcRollUser->pcUser == pcUser) && (pcRollUser->iID == pcUser->GetID())) )
					{
						pcRollUser->iRandomNumber = psPacket->bAcceptItem ? ((iNumber * 5) + RandomI( 1, 900 ) + iNumber) : 0;

						if ( psPacket->bAcceptItem )
							CHATSERVER->SendChatEx( pcRollUser->pcUser, CHATCOLOR_Global, "Roll> You rolled #%d, Good Luck!", pcRollUser->iRandomNumber );

						//Prepare Packet of Ranking to others players
						PacketRollDiceRanking sPacket;
						ZeroMemory( &sPacket, sizeof( PacketRollDiceRanking ) );

						STRINGCOPY( sPacket.szCharacterName, CHARACTERSERVER->GetCharacterName( pcRollUser->pcUser->pcUserData ) );
						sPacket.uNumber = pcRollUser->iRandomNumber;

						//Send for all players of dice roll
						for( auto &pcOtherRollUser : pcItemDrop->vListUsers )
							ProcessPacket( pcOtherRollUser->pcUser, &sPacket );

						break;
					}
					iNumber++;
				}
			}

			if ( psPacket->bAcceptItem == FALSE )
			{
				for ( auto &pcRollUser : pcItemDrop->vListUsers )
					CHATSERVER->SendChatEx( pcRollUser->pcUser, CHATCOLOR_Global, "Roll> %s PASS!", CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );
			}
		}
	}
}

void CRollDiceHandler::Tick()
{	
	for ( auto & vMap : vListMap )
	{
		std::vector<RollDiceDrop*> & vItemDrops = vMap->vListDrop;

		//Have item to roll?
		if ( vItemDrops.size() > 0 )
		{
			RollDiceDrop * pcItemDrop = vItemDrops[vItemDrops.size() - 1];

			//Item drop valid for usage?
			if ( (pcItemDrop) && pcItemDrop->vListUsers.size() > 0 && pcItemDrop->psLoot->sLootItem.sItem.sItemID.ToItemID() != ITEMID_None )
			{
				//if dwEndTime is -1, so will send the roll dice to all users in the array
				if ( pcItemDrop->dwEndTime == -1 )
				{
					pcItemDrop->dwEndTime = TICKCOUNT + (MAX_ROLLDICEDROP_TIME * 1000);

					PacketRollDiceDrop sPacket;
					sPacket.eItemID = pcItemDrop->psLoot->sLootItem.sItem.sItemID.ToItemID();
					sPacket.bAcceptItem = FALSE;

					//Packet for user confirmation box
					for ( auto &pcRollUser : pcItemDrop->vListUsers )
					{
						if ( pcRollUser->iID && pcRollUser->iRandomNumber == -1 && (pcRollUser->pcUser && (pcRollUser->pcUser->GetID() == pcRollUser->iID)) &&
							 (pcItemDrop->iMapID == pcRollUser->pcUser->GetMapID()) )
						{
							sPacket.iID = pcRollUser->pcUser->GetID();

							pcRollUser->iRandomNumber = 0;

							ProcessPacket( pcRollUser->pcUser, &sPacket );
						}
					}
				}
				else if ( TICKCOUNT > pcItemDrop->dwEndTime )
				{
					int iLastNumber = -1;
					User * pcWinnerUser = NULL;

					//Check for the Random number
					for ( const auto &pcRollUser : pcItemDrop->vListUsers )
					{
						if ( pcRollUser->iID && pcRollUser->iRandomNumber > 0 && pcRollUser->pcUser )
						{
							if ( pcRollUser->iRandomNumber > iLastNumber )
							{
								iLastNumber = pcRollUser->iRandomNumber;
								pcWinnerUser = pcRollUser->pcUser;
							}
						}
					}

					//Have a Winner? so send the item and delete loot from the map...
					if ( iLastNumber > 0 && pcWinnerUser )
					{
						ItemData * psItemData = ITEMSERVER->FindItemPointerTable( pcItemDrop->psLoot->sLootItem.sItem.sItemID.ToItemID() );
						if ( psItemData )
						{
							DefinitionItem * psDefItem = ITEMSERVER->FindItemDefByCode( psItemData->sBaseItemID.ToItemID() );
							if ( psDefItem )
							{
								//Send item to Distributor
								if( ITEMSERVER->AddItemOpenBox( pcWinnerUser->pcUserData, psItemData->szInventoryName, 0 ) )
									CHATSERVER->SendChat( pcWinnerUser, CHATCOLOR_Error, "> Check Item Distributor to get your item!" );

								//Chat Alert
								if ( pcItemDrop->vListUsers.size() > 1 )
									CHATSERVER->SendChatAllUsersInStage( pcWinnerUser->GetMapID(), CHATCOLOR_Global, "Roll> %s #%d won %s from %d players!", CHARACTERSERVER->GetCharacterName( pcWinnerUser->pcUserData ), iLastNumber, psDefItem->sItem.szItemName, pcItemDrop->vListUsers.size() );
								else
									CHATSERVER->SendChatAllUsersInStage( pcWinnerUser->GetMapID(), CHATCOLOR_Global, "Roll> %s #%d won %s!", CHARACTERSERVER->GetCharacterName( pcWinnerUser->pcUserData ), iLastNumber, psDefItem->sItem.szItemName );

								//Log of send item
								LOGSERVER->OnLogItemEx( pcWinnerUser, ITEMLOGID_RollDiceWon, "Character[%s] Won Roll Dice MapID[%d] ItemName[%s] TotalUsers[%d] RollNumber[%d]",
									CHARACTERSERVER->GetCharacterName( pcWinnerUser->pcUserData ), pcWinnerUser->GetMapID(), psDefItem->sItem.szItemName, pcItemDrop->vListUsers.size(), iLastNumber );

								SENDPACKETBLANK( pcWinnerUser, PKTHDR_PacketFun );
							}
						}

						//Delete Item from Map
						Map * pcMap = MAPSDATA + pcItemDrop->iMapID;
						if ( pcMap )
							pcMap->DeleteItem( pcItemDrop->psLoot );

						//Delete Item Drop from Handler
						DeleteItemDrop( pcItemDrop, pcItemDrop->iMapID );
					}
					else
					{
						pcItemDrop->psLoot->dwDropTime			= TICKCOUNT + 1000;
						pcItemDrop->psLoot->dwEndTime			= TICKCOUNT + (3 * 60 * 1000);
						pcItemDrop->psLoot->bExclusive			= FALSE;
						pcItemDrop->psLoot->lExclusiveOwnerID	= 0;

						//Delete Item Drop from Handler
						DeleteItemDrop( pcItemDrop, pcItemDrop->iMapID );
					}
				}
			}
		}
	}
}

BOOL CRollDiceHandler::OnExchangeItemDrop( UnitData * pcUnitData, Map * pcMap, Map::Loot * psLoot, int iDropIndex )
{
	BOOL bRet = FALSE;

	//Unit Boss?
	if ( pcUnitData->sCharacterData.sGlow == 1 && (psLoot->sLootItem.sItem.sItemID.ToItemType() != ITEMTYPE_GoldAndExp && psLoot->sLootItem.sItem.sItemID.ToItemBase() != ITEMBASE_Potion) )
	{
		int iDamageNeeded = (UNITSERVER->GetTotalHP( pcUnitData ) * ROLLDICE_DAMAGE_PERCENT) / 100;

		std::vector<RollDiceUser*> v;

		if ( pcUnitData->psaDamageUsersData )
		{
			for ( int i = 0; i < PLAYERS_MAX; i++ )
			{
				UserData * ud = USERSDATA + i;
				User * pcUser = USERDATATOUSER( ud );

				AttackDamageData * psDamageData = NULL;

				//User is In-game and on Unit Users List?
				if ( pcUser->IsConnected() && pcUser->IsInGame() && (psDamageData = UNITSERVER->GetUserInDamageList( pcUnitData, pcUser )) )
				{
					//Compute distance beetween User and Unit
					int iX = (ud->sPosition.iX - pcUnitData->sPosition.iX) >> 8;
					int iZ = (ud->sPosition.iX - pcUnitData->sPosition.iX) >> 8;
					iX *= iX;
					iZ *= iZ;
					int iDistance = iX + iZ;
					if ( iDistance < DISTANCE_UNIT_FAR )
					{
						//User damage are >= % needed?
						if ( (psDamageData->iDamage >= iDamageNeeded) || (psDamageData->iCounter >= ROLLDICE_DAMAGE_COUNT) )
						{
							//Check if have same Hardware...
							BOOL bAllowedUser = TRUE;
							UINT uUsersInGame = USERSERVER->uUsersInGame;
							User ** pcaUserInGame = USERSERVER->pcaUserInGame;
							int iCount = 0;
							for ( auto pc : v )
							{
								if ( pcUser != pc->pcUser )
								{
									if ( pc->pcUser->szHardwareID[0] != 0 )
									{
										if ( STRINGCOMPARE( pc->pcUser->szHardwareID, pcUser->szHardwareID ) )
											iCount++;

										if ( iCount >= 1 )
										{
											bAllowedUser = FALSE;
											break;
										}
									}
								}
							}

							//Add User to Roll Dice...
							if ( bAllowedUser )
							{
								v.push_back( new RollDiceUser( pcUser->GetID(), pcUser ) );

								bRet = TRUE;

								//Log it...
								LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_RollDiceJoin, "Character[%s] Join on Roll Dice MapID[%d]",
									CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), pcUser->GetMapID() );

								HandleQuest( pcUser, pcUnitData );
							}
						}
					}
				}
			}
		}

		//Add Drop...
		if ( bRet )
		{
			AddItemDrop( new RollDiceDrop( v, psLoot, pcMap->pcBaseMap->iMapID, pcUnitData->iID ), pcMap->pcBaseMap->iMapID );
			psLoot->dwDropTime = TICKCOUNT + (iDropIndex * 60000);
			psLoot->dwEndTime += (iDropIndex * 60000);
			psLoot->bExclusive = TRUE;
			psLoot->lExclusiveOwnerID = 10;
		}
	}


	return bRet;
}

void CRollDiceHandler::DeleteItemDrop( RollDiceDrop * p, int iMapID )
{
	std::vector<RollDiceDrop*> & vItemDrops = vListMap[iMapID]->vListDrop;

	for( std::vector<RollDiceDrop*>::iterator it = vItemDrops.begin(); it != vItemDrops.end(); )
	{
		RollDiceDrop * pcItemDrop = (*it);
		if( pcItemDrop == p )
		{
			//Delete Users
			for ( std::vector<RollDiceUser*>::iterator it = pcItemDrop->vListUsers.begin(); it != pcItemDrop->vListUsers.end(); it++ )
			{
				RollDiceUser * pcRollUser = (*it);
				DELET( pcRollUser );
			}
		
			//Clear User List
			pcItemDrop->vListUsers.clear();

			//Delete drop
			DELET( pcItemDrop );

			it = vItemDrops.erase( it );
		}
		else
			it++;
	}
}

void CRollDiceHandler::HandleQuest( User * pcUser, UnitData * pcUnitData )
{
	if ( pcUnitData->pcKillerData == pcUser->pcUserData )
		return;

	if ( pcUser->bParty )
	{
		auto vUsers = PARTYHANDLER->GetPartyMembers( pcUser->psParty );

		bool bFound = false;
		for ( auto pc : vUsers )
		{
			if ( pc->pcUserData == pcUnitData->pcKillerData )
			{
				bFound = true;
				break;
			}
		}

		vUsers.clear();

		if ( bFound )
			return;
	}

	if ( pcUnitData->pcMap && pcUnitData->pcMap->pcBaseMap )
	{
		bool bSend = false;

		if ( QUESTSERVER->IsActiveQuest( pcUser, QUESTID_TheDemonAngel ) && (pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_DEVILSHY) )
			bSend = true;
		if ( QUESTSERVER->IsActiveQuest( pcUser, QUESTID_TheMineCommander ) && (pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_TULLA) )
			bSend = true;
        if ( QUESTSERVER->IsActiveQuest( pcUser, QUESTID_Retrievingbackthelight ) && (pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_Draxos) )
            bSend = true;
		if ( QUESTSERVER->IsActiveQuest( pcUser, QUESTID_PelgiaQuest ) && (pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_Pelgia) )
			bSend = true;
		if ( QUESTSERVER->IsActiveQuest( pcUser, QUESTID_Pumpking ) && STRINGCOMPARE( "King Pumpkin Ghost", pcUnitData->sCharacterData.szName ) )
			bSend = true;
		if ( QUESTSERVER->IsActiveQuest( pcUser, QUESTID_FlamoQuest ) && (pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_Flamo) )
			bSend = true;
		if ( QUESTSERVER->IsActiveQuest( pcUser, QUESTID_CommanderSaxhleel ) && (pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_LizardSoldier) )
			bSend = true;
		if ( QUESTSERVER->IsActiveQuest( pcUser, QUESTID_VestigesOfAncientWar ) && (pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_AncientGuardian) )
			bSend = true;
		if ( QUESTSERVER->IsActiveQuest( pcUser, QUESTID_TheGreedy ) && (pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_Greedy) )
			bSend = true;
		if ( QUESTSERVER->IsActiveQuest( pcUser, QUESTID_LordSantaGoblin ) && STRINGCOMPARE( "Lord Santa Goblin", pcUnitData->sCharacterData.szName ) )
			bSend = true;
		if ( QUESTSERVER->IsActiveQuest( pcUser, QUESTID_LordSantaSkeleton ) && STRINGCOMPARE( "Lord Santa Skeleton", pcUnitData->sCharacterData.szName ) )
			bSend = true;
		if ( QUESTSERVER->IsActiveQuest( pcUser, QUESTID_TheStoneGod ) && STRINGCOMPARE( "Volcanos", pcUnitData->sCharacterData.szName ) )
			bSend = true;
		if ( QUESTSERVER->IsActiveQuest(pcUser, QUESTID_SeeknDestroyYagditha ) && STRINGCOMPARE("Yagditha", pcUnitData->sCharacterData.szName ) )
			bSend = true;
		if ( QUESTSERVER->IsActiveQuest( pcUser, QUESTID_TheReturnOfTheLizards ) && STRINGCOMPARE( "Lizard Commander", pcUnitData->sCharacterData.szName) )
			bSend = true;
		if ( QUESTSERVER->IsActiveQuest( pcUser, QUESTID_TheunknownplanofHagios ) && STRINGCOMPARE( "Mini Hagios", pcUnitData->sCharacterData.szName ) )
			bSend = true;
		if ( QUESTSERVER->IsActiveQuest( pcUser, QUESTID_TheEasterChef ) && STRINGCOMPARE( "King Valdo", pcUnitData->sCharacterData.szName ) )
			bSend = true;

		if ( bSend )
			UNITSERVER->ProcessPacketKillUnitID( pcUser, pcUnitData->iID, pcUnitData->pcMap->pcBaseMap->iMapID );
	}
}
