#pragma once

#include "user.h"
#include "character.h"
#include "item.h"

#define MAX_BATTLEARENA_PLAYERS			60
#define MAX_BATTLEARENA_GROUPS			3

namespace BattleArena
{

enum EGameState : int
{
	GAMESTATE_None,
	GAMESTATE_WaitingForPlayers,
};

enum EGroup : int
{
	GROUP_Red,
	GROUP_Green,
	GROUP_Blue,
};

struct State
{
	EGameState			eGameState;

	int					iaPlayers[MAX_BATTLEARENA_GROUPS];

	int					iaPlayerLimit[MAX_BATTLEARENA_GROUPS];
};

enum EPlayerState : int
{
	PLAYERSTATE_None,
	PLAYERSTATE_Alive,
	PLAYERSTATE_Dead,
	PLAYERSTATE_Disconnected,
};

struct PlayerState
{
	int					lID;

	User				* pcUser;

	EPlayerState		ePlayerState;

	EGroup				eGroup;

	char				szName[32];
	int					iClass;
	int					iLevel;

	int					iLives;

	int					iKills;
	int					iDeaths;

	Point3D				sPosition;

	int					iTimerDeath;
};

static BOOL IsValidGroup( EGroup eGroup )
{
	if ( (eGroup >= EGroup::GROUP_Red) && (eGroup <= EGroup::GROUP_Blue) )
		return TRUE;

	return FALSE;
};

};

struct PacketBattleArenaHost : Packet
{

};

class CBattleArena
{
private:
	BattleArena::State			sState;

	BattleArena::PlayerState	saPlayerState[MAX_BATTLEARENA_PLAYERS];

public:

	CBattleArena();
	~CBattleArena();

	BattleArena::State			& GetState() { return sState; }
	BattleArena::PlayerState	* GetPlayerStates() { return saPlayerState; }

	BattleArena::PlayerState	* GetPlayerState( int iIndex );
	BattleArena::PlayerState	* GetPlayerStateByID( int iID );
};
