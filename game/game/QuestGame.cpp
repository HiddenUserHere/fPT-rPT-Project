#include "stdafx.h"
#include "QuestGame.h"


QuestGame::QuestGame()
{

}

QuestGame::~QuestGame()
{

}

BOOL QuestGame::KillTimers()
{
	TIMERSKILLHANDLER->KillTimer( 0x096A0003 );
	return TIMERSKILLHANDLER->KillTimer( 0x096A0002 );
}

void QuestGame::ProcessCancelQuest( DWORD dwQuestID )
{
	PacketQuestCancel sPacket;
	sPacket.iLength		= sizeof( PacketQuestCancel );
	sPacket.iHeader		= PKTHDR_CancelQuest;
	sPacket.dwQuestID	= dwQuestID;
	SENDPACKETL( &sPacket );
}

void QuestGame::ShowMessageBox( DWORD dwQuestID, const char * pszName )
{
	typedef void( __stdcall *tfnMessageBoxQuest )(DWORD dwMessageText, const char * pszName);
	tfnMessageBoxQuest fnMessageBoxQuest = (tfnMessageBoxQuest)0x004D1680;
	dwSelectQuestID = dwQuestID;
	fnMessageBoxQuest( 45, pszName );
}

void QuestGame::SetQuestTimer( const char * pszName, const char * pszDescription )
{
	typedef DWORD( __cdecl *tfnCreateTexture )(const char * pszFile, int iUnk1, int iUnk2, int iUnk3, int iUnk4, int iBlend);
	tfnCreateTexture fnCreateTexture = (tfnCreateTexture)0x0048A140;
	typedef DWORD( __cdecl *tfnSetSkillBuffer )(Skill * psSkill);
	tfnSetSkillBuffer fnSetSkillBuffer = (tfnSetSkillBuffer)0x00520F60;

	if ( (*(DWORD*)(0x367DF40)) == 0 )
	{
		(*(DWORD*)(0x367DF40)) = fnCreateTexture( "image\\Sinimage\\skill\\keep\\QuestTimer.tga", 0, 0, 0, 0, 1 );
		CALL( 0x0048A260 );
	}


	Skill sTempSkill;
	ZeroMemory( &sTempSkill, sizeof( Skill ) );
	STRINGCOPY( sTempSkill.sSkillInfo.szName, pszName );
	STRINGCOPY( sTempSkill.sSkillInfo.szDescription, pszDescription );
	sTempSkill.iID	= 0x096A0003;
	sTempSkill.iUseTime	= 70 * 70;
	sTempSkill.iLevel	= 1;
	sTempSkill.MatIcon	= (int)(*(DWORD*)(0x367DF40));
	fnSetSkillBuffer( &sTempSkill );
	PLAYMINISOUND( 25 );
}

BOOL QuestGame::CancelQuests()
{
	KillTimers();
	ChatGame::AddChatBoxText( "> All Active Quests cancelled!" );
	return TRUE;
}

BOOL QuestGame::IsFinishedQuest( DWORD dwQuestID )
{
	return FALSE;
}

BOOL QuestGame::IsActiveQuest( DWORD dwQuestID )
{
	return FALSE;
}

BOOL QuestGame::OnClickNpc( DWORD dwQuestID )
{
	switch ( dwQuestID )
	{
		case QUESTID_DailyQuestsIan:
			DAILYQUESTHANDLER->OnOpenNPC();
			break;

		case QUESTID_Warehouse2Page:
			if ( !IsFinishedQuest( QUESTID_WarehouseGold ) )
			{
				if ( TIMERSKILLHANDLER->FindSkillTimer( 0x096A0003 ) )
					return FALSE;
				TEXTWINDOW_OPEN( QUESTID_WarehouseGold, "game\\quests\\quest_wh\\1552.txt" );
				return FALSE;
			}
			if ( !IsFinishedQuest( dwQuestID ) && UNITDATA->sCharacterData.iRank >= 3 && UNITDATA->sCharacterData.iLevel >= 80 )
			{
				WAREHOUSEHANDLER->OpenNPCQuest();
			}
			break;

		case QUESTID_BlacksmithGodenGold:
			if ( !IsFinishedQuest( dwQuestID ) )
			{
				if ( UNITDATA->sCharacterData.iRank > 0 && UNITDATA->sCharacterData.iLevel >= 40 )
				{
					if ( TIMERSKILLHANDLER->FindSkillTimer( 0x096A0003 ) )
						return FALSE;

					TEXTWINDOW_OPEN( dwQuestID, "game\\quests\\quest_wh\\1551.txt" );
				}
			}
			break;

		default:
			break;
	}
	return FALSE;
}

BOOL QuestGame::OnClickQuestWindow( DWORD dwQuestID )
{
	switch ( dwQuestID )
	{
		case QUESTID_BlacksmithGodenGold:
			if ( !IsFinishedQuest( dwQuestID ) )
				FinishQuest( dwQuestID );
			break;

		case QUESTID_WarehouseGold:
			if ( !IsFinishedQuest( dwQuestID ) )
				FinishQuest( dwQuestID );
			break;

		case QUESTID_EasterNPC:
			EASTEREVENTHANDLER->OnClickOKNPC();
			return TRUE;
			break;

		case QUESTID_RudolphNPC:
			XMASHANDLER->OnClickOKNPC();
			return TRUE;
			break;

		default:
			break;
	}
	return FALSE;
}

void QuestGame::OnClickOkMessageBox(UIEventArgs eArgs)
{
	switch ( dwSelectQuestID )
	{
		case QUESTID_Quest110A:
			StartQuest( dwSelectQuestID );
			break;

		default:
			break;
	}
	dwSelectQuestID = 0;
}

BOOL QuestGame::OnKillUnit( UnitData * pcUnitData )
{
	// Notify Handlers
	DAILYQUESTHANDLER->OnUnitKilled( pcUnitData );

	char szBuffer[128]		= { 0 };
	QuestStatusData * psQuestData = QUESTGAMEHANDLER->GetData();
	BOOL bUpdate	= FALSE;
	BOOL bFinished	= FALSE;
	DWORD dwQuestID = 0;


	// Finished?
	if ( bFinished )
	{
		QUESTGAMEHANDLER->FinishQuest( dwQuestID );
	}

	return FALSE;
}

BOOL QuestGame::PHQuestData( PacketQuestData * psPacket )
{
	// Copy Data
	UpdateQuests();
	return TRUE;
}

BOOL QuestGame::PHQuestRestart( struct PacketQuestRestart * psPacket )
{
	DWORD dwQuestID = psPacket->dwQuestID;
	switch ( dwQuestID )
	{
		case QUESTID_DailyQuest80A:
			UpdateQuest( dwQuestID, QUESTSTATUSID_None );
			DAILYQUESTHANDLER->PHRestartQuest( psPacket );
			break;

		case QUESTID_DailyQuest85A:
			UpdateQuest( dwQuestID, QUESTSTATUSID_None );
			DAILYQUESTHANDLER->PHRestartQuest( psPacket );
			break;

		case QUESTID_DailyQuest90A:
			UpdateQuest( dwQuestID, QUESTSTATUSID_None );
			DAILYQUESTHANDLER->PHRestartQuest( psPacket );
			break;

		case QUESTID_DailyQuest95A:
			UpdateQuest( dwQuestID, QUESTSTATUSID_None );
			DAILYQUESTHANDLER->PHRestartQuest( psPacket );
			break;

		case QUESTID_DailyQuest100A:
			UpdateQuest( dwQuestID, QUESTSTATUSID_None );
			DAILYQUESTHANDLER->PHRestartQuest( psPacket );
			break;

		case QUESTID_DailyQuest105A:
			UpdateQuest( dwQuestID, QUESTSTATUSID_None );
			DAILYQUESTHANDLER->PHRestartQuest( psPacket );
			break;

		case QUESTID_DailyQuest110A:
			UpdateQuest( dwQuestID, QUESTSTATUSID_None );
			DAILYQUESTHANDLER->PHRestartQuest( psPacket );
			break;

		case QUESTID_DailyQuest115A:
			UpdateQuest( dwQuestID, QUESTSTATUSID_None );
			DAILYQUESTHANDLER->PHRestartQuest( psPacket );
			break;

		default:
			break;
	}
	return TRUE;
}

void QuestGame::UpdateQuests()
{
	// Find Active Quest
	if ( IsActiveQuest( QUESTID_DailyQuest80A ) )
	{
		SetQuestTimer( "The Babel's Army", "Kill 70 Monsters at Railway of Chaos" );
		dwCurrentQuestID = QUESTID_DailyQuest80A;
	}
	if ( IsActiveQuest( QUESTID_DailyQuest85A ) )
	{
		SetQuestTimer( "The Second Army", "Kill 100 Monsters at Heart of Perum" );
		dwCurrentQuestID = QUESTID_DailyQuest85A;
	}
	if ( IsActiveQuest( QUESTID_DailyQuest90A ) )
	{
		SetQuestTimer( "Help Andrei", "Kill 100 Monsters at Gallubia Valley" );
		dwCurrentQuestID = QUESTID_DailyQuest90A;
	}
	if ( IsActiveQuest( QUESTID_DailyQuest95A ) )
	{
		SetQuestTimer( "The Bosses Power", "Kill 100 Monsters at Frozen Sanctuary" );
		dwCurrentQuestID = QUESTID_DailyQuest95A;
	}
	if ( IsActiveQuest( QUESTID_DailyQuest100A ) )
	{
		SetQuestTimer( "The New Island", "Kill 100 Monsters at Lost Island" );
		dwCurrentQuestID = QUESTID_DailyQuest100A;
	}
	if ( IsActiveQuest( QUESTID_DailyQuest105A ) )
	{
		SetQuestTimer( "The Lost Temple", "Kill 100 Monsters at Lost Temple" );
		dwCurrentQuestID = QUESTID_DailyQuest105A;
	}
	if ( IsActiveQuest( QUESTID_DailyQuest110A ) )
	{
		SetQuestTimer( "Help Muriel", "Kill 100 Monsters at Endless Tower 1" );
		dwCurrentQuestID = QUESTID_DailyQuest110A;
	}
	if ( IsActiveQuest( QUESTID_DailyQuest115A ) )
	{
		SetQuestTimer( "The Gorgonyte Menace", "Kill 100 Monsters at Endless Tower 2" );
		dwCurrentQuestID = QUESTID_DailyQuest115A;
	}
	if ( IsActiveQuest( QUESTID_Warehouse2Page ) )
	{
		SetQuestTimer( "Warehouse #1", "Kill Monsters indicated by John" );
		dwCurrentQuestID = QUESTID_Warehouse2Page;
	}
	if ( IsActiveQuest( QUESTID_Quest110A ) )
	{
		SetQuestTimer( "Returning the Origins", "Kill Monsters indicated by The Guard" );
		dwCurrentQuestID = QUESTID_Quest110A;
	}
}

void QuestGame::StartQuest( DWORD dwQuestID )
{
	BOOL bStartQuest = TRUE;
	QuestStatusData * psQuestData = GetData();

	if ( TIMERSKILLHANDLER->FindSkillTimer( 0x096A0003 ) )
	{
		TITLEBOX( "You are already in a quest" );
		return;
	}
	UpdateQuests();
}

BOOL QuestGame::RestartQuest( DWORD dwQuestID )
{
	PacketQuestRestart sPacket;
	sPacket.iLength		= sizeof( PacketQuestRestart );
	sPacket.iHeader		= PKTHDR_RestartQuest;
	sPacket.dwQuestID	= dwQuestID;
	SENDPACKETL( &sPacket );
	return TRUE;
}

BOOL QuestGame::FinishQuest( DWORD dwQuestID )
{
	PacketQuestFinish sPacket;
	sPacket.iLength			= sizeof( PacketQuestFinish );
	sPacket.iHeader			= PKTHDR_FinishQuest;
	sPacket.dwQuestID		= dwQuestID;
	sPacket.eQuestStatus	= QUESTSTATUSID_Finished;
	SENDPACKETL( &sPacket );
	UpdateQuest( dwQuestID, QUESTSTATUSID_Finished );
	KillTimers();
	return TRUE;
}

BOOL QuestGame::UpdateQuest( DWORD dwQuestID, EQuestStatus eStatus )
{
	return TRUE;
}

