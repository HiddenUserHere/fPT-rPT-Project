#include "stdafx.h"
#include "CActionFieldHandler.h"

CActionFieldInstance::CActionFieldInstance()
{
	iTimeUpdate				= 0;
	iTimeFinished			= 0;

	bStarted				= FALSE;
	bDelete					= FALSE;
	
	pcActionField			= NULL;
	pcRoom					= NULL;

	iRound					= 0;

	iMonstersKilled			= 0;
	iMonstersKilledRound	= 0;
	iMonstersAlive			= 0;

	bSpawnBoss				= FALSE;

	bSentPrizes				= FALSE;
}

CActionFieldInstance::~CActionFieldInstance()
{
}

void CActionFieldInstance::SetMonstersAlive( int i )
{
	iMonstersAlive = i; 

	if ( iMonstersAlive < 0 )
		iMonstersAlive = 0;
}


CActionFieldHandler::CActionFieldHandler()
{
	iActionFieldWheel = 1;
}


CActionFieldHandler::~CActionFieldHandler()
{
	for ( std::vector<CActionField*>::iterator it = vActionFields.begin(); it != vActionFields.end(); it++ )
	{
		CActionField * pc = (*it);
		DELET( pc );
	}
	vActionFields.clear();
}

CActionFieldInstance * CActionFieldHandler::GetInstanceRoom( CRoomInstance * pcRoom )
{
	if ( pcRoom == NULL )
		return nullptr;

	for ( std::vector<CActionFieldInstance*>::iterator it = vInstances.begin(); it != vInstances.end(); it++ )
	{
		CActionFieldInstance * pc = (*it);
		if ( pc->GetRoom() == pcRoom )
			return pc;
	}
	return nullptr;
}

void CActionFieldHandler::Init()
{
	LoadActionField( ACTIONFIELDID_GhostCastle1 );
	LoadActionField( ACTIONFIELDID_GhostCastle2 );
	LoadActionField( ACTIONFIELDID_ForestDungeon1 );
	LoadActionField( ACTIONFIELDID_ForestDungeon2 );
	LoadActionField( ACTIONFIELDID_ForestDungeon3 );
	LoadActionField( ACTIONFIELDID_DragonsDungeon1 );
	LoadActionField( ACTIONFIELDID_DragonsDungeon2 );
	LoadActionField( ACTIONFIELDID_DragonsDungeon3 );
}

void CActionFieldHandler::Delete( EActionFieldModeID iModeID )
{
	for ( std::vector<CActionField*>::iterator it = vActionFields.begin(); it != vActionFields.end(); )
	{
		CActionField * pc = (*it);
		if ( pc->GetModeID() == iModeID )
		{
			DELET( pc );
			it = vActionFields.erase( it );
		}
		else
			it++;
	}
}

BOOL CActionFieldHandler::SQLRewardLog( EActionFieldModeID iModeID, int iChestTypeID, User * pcUser, ActionFieldReward * psReward )
{
	BOOL bRet = FALSE;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "INSERT INTO ActionFieldRewardLog([ModeID],[ChestTypeID],[AccountID],[CharacterID],[ItemCode],[Quantity],[Date]) VALUES(?,?,?,?,?,?,GETDATE())" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iModeID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iChestTypeID );
			pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &pcUser->iAccountID );
			pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &pcUser->iCharacterID );
			pcDB->BindParameterInput( 5, PARAMTYPE_String, psReward->szItemCode, STRLEN( psReward->szItemCode ) );
			pcDB->BindParameterInput( 6, PARAMTYPE_Integer, &psReward->iQuantity );

			bRet = pcDB->Execute();
		}
		pcDB->Close();
	}

	return bRet;
}

BOOL CActionFieldHandler::SQLInstanceLog( EActionFieldModeID iModeID, User * pcUser, const char * pszDescription )
{
	BOOL bRet = FALSE;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "INSERT INTO ActionFieldLog([ModeID],[AccountID],[CharacterID],[IP],[Description],[Date]) VALUES(?,?,?,?,?,GETDATE())" ) )
		{
			char * pszIP = pcUser->pcUserData->pcSocketData ? pcUser->pcUserData->pcSocketData->szIP : "";

			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iModeID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &pcUser->iAccountID );
			pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &pcUser->iCharacterID );
			pcDB->BindParameterInput( 4, PARAMTYPE_String, pszIP, STRLEN( pszIP ) );
			pcDB->BindParameterInput( 5, PARAMTYPE_String, (char *)pszDescription, STRLEN( pszDescription ) );

			bRet = pcDB->Execute();
		}
		pcDB->Close();
	}

	return bRet;
}

BOOL CActionFieldHandler::SQLGetInstance( User * pcUser, SQLActionFieldInstance * psInstance )
{
	BOOL bRet = FALSE;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT ID, InstanceID, ModeID, AccountID, CharacterClass, IP, [Date], TotalScore, TotalKills, LastRound FROM CharacterActionFieldInstance WHERE CharacterID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );
			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_Integer, &psInstance->iID );
				pcDB->GetData( 2, PARAMTYPE_Integer, &psInstance->iInstanceID );
				pcDB->GetData( 3, PARAMTYPE_Integer, &psInstance->iModeID );
				pcDB->GetData( 4, PARAMTYPE_Integer, &psInstance->iAccountID );
				pcDB->GetData( 5, PARAMTYPE_Integer, &psInstance->iCharacterClass );
				pcDB->GetData( 6, PARAMTYPE_String, psInstance->szIP, _countof( psInstance->szIP ) );
				pcDB->GetData( 7, PARAMTYPE_Time, &psInstance->sDate, sizeof( SYSTEMTIME ) );
				pcDB->GetData( 8, PARAMTYPE_Integer, &psInstance->iTotalScore );
				pcDB->GetData( 9, PARAMTYPE_Integer, &psInstance->iTotalKills );
				pcDB->GetData( 10, PARAMTYPE_Integer, &psInstance->iLastRound );

				psInstance->iCharacterID = pcUser->iCharacterID;

				bRet = TRUE;
			}
		}
		pcDB->Close();
	}

	return bRet;
}

EMapID CActionFieldHandler::GetMapID( EActionFieldModeID iModeID )
{
	if ( (iModeID == ACTIONFIELDID_GhostCastle1) || (iModeID == ACTIONFIELDID_GhostCastle2) )
		return MAPID_GhostCastle;
	else if ( iModeID == ACTIONFIELDID_Swamp )
		return MAPID_Swamp;
	else if ( (iModeID == ACTIONFIELDID_ForestDungeon1) || (iModeID == ACTIONFIELDID_ForestDungeon2) || (iModeID == ACTIONFIELDID_ForestDungeon3) )
		return MAPID_ForestDungeon;
	else if ( (iModeID == ACTIONFIELDID_DragonsDungeon1) || (iModeID == ACTIONFIELDID_DragonsDungeon2) || (iModeID == ACTIONFIELDID_DragonsDungeon3) )
		return MAPID_DragonsDungeon;

	return MAPID_DistortedForest;
}

bool CActionFieldHandler::GetEntranceXZ( EActionFieldModeID iModeID, Point3D & sEntrance )
{
	bool bRet = false;

	sEntrance = Point3D( 0, 0, 0 );
	
	if ( (iModeID == ACTIONFIELDID_GhostCastle1) || (iModeID == ACTIONFIELDID_GhostCastle2) )
	{
		sEntrance.iX = 5586 << 8;
		sEntrance.iZ = 36744 << 8;

		bRet = true;
	}
	else if ( (iModeID == ACTIONFIELDID_ForestDungeon1) || (iModeID == ACTIONFIELDID_ForestDungeon2) || (iModeID == ACTIONFIELDID_ForestDungeon3) )
	{
		sEntrance.iX = -109346 << 8;
		sEntrance.iZ = 27588 << 8;

		bRet = true;
	}
	else if ( (iModeID == ACTIONFIELDID_DragonsDungeon1) || (iModeID == ACTIONFIELDID_DragonsDungeon2) || (iModeID == ACTIONFIELDID_DragonsDungeon3) )
	{
		sEntrance.iX = -199874 << 8;
		sEntrance.iZ = 199376 << 8;

		bRet = true;
	}

	return bRet;
}

CActionField * CActionFieldHandler::GetActionField( EActionFieldModeID iModeID )
{
	for ( std::vector<CActionField*>::iterator it = vActionFields.begin(); it != vActionFields.end(); it++ )
	{
		CActionField * pc = (*it);
		if ( pc->GetModeID() == iModeID )
			return pc;
	}
	return nullptr;
}

int CActionFieldHandler::SQLGetInstanceTodayCount( User * pcUser, EActionFieldModeID iModeID )
{
	int iRet = 0;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT COUNT(ID) FROM CharacterActionFieldInstance WHERE (ModeID=?) AND (CharacterID=?) AND (YEAR([Date])=?) AND (MONTH([Date])=?) AND (DAY([Date])=?)" ) )
		{
			SYSTEMTIME * psServerTime = GetServerTime();
			int iYear	= (int)psServerTime->wYear;
			int iMonth	= (int)psServerTime->wMonth;
			int iDay	= (int)psServerTime->wDay;

			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iModeID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &pcUser->iCharacterID );
			pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &iYear );
			pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &iMonth );
			pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &iDay );
			if ( pcDB->Execute() && pcDB->Fetch() )
				pcDB->GetData( 1, PARAMTYPE_Integer, &iRet );
		}
		pcDB->Close();
	}

	return iRet;
}

void CActionFieldHandler::UpdateMonsterSpawn( CActionFieldInstance * pcInstance )
{
	ActionFieldRound * psRound = pcInstance->GetActionField()->GetRound( pcInstance->GetRound() );
	if ( psRound )
	{
		//Time to update monsters count?
		if ( (pcInstance->GetMonstersAlive() < MAX_ACTIONFIELD_UNITS_SPAWN) && (pcInstance->GetRoundKills() < psRound->iKillsNeeded) )
		{
			//Seed to random spawn
			int iSeed = RandomI( 0, 5000 + pcInstance->GetMonstersAlive() );

			int iMonsterAmount = (MAX_ACTIONFIELD_UNITS_SPAWN - pcInstance->GetMonstersAlive());
			for ( int i = 0; i < iMonsterAmount; )
			{
				//Get Monster Data
				ActionFieldMonster * psMonster = psRound->saMonsters + ((iSeed + i) % psRound->iMonsterCount);
				if ( psMonster )
				{
					//Boss? Ignore...
					if ( psMonster->bIsBoss )
					{
						iSeed++;
						continue;
					}

					//Spawn
					SpawnMonster( pcInstance, psMonster, i );
				}

				//Next
				i++;
			}
		}

		//Boss Spawn Time?
		if ( pcInstance->GetBossSpawn() )
		{
			//Get Monster Data
			ActionFieldMonster * psMonster = NULL;
			std::vector<ActionFieldMonster*> vMonsters;
			for ( int i = 0; i < psRound->iMonsterCount; i++ )
			{
				ActionFieldMonster * ps = psRound->saMonsters + i;
				if ( ps->bIsBoss )
				{
					//Set Boss Monster Type
					vMonsters.push_back( ps );
				}
			}

			if ( vMonsters.size() > 0 )
				psMonster = vMonsters[rand() % vMonsters.size()];

			vMonsters.clear();

			if ( psMonster )
			{
				//Spawn
				SpawnMonster( pcInstance, psMonster );
			}

			//Set Spawn Boss off...
			pcInstance->SetBossSpawn( FALSE );
		}
	}
}

void CActionFieldHandler::UpdatePrizes( CActionFieldInstance * pcInstance )
{
	//Finished?
	if ( (pcInstance->iTimeFinished != 0) && (pcInstance->IsSentPrizes() == FALSE) )
	{
		//Get Chest to send
		CActionField * pcActionField = pcInstance->GetActionField();
		if ( pcActionField->GetChests().size() > 0 )
		{
			//Get Random Chest
			ActionFieldChest * psChest = pcActionField->GetChests()[rand() % pcActionField->GetChests().size()];

			std::vector<ActionFieldUser> vUsersAC;

			//Send Keys
			std::vector<User*> & vUsers = ROOMINSTANCEHANDLER->GetUsersRoom( pcInstance->GetRoom()->GetID() );
			for ( auto pcUser : vUsers )
			{
				//Add Item
				ITEMSERVER->AddItemOpenBox( pcUser->pcUserData->szAccountName, psChest->szItemCode, 0, 1 );

				LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_ActionFieldChestReceived, "Character[%s] Received Action Field Chest Item[%s] Quantity[%d]",
					CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), psChest->szItemCode, 1 );

				//Dance
				SENDPACKETBLANK( pcUser, PKTHDR_PacketFun );

				//Notify User
				CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Global, "%s> You won a chest item! Check the Item Distributor!", pcActionField->GetName() );
				CHATSERVER->SendChatMessageBox( pcUser, "You won a chest item! Check the Item Distributor!" );

				ActionFieldUser sUserAC;
				sUserAC.iID = pcUser->GetID();
				sUserAC.iKills = pcUser->iActionFieldKills;
				sUserAC.iScore = pcUser->iActionFieldScore;
				vUsersAC.push_back( sUserAC );
			}

			//Give EXP
			auto PointsShort = []( ActionFieldUser * psaUsers, int iCount ) -> void
			{
				auto Swap = []( ActionFieldUser * psUser1, ActionFieldUser * psUser2 ) -> void
				{
					ActionFieldUser sTemp;
					CopyMemory( &sTemp, psUser1, sizeof( ActionFieldUser ) );
					CopyMemory( psUser1, psUser2, sizeof( ActionFieldUser ) );
					CopyMemory( psUser2, &sTemp, sizeof( ActionFieldUser ) );
				};

				BOOL bSwapped = FALSE;
				for ( int i = 0; i < iCount - 1; i++ )
				{
					bSwapped = FALSE;
					for ( int j = 0; j < iCount - i - 1; j++ )
					{
						if ( psaUsers[j].iScore < psaUsers[j + 1].iScore )
						{
							Swap( &psaUsers[j], &psaUsers[j + 1] );
							bSwapped = TRUE;
						}
					}

					if ( bSwapped == FALSE )
						break;
				}
			};

			if ( vUsersAC.size() > 0 )
			{
				PointsShort( vUsersAC.data(), vUsersAC.size() );

				for ( UINT i = 0; i < vUsersAC.size(); i++ )
				{
					auto & sUser = vUsersAC[i];
					for ( auto pcUser : vUsers )
					{
						if ( sUser.iID == pcUser->GetID() )
						{
							if ( pcUser->pcUserData->sCharacterData.iLevel < SERVER_LEVEL_MAX )
							{
								INT64 iEXP = 50000000000 - ((INT64)i * 1000000000);
								if ( pcInstance->GetActionField()->GetModeID() == ACTIONFIELDID_GhostCastle2 )
									iEXP = 80000000000 - ((INT64)i * 1000000000);
								else if ( pcInstance->GetActionField()->GetModeID() == ACTIONFIELDID_ForestDungeon1 )
									iEXP = 50000000000;
								else if ( pcInstance->GetActionField()->GetModeID() == ACTIONFIELDID_ForestDungeon2 )
									iEXP = 70000000000;
								else if ( pcInstance->GetActionField()->GetModeID() == ACTIONFIELDID_ForestDungeon3 )
									iEXP = 90000000000;
								else if ( pcInstance->GetActionField()->GetModeID() == ACTIONFIELDID_DragonsDungeon1 )
									iEXP = 60000000000;
								else if ( pcInstance->GetActionField()->GetModeID() == ACTIONFIELDID_DragonsDungeon2 )
									iEXP = 80000000000;
								else if ( pcInstance->GetActionField()->GetModeID() == ACTIONFIELDID_DragonsDungeon3 )
									iEXP = 100000000000;

								//Give EXP
								CHARACTERSERVER->GiveEXP( pcUser, iEXP );

								//Notfy User
								CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Global, "%s> You received %s EXP!", pcActionField->GetName(), FormatNumber( iEXP ) );
								CHATSERVER->SendChatMessageBox( pcUser, "You received %s EXP!", FormatNumber( iEXP ) );

								//Log
								LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_ActionFieldRewardEXP, "Character[%s] Received Action Field EXP[%s]",
														CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), FormatNumber( iEXP ) );
							}
							else
							{
								int iGold = 5000000;
								if ( pcInstance->GetActionField()->GetModeID() == ACTIONFIELDID_ForestDungeon3 )
									iGold = 10000000;

								NETSERVER->SendBellatraGiveGold( pcUser, iGold );

								//Notfy User
								CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Global, "%s> You received %s gp!", pcActionField->GetName(), FormatNumber( iGold ) );
								CHATSERVER->SendChatMessageBox( pcUser, "You received %s gp!", FormatNumber( iGold ) );

								//Log
								LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_ActionFieldRewardGold, "Character[%s] Received Action Field Gold[%s]",
														CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), FormatNumber( iGold ) );
							}
							break;
						}
					}
				}
			}

			vUsersAC.clear();
			vUsers.clear();
		}

		//Done
		pcInstance->SetSentPrizes( TRUE );
	}
}

void CActionFieldHandler::UpdateRound( CActionFieldInstance * pcInstance )
{
	//Get Current Round
	ActionFieldRound * psRound = pcInstance->GetActionField()->GetRound( pcInstance->GetRound() );
	if ( psRound )
	{
		if ( pcInstance->IsStarted() )
		{
			//Monsters Killed Count?
			if ( pcInstance->GetRoundKills() >= psRound->iKillsNeeded )
			{
				//Stop (Freeze action field instance)
				pcInstance->Stop();

				//Kill Units
				KillUnits( pcInstance );

				//Not the last round?
				if ( pcInstance->GetRound() < (int)pcInstance->GetActionField()->GetRounds().size() )
				{
					//Next Round
					pcInstance->SetRound( pcInstance->GetRound() + 1 );

					//Start
					pcInstance->Start();
				}
				else
				{
					//Set Finished Time of the last round
					pcInstance->iTimeFinished = pcInstance->iTimeUpdate;

					//Notify Users
					ProcessPacket( pcInstance, &PacketActionFieldEnd( pcInstance->iTimeFinished ) );

					//Save Data
					SaveInstance( pcInstance );
				}
			}
			//End Time?
			else if ( pcInstance->iTimeUpdate == pcInstance->GetActionField()->GetSettings().iDurationTime )
			{
				//Stop
				pcInstance->Stop();

				//Leave Users
				LeaveUsers( pcInstance );

				//Send Message
				SendChat( pcInstance, CHATCOLOR_Global, FormatString( "%s> Duration time exceeded!", pcInstance->GetActionField()->GetName() ) );

				//Send Fail
				ProcessPacket( pcInstance, &PacketActionFieldFail( ACTIONFIELDFAIL_EndTime ) );

				//Disband Instance
				pcInstance->Delete( TRUE );

				//Save Data
				SaveInstance( pcInstance );
			}
		}
		else
		{
			//First Round?
			if ( pcInstance->GetRound() == 1 )
			{
				//Time to Start?
				if ( pcInstance->iTimeUpdate == psRound->iBeginTime )
				{
					//Start
					pcInstance->Start();

					//Notify Users
					ProcessPacket( pcInstance, &PacketActionFieldStart( 0 ) );

					//Reset Time Update
					pcInstance->iTimeUpdate = 0;
				}
			}
			else if ( pcInstance->GetRound() == pcInstance->GetActionField()->GetRounds().size() )
			{
				//Finished Time?
				if ( pcInstance->iTimeFinished != 0 )
				{
					//Leave Time?
					if ( (pcInstance->iTimeFinished + TIME_ACTIONFIELD_LEAVE_FINISHED) == pcInstance->iTimeUpdate )
					{
						//Leave Users
						LeaveUsers( pcInstance );

						pcInstance->Delete( TRUE );
					}
				}
			}
		}

		//Update Data
		PacketActionFieldUpdate sPacket;
		ZeroMemory( &sPacket, sizeof( PacketActionFieldUpdate ) );
		STRINGCOPY( sPacket.szName, pcInstance->GetActionField()->GetName() );
		sPacket.iTime = pcInstance->IsStarted() ? (pcInstance->GetActionField()->GetSettings().iDurationTime - pcInstance->iTimeUpdate) : (psRound->iBeginTime - pcInstance->iTimeUpdate);
		sPacket.iRound = pcInstance->GetRound();
		sPacket.bStarted = pcInstance->IsStarted();
		sPacket.bFinished = pcInstance->iTimeFinished != 0 ? TRUE : FALSE;
		ProcessPacket( pcInstance, &sPacket );
	}
}

void CActionFieldHandler::ProcessPacket( CActionFieldInstance * pcInstance, PacketActionFieldEnter * psPacket )
{
	psPacket->iLength = sizeof( PacketActionFieldEnter );
	psPacket->iHeader = PKTHDR_ActionFieldEnter;

	std::vector<User*> & vUsers = ROOMINSTANCEHANDLER->GetUsersRoom( pcInstance->GetRoom()->GetID() );
	for ( auto pcUser : vUsers )
		SENDPACKET( pcUser, psPacket );
}

void CActionFieldHandler::ProcessPacket( CActionFieldInstance * pcInstance, PacketActionFieldFail * psPacket )
{
	psPacket->iLength = sizeof( PacketActionFieldFail );
	psPacket->iHeader = PKTHDR_ActionFieldFail;

	std::vector<User*> & vUsers = ROOMINSTANCEHANDLER->GetUsersRoom( pcInstance->GetRoom()->GetID() );
	for ( auto pcUser : vUsers )
		SENDPACKET( pcUser, psPacket );
}

void CActionFieldHandler::ProcessPacket( CActionFieldInstance * pcInstance, PacketActionFieldUpdate * psPacket )
{
	psPacket->iLength = sizeof( PacketActionFieldUpdate );
	psPacket->iHeader = PKTHDR_ActionFieldUpdate;
	psPacket->iUsersCount = 0;

	auto PointsShort = []( ActionFieldUser * psaUsers, int iCount ) -> void
	{
		auto Swap = []( ActionFieldUser * psUser1, ActionFieldUser * psUser2 ) -> void
		{
			ActionFieldUser sTemp;
			CopyMemory( &sTemp, psUser1, sizeof( ActionFieldUser ) );
			CopyMemory( psUser1, psUser2, sizeof( ActionFieldUser ) );
			CopyMemory( psUser2, &sTemp, sizeof( ActionFieldUser ) );
		};

		BOOL bSwapped = FALSE;
		for ( int i = 0; i < iCount - 1; i++ )
		{
			bSwapped = FALSE;
			for ( int j = 0; j < iCount - i - 1; j++ )
			{
				if ( psaUsers[j].iScore < psaUsers[j + 1].iScore )
				{
					Swap( &psaUsers[j], &psaUsers[j + 1] );
					bSwapped = TRUE;
				}
			}

			if ( bSwapped == FALSE )
				break;
		}
	};

	std::vector<User*> & vUsers = ROOMINSTANCEHANDLER->GetUsersRoom( pcInstance->GetRoom()->GetID() );
	if ( vUsers.size() > 0 )
	{
		for ( auto pcUser : vUsers )
		{
			psPacket->saUsers[psPacket->iUsersCount].iID = pcUser->GetID();
			psPacket->saUsers[psPacket->iUsersCount].iKills = pcUser->iActionFieldKills;
			psPacket->saUsers[psPacket->iUsersCount].iScore = pcUser->iActionFieldScore;
			psPacket->iUsersCount++;
		}

		PointsShort( psPacket->saUsers, psPacket->iUsersCount );
	}

	for ( auto pcUser : vUsers )
		SENDPACKET( pcUser, psPacket );
}

void CActionFieldHandler::ProcessPacket( CActionFieldInstance * pcInstance, PacketActionFieldStart * psPacket )
{
	psPacket->iLength = sizeof( PacketActionFieldStart );
	psPacket->iHeader = PKTHDR_ActionFieldStart;

	std::vector<User*> & vUsers = ROOMINSTANCEHANDLER->GetUsersRoom( pcInstance->GetRoom()->GetID() );
	for ( auto pcUser : vUsers )
		SENDPACKET( pcUser, psPacket );
}

void CActionFieldHandler::ProcessPacket( CActionFieldInstance * pcInstance, PacketActionFieldEnd * psPacket )
{
	psPacket->iLength = sizeof( PacketActionFieldEnd );
	psPacket->iHeader = PKTHDR_ActionFieldEnd;

	std::vector<User*> & vUsers = ROOMINSTANCEHANDLER->GetUsersRoom( pcInstance->GetRoom()->GetID() );
	for ( auto pcUser : vUsers )
		SENDPACKET( pcUser, psPacket );
}

void CActionFieldHandler::LeaveUsers( CActionFieldInstance * pcInstance )
{
	Point3D sPosition = Point3D( -9478716, 0, -4513597 );
	EMapID eMapID = MAPID_Atlantis;

	if ( (pcInstance->GetActionField()->GetModeID() == ACTIONFIELDID_ForestDungeon1) || (pcInstance->GetActionField()->GetModeID() == ACTIONFIELDID_ForestDungeon2) ||
		 (pcInstance->GetActionField()->GetModeID() == ACTIONFIELDID_ForestDungeon3) )
	{
		eMapID = MAPID_PhillaiTown;

		sPosition = Point3D( 2726 << 8, 0, 75540 << 8 );
	}

	if ( (pcInstance->GetActionField()->GetModeID() == ACTIONFIELDID_DragonsDungeon1) || (pcInstance->GetActionField()->GetModeID() == ACTIONFIELDID_DragonsDungeon2) ||
		 (pcInstance->GetActionField()->GetModeID() == ACTIONFIELDID_DragonsDungeon3) )
	{
		eMapID = MAPID_RicartenTown;

		sPosition = Point3D( 2596 << 8, 0, -18738 << 8 );
	}

	std::vector<User *> & vUsers = ROOMINSTANCEHANDLER->GetUsersRoom( pcInstance->GetRoom()->GetID() );
	for ( auto pcUser : vUsers )
		CHARACTERSERVER->SetPositionWarp( pcUser->pcUserData, sPosition.iX + (rand() % 8192), sPosition.iZ + (rand() % 8192), eMapID );
}

bool CActionFieldHandler::HaveChestItemBox( User * pcUser, EItemID eItemID )
{
	if ( ITEMSERVER->SQLItemBoxCount( pcUser->pcUserData->szAccountName, eItemID ) >= MAX_CHEST_ITEMBOX )
		return true;

	return false;
}

void CActionFieldHandler::Request( User * pcUser, EActionFieldModeID iModeID )
{
	CActionField * pcActionField = GetActionField( iModeID );
	if ( pcActionField )
	{
		std::vector<User *> vUsers = PARTYHANDLER->GetPartyMembers( pcUser->bParty ? pcUser->psParty : NULL );
		if ( vUsers.size() == 0 )
			vUsers.push_back( pcUser );

		//New Session
		CActionFieldInstance * pcInstance = new CActionFieldInstance();
		pcInstance->SetActionField( pcActionField );
		pcInstance->SetRoom( ROOMINSTANCEHANDLER->Add( GetMapID( iModeID ), FALSE, FormatString( "%s's room", pcUser->pcUserData->sCharacterData.szName ) ) );
		pcInstance->SetRound( 1 );

		//Add Users
		CRoomTeam * pcTeam = pcInstance->GetRoom()->Add( ACTIONFIELDUNIT_Friendly )->Add( 1 );
		for ( auto pcPartyUser : vUsers )
			pcTeam->Add( pcPartyUser->GetID() );

		//Create Enemy Team
		pcInstance->GetRoom()->Add( ACTIONFIELDUNIT_Enemy )->Add( 1 );

		//Add Instance
		vInstances.push_back( pcInstance );

		//Teleport Users
		EMapID iMapID = GetMapID( iModeID );
		Point3D sEntrance;
		if ( GetEntranceXZ( iModeID, sEntrance ) )
		{
			for ( auto pcPartyUser : vUsers )
				CHARACTERSERVER->SetPositionWarp( pcPartyUser->pcUserData, sEntrance.iX, sEntrance.iZ, iMapID );
		}

		vUsers.clear();
	}
}

bool CActionFieldHandler::CanEnter( User * pcUser, EActionFieldModeID iModeID )
{
	bool bRet = false;
	if ( EVENT_ACTIONFIELD == FALSE )
	{
		if ( pcUser->GetGameLevel() < GAMELEVEL_Two )
		{
			CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "ActionField> Event is off!" );
			return false;
		}
	}

	CActionField * pcActionField = GetActionField( iModeID );
	if ( pcActionField )
	{
		std::vector<User *> vUsers = PARTYHANDLER->GetPartyMembers( pcUser->bParty ? pcUser->psParty : NULL );
		if ( vUsers.size() == 0 )
			vUsers.push_back( pcUser );

		BOOL bHaveLevel = TRUE;

		//Level
		for ( auto pcPartyUser : vUsers )
		{
			if ( (pcPartyUser->pcUserData->GetLevel() < pcActionField->GetSettings().sRequiredLevel.iMin) || (pcPartyUser->pcUserData->GetLevel() > pcActionField->GetSettings().sRequiredLevel.iMax) )
			{
				bHaveLevel = FALSE;
				break;
			}
		}

		if ( bHaveLevel == FALSE )
		{
			//Notify user
			CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> All players must be level %d-%d!",
				pcActionField->GetSettings().sRequiredLevel.iMin, pcActionField->GetSettings().sRequiredLevel.iMax );
		}
		//Don't reach the users needed?
		else if ( ((int)vUsers.size() < pcActionField->GetSettings().sRequiredUsers.iMin) || ((int)vUsers.size() > pcActionField->GetSettings().sRequiredUsers.iMax) )
		{
			//Notify user
			CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> You need %d-%d players to participate, you have %d player%s.",
				pcActionField->GetSettings().sRequiredUsers.iMin, pcActionField->GetSettings().sRequiredUsers.iMax,
				vUsers.size(), vUsers.size() > 1 ? "s" : "" );
		}
		else
		{
			bRet = true;

			//Check each user in the party or raid...
			for ( auto pcPartyUser : vUsers )
			{
				//Get Saved Instance count
				if ( int iSavedInstances = SQLGetInstanceTodayCount( pcPartyUser, iModeID ); iSavedInstances > 0 )
				{
					//Already joined today?
					SYSTEMTIME * ps = GetServerTime();

					if ( iSavedInstances >= pcActionField->GetSettings().iInstanceDayLimit )
					{
						//Notify users
						CHATSERVER->SendChat( pcPartyUser, CHATCOLOR_Error, "> You already participate of the event today!" );
						PARTYHANDLER->SendChatParty( pcUser, FormatString( "%s already participate of the event today!", pcPartyUser->pcUserData->sCharacterData.szName ), CHATCOLOR_Error, true );

						bRet = false;
						break;
					}
				}

				//Already in a room?
				if ( ROOMINSTANCEHANDLER->GetRoomUnitID( pcPartyUser->GetID() ) )
				{
					//Notify user
					CHATSERVER->SendChatEx( pcPartyUser, CHATCOLOR_Error, "> You already have a %s session!", pcActionField->GetName() );
					PARTYHANDLER->SendChatParty( pcUser, FormatString( "%s already have a %s session!", pcPartyUser->pcUserData->sCharacterData.szName, pcActionField->GetName() ), CHATCOLOR_Error, true );

					bRet = false;
					break;
				}

				//Have Item Chest on ItemBox?
				for ( auto psChest : pcActionField->GetChests() )
				{
					if ( HaveChestItemBox( pcPartyUser, psChest->eItemID ) )
					{
						//Notify user
						CHATSERVER->SendChatEx( pcPartyUser, CHATCOLOR_Error, "> You have more than %d Chest Box on Item Distributor!", MAX_CHEST_ITEMBOX );
						PARTYHANDLER->SendChatParty( pcUser, FormatString( "%s have more than %d Chest Box on Item Distributor!", pcPartyUser->pcUserData->sCharacterData.szName, MAX_CHEST_ITEMBOX ), CHATCOLOR_Error, true );

						bRet = false;
						break;
					}
				}
			}


			//Check each classes
			for ( auto pcPartyUser : vUsers )
			{
				BOOL bContinue = TRUE;
				for ( auto pcPartyUserDistinct : vUsers )
				{
					if ( pcPartyUserDistinct != pcPartyUser )
					{
						if ( (pcPartyUserDistinct->szHardwareID[0] != 0) && (pcPartyUser->szHardwareID[0] != 0) )
						{
							if ( STRINGCOMPARE( pcPartyUserDistinct->szHardwareID, pcPartyUser->szHardwareID ) )
							{
								CHATSERVER->SendChat( pcPartyUser, CHATCOLOR_Error, "> Players must be distinct computers!" );

								bContinue = FALSE;
								break;
							}
						}
					}
				}

				if ( bContinue == FALSE )
				{
					bRet = false;
					break;
				}
			}

			//Leader?
			if ( pcUser->bParty && pcUser->psParty && pcUser->psParty->pcLeader != pcUser )
			{
				CHATSERVER->SendChat( pcUser, CHATCOLOR_Error, "> You must be a party leader to invite others players!" );
				bRet = false;
			}

			//Can join?
			if ( bRet )
			{

			}
		}

		vUsers.clear();
	}

	return bRet;
}

void CActionFieldHandler::SaveInstance( CActionFieldInstance * pcInstance )
{
	std::vector<User*> & vUsers = ROOMINSTANCEHANDLER->GetUsersRoom( pcInstance->GetRoom()->GetID() );

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		int iInstanceID				= -1;
		int iLastRound				= pcInstance->GetRound();
		EActionFieldModeID iModeID	= pcInstance->GetActionField()->GetModeID();

		//Save new Instance
		if ( pcDB->Prepare( "INSERT INTO ActionFieldInstance([ModeID],[Date]) VALUES(?,GETDATE())" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iModeID );
			pcDB->Execute();
		}

		//Get Instance ID
		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT TOP 1 ID FROM ActionFieldInstance ORDER BY ID DESC" ) )
		{
			if ( pcDB->Execute() && pcDB->Fetch() )
				pcDB->GetData( 1, PARAMTYPE_Integer, &iInstanceID );
		}
		
		//Get?
		if ( iInstanceID != -1 )
		{
			pcDB->Close();

			pcDB = SQLCONNECTION( DATABASEID_UserDB );

			if ( pcDB->Open() )
			{
				//Loop each Instance Users
				for ( auto pcUser : vUsers )
				{
					//Insert Instance
					pcDB->Clear();
					if ( pcDB->Prepare( "INSERT INTO CharacterActionFieldInstance([InstanceID],[ModeID],[AccountID],[CharacterID],[CharacterClass],[IP],[Date],[TotalScore],[TotalKills],[LastRound]) VALUES(?,?,?,?,?,?,GETDATE(),?,?,?)" ) )
					{
						char * pszIP = pcUser->pcUserData->pcSocketData ? pcUser->pcUserData->pcSocketData->szIP : "";

						pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iInstanceID );
						pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iModeID );
						pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &pcUser->iAccountID );
						pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &pcUser->iCharacterID );
						pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &pcUser->pcUserData->sCharacterData.iClass );
						pcDB->BindParameterInput( 6, PARAMTYPE_String, pszIP, STRLEN( pszIP ) );
						pcDB->BindParameterInput( 7, PARAMTYPE_Integer, &pcUser->iActionFieldScore );
						pcDB->BindParameterInput( 8, PARAMTYPE_Integer, &pcUser->iActionFieldKills );
						pcDB->BindParameterInput( 9, PARAMTYPE_Integer, &iLastRound );
						pcDB->Execute();
					}
				}
				pcDB->Close();
			}
		}
		pcDB->Close();
	}

}

void CActionFieldHandler::SpawnMonster( CActionFieldInstance * pcInstance, ActionFieldMonster * psMonster, int iIndex )
{
	ActionFieldRound * psRound = pcInstance->GetActionField()->GetRound( pcInstance->GetRound() );
	if ( psRound )
	{
		//Map
		Map * pcMap = MAPSDATA + GetMapID( pcInstance->GetActionField()->GetModeID() );
		if ( pcMap )
		{
			MonsterMapSpawnHandle sMonsterMap;
			sMonsterMap.bFixed = FALSE;
			CopyMemory( &sMonsterMap.sCharacterData, &psMonster->sCharacterData, sizeof( CharacterData ) );
			sMonsterMap.iPositionState = 1;
			sMonsterMap.sCharacterData.iType = ECharacterType::CHARACTERTYPE_Monster;

			pcMap->bForceCreateMonster = TRUE;

			UnitData * pcUnitData = MAPSERVER->CreateUnit( pcMap, &sMonsterMap, 0 );
			if ( pcUnitData )
			{
				CopyMemory( &pcUnitData->sUnitInfo, &psMonster->sUnitInfo, sizeof( UnitInfo ) );

				//Boss
				pcUnitData->sCharacterData.sGlow = psMonster->bIsBoss ? 1 : 0;

				//Remove Drops
				pcUnitData->sUnitInfo.iNumDrops = 0;
				pcUnitData->sUnitInfo.iDropPlusCount = 0;

				//Get Spawn position
				Point3D sPosition = pcUnitData->sPosition;
				sPosition.iX >>= 8;
				sPosition.iY >>= 8;
				sPosition.iZ >>= 8;

				if ( psMonster->bIsBoss )
				{
					if ( psRound->vSpawnBossPosition.size() > 0 )
						sPosition = psRound->vSpawnBossPosition[RandomI( 0, psRound->vSpawnBossPosition.size() - 1 )];
				}
				else
				{
					if ( psRound->vSpawnMonsterPosition.size() > 0 )
						sPosition = psRound->vSpawnMonsterPosition[RandomI( 0, psRound->vSpawnMonsterPosition.size() - 1 )];
				}

				sPosition.iX <<= 8;
				sPosition.iY <<= 8;
				sPosition.iZ <<= 8;

				//Random position
				sPosition.iX += (rand() % 16384) + (iIndex * 16384);

				if ( pcMap->pcBaseMap && (pcMap->pcBaseMap->GetID() != MAPID_GhostCastle) )
					sPosition.iY = pcMap->pcStage->GetHighestPoint( sPosition.iX, sPosition.iZ ) + (3 << 8);

				pcUnitData->sPosition = sPosition;

				pcUnitData->Animate( ANIMATIONTYPE_Idle );
				pcUnitData->iTargetMoveCount = 0;

				pcUnitData->sSpawnPosition = sPosition;
				pcUnitData->iLureDistance = (12 * 64 * 12 * 64);

				pcUnitData->sCharacterData.iSight = (1300 * 1300);

				//Fix Name
				FixUnitDataName( pcUnitData );

				//Add to Instance
				pcInstance->GetRoom()->Get( ACTIONFIELDUNIT_Enemy )->Get( 1 )->Add( pcUnitData->iID );

				//Update Alive Counter
				pcInstance->SetMonstersAlive( pcInstance->GetMonstersAlive() + 1 );
			}
		}
	}
}

void CActionFieldHandler::KillUnits( CActionFieldInstance * pcInstance )
{
	EMapID iMapID = GetMapID( pcInstance->GetActionField()->GetModeID() );

	std::vector<int> vUnitIDs;

	//Kill Units
	std::vector<CRoomAllies *> & vAllies = pcInstance->GetRoom()->GetAllies();
	for ( auto pcAlly : vAllies )
	{
		std::vector<CRoomTeam *> & vTeams = pcAlly->GetTeams();
		for ( auto pcTeam : vTeams )
		{
			std::vector<UnitRoom *> & vUnits = pcTeam->GetUnits();
			for ( auto pc : vUnits )
			{
				//Monster?
				UnitData * pcUnitData = UNITDATABYIDMAP( pc->iID, iMapID );
				if ( pcUnitData )
				{
					vUnitIDs.push_back( pcUnitData->iID );

					pcUnitData->sCharacterData.sHP.sCur = 0;
					pcUnitData->Animate( ANIMATIONTYPE_Die );
				}
			}
		}
	}

	for ( auto i : vUnitIDs )
		pcInstance->GetRoom()->Get( ACTIONFIELDUNIT_Enemy )->Get( 1 )->Remove( i );

	vUnitIDs.clear();
}

void CActionFieldHandler::SendChat( CActionFieldInstance * pcInstance, EChatColor eColor, const char * pszText )
{
	std::vector<User*> & vUsers = ROOMINSTANCEHANDLER->GetUsersRoom( pcInstance->GetRoom()->GetID() );
	for ( auto pcUser : vUsers )
		CHATSERVER->SendChat( pcUser, eColor, pszText );
}

void CActionFieldHandler::LoadActionField( EActionFieldModeID iModeID )
{
	Delete( iModeID );

	CActionField * pcActionField = NULL;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );
	if ( pcDB->Open() )
	{
		BOOL bActionFieldLoaded = FALSE;

		if ( pcDB->Prepare( "SELECT Name, LevelRequiredMin, LevelRequiredMax, UsersRequiredMin, UsersRequiredMax, GoldEntrancePlayerLevelMultiplier, GoldEntranceExtraValue, DurationEventSeconds, InstanceLimitPerDay FROM ActionFieldSettings WHERE ID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iModeID );
			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				char szName[64] = { 0 };
				ActionFieldSettings sSettings;

				pcDB->GetData( 1, PARAMTYPE_String, szName, _countof( szName ) );
				pcDB->GetData( 2, PARAMTYPE_Integer, &sSettings.sRequiredLevel.iMin );
				pcDB->GetData( 3, PARAMTYPE_Integer, &sSettings.sRequiredLevel.iMax );
				pcDB->GetData( 4, PARAMTYPE_Integer, &sSettings.sRequiredUsers.iMin );
				pcDB->GetData( 5, PARAMTYPE_Integer, &sSettings.sRequiredUsers.iMax );
				pcDB->GetData( 6, PARAMTYPE_Integer, &sSettings.iGoldEntranceLevelMultiplier );
				pcDB->GetData( 7, PARAMTYPE_Integer, &sSettings.iGoldEntranceExtra );
				pcDB->GetData( 8, PARAMTYPE_Integer, &sSettings.iDurationTime );
				pcDB->GetData( 9, PARAMTYPE_Integer, &sSettings.iInstanceDayLimit );

				pcActionField = new CActionField( iActionFieldWheel++, szName, iModeID );
				pcActionField->SetSettings( sSettings );

				bActionFieldLoaded = TRUE;
			}
		}

		if ( bActionFieldLoaded )
		{
			std::vector<ActionFieldRound*> vRounds;

			pcDB->Clear();
			if ( pcDB->Prepare( "SELECT ID, RoundLevel, RoundTimeStartSeconds, RoundTimeFinishSeconds, BossSpawnMonstersKilled, KillsNeeded FROM ActionFieldRoundSettings WHERE ModeID=? ORDER BY RoundLevel ASC" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iModeID );
				if ( pcDB->Execute() )
				{
					while ( pcDB->Fetch() )
					{
						ActionFieldRound * psRound = new ActionFieldRound;

						pcDB->GetData( 1, PARAMTYPE_Integer, &psRound->iID );
						pcDB->GetData( 2, PARAMTYPE_Integer, &psRound->iLevel );
						pcDB->GetData( 3, PARAMTYPE_Integer, &psRound->iBeginTime );
						pcDB->GetData( 4, PARAMTYPE_Integer, &psRound->iEndTime );
						pcDB->GetData( 5, PARAMTYPE_Integer, &psRound->iBossSpawnKills );
						pcDB->GetData( 6, PARAMTYPE_Integer, &psRound->iKillsNeeded );

						vRounds.push_back( psRound );
					}
				}
			}

			for ( std::vector<ActionFieldRound*>::iterator it = vRounds.begin(); it != vRounds.end(); it++ )
			{
				ActionFieldRound * psRound	= (*it);
				psRound->iMonsterCount		= 0;

				pcDB->Clear();
				if ( pcDB->Prepare( "SELECT TOP 10 MonsterName, Level, HP, HPMultiplier, EXP, Points, AttackPowerMin, AttackPowerMax, SpecialSkillType, SpecialSkillHit, SpecialSkillHitRate, SpecialSkillHitPowerMin, SpecialSkillHitPowerMax, SpecialSkillHitScope, Absorb, Block, Defense, IsBoss FROM ActionFieldRoundMonsterSettings WHERE RoundID=?" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &psRound->iID );
					if ( pcDB->Execute() )
					{
						while ( pcDB->Fetch() )
						{
							ActionFieldMonster * psMonster = psRound->saMonsters + psRound->iMonsterCount;

							pcDB->GetData( 1, PARAMTYPE_String, psMonster->sCharacterData.szName, _countof( psMonster->sCharacterData.szName ) );

							CharacterData * psCharacterData = UNITINFODATA->GetCharacterDataByMonsterName( psMonster->sCharacterData.szName );
							if ( psCharacterData )
							{
								CopyMemory( &psMonster->sCharacterData, psCharacterData, sizeof( CharacterData ) );
								CopyMemory( &psMonster->sUnitInfo, psCharacterData->psUnitInfo, sizeof( UnitInfo ) );
							}

							pcDB->GetData( 2, PARAMTYPE_Integer, &psMonster->sCharacterData.iLevel );
							
							pcDB->GetData( 3, PARAMTYPE_Short, &psMonster->sCharacterData.sHP.sMax );
							psMonster->sCharacterData.sHP.sCur = psMonster->sCharacterData.sHP.sMax;
							
							pcDB->GetData( 4, PARAMTYPE_Short, &psMonster->sCharacterData.sHPBlocks.sMax );
							psMonster->sCharacterData.sHPBlocks.sCur = psMonster->sCharacterData.sHPBlocks.sMax;

							LARGE_INTEGER liEXP;
							pcDB->GetData( 5, PARAMTYPE_Int64, &liEXP.QuadPart );
							if ( RATE_EXP > 0 )
								liEXP.QuadPart = liEXP.QuadPart * RATE_EXP;

							if ( EVENT_PVPMODE )
								liEXP.QuadPart += (liEXP.QuadPart * RATE_EXPPVPMODE) / 100;

							psMonster->sCharacterData.iCurrentExpLow	= liEXP.LowPart;
							psMonster->sCharacterData.iCurrentExpHigh	= liEXP.HighPart;

							pcDB->GetData( 6, PARAMTYPE_Integer, &psMonster->iPoints );
							
							pcDB->GetData( 7, PARAMTYPE_Integer, &psMonster->sCharacterData.iMinDamage );
							pcDB->GetData( 8, PARAMTYPE_Integer, &psMonster->sCharacterData.iMaxDamage );
							pcDB->GetData( 9, PARAMTYPE_Integer, &psMonster->sUnitInfo.iSkillType );
							pcDB->GetData( 10, PARAMTYPE_Integer, &psMonster->sUnitInfo.iSkillPierceRange );
							pcDB->GetData( 11, PARAMTYPE_Integer, &psMonster->sUnitInfo.iSkillChance );
							pcDB->GetData( 12, PARAMTYPE_Short, &psMonster->sUnitInfo.sSkillPower.sMin );
							pcDB->GetData( 13, PARAMTYPE_Short, &psMonster->sUnitInfo.sSkillPower.sMax );
							pcDB->GetData( 14, PARAMTYPE_Integer, &psMonster->sUnitInfo.iSkillArea );
							pcDB->GetData( 15, PARAMTYPE_Integer, &psMonster->sCharacterData.iAbsorbRating );
							pcDB->GetData( 16, PARAMTYPE_Integer, &psMonster->sCharacterData.iBlockRating );
							pcDB->GetData( 17, PARAMTYPE_Integer, &psMonster->sCharacterData.iDefenseRating );
							pcDB->GetData( 18, PARAMTYPE_Integer, &psMonster->bIsBoss );

							psRound->iMonsterCount++;
						}
					}
				}

				pcDB->Clear();
				if ( pcDB->Prepare( "SELECT PositionX, PositionY, PositionZ FROM ActionFieldMonsterSpawnLocation WHERE RoundID=?" ) )
				{
					psRound->vSpawnMonsterPosition.clear();

					pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &psRound->iID );
					if ( pcDB->Execute() )
					{
						while ( pcDB->Fetch() )
						{
							Point3D sPosition = Point3D( 0, 0, 0 );
							pcDB->GetData( 1, PARAMTYPE_Integer, &sPosition.iX );
							pcDB->GetData( 2, PARAMTYPE_Integer, &sPosition.iY );
							pcDB->GetData( 3, PARAMTYPE_Integer, &sPosition.iZ );

							psRound->vSpawnMonsterPosition.push_back( sPosition );
						}
					}
				}

				pcDB->Clear();
				if ( pcDB->Prepare( "SELECT PositionX, PositionY, PositionZ FROM ActionFieldBossSpawnLocation WHERE RoundID=?" ) )
				{
					psRound->vSpawnBossPosition.clear();

					pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &psRound->iID );
					if ( pcDB->Execute() )
					{
						while ( pcDB->Fetch() )
						{
							Point3D sPosition = Point3D( 0, 0, 0 );
							pcDB->GetData( 1, PARAMTYPE_Integer, &sPosition.iX );
							pcDB->GetData( 2, PARAMTYPE_Integer, &sPosition.iY );
							pcDB->GetData( 3, PARAMTYPE_Integer, &sPosition.iZ );

							psRound->vSpawnBossPosition.push_back( sPosition );
						}
					}
				}

				pcActionField->AddRound( *psRound );

				DELET( psRound );
			}
			vRounds.clear();

			pcDB->Clear();
			if ( pcDB->Prepare( "SELECT ID, ChestItemCode, RequiredKeyItemCode, RequiredKeyQuantity FROM ActionFieldChestSettings WHERE ModeID=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iModeID );
				if ( pcDB->Execute() )
				{
					while ( pcDB->Fetch() )
					{
						ActionFieldChest sChest;
						pcDB->GetData( 1, PARAMTYPE_Integer, &sChest.iID );
						pcDB->GetData( 2, PARAMTYPE_String, sChest.szItemCode, _countof( sChest.szItemCode ) );
						pcDB->GetData( 3, PARAMTYPE_String, sChest.szRequiredItemCode, _countof( sChest.szRequiredItemCode ) );
						pcDB->GetData( 4, PARAMTYPE_Integer, &sChest.iRequiredItemCount );

						if ( auto pcItemData = ITEMSERVER->FindItemPointerTable( sChest.szItemCode ) )
							sChest.eItemID = pcItemData->sBaseItemID.ToItemID();

						pcActionField->AddChest( sChest );
					}
				}
			}

			pcDB->Clear();
			if ( pcDB->Prepare( "SELECT ChestID, ItemCode, Quantity, PercentChance FROM ActionFieldRewardList WHERE ModeID=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iModeID );
				if ( pcDB->Execute() )
				{
					while ( pcDB->Fetch() )
					{
						ActionFieldReward sReward;
						pcDB->GetData( 1, PARAMTYPE_Integer, &sReward.iChestID );
						pcDB->GetData( 2, PARAMTYPE_String, sReward.szItemCode, _countof( sReward.szItemCode ) );
						pcDB->GetData( 3, PARAMTYPE_Integer, &sReward.iQuantity );
						pcDB->GetData( 4, PARAMTYPE_Integer, &sReward.iPercentChance );

						pcActionField->AddReward( sReward );
					}
				}
			}
			vActionFields.push_back( pcActionField );
		}
		pcDB->Close();
	}
}

void CActionFieldHandler::Update()
{
	//Check Instances to disband...
	for ( std::vector<CActionFieldInstance*>::iterator it = vInstances.begin(); it != vInstances.end(); )
	{
		CActionFieldInstance * pcInstance = (*it);
		if ( pcInstance->Delete() )
		{
			//Kill
			KillUnits( pcInstance );

			//Reset score
			std::vector<User*> & vUsers = ROOMINSTANCEHANDLER->GetUsersRoom( pcInstance->GetRoom()->GetID() );
			for ( auto pcUser : vUsers )
			{
				pcUser->iActionFieldKills = 0;
				pcUser->iActionFieldScore = 0;
			}

			//Delete Room
			ROOMINSTANCEHANDLER->Delete( pcInstance->GetRoom() );

			DELET( pcInstance );
			it = vInstances.erase( it );
		}
		else
			it++;
	}

	for ( std::vector<CActionFieldInstance*>::iterator it = vInstances.begin(); it != vInstances.end(); it++ )
	{
		CActionFieldInstance * pcInstance = (*it);
		if ( pcInstance->Delete() == FALSE )
		{
			if ( pcInstance->IsStarted() )
			{
				//Update Spawns
				UpdateMonsterSpawn( pcInstance );
			}

			//Update Round
			UpdateRound( pcInstance );

			//Update Prizes
			UpdatePrizes( pcInstance );

			//Update Time
			pcInstance->iTimeUpdate++;
		}
	}
}

BOOL CActionFieldHandler::OnUnitKilled( User * pcUser, Unit * pcUnit )
{
	BOOL bRet = FALSE;

	//Same Room?
	if ( ROOMINSTANCEHANDLER->SameRoom( pcUser->GetID(), pcUnit->pcUnitData->iID ) )
	{
		//Get Instance
		CActionFieldInstance * pcInstance = GetInstanceRoom( ROOMINSTANCEHANDLER->GetRoomUnitID( pcUser->GetID() ) );
		if ( pcInstance )
		{
			//Update and remove unit killed
			pcInstance->SetRoundKills( pcInstance->GetRoundKills() + 1 );
			pcInstance->SetTotalKills( pcInstance->GetTotalKills() + 1 );
			pcInstance->GetRoom()->Get( ACTIONFIELDUNIT_Enemy )->Get( 1 )->Remove( pcUnit->pcUnitData->iID );

			//Get Current Round
			ActionFieldRound * psRound = pcInstance->GetActionField()->GetRound( pcInstance->GetRound() );
			if ( psRound )
			{
				//Get Monster Data
				for ( int i = 0; i < psRound->iMonsterCount; i++ )
				{
					ActionFieldMonster * ps = psRound->saMonsters + i;
					if ( STRINGCOMPARE( FixUnitDataName(ps->sCharacterData.szName).c_str(), pcUnit->GetName() ) )
					{
						//Add Kill and Score
						pcUser->iActionFieldKills++;
						pcUser->iActionFieldScore += ps->iPoints;
						break;
					}
				}

				//Kills for boss spawn? So spawn it...
				if ( pcInstance->GetRoundKills() == psRound->iBossSpawnKills )
					pcInstance->SetBossSpawn( TRUE );
			}

			//Update Alive Counter
			pcInstance->SetMonstersAlive( pcInstance->GetMonstersAlive() - 1 );

			bRet = TRUE;
		}
	}

	return bRet;
}

void CActionFieldHandler::HandlePacket( User * pcUser, PacketActionFieldRequest * psPacket )
{
	if ( CanEnter( pcUser, psPacket->iaModeID[0] ) )
	{
		std::vector<User *> vUsers = PARTYHANDLER->GetPartyMembers( pcUser->bParty ? pcUser->psParty : NULL );
		if ( vUsers.size() == 0 )
			vUsers.push_back( pcUser );

		PacketNetActionFieldRequest sPacket;
		sPacket.iID = pcUser->GetID();
		sPacket.bEnter = FALSE;
		sPacket.iUserErrorID = -1;
		sPacket.iModeID = psPacket->iaModeID[0];
		sPacket.iCount = 0;
		for ( auto pcUserParty : vUsers )
			sPacket.iaUsersPartyID[sPacket.iCount++] = pcUserParty->GetID();

		NETSERVER->SendActionFieldRequest( &sPacket, TRUE );

		vUsers.clear();
	}
}

void CActionFieldHandler::HandlePacket( User * pcUser, PacketActionFieldExchange * psPacket )
{
	ActionFieldChest * psChest = NULL;
	CActionField * pcActionField = NULL;

	//Secure code
	if ( psPacket->iItemCount < 0 )
		psPacket->iItemCount = 0;
	else if ( psPacket->iItemCount > 12 )
		psPacket->iItemCount = 12;

	//Check Items
	if ( psPacket->iItemID == ITEMID_GoldenLockedChest )
	{
		pcActionField = GetActionField( EActionFieldModeID::ACTIONFIELDID_DragonsDungeon1 );
		if ( pcActionField )
		{
			ItemData * pcItemData = ITEMSERVER->FindItemPointerTable( psPacket->iItemID );
			if ( pcItemData )
				psChest = pcActionField->GetChestItem( pcItemData->szInventoryName );
		}
	}

	if ( psPacket->iItemID == ITEMID_SilverLockedChest )
	{
		pcActionField = GetActionField( EActionFieldModeID::ACTIONFIELDID_GhostCastle1 );
		if ( pcActionField )
		{
			ItemData * pcItemData = ITEMSERVER->FindItemPointerTable( psPacket->iItemID );
			if ( pcItemData )
				psChest = pcActionField->GetChestItem( pcItemData->szInventoryName );
		}
	}

	if ( psPacket->iItemID == ITEMID_BlueLockedChest )
	{
		pcActionField = GetActionField( EActionFieldModeID::ACTIONFIELDID_ForestDungeon1 );
		if ( pcActionField )
		{
			ItemData * pcItemData = ITEMSERVER->FindItemPointerTable( psPacket->iItemID );
			if ( pcItemData )
				psChest = pcActionField->GetChestItem( pcItemData->szInventoryName );
		}
	}

	if ( psPacket->iItemID == ITEMID_PurpleLockedChest )
	{
		pcActionField = GetActionField( EActionFieldModeID::ACTIONFIELDID_ForestDungeon3 );
		if ( pcActionField )
		{
			ItemData * pcItemData = ITEMSERVER->FindItemPointerTable( psPacket->iItemID );
			if ( pcItemData )
				psChest = pcActionField->GetChestItem( pcItemData->szInventoryName );
		}
	}

	//Found Chest?
	if ( psChest && pcActionField )
	{
		//Get Item Key Data
		ItemData * pcItemData = ITEMSERVER->FindItemPointerTable( psChest->szRequiredItemCode );
		if ( pcItemData )
		{
			BOOL bValid = TRUE;

			//Check Valid Item
			for ( int i = 0; i < psPacket->iItemCount; i++ )
			{
				if ( psPacket->iaKeyItemID[i] != pcItemData->sBaseItemID.ToItemID() )
				{
					bValid = FALSE;
					break;
				}
			}

			//Not the same required count? Invalidate...
			if ( psPacket->iItemCount != psChest->iRequiredItemCount )
			{
				CHATSERVER->SendChatMessageBox( pcUser, "You need %d %s!", psChest->iRequiredItemCount, pcItemData->szBaseName );
				CHATSERVER->SendChatEx( pcUser, EChatColor::CHATCOLOR_Global, "> You need %d %s!", psChest->iRequiredItemCount, pcItemData->szBaseName );

				bValid = FALSE;
			}
			//Valid?
			if ( bValid )
			{
				//Checksum...
				if ( ITEMSERVER->UseItem( pcUser->pcUserData, psPacket->iItemID, psPacket->iChk1, psPacket->iChk2 ) < 0 )
				{
					//Log
					PacketLogCheat s;
					s.iCheatID = CHEATLOGID_ActionFieldFakeItem;
					s.LParam = psPacket->iItemID;
					s.SParam = psPacket->iChk1;
					s.EParam = psPacket->iChk2;
					LOGSERVER->OnLogCheat( pcUser->pcUserData, &s );

					//Error
					bValid = FALSE;
				}

				if ( bValid )
				{
					for ( int i = 0; i < psPacket->iItemCount; i++ )
					{
						//Checksum...
						if ( ITEMSERVER->UseItem( pcUser->pcUserData, psPacket->iaKeyItemID[i], psPacket->iaKeyChk1[i], psPacket->iaKeyChk2[i] ) < 0 )
						{
							//Log
							PacketLogCheat s;
							s.iCheatID = CHEATLOGID_ActionFieldFakeItem;
							s.LParam = psPacket->iaKeyItemID[i];
							s.SParam = psPacket->iaKeyChk1[i];
							s.EParam = psPacket->iaKeyChk2[i];
							LOGSERVER->OnLogCheat( pcUser->pcUserData, &s );

							//Error
							bValid = FALSE;
							break;
						}
					}

					//Allowed reward?
					if ( bValid )
					{
						//Send...
						std::vector<ActionFieldReward*> vRewards = pcActionField->GetRewards( psChest->iID );

						int iTotalPercent = 0;
						for ( auto psReward : vRewards )
							iTotalPercent += psReward->iPercentChance;

						if ( iTotalPercent > 0 )
						{
							int iRand = (rand() << 7) | ((GetCurrentTime() >> 2) & 0x7F);
							iRand = iRand % iTotalPercent;

							int iTotal = 0;

							BOOL bReward = FALSE;
							for ( auto psReward : vRewards )
							{
								if ( (iRand >= iTotal) && (iRand < (iTotal + psReward->iPercentChance)) )
								{
									//Send to Item Box
									ITEMSERVER->AddItemOpenBox( pcUser->pcUserData, psReward->szItemCode, 0, psReward->iQuantity );
									
									//Log
									LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_ActionFieldRewardReceived, "Character[%s] Received Action Field Reward Item[%s] Quantity[%d]",
										CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), psReward->szItemCode, psReward->iQuantity );

									SQLRewardLog( pcActionField->GetModeID(), psChest->iID, pcUser, psReward );

									bReward = TRUE;
									break;
								}

								iTotal += psReward->iPercentChance;
							}

							if ( bReward == FALSE )
							{
								if ( vRewards.size() > 0 )
								{
									//Send to Item Box
									ActionFieldReward * psReward = vRewards[0];
									ITEMSERVER->AddItemOpenBox( pcUser->pcUserData, psReward->szItemCode, 0, psReward->iQuantity );

									//Log
									LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_ActionFieldRewardReceived, "Character[%s] Received Action Field Reward Item[%s] Quantity[%d]",
										CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), psReward->szItemCode, psReward->iQuantity );

									SQLRewardLog( pcActionField->GetModeID(), psChest->iID, pcUser, psReward );
								}
							}

							//Send Message
							CHATSERVER->SendChatMessageBox( pcUser, "Check Item Distributor to get your reward!" );
							CHATSERVER->SendChat( pcUser, EChatColor::CHATCOLOR_Global, "> Check Item Distributor to get your reward!" );

							psPacket->bSuccess = TRUE;
						}
					}
				}

			}
		}
	}

	SENDPACKET( pcUser, psPacket );
}

void CActionFieldHandler::HandleNetPacket( PacketNetActionFieldRequest * psPacket )
{
	if ( GAME_SERVER )
	{
		if ( psPacket->bEnter )
		{
			if ( auto pcUser = USERDATATOUSER( USERDATABYID( psPacket->iID ) ) )
				Request( pcUser, psPacket->iModeID );
		}
		else
		{
			if ( auto pcUser = USERDATATOUSER( USERDATABYID( psPacket->iID ) ) )
			{
				if ( auto pcUserError = USERDATATOUSER( USERDATABYID( psPacket->iUserErrorID ) ) )
				{
					//Notify users
					PARTYHANDLER->SendChatParty( pcUser, FormatString( "%s doesn't have an Event Pass item to attend again!", pcUserError->pcUserData->sCharacterData.szName ), CHATCOLOR_Error, true );
				}
			}
		}
	}
	else if ( LOGIN_SERVER )
	{
		if ( auto pcUser = USERDATATOUSER( USERDATABYID( psPacket->iID ) ) )
		{
			if ( auto pcActionField = GetActionField( psPacket->iModeID ) )
			{
				EItemID eItemPassID = ITEMID_ActionFieldPass;
				if ( (psPacket->iModeID == ACTIONFIELDID_ForestDungeon1) || (psPacket->iModeID == ACTIONFIELDID_ForestDungeon2) || (psPacket->iModeID == ACTIONFIELDID_ForestDungeon3) )
					eItemPassID = ITEMID_ActionFieldPassFD;
				else if ( (psPacket->iModeID == ACTIONFIELDID_DragonsDungeon1) || (psPacket->iModeID == ACTIONFIELDID_DragonsDungeon2) || (psPacket->iModeID == ACTIONFIELDID_DragonsDungeon3) )
					eItemPassID = ITEMID_ActionFieldPassDD;

				struct ItemDeleteActionField
				{
					User * pcUser;
					int				iItemID;
					int				iChk1;
					int				iChk2;
				};

				std::vector<ItemDeleteActionField> vDeleteItems;

				psPacket->bEnter = TRUE;

				//Delete all Tickets
				for ( int i = 0; i < psPacket->iCount; i++ )
				{
					if ( auto pcUserParty = USERDATATOUSER( USERDATABYID( psPacket->iaUsersPartyID[i] ) ) )
					{
						if ( int iSavedInstances = SQLGetInstanceTodayCount( pcUserParty, psPacket->iModeID ); iSavedInstances >= 0 )
						{
							if ( (iSavedInstances > 0) && (iSavedInstances <= (pcActionField->GetSettings().iInstanceDayLimit - 1)) )
							{
								int iChk1 = 0;
								int iChk2 = 0;

								//User have Ticket?
								if ( ITEMSERVER->GetItemInventoryCheksum( pcUserParty, eItemPassID, iChk1, iChk2 ) )
								{
									//Add to queue for Delete Ticket
									ItemDeleteActionField sDeleteItem;
									sDeleteItem.pcUser = pcUserParty;
									sDeleteItem.iChk1 = iChk1;
									sDeleteItem.iChk2 = iChk2;
									vDeleteItems.push_back( sDeleteItem );
								}
								else
								{
									psPacket->bEnter = FALSE;
									psPacket->iUserErrorID = pcUserParty->GetID();
									break;
								}
							}
						}
						else
						{
							psPacket->bEnter = FALSE;
							psPacket->iUserErrorID = pcUserParty->GetID();
							break;
						}
					}
					else
					{
						psPacket->bEnter = FALSE;
						psPacket->iUserErrorID = psPacket->iaUsersPartyID[i];
						break;
					}
				}

				if ( psPacket->bEnter )
				{
					for ( auto & sDelete : vDeleteItems )
					{
						ITEMSERVER->UseItem( sDelete.pcUser->pcUserData, eItemPassID, sDelete.iChk1, sDelete.iChk2 );
						ITEMSERVER->SendDeleteItem( sDelete.pcUser, eItemPassID, sDelete.iChk1, sDelete.iChk2 );
					}
				}

				vDeleteItems.clear();

				//Send Packet
				NETSERVER->SendActionFieldRequest( psPacket, FALSE );
			}
		}
	}
}

void CActionFieldHandler::RequestTest( User * pcUser )
{
	Request( pcUser, EActionFieldModeID::ACTIONFIELDID_GhostCastle1 );
}

void CActionFieldHandler::OnDisconnectedUser( User * pcUser )
{
	//Get Instance
	CActionFieldInstance * pcInstance = GetInstanceRoom( ROOMINSTANCEHANDLER->GetRoomUnitID( pcUser->GetID() ) );
	if ( pcInstance )
	{
		//Send Fail
		ProcessPacket( pcInstance, &PacketActionFieldFail( ACTIONFIELDFAIL_Disconnected ) );

		//Leave Users
		LeaveUsers( pcInstance );

		//Send Message
		SendChat( pcInstance, CHATCOLOR_Global, FormatString( "%s> %s was disconnected from the room!", pcInstance->GetActionField()->GetName(),
			CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) ) );

		//Delete
		pcInstance->Delete( TRUE );
	}
}

void CActionFieldHandler::OnEnterParty( User * pcUser )
{
	//Get Instance
	CActionFieldInstance * pcInstance = GetInstanceRoom( ROOMINSTANCEHANDLER->GetRoomUnitID( pcUser->GetID() ) );
	if ( pcInstance )
	{
		//Send Fail
		ProcessPacket( pcInstance, &PacketActionFieldFail( ACTIONFIELDFAIL_LeaveParty ) );

		//Leave Users
		LeaveUsers( pcInstance );

		//Send Message
		SendChat( pcInstance, CHATCOLOR_Global, FormatString( "%s> %s has entered in the party while event is running!", pcInstance->GetActionField()->GetName(),
			CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) ) );

		//Delete
		pcInstance->Delete( TRUE );
	}
}

void CActionFieldHandler::OnLeaveParty( User * pcUser )
{
	//Get Instance
	CActionFieldInstance * pcInstance = GetInstanceRoom( ROOMINSTANCEHANDLER->GetRoomUnitID( pcUser->GetID() ) );
	if ( pcInstance )
	{
		//Send Fail
		ProcessPacket( pcInstance, &PacketActionFieldFail( ACTIONFIELDFAIL_LeaveParty ) );

		//Leave Users
		LeaveUsers( pcInstance );

		//Send Message
		SendChat( pcInstance, CHATCOLOR_Global, FormatString( "%s> %s has left the party!", pcInstance->GetActionField()->GetName(),
			CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) ) );

		//Delete
		pcInstance->Delete( TRUE );
	}
}

void CActionFieldHandler::OnLeaveMap( User * pcUser, int iMapID )
{
	//Get Instance
	CActionFieldInstance * pcInstance = GetInstanceRoom( ROOMINSTANCEHANDLER->GetRoomUnitID( pcUser->GetID() ) );
	if ( pcInstance )
	{
		if ( pcInstance->IsStarted() )
		{
			if ( GetMapID( pcInstance->GetActionField()->GetModeID() ) != iMapID )
			{
				//Send Fail
				ProcessPacket( pcInstance, &PacketActionFieldFail( ACTIONFIELDFAIL_LeaveMap ) );

				//Leave Users
				LeaveUsers( pcInstance );

				//Send Message
				SendChat( pcInstance, CHATCOLOR_Global, FormatString( "%s> %s has left the map!", pcInstance->GetActionField()->GetName(),
					CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) ) );

				//Delete
				pcInstance->Delete( TRUE );
			}
		}
	}
}

BOOL CActionFieldHandler::IsOnInstance( Unit * pcUnit )
{
	//Get Instance
	CActionFieldInstance * pcInstance = GetInstanceRoom( ROOMINSTANCEHANDLER->GetRoomUnitID( pcUnit->GetID() ) );
	if ( pcInstance )
		return TRUE;

	return FALSE;
}
