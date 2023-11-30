#pragma once

#include "CCaravanWindowHandler.h"

class CCaravanHandler
{
private:
	CCaravanWindowHandler			* pcWindow;

	CaravanBase						* pcCaravanBase;

public:
	CCaravanHandler();
	virtual ~CCaravanHandler();

	CCaravanWindowHandler			* GetWindow() { return pcWindow; }

	void							Init();

	void							OpenNPC();

	void							HandlePacket( PacketCaravanOpen * psPacket );
	void							HandlePacket( PacketCaravan * psPacket );
	void							HandlePacket( PacketCaravanUpdate * psPacket );

	void							SaveCaravan( std::vector<ItemData*> vItems, const std::string strName, bool bFollow );

	ItemTimer						* GetCaravanItemTimer();

	void							UpdateCaravanParticle( Unit * pcUnit );

private:

	void							Receive( PacketCaravan * psPacket );

	void							ProcessPacket( PacketCaravan * psPacket );
	void							ProcessPacket( PacketCaravanOpen * psPacket );
	void							ProcessPacket( PacketCaravanUpdate * psPacket );
};

