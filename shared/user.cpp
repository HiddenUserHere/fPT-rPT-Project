#include "stdafx.h"
#include "Coin.h"
#include "user.h"

User::User()
{
	pcCaravan = nullptr;

	pcXmasTreeBase = nullptr;

	uInGameSecondsTime = 0;
}

User::~User()
{
	if ( pcCaravanBase )
		DELET( pcCaravanBase->psCaravanData );
	DELET( pcCaravanBase );

	if ( pcXmasTreeBase )
		DELET( pcXmasTreeBase->psXmasTreeData );
	DELET( pcXmasTreeBase );

	vAchievement.clear();

	vCaravanItems.clear();

	vMountList.clear();
}

void User::UpdateQuestWithValues( DWORD dwQuestID, EQuestStatus eStatus, DWORD dwValue1, DWORD dwValue2, DWORD dwValue3, DWORD dwValue4, DWORD dwValue5 )
{
}

void User::UpdateQuest( DWORD dwQuestID, EQuestStatus eStatus )
{

}

BOOL User::IsFinishedQuest( DWORD dwQuestID )
{
	return FALSE;
}

BOOL User::RestartQuest( DWORD dwQuestID )
{
	BOOL bRet = FALSE;
	return bRet;
}

BOOL User::CanSee( User * pcOtherUser )
{
	//Other User cloaked? Then cannot see other user
	if ( pcOtherUser->pcUserData->bCloak )
		return FALSE;

	//Not in one of the two Maps that the User can see?
	EMapID eOtherMapID = pcOtherUser->GetMapID();
	if ( (eOtherMapID != eMapID) &&
		(eOtherMapID != eSecondMapID) )
		return FALSE;

	//Other User in Quest Arena?
	if ( (pcOtherUser->GetMapID() == MAPID_T5QuestArena) || (pcOtherUser->GetMapID() == MAPID_QuestArena) )
	{
		//Not a GM? Then cannot see other User
		if ( GetGameLevel() == GAMELEVEL_None )
			return FALSE;
	}

	return TRUE;
}

BOOL User::CanSee( Unit * pcOtherUnit )
{
	//In Quest Arena?
	if ( (pcUserData->iMapID == MAPID_T5QuestArena) || (pcUserData->iMapID == MAPID_QuestArena) )
	{
		//Monster is not for this User?
		if ( pcOtherUnit->pcUnitData->lExclusiveTargetID != pcUserData->iID )
		{
			//Not a GM?
			if ( GetGameLevel() == GAMELEVEL_None )
			{
				//Not a Pet? Then probably someone else's Monster
				if ( pcOtherUnit->pcUnitData->sCharacterData.iMonsterType != MONSTERTYPE_Friendly )
					return FALSE;

				//Not your Pet? Then probably someone else's Pet
				if ( pcOtherUnit->pcUnitData->pcOwner != pcUserData )
					return FALSE;
			}
		}
	}

	//Not in one of the two Maps that the User can see?
	EMapID eOtherMapID = pcOtherUnit->GetMapID();
	if ( (eOtherMapID != eMapID) &&
		(eOtherMapID != eSecondMapID) )
		return FALSE;

	//Is NPC?
	if ( pcOtherUnit->pcUnitData->sCharacterData.iType == CHARACTERTYPE_NPC )
	{
		//Is Hidden NPC?
		if ( (pcOtherUnit->pcUnitData->sCharacterData.iLevel & 2) )
		{
			//User not a GM? then NPC stays hidden
			if ( GetGameLevel() == GAMELEVEL_None )
				return FALSE;
		}
	}

	//Caravan?
	if ( pcOtherUnit->bFollowUnitType )
	{
		if ( pcOtherUnit->pcUnitData->pcOwner && (pcOtherUnit->pcUnitData->pcOwner != pcUserData) )
		{
			if ( pcOtherUnit->pcUnitData->pcOwner->bCloak )
				return FALSE;
		}
	}

	return TRUE;
}

void User::Swap( User * pcUser )
{
	bInUse							= pcUser->bInUse;

	CharacterFile * psCharacterFileOld	= pcUserData->psCharacterFile;
	SocketData * pcSocketDataOld		= pcUserData->pcSocketData;
	CopyMemory( pcUserData, pcUser->pcUserData, sizeof( UserData ) );
	pcUserData->psCharacterFile			= psCharacterFileOld;
	pcUserData->pcSocketData			= pcSocketDataOld;

	if ( pcUser->pcUserData->psCharacterFile && pcUserData->psCharacterFile )
		CopyMemory( pcUserData->psCharacterFile, pcUser->pcUserData->psCharacterFile, 0x6000 );


	bNetServer						= pcUser->bNetServer;
	bClosingSocket					= pcUser->bClosingSocket;
	
	bDisconnectNet					= pcUser->bClosingSocket;
	dwIP							= pcUser->dwIP;
	
	iAccountID						= pcUser->iAccountID;
	iCharacterID					= pcUser->iCharacterID;
	
	uSerialHD						= pcUser->uSerialHD;

	STRINGCOPY( szHardwareID, pcUser->szHardwareID );

	CopyMemory( &sDateLevelUP, &pcUser->sDateLevelUP, sizeof( SYSTEMTIME ) );

	eMapID							= pcUser->eMapID;
	eSecondMapID					= pcUser->eSecondMapID;

	b16								= pcUser->b16;
	b32								= pcUser->b32;
	b64								= pcUser->b64;

	CopyMemory( &pData, &pcUser->pData, sizeof( PacketPlayDataEx ) );

	bReceivedExp					= pcUser->bReceivedExp;

	iEXPPotion						= pcUser->iEXPPotion;

	iPvPKills						= pcUser->iPvPKills;
	iPvPDeaths						= pcUser->iPvPDeaths;
	iPvPStreak  					= pcUser->iPvPStreak;
    iPvPExperience                  = pcUser->iPvPExperience;
    iPvPClanIndex                   = pcUser->iPvPClanIndex;
    bPvPRankChanged                 = pcUser->bPvPRankChanged;
    bPvPRankLoaded                  = pcUser->bPvPRankLoaded;
    bPvPNewRank                     = pcUser->bPvPNewRank;

	bPvPMode						= pcUser->bPvPMode;

	iActionFieldKills				= pcUser->iActionFieldKills;
	iActionFieldScore				= pcUser->iActionFieldScore;


	CopyMemory( &sPremiumData, &pcUser->sPremiumData, sizeof( PacketItemPremiumData ) );

	dwTimeSync						= pcUser->dwTimeSync;
	dwTimeSavePremium				= pcUser->dwTimeSavePremium;

	CopyMemory( &sTempItem, &pcUser->sTempItem, sizeof( PacketItemData ) );

	dwLastForceTime					= pcUser->dwLastForceTime;
	dwLastPremiumTime				= pcUser->dwLastPremiumTime;

	pcUserDataSpyoner				= NULL;
	
	iWarehouseError					= pcUser->iWarehouseError;
	iWarehouseGold					= pcUser->iWarehouseGold;
	CopyMemory( &sWarehouseItems, &pcUser->sWarehouseItems, sizeof( DropItemData ) * _countof( sWarehouseItems ) );
	
	CopyMemory( &saTradeItems, &pcUser->saTradeItems, sizeof( DropItemData ) * _countof( saTradeItems ) );

	CopyMemory( &sItemBox, &pcUser->sItemBox,  sizeof( ItemBoxSlot ) );

	bCharacterExLoaded				= pcUser->bCharacterExLoaded;
	CopyMemory( &sCharacterDataEx, &pcUser->sCharacterDataEx, sizeof( CharacterDataEx ) );

	bQuestLoaded					= pcUser->bQuestLoaded;
	CopyMemory( &saPacketQuestData, &pcUser->saPacketQuestData, sizeof( PacketQuestUpdateData ) * _countof( saPacketQuestData ) );

	dwChatTradeTime					= pcUser->dwChatTradeTime;

	bDebugChat						= pcUser->bDebugChat;

	CopyMemory( &sSettingsData, &pcUser->sSettingsData, sizeof( PacketSettingsData ) );

	bUnbrokeAge						= pcUser->bUnbrokeAge;

	bMuted							= pcUser->bMuted;

	bInT5QuestArena					= pcUser->bInT5QuestArena;
	iRoundT5QuestArena				= pcUser->iRoundT5QuestArena;
	dwTimeT5QuestArenaUpdate		= pcUser->dwTimeT5QuestArenaUpdate;
	dwTimeT5QuestArenaSpawnUpdate	= pcUser->dwTimeT5QuestArenaSpawnUpdate;

	uQuestT5ID						= pcUser->uQuestT5ID;
	sQuestT5ProgressValue[0]		= pcUser->sQuestT5ProgressValue[0];
	sQuestT5ProgressValue[1]		= pcUser->sQuestT5ProgressValue[1];

	bInGrandFury					= pcUser->bInGrandFury;
	dwGrandFuryTickEnter			= pcUser->dwGrandFuryTickEnter;

	dwBlindDamage					= pcUser->dwBlindDamage;
	dwBlindTime						= pcUser->dwBlindTime;

	dwAlasEvasion					= pcUser->dwAlasEvasion;
	dwAlasTime						= pcUser->dwAlasTime;

	dwAdventMigalDamage				= pcUser->dwAdventMigalDamage;
	dwAdventMigalTime				= pcUser->dwAdventMigalTime;

	sChasingHuntDamageBoost.sMin	= pcUser->sChasingHuntDamageBoost.sMin;
	sChasingHuntDamageBoost.sMax	= pcUser->sChasingHuntDamageBoost.sMax;
	dwChasingHuntTime				= pcUser->dwChasingHuntTime;

	iHolyMindAbsorb					= pcUser->iHolyMindAbsorb;
	dwHolyMindTime					= pcUser->dwHolyMindTime;

	sSummonMuspellDamageBoost.sMin	= pcUser->sSummonMuspellDamageBoost.sMin;
	sSummonMuspellDamageBoost.sMax	= pcUser->sSummonMuspellDamageBoost.sMax;

	iWispAbsorb						= pcUser->iWispAbsorb;
	dwWispTime						= pcUser->dwWispTime;

	uLethalSightLevel				= pcUser->uLethalSightLevel;
	uLethalSightRange				= pcUser->uLethalSightRange;
	uLethalSightCritical			= pcUser->uLethalSightCritical;
	uLethalSightDamageBoost			= pcUser->uLethalSightDamageBoost;
	dwLethalSightTimeOut			= pcUser->dwLethalSightTimeOut;

	uFierceWindLevel				= pcUser->uFierceWindLevel;
	dwFierceWindTimeOut				= pcUser->dwFierceWindTimeOut;

	uHolyConvictionLevel			= pcUser->uHolyConvictionLevel;
	dwHolyConvictionTimeOut			= pcUser->dwHolyConvictionTimeOut;

	uWizardTranceLevel				= pcUser->uWizardTranceLevel;
	dwWizardTranceTimeOut			= pcUser->dwWizardTranceTimeOut;

	bWhirlWind						= pcUser->bWhirlWind;

	bMagneticDischargeSelf			= pcUser->bMagneticDischargeSelf;
	uMagneticDischargeLevel			= pcUser->uMagneticDischargeLevel;
	dwMagneticDischargeTimeOut		= pcUser->dwMagneticDischargeTimeOut;

	uKissViperLevel					= pcUser->uKissViperLevel;
	dwKissViperTimeOut				= pcUser->dwKissViperTimeOut;
	uKissViperCriticalAdd			= pcUser->uKissViperCriticalAdd;

	iTradeCoin						= pcUser->iTradeCoin;
	iTradeGold						= pcUser->iTradeGold;

	bDieUpdate						= pcUser->bDieUpdate;

	iBlessCastleScore				= pcUser->iBlessCastleScore;

	CopyMemory( &sSkillStatus, &pcUser->sSkillStatus, sizeof( PacketSkillStatus ) );
	CopyMemory( &sSkillContainer, &pcUser->sSkillContainer, sizeof( PacketSkillStatusContainer ) );

	uBufferSkillPosition			= pcUser->uBufferSkillPosition;

	uPriorityCounter				= pcUser->uPriorityCounter;
	uPriority						= pcUser->uPriority;

	uBufferPosition					= pcUser->uBufferPosition;
	CopyMemory( &sUnitStatusContainer, &pcUser->sUnitStatusContainer, sizeof( PacketUnitStatusContainer ) );
	uBufferPositionM				= pcUser->uBufferPositionM;
	CopyMemory( &sUnitStatusContainerM, &pcUser->sUnitStatusContainerM, sizeof( PacketUnitStatusContainer ) );

	for ( int i = 0; i < (USER_STATUS_UPDATE_MAX+1); i++ )
		CopyMemory( &baUnitStatusNew[i], &pcUser->baUnitStatusNew[i], 0x2000 );

	bParty							= FALSE;
	psParty							= NULL;

	pcCaravanBase					= pcUser->pcCaravanBase;
	pcCaravan						= pcUser->pcCaravan;
	vCaravanItems.clear();
	if ( pcUser->vCaravanItems.size() > 0 )
	{
		for ( UINT i = 0; i < pcUser->vCaravanItems.size(); i++ )
			vCaravanItems.push_back( pcUser->vCaravanItems[i] );
	}

	vAchievement.clear();
	if ( pcUser->vAchievement.size() > 0 )
	{
		for ( UINT i = 0; i < pcUser->vAchievement.size(); i++ )
			vAchievement.push_back( pcUser->vAchievement[i] );
	}

	pcXmasTreeBase					= pcUser->pcXmasTreeBase;

	bInBattleRoyale					= pcUser->bInBattleRoyale;

	eAccountShare					= pcUser->eAccountShare;

	iPvPHonorType					= pcUser->iPvPHonorType;
	uPvPHonorTimeOut				= pcUser->uPvPHonorTimeOut;
	iBellatraHonorType				= pcUser->iBellatraHonorType;
	uBellatraHonorTimeOut			= pcUser->uBellatraHonorTimeOut;

	uInGameSecondsTime				= pcUser->uInGameSecondsTime;

	vMountList.clear();
	for ( size_t i = 0; i < pcUser->vMountList.size(); i++ )
		vMountList.push_back( pcUser->vMountList[i] );
}

void User::Init()
{
	bInUse							= TRUE;

	CALLT( 0x00567530, uIndex );
	if ( pcUserData->psCharacterFile )
		pcUserData->psCharacterFile->iLength = 0;

	pcUserData->uBattleRoyaleUsersNearCount = 0;
	pcUserData->dwBattleRoyaleUsersNearTime = 0;
	pcUserData->dwPersonalShopBuyTimeOut	= 0;

	bNetServer						= FALSE;
	bClosingSocket					= FALSE;
	
	bDisconnectNet					= FALSE;
	dwIP							= 0;
	
	iAccountID						= -1;
	iCharacterID					= -1;
	
	uSerialHD						= 0;

	ZeroMemory( szHardwareID, _countof( szHardwareID ) );

	ZeroMemory( &sDateLevelUP, sizeof( SYSTEMTIME ) );

	eMapID							= MAPID_Invalid;
	eSecondMapID					= MAPID_Invalid;

	b16								= FALSE;
	b32								= FALSE;
	b64								= FALSE;

	ZeroMemory( &pData, sizeof( PacketPlayDataEx ) );

	bReceivedExp					= FALSE;

	iEXPPotion						= 0;

    iPvPKills                       = 0;
    iPvPDeaths                      = 0;
    iPvPStreak                      = 0;
    iPvPExperience                  = 0;
    iPvPClanIndex                   = -1;
    bPvPRankChanged                 = false;
    bPvPRankLoaded                  = false;
    bPvPNewRank                     = false;

	bPvPMode						= false;

	iActionFieldKills				= 0;
	iActionFieldScore				= 0;

	ZeroMemory( &sPremiumData, sizeof( PacketItemPremiumData ) );

	dwTimeSync						= 0;
	dwTimeSavePremium				= 0;

	ZeroMemory( &sTempItem, sizeof( PacketItemData ) );

	dwLastForceTime					= 0;
	dwLastPremiumTime				= 0;

	pcUserDataSpyoner				= NULL;
	
	iWarehouseError					= 0;
	iWarehouseGold					= 0;
	ZeroMemory( &sWarehouseItems, sizeof( DropItemData ) * _countof( sWarehouseItems ) );
	
	ZeroMemory( &saTradeItems, sizeof( DropItemData ) * _countof( saTradeItems ) );

	ZeroMemory( &sItemBox, sizeof( ItemBoxSlot ) );

	bCharacterExLoaded				= FALSE;
	ZeroMemory( &sCharacterDataEx, sizeof( CharacterDataEx ) );

	bQuestLoaded					= FALSE;
	ZeroMemory( &saPacketQuestData, sizeof( PacketQuestUpdateData ) * _countof( saPacketQuestData ) );

	dwChatTradeTime					= FALSE;

	bDebugChat						= FALSE;

	ZeroMemory( &sSettingsData, sizeof( PacketSettingsData ) );

	bUnbrokeAge						= FALSE;

	bMuted							= FALSE;

	bInT5QuestArena					= FALSE;
	iRoundT5QuestArena				= 0;
	dwTimeT5QuestArenaUpdate		= 0;
	dwTimeT5QuestArenaSpawnUpdate	= 0;

	uQuestT5ID						= 0;
	sQuestT5ProgressValue[0]		= 0;
	sQuestT5ProgressValue[1]		= 0;

	bInGrandFury					= FALSE;
	dwGrandFuryTickEnter			= 0;

	dwBlindTime						= 0;
	dwBlindDamage					= 0;

	dwAlasEvasion					= 0;
	dwAlasTime						= 0;

	dwAdventMigalDamage				= 0;
	dwAdventMigalTime				= 0;

	sChasingHuntDamageBoost.sMin	= 0;
	sChasingHuntDamageBoost.sMax	= 0;
	dwChasingHuntTime				= 0;

	iHolyMindAbsorb					= 0;
	dwHolyMindTime					= 0;

	sSummonMuspellDamageBoost.sMin	= 0;
	sSummonMuspellDamageBoost.sMax	= 0;

	iWispAbsorb						= 0;
	dwWispTime						= 0;

	uLethalSightLevel				= 0;
	uLethalSightRange				= 0;
	uLethalSightCritical			= 0;
	uLethalSightDamageBoost			= 0;
	dwLethalSightTimeOut			= 0;

	uFierceWindLevel				= 0;
	dwFierceWindTimeOut				= 0;

	uHolyConvictionLevel			= 0;
	dwHolyConvictionTimeOut			= 0;

	uWizardTranceLevel				= 0;
	dwWizardTranceTimeOut			= 0;

	bWhirlWind						= false;

	bMagneticDischargeSelf			= false;
	uMagneticDischargeLevel			= 0;
	dwMagneticDischargeTimeOut		= 0;

	uKissViperLevel					= 0;
	dwKissViperTimeOut				= 0;
	uKissViperCriticalAdd			= 0;

	iTradeCoin						= 0;
	iTradeGold						= 0;

	bDieUpdate						= false;

	iBlessCastleScore				= 0;

	ZeroMemory( &sSkillStatus, sizeof( PacketSkillStatus ) );
	ZeroMemory( &sSkillContainer, sizeof( PacketSkillStatusContainer ) );

	uBufferSkillPosition			= 0;

	uPriorityCounter				= 0;
	uPriority						= 0;

	uBufferPosition					= 0;
	ZeroMemory( &sUnitStatusContainer, sizeof( PacketUnitStatusContainer ) );
	uBufferPositionM				= 0;
	ZeroMemory( &sUnitStatusContainerM, sizeof( PacketUnitStatusContainer ) );

	for ( int i = 0; i < 5; i++ )
		ZeroMemory( &baUnitStatusNew[i], 0x2000 );

	bParty							= FALSE;
	psParty							= NULL;

	if ( pcCaravanBase )
		DELET( pcCaravanBase->psCaravanData );
	DELET( pcCaravanBase );

	pcCaravan						= nullptr;
	vCaravanItems.clear();

	vAchievement.clear();

	if ( pcXmasTreeBase )
		DELET( pcXmasTreeBase->psXmasTreeData );
	DELET( pcXmasTreeBase );

	bInBattleRoyale					= false;

	eAccountShare					= ACCOUNTSHARE_None;

	iPvPHonorType					= 0;
	uPvPHonorTimeOut				= 0;
	iBellatraHonorType				= 0;
	uBellatraHonorTimeOut			= 0;
}


UserData::UserData()
{
}

BOOL UserData::IsDead()
{
	if ( this->sCharacterData.sHP.sCur <= 0 )
		return TRUE;

	return FALSE;
}