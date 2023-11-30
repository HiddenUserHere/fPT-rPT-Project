#pragma once

#define MAX_COMMANDSPERTIME 10

enum ESQLServerCommand
{
	SQLSERVERCOMMAND_ChangeCharacterName	= 10,
	SQLSERVERCOMMAND_ChangeCharacterLevel	= 11,
	SQLSERVERCOMMAND_ChangeCharacterAccount = 12,
	SQLSERVERCOMMAND_ChangeCharacterClass	= 13,

	SQLSERVERCOMMAND_ChangeAccountName		= 20,

	SQLSERVERCOMMAND_KickAccountName		= 30,
	SQLSERVERCOMMAND_KickCharacterName		= 31,

	SQLSERVERCOMMAND_LoadCoinShop			= 101,
	SQLSERVERCOMMAND_LoadCheatList			= 111,
	SQLSERVERCOMMAND_LoadMixFormula			= 121,
};

struct SQLServerCommand
{
	ESQLServerCommand	iType;
	char				szOperator[32];
	char				szParameter1[64];
	char				szParameter2[64];
	char				szParameter3[64];
	SYSTEMTIME			sDateSubmitted;
	SYSTEMTIME			sDateProcessed;
};


class ServerCommand
{
private:

	

	void									OnActivateGameMaster( UserData * pcUserData );

	BOOL									OnGameMasterAdminCommand( UserData * pcUserData, const char * pszBuff );
	BOOL									OnGameMasterEventCommand( UserData * pcUserData, const char * pszBuff );

	BOOL									OnPlayerCommand( UserData * pcUserData, const char * pszBuff );
	BOOL									OnGameMasterCommand( UserData * pcUserData, const char * pszBuff );
public:
	ServerCommand();
	virtual ~ServerCommand();

	static void								SQLRead();
	static BOOL								SQLReadFromCharacter( const char * pszCharacterName, ESQLServerCommand iCommand, SQLServerCommand & s );

	bool									Process( SQLServerCommand * ps );

	void									SetVersion( int iVersion );
	void									SetMoriphEvent( BOOL b );
	void									SetWolfEvent( BOOL b );
	void									SetHalloweenEvent( BOOL b );
	void									SetAgingEvent( BOOL bAgingFree, int iReduceTax );
	void									SetActionFieldEvent( BOOL b );

	BOOL									OnReceiveChat( UserData * pcUserData, PacketChat * psPacket );

	void									CheckUserOnline( UserData * pcUserData, struct PacketUserOnline * psPacket );

	void									AddCommand( int iCommandID, char * pszParameter1, char * pszParameter2, char * pszParameter3 );

};

