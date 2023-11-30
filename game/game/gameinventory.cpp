#include "stdafx.h"
#include "gameinventory.h"


CGameInventory::CGameInventory()
{
}


CGameInventory::~CGameInventory()
{
}

NAKED int CGameInventory::CheckMousePotionCount()
{
	JMP( pfnCheckMousePotionCount );
}

NAKED void CGameInventory::OnMouseLDownClick( int iX, int iY )
{
	JMP( pfnOnMouseLClickDown );
}

NAKED BOOL CGameInventory::CanChangeSpecialItem( BOOL b )
{
	JMP( 0x004B1130 );
}

BOOL CGameInventory::ChangeInventorySide( int iType )
{

	//Delay Time for change window
	WRITEDWORD( 0x035EB6E0, TRUE );

	if ( AGEPACKET_SENDING )
		return TRUE;
	if ( NOTCHANGE_ITEM )
		return TRUE;
	if ( PERSONALSHOP_SENDING )
		return TRUE;
	if ( PERSONALSHOPHANDLER->GetWindow()->IsOpen() )
	{
		if ( ITEMONMOUSE )
			return TRUE;

		if ( PERSONALSHOPHANDLER->GetWindow()->IsSellingTime() )
			return TRUE;

		if ( PERSONALSHOPHANDLER->GetWindow()->IsSeller() )
			return TRUE;
	}

	if ( CanChangeSpecialItem( TRUE ) )
		return TRUE;

	if ( WAREHOUSEHANDLER->IsWarehouseWindowOpen() )
	{
	//	if ( ITEMONMOUSE )
	//		return TRUE;
	}

	//Teleport Core is open?
	if ( TELEPORTCOREHANDLER->IsOpen() )
		return TRUE;

	//Tier Up Box Open?
	if ( READDWORD( 0x0095E418 ) )
		return TRUE;

	if ( SHOWSPECIAL_ITEM )
		return TRUE;

	//Teleport Core Open?
	if ( READDWORD( 0x0095F8E0 ) )
		return TRUE;

	if ( CRAFTPROCCESS_START || BRIGHTEFFECT_ITEM_PROCESSING )
		return TRUE;
	else
	{
		if ( (CRAFTWINDOW_OPEN || SMELTWINDOW_OPEN) && RESETMIXWINDOW_DELAY )
			return TRUE;
	}

	int iSlot = 1;

	if ( iType > 2 )
		iSlot = 2;

	if ( iType == 1 )
	{
		ARROWINVENTORY_LEFT = FALSE;
		PLAYERDATA->sViewBoxZoom.sMin = 1;
	}
	if ( iType == 2 )
	{
		ARROWINVENTORY_LEFT = TRUE;
		PLAYERDATA->sViewBoxZoom.sMin = 2;
	}
	if ( iType == 3 )
	{
		ARROWINVENTORY_RIGHT = FALSE;
		PLAYERDATA->sViewBoxZoom.sMax = 3;
	}
	if ( iType == 4 )
	{
		ARROWINVENTORY_RIGHT = TRUE;
		PLAYERDATA->sViewBoxZoom.sMax = 4;
	}

	if ( iSlot == 1 )
	{
		CopyMemory( &saBackupInventoryItemTemp, &saInventoryItemTemp, sizeof( ItemData ) * 100 );
		
		for ( int i = 0; i < INVENTORY_DATACOUNT; i++ )
		{
			//Fix
			if ( i >= INVENTORYSLOT_WHEEL && i < INVENTORYSLOT_WHEELCOUNT )
				continue;

			saInventoryItemTemp[i].bValid = FALSE;
		}

		for ( int i = 0; i < INVENTORY_DATACOUNT; i++ )
		{
			//Fix
			if ( i >= INVENTORYSLOT_WHEEL && i < INVENTORYSLOT_WHEELCOUNT )
				continue;

			ItemData * pc = &saInventoryItems[i];

			if ( pc->bValid )
			{
				if ( (pc->iItemSlot == 0) && (pc->sItem.sSpecialItemType[1] != 8) )
				{
					for ( int j = 0; j < INVENTORY_DATACOUNT; j++ )
					{
						//Fix
						if ( j >= INVENTORYSLOT_WHEEL && j < INVENTORYSLOT_WHEELCOUNT )
							continue;

						ItemData * pc2 = &saInventoryItemTemp[j];

						if ( pc2->bValid == FALSE )
						{
							CopyMemory( pc2, pc, sizeof( ItemData ) );
							pc->bValid = FALSE;
							break;
						}
					}
				}
			}
		}

		for ( int i = 0; i < INVENTORY_DATACOUNT; i++ )
		{
			//Fix
			if ( i >= INVENTORYSLOT_WHEEL && i < INVENTORYSLOT_WHEELCOUNT )
				continue;

			ItemData * pc = &saBackupInventoryItemTemp[i];

			if ( pc->bValid )
			{
				for ( int j = 0; j < INVENTORY_DATACOUNT; j++ )
				{
					//Fix
					if ( j >= INVENTORYSLOT_WHEEL && j < INVENTORYSLOT_WHEELCOUNT )
						continue;

					ItemData * pc2 = &saInventoryItems[j];

					if ( pc2->bValid == FALSE )
					{
						CopyMemory( pc2, pc, sizeof( ItemData ) );
						pc->bValid = FALSE;
						break;
					}
				}
			}
		}
	}

	if ( iSlot == 2 )
	{
		//Skill
		WRITEDWORD( 0x03707138, TRUE );

		CopyMemory( &saBackupInventoryItemTemp[INVENTORYSLOT_WHEEL], &saInventoryItemTemp[INVENTORYSLOT_WHEEL], sizeof( ItemData ) * 2 );

		for ( int i = INVENTORYSLOT_WHEEL; i < INVENTORYSLOT_WHEELCOUNT; i++ )
		{
			saInventoryItemTemp[i].bValid = FALSE;
		}


		for ( int i = 0; i < INVENTORY_DATACOUNT; i++ )
		{
			ItemData * pc = &saInventoryItems[i];

			if ( pc->bValid )
			{
				if ( (pc->iItemSlot == 1) || (pc->iItemSlot == 2) )
				{
					for ( int j = INVENTORYSLOT_WHEEL; j < INVENTORYSLOT_WHEELCOUNT; j++ )
					{
						ItemData * pc2 = &saInventoryItemTemp[j];

						if ( pc2->bValid == FALSE )
						{
							CopyMemory( pc2, pc, sizeof( ItemData ) );
							pc->bValid = FALSE;
						
							INVENTORYITEMSLOT[pc->iItemSlot - 1].iItemIndex = 0;

							if ( UNITDATA->sCharacterData.iClass == CHARACTERCLASS_Assassin && pc->sBaseItemID.ToItemType() == ITEMTYPE_Dagger )
							{
								ITEMHANDLER->SetCharacterItem( pc->sBaseItemID.ToItemID(), 2, FALSE );
								ITEMHANDLER->SetCharacterItem( pc->sBaseItemID.ToItemID(), 4, FALSE );
							}
							else 
								ITEMHANDLER->SetCharacterItem( pc->sBaseItemID.ToItemID(), pc->iVisibleItemType, FALSE );

							break;
						}
					}
				}
			}
		}

		for ( int i = INVENTORYSLOT_WHEEL; i < INVENTORYSLOT_WHEELCOUNT; i++ )
		{
			ItemData * pc = &saBackupInventoryItemTemp[i];

			if ( pc->bValid )
			{
				for ( int j = 0; j < INVENTORY_DATACOUNT; j++ )
				{
					//Fix
					if ( j >= INVENTORYSLOT_WHEEL && j < INVENTORYSLOT_WHEELCOUNT )
						continue;

					ItemData * pc2 = &saInventoryItems[j];

					if ( pc2->bValid == FALSE )
					{
						CopyMemory( pc2, pc, sizeof( ItemData ) );
						pc->bValid = FALSE;
						
						INVENTORYITEMSLOT[pc->iItemSlot - 1].iItemIndex = j + 1;

						if ( UNITDATA->sCharacterData.iClass == CHARACTERCLASS_Assassin && pc->sBaseItemID.ToItemType() == ITEMTYPE_Dagger )
						{
							ITEMHANDLER->SetCharacterItem( pc->sBaseItemID.ToItemID(), 2, TRUE );
							ITEMHANDLER->SetCharacterItem( pc->sBaseItemID.ToItemID(), 4, TRUE );
						}
						else
							ITEMHANDLER->SetCharacterItem( pc->sBaseItemID.ToItemID(), pc->iVisibleItemType, TRUE );

						if ( pc->iInvItemSound )
							PLAYMINISOUND( pc->iInvItemSound );

						break;
					}
				}
			}
		}

		ITEMHANDLER->UpdateItemCharacterStatus();
	}

	if ( PERSONALSHOP_OPEN )
	{
		int * piaPersonalShopItemIndex = ((int*)0x03653C50);
		ZeroMemory( piaPersonalShopItemIndex, sizeof( int ) * 100 );

		ItemData * psaPersonalShopItemData = (ItemData*)0x0363C114;

		for ( int i = 0; i < INVENTORY_DATACOUNT; i++ )
		{
			ItemData * pc = &saInventoryItems[i];

			if ( pc->bValid )
			{
				const auto vItems = PERSONALSHOPHANDLER->GetWindow()->GetItems();
				for ( const auto & pcItemShop : vItems )
				{
					if ( (pcItemShop->sItem.iChk1 == pc->sItem.iChk1) && (pcItemShop->sItem.iChk2 == pc->sItem.iChk2) )
					{
						piaPersonalShopItemIndex[i] = 1;
						break;
					}
				}
			}
		}
	}

	ITEMHANDLER->CheckRequirements();
	ITEMHANDLER->CheckPuzzleItemEvent();

	if ( (READDWORD( 0x03625718 ) == TRUE) && ((iType == 3) || (iType == 4)) )
	{
		HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->SwapInventory( TRUE );
	}
	
	return TRUE;
}

int CGameInventory::GetInventoryPersonalShopItem( ItemData * pcItemData )
{
	int iIndex = 0;
	for ( int i = 0; i < INVENTORY_DATACOUNT; i++ )
	{
		ItemData * pc = saInventoryItems + i;
		if ( pc->bValid )
		{
			if ( (pc->sItem.iChk1 == pcItemData->sItem.iChk1) && (pc->sItem.iChk2 == pcItemData->sItem.iChk2) )
			{
				if ( pcItemData->sItem.iIndex == (i + 1) )
					iIndex = i + 1;
				break;
			}
		}

		pc = saInventoryItemTemp + i;
		if ( pc->bValid )
		{
			if ( (pc->sItem.iChk1 == pcItemData->sItem.iChk1) && (pc->sItem.iChk2 == pcItemData->sItem.iChk2) )
			{
				iIndex = i + 101;
				break;
			}
		}
	}

	return iIndex;
}

int CGameInventory::GetInventoryItemIndex( ItemData * pcItemData )
{
	int iIndex = -1;
	for ( int i = 0; i < INVENTORY_DATACOUNT; i++ )
	{
		ItemData * pc = saInventoryItems + i;
		if ( pc->bValid )
		{
			if ( (pc->sItem.iChk1 == pcItemData->sItem.iChk1) && (pc->sItem.iChk2 == pcItemData->sItem.iChk2) )
			{
				iIndex = i;
				break;
			}
		}
	}

	return iIndex;
}

ItemData * CGameInventory::GetInventoryItemIndexEx( int iItemID, int iChk1, int iChk2 )
{
	ItemData * pcItemData = nullptr;

	for ( int i = 0; i < INVENTORY_DATACOUNT; i++ )
	{
		ItemData * pc = saInventoryItems + i;
		if ( pc->bValid )
		{
			if ( (pc->sItem.sItemID.ToInt() == iItemID) && (pc->sItem.iChk1 == iChk1) && (pc->sItem.iChk2 == iChk2) )
			{
				pcItemData = pc;
				break;
			}
		}
	}

	if ( pcItemData == nullptr )
	{
		for ( int i = 0; i < INVENTORY_DATACOUNT; i++ )
		{
			ItemData * pc = saInventoryItemTemp + i;
			if ( pc->bValid )
			{
				if ( (pc->sItem.sItemID.ToInt() == iItemID) && (pc->sItem.iChk1 == iChk1) && (pc->sItem.iChk2 == iChk2) )
				{
					pcItemData = pc;
					break;
				}
			}
		}
	}

	return pcItemData;
}

BOOL CGameInventory::GetInventoryItem( int iX, int iY, int iPickup )
{
	//Point2D sCurMouse = Point2D(MOUSEHANDLER->GetPosition()->iX, MOUSEHANDLER->GetPosition()->iY);
	Point2D sCurMouse = Point2D( iX, iY );

	WRITEDWORD( 0x35EB684, NULL );
	WRITEDWORD( 0x35EB65C, NULL );
	WRITEDWORD( 0x35EB660, NULL );

	for( size_t i = 0; i < 100; i++ )
	{
		ItemData * pcItemData = saInventoryItems + i;

		if( pcItemData )
		{
			if( pcItemData->bValid )
			{
				if ( READDWORD( 0x03625718 ) == TRUE )
				{
					if ( (pcItemData->iItemSlot == ITEMSLOT_RightHand) || (pcItemData->iItemSlot == ITEMSLOT_LeftHand) || (pcItemData->iItemSlot == ITEMSLOT_Armor) )
						continue;
				}
				else
				{
					if ( pcItemData->iItemSlot == ITEMSLOT_Costume )
						continue;
				}

				if( sCurMouse.Inside( Rectangle2D( pcItemData->sPosition.iX, pcItemData->sPosition.iY, pcItemData->iWidth, pcItemData->iHeight ) ) )
				{
					//Potions click while inventory is closed, probably remove it later...
					if ( this->bOpen == FALSE )
						if( (pcItemData->iItemSlot != ITEMSLOT_PotionOne) && (pcItemData->iItemSlot != ITEMSLOT_PotionTwo) && (pcItemData->iItemSlot != ITEMSLOT_PotionThree) )
							return FALSE;

					if( iPickup == 1 )
					{
						int * piaPersonalShopItemIndex = ((int *)0x03653C50);
						if ( piaPersonalShopItemIndex[i] )
							return FALSE;

						if ( pcItemData->iItemSlot )
						{
							if ( UNITDATA->sCharacterData.sWeight.sCur > UNITDATA->sCharacterData.sWeight.sMax )
							{
								TITLEBOX( AY_OBFUSCATE( "You're overweight!" ) );
								return FALSE;
							}

							if ( ACCOUNTHANDLER->IsAccountShareDenied( ACCOUNTSHARE_DenyGrabEquipment ) && (GM_MODE == FALSE) )
							{
								TITLEBOX( AY_OBFUSCATE( "You can't Grab Equipments, because you're logged in with the Share PW!" ) );
								return FALSE;
							}
						}

						//Repair Items
						if ( MOUSE_ICON == EMOUSEICON_Repair )
						{
							ItemPriceData sPrice;
							GetItemPrice( &sPrice, pcItemData );
							if ( RepairItem( pcItemData, sPrice.iRepairCost ) )
								PLAYMINISOUND( 16 );

							return TRUE;
						}


						pcItemData->sOldPosition.iX = pcItemData->sPosition.iX;
						pcItemData->sOldPosition.iY = pcItemData->sPosition.iY;

						//Potion Index (?)
						WRITEDWORD( 0x035EB678, i + 1 );

						//Backup Item Index (?)
						WRITEDWORD( 0x035EB680, i + 1 );

						if( MOUSE_ICON == EMOUSEICON_Sell )
						{
							if ( pcItemData->sItem.iChk2 == 0 )
								return TRUE;

							if ( ITEMWINDOWHANDLER->IsSellItem( pcItemData ) == FALSE )
								return TRUE;

							if ( (pcItemData->iItemSlotFlag == ITEMSLOTFLAG_Potion) || (pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Gift) )
									return TRUE;
							else
							{
								if ( ITEMHANDLER->SellItem( pcItemData ) == FALSE )
									return TRUE;
							}
						}
						else
						{
							if( pcItemData->iItemSlotFlag == ITEMSLOTFLAG_LeftRightHand )
								GetInventoryTwoHandItem( pcItemData );

							CopyMemory( ITEMHANDLER->GetMouseItem(), pcItemData, sizeof(ItemData) );
						}

						pcItemData->bValid = FALSE;

						if( pcItemData->iItemSlot )
						{
							if( UNITDATA->sCharacterData.iClass == CHARACTERCLASS_Assassin )
							{
								ITEMHANDLER->SetCharacterItem( pcItemData->sBaseItemID.ToItemID(), ITEMSLOTFLAG_LeftHand, FALSE );
								ITEMHANDLER->SetCharacterItem( pcItemData->sBaseItemID.ToItemID(), ITEMSLOTFLAG_RightHand, FALSE );
							}
							else
							{
								ITEMHANDLER->SetCharacterItem( pcItemData->sBaseItemID.ToItemID(), pcItemData->iVisibleItemType, FALSE );
							}

							INVENTORYITEMSLOT[pcItemData->iItemSlot - 1].iItemIndex = 0;
						}

						ITEMHANDLER->ValidateInventoryItems();
						CHARACTERGAME->UpdateWeight();
						ITEMHANDLER->UpdateItemCharacterStatus();
						PLAYMINISOUND( pcItemData->iInvItemSound );
						WRITEDWORD( 0x35EB5F4, 0 );

						return TRUE;
					}
					else if ( iPickup == 2 ) //Personal Shop, obsolete...
					{

					}
					else
					{
						//Potion Index
						if ( pcItemData->iItemSlotFlag == ITEMSLOTFLAG_Potion )
							if ( pcItemData->iItemSlot == 0 )
								WRITEDWORD( 0x35EB660, i + 1 );

						WRITEDWORD( 0x035EB65C, i + 1 );
						WRITEDWORD( 0x035EB5F4, 5 );
						WRITEDWORD( 0x036286EC, 2 );

						extern BOOL __stdcall PrepareItemInfo( ItemData * psItemData, int iFlag, int iIndex );

						PrepareItemInfo( pcItemData, 2, 0 );
					}
				}
			}
		}
	}

	return FALSE;
}

NAKED BOOL CGameInventory::GetInventoryTwoHandItem( ItemData * pcItem )
{
	JMP( pfnGetInventoryTwoHandItem );
}

BOOL CGameInventory::SetInventoryItem( ItemData * psItem )
{
	if( psItem->sBaseItemID.ToItemBase() == ITEMBASE_Potion )
	{
		for( size_t i = 0; i < 3; i++ )
		{
			if( INVENTORYITEMSLOT[10 + i].sBox.Inside( MOUSEHANDLER->GetPosition() ) )
				return TRUE;
		}

		ITEMHANDLER->BackItemToInventory( psItem );
	}
	else
		ITEMHANDLER->BackItemToInventory( psItem );

	return TRUE;
}

NAKED BOOL CGameInventory::SetInventoryItem( Item * psItem )
{
	JMP( pfnSetInventoryItemItem );
}

BOOL CGameInventory::PlaceItemInventory( Item * psItem )
{
	SHOWSPECIAL_ITEM = FALSE;
	
	//Set Special Item Type
	CALL_WITH_ARG1( 0x00505CB0, (DWORD)psItem );

	//Message Item
	WRITEDWORD( 0x0362A3CC, FALSE );

	auto & sWeight = UNITDATA->sCharacterData.sWeight;

	ItemData cItemDataOut;
	if ( ITEMHANDLER->LoadItemImage( psItem, &cItemDataOut ) )
	{
		if ( cItemDataOut.sItem.iPotionCount <= 0 )
			cItemDataOut.sItem.iPotionCount = 1;

		if ( ITEMHANDLER->GetMouseItem()->bValid || ISBIGMAP )
		{
			if ( IsValidRequirements( &cItemDataOut ) )
			{
				if ( ITEMHANDLER->BackItemToInventory( &cItemDataOut ) == FALSE )
				{
					if ( ITEMHANDLER->DropItemToMap( &cItemDataOut ) )
						cItemDataOut.bValid = FALSE;
				}
				else
				{
					ITEMHANDLER->UpdateItemCharacterStatus();
					CHARACTERGAME->UpdateWeight();
				}
			}
		}
		else
		{
			if ( WINDOW_INVENTORY )
			{
				int iWeight = cItemDataOut.sItem.iWeight;
				if ( cItemDataOut.iItemSlotFlag == ITEMSLOTFLAG_Potion )
					iWeight = cItemDataOut.sItem.iPotionCount;

				if ( (iWeight + (int)sWeight.sCur) > (int)sWeight.sMax )
				{
					TITLEBOX( "" );
					INVENTORY_OVERWEIGHT++;
					ITEMHANDLER->DropItemToMap( &cItemDataOut );
					return FALSE;
				}

				IsValidRequirements( &cItemDataOut );
				CopyMemory( ITEMHANDLER->GetMouseItem(), &cItemDataOut, sizeof( ItemData ) );
			}
			else
			{
				if ( IsValidRequirements( &cItemDataOut ) )
				{
					if ( ITEMHANDLER->BackItemToInventory( &cItemDataOut ) == FALSE )
					{
						if ( ITEMHANDLER->DropItemToMap( &cItemDataOut ) )
							cItemDataOut.bValid = FALSE;
					}
					else
					{
						ITEMHANDLER->UpdateItemCharacterStatus();
						CHARACTERGAME->UpdateWeight();
					}
				}
			}
		}
	}

	if ( ITEMHANDLER->GetMouseItem()->bValid )
		MOUSE_ICON = EMOUSEICON_None;
	else
		MOUSE_ICON = EMOUSEICON_Default;

	SAVE;
	return TRUE;
}

NAKED BOOL CGameInventory::IsValidRequirements( ItemData * pcItemData )
{
	JMP( 0x004ACA90 );
}

NAKED BOOL CGameInventory::LoadItemImageEXPGold( ItemData * pcItemData )
{
	JMP( pfnLoadItemImageEXPGold );
}

NAKED BOOL CGameInventory::ResetPotion()
{
	JMP( pfnResetPotion );
}

void CGameInventory::RenderForceEffect( Point2D sPosition )
{
	CALLT_WITH_ARG2( 0x004B25E0, (DWORD)CGameInventory::GetInstance(), sPosition.iX, sPosition.iY );
}

void CGameInventory::RenderUnionCoreEffect( Point2D sPosition )
{
	int * piaCoreEffect = (int*)0x035E5A3C;
	int iCoreEffectTime = READDWORD( 0x035EB6D4 );

	ITEMHANDLER->DrawSprite( sPosition.iX, sPosition.iY, piaCoreEffect[iCoreEffectTime / 10], 0, 0, 22, 44 );
}

NAKED BOOL CGameInventory::PlaceQuestItem( Item * psItem )
{
	JMP( 0x0050A0B0 );
}

void CGameInventory::DropInventoryItemsOnMap()
{
	auto DropItem = []( ItemData * pcItemData ) -> void
	{
		Packet * psPacketItem = ((Packet *)0x03A46840);
		psPacketItem->iHeader = 0;

		if ( ITEMHANDLER->IsValidItem( &pcItemData->sItem ) )
		{
			ITEMHANDLER->DropItemToMap( pcItemData );

			if ( psPacketItem->iHeader )
			{
				SENDPACKETG( psPacketItem );
				psPacketItem->iHeader = 0;
			}
		}
	};

	for ( int i = 0; i < _countof( saInventoryItems ); i++ )
	{
		ItemData * pcItemData = saInventoryItems + i;
		if ( pcItemData->bValid )
			DropItem( pcItemData );
	}

	for ( int i = 0; i < _countof( saInventoryItemTemp ); i++ )
	{
		ItemData * pcItemData = saInventoryItemTemp + i;
		if ( pcItemData->bValid )
			DropItem( pcItemData );
	}

	for ( int i = 0; i < 14; i++ )
		INVENTORYITEMSLOT[i].iItemIndex = 0;

	if ( ITEMHANDLER->GetMouseItem() && ITEMHANDLER->GetMouseItem()->bValid )
		DropItem( ITEMHANDLER->GetMouseItem() );

	ZeroMemory( CGameInventory::GetInstance()->saInventoryItems, sizeof( ItemData ) * INVENTORY_DATACOUNT );
	ZeroMemory( CGameInventory::GetInstance()->saInventoryItemTemp, sizeof( ItemData ) * INVENTORY_DATACOUNT );
	ZeroMemory( CGameInventory::GetInstance()->saBackupInventoryItemTemp, sizeof( ItemData ) * INVENTORY_DATACOUNT );
	ZeroMemory( ITEMHANDLER->GetMouseItem(), sizeof( ItemData ) );

	ITEMHANDLER->ValidateInventoryItems();
	ITEMHANDLER->UpdateItemCharacterStatus();
	CHARACTERGAME->UpdateWeight();

	CHECK_CHARACTER_CHECKSUM;

	SAVE;
}

NAKED void CGameInventory::GetItemPrice( ItemPriceData * psItemPrice, ItemData * pcItemData )
{
	JMP( 0x004AD160 );
}

NAKED BOOL CGameInventory::RepairItem( ItemData * pcItemData, int iRepairCost )
{
	JMP( 0x004AD250 );
}
