#pragma once

#define TIMER_STATUSWAIT	60000
#define MAX_CLANMEMBERS		100
#define MAX_FRIENDS			101

typedef EXESurfaceOld*( __cdecl *tfnLoadBitmap )(char * pszFile);
static tfnLoadBitmap fnLoadBitmapGlobal = (tfnLoadBitmap)0x0048A2B0;

#define LOADBMPPT		fnLoadBitmapGlobal

enum EManageWindowPage
{
	MANAGEWINDOWPAGE_None			= 0,
	MANAGEWINDOWPAGE_Friends		= 2,
	MANAGEWINDOWPAGE_ClanMembers	= 3
};

enum EManageWindowSubPage
{
	MANAGEWINDOWSUBPAGE_None		= 0,
	MANAGEWINDOWSUBPAGE_Friends		= 1
};


class CManageWindow
{
private:
	static DWORD				dwMemberStatusTimer;

	static char *			pszaClanMembers, * pszaFriends;
public:
	static bool				baClanMemberStatus[MAX_CLANMEMBERS], baFriendStatus[MAX_FRIENDS];

	static int *			piHorScroll;
	char											szUnknown01[0x0210];

	EManageWindowPage								iPage;

	char											szUnknown02[0xA550];

	int												iFriendListStartIndex;
	int												iFriendListEndIndex;
	int												iFriendListNumFriends;
	char											szUnknown03[0x1C78];
	EManageWindowSubPage							iSubPage;

	void											RenderClanMemberStatus();
	void											RenderFriendStatus();

	void											Tick();

	void											Init();

	void											PHUserStatus( struct PacketUserOnline * psPacket );

	static void										UpdateClanIcons();

};

class ClanManageWindow
{
private:
public:
	static BOOL *			pbOpen;
	static int *			piScroll;
	static int *			piPosition;
};

