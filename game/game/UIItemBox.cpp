#include "stdafx.h"
#include "UIItemBox.h"

namespace UI
{

ItemBox::ItemBox( int iX, int iY, int _iSlotCountX, int _iSlotCountY ) : Element(), sPosition(Point2D{iX,iY}), iSlotCountX(_iSlotCountX), iSlotCountY(_iSlotCountY)
{
	SetMaxItems( 1 );
}

ItemBox::~ItemBox()
{
	Clear();
}

void ItemBox::AddRule( ERule eRule, ETypeRule eType, int iParam )
{
	SRuleData * ps	= new SRuleData;
	ps->eRule		= eRule;
	ps->eType		= eType;
	ps->iParam		= iParam;

	vRules.push_back( ps );
}

void ItemBox::HandleEvent( ItemData * pcItemData, int iEventID )
{
	pcLastItemDataPtr = NULL;

	if ( iEventID != -1 )
	{
		pcLastItemDataPtr = pcItemData;
		UI::Event::Invoke( iEventID );
	}
}

void ItemBox::ClearItems()
{
	for ( std::vector<ItemData*>::iterator it = vItems.begin(); it != vItems.end(); it++ )
	{
		ItemData * pc = (*it);

		delete pc;
	}

	vItems.clear();
}

void ItemBox::Clear()
{
	ClearItems();

	for ( std::vector<SRuleData*>::iterator it = vRules.begin(); it != vRules.end(); it++ )
	{
		SRuleData * ps = (*it);

		delete ps;
	}

	
	vRules.clear();

	SetMaxItems( 1 );

	iSlotCountX = 4;
	iSlotCountY = 6;
}

void ItemBox::Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	iLastRenderX = iX;
	iLastRenderY = iY;

	pcLastRenderedItem = NULL;

	if ( IsOpen() )
	{
		if ( (iWidth - sPosition.iX) <= 0 )
			return;

		if ( (iHeight - sPosition.iY) <= 0 )
			return;


		int iRenderX = sPosition.iX + iX;
		int iRenderY = sPosition.iY + iY;
		int iRenderWidth = low( (iSlotCountX * 22), iWidth - sPosition.iX );
		int iRenderHeight = low( (iSlotCountY * 22), iHeight - sPosition.iY );

		for ( std::vector<ItemData*>::iterator it = vItems.begin(); it != vItems.end(); it++ )
		{
			ItemData * pc = (*it);

			if ( pc && pc->bValid )
			{
				ITEMHANDLER->DrawSprite( (pc->sPosition.iX + iRenderX), (pc->sPosition.iY + iRenderY), (int)pc->pcInventoryTexture, 0, 0, pc->iWidth, pc->iHeight );

				if ( ITEMHANDLER->IsForceEffect( pc->sItem ) )
				{
					Point2D sPositionForce = Point2D( (pc->sPosition.iX + iRenderX), (pc->sPosition.iY + iRenderY) );

					sPositionForce.iX += (pc->iWidth >> 1) - 11;
					sPositionForce.iY += (pc->iHeight >> 1) - 11;

					CGameInventory::RenderForceEffect( sPositionForce );
				}

				if ( pc->sItem.sItemID.ToItemID() == ITEMID_UnionCore )
				{
					//Get Character name
					int iLen = STRLEN( pc->sItem.szItemName );
					if ( pc->sItem.szItemName[iLen + 1] != 0 )
					{
						Point2D sPositionCore = Point2D( (pc->sPosition.iX + iRenderX), (pc->sPosition.iY + iRenderY) );
						CGameInventory::RenderUnionCoreEffect( sPositionCore );
					}
				}

				if ( pc->sItem.sItemID.ToItemBase() == ITEMBASE_Potion )
					GetGraphicsDevice()->GetDefaultFont()->Draw( FormatString( "%d", pc->sItem.iPotionCount > 0 ? pc->sItem.iPotionCount : 1 ), (pc->sPosition.iX + iRenderX), (pc->sPosition.iY + iRenderY) - 3, 0, DX::Color( 255, 255, 255 ) );

				Point2D * psPosition = MOUSEHANDLER->GetPosition();

				if ( psPosition->iX >= (pc->sPosition.iX + iRenderX) && psPosition->iX <= (pc->sPosition.iX + pc->iWidth + iRenderX) )
				{
					if ( psPosition->iY >= (pc->sPosition.iY + iRenderY) && psPosition->iY <= (pc->sPosition.iY + pc->iHeight + iRenderY) )
					{
						pc->sPosition.iX += iRenderX;
						pc->sPosition.iY += iRenderY;
						ITEMHANDLER->ShowItemInfo( pc, FALSE );
						pcLastRenderedItem = pc;
						pc->sPosition.iX -= iRenderX;
						pc->sPosition.iY -= iRenderY;
					}
				}
			}
		}

		Element::Render( iRenderX, iRenderY, iRenderWidth, iRenderHeight, iSourceX, iSourceY );
	}
}

void ItemBox::Update( float fTime )
{
	for ( auto it = vItems.begin(); it != vItems.end(); )
	{
		auto pc = (*it);
		if ( ItemInfoBox::IsExpiredItemTime( &pc->sItem ) )
		{
			ITEMHANDLER->DeleteItemForServer( pc->sItem.sItemID.ToItemID(), pc->sItem.iChk1, pc->sItem.iChk2 );

			ITEMHANDLER->ResetInventoryItemChecksum();
			CHARACTERGAME->UpdateWeight();
			ITEMHANDLER->ValidateInventoryItems();
			
			if ( iBeforeExpiredItem != -1 )
				HandleEvent( pc, iBeforeExpiredItem );

			pc->bValid = FALSE;
			DELET( pc );
			it = vItems.erase( it );

			SAVE;
		}
		else
			it++;
	}
}

BOOL ItemBox::OnMouseClick( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	iLastRenderX = iX;
	iLastRenderY = iY;

	if ( bDisableClick )
		return FALSE;

	if ( EMouseEvent::ClickDownL == pcMouse->GetEvent() )
	{
		Point2D * psPosition = pcMouse->GetPosition();

		int iRenderX = sPosition.iX + iX + iSourceX;
		int iRenderY = sPosition.iY + iY + iSourceY;
		int iRenderWidth = low( (iSlotCountX * 22), iWidth );
		int iRenderHeight = low( (iSlotCountY * 22), iHeight );

		// Inside X?
		if ( (psPosition->iX >= iRenderX) && (psPosition->iX <= (iRenderX + iRenderWidth - iSourceX)) )
		{
			// Inside Y?
			if ( (psPosition->iY >= iRenderY) && (psPosition->iY <= (iRenderY + iRenderHeight - iSourceY)) )
			{
				ItemData * pcItemData = ITEMHANDLER->GetMouseItem();

				Point2D sPositionMouse = Point2D( psPosition->iX, psPosition->iY );

				if ( bFreeSlotPlace == FALSE )
					sPositionMouse = Point2D( 0, 0 );

				if ( pcItemData->bValid )
				{
					if ( iBeforePutItemEventID != -1 )
					{
						HandleEvent( pcItemData, iBeforePutItemEventID );
					}
					else
					{
						if ( AddItem( pcItemData, iRenderX, iRenderY, sPositionMouse.iX, sPositionMouse.iY ) )
						{
							//Show Cursor
							WRITEDWORD( 0x00999308, TRUE );
							return TRUE;
						}
					}
				}
				else
				{
					if ( iBeforeGetItemEventID != -1 )
					{
						iLastRenderGetItemX = iRenderX;
						iLastRenderGetItemY = iRenderY;

						sLastItemOnMouse.iX = psPosition->iX;
						sLastItemOnMouse.iY = psPosition->iY;

						if ( pcItemData = GetItemOnPosition( psPosition, iRenderX, iRenderY ) )
							HandleEvent( pcItemData, iBeforeGetItemEventID );
					}
					else
					{
						if ( GetItem( psPosition, iRenderX, iRenderY, FALSE ) )
							return TRUE;
					}
				}
			}
		}
	}
	else if ( (EMouseEvent::ClickDownR == pcMouse->GetEvent()) && bEnableRightClick )
	{
		int iRenderX = sPosition.iX + iX + iSourceX;
		int iRenderY = sPosition.iY + iY + iSourceY;

		ItemData * pcItemData = ITEMHANDLER->GetMouseItemHover();

		if ( pcItemData && pcItemData->bValid )
		{
			if ( iBeforePutItemEventID != -1 )
			{
				HandleEvent( pcItemData, iBeforePutItemEventID );
			}
			else
			{
				AddItem( pcItemData, iRenderX, iRenderY );
				return TRUE;
			}
		}
		else
		{
			if ( iBeforeGetItemEventID != -1 )
			{
				iLastRenderGetItemX = iRenderX;
				iLastRenderGetItemY = iRenderY;

				sLastItemOnMouse.iX = pcMouse->GetPosition()->iX;
				sLastItemOnMouse.iY = pcMouse->GetPosition()->iY;
				
				if ( pcItemData = GetItemOnPosition( pcMouse->GetPosition(), iRenderX, iRenderY ) )
					HandleEvent( pcItemData, iBeforeGetItemEventID );
			}
			else
			{
				if ( GetItem( pcMouse->GetPosition(), iRenderX, iRenderY, TRUE ) )
					return TRUE;
			}
		}
	}

	return FALSE;
}

void ItemBox::OnMouseMove( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	iLastRenderX = iX;
	iLastRenderY = iY;
}

BOOL ItemBox::IsAllowedItem( ItemData * pcItemData )
{		
	BOOL bRet = FALSE;
	
	int iDisallowed	= -1;
	int iAllowed	= -1;

	auto IsRule = []( ItemData * pcItemData, ERule eRule, std::vector<SRuleData*> vRules )->int
	{	
		int iRet = -1;
	
		for ( int i = 0; i < (int)vRules.size(); i++ )
		{
			SRuleData * ps = vRules[i];
			if ( ps )
			{
				if ( ps->eRule == eRule )
				{
					if ( ps->eType == TYPERULE_EquippedItem )
					{
						BOOL bEquipped = FALSE;

						for ( int j = 0; j < 15; j++ )
						{
							if ( INVENTORYITEMSLOT[j].iItemIndex )
							{
								ItemData * psItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[j].iItemIndex - 1];

								if ( psItemData == pcItemData )
								{
									bEquipped = TRUE;
									break;
								}
							}
						}

						if ( bEquipped == ps->iParam )
							iRet = i;
					}

					if ( ps->eType == TYPERULE_ItemID )
						if ( (EItemID)ps->iParam == pcItemData->sItem.sItemID.ToItemID() )
							iRet = i;

					if ( ps->eType == TYPERULE_ItemType )
						if ( (EItemType)ps->iParam == pcItemData->sItem.sItemID.ToItemType() )
							iRet = i;

					if ( ps->eType == TYPERULE_ItemBase )
						if ( (EItemBase)ps->iParam == pcItemData->sItem.sItemID.ToItemBase() )
							iRet = i;

					if ( ps->eType == TYPERULE_AgeItem )
						if ( pcItemData->sItem.eCraftType == ITEMCRAFTTYPE_Aging )
							iRet = i;

					if ( ps->eType == TYPERULE_QuestItem )
						if ( pcItemData->sItem.eCraftType == ITEMCRAFTTYPE_Quest || pcItemData->sItem.eCraftType == ITEMCRAFTTYPE_QuestWeapon ||
							ITEMINFOBOX->IsBeginnerSet(pcItemData) )
							iRet = i;

					if ( ps->eType == TYPERULE_ItemPerfect )
						if ( pcItemData->sItem.bPerfectItem == ps->iParam )
							iRet = i;

					if ( ps->eType == TYPERULE_Level )
					{
						int iLevelItem = pcItemData->sItem.iLevel;
						if ( pcItemData->sItem.eCraftType == ITEMCRAFTTYPE_Aging && pcItemData->sItem.sAgeLevel > 0 )
						{
							int iLevelAge = Base::Coin::GetLevelItemEachAge( &pcItemData->sItem );
							if ( iLevelAge > 0 )
								iLevelItem -= (pcItemData->sItem.sAgeLevel / iLevelAge);
						}

						if ( ps->iParam == iLevelItem )
							iRet = i;
					}

					if ( ps->eType == TYPERULE_ItemTime )
						if ( ItemInfoBox::GetItemPlayTime( &pcItemData->sItem ) )
							iRet = i;

					if ( ps->eType == TYPERULE_CostumeFlag )
						if ( pcItemData->sItem.bCostumeItem )
							iRet = i;

					if ( ps->eType == TYPERULE_LockItemGM )
						if ( pcItemData->sItem.bLockedItemType && ItemInfoBox::GetItemPlayTime( &pcItemData->sItem ) )
							iRet = i;
				}
			}
		}

		if ( vRules.size() == 0 )
			return 8000;

		return iRet;
	};

	iAllowed	= IsRule( pcItemData, RULE_Allow, vRules );
	iDisallowed	= IsRule( pcItemData, RULE_Disallow, vRules );

	//Allowed or not in Disallowed Rule? Allow to set...
	if ( (iAllowed != -1) || (iDisallowed != -1) )
	{
		if ( (iAllowed == 8000) && (iDisallowed == 8000) )
			bRet = TRUE;
		else
		{
			if ( iAllowed > iDisallowed )
				bRet = TRUE;
			else
				bRet = FALSE;
		}
	}
	else
	{
		int iAllow = 0;
		int iDisallow = 0;
		//Get Counters
		for ( auto p : vRules )
		{
			if ( p->eRule == RULE_Allow )
				iAllow++;
			else
				iDisallow++;
		}

		//Have just specified disallow
		if ( (iAllow == 0) && (iDisallow > 0) )
			bRet = TRUE;
		//Have just specified allow
		else if ( (iDisallow == 0) && (iAllow > 0) )
			bRet = FALSE;
	}

	return bRet;
}

BOOL ItemBox::AddItem( ItemData * pcItemData )
{
	int iItemX = bFreeSlotPlace ? pcItemData->sPosition.iX : 0;
	int iItemY = bFreeSlotPlace ? pcItemData->sPosition.iY : 0;

	BOOL bRet = AddItem( pcItemData, iLastRenderX, iLastRenderY, iItemX, iItemY );
	if ( bRet )
	{
		//Show Cursor
		WRITEDWORD( 0x00999308, TRUE );
	}

	return bRet;
}


BOOL ItemBox::AddItem(ItemData * pcItemData, int iX, int iY, int iMouseX, int iMouseY )
{

	if ( !IsAllowedItem( pcItemData ) )
	{
		//Event
		HandleEvent( pcItemData, iDeniedItemEventID );
		return FALSE;
	}

	//Event
	HandleEvent( pcItemData, iAllowedItemEventID );

	Rectangle2D sBox( iX, iY, iSlotCountX * 22, iSlotCountY * 22 );

	if ( (vItems.size() >= (UINT)iMaxItems) && ((iMaxItems == 1) && (bSwapItemSlot == FALSE)) )
		return FALSE;

	if ( iMaxItems > 1 )
	{
		int iPlaceX = -1;
		int iPlaceY = -1;

		//Check Collision
		for ( int i = 0; i < iSlotCountY; i++ )
		{
			BOOL bDone = FALSE;

			for ( int j = 0; j < iSlotCountX; j++ )
			{
				int iRelativeX = (j * 22);
				int iRelativeY = (i * 22);

				RECT rPlaceRect;
				rPlaceRect.left = iRelativeX;
				rPlaceRect.top = iRelativeY;
				rPlaceRect.right = rPlaceRect.left + pcItemData->iWidth;
				rPlaceRect.bottom = rPlaceRect.top + pcItemData->iHeight;

				int iCollideX = rPlaceRect.right + sBox.iX - 1;
				int iCollideY = rPlaceRect.bottom + sBox.iY - 1;

				if ( !sBox.Inside( &Point2D( iCollideX, iCollideY ) ) )
					continue;

				if ( iMouseX > 0 || iMouseY > 0 )
				{
					int iDifferenceX = (iMouseX - iX) - (pcItemData->iWidth >> 1);
					int iDifferenceY = (iMouseY - iY) - (pcItemData->iHeight >> 1);

					if ( iDifferenceX < 0 )
						iDifferenceX = 0;

					if ( iDifferenceY < 0 )
						iDifferenceY = 0;

					Rectangle2D sBoxFree( iX, iY, (iSlotCountX * 22), (iSlotCountY * 22) );

					if ( bNoCheckSlot )
					{
						rPlaceRect.left = iMouseX;
						rPlaceRect.top = iMouseY;

						rPlaceRect.right = rPlaceRect.left + pcItemData->iWidth;
						rPlaceRect.bottom = rPlaceRect.top + pcItemData->iHeight;
					}
					else if ( sBoxFree.Inside( &Point2D( iMouseX, iMouseY ) ) )
					{
						if ( sBoxFree.Inside( &Point2D( iX + iDifferenceX + pcItemData->iWidth - 21, iY + iDifferenceY + pcItemData->iHeight - 21 ) ) )
						{
							rPlaceRect.left = (iDifferenceX / 22) * 22;
							rPlaceRect.top = (iDifferenceY / 22) * 22;

							rPlaceRect.right = rPlaceRect.left + pcItemData->iWidth;
							rPlaceRect.bottom = rPlaceRect.top + pcItemData->iHeight;
						}
						else
							continue;
					}
				}

				BOOL bCollides = FALSE;

				for ( std::vector<ItemData*>::iterator it = vItems.begin(); it != vItems.end(); it++ )
				{
					ItemData * pc = (*it);

					if ( pc )
					{
						if ( pc->bValid == FALSE )
							continue;

						RECT rItemRect;
						rItemRect.left = pc->sPosition.iX;
						rItemRect.top = pc->sPosition.iY;
						rItemRect.right = rItemRect.left + pc->iWidth - 1;
						rItemRect.bottom = rItemRect.top + pc->iHeight - 1;

						if ( rPlaceRect.left < rItemRect.right && rPlaceRect.right > rItemRect.left &&
							rPlaceRect.top < rItemRect.bottom && rPlaceRect.bottom > rItemRect.top )
						{
							bCollides = TRUE;
							break;
						}
					}
				}

				if ( bCollides )
					continue;

				iPlaceX = rPlaceRect.left;
				iPlaceY = rPlaceRect.top;

				bDone = TRUE;
				break;
			}

			if ( bDone )
				break;
		}

		if ( iPlaceY == (-1) )
			return FALSE;

		ItemData * psSlot = new ItemData;
		CopyMemory( psSlot, pcItemData, sizeof( ItemData ) );

		if ( bKeepItemValid && ITEMHANDLER->GetMouseItem() == pcItemData )
		{
			pcItemData->sPosition.iX = 23;
			pcItemData->sPosition.iY = 436;
			pcItemData->sPlacePosition.iX = 23;
			pcItemData->sPlacePosition.iY = 436;
			pcItemData->iItemSlot = 0;
			ItemID iItemID = pcItemData->sBaseItemID;
			pcItemData->sBaseItemID.SetItemID( (EItemID)(pcItemData->sBaseItemID.ToInt() + 1) );
			ITEMHANDLER->BackItemToInventory( pcItemData, ITEMCHECKINVENTORY_None, false );

			ItemData * pcItemDataOld = CGameInventory::GetInstance()->GetInventoryItemIndexEx( pcItemData->sItem.sItemID.ToInt(), pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 );
			if ( pcItemDataOld )
				pcItemDataOld->sBaseItemID.SetItemID( iItemID.ToItemID() );

			pcItemData->bValid = FALSE;
		}
		else
			pcItemData->bValid = bKeepItemValid;

		psSlot->sPosition.iX = iPlaceX;
		psSlot->sPosition.iY = iPlaceY;

		if( bDisableSound == FALSE )
			PLAYMINISOUND( pcItemData->iInvItemSound );

		vItems.push_back( psSlot );

		// Validate Inventory
		ITEMHANDLER->ValidateInventoryItems();
		ITEMHANDLER->ResetInventoryItemChecksum();

		CHARACTERGAME->UpdateWeight();

		//Event
		HandleEvent( psSlot, iPutItemEventID );

		return TRUE;

	}
	else
	{
		ItemData * psSlot = new ItemData;
		CopyMemory( psSlot, pcItemData, sizeof( ItemData ) );

		psSlot->sPosition.iX = (sBox.iWidth >> 1) - (psSlot->iWidth >> 1);
		psSlot->sPosition.iY = (sBox.iHeight >> 1) - (psSlot->iHeight >> 1);

		if ( bDisableSound == FALSE )
			PLAYMINISOUND( pcItemData->iInvItemSound );

		vItems.push_back( psSlot );

		ZeroMemory( pcItemData, sizeof( ItemData ) );

		if ( bSwapItemSlot && (vItems.size() == 2) && (pcItemData == ITEMHANDLER->GetMouseItem()) )
		{
			CopyMemory( pcItemData, vItems[0], sizeof( ItemData ) );

			DELET( vItems[0] );

			auto it = vItems.begin();

			it = vItems.erase( it );
		}

		// Validate Inventory
		ITEMHANDLER->ValidateInventoryItems();
		ITEMHANDLER->ResetInventoryItemChecksum();

		CHARACTERGAME->UpdateWeight();

		//Event
		HandleEvent( psSlot, iPutItemEventID );

		return TRUE;
	}

	return FALSE;
}

BOOL ItemBox::GetItem( Point2D * psPosition, int iX, int iY, BOOL bAutomatic )
{
	BOOL bCollides = FALSE;

	for ( std::vector<ItemData*>::iterator it = vItems.begin(); it != vItems.end(); )
	{
		ItemData * pc = (*it);

		if ( pc )
		{
			if ( pc->bValid == FALSE )
			{
				it++;
				continue;
			}

			if ( psPosition->iX >= (pc->sPosition.iX + iX) && psPosition->iX <= (pc->sPosition.iX + iX + pc->iWidth) )
			{
				if ( psPosition->iY >= (pc->sPosition.iY + iY) && psPosition->iY <= (pc->sPosition.iY + iY + pc->iHeight) )
				{
					if ( bKeepItemValid == FALSE )
					{
						// Check Space
						if ( !CHARACTERGAME->CheckInventorySpace( pc ) )
						{
							TITLEBOX( "Not enough space in inventory" );
							return TRUE;
						}


						// Put Item Back to Inventory
						if ( bAutomatic == TRUE )
						{
							ITEMHANDLER->BackItemToInventory( pc );
						}
						else
						{
							CopyMemory( ITEMHANDLER->GetMouseItem(), pc, sizeof( ItemData ) );

							ITEMHANDLER->GetMouseItem()->iItemSlot = 0;

							//Hide Cursor
							WRITEDWORD( 0x00999308, FALSE );
						}
						ITEMHANDLER->ResetInventoryItemChecksum();
					}

					// Invalidate Item
					pc->bValid = FALSE;

					// iSound
					if ( bDisableSound == FALSE )
						PLAYMINISOUND( pc->iInvItemSound );

					//Event
					HandleEvent( pc, iGetItemEventID );
					DELET( pc );
					it = vItems.erase( it );
				}
				else 
					it++;
			}
			else
				it++;
		}
		else
			it++;
	}

	return TRUE;
}

BOOL ItemBox::GetLastItemOnMouse( BOOL bAutomatic )
{
	return GetItem( &sLastItemOnMouse, iLastRenderGetItemX, iLastRenderGetItemY, bAutomatic );
}

BOOL ItemBox::RemoveItem( int iItemID, int iChk1, int iChk2 )
{
	BOOL bRet = FALSE;
	for ( std::vector<ItemData*>::iterator it = vItems.begin(); it != vItems.end(); )
	{
		ItemData * pc = (*it);

		if ( pc )
		{
			if ( pc->bValid == FALSE )
			{
				it++;
				continue;
			}

			if ( (pc->sItem.sItemID.ToInt() == iItemID) && (pc->sItem.iChk1 == iChk1) && (pc->sItem.iChk2 == iChk2) )
			{
				DELET( pc );
				it = vItems.erase( it );
				bRet = TRUE;
			}
			else
				it++;
		}
		else
			it++;
	}

	return bRet;
}

ItemData * ItemBox::GetItemData( int iItemID, int iChk1, int iChk2 )
{
	for ( auto pc : vItems )
	{
		if ( pc->bValid )
		{
			if ( (pc->sItem.sItemID.ToInt() == iItemID) && (pc->sItem.iChk1 == iChk1) && (pc->sItem.iChk2 == iChk2) )
				return pc;
		}
	}
	return nullptr;
}

ItemData * ItemBox::GetItemOnPosition( Point2D * psPosition, int iX, int iY )
{
	for ( std::vector<ItemData*>::iterator it = vItems.begin(); it != vItems.end(); )
	{
		ItemData * pc = (*it);
		if ( pc )
		{
			if ( pc->bValid == FALSE )
			{
				it++;
				continue;
			}

			if ( psPosition->iX >= (pc->sPosition.iX + iX) && psPosition->iX <= (pc->sPosition.iX + iX + pc->iWidth) )
			{
				if ( psPosition->iY >= (pc->sPosition.iY + iY) && psPosition->iY <= (pc->sPosition.iY + iY + pc->iHeight) )
				{
					return pc;
				}
				else
					it++;
			}
			else
				it++;
		}
		else it++;
	}

	return nullptr;
}

}