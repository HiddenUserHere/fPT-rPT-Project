#pragma once

class CBattleArenaServerHandler
{
public:
	CBattleArenaServerHandler();
	virtual ~CBattleArenaServerHandler();

	void						Init();

	void						Update();

	void						OnUserKilled( User * pcKiller, User * pcVictim );

	void						HandlePacket( User * pcUser, PacketBattleArenaHost * psPacket );

private:
	CBattleArena				* pcBattleArena = nullptr;
};

