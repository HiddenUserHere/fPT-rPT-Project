#include "stdafx.h"
#include "CServerHandler.h"

#include "CServer.h"

CServerHandler::CServerHandler()
{
	pcFuryArenaHandler			= new CFuryArenaHandler();
	pcBlessCastleHandler		= new CBlessCastleHandler();
	pcHalloweenHandler			= new CHalloweenHandler();
	pcChristmasHandler			= new CChristmasHandler();
	pcBotServerHandler			= new CBotServerHandler();
	pcCoinShopHandler			= new CCoinShopHandler();
	pcEventServerHandler		= new CEventServer();
	pcTradeHandler				= new CTradeHandler();
	pcDamageHandler				= new CDamageHandler();
	pcAgeHandler				= new CAgeHandler();
	pcMixHandler				= new CMixHandler();
	pcQuestArenaHandler			= new CQuestArenaHandler();
	pcPartyHandler				= new CPartyHandler();
	pcRollDiceHandler			= new CRollDiceHandler();
	pcWarehouseHandler			= new CWarehouseHandler();
	pcManufactureHandler		= new CManufactureHandler();
	pcAchievementHandler		= new CAchievementHandler();
	pcReconnectServerHandler	= new CReconnectServerHandler();
	pcRoomInstanceHandler		= new CRoomInstanceHandler();
	pcActionFieldHandler		= new CActionFieldHandler();
	pcFollowUnitHandler			= new CFollowUnitHandler();
	pcPersonalShopHandler		= new CPersonalShopHandler();
	pcChristmasTreeHandler		= new CChristmasTreeHandler();
	pcMonsterHordeHandler		= new CMonsterHordeHandler();
	pcServerEventHandler		= new CServerEventHandler();
	pcBattleRoyaleHandler		= new CBattleRoyaleHandler();
	pcBellatraServerHandler		= new CBellatraServerHandler();
	pcGlamorousBrushHandler		= new CGlamorousBrushHandler();
	pcDropItemAreaHandler		= new CDropItemAreaHandler();
	pcHonorHandler				= new CHonorHandler();
	pcTimePointsHandler			= new CTimePointsHandler();
	pcRankingServerHandler		= new CRankingServerHandler();
	pcMountServerHandler		= new CMountServerHandler();
}

CServerHandler::~CServerHandler()
{
	DELET( pcFuryArenaHandler );
	DELET( pcBlessCastleHandler );
	DELET( pcHalloweenHandler );
	DELET( pcChristmasHandler );
	DELET( pcBotServerHandler );
	DELET( pcCoinShopHandler );
	DELET( pcEventServerHandler );
	DELET( pcTradeHandler );
	DELET( pcDamageHandler );
	DELET( pcAgeHandler );
	DELET( pcMixHandler );
	DELET( pcQuestArenaHandler );
	DELET( pcPartyHandler );
	DELET( pcRollDiceHandler );
	DELET( pcWarehouseHandler );
	DELET( pcManufactureHandler );
	DELET( pcAchievementHandler );
	DELET( pcReconnectServerHandler );
	DELET( pcRoomInstanceHandler );
	DELET( pcActionFieldHandler );
	DELET( pcFollowUnitHandler );
	DELET( pcPersonalShopHandler );
	DELET( pcChristmasTreeHandler );
	DELET( pcMonsterHordeHandler );
	DELET( pcServerEventHandler );
	DELET( pcBattleRoyaleHandler );
	DELET( pcBellatraServerHandler );
	DELET( pcGlamorousBrushHandler );
	DELET( pcDropItemAreaHandler );
	DELET( pcHonorHandler );
	DELET( pcTimePointsHandler );
	DELET( pcRankingServerHandler );
	DELET( pcMountServerHandler );
}

BOOL CServerHandler::Init()
{
	pcCoinShopHandler->Init();
	pcFuryArenaHandler->Init();
	pcAchievementHandler->Init();
	pcReconnectServerHandler->Init();
	pcRoomInstanceHandler->Init();
	pcActionFieldHandler->Init();
	pcChristmasTreeHandler->Init();
	pcMonsterHordeHandler->Init();
	pcBattleRoyaleHandler->Init();
	pcBellatraServerHandler->Init();
	pcDropItemAreaHandler->Init();
	pcRankingServerHandler->Init();
	pcMountServerHandler->Init();

	//Add Handlers After
	pcServerEventHandler->AddHandler( pcBattleRoyaleHandler );
	pcServerEventHandler->AddHandler( pcBellatraServerHandler );

	CSkillServerHandler::Init();

	return TRUE;
}

void CServerHandler::Shutdown()
{
	pcAchievementHandler->Shutdown();
}

void CServerHandler::Update( ETickType iTickType )
{
	switch ( iTickType )
	{
		case TICKTYPE_None:
			pcQuestArenaHandler->Update();
			pcEventServerHandler->Tick();
			break;
		case TICKTYPE_100MilliSecond:
			break;
		case TICKTYPE_500MilliSecond:
			break;
		case TICKTYPE_1Second:
			pcRoomInstanceHandler->Update();
			pcActionFieldHandler->Update();
			pcMonsterHordeHandler->Update();
			pcChristmasTreeHandler->Update();
			pcHonorHandler->Update( iTickType );
			pcTimePointsHandler->Update( iTickType );
			break;
		case TICKTYPE_1Minute:
			pcRankingServerHandler->Update( iTickType );
			break;
		case TICKTYPE_1Hour:
			break;
		default:
			break;
	}

	pcServerEventHandler->Update( iTickType );
}
