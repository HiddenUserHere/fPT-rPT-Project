#include "stdafx.h"
#include "FuryArenaHandler.h"


CFuryArenaHandler::CFuryArenaHandler()
{
	for ( int i = 0; i < _MINION_FURYARENA_CNT; i++ )
		pcUnitDataMinions[i] = NULL;

	// Boss Name
	ZeroMemory( szBossName[0], 32 );
	STRINGCOPY( szBossName[0], _BOSS_FURYARENA );
	ZeroMemory( szBossName[1], 32 );
	STRINGCOPY( szBossName[1], _BOSS_FURYARENALOW );
	// Minion Name
	ZeroMemory( szMinionName[0], 32 );
	STRINGCOPY( szMinionName[0], _MINION_FURYARENA );
	ZeroMemory( szMinionName[1], 32 );
	STRINGCOPY( szMinionName[1], _MINION_FURYARENALOW );
	// Minion Name
	ZeroMemory( szMonsterEventName[0], 32 );
	STRINGCOPY( szMonsterEventName[0], _MONSTEREVENT_FURYARENA );
	ZeroMemory( szMonsterEventName[1], 32 );
	STRINGCOPY( szMonsterEventName[1], _MONSTEREVENT_FURYARENALOW );

	iaTotalPercent[0] = 0;
	iaTotalPercent[1] = 0;
	iaTotalPercent[2] = 0;
	iaTotalPercent[3] = 0;
}


CFuryArenaHandler::~CFuryArenaHandler()
{
	for ( auto & v : vaPrizesReward[0] )
		DELET( v );

	vaPrizesReward[0].clear();

	for ( auto & v : vaPrizesReward[1] )
		DELET( v );

	vaPrizesReward[1].clear();

	for ( auto & v : vaPrizesReward[2] )
		DELET( v );

	vaPrizesReward[2].clear();

	for ( auto & v : vaPrizesReward[3] )
		DELET( v );

	vaPrizesReward[3].clear();
}

BOOL CFuryArenaHandler::InsideArena( Point3D * psPosition )
{
	if ( psPosition->iX >= _XMIN_FURYARENA && psPosition->iX <= _XMAX_FURYARENA )
	{
		if ( psPosition->iZ >= _ZMIN_FURYARENA && psPosition->iZ <= _ZMAX_FURYARENA )
			return TRUE;
	}
	return FALSE;
}

int CFuryArenaHandler::GetTotalUsers()
{
	int iUsers = 0;

	for ( int i = 0; i < PLAYERS_MAX; i++ )
	{
		UserData * ud = USERSDATA + i;
		User * u = USERDATATOUSER( ud );
		if ( ud && u )
		{
			if ( u->bInGrandFury )
				iUsers++;
		}
	}
	return iUsers;
}

BOOL CFuryArenaHandler::IsBoss( UnitData * pcUnitData )
{
	if ( STRINGCOMPARE( szBossName[iEventIDNow], pcUnitData->GetName() ) )
	{
		pcUnitData->sPosition.iX = _X_KINGFURY;
		pcUnitData->sPosition.iZ = _Z_KINGFURY;
	}
	return TRUE;
}

BOOL CFuryArenaHandler::KillUnits()
{
	// Boss
	if ( pcUnitDataBoss )
	{
		if ( !pcUnitDataBoss->IsDead() )
			UNITSERVER->SetMotionFromCode( pcUnitDataBoss, UnitServer::MOTIONID_Death );
		
		pcUnitDataBoss = NULL;
	}

	// Minions
	for ( int i = 0; i < _MINION_FURYARENA_CNT; i++ )
	{
		if ( pcUnitDataMinions[i] )
		{
			if ( !pcUnitDataMinions[i]->IsDead() )
				UNITSERVER->SetMotionFromCode( pcUnitDataMinions[i], UnitServer::MOTIONID_Death );
		}
		pcUnitDataMinions[i] = NULL;
	}
	return TRUE;
}

void CFuryArenaHandler::SQLLog( char * pszLog, ... )
{
	va_list vl;

	static char szString[1024];

	va_start( vl, pszLog );
	_vsnprintf_s( szString, _countof( szString ), _TRUNCATE, pszLog, vl );
	va_end( vl );

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "INSERT INTO FuryArenaLog([Description],[Date]) VALUES(?,GETDATE())" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, szString, STRLEN( szString ) );
			pcDB->Execute();
		}
		pcDB->Close();
	}
}

SPrizeData * CFuryArenaHandler::GetRewardItem( int iID )
{
	// Randomize Items
	int iRand	= (rand() << 7) | ((GetCurrentTime() >> 2) & 0x7F);
	iRand		= iRand % iaTotalPercent[iID];
	
	int iTotal = 0;

	for ( auto & p : vaPrizesReward[iID] )
	{
		// Items for boss ID?
		if ( p->bActive )
		{
			if ( (iRand >= iTotal) && (iRand < (iTotal + p->iPercent)) )
				return p;

			iTotal += p->iPercent;
		}
	}
	return vaPrizesReward[iID][0];
}

int CFuryArenaHandler::GetEventID()
{
	return iEventID;
}

UINT64 CFuryArenaHandler::GetTimeLeft( SYSTEMTIME * ps )
{
	static const DWORD dw6HoursTGF = (60 * 60 * 6);

	UINT64 dw = 0;
	
	UINT64 uTimePS = SystemTimeToQWORD( ps );

	SYSTEMTIME s;
	ZeroMemory( &s, sizeof( SYSTEMTIME ) );
	
	s.wHour = 6;
	s.wDay = ps->wDay;
	s.wMonth = ps->wMonth;
	s.wYear = ps->wYear;
	dw = (SystemTimeToQWORD( &s ) - uTimePS);
	if ( dw <= dw6HoursTGF )
	{
		iEventID = 1;
		return dw;
	}

	s.wHour = 12;
	dw = (SystemTimeToQWORD( &s ) - uTimePS);
	if ( dw <= dw6HoursTGF )
	{
		iEventID = 0;
		return dw;
	}

	s.wHour = 18;
	dw = (SystemTimeToQWORD( &s ) - uTimePS);
	if ( dw <= dw6HoursTGF )
	{
		iEventID = 1;
		return dw;
	}
	s.wHour = 0;
	dw = ((SystemTimeToQWORD( &s ) + (60 * 60 * 24)) - uTimePS);
	if ( dw <= dw6HoursTGF )
	{
		iEventID = 0;
		return dw;
	}

	return 0;
}

BOOL CFuryArenaHandler::ExchangeItem( User * pcUser, PacketFuryArenaRequest * psPacket )
{
	if ( psPacket->p1 != 100 )
		return FALSE;

	int iMonster = psPacket->p4;
	int iMinutes = psPacket->p5;

	INT64 iExp	= 0;

	int iID		= -1;
	
	if ( iMonster == ITEMCRAFTTYPE_TGFKingFury )
	{
		// EXP King Fury
		iExp		= 100000000000;
		if ( iMinutes > 10 && iMinutes <= 20 )
			iExp	= 80000000000;
		else if ( iMinutes > 20 )
			iExp	= 65000000000;

		iID = 0;
	}
	else
	{
		// EXP Lord Babel
		iExp		= 60000000000;
		if ( iMinutes > 10 && iMinutes <= 20 )
			iExp	= 45000000000;
		else if ( iMinutes > 20 )
			iExp	= 30000000000;

		iID = 1;
	}

	if ( (*(int *)0x0084601C) > 0 )
		iExp += (iExp * (*(int *)0x0084601C)) / 100;

	// Get Prize
	SPrizeData * p = GetRewardItem( iID );

	// Exp?
	if ( STRINGCOMPAREI( "GG101", p->szCode ) )
	{
		if ( (psPacket->iLevel ^ 0x5767) < SERVER_LEVEL_MAX )
		{
			//Give EXP to user
			CHARACTERSERVER->GiveEXP( pcUser, iExp );

			//Send EXP Sound
			PacketFuryArenaRequest sPacket;
			sPacket.iLength = sizeof( PacketFuryArenaRequest );
			sPacket.iHeader = PKTHDR_FuryArena;
			sPacket.eState = FURYARENASTATEID_EXP;
			SENDPACKET( pcUser, &sPacket, TRUE );

			//Log It
			SQLLog( "AccountName[%s] CharacterName[%s] Received EXP [%s]", pcUser->pcUserData->szAccountName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), FormatNumber( iExp ) );

			//Send message to user to be confirmed
			CHATSERVER->SendChatMessageBox( pcUser, "You received %s EXP", FormatNumber( iExp ) );
			CHATSERVER->SendChatEx( pcUser, EChatColor::CHATCOLOR_Error, "You received %s EXP", FormatNumber( iExp ) );
			SENDPACKETBLANK( pcUser, PKTHDR_Save, TRUE );
		}
		else
		{
			int iGold = (iMonster == ITEMCRAFTTYPE_TGFKingFury ? 20000000 : 10000000);

			//Gold
			CHARACTERSERVER->GiveGOLD( pcUser, iGold, WHEREID_FuryArena );

			//Send EXP Sound
			PacketFuryArenaRequest sPacket;
			sPacket.iLength = sizeof( PacketFuryArenaRequest );
			sPacket.iHeader = PKTHDR_FuryArena;
			sPacket.eState = FURYARENASTATEID_Gold;
			SENDPACKET( pcUser, &sPacket, TRUE );

			//Log It
			SQLLog( "AccountName[%s] CharacterName[%s] Received Gold [%s gp]", pcUser->pcUserData->szAccountName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), FormatNumber( iGold ) );

			//Send message to user to be confirmed
			CHATSERVER->SendChatMessageBox( pcUser, "You received %s gp", FormatNumber( iGold ) );
			CHATSERVER->SendChatEx( pcUser, EChatColor::CHATCOLOR_Error, "You received %s gp", FormatNumber( iGold ) );
			SENDPACKETBLANK( pcUser, PKTHDR_Save, TRUE );
		}
	}
	else
	{
		// Item
		ItemData * psItemData = ITEMSERVER->FindItemPointerTable( p->szCode );
		if ( psItemData )
		{
			DefinitionItem * psDef = ITEMSERVER->FindItemDefByCode( psItemData->sBaseItemID.ToItemID() );
			if ( psDef )
			{
				// Structure Item Data to send to user
				PacketFuryArenaItemData sPacket;
				sPacket.iLength = sizeof( PacketFuryArenaItemData );
				sPacket.iHeader = PKTHDR_FuryArenaItemData;
				CopyMemory( &sPacket.sItemData, psItemData, sizeof( ItemData ) );
				
				ITEMSERVER->CreateItem( &sPacket.sItemData.sItem, psDef );
				ITEMSERVER->SaveItemData( &sPacket.sItemData.sItem );
				ITEMSERVER->ValidateItem( &sPacket.sItemData.sItem );
				ITEMSERVER->AddItemInventory( pcUser->pcUserData, sPacket.sItemData.sItem.sItemID.ToItemID(), sPacket.sItemData.sItem.iChk1, sPacket.sItemData.sItem.iChk2 );
				
				SENDPACKET( pcUser, &sPacket, TRUE );

				// Log It
				SQLLog( "AccountName[%s] CharacterName[%s] Received Item [0x%08X][%s]", pcUser->pcUserData->szAccountName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ),
					psItemData->sBaseItemID.ToItemID(), psItemData->szBaseName );
				
				// Send message to user to be confirmed
				CHATSERVER->SendChatMessageBox( pcUser, "You received %s", psItemData->szBaseName );
				CHATSERVER->SendChatEx( pcUser, EChatColor::CHATCOLOR_Error, "You received %s", psItemData->szBaseName );
			}
		}
	}

	return TRUE;
}

BOOL CFuryArenaHandler::SetBoss( const char * pszMonsterName )
{
//	STRINGCOPY( szBossName, pszMonsterName );
	return TRUE;
}

BOOL CFuryArenaHandler::SetMinion( const char * pszMonsterName )
{
//	STRINGCOPY( szMinionName, pszMonsterName );
	return TRUE;
}

BOOL CFuryArenaHandler::SetMonsterEvent( const char * pszMonsterName )
{
//	STRINGCOPY( szMonsterEventName, pszMonsterName );
	return TRUE;
}

BOOL CFuryArenaHandler::UpdateTime()
{
	dwTickTime = (DWORD)GetTimeLeft( GetServerTime() );
	return TRUE;
}

BOOL CFuryArenaHandler::SetTimeMinion( int iMinute )
{
	iMinuteSpawnMinion	= iMinute;
	return TRUE;
}

BOOL CFuryArenaHandler::StartEvent()
{
	// Is Disabled? no event
	if ( !IsEnabled() )
		return FALSE;

	if ( !bStarted )
	{
		SYSTEMTIME * psServerTime = GetServerTime();

		// Spawn Monster
		pcUnitDataBoss = MAPSERVER->SpawnMonster( szBossName[iEventIDNow], MAPID_CursedTempleF2, Point3D( _X_KINGFURY, _Y_KINGFURY, _Z_KINGFURY ) );
		if ( pcUnitDataBoss )
		{
			pcUnitDataBoss->dwLastUpdateTime = TICKCOUNT + ((60 * 60 * 2) * 1000);
			pcUnitDataBoss->sCharacterData.bExclusiveBoss = TRUE;
			pcUnitDataBoss->sCharacterData.bGrandFuryBoss = TRUE;

			wDayEvent = psServerTime->wDay;
			wMinuteMinionSpawn = psServerTime->wMinute;
			iHourEvent = psServerTime->wHour;

			// Time solve
			if ( (short)wMinuteMinionSpawn < 0 )
				wMinuteMinionSpawn = 60 - (WORD)abs( wMinuteMinionSpawn );

			// Generate a random spawn
			int iRnd = RandomI( 0, 5 );

			// Spawn Minions
			for ( int i = 0; i < _MINION_FURYARENA_CNT; i++ )
			{
				// Not Dead? go to next minion
				if ( pcUnitDataMinions[i] && !pcUnitDataMinions[i]->IsDead() )
					continue;

				// Spawn
				pcUnitDataMinions[i] = MAPSERVER->SpawnMonster( szMinionName[iEventIDNow], MAPID_CursedTempleF2, Point3D( MinionsFuryArena[i].iX, _Y_KINGFURY + 3000, MinionsFuryArena[i].iZ ) );
			}

			// Send start to all
			PacketFuryArenaRequest sPacket;
			sPacket.iLength = sizeof( PacketFuryArenaRequest );
			sPacket.iHeader = PKTHDR_FuryArena;
			sPacket.eState = FURYARENASTATEID_Status;
			sPacket.p1 = TRUE;

			// Start at all
			for ( int i = 0; i < PLAYERS_MAX; i++ )
			{
				if ( USERSDATA[i].pcSocketData )
				{
					User * pcUser = USERDATATOUSER( &USERSDATA[i] );

					if ( pcUser->bInGrandFury && InsideArena( &pcUser->pcUserData->sPosition ) )
					{
						SENDPACKET( pcUser, &sPacket, TRUE );
					}
				}
			}


			// Notify Users about event
			CHATSERVER->SendChatAll( CHATCOLOR_Global, "BossArena> Boss Arena Started!" );

			dwStarted = TICKCOUNT;

			dwMeteor = 0;
		}

		// Event Started
		bStarted = TRUE;

		return TRUE;
	}

	return FALSE;
}

BOOL CFuryArenaHandler::EndEvent()
{
	// Is Disabled? no event
	if ( !IsEnabled() )
		return FALSE;

	if ( bStarted )
	{
		// Set Vars
		bAlert1Start		= FALSE;
		bAlert2Start		= FALSE;
		bAlert3Start		= FALSE;
		bAlert4Start		= FALSE;
		wMinuteMinionSpawn	= 0;

		// Leave Time
		dwEventLeaveTime	= TICKCOUNT + (20 * 1000);

		// Notify users from event end
		CHATSERVER->SendChatAll( CHATCOLOR_Global, "BossArena> Boss Arena Ended!" );

		// Kill All Units in event
		KillUnits();

		PacketFuryArenaRequest sPacket;
		sPacket.iLength = sizeof( PacketFuryArenaRequest );
		sPacket.iHeader = PKTHDR_FuryArena;
		sPacket.eState	= FURYARENASTATEID_Status;
		sPacket.p1		= FALSE;

		// End at all
		for ( int i = 0; i < PLAYERS_MAX; i++ )
		{
			if ( USERSDATA[i].pcSocketData )
			{
				User * pcUser = USERDATATOUSER( &USERSDATA[i] );

				if ( pcUser->bInGrandFury && InsideArena( &pcUser->pcUserData->sPosition ) )
				{
					SENDPACKET( pcUser, &sPacket, TRUE );
				}
			}
		}

		// Event Ended
		bStarted			= FALSE;

		return TRUE;
	}

	return FALSE;
}

BOOL CFuryArenaHandler::EnterFromArena( User * pcUser )
{

	// Is Already entered? no need again
	if ( InsideArena( &pcUser->pcUserData->sPosition ) )
		return FALSE;

	//Hardware ID
	UINT uUsersInGame = USERSERVER->uUsersInGame;
	User ** pcaUserInGame = USERSERVER->pcaUserInGame;
	int iCount = 0;
	for ( UINT i = 0; i < uUsersInGame; i++ )
	{
		User * pc = pcaUserInGame[i];
		if ( (pcUser != pc) && pc->bInGrandFury )
		{
			if ( pc->szHardwareID[0] != 0 )
			{
				if ( STRINGCOMPARE( pc->szHardwareID, pcUser->szHardwareID ) )
					iCount++;

				if ( iCount >= 2000 )
					break;
			}
		}
	}

	if ( iCount >= 2000 )
	{
		CHATSERVER->SendChatMessageBox( pcUser, "You already entered the Arena with 4 accounts!" );
		CHATSERVER->SendChat( pcUser, CHATCOLOR_Error, "> You already entered the Arena with 4 accounts!" );
		return FALSE;
	}

	// Teleport
	CHARACTERSERVER->SetPositionWarp( pcUser->pcUserData, _X_KINGFURY + (rand() % (4096 * 2)), _Z_KINGFURY + (rand() % (4096 * 2)), MAPID_CursedTempleF2 );

	// Entered from Arena
	CHATSERVER->SendChatAllUsersInRange( _X_KINGFURY, _Z_KINGFURY, 0x12C000, CHATCOLOR_Global, "BossArena> %s has entered the Arena", CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );

	// Success
	pcUser->bInGrandFury = TRUE;
	pcUser->dwGrandFuryTickEnter = TICKCOUNT + (15 * 1000);

	// Packet Success
	PacketFuryArenaRequest sPacket;
	sPacket.iLength		= sizeof( PacketFuryArenaRequest );
	sPacket.iHeader		= PKTHDR_FuryArena;
	sPacket.eState		= FURYARENASTATEID_Enter;
	SENDPACKET( pcUser, &sPacket );

	sPacket.eState	= FURYARENASTATEID_Status;
	sPacket.p1		= bStarted;
	SENDPACKET( pcUser, &sPacket );
	return TRUE;
}

BOOL CFuryArenaHandler::LeaveFromArena()
{
	for ( int i = 0; i < PLAYERS_MAX; i++ )
	{
		// Alive and in the map?
		if ( USERSDATA[i].pcSocketData && USERSDATA[i].iMapID == MAPID_CursedTempleF2 )
		{
			User * pcUser = USERDATATOUSER( &USERSDATA[i] );

			// Inside Arena?
			if ( InsideArena( &pcUser->pcUserData->sPosition ) || pcUser->bInGrandFury )
			{

				// Leave
				CHARACTERSERVER->SetPositionWarp( pcUser->pcUserData, _X_LEAVEFURYARENA, _Z_LEAVEFURYARENA, MAPID_CursedTempleF2 );
				pcUser->bInGrandFury			= FALSE;
				pcUser->dwGrandFuryTickEnter	= 0;
			}
		}
	}

	// All leaved, reset timer
	dwEventLeaveTime = 0;
	return TRUE;
}

BOOL CFuryArenaHandler::LeaveFromArena( User * pcUser )
{
	if ( pcUser )
	{
		// Teleport Leave
		CHARACTERSERVER->SetPositionWarp( pcUser->pcUserData, _X_LEAVEFURYARENA, _Z_LEAVEFURYARENA, MAPID_CursedTempleF2 );
		
		// Set State
		pcUser->bInGrandFury = FALSE;
		pcUser->dwGrandFuryTickEnter = 0;

		// Send Arena Status to user
		PacketFuryArenaRequest sPacket;
		sPacket.iLength = sizeof( PacketFuryArenaRequest );
		sPacket.iHeader = PKTHDR_FuryArena;
		sPacket.eState	= FURYARENASTATEID_Status;
		sPacket.p1		= FALSE;
		SENDPACKET( pcUser, &sPacket, TRUE );

		// Notify users
		CHATSERVER->SendChatAllUsersInRange( _X_KINGFURY, _Z_KINGFURY, 0x12C000, CHATCOLOR_Global, "BossArena> %s left the Arena (Kicked)", CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );

		return TRUE;
	}

	return FALSE;
}

BOOL CFuryArenaHandler::OnDisconnected( User * pcUser )
{
	CHATSERVER->SendChatAllUsersInRange( _X_KINGFURY, _Z_KINGFURY, 0x12C000, CHATCOLOR_Global, "BossArena> %s left the Arena (DC)", CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );
	pcUser->bInGrandFury			= FALSE;
	pcUser->dwGrandFuryTickEnter	= 0;
	return TRUE;
}

BOOL CFuryArenaHandler::OnUnitKilled( UserData * pcUserData, UnitData * pcUnitData )
{
	// Is Disabled? no event
	if ( !IsEnabled() )
		return FALSE;
	
	// Not Boss?
	if ( !STRINGCOMPARE( szBossName[iEventIDNow], pcUnitData->GetName() ) )
	{
		// Inside arena?
		if ( InsideArena( &pcUnitData->sPosition ) )
		{

			// Minion?
			for ( int i = 0; i < _MINION_FURYARENA_CNT; i++ )
			{
				// Kill him
				if ( pcUnitData == pcUnitDataMinions[i] )
				{
					pcUnitDataMinions[i] = NULL;
					break;
				}
			}
			return FALSE;
		}
	}
	// Is on Arena?
	if ( !InsideArena( &pcUnitData->sPosition ) )
		return FALSE;

	// Set End Event
	EndEvent();

	DefinitionItem * psDef = ITEMSERVER->FindItemDefByCode( ITEMID_KingStone );
	if ( psDef )
	{
		// Get Minutes killed TGF Boss
		int iMinute = (((TICKCOUNT - dwStarted) / 1000) / 60) % 60;
		if ( iMinute == 0 )
			iMinute = 1;

		// Create King Stone with monster tag
		Item sItem;
		ITEMSERVER->CreateItem( &sItem, psDef );
		sItem.eMixTypeName		= (EMixTypeName)(iEventIDNow == 1 ? ITEMCRAFTTYPE_TGFLordBabel : ITEMCRAFTTYPE_TGFKingFury);
		sItem.sQuestMonsters.sCur = iMinute;

		// Handle Stone
		for ( int i = 0; i < PLAYERS_MAX; i++ )
		{
			// Alive and in the map?
			if ( USERSDATA[i].pcSocketData && USERSDATA[i].iMapID == MAPID_CursedTempleF2 )
			{
				User * pcUser = USERDATATOUSER( &USERSDATA[i] );

				// Inside Arena?
				if ( InsideArena( &pcUser->pcUserData->sPosition ) )
				{
					// Is in event?
					if ( pcUser->bInGrandFury )
					{
						if( pcUserData != pcUser->pcUserData )
						{
							UNITSERVER->ProcessPacketKillUnitID( pcUser, pcUnitData->iID, MAPID_CursedTempleF2 );
						}

						// Gained The Stone
						ITEMSERVER->SendItemData( pcUser->pcUserData, &sItem );
						CHATSERVER->SendChatMessageBox( pcUser, "You received the King Stone" );
						SQLLog( "AccountName[%s] CharacterName[%s] Received King Stone [%d]", pcUser->pcUserData->szAccountName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), iEventIDNow == 1 ? ITEMCRAFTTYPE_TGFLordBabel : ITEMCRAFTTYPE_TGFKingFury );
					}
				}
			}
		}
	}

	// Spawn Wanted Wolf
	MAPSERVER->SpawnMonster( szMonsterEventName[iEventIDNow], MAPID_CursedTempleF2, Point3D( _X_KINGFURY, _Y_KINGFURY, _Z_KINGFURY ) );

	return TRUE;
}

void CFuryArenaHandler::PacketHandlerData( User * pcUser, PacketFuryArenaRequest * psPacket )
{
	EFuryArenaState eState = psPacket->eState;

	DWORD dw = (DWORD)GetTimeLeft( GetServerTime() );

	switch ( eState )
	{
		// Open NPC, request time
		case FURYARENASTATEID_OpenNPC:
			if ( LOGIN_SERVER && (GAME_SERVER == FALSE) )
				break;

			if ( SERVER_CODE == 1 )
			{

				psPacket->dwTimeLeft = (DWORD)GetTimeLeft( GetServerTime() );
				psPacket->iEventID = GetEventID();
				SENDPACKET( pcUser, psPacket, TRUE );
			}
			else
			{
				CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "BossArena> Boss Arena is closed on this server!" );
			}
			break;
	
		// Request Teleport
		case FURYARENASTATEID_TeleportCT2:
			if ( GAME_SERVER )
				break;

			if ( psPacket->p1 > 0 )
			{
				CHARACTERSERVER->GiveGOLD( pcUser, -(psPacket->p1 * 2000), EWhereAction::WHEREID_FuryArena );
				CHARACTERSERVER->SetPositionWarp( pcUser->pcUserData, _X_LEAVEFURYARENA + (rand() % 4096), _Z_LEAVEFURYARENA + (rand() % 4096), MAPID_CursedTempleF2 );
			}
			break;

		// Request state
		case FURYARENASTATEID_RequestEnterArena:
			if ( LOGIN_SERVER && (GAME_SERVER == FALSE) )
				break;
			// Started?
			if ( bStarted )
			{
				// Notify user
				CHATSERVER->SendChatMessageBox( pcUser, "Event was started" );
				break;
			}

			// Time to enter on arena
			if ( dw < 30 || dw > (15*60) )
			{
				CHATSERVER->SendChatMessageBox( pcUser, "You cannot enter at the arena" );
				break;
			}
			// Level <= 100?
			if ( iEventID == 1 )
			{
				if ( pcUser->pcUserData->GetLevel() < 110 )
				{
					// Hack, can't enter at arena
					CHATSERVER->SendChatMessageBox( pcUser, "You must be level 110 or above" );
					break;
				}
			}
			else if ( pcUser->pcUserData->GetLevel() < 130 )
			{
				// Hack, can't enter at arena
				CHATSERVER->SendChatMessageBox( pcUser, "You must be level 130 or above" );
				break;
			}

			// Success!
			EnterFromArena( pcUser );
			break;

		case FURYARENASTATEID_Exchange:
			// Items need to be on Login Server
			if ( GAME_SERVER )
				break;

			// Exchange Item
			if ( !ExchangeItem(pcUser, psPacket) )
				CHATSERVER->SendChatMessageBox( pcUser, "Error: Cannot receive reward" );

			break;
		
		default:
			break;
	}
}

void CFuryArenaHandler::Meteorite()
{
	if ( !dwMeteor )
	{
		dwMeteor = TICKCOUNT + 10000;
	}

	
	// Meteor Tick
	if ( dwMeteor < TICKCOUNT )
	{
		// Structure Packet to fire meteor effect
		PacketFireMeteorite sPacket;
		sPacket.iLength = sizeof( PacketFireMeteorite );
		sPacket.iHeader = PKTHDR_FireMeteorite;
		sPacket.bSmallMeteor = FALSE;
		sPacket.iDelay = RandomI( 50, 251 );
		sPacket.iCount = RandomI( 1, 3 );

		// Structure Packet to Quick Damage
		PacketDamageQuick sPDM;
		sPDM.iLength		= sizeof( PacketDamageQuick );
		sPDM.iHeader		= PKTHDR_DamageQuick;
		sPDM.bPercentile	= FALSE;
		sPDM.bBaseCurrentHP = FALSE;
		sPDM.iDamage		= iEventIDNow == 0 ? RandomI( 135, 150 ) : RandomI( 55, 70 );

		Point3D saPosition[12] =
		{
			{ -925752, 124539, -11147648 },
			{ -876745, 124539, -11133500 },
			{ -841600, 124539, -11094106 },
			{ -829474, 124539, -11045315 },
			{ -845359, 124539, -11000365 },
			{ -880143, 124539, -10960888 },
			{ -928917, 124539, -10949324 },
			{ -977675, 124539, -10965116 },
			{ -1014652, 124539, -11002253 },
			{ -1026306, 124539, -11052156 },
			{ -1013365, 124539, -11101367 },
			{ -975908, 124539, -11134549 },
		};


		// Meteors on same position of minions
		for ( int i = 0; i < 12; i++ )
		{
			// Set Meteor Position and send effect to users on range
			sPacket.sBegin.iX	= saPosition[i].iX - 8092;
			sPacket.sBegin.iY	= saPosition[i].iY + 150000;
			sPacket.sBegin.iZ	= saPosition[i].iZ - 8092;
			sPacket.sEnd.iX		= saPosition[i].iX + 8092;
			sPacket.sEnd.iY		= saPosition[i].iY;
			sPacket.sEnd.iZ		= saPosition[i].iZ + 8092;
			SENDPACKETRANGE( &sPacket, (Point3D*)&saPosition[i], DISTANCE_UNIT_FAR );
		
			// Send Quick Damage Packet at users on range
			SENDPACKETRANGE( &sPDM, (Point3D*)&saPosition[i], (250 * 27) );
		}
		// Tick Next
		dwMeteor = TICKCOUNT + 3000;
	}
}

void CFuryArenaHandler::UpdateHPBoss()
{
	PacketFuryArenaBossData s;
	s.iLength				= sizeof( PacketFuryArenaBossData );
	s.iHeader				= PKTHDR_FuryArenaBossData;

	// Convert to real HP
	int iPercent			= pcUnitDataBoss->iHPBlocks != 0 ? pcUnitDataBoss->sHPBlocks.sCur : 0;
	int iTotal				= pcUnitDataBoss->iHPBlocks;
	s.sHP.iCurrentBossHP	= ((int)pcUnitDataBoss->sCharacterData.sHP.sMax * iPercent) + pcUnitDataBoss->sCharacterData.sHP.sCur;
	s.sHP.iMaxBossHP		= ((int)pcUnitDataBoss->sCharacterData.sHP.sMax * iTotal) + pcUnitDataBoss->sCharacterData.sHP.sMax;

	// Handle HP
	for ( int i = 0; i < PLAYERS_MAX; i++ )
	{
		// Alive and in the map?
		if ( USERSDATA[i].pcSocketData && USERSDATA[i].iMapID == MAPID_CursedTempleF2 )
		{
			User * pcUser = USERDATATOUSER( &USERSDATA[i] );

			// Inside Arena?
			if ( InsideArena( &pcUser->pcUserData->sPosition ) )
			{
				// Is in event?
				if ( pcUser->bInGrandFury )
				{
					SENDPACKET( pcUser, &s, TRUE );
				}
			}
		}
	}
}

void CFuryArenaHandler::Tick()
{
	// Is Disabled? no event
	if ( !IsEnabled() )
		return;

	// Not GameServer? no event
	if ( LOGIN_SERVER && (GAME_SERVER == FALSE) )
		return;

	// Not Midgard? no event
	if ( SERVER_CODE != 1 )
		return;

	SYSTEMTIME * psServerTime = GetServerTime();

	if ( dwTickTime == 0 )
		dwTickTime = (DWORD)GetTimeLeft( psServerTime );

	dwTickTime -= 1;

	if ( !bStarted )
	{
		if ( iEventID < 2 )
		{
			if ( !bAlert1Start && dwTickTime == (30 * 60) )
			{
				// Notify Users about event
				CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BossArena> %s will start in 30 minutes!", szBossName[iEventID] );
				bAlert1Start = TRUE;
			}
			if ( !bAlert2Start && dwTickTime == (15 * 60) )
			{
				// Notify Users about event
				CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BossArena> %s will start in 15 minutes!", szBossName[iEventID] );
				bAlert2Start = TRUE;
				bCanEnter = TRUE;
			}

			if ( !bAlert3Start && dwTickTime == (10 * 60) )
			{
				// Notify Users about event
				CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BossArena> %s will start in 10 minutes!", szBossName[iEventID] );
				bAlert3Start = TRUE;
			}

			if ( !bAlert4Start && dwTickTime == (5 * 60) )
			{
				// Notify Users about event
				CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BossArena> %s will start in 5 minutes!", szBossName[iEventID] );
				bAlert4Start = TRUE;
			}
		}
		
		if ( dwTickTime > 15 && dwTickTime <= (30 * 60) )
		{
			iEventIDNow = iEventID;
		}

		if ( dwTickTime == 0 )
		{
			//Start event
			StartEvent();
		}

		if ( (dwTickTime % ((30 * 60) + 1)) == 0 )
			dwTickTime = (DWORD)GetTimeLeft( psServerTime );
	}

	if ( bStarted )
	{
		Meteorite();

		UpdateHPBoss();

		WORD wMinute = wMinuteMinionSpawn + (WORD)iMinuteSpawnMinion;

		// Next Hour? solve minutes
		if ( wMinute >= 60 )
			wMinute -= 60;

		// Time to spawn minions?
		if ( wMinute == psServerTime->wMinute )
		{		
			// Spawn Minions
			for ( int i = 0; i < _MINION_FURYARENA_CNT; i++ )
			{
				// Not Dead? go to next minion
				if ( pcUnitDataMinions[i] && !pcUnitDataMinions[i]->IsDead() )
					continue;

				// Spawn
				pcUnitDataMinions[i] = MAPSERVER->SpawnMonster( szMinionName[iEventIDNow], MAPID_CursedTempleF2, Point3D( MinionsFuryArena[i].iX, _Y_KINGFURY + 3000, MinionsFuryArena[i].iZ ) );
			}

			// Set time spawned
			wMinuteMinionSpawn = psServerTime->wMinute;
		}

		int iHour = iHourEvent + 2;
		if ( iHour >= 24 )
			iHour -= 24;

		// Is after 2 Hours?
		if ( psServerTime->wHour == (WORD)iHour )
		{
			// End event
			EndEvent();
		}

		// Boss was killed extern? end event
		if ( pcUnitDataBoss && (pcUnitDataBoss->bActive == FALSE || pcUnitDataBoss->IsDead()) )
			EndEvent();

		if ( GetTotalUsers() == 0 )
			EndEvent();
	}
	// Check Leave from Arena
	for ( int i = 0; i < PLAYERS_MAX; i++ )
	{
		if ( USERSDATA[i].pcSocketData )
		{
			User * pcUser = USERDATATOUSER( &USERSDATA[i] );

			if ( pcUser->bInGrandFury && pcUser->dwGrandFuryTickEnter < TICKCOUNT )
			{
				// Not Inside Arena?
				if ( !InsideArena( &pcUser->pcUserData->sPosition ) )
				{
					PacketFuryArenaRequest sPacket;
					sPacket.iLength = sizeof( PacketFuryArenaRequest );
					sPacket.iHeader = PKTHDR_FuryArena;
					sPacket.eState	= FURYARENASTATEID_Status;
					sPacket.p1		= FALSE;
					SENDPACKET( pcUser, &sPacket, TRUE );

					// Leave from Arena
					CHATSERVER->SendChatAllUsersInRange( _X_KINGFURY, _Z_KINGFURY, 0x12C000, CHATCOLOR_Global, "BossArena> %s left the Arena", CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );
					pcUser->bInGrandFury = FALSE;
					pcUser->dwGrandFuryTickEnter = 0;
				}

				if ( USERSDATA[i].IsDead() )
				{
					// Leave from Arena ( Killed )
					CHATSERVER->SendChatAllUsersInRange( _X_KINGFURY, _Z_KINGFURY, 0x12C000, CHATCOLOR_Global, "BossArena> %s left the Arena (Killed)", CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );
						
					PacketFuryArenaRequest sPacket;
					sPacket.iLength = sizeof( PacketFuryArenaRequest );
					sPacket.iHeader = PKTHDR_FuryArena;
					sPacket.eState	= FURYARENASTATEID_Status;
					sPacket.p1		= FALSE;
					SENDPACKET( pcUser, &sPacket, TRUE );

					// Teleport Leave
					CHARACTERSERVER->SetPositionWarp( pcUser->pcUserData, _X_LEAVEFURYARENA, _Z_LEAVEFURYARENA, MAPID_CursedTempleF2 );
		
					// Set State
					pcUser->bInGrandFury = FALSE;
					pcUser->dwGrandFuryTickEnter = 0;
				}
			}
		}
	}

	// Leave Time?
	if ( dwEventLeaveTime != 0 && dwEventLeaveTime <= TICKCOUNT )
	{
		// Leave users from arena
		LeaveFromArena();
	}
}

void CFuryArenaHandler::Init()
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
		
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT ItemCode, [Percent], BossID FROM FuryArena ORDER BY ID ASC" ) )
		{
			pcDB->Execute();

			while ( pcDB->Fetch() )
			{
				SPrizeData * ps = new SPrizeData;

				pcDB->GetData( 1, PARAMTYPE_String, ps->szCode, 32 );
				pcDB->GetData( 2, PARAMTYPE_Integer, &ps->iPercent );
				pcDB->GetData( 3, PARAMTYPE_Integer, &ps->iQuantity ); // BossID

				iaTotalPercent[ps->iQuantity] += ps->iPercent;

				ps->bActive = TRUE;

				vaPrizesReward[ps->iQuantity].push_back( ps );
			}
		}
		pcDB->Close();
	}

	dwTickTime = (DWORD)GetTimeLeft( GetServerTime() );

	SetEnable( THEGRANDFURY_FLAG );
}
