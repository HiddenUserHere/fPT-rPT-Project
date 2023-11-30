#pragma once
#include "structaccount.h"


class AccountServer
{
public:

	static bool								bActive;
	static int								iWaitingAccountLogins;
	static CMutex *							pcMutex;
	static AccountLogin						saAccountLogin[MAX_ACCOUNTLOGINQUEUE];


	AccountServer();
	virtual ~AccountServer();

	BOOL									IsOnlineAccount( UserData * pcUserData, const char * pszUserID );

	BOOL									IsAdminMacAddress( const char * pszMacAddress, UINT uSerialHD );
	BOOL									IsAdminDeveloper( const char * pszMacAddress, UINT uSerialHD );
	BOOL									IsIPAccept( const char * pszIP );
	BOOL									IsAccountAccept( const char * pszAccountName );

	BOOL									IsMacBanned( UserData * pcUserData, const char * pszMacAddress );
	BOOL									IsSerialHDBanned( UserData * pcUserData, const char * pszSerialHD );
	BOOL									IsComputerNameBanned( UserData * pcUserData, const char * pszPCName );
	BOOL									IsVideoGUIDBanned( UserData * pcUserData, UINT uVideoGUID );
	BOOL									IsHardwareIDBanned( UserData * pcUserData, const char * pszHardwareID );
	BOOL									IsFolderIDBanned( UserData * pcUserData, UINT uFolderID );
	bool									GetVersionChecksum( int iGameVersion, UINT & uGameChecksum, UINT & uDLLChecksum );

	static void								SQLCharacterLog( const char * pszAccountName, int iCharacterClass, const char * pszCharacterName, const char * pszCode, const char * pszIP, const char * pszServerName, int iCharacterLevel, INT64 iCharacterExperience, int iCharacterGold, int iAction );
	static void								SQLUserOnline( const char * pszAccountName, const char * pszCharacterName, const char * pszIP, int iTicket, int iAction, int iCharacterClass, int iCharacterLevel );

	static void								SQLCharacterSave( UserData * pcUserData );

	int										OnLoginSuccess( UserData * pcUserData );

	static bool								AddAccountLogin( User * pcUser, AccountLogin & al );

	//SQL
	static bool								SQLAccountHasFlag( char * pszAccountName, EAccountFlag iFlag );
	static EAccountFlag						SQLAccountGetFlag( char * pszAccountName );

	static void								PHAccountLogin( User * pcUser, PacketLoginUser * psPacket );

	static bool								SQLUnbanAccount( char * pszAccountName );
	static int								SQLGetBanStatus( char * pszAccountName, SYSTEMTIME * psBanTime = NULL );

	static EAccountLogin					SQLAccountLogin( char * pszAccountName, char * pszPassword, char * pszIP, EAccountFlag iAccountFlag, EAccountShare & eAccountShare, SYSTEMTIME & sUnbanDate );

	static void								ProcessAccountLogin( AccountLogin & al );
	static DWORD WINAPI						ProcessAccountLoginQueue();

	std::vector<std::string>				SQLGetAccountNotification( const char * pszAccountName );

	void									DeleteCharacter( UserData * pcUserData, PacketDeleteCharacter * psPacket );

	void									OnReadPostBox( UserData * pcUserData );
	void									OnDeletePostBox( UINT ItemID );
	void									OnAccountSave( UserData * pcUserData );

	BOOL									OnDisconnectUser( UserData * pcUserData );
private:
	const int								iCountCharacterPlay = 6;
	void									LogAccountLogin( UserData * pcUserData, AccountLogin & al, ELogAccount eLogAccount );
};

