#include "stdafx.h"
#include "AccountHandler.h"


CAccountHandler::CAccountHandler()
{
	iCoinAmount = 0;
}


CAccountHandler::~CAccountHandler()
{
}

void CAccountHandler::HandlePacket( PacketServerList * psPacket )
{
	SetAccountShare( psPacket->eAccountShare );
}
