#pragma once

#define MAX_CHATLENGTH			100
#define MAX_CHATMESSAGES		320

enum EChatColor : int
{
	CHATCOLOR_Error				= 0,
	CHATCOLOR_Whisper			= 1,
	CHATCOLOR_Blue				= 2,
	CHATCOLOR_Global			= 3,
	CHATCOLOR_Party				= 4,
	CHATCOLOR_Clan				= 5,
	CHATCOLOR_Trade				= 6,
	CHATCOLOR_Notice			= 9,
	CHATCOLOR_ClanLeader		= 10,
	CHATCOLOR_Normal			= 11,
	CHATCOLOR_Raid				= 12,
};

struct ChatBoxMessage
{
	EChatColor	  iColor;
	char		  szMessage[256];
};

struct PacketChat : Packet
{
	char			  szChat[128];
};

struct PacketChatBoxMessage : Packet
{
	EChatColor		  iChatColor;	//Chat Type, Party, Clan, Trade, etc.
	ID				  lID;			//User ID, if 0 only shows in Chat Box, otherwise also above User
	char			  szChatBoxMessage[256+8];
};

class Chat
{
public:
	static unsigned int	ChatCommand( char * pszCommand, char * pszChat );
	static void			ChatParamQ( char * pszCommand, int iParamNum, char * pszOut );
	static void			ChatParam( char * pszCommand, int iParamNum, char * pszOut );
	static void			TrimSpace( char * pszIn, char * pszOut );
	static int			TrimSpaceBegin( char * pszIn, char * pszOut );
};