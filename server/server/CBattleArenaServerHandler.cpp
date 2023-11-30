#include "stdafx.h"
#include "CBattleArenaServerHandler.h"


CBattleArenaServerHandler::CBattleArenaServerHandler()
{
	if ( GAME_SERVER )
		pcBattleArena = new CBattleArena();
}


CBattleArenaServerHandler::~CBattleArenaServerHandler()
{
	if ( GAME_SERVER )
		DELET( pcBattleArena );
}

void CBattleArenaServerHandler::Init()
{

}

void CBattleArenaServerHandler::Update()
{
}

void CBattleArenaServerHandler::OnUserKilled( User * pcKiller, User * pcVictim )
{
}

void CBattleArenaServerHandler::HandlePacket( User * pcUser, PacketBattleArenaHost * psPacket )
{
}
