#pragma once

enum
{
	JOBCODEID_NonSpec,
	JOBCODEID_Fighter,
	JOBCODEID_Mechanician,
	JOBCODEID_Archer,
	JOBCODEID_Pikeman,
	JOBCODEID_Atalanta,
	JOBCODEID_Knight,
	JOBCODEID_Magician,
	JOBCODEID_Priestess,
	JOBCODEID_Assassin,
	JOBCODEID_Shaman,
};


class SUserData
{
public:

	char						szOwner[ 32 ];				//0x0
	char						szAccountName[ 32 ];		//0x20
	char						szCharacterName[ 32 ];		//0x40

	DWORD						dwHashAccountName;			//0x60
	DWORD						dwHashCharacterName;		//0x64
	
	int							AdminOperMode;				//0x68
	int							bUse_InsertCommand;			//0x6C

	BOOL						bInGame;					//0x70
	DWORD						dwDataError;				//0x74
	int							bKeepAlive;					//0x78
	int							iMapID;						//0x7C
	Point3D						sPosition;					//0x80
	Point3D						sAngle;						//0x8C
	char						szEmpty03b[0x0C];
	struct CharacterData		sCharacterData;				//0xA4
	int							iCharacterList;				//0x274

	char						szEmpty03a[0x04];			//0x278

	BYTE						baUnitStatus[5][0x1000];	//0x27C
	DWORD						dwaTimeUnitStatus[5];		//0x527C
	int							iMovingIndex;				//0x5290

	DropItemData				sDropItems[64];				//0x5298
	DropItemData				sIntentoryItems[316];		//0x5598
	DropItemData				sWarehouseOldItems[120];	//0x6464	dont use it more...
	BOOL						bWarehouseOpen;				//0x6A04

	DWORD						dwTradePotion[16];			//0x6A08	???
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
	char						szChat[128];				//0x6C78
	char						szEmpty05[0x0104];
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

	char						szEmpty07[0x1C];

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
	CharacterFile				* psCharacterFile2;			//0xAECC
	DWORD						dwExclusiveNum;				//0xAED0
	int							iBellatraScore;				//0xAED4
	int							iBellatraKills;				//0xAED8
	char						szEmpty07b[0x04];
	short						sBellatraRoomID;			//0xAEE0
	short						sZero;
	UINT						iPing;						//0xAEE4 [override]
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
	DWORD						dwPadding5[2];				//0xAF00
	Point3D						sOldPosition;				//0xAF08
	int							iBlockSaveEXP;				//0xAF14
	char						szEmpty08[0x08];			//af20 holds TICKCOUNT last activity, af28 holds TICKCOUNT at which to dc

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
															//B478 = Class Might Stone
															//B47C = Damage Boost Might Stone

	char						szEmpty09[0x030C];

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
	int							iSpiritElementalBoost;		//0xB384
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
	int							iVagueParam;				//0xB3C0
	DWORD						dwForceOfNatureEndTime;		//0xB3C4
	unsigned short				saForceOfNatureParam[2];	//0xB3C8
	DWORD						dwGodlyShiedEndTime;		//0xB3CC
	int							iGodlyShiedParam;			//0xB3D0
	DWORD						GodsBlessingTime; // 0xB3D4
	DWORD						GodsBlessingParam; // 0xB3D8
	DWORD						HallOfValhallaTime; // 0xB3DC
	unsigned short				HallOfValhallaParam[ 0x2 ]; // 0xB3E0
	DWORD						FrostJavelinTime; // 0xB3E4
	DWORD						FrostJavelinParam; // 0xB3E8
	DWORD						RegenerationFieldTime; // 0xB3EC
	DWORD						SummonMuspellTime; // 0xB3F0
	DWORD						SummonMuspellParam; // 0xB3F4
	DWORD						dwCurse_AttackTime; // 0xB3F8
	DWORD						dwCurse_AttackParam; // 0xB3FC
	DWORD						dwCurse_DefenceTime; // 0xB400
	DWORD						dwCurse_DefenceParam; // 0xB404
	unsigned char				bSkillPoint[ 0x10 ]; // 0xB408
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
	DWORD						dwSiegeItem_ScrollTime1; // 0xB45C
	DWORD						dwSiegeItem_Scroll_Code1; // 0xB460
	DWORD						dwSiegeItem_ScrollParam1; // 0xB464
	DWORD						dwSiegeItem_ScrollTime2; // 0xB468
	DWORD						dwSiegeItem_Scroll_Code2; // 0xB46C
	DWORD						dwSiegeItem_ScrollParam2; // 0xB470
	DWORD						dwSiegeItem_ScrollTime3; // 0xB474
	DWORD						dwSiegeItem_Scroll_Code3; // 0xB478
	DWORD						dwSiegeItem_ScrollParam3; // 0xB47C
	int							iVerifiedLogin; // 0xB480
	class UserServer			* pcUser; // 0xB484
	DWORD						dwHolyIncantationID; // 0xB488
	void						* pWarehouseData; // 0xB48C
	int 						iWarehouseDataSize; // 0xB490
	int  						iWarehouseTotalSize; // 0xB494
	int							iWarehouseCountPage; // 0xB498
	BOOL						bDisconnectFunction; // 0xB49C
	int							dwunk; // 0xB4A0
	DWORD						dwT4RoomTime; // 0xB4A4
	DWORD						dwObjectIDT4;
	class UnitData				* pcUnitDataT4;
	DWORD						dwUnknown9[ 23 ]; // 0xB4B0

	int							GetLevel() { return this->sCharacterData.iLevel; };
	int							GetGold() { return this->iInventoryGold; };

	BOOL						IsDead();
};

