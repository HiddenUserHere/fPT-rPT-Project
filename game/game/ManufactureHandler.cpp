#include "stdafx.h"
#include "ManufactureHandler.h"


CManufactureHandler::CManufactureHandler()
{
}


CManufactureHandler::~CManufactureHandler()
{
}

void CManufactureHandler::SetManufactureAreaCheck( class ItemData * pcItem )
{
	POINT sRunePoint[3] = { {240, 159},  {215, 190}, {265, 190} };

	ManufactureRune * psRuneData = (ManufactureRune*)0x03692E70;

	struct ManufactureItem
	{
		ItemData					sItemData[4];
		int							iRuneIndex[12];
	};

	ManufactureItem * psManufactureData = (ManufactureItem *)0x0368D9A0;

	DWORD dwCode = psManufactureData->sItemData[0].bValid ? psManufactureData->sItemData[0].sItem.sItemID.ToInt() : 0;
	int iLevelItem = psManufactureData->sItemData[0].bValid ? psManufactureData->sItemData[0].sItem.iLevel : -1;

	RECT * pRect = ((RECT*)0x036EFCAC);
	CALL( 0x0050BDC0 );

	ITEMWINDOWHANDLER->SetTradeItemColorBox( TRADECOLORBOX_None );

	if ( pcItem->sItem.bCostumeItem )
	{
		TITLEBOX( AY_OBFUSCATE( "This Item can't be used on Master Shalon!" ) );
		ITEMWINDOWHANDLER->SetTradeItemColorBox( TRADECOLORBOX_NotSetItem );
		return;
	}

	if ( IsRecipe( pcItem->sItem.sItemID.ToItemID() ) || CanRespec( pcItem->sItem.sItemID.ToItemID() ) )
	{
		pRect->left		= 58;
		pRect->top		= 193 - 50;
		pRect->right	= (pRect->left + (3 * 22)) - pRect->left;
		pRect->bottom	= (pRect->top + (4 * 22)) - pRect->top;

		Rectangle2D cRectItem{ pRect->left, pRect->top, (3 * 22), (4 * 22) };

		if ( cRectItem.Inside( MOUSEHANDLER->GetPosition() ) )
		{
			pcItem->sPlacePosition.iX = pRect->left + (((3 * 22) / 2) - (pcItem->iWidth / 2));
			pcItem->sPlacePosition.iY = pRect->top + (((4 * 22) / 2) - (pcItem->iHeight / 2));
			pcItem->iItemSlot = 0;
			ITEMWINDOWHANDLER->SetTradeItemColorBox( TRADECOLORBOX_SetItem );

			if ( (pcItem->sItem.sItemID.ToItemID() == ITEMID_RecipeXMas01) || (pcItem->sItem.sItemID.ToItemID() == ITEMID_RecipeXMas02) || (pcItem->sItem.sItemID.ToItemID() == ITEMID_XmasRecipeGift) ||
				 (pcItem->sItem.sItemID.ToItemID() == ITEMID_IceRecipe) || (pcItem->sItem.sItemID.ToItemID() == ITEMID_MysteriousBookRecipe) || (pcItem->sItem.sItemID.ToItemID() == ITEMID_SecretRecipe) )
			{
				WRITEDWORD( 0x036EFD24, 200000 );
			}
			else
			{
				WRITEDWORD( 0x036EFD24, pcItem->sItem.iSalePrice );
			}

			if ( (int)READDWORD( 0x036EFD24 ) > UNITDATA->sCharacterData.iGold )
			{
				TITLEBOX( "Not enough gold" );
				ITEMWINDOWHANDLER->SetTradeItemColorBox( TRADECOLORBOX_NotSetItem );
				return;
			}

			if ( ((*(DWORD*)0x036EFCA4) = ITEMWINDOWHANDLER->CollideItemTrade( pRect, 0, 5 )) )
			{
				ITEMWINDOWHANDLER->SetTradeItemColorBox( TRADECOLORBOX_NotSetItem );
				return;
			}

		}
		return;
	}

	int iRuneSlotIndex = -1;

	BOOL bCollides = FALSE;
	for ( int i = 0; i < 3; i++ )
	{
		Rectangle2D cRect( sRunePoint[i].x-11, sRunePoint[i].y-11, 22, 22 );
		Point2D sPoint = { pcItem->sPosition.iX, pcItem->sPosition.iY };

		if ( cRect.Inside( &sPoint ) )
		{
			pRect->left		= sRunePoint[i].x;
			pRect->top		= sRunePoint[i].y;
			pRect->right	= pcItem->iWidth;
			pRect->bottom	= pcItem->iHeight;
			bCollides		= TRUE;

			iRuneSlotIndex = i;
			break;
		}
	}
	if ( !bCollides )
		return;

	EItemID eRuneItemID = pcItem->sItem.sItemID.ToItemID();

	if ( dwCode && (IsRespecStone( pcItem->sItem.sItemID.ToItemID() ) || pcItem->sItem.sItemID.ToItemID() == ITEMID_Jera || pcItem->sItem.sItemID.ToItemID() == ITEMID_SuperJera) )
	{
		bool bContinue = true;
		if ( (pcItem->sItem.sItemID.ToItemID() == ITEMID_SuperJera) || (pcItem->sItem.sItemID.ToItemID() == ITEMID_Jera) )
			bContinue = iLevelItem >= 128 ? (pcItem->sItem.sItemID.ToItemID() == ITEMID_SuperJera) : (pcItem->sItem.sItemID.ToItemID() == ITEMID_Jera);

		if ( CanRespec( (EItemID)dwCode ) && bContinue )
		{
			pcItem->sPlacePosition.iX = pRect->left;
			pcItem->sPlacePosition.iY = pRect->top;
			pcItem->iItemSlot = 0;
			ITEMWINDOWHANDLER->SetTradeItemColorBox( TRADECOLORBOX_SetItem );
			return;
		}
		return;
	}

	// Check is in slot
	if ( !dwCode || !IsRune( pcItem->sItem.sItemID.ToItemID() ) )
		return;

	ITEMWINDOWHANDLER->SetTradeItemColorBox( TRADECOLORBOX_NotSetItem );

	if ( ((*(DWORD*)0x036EFCA4) = ITEMWINDOWHANDLER->CollideItemTrade( pRect, 0, 5 )) )
	{
		return;
	}
	if ( dwCode == ITEMID_CoreHW )
	{
		if ( HALLOWEENHANDLER->IsValidRune( pcItem ) )
		{
			pcItem->sPlacePosition.iX			= pRect->left;
			pcItem->sPlacePosition.iY			= pRect->top;
			pcItem->iItemSlot	= 0;
			ITEMWINDOWHANDLER->SetTradeItemColorBox( TRADECOLORBOX_SetItem );
			return;
		}
		
		return;
	}
	if ( (dwCode == ITEMID_RecipeXMas01) || (dwCode == ITEMID_RecipeXMas02) )
	{
		if ( XMASHANDLER->IsValidRune( pcItem ) )
		{
			pcItem->sPlacePosition.iX = pRect->left;
			pcItem->sPlacePosition.iY = pRect->top;
			pcItem->iItemSlot = 0;
			ITEMWINDOWHANDLER->SetTradeItemColorBox( TRADECOLORBOX_SetItem );
			return;
		}

		return;
	}

	//Secret Recipe
	if ( HaveSlot( ITEMID_SecretRecipe, eRuneItemID, iRuneSlotIndex, false, ITEMID_Bellum, 3 ) )
	{
		pcItem->sPlacePosition.iX = pRect->left;
		pcItem->sPlacePosition.iY = pRect->top;
		pcItem->iItemSlot = 0;
		ITEMWINDOWHANDLER->SetTradeItemColorBox( TRADECOLORBOX_SetItem );
		return;
	}

	//New Xmas
	if ( HaveSlot( ITEMID_XmasRecipeGift, eRuneItemID, iRuneSlotIndex, false, ITEMID_RedXmasGiftRune, 1, ITEMID_GreenXmasGiftRune, 1, ITEMID_BlueXmasGiftRune, 1 ) )
	{
		pcItem->sPlacePosition.iX = pRect->left;
		pcItem->sPlacePosition.iY = pRect->top;
		pcItem->iItemSlot = 0;
		ITEMWINDOWHANDLER->SetTradeItemColorBox( TRADECOLORBOX_SetItem );
		return;
	}

	//Ice Recipe
	if ( HaveSlot( ITEMID_IceRecipe, eRuneItemID, iRuneSlotIndex, false, ITEMID_IceRune, 3 ) )
	{
		pcItem->sPlacePosition.iX = pRect->left;
		pcItem->sPlacePosition.iY = pRect->top;
		pcItem->iItemSlot = 0;
		ITEMWINDOWHANDLER->SetTradeItemColorBox( TRADECOLORBOX_SetItem );
		return;
	}

	//Golden Costume Recipe
	if ( HaveSlot( ITEMID_MysteriousBookRecipe, eRuneItemID, iRuneSlotIndex, false, ITEMID_HopyRune, 1, ITEMID_CuepyRune, 1, ITEMID_GoldenHopyRune, 1 ) )
	{
		pcItem->sPlacePosition.iX = pRect->left;
		pcItem->sPlacePosition.iY = pRect->top;
		pcItem->iItemSlot = 0;
		ITEMWINDOWHANDLER->SetTradeItemColorBox( TRADECOLORBOX_SetItem );
		return;
	}

	//Tulla Amulet
	if ( dwCode == ITEMID_KelvezuAmulet )
	{
		if ( IsValidRuneTullaAmulet( pcItem->sItem.sItemID.ToItemID() ) )
		{
			pcItem->sPlacePosition.iX = pRect->left;
			pcItem->sPlacePosition.iY = pRect->top;
			pcItem->iItemSlot = 0;
			ITEMWINDOWHANDLER->SetTradeItemColorBox( TRADECOLORBOX_SetItem );
			return;
		}
		return;
	}

	//Throne Amulet
	if (dwCode == ITEMID_TullaAmulet )
	{
		if ( IsValidRuneThroneAmulet(pcItem->sItem.sItemID.ToItemID()))
		{
			pcItem->sPlacePosition.iX = pRect->left;
			pcItem->sPlacePosition.iY = pRect->top;
			pcItem->iItemSlot = 0;
			ITEMWINDOWHANDLER->SetTradeItemColorBox(TRADECOLORBOX_SetItem);
			return;
		}
		return;
	}
	//Draxos Boots
	if ( HaveSlot( ITEMID_MokovaBoots, eRuneItemID, iRuneSlotIndex, false, ITEMID_DarkRedRune, 1, ITEMID_Sapphire, 2 ) )
	{
		pcItem->sPlacePosition.iX = pRect->left;
		pcItem->sPlacePosition.iY = pRect->top;
		pcItem->iItemSlot = 0;
		ITEMWINDOWHANDLER->SetTradeItemColorBox( TRADECOLORBOX_SetItem );
		return;
	}

	//Tropical Bracelets
	if ( HaveSlot( ITEMID_GhostCastleBracelets, eRuneItemID, iRuneSlotIndex, false, ITEMID_LightRedRune, 3 ) )
	{
		pcItem->sPlacePosition.iX = pRect->left;
		pcItem->sPlacePosition.iY = pRect->top;
		pcItem->iItemSlot = 0;
		ITEMWINDOWHANDLER->SetTradeItemColorBox( TRADECOLORBOX_SetItem );
		return;
	}


	//Greedy Gauntlets
	if ( HaveSlot( ITEMID_ArgonianGauntlets, eRuneItemID, iRuneSlotIndex, false, ITEMID_LightOrangeRune, 1, ITEMID_Sapphire, 2 ) )
	{
		pcItem->sPlacePosition.iX = pRect->left;
		pcItem->sPlacePosition.iY = pRect->top;
		pcItem->iItemSlot = 0;
		ITEMWINDOWHANDLER->SetTradeItemColorBox( TRADECOLORBOX_SetItem );
		return;
	}

	//Devil Shy Ring
	if ( dwCode == ITEMID_ValentoRing )
	{
		if ( (pcItem->sItem.sItemID.ToItemID() == ITEMID_LightPurpleRune) || (pcItem->sItem.sItemID.ToItemID() == ITEMID_Bellum) )
		{
			pcItem->sPlacePosition.iX = pRect->left;
			pcItem->sPlacePosition.iY = pRect->top;
			pcItem->iItemSlot = 0;
			ITEMWINDOWHANDLER->SetTradeItemColorBox( TRADECOLORBOX_SetItem );
			return;
		}
		return;
	}

	//Throne Ring
	if (dwCode == ITEMID_DevilShyRing )
	{
		if ((pcItem->sItem.sItemID.ToItemID() == ITEMID_ThroneYellowRune) || (pcItem->sItem.sItemID.ToItemID() == ITEMID_Sapphire))
		{
			pcItem->sPlacePosition.iX = pRect->left;
			pcItem->sPlacePosition.iY = pRect->top;
			pcItem->iItemSlot = 0;
			ITEMWINDOWHANDLER->SetTradeItemColorBox(TRADECOLORBOX_SetItem);
			return;
		}
		return;
	}

	//Volcanos Earring
	if ( dwCode == ITEMID_GorgonyteEarring )
	{
		if ( (pcItem->sItem.sItemID.ToItemID() == ITEMID_LightSilverRune) || (pcItem->sItem.sItemID.ToItemID() == ITEMID_Sapphire) )
		{
			pcItem->sPlacePosition.iX = pRect->left;
			pcItem->sPlacePosition.iY = pRect->top;
			pcItem->iItemSlot = 0;
			ITEMWINDOWHANDLER->SetTradeItemColorBox( TRADECOLORBOX_SetItem );
			return;
		}
		return;
	}

	//Throne Amulet
	if (dwCode == ITEMID_TullaAmulet )
	{
		if ((pcItem->sItem.sItemID.ToItemID() == ITEMID_ThroneRedRune) || (pcItem->sItem.sItemID.ToItemID() == ITEMID_Sapphire))
		{
			pcItem->sPlacePosition.iX = pRect->left;
			pcItem->sPlacePosition.iY = pRect->top;
			pcItem->iItemSlot = 0;
			ITEMWINDOWHANDLER->SetTradeItemColorBox(TRADECOLORBOX_SetItem);
			return;
		}
		return;
	}

	//Throne Sheltom 
	if (dwCode == ITEMID_FurySheltom )
	{
		if ((pcItem->sItem.sItemID.ToItemID() == ITEMID_ThroneGreenRune) || (pcItem->sItem.sItemID.ToItemID() == ITEMID_Sapphire))
		{
			pcItem->sPlacePosition.iX = pRect->left;
			pcItem->sPlacePosition.iY = pRect->top;
			pcItem->iItemSlot = 0;
			ITEMWINDOWHANDLER->SetTradeItemColorBox(TRADECOLORBOX_SetItem);
			return;
		}
		return;
	}

	//Fury Sheltom
	if ( dwCode == ITEMID_DarkGuardianSheltom )
	{
		if ( (pcItem->sItem.sItemID.ToItemID() == ITEMID_GreenCrystalRune) || (pcItem->sItem.sItemID.ToItemID() == ITEMID_RedCrystalRune) ||
			 (pcItem->sItem.sItemID.ToItemID() == ITEMID_BlueCrystalRune) )
		{
			if ( HasFurySheltomRune( pcItem->sItem.sItemID.ToItemID() ) == FALSE )
			{
				pcItem->sPlacePosition.iX = pRect->left;
				pcItem->sPlacePosition.iY = pRect->top;
				pcItem->iItemSlot = 0;
				ITEMWINDOWHANDLER->SetTradeItemColorBox( TRADECOLORBOX_SetItem );
				return;
			}
		}
		return;
	}

	//Throne Sheltom
	if (dwCode == ITEMID_FurySheltom)
	{
		if ((pcItem->sItem.sItemID.ToItemID() == ITEMID_GoldCrystalRune) || (pcItem->sItem.sItemID.ToItemID() == ITEMID_SilverCrystalRune) ||
			(pcItem->sItem.sItemID.ToItemID() == ITEMID_BronzeCrystalRune))
		{
			if (HasThroneSheltomRune(pcItem->sItem.sItemID.ToItemID()) == FALSE)
			{
				pcItem->sPlacePosition.iX = pRect->left;
				pcItem->sPlacePosition.iY = pRect->top;
				pcItem->iItemSlot = 0;
				ITEMWINDOWHANDLER->SetTradeItemColorBox(TRADECOLORBOX_SetItem);
				return;
			}
		}
		return;
	}
}

BOOL CManufactureHandler::OnClickItemSlotPut( class ItemData * pcItem )
{
	struct ManufactureItem
	{
		ItemData					sItemData[4];
		int							iRuneIndex[12];
	};

	ManufactureItem * psaManufactureData = (ManufactureItem*)0x0368D9A0;

	if ( IsRune( pcItem->sBaseItemID.ToItemID() ) )
	{
		// Check Slots
		ItemData * psaItemData = (ItemData*)0x0368DD80;
		for ( int i = 1; i < 4; i++ )
		{
			if ( psaManufactureData->sItemData[i].bValid == FALSE )
			{
				ItemData * ps	= &psaManufactureData->sItemData[i];
				
				CopyMemory( ps, pcItem, sizeof( ItemData ) );
				
				ps->sPosition.iX	= pcItem->sPlacePosition.iX;
				ps->sPosition.iY	= pcItem->sPlacePosition.iY;
				pcItem->bValid		= FALSE;
				
				PLAYMINISOUND( pcItem->iInvItemSound );
				
				CALLT( 0x005125B0, 0x0369DD10 );

				break;
			}
		}
	}
	else if ( psaManufactureData->sItemData[0].bValid == FALSE )
	{

		if ( (pcItem->sItem.sItemID.ToItemID() == ITEMID_RecipeXMas01) || (pcItem->sItem.sItemID.ToItemID() == ITEMID_RecipeXMas02) || (pcItem->sItem.sItemID.ToItemID() == ITEMID_XmasRecipeGift) ||
			 (pcItem->sItem.sItemID.ToItemID() == ITEMID_IceRecipe) || (pcItem->sItem.sItemID.ToItemID() == ITEMID_MysteriousBookRecipe) || (pcItem->sItem.sItemID.ToItemID() == ITEMID_SecretRecipe) )
		{
			WRITEDWORD( 0x036EFD24, 200000 );
		}
		else if ( pcItem->sItem.sItemID.ToItemID() == ITEMID_ValentoRing )
		{
			WRITEDWORD( 0x036EFD24, pcItem->sItem.iSalePrice * 10 );
		}
		else if (pcItem->sItem.sItemID.ToItemID() == ITEMID_ThroneRing )
		{
			WRITEDWORD(0x036EFD24, pcItem->sItem.iSalePrice * 10);
		}
		else if ( pcItem->sItem.sItemID.ToItemID() == ITEMID_GorgonyteEarring )
		{
			WRITEDWORD( 0x036EFD24, pcItem->sItem.iSalePrice * 10 );
		}
		else if ( pcItem->sItem.sItemID.ToItemID() == ITEMID_KelvezuAmulet )
		{
			WRITEDWORD( 0x036EFD24, pcItem->sItem.iSalePrice * 10 );
		}
		else if ( pcItem->sItem.sItemID.ToItemID() == ITEMID_DarkGuardianSheltom )
		{
			WRITEDWORD( 0x036EFD24, pcItem->sItem.iSalePrice * 10 );
		}
		else if (pcItem->sItem.sItemID.ToItemID() == ITEMID_FurySheltom )
		{
			WRITEDWORD(0x036EFD24, pcItem->sItem.iSalePrice * 10);
		}
		else if (pcItem->sItem.sItemID.ToItemID() == ITEMID_ThroneSheltom)
		{
			WRITEDWORD(0x036EFD24, pcItem->sItem.iSalePrice * 10);
		}
		else if ( pcItem->sItem.sItemID.ToItemID() == ITEMID_GhostCastleBracelets )
		{
			WRITEDWORD( 0x036EFD24, pcItem->sItem.iSalePrice * 10 );
		}
		else if ( pcItem->sItem.sItemID.ToItemID() == ITEMID_ArgonianGauntlets )
		{
			WRITEDWORD( 0x036EFD24, pcItem->sItem.iSalePrice * 10 );
		}
		else
		{
			WRITEDWORD( 0x036EFD24, pcItem->sItem.iSalePrice );
		}

		if ( (int)READDWORD( 0x036EFD24 ) > UNITDATA->sCharacterData.iGold )
		{
			TITLEBOX( "Not enough gold" );
			return TRUE;
		}

		//Beginners?
		switch ( pcItem->sItem.sItemID.ToItemID() )
		{
			case ITEMID_BeginnerAxe:
			case ITEMID_BeginnerClaw:
			case ITEMID_BeginnerHammer:
			case ITEMID_BeginnerSword:
			case ITEMID_BeginnerJavelin:
			case ITEMID_BeginnerBow:
			case ITEMID_BeginnerScythe:
			case ITEMID_BeginnerPhantom:
			case ITEMID_BeginnerDagger:
			case ITEMID_BeginnerStaff:
			case ITEMID_BeginnerBoots:
			case ITEMID_BeginnerGauntlet:
			case ITEMID_BeginnerBracelet:
			case ITEMID_GrumpyArmor:
			case ITEMID_GrumpyRobe:
			case ITEMID_HopyCostumeTime:
			case ITEMID_CuepyCostumeTime:
			case ITEMID_HopyCostumeIce:
			case ITEMID_CuepyCostumeIce:

			//Halloween
			case ITEMID_VampireSuit:
			case ITEMID_Succubus:
			case ITEMID_HalloweenAxe:
			case ITEMID_HalloweenClaw:
			case ITEMID_HalloweenDagger:
			case ITEMID_HalloweenHammer:
			case ITEMID_HalloweenStaff:
			case ITEMID_HalloweenWand:
			case ITEMID_HalloweenPhantom:
			case ITEMID_HalloweenScythe:
			case ITEMID_HalloweenBow:
			case ITEMID_HalloweenSword:
			case ITEMID_HalloweenBladeSword:
			case ITEMID_HalloweenJavelin:
			case ITEMID_HalloweenShield:

			case ITEMID_XmasCostumeOldAxe:
			case ITEMID_XmasCostumeOldAxe1Hand:
			case ITEMID_XmasCostumeOldClaw:
			case ITEMID_XmasCostumeOldDagger:
			case ITEMID_XmasCostumeOldHammer:
			case ITEMID_XmasCostumeOldStaff:
			case ITEMID_XmasCostumeOldPhantom:
			case ITEMID_XmasCostumeOldScythe:
			case ITEMID_XmasCostumeOldBow:
			case ITEMID_XmasCostumeOldSword:
			case ITEMID_XmasCostumeOldSword1Hand:
			case ITEMID_XmasCostumeOldJavelin:

			case ITEMID_XmasCostumeAxe:
			case ITEMID_XmasCostumeAxe1Hand:
			case ITEMID_XmasCostumeClaw:
			case ITEMID_XmasCostumeDagger:
			case ITEMID_XmasCostumeHammer:
			case ITEMID_XmasCostumeStaff:
			case ITEMID_XmasCostumeWand:
			case ITEMID_XmasCostumePhantom:
			case ITEMID_XmasCostumeScythe:
			case ITEMID_XmasCostumeBow:
			case ITEMID_XmasCostumeSword:
			case ITEMID_XmasCostumeSword1Hand:
			case ITEMID_XmasCostumeJavelin:
			case ITEMID_XmasCostumeShield:

			case ITEMID_ObscureAxe:
			case ITEMID_ObscureAxe1Hand:
			case ITEMID_ObscureClaw:
			case ITEMID_ObscureDagger:
			case ITEMID_ObscureHammer:
			case ITEMID_ObscureStaff:
			case ITEMID_ObscureWand:
			case ITEMID_ObscurePhantom:
			case ITEMID_ObscureScythe:
			case ITEMID_ObscureBow:
			case ITEMID_ObscureSword:
			case ITEMID_ObscureSword1Hand:
			case ITEMID_ObscureJavelin:
			case ITEMID_ObscureShield:

			case ITEMID_SandurrRedAxe:
			case ITEMID_SandurrRedClaw:
			case ITEMID_SandurrRedDagger:
			case ITEMID_SandurrRedHammer:
			case ITEMID_SandurrRedStaff:
			case ITEMID_SandurrRedPhantom:
			case ITEMID_SandurrRedScythe:
			case ITEMID_SandurrRedBow:
			case ITEMID_SandurrRedSword:
			case ITEMID_SandurrRedJavelin:
			case ITEMID_SandurrRedShield:


			case ITEMID_SandurrRedAxe1Hand:
			case ITEMID_SandurrPurpleAxe1Hand:
			case ITEMID_SandurrRedSword1Hand:
			case ITEMID_SandurrPurpleSword1Hand:
			case ITEMID_SandurrPurple2Scythe:
			case ITEMID_SandurrPurpleAxe:
			case ITEMID_SandurrPurpleClaw:
			case ITEMID_SandurrPurpleDagger:
			case ITEMID_SandurrPurpleHammer:
			case ITEMID_SandurrPurpleStaff:
			case ITEMID_SandurrPurplePhantom:
			case ITEMID_SandurrPurpleScythe:
			case ITEMID_SandurrPurpleBow:
			case ITEMID_SandurrPurpleSword:
			case ITEMID_SandurrPurpleJavelin:
			case ITEMID_SandurrPurpleShield:

			case ITEMID_CandySheltom:
			case ITEMID_CeruleanRing:

			case ITEMID_IceCostumeMale:
			case ITEMID_IceCostumeFemale:
			case ITEMID_GoldenHopyCostumeMaleTime:
			case ITEMID_GoldenHopyCostumeFemaleTime:

				TITLEBOX( AY_OBFUSCATE( "This item is not allowed" ) );
				return TRUE;
				break;
			default:
				break;
		}

		ItemData * ps	= &psaManufactureData->sItemData[0];

		CopyMemory( ps, pcItem, sizeof( ItemData ) );

		ps->sPosition.iX	= pcItem->sPlacePosition.iX;
		ps->sPosition.iY	= pcItem->sPlacePosition.iY;
		pcItem->bValid		= FALSE;

		PLAYMINISOUND( pcItem->iInvItemSound );

		ItemData * psCraft = (ItemData*)0x03692FA0;
		CopyMemory( psCraft, ps, sizeof( ItemData ) );

		CALLT( 0x005125B0, 0x0369DD10 );

	}

	return TRUE;
}

void CManufactureHandler::SetManufacture()
{
	(*(BOOL*)0x037B7F08) = TRUE;
	CALL( 0x0052B020 );
	(*(BOOL*)0x036EFD80) = TRUE;
}

void CManufactureHandler::Update()
{
	struct ManufactureItem
	{
		ItemData					sItemData[4];
		int							iRuneIndex[12];
	};

	if ( CRAFTWINDOW_OPEN && CRAFTPROCCESS_START )
	{
		ManufactureItem * psaManufactureData = (ManufactureItem*)0x0368D9A0;

		if ( !CHARACTERGAME->CheckInventorySpace( &psaManufactureData->sItemData[0] ) )
		{
			TITLEBOX( "Not enough space in inventory" );
			CRAFTPROCCESS_START = FALSE;
		}

		// Check iWeight
		int iItemID = psaManufactureData->sItemData[0].sItem.sItemID.ToInt();
		if ( (iItemID == ITEMID_RecipeXMas01) || (iItemID == ITEMID_RecipeXMas02) || (iItemID == ITEMID_XmasRecipeGift) || (iItemID == ITEMID_IceRecipe) || (iItemID == ITEMID_MysteriousBookRecipe) )
		{
			if ( !CHARACTERGAME->CheckInventorySpace( ITEMHANDLER->FindItemTableByCode( ITEMID_BrazilianSoccer ) ) )
			{
				TITLEBOX( "Not enough space in inventory" );
				CRAFTPROCCESS_START = FALSE;
			}

			if ( (UNITDATA->sCharacterData.sWeight.sCur + 200) > UNITDATA->sCharacterData.sWeight.sMax )
			{
				TITLEBOX( "Weight limit exceeded" );
				CRAFTPROCCESS_START = FALSE;
			}
		}
	}
}


BOOL CManufactureHandler::IsRecipe( EItemID eID )
{
	BOOL b = FALSE;

	switch ( eID )
	{
		case ITEMID_KelvezuAmulet:
		case ITEMID_DarkGuardianSheltom:
		case ITEMID_GhostCastleBracelets:
		case ITEMID_ArgonianGauntlets:
			b = TRUE;
			break;

		default:
			break;
	}

	if ( (eID & 0xFFFF0000) == ITEMID_RecipeCode )
		b = TRUE;

	return b;
}

BOOL CManufactureHandler::CanRespec( EItemID eID )
{
	switch ( (eID & 0xFF000000) )
	{
		case ITEMBASE_Weapon:
		case ITEMBASE_Defense:
		case ITEMBASE_Accessory:
		{
			if ( IsRune( eID ) )
				return FALSE;

			if ( (eID == ITEMID_BroyaleCostumeMale) || (eID == ITEMID_BroyaleCostumeFemale) || (eID == ITEMID_SandurrRedCostumeMale) || (eID == ITEMID_SandurrRedCostumeFemale) ||
				(eID == ITEMID_GoldenHopyCostumeMaleTime) || (eID == ITEMID_GoldenHopyCostumeFemaleTime) )
				return FALSE;

			if ( (eID & 0xFFFF0000) == ITEMTYPE_Sheltom || (eID & 0xFFFF0000) == ITEMTYPE_Amulet )
			{
				/*
				if ( eID != ITEMID_FurySheltom &&
					eID != ITEMID_ThroneSheltom &&
					eID != ITEMID_KelvezuAmulet &&
					eID != ITEMID_TullaAmulet &&
					eID != ITEMID_ThroneAmulet &&
					eID != ITEMID_GuidanceAmulet &&
					eID != ITEMID_GuidanceAmuletPlus1 &&
					eID != ITEMID_ValentoRing )
					return FALSE;
				*/
			}

			if ( IsRespecStone( eID ) )
				return FALSE;

			return TRUE;
		}
		default: 
			break;
	}

	return FALSE;
}

BOOL CManufactureHandler::IsRespecStone( EItemID eID )
{
	if ( (eID & 0xFFFFFF00) == ITEMID_SpecStoneCode )
		return TRUE;

	return FALSE;
}

BOOL CManufactureHandler::IsRune( EItemID eID )
{
	BOOL b = FALSE;

	switch ( eID )
	{
		case ITEMID_Oredo:
		case ITEMID_Bellum:
		case ITEMID_Sapphire:
		case ITEMID_HopyRune:
		case ITEMID_CuepyRune:
		case ITEMID_GoldenHopyRune:
			b = TRUE;
			break;

		default:
			break;
	}

	if ( ((eID & 0xFFFF0000) == ITEMID_RuneCodeManufacture1) || ((eID & 0xFFFF0000) == ITEMTYPE_Crafting2) )
		b = TRUE;
	if ( (eID & 0xFFFF0000) == ITEMTYPE_Respec )
		b = TRUE;

	return b;
}

BOOL CManufactureHandler::IsValidRuneTullaAmulet( EItemID eID )
{
	BOOL bValid = FALSE;
	switch ( eID )
	{
		case ITEMID_Oredo:
		case ITEMID_LightBlueRune:
			bValid = TRUE;
			break;

		default:
			break;
	}

	// Check if have items
	if ( bValid )
	{
		struct ManufactureItem
		{
			ItemData					sItemData[4];
			int							iRuneIndex[12];
		};

		ManufactureItem * psManufactureData = (ManufactureItem*)0x0368D9A0;
		int iOredos = 0;
		int iLight = 0;

		for ( int i = 0; i < 4; i++ )
		{
			int iItemID = psManufactureData->sItemData[i].sBaseItemID.ToInt();

			if ( iItemID == ITEMID_Oredo )
				iOredos++;
			else if ( iItemID == ITEMID_LightBlueRune )
				iLight++;
		}

		if ( (iOredos >= 2 && eID == ITEMID_Oredo) || (iLight >= 1 && eID == ITEMID_LightBlueRune) )
			bValid = FALSE;
	}

	return bValid;
}

BOOL CManufactureHandler::IsValidRuneThroneAmulet(EItemID eID)
{
	BOOL bValid = FALSE;
	switch (eID)
	{
	case ITEMID_Sapphire:
	case ITEMID_ThroneRedRune:
		bValid = TRUE;
		break;

	default:
		break;
	}

	// Check if have items
	if (bValid)
	{
		struct ManufactureItem
		{
			ItemData					sItemData[4];
			int							iRuneIndex[12];
		};

		ManufactureItem* psManufactureData = (ManufactureItem*)0x0368D9A0;
		int iOredos = 0;
		int iLight = 0;

		for (int i = 0; i < 4; i++)
		{
			int iItemID = psManufactureData->sItemData[i].sBaseItemID.ToInt();

			if (iItemID == ITEMID_Sapphire )
				iOredos++;
			else if (iItemID == ITEMID_ThroneRedRune )
				iLight++;
		}

		if ((iOredos >= 2 && eID == ITEMID_Sapphire) || (iLight >= 1 && eID == ITEMID_ThroneRedRune) )
			bValid = FALSE;
	}

	return bValid;
}

BOOL CManufactureHandler::IsValidRuneAbyssRing( EItemID eID )
{
	BOOL bValid = FALSE;
	switch ( eID )
	{
		case ITEMID_Oredo:
		case ITEMID_LightGreenRune:
			bValid = TRUE;
			break;

		default:
			break;
	}

	// Check if have items
	if ( bValid )
	{
		struct ManufactureItem
		{
			ItemData					sItemData[4];
			int							iRuneIndex[12];
		};

		ManufactureItem * psManufactureData = (ManufactureItem*)0x0368D9A0;
		int iOredos = 0;
		int iLight = 0;

		for ( int i = 0; i < 4; i++ )
		{
			int iItemID = psManufactureData->sItemData[i].sBaseItemID.ToInt();

			if ( iItemID == ITEMID_Oredo )
				iOredos++;
			else if ( iItemID == ITEMID_LightGreenRune )
				iLight++;
		}

		if ( (iOredos >= 2 && eID == ITEMID_Oredo) || (iLight >= 1 && eID == ITEMID_LightGreenRune) )
			bValid = FALSE;
	}

	return bValid;
}

BOOL CManufactureHandler::IsValidRuneFurySheltom( EItemID eID )
{
	BOOL bValid = FALSE;
	switch ( eID )
	{
		case ITEMID_DarkGuardianSheltom:
		case ITEMID_GreenCrystalRune:
		case ITEMID_RedCrystalRune:
		case ITEMID_BlueCrystalRune:
			bValid = TRUE;
			break;

		default:
			break;
	}

	// Check if have items
	if ( bValid )
	{
		struct ManufactureItem
		{
			ItemData					sItemData[4];
			int							iRuneIndex[12];
		};

		ManufactureItem * psManufactureData = (ManufactureItem *)0x0368D9A0;
		int iDGS = 0;
		int iaCrystal[3] = { 0,0,0 };

		for ( int i = 0; i < 4; i++ )
		{
			int iItemID = psManufactureData->sItemData[i].sBaseItemID.ToInt();

			if ( iItemID == ITEMID_DarkGuardianSheltom )
				iDGS++;
			else if ( iItemID == ITEMID_GreenCrystalRune )
				iaCrystal[0]++;
			else if ( iItemID == ITEMID_RedCrystalRune )
				iaCrystal[1]++;
			else if ( iItemID == ITEMID_BlueCrystalRune )
				iaCrystal[2]++;
		}

		if ( (iDGS == 1 && eID == ITEMID_DarkGuardianSheltom) || 
			(iaCrystal[0] == 1 && eID == ITEMID_GreenCrystalRune) ||
			(iaCrystal[1] == 1 && eID == ITEMID_RedCrystalRune) ||
			(iaCrystal[2] == 1 && eID == ITEMID_BlueCrystalRune) )
			bValid = FALSE;
	}

	return bValid;
}

BOOL CManufactureHandler::IsValidRuneThroneSheltom(EItemID eID)
{
	BOOL bValid = FALSE;
	switch (eID)
	{
	case ITEMID_FurySheltom:
	case ITEMID_GoldCrystalRune:
	case ITEMID_SilverCrystalRune:
	case ITEMID_BronzeCrystalRune:
		bValid = TRUE;
		break;

	default:
		break;
	}

	// Check if have items
	if (bValid)
	{
		struct ManufactureItem
		{
			ItemData					sItemData[4];
			int							iRuneIndex[12];
		};

		ManufactureItem* psManufactureData = (ManufactureItem*)0x0368D9A0;
		int iDGS = 0;
		int iaCrystal[3] = { 0,0,0 };

		for (int i = 0; i < 4; i++)
		{
			int iItemID = psManufactureData->sItemData[i].sBaseItemID.ToInt();

			if (iItemID == ITEMID_FurySheltom)
				iDGS++;
			else if (iItemID == ITEMID_GoldCrystalRune)
				iaCrystal[0]++;
			else if (iItemID == ITEMID_SilverCrystalRune)
				iaCrystal[1]++;
			else if (iItemID == ITEMID_BronzeCrystalRune)
				iaCrystal[2]++;
		}

		if ((iDGS == 1 && eID == ITEMID_FurySheltom) ||
			(iaCrystal[0] == 1 && eID == ITEMID_GoldCrystalRune) ||
			(iaCrystal[1] == 1 && eID == ITEMID_SilverCrystalRune) ||
			(iaCrystal[2] == 1 && eID == ITEMID_BronzeCrystalRune))
			bValid = FALSE;
	}

	return bValid;
}

BOOL CManufactureHandler::HasFurySheltomRune( EItemID eID )
{
	BOOL bRet = FALSE;

	struct ManufactureItem
	{
		ItemData					sItemData[4];
		int							iRuneIndex[12];
	};

	ManufactureItem * psManufactureData = (ManufactureItem *)0x0368D9A0;
	int iDGS = 0;
	int iaCrystal[3] = { 0,0,0 };

	for ( int i = 0; i < 4; i++ )
	{
		int iItemID = psManufactureData->sItemData[i].sBaseItemID.ToInt();

		if ( eID == iItemID )
		{
			bRet = TRUE;

			break;
		}
	}

	return bRet;
}

BOOL CManufactureHandler::HasThroneSheltomRune(EItemID eID)
{
	BOOL bRet = FALSE;

	struct ManufactureItem
	{
		ItemData					sItemData[4];
		int							iRuneIndex[12];
	};

	ManufactureItem* psManufactureData = (ManufactureItem*)0x0368D9A0;
	int iDGS = 0;
	int iaCrystal[3] = { 0,0,0 };

	for (int i = 0; i < 4; i++)
	{
		int iItemID = psManufactureData->sItemData[i].sBaseItemID.ToInt();

		if (eID == iItemID)
		{
			bRet = TRUE;

			break;
		}
	}

	return bRet;
}

BOOL CManufactureHandler::HaveSlot( EItemID iItemRecipeID, EItemID eRuneID, int iRuneIndex, bool bUniqueSlot, EItemID iItemSlot1ID, int iCount1, EItemID iItemSlot2ID, int iCount2, EItemID iItemSlot3ID, int iCount3 )
{
	struct ManufactureItem
	{
		ItemData					sItemData[4];
		int							iRuneIndex[12];
	};

	ManufactureItem * psManufactureData = (ManufactureItem *)0x0368D9A0;

	int iaCount[3] = {iCount1,iCount2,iCount3};

	if ( (psManufactureData->sItemData[0].sItem.sItemID.ToItemID() == iItemRecipeID) || (iItemRecipeID == ITEMID_None) )
	{
		if ( bUniqueSlot )
		{
			if ( (iItemSlot1ID != ITEMID_None) && (psManufactureData->sItemData[1].sItem.sItemID.ToItemID() == iItemSlot1ID) )
				iaCount[0]--;
			if ( (iItemSlot2ID != ITEMID_None) && (psManufactureData->sItemData[2].sItem.sItemID.ToItemID() == iItemSlot2ID) )
				iaCount[1]--;
			if ( (iItemSlot3ID != ITEMID_None) && (psManufactureData->sItemData[3].sItem.sItemID.ToItemID() == iItemSlot3ID) )
				iaCount[2]--;

			if ( iaCount[iRuneIndex] > 0 )
				return TRUE;
		}
		else
		{
			for ( int i = 1; i < 4; i++ )
			{
				if ( (iItemSlot1ID != ITEMID_None) && (psManufactureData->sItemData[i].sItem.sItemID.ToItemID() == iItemSlot1ID) )
					iCount1--;
				if ( (iItemSlot2ID != ITEMID_None) && (psManufactureData->sItemData[i].sItem.sItemID.ToItemID() == iItemSlot2ID) )
					iCount2--;
				if ( (iItemSlot3ID != ITEMID_None) && (psManufactureData->sItemData[i].sItem.sItemID.ToItemID() == iItemSlot3ID) )
					iCount3--;
			}

			if ( eRuneID == iItemSlot1ID )
				return iCount1 > 0 ? TRUE : FALSE;
			if ( eRuneID == iItemSlot2ID )
				return iCount2 > 0 ? TRUE : FALSE;
			if ( eRuneID == iItemSlot3ID )
				return iCount3 > 0 ? TRUE : FALSE;
		}
	}

	return FALSE;
}
