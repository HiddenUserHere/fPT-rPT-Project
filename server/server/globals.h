#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#define WM_UPDATE			WM_USER +   1
#define WM_WINDOWREADY		WM_USER +  51
#define WM_SOCKETACCEPT		WM_USER + 101
#define WM_SOCKETPACKET		WM_USER + 102
#define WM_SOCKETCLOSE		WM_USER + 103


#define PTModel EXEModel
#define PTModel_ptr EXEModel*

#define PTMesh EXEMesh
#define PTMesh_ptr EXEMesh*

#include "CApplication.h"

//Global Functions
#include "def.h"
#include "common.h"
#include "strings.h"
#include "point.h"

//File Library
#include "CReader.h"
#include "CReaderFile.h"
#include "CMutex.h"

#include "packets.h"
#include "IOCP.h"
#include "socket.h"


#include "CSystem.h"

//Definitions
#include "sound.h"
#include "achievement.h"
#include "map.h"
#include "quest.h"
#include "party.h"
#include "chat.h"
#include "warehouse.h"
#include "bellatra.h"
#include "battlearena.h"
#include "battleroyale.h"
#include "caravan.h"
#include "christmas.h"
#include "horde.h"
#include "Coin.h"
#include "CoinShop.h"
#include "clan.h"
#include "debug.h"
#include "unit.h"
#include "account.h"
#include "user.h"

#include "skill.h"

#include "point.h"

#include "room.h"
#include "actionfield.h"

#include "mount.h"

#include "ranking.h"

#include "SQLConnection.h"
#include "server.h"


#define IDI_ICON			107

#define WINDOW_WIDTH		300
#define WINDOW_HEIGHT		120




extern SYSTEMTIME								sLocalTimeServer;


static void UpdateServerTime()
{
	//GetLocalTime( &sLocalTimeServer );
}

static SYSTEMTIME * GetServerTime()
{
	GetLocalTime( &sLocalTimeServer );
	return &sLocalTimeServer;
}

extern int								iEventExpGlobal;
extern int								iServerCodeGlobal;
extern int								iRateExp;
extern BOOL								bFreeTradeChat;
extern BOOL								bHalloweenEvent;
extern int								iRateHalloweenMonster;
extern BOOL								bChristmasEvent;
extern int								iRateChristmasMonster;
extern int								iRateChristmasMonsterEXP;
extern int								iRateMoriphMonster;
extern int								iRateWolfMonster;
extern BOOL								bEasterEvent;
extern int								iEasterEventRate;
extern int								iEasterEventRateMapEXP;
extern BOOL								bCarnivalEvent;
extern BOOL								bEventGirlFree;
extern int								iBlessCastleMode;
extern BOOL								bAgingFreeEvent;
extern int								iReduceAgingRateEvent;
extern BOOL								bTheGrandFury;
extern BOOL								bActionFieldEvent;
extern BOOL								bPvPServerMode;
extern int								iRateExpPvPMode;
extern int								iRateDropPvPMode;
extern int								iSuperPowerEvent;


#define USERDATATOUSER(ud)								(USERSERVER->UserdatatoUser(ud))
#define USERDATABYID(id)								(UserServer::GetUserdata((int)id))
#define USERDATABYNAME(n)								(UserServer::GetUserdata((char*)n))
#define USERDATABYACCOUNTNAME(n)								(UserServer::GetUserdataByAccount((char*)n))
#define UNITDATABYIDMAP(id,m)							(UnitServer::UnitDataByIDMap(id,m))
#define UNITDATATOUNIT(ud)								(UNITSERVER->UnitDataToUnit(ud))

// Definition Server
#define HWND_SERVER						( *( HWND* )0x07258F4 )
#define SERVER_CODE						( *( int* )0x00845858 )
#define TOTAL_SERVERS					( *( int* )0x0084A578 )
#define MAX_USERS						( *( int* )0x00845868 )
#define USERS_ONLINE					( *( int* )0x07AC9BC8 ) 
#define SERVER_VERSION_MIN				( *( int* )0x00845988 )
#define SERVER_VERSION_MAX				( *( int* )0x0084598C )
#define SERVER_VERSION					( *(int*)0x006E4F5C)
#define SERVER_WINDOW					( *( int* )0x008487B8 )
#define RECORD_SAVE_CHARACTER			( *( int* )0x00845870 )
#define SERVER_NAME						( ( char* )0x00845A40 )
#define BOSS_TIME						( *( int* )0x006E46A8 )
#define CHANNEL_PACKET_SERVER			( 0x0084A550 )
#define USERSDATA						( *( UserData** )0x007AAC888 )
#define UNITSDATA						( *( UnitData** )0x007AC3E78 )
#define MAPSDATA						( *(Map**)0x07AC9CF4)
#define MAX_UNITS						4096
#define PLAYERS_MAX						1024
#define NET_SERVER_PORT					"30010"
#define NET_SERVER_MAX_CONNECTION		3
#define WM_NET_SERVER_SOCKET			0x6A60
#define ALLOCMEM						fnAllocMemoryPT
#define DEALLOCMEM						fnDeAllocMemoryPT
#define ServerTime						GetServerTime
#define FREE_TRADECHAT					bFreeTradeChat
#define TIMEHOUR						(*(UINT*)0x07AAC88C)
#define FREE_EVENTGIRL					bEventGirlFree


#define	PARTY_GETTING_DIST				(18*64*18*64)

#define SQLCONNECTION					(SQLConnection::GetConnection)


#define LOG								LogServer::OnLog
#define LOGEx							LogServer::OnLogEx

//						Events
// Bless Castle
#define EVENT_BLESSCASTLE				( *( int* )0x00846024 )
#define EVENT_BLESSCASTLE_DAY			( *( short* )0x00846028 )
#define EVENT_BLESSCASTLE_HOUR			( *( short* )0x0084602A )
#define EVENT_BLESSCASTLE_MODE			iBlessCastleMode

// Bellatra
#define EVENT_BELLATRA					( *( int* )0x00845FEC )
#define EVENT_BELLATRA_DIVSCORE			( *( int* )0x006E46A4 )
#define EVENT_BELLATRA_TAX				( *( int* )0x006E46A0 )
#define EVENT_BELLATRA_TEXT				( ( char* )0x07AC9E40 )

//Some Events
//Haloween
#define EVENT_HALLOWEEN					bHalloweenEvent
#define RATE_HALLOWEEN					iRateHalloweenMonster

//Aging Free
#define EVENT_AGINGFREE					bAgingFreeEvent
#define EVENT_AGINGREDUCEFAIL			iReduceAgingRateEvent

//Xmas
#define EVENT_XMAS						bChristmasEvent
#define RATE_XMAS						iRateChristmasMonster
#define RATE_XMAS_EXPMAP				iRateChristmasMonsterEXP

//TGF
#define THEGRANDFURY_FLAG				bTheGrandFury

//Wanted Wolf
#define EVENT_WANTEDWOLF				( *( int* )0x00846010 )

//Wanted Moriph
#define EVENT_WANTEDMORIPH				( *( int* )0x0084600C )
#define EVENT_WANTEDMORIPH_EXPMAP		iRateMoriphMonster
#define EVENT_WANTEDWOLF_EXPMAP			iRateWolfMonster

//#define EVENT_VALENTINEDAY				( *( BYTE* )0x00846004 )
#define RATE_EXP						iRateExp
#define RATE_EXPPVPMODE					iRateExpPvPMode
#define RATE_DROPPVPMODE				iRateDropPvPMode
#define EVENT_EXP						iEventExpGlobal

#define EVENT_EASTER					bEasterEvent
#define EVENT_EASTER_RATE				iEasterEventRate
#define EVENT_EASTER_RATEMAPEXP			iEasterEventRateMapEXP
#define EVENT_MONSPAWN_EASTER			"Easter Rabbie"
#define EVENT_CARNIVAL					bCarnivalEvent
#define EVENT_REDUCE_DAMAGEMON_LIVE		(*(BOOL*)0x07AC9D4C)
#define EVENT_REDUCE_DAMAGEMON			(*(BOOL*)0x00845FF0)

#define EVENT_ACTIONFIELD				bActionFieldEvent

//PvP Server Mode
#define EVENT_PVPMODE					bPvPServerMode

#define EVENT_SUPERPOWER				iSuperPowerEvent

#endif