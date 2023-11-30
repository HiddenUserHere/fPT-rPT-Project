#pragma once

#include "user.h"
#include "character.h"

#define _REQUIREACCOUNTVALIDATION

#define MAX_ACCOUNTNAMELEN			32
#define MAX_PASSWORDLEN				32
#define MAX_ACCOUNTLOGINQUEUE		50
#define MAX_CHARACTERSPERACCOUNT	5

enum EBanStatus
{
	BANSTATUS_Error				= -1,
	BANSTATUS_NotBanned			= 0,
	BANSTATUS_Banned			= 1,
	BANSTATUS_TempBanned		= 2,
	BANSTATUS_BanExpired		= 3,
};

enum EAccountLogin
{
	ACCOUNTLOGIN_Success			= 1,	//[Successful]
	ACCOUNTLOGIN_ConnectionFailed	= 0,	//Connecting failed
	ACCOUNTLOGIN_IncorrectName		= -1,	//Account does not exist in the selected world
	ACCOUNTLOGIN_IncorrectPassword	= -2,	//Incorrect Password
	ACCOUNTLOGIN_Banned				= -3,	//Account is banned
	ACCOUNTLOGIN_LoggedIn			= -4,	//Account is already logged in
	ACCOUNTLOGIN_IPBanned			= -5,	//Your IP is banned
	ACCOUNTLOGIN_YouAreBanned		= -6,	//Your IP is banned
	ACCOUNTLOGIN_TryAgain			= -7,	//[Game will automatically retry]
	ACCOUNTLOGIN_Maintenance		= -8,	//Server is in Maintenance
	ACCOUNTLOGIN_AccountNotActive	= -16,	//Account not active, see User Management Panel
	ACCOUNTLOGIN_WrongVersion		= -17,	//Version does not Match
	ACCOUNTLOGIN_TempBanned			= -18,	//Temporarily Banned
};



enum EAccountFlag
{
	ACCOUNTFLAG_NotExist			= -1,
	ACCOUNTFLAG_Invalid				= 0,
	ACCOUNTFLAG_Mule				= 1,
	ACCOUNTFLAG_Activated			= 2,
	ACCOUNTFLAG_EventGM				= 4,
	ACCOUNTFLAG_Supporter			= 8,
	ACCOUNTFLAG_GameMaster			= 16,
	ACCOUNTFLAG_AcceptedLatestTOA	= 32,
	ACCOUNTFLAG_Approved			= 64,
	ACCOUNTFLAG_MustConfirm			= 128,
	ACCOUNTFLAG_GMIPRequired		= 256,
	ACCOUNTFLAG_AccountTalisman		= 512,
};

enum EAccountShare : int
{
	ACCOUNTSHARE_None					= 0,
	ACCOUNTSHARE_DenyLoginUserPanel		= 1,
	ACCOUNTSHARE_DenyCreateDelCharacter = 2,
	ACCOUNTSHARE_DenyGrabEquipment		= 4,
	ACCOUNTSHARE_DenyWarehouse			= 8,
	ACCOUNTSHARE_DenyClanMaster			= 16,
	ACCOUNTSHARE_DenyCaravanPet			= 32,
	ACCOUNTSHARE_DenyTrade				= 64,
	ACCOUNTSHARE_DenySetupPersonalShop	= 128,
	ACCOUNTSHARE_DenyBuyPersonalShop	= 256,
	ACCOUNTSHARE_DenySpendCoins			= 512,
	ACCOUNTSHARE_DenyTradeCoins			= 1024,
	ACCOUNTSHARE_DenyOpenItemBox		= 2048,
	ACCOUNTSHARE_DenyUseTradeChat		= 4096,
	ACCOUNTSHARE_DenyUseResetStones		= 8192,
	ACCOUNTSHARE_DenyPickupAndDropItems = 16384,
	ACCOUNTSHARE_DenyBuySellItems		= 32768,
	ACCOUNTSHARE_DenyChat				= 65536,
};

enum ECharacterLog
{
	CHARACTERLOG_Create				= 1,
	CHARACTERLOG_Delete				= 2,
	CHARACTERLOG_Update				= 3,
};

struct AccountFileData
{
	char	  szHeader[16];
	char	  szAccountName[32];
	char	  szCharacterName[MAX_CHARACTERSPERACCOUNT + 1][32];
};

struct PacketAccountLoginCode : Packet
{
	DWORD			  dwReserved;
	EAccountLogin	  iCode;
	int				  iFailCode;

	char			  szMessage[256];
};

struct PacketAccountID : Packet
{
	ID				  lAccountID;
};

struct AccountLogin
{
	User				* pcUser;
	UserData			* pcUserData;
	SocketData			* pcSocketData;

	char				szAccountName[32];
	char				szPassword[32];
	char				szIP[20];
	char				szMacAddress[20];

	UINT				uSerialHD;
	char				szPCName[32];
	char				szVideoName[256];
	UINT				uVideoGUID;

	UINT				uUniqueFolderID;

	char				szHardwareID[40];

	UINT				uWidthScreen;
	UINT				uHeightScreen;
	int					iSystemOS;

	int					iGameVersion;
	UINT				uGameChecksum;
	UINT				uDLLChecksum;
};
