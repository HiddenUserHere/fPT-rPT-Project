#pragma once

#include "CDailyQuestWindow.h"

class DailyQuest
{
private:
	DailyQuestWindow							* pcDailyQuestWindow = NULL;
public:
	DailyQuest();
	virtual ~DailyQuest();

	DailyQuestWindow							* GetWindow() { return pcDailyQuestWindow; };

	BOOL										OnOpenNPC();

	BOOL										IsFinishedQuestIan();

	void										Init();

	void										StartQuest( DWORD dwQuestID );
	void										PHRestartQuest( struct PacketQuestRestart * psPacket );

	void										OnUnitKilled( UnitData * pcUnit );

	int											OnRenderTimer( Skill * psSkill );
};

