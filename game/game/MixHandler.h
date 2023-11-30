#pragma once

#define MIXWINDOW_OPEN			(*(BOOL*)0x03680334)

enum EMixMask
{
	MIXMASKID_Fire								= 0x00000001,	
	ADD_ICE										= 0x00000002,	
	ADD_LIGHTNING								= 0x00000004,	
	ADD_POISON									= 0x00000008,
	ADD_BIO										= 0x00000010,
	ADD_CRITICAL								= 0x00000020,	
	ADD_ATTACK_RATE								= 0x00000040,
	MIXMASKID_MinAP								= 0x00000080,
	MIXMASKID_MaxAP								= 0x00000100,
	ADD_ATTACK_SPEED							= 0x00000200,	
	ADD_ABSORB									= 0x00000400,
	ADD_DEFENCE									= 0x00000800,	
	ADD_BLOCK_RATE								= 0x00001000,	
	ADD_MOVE_SPEED								= 0x00002000,	
	ADD_LIFE									= 0x00004000,	
	ADD_MANA									= 0x00008000,	
	ADD_STAMINA									= 0x00010000,	
	ADD_LIFEREGEN								= 0x00020000, 
	ADD_MANAREGEN								= 0x00040000,
	ADD_STAMINAREGEN							= 0x00080000, 
};

enum EMixType
{
	MIXTYPEID_None,
	MIXTYPEID_DevineMix,
	MIXTYPEID_CelestoMix,
	MIXTYPEID_MirageMix,
	MIXTYPEID_InfernaMix,
};


class MixHandler
{
protected:
	int											iLineDescriptionColor[2];
	int											iDescriptionColor[2];

	const DWORD									pdwInventory	= 0x035EBB20;
	const DWORD									pdwDrillAmount	= 0x0362A1BC;
	const DWORD									pdwStrength		= pdwInventory + 0x39C4C;
	const DWORD									pdwSpirit		= pdwInventory + 0x39C58;
	const DWORD									pdwTalent		= pdwInventory + 0x39C54;
	const DWORD									pdwAgility		= pdwInventory + 0x39C5C;
	const DWORD									pdwHealth		= pdwInventory + 0x39C50;

private:
	BOOL										AddItemDescription( const char * pszText, int iColor, int iArray );

	BOOL										IsRespecOnlyItem( int iItemID );

	BOOL										CanUseSheltomSlot( int iItemID );

	BOOL										CanUseSocketItem( int iItemIDA, int iItemIDB, int iItemIDC );

	BOOL										IsSocketItem( int iItemID );

	

public:
	MixHandler();
	virtual ~MixHandler();

	void										HandlePacket( struct PacketMixServer * psPacket );

	BOOL										SetItemMix( ItemData * psItemData, BOOL bFlag );

	BOOL										CanPutStoneSlotSocket( ItemData * pcItemData, BOOL bGem );

	BOOL										CanPutItemSlotSocket( ItemData * pcItemData, BOOL bGemNPC );

	void										SetSocketStatus( EGemType eGemType, EGemItem eGemItem );
	void										OnHandlerCharacterUpdate();

};

