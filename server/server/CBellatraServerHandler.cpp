#include "stdafx.h"
#include "CBellatraServerHandler.h"

const int BELLATRA_ROOM_MONSTERID	= 500;
const int BELLATRA_ROOM_USERID		= 501;


using namespace SOD;

CBellatraInstance::CBellatraInstance()
{
}

CBellatraInstance::~CBellatraInstance()
{
	for ( auto psTeam : vTeams )
		DELET( psTeam );

	vTeams.clear();
}

void CBellatraInstance::Start()
{
	bStart			= TRUE;

	auto pcRound	= GetBellatra()->GetRounds()[GetRoundNumber() - 1];
	iDelayTime		= TICKCOUNT + (pcRound->GetDelay() * 1000);
}

CBellatraRound * CBellatraInstance::GetActiveRound()
{
	if ( (GetRoundNumber() == 0) || (GetRoundNumber() > GetBellatra()->GetRounds().size()) )
		return nullptr;

	return GetBellatra()->GetRounds()[GetRoundNumber() - 1];
}

CBellatraRound * CBellatraInstance::GetNextRound()
{
	if ( (GetRoundNumber() == 0) || (GetRoundNumber() >= GetBellatra()->GetRounds().size()) )
		return nullptr;

	return GetBellatra()->GetRounds()[GetRoundNumber()];
}

CBellatraRound * CBellatraInstance::GetFirstRound()
{
	if ( GetBellatra()->GetRounds().size() == 0 )
		return nullptr;

	return GetBellatra()->GetRounds()[0];
}

BellatraTeam * CBellatraInstance::GetTeam( int iID )
{
	BellatraTeam * psBellatraTeam = nullptr;
	for ( auto psTeam : vTeams )
	{
		if ( psTeam->bActive )
		{
			if ( psTeam->iID == iID )
			{
				psBellatraTeam = psTeam;
				break;
			}
		}
	}

	return psBellatraTeam;
}

BellatraTeam * CBellatraInstance::GetTeamByPlayerID( int iID )
{
	BellatraTeam * psBellatraTeam = nullptr;
	for ( auto psTeam : vTeams )
	{
		if ( psTeam->bActive )
		{
			for ( int i = 0; i < MAX_BELLATRA_PLAYERS; i++ )
			{
				auto & sPlayer = psTeam->saPlayers[i];
				if ( (sPlayer.bActive) && (sPlayer.iID == iID) )
				{
					psBellatraTeam = psTeam;
					break;
				}
			}

			if ( psBellatraTeam )
				break;
		}
	}

	return psBellatraTeam;
}

BellatraTeam * CBellatraInstance::GetTeamByUnitID( int iID )
{
	BellatraTeam * psBellatraTeam = nullptr;
	for ( auto psTeam : vTeams )
	{
		if ( psTeam->bActive )
		{
			if ( auto psUnit = GetRoom()->Get( BELLATRA_ROOM_MONSTERID )->Get( psTeam->iRoomID )->GetUnit( iID ) )
			{
				if ( psUnit->bInvalid == false )
					psBellatraTeam = psTeam;

				break;
			}
		}
	}

	return psBellatraTeam;
}

SOD::BellatraTeam * CBellatraInstance::GetTeamByRoomID( int iRoomID )
{
	BellatraTeam * psBellatraTeam = nullptr;
	for ( auto psTeam : vTeams )
	{
		if ( psTeam->bActive )
		{
			if ( psTeam->iRoomID == iRoomID )
			{
				psBellatraTeam = psTeam;
				break;
			}
		}
	}

	return psBellatraTeam;
}

BellatraPlayer * CBellatraInstance::GetPlayer( int iID )
{
	BellatraPlayer * psBellatraPlayer = nullptr;
	for ( auto psTeam : vTeams )
	{
		if ( psTeam->bActive )
		{
			for ( int i = 0; i < MAX_BELLATRA_PLAYERS; i++ )
			{
				auto psPlayer = psTeam->saPlayers + i;
				if ( (psPlayer->bActive) && (psPlayer->iID == iID) )
				{
					psBellatraPlayer = psPlayer;
					break;
				}
			}

			if ( psBellatraPlayer )
				break;
		}
	}

	return psBellatraPlayer;
}

bool CBellatraInstance::IsSameRoom( int iUserID, int iUnitID )
{
	if ( auto psTeam = GetTeamByPlayerID( iUserID ) )
	{
		if ( auto psUnit = GetRoom()->Get( BELLATRA_ROOM_MONSTERID )->Get( psTeam->iRoomID )->GetUnit( iUnitID ) )
			return true;
	}

	return false;
}

bool CBellatraInstance::GetTeamUsers( BellatraTeam * psTeam, std::vector<User *> & vUsers, bool bAddInvalid )
{
	if ( psTeam )
	{
		if ( psTeam->bActive )
		{
			for ( int i = 0; i < MAX_BELLATRA_PLAYERS; i++ )
			{
				auto psPlayer = psTeam->saPlayers + i;
				if ( psPlayer->bActive && ((psPlayer->bInvalid == false) || bAddInvalid) )
				{
					for ( UINT i = 0; i < USERSERVER->uUsersInGame; i++ )
					{
						User * pcUserSod = USERSERVER->pcaUserInGame[i];
						if ( pcUserSod->GetID() == psPlayer->iID )
						{
							vUsers.push_back( pcUserSod );
							break;
						}
					}
				}
			}
		}
	}

	return (vUsers.size() > 0);
}

bool CBellatraInstance::GetTeamUnits( BellatraTeam * psTeam, std::vector<Unit *> & vUnits )
{
	auto vUnitsRoom = GetRoom()->Get( BELLATRA_ROOM_MONSTERID )->Get( psTeam->iRoomID )->GetUnits();
	for ( auto psUnit : vUnitsRoom )
	{
		if ( Unit * pcUnit = UNITDATATOUNIT( UNITDATABYIDMAP( psUnit->iID, GetActiveRound()->GetMapID() ) ) )
			vUnits.push_back( pcUnit );
	}

	return (vUnits.size() > 0);
}

void CBellatraInstance::RemoveUnit( int iID )
{
	for ( auto psTeam : vTeams )
	{
		if ( psTeam->bActive )
			GetRoom()->Get( BELLATRA_ROOM_MONSTERID )->Get( psTeam->iRoomID )->Remove( iID );
	}
}

void CBellatraInstance::InvalidateUnit( int iID )
{
	for ( auto psTeam : vTeams )
	{
		if ( psTeam->bActive )
			GetRoom()->Get( BELLATRA_ROOM_MONSTERID )->Get( psTeam->iRoomID )->Invalidate( iID );
	}
}

bool CBellatraInstance::AddUnit( int iRoomID, int iID )
{
	return GetRoom()->Get( BELLATRA_ROOM_MONSTERID )->Get( iRoomID )->Add( iID ) != nullptr;
}

bool CBellatraInstance::AddUser( int iRoomID, int iID, int iCharacterID )
{
	bool bRet = false;

	bool bFoundTeam = false;

	for ( auto psTeam : vTeams )
	{
		if ( psTeam->bActive && (psTeam->iRoomID == iRoomID) )
		{
			GetRoom()->Get( BELLATRA_ROOM_USERID )->Get( psTeam->iRoomID )->Add( iID );

			for ( auto & sPlayer : psTeam->saPlayers )
			{
				if ( sPlayer.bActive == false )
				{
					sPlayer.iID			= iID;
					sPlayer.bInvalid	= false;
					sPlayer.iKills		= 0;
					sPlayer.iScore		= 0;
					ZeroMemory( sPlayer.iaBellatraItemCount, sizeof( int ) * _countof( sPlayer.iaBellatraItemCount ) );
					sPlayer.iCharacterID = iCharacterID;
					sPlayer.bActive		= true;

					bRet = true;
					break;
				}
			}

			bFoundTeam = true;
			break;
		}
	}

	if ( bFoundTeam == false )
	{
		//Entrance?
		if ( GetRoundNumber() == 0 )
		{
			if ( (iRoomID >= 0) && (iRoomID < GetFirstRound()->GetMaxTeams()) )
			{
				BellatraTeam * psTeam	= new BellatraTeam();
				psTeam->iID				= BELLATRASERVERHANDLER->SQLGetLastTeamInstance() + 1;
				psTeam->bActive			= true;
				psTeam->bInvalid		= false;
				psTeam->bKick			= false;
				psTeam->iAreaID			= iRoomID;
				psTeam->iRoomID			= iRoomID;
				psTeam->iKills			= 0;
				psTeam->iScore			= 0;
				psTeam->iMonstersAlive	= 0;
				psTeam->iUsersCount			= 0;
				psTeam->iSuccessUsersCount	= 0;
				psTeam->iFailUsersCount		= 0;
				psTeam->bInParty		= FALSE;
				psTeam->bIsClan			= FALSE;
				psTeam->bBoss			= false;
				vTeams.push_back( psTeam );

				BELLATRASERVERHANDLER->SQLCreateTeamInstance( this, psTeam );

				GetRoom()->Get( BELLATRA_ROOM_USERID )->Get( psTeam->iRoomID )->Add( iID );

				auto & sPlayer = psTeam->saPlayers[0];

				sPlayer.iID			= iID;
				sPlayer.bInvalid	= false;
				sPlayer.iKills		= 0;
				sPlayer.iScore		= 0;
				ZeroMemory( sPlayer.iaBellatraItemCount, sizeof( int ) * _countof( sPlayer.iaBellatraItemCount ) );
				sPlayer.iCharacterID = iCharacterID;
				sPlayer.bActive		= true;

				bRet = true;
			}
		}
	}

	return bRet;
}

bool CBellatraInstance::HasUser( int iID )
{
	return (GetPlayer( iID ) != nullptr);
}

bool CBellatraInstance::InvalidateUser( int iID )
{
	bool bRet = false;
	for ( auto psTeam : vTeams )
	{
		if ( psTeam->bActive )
		{
			for ( auto & sPlayer : psTeam->saPlayers )
			{
				if ( sPlayer.bActive && (sPlayer.bInvalid == false) && (sPlayer.iID == iID) )
				{
					GetRoom()->Get( BELLATRA_ROOM_USERID )->Get( psTeam->iRoomID )->Remove( iID );

					sPlayer.bInvalid = true;
					bRet = true;
					break;
				}
			}

			if ( bRet )
				break;
		}
	}

	return bRet;
}

void CBellatraInstance::ClearRound()
{
	for ( auto psTeam : vTeams )
	{
		if ( psTeam->bActive )
		{
			psTeam->bBoss			= false;

			psTeam->iMonstersAlive	= 0;
		}
	}
}

void CBellatraInstance::Update()
{
	for ( auto psTeam : vTeams )
	{
		if ( psTeam->bActive )
		{
			psTeam->iKills = 0;
			psTeam->iScore = 0;
			psTeam->iUsersCount = 0;
			psTeam->iSuccessUsersCount = 0;
			psTeam->iFailUsersCount = 0;

			for ( int i = 0; i < MAX_BELLATRA_PLAYERS; i++ )
			{
				auto & sPlayer = psTeam->saPlayers[i];
				if ( sPlayer.bActive )
				{
					psTeam->iUsersCount++;

					psTeam->iKills += sPlayer.iKills;
					psTeam->iScore += sPlayer.iScore;

					if ( sPlayer.bInvalid )
						psTeam->iFailUsersCount++;
					else
						psTeam->iSuccessUsersCount++;
				}
			}
		}
	}
}


CBellatraServerHandler::CBellatraServerHandler()
{
}


CBellatraServerHandler::~CBellatraServerHandler()
{
	Clear();
}

void CBellatraServerHandler::Clear()
{
	for ( auto pcBellatra : vBellatras )
		DELET( pcBellatra );

	vBellatras.clear();

	for ( auto pcInstance : vInstances )
		DELET( pcInstance );

	vInstances.clear();
}

void CBellatraServerHandler::Init()
{
	Load();
}

void CBellatraServerHandler::Load( int iID )
{
	if ( LOGIN_SERVER )
		return;

	if ( iID >= 0 )
	{
		RemoveBellatra( iID );

		if ( auto pcBellatra = SQLLoadBellatra( iID ) )
			vBellatras.push_back( pcBellatra );
	}
	else
	{
		Clear();

		std::vector<int> vBellatrasID;
		if ( SQLGetBellatras( vBellatrasID ) )
		{
			for ( auto i : vBellatrasID )
			{
				if ( auto pcBellatra = SQLLoadBellatra( i ) )
					vBellatras.push_back( pcBellatra );
			}
		}

		vBellatrasID.clear();
	}
}

void CBellatraServerHandler::HandlePacket( User * pcUser, PacketBellatraRequest * psPacket )
{
	if ( GAME_SERVER )
		return;

	if ( psPacket->iGold <= 0 )
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

void CBellatraServerHandler::HandleNetPacket( PacketNetBellatraRequest * psPacket )
{
	if ( User * pcUser = USERDATATOUSER( USERDATABYID( psPacket->iID ) ) )
	{
		for ( auto pcInstance : vInstances )
		{
			//Entrance?
			if ( pcInstance->GetRoundNumber() == 0 )
			{
				if ( (pcUser->pcUserData->sCharacterData.iLevel >= pcInstance->GetBellatra()->GetRequiredLevel().iMin) && (pcUser->pcUserData->sCharacterData.iLevel <= pcInstance->GetBellatra()->GetRequiredLevel().iMax) )
				{
					if ( pcInstance->HasUser( pcUser->GetID() ) == false )
					{
						if ( pcInstance->AddUser( pcUser->pcUserData->sBellatraRoomID, pcUser->GetID(), pcUser->iCharacterID ) )
						{
							TeleportUserRoom( pcInstance, pcUser, pcUser->pcUserData->sBellatraRoomID );

							SQLCreateCharacterInstance( pcInstance, pcInstance->GetTeamByPlayerID( pcUser->GetID() ), pcUser );
						}
					}
					break;
				}
			}
		}
	}
}

void CBellatraServerHandler::HandleNetPacket( PacketNetBellatraGiveGold * psPacket )
{
	if ( User * pcUser = USERDATATOUSER( USERDATABYID( psPacket->iID ) ) )
		CHARACTERSERVER->GiveGOLD( pcUser, psPacket->iGold );
}

void CBellatraServerHandler::Update( ETickType eTick )
{
	if ( LOGIN_SERVER )
		return;

	//Update every 100ms
	if ( eTick != TICKTYPE_100MilliSecond )
		return;

	//Bellatras
	for ( auto pcBellatra : vBellatras )
	{
		//Time to enter?
		if ( pcBellatra->GetEntranceTime() == TIMEHOUR )
		{
			//Find if have active Instances
			bool bFoundInstance = false;
			for ( auto pcInstance : vInstances )
			{
				//Same Instance?
				if ( pcInstance->GetBellatra() == pcBellatra )
				{
					//Done
					bFoundInstance = true;
					break;
				}
			}

			//New Instance
			if ( !bFoundInstance )
				vInstances.push_back( CreateInstance( pcBellatra ) );
		}
	}

	//Instances
	for ( auto pcInstance : vInstances )
	{
		//Update Instance
		pcInstance->Update();

		//Update Teams
		UpdateTeams( pcInstance );

		//Update Round Instance
		UpdateRounds( pcInstance );
	}

	//Delete Instances
	for ( auto it = vInstances.begin(); it != vInstances.end(); )
	{
		auto pcInstance = (*it);
		if ( pcInstance->IsFinished() )
		{
			if ( ((pcInstance->GetActiveRound()->GetGameTime() + MAX_BELLATRA_LEAVELASTROUND) % 24) == TIMEHOUR )
			{
				SQLEndInstance( pcInstance );

				HandleScore( pcInstance );

				ROOMINSTANCEHANDLER->Delete( pcInstance->GetRoom() );

				DELET( pcInstance );
				it = vInstances.erase( it );
			}
			else
				it++;
		}
		else
			it++;
	}

	//Ihin
	if ( bIhin )
	{
		if ( TIMEHOUR == 6 )
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
			bIhin = FALSE;
		}
	}
	else
	{
		if ( TIMEHOUR == 3 )
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

			bIhin = TRUE;
		}
	}
}

void CBellatraServerHandler::HandleItem( User * pcUser, EItemID eItemID )
{
	PacketBellatraEffectCast sPacket;
	sPacket.iLength			= sizeof( PacketBellatraEffectCast );
	sPacket.iHeader			= PKTHDR_SkillCast;
	sPacket.iBellatraCastID = 0x0D0;
	sPacket.iBellatraSealID = 0;

	for ( auto pcInstance : vInstances )
	{
		if ( auto psPlayer = pcInstance->GetPlayer( pcUser->GetID() ) )
		{
			if ( psPlayer->bActive && (psPlayer->bInvalid == false) )
			{
				if ( auto psTeam = pcInstance->GetTeamByPlayerID( pcUser->GetID() ) )
				{
					if ( psTeam->bActive && (psTeam->bInvalid == false) )
					{
						std::vector<Unit *> vUnits;
						pcInstance->GetTeamUnits( psTeam, vUnits );

						switch ( eItemID )
						{
							case ITEMID_QuakeSeal:
								HandleQuakeSeal( pcInstance, psTeam, vUnits );

								sPacket.iBellatraSealID = 1;

								psPlayer->iaBellatraItemCount[SEALTYPE_Quake]++;
								break;
							case ITEMID_StunSeal:
								HandleStunSeal( pcInstance, psTeam, vUnits );

								sPacket.iBellatraSealID = 2;

								psPlayer->iaBellatraItemCount[SEALTYPE_Stun]++;
								break;
							case ITEMID_FreezeSeal:
								HandleIceFreezeSeal( pcInstance, psTeam, vUnits );

								sPacket.iBellatraSealID = 3;

								psPlayer->iaBellatraItemCount[SEALTYPE_IceFreeze]++;
								break;
							case ITEMID_RabieSeal:
								HandleRabieSeal( pcInstance, psTeam, vUnits );

								sPacket.iBellatraSealID = 4;

								psPlayer->iaBellatraItemCount[SEALTYPE_Rabie]++;
								break;
							case ITEMID_StygianSeal:
								HandleSummonSeal( pcInstance, psTeam, pcUser, MONSTEREFFECTID_STYGIANLORD, 86 );

								sPacket.iBellatraSealID = 5;

								psPlayer->iaBellatraItemCount[SEALTYPE_Stygian]++;
								break;
							case ITEMID_GuardianSaintSeal:
								HandleSummonSeal( pcInstance, psTeam, pcUser, MONSTEREFFECTID_GUARDIAN_SAINT, 0 );

								sPacket.iBellatraSealID = 6;

								psPlayer->iaBellatraItemCount[SEALTYPE_GuardianSaint]++;
								break;
							case ITEMID_ScoreSeal:
								HandleScoreSeal( pcInstance, pcUser, psPlayer );

								sPacket.iBellatraSealID = 7;

								psPlayer->iaBellatraItemCount[SEALTYPE_Score]++;
								break;
						}

						vUnits.clear();

						if ( sPacket.iBellatraSealID > 0 )
							ProcessPacket( pcInstance, psTeam, &sPacket );
					}
				}
			}
			break;
		}
	}
}

BOOL CBellatraServerHandler::IsExclusiveDrop( User * pcUser, int iDropID )
{
	BOOL bRet = FALSE;
	for ( auto pcInstance : vInstances )
	{
		if ( auto psPlayer = pcInstance->GetPlayer( pcUser->GetID() ) )
		{
			if ( psPlayer->bActive && (psPlayer->bInvalid == false) )
			{
				if ( pcInstance->GetDropID() == iDropID )
					bRet = TRUE;
			}

			break;
		}
	}

	return bRet;
}

void CBellatraServerHandler::SendRoomScoreNPC( User * pcUser, int iRoomID )
{
	static DWORD dwaRoomTimeScoreUpdate[2][8];
	static PacketBellatraRoomScore saPacket[2][8];

	int iRankID = pcUser->GetMapID() == MAPID_NaviskoTown ? 0 : 1;

	//Safe
	if ( (iRoomID < 0) || (iRoomID > 4) )
		iRoomID = 0;

	//CHATSERVER->SendDebugChat( pcUser->pcUserData, CHATCOLOR_Error, "> Bellatra Room Log[%d]", iRoomID );

	//Time to Update?
	if ( (dwaRoomTimeScoreUpdate[iRankID][iRoomID] + 10000) < TICKCOUNT )
	{
		//Free Result
		ZeroMemory( saPacket[iRankID] + iRoomID, sizeof( PacketBellatraRoomScore ) );

		//Ranked?
		if ( auto pcInstance = pcUser->GetMapID() == MAPID_NaviskoTown ? GetRankedInstance() : GetUnrankedInstance() )
		{
			//CHATSERVER->SendDebugChat( pcUser->pcUserData, CHATCOLOR_Error, "> Bellatra Room 2 Log[%d]", iRoomID );

			//Active Room Team?
			if ( auto psTeam = pcInstance->GetTeamByRoomID( iRoomID ) )
			{
				//CHATSERVER->SendDebugChat( pcUser->pcUserData, CHATCOLOR_Error, "> Bellatra Room 3 Log[%d]", iRoomID );

				//Get Users in the Room
				std::vector<User *> vUsers;
				if ( pcInstance->GetTeamUsers( psTeam, vUsers, true ) )
				{
					//CHATSERVER->SendDebugChat( pcUser->pcUserData, CHATCOLOR_Error, "> Bellatra Room 4 Log[%d]", iRoomID );

					for ( auto pcUserBellatra : vUsers )
					{
						if ( saPacket[iRankID][iRoomID].iResultCount == MAX_PACKET_BELLATRA_RESULT )
							break;

						if ( auto psPlayer = pcInstance->GetPlayer( pcUserBellatra->GetID() ) )
						{
							//Not GM inside the Room or viewer is a GM?
							if ( (pcUserBellatra->GetGameLevel() == GAMELEVEL_None) || (pcUser->GetGameLevel() > GAMELEVEL_Three) )
							{
								//Add User Score
								STRINGCOPY( saPacket[iRankID][iRoomID].saResult[saPacket[iRankID][iRoomID].iResultCount].szCharacterName, CHARACTERSERVER->GetCharacterName( pcUserBellatra->pcUserData ) );
								saPacket[iRankID][iRoomID].saResult[saPacket[iRankID][iRoomID].iResultCount].iCharacterRank	= pcUserBellatra->pcUserData->sCharacterData.iRank;
								saPacket[iRankID][iRoomID].saResult[saPacket[iRankID][iRoomID].iResultCount].iCharacterClass	= pcUserBellatra->pcUserData->sCharacterData.iClass;
								saPacket[iRankID][iRoomID].saResult[saPacket[iRankID][iRoomID].iResultCount].iScore			= psPlayer->iScore;
								saPacket[iRankID][iRoomID].saResult[saPacket[iRankID][iRoomID].iResultCount].iKills			= psPlayer->iKills;

								saPacket[iRankID][iRoomID].iResultCount++;
							}
						}
					}
				}

				vUsers.clear();
			}
		}

		//Update
		dwaRoomTimeScoreUpdate[iRankID][iRoomID] = TICKCOUNT;
	}

	//Has Results?
	if ( saPacket[iRankID][iRoomID].iResultCount > 0 )
	{
		//CHATSERVER->SendDebugChat( pcUser->pcUserData, CHATCOLOR_Error, "> Bellatra Send Room Log[%d]", iRoomID );

		//Send Packet
		saPacket[iRankID][iRoomID].iLength		= sizeof( PacketBellatraRoomScore ) - ((MAX_PACKET_BELLATRA_RESULT - saPacket[iRankID][iRoomID].iResultCount) * sizeof( PacketBellatraRoomScore::BellatraResult ));
		saPacket[iRankID][iRoomID].iHeader		= PKTHDR_BellatraRoomScore;
		saPacket[iRankID][iRoomID].uGameCode		= RandomU( 500, 2000 );
		saPacket[iRankID][iRoomID].uGameTime		= TICKCOUNT;
		saPacket[iRankID][iRoomID].bShowScore	= TRUE;
		SENDPACKET( pcUser, &saPacket[iRankID][iRoomID] );
	}
}

void CBellatraServerHandler::RemoveBellatra( int iID )
{
	for ( auto it = vInstances.begin(); it != vInstances.end(); )
	{
		auto pcInstance = (*it);
		if ( pcInstance->GetBellatra()->GetID() == iID )
		{
			ROOMINSTANCEHANDLER->Delete( pcInstance->GetRoom() );

			DELET( pcInstance );
			it = vInstances.erase( it );
		}
		else
			it++;
	}

	for ( auto it = vBellatras.begin(); it != vBellatras.end(); )
	{
		auto pcBellatra = (*it);
		if ( pcBellatra->GetID() == iID )
		{
			DELET( pcBellatra );
			it = vBellatras.erase( it );
		}
		else
			it++;
	}
}

void CBellatraServerHandler::UpdateRounds( CBellatraInstance * pcInstance )
{
	//Entrance?
	if ( pcInstance->GetRoundNumber() == 0 )
	{
		//Has Rounds?
		if ( pcInstance->GetBellatra()->GetRounds().size() > 0 )
		{
			//Time to start?
			if ( pcInstance->GetFirstRound()->GetGameTime() == TIMEHOUR )
			{
				//Update Round
				pcInstance->SetRoundNumber( pcInstance->GetRoundNumber() + 1 );

				//Start
				pcInstance->Start();

				SendScore( pcInstance );

				//Handle Start
				HandleStartRound( pcInstance );
			}
		}
	}

	//Last Round Finished?
	if ( pcInstance->IsFinished() )
	{
		if ( ((pcInstance->GetActiveRound()->GetGameTime() + MAX_BELLATRA_LEAVELASTROUND) % 24) == TIMEHOUR )
		{
			for ( auto psTeam : pcInstance->GetTeams() )
			{
				if ( psTeam->bActive )
				{
					std::vector<User *> vUsers;
					if ( pcInstance->GetTeamUsers( psTeam, vUsers ) )
					{
						for ( auto pcUser : vUsers )
							TeleportUser( pcUser, pcInstance->GetBellatra()->GetLeaveMapID(), Point3D( pcInstance->GetBellatra()->GetLeavePosition().iX << 8, 0, pcInstance->GetBellatra()->GetLeavePosition().iZ << 8 ) );
					}

					vUsers.clear();

					SQLEndTeam( pcInstance, psTeam );

					for ( auto & sPlayer : psTeam->saPlayers )
					{
						if ( sPlayer.bActive )
							SQLEndCharacterInstance( pcInstance, &sPlayer );
					}
				}
			}
		}

		return;
	}

	if ( (pcInstance->GetRoundNumber() == 0) || (pcInstance->GetRoundNumber() > pcInstance->GetBellatra()->GetRounds().size()) )
		return;

	//Started?
	if ( pcInstance->IsStarted() )
	{
		//In Delay? No updates...
		if ( pcInstance->GetDelayTime() > 0 )
		{
			if ( pcInstance->GetDelayTime() > TICKCOUNT )
				return;
			else
			{
				SendScore( pcInstance );

				pcInstance->SetDelayTime( 0 );

				KickFailedTeams( pcInstance );
			}
		}
	}

	auto pcRound	= pcInstance->GetActiveRound();
	auto pcMap		= MAPSDATA + pcRound->GetMapID();

	//Update Monsters
	for ( auto psTeam : pcInstance->GetTeams() )
	{
		if ( psTeam->bActive )
		{
			//Spawn new monsters?
			if ( (psTeam->iMonstersAlive >= 0) && (psTeam->iMonstersAlive < pcRound->GetMaxSpawnMonsters()) )
			{
				for ( int i = 0; i < (pcRound->GetMaxSpawnMonsters() - psTeam->iMonstersAlive); i++ )
				{
					//Boss still not spawned?
					if ( psTeam->bBoss == false )
					{
						//Have kills?
						if ( psTeam->iKills >= pcInstance->GetBellatra()->GetKilledMonstersBoss() )
						{
							//Spawn Boss
							if ( auto pcUnitData = SpawnBoss( pcMap, pcRound, psTeam->iAreaID ) )
							{
								//Add to the room
								pcInstance->AddUnit( psTeam->iRoomID, pcUnitData->iID );

								psTeam->bBoss = true;

								continue;
							}
						}
					}

					//Spawn Monster
					if ( auto pcUnitData = SpawnMonster( pcMap, pcRound, psTeam->iAreaID ) )
					{
						//Add to the room
						pcInstance->AddUnit( psTeam->iRoomID, pcUnitData->iID );
					}
				}

				psTeam->iMonstersAlive = pcRound->GetMaxSpawnMonsters();
			}
		}
	}

	pcInstance->iUpdateTime += 100;
	if ( (pcInstance->iUpdateTime % 6000) == 0 )
		SendScore( pcInstance );

	//Next Round update
	{
		if ( pcInstance->GetRoundNumber() < pcInstance->GetBellatra()->GetRounds().size() )
		{
			//Next Round?
			auto pcNextRound = pcInstance->GetNextRound();
			if ( pcNextRound->GetGameTime() == TIMEHOUR )
			{
				//Stop
				pcInstance->Stop();

				//Handle End
				HandleEndRound( pcInstance );

				//Clear
				ClearRounds( pcInstance );

				//Update Round
				pcInstance->SetRoundNumber( pcInstance->GetRoundNumber() + 1 );

				//Start
				pcInstance->Start();

				//Handle Start
				HandleStartRound( pcInstance );
			}
		}
		else
		{
			//Time to Finish?
			if ( ((pcRound->GetGameTime() + MAX_BELLATRA_ENDLASTROUND) % 24) == TIMEHOUR )
			{
				//Stop
				pcInstance->Stop();

				//Set Finished (last round)
				pcInstance->Finish();

				//Handle Start
				HandleStartRound( pcInstance );

				//Clear
				ClearRounds( pcInstance );
			}
		}
	}

}

void CBellatraServerHandler::UpdateTeams( CBellatraInstance * pcInstance )
{
	std::sort( pcInstance->GetTeams().begin(), pcInstance->GetTeams().end(),
	[]( BellatraTeam * psTeamOne, BellatraTeam * psTeamTwo )
	{
		return psTeamOne->iScore > psTeamTwo->iScore;
	});

	for ( auto psTeam : pcInstance->GetTeams() )
	{
		if ( psTeam->bActive )
		{
			psTeam->bInParty = IsInPartyTeam( pcInstance, psTeam );
			psTeam->bIsClan = IsClanTeam( pcInstance, psTeam );
		}
	}
}

void CBellatraServerHandler::HandleEndRound( CBellatraInstance * pcInstance )
{

}

void CBellatraServerHandler::HandleStartRound( CBellatraInstance * pcInstance )
{
	auto pcRound = pcInstance->GetActiveRound();

	//Check if has failed
	int iTeams = 0;
	for ( auto psTeam : pcInstance->GetTeams() )
	{
		if ( psTeam->bActive )
		{
			if ( psTeam->bInvalid == false )
			{
				if ( ((pcRound->GetMinimumScore() > 0) && (psTeam->iScore < pcRound->GetMinimumScore())) || ((++iTeams) > pcRound->GetMaxTeams()) )
				{
					std::vector<User *> vUsers;
					if ( pcInstance->GetTeamUsers( psTeam, vUsers ) )
					{
						for ( auto pcUser : vUsers )
							SendStageFail( pcInstance, pcUser );
					}

					vUsers.clear();

					SendGoldTeam( pcInstance, psTeam, false );

					SendChatTeam( pcInstance, psTeam, CHATCOLOR_Global, "Bellatra> Your team failed to reach the points to the next round!" );

					SQLFailTeam( pcInstance, psTeam );

					SQLEndTeam( pcInstance, psTeam );

					for ( auto & sPlayer : psTeam->saPlayers )
					{
						if ( sPlayer.bActive )
							SQLEndCharacterInstance( pcInstance, &sPlayer );
					}

					psTeam->bInvalid = true;
					psTeam->bKick = true;
				}
				else
				{
					//Update Room
					bool bFoundArea = false;
					for ( auto psArea : pcRound->GetAreas() )
					{
						if ( psTeam->iAreaID == psArea->iRoomID )
						{
							std::vector<User *> vUsers;
							if ( pcInstance->GetTeamUsers( psTeam, vUsers ) )
							{
								for ( auto pcUser : vUsers )
								{
									if ( pcInstance->IsFinished() == FALSE )
										SendStageComplete( pcInstance, pcUser, -1 );
								}
							}

							vUsers.clear();

							bFoundArea = true;
							break;
						}
					}

					if ( !bFoundArea )
					{
						for ( auto psArea : pcRound->GetAreas() )
						{
							if ( psArea->iRoomID != psTeam->iAreaID )
							{
								bool bInUse = false;
								for ( auto psOtherTeam : pcInstance->GetTeams() )
								{
									if ( (psOtherTeam->bActive) && (psTeam != psOtherTeam) )
									{
										if ( psOtherTeam->bInvalid == false )
										{
											if ( psArea->iRoomID == psOtherTeam->iAreaID )
											{
												bInUse = true;
												break;
											}
										}
									}
								}

								if ( bInUse == false )
								{
									std::vector<User *> vUsers;
									if ( pcInstance->GetTeamUsers( psTeam, vUsers ) )
									{
										for ( auto pcUser : vUsers )
											SendStageComplete( pcInstance, pcUser, psArea->iRoomID );
									}

									vUsers.clear();

									psTeam->iAreaID = psArea->iRoomID;
									break;
								}
							}
						}
					}

					if ( pcInstance->IsFinished() )
						SendGoldTeam( pcInstance, psTeam, true );
				}
			}
		}
	}
}

BOOL CBellatraServerHandler::OnUnitKilled( User * pcUser, Unit * pcUnit )
{
	BOOL bRet = FALSE;

	//Find Player
	for ( auto pcInstance : vInstances )
	{
		if ( auto psPlayer = pcInstance->GetPlayer( pcUser->GetID() ) )
		{
			if ( psPlayer->bInvalid == false )
			{
				//Score
				LARGE_INTEGER lsScore;
				lsScore.QuadPart = UNITSERVER->GetExp( pcUnit->pcUnitData ) / pcInstance->GetActiveRound()->GetDivScore();

				if ( pcInstance->IsSameRoom( pcUser->GetID(), pcUnit->GetID() ) )
				{
					//Boss?
					if ( pcUnit->pcUnitData->sCharacterData.sGlow == TRUE )
					{
						//Notify Kill
						CHATSERVER->SendChatEx( pcUser, EChatColor::CHATCOLOR_Global, "Bellatra> %s killed!", pcUnit->GetName() );
						CHATSERVER->SendChatEx( pcUser, EChatColor::CHATCOLOR_Global, "Bellatra> %s points", FormatNumber( lsScore.LowPart ) );
					}

					//Add Score
					psPlayer->iScore += lsScore.LowPart;
					psPlayer->iKills++;
					SendScore( pcInstance, pcUser );

					//User Personal Kills
					{
						PacketSimple s;
						s.iLength = sizeof( PacketSimple );
						s.iHeader = PKTHDR_KillsInSod;
						s.iUnk = psPlayer->iKills;
						SENDPACKET( pcUser, &s, TRUE );
					}

					HandleBellatraItem( pcInstance, pcInstance->GetTeamByPlayerID( pcUser->GetID() ) );
				}
			}

			if ( auto psTeam = pcInstance->GetTeamByUnitID( pcUnit->GetID() ) )
			{
				if ( psTeam->iMonstersAlive > 0 )
					psTeam->iMonstersAlive--;

				pcInstance->InvalidateUnit( pcUnit->GetID() );
			}

			bRet = TRUE;
			break;
		}

		if ( auto psTeam = pcInstance->GetTeamByUnitID( pcUnit->GetID() ) )
		{
			if ( psTeam->iMonstersAlive > 0 )
				psTeam->iMonstersAlive--;

			pcInstance->InvalidateUnit( pcUnit->GetID() );
		}
	}

	return bRet;
}

BOOL CBellatraServerHandler::OnUnitKilledPost( Unit * pcUnit )
{
	BOOL bRet = TRUE;

	//Remove
	for ( auto pcInstance : vInstances )
		pcInstance->RemoveUnit( pcUnit->GetID() );

	return bRet;
}

BOOL CBellatraServerHandler::OnUserMapChanged( User * pcUser, EMapID eMapID )
{
	BOOL bRet = FALSE;

	for ( auto pcInstance : vInstances )
	{
		if ( auto psPlayer = pcInstance->GetPlayer( pcUser->GetID() ) )
		{
			auto pcRound = pcInstance->GetFirstRound();
			if ( pcInstance->GetRoundNumber() > 0 )
				pcRound = pcInstance->GetActiveRound();

			if ( pcRound && (pcRound->GetMapID() != eMapID) && (pcUser->GetMapID() == pcRound->GetMapID()) )
			{
				if ( psPlayer->bInvalid == false )
				{
					SendChatTeam( pcInstance, pcInstance->GetTeamByPlayerID( pcUser->GetID() ), CHATCOLOR_Notice, FormatString( "Bellatra> %s kicked!", CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) ) );

					SQLLeaveCharacterInstance( pcInstance, psPlayer, pcUser );

					pcInstance->InvalidateUser( pcUser->GetID() );

					Point3D sPosition = pcInstance->GetBellatra()->GetLeavePosition();
					sPosition.iX <<= 8;
					sPosition.iZ <<= 8;

					TeleportUser( pcUser, pcInstance->GetBellatra()->GetLeaveMapID(), sPosition );
				}
			}

			bRet = TRUE;
			break;
		}
	}

	return bRet;
}

BOOL CBellatraServerHandler::OnDisconnected( User * pcUser )
{
	if ( LOGIN_SERVER )
		return FALSE;

	BOOL bRet = FALSE;

	for ( auto pcInstance : vInstances )
	{
		if ( auto psPlayer = pcInstance->GetPlayer( pcUser->GetID() ) )
		{
			if ( psPlayer->bInvalid == false )
			{
				SendChatTeam( pcInstance, pcInstance->GetTeamByPlayerID( pcUser->GetID() ), CHATCOLOR_Notice, FormatString( "Bellatra> %s disconnect!", CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) ) );

				SQLLeaveCharacterInstance( pcInstance, psPlayer, pcUser );

				pcInstance->InvalidateUser( pcUser->GetID() );
			}

			bRet = TRUE;
			break;
		}
	}

	return bRet;
}

BOOL CBellatraServerHandler::OnNPCClick( User * pcUser, Unit * pcUnit )
{
	const int MIN_LEVEL_BELLATRA_BEGINNER	= 40;
	const int MAX_LEVEL_BELLATRA_BEGINNER	= 129;

	const int MIN_LEVEL_BELLATRA_RANKED		= 130;

	if ( (pcUser->GetMapID() == MAPID_RicartenTown) || (pcUser->GetMapID() == MAPID_NaviskoTown) )
	{
		if ( pcUnit->pcUnitData->sUnitInfo.iBellatraTypeNPC )
		{
			if ( pcUnit->pcUnitData->sUnitInfo.iBellatraTypeNPC == 5 )
				ProcessPacket( pcUser, &PacketBellatraNPC( pcUnit->pcUnitData->sUnitInfo.iBellatraTypeNPC ) );
			else
			{
				if ( (TIMEHOUR >= 4) && (TIMEHOUR < 6) )
				{
					int iCharacterLevel = pcUser->pcUserData->sCharacterData.iLevel;

					bool bSend = false;
					if ( pcUser->GetMapID() == MAPID_RicartenTown )
					{
						if ( (pcUser->GetGameLevel() >= GAMELEVEL_Two) || ((iCharacterLevel >= MIN_LEVEL_BELLATRA_BEGINNER) && (iCharacterLevel <= MAX_LEVEL_BELLATRA_BEGINNER)) )
							bSend = true;
						else
						{
							CHATSERVER->SendChatMessageBox( pcUser, "You can't enter due level requirements (%d-%d)", MIN_LEVEL_BELLATRA_BEGINNER, MAX_LEVEL_BELLATRA_BEGINNER );
							CHATSERVER->SendChatMessageBox( pcUser, "Please, visit the same NPC on Navisko Town!" );
							CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Global, "Bellatra> You can't enter due level requirements (%d-%d)", MIN_LEVEL_BELLATRA_BEGINNER, MAX_LEVEL_BELLATRA_BEGINNER );
							CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "Please, visit this NPC on Navisko Town!" );
						}
					}
					else if ( pcUser->GetMapID() == MAPID_NaviskoTown )
					{
						if ( (pcUser->GetGameLevel() >= GAMELEVEL_Two) || (iCharacterLevel >= MIN_LEVEL_BELLATRA_RANKED) )
							bSend = true;
						else
						{
							CHATSERVER->SendChatMessageBox( pcUser, "You can't enter due level requirements (%d+)", MIN_LEVEL_BELLATRA_RANKED );
							CHATSERVER->SendChatMessageBox( pcUser, "Please, visit the same NPC on Ricarten Town!" );
							CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Global, "Bellatra> You can't enter due level requirements (%d+)", MIN_LEVEL_BELLATRA_RANKED );
							CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "Bellatra> Please, visit this NPC on Ricarten Town!" );
						}
					}


					if ( bSend )
					{
						pcUser->pcUserData->sBellatraRoomID = pcUnit->pcUnitData->sUnitInfo.iBellatraTypeNPC - 1;

						ProcessPacket( pcUser, &PacketBellatraNPC( pcUnit->pcUnitData->sUnitInfo.iBellatraTypeNPC - 1 ) );
					}
				}
				else
					SendRoomScoreNPC( pcUser, pcUnit->pcUnitData->sUnitInfo.iBellatraTypeNPC - 1 );
			}

			return TRUE;
		}
	}

	return FALSE;
}

void CBellatraServerHandler::HandleScore( CBellatraInstance * pcInstance )
{
	if ( pcInstance->GetBellatra()->IsRanked() )
	{
		BellatraClanTopScore sClanTopScoreOld;
		sClanTopScoreOld.iClanID = -1;
		sClanTopScoreOld.iGold = 0;
		SQLGetTopClanScore( sClanTopScoreOld );

		struct BellatraTeamScore
		{
			int			iTeamID;
			int			iScore;
		};

		std::vector<BellatraTeamScore> vTeamsScore;

		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "SELECT TeamID, Score FROM BellatraTeamInstance WHERE (IsClan=1) AND (FailDate IS NULL) AND (InstanceID=?)" ) )
			{
				int iInstanceID = pcInstance->GetID();
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iInstanceID );
				if ( pcDB->Execute() )
				{
					while ( pcDB->Fetch() )
					{
						BellatraTeamScore sTeamScore;
						pcDB->GetData( 1, PARAMTYPE_Integer, &sTeamScore.iTeamID );
						pcDB->GetData( 2, PARAMTYPE_Integer, &sTeamScore.iScore );

						vTeamsScore.push_back( sTeamScore );
					}
				}
			}

			pcDB->Close();
		}

		for ( auto & sTeamScore : vTeamsScore )
			BELLATRASERVER->AddGoldClan( sTeamScore.iScore );

		for ( auto & sTeamScore : vTeamsScore )
		{
			int iClanID = SQLGetTeamClanID( sTeamScore.iTeamID );
			if ( iClanID >= 0 )
				SQLSetTeamScoreClanID( iClanID, sTeamScore.iScore );
		}

		vTeamsScore.clear();

		BellatraClanTopScore sClanTopScore;
		if ( SQLGetTopClanScore( sClanTopScore ) )
		{
			if ( sClanTopScore.iClanID != sClanTopScoreOld.iClanID )
			{
				CHATSERVER->SendChatAll( CHATCOLOR_Global, "Bellatra> New Top Clan!" );
				CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "Bellatra> Clan: %s", sClanTopScore.szName );
				CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "Bellatra> Score: %s", FormatNumber( sClanTopScore.iScore ) );

				pcDB = SQLCONNECTION( DATABASEID_ClanDB );
				if ( pcDB->Open() )
				{
					if ( pcDB->Prepare( "UPDATE ClanList SET BellatraGold=0" ) )
					{
						pcDB->Execute();
					}
					pcDB->Clear();
				
					if ( pcDB->Prepare( "UPDATE ClanList SET BellatraGold=? WHERE ID=?" ) )
					{
						pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &sClanTopScoreOld.iGold );
						pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &sClanTopScore.iClanID );
						pcDB->Execute();
					}
					pcDB->Close();
				}
			}
		}

		BELLATRASERVER->UpdateClanCrown();
	}
}

UnitData * CBellatraServerHandler::SpawnMonster( Map * pcMap, CBellatraRound * pcRound, int iAreaID )
{
	UnitData * pcUnitData = nullptr;

	int iRandom = RandomI( 0, pcRound->GetTotalMonstersChance() );
	int iRandomCount = 0;
	for ( auto & psMonster : pcRound->GetMonsters() )
	{
		if ( (iRandom >= iRandomCount) && (iRandom < (iRandomCount + psMonster->iChance)) )
		{
			Point3D sPosition = GetRandomPosition( pcRound, iAreaID );
			sPosition.iX <<= 8;
			sPosition.iZ <<= 8;
			sPosition.iY = pcMap->pcStage->GetHighestPoint( sPosition.iX, sPosition.iZ );

			pcUnitData = MAPSERVER->SpawnMonster( psMonster->szName, pcRound->GetMapID(), sPosition );
			if ( pcUnitData )
			{
				//Fix Name
				if ( std::string( pcUnitData->GetName() ).find( "_SOD" ) != std::string::npos )
				{
					char szName[32] = { 0 };
					STRINGCOPY( szName, pcUnitData->GetName() );

					int iLength = STRLEN( szName );
					if ( iLength > 4 )
						szName[iLength - 4] = 0;

					ZeroMemory( pcUnitData->sCharacterData.szName, 32 );
					STRINGCOPY( pcUnitData->sCharacterData.szName, szName );
				}

				pcUnitData->Animate( ANIMATIONTYPE_Idle );
				pcUnitData->iTargetMoveCount = 0;

				pcUnitData->iLureDistance = (12 * 64 * 12 * 64);
				pcUnitData->sCharacterData.iSight = (1300 * 1300);

				for ( auto psArea : pcRound->GetAreas() )
				{
					if ( psArea->iRoomID == iAreaID )
					{
						pcUnitData->sSpawnPosition = Point3D( psArea->sPosition.iX << 8, 0, psArea->sPosition.iZ << 8 );
						break;
					}
				}
			}
			break;
		}

		iRandomCount += psMonster->iChance;
	}

	return pcUnitData;
}

UnitData * CBellatraServerHandler::SpawnBoss( Map * pcMap, CBellatraRound * pcRound, int iAreaID )
{
	UnitData * pcUnitData = nullptr;

	int iRandom = RandomI( 0, pcRound->GetTotalBossesChance() );
	int iRandomCount = 0;
	for ( auto & psMonster : pcRound->GetBosses() )
	{
		if ( (iRandom >= iRandomCount) && (iRandom < (iRandomCount + psMonster->iChance)) )
		{
			Point3D sPosition = GetRandomPosition( pcRound, iAreaID );
			sPosition.iX <<= 8;
			sPosition.iZ <<= 8;
			sPosition.iY = pcMap->pcStage->GetHighestPoint( sPosition.iX, sPosition.iZ );

			pcUnitData = MAPSERVER->SpawnMonster( psMonster->szName, pcRound->GetMapID(), sPosition );
			if ( pcUnitData )
			{
				pcUnitData->sCharacterData.sGlow = TRUE;
				pcUnitData->sCharacterData.cNewLoad++;

				//Fix Name
				if ( std::string( pcUnitData->GetName() ).find( "_SOD" ) != std::string::npos )
				{
					char szName[32] = { 0 };
					STRINGCOPY( szName, pcUnitData->GetName() );

					int iLength = STRLEN( szName );
					if ( iLength > 4 )
						szName[iLength - 4] = 0;

					ZeroMemory( pcUnitData->sCharacterData.szName, 32 );
					STRINGCOPY( pcUnitData->sCharacterData.szName, szName );
				}

				pcUnitData->Animate( ANIMATIONTYPE_Idle );
				pcUnitData->iTargetMoveCount = 0;

				pcUnitData->iLureDistance = (12 * 64 * 12 * 64);
				pcUnitData->sCharacterData.iSight = (1300 * 1300);

				for ( auto psArea : pcRound->GetAreas() )
				{
					if ( psArea->iRoomID == iAreaID )
					{
						pcUnitData->sSpawnPosition = Point3D( psArea->sPosition.iX << 8, 0, psArea->sPosition.iZ << 8 );
						break;
					}
				}
			}
			break;
		}

		iRandomCount += psMonster->iChance;
	}

	return pcUnitData;
}

Point3D CBellatraServerHandler::GetRandomPosition( CBellatraRound * pcRound, int iAreaID )
{
	Point3D sPosition = Point3D( 0, 0, 0 );

	for ( auto psRoom : pcRound->GetAreas() )
	{
		if ( psRoom->iRoomID == iAreaID )
		{
			sPosition = Point3D( RandomI( psRoom->iWest, psRoom->iEast ), 0, RandomI( psRoom->iSouth, psRoom->iNorth ) );
			break;
		}
	}

	return sPosition;
}

void CBellatraServerHandler::ClearRounds( CBellatraInstance * pcInstance )
{
	//Kill Units
	KillUnits( pcInstance );

	//Clear Instance
	pcInstance->ClearRound();
}

void CBellatraServerHandler::KillUnits( CBellatraInstance * pcInstance )
{
	EMapID eMapID = pcInstance->GetActiveRound()->GetMapID();

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
				UnitData * pcUnitData = UNITDATABYIDMAP( pc->iID, eMapID );
				if ( pcUnitData )
				{
					vUnitIDs.push_back( pcUnitData->iID );

					pcUnitData->sCharacterData.sHP.sCur = 0;
					pcUnitData->Animate( ANIMATIONTYPE_Die );
				}
			}
		}
	}

//	for ( auto iID : vUnitIDs )
//		pcInstance->RemoveUnit( iID );

	vUnitIDs.clear();
}

bool CBellatraServerHandler::SQLGetBellatras( std::vector<int> & vBellatrasID )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT ID FROM BellatraList ORDER BY ID ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
					int iID = -1;

					pcDB->GetData( 1, PARAMTYPE_Integer, &iID );

					vBellatrasID.push_back( iID );
				}
			}
		}
		pcDB->Close();
	}

	return (vBellatrasID.size() > 0);
}

CBellatra * CBellatraServerHandler::SQLLoadBellatra( int iID )
{
	CBellatra * pcBellatra = nullptr;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT Name, KilledMonsterBoss, EntranceTime, RequiredLevelMin, RequiredLevelMax, LeaveMapID, LeaveMapX, LeaveMapZ, IsRanked FROM BellatraList WHERE ID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iID );
			if ( pcDB->Execute() )
			{
				if ( pcDB->Fetch() )
				{
					char szName[32]			= { 0 };
					int iKilledMonsterBoss	= 0;
					int iEntranceTime		= 0;
					EMapID eLeaveMapID		= MAPID_Invalid;
					Point3D sLeavePosition	= Point3D( 0, 0, 0 );
					BOOL bIsRanked			= FALSE;

					pcBellatra				= new CBellatra();

					pcDB->GetData( 1, PARAMTYPE_String, szName, _countof( szName ) );
					pcDB->GetData( 2, PARAMTYPE_Integer, &iKilledMonsterBoss );
					pcDB->GetData( 3, PARAMTYPE_Integer, &iEntranceTime );
					pcDB->GetData( 4, PARAMTYPE_Integer, &pcBellatra->GetRequiredLevel().iMin );
					pcDB->GetData( 5, PARAMTYPE_Integer, &pcBellatra->GetRequiredLevel().iMax );
					pcDB->GetData( 6, PARAMTYPE_Integer, &eLeaveMapID );
					pcDB->GetData( 7, PARAMTYPE_Integer, &sLeavePosition.iX );
					pcDB->GetData( 8, PARAMTYPE_Integer, &sLeavePosition.iZ );
					pcDB->GetData( 9, PARAMTYPE_Integer, &bIsRanked );

					pcBellatra->SetID( iID );
					pcBellatra->SetName( szName );
					pcBellatra->SetKilledMonstersBoss( iKilledMonsterBoss );
					pcBellatra->SetEntranceTime( iEntranceTime );
					pcBellatra->SetLeaveMapID( eLeaveMapID );
					pcBellatra->GetLeavePosition() = sLeavePosition;
					pcBellatra->SetIsRanked( bIsRanked );
				}
			}
		}
		pcDB->Close();
	}

	if ( pcBellatra )
		SQLLoadBellatraRound( pcBellatra );

	return pcBellatra;
}

bool CBellatraServerHandler::SQLLoadBellatraRound( CBellatra * pcBellatra )
{
	bool bRet = false;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT ID, Round, GameTime, MinimumScore, DelayStart, MaxMonsters, MaxTeams, MapID, DivScore FROM BellatraRound WHERE BellatraID=?" ) )
		{
			int iBellatraID = pcBellatra->GetID();
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iBellatraID );
			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
					int iID				= -1;
					int iRound			= 0;
					int iGameTime		= 0;
					int iMinimumScore	= 0;
					int iDelayStart		= 0;
					int iMaxMonsters	= 0;
					int iMaxTeams		= 0;
					EMapID eMapID		= MAPID_Invalid;
					int iDivScore		= 1;

					pcDB->GetData( 1, PARAMTYPE_Integer, &iID );
					pcDB->GetData( 2, PARAMTYPE_Integer, &iRound );
					pcDB->GetData( 3, PARAMTYPE_Integer, &iGameTime );
					pcDB->GetData( 4, PARAMTYPE_Integer, &iMinimumScore );
					pcDB->GetData( 5, PARAMTYPE_Integer, &iDelayStart );
					pcDB->GetData( 6, PARAMTYPE_Integer, &iMaxMonsters );
					pcDB->GetData( 7, PARAMTYPE_Integer, &iMaxTeams );
					pcDB->GetData( 8, PARAMTYPE_Integer, &eMapID );
					pcDB->GetData( 9, PARAMTYPE_Integer, &iDivScore );

					CBellatraRound * pcBellatraRound = new CBellatraRound();
					pcBellatraRound->SetID( iID );
					pcBellatraRound->SetRound( iRound );
					pcBellatraRound->SetGameTime( iGameTime );
					pcBellatraRound->SetMinimumScore( iMinimumScore );
					pcBellatraRound->SetDelay( iDelayStart );
					pcBellatraRound->SetMaxSpawnMonsters( iMaxMonsters );
					pcBellatraRound->SetMaxTeams( iMaxTeams );
					pcBellatraRound->SetMapID( eMapID );
					pcBellatraRound->SetDivScore( iDivScore );
					pcBellatra->GetRounds().push_back( pcBellatraRound );

					bRet = true;
				}
			}
		}
		pcDB->Close();
	}

	if ( pcBellatra->GetRounds().size() > 0 )
	{
		for ( auto pcRound : pcBellatra->GetRounds() )
		{
			SQLLoadBellatraMonsterRound( pcRound );
			SQLLoadBellatraAreaRound( pcRound );
		}
	}

	return bRet;
}

bool CBellatraServerHandler::SQLLoadBellatraMonsterRound( CBellatraRound * pcBellatraRound )
{
	bool bRet = false;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT ID, Name, Chance, IsBoss FROM BellatraMonster WHERE RoundID=?" ) )
		{
			int iRoundID = pcBellatraRound->GetID();
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iRoundID );
			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
					BellatraMonster * psMonster = new BellatraMonster();
					BOOL bIsBoss	= FALSE;

					pcDB->GetData( 1, PARAMTYPE_Integer, &psMonster->iID );
					pcDB->GetData( 2, PARAMTYPE_String, psMonster->szName, _countof( psMonster->szName ) );
					pcDB->GetData( 3, PARAMTYPE_Integer, &psMonster->iChance );
					pcDB->GetData( 4, PARAMTYPE_Integer, &bIsBoss );

					if ( bIsBoss )
					{
						pcBellatraRound->GetBosses().push_back( psMonster );
						pcBellatraRound->SetTotalBossesChance( pcBellatraRound->GetTotalBossesChance() + psMonster->iChance );
					}
					else
					{
						pcBellatraRound->GetMonsters().push_back( psMonster );
						pcBellatraRound->SetTotalMonstersChance( pcBellatraRound->GetTotalMonstersChance() + psMonster->iChance );
					}
					bRet = true;
				}
			}
		}
		pcDB->Close();
	}

	return bRet;
}

bool CBellatraServerHandler::SQLLoadBellatraAreaRound( CBellatraRound * pcBellatraRound )
{
	bool bRet = false;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT RoomID, X, Z, North, South, East, West FROM BellatraArea WHERE RoundID=?" ) )
		{
			int iRoundID = pcBellatraRound->GetID();
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iRoundID );
			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
					BellatraArea * psBellatraArea = new BellatraArea();
					psBellatraArea->sPosition.iY = 0;

					pcDB->GetData( 1, PARAMTYPE_Integer, &psBellatraArea->iRoomID );
					pcDB->GetData( 2, PARAMTYPE_Integer, &psBellatraArea->sPosition.iX );
					pcDB->GetData( 3, PARAMTYPE_Integer, &psBellatraArea->sPosition.iZ );
					pcDB->GetData( 4, PARAMTYPE_Integer, &psBellatraArea->iNorth );
					pcDB->GetData( 5, PARAMTYPE_Integer, &psBellatraArea->iSouth );
					pcDB->GetData( 6, PARAMTYPE_Integer, &psBellatraArea->iEast );
					pcDB->GetData( 7, PARAMTYPE_Integer, &psBellatraArea->iWest );

					pcBellatraRound->GetAreas().push_back( psBellatraArea );

					bRet = true;
				}
			}
		}
		pcDB->Close();
	}

	return bRet;
}

int CBellatraServerHandler::SQLGetTeamClanID( int iTeamID )
{
	int iRet = -1;

	int iCharacterID = -1;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT TOP 1 CharacterID FROM BellatraCharacterInstance WHERE TeamID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iTeamID );
			if ( pcDB->Execute() )
				if ( pcDB->Fetch() )
					pcDB->GetData( 1, PARAMTYPE_Integer, &iCharacterID );
		}

		pcDB->Close();
	}

	if ( iCharacterID >= 0 )
	{
		pcDB = SQLCONNECTION( DATABASEID_UserDB );
		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "SELECT ClanID FROM CharacterInfo WHERE ID=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iCharacterID );
				if ( pcDB->Execute() && pcDB->Fetch() )
					pcDB->GetData( 1, PARAMTYPE_Integer, &iRet, 0 );
			}
			pcDB->Close();
		}
	}

	return iRet;
}

void CBellatraServerHandler::SQLSetTeamScoreClanID( int iClanID, int iScore )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ClanDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE ClanList SET BellatraPoints=?, BellatraDate=? WHERE (ID=?) AND (BellatraPoints<?)" ) )
		{
			DWORD dwTime = (DWORD)time( NULL );
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iScore );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &dwTime );
			pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &iClanID );
			pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &iScore );
			pcDB->Execute();
		}
		pcDB->Close();
	}
}

bool CBellatraServerHandler::SQLGetTopClanScore( BellatraClanTopScore & sClanTopScore )
{
	bool bRet = false;
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ClanDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT TOP 1 ID, ClanName, BellatraPoints, BellatraGold FROM ClanList WHERE (BellatraPoints > 0) ORDER BY BellatraPoints DESC" ) )
		{
			if ( pcDB->Execute() )
			{
				if ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sClanTopScore.iClanID );
					pcDB->GetData( 2, PARAMTYPE_String, sClanTopScore.szName, _countof( sClanTopScore.szName ) );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sClanTopScore.iScore );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sClanTopScore.iGold );

					bRet = true;
				}
			}
		}
		pcDB->Close();
	}

	return bRet;
}


int CBellatraServerHandler::SQLGetLastInstance()
{
	int iRet = 0;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT TOP 1 InstanceID FROM BellatraInstance ORDER BY ID DESC" ) )
		{
			if ( pcDB->Execute() )
				if ( pcDB->Fetch() )
					pcDB->GetData( 1, PARAMTYPE_Integer, &iRet );
		}

		pcDB->Close();
	}

	if ( iRet == 0 )
		iRet = (rand() % 500) + 1;

	return iRet;
}

int CBellatraServerHandler::SQLGetLastTeamInstance()
{
	int iRet = 0;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT TOP 1 TeamID FROM BellatraTeamInstance ORDER BY ID DESC" ) )
		{
			if ( pcDB->Execute() )
				if ( pcDB->Fetch() )
					pcDB->GetData( 1, PARAMTYPE_Integer, &iRet );
		}

		pcDB->Close();
	}

	if ( iRet == 0 )
		iRet = (rand() % 500) + 1;

	return iRet;
}

bool CBellatraServerHandler::SQLCreateInstance( CBellatraInstance * pcInstance )
{
	bool bRet = false;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "INSERT INTO BellatraInstance([InstanceID],[BellatraID],[EndDate],[Date]) VALUES(?,?,NULL,GETDATE())" ) )
		{
			int iInstanceID = pcInstance->GetID();
			int iBellatraID = pcInstance->GetBellatra()->GetID();
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iInstanceID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iBellatraID );
			if ( pcDB->Execute() )
				bRet = true;
		}
		pcDB->Close();
	}

	return bRet;
}

bool CBellatraServerHandler::SQLEndInstance( CBellatraInstance * pcInstance )
{
	bool bRet = false;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE BellatraInstance SET [EndDate]=GETDATE() WHERE InstanceID=?" ) )
		{
			int iInstanceID = pcInstance->GetID();
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iInstanceID );
			if ( pcDB->Execute() )
				bRet = true;
		}
		pcDB->Close();
	}

	return bRet;
}

bool CBellatraServerHandler::SQLCreateCharacterInstance( CBellatraInstance * pcInstance, BellatraTeam * psTeam, User * pcUser )
{
	bool bRet = false;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "INSERT INTO BellatraCharacterInstance([BattleID],[InstanceID],[TeamID],[CharacterID],[Kills],[Score],[QuakeSeal],[StunSeal],[IceSeal],[RabieSeal],[StygianSeal],[GuardianSaintSeal],[ScoreSeal],[NewSeal1],[NewSeal2],[LeaveDate],[Date]) VALUES(?,?,?,?,0,0,0,0,0,0,0,0,0,0,0,NULL,GETDATE())" ) )
		{
			int iBattleID = pcInstance->GetBellatra()->GetID();
			int iInstanceID = pcInstance->GetID();
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iBattleID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iInstanceID );
			pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &psTeam->iID );
			pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &pcUser->iCharacterID );
			if ( pcDB->Execute() )
				bRet = true;
		}
		pcDB->Close();
	}

	return bRet;
}
bool CBellatraServerHandler::SQLCreateTeamInstance( CBellatraInstance * pcInstance, SOD::BellatraTeam * psTeam )
{
	bool bRet = false;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "INSERT INTO BellatraTeamInstance([InstanceID],[TeamID],[Kills],[Score],[InParty],[IsClan],[UsersCount],[SuccessUsersCount],[FailUsersCount],[FailDate],[Date]) VALUES(?,?,0,0,0,0,0,0,0,NULL,GETDATE())" ) )
		{
			int iInstanceID = pcInstance->GetID();
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iInstanceID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &psTeam->iID );
			if ( pcDB->Execute() )
				bRet = true;
		}
		pcDB->Close();
	}

	return bRet;
}
bool CBellatraServerHandler::SQLLeaveCharacterInstance( CBellatraInstance * pcInstance, BellatraPlayer * psPlayer, User * pcUser )
{
	bool bRet = false;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE BellatraCharacterInstance SET [LeaveDate]=GETDATE() WHERE (InstanceID=?) AND (CharacterID=?)" ) )
		{
			int iInstanceID = pcInstance->GetID();
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iInstanceID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &pcUser->iCharacterID );
			if ( pcDB->Execute() )
				bRet = true;
		}
		pcDB->Close();
	}

	return bRet;
}

bool CBellatraServerHandler::SQLEndCharacterInstance( CBellatraInstance * pcInstance, BellatraPlayer * psPlayer )
{
	bool bRet = false;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE BellatraCharacterInstance SET [Kills]=?, [Score]=?, [QuakeSeal]=?, [StunSeal]=?, [IceSeal]=?, [RabieSeal]=?, [StygianSeal]=?, [GuardianSaintSeal]=?, [ScoreSeal]=? WHERE (InstanceID=?) AND (CharacterID=?)" ) )
		{
			int iInstanceID = pcInstance->GetID();
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &psPlayer->iKills );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &psPlayer->iScore );
			pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &psPlayer->iaBellatraItemCount[SEALTYPE_Quake] );
			pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &psPlayer->iaBellatraItemCount[SEALTYPE_Stun] );
			pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &psPlayer->iaBellatraItemCount[SEALTYPE_IceFreeze] );
			pcDB->BindParameterInput( 6, PARAMTYPE_Integer, &psPlayer->iaBellatraItemCount[SEALTYPE_Rabie] );
			pcDB->BindParameterInput( 7, PARAMTYPE_Integer, &psPlayer->iaBellatraItemCount[SEALTYPE_Stygian] );
			pcDB->BindParameterInput( 8, PARAMTYPE_Integer, &psPlayer->iaBellatraItemCount[SEALTYPE_GuardianSaint] );
			pcDB->BindParameterInput( 9, PARAMTYPE_Integer, &psPlayer->iaBellatraItemCount[SEALTYPE_Score] );
			pcDB->BindParameterInput( 10, PARAMTYPE_Integer, &iInstanceID );
			pcDB->BindParameterInput( 11, PARAMTYPE_Integer, &psPlayer->iCharacterID );
			if ( pcDB->Execute() )
				bRet = true;
		}
		pcDB->Close();
	}

	return bRet;
}

bool CBellatraServerHandler::SQLFailTeam( CBellatraInstance * pcInstance, BellatraTeam * psTeam )
{
	bool bRet = false;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE BellatraTeamInstance SET [FailDate]=GETDATE(), [UsersCount]=?, [SuccessUsersCount]=?, [FailUsersCount]=? WHERE (InstanceID=?) AND (TeamID=?)" ) )
		{
			int iInstanceID = pcInstance->GetID();
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &psTeam->iUsersCount );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &psTeam->iSuccessUsersCount );
			pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &psTeam->iFailUsersCount );
			pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &iInstanceID );
			pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &psTeam->iID );
			if ( pcDB->Execute() )
				bRet = true;
		}
		pcDB->Close();
	}

	return bRet;
}

bool CBellatraServerHandler::SQLEndTeam( CBellatraInstance * pcInstance, BellatraTeam * psTeam )
{
	bool bRet = false;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE BellatraTeamInstance SET [Kills]=?, [Score]=?, [InParty]=?, [IsClan]=?, [UsersCount]=?, [SuccessUsersCount]=?, [FailUsersCount]=? WHERE (InstanceID=?) AND (TeamID=?)" ) )
		{
			int iInstanceID = pcInstance->GetID();
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &psTeam->iKills );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &psTeam->iScore );
			pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &psTeam->bInParty );
			pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &psTeam->bIsClan );
			pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &psTeam->iUsersCount );
			pcDB->BindParameterInput( 6, PARAMTYPE_Integer, &psTeam->iSuccessUsersCount );
			pcDB->BindParameterInput( 7, PARAMTYPE_Integer, &psTeam->iFailUsersCount );
			pcDB->BindParameterInput( 8, PARAMTYPE_Integer, &iInstanceID );
			pcDB->BindParameterInput( 9, PARAMTYPE_Integer, &psTeam->iID );
			if ( pcDB->Execute() )
				bRet = true;
		}
		pcDB->Close();
	}

	return bRet;
}

CBellatraInstance * CBellatraServerHandler::CreateInstance( CBellatra * pcBellatra )
{
	CBellatraInstance * pcInstance = new CBellatraInstance();
	pcInstance->SetID( SQLGetLastInstance() + 1 );
	pcInstance->SetBellatra( pcBellatra );
	pcInstance->SetRoom( ROOMINSTANCEHANDLER->Add( MAPID_Bellatra, FALSE ) );
	pcInstance->GetRoom()->Add( BELLATRA_ROOM_MONSTERID );
	pcInstance->GetRoom()->Add( BELLATRA_ROOM_USERID );
	
	//Has Rounds?
	if ( pcInstance->GetBellatra()->GetRounds().size() > 0 )
	{
		//Set Rooms
		for ( auto psArea : pcInstance->GetFirstRound()->GetAreas() )
		{
			pcInstance->GetRoom()->Get( BELLATRA_ROOM_MONSTERID )->Add( psArea->iRoomID );
			pcInstance->GetRoom()->Get( BELLATRA_ROOM_USERID )->Add( psArea->iRoomID );
		}
	}

	SQLCreateInstance( pcInstance );

	return pcInstance;
}

CBellatraInstance * CBellatraServerHandler::GetRankedInstance()
{
	CBellatraInstance * pcRetInstance = nullptr;

	for ( auto pcInstance : vInstances )
	{
		if ( pcInstance->GetBellatra()->IsRanked() )
		{
			pcRetInstance = pcInstance;
			break;
		}
	}

	return pcRetInstance;
}

CBellatraInstance * CBellatraServerHandler::GetUnrankedInstance()
{
	CBellatraInstance * pcRetInstance = nullptr;

	for ( auto pcInstance : vInstances )
	{
		if ( pcInstance->GetBellatra()->IsRanked() == FALSE )
		{
			pcRetInstance = pcInstance;
			break;
		}
	}

	return pcRetInstance;
}

void CBellatraServerHandler::SendGold( CBellatraInstance * pcInstance, User * pcUser, int iGold, bool bWinner )
{
	if ( bWinner )
	{
		PacketBellatraInformation s;
		s.iType = PacketBellatraInformation::BELLATRAINFOTYPE_GoldYahoo;
		s.iRound = pcInstance->GetRoundNumber() - 1;
		s.iValue1 = 100;
		s.iValue2 = iGold;
		ProcessPacket( pcUser, &s );
	}

	CHATSERVER->SendChatMessageBox( pcUser, "You won %s gp!", FormatNumber( iGold ) );
	CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Global, "Bellatra> You won %s gp!", FormatNumber( iGold ) );

	NETSERVER->SendBellatraGiveGold( pcUser, iGold );
}

void CBellatraServerHandler::SendGoldTeam( CBellatraInstance * pcInstance, BellatraTeam * psTeam, bool bWinner )
{
	if ( bWinner )
	{
		//Divide gold by Users in Party
		if ( IsInPartyTeam( pcInstance, psTeam ) && GOLD_SOD_DIVIDE_MEMBERS )
		{
			int iScore = 0;
			for ( auto & sPlayer : psTeam->saPlayers )
			{
				if ( sPlayer.bActive && (sPlayer.bInvalid == false) )
					iScore += sPlayer.iScore;
			}

			//Send Gold
			std::vector<User *> vUsers;
			if ( pcInstance->GetTeamUsers( psTeam, vUsers ) )
			{
				if ( vUsers.size() > 0 )
				{
					int iGold = iScore - ((iScore * EVENT_BELLATRA_TAX) / 100);
					iGold /= vUsers.size();

					for ( auto pcUser : vUsers )
					{
						if ( pcInstance->GetBellatra()->IsRanked() )
							SendRoomScoreNPC( pcUser, psTeam->iRoomID );

						SendGold( pcInstance, pcUser, iGold, true );
					}
				}
			}
		}
		//Solo Gold
		else
		{
			//Send Gold
			std::vector<User *> vUsers;
			if ( pcInstance->GetTeamUsers( psTeam, vUsers ) )
			{
				if ( vUsers.size() > 0 )
				{
					for ( auto pcUser : vUsers )
					{
						if ( auto psPlayer = pcInstance->GetPlayer( pcUser->GetID() ) )
						{
							if ( pcInstance->GetBellatra()->IsRanked() )
								SendRoomScoreNPC( pcUser, psTeam->iRoomID );

							int iGold = psPlayer->iScore - ((psPlayer->iScore * EVENT_BELLATRA_TAX) / 100);

							SendGold( pcInstance, pcUser, iGold, true );
						}
					}
				}
			}

			vUsers.clear();
		}
	}
	//Divide gold by Users in Team
	else
	{
		int iScore = 0;
		for ( auto & sPlayer : psTeam->saPlayers )
		{
			if ( sPlayer.bActive && (sPlayer.bInvalid == false) )
				iScore += sPlayer.iScore;
		}

		//Send Gold
		std::vector<User *> vUsers;
		if ( pcInstance->GetTeamUsers( psTeam, vUsers ) )
		{
			if ( vUsers.size() > 0 )
			{
				int iGold = iScore - ((iScore * EVENT_BELLATRA_TAX) / 100);
				iGold /= vUsers.size();

				for ( auto pcUser : vUsers )
					SendGold( pcInstance, pcUser, iGold, false );
			}
		}

		vUsers.clear();
	}
}

void CBellatraServerHandler::SendStageComplete( CBellatraInstance * pcInstance, User * pcUser, int iAreaID )
{
	PacketBellatraInformation s;
	s.iType = PacketBellatraInformation::BELLATRAINFOTYPE_StageComplete;
	s.iRound = pcInstance->GetRoundNumber() - 1;
	s.iRoomID = iAreaID;
	s.iValue2 = 0;
	ProcessPacket( pcUser, &s );
}

void CBellatraServerHandler::SendStageFail( CBellatraInstance * pcInstance, User * pcUser )
{
	PacketBellatraInformation s;
	s.iType = PacketBellatraInformation::BELLATRAINFOTYPE_StageFail;
	s.iRound = pcInstance->GetRoundNumber() - 1;
	s.iValue1 = -1;
	s.iValue2 = 0;
	ProcessPacket( pcUser, &s );
}

void CBellatraServerHandler::SendScore( CBellatraInstance * pcInstance, User * pcUser )
{
	PacketBellatraScore sPacket;
	ZeroMemory( &sPacket, sizeof( PacketBellatraScore ) );
	sPacket.iType	= PacketBellatraInformation::BELLATRAINFOTYPE_UpdateRound;
	sPacket.iValue1 = pcInstance->GetRoundNumber();

	int iTeams = 0;
	for ( auto psTeam : pcInstance->GetTeams() )
	{
		sPacket.iaScoreTeams[psTeam->iRoomID] = psTeam->iScore;

		iTeams++;

		if ( iTeams == 4 )
			break;
	}

	//Solo
	if ( pcUser )
	{
		if ( auto psTeam = pcInstance->GetTeamByPlayerID( pcUser->GetID() ) )
		{
			if ( auto psPlayer = pcInstance->GetPlayer( pcUser->GetID() ) )
			{
				sPacket.iScore = psPlayer->iScore;
				sPacket.iTeamRoomID = psTeam->iRoomID;
				sPacket.iRound = 1;

				ProcessPacket( pcUser, &sPacket );
			}
		}
	}
	//Team
	else
	{
		for ( auto psTeam : pcInstance->GetTeams() )
		{
			if ( psTeam->bActive && (psTeam->bInvalid == false) )
			{
				for ( auto & sPlayer : psTeam->saPlayers )
				{
					if ( sPlayer.bActive && (sPlayer.bInvalid == false) )
					{
						for ( UINT i = 0; i < USERSERVER->uUsersInGame; i++ )
						{
							User * pcUserSod = USERSERVER->pcaUserInGame[i];
							if ( pcUserSod->GetID() == sPlayer.iID )
							{
								sPacket.iScore = sPlayer.iScore;
								sPacket.iTeamRoomID = psTeam->iRoomID;
								sPacket.iRound = 0;

								ProcessPacket( pcUserSod, &sPacket );
								break;
							}
						}
					}
				}
			}
		}
	}
}

BOOL CBellatraServerHandler::IsInPartyTeam( CBellatraInstance * pcInstance, BellatraTeam * psTeam )
{
	BOOL bRet = TRUE;

	//Get Team Users
	std::vector<User *> vUsers;
	if ( pcInstance->GetTeamUsers( psTeam, vUsers ) )
	{
		if ( vUsers.size() > 0 )
		{
			//User in Party?
			User * pcFirstUser = vUsers[0];
			if ( pcFirstUser->bParty )
			{
				bRet = TRUE;

				//Get Party Users
				std::vector<User *> vUsersParty = PARTYHANDLER->GetPartyMembers( pcFirstUser->psParty );
				for ( auto pcTeamUser : vUsers )
				{
					//Find User Team
					bool bFound = false;
					for ( auto pcPartyUser : vUsersParty )
					{
						//In Party?
						if ( pcPartyUser == pcTeamUser )
						{
							//Done
							bFound = true;
							break;
						}
					}

					//Not in Party?
					if ( bFound == false )
					{
						//Error
						bRet = TRUE;
						break;
					}
				}

				vUsersParty.clear();
			}
		}
	}

	vUsers.clear();

	return bRet;
}

BOOL CBellatraServerHandler::IsClanTeam( CBellatraInstance * pcInstance, BellatraTeam * psTeam )
{
	BOOL bRet = FALSE;

	//Get Team Users
	std::vector<User *> vUsers;
	if ( pcInstance->GetTeamUsers( psTeam, vUsers ) )
	{
		//Compare Clans
		BOOL bDifferent = FALSE;
		for ( auto pcUser : vUsers )
		{
			//User has nos clan?
			if ( pcUser->pcUserData->iClanID == 0 )
			{
				//Error
				bDifferent = TRUE;
				break;
			}

			//Compare Other User
			for ( auto pcOtherUser : vUsers )
			{
				//User isn't the same?
				if ( pcUser != pcOtherUser )
				{
					//User has no clan or different clan?
					if ( (pcOtherUser->pcUserData->iClanID == 0) || (pcUser->pcUserData->iClanID != pcOtherUser->pcUserData->iClanID) )
					{
						//Error
						bDifferent = TRUE;
						break;
					}
				}
			}

			if ( bDifferent )
				break;
		}

		//Same Clan?
		if ( (bRet = !bDifferent) == TRUE )
		{
			//Minimum Users in the clan? Done!
			if ( vUsers.size() < MIN_CLANUSERS_SCORE )
				bRet = FALSE;
		}
	}

	vUsers.clear();

	return bRet;
}

bool CBellatraServerHandler::KickFailedTeams( CBellatraInstance * pcInstance )
{
	for ( auto psTeam : pcInstance->GetTeams() )
	{
		if ( psTeam->bActive && psTeam->bInvalid && psTeam->bKick )
		{
			std::vector<User *> vUsers;
			if ( pcInstance->GetTeamUsers( psTeam, vUsers ) )
			{
				for ( auto pcUser : vUsers )
				{
					pcInstance->InvalidateUser( pcUser->GetID() );

					Point3D sPosition = pcInstance->GetBellatra()->GetLeavePosition();
					sPosition.iX <<= 8;
					sPosition.iZ <<= 8;

					//TeleportUser( pcUser, pcInstance->GetBellatra()->GetLeaveMapID(), sPosition );
				}
			}

			psTeam->bKick = false;
		}
	}

	return true;
}

void CBellatraServerHandler::ProcessPacket( User * pcUser, PacketBellatraNPC * psPacket )
{
	psPacket->iLength = sizeof( PacketBellatraNPC );
	psPacket->iHeader = PKTHDR_NPCBellatra;
	SENDPACKET( pcUser, psPacket );
}

void CBellatraServerHandler::ProcessPacket( User * pcUser, PacketBellatraInformation * psPacket )
{
	psPacket->iLength = sizeof( PacketBellatraInformation );
	psPacket->iHeader = PKTHDR_BellatraInformation;
	SENDPACKET( pcUser, psPacket );
}

void CBellatraServerHandler::ProcessPacket( User * pcUser, PacketBellatraScore * psPacket )
{
	psPacket->iLength = sizeof( PacketBellatraScore );
	psPacket->iHeader = PKTHDR_BellatraInformation;
	SENDPACKET( pcUser, psPacket );
}

void CBellatraServerHandler::ProcessPacket( CBellatraInstance * pcInstance, BellatraTeam * psTeam, PacketBellatraEffectCast * psPacket )
{
	psPacket->iLength = sizeof( PacketBellatraEffectCast );
	psPacket->iHeader = PKTHDR_SkillCast;

	std::vector<User *> vUsers;
	if ( pcInstance->GetTeamUsers( psTeam, vUsers ) )
		for ( auto pcUser : vUsers )
			SENDPACKET( pcUser, psPacket );

	vUsers.clear();
}

void CBellatraServerHandler::ProcessPacket( CBellatraInstance * pcInstance, BellatraTeam * psTeam, PacketEffectSpawn * psPacket )
{
	psPacket->iLength = sizeof( PacketEffectSpawn );
	psPacket->iHeader = PKTHDR_EffectSpawn;

	std::vector<User *> vUsers;
	if ( pcInstance->GetTeamUsers( psTeam, vUsers ) )
		for ( auto pcUser : vUsers )
			SENDPACKET( pcUser, psPacket );

	vUsers.clear();
}

void CBellatraServerHandler::TeleportUserRoom( CBellatraInstance * pcInstance, User * pcUser, int iAreaID )
{
	CBellatraRound * pcRound = nullptr;
	if ( pcInstance->GetRoundNumber() == 0 )
		pcRound = pcInstance->GetFirstRound();
	else
		pcRound = pcInstance->GetActiveRound();

	if ( pcRound )
	{
		for ( auto psArea : pcRound->GetAreas() )
		{
			if ( psArea->iRoomID == iAreaID )
			{
				TeleportUser( pcUser, pcRound->GetMapID(), Point3D( psArea->sPosition.iX << 8, 0, psArea->sPosition.iZ << 8 ) );
				break;
			}
		}
	}
}

void CBellatraServerHandler::TeleportUser( User * pcUser, EMapID iMapID, Point3D sPosition )
{
	PacketWarpGateField sPacket;
	sPacket.iLength = sizeof( PacketWarpGateField );
	sPacket.iHeader = PKTHDR_WarpGateField;
	sPacket.iX		= sPosition.iX + (rand() % 8192);
	sPacket.iZ		= sPosition.iZ + (rand() % 8192);
	sPacket.iStage	= iMapID;
	SENDPACKET( pcUser, &sPacket );
}

void CBellatraServerHandler::SendChatTeam( CBellatraInstance * pcInstance, BellatraTeam * psTeam, EChatColor eChatColor, const std::string strMessage )
{
	std::vector<User *> vUsers;
	if ( pcInstance->GetTeamUsers( psTeam, vUsers ) )
	{
		for ( auto pcUser : vUsers )
			CHATSERVER->SendChat( pcUser, eChatColor, strMessage.c_str() );
	}

	vUsers.clear();
}

void CBellatraServerHandler::HandleBellatraItem( CBellatraInstance * pcInstance, BellatraTeam * psTeam )
{
	struct BellatraItemSeal
	{
		EItemID			eItemID;
		int				iChance;
	};

	const BellatraItemSeal saBellatraSeals[] =
	{
		{ITEMID_QuakeSeal,			20},
		{ITEMID_StunSeal,			40},
		{ITEMID_FreezeSeal,			60},
		{ITEMID_RabieSeal,			68},
		{ITEMID_StygianSeal,		80},
		{ITEMID_GuardianSaintSeal,	95},
		{ITEMID_ScoreSeal,			100},
		{ITEMID_None,				0}
	};

	if ( RandomI( 0, 1000 ) < 10 )
	{
		int iRandom = RandomI( 0, 99 );
		for ( int i = 0; i < _countof( saBellatraSeals ); i++ )
		{
			if ( saBellatraSeals[i].eItemID == ITEMID_None )
				break;

			if ( iRandom < saBellatraSeals[i].iChance )
			{
				Map::Loot::LootItem sLootItem;

				//Get Item Data
				DefinitionItem * psDefinitionItem = ITEMSERVER->FindItemDefByCode( saBellatraSeals[i].eItemID );
				if ( psDefinitionItem )
				{
					//Create Item with non-spec
					ITEMSERVER->CreateItem( &sLootItem.sItem, psDefinitionItem, 0 );
					
					//Get Map Data
					Map * pcMap = MAPSDATA + pcInstance->GetActiveRound()->GetMapID();
					if ( pcMap )
					{
						Point3D sPosition	= GetRandomPosition( pcInstance->GetActiveRound(), psTeam->iAreaID );
						sPosition.iY		= pcMap->pcStage->GetHighestPoint( sPosition.iX << 8, sPosition.iZ << 8 ) >> 8;

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
						}
					}
				}

				break;
			}
		}
	}

	//3%
	if ( pcInstance->GetBellatra()->IsRanked() && (RandomI( 0, 1000 ) < 9) )
	{
		Map::Loot::LootItem sLootItem;

		//Get Item Data
		DefinitionItem * psDefinitionItem = ITEMSERVER->FindItemDefByCode( ITEMID_BrilliantStone );
		if ( psDefinitionItem )
		{
			//Create Item with non-spec
			ITEMSERVER->CreateItem( &sLootItem.sItem, psDefinitionItem, 0 );

			//Get Map Data
			Map * pcMap = MAPSDATA + pcInstance->GetActiveRound()->GetMapID();
			if ( pcMap )
			{
				Point3D sPosition = GetRandomPosition( pcInstance->GetActiveRound(), psTeam->iAreaID );
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
				}
			}
		}
	}
}

void CBellatraServerHandler::HandleQuakeSeal( CBellatraInstance * pcInstance, BellatraTeam * psTeam, std::vector<Unit *> & vUnits )
{
	for ( auto pcUnit : vUnits )
	{
		if ( (pcUnit->pcUnitData->GetAnimation() != ANIMATIONTYPE_Die) && (pcUnit->pcUnitData->sCharacterData.sGlow != TRUE) )
		{
			pcUnit->pcUnitData->sCharacterData.sHP.sCur -= pcUnit->pcUnitData->sCharacterData.sHP.sMax / 3;
			if ( pcUnit->pcUnitData->sCharacterData.sHP.sCur <= 0 )
				pcUnit->pcUnitData->sCharacterData.sHP.sCur = 1;

			pcUnit->pcUnitData->Animate( ANIMATIONTYPE_Flinch );
		}
	}
}

void CBellatraServerHandler::HandleStunSeal( CBellatraInstance * pcInstance, BellatraTeam * psTeam, std::vector<Unit *> & vUnits )
{
	for ( auto pcUnit : vUnits )
	{
		if ( (pcUnit->pcUnitData->GetAnimation() != ANIMATIONTYPE_Die) && (pcUnit->pcUnitData->sCharacterData.sGlow != TRUE) )
		{
			pcUnit->pcUnitData->Animate( ANIMATIONTYPE_Flinch );

			pcUnit->pcUnitData->iStunTimeLeft = 16 * 10;
		}
	}
}

void CBellatraServerHandler::HandleIceFreezeSeal( CBellatraInstance * pcInstance, BellatraTeam * psTeam, std::vector<Unit *> & vUnits )
{
	for ( auto pcUnit : vUnits )
	{
		if ( (pcUnit->pcUnitData->GetAnimation() != ANIMATIONTYPE_Die) && (pcUnit->pcUnitData->sCharacterData.sGlow != TRUE) )
		{
			pcUnit->pcUnitData->Animate( ANIMATIONTYPE_Flinch );

			pcUnit->pcUnitData->iFreezeTimeLeft		= 16 * 15;
			pcUnit->pcUnitData->iUpdateMoveReduce	= 0;
		}
	}
}

void CBellatraServerHandler::HandleRabieSeal( CBellatraInstance * pcInstance, BellatraTeam * psTeam, std::vector<Unit *> & vUnits )
{
	auto psCharacterMutantRabie = UNITSERVER->GetCharacterDataByEffect( MONSTEREFFECTID_MUTANTRABIE );
	auto psCharacterRabie		= UNITSERVER->GetCharacterDataByEffect( MONSTEREFFECTID_RABIE );

	if ( psCharacterMutantRabie && psCharacterRabie )
	{
		for ( auto pcUnit : vUnits )
		{
			if ( (pcUnit->pcUnitData->GetAnimation() != ANIMATIONTYPE_Die) && (pcUnit->pcUnitData->sCharacterData.sGlow != TRUE) )
			{
				if ( RandomI( 0, 1 ) == 0 )
					UNITSERVER->UnitSwapper( pcUnit->pcUnitData, psCharacterRabie, psCharacterRabie->psUnitInfo );
			}
		}

		CBellatraRound * pcRound = nullptr;
		if ( pcInstance->GetRoundNumber() == 0 )
			pcRound = pcInstance->GetFirstRound();
		else
			pcRound = pcInstance->GetActiveRound();

		if ( pcRound )
		{
			for ( auto psArea : pcRound->GetAreas() )
			{
				if ( psArea->iRoomID == psTeam->iAreaID )
				{
					Map * pcMap = MAPSDATA + pcRound->GetMapID();

					Point3D sPosition	= GetRandomPosition( pcRound, psTeam->iAreaID ) << 8;
					sPosition.iY		= pcMap->pcStage->GetHighestPoint( sPosition.iX, sPosition.iZ );

					if ( auto pcUnitData = MAPSERVER->SpawnMonster( psCharacterMutantRabie->szName, pcRound->GetMapID(), sPosition ) )
					{
						pcUnitData->sUnitInfo.iNumDrops = 0;

						pcUnitData->sCharacterData.sGlow = TRUE;

						pcUnitData->iLureDistance = (12 * 64 * 12 * 64);
						pcUnitData->sCharacterData.iSight = (1300 * 1300);

						pcUnitData->sSpawnPosition = Point3D( psArea->sPosition.iX << 8, 0, psArea->sPosition.iZ << 8 );

						//Fix Name
						if ( std::string( pcUnitData->GetName() ).find( "_SOD" ) != std::string::npos )
						{
							char szName[32] = { 0 };
							STRINGCOPY( szName, pcUnitData->GetName() );

							int iLength = STRLEN( szName );
							if ( iLength > 4 )
								szName[iLength - 4] = 0;

							ZeroMemory( pcUnitData->sCharacterData.szName, 32 );
							STRINGCOPY( pcUnitData->sCharacterData.szName, szName );
						}

						pcInstance->AddUnit( psTeam->iRoomID, pcUnitData->iID );

						psTeam->iMonstersAlive++;

						ProcessPacket( pcInstance, psTeam, &PacketEffectSpawn( sPosition ) );
					}
					break;
				}
			}
		}
	}
}

void CBellatraServerHandler::HandleSummonSeal( CBellatraInstance * pcInstance, BellatraTeam * psTeam, User * pcUser, EMonsterEffectID eMonsterEffectID, int iLevel )
{
	if ( auto psCharacterData = UNITSERVER->GetCharacterDataByEffectLevel( eMonsterEffectID, iLevel ) )
	{
		CBellatraRound * pcRound = nullptr;
		if ( pcInstance->GetRoundNumber() == 0 )
			pcRound = pcInstance->GetFirstRound();
		else
			pcRound = pcInstance->GetActiveRound();

		if ( pcRound )
		{
			for ( auto psArea : pcRound->GetAreas() )
			{
				if ( psArea->iRoomID == psTeam->iAreaID )
				{
					Map * pcMap = MAPSDATA + pcRound->GetMapID();

					Point3D sPosition = GetRandomPosition( pcRound, psTeam->iAreaID ) << 8;
					sPosition.iY = pcMap->pcStage->GetHighestPoint( sPosition.iX, sPosition.iZ );

					if ( auto pcUnitData = MAPSERVER->SpawnMonster( psCharacterData->szName, pcRound->GetMapID(), sPosition ) )
					{
						pcUnitData->sUnitInfo.iNumDrops = 0;

						pcUnitData->iLureDistance = (12 * 64 * 12 * 64);
						pcUnitData->sCharacterData.iSight = (1300 * 1300);
						pcUnitData->pcOwner = pcUser->pcUserData;
						pcUnitData->sUnitInfo.dwHashCharacterName = 0;
						pcUnitData->sCharacterData.iMonsterType = MONSTERTYPE_Friendly;
						STRINGCOPY( pcUnitData->sCharacterData.Pet.szOwnerName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );
						pcUnitData->sCharacterData.iOwnerID = pcUser->GetID();
						pcUnitData->dwLastUpdateTime = (300 * 1000) + TICKCOUNT;

						pcUnitData->sSpawnPosition = Point3D( psArea->sPosition.iX << 8, 0, psArea->sPosition.iZ << 8 );

						//Fix Name
						if ( std::string( pcUnitData->GetName() ).find( "_SOD" ) != std::string::npos )
						{
							char szName[32] = { 0 };
							STRINGCOPY( szName, pcUnitData->GetName() );

							int iLength = STRLEN( szName );
							if ( iLength > 4 )
								szName[iLength - 4] = 0;

							ZeroMemory( pcUnitData->sCharacterData.szName, 32 );
							STRINGCOPY( pcUnitData->sCharacterData.szName, szName );
						}

						pcInstance->AddUnit( psTeam->iRoomID, pcUnitData->iID );

						ProcessPacket( pcInstance, psTeam, &PacketEffectSpawn( sPosition ) );
					}
					break;
				}
			}
		}
	}
}

void CBellatraServerHandler::HandleScoreSeal( CBellatraInstance * pcInstance, User * pcUser, BellatraPlayer * psPlayer )
{
	if ( psPlayer->bInvalid == false )
	{
		psPlayer->iScore += 5000;

		SendScore( pcInstance, pcUser );
	}
}
