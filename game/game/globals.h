#ifndef _GLOBAL_H_
#define _GLOBAL_H_

//#define IP_FORTRESS_WORLD		"51.161.12.157"
#define IP_FORTRESS_WORLD		"176.31.126.139"
#define IP_FORTRESS_BETA		"74.122.192.117" //"74.122.192.116"
//#define IP_FORTRESS_BETA		"127.0.0.1" //"74.122.192.116"

//#define TEST_SCREEN
#define NEW_USERINTERFACE

#define _NEW_DMG__

#define MAX_UNITS		1024

#define WM_UPDATE			WM_USER +   1
#define WM_WINDOWREADY		WM_USER +  51
#define WM_SOCKETACCEPT		WM_USER + 101
#define WM_SOCKETPACKET		WM_USER + 102
#define WM_SOCKETCLOSE		WM_USER + 103

//#define _PT_NEW3DENGINE

#if defined(_GAME)
class Delta3D::Graphics::Model;
class Delta3D::Graphics::Mesh;

#ifdef _PT_NEW3DENGINE

#define PTModel Delta3D::Graphics::Model
#define PTModel_ptr Delta3D::Graphics::Model*

#define PTMesh Delta3D::Graphics::Mesh
#define PTMesh_ptr Delta3D::Graphics::Mesh*


#else
#define PTModel EXEModel
#define PTModel_ptr EXEModel*

#define PTMesh EXEMesh
#define PTMesh_ptr EXEMesh*

#endif

#else

#define PTModel EXEModel
#define PTModel_ptr EXEModel*

#define PTMesh EXEMesh
#define PTMesh_ptr EXEMesh*

#endif

static const BOOL XMAS_EVENT_THINGS = FALSE;
static const BOOL HALLOWEEN_EVENT_THINGS = FALSE;

//Urho3D
#define URHO3D_CXX11
#define URHO3D_ANGELSCRIPT
#define URHO3D_FILEWATCHER
#define URHO3D_IK
#define URHO3D_LOGGING
#define URHO3D_LUA
#define URHO3D_MINIDUMPS
#define URHO3D_NAVIGATION
#define URHO3D_NETWORK
#define URHO3D_PHYSICS
#define URHO3D_PROFILING
#define URHO3D_THREADING
#define URHO3D_URHO2D
#define URHO3D_WEBP
#define URHO_SDL

#define ONLOSTDEVICE(p)		{if(!p)return; p->OnLostDevice();}
#define ONRESETDEVICE(p)	{if(!p)return; p->OnResetDevice();}

#define SAVESTATEBLOCK(p)	{if(!p)GRAPHICENGINE->GetDevice()->CreateStateBlock( D3DSBT_ALL, &p );}
#define RESTORESTATEBLOCK(p)	{if(p)p->Apply();}


//Global Functions
#include "def.h"
#include "strings.h"
#include "common.h"
#include "point.h"

#include "X3D.h"

//System
#include "CSystem.h"
#include "CApplication.h"
#include "CWindow.h"
#include "Timer.h"
#include "CTimer.h"

//File Library
#include "CReader.h"
#include "CReaderFile.h"

//Definitions
#include "sound.h"
#include "packets.h"
#include "IOCP.h"
#include "achievement.h"
#include "map.h"
#include "quest.h"
#include "party.h"
#include "chat.h"
#include "clan.h"
#include "debug.h"
#include "warehouse.h"
#include "caravan.h"
#include "mount.h"
#include "christmas.h"
#include "user.h"
#include "account.h"

#include "actionfield.h"
#include "battleroyale.h"

#include "ranking.h"

#include "itemsetattribute.h"

#include "sky.h"

#include "Game.h"
#include "CGameWindow.h"
#include "DX.h"
#include "RecvPacket.h"
#include "GameCore.h"

#include "socket.h"
#include "Coin.h"
#include "CoinShopTab.h"
#include "CoinShop.h"


#include "EXE.h"
#include "EXERefactorEngine3D.h"


static void __cdecl TitlePTBox( const char * format, ... )
{
	char szBuf[ 256 ];
	ZeroMemory( szBuf, 256 );
	va_list ap;
	va_start( ap, format );
	_vsnprintf_s( szBuf, 256, 255, format, ap );
	va_end( ap );

	CTITLEBOX->SetText( szBuf, 3000 );
}

static void __cdecl TeleportPlayer( int iStage )
{
	CALL_WITH_ARG1( 0x004441D0, ( DWORD )iStage );
}

static void __cdecl SetPlayerPattern( UnitData * Player, char * szModelFile, char * szModelFile2 )
{
	typedef int( __cdecl *t_SetPlayerPattern ) ( UnitData *, char *, char * );
	t_SetPlayerPattern SetPlayerPattern = ( t_SetPlayerPattern )0x0043C410;

	SetPlayerPattern( Player, szModelFile, szModelFile2 );
}

static void __cdecl PlaySoundMini( int iMiniSoundCode, int iVolume = 400 )
{
	CALL_WITH_ARG2( 0x00543A30, iMiniSoundCode, iVolume );
}

static void __cdecl PlayUnitSound( UnitData * pcUnitData )
{
	CALL_WITH_ARG1( 0x00543F80, (DWORD)pcUnitData );
}

static int __cdecl GetSpeedFrame( int iCharacterSpeed, int iSpeed )
{
	int ir = 0;
	__asm
	{
		PUSH iSpeed;
		PUSH iCharacterSpeed;
		MOV EAX, 0x0045F7D0;
		CALL EAX;
		ADD ESP, 0x08;
		MOV ir, EAX;
	}
	return ir;
}

enum
{
	CHARSTATUSCOLOR_White,
	CHARSTATUSCOLOR_Red,
	CHARSTATUSCOLOR_Yellow,
	CHARSTATUSCOLOR_Blue,
	CHARSTATUSCOLOR_Green,
	CHARSTATUSCOLOR_Gold,
	CHARSTATUSCOLOR_Pink,
	CHARSTATUSCOLOR_Orange,
};

enum
{
	CHARSTATUSTYPECOLOR_AttackRating,
	CHARSTATUSTYPECOLOR_Damage,
	CHARSTATUSTYPECOLOR_Defense,
	CHARSTATUSTYPECOLOR_Absorb,
	CHARSTATUSTYPECOLOR_Speed,
	CHARSTATUSTYPECOLOR_HP,
	CHARSTATUSTYPECOLOR_MP,
	CHARSTATUSTYPECOLOR_SP,

};

enum EMouseIcon : int
{
	EMOUSEICON_None,
	EMOUSEICON_Default,
	EMOUSEICON_Sell,
	EMOUSEICON_Buy,
	EMOUSEICON_Repair,
	EMOUSEICON_RepairAll,
	EMOUSEICON_Attack,
	EMOUSEICON_GetItem1,
	EMOUSEICON_GetItem2,
	EMOUSEICON_Talk,
	EMOUSEICON_GlamorousBrush1,
	EMOUSEICON_GlamorousBrush2,
};


static void SetTextStatusColor( int iStatus, int iColor )
{
	(*(int*)(0x0370713C + (iStatus * 4))) = iColor;
}

#define POLYCOUNT							(*(int*)0x02444BF0)

#define COLOR_R								(*(int*)0x00CF4778)
#define COLOR_G								(*(int*)0x00CF477C)
#define COLOR_B								(*(int*)0x00CF4780)

#define BRIGHTNESS							(*(int*)0x00A17560)

#define BLURTIME							(*(DWORD*)0x00A17564)

#define ANGLE_X								(*(int*)0x00824FA4)

#define GAME_DISTANCE						(*(int*)0x00824FB0)

#define ACTIONMODE							(*(int*)0x00CF4818)
#define ACTIONMODE_MOVE						(*(int*)0x009E2188)

#define MINIMIZED_GAME						(*(BOOL*)0x00A17550)

#define TEXTUREFRAME						(*(int*)0x03500CC0)
#define ISTEXTUREREADY						(*(BOOL*)0x0082C124)

#define TRACECAMERA							((Point3D*)0x00A15DE8)
#define TRACETARGET							((Point3D*)0x00A16254)
#define ISAUTOCAMERA						(*(BOOL*)0x00A1757C)
#define ISBIGMAP							(*(BOOL*)0x02E45824)
#define CAMERA_MOVE_STEP					(8<<8)
#define FIELDGAME							((BaseMap**)0x00CF4748)

#define VIEWPOINTSIGHT						(*(int*)0x0082BF34)
#define ZDISTANCEMIN						(*(int*)0x0082BF38)
#define RENDERSIGHTPOSITION					(*(int*)0x00A1768C)

#define CHARACTERMESSAGE					((UnitData**)0x00A16418)
#define CHARACTERMESSAGE_COUNT				(*(int*)0x00A16250)

#define HIDERIGHTNPCBOX						{WRITEDWORD( 0x0207CA64, 0 );}
#define SHOWRIGHTNPCBOX						{WRITEDWORD( 0x0207CA64, 1 );}

#define STAGE1								(*(Stage**)0x00CF4740)
#define STAGE2								(*(Stage**)0x00CF4744)

#define RESTARTCOUNTER						(*(int*)0x00CF4770)

#define CASTLEMASTERID						(READDWORD(0x0392E08C))

#define DARKCOLOR_AMOUNT					(*(int*)0x00CF4774)
#define DARKLIGHTCOLOR_RANGE				(*(int*)0x00826D0C)

#define ISSHOW_MINIMAP						(*(BOOL*)0x035E11D8)
#define ISSHOW_INTERFACE					(*(BOOL*)0x00824F80)
#define FOLLOWMODE_ACTION					(*(BOOL*)0x00CF4894)

#define PLAYER_BACKUP						*( UINT* )0x0CF481C

#define UNITDATA_SELECTED					(*(UnitData**)0x00CF489C)
#define UNITDATA_FOCUS						(*(UnitData**)0x00CF48A0)
#define ITEMVIEW_SELECTED					(*(DropItemView**)0x00CF48A4)

#define DEBUG_GM							(*(BOOL*)0x00A1758C)
#define DEBUG_USER							(*(BOOL*)0x00A17590)
#define SHOP_OPEN							(*(BOOL*)0x03653E40)
#define PERSONALSHOP_OPEN					(*(BOOL*)0x0363C110)
#define PERSONALSHOP_OPEN_SELLING			(*(BOOL*)0x03642090)
#define GLOBALWINDOW_BLOCKACTION			(*(BOOL*)0x0363C118)
#define MOUSE_ICON							(*(EMouseIcon*)0x00999308)

#define ISPRESSEDKEY(c)						(*(BYTE*)(0x00A16318 + c))
#define ISMOUSELEFT_CLICKED					(*(BOOL*)0x00A17568)

#define RUN_UIMODE							(*(BOOL*)0x035E11D0)

#define SETPLAYERPATTERN					SetPlayerPattern
#define CURRENT_TARGET						(*(UnitData**)0x0CF48D0)
#define	TARGET_UNITDATA						((UnitData*)(0x0207C888))
#define GM_MODE								*( int* )0x00CDE1CC
#define TITLEBOX							TitlePTBox
#define TICKCOUNT							GetTickCount()
#define ITEMINDEX_INVENTORY					((*(int*)0x35EB65C)-1)
#define PLAYMINISOUND						PlaySoundMini
#define PLAYUNITSOUND						PlayUnitSound

#define CAMERAPOSITION						((Point3D*)0x00A15DE8)
#define CAMERATARGETPOSITION				((Point3D*)0x00A16254)

#define UNITTARGETPOSITION_X				(*(int*)0x00A1CCD4)
#define UNITTARGETPOSITION_Y				(*(int*)0x00A186A8)
#define UNITTARGETPOSITION_Z				(*(int*)0x00A1CCC0)

#define UNITTARGETANGLE_X					(*(int*)0x00A1BCB4)
#define UNITTARGETANGLE_Y					(*(int*)0x00A180A0)
#define UNITTARGETANGLE_Z					(*(int*)0x00A18090)

#define SETSTATUSCOLOR						SetTextStatusColor

// Definitions
extern int iGamePort;
#define PORT_GAME							iGamePort

#define UNITPLAYER							( Unit::GetUnit(UNITDATA) )
#define GETUNIT(p)							(Unit::GetUnit(p))
#define PLAYERDATA							( *( CharacterData**)0x0362A3B8 )
#define SKILLSCHARACTER						( ( struct Skill* )0x3670D78)
#define	DISCONNECT_TIME						(*(DWORD*)0x03A97670)
#define	DISCONNECT_TYPE						(*(DWORD*)0x03A97674)


// D3D
#define ANIMOBJTREE							Game::AnimObjectTree

#define GETSPEEDFRAME						GetSpeedFrame

#define PTSEN								(*(int**)(0x0082C104))
#define PTCOS								(*(int**)(0x0082C108))

#define TELEPORTPLAYER						TeleportPlayer

#define RESOLUTION_WIDTH					DX::Graphic::GetWidthBackBuffer()
#define RESOLUTION_HEIGHT					DX::Graphic::GetHeightBackBuffer()

#define WM_WINDOWREADY		WM_USER +  51

#endif