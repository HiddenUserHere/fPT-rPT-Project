#pragma once



enum ELogItem
{
	 ITEMLOGID_Get = 0,
	 ITEMLOGID_Put = 6,
	 ITEMLOGID_Mixing = 9,
	 ITEMLOGID_MixLost = 3,
	 ITEMLOGID_Aging = 4,
	 ITEMLOGID_AgingLost = 10,
	 ITEMLOGID_AgingUp = 12,
	 ITEMLOGID_BuyShop = 5,
	 ITEMLOGID_SellShop = 7,
	 ITEMLOGID_Trade = 8,
	 ITEMLOGID_Used = 11,
	 ITEMLOGID_Express = 13,
	 ITEMLOGID_GetWing = 14,
	 ITEMLOGID_WingLost = 15,
	 ITEMLOGID_BuyStar = 16,

	 ITEMLOGID_Aging2Up = 17,
	 ITEMLOGID_Aging2Down = 18,
	 ITEMLOGID_Aging1Down = 19,
	 ITEMLOGID_AgingKeep = 20,

	 ITEMLOGID_Present = 21,
	 ITEMLOGID_PersonalShop = 22,

	 ITEMLOGID_UseAlchmy = 24,
	 ITEMLOGID_LostAlchmy = 25,
	 ITEMLOGID_GetAlchmy = 26,

	 ITEMLOGID_GetRecon = 27,
	 ITEMLOGID_LostRecon = 28,
	 ITEMLOGID_SmeltingLost = 29,
	 ITEMLOGID_SmeltingGet = 30,
	 ITEMLOGID_ManufactureLost = 31,
	 ITEMLOGID_ManufactureGet = 32,
	 ITEMLOGID_ManufactureFail = 33,
	 ITEMLOGID_MixtureResetLost = 34,
	 ITEMLOGID_MixtureResetFail = 35,
	 ITEMLOGID_MixtureResetGet = 36,
	 ITEMLOGID_PremiumTimerOver = 1001,
	 ITEMLOGID_TradeItems = 1002,
	 ITEMLOGID_TradeItemsRemove = 1003,
	 ITEMLOGID_RewardEasterGet = 1004,
	 ITEMLOGID_RewardXmasRudolphGet = 1005,
	 ITEMLOGID_TradeItemSuccess = 1006,
	 ITEMLOGID_TradeItemSuccessReceive = 1007,
	 ITEMLOGID_GetPerfectize = 1008,
	 ITEMLOGID_PutPerfectize = 1009,
	 ITEMLOGID_GetSwap = 1010,
	 ITEMLOGID_PutSwap = 1011,
	 ITEMLOGID_RewardCarnivalPuzzleGet = 1012,
	 ITEMLOGID_RewardCarnivalPuzzleReceive = 1013,
	 ITEMLOGID_PerfectizedItem = 1050,
	 ITEMLOGID_SwappedItem = 1051,
	 ITEMLOGID_RollDiceJoin = 1060,
	 ITEMLOGID_RollDiceWon = 1061,
	 ITEMLOGID_RewardXmasRudolphSet = 1062,
	 ITEMLOGID_ItemBoxPut = 1063,
	 ITEMLOGID_ItemBoxReceived = 1064,
	 ITEMLOGID_ActionFieldChestReceived = 1065,
	 ITEMLOGID_ActionFieldRewardReceived = 1066,
	 ITEMLOGID_ActionFieldRewardEXP = 1067,
	 ITEMLOGID_ActionFieldRewardGold = 1068,
	 ITEMLOGID_PersonalShopBought = 1070,
	 ITEMLOGID_PersonalShopSell = 1071,
	 ITEMLOGID_CaravanAdd = 1080,
	 ITEMLOGID_CaravanRemove = 1081,
	 ITEMLOGID_XmasTreeAdd = 1090,
	 ITEMLOGID_XmasTreeRemove = 1091,
	 ITEMLOGID_XmasTreeSpawn = 1092,
     ITEMLOGID_BattleRoyaleReceivedItem = 1093,
     ITEMLOGID_GoldDrop = 1094,
	 ITEMLOGID_XmasGiftDaily = 1095,
	 ITEMLOGID_GlamorousAttireGlaze = 1096,
	 ITEMLOGID_GlamorousArmamentVarnish = 1097,

};


struct	_LogITEM 
{
	DWORD	dwCode;
	DWORD	dwHead;
	DWORD	dwCheckSum;
};

struct LogItem 
{
	int 		size;			// 사용한 구조체의 크기
	char 		UserID[32];  		// 사용자 ID
	char 		CharName[24]; 		// 사용자 캐랙터 이름
	int 		IP; 			// 사용자 IP            <===== 추가
	int 		Flag;			// 아이템 정보
	int		ItemCount;		// 처리할 아이템 갯수
	_LogITEM	Item[32];		// 아이템 정보
};

struct LogItemTrade
{
	int 		size;			// 사용한 구조체의 크기
	char 		UserID[32];  		// 사용자 ID
	char 		CharName[24]; 		// 사용자 캐랙터 이름
	int		IP;			// 사용자 IP         <========= 추가 
	int 		Flag;			// 아이템 정보

	char 		TUserID[32];		// 거래 아이디
	char 		TCharName[24];		// 거래 캐랙터 이름
	int 		TIP;			// 거래 IP           <========= 추가 
	int		Money;			// 거래한 금액
	int		ItemCount;		// 거래한 아이템 갯수
	_LogITEM 	Item[32];		// 아이템 정보
};

struct LogItemGift
{
	int 		size;		// 사용한 구조체의 크기
	char 		UserID[32];  	// 사용자 ID
	char 		CharName[24]; 	// 사용자 캐랙터 이름
	int 		IP; 		// 사용자 IP
	int 		Flag;		// 아이템 정보    (아이템교부13)
	int		GNo;		// 인증번호
	char 		TEL[16]; 	// 사용자 전화번호
	_LogITEM 	Item;		// 아이템 정보
};


class LogServer
{
private:

	WORD										wLastHour = -1;

	int											iaUsersOnlineServers[3];

	void										SetLogString( char * pszLog, ELogItem eType );
	void										SetLogCheatString( char * pszLog, ELogCheat eType );

	void										OnLogAging( UserData * pcUserData, int iLogID, const char * pszLog );
	void										OnLogMixtrue( UserData * pcUserData, int iLogID, const char * pszLog );
	void										OnLogItemGeneral( UserData * pcUserData, int iLogID, const char * pszLog );

	void										OnLogItemData( LogItem * psLog );
	void										OnLogItemTradeData( LogItemTrade * psLog );
	void										OnLogItemExpressData( LogItemGift * psLog );

	static void									OnLog( const char * pszLogName, const char * pszLog );
	static void									OnLog( User * pcUser, const char * pszLogName, const char * pszLog );
	static void									OnLogEx( const char * pszLogName, const char * pszLog, ... );

public:

	enum ELogServerType
	{
		LOGTYPEID_Unknown,
		LOGTYPEID_Aging,
		LOGTYPEID_Mixtrue,
		LOGTYPEID_HackUser,
		LOGTYPEID_Item,
		LOGTYPEID_ClanMoney,
		LOGTYPEID_BlessCastle,
		LOGTYPEID_LogItem = 1,
		LOGTYPEID_LogItemTrade = 2,
		LOGTYPEID_LogItemExpress = 3,
	};

	LogServer();
	virtual ~LogServer();

	BOOL										OnLogCheat( UserData * pcUserData, void * pCheatLog );

	void										OnLogWrite( UserData * pcUserData, int iLogID, const char * pszLog, ELogServerType eType );
	void										OnLogItem( int eType, int iSize, void * psLogItem );
	void										OnLogLoadItemCharacter( UserData * pcUserData );
	void										OnLogUnloadItemCharacter( UserData * pcUserData );
	static void									OnLogItemEx( User * pcUser, int iLogID, const char * pszLog, ... );
	static void									OnLogItemEx( const char * pszAccountName, const char * pszIP, int iLogID, const char * pszLog, ... );

	void										LogSocketClose( SocketData * pcSocketData, char * pszLocation, void * p = NULL );

	static void									OnLogEx( User * pcUser, int iLogID, const char * pszLog, ... );
	static void									OnLogEx( const char * pszAccountName, const char * pszIP, int iLogID, const char * pszLog, ... );

	void										OnLogUsersOnline();

	void										HandlePacket( PacketNetUsersOnline * psPacket );

	void										AddFirewallBan( const char * pszIP );


};

