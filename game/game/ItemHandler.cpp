#include "stdafx.h"
#include "ItemHandler.h"

#include "itemhandlerstruct.h"

typedef int( __cdecl *t_LoadArmorChr ) ( const char * ArmorFileInx );
t_LoadArmorChr LoadArmorCharacter = ( t_LoadArmorChr )0x00457BA0;

typedef int( __cdecl *tfnSetItemCharacter )(EItemID, DWORD dwPositionModel, BOOL bActive);
tfnSetItemCharacter fnSetItemCharacter = (tfnSetItemCharacter)0x00458FF0;

typedef BOOL( __cdecl *tfnLoadItemImage )(Item * psIn, ItemData * psOut);
tfnLoadItemImage fnLoadItemImage = (tfnLoadItemImage)0x00505D10;

ItemData *						pItemTable = NULL;

int								iRollDiceTimerID = NULL;

DWORD						g_dwDelayCristal = 0;

extern void * allocIT;

// Pivot Table
static UINT PivotTableItemChr[]
{
	//Set 110
		0x01011600, 0x01011500, // Zecram Axe
		0x01021500, 0x01021400, // Injustice Talon
		0x01031700, 0x01031600, // Bane Hammer
		0x01041700, 0x01041300, // Celestial Staff
		0x01051800, 0x01051700, // Reaper Scythe
		0x01061800, 0x01061700, // Immortal Bow
		0x01071900, 0x01071800, // Extreme Sword
		0x01081700, 0x01081600, // Heretic Javelin

		//Set 115
		0x01011700, 0x01011500, // Dragon Axe
		0x01021700, 0x01021400, // Heretic Talon
		0x01031800, 0x01031600, // Dragon Hammer
		0x01041800, 0x01041300, // Elf Staff
		0x01051900, 0x01051700, // Dragon Spear
		0x01061900, 0x01061700, // Salamander Bow
		0x01071A00, 0x01071800, // Mirage Sword
		0x01081900, 0x01081600, // Salamander Javelin
		//Set 120
		0x01011800, 0x01011500, // Kronus Axe
		0x01011900, 0x01010100, // Legend Axe
		0x01021800, 0x01021400, // Kronus Talon
		0x01031900, 0x01031600, // Kronus Hammer
		0x01041900, 0x01041300, // Kronus Staff
		0x01042A00, 0x01040100, // Legend Wand
		0x01051A00, 0x01051700, // Kronus Spear
		0x01062A00, 0x01061700, // Kronus Bow
		0x01072B00, 0x01071800, // Kronus Sword
		0x01072C00, 0x01070100, // Legend Sword
		0x01081A00, 0x01081600, // Kronus Javelin

		//Set 125 Mythology
		ITEMID_MythologyAxe, 0x01011500, 
		ITEMID_MythologyTalon, 0x01021400,
		ITEMID_MythologyHammer, 0x01031600, 
		ITEMID_MythologyStaff, 0x01041300, 
		ITEMID_MythologySpear, 0x01051700, 
		ITEMID_MythologyBow, 0x01061700, 
		ITEMID_MythologySword, 0x01071800, 
		ITEMID_MythologyJavelin, 0x01081600, 

		//Set 128 Death
		ITEMID_DeathAxe, 0x01011500, 
		ITEMID_DeathTalon, 0x01021400,
		ITEMID_DeathHammer, 0x01031600, 
		ITEMID_DeathStaff, 0x01041300, 
		ITEMID_DiabolicScythe, 0x01051700, 
		ITEMID_DeathBow, 0x01061700, 
		ITEMID_DeathSword, 0x01071800, 
		ITEMID_DeathJavelin, 0x01081600, 

		//Set 106 Inferno
		ITEMID_InfernoAxe, 0x01010100,
		ITEMID_InfernoClaw, 0x01021400,
		ITEMID_InfernoHammer, 0x01031600,
		ITEMID_BlessedStaff, 0x01041300,
		ITEMID_InfernoScythe, 0x01051700,
		ITEMID_EnigmaBow, 0x01061700,
		ITEMID_InfernoSword, 0x01070100,
		ITEMID_InfernoJavelin, 0x01081600,

		//Set 114 Hades
		ITEMID_BlazingAxe, 0x01011500,
		ITEMID_ImperialTalon, 0x01021400,
		ITEMID_LegendaryHammer, 0x01031600,
		ITEMID_MysticStaff, 0x01041300,
		ITEMID_DeathScythe, 0x01051700,
		ITEMID_SupremeBow, 0x01061700,
		ITEMID_ImperialSword, 0x01071800,
		ITEMID_FlameJavelin, 0x01081600,

		//Set 118 Mythril
		ITEMID_DemolitionAxe, 0x01011500,
		ITEMID_RagingClaw, 0x01021400,
		ITEMID_DestructiveHammer, 0x01031600,
		ITEMID_BrutalWand, 0x01040100,
		ITEMID_TwinScythe, 0x01051700,
		ITEMID_RogueBow, 0x01061700,
		ITEMID_EmperorSword, 0x01071800,
		ITEMID_BlessedJavelin, 0x01081600,

		//Set 126 Imperial
		ITEMID_GladiatorAxe, 0x01010100,
		ITEMID_AvengerClaw, 0x01021400,
		ITEMID_MoltenHammer, 0x01030200,
		ITEMID_CrystalWand, 0x01040100,
		ITEMID_DoubleSidedSpear, 0x01050200,
		ITEMID_FatalHandCrossBow, 0x01060200,
		ITEMID_RelicSword, 0x01070100,
		ITEMID_RoyalJavelin, 0x01081600,

		//??
		ITEMID_EragonSword, 0x01070100,
		ITEMID_TwilightStaff, 0x01041300,

		// Set Beginner
		0x01012E00, 0x01011500, // Kronus Axe
		0x01022D00, 0x01021400, // Kronus Talon
		0x01042F00, 0x01041300, // Kronus Staff
		0x01052E00, 0x01051700, // Kronus Spear
		0x01062F00, 0x01061700, // Kronus Bow
		0x01073100, 0x01071800, // Kronus Sword
		0x01082E00, 0x01081600, // Kronus Javelin

		//Toys
		ITEMID_LoveSword, 0x01070100,
		ITEMID_Rose, 0x01020100,
		ITEMID_PickAxe, 0x01011500,
		ITEMID_Guitar, 0x01051700,
		ITEMID_XmasCane, 0x01010100,
		ITEMID_TigersClaw, 0x01020100,
		ITEMID_GoldenTigersClaw, 0x01020100,
		ITEMID_PanFun, 0x01030100,
		ITEMID_GoldenPanFun, 0x01030100,


		//Set 120
		ITEMID_HalloweenAxe, 0x01010500, // Kronus Axe
		ITEMID_HalloweenClaw, 0x01021400, // Kronus Talon
		ITEMID_HalloweenHammer, 0x01031600, // Kronus Hammer
		ITEMID_HalloweenStaff, 0x01041300, // Kronus Staff
		ITEMID_HalloweenWand, 0x01040100, // Legend Wand
		ITEMID_HalloweenScythe, 0x01051700, // Kronus Spear
		ITEMID_HalloweenBow, 0x01061700, // Kronus Bow
		ITEMID_HalloweenSword, 0x01071800, // Kronus Sword
		ITEMID_HalloweenBladeSword, 0x01070100, // Legend Sword
		ITEMID_HalloweenJavelin, 0x01081600, // Kronus Javelin

		ITEMID_XmasCostumeOldAxe, 0x01011500, // Kronus Axe
		ITEMID_XmasCostumeOldAxe1Hand, 0x01010100, // Kronus Axe
		ITEMID_XmasCostumeOldClaw, 0x01021400, // Kronus Talon
		ITEMID_XmasCostumeOldHammer, 0x01031600, // Kronus Hammer
		ITEMID_XmasCostumeOldStaff, 0x01041300, // Kronus Staff
		ITEMID_XmasCostumeOldScythe, 0x01051700, // Kronus Spear
		ITEMID_XmasCostumeOldBow, 0x01060400, // Kronus Bow
		ITEMID_XmasCostumeOldSword, 0x01071800, // Kronus Sword
		ITEMID_XmasCostumeOldSword1Hand, 0x01070100, // Legend Sword
		ITEMID_XmasCostumeOldJavelin, 0x01081600, // Kronus Javelin

		ITEMID_XmasCostumeAxe, 0x01011500, // Kronus Axe
		ITEMID_XmasCostumeAxe1Hand, 0x01010100, // Kronus Axe
		ITEMID_XmasCostumeClaw, 0x01021400, // Kronus Talon
		ITEMID_XmasCostumeHammer, 0x01031600, // Kronus Hammer
		ITEMID_XmasCostumeStaff, 0x01041300, // Kronus Staff
		ITEMID_XmasCostumeWand, 0x01040100, // Legend Wand
		ITEMID_XmasCostumeScythe, 0x01051700, // Kronus Spear
		ITEMID_XmasCostumeBow, 0x01061700, // Kronus Bow
		ITEMID_XmasCostumeSword, 0x01071800, // Kronus Sword
		ITEMID_XmasCostumeSword1Hand, 0x01070100, // Legend Sword
		ITEMID_XmasCostumeJavelin, 0x01081600, // Kronus Javelin

		ITEMID_CandyAxe, 0x01011500, // Kronus Axe
		ITEMID_CandyClaw, 0x01021400, // Kronus Talon
		ITEMID_CandyHammer, 0x01031600, // Kronus Hammer
		ITEMID_CandyWand, 0x01040100, // Legend Wand
		ITEMID_CandyScythe, 0x01051700, // Kronus Spear
		ITEMID_CandyBow, 0x01061700, // Kronus Bow
		ITEMID_CandySword, 0x01071800, // Kronus Sword
		ITEMID_CandyJavelin, 0x01081600, // Kronus Javelin

		ITEMID_PCandyAxe, 0x01011500, // Kronus Axe
		ITEMID_PCandyClaw, 0x01021400, // Kronus Talon
		ITEMID_PCandyHammer, 0x01031600, // Kronus Hammer
		ITEMID_PCandyWand, 0x01040100, // Legend Wand
		ITEMID_PCandyScythe, 0x01051700, // Kronus Spear
		ITEMID_PCandyBow, 0x01061700, // Kronus Bow
		ITEMID_PCandySword, 0x01071800, // Kronus Sword
		ITEMID_PCandyJavelin, 0x01081600, // Kronus Javelin

		ITEMID_ObscureAxe, 0x01011500, // Kronus Axe
		ITEMID_ObscureAxe1Hand, 0x01010100, // Kronus Axe
		ITEMID_ObscureClaw, 0x01021400, // Kronus Talon
		ITEMID_ObscureHammer, 0x01031600, // Kronus Hammer
		ITEMID_ObscureStaff, 0x01041300, // Kronus Staff
		ITEMID_ObscureWand, 0x01040100, // Legend Wand
		ITEMID_ObscureScythe, 0x01051700, // Kronus Spear
		ITEMID_ObscureBow, 0x01061700, // Kronus Bow
		ITEMID_ObscureSword, 0x01071800, // Kronus Sword
		ITEMID_ObscureSword1Hand, 0x01070100, // Legend Sword
		ITEMID_ObscureJavelin, 0x01081600, // Kronus Javelin

		ITEMID_SandurrRedAxe, 0x01011500, // Kronus Axe
		ITEMID_SandurrRedClaw, 0x01021400, // Kronus Talon
		ITEMID_SandurrRedHammer, 0x01031600, // Kronus Hammer
		ITEMID_SandurrRedStaff, 0x01041300, // Kronus Staff
		ITEMID_SandurrRedScythe, 0x01051700, // Kronus Spear
		ITEMID_SandurrRedBow, 0x01061700, // Kronus Bow
		ITEMID_SandurrRedSword, 0x01071800, // Kronus Sword
		ITEMID_SandurrRedJavelin, 0x01081600, // Kronus Javelin

		ITEMID_SandurrPurpleAxe, 0x01011500, // Kronus Axe
		ITEMID_SandurrPurpleClaw, 0x01021400, // Kronus Talon
		ITEMID_SandurrPurpleHammer, 0x01031600, // Kronus Hammer
		ITEMID_SandurrPurpleStaff, 0x01041300, // Kronus Staff
		ITEMID_SandurrPurpleScythe, 0x01051700, // Kronus Spear
		ITEMID_SandurrPurpleBow, 0x01061700, // Kronus Bow
		ITEMID_SandurrPurpleSword, 0x01071800, // Kronus Sword
		ITEMID_SandurrPurpleJavelin, 0x01081600, // Kronus Javelin

		ITEMID_SandurrPurpleAxe1Hand, 0x01010100, // Kronus Axe
		ITEMID_SandurrRedAxe1Hand, 0x01010100, // Kronus Axe
		ITEMID_SandurrRedSword1Hand, 0x01070100, // Legend Sword
		ITEMID_SandurrPurpleSword1Hand, 0x01070100, // Legend Sword
		ITEMID_SandurrPurple2Scythe, 0x01051700, // Kronus Spear


		ITEMID_ZealousAxe, 0x01011500, // Kronus Axe
		ITEMID_FerociousAxe, 0x01010100, // Kronus Axe
		ITEMID_FerociousClaw, 0x01021400, // Kronus Talon
		ITEMID_TaurusHammer, 0x01031600, // Kronus Hammer
		ITEMID_DeviousStaff, 0x01041300, // Kronus Staff
		ITEMID_EternalWand, 0x01040100, // Legend Wand
		ITEMID_TormentorScythe, 0x01051700, // Kronus Spear
		ITEMID_LuciferBow, 0x01061700, // Kronus Bow
		ITEMID_HellbringerSword, 0x01071800, // Kronus Sword
		ITEMID_FerociousSword, 0x01070100, // Legend Sword
		ITEMID_OphidianJavelin, 0x01081600, // Kronus Javelin

		ITEMID_AbyssAxe, 0x01011500, // Abyss Axe
		ITEMID_AbyssClaw, 0x01021400, // Abyss Talon
		ITEMID_AbyssHammer, 0x01031600, // Abyss Hammer
		ITEMID_AbyssStaff, 0x01041300, // Abyss Staff
		ITEMID_AbyssScythe, 0x01051700, // Abyss Spear
		ITEMID_AbyssBow, 0x01061700, // Abyss Bow
		ITEMID_AbyssSword, 0x01071800, // Abyss Sword
		ITEMID_AbyssJavelin, 0x01081600, // Abyss Javelin

};


ItemHandler::ItemHandler()
{
	ZeroMemory( psaModelDataDefault, sizeof( EXEModelData* ) * _countof( psaModelDataDefault ) );
}


ItemHandler::~ItemHandler()
{
}

ItemData * ItemHandler::FindItemTable( UINT uPosition )
{
	if ( pItemTable )
		return &pItemTable[ uPosition ];

	return NULL;
}

int ItemHandler::FindItemTablePosition( int iItemID )
{
	for ( UINT i = 0; i < ITEMTABLE_ITEM_COUNT; i++ )
	{
		ItemData * psItemData = (ItemData *)((DWORD)allocIT + (i * 0x314));
		if ( psItemData->sBaseItemID.ToInt() == iItemID )
			return i;
	}

	return -1;
}

ItemData * ItemHandler::FindItemTableByCode( DWORD dwCode )
{
	for ( UINT i = 0; i < ITEMTABLE_ITEM_COUNT; i++ )
	{
		ItemData * psItemData = (ItemData*)((DWORD)allocIT + (i * 0x314));

		if ( psItemData->sBaseItemID.ToInt() == dwCode )
			return psItemData;
	}
	return NULL;
}

ItemData * ItemHandler::GetMouseItemHover()
{
	if ( ITEMINDEX_INVENTORY >= 0 )
		return &INVENTORYITEMS[ITEMINDEX_INVENTORY];

	if ( (*(int*)0x03653E14) )
	{
		ItemData * psItemCharShop = (ItemData*)(0x03642094);
		return &psItemCharShop[(*(int*)0x03653E14)-1];
	}

	// Blacksmith
	if ( (*(BOOL*)0x03653E40) )
	{
		int iX = (*(int*)0x003653DE4);
		int iY = (*(int*)0x003653DE8);

		Rectangle2D rBlacksmith( 21, 86, 200, 200 );

		if ( rBlacksmith.Inside( MOUSEHANDLER->GetPosition() ) )
		{
			ItemData * psItemShop = (ItemData*)(0x03653E80);
			for ( int i = 0; i < 30; i++ )
			{
				if ( psItemShop[i].bValid )
				{
					if ( psItemShop[i].sPosition.iX == iX && psItemShop[i].sPosition.iY == iY )
						return &psItemShop[i];
				}
			}
		}
	}

	return NULL;
}

ItemData * ItemHandler::GetItemEquippedByType( EItemType eType )
{
	for( int i = 1; i < 15; i++ )
	{
		if( INVENTORYITEMSLOT[ i - 1 ].iItemIndex )
		{
			ItemData * psItemData = &INVENTORYITEMS[ INVENTORYITEMSLOT[ i - 1 ].iItemIndex - 1 ];

			if( psItemData->sBaseItemID.ToItemType() == eType )
				return psItemData;
		}
	}

	return NULL;
}

ItemData * ItemHandler::GetItemEquippedByBase( EItemBase eItemBase )
{
	for ( int i = 1; i < 15; i++ )
	{
		if ( INVENTORYITEMSLOT[i - 1].iItemIndex )
		{
			ItemData * psItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[i - 1].iItemIndex - 1];

			if ( psItemData->sBaseItemID.ToItemBase() == eItemBase )
				return psItemData;
		}
	}

	return NULL;
}

int ItemHandler::GetItemEquippedCountByBase( EItemBase iBase, ECharacterClass iClass )
{
	int iCount = 0;
	for ( int i = 1; i < 15; i++ )
	{
		if ( INVENTORYITEMSLOT[i].iItemIndex >= 0 )
		{
			ItemData * pcItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[i].iItemIndex - 1];

			if ( pcItemData->sBaseItemID.ToItemBase() == iBase )
			{
				iCount++;
				
				if ( iClass != CHARACTERCLASS_None )
				{
					if ( CharacterClassToClassFlag( iClass ) != pcItemData->sItem.eSpecialization )
						iCount--;
				}
			}
		}
	}

	return iCount;
}

int ItemHandler::GetItemEquippedCountByType( EItemType iType, ECharacterClass iClass )
{
	int iCount = 0;
	for ( int i = 1; i < 15; i++ )
	{
		if ( INVENTORYITEMSLOT[i].iItemIndex >= 0 )
		{
			ItemData * pcItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[i].iItemIndex - 1];

			if ( pcItemData->sBaseItemID.ToItemType() == iType )
			{
				iCount++;

				if ( iClass != CHARACTERCLASS_None )
				{
					if ( CharacterClassToClassFlag( iClass ) != pcItemData->sItem.eSpecialization )
						iCount--;
				}
			}
		}
	}

	return iCount;
}

int ItemHandler::GetItemEquippedCountByCode( EItemID iItemID, ECharacterClass iClass )
{
	int iCount = 0;
	for ( int i = 1; i < 15; i++ )
	{
		if ( INVENTORYITEMSLOT[i].iItemIndex >= 0 )
		{
			ItemData * pcItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[i].iItemIndex - 1];

			if ( pcItemData->sBaseItemID.ToItemID() == iItemID )
			{
				iCount++;

				if ( iClass != CHARACTERCLASS_None )
				{
					if ( CharacterClassToClassFlag( iClass ) != pcItemData->sItem.eSpecialization )
						iCount--;
				}
			}
		}
	}

	return iCount;
}

ItemData * ItemHandler::GetItemInventoryByCode( EItemID eID )
{
	for ( int i = 0; i < 100; i++ )
	{
		ItemData * ps = INVENTORYITEMS + i;
		if ( ps->bValid && ps->sBaseItemID.ToItemID() == eID )
			return ps;

		ps = TEMPINVENTORYITEMS + i;
		if ( ps->bValid && ps->sBaseItemID.ToItemID() == eID )
			return ps;
	}
	return NULL;
}

ItemData * ItemHandler::GetItemInventory( EItemID eID, int iChk1, int iChk2 )
{
	for ( int i = 0; i < 100; i++ )
	{
		ItemData * ps = INVENTORYITEMS + i;
		if ( ps->bValid && (ps->sItem.sItemID.ToItemID() == eID) && (ps->sItem.iChk1 == iChk1) && (ps->sItem.iChk2 == iChk2) )
			return ps;

		ps = TEMPINVENTORYITEMS + i;
		if ( ps->bValid && (ps->sItem.sItemID.ToItemID() == eID) && (ps->sItem.iChk1 == iChk1) && (ps->sItem.iChk2 == iChk2) )
			return ps;
	}
	return NULL;
}

int ItemHandler::GetItemAmountByCode( EItemID eID, EItemCraftType eCraft )
{
	int iCount = 0;
	if ( eID )
	{
		for ( int i = 0; i < 200; i++ )
		{
			ItemData * ps = INVENTORYITEMS + i;
			if ( ps->bValid && ps->sBaseItemID.ToItemID() == eID )
				iCount++;

			ps = TEMPINVENTORYITEMS + i;
			if ( ps->bValid && ps->sBaseItemID.ToItemID() == eID )
				iCount++;
		}
	}
	if ( eCraft != ITEMCRAFTTYPE_None )
	{
		for ( int i = 0; i < 200; i++ )
		{
			ItemData * ps = INVENTORYITEMS + i;
			if ( ps->bValid && ps->sItem.eCraftType == eCraft )
				iCount++;

			ps = TEMPINVENTORYITEMS + i;
			if ( ps->bValid && ps->sItem.eCraftType == eCraft )
				iCount++;
		}
	}
	return iCount;
}

char * ItemHandler::GetArmorPatternName( class UnitData * pcUnit, DWORD dwCode )
{
	char * pszArmorName = NULL;
	for ( int i = 0; i < _countof( saCharacterBodyList ); i++ )
	{
		if ( saCharacterBodyList[i].eItemID == dwCode )
		{
			if ( saCharacterBodyList[i].iClass == 0 || saCharacterBodyList[i].iClass == pcUnit->sCharacterData.iClass )
			{
				pszArmorName = saCharacterBodyList[i].pszFileName;
				continue;
			}
		}
	}
	return pszArmorName;
}

BOOL ItemHandler::GetItemPrice( class ItemData * pcItemData, PriceItem * psSellData )
{
	if ( !pcItemData || !psSellData )
		return FALSE;

	if ( pcItemData->iItemSlotFlag == ITEMSLOTFLAG_LeftRightHand )
	{
		if ( pcItemData->iItemSlot )
			pcItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[0].iItemIndex - 1];
	}

	float fIntegrity[2];
	float fPrice;


	fPrice			= (float)pcItemData->sItem.iSalePrice;
	fIntegrity[0]	= pcItemData->sItem.sIntegrity.sCur == 0 ? 1.0f : (float)(int)pcItemData->sItem.sIntegrity.sCur;
	fIntegrity[1]	= pcItemData->sItem.sIntegrity.sMax == 0 ? 1.0f : (float)(int)pcItemData->sItem.sIntegrity.sMax;

	psSellData->iRealPrice		= (int)((fPrice*(fIntegrity[0] / fIntegrity[1])) + (fPrice - (fPrice*(fIntegrity[0] / fIntegrity[1])))*0.25);
	psSellData->iPrice			= psSellData->iRealPrice / 5;
	psSellData->iRepairPrice	= (pcItemData->sItem.iSalePrice - psSellData->iRealPrice) / 8;

	return TRUE;
}

BOOL ItemHandler::DropItemToMap( ItemData * pcItemData )
{
	//Check if there's a special Item
	if ( (pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Premium1) ||
		(pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Premium2) ||
		(pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Premium3) ||
		(pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Premium4) ||
		(pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Premium5) ||
		(pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Puzzle3) ||
		(pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Puzzle4) )
	{
		//Is Drop? So Deny
		if ( pcItemData->sItem.iPotionCount == 0 )
			return FALSE;
	}

	//Warehouse Open?
	if ( WAREHOUSEHANDLER->IsWarehouseWindowOpen() )
	{
		//Try to Close Warehouse
		if ( (BOOL)CALLT( 0x005175E0, 0x036932FC ) == FALSE )
		{
			//Error
			pcItemData->bValid = FALSE;
			return FALSE;
		}
	}

	//Arenas?
	if ( (MAP_ID == MAPID_QuestArena) || (MAP_ID == MAPID_T5QuestArena) )
	{
		//Can't drop
		if ( pcItemData->sItem.iPotionCount != 100 )
			return FALSE;
	}

	//Item isn't Gold and isn't Valid? Deny
	if ( (pcItemData->sItem.sItemID.ToItemID() != ITEMID_Gold) && (pcItemData->bValid == FALSE) )
		return FALSE;

	//Invalid Item?
	if ( IsValidItem( &pcItemData->sItem ) == FALSE )
	{
		//Disconnect 4
		CALL_WITH_ARG1( 0x0062C340, 80 );
		return FALSE;
	}

	//NPCs open?
	if ( BLACKSMITHWINDOW_OPEN || (READDWORD( 0x0367DE9C ) == TRUE) || MIXWINDOW_OPEN || (*(BOOL *)(0x036834F0)) || SMELTWINDOW_OPEN ||
		CRAFTWINDOW_OPEN || RESETMIXWINDOW_OPEN || (*(BOOL *)0x03692F50) )
	{
		//Drop Gold? Deny
		if ( pcItemData->sItem.sItemID.ToItemID() == ITEMID_Gold )
			return FALSE;
	}

	//Share PW?
	if ( ACCOUNTHANDLER->IsAccountShareDenied( ACCOUNTSHARE_DenyPickupAndDropItems ) )
	{
		if ( pcItemData->sItem.iPotionCount > 0 )
		{
			if ( (pcItemData->sItem.iPotionCount < 100) && (pcItemData->sItem.sItemID.ToItemBase() != ITEMBASE_Potion) )
			{
				TITLEBOX( AY_OBFUSCATE( "You can't Drop Items, because you're logged in with the Share PW!" ) );

				//Deny
				return FALSE;
			}
		}
	}

	//Share PW?
	if ( pcItemData->sItem.bLockedItemType )
	{
		if ( pcItemData->sItem.iPotionCount > 0 )
		{
			if ( (pcItemData->sItem.iPotionCount < 100) && (pcItemData->sItem.sItemID.ToItemBase() != ITEMBASE_Potion) )
			{
				TITLEBOX( AY_OBFUSCATE( "You can't drop this item, because this item is locked!" ) );

				//Deny
				return FALSE;
			}
		}
	}

	//Back Item
	BOOL bBackItemOld = FALSE;
	ItemData cItemDataOld;

	//Potions?
	if ( pcItemData->sItem.sItemID.ToItemBase() == ITEMBASE_Potion )
	{
		bBackItemOld = TRUE;
		CopyMemory( &cItemDataOld, pcItemData, sizeof( ItemData ) );

		CALL_WITH_ARG1( 0x0045BB30, (DWORD)&pcItemData->sItem );
	}

	//Drop Item
	if ( DropItemToMap( &pcItemData->sItem, UNIT->GetPosition() ) )
	{
		CALL_WITH_ARG1( 0x0045D780, (DWORD)&pcItemData->sItem );

		pcItemData->bValid = FALSE;

		SAVE;

		return TRUE;
	}

	if ( bBackItemOld )
		CopyMemory( pcItemData, &cItemDataOld, sizeof( ItemData ) );

	return FALSE;
}


BOOL ItemHandler::DropItemToMap( Item * psItem, Point3D sPosition )
{
	return (BOOL)CALL_WITH_ARG4( 0x0062AFB0, (DWORD)psItem, sPosition.iX, sPosition.iY, sPosition.iZ );
}

NAKED BOOL ItemHandler::UseItem( Item * psItem )
{
	JMP( 0x00622F30 );
}

int ItemHandler::DeleteItemQuest( EItemID eID, int iCount )
{
	int iCountDeleted = 0;
	for ( int i = 0; i < 200; i++ )
	{
		ItemData * ps = NULL;
		if ( i < 100 )
		{
			ps = INVENTORYITEMS + i;
		}
		else
		{
			ps = TEMPINVENTORYITEMS + (i - 100);
		}
		if ( ps && ps->bValid && ps->sBaseItemID.ToItemID() == eID )
		{
			ps->bValid = FALSE;
			if ( i < 100 )
			{
				if ( ps->iItemSlot )
				{
					INVENTORYITEMSLOT[ps->iItemSlot - 1].iItemIndex = 0;
					if ( ps->iVisibleItemType )
						fnSetItemCharacter( ps->sBaseItemID.ToItemID(), ps->iVisibleItemType, FALSE );
				}
			}
			if ( iCount != 0 )
			{
				iCountDeleted++;
				if ( iCountDeleted == iCount )
					break;
			}
		}
	}
	ValidateInventoryItems();
	CHECK_CHARACTER_CHECKSUM;
	CALLT( 0x004AE430, 0x035EBB20 );
	UpdateItemCharacterStatus();
	CALLT( 0x004AD2E0, 0x035EBB20 );

	return iCountDeleted;
}

void ItemHandler::DeleteItem( ItemData * psItemData )
{
	if ( psItemData )
	{
		psItemData->bValid = FALSE;
		if ( psItemData->iItemSlot )
		{
			INVENTORYITEMSLOT[psItemData->iItemSlot - 1].iItemIndex = 0;
			if ( psItemData->iVisibleItemType )
				fnSetItemCharacter( psItemData->sBaseItemID.ToItemID(), psItemData->iVisibleItemType, FALSE );
		}
		ValidateInventoryItems();
		CHECK_CHARACTER_CHECKSUM;
		CALLT( 0x004AE430, 0x035EBB20 );
		UpdateItemCharacterStatus();
		CALLT( 0x004AD2E0, 0x035EBB20 );
	}
}

BOOL ItemHandler::SetCharacterItem( EItemID iItemID, int iPosition, BOOL bActive )
{
	auto SetToolWeaponShield = []( EItemID iItemToolID, EItemSlot eSlot )->BOOL
	{
		return (BOOL)CALLT_WITH_ARG2( 0x00433990, (DWORD)UNITDATA, iItemToolID, eSlot );
	};

	auto SetAgingMixEffect = []( UnitTool * psHand, Item * psItem ) -> void
	{
		psHand->iBlinkSpeed = psItem->sEffectBlink[0];
		CopyMemory( psHand->saBlinkColor, psItem->sEffectColor, sizeof( short ) * _countof( psItem->sEffectColor ) );
		psHand->bStarEffect = psItem->bEnableEffect;
		psHand->iBlinkScale = psItem->iScaleBlink[0];
		psHand->pcBlendingMaterial = nullptr;
		psHand->iAgeLevel = 0;
		psHand->iMixID = 0;

		if ( psItem->eCraftType == ITEMCRAFTTYPE_Aging )
		{
			if ( (psItem->sAgeLevel >= 4) && (psItem->sAgeLevel <= MAX_AGING) )
			{
				psHand->pcBlendingMaterial = GRAPHICENGINE->GetAgingMaterial( psItem->sAgeLevel );
				psHand->iAgeLevel = psItem->sAgeLevel;
				psHand->iBlinkSpeed = 9;
			}
		}
		else if ( psItem->eCraftType == ITEMCRAFTTYPE_Mixing )
		{
			int iColorID = psItem->sAgeLevel;

			if ( iColorID > 0 )
			{
				psHand->pcBlendingMaterial = GRAPHICENGINE->GetMixingMaterial( iColorID - 1 );
				psHand->iMixID = iColorID;
				psHand->iBlinkSpeed = 8;
			}
		}

		//Set Mix Color
		//Deprecated CALL_WITH_ARG1( 0x00458F20, (DWORD)psHand );

		//Item Effect?
		if ( psHand->bStarEffect )
			CALL_WITH_ARG1( 0x005BEA40, (DWORD)UNITDATA );
	};

	auto SetBodyCharacter = []( int iNumberID, EItemType eItemType ) -> BOOL
	{
		return (BOOL)CALL_WITH_ARG2( 0x004581D0, iNumberID, eItemType );
	};

	switch ( iPosition )
	{
		case ITEMSLOTFLAG_LeftHand:
			if ( bActive )
			{
				SetToolWeaponShield( iItemID, ITEMSLOT_LeftHand );

				auto psItem = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex - 1].sItem;
				if ( psItem->szItemName[0] == 0 )
					auto psItem = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex - 1].sItem;

				if ( ItemID( iItemID ).ToItemType() != ITEMTYPE_Dagger )
					SetAgingMixEffect( &UNITDATA->sLeftHandTool, psItem );
			}
			else
				SetToolWeaponShield( ITEMID_None, ITEMSLOT_LeftHand );
			break;

		case ITEMSLOTFLAG_RightHand:
			if ( bActive )
			{
				if ( ItemID( iItemID ).ToItemType() == ITEMTYPE_Dagger )
				{
					SetToolWeaponShield( iItemID, ITEMSLOT_LeftHand );
					SetToolWeaponShield( iItemID, ITEMSLOT_RightHand );
				}
				else if ( (ItemID( iItemID ).ToItemType() == ITEMTYPE_Bow) )
				{
					if ( INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex )
					{
						auto pcItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex - 1];
						if ( pcItemData->iVisibleItemType == ITEMSLOTFLAG_LeftHand )
						{
							SetToolWeaponShield( iItemID, ITEMSLOT_LeftHand );

							if ( pcItemData->IsTwoHandedWeapon() )
								SetToolWeaponShield( ITEMID_None, ITEMSLOT_RightHand );
						}
						else
						{
							SetToolWeaponShield( iItemID, ITEMSLOT_RightHand );

							if ( pcItemData->IsTwoHandedWeapon() )
								SetToolWeaponShield( ITEMID_None, ITEMSLOT_LeftHand );
						}
					}
				}
				else
				{
					SetToolWeaponShield( iItemID, ITEMSLOT_RightHand );
				}

				if ( INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex )
				{
					auto psItem = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex - 1].sItem;

					if ( ItemID( iItemID ).ToItemType() == ITEMTYPE_Dagger )
					{
						SetAgingMixEffect( &UNITDATA->sLeftHandTool, psItem );
						SetAgingMixEffect( &UNITDATA->sRightHandTool, psItem );
					}
					else if ( (ItemID( iItemID ).ToItemType() == ITEMTYPE_Bow) )
					{
						if ( INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex )
						{
							auto pcItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex - 1];
                            if ( pcItemData->iVisibleItemType == ITEMSLOTFLAG_LeftHand )
								SetAgingMixEffect( &UNITDATA->sLeftHandTool, psItem );
							else
								SetAgingMixEffect( &UNITDATA->sRightHandTool, psItem );
						}
					}
					else
						SetAgingMixEffect( &UNITDATA->sRightHandTool, psItem );
				}
			}
			else
			{
				if ( ItemID( iItemID ).ToItemType() == ITEMTYPE_Dagger )
				{
					SetToolWeaponShield( ITEMID_None, ITEMSLOT_LeftHand );
					SetToolWeaponShield( ITEMID_None, ITEMSLOT_RightHand );
				}
				else if ( (ItemID( iItemID ).ToItemType() == ITEMTYPE_Bow) )
				{
					if ( auto pcItemData = ITEMHANDLER->FindItemTableByCode( iItemID ) )
					{
						if ( pcItemData->iVisibleItemType == ITEMSLOTFLAG_LeftHand )
							SetToolWeaponShield( ITEMID_None, ITEMSLOT_LeftHand );
						else
						{
							SetToolWeaponShield( ITEMID_None, ITEMSLOT_RightHand );

							if ( pcItemData->IsTwoHandedWeapon() )
								SetToolWeaponShield( ITEMID_None, ITEMSLOT_LeftHand );
						}
					}
				}
				else
					SetToolWeaponShield( ITEMID_None, ITEMSLOT_RightHand );
			}
			break;

		case ITEMSLOTFLAG_CostumeLeft:
			if ( bActive )
			{
				SetToolWeaponShield( iItemID, ITEMSLOT_LeftHand );

				auto psItem = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex - 1].sItem;
				if ( psItem->szItemName[0] == 0 )
					auto psItem = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex - 1].sItem;

				if ( auto pcItemData = HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->GetShieldItem() )
					psItem = &pcItemData->sItem;

				SetAgingMixEffect( &UNITDATA->sLeftHandTool, psItem );
			}
			else
				SetToolWeaponShield( ITEMID_None, ITEMSLOT_LeftHand );
			break;

		case ITEMSLOTFLAG_CostumeRight:
			if ( bActive )
			{
				if ( ItemID( iItemID ).ToItemType() == ITEMTYPE_Dagger )
				{
					SetToolWeaponShield( iItemID, ITEMSLOT_LeftHand );
					SetToolWeaponShield( iItemID, ITEMSLOT_RightHand );
				}
				else if ( (ItemID( iItemID ).ToItemType() == ITEMTYPE_Bow) )
				{
                    if ( auto pcItemData = HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->GetWeaponItem() )
                    {
                        if ( pcItemData->iVisibleItemType == ITEMSLOTFLAG_LeftHand )
                        {
                            SetToolWeaponShield( iItemID, ITEMSLOT_LeftHand );

                            if ( pcItemData->IsTwoHandedWeapon() )
                                SetToolWeaponShield( ITEMID_None, ITEMSLOT_RightHand );
                        }
						else
						{
							SetToolWeaponShield( iItemID, ITEMSLOT_RightHand );

							if ( pcItemData->IsTwoHandedWeapon() )
								SetToolWeaponShield( ITEMID_None, ITEMSLOT_LeftHand );
						}
					}
				}
				else
					SetToolWeaponShield( iItemID, ITEMSLOT_RightHand );

				if ( INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex )
				{
					auto psItem = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex - 1].sItem;
					if ( auto pcItemData = HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->GetWeaponItem() )
						psItem = &pcItemData->sItem;

					if ( ItemID( iItemID ).ToItemType() == ITEMTYPE_Dagger )
					{
						SetAgingMixEffect( &UNITDATA->sLeftHandTool, psItem );
						SetAgingMixEffect( &UNITDATA->sRightHandTool, psItem );
					}
					else if ( (ItemID( iItemID ).ToItemType() == ITEMTYPE_Bow) )
					{
                        if ( auto pcItemData = HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->GetWeaponItem() )
                        {
                            if ( pcItemData->iVisibleItemType == ITEMSLOTFLAG_LeftHand )
								SetAgingMixEffect( &UNITDATA->sLeftHandTool, psItem );
							else
								SetAgingMixEffect( &UNITDATA->sRightHandTool, psItem );
						}
					}
					else
						SetAgingMixEffect( &UNITDATA->sRightHandTool, psItem );
				}
			}
			else
			{
				if ( ItemID( iItemID ).ToItemType() == ITEMTYPE_Dagger )
				{
					SetToolWeaponShield( ITEMID_None, ITEMSLOT_LeftHand );
					SetToolWeaponShield( ITEMID_None, ITEMSLOT_RightHand );
				}
				else if ( (ItemID( iItemID ).ToItemType() == ITEMTYPE_Bow) )
				{
					if ( auto pcItemData = HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->GetWeaponItem() )
					{
						if ( pcItemData->iVisibleItemType == ITEMSLOTFLAG_LeftHand )
							SetToolWeaponShield( ITEMID_None, ITEMSLOT_LeftHand );
						else
						{
							SetToolWeaponShield( ITEMID_None, ITEMSLOT_RightHand );

							if ( pcItemData->IsTwoHandedWeapon() )
								SetToolWeaponShield( ITEMID_None, ITEMSLOT_LeftHand );
						}
					}
				}
				else
					SetToolWeaponShield( ITEMID_None, ITEMSLOT_RightHand );
			}
			break;

		case ITEMSLOTFLAG_Armor:
		case ITEMSLOTFLAG_Costume:
		{
			if ( iPosition == ITEMSLOTFLAG_Costume )
			{
				if ( INVENTORYITEMSLOT[ITEMSLOT_Costume - 1].iItemIndex )
					WRITEDWORD( 0x03625718, TRUE );
			}

			EAnimationType eAnimationType = UNITDATA->GetAnimation();
			int iFrameTimer = UNITDATA->iFrameTimer;

			EItemType eItemType = ItemID( iItemID ).ToItemType();

			if ( iItemID != ITEMID_None )
			{
				int iNumberID = ((iItemID & 0x0000FFFF) >> 8);
				if ( bActive )
				{
					if ( ITEMHANDLER->OnSetArmorUnit( UNITDATA, iItemID ) == FALSE )
						SetBodyCharacter( iNumberID > 25 ? (iNumberID - 10) : iNumberID, eItemType );
					
					UNITDATA->saEquippedItems[0] = ITEMHANDLER->FindItemTablePosition( iItemID );
				}
				else
				{
					SetBodyCharacter( 0, ITEMTYPE_None );

					UNITDATA->saEquippedItems[0] = -2;
				}

				if ( eAnimationType == ANIMATIONTYPE_Die )
				{
					UNITDATA->Animate( eAnimationType );
					UNITDATA->iFrame = (UNITDATA->psModelAnimation->iEndFrame - 1) * 160;
					UNITDATA->iFrameTimer = iFrameTimer;
				}
			}
			else
			{
				SetBodyCharacter( -1, eItemType );

				UNITDATA->saEquippedItems[0] = -2;
			}
		}
		break;

		default:
			break;
	}

	return TRUE;
}

void ItemHandler::CheckPuzzleItemEvent()
{
	CALLT( 0x004B1810, 0x035EBB20 );
}

void ItemHandler::CheckRequirements()
{
	CALLT( 0x004BA670, 0x035EBB20 );
}

void ItemHandler::ResetPotionGate()
{
	CALLT( 0x004BB9E0, 0x035EBB20 );
}

BOOL ItemHandler::LoadItemImage( Item * psIn, ItemData * psOut )
{
	return fnLoadItemImage( psIn, psOut );
}

void ItemHandler::OnLoadArmorCharacter()
{
	// Load Armor List
	for ( int i = 0; i < _countof( saCharacterBodyList ); i++ )
		LoadArmorCharacter( saCharacterBodyList[ i ].pszFileName );
}

void ItemHandler::CheckDamageItems()
{
	CALLT( 0x004AFC10, 0x035EBB20 );
}

void ItemHandler::UpdateItemCharacterStatus( BOOL bHackCheck )
{
	typedef void( __thiscall *tfnUpdateCharacterItemStatus )(DWORD dwClass, BOOL b);
	tfnUpdateCharacterItemStatus fnUpdateCharacterItemStatus = (tfnUpdateCharacterItemStatus)0x004BC8C0;
	fnUpdateCharacterItemStatus( 0x035EBB20, bHackCheck );
}

int ItemHandler::OnItemPivot( int iPosition, UINT uItemID )
{
	int iSizeList = ( sizeof( PivotTableItemChr ) / 4 );

	if ( (UINT)iPosition > ITEMTABLE_ITEM_COUNT )
		return 0;

	int ItemPosition = iPosition * 0x314;

	if ( (uItemID & 0xFFFF0000) == 0x010A0000 )
	{
		if ( (*(UINT*)((UINT)allocIT + ItemPosition) & 0xFFFF0000) == 0x010A0000 )
			return 1;
	}

	for ( int i = 0; i < iSizeList; i += 2 )
	{
		if ( PivotTableItemChr[ i ] == uItemID )
		{
			int iNextItem = PivotTableItemChr[ i + 1 ];
			if ( *( UINT* )( ( UINT )allocIT + ItemPosition ) == iNextItem )
				return 1;
		}
	}

	if ( *( UINT* )( ( UINT )allocIT + ItemPosition ) == uItemID )
		return 1;

	return 0;
}

BOOL ItemHandler::OnSetArmorUnit( UnitData * psUnit, UINT uItemCode )
{
	// Set item code to handler address
	( *( UINT* )0x0209EA44 ) = uItemCode;
	
	// Character data
	int iJobCode = psUnit->sCharacterData.iClass - 1;
	char * pszHeadCharacter = psUnit->sCharacterData.Player.szHeadModel;
	char * pszArmorName = GetArmorPatternName( psUnit, uItemCode );

	// Set Armor
	if ( pszArmorName )
	{
		psUnit->bNoMove = TRUE;
		SETPLAYERPATTERN( psUnit, pszArmorName, pszHeadCharacter );
		return TRUE;
	}
	return FALSE;
}

BOOL ItemHandler::OnSetArmorTargetUnit( UnitData * psUnit, UINT uPosition )
{
	ItemData * pItem = FindItemTable( uPosition );
	
	if ( pItem )
		return OnSetArmorUnit( psUnit, pItem->sBaseItemID.ToInt() );

	return FALSE;
}

ECharacterGender ItemHandler::GetItemGender( int iItemID )
{
	ECharacterGender e = ECharacterGender::CHARACTERGENDER_None;

	switch ( iItemID )
	{
		//Male
		case ITEMID_WeddingSuit:
		case ITEMID_Costume23ArmorMale:
		case ITEMID_Costume24ArmorMale:
		case ITEMID_Costume25ArmorMale:
		case ITEMID_RomanticSuit:
		case ITEMID_XMasElegantCostumeM:
		case ITEMID_PartyCostumeMale:
		case ITEMID_IdasCostumeMale:
		case ITEMID_ThalesArmorMale:
		case ITEMID_Costume4ArmorMale:
		case ITEMID_Costume5ArmorMale:
		case ITEMID_WingArmorMale:
		case ITEMID_DarkWingArmorMale:
		case ITEMID_SummerClotheMale:
		case ITEMID_SpringCostumeMale:
		case ITEMID_YellowSpringCostumeMale:
		case ITEMID_Costume11ArmorMale:
		case ITEMID_BlackSuit:
		case ITEMID_MechanicCostumeM:
		case ITEMID_BroyaleCostumeMale:
		case ITEMID_CasualSchoolUniformMale:
		case ITEMID_ElegantSchoolUniformMale:
		case ITEMID_Costume16ArmorMale:
		case ITEMID_MarineCostumeMale:
		case ITEMID_HopyCostume:
		case ITEMID_HopyCostumeTime:
		case ITEMID_HopyCostumeIce:
		case ITEMID_ChemistCostumeMale:
		case ITEMID_DraculaDragonCostumeMale:
		case ITEMID_GaiaArmorMale:
		case ITEMID_DarkIriaRobeMale:
		case ITEMID_DarkGaiaArmorMale:
		case ITEMID_IriaRobeMale:
		case ITEMID_MikalRobeMale:
		case ITEMID_MartanosRobeMale:
		case ITEMID_SilverCostumeMale:
		case ITEMID_VampireSuit:
		case ITEMID_XMasCloatCostumeTM:
		case ITEMID_IceCostumeMale:
		case ITEMID_GoldenHopyCostumeMale:
		case ITEMID_GoldenHopyCostumeMaleTime:
		case ITEMID_ChaoticCostume:

		case ITEMID_SandurrRedCostumeMale:
		case ITEMID_SandurrPurpleCostumeMale:
			e = CHARACTERGENDER_Male;
			break;

		//Female
		case ITEMID_RomanticDress:
		case ITEMID_XMasElegantCostumeF:
		case ITEMID_PartyCostumeFemale:
		case ITEMID_IdasCostumeFemale:
		case ITEMID_ThalesArmorFemale:
		case ITEMID_Costume4ArmorFemale:
		case ITEMID_Costume5ArmorFemale:
		case ITEMID_WingArmorFemale:
		case ITEMID_DarkWingArmorFemale:
		case ITEMID_SummerClotheFemale:
		case ITEMID_SpringCostumeFemale:
		case ITEMID_YellowSpringCostumeFemale:
		case ITEMID_Costume11ArmorFemale:
		case ITEMID_WhiteDress:
		case ITEMID_MechanicCostumeF:
		case ITEMID_BroyaleCostumeFemale:
		case ITEMID_CasualSchoolUniformFemale:
		case ITEMID_ElegantSchoolUniformFemale:
		case ITEMID_Costume16ArmorFemale:
		case ITEMID_MarineCostumeFemale:
		case ITEMID_CuepyCostume:
		case ITEMID_CuepyCostumeTime:
		case ITEMID_CuepyCostumeIce:
		case ITEMID_ChemistCostumeFemale:
		case ITEMID_DraculaDragonCostumeFemale:
		case ITEMID_GaiaArmorFemale:
		case ITEMID_IriaRobeFemale:
		case ITEMID_DarkGaiaArmorFemale:
		case ITEMID_DarkIriaRobeFemale:
		case ITEMID_WeddingDress:
		case ITEMID_Costume23ArmorFemale:
		case ITEMID_Costume24ArmorFemale:
		case ITEMID_Costume25ArmorFemale:
		case ITEMID_MikalRobeFemale:
		case ITEMID_MartanosRobeFemale:
		case ITEMID_SilverCostumeFemale:
		case ITEMID_Succubus:
		case ITEMID_XMasCloatCostumeTF:
		case ITEMID_IceCostumeFemale:
		case ITEMID_GoldenHopyCostumeFemale:
		case ITEMID_GoldenHopyCostumeFemaleTime:
		case ITEMID_SandurrRedCostumeFemale:
		case ITEMID_SandurrPurpleCostumeFemale:
		case ITEMID_ChaoticCostumeFemale:
			e = CHARACTERGENDER_Female;
			break;

		default:
			break;
	}

	if ( e == CHARACTERGENDER_None )
	{
		if ( iItemID == ITEMID_Costume19MaleArmorFemale )
			e = UNITDATA->GetCharacterGender();
	}

	return e;
}

BOOL ItemHandler::CanEquipItem( void * pcClassItemData, ItemData * pcItemData )
{
	int iJobCode = UNITDATA->sCharacterData.iClass;
	EItemID iItemID = pcItemData->sItem.sItemID.ToItemID();
	int iGender = GetItemGender( iItemID );

	int iMask = iItemID & 0xFFFF0000;

	// TRUE = Can't use the item
	BOOL bNotUse	= FALSE;

	switch ( iMask )
	{
		case ITEMTYPE_Orb:
		case ITEMTYPE_Robe:
			if ( iJobCode != CHARACTERCLASS_Priestess && iJobCode != CHARACTERCLASS_Magician && iJobCode != CHARACTERCLASS_Shaman )
			{
				bNotUse = TRUE;
				if ( iItemID == ITEMID_SoccerShirt )
					bNotUse = FALSE;
			}
			if ( iItemID == ITEMID_SoccerShirt )
			{
				ItemData * psItemData = GetItemEquippedByType( ITEMTYPE_Costume );
				if ( psItemData && psItemData->bValid )
				{
					bNotUse = TRUE;
				}
				else if ( (psItemData = GetItemEquippedByType( ITEMTYPE_Costume2 )) && psItemData->bValid )
				{
					bNotUse = TRUE;
				}
				else if ( (psItemData = GetItemEquippedByType( ITEMTYPE_Costume3 )) && psItemData->bValid )
				{
					bNotUse = TRUE;
				}
				else if ( (psItemData = GetItemEquippedByType( ITEMTYPE_Costume4 )) && psItemData->bValid )
				{
					bNotUse = TRUE;
				}

				if ( bNotUse )
				{
					psItemData = GetItemEquippedByType( ITEMTYPE_Robe );
					if ( !psItemData || psItemData->sItem.sItemID.ToItemID() != ITEMID_SoccerShirt )
						bNotUse = FALSE;
				}
			}
			break;

		case ITEMTYPE_Armor:
			if ( iJobCode == CHARACTERCLASS_Priestess || iJobCode == CHARACTERCLASS_Magician || iJobCode == CHARACTERCLASS_Shaman )
			{
				bNotUse = TRUE;
				// This items can be used by the classes
				if ( iItemID == ITEMID_XMasCloatCostumeTM && (iJobCode == CHARACTERCLASS_Magician || iJobCode == CHARACTERCLASS_Shaman) )
					bNotUse = FALSE;
				if ( iItemID == ITEMID_XMasCloatCostumeTF && (iJobCode == CHARACTERCLASS_Priestess) )
					bNotUse = FALSE;

				if ( iJobCode == CHARACTERCLASS_Magician || iJobCode == CHARACTERCLASS_Shaman )
				{
					if ( iItemID == ITEMID_WeddingSuit ||
						iItemID == ITEMID_Costume23ArmorMale ||
						iItemID == ITEMID_Costume24ArmorMale ||
						iItemID == ITEMID_Costume25ArmorMale ||
						iItemID == ITEMID_SilverCostumeMale ||
						iItemID == ITEMID_Costume19MaleArmorFemale )
						bNotUse = FALSE;
				}

				if ( iJobCode == CHARACTERCLASS_Priestess )
				{
					if ( iItemID == ITEMID_WeddingDress ||
						iItemID == ITEMID_Costume23ArmorFemale ||
						iItemID == ITEMID_Costume24ArmorFemale ||
						iItemID == ITEMID_Costume25ArmorFemale ||
						iItemID == ITEMID_SilverCostumeFemale ||
						iItemID == ITEMID_Costume19MaleArmorFemale )
						bNotUse = FALSE;
				}
			}

			if ( bNotUse == FALSE )
			{
				if ( iGender != CHARACTERGENDER_None && UNITDATA->GetCharacterGender() != iGender )
					bNotUse = TRUE;
			}
			break;

		case ITEMTYPE_Shield:
			if ( iJobCode == CHARACTERCLASS_Priestess || iJobCode == CHARACTERCLASS_Magician || iJobCode == CHARACTERCLASS_Shaman || iJobCode == CHARACTERCLASS_Assassin )
				bNotUse = TRUE;
			break;

		case ITEMTYPE_Dagger:
			if ( iJobCode != CHARACTERCLASS_Assassin )
				bNotUse = TRUE;
			break;

		case ITEMTYPE_Phantom:
			if ( iJobCode != CHARACTERCLASS_Shaman )
				bNotUse = TRUE;
			break;

		case ITEMTYPE_Costume:
		case ITEMTYPE_Costume2:
		case ITEMTYPE_Costume3:
		case ITEMTYPE_Costume4:
			if ( iGender != CHARACTERGENDER_None && UNITDATA->GetCharacterGender() != iGender )
				bNotUse = TRUE;		

			if ( bNotUse == FALSE )
			{
				if ( (iItemID == ITEMID_MikalRobeMale || iItemID == ITEMID_MikalRobeFemale || //MAGIC CLASSES CAN USE
					iItemID == ITEMID_MartanosRobeMale || iItemID == ITEMID_MartanosRobeFemale) )
				{
					if ( iJobCode != CHARACTERCLASS_Priestess && iJobCode != CHARACTERCLASS_Magician && iJobCode != CHARACTERCLASS_Shaman )
						bNotUse = TRUE;
				}
				else if ( (iItemID == ITEMID_IdasCostumeMale || iItemID == ITEMID_IdasCostumeFemale || //MAGIC CLASSES CANNOT USE
					iItemID == ITEMID_ThalesArmorMale || iItemID == ITEMID_ThalesArmorFemale) )
				{
					if ( iJobCode == CHARACTERCLASS_Priestess || iJobCode == CHARACTERCLASS_Magician || iJobCode == CHARACTERCLASS_Shaman )
						bNotUse = TRUE;
				}
			}

			break;

		default:
			break;
	}

	if ( bNotUse )
		pcItemData->sItem.bCanNotUse = TRUE;

	return TRUE;
}

BOOL ItemHandler::EquipItem( UnitData * pcUnitData, EItemID iItemID, int iPositionID )
{
	return (BOOL)CALLT_WITH_ARG2( 0x00433990, (DWORD)pcUnitData, iItemID, iPositionID );
}

BOOL ItemHandler::EquipBody( UnitData * pcUnitData, int iPositionID )
{
	return (BOOL)CALL_WITH_ARG2( 0x00458340, (DWORD)pcUnitData, iPositionID );
}

BOOL ItemHandler::OnSetHairModel( DWORD dwCode )
{

	HeadModelStruct * pHead				= NULL;
	HeadModelStruct * psaHeadModelList	= NULL;

	int iArrayCount = 0;

	switch ( UNITDATA->sCharacterData.iClass )
	{
		case CHARACTERCLASS_Fighter:
			iArrayCount			= _countof( saFighterHeadModel );
			psaHeadModelList	= saFighterHeadModel;
			break;

		case CHARACTERCLASS_Mechanician:
			iArrayCount			= _countof( saMechanicianHeadModel );
			psaHeadModelList	= saMechanicianHeadModel;
			break;

		case CHARACTERCLASS_Archer:
			iArrayCount			= _countof( saArcherHeadModel );
			psaHeadModelList	= saArcherHeadModel;
			break;

		case CHARACTERCLASS_Pikeman:
			iArrayCount			= _countof( saPikemanHeadModel );
			psaHeadModelList	= saPikemanHeadModel;
			break;

		case CHARACTERCLASS_Atalanta:
			iArrayCount			= _countof( saAtalantaHeadModel );
			psaHeadModelList	= saAtalantaHeadModel;
			break;

		case CHARACTERCLASS_Knight:
			iArrayCount			= _countof( saKnightHeadModel );
			psaHeadModelList	= saKnightHeadModel;
			break;

		case CHARACTERCLASS_Magician:
			iArrayCount			= _countof( saMagicianHeadModel );
			psaHeadModelList	= saMagicianHeadModel;
			break;

		case CHARACTERCLASS_Priestess:
			iArrayCount			= _countof( saPriestessHeadModel );
			psaHeadModelList	= saPriestessHeadModel;
			break;

		case CHARACTERCLASS_Assassin:
			iArrayCount			= _countof( saAssassinHeadModel );
			psaHeadModelList	= saAssassinHeadModel;
			break;

		case CHARACTERCLASS_Shaman:
			iArrayCount			= _countof( saShamanHeadModel );
			psaHeadModelList	= saShamanHeadModel;
			break;

		default:
			break;
	}

	for ( int i = 0; i < iArrayCount; i++ )
	{
		// Mismatch sItemID?
		if ( psaHeadModelList[i].dwCode == dwCode )
		{
			// No need tier? save it
			if ( psaHeadModelList[i].dwTier == 0 )
				pHead = &psaHeadModelList[i];

			// Found head model with headbang? Set pointer and stop loop list
			if ( psaHeadModelList[i].dwTier && psaHeadModelList[i].dwTier == UNITDATA->sCharacterData.iRank )
			{
				pHead = &psaHeadModelList[i];
				break;
			}
		}
	}

	// Found head model?
	if ( pHead )
	{
		// Set to character model
		UNITGAME->SetCharacterHeadModel( pHead->pszFile, TRUE );
		return TRUE;
	}

	int iNum = -1;
	switch ( dwCode )
	{
		case ITEMID_HairPotionA:
			iNum = 1;
			break;
		case ITEMID_HairPotionB:
			iNum = 2;
			break;
		case ITEMID_HairPotionC:
			iNum = 3;
			break;
		case ITEMID_HairPotionD:
			iNum = 4;
			break;
		case ITEMID_HairPotionE:
			iNum = 5;
			break;
		case ITEMID_HairPotionF:
			iNum = 6;
			break;
		case ITEMID_HairPotionG:
			iNum = 7;
			break;
		case ITEMID_HairPotionH:
			iNum = 8;
			break;
		case ITEMID_HairPotionI:
			iNum = 9;
			break;
		case ITEMID_HairPotionJ:
			iNum = 10;
			break;
		case ITEMID_HairPotionK:
			iNum = 11;
			break;
		case ITEMID_HairPotionL:
			iNum = 12;
			break;
		case ITEMID_HairPotionM:
			iNum = 13;
			break;
		case ITEMID_HairPotionN:
			iNum = 17;
			break;
		case ITEMID_HairPotionO:
			iNum = 18;
			break;
		case ITEMID_HairPotionP:
			iNum = 19;
			break;
		case ITEMID_HairPotionQ:
			iNum = 20;
			break;
		case ITEMID_HairPotionR:
			iNum = 21;
			break;
		default:
			break;
	}

	if ( iNum != -1 )
	{
		if ( iNum == 1 )
		{
			CHARACTERGAME->ResetHead( TRUE );
			SKILLMANAGERHANDLER->StartSkillEffect( UNITDATA->sPosition.iX, UNITDATA->sPosition.iY, UNITDATA->sPosition.iZ, 0, 0, 0, SKILL_UP1 );
			SKILLMANAGERHANDLER->SkillPlaySound( 0x1000, UNITDATA->sPosition.iX, UNITDATA->sPosition.iY, UNITDATA->sPosition.iZ );
			return TRUE;
		}

		BOOL bResult = (BOOL)CALLT_WITH_ARG1( 0x00494630, 0x035D0350, iNum );
		return bResult;
	}
	return FALSE;
}

const char * ItemHandler::GetHeadModel( DWORD dwCode )
{
	HeadModelStruct * psaHeadModelList	= NULL;

	int iArrayCount = 0;

	switch ( UNITDATA->sCharacterData.iClass )
	{
		case CHARACTERCLASS_Fighter:
			iArrayCount			= _countof( saFighterHeadModel );
			psaHeadModelList	= saFighterHeadModel;
			break;

		case CHARACTERCLASS_Mechanician:
			iArrayCount			= _countof( saMechanicianHeadModel );
			psaHeadModelList	= saMechanicianHeadModel;
			break;

		case CHARACTERCLASS_Archer:
			iArrayCount			= _countof( saArcherHeadModel );
			psaHeadModelList	= saArcherHeadModel;
			break;

		case CHARACTERCLASS_Pikeman:
			iArrayCount			= _countof( saPikemanHeadModel );
			psaHeadModelList	= saPikemanHeadModel;
			break;

		case CHARACTERCLASS_Atalanta:
			iArrayCount			= _countof( saAtalantaHeadModel );
			psaHeadModelList	= saAtalantaHeadModel;
			break;

		case CHARACTERCLASS_Knight:
			iArrayCount			= _countof( saKnightHeadModel );
			psaHeadModelList	= saKnightHeadModel;
			break;

		case CHARACTERCLASS_Magician:
			iArrayCount			= _countof( saMagicianHeadModel );
			psaHeadModelList	= saMagicianHeadModel;
			break;

		case CHARACTERCLASS_Priestess:
			iArrayCount			= _countof( saPriestessHeadModel );
			psaHeadModelList	= saPriestessHeadModel;
			break;

		case CHARACTERCLASS_Assassin:
			iArrayCount			= _countof( saAssassinHeadModel );
			psaHeadModelList	= saAssassinHeadModel;
			break;

		case CHARACTERCLASS_Shaman:
			iArrayCount			= _countof( saShamanHeadModel );
			psaHeadModelList	= saShamanHeadModel;
			break;

		default:
			break;
	}

	for ( int i = 0; i < iArrayCount; i++ )
	{
		// Mismatch sItemID?
		if ( psaHeadModelList[i].dwCode == dwCode )
		{
			return psaHeadModelList[i].pszFile;
		}
	}
	
	return NULL;
}

char * ItemHandler::OnDescriptionItem( DWORD dwCode )
{
	return NULL;
}

void ItemHandler::ValidateInventoryItems()
{
	typedef void( __thiscall *tfnValidateInventory )(DWORD pcClass);
	tfnValidateInventory ValidateInventory = (tfnValidateInventory)0x004AE020;
	ValidateInventory( 0x035EBB20 );
}

void ItemHandler::ResetInventoryItemChecksum()
{
	CALL( 0x0045D760 );
}

void ItemHandler::ResetItemMouse()
{
	CALLT( 0x004BB690, 0x035EBB20 );
}

BOOL ItemHandler::BackItemToInventory( ItemData * psItem, EItemCheckInventory eCheck, bool bSlot )
{
	typedef BOOL( __thiscall *tfnBackItemToInventory )(DWORD dwClass, ItemData * psItemData, int v);
	tfnBackItemToInventory fnBackItemToInventory = (tfnBackItemToInventory)0x004BB200;

	BOOL bRet = FALSE;

	if ( bSlot )
		bRet = fnBackItemToInventory( 0x035EBB20, psItem, eCheck );
	else
		bRet = (BOOL)CALLT_WITH_ARG1( 0x004BB7B0, 0x035EBB20, (DWORD)psItem );

	return bRet;
}

BOOL ItemHandler::DeleteItemForServer( EItemID eItemID, int iChk1, int iChk2 )
{
	return (BOOL)CALLT_WITH_ARG3( 0x004AF340, 0x035EBB20, eItemID, iChk1, iChk2 );
}

void ItemHandler::ShowItemInfo( ItemData * pItem, BOOL bBottom )
{
	typedef void( __thiscall *tfnShowItemInfo )(DWORD pClass, ItemData * pItem, int iFlag, int index);
	tfnShowItemInfo fnShowItemInfo = (tfnShowItemInfo)0x004C9370;
	*(UINT*)0x036286EC = bBottom ? 2 : 1;
	fnShowItemInfo( 0x03628800, pItem, 0, 0 );
}

void ItemHandler::DrawColorBox( DWORD Color, int iX, int iY, int iW, int iH )
{
	typedef void( __cdecl *tfnDrawColorBox )(DWORD Color, int iX, int iY, int iW, int iH);
	tfnDrawColorBox fnDrawColorBox = (tfnDrawColorBox)0x00409F00;
	fnDrawColorBox( Color, iX, iY, iW, iH );
}

void ItemHandler::DrawSprite( int winX, int winY, int lpdds, int x, int y, int width, int height )
{
	typedef void( __cdecl *tfnDrawSprite )( int winX, int winY, int lpdds, int x, int y, int width, int height, int bltfast );
	tfnDrawSprite fnDrawSprite = (tfnDrawSprite)0x00505B20;
	fnDrawSprite( winX, winY, lpdds, x, y, width, height, 1 );
}

bool ItemHandler::DelayCristal()
{
	typedef bool( __thiscall *tfnCrystal )(DWORD item);
	tfnCrystal fnCrystal = (tfnCrystal)0x004B8510;
	if ( g_dwDelayCristal == 0 )
	{
		g_dwDelayCristal = TICKCOUNT + 2000;
		return fnCrystal( 0x035EBB20 );
	}
	else
	{
		if ( g_dwDelayCristal > TICKCOUNT )
		{
			TITLEBOX( "You are already summoning a pet monster" );
			return false;
		}
		else
		{
			g_dwDelayCristal = TICKCOUNT + 2000;
			return fnCrystal( 0x035EBB20 );
		}
	}
	return false;
}

BOOL ItemHandler::OnSendUpgradeAgingItem( ItemData * psItem )
{
	typedef void( __cdecl *tfnAgingUpgradeItem )(Item * psItem);
	tfnAgingUpgradeItem fnAgingUpgradeItem = (tfnAgingUpgradeItem)0x006228F0;

	if ( psItem && (psItem->bValid || psItem->sItem.bMaturing) && psItem->sItem.sMatureBar.sMax > 0)
	{
		if ( psItem->sItem.sMatureBar.sCur < 0 || psItem->sItem.sMatureBar.sCur >= psItem->sItem.sMatureBar.sMax )
		{
			psItem->sItem.sMatureBar.sCur = psItem->sItem.sMatureBar.sMax;
			fnAgingUpgradeItem( &psItem->sItem );
			return TRUE;
		}
		return TRUE;
	}
	return FALSE;
}

BOOL ItemHandler::CheckQuestItemAging( Item * psItemInfo )
{
	if ( psItemInfo->sItemID.ToItemID() == ITEMID_PotionAgingQuestBee )
		return TRUE;

	if ( STRINGCOMPARE( psItemInfo->szItemName, "Demon Hunter" ) )
		return TRUE;

	if ( STRINGCOMPARE( psItemInfo->szItemName, "Ancient Revenge" ) )
		return TRUE;

	if ( STRINGCOMPARE( psItemInfo->szItemName, "Dark Skull" ) )
		return TRUE;

	if ( STRINGCOMPARE( psItemInfo->szItemName, "Devil Scythe" ) )
		return TRUE;

	if ( STRINGCOMPARE( psItemInfo->szItemName, "Mist" ) )
		return TRUE;

	if ( STRINGCOMPARE( psItemInfo->szItemName, "Platinum Sword" ) )
		return TRUE;

	if ( STRINGCOMPARE( psItemInfo->szItemName, "Anaconda" ) )
		return TRUE;

	if ( STRINGCOMPARE( psItemInfo->szItemName, "Platinum Dagger" ) )
		return TRUE;

	if ( STRINGCOMPARE( psItemInfo->szItemName, "Hutan Phantom" ) )
		return TRUE;

	return FALSE;
}


BOOL ItemHandler::OnMatureStone( DWORD dwCode )
{
	ItemData * psItem = NULL;
	
	if ( dwCode == ITEMID_TurquoiseStone )
	{
		// Weapon
		psItem = &INVENTORYITEMS[INVENTORYITEMSLOT[0].iItemIndex - 1];
		if ( !CheckQuestItemAging( &psItem->sItem ) )
		{
			psItem->sItem.sMatureBar.sCur += 50;
			if ( OnSendUpgradeAgingItem( psItem ) )
				return TRUE;
		}

		if ( HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->IsValidWeaponItem() )
		{
			if ( auto pcItemData = HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->GetWeaponItem() )
			{
				pcItemData->sItem.sMatureBar.sCur += 50;
				if ( OnSendUpgradeAgingItem( pcItemData ) )
					return TRUE;
			}
		}

		if ( HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->IsValidShieldItem() )
		{
			if ( auto pcItemData = HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->GetShieldItem() )
			{
				pcItemData->sItem.sMatureBar.sCur += 50;
				if ( OnSendUpgradeAgingItem( pcItemData ) )
					return TRUE;
			}
		}

		// Shield, Orb
		psItem = &INVENTORYITEMS[INVENTORYITEMSLOT[1].iItemIndex - 1];
		psItem->sItem.sMatureBar.sCur += 50;
		if ( OnSendUpgradeAgingItem( psItem ) )
			return TRUE;

		// Armor
		psItem = &INVENTORYITEMS[INVENTORYITEMSLOT[2].iItemIndex - 1];
		psItem->sItem.sMatureBar.sCur += 50;
		if ( OnSendUpgradeAgingItem( psItem ) )
			return TRUE;

	}
	else if ( dwCode == ITEMID_TopazStone )
	{
		// Weapon
		psItem = &INVENTORYITEMS[INVENTORYITEMSLOT[0].iItemIndex - 1];
		if ( !CheckQuestItemAging( &psItem->sItem ) )
		{
			psItem->sItem.sMatureBar.sCur = psItem->sItem.sMatureBar.sMax;
			if ( OnSendUpgradeAgingItem( psItem ) )
				return TRUE;
		}

		//if ( HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->IsValidWeaponItem() )
		{
			if ( auto pcItemData = HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->GetWeaponItem() )
			{
				pcItemData->sItem.sMatureBar.sCur = pcItemData->sItem.sMatureBar.sMax;
				if ( OnSendUpgradeAgingItem( pcItemData ) )
					return TRUE;
			}
		}

		//if ( HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->IsValidShieldItem() )
		{
			if ( auto pcItemData = HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->GetShieldItem() )
			{
				pcItemData->sItem.sMatureBar.sCur = pcItemData->sItem.sMatureBar.sMax;
				if ( OnSendUpgradeAgingItem( pcItemData ) )
					return TRUE;
			}
		}

		// Shield, Orb
		psItem = &INVENTORYITEMS[INVENTORYITEMSLOT[1].iItemIndex - 1];
		psItem->sItem.sMatureBar.sCur = psItem->sItem.sMatureBar.sMax;
		if ( OnSendUpgradeAgingItem( psItem ) )
			return TRUE;

		// Armor
		psItem = &INVENTORYITEMS[INVENTORYITEMSLOT[2].iItemIndex - 1];
		psItem->sItem.sMatureBar.sCur = psItem->sItem.sMatureBar.sMax;
		if ( OnSendUpgradeAgingItem( psItem ) )
			return TRUE;

		psItem = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_Costume - 1].iItemIndex - 1];
		psItem->sItem.sMatureBar.sCur = psItem->sItem.sMatureBar.sMax;
		if ( OnSendUpgradeAgingItem( psItem ) )
			return TRUE;
	}


	return FALSE;
}

BOOL ItemHandler::OnSkillStone( DWORD dwCode )
{
	BOOL bTrained = FALSE;
	if ( dwCode == ITEMID_EmeraldStone )
	{
		for ( int i = 0; i < 12; i++ )
		{
			if ( SKILLSCHARACTER[i+1].bActive && SKILLSCHARACTER[i+1].iLevel && SKILLSCHARACTER[i+1].UseSkillCount < 10000 )
			{
				SKILLSCHARACTER[i+1].UseSkillCount += 1000;
				SKILLMANAGERHANDLER->ReformSkill( NULL, i + 1 );
				bTrained = TRUE;
			}
		}

		if ( bTrained == FALSE )
			TITLEBOX( "Don't have skills to train!" );
	}
	if ( dwCode == ITEMID_RubyStone )
	{
		if ( UNITDATA->sCharacterData.iLevel <= 95 )
		{
			for ( int i = 0; i < 12; i++ )
			{
				if ( SKILLSCHARACTER[i + 1].bActive && SKILLSCHARACTER[i + 1].iLevel && SKILLSCHARACTER[i + 1].UseSkillCount < 10000 )
				{
					SKILLSCHARACTER[i + 1].UseSkillCount += 1000;
					SKILLMANAGERHANDLER->ReformSkill( NULL, i + 1 );
					bTrained = TRUE;
				}
			}
			if ( bTrained == FALSE )
				TITLEBOX( "Don't have skills to train!" );
		}
		else
			TITLEBOX( "Required level 1-95 to use this item" );
	}
	return bTrained;
}

void ItemHandler::OnAcceptRollDiceItem()
{
	if( UNITDATA )
	{
		PacketRollDiceDrop sPacket;
		sPacket.iHeader = PKTHDR_RollDiceDrop;
		sPacket.iLength = sizeof(PacketRollDiceDrop);
		sPacket.bAcceptItem = MESSAGEBOX->GetType();
		sPacket.iID = UNITDATA->iID;
		sPacket.eItemID = ITEMID_None;
		SENDPACKETG( &sPacket );
	}
}

BOOL ItemHandler::OnCheckCanItemDrill( ItemData * pcItemData, int iUnk )
{
	return FALSE;
}

BOOL ItemHandler::OnRenderRightWeapon( UnitData * pcUnitData, PTModel * pcModel, EXEMatrixI * psSourceMatrix, EXEMatrixI * psDestMatrix )
{
	if ( pcUnitData->bRenderRightHand && (pcUnitData->sRightHandTool.eItemID & 0xFFFF0000) == ITEMTYPE_Phantom )
	{
		pcModel->uFrame = pcUnitData->iFrame;

		EXEMatrixI si;
		EXEMatrixIdentityI( si );
		si.i[0][0] += (256 / 3);
		si.i[1][1] += (256 / 3);
		si.i[2][2] += (256 / 3);

		pcModel->paMesh[0]->GetWorldMatrix() = pcUnitData->sRightHandMatrix;
		pcModel->paMesh[0]->GetWorldMatrix() = EXEMatrixMultiplyI( si, pcModel->paMesh[0]->GetWorldMatrix() );
	}
	
	CopyMemory( psDestMatrix, psSourceMatrix, sizeof( EXEMatrixI ) );

	return pcUnitData->bRenderRightHand;
}

BOOL ItemHandler::OnRenderSecondWeapon( UnitData * pcUnitData, PTModel * pcModel, EXEMatrixI * psSourceMatrix, EXEMatrixI * psDestMatrix, BOOL bRight )
{
	if ( (pcUnitData->sRightHandTool.eItemID & 0xFFFF0000) == ITEMTYPE_Dagger )
	{
		EXEMatrixI si;
		EXEMatrixIdentityI( si );

		pcModel->uFrame = -1;

		if ( bRight == FALSE )
			EXEMatrixRotateZI( si, (4096 / 2) & 4095 );
		
		pcModel->paMesh[0]->GetWorldMatrix() = pcUnitData->sLeftHandMatrix;
		pcModel->paMesh[0]->GetWorldMatrix() = EXEMatrixMultiplyI( si, pcModel->paMesh[0]->GetWorldMatrix() );
	}

	CopyMemory( psDestMatrix, psSourceMatrix, sizeof( EXEMatrixI ) );

	return TRUE;
}

int ItemHandler::OnUseItem( ItemData * pcItemData )
{ //Return 0: Deny Use 1: Allow Use 2: Used
	EItemID iItemID		= pcItemData->sItem.sItemID.ToItemID();
	EItemType iItemType = pcItemData->sItem.sItemID.ToItemType();

	//Crystals
	if ( (iItemType == ITEMTYPE_Crystal) || (iItemType == ITEMTYPE_MonsterCrystal) )
	{
		if ( UNITDATA->iLoadedMapIndex >= 0 )
		{
			if ( FIELDGAME[UNITDATA->iLoadedMapIndex]->iMapType == MAPTYPE_Town )
			{
				if ( (iItemID != ITEMID_CaravanHopyCrystal) && (iItemID != ITEMID_CaravanRudolphCrystal) && (iItemID != ITEMID_CaravanArmaCrystal) &&
					 (iItemID != ITEMID_CaravanBumaCrystal) && (iItemID != ITEMID_CaravanCuepyCrystal) && (iItemID != ITEMID_CaravanHungkyCrystal) &&
					 (iItemID != ITEMID_CaravanRabieCrystal) && (iItemID != ITEMID_CaravanTobieCrystal) && (iItemID != ITEMID_CaravanSirKnightCrystal) &&
					 (iItemID != ITEMID_CaravanPinguCrystal) && (iItemID != ITEMID_CaravanHopyCheapCrystal) && (iItemID != ITEMID_CaravanYetiCrystal) && (iItemID != ITEMID_CaravanGoldenHopyCrystal) )
					return 0;
			}
		}

		//Battle Town
		if ( MAP_ID == MAPID_BattleTown )
			if ( (iItemID != ITEMID_CaravanHopyCrystal) && (iItemID != ITEMID_CaravanRudolphCrystal) && (iItemID != ITEMID_CaravanArmaCrystal) &&
				 (iItemID != ITEMID_CaravanBumaCrystal) && (iItemID != ITEMID_CaravanCuepyCrystal) && (iItemID != ITEMID_CaravanHungkyCrystal) &&
				 (iItemID != ITEMID_CaravanRabieCrystal) && (iItemID != ITEMID_CaravanTobieCrystal) && (iItemID != ITEMID_CaravanSirKnightCrystal) &&
				 (iItemID != ITEMID_CaravanPinguCrystal) && (iItemID != ITEMID_CaravanHopyCheapCrystal) && (iItemID != ITEMID_CaravanYetiCrystal) && (iItemID != ITEMID_CaravanGoldenHopyCrystal) )
				return 0;

		//T5 Quest Arena
		if ( MAP_ID == MAPID_T5QuestArena )
			return 0;

		//Bellatra
		if ( MAP_ID == MAPID_Bellatra )
		{
			TITLEBOX( "You can't summon a Crystal in Bellatra!" );
			return 0;
		}

		//Bless Castle
		if ( MAP_ID == MAPID_BlessCastle )
		{
			if ( iItemID == ITEMID_RicartenGuardCrystal )
				return 1;
			if ( iItemID == ITEMID_RicartenMilitiaGuardCrystal )
				return 1;
			if ( iItemID == ITEMID_RoyalBlessGuardCrystal )
				return 1;
			if ( iItemID == ITEMID_GuardianSaintCrystal )
				return 1;

			TITLEBOX( "You can only use Guardian Saint and Castle monster crystals in Bless Castle" );
			return 0;
		}

		if ( iItemID == ITEMID_SoulCrystal )
		{
			if ( GRANDFURYHANDLER->InsideArena( UNITDATA->sPosition.iX, UNITDATA->sPosition.iZ ) )
			{
				TITLEBOX( "You cannot summon a Soul Crystal inside the arena!" );
				return 0;
			}

			if ( SKILLMANAGERHANDLER->IsActiveBuff( ITEMID_SoulCrystal ) )
			{
				TITLEBOX( "You have summoned a Soul Crystal already, please wait until it finishes!" );
				return 0;
			}
		}
	}

	//Cores
	if ( iItemType == ITEMTYPE_EtherCore )
	{
		if ( BLESSCASTLEHANDLER->InBattleZone() )
		{
			TITLEBOX( "You cannot use ether cores inside the battle zone!" );
			return 0;
		}

		if ( MAP_ID == MAPID_CursedTempleF2 )
		{
			if ( GRANDFURYHANDLER->InsideArena( UNITDATA->sPosition.iX, UNITDATA->sPosition.iZ ) )
			{
				TITLEBOX( "You cannot use ether cores inside the arena!" );
				return 0;
			}

			return 1;
		}

		if ( (MAP_ID == MAPID_QuestArena) || (MAP_ID == MAPID_T5QuestArena) || (MAP_ID == MAPID_Bellatra) ||
			 (MAP_ID == MAPID_GhostCastle) || (MAP_ID == MAPID_ForestDungeon) )
		{
			TITLEBOX( "You cannot use ether cores inside the arena!" );
			return 0;
		}

		if ( BATTLEROYALEHANDLER->InWaiting() || BATTLEROYALEHANDLER->InBattle() )
		{
			TITLEBOX( "You cannot use ether cores inside the Battle Royale!" );
			return 0;
		}

		if ( iItemID == ITEMID_PartyCore )
		{
			PARTYHANDLER->GetPartyCore()->Open( pcItemData );
			return 2;
		}

		//Block it while riding
		if ( MOUNTHANDLER->IsRiding( UNITDATA ) )
		{
			switch ( iItemID )
			{
				case ITEMID_RicartenCore:
				case ITEMID_NaviskoCore:
				case ITEMID_AtlantisCore:
				case ITEMID_PillaiCore:
				case ITEMID_UnionCore:
					TITLEBOX( "You can't use Normal Cores while in a mount!" );
					return 0;
					break;
			}
		}
	}

	//Teleport Core
	if ( iItemID == ITEMID_TeleportCore )
	{
		if ( BLESSCASTLEHANDLER->InBattleZone() )
		{
			TITLEBOX( "You cannot use ether cores inside the battle zone!" );
			return 0;
		}

		if ( (MAP_ID == MAPID_T5QuestArena) || (MAP_ID == MAPID_Bellatra) || (MAP_ID == MAPID_GhostCastle) || (MAP_ID == MAPID_ForestDungeon) )
		{
			TITLEBOX( "You cannot use ether cores inside the arena!" );
			return 0;
		}

		if ( BATTLEROYALEHANDLER->InWaiting() || BATTLEROYALEHANDLER->InBattle() )
		{
			TITLEBOX( "You cannot use ether cores inside the Battle Royale!" );
			return 0;
		}

		TELEPORTCOREHANDLER->Open( pcItemData );

		return 2;
	}

	int iRet = 0;

	BOOL bSpecialItem = FALSE;

	switch ( iItemID )
	{
		case ITEMID_SilverLockedChest:
		case ITEMID_GoldenLockedChest:
		case ITEMID_PurpleLockedChest:
		case ITEMID_BlueLockedChest:
			iRet = 0;
			break;

		case ITEMID_HairPotionA:
		case ITEMID_HairPotionB:
		case ITEMID_HairPotionC:
		case ITEMID_HairPotionD:
		case ITEMID_HairPotionE:
		case ITEMID_HairPotionF:
		case ITEMID_HairPotionG:
		case ITEMID_HairPotionH:
		case ITEMID_HairPotionI:
		case ITEMID_HairPotionJ:
		case ITEMID_HairPotionK:
		case ITEMID_HairPotionL:
		case ITEMID_HairPotionM:
		case ITEMID_HairPotionN:
		case ITEMID_HairPotionO:
		case ITEMID_HairPotionP:
		case ITEMID_HairPotionQ:
		case ITEMID_HairPotionR:
		case ITEMID_HairPotionS:
		case ITEMID_HairPotionT:
		case ITEMID_SharinganEye:
		case ITEMID_IceHair:
		case ITEMID_SamuraiHair:
			iRet = 2;
			if ( ITEMTIMERHANDLER->GetHead() != NULL )
			{
				iRet = 0;

				TITLEBOX( "Can not use Hair Tint Potion while Big Head is active!" );
			}

			if ( iRet == 2 )
			{
				if ( ITEMHANDLER->OnSetHairModel( iItemID ) == FALSE )
					iRet = 0;
				else
				{
					//Save Old Head
					ITEMTIMERHANDLER->ProcessPacket( &PacketOldHeadUse( TRUE ) );

					STRINGCOPY( CHARACTERGAME->sCharacterDataEx.szOldHead, UNITDATA->sCharacterData.Player.szHeadModel );

					bSpecialItem = TRUE;
				}
			}
			break;

		case ITEMID_BigHeadHappyness:
		case ITEMID_BigHeadLove:
		case ITEMID_BigHeadSadness:
		case ITEMID_BigHeadShyness:
		case ITEMID_BigHeadAngry:
		case ITEMID_BigHeadSurprised:
		case ITEMID_BigHeadSensual:
		case ITEMID_CartolaHat:
		case ITEMID_WitchHat:
		case ITEMID_HalloweenHat:
		case ITEMID_XMasGreenHat:
		case ITEMID_XMasRedHat:
		case ITEMID_SheepHat:
		case ITEMID_GiraffeHat:
		case ITEMID_SoccerHat:
		case ITEMID_TuttiFruttiHat:
		case ITEMID_EasterHat:
		case ITEMID_ChemistHat:
		case ITEMID_PatronumHat:
		case ITEMID_GoldenHopyHat:
		case ITEMID_ObscureHat:

			if ( ITEMTIMERHANDLER->GetHead() == NULL )
			{
				iRet = 2;

				//Save Old Head
				ITEMTIMERHANDLER->ProcessPacket( &PacketOldHeadUse( TRUE ) );

				ITEMTIMERHANDLER->ProcessPacket( &PacketNewItemTimer( iItemID, pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 ) );

				bSpecialItem = TRUE;
			}
			break;

		case ITEMID_ExpUp1H:
		case ITEMID_ExpUp3H:
		case ITEMID_ExpUp24H:
		case ITEMID_ExpUp48H:
		case ITEMID_ExpUp72H:
			if ( ITEM_TIMER( ITEMTIMERTYPE_ExpUp ) == NULL )
			{
				iRet = 2;

				ITEMTIMERHANDLER->ProcessPacket( &PacketNewItemTimer( iItemID, pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 ) );

				bSpecialItem = TRUE;
			}
			break;

		case ITEMID_ThirdEye1H:
		case ITEMID_ThirdEye3H:
		case ITEMID_ThirdEye24H:
		case ITEMID_ThirdEye48H:
		case ITEMID_ThirdEye72H:
			if ( ITEM_TIMER( ITEMTIMERTYPE_ThirdEye ) == NULL )
			{
				iRet = 2;

				ITEMTIMERHANDLER->ProcessPacket( &PacketNewItemTimer( iItemID, pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 ) );

				bSpecialItem = TRUE;
			}
			break;

		case ITEMID_MPDown12H:
		case ITEMID_MPDown24H:
		case ITEMID_MPDown48H:
		case ITEMID_MPDown72H:
			if ( ITEM_TIMER( ITEMTIMERTYPE_MPDown ) == NULL )
			{
				iRet = 2;

				ITEMTIMERHANDLER->ProcessPacket( &PacketNewItemTimer( iItemID, pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 ) );

				bSpecialItem = TRUE;
			}
			break;

		case ITEMID_SPDown12H:
		case ITEMID_SPDown24H:
		case ITEMID_SPDown48H:
		case ITEMID_SPDown72H:
			if ( ITEM_TIMER( ITEMTIMERTYPE_SPDown ) == NULL )
			{
				iRet = 2;

				ITEMTIMERHANDLER->ProcessPacket( &PacketNewItemTimer( iItemID, pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 ) );

				bSpecialItem = TRUE;
			}
			break;

		case ITEMID_MPUp12H:
		case ITEMID_MPUp24H:
		case ITEMID_MPUp48H:
		case ITEMID_MPUp72H:
			if ( ITEM_TIMER( ITEMTIMERTYPE_MPUp ) == NULL )
			{
				iRet = 2;

				ITEMTIMERHANDLER->ProcessPacket( &PacketNewItemTimer( iItemID, pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 ) );

				bSpecialItem = TRUE;
			}
			break;

		case ITEMID_HPUp12H:
		case ITEMID_HPUp24H:
		case ITEMID_HPUp48H:
		case ITEMID_HPUp72H:
			if ( ITEM_TIMER( ITEMTIMERTYPE_HPUp ) == NULL )
			{
				iRet = 2;

				ITEMTIMERHANDLER->ProcessPacket( &PacketNewItemTimer( iItemID, pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 ) );

				bSpecialItem = TRUE;
			}
			break;

		case ITEMID_PhoenixEgg:
			iRet = 2;

			ITEMTIMERHANDLER->ProcessPacket( &PacketNewItemTimer( iItemID, pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 ) );

			bSpecialItem = TRUE;
			break;

		case ITEMID_TopazStone:
		case ITEMID_TurquoiseStone:
			if ( ITEMHANDLER->OnMatureStone( iItemID ) )
			{
				iRet = 2;
				bSpecialItem = TRUE;
			}
			else
			{
				TITLEBOX( "Don't have items to mature!" );
				iRet = 1;
			}
			break;

		case ITEMID_EmeraldStone:
		case ITEMID_RubyStone:
			if ( ITEMHANDLER->OnSkillStone( iItemID ) )
			{
				iRet = 2;
				bSpecialItem = TRUE;
			}
			else
			{
				TITLEBOX( "Don't have skills to train!" );
				iRet = 1;
			}
			break;

		case ITEMID_GravityStone72H:
			if ( ITEM_TIMER( ITEMTIMERTYPE_WeightStone ) == NULL )
			{
				iRet = 2;

				ITEMTIMERHANDLER->ProcessPacket( &PacketNewItemTimer( iItemID, pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 ) );

				bSpecialItem = TRUE;
			}
			break;

		case ITEMID_PinguMountMysterious:
		case ITEMID_MountObscureHog:
		case ITEMID_MountSnowflakeUnicorn:
		case ITEMID_MountHog:
		case ITEMID_MountHog1:
		case ITEMID_MountHopy:
		case ITEMID_MountHopy1:
		case ITEMID_MountHopy2:
		case ITEMID_MountHorse:
		case ITEMID_MountHorse1:
		case ITEMID_MountHorse2:
		case ITEMID_MountLion1:
		case ITEMID_MountLion5:
		case ITEMID_MountUnicorn3:
		case ITEMID_MountPingu1:
		case ITEMID_MountGhostWolf:
		case ITEMID_MountHorsePTM:
		case ITEMID_MountSnowdeer:
		case ITEMID_MountRudolf:
		case ITEMID_MountWhiteDeer:
		case ITEMID_MountIronCharger:
		case ITEMID_MountMecaballus:
		case ITEMID_MountZira:
		case ITEMID_MountIceTiger:
		case ITEMID_MountUnicorn6:
		case ITEMID_MountRabie:
		case ITEMID_MountRaptor:
		case ITEMID_MountRaptor1:
		case ITEMID_MountRaptor2:
		case ITEMID_MountRaptor3:
			if ( MOUNTHANDLER->HasMount( GetMountTypeByItemID( iItemID ) ) == false )
			{
				iRet = 2;

				ITEMTIMERHANDLER->ProcessPacket( &PacketNewItemTimer( iItemID, pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 ) );

				bSpecialItem = TRUE;
			}
			break;

		case ITEMID_RottenLantern:
		case ITEMID_EvilLantern:
		case ITEMID_InfernalLantern:
			// Check iWeight
			if ( (UNITDATA->sCharacterData.sWeight.sCur + 200) > UNITDATA->sCharacterData.sWeight.sMax )
			{
				TITLEBOX( "Weight limit exceeded" );
			}
			else if ( !CHARACTERGAME->CheckInventorySpace( ITEMHANDLER->FindItemTableByCode( ITEMID_BrazilianSoccer ) ) )
			{
				TITLEBOX( "Not enough space in inventory" );
			}
			else
			{
				if ( HALLOWEENHANDLER->EventIsAlive() == FALSE )
				{
					iRet = 0;

					//bSpecialItem = TRUE;

					TITLEBOX( "Halloween Event ended!" );
				}
				else
				{
					iRet = 2;

					bSpecialItem = TRUE;
				}
			}
			break;

		case ITEMID_SnowyXmasTree:
		case ITEMID_ColorfulXmasTree:
			if ( XMAS_EVENT_THINGS == FALSE )
			{
				iRet = 0;

				TITLEBOX( "Xmas Event ended!" );
			}
			else if ( (MAP_ID == MAPID_QuestArena) || (MAP_ID == MAPID_T5QuestArena) || (MAP_ID == MAPID_BlessCastle) || 
				(MAP_ID == MAPID_Bellatra) || GRANDFURYHANDLER->InsideArena( UNITDATA->sPosition.iX, UNITDATA->sPosition.iZ ) )
			{
				iRet = 0;

				TITLEBOX( "You can't spawn a Xmas Tree in this area!" );
			}
			else
			{
				if ( XMASTREEHANDLER->IsNearNPC( UNIT->GetPosition() ) == false )
				{
					XMASTREEHANDLER->ProcessPacket( &PacketXmasTreeItemUse( pcItemData->sItem.sItemID.ToInt(), pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 ) );

					iRet = 2;

					bSpecialItem = TRUE;
				}
				else
				{
					iRet = 0;

					TITLEBOX( "You can't spawn a Xmas Tree at this place!" );
				}
			}
			break;

		case ITEMID_CaravanHopyCrystal:
		case ITEMID_CaravanRudolphCrystal:
		case ITEMID_CaravanArmaCrystal:
		case ITEMID_CaravanBumaCrystal:
		case ITEMID_CaravanCuepyCrystal:
		case ITEMID_CaravanHungkyCrystal:
		case ITEMID_CaravanRabieCrystal:
		case ITEMID_CaravanTobieCrystal:
		case ITEMID_CaravanSirKnightCrystal:
		case ITEMID_CaravanPinguCrystal:
		case ITEMID_CaravanHopyCheapCrystal:
		case ITEMID_CaravanYetiCrystal:
		case ITEMID_CaravanGoldenHopyCrystal:
			if ( (ITEM_TIMER( ITEMTIMERTYPE_CaravanHopy ) == NULL) && (ITEM_TIMER( ITEMTIMERTYPE_CaravanRudolph ) == NULL) && (ITEM_TIMER( ITEMTIMERTYPE_CaravanArma ) == NULL) &&
				 (ITEM_TIMER( ITEMTIMERTYPE_CaravanBuma ) == NULL) && (ITEM_TIMER( ITEMTIMERTYPE_CaravanCuepy ) == NULL) && (ITEM_TIMER( ITEMTIMERTYPE_CaravanHungky ) == NULL) &&
				 (ITEM_TIMER( ITEMTIMERTYPE_CaravanRabie ) == NULL) && (ITEM_TIMER( ITEMTIMERTYPE_CaravanTobie ) == NULL) && (ITEM_TIMER( ITEMTIMERTYPE_CaravanSirKnight ) == NULL) &&
				 (ITEM_TIMER( ITEMTIMERTYPE_CaravanPingu ) == NULL) && (ITEM_TIMER( ITEMTIMERTYPE_CaravanYeti ) == NULL) && (ITEM_TIMER( ITEMTIMERTYPE_CaravanGoldenHopy ) == NULL) )
			{
				if ( (MAP_ID == MAPID_QuestArena) || (MAP_ID == MAPID_T5QuestArena) || (MAP_ID == MAPID_BlessCastle) ||
					(MAP_ID == MAPID_Bellatra) || GRANDFURYHANDLER->InsideArena( UNITDATA->sPosition.iX, UNITDATA->sPosition.iZ ) )
				{
					iRet = 0;

					TITLEBOX( "You can't summon a caravan in this area!" );
				}
				else
				{
					iRet = 1;

					ITEMTIMERHANDLER->ProcessPacket( &PacketNewItemTimer( iItemID, pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 ) );
				}
			}
			break;

		case ITEMID_FeatherTier1:
			if ( ITEM_TIMER( ITEMTIMERTYPE_SkillCooldownReduceT1 ) == NULL )
			{
				iRet = 2;

				ITEMTIMERHANDLER->ProcessPacket( &PacketNewItemTimer( iItemID, pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 ) );

				bSpecialItem = TRUE;
			}
			break;

		case ITEMID_FeatherTier2:
			if ( ITEM_TIMER( ITEMTIMERTYPE_SkillCooldownReduceT2 ) == NULL )
			{
				iRet = 2;

				ITEMTIMERHANDLER->ProcessPacket( &PacketNewItemTimer( iItemID, pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 ) );

				bSpecialItem = TRUE;
			}
			break;

		case ITEMID_FeatherTier3:
			if ( ITEM_TIMER( ITEMTIMERTYPE_SkillCooldownReduceT3 ) == NULL )
			{
				iRet = 2;

				ITEMTIMERHANDLER->ProcessPacket( &PacketNewItemTimer( iItemID, pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 ) );

				bSpecialItem = TRUE;
			}
			break;

		case ITEMID_FeatherTier4:
			if ( ITEM_TIMER( ITEMTIMERTYPE_SkillCooldownReduceT4 ) == NULL )
			{
				iRet = 2;

				ITEMTIMERHANDLER->ProcessPacket( &PacketNewItemTimer( iItemID, pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 ) );

				bSpecialItem = TRUE;
			}
			break;

		case ITEMID_FeatherTier5:
			if ( ITEM_TIMER( ITEMTIMERTYPE_SkillCooldownReduceT5 ) == NULL )
			{
				iRet = 2;

				ITEMTIMERHANDLER->ProcessPacket( &PacketNewItemTimer( iItemID, pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 ) );

				bSpecialItem = TRUE;
			}
			break;

		case ITEMID_HPBooster:
			if ( ITEM_TIMER( ITEMTIMERTYPE_HPBooster ) == NULL )
			{
				iRet = 2;

				ITEMTIMERHANDLER->ProcessPacket( &PacketNewItemTimer( iItemID, pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 ) );

				bSpecialItem = TRUE;
			}
			break;

		case ITEMID_MPBooster:
			if ( ITEM_TIMER( ITEMTIMERTYPE_MPBooster ) == NULL )
			{
				iRet = 2;

				ITEMTIMERHANDLER->ProcessPacket( &PacketNewItemTimer( iItemID, pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 ) );

				bSpecialItem = TRUE;
			}
			break;

		case ITEMID_STMBooster:
			if ( ITEM_TIMER( ITEMTIMERTYPE_STMBooster ) == NULL )
			{
				iRet = 2;

				ITEMTIMERHANDLER->ProcessPacket( &PacketNewItemTimer( iItemID, pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 ) );

				bSpecialItem = TRUE;
			}
			break;

		case ITEMID_GlamorousAttireGlaze:
		case ITEMID_GlamorousArmamentVarnish:
			GLAMOROUSBRUSHHANDLER->OnItemBrush( pcItemData );
			iRet = 2;
			break;

		case ITEMID_CarnivalLetterA:
		case ITEMID_CarnivalLetterC:
		case ITEMID_CarnivalLetterI:
		case ITEMID_CarnivalLetterL:
		case ITEMID_CarnivalLetterN:
		case ITEMID_CarnivalLetterR:
		case ITEMID_CarnivalLetterV:
		case ITEMID_CarnivalApple:
		case ITEMID_CarnivalBanana:
		case ITEMID_CarnivalJabuticaba:
		case ITEMID_CarnivalMaracuja:
		case ITEMID_CarnivalPitanga:
		case ITEMID_CarnivalStrawberry:
			iRet = 0;
			bSpecialItem = FALSE;
			break;

		default:
			iRet = 1;
			break;
	}

	if ( bSpecialItem )
		DropSpecialItem( pcItemData );

	return iRet;
}

void ItemHandler::ProcessItemSlotAction( ESlotTypeHandle iAction, ESlotTypeHandleWhere iWhere, int iItemID, int iChk1, int iChk2 )
{
	PacketSlotItemHandle s;
	s.iLength		= sizeof( PacketSlotItemHandle );
	s.iHeader		= PKTHDR_ItemSlotHandle;
	s.iTypeHandle	= iAction;
	s.iTypeWhere	= iWhere;
	s.iItemID		= iItemID;
	s.iChk1			= iChk1;
	s.iChk2			= iChk2;
	SENDPACKETL( &s );
}

void ItemHandler::SendGoldUse( int iGold )
{
	CALL_WITH_ARG1( 0x00629B00, iGold );
}

void ItemHandler::ProcessGold()
{
	CALL( 0x00628080 );
}

void ItemHandler::FormatDropItemGold( DropItemView * ps )
{
	STRINGCOPY( ps->szName, FormatNumber(atoi(ps->szName)) );
	STRINGCONCAT( ps->szName, " Gold" );

	ps->bFormatted = TRUE;
}

BOOL ItemHandler::CanViewDropItem( DropItemView * ps )
{
	ItemID sItemID( ps->iItemID );

	if ( SETTINGSHANDLER->GetModel()->GetSettings().bEnableLootFilter )
	{
		BOOL bRet = FALSE;

		BOOL bSpecUsage = TRUE;

		if ( sItemID.ToItemBase() == ITEMBASE_Potion )
		{
			if ( sItemID.ToItemType() == ITEMTYPE_HealthPotion )
				if ( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_HPPotion ) )
					bRet = TRUE;
			if ( sItemID.ToItemType() == ITEMTYPE_ManaPotion )
				if ( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_MPPotion ) )
					bRet = TRUE;
			if ( sItemID.ToItemType() == ITEMTYPE_StaminaPotion )
				if ( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_SPPotion ) )
					bRet = TRUE;
		}
		else if ( sItemID.ToItemID() == ITEMID_Gold )
		{
			if ( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_Gold ) )
				bRet = TRUE;
		}
		else if ( sItemID.ToItemType() == ITEMTYPE_Amulet )
		{
			if ( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_Amulets ) )
				bRet = TRUE;
		}
		else if ( (sItemID.ToItemType() == ITEMTYPE_Ring) || (sItemID.ToItemType() == ITEMTYPE_Ring2) )
		{
			if ( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_Rings ) )
				bRet = TRUE;
		}
		else if ( sItemID.ToItemType() == ITEMTYPE_Sheltom )
		{
			if ( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_Sheltoms ) )
				bRet = TRUE;
		}
		else if ( sItemID.ToItemType() == ITEMTYPE_ForceOrb )
		{
			if ( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_ForceOrbs ) )
				bRet = TRUE;
		}
		else if ( (sItemID.ToItemType() == ITEMTYPE_Premium1) || (sItemID.ToItemType() == ITEMTYPE_Premium2) )
		{
			bSpecUsage = FALSE;

			if ( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_Premiums ) )
				bRet = TRUE;
		}
		else if ( sItemID.ToItemType() == ITEMTYPE_MonsterCrystal )
		{
			bSpecUsage = FALSE;

			if ( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_MonsterCrystals ) )
				bRet = TRUE;
		}
		else if ( (sItemID.ToItemBase() == ITEMBASE_Defense) || (sItemID.ToItemType() == ITEMTYPE_Orb) )
		{
			if ( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_DefenseItems ) )
				bRet = TRUE;
		}
		else if ( (sItemID.ToItemBase() == ITEMBASE_Weapon) || (sItemID.ToItemType() == ITEMTYPE_Bracelets) )
		{
		if ( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_OffenseItems ) )
			bRet = TRUE;
		}
		else if ( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_Everything ) )
		bRet = TRUE;

		if ( bRet )
		{
			if ( ps->iItemID && SETTINGSHANDLER->GetModel()->HaveViewLootSpec() )
			{
				bRet = SameSpecFilterDropItem( ps );

				if ( bSpecUsage == FALSE )
					bRet = TRUE;
			}
		}

		return bRet;
	}

	return TRUE;
}

BOOL ItemHandler::SameSpecFilterDropItem( DropItemView * ps )
{
	BOOL bCanView = FALSE;
	switch ( ps->iClass )
	{
		case CHARACTERCLASS_Fighter:
			bCanView = SETTINGSHANDLER->GetModel()->CanViewLootSpec( LOOTFILTERSPEC_FS );
			break;
		case CHARACTERCLASS_Mechanician:
			bCanView = SETTINGSHANDLER->GetModel()->CanViewLootSpec( LOOTFILTERSPEC_MS );
			break;
		case CHARACTERCLASS_Archer:
			bCanView = SETTINGSHANDLER->GetModel()->CanViewLootSpec( LOOTFILTERSPEC_AS );
			break;
		case CHARACTERCLASS_Pikeman:
			bCanView = SETTINGSHANDLER->GetModel()->CanViewLootSpec( LOOTFILTERSPEC_PS );
			break;
		case CHARACTERCLASS_Atalanta:
			bCanView = SETTINGSHANDLER->GetModel()->CanViewLootSpec( LOOTFILTERSPEC_ATA );
			break;
		case CHARACTERCLASS_Knight:
			bCanView = SETTINGSHANDLER->GetModel()->CanViewLootSpec( LOOTFILTERSPEC_KS );
			break;
		case CHARACTERCLASS_Magician:
			bCanView = SETTINGSHANDLER->GetModel()->CanViewLootSpec( LOOTFILTERSPEC_MGS );
			break;
		case CHARACTERCLASS_Priestess:
			bCanView = SETTINGSHANDLER->GetModel()->CanViewLootSpec( LOOTFILTERSPEC_PRS );
			break;
		case CHARACTERCLASS_Assassin:
			bCanView = SETTINGSHANDLER->GetModel()->CanViewLootSpec( LOOTFILTERSPEC_ASS );
			break;
		case CHARACTERCLASS_Shaman:
			bCanView = SETTINGSHANDLER->GetModel()->CanViewLootSpec( LOOTFILTERSPEC_SHA );
			break;

		default:
			bCanView = SETTINGSHANDLER->GetModel()->CanViewLootSpec( LOOTFILTERSPEC_NS );
			break;
	}

	ItemID sItemID( ps->iItemID );
	if ( (sItemID.ToItemBase() == ITEMBASE_Potion) || (sItemID.ToItemType() == ITEMTYPE_GoldAndExp) )
		bCanView = TRUE;

	return bCanView;
}

BOOL ItemHandler::IsImportantDrop( DropItemView * ps )
{
	BOOL bRet = FALSE;

	if ( MAP_ID == MAPID_IceMineF1 )
	{
		switch ( ps->iItemID )
		{
			case ITEMID_KronusArmor:
				bRet = TRUE;
				break;
		}
	}

	return bRet;
}

NAKED PacketItem * ItemHandler::GetItemQueue( EItemID eItemID, int iChk1, int iChk2 )
{
	JMP( 0x00622E80 );
}

void ItemHandler::HandlePacket( PacketNPCItemBox * psPacket )
{

}

void ItemHandler::HandlePacket( PacketItemShopList * psPacket )
{
	iUniqueShopID = psPacket->iUniqueID;
	iNPCID = psPacket->iNPCID;
}

void ItemHandler::HandlePacket( PacketPlaceItem * psPacket )
{
	PacketItem * psPacketItemIn = (PacketItem *)0x03A46600;

	if ( PacketItem * psPacketItem = GetItemQueue( psPacket->iItemID, psPacket->iChk1, psPacket->iChk2 ) )
	{
		if ( CQUESTGAME->CanGetItem( psPacketItem, psPacket->iHeader ) )
		{
			CopyMemory( psPacketItemIn, psPacketItem, sizeof( PacketItem ) );

			if ( IsValidItem( &psPacketItem->sItem ) )
			{
				if ( psPacketItem->sItem.sItemID.ToItemID() == ITEMID_Gold )
				{
					if ( CHARACTERGAME->CanCarryGold( psPacketItem->sItem.iGold ) )
					{
						UNITDATA->sCharacterData.iGold += psPacketItem->sItem.iGold;
						UPDATE_CHARACTER_CHECKSUM;
						PLAYMINISOUND( 18 );

						CALL_WITH_ARG2( 0x0061EE40, (DWORD)SOCKETL, (DWORD)psPacketItem );
					}
					else
						DropItemToMap( &psPacketItem->sItem, UNITDATA->sPosition );
				}
				else
				{
					if ( (psPacketItem->sItem.sItemID.ToItemType() == ITEMTYPE_Quest) || (psPacketItem->sItem.eCraftType == ITEMCRAFTTYPE_QuestWeapon) )
					{
						if ( CGameInventory::PlaceQuestItem( &psPacketItem->sItem ) )
							SAVE;
					}
					else
					{
						if ( psPacketItem->sItem.eCraftType == ITEMCRAFTTYPE_Aging )
						{
							ItemData cItemDataOut;
							if ( LoadItemImage( &psPacketItemIn->sItem, &cItemDataOut ) )
							{
								if ( CGameInventory::GetInstance()->IsValidRequirements( &cItemDataOut ) )
								{
									if ( BackItemToInventory( &cItemDataOut ) )
									{
										UpdateItemCharacterStatus();
										CHARACTERGAME->UpdateWeight();
									}
									else
										DropItemToMap( &cItemDataOut );
								}
							}
						}
						else
							CGameInventory::GetInstance()->PlaceItemInventory( &psPacketItemIn->sItem );

						if ( psPacketItem->sItem.iSalePrice >= 3000 )
							SAVE;

						if ( psPacketItem->sItem.sItemID.ToItemBase() == ITEMBASE_Potion )
							CGameInventory::ResetPotion();
					}
				}
			}
		}

		ZeroMemory( psPacketItemIn, sizeof( PacketItem ) );
		ZeroMemory( psPacketItem, sizeof( PacketItem ) );

		ResetInventoryItemChecksum();
	}
}

void ItemHandler::HandlePacket( PacketDeleteItem * psPacket )
{
	//Find Item
	if ( auto pcItemData = ITEMHANDLER->GetItemInventory( psPacket->iItemID, psPacket->iChk1, psPacket->iChk2 ) )
	{
		//Delete Item
		ITEMHANDLER->DeleteItem( pcItemData );
	}
}

void ItemHandler::LoadDefaultModels()
{
	for ( int i = 0; i < 10; i++ )
	{
		EXEModelData * ps = Game::ReadInx( saCharacterBodyLoadDefault[i].pszFileName );
		if ( ps && ps->psModelData )
		{
			for ( int j = 0; j < ps->psModelData->iNumModelAnimation; j++ )
			{
				if ( ps->psModelData->saModelAnimation[j].iType == ANIMATIONTYPE_Idle )
				{
					psaModelDataDefault[i] = &ps->psModelData->saModelAnimation[j];
					break;
				}
			}
		}
	}
}

ModelAnimation * ItemHandler::GetModelDefault( int iClass )
{
	if ( iClass > 0 && iClass <= 10 )
		return psaModelDataDefault[iClass - 1];

	return NULL;
}

void ItemHandler::ShowDebugCloneItemWarehouse( ItemData * pcItemData )
{
	if ( GM_MODE )
	{
		ChatGame::AddChatBoxTextEx( CHATCOLOR_Error, "> %s [0x%08X][%d x %d]", pcItemData->sItem.szItemName,
			pcItemData->sItem.sItemID.ToInt(), pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 );
	}
}

void ItemHandler::DropSpecialItem( ItemData * pcItemData )
{
	pcItemData->sItem.iPotionCount = 300;

	ItemHandler::DropItemToMap( pcItemData );

	pcItemData->bValid = FALSE;

	CHARACTERGAME->UpdateWeight();
	ValidateInventoryItems();

	SAVE;
}

int ItemHandler::EncodeItemsData( void * psItems, void * psDest, int iLength )
{
	return CALL_WITH_ARG3( 0x0045B1B0, (DWORD)psItems, (DWORD)psDest, iLength );
}

int ItemHandler::DecodeItemsData( void * psItems, void * psDest, int iLength )
{
	return CALL_WITH_ARG3( 0x0045B120, (DWORD)psItems, (DWORD)psDest, iLength );
}

bool ItemHandler::IsForceEffect( Item & sItem )
{
	bool bRet = false;

	if ( sItem.sItemID.ToItemType() == ITEMTYPE_ForceOrb )
		bRet = true;

	return bRet;
}

BOOL ItemHandler::BuyItemShop( ItemData * pcItemData, int iItemCount )
{
	BOOL bRes = TRUE;

	if ( (UNITDATA->sCharacterData.sWeight.sCur + pcItemData->sItem.iWeight) > UNITDATA->sCharacterData.sWeight.sMax )
	{
		TITLEBOX( "Weight limit exceeded" );
		bRes = FALSE;
	}
	else if ( !CHARACTERGAME->CheckInventorySpace( pcItemData ) )
	{
		TITLEBOX( "Not enough space in inventory" );
		bRes = FALSE;
	}
	else
	{
		int iPrice = pcItemData->sItem.iSalePrice * iItemCount;

		BOOL bResPrice = FALSE;

		__asm
		{
			PUSH iPrice;
			MOV ECX, 0x03653E40;
			MOV EAX, 0x004E59C0;
			CALL EAX;
			MOV bResPrice, EAX;
		}

		if ( bResPrice == FALSE )
		{
			TITLEBOX( "Not enough gold" );
			bResPrice = FALSE;
			bRes = FALSE;
		}
	}

	if ( bRes )
	{
		PacketBuyNPCShop sPacket;
		sPacket.iLength = sizeof( PacketBuyNPCShop );
		sPacket.iHeader = PKTHDR_BuyItemNPCShop;
		sPacket.iUniqueID = iUniqueShopID;
		sPacket.iNPCID = iNPCID;

		sPacket.iCount = iItemCount;
		CopyMemory( &sPacket.sItemData, pcItemData, sizeof( ItemData ) );
		SENDPACKETL( &sPacket );
		SENDPACKETG( &sPacket );
	}

	return bRes;
}

NAKED BOOL ItemHandler::IsValidItem( Item * psItem )
{
	JMP( 0x0045BBA0 );
}

bool ItemHandler::GetWeaponParticlePosition( UnitData * pcUnitData, WeaponParticlePosition * psWeaponPosition )
{
	if ( pcUnitData == nullptr )
		return false;

	if ( (pcUnitData->pcBodyMeshContainer == nullptr) || (pcUnitData->sLeftHandTool.pBone == nullptr) || (pcUnitData->sLeftHandTool.pBone == nullptr) )
		return false;

	psWeaponPosition->iCount = 0;

	if ( pcUnitData->bProjectileMode )
	{
		psWeaponPosition->saPosition[psWeaponPosition->iCount++] = pcUnitData->sProjectilePosition;
		return true;
	}

	float fSizeMultiplier = 0.2f;

	EXEMatrixI mWorld;
	Point3D saItemPoint[3];
	for ( int i = 0; i < 3; i++ )
	{
		saItemPoint[i].iX = 0;
		saItemPoint[i].iY = 0;
	}

	if ( pcUnitData->sRightHandTool.eItemID )
	{
		mWorld = pcUnitData->sRightHandTool.pBone->GetWorldMatrix();
		if ( ItemID( pcUnitData->sRightHandTool.eItemID ).ToItemType() == ITEMTYPE_Javelin )
		{
			psWeaponPosition->iCount = 1;
			saItemPoint[0].iZ = -pcUnitData->sRightHandTool.iSizeMax + int( (float)pcUnitData->sRightHandTool.iSizeMax * fSizeMultiplier );
		}
		else if ( ItemID( pcUnitData->sRightHandTool.eItemID ).ToItemType() == ITEMTYPE_Sword )
		{
			if ( pcUnitData->sRightHandTool.iSizeMax > 5000 )
			{
				psWeaponPosition->iCount = 3;
				saItemPoint[0].iZ = pcUnitData->sRightHandTool.iSizeMax - int( (float)pcUnitData->sRightHandTool.iSizeMax * 0.25f );
				saItemPoint[1].iZ = pcUnitData->sRightHandTool.iSizeMax - int( (float)pcUnitData->sRightHandTool.iSizeMax * 0.48f );
				saItemPoint[2].iZ = pcUnitData->sRightHandTool.iSizeMax - int( (float)pcUnitData->sRightHandTool.iSizeMax * 0.69f );

			}
			else
			{
				psWeaponPosition->iCount = 2;
				saItemPoint[0].iZ = pcUnitData->sRightHandTool.iSizeMax - int( (float)pcUnitData->sRightHandTool.iSizeMax * 0.2f );
				saItemPoint[1].iZ = pcUnitData->sRightHandTool.iSizeMax - int( (float)pcUnitData->sRightHandTool.iSizeMax * 0.6f );
			}
		}
		else if ( ItemID( pcUnitData->sRightHandTool.eItemID ).ToItemType() == ITEMTYPE_Wand )
		{
			psWeaponPosition->iCount = 1;
			saItemPoint[0].iZ = pcUnitData->sRightHandTool.iSizeMax - int( (float)pcUnitData->sRightHandTool.iSizeMax * 0.2f );
		}
		else
		{
			psWeaponPosition->iCount = 1;
			saItemPoint[0].iZ = pcUnitData->sRightHandTool.iSizeMax - int( (float)pcUnitData->sRightHandTool.iSizeMax * fSizeMultiplier );
		}
	}
	else if ( pcUnitData->sLeftHandTool.eItemID )
	{
		mWorld = pcUnitData->sLeftHandTool.pBone->GetWorldMatrix();
		psWeaponPosition->iCount = 1;
		saItemPoint[0].iZ = pcUnitData->sLeftHandTool.iSizeMax - (4 << 8);

		if ( pcUnitData->sLeftHandTool.eItemID )
		{
			int iAngle = PTANGLE_90 & PTANGLE_Mask;
			GetMoveLocation( saItemPoint[0].iX, saItemPoint[0].iY, saItemPoint[0].iZ, 0, iAngle, 0 );
			saItemPoint[0].iX = RADIAN3D_X;
			saItemPoint[0].iY = RADIAN3D_Y;
			saItemPoint[0].iZ = RADIAN3D_Z;
		}
	}
	else
		return false;

	if ( pcUnitData->eWeaponItemID )
	{
		for ( int i = 0; i < psWeaponPosition->iCount; i++ )
		{
			int iAngle = (-PTANGLE_45 / 5) & PTANGLE_Mask;

			int sin = PTSEN[iAngle] >> 8;
			int cos = PTCOS[iAngle] >> 8;

			saItemPoint[i].iY = -(saItemPoint[i].iZ * sin) >> 8;
			saItemPoint[i].iZ = (saItemPoint[i].iZ * cos) >> 8;

		}
	}

	Point3D sAngle;
	memcpy( &sAngle, &pcUnitData->sAngle, sizeof( Point3D ) );
	sAngle.iY = (-sAngle.iY + PTANGLE_180) & PTANGLE_Mask;

	if ( (pcUnitData->sRightHandTool.eItemID == ITEMID_None) && (pcUnitData->sLeftHandTool.eItemID == ITEMID_None) )
		return false;

	int rx, ry, rz;
	for ( int index = 0; index < psWeaponPosition->iCount; index++ )
	{
		rx = saItemPoint[index].iX * mWorld.i[0][0] +
			 saItemPoint[index].iY * mWorld.i[1][0] +
			 saItemPoint[index].iZ * mWorld.i[2][0];
										   
		ry = saItemPoint[index].iX * mWorld.i[0][1] +
			 saItemPoint[index].iY * mWorld.i[1][1] +
			 saItemPoint[index].iZ * mWorld.i[2][1];
										   
		rz = saItemPoint[index].iX * mWorld.i[0][2] +
			 saItemPoint[index].iY * mWorld.i[1][2] +
			 saItemPoint[index].iZ * mWorld.i[2][2];

		psWeaponPosition->saPosition[index].iX = pcUnitData->sPosition.iX + (rx >> 8) + mWorld.i[3][0];
		psWeaponPosition->saPosition[index].iZ = pcUnitData->sPosition.iZ + (ry >> 8) + mWorld.i[3][1];
		psWeaponPosition->saPosition[index].iY = pcUnitData->sPosition.iY + (rz >> 8) + mWorld.i[3][2];
	}

	return true;
}

BOOL ItemHandler::MatureItem( EMatureType eMatureType, bool bInstantMature )
{
	auto MatureItemData = [this, bInstantMature]( ItemData * pcItemData )->void
	{
		if ( pcItemData && pcItemData->bValid && (pcItemData->sItem.eCraftType == ITEMCRAFTTYPE_Aging) )
		{
			if ( GM_MODE )
			{
				if ( bInstantMature == false )
					pcItemData->sItem.sMatureBar.sCur += 200;
				else
					pcItemData->sItem.sMatureBar.sCur = pcItemData->sItem.sMatureBar.sMax;
			}
			else
			{
				if ( bInstantMature == false )
					pcItemData->sItem.sMatureBar.sCur++;
				else
					pcItemData->sItem.sMatureBar.sCur = pcItemData->sItem.sMatureBar.sMax;
			}

			//Send Aging
			if ( pcItemData->sItem.sMatureBar.sMax && (pcItemData->sItem.sMatureBar.sCur >= pcItemData->sItem.sMatureBar.sMax) )
				OnSendUpgradeAgingItem( pcItemData );
		}
	};

	auto GetItemSlot = []( EItemSlot eSlot )-> ItemData *
	{
		if ( INVENTORYITEMSLOT[eSlot - 1].iItemIndex )
		{
			auto pcItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[eSlot - 1].iItemIndex - 1];
			if ( pcItemData->bValid )
				if ( pcItemData->sItem.bMaturing )
					return pcItemData;
		}

		return nullptr;
	};

	auto IsValidItemMatureType = [eMatureType]( ItemData * pcItemData ) -> bool
	{
		bool bRet = false;

		switch ( eMatureType )
		{
			case MATURETYPE_Attack:
				if ( (pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Axe) ||
					 (pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Hammer) ||
					 (pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Phantom) ||
					 (pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Wand) )
					bRet = true;
				break;
			case MATURETYPE_Critical:
				if ( (pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Claw) ||
					 (pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Bow) ||
					 (pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Sword) ||
					 (pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Scythe) ||
					 (pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Dagger) ||
					 (pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Javelin) )
					bRet = true;
				break;
			case MATURETYPE_Block:
				if ( pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Shield )
					bRet = true;
				break;
			case MATURETYPE_Orb:
				if ( pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Orb )
					bRet = true;
				break;
			case MATURETYPE_Armor:
				if ( (pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Armor) ||
					 (pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Robe) )
					bRet = true;
				break;
			case MATURETYPE_Boots:
				if ( pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Boots )
					bRet = true;
				break;
			case MATURETYPE_Gauntlets:
				if ( pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Gauntlets )
					bRet = true;
				break;
			case MATURETYPE_Bracelets:
				if ( pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Bracelets )
					bRet = true;
				break;
		}

		return bRet;
	};

	switch ( eMatureType )
	{
		case MATURETYPE_Attack:
		case MATURETYPE_Critical:
			if ( auto pcItemData = GetItemSlot( ITEMSLOT_RightHand ) )
			{
				if ( IsValidItemMatureType( pcItemData ) )
					MatureItemData( pcItemData );
			}
			if ( HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->IsValidWeaponItem() )
			{
				if ( auto pcItemData = HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->GetWeaponItem() )
				{
					if ( IsValidItemMatureType( pcItemData ) )
						MatureItemData( pcItemData );
				}
			}
		break;
		case MATURETYPE_Block:
		case MATURETYPE_Orb:
			if ( auto pcItemData = GetItemSlot( ITEMSLOT_LeftHand ) )
			{
				if ( IsValidItemMatureType( pcItemData ) )
					MatureItemData( pcItemData );
			}
			if ( HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->IsValidShieldItem() )
			{
				if ( auto pcItemData = HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->GetShieldItem() )
				{
					if ( IsValidItemMatureType( pcItemData ) )
						MatureItemData( pcItemData );
				}
			}
			break;
		case MATURETYPE_Armor:
			if ( auto pcItemData = GetItemSlot( ITEMSLOT_Armor ) )
			{
				if ( IsValidItemMatureType( pcItemData ) )
					MatureItemData( pcItemData );
			}
			if ( auto pcItemData = GetItemSlot( ITEMSLOT_Costume ) )
			{
				if ( IsValidItemMatureType( pcItemData ) && pcItemData->sItem.bCostumeItem )
					MatureItemData( pcItemData );
			}
			break;
		case MATURETYPE_Boots:
			if ( auto pcItemData = GetItemSlot( ITEMSLOT_Boots ) )
			{
				if ( IsValidItemMatureType( pcItemData ) )
					MatureItemData( pcItemData );
			}
			break;
		case MATURETYPE_Gauntlets:
			if ( auto pcItemData = GetItemSlot( ITEMSLOT_Gauntlets ) )
			{
				if ( IsValidItemMatureType( pcItemData ) )
					MatureItemData( pcItemData );
			}
			break;
		case MATURETYPE_Bracelets:
			if ( auto pcItemData = GetItemSlot( ITEMSLOT_Bracelets ) )
			{
				if ( IsValidItemMatureType( pcItemData ) )
					MatureItemData( pcItemData );
			}
			break;
	}

	return TRUE;
}

void ItemHandler::HandlePacket( PacketAgingUpgradeItem * psPacket )
{
	bool bFound = false;

	//Swap Inventory Item
	{
		if ( auto pcItemData = HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->GetWeaponItem() )
		{
			if ( (pcItemData->sItem.iBackupChk == psPacket->sItem.iBackupChk) &&
				(pcItemData->sItem.iBackupKey == psPacket->sItem.iBackupKey) &&
				 (pcItemData->sItem.sItemID.ToItemID() == psPacket->sItem.sItemID.ToItemID()) )
			{
				CopyMemory( &pcItemData->sItem, &psPacket->sItem, sizeof( Item ) );

				ValidateInventoryItems();
				CGameInventory::GetInstance()->IsValidRequirements( pcItemData );
				CGameCharacterStatus::GetInstance()->UpdateCharacterStatusItem();

				bFound = true;
			}
		}

		if ( auto pcItemData = HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->GetShieldItem() )
		{
			if ( (pcItemData->sItem.iBackupChk == psPacket->sItem.iBackupChk) &&
				(pcItemData->sItem.iBackupKey == psPacket->sItem.iBackupKey) &&
				 (pcItemData->sItem.sItemID.ToItemID() == psPacket->sItem.sItemID.ToItemID()) )
			{
				CopyMemory( &pcItemData->sItem, &psPacket->sItem, sizeof( Item ) );

				ValidateInventoryItems();
				CGameInventory::GetInstance()->IsValidRequirements( pcItemData );
				CGameCharacterStatus::GetInstance()->UpdateCharacterStatusItem();

				bFound = true;
			}
		}

		if ( bFound == false )
		{
			for ( int i = 0; i < INVENTORY_DATACOUNT; i++ )
			{
				auto pcItemData = &INVENTORYITEMS[i];
				if ( (pcItemData->sItem.iBackupChk == psPacket->sItem.iBackupChk) &&
					(pcItemData->sItem.iBackupKey == psPacket->sItem.iBackupKey) &&
					 (pcItemData->sItem.sItemID.ToItemID() == psPacket->sItem.sItemID.ToItemID()) )
				{
					CopyMemory( &pcItemData->sItem, &psPacket->sItem, sizeof( Item ) );

					ValidateInventoryItems();
					CGameInventory::GetInstance()->IsValidRequirements( pcItemData );
					CGameCharacterStatus::GetInstance()->UpdateCharacterStatusItem();
					break;
				}
			}
		}
	}

	AGEPACKET_SENDING = FALSE;
	ResetInventoryItemChecksum();
	SAVE;

	CALL_WITH_ARG4( 0x00623390, 0x110, 0, 0, 0 );

	auto GetItemSlot = []( EItemSlot eSlot )-> ItemData *
	{
		if ( INVENTORYITEMSLOT[eSlot - 1].iItemIndex )
		{
			auto pcItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[eSlot - 1].iItemIndex - 1];
			return pcItemData;
		}

		return nullptr;
	};

	if ( bFound == false )
	{
		if ( auto pcItemData = GetItemSlot( ITEMSLOT_RightHand ) )
		{
			if ( pcItemData->sItem.szItemName[0] && (pcItemData->sItem.sItemID.ToItemID() == psPacket->sItem.sItemID.ToItemID()) )
			{
				if ( (UNITDATA->sCharacterData.iClass == CHARACTERCLASS_Assassin) && (pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Dagger) )
				{
					SetCharacterItem( pcItemData->sItem.sItemID.ToItemID(), ITEMSLOTFLAG_RightHand, TRUE );
					SetCharacterItem( pcItemData->sItem.sItemID.ToItemID(), ITEMSLOTFLAG_LeftHand, TRUE );
				}
				else
					SetCharacterItem( pcItemData->sItem.sItemID.ToItemID(), pcItemData->iVisibleItemType, TRUE );
			}
		}

		if ( auto pcItemData = GetItemSlot( ITEMSLOT_LeftHand ) )
		{
			if ( pcItemData->sItem.szItemName[0] && (pcItemData->sItem.sItemID.ToItemID() == psPacket->sItem.sItemID.ToItemID()) )
				SetCharacterItem( pcItemData->sItem.sItemID.ToItemID(), pcItemData->iVisibleItemType, TRUE );
		}
	}
	else
	{
		if ( auto pcItemData = HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->GetWeaponItem() )
		{
			if ( pcItemData->sItem.szItemName[0] && (pcItemData->sItem.sItemID.ToItemID() == psPacket->sItem.sItemID.ToItemID()) )
			{
				if ( (UNITDATA->sCharacterData.iClass == CHARACTERCLASS_Assassin) && (pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Dagger) )
				{
					SetCharacterItem( pcItemData->sItem.sItemID.ToItemID(), ITEMSLOTFLAG_CostumeRight, TRUE );
					SetCharacterItem( pcItemData->sItem.sItemID.ToItemID(), ITEMSLOTFLAG_CostumeLeft, TRUE );
				}
				else
					SetCharacterItem( pcItemData->sItem.sItemID.ToItemID(), pcItemData->iVisibleItemType == ITEMSLOTFLAG_RightHand ? ITEMSLOTFLAG_CostumeRight : ITEMSLOTFLAG_CostumeLeft, TRUE );
			}
		}

		if ( auto pcItemData = HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->GetShieldItem() )
		{
			if ( pcItemData->sItem.szItemName[0] && (pcItemData->sItem.sItemID.ToItemID() == psPacket->sItem.sItemID.ToItemID()) )
				SetCharacterItem( pcItemData->sItem.sItemID.ToItemID(), pcItemData->iVisibleItemType == ITEMSLOTFLAG_RightHand ? ITEMSLOTFLAG_CostumeRight : ITEMSLOTFLAG_CostumeLeft, TRUE );
		}
	}
}

BOOL ItemHandler::SellItem( ItemData * pcItemData, int iCount )
{
	return (BOOL)CALLT_WITH_ARG2( 0x004E4090, 0x03653E40, (DWORD)pcItemData, iCount );
}

