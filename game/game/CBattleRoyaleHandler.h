#pragma once

#include "CBattleRoyaleWindowHandler.h"


class CBattleRoyaleHandler
{
private:
	CBattleRoyaleWindowHandler * pcWindow = nullptr;

	UI::Image					* pcImageArrow = nullptr;

	int								iSpawnCoordinatesCount;
	BattleRoyale::SpawnCoordinate	saCoordinates[MAX_BATTLEROYALE_SPAWNCOORDS];

	enum
	{
		WINDOW_Main,
		TEXT_GasTime,
		TEXT_PlayersCount,
		TEXT_State,
		TEXT_Team,
		TEXT_Countdown,
		TEXT_TimeRevive,

		TEXT_TeamGreenKills,
		TEXT_TeamWhiteKills,
	};

public:
	CBattleRoyaleHandler();
	virtual ~CBattleRoyaleHandler();

	UI::Window_ptr			GetWindowMinimapHUD() { return pWindow; }

	CBattleRoyaleWindowHandler * GetWindow() { return pcWindow; }

	void					Init();

	void					HandlePacket( PacketBattleRoyaleUpdate * psPacket );
	void					HandlePacket( PacketBattleRoyaleBattleTime * psPacket );
	void					HandlePacket( PacketBattleRoyaleList * psPacket );
	void					HandlePacket( PacketBattleRoyaleRewardList * psPacket );
	void					HandlePacket( PacketBattleRoyaleResponse * psPacket );
	void					HandlePacket( PacketBattleRoyaleTeleports * psPacket );

	void					ProcessPacket( PacketBattleRoyaleRequest * psPacket );

	BattleRoyale::State		& GetState() { return sState; }

	BOOL					IsStarted() { return bStart; }
	BOOL					InBattle() { return IsStarted() && (sState.eGameState == BattleRoyale::EGameState::GAMESTATE_InBattle) && (sState.iRadius > 0); }
	BOOL					InWaiting() { return bWaitingTime; }
	BOOL					InRoyale() { return InBattle() || (sState.eGameState == BattleRoyale::EGameState::GAMESTATE_End); }

	BOOL					IsInsideRadius( Point3D sPosition );
	BOOL					IsInsideNextRadius( Point3D sPosition );
	BOOL					IsInsideMeteorite( Point3D sPosition );

	void					Update( float fTime );

	void					OnResolutionChanged();

	void					Render();
	void					Render3D();

	void					RenderUnitArrow( UnitData * pcUnitData );

	void					Test();

	void					OnDie();

	bool					IsEnemy( UnitData * pcUnitData );

	BattleRoyale::EPlayerTeam GetTeam() { return eTeamID; }

	bool					OnRespawn( Point3D & sPosition );

private:
	BOOL					bStart = FALSE;
	BOOL					bWaitingTime = FALSE;

	BattleRoyale::State		sState;

	BattleRoyale::EPlayerTeam eTeamID;

	float					fTimeUpdateDecreaseHP = 0.0f;

	UI::Window_ptr			pWindow = nullptr;
	UI::Window_ptr			pWindowTop = nullptr;

	EXEModel				* pcModelSmoke = nullptr;

	ItemData				* pcaItemData = nullptr;
	InventoryItemData		* psaInventorySlot = nullptr;

	ItemData				* pcUnknownItem = nullptr;
	
	char					szBodyModel[64];

	ItemID					saItemID[3];

	float					fTimeCharacter = 0.0f;

	float					fTimeToRevive = 0.0f;

	EXEModel				* pcaCrowns[2];
};

