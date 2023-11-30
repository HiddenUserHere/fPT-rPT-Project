#pragma once

#include "CItemBoxWindowHandler.h"

class CItemBoxHandler
{
public:
	CItemBoxHandler();
	virtual ~CItemBoxHandler();

	CItemBoxWindowHandler			* GetWindow() { return pcWindow; };

	void							Init();

	void							HandlePacket( PacketNPCItemBox * psPacket );
	void							HandlePacket( PacketNPCItemBoxSendResponse * psPacket );

private:
	CItemBoxWindowHandler			* pcWindow = nullptr;

private:
	friend class CItemBoxWindowHandler;

	void							ProcessPacket( PacketNPCItemBox * psPacket );
	void							ProcessPacket( PacketNPCItemBoxSend * psPacket );
};

