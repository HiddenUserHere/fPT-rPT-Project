#include "stdafx.h"
#include "characterserver.h"
#include "LevelTable.h"
typedef UINT( __cdecl *t_GetCodebyName ) ( char * nick );
t_GetCodebyName GetCodebyName = ( t_GetCodebyName )0x00415F80;



CharacterServer::CharacterServer()
{
}

CharacterServer::~CharacterServer()
{
}

BOOL CharacterServer::SQLGetCharacter( int iCharacterID, SQLCharacter * ps )
{
	if ( iCharacterID == (-1) )
		return FALSE;

	if ( ps == NULL )
		return FALSE;

	BOOL bRet = FALSE;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT [ID],[AccountName],[Name],[Level],[Experience],[Gold],[JobCode],[ClanID],[ClanPermission],[LastSeenDate],[LastStage] FROM CharacterInfo WHERE ID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iCharacterID );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_Integer, &ps->iID );
				pcDB->GetData( 2, PARAMTYPE_String, ps->szAccountName, _countof( ps->szAccountName ) );
				pcDB->GetData( 3, PARAMTYPE_String, ps->szCharacterName, _countof( ps->szCharacterName ) );
				pcDB->GetData( 4, PARAMTYPE_Integer, &ps->iLevel );
				pcDB->GetData( 5, PARAMTYPE_Int64, &ps->iEXP );
				pcDB->GetData( 6, PARAMTYPE_Integer, &ps->iGold );
				pcDB->GetData( 7, PARAMTYPE_Integer, &ps->iClass );
				pcDB->GetData( 8, PARAMTYPE_Integer, &ps->iClanID );
				pcDB->GetData( 9, PARAMTYPE_Integer, &ps->iClanPermission );
				pcDB->GetData( 10, PARAMTYPE_Time, &ps->sLastSeenDate );
				pcDB->GetData( 11, PARAMTYPE_Integer, &ps->iLastMapID );

				bRet = TRUE;
			}
		}

		pcDB->Close();
	}

	return bRet;
}

BOOL CharacterServer::SQLGetCharacter( const char * pszCharacterName, SQLCharacter * ps )
{
	if ( pszCharacterName == NULL || pszCharacterName[0] == 0 )
		return FALSE;

	if ( ps == NULL )
		return FALSE;

	BOOL bRet = FALSE;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
	
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT [ID],[AccountName],[Name],[Level],[Experience],[Gold],[JobCode],[ClanID],[ClanPermission],[LastSeenDate],[LastStage] FROM CharacterInfo WHERE Name=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, (char*)pszCharacterName );
			
			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_Integer, &ps->iID );
				pcDB->GetData( 2, PARAMTYPE_String, ps->szAccountName, _countof( ps->szAccountName ) );
				pcDB->GetData( 3, PARAMTYPE_String, ps->szCharacterName, _countof( ps->szCharacterName ) );
				pcDB->GetData( 4, PARAMTYPE_Integer, &ps->iLevel );
				pcDB->GetData( 5, PARAMTYPE_Int64, &ps->iEXP );
				pcDB->GetData( 6, PARAMTYPE_Integer, &ps->iGold );
				pcDB->GetData( 7, PARAMTYPE_Integer, &ps->iClass );
				pcDB->GetData( 8, PARAMTYPE_Integer, &ps->iClanID );
				pcDB->GetData( 9, PARAMTYPE_Integer, &ps->iClanPermission );
				pcDB->GetData( 10, PARAMTYPE_Time, &ps->sLastSeenDate );
				pcDB->GetData( 11, PARAMTYPE_Integer, &ps->iLastMapID );

				bRet = TRUE;
			}
		}

		pcDB->Close();
	}

	return bRet;
}

int CharacterServer::SQLGetCharacterClanID( char * pszCharacterName )
{
	int iID = -1;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT ClanID FROM CharacterInfo WHERE Name=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, pszCharacterName, STRLEN( pszCharacterName ) );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_Integer, &iID );
			}
		}

		pcDB->Close();
	}

	return iID;
}

void CharacterServer::SQLRenameCharacter( char * pszCharacterName, char * pszNewCharacterName )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE CharacterInfo SET Name=? WHERE Name=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, pszNewCharacterName, 32 );
			pcDB->BindParameterInput( 2, PARAMTYPE_String, pszCharacterName, 32 );

			pcDB->Execute();
		}

		pcDB->Clear();

		if ( pcDB->Prepare( "UPDATE CharacterItemTimer SET CharacterName=? WHERE CharacterName=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, pszNewCharacterName, 32 );
			pcDB->BindParameterInput( 2, PARAMTYPE_String, pszCharacterName, 32 );

			pcDB->Execute();
		}

		pcDB->Clear();

		if ( pcDB->Prepare( "UPDATE CharacterQuest SET CharacterName=? WHERE CharacterName=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, pszNewCharacterName, 32 );
			pcDB->BindParameterInput( 2, PARAMTYPE_String, pszCharacterName, 32 );

			pcDB->Execute();
		}

		pcDB->Close();
	}

	pcDB = SQLCONNECTION( DATABASEID_ClanDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE ClanList SET ClanLeader=? WHERE ClanLeader=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, pszNewCharacterName, 32 );
			pcDB->BindParameterInput( 2, PARAMTYPE_String, pszCharacterName, 32 );

			pcDB->Execute();
		}

		pcDB->Clear();

		if ( pcDB->Prepare( "UPDATE BellatraPersonalScore SET CharacterName=? WHERE CharacterName=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, pszNewCharacterName, 32 );
			pcDB->BindParameterInput( 2, PARAMTYPE_String, pszCharacterName, 32 );

			pcDB->Execute();
		}

		pcDB->Clear();

		if ( pcDB->Prepare( "UPDATE BellatraTeamScore SET CharacterName=? WHERE CharacterName=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, pszNewCharacterName, 32 );
			pcDB->BindParameterInput( 2, PARAMTYPE_String, pszCharacterName, 32 );

			pcDB->Execute();
		}

		pcDB->Close();
	}
}

void CharacterServer::SQLChangeCharacterLevel( char * pszCharacterName, int iLevel, INT64 iEXP )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE CharacterInfo SET [Level]=?,[Experience]=? WHERE Name=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iLevel );
			pcDB->BindParameterInput( 2, PARAMTYPE_Int64, &iEXP );
			pcDB->BindParameterInput( 3, PARAMTYPE_String, pszCharacterName, 32 );

			pcDB->Execute();
		}

		pcDB->Close();
	}
}

void CharacterServer::SQLChangeCharacterClass( char * pszCharacterName, int iClass )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE CharacterInfo SET [JobCode]=? WHERE Name=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iClass );
			pcDB->BindParameterInput( 2, PARAMTYPE_String, pszCharacterName, 32 );

			pcDB->Execute();
		}

		pcDB->Close();
	}
}


char * CharacterServer::GetCharacterName( UserData * pcUserData )
{
	if ( LOGIN_SERVER && (GAME_SERVER == FALSE) )
		return pcUserData->szCharacterName;

	return pcUserData->sCharacterData.szName;
}

bool CharacterServer::GetCharacterExFilePath( const char * pszCharacterName, char * pszBuffer, int iBufferLen )
{
	if( StringCbPrintfA( pszBuffer, iBufferLen, "Data\\Character\\%s.chd", pszCharacterName ) == S_OK )
		return true;

	return false;
}

bool CharacterServer::GetCharacterFilePath( const char * pszCharacterName, char * pszBuffer, int iBufferLen )
{
	if ( StringCbPrintfA( pszBuffer, iBufferLen, "Data\\Character\\%s.chr", pszCharacterName ) == S_OK )
		return true;

	return false;
}

bool CharacterServer::GetCharacterBroyaleFilePath( const char * pszCharacterName, char * pszBuffer, int iBufferLen )
{
	if ( StringCbPrintfA( pszBuffer, iBufferLen, "Data\\CharacterBroyale\\%s.chr", pszCharacterName ) == S_OK )
		return true;

	return false;
}

DWORD CharacterServer::GetClanCode( int iClassClan )
{
	DWORD	dwCode,dwCode1,dwCode2;

	dwCode = iClassClan % 1000000;
	dwCode1 = iClassClan / 100000000;
	dwCode2 = dwCode1 * 100000000;

	return dwCode + dwCode2;
}

void CharacterServer::SendBlessCastleSkill( UserData * pcUserData )
{
	typedef void( __cdecl *tfnSendBlessClastleSkill )(UserData * pcUserData);
	tfnSendBlessClastleSkill fnSendBlessClastleSkill = (tfnSendBlessClastleSkill)0x00557F10;
	if ( LOGIN_SERVER )
		fnSendBlessClastleSkill( pcUserData );
}

int CharacterServer::ChangeCharacterName( char * pszCharacterName, char * pszNewCharacterName )
{
	if ( STRLEN( pszCharacterName ) >= 32 || STRLEN( pszNewCharacterName ) >= 32 )
		return -2;

	UserData * pcUserData = USERDATABYNAME( pszCharacterName );

	if ( pcUserData != NULL )
	{
		if ( pcUserData->pcSocketData )
		{
			if ( pcUserData->pcSocketData->bConnected )
				return -1;

			LOGSERVER->LogSocketClose( pcUserData->pcSocketData, "CharacterServer::ChangeCharacterName()", _ReturnAddress() );
			WNDPROC_SOCKETCLOSE( SOCKETSERVER->GetHWND(), pcUserData->pcSocketData );
		}
	}

	SQLCharacter s;

	if ( CharacterServer::SQLGetCharacter( pszCharacterName, &s ) == FALSE )
		return 1;

	if ( CharacterServer::SQLGetCharacterClanID( pszNewCharacterName ) != -1 )
		return 2;

	char szCharacterFilePath[MAX_PATH];
	GetCharacterFilePath( pszCharacterName, szCharacterFilePath, _countof( szCharacterFilePath ) );

	char szNewCharacterFilePath[MAX_PATH];
	GetCharacterFilePath( pszNewCharacterName, szNewCharacterFilePath, _countof( szNewCharacterFilePath ) );

	if ( CopyFileA( szCharacterFilePath, szNewCharacterFilePath, FALSE ) == FALSE )
		return 3;

	DeleteFileA( szCharacterFilePath );

	FILE * pFile;
	if ( fopen_s( &pFile, szNewCharacterFilePath, "rb+" ) == 0 )
	{
		fseek( pFile, 0x10, SEEK_SET );
		fwrite( pszNewCharacterName, 1, lstrlenA( pszNewCharacterName ) + 1, pFile );
		fflush( pFile );
		fclose( pFile );

		SQLRenameCharacter( pszCharacterName, pszNewCharacterName );
	}

	return 0;
}

int CharacterServer::ChangeCharacterLevel( char * pszCharacterName, int iLevel )
{
	if ( STRLEN( pszCharacterName ) >= 32 )
		return -2;

	if ( iLevel < 1 || iLevel > SERVER_LEVEL_MAX )
		return -1;

	SQLCharacter s;

	if ( CharacterServer::SQLGetCharacter( pszCharacterName, &s ) == FALSE )
		return 1;

	char szCharacterFilePath[MAX_PATH];
	GetCharacterFilePath( pszCharacterName, szCharacterFilePath, _countof( szCharacterFilePath ) );

	//EXP
	LARGE_INTEGER liEXP;
	liEXP.QuadPart = GetExpFromLevel( iLevel );

	User * pc = USERDATATOUSER( USERDATABYNAME( pszCharacterName ) );

	if ( pc == NULL )
	{

		FILE * pFile;
		if ( fopen_s( &pFile, szCharacterFilePath, "rb+" ) == 0 )
		{
			fseek( pFile, 0xC8, SEEK_SET );
			fwrite( &iLevel, 1, 4, pFile );
			fseek( pFile, 0x14C, SEEK_SET );
			fwrite( &liEXP.LowPart, 1, 4, pFile );
			fseek( pFile, 0x194, SEEK_SET );
			fwrite( &liEXP.HighPart, 1, 4, pFile );
			fflush( pFile );
			fclose( pFile );

			SQLChangeCharacterLevel( pszCharacterName, iLevel, liEXP.QuadPart );
		}
	}
	else
	{
		GiveEXP( pc, liEXP.QuadPart - GetExp( pc->pcUserData ) );
	}

	return 0;
}

int CharacterServer::ChangeCharacterClass( char * pszCharacterName, int iClass )
{
	if ( STRLEN( pszCharacterName ) >= 32 )
		return -2;

	if ( iClass < 1 || iClass > 12 )
		return -1;

	SQLCharacter s;

	if ( CharacterServer::SQLGetCharacter( pszCharacterName, &s ) == FALSE )
		return -3;

	char szCharacterFilePath[MAX_PATH];
	GetCharacterFilePath( pszCharacterName, szCharacterFilePath, _countof( szCharacterFilePath ) );

	User * pc = USERDATATOUSER( USERDATABYNAME( pszCharacterName ) );

	if ( pc == NULL )
	{

		FILE * pFile;
		if ( fopen_s( &pFile, szCharacterFilePath, "rb+" ) == 0 )
		{
			int iRank = 0;

			fseek( pFile, 0x184, SEEK_SET );
			fread( &iRank, 1, 4, pFile );


			char szNewHeadModel[64] = { 0 };
			char cTier[2] = { iRank ? 0x60 + (char)iRank : 0, 0 };
			char cChar[2] = { iRank == 2 ? '_' : '-', 0 };

			switch ( iClass )
			{
				case CHARACTERCLASS_Fighter:
					STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Tmh%sB01%s.inf", cChar, cTier );
					break;

				case CHARACTERCLASS_Mechanician:
					STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Tmh%sA01%s.inf", cChar, cTier );
					break;

				case CHARACTERCLASS_Archer:
					STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Tfh%sD01%s.inf", cChar, cTier );
					break;

				case CHARACTERCLASS_Pikeman:
					STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Tmh%sC01%s.inf", cChar, cTier );
					if ( iRank == 3 )
						STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\TmhC-01c.inf" );
					break;

				case CHARACTERCLASS_Atalanta:
					STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Mfh%sB01%s.inf", cChar, cTier );
					break;

				case CHARACTERCLASS_Knight:
					STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Mmh%sA01%s.inf", cChar, cTier );
					break;

				case CHARACTERCLASS_Magician:
					STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Mmh%sD01%s.inf", cChar, cTier );
					break;

				case CHARACTERCLASS_Priestess:
					STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Mfh%sC01%s.inf", cChar, cTier );
					break;

				case CHARACTERCLASS_Assassin:
					STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Tfh%sE01%s.inf", cChar, cTier );
					break;

				case CHARACTERCLASS_Shaman:
					STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Mmh%sE01%s.inf", cChar, cTier );
					break;

				default:
					break;
			}

			int iOldClass = 0;
			
			fseek( pFile, 0xC4, SEEK_SET );
			fread( &iOldClass, 1, 4, pFile );
			
			fseek( pFile, 0xC4, SEEK_SET );
			fwrite( &iClass, 1, 4, pFile );
			fflush( pFile );
			
			fseek( pFile, 0x70, SEEK_SET );
			fwrite( szNewHeadModel, 1, 64, pFile );
			fflush( pFile );
			
			fclose( pFile );

			SQLChangeCharacterClass( pszCharacterName, iClass );

			//Quest Handler
			ECharacterRace iOldRace = GetCharacterRace( (ECharacterClass)iOldClass );
			ECharacterRace iNewRace = GetCharacterRace( (ECharacterClass)iClass );


			if ( (iOldRace == CHARACTERRACE_Tempskron) && (iNewRace == CHARACTERRACE_Morion) )
			{
				QUESTSERVER->SQLQuestSwapClass( pszCharacterName, 1601, 1606 );
				QUESTSERVER->SQLQuestSwapClass( pszCharacterName, 1602, 1607 );
				QUESTSERVER->SQLQuestSwapClass( pszCharacterName, 1603, 1608 );
				QUESTSERVER->SQLQuestSwapClass( pszCharacterName, 1604, 1609 );
				QUESTSERVER->SQLQuestSwapClass( pszCharacterName, 1605, 1610 );
				QUESTSERVER->SQLQuestSwapClass( pszCharacterName, 1640, 1642 );
				QUESTSERVER->SQLQuestSwapClass( pszCharacterName, 1641, 1643 );
				QUESTSERVER->SQLQuestSwapClass( pszCharacterName, 1800, 1802 );
				QUESTSERVER->SQLQuestSwapClass( pszCharacterName, 1801, 1803 );
				QUESTSERVER->SQLQuestSwapClass( pszCharacterName, 1648, 1650 );
				QUESTSERVER->SQLQuestSwapClass( pszCharacterName, 1649, 1651 );

				if ( (iClass == CHARACTERCLASS_Magician) || (iClass == CHARACTERCLASS_Priestess) || (iClass == CHARACTERCLASS_Shaman) )
					QUESTSERVER->SQLQuestSwapClass( pszCharacterName, 1621, 1622 );
			}
			else if ( (iOldRace == CHARACTERRACE_Morion) && (iNewRace == CHARACTERRACE_Tempskron) )
			{
				QUESTSERVER->SQLQuestSwapClass( pszCharacterName, 1606, 1601 );
				QUESTSERVER->SQLQuestSwapClass( pszCharacterName, 1607, 1602 );
				QUESTSERVER->SQLQuestSwapClass( pszCharacterName, 1608, 1603 );
				QUESTSERVER->SQLQuestSwapClass( pszCharacterName, 1609, 1604 );
				QUESTSERVER->SQLQuestSwapClass( pszCharacterName, 1610, 1605 );
				QUESTSERVER->SQLQuestSwapClass( pszCharacterName, 1642, 1640 );
				QUESTSERVER->SQLQuestSwapClass( pszCharacterName, 1643, 1641 );
				QUESTSERVER->SQLQuestSwapClass( pszCharacterName, 1802, 1800 );
				QUESTSERVER->SQLQuestSwapClass( pszCharacterName, 1803, 1801 );
				QUESTSERVER->SQLQuestSwapClass( pszCharacterName, 1650, 1648 );
				QUESTSERVER->SQLQuestSwapClass( pszCharacterName, 1651, 1649 );

				if ( (iOldClass == CHARACTERCLASS_Magician) || (iOldClass == CHARACTERCLASS_Priestess) || (iOldClass == CHARACTERCLASS_Shaman) )
					QUESTSERVER->SQLQuestSwapClass( pszCharacterName, 1622, 1621 );
			}
			else if ( (iOldRace == CHARACTERRACE_Morion) && (iNewRace == CHARACTERRACE_Morion) )
			{
				if ( (iOldClass == CHARACTERCLASS_Magician) || (iOldClass == CHARACTERCLASS_Priestess) || (iOldClass == CHARACTERCLASS_Shaman) )
				{
					if ( (iClass == CHARACTERCLASS_Atalanta) || (iClass == CHARACTERCLASS_Knight) )
						QUESTSERVER->SQLQuestSwapClass( pszCharacterName, 1622, 1621 );
				}
				else if ( (iOldClass == CHARACTERCLASS_Atalanta) || (iOldClass == CHARACTERCLASS_Knight) )
				{
					if ( (iClass == CHARACTERCLASS_Magician) || (iClass == CHARACTERCLASS_Priestess) || (iClass == CHARACTERCLASS_Shaman) )
						QUESTSERVER->SQLQuestSwapClass( pszCharacterName, 1621, 1622 );
				}
			}
		}
	}
	else
	{
		return -4;
	}

	return 0;
}

bool CharacterServer::KickCharacterName( char * pszCharacterName )
{
	UserData * pcUserKicked = USERSERVER->GetUserdata( pszCharacterName );
	if ( pcUserKicked )
	{
		LOGSERVER->OnLogEx( USERDATATOUSER( pcUserKicked ), ACCLOGID_CharacterNameKicked, "Character[%s] Disconnected by UserPanel", CHARACTERSERVER->GetCharacterName( USERDATATOUSER( pcUserKicked )->pcUserData ) );

		CHATSERVER->SendChat( USERDATATOUSER( pcUserKicked ), CHATCOLOR_Error, "> A disconnect has been initiated through the User Panel, closing game..." );

		ACCOUNTSERVER->OnDisconnectUser( pcUserKicked );
		SENDPACKETBLANK( USERDATATOUSER( pcUserKicked ), PKTHDR_Disconnect, TRUE );
		pcUserKicked->dwDisconnectTime = TICKCOUNT + 1500;
		NETSERVER->DisconnectUser( pcUserKicked );
	}

	return true;
}

BOOL CharacterServer::SetCharacterEXPOnDisconnect( char * pszCharacterName, INT64 iEXP )
{
	BOOL bRet = FALSE;
	
	char szCharacterFilePath[MAX_PATH];
	GetCharacterFilePath( pszCharacterName, szCharacterFilePath, _countof( szCharacterFilePath ) );

	//EXP
	LARGE_INTEGER liEXP;
	liEXP.QuadPart = iEXP;

	User * pc = USERDATATOUSER( USERDATABYNAME( pszCharacterName ) );

	FILE * pFile;
	if ( fopen_s( &pFile, szCharacterFilePath, "rb+" ) == 0 )
	{
		int iLevel = 0;
		fseek( pFile, 0x0C8, SEEK_SET );
		fread( &iLevel, 1, 4, pFile );		
		fclose( pFile );

		SQLChangeCharacterLevel( pszCharacterName, iLevel, liEXP.QuadPart );
		bRet = TRUE;
	}

	return bRet;
}

#define MAX_BATTLEROYALE_FILETIMECOPY	(7 * 60)
BOOL CharacterServer::CopyCharacterBattleRoyale( const char * pszCharacterName )
{
	if ( (pszCharacterName == nullptr) || (pszCharacterName[0] == 0) )
		return FALSE;

	char szCharacterFilePath[MAX_PATH];
	GetCharacterFilePath( pszCharacterName, szCharacterFilePath, _countof( szCharacterFilePath ) );

	char szCharacterBroyaleFilePath[MAX_PATH];
	GetCharacterBroyaleFilePath( pszCharacterName, szCharacterBroyaleFilePath, _countof( szCharacterBroyaleFilePath ) );

	SYSTEMTIME sTime;

	bool bCopy = false;
	if ( GetFileLastModifiedSystemTime( szCharacterBroyaleFilePath, sTime ) )
	{
		//File ins't on 'Waiting Time'? So we can copy this to backup
		if ( (SystemTimeToQWORD( GetServerTime() ) - SystemTimeToQWORD( &sTime )) >= MAX_BATTLEROYALE_FILETIMECOPY )
			bCopy = true;
	}
	else
		bCopy = true;

	if ( bCopy )
		CopyFile( szCharacterFilePath, szCharacterBroyaleFilePath, FALSE );

	return TRUE;
}

void CharacterServer::SQLUpdateCharacter( const char * pszCharacterName, int iCharacterClass, int iCharacterLevel, INT64 iCharacterExperience, int iCharacterGold )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE CharacterInfo SET Level=?, JobCode=?, Experience=?, Gold=? WHERE Name=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iCharacterLevel );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iCharacterClass );
			pcDB->BindParameterInput( 3, PARAMTYPE_Int64, &iCharacterExperience );
			pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &iCharacterGold );
			pcDB->BindParameterInput( 5, PARAMTYPE_String, (void*)pszCharacterName );
			pcDB->Execute();
		}
		pcDB->Close();
	}
}

void CharacterServer::SQLUpdateDateLevelUP( User * pcUser )
{
	return;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE CharacterInfo SET DateLevelUP=? WHERE Name=?" ) )
		{
			SYSTEMTIME & sDate = pcUser->sDateLevelUP;

			pcDB->BindParameterInput( 1, PARAMTYPE_Time, &sDate, sizeof( SYSTEMTIME ) );
			pcDB->BindParameterInput( 2, PARAMTYPE_String, pcUser->pcUserData->sCharacterData.szName, STRLEN( pcUser->pcUserData->sCharacterData.szName ) );
			pcDB->Execute();
		}
		pcDB->Close();
	}
}

int CharacterServer::SQLGetCharacterClass( const char * pszCharacterName )
{
	int iClass = 0;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT JobCode FROM CharacterInfo WHERE Name=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, (void*)pszCharacterName, STRLEN( pszCharacterName ) );
			
			if ( pcDB->Execute() && pcDB->Fetch() )
				pcDB->GetData( 1, PARAMTYPE_Integer, &iClass );
		}
		pcDB->Close();
	}

	return iClass;
}

int CharacterServer::SQLGetCharacterClass( int iCharacterID )
{
    int iClass = 0;

    SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );

    if ( pcDB->Open() )
    {
        if ( pcDB->Prepare( "SELECT JobCode FROM CharacterInfo WHERE ID=?" ) )
        {
            pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iCharacterID );

            if ( pcDB->Execute() && pcDB->Fetch() )
                pcDB->GetData( 1, PARAMTYPE_Integer, &iClass );
        }
        pcDB->Close();
    }

    return iClass;
}

BOOL CharacterServer::UpdateForceOrb( UserData * pcUserData )
{
	User * pcUser = USERDATATOUSER( pcUserData );
	if ( pcUserData && pcUserData->iID )
	{
		if ( GAME_SERVER )
		{
			if ( !CharacterInCityStage( pcUserData ) )
			{
				// Send that now is in city stage
				if ( !pcUser->dwLastForceTime )
				{
					pcUser->dwLastForceTime = TICKCOUNT;
				}

				// Update Time? update force
				if ( pcUserData->ForceOrbTime > 0 )
				{
					DWORD dwTime = (TICKCOUNT - pcUser->dwLastForceTime);
					pcUserData->ForceOrbTime			-= 1000;
					if ( pcUserData->ForceOrbTime <= 2 )
						pcUserData->ForceOrbTime = 0;
					pcUser->dwLastForceTime = TICKCOUNT;
					if ( (pcUserData->ForceOrbTime % 2000) == 0 )
						NETSERVER->SendForceOrb( pcUserData, TRUE );
				}
				return TRUE;
			}
		}
	}
	return TRUE;
}

BOOL CharacterServer::UpdatePremiumTimer( UserData * pcUserData )
{
	User * pcUser = USERDATATOUSER( pcUserData );
	if ( pcUserData && pcUserData->pcSocketData )
	{
		DWORD dwLag = TICKCOUNT - pcUserData->dwTimeLastPacket;
		User * pcUser = USERDATATOUSER( pcUserData );
		if ( GAME_SERVER && dwLag <= 10000 )
		{
			if ( !CharacterInCityStage( pcUserData ) )
			{
				if ( !pcUser->dwLastPremiumTime )
					pcUser->dwLastPremiumTime = TICKCOUNT;

				if ( true ) //if ( (TICKCOUNT - pcUser->dwLastPremiumTime) > 1000 )
				{
					for ( int i = 0; i < PREMIUM_ITEM_TOTAL; i++ )
					{
						ItemPremium * psItem = &pcUser->sPremiumData.saItemList[i];

						if ( ITEMSERVER->IsItemTimerFreezedMap( pcUser->GetMapID(), psItem->eType ) )
							continue;

						if ( psItem->dwCode && psItem->dwTimeLeft )
						{
							if ( psItem->dwTimeLeft <= 2 )
							{
								psItem->dwTimeLeft = 0;

								//ITEMSERVER->UpdateItemTimerEXP( pcUser, psItem->eType, true );

								ITEMSERVER->UpdateItemTimer( pcUser );
							}
							else
							{
								psItem->dwTimeLeft -= 1;
							}
						}
					}
					pcUser->dwLastPremiumTime = TICKCOUNT;				
				}
			}
		}
	}
	return TRUE;
}

void CharacterServer::CheckGold( UserData * pcUserData )
{
	User * pcUser = USERDATATOUSER( pcUserData );
	if ( LOGIN_SERVER && pcUserData )
	{
		if ( pcUser->pData.dwGold != pcUserData->iInventoryGold && !pcUserData->bWarehouseOpen )
		{
			
		}
		if ( pcUserData->iInventoryGold == -150000000 )
		{
			LOGSERVER->LogSocketClose( pcUserData->pcSocketData, "CharacterServer::CheckGold()", _ReturnAddress() );
			WNDPROC_SOCKETCLOSE( SOCKETSERVER->GetHWND(), pcUserData->pcSocketData );
		}
	}
}

INT64 CharacterServer::GetExp( UserData * pcUserData )
{
	if ( pcUserData )
		return pcUserData->liEXP.QuadPart;
	return 0;
}

void CharacterServer::SetExpFromSQL( UserData * pcUserData )
{
	
}

INT64 CharacterServer::GetExpFromLevel( int iLevel )
{
	UINT uExpLow = *( UINT* )( ( ( iLevel - 1 ) * 8 ) + (DWORD)&LevelTable );
	UINT uExpHigh = *( UINT* )( ( ( iLevel - 1 ) * 8 ) + ((DWORD)&LevelTable + 4));
	return ( (INT64)uExpHigh << 32 ) | ( (INT64)uExpLow & 0xFFFFFFFF );
}

NAKED int CharacterServer::GetLevelFromEXP( INT64 iEXP )
{
	JMP( 0x00443A30 );
}

void CharacterServer::OnCharacterUpdate( UserData * pcUserData )
{

	char * pszCharacterName = GetCharacterName( pcUserData );

	if ( LOGIN_SERVER )
	{
		
	}
}

BOOL CharacterServer::CharacterInCityStage( UserData * pcUserData )
{
	if ( pcUserData )
	{
		if ( pcUserData->iMapID < 0 || pcUserData->iMapID > 80 )
			return TRUE;

		for ( int i = 0; i < _countof( aStageCity ); i++ )
		{
			if ( pcUserData->iMapID == aStageCity[ i ] )
				return TRUE;
		}
	}
	return FALSE;
}

BOOL CharacterServer::CharacterInBattleStage( UserData * pcUserData )
{
	if ( pcUserData )
	{
		for ( int i = 0; i < _countof( aStageBattle ); i++ )
		{
			if ( pcUserData->iMapID == aStageBattle[ i ] )
				return TRUE;
		}
	}
	return FALSE;
}

void CharacterServer::OnAddExp( UserData * pcUserData )
{

}

void CharacterServer::OnLoseExp( UserData * pcUserData, PacketLoseExperience * psPacket )
{
	int iLevelCap = SERVER_LEVEL_MAX;
	if ( pcUserData->iGameLevel >= GAMELEVEL_Two )
		iLevelCap = 155;

	// User alive and not in city maps and not in Bless Castle?
	if ( pcUserData && pcUserData->pcSocketData && psPacket->iLevel < iLevelCap )
	{
		// Is Hack or bug?
		if ( psPacket->iSubExp < 0 )
			return;

		INT64 iExpLevel = GetExpFromLevel( psPacket->iLevel );

		INT64 iExpCharacter = GetExp( pcUserData ) - psPacket->iSubExp;

		if ( iExpCharacter < iExpLevel )
			iExpCharacter = iExpLevel;

		SetExp( pcUserData, iExpCharacter );

		User * pcUser = USERDATATOUSER( pcUserData );

		//Log It
		LOGSERVER->OnLogEx( pcUser, ACCLOGID_CharacterQuestEXPLose, "Character[%s] Lose EXP[%I64d] MapID[%d] RessurectionItem[%s]",
			CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), psPacket->iSubExp, psPacket->iMapID, psPacket->bRessurectionItem ? "TRUE" : "FALSE" );
	}
}

void CharacterServer::SetExp( UserData * pcUserData, INT64 iExp )
{
	int iLevelCap = SERVER_LEVEL_MAX;
	if ( pcUserData->iGameLevel >= GAMELEVEL_Two )
		iLevelCap = 155;

	if ( pcUserData && LOGIN_SERVER )
	{
		pcUserData->liEXP.QuadPart = iExp;
		INT64 iExpMax = GetExpFromLevel( iLevelCap );

		if ( pcUserData->liEXP.QuadPart > iExpMax )
			pcUserData->liEXP.QuadPart = iExpMax;
	}
}

void CharacterServer::GiveEXP( User * pcUser, INT64 iExp )
{
	if ( pcUser )
	{
		INT64 iExpCharacter = GetExp( pcUser->pcUserData ) + iExp;
		INT64 iExpCharacterOld = GetExp( pcUser->pcUserData );

		if ( LOGIN_SERVER )
		{
			SetExp( pcUser->pcUserData, iExpCharacter );
			PacketExperience sPacket;
			ZeroMemory( &sPacket, sizeof( PacketExperience ) );
			sPacket.iLength = sizeof( PacketExperience );
			sPacket.iHeader = PKTHDR_AddExp;
			sPacket.iAddExp = GetExp( pcUser->pcUserData ) - iExpCharacterOld;
			SENDPACKET( pcUser, &sPacket );
		}
		else if ( GAME_SERVER )
			NETSERVER->GiveEXP( pcUser->pcUserData, iExp );
	}
}

BOOL CharacterServer::GiveGOLD( User * pcUser, int iGold, EWhereAction eWhere )
{
	BOOL bRet = FALSE;

	if ( LOGIN_SERVER )
	{
		if ( iGold > 0 )
			bRet = USERSERVER->AddServerUserGold( pcUser->pcUserData, iGold, eWhere );

		if ( iGold < 0 )
			bRet = USERSERVER->SubServerUserGold( pcUser->pcUserData, abs( iGold ), eWhere );

		if ( bRet )
		{
			PacketSetCharacterGold sPacket;
			sPacket.iLength = sizeof( PacketSetCharacterGold );
			sPacket.iHeader = PKTHDR_SetGold;
			sPacket.dwGold = pcUser->pcUserData->GetGold();
			SENDPACKET( pcUser, &sPacket );
		}
	}
	return bRet;
}

void CharacterServer::CharacterCreate( UserData * pcUserData, PacketCreateCharacter * psPacket )
{
	// Is Userdata?
	if ( pcUserData )
	{
		char szDataPath[ 256 ] = { 0 };
		STRINGFORMAT( szDataPath, "Data\\Character\\%s.chr", psPacket->szCharname );
		psPacket->iCode = FALSE;

		for ( int i = 0; i < lstrlenA( psPacket->szCharname ); i++ )
		{
			if ( psPacket->szCharname[i] >= 'a' && psPacket->szCharname[i] <= 'z' )
				continue;
			if ( psPacket->szCharname[i] >= 'A' && psPacket->szCharname[i] <= 'Z' )
				continue;
			if ( psPacket->szCharname[i] >= '0' && psPacket->szCharname[i] <= '9' )
				continue;

			SENDPACKET( USERDATATOUSER(pcUserData), psPacket );
			return;
		}

		BOOL bContinue		= TRUE;
		BOOL bFileExists	= FILEEXIST( szDataPath );
		{
			SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "SELECT ID FROM CharacterInfo WHERE Name=?" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_String, psPacket->szCharname, STRLEN( psPacket->szCharname ) );
					if ( pcDB->Execute() )
					{
						if ( pcDB->Fetch() )
						{
							bContinue = FALSE;
						}
					}
				}
				pcDB->Close();
			}

			//Have on SQL but don't exists on File? Delete it
			if ( !bContinue && !bFileExists )
			{
				if ( pcDB->Open() )
				{
					if ( pcDB->Prepare( "DELETE FROM CharacterInfo WHERE Name=?" ) )
					{
						pcDB->BindParameterInput( 1, PARAMTYPE_String, psPacket->szCharname, STRLEN( psPacket->szCharname ) );
						pcDB->Execute();
					}
					pcDB->Close();
				}
			}
		}

		// Character not exists?
		if ( !FILEEXIST( szDataPath ) && bContinue )
		{
			psPacket->iCode = TRUE;
			pcUserData->dwHashCharacterName = GetCodebyName( psPacket->szCharname );
			STRINGCOPY( pcUserData->szCharacterName, psPacket->szCharname );

			char szDateTime[ 20 ] = { 0 };
			GetDateTime( szDateTime );

			// SQL Data
			{ 
				SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
				if ( pcDB->Open() )
				{
					if ( pcDB->Prepare( "INSERT INTO CharacterInfo VALUES (?, ?, ?, 1, 0, 0, 0, 0, 0, 0, GETDATE(), 0, 0, 0, 1, GETDATE())" ) )
					{
						char szNull[2] = { 0,0 };
						pcDB->BindParameterInput( 1, PARAMTYPE_String, psPacket->szUserID );
						pcDB->BindParameterInput( 2, PARAMTYPE_String, psPacket->szCharname );
						pcDB->BindParameterInput( 3, PARAMTYPE_String, szNull );
						pcDB->Execute();
					}
					pcDB->Close();
				}
			}

			// Log
			{
				char szLog[ 256 ] = { 0 };
				char * pszIP = NULL;

				if ( pcUserData->pcSocketData )
					pszIP = pcUserData->pcSocketData->szIP;

				STRINGFORMAT( szLog, CHARACTERCREATED_LOGACCTEXT, pcUserData->szAccountName, psPacket->szCharname );

				SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );
				
				if ( pcDB->Open() )
				{
					if ( pcDB->Prepare( "INSERT INTO AccountLog VALUES (?,?,?,?,?,?)" ) )
					{
						int iLogID = ACCLOGID_CharacterCreated;
						pcDB->BindParameterInput( 1, PARAMTYPE_String, pszIP );
						pcDB->BindParameterInput( 2, PARAMTYPE_String, pcUserData->szAccountName );
						pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &iLogID );
						pcDB->BindParameterInput( 4, PARAMTYPE_String, szLog );
						pcDB->BindParameterInput( 5, PARAMTYPE_String, szDateTime );
						pcDB->BindParameterInput( 6, PARAMTYPE_Integer, &GAME_SERVER );

						pcDB->Execute();
					}
					pcDB->Close();
				}

			}
		}

		SENDPACKET( USERDATATOUSER(pcUserData), psPacket );
	}
}

void CharacterServer::SendCharacterDataEx( User * pcUser )
{
	if ( pcUser == NULL || pcUser->pcUserData == NULL )
		return;

	PacketCharacterDataEx s;
	ZeroMemory( &s, sizeof( PacketCharacterDataEx ) );
	s.iLength = sizeof( PacketCharacterDataEx );
	s.iHeader = PKTHDR_CharacterDataEx;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT OldHead, FSP FROM CharacterInfo WHERE ID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );

			if ( pcDB->Execute() )
			{
				if ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_String, s.sData.szOldHead, _countof( s.sData.szOldHead ) );
					pcDB->GetData( 2, PARAMTYPE_Integer, &s.sData.iFSP );
				}
			}
		}

		//Costume
		{
			bool bExists = false;

			pcDB->Clear();
			if ( pcDB->Prepare( "SELECT Weapon, Shield FROM CharacterCostume WHERE CharacterID=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );

				if ( pcDB->Execute() )
				{
					if ( pcDB->Fetch() )
					{
						pcDB->GetData( 1, PARAMTYPE_Binary, &s.cCostumeWeapon, sizeof( ItemData ) );
						pcDB->GetData( 2, PARAMTYPE_Binary, &s.cCostumeShield, sizeof( ItemData ) );

						bExists = true;
					}
				}
			}

			if ( bExists == false )
			{
				pcDB->Clear();
				if ( pcDB->Prepare( "INSERT INTO CharacterCostume([CharacterID], [Weapon], [Shield]) VALUES(?,?,?)" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );
					pcDB->BindParameterInput( 2, PARAMTYPE_Binary, &s.cCostumeWeapon, sizeof( ItemData ) );
					pcDB->BindParameterInput( 3, PARAMTYPE_Binary, &s.cCostumeShield, sizeof( ItemData ) );

					pcDB->Execute();
				}
			}
		}

		//Earring
		{
			bool bExists = false;

			pcDB->Clear();
			if ( pcDB->Prepare( "SELECT Earring1, Earring2 FROM CharacterEarring WHERE CharacterID=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );

				if ( pcDB->Execute() )
				{
					if ( pcDB->Fetch() )
					{
						pcDB->GetData( 1, PARAMTYPE_Binary, &s.cEarring1, sizeof( ItemData ) );
						pcDB->GetData( 2, PARAMTYPE_Binary, &s.cEarring2, sizeof( ItemData ) );

						bExists = true;
					}
				}
			}

			if ( bExists == false )
			{
				pcDB->Clear();
				if ( pcDB->Prepare( "INSERT INTO CharacterEarring([CharacterID], [Earring1], [Earring2]) VALUES(?,?,?)" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );
					pcDB->BindParameterInput( 2, PARAMTYPE_Binary, &s.cEarring1, sizeof( ItemData ) );
					pcDB->BindParameterInput( 3, PARAMTYPE_Binary, &s.cEarring2, sizeof( ItemData ) );

					pcDB->Execute();
				}
			}
		}

		pcDB->Close();
	}

	if ( s.cCostumeWeapon.bValid )
		ITEMSERVER->AddItemInventory( pcUser->pcUserData, s.cCostumeWeapon.sItem.sItemID.ToInt(), s.cCostumeWeapon.sItem.iChk1, s.cCostumeWeapon.sItem.iChk2 );

	if ( s.cCostumeShield.bValid )
		ITEMSERVER->AddItemInventory( pcUser->pcUserData, s.cCostumeShield.sItem.sItemID.ToInt(), s.cCostumeShield.sItem.iChk1, s.cCostumeShield.sItem.iChk2 );

	if ( s.cEarring1.bValid )
		ITEMSERVER->AddItemInventory( pcUser->pcUserData, s.cEarring1.sItem.sItemID.ToInt(), s.cEarring1.sItem.iChk1, s.cEarring1.sItem.iChk2 );

	if ( s.cEarring2.bValid )
		ITEMSERVER->AddItemInventory( pcUser->pcUserData, s.cEarring2.sItem.sItemID.ToInt(), s.cEarring2.sItem.iChk1, s.cEarring2.sItem.iChk2 );

	SENDPACKET( pcUser, &s );
}

void CharacterServer::OnCharacterSyncData( UserData * pcUserData )
{
	User * pcUser = USERDATATOUSER( pcUserData );


	char szMessageLogin[128] = { 0 };

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
				
	if ( EVENT_PVPMODE )
		CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "> Welcome to PvP Server!" );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT Message FROM MessageLogin" ) )
		{
			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_String, szMessageLogin, 128 );
					CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, szMessageLogin );
				}
			}
		}
		pcDB->Close();
	}

	USERSERVER->SetUserID( pcUserData );

	USERSERVER->DeleteReconnectUser( pcUserData->szAccountName );

	NETSERVER->OnGetGameMaster( pcUserData );
	SetExpFromSQL( pcUserData );

	pcUser->iAccountID = UserServer::SQLGetAccountID( pcUser->pcUserData->szAccountName );

	USERSERVER->UpdateID( pcUser );

	ITEMSERVER->OnLoadUser( pcUser );


	ACHIEVEMENTHANDLER->OnLoadUser( pcUser );

	HONORHANDLER->OnLoadUser( pcUser );

	// Online Status
	pcDB = SQLCONNECTION( DATABASEID_UserDB );
				
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE CharacterInfo SET IsOnline=1, JobCode=? WHERE Name=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUserData->sCharacterData.iClass );
			pcDB->BindParameterInput( 2, PARAMTYPE_String, GetCharacterName( pcUserData ) );
			pcDB->Execute();
		}
		pcDB->Close();
	}

	CopyMemory( &pcUser->sDateLevelUP, GetServerTime(), sizeof( SYSTEMTIME ) );

	// Clan Message
	int iClanID = 0;
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT ClanID, DateLevelUP FROM CharacterInfo WHERE Name=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, pcUserData->sCharacterData.szName );
			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_Integer, &iClanID, 0 );
				pcDB->GetData( 2, PARAMTYPE_Time, &pcUser->sDateLevelUP );
			}
		}
		pcDB->Close();
	}
	
	pcDB = SQLCONNECTION( DATABASEID_ClanDB );

	if ( iClanID > 0 )
	{
		char szClanMessage[128];
		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "SELECT LoginMessage FROM ClanList WHERE ID=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iClanID );
				if ( pcDB->Execute() && pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_String, szClanMessage, 128 );
				}
			}
			pcDB->Close();
		}

		if ( !IsNull( szClanMessage ) )
			CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Clan, "> %s", szClanMessage );
	}

	// Get Item Box
	int iItemsBox = ITEMSERVER->SQLItemBoxCount( pcUserData->szAccountName );
	if ( iItemsBox > 0 )
	{
		CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Global, "> You have %d item%s in Item Distributor!", iItemsBox, iItemsBox == 1 ? "" : "s" );
	}
	// Get Coin Amount
	int iCoinAmount = COINSHOPHANDLER->GetCoinAmount( COINSHOPHANDLER->GetCoinShop(), pcUserData->szAccountName );
	if ( iCoinAmount > 0 )
	{
		CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Global, "> You have %d credits!", iCoinAmount );
	}

	std::vector<std::string> vNotification = ACCOUNTSERVER->SQLGetAccountNotification( pcUserData->szAccountName );
	if ( vNotification.size() > 0 )
	{
		CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> You have %d new notification%s:", vNotification.size(), vNotification.size() > 1 ? "s" : "" );
		for ( std::vector<std::string>::iterator it = vNotification.begin(); it != vNotification.end(); it++ )
		{
			std::string str = (*it);
			CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> %s", str.c_str() );
		}
	}

	BLESSCASTLESERVER->SendUserScore( pcUser );

    EVENTSERVER->SendEvents( pcUser );
}

void CharacterServer::OnCharacterSyncDataEx( UserData * pcUserData, PacketPlayDataEx * psPacket )
{
	User * pcUser = USERDATATOUSER( pcUserData );
	memcpy( &pcUser->pData, psPacket, sizeof( PacketPlayDataEx ) );
	memcpy( &pcUserData->sCharacterData.iStrength, &pcUser->pData.sCharacterData, sizeof( CharacterDataPacket ) );
	pcUserData->sCharacterData.iCritical = pcUser->pData.iCritical;


	if ( pcUserData->iGameLevel == GAMELEVEL_None )
	{
		if ( LOGIN_SERVER && psPacket->dwSpeed >= ((((25 * 10) + 250) * 460) >> 8) )
		{
			PacketLogCheat sPacket;
			sPacket.iCheatID = CHEATLOGID_RunSpeedHack;
			sPacket.LParam = psPacket->dwSpeed;
			LOGSERVER->OnLogCheat( pcUserData, &sPacket );
			return;
		}
	}
	// Check Gold
	CheckGold( pcUserData );

	// Not Game Master?
	if ( pcUser->pData.bGameMaster && !pcUserData->iGameLevel )
	{
		if ( pcUser->bCheckedGameMaster )
		{
			PacketLogCheat sPacket;
			sPacket.iCheatID = CHEATLOGID_FakeGM;
			LOGSERVER->OnLogCheat( pcUserData, &sPacket );
		}
		else
			pcUser->bCheckedGameMaster = TRUE;
	}
	if ( !pcUser->pData.bGameMaster )
		pcUser->bCheckedGameMaster = FALSE;


	if ( (pcUser->dwTimeSync + 10000) < TICKCOUNT )
	{
		NETSERVER->SendClan( pcUserData );
		pcUser->dwTimeSync = TICKCOUNT;
	}

	// Server ID
	pcUserData->bServerIndexID = (BYTE)psPacket->dwServerID;

	// Login Server?
	if ( GAME_SERVER )
	{
		// Send server information to user debug
		PacketServerInfo sPacket;
		ZeroMemory( &sPacket, sizeof( PacketServerInfo ) );
		sPacket.iLength			= sizeof( PacketServerInfo );
		sPacket.iHeader			= PKTHDR_ServerInfo;
		sPacket.iUsers			= pcUserData->iGameLevel > GAMELEVEL_Two ? USERS_ONLINE : 0;
		sPacket.iMaxUsers		= pcUserData->iGameLevel > GAMELEVEL_Two ? MAX_USERS : 0;
		sPacket.iBossTime		= BOSS_TIME;
		sPacket.iServerVersion	= SERVER_VERSION_MIN;
		sPacket.bSiegeWarType	= BLESSCASTLESERVER->GetSiegeWarMode();
		sPacket.sSiegeWarDay	= GetSiegeWarDay();
		sPacket.sSiegeWarHour	= GetSiegeWarHour();
		GetSystemTime( &sPacket.sServerTime );

		if ( pcUserData->iGameLevel )
			sPacket.bGameMaster = TRUE;

		SENDPACKET( USERDATATOUSER(pcUserData), &sPacket );
	}

	// Check State Hack
	CHEATSERVER->CheckStatePoint( pcUserData, psPacket->dwQuestLevelLog );
}

void CharacterServer::OnCharacterDataExSave( User * pcUser )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE CharacterInfo SET FSP=? WHERE Name=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->sCharacterDataEx.iFSP );
			pcDB->BindParameterInput( 2, PARAMTYPE_String, GetCharacterName( pcUser->pcUserData ), STRLEN( GetCharacterName( pcUser->pcUserData ) ) );
			pcDB->Execute();
		}
		pcDB->Close();
	}
}

void CharacterServer::OnCharacterUpdateLevelClan( User * pcUser, PacketUpdateClanLevel * psPacket )
{
	if ( pcUser )
	{
		int iLevelOld = pcUser->pcUserData->sCharacterData.iLevel;

		if ( psPacket->dwCodeOP == 0 )
		{
			pcUser->pcUserData->sCharacterData.iLevel = psPacket->dwLevel;
		}
		else if ( psPacket->dwCodeOP == 1 )
		{
			pcUser->pcUserData->sCharacterData.iLevel = psPacket->dwLevel;
			pcUser->pcUserData->sCharacterData.iClanID = psPacket->dwClanCode;
			pcUser->pcUserData->sCharacterData.cNewLoad = (BYTE)psPacket->dwUpdateInfo;
			pcUser->pcUserData->iClanID = GetClanCode( pcUser->pcUserData->sCharacterData.iClanID );

			SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );

			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "UPDATE CharacterInfo SET JobCode=?, Level=? WHERE Name=?" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->pcUserData->sCharacterData.iClass );
					pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &pcUser->pcUserData->sCharacterData.iLevel );
					pcDB->BindParameterInput( 3, PARAMTYPE_String, &pcUser->pcUserData->sCharacterData.szName );
					pcDB->Execute();
				}
				pcDB->Close();
			}
			SendBlessCastleSkill( pcUser->pcUserData );
		}

		if ( pcUser->pcUserData->sCharacterData.iLevel != iLevelOld )
		{
			CopyMemory( &pcUser->sDateLevelUP, GetServerTime(), sizeof( SYSTEMTIME ) );

			//Level
			/*
			if ( (iLevelOld >= 120) || (pcUser->pcUserData->sCharacterData.iLevel >= 120) )
			{
				if ( pcUser->GetGameLevel() < GAMELEVEL_Two )
				{
					if ( pcUser->pcUserData->sCharacterData.iLevel >= (iLevelOld + 5) )
					{
						PacketLogCheat sPacket;
						sPacket.iCheatID = CHEATLOGID_LevelErrorSyncEx;
						sPacket.LParam = iLevelOld;
						sPacket.SParam = pcUser->pcUserData->sCharacterData.iLevel;
						LOGSERVER->OnLogCheat( pcUser->pcUserData, &sPacket );
					}
				}
			}
			*/

			ACHIEVEMENTHANDLER->OnTask( pcUser, ACHIEVEMENTTASK_LevelUP, ValueTaskAchievement( pcUser->pcUserData->sCharacterData.iLevel ), ValueTaskAchievement( 0 ), ValueTaskAchievement( 0 ) );

			if ( pcUser->bParty )
			{
				PARTYHANDLER->SendChatParty( pcUser, FormatString( "> %s has leveled up to %d!", CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ),
					pcUser->pcUserData->sCharacterData.iLevel ), CHATCOLOR_Party, true );
			}
		}
	}
}


void CharacterServer::OnOutCharacterData( UserData * pcUserData )
{
	return;
}

void CharacterServer::SetPositionWarp( UserData * pcUserData, int iX, int iZ, int iStageID )
{
	if ( pcUserData )
	{
		PacketWarpGateField sPacket;
		sPacket.iLength	= sizeof( PacketWarpGateField );
		sPacket.iHeader = PKTHDR_WarpGateField;
		sPacket.iStage	= iStageID != -1 ? iStageID : pcUserData->iMapID;
		sPacket.iX		= iX;
		sPacket.iZ		= iZ;
		SENDPACKET( USERDATATOUSER( pcUserData ), &sPacket );

		if ( iStageID != -1 )
			pcUserData->iMapID = (EMapID)iStageID;

		pcUserData->sPosition.iX = iX;
		pcUserData->sPosition.iZ = iZ;
	}
}

BOOL CharacterServer::IsCharacterLeader( User * pcUser )
{
	BOOL bRet = FALSE;
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ClanDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT ID FROM ClanList WHERE ClanLeader=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, GetCharacterName( pcUser->pcUserData ) );
			if ( pcDB->Execute() && pcDB->Fetch() )
				bRet = TRUE;
		}
		pcDB->Close();
	}
	return bRet;
}

char * CharacterServer::GetServerName( User * pcUser )
{
	return GSERVER->GetServerName( pcUser->pcUserData->bServerIndexID );
}

void CharacterServer::PHCharacterDataEx( User * pcUser )
{
	if ( LOGIN_SERVER )
	{
		char szPath[MAX_PATH];
		GetCharacterExFilePath( pcUser->pcUserData->szCharacterName, szPath, MAX_PATH );

		FILE * fp;
		fopen_s( &fp, szPath, "rb" );
		if ( fp )
		{
		//	fread()
			fclose( fp );
		}
	}
}

int CharacterServer::SQLGetMonsterID( char * pszMonsterName )
{
	int iID = -1;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );

	if ( pcDB->Open() != FALSE )
	{
		if ( pcDB->Prepare( "SELECT ID FROM MonsterList WHERE Name=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, pszMonsterName, 32 );

			if ( pcDB->Execute() )
			{
				if ( pcDB->Fetch() )
					pcDB->GetData( 1, PARAMTYPE_Integer, &iID );
			}
		}

		pcDB->Close();
	}

	return iID;
}

int CharacterServer::SQLGetNPCID( char * pszNPCName, int iMapID )
{
	int iID = -1;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );



	if ( pcDB->Open() != FALSE )
	{
		if ( pcDB->Prepare( iMapID == (-1) ? "SELECT ID FROM NPCList WHERE Name=?" : "SELECT IDNPC FROM MapNPC WHERE IDNPC in (SELECT ID FROM NPCList WHERE Name=?) AND (Stage=?) AND ([Enabled]=1)" ) )
		{
			if ( iMapID != (-1) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_String, pszNPCName, STRLEN( pszNPCName ) );
				pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iMapID );
			}
			else
				pcDB->BindParameterInput( 1, PARAMTYPE_String, pszNPCName, 32 );

			if ( pcDB->Execute() )
			{
				if ( pcDB->Fetch() )
					pcDB->GetData( 1, PARAMTYPE_Integer, &iID );
			}
		}

		pcDB->Close();
	}

	return iID;
}

int CharacterServer::SQLGetCharacterID( char * pszName )
{
	int iID = -1;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
	
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT ID FROM CharacterInfo WHERE Name=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, pszName, STRLEN( pszName ) );
			
			if ( pcDB->Execute() && pcDB->Fetch() )
				pcDB->GetData( 1, PARAMTYPE_Integer, &iID );
		}

		pcDB->Close();
	}

	return iID;
}

void CharacterServer::HandlePacket( User * pcUser, PacketCharacterDataEx * psPacket )
{
	CopyMemory( &pcUser->sCharacterDataEx, &psPacket->sData, sizeof( CharacterDataEx ) );
	CHATSERVER->SendDebugChat( pcUser->pcUserData, CHATCOLOR_Error, "> SaveEx Success" );

	if ( (psPacket->cCostumeWeapon.bValid) && (ITEMSERVER->CheckValidItem( &psPacket->cCostumeWeapon.sItem ) == FALSE) )
	{
		auto pcItemData = &psPacket->cCostumeWeapon;

		//Item Name
		char szItemName[32] = { 0 };
		if ( ITEMSERVER->FindItemName( pcItemData->sItem.sItemID.ToInt(), szItemName ) )
		{
			//Log It...
			PacketLogCheat sPacket;
			sPacket.iCheatID = CHEATLOGID_WeaponCostumeError;
			sPacket.LParam = pcItemData->sItem.sItemID.ToInt();
			sPacket.SParam = pcItemData->sItem.iChk1;
			sPacket.EParam = pcItemData->sItem.iChk2;
			STRINGCOPY( sPacket.szBuffer1, szItemName );
			LOGSERVER->OnLogCheat( pcUser->pcUserData, &sPacket );
		}

		ZeroMemory( &psPacket->cCostumeWeapon, sizeof( ItemData ) );
	}
	
	if ( (psPacket->cCostumeShield.bValid) && (ITEMSERVER->CheckValidItem( &psPacket->cCostumeShield.sItem ) == FALSE) )
	{
		auto pcItemData = &psPacket->cCostumeShield;

		//Item Name
		char szItemName[32] = {0};
		if ( ITEMSERVER->FindItemName( pcItemData->sItem.sItemID.ToInt(), szItemName ) )
		{
			//Log It...
			PacketLogCheat sPacket;
			sPacket.iCheatID = CHEATLOGID_ShieldCostumeError;
			sPacket.LParam = pcItemData->sItem.sItemID.ToInt();
			sPacket.SParam = pcItemData->sItem.iChk1;
			sPacket.EParam = pcItemData->sItem.iChk2;
			STRINGCOPY( sPacket.szBuffer1, szItemName );
			LOGSERVER->OnLogCheat( pcUser->pcUserData, &sPacket );
		}

		ZeroMemory( &psPacket->cCostumeShield, sizeof( ItemData ) );
	}

	if ( (psPacket->cEarring1.bValid) && (ITEMSERVER->CheckValidItem( &psPacket->cEarring1.sItem ) == FALSE) )
	{
		auto pcItemData = &psPacket->cEarring1;

		//Item Name
		char szItemName[32] = {0};
		if ( ITEMSERVER->FindItemName( pcItemData->sItem.sItemID.ToInt(), szItemName ) )
		{
			//Log It...
			PacketLogCheat sPacket;
			sPacket.iCheatID = CHEATLOGID_EarringItemError;
			sPacket.LParam = pcItemData->sItem.sItemID.ToInt();
			sPacket.SParam = pcItemData->sItem.iChk1;
			sPacket.EParam = pcItemData->sItem.iChk2;
			STRINGCOPY( sPacket.szBuffer1, szItemName );
			LOGSERVER->OnLogCheat( pcUser->pcUserData, &sPacket );
		}

		ZeroMemory( &psPacket->cEarring1, sizeof( ItemData ) );
	}

	if ( (psPacket->cEarring2.bValid) && (ITEMSERVER->CheckValidItem( &psPacket->cEarring2.sItem ) == FALSE) )
	{
		auto pcItemData = &psPacket->cEarring2;

		//Item Name
		char szItemName[32] = {0};
		if ( ITEMSERVER->FindItemName( pcItemData->sItem.sItemID.ToInt(), szItemName ) )
		{
			//Log It...
			PacketLogCheat sPacket;
			sPacket.iCheatID = CHEATLOGID_EarringItemError;
			sPacket.LParam = pcItemData->sItem.sItemID.ToInt();
			sPacket.SParam = pcItemData->sItem.iChk1;
			sPacket.EParam = pcItemData->sItem.iChk2;
			STRINGCOPY( sPacket.szBuffer1, szItemName );
			LOGSERVER->OnLogCheat( pcUser->pcUserData, &sPacket );
		}

		ZeroMemory( &psPacket->cEarring2, sizeof( ItemData ) );
	}

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
	if ( pcDB->Open() )
	{	
		if ( pcDB->Prepare( "UPDATE CharacterCostume SET [Weapon]=?, [Shield]=? WHERE [CharacterID]=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Binary, &psPacket->cCostumeWeapon, sizeof( ItemData ) );
			pcDB->BindParameterInput( 2, PARAMTYPE_Binary, &psPacket->cCostumeShield, sizeof( ItemData ) );
			pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &pcUser->iCharacterID );

			pcDB->Execute();
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "UPDATE CharacterEarring SET [Earring1]=?, [Earring2]=? WHERE [CharacterID]=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Binary, &psPacket->cEarring1, sizeof( ItemData ) );
			pcDB->BindParameterInput( 2, PARAMTYPE_Binary, &psPacket->cEarring2, sizeof( ItemData ) );
			pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &pcUser->iCharacterID );

			pcDB->Execute();
		}

		pcDB->Close();
	}
}

void CharacterServer::HandlePacket( User * pcUser, PacketCharacterDataSyncEx * psPacket )
{
	if ( pcUser->GetGameLevel() >= GAMELEVEL_Four )
	{
		if ( UserData * pcUserData = USERDATABYID( psPacket->iID ) )
		{
			CopyMemory( &psPacket->sData, &pcUserData->sCharacterData.iStrength, sizeof( CharacterDataPacket ) );
			SENDPACKET( pcUser, psPacket );
		}
	}
}

void CharacterServer::HandlePacket( User * pcUser, PacketSaveData * psPacket )
{
	if ( SERVEREVENTHANDLER->CannotSave( pcUser ) )
	{
		psPacket->iHeader = 0;
		pcUser->pcUserData->dwCharacterUpdateTime = TICKCOUNT;

		PacketSaveSuccess sPacket;
		sPacket.iLength = sizeof( PacketSaveSuccess );
		sPacket.iHeader = PKTHDR_SaveSuccess;
		sPacket.iUnknown = 0;
		sPacket.bSaved = TRUE;
		sPacket.iBlessCastleTax = BLESSCASTLESERVER->psBlessCastleStatusData->iTax;
		sPacket.iBlessCastleClanID = BLESSCASTLESERVER->psBlessCastleStatusData->iClanID;
		SENDPACKET( pcUser, &sPacket );

		if ( pcUser->GetGameLevel() >= GAMELEVEL_Three )
			CHATSERVER->SendChat( pcUser, CHATCOLOR_Error, "> Cannot Save Event!" );
	}
}

void CharacterServer::HandlePacket( User * pcUser, PacketSelectCharacter * psPacket )
{
	SQLCharacter sCharacter;
	if ( SQLGetCharacter( psPacket->szName, &sCharacter ) )
	{
		if ( STRINGCOMPAREI( pcUser->pcUserData->szAccountName, sCharacter.szAccountName ) )
		{
			pcUser->iCharacterID = sCharacter.iID;

			SendCharacterDataEx( pcUser );
		}
	}
}

BOOL CharacterServer::Validate( CharacterData * psCharacterData )
{
	return CALL( 0x00446050 );
}

DWORD CharacterServer::GetHashString( const char * pszName )
{
	return CALL_WITH_ARG1( 0x00415F80, (DWORD)pszName );
}
