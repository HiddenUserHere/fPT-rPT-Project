#pragma once


class SocketData;

struct PartyInfo;
struct PacketItemData;
struct QuestStatusData;

class CaravanBase;
class XmasTreeBase;

enum EAccountShare;

struct MountUserListData;

#define PVPDAMAGE_MAX_QUEUE				256
#define PVPDAMAGE_TIME_DAMAGE			750
#define PVPDAMAGE_TIME_DAMAGE_MAX		1500


#define USER_TALKRANGE			1200
#define USER_TALKRANGE_FAR		307200

#define USER_VIEWRANGE			2400
#define USER_VIEWRANGE_FAR		614400

#define USER_UPDATERANGE		3600
#define USER_UPDATERANGE_FAR	921600

#define USER_SIGHTRANGE			4800
#define USER_SIGHTRANGE_FAR		1228800

#define USER_STATUS_UPDATE_GRACE		2500
#define USER_USER_STATUS_UPDATE_GRACE	10000

#define USER_STATUS_UPDATE_MAX			4


enum EGameLevel
{
	GAMELEVEL_None = 0,
	GAMELEVEL_One = 1,
	GAMELEVEL_Two = 2,
	GAMELEVEL_Three = 3,
	GAMELEVEL_Four = 4,
	GAMELEVEL_Five = 5,
};

struct BlessCastleItemBuff
{
	DWORD			dwItemEndTime;
	int				iItemID;
	int				iValueData;
};

#define MAX_PVP_DAMAGE		400

struct DamagePvPDataHandler
{
	BOOL						bActive;

	UserData					* pcTarget;
	int							iTargetID;

	int							iDamage;
	int							iSkillID;
	int							iParam1;
	int							iParam2;

	DWORD						dwTimeUpdate;
};

class UserData
{
public:

	UserData();

	char						szOwner[32];				//0x0
	char						szAccountName[32];			//0x20
	char						szCharacterName[32];		//0x40

	DWORD						dwHashAccountName;			//0x60
	DWORD						dwHashCharacterName;		//0x64

	int							AdminOperMode;				//0x68
	int							bUse_InsertCommand;			//0x6C

	BOOL						bInGame;					//0x70
	DWORD						dwDataError;				//0x74
	int							bKeepAlive;					//0x78
	EMapID						iMapID;						//0x7C
	Point3D						sPosition;					//0x80
	Point3D						sAngle;						//0x8C
	char						szEmpty03b[0x0C];
	struct CharacterData		sCharacterData;				//0xA4
	int							iCharacterList;				//0x274

	DWORD						dwNoticeCounter;			//0x278

	union
	{
		struct
		{
			DWORD						dwaTimeUnitStatus[32];
			DamagePvPDataHandler		saPvPDamageData[0x27C];
		};

		BYTE						baUnitStatus[5][0x1000];	//0x27C
	};
	DWORD						dwaTimeUnitStatusOld[5];	//0x527C
	int							iMovingIndex;				//0x5290

	DropItemData				sDropItems[64];				//0x5294
	DropItemData				sIntentoryItems[316];		//0x5594
	DropItemData				sWarehouseOldItems[120];	//0x6464	dont use it more...
	BOOL						bWarehouseOpen;				//0x6A04

	int							iaItemTradePotionID[16];	//0x6A08	???
	int							iTradePotion;				//0x6A48	???
	SellItemShopData			sItemShopList[32];			//0x6A4C
	int							iItemShopList;				//0x6C4C
	DWORD						dwItemShopTime;				//0x6C50

	int							iSaveItems;					//0x6C54   when item dropping this gets updated
	int							iSaveGold;					//0x6C58  -1 after save game again..
	int							iInventoryGold;				//0x6C5C
	char						szEmpty04a[0x0C];
	UINT						uUnitStatusNum;				//0x6C6C
	int							iID;						//0x6C70
	EChatColor					iChatColor;					//0x6C74
	char						szChat[256];				//0x6C78
	char						szPersonalShopMessage[128];	//0x6D78
	int							iPersonalShop;				//0x6DF8
	SocketData *				pcSocketData;				//0x6DFC
	DWORD						dwTimeLastPacket;			//0x6E00
	DWORD						dwTimeLastStatusUpdate;		//0x6E04
	char						szEmpty06[0x08];
	void *						psUnknown1;					//0x6E10

															//Equipment
	EItemID						eShieldEquipped;			//0x6E14
	EItemID						eWeaponEquipped;			//0x6E18

	EGameLevel					iGameLevel;					//0x6E1C
	int							bCloak;						//0x6E20
	int							bShowDebugInfo;				//0x6E24

	DWORD						dwTimeOnline;				//0x6E28
	int							iLastMaskCountUser;			//0x6E2C
	int							iLastMaskCountMonster;		//0x6E30

															//6eac = free
															//6e98 = free
															//6e9c = free
															//6ea0 = free
															//6ea4 = free
															//6ea8 = free

	char						padddd[0x8];

	int							iUnitStatusCounter;			//0x6E3C

	char  paddddd[0x08];

	int							iUnitHitCounter;			//0x6E48

	class UnitData *			pcLastAttack;				//0x6E4C
	int							lLastAttackID;				//0x6E50

	char						szEmpty07[0x14];

	int							iPacketDamageCount;
	DWORD						dwPacketDamageTime;

	UINT						uUnitKillCounter;			//0x6E70

	char						szEmpty07a[0x3C];

	UINT						uaUpdateCounter1[1024];		//0x6EB0
	UINT						uaUpdateCounter2[1024];		//0x6EB0 + 0x1000
	USHORT						uaUpdateCounter3[1024];		//0x6EB0 + 0x1000 + 0x1000
	USHORT						uaUpdateCounter4[1024];		//0x6EB0 + 0x1000 + 0x1000 + 0x800
	UCHAR						uaUpdateCounter5[4096];		//0x6EB0 + 0x1000 + 0x1000 + 0x800 + 0x800

	int							uUnk;

	BOOL						bBlock;						//0xAEB4
	DWORD						dwUnknownso;				//0xAEB8
	int							iPacketCounter;				//0xAEBC
	DWORD						dwTickPacketCounter;		//0xAEC0
	char						szEmpty07aA[0x04];
	CharacterFile				* psCharacterFile;			//0xAEC8
	DWORD						dwSaveCharacterData;		//0xAECC
	DWORD						dwExclusiveNum;				//0xAED0
	int							iBellatraScore;				//0xAED4
	int							iBellatraKills;				//0xAED8
	char						szEmpty07b[0x04];
	short						sBellatraRoomID;			//0xAEE0
	short						sZero;
	char						cUnknown0800;				//0xAEE4
	BYTE						bServerIndexID;				//0xAEE5
	char						cUnknown0801[2];			//0xAEE6
	UnitData					* pcHolyIncantatedUnitData;	//0xAEE8

															//Settings
	bool						bDisabledWhisper;			//0xAEEC
	bool						bDisabledTrade;				//0xAEED
	bool						bDisabledParty;				//0xAEEE
	bool						bThirdEye;					//0xAEEF

															//EXP and GOLD
	DWORD						bAFK;						//0xAEF0
	int							iCompulsionMagneticRange;	//0xAEF4
	LARGE_INTEGER				liEXP;						//0xAEF8
	int							iGoldStart;					//0xAF00
	int							iLevelStart;				//0xAF04
	Point3D						sOldPosition;				//0xAF08
	int							iBlockSaveEXP;				//0xAF14
	int							iGoldIn;
	int							iGoldOut;
															//Unit Status
	UINT						uNewUnitStatus;				//0xAF20
	UINT						uLastUnitStatus;			//0xAF24

	DWORD						dwDisconnectTime;			//0xAF28 if non-zero then no save
	int							iVersion;					//0xAF2C not set
	DWORD						dwCharacterUpdateTime;		//0xAF30

	char						szEmpty08b[0x003C];			//

	BOOL						bBlockAction;				//0xAF70 Blocks Further Action of User

	char						szEmpty08c[0x0034];			//

															//Party

	BOOL						bUnused11;					//Depracated 0xAFA8
	DWORD						dwUnused12;					//Depracated
	void 					  * pUnused13;					//Depracated
	ID							iUnused14;					//Depracated
	void					  * pUnused15[6];				//Depracated
	ID							iaUnused16[6];				//Depracated
	int							iUnused17;					//Depracated
	int							iUnused18;					//Depracated

	ULONG						iClanID;					//0xAFF0
	int							iTicket;					//0xAFF4
	int							iEXPPotion;					//0xAFF8

															//B474 = TIME Might Stone
															//B478 = iItemSlotFlag Might Stone
															//B47C = sDamage Boost Might Stone

	int							iaServerPotion[3][4];

	char						szEmpty09[0x2A4];

	MinMax						sDamageLimit;				//0xB2D0
	MinMax						sCriticalLimit;				//0xB2D4

	char						szEmpty09a[0x30];

	DWORD						dwAutomationEndTime;		//0xB308
	int							iAutomationDamageBoost;		//0xB30C
	DWORD						dwMaximizeEndTime;			//0xB310
	int							iMaximizeMaxDamageBoost;	//0xB314
	DWORD						dwPhysicalAbsorbEndTime;	//0xB318
	int							iPhysicalAbsorbBoost;		//0xB31C
	DWORD						dwExtremeShieldEndTime;		//0xB320
	int							iExtremeShield;				//0xB324
	DWORD						dwHolyBodyEndTime;			//0xB328
	int							iHolyBodyAbsorb;			//0xB32C
	DWORD						dwHolyValorEndTime;			//0xB330
	int							iHolyValorBoost;			//0xB334
	DWORD						dwHolyReflectionEndTime;	//0xB338
	int							iHolyReflectionDamage;		//0xB33C
	DWORD						dwEnchantWeaponEndTime;		//0xB340
	int							iEnchantWeaponAttr;			//0xB344
	DWORD						dwMetalArmorEndTime;		//0xB348
	int							iMetalArmorParam;			//0xB34C
	DWORD						dwSparkShieldEndTime;		//0xB350
	int							iSparkShieldDamage;			//0xB354
	DWORD						dwFalconEndTime;			//0xB358
	short						saFalconDamage[2];			//0xB35C
	DWORD						dwDivineShieldEndTime;		//0xB360
	short						sDivineShieldAbsorb;		//0xB364
	short						sDivineShieldAttackRating;	//0xB366
	DWORD						dwTriumphOfValhallaEndTime; //0xB368
	int 						iTriumphOfValhallaDamage;	//0xB36C
	DWORD						dwVirtualLifeEndTime;		//0xB370
	int							iVirtualLifeData;			//0xB374
	DWORD						dwEnergyShieldEndTime;		//0xB378
	int							iEnergyShieldDamage;		//0xB37C
	DWORD						dwSpiritElementalEndTime;	//0xB380
	union
	{
		int						iSpiritElementalBoost;		//0xB384
		MinMax					sSpiritElementalBoost;
	};
	DWORD						dwDancingSwordEndTime;		//0xB388
	int							iDancingSwordLevel;			//0xB38C
	DWORD						dwDancingSwordDelayTime;	//0xB390 ????
	DWORD						dwVanishEndTime;			//0xB394
	short						saVanishParam[2];			//0xB398
	DWORD						dwHookedTargetEndTime;		//0xB39C
	DWORD						iCompulsionParam;			//0xB3A0
	DWORD						dwMagneticSphereEndTime;	//0xB3A4
	int							iMagneticSphereParam;		//0xB3A8
	DWORD						dwBerserkerEndTime;			//0xB3AC
	int 						iBerserkerParam;			//0xB3B0
	DWORD						dwAssassinEyeEndTime;		//0xB3B4
	DWORD						AssassinEyeParam;			//0xB3B8
	DWORD						dwVagueEndTime;				//0xB3BC
	short						sVagueEvade;				//0xB3C0
	short						sVagueLevel;				//0xB3C2
	DWORD						dwForceOfNatureEndTime;		//0xB3C4
	unsigned short				saForceOfNatureParam[2];	//0xB3C8
	DWORD						dwGodlyShiedEndTime;		//0xB3CC
	int							iGodlyShiedParam;			//0xB3D0
	DWORD						GodsBlessingTime; // 0xB3D4
	DWORD						GodsBlessingParam; // 0xB3D8
	DWORD						HallOfValhallaTime; // 0xB3DC
	unsigned short				HallOfValhallaParam[0x2]; // 0xB3E0
	DWORD						FrostJavelinTime; // 0xB3E4
	DWORD						FrostJavelinParam; // 0xB3E8
	DWORD						RegenerationFieldTime; // 0xB3EC
	DWORD						SummonMuspellTime; // 0xB3F0
	DWORD						SummonMuspellParam; // 0xB3F4
	DWORD						dwCurseQuestTimeOut; // 0xB3F8
	DWORD						dwCurseDamage; // 0xB3FC
	DWORD						dwCurse_DefenceTime; // 0xB400
	DWORD						dwCurse_DefenceParam; // 0xB404
	unsigned char				baSkillPoint[0x10]; // 0xB408
	DWORD						ForceOrbCode; // 0xB418
	int							ForceOrbTime; // 0xB41C
	DWORD						ForceOrbDamage; // 0xB420
	DWORD						dwForceOrb_SaveCode; // 0xB424
	DWORD						dwForceOrb_SaveTime; // 0xB428
	DWORD						dwForceOrb_SaveDamage; // 0xB42C
	DWORD						dwForceOrbCityTime; // 0xB430
	class UnitData				* pcPetData; // 0xB434
	DWORD						dwObjectPetID; // 0xB438
	short						sNumDeath; // 0xB43C
	short						sPVPKills; // 0xB43E
	DWORD						dwUnknown5; // 0xB440
	DWORD						dwUnknown6; // 0xB444
	DWORD						dwUnknown7; // 0xB448
	DWORD						dwUnknown8; // 0xB44C
	DWORD						dwBlessSkill_Code; // 0xB450
	DWORD						dwBlessSkillTime; // 0xB454
	DWORD						dwBlessSkillParam; // 0xB458
	BlessCastleItemBuff			saBCBuff[3]; // 0xB45C
	DWORD						dwVerifiedLoginTimeOut; // 0xB480
	int							* iUnkUnused001; // 0xB484
	DWORD						dwHolyIncantationID; // 0xB488
	CWarehouseBase				* pcWarehouse; // 0xB48C
	int 						iWarehouseDataSize; // 0xB490
	int  						iWarehouseTotalSize; // 0xB494
	int							iWarehouseCountPage; // 0xB498
	BOOL						bDisconnectFunction; // 0xB49C
	int							dwunk; // 0xB4A0
	DWORD						dwT4RoomTime; // 0xB4A4
	DWORD						dwObjectIDT4;	//0xB4A8
	class UnitData				* pcUnitDataT4; //0xB4AC
	EItemID						eSoulCrystalItemID; //0xB4B0
	DWORD						dwSoulCrystalTimeOut; //0xB4B4
	UINT						uBattleRoyaleUsersNearCount;	//0xB4B8
	DWORD						dwBattleRoyaleUsersNearTime;    //0xB4BC
	DWORD						dwPersonalShopBuyTimeOut;	//0xB4C0
	DWORD						dwUnknown9[18]; // 0xB4C4


	int							GetLevel() { return this->sCharacterData.iLevel; };
	int							GetGold() { return this->iInventoryGold; };

	BOOL						IsDead();

};


class User
{
public:
	BOOL									bInUse;
	class UserData						  * pcUserData;
	UINT									uIndex;
	BOOL									bNetServer;
	BOOL									bClosingSocket;

	BOOL									bDisconnectNet;
	DWORD									dwIP;

	int										iAccountID;
	int										iCharacterID;

	UINT									uSerialHD;

	char									szHardwareID[40];

	SYSTEMTIME								sDateLevelUP;
	
	EMapID									 eMapID;		//Map that the Player is currently in
	EMapID									 eSecondMapID;	//Second Map that the Player has loaded

	BOOL									b16;
	BOOL									b32;
	BOOL									b64;
	struct PacketPlayDataEx					pData;
	BOOL									bReceivedExp;
	int										iEXPPotion;

	int										iPvPKills;
	int										iPvPDeaths;
	int										iPvPStreak;
    int                                     iPvPExperience;
    int                                     iPvPClanIndex;
    bool                                    bPvPRankChanged;
    bool                                    bPvPRankLoaded;
    bool                                    bPvPNewRank;

	bool									bPvPMode;

	//Action Field
	int										iActionFieldKills;
	int										iActionFieldScore;

	struct PacketItemPremiumData			sPremiumData;
	DWORD									dwTimeSync;
	DWORD									dwTimeSavePremium;
	struct PacketItemData					sTempItem;

	DWORD									dwLastForceTime;
	DWORD									dwLastPremiumTime;
	BOOL									bCheckedGameMaster;
	class UserData						  * pcUserDataSpyoner;
	int										iWarehouseError;
	int										iWarehouseGold;
	DropItemData							sWarehouseItems[420];
	DropItemData							saTradeItems[20];

	ItemBoxSlot								sItemBox;
	BOOL									bCharacterExLoaded;
	CharacterDataEx							sCharacterDataEx;

	BOOL									bQuestLoaded;

	PacketQuestUpdateData					saPacketQuestData[MAX_QUESTPACKETDATA];

	DWORD									dwChatTradeTime;
	BOOL									bDebugChat;
	PacketSettingsData						sSettingsData;

	BOOL									bUnbrokeAge;

	BOOL									bMuted;

	//T5
	BOOL									bInT5QuestArena;
	int										iRoundT5QuestArena;
	DWORD									dwTimeT5QuestArenaUpdate;
	DWORD									dwTimeT5QuestArenaSpawnUpdate;

	UINT									uQuestT5ID;
	USHORT									sQuestT5ProgressValue[2];

	BOOL									bInGrandFury;
	DWORD									dwGrandFuryTickEnter;

	DWORD									dwBlindTime;
	DWORD									dwBlindDamage;

	DWORD									dwAlasEvasion;
	DWORD									dwAlasTime;

	DWORD									dwAdventMigalDamage;
	DWORD									dwAdventMigalTime;

	
	struct MinMax							sChasingHuntDamageBoost;
	DWORD									dwChasingHuntTime;

	int										iHolyMindAbsorb;
	DWORD									dwHolyMindTime;

	MinMax									sSummonMuspellDamageBoost;

	int										iWispAbsorb;
	DWORD									dwWispTime;

	UINT									uLethalSightLevel;
	UINT									uLethalSightRange;
	UINT									uLethalSightCritical;
	UINT									uLethalSightDamageBoost;
	DWORD									dwLethalSightTimeOut;

	UINT									uFierceWindLevel;
	DWORD									dwFierceWindTimeOut;

	UINT									uHolyConvictionLevel;
	DWORD									dwHolyConvictionTimeOut;

	UINT									uWizardTranceLevel;
	DWORD									dwWizardTranceTimeOut;

	bool									bWhirlWind;

	bool									bMagneticDischargeSelf;
	UINT									uMagneticDischargeLevel;
	DWORD									dwMagneticDischargeTimeOut;

	UINT									uKissViperLevel;
	DWORD									dwKissViperTimeOut;
	UINT									uKissViperCriticalAdd;

	bool									bDieUpdate;

	int										iTradeCoin;
	int										iTradeGold;

	int										iBlessCastleScore;

	//Skill Data
	struct PacketSkillStatus				sSkillStatus;
	struct PacketSkillStatusContainer		sSkillContainer;

	UINT									uBufferSkillPosition;

	//Unit Status
	UINT									 uPriorityCounter;
	UINT									 uPriority;

	//Status Send Buffers
	UINT									uBufferPosition;
	PacketUnitStatusContainer				sUnitStatusContainer;
	UINT									uBufferPositionM;
	PacketUnitStatusContainer				sUnitStatusContainerM;

	BYTE									baUnitStatusNew[USER_STATUS_UPDATE_MAX + 1][0x2000];

	//Party
	BOOL									bParty;
	PartyInfo							  * psParty;

	//Caravan
	CaravanBase							  * pcCaravanBase;
	Unit								  * pcCaravan;
	std::vector <DropItemData>				vCaravanItems;

	std::vector<PacketAchievementUser>		vAchievement;

	BYTE									baSoDItemCount[8];

	//Xmas Tree
	XmasTreeBase						  * pcXmasTreeBase;

	//Battle Royale
	bool									bInBattleRoyale;

	EAccountShare							eAccountShare;

	//Honors
	int										iPvPHonorType;
	UINT									uPvPHonorTimeOut;
	int										iBellatraHonorType;
	UINT									uBellatraHonorTimeOut;

	UINT									uInGameSecondsTime;

	std::vector<MountUserListData>			vMountList;

public:
											User();
										   ~User();

	BOOL									IsInGame() { return pcUserData->iID != 0 ? TRUE : FALSE; };
	BOOL									IsConnected() { return pcUserData->pcSocketData != 0 ? TRUE : FALSE; };

	BOOL									IsAccountShareDenied( EAccountShare _eAccountShare ) { return (eAccountShare & _eAccountShare) == _eAccountShare; }
	void									SetAccountShare( EAccountShare _eAccountShare ) { eAccountShare = _eAccountShare; }

	int										GetID() { return pcUserData->iID; }
	EMapID									GetMapID() { return pcUserData->iMapID; }
	EMapID									GetSecondMapID() { return eSecondMapID; }

    int                                     GetClanID() { return pcUserData->iClanID; }

	EGameLevel								GetGameLevel() { return pcUserData->iGameLevel; }
	void									SetGameLevel( EGameLevel eLevel ) { pcUserData->iGameLevel = eLevel; }

	void									Start( SocketData * sd ) { pcUserData->pcSocketData = sd; };

	void									Stop() { pcUserData->pcSocketData = NULL; };

	void									UpdateQuestWithValues( DWORD dwQuestID, EQuestStatus eStatus, DWORD dwValue1 = 0, DWORD dwValue2 = 0, DWORD dwValue3 = 0, DWORD dwValue4 = 0, DWORD dwValue5 = 0 );
	void									UpdateQuest( DWORD dwQuestID, EQuestStatus eStatus );

	BOOL									IsFinishedQuest( DWORD dwQuestID );

	BOOL									RestartQuest( DWORD dwQuestID );

	BOOL									CanSee( User * pcOtherUser );
	BOOL									CanSee( Unit * pcOtherUnit );

	void									Swap( User * pcUser );

	void									Init();

	void									UnInit() { bInUse = FALSE; };

};

struct PacketBellatraResult : Packet
{
	int					iUnk;
	DWORD				dwTime;
	BOOL				bShow;

	short				saTeamRound[2];

	DWORD				dwUnk[15];

	int					iResultCount;

	struct BellatraResult
	{
		char			szName[32];
		UserData		* pcUserData;
		int				iClass;
		int				iClanID;
		int				iScore;
		int				iMonster;
		DWORD			dwUnk[4];
	};

	BellatraResult		saResult[32];
};
