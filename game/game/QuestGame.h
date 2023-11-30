#pragma once


struct QuestTimer
{ 
	DWORD										dwQuestID;
	DWORD										dwBackUpCode;
	int											Kind;
	int											iCount;    
	short										sMonsterCount[2];   // Count Monsters
};


class QuestGame
{
protected:
	struct QuestStatusData							* psQuestData = NULL;
	DWORD										dwCurrentQuestType		= 0;
	DWORD										dwCurrentQuestID		= 0;
private:
	BOOL										KillTimers();
	void										ShowMessageBox( DWORD dwQuestID, const char * pszName );
	void										SetQuestTimer( const char * pszName, const char * pszDescription );
	void										ProcessCancelQuest( DWORD dwQuestID );

	DWORD										dwSelectQuestID = 0;


public:
	QuestGame();
	virtual ~QuestGame();

	struct QuestStatusData						* GetData() { return psQuestData; };

	BOOL										IsFinishedQuest( DWORD dwQuestID );
	BOOL										IsActiveQuest( DWORD dwQuestID );
	DWORD										IsInQuest() { return dwCurrentQuestID; };

	BOOL										CancelQuests();

	BOOL										OnClickNpc( DWORD dwQuestID );
	BOOL										OnClickQuestWindow( DWORD dwQuestID );
	void										OnClickOkMessageBox(UIEventArgs eArgs);
							
	DWORD										GetQuestType() { return dwCurrentQuestType; };

	BOOL										OnKillUnit( UnitData * pcUnit );

	BOOL										PHQuestData( struct PacketQuestData * psPacket );
	BOOL										PHQuestRestart( struct PacketQuestRestart * psPacket );

	void										StartQuest( DWORD dwQuestID );
	BOOL										RestartQuest( DWORD dwQuestID );
	BOOL										FinishQuest( DWORD dwQuestID );
	BOOL										UpdateQuest( DWORD dwQuestID, enum EQuestStatus eStatus );
	void										SendQuests();

	void										UpdateQuests();

};

