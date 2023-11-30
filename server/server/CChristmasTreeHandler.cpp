#include "stdafx.h"
#include "CChristmasTreeHandler.h"


CChristmasTreeHandler::CChristmasTreeHandler()
{
}


CChristmasTreeHandler::~CChristmasTreeHandler()
{
}

bool CChristmasTreeHandler::SpawnTree( int iCharacterID, const char * pszCharacterName, EMapID iMapID, int iX, int iZ, BOOL bNormalTree )
{
	RemoveTree( iCharacterID );

	static char * pszMessage = "Give me a gift pls! ^^";

	bool bRet = false;

	Map * pcMap = MAPSERVER->GetMap( iMapID == MAPID_AcasiaForest ? MAPID_BambooForest : MAPID_AcasiaForest );
	if ( pcMap->pcStage )
	{
		CharacterData * psCharacterData = UNITSERVER->GetCharacterDataByName( bNormalTree ? "Xmas Tree 01" : "Xmas Tree 02" );
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

				STRINGFORMAT( pcUnitData->sCharacterData.szName, "%s's Xmas Tree", pszCharacterName );
				pcUnitData->sCharacterData.XmasTree.szHeadModel[0] = 0;
				STRINGCOPY( pcUnitData->sCharacterData.XmasTree.szCharacterNameOwner, pszCharacterName );

				pcUnitData->sCharacterData.iType = CHARACTERTYPE_NPC;

				pcUnitData->dwLastUpdateTime = 0;

				pcUnitData->sUnitInfo.iSmelting = NPCEVENTCODE_XmasTreeNPC;

				Unit * pcUnit = UNITDATATOUNIT( pcUnitData );
				pcUnit->bFollowUnitType = true;
				pcUnit->bCaravanFollow = false;
				pcUnit->iCharacterXmasTreeID = iCharacterID;

				pcUnitData->Animate( ANIMATIONTYPE_Idle );

				STRINGCOPY( pcUnitData->szMessage, pszMessage );
				pcUnitData->sUnitInfo.iNumMessage = 0;

				pcUnitData->sPosition.iX = iX;
				pcUnitData->sPosition.iZ = iZ;
				pcUnitData->sPosition.iY = pcUnitData->pcStage->GetHighestPoint( iX, iZ );

				pcUnitData->sCharacterData.cNewLoad++;

				UpdateMap( pcUnit, iMapID, pcUnitData->sPosition );

				SQLNewTree( iCharacterID, pcUnitData, bNormalTree );

				bRet = true;
			}
		}
	}

	return bRet;
}

void CChristmasTreeHandler::Init()
{
	if ( EVENT_XMAS == FALSE )
		return;

	std::vector<ChristmasTreeSQL*> vTrees;

	//Get xmas tree
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT Name, CharacterID, MapID, X, Z, IsNormalTree FROM CharacterXmasTree ORDER BY ID ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
					ChristmasTreeSQL * ps = new ChristmasTreeSQL;
					pcDB->GetData( 1, PARAMTYPE_String, ps->szName, _countof( ps->szName ) );
					pcDB->GetData( 2, PARAMTYPE_Integer, &ps->iCharacterID );
					pcDB->GetData( 3, PARAMTYPE_Integer, &ps->iMapID );
					pcDB->GetData( 4, PARAMTYPE_Integer, &ps->iX );
					pcDB->GetData( 5, PARAMTYPE_Integer, &ps->iZ );
					pcDB->GetData( 6, PARAMTYPE_Integer, &ps->bNormalTree );
					vTrees.push_back( ps );
				}
			}
		}
		pcDB->Close();
	}

	if ( vTrees.size() > 0 )
	{
		for ( auto & ps : vTrees )
		{
			SQLCharacter s;
			if ( CHARACTERSERVER->SQLGetCharacter( ps->iCharacterID, &s ) )
				SpawnTree( ps->iCharacterID, s.szCharacterName, ps->iMapID, ps->iX << 8, ps->iZ << 8, ps->bNormalTree );

			DELET( ps );
		}

		vTrees.clear();
	}
}

void CChristmasTreeHandler::Update()
{
	WORD wHour = GetServerTime()->wHour;

	if ( EVENT_XMAS )
	{
		if ( GetServerTime()->wMonth == 12 )
		{
			if ( (GetServerTime()->wDay == 24) || (GetServerTime()->wDay == 25) )
			{
				if ( LOGIN_SERVER && (wLastHour != wHour) )
					EVENTSERVER->SetDailyReward( wHour );
			}
		}
		if ( (wLastHour != wHour) && (wHour == 5) )
		{
			if ( LOGIN_SERVER )
			{
				//Update Items
				SQLUpdateGiftStatus( TRUE );

				CHATSERVER->SendChatAll( CHATCOLOR_Global, "Xmas> All received gifts are available! Visit Nataly to get yours!" );
			}

			if ( GAME_SERVER )
			{
				//Remove Trees
				RemoveAllTrees();
			}
		}

		if ( wLastHour != wHour )
			wLastHour = wHour;
	}
}

void CChristmasTreeHandler::OpenTree( User * pcUser, Unit * pcUnit )
{
	if ( pcUnit->iCharacterXmasTreeID != (-1) )
	{
		if ( pcUnit->iCharacterXmasTreeID != pcUser->iCharacterID )
		{
			if ( !SQLUserHasSentGift( pcUnit->iCharacterXmasTreeID, pcUser->iCharacterID ) )
			{
				SQLCharacter s1, s2;
				if ( CharacterServer::SQLGetCharacter( pcUser->iCharacterID, &s1 ) && CharacterServer::SQLGetCharacter( pcUnit->iCharacterXmasTreeID, &s2 ) )
				{
					if ( STRINGCOMPAREI( s1.szAccountName, s2.szAccountName ) == FALSE )
						ProcessPacket( pcUser, &PacketXmasTreeOpen( pcUnit->iCharacterXmasTreeID ^ CRC_XMASTREE_ITEMCHECKSUM, pcUnit->pcUnitData->sCharacterData.XmasTree.szCharacterNameOwner ) );
					else
					{
						CHATSERVER->SendChat( pcUser, CHATCOLOR_Error, "> You cannot send gift to yourself!" );
						CHATSERVER->SendChatMessageBox( pcUser, "You cannot send gift to yourself!" );
					}
				}
			}
			else
			{
				CHATSERVER->SendChat( pcUser, CHATCOLOR_Error, "> You cannot send more gifts to this friend today!" );
				CHATSERVER->SendChatMessageBox( pcUser, "You cannot send more gifts to this friend today!" );
			}
		}
		else
		{
			CHATSERVER->SendChat( pcUser, CHATCOLOR_Error, "> You cannot open your Xmas Tree!" );
			CHATSERVER->SendChatMessageBox( pcUser, "You cannot open your Xmas Tree!" );
		}
	}
}

void CChristmasTreeHandler::OpenGiftNPC( User * pcUser, Unit * pcUnit )
{
	SENDPACKETBLANK( pcUser, PKTHDR_XmasTreeNPCGiftOpen );
}

void CChristmasTreeHandler::HandlePacket( User * pcUser, PacketXmasTree * psPacket )
{
	if ( pcUser->pcXmasTreeBase == NULL )
	{
		pcUser->pcXmasTreeBase = new XmasTreeBase();
		pcUser->pcXmasTreeBase->uBufferCount = 0;
	}

	Receive( pcUser, psPacket );

	if ( psPacket->IsOnly() )
	{
		pcUser->pcXmasTreeBase->psXmasTreeData = new XmasTreeData;

		if ( psPacket->iItemCount > MAX_XMASTREE_ITEMS )
			psPacket->iItemCount = 0;

		pcUser->pcXmasTreeBase->psXmasTreeData->iItemCount = psPacket->iItemCount;

		SaveItems( pcUser, psPacket->iCharacterID ^ CRC_XMASTREE_ITEMCHECKSUM );

		DELET( pcUser->pcXmasTreeBase->psXmasTreeData );
		DELET( pcUser->pcXmasTreeBase );
	}
}

void CChristmasTreeHandler::HandlePacket( User * pcUser, PacketXmasTreeItemNPC * psPacket )
{
	ItemData cItemData;

	if ( psPacket->bItem )
	{
		std::vector<ChristmasItemData*> vItems;
		if ( SQLGetItemsTree( pcUser->iCharacterID, vItems ) )
		{
			for ( auto & ps : vItems )
			{
				SQLCharacter s;
				STRINGCOPY( s.szCharacterName, "Server" );
				CharacterServer::SQLGetCharacter( ps->iCharacterSenderID, &s );
				
				ITEMSERVER->AddItemOpenBox( pcUser->pcUserData->szAccountName, &ps->cItemData.sItem, FormatString( "%s gift", s.szCharacterName ) );

				SQLDeleteItemTree( pcUser->iCharacterID, ps->cItemData.sItem.sItemID.ToInt(), ps->cItemData.sItem.iChk1, ps->cItemData.sItem.iChk2 );

				DELET( ps );
			}
			vItems.clear();

			ProcessPacket( pcUser, &PacketXmasTreeResponse( 2 ) );
		}
	}
	else
		SendNextItem( pcUser );
}

void CChristmasTreeHandler::HandlePacket( User * pcUser, PacketXmasTreeItemUse * psPacket )
{
	switch ( psPacket->iItemID )
	{
		case ITEMID_SnowyXmasTree:
		case ITEMID_ColorfulXmasTree:
			if ( SpawnTree( pcUser->iCharacterID, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), pcUser->GetMapID(), pcUser->pcUserData->sPosition.iX, pcUser->pcUserData->sPosition.iZ,
							psPacket->iItemID == ITEMID_ColorfulXmasTree ? true : false ) )
			{
				LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_XmasTreeSpawn, "Character[%s] CharacterID[%d] Spawn Tree ( [%s Xmas Tree][0x%08X] %d x %d ) NormalTree[%s] MapID[%d] X[%d] Z[%d]",
										CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), pcUser->iCharacterID,
										psPacket->iItemID == ITEMID_ColorfulXmasTree ? "Colorful" : "Snowy", psPacket->iItemID, psPacket->iChk1, psPacket->iChk2,
										psPacket->iItemID == ITEMID_ColorfulXmasTree ? "Yes" : "No", pcUser->GetMapID(), pcUser->pcUserData->sPosition.iX >> 8, pcUser->pcUserData->sPosition.iZ >> 8 );
			}
			break;

		default:
			break;
	}
}

void CChristmasTreeHandler::Receive( User * pcUser, PacketXmasTree * psPacket )
{
	if ( psPacket->uBufferCount <= MAX_XMASTREE_BUFFERSIZE )
	{
		if ( (pcUser->pcXmasTreeBase->uBufferCount + psPacket->uBufferCount) <= MAX_XMASTREE_DATASIZE )
		{
			//First?
			if ( psPacket->IsFirst() )
			{
				ZeroMemory( pcUser->pcXmasTreeBase->baBuffer, MAX_XMASTREE_DATASIZE );
				pcUser->pcXmasTreeBase->uBufferCount = 0;
			}

			CopyMemory( pcUser->pcXmasTreeBase->baBuffer + pcUser->pcXmasTreeBase->uBufferCount, psPacket->baBuffer, psPacket->uBufferCount );
			pcUser->pcXmasTreeBase->uBufferCount += psPacket->uBufferCount;
		}
	}
}

bool CChristmasTreeHandler::UpdateXmasTreeItems( User * pcUser, int iCharacterID )
{
	bool bCanSave = true;

	//Inventory
	for ( int i = 0; i < pcUser->pcXmasTreeBase->psXmasTreeData->iItemCount; i++ )
	{
		ItemData * pc = pcUser->pcXmasTreeBase->psXmasTreeData->caItemData + i;
		if ( pc->bValid )
		{
			if ( ITEMSERVER->CheckValidItem( &pc->sItem ) )
			{
				if ( ITEMSERVER->UseItem( pcUser->pcUserData, pc->sItem.sItemID.ToInt(), pc->sItem.iChk1, pc->sItem.iChk2 ) < 0 )
				{
					bCanSave = false;
					break;
				}

				LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_XmasTreeAdd, "AccountName[%s] Character[%s] Added Item to Xmas Tree (Owner ID[%d]) ([%s][%08X][%d x %d])",
										pcUser->pcUserData->szAccountName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ),
										iCharacterID, pc->sItem.szItemName, pc->sItem.sItemID.ToInt(), pc->sItem.iChk1, pc->sItem.iChk2 );
			}
		}
	}

	return bCanSave;
}


void CChristmasTreeHandler::SaveItems( User * pcUser, int iCharacterID )
{
	int iLength = ITEMSERVER->DecodeItemsData( pcUser->pcXmasTreeBase->baBuffer, pcUser->pcXmasTreeBase->psXmasTreeData->caItemData, sizeof( ItemData ) * MAX_XMASTREE_ITEMS );
	if ( iLength >= 0 )
	{
		if ( UpdateXmasTreeItems( pcUser, iCharacterID ) )
		{
			if ( SQLHaveTree( iCharacterID ) )
			{
				SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
				if ( pcDB->Open() )
				{
					if ( pcDB->Prepare( "UPDATE CharacterXmasTree SET ItemsCount=ItemsCount+?, Date=GETDATE() WHERE CharacterID=?" ) )
					{
						pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->pcXmasTreeBase->psXmasTreeData->iItemCount );
						pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iCharacterID );
						pcDB->Execute();
					}

					int iID = -1;

					pcDB->Clear();
					if ( pcDB->Prepare( "SELECT ID FROM CharacterXmasTree WHERE CharacterID=?" ) )
					{
						pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iCharacterID );
						if ( pcDB->Execute() && pcDB->Fetch() )
							pcDB->GetData( 1, PARAMTYPE_Integer, &iID );
					}

					bool bSavedItems = false;

					if ( iID >= 0 )
					{
						for ( int i = 0; i < pcUser->pcXmasTreeBase->psXmasTreeData->iItemCount; i++ )
						{
							ItemData * pcItemData = pcUser->pcXmasTreeBase->psXmasTreeData->caItemData + i;

							pcDB->Clear();
							if ( pcDB->Prepare( "INSERT INTO CharacterXmasTreeItem([CharacterReceiverID],[CharacterSenderID],[ItemID],[Code1],[Code2],[Item],[Date],[IsAvailable]) VALUES(?,?,?,?,?,?,GETDATE(),0)" ) )
							{
								int iItemID = pcItemData->sItem.sItemID.ToInt();
								pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iCharacterID );
								pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &pcUser->iCharacterID );
								pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &iItemID );
								pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &pcItemData->sItem.iChk1 );
								pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &pcItemData->sItem.iChk2 );
								pcDB->BindParameterInput( 6, PARAMTYPE_Binary, pcItemData, sizeof( ItemData ) );
								pcDB->Execute();

								bSavedItems = true;
							}
						}
					}

					pcDB->Close();

					if ( bSavedItems )
					{
						ProcessPacket( pcUser, &PacketXmasTreeResponse( 1 ) );

						EVENTSERVER->AddXmasPrize( pcUser );
					}
				}
			}
		}
	}
}


void CChristmasTreeHandler::RemoveTree( Unit * pcUnit )
{
	//Delete from SQL
	SQLDeleteTree( pcUnit->iCharacterXmasTreeID );

	//Free data
	pcUnit->pcUnitData->Free();
}

void CChristmasTreeHandler::RemoveTree( int iCharacterID )
{
	//Find an user's tree
	for ( UINT i = 0; i < UNITSERVER->uUnitsInGame; i++ )
	{
		Unit * pcUnit = UNITSERVER->pcaUnitInGame[i];
		if ( pcUnit->iCharacterXmasTreeID == iCharacterID )
		{
			//Remove
			RemoveTree( pcUnit );
			break;
		}
	}
}

void CChristmasTreeHandler::RemoveAllTrees()
{
	//Find an user's tree
	for ( UINT i = 0; i < UNITSERVER->uUnitsInGame; i++ )
	{
		Unit * pcUnit = UNITSERVER->pcaUnitInGame[i];
		if ( pcUnit->iCharacterXmasTreeID != (-1) )
		{
			//Remove
			RemoveTree( pcUnit );
		}
	}
}

bool CChristmasTreeHandler::SQLHaveTree( int iCharacterID )
{
	bool bRet = false;
	
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT ID FROM CharacterXmasTree WHERE CharacterID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iCharacterID );
			if ( pcDB->Execute() )
				if ( pcDB->Fetch() )
					bRet = true;
		}
		pcDB->Close();
	}

	return bRet;
}

void CChristmasTreeHandler::SQLNewTree( int iCharacterID, UnitData * pcUnitData, BOOL bNormalTree )
{
	if ( !SQLHaveTree( iCharacterID ) )
	{
		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "INSERT INTO CharacterXmasTree([Name],[CharacterID],[MapID],[X],[Z],[IsNormalTree],[ItemsCount],[Date]) VALUES(?,?,?,?,?,?,0,GETDATE())" ) )
			{
				int iMapID = MAPID_Invalid;
				if ( pcUnitData->pcMap )
				{
					if ( pcUnitData->pcMap->pcBaseMap )
						iMapID = pcUnitData->pcMap->pcBaseMap->iMapID;
				}

				int iX = pcUnitData->sPosition.iX >> 8;
				int iZ = pcUnitData->sPosition.iZ >> 8;

				pcDB->BindParameterInput( 1, PARAMTYPE_String, pcUnitData->GetName(), STRLEN( pcUnitData->GetName() ) );
				pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iCharacterID );
				pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &iMapID );
				pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &iX );
				pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &iZ );
				pcDB->BindParameterInput( 6, PARAMTYPE_Integer, &bNormalTree );
				pcDB->Execute();
			}
			pcDB->Close();
		}
	}
}

void CChristmasTreeHandler::SQLDeleteTree( int iCharacterID )
{
	if ( SQLHaveTree( iCharacterID ) )
	{
		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "DELETE FROM CharacterXmasTree WHERE CharacterID=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iCharacterID );
				pcDB->Execute();
			}
			pcDB->Close();
		}
	}
}

bool CChristmasTreeHandler::SQLGetItemTree( int iCharacterID, int iItemID, int iChk1, int iChk2, ItemData & cItemData )
{
	bool bRet = false;
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT Item FROM CharacterXmasTreeItem WHERE (CharacterReceiverID=?) AND (ItemID=?) AND (Code1=?) AND (Code2=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iCharacterID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iItemID );
			pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &iChk1 );
			pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &iChk2 );

			if ( pcDB->Execute() )
			{
				if ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Binary, &cItemData, sizeof( ItemData ) );
					bRet = true;
				}
			}
		}
		pcDB->Close();
	}

	return bRet;
}

bool CChristmasTreeHandler::SQLGetItemsTree( int iCharacterID, std::vector<ChristmasItemData*>& vItems )
{
	bool bRet = false;
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT Item, CharacterSenderID FROM CharacterXmasTreeItem WHERE (CharacterReceiverID=?) AND (IsAvailable=1)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iCharacterID );
			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
					ChristmasItemData * ps = new ChristmasItemData();
					pcDB->GetData( 1, PARAMTYPE_Binary, &ps->cItemData, sizeof( ItemData ) );
					pcDB->GetData( 2, PARAMTYPE_Integer, &ps->iCharacterSenderID );
					vItems.push_back( ps );
					bRet = true;
				}
			}
		}
		pcDB->Close();
	}

	return bRet;
}

void CChristmasTreeHandler::SQLDeleteItemTree( int iCharacterID, int iItemID, int iChk1, int iChk2 )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "DELETE FROM CharacterXmasTreeItem WHERE (CharacterReceiverID=?) AND (ItemID=?) AND (Code1=?) AND (Code2=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iCharacterID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iItemID );
			pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &iChk1 );
			pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &iChk2 );
			pcDB->Execute();
		}
		pcDB->Close();
	}
}

bool CChristmasTreeHandler::SQLUserHasSentGift( int iCharacterReceiverID, int iCharacterSenderID )
{
	bool bRet = false;
	
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT TOP 1 ID FROM CharacterXmasTreeItem WHERE (CharacterReceiverID=?) AND (CharacterSenderID=?) AND (IsAvailable=0)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iCharacterReceiverID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iCharacterSenderID );
			if ( pcDB->Execute() )
				if ( pcDB->Fetch() )
					bRet = true;
		}
		pcDB->Close();
	}

	return bRet;
}

void CChristmasTreeHandler::SQLUpdateGiftStatus( BOOL bIsAvailable )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE CharacterXmasTreeItem SET IsAvailable=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &bIsAvailable );
			pcDB->Execute();
		}
		pcDB->Close();
	}
}

void CChristmasTreeHandler::SendNextItem( User * pcUser )
{
	PacketXmasTreeItemNPC sPacket;
	sPacket.bItem = FALSE;
	sPacket.iItemCount = 0;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT COUNT(ID) FROM CharacterXmasTreeItem WHERE (CharacterReceiverID=?) AND (IsAvailable=1)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );

			if ( pcDB->Execute() )
			{
				if ( pcDB->Fetch() )
					pcDB->GetData( 1, PARAMTYPE_Integer, &sPacket.iItemCount );
			}
		}
		pcDB->Close();
	}

	ProcessPacket( pcUser, &sPacket );
}

void CChristmasTreeHandler::ProcessPacket( User * pcUser, PacketXmasTreeOpen * psPacket )
{
	psPacket->iLength = sizeof( PacketXmasTreeOpen );
	psPacket->iHeader = PKTHDR_XmasTreeOpen;
	SENDPACKET( pcUser, psPacket );
}

void CChristmasTreeHandler::ProcessPacket( User * pcUser, PacketXmasTreeItemNPC * psPacket )
{
	psPacket->iLength = sizeof( PacketXmasTreeItemNPC );
	psPacket->iHeader = PKTHDR_XmasTreeNPCGift;
	SENDPACKET( pcUser, psPacket );
}

void CChristmasTreeHandler::ProcessPacket( User * pcUser, PacketXmasTreeResponse * psPacket )
{
	psPacket->iLength = sizeof( PacketXmasTreeResponse );
	psPacket->iHeader = PKTHDR_XmasTreeNPCResponse;
	SENDPACKET( pcUser, psPacket );
}

bool CChristmasTreeHandler::IsNearNPC( Map * pcMap, int iX, int iZ )
{
	bool bRet = false;

	Point3D sPosition = Point3D( iX, 0, iZ );

	for ( UINT i = 0; i < UNITSERVER->uUnitsInGame; i++ )
	{
		Unit * pcUnit = UNITSERVER->pcaUnitInGame[i];
		if ( pcUnit->pcUnitData->sCharacterData.iType == CHARACTERTYPE_NPC )
		{
			if ( pcUnit->GetPosition().WithinPTDistance( &sPosition, 300 ) )
			{
				bRet = true;
				break;
			}
		}
	}

	return bRet;
}

void CChristmasTreeHandler::UpdateMap( Unit * pcUnit, EMapID iMapID, Point3D sPosition )
{
	if ( pcUnit->pcUnitData->pcMap->pcBaseMap && (pcUnit->pcUnitData->pcMap->pcBaseMap->iMapID != iMapID) )
	{
		if ( auto pcNewMap = MAPSERVER->GetMap( iMapID ) )
		{
			if ( auto pcNewStage = MAPSERVER->GetMap( iMapID )->pcStage )
			{
				pcUnit->pcUnitData->pcMap = pcNewMap;
				pcUnit->pcUnitData->pcStage = pcNewStage;

				sPosition.iY = pcUnit->pcUnitData->pcStage->GetHighestPoint( sPosition.iX, sPosition.iZ );
				pcUnit->pcUnitData->sPosition = sPosition;

				pcUnit->pcUnitData->Animate( ANIMATIONTYPE_Idle );

				pcUnit->pcUnitData->sCharacterData.sHP.sCur = pcUnit->pcUnitData->sCharacterData.sHP.sMax;
			}
		}
	}
}
