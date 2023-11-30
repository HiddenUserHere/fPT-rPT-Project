#include "stdafx.h"
#include "accountserver.h"



//#define _LOGIN_TEST

PacketCharacterRecordData sPacketCharacterDataLogin;
PacketCharacterSelectUser sPacketUserInfoLogin;

bool AccountServer::bActive;
int AccountServer::iWaitingAccountLogins;
CMutex * AccountServer::pcMutex;
AccountLogin AccountServer::saAccountLogin[MAX_ACCOUNTLOGINQUEUE];

AccountServer::AccountServer()
{
	pcMutex = new CMutex( "Account Server" );

	ZeroMemory( &saAccountLogin, sizeof( saAccountLogin ) );
	iWaitingAccountLogins = 0;
	bActive = true;
}

AccountServer::~AccountServer()
{
	bActive = false;
	iWaitingAccountLogins = 0;
	ZeroMemory( &saAccountLogin, sizeof( saAccountLogin ) );

	DELET( pcMutex );
}

void AccountServer::LogAccountLogin( UserData * pcUserData, AccountLogin & al, ELogAccount eLogAccount )
{

	char szLogDescription[ 512 ] = { 0 };
	char * pszIP = NULL;
	char szDateTime[ 20 ] = { 0 };
	GetDateTime( szDateTime );

	char * pszSystemOS = "Unknown";

	switch ( (ESystemOS)al.iSystemOS )
	{
		case SYSTEMOS_Windows2000:
			pszSystemOS = "Windows 2000";
			break;
		case SYSTEMOS_WindowsXP:
			pszSystemOS = "Windows XP";
			break;
		case SYSTEMOS_Windows2003:
			pszSystemOS = "Windows Server 2003";
			break;
		case SYSTEMOS_Windows2003R2:
			pszSystemOS = "Windows Server 2003 R2";
			break;
		case SYSTEMOS_WindowsVista:
			pszSystemOS = "Windows Vista";
			break;
		case SYSTEMOS_Windows2008:
			pszSystemOS = "Windows Server 2008";
			break;
		case SYSTEMOS_Windows2008R2:
			pszSystemOS = "Windows Server 2008 R2";
			break;
		case SYSTEMOS_Windows7:
			pszSystemOS = "Windows 7";
			break;
		case SYSTEMOS_Windows2012:
			pszSystemOS = "Windows Server 2012";
			break;
		case SYSTEMOS_Windows8:
			pszSystemOS = "Windows 8";
			break;
		case SYSTEMOS_Windows2012R2:
			pszSystemOS = "Windows Server 2012 R2";
			break;
		case SYSTEMOS_Windows8_1:
			pszSystemOS = "Windows 8.1";
			break;
		case SYSTEMOS_Windows2016:
			pszSystemOS = "Windows Server 2016";
			break;
		case SYSTEMOS_Windows10:
			pszSystemOS = "Windows 10";
			break;
	}

	if ( pcUserData )
	{
		if ( pcUserData->pcSocketData )
			pszIP = pcUserData->pcSocketData->szIP;
	}

	switch ( eLogAccount )
	{
		case ACCLOGID_LoginSuccess:
			STRINGFORMAT( szLogDescription, "[%s] (Login Success) -> Password[%s] MacAddr[%s] PCName[%s] SerialHD[%d] VideoName[%s] VideoGUID[%d] HardwareID[%s] FolderID[%d] Width[%d] Height[%d] SystemOS[%s]", 
				al.szAccountName, al.szPassword, al.szMacAddress, al.szPCName, al.uSerialHD, al.szVideoName, al.uVideoGUID, al.szHardwareID, al.uUniqueFolderID, al.uWidthScreen, al.uHeightScreen, pszSystemOS );
			break;

		case ACCLOGID_IncorrectAccount:
			STRINGFORMAT( szLogDescription, INCORRECT_ACCOUNT_LOGACCTEXT, 
				al.szAccountName, al.szPassword, al.szMacAddress, al.szPCName, al.uSerialHD, al.szVideoName, al.uVideoGUID, al.szHardwareID, al.uUniqueFolderID );
			break;

		case ACCLOGID_IncorrectPassword:
			STRINGFORMAT( szLogDescription, INCORRECT_PASSWORD_LOGACCTEXT, al.szAccountName, al.szPassword );
			break;

		case ACCLOGID_BlockedAccount:
			STRINGFORMAT( szLogDescription, TRYLOGIN_ACCBLOCKED_LOGACCTEXT, al.szAccountName, al.szPassword );
			break;

		case ACCLOGID_UnblockedAccount:
			STRINGFORMAT( szLogDescription, UNBANNED_ACCBLOCKED_LOGACCTEXT, al.szAccountName, al.szPassword );
			break;

		case ACCLOGID_NotActivatedAccount:
			STRINGFORMAT( szLogDescription, TRYLOGIN_ACCNOTACTIVATED_LOGACCTEXT, al.szAccountName, al.szPassword );
			break;

		case ACCLOGID_CharacterSelectSend:
			STRINGFORMAT( szLogDescription, CHARACTERSELECTSEND_LOGACCTEXT, al.szAccountName );
			break;

		case ACCLOGID_TempBanAccount:
			STRINGFORMAT( szLogDescription, "[%s] (Temporarily Banned) -> Password[%s] MacAddr[%s] PCName[%s] SerialHD[%d] VideoName[%s]",
				al.szAccountName, al.szPassword, al.szMacAddress, al.szPCName, al.uSerialHD, al.szVideoName );
			break;

		default:
			break;
	}

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "INSERT INTO AccountLog VALUES (?,?,?,?,?,?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, pszIP );
			pcDB->BindParameterInput( 2, PARAMTYPE_String, (void*)al.szAccountName );
			pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &eLogAccount );
			pcDB->BindParameterInput( 4, PARAMTYPE_String, szLogDescription, 512 );
			pcDB->BindParameterInput( 5, PARAMTYPE_String, szDateTime );
			pcDB->BindParameterInput( 6, PARAMTYPE_Integer, &GAME_SERVER );
			pcDB->Execute();
		}
		pcDB->Close();
	}
}

BOOL AccountServer::IsOnlineAccount( UserData * pcUserData, const char * pszUserID )
{
	for ( int i = 0; i < PLAYERS_MAX; i++ )
	{
		if ( &USERSDATA[i] != pcUserData && USERSDATA[i].pcSocketData )
		{
			if ( lstrcmpiA( USERSDATA[i].szAccountName, pszUserID ) == 0 )
			{
				if ( USERSDATA[i].dwHashAccountName == pcUserData->dwHashAccountName )
				{
					if ( USERSDATA[i].pcSocketData->bConnected == FALSE )
					{
						LOGSERVER->LogSocketClose( USERSDATA[i].pcSocketData, "AccountServer::IsOnlineAccount", _ReturnAddress() );
						WNDPROC_SOCKETCLOSE( SOCKETSERVER->GetHWND(), USERSDATA[i].pcSocketData );

						USERSERVER->DeleteReconnectUser( pszUserID );

						return FALSE;
					}

					return TRUE;
				}
			}
		}
	}

	USERSERVER->DeleteReconnectUser( pszUserID );

	return FALSE;
}

BOOL AccountServer::IsAdminMacAddress( const char * pszMacAddress, UINT uSerialHD )
{

	return FALSE;
}

BOOL AccountServer::IsAdminDeveloper( const char * pszMacAddress, UINT uSerialHD )
{
	return FALSE;
}

BOOL AccountServer::IsIPAccept( const char * pszIP )
{
	BOOL bRet = FALSE;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT Corno FROM Maintenance WHERE (Mode=1) AND (IP=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, (void*)pszIP );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				bRet = TRUE;
			}
		}
		pcDB->Close();
	}

	return bRet;
}

BOOL AccountServer::IsAccountAccept( const char * pszAccountName )
{
	BOOL bRet = FALSE;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT Corno FROM Maintenance WHERE (Mode=7) AND (IP=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, (void*)pszAccountName );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				bRet = TRUE;
			}
		}
		pcDB->Close();
	}

	return bRet;
}

BOOL AccountServer::IsMacBanned( UserData * pcUserData, const char * pszMacAddress )
{
	BOOL bRet = FALSE;
	
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT Corno FROM Maintenance WHERE (Mode=4) AND (IP=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, (void*)pszMacAddress );
			
			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				bRet = TRUE;
			}
		}
		pcDB->Close();
	}

	if ( bRet == FALSE )
	{
		//Mac Address of VM Ware 2
		if ( std::string( pszMacAddress ).find( "00-0C-29-", 0, 9 ) != std::string::npos )
			bRet = TRUE;

		//Mac Address of VM Ware 3
		if ( std::string( pszMacAddress ).find( "00-05-69-", 0, 9 ) != std::string::npos )
			bRet = TRUE;

		//Mac Address of Virtual Box
		if ( std::string( pszMacAddress ).find( "08-00-27-", 0, 9 ) != std::string::npos )
			bRet = TRUE;

		//Mac Address of Virtual PC
		if ( std::string( pszMacAddress ).find( "00-03-FF-", 0, 9 ) != std::string::npos )
			bRet = TRUE;
	}

	return bRet;
}

BOOL AccountServer::IsSerialHDBanned( UserData * pcUserData, const char * pszSerialHD )
{
	BOOL bRet = FALSE;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT Corno FROM Maintenance WHERE (Mode=5) AND (IP=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, (void*)pszSerialHD );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				bRet = TRUE;
			}
		}
		pcDB->Close();
	}
	return bRet;
}

BOOL AccountServer::IsComputerNameBanned( UserData * pcUserData, const char * pszPCName )
{
	BOOL bRet = FALSE;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT Corno FROM Maintenance WHERE (Mode=6) AND (IP=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, (void*)pszPCName );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				bRet = TRUE;
			}
		}
		pcDB->Close();
	}

	if ( bRet == FALSE )
	{
		if ( std::string( (char*)0x008B867F0 ).find( "VMWare" ) != std::string::npos )
			bRet = TRUE;
		if ( std::string( (char*)0x008B867F0 ).find( "Virtual" ) != std::string::npos )
			bRet = TRUE;
	}

	return bRet;
}

BOOL AccountServer::IsVideoGUIDBanned( UserData * pcUserData, UINT uVideoGUID )
{
	char szValue[32] = { 0 };
	STRINGFORMAT( szValue, "%d", uVideoGUID );

	BOOL bRet = FALSE;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT Corno FROM Maintenance WHERE (Mode=8) AND (IP=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, szValue );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				bRet = TRUE;
			}
		}
		pcDB->Close();
	}
	return bRet;
}

BOOL AccountServer::IsHardwareIDBanned( UserData * pcUserData, const char * pszHardwareID )
{
	BOOL bRet = FALSE;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT Corno FROM Maintenance WHERE (Mode=9) AND (IP=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, (void*)pszHardwareID );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				bRet = TRUE;
			}
		}
		pcDB->Close();
	}
	return bRet;
}

BOOL AccountServer::IsFolderIDBanned( UserData * pcUserData, UINT uFolderID )
{
	char szValue[32] = { 0 };
	STRINGFORMAT( szValue, "%d", uFolderID );

	BOOL bRet = FALSE;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT Corno FROM Maintenance WHERE (Mode=10) AND (IP=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, szValue );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				bRet = TRUE;
			}
		}
		pcDB->Close();
	}
	return bRet;
}

bool AccountServer::GetVersionChecksum( int iGameVersion, UINT & uGameChecksum, UINT & uDLLChecksum )
{
	bool bRet = false;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT GameChecksum, DLLChecksum FROM Patches WHERE VersionID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iGameVersion );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_Integer, &uGameChecksum );
				pcDB->GetData( 2, PARAMTYPE_Integer, &uDLLChecksum );

				bRet = true;
			}
		}
		pcDB->Close();
	}

	return bRet;
}

void AccountServer::SQLCharacterLog( const char * pszAccountName, int iCharacterClass, const char * pszCharacterName, const char * pszCode, const char * pszIP, const char * pszServerName, int iCharacterLevel, INT64 iCharacterExperience, int iCharacterGold, int iAction )
{
	switch ( iAction )
	{
		case 3:
			CharacterServer::SQLUpdateCharacter( pszCharacterName, iCharacterClass, iCharacterLevel, iCharacterExperience, iCharacterGold );
			break;
		
		default:
			break;
	}
}

void AccountServer::SQLUserOnline( const char * pszAccountName, const char * pszCharacterName, const char * pszIP, int iTicket, int iAction, int iCharacterClass, int iCharacterLevel )
{
	if ( iAction == 2 )
	{
		if ( GSERVER->GetServerType() == SERVERTYPE_Multi )
			return;
	}

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );

	if( pcDB->Open() != FALSE )
	{
		switch( iAction )
		{
		case 0:
			if( pcDB->Prepare( "INSERT INTO UsersOnline ([AccountName],[CharacterName],[IP],[CharacterClass],[CharacterLevel],[Ticket],[LoginTime]) VALUES (?,?,?,?,?,?,GETDATE())" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_String, (void *)pszAccountName );
				pcDB->BindParameterInput( 2, PARAMTYPE_String, (void *)pszCharacterName );
				pcDB->BindParameterInput( 3, PARAMTYPE_String, (void *)pszIP );
				pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &iCharacterClass );
				pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &iCharacterLevel );
				pcDB->BindParameterInput( 6, PARAMTYPE_Integer, &iTicket );

				pcDB->Execute();

			}
			break;
		case 1:

			if( pcDB->Prepare( "DELETE FROM UsersOnline WHERE AccountName=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_String, (void *)pszAccountName );

				pcDB->Execute();
			}
			break;
		
		case 2:

			if( pcDB->Prepare( "DELETE FROM UsersOnline" ) )
			{
				pcDB->Execute();
			}
			break;
		
			default:
				break;
		}

		pcDB->Close();
	}
}

void AccountServer::SQLCharacterSave( UserData * pcUserData )
{
	char szLogDescription[512] = { 0 };
	char * pszIP = NULL;
	char szDateTime[20] = { 0 };
	GetDateTime( szDateTime );

	if ( pcUserData->pcSocketData )
		pszIP = pcUserData->pcSocketData->szIP;


	int iLogID = ACCLOGID_CharacterSave;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );

	std::string str = FormatNumber( (INT64)pcUserData->iInventoryGold );

	INT64 iCurrEXP = CHARACTERSERVER->GetExpFromLevel( pcUserData->sCharacterData.iLevel );
	INT64 iNextEXP = CHARACTERSERVER->GetExpFromLevel( pcUserData->sCharacterData.iLevel + 1 );

	INT64 iTotalEXP = iNextEXP - iCurrEXP;
	if ( iTotalEXP == 0 )
		iTotalEXP = 1;
	INT64 iCharEXP	= pcUserData->liEXP.QuadPart - iCurrEXP;
	int iPercentEXP = (int)((iCharEXP * 100) / iTotalEXP);


	STRINGFORMAT( szLogDescription, "Character[%s] SaveEXP[%s] PercentEXP[%d%%] Level[%d] Gold[%s]", CHARACTERSERVER->GetCharacterName( pcUserData ),
		FormatNumber( pcUserData->liEXP.QuadPart ), iPercentEXP, pcUserData->sCharacterData.iLevel, str.c_str() );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "INSERT INTO CharacterLog VALUES (?,?,?,?,?,?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, pszIP );
			pcDB->BindParameterInput( 2, PARAMTYPE_String, pcUserData->szAccountName );
			pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &iLogID );
			pcDB->BindParameterInput( 4, PARAMTYPE_String, szLogDescription, 512 );
			pcDB->BindParameterInput( 5, PARAMTYPE_String, szDateTime );
			pcDB->BindParameterInput( 6, PARAMTYPE_Integer, &GAME_SERVER );
			pcDB->Execute();
		}
		pcDB->Close();
	}
}


bool AccountServer::SQLAccountHasFlag( char * pszAccountName, EAccountFlag iFlag )
{

	return true;
}


void AccountServer::PHAccountLogin( User * pcUser, PacketLoginUser * psPacket )
{
	if ( LOGIN_SERVER )
	{
		BYTE * pPacket = (BYTE*)psPacket;
		for ( int i = 8; i < sizeof( PacketLoginUser ); i++ )
		{
			if ( pPacket[i] != 0 )
				pPacket[i] ^= 8;
		}

		AccountLogin al;
		al.pcUser = pcUser;
		al.pcUserData = pcUser->pcUserData;
		al.pcSocketData = pcUser->pcUserData->pcSocketData;
		al.uSerialHD = psPacket->dwSerialHD;
		al.uVideoGUID = psPacket->uUniqueVideoID;
		al.uUniqueFolderID = psPacket->uUniqueFolderID;
		al.uWidthScreen = psPacket->uWidthScreen;
		al.uHeightScreen = psPacket->uHeightScreen;
		al.iSystemOS = psPacket->iSystemOS;

		al.iGameVersion = psPacket->iGameVersion;
		al.uGameChecksum = psPacket->uGameChecksum;
		al.uDLLChecksum = psPacket->uDLLChecksum;

		STRINGCOPY( al.szAccountName, psPacket->szUserID );
		STRINGCOPY( al.szPassword, psPacket->szPassword );
		STRINGCOPY( al.szIP, pcUser->pcUserData->pcSocketData->szIP );
		STRINGCOPY( al.szMacAddress, psPacket->szMacAddr );
		STRINGCOPY( al.szPCName, psPacket->szPCName );
		STRINGCOPY( al.szVideoName, psPacket->szVideoName );
		STRINGCOPY( al.szHardwareID, psPacket->szHardwareID );

		//Obfuscator Login
		{
			int iLengthStr = STRLEN( al.szAccountName );
			for ( int i = 0; i < iLengthStr; i++ )
				al.szAccountName[i] ^= 0x2;

			iLengthStr = STRLEN( al.szPassword );
			for ( int i = 0; i < iLengthStr; i++ )
				al.szPassword[i] ^= 0x4;
		}

		EAccountLogin iCode = ACCOUNTLOGIN_Success;

		if ( !AddAccountLogin( pcUser, al ) )
		{
			//Connection Failed
			iCode = ACCOUNTLOGIN_ConnectionFailed;
		}

		if ( (iCode != ACCOUNTLOGIN_Success) && (iCode != ACCOUNTLOGIN_WrongVersion) )
		{
			PacketAccountLoginCode s;
			s.iLength = sizeof( s );
			s.iHeader = PKTHDR_AccountLoginCode;
			s.dwReserved = 0;
			s.iCode = iCode;
			s.iFailCode = 1;

			SENDPACKET( pcUser, &s, TRUE );
		}
	}
}


bool AccountServer::AddAccountLogin( User * pcUser, AccountLogin & al )
{
	bool bRet = false;

	pcMutex->Lock( 4000 );

	bool bAdd = true;

	for ( int i = 0; i < iWaitingAccountLogins; i++ )
	{
		if ( saAccountLogin[iWaitingAccountLogins].pcUser == pcUser )
		{
			bAdd = false;
			break;
		}
	}

	if ( bAdd && (iWaitingAccountLogins < MAX_ACCOUNTLOGINQUEUE) )
	{
		CopyMemory( &saAccountLogin[iWaitingAccountLogins], &al, sizeof( AccountLogin ) );

		iWaitingAccountLogins++;

		bRet = true;
	}

	pcMutex->Unlock();

	return bRet;
}


EAccountFlag AccountServer::SQLAccountGetFlag( char * pszAccountName )
{
	EAccountFlag iRet = ACCOUNTFLAG_NotExist;

	SQLConnection * cDB;

	cDB = SQLCONNECTION( DATABASEID_UserDB );

	if ( cDB->Open() != FALSE )
	{
		if ( cDB->Prepare( "SELECT Flag FROM UserInfo WHERE AccountName=?" ) )
		{
			cDB->BindParameterInput( 1, PARAMTYPE_String, pszAccountName, 32 );

			if ( cDB->Execute() )
				if ( cDB->Fetch() )
					cDB->GetData( 1, PARAMTYPE_Integer, &iRet );
		}

		cDB->Close();
	}

	return iRet;
}


bool AccountServer::SQLUnbanAccount( char * pszAccountName )
{
	bool bRet = false;

	SQLConnection * cDB;

	cDB = SQLCONNECTION( DATABASEID_UserDB );

	if ( cDB->Open() != FALSE )
	{
		if ( cDB->Prepare( "UPDATE UserInfo SET BanStatus=0,UnbanDate=NULL WHERE AccountName=?" ) )
		{
			cDB->BindParameterInput( 1, PARAMTYPE_String, pszAccountName, 32 );

			if ( cDB->Execute() )
				bRet = true;
		}

		cDB->Close();
	}

	return bRet;
}

int AccountServer::SQLGetBanStatus( char * pszAccountName, SYSTEMTIME * psBanTime )
{
	int			iRet = BANSTATUS_Error;
	SYSTEMTIME	sCurrentTime, sUnbanDate;
	GetLocalTime( &sCurrentTime );

	SQLConnection * cDB;

	cDB = SQLCONNECTION( DATABASEID_UserDB );

	if ( cDB->Open() != FALSE )
	{
		if ( cDB->Prepare( "SELECT BanStatus,UnbanDate FROM UserInfo WHERE AccountName=?" ) )
		{
			cDB->BindParameterInput( 1, PARAMTYPE_String, pszAccountName, 32 );

			if ( cDB->Execute() )
			{
				if ( cDB->Fetch() )
				{
					cDB->GetData( 1, PARAMTYPE_Integer, &iRet );
					cDB->GetData( 2, PARAMTYPE_Time, &sUnbanDate );

					if ( iRet == BANSTATUS_TempBanned )
					{
						if ( psBanTime )
							CopyMemory( psBanTime, &sUnbanDate, sizeof( SYSTEMTIME ) );
						
						if ( sCurrentTime.wYear > sUnbanDate.wYear )
							iRet = BANSTATUS_BanExpired;
						else if ( sCurrentTime.wYear == sUnbanDate.wYear )
						{
							if ( sCurrentTime.wMonth > sUnbanDate.wMonth )
								iRet = BANSTATUS_BanExpired;
							else if ( sCurrentTime.wMonth == sUnbanDate.wMonth )
							{
								if ( sCurrentTime.wDay > sUnbanDate.wDay )
									iRet = BANSTATUS_BanExpired;
								else if ( sCurrentTime.wDay == sUnbanDate.wDay )
								{
									if ( sCurrentTime.wHour > sUnbanDate.wHour )
										iRet = BANSTATUS_BanExpired;
									else if ( sCurrentTime.wHour == sUnbanDate.wHour )
									{
										if ( sCurrentTime.wMinute >= sUnbanDate.wMinute )
											iRet = BANSTATUS_BanExpired;
									}
								}
							}
						}
					}
				}
				else
					iRet = BANSTATUS_NotBanned;
			}
		}

		cDB->Close();
	}

	if ( iRet == BANSTATUS_BanExpired )
		if ( SQLUnbanAccount( pszAccountName ) )
		{
			iRet = BANSTATUS_NotBanned;
		}

	return iRet;
}


EAccountLogin AccountServer::SQLAccountLogin( char * pszAccountName, char * pszPassword, char * pszIP, EAccountFlag iAccountFlag, EAccountShare & eAccountShare, SYSTEMTIME & sUnbanDate )
{
	eAccountShare = ACCOUNTSHARE_None;

	if ( iAccountFlag == ACCOUNTFLAG_NotExist )
		return ACCOUNTLOGIN_IncorrectName;

	if ( !(ACCOUNTFLAG_Activated & iAccountFlag) )
		return ACCOUNTLOGIN_AccountNotActive;
	else if ( !(ACCOUNTFLAG_AcceptedLatestTOA & iAccountFlag) )
		return ACCOUNTLOGIN_AccountNotActive;
	else if ( !(ACCOUNTFLAG_Approved & iAccountFlag) )
		return ACCOUNTLOGIN_AccountNotActive;
	else if ( ACCOUNTFLAG_MustConfirm & iAccountFlag )
		return ACCOUNTLOGIN_AccountNotActive;

	int iBanStatus;
	if ( (iBanStatus = SQLGetBanStatus( pszAccountName, &sUnbanDate )) != BANSTATUS_NotBanned )
		return (iBanStatus == BANSTATUS_TempBanned ? ACCOUNTLOGIN_TempBanned : (iBanStatus == BANSTATUS_Banned ? ACCOUNTLOGIN_Banned : ACCOUNTLOGIN_ConnectionFailed));

	int iAccountID = -1;
	char szPassword[128] = { 0 };
	EAccountLogin iRet = ACCOUNTLOGIN_ConnectionFailed;

	SQLConnection * cDB;

	cDB = SQLCONNECTION( DATABASEID_UserDB );

	if ( cDB->Open() )
	{
		if ( cDB->Prepare( "SELECT ID, Password FROM UserInfo WHERE AccountName=?" ) )
		{
			cDB->BindParameterInput( 1, PARAMTYPE_String, pszAccountName, 32 );

			if ( cDB->Execute() )
			{
				if ( cDB->Fetch() )
				{
					cDB->GetData( 1, PARAMTYPE_Integer, &iAccountID );
					cDB->GetData( 2, PARAMTYPE_String, szPassword, _countof( szPassword ) );

					if ( STRINGCOMPARE( pszPassword, szPassword ) )
						iRet = ACCOUNTLOGIN_Success;
					else
						iRet = ACCOUNTLOGIN_IncorrectPassword;
				}
				else
					iRet = ACCOUNTLOGIN_IncorrectName;
			}
		}

		if ( (iRet == ACCOUNTLOGIN_IncorrectPassword) && (iAccountID >= 0) )
		{
			cDB->Clear();

			if ( cDB->Prepare( "SELECT Password, AccountShareFlag FROM UserShare WHERE (AccountID=?) AND (ExpireDate > GETDATE()) AND (Password IS NOT NULL)" ) )
			{
				cDB->BindParameterInput( 1, PARAMTYPE_Integer, &iAccountID );
				if ( cDB->Execute() )
				{
					if ( cDB->Fetch() )
					{
						cDB->GetData( 1, PARAMTYPE_String, szPassword, _countof( szPassword ) );
						cDB->GetData( 2, PARAMTYPE_Integer, &eAccountShare );

						if ( STRINGCOMPARE( pszPassword, szPassword ) )
							iRet = ACCOUNTLOGIN_Success;
						else
							iRet = ACCOUNTLOGIN_IncorrectPassword;
					}
				}
			}
		}

		cDB->Close();
	}

	SecureZeroMemory( szPassword, sizeof( szPassword ) );

	return iRet;
}


void AccountServer::ProcessAccountLogin( AccountLogin & al )
{
	User * pcUser = al.pcUser;
	UserData * pcUserData = al.pcUserData;
	SocketData * pcSocketData = al.pcSocketData;
	char * pszAccountName = al.szAccountName;
	char * pszPassword = al.szPassword;
	char * pszIP = al.szIP;

	SYSTEMTIME sUnbanDate;
	EAccountShare eAccountShare = ACCOUNTSHARE_None;

	bool bContinue = false;

	if ( pcUser && pcUserData && pcSocketData && pszAccountName && pszPassword && pszIP )
	{
		if ( pcUser->pcUserData == pcUserData )
		{
			if ( pcUser->pcUserData->pcSocketData == pcSocketData )
			{
				if ( lstrcmpiA( pcSocketData->szIP, pszIP ) == 0 )
				{
					bContinue = true;
				}
			}
		}
	}

	if ( bContinue )
	{
		pcUser->SetGameLevel( GAMELEVEL_None );

		EAccountLogin iCode = ACCOUNTLOGIN_ConnectionFailed;

		if ( pszAccountName[0] == 0 )
		{
			//User did not fill in Account Name
			iCode = ACCOUNTLOGIN_IncorrectName;
		}
		else if ( pszPassword[0] == 0 )
		{
			//User did not fill in Password
			iCode = ACCOUNTLOGIN_IncorrectPassword;
		}

		bool bAccountLogin = false;

		if ( iCode == ACCOUNTLOGIN_ConnectionFailed )
		{
			EAccountFlag eAccountFlag = SQLAccountGetFlag( pszAccountName );

			//Event GM?
			if ( ACCOUNTFLAG_EventGM & eAccountFlag )
			{
				//Set GM Level to 2 (Event GM) if not already a GM
				if ( pcUser->GetGameLevel() < GAMELEVEL_Two )
					pcUser->SetGameLevel( GAMELEVEL_Two );
			}

			//Admin?
			if ( ACCOUNTSERVER->IsAdminMacAddress( al.szMacAddress, al.uSerialHD ) )
			{
				pcUser->SetGameLevel( GAMELEVEL_Four );

				iCode = ACCOUNTLOGIN_ConnectionFailed;

				//Admin?
				if ( ACCOUNTSERVER->IsAdminDeveloper( al.szMacAddress, al.uSerialHD ) )
				{
					if ( al.uVideoGUID == 12 )
						pcUser->SetGameLevel( GAMELEVEL_Five );
				}
			}

			if ( SERVER->GetMaintenance() )
			{
				//If Server is in Maintenance then access is denied for non-GMs
				if ( pcUser->GetGameLevel() == GAMELEVEL_None )
					iCode = ACCOUNTLOGIN_Maintenance;

				//Can connect?
				if ( ACCOUNTSERVER->IsIPAccept( pszIP ) )
				{
					iCode = ACCOUNTLOGIN_ConnectionFailed;
				}
				else if ( ACCOUNTSERVER->IsAccountAccept( pszAccountName ) )
				{
					iCode = ACCOUNTLOGIN_ConnectionFailed;

					bAccountLogin = true;
				}
			}
			else if ( ACCOUNTSERVER->IsAccountAccept( pszAccountName ) )
			{
				iCode = ACCOUNTLOGIN_ConnectionFailed;

				bAccountLogin = true;
			}

			if ( eAccountFlag == ACCOUNTFLAG_NotExist )
				iCode = ACCOUNTLOGIN_IncorrectName;

			if ( iCode == ACCOUNTLOGIN_ConnectionFailed )
			{
				pcUser->uSerialHD = al.uSerialHD;

				UserData * pcOtherUserData = NULL;
				if ( (pcOtherUserData = USERDATABYACCOUNTNAME( pszAccountName )) != NULL )
					iCode = ACCOUNTLOGIN_LoggedIn;

				if ( (pcUser->GetGameLevel() >= GAMELEVEL_Four) && (iCode == ACCOUNTLOGIN_ConnectionFailed) )
					iCode = ACCOUNTLOGIN_Success;

				if ( ACCOUNTSERVER->IsMacBanned( pcUserData, al.szMacAddress ) )
				{
					ACCOUNTSERVER->LogAccountLogin( pcUserData, al, ACCLOGID_IncorrectAccount );

					iCode = ACCOUNTLOGIN_YouAreBanned;
				}

				char szSerialHD[32] = { 0 };
				STRINGFORMAT( szSerialHD, "%d", al.uSerialHD );

				if ( ACCOUNTSERVER->IsSerialHDBanned( pcUserData, szSerialHD ) )
				{
					ACCOUNTSERVER->LogAccountLogin( pcUserData, al, ACCLOGID_IncorrectAccount );

					iCode = ACCOUNTLOGIN_YouAreBanned;
				}

				if ( ACCOUNTSERVER->IsComputerNameBanned( pcUserData, al.szPCName ) )
				{
					ACCOUNTSERVER->LogAccountLogin( pcUserData, al, ACCLOGID_IncorrectAccount );

					iCode = ACCOUNTLOGIN_YouAreBanned;
				}

				if ( ACCOUNTSERVER->IsVideoGUIDBanned( pcUserData, al.uVideoGUID ) )
				{
					ACCOUNTSERVER->LogAccountLogin( pcUserData, al, ACCLOGID_IncorrectAccount );

					iCode = ACCOUNTLOGIN_YouAreBanned;
				}

				if ( ACCOUNTSERVER->IsHardwareIDBanned( pcUserData, al.szHardwareID ) )
				{
					ACCOUNTSERVER->LogAccountLogin( pcUserData, al, ACCLOGID_IncorrectAccount );

					iCode = ACCOUNTLOGIN_YouAreBanned;
				}

				if ( ACCOUNTSERVER->IsFolderIDBanned( pcUserData, al.uUniqueFolderID ) )
				{
					ACCOUNTSERVER->LogAccountLogin( pcUserData, al, ACCLOGID_IncorrectAccount );

					iCode = ACCOUNTLOGIN_YouAreBanned;
				}
			}

			if ( iCode == ACCOUNTLOGIN_ConnectionFailed )
				iCode = SQLAccountLogin( pszAccountName, pszPassword, pszIP, eAccountFlag, eAccountShare, sUnbanDate );
		}

		if ( iCode == ACCOUNTLOGIN_Success )
		{
			//Bind Account Name and Account ID to User
			StringCchCopyA( pcUser->pcUserData->szAccountName, _countof( pcUser->pcUserData->szAccountName ), pszAccountName );
			StringCchCopyA( pcUser->szHardwareID, _countof( pcUser->szHardwareID ), al.szHardwareID );
			pcUser->iAccountID = UserServer::SQLGetAccountID( pszAccountName );
			pcUser->SetAccountShare( eAccountShare );

			//First Check if Checksum is correct
			UINT uGameChk = 0, uDLLChk = 0;
			if ( ACCOUNTSERVER->GetVersionChecksum( al.iGameVersion, uGameChk, uDLLChk ) )
			{
				if ( ((uGameChk == al.uGameChecksum) && (uDLLChk == al.uDLLChecksum)) || (pcUser->GetGameLevel() >= GAMELEVEL_Four) || bAccountLogin )
				{
					//Send List of Characters
					ACCOUNTSERVER->OnLoginSuccess( pcUser->pcUserData );

					//Set Random Ticket Number
					pcUserData->iTicket = RandomI( 1, 1000 );

					//Send Server Liset and Tick Number
					Server::GetInstance()->PHServerList( pcSocketData, pcUserData->iTicket, pcUser->eAccountShare );

					CSkillServerHandler::SendSkillTable( pcUser );

					ACCOUNTSERVER->LogAccountLogin( pcUser->pcUserData, al, ACCLOGID_LoginSuccess );
				}
				else
				{
					PacketLogCheat sPacket;
					sPacket.iCheatID = CHEATLOGID_CRC32ChecksumError;
					sPacket.LParam = al.uGameChecksum;
					sPacket.SParam = uGameChk;
					sPacket.EParam = al.uDLLChecksum;
					sPacket.SxParam = uDLLChk;
					sPacket.LxParam = al.iGameVersion;
					LOGSERVER->OnLogCheat( pcUser->pcUserData, &sPacket );
				}
			}
		}
		else
		{
			PacketAccountLoginCode s;
			ZeroMemory( &s, sizeof( PacketAccountLoginCode ) );
			
			if ( (iCode == ACCOUNTLOGIN_Banned) || (iCode == ACCOUNTLOGIN_IPBanned) || (iCode == ACCOUNTLOGIN_YouAreBanned) ||
				(iCode == ACCOUNTLOGIN_TempBanned) )
			{
				if ( iCode == ACCOUNTLOGIN_Banned )
					STRINGCOPY( s.szMessage, "Account is banned" );
				else if ( iCode == ACCOUNTLOGIN_IPBanned )
					STRINGCOPY( s.szMessage, "Your IP is banned" );
				else if ( iCode == ACCOUNTLOGIN_YouAreBanned )
					STRINGCOPY( s.szMessage, "You are banned" );
				else if ( iCode == ACCOUNTLOGIN_TempBanned )
					STRINGFORMAT( s.szMessage, "Account is banned until %02d/%02d/%04d %02d:%02d:%02d GMT", 
						sUnbanDate.wMonth, sUnbanDate.wDay, sUnbanDate.wYear, sUnbanDate.wHour, sUnbanDate.wMinute, sUnbanDate.wSecond );

				iCode = ACCOUNTLOGIN_Banned;
			}


			//Send Login Failed Packet
			s.iLength = sizeof( s );
			s.iHeader = PKTHDR_AccountLoginCode;
			s.dwReserved = 0;
			s.iCode = iCode;
			s.iFailCode = 2;
			SENDPACKETSOCKET( pcSocketData, &s, TRUE );
		}
	}

	ZeroMemory( &al, sizeof( AccountLogin ) );
}

DWORD WINAPI AccountServer::ProcessAccountLoginQueue()
{
	while ( bActive )
	{
		int iProcessAccountLogins = 0;
		static AccountLogin saProcessAccountLogin[MAX_ACCOUNTLOGINQUEUE];

		//Place all the pending Account Logins in the process queue
		pcMutex->Lock( 4000 );

		if ( iWaitingAccountLogins > 0 )
		{
			if ( iWaitingAccountLogins > MAX_ACCOUNTLOGINQUEUE )
			{
				//For Safety, should not happen
				iWaitingAccountLogins = MAX_ACCOUNTLOGINQUEUE;
			}

			for ( int i = 0; i < iWaitingAccountLogins; i++ )
			{
				CopyMemory( &saProcessAccountLogin[iProcessAccountLogins], &saAccountLogin[i], sizeof( AccountLogin ) );

				iProcessAccountLogins++;
			}

			iWaitingAccountLogins = 0;
		}

		pcMutex->Unlock();

		//Now process the ones ready to login
		if ( iProcessAccountLogins > 0 )
		{
			SERVER_MUTEX->Lock( 9000 );

			for ( int i = 0; i < iProcessAccountLogins; i++ )
			{
				//Process the Login
				ProcessAccountLogin( saProcessAccountLogin[i] );
			}

			SERVER_MUTEX->Unlock();

			for ( int i = 0; i < iProcessAccountLogins; i++ )
			{
				//Clean it
				SecureZeroMemory( &saProcessAccountLogin[i], sizeof( AccountLogin ) );
			}

			iProcessAccountLogins = 0;
		}

		Sleep( 1024 );
	}

	return TRUE;
}

std::vector<std::string> AccountServer::SQLGetAccountNotification( const char * pszAccountName )
{
	std::vector<std::string> vNotification;

	if ( (pszAccountName != NULL) && (pszAccountName[0] != 0) )
	{
		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "SELECT Message FROM NotificationData WHERE (AccountName=?) AND (Checked=0) ORDER BY ID ASC" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_String, (void*)pszAccountName, STRLEN( pszAccountName ) );
				if ( pcDB->Execute() )
				{
					while ( pcDB->Fetch() )
					{
						char szMessage[100] = { 0 };
						pcDB->GetData( 1, PARAMTYPE_String, szMessage, _countof( szMessage ) );
						vNotification.push_back( szMessage );
					}

					pcDB->Clear();
					if ( pcDB->Prepare( "UPDATE NotificationData SET Checked=1 WHERE AccountName=?" ) )
					{
						pcDB->BindParameterInput( 1, PARAMTYPE_String, (void*)pszAccountName, STRLEN( pszAccountName ) );
						pcDB->Execute();
					}
				}
			}

			pcDB->Close();
		}
	}

	return vNotification;
}


BOOL AccountServer::OnLoginSuccess( UserData * pcUserData )
{
	ZeroMemory( &sPacketUserInfoLogin, sizeof( PacketCharacterSelectUser ) );
	sPacketUserInfoLogin.iLength = sizeof( PacketCharacterSelectUser );
	sPacketUserInfoLogin.iHeader = PKTHDR_UserInfo;
	STRINGCOPY( sPacketUserInfoLogin.szAccountName, pcUserData->szAccountName );

	int iCount = 0;
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT TOP 6 * FROM CharacterInfo WHERE AccountName=? ORDER BY Experience DESC" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, pcUserData->szAccountName );
			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				// Get Data in SQL
				for ( int i = 0; i < 6; i++ )
				{
					char szPathBuffer[256] = { 0 };
					char szCharname[32] = { 0 };

					// Get Character Data file path
					pcDB->GetData( 3, PARAMTYPE_String, szCharname, 32 );
					STRINGFORMAT( szPathBuffer, "Data\\Character\\%s.chr", szCharname );

					FILE * pFile = NULL;
					fopen_s( &pFile, szPathBuffer, "rb" );

					// Character Data open?
					if ( pFile )
					{
						// Read .chr File

						ZeroMemory( &sPacketCharacterDataLogin, sizeof( PacketCharacterRecordData ) );
						fseek( pFile, 0, SEEK_SET );
						fread( &sPacketCharacterDataLogin, sizeof( PacketCharacterRecordData ), 1, pFile );
						fclose( pFile );

						// Packet Structure
						{
							STRINGCOPY( sPacketUserInfoLogin.saData[iCount].szName, sPacketCharacterDataLogin.sCharacterData.szName );
							STRINGCOPY( sPacketUserInfoLogin.saData[iCount].szBodyModel, sPacketCharacterDataLogin.sCharacterData.Player.szBodyModel );
							STRINGCOPY( sPacketUserInfoLogin.saData[iCount].szHeadModel, sPacketCharacterDataLogin.sCharacterData.Player.szHeadModel );
							sPacketUserInfoLogin.saData[iCount].iLevel = sPacketCharacterDataLogin.sCharacterData.iLevel;
							sPacketUserInfoLogin.saData[iCount].iClass = sPacketCharacterDataLogin.sCharacterData.iClass;
							sPacketUserInfoLogin.saData[iCount].iArmorID = ITEMID_None;
							sPacketUserInfoLogin.saData[iCount].iType = sPacketCharacterDataLogin.sCharacterData.iMonsterType;
							sPacketUserInfoLogin.saData[iCount].iMapID = sPacketCharacterDataLogin.sCharacterSaveData.iMapID;
							sPacketUserInfoLogin.saData[iCount].iX = sPacketCharacterDataLogin.sCharacterSaveData.iCameraPositionX;
							sPacketUserInfoLogin.saData[iCount].iZ = sPacketCharacterDataLogin.sCharacterSaveData.iCameraPositionZ;

							// Is in Sod?
							if ( sPacketUserInfoLogin.saData[iCount].iMapID == MAPID_Bellatra )
								sPacketUserInfoLogin.saData[iCount].iMapID = sPacketUserInfoLogin.saData[iCount].iLevel < 125 ? MAPID_RicartenTown : MAPID_NaviskoTown;

							// Is in Fury Arena ( Quest )?
							else if ( sPacketUserInfoLogin.saData[iCount].iMapID == MAPID_QuestArena )
								sPacketUserInfoLogin.saData[iCount].iMapID = MAPID_PhillaiTown;
							// Is in T5 Arena ( Quest )?
							else if ( sPacketUserInfoLogin.saData[iCount].iMapID == MAPID_T5QuestArena )
								sPacketUserInfoLogin.saData[iCount].iMapID = MAPID_Atlantis;
							//Is in Ghost Castle
							else if ( (sPacketUserInfoLogin.saData[iCount].iMapID == MAPID_GhostCastle) || (sPacketUserInfoLogin.saData[iCount].iMapID == MAPID_ForestDungeon) )
								sPacketUserInfoLogin.saData[iCount].iMapID = MAPID_Atlantis;
							//Is in BattleRoyale?
							else if ( (sPacketUserInfoLogin.saData[iCount].iMapID == MAPID_OldRuinen2) || (sPacketUserInfoLogin.saData[iCount].iMapID == MAPID_RoyalDesert) || 
									  (sPacketUserInfoLogin.saData[iCount].iMapID == MAPID_DragonsDungeon) )
								sPacketUserInfoLogin.saData[iCount].iMapID = MAPID_RicartenTown;
						}
						iCount++;
					}
					if ( !pcDB->Fetch() )
						break;
				}
			}
		}
		pcDB->Close();
	}
	sPacketUserInfoLogin.iCount = iCount;
	SENDPACKET( USERDATATOUSER(pcUserData), &sPacketUserInfoLogin );
	
	AccountLogin al;
	STRINGCOPY( al.szAccountName, pcUserData->szAccountName );

	// Log data
	if ( pcUserData )
		LogAccountLogin( pcUserData, al, ACCLOGID_CharacterSelectSend );

	return TRUE;
}

void AccountServer::DeleteCharacter( UserData * pcUserData, PacketDeleteCharacter * psPacket )
{
	// Create Folder
	CreateDirectoryA( "Data\\CharacterDelete", NULL );

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );

	// User alive?
	if ( pcUserData )
	{
		// Mismatch UserID and not SQL Inject code?
		if ( STRINGCOMPARE( pcUserData->szAccountName, psPacket->szUserID ) )
		{
			// Old Character
			char szFilePath[ 128 ] = { 0 };
			STRINGFORMAT( szFilePath, "Data\\CharacterDelete\\%s.chr", psPacket->szCharname );
			
			// File exists? Delete
			if ( FILEEXIST( szFilePath ) )
				DeleteFileA( szFilePath );

			// Move data to deleted folder ( backup if player delete acidentally )
			char szNewFilePath[ 128 ] = { 0 };
			
			ZeroMemory( szFilePath, 128 );
			STRINGFORMAT( szFilePath, "Data\\Character\\%s.chr", psPacket->szCharname );
			STRINGFORMAT( szNewFilePath, "Data\\CharacterDelete\\%s.chr", psPacket->szCharname );
			
			// Success?
			if ( MoveFileA( szFilePath, szNewFilePath ) )
			{
				// Move Row to deleted table characters and Delete from CharacterInfo
				if ( pcDB->Open() )
				{
					if ( pcDB->Prepare( "INSERT INTO CharacterInfoDelete SELECT * FROM CharacterInfo WHERE Name=?" ) )
					{
						pcDB->BindParameterInput( 1, PARAMTYPE_String, psPacket->szCharname );
						pcDB->Execute();
					}
					pcDB->Close();
				}
				// Delete Character Info from Database
				if ( pcDB->Open() )
				{
					if ( pcDB->Prepare( "DELETE FROM CharacterInfo WHERE Name=?" ) )
					{
						pcDB->BindParameterInput( 1, PARAMTYPE_String, psPacket->szCharname );
						pcDB->Execute();
					}
					pcDB->Close();
				}
				// Delete Premium Items from Database
				if ( pcDB->Open() )
				{
					if ( pcDB->Prepare( "DELETE FROM CharacterItemTimer WHERE CharacterName=?" ) )
					{
						pcDB->BindParameterInput( 1, PARAMTYPE_String, psPacket->szCharname );
						pcDB->Execute();
					}
					pcDB->Close();
				}
			}
		}
	}
}

void AccountServer::OnReadPostBox( UserData * pcUserData )
{

}

void AccountServer::OnDeletePostBox( UINT ItemID )
{
	
	char szUserID[ 32 ] = { 0 };
	char szItemName[ 32 ] = { 0 };
	int iSpecOrCount = 0;
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT * FROM PostBox WHERE ID=?" ) )
		{
			int iID = *(int*)(ItemID + 8);
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iID );
			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 2, PARAMTYPE_String, szUserID, 32 );
				pcDB->GetData( 3, PARAMTYPE_String, szItemName, 32 );
				pcDB->GetData( 4, PARAMTYPE_Integer, &iSpecOrCount, 0 );
			}
			pcDB->Close();
		}
	}
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "DELETE FROM PostBox WHERE ID=?" ) )
		{
			int iID = *(int*)(ItemID + 8);
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iID );
			pcDB->Execute();
		}
		pcDB->Close();
	}
	*(UINT*)(ItemID+8) = 0;

	pcDB = SQLCONNECTION( DATABASEID_LogDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "INSERT INTO PostBoxLog VALUES (?,?,?,?,?,?)" ) )
		{
			char * pszIP = "127.0.0.1";
			char szLog[256] = { 0 };
			char szDateTime[20] = { 0 };
			GetDateTime( szDateTime );
			STRINGFORMAT( szLog, POSTBOX_GETITEMLOG, szUserID, szItemName, iSpecOrCount );
			int iLogID = ACCLOGID_ReceivedPostBox;
			pcDB->BindParameterInput( 1, PARAMTYPE_String, (void*)pszIP );
			pcDB->BindParameterInput( 2, PARAMTYPE_String, szUserID );
			pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &iLogID );
			pcDB->BindParameterInput( 4, PARAMTYPE_String, szLog );
			pcDB->BindParameterInput( 5, PARAMTYPE_String, szDateTime );
			pcDB->BindParameterInput( 6, PARAMTYPE_Integer, &GAME_SERVER );
			pcDB->Execute();
		}
		pcDB->Close();
	}
}

void AccountServer::OnAccountSave( UserData * pcUserData )
{

}

BOOL AccountServer::OnDisconnectUser( UserData * pcUserData )
{
	if ( pcUserData )
	{
		if ( LOGIN_SERVER )
		{
			//USERSERVER->SetUserID( pcUserData );
			if ( pcUserData->szAccountName[0] != 0 )
			{
				SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
				if ( pcDB->Open() )
				{
					if ( pcDB->Prepare( "UPDATE CharacterInfo SET IsOnline=0 WHERE AccountName=?" ) )
					{
						pcDB->BindParameterInput( 1, PARAMTYPE_String, pcUserData->szAccountName );
						pcDB->Execute();
					}
					pcDB->Close();
				}
			}
		}
		
		if ( GAME_SERVER )
		{
			//USERSERVER->SetUserID( pcUserData );
			auto pszCharacterName = CHARACTERSERVER->GetCharacterName( pcUserData );
			if ( pszCharacterName && (pszCharacterName[0] != 0) )
			{
				SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
				if ( pcDB->Open() )
				{
					if ( pcDB->Prepare( "UPDATE CharacterInfo SET LastSeenDate=GETDATE() WHERE Name=?" ) )
					{
						pcDB->BindParameterInput( 1, PARAMTYPE_String, CHARACTERSERVER->GetCharacterName( pcUserData ) );
						pcDB->Execute();
					}
					pcDB->Close();
				}
			}
		}
	}
	return TRUE;
}

