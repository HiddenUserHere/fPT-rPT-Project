#pragma once

#include "user.h"
#include "character.h"
#include "item.h"

#define MAX_CARAVAN_ITEMS		81
#define MAX_CARAVAN_BUFFERSIZE	0x1FEC
#define MAX_CARAVAN_DATASIZE	((sizeof( ItemData ) * MAX_CARAVAN_ITEMS) + (sizeof( Packet ) * 10))

struct CaravanData
{
	int					iItemCount;
	ItemData			caItemData[MAX_CARAVAN_ITEMS];
};


struct PacketCaravan : Packet
{
	MinMax				sPacketCount;

	int					iItemCount;

	UINT				uBufferCount;
	BYTE				baBuffer[MAX_CARAVAN_BUFFERSIZE];

	bool				IsFirst() { return sPacketCount.sMin == 1 ? true : false; }
	bool				IsOnly() { return sPacketCount.sMin == sPacketCount.sMax ? true : false; }
};

struct PacketCaravanOpen : Packet
{
	int					iUnk001;
};

struct PacketCaravanUpdate : Packet
{
	char				szName[32];

	bool				bFollow;
};

class CaravanBase
{
public:
	UINT				uBufferCount;
	BYTE				baBuffer[MAX_CARAVAN_DATASIZE];

	CaravanData			* psCaravanData = nullptr;
};