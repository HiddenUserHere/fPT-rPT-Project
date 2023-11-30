#include "stdafx.h"
#include "CQuestGame.h"


void CQuestGame::HandleFinish( QuestData * pcQuestData )
{
	if ( pcQuestData )
	{
		TITLEBOX( "%s completed!", pcQuestData->GetName() );
		PacketQuestUpdateData s;
		s.iLength = sizeof( PacketQuestUpdateData );
		s.iHeader = PKTHDR_QuestUpdateData;

		CopyMemory( s.iaMonsterKilled, pcQuestData->GetMonsterKilledData(), sizeof( int ) * QUEST_MONSTERITEM_MAX );
		CopyMemory( s.iaItemsCollected, pcQuestData->GetItemsCollectData(), sizeof( int ) * QUEST_MONSTERITEM_MAX );

		s.iItemsCount = pcQuestData->GetItemCount();
		s.iMonsterCount = pcQuestData->GetMonsterCount();
		s.dwTimeLeft = pcQuestData->GetTimeQuest();
		s.iID = pcQuestData->GetID();
		SENDPACKETL( &s, TRUE );
	}
}

void CQuestGame::HandleTimeOver( QuestData * pcQuestData )
{
	TITLEBOX( "%s time is over!", pcQuestData->GetName() );

	TIMERSKILLHANDLER->KillTimerQuest( pcQuestData->IsTimeQuest() ? 0x096B0003 : 0x096A0003, pcQuestData->GetID() );
}

void CQuestGame::HandleDeleteItem( int iQuestID, BOOL bFinished )
{
	switch ( iQuestID )
	{
		case QUESTID_AMatterOfStrenght:
			ITEMHANDLER->DeleteItemQuest( ITEMID_ValentoEye );
			ITEMHANDLER->DeleteItemQuest( ITEMID_PrinceEye );
			break;

		case QUESTID_CryMeARiver:
			ITEMHANDLER->DeleteItemQuest( ITEMID_DiamondTears );
			break;

		case QUESTID_GreenLiquidQuestStep1:
		case QUESTID_GreenLiquidQuestStep2:
			ITEMHANDLER->DeleteItemQuest( ITEMID_PotionAgingQuestBee );
			ITEMHANDLER->DeleteItemQuest( ITEMID_GreenLiquidTree );
			ITEMHANDLER->DeleteItemQuest( ITEMID_MagicRune, 5 );
			break;

		default:
			break;
	}
}

void CQuestGame::HandleT5QuestCancel( int iQuestID )
{
	if ( iQuestID == QUESTID_T5QuestArena )
	{
		T5QUEST_ACTIVE = QUESTT5ACTIVE_None;
	}
}

void CQuestGame::HandleAlertOnQuestDone( int iQuestID )
{
	char szTextTitle[256]	= { 0 };
	char szTextChat[256]	= { 0 };

	int iPointsSkills = 0;

	//T5 Quest
	if ( (iQuestID == QUESTID_AMatterOfStrenght) || (iQuestID == QUESTID_CryMeARiver) || (iQuestID == QUESTID_TheLastStage) ||
		(iQuestID == QUESTID_AnEndlessBattle) || (iQuestID == QUESTID_AnEndlessSuffering) || (iQuestID == QUESTID_ATestOfBravery) ||
		(iQuestID == QUESTID_MercyfortheLost2) || (iQuestID == QUESTID_HuntingTheIronGuardian) )
	{
		iPointsSkills = 1;

		if ( (iQuestID == QUESTID_MercyfortheLost2) || (iQuestID == QUESTID_HuntingTheIronGuardian) )
			iPointsSkills = 2;
	}

	if ( iPointsSkills > 0 )
	{
		STRINGFORMAT( szTextTitle, "You won %d Regnum Skill Point%s!", iPointsSkills, iPointsSkills > 1 ? "s" : "" );
		STRINGFORMAT( szTextChat, "> You won %d Regnum Skill Point%s!", iPointsSkills, iPointsSkills > 1 ? "s" : "" );
	}

	if ( szTextTitle[0] != 0 )
		TITLEBOX( szTextTitle );
}

CQuestGame::CQuestGame()
{
	pcWindow = new CQuestWindow();
	pcQuest = new Quest();
}


CQuestGame::~CQuestGame()
{
	DELET( pcWindow );
	DELET( pcQuest );
}

BOOL CQuestGame::IsFinishing()
{
	return bFinishingQuest;
}

void CQuestGame::Init()
{
	pcWindow->Init();
}

void CQuestGame::SetQuestTimer( char * pszName, char * pszDescription, int iQuestID, int iTime, int iLevel )
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
	
	sTempSkill.iID			= iTime != 0 ? 0x096B0003 : 0x096A0003;
	sTempSkill.iUseTime		= iTime != 0 ? iTime : 70 * 70;
	sTempSkill.iLevel		= 1;
	sTempSkill.MatIcon		= (int)(*(DWORD*)(0x367DF40));
	sTempSkill.PlusState[4] = iQuestID;
	sTempSkill.sSkillInfo.iSkillLevelRequired = iLevel;
	fnSetSkillBuffer( &sTempSkill );
	
	PLAYMINISOUND( 25 );
}

int CQuestGame::OnRenderTimer( Skill * psSkillData )
{
	int iLineCount = 0;
	
	for ( std::vector<QuestData*>::iterator it = pcQuest->GetData().begin(); it != pcQuest->GetData().end(); it++ )
	{
		QuestData * pc = (*it);

		if ( pc && pc->GetID() == psSkillData->PlusState[4] )
		{
			if ( pc->GetMonsterCounter( 0 ) > 0 )
			{
				for ( int i = 0; i < QUEST_MONSTERITEM_MAX; i++ )
				{
					if ( pc->GetMonsterCounter( i ) == 0 )
						break;

					if ( pc->GetMonsterCounter( i ) > 0 )
					{
						if ( pc->GetMapIDs().size() > 1 )
							SKILLMANAGERHANDLER->AddSkillInfo( 1, "%s:\r", MAPNAME( pc->GetMapIDs()[i] ) );
						else
							SKILLMANAGERHANDLER->AddSkillInfo( 1, "%s:\r", pc->GetMonsterName( i ) );

						SKILLMANAGERHANDLER->AddSkillInfo( 2, "%d/%d\r", pc->GetMonsterKilledAmount( i ), pc->GetMonsterCounter( i ) );
						iLineCount++;
					}
				}
			}
		
			if ( pc->GetItemCounter( 0 ) > 0 )
			{
				if ( iLineCount > 0 )
				{
					//Next Line
					SKILLMANAGERHANDLER->AddSkillInfo( 1, "\r" );
					SKILLMANAGERHANDLER->AddSkillInfo( 2, "\r" );
					iLineCount += 1;
				}

				for ( int i = 0; i < QUEST_MONSTERITEM_MAX; i++ )
				{
					if ( pc->GetItemCounter( i ) == 0 )
						break;

					if ( pc->GetItemCounter( i ) > 0 )
					{
						SKILLMANAGERHANDLER->AddSkillInfo( 1, "%s:\r", pc->GetItemName( i ) );
						SKILLMANAGERHANDLER->AddSkillInfo( 2, "%d/%d\r", pc->GetItemsCollected( i ), pc->GetItemCounter( i ) );
						iLineCount += 1;
					}
				}
			}

			if ( pc->IsTimeQuest() && (pc->GetWaitTime() > 0) )
			{
				int iHour = abs( (int)pc->GetWaitTime() / 3600 );
				int iMinutes = abs( (int)pc->GetWaitTime() / 60 ) % 60;
				int iSeconds = abs( (int)pc->GetWaitTime() % 60 );

				SKILLMANAGERHANDLER->AddSkillInfo( 1, "Wait Time:\r" );
				SKILLMANAGERHANDLER->AddSkillInfo( 2, "%02d:%02d:%02d\r", iHour, iMinutes, iSeconds );
				iLineCount += 1;
			}
		}
	}

	return iLineCount;
}

int CQuestGame::GetQuestType( Skill * psSkill )
{
	int iRet = 0;

	for ( std::vector<QuestData*>::iterator it = pcQuest->GetData().begin(); it != pcQuest->GetData().end(); it++ )
	{
		QuestData * pc = (*it);

		if ( pc && pc->GetID() == psSkill->PlusState[4] )
		{
			//Solo
			if ( pc->GetPartyType() == 0 )
				iRet = 1;

			//Party
			if ( pc->GetPartyType() == 1 )
				iRet = 2;

			//Raid
			if ( pc->GetPartyType() == 2 )
				iRet = 3;

			//Raid
			if ( pc->GetPartyType() == 3 )
				iRet = 4;

			//Both
			if ( pc->GetPartyType() == 4 )
				iRet = 5;

			break;
		}
	}

	return iRet;
}

BOOL CQuestGame::OnUnitKilled( UnitData * pcUnitData, int iMapID )
{
	BOOL bParty = PARTYHANDLER->GetPartyData().iMembersCount > 0 ? TRUE : FALSE;
	BOOL bRaid = PARTYHANDLER->GetPartyData().iRaidCount > 0 ? TRUE : FALSE;
	return pcQuest->HandleKill( pcUnitData->sCharacterData.iStatPoints, iMapID, pcUnitData->sPosition.iX, pcUnitData->sPosition.iZ, bParty, bRaid, FALSE );
}

BOOL CQuestGame::OnUnitPvPKilled( UnitData * pcUnitData )
{
	if ( pcUnitData )
	{
		BOOL bParty = PARTYHANDLER->GetPartyData().iMembersCount > 0 ? TRUE : FALSE;
		BOOL bRaid = PARTYHANDLER->GetPartyData().iRaidCount > 0 ? TRUE : FALSE;
		return pcQuest->HandleKill( pcUnitData->sCharacterData.iStatPoints, CHARACTERGAME->GetStageID(), pcUnitData->sPosition.iX, pcUnitData->sPosition.iZ, bParty, bRaid, TRUE );
	}
	return FALSE;
}

BOOL CQuestGame::OnItemCollect( int iItemID )
{
	BOOL bParty = PARTYHANDLER->GetPartyData().iMembersCount > 0 ? TRUE : FALSE;
	BOOL bRaid = PARTYHANDLER->GetPartyData().iRaidCount > 0 ? TRUE : FALSE;
	return pcQuest->HandleCollectedItem( iItemID, CHARACTERGAME->GetStageID(), UNITDATA->sPosition.iX, UNITDATA->sPosition.iZ, bParty, bRaid );
}

void CQuestGame::HandlePacket( PacketQuestNPCOpen * psPacket )
{
	QuestData * pcQuestData = pcQuest->GetQuest( psPacket->iID );
	
	if ( pcQuestData )
	{
		if ( pcQuestData->IsFinished() )
		{
			if ( (psPacket->iType == 3 || psPacket->iType == 4) )
				GetWindow()->Open( psPacket->iID, psPacket->szName, psPacket->szConclusion, psPacket->iType );
		}
		else if ( (psPacket->iType == 2 || psPacket->iType == 3) )
			GetWindow()->Open( psPacket->iID, psPacket->szName, psPacket->szProgressText, psPacket->iType );
	}
	else if ( psPacket->iType == 1 )
		GetWindow()->Open( psPacket->iID, psPacket->szName, psPacket->szDescription, psPacket->iType );
}

void CQuestGame::HandlePacket( PacketQuestNPCFinish * psPacket )
{
	QuestData * pcQuestData = pcQuest->GetQuest( psPacket->iID );
	if ( pcQuestData )
	{
		TIMERSKILLHANDLER->KillTimerQuest( pcQuestData->IsTimeQuest() ? 0x096B0003 : 0x096A0003, pcQuestData->GetID() );
		HandleDeleteItem( pcQuestData->GetID(), TRUE );
		pcQuest->DeleteQuest( pcQuestData->GetID() );
	}

	bFinishingQuest = FALSE;
}

void CQuestGame::HandlePacket( PacketQuestHandleCancel * psPacket )
{
	QuestData * pcQuestData = pcQuest->GetQuest( psPacket->iID );
	if ( pcQuestData )
	{
		TIMERSKILLHANDLER->KillTimerQuest( pcQuestData->IsTimeQuest() ? 0x096B0003 : 0x096A0003, pcQuestData->GetID() );
		HandleDeleteItem( pcQuestData->GetID(), FALSE );

		HandleT5QuestCancel( pcQuestData->GetID() );

		CQUESTGAME->GetQuestInstance()->DeleteQuest( pcQuestData->GetID() );
	}
}

void CQuestGame::HandlePacket( PacketQuestStartData * psPacket )
{
	QuestData * pcQuestData = new QuestData( psPacket );

	if ( pcQuestData->GetItemCounter( 0 ) > 0 )
	{
		for ( int i = 0; i < QUEST_MONSTERITEM_MAX; i++ )
		{
			if ( pcQuestData->GetItemCounter( 0 ) == 0 )
				break;

			if ( pcQuestData->GetItemCounter( i ) > 0 )
			{
				ItemData * pc = ITEMHANDLER->FindItemTableByCode( pcQuestData->GetItemID( i ) );
				
				if ( pc )
					STRINGCOPYLEN( pcQuestData->GetItemName( i ), 32, pc->szBaseName );
			}
		}
	}

	DWORD dwTime = pcQuestData->IsTimeQuest() ? pcQuestData->GetTimeQuest() : 0;

	if ( dwTime == 0 && pcQuestData->IsTimeQuest() )
		dwTime = 1;

	//Timer
	SetQuestTimer( pcQuestData->GetName(), pcQuestData->GetDescription(), pcQuestData->GetID(), dwTime, pcQuestData->GetLevel() );

	pcQuestData->SetFunctionFinishHandle( &CQuestGame::HandleFinish );

	pcQuest->AddQuest( pcQuestData );

	pcQuestData->Update( TRUE );

	//T5 Quest?
	if ( pcQuestData->GetID() == QUESTID_T5QuestArena )
		T5QUEST_ACTIVE = QUESTT5ACTIVE_Active;
}

void CQuestGame::HandlePacket( PacketQuestNPCItemList * psPacket )
{
	ItemData * pcItemData = ITEMHANDLER->FindItemTableByCode( psPacket->cItemData.sItem.sItemID.ToInt() );
	if ( pcItemData )
	{
		ItemData sNew;
		ITEMHANDLER->LoadItemImage( &psPacket->cItemData.sItem, &sNew );

		GetWindow()->AddItem( &sNew );
	}
	
	if ( psPacket->bLast )
		GetWindow()->OpenItems( psPacket->iID );
}

void CQuestGame::HandlePacket( PacketHandleFinishedQuest * psPacket )
{
	GetQuestInstance()->AddFinishedQuest( psPacket );

	if ( psPacket->bLoginTime == FALSE )
	{
		if ( psPacket->iID == QUESTID_TheLoyaltyTestStep2 )
		{
			UINT iQuestFlag = READDWORD( 0x0362BA68 );
			iQuestFlag |= QUESTSAVEFLAG_TheLoyaltyTest;
			WRITEDWORD( 0x0362BA68, iQuestFlag );
			CGameProtect::ValidateStatusPoints( UNITDATA->sCharacterData, READDWORD( 0x0362BA68 ) );
			UPDATE_CHARACTER_CHECKSUM;
		}

		HandleAlertOnQuestDone( psPacket->iID );
	}
}

void CQuestGame::HandlePacket( PacketQuestArenaTier5 * psPacket )
{
	if ( psPacket->iType == PacketQuestArenaTier5::EType::TYPEID_FinishQuest )
	{
		BOOL bParty = PARTYHANDLER->GetPartyData().iMembersCount > 0 ? TRUE : FALSE;
		BOOL bRaid = PARTYHANDLER->GetPartyData().iRaidCount > 0 ? TRUE : FALSE;

		pcQuest->HandleKill( QUESTMONSTERID_CrystalTower, MAPID_T5QuestArena, UNITDATA->sPosition.iX, UNITDATA->sPosition.iY, bParty, bRaid, FALSE );
	}
}

void CQuestGame::HandlePacket( PacketDynamicQuestOpen * psPacket )
{
	GetWindow()->ClearDynamicQuest();
	GetWindow()->SetDynamicQuestMain( psPacket->szImagePath, psPacket->szMainText );
	
	for ( int i = 0; i < psPacket->iQuestCount; i++ )
	{
		if ( CQUESTGAME->GetQuestInstance()->IsActiveQuest( psPacket->saQuestData[i].iQuestID ) )
		{
			GetWindow()->ClearDynamicQuest();
			TITLEBOX( "You have an active quest!" );
			return;
		}

		GetWindow()->AddDynamicQuestMainMenu( &psPacket->saQuestData[i] );
	}

	GetWindow()->OpenDynamicQuestWindow();
}

void CQuestGame::HandlePacketCollecItem( PacketItem * psPacket )
{
	BOOL bParty = PARTYHANDLER->GetPartyData().iMembersCount > 0 ? TRUE : FALSE;
	BOOL bRaid = PARTYHANDLER->GetPartyData().iRaidCount > 0 ? TRUE : FALSE;
	pcQuest->HandleCollectedItem( psPacket->sItem.sItemID.ToInt(), CHARACTERGAME->GetStageID(), UNITDATA->sPosition.iX, UNITDATA->sPosition.iZ, bParty, bRaid );
}

void CQuestGame::ProcessPacket( PacketQuestNPCAccept * psPacket )
{
	if ( dwAcceptQuestTime > TICKCOUNT )
		return;

	psPacket->iLength = sizeof( PacketQuestNPCAccept );
	psPacket->iHeader = PKTHDR_QuestNPCAccept;
	SENDPACKETL( psPacket, TRUE );

	dwAcceptQuestTime = TICKCOUNT + 2000;
}

void CQuestGame::ProcessPacket( PacketQuestNPCFinish * psPacket )
{
	bFinishingQuest = TRUE;

	psPacket->iLength = sizeof( PacketQuestNPCFinish );
	psPacket->iHeader = PKTHDR_QuestNPCFinish;
	
	psPacket->bHaveSpace = TRUE;

	if ( !CHARACTERGAME->CheckInventorySpace( ITEMHANDLER->FindItemTableByCode( ITEMID_BrazilianSoccer ) ) )
		psPacket->bHaveSpace = FALSE;

	psPacket->sWeight[0] = UNITDATA->sCharacterData.sWeight.sCur;
	psPacket->sWeight[1] = UNITDATA->sCharacterData.sWeight.sMax;

	SENDPACKETL( psPacket, TRUE );
}

void CQuestGame::ProcessPacket( PacketDynamicQuestNPCOpen * psPacket )
{
	psPacket->iLength = sizeof( PacketDynamicQuestNPCOpen );
	psPacket->iHeader = PKTHDR_DynamicQuestOpenNPC;
	SENDPACKETG( psPacket );
}

void CQuestGame::ProcessPacket( PacketQuestItemSwap * psPacket )
{
	psPacket->iLength = sizeof( PacketQuestItemSwap );
	psPacket->iHeader = PKTHDR_QuestItemSwap;
	SENDPACKETL( psPacket );
}

void CQuestGame::ProcessQuestCancel( int iQuestID )
{
	PacketQuestHandleCancel s;
	s.iLength = sizeof( PacketQuestHandleCancel );
	s.iHeader = PKTHDR_QuestCancel;
	s.iID = iQuestID;
	SENDPACKETL( &s, TRUE );
}

void CQuestGame::HandleCancelQuest( int iTimerID, int iQuestID )
{
	if ( MESSAGEBOX->GetType() == FALSE )
		return;
	else
	{
		QuestData * pc = CQUESTGAME->GetQuestInstance()->GetQuest( iQuestID );
		if ( pc )
		{
			if ( iQuestID == QUESTID_T5QuestArena )
			{
				if ( MAP_ID == MAPID_T5QuestArena )
				{
					TITLEBOX( "You can't cancel this quest now!" );
					return;
				}
			}

			ChatGame::AddChatBoxTextEx( CHATCOLOR_Error, "> %s cancelled!", pc->GetName() );
			HandleDeleteItem( iQuestID, FALSE );
			CQUESTGAME->ProcessQuestCancel( iQuestID );
			CQUESTGAME->GetQuestInstance()->DeleteQuest( iQuestID );
			TIMERSKILLHANDLER->KillTimerQuest( iTimerID, iQuestID );

			HandleT5QuestCancel( iQuestID );
		}
	}
}

void CQuestGame::Update()
{
	if ( dwTimeUpdate == 0 )
		dwTimeUpdate = TICKCOUNT + 5000;

	for ( std::vector<QuestData*>::iterator it = pcQuest->GetData().begin(); it != pcQuest->GetData().end(); )
	{
		QuestData * pc = (*it);

		if ( pc )
		{
			pc->Update( 1.0f );
			
			if ( pc->IsTimeQuest() && pc->GetTimeQuest() == 0 )
			{
				CQuestGame::HandleTimeOver( pc );

				HandleDeleteItem( pc->GetID(), FALSE );

				ProcessQuestCancel( pc->GetID() );

				HandleT5QuestCancel( pc->GetID() );

				DELET( pc );

				it = pcQuest->GetData().erase( it );
			}
			else
				it++;
		}
		else
			it++;
	}

	if ( dwTimeUpdate <= TICKCOUNT )
	{
		if ( T5QUEST_ACTIVE == QUESTT5ACTIVE_Active )
		{
			if ( pcQuest->IsActiveQuest( QUESTID_T5QuestArena ) == FALSE )
				T5QUEST_ACTIVE = QUESTT5ACTIVE_None;
		}

		PacketQuestUpdateData s;
		s.iLength = sizeof( PacketQuestUpdateData );
		s.iHeader = PKTHDR_QuestUpdateData;

		for ( std::vector<QuestData*>::iterator it = pcQuest->GetData().begin(); it != pcQuest->GetData().end(); it++ )
		{
			QuestData * pc = (*it);
			
			if ( pc )
			{
				if ( pc->Update() )
				{
					CopyMemory( s.iaMonsterKilled, pc->GetMonsterKilledData(), sizeof( int ) * QUEST_MONSTERITEM_MAX );
					CopyMemory( s.iaItemsCollected, pc->GetItemsCollectData(), sizeof( int ) * QUEST_MONSTERITEM_MAX );

					s.iItemsCount = pc->GetItemCount();
					s.iMonsterCount = pc->GetMonsterCount();
					s.dwTimeLeft = pc->GetTimeQuest();
					s.iID = pc->GetID();

					SENDPACKETL( &s, TRUE );

					pc->Update( FALSE );
				}
				else if ( pc->IsValidQuest() == false )
				{
					PacketLogCheatEx sPacket;
					sPacket.iLength = sizeof( PacketLogCheatEx );
					sPacket.iHeader = PKTHDR_LogCheat;
					sPacket.iCheatID = CHEATLOGID_CharacterQuestInvalid;
					sPacket.LParam = pc->GetID();
					sPacket.SParam = pc->GetQuestValidate();
					sPacket.EParam = pc->GetCurrentValidateQuestCode();
					STRINGCOPY( sPacket.szParam, pc->GetName() );
					SENDPACKETL( &sPacket );
				}
			}
		}

		dwTimeUpdate = TICKCOUNT + 5000;
	}
}

BOOL CQuestGame::OnQuestT5OpenProgress()
{
	if ( pcQuest->IsActiveQuest( QUESTID_T5QuestArena ) )
	{
		if ( pcQuest->IsFinishedQuestKills( QUESTID_T5QuestArena ) )
		{
			//Text
			if ( T5QUEST_ACTIVE == QUESTT5ACTIVE_Active )
			{
				T5QUEST_ACTIVE = QUESTT5ACTIVE_Finishing;

				GetWindow()->OpenT5QuestWindow( FALSE );
			}
			else
			{
				if ( T5QUEST_ACTIVE == QUESTT5ACTIVE_Finishing )
				{
					//Message Box Rankup
					MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
					MESSAGEBOX->SetTitle( "Rank Up" );
					MESSAGEBOX->SetDescription( "Do you wanna rank up?" );
					MESSAGEBOX->SetEvent( std::bind( &CQuestGame::OnMessageBoxT5Click, this, std::placeholders::_1 ) );
					MESSAGEBOX->Show();
				}
			}

			return TRUE;
		}
		else
		{
			//Message Box Rankup
			MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
			MESSAGEBOX->SetTitle( "Teleport" );
			MESSAGEBOX->SetDescription( "Do you want to teleport?" );
			MESSAGEBOX->SetEvent( std::bind( &CQuestGame::OnMessageBoxT5TeleportClick, this, std::placeholders::_1 ) );
			MESSAGEBOX->Show();
		}
	}

	return FALSE;
}

void CQuestGame::OnQuestT5Start()
{
	BOOL bCanGetQuest = FALSE;

	if ( pcQuest->IsDoneQuest( QUESTID_AMatterOfStrenght ) && pcQuest->IsDoneQuest( QUESTID_CryMeARiver ) && pcQuest->IsDoneQuest( QUESTID_TheLastStage ) )
		bCanGetQuest = TRUE;

	if ( bCanGetQuest )
	{
		WRITEDWORD( 0x0362BA7C, 0 );
		WRITEDWORD( 0x0367DE9C, 0 );

		//Window
		GetWindow()->OpenT5QuestWindow( TRUE );

		WRITEDWORD( 0x0367E8AC, 1 );
	}
	else
	{
		TITLEBOX( "You can't get this quest now!" );
	}
}

BOOL CQuestGame::OnMessageBoxT5Click( UIEventArgs e )
{
	T5QUEST_ACTIVE = QUESTT5ACTIVE_Active;
	
	if ( MESSAGEBOX->GetType() )
	{
		UNITDATA->sCharacterData.iRank = CHARACTERRANK_Rank5;

		UPDATE_CHARACTER_CHECKSUM;
		SAVE;


		GetWindow()->CloseT5QuestWindow();

		PacketQuestNPCFinish s;
		s.iID = QUESTID_T5QuestArena;
		CQUESTGAME->ProcessPacket( &s );

		T5QUEST_ACTIVE = QUESTT5ACTIVE_None;

		CHARACTERGAME->ResetHead();
		SKILLMANAGERHANDLER->SkillEffectLearn();
	}
	return TRUE;
}

BOOL CQuestGame::OnMessageBoxT5TeleportClick( UIEventArgs e )
{
	if ( MESSAGEBOX->GetType() )
	{
		PacketQuestArenaTier5 s;
		s.iLength = sizeof( PacketQuestArenaTier5 );
		s.iHeader = PKTHDR_T5QuestArena;
		s.iType = PacketQuestArenaTier5::EType::TYPEID_EnterArena;
		SENDPACKETG( &s );
	}
	return TRUE;
}

void CQuestGame::SkipOriginalQuests( BOOL bChatMessage )
{
	UINT uAddrBase = 0x362B5BC;
	typedef void( __cdecl *t_RecordLastQuest ) (UINT uCode);
	t_RecordLastQuest RecordLastQuest = (t_RecordLastQuest)0x004631C0;

	typedef void( __cdecl *t_SendPacket18 ) (UINT Socket, UINT Opcode, UINT Code, UINT Unk1, UINT Unk2, UINT Unk3);
	t_SendPacket18 SendPacket18 = (t_SendPacket18)0x00627720;

	typedef int( __thiscall *t_SearchItemCode ) (UINT MemoryPtr, UINT ItemID, UINT Unk);
	t_SearchItemCode SearchItemCode = (t_SearchItemCode)0x004AF8B0;

	typedef void( __cdecl *t_ReformCharStatePoint ) (UINT PlayerMemory, UINT Points);
	t_ReformCharStatePoint ReformCharStatePoint = (t_ReformCharStatePoint)0x00462900;

	typedef void( __cdecl *t_SetQuestBoard ) ();
	t_SetQuestBoard SetQuestBoard = (t_SetQuestBoard)0x0041A280;

	// For Her
	RecordLastQuest( 5 );
	if ( *(UINT*)0x3A97624 != 0 )
		SendPacket18( *(UINT*)0x3A97624, 0x50320220, 5, 0, 0, 0 );
	*(UINT*)(PLAYER_BACKUP + 0x3A74) += 5;
	*(UINT*)0x362BA68 = 1;
	*(UINT*)(uAddrBase + 0x0444) = 3;
	*(UINT*)(uAddrBase + 0x043C) = 5;
	*(UINT*)(uAddrBase + 0x0438) = 0;
	if ( bChatMessage )
		CHATGAMEHANDLER->AddChatBoxText( "> For Her Skipped!" );

	// The Cave
	RecordLastQuest( 6 );
	if ( *(UINT*)0x3A97624 != 0 )
		SendPacket18( *(UINT*)0x3A97624, 0x50320220, 6, 0, 0, 0 );
	*(UINT*)0x3673B70 += 1;
	*(UINT*)0x0362BA68 = 3;
	*(UINT*)(uAddrBase + 0x0110) = 3;
	*(UINT*)(uAddrBase + 0x0108) = 6;
	*(UINT*)(uAddrBase + 0x0104) = 0;
	if ( bChatMessage )
		CHATGAMEHANDLER->AddChatBoxText( "> The Cave Skipped!" );

	// Friendship Mitchell
	RecordLastQuest( 8 );
	if ( *(UINT*)0x3A97624 != 0 )
		SendPacket18( *(UINT*)0x3A97624, 0x50320220, 8, 0, 0, 0 );
	*(UINT*)0x3673B70 += 1;
	*(UINT*)(PLAYER_BACKUP + 0x3A74) += 5;
	*(UINT*)0x0362BA68 = 7;
	*(UINT*)(uAddrBase + 0x0084) = 3;
	*(UINT*)(uAddrBase + 0x007C) = 8;
	*(UINT*)(uAddrBase + 0x0078) = 0;
	if ( bChatMessage )
		CHATGAMEHANDLER->AddChatBoxText( "> Friendship Mitchell Skipped!" );

	// Sealed Fury
	RecordLastQuest( 9 );
	if ( *(UINT*)0x3A97624 != 0 )
		SendPacket18( *(UINT*)0x3A97624, 0x50320220, 9, 0, 0, 0 );
	*(UINT*)0x3673B70 += 2;
	*(UINT*)(PLAYER_BACKUP + 0x3A74) += 5;
	*(UINT*)0x0362BA68 = 15;
	*(UINT*)(uAddrBase + 0x0470) = 3;
	*(UINT*)(uAddrBase + 0x0468) = 9;
	*(UINT*)(uAddrBase + 0x0464) = 0;
	if ( bChatMessage )
		CHATGAMEHANDLER->AddChatBoxText( "> Sealed Fury Skipped!" );

	// Tears Of Kalia
	RecordLastQuest( 10 );
	if ( *(UINT*)0x3A97624 != 0 )
		SendPacket18( *(UINT*)0x3A97624, 0x50320220, 10, 0, 0, 0 );
	*(UINT*)0x0362BA68 = 0x5F;
	*(UINT*)(uAddrBase + 0x0068) = 4;
	*(UINT*)(uAddrBase + 0x0060) = 10;
	*(UINT*)(uAddrBase + 0x005C) = 0;
	if ( bChatMessage )
		CHATGAMEHANDLER->AddChatBoxText( "> Tears Of Kalia Skipped!" );

	// Eura Village
	RecordLastQuest( 11 );
	*(UINT*)0x0362BA68 = 0x7F;
	*(UINT*)(uAddrBase + 0x000C) = 3;
	*(UINT*)(uAddrBase + 0x0004) = 11;
	*(UINT*)(uAddrBase + 0x0000) = 0;
	if ( SearchItemCode( 0x35EBB20, 0x07010700, 0 ) == 0 )
	{
		if ( *(UINT*)0x3A97624 != 0 )
			SendPacket18( *(UINT*)0x3A97624, 0x50320220, 11, 0, 0, 0 );
	}
	if ( bChatMessage )
		CHATGAMEHANDLER->AddChatBoxText( "> Eura Village Skipped!" );

	WRITEDWORD( 0x0362BA2C, 4 );
/*
	// Test of Kingdom
	RecordLastQuest( 12 );
	*(UINT*)0x0362BA68 = 79;
	*(UINT*)(uAddrBase + 0x00F4) = 5;
	*(UINT*)(uAddrBase + 0x00EC) = 12;
	*(UINT*)(uAddrBase + 0x00E8) = 0;
	ReformCharStatePoint( *(UINT*)0x362A3B8, 79 );
	SetQuestBoard();
	if ( bChatMessage )
		CHATGAMEHANDLER->AddChatBoxText( "> Test of Kingdom Skipped!" );
*/
	// Bitter Ordeal
	RecordLastQuest( 14 );
	*(UINT*)0x0362BA68 = 255;
	*(UINT*)(uAddrBase + 0x00D8) = 4;
	*(UINT*)(uAddrBase + 0x00D0) = 13;
	*(UINT*)(uAddrBase + 0x00CC) = 0;
	ReformCharStatePoint( *(UINT*)0x362A3B8, 255 );
	SetQuestBoard();
	if ( bChatMessage )
		CHATGAMEHANDLER->AddChatBoxText( "> Bitter Ordeal Skipped!" );

	CHECK_CHARACTER_CHECKSUM;
	SAVE;
}

BOOL CQuestGame::CanGetItem( PacketItem * psPacket, int iPacketHeader )
{
	if ( (psPacket->sItem.sItemID.ToItemID() == ITEMID_MagicRune) && pcQuest->IsActiveQuest( QUESTID_GreenLiquidQuestStep1 ) )
		return TRUE;

	if ( (psPacket->sItem.sItemID.ToItemID() == ITEMID_PotionAgingQuestBee) && (pcQuest->IsActiveQuest( QUESTID_GreenLiquidQuestStep1 ) || pcQuest->IsDoneQuest( QUESTID_GreenLiquidQuestStep1 )) )
	{
		if ( pcQuest->IsDoneQuest( QUESTID_GreenLiquidQuestStep2 ) == FALSE )
			return TRUE;
	}

	if ( (psPacket->sItem.sItemID.ToItemID() == ITEMID_BlessOfKalia) && pcQuest->IsDoneQuest( QUESTID_GreenLiquidQuestStep2 ) )
	{
		if ( pcQuest->IsFinishedQuestItems( QUESTID_GreenLiquidQuestStep2 ) )
			return TRUE;
	}

	if ( (psPacket->sItem.sItemID.ToItemID() == ITEMID_GoldBag) && (pcQuest->IsActiveQuest( QUESTID_AnAncientThreat ) || pcQuest->IsDoneQuest( QUESTID_AnAncientThreat )) )
	{
		return TRUE;
	}
	return (BOOL)CALL_WITH_ARG2( 0x00624DA0, (DWORD)psPacket, (DWORD)iPacketHeader );
}

void CQuestGame::OnCharacterKilled()
{
	int iQuestID = QUESTID_TheLoyaltyTestStep2;
	QuestData * pc = CQUESTGAME->GetQuestInstance()->GetQuest( iQuestID );
	if ( pc )
	{
		ChatGame::AddChatBoxTextEx( CHATCOLOR_Error, "> %s cancelled!", pc->GetName() );
		HandleDeleteItem( iQuestID, FALSE );
		CQUESTGAME->ProcessQuestCancel( iQuestID );
		CQUESTGAME->GetQuestInstance()->DeleteQuest( iQuestID );
		TIMERSKILLHANDLER->KillTimerQuest( 0x096A0003, iQuestID );
		TIMERSKILLHANDLER->KillTimerQuest( 0x096B0003, iQuestID );
	}
}
