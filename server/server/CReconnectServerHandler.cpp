#include "stdafx.h"
#include "CReconnectServerHandler.h"


CReconnectServerHandler::CReconnectServerHandler()
{
}


CReconnectServerHandler::~CReconnectServerHandler()
{
	DELETA( pcaUser );
	DELETA( pcaUserData );
	DELETA( psaCharacterFile );
}

void CReconnectServerHandler::Init()
{
	pcaUser					= new User[MAX_RECONNECT_USERS];
	pcaUserData				= new UserData[MAX_RECONNECT_USERS];
	psaCharacterFile		= (CharacterFile*)(new char[(MAX_RECONNECT_USERS * 0x6100) + 4]);

	for ( int i = 0; i < MAX_RECONNECT_USERS; i++ )
	{
		User * pcUser			= pcaUser + i;
		UserData * pcUserData	= pcaUserData + i;
		pcUserData->psCharacterFile = psaCharacterFile + i;

		pcUser->uIndex = i;
		pcUser->pcUserData = pcUserData;
	}
}

BOOL CReconnectServerHandler::AddUser( User * pcUser )
{
	//Have old User to reconnect? Free it
	if ( GetUser( pcUser ) )
		FreeUser( pcUser );

	//Get a new User
	User * pcNewUser = GetFreeUser();
	if ( pcNewUser )
	{
		//Swap
		int iIndex = GetIndex( pcNewUser );
		if ( iIndex >= 0 )
		{
			pcNewUser->Swap( pcUser );
			pcNewUser->bInUse = TRUE;

			//Warehouse? Close
			if ( pcUser->pcUserData->pcWarehouse )
			{
				WAREHOUSEHANDLER->Close( pcUser->pcUserData->pcWarehouse );
				pcUser->pcUserData->pcWarehouse		= NULL;
				pcNewUser->pcUserData->pcWarehouse	= NULL;
			}

			//??
			if ( pcUser->pcUserData->psUnknown1 )
			{
				DELET( pcUser->pcUserData->psUnknown1 );
				pcNewUser->pcUserData->psUnknown1 = NULL;
			}
		}
	}

	return TRUE;
}

void CReconnectServerHandler::HandlePacket( User * pcUser, PacketReconnectUser * psPacket )
{
	BOOL bInGame = FALSE;
	CharacterFile * psCharacterFile = NULL;
	SocketData * pcSocketData = NULL;

	pcUser->pcUserData->bInGame = TRUE;

	//Have User to reconnect?
	User * pcUserReconnect = USERDATATOUSER( USERDATABYID( psPacket->iID ) );
	if ( pcUserReconnect )
	{
		pcSocketData = pcUserReconnect->pcUserData->pcSocketData;
		psCharacterFile = pcUserReconnect->pcUserData->psCharacterFile;

		bInGame = TRUE;
	}

	if ( pcUserReconnect )
	{
		//Swap
		pcUser->Swap( pcUserReconnect );
		pcUser->pcUserData->iID = psPacket->iID;
		pcUser->pcUserData->bInGame = TRUE;

		//Add to Users Online
		if ( (pcUser->pcUserData->szAccountName[0] != 0) && (CHARACTERSERVER->GetCharacterName( pcUser->pcUserData )[0] != 0) && pcUser->pcUserData->pcSocketData )
		{
			AccountServer::SQLUserOnline( pcUser->pcUserData->szAccountName, "", "", 0, 1, 0, 0 );
			AccountServer::SQLUserOnline( pcUser->pcUserData->szAccountName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), pcUser->pcUserData->pcSocketData->szIP, pcUser->pcUserData->iTicket, 0, pcUser->pcUserData->sCharacterData.iClass, pcUser->pcUserData->sCharacterData.iLevel );
		}

		//Quest
		if ( GAME_SERVER )
			QUESTARENAHANDLER->OnReconnect( pcUser );

		if ( bInGame )
		{
			if ( psCharacterFile )
				psCharacterFile->iLength = 0;

			if ( pcSocketData )
			{
				WNDPROC_SOCKETCLOSE( SOCKETSERVER->GetHWND(), pcSocketData );
			}
		}

		SENDPACKETBLANK( pcUser, LOGIN_SERVER ? PKTHDR_ReconnectLogin : PKTHDR_Reconnect );
	}
}

User * CReconnectServerHandler::GetUser( User * pcUser )
{
	User * pcUserRet = NULL;

	if ( pcUser->GetID() )
		pcUserRet = GetUser( pcUser->GetID() );

	return pcUserRet;
}

User * CReconnectServerHandler::GetUser( int iID )
{
	User * pcUserRet = NULL;

	for ( int i = 0; i < MAX_RECONNECT_USERS; i++ )
	{
		User * pc = pcaUser + i;
		if ( pc->GetID() == iID )
		{
			pcUserRet = pc;
			break;
		}
	}

	return pcUserRet;
}

BOOL CReconnectServerHandler::FreeUser( User * pcUser )
{
	if ( pcUser->GetID() )
	{
		for ( int i = 0; i < MAX_RECONNECT_USERS; i++ )
		{
			User * pc = pcaUser + i;
			if ( pc->GetID() == pcUser->GetID() )
			{
				pc->bInUse = FALSE;
				break;
			}
		}
	}

	return TRUE;
}

int CReconnectServerHandler::GetIndex( User * pcUser )
{
	int iIndex = -1;

	for ( int i = 0; i < MAX_RECONNECT_USERS; i++ )
	{
		User * pc = pcaUser + i;
		if ( pc == pcUser )
		{
			iIndex = i;
			break;
		}
	}

	return iIndex;
}

User * CReconnectServerHandler::GetFreeUser()
{
	User * pcUserRet = NULL;
	for ( int i = 0; i < MAX_RECONNECT_USERS; i++ )
	{
		User * pc = pcaUser + i;
		if ( (pc->bInUse == FALSE) || ((pc->pcUserData->dwTimeLastPacket + (5 * 60 * 1000)) < TICKCOUNT) )
		{
			pcUserRet = pc;
			pc->bInUse = TRUE;
			break;
		}
	}

	return pcUserRet;
}

CharacterFile * CReconnectServerHandler::GetNewCharacterFile( int iIndex )
{
	return (psaCharacterFile + iIndex);
}
