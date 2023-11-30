#include "stdafx.h"
#include "MixHandler.h"


MixHandler::MixHandler()
{
}


MixHandler::~MixHandler()
{
}

void MixHandler::HandlePacket( PacketMixServer * psPacket )
{
	//Socket?
	if ( psPacket->iResult == 2 )
	{
		ItemData sItemData;
		if ( ITEMHANDLER->LoadItemImage( &psPacket->sItemData.sItem, &sItemData ) )
		{
			ITEMHANDLER->BackItemToInventory( &sItemData );
			ZeroMemory( (void*)0x03680358, 2967 * 4 );
			MIXWINDOW_OPEN = FALSE;
			psPacket->iHeader = 0;
		}
	}
}


BOOL MixHandler::AddItemDescription( const char * pszText, int iColor, int iArray )
{
	lstrcatA( (char*)0x03627040, pszText );
	lstrcatA( (char*)0x036259A0, "\r" );
	*(DWORD*)(0x3628768)			+= 1;
	iLineDescriptionColor[iArray]	= *(DWORD*)(0x3628768);
	iDescriptionColor[iArray]		= iColor;
	return TRUE;
}

BOOL MixHandler::IsRespecOnlyItem( int iItemID )
{
	if ( iItemID == ITEMID_ValentoRing ||
		iItemID == ITEMID_KelvezuAmulet ||
		iItemID == ITEMID_DarkGuardianSheltom ||
		iItemID == ITEMID_AbyssRing ||
		iItemID == ITEMID_FurySheltom ||
		iItemID == ITEMID_ThroneSheltom ||
		iItemID == ITEMID_DevilShyRing ||
		iItemID == ITEMID_ThroneRing ||
		iItemID == ITEMID_ArgonianGauntlets ||
		iItemID == ITEMID_GreedyGauntlets ||
		iItemID == ITEMID_GhostCastleBracelets )
		return TRUE;

	return FALSE;
}

BOOL MixHandler::CanUseSheltomSlot( int iItemID )
{
	if ( (iItemID & 0xFFFFFF00) == ITEMID_SpecStoneCode )
	{
		TITLEBOX( "Use craft npc to respec with this stone!" );
		return FALSE;
	}

	if ( (iItemID & 0xFFFF0000) == ITEMTYPE_Respec  ||
		 (iItemID & 0xFFFF0000) == ITEMTYPE_Sheltom ||
		iItemID == ITEMID_RedGemPiece				||
		iItemID == ITEMID_GreenGemPiece				|| 
		iItemID == ITEMID_BlueGemPiece				|| 
		iItemID == ITEMID_YellowGemPiece			||
		iItemID == ITEMID_PurpleGemPiece			||
		iItemID == ITEMID_RedTriangularGem			||
		iItemID == ITEMID_RedSquareGem				||
		iItemID == ITEMID_GreenTriangularGem		||
		iItemID == ITEMID_GreenSquareGem			||
		iItemID == ITEMID_YellowTriangularGem		||
		iItemID == ITEMID_YellowSquareGem			||
		iItemID == ITEMID_BlueTriangularGem			||
		iItemID == ITEMID_BlueSquareGem				||
		iItemID == ITEMID_PurpleTriangularGem		||
		iItemID == ITEMID_PurpleSquareGem
		)
		return TRUE;


	return FALSE;
}

BOOL MixHandler::CanUseSocketItem( int iItemIDA, int iItemIDB, int iItemIDC )
{

	if ( iItemIDA == ITEMID_PurpleDiamond &&
	   (iItemIDB == ITEMID_RedGemPiece ||
		iItemIDB == ITEMID_GreenGemPiece ||
		iItemIDB == ITEMID_BlueGemPiece ||
		iItemIDB == ITEMID_YellowGemPiece ||
		iItemIDB == ITEMID_PurpleGemPiece) )
		return TRUE;

	if ( iItemIDA == ITEMID_SilverDiamond &&
		(iItemIDB == ITEMID_RedTriangularGem ||
		 iItemIDB == ITEMID_GreenTriangularGem ||
		 iItemIDB == ITEMID_BlueTriangularGem ||
		 iItemIDB == ITEMID_YellowTriangularGem ||
		 iItemIDB == ITEMID_PurpleTriangularGem) )
		return TRUE;

	if ( iItemIDA == ITEMID_GoldDiamond &&
		(iItemIDB == ITEMID_RedSquareGem ||
		 iItemIDB == ITEMID_GreenSquareGem ||
		 iItemIDB == ITEMID_BlueSquareGem ||
		 iItemIDB == ITEMID_YellowSquareGem ||
		 iItemIDB == ITEMID_PurpleSquareGem) )
		return TRUE;

	//Marquise Gems
	if ( (iItemIDA == ITEMID_RedMarquiseGem ||
		iItemIDA == ITEMID_GreenMarquiseGem ||
		iItemIDA == ITEMID_BlueMarquiseGem ||
		iItemIDA == ITEMID_YellowMarquiseGem ||
		iItemIDA == ITEMID_PurpleMarquiseGem) && (
		iItemIDB == ITEMID_Devine ||
		iItemIDB == ITEMID_Celesto ||
		iItemIDB == ITEMID_Mirage ||
		iItemIDB == ITEMID_Inferna ||
		iItemIDB == ITEMID_Enigma ||
		iItemIDB == ITEMID_Bellum ||
		iItemIDB == ITEMID_Oredo) )
		return TRUE;


	if ( (iItemIDA == ITEMID_PurpleDiamond ||
		iItemIDA == ITEMID_SilverDiamond ||
		iItemIDA == ITEMID_GoldDiamond ||
		//Marquise Gems
		iItemIDA == ITEMID_RedMarquiseGem ||
		iItemIDA == ITEMID_GreenMarquiseGem ||
		iItemIDA == ITEMID_BlueMarquiseGem ||
		iItemIDA == ITEMID_YellowMarquiseGem ||
		iItemIDA == ITEMID_PurpleMarquiseGem ) 
		&& (iItemIDB == 0 && iItemIDC == 0) )
		return TRUE;

	if ( iItemIDC != 0 && iItemIDC != iItemIDB )
		return FALSE;

	return FALSE;
}

BOOL MixHandler::IsSocketItem( int iItemID )
{
	if ( (iItemID == ITEMID_PurpleDiamond ||
		iItemID == ITEMID_SilverDiamond ||
		iItemID == ITEMID_GoldDiamond ||
		//Marquise Gems
		iItemID == ITEMID_RedMarquiseGem ||
		iItemID == ITEMID_GreenMarquiseGem ||
		iItemID == ITEMID_BlueMarquiseGem ||
		iItemID == ITEMID_YellowMarquiseGem ||
		iItemID == ITEMID_PurpleMarquiseGem) )
		return TRUE;

	return FALSE;
}

BOOL MixHandler::CanPutStoneSlotSocket( ItemData * pcItemData, BOOL bGem )
{
	BOOL bResult = FALSE;
	int iItemID = pcItemData->sBaseItemID.ToItemID();

	SocketItemData * pcSocketItem = (SocketItemData*)(0x03628C08);

	int iSocket = pcSocketItem->sItemData.sItem.sSocketData[0].eGemItem == 0 ? 0 : 1;

	BOOL bSetCounter = pcSocketItem->sRune[0].bValid ? FALSE : TRUE;

	ESocketType eType = pcSocketItem->sItemData.sItem.sSocketData[iSocket].eSocketType;

	int iCounter = 0;

	if ( bGem )
	{
		pcSocketItem->iItemCounter = 1;

		switch ( iItemID )
		{
			case ITEMID_RedTriangularGem:
			case ITEMID_RedSquareGem:
			case ITEMID_RedMarquiseGem:
				if ( eType == ESocketType::SOCKETTYPE_Ruby || eType == ESocketType::SOCKETTYPE_Prismatic )
					bResult = TRUE;
				break;

			case ITEMID_GreenTriangularGem:
			case ITEMID_GreenSquareGem:
			case ITEMID_GreenMarquiseGem:
				if ( eType == ESocketType::SOCKETTYPE_Emerald || eType == ESocketType::SOCKETTYPE_Prismatic )
					bResult = TRUE;
				break;

			case ITEMID_BlueTriangularGem:
			case ITEMID_BlueSquareGem:
			case ITEMID_BlueMarquiseGem:
				if ( eType == ESocketType::SOCKETTYPE_Aquamarine || eType == ESocketType::SOCKETTYPE_Prismatic )
					bResult = TRUE;
				break;

			case ITEMID_YellowTriangularGem:
			case ITEMID_YellowSquareGem:
			case ITEMID_YellowMarquiseGem:
				if ( eType == ESocketType::SOCKETTYPE_Topaz || eType == ESocketType::SOCKETTYPE_Prismatic )
					bResult = TRUE;
				break;

			case ITEMID_PurpleTriangularGem:
			case ITEMID_PurpleSquareGem:
			case ITEMID_PurpleMarquiseGem:
				if ( eType == ESocketType::SOCKETTYPE_Amethyst || eType == ESocketType::SOCKETTYPE_Prismatic )
					bResult = TRUE;
				break;

			default:
				break;
		}
	}
	else
	{
		switch ( iItemID )
		{
			case ITEMID_DiamondDrill:
				iCounter = 1;
				bResult = TRUE;
				break;

			case ITEMID_TitaniumDrill:
				iCounter = 3;
				bResult = TRUE;
				break;

			case ITEMID_IronDrill:
				iCounter = 5;
				bResult = TRUE;
				break;

			default:
				break;
		}
	}

	if ( bSetCounter && iCounter != 0 )
		pcSocketItem->iItemCounter = iCounter;

	return bResult;
}

BOOL MixHandler::CanPutItemSlotSocket( ItemData * pcItemData, BOOL bGemNPC )
{
	BOOL bResult = FALSE;

	int iItemID = pcItemData->sBaseItemID.ToItemID();

	if ( bGemNPC )
	{
		if ( pcItemData->sItem.sSocketData[0].eSocketType != 0 && pcItemData->sItem.sSocketData[0].eGemItem == 0 )
			bResult = TRUE;
		if ( pcItemData->sItem.sSocketData[1].eSocketType != 0 && pcItemData->sItem.sSocketData[1].eGemItem == 0 )
			bResult = TRUE;
	}
	else
	{
		switch ( iItemID & 0xFFFF0000 )
		{
			case ITEMTYPE_Axe:
			case ITEMTYPE_Scythe:
			case ITEMTYPE_Javelin:
			case ITEMTYPE_Hammer:
			case ITEMTYPE_Sword:
			case ITEMTYPE_Wand:
			case ITEMTYPE_Bow:
			case ITEMTYPE_Claw:
			case ITEMTYPE_Dagger:
			case ITEMTYPE_Phantom:
				if ( pcItemData->IsTwoHandedWeapon() == FALSE )
				{
					if ( pcItemData->sItem.sSocketData[0].eSocketType != 0 )
					{
						bResult = FALSE;
						break;
					}
				}
				bResult = TRUE;
				break;

			case ITEMTYPE_Armor:
			case ITEMTYPE_Robe:
				bResult = TRUE;
				break;

			case ITEMTYPE_Bracelets:
			case ITEMTYPE_Boots:
			case ITEMTYPE_Gauntlets:
			case ITEMTYPE_Shield:
			case ITEMTYPE_Orb:
				bResult = TRUE;
				if ( pcItemData->sItem.sSocketData[0].eSocketType != 0 )
				{
					bResult = FALSE;
				}
				break;

			case ITEMTYPE_Ring2:
				if ( iItemID == ITEMID_ValentoRing || iItemID == ITEMID_FallenRing || iItemID == ITEMID_ThroneRing )
				{
					bResult = TRUE;
					if ( pcItemData->sItem.sSocketData[0].eSocketType != 0 )
					{
						bResult = FALSE;
					}
				}
				break;

			case ITEMTYPE_Amulet:
				if ( iItemID == ITEMID_KelvezuAmulet || iItemID == ITEMID_TullaAmulet || iItemID == ITEMID_ThroneAmulet )
				{
					bResult = TRUE;
					if ( pcItemData->sItem.sSocketData[0].eSocketType != 0 )
					{
						bResult = FALSE;
					}
				}
				break;

			case ITEMTYPE_Sheltom:
				if ( (iItemID == ITEMID_FurySheltom) || (iItemID == ITEMID_DarkGuardianSheltom) || (iItemID == ITEMID_ThroneSheltom) )
				{
					bResult = TRUE;
					if ( pcItemData->sItem.sSocketData[0].eSocketType != 0 )
					{
						bResult = FALSE;
					}
				}
				break;

			default:
				break;
		}
	}

	return bResult;
}

void MixHandler::SetSocketStatus( EGemType eGemType, EGemItem eGemItem )
{
	int iAddStatus = 0;
	switch ( eGemType )
	{
		case GEMTYPE_Triangular:
			iAddStatus = 5;
			break;
		case GEMTYPE_Square:
			iAddStatus = 10;
			break;
		case GEMTYPE_Marquise:
			iAddStatus = 15;
			break;
		default:
			break;
	}

	switch ( eGemItem )
	{
		case GEMITEM_Emerald:
			(*(DWORD*)pdwAgility) += iAddStatus;
			break;
		case GEMITEM_Ruby:
			(*(DWORD*)pdwStrength) += iAddStatus;
			break;
		case GEMITEM_Aquamarine:
			(*(DWORD*)pdwSpirit) += iAddStatus;
			break;
		case GEMITEM_Amethyst:
			(*(DWORD*)pdwHealth) += iAddStatus;
			break;
		case GEMITEM_Topaz:
			(*(DWORD*)pdwTalent) += iAddStatus;
			break;
		default:
			break;
	}
}

void MixHandler::OnHandlerCharacterUpdate()
{

	for ( int i = 0; i < 14; i++ )
	{
		if ( INVENTORYITEMSLOT[i].iItemIndex > 0 )
		{
			ItemData * ps = &INVENTORYITEMS[INVENTORYITEMSLOT[i].iItemIndex - 1];
			if ( ps->bValid )
			{
				if ( ps->IsTwoHandedWeapon() && i == (ITEMSLOT_LeftHand - 1) )
					continue;

				if ( ps->sItem.sSocketData[0].eGemItem != 0 )
				{
					SetSocketStatus( ps->sItem.sSocketData[0].eGemType, ps->sItem.sSocketData[0].eGemItem );
				}
				if ( ps->sItem.sSocketData[1].eGemItem != 0 )
				{
					SetSocketStatus( ps->sItem.sSocketData[1].eGemType, ps->sItem.sSocketData[1].eGemItem );
				}
			}
		}
	}
}

BOOL MixHandler::SetItemMix( ItemData * psItemData, BOOL bFlag )
{
	// Item Collide Index
	ITEMWINDOWHANDLER->SetTradeItemColorBox( ETradeColorBox::TRADECOLORBOX_None );
	(*(DWORD*)0x036EFCA4) = 0;
	// Init Trade Color Box
	CALL( 0x0050BDC0 );

	ItemData * psaMixItemData = (ItemData*)0x03680358;

	// Check NPC position
	if ( (*(short*)0x04B0B22A) >= 300 )
		return TRUE;

	// Is Sheltom or Respec Stone?
	if ( CanUseSheltomSlot( psItemData->sItem.sItemID.ToInt() ) && (psItemData->sItem.sItemID.ToItemID() != ITEMID_FurySheltom) &&
		(psItemData->sItem.sItemID.ToItemID() != ITEMID_DarkGuardianSheltom) && (psItemData->sItem.sItemID.ToItemID() != ITEMID_ThroneSheltom) )
	{
		// Set sBox Box
		RECT sRect{ 218, 193 + (*(int*)0x0082D3A0), 218 + (3 * 22), 193 + (*(int*)0x0082D3A0) + (4 * 22) };
		ITEMWINDOWHANDLER->SetTradeRect( &sRect );

		// Check for Respec only items
		if ( psItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Sheltom )
		{
			ItemData * ps = psaMixItemData;
			if ( ps->bValid )
			{
				if ( IsRespecOnlyItem( ps->sItem.sItemID.ToInt() ) )
				{
					// Cannot use
					ITEMWINDOWHANDLER->SetTradeItemColorBox( ETradeColorBox::TRADECOLORBOX_NotSetItem );
					(*(DWORD*)0x036EFCA4) = 0;
					return FALSE;
				}
			}
			else
			{
				//Force
				if ( ((*(BOOL*)0x0368033C) == FALSE) && MIXWINDOW_OPEN )
				{
					TITLEBOX( "Please insert an item first!" );
					// Cannot use
					ITEMWINDOWHANDLER->SetTradeItemColorBox( ETradeColorBox::TRADECOLORBOX_NotSetItem );
					(*(DWORD*)0x036EFCA4) = 0;
					return FALSE;
				}
			}
		}

		//Check Gem Use
		if ( psItemData->sItem.sItemID.ToItemType() == 0x03130000 )
		{
			ItemData * ps = psaMixItemData;
			if ( ps->bValid )
			{
				int iID = ps->sItem.sItemID.ToInt();

				ps = psaMixItemData + 1;
				int iFirstItemID = ps->bValid ? ps->sItem.sItemID.ToItemID() : 0;

				if ( CanUseSocketItem( iID, psItemData->sItem.sItemID.ToItemID(), iFirstItemID ) == FALSE )
				{
					// Cannot use
					ITEMWINDOWHANDLER->SetTradeItemColorBox( ETradeColorBox::TRADECOLORBOX_NotSetItem );
					(*(DWORD*)0x036EFCA4) = 0;
					return FALSE;
				}
			}
		}


		for ( int i = psItemData->sPosition.iX + 11; i < psItemData->sPosition.iX + psItemData->iWidth; i += 22 )
		{
			for ( int j = psItemData->sPosition.iY + 11; j < psItemData->sPosition.iY + psItemData->iHeight; j += 22 )
			{
				// Inside Sheltoms slot?
				if ( sRect.left <= i && sRect.right > 1 && sRect.top <= j && sRect.bottom > j )
				{
					// Set Trade Color sBox
					RECT * psRect = (RECT*)(0x036EFCAC);
					psRect->left	= sRect.left + (((i - sRect.left) / 22) * 22);
					psRect->top		= sRect.top + (((j - sRect.top) / 22) * 22);
					psRect->right	= psItemData->iWidth;
					psRect->bottom	= psItemData->iHeight;

					// Outside Sheltoms slot?
					if ( sRect.right < ((psRect->left + psRect->right) - 11) || sRect.bottom < ((psRect->top + psRect->bottom) - 11) )
					{
						// Clear Trade Box
						CALL( 0x0050BDC0 );
						return FALSE;
					}

					// Place Item in the new position
					psItemData->sPlacePosition.iX			= psRect->left;
					psItemData->sPlacePosition.iY			= psRect->top;
					psItemData->iItemSlot	= 0;
					ITEMWINDOWHANDLER->SetTradeItemColorBox( ETradeColorBox::TRADECOLORBOX_SetItem );

					// Force Type NPC?
					if ( (*(BOOL*)0x0368033C) )
					{
						if ( (*(BOOL*)0x000CDC4EC) == FALSE )
						{
							int iPrice = 0;

							BOOL bRes = FALSE;
							__asm
							{
								PUSH psItemData;
								MOV ECX, 0x03680334;
								MOV EAX, 0x0050CF20;
								CALL EAX;
								MOV iPrice, EAX;
							}
							__asm
							{
								PUSH iPrice;
								MOV ECX, 0x03653E40;
								MOV EAX, 0x004E59C0;
								CALL EAX;
								MOV bRes, EAX;
							}
							if ( bRes == FALSE )
							{
								ITEMWINDOWHANDLER->SetTradeItemColorBox( ETradeColorBox::TRADECOLORBOX_NotEnoughGold );
								return FALSE;
							}
						}
						if ( !GM_MODE )
						{
							if ( (psItemData->sItem.sItemID.ToInt() & 0x0000FFFF) >= 0x0F00 )
							{
								ITEMWINDOWHANDLER->SetTradeItemColorBox( ETradeColorBox::TRADECOLORBOX_NotSetItem );
								return FALSE;
							}
						}
					}

					if ( (*(BOOL*)0x0368033C) == FALSE )
					{
						if ( psItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Sheltom )
						{
							if ( (psItemData->sItem.sItemID.ToInt() & 0x0000FFFF) >= 0x1500 && (psItemData->sItem.sItemID.ToInt() & 0x0000FFFF) <= 0x3200 )
							{
								ITEMWINDOWHANDLER->SetTradeItemColorBox( ETradeColorBox::TRADECOLORBOX_NotSetItem );
								return FALSE;
							}
						}

					}

					// Cannot use item
					if ( psItemData->sItem.sItemID.ToItemID() == ITEMID_Roitra ||
						psItemData->sItem.sItemID.ToItemID() == ITEMID_TearsOfKalia ||
						psItemData->sItem.eCraftType == ITEMCRAFTTYPE_QuestWeapon )
					{
						ITEMWINDOWHANDLER->SetTradeItemColorBox( ETradeColorBox::TRADECOLORBOX_NotSetItem );
						return FALSE;
					}

					// ???
					if ( bFlag == FALSE )
					{
						if ( (*(DWORD*)0x036EFCA4) = ITEMWINDOWHANDLER->CollideItemTrade( psRect, 0, 2 ) )
						{
							ITEMWINDOWHANDLER->SetTradeItemColorBox( ETradeColorBox::TRADECOLORBOX_NotSetItem );
							return FALSE;
						}
						return TRUE;
					}
					// ???
					if ( bFlag == TRUE )
					{
						if ( (*(DWORD*)0x036EFCA4) = ITEMWINDOWHANDLER->CollideItemTrade( psRect, 0, 3 ) )
						{
							ITEMWINDOWHANDLER->SetTradeItemColorBox( ETradeColorBox::TRADECOLORBOX_NotSetItem );
							return FALSE;
						}
						return TRUE;
					}
				}
			}
		}
	}
	else
	{
		// Force Type NPC?
		if ( (*(BOOL*)0x0368033C) )
			return FALSE;

		// Item Collide Index
		ITEMWINDOWHANDLER->SetTradeItemColorBox( ETradeColorBox::TRADECOLORBOX_None );
		(*(DWORD*)0x036EFCA4) = 0;
		// Init Trade Color Box
		CALL( 0x0050BDC0 );

		RECT sRect{ 58, 193 + (*(int*)0x0082D3A0), 58 + (3 * 22), 193 + (*(int*)0x0082D3A0) + (4 * 22) };
		ITEMWINDOWHANDLER->SetTradeRect( &sRect );

		// Check for Respec only items
		if ( IsRespecOnlyItem( psItemData->sItem.sItemID.ToInt() ) )
		{
			for ( int i = 1; i < 15; i++ )
			{
				ItemData * ps = psaMixItemData + i;
				if ( ps->bValid && ps->sItem.sItemID.ToItemType() == ITEMTYPE_Sheltom )
				{
					// Cannot use
					ITEMWINDOWHANDLER->SetTradeItemColorBox( ETradeColorBox::TRADECOLORBOX_NotSetItem );
					(*(DWORD*)0x036EFCA4) = 0;
					return FALSE;
				}
			}
		}

		//Check Gem Use
		{
			ItemData * ps = psItemData;
			if ( ps->bValid )
			{
				int iID = ps->sItem.sItemID.ToInt();

				ps = psaMixItemData + 1;
				int iFirstItemID = ps->bValid ? ps->sItem.sItemID.ToInt() : 0;

				if ( IsSocketItem( iID ) && CanUseSocketItem( iID, iFirstItemID, iFirstItemID ) == FALSE )
				{
					// Cannot use
					ITEMWINDOWHANDLER->SetTradeItemColorBox( ETradeColorBox::TRADECOLORBOX_NotSetItem );
					(*(DWORD*)0x036EFCA4) = 0;
					return FALSE;
				}
			}
		}

		// Inside box?
		Point2D * psPoint = MOUSEHANDLER->GetPosition();

		if ( sRect.left <= psPoint->iX && sRect.right > psPoint->iX && sRect.top <= psPoint->iY && sRect.bottom > psPoint->iY )
		{
			// Set Trade Color sBox
			RECT * psRect	= (RECT*)(0x036EFCAC);
			psRect->left	= sRect.left;
			psRect->top		= sRect.top;
			psRect->right	= sRect.right - sRect.left;
			psRect->bottom	= sRect.bottom - sRect.top;

			// Place Item in the new position
			psItemData->sPlacePosition.iX			= sRect.left + ((sRect.right - sRect.left) - psItemData->iWidth) / 2;
			psItemData->sPlacePosition.iY			= sRect.top + ((sRect.bottom - sRect.top) - psItemData->iHeight) / 2;
			psItemData->iItemSlot	= 0;
			ITEMWINDOWHANDLER->SetTradeItemColorBox( ETradeColorBox::TRADECOLORBOX_SetItem );

			if ( (*(DWORD*)0x036EFCA4) = ITEMWINDOWHANDLER->CollideItemTrade( psRect, 0, 2 ) )
			{
				ITEMWINDOWHANDLER->SetTradeItemColorBox( ETradeColorBox::TRADECOLORBOX_NotSetItem );
				return FALSE;
			}

			if ( (*(BOOL*)0x00CDC4E8) == FALSE )
			{
				BOOL bRes = FALSE;
				int iPrice = psItemData->sItem.iSalePrice;
				__asm
				{
					PUSH iPrice;
					MOV ECX, 0x03653E40;
					MOV EAX, 0x004E59C0;
					CALL EAX;
					MOV bRes, EAX;
				}
				if ( bRes == FALSE )
				{
					ITEMWINDOWHANDLER->SetTradeItemColorBox( ETradeColorBox::TRADECOLORBOX_NotEnoughGold );
					return FALSE;
				}
			}

			// Price	
			PriceItem sSell;
			if ( ITEMHANDLER->GetItemPrice( psItemData, &sSell ) )
			{
				int iPrice = (sSell.iPrice * 5) + ((sSell.iPrice * 50) / 100);
				if ( psItemData->sItem.eSpecialization )
					iPrice += (iPrice * 2) / 10;

				// BC Price
				__asm
				{
					PUSH iPrice;
					MOV ECX, 0x03653E40;
					MOV EAX, 0x004E5A10;
					CALL EAX;
					MOV iPrice, EAX;
				}

				if ( iPrice > UNITDATA->sCharacterData.iGold )
				{
					ITEMWINDOWHANDLER->SetTradeItemColorBox( ETradeColorBox::TRADECOLORBOX_NotEnoughGold );
					return FALSE;
				}
			}

			// Force Sheltom Check
			if ( (*(BOOL*)0x0368033C) == FALSE )
			{
				if ( psItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Sheltom )
				{
					if ( (psItemData->sItem.sItemID.ToInt() & 0x0000FFFF) >= 0x1500 && (psItemData->sItem.sItemID.ToInt() & 0x0000FFFF) <= 0x3200 )
					{
						if ( (psItemData->sItem.sItemID.ToInt() != ITEMID_FurySheltom) && (psItemData->sItem.sItemID.ToInt() != ITEMID_DarkGuardianSheltom) && (psItemData->sItem.sItemID.ToInt() != ITEMID_ThroneSheltom) )
						{
							ITEMWINDOWHANDLER->SetTradeItemColorBox( ETradeColorBox::TRADECOLORBOX_NotSetItem );
							return FALSE;
						}
					}
				}
			}
			// Cannot use item
			if ( psItemData->sItem.sItemID.ToItemID() == ITEMID_Roitra ||
				psItemData->sItem.sItemID.ToItemID() == ITEMID_TearsOfKalia ||
				psItemData->sItem.eCraftType == ITEMCRAFTTYPE_QuestWeapon )
			{
				ITEMWINDOWHANDLER->SetTradeItemColorBox( ETradeColorBox::TRADECOLORBOX_NotSetItem );
				return FALSE;
			}
			return TRUE;
		}
	}
	return TRUE;
}
