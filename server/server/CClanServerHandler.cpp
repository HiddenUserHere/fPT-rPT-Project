#include "stdafx.h"
#include "CClanServerHandler.h"


CClanServerHandler::CClanServerHandler()
{
}


CClanServerHandler::~CClanServerHandler()
{
}

void CClanServerHandler::HandlePacket( User * pcUser, PacketGetClanInfo * psPacket )
{

}

void CClanServerHandler::ProcessPacket( User * pcUser, PacketClanInfoData * psPacket )
{
	psPacket->iLength = sizeof( PacketClanInfoData );
	psPacket->iHeader = PKTHDR_ClanInfoData;
}

BOOL CClanServerHandler::SQLGetClanInfoData( ClanInfoHeader & sHeader )
{
	BOOL bRet = FALSE;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ClanDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "" ) )
		{

		}
		pcDB->Close();
	}

	return bRet;
}
