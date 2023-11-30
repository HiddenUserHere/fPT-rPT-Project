#include "stdafx.h"
#include "pvpserver.h"


void PvPServer::SQLKillLog( User * pcKiller, User * pcVictim, EPvPKillType iKillType )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "INSERT INTO PvPLog([CharacterKillerID],[CharacterVictimID],[MapID],[KillType],[Date]) VALUES(?,?,?,?,GETDATE())" ) )
		{
			int iKillerID	= pcKiller->iCharacterID;
			int iVictimID	= pcVictim->iCharacterID;
			int iMapID		= pcKiller->GetMapID();

			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iKillerID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iVictimID );
			pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &iMapID );
			pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &iKillType );
			pcDB->Execute();
		}
		pcDB->Close();
	}
}

PvPServer::PvPServer()
{
	
}


PvPServer::~PvPServer()
{
}

void PvPServer::Init()
{
    if ( LOGIN_SERVER )
        SQLLoadPvPClanScores();
}

void PvPServer::Tick( ETickType iTickType )
{
    switch ( iTickType )
    {
        case TICKTYPE_1Hour:
            SQLSavePvPClanScores();
            break;
    }
}

BOOL PvPServer::SQLGetTopPvP( PvPClassData & s )
{
	std::vector<int> vCharacters;

	BOOL bRet = FALSE;

	ZeroMemory( &s, sizeof( PvPClassData ) );

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT TOP 100 CharacterID FROM PvPPlayerRank WHERE (Kills>0) ORDER BY Kills DESC" ) )
		{
			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
                    int iCharacterID = -1;
                    
					pcDB->GetData( 1, PARAMTYPE_Integer, &iCharacterID );

					vCharacters.push_back( iCharacterID );
				}
			}
		}

		pcDB->Close();
	}

	for ( auto iCharacterID : vCharacters )
	{
		for ( int i = 0; i < 10; i++ )
		{
			if ( s.iaCharacterID[i] == 0 )
			{
                SQLCharacter sSQLCharacter;
                if ( CHARACTERSERVER->SQLGetCharacter( iCharacterID, &sSQLCharacter ) )
                {
                    if ( sSQLCharacter.iClass == (i + 1) )
                    {
                        STRINGCOPY( s.szCharacterName[i], sSQLCharacter.szCharacterName );

                        s.iaCharacterID[i] = iCharacterID;

                        bRet = TRUE;
                        break;
                    }
                }
			}
		}
	}

	vCharacters.clear();

	return bRet;
}

BOOL PvPServer::SQLSetTopPvP( PvPClassData & s )
{
	BOOL bRet = FALSE;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );

	if ( pcDB->Open() )
	{
		for ( int i = 0; i < 10; i++ )
		{
            if ( s.iaCharacterID[i] > 0 )
            {
                if ( pcDB->Prepare( "UPDATE PvPPlayerRank SET TopPvP=1 WHERE CharacterID=?" ) )
                {
                    pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &s.iaCharacterID[i] );
                    pcDB->Execute();
                }
            }

			pcDB->Clear();
		}
		pcDB->Close();
	}

	return bRet;
}

void PvPServer::ResetPvPBuffer()
{
	for ( int i = 0; i < PLAYERS_MAX; i++ )
	{
		UserData * ud = USERSDATA + i;
		
		if ( ud->pcSocketData && ud->iID )
		{
			User * pcUser = USERDATATOUSER( ud );
			if ( pcUser )
			{
				pcUser->iPvPKills = 0;
				pcUser->iPvPDeaths = 0;
                pcUser->iPvPStreak = 0;
                pcUser->iPvPExperience = 0;

				OnUnLoadUser( pcUser );

				ProcessPvPData( pcUser, 0, FALSE );
			}
		}
	}

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE PvPPlayerRank SET Experience=0, Kills=0, Deaths=0, Streak=0, TopPvP=0" ) )
		{
			pcDB->Execute();
		}
		pcDB->Close();
	}
}

void PvPServer::HandlePvPBuffers( bool bNotifyReset, bool bAddBuff )
{
    return;

	for ( int i = 0; i < PLAYERS_MAX; i++ )
	{
		UserData * ud = USERSDATA + i;

		if ( ud->pcSocketData && ud->iID )
		{
			User * pcUser = USERDATATOUSER( ud );
			if ( pcUser )
				OnUnLoadUser( pcUser );
		}
	}

	PvPClassData s;
	if ( SQLGetTopPvP( s ) )
	{
		for ( int i = 0; i < 10; i++ )
		{
			int iClass = (i + 1);

			if ( s.szCharacterName[i][0] != 0 )
			{
				char * pszClass = "Fighter";

				if ( iClass == CHARACTERCLASS_Mechanician )
					pszClass = "Mechanician";
				if ( iClass == CHARACTERCLASS_Archer )
					pszClass = "Archer";
				if ( iClass == CHARACTERCLASS_Pikeman )
					pszClass = "Pikeman";
				if ( iClass == CHARACTERCLASS_Atalanta )
					pszClass = "Atalanta";
				if ( iClass == CHARACTERCLASS_Knight )
					pszClass = "Knight";
				if ( iClass == CHARACTERCLASS_Magician )
					pszClass = "Magician";
				if ( iClass == CHARACTERCLASS_Priestess )
					pszClass = "Priestess";
				if ( iClass == CHARACTERCLASS_Assassin )
					pszClass = "Assassin";
				if ( iClass == CHARACTERCLASS_Shaman )
					pszClass = "Shaman";

				if ( bAddBuff )
					CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "PvP> Top Rank %s: %s", pszClass, s.szCharacterName[i] );
			}
		}

		ResetPvPBuffer();

		if ( bNotifyReset )
			CHATSERVER->SendChatAll( CHATCOLOR_Global, "PvP> Rank Resetted!" );

		if ( bAddBuff )
		{
			SQLSetTopPvP( s );

			std::vector<int> v;

			for ( int i = 0; i < PLAYERS_MAX; i++ )
			{
				UserData * ud = USERSDATA + i;

				if ( ud->pcSocketData && ud->iID )
				{
					for ( int j = 0; j < 10; j++ )
					{
						if ( s.szCharacterName[j][0] != 0 )
						{
							if ( STRINGCOMPARE( CHARACTERSERVER->GetCharacterName( ud ), s.szCharacterName[j] ) )
							{
								if ( IsTopPVP( USERDATATOUSER( ud ) ) )
									v.push_back( ud->iID );
							}
						}
					}
				}
			}

			if ( v.size() > 0 )
				NETSERVER->SendPvPBuff( v );

			v.clear();
		}
	}
}


BOOL PvPServer::SQLSelectCharacterPvP( User * pcUser )
{
	BOOL bRet = FALSE;
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT ID FROM PvPPlayerRank WHERE CharacterID=?" ) )
		{
			char * pszCharacterName = CHARACTERSERVER->GetCharacterName( pcUser->pcUserData );
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );
			
			if ( pcDB->Execute() && pcDB->Fetch() )
				bRet = TRUE;
		}
		pcDB->Close();
	}
	return bRet;
}

BOOL PvPServer::IsTopPVP( User * pcUser )
{
	BOOL bPvP = FALSE;
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT TopPvP FROM PvPPlayerRank WHERE CharacterID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_Integer, &bPvP, 0 );
			}
		}
		pcDB->Close();
	}

	return bPvP;
}

void PvPServer::SetTopPVP( User * pcUser, BOOL bPvP )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE PvPPlayerRank SET TopPvP=? WHERE CharacterID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &bPvP );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &pcUser->iCharacterID );

			pcDB->Execute();
		}
		pcDB->Close();
	}
}

void PvPServer::ProcessPvPData( User * pcUser, int iID, BOOL bKill )
{
	PacketPvPData sPacket;
	sPacket.iLength			= sizeof( PacketPvPData );
	sPacket.iHeader			= PKTHDR_PVPDataUpdate;
	sPacket.iPvPKills		= pcUser->iPvPKills;
	sPacket.iPvPDeaths		= pcUser->iPvPDeaths;
	sPacket.iPvPStreak  	= pcUser->iPvPStreak;
	sPacket.iPvPExperience  = pcUser->iPvPExperience;
	sPacket.bKill			= bKill;
	sPacket.iID				= iID;
	SENDPACKET( pcUser, &sPacket, TRUE );

	// Old BP
	pcUser->pcUserData->sPVPKills = (short)pcUser->iPvPKills;
}

void PvPServer::ProcessPvPKill( User * pcKiller, User * pcVictim, int iType )
{
	int iaMaps[] =
	{
		MAPID_BlessCastle, MAPID_BattleTown,
	};

	int iMapID = pcKiller->GetMapID();
	BOOL bSend = FALSE;

	for ( int i : iaMaps )
	{
		if ( i == iMapID )
		{
			bSend = TRUE;
			break;
		}
	}

	if ( bSend == FALSE )
		return;

	PacketPvPKillType sPacket;
	sPacket.iLength			= sizeof( PacketPvPKillType );
	sPacket.iHeader			= PKTHDR_PvPKill;
	sPacket.iKillType		= iType;
	sPacket.iKillerClass	= pcKiller->pcUserData->sCharacterData.iClass;
	sPacket.iVictimClass	= pcVictim->pcUserData->sCharacterData.iClass;
	STRINGCOPY( sPacket.szKillerName, CHARACTERSERVER->GetCharacterName( pcKiller->pcUserData ) );
	STRINGCOPY( sPacket.szVictimName, CHARACTERSERVER->GetCharacterName( pcVictim->pcUserData ) );
	SENDPACKETSTAGE( &sPacket, pcKiller->pcUserData->iMapID );
}

#define MAX_TIME_KILLSAFE		80

void PvPServer::OnKilled( User * pcKiller, User * pcVictim )
{
    if ( (pcKiller == NULL) || (pcVictim == NULL) )
        return;

    if ( (!pcKiller->IsConnected()) || (!pcVictim->IsConnected()) )
        return;

    if ( (!pcKiller->IsInGame()) || (!pcVictim->IsInGame()) )
        return;

    //Calculate Experience
    int iExperience = CalculateExperience(
        pcKiller, pcKiller->pcUserData->sCharacterData.iLevel, pcKiller->pcUserData->sCharacterData.iClass,
        pcVictim, pcVictim->pcUserData->sCharacterData.iLevel, pcVictim->pcUserData->sCharacterData.iClass
    );

    //Update the victim's statistics
    pcKiller->bPvPRankChanged = true;
    pcKiller->iPvPKills++;
    pcKiller->iPvPExperience += iExperience;
    pcKiller->iPvPStreak = (pcKiller->iPvPStreak > 0) ? (pcKiller->iPvPStreak + 1) : 1;

    //Update the victim's statistics
    pcVictim->bPvPRankChanged = true;
    pcVictim->iPvPDeaths++;
    pcVictim->iPvPExperience -= iExperience;
    pcVictim->iPvPStreak = (pcVictim->iPvPStreak < 0) ? (pcVictim->iPvPStreak - 1) : -1;

    //Process Clan PvP Adjustments
    ProcessClanKill( pcKiller, pcVictim, iExperience );

	if( pcKiller->GetID() != pcVictim->GetID() )
	{
		CHATSERVER->SendChatEx( pcKiller, CHATCOLOR_Notice, "> You killed %s (+%d battle exp)", CHARACTERSERVER->GetCharacterName( pcVictim->pcUserData ), iExperience );
		CHATSERVER->SendChatEx( pcVictim, CHATCOLOR_Notice, "> %s killed you (-%d battle exp)", CHARACTERSERVER->GetCharacterName( pcKiller->pcUserData ), iExperience );
	}

	ProcessPvPData( pcKiller, pcVictim->pcUserData->iID, TRUE );
	ProcessPvPData( pcVictim, pcKiller->pcUserData->iID, FALSE );
}

void PvPServer::SetPvPBuffs( std::vector<int> vID )
{
	for ( const auto iID : vID )
	{
		UserData * pcUserData = USERDATABYID( iID );
		if ( pcUserData )
		{
			ITEMSERVER->SQLDeleteItemTimer( USERDATATOUSER( pcUserData ), ITEMID_BCBuff );
		//	if (ITEMSERVER->CreatePremium( pcUserData, ITEMID_BCBuff, ITEMTIMERTYPE_BCBuff, ITEMTIMERTIME_72H ) )
		//		SetTopPVP( USERDATATOUSER( pcUserData ), FALSE );
		}
	}
}

BOOL PvPServer::OnLoadUser( User * pcUser )
{
	if ( LOGIN_SERVER )
	{
        SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
        if ( pcDB->Open() )
        {
            if ( pcDB->Prepare( "SELECT Experience, Kills, Deaths, Streak, Date FROM PvPPlayerRank WHERE CharacterID=?" ) )
            {
                pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );

                if ( pcDB->Execute() )
                {
                    if ( pcDB->Fetch() )
                    {
                        SYSTEMTIME sDate;

                        pcUser->bPvPRankLoaded = true;
                        pcUser->bPvPNewRank = false;
                        pcDB->GetData( 1, PARAMTYPE_Integer, &pcUser->iPvPExperience );
                        pcDB->GetData( 2, PARAMTYPE_Integer, &pcUser->iPvPKills );
                        pcDB->GetData( 3, PARAMTYPE_Integer, &pcUser->iPvPDeaths );
                        pcDB->GetData( 4, PARAMTYPE_Integer, &pcUser->iPvPStreak );
                        pcDB->GetData( 5, PARAMTYPE_Time, &sDate, sizeof( SYSTEMTIME ) );

                        int iNotEngagedDays = (int)((SystemTimeToQWORD( GetServerTime() ) - SystemTimeToQWORD( &sDate )) / (60 * 60 * 24));
                        if ( iNotEngagedDays > 15 )
                            iNotEngagedDays = 15;

                        if ( iNotEngagedDays < 0 )
                            iNotEngagedDays = 0;

                        if ( iNotEngagedDays > 0 )
                        {
                            if ( pcUser->iPvPExperience > 0 )
                            {
                                pcUser->iPvPExperience -= ((pcUser->iPvPExperience * (iNotEngagedDays * 10)) / 100);

                                if ( pcUser->iPvPExperience < 0 )
                                    pcUser->iPvPExperience = 0;

                                pcUser->bPvPRankChanged = true;
                            }
                        }
                    }
                    else
                    {
                        pcUser->bPvPRankLoaded = true;
                        pcUser->bPvPNewRank = true;
                        pcUser->iPvPExperience = 0;
                        pcUser->iPvPKills = 0;
                        pcUser->iPvPDeaths = 0;
                        pcUser->iPvPStreak = 0;
                    }
                }
            }

            pcDB->Close();
        }

        if ( pcUser->bPvPRankChanged )
        {
            OnUnLoadUser( pcUser );
            pcUser->bPvPRankChanged = false;
        }

		ProcessPvPData( pcUser, 0, FALSE );
	}
	return TRUE;
}

BOOL PvPServer::OnUnLoadUser( User * pcUser, bool bDateChange )
{
	if ( LOGIN_SERVER )
	{
        char * pszCharacterName = CHARACTERSERVER->GetCharacterName( pcUser->pcUserData );

		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );

        //PvP Rank
        if ( (pcUser->bPvPRankLoaded) && (pcUser->bPvPRankChanged) )
        {
            if ( pcDB->Open() )
            {
                if ( pcUser->bPvPNewRank )
                {
                    if ( pcDB->Prepare( "INSERT INTO PvPPlayerRank ([CharacterID],[CharacterClass],[Experience],[Kills],[Deaths],[Streak],[TopPvP],[Date]) VALUES(?,?,?,?,?,?,0,GETDATE())" ) )
                    {
                        pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );
                        pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &pcUser->pcUserData->sCharacterData.iClass );
                        pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &pcUser->iPvPExperience );
                        pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &pcUser->iPvPKills );
                        pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &pcUser->iPvPDeaths );
                        pcDB->BindParameterInput( 6, PARAMTYPE_Integer, &pcUser->iPvPStreak );

                        pcDB->Execute();
                    }
                }
                else
                {
                    char * pszQuery = "UPDATE PvPPlayerRank SET CharacterClass=?, Experience=?, Kills=?, Deaths=?, Streak=?, Date=GETDATE() WHERE CharacterID=?";
                    if ( bDateChange == false )
                        pszQuery = "UPDATE PvPPlayerRank SET CharacterClass=?, Experience=?, Kills=?, Deaths=?, Streak=? WHERE CharacterID=?";

                    if ( pcDB->Prepare( pszQuery ) )
                    {
                        pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->pcUserData->sCharacterData.iClass );
                        pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &pcUser->iPvPExperience );
                        pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &pcUser->iPvPKills );
                        pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &pcUser->iPvPDeaths );
                        pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &pcUser->iPvPStreak );
                        pcDB->BindParameterInput( 6, PARAMTYPE_Integer, &pcUser->iCharacterID );

                        pcDB->Execute();
                    }
                }

                pcDB->Close();
            }
        }
	}
	return TRUE;
}


void PvPServer::Update()
{
	if ( GAME_SERVER )
		return;

	if ( dwTick < TICKCOUNT )
	{
		SYSTEMTIME * ps = GetServerTime();

		if ( ps->wDayOfWeek == 6 && sLastDay != ps->wDay )
		{
		//	HandlePvPBuffers( false, false );

			sLastDay = ps->wDay;
		}

		dwTick = TICKCOUNT + (120 * 1000);
	}
}

int PvPServer::CalculateExperience( User * pcKiller, int iKillerLevel, ECharacterClass iKillerClass, User * pcVictim, int iVictimLevel, ECharacterClass iVictimClass )
{
    //If the killer is in the same clan as the victim, then 0 exp
    if ( (pcKiller->GetClanID() != 0) && (pcKiller->GetClanID() == pcVictim->GetClanID()) )
    {
        EBattleRoyaleStateEnemy eState = BATTLEROYALEHANDLER->GetStateEnemy( pcKiller, pcVictim );
        if ( pcKiller->bInBattleRoyale && ((eState == BATTLEROYALESTATEENEMY_Friendly) || (eState == BATTLEROYALESTATEENEMY_Enemy) || (eState == BATTLEROYALESTATEENEMY_None)) )
            return 0;

        if ( pcKiller->bInBattleRoyale == false )
            return 0;
    }
    //If the killer has the same IP as the victim, then 0 exp
    if ( pcKiller->pcUserData->pcSocketData->lIP == pcVictim->pcUserData->pcSocketData->lIP )
        return 0;

    //Otherwise, always > 0 exp
    int iExp;

    //Difference in Level of Victim - Killer
    int iLevelDifference = iVictimLevel - iKillerLevel;

    //Boolean based on Level Difference that determines if kill gives positive additional exp or not
    BOOL bPositiveAddedExp;

    //Calculate amount of Base EXP for this level difference
    if ( iLevelDifference >= 30 )
    {
        //No EXP if Victim is 30+ levels higher
        return 0;
    }
    else if ( iLevelDifference >= 25 )
    {
        iExp = 95;
        bPositiveAddedExp = FALSE;
    }
    else if ( iLevelDifference >= 20 )
    {
        iExp = 85;
        bPositiveAddedExp = FALSE;
    }
    else if ( iLevelDifference >= 15 )
    {
        iExp = 75;
        bPositiveAddedExp = FALSE;
    }
    else if ( iLevelDifference >= 10 )
    {
        iExp = 65;
        bPositiveAddedExp = FALSE;
    }
    else if ( iLevelDifference >= 5 )
    {
        iExp = 55;
        bPositiveAddedExp = FALSE;
    }
    else if ( iLevelDifference > -5 )
    {
        iExp = 45;
        bPositiveAddedExp = FALSE;
    }
    else if ( iLevelDifference > -10 )
    {
        iExp = 40;
        bPositiveAddedExp = TRUE;
    }
    else if ( iLevelDifference > -15 )
    {
        iExp = 35;
        bPositiveAddedExp = TRUE;
    }
    else if ( iLevelDifference > -20 )
    {
        iExp = 25;
        bPositiveAddedExp = TRUE;
    }
    else if ( iLevelDifference > -25 )
    {
        iExp = 5;
        bPositiveAddedExp = TRUE;
    }
    else
    {
        iExp = 1;
        bPositiveAddedExp = TRUE;
    }

    //Calculate Exp
    iExp = CalculateBaseExperience( iExp,						//Base EXP
                                    pcVictim->iPvPExperience >= 0,							//Victim has positive EXP
                                    pcVictim->iPvPExperience >= pcKiller->iPvPExperience,	//Victim has higher EXP than Killer
                                    bPositiveAddedExp );									//Extra EXP or Less EXP

    //Streak
    if ( pcKiller->iPvPStreak >= 10 )
    {
        if ( pcVictim->iPvPStreak >= 0 )
        {
            iExp = iExp; //100%
        }
        else if ( pcVictim->iPvPStreak >= -5 )
        {
            iExp = int( round( float( iExp ) * 0.25f ) ); //-75%
        }
        else
        {
            iExp = 0; //0%
        }
    }
    else if ( pcKiller->iPvPStreak > 0 )
    {
        if ( pcVictim->iPvPStreak >= 0 )
        {
            iExp = iExp; //100%
        }
        else if ( pcVictim->iPvPStreak >= -5 )
        {
            iExp = int( round( float( iExp ) * 0.70f ) ); //-30%
        }
        else
        {
            iExp = int( round( float( iExp ) * 0.40f ) ); //-60%
        }
    }

    return iExp;
}

int PvPServer::CalculateBaseExperience( int iBaseExp, BOOL bPositiveExp, BOOL bHigherExp, BOOL bPositiveAddedExp )
{
    int iExp;

    if ( bPositiveAddedExp == FALSE )
    {
        if ( bPositiveExp )
        {
            if ( bHigherExp )
                iExp = iBaseExp;
            else
                iExp = int( round( float( iBaseExp ) * 0.80f ) ); //-20%
        }
        else
        {
            if ( bHigherExp )
                iExp = iBaseExp;
            else
                iExp = int( round( float( iBaseExp ) * 0.40f ) ); //-60%
        }
    }
    else
    {
        if ( bPositiveExp )
        {
            if ( bHigherExp )
                iExp = int( round( float( iBaseExp ) * 1.20f ) ); //+20%
            else
                iExp = iBaseExp;
        }
        else
        {
            if ( bHigherExp )
                iExp = int( round( float( iBaseExp ) * 1.60f ) ); //+60%
            else
                iExp = iBaseExp;
        }
    }

    return iExp;
}

void PvPServer::ProcessClanKill( User * pcKiller, User * pcVictim, int iExperience )
{
    PvPClanScore * k = NULL, * v = NULL;

    if ( (pcKiller->iPvPClanIndex >= 0) && (pcKiller->iPvPClanIndex < MAX_PVPCLANSCORES) )
    {
        k = &saPvPClanScore[pcKiller->iPvPClanIndex];
    }
    else
    {
        if ( pcKiller->pcUserData->iClanID != 0 )
        {
            pcKiller->iPvPClanIndex = GetPvPClanScoreIndex( pcKiller->pcUserData->iClanID );
            if ( pcKiller->iPvPClanIndex != -1 )
                k = &saPvPClanScore[pcKiller->iPvPClanIndex];
        }
    }

    if ( k != NULL )
    {
        k->iKills++;
        k->iExperience += iExperience;
        k->iStreak = (k->iStreak > 0) ? (k->iStreak + 1) : 1;
    }

    if ( (pcVictim->iPvPClanIndex >= 0) && (pcVictim->iPvPClanIndex < MAX_PVPCLANSCORES) )
    {
        v = &saPvPClanScore[pcVictim->iPvPClanIndex];
    }
    else
    {
        if ( pcVictim->pcUserData->iClanID != 0 )
        {
            pcVictim->iPvPClanIndex = GetPvPClanScoreIndex( pcVictim->pcUserData->iClanID );
            if ( pcVictim->iPvPClanIndex != -1 )
                v = &saPvPClanScore[pcVictim->iPvPClanIndex];
        }
    }

    if ( v != NULL )
    {
        v->iDeaths++;
        v->iExperience -= iExperience;
        v->iStreak = (v->iStreak < 0) ? (v->iStreak - 1) : -1;
    }
}

int PvPServer::GetPvPClanScoreIndex( int iClanID )
{
    for ( int i = 0; i < MAX_PVPCLANSCORES; i++ )
    {
        if ( saPvPClanScore[i].iClanID == 0 )
        {
            saPvPClanScore[i].iClanID = iClanID;
            return i;
        }

        if ( saPvPClanScore[i].iClanID == iClanID )
            return i;
    }

    return -1;
}

void PvPServer::SQLLoadPvPClanScores()
{
    SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
    if ( pcDB->Open() )
    {
        if ( pcDB->Prepare( "SELECT ClanID, Experience, Kills, Deaths, Streak FROM PvPClanRank" ) )
        {
            if ( pcDB->Execute() )
            {
                int i = 0;
                while ( pcDB->Fetch() )
                {
                    PvPClanScore * s = &saPvPClanScore[i];
                    pcDB->GetData( 1, PARAMTYPE_Integer, &s->iClanID );
                    pcDB->GetData( 2, PARAMTYPE_Integer, &s->iExperience );
                    pcDB->GetData( 3, PARAMTYPE_Integer, &s->iKills );
                    pcDB->GetData( 4, PARAMTYPE_Integer, &s->iDeaths );
                    pcDB->GetData( 5, PARAMTYPE_Integer, &s->iStreak );

                    i++;
                }
            }
        }

        pcDB->Close();
    }
}

void PvPServer::SQLSavePvPClanScores()
{
    if ( LOGIN_SERVER )
    {
        SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
        if ( pcDB->Open() )
        {
            if ( pcDB->Prepare( "TRUNCATE TABLE PvPClanRank" ) )
            {
                if ( pcDB->Execute() )
                {
                    for ( int i = 0; i < MAX_PVPCLANSCORES; i++ )
                    {
                        PvPClanScore * p = &saPvPClanScore[i];

                        if ( p->iClanID != 0 )
                        {
                            pcDB->Clear();

                            if ( pcDB->Prepare( "INSERT INTO PvPClanRank([ClanID],[Experience],[Kills],[Deaths],[Streak]) VALUES(?,?,?,?,?)" ) )
                            {
                                pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &p->iClanID );
                                pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &p->iExperience );
                                pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &p->iKills );
                                pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &p->iDeaths );
                                pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &p->iStreak );

                                pcDB->Execute();
                            }
                        }
                    }
                }
            }

            pcDB->Close();
        }
    }
}
