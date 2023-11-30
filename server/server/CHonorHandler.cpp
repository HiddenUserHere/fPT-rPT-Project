#include "stdafx.h"
#include "CHonorHandler.h"

CHonorHandler::CHonorHandler()
{
}

CHonorHandler::~CHonorHandler()
{
}

void CHonorHandler::OnLoadUser( User * pcUser )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT HonorType, DateEnd FROM PvPHonor WHERE CharacterID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				SYSTEMTIME sEndDate;
				pcDB->GetData( 1, PARAMTYPE_Integer, &pcUser->iPvPHonorType );
				pcDB->GetData( 2, PARAMTYPE_Time, &sEndDate );

				pcUser->uPvPHonorTimeOut = (UINT)(SystemTimeToQWORD( &sEndDate ) - SystemTimeToQWORD( GetServerTime() ));

				if ( (int)pcUser->uPvPHonorTimeOut > 0 )
				{
					pcUser->uPvPHonorTimeOut *= 1000;
					pcUser->uPvPHonorTimeOut += TICKCOUNT;
				}
				else
				{
					pcUser->iPvPHonorType = 0;
					pcUser->uPvPHonorTimeOut = 0;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT HonorType, DateEnd FROM BellatraHonor WHERE CharacterID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				SYSTEMTIME sEndDate;
				pcDB->GetData( 1, PARAMTYPE_Integer, &pcUser->iBellatraHonorType );
				pcDB->GetData( 2, PARAMTYPE_Time, &sEndDate );

				pcUser->uBellatraHonorTimeOut = (UINT)(SystemTimeToQWORD( &sEndDate ) - SystemTimeToQWORD( GetServerTime() ));

				if ( (int)pcUser->uBellatraHonorTimeOut > 0 )
				{
					pcUser->uBellatraHonorTimeOut *= 1000;
					pcUser->uBellatraHonorTimeOut += TICKCOUNT;
				}
				else
				{
					pcUser->iBellatraHonorType = 0;
					pcUser->uBellatraHonorTimeOut = 0;
				}
			}
		}
		pcDB->Close();
	}

	SendHonors( pcUser );
}

void CHonorHandler::OnUnloadUser( User * pcUser )
{
	SQLUpdatePvPHonor( pcUser );
	SQLUpdateBellatraHonor( pcUser );
}

void CHonorHandler::SQLAddPvPHonor( int iCharacterID, PacketHonor::EHonorType eType, UINT uTimeLeft )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "INSERT INTO PvPHonor([CharacterID], [HonorType], [DateEnd]) VALUES(?,?,DATEADD(second, ?, GETDATE()))" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iCharacterID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &eType );
			pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &uTimeLeft );

			pcDB->Execute();
		}
		pcDB->Close();
	}
}

void CHonorHandler::SQLAddPvPHonor( User * pcUser, PacketHonor::EHonorType eType, UINT uTimeLeft )
{
	SQLAddPvPHonor( pcUser->iCharacterID, eType, uTimeLeft );

	pcUser->iPvPHonorType = eType;
	pcUser->uPvPHonorTimeOut = (uTimeLeft * 1000) + TICKCOUNT;
}

void CHonorHandler::SQLUpdatePvPHonor( User * pcUser )
{
	int iTimeOut = (int)(pcUser->uPvPHonorTimeOut - TICKCOUNT);
	iTimeOut	/= 1000;
	if ( iTimeOut <= 0 )
	{
		pcUser->iPvPHonorType		= 0;
		pcUser->uPvPHonorTimeOut	= 0;

		SQLDeletePvPHonor( pcUser );
	}
}

void CHonorHandler::SQLDeletePvPHonor( User * pcUser )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "DELETE FROM PvPHonor WHERE CharacterID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );

			pcDB->Execute();
		}
		pcDB->Close();
	}
}

void CHonorHandler::SQLAddBellatraHonor( int iCharacterID, PacketHonor::EHonorType eType, UINT uTimeLeft )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "INSERT INTO BellatraHonor([CharacterID], [HonorType], [DateEnd]) VALUES(?,?,DATEADD(second, ?, GETDATE()))" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iCharacterID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &eType );
			pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &uTimeLeft );

			pcDB->Execute();
		}
		pcDB->Close();
	}
}

void CHonorHandler::SQLAddBellatraHonor( User * pcUser, PacketHonor::EHonorType eType, UINT uTimeLeft )
{
	SQLAddBellatraHonor( pcUser->iCharacterID, eType, uTimeLeft );

	pcUser->iBellatraHonorType = eType;
	pcUser->uBellatraHonorTimeOut = (uTimeLeft * 1000) + TICKCOUNT;
}

void CHonorHandler::SQLUpdateBellatraHonor( User * pcUser )
{
	int iTimeOut = (int)(pcUser->uBellatraHonorTimeOut - TICKCOUNT);
	iTimeOut	/= 1000;
	if ( iTimeOut <= 0 )
	{
		pcUser->iBellatraHonorType		= 0;
		pcUser->uBellatraHonorTimeOut	= 0;

		SQLDeleteBellatraHonor( pcUser );
	}
}

void CHonorHandler::SQLDeleteBellatraHonor( User * pcUser )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "DELETE FROM BellatraHonor WHERE CharacterID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );

			pcDB->Execute();
		}
		pcDB->Close();
	}
}

void CHonorHandler::Update( ETickType eTick )
{
	if ( eTick != TICKTYPE_1Second )
		return;

	if ( LOGIN_SERVER )
	{
		auto psServerTime = GetServerTime();
		if ( psServerTime->wDayOfWeek == 1 )
		{
			if ( psServerTime->wDay != wDay )
			{
				//if ( wDay != 0 )
				{
					for ( UINT i = 0; i < USERSERVER->uUsersInGame; i++ )
					{
						User * pcUser = USERSERVER->pcaUserInGame[i];
						PVPSERVER->OnUnLoadUser( pcUser );
					}

					UpdateHonor();
				}

				wDay = psServerTime->wDay;
			}
		}
	}
}

void CHonorHandler::SendHonors( User * pcUser )
{
	PacketHonor sPacket;
	sPacket.ePvPHonor = (PacketHonor::EHonorType)pcUser->iPvPHonorType;
	sPacket.uPvPTime = (pcUser->uPvPHonorTimeOut - TICKCOUNT) / 1000;
	sPacket.eBellatraHonor = (PacketHonor::EHonorType)pcUser->iBellatraHonorType;
	sPacket.uBellatraTime = (pcUser->uBellatraHonorTimeOut - TICKCOUNT) / 1000;

	ProcessPacket( pcUser, &sPacket );
}

void CHonorHandler::UpdateHonor()
{
	SQLResetHonors();
	CHATSERVER->SendChatAll( CHATCOLOR_Global, "Honor> Bellatra Honor and PvP Honor has been reset!" );

	PacketNetUpdatePvPBellatraBuff sPacket;
	for ( int i = 0; i < 10; i++ )
	{
		for ( int j = 0; j < 3; j++ )
		{
			sPacket.iaBellatraCharacterID[i][j] = -1;
			sPacket.iaPvPCharacterID[i][j] = -1;
		}
	}

	HonorTopClass3 sPvP, sBellatra;
	if ( SQLGetPvPTop( sPvP ) )
	{
		for ( int i = 0; i < 10; i++ )
		{
			for ( int j = 0; j < 3; j++ )
			{
				sPacket.iaPvPCharacterID[i][j]	= sPvP.iaCharacterID[i][j];
				sPacket.iaPvPHonorType[i][j]	= PacketHonor::EHonorType::HONORTYPE_PvPGold + j;
			}
		}
	}

	if ( SQLGetBellatraTop( sBellatra ) )
	{
		for ( int i = 0; i < 10; i++ )
		{
			for ( int j = 0; j < 3; j++ )
			{
				sPacket.iaBellatraCharacterID[i][j] = sBellatra.iaCharacterID[i][j];
				sPacket.iaBellatraHonorType[i][j]	= PacketHonor::EHonorType::HONORTYPE_BellatraGold + j;
			}
		}
	}

	SQLResetBellatraPlayer();
	
	PVPSERVER->ResetPvPBuffer();

	NETSERVER->SendPvPUpdateGameServer( &sPacket );
}

void CHonorHandler::NetHandlePacket( PacketNetUpdatePvPBellatraBuff * psPacket )
{
	for ( UINT i = 0; i < USERSERVER->uUsersInGame; i++ )
	{
		User * pcUser = USERSERVER->pcaUserInGame[i];
		pcUser->uBellatraHonorTimeOut = 0;
		pcUser->uPvPHonorTimeOut = 0;
	}

	const int MAX_TIMEHONOR_PVP = 60 * 60 * 24 * 7;
	const int MAX_TIMEHONOR_BELLATRA = 60 * 60 * 24 * 7;

	auto SetCharacterHonor = [this]( int iCharacterID, bool bPvP, int eType, UINT uTime ) -> void
	{
		if ( iCharacterID != -1 )
		{
			bool bOnline = false;
			for ( UINT i = 0; i < USERSERVER->uUsersInGame; i++ )
			{
				User * pcUser = USERSERVER->pcaUserInGame[i];
				if ( pcUser->iCharacterID == iCharacterID )
				{
					if ( bPvP )
						SQLAddPvPHonor( pcUser, (PacketHonor::EHonorType)eType, uTime );
					else
						SQLAddBellatraHonor( pcUser, (PacketHonor::EHonorType)eType, uTime );

					bOnline = true;
					break;
				}
			}

			if ( bOnline == false )
			{
				if ( SERVER_CODE == 1 )
				{
					if ( bPvP )
						SQLAddPvPHonor( iCharacterID, (PacketHonor::EHonorType)eType, uTime );
					else
						SQLAddBellatraHonor( iCharacterID, (PacketHonor::EHonorType)eType, uTime );
				}
			}

			SQLLog( iCharacterID, (PacketHonor::EHonorType)eType, bPvP );
		}
	};

	for ( int i = 0; i < 10; i++ )
	{
		for ( int j = 0; j < 3; j++ )
		{
			SetCharacterHonor( psPacket->iaBellatraCharacterID[i][j], false, (PacketHonor::EHonorType)psPacket->iaBellatraHonorType[i][j], MAX_TIMEHONOR_BELLATRA );
			SetCharacterHonor( psPacket->iaPvPCharacterID[i][j], true, (PacketHonor::EHonorType)psPacket->iaPvPHonorType[i][j], MAX_TIMEHONOR_PVP );
		}
	}

	for ( UINT i = 0; i < USERSERVER->uUsersInGame; i++ )
	{
		User * pcUser = USERSERVER->pcaUserInGame[i];

		SendHonors( pcUser );
	}
}

void CHonorHandler::ProcessPacket( User * pcUser, PacketHonor * psPacket )
{
	psPacket->iLength = sizeof( PacketHonor );
	psPacket->iHeader = PKTHDR_HonorUpdate;

	SENDPACKET( pcUser, psPacket );
}

bool CHonorHandler::SQLGetPvPTop( HonorTopClass3 & sCharacters )
{
	std::vector<int> vCharacters;
	std::vector<SQLCharacter> vSQLCharacters;

	for ( int i = 0; i < 10; i++ )
	{
		for ( int j = 0; j < 3; j++ )
			sCharacters.iaCharacterID[i][j] = -1;
	}

	bool bRet = false;
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT TOP 100 CharacterID FROM PvPPlayerRank WHERE (Kills>0) ORDER BY Experience DESC" ) )
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
		SQLCharacter sSQLCharacter;
		if ( CHARACTERSERVER->SQLGetCharacter( iCharacterID, &sSQLCharacter ) )
		{
			vSQLCharacters.push_back( sSQLCharacter );

			bRet = true;
		}
	}

	for ( auto & s : vSQLCharacters )
	{
		for ( int i = 0; i < 10; i++ )
		{
			if ( s.iClass == (i + 1) )
			{
				for ( int j = 0; j < 3; j++ )
				{
					if ( sCharacters.iaCharacterID[i][j] < 0 )
					{
						sCharacters.iaCharacterID[i][j] = s.iID;

						break;
					}
				}
			}
		}
	}

	vCharacters.clear();
	vSQLCharacters.clear();

	return bRet;
}

bool CHonorHandler::SQLGetBellatraTop( HonorTopClass3 & sCharacters )
{
	std::vector<int> vCharacters;
	std::vector<SQLCharacter> vSQLCharacters;

	for ( int i = 0; i < 10; i++ )
	{
		for ( int j = 0; j < 3; j++ )
			sCharacters.iaCharacterID[i][j] = -1;
	}

	bool bRet = false;
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT TOP 100 CharacterID FROM BellatraCharacterInstance WHERE (Score>0) AND (BattleID=1) AND (Date > DATEADD(Day,-7,GETDATE())) GROUP BY CharacterID ORDER BY MAX(Score) DESC" ) )
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
		SQLCharacter sSQLCharacter;
		if ( CHARACTERSERVER->SQLGetCharacter( iCharacterID, &sSQLCharacter ) )
		{
			vSQLCharacters.push_back( sSQLCharacter );

			bRet = true;
		}
	}

	for ( auto & s : vSQLCharacters )
	{
		for ( int i = 0; i < 10; i++ )
		{
			if ( s.iClass == (i + 1) )
			{
				for ( int j = 0; j < 3; j++ )
				{
					if ( sCharacters.iaCharacterID[i][j] < 0 )
					{
						sCharacters.iaCharacterID[i][j] = s.iID;

						break;
					}
				}
			}
		}
	}

	vCharacters.clear();
	vSQLCharacters.clear();

	return bRet;
}

void CHonorHandler::SQLLog( int iCharacterID, PacketHonor::EHonorType eHonorType, bool bPvP )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( FormatString( "INSERT INTO %s([CharacterID], [HonorType], [Date]) VALUES(?,?,GETDATE())", bPvP ? "PvPHonorLog" : "BellatraHonorLog" ) ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iCharacterID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &eHonorType );

			pcDB->Execute();
		}
		pcDB->Close();
	}
}

void CHonorHandler::SQLResetHonors()
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "TRUNCATE TABLE PvPHonor" ) )
			pcDB->Execute();

		pcDB->Clear();
		if ( pcDB->Prepare( "TRUNCATE TABLE BellatraHonor" ) )
			pcDB->Execute();

		pcDB->Close();
	}
}

void CHonorHandler::SQLResetBellatraPlayer()
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "TRUNCATE TABLE BellatraCharacterInstance" ) )
			pcDB->Execute();

		pcDB->Close();
	}
}
