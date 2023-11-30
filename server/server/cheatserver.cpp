#include "stdafx.h"
#include "cheatserver.h"


CheatServer::CheatServer()
{
}


CheatServer::~CheatServer()
{
}

void CheatServer::UnbanAccount( const char * pszUserID )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
				
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE UserInfo SET BanStatus=0, UnbanDate=NULL WHERE AccountName=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, (void*)pszUserID );
			pcDB->Execute();
		}
		pcDB->Close();
	}
}

void CheatServer::OnLoginUserPacket( UserData * pcUserData )
{

	if ( !psPacketChecksumList )
	{
		psPacketChecksumList = new PacketChecksumFunctionList;
		ZeroMemory( psPacketChecksumList, sizeof( PacketChecksumFunctionList ) );
		psPacketChecksumList->iLength = sizeof( PacketChecksumFunctionList );
		psPacketChecksumList->iHeader = PKTHDR_CheckSum;
		psPacketChecksumList->bKey = 0x6A;


		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
				
		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "SELECT * FROM FunctionChecksumList" ) )
			{
				if ( pcDB->Execute() && pcDB->Fetch() )
				{
					for ( int i = 0; i < 400; i++ )
					{
						char szAddress[32] = { 0 };
						pcDB->GetData( 2, PARAMTYPE_Integer, &psPacketChecksumList->sCheckSum[i].dwSize, 0 );
						pcDB->GetData( 3, PARAMTYPE_String, szAddress, 32 );
						psPacketChecksumList->sCheckSum[i].dwAddress = std::strtol( szAddress, nullptr, 16 );
						pcDB->GetData( 4, PARAMTYPE_Integer, &psPacketChecksumList->sCheckSum[i].dwCheckSum, 0 );
						if ( !pcDB->Fetch() )
							break;
					}
				}
			}
			pcDB->Close();
		}

		psPacketWindowList = new PacketWindowList;
		ZeroMemory( psPacketWindowList, sizeof( PacketWindowList ) );
		psPacketWindowList->iLength = sizeof( PacketWindowList );
		psPacketWindowList->iHeader = PKTHDR_WindowList;

		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "SELECT TOP 100 * FROM WindowCheatList ORDER BY ID DESC" ) )
			{
				if ( pcDB->Execute() )
				{
					while ( pcDB->Fetch() )
					{
						pcDB->GetData( 2, PARAMTYPE_String, psPacketWindowList->szaWindowName[psPacketWindowList->iCount], _countof( psPacketWindowList->szaWindowName[psPacketWindowList->iCount] ) );
						psPacketWindowList->iCount++;
					}
				}
			}
			pcDB->Close();
		}
	}
	SENDPACKET( USERDATATOUSER(pcUserData), psPacketChecksumList, TRUE );
	SENDPACKET( USERDATATOUSER(pcUserData), psPacketWindowList, TRUE );

	
}


void CheatServer::CheckStatePoint( UserData * pcUserData, DWORD dwLevelQuestLog )
{
	
}

BOOL CheatServer::OnCheckMacAddress( const char * pszUserID, const char * pszMacAddr )
{
	return FALSE;
}

void CheatServer::HandlePacket( User * pcUser, PacketCrash * psPacket )
{
	if ( psPacket->iLength == sizeof( PacketCrash ) && pcUser->pcUserData && pcUser->pcUserData->pcSocketData )
	{
		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "INSERT INTO CrashData([AccountName],[IP],[Version],[Address],[Flags],[Code],[EAX],[ECX],[EDX],[EBX],[ESP],[EBP],[ESI],[EDI],[EIP],[DateReceived]) VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,GETDATE())" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_String, psPacket->szAccountName, 32 );
				pcDB->BindParameterInput( 2, PARAMTYPE_String, pcUser->pcUserData->pcSocketData->szIP, 16 );
				pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &psPacket->dwVersion );
				pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &psPacket->pvExceptionAddress );
				pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &psPacket->dwExceptionFlags );
				pcDB->BindParameterInput( 6, PARAMTYPE_Integer, &psPacket->dwExceptionCode );
				pcDB->BindParameterInput( 7, PARAMTYPE_Integer, &psPacket->EAX );
				pcDB->BindParameterInput( 8, PARAMTYPE_Integer, &psPacket->ECX );
				pcDB->BindParameterInput( 9, PARAMTYPE_Integer, &psPacket->EDX );
				pcDB->BindParameterInput( 10, PARAMTYPE_Integer, &psPacket->EBX );
				pcDB->BindParameterInput( 11, PARAMTYPE_Integer, &psPacket->ESP );
				pcDB->BindParameterInput( 12, PARAMTYPE_Integer, &psPacket->EBP );
				pcDB->BindParameterInput( 13, PARAMTYPE_Integer, &psPacket->ESI );
				pcDB->BindParameterInput( 14, PARAMTYPE_Integer, &psPacket->EDI );
				pcDB->BindParameterInput( 15, PARAMTYPE_Integer, &psPacket->EIP );
				pcDB->Execute();
			}
			pcDB->Close();
		}
	}
}

void CheatServer::HandlePacket( User * pcUser, PacketCrashData * psPacket )
{
	if ( psPacket->iLength != sizeof( PacketCrashData ) || psPacket->iDataLen <= 0 || psPacket->iDataLen > CRASHDATA_SIZE )
		return;

	char * pszMode = "ab";
	psPacket->szAccountName[31] = 0;

	char szFileName[MAX_PATH + 1] = { 0 };

	SYSTEMTIME * ps = GetServerTime();

	CreateDirectoryA( "Data\\Crash", NULL );

	STRINGFORMAT( szFileName, "Data\\Crash\\%d_%s.dmp", psPacket->dwTick, psPacket->szAccountName[0] != 0 ? psPacket->szAccountName : "unkplayer" );

	FILE * fp = NULL;
	fopen_s( &fp, szFileName, pszMode );

	if ( fp )
	{
		fwrite( psPacket->baData, psPacket->iDataLen, 1, fp );

		fclose( fp );
	}
}
