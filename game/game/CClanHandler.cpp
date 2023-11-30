#include "stdafx.h"
#include "CClanHandler.h"


CClanHandler::CClanHandler()
{
}


CClanHandler::~CClanHandler()
{
}

void CClanHandler::Update( float fTime )
{
	UpdateGetClanInfo();
}

int CClanHandler::GetNewClanInfoIndex( int iClanID )
{
	int iIndex = -1;

	ClanInfo * psaClanInfo = GetClanInfo();

	for ( int i = 0; i < MAX_CLANINFO; i++ )
	{
		ClanInfo * ps = psaClanInfo + i;
		if ( ps->sHeader.iClanID && (ps->sHeader.iClanID == iClanID) )
		{
			if ( (ps->iStatus == CLANSTATUSINFO_Done) || (ps->iStatus == CLANSTATUSINFO_Updating) )
			{
				iIndex = i;
				break;
			}
		}
	}

	if ( iIndex == (-1) )
	{
		for ( int i = 0; i < MAX_CLANINFO; i++ )
		{
			ClanInfo * ps = psaClanInfo + i;
			if ( (ps->iStatus == CLANSTATUSINFO_Disabled) || (ps->iStatus == CLANSTATUSINFO_Error) )
			{
				iIndex = i;
				break;
			}
		}
	}

	return iIndex;
}

void CClanHandler::UpdateGetClanInfo()
{
	ClanInfo * psaClanInfo = GetClanInfo();

	for ( int i = 0; i < MAX_CLANINFO; i++ )
	{
		ClanInfo * ps = psaClanInfo + i;
		if ( ps->sHeader.iClanID && (ps->iStatus == CLANSTATUSINFO_Updating) && ((ps->dwTime + MAX_TIME_CLANUPDATE) < TICKCOUNT) )
		{
			ProcessPacket( &PacketGetClanInfo( ps->sHeader.iClanID ) );
			ps->dwTime = TICKCOUNT;
		}
	}
}

void CClanHandler::ProcessPacket( PacketGetClanInfo * psPacket )
{
	psPacket->iLength = sizeof( PacketGetClanInfo );
	psPacket->iHeader = PKTHDR_GetClanInfo;
	SENDPACKETL( psPacket );
}
