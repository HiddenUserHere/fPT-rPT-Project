#include "stdafx.h"
#include "blesscastleserver.h"


BlessCastleServer::BlessCastleServer()
{
	psBlessCastlePVPLeague	= (BlessCastlePVPLeague*)0x07AB14FC;
	psBlessCastleStatusData = (BlessCastleStatusData*)0x07AB1EE8;
}


BlessCastleServer::~BlessCastleServer()
{
}

void BlessCastleServer::NetSendBlessCastleStatus( NetConnection * psNetConnection )
{
	if ( LOGIN_SERVER && GAME_SERVER )
		return;

	BlessCastleStatusShort sBlessCastleStatusShort = { 0 };

	sBlessCastleStatusShort.iClanID			= psBlessCastleStatusData->iClanID;
	sBlessCastleStatusShort.iTax				= psBlessCastleStatusData->iTax;
	sBlessCastleStatusShort.iSkill				= (int)psBlessCastleStatusData->iSkill;
	sBlessCastleStatusShort.iaTowerType[0]		= (int)(psBlessCastleStatusData->sSettings.saTowerType[0]);
	sBlessCastleStatusShort.iaTowerType[1]		= (int)(psBlessCastleStatusData->sSettings.saTowerType[1]);
	sBlessCastleStatusShort.iaTowerType[2]		= (int)(psBlessCastleStatusData->sSettings.saTowerType[2]);
	sBlessCastleStatusShort.iaTowerType[3]		= (int)(psBlessCastleStatusData->sSettings.saTowerType[3]);
	sBlessCastleStatusShort.iaTowerType[4]		= (int)(psBlessCastleStatusData->sSettings.saTowerType[4]);
	sBlessCastleStatusShort.iaTowerType[5]		= (int)(psBlessCastleStatusData->sSettings.saTowerType[5]);
	sBlessCastleStatusShort.iaGuardAmount[0]	= (int)(psBlessCastleStatusData->sSettings.caGuardAmount[0]);
	sBlessCastleStatusShort.iaGuardAmount[1]	= (int)(psBlessCastleStatusData->sSettings.caGuardAmount[1]);
	sBlessCastleStatusShort.iaGuardAmount[2]	= (int)(psBlessCastleStatusData->sSettings.caGuardAmount[2]);

	PacketNetBCStatus sPacketNet;
	sPacketNet.iLength		= sizeof( PacketNetBCStatus );
	sPacketNet.iHeader		= PKTHDR_NetBCStatusSnd;
	CopyMemory( &sPacketNet.sBlessCastleStatus, &sBlessCastleStatusShort, sizeof( sPacketNet.sBlessCastleStatus ) );

	if( psNetConnection == NULL )
		NETSERVER->SendPacketGameServers( &sPacketNet );
	else
		NETSERVER->SendPacket( psNetConnection->pcSocketData, &sPacketNet );
}


BOOL BlessCastleServer::SQLWriteBlessCastleStatus()
{
	if( LOGIN_SERVER )
	{
		BlessCastleStatusShort sBlessCastleStatusShort = { 0 };

		sBlessCastleStatusShort.iClanID			= psBlessCastleStatusData->iClanID;
		sBlessCastleStatusShort.iTax				= psBlessCastleStatusData->iTax;
		sBlessCastleStatusShort.iSkill				= (int)psBlessCastleStatusData->iSkill;
		sBlessCastleStatusShort.iaTowerType[0]		= (int)(psBlessCastleStatusData->sSettings.saTowerType[0]);
		sBlessCastleStatusShort.iaTowerType[1]		= (int)(psBlessCastleStatusData->sSettings.saTowerType[1]);
		sBlessCastleStatusShort.iaTowerType[2]		= (int)(psBlessCastleStatusData->sSettings.saTowerType[2]);
		sBlessCastleStatusShort.iaTowerType[3]		= (int)(psBlessCastleStatusData->sSettings.saTowerType[3]);
		sBlessCastleStatusShort.iaTowerType[4]		= (int)(psBlessCastleStatusData->sSettings.saTowerType[4]);
		sBlessCastleStatusShort.iaTowerType[5]		= (int)(psBlessCastleStatusData->sSettings.saTowerType[5]);
		sBlessCastleStatusShort.iaGuardAmount[0]	= (int)(psBlessCastleStatusData->sSettings.caGuardAmount[0]);
		sBlessCastleStatusShort.iaGuardAmount[1]	= (int)(psBlessCastleStatusData->sSettings.caGuardAmount[1]);
		sBlessCastleStatusShort.iaGuardAmount[2]	= (int)(psBlessCastleStatusData->sSettings.caGuardAmount[2]);

		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );

		if( pcDB->Open() )
		{
			if( pcDB->Prepare( "UPDATE BlessCastleSettings SET ClanID=?,Tax=?,Skill=?,Tower1Type=?,Tower2Type=?,Tower3Type=?,Tower4Type=?,Tower5Type=?,Tower6Type=?,Guard1Amount=?,Guard2Amount=?,Guard3Amount=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &sBlessCastleStatusShort.iClanID );
				pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &sBlessCastleStatusShort.iTax );
				pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &sBlessCastleStatusShort.iSkill );
				pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &sBlessCastleStatusShort.iaTowerType[0] );
				pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &sBlessCastleStatusShort.iaTowerType[1] );
				pcDB->BindParameterInput( 6, PARAMTYPE_Integer, &sBlessCastleStatusShort.iaTowerType[2] );
				pcDB->BindParameterInput( 7, PARAMTYPE_Integer, &sBlessCastleStatusShort.iaTowerType[3] );
				pcDB->BindParameterInput( 8, PARAMTYPE_Integer, &sBlessCastleStatusShort.iaTowerType[4] );
				pcDB->BindParameterInput( 9, PARAMTYPE_Integer, &sBlessCastleStatusShort.iaTowerType[5] );
				pcDB->BindParameterInput( 10, PARAMTYPE_Integer, &sBlessCastleStatusShort.iaGuardAmount[0] );
				pcDB->BindParameterInput( 11, PARAMTYPE_Integer, &sBlessCastleStatusShort.iaGuardAmount[1] );
				pcDB->BindParameterInput( 12, PARAMTYPE_Integer, &sBlessCastleStatusShort.iaGuardAmount[2] );

				pcDB->Execute();
			}

			pcDB->Close();
		}

		NetSendBlessCastleStatus();
	}

	return TRUE;
}


void BlessCastleServer::SQLReadBlessCastleStatus()
{
	if( LOGIN_SERVER )
	{
		BlessCastleStatusShort sBlessCastleStatusShort = { 0 };

		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );

		if( pcDB->Open() )
		{
			if( pcDB->Prepare( "SELECT ClanID,Tax,Skill,Tower1Type,Tower2Type,Tower3Type,Tower4Type,Tower5Type,Tower6Type,Guard1Amount,Guard2Amount,Guard3Amount FROM BlessCastleSettings" ) )
			{
				if( pcDB->Execute() )
				{
					if( pcDB->Fetch() )
					{
						pcDB->GetData( 1, PARAMTYPE_Integer, &sBlessCastleStatusShort.iClanID, 0 );
						pcDB->GetData( 2, PARAMTYPE_Integer, &sBlessCastleStatusShort.iTax, 0 );
						pcDB->GetData( 3, PARAMTYPE_Integer, &sBlessCastleStatusShort.iSkill, 0 );
						pcDB->GetData( 4, PARAMTYPE_Integer, &sBlessCastleStatusShort.iaTowerType[0], 0 );
						pcDB->GetData( 5, PARAMTYPE_Integer, &sBlessCastleStatusShort.iaTowerType[1], 0 );
						pcDB->GetData( 6, PARAMTYPE_Integer, &sBlessCastleStatusShort.iaTowerType[2], 0 );
						pcDB->GetData( 7, PARAMTYPE_Integer, &sBlessCastleStatusShort.iaTowerType[3], 0 );
						pcDB->GetData( 8, PARAMTYPE_Integer, &sBlessCastleStatusShort.iaTowerType[4], 0 );
						pcDB->GetData( 9, PARAMTYPE_Integer, &sBlessCastleStatusShort.iaTowerType[5], 0 );
						pcDB->GetData( 10, PARAMTYPE_Integer, &sBlessCastleStatusShort.iaGuardAmount[0], 0 );
						pcDB->GetData( 11, PARAMTYPE_Integer, &sBlessCastleStatusShort.iaGuardAmount[1], 0 );
						pcDB->GetData( 12, PARAMTYPE_Integer, &sBlessCastleStatusShort.iaGuardAmount[2], 0 );
					}
				}
			}

			pcDB->Close();
		}

		psBlessCastleStatusData->iClanID					= sBlessCastleStatusShort.iClanID;
		psBlessCastleStatusData->iTax						= sBlessCastleStatusShort.iTax;
		psBlessCastleStatusData->iTaxNow					= sBlessCastleStatusShort.iTax;
		psBlessCastleStatusData->iSkill						= sBlessCastleStatusShort.iSkill;
		psBlessCastleStatusData->sSettings.iTax				= sBlessCastleStatusShort.iTax;
		psBlessCastleStatusData->sSettings.saTowerType[0]	= (short)(sBlessCastleStatusShort.iaTowerType[0]);
		psBlessCastleStatusData->sSettings.saTowerType[1]	= (short)(sBlessCastleStatusShort.iaTowerType[1]);
		psBlessCastleStatusData->sSettings.saTowerType[2]	= (short)(sBlessCastleStatusShort.iaTowerType[2]);
		psBlessCastleStatusData->sSettings.saTowerType[3]	= (short)(sBlessCastleStatusShort.iaTowerType[3]);
		psBlessCastleStatusData->sSettings.saTowerType[4]	= (short)(sBlessCastleStatusShort.iaTowerType[4]);
		psBlessCastleStatusData->sSettings.saTowerType[5]	= (short)(sBlessCastleStatusShort.iaTowerType[5]);
		psBlessCastleStatusData->sSettings.caGuardAmount[0]	= (unsigned char)(sBlessCastleStatusShort.iaGuardAmount[0]);
		psBlessCastleStatusData->sSettings.caGuardAmount[1]	= (unsigned char)(sBlessCastleStatusShort.iaGuardAmount[1]);
		psBlessCastleStatusData->sSettings.caGuardAmount[2]	= (unsigned char)(sBlessCastleStatusShort.iaGuardAmount[2]);
		psBlessCastleStatusData->sSettings.iSkill			= sBlessCastleStatusShort.iSkill;
		NetSendBlessCastleStatus();
	}
	
	if ( GAME_SERVER )
	{
		Packet sPacket;
		sPacket.iLength		= sizeof( sPacket );
		sPacket.iHeader		= PKTHDR_NetBCStatusReq;

		NetConnection * nc = NETSERVER->GetLoginServerConnection();
		if ( nc )
			NETSERVER->SendPacket( nc->pcSocketData, &sPacket );
	}
}

void BlessCastleServer::SQLUpdateUserScore( User * pcUser )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE CharacterInfo SET [BlessCastleScore]=? WHERE Name=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iBlessCastleScore );
			pcDB->BindParameterInput( 2, PARAMTYPE_String, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );

			pcDB->Execute();
		}

		pcDB->Close();
	}
}

bool BlessCastleServer::HasCrystalsTowersAlive()
{
	if ( psBlessCastleStatusData )
	{
		for ( int i = 0; i < _countof( BlessCastleStatusData::baCrystalTowerCount ); i++ )
			if ( psBlessCastleStatusData->baCrystalTowerCount[i] > 0 )
				return true;
	}

	return false;
}

void BlessCastleServer::NetRecvBlessCastleStatus( PacketNetBCStatus * psPacket )
{
	BlessCastleStatusShort sBlessCastleStatusShort = { 0 };

	memcpy( &sBlessCastleStatusShort, &psPacket->sBlessCastleStatus, sizeof( sBlessCastleStatusShort ) );

	psBlessCastleStatusData->iClanID					= sBlessCastleStatusShort.iClanID;
	psBlessCastleStatusData->iTax						= sBlessCastleStatusShort.iTax;
	psBlessCastleStatusData->iTaxNow					= sBlessCastleStatusShort.iTax;
	psBlessCastleStatusData->iSkill						= sBlessCastleStatusShort.iSkill;
	psBlessCastleStatusData->sSettings.iTax				= sBlessCastleStatusShort.iTax;
	psBlessCastleStatusData->sSettings.saTowerType[0]	= (short)(sBlessCastleStatusShort.iaTowerType[0]);
	psBlessCastleStatusData->sSettings.saTowerType[1]	= (short)(sBlessCastleStatusShort.iaTowerType[1]);
	psBlessCastleStatusData->sSettings.saTowerType[2]	= (short)(sBlessCastleStatusShort.iaTowerType[2]);
	psBlessCastleStatusData->sSettings.saTowerType[3]	= (short)(sBlessCastleStatusShort.iaTowerType[3]);
	psBlessCastleStatusData->sSettings.saTowerType[4]	= (short)(sBlessCastleStatusShort.iaTowerType[4]);
	psBlessCastleStatusData->sSettings.saTowerType[5]	= (short)(sBlessCastleStatusShort.iaTowerType[5]);
	psBlessCastleStatusData->sSettings.caGuardAmount[0]	= (unsigned char)(sBlessCastleStatusShort.iaGuardAmount[0]);
	psBlessCastleStatusData->sSettings.caGuardAmount[1]	= (unsigned char)(sBlessCastleStatusShort.iaGuardAmount[1]);
	psBlessCastleStatusData->sSettings.caGuardAmount[2]	= (unsigned char)(sBlessCastleStatusShort.iaGuardAmount[2]);
	psBlessCastleStatusData->sSettings.iSkill			= sBlessCastleStatusShort.iSkill;

	if ( LOGIN_SERVER )
		SQLWriteBlessCastleStatus();
}

void BlessCastleServer::SetBlessCastleOwnerClanID( DWORD iClanID )
{
	psBlessCastleStatusData->iClanID = iClanID;

	SQLWriteBlessCastleStatus();
}

void BlessCastleServer::NetSendBlessCastleOwnerClanID( DWORD iClanID )
{
	if( GAME_SERVER )
	{
		PacketNetBCNewOwnerClanID sPacketNet;
		sPacketNet.iLength			= sizeof( sPacketNet );
		sPacketNet.iHeader			= PKTHDR_NetBCNewOwnerClanID;
		sPacketNet.dwNewClanID		= iClanID;

		NetConnection * nc = NETSERVER->GetLoginServerConnection();
		if ( nc )
			NETSERVER->SendPacket( nc->pcSocketData, &sPacketNet );
	}
}

void BlessCastleServer::UpdateIndividualScores( BOOL bReset )
{
	for ( int i = 0; i < 1024; i++ )
	{
		UserData * pc = &USERSDATA[i];
		User * pcUser = USERDATATOUSER( pc );

		if ( pc->pcSocketData && pc->iID && pc->iMapID == MAPID_BlessCastle && pcUser->iBlessCastleScore > 0 )
		{
			if ( bReset )
				pcUser->iBlessCastleScore = 0;

			SQLUpdateUserScore( pcUser );
		}
	}
}

void BlessCastleServer::HandleIndividualScore()
{
	char szAccountName[3][32] = { 0 };
	char szCharacterName[3][32] = { 0 };
	int iCharacterScore[3] = { 0 };
	char szGold[32] = { 0 };

	UpdateIndividualScores();

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT TOP 3 [AccountName],[Name],[BlessCastleScore] FROM CharacterInfo ORDER BY BlessCastleScore DESC" ) )
		{
			int i = 0;

			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_String, szAccountName[i], 32 );
					pcDB->GetData( 2, PARAMTYPE_String, szCharacterName[i], 32 );
					pcDB->GetData( 3, PARAMTYPE_Integer, &iCharacterScore[i] );

					i++;
				}
			}
		}

		pcDB->Close();
	
		//Conquest Mode?
		if ( (GetSiegeWarMode() == SIEGEWARMODE_TheConquest) || (GetSiegeWarMode() == SIEGEWARMODE_TheMassacre) )
		{
			CHATSERVER->SendChatAll( CHATCOLOR_Global, "BlessCastle> Player Top Scores:" );

			if ( szCharacterName[0] && iCharacterScore[0] > 0 )
			{
				STRINGCOPY( szGold, FormatNumber( 30000000 ) );
				CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> 1st Place: %s", szCharacterName[0] );
				CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> %s Points, %s gp!", FormatNumber( iCharacterScore[0] ), szGold );
				ITEMSERVER->AddItemOpenBox( szAccountName[0], "GG101", 0, 30000000 );
			}
			if ( szCharacterName[1] && iCharacterScore[1] > 0 )
			{
				STRINGCOPY( szGold, FormatNumber( 20000000 ) );
				CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> 2nd Place: %s", szCharacterName[1] );
				CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> %s Points, %s gp!", FormatNumber( iCharacterScore[1] ), szGold );
				ITEMSERVER->AddItemOpenBox( szAccountName[1], "GG101", 0, 20000000 );
			}
			if ( szCharacterName[2] && iCharacterScore[2] > 0 )
			{
				STRINGCOPY( szGold, FormatNumber( 10000000 ) );
				CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> 3rd Place: %s", szCharacterName[2] );
				CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> %s Points, %s gp!", FormatNumber( iCharacterScore[2] ), szGold );
				ITEMSERVER->AddItemOpenBox( szAccountName[2], "GG101", 0, 10000000 );
			}
		}
	}

	UpdateIndividualScores( TRUE );
}

void BlessCastleServer::SendIndividualScore()
{
	PacketBlessCastleTopIndividual s, s2;

	ZeroMemory( &s, sizeof( PacketBlessCastleTopIndividual ) );
	ZeroMemory( &s2, sizeof( PacketBlessCastleTopIndividual ) );

	s.iLength = sizeof( PacketBlessCastleTopIndividual );
	s.iHeader = PKTHDR_BlessCastleIndividualScore;
	s.iMode = GetSiegeWarMode();

	s2.iLength = sizeof( PacketBlessCastleTopIndividual );
	s2.iHeader = PKTHDR_BlessCastleIndividualScore;
	s2.iMode = GetSiegeWarMode();

	int i = 0;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT TOP 3 [Name],[JobCode],[BlessCastleScore] FROM CharacterInfo ORDER BY BlessCastleScore DESC" ) )
		{
			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_String, s.szCharacterName[i], 32 );
					pcDB->GetData( 2, PARAMTYPE_Integer, &s.iClass[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &s.iDamage[i] );

					i++;
				}
			}
		}

		pcDB->Close();
	}

	if ( i > 0 )
	{
		if ( CanShowScore() )
		{
			SENDPACKETSTAGE( &s, MAPID_BlessCastle );
		}
		else
		{
			for ( int i = 0; i < 1024; i++ )
			{
				UserData * pc = &USERSDATA[i];

				if ( pc->pcSocketData && pc->iID && pc->iMapID == MAPID_BlessCastle )
				{
					if ( pc->iGameLevel >= GAMELEVEL_Two )
						SENDPACKET( USERDATATOUSER(pc), &s );
					else
						SENDPACKET( USERDATATOUSER(pc), &s2 );
				}
			}
		}
	}
}

BOOL BlessCastleServer::Main()
{
	if ( LOGIN_SERVER && (GAME_SERVER == FALSE) )
		return TRUE;

	psBlessCastleStatusData->iCounter++;

	Map * pcMap = MAPSDATA + MAPID_BlessCastle;

	int iUpdate = 0;

	//Fountain Main...
	BlessCastleServer::MainFountain();

	if ( psBlessCastlePVPLeague->iMode )
	{
		if ( psBlessCastlePVPLeague->dwEndTime && (psBlessCastlePVPLeague->dwEndTime < TICKCOUNT) )
			psBlessCastlePVPLeague->iMode = 0;
		else
		{
			if ( psBlessCastleStatusData->iCastleMode == 0 )
			{
				if ( psBlessCastlePVPLeague->dwRestartTime && (psBlessCastlePVPLeague->dwRestartTime < TICKCOUNT) )
					BLESSCASTLESERVER->Start();
			}
		}
	}

	if ( psBlessCastleStatusData->iCastleMode == 0 )
	{
		psBlessCastleStatusData->saTimeSec[0] = 0;
		psBlessCastleStatusData->saTimeSec[1] = 0;
		return FALSE;
	}

	if ( psBlessCastleStatusData->iCastleMode == 3 )
	{
		if ( psBlessCastleStatusData->dwBattleOverTime < TICKCOUNT )
			EndBattle();

		return TRUE;
	}

	if ( (psBlessCastleStatusData->iCounter & 0x1F) == 0 )
	{
		psBlessCastleStatusData->saTimeSec[0] = (short)((TICKCOUNT - psBlessCastleStatusData->dwStartTime) / 1000);
		psBlessCastleStatusData->saTimeSec[1] = (short)((psBlessCastleStatusData->dwBattleOverTime - psBlessCastleStatusData->dwStartTime) / 1000);

		psBlessCastleStatusData->baSoldierCount[0] = 0;
		psBlessCastleStatusData->baSoldierCount[1] = 0;
		psBlessCastleStatusData->baSoldierCount[2] = 0;
		psBlessCastleStatusData->baSoldierCount[3] = 0;

		psBlessCastleStatusData->baCrystalTowerCount[0] = 0;
		psBlessCastleStatusData->baCrystalTowerCount[1] = 0;
		psBlessCastleStatusData->baCrystalTowerCount[2] = 0;
		psBlessCastleStatusData->baCrystalTowerCount[3] = 0;

		//Tick
		if ( dwTickUpdateUsersScore < TICKCOUNT )
		{
			UpdateIndividualScores();

			SendIndividualScore();

			dwTickUpdateUsersScore = TICKCOUNT + 1500;
		}

		BOOL bCastleDoor = FALSE;
		int iCrystalTowerCount = 0;

		for ( UINT i = 0; i < UNITSERVER->uUnitsInGame; i++ )
		{
			Unit * pcUnit = UNITSERVER->pcaUnitInGame[i];
			if ( pcUnit->pcUnitData->pcMap == pcMap )
			{
				UnitData * pc = pcUnit->pcUnitData;

				//BC Door Alive?
				if ( pc->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CASTLE_DOOR )
				{
					//Set
					bCastleDoor = TRUE;

					//Conquest Mode?
					if ( GetSiegeWarMode() == SIEGEWARMODE_TheConquest )
					{
						//Set Door to score on Siege War..
						if ( psBlessCastleStatusData->iDefenseLevel == 0 && psBlessCastleStatusData->psaDamageData )
						{
							if ( pc->psaSiegeWarDataList == NULL )
								pc->psaSiegeWarDataList = psBlessCastleStatusData->psaDamageData;
						}
					}

					*(UnitData **)0x08B64054 = pc;
				}
				else if ( pc->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CASTLE_CRYSTAL_N ||
						  pc->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CASTLE_CRYSTAL_R ||
						  pc->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CASTLE_CRYSTAL_G ||
						  pc->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CASTLE_CRYSTAL_B )
				{

					switch ( pc->sCharacterData.iMonsterEffectID )
					{
						case MONSTEREFFECTID_CASTLE_CRYSTAL_R:
							if ( psBlessCastleStatusData->baCrystalTowerCount[0] < 100 )
								psBlessCastleStatusData->baCrystalTowerCount[0]++;
							break;

						case MONSTEREFFECTID_CASTLE_CRYSTAL_G:
							if ( psBlessCastleStatusData->baCrystalTowerCount[1] < 100 )
								psBlessCastleStatusData->baCrystalTowerCount[1]++;
							break;

						case MONSTEREFFECTID_CASTLE_CRYSTAL_B:
							if ( psBlessCastleStatusData->baCrystalTowerCount[2] < 100 )
								psBlessCastleStatusData->baCrystalTowerCount[2]++;
							break;

						case MONSTEREFFECTID_CASTLE_CRYSTAL_N:
							if ( psBlessCastleStatusData->baCrystalTowerCount[3] < 100 )
								psBlessCastleStatusData->baCrystalTowerCount[3]++;
							break;

						default:
							break;
					}

					iCrystalTowerCount++;

					if ( psBlessCastleStatusData->iDefenseLevel == 0 && psBlessCastleStatusData->psaDamageData )
					{
						if ( pc->psaSiegeWarDataList == NULL )
							pc->psaSiegeWarDataList = psBlessCastleStatusData->psaDamageData;
					}
				}
				else if ( pc->sCharacterData.iOwnerID )
				{
					BOOL bSoldier = FALSE;

					switch ( pc->sCharacterData.iMonsterEffectID )
					{
						case MONSTEREFFECTID_CASTLE_SOLDER_A:
							if ( psBlessCastleStatusData->baSoldierCount[0] < 100 )
								psBlessCastleStatusData->baSoldierCount[0]++;

							bSoldier = TRUE;
							break;
						case MONSTEREFFECTID_CASTLE_SOLDER_B:
							if ( psBlessCastleStatusData->baSoldierCount[1] < 100 )
								psBlessCastleStatusData->baSoldierCount[1]++;

							bSoldier = TRUE;
							break;
						case MONSTEREFFECTID_CASTLE_SOLDER_C:
							if ( psBlessCastleStatusData->baSoldierCount[2] < 100 )
								psBlessCastleStatusData->baSoldierCount[2]++;

							bSoldier = TRUE;
							break;

						default:
							break;
					}

					if ( bSoldier )
					{
						//Conquest Mode?
						if ( GetSiegeWarMode() == SIEGEWARMODE_TheConquest )
						{
							//Set Soldiers to score on Siege War..
							if ( psBlessCastleStatusData->psaDamageData )
							{
								if ( pc->psaSiegeWarDataList == NULL )
									pc->psaSiegeWarDataList = psBlessCastleStatusData->psaDamageData;
							}
						}
					}
				}
				else if ( pc->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CASTLE_SOLDER_A ||
						  pc->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CASTLE_SOLDER_B ||
						  pc->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CASTLE_SOLDER_C )
				{
					//Conquest Mode?
					if ( GetSiegeWarMode() == SIEGEWARMODE_TheConquest )
					{
						//Set Soldiers to score on Siege War..
						if ( psBlessCastleStatusData->psaDamageData )
						{
							if ( pc->psaSiegeWarDataList == NULL )
								pc->psaSiegeWarDataList = psBlessCastleStatusData->psaDamageData;
						}
					}
				}

				pc->dwLastActiveTime = TICKCOUNT;
				pc->iAliveCounter = 256;
			}
		}

		//Check for Users inside castle
		if ( bCastleDoor && (GetSiegeWarMode() == SIEGEWARMODE_TheConquest) )
		{
			UINT uUsersInGame = USERSERVER->uUsersInGame;
			User ** pcaUserInGame = USERSERVER->pcaUserInGame;
			for ( UINT i = 0; i < uUsersInGame; i++ )
			{
				User * pcUser = pcaUserInGame[i];

				if ( pcUser->IsInGame() && (pcUser->GetMapID() == MAPID_BlessCastle) && (pcUser->GetGameLevel() < GAMELEVEL_Two) )
				{
					if ( pcUser->pcUserData->sPosition.iZ < ((-25580) << 8) )
					{
						CHARACTERSERVER->SetPositionWarp( pcUser->pcUserData, 8859282, -5931214, MAPID_BlessCastle );
					}
				}
			}
		}

		if ( (psBlessCastleStatusData->iDefenseLevel == 0) && (bCastleDoor == FALSE) )
			psBlessCastleStatusData->iDefenseLevel++;

		BOOL bFinished = FALSE;

		if ( pcBlessCastleTower == nullptr )
		{
			BlessCastleServer::End( FALSE );

			if ( psBlessCastleStatusData->sPacketTopData.iClanID[0] )
			{
				psBlessCastleStatusData->iClanID = BlessCastleServer::GetClanCodeByID( psBlessCastleStatusData->sPacketTopData.iClanID[0] );

				KillBuffs();

				ResetUserScore();

				BLESSCASTLESERVER->NetSendBlessCastleOwnerClanID( psBlessCastleStatusData->iClanID );

				iUpdate = 2;

				BLESSCASTLESERVER->SQLWriteBlessCastleStatus();

				bFinished = TRUE;
			}
		}
		else if ( psBlessCastleStatusData->dwBattleOverTime < TICKCOUNT )
		{
			//Change to new Mode = TRUE
			BOOL bUnitsAlive	= FALSE;

			//Conquest Mode? Units Alive
			if ( GetSiegeWarMode() == SIEGEWARMODE_TheConquest )
				bUnitsAlive		= TRUE;

			BlessCastleServer::End( bUnitsAlive );

			if ( psBlessCastleStatusData->sPacketTopData.iClanID[0] )
			{
				//Conquest Mode?
				if ( (GetSiegeWarMode() == SIEGEWARMODE_TheConquest) )
				{
					//No Winner
					psBlessCastleStatusData->iClanID = 0;
				}
				else if ( (GetSiegeWarMode() == SIEGEWARMODE_TheMassacre) )
				{
					//Otherwise, on Normal Mode...
					psBlessCastleStatusData->iClanID = BlessCastleServer::GetClanCodeByID( psBlessCastleStatusData->sPacketTopData.iClanID[0] );
				}

				KillBuffs();

				ResetUserScore();

				BLESSCASTLESERVER->NetSendBlessCastleOwnerClanID( psBlessCastleStatusData->iClanID );

				iUpdate = 2;

				BLESSCASTLESERVER->SQLWriteBlessCastleStatus();

				bFinished = TRUE;
			}
		}

		//Finished?
		if ( bFinished )
		{
			//Handle Prizes
			if ( GetSiegeWarMode() == SIEGEWARMODE_TheConquest )
			{
				char szAccountName[32] = { 0 };
				char szClanName[32] = { 0 };
				char szGold[32] = { 0 };

				//1st
				int iGold = 0;

				//2,500,000 every 10,000 points
				iGold = ((psBlessCastleStatusData->sPacketTopData.iDamage[0] / 10000) * 2500000);

				if ( GetClanLeaderAccount( psBlessCastleStatusData->sPacketTopData.iClanID[0], szAccountName ) )
				{
					if ( iGold > MAX_GOLD_PERBOX )
					{
						int iGoldR = iGold;
						int iCount = (iGold / MAX_GOLD_PERBOX) + 1;
						for ( int i = 0; i < iCount; i++ )
						{
							int j = iGoldR > MAX_GOLD_PERBOX ? MAX_GOLD_PERBOX : iGoldR;
							ITEMSERVER->AddItemOpenBox( szAccountName, "GG101", 0, j );
							iGoldR -= MAX_GOLD_PERBOX;
						}
					}
					else
						ITEMSERVER->AddItemOpenBox( szAccountName, "GG101", 0, iGold );
				}

				if ( GetClanName( psBlessCastleStatusData->sPacketTopData.iClanID[0], szClanName ) )
				{
					STRINGCOPY( szGold, FormatNumber( iGold ) );
					CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> Winner Clan: %s!", szClanName );
					CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> 1st Place: %s", szClanName );
					CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> %s Points, %s gp!", FormatNumber( psBlessCastleStatusData->sPacketTopData.iDamage[0] ), szGold );
				}

				//2nd
				if ( psBlessCastleStatusData->sPacketTopData.iClanID[1] )
				{
					iGold = ((psBlessCastleStatusData->sPacketTopData.iDamage[1] / 10000) * 2500000);

					if ( GetClanLeaderAccount( psBlessCastleStatusData->sPacketTopData.iClanID[1], szAccountName ) )
					{
						if ( iGold > MAX_GOLD_PERBOX )
						{
							int iGoldR = iGold;
							int iCount = (iGold / MAX_GOLD_PERBOX) + 1;
							for ( int i = 0; i < iCount; i++ )
							{
								int j = iGoldR > MAX_GOLD_PERBOX ? MAX_GOLD_PERBOX : iGoldR;
								ITEMSERVER->AddItemOpenBox( szAccountName, "GG101", 0, j );
								iGoldR -= MAX_GOLD_PERBOX;
							}
						}
						else
							ITEMSERVER->AddItemOpenBox( szAccountName, "GG101", 0, iGold );
					}

					if ( GetClanName( psBlessCastleStatusData->sPacketTopData.iClanID[1], szClanName ) )
					{
						STRINGCOPY( szGold, FormatNumber( iGold ) );
						CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> 2nd Place: %s", szClanName );
						CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> %s Points, %s gp!", FormatNumber( psBlessCastleStatusData->sPacketTopData.iDamage[1] ), szGold );
					}
				}

				//3rd
				if ( psBlessCastleStatusData->sPacketTopData.iClanID[2] )
				{
					iGold = ((psBlessCastleStatusData->sPacketTopData.iDamage[2] / 10000) * 2500000);

					if ( GetClanLeaderAccount( psBlessCastleStatusData->sPacketTopData.iClanID[2], szAccountName ) )
					{
						if ( iGold > MAX_GOLD_PERBOX )
						{
							int iGoldR = iGold;
							int iCount = (iGold / MAX_GOLD_PERBOX) + 1;
							for ( int i = 0; i < iCount; i++ )
							{
								int j = iGoldR > MAX_GOLD_PERBOX ? MAX_GOLD_PERBOX : iGoldR;
								ITEMSERVER->AddItemOpenBox( szAccountName, "GG101", 0, j );
								iGoldR -= MAX_GOLD_PERBOX;
							}
						}
						else
							ITEMSERVER->AddItemOpenBox( szAccountName, "GG101", 0, iGold );
					}

					if ( GetClanName( psBlessCastleStatusData->sPacketTopData.iClanID[2], szClanName ) )
					{
						STRINGCOPY( szGold, FormatNumber( iGold ) );
						CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> 3rd Place: %s", szClanName );
						CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> %s Points, %s gp!", FormatNumber( psBlessCastleStatusData->sPacketTopData.iDamage[2] ), szGold );
					}
				}

				HandleIndividualScore();

				//Notify Users
				CHATSERVER->SendChatAll( CHATCOLOR_Global, "BlessCastle> All prizes delivered to the leaders!" );
			}
			else if ( GetSiegeWarMode() == SIEGEWARMODE_TheMassacre )
			{
				char szAccountName[32] = { 0 };
				char szClanName[32] = { 0 };
				char szGold[32] = { 0 };

				//1st
				int iGold = 0;

				//5,000,000 every 10,000 points
				iGold = ((psBlessCastleStatusData->sPacketTopData.iDamage[0] / 5000) * 5000000);

				if ( GetClanLeaderAccount( psBlessCastleStatusData->sPacketTopData.iClanID[0], szAccountName ) )
					ITEMSERVER->AddItemOpenBox( szAccountName, "GG101", 0, iGold );


				if ( GetClanName( psBlessCastleStatusData->sPacketTopData.iClanID[0], szClanName ) )
				{
					STRINGCOPY( szGold, FormatNumber( iGold ) );
					CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> Winner Clan: %s!", szClanName );
					CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> 1st Place: %s", szClanName );
					CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> %s Points, %s gp!", FormatNumber( psBlessCastleStatusData->sPacketTopData.iDamage[0] ), szGold );
				}

				//2nd
				if ( psBlessCastleStatusData->sPacketTopData.iClanID[1] )
				{
					iGold = ((psBlessCastleStatusData->sPacketTopData.iDamage[1] / 5000) * 5000000);

					if ( GetClanLeaderAccount( psBlessCastleStatusData->sPacketTopData.iClanID[1], szAccountName ) )
						ITEMSERVER->AddItemOpenBox( szAccountName, "GG101", 0, iGold );

					if ( GetClanName( psBlessCastleStatusData->sPacketTopData.iClanID[1], szClanName ) )
					{
						STRINGCOPY( szGold, FormatNumber( iGold ) );
						CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> 2nd Place: %s", szClanName );
						CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> %s Points, %s gp!", FormatNumber( psBlessCastleStatusData->sPacketTopData.iDamage[1] ), szGold );
					}

				}

				//3rd
				if ( psBlessCastleStatusData->sPacketTopData.iClanID[2] )
				{
					iGold = ((psBlessCastleStatusData->sPacketTopData.iDamage[2] / 5000) * 5000000);

					if ( GetClanLeaderAccount( psBlessCastleStatusData->sPacketTopData.iClanID[2], szAccountName ) )
						ITEMSERVER->AddItemOpenBox( szAccountName, "GG101", 0, iGold );

					if ( GetClanName( psBlessCastleStatusData->sPacketTopData.iClanID[2], szClanName ) )
					{
						STRINGCOPY( szGold, FormatNumber( iGold ) );
						CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> 3rd Place: %s", szClanName );
						CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> %s Points, %s gp!", FormatNumber( psBlessCastleStatusData->sPacketTopData.iDamage[2] ), szGold );
					}
				}

				HandleIndividualScore();

				//Notify Users
				CHATSERVER->SendChatAll( CHATCOLOR_Global, "BlessCastle> All prizes delivered to the leaders!" );
			}
		}

		psBlessCastleStatusData->pcUnitDataTower = pcBlessCastleTower;

		if ( pcBlessCastleTower )
		{
			if ( iCrystalTowerCount > 0 )
			{
				pcBlessCastleTower->sCharacterData.sHP.sCur = pcBlessCastleTower->sCharacterData.sHP.sMax;
				if ( pcBlessCastleTower->iHPBlocks )
					pcBlessCastleTower->sHPBlocks.sCur = pcBlessCastleTower->sHPBlocks.sMax;
			}
			else
			{
				if ( pcBlessCastleTower->psaDamageUsersData == NULL )
				{
					if ( psBlessCastleStatusData->psaDamageData )
					{
						pcBlessCastleTower->psaDamageUsersData = psBlessCastleStatusData->psaDamageData;

						psBlessCastleStatusData->psaDamageData = NULL;
					}
					else
					{
						pcBlessCastleTower->psaDamageUsersData = new AttackDamageData[MAX_DAMAGEDATA];

						if ( pcBlessCastleTower->psaDamageUsersData )
							ZeroMemory( pcBlessCastleTower->psaDamageUsersData, sizeof( AttackDamageData ) * MAX_DAMAGEDATA );
					}
				}
			}

			BlessCastleServer::GetTop10ClanDamage( pcBlessCastleTower, &psBlessCastleStatusData->sPacketTopData );
		}
	}
	
	if ( psBlessCastleStatusData->sPacketTopData.iHeader )
	{
		for ( int i = (psBlessCastleStatusData->iCounter & 0x1F); i < 1024; i += 0x20 )
		{
			UserData * pc = &USERSDATA[i];

			if ( pc->pcSocketData && pc->iID && pc->iMapID == MAPID_BlessCastle )
			{
				if ( CanShowScore() || (pc->iGameLevel >= GAMELEVEL_Two) )
				{
					SENDPACKET( USERDATATOUSER(pc), &psBlessCastleStatusData->sPacketTopData );
				}

				if ( iUpdate > 0 )
				{
					BlessCastleServer::SendBlessCastleDataToLoginServer( pc );

					iUpdate--;
				}
			}
		}
	}

	return TRUE;
}

void BlessCastleServer::SendUserScore( User * pcUser )
{
	if ( psBlessCastleStatusData->iCastleMode <= 0 )
		return;

	PacketBlessCastleUserScoreUpdate s;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT [BlessCastleScore] FROM CharacterInfo WHERE Name=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_Integer, &pcUser->iBlessCastleScore );

				s.iLength = sizeof( PacketBlessCastleUserScoreUpdate );
				s.iHeader = PKTHDR_BlessCastleUserScoreUpdate;
				s.iUserScore = pcUser->iBlessCastleScore;

				SENDPACKET( pcUser, &s );
			}
		}

		pcDB->Close();
	}
}

NAKED int BlessCastleServer::GetClanCodeByID( int iID )
{
	JMP( 0x00557FF0 );
}

ESiegeWarMode BlessCastleServer::GetSiegeWarMode()
{
	return (ESiegeWarMode)EVENT_BLESSCASTLE_MODE;
}

void BlessCastleServer::SetSiegeWarMode( ESiegeWarMode iNewMode )
{
	EVENT_BLESSCASTLE_MODE = iNewMode;
	WRITEDWORD( 0x08B86750, iNewMode );
}

BOOL BlessCastleServer::Start()
{
	Map * pcBCMap = MAPSDATA + MAPID_BlessCastle;

	pcBlessCastleTower = nullptr;

	if ( EVENT_BLESSCASTLE == 2 )
	{
		psBlessCastlePVPLeague->iMode		= 1;
		psBlessCastlePVPLeague->dwBeginTime = TICKCOUNT;

		if ( GetSiegeWarMode() == SIEGEWARMODE_TheMassacre )
			psBlessCastlePVPLeague->dwEndTime = TICKCOUNT + (30 * 60 * 1000);
		else
			psBlessCastlePVPLeague->dwEndTime = TICKCOUNT + (120 * 60 * 1000);
		
		EVENT_BLESSCASTLE = 1;
	}

	psBlessCastleStatusData->iCastleMode	= 1;
	psBlessCastleStatusData->iCounter		= 0;
	psBlessCastleStatusData->iDefenseLevel	= 0;

	if ( GetSiegeWarMode() == SIEGEWARMODE_TheMassacre )
		psBlessCastleStatusData->dwBattleOverTime = TICKCOUNT + (30 * 60 * 1000);
	else
		psBlessCastleStatusData->dwBattleOverTime = TICKCOUNT + (120 * 60 * 1000);

	psBlessCastleStatusData->dwStartTime	= TICKCOUNT;
	psBlessCastleStatusData->saTimeSec[0]	= 0;
	psBlessCastleStatusData->saTimeSec[1]	= 0;
	psBlessCastleStatusData->dwScoreTime	= TICKCOUNT + (5 * 60 * 1000);

	if ( psBlessCastlePVPLeague->iMode )
	{
		psBlessCastlePVPLeague->dwRestartTime = 0;
	}

	if ( psBlessCastleStatusData->psaDamageData == NULL )
		psBlessCastleStatusData->psaDamageData = new AttackDamageData[ATTACK_DAMAGE_MAX];

	if ( psBlessCastleStatusData->psaDamageData )
	{
		ZeroMemory( psBlessCastleStatusData->psaDamageData, sizeof( AttackDamageData ) * ATTACK_DAMAGE_MAX );
	}

	if ( (psBlessCastleStatusData->iClanID == 0) || (GetSiegeWarMode() == SIEGEWARMODE_TheConquest) )
	{
		psBlessCastleStatusData->sSettings.caGuardAmount[0] = 20;
		psBlessCastleStatusData->sSettings.caGuardAmount[1] = 20;
		psBlessCastleStatusData->sSettings.caGuardAmount[2] = 20;
		psBlessCastleStatusData->sSettings.caGuardAmount[3] = 0;

		//Towers
		for ( int i = 0; i < 6; i++ )
			psBlessCastleStatusData->sSettings.saTowerType[i] = (rand() % 3) + 1;
	}

	ZeroMemory( &psBlessCastleStatusData->sPacketTopData, sizeof( PacketBlessCastleTopData ) );

	for ( int i = 0; i < MAX_UNITS; i++ )
	{
		UnitData * pc = UNITSDATA + i;
		if ( pc->bActive && pc->pcMap == pcBCMap )
		{
			bool bIsNPC = (pc->sCharacterData.iType == CHARACTERTYPE_NPC);

			pc->Free();

			if ( (bIsNPC == false) && MAPSERVER->DelMonsterSpawn( pcBCMap, pc ) )
				pcBCMap->iNumAliveUnitData--;
		}
	}

	CharacterData * psaCharacterData	= *(CharacterData**)0x07AB3050;
	int iCounterCharacterData			= (*(int*)0x07AC57E8);

	CharacterData * psCastleDoor	= NULL;
	CharacterData * psValhalla		= NULL;
	CharacterData * psTowerR		= NULL;
	CharacterData * psTowerG		= NULL;
	CharacterData * psTowerB		= NULL;
	CharacterData * psTowerN		= NULL;
	CharacterData * psCastleGuardA	= NULL;
	CharacterData * psCastleGuardB	= NULL;
	CharacterData * psCastleGuardC	= NULL;
	CharacterData * psSetTower		= NULL;

	for ( int i = 0; i < iCounterCharacterData; i++ )
	{
		CharacterData * ps = psaCharacterData + i;

		if ( ps->iMonsterEffectID == MONSTEREFFECTID_CASTLE_DOOR )
			psCastleDoor = ps;
		else if ( ps->iMonsterEffectID == MONSTEREFFECTID_CASTLE_TOWER_B )
			psValhalla = ps;
		else if ( ps->iMonsterEffectID == MONSTEREFFECTID_CASTLE_CRYSTAL_R )
			psTowerR = ps;
		else if ( ps->iMonsterEffectID == MONSTEREFFECTID_CASTLE_CRYSTAL_G )
			psTowerG = ps;
		else if ( ps->iMonsterEffectID == MONSTEREFFECTID_CASTLE_CRYSTAL_B )
			psTowerB = ps;
		else if ( ps->iMonsterEffectID == MONSTEREFFECTID_CASTLE_CRYSTAL_N )
			psTowerN = ps;
		else if ( ps->iMonsterEffectID == MONSTEREFFECTID_CASTLE_SOLDER_A )
			psCastleGuardA = ps;
		else if ( ps->iMonsterEffectID == MONSTEREFFECTID_CASTLE_SOLDER_B )
			psCastleGuardB = ps;
		else if ( ps->iMonsterEffectID == MONSTEREFFECTID_CASTLE_SOLDER_C )
			psCastleGuardC = ps;
	}

	if ( psCastleDoor )
		MAPSERVER->SpawnMonsterBC( pcBCMap, psCastleDoor, sCastleDoorPosition.iX << 8, sCastleDoorPosition.iY << 8, sCastleDoorPosition.iZ << 8 );

	if ( psValhalla )
		pcBlessCastleTower = MAPSERVER->SpawnMonsterBC( pcBCMap, psValhalla, sCastleValhallaPosition.iX << 8, sCastleValhallaPosition.iY << 8, sCastleValhallaPosition.iZ << 8 );

	for ( int i = 0; i < _countof( saBlessCastleTowerPosition ); i++ )
	{
		if ( saBlessCastleTowerPosition[i].iX == 0 )
			break;

		psSetTower = psTowerN;

		switch ( psBlessCastleStatusData->sSettings.saTowerType[i] )
		{
			case 1: //Ice
				psSetTower = psTowerB;
				break;
			case 2: //Lightning
				psSetTower = psTowerG;
				break;
			case 3: //Fire
				psSetTower = psTowerR;
				break;
			default:
				break;
		}

		if ( psSetTower )
			MAPSERVER->SpawnMonsterBC( pcBCMap, psSetTower, saBlessCastleTowerPosition[i].iX << 8, saBlessCastleTowerPosition[i].iY << 8, saBlessCastleTowerPosition[i].iZ << 8 );
	}

	for ( int i = 0; i < 3; i++ )
	{
		if ( psBlessCastleStatusData->sSettings.caGuardAmount[i] > 20 )
			psBlessCastleStatusData->sSettings.caGuardAmount[i] = 20;
	}

	if ( psCastleGuardA )
	{
		for ( int i = 0; i < psBlessCastleStatusData->sSettings.caGuardAmount[0]; i++ )
			MAPSERVER->SpawnMonsterBC( pcBCMap, psCastleGuardA, saCastleGuardPositionA[i].iX << 8, saCastleGuardPositionA[i].iY << 8, saCastleGuardPositionA[i].iZ << 8 );
	}
	if ( psCastleGuardB )
	{
		for ( int i = 0; i < psBlessCastleStatusData->sSettings.caGuardAmount[1]; i++ )
			MAPSERVER->SpawnMonsterBC( pcBCMap, psCastleGuardB, saCastleGuardPositionB[i].iX << 8, saCastleGuardPositionB[i].iY << 8, saCastleGuardPositionB[i].iZ << 8 );
	}
	if ( psCastleGuardC )
	{
		for ( int i = 0; i < psBlessCastleStatusData->sSettings.caGuardAmount[2]; i++ )
			MAPSERVER->SpawnMonsterBC( pcBCMap, psCastleGuardC, saCastleGuardPositionC[i].iX << 8, saCastleGuardPositionC[i].iY << 8, saCastleGuardPositionC[i].iZ << 8 );
	}

	CHATSERVER->SendChatAllUsersInStage( MAPID_BlessCastle, CHATCOLOR_Global, "BlessCastle> Siege War started! Good Luck for all clans!" );

	NETSERVER->HandlePvPBufferLogin( true, false );

	return TRUE;
}

BOOL BlessCastleServer::End( BOOL bUnitsAlive )
{
	Map * pcMap = MAPSDATA + MAPID_BlessCastle;

	psBlessCastleStatusData->iCastleMode = 3;
	psBlessCastleStatusData->dwBattleOverTime = TICKCOUNT + 5000;

	for ( UINT i = 0; i < UNITSERVER->uUnitsInGame; i++ )
	{
		Unit * pcUnit = UNITSERVER->pcaUnitInGame[i];
		if ( pcUnit->pcUnitData->pcMap == pcMap )
		{
			if ( bUnitsAlive && (pcUnit->pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CASTLE_TOWER_B) )
			{
				pcUnit->pcUnitData->sCharacterData.sHP.sCur = pcUnit->pcUnitData->sCharacterData.sHP.sMax;
				pcUnit->pcUnitData->Animate( ANIMATIONTYPE_Idle );
			}
			else
			{
				pcUnit->pcUnitData->sCharacterData.sHP.sCur = 0;
				pcUnit->pcUnitData->Animate( ANIMATIONTYPE_Die );
			}
		}
	}

	return TRUE;
}

NAKED BOOL BlessCastleServer::EasySiegeWar()
{
	JMP( 0x00557CA0 );
}

void BlessCastleServer::EndBattle()
{
	Map * pcMap = MAPSDATA + MAPID_BlessCastle;

	psBlessCastleStatusData->iCastleMode = 0;
	psBlessCastleStatusData->saTimeSec[0] = 0;
	psBlessCastleStatusData->saTimeSec[1] = 0;

	psBlessCastleStatusData->sSettings.caGuardAmount[0] = 0;
	psBlessCastleStatusData->sSettings.caGuardAmount[1] = 0;
	psBlessCastleStatusData->sSettings.caGuardAmount[2] = 0;
	psBlessCastleStatusData->sSettings.caGuardAmount[3] = 0;


	ZeroMemory( psBlessCastleStatusData->sSettings.saTowerType, sizeof( short ) * _countof( psBlessCastleStatusData->sSettings.saTowerType ) );

	if ( psBlessCastleStatusData->psaDamageData )
		DELET( psBlessCastleStatusData->psaDamageData );

	for ( UINT i = 0; i < UNITSERVER->uUnitsInGame; i++ )
	{
		Unit * pcUnit = UNITSERVER->pcaUnitInGame[i];
		if ( pcUnit->pcUnitData->pcMap == pcMap )
		{
			pcUnit->pcUnitData->Free();
			if ( MAPSERVER->DelMonsterSpawn( pcMap, pcUnit->pcUnitData ) )
				pcMap->iNumAliveUnitData--;
		}
	}

	//Spawn NPCs
	for ( UINT i = 0; i < MAX_NPCINMAP; i++ )
	{
		PacketUnitInfo * psNPC = pcMap->saNPC + i;

		if ( (psNPC->iHeader != 0) )
			MAPSERVER->SpawnNPC( pcMap, psNPC );
	}


	CHATSERVER->SendChatAll( CHATCOLOR_Global, "BlessCastle> Siege War has ended! thanks for joining." );

	if ( psBlessCastlePVPLeague->iMode )
	{
		psBlessCastlePVPLeague->iMode = 0;
		psBlessCastlePVPLeague->dwRestartTime = 0;
		psBlessCastlePVPLeague->dwEndTime = 0;
	}

	pcBlessCastleTower = nullptr;

	NETSERVER->HandlePvPBufferLogin( false, true );
}

NAKED void BlessCastleServer::GetTop10ClanDamage( UnitData * pcUnitData, PacketBlessCastleTopData * psPacket )
{
	JMP( 0x0055D3C0 );
}

NAKED void BlessCastleServer::SendBlessCastleDataToLoginServer( UserData * pcUserData )
{
	JMP( 0x00557DD0 );
}

NAKED void BlessCastleServer::MainFountain()
{
	JMP( 0x0055D540 );
}

void BlessCastleServer::KillBuffs()
{
	for ( int i = 0; i < 1024; i++ )
	{
		UserData * pc = &USERSDATA[i];

		if ( pc->pcSocketData && pc->iID )
		{
			pc->dwBlessSkill_Code	= 0;
			pc->dwBlessSkillTime	= 0;
		}
	}
}

void BlessCastleServer::ResetUserScore()
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE CharacterInfo SET [BlessCastleScore]=0" ) )
		{
			pcDB->Execute();
		}

		pcDB->Close();
	}
}

BOOL BlessCastleServer::OnUnitKilled( UnitData * pcUnitData )
{
	if ( pcUnitData == pcBlessCastleTower )
		pcBlessCastleTower = nullptr;

	return TRUE;
}

BOOL BlessCastleServer::GetClanLeaderAccount( int iClanID, char szLeaderAccount[32] )
{
	BOOL bRet = FALSE;

	szLeaderAccount[0] = 0;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ClanDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT AccountName FROM ClanList WHERE IconID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iClanID );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_String, szLeaderAccount, 32 );

				bRet = TRUE;
			}
		}
		pcDB->Close();
	}

	return bRet;
}

BOOL BlessCastleServer::GetClanName( int iClanID, char szName[32] )
{
	BOOL bRet = FALSE;

	szName[0] = 0;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ClanDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT ClanName FROM ClanList WHERE IconID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iClanID );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_String, szName, 32 );

				bRet = TRUE;
			}
		}
		pcDB->Close();
	}

	return bRet;
}

BOOL BlessCastleServer::CanShowScore()
{
	BOOL bRet = TRUE;

	if ( GetSiegeWarMode() == SIEGEWARMODE_TheMassacre )
	{
		bRet = FALSE;

		DWORD dwTime = psBlessCastleStatusData->dwBattleOverTime - psBlessCastleStatusData->dwStartTime;

		//5 minutes or less than? Show score
		if ( (5 * 60 * 1000) >= dwTime )
			bRet = TRUE;
	}

	return bRet;
}

