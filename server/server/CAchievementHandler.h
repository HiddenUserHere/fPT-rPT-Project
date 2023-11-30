#pragma once

class CAchievementHandler
{
public:
	CAchievementHandler();
	~CAchievementHandler();


	void					Init();

	void					Shutdown();

	BOOL					SQLGetAchievements();

	BOOL					SQLGetAchievementEvent( CAchievement * pcAchievement );
	BOOL					HaveAchievement( User * pcUser, EAchievementType iType );
	BOOL					SQLHaveAchievementValue( EAchievementType iType, ValueTaskAchievement & sValue1, ValueTaskAchievement & sValue2, ValueTaskAchievement & sValue3 );
	BOOL					CompleteAchievement( User * pcUser, EAchievementType iType, ValueTaskAchievement & sValue1, ValueTaskAchievement & sValue2, ValueTaskAchievement & sValue3 );

	std::vector<PacketAchievementUser>  GetUserAchievements( int iAccountID, int iCharacterID );
	BOOL								GetUserAchievements( User * pcUser, User * pcTarget );
	std::vector<PacketAchievementUser>  GetCharacterAchievements( const char * pszCharacterName );

	BOOL					OnLoadUser( User * pcUser );
	BOOL					OnUnloadUser( User * pcUser );

	BOOL					OnTask( User * pcUser, EAchievementTask iTask, ValueTaskAchievement & sValue1, ValueTaskAchievement & sValue2, ValueTaskAchievement & sValue3 );

	void					ProcessPacket( User * pcUser, PacketAchievement * psPacket );
	void					ProcessPacket( User * pcUser, PacketAchievementUser * psPacket );

	void					HandlePacket( User * pcUser, PacketGetUnitData * psPacket );

private:
	std::vector<CAchievement*>	vAchievements;

	BOOL					SQLHaveAchievement( int iAccountID, int iCharacterID, EAchievementType iType );
	BOOL					SQLNewAchievement( User * pcUser, EAchievementType iType, ValueTaskAchievement & sValue1, ValueTaskAchievement & sValue2, ValueTaskAchievement & sValue3 );
	BOOL					SQLUpdateAchievement( User * pcUser, EAchievementType iType, BOOL bIncreaseLevel, ValueTaskAchievement & sValue1, ValueTaskAchievement & sValue2, ValueTaskAchievement & sValue3 );

	BOOL					UpdateAchievement( User * pcUser, EAchievementType iType, BOOL bIncreaseLevel, ValueTaskAchievement & sValue1, ValueTaskAchievement & sValue2, ValueTaskAchievement & sValue3 );

	int						GetAchievementID( EAchievementType iType );
	BOOL					IsLevelAchievement( EAchievementType iType );
	BOOL					IsCharacterAchievement( EAchievementType iType );
};

