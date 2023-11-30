#include "stdafx.h"
#include "CFollowUnitHandler.h"

struct SUnitFollowJokeMessage
{
	char			szName[32];
	char			szMessage[128];
};

static const SUnitFollowJokeMessage szaFollowMessagesUnit[] =
{
	{"4002-8922", "Congratulations, you've won The Game of Life!"},
	{"Thor", "I love GM-Thor *_*"},
	{"GM-Thor", "I love GM-Thor *_*"},
	{"GM-BigThor", "I love GM-Thor *_*"},
	{"Prog", "Prog be praised!"},
	{"GM-Prog", "Prog be praised!"},
	{"GM-BigProg", "Prog be praised!"},
	{"Bran", "I'm the king!"},
	{"GM-Bran", "I'm the king!"},
	{"BranTheBroken", "I'm the king!"},
	{"Zaryad", "Let's make a big monster!"},
	{"GM-Zaryad", "I love Big Willy!"},
	{"GM-BigZaryad", "Short Maintenance!"},
	{"Bob", "Aham prog"},
	{"Jamal", "I miss Latika!"},
	{"BigTowerGolem", "Hurr durr, durr... Ima F1 kar"},
	{"ThomasTheTrain", "Choo Choo!..."},
};


CFollowUnitHandler::CFollowUnitHandler()
{
}


CFollowUnitHandler::~CFollowUnitHandler()
{
}

bool CFollowUnitHandler::CanEnterOnMap( EMapID iMapID )
{
	if ( (iMapID == MAPID_Bellatra) || (iMapID == MAPID_RoyalDesert) || (iMapID == MAPID_OldRuinen2) || (iMapID == MAPID_T5QuestArena) || (iMapID == MAPID_QuestArena) || (iMapID == MAPID_GhostCastle) ||
		 (iMapID == MAPID_DistortedForest) || (iMapID == MAPID_ForestDungeon) || (iMapID == MAPID_DragonsDungeon) || (iMapID == MAPID_OasisRoyale) )
		return false;

	return true;
}

bool CFollowUnitHandler::CanWalkOnMap( EMapID iMapID )
{
	if ( (iMapID == MAPID_Bellatra) || (iMapID == MAPID_QuestArena) || (iMapID == MAPID_BlessCastle) || (iMapID == MAPID_GhostCastle) ||
		(iMapID == MAPID_T5QuestArena) || (iMapID == MAPID_DistortedForest) || (iMapID == MAPID_ForestDungeon) || (iMapID == MAPID_DragonsDungeon) )
		return false;

	return true;
}

bool CFollowUnitHandler::SpawnUnit( User * pcUser, const char * pszUnitName )
{
	static char * pszMessage = "At your service! ^^";

	bool bRet = false;

    Map * pcMap = MAPSERVER->GetMap( pcUser->GetMapID() == MAPID_AcasiaForest ? MAPID_BambooForest : MAPID_AcasiaForest );
	if ( pcMap->pcStage )
	{
		CharacterData * psCharacterData = UNITSERVER->GetCharacterDataByName( pszUnitName );
		if ( psCharacterData )
		{
			MonsterMapSpawnHandle sMonsterMap;
			sMonsterMap.bFixed = FALSE;
			CopyMemory( &sMonsterMap.sCharacterData, psCharacterData, sizeof( CharacterData ) );
			sMonsterMap.iPositionState = 1;
			sMonsterMap.sCharacterData.iType = ECharacterType::CHARACTERTYPE_Monster;

			pcMap->bForceCreateMonster = TRUE;

			UnitData * pcUnitData = MAPSERVER->CreateUnit( pcMap, &sMonsterMap, 0 );
			if ( pcUnitData )
			{
				MAPSERVER->DelMonsterSpawn( pcUnitData->pcMap, pcUnitData );
				pcMap->iNumAliveUnitData--;

				STRINGCOPY( pcUnitData->sCharacterData.szName, SQLGetCaravanName( pcUser ).c_str() );

				pcUnitData->sCharacterData.iType = CHARACTERTYPE_NPC;

				pcUnitData->pcOwner = pcUser->pcUserData;

				pcUnitData->dwLastUpdateTime = 0;

				pcUnitData->sUnitInfo.iSmelting = NPCEVENTCODE_CaravanNPC;

				Unit * pcUnit = UNITDATATOUNIT( pcUnitData );
				pcUnit->bFollowUnitType = true;
				pcUnit->bCaravanFollow = true;

				pcUnitData->Animate( ANIMATIONTYPE_Idle );

				STRINGCOPY( pcUnitData->szMessage, pszMessage );
				pcUnitData->sUnitInfo.iNumMessage = 0;

				for ( auto & sMessage : szaFollowMessagesUnit )
				{
					if ( STRINGCOMPARE( sMessage.szName, pcUnitData->sCharacterData.szName ) )
					{
						STRINGCOPY( pcUnitData->szMessage, sMessage.szMessage );
						break;
					}
				}

				pcUnitData->sCharacterData.cNewLoad++;

				pcUser->pcCaravan = pcUnit;

				SQLNewCaravan( pcUser );

                UpdateMap( pcUser->pcCaravan, pcUser->GetMapID(), pcUser->pcUserData->sPosition );

				bRet = true;
			}
		}
	}

	return bRet;
}

bool CFollowUnitHandler::IsFollowUnitType( Unit * pcUnit )
{
	return pcUnit->bFollowUnitType;
}

void CFollowUnitHandler::UpdateMap( Unit * pcUnit, EMapID iMapID, Point3D sPosition )
{
	if ( pcUnit->bCaravanFollow == false )
		return;

	if ( CanEnterOnMap( iMapID ) == false )
		return;

	if ( pcUnit->pcUnitData->pcMap->pcBaseMap && (pcUnit->pcUnitData->pcMap->pcBaseMap->iMapID != iMapID) )
	{
/*		if ( pcUnit->pcUnitData->pcMap->iNumAliveUnitData > 0 )
		{
			//Decrease Monster Counter on Map and delete from Spawn Flag
			pcUnit->pcUnitData->pcMap->iNumAliveUnitData--;
			MAPSERVER->DelMonsterSpawn( pcUnit->pcUnitData->pcMap, pcUnit->pcUnitData );
		}
*/
		if ( auto pcNewMap = MAPSERVER->GetMap( iMapID ) )
		{
			if ( auto pcNewStage = MAPSERVER->GetMap( iMapID )->pcStage )
			{
				pcUnit->pcUnitData->pcMap = pcNewMap;
				pcUnit->pcUnitData->pcStage = pcNewStage;

				sPosition.iY = pcUnit->pcUnitData->pcStage->GetHighestPoint( sPosition.iX, sPosition.iZ );
				pcUnit->pcUnitData->sPosition = sPosition;

				pcUnit->pcUnitData->Animate( ANIMATIONTYPE_Idle );

				//		pcUnit->pcUnitData->pcMap->AddUnitData( pcUnit->pcUnitData );

				pcUnit->pcUnitData->sCharacterData.sHP.sCur = pcUnit->pcUnitData->sCharacterData.sHP.sMax;
			}
		}
	}
}

void CFollowUnitHandler::OpenCaravan( User * pcUser )
{
	if ( pcUser->pcCaravanBase == NULL )
	{
		CaravanData * psCaravanData = new CaravanData();
		ZeroMemory( psCaravanData, sizeof( CaravanData ) );

		int iID = -1;

		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "SELECT ID, ItemsCount FROM CharacterCaravan WHERE CharacterID=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );
				if ( pcDB->Execute() && pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &iID );
					pcDB->GetData( 2, PARAMTYPE_Integer, &psCaravanData->iItemCount );
				}
			}
			pcDB->Close();
		}

		if ( iID >= 0 )
		{
			SQLGetCaravanItems( psCaravanData, iID );
			SendCaravan( pcUser, psCaravanData );
		}

		DELET( psCaravanData );
	}
}

void CFollowUnitHandler::SendOpenCaravan( User * pcUser, Unit * pcUnit )
{
	if ( pcUser->pcCaravan == pcUnit )
	{
		SENDPACKETBLANK( pcUser, PKTHDR_NPCCaravanOpen, TRUE );

		if ( pcUser->pcCaravan )
		{
			PacketCaravanUpdate sPacket;
			sPacket.bFollow = pcUser->pcCaravan->bCaravanFollow;
			STRINGCOPY( sPacket.szName, pcUser->pcCaravan->GetName() );
			ProcessPacket( pcUser, &sPacket );
		}
	}
}

void CFollowUnitHandler::HandlePacket( User * pcUser, PacketCaravanOpen * psPacket )
{
	OpenCaravan( pcUser );
}

void CFollowUnitHandler::HandlePacket( User * pcUser, PacketCaravan * psPacket )
{
	if ( pcUser->pcCaravanBase == NULL )
	{
		pcUser->pcCaravanBase = new CaravanBase();
		pcUser->pcCaravanBase->uBufferCount = 0;
	}

	Receive( pcUser, psPacket );

	if ( psPacket->IsOnly() )
	{
		pcUser->pcCaravanBase->psCaravanData = new CaravanData;

		if ( psPacket->iItemCount > MAX_CARAVAN_ITEMS )
			psPacket->iItemCount = 0;

		pcUser->pcCaravanBase->psCaravanData->iItemCount = psPacket->iItemCount;

		SaveCaravan( pcUser );

		DELET( pcUser->pcCaravanBase->psCaravanData );
		DELET( pcUser->pcCaravanBase );
	}
}

void CFollowUnitHandler::HandlePacket( User * pcUser, PacketCaravanUpdate * psPacket )
{
	if ( pcUser->pcCaravan )
	{
		if ( STRINGCOMPARE( pcUser->pcCaravan->pcUnitData->sCharacterData.szName, psPacket->szName ) == FALSE )
		{
			if ( psPacket->szName[0] == 0 )
				STRINGCOPY( psPacket->szName, "Caravan" );

			STRINGCOPY( pcUser->pcCaravan->pcUnitData->sCharacterData.szName, psPacket->szName );

			for ( auto & sMessage : szaFollowMessagesUnit )
			{
				if ( STRINGCOMPARE( sMessage.szName, pcUser->pcCaravan->pcUnitData->sCharacterData.szName ) )
				{
					STRINGCOPY( pcUser->pcCaravan->pcUnitData->szMessage, sMessage.szMessage );
					break;
				}
			}

			SQLUpdateCaravanName( pcUser );

			pcUser->pcCaravan->pcUnitData->sCharacterData.cNewLoad++;
		}

		pcUser->pcCaravan->bCaravanFollow = psPacket->bFollow;
		if ( pcUser->pcCaravan->bCaravanFollow == false )
		{
			pcUser->pcCaravan->pcUnitData->Animate( ANIMATIONTYPE_Idle );
		}
	}
}

void CFollowUnitHandler::OnDisconnected( User * pcUser )
{
	if ( pcUser->pcCaravan )
	{
		pcUser->pcCaravan->pcUnitData->Free();

		pcUser->pcCaravan = nullptr;
	}
}

void CFollowUnitHandler::OnItemTimerEnd( User * pcUser )
{
	if ( pcUser->pcCaravan )
	{
		CaravanData * psCaravanData = new CaravanData();
		ZeroMemory( psCaravanData, sizeof( CaravanData ) );

		int iID = -1;

		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "SELECT ID, ItemsCount FROM CharacterCaravan WHERE CharacterID=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );
				if ( pcDB->Execute() && pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &iID );
					pcDB->GetData( 2, PARAMTYPE_Integer, &psCaravanData->iItemCount );
				}
			}
			pcDB->Close();
		}

		if ( iID >= 0 )
		{
			SQLGetCaravanItems( psCaravanData, iID );

			for ( int i = 0; i < psCaravanData->iItemCount; i++ )
				ITEMSERVER->AddItemOpenBox( pcUser->pcUserData->szAccountName, &psCaravanData->caItemData[i].sItem, "Caravan" );

			SQLDeleteCaravanItems( pcUser );
		}

		DELET( psCaravanData );

		CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "> Caravan time has ended!" );
		CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "> Items has been sent to distributor!" );

		pcUser->pcCaravan->pcUnitData->Free();

		pcUser->pcCaravan = nullptr;
	}
}

void CFollowUnitHandler::Receive( User * pcUser, PacketCaravan * psPacket )
{
	if ( psPacket->uBufferCount <= MAX_CARAVAN_BUFFERSIZE )
	{
		if ( (pcUser->pcCaravanBase->uBufferCount + psPacket->uBufferCount) <= MAX_CARAVAN_DATASIZE )
		{
			//First?
			if ( psPacket->IsFirst() )
			{
				ZeroMemory( pcUser->pcCaravanBase->baBuffer, MAX_CARAVAN_DATASIZE );
				pcUser->pcCaravanBase->uBufferCount = 0;
			}

			CopyMemory( pcUser->pcCaravanBase->baBuffer + pcUser->pcCaravanBase->uBufferCount, psPacket->baBuffer, psPacket->uBufferCount );
			pcUser->pcCaravanBase->uBufferCount += psPacket->uBufferCount;
		}
	}
}

void CFollowUnitHandler::SaveCaravan( User * pcUser )
{
	int iLength = ITEMSERVER->DecodeItemsData( pcUser->pcCaravanBase->baBuffer, pcUser->pcCaravanBase->psCaravanData->caItemData, sizeof( ItemData ) * MAX_CARAVAN_ITEMS );
	if ( iLength >= 0 )
	{
		if ( UpdateCaravanItems( pcUser ) )
		{
			if ( SQLHaveCaravan( pcUser ) )
			{
				SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
				if ( pcDB->Open() )
				{
					if ( pcDB->Prepare( "UPDATE CharacterCaravan SET ItemsCount=?, Date=GETDATE() WHERE CharacterID=?" ) )
					{
						pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->pcCaravanBase->psCaravanData->iItemCount );
						pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &pcUser->iCharacterID );
						pcDB->Execute();
					}

					int iID = -1;

					pcDB->Clear();
					if ( pcDB->Prepare( "SELECT ID FROM CharacterCaravan WHERE CharacterID=?" ) )
					{
						pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );
						if ( pcDB->Execute() && pcDB->Fetch() )
							pcDB->GetData( 1, PARAMTYPE_Integer, &iID );
					}

					if ( iID >= 0 )
					{
						pcDB->Clear();
						if ( pcDB->Prepare( "DELETE FROM CharacterCaravanItem WHERE CaravanID=?" ) )
						{
							pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iID );
							pcDB->Execute();
						}

						for ( int i = 0; i < pcUser->pcCaravanBase->psCaravanData->iItemCount; i++ )
						{
							ItemData * pcItemData = pcUser->pcCaravanBase->psCaravanData->caItemData + i;

							pcDB->Clear();
							if ( pcDB->Prepare( "INSERT INTO CharacterCaravanItem([CaravanID],[ItemID],[Code1],[Code2],[Item]) VALUES(?,?,?,?,?)" ) )
							{
								int iItemID = pcItemData->sItem.sItemID.ToInt();
								pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iID );
								pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iItemID );
								pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &pcItemData->sItem.iChk1 );
								pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &pcItemData->sItem.iChk2 );
								pcDB->BindParameterInput( 5, PARAMTYPE_Binary, pcItemData, sizeof( ItemData ) );
								pcDB->Execute();
							}
						}
					}

					pcDB->Close();
				}
			}
		}
	}
}

void CFollowUnitHandler::SendCaravan( User * pcUser, CaravanData * psCaravanData )
{
	BYTE * baData = new BYTE[sizeof( ItemData ) * _countof( psCaravanData->caItemData )];
	ZeroMemory( baData, sizeof( ItemData ) * _countof( psCaravanData->caItemData ) );
	
	int iLength = ITEMSERVER->EncodeItemsData( psCaravanData->caItemData, baData, sizeof( ItemData ) * _countof( psCaravanData->caItemData ) );
	if ( iLength >= 0 )
	{
		//Save Items List
		pcUser->vCaravanItems.clear();
		for ( int i = 0; i < psCaravanData->iItemCount; i++ )
		{
			DropItemData sDrop;
			sDrop.iItemID	= psCaravanData->caItemData[i].sItem.sItemID.ToInt();
			sDrop.iChk1		= psCaravanData->caItemData[i].sItem.iChk1;
			sDrop.iChk2		= psCaravanData->caItemData[i].sItem.iChk2;

			pcUser->vCaravanItems.push_back( sDrop );
		}

		PacketCaravan sPacket;
		std::vector<PacketCaravan> vPackets;

		sPacket.sPacketCount.sMin = 1;

		sPacket.iItemCount = psCaravanData->iItemCount;

		UINT uBufferPosition = 0;
		if ( iLength > 0 )
		{
			while ( uBufferPosition < (UINT)iLength )
			{
				sPacket.uBufferCount = (int)(uBufferPosition + MAX_CARAVAN_BUFFERSIZE) <= iLength ? MAX_CARAVAN_BUFFERSIZE : (iLength - uBufferPosition);
				CopyMemory( sPacket.baBuffer, baData + uBufferPosition, sPacket.uBufferCount );

				vPackets.push_back( sPacket );

				sPacket.sPacketCount.sMin++;

				uBufferPosition += sPacket.uBufferCount;
			}
		}
		else
			vPackets.push_back( sPacket );

		for ( auto & s : vPackets )
		{
			s.sPacketCount.sMax = (short)vPackets.size();
			ProcessPacket( pcUser, &s );
		}

		vPackets.clear();
	}

	DELETA( baData );
}

bool CFollowUnitHandler::UpdateCaravanItems( User * pcUser )
{
	bool bCanSave = true;

	//Inventory
	for ( int i = 0; i < pcUser->pcCaravanBase->psCaravanData->iItemCount; i++ )
	{
		ItemData * pc = pcUser->pcCaravanBase->psCaravanData->caItemData + i;
		if ( pc->bValid )
		{
			if ( ITEMSERVER->CheckValidItem( &pc->sItem ) )
			{
				bool bFound = false;
				for ( const auto s : pcUser->vCaravanItems )
				{
					if ( s.iItemID == pc->sItem.sItemID.ToInt() )
					{
						if ( s.iChk1 == pc->sItem.iChk1 )
						{
							if ( s.iChk2 == pc->sItem.iChk2 )
							{
								bFound = true;
								break;
							}
						}
					}
				}

				if ( bFound == false )
				{
					if ( ITEMSERVER->UseItem( pcUser->pcUserData, pc->sItem.sItemID.ToInt(), pc->sItem.iChk1, pc->sItem.iChk2 ) < 0 )
					{
						PacketLogCheat sPacket;
						sPacket.iCheatID = CHEATLOGID_CaravanItemAddError;
						sPacket.LParam = pc->sItem.sItemID.ToInt();
						sPacket.SParam = pc->sItem.iChk1;
						sPacket.EParam = pc->sItem.iChk2;
						STRINGCOPY( sPacket.szBuffer1, pc->sItem.szItemName );
						LOGSERVER->OnLogCheat( pcUser->pcUserData, &sPacket );

						bCanSave = false;
						break;
					}

					LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_CaravanAdd, "AccountName[%s] Character[%s] Added Item to Caravan ([%s][%08X][%d x %d])",
						pcUser->pcUserData->szAccountName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), pc->sItem.szItemName, pc->sItem.sItemID.ToInt(), pc->sItem.iChk1, pc->sItem.iChk2 );
				}
			}
		}
	}

	//Server
	if ( bCanSave )
	{
		for ( const auto s : pcUser->vCaravanItems )
		{
			bool bFound = false;
			for ( int i = 0; i < pcUser->pcCaravanBase->psCaravanData->iItemCount; i++ )
			{
				ItemData * pc = pcUser->pcCaravanBase->psCaravanData->caItemData + i;
				if ( pc->bValid )
				{
					if ( ITEMSERVER->CheckValidItem( &pc->sItem ) )
					{
						if ( s.iItemID == pc->sItem.sItemID.ToInt() )
						{
							if ( s.iChk1 == pc->sItem.iChk1 )
							{
								if ( s.iChk2 == pc->sItem.iChk2 )
								{
									bFound = true;
									break;
								}
							}
						}
					}
				}
			}

			if ( bFound == false )
			{
				char szItemName[32] = {0};
				ITEMSERVER->FindItemName( s.iItemID, szItemName );

				if ( ITEMSERVER->AddItemInventory( pcUser->pcUserData, s.iItemID, s.iChk1, s.iChk2 ) < 0 )
				{
					PacketLogCheat sPacket;
					sPacket.iCheatID = CHEATLOGID_CaravanItemDelError;
					sPacket.LParam = s.iItemID;
					sPacket.SParam = s.iChk1;
					sPacket.EParam = s.iChk2;
					STRINGCOPY( sPacket.szBuffer1, szItemName );
					LOGSERVER->OnLogCheat( pcUser->pcUserData, &sPacket );

					bCanSave = false;
					break;
				}

				LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_CaravanRemove, "AccountName[%s] Character[%s] Removed Item from Caravan ([%s][%08X][%d x %d])",
					pcUser->pcUserData->szAccountName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), szItemName, s.iItemID, s.iChk1, s.iChk2 );
			}
		}
	}

	return bCanSave;
}

void CFollowUnitHandler::ProcessPacket( User * pcUser, PacketCaravan * psPacket )
{
	psPacket->iLength = sizeof( Packet ) + psPacket->uBufferCount + sizeof( psPacket->uBufferCount ) + sizeof( psPacket->sPacketCount ) + sizeof( psPacket->iItemCount );
	psPacket->iHeader = PKTHDR_NPCCaravan;
	SENDPACKET( pcUser, psPacket );
}

void CFollowUnitHandler::ProcessPacket( User * pcUser, PacketCaravanUpdate * psPacket )
{
	psPacket->iLength = sizeof( PacketCaravanUpdate );
	psPacket->iHeader = PKTHDR_NPCCaravanUpdate;
	SENDPACKET( pcUser, psPacket );
}

void CFollowUnitHandler::ProcessPacket( User * pcUser, PacketCaravanOpen * psPacket )
{
	psPacket->iLength = sizeof( PacketCaravanOpen );
	psPacket->iHeader = PKTHDR_NPCCaravanOpen;
	SENDPACKET( pcUser, psPacket );
}

std::string CFollowUnitHandler::SQLGetCaravanName( User * pcUser )
{
	char szCaravanName[32] = { 0 };
	STRINGCOPY( szCaravanName, "Caravan" );

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT Name FROM CharacterCaravan WHERE CharacterID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );
			if ( pcDB->Execute() && pcDB->Fetch() )
				pcDB->GetData( 1, PARAMTYPE_String, szCaravanName, _countof( szCaravanName ) );
		}
		pcDB->Close();
	}

	return szCaravanName;
}

bool CFollowUnitHandler::SQLHaveCaravan( User * pcUser )
{
	bool bRet = false;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT ID FROM CharacterCaravan WHERE CharacterID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );
			if ( pcDB->Execute() && pcDB->Fetch() )
				bRet = true;
		}
		pcDB->Close();
	}

	return bRet;
}

void CFollowUnitHandler::SQLNewCaravan( User * pcUser )
{
	if ( !SQLHaveCaravan( pcUser ) )
	{
		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "INSERT INTO CharacterCaravan([Name],[CharacterID],[ItemsCount],[Date]) VALUES(?,?,0,GETDATE())" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_String, pcUser->pcCaravan->GetName(), STRLEN( pcUser->pcCaravan->GetName() ) );
				pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &pcUser->iCharacterID );
				pcDB->Execute();
			}
			pcDB->Close();
		}
	}
}

void CFollowUnitHandler::SQLUpdateCaravanName( User * pcUser )
{
	if ( SQLHaveCaravan( pcUser ) )
	{
		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "UPDATE CharacterCaravan SET Name=? WHERE CharacterID=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_String, pcUser->pcCaravan->GetName(), STRLEN( pcUser->pcCaravan->GetName() ) );
				pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &pcUser->iCharacterID );
				pcDB->Execute();
			}
			pcDB->Close();
		}
	}
}

bool CFollowUnitHandler::SQLGetCaravanItems( CaravanData * psCaravanData, int iCaravanID )
{
	bool bRet = false;

	char szQuery[256] = { 0 };
	STRINGFORMAT( szQuery, "SELECT TOP %d Item FROM CharacterCaravanItem WHERE CaravanID=?", psCaravanData->iItemCount );

	psCaravanData->iItemCount = 0;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( szQuery ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iCaravanID );
			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
					pcDB->GetData( 1, PARAMTYPE_Binary, &psCaravanData->caItemData[psCaravanData->iItemCount++], sizeof( ItemData ) );

				bRet = true;
			}
		}
		pcDB->Close();
	}

	return bRet;
}

void CFollowUnitHandler::SQLDeleteCaravanItems( User * pcUser )
{
	if ( SQLHaveCaravan( pcUser ) )
	{
		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
		if ( pcDB->Open() )
		{
			int iID = -1;

			if ( pcDB->Prepare( "UPDATE CharacterCaravan SET ItemsCount=0, Date=GETDATE() WHERE CharacterID=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );
				pcDB->Execute();
			}

			pcDB->Clear();
			if ( pcDB->Prepare( "SELECT ID FROM CharacterCaravan WHERE CharacterID=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );
				if ( pcDB->Execute() && pcDB->Fetch() )
					pcDB->GetData( 1, PARAMTYPE_Integer, &iID );
			}

			if ( iID >= 0 )
			{
				pcDB->Clear();
				if ( pcDB->Prepare( "DELETE FROM CharacterCaravanItem WHERE CaravanID=?" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iID );
					pcDB->Execute();
				}
			}
			pcDB->Close();
		}
	}
}
