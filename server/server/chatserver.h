#pragma once

static char * pszaWordsTrade[]
{
		"nobody cares",
		"fuck",
		"fruit",
		"bpt",
		"ept",
		"kpt",
		"upt",
		"mpt",
		"rpt",
		"realm",
		"unique",
		"sandurr",
		"kenni",
		"magicpt",
		"zado",
		"lost kingdom",
		"lostkingdom",
		"dick",
		"penis",
		"giromba",
		"prog",
		"3dfx",
		"argos",
		"horus",
		"elsa",
		"im gm",
		"n4p4lm",
		"n4palm",
		"nap4lm",
		"napa1m",
		"n4p41m",
		"eat",
		"dog",
		"mama",
		"mother",
		"mommy",
		"suck",
		"8=",
		"bucet",
		"piroc",
		"bitch",
		"unban",
		"unb4n",
		"1nb4n",
		"un ban",
		"fode",
		"f0d3",
		"server",
		"http",
		"www.",
		".net",
		".com",
		"ticket",
		"t1ck3t",
		"gm pls",
		"merda",
		"m3rd4",
		"merd4",
		"m3rda",
		"desban",
		"p r o g",
		"plog",
		"vince",
		"v1nc3",
		"vinc3",
		"Naiany", //Namorada do lee
		"Naiane",
		"Naiani",
		"N4i4n3",
		"N414n3",
		"brocco",
		"br0cc0",
		"b r o c c o",
		"b r 0 c c 0",
		"vietcong",
		"vietkong",
		"mongo",
		"m0ng0",
		"m0ngo"
};


class ChatServer
{
private:
	std::vector<std::string>				vMutedNames;

public:
	ChatServer();
	virtual ~ChatServer();

    void                                    SQLLogChat( int iFromAccountID, int iFromCharacterID, int iToAccountID, int iToCharacterID, EChatColor eColor, const std::string & strMessage, const std::string & strFromIP, const std::string & strToIP );

	void									SendDebugChat( UserData * pcUserData, EChatColor eColor, const char * pszFormat, ... );
	void									SendChat( User * pcUserData, EChatColor eColor, const char * pszText );
	void									SendChatEx( User * pcUserData, EChatColor eColor, const char * pszFormat, ... );
	void									SendPersonalShopChat( UserData * pcUserData, DWORD dwObjectID, const char * pszFormat, ... );
	void									SendUserBoxChat( UserData * pcUserData, DWORD dwObjectID, const char * pszFormat, ... );
	void									SendUserBoxChatRange( int iX, int iZ, int iRange, DWORD dwObjectID, const char * pszFormat, ... );
	void									SendChatAllUsersInStage( int iStage, EChatColor eColor, const char * pszFormat, ... );
	void									SendChatAll( EChatColor eColor, const char * pszText );
	void									SendChatAllEx( EChatColor eColor, const char * pszFormat, ... );
	void									SendChatAllUsersInRange( int iX, int iZ, int iRange, EChatColor eColor, const char * pszFormat, ... );
	void									SendChatAllUsersInClan( UserData * pcUserData, EChatColor eColor, const char * pszFormat, ... );
	void									SendChatMessageBox( User * pcUser, const char * pszFormat, ... );
	void									SendChatSpyMember( UserData * pcUserData, void * pPacket );
	void									SendChatTrade( UserData * pcUserData, std::string strMessage );

	void									AddMute( const char * pszName );
	void									ClearMute();

	std::string								GetFormatWhisper( User * pcUser, const char * pszMessage, BOOL bReceiverTime );

	void									SendWhisper( User * pcUserSender, User * pcUserReceiver, const char * pszMessage );
	void									SendWhisperItemLink( User * pcUserSender, User * pcUserReceiver, PacketItemLinkChat * psPacket );

	void									HandlePacket( User * pcUser, PacketItemLinkChat * psPacket );

	std::string								GetMessageString( std::string strMessage, int iChatColor );

private:
	BOOL									CanSendMessage( User * pcUser, std::string strMessage, int iChatColor );
};

