#include "stdafx.h"
#include "DailyQuest.h"


DailyQuest::DailyQuest()
{
}


DailyQuest::~DailyQuest()
{
	DELET( pcDailyQuestWindow );
}

void DailyQuest::Init()
{
	pcDailyQuestWindow = new DailyQuestWindow();
	pcDailyQuestWindow->Init();
}

BOOL DailyQuest::OnOpenNPC()
{
	GetWindow()->Open();
	return TRUE;
}

BOOL DailyQuest::IsFinishedQuestIan()
{
	BOOL bRet = FALSE;

	if ( QUESTGAMEHANDLER->IsFinishedQuest( QUESTID_DailyQuest80A ) )
		bRet = TRUE;
	else if ( QUESTGAMEHANDLER->IsFinishedQuest( QUESTID_DailyQuest85A ) )
		bRet = TRUE;
	else if ( QUESTGAMEHANDLER->IsFinishedQuest( QUESTID_DailyQuest90A ) )
		bRet = TRUE;
	else if ( QUESTGAMEHANDLER->IsFinishedQuest( QUESTID_DailyQuest95A ) )
		bRet = TRUE;
	else if ( QUESTGAMEHANDLER->IsFinishedQuest( QUESTID_DailyQuest100A ) )
		bRet = TRUE;
	else if ( QUESTGAMEHANDLER->IsFinishedQuest( QUESTID_DailyQuest105A ) )
		bRet = TRUE;
	else if ( QUESTGAMEHANDLER->IsFinishedQuest( QUESTID_DailyQuest110A ) )
		bRet = TRUE;
	else if ( QUESTGAMEHANDLER->IsFinishedQuest( QUESTID_DailyQuest115A ) )
		bRet = TRUE;

	return bRet;
}

void DailyQuest::StartQuest( DWORD dwQuestID )
{
	QUESTGAMEHANDLER->StartQuest( dwQuestID );
}

void DailyQuest::PHRestartQuest( PacketQuestRestart * psPacket )
{
	GetWindow()->OpenSubWindow( psPacket->dwQuestID );
}

void DailyQuest::OnUnitKilled( UnitData * pcUnit )
{
	char szBuffer[128]		= { 0 };
	QuestStatusData * psQuestData = QUESTGAMEHANDLER->GetData();
	BOOL bUpdate	= FALSE;
	BOOL bFinished	= FALSE;
	DWORD dwQuestID = 0;


	// Message?
	if ( szBuffer[0] != 0 )
		ChatGame::AddChatBoxText( szBuffer, CHATCOLOR_Notice );

	// Finished?
	if ( bFinished )
	{
		QUESTGAMEHANDLER->FinishQuest( dwQuestID );
	}
}

int DailyQuest::OnRenderTimer( Skill * psSkill )
{
	int iLine = 0;
	QuestStatusData * psQuestData = QUESTGAMEHANDLER->GetData();
	char szBuffer[128] = { 0 };

	return iLine;
}
