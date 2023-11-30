#pragma once

struct SQLCharacter
{
	int					iID;
	char				szAccountName[32];
	char				szCharacterName[32];
	int					iLevel;
	INT64				iEXP;
	int					iGold;
	int					iClass;
	int					iClanID;
	int					iClanPermission;
	SYSTEMTIME			sLastSeenDate;
	int					iLastMapID;
};


class CharacterServer
{
private:
	int											iSiegeWarDay = 0;
	int											iSiegeWarHour = 0;

public:
	CharacterServer();
	virtual ~CharacterServer();

	void										SetSiegeWarDay( int i ) { iSiegeWarDay = i; }
	void										SetSiegeWarHour( int i ) { iSiegeWarHour = i; }

	int											GetSiegeWarDay() { return iSiegeWarDay; }
	int											GetSiegeWarHour() { return iSiegeWarHour; }

	static BOOL									SQLGetCharacter( int iCharacterID, SQLCharacter * ps );
	static BOOL									SQLGetCharacter( const char * pszCharacterName, SQLCharacter * ps );
	static int									SQLGetCharacterClanID( char * pszCharacterName );
	static void									SQLRenameCharacter( char * pszCharacterName, char * pszNewCharacterName );
	static void									SQLChangeCharacterLevel( char * pszCharacterName, int iLevel, INT64 iEXP );
	static void									SQLChangeCharacterClass( char * pszCharacterName, int iClass );

	char *										GetCharacterName( UserData * pcUserData );
	bool										GetCharacterExFilePath( const char * pszCharacterName, char * pszBuffer, int iBufferLen );
	bool										GetCharacterFilePath( const char * pszCharacterName, char * pszBuffer, int iBufferLen );
	bool										GetCharacterBroyaleFilePath( const char * pszCharacterName, char * pszBuffer, int iBufferLen );
	DWORD										GetClanCode( int iClassClan );
	void										SendBlessCastleSkill( UserData * pcUserData );

	int											ChangeCharacterName( char * pszCharacterName, char * pszNewCharacterName );
	int											ChangeCharacterLevel( char * pszCharacterName, int iLevel );
	int											ChangeCharacterClass( char * pszCharacterName, int iClass );
	bool										KickCharacterName( char * pszCharacterName );
	BOOL										SetCharacterEXPOnDisconnect( char * pszCharacterName, INT64 iEXP );

	BOOL										CopyCharacterBattleRoyale( const char * pszCharacterName );

	static void									SQLUpdateCharacter( const char * pszCharacterName, int iCharacterClass, int iCharacterLevel, INT64 iCharacterExperience, int iCharacterGold );

	static void									SQLUpdateDateLevelUP( User * pcUser );

    int											SQLGetCharacterClass( const char * pszCharacterName );
    int											SQLGetCharacterClass( int iCharacterID );

	BOOL										UpdateForceOrb( UserData * pcUserData );
	BOOL										UpdatePremiumTimer( UserData * pcUserData );
	void										CheckGold( UserData * pcUserData );

	INT64										GetExp( UserData * pcUserData );
	void										SetExpFromSQL( UserData * pcUserData );

	INT64										GetExpFromLevel( int iLevel );

	static int									GetLevelFromEXP( INT64 iEXP );

	void										OnCharacterUpdate( UserData * pcUserData );

	BOOL										CharacterInCityStage( UserData * pcUserData );
	BOOL										CharacterInBattleStage( UserData * pcUserData );

	void										OnAddExp( UserData * pcUserData );
	void										OnLoseExp( UserData * pcUserData, PacketLoseExperience * psPacket );
	void										SetExp( UserData * pcUserData, INT64 iExp );
	void										GiveEXP( User * pcUser, INT64 iExp );
	BOOL										GiveGOLD( User * pcUser, int iGold, EWhereAction eWhere = WHEREID_GiveMoney );

	void										CharacterCreate( UserData * pcUserData, PacketCreateCharacter * psPacket );

	void										SendCharacterDataEx( User * pcUser );

	void										OnCharacterSyncData( UserData * pcUserData );
	void										OnCharacterSyncDataEx( UserData * pcUserData, PacketPlayDataEx * psPacket );

	void										OnCharacterDataExSave( User * pcUser );
	void										OnCharacterUpdateLevelClan( User * pcUser, PacketUpdateClanLevel * psPacket );

	void										OnCharacterData( UserData * pcUserData );
	void										OnOutCharacterData( UserData * pcUserData );

	void										SetPositionWarp( UserData * pcUserData, int iX, int iZ, int iStageID = -1 );

	BOOL										IsCharacterLeader( User * pcUser );

	char										* GetServerName( User * pcUser );

	void										PHCharacterDataEx( User * pcUser );

	int											SQLGetMonsterID( char * pszMonsterName );
	int											SQLGetNPCID( char * pszNPCName, int iMapID = -1 );

	int											SQLGetCharacterID( char * pszName );

	void										HandlePacket( User * pcUser, PacketCharacterDataEx * psPacket );
	void										HandlePacket( User * pcUser, PacketCharacterDataSyncEx * psPacket );
	void										HandlePacket( User * pcUser, PacketSaveData * psPacket );

	void										HandlePacket( User * pcUser, PacketSelectCharacter * psPacket );

	BOOL										Validate( CharacterData * psCharacterData );

	DWORD										GetHashString( const char * pszName );
};

