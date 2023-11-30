#pragma once

struct PrizeEventRudolphUser
{
	int						iID;
	EItemID					iItemGiftID;
	char					szAccountName[32];
	char					szCharacterName[32];
	char					szAccountIP[32];

	PrizeEventRudolphUser( int iID, EItemID iItemGiftBoxID, char szA[32], char szName[32], char szIP[32] )
	{
		this->iID = iID;
		iItemGiftID = iItemGiftBoxID;
		StringCchCopyA( szAccountName, 32, szA );
		StringCchCopyA( szCharacterName, 32, szName );
		StringCchCopyA( szAccountIP, 32, szIP );
	};

	~PrizeEventRudolphUser() {};
};

struct PrizeXmasAccount
{
	char			szAccountName[32];

	char			szAccountIP[32];

	char			szCharacterName[32];
};

class CEventServer
{

	enum 
	{
		SERVEREVENTID_EXP,
		SERVEREVENTID_SiegeWar,
		SERVEREVENTID_Bellatra,
		SERVEREVENTID_Morif,
		SERVEREVENTID_Wolf,
		SERVEREVENTID_XMAS,
		SERVEREVENTID_Halloween,
		SERVEREVENTID_Drop,
		SERVEREVENTID_Valentine,
		SERVEREVENTID_OnlineReward,
	};

protected:
	std::vector<SPrizeData *>						vPrizesReward;
	int												iTotalPercent;

	std::vector<SPrizeData *>						vPrizesXmasReward;
	int												iXmasTotalPercent;

	std::vector<SPrizeData*>						vPrizesRewardEaster;
	int												iTotalPercentEaster;

	std::vector<SPrizeData *>						vPrizesDailyReward;
	int												iTotalPercentDaily;

	std::vector<SPrizeData *>						vPrizesCarnivalPuzzleReward;
	int												iTotalPercentCarnivalPuzzle;

    PacketEventServer                               sPacketEventData;

private:


	void											SQLOnlineRewardLog( char * pszAccountName, char * pszName, char * pszItemName, int iQuantity, bool bDailyReward );

	bool											SQLGetAccountGifts( std::vector<PrizeXmasAccount *> & vAccounts );
	void											SQLAddAccountGift( char * pszAccountName, char * pszAccountIP, char * pszCharacterName );
	void											SQLClearAccountGifts();

	void											SetPrizeOnlineReward();
	void											SetPrizeDailyReward();

	void											LoadPrizesOnlineReward();
	void											LoadPrizesXmasReward();
	void											LoadPrizesEasterReward();
	void											LoadPrizesCarnivalPuzzleReward();
	void											LoadEventsData();

    void                                            ProcessPacket( User * pcUser, PacketEventServer * psPacket );

	DWORD											wLastHourOnlineReward = 0;
	WORD											wNextHourDailyReward = 0;
	bool											bDeliveredDailyReward = false;
	WORD											wLastHourXmas = 0;

	BOOL											bResetRudolph = FALSE;

	std::vector<PrizeEventRudolphUser*>				vXmasEventRudolph;

public:
	CEventServer();
	virtual ~CEventServer();

	void											GiveFriendGifts();
	void											GiveRudolphPrizes();

	void											HandlePacket( UserData * pcUserData, PacketNPCEasterEgg * psPacket );
	void											HandlePacket( UserData * pcUserData, PacketNPCRudolphXmas * psPacket );
	void											HandlePacket( User * pcUser, PacketNPCCarnival * psPacket );

	void											EasterEventUnitKilled( UnitData * pcUnitData );

	void											OnUnitKilled( UnitData * pcUnitData );

	void											Tick();

	void											SetDailyReward( int iHour );

	void											ClearTickOnlineReward() { wLastHourOnlineReward = 27; }
	void											ClearTickOnlineRewardXmas() { wLastHourXmas = 0; bResetRudolph = TRUE; }

	void											OnSpawnMonsterEvent( struct CharacterData * psCharacterData, Map * pcMap );

	void											OnSayTime( UnitData * pcUnitData );

    void                                            SendEvents( User * pcUser );
    void                                            SendEventsAll();

    void                                            UpdateEventMessages();
    void                                            SendUpdateEventMessages();

	void											AddXmasPrize( User * pcUser );

	void											SendNPCCarnival( User * pcUser );

	SPrizeData										* GetPrizeRewardItem();
	SPrizeData										* GetPrizeXmasRewardItem();
	SPrizeData										* GetPrizeRewardItemDaily();
	SPrizeData										* GetPrizeRewardItemEaster();
	SPrizeData										* GetPrizeRewardCarnivalPuzzle();

};

