#pragma once


enum EBattleRoyaleStateEnemy : int
{
	BATTLEROYALESTATEENEMY_None,
	BATTLEROYALESTATEENEMY_Friendly,
	BATTLEROYALESTATEENEMY_Enemy,
	BATTLEROYALESTATEENEMY_InBattleEnemy,
};

#define MAX_BATTLEROYALE_TIMERANGE		(10 * 1000)
#define MAX_BATTLEROYALE_USERSRANGE		5

class CBattleRoyaleInstance
{
private:
	int							iID = -1;
	int							iDropID = -1;

	CBattleRoyale * pcBattleRoyale = nullptr;

	BattleRoyale::State			sState;

	CRoomInstance * pcRoomInstance = nullptr;

	std::vector<BattleRoyale::PlayerState *>	 vPlayers;

public:

	CBattleRoyaleInstance();
	~CBattleRoyaleInstance();


	int							GetID() { return iID; }
	void						SetID( int _iID ) { iID = _iID; };

	int							GetDropID() { return iDropID; }
	void						SetDropID( int _iID ) { iDropID = _iID; };

	CBattleRoyale * GetBattleRoyale() { return pcBattleRoyale; }
	void						SetBattleRoyale( CBattleRoyale * pc ) { pcBattleRoyale = pc; }

	CRoomInstance * GetRoom() { return pcRoomInstance; }
	void						SetRoom( CRoomInstance * pc ) { pcRoomInstance = pc; }

	BattleRoyale::State & GetState() { return sState; }
	std::vector<BattleRoyale::PlayerState *> & GetPlayers() { return vPlayers; }

	bool						HasUser( User * pcUser );
	bool						AddUser( User * pcUser );
	bool						RemoveUser( int iID );
	bool						RemoveUser( User * pcUser, bool bDisconnected = false );

};

class CBattleRoyaleHandler : public CServerEvent
{
private:
	std::vector<CBattleRoyale *> vBattleRoyales;
	std::vector<CBattleRoyaleInstance *> vInstances;

	struct BattleRoyaleRewardServer
	{
		int				iID;

		char			szName[32];
		char			szItemCode[32];
		char			szImagePath[260];

		int				iQuantity;

		int				iChance;
	};

	struct BattleRoyaleBattleServer
	{
		int				iID;

		bool			bOpen;
		char			szName[32];
		int				iMapID;
		IMinMax			sRequiredLevel;

		INT64			iTimeLeft;
	};

	struct BattleRoyaleDeathInstance
	{

	};

public:
	CBattleRoyaleHandler();
	virtual ~CBattleRoyaleHandler();

	void						Init();

	void						Load( int iBattleID = -1 );

	BOOL						AddUser( User * pcUser, int iBattleID );

	BOOL						IsExclusiveDrop( User * pcUser, int iExclusiveID );

	void						TeleportUser( CBattleRoyaleInstance * pcInstance, User * pcUser, bool bArena, bool bRandomPosition = false, bool bCountdownPosition = false );

	BOOL						IsPVPMap( EMapID eMapID );

	void						HandlePacket( User * pcUser, PacketBattleRoyaleGasDeath * psPacket );
	void						HandlePacket( PacketNetBattleRoyaleStatus * psPacket );
	void						HandlePacket( PacketNetBattleRoyaleEnter * psPacket );
	void						HandlePacket( User * pcUser, PacketBattleRoyaleRequest * psPacket );

	void						ProcessStart( CBattleRoyaleInstance * pcInstance, BOOL bStart );
	void						ProcessPacket( User * pcUser, PacketBattleRoyaleBattleTime * psPacket );
	void						ProcessPacket( User * pcUser, PacketBattleRoyaleList * psPacket );
	void						ProcessPacket( User * pcUser, PacketBattleRoyaleRewardList * psPacket );
	void						ProcessPacket( User * pcUser, PacketBattleRoyaleResponse * psPacket );
	void						ProcessPacket( User * pcUser, PacketBattleRoyaleTeleports * psPacket );

	float						GetHandicap( User * pcUser, BattleRoyale::EHandicapType eHandicapType, BOOL bItem = FALSE );

	BOOL						IsDropableItemUser( User * pcUser );

	BOOL						DropItemBattle( User * pcUser, Item & sItem );

	void						SQLLogBattleRoyaleUsersInRange( User * pcUser );

	EBattleRoyaleStateEnemy		GetStateEnemy( User * pcUser, User * pcTarget );

	bool						AddInstanceTimeSeconds( User * pcUser, int iSeconds );

private:

	void						Clear();

	void						Remove( int iBattleID );

	bool						SQLGetBattleRoyale( int iID, CBattleRoyale * pcBattleRoyale );
	bool						SQLGetBattleRoyaleDrop( CBattleRoyale * pcBattleRoyale );
	bool						SQLGetBatteRoyaleCoordinates( CBattleRoyale * pcBattleRoyale );
	bool						SQLGetBatteRoyaleTime( CBattleRoyale * pcBattleRoyale );
	bool						SQLGetHandicap( CBattleRoyale * pcBattleRoyale );
	bool						SQLGetListID( std::vector<int> & vListID );
	int							SQLLastInstanceID();
	void						SQLNewBattle( int iBattleID, int iInstanceID );
	void						SQLStartBattle( int iInstanceID, int iUsersCount );
	void						SQLEndBattle( int iInstanceID, int iWinnerCharacterID = -1 );
	void						SQLKilledInBattle( int iInstanceID, int iCharacterID, int iCharacterKillerID = -1 );
	void						SQLWinBattle( int iInstanceID, int iCharacterID );
	void						SQLNewCharacterInstance( int iBattleID, int iInstanceID, int iCharacterID );

	bool						SQLGetRewardList( int iBattleID, std::vector<BattleRoyaleRewardServer *> & vRewards );
	bool						SQLGetUserInstanceID( User * pcUser, int & iInstanceID, int & iBattleID );
	bool						GetBattleList( std::vector<BattleRoyaleBattleServer *> & vBattles );

	bool						SQLUserWonInstance( User * pcUser, int iBattleID, int iInstanceID );
	bool						SQLGetLastInstanceDeath( User * pcUser, int & iBattleID, int & iInstanceID, SYSTEMTIME & sStartDate, SYSTEMTIME & sDieDate );
	void						SQLSetUserReceiveReward( User * pcUser, int iBattleID, int iInstanceID, SYSTEMTIME sDieDate, bool bWinner = false );

	void						SQLUpdateCharacterRanking( User * pcUser, int iKills, BOOL bKilled, BOOL bWinner, int iPoints );
	bool						SQLGetCharacterRanking( User * pcUser, int & iKills, int & iDeaths, int & iWins, int & iPoints, SYSTEMTIME & sDate );

	bool						SQLGetUserReward( int iCharacterID, int & iBattleID, int & iInstanceID, int & iKills, BOOL & bWinner, BattleRoyale::EPlayerTeam & eTeamID, SYSTEMTIME & sDate );
	void						SQLAddUserReward( int iBattleID, int iInstanceID, int iCharacterID, int iKills, BOOL bWinner, BattleRoyale::EPlayerTeam eTeamID );
	void						SQLUpdateUserReceivedReward( int iBattleID, int iInstanceID, int iCharacterID, SYSTEMTIME sDate );

	CBattleRoyaleInstance		* GetInstanceByBattleID( int iID );
	CBattleRoyaleInstance		* GetInstance( int iID );
	CBattleRoyaleInstance		* GetInstance( CBattleRoyale * pcBattleRoyale );
	CBattleRoyaleInstance		* GetInstance( User * pcUser );
	bool						IsInBattleRoyale( User * pcUser );

	void						SendChat( CBattleRoyaleInstance * pcInstance, EChatColor eColor, std::string strMessage, bool bGameMaster = false );

	BOOL						OnGasKilled( User * pcUser );
	BOOL						OnUserKilled( User * pcKiller, User * pcVictim ) override;

	BOOL						OnDisconnected( User * pcUser );

	BOOL						CannotSave( User * pcUser ) override;

	BOOL						OnNPCClick( User * pcUser, Unit * pcUnit ) override;

	void						Update( ETickType eTick ) override;

	BOOL						OnUserMapChanged( User * pcUser, EMapID eMapID ) override;

	BOOL						OnUserGetMapItem( User * pcUser, EItemID eItemID ) override;

	void						UpdateBattleTime();

	void						UpdateEventTimer();

	void						Damage( CBattleRoyaleInstance * pcInstance, bool bMeteorite, Point3D sPosition, int iDamage = 10 );

	void						DropItemOnSpawns( CBattleRoyaleInstance * pcInstance );
	void						DropItem( CBattleRoyaleInstance * pcInstance, Point3D sPosition, int iItemCount );

	void						StartBattle( CBattleRoyaleInstance * pcInstance );
	void						EndBattle( CBattleRoyaleInstance * pcInstance, bool bTimeOut = false );

	void						Process( CBattleRoyaleInstance * pcInstance );

	void						RemoveItems( CBattleRoyaleInstance * pcInstance );

	void						RequestBattleList( User * pcUser );

	bool						GetRandomCoordinate( CBattleRoyale * pcBattleRoyale, Point3D & sPosition );

	bool						SQLIsAccountBanned( int iAccountID, SYSTEMTIME & sUnbanDate, char szReason[128] );

	void						CreateTeams( CBattleRoyaleInstance * pcInstance );

	bool						GetTeamsAlive( CBattleRoyaleInstance * pcInstance, std::vector<BattleRoyale::TeamsAliveData> & vTeams );

	bool						IsWinnerTime( CBattleRoyaleInstance * pcInstance );

	void						TeleportBeforeStart( CBattleRoyaleInstance * pcInstance, bool bCountdown );

	void						SendEXPPrizeGold( User * pcUser, EBattleRoyaleID iBattleID, int iInstanceID, int iKills, bool bWinner, int iMinutes, SYSTEMTIME sDate );

	void						SendUserCoordinateList( User * pcUser, CBattleRoyaleInstance * pcInstance );
};

