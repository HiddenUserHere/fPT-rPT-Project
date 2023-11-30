#include "stdafx.h"
#include "ManageWindow.h"

int iImageFriendOff		= -1;
int iImageClanFriendOff = -1;

int * CManageWindow::piHorScroll = (int*)0x03911EAC;
char * CManageWindow::pszaClanMembers = (char *)0x03AFACA0, * CManageWindow::pszaFriends = (char *)0x0390ED70;
bool CManageWindow::baClanMemberStatus[MAX_CLANMEMBERS], CManageWindow::baFriendStatus[MAX_FRIENDS];
DWORD CManageWindow::dwMemberStatusTimer = 0;

void CManageWindow::Init()
{
	iImageClanFriendOff = (int)LOADBMPPT( "game\\images\\UI\\hud\\offline.bmp" );
	iImageFriendOff		= (int)LOADBMPPT( "game\\images\\UI\\hud\\offchatf.bmp" );

}

void CManageWindow::RenderClanMemberStatus()
{

	int l_iPosListX = DXGraphicEngine::GetWidthBackBuffer();
	int l_iPosListY = DXGraphicEngine::GetHeightBackBuffer();

	for( int j = 0, i = (*ClanManageWindow::piScroll); i < ((*ClanManageWindow::piScroll) + 8); i++, j++ )
	{
		if( i > MAX_CLANMEMBERS )
			break;

		char * pszClanMember = pszaClanMembers + (i * 30);

		if( pszClanMember[0] == 0 )
			continue;

		if ( !baClanMemberStatus[i] )
		{
			DRAWBMP( l_iPosListX - 181, l_iPosListY - 395 + (j * 20), (EXESurfaceOld*)iImageClanFriendOff, 0, 0, 16, 16, TRUE );
		}
	}
}
void CManageWindow::RenderFriendStatus()
{
	int l_iPosListX = DXGraphicEngine::GetWidthBackBuffer();
	int l_iPosListY = DXGraphicEngine::GetHeightBackBuffer();

	int k = 0;
	for( int j = 0, i = iFriendListStartIndex; i < iFriendListEndIndex; i++, j++ )
	{
		if( (i > MAX_FRIENDS) || (i >= iFriendListNumFriends) )
			break;

		if ( !baFriendStatus[i] )
		{
			if ( (*(BOOL*)0x0039109F4) && ((*(DWORD*)0x0039109F0)-3) == i )
			{
				k++;
				continue;
			}

			DRAWBMP( l_iPosListX - 184, l_iPosListY - 392 + (k * 20), (EXESurfaceOld*)iImageFriendOff, 0, 0, 16, 16, TRUE );
			k++;
		}
	}
}

void CManageWindow::PHUserStatus( struct PacketUserOnline * psPacket )
{
	if ( psPacket->iType == 0 )
	{
		if ( psPacket->iID >= 0 && psPacket->iID < MAX_CLANMEMBERS )
			baClanMemberStatus[psPacket->iID] = psPacket->bOnline ? true : false;
	}
	else
	{
		if ( psPacket->iID >= 0 && psPacket->iID < MAX_FRIENDS )
			baFriendStatus[psPacket->iID] = psPacket->bOnline ? true : false;
	}
}

void CManageWindow::UpdateClanIcons()
{
	if ( Game::GetGameMode() != GAMEMODE_InGame )
		return;
/*
	char szPath[MAX_PATH];
	char szClanID[32];

	ClanInfo * psClanInfo = (ClanInfo*)0x03AA4608;

	for ( int i = 0; i < 500; i++ )
	{
		ClanInfo * ps = &psClanInfo[i];

		if ( ps->iStatus == 2 )
		{
			if ( ps->psTexture16 == NULL )
			{
				STRINGFORMAT( szClanID, "10_%06d", ps->sHeader.iClanID - 1000000000 );
				STRINGFORMAT( szPath, "save\\clanDATA\\Fortress World\\%s_16.bmp", szClanID );
				ps->psTexture16 = LOADBMPPT( szPath );
				if ( ps->psTexture16 == NULL )
					ps->iStatus = 0;
			}
			if ( ps->psTexture32 == NULL )
			{
				STRINGFORMAT( szClanID, "10_%06d", ps->sHeader.iClanID - 1000000000 );
				STRINGFORMAT( szPath, "save\\clanDATA\\Fortress World\\%s.bmp", szClanID );
				ps->psTexture32 = LOADBMPPT( szPath );
				if ( ps->psTexture32 == NULL )
					ps->iStatus = 0;
			}
		}
	}

	if ( UNITDATA && READDWORD( 0x03B18BC4 ) == 0 && READDWORD( 0x03B18BC0 ) != 0 && UNITDATA->sCharacterData.iClanID != 0 )
	{
		STRINGFORMAT( szClanID, "10_%06d", UNITDATA->sCharacterData.iClanID - 1000000000 );
		STRINGFORMAT( szPath, "save\\clanDATA\\Fortress World\\%s.bmp", szClanID );
		WRITEDWORD( 0x03B18BC4, (DWORD)LOADBMPPT( szPath ) );
	}

	*/
}

void CManageWindow::Tick()
{
	if ( Game::GetGameMode() == GAMEMODE_InGame && TICKCOUNT > CManageWindow::dwMemberStatusTimer )
	{
		for ( int i = 0; i < MAX_CLANMEMBERS; i++ )
		{
			char * pszClanMember = pszaClanMembers + (i * 30);

			if( pszClanMember[0] == 0 )
				continue;

			PacketUserOnline sPacket;
			sPacket.iLength = sizeof( PacketUserOnline );
			sPacket.iHeader = PKTHDR_UserOnline;
			sPacket.iType = 0;
			sPacket.iID = i;
			STRINGCOPY( sPacket.szName, pszClanMember );
			SENDPACKETL( &sPacket, TRUE );
		}
		for ( int i = 0; i < MAX_FRIENDS; i++ )
		{
			char * pszFriend = pszaFriends + (i * 32);

			if( pszFriend[0] == 0 )
				continue;

			PacketUserOnline sPacket;
			sPacket.iLength = sizeof( PacketUserOnline );
			sPacket.iHeader = PKTHDR_UserOnline;
			sPacket.iType = 1;
			sPacket.iID = i;
			STRINGCOPY( sPacket.szName, pszFriend );
			SENDPACKETL( &sPacket, TRUE );
		}
		CManageWindow::dwMemberStatusTimer = TICKCOUNT + TIMER_STATUSWAIT;
	}
}

BOOL * ClanManageWindow::pbOpen = (BOOL *)0x03B1D478;
int * ClanManageWindow::piScroll = (int *)0x03ACA968;
int * ClanManageWindow::piPosition = (int *)0x03911EAC;
