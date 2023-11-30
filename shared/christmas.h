#pragma once

#include "user.h"
#include "character.h"
#include "item.h"

#define MAX_XMASTREE_ITEMS		25
#define MAX_XMASTREE_BUFFERSIZE	0x1FE8
#define MAX_XMASTREE_DATASIZE	((sizeof( ItemData ) * MAX_XMASTREE_ITEMS) + (sizeof( Packet ) * 10))

struct XmasTreeData
{
	int					iItemCount;
	ItemData			caItemData[MAX_XMASTREE_ITEMS];
};


struct PacketXmasTree : Packet
{
	int					iCharacterID;

	MinMax				sPacketCount;

	int					iItemCount;

	UINT				uBufferCount;
	BYTE				baBuffer[MAX_XMASTREE_BUFFERSIZE];

	bool				IsFirst() { return sPacketCount.sMin == 1 ? true : false; }
	bool				IsOnly() { return sPacketCount.sMin == sPacketCount.sMax ? true : false; }
};

struct PacketXmasTreeOpen : Packet
{
	PacketXmasTreeOpen( int _iUnitID, const char * pszCharacterName )
	{
		iUnitID = _iUnitID;
		STRINGCOPY( szCharacterNameOwner, pszCharacterName );
	};

	~PacketXmasTreeOpen() {};

	int					iUnitID;
	char				szCharacterNameOwner[32];
};

class XmasTreeBase
{
public:
	UINT				uBufferCount;
	BYTE				baBuffer[MAX_XMASTREE_DATASIZE];

	XmasTreeData		* psXmasTreeData = nullptr;
};

struct PacketXmasTreeResponse : Packet
{
	PacketXmasTreeResponse( int i ) { iResult = i; };
	~PacketXmasTreeResponse() {};

	int					iResult;
};

struct PacketXmasTreeItemNPC : Packet
{
	BOOL				bItem;
	int					iItemCount;
};

struct PacketXmasTreeItemUse : Packet
{
	PacketXmasTreeItemUse( int _iItemID, int _iChk1, int _iChk2 )
	{
		iItemID = _iItemID;
		iChk1 = _iChk1;
		iChk2 = _iChk2;
	};

	~PacketXmasTreeItemUse() {};

	int					iItemID;
	int					iChk1;
	int					iChk2;
};