#pragma once
#include "QuestWindow.h"

enum EQUestT5ActiveStatus
{
	QUESTT5ACTIVE_None,
	QUESTT5ACTIVE_Active = 0x30,
	QUESTT5ACTIVE_Finishing = 0x31,
};

#define T5QUEST_ACTIVE				(*(int*)0x0362BA10)

class CQuestGame
{
protected:
	CQuestWindow					* pcWindow = NULL;
	Quest							* pcQuest = NULL;

	DWORD							dwTimeUpdate = 0;

	BOOL							bFinishingQuest = FALSE;

	static void						HandleFinish( QuestData * pcQuestData );
	static void						HandleTimeOver( QuestData * pcQuestData );

	void							HandleDeleteItem( int iQuestID, BOOL bFinished );

	void							HandleT5QuestCancel( int iQuestID );

	void							HandleAlertOnQuestDone( int iQuestID );

	DWORD							dwAcceptQuestTime = 0;

public:
	CQuestGame();
	virtual ~CQuestGame();

	CQuestWindow					* GetWindow() { return pcWindow; }
	Quest							* GetQuestInstance() { return pcQuest; }
	BOOL							IsFinishing();

	void							Init();

	void							SetQuestTimer( char * pszName, char * pszDescription, int iQuestID, int iTime, int iLevel );

	int								OnRenderTimer( Skill * psSkillData );

	int								GetQuestType( Skill * psSkill );

	BOOL							OnUnitKilled( UnitData * pcUnitData, int iMapID );
	BOOL							OnUnitPvPKilled( UnitData * pcUnitData );

	BOOL							OnItemCollect( int iItemID );

	void							HandlePacket( PacketQuestNPCOpen * psPacket );
	void							HandlePacket( PacketQuestNPCFinish * psPacket );
	void							HandlePacket( PacketQuestHandleCancel * psPacket );
	void							HandlePacket( PacketQuestStartData * psPacket );
	void							HandlePacket( PacketQuestNPCItemList * psPacket );
	void							HandlePacket( PacketHandleFinishedQuest * psPacket );
	void							HandlePacket( PacketQuestArenaTier5 * psPacket );
	void							HandlePacket( PacketDynamicQuestOpen * psPacket );
	void							HandlePacketCollecItem( PacketItem * psPacket );

	void							ProcessPacket( PacketQuestNPCAccept * psPacket );
	void							ProcessPacket( PacketQuestNPCFinish * psPacket );
	void							ProcessPacket( PacketDynamicQuestNPCOpen * psPacket );
	void							ProcessPacket( PacketQuestItemSwap * psPacket );

	void							ProcessQuestCancel( int iQuestID );

	void							HandleCancelQuest( int iTimerID, int iQuestID );

	void							Update();

	//T5
	BOOL							OnQuestT5OpenProgress();
	void							OnQuestT5Start();

	BOOL							OnMessageBoxT5Click( UIEventArgs e );
	BOOL							OnMessageBoxT5TeleportClick( UIEventArgs e );

	void							SkipOriginalQuests( BOOL bChatMessage );

	BOOL							CanGetItem( PacketItem * psPacket, int iPacketHeader );

	void							OnCharacterKilled();
};

