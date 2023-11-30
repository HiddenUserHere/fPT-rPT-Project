#pragma once
class CheatServer
{
protected:
	struct PacketChecksumFunctionList			* psPacketChecksumList	= NULL;
	struct PacketWindowList						* psPacketWindowList	= NULL;
	int											iTotalCheatWindowList	= 0;
	BOOL										bCreatedList			= FALSE;
public:
	CheatServer();
	virtual ~CheatServer();

	void										OnLoginUserPacket( UserData * pcUserData );

	void										UnbanAccount( const char * pszUserID );

	void										CheckStatePoint( UserData * pcUserData, DWORD dwLevelQuestLog );
	void										CheckSpeedUser( UserData * pcUserData, int iX, int iZ );

	BOOL										OnCheckMacAddress( const char * pszUserID, const char * pszMacAddr );

	void										HandlePacket( User * pcUser, PacketCrash * psPacket );
	void										HandlePacket( User * pcUser, PacketCrashData * psPacket );
};

