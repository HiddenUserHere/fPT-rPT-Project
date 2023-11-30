#include "stdafx.h"
#include "CAchievementHandler.h"


CAchievementHandler::CAchievementHandler()
{
}


CAchievementHandler::~CAchievementHandler()
{
}

void CAchievementHandler::Init()
{
	SQLGetAchievements();
}

void CAchievementHandler::Shutdown()
{
	//Get Achievement events
	for ( std::vector<CAchievement*>::iterator it = vAchievements.begin(); it != vAchievements.end(); it++ )
	{
		//Clear and delete achievement and events
		CAchievement * pc = (*it);
		SHUTDOWN( pc );
		DELET( pc );
	}

	vAchievements.clear();
}

BOOL CAchievementHandler::SQLGetAchievements()
{
	BOOL bRet = FALSE;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT * FROM AchievementList" ) )
		{
			if( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
					int	 iID					= 0;
					int	 iType					= 0;
					char szName[32]				= { 0 };
					char szImagePath[MAX_PATH]	= { 0 };
					char szDescription[256]		= { 0 };

					pcDB->GetData( 1, PARAMTYPE_Integer, &iID );
					pcDB->GetData( 2, PARAMTYPE_Integer, &iType );
					pcDB->GetData( 3, PARAMTYPE_String, szName, _countof( szName ) );
					pcDB->GetData( 4, PARAMTYPE_String, szImagePath, _countof( szImagePath ) );
					pcDB->GetData( 5, PARAMTYPE_String, szDescription, _countof( szDescription ) );

					//Set new Achievement to the vector to be added new events after that
					CAchievement * pc = new CAchievement();
					pc->SetAchievement( iID, iType, szName, szImagePath, szDescription );
					vAchievements.push_back( pc );
					bRet = TRUE;
				}
			}

		}
		pcDB->Close();
	}

	//Have Achievement?
	if ( bRet )
	{
		//Get Achievement events
		for ( std::vector<CAchievement*>::iterator it = vAchievements.begin(); it != vAchievements.end(); it++ )
		{
			CAchievement * pc = (*it);
			SQLGetAchievementEvent( pc );
		}
	}

	return bRet;
}

BOOL CAchievementHandler::SQLGetAchievementEvent( CAchievement * pcAchievement )
{
	BOOL bRet = FALSE;

	//Get Event List of achievement
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );
	if ( pcDB->Open() )
	{
		int iBindID = pcAchievement->GetID();
		if ( pcDB->Prepare( "SELECT * FROM AchievementEventList WHERE AchievementID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iBindID );
			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
					int	 iID						= 0;
					int	 iAchievementID				= 0;
					int	 iType						= 0;
					BOOL bIsLevelAchievement		= FALSE;
					char szValue1[64]				= { 0 };
					char szValue2[64]				= { 0 };
					char szValue3[64]				= { 0 };
					BOOL bIsUnique					= FALSE;
					BOOL bIsCharacterAchievement	= FALSE;

					//Get data
					pcDB->GetData( 1, PARAMTYPE_Integer, &iID );
					pcDB->GetData( 2, PARAMTYPE_Integer, &iType );
					pcDB->GetData( 3, PARAMTYPE_Integer, &iAchievementID );
					pcDB->GetData( 4, PARAMTYPE_Integer, &bIsLevelAchievement );
					pcDB->GetData( 5, PARAMTYPE_String, szValue1, _countof( szValue1 ) );
					pcDB->GetData( 6, PARAMTYPE_String, szValue2, _countof( szValue2 ) );
					pcDB->GetData( 7, PARAMTYPE_String, szValue3, _countof( szValue3 ) );
					pcDB->GetData( 8, PARAMTYPE_Integer, &bIsUnique );
					pcDB->GetData( 9, PARAMTYPE_Integer, &bIsCharacterAchievement );

					//Set New Achievement Event to the vector to be added new events after that
					CAchievementEvent * pc = new CAchievementEvent();
					pc->SetID( iID );
					pc->SetType( iType );
					pc->SetIsLevelAchievement( bIsLevelAchievement );
					pc->SetValue( 0, szValue1 );
					pc->SetValue( 1, szValue2 );
					pc->SetValue( 2, szValue3 );
					pc->SetIsUnique( bIsUnique );
					pc->SetIsCharacterAchievement( bIsCharacterAchievement );
					pcAchievement->AddEvent( pc );

					bRet = TRUE;
				}
			}

		}
		pcDB->Close();
	}

	return bRet;
}

BOOL CAchievementHandler::HaveAchievement( User * pcUser, EAchievementType iType )
{
	return SQLHaveAchievement( pcUser->iAccountID, pcUser->iCharacterID, iType );
}

BOOL CAchievementHandler::SQLHaveAchievementValue( EAchievementType iType, ValueTaskAchievement & sValue1, ValueTaskAchievement & sValue2, ValueTaskAchievement & sValue3 )
{
	BOOL bRet = FALSE;

	int iAchievementID = GetAchievementID( iType );

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT ID FROM UserAchievement WHERE (AchievementID=?) AND (Value1=?) AND (Value2=?) AND (Value3=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iAchievementID );
			pcDB->BindParameterInput( 2, PARAMTYPE_String, (char*)sValue1.GetString(), STRLEN( sValue1.GetString() ) );
			pcDB->BindParameterInput( 3, PARAMTYPE_String, (char*)sValue2.GetString(), STRLEN( sValue2.GetString() ) );
			pcDB->BindParameterInput( 4, PARAMTYPE_String, (char*)sValue3.GetString(), STRLEN( sValue3.GetString() ) );
			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				bRet = TRUE;
			}
		}
		pcDB->Close();
	}

	return bRet;
}

BOOL CAchievementHandler::CompleteAchievement( User * pcUser, EAchievementType iType, ValueTaskAchievement & sValue1, ValueTaskAchievement & sValue2, ValueTaskAchievement & sValue3 )
{
	BOOL bRet = TRUE;

	BOOL bHave	= FALSE;
	BOOL bLevel = FALSE;

	//Have Achievement?
	if ( (bHave = HaveAchievement( pcUser, iType )) == TRUE )
	{
		//Is not Level Achievement type?
		if ( (bLevel = IsLevelAchievement( iType )) == FALSE )
			bRet = FALSE;
	}

	//Can continue?
	if ( bRet )
	{
		//Update User Achievement
		if ( bHave == FALSE )
			SQLNewAchievement( pcUser, iType, sValue1, sValue2, sValue3 );
		else
			SQLUpdateAchievement( pcUser, iType, bLevel, sValue1, sValue2, sValue3 );

		//Update to the user
		UpdateAchievement( pcUser, iType, bLevel, sValue1, sValue2, sValue3 );
	}

	return bRet;
}

std::vector<PacketAchievementUser> CAchievementHandler::GetUserAchievements( int iAccountID, int iCharacterID )
{
	std::vector<PacketAchievementUser> v;

	//Loaded account?
	if ( iAccountID && iCharacterID )
	{
		//Handle info about Achievement of that account
		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "SELECT [CharacterID], [AchievementID], [Level], [Value1], [Value2], [Value3], [Date] FROM UserAchievement WHERE (AccountID=?)" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iAccountID );
				if ( pcDB->Execute() )
				{
					while ( pcDB->Fetch() )
					{
						int iCharacterDataID	= -1;
						int iAchievementID		= -1;
						int iLevel				= 0;
						char szValue1[64]		= { 0 };
						char szValue2[64]		= { 0 };
						char szValue3[64]		= { 0 };
						SYSTEMTIME sDate		= { 0 };

						//Get data
						pcDB->GetData( 1, PARAMTYPE_Integer, &iCharacterDataID );
						pcDB->GetData( 2, PARAMTYPE_Integer, &iAchievementID );
						pcDB->GetData( 3, PARAMTYPE_Integer, &iLevel );
						pcDB->GetData( 4, PARAMTYPE_String, szValue1, _countof( szValue1 ) );
						pcDB->GetData( 5, PARAMTYPE_String, szValue2, _countof( szValue2 ) );
						pcDB->GetData( 6, PARAMTYPE_String, szValue3, _countof( szValue3 ) );
						pcDB->GetData( 7, PARAMTYPE_Time, &sDate );

						//Create the packet to be send to the user
						PacketAchievementUser sPacket;
						ZeroMemory( &sPacket, sizeof( PacketAchievementUser ) );
						sPacket.iAchievementID	= iAchievementID;
						sPacket.iLevel			= iLevel;
						STRINGCOPY( sPacket.szaValue[0], szValue1 );
						STRINGCOPY( sPacket.szaValue[1], szValue2 );
						STRINGCOPY( sPacket.szaValue[2], szValue3 );
						CopyMemory( &sPacket.sDate, &sDate, sizeof( SYSTEMTIME ) );

						//Achievement Account or only for the current Character?
						if ( (iCharacterDataID == (-1)) || (iCharacterDataID == iCharacterID) )
						{
							//Add to the user
							v.push_back( sPacket );
						}
					}
				}
			}
			pcDB->Close();
		}
	}

	return v;
}

BOOL CAchievementHandler::GetUserAchievements( User * pcUser, User * pcTarget )
{
	BOOL bRet = FALSE;

	//Get Target achievements to be send to the user
	std::vector<PacketAchievementUser> vAchievementUser = GetUserAchievements( pcTarget->iAccountID, pcTarget->iCharacterID );
	if ( vAchievementUser.size() > 0 )
	{
		//Send packets to the user
		for ( auto & sPacket : vAchievementUser )
		{
			//Set ID of user to identify on client side
			sPacket.iID = pcTarget->GetID();
			//Send
			ProcessPacket( pcUser, &sPacket );
		}

		bRet = TRUE;
	}

	return bRet;
}

std::vector<PacketAchievementUser> CAchievementHandler::GetCharacterAchievements( const char * pszCharacterName )
{
	if( pszCharacterName == NULL || pszCharacterName[0] == 0 )
		return std::vector<PacketAchievementUser>();

	//Get Character
	SQLCharacter s;
	if ( CHARACTERSERVER->SQLGetCharacter( pszCharacterName, &s ) )
	{
		//Get Account ID
		int iAccountID = UserServer::SQLGetAccountID( s.szAccountName );

		//Get User Achievements
		return GetUserAchievements( iAccountID, s.iID );
	}

	//Error
	return std::vector<PacketAchievementUser>();
}

BOOL CAchievementHandler::OnLoadUser( User * pcUser )
{
	BOOL bRet = FALSE;

	//Get Achievements
	if ( vAchievements.size() > 0 )
	{
		for ( std::vector<CAchievement*>::iterator it = vAchievements.begin(); it != vAchievements.end(); it++ )
		{
			CAchievement * pcAchievement = (*it);
			
			PacketAchievement sPacket;
			ZeroMemory( &sPacket, sizeof( PacketAchievement ) );
			sPacket.iID		= pcAchievement->GetID();
			sPacket.iType	= pcAchievement->GetType();
			STRINGCOPY( sPacket.szName, pcAchievement->GetName() );
			STRINGCOPY( sPacket.szImagePath, pcAchievement->GetImagePath() );
			STRINGCOPY( sPacket.szDescription, pcAchievement->GetDescription() );

			//Get Events of that Achievement
			for ( std::vector<CAchievementEvent*>::iterator itE = pcAchievement->GetEvents().begin(); itE != pcAchievement->GetEvents().end(); itE++ )
			{
				CAchievementEvent * pcAchievementEvent = (*itE);

				sPacket.saEvent[sPacket.iEventCount].iID						= pcAchievementEvent->GetID();
				sPacket.saEvent[sPacket.iEventCount].iType						= pcAchievementEvent->GetType();
				sPacket.saEvent[sPacket.iEventCount].bIsLevelAchievement		= pcAchievementEvent->IsLevelAchievement();
				STRINGCOPY( sPacket.saEvent[sPacket.iEventCount].szaValue[0], pcAchievementEvent->GetValueString( 0 ) );
				STRINGCOPY( sPacket.saEvent[sPacket.iEventCount].szaValue[1], pcAchievementEvent->GetValueString( 1 ) );
				STRINGCOPY( sPacket.saEvent[sPacket.iEventCount].szaValue[2], pcAchievementEvent->GetValueString( 2 ) );
				sPacket.saEvent[sPacket.iEventCount].bIsUnique					= pcAchievementEvent->IsUnique();
				sPacket.saEvent[sPacket.iEventCount].bIsCharacterAchievement	= pcAchievementEvent->IsCharacterAchievement();
				sPacket.iEventCount++;
			}

			//Send packet to the user
			ProcessPacket( pcUser, &sPacket );
		}

		//Get User Achievements
		std::vector<PacketAchievementUser> vAchievementUser = GetUserAchievements( pcUser->iAccountID, pcUser->iCharacterID );
		if ( vAchievementUser.size() > 0 )
		{
			//Send packets to the user
			for ( auto & sPacket : vAchievementUser )
			{
				//Set ID of user to identify on client side
				sPacket.iID = 0;

				pcUser->vAchievement.push_back( sPacket );

				//Send
				ProcessPacket( pcUser, &sPacket );
			}
		}

		bRet = TRUE;
	}

	return bRet;
}

BOOL CAchievementHandler::OnUnloadUser( User * pcUser )
{
	pcUser->vAchievement.clear();

	return TRUE;
}


BOOL CAchievementHandler::OnTask( User * pcUser, EAchievementTask iTask, ValueTaskAchievement & sValue1, ValueTaskAchievement & sValue2, ValueTaskAchievement & sValue3 )
{
	BOOL bRet = FALSE;

	switch ( iTask )
	{
		case ACHIEVEMENTTASK_LevelUP:
			//Reach Level 115?
			if ( pcUser->GetGameLevel() == 0 )
			{
				if ( sValue1.GetInt() == 115 )
				{
					if ( SQLHaveAchievementValue( ACHIEVEMENTTYPE_ReachLevel115, ValueTaskAchievement( pcUser->pcUserData->sCharacterData.iClass ), ValueTaskAchievement( 0 ), ValueTaskAchievement( 0 ) ) == FALSE )
					{
						CompleteAchievement( pcUser, ACHIEVEMENTTYPE_ReachLevel115, ValueTaskAchievement( pcUser->pcUserData->sCharacterData.iClass ), ValueTaskAchievement( 0 ), ValueTaskAchievement( 0 ) );

						const char * pszClassName = GetCharacterClassByFlag( CharacterClassToClassFlag( pcUser->pcUserData->sCharacterData.iClass ) );
						if ( pszClassName && pszClassName[0] != 0 )
							CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "Achievement> New Top %s: %s! Congratulations!", pszClassName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );

						bRet = TRUE;
					}
				}
				else if ( sValue1.GetInt() == 125 )
				{
					if ( SQLHaveAchievementValue( ACHIEVEMENTTYPE_ReachLevel125, ValueTaskAchievement( pcUser->pcUserData->sCharacterData.iClass ), ValueTaskAchievement( 0 ), ValueTaskAchievement( 0 ) ) == FALSE )
					{
						CompleteAchievement( pcUser, ACHIEVEMENTTYPE_ReachLevel125, ValueTaskAchievement( pcUser->pcUserData->sCharacterData.iClass ), ValueTaskAchievement( 0 ), ValueTaskAchievement( 0 ) );

						const char * pszClassName = GetCharacterClassByFlag( CharacterClassToClassFlag( pcUser->pcUserData->sCharacterData.iClass ) );
						if ( pszClassName && pszClassName[0] != 0 )
							CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "Achievement> New Top %s: %s! Congratulations!", pszClassName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );

						bRet = TRUE;
					}
				}
				else if ( sValue1.GetInt() == 130 )
				{
					if ( SQLHaveAchievementValue( ACHIEVEMENTTYPE_ReachLevel130, ValueTaskAchievement( pcUser->pcUserData->sCharacterData.iClass ), ValueTaskAchievement( 0 ), ValueTaskAchievement( 0 ) ) == FALSE )
					{
						CompleteAchievement( pcUser, ACHIEVEMENTTYPE_ReachLevel130, ValueTaskAchievement( pcUser->pcUserData->sCharacterData.iClass ), ValueTaskAchievement( 0 ), ValueTaskAchievement( 0 ) );

						const char * pszClassName = GetCharacterClassByFlag( CharacterClassToClassFlag( pcUser->pcUserData->sCharacterData.iClass ) );
						if ( pszClassName && pszClassName[0] != 0 )
							CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "Achievement> New Top %s: %s! Congratulations!", pszClassName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );

						bRet = TRUE;
					}
				}
			}
			break;

		case ACHIEVEMENTTASK_BattleRoyaleWinner:
			if ( GetCharacterRace( pcUser->pcUserData->sCharacterData.iClass ) == CHARACTERRACE_Tempskron )
				CompleteAchievement( pcUser, ACHIEVEMENTTYPE_BattleRoyaleLastStandingT, ValueTaskAchievement( pcUser->pcUserData->sCharacterData.iClass ), ValueTaskAchievement( 0 ), ValueTaskAchievement( 0 ) );
			else if ( GetCharacterRace( pcUser->pcUserData->sCharacterData.iClass ) == CHARACTERRACE_Morion )
				CompleteAchievement( pcUser, ACHIEVEMENTTYPE_BattleRoyaleLastStandingM, ValueTaskAchievement( pcUser->pcUserData->sCharacterData.iClass ), ValueTaskAchievement( 0 ), ValueTaskAchievement( 0 ) );
			break;

		default:
			break;
	}

	return bRet;
}

void CAchievementHandler::ProcessPacket( User * pcUser, PacketAchievement * psPacket )
{
	psPacket->iLength = sizeof( PacketAchievement );
	psPacket->iHeader = PKTHDR_Achievement;
	SENDPACKET( pcUser, psPacket );
}

void CAchievementHandler::ProcessPacket( User * pcUser, PacketAchievementUser * psPacket )
{
	psPacket->iLength = sizeof( PacketAchievementUser );
	psPacket->iHeader = PKTHDR_AchievementUser;
	SENDPACKET( pcUser, psPacket );
}

void CAchievementHandler::HandlePacket( User * pcUser, PacketGetUnitData * psPacket )
{
	for ( UINT i = 0; i < USERSERVER->uUsersInGame; i++ )
	{
		User * pcUserTarget = USERSERVER->pcaUserInGame[i];
		if ( pcUserTarget->GetID() == psPacket->iID )
		{
			for ( auto & sPacket : pcUserTarget->vAchievement )
			{
				sPacket.iID = pcUserTarget->GetID();

				ProcessPacket( pcUser, &sPacket );
			}
			break;
		}
	}
}

BOOL CAchievementHandler::SQLHaveAchievement( int iAccountID, int iCharacterID, EAchievementType iType )
{
	BOOL bRet = FALSE;

	int iAchievementID = GetAchievementID( iType );

	//Handle info about Achievement of that account
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT [ID], [CharacterID] FROM UserAchievement WHERE (AccountID=?) AND (AchievementID=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iAccountID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iAchievementID );
			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
					int iCharacterUserID = -1;

					pcDB->GetData( 2, PARAMTYPE_Integer, &iCharacterUserID );

					if ( iCharacterUserID == 0 || (iCharacterUserID == iCharacterID) )
					{
						bRet = TRUE;
						break;
					}
				}
			}
		}
		pcDB->Close();
	}

	return bRet;
}

BOOL CAchievementHandler::SQLNewAchievement( User * pcUser, EAchievementType iType, ValueTaskAchievement & sValue1, ValueTaskAchievement & sValue2, ValueTaskAchievement & sValue3 )
{
	BOOL bRet = FALSE;

	int iAchievementID = GetAchievementID( iType );

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
	if ( pcDB->Open() )
	{
		if ( IsCharacterAchievement( iType ) )
		{
			if ( pcDB->Prepare( "INSERT INTO UserAchievement([AccountID], [CharacterID], [AchievementID], [Level], [Value1], [Value2], [Value3], [Date]) VALUES(?,?,?,1,?,?,?,GETDATE())" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iAccountID );
				pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &pcUser->iCharacterID );
				pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &iAchievementID );
				pcDB->BindParameterInput( 4, PARAMTYPE_String, (char*)sValue1.GetString(), STRLEN( sValue1.GetString() ) );
				pcDB->BindParameterInput( 5, PARAMTYPE_String, (char*)sValue2.GetString(), STRLEN( sValue2.GetString() ) );
				pcDB->BindParameterInput( 6, PARAMTYPE_String, (char*)sValue3.GetString(), STRLEN( sValue3.GetString() ) );

				pcDB->Execute();

				bRet = TRUE;
			}
		}
		else
		{
			if ( pcDB->Prepare( "INSERT INTO UserAchievement([AccountID], [CharacterID], [AchievementID], [Level], [Value1], [Value2], [Value3], [Date]) VALUES(?,NULL,?,1,?,?,?,GETDATE())" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iAccountID );
				pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iAchievementID );
				pcDB->BindParameterInput( 3, PARAMTYPE_String, (char*)sValue1.GetString(), STRLEN( sValue1.GetString() ) );
				pcDB->BindParameterInput( 4, PARAMTYPE_String, (char*)sValue2.GetString(), STRLEN( sValue2.GetString() ) );
				pcDB->BindParameterInput( 5, PARAMTYPE_String, (char*)sValue3.GetString(), STRLEN( sValue3.GetString() ) );

				pcDB->Execute();

				bRet = TRUE;
			}
		}
		pcDB->Close();
	}

	return bRet;
}

BOOL CAchievementHandler::SQLUpdateAchievement( User * pcUser, EAchievementType iType, BOOL bIncreaseLevel, ValueTaskAchievement & sValue1, ValueTaskAchievement & sValue2, ValueTaskAchievement & sValue3 )
{
	BOOL bRet = FALSE;

	int iAchievementID	= GetAchievementID( iType );
	int iLevel			= bIncreaseLevel ? 1 : 0;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
	if ( pcDB->Open() )
	{
		if ( IsCharacterAchievement( iType ) )
		{
			if ( pcDB->Prepare( "UPDATE UserAchievement SET [Level]=[Level]+?, [Value1]=?, [Value2]=?, [Value3]=?, [Date]=GETDATE() WHERE (AccountID=?) AND (AchievementID=?)" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iLevel );
				pcDB->BindParameterInput( 2, PARAMTYPE_String, (char*)sValue1.GetString(), STRLEN( sValue1.GetString() ) );
				pcDB->BindParameterInput( 3, PARAMTYPE_String, (char*)sValue2.GetString(), STRLEN( sValue2.GetString() ) );
				pcDB->BindParameterInput( 4, PARAMTYPE_String, (char*)sValue3.GetString(), STRLEN( sValue3.GetString() ) );
				pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &pcUser->iAccountID );
				pcDB->BindParameterInput( 6, PARAMTYPE_Integer, &iAchievementID );

				pcDB->Execute();

				bRet = TRUE;
			}
		}
		else
		{
			if ( pcDB->Prepare( "UPDATE UserAchievement SET [Level]=[Level]+?, [Value1]=?, [Value2]=?, [Value3]=?, [Date]=GETDATE() WHERE (AccountID=?) AND (CharacterID=?) AND (AchievementID=?)" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iLevel );
				pcDB->BindParameterInput( 2, PARAMTYPE_String, (char*)sValue1.GetString(), STRLEN( sValue1.GetString() ) );
				pcDB->BindParameterInput( 3, PARAMTYPE_String, (char*)sValue2.GetString(), STRLEN( sValue2.GetString() ) );
				pcDB->BindParameterInput( 4, PARAMTYPE_String, (char*)sValue3.GetString(), STRLEN( sValue3.GetString() ) );
				pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &pcUser->iAccountID );
				pcDB->BindParameterInput( 6, PARAMTYPE_Integer, &pcUser->iCharacterID );
				pcDB->BindParameterInput( 7, PARAMTYPE_Integer, &iAchievementID );

				pcDB->Execute();

				bRet = TRUE;
			}
		}
		pcDB->Close();
	}

	return bRet;
}

BOOL CAchievementHandler::UpdateAchievement( User * pcUser, EAchievementType iType, BOOL bIncreaseLevel, ValueTaskAchievement & sValue1, ValueTaskAchievement & sValue2, ValueTaskAchievement & sValue3 )
{
	BOOL bRet = FALSE;

	int iAchievementID = GetAchievementID( iType );

	for ( auto & sPacket : pcUser->vAchievement )
	{
		if ( sPacket.iAchievementID == iAchievementID )
		{
			sPacket.iID		= pcUser->GetID();
			sPacket.iLevel += bIncreaseLevel ? 1 : 0;
			STRINGCOPY( sPacket.szaValue[0], sValue1.GetString() );
			STRINGCOPY( sPacket.szaValue[1], sValue2.GetString() );
			STRINGCOPY( sPacket.szaValue[2], sValue3.GetString() );
			CopyMemory( &sPacket.sDate, GetServerTime(), sizeof( SYSTEMTIME ) );

			ProcessPacket( pcUser, &sPacket );

			bRet = TRUE;
			break;
		}
	}

	if ( bRet == FALSE )
	{
		PacketAchievementUser sPacket;
		ZeroMemory( &sPacket, sizeof( PacketAchievementUser ) );
		sPacket.iID				= pcUser->GetID();
		sPacket.iAchievementID	= iAchievementID;
		sPacket.iLevel			= 1;
		STRINGCOPY( sPacket.szaValue[0], sValue1.GetString() );
		STRINGCOPY( sPacket.szaValue[1], sValue2.GetString() );
		STRINGCOPY( sPacket.szaValue[2], sValue3.GetString() );
		CopyMemory( &sPacket.sDate, GetServerTime(), sizeof( SYSTEMTIME ) );

		pcUser->vAchievement.push_back( sPacket );
		
		ProcessPacket( pcUser, &sPacket );

		bRet = TRUE;
	}

	return bRet;
}

int CAchievementHandler::GetAchievementID( EAchievementType iType )
{
	int iRet = 0;

	if ( vAchievements.size() > 0 )
	{
		for ( std::vector<CAchievement*>::iterator it = vAchievements.begin(); it != vAchievements.end(); it++ )
		{
			CAchievement * pcAchievement = (*it);
			
			//Get Events of that Achievement
			for ( std::vector<CAchievementEvent*>::iterator itE = pcAchievement->GetEvents().begin(); itE != pcAchievement->GetEvents().end(); itE++ )
			{
				CAchievementEvent * pcAchievementEvent = (*itE);

				if ( pcAchievementEvent->GetType() == iType )
				{
					iRet = pcAchievement->GetID();
					break;
				}
			}
		}
	}

	return iRet;
}

BOOL CAchievementHandler::IsLevelAchievement( EAchievementType iType )
{
	BOOL bRet = FALSE;

	if ( vAchievements.size() > 0 )
	{
		for ( std::vector<CAchievement*>::iterator it = vAchievements.begin(); it != vAchievements.end(); it++ )
		{
			CAchievement * pcAchievement = (*it);

			//Get Events of that Achievement
			for ( std::vector<CAchievementEvent*>::iterator itE = pcAchievement->GetEvents().begin(); itE != pcAchievement->GetEvents().end(); itE++ )
			{
				CAchievementEvent * pcAchievementEvent = (*itE);

				if ( pcAchievementEvent->GetType() == iType )
				{
					bRet = pcAchievementEvent->IsLevelAchievement();
					break;
				}
			}
		}
	}

	return bRet;
}

BOOL CAchievementHandler::IsCharacterAchievement( EAchievementType iType )
{
	BOOL bRet = FALSE;

	if ( vAchievements.size() > 0 )
	{
		for ( std::vector<CAchievement*>::iterator it = vAchievements.begin(); it != vAchievements.end(); it++ )
		{
			CAchievement * pcAchievement = (*it);

			//Get Events of that Achievement
			for ( std::vector<CAchievementEvent*>::iterator itE = pcAchievement->GetEvents().begin(); itE != pcAchievement->GetEvents().end(); itE++ )
			{
				CAchievementEvent * pcAchievementEvent = (*itE);

				if ( pcAchievementEvent->GetType() == iType )
				{
					bRet = pcAchievementEvent->IsCharacterAchievement();
					break;
				}
			}
		}
	}

	return bRet;
}
