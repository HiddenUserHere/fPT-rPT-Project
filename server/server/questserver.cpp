#include "stdafx.h"
#include "questserver.h"

extern BOOL bFreeExpEvent;
extern int iExpFreeExp;

QuestServer::QuestServer()
{
}


QuestServer::~QuestServer()
{
}

BOOL QuestServer::SQLMultipleQuest( int iQuestID )
{
	BOOL bRet = FALSE;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT Multiple FROM QuestList WHERE ID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iQuestID );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_Integer, &bRet );
			}
		}

		pcDB->Close();
	}


	return bRet;
}

std::vector<int> QuestServer::SQLDenyMultipleQuest( int iQuestID )
{
	std::vector<int> vQuests;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT DenyMultipleQuestsID FROM QuestList WHERE ID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iQuestID );

			if ( pcDB->Execute() )
			{
				if ( pcDB->Fetch() )
				{
					char szQuestsList[128] = {0};

					pcDB->GetData( 1, PARAMTYPE_String, szQuestsList, _countof( szQuestsList ) );

					auto vQuestList = split( szQuestsList, ',' );

					for( auto & strQuestID : vQuestList )
						vQuests.push_back( atoi( trim( strQuestID ).c_str() ) );
				}
			}
		}

		pcDB->Close();
	}

	return vQuests;
}

BOOL QuestServer::SQLHaveQuest( User * pcUser, int iQuestID, BOOL bStart )
{
	int iRet = 0;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );

	SYSTEMTIME s;
	SYSTEMTIME * ps = GetServerTime();

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT EndDate FROM CharacterQuest WHERE (CharacterID=?) AND (QuestID=?) AND (EndDate IS NOT NULL)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iQuestID );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_Time, &s );
				iRet = 1;
			}
		}

		pcDB->Close();
	}


	if ( iRet == 0 && pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT EndDate FROM CharacterQuest WHERE (CharacterID=?) AND (QuestID=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iQuestID );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_Time, &s );
				iRet = 2;
			}
		}

		pcDB->Close();
	}

	if ( iRet == 1 )
	{
		DWORD dwTime = (DWORD)(SystemTimeToQWORD( ps ) - SystemTimeToQWORD( &s ));

		pcDB = SQLCONNECTION( DATABASEID_GameDB );

		int iType = 0;

		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "SELECT QuestType FROM QuestList WHERE (ID=?)" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iQuestID );
				
				if ( pcDB->Execute() && pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &iType );

					if ( iType == QUESTTYPE_Daily )
					{
						if ( dwTime >= (60 * 60 * 24) )
							iRet = 3;
						else if ( bStart )
							CHATSERVER->SendChatMessageBox( pcUser, "You need to wait 24 hours before doing this quest again!" );
					}
					else if ( iType == QUESTTYPE_DailyMidNight )
					{
						if ( ps->wDay != s.wDay || ps->wMonth != s.wMonth )
							iRet = 3;
						else if ( bStart )
							CHATSERVER->SendChatMessageBox( pcUser, "You need to wait next day before doing this quest again!" );
					}
					else if ( iType == QUESTTYPE_DailyWeek )
					{
						if ( dwTime >= (60 * 60 * 24 * 7) )
							iRet = 3;
						else if ( bStart )
							CHATSERVER->SendChatMessageBox( pcUser, "You need to wait one week before doing this quest again!" );
					}
					else if ( iType == QUESTTYPE_DailyMonth )
					{
						if ( dwTime >= (60 * 60 * 24 * 30) )
							iRet = 3;
						else if ( bStart )
							CHATSERVER->SendChatMessageBox( pcUser, "You need to wait one month before doing this quest again!" );
					}
					else if ( iType == QUESTTYPE_Repeatable )
					{
						iRet = 3;
					}
					else if ( iType == QUESTTYPE_Weekend )
					{
						if ( (ps->wDayOfWeek == 0) || (ps->wDayOfWeek == 6) )
						{
							if ( dwTime >= (60 * 60 * 24 * 4) )
								iRet = 3;
						}

						if ( bStart && (iRet != 3) )
							CHATSERVER->SendChatMessageBox( pcUser, "You need to wait next weekend before doing this quest again!" );
					}
					else if ( iType == QUESTTYPE_WeekendDaily )
					{
						if ( (ps->wDayOfWeek == 0) || (ps->wDayOfWeek == 6) )
						{
							if ( dwTime >= (60 * 60 * 24) )
								iRet = 3;
						}

						if ( bStart && (iRet != 3) )
							CHATSERVER->SendChatMessageBox( pcUser, "You need to wait 24 hours on this weekend before doing this quest again!" );
					}
					else if ( iType == QUESTTYPE_WeekendDailyMidNight )
					{
						if ( (ps->wDayOfWeek == 0) || (ps->wDayOfWeek == 6) )
						{
							if ( s.wDay != ps->wDay )
								iRet = 3;
						}

						if ( bStart && (iRet != 3) )
							CHATSERVER->SendChatMessageBox( pcUser, "You need to wait next day on this weekend before doing this quest again!" );
					}
					else if ( iType == QUESTTYPE_WeekendRepeatable )
					{
						if ( (ps->wDayOfWeek == 0) || (ps->wDayOfWeek == 6) )
							iRet = 3;
					}
					else if ( iType == QUESTTYPE_WeekFriday )
					{
						if ( s.wDay != ps->wDay )
						{
							if ( ps->wDayOfWeek == 5 )
								iRet = 3; 
							else if ( dwTime < (60 * 60 * 24 * 7) )
							{
								int iDaysCount = ((dwTime / 24) / 60) / 60;
								if ( (s.wDayOfWeek + iDaysCount) >= 5 )
									iRet = 3;
							}
							else
								iRet = 3;
						}

						if ( bStart && (iRet != 3) )
							CHATSERVER->SendChatMessageBox( pcUser, "You need to wait next friday before doing this quest again!" );
					}

				}
			}

			pcDB->Close();
		}
	}

	return iRet;
}

BOOL QuestServer::SQLCanGetQuest( User * pcUser, int iQuestID, BOOL bStart )
{
	//Have Quest?
	int iRet = SQLHaveQuest( pcUser, iQuestID, bStart );

	if ( iRet == 1 || iRet == 2 )
		return FALSE;

	std::vector<int> vQuests = SQLGetActiveQuestsID( pcUser );

	if ( SQLMultipleQuest( iQuestID ) == FALSE )
	{
		if ( vQuests.size() > 0 )
			return FALSE;
	}
	else
	{
		for ( auto i : vQuests )
		{
			if ( SQLMultipleQuest( i ) == FALSE )
				return FALSE;
		}
		/*
		std::vector<int> vDenyQuests = SQLDenyMultipleQuest( iQuestID );
		for ( auto i : vQuests )
		{
			for ( auto j : vQuests )
			{
				if ( j == i )
					return FALSE;
			}
		}
		*/
	}

	struct SQuestData
	{
		int iMinLevel = 0;
		int iMaxLevel = 0;
		char szRequiredQuestIDs[64] = { 0 };
		char szJobRestriction[32] = { 0 };
		char szInclusionQuestIDs[64] = { 0 };
	};

	std::vector<SQuestData> vQuestsData;

	BOOL bRet = TRUE;

	int iLevel = pcUser->pcUserData->sCharacterData.iLevel;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT [MinLevel],[MaxLevel],[RequiredQuestIDs],[InclusionQuestIDs],[ClassRestriction] FROM QuestList WHERE ID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iQuestID );
			
			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				SQuestData sData;

				pcDB->GetData( 1, PARAMTYPE_Integer, &sData.iMinLevel );
				pcDB->GetData( 2, PARAMTYPE_Integer, &sData.iMaxLevel );
				pcDB->GetData( 3, PARAMTYPE_String, sData.szRequiredQuestIDs, _countof( sData.szRequiredQuestIDs ) );
				pcDB->GetData( 4, PARAMTYPE_String, sData.szInclusionQuestIDs, _countof( sData.szInclusionQuestIDs ) );
				pcDB->GetData( 5, PARAMTYPE_String, sData.szJobRestriction, _countof( sData.szJobRestriction ) );

				vQuestsData.push_back( sData );
			}
		}

		pcDB->Close();
	}

	for ( auto & sData : vQuestsData )
	{
		//Inside Level Requeriment?
		if ( sData.iMinLevel <= iLevel && (sData.iMaxLevel == 0 || sData.iMaxLevel >= iLevel) )
		{
			std::vector<std::string> vQuestID = split( sData.szRequiredQuestIDs, ',' );

			//Check if have Required Quests
			for ( auto v : vQuestID )
			{
				if ( atoi( v.c_str() ) > 10 )
				{
					//Not have Quest?
					iRet = SQLHaveQuest( pcUser, atoi( v.c_str() ) );
					if ( iRet == 0 || iRet == 2 )
					{
						//Error
						bRet = FALSE;
						break;
					}
				}
			}

			if ( bRet && !IsNull( sData.szInclusionQuestIDs ) )
			{
				std::vector<std::string> vQuestID = split( sData.szInclusionQuestIDs, ',' );

				bRet = FALSE;

				//Check if have Required Quests
				for ( auto v : vQuestID )
				{
					if ( atoi( v.c_str() ) > 10 )
					{
						//have Quest?
						iRet = SQLHaveQuest( pcUser, atoi( v.c_str() ) );
						if ( iRet != 0 && iRet != 2 )
						{
							//Sucess
							bRet = TRUE;
							break;
						}
					}
				}

			}

			//Class Restriction
			if ( bRet && !IsNull( sData.szJobRestriction ) )
			{
				std::vector<std::string> vJobs = split( sData.szJobRestriction, ',' );

				bRet = FALSE;

				//Check if Class can do this Quest
				for ( auto v : vJobs )
				{
					//Can Do?
					if ( atoi( v.c_str() ) == pcUser->pcUserData->sCharacterData.iClass )
					{
						//Success
						bRet = TRUE;
						break;
					}
				}
			}
		}
		else
		{
			if ( bStart )
			{
				if ( sData.iMaxLevel > 0 )
					CHATSERVER->SendChatMessageBox( pcUser, "You must be level %d-%d to get this quest!", sData.iMinLevel, sData.iMaxLevel );
				else
					CHATSERVER->SendChatMessageBox( pcUser, "You must be level %d to get this quest!", sData.iMinLevel );
			}

			bRet = FALSE;
		}
	}

	vQuestsData.clear();

	return bRet;
}

BOOL QuestServer::SQLCancelQuest( User * pcUser, int iQuestID, BOOL bAutoStartQuest )
{
	if ( SQLHaveQuest( pcUser, iQuestID ) == (bAutoStartQuest ? 1 : 2) )
	{
		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );

		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "DELETE FROM CharacterQuest WHERE (QuestID=?) AND (CharacterID=?)" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iQuestID );
				pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &pcUser->iCharacterID );
				pcDB->Execute();
			}
			pcDB->Close();
		}

		if ( IsT5QuestProgress( iQuestID ) )
		{
			pcUser->uQuestT5ID = 0;
			pcUser->sQuestT5ProgressValue[0] = 0;
			pcUser->sQuestT5ProgressValue[1] = 0;
		}

		for ( int i = 0; i < MAX_QUESTPACKETDATA; i++ )
		{
			PacketQuestUpdateData * ps = pcUser->saPacketQuestData + i;

			if ( (ps->iID != 0) && (ps->iID == iQuestID) )
			{
				ZeroMemory( ps, sizeof( PacketQuestUpdateData ) );

				NETSERVER->SendQuestUpdateData( pcUser );
				break;
			}
		}

		//Log
		char szQuestName[64] = { 0 };

		pcDB = SQLCONNECTION( DATABASEID_GameDB );
		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "SELECT [Name] FROM QuestList WHERE ID=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iQuestID );

				if ( pcDB->Execute() && pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_String, szQuestName, _countof( szQuestName ) );
				}
			}

			pcDB->Close();

			//Log It
			LOGSERVER->OnLogEx( pcUser, ACCLOGID_CharacterQuestCancel, "Character[%s] Cancelled Quest [%s]",
				CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), szQuestName );
		}
	}

	return TRUE;
}

BOOL QuestServer::SQLFinishQuest( User * pcUser, int iQuestID )
{
	BOOL bRet = FALSE;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE CharacterQuest SET EndDate=GETDATE(), Counter=Counter+1 WHERE (CharacterID=?) AND (QuestID=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iQuestID );
			pcDB->Execute();

			bRet = TRUE;
		}

		pcDB->Close();
	}

	return bRet;
}

int QuestServer::SQLGetAutoStartQuestID( User * pcUser, int iQuestID )
{
	int iRet = 0;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT AutoStartQuestID FROM QuestList WHERE ID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iQuestID );
			
			if ( pcDB->Execute() && pcDB->Fetch() )
				pcDB->GetData( 1, PARAMTYPE_Integer, &iRet );
		}

		pcDB->Close();
	}

	return iRet;
}

int QuestServer::SQLGetWasAutoStartQuestID( User * pcUser, int iQuestID )
{
	int iRet = 0;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT ID FROM QuestList WHERE AutoStartQuestID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iQuestID );

			if ( pcDB->Execute() && pcDB->Fetch() )
				pcDB->GetData( 1, PARAMTYPE_Integer, &iRet );
		}

		pcDB->Close();
	}

	return iRet;
}

std::vector<int> QuestServer::SQLGetActiveQuestsID( User * pcUser )
{
	std::vector<int> vQuests;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT QuestID FROM CharacterQuest WHERE (CharacterID=?) AND (EndDate IS NULL)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );

			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
					int iQuestID = 0;
					pcDB->GetData( 1, PARAMTYPE_Integer, &iQuestID );
					vQuests.push_back( iQuestID );
				}
			}
		}

		pcDB->Close();
	}

	return vQuests;
}

BOOL QuestServer::SQLExtraRewardData( int iID, int & iType, char * pszReward, char * pszRewardValue )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );

	BOOL bRet = FALSE;

	int iQuestID = 0;

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT QuestID, ExtraRewardType, ExtraRewardValues FROM QuestRewardList WHERE ID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iID );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_Integer, &iQuestID );
				pcDB->GetData( 2, PARAMTYPE_String, pszReward, 64 );
				pcDB->GetData( 3, PARAMTYPE_String, pszRewardValue, 64 );
				bRet = TRUE;
			}
		}

		pcDB->Close();
	}

	if ( bRet )
	{
		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "SELECT QuestType FROM QuestList WHERE ID=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iQuestID );

				if ( pcDB->Execute() && pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &iType );
					bRet = TRUE;
				}
			}

			pcDB->Close();
		}
	}

	return bRet;
}

BOOL QuestServer::HandleFinishedQuestUser( User * pcUser, int iQuestID )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );

	if ( IsT5QuestProgress( iQuestID ) )
	{
		pcUser->uQuestT5ID = 0;
		pcUser->sQuestT5ProgressValue[0] = 0;
		pcUser->sQuestT5ProgressValue[1] = 0;
	}

	for ( int i = 0; i < MAX_QUESTPACKETDATA; i++ )
	{
		PacketQuestUpdateData * ps = pcUser->saPacketQuestData + i;

		if ( (ps->iID != 0) && (ps->iID == iQuestID) )
		{
			ZeroMemory( ps, sizeof( PacketQuestUpdateData ) );
			break;
		}
	}

	struct SQuestData
	{
		PacketHandleFinishedQuest sPacket;

		int iRewardID = 0;
	};

	std::vector<SQuestData> vQuestsData;

	if ( pcDB->Open() )
	{
		char * pszQuery = "SELECT QuestID, QuestRewardID, StartDate, EndDate FROM CharacterQuest WHERE (CharacterID=?) AND (EndDate IS NOT NULL)";

		if ( iQuestID != 0 )
			pszQuery = "SELECT QuestID, QuestRewardID, StartDate, EndDate FROM CharacterQuest WHERE (CharacterID=?) AND (QuestID=?) AND (EndDate IS NOT NULL)";

		if ( pcDB->Prepare( pszQuery ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );

			if ( iQuestID != 0 )
				pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iQuestID );

			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
					SQuestData sData;
					ZeroMemory( &sData.sPacket, sizeof( PacketHandleFinishedQuest ) );
					sData.sPacket.iLength = sizeof( PacketHandleFinishedQuest );
					sData.sPacket.iHeader = PKTHDR_QuestHandleFinished;
					sData.sPacket.bLoginTime = (iQuestID == 0) ? TRUE : FALSE;

					pcDB->GetData( 1, PARAMTYPE_Integer, &sData.sPacket.iID );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sData.iRewardID );
					pcDB->GetData( 3, PARAMTYPE_Time, &sData.sPacket.sStartDate );
					pcDB->GetData( 4, PARAMTYPE_Time, &sData.sPacket.sEndDate );

					vQuestsData.push_back( sData );

					if ( iQuestID != 0 )
						break;
				}
			}
		}

		pcDB->Close();
	}

	for ( auto & sData : vQuestsData )
	{
		char szExtraReward[64] = { 0 };
		char szExtraRewardValue[64] = { 0 };
		if ( SQLExtraRewardData( sData.iRewardID, sData.sPacket.iType, szExtraReward, szExtraRewardValue ) )
		{
			std::vector<std::string> vExtraReward = split( szExtraReward, ',' );
			std::vector<std::string> vExtraRewardValue = split( szExtraRewardValue, ',' );

			if ( vExtraReward.size() > 0 && vExtraReward.size() == vExtraRewardValue.size() )
			{
				int i = 0;

				for ( auto v : vExtraReward )
				{
					auto c = vExtraRewardValue[i];

					sData.sPacket.iaExtraReward[i] = atoi( v.c_str() );
					sData.sPacket.iaExtraRewardValue[i] = atoi( c.c_str() );

					i++;
				}
			}

			SENDPACKET( pcUser, &sData.sPacket, TRUE );
		}
	}

	vQuestsData.clear();

	return TRUE;
}

int QuestServer::SQLGetQuestTimeLeft( User * pcUser, int iQuestID, DWORD & dwTimeLeft )
{
	int iRet = 0;

	DWORD dwTime = 0;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );

	SYSTEMTIME sStartDate;

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT TimeLeft, StartDate FROM CharacterQuest WHERE (QuestID=?) AND (CharacterID=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iQuestID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &pcUser->iCharacterID );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_Integer, &dwTime );
				pcDB->GetData( 2, PARAMTYPE_Time, &sStartDate );
			}
		}

		pcDB->Close();
	}

	pcDB = SQLCONNECTION( DATABASEID_GameDB );

	DWORD dwMax = 0;

	int iType = 0;
	
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT DurationType, MaxDuration FROM QuestList WHERE (ID=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iQuestID );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_Integer, &iType );
				pcDB->GetData( 2, PARAMTYPE_Integer, &dwMax );

				if ( iType == QUESTDURATIONTYPE_Running )
				{
					DWORD dwStart = abs( (long)(SystemTimeToQWORD( &sStartDate ) - SystemTimeToQWORD( GetServerTime() )) );

					if ( dwStart > dwTime )
						dwTime = 0;
					else
						dwTime -= dwStart;

					iRet = QUESTDURATIONTYPE_Running;
				}
				else if ( iType == QUESTDURATIONTYPE_Amount )
				{
					iRet = QUESTDURATIONTYPE_Amount;
				}
			}
		}

		pcDB->Close();
	}

	dwTimeLeft = dwTime;

	return iRet;
}

int QuestServer::SQLGetQuestTimeLeft( int iQuestID, DWORD & dwTimeLeft )
{
	BOOL iRet = 0;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );

	DWORD dwMax = 0;

	int iType = 0;

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT DurationType, MaxDuration FROM QuestList WHERE (ID=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iQuestID );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_Integer, &iType );
				pcDB->GetData( 2, PARAMTYPE_Integer, &dwMax );

				iRet = iType;
			}
		}

		pcDB->Close();
	}

	dwTimeLeft = dwMax;

	return iRet;
}

BOOL QuestServer::SQLGetAreaQuest( int iQuestID, int * piaMapID, int & iMapCount, int & iMinX, int & iMaxX, int & iMinZ, int & iMaxZ, int & iType, int & iRadius )
{
	BOOL bRet = FALSE;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT MapIDs, AreaType, MinX, MaxX, MinZ, MaxZ, Radius FROM QuestList WHERE (ID=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iQuestID );
			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				char szMapIDs[128] = { 0 };

				pcDB->GetData( 1, PARAMTYPE_String, szMapIDs, _countof( szMapIDs ) );
				pcDB->GetData( 2, PARAMTYPE_Integer, &iType );
				pcDB->GetData( 3, PARAMTYPE_Integer, &iMinX );
				pcDB->GetData( 4, PARAMTYPE_Integer, &iMaxX );
				pcDB->GetData( 5, PARAMTYPE_Integer, &iMinZ );
				pcDB->GetData( 6, PARAMTYPE_Integer, &iMaxZ );
				pcDB->GetData( 7, PARAMTYPE_Integer, &iRadius );
				bRet = TRUE;

				iMapCount = 0;
				auto vMapIDs = split( szMapIDs, ' ' );
				for ( auto & strMapID : vMapIDs )
				{
					piaMapID[iMapCount++] = atoi( strMapID.c_str() );
					if ( iMapCount == MAX_QUEST_MAPDATA )
						break;
				}
			}
		}

		pcDB->Close();
	}

	return bRet;
}

int QuestServer::SQLGetRewardQuestID( int iQuestID )
{
	int iID = -1;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT [ID] FROM QuestRewardList WHERE QuestID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iQuestID );
			
			if ( pcDB->Execute() && pcDB->Fetch() )
				pcDB->GetData( 1, PARAMTYPE_Integer, &iID );
		}

		pcDB->Close();
	}

	return iID;
}

BOOL QuestServer::SQLGetMonsterItem( int iQuestID, char * pszMonster, char * pszMonsterCounter, char * pszItem, char * pszItemCounter )
{
	BOOL bRet = FALSE;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT MonsterID, RequiredItems FROM QuestList WHERE ID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iQuestID );
			
			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_String, pszMonster, 64 );
				pcDB->GetData( 2, PARAMTYPE_String, pszItem, 64 );
				bRet = TRUE;
			}
		}

		pcDB->Close();
	}

	if ( bRet && pcDB->Open() )
	{
		bRet = FALSE;

		if ( pcDB->Prepare( "SELECT MonsterQuantities, RequiredDropQuantities FROM QuestRewardList WHERE QuestID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iQuestID );
			
			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_String, pszMonsterCounter, 64 );
				pcDB->GetData( 2, PARAMTYPE_String, pszItemCounter, 64 );
				bRet = TRUE;
			}
		}

		pcDB->Close();
	}

	return bRet;
}

BOOL QuestServer::SQLGetMonsterItemCharacter( User * pcUser, int iQuestID, char * pszMonsterCounter, char * pszItemCounter )
{
	BOOL bResult = FALSE;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT MonsterCount, ItemCount FROM CharacterQuest WHERE (QuestID=?) AND (CharacterID=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iQuestID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &pcUser->iCharacterID );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_String, pszMonsterCounter, 64 );
				pcDB->GetData( 2, PARAMTYPE_String, pszItemCounter, 64 );
				bResult = TRUE;
			}
		}

		pcDB->Close();
	}

	return bResult;
}

BOOL QuestServer::SQLGetQuestNameShortDesc( int iQuestID, char * pszName, char * pszShortDescription, BOOL & bPvP, int & iParty, int & iType, int & iLevel, BOOL & bMultiple, DWORD & dwWaitingTime, DWORD & dwTimeTotal )
{
	BOOL bRet = FALSE;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT Name, ShortDescription, Party, PVP, QuestType, MinLevel, Multiple, WaitTime, MaxDuration FROM QuestList WHERE ID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iQuestID );
			
			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_String, pszName, 64 );
				pcDB->GetData( 2, PARAMTYPE_String, pszShortDescription, 128 );
				pcDB->GetData( 3, PARAMTYPE_Integer, &iParty );
				pcDB->GetData( 4, PARAMTYPE_Integer, &bPvP );
				pcDB->GetData( 5, PARAMTYPE_Integer, &iType );
				pcDB->GetData( 6, PARAMTYPE_Integer, &iLevel );
				pcDB->GetData( 7, PARAMTYPE_Integer, &bMultiple );
				pcDB->GetData( 8, PARAMTYPE_Integer, &dwWaitingTime );
				pcDB->GetData( 9, PARAMTYPE_Integer, &dwTimeTotal );
				bRet = TRUE;
			}
		}

		pcDB->Close();
	}

	return bRet;
}

BOOL QuestServer::SQLGetDynamicQuestShortData( int iQuestID, char * pszName, IMinMax & sLevel )
{
	BOOL bRet = FALSE;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT Name, MinLevel, MaxLevel FROM QuestList WHERE ID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iQuestID );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_String, pszName, 64 );
				pcDB->GetData( 2, PARAMTYPE_Integer, &sLevel.iMin );
				pcDB->GetData( 3, PARAMTYPE_Integer, &sLevel.iMax );
				bRet = TRUE;
			}
		}

		pcDB->Close();
	}

	return bRet;
}


BOOL QuestServer::SQLStartQuest( User * pcUser, int iQuestID, char * pszMonster, char * pszMonsterCounter, char * pszItem, char * pszItemCounter )
{
	int iRet		= SQLHaveQuest( pcUser, iQuestID, TRUE );
	BOOL bInsert	= TRUE;

	if ( iRet == 3 )
		bInsert = FALSE;
	else if ( iRet != 0 )
		return FALSE;

	int iRewardID	= SQLGetRewardQuestID( iQuestID );
	
	if ( SQLGetMonsterItem( iQuestID, pszMonster, pszMonsterCounter, pszItem, pszItemCounter ) == FALSE )
		return FALSE;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );

	std::string v = pszMonsterCounter;
	UINT uCount = std::count( v.begin(), v.end(), ',' ) + 1;
	if ( IsNull( pszMonsterCounter ) )
		uCount = 0;

	char szMonsterC[64] = { 0 };

	for ( UINT u = 0; u < uCount; u++ )
	{
		if ( u == (uCount - 1) )
			StringCchCatA( szMonsterC, 64, "0" );
		else
			StringCchCatA( szMonsterC, 64, "0," );
	}

	char szItemC[64] = { 0 };

	v = pszItemCounter;
	uCount = std::count( v.begin(), v.end(), ',' ) + 1;
	if ( IsNull( pszItemCounter ) )
		uCount = 0;

	for ( UINT u = 0; u < uCount; u++ )
	{
		if ( u == (uCount - 1) )
			StringCchCatA( szItemC, 64, "0" );
		else
			StringCchCatA( szItemC, 64, "0," );
	}

	DWORD dwTime = 0;
	SQLGetQuestTimeLeft( iQuestID, dwTime );

	if ( pcDB->Open() )
	{
		if ( bInsert )
		{
			if ( pcDB->Prepare( "INSERT INTO CharacterQuest([QuestID],[QuestRewardID],[CharacterID],[AccountName],[CharacterName],[StartDate],[EndDate],[MonsterCount],[ItemCount],[TimeLeft],[Counter]) VALUES (?,?,?,?,?,GETDATE(),NULL,?,?,?,1)" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iQuestID );
				pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iRewardID );
				pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &pcUser->iCharacterID );
				pcDB->BindParameterInput( 4, PARAMTYPE_String, pcUser->pcUserData->szAccountName, _countof( pcUser->pcUserData->szAccountName ) );
				pcDB->BindParameterInput( 5, PARAMTYPE_String, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), 32 );
				pcDB->BindParameterInput( 6, PARAMTYPE_String, szMonsterC, STRLEN( szMonsterC ) );
				pcDB->BindParameterInput( 7, PARAMTYPE_String, szItemC, STRLEN( szItemC ) );
				pcDB->BindParameterInput( 8, PARAMTYPE_Integer, &dwTime );

				pcDB->Execute();
			}
		}
		else
		{
			if ( pcDB->Prepare( "UPDATE CharacterQuest SET [StartDate]=GETDATE(), [EndDate]=NULL, [TimeLeft]=?, [MonsterCount]=?, [ItemCount]=? WHERE (CharacterID=?) AND (QuestID=?)" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &dwTime );
				pcDB->BindParameterInput( 2, PARAMTYPE_String, szMonsterC, STRLEN( szMonsterC ) );
				pcDB->BindParameterInput( 3, PARAMTYPE_String, szItemC, STRLEN( szItemC ) );
				pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &pcUser->iCharacterID );
				pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &iQuestID );

				pcDB->Execute();
			}
		}

		pcDB->Close();
	}

	return TRUE;
}

BOOL QuestServer::SQLQuestSwapClass( char * pszCharacterName, int iOldQuestID, int iNewQuestID )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE CharacterQuest SET QuestID=? WHERE (CharacterName=?) AND (QuestID=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iNewQuestID );
			pcDB->BindParameterInput( 2, PARAMTYPE_String, pszCharacterName, STRLEN( pszCharacterName ) );
			pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &iOldQuestID );

			pcDB->Execute();
		}
		pcDB->Close();
	}

	return TRUE;
}

BOOL QuestServer::LoadQuestData( User * pcUser, DWORD dwQuestID, EQuestStatus eStatus, DWORD dwValue1, DWORD dwValue2, DWORD dwValue3, DWORD dwValue4, DWORD dwValue5, SYSTEMTIME * psFinishDate )
{
	return TRUE;
}

// Load Quests
BOOL QuestServer::OnLoadUser( User * pcUser )
{
	//Not Login Server? not acceptable
	if ( GAME_SERVER )
		return FALSE;

	if ( pcUser->bQuestLoaded )
		return TRUE;

	HandleFinishedQuestUser( pcUser );
	HandleLoadQuests( pcUser );
	
	pcUser->bQuestLoaded = TRUE;

	return TRUE;
}

BOOL QuestServer::OnUnLoadUser( User * pcUser )
{
	// Not Login Server? not acceptable
	if ( GAME_SERVER )
		return FALSE;

	for ( int i = 0; i < MAX_QUESTPACKETDATA; i++ )
	{
		PacketQuestUpdateData * ps = pcUser->saPacketQuestData + i;

		if ( (ps->iID != 0) )
		{
			HandlePacketUnload( pcUser, ps );
		}
	}
	return TRUE;
}

void QuestServer::OnSayTime( UnitData * pcUnitData )
{
	if ( pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_NPC_SKILLMASTER || pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_NPC_MAGICMASTER )
	{
		if ( pcUnitData->sCharacterData.sHP.sCur <= 0 && pcUnitData->pcExclusiveTarget && pcUnitData->lExclusiveTargetID == pcUnitData->pcExclusiveTarget->iID && pcUnitData->pcExclusiveTarget->pcSocketData )
		{
			UserData * ud = pcUnitData->pcExclusiveTarget;
			DWORD dw = pcUnitData->iID;
			ud->dwObjectIDT4 = dw;
			ud->pcUnitDataT4 = pcUnitData;
			// Process T4 Quest
			__asm
			{
				PUSH dw;
				PUSH 0x10; // T4 Quest Code
				PUSH pcUnitData;
				PUSH ud;
				MOV EAX, 0x0055AF20;
				CALL EAX;
				ADD ESP, 0x10;
			}
			ud->dwT4RoomTime = TICKCOUNT + 6000;
			pcUnitData->pcExclusiveTarget = NULL;

			pcUnitData->iFrameTimer = 401;
		}
	}
}

BOOL QuestServer::OnClickDynamicNPC( User * pcUser, UnitData * pcUnitData )
{
	if ( pcUser == NULL || pcUser->pcUserData == NULL || pcUnitData == NULL || pcUnitData->sCharacterData.iType != CHARACTERTYPE_NPC )
		return FALSE;

	BOOL bRet = FALSE;

	int iID = CHARACTERSERVER->SQLGetNPCID( pcUnitData->GetName(), pcUser->pcUserData->iMapID );

	std::vector<std::string> vQuestID;

	if ( iID >= 0 )
	{
		PacketDynamicQuestOpen s;
		ZeroMemory( &s, sizeof( PacketDynamicQuestOpen ) );

		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );

		char szQuestList[128] = { 0 };

		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "SELECT [QuestListID],[MainImage],[MainText] FROM QuestWindowList WHERE NPCID=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iID );

				if ( pcDB->Execute() && pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_String, szQuestList, _countof( szQuestList ) );
					pcDB->GetData( 2, PARAMTYPE_String, s.szImagePath, _countof( s.szImagePath ) );
					pcDB->GetData( 3, PARAMTYPE_String, s.szMainText, _countof( s.szMainText ) );

					vQuestID = split( szQuestList, ',' );

					bRet = TRUE;
				}
			}

			pcDB->Close();
		}

		if ( bRet )
		{
			s.iQuestCount = 0;

			for ( const auto v : vQuestID )
			{
				s.saQuestData[s.iQuestCount].iQuestID = atoi( v.c_str() );
				SQLGetDynamicQuestShortData( s.saQuestData[s.iQuestCount].iQuestID, s.saQuestData[s.iQuestCount].szQuestName, s.saQuestData[s.iQuestCount].sRequiredLevel );

				s.saQuestData[s.iQuestCount].bDone = SQLCanGetQuest( pcUser, s.saQuestData[s.iQuestCount].iQuestID, FALSE );

				s.iQuestCount++;
			}

			ProcessPacket( pcUser, &s );
		}
	}

	vQuestID.clear();

	return bRet;
}

BOOL QuestServer::OnClickNPC( User * pcUser, UnitData * pcUnitData )
{
	if ( pcUser == NULL || pcUser->pcUserData == NULL || pcUnitData == NULL || pcUnitData->sCharacterData.iType != CHARACTERTYPE_NPC )
		return FALSE;

	struct NPCQuestInfoDataS
	{
		PacketQuestNPCOpen sPacket;
		int iQuestID = -1;
		int iTargetNPCID = -1;
		int iNPCID = -1;
		int iConclusionNPCID = -1;
	};

	std::vector<NPCQuestInfoDataS> vQuests;

	BOOL bRet = FALSE;

	int iID = CHARACTERSERVER->SQLGetNPCID( pcUnitData->GetName(), pcUser->pcUserData->iMapID );

	if ( iID >= 0 )
	{
		PacketQuestNPCOpen s;

		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );

		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "SELECT [ID],[Name],[Description],[ProgressText],[ConclusionText],[NPCID],[ProgressNPCID],[ConclusionNPCID] FROM QuestList WHERE (NPCID=? OR ProgressNPCID=? OR ConclusionNPCID=?) AND (MinLevel<=? AND MaxLevel>=?) ORDER BY MinLevel ASC" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iID );
				pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iID );
				pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &iID );
				pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &pcUser->pcUserData->sCharacterData.iLevel );
				pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &pcUser->pcUserData->sCharacterData.iLevel );

				if ( pcDB->Execute() )
				{
					while ( pcDB->Fetch() )
					{
						NPCQuestInfoDataS s;

						pcDB->GetData( 1, PARAMTYPE_Integer, &s.iQuestID );
						pcDB->GetData( 2, PARAMTYPE_String, s.sPacket.szName, _countof( s.sPacket.szName ) );
						pcDB->GetData( 3, PARAMTYPE_String, s.sPacket.szDescription, _countof( s.sPacket.szDescription ) );
						pcDB->GetData( 4, PARAMTYPE_String, s.sPacket.szProgressText, _countof( s.sPacket.szProgressText ) );
						pcDB->GetData( 5, PARAMTYPE_String, s.sPacket.szConclusion, _countof( s.sPacket.szConclusion ) );
						pcDB->GetData( 6, PARAMTYPE_Integer, &s.iNPCID );
						pcDB->GetData( 7, PARAMTYPE_Integer, &s.iTargetNPCID );
						pcDB->GetData( 8, PARAMTYPE_Integer, &s.iConclusionNPCID );

						s.sPacket.iLength = sizeof( PacketQuestNPCOpen );
						s.sPacket.iHeader = PKTHDR_QuestNPCOpen;
						s.sPacket.iID = s.iQuestID;

						vQuests.push_back( s );
					}
				}
			}

			pcDB->Close();
		}
	}

	for ( auto & s : vQuests )
	{
		if ( s.iNPCID == iID )
		{
			if ( SQLCanGetQuest( pcUser, s.iQuestID ) )
			{
				s.sPacket.iType = 1;
				SENDPACKET( pcUser, &s.sPacket );
				break;
			}
			else if ( s.iTargetNPCID == iID )
			{
				if ( SQLHaveQuest( pcUser, s.iQuestID ) == 2 )
				{
					s.sPacket.iType = 2;

					if ( s.iTargetNPCID == s.iConclusionNPCID )
					{
						s.sPacket.iType = 3;
						bRet = TRUE;
					}

					SENDPACKET( pcUser, &s.sPacket );
					break;
				}
			}
		}
		else if ( s.iTargetNPCID == iID )
		{
			if ( SQLHaveQuest( pcUser, s.iQuestID ) == 2 )
			{
				s.sPacket.iType = 2;

				if ( s.iTargetNPCID == s.iConclusionNPCID )
				{
					s.sPacket.iType = 3;
					bRet = TRUE;
				}
				SENDPACKET( pcUser, &s.sPacket );
				break;
			}
		}
		else if ( s.iConclusionNPCID == iID )
		{
			if ( SQLHaveQuest( pcUser, s.iQuestID ) == 2 )
			{
				s.sPacket.iType = 4;
				SENDPACKET( pcUser, &s.sPacket );

				bRet = TRUE;
				break;
			}
		}
	}

	return bRet;
}

void QuestServer::HandlePacket( User * pcUser, PacketQuestNPCAccept * psPacket )
{
	if ( pcUser && pcUser->pcUserData )
	{
		if ( SQLCanGetQuest( pcUser, psPacket->iID, TRUE ) )
		{
			char szMonsterID[64]		= { 0 };
			char szMonsterCounter[64]	= { 0 };
			char szItemID[64]			= { 0 };
			char szItemCounter[64]		= { 0 };

			if ( SQLStartQuest( pcUser, psPacket->iID, szMonsterID, szMonsterCounter, szItemID, szItemCounter ) )
			{
				PacketQuestStartData s;

				SQLGetAreaQuest( psPacket->iID, s.iaMapID, s.iMapCount, s.iXMin, s.iXMax, s.iZMin, s.iZMax, s.iAreaType, s.iRadius );

				s.bTime = SQLGetQuestTimeLeft( psPacket->iID, s.dwTimeLeft ) > 0 ? TRUE : FALSE;

				ZeroMemory( s.iaMonsterID, _countof( s.iaMonsterID ) * sizeof( int ) );
				ZeroMemory( s.iaMonsterCounter, _countof( s.iaMonsterCounter ) * sizeof( int ) );
				ZeroMemory( s.iaMonsterKilled, _countof( s.iaMonsterKilled ) * sizeof( int ) );
				ZeroMemory( s.szaMonsterName, _countof( s.szaMonsterName ) * (sizeof(char)*32) );
				ZeroMemory( s.iaItemID, _countof( s.iaItemID ) * sizeof( int ) );
				ZeroMemory( s.iaItemCounter, _countof( s.iaItemCounter ) * sizeof( int ) );
				ZeroMemory( s.iaItemsCollected, _countof( s.iaItemsCollected ) * sizeof( int ) );

				std::vector<std::string> vMonsterID			= split( szMonsterID, ',' );
				std::vector<std::string> vMonsterCounter	= split( szMonsterCounter, ',' );
				std::vector<std::string> vItemID			= split( szItemID, ',' );
				std::vector<std::string> vItemCounter		= split( szItemCounter, ',' );
				
				if ( (vMonsterID.size() == vMonsterCounter.size() && vMonsterID.size() <= QUEST_MONSTERITEM_MAX) &&
					(vItemID.size() == vItemCounter.size() && vItemID.size() <= QUEST_MONSTERITEM_MAX) )
				{
					int iCounter = 0;

					CharacterData * ps = NULL;

					SQLGetQuestNameShortDesc( psPacket->iID, s.szName, s.szShortDescription, s.bPvP, s.iParty, s.iType, s.iLevel, s.bMultiple, s.dwWaitingTime, s.dwTimeTotal );

					for ( auto v : vMonsterID )
					{
						auto c = vMonsterCounter[iCounter];

						s.iaMonsterID[iCounter]			= atoi( v.c_str() );
						s.iaMonsterCounter[iCounter]	= atoi( c.c_str() );

						if ( (s.iaMonsterID[iCounter] != 0) && (s.iaMonsterID[iCounter] < 50000) )
						{
							ps = UNITINFODATA->GetCharacterDataByMonsterID( s.iaMonsterID[iCounter] );
							if ( ps )
								STRINGCOPY( s.szaMonsterName[iCounter], ps->szName );
						}
						else
						{
							if ( s.bPvP )
								STRINGCOPY( s.szaMonsterName[iCounter], "Players" );
							else
								STRINGCOPY( s.szaMonsterName[iCounter], "Monsters" );
						}
						iCounter++;
					}
				
					iCounter = 0;
					int iCounter2 = 0;

					for ( auto v : vItemID )
					{
						auto c = vItemCounter[iCounter2];

						ItemData * pcItemData = ITEMSERVER->FindItemPointerTable( v.c_str() );

						if ( pcItemData )
						{
							s.iaItemID[iCounter] = pcItemData->sBaseItemID.ToInt();
							s.iaItemCounter[iCounter] = atoi( c.c_str() );

							iCounter++;
						}

						iCounter2++;
					}

					s.iID = psPacket->iID;

					if ( IsT5QuestProgress( psPacket->iID ) )
					{
						pcUser->uQuestT5ID					= 0;
						pcUser->sQuestT5ProgressValue[0]	= 0;
						pcUser->sQuestT5ProgressValue[1]	= 0;
					}

					//Tier5
					ProcessT5Quests( pcUser, s.iID, 0, 0 );

					ProcessPacket( pcUser, &s );

					//Log
					char szQuestName[64] = { 0 };

					SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );
					if ( pcDB->Open() )
					{
						if ( pcDB->Prepare( "SELECT [Name] FROM QuestList WHERE ID=?" ) )
						{
							pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &psPacket->iID );

							if ( pcDB->Execute() && pcDB->Fetch() )
							{
								pcDB->GetData( 1, PARAMTYPE_String, szQuestName, _countof( szQuestName ) );
							}
						}

						pcDB->Close();

						//Log It
						LOGSERVER->OnLogEx( pcUser, ACCLOGID_CharacterQuestGot, "Character[%s] Got Quest [%s]",
							CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), szQuestName );
					}
				}
			}
		}
	}
}

void QuestServer::HandlePacket( User * pcUser, PacketQuestHandleCancel * psPacket )
{
	SQLCancelQuest( pcUser, psPacket->iID, FALSE );

	//Auto Start
	int iAutoStartID = SQLGetWasAutoStartQuestID( pcUser, psPacket->iID );
	if ( iAutoStartID != 0 )
		SQLCancelQuest( pcUser, iAutoStartID, TRUE );
}

void QuestServer::HandlePacketUnload( User * pcUser, PacketQuestUpdateData * psPacket )
{
	if ( pcUser && SQLHaveQuest( pcUser, psPacket->iID ) == 2 )
	{
		DWORD dwTimeLeft = 0;

		int iTime = SQLGetQuestTimeLeft( psPacket->iID, dwTimeLeft );
	
		if ( iTime == QUESTDURATIONTYPE_Amount )
			dwTimeLeft = psPacket->dwTimeLeft;

		char szBuffer[32] = { 0 };


		std::string strM;
		std::string strI;

		BOOL bMonstersFound = FALSE;
		BOOL bItemsFound = FALSE;

		if ( psPacket->iMonsterCount < 0 )
			psPacket->iMonsterCount = 0;
		else if ( psPacket->iMonsterCount > QUEST_MONSTERITEM_MAX )
			psPacket->iMonsterCount = QUEST_MONSTERITEM_MAX;

		if ( psPacket->iItemsCount < 0 )
			psPacket->iItemsCount = 0;
		else if ( psPacket->iItemsCount > QUEST_MONSTERITEM_MAX )
			psPacket->iItemsCount = QUEST_MONSTERITEM_MAX;

		for ( int i = 0; i < psPacket->iMonsterCount; i++ )
		{
			STRINGFORMAT( szBuffer, "%d,", psPacket->iaMonsterKilled[i] );
			strM += szBuffer;
		}

		for ( int i = 0; i < psPacket->iItemsCount; i++ )
		{
			STRINGFORMAT( szBuffer, "%d,", psPacket->iaItemsCollected[i] );
			strI += szBuffer;
		}

		//Tier5
		ProcessT5Quests( pcUser, psPacket->iID, psPacket->iaItemsCollected[0], psPacket->iaItemsCollected[1] );
		
		if ( strI.length() > 0 )
			strI.erase( strI.end() - 1 );

		if ( strM.length() > 0 )
			strM.erase( strM.end() - 1 );

		char szQuery[512] = { 0 };
		STRINGFORMAT( szQuery, "UPDATE CharacterQuest SET [MonsterCount]=%s, [ItemCount]=%s, [TimeLeft]=? WHERE (QuestID=?) AND (CharacterID=?)", strM.length() > 0 ? "?" : "[MonsterCount]", strI.length() > 0 ? "?" : "[ItemCount]" );

		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );

		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( szQuery ) )
			{
				char szBuffer[128] = { 0 };

				int i = 1;

				if ( strM.length() > 0 )
				{
					STRINGCOPY( szBuffer, strM.c_str() );
					pcDB->BindParameterInput( i, PARAMTYPE_String, szBuffer, STRLEN( szBuffer ) );
					i++;
				}

				if ( strI.length() > 0 )
				{
					STRINGCOPY( szBuffer, strI.c_str() );
					pcDB->BindParameterInput( i, PARAMTYPE_String, szBuffer, STRLEN( szBuffer ) );
					i++;
				}

				pcDB->BindParameterInput( i, PARAMTYPE_Integer, &dwTimeLeft );
				i++;
				pcDB->BindParameterInput( i, PARAMTYPE_Integer, &psPacket->iID );
				i++;
				pcDB->BindParameterInput( i, PARAMTYPE_Integer, &pcUser->iCharacterID );


				pcDB->Execute();
			}

			pcDB->Close();
		}
	}
}

void QuestServer::HandlePacket( User * pcUser, PacketQuestUpdateData * psPacket )
{
	BOOL bFound = FALSE;

	for ( int i = 0; i < MAX_QUESTPACKETDATA; i++ )
	{
		PacketQuestUpdateData * ps = pcUser->saPacketQuestData + i;

		if ( (ps->iID != 0) && (psPacket->iID == ps->iID) )
		{
			CopyMemory( ps, psPacket, sizeof( PacketQuestUpdateData ) );
			bFound = TRUE;
			break;
		}
	}

	if ( bFound == FALSE )
	{
		for ( int i = 0; i < MAX_QUESTPACKETDATA; i++ )
		{
			PacketQuestUpdateData * ps = pcUser->saPacketQuestData + i;

			if ( ps->iID == 0 )
			{
				CopyMemory( ps, psPacket, sizeof( PacketQuestUpdateData ) );
				break;
			}
		}
	}

	NETSERVER->SendQuestUpdateData( pcUser );
}

void QuestServer::HandlePacket( User * pcUser, PacketNetQuestUpdateData * psPacket )
{
	if ( pcUser )
	{
		CopyMemory( pcUser->saPacketQuestData, psPacket->saPacket, sizeof( PacketQuestUpdateData ) * MAX_QUESTPACKETDATA );
		
	//	CHATSERVER->SendDebugChat( pcUser->pcUserData, CHATCOLOR_Error, "> %s QuestNet", CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );
	}
}


void QuestServer::HandlePacket( User * pcUser, PacketQuestNPCFinish * psPacket )
{
	if ( pcUser && pcUser->pcUserData )
	{
		if ( SQLHaveQuest( pcUser, psPacket->iID ) == 2 )
		{
			int iMinLevel					= 0;
			INT64 iExp						= 0;
			BOOL bEXPPot					= FALSE;
			BOOL bEXPLevelDifference		= FALSE;
			BOOL bItemRewardSelect			= FALSE;
			char szItemsReward[64]			= { 0 };
			char szItemsQuantityReward[64]	= { 0 };
			char szExtraReward[64]			= { 0 };
			char szExtraRewardValue[64]		= { 0 };

			char szQuestName[64]			= { 0 };

			SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );

			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "SELECT [EXPReward],[EXPPotBonus],[EXPLevelDifference],[ItemRewardSelect],[ItemsReward],[ItemsRewardQuantities],[ExtraRewardType],[ExtraRewardValues] FROM QuestRewardList WHERE QuestID=?" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &psPacket->iID );
					
					if ( pcDB->Execute() && pcDB->Fetch() )
					{
						pcDB->GetData( 1, PARAMTYPE_Int64, &iExp );
						pcDB->GetData( 2, PARAMTYPE_Integer, &bEXPPot );
						pcDB->GetData( 3, PARAMTYPE_Integer, &bEXPLevelDifference );
						pcDB->GetData( 4, PARAMTYPE_Integer, &bItemRewardSelect );
						pcDB->GetData( 5, PARAMTYPE_String, szItemsReward, _countof( szExtraReward ) );
						pcDB->GetData( 6, PARAMTYPE_String, szItemsQuantityReward, _countof( szItemsQuantityReward ) );
						pcDB->GetData( 7, PARAMTYPE_String, szExtraReward, _countof( szExtraReward ) );
						pcDB->GetData( 8, PARAMTYPE_String, szExtraRewardValue, _countof( szExtraRewardValue ) );
					}
				}

				pcDB->Close();
			}

			if ( (bItemRewardSelect == TRUE && psPacket->iItemIndex > 0) || (bItemRewardSelect == FALSE) )
			{
				BOOL bCanFinish = TRUE;

				//Level Quest
				if ( pcDB->Open() )
				{
					if ( pcDB->Prepare( "SELECT [Name], [MinLevel] FROM QuestList WHERE ID=?" ) )
					{
						pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &psPacket->iID );

						if ( pcDB->Execute() && pcDB->Fetch() )
						{
							pcDB->GetData( 1, PARAMTYPE_String, szQuestName, _countof( szQuestName ) );
							pcDB->GetData( 2, PARAMTYPE_Integer, &iMinLevel );
						}
					}

					pcDB->Close();
				}

				//Items Reward?
				if ( IsNull( szItemsReward ) == FALSE )
				{
					std::vector<std::string> strItemsReward = split( szItemsReward, ',' );
					std::vector<std::string> strItemsRewardQuantities = split( szItemsQuantityReward, ',' );

					if ( strItemsReward.size() == strItemsRewardQuantities.size() )
					{
						if ( bItemRewardSelect )
						{
							if ( (psPacket->iItemIndex > 0 && (UINT)psPacket->iItemIndex <= strItemsReward.size()) )
							{
								auto v = strItemsReward[psPacket->iItemIndex - 1];
								auto c = strItemsRewardQuantities[psPacket->iItemIndex - 1];

								int iQuantity = atoi( c.c_str() );

								ItemData * pcItemData = ITEMSERVER->FindItemPointerTable( (char*)v.c_str() );

								if ( pcItemData )
								{
									if ( pcItemData->sBaseItemID.ToItemBase() == ITEMBASE_Potion )
									{
										ITEMSERVER->CreateItem( pcUser->pcUserData, pcItemData->sBaseItemID.ToItemID(), iQuantity );
									}
									else
									{
										DefinitionItem * psDef = ITEMSERVER->FindItemDefByCode( (char*)v.c_str() );

										Item sItem;

										if ( psDef )
										{
											int iWeight = psDef->sItem.iWeight * iQuantity;

											if ( (psPacket->sWeight[0] + iWeight) <= psPacket->sWeight[1] && psPacket->bHaveSpace )
											{
												for ( int j = 0; j < iQuantity; j++ )
												{
													ZeroMemory( &sItem, sizeof( Item ) );

													ITEMSERVER->CreateItem( &sItem, psDef, pcUser->pcUserData->sCharacterData.iClass );

													ITEMSERVER->ValidateItem( &sItem );

													ITEMSERVER->SendItemData( pcUser->pcUserData, &sItem );
												}

												CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> Reward: %s, Quantity: %d", pcItemData->szBaseName, iQuantity );
											}
											else
											{
												if ( psPacket->bHaveSpace == FALSE )
													CHATSERVER->SendChatMessageBox( pcUser, "Not enough space in inventory" );
												else
													CHATSERVER->SendChatMessageBox( pcUser, "Weight limit exceeded" );

												bCanFinish = FALSE;

												psPacket->iID = -1;
												SENDPACKET( pcUser, psPacket );
											}
										}
									}
								}
							}
						}
						else
						{
							int iAddWeight = 0;
							std::vector<QuestItemRewardData*> vItemData;

							for ( UINT i = 0; i < strItemsReward.size(); i++ )
							{
								auto v = strItemsReward[i];
								auto c = strItemsRewardQuantities[i];

								int iQuantity = atoi( c.c_str() );

								ItemData * pcItemData = ITEMSERVER->FindItemPointerTable( (char*)v.c_str() );

								DefinitionItem * psDef = ITEMSERVER->FindItemDefByCode( (char*)v.c_str() );

								if ( psDef )
								{
									iAddWeight += (psDef->sItem.iWeight * iQuantity);

									if ( (psPacket->sWeight[0] + iAddWeight) <= psPacket->sWeight[1] && psPacket->bHaveSpace )
									{
										if ( pcItemData )
										{
											//Add Item to the List
											QuestItemRewardData * ps = new QuestItemRewardData;
											ps->pcItemData	= pcItemData;
											ps->iQuantity	= iQuantity;
											vItemData.push_back( ps );
										}
									}
									else
									{
										if ( psPacket->bHaveSpace == FALSE )
											CHATSERVER->SendChatMessageBox( pcUser, "Not enough space in inventory" );
										else
											CHATSERVER->SendChatMessageBox( pcUser, "Weight limit exceeded" );

										psPacket->iID = -1;
										SENDPACKET( pcUser, psPacket );

										bCanFinish = FALSE;

										break;
									}
								}
							}

							//Now send items to the user
							for ( std::vector<QuestItemRewardData*>::iterator it = vItemData.begin(); it != vItemData.end(); it++ )
							{
								QuestItemRewardData * ps = (*it);
								if ( ps && bCanFinish )
								{
									if ( ps->pcItemData )
									{
										if ( ps->pcItemData->sBaseItemID.ToItemBase() == ITEMBASE_Potion )
										{
											ITEMSERVER->CreateItem( pcUser->pcUserData, ps->pcItemData->sBaseItemID.ToItemID(), ps->iQuantity );
										}
										else
										{

											ITEMSERVER->AddItemOpenBox(pcUser->pcUserData, ps->pcItemData->szInventoryName, pcUser->pcUserData->sCharacterData.iClass, ps->iQuantity);
											//for (int j = 0; j < ps->iQuantity; j++)																																		
												//ITEMSERVER->CreateItem( pcUser->pcUserData, ps->pcItemData->sBaseItemID.ToItemID(), 0 );
										}
										CHATSERVER->SendChatEx(pcUser, CHATCOLOR_Error, "> Reward: %s, Quantity: %d", ps->pcItemData->szBaseName, ps->iQuantity);
										CHATSERVER->SendChatEx(pcUser, CHATCOLOR_Error, "> Check Item Distributor");
									}
								}
								DELET( ps );
							}

							//Clear Data
							vItemData.clear();
						}
					}
				}

				//EXP?
				if ( bCanFinish && iExp > 0 )
				{
					if ( bEXPPot )
					{
						if ( pcUser->iEXPPotion > 0 )
							iExp += (iExp * pcUser->iEXPPotion) / 100;

						if ( (*(int*)0x0084601C) > 0 )
							iExp += (iExp * (*(int*)0x0084601C)) / 100;
					}
					if ( bEXPLevelDifference )
						iExp = UNITSERVER->GetExpLevelDifference( pcUser->pcUserData->sCharacterData.iLevel, iMinLevel, iExp );

					if ( pcUser->pcUserData->sCharacterData.iLevel >= 0 && pcUser->pcUserData->sCharacterData.iLevel <= FREE_EXP_EVENT_LEVEL && bFreeExpEvent )
					{
						iExp += (iExp * iExpFreeExp) / 100;
						CHATSERVER->SendChatEx(pcUser, CHATCOLOR_Notice, "> Free EXP Quest +%d%%", iExpFreeExp);
					}

					std::string strEXP = FormatNumber( iExp );

					CHARACTERSERVER->GiveEXP( pcUser, iExp );
					CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> Reward: %s EXP!", strEXP.c_str() );

					//Log It
					LOGSERVER->OnLogEx( pcUser, ACCLOGID_CharacterQuestEXP, "Character[%s] Finished Quest [%s] Received [%s] EXP",
						CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), szQuestName, strEXP.c_str() );
				}

				if ( bCanFinish && IsNull( szExtraReward ) == FALSE )
				{
					std::vector<std::string> strExtraReward = split( szExtraReward, ',' );
					std::vector<std::string> strExtraRewardValue = split( szExtraRewardValue, ',' );

					if ( strExtraReward.size() == strExtraRewardValue.size() )
					{
						for ( UINT i = 0; i < strExtraReward.size(); i++ )
						{
							auto v = strExtraReward[i];
							auto c = strExtraRewardValue[i];

							int iType = atoi( v.c_str() );
							int iValue = atoi( c.c_str() );

							if ( iType == QUESTREWARDTYPE_Gold )
							{
								CHARACTERSERVER->GiveGOLD( pcUser, iValue );
								CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> Reward: %s gp!", FormatNumber( iValue ) );
							}

							if ( iType == QUESTREWARDTYPE_EXPPotBuff )
							{
								ITEMSERVER->CreatePremium( pcUser->pcUserData, ITEMID_ExpUp24H, ITEMTIMERTYPE_ExpUp, (iValue * 60) );
								CHATSERVER->SendChat( pcUser, CHATCOLOR_Error, "> Reward: EXP Potion!" );
							}
							if ( iType == QUESTREWARDTYPE_ThirdEyeBuff )
							{
								ITEMSERVER->CreatePremium( pcUser->pcUserData, ITEMID_ThirdEye24H, ITEMTIMERTYPE_ThirdEye, (iValue * 60) );
								CHATSERVER->SendChat( pcUser, CHATCOLOR_Error, "> Reward: Third Eye!" );
							}
							if ( iType == QUESTREWARDTYPE_HPUpBuff )
							{
								ITEMSERVER->CreatePremium( pcUser->pcUserData, ITEMID_HPUp12H, ITEMTIMERTYPE_HPUp, (iValue * 60) );
								CHATSERVER->SendChat( pcUser, CHATCOLOR_Error, "> Reward: Vampire Teeth!" );
							}
							if ( iType == QUESTREWARDTYPE_MPUpBuff )
							{
								ITEMSERVER->CreatePremium( pcUser->pcUserData, ITEMID_MPUp12H, ITEMTIMERTYPE_MPUp, (iValue * 60) );
								CHATSERVER->SendChat( pcUser, CHATCOLOR_Error, "> Reward: Mana Aura!" );
							}
						}
					}
				}

				if ( bCanFinish && SQLFinishQuest( pcUser, psPacket->iID ) )
				{
					SENDPACKET( pcUser, psPacket, TRUE );
					HandleFinishedQuestUser( pcUser, psPacket->iID );

					SENDPACKETBLANK( pcUser, PKTHDR_Save, TRUE );

					//Auto Start
					int iAutoStartID = SQLGetAutoStartQuestID( pcUser, psPacket->iID );
					if ( iAutoStartID != 0 )
						HandlePacket( pcUser, &PacketQuestNPCAccept( iAutoStartID ) );
				}
			}

			if ( bItemRewardSelect && psPacket->iItemIndex == 0 )
			{
				//Items Reward?
				if ( IsNull( szItemsReward ) == FALSE )
				{
					std::vector<std::string> strItemsReward = split( szItemsReward, ',' );
					
					std::vector<std::string> strItemsRewardQuantities = split( szItemsQuantityReward, ',' );

					if ( strItemsReward.size() == strItemsRewardQuantities.size() )
					{

						PacketQuestNPCItemList s;
						
						s.iLength = sizeof( PacketQuestNPCItemList );
						s.iHeader = PKTHDR_QuestNPCItemList;

						s.iID = psPacket->iID;

						for ( UINT i = 0; i < strItemsReward.size(); i++ )
						{
							auto v = strItemsReward[i];

							s.bLast = FALSE;

							if ( i == (strItemsReward.size() - 1) )
								s.bLast = TRUE;

							DefinitionItem * psDef = ITEMSERVER->FindItemDefByCode( (char*)v.c_str() );

							if ( psDef )
							{
								ZeroMemory( &s.cItemData.sItem, sizeof( Item ) );

								ITEMSERVER->CreateItem( &s.cItemData.sItem, psDef, pcUser->pcUserData->sCharacterData.iClass );

								SENDPACKET( pcUser, &s, TRUE );
							}
						}
					}
				}
			}
		}
	}
}

void QuestServer::HandlePacket( User * pcUser, PacketDynamicQuestNPCOpen * psPacket )
{
	struct SQuestData
	{
		PacketQuestNPCOpen sPacket;

		int iQuestID = -1;
	};

	std::vector<SQuestData> vQuestsData;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT [ID],[Name],[Description],[ProgressText],[ConclusionText] FROM QuestList WHERE (ID=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &psPacket->iQuestID );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				SQuestData sData;

				pcDB->GetData( 1, PARAMTYPE_Integer, &sData.iQuestID );
				pcDB->GetData( 2, PARAMTYPE_String, sData.sPacket.szName, _countof( sData.sPacket.szName ) );
				pcDB->GetData( 3, PARAMTYPE_String, sData.sPacket.szDescription, _countof( sData.sPacket.szDescription ) );
				pcDB->GetData( 4, PARAMTYPE_String, sData.sPacket.szProgressText, _countof( sData.sPacket.szProgressText ) );
				pcDB->GetData( 5, PARAMTYPE_String, sData.sPacket.szConclusion, _countof( sData.sPacket.szConclusion ) );

				vQuestsData.push_back( sData );
			}
		}

		pcDB->Close();
	}

	for ( auto & sData : vQuestsData )
	{
		if ( SQLCanGetQuest( pcUser, sData.iQuestID, TRUE ) )
		{
			sData.sPacket.iLength = sizeof( PacketQuestNPCOpen );
			sData.sPacket.iHeader = PKTHDR_QuestNPCOpen;
			sData.sPacket.iID = sData.iQuestID;
			sData.sPacket.iType = 1;
			SENDPACKET( pcUser, &sData.sPacket );
		}
	}
}

void QuestServer::HandlePacket( User * pcUser, PacketQuestItemSwap * psPacket )
{
	auto GetQuestDataUpdate = [&pcUser]( int iQuestID )->PacketQuestUpdateData*
	{
		for ( int i = 0; i < MAX_QUESTPACKETDATA; i++ )
		{
			PacketQuestUpdateData * ps = pcUser->saPacketQuestData + i;

			if ( ps->iID != 0 && ps->iID == iQuestID )
				return ps;
		}

		return nullptr;
	};

	PacketQuestUpdateData * psQuestData = NULL;

	switch ( psPacket->iQuestID )
	{
		case QUESTID_GreenLiquidQuestStep2:
			if ( (psQuestData = GetQuestDataUpdate( psPacket->iQuestID )) )
			{
				if ( psQuestData->iaItemsCollected[0] < 1 )
				{
					if ( ITEMSERVER->UseItem( pcUser->pcUserData, psPacket->iItemID, psPacket->iChk1, psPacket->iChk2 ) >= 0 )
					{
						ITEMSERVER->CreateItem( pcUser->pcUserData, ITEMID_GreenLiquidTree, 1 );
					}
				}
			}
			break;

		default:
			break;
	}
}

void QuestServer::HandlePacket( User * pcUser, PacketCustomQuestValue * psPacket )
{
	switch ( psPacket->iQuestID )
	{
		case QUESTID_TheSecretRevealed:
			DROPAREAHANDLER->RemoveItem( pcUser );
			if ( psPacket->iValue1 == 1 )
				DROPAREAHANDLER->AddItem( pcUser, pcUser->pcUserData->sPosition, 600, true );
			break;
	}
}

void QuestServer::HandleLoadQuests( User * pcUser )
{
	if ( pcUser && pcUser->pcUserData )
	{
		pcUser->uQuestT5ID					= 0;
		pcUser->sQuestT5ProgressValue[0]	= 0;
		pcUser->sQuestT5ProgressValue[1]	= 0;

		std::vector<int> vQuestID = SQLGetActiveQuestsID( pcUser );

		for ( std::vector<int>::iterator it = vQuestID.begin(); it != vQuestID.end(); it++ )
		{
			int iQuestID = (*it);
			
			if ( iQuestID != 0 )
			{
				PacketQuestStartData s;
				ZeroMemory( &s, sizeof( PacketQuestStartData ) );

				SQLGetAreaQuest( iQuestID, s.iaMapID, s.iMapCount, s.iXMin, s.iXMax, s.iZMin, s.iZMax, s.iAreaType, s.iRadius );

				s.bTime = SQLGetQuestTimeLeft( pcUser, iQuestID, s.dwTimeLeft ) > 0 ? TRUE : FALSE;
				
				char szMonsterID[64]		= { 0 };
				char szMonsterCounter[64]	= { 0 };
				char szItemID[64]			= { 0 };
				char szItemCounter[64]		= { 0 };
				char szUserMonsterCount[64] = { 0 };
				char szUserItemCount[64]	= { 0 };

				SQLGetQuestNameShortDesc( iQuestID, s.szName, s.szShortDescription, s.bPvP, s.iParty, s.iType, s.iLevel, s.bMultiple, s.dwWaitingTime, s.dwTimeTotal );
				SQLGetMonsterItem( iQuestID, szMonsterID, szMonsterCounter, szItemID, szItemCounter );
				SQLGetMonsterItemCharacter( pcUser, iQuestID, szUserMonsterCount, szUserItemCount );

				std::vector<std::string> vMonsterID = split( szMonsterID, ',' );
				std::vector<std::string> vMonsterCounter = split( szMonsterCounter, ',' );
				std::vector<std::string> vItemID = split( szItemID, ',' );
				std::vector<std::string> vItemCounter = split( szItemCounter, ',' );
				std::vector<std::string> vUserItemCount = split( szUserItemCount, ',' );
				std::vector<std::string> vUserMonsterCount = split( szUserMonsterCount, ',' );

				if ( (vMonsterID.size() == vMonsterCounter.size() && vMonsterID.size() <= QUEST_MONSTERITEM_MAX) &&
					(vItemID.size() == vItemCounter.size() && vItemID.size() <= QUEST_MONSTERITEM_MAX) &&
					(vUserItemCount.size() == vItemID.size() && vUserMonsterCount.size() == vMonsterID.size()) )
				{
					int iCounter = 0;

					CharacterData * ps = NULL;

					for ( auto v : vMonsterID )
					{
						auto c = vMonsterCounter[iCounter];
						auto f = vUserMonsterCount[iCounter];

						s.iaMonsterID[iCounter] = atoi( v.c_str() );
						s.iaMonsterCounter[iCounter] = atoi( c.c_str() );
						s.iaMonsterKilled[iCounter] = atoi( f.c_str() );
						
						if ( (s.iaMonsterID[iCounter] != 0) && (s.iaMonsterID[iCounter] < 50000) )
						{
							ps = UNITINFODATA->GetCharacterDataByMonsterID( s.iaMonsterID[iCounter] );
							if ( ps )
								STRINGCOPY( s.szaMonsterName[iCounter], ps->szName );
						}
						else
						{
							if ( s.bPvP )
								STRINGCOPY( s.szaMonsterName[iCounter], "Players" );
							else
								STRINGCOPY( s.szaMonsterName[iCounter], "Monsters" );
						}

						iCounter++;
					}

					iCounter = 0;
					int iCounter2 = 0;

					for ( auto v : vItemID )
					{
						auto c = vItemCounter[iCounter2];
						auto f = vUserItemCount[iCounter2];

						ItemData * pcItemData = ITEMSERVER->FindItemPointerTable( v.c_str() );

						if ( pcItemData )
						{
							s.iaItemID[iCounter] = pcItemData->sBaseItemID.ToInt();
							s.iaItemCounter[iCounter] = atoi( c.c_str() );
							s.iaItemsCollected[iCounter] = atoi( f.c_str() );

							iCounter++;
						}

						iCounter2++;
					}

					s.iID = iQuestID;

					//Tier5
					ProcessT5Quests( pcUser, s.iID, s.iaItemsCollected[0], s.iaItemsCollected[1] );

					ProcessPacket( pcUser, &s );

				}
			}
		}
	}
}

void QuestServer::ProcessPacket( User * pcUser, PacketQuestStartData * psPacket )
{
	psPacket->iLength = sizeof( PacketQuestStartData );
	psPacket->iHeader = PKTHDR_QuestStartData;
	SENDPACKET( pcUser, psPacket );
}

void QuestServer::ProcessPacket( User * pcUser, PacketDynamicQuestOpen * psPacket )
{
	psPacket->iLength = sizeof( PacketDynamicQuestOpen );
	psPacket->iHeader = PKTHDR_DynamicQuestOpen;
	SENDPACKET( pcUser, psPacket );
}

void QuestServer::ProcessT5Quests( User * pcUser, int iQuestID, int iValue1, int iValue2 )
{
	if ( IsT5QuestProgress( iQuestID ) )
	{
		pcUser->uQuestT5ID					= iQuestID;
		pcUser->sQuestT5ProgressValue[0]	= (USHORT)iValue1;
		pcUser->sQuestT5ProgressValue[1]	= (USHORT)iValue2;

		NETSERVER->SendQuestData( pcUser );
	//	CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "> Quest Process Packet" );
	};
}

void QuestServer::HandlePacket( PacketNetT5QuestData * psPacket )
{
	User * pcUser = USERDATATOUSER( USERDATABYID( psPacket->iID ) );
	if ( pcUser )
	{
		pcUser->uQuestT5ID					= psPacket->uQuestID;
		pcUser->sQuestT5ProgressValue[0]	= psPacket->sQuestValue[0];
		pcUser->sQuestT5ProgressValue[1]	= psPacket->sQuestValue[1];

//		CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "> Quest Handle Packet" );
	}
}

int QuestServer::GetReduceQuestDrop( User * pcUser, UnitData * pcUnitData )
{
	int iDropReduce = 0;

	for ( int i = 0; i < MAX_QUESTPACKETDATA; i++ )
	{
		PacketQuestUpdateData * ps = pcUser->saPacketQuestData + i;

		if ( ps->iID != 0 )
		{
			for ( std::vector<QuestActiveMonsterKillData*>::iterator it = vQuestActiveMonsterKill.begin(); it != vQuestActiveMonsterKill.end(); it++ )
			{
				QuestActiveMonsterKillData * psQuestData = (*it);

				if ( psQuestData && (psQuestData->iQuestID == ps->iID) )
				{
					for ( int j = 0; j < psQuestData->iMonsterCount; j++ )
					{
						for ( int k = 0; k < psQuestData->iMapCount; k++ )
						{
							if ( (psQuestData->iaMapID[k] == -1) || (psQuestData->iaMapID[k] == pcUser->GetMapID()) )
							{
								if ( ((pcUnitData->sCharacterData.iStatPoints == psQuestData->iaMonsterID[j]) || (psQuestData->iaMonsterID[j] == 0)) && (ps->iaMonsterKilled[j] < psQuestData->iaMonsterKillCount[j]) )
								{
									iDropReduce += psQuestData->iDropReduce;
									break;
								}
							}
						}
					}
				}
			}
		}
	}

	return iDropReduce;
}

INT64 QuestServer::GetQuestEXPOnKill( User * pcUser, UnitData * pcUnitData, INT64 iEXP )
{
	INT64 iIncreaseEXP = 0;

	for ( int i = 0; i < MAX_QUESTPACKETDATA; i++ )
	{
		PacketQuestUpdateData * ps = pcUser->saPacketQuestData + i;

		if ( ps->iID != 0 )
		{
			for ( std::vector<QuestActiveMonsterKillData*>::iterator it = vQuestActiveMonsterKill.begin(); it != vQuestActiveMonsterKill.end(); it++ )
			{
				QuestActiveMonsterKillData * psQuestData = (*it);

				if ( psQuestData && (psQuestData->iQuestID == ps->iID) )
				{
					for ( int j = 0; j < psQuestData->iMonsterCount; j++ )
					{
						for ( int k = 0; k < psQuestData->iMapCount; k++ )
						{
							if ( (psQuestData->iaMapID[k] == -1) || (psQuestData->iaMapID[k] == pcUser->GetMapID()) )
							{
								if ( ((pcUnitData->sCharacterData.iStatPoints == psQuestData->iaMonsterID[j]) || (psQuestData->iaMonsterID[j] == 0)) && (ps->iaMonsterKilled[j] < psQuestData->iaMonsterKillCount[j]) )
								{
									iIncreaseEXP += (iEXP * (INT64)psQuestData->iPercentIncreaseEXP) / 100;
									break;
								}
							}
						}
					}
				}
			}
		}
	}

	return iIncreaseEXP;
}

void QuestServer::UpdateQuestActiveMonsterKill()
{
	//Clear List
	{
		for ( std::vector<QuestActiveMonsterKillData*>::iterator it = vQuestActiveMonsterKill.begin(); it != vQuestActiveMonsterKill.end(); it++ )
		{
			QuestActiveMonsterKillData * ps = (*it);
			DELET( ps );
		}

		vQuestActiveMonsterKill.clear();
	}


	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT [QuestID],[PercentIncreaseEXP],[DropReduction] FROM QuestEventList" ) )
		{
			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
					QuestActiveMonsterKillData * ps = new QuestActiveMonsterKillData;
					pcDB->GetData( 1, PARAMTYPE_Integer, &ps->iQuestID );
					pcDB->GetData( 2, PARAMTYPE_Integer, &ps->iPercentIncreaseEXP );
					pcDB->GetData( 3, PARAMTYPE_Integer, &ps->iDropReduce );

					vQuestActiveMonsterKill.push_back( ps );
				}
			}
		}

		pcDB->Close();
	}

	for ( auto ps : vQuestActiveMonsterKill )
	{
		//Monster Kill Count
		char szMonsterID[64] = { 0 };
		char szMonsterCount[64] = { 0 };
		char szItem[64] = { 0 };
		char szItemCount[64] = { 0 };
		if ( SQLGetMonsterItem( ps->iQuestID, szMonsterID, szMonsterCount, szItem, szItemCount ) )
		{
			std::vector<std::string> vMonsterCount = split( szMonsterCount, ',' );
			std::vector<std::string> vMonsterID = split( szMonsterID, ',' );

			ps->iMonsterCount = 0;
			for ( const auto v : vMonsterCount )
			{
				const auto c = vMonsterID[ps->iMonsterCount];

				ps->iaMonsterID[ps->iMonsterCount] = atoi( c.c_str() );
				ps->iaMonsterKillCount[ps->iMonsterCount] = atoi( v.c_str() );
				ps->iMonsterCount++;
			}
		}

		int iAreaType = 0;

		int iXMin = 0;
		int iXMax = 0;
		int iZMin = 0;
		int iZMax = 0;

		int iRadius = 0;

		SQLGetAreaQuest( ps->iQuestID, ps->iaMapID, ps->iMapCount, iXMin, iXMax, iZMin, iZMax, iAreaType, iRadius );
	}
}

BOOL QuestServer::OnUnitKilled( User * pcUser, Unit * pcUnit )
{
	PacketQuestUpdateData sData;

	BOOL bRet = FALSE;
	if ( pcUnit->GetMonsterEffectID() == MONSTEREFFECTID_Gurkob )
	{
		if ( IsActiveQuest( pcUser, QUESTID_GreenLiquidQuestStep1, &sData ) )
		{
			//Chance to Drop?
			if ( RandomI( 0, 100 ) < 10 )
			{
				//Not completed yet?
				if ( sData.iaItemsCollected[0] < 5 )
				{
					//Send Item
					ITEMSERVER->CreateItem( pcUser->pcUserData, ITEMID_MagicRune, 1 );
				}
			}
		}
	}
	return bRet;
}

BOOL QuestServer::IsActiveQuest( User * pcUser, int iQuestID, PacketQuestUpdateData * psData )
{
	BOOL bRet = FALSE;
	for ( int i = 0; i < MAX_QUESTPACKETDATA; i++ )
	{
		PacketQuestUpdateData * ps = pcUser->saPacketQuestData + i;
		if ( ps->iID != 0 )
		{
			if ( ps->iID == iQuestID )
			{
				if ( psData )
					CopyMemory( psData, ps, sizeof( PacketQuestUpdateData ) );

				bRet = TRUE;
				break;
			}
		}
	}

	return bRet;
}

BOOL QuestServer::SkipTier5Quests( User * pcUser )
{
	BOOL bRet = TRUE;

	char szMonsterID[64] = { 0 };
	char szMonsterCounter[64] = { 0 };
	char szItemID[64] = { 0 };
	char szItemCounter[64] = { 0 };

	SQLStartQuest( pcUser, QUESTID_T5QuestArena, szMonsterID, szMonsterCounter, szItemID, szItemCounter );
	SQLStartQuest( pcUser, QUESTID_AMatterOfStrenght, szMonsterID, szMonsterCounter, szItemID, szItemCounter );
	SQLStartQuest( pcUser, QUESTID_CryMeARiver, szMonsterID, szMonsterCounter, szItemID, szItemCounter );
	SQLStartQuest( pcUser, QUESTID_TheLastStage, szMonsterID, szMonsterCounter, szItemID, szItemCounter );
	SQLStartQuest( pcUser, QUESTID_HuntingTheIronGuardian, szMonsterID, szMonsterCounter, szItemID, szItemCounter );
	SQLStartQuest( pcUser, QUESTID_KingHopyQuest, szMonsterID, szMonsterCounter, szItemID, szItemCounter );
	SQLStartQuest( pcUser, QUESTID_MercyfortheLost2, szMonsterID, szMonsterCounter, szItemID, szItemCounter );
	SQLStartQuest( pcUser, QUESTID_TheLoyaltyTestStep2, szMonsterID, szMonsterCounter, szItemID, szItemCounter );

	SQLFinishQuest( pcUser, QUESTID_T5QuestArena );
	SQLFinishQuest( pcUser, QUESTID_AMatterOfStrenght );
	SQLFinishQuest( pcUser, QUESTID_CryMeARiver );
	SQLFinishQuest( pcUser, QUESTID_TheLastStage );
	SQLFinishQuest( pcUser, QUESTID_HuntingTheIronGuardian );
	SQLFinishQuest( pcUser, QUESTID_KingHopyQuest );
	SQLFinishQuest( pcUser, QUESTID_MercyfortheLost2 );
	SQLFinishQuest( pcUser, QUESTID_TheLoyaltyTestStep2 );

	HandleFinishedQuestUser( pcUser, QUESTID_T5QuestArena );
	HandleFinishedQuestUser( pcUser, QUESTID_AMatterOfStrenght );
	HandleFinishedQuestUser( pcUser, QUESTID_CryMeARiver );
	HandleFinishedQuestUser( pcUser, QUESTID_TheLastStage );
	HandleFinishedQuestUser( pcUser, QUESTID_HuntingTheIronGuardian );
	HandleFinishedQuestUser( pcUser, QUESTID_KingHopyQuest );
	HandleFinishedQuestUser( pcUser, QUESTID_MercyfortheLost2 );
	HandleFinishedQuestUser( pcUser, QUESTID_TheLoyaltyTestStep2 );

	SENDPACKETBLANK( pcUser, PKTHDR_Save, TRUE );

	return bRet;
}

BOOL QuestServer::SkipQuestDance( User * pcUser )
{
	BOOL bRet = FALSE;

	char szMonsterID[64] = {0};
	char szMonsterCounter[64] = {0};
	char szItemID[64] = {0};
	char szItemCounter[64] = {0};

	if ( SQLCanGetQuest( pcUser, QUESTID_AnimationDance ) )
	{
		SQLStartQuest( pcUser, QUESTID_AnimationDance, szMonsterID, szMonsterCounter, szItemID, szItemCounter );

		SQLFinishQuest( pcUser, QUESTID_AnimationDance );

		HandleFinishedQuestUser( pcUser, QUESTID_AnimationDance );

		SENDPACKETBLANK( pcUser, PKTHDR_Save, TRUE );

		bRet = TRUE;
	}
	else
	{
		CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "> You already have the dance!" );
		CHATSERVER->SendChatMessageBox( pcUser, "You already have the dance!" );
	}

	return bRet;
}


BOOL QuestServer::IsT5QuestProgress( int iQuestID )
{
	if ( (iQuestID == QUESTID_AMatterOfStrenght) || (iQuestID == QUESTID_CryMeARiver) || (iQuestID == QUESTID_KingHopyQuest) )
		return TRUE;

	return FALSE;
}

