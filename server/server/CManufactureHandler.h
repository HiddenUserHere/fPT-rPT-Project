#pragma once
class CManufactureHandler
{
public:
	CManufactureHandler();
	virtual ~CManufactureHandler();

	void					HandlePacket( User * pcUser, PacketManufactureItem * psPacket );

	int						GetHalloweenCombination() { return iHalloweenCombinationID; }
	void					SetHalloweenCombination( int iID );

	void					RandomizeCombinations();

private:
	BOOL					DeleteItem( User * pcUser, EItemID iItemID, int iChk1, int iChk2 );
	BOOL					DeleteRunes( User * pcUser, EItemID * piaItemID, int * piaChk1, int * piaChk2 );

	BOOL					AddItem( User * pcUser, EItemID iItemID, int iChk1, int iChk2 );
	BOOL					AddItem( User * pcUser, Item * psItem );

	BOOL					HaveItem( PacketManufactureItem * psPacket, EItemID iItemRecipeID, bool bUniqueSlot, EItemID iItemSlot1ID, int iCount1, EItemID iItemSlot2ID = ITEMID_None, int iCount2 = 0, EItemID iItemSlot3ID = ITEMID_None, int iCount3 = 0 );

	BOOL					SwapItem( User * pcUser, PacketManufactureItem * psPacket, EItemID iNewItemID, BOOL bSameSpec, BOOL bPerfect );

	EItemID					iaHalloweenEvilID[3];
	EItemID					iaHalloweenInfernalID[3];
	int						iHalloweenCombinationID = 1;

};

