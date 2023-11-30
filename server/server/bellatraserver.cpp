#include "stdafx.h"
#include "bellatraserver.h"

DWORD dwClanBellatraUpdateTime = 0;

WORD wLastDayResetSOD = 0;

const int iMinimalUserClanPoints = 3;

static BOOL baBossSpawnSOD[8];

BellatraServer::BellatraServer()
{
	SYSTEMTIME * ps = GetServerTime();
	wLastDayResetSOD = ps->wDay;
}


BellatraServer::~BellatraServer()
{
}

void BellatraServer::UpdateClanCrown()
{
	DWORD iClanID[3]		= { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };
	SQLConnection * pcDB	= SQLCONNECTION( DATABASEID_ClanDB );

	PacketBellatraCrown sPacket;
	sPacket.iLength			= sizeof( PacketBellatraCrown );
	sPacket.iHeader			= PKTHDR_BellatraCrown;
	sPacket.iBellatraCrown	= 0;

	//Clear Crowns
	for ( UINT i = 0; i < USERSERVER->uUsersInGame; i++ )
	{
		User * pcUser = USERSERVER->pcaUserInGame[i];
		if ( pcUser->pcUserData->iClanID != 0 )
			SENDPACKET( pcUser, &sPacket, TRUE );
	}

	int j = 0;
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT TOP 3 IconID FROM ClanList ORDER BY BellatraPoints DESC" ) )
		{
			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &iClanID[j], 0 );
					j++;
				}
			}
		}
		pcDB->Close();
	}

	//Update
	for ( UINT i = 0; i < USERSERVER->uUsersInGame; i++ )
	{
		User * pcUser = USERSERVER->pcaUserInGame[i];

		sPacket.iBellatraCrown = 0;

		if ( pcUser->pcUserData->sCharacterData.iClanID == iClanID[0] )
			sPacket.iBellatraCrown = 1;
		else if ( pcUser->pcUserData->sCharacterData.iClanID == iClanID[1] )
			sPacket.iBellatraCrown = 2;
		else if ( pcUser->pcUserData->sCharacterData.iClanID == iClanID[2] )
			sPacket.iBellatraCrown = 3;

		SENDPACKET( pcUser, &sPacket, TRUE );
	}
}

int BellatraServer::GetUserRoomCount( int i )
{
	if ( i >= 0 && i < 4 )
		return iaUserRoomCount[i];

	return 0;
}

void BellatraServer::SetUserRoomCount( int iRoomID, int iCount )
{
	if ( iRoomID >= 0 && iRoomID < 4 )
		iaUserRoomCount[iRoomID] = iCount;
}

int BellatraServer::GetRoomScore( int i )
{
	if ( i >= 0 && i < 4 )
	{
		int * p = (int*)pfnRoomScoreData;

		return p[i];
	}

	return 0;
}

void BellatraServer::SetBellatraDataNew( BellatraData * ps )
{
	//CopyMemory( saBellatraData, )
}

void BellatraServer::SetRoundNew(int i)
{
	int * piaRoomKills = ((int*)pfnRoomScoreKill);
	ZeroMemory(piaRoomKills, sizeof(int) * 8);
	ZeroMemory(baBossSpawnSOD, sizeof(BOOL) * 8);
	iRound = i;
}

void BellatraServer::ResetBellatra()
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ClanDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "DELETE FROM BellatraTeamScore" ) )
			pcDB->Execute();
		pcDB->Close();
	}
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "DELETE FROM BellatraPersonalScore" ) )
			pcDB->Execute();
		pcDB->Close();
	}
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE ClanList SET BellatraPoints=0, BellatraGold=0, BellatraDate=0" ) )
			pcDB->Execute();
		pcDB->Close();
	}
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE ClanList SET BellatraPoints=100000 WHERE ClanName=?" ) )
		{
			char * pszClanName = "GM";
			pcDB->BindParameterInput( 1, PARAMTYPE_String, pszClanName, STRLEN( pszClanName ) );
			pcDB->Execute();
		}
		pcDB->Close();
	}

	// Update
	PacketBellatraCrown sPacket;
	sPacket.iLength			= sizeof( PacketBellatraCrown );
	sPacket.iHeader			= PKTHDR_BellatraCrown;
	sPacket.iBellatraCrown	= 0;

	// Clear Crowns
	for ( int i = 0; i < PLAYERS_MAX; i++ )
	{
		if ( USERSDATA[i].iClanID && USERSDATA[i].iID && USERSDATA[i].pcSocketData )
		{
			SENDPACKET( USERDATATOUSER(&USERSDATA[i]), &sPacket, TRUE );
		}
	}

	CHATSERVER->SendChatAll( CHATCOLOR_Global, "Bellatra> SoD Scores has been Reset!" );
}

BOOL BellatraServer::PHGoldClanRecv( User * pcUser, struct PacketClanGoldReceive * psPacket )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ClanDB );
	int iGoldDB = 0; 
	int iGold	= psPacket->iGold;
	BOOL bUpdate = FALSE;
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT BellatraGold FROM ClanList WHERE ClanLeader=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );
			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_Integer, &iGoldDB, 0 );
			}
		}
		pcDB->Close();
	}

	if ( iGold > 0 )
	{
		if ( iGold < 0 || iGoldDB < iGold )
		{
			PacketLogCheat sPacket;
			sPacket.iCheatID = CHEATLOGID_BellatraGoldHackLeader;
			sPacket.LParam = iGold;
			sPacket.SParam = iGoldDB;
			LOGSERVER->OnLogCheat( pcUser->pcUserData, &sPacket );
		}
		else
		{
			USERSERVER->AddServerUserGold( pcUser->pcUserData, iGold, WHEREID_ClanMoney );
			psPacket->iGoldSend = iGold;
			psPacket->iGold		= -iGold;
			SENDPACKET( pcUser, psPacket );
			bUpdate = TRUE;
		}
	}
	else
	{
		psPacket->iGold		= iGoldDB;
		psPacket->iGoldSend = 0;
		SENDPACKET( pcUser, psPacket );
	}

	if ( bUpdate )
	{
		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "UPDATE ClanList SET BellatraGold=BellatraGold-? WHERE ClanLeader=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iGold );
				pcDB->BindParameterInput( 2, PARAMTYPE_String, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );
				pcDB->Execute();
			}
			pcDB->Close();
		}
	}
	return TRUE;
}

void BellatraServer::DeletePoint( char * pszUserID, char * pszCharName, char * pszIP, int iUnk, int iUnk2 )
{

}


BOOL BellatraServer::AddGoldClan( int SodScore )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ClanDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE ClanList SET BellatraGold=BellatraGold+? WHERE ID=(SELECT TOP 1 ID FROM ClanList ORDER BY BellatraPoints DESC)" ) )
		{
			int iGoldPercent = (SodScore * EVENT_BELLATRA_TAX) / 100;
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iGoldPercent );
			pcDB->Execute();
		}
		pcDB->Close();
	}
	return TRUE;
}

int BellatraServer::GetClanPoint( UserData * pcUserData )
{
	int iPoints = 0;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ClanDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT TotalPoint FROM BellatraTeamScore WHERE (CharacterName=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, CHARACTERSERVER->GetCharacterName( pcUserData ) );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_Integer, &iPoints );
			}
		}

		pcDB->Close();
	}
	return iPoints;
}

BOOL BellatraServer::RecordPoint( UserData * pcUserData, int iLevel, int SodScore, int SodKillCount, int SodTotalScore, int SodTotalSucessUser, int SodTotalUser, int SodCode )
{
	if ( pcUserData && SodScore > 0 )
	{
		int iClanID = 0;

		char szDateTime[20];
		GetDateTime( szDateTime );

		USERSERVER->SetUserID( pcUserData );

		if ( GetClanPoint( pcUserData ) <= SodTotalScore )
		{
			SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ClanDB );
			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "DELETE FROM BellatraTeamScore WHERE (CharacterName=?)" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_String, CHARACTERSERVER->GetCharacterName( pcUserData ) );
					pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &SodScore );

					pcDB->Execute();
				}
				pcDB->Close();
			}
			if ( SodTotalUser > 1 )
			{
				if ( pcDB->Open() )
				{
					if ( pcDB->Prepare( "INSERT INTO BellatraTeamScore ([AccountName],[CharacterName],[JobCode],[Level],[ClanCode],[Score],[Kills],[TotalPoint],[TotalUser],[SuccessUser],[Code],[Quake],[StunSeal],[FreezeSeal],[RabieSeal],[StygianSeal],[GuardianSaintSeal],[PointBag],[Date]) VALUES (?,?,?,?,?,?,?,?,?,?,?,0,0,0,0,0,0,0,?)" ) )
					{
						pcDB->BindParameterInput( 1, PARAMTYPE_String, pcUserData->szAccountName );
						pcDB->BindParameterInput( 2, PARAMTYPE_String, CHARACTERSERVER->GetCharacterName( pcUserData ) );
						pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &pcUserData->sCharacterData.iClass );
						pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &iLevel );
						pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &iClanID );
						pcDB->BindParameterInput( 6, PARAMTYPE_Integer, &SodScore );
						pcDB->BindParameterInput( 7, PARAMTYPE_Integer, &SodKillCount );
						pcDB->BindParameterInput( 8, PARAMTYPE_Integer, &SodTotalScore );
						pcDB->BindParameterInput( 9, PARAMTYPE_Integer, &SodTotalUser );
						pcDB->BindParameterInput( 10, PARAMTYPE_Integer, &SodTotalSucessUser );
						pcDB->BindParameterInput( 11, PARAMTYPE_Integer, &SodCode );
						pcDB->BindParameterInput( 12, PARAMTYPE_String, szDateTime );

						pcDB->Execute();
					}
					pcDB->Close();
				}
			}
		}

		AddGoldClan( SodScore );

		int iID = 0;
		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ClanDB );
		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "SELECT ID FROM BellatraPersonalScore WHERE CharacterName=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_String, CHARACTERSERVER->GetCharacterName( pcUserData ) );
				if ( pcDB->Execute() && pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &iID, 0 );
				}
			}
			pcDB->Close();
		}
		if ( SodTotalUser == 1 && SodTotalSucessUser == 1 )
		{
			SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ClanDB );
			if ( pcDB->Open() )
			{
				if ( iID )
				{
					if ( pcDB->Prepare( "UPDATE BellatraPersonalScore SET Kills=?, Score=?, Date=? WHERE (CharacterName=?) AND (Score < ?)" ) )
					{
						pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &SodKillCount );
						pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &SodScore );
						pcDB->BindParameterInput( 3, PARAMTYPE_String, szDateTime );
						pcDB->BindParameterInput( 4, PARAMTYPE_String, CHARACTERSERVER->GetCharacterName( pcUserData ) );
						pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &SodScore );
						pcDB->Execute();
					}
				}
				else
				{
					if ( pcDB->Prepare( "INSERT INTO BellatraPersonalScore ([AccountName],[CharacterName],[Kills],[Score],[Date]) VALUES (?,?,?,?,?)" ) )
					{
						pcDB->BindParameterInput( 1, PARAMTYPE_String, pcUserData->szAccountName );
						pcDB->BindParameterInput( 2, PARAMTYPE_String, CHARACTERSERVER->GetCharacterName( pcUserData ) );
						pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &SodKillCount );
						pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &SodScore );
						pcDB->BindParameterInput( 5, PARAMTYPE_String, szDateTime );
						pcDB->Execute();
					}
				}
				pcDB->Close();
			}
		}
	}
	return TRUE;
}

BOOL BellatraServer::RecordClanPoint( const char * pszClanName, int iPoint, int iOldPoint )
{
	int iClanLeaderPoint	= 0; 
	int iGold				= 0;
	char szClanName[32]		= { 0 };
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ClanDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT TOP 1 ClanName, BellatraPoints, BellatraGold FROM ClanList ORDER BY BellatraPoints DESC" ) )
		{
			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_String, szClanName, 0 );
				pcDB->GetData( 2, PARAMTYPE_Integer, &iClanLeaderPoint, 0 );
				pcDB->GetData( 3, PARAMTYPE_Integer, &iGold, 0 );
			}
		}
		pcDB->Close();
	}



	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE ClanList SET BellatraPoints=?, BellatraDate=? WHERE ClanName=?" ) )
		{
			DWORD dwTime = (DWORD)time( NULL );
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iPoint );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &dwTime );
			pcDB->BindParameterInput( 3, PARAMTYPE_String, (void*)pszClanName );
			pcDB->Execute();
		}
		pcDB->Close();
	}

	if ( !STRINGCOMPAREI( szClanName, pszClanName ) && iPoint > iClanLeaderPoint )
	{
		CHATSERVER->SendChatAll( CHATCOLOR_Global, "Bellatra> New Top Clan!" );
		CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "Bellatra> Clan: %s", pszClanName );
		CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "Bellatra> Score: %s", FormatNumber(iPoint) );

		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "UPDATE ClanList SET BellatraGold=0" ) )
			{
				pcDB->Execute();
			}
			pcDB->Close();
		}
		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "UPDATE ClanList SET BellatraGold=? WHERE ClanName=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iGold );
				pcDB->BindParameterInput( 2, PARAMTYPE_String, (void*)pszClanName );
				pcDB->Execute();
			}
			pcDB->Close();
		}
		
		// Update Crowns
		UpdateClanCrown();
	}
	return TRUE;
}

int BellatraServer::GetClanPoint( const char * pszClanName )
{
	int iClanPoint = 0;
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ClanDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT BellatraPoints FROM ClanList WHERE ClanName=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, (void*)pszClanName );
			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_Integer, &iClanPoint, 0 );
			}
		}
		pcDB->Close();
	}
	return iClanPoint;
}

BOOL BellatraServer::UpdateClanBellatra()
{
	if ( GAME_SERVER )
		return TRUE;

	if ( dwClanBellatraUpdateTime > TICKCOUNT )
		return TRUE;

	dwClanBellatraUpdateTime = TICKCOUNT + 30000;


	char szCharName[32]		= { 0 };
	char szCharName2[32]	= { 0 };
	int iPoint				= 0;
	int iCode				= 0;
	int iTotalPoint			= 0;
	int iSuccessUser		= 0;
	int iCurrentClanPoint	= 0;
	int iTotalPlayers		= 0;
	char szClanName[32]		= { 0 };
	char szClanName2[32]	= { 0 };

	SYSTEMTIME sTime;
	GetLocalTime( &sTime );
	if ( sTime.wDayOfWeek == 1 && sTime.wDay != wLastDayResetSOD )
	{
		ResetBellatra();
		wLastDayResetSOD = sTime.wDay;
	}
	return TRUE;
}

BOOL BellatraServer::AddRoomScore( int iRoomID, int iScore )
{
	int * piaRoomScore = ((int*)pfnRoomScoreData);

	if ( iRoomID >= 0 && iRoomID < 8 )
	{
		piaRoomScore[iRoomID] += iScore;
		return TRUE;
	}

	return FALSE;
}

BOOL BellatraServer::UnitKilled( UnitData * pcUnitData )
{
	int * piaRoomKills = ((int*)pfnRoomScoreKill);

	piaRoomKills[pcUnitData->iBellatraRoom]--;
	
	//3%
	if ( RandomI( 0, 1000 ) < 10 )
	{
		Map::Loot::LootItem sLootItem;

		//Get Item Data
		DefinitionItem * ps = ITEMSERVER->FindItemDefByCode( ITEMID_BrilliantStone );  
		if ( ps )
		{
			//Create Item with non-spec
			ITEMSERVER->CreateItem( &sLootItem.sItem, ps, 0 );

			//Get Position of Unit on Map
			int iX = pcUnitData->sPosition.iX >> 8;
			int iY = pcUnitData->sPosition.iY >> 8;
			int iZ = pcUnitData->sPosition.iZ >> 8;

			//Get Map Data from Unit
			Map * psMap = (Map*)pcUnitData->pcMap;
			if ( psMap )
			{
				//Add Item to Map
				Map::Loot * psItemD = psMap->AddItem( &sLootItem, iX, iY, iZ, TRUE );

				//Added? Send It to near users...
				if ( psItemD )
					MAPSERVER->SendItemStageNearUsers( psMap, psItemD );
			}
		}
	}

	return TRUE;
}

NAKED BOOL BellatraServer::UpdateBellatraUserScore( UserData * pcUserData )
{
	JMP( pfnUpdateBellatraUserScore );
}

NAKED BOOL BellatraServer::HandleItemRoom( UnitData * pcUnitData, UserData * pcUserData )
{
	JMP( pfnHandleBellatraRoomItem );
}

NAKED void BellatraServer::HandleConfigurationNextRound()
{
	JMP( pfnHandleConfigNextRound );
}

NAKED void BellatraServer::SendScoreUser( UserData * pcUserData, int iScore, int iKills, int iTotalScore, int iUserCount, int iTotalUsers, int iCode )
{
	JMP( pfnSendScore );
}

NAKED BOOL BellatraServer::CreateBellatraItem( int iItemID, Map * pcStage, int iX, int iY, int iZ, int iQuantity )
{
	JMP( pfnCreateBellatraItem );
}

void BellatraServer::UpdateRound( int iNewRound )
{
}

void BellatraServer::SpawnMonster( int iCount )
{
	//Get Bellatra Data
	BellatraData * psBellatra = GetBellatraDataNew( GetRound() );
}

void BellatraServer::UpdateIhin( bool bShow )
{
	Map * psSOD = MAPSDATA + MAPID_Bellatra;
	//Spawn NPC?
	if ( bShow )
	{
		for ( int i = 0; i < MAX_NPCINMAP; i++ )
		{
			PacketUnitInfo * ps = psSOD->saNPC + i;
			if ( ps->iHeader && STRINGCOMPARE( "152", ps->sCharacterData.szName ) )
			{
				CALLT_WITH_ARG1( 0x0054E100, (DWORD)psSOD, (DWORD)ps );
				break;
			}
		}
	}
	else
	{
		for ( int i = 0; i < MAX_UNITS; i++ )
		{
			UnitData * pc = UNITSDATA + i;
			if ( pc && pc->sCharacterData.iType == ECharacterType::CHARACTERTYPE_NPC && (DWORD)pc->pcMap == (DWORD)psSOD )
			{
				if ( STRINGCOMPARE( "Ihin", pc->GetName() ) )
				{
					pc->Free();
					break;
				}
			}
		}
	}
}

void BellatraServer::UpdateUsersRoom()
{
	/*
	//Clear Rooms
	vUsersRoom[0].clear();
	vUsersRoom[1].clear();
	vUsersRoom[2].clear();
	vUsersRoom[3].clear();

	//Update
	if ( USERSERVER->uUsersInGame > 0 )
	{
		for ( UINT i = 0; i < USERSERVER->uUsersInGame; i++ )
		{
			User * pcUser = USERSERVER->pcaUserInGame[i];

			//User in bellatra room?
			if ( pcUser->pcUserData->dwExclusiveNum == 0x100 )
			{
				//Valid room? Add User
				if ( (pcUser->pcUserData->sBellatraRoomID >= 0) && (pcUser->pcUserData->sBellatraRoomID < 4) )
					vUsersRoom[pcUser->pcUserData->sBellatraRoomID].push_back( pcUser );
			}
		}
	}
	*/
}

void BellatraServer::UpdateUsersScore()
{
	//Have Users?
	if ( USERSERVER->uUsersInGame > 0 )
	{
		for ( UINT i = 0; i < USERSERVER->uUsersInGame; i++ )
		{
			User * pcUser = USERSERVER->pcaUserInGame[i];
			
			//User in bellatra room?
			if ( pcUser->pcUserData->dwExclusiveNum == 0x100 )
			{
				//Send Score
			//	BellatraServer::SendScoreUser( pcUser->pcUserData, pcUser->pcUserData->iBellatraScore, pcUser->pcUserData->iBellatraKills, iTotalScore, iUserCount, GetTotalUsers(), (*(int*)0x07AB1550) );
			}
		}
	}
}

BOOL BellatraServer::UpdateServer( Map * pcStage, MonsterMapSpawnHandle * psMonsterData )
{
	return FALSE;

	/*
	//Event is not started?
	if ( bEventStart == false )
	{
		//Update Round
		UpdateRound( 1 );

		//Hide Ihin
		UpdateIhin( false );

		//Spawn Monsters
		SpawnMonster( GetBellatraData( GetRound() )->iMaxMonsters );

		//Update Users in room
		UpdateUsersRoom();

		//Update Information to Users
		UpdateUsersScore();

		//Success
		bEventStart = true;
	}
	else
	{

	}


	return TRUE;

*/
	BellatraData * psBellatraData = GetBellatraData() + (GetRound() - 1);

	PacketBellatraInformation sPacket1;

	if ( READDWORD( 0x07AC9E30 ) )
	{
		if ( READDWORD( 0x07AC9E2C ) )
		{
			WRITEDWORD( 0x07AC9E30, FALSE );
		}
		else
			return TRUE;
	}

	BOOL bHour = FALSE;

	if ( psBellatraData->saMonster[0].szName[0] == 0 )
	{
		if ( dwFinishTime <= TICKCOUNT )
		{
			Map * psSOD = MAPSDATA + MAPID_Bellatra;
			for ( int i = 0; i < MAX_NPCINMAP; i++ )
			{
				PacketUnitInfo * ps = psSOD->saNPC + i;

				if ( ps->iHeader && STRINGCOMPARE( "152", ps->sCharacterData.szName ) )
				{
					CALLT_WITH_ARG1( 0x0054E100, (DWORD)psSOD, (DWORD)ps );
					break;
				}
			}

			bHour = TRUE;
		}
	}
	else if ( psBellatraData->iHour == TIMEHOUR )
		bHour = TRUE;

	if ( bHour )
	{
		if ( GetRound() == 1 )
		{
			Map * psSOD = MAPSDATA + MAPID_Bellatra;		
			for ( int i = 0; i < MAX_UNITS; i++ )
			{
				UnitData * pc = UNITSDATA + i;

				if ( pc && pc->sCharacterData.iType == ECharacterType::CHARACTERTYPE_NPC && (DWORD)pc->pcMap == (DWORD)psSOD )
				{
					if ( STRINGCOMPARE( "Ihin", pc->GetName() ) )
					{
						pc->Free();
						break;
					}
				}
			}

			BellatraServer::HandleConfigurationNextRound();
			psBellatraData = GetBellatraData() + (GetRound() - 1);

			dwFinishTime = TICKCOUNT + (47 * 1000) + (16 * 60 * 1000);
		}
		
		for ( int i = 0; i < MAX_ALIVEMONSTERS; i++ )
		{
			UnitData * pc = (UnitData*)pcStage->pcaUnitData[i];

			if ( pc && pc->sCharacterData.iType == ECharacterType::CHARACTERTYPE_Monster )
			{
				if ( pc->bActive && pc->dwExclusiveNum == 0x100 && pc->IsDead() == FALSE )
				{
					//Kill
					pc->sCharacterData.sHP.sCur = 0;
					pc->Animate( ANIMATIONTYPE_Die );

					//Not Have Name? (Last Round Finished)
					if ( psBellatraData->saMonster[0].szName[0] == 0 )
					{
						//Put Mana??
						pc->dwExclusiveNum = 0x04010400;

						//Create Item Present (??)
						BellatraServer::CreateBellatraItem( 0x04010400, pcStage, pc->sPosition.iX, pc->sPosition.iY, pc->sPosition.iZ, 6 );
					}
				}
			}
		}

		ZeroMemory( BellatraServer::GetMonsterSpawnHandle(), sizeof( BellatraMonsterSpawnHandle ) );

		if ( psBellatraData->iPoints > 0 )
		{
			int iTotalPoints = 0;

			for ( int i = 0; i < PLAYERS_MAX; i++ )
			{
				UserData * ud = USERSDATA + i;

				if ( ud->pcSocketData && ud->dwExclusiveNum == 0x100 )
				{
					if ( MAPSERVER->IsBellatraMapArea( ud->sPosition.iX, ud->sPosition.iY, ud->sPosition.iZ ) )
					{
						if ( ud->iBellatraScore > 0 )
							iTotalPoints += ud->iBellatraScore;
					}
				}
			}

			if ( (iTotalPoints > 0) && (iTotalPoints < psBellatraData->iPoints) )
			{
				//Update Round
				SetRound( GetRound() );

				psBellatraData = GetBellatraData() + (GetRound() - 1);

				Map * psNaviskoMap = MAPSDATA + MAPID_NaviskoTown;
				PacketWarpGateField sPacket;
				sPacket.iLength = sizeof( PacketWarpGateField );
				sPacket.iHeader = PKTHDR_WarpGateField;
				sPacket.iX = psNaviskoMap->pcBaseMap->GetCenter().iX << 8;
				sPacket.iZ = psNaviskoMap->pcBaseMap->GetCenter().iY << 8;
				sPacket.iStage = MAPID_NaviskoTown;
				
				for ( int i = 0; i < PLAYERS_MAX; i++ )
				{
					UserData * ud = USERSDATA + i;

					if ( ud->pcSocketData && ud->dwExclusiveNum == 0x100 )
					{
						if ( MAPSERVER->IsBellatraMapArea( ud->sPosition.iX, ud->sPosition.iY, ud->sPosition.iZ ) )
						{
							CHATSERVER->SendChat( USERDATATOUSER( ud ), CHATCOLOR_Global, "Bellatra> Your team failed to reach the points to the next round!" );
							SENDPACKET( USERDATATOUSER( ud ), &sPacket, TRUE );

							int iScore = GetRoomScore( ud->sBellatraRoomID );

							int iGold = iScore - ((iScore * EVENT_BELLATRA_TAX) / 100);

							int iUserRoomCount = GetUserRoomCount( ud->sBellatraRoomID );

							if ( iUserRoomCount == 0 )
								iUserRoomCount++;

							iGold /= iUserRoomCount;

							CHARACTERSERVER->GiveGOLD( USERDATATOUSER( ud ), iGold );

							CHATSERVER->SendChatMessageBox( USERDATATOUSER( ud ), "You won %s gp!", FormatNumber( iGold ) );
							CHATSERVER->SendChatEx( USERDATATOUSER( ud ), CHATCOLOR_Global, "Bellatra> You won %s gp!", FormatNumber( iGold ) );
						}
					}
				}
			}
		}

		if ( psBellatraData->saMonster[0].szName[0] == 0 )
		{
			switch ( rand() % 3 )
			{
				case 0:
					SetBellatraData( (BellatraData*)0x07AC4E98 );
					break;
				case 1:
					SetBellatraData( (BellatraData*)0x07AC46DC );
					break;
				case 2:
					SetBellatraData( (BellatraData*)0x07AC3F20 );
					break;
			}

			//Round 1
			SetRound( 0 );

			iRoomTopScore = 0;

			psBellatraData = GetBellatraData() + (GetRound() - 1);

			CopyMemory( GetBellatraMonster( 0 ), &psBellatraData->saMonster[0], sizeof( BellatraData::BellatraUnitData ) );
			CopyMemory( GetBellatraMonster( 1 ), &psBellatraData->saMonster[1], sizeof( BellatraData::BellatraUnitData ) );
			CopyMemory( GetBellatraMonster( 2 ), &psBellatraData->saMonster[2], sizeof( BellatraData::BellatraUnitData ) );
			CopyMemory( GetBellatraMonster( 3 ), &psBellatraData->saMonster[3], sizeof( BellatraData::BellatraUnitData ) );
			CopyMemory( GetBellatraMonster( 4 ), &psBellatraData->sBoss, sizeof( BellatraData::BellatraUnitData ) );

			SetMaxMonsterInRooms( psBellatraData->iMaxMonsters );

			ZeroMemory( BellatraServer::GetBossSpawnHandle(), sizeof( BellatraMonsterSpawnHandle ) );
			ZeroMemory( BellatraServer::GetMonsterSpawnHandle(), sizeof( BellatraMonsterSpawnHandle ) );
			(*(CharacterData**)0x07AC9B90) = psBellatraData->sBoss.psCharacterData;

			SetTimeout( TICKCOUNT + 30000 );

			WRITEDWORD( 0x07AB26B0, 0 );
			WRITEDWORD( 0x07AB2768, 0 );
			WRITEDWORD( 0x07AB276C, 0 );
			WRITEDWORD( 0x07AB2770, 0 );
			WRITEDWORD( 0x07AB2774, 0 );


			int iTotalScore = 0;
			int iUserCount = 0;

			SetUserRoomCount( 0, 0 );
			SetUserRoomCount( 1, 0 );
			SetUserRoomCount( 2, 0 );
			SetUserRoomCount( 3, 0 );

			std::vector<UserData *> vUsersRoom[4];
			BOOL bRoomInParty[4] = { FALSE, FALSE, FALSE, FALSE };

			for ( int i = 0; i < PLAYERS_MAX; i++ )
			{
				UserData * ud = USERSDATA + i;

				if ( ud->pcSocketData && ud->dwExclusiveNum == 0x100 )
				{
					if ( MAPSERVER->IsBellatraMapArea( ud->sPosition.iX, ud->sPosition.iY, ud->sPosition.iZ ) )
					{
						iTotalScore += ud->iBellatraScore;
						iUserCount++;

						SetUserRoomCount( ud->sBellatraRoomID, GetUserRoomCount( ud->sBellatraRoomID ) + 1 );

						if ( (ud->sBellatraRoomID >= 0) && (ud->sBellatraRoomID < 4) )
							vUsersRoom[ud->sBellatraRoomID].push_back( ud );
					}
				}
			}

			ZeroMemory( GetBellatraResultPacket(), sizeof( PacketBellatraResult ) );

			int iGold = 0;

			//Check Users in Party
			for ( int i = 0; i < 4; i++ )
			{
				bRoomInParty[i] = IsRoomInParty( vUsersRoom[i] );

				vUsersRoom[i].clear();
			}

			for ( int i = 0; i < PLAYERS_MAX; i++ )
			{
				UserData * ud = USERSDATA + i;

				User * pcUser = USERDATATOUSER( ud );

				if ( ud->pcSocketData && ud->dwExclusiveNum == 0x100 )
				{
					if ( MAPSERVER->IsBellatraMapArea( ud->sPosition.iX, ud->sPosition.iY, ud->sPosition.iZ ) )
					{
						int iUserRoomCount = GetUserRoomCount( ud->sBellatraRoomID );

						if ( iUserRoomCount == 0 )
							iUserRoomCount++;

						if ( ((ud->sBellatraRoomID >= 0) && (ud->sBellatraRoomID < 4)) && bRoomInParty[ud->sBellatraRoomID] )
						{
							int iScore = GetRoomScore( ud->sBellatraRoomID );
							iGold = iScore - ((iScore * EVENT_BELLATRA_TAX) / 100);
							iGold /= iUserRoomCount;
						}
						else
						{
							int iScore = ud->iBellatraScore;
							iGold = iScore - ((iScore * EVENT_BELLATRA_TAX) / 100);
						}

						PacketBellatraInformation s;
						s.iLength = sizeof( PacketBellatraInformation );
						s.iHeader = PKTHDR_BellatraInformation;
						s.iType = PacketBellatraInformation::BELLATRAINFOTYPE_GoldYahoo;
						s.iRound = GetRound() - 1;
						s.iValue1 = 100;
						s.iValue2 = iGold;
						SENDPACKET( USERDATATOUSER(ud), &s, TRUE );

						CHATSERVER->SendChatMessageBox( USERDATATOUSER( ud ), "You won %s gp!", FormatNumber( iGold ) );
						CHATSERVER->SendChatEx( USERDATATOUSER( ud ), CHATCOLOR_Global, "Bellatra> You won %s gp!", FormatNumber( iGold ) );

						PacketBellatraResult * ps = GetBellatraResultPacket();

						if ( ps->iResultCount < 32 && ud->bCloak == FALSE )
						{
							STRINGCOPY( ps->saResult[ps->iResultCount].szName, ud->sCharacterData.szName );
							ps->saResult[ps->iResultCount].pcUserData = ud;
							ps->saResult[ps->iResultCount].iClanID = ud->sCharacterData.iClanID;
							ps->saResult[ps->iResultCount].iClass = ud->sCharacterData.iClass;
							ps->saResult[ps->iResultCount].iScore = ud->iBellatraScore;
							ps->saResult[ps->iResultCount].iMonster = ud->iBellatraKills;
							ps->iResultCount++;
						}

						if ( iGold )
							CALL_WITH_ARG2( 0x005562A0, (DWORD)ud, iGold );

						BellatraServer::SendScoreUser( ud, ud->iBellatraScore, ud->iBellatraKills, iTotalScore, iUserCount, GetTotalUsers(), (*(int*)0x07AB1550) );


					}
					else
						BellatraServer::SendScoreUser( ud, 0, ud->iBellatraKills, 0, 0, GetTotalUsers(), (*(int*)0x07AB1550) );
				}
			}

			PacketBellatraResult * ps = GetBellatraResultPacket();
			ps->iLength = sizeof( PacketBellatraResult ) - ((32 - ps->iResultCount) * sizeof( PacketBellatraResult::BellatraResult ));
			ps->iUnk = (*(int*)0x07AB1550);
			ps->dwTime = READDWORD( 0x07AC9D6C );
			ps->bShow = TRUE;

			for ( int i = 0; i < ps->iResultCount; i++ )
			{
				UserData * ud = ps->saResult[i].pcUserData;

				if ( ud && ud->pcSocketData )
					SENDPACKET( USERDATATOUSER(ud), ps, TRUE );
			}

			ps->bShow = FALSE;

			return TRUE;
		}

		CopyMemory( GetBellatraMonster( 0 ), &psBellatraData->saMonster[0], sizeof( BellatraData::BellatraUnitData ) );
		CopyMemory( GetBellatraMonster( 1 ), &psBellatraData->saMonster[1], sizeof( BellatraData::BellatraUnitData ) );
		CopyMemory( GetBellatraMonster( 2 ), &psBellatraData->saMonster[2], sizeof( BellatraData::BellatraUnitData ) );
		CopyMemory( GetBellatraMonster( 3 ), &psBellatraData->saMonster[3], sizeof( BellatraData::BellatraUnitData ) );
		CopyMemory( GetBellatraMonster( 4 ), &psBellatraData->sBoss, sizeof( BellatraData::BellatraUnitData ) );

		SetMaxMonsterInRooms( psBellatraData->iMaxMonsters );

		(*(CharacterData**)0x07AC9B90) = psBellatraData->sBoss.psCharacterData;

		int * piaRoomKills = ((int*)pfnRoomScoreKill);

		ZeroMemory( BellatraServer::GetMonsterSpawnHandle(), sizeof( BellatraMonsterSpawnHandle ) );

		ZeroMemory( BellatraServer::GetBossSpawnHandle(), sizeof( BellatraMonsterSpawnHandle ) );

		if ( GetBellatraMonster( 0 )->psCharacterData )
		{
			SetDelayTime( TICKCOUNT + (psBellatraData->iDelayTime * 1000) );

			sPacket1.iLength = sizeof( PacketBellatraInformation );
			sPacket1.iHeader = PKTHDR_BellatraInformation;
			sPacket1.iType = PacketBellatraInformation::BELLATRAINFOTYPE_StageComplete;
			sPacket1.iRound = GetRound() - 1;
			sPacket1.iValue1 = -1;
			sPacket1.iValue2 = 0;

			PacketBellatraInformation sPacket2;
			sPacket2.iLength = sizeof( PacketBellatraInformation );
			sPacket2.iHeader = PKTHDR_BellatraInformation;
			sPacket2.iType = PacketBellatraInformation::BELLATRAINFOTYPE_StageFail;
			sPacket2.iRound = GetRound() - 1;
			sPacket2.iValue1 = -1;
			sPacket2.iValue2 = 0;

			//Set Team Rank
			CALL( 0x0054DA30 );

			if ( GetRound() == 4 || GetRound() == 7 )
			{
				std::vector<UserData *> vUsersRoom[4];
				BOOL bRoomInParty[4] = { FALSE, FALSE, FALSE, FALSE };

				if ( GetRound() == 4 )
				{
					SetUserRoomCount( 0, 0 );
					SetUserRoomCount( 1, 0 );
					SetUserRoomCount( 2, 0 );
					SetUserRoomCount( 3, 0 );

					for ( int i = 0; i < PLAYERS_MAX; i++ )
					{
						UserData * ud = USERSDATA + i;

						if ( ud->pcSocketData && ud->dwExclusiveNum == 0x100 )
						{
							if ( MAPSERVER->IsBellatraMapArea( ud->sPosition.iX, ud->sPosition.iY, ud->sPosition.iZ ) )
							{
								SetUserRoomCount( ud->sBellatraRoomID, GetUserRoomCount( ud->sBellatraRoomID ) + 1 );

								if ( (ud->sBellatraRoomID >= 0) && (ud->sBellatraRoomID < 4) )
									vUsersRoom[ud->sBellatraRoomID].push_back( ud );
							}
						}
					}
				}

				//Check Users in Party
				for ( int i = 0; i < 4; i++ )
				{
					bRoomInParty[i] = IsRoomInParty( vUsersRoom[i] );

					vUsersRoom[i].clear();
				}

				for ( int i = 0; i < PLAYERS_MAX; i++ )
				{
					UserData * ud = USERSDATA + i;

					if ( ud->pcSocketData && ud->dwExclusiveNum == 0x100 )
					{
						if ( MAPSERVER->IsBellatraMapArea( ud->sPosition.iX, ud->sPosition.iY, ud->sPosition.iZ ) )
						{
							if ( ud->sBellatraRoomID == GetRankRoomID( 0 ) || (ud->sBellatraRoomID == GetRankRoomID( 1 ) && GetRound() == 4) )
							{
								iRoomTopScore = GetRankRoomID( 0 );

								if ( ud->sBellatraRoomID == GetRankRoomID( 0 ) )
									sPacket1.iValue1 = iaBellatraRoundRoomTable[0][GetRound() - 1];
								else
									sPacket1.iValue1 = iaBellatraRoundRoomTable[2][GetRound() - 1];

								SENDPACKET( USERDATATOUSER(ud), &sPacket1, TRUE );
							}
							else
							{
								SENDPACKET( USERDATATOUSER(ud), &sPacket2, TRUE );

								BellatraServer::SendScoreUser( ud, -ud->iBellatraScore, ud->iBellatraKills, 0, 0, GetTotalUsers(), (*(int*)0x07AB1550) );

								int iUserRoomCount = GetUserRoomCount( ud->sBellatraRoomID );

								if ( iUserRoomCount == 0 )
									iUserRoomCount++;

								int iGold = 0;

								if ( ((ud->sBellatraRoomID >= 0) && (ud->sBellatraRoomID < 4)) && bRoomInParty[ud->sBellatraRoomID] )
								{
									int iScore = GetRoomScore( ud->sBellatraRoomID );
									iGold = iScore - ((iScore * EVENT_BELLATRA_TAX) / 100);
									iGold /= iUserRoomCount;
								}
								else
								{
									int iScore = ud->iBellatraScore;
									iGold = iScore - ((iScore * EVENT_BELLATRA_TAX) / 100);
								}


								if ( iGold )
									CALL_WITH_ARG2( 0x005562A0, (DWORD)ud, iGold );

								ud->iBellatraScore = 0;
								ud->iBellatraKills = 0;
								ud->dwExclusiveNum = 0;

								int iRank = 1;
								if ( ud->sBellatraRoomID == GetRankRoomID( 1 ) )
									iRank = 2;
								if ( ud->sBellatraRoomID == GetRankRoomID( 2 ) )
									iRank = 3;
								if ( ud->sBellatraRoomID == GetRankRoomID( 3 ) )
									iRank = 4;

								PacketChatBoxMessage sChatBox;
								sChatBox.iHeader = PKTHDR_ChatMessageBox;
								sChatBox.iChatColor = CHATCOLOR_Error;
								sChatBox.lID = 0;
								STRINGFORMAT( sChatBox.szChatBoxMessage, "Rank: %d, Gold: %s gp", iRank, FormatNumber( iGold ) );
								sChatBox.iLength = STRLEN( sChatBox.szChatBoxMessage ) + 32;
								SENDPACKET( USERDATATOUSER(ud), &sChatBox, TRUE );

								CHATSERVER->SendChatEx( USERDATATOUSER( ud ), CHATCOLOR_Global, "Bellatra> Rank: %d, Gold: %s gp", iRank, FormatNumber( iGold ) );
							}
						}
					}
				}
				sPacket1.iLength = 0;
			}
		}


		for ( int i = 0; i < PLAYERS_MAX; i++ )
		{
			UserData * ud = USERSDATA + i;

			if ( ud->pcSocketData && ud->dwExclusiveNum == 0x100 )
			{
				if ( MAPSERVER->IsBellatraMapArea( ud->sPosition.iX, ud->sPosition.iY, ud->sPosition.iZ ) )
				{
					if ( sPacket1.iLength )
						SENDPACKET( USERDATATOUSER(ud), &sPacket1, TRUE );
				}
				else
				{
					ud->dwExclusiveNum = 0;
					BellatraServer::SendScoreUser( ud, 0, ud->iBellatraKills, 0, 0, GetTotalUsers(), (*(int*)0x07AB1550) );
				}
			}
		}

		SetRound( GetRound() );

		return FALSE;
	}

	if ( GetRound() == 1 )
	{
		if ( GetTimeout() && GetTimeout() < TICKCOUNT )
		{
			for ( int i = 0; i < PLAYERS_MAX + 256; i++ )
			{
				UserData * ud = USERSDATA + i;

				if ( ud->dwExclusiveNum == 0x100 )
				{
					ud->iBellatraScore = 0;
					ud->iBellatraKills = 0;
					ud->dwExclusiveNum = 0;

					if ( MAPSERVER->IsBellatraMapArea( ud->sPosition.iX, ud->sPosition.iY, ud->sPosition.iZ ) )
					{
						if ( ud->pcSocketData && i < PLAYERS_MAX )
						{
							Map * psNaviskoMap = MAPSDATA + MAPID_NaviskoTown;
							PacketWarpGateField sPacket;
							sPacket.iLength = sizeof( PacketWarpGateField );
							sPacket.iHeader = PKTHDR_WarpGateField;
							sPacket.iX = psNaviskoMap->pcBaseMap->GetCenter().iX << 8;
							sPacket.iZ = psNaviskoMap->pcBaseMap->GetCenter().iY << 8;
							sPacket.iStage = MAPID_NaviskoTown;
							SENDPACKET( USERDATATOUSER(ud), &sPacket );
						}
					}
				}
			}

			SetTotalUsers( 0 );
			SetTimeout( 0 );

			if ( (*(BOOL*)0x07AC9E2C) == FALSE )
				WRITEDWORD( 0x07AC9E30, TRUE );

			(*(int*)0x07AB1550) = (((*(DWORD*)0x07AC9D1C) << 24) | (((*(DWORD*)0x07AC9D6C) & 0xFFFFFF)));
			WRITEDWORD( 0x07AC9BB8, 0 );
			WRITEDWORD( 0x07AC9BBC, 0 );
			WRITEDWORD( 0x07AC9BC0, 0 );
			WRITEDWORD( 0x07AC9BC4, 0 );

			ZeroMemory( (void*)0x07AC57F0, 0x2D0 * 4 );
			WRITEDWORD( 0x07AB3074, 0 );
		}

		return FALSE;
	}

	if ( GetTimeUpdate() < TICKCOUNT )
	{
		CALL_WITH_ARG1( 0x0054DAB0, 0 );
		SetTimeUpdate( TICKCOUNT + 1000 );
	}

	int iRoundCount = GetRound() - 2;

	if ( iRoundCount < 0 || iRoundCount >= 8 || GetDelayTime() > TICKCOUNT )
		return FALSE;


	int * piaRoomKills = ((int*)pfnRoomScoreKill);

	for ( int i = iaBellatraRoundTable[iRoundCount][0]; i < iaBellatraRoundTable[iRoundCount][1]; i++ )
	{
		int iCount = piaRoomKills[i];

		if ( iCount > GetMaxMonsterInRooms() )
			continue;

		if ( (*(CharacterData**)0x07AC9B90) && (baBossSpawnSOD[i] == FALSE) && GetMonsterSpawnHandle()->saRound[i].iCount > (GetMaxMonsterInRooms() + 5) )
		{
			CopyMemory( &psMonsterData->sCharacterData, (*(CharacterData**)0x07AC9B90), sizeof( CharacterData ) );
			GetBossSpawnHandle()->saRound[i].iCount++;
			baBossSpawnSOD[i] = TRUE;
		}
		else
		{
			for ( int j = 0; j < 4; j++ )
			{
				int iChance = rand() % 100;
				if ( GetBellatraMonster( j )->psCharacterData && iChance <= GetBellatraMonster( j )->iPercent )
				{
					CopyMemory( &psMonsterData->sCharacterData, GetBellatraMonster( j )->psCharacterData, sizeof( CharacterData ) );
					break;
				}
			}
		}

		int iX, iY, iZ;

		CALL_WITH_ARG4( 0x0054D9B0, (DWORD)&iX, (DWORD)&iY, (DWORD)&iZ, i );

		psMonsterData->iPositionState = 1;

		psMonsterData->sCharacterData.iType = ECharacterType::CHARACTERTYPE_Monster;

		pcStage->bForceCreateMonster = TRUE;

		UnitData * pcUnitData = MAPSERVER->CreateUnit( pcStage, psMonsterData, 0 );
		if ( pcUnitData )
		{
			char szName[32] = { 0 };
			STRINGCOPY( szName, pcUnitData->GetName() );
		
			int iLength = STRLEN( szName );

			//_SOD Fix
			if ( iLength > 4 )
				szName[iLength - 4] = 0;

			ZeroMemory( pcUnitData->sCharacterData.szName, 32 );
			STRINGCOPY( pcUnitData->sCharacterData.szName, szName );

			pcUnitData->sPosition = Point3D( iX, iY, iZ );

			pcUnitData->Animate( ANIMATIONTYPE_Idle );
			pcUnitData->iTargetMoveCount = 0;
			pcUnitData->dwExclusiveNum = 0x100;
			pcUnitData->iBellatraRoom = i;

			pcUnitData->sSpawnPosition = saBellatraSpawnPoints[i][2];
			pcUnitData->sSpawnPosition <<= 8;
			pcUnitData->iLureDistance = (12 * 64 * 12 * 64);

			pcUnitData->sCharacterData.iSight = (1300 * 1300);

			GetMonsterSpawnHandle()->saRound[i].iCount++;
			piaRoomKills[i]++;
		}
	}

	return FALSE;
}

void BellatraServer::SQLBellatraRewardLog( char * pszAccountName, char * pszName, char * pszItemName, int iQuantity )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "INSERT INTO BellatraRewardLog([AccountName],[Name],[ItemName],[Quantity],[Date]) VALUES (?,?,?,?,GETDATE())" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, pszAccountName, 32 );
			pcDB->BindParameterInput( 2, PARAMTYPE_String, pszName, 32 );
			pcDB->BindParameterInput( 3, PARAMTYPE_String, pszItemName, 32 );
			pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &iQuantity );
			pcDB->Execute();
		}
		pcDB->Close();
	}
}

void BellatraServer::HandlePacket( User * pcUser, PacketReceiveItemIhin * psPacket )
{
	if ( (psPacket->iCount - 80) >= 15 )
	{
		UserData * ud = pcUser->pcUserData;
		if ( ud && ud->pcSocketData )
		{
			SPrizeData * ps = GetPrizeBellatraRewardItem();
				
			if ( ps->bActive )
			{
				char szItemName[32] = { 0 };
				if ( ITEMSERVER->FindItemName( ps->szCode, szItemName ) )
				{
					USERSERVER->SetUserID( ud );

					ITEMSERVER->AddItemOpenBox( ud, ps->szCode, 0, ps->iQuantity );
					SQLBellatraRewardLog( ud->szAccountName, CHARACTERSERVER->GetCharacterName( ud ), ps->szCode, ps->iQuantity );

					if ( STRINGCOMPAREI("GG101", ps->szCode ) )
						CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Global, "Bellatra> You won %s gp!", FormatNumber( ps->iQuantity ) );
					else
						CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Global, "Bellatra> You won %s[%s]!", szItemName, FormatNumber( ps->iQuantity ) );

					CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "Bellatra> Check Item Distributor to get your prize!" );
				}
			}
		}
	}
}

SPrizeData * BellatraServer::GetPrizeBellatraRewardItem()
{
	int iRand = (rand() << 7) | ((GetCurrentTime() >> 2) & 0x7F);
	iRand = iRand % iTotalPercent;
	int iTotal = 0;
	for ( int i = 0; i < 50; i++ )
	{
		SPrizeData * p = saPrizes + i;
		if ( p->bActive )
		{
			if ( (iRand >= iTotal) && (iRand < (iTotal + p->iPercent)) )
				return p;

			iTotal += p->iPercent;
		}
	}
	return &saPrizes[0];
}


void BellatraServer::Init()
{
	if ( GAME_SERVER )
	{
		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
		if ( pcDB->Open() )
		{

			if ( pcDB->Prepare( "SELECT ItemCode, Quantity, [Percent] FROM BellatraReward ORDER BY [Percent] DESC" ) )
			{
				if ( pcDB->Execute() )
				{
					int i = 0;
					while ( pcDB->Fetch() )
					{
						SPrizeData * p = saPrizes + i;
						pcDB->GetData( 1, PARAMTYPE_String, p->szCode, 32 );
						pcDB->GetData( 2, PARAMTYPE_Integer, &p->iQuantity );
						pcDB->GetData( 3, PARAMTYPE_Integer, &p->iPercent );
						p->bActive = TRUE;
						iTotalPercent += p->iPercent;
						i++;
					}
				}
			}
			pcDB->Close();
		}
	}
}

BOOL BellatraServer::IsKilledUnitSameRoom( UserData * pcUserData, UnitData * pcUnitData )
{
	BOOL bRet = TRUE;

	User * pcUser = USERDATATOUSER( pcUserData );

	if ( (pcUserData->sBellatraRoomID >= 0) && (pcUserData->sBellatraRoomID < 4) )
	{
		int iRound = GetRound() - 1;
		if ( iRound < 1 )
			iRound = 1;

		int iRoomID = pcUserData->sBellatraRoomID;

		if ( (iRound >= 4) && (iRound <= 6) )
		{
			if ( pcUserData->sBellatraRoomID == iRoomTopScore )
				iRoomID = 4;
			else
				iRoomID = 5;
		}
		else if ( iRound >= 7 )
			iRoomID = 6;

		if ( pcUnitData->iBellatraRoom != iRoomID )
			bRet = FALSE;

		if ( pcUser->bDebugChat )
		{
			CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> UR[%d] MR[%d] RoomID[%d][%d][%d]",
				pcUserData->sBellatraRoomID, pcUnitData->iBellatraRoom, iRoomID, iRound, GetRankRoomID( 0 ) );
		}
	}

	return bRet;
}

BOOL BellatraServer::SpawnRabieBellatra( UserData * pcUserData, int iSpawnID, Map * pcMap )
{
	CharacterData * psCharacterDataRabie		= UNITSERVER->GetCharacterDataByName( "Rabie" );
	CharacterData * psCharacterDataMutantRabie	= UNITSERVER->GetCharacterDataByName( "Mutant Rabie" );
	if ( psCharacterDataRabie && psCharacterDataMutantRabie )
	{

	}

	return 0;
}

void BellatraServer::HandlePacket( User * pcUser, PacketBellatraRequest * psPacket )
{
	if ( GAME_SERVER )
		return;

	//Check for gold
	if ( !CHARACTERSERVER->GiveGOLD( pcUser, -psPacket->iGold, WHEREID_GiveMoney ) )
	{
		//Log it...
		PacketLogCheat sPacket;
		sPacket.iCheatID = CHEATLOGID_BellatraRequestGoldError;
		sPacket.LParam = -psPacket->iGold;
		LOGSERVER->OnLogCheat( pcUser->pcUserData, &sPacket );
	}
	else
	{
		//Send to GameServer
		NETSERVER->SendBellatraRequest( pcUser, psPacket->iGold );
	}
}

void BellatraServer::HandleNetPacket( PacketNetBellatraRequest * psPacket )
{
	if ( User * pcUser = USERDATATOUSER( USERDATABYID( psPacket->iID ) ) )
		RequestSod( pcUser->pcUserData, psPacket->iGold );
}

void BellatraServer::HandleNetPacket( PacketNetBellatraGiveGold * psPacket )
{
	if ( User * pcUser = USERDATATOUSER( USERDATABYID( psPacket->iID ) ) )
		CHARACTERSERVER->GiveGOLD( pcUser, psPacket->iGold );
}

BOOL BellatraServer::IsRoomInParty( std::vector<UserData*> & vUsersRoom )
{
	BOOL bRet = TRUE;

	if ( vUsersRoom.size() > 0 )
	{
		UserData * ud = vUsersRoom[0];
		User * pcUser = USERDATATOUSER( ud );
		if ( pcUser )
		{
			if ( pcUser->bParty && pcUser->psParty && (pcUser->psParty->iMembersCount > 0) && (vUsersRoom.size() == (UINT)pcUser->psParty->iMembersCount) )
			{
				std::vector<User *> vUsersParty = PARTYHANDLER->GetPartyMembers( pcUser->psParty );
				for ( std::vector<User *>::iterator it = vUsersParty.begin(); it != vUsersParty.end(); it++ )
				{
					BOOL bInParty = FALSE;

					User * pcUserParty = (*it);
					if ( pcUserParty )
					{
						for ( UINT i = 0; i < vUsersRoom.size(); i++ )
						{
							if ( vUsersRoom[i] && (vUsersRoom[i] == pcUserParty->pcUserData) )
							{
								bInParty = TRUE;
								break;
							}
						}
					}

					if ( bInParty == FALSE )
					{
						bRet = FALSE;
						break;
					}
				}
			}
		}
	}

	return bRet;
}

int BellatraServer::GetRoomTopScore()
{
	int * piaRoomScore = ((int*)pfnRoomScoreData);

	int iRoomID = 0;
	int iLastScore = 0;
	for ( int i = 0; i < 4; i++ )
	{
		if ( piaRoomScore[i] > iLastScore )
		{
			iLastScore = piaRoomScore[i];
			iRoomID = i;
		}
	}

	return iRoomID;
}

bool BellatraServer::IsEventTime()
{


	return false;
}

NAKED BOOL BellatraServer::RequestSod( UserData * pcUserData, int iGold )
{
	JMP( 0x00560310 );
}

