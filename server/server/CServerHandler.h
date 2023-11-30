#pragma once

#include "FuryArenaHandler.h"
#include "BlessCastleHandler.h"
#include "HalloweenHandler.h"
#include "BotServerHandler.h"
#include "CoinShopHandler.h"
#include "eventserver.h"
#include "ChristmasHandler.h"
#include "TradeHandler.h"
#include "DamageHandler.h"
#include "AgeHandler.h"
#include "MixHandler.h"
#include "QuestArenaHandler.h"
#include "CPartyHandler.h"
#include "CRollDiceHandler.h"
#include "CWarehouseHandler.h"
#include "CManufactureHandler.h"
#include "CAchievementHandler.h"
#include "CReconnectServerHandler.h"
#include "CRoomInstanceHandler.h"
#include "CActionFieldHandler.h"
#include "CFollowUnitHandler.h"
#include "CPersonalShopHandler.h"
#include "CChristmasTreeHandler.h"
#include "CCharacterDataHandler.h"
#include "CMonsterHordeHandler.h"
#include "CServerEventHandler.h"
#include "CBattleRoyaleHandler.h"
#include "CBellatraServerHandler.h"
#include "CGlamorousBrushHandler.h"
#include "CDropItemAreaHandler.h"

#include "CSkillServerHandler.h"
#include "CHonorHandler.h"
#include "CTimePointsHandler.h"
#include "CRankingServerHandler.h"
#include "CMountServerHandler.h"

#define FURYARENAHANDLER				(SERVERHANDLER->GetFuryArenaHandler())
#define BLESSCASTLEHANDLER				(SERVERHANDLER->GetBlessCastleHandler())
#define HALLOWEENHANDLER				(SERVERHANDLER->GetHalloweenHandler())
#define XMASHANDLER						(SERVERHANDLER->GetChristmasHandler())
#define BOTSERVER						(SERVERHANDLER->GetBotServerHandler())
#define COINSHOPHANDLER					(SERVERHANDLER->GetCoinShopHandler())
#define EVENTSERVER						(SERVERHANDLER->GetEventServerHandler())
#define TRADEHANDLER					(SERVERHANDLER->GetTradeHandler())
#define DAMAGEHANDLER					(SERVERHANDLER->GetDamageHandler())
#define AGEHANDLER						(SERVERHANDLER->GetAgeHandler())
#define MIXHANDLER						(SERVERHANDLER->GetMixHandler())
#define QUESTARENAHANDLER				(SERVERHANDLER->GetQuestArenaHandler())
#define PARTYHANDLER					(SERVERHANDLER->GetPartyHandler())
#define ROLLDICEHANDLER					(SERVERHANDLER->GetRollDiceHandler())
#define WAREHOUSEHANDLER				(SERVERHANDLER->GetWarehouseHandler())
#define MANUFACTUREHANDLER				(SERVERHANDLER->GetManufactureHandler())
#define ACHIEVEMENTHANDLER				(SERVERHANDLER->GetAchievementHandler())
#define RECONNECTSERVERHANDLER			(SERVERHANDLER->GetReconnectServerHandler())
#define ROOMINSTANCEHANDLER				(SERVERHANDLER->GetRoomInstanceHandler())
#define ACTIONFIELDHANDLER				(SERVERHANDLER->GetActionFieldHandler())
#define FOLLOWUNITHANDLER				(SERVERHANDLER->GetFollowUnitHandler())
#define PERSONALSHOPHANDLER				(SERVERHANDLER->GetPersonalShopHandler())
#define XMASTREEHANDLER					(SERVERHANDLER->GetChristmasTreeHandler())
#define MONSTERHORDEHANDLER				(SERVERHANDLER->GetMonsterHordeHandler())
#define SERVEREVENTHANDLER				(SERVERHANDLER->GetServerEventHandler())
#define BATTLEROYALEHANDLER				(SERVERHANDLER->GetBattleRoyaleHandler())
#define BELLATRASERVERHANDLER			(SERVERHANDLER->GetBellatraServerHandler())
#define GLAMOROUSBRUSHHANDLER			(SERVERHANDLER->GetGlamorousBrushHandler())
#define DROPAREAHANDLER					(SERVERHANDLER->GetDropItemAreaHandler())
#define HONORHANDLER					(SERVERHANDLER->GetHonorHandler())
#define TIMEPOINTSHANDLER				(SERVERHANDLER->GetTimePointsHandler())
#define RANKINGHANDLER					(SERVERHANDLER->GetRankingServerHandler())
#define MOUNTSERVERHANDLER				(SERVERHANDLER->GetMountServerHandler())

class CServerHandler
{
private:
	CFuryArenaHandler			* pcFuryArenaHandler;
	CBlessCastleHandler			* pcBlessCastleHandler;
	CHalloweenHandler			* pcHalloweenHandler;
	CChristmasHandler			* pcChristmasHandler;
	CBotServerHandler			* pcBotServerHandler;
	CCoinShopHandler			* pcCoinShopHandler;
	CEventServer				* pcEventServerHandler;
	CTradeHandler				* pcTradeHandler;
	CDamageHandler				* pcDamageHandler;
	CAgeHandler					* pcAgeHandler;
	CMixHandler					* pcMixHandler;
	CQuestArenaHandler			* pcQuestArenaHandler;
	CPartyHandler				* pcPartyHandler;
	CRollDiceHandler			* pcRollDiceHandler;
	CWarehouseHandler			* pcWarehouseHandler;
	CManufactureHandler			* pcManufactureHandler;
	CAchievementHandler			* pcAchievementHandler;
	CReconnectServerHandler		* pcReconnectServerHandler;
	CRoomInstanceHandler		* pcRoomInstanceHandler;
	CActionFieldHandler			* pcActionFieldHandler;
	CFollowUnitHandler			* pcFollowUnitHandler;
	CPersonalShopHandler		* pcPersonalShopHandler;
	CChristmasTreeHandler		* pcChristmasTreeHandler;
	CMonsterHordeHandler		* pcMonsterHordeHandler;
	CServerEventHandler			* pcServerEventHandler;
	CBattleRoyaleHandler		* pcBattleRoyaleHandler;
	CBellatraServerHandler		* pcBellatraServerHandler;
	CGlamorousBrushHandler		* pcGlamorousBrushHandler;
	CDropItemAreaHandler		* pcDropItemAreaHandler;
	CHonorHandler				* pcHonorHandler;
	CTimePointsHandler			* pcTimePointsHandler;
	CRankingServerHandler		* pcRankingServerHandler;
	CMountServerHandler			* pcMountServerHandler;


public:
						  CServerHandler();
	virtual				 ~CServerHandler();

	BOOL				  Init();
	void				  Shutdown();

	void				  Update( ETickType iTickType );

	CFuryArenaHandler			* GetFuryArenaHandler() { return pcFuryArenaHandler; }
	CBlessCastleHandler			* GetBlessCastleHandler() { return pcBlessCastleHandler; }
	CHalloweenHandler			* GetHalloweenHandler() { return pcHalloweenHandler; }
	CChristmasHandler			* GetChristmasHandler() { return pcChristmasHandler; }
	CBotServerHandler			* GetBotServerHandler() { return pcBotServerHandler; }
	CCoinShopHandler			* GetCoinShopHandler() { return pcCoinShopHandler; }
	CEventServer				* GetEventServerHandler() { return pcEventServerHandler; }
	CTradeHandler				* GetTradeHandler() { return pcTradeHandler; }
	CDamageHandler				* GetDamageHandler() { return pcDamageHandler; }
	CAgeHandler					* GetAgeHandler() { return pcAgeHandler; }
	CMixHandler					* GetMixHandler() { return pcMixHandler; }
	CQuestArenaHandler			* GetQuestArenaHandler() { return pcQuestArenaHandler; }
	CPartyHandler				* GetPartyHandler() { return pcPartyHandler; }
	CRollDiceHandler			* GetRollDiceHandler() { return pcRollDiceHandler; }
	CWarehouseHandler			* GetWarehouseHandler() { return pcWarehouseHandler; }
	CManufactureHandler			* GetManufactureHandler() { return pcManufactureHandler; }
	CAchievementHandler			* GetAchievementHandler() { return pcAchievementHandler; }
	CReconnectServerHandler		* GetReconnectServerHandler() { return pcReconnectServerHandler; }
	CRoomInstanceHandler		* GetRoomInstanceHandler() { return pcRoomInstanceHandler; }
	CActionFieldHandler			* GetActionFieldHandler() { return pcActionFieldHandler; }
	CFollowUnitHandler			* GetFollowUnitHandler() { return pcFollowUnitHandler; }
	CPersonalShopHandler		* GetPersonalShopHandler() { return pcPersonalShopHandler; }
	CChristmasTreeHandler		* GetChristmasTreeHandler() { return pcChristmasTreeHandler; }
	CMonsterHordeHandler		* GetMonsterHordeHandler() { return pcMonsterHordeHandler; }
	CServerEventHandler			* GetServerEventHandler() { return pcServerEventHandler; }
	CBattleRoyaleHandler		* GetBattleRoyaleHandler() { return pcBattleRoyaleHandler; }
	CBellatraServerHandler		* GetBellatraServerHandler() { return pcBellatraServerHandler; }
	CGlamorousBrushHandler		* GetGlamorousBrushHandler() { return pcGlamorousBrushHandler; }
	CDropItemAreaHandler		* GetDropItemAreaHandler() { return pcDropItemAreaHandler; }
	CHonorHandler				* GetHonorHandler() { return pcHonorHandler; }
	CTimePointsHandler			* GetTimePointsHandler() { return pcTimePointsHandler; }
	CRankingServerHandler		* GetRankingServerHandler() { return pcRankingServerHandler; }
	CMountServerHandler			* GetMountServerHandler() { return pcMountServerHandler; }

private:


};