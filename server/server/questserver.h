#pragma once

struct QuestActiveMonsterKillData
{
	int						iQuestID;
	int						iPercentIncreaseEXP;
	int						iDropReduce;

	int						iMonsterCount;
	int						iaMonsterID[QUEST_MONSTERITEM_MAX];
	int						iaMonsterKillCount[QUEST_MONSTERITEM_MAX];

	int						iMapCount;
	int						iaMapID[MAX_QUEST_MAPDATA];
};

class QuestServer
{
public:
												QuestServer();
	virtual									   ~QuestServer();

	//SQL Data
	BOOL										SQLMultipleQuest( int iQuestID );
	std::vector<int>							SQLDenyMultipleQuest( int iQuestID );

	int											SQLHaveQuest( User * pcUser, int iQuestID, BOOL bStart = FALSE );
	BOOL										SQLCanGetQuest( User * pcUser, int iQuestID, BOOL bStart = FALSE );

	BOOL										SQLCancelQuest( User * pcUser, int iQuestID, BOOL bAutoStartQuest );

	BOOL										SQLFinishQuest( User * pcUser, int iQuestID );

	int											SQLGetAutoStartQuestID( User * pcUser, int iQuestID );
	int											SQLGetWasAutoStartQuestID( User * pcUser, int iQuestID );
	std::vector<int>							SQLGetActiveQuestsID( User * pcUser );
	BOOL										SQLExtraRewardData( int iID, int & iType, char * pszReward, char * pszRewardValue );

	BOOL										HandleFinishedQuestUser( User * pcUser, int iQuestID = 0 );

	int											SQLGetQuestTimeLeft( User * pcUser, int iQuestID, DWORD & dwTimeLeft );
	int											SQLGetQuestTimeLeft( int iQuestID, DWORD & dwTimeLeft );

	BOOL										SQLGetAreaQuest( int iQuestID, int * piaMapID, int & iMapCount, int & iMinX, int & iMaxX, int & iMinZ, int & iMaxZ, int & iType, int & iRadius );

	int											SQLGetRewardQuestID( int iQuestID );
	BOOL										SQLGetMonsterItem( int iQuestID, char * pszMonster, char * pszMonsterCounter, char * pszItem, char * pszItemCounter );
	BOOL										SQLGetMonsterItemCharacter( User * pcUser, int iQuestID, char * pszMonsterCounter, char * pszItemCounter );
	BOOL										SQLGetQuestNameShortDesc( int iQuestID, char * pszName, char * pszShortDescription, BOOL & bPvP, int & iParty, int & iType, int & iLevel, BOOL & bMultiple, DWORD & dwWaitingTime, DWORD & dwTimeTotal );
	BOOL										SQLGetDynamicQuestShortData( int iQuestID, char * pszName, IMinMax & sLevel );

	BOOL										SQLStartQuest( User * pcUser, int iQuestID, char * pszMonster, char * pszMonsterCounter, char * pszItem, char * pszItemCounter );
	BOOL										SQLQuestSwapClass( char * pszCharacterName, int iOldQuestID, int iNewQuestID );

	BOOL										LoadQuestData( User * pcUser, DWORD dwQuestID, EQuestStatus eStatus, DWORD dwValue1, DWORD dwValue2, DWORD dwValue3, DWORD dwValue4, DWORD dwValue5, SYSTEMTIME * psFinishDate );
	BOOL										OnLoadUser( User * pcUser );
	BOOL										OnUnLoadUser( User * pcUser );


	void										OnSayTime( UnitData * pcUnitData );

	BOOL										OnClickDynamicNPC( User * pcUser, UnitData * pcUnitData );
	BOOL										OnClickNPC( User * pcUser, UnitData * pcUnitData );

	void										HandlePacket( User * pcUser, PacketQuestNPCAccept * psPacket );
	void										HandlePacket( User * pcUser, PacketQuestHandleCancel * psPacket );
	void										HandlePacketUnload( User * pcUser, PacketQuestUpdateData * psPacket );
	void										HandlePacket( User * pcUser, PacketQuestUpdateData * psPacket );
	void										HandlePacket( User * pcUser, PacketNetQuestUpdateData * psPacket );
	void										HandlePacket( User * pcUser, PacketQuestNPCFinish * psPacket );
	void										HandlePacket( User * pcUser, PacketDynamicQuestNPCOpen * psPacket );
	void										HandlePacket( User * pcUser, PacketQuestItemSwap * psPacket );
	void										HandlePacket( User * pcUser, PacketCustomQuestValue * psPacket );

	void										HandleLoadQuests( User * pcUser );

	void										ProcessPacket( User * pcUser, PacketQuestStartData * psPacket );
	void										ProcessPacket( User * pcUser, PacketDynamicQuestOpen * psPacket );

	//T5
	void										ProcessT5Quests( User * pcUser, int iQuestID, int iValue1, int iValue2 );

	void										HandlePacket( PacketNetT5QuestData * psPacket );

	int											GetReduceQuestDrop( User * pcUser, UnitData * pcUnitData );

	INT64										GetQuestEXPOnKill( User * pcUser, UnitData * pcUnitData, INT64 iEXP );

	void										UpdateQuestActiveMonsterKill();

	//Kill
	BOOL										OnUnitKilled( User * pcUser, Unit * pcUnit );

	BOOL										IsActiveQuest( User * pcUser, int iQuestID, PacketQuestUpdateData * psData = NULL );

	BOOL										SkipTier5Quests( User * pcUser );

	BOOL										SkipQuestDance( User * pcUser );

private:

	struct QuestItemRewardData
	{
		ItemData			* pcItemData;
		int					iQuantity;
	};

	BOOL										IsT5QuestProgress( int iQuestID );

	std::vector<QuestActiveMonsterKillData*>	vQuestActiveMonsterKill;
};

