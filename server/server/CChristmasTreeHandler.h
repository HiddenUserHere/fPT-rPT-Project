#pragma once

static const int CRC_XMASTREE_ITEMCHECKSUM	=	0x6A5B0000;

class CChristmasTreeHandler
{
private:
	struct ChristmasTreeSQL
	{
		char		szName[64];
		int			iCharacterID;
		EMapID		iMapID;
		int			iX;
		int			iZ;
		BOOL		bNormalTree;
	};

	struct ChristmasItemData
	{
		int			iCharacterSenderID;
		ItemData	cItemData;
	};

	WORD					wLastHour = 26;

public:
	CChristmasTreeHandler();
	virtual ~CChristmasTreeHandler();

	bool					SpawnTree( int iCharacterID, const char * pszCharacterName, EMapID iMapID, int iX, int iZ, BOOL bNormalTree );

	void					Init();

	void					Update();

	//TODO: Check if a tree has received a gift by an account
	//Add limit for trees?

	void					OpenTree( User * pcUser, Unit * pcUnit );
	void					OpenGiftNPC( User * pcUser, Unit * pcUnit );

	void					HandlePacket( User * pcUser, PacketXmasTree * psPacket );
	void					HandlePacket( User * pcUser, PacketXmasTreeItemNPC * psPacket );
	void					HandlePacket( User * pcUser, PacketXmasTreeItemUse * psPacket );

private:

	void					Receive( User * pcUser, PacketXmasTree * psPacket );

	bool					UpdateXmasTreeItems( User * pcUser, int iCharacterID );

	void					SaveItems( User * pcUser, int iCharacterID );

	void					RemoveTree( Unit * pcUnit );
	void					RemoveTree( int iCharacterID );
	void					RemoveAllTrees();

	bool					SQLHaveTree( int iCharacterID );
	void					SQLNewTree( int iCharacterID, UnitData * pcUnitData, BOOL bNormalTree );

	void					SQLDeleteTree( int iCharacterID );

	bool					SQLGetItemTree( int iCharacterID, int iItemID, int iChk1, int iChk2, ItemData & cItemData );
	bool					SQLGetItemsTree( int iCharacterID, std::vector<ChristmasItemData*> & vItems );
	void					SQLDeleteItemTree( int iCharacterID, int iItemID, int iChk1, int iChk2 );

	bool					SQLUserHasSentGift( int iCharacterReceiverID, int iCharacterSenderID );
	
	void					SQLUpdateGiftStatus( BOOL bIsAvailable );

	void					SendNextItem( User * pcUser );

	void					ProcessPacket( User * pcUser, PacketXmasTreeOpen * psPacket );
	void					ProcessPacket( User * pcUser, PacketXmasTreeItemNPC * psPacket );

	void					ProcessPacket( User * pcUser, PacketXmasTreeResponse * psPacket );

	bool					IsNearNPC( Map * pcMap, int iX, int iZ );

	void					UpdateMap( Unit * pcUnit, EMapID iMapID, Point3D sPosition );
};

