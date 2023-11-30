#include "stdafx.h"
#include "battlearena.h"

namespace BattleArena
{

};

CBattleArena::CBattleArena()
{
	sState.eGameState				= BattleArena::GAMESTATE_None;

	for ( auto & sPlayerState : saPlayerState )
	{
		sPlayerState.ePlayerState	= BattleArena::PLAYERSTATE_None;
		sPlayerState.lID			= 0;
		sPlayerState.pcUser			= nullptr;
	}

	ZeroMemory( sState.iaPlayers, sizeof( int ) * _countof( sState.iaPlayers ) );
	ZeroMemory( sState.iaPlayerLimit, sizeof( int ) * _countof( sState.iaPlayerLimit ) );
}

CBattleArena::~CBattleArena()
{

}

BattleArena::PlayerState * CBattleArena::GetPlayerState( int iIndex )
{
	if ( (iIndex >= 0) && (iIndex < MAX_BATTLEARENA_PLAYERS) )
		return saPlayerState + iIndex;

	return nullptr;
}

BattleArena::PlayerState * CBattleArena::GetPlayerStateByID( int iID )
{
	for ( auto & sPlayerState : saPlayerState )
		if ( (sPlayerState.lID != 0) && (sPlayerState.lID == iID) )
			return &sPlayerState;

	return nullptr;
}
