#pragma once

#define _MAX_BOTSERVER							80

#define _RICARTEN_XMIN							-306087
#define _RICARTEN_XMAX							227290
#define _RICARTEN_ZMIN							-4717634
#define _RICARTEN_ZMAX							-4227246

struct PacketGetUnitInfo : Packet
{
	DWORD									dwObjectID;
	int										iX;
	int										iY;
	int										iZ;
	int										ax,ay,az;
	int										iState;

	struct CharacterData						sCharInfo;
};


struct SBotServer
{
	BOOL										bValid;
	DWORD										dwObjectID;
	DWORD										dwTime;
	int											iPositionItemHandL;
	int											iPositionItemHandR;
	int											iPositionItemHandB;
	int											iX;
	int											iY;
	int											iZ;
	char										szTextSay[128];
	BOOL										bSay;
	CharacterData									sCharInfo;
};

struct PacketBotLogin : Packet
{
	BOOL										bAdd;
	char										szCharName[32];
	DWORD										dwObjectID;
	int											iIndexServer;
};

class CBotServerHandler
{
protected:
	DWORD										dwTimeUpdate = 0;
private:
	SBotServer									sBotData[_MAX_BOTSERVER];

	SBotServer									* GetFreeSlot();

	BOOL										Inside( int iX, int iZ );

public:
	CBotServerHandler();
	virtual ~CBotServerHandler();

	void										AddGameBot( struct PacketBotLogin * psPacket );

	void										OnGetUnitInfo( User * pcUser, struct PacketSimple * psPacket );

	BOOL										CreateBot( char * pszCharName, int iX, int iY, int iZ );

	BOOL										DeleteBot( char * pszCharName );

	BOOL										IsOnlineBot( char * pszCharName );

	BOOL										SetItemHand( int iHand, char * pszCharName, char * pszItemName );
	BOOL										SetBotSay( char * pszCharName, char * pszText );

	void										Tick();

};

