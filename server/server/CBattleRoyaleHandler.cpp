#include "stdafx.h"
#include "CBattleRoyaleHandler.h"

using namespace BattleRoyale;

CBattleRoyaleInstance::CBattleRoyaleInstance()
{
	for ( auto & s : sState.saPlayerTeams )
		s.iID = -1;
}

CBattleRoyaleInstance::~CBattleRoyaleInstance()
{
	for ( auto psPlayer : vPlayers )
	{
		if ( auto pcUser = USERDATATOUSER( USERDATABYID( psPlayer->iID ) ) )
		{
			pcUser->bInBattleRoyale = false;
			NETSERVER->SendBattleRoyaleStatus( pcUser );
			BATTLEROYALEHANDLER->TeleportUser( this, pcUser, false );

			BATTLEROYALEHANDLER->ProcessPacket( pcUser, &PacketBattleRoyaleBattleTime( FALSE ) );
		}

		DELET( psPlayer );
	}
	vPlayers.clear();
}

bool CBattleRoyaleInstance::HasUser( User * pcUser )
{
	for ( auto psPlayer : vPlayers )
		if ( psPlayer->iID == pcUser->GetID() )
			return true;

	return false;
}

bool CBattleRoyaleInstance::AddUser( User * pcUser )
{
	if ( GetState().iPlayersCount < GetBattleRoyale()->GetRequiredUsers().iMax )
	{
		auto psPlayerState = new PlayerState;
		psPlayerState->iID = pcUser->GetID();
		psPlayerState->iCharacterID = pcUser->iCharacterID;
		psPlayerState->eState = PLAYERSTATE_Alive;
		psPlayerState->iTimeDeath = 0;
		psPlayerState->eTeamID = PLAYERTEAM_None;
		psPlayerState->iKills = 0;
		vPlayers.push_back( psPlayerState );

		pcUser->bInBattleRoyale = true;
		NETSERVER->SendBattleRoyaleStatus( pcUser );

		GetRoom()->Get( 1 )->Get( 1 )->Add( pcUser->GetID() );

		GetState().iPlayersCount++;

		return true;
	}

	return false;
}

bool CBattleRoyaleInstance::RemoveUser( int iID )
{
	bool bRet = false;
	for ( auto it = vPlayers.begin(); it != vPlayers.end(); )
	{
		auto psPlayerState = (*it);
		if ( psPlayerState->iID == iID )
		{
			DELET( psPlayerState );
			it = vPlayers.erase( it );

			bRet = true;
		}
		else
			it++;
	}

	if ( auto pcUser = USERDATATOUSER( USERDATABYID( iID ) ) )
	{
		pcUser->bInBattleRoyale = false;
		NETSERVER->SendBattleRoyaleStatus( pcUser );
		BATTLEROYALEHANDLER->TeleportUser( this, pcUser, false );

		BATTLEROYALEHANDLER->ProcessPacket( pcUser, &PacketBattleRoyaleBattleTime( FALSE ) );
	}

	if ( bRet )
		ROOMINSTANCEHANDLER->RemoveUnit( iID );

	return bRet;
}

bool CBattleRoyaleInstance::RemoveUser( User * pcUser, bool bDisconnected )
{
	bool bRet = false;
	for ( auto it = vPlayers.begin(); it != vPlayers.end(); )
	{
		auto psPlayerState = (*it);
		if ( psPlayerState->iID == pcUser->GetID() )
		{
			DELET( psPlayerState );
			it = vPlayers.erase( it );

			bRet = true;
		}
		else
			it++;
	}

	if ( (bDisconnected == true) || GetBattleRoyale()->IsTeamRoyalTickets() )
	{
		for ( int i = 0; i < sState.iPlayerTeamCount; i++ )
		{
			if ( sState.saPlayerTeams[i].iID == pcUser->GetID() )
			{
				sState.saPlayerTeams[i].iID = -1;

				break;
			}
		}
	}

	pcUser->bInBattleRoyale = false;
	NETSERVER->SendBattleRoyaleStatus( pcUser );

	if ( !bDisconnected )
	{
		BATTLEROYALEHANDLER->TeleportUser( this, pcUser, false );

		BATTLEROYALEHANDLER->ProcessPacket( pcUser, &PacketBattleRoyaleBattleTime( FALSE ) );
	}
	if ( bRet )
		ROOMINSTANCEHANDLER->RemoveUnit( pcUser->GetID() );

	return bRet;
}

CBattleRoyaleHandler::CBattleRoyaleHandler()
{
}


CBattleRoyaleHandler::~CBattleRoyaleHandler()
{
	Clear();
}

void CBattleRoyaleHandler::Init()
{
	//Create Directory
	CreateDirectoryA( "Data\\CharacterBroyale", NULL );

	Load();
}

void CBattleRoyaleHandler::Load( int iBattleID )
{
	if ( iBattleID >= 0 )
	{
		Remove( iBattleID );

		CBattleRoyale * pcBattleRoyale = new CBattleRoyale();
		if ( SQLGetBattleRoyale( iBattleID, pcBattleRoyale ) )
			vBattleRoyales.push_back( pcBattleRoyale );
		else
			DELET( pcBattleRoyale );
	}
	else
	{
		Clear();

		std::vector<int> vListID;
		if ( SQLGetListID( vListID ) )
		{
			for ( auto i : vListID )
			{
				CBattleRoyale * pcBattleRoyale = new CBattleRoyale();
				if ( SQLGetBattleRoyale( i, pcBattleRoyale ) )
					vBattleRoyales.push_back( pcBattleRoyale );
				else
					DELET( pcBattleRoyale );
			}
		}
	}
}

BOOL CBattleRoyaleHandler::AddUser( User * pcUser, int iBattleID )
{
	BOOL bRet = FALSE;
	for ( auto pcInstance : vInstances )
	{
		if ( pcInstance->GetState().eGameState == GAMESTATE_WaitingForPlayers )
		{
			if ( pcInstance->GetBattleRoyale()->GetID() == iBattleID )
			{
				if ( !pcInstance->HasUser( pcUser ) )
				{
					auto sRequiredLevel = pcInstance->GetBattleRoyale()->GetRequiredLevel();

					if ( (pcUser->pcUserData->sCharacterData.iLevel >= sRequiredLevel.iMin) && (pcUser->pcUserData->sCharacterData.iLevel <= sRequiredLevel.iMax) )
					{
						if ( pcInstance->AddUser( pcUser ) )
						{
							TeleportUser( pcInstance, pcUser, true );

							bRet = TRUE;
						}
						else
						{
							CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "BattleRoyale> Maximum number of players have been reached!" );
							CHATSERVER->SendChatMessageBox( pcUser, "Maximum number of players have been reached!" );
						}
					}
					else
					{
						CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Global, "BattleRoyale> You can't enter due level requirements (%d-%d)", sRequiredLevel.iMin, sRequiredLevel.iMax );
						CHATSERVER->SendChatMessageBox( pcUser, "You can't enter due level requirements (%d-%d)", sRequiredLevel.iMin, sRequiredLevel.iMax );
					}
				}
				break;
			}
		}
	}

	return bRet;
}

BOOL CBattleRoyaleHandler::IsExclusiveDrop( User * pcUser, int iExclusiveID )
{
	if ( IsInBattleRoyale( pcUser ) )
	{
		if ( auto pcInstance = GetInstance( pcUser ) )
			if ( pcInstance->GetDropID() == iExclusiveID )
				return TRUE;
	}

	return FALSE;
}

void CBattleRoyaleHandler::TeleportUser( CBattleRoyaleInstance * pcInstance, User * pcUser, bool bArena, bool bRandomPosition, bool bCountdownPosition )
{
	Point3D sSpawn;

	bool bRoyalDesert = false;
	if ( (pcInstance->GetBattleRoyale()->GetID() == BATTLEROYALE_PvPFast) || pcInstance->GetBattleRoyale()->IsTeamRoyal() )
	{
		if ( pcInstance->GetBattleRoyale()->GetMapID() == MAPID_RoyalDesert )
			bRoyalDesert = true;
	}

	if ( bCountdownPosition )
	{
		if ( bRoyalDesert )
		{
			sSpawn.iX = RandomI( -21990, -20790 ) << 8;
			sSpawn.iZ = RandomI( 33145, 34350 ) << 8;

			sSpawn.iY = 0;
		}
		else if ( pcInstance->GetBattleRoyale()->GetMapID() == MAPID_OasisRoyale )
		{
			static UINT SPAWN_BROYALE_WHEEL = 0;

			std::vector<Point3D> vSpawns;

			//Get Map Data
			Map * pcMap = MAPSDATA + MAPID_OasisRoyale;
			if ( MAPSERVER->IsValidMap( pcMap ) )
			{
				for ( UINT i = 0; i < _countof( pcMap->saSpawnFlag ); i++ )
				{
					if ( pcMap->saSpawnFlag[i].bActive )
					{
						Point3D sNewPosition = Point3D( pcMap->saSpawnFlag[i].iX << 8, 0, pcMap->saSpawnFlag[i].iZ << 8 );
						
						vSpawns.push_back( sNewPosition );
					}
				}
			}

			vSpawns.push_back( pcInstance->GetState().sPosition );

			sSpawn = vSpawns[SPAWN_BROYALE_WHEEL % vSpawns.size()];

			SPAWN_BROYALE_WHEEL += RandomI( 1, 3 );

			vSpawns.clear();
		}
		else
		{	//Get Random Position
			if ( GetRandomCoordinate( pcInstance->GetBattleRoyale(), sSpawn ) )
			{
				sSpawn.iX <<= 8;
				sSpawn.iZ <<= 8;
			}
		}
	}
	else if ( bArena )
	{
		if ( bRandomPosition )
		{
			if ( bRoyalDesert )
			{
				if ( pcInstance->GetState().eGameState == GAMESTATE_WaitingForPlayers )
				{
					//Get Random Position
					if ( GetRandomCoordinate( pcInstance->GetBattleRoyale(), sSpawn ) )
					{
						sSpawn.iX <<= 8;
						sSpawn.iZ <<= 8;
					}
				}
				else if ( pcInstance->GetState().eGameState == GAMESTATE_WaitingPvP )
				{
					//Get Random Position
					if ( GetRandomCoordinate( pcInstance->GetBattleRoyale(), sSpawn ) )
					{
						sSpawn.iX <<= 8;
						sSpawn.iZ <<= 8;
					}
				}
				else
				{
					sSpawn.iX = RandomI( -21990, -20790 ) << 8;
					sSpawn.iZ = RandomI( 33145, 34350 ) << 8;

					sSpawn.iY = 0;
				}
			}
			else
			{
				//Get Random Position
				if ( GetRandomCoordinate( pcInstance->GetBattleRoyale(), sSpawn ) )
				{
					sSpawn.iX <<= 8;
					sSpawn.iZ <<= 8;
				}
			}
		}
		else
		{
			sSpawn = pcInstance->GetBattleRoyale()->GetWaitingPosition();

			sSpawn.iX += (rand() % 8192);
			sSpawn.iZ += (rand() % 8192);
		}
	}
	else
	{
		sSpawn = (rand() % 2) == 0 ? Point3D( 2592 << 8, 0, -18566 << 8 ) : Point3D( -1047 << 8, 0, -16973 << 8 );

		sSpawn.iX += (rand() % 8192);
		sSpawn.iZ += (rand() % 8192);
	}

	PacketWarpGateField sPacket;
	sPacket.iLength = sizeof( PacketWarpGateField );
	sPacket.iHeader = PKTHDR_WarpGateField;
	sPacket.iX		= sSpawn.iX;
	sPacket.iZ		= sSpawn.iZ;
	sPacket.iStage	= bArena ? pcInstance->GetBattleRoyale()->GetMapID() : MAPID_RicartenTown;

	SENDPACKET( pcUser, &sPacket );

	if ( bArena && (bRandomPosition == false) )
		ProcessPacket( pcUser, &PacketBattleRoyaleBattleTime( FALSE, TRUE ) );
}

BOOL CBattleRoyaleHandler::IsPVPMap( EMapID eMapID )
{
	BOOL bRet = FALSE;
	for ( auto pcInstance : vInstances )
	{
		if ( pcInstance->GetBattleRoyale()->GetMapID() == eMapID )
		{
			if ( pcInstance->GetState().eGameState == GAMESTATE_InBattle )
			{
				bRet = TRUE;
				break;
			}
		}
	}

	return bRet;
}

void CBattleRoyaleHandler::HandlePacket( User * pcUser, PacketBattleRoyaleGasDeath * psPacket )
{
	OnGasKilled( pcUser );
}

void CBattleRoyaleHandler::HandlePacket( PacketNetBattleRoyaleStatus * psPacket )
{
	//Update User Status on LoginServer
	if ( auto pcUser = USERDATATOUSER( USERDATABYID( psPacket->iID ) ) )
		pcUser->bInBattleRoyale = psPacket->bInBattleRoyale;
}

void CBattleRoyaleHandler::HandlePacket( PacketNetBattleRoyaleEnter * psPacket )
{
	if ( GAME_SERVER )
		return;

	if ( auto pcUser = USERDATATOUSER( USERDATABYID( psPacket->iID ) ) )
		CHARACTERSERVER->CopyCharacterBattleRoyale( psPacket->szCharacterName );
}

void CBattleRoyaleHandler::HandlePacket( User * pcUser, PacketBattleRoyaleRequest * psPacket )
{
	//Entrance
	if ( psPacket->bReward == FALSE )
	{
		if ( psPacket->iBattleID >= 0 )
		{
			bool bCanEnter = true;

			//User already on Party?
			if ( pcUser->bParty )
			{
				CHATSERVER->SendChat( pcUser, CHATCOLOR_Error, "> You are in a party! Please, leave it before BattleRoyale!" );

				bCanEnter = false;
			}

			//Hardware ID
			for ( UINT i = 0; i < USERSERVER->uUsersInGame; i++ )
			{
				User * pc = USERSERVER->pcaUserInGame[i];
				if ( (pcUser != pc) && pc->bInBattleRoyale )
				{
					if ( pc->szHardwareID[0] != 0 )
					{
						if ( STRINGCOMPARE( pc->szHardwareID, pcUser->szHardwareID ) )
						{
							CHATSERVER->SendChat( pcUser, CHATCOLOR_Error, "BattleRoyale> Players must be distinct computers!" );

							bCanEnter = false;
							break;
						}
					}
				}
			}

			if ( bCanEnter )
			{
				//Net Server
				NETSERVER->SendBattleRoyaleEnter( pcUser );

				if ( AddUser( pcUser, psPacket->iBattleID ) )
				{
					for ( auto pcInstance : vInstances )
					{
						if ( pcInstance->GetState().eGameState == GAMESTATE_WaitingForPlayers )
						{
							if ( pcInstance->GetBattleRoyale()->GetID() == psPacket->iBattleID )
							{
								SQLNewCharacterInstance( pcInstance->GetBattleRoyale()->GetID(), pcInstance->GetID(), pcUser->iCharacterID );

								SendUserCoordinateList( pcUser, pcInstance );
								break;
							}
						}
					}

					ProcessPacket( pcUser, &PacketBattleRoyaleResponse( 2 ) );
				}
			}
		}
		else
			RequestBattleList( pcUser );
	}
	else //Reward
	{
		if ( psPacket->iRewardID >= 0 )
		{
			//Get Reward User Data
			int iBattleID = -1;
			int iInstanceID = -1;
			BOOL bWinner = FALSE;
			int iKills = 0;
			EPlayerTeam eTeamID = EPlayerTeam::PLAYERTEAM_None;
			SYSTEMTIME sDate;
			if ( SQLGetUserReward( pcUser->iCharacterID, iBattleID, iInstanceID, iKills, bWinner, eTeamID, sDate ) )
			{
				BOOL bCanRoll = bWinner;
				if ( bCanRoll )
				{
					if ( iBattleID == BATTLEROYALE_TeamRoyalOasis )
					{
						if ( iKills <= 0 )
							bCanRoll = FALSE;
					}
				}

				if ( bCanRoll )
				{
					std::vector<BattleRoyaleRewardServer *> vRewards;

					//Get rewards
					if ( SQLGetRewardList( psPacket->iBattleID, vRewards ) )
					{
						for ( auto psReward : vRewards )
						{
							//Found reward?
							if ( psReward->iID == psPacket->iRewardID )
							{
								//Send to the User
								ITEMSERVER->AddItemOpenBox( pcUser->pcUserData->szAccountName, psReward->szItemCode, 0, psReward->iQuantity );

								//Set done
								SQLSetUserReceiveReward( pcUser, iBattleID, iInstanceID, SYSTEMTIME{}, true );
								SQLUpdateUserReceivedReward( iBattleID, iInstanceID, pcUser->iCharacterID, sDate );

								//Log
								LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_BattleRoyaleReceivedItem, "Character[%s] Received BattleRoyale Reward Item[%s] Quantity[%d]",
														CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), psReward->szItemCode, psReward->iQuantity );

								//Notify
								CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Global, "BattleRoyale> You received %s!", psReward->szName );
								CHATSERVER->SendChatMessageBox( pcUser, "You received %s!", psReward->szName );

								if ( iBattleID == BATTLEROYALE_Normal )
								{
									SYSTEMTIME sStartDate, sDieDate;
									if ( SQLGetLastInstanceDeath( pcUser, psPacket->iBattleID, psPacket->iInstanceID, sStartDate, sDieDate ) )
									{
										UINT64 iTimeStart = SystemTimeToQWORD( &sStartDate );
										UINT64 iTimeDie = SystemTimeToQWORD( &sDieDate );

										int iMinutes = (int)(iTimeDie - iTimeStart) / 60;

										SendEXPPrizeGold( pcUser, (EBattleRoyaleID)iBattleID, iInstanceID, iKills, bWinner, iMinutes, sDate );
									}
								}
								else
									SendEXPPrizeGold( pcUser, (EBattleRoyaleID)iBattleID, iInstanceID, iKills, bWinner, 0, sDate );

								ProcessPacket( pcUser, &PacketBattleRoyaleResponse( 1 ) );

								break;
							}
						}
					}

					for ( auto psReward : vRewards )
						DELET( psReward );

					vRewards.clear();
				}
				//EXP
				else if ( psPacket->bGiveGoldEXP )
				{
					if ( iBattleID == BATTLEROYALE_Normal )
					{
						SYSTEMTIME sStartDate, sDieDate;
						if ( SQLGetLastInstanceDeath( pcUser, psPacket->iBattleID, psPacket->iInstanceID, sStartDate, sDieDate ) )
						{
							UINT64 iTimeStart = SystemTimeToQWORD( &sStartDate );
							UINT64 iTimeDie = SystemTimeToQWORD( &sDieDate );

							int iMinutes = (int)(iTimeDie - iTimeStart) / 60;

							SendEXPPrizeGold( pcUser, (EBattleRoyaleID)iBattleID, iInstanceID, iKills, bWinner, iMinutes, sDate );
						}
					}
					else
						SendEXPPrizeGold( pcUser, (EBattleRoyaleID)iBattleID, iInstanceID, iKills, bWinner, 0, sDate );

					ProcessPacket( pcUser, &PacketBattleRoyaleResponse( 1 ) );
				}
			}
		}
		else
		{
			std::vector<BattleRoyaleRewardServer *> vRewards;

			std::vector<PacketBattleRoyaleRewardList> vPackets;

			PacketBattleRoyaleRewardList sPacket;
			sPacket.iCount = 0;
			sPacket.sPacketCount.sMax = sPacket.sPacketCount.sMin = 0;

			//Get Reward User Data
			int iBattleID = -1;
			int iInstanceID = -1;
			BOOL bWinner = FALSE;
			int iKills = 0;
			EPlayerTeam eTeamID = EPlayerTeam::PLAYERTEAM_None;
			SYSTEMTIME sDate;
			if ( SQLGetUserReward( pcUser->iCharacterID, iBattleID, iInstanceID, iKills, bWinner, eTeamID, sDate ) )
			{
				BOOL bCanRoll = bWinner;
				if ( bCanRoll )
				{
					if ( iBattleID == BATTLEROYALE_TeamRoyalOasis )
					{
						if ( iKills <= 0 )
							bCanRoll = FALSE;
					}
				}

				if ( bCanRoll )
				{
					sPacket.iBattleID = iBattleID;
					sPacket.iInstanceID = iInstanceID;

					//Get rewards
					if ( SQLGetRewardList( iBattleID, vRewards ) )
					{
						//Get rewards
						for ( auto psReward : vRewards )
						{
							sPacket.saRewards[sPacket.iCount].iID = psReward->iID;
							sPacket.saRewards[sPacket.iCount].iChance = psReward->iChance;
							STRINGCOPY( sPacket.saRewards[sPacket.iCount].szName, psReward->szName );
							STRINGCOPY( sPacket.saRewards[sPacket.iCount].szImagePath, psReward->szImagePath );

							sPacket.iCount++;
							if ( sPacket.iCount == MAX_BATTLEROYALEREWARD )
							{
								sPacket.sPacketCount.sMin++;

								vPackets.push_back( sPacket );

								sPacket.iCount = 0;
							}
						}
					}
					else
					{
						SYSTEMTIME sStartDate, sDieDate;
						if ( SQLGetLastInstanceDeath( pcUser, psPacket->iBattleID, psPacket->iInstanceID, sStartDate, sDieDate ) )
						{
							sPacket.iBattleID = psPacket->iBattleID;
							sPacket.iInstanceID = psPacket->iInstanceID;

							sPacket.saRewards[sPacket.iCount].iID = 0;
							sPacket.saRewards[sPacket.iCount].iChance = 10000;
							STRINGCOPY( sPacket.saRewards[sPacket.iCount].szName, "1bil EXP" );
							STRINGCOPY( sPacket.saRewards[sPacket.iCount].szImagePath, "" );

							sPacket.iCount++;
						}
					}
				}
				else
				{
					if ( iBattleID == BATTLEROYALE_Normal )
					{
						SYSTEMTIME sStartDate, sDieDate;
						if ( SQLGetLastInstanceDeath( pcUser, psPacket->iBattleID, psPacket->iInstanceID, sStartDate, sDieDate ) )
						{
							sPacket.iBattleID = psPacket->iBattleID;
							sPacket.iInstanceID = psPacket->iInstanceID;

							sPacket.saRewards[sPacket.iCount].iID = 0;
							sPacket.saRewards[sPacket.iCount].iChance = 10000;
							STRINGCOPY( sPacket.saRewards[sPacket.iCount].szName, "1bil EXP" );
							STRINGCOPY( sPacket.saRewards[sPacket.iCount].szImagePath, "" );

							sPacket.iCount++;
						}
					}
					else
					{
						sPacket.iBattleID = psPacket->iBattleID;
						sPacket.iInstanceID = psPacket->iInstanceID;

						sPacket.saRewards[sPacket.iCount].iID = 0;
						sPacket.saRewards[sPacket.iCount].iChance = 10000;
						STRINGCOPY( sPacket.saRewards[sPacket.iCount].szName, "1bil EXP" );
						STRINGCOPY( sPacket.saRewards[sPacket.iCount].szImagePath, "" );

						sPacket.iCount++;
					}
				}
			}

			for ( auto psReward : vRewards )
				DELET( psReward );

			sPacket.sPacketCount.sMin++;
			vPackets.push_back( sPacket );

			for ( auto & sSendPacket : vPackets )
			{
				sSendPacket.sPacketCount.sMax = (short)vPackets.size();

				ProcessPacket( pcUser, &sSendPacket );
			}

			vPackets.clear();

			vRewards.clear();
		}
	}
}

void CBattleRoyaleHandler::ProcessStart( CBattleRoyaleInstance * pcInstance, BOOL bStart )
{
	//Send Packet to Users
	for ( auto psPlayer : pcInstance->GetPlayers() )
	{
		for ( UINT i = 0; i < USERSERVER->uUsersInGame; i++ )
		{
			User * pcUser = USERSERVER->pcaUserInGame[i];
			if ( (pcUser->GetID() == psPlayer->iID) && (psPlayer->iTimeDeath == 0) )
				ProcessPacket( pcUser, &PacketBattleRoyaleBattleTime( bStart ) );
		}
	}
}

void CBattleRoyaleHandler::ProcessPacket( User * pcUser, PacketBattleRoyaleBattleTime * psPacket )
{
	psPacket->iLength = sizeof( PacketBattleRoyaleBattleTime );
	psPacket->iHeader = PKTHDR_BattleRoyaleBattleTime;
	SENDPACKET( pcUser, psPacket );
}

void CBattleRoyaleHandler::ProcessPacket( User * pcUser, PacketBattleRoyaleList * psPacket )
{
	psPacket->iLength = sizeof( PacketBattleRoyaleList );
	psPacket->iHeader = PKTHDR_BattleRoyaleList;
	SENDPACKET( pcUser, psPacket );
}

void CBattleRoyaleHandler::ProcessPacket( User * pcUser, PacketBattleRoyaleRewardList * psPacket )
{
	psPacket->iLength = sizeof( PacketBattleRoyaleRewardList );
	psPacket->iHeader = PKTHDR_BattleRoyaleRewardList;
	SENDPACKET( pcUser, psPacket );
}

void CBattleRoyaleHandler::ProcessPacket( User * pcUser, PacketBattleRoyaleResponse * psPacket )
{
	psPacket->iLength = sizeof( PacketBattleRoyaleResponse );
	psPacket->iHeader = PKTHDR_BattleRoyaleResponse;
	SENDPACKET( pcUser, psPacket );
}

void CBattleRoyaleHandler::ProcessPacket( User * pcUser, PacketBattleRoyaleTeleports * psPacket )
{
	psPacket->iLength = sizeof( PacketBattleRoyaleTeleports );
	psPacket->iHeader = PKTHDR_BattleRoyaleTeleports;
	SENDPACKET( pcUser, psPacket );
}

float CBattleRoyaleHandler::GetHandicap( User * pcUser, BattleRoyale::EHandicapType eHandicapType, BOOL bItem )
{
	float fRet = 0.0f;

	if ( IsInBattleRoyale( pcUser ) )
	{
		if ( auto pcInstance = GetInstance( pcUser ) )
		{
			auto vBattleHandicap = pcInstance->GetBattleRoyale()->GetBattleHandicap();
			for ( auto psBattleHandicap : vBattleHandicap )
			{
				if ( psBattleHandicap->iCharacterClass == pcUser->pcUserData->sCharacterData.iClass )
				{
					if ( psBattleHandicap->bItem == bItem )
					{
						if ( eHandicapType == HANDICAPTYPE_Damage )
							fRet = psBattleHandicap->fDamage;
						else if ( eHandicapType == HANDICAPTYPE_AttackRating )
							fRet = psBattleHandicap->fAttackRating;

						break;
					}
				}
			}
		}
	}

	return fRet;
}

BOOL CBattleRoyaleHandler::IsDropableItemUser( User * pcUser )
{
	if ( auto pcInstance = GetInstance( pcUser ) )
	{
		if ( pcInstance->GetState().eGameState == GAMESTATE_End )
			return FALSE;
	}

	return TRUE;
}

BOOL CBattleRoyaleHandler::DropItemBattle( User * pcUser, Item & sItem )
{
	BOOL bRet = FALSE;

	if ( auto pcInstance = GetInstance( pcUser ) )
	{
		//Get Map Data
		Map * pcMap = MAPSDATA + pcUser->GetMapID();
		if ( MAPSERVER->IsValidMap( pcMap ) )
		{
			//Add Loot
			Map::Loot::LootItem sLootItem;
			CopyMemory( &sLootItem.sItem, &sItem, sizeof( Item ) );

			Point3D sPosition = pcUser->pcUserData->sPosition;
			sPosition.iX >>= 8;
			sPosition.iZ >>= 8;
			sPosition.iY = pcMap->pcStage->GetHighestPoint( sPosition.iX << 8, sPosition.iZ << 8 ) >> 8;

			//Add Item to Map
			Map::Loot * psItemD = pcMap->AddItem( &sLootItem, sPosition.iX, sPosition.iY, sPosition.iZ, TRUE );

			//Added? Send It to near users...
			if ( psItemD )
			{
				//Exclusive ID
				psItemD->bExclusive = TRUE;
				psItemD->lExclusiveOwnerID = pcInstance->GetDropID();

				//Send
				MAPSERVER->SendItemStageNearUsers( pcMap, psItemD );

				bRet = TRUE;
			}
		}
	}

	return bRet;
}

void CBattleRoyaleHandler::SQLLogBattleRoyaleUsersInRange( User * pcUser )
{
	if ( pcUser->bInBattleRoyale == false )
		return;

	if ( auto pcInstance = GetInstance( pcUser ) )
	{
		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );
		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "INSERT INTO BattleRoyaleUsersRangeLog([InstanceID],[CharacterID],[UsersInRange],[Date]) VALUES (?,?,?,GETDATE())" ) )
			{
				int iID = pcInstance->GetID();

				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iID );
				pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &pcUser->iCharacterID );
				pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &pcUser->pcUserData->uBattleRoyaleUsersNearCount );
				pcDB->Execute();
			}
			pcDB->Close();
		}
	}
}

EBattleRoyaleStateEnemy CBattleRoyaleHandler::GetStateEnemy( User * pcUser, User * pcTarget )
{
	EBattleRoyaleStateEnemy eRet = BATTLEROYALESTATEENEMY_None;
	if ( IsInBattleRoyale( pcUser ) && IsInBattleRoyale( pcTarget ) )
	{
		if ( auto pcInstance = GetInstance( pcUser ) )
		{
			if ( pcInstance == GetInstance( pcTarget ) )
			{
				eRet = BATTLEROYALESTATEENEMY_Enemy;
				if ( pcInstance->GetState().eGameState == GAMESTATE_InBattle )
				{
					eRet = BATTLEROYALESTATEENEMY_InBattleEnemy;

					PlayerState * psPlayerUser = nullptr;
					PlayerState * psPlayerTarget = nullptr;
					if ( pcInstance->GetBattleRoyale()->IsTeamRoyal() )
					{
						eRet = BATTLEROYALESTATEENEMY_Friendly;
						for ( auto psPlayer : pcInstance->GetPlayers() )
						{
							if ( (psPlayerUser == nullptr) && (psPlayer->iID == pcUser->GetID()) )
								psPlayerUser = psPlayer;

							if ( (psPlayerTarget == nullptr) && (psPlayer->iID == pcTarget->GetID()) )
								psPlayerTarget = psPlayer;

							if ( psPlayerUser && psPlayerTarget )
							{
								eRet = psPlayerUser->eTeamID != psPlayerTarget->eTeamID ? BATTLEROYALESTATEENEMY_InBattleEnemy : BATTLEROYALESTATEENEMY_Friendly;
								break;
							}
						}
					}
				}
			}
		}
	}

    return eRet;
}

bool CBattleRoyaleHandler::AddInstanceTimeSeconds( User * pcUser, int iSeconds )
{
	bool bRet = false;

	if ( IsInBattleRoyale( pcUser ) )
	{
		if ( auto pcInstance = GetInstance( pcUser ) )
		{
			pcInstance->GetState().iTimeUpdate += iSeconds * 1000;
			bRet = true;
		}
	}

	return bRet;
}

BOOL CBattleRoyaleHandler::OnNPCClick( User * pcUser, Unit * pcUnit )
{
	if ( pcUnit->pcUnitData->sUnitInfo.iSmelting == NPCEVENTCODE_BattleRoyaleNPC )
	{
		char szReason[128] = { 0 };
		SYSTEMTIME sUnbanDate;
		if ( SQLIsAccountBanned( pcUser->iAccountID, sUnbanDate, szReason ) == false )
		{
			std::vector<BattleRoyaleBattleServer *> vBattles;
			if ( GetBattleList( vBattles ) )
			{
				PacketBattleRoyaleList sPacket;
				sPacket.iCount = 0;

				for ( auto psBattle : vBattles )
				{
					sPacket.saList[sPacket.iCount].iID = psBattle->iID;
					sPacket.saList[sPacket.iCount].bOpen = psBattle->bOpen;
					sPacket.saList[sPacket.iCount].iMapID = (EMapID)psBattle->iMapID;
					sPacket.saList[sPacket.iCount].sRequiredLevel = psBattle->sRequiredLevel;
					sPacket.saList[sPacket.iCount].iTimeLeft = psBattle->iTimeLeft;
					STRINGCOPY( sPacket.saList[sPacket.iCount].szName, psBattle->szName );
					sPacket.iCount++;
				}

				ProcessPacket( pcUser, &sPacket );
			}

			for ( auto psBattle : vBattles )
				DELET( psBattle );

			vBattles.clear();
		}
		else
		{
			std::string strBanned = FormatString( "You have been banned from Battle Royale until %02d-%02d-%04d %02d:%02d",
				sUnbanDate.wDay, sUnbanDate.wMonth, sUnbanDate.wYear, sUnbanDate.wHour, sUnbanDate.wMinute );

			CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Global, "> %s", strBanned.c_str() );
			if ( szReason[0] != 0 )
				CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Global, "> Reason: %s", szReason );
			
			CHATSERVER->SendChatMessageBox( pcUser, "%s", strBanned.c_str() );
		}
		return TRUE;
	}

	return FALSE;
}

void CBattleRoyaleHandler::Clear()
{
	//Remove Ended Battles
	for ( auto pcInstance : vInstances )
	{
		ROOMINSTANCEHANDLER->Delete( pcInstance->GetRoom() );

		DELET( pcInstance );
	}

	vInstances.clear();

	for ( auto pc : vBattleRoyales )
		DELET( pc );

	vBattleRoyales.clear();
}

void CBattleRoyaleHandler::Remove( int iBattleID )
{
	for ( auto it = vBattleRoyales.begin(); it != vBattleRoyales.end(); it++ )
	{
		CBattleRoyale * pcBattleRoyale = (*it);
		if ( pcBattleRoyale->GetID() == iBattleID )
		{
			//Remove Users
			for ( auto it = vInstances.begin(); it != vInstances.end(); )
			{
				auto pcInstance = (*it);
				if ( pcInstance->GetBattleRoyale() == pcBattleRoyale )
				{
					ROOMINSTANCEHANDLER->Delete( pcInstance->GetRoom() );

					DELET( pcInstance );
					it = vInstances.erase( it );
				}
				else
					it++;
			}

			DELET( pcBattleRoyale );
			it = vBattleRoyales.erase( it );
		}
		else
			it++;
	}
}

bool CBattleRoyaleHandler::SQLGetBattleRoyale( int iID, CBattleRoyale * pcBattleRoyale )
{
	bool bRet = false;
	
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT Name, MapID, MinUsers, MaxUsers, MinLevel, MaxLevel, SpawnX, SpawnZ, KillsNeeded, Duration FROM BattleRoyaleList WHERE (ID=?) AND (Enabled=1)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iID );
			if ( pcDB->Execute() )
			{
				if ( pcDB->Fetch() )
				{
					char szName[32]		= {0};
					EMapID eMapID		= MAPID_Invalid;
					IMinMax sRequiredUsers, sRequiredLevel;
					int iKillsNeeded	= 0;
					int iDuration		= 0;
					Point3D sSpawnPosition = Point3D( 0, 0, 0 );


					pcDB->GetData( 1, PARAMTYPE_String, szName, _countof( szName ) );
					pcDB->GetData( 2, PARAMTYPE_Integer, &eMapID );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sRequiredUsers.iMin );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sRequiredUsers.iMax );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sRequiredLevel.iMin );
					pcDB->GetData( 6, PARAMTYPE_Integer, &sRequiredLevel.iMax );
					pcDB->GetData( 7, PARAMTYPE_Integer, &sSpawnPosition.iX );
					pcDB->GetData( 8, PARAMTYPE_Integer, &sSpawnPosition.iZ );
					pcDB->GetData( 9, PARAMTYPE_Integer, &iKillsNeeded );
					pcDB->GetData( 10, PARAMTYPE_Integer, &iDuration );

					sSpawnPosition.iX <<= 8;
					sSpawnPosition.iZ <<= 8;

					pcBattleRoyale->SetID( iID );
					pcBattleRoyale->SetName( szName );
					pcBattleRoyale->SetMapID( eMapID );
					pcBattleRoyale->GetRequiredUsers() = sRequiredUsers;
					pcBattleRoyale->GetRequiredLevel() = sRequiredLevel;
					pcBattleRoyale->SetKillsNeeded( iKillsNeeded );
					pcBattleRoyale->SetDuration( iDuration * 60 * 1000 );
					pcBattleRoyale->SetWaitingPosition( sSpawnPosition );

					pcBattleRoyale->SetAnnouncement( ANNOUNCEMENTTYPE_None );

					bRet = true;
				}
			}
		}

		pcDB->Close();
	}

	if ( bRet )
	{
		SQLGetBatteRoyaleTime( pcBattleRoyale );
		SQLGetBattleRoyaleDrop( pcBattleRoyale );
		SQLGetBatteRoyaleCoordinates( pcBattleRoyale );
		SQLGetHandicap( pcBattleRoyale );
	}
	return bRet;
}

bool CBattleRoyaleHandler::SQLGetBattleRoyaleDrop( CBattleRoyale * pcBattleRoyale )
{
	bool bRet = false;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT Items, Chance FROM BattleRoyaleDropItem WHERE BattleID=?" ) )
		{
			int iID = pcBattleRoyale->GetID();
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iID );
			if ( pcDB->Execute() )
			{
				int iTotalChance = 0;

				while ( pcDB->Fetch() )
				{
					char szItems[512] = {0};
					int iChance = 0;
					pcDB->GetData( 1, PARAMTYPE_String, szItems, _countof( szItems ) );
					pcDB->GetData( 2, PARAMTYPE_Integer, &iChance );

					auto vItems = split( szItems, ' ' );
					if ( vItems.size() > 0 )
					{
						std::vector<DropSettingsData*> vDropData;

						for ( auto & strItem : vItems )
						{
							if ( auto pcItemData = ITEMSERVER->FindItemPointerTable( strItem.c_str() ) )
							{
								auto psDrop			= new DropSettingsData;
								psDrop->iItemID		= pcItemData->sBaseItemID.ToInt();
								psDrop->sGoldMax	= psDrop->sGoldMin = 0;
								psDrop->iPercent	= 0;
								vDropData.push_back( psDrop );
							}
						}

						if ( vDropData.size() > 0 )
						{
							for ( auto ps : vDropData )
								ps->iPercent = iChance / vDropData.size();
						}

						bRet = true;

						pcBattleRoyale->GetDrops().push_back( vDropData );
						iTotalChance += iChance;
					}
				}

				pcBattleRoyale->SetTotalDropChance( iTotalChance );
			}
		}
		pcDB->Close();
	}

	return bRet;
}

bool CBattleRoyaleHandler::SQLGetBatteRoyaleCoordinates( CBattleRoyale * pcBattleRoyale )
{
	bool bRet = false;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT North, South, West, East FROM BattleRoyaleCoordinates WHERE BattleID=?" ) )
		{
			int iID = pcBattleRoyale->GetID();
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iID );

			if ( pcDB->Execute() )
			{
				while( pcDB->Fetch() )
				{
					int iNorth	= 0;
					int iSouth	= 0;
					int iWest	= 0;
					int iEast	= 0;

					pcDB->GetData( 1, PARAMTYPE_Integer, &iNorth );
					pcDB->GetData( 2, PARAMTYPE_Integer, &iSouth );
					pcDB->GetData( 3, PARAMTYPE_Integer, &iWest );
					pcDB->GetData( 4, PARAMTYPE_Integer, &iEast );

					pcBattleRoyale->GetCoordinates().push_back( new SpawnCoordinate( iNorth, iSouth, iWest, iEast ) );

					bRet = true;
				}
			}
		}
		pcDB->Close();
	}

	return bRet;
}

bool CBattleRoyaleHandler::SQLGetBatteRoyaleTime( CBattleRoyale * pcBattleRoyale )
{
	bool bRet = false;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT DayWeek, Times FROM BattleRoyaleTime WHERE BattleID=?" ) )
		{
			int iID = pcBattleRoyale->GetID();
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iID );

			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
					int iDayWeek = -1;
					char szTimes[128] = { 0 };

					pcDB->GetData( 1, PARAMTYPE_Integer, &iDayWeek );
					pcDB->GetData( 2, PARAMTYPE_String, &szTimes, _countof( szTimes ) );

					BattleTimes * psBattleTimes = new BattleTimes();
					psBattleTimes->iDayWeek = iDayWeek;

					std::vector<std::string> vTimes = split( szTimes, ' ' );
					for ( auto & strTime : vTimes )
					{
						std::vector<std::string> vSplitTime = split( strTime, ':' );
						if ( vSplitTime.size() == 2 )
							psBattleTimes->Add( atoi( vSplitTime[0].c_str() ), atoi( vSplitTime[1].c_str() ) );
					}

					pcBattleRoyale->AddTime( psBattleTimes );

					bRet = true;
				}
			}
		}
		pcDB->Close();
	}

	return bRet;
}

bool CBattleRoyaleHandler::SQLGetHandicap( CBattleRoyale * pcBattleRoyale )
{
	bool bRet = false;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT CharacterClass, AttackRating, Damage, IsItem FROM BattleRoyaleHandicap WHERE BattleID=?" ) )
		{
			int iID = pcBattleRoyale->GetID();
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iID );

			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
					ECharacterClass iCharacterClass = CHARACTERCLASS_None;
					float fAttackRating				= 0.0f;
					float fDamage					= 0.0f;
					BOOL bItem						= FALSE;


					pcDB->GetData( 1, PARAMTYPE_Integer, &iCharacterClass );
					pcDB->GetData( 2, PARAMTYPE_Float, &fAttackRating );
					pcDB->GetData( 3, PARAMTYPE_Float, &fDamage );
					pcDB->GetData( 4, PARAMTYPE_Integer, &bItem );

					pcBattleRoyale->GetBattleHandicap().push_back( new BattleHandicap( iCharacterClass, fAttackRating, fDamage, bItem ) );

					bRet = true;
				}
			}
		}
		pcDB->Close();
	}

	return bRet;
}

bool CBattleRoyaleHandler::SQLGetListID( std::vector<int>& vListID )
{
	bool bRet = false;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT ID From BattleRoyaleList WHERE Enabled=1 ORDER BY ID ASC" ) ) 
		{
			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
					int iID = -1;
					pcDB->GetData( 1, PARAMTYPE_Integer, &iID );
					vListID.push_back( iID );
				}
			}
		}

		pcDB->Close();
	}

	if ( vListID.size() > 0 )
		bRet = true;

	return bRet;
}

int CBattleRoyaleHandler::SQLLastInstanceID()
{
	int iRet = -1;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT TOP 1 InstanceID FROM BattleRoyaleInstance ORDER BY ID DESC" ) )
		{
			if ( pcDB->Execute() )
			{
				if ( pcDB->Fetch() )
					pcDB->GetData( 1, PARAMTYPE_Integer, &iRet );
			}
		}

		pcDB->Close();
	}

	if ( iRet == -1 )
		iRet = rand() % 1000;

	return iRet;
}

void CBattleRoyaleHandler::SQLNewBattle( int iBattleID, int iInstanceID )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "INSERT INTO BattleRoyaleInstance([BattleID],[InstanceID],[UsersCount],[CharacterWinnerID],[EndDate],[ReceiveRewardDate],[Date]) VALUES(?,?,0,-1,NULL,NULL,GETDATE())" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iBattleID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iInstanceID );
			
			pcDB->Execute();
		}

		pcDB->Close();
	}
}

void CBattleRoyaleHandler::SQLStartBattle( int iInstanceID, int iUsersCount )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE BattleRoyaleInstance SET [UsersCount]=? WHERE InstanceID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iUsersCount );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iInstanceID );

			pcDB->Execute();
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "UPDATE BattleRoyaleCharacterInstance SET [StartDate]=GETDATE() WHERE InstanceID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iInstanceID );

			pcDB->Execute();
		}

		pcDB->Close();
	}
}

void CBattleRoyaleHandler::SQLEndBattle( int iInstanceID, int iWinnerCharacterID )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE BattleRoyaleInstance SET [CharacterWinnerID]=?, [EndDate]=GETDATE() WHERE InstanceID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iWinnerCharacterID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iInstanceID );

			pcDB->Execute();
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "UPDATE BattleRoyaleCharacterInstance SET [EndDate]=GETDATE() WHERE InstanceID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iInstanceID );

			pcDB->Execute();
		}

		pcDB->Close();
	}
}

void CBattleRoyaleHandler::SQLKilledInBattle( int iInstanceID, int iCharacterID, int iCharacterKillerID )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE BattleRoyaleCharacterInstance SET [DieDate]=GETDATE(), [CharacterKillerID]=? WHERE (InstanceID=?) AND (CharacterID=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iCharacterKillerID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iInstanceID );
			pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &iCharacterID );

			pcDB->Execute();
		}

		pcDB->Close();
	}
}

void CBattleRoyaleHandler::SQLWinBattle( int iInstanceID, int iCharacterID )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE BattleRoyaleCharacterInstance SET [Winner]=1 WHERE (InstanceID=?) AND (CharacterID=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iInstanceID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iCharacterID );

			pcDB->Execute();
		}

		pcDB->Close();
	}
}

void CBattleRoyaleHandler::SQLNewCharacterInstance( int iBattleID, int iInstanceID, int iCharacterID )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "INSERT INTO BattleRoyaleCharacterInstance([BattleID],[InstanceID],[CharacterID],[CharacterKillerID],[StartDate],[EndDate],[DieDate],[ReceiveRewardDate],[Winner]) VALUES(?,?,?,-1,NULL,NULL,NULL,NULL,0)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iBattleID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iInstanceID );
			pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &iCharacterID );

			pcDB->Execute();
		}

		pcDB->Close();
	}
}

bool CBattleRoyaleHandler::SQLGetRewardList( int iBattleID, std::vector<BattleRoyaleRewardServer *> & vRewards )
{
	bool bRet = false;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT ID, Name, ItemCode, ItemImage, Quantity, Chance FROM BattleRoyaleReward WHERE BattleID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iBattleID );

			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
					BattleRoyaleRewardServer * psReward = new BattleRoyaleRewardServer;

					pcDB->GetData( 1, PARAMTYPE_Integer, &psReward->iID );
					pcDB->GetData( 2, PARAMTYPE_String, psReward->szName, _countof( psReward->szName ) );
					pcDB->GetData( 3, PARAMTYPE_String, psReward->szItemCode, _countof( psReward->szItemCode ) );
					pcDB->GetData( 4, PARAMTYPE_String, psReward->szImagePath, _countof( psReward->szImagePath ) );
					pcDB->GetData( 5, PARAMTYPE_Integer, &psReward->iQuantity );
					pcDB->GetData( 6, PARAMTYPE_Integer, &psReward->iChance );

					vRewards.push_back( psReward );

					bRet = true;
				}
			}
		}

		pcDB->Close();
	}

	return bRet;
}

bool CBattleRoyaleHandler::SQLGetUserInstanceID( User * pcUser, int & iInstanceID, int & iBattleID )
{
	bool bRet = false;

	if ( pcUser->iCharacterID >= 0 )
	{
		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "SELECT TOP 1 InstanceID, BattleID FROM BattleRoyaleInstance WHERE (CharacterWinnerID=?) AND (ReceiveRewardDate IS NULL)" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );

				if ( pcDB->Execute() )
				{
					if ( pcDB->Fetch() )
					{
						pcDB->GetData( 1, PARAMTYPE_Integer, &iInstanceID );
						pcDB->GetData( 2, PARAMTYPE_Integer, &iBattleID );

						bRet = true;
					}
				}
			}

			pcDB->Close();
		}
	}

	return bRet;
}

bool CBattleRoyaleHandler::GetBattleList( std::vector<BattleRoyaleBattleServer *> & vBattles )
{
	bool bRet = false;

	SYSTEMTIME sServerTime;
	CopyMemory( &sServerTime, GetServerTime(), sizeof( SYSTEMTIME ) );

	for ( auto pcBattleRoyale : vBattleRoyales )
	{
		BattleRoyaleBattleServer * psBattle = new BattleRoyaleBattleServer;
		psBattle->iID				= pcBattleRoyale->GetID();
		psBattle->iMapID			= pcBattleRoyale->GetMapID();
		psBattle->sRequiredLevel	= pcBattleRoyale->GetRequiredLevel();
		psBattle->iTimeLeft			= pcBattleRoyale->GetTimeLeft( sServerTime );
		STRINGCOPY( psBattle->szName, pcBattleRoyale->GetName().c_str() );

		if ( auto pcInstance = GetInstance( pcBattleRoyale ) )
			psBattle->bOpen = (pcInstance->GetState().eGameState == GAMESTATE_WaitingForPlayers);
		else
			psBattle->bOpen = false;

		vBattles.push_back( psBattle );

		bRet = true;
	}

	return bRet;
}

bool CBattleRoyaleHandler::SQLUserWonInstance( User * pcUser, int iBattleID, int iInstanceID )
{
	bool bRet = false;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT ID FROM BattleRoyaleInstance WHERE (BattleID=?) AND (InstanceID=?) AND (CharacterWinnerID=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iBattleID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iInstanceID );
			pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &pcUser->iCharacterID );

			if ( pcDB->Execute() )
			{
				if ( pcDB->Fetch() )
					bRet = true;
			}
		}

		pcDB->Close();
	}

	return bRet;
}

bool CBattleRoyaleHandler::SQLGetLastInstanceDeath( User * pcUser, int & iBattleID, int & iInstanceID, SYSTEMTIME & sStartDate, SYSTEMTIME & sDieDate )
{
	bool bRet = false;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT TOP 1 BattleID, InstanceID, StartDate, DieDate FROM BattleRoyaleCharacterInstance WHERE (CharacterID=?) AND (EndDate IS NOT NULL) AND (DieDate IS NOT NULL) AND (ReceiveRewardDate IS NULL) AND (Winner=0) ORDER BY ID DESC" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );

			if ( pcDB->Execute() )
			{
				if ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &iBattleID );
					pcDB->GetData( 2, PARAMTYPE_Integer, &iInstanceID );
					pcDB->GetData( 3, PARAMTYPE_Time, &sStartDate );
					pcDB->GetData( 4, PARAMTYPE_Time, &sDieDate );

					bRet = true;
				}
			}
		}

		pcDB->Close();
	}

	return bRet;
}

void CBattleRoyaleHandler::SQLSetUserReceiveReward( User * pcUser, int iBattleID, int iInstanceID, SYSTEMTIME sDieDate, bool bWinner )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( bWinner )
		{
			if ( pcDB->Prepare( "UPDATE BattleRoyaleInstance SET [ReceiveRewardDate]=GETDATE() WHERE (InstanceID=?) AND (CharacterWinnerID=?)" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iInstanceID );
				pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &pcUser->iCharacterID );

				pcDB->Execute();
			}
			pcDB->Clear();

			if ( pcDB->Prepare( "UPDATE BattleRoyaleCharacterInstance SET [ReceiveRewardDate]=GETDATE() WHERE (InstanceID=?) AND (CharacterID=?) AND (DieDate IS NOT NULL)" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iInstanceID );
				pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &pcUser->iCharacterID );

				pcDB->Execute();
			}
		}
		else
		{
			if ( pcDB->Prepare( "UPDATE BattleRoyaleCharacterInstance SET [ReceiveRewardDate]=GETDATE() WHERE (BattleID=?) AND (CharacterID=?) AND (YEAR(DieDate)=?) AND (MONTH(DieDate)=?) AND (DAY(DieDate)=?)" ) )			
			{
				int iYear	= (int)sDieDate.wYear;
				int iMonth	= (int)sDieDate.wMonth;
				int iDay	= (int)sDieDate.wDay;

				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iBattleID );
				pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &pcUser->iCharacterID );
				pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &iYear );
				pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &iMonth );
				pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &iDay );

				pcDB->Execute();
			}
		}

		pcDB->Close();
	}
}

void CBattleRoyaleHandler::SQLUpdateCharacterRanking( User * pcUser, int iKills, BOOL bKilled, BOOL bWinner, int iPoints )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		bool bInsert = true;

		if ( pcDB->Prepare( "SELECT ID FROM BattleRoyaleCharacterRanking WHERE (CharacterID=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );
			if ( pcDB->Execute() )
			{
				if ( pcDB->Fetch() )
					bInsert = false;
			}
		}

		pcDB->Clear();
		if ( bInsert )
		{
			if ( pcDB->Prepare( "INSERT BattleRoyaleCharacterRanking([CharacterID],[Kills],[Deaths],[Wins],[Points],[Date]) VALUES(?,?,?,?,?,GETDATE())" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );
				pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iKills );
				pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &bKilled );
				pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &bWinner );
				pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &iPoints );
				pcDB->Execute();
			}
		}
		else
		{
			if ( pcDB->Prepare( "UPDATE BattleRoyaleCharacterRanking SET [Kills]=[Kills]+?, [Deaths]=[Deaths]+?, [Wins]=[Wins]+?, [Points]=[Points]+?, [Date]=GETDATE() WHERE [CharacterID]=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iKills );
				pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &bKilled );
				pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &bWinner );
				pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &iPoints );
				pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &pcUser->iCharacterID );
				pcDB->Execute();
			}
		}

		pcDB->Close();
	}
}

bool CBattleRoyaleHandler::SQLGetCharacterRanking( User * pcUser, int & iKills, int & iDeaths, int & iWins, int & iPoints, SYSTEMTIME & sDate )
{
	bool bRet = false;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT Kills, Deaths, Wins, Points, [Date] FROM BattleRoyaleCharacterRanking WHERE (CharacterID=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );
			if ( pcDB->Execute() )
			{
				if ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &iKills );
					pcDB->GetData( 2, PARAMTYPE_Integer, &iDeaths );
					pcDB->GetData( 3, PARAMTYPE_Integer, &iWins );
					pcDB->GetData( 4, PARAMTYPE_Integer, &iPoints );
					pcDB->GetData( 5, PARAMTYPE_Time, &sDate );

					bRet = true;
				}
			}
		}
		pcDB->Close();
	}

	return bRet;
}

bool CBattleRoyaleHandler::SQLGetUserReward( int iCharacterID, int & iBattleID, int & iInstanceID, int & iKills, BOOL & bWinner, BattleRoyale::EPlayerTeam & eTeamID, SYSTEMTIME & sDate )
{
	bool bRet = false;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT TOP 1 [BattleID],[InstanceID],[Kills],[TeamID],[IsWinner],[Date] FROM BattleRoyaleCharacterReward WHERE (CharacterID=?) AND (ReceivedRewardDate IS NULL) ORDER BY IsWinner DESC" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iCharacterID );
			if ( pcDB->Execute() )
			{
				if ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &iBattleID );
					pcDB->GetData( 2, PARAMTYPE_Integer, &iInstanceID );
					pcDB->GetData( 3, PARAMTYPE_Integer, &iKills );
					pcDB->GetData( 4, PARAMTYPE_Integer, &eTeamID );
					pcDB->GetData( 5, PARAMTYPE_Integer, &bWinner );
					pcDB->GetData( 6, PARAMTYPE_Time, &sDate );

					bRet = true;
				}
			}
		}

		pcDB->Close();
	}

	return bRet;
}

void CBattleRoyaleHandler::SQLAddUserReward( int iBattleID, int iInstanceID, int iCharacterID, int iKills, BOOL bWinner, BattleRoyale::EPlayerTeam eTeamID )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "INSERT INTO BattleRoyaleCharacterReward([BattleID],[InstanceID],[CharacterID],[Kills],[TeamID],[IsWinner],[ReceivedRewardDate],[Date]) VALUES(?,?,?,?,?,?,NULL,GETDATE())" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iBattleID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iInstanceID );
			pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &iCharacterID );
			pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &iKills );
			pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &eTeamID );
			pcDB->BindParameterInput( 6, PARAMTYPE_Integer, &bWinner );

			pcDB->Execute();
		}

		pcDB->Close();
	}
}

void CBattleRoyaleHandler::SQLUpdateUserReceivedReward( int iBattleID, int iInstanceID, int iCharacterID, SYSTEMTIME sDate )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE BattleRoyaleCharacterReward SET [ReceivedRewardDate]=GETDATE() WHERE (BattleID=?) AND (CharacterID=?) AND (InstanceID=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iBattleID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iCharacterID );
			pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &iInstanceID );

			pcDB->Execute();
		}

		pcDB->Close();
	}
}

CBattleRoyaleInstance * CBattleRoyaleHandler::GetInstance( int iID )
{
	for ( auto pcInstance : vInstances )
	{
		if ( pcInstance->GetID() == iID )
			return pcInstance;
	}

	return nullptr;
}

CBattleRoyaleInstance * CBattleRoyaleHandler::GetInstance( CBattleRoyale * pcBattleRoyale )
{
	for ( auto pcInstance : vInstances )
	{
		if ( pcInstance->GetBattleRoyale() == pcBattleRoyale )
			return pcInstance;
	}

	return nullptr;
}

CBattleRoyaleInstance * CBattleRoyaleHandler::GetInstance( User * pcUser )
{
	for ( auto pcInstance : vInstances )
	{
		for ( auto psPlayer : pcInstance->GetPlayers() )
		{
			if ( psPlayer->iID == pcUser->GetID() )
				return pcInstance;
		}
	}

	return nullptr;
}

bool CBattleRoyaleHandler::IsInBattleRoyale( User * pcUser )
{
	if ( pcUser->bInBattleRoyale )
		return true;

	return false;
}

void CBattleRoyaleHandler::SendChat( CBattleRoyaleInstance * pcInstance, EChatColor eColor, std::string strMessage, bool bGameMaster )
{
	for ( auto psPlayer : pcInstance->GetPlayers() )
	{
		for ( UINT i = 0; i < USERSERVER->uUsersInGame; i++ )
		{
			User * pcUser = USERSERVER->pcaUserInGame[i];
			if ( pcUser->GetID() == psPlayer->iID )
			{
				if ( bGameMaster )
				{
					if ( pcUser->GetGameLevel() >= GAMELEVEL_Two )
						CHATSERVER->SendChat( pcUser, eColor, strMessage.c_str() );
				}
				else
					CHATSERVER->SendChat( pcUser, eColor, strMessage.c_str() );
			}
		}
	}
}

BOOL CBattleRoyaleHandler::OnGasKilled( User * pcUser )
{
	if ( IsInBattleRoyale( pcUser ) )
	{
		if ( auto pcInstance = GetInstance( pcUser ) )
		{
			for ( auto psPlayer : pcInstance->GetPlayers() )
			{
				if ( (psPlayer->eState == PLAYERSTATE_Alive) && (pcUser->GetID() == psPlayer->iID) )
				{
					SQLKilledInBattle( pcInstance->GetID(), pcUser->iCharacterID );

					//Compute Score
					INT64 iPercentage = ((INT64)pcInstance->GetState().iTimeUpdate * 96) / (INT64)pcInstance->GetState().iDuration;
					SQLUpdateCharacterRanking( pcUser, 0, TRUE, FALSE, (int)iPercentage );

					psPlayer->eState = PLAYERSTATE_Dead;
					psPlayer->iTimeDeath = pcInstance->GetState().iTimeUpdate;

					SendChat( pcInstance, CHATCOLOR_Notice, FormatString( "BattleRoyale> %s killed!",
						CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) ) );

					PacketBattleRoyaleUpdate sPacket;
					sPacket.iLength = sizeof( PacketBattleRoyaleUpdate );
					sPacket.iHeader = PKTHDR_BattleRoyaleUpdate;
					CopyMemory( &sPacket.sState, &pcInstance->GetState(), sizeof( State ) );
					sPacket.sState.eGameState = GAMESTATE_End;
					SENDPACKET( pcUser, &sPacket );
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

BOOL CBattleRoyaleHandler::OnUserKilled( User * pcKiller, User * pcVictim )
{
	if ( LOGIN_SERVER )
		return FALSE;

	if ( IsInBattleRoyale( pcKiller ) && IsInBattleRoyale( pcVictim ) )
	{
		if ( auto pcInstance = GetInstance( pcVictim ) )
		{
			for ( auto psPlayer : pcInstance->GetPlayers() )
			{
				if ( (psPlayer->eState == PLAYERSTATE_Alive) && (pcVictim->GetID() == psPlayer->iID) )
				{
					SQLKilledInBattle( pcInstance->GetID(), pcVictim->iCharacterID, pcKiller->iCharacterID );

					SQLUpdateCharacterRanking( pcKiller, 1, FALSE, FALSE, RandomI( 110, 120 ) );

					//Add Kill
					for ( int i = 0; i < pcInstance->GetState().iPlayerTeamCount; i++ )
					{
						if ( pcInstance->GetState().saPlayerTeams[i].iID == pcKiller->GetID() )
						{
							pcInstance->GetState().saPlayerTeams[i].iKills++;
							break;
						}
					}

					psPlayer->iKills++;

					//Compute Score
					INT64 iPercentage = ((INT64)pcInstance->GetState().iTimeUpdate * 96) / (INT64)pcInstance->GetState().iDuration;
					SQLUpdateCharacterRanking( pcVictim, 0, TRUE, FALSE, (int)iPercentage );

					if ( (pcInstance->GetBattleRoyale()->IsTeamRoyalTickets() == false) )
					{
						psPlayer->eState = PLAYERSTATE_Dead;
						psPlayer->iTimeDeath = pcInstance->GetState().iTimeUpdate;

						SendChat( pcInstance, CHATCOLOR_Notice, FormatString( "BattleRoyale> %s killed %s!",
																			  CHARACTERSERVER->GetCharacterName( pcKiller->pcUserData ), CHARACTERSERVER->GetCharacterName( pcVictim->pcUserData ) ) );

						PacketBattleRoyaleUpdate sPacket;
						sPacket.iLength = sizeof( PacketBattleRoyaleUpdate );
						sPacket.iHeader = PKTHDR_BattleRoyaleUpdate;
						CopyMemory( &sPacket.sState, &pcInstance->GetState(), sizeof( State ) );
						sPacket.sState.eGameState = GAMESTATE_End;
						SENDPACKET( pcVictim, &sPacket );
					}

					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

BOOL CBattleRoyaleHandler::OnDisconnected( User * pcUser )
{
	if ( GAME_SERVER )
	{
		if ( IsInBattleRoyale( pcUser ) )
		{
			if ( auto pcInstance = GetInstance( pcUser ) )
			{
				SendChat( pcInstance, CHATCOLOR_Notice, FormatString( "BattleRoyale> %s disconnected!", CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) ) );

				return pcInstance->RemoveUser( pcUser, true ) ? TRUE : FALSE;
			}
		}
	}
	return FALSE;
}

BOOL CBattleRoyaleHandler::CannotSave( User * pcUser )
{
	if ( pcUser->bInBattleRoyale )
		return TRUE;

	return FALSE;
}

void CBattleRoyaleHandler::Update( ETickType eTick )
{
	if ( LOGIN_SERVER )
		return;

    if ( SERVER_CODE > 1 )
        return;

	switch ( eTick )
	{
		case TICKTYPE_1Second:
			UpdateBattleTime();
			UpdateEventTimer();
			break;
	}
}

BOOL CBattleRoyaleHandler::OnUserMapChanged( User * pcUser, EMapID eMapID )
{
	if ( GAME_SERVER )
	{
		if ( IsInBattleRoyale( pcUser ) )
		{
			if ( auto pcInstance = GetInstance( pcUser ) )
			{
				if ( (pcUser->GetMapID() == pcInstance->GetBattleRoyale()->GetMapID()) && (eMapID != pcInstance->GetBattleRoyale()->GetMapID()) )
				{
					SendChat( pcInstance, CHATCOLOR_Notice, FormatString( "BattleRoyale> %s kicked!", CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) ) );

					pcInstance->RemoveUser( pcUser );
				}

				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL CBattleRoyaleHandler::OnUserGetMapItem( User * pcUser, EItemID eItemID )
{
	if ( IsInBattleRoyale( pcUser ) )
	{
		if ( auto pcInstance = GetInstance( pcUser ) )
		{
			if ( pcInstance->GetState().eGameState == GAMESTATE_InBattle )
			{
				if ( pcInstance->GetBattleRoyale()->GetID() == BATTLEROYALE_TeamRoyalOasis )
					return FALSE;

				if ( (pcInstance->GetBattleRoyale()->GetID() == BATTLEROYALE_PvPFast) || pcInstance->GetBattleRoyale()->IsTeamRoyal() )
				{
					if ( ItemID( eItemID ).ToItemBase() == ITEMBASE_Potion )
						return TRUE;
				}
			}
		}
	}

	return FALSE;
}

void CBattleRoyaleHandler::UpdateBattleTime()
{
	//Update Battle
	for ( auto pcInstance : vInstances )
	{
		//In Battle?
		if ( pcInstance->GetState().eGameState == GAMESTATE_InBattle )
		{
			pcInstance->GetState().iTimeUpdate += 1000;

			if ( (pcInstance->GetBattleRoyale()->GetDuration() - pcInstance->GetState().iTimeUpdate) <= 4000 )
			{
				if ( pcInstance->GetState().iPlayersCount > 1 )
					pcInstance->GetState().iTimeUpdate = pcInstance->GetBattleRoyale()->GetDuration() - 4000;

				if ( pcInstance->GetState().iEndTime < MAX_BATTLEROYALE_AFTERBATTLE )
					pcInstance->GetState().iEndTime += 1000;
			}

			std::vector<int> vRemoveUsers;

			pcInstance->GetState().iPlayersCount = 0;
			for ( auto psPlayer : pcInstance->GetPlayers() )
			{
				if ( psPlayer->eState == PLAYERSTATE_Alive )
					pcInstance->GetState().iPlayersCount++;
				else if ( psPlayer->eState == PLAYERSTATE_Dead )
				{
					bool bRemove = false;
					if ( pcInstance->GetState().iTimeUpdate >= (psPlayer->iTimeDeath + (MAX_BATTLEROYALE_TIMEDEATH * 1000)) )
						bRemove = true;
					else if ( (pcInstance->GetBattleRoyale()->GetDuration() - pcInstance->GetState().iTimeUpdate) <= (MAX_BATTLEROYALE_TIMEDEATH * 1000) )
						bRemove = true;

					//Death timeout?
					if ( bRemove )
					{
						//Add to Remove
						vRemoveUsers.push_back( psPlayer->iID );
					}
					else
						pcInstance->GetState().iPlayersCount++;
				}
			}

			for ( auto iRemoveID : vRemoveUsers )
				pcInstance->RemoveUser( iRemoveID );

			vRemoveUsers.clear();

			if ( pcInstance->GetState().iEndTime >= MAX_BATTLEROYALE_AFTERBATTLE )
			{
				if ( pcInstance->GetState().iPlayersCount > 1 )
				{
					PlayerState * psPlayerAlive = nullptr;
					for ( auto psPlayer : pcInstance->GetPlayers() )
					{
						if ( psPlayer->eState == PLAYERSTATE_Alive )
						{
							psPlayerAlive = psPlayer;
							break;
						}
					}

					if ( psPlayerAlive )
					{
						for ( auto psPlayer : pcInstance->GetPlayers() )
						{
							if ( (psPlayerAlive != psPlayer) && (psPlayer->eState == PLAYERSTATE_Alive) )
							{
								for ( UINT i = 0; i < USERSERVER->uUsersInGame; i++ )
								{
									User * pcUser = USERSERVER->pcaUserInGame[i];
									if ( pcUser->GetID() == psPlayer->iID )
									{
										PacketSimple sPacket;
										sPacket.iLength = sizeof( PacketSimple );
										sPacket.iHeader = PKTHDR_TheDeath;
										sPacket.iUnk = 0;
										SENDPACKET( pcUser, &sPacket );
									}
								}
							}
						}
					}
				}

				pcInstance->GetState().iEndTime = 0;
			}

			//Update Radius
			pcInstance->GetState().iRadius	= ComputeBattleRoyaleRadius( pcInstance->GetState().iTimeUpdate, pcInstance->GetBattleRoyale()->GetDuration() );

			//Update Next Radius
			if ( pcInstance->GetState().iNextRadius <= pcInstance->GetState().iRadius )
				pcInstance->GetState().iNextRadius = (pcInstance->GetState().iRadius * RADIUS_BATTLEROYALE_NEXT) / 100;

			//Generate Meteorite?
			if ( pcInstance->GetState().iMeteoriteTime == 0 )
			{
				if ( ((pcInstance->GetState().iTimeUpdate % (60 * 1000)) == 0) && (pcInstance->GetState().iRadius > (MAX_BATTLEROYALE_RADIUS >> 1)) )
				{
					if ( RandomI( 0, 10000 ) < 100 )
					{
						Point3D sPosition;
						if ( GetRandomCoordinate( pcInstance->GetBattleRoyale(), sPosition ) )
						{
							sPosition.iX <<= 8;
							sPosition.iZ <<= 8;

							//Update Y coordinate
							Map * pcMap = MAPSDATA + pcInstance->GetBattleRoyale()->GetMapID();
							sPosition.iY = pcMap->pcStage->GetHighestPoint( sPosition.iX, sPosition.iZ );

							//Set MNeteorite
							pcInstance->GetState().iMeteoriteTime = pcInstance->GetState().iTimeUpdate + TIME_BATTLEROYALE_METEORITE;
							pcInstance->GetState().sMeteoritePosition = sPosition;

							SendChat( pcInstance, CHATCOLOR_Global, FormatString( "BattleRoyale> Meteorite: X[%d] Z[%d]!", sPosition.iX >> 8, sPosition.iZ >> 8 ), true );
						}
					}
				}
			}

			//Meteorite Time?
			if ( pcInstance->GetState().iMeteoriteTime > 0 )
			{
				if ( pcInstance->GetState().iMeteoriteTime == pcInstance->GetState().iTimeUpdate )
				{
					//Fire
					Damage( pcInstance, true, pcInstance->GetState().sMeteoritePosition );
					pcInstance->GetState().iMeteoriteTime = 0;
				}
			}

			//Meteorite
			if ( ((pcInstance->GetBattleRoyale()->GetDuration() - pcInstance->GetState().iTimeUpdate) / 1000) <= 4 )
				Damage( pcInstance, true, pcInstance->GetState().sPosition, 40 );

			//Time to Drop?
			if ( (pcInstance->GetState().iTimeUpdate % MAX_BATTLEROYALE_DROPTIME) == 0 )
			{
				bool bCanDrop = true;
				if ( (pcInstance->GetBattleRoyale()->GetID() == BATTLEROYALE_PvPFast) || pcInstance->GetBattleRoyale()->IsTeamRoyal() )
					bCanDrop = false;

				if ( pcInstance->GetBattleRoyale()->GetID() == BATTLEROYALE_TeamRoyalOasis )
					bCanDrop = true;

				if ( bCanDrop )
				{
					//Drop Items
					DropItemOnSpawns( pcInstance );
				}
			}

			//Winner?
			if ( IsWinnerTime( pcInstance ) )
				EndBattle( pcInstance );
			else if ( pcInstance->GetState().iTimeUpdate == pcInstance->GetBattleRoyale()->GetDuration() )
				EndBattle( pcInstance, true );
		}
		else if ( pcInstance->GetState().eGameState == GAMESTATE_WaitingForPlayers )
		{
			pcInstance->GetState().iTimeUpdate += 1000;

			pcInstance->GetState().iPlayersCount = 0;
			for ( auto psPlayer : pcInstance->GetPlayers() )
			{
				if ( psPlayer->eState == PLAYERSTATE_Alive )
					pcInstance->GetState().iPlayersCount++;
			}

			//Start?
			if ( pcInstance->GetState().iTimeUpdate >= MAX_BATTLEROYALE_WAITTIME )
			{
				bool bCountdown = true;
				if ( (pcInstance->GetBattleRoyale()->GetID() == BATTLEROYALE_PvPFast) || pcInstance->GetBattleRoyale()->IsTeamRoyal() )
				{
					if ( pcInstance->GetBattleRoyale()->GetMapID() == MAPID_RoyalDesert )
						bCountdown = false;
				}

				if ( pcInstance->GetState().iTimeUpdate == MAX_BATTLEROYALE_WAITTIME )
				{
					//Teleport
					TeleportBeforeStart( pcInstance, bCountdown );

					pcInstance->GetState().bCountdown		= bCountdown;
					if ( bCountdown )
						pcInstance->GetState().iCountdownEnd	= MAX_BATTLEROYALE_WAITTIMEAFTER;
				}

				//Minimum Users required in arena?
				if ( pcInstance->GetState().iPlayersCount >= pcInstance->GetBattleRoyale()->GetRequiredUsers().iMin )
				{
					if ( pcInstance->GetState().iTimeUpdate == (bCountdown ? MAX_BATTLEROYALE_WAITTIMEAFTER : MAX_BATTLEROYALE_WAITTIME) )
					{
						//Start
						StartBattle( pcInstance );
					}
				}
				else
				{
					//End
					EndBattle( pcInstance, true );
				}
			}
		}
		else if ( pcInstance->GetState().eGameState == GAMESTATE_WaitingPvP )
		{
			pcInstance->GetState().iTimeUpdate += 1000;

			//Time to Drop?
			if ( (pcInstance->GetState().iTimeUpdate % MAX_BATTLEROYALE_DROPTIME) == 0 )
			{
				//Remove Items
				RemoveItems( pcInstance );

				//Drop Items
				DropItemOnSpawns( pcInstance );
			}

			//Start?
			if ( pcInstance->GetState().iTimeUpdate >= MAX_BATTLEROYALE_WAITPVP )
			{
				if ( pcInstance->GetState().iTimeUpdate == MAX_BATTLEROYALE_WAITPVP )
				{
					//Teleport
					TeleportBeforeStart( pcInstance, true );

					pcInstance->GetState().bCountdown		= true;
					pcInstance->GetState().iCountdownEnd	= MAX_BATTLEROYALE_WAITPVPAFTER;
				}

				if ( pcInstance->GetState().iTimeUpdate == MAX_BATTLEROYALE_WAITPVPAFTER )
				{
					//Start
					StartBattle( pcInstance );
				}
			}
		}
		else if ( pcInstance->GetState().eGameState == GAMESTATE_End )
			pcInstance->GetState().iTimeUpdate += 1000;


		//Process data to Users
		Process( pcInstance );
	}

	//Remove Ended Battles
	for ( auto it = vInstances.begin(); it != vInstances.end(); )
	{
		auto pcInstance = (*it);
		if ( (pcInstance->GetState().eGameState == GAMESTATE_End) && (pcInstance->GetState().iTimeUpdate == MAX_BATTLEROYALE_ENDBATTLE) )
		{
			ROOMINSTANCEHANDLER->Delete( pcInstance->GetRoom() );

			DELET( pcInstance );
			it = vInstances.erase( it );
		}
		else
			it++;
	}
}

void CBattleRoyaleHandler::UpdateEventTimer()
{
	static UINT uInstanceWheelID = 1;

	SYSTEMTIME * psServerTime = GetServerTime();

	//Check If some event need to start
	for ( auto pcBattleRoyale : vBattleRoyales )
	{
		//On time to start?
		if ( pcBattleRoyale->IsTime( psServerTime->wDayOfWeek, psServerTime->wHour, psServerTime->wMinute ) )
		{
			//Have an incoming event? then continue
			bool bHaveEvent = false;
			for ( auto pcIncoming : vInstances )
			{
				if ( pcIncoming->GetBattleRoyale() == pcBattleRoyale )
				{
					if ( pcIncoming->GetState().eGameState == GAMESTATE_WaitingForPlayers )
					{
						bHaveEvent = true;
						break;
					}
				}
			}
		}
		else
		{
			if ( pcBattleRoyale->IsTime( psServerTime->wDayOfWeek, psServerTime->wHour, psServerTime->wMinute, 5 ) && (pcBattleRoyale->GetAnnouncement() != ANNOUNCEMENTTYPE_5Minutes) )
			{
				//New Instance
				CBattleRoyaleInstance * pcInstance = new CBattleRoyaleInstance();
				pcInstance->SetID( SQLLastInstanceID() + 2 );
				pcInstance->SetDropID( USERSERVER->GetNewUserID() );
				pcInstance->SetBattleRoyale( pcBattleRoyale );
				pcInstance->SetRoom( ROOMINSTANCEHANDLER->Add( pcBattleRoyale->GetMapID(), FALSE ) );
				pcInstance->GetState().iBattleID = pcBattleRoyale->GetID();
				pcInstance->GetState().eGameState = GAMESTATE_WaitingForPlayers;
				pcInstance->GetState().iKillsNeeded = pcBattleRoyale->GetKillsNeeded();
				pcInstance->GetState().iPlayersCount = 0;
				pcInstance->GetState().iDuration = pcBattleRoyale->GetDuration();

				pcInstance->GetRoom()->Add( 1 )->Add( 1 );

				//Get Random Position
				Point3D sPosition = Point3D( 0, 0, 0 );
				if ( GetRandomCoordinate( pcInstance->GetBattleRoyale(), sPosition ) )
				{
					sPosition.iX <<= 8;
					sPosition.iZ <<= 8;

					pcInstance->GetState().sPosition = sPosition;

					if ( pcInstance->GetBattleRoyale()->GetMapID() == MAPID_RoyalDesert )
					{
						sPosition.iX = RandomI( -22138, -20540 ) << 8;
						sPosition.iZ = RandomI( 33050, 34650 ) << 8;

						pcInstance->GetState().sPosition = sPosition;
					}
				}

				vInstances.push_back( pcInstance );

				CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BattleRoyale> %s is open at %s!", pcBattleRoyale->GetName().c_str(), GetMapName( pcBattleRoyale->GetMapID() ) );

				pcBattleRoyale->SetAnnouncement( ANNOUNCEMENTTYPE_5Minutes );

				SQLNewBattle( pcInstance->GetBattleRoyale()->GetID(), pcInstance->GetID() );
			}
			else if ( pcBattleRoyale->IsTime( psServerTime->wDayOfWeek, psServerTime->wHour, psServerTime->wMinute, 10 ) && (pcBattleRoyale->GetAnnouncement() != ANNOUNCEMENTTYPE_10Minutes) )
			{
				CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BattleRoyale> %s at %s in 10 minutes!", pcBattleRoyale->GetName().c_str(), GetMapName( pcBattleRoyale->GetMapID() ) );
				pcBattleRoyale->SetAnnouncement( ANNOUNCEMENTTYPE_10Minutes );
			}
			else if ( pcBattleRoyale->IsTime( psServerTime->wDayOfWeek, psServerTime->wHour, psServerTime->wMinute, 15 ) && (pcBattleRoyale->GetAnnouncement() != ANNOUNCEMENTTYPE_15Minutes) )
			{
				CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BattleRoyale> %s at %s in 15 minutes!", pcBattleRoyale->GetName().c_str(), GetMapName( pcBattleRoyale->GetMapID() ) );
				pcBattleRoyale->SetAnnouncement( ANNOUNCEMENTTYPE_15Minutes );
			}
		}
	}
}

void CBattleRoyaleHandler::Damage( CBattleRoyaleInstance * pcInstance, bool bMeteorite, Point3D sPosition, int iDamage )
{
	//Compute Damage Packet
	PacketDamageQuick sPacketDamage;
	sPacketDamage.iLength			= sizeof( PacketDamageQuick );
	sPacketDamage.iHeader			= PKTHDR_DamageQuick;
	sPacketDamage.bBaseCurrentHP	= FALSE;
	sPacketDamage.bPercentile		= TRUE;

	//Meteorite time?
	if ( bMeteorite )
	{
		PacketFireMeteorite sPacket;
		sPacket.iLength				= sizeof( PacketFireMeteorite );
		sPacket.iHeader				= PKTHDR_FireMeteorite;
		sPacket.bSmallMeteor		= FALSE;
		sPacket.iDelay				= RandomI( 50, 251 );
		sPacket.sBegin				= sPosition;
		sPacket.sBegin.iY		   += 150000;
		sPacket.sEnd				= sPosition;
		sPacket.iCount				= RandomI( 1, 10 );
		SENDPACKETRANGE( &sPacket, &sPosition, (int)pow( RADIUS_BATTLEROYALE_METEORITE, 2 ) );

		//Only 10%
		sPacketDamage.iDamage		= iDamage;
		sPacketDamage.bDie			= (iDamage > 10) ? FALSE : TRUE;
		SENDPACKETRANGE( &sPacketDamage, &sPosition, (int)pow( RADIUS_BATTLEROYALE_METEORITE, 2 ) );
	}
	else
	{
		//Send damage to users out of the radius based on time
		sPacketDamage.bPercentile	= TRUE;
		sPacketDamage.iDamage		= (pcInstance->GetState().iTimeUpdate * 90) / pcInstance->GetBattleRoyale()->GetDuration();

		for ( auto psPlayer : pcInstance->GetPlayers() )
		{
			for ( UINT i = 0; i < USERSERVER->uUsersInGame; i++ )
			{
				User * pcUser = USERSERVER->pcaUserInGame[i];
				if ( pcUser->GetID() == psPlayer->iID )
				{
					if ( pcUser->pcUserData->sPosition.WithinPTDistance( &sPosition, pcInstance->GetState().iRadius + 150 ) == FALSE )
						SENDPACKET( pcUser, &sPacketDamage, TRUE );
				}
			}
		}
	}
}

void CBattleRoyaleHandler::DropItemOnSpawns( CBattleRoyaleInstance * pcInstance )
{
	for ( int i = 0; i < MAX_BATTLEROYALE_ITEMSPAWN; i++ )
	{
		Point3D sPosition;
		if ( GetRandomCoordinate( pcInstance->GetBattleRoyale(), sPosition ) )
			DropItem( pcInstance, Point3D( sPosition.iX << 8, 0, sPosition.iZ << 8 ), MAX_BATTLEROYALE_ITEMDROP );
	}
}

void CBattleRoyaleHandler::DropItem( CBattleRoyaleInstance * pcInstance, Point3D sPosition, int iItemCount )
{
	for ( int i = 0; i < iItemCount; i++ )
	{
		std::random_device rd;
		std::uniform_int_distribution<int> dist( 0, pcInstance->GetBattleRoyale()->GetTotalDropChance() );
		int iRandom = dist( rd );

		int iRandomCount = 0;
		for( auto & vDropData : pcInstance->GetBattleRoyale()->GetDrops() )
		{
			bool bDrop = false;

			for ( auto psDrop : vDropData )
			{
				if ( (iRandom >= iRandomCount) && (iRandom < (iRandomCount + psDrop->iPercent)) )
				{
					Map::Loot::LootItem sLootItem;

					//Get Item Data
					DefinitionItem * psDefinitionItem = ITEMSERVER->FindItemDefByCode( psDrop->iItemID );
					if ( psDefinitionItem )
					{
						//Create Item with non-spec
						ITEMSERVER->CreateItem( &sLootItem.sItem, psDefinitionItem, 0 );

						//Get Position on Map
						int iX = (sPosition.iX + (rand() % 8192)) >> 8;
						int iZ = (sPosition.iZ + (rand() % 8192)) >> 8;

						//Age
						if ( AGEHANDLER->CanAge( sLootItem.sItem.sItemID ) )
						{
							AGEHANDLER->OnUpAgeHandler( &sLootItem.sItem, RandomI( 16, 22 ) );

							sLootItem.sItem.eCraftType = ITEMCRAFTTYPE_Aging;
						}
						else
						{
							if ( (sLootItem.sItem.sItemID.ToItemType() == ITEMTYPE_Bracelets) || (sLootItem.sItem.sItemID.ToItemType() == ITEMTYPE_Gauntlets) ||
								(sLootItem.sItem.sItemID.ToItemType() == ITEMTYPE_Boots) )
							{
								int iRandNumber = RandomI( 1, 3 );

								EMixTypeName eMixName = iRandNumber == 1 ? MIXTYPENAME_OredoVitality : MIXTYPENAME_Bellum;

								switch ( sLootItem.sItem.sItemID.ToItemType() )
								{
									case ITEMTYPE_Bracelets:
										if ( iRandNumber == 3 )
											eMixName = MIXTYPENAME_BellumRating;
										break;
									case ITEMTYPE_Gauntlets:
										if ( iRandNumber == 3 )
											eMixName = MIXTYPENAME_BellumDefense;
										break;
									case ITEMTYPE_Boots:
										if ( iRandNumber == 3 )
											eMixName = MIXTYPENAME_BellumVelocity;
										break;


									default:
										break;
								}

								sLootItem.sItem.eCraftType = ITEMCRAFTTYPE_Mixing;
								MIXHANDLER->SetItemMixByMixName( &sLootItem.sItem, eMixName );
								sLootItem.sItem.eMixTypeName = eMixName;
								sLootItem.sItem.sMixID = 14;
							}
						}

						//Remove Requirements
						sLootItem.sItem.iLevel = 0;
						sLootItem.sItem.iStrength = 0;
						sLootItem.sItem.iSpirit = 0;
						sLootItem.sItem.iTalent = 0;
						sLootItem.sItem.iAgility = 0;
						sLootItem.sItem.iHealth = 0;
						if ( sLootItem.sItem.sIntegrity.sMax > 0 )
							sLootItem.sItem.sIntegrity.sMax = sLootItem.sItem.sIntegrity.sCur = 300;

						ITEMSERVER->ValidateItem( &sLootItem.sItem );

						//Get Map Data
						Map * pcMap = MAPSDATA + pcInstance->GetBattleRoyale()->GetMapID();
						if ( pcMap )
						{
							int iY = pcMap->pcStage->GetHighestPoint( iX << 8, iZ << 8 ) >> 8;

							//Add Item to Map
							Map::Loot * psItemD = pcMap->AddItem( &sLootItem, iX, iY, iZ, TRUE );

							//Added? Send It to near users...
							if ( psItemD )
							{
								//Exclusive ID
								psItemD->bExclusive			= TRUE;
								psItemD->lExclusiveOwnerID	= pcInstance->GetDropID();

								//Elapse Drop
								psItemD->dwEndTime			= MAX_BATTLEROYALE_DROPTIME + TICKCOUNT;

								//Send
								MAPSERVER->SendItemStageNearUsers( pcMap, psItemD );

								bDrop = true;
								break;
							}
						}
					}
				}

				iRandomCount += psDrop->iPercent;
			}

			if ( bDrop )
				break;
		}
	}
}

void CBattleRoyaleHandler::StartBattle( CBattleRoyaleInstance * pcInstance )
{
	bool bStart = true;

	if ( pcInstance->GetState().eGameState == GAMESTATE_WaitingForPlayers )
	{
		//Create Teams
		CreateTeams( pcInstance );
	}

	bool bRoyalDesert = false;

	if ( (pcInstance->GetBattleRoyale()->GetID() == BATTLEROYALE_PvPFast) || pcInstance->GetBattleRoyale()->IsTeamRoyal() )
	{
		if ( pcInstance->GetBattleRoyale()->GetMapID() == MAPID_RoyalDesert )
			bRoyalDesert = true;
	}

	if ( bRoyalDesert )
	{
		if ( pcInstance->GetState().eGameState == GAMESTATE_WaitingForPlayers )
		{
			//Can't start
			bStart = false;

			//Announcement
			SendChat( pcInstance, CHATCOLOR_Global, FormatString( "BattleRoyale> %s drops started!", pcInstance->GetBattleRoyale()->GetName().c_str() ) );

			//Drop Items
			DropItemOnSpawns( pcInstance );

			//Set Battle
			pcInstance->GetState().eGameState = GAMESTATE_WaitingPvP;

			//Reset Timer
			pcInstance->GetState().iTimeUpdate = 0;
		}
		else
			RemoveItems( pcInstance );
	}

	if ( bStart )
	{
		//Set Battle
		pcInstance->GetState().eGameState = GAMESTATE_InBattle;

		//Reset Timer
		pcInstance->GetState().iTimeUpdate = 0;

		//Announcement
		SendChat( pcInstance, CHATCOLOR_Global, FormatString( "BattleRoyale> %s started!", pcInstance->GetBattleRoyale()->GetName().c_str() ) );
		SendChat( pcInstance, CHATCOLOR_Global, FormatString( "BattleRoyale> Gas: X[%d] Z[%d]!", pcInstance->GetState().sPosition.iX >> 8, pcInstance->GetState().sPosition.iZ >> 8 ), true );

		//Drop Items
		DropItemOnSpawns( pcInstance );

		//Update Instance
		SQLStartBattle( pcInstance->GetID(), pcInstance->GetPlayers().size() );

		ProcessStart( pcInstance, TRUE );
	}
}

void CBattleRoyaleHandler::EndBattle( CBattleRoyaleInstance * pcInstance, bool bTimeOut )
{
	//Process Data to User Update
	Process( pcInstance );

	auto pcBattleRoyale = pcInstance->GetBattleRoyale();

	CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BattleRoyale> %s ended!", pcBattleRoyale->GetName().c_str() );
	
	int iCharacterWinnerID = -1;

	EPlayerTeam eTeamID = PLAYERTEAM_None;

	auto UpdateRewards = [this, pcInstance, pcBattleRoyale]( EPlayerTeam eTeamWinnerID, int iChWinnerID ) -> void
	{
		//Mode Team?
		if ( pcBattleRoyale->IsTeamRoyal() )
		{
			for ( int i = 0; i < pcInstance->GetState().iPlayerTeamCount; i++ )
			{
				auto psPlayer = pcInstance->GetState().saPlayerTeams + i;

				if ( (psPlayer->iID != 0) && (psPlayer->iID != -1) )
				{
					SQLAddUserReward( pcBattleRoyale->GetID(), pcInstance->GetID(), psPlayer->iEncryptedCharacterID ^ ENC_BATTLEROYALE_CHARACTERID,
									  psPlayer->iKills, psPlayer->eTeamID == eTeamWinnerID ? TRUE : FALSE, psPlayer->eTeamID );
				}
			}
		}
		else
		{
			for ( int i = 0; i < pcInstance->GetState().iPlayerTeamCount; i++ )
			{
				auto psPlayer = pcInstance->GetState().saPlayerTeams + i;

				if ( (psPlayer->iID != 0) && (psPlayer->iID != -1) )
				{
					SQLAddUserReward( pcBattleRoyale->GetID(), pcInstance->GetID(), psPlayer->iEncryptedCharacterID ^ ENC_BATTLEROYALE_CHARACTERID,
									  psPlayer->iKills, (psPlayer->iEncryptedCharacterID ^ ENC_BATTLEROYALE_CHARACTERID) == iChWinnerID ? TRUE : FALSE, psPlayer->eTeamID );
				}
			}
		}
	};

	if ( !bTimeOut )
	{
		if ( pcBattleRoyale->IsTeamRoyal() )
		{
			if ( pcInstance->GetPlayers().size() > 0 )
			{
				if ( pcBattleRoyale->GetID() == BATTLEROYALE_TeamRoyal )
				{
					User * pcWinner = USERDATATOUSER( USERDATABYID( pcInstance->GetPlayers()[0]->iID ) );
					if ( pcWinner )
					{
						eTeamID = pcInstance->GetPlayers()[0]->eTeamID;
						iCharacterWinnerID = pcWinner->iCharacterID;

						CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BattleRoyale> Winner: %s team!", BroyaleTeamToString( eTeamID ).c_str() );

						SQLWinBattle( pcInstance->GetID(), iCharacterWinnerID );

						SQLUpdateCharacterRanking( pcWinner, 0, FALSE, TRUE, RandomI( 130, 145 ) );

						ACHIEVEMENTHANDLER->OnTask( pcWinner, ACHIEVEMENTTASK_BattleRoyaleWinner, ValueTaskAchievement( 0 ), ValueTaskAchievement( 0 ), ValueTaskAchievement( 0 ) );

						UpdateRewards( eTeamID, iCharacterWinnerID );
					}
				}
				else if ( pcInstance->GetBattleRoyale()->IsTeamRoyalTickets() )
				{
					std::vector<TeamsAliveData> vTeams;
					if ( GetTeamsAlive( pcInstance, vTeams ) )
					{
						eTeamID = vTeams[0].eTeamID;

						iCharacterWinnerID = -1;

						CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BattleRoyale> Winner: %s team!", BroyaleTeamToString( eTeamID ).c_str() );

						UpdateRewards( eTeamID, iCharacterWinnerID );
					}

					vTeams.clear();
				}
			}
		}
		else if ( pcInstance->GetPlayers().size() == 1 )
		{
			User * pcWinner = USERDATATOUSER( USERDATABYID( pcInstance->GetPlayers()[0]->iID ) );
			if ( pcWinner )
			{
				CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BattleRoyale> Winner: %s!", CHARACTERSERVER->GetCharacterName( pcWinner->pcUserData ) );

				eTeamID = pcInstance->GetPlayers()[0]->eTeamID;
				iCharacterWinnerID = pcWinner->iCharacterID;

				SQLWinBattle( pcInstance->GetID(), iCharacterWinnerID );

				SQLUpdateCharacterRanking( pcWinner, 0, FALSE, TRUE, RandomI( 130, 145 ) );

				ACHIEVEMENTHANDLER->OnTask( pcWinner, ACHIEVEMENTTASK_BattleRoyaleWinner, ValueTaskAchievement( 0 ), ValueTaskAchievement( 0 ), ValueTaskAchievement( 0 ) );

				UpdateRewards( eTeamID, iCharacterWinnerID );
			}
		}
	}
	else
	{
		if ( pcBattleRoyale->IsTeamRoyal() )
		{
			if ( pcInstance->GetPlayers().size() > 0 )
			{
				std::vector<TeamsAliveData> vTeams;
				if ( GetTeamsAlive( pcInstance, vTeams ) )
				{
					TeamsAliveData sTeamWinner = {PLAYERTEAM_None, 0,0};
					if ( pcBattleRoyale->GetID() == BATTLEROYALE_TeamRoyal )
					{
						for ( auto & sTeam : vTeams )
						{
							if ( sTeam.iCount > sTeamWinner.iCount )
							{
								sTeamWinner.eTeamID = sTeam.eTeamID;
								sTeamWinner.iCount = sTeam.iCount;
							}
						}
					}
					else if ( pcInstance->GetBattleRoyale()->IsTeamRoyalTickets() )
					{
						sTeamWinner.eTeamID = vTeams[0].eTeamID;
						sTeamWinner.iKills = vTeams[0].iKills;
						sTeamWinner.iCount = vTeams[0].iCount;
					}

					if ( (sTeamWinner.eTeamID != PLAYERTEAM_None) && (sTeamWinner.iCount > 0) )
					{
						CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BattleRoyale> Winner: %s team!", BroyaleTeamToString( sTeamWinner.eTeamID ).c_str() );
						UpdateRewards( sTeamWinner.eTeamID, iCharacterWinnerID );

						iCharacterWinnerID = 0;
					}
				}

				vTeams.clear();
			}
		}
	}
	
	if ( (pcBattleRoyale->IsTeamRoyal() == false) )
 		if ( iCharacterWinnerID == -1 )
			CHATSERVER->SendChatAll( CHATCOLOR_Global, "BattleRoyale> No winner at this time!" );

	pcInstance->GetState().eGameState = GAMESTATE_End;

	pcInstance->GetState().iTimeUpdate = 0;

	//Update Instance
	SQLEndBattle( pcInstance->GetID(), iCharacterWinnerID );

	pcInstance->GetBattleRoyale()->SetAnnouncement( ANNOUNCEMENTTYPE_None );

	RemoveItems( pcInstance );
}

void CBattleRoyaleHandler::Process( CBattleRoyaleInstance * pcInstance )
{
	PacketBattleRoyaleUpdate sPacket;
	sPacket.iLength = sizeof( PacketBattleRoyaleUpdate );
	sPacket.iHeader = PKTHDR_BattleRoyaleUpdate;
	CopyMemory( &sPacket.sState, &pcInstance->GetState(), sizeof( State ) );

	//Waiting for Players?
	if ( pcInstance->GetState().eGameState == GAMESTATE_WaitingForPlayers )
	{
		//Clear Position
		sPacket.sState.sPosition = Point3D( 0, 0, 0 );
	}

	//Send Packet to Users
	for ( auto psPlayer : pcInstance->GetPlayers() )
	{
		for ( UINT i = 0; i < USERSERVER->uUsersInGame; i++ )
		{
			User * pcUser = USERSERVER->pcaUserInGame[i];
			if ( (pcUser->GetID() == psPlayer->iID) && (psPlayer->iTimeDeath == 0) )
				SENDPACKET( pcUser, &sPacket, TRUE );
		}
	}
}

void CBattleRoyaleHandler::RemoveItems( CBattleRoyaleInstance * pcInstance )
{
	Map * pcMap = MAPSDATA + pcInstance->GetBattleRoyale()->GetMapID();
	if ( pcMap )
	{
		//Remove Loots
		for ( int i = 0; i < MAX_LOOT; i++ )
		{
			Map::Loot * psLoot = pcMap->saLoot + i;
			if ( psLoot->bInUse )
				MAPSERVER->DelItem( pcMap, psLoot );
		}
	}
}

void CBattleRoyaleHandler::RequestBattleList( User * pcUser )
{
    if ( SERVER_CODE > 1 )
    {
        CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "BattleRoyale> Battle Royale is closed on this server!" );
        return;
    }

	std::vector<BattleRoyaleBattleServer *> vBattles;
	if ( GetBattleList( vBattles ) )
	{
		PacketBattleRoyaleList sPacket;
		sPacket.iCount = 0;

		for ( auto psBattle : vBattles )
		{
			sPacket.saList[sPacket.iCount].iID = psBattle->iID;
			sPacket.saList[sPacket.iCount].bOpen = psBattle->bOpen;
			sPacket.saList[sPacket.iCount].iMapID = (EMapID)psBattle->iMapID;
			sPacket.saList[sPacket.iCount].sRequiredLevel = psBattle->sRequiredLevel;
			sPacket.saList[sPacket.iCount].iTimeLeft = psBattle->iTimeLeft;
			STRINGCOPY( sPacket.saList[sPacket.iCount].szName, psBattle->szName );
			sPacket.iCount++;
		}

		ProcessPacket( pcUser, &sPacket );
	}

	for ( auto psBattle : vBattles )
		DELET( psBattle );

	vBattles.clear();
}

bool CBattleRoyaleHandler::GetRandomCoordinate( CBattleRoyale * pcBattleRoyale, Point3D & sPosition )
{
	if ( pcBattleRoyale->GetCoordinates().size() > 0 )
	{
		auto psCoordinate = pcBattleRoyale->GetCoordinates()[rand() % pcBattleRoyale->GetCoordinates().size()];

		std::random_device rd;
		std::uniform_int_distribution<int> cGenX( psCoordinate->iWest, psCoordinate->iEast );
		std::uniform_int_distribution<int> cGenZ( psCoordinate->iSouth, psCoordinate->iNorth );

		sPosition.iX = cGenX( rd );
		sPosition.iZ = cGenZ( rd );
		sPosition.iY = 0;

		return true;
	}

	return false;
}

bool CBattleRoyaleHandler::SQLIsAccountBanned( int iAccountID, SYSTEMTIME & sUnbanDate, char szReason[128] )
{
	bool bRet = false;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT TOP 1 Reason, UnbanDate FROM UserBattleRoyaleBanned WHERE (AccountID=?) ORDER BY ID DESC" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iAccountID );
			if ( pcDB->Execute() )
			{
				if ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_String, szReason, 128 );
					pcDB->GetData( 2, PARAMTYPE_Time, &sUnbanDate, sizeof( SYSTEMTIME ) );

					bRet = true;
				}
			}
		}

		pcDB->Close();
	}

	if ( bRet )
	{
		SYSTEMTIME sCurrentTime;
		CopyMemory( &sCurrentTime, GetServerTime(), sizeof( SYSTEMTIME ) );

		if ( sCurrentTime.wYear > sUnbanDate.wYear )
			bRet = false;
		else if ( sCurrentTime.wYear == sUnbanDate.wYear )
		{
			if ( sCurrentTime.wMonth > sUnbanDate.wMonth )
				bRet = false;
			else if ( sCurrentTime.wMonth == sUnbanDate.wMonth )
			{
				if ( sCurrentTime.wDay > sUnbanDate.wDay )
					bRet = false;
				else if ( sCurrentTime.wDay == sUnbanDate.wDay )
				{
					if ( sCurrentTime.wHour > sUnbanDate.wHour )
						bRet = false;
					else if ( sCurrentTime.wHour == sUnbanDate.wHour )
					{
						if ( sCurrentTime.wMinute >= sUnbanDate.wMinute )
							bRet = false;
					}
				}
			}
		}

		if ( bRet == false )
		{
			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "DELETE FROM UserBattleRoyaleBanned WHERE (AccountID=?)" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iAccountID );
					pcDB->Execute();
				}

				pcDB->Close();
			}
		}
	}

	return bRet;
}

void CBattleRoyaleHandler::CreateTeams( CBattleRoyaleInstance * pcInstance )
{
	pcInstance->GetState().iPlayerTeamCount = 0;

	EPlayerTeam eTeamID = PLAYERTEAM_Green;
	if ( (pcInstance->GetBattleRoyale()->IsTeamRoyal() == false) )
		eTeamID = PLAYERTEAM_Solo;

	pcInstance->GetState().bTeam = pcInstance->GetBattleRoyale()->IsTeamRoyal() ? true : false;

	//Set Team to Players

	for ( auto psPlayer : pcInstance->GetPlayers() )
	{
		psPlayer->eTeamID = eTeamID;
		pcInstance->GetState().saPlayerTeams[pcInstance->GetState().iPlayerTeamCount].iID = psPlayer->iID;
		pcInstance->GetState().saPlayerTeams[pcInstance->GetState().iPlayerTeamCount].iEncryptedCharacterID = psPlayer->iCharacterID ^ ENC_BATTLEROYALE_CHARACTERID;
		pcInstance->GetState().saPlayerTeams[pcInstance->GetState().iPlayerTeamCount].eTeamID = eTeamID;
		pcInstance->GetState().saPlayerTeams[pcInstance->GetState().iPlayerTeamCount].iKills = 0;
		pcInstance->GetState().iPlayerTeamCount++;

		if ( pcInstance->GetBattleRoyale()->IsTeamRoyal() )
		{
			if ( eTeamID == PLAYERTEAM_White )
				eTeamID = PLAYERTEAM_Green;
			else
				eTeamID = PLAYERTEAM_White;
		}
		else
			eTeamID = (EPlayerTeam)(PLAYERTEAM_Solo + pcInstance->GetState().iPlayerTeamCount);
	}
}

bool CBattleRoyaleHandler::GetTeamsAlive( CBattleRoyaleInstance * pcInstance, std::vector<TeamsAliveData> & vTeams )
{
	bool bRet = false;

	vTeams.clear();
	for ( auto psPlayer : pcInstance->GetPlayers() )
	{
		bool bFound = false;
		for ( auto & sTeam : vTeams )
		{
			if ( psPlayer->eTeamID == sTeam.eTeamID )
			{
				sTeam.iCount++;

				bFound = true;
				break;
			}
		}

		if ( bFound == false )
		{
			TeamsAliveData sTeam;
			sTeam.eTeamID = psPlayer->eTeamID;
			sTeam.iCount = 1;
			sTeam.iKills = 0;
			vTeams.push_back( sTeam );
		}
	}

	for ( int i = 0; i < pcInstance->GetState().iPlayerTeamCount; i++ )
	{
		for ( auto & sTeam : vTeams )
		{
			if ( pcInstance->GetState().saPlayerTeams[i].eTeamID == sTeam.eTeamID )
			{
				sTeam.iKills += pcInstance->GetState().saPlayerTeams[i].iKills;
				break;
			}
		}
	}

	std::sort( vTeams.begin(), vTeams.end(),
			   []( TeamsAliveData & sTeamOne, TeamsAliveData & sTeamTwo )
	{
		return sTeamOne.iKills > sTeamTwo.iKills;
	} );

	return vTeams.size() > 0;
}

bool CBattleRoyaleHandler::IsWinnerTime( CBattleRoyaleInstance * pcInstance )
{
	bool bRet = false;

	if ( pcInstance->GetBattleRoyale()->IsTeamRoyal() && (pcInstance->GetBattleRoyale()->IsTeamRoyalTickets() == false) )
	{
		std::vector<TeamsAliveData> vTeams;
		if ( GetTeamsAlive( pcInstance, vTeams ) )
		{
			if ( vTeams.size() == 1 )
				bRet = true;
		}

		vTeams.clear();
	}

	if ( pcInstance->GetBattleRoyale()->IsTeamRoyalTickets() )
	{
		std::vector<TeamsAliveData> vTeams;
		if ( GetTeamsAlive( pcInstance, vTeams ) )
		{
			if ( vTeams.size() > 0 )
			{
				if ( vTeams.size() == 1 )
					bRet = true;

				if ( vTeams[0].iKills >= pcInstance->GetBattleRoyale()->GetKillsNeeded() )
					bRet = true;

				if ( vTeams.size() >= _countof( pcInstance->GetState().eaTeamID ) )
				{
					for ( int i = 0; i < _countof( pcInstance->GetState().eaTeamID ); i++ )
					{
						pcInstance->GetState().eaTeamID[i] = vTeams[i].eTeamID;
						pcInstance->GetState().iaTeamKills[i] = vTeams[i].iKills;
					}
				}
			}
		}

		vTeams.clear();
	}

	if ( bRet == false )
	{
		if ( pcInstance->GetPlayers().size() <= 1 )
			bRet = true;
	}

	return bRet;
}

void CBattleRoyaleHandler::TeleportBeforeStart( CBattleRoyaleInstance * pcInstance, bool bCountdown )
{
	//Teleport Users to randomly positions
	for ( auto psPlayer : pcInstance->GetPlayers() )
	{
		for ( UINT i = 0; i < USERSERVER->uUsersInGame; i++ )
		{
			//User in Battle?
			User * pcUser = USERSERVER->pcaUserInGame[i];
			if ( pcUser->GetID() == psPlayer->iID )
			{
				//Teleport
				TeleportUser( pcInstance, pcUser, true, true, bCountdown );
			}
		}
	}
}

void CBattleRoyaleHandler::SendEXPPrizeGold( User * pcUser, EBattleRoyaleID iBattleID, int iInstanceID, int iKills, bool bWinner, int iMinutes, SYSTEMTIME sDate )
{
	if ( pcUser->pcUserData->sCharacterData.iLevel < SERVER_LEVEL_MAX )
	{
		//EXP
		INT64 iEXP = 40000000000;

		if ( iBattleID == BATTLEROYALE_Normal )
		{
			iEXP = 20000000000;

			if ( iMinutes > 5 )
				iEXP = 30000000000;
			if ( iMinutes > 10 )
				iEXP = 40000000000;
		}

		if ( bWinner == false )
			iEXP /= 2;

		//EXP per Mode
		INT64 iEXPGainMode = 2000000000;
		if ( CBattleRoyale::IsTeamRoyalTickets( iBattleID ) )
			iEXPGainMode = 1000000000;

		iEXP += iEXPGainMode * (INT64)iKills;

		if ( (*(int *)0x0084601C) > 0 )
			iEXP += (iEXP * (*(int *)0x0084601C)) / 100;

		CHARACTERSERVER->GiveEXP( pcUser, iEXP );

		//Set Done
		SQLUpdateUserReceivedReward( iBattleID, iInstanceID, pcUser->iCharacterID, sDate );

		//Log
		LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_BattleRoyaleReceivedItem, "Character[%s] Received BattleRoyale Reward EXP[%s]",
								CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), FormatNumber( iEXP ) );

		//Notify
		CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Global, "BattleRoyale> You received %s EXP!", FormatNumber( iEXP ) );
		CHATSERVER->SendChatMessageBox( pcUser, "You received %s EXP!", FormatNumber( iEXP ) );
	}
	else
	{
		//Gold
		int iGold = 20000000;
		if ( bWinner == false )
			iGold = 10000000;

		if ( LOGIN_SERVER )
			CHARACTERSERVER->GiveGOLD( pcUser, iGold );
		else
			NETSERVER->SendBellatraGiveGold( pcUser, iGold );

		//Set Done
		SQLUpdateUserReceivedReward( iBattleID, iInstanceID, pcUser->iCharacterID, sDate );

		//Log
		LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_BattleRoyaleReceivedItem, "Character[%s] Received BattleRoyale Reward Gold[%s]",
								CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), FormatNumber( iGold ) );

		//Notify
		CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Global, "BattleRoyale> You received %s gp!", FormatNumber( iGold ) );
		CHATSERVER->SendChatMessageBox( pcUser, "You received %s gp!", FormatNumber( iGold ) );
	}
}

void CBattleRoyaleHandler::SendUserCoordinateList( User * pcUser, CBattleRoyaleInstance * pcInstance )
{
	PacketBattleRoyaleTeleports sPacket;
	sPacket.iCount = 0;

	if ( pcInstance->GetBattleRoyale()->GetMapID() == MAPID_RoyalDesert )
	{
		sPacket.saCoordinates[sPacket.iCount].iWest = -21990;
		sPacket.saCoordinates[sPacket.iCount].iEast = -20790;
		sPacket.saCoordinates[sPacket.iCount].iNorth = 34350;
		sPacket.saCoordinates[sPacket.iCount].iSouth = 33145;

		sPacket.iCount++;
	}
	else if ( pcInstance->GetBattleRoyale()->GetMapID() == MAPID_OasisRoyale )
	{
		//Get Map Data
		Map * pcMap = MAPSDATA + MAPID_OasisRoyale;
		if ( MAPSERVER->IsValidMap( pcMap ) )
		{
			for ( UINT i = 0; i < _countof( pcMap->saSpawnFlag ); i++ )
			{
				if ( pcMap->saSpawnFlag[i].bActive )
				{
					SpawnCoordinate sSpawn;
					sSpawn.iNorth = pcMap->saSpawnFlag[i].iZ + 8;
					sSpawn.iSouth = pcMap->saSpawnFlag[i].iZ - 8;
					sSpawn.iEast  = pcMap->saSpawnFlag[i].iX + 8;
					sSpawn.iWest  = pcMap->saSpawnFlag[i].iX - 8;

					CopyMemory( sPacket.saCoordinates + sPacket.iCount, &sSpawn, sizeof( SpawnCoordinate ) );

					sPacket.iCount++;

					if ( sPacket.iCount == MAX_BATTLEROYALE_SPAWNCOORDS )
						break;
				}
			}
		}
	}
	else
	{
		for ( UINT i = 0; i < pcInstance->GetBattleRoyale()->GetCoordinates().size(); i++ )
		{
			CopyMemory( sPacket.saCoordinates + sPacket.iCount, pcInstance->GetBattleRoyale()->GetCoordinates()[sPacket.iCount], sizeof( SpawnCoordinate ) );

			sPacket.iCount++;

			if ( sPacket.iCount == MAX_BATTLEROYALE_SPAWNCOORDS )
				break;
		}
	}

	ProcessPacket( pcUser, &sPacket );
}
