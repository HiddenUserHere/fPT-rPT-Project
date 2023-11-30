#include "stdafx.h"
#include "ItemWindowHandler.h"

typedef BOOL( __thiscall *tfnCheckItemSell )(DWORD dwClass, ItemData * psItemData);
tfnCheckItemSell fnCheckItemSell = (tfnCheckItemSell)0x004E4290;

typedef void( __thiscall *tfnSellItem )(DWORD dwClass, ItemData * psItemData, BOOL bUnk);
tfnSellItem fnSellItem = (tfnSellItem)0x004E4090;

typedef void( __thiscall *tfnSellItemBox )(DWORD dwClass, int iUnk, char * pszName);
tfnSellItemBox fnSellItemBox = (tfnSellItemBox)0x004D1680;

typedef void( __thiscall *tfnSetItemPersonalShop )(DWORD dwClass, ItemData * psItemData);
tfnSetItemPersonalShop fnSetItemPersonalShop = (tfnSetItemPersonalShop)0x004E43A0;

CItemWindowHandler::CItemWindowHandler()
{
}


CItemWindowHandler::~CItemWindowHandler()
{
}

NAKED int CItemWindowHandler::GetPersonamShopGoldAmount()
{
	JMP( pfnGetAmountPersonalShop );
}

void CItemWindowHandler::OnOkButtonClickUnstackPotion( ItemData * psIn, ItemData * psOut, int iType )
{
	if ( INPUTBOX->GetType() || psIn->sItem.iPotionCount == 1 )
	{
		int iCount			= psIn->sItem.iPotionCount == 1 ? psIn->sItem.iPotionCount : INPUTBOX->GetValue();
		if ( iCount > 0 )
		{
			CopyMemory( psOut, psIn, sizeof( ItemData ) );

			if ( iType == UNSTACKPOTION_WarehousePut )
			{
				psOut->sItem.iPotionCount = iCount;
				psIn->sItem.iPotionCount -= iCount;

				// Invalidate Item
				if ( psIn->sItem.iPotionCount <= 0 )
					psIn->bValid = FALSE;

				// iSound
				PLAYMINISOUND( psIn->iInvItemSound );

				//Set iItemSlot
				psOut->sPosition.iX = psOut->sPlacePosition.iX;
				psOut->sPosition.iY = psOut->sPlacePosition.iY;

				CALL_WITH_ARG1( 0x0045BB30, (DWORD)&psOut->sItem );

				// Validate Inventory
				ITEMHANDLER->ValidateInventoryItems();
				ITEMHANDLER->ResetInventoryItemChecksum();
				CALLT( 0x0050E510, 0x036932FC );

				CHARACTERGAME->UpdateWeight();
			}
			else if ( iType == UNSTACKPOTION_WarehouseGet )
			{

				ItemData sItemData;
				CopyMemory( &sItemData, psOut, sizeof( ItemData ) );
				sItemData.sItem.iPotionCount = iCount;
				psIn->sItem.iPotionCount -= iCount;

				// Invalidate Item
				if ( psIn->sItem.iPotionCount <= 0 )
					psIn->bValid = FALSE;

				CALL_WITH_ARG1( 0x0045BB30, (DWORD)&sItemData.sItem );

				// Put Item Back to Inventory
				ITEMHANDLER->BackItemToInventory( &sItemData );
				ITEMHANDLER->ResetInventoryItemChecksum();

				// iSound
				PLAYMINISOUND( psIn->iInvItemSound );

				// Validate Inventory
				CALLT( 0x0050E510, 0x036932FC );
			}
			else if ( iType == UNSTACKPOTION_TradePut )
			{
				psOut->sItem.iPotionCount = iCount;
				psIn->sItem.iPotionCount -= iCount;

				// Invalidate Item
				if ( psIn->sItem.iPotionCount <= 0 )
					psIn->bValid = FALSE;

				// iSound
				PLAYMINISOUND( psIn->iInvItemSound );

				//Set iItemSlot
				psOut->sPosition.iX = psOut->sPlacePosition.iX;
				psOut->sPosition.iY = psOut->sPlacePosition.iY;

				CALL_WITH_ARG1( 0x0045BB30, (DWORD)&psOut->sItem );

				// Validate Inventory
				ITEMHANDLER->ValidateInventoryItems();
				ITEMHANDLER->ResetInventoryItemChecksum();

				(*(DWORD*)0x036EFCC8) = TRUE;
				(*(DWORD*)0x0369D820) = FALSE;

				CALL_WITH_ARG1( 0x0045BCD0, (DWORD)0x03699A88 );
				CALLT( 0x0050C6A0, 0x036EFD98 );
				CALL_WITH_ARG1( 0x00628410, (DWORD)(*(DWORD*)(0x036EFD98 + 0x171AC)) );
				CALLT( 0x0050DFD0, 0x036EFD98 );

				CHARACTERGAME->UpdateWeight();
			}
			else if ( iType == UNSTACKPOTION_TradeGet )
			{
				ItemData sItemData;
				CopyMemory( &sItemData, psOut, sizeof( ItemData ) );
				sItemData.sItem.iPotionCount = iCount;
				psIn->sItem.iPotionCount -= iCount;

				// Invalidate Item
				if ( psIn->sItem.iPotionCount <= 0 )
					psIn->bValid = FALSE;

				CALL_WITH_ARG1( 0x0045BB30, (DWORD)&sItemData.sItem );


				// Put Item Back to Inventory
				ITEMHANDLER->BackItemToInventory( &sItemData );
				ITEMHANDLER->ResetInventoryItemChecksum();

				// iSound
				PLAYMINISOUND( psOut->iInvItemSound );

				(*(DWORD*)0x036EFCC8) = TRUE;
				(*(DWORD*)0x0369D820) = FALSE;

				CALL_WITH_ARG1( 0x0045BCD0, (DWORD)0x03699A88 );
				CALLT( 0x0050C6A0, 0x036EFD98 );
				CALL_WITH_ARG1( 0x00628410, (DWORD)(*(DWORD*)(0x036EFD98 + 0x171AC)) );
				CALLT( 0x0050DFD0, 0x036EFD98 );
			}
			else if ( iType == UNSTACKPOTION_MouseGet )
			{
				psOut->sItem.iPotionCount = iCount;
				psIn->sItem.iPotionCount -= iCount;

				// Invalidate Item
				if ( psIn->sItem.iPotionCount <= 0 )
					psIn->bValid = FALSE;

				// iSound
				PLAYMINISOUND( psIn->iInvItemSound );

				CALL_WITH_ARG1( 0x0045BB30, (DWORD)&psOut->sItem );

				// Put Item Back to Inventory
				ITEMHANDLER->ResetInventoryItemChecksum();
			}
		}
	}
}

int CItemWindowHandler::OnUnstackPotion( ItemData * psIn, ItemData * psOut, int iType )
{
	if ( psIn->sItem.iPotionCount <= 1 )
	{
		OnOkButtonClickUnstackPotion( psIn, psOut, iType );
		return TRUE;
	}

	INPUTBOX->SetTitle( "Unstack Potions" );

	if ( iType == UNSTACKPOTION_WarehouseGet || iType == UNSTACKPOTION_TradeGet || iType == UNSTACKPOTION_FieldGet )
		INPUTBOX->SetDescription( "How many potions would you like to take?" );
	else
		INPUTBOX->SetDescription( "How many potions would you like to put?" );

	int iCounter = psIn->sItem.iPotionCount;
	if ( (UNITDATA->sCharacterData.sWeight.sCur + (short)iCounter) > UNITDATA->sCharacterData.sWeight.sMax )
		iCounter = (int)(UNITDATA->sCharacterData.sWeight.sMax - UNITDATA->sCharacterData.sWeight.sCur);

	INPUTBOX->SetValue( "1" );
	INPUTBOX->SetNumberType( 1, iCounter, TRUE );
	INPUTBOX->SetEvent( std::bind( &CItemWindowHandler::OnOkButtonClickUnstackPotion, this, psIn, psOut, iType ) );
	INPUTBOX->Show();
	return TRUE;
}

BOOL CItemWindowHandler::OnWarehouseAutoPlaceItem( ItemData * psItemData )
{
	if ( IsWarehouseUse( psItemData ) == FALSE )
		return FALSE;

	// Items in warehouse from current page
	ItemData * psaDataWarehouse = WAREHOUSEHANDLER->GetItemData() + ((WAREHOUSEHANDLER->GetPage() - 1) * 81);

	EItemType eItemType = psItemData->sBaseItemID.ToItemType();

	// Warehouse rect box
	RECT rWarehouseBox{ 21, 86, 21 + 200, 86 + 200 };

	int iPlaceX = -1;
	int iPlaceY = -1;

	// Rows
	for ( int i = 0; i < 9; i++ )
	{
		BOOL bDone = FALSE;

		// Columns
		for ( int j = 0; j < 9; j++ )
		{
			int iRelativeX = (j * 22);
			int iRelativeY = (i * 22);

			RECT rPlaceRect;
			rPlaceRect.left		= rWarehouseBox.left + iRelativeX;
			rPlaceRect.top		= rWarehouseBox.top + iRelativeY;
			rPlaceRect.right	= rPlaceRect.left + psItemData->iWidth;
			rPlaceRect.bottom	= rPlaceRect.top + psItemData->iHeight;

			// Rectangle exceeds size of Warehouse Box?
			if ( rPlaceRect.right > rWarehouseBox.right || rPlaceRect.bottom > rWarehouseBox.bottom )
				continue;

			// Colliding with existing items in the Warehouse Box?
			BOOL bCollides = FALSE;
			for ( int k = 0; k < 81; k++ )
			{
				ItemData * psWarehouseItem = psaDataWarehouse + k;

				if ( !psWarehouseItem->bValid )
					continue;

				RECT rWarehouseItemRect;
				rWarehouseItemRect.left = psWarehouseItem->sPosition.iX;
				rWarehouseItemRect.top = psWarehouseItem->sPosition.iY;
				rWarehouseItemRect.right = rWarehouseItemRect.left + psWarehouseItem->iWidth - 1;
				rWarehouseItemRect.bottom = rWarehouseItemRect.top + psWarehouseItem->iHeight - 1;

				if ( rPlaceRect.left < rWarehouseItemRect.right && rPlaceRect.right > rWarehouseItemRect.left &&
						rPlaceRect.top < rWarehouseItemRect.bottom && rPlaceRect.bottom > rWarehouseItemRect.top )
				{
					bCollides = TRUE;
					break;
				}
			}

			if( bCollides )
				continue;

			//Found spot
			iPlaceX = rPlaceRect.left;
			iPlaceY = rPlaceRect.top;

			bDone = TRUE;
			break;
		}

		if( bDone )
			break;
	}

	// Not Space found?
	if( iPlaceX == -1 )
		return FALSE;

	// Get Free Slot
	int iFreeSlot = -1;
	for( int i = 0; i < 405; i++ )
	{
		if ( !psaDataWarehouse[i].bValid )
		{
			iFreeSlot = i;
			break;
		}
	}

	// No free slot in Warehouse?
	if( iFreeSlot == -1 )
		return FALSE;

	ItemData * psFreeSlot = psaDataWarehouse + iFreeSlot;

	if ( psItemData->sBaseItemID.ToItemBase() == ITEMBASE_Potion && KEYBOARDHANDLER->IsKeyDown( VK_CONTROL ) )
	{
		//Set iItemSlot
		psItemData->sPlacePosition.iX = iPlaceX;
		psItemData->sPlacePosition.iY = iPlaceY;

		OnUnstackPotion( psItemData, psFreeSlot, UNSTACKPOTION_WarehousePut );

		return TRUE;
	}

	// Copy Item Data to Free Slot of Warehouse
	CopyMemory( psFreeSlot, psItemData, sizeof( ItemData ) );
	
	// Invalidate Item
	psItemData->bValid = FALSE;

	// iSound
	PLAYMINISOUND( psItemData->iInvItemSound );

	//Set iItemSlot
	psFreeSlot->sPosition.iX = iPlaceX;
	psFreeSlot->sPosition.iY = iPlaceY;

	// Validate Inventory
	ITEMHANDLER->ValidateInventoryItems();
	ITEMHANDLER->ResetInventoryItemChecksum();
	CALLT( 0x0050E510, 0x036932FC );

	CHARACTERGAME->UpdateWeight();

	//Done!
	return TRUE;

}

BOOL CItemWindowHandler::OnTradeAutoPlaceItem( ItemData * psItemData )
{
	if ( TRADEHANDLER->IsDelayTime() )
		return FALSE;

	// Items in warehouse from current page
	ItemData * psaDataTrade = ((ItemData*)0x03699A88);

	EItemType eItemType = psItemData->sBaseItemID.ToItemType();

	// Warehouse rect box
	RECT rTradeBox{ 21, 195, 21 + (22*9), 195 + (22*4) };

	int iPlaceX = -1;
	int iPlaceY = -1;

	if ( (psItemData->sItem.bEnableEffect && !IsTradeUse( &psItemData->sItem )) || (psItemData->sItem.bLockedItemType) )
	{
		TITLEBOX( "This item cannot be place on trade!" );
		return TRUE;
	}

	// Rows
	for ( int i = 0; i < 4; i++ )
	{
		BOOL bDone = FALSE;

		// Columns
		for ( int j = 0; j < 9; j++ )
		{
			int iRelativeX = (j * 22);
			int iRelativeY = (i * 22);

			RECT rPlaceRect;
			rPlaceRect.left		= rTradeBox.left + iRelativeX;
			rPlaceRect.top		= rTradeBox.top + iRelativeY;
			rPlaceRect.right	= rPlaceRect.left + psItemData->iWidth;
			rPlaceRect.bottom	= rPlaceRect.top + psItemData->iHeight;

			// Rectangle exceeds size of Warehouse Box?
			if ( rPlaceRect.right > rTradeBox.right || rPlaceRect.bottom > rTradeBox.bottom )
				continue;

			// Colliding with existing items in the Warehouse Box?
			BOOL bCollides = FALSE;
			for ( int k = 0; k < 20; k++ )
			{
				ItemData * psTradeItem = psaDataTrade + k;

				if ( !psTradeItem->bValid )
					continue;

				RECT rTradeItemRect;
				rTradeItemRect.left = psTradeItem->sPosition.iX;
				rTradeItemRect.top = psTradeItem->sPosition.iY;
				rTradeItemRect.right = rTradeItemRect.left + psTradeItem->iWidth - 1;
				rTradeItemRect.bottom = rTradeItemRect.top + psTradeItem->iHeight - 1;

				if ( rPlaceRect.left < rTradeItemRect.right && rPlaceRect.right > rTradeItemRect.left &&
						rPlaceRect.top < rTradeItemRect.bottom && rPlaceRect.bottom > rTradeItemRect.top )
				{
					bCollides = TRUE;
					break;
				}
			}

			if( bCollides )
				continue;

			//Found spot
			iPlaceX = rPlaceRect.left;
			iPlaceY = rPlaceRect.top;

			bDone = TRUE;
			break;
		}

		if( bDone )
			break;
	}

	// Not Space found?
	if( iPlaceX == -1 )
		return FALSE;

	// Get Free Slot
	int iFreeSlot = -1;
	for( int i = 0; i < 20; i++ )
	{
		if ( !psaDataTrade[i].bValid )
		{
			iFreeSlot = i;
			break;
		}
	}

	// No free slot in Warehouse?
	if( iFreeSlot == -1 )
		return FALSE;

	ItemData * psFreeSlot = psaDataTrade + iFreeSlot;

	if ( psItemData->sBaseItemID.ToItemBase() == ITEMBASE_Potion && KEYBOARDHANDLER->IsKeyDown( VK_CONTROL ) )
	{
		//Set iItemSlot
		psItemData->sPlacePosition.iX = iPlaceX;
		psItemData->sPlacePosition.iY = iPlaceY;

		OnUnstackPotion( psItemData, psFreeSlot, UNSTACKPOTION_TradePut );

		return TRUE;
	}

	// Copy Item Data to Free Slot of Warehouse
	CopyMemory( psFreeSlot, psItemData, sizeof( ItemData ) );
	
	// Invalidate Item
	psItemData->bValid = FALSE;

	// iSound
	PLAYMINISOUND( psItemData->iInvItemSound );

	//Set iItemSlot
	psFreeSlot->sPosition.iX = iPlaceX;
	psFreeSlot->sPosition.iY = iPlaceY;

	// Validate Inventory
	ITEMHANDLER->ValidateInventoryItems();
	ITEMHANDLER->ResetInventoryItemChecksum();
	
	(*(DWORD*)0x036EFCC8) = TRUE;
	(*(DWORD*)0x0369D820) = FALSE;

	CALL_WITH_ARG1( 0x0045BCD0, ( DWORD )0x03699A88 );
	CALLT( 0x0050C6A0, 0x036EFD98 );
	CALL_WITH_ARG1( 0x00628410, ( DWORD )( *( DWORD* )( 0x036EFD98 + 0x171AC ) ) );
	CALLT( 0x0050DFD0, 0x036EFD98 );

	CHARACTERGAME->UpdateWeight();

	//Done!
	return TRUE;

}

BOOL CItemWindowHandler::IsSellItem( ItemData * psItemData )
{
	BOOL bResult = FALSE;
	EItemBase eBase = psItemData->sBaseItemID.ToItemBase();

	if ( !bResult && eBase != ITEMBASE_Potion && eBase != ITEMBASE_Premium && eBase != ITEMBASE_Quest1 && eBase != ITEMBASE_Quest2 )
		bResult = TRUE;

	switch ( psItemData->sItem.sItemID.ToItemID() )
	{
		case ITEMID_TopazStone:
		case ITEMID_EmeraldStone:
		case ITEMID_RubyStone:
		case ITEMID_SapphireStone:
		case ITEMID_PhoenixEgg:
		case ITEMID_TeleportCore:
		case ITEMID_CleanItem:
		case ITEMID_CopperOre:
		case ITEMID_AgingStone:
		case ITEMID_RipeStone:
		case ITEMID_FluenceStone:
		case ITEMID_ArcaneStone:
		case ITEMID_BigHeadHappyness:
		case ITEMID_BigHeadLove:
		case ITEMID_BigHeadSadness:
		case ITEMID_BigHeadShyness:
		case ITEMID_BigHeadAngry:
		case ITEMID_BigHeadSurprised:
		case ITEMID_BigHeadSensual:
		case ITEMID_SheepHat:
		case ITEMID_GiraffeHat:
		case ITEMID_SoccerHat:
		case ITEMID_XMasGreenHat:
		case ITEMID_XMasRedHat:
		case ITEMID_CartolaHat:
		case ITEMID_WitchHat:
		case ITEMID_HalloweenHat:
		case ITEMID_TuttiFruttiHat:
		case ITEMID_EasterHat:
		case ITEMID_ChemistHat:
		case ITEMID_PatronumHat:
		case ITEMID_GoldenHopyHat:
		case ITEMID_ObscureHat:
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
		case ITEMID_IceHair:
		case ITEMID_SamuraiHair:
			bResult = TRUE;
			break;

		case ITEMID_MechanicCostumeM:
		case ITEMID_MechanicCostumeF:
		case ITEMID_GoldBag:
		case ITEMID_BlessOfKalia:
			bResult = FALSE;
			break;
		default:
			break;
	}

	if ( bResult && ACCOUNTHANDLER->IsAccountShareDenied( ACCOUNTSHARE_DenyBuySellItems ) )
	{
		bResult = FALSE;
		TITLEBOX( AY_OBFUSCATE( "You can't Buy/Sell Items, because you're logged in with the Share PW!" ) );
	}

	return bResult;
}

BOOL CItemWindowHandler::IsUseMixNPCItem( Item * psItem )
{
	switch ( psItem->sItemID.ToItemID() )
	{
		case ITEMID_BeginnerAxe:
		case ITEMID_BeginnerClaw:
		case ITEMID_BeginnerHammer:
		case ITEMID_BeginnerJavelin:
		case ITEMID_BeginnerScythe:
		case ITEMID_BeginnerBoots:
		case ITEMID_BeginnerGauntlet:
		case ITEMID_BeginnerBracelet:
		case ITEMID_BeginnerBow:
		case ITEMID_BeginnerDagger:
		case ITEMID_BeginnerPhantom:
		case ITEMID_BeginnerStaff:
		case ITEMID_BeginnerSword:
		case ITEMID_GrumpyArmor:
		case ITEMID_GrumpyRobe:

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
		case ITEMID_CandySheltom:

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

		case ITEMID_IceCostumeMale:
		case ITEMID_IceCostumeFemale:
		case ITEMID_SandurrRedCostumeMale:
		case ITEMID_SandurrRedCostumeFemale:
		case ITEMID_SandurrPurpleCostumeMale:
		case ITEMID_SandurrPurpleCostumeFemale:
		case ITEMID_GoldenHopyCostumeMaleTime:
		case ITEMID_GoldenHopyCostumeFemaleTime:

			return FALSE;
		default:
			break;
	}

	if ( UI::ItemInfoBox::GetItemPlayTime( psItem ) )
		return FALSE;

	return TRUE;
}

BOOL CItemWindowHandler::IsUseMixNPC( Item * psItem )
{
	if ( UI::ItemInfoBox::GetItemPlayTime( psItem ) )
		return FALSE;

	Rectangle2D rRect{ 58, 142, 66, 88 };
	switch ( psItem->sItemID.ToItemID() )
	{
		case ITEMID_FurySheltom:
		case ITEMID_ThroneSheltom:
			if ( !rRect.Inside( MOUSEHANDLER->GetPosition() ) )
			{
				TITLEBOX( "This item cannot be mixed" );
				return FALSE;
			}
			return TRUE;
			break;

		case ITEMID_BeginnerHammer:
		case ITEMID_BeginnerJavelin:
		case ITEMID_BeginnerScythe:
		case ITEMID_BeginnerBoots:
		case ITEMID_BeginnerGauntlet:
		case ITEMID_BeginnerBracelet:
		case ITEMID_BeginnerBow:
		case ITEMID_BeginnerDagger:
		case ITEMID_BeginnerPhantom:
		case ITEMID_BeginnerStaff:
		case ITEMID_BeginnerSword:
		case ITEMID_GrumpyArmor:
		case ITEMID_GrumpyRobe:
			return FALSE;

		default:
			break;
	}
	if ( psItem->sItemID.ToItemType() == ITEMTYPE_Sheltom )
	{
		ItemData * psItemData = (ItemData*)(0x03680358);
		if ( psItemData->bValid )
		{
			if ( psItemData->sItem.sItemID.ToItemID() == ITEMID_KelvezuAmulet || psItemData->sItem.sItemID.ToItemID() == ITEMID_ValentoRing ||
				psItemData->sItem.sItemID.ToItemID() == ITEMID_TullaAmulet || psItemData->sItem.sItemID.ToItemID() == ITEMID_FurySheltom ||
				psItemData->sItem.sItemID.ToItemID() == ITEMID_MokovaBoots || psItemData->sItem.sItemID.ToItemID() == ITEMID_DarkGuardianSheltom ||
				 psItemData->sItem.sItemID.ToItemID() == ITEMID_ArgonianGauntlets || psItemData->sItem.sItemID.ToItemID() == ITEMID_DevilShyRing ||
				 psItemData->sItem.sItemID.ToItemID() == ITEMID_XMasCloatCostumeMF || psItemData->sItem.sItemID.ToItemID() == ITEMID_XMasCloatCostumeTF ||
				 psItemData->sItem.sItemID.ToItemID() == ITEMID_XMasCloatCostumeMM || psItemData->sItem.sItemID.ToItemID() == ITEMID_XMasCloatCostumeTM ||
				 psItemData->sItem.sItemID.ToItemID() == ITEMID_GhostCastleBracelets || psItemData->sItem.sItemID.ToItemID() == ITEMID_DraxosBoots ||
				 psItemData->sItem.sItemID.ToItemID() == ITEMID_TropicalBracelets || psItemData->sItem.sItemID.ToItemID() == ITEMID_ThroneSheltom ||
				 psItemData->sItem.sItemID.ToItemID() == ITEMID_GreedyGauntlets || psItemData->sItem.sItemID.ToItemID() == ITEMID_ThroneRing ||
				 psItemData->sItem.sItemID.ToItemID() == ITEMID_ThroneAmulet )
			{
				TITLEBOX( "This item can only be respec" );
				return FALSE;
			}
		}
		else
		{
			//Force
			if ( (*(BOOL*)0x0368033C) )
				return TRUE;

			if ( MIXWINDOW_OPEN == FALSE )
				return TRUE;

			if ( psItem->sItemID.ToItemID() == ITEMID_DarkGuardianSheltom )
				return TRUE;

			TITLEBOX( "Please insert an item first!" );
			return FALSE;
		}
		return TRUE;
	}

	if ( (psItem->sItemID.ToInt() & 0xFFFFFF00) == ITEMID_SpecStoneCode )
	{
		TITLEBOX( "Use craft npc to respec with this stone!" );
		return FALSE;
	}

	if ( psItem->sItemID.ToItemType() == ITEMTYPE_EventPR5 )
	{
		ItemData * psItemData = (ItemData*)(0x03680358);
		if ( psItemData->bValid )
		{
			if ( psItemData->sItem.sItemID.ToItemID() == ITEMID_PurpleDiamond || psItemData->sItem.sItemID.ToItemID() == ITEMID_SilverDiamond || psItemData->sItem.sItemID.ToItemID() == ITEMID_GoldDiamond )
			{
				return TRUE;
			}
		}
		return FALSE;
	}

	return FALSE;
}

BOOL CItemWindowHandler::IsUseAgingNPC( ItemData * psItemData )
{
	BOOL bRet = TRUE;

	switch ( psItemData->sItem.sItemID.ToItemID() )
	{
		case ITEMID_BeginnerAxe:
		case ITEMID_BeginnerClaw:
		case ITEMID_BeginnerHammer:
		case ITEMID_BeginnerJavelin:
		case ITEMID_BeginnerScythe:
		case ITEMID_BeginnerBoots:
		case ITEMID_BeginnerGauntlet:
		case ITEMID_BeginnerBracelet:
		case ITEMID_BeginnerBow:
		case ITEMID_BeginnerDagger:
		case ITEMID_BeginnerPhantom:
		case ITEMID_BeginnerStaff:
		case ITEMID_BeginnerSword:
		case ITEMID_GrumpyArmor:
		case ITEMID_GrumpyRobe:
		case ITEMID_XMasCloatCostumeMF:
		case ITEMID_XMasCloatCostumeTF:
		case ITEMID_XMasCloatCostumeMM:
		case ITEMID_XMasCloatCostumeTM:

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

		case ITEMID_IceCostumeMale:
		case ITEMID_IceCostumeFemale:
		case ITEMID_GoldenHopyCostumeMale:
		case ITEMID_GoldenHopyCostumeFemale:
		case ITEMID_GoldenHopyCostumeMaleTime:
		case ITEMID_GoldenHopyCostumeFemaleTime:

			bRet = FALSE;
			break;

		default:
			break;
	}

	if ( UI::ItemInfoBox::GetItemPlayTime( &psItemData->sItem ) && (psItemData->sItem.bLockedItemType == ITEMLOCKEDTYPE_None) )
		bRet = FALSE;

	return bRet;
}

BOOL CItemWindowHandler::IsLockedItem( ItemData * pcItemData )
{
	BOOL bRet = FALSE;
	switch ( pcItemData->sItem.sItemID.ToItemID() )
	{
		case ITEMID_KingStone:
			bRet = TRUE;
			break;
		default:
			break;
	}

	if ( ACCOUNTHANDLER->IsAccountShareDenied( ACCOUNTSHARE_DenyGrabEquipment ) )
	{
		BOOL bEquipped = FALSE;
		for ( int i = 1; i < 15; i++ )
		{
			if ( INVENTORYITEMSLOT[i - 1].iItemIndex )
			{
				ItemData * pcEquippedItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[i - 1].iItemIndex - 1];
				if ( pcEquippedItemData == pcItemData )
				{
					bEquipped = TRUE;
					break;
				}
			}
		}

		if ( bEquipped )
		{
			bRet = TRUE;
			TITLEBOX( AY_OBFUSCATE( "You can't Grab Equipments, because you're logged in with the Share PW!" ) );
		}
	}

	return bRet;
}

BOOL CItemWindowHandler::OnBlacksmithAutoPlaceItem( ItemData * psItemData )
{
	if ( (*(BOOL*)0x03653E40) )
	{
		EItemBase eBase = psItemData->sBaseItemID.ToItemBase();

		if( eBase == ITEMBASE_Quest1 )
			return FALSE;

		if ( psItemData && IsSellItem( psItemData ) && (*(BOOL*)0x0095E418) == FALSE && ITEMINDEX_INVENTORY >= 0 )
		{
			CopyMemory( ITEMHANDLER->GetMouseItem(), psItemData, sizeof( ItemData ) );
			fnSellItemBox( 0x0362B210, 13, psItemData->sItem.szItemName );
		
			psItemData->bValid					= FALSE;
			ITEMHANDLER->GetMouseItem()->bValid = FALSE;

			ITEMHANDLER->ValidateInventoryItems();
			ITEMHANDLER->ResetInventoryItemChecksum();
			CHARACTERGAME->UpdateWeight();
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CItemWindowHandler::OnPersonalShopAutoPlaceItem( ItemData * psItemData )
{
	if ( (*(BOOL*)0x0363C110) && ITEMINDEX_INVENTORY >= 0 )
	{
		if ( IsPersonalShopUse( psItemData ) == FALSE )
			return FALSE;

		// Check already is in shop
		if ( (*(int*)(0x03653C50 + (ITEMINDEX_INVENTORY * 4))) )
			 return FALSE;

		CopyMemory( ((ItemData*)0x035E5BE0), psItemData, sizeof( ItemData ) );
		ZeroMemory( &((ItemData*)0x035E5BE0)->sItem, sizeof( Item ) );
		((ItemData*)0x035E5BE0)->sItem.iPotionCount	= psItemData->sItem.iPotionCount;
		((ItemData*)0x035E5BE0)->sItem.iChk1		= psItemData->sItem.iChk1;
		((ItemData*)0x035E5BE0)->sItem.dwVersion	= psItemData->sItem.dwVersion;
		((ItemData*)0x035E5BE0)->sItem.dwTime		= psItemData->sItem.dwTime;
		((ItemData*)0x035E5BE0)->sItem.iChk2		= psItemData->sItem.iChk2;
		((ItemData*)0x035E5BE0)->sItem.iIndex		= ITEMINDEX_INVENTORY+1;
		((ItemData*)0x035E5BE0)->sPosition.iX		= 21;
		((ItemData*)0x035E5BE0)->sPosition.iY		= 136 - 50;

		WORD wOldY				= (*(WORD*)0x04B0B22A);
		(*(WORD*)0x04B0B22A)	= 299;

		int iOldPrice = (*(int*)0x0095D650);

		fnSetItemPersonalShop( 0x0363C110, ((ItemData*)0x035E5BE0) );

		(*(int*)0x0095D650) = CHARACTERGAME->CanUseGold( GetPersonamShopGoldAmount() + iOldPrice ) ? iOldPrice : 0;
		
		(*(WORD*)0x04B0B22A)	= wOldY;
	
		return TRUE;
	}
	return FALSE;
}

BOOL CItemWindowHandler::OnWarehouseAutoPlaceItemBack( const Point2D * psPoint )
{
	// Items in warehouse from current page
	ItemData * psaDataWarehouse = WAREHOUSEHANDLER->GetItemData() + ((WAREHOUSEHANDLER->GetPage() - 1) * 81);

	// Warehouse rect box
	RECT rWarehouseBox{ 21, 86, 21 + 200, 86 + 200 };

	ItemData * psItemData = NULL;

	// Rows
	for ( int i = 0; i < 9; i++ )
	{
		// Columns
		for ( int j = 0; j < 9; j++ )
		{
			// Get Item from mouse inside
			for ( int k = 0; k < 81; k++ )
			{
				ItemData * psWarehouseItem = psaDataWarehouse + k;

				if ( !psWarehouseItem->bValid )
					continue;

				Rectangle2D rRectangle;
				rRectangle.iX		= psWarehouseItem->sPosition.iX;
				rRectangle.iY		= psWarehouseItem->sPosition.iY;
				rRectangle.iWidth	= psWarehouseItem->iWidth - 1;
				rRectangle.iHeight  = psWarehouseItem->iHeight - 1;

				if ( rRectangle.Inside( psPoint ) )
				{
					psItemData = psWarehouseItem;
					break;
				}
			}

			if ( psItemData )
				break;
		}

		if( psItemData )
			break;
	}


	// Not item selected? return
	if ( !psItemData )
		return FALSE;

	if ( psItemData->sBaseItemID.ToItemBase() == ITEMBASE_Potion && KEYBOARDHANDLER->IsKeyDown( VK_CONTROL ) )
	{

		OnUnstackPotion( psItemData, psItemData, UNSTACKPOTION_WarehouseGet );

		return TRUE;
	}

	// Check iWeight
	if ( !CHARACTERGAME->CheckInventoryWeight( psItemData ) )
	{
		TITLEBOX( "Weight limit exceeded" );
		return TRUE;
	}

	// Check Space
	if ( !CHARACTERGAME->CheckInventorySpace( psItemData ) )
	{
		TITLEBOX( "Not enough space in inventory" );
		return TRUE;
	}


	// Put Item Back to Inventory
	ITEMHANDLER->BackItemToInventory( psItemData );
	ITEMHANDLER->ResetInventoryItemChecksum();

	// Invalidate Item
	psItemData->bValid = FALSE;

	// iSound
	PLAYMINISOUND( psItemData->iInvItemSound );

	// Validate Inventory
	CALLT( 0x0050E510, 0x036932FC );


	//Done!
	return TRUE;
}

BOOL CItemWindowHandler::OnTradeAutoPlaceItemBack( const Point2D * psPoint )
{
	if ( TRADE_ACCEPT )
		return FALSE;

	if ( TRADEHANDLER->IsDelayTime() )
		return FALSE;

	// Items in warehouse from current page
	ItemData * psaDataTrade = ((ItemData*)0x03699A88);

	// Warehouse rect box
	RECT rTradeBox{ 21, 195, 21 + (22*9), 195 + (22*4) };

	ItemData * psItemData = NULL;

	// Get Item from mouse inside
	for ( int k = 0; k < 20; k++ )
	{
		ItemData * pTradeItem = psaDataTrade + k;

		if ( !pTradeItem->bValid )
			continue;

		Rectangle2D rRectangle;
		rRectangle.iX		= pTradeItem->sPosition.iX;
		rRectangle.iY		= pTradeItem->sPosition.iY;
		rRectangle.iWidth	= pTradeItem->iWidth - 1;
		rRectangle.iHeight  = pTradeItem->iHeight - 1;

		if ( rRectangle.Inside( psPoint ) )
		{
			psItemData = pTradeItem;
			break;
		}
	}


	// Not item selected? return
	if ( !psItemData )
		return FALSE;

	// Check iWeight
	if ( !CHARACTERGAME->CheckInventoryWeight( psItemData ) )
	{
		TITLEBOX( "Weight limit exceeded" );
		return TRUE;
	}

	// Check Space
	if ( !CHARACTERGAME->CheckInventorySpace( psItemData ) )
	{
		TITLEBOX( "Not enough space in inventory" );
		return TRUE;
	}

	if ( psItemData->sBaseItemID.ToItemBase() == ITEMBASE_Potion && KEYBOARDHANDLER->IsKeyDown(VK_CONTROL) )
	{

		OnUnstackPotion( psItemData, psItemData, UNSTACKPOTION_TradeGet );

		return TRUE;
	}

	// Put Item Back to Inventory
	ITEMHANDLER->BackItemToInventory( psItemData );
	ITEMHANDLER->ResetInventoryItemChecksum();

	// Invalidate Item
	psItemData->bValid = FALSE;

	// iSound
	PLAYMINISOUND( psItemData->iInvItemSound );

	(*(DWORD*)0x036EFCC8) = TRUE;
	(*(DWORD*)0x0369D820) = FALSE;

	CALL_WITH_ARG1( 0x0045BCD0, ( DWORD )0x03699A88 );
	CALLT( 0x0050C6A0, 0x036EFD98 );
	CALL_WITH_ARG1( 0x00628410, ( DWORD )( *( DWORD* )( 0x036EFD98 + 0x171AC ) ) );
	CALLT( 0x0050DFD0, 0x036EFD98 );

	//Done!
	return TRUE;
}

BOOL CItemWindowHandler::OnMouseClick( CMouse * pcMouse )
{
	// Mouse position
	Point2D sPoint;
	sPoint.iX = pcMouse->GetPosition()->iX;
	sPoint.iY = pcMouse->GetPosition()->iY;

	ItemData * psItemData = ITEMHANDLER->GetMouseItemHover();

	if ( psItemData )
	{
		// Check Locked Items
		if ( HUDHANDLER->IsLockItems() )
		{
			TITLEBOX( "Items locked, click on unlock button in invetory to unlock items!" );
			return TRUE;
		}

		if ( IsLockedItem( psItemData ) && (GM_MODE == FALSE) )
			return TRUE;

		// Check Item using
		for ( int i = 0; i < 14; i++ )
		{
			if ( psItemData == &INVENTORYITEMS[INVENTORYITEMSLOT[i].iItemIndex - 1] )
			{
				return FALSE;
			}
		}
	}

	// Is mouse right button click?
	if ( pcMouse->GetEvent() == EMouseEvent::ClickDownR )
	{
		// Warehouse Open?
		if ( (*(BOOL*)(0x036932FC)) )
		{
			// Back to inventory?
			if ( OnWarehouseAutoPlaceItemBack( &sPoint ) )
			{
				return TRUE;
			}

			// Set to warehouse?
			if ( psItemData && OnWarehouseAutoPlaceItem( psItemData ) )
			{
				return TRUE;
			}
		}

		if ( (*(BOOL*)(0x036EFD98)) )
		{
			if ( OnTradeAutoPlaceItemBack( &sPoint ) )
				return TRUE;

			if ( psItemData && OnTradeAutoPlaceItem( psItemData ) )
			{
				return TRUE;
			}
		}

		if ( psItemData && OnBlacksmithAutoPlaceItem( psItemData ) )
			return TRUE;
		if ( psItemData && OnPersonalShopAutoPlaceItem( psItemData ) )
			return TRUE;
	}

	if ( pcMouse->GetEvent() == EMouseEvent::ClickDownL )
	{
		if ( KEYBOARDHANDLER->IsKeyDown( VK_CONTROL ) )
		{
			if ( (*(BOOL*)0x035EBB20) )
			{
				ItemData * psItemData = ITEMHANDLER->GetMouseItemHover();
				ItemData * psItemMouse = ITEMHANDLER->GetMouseItem();
				if ( psItemData && psItemData->sBaseItemID.ToItemBase() == ITEMBASE_Potion && psItemMouse->bValid == FALSE )
				{
					OnUnstackPotion( psItemData, psItemMouse, UNSTACKPOTION_MouseGet );
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

int CItemWindowHandler::CollideItemTrade( RECT * pRect, int iPassItemIndex, int iSlotType )
{
	typedef int( __thiscall *tfnCrashItemTrade )(DWORD dwClass, RECT * pRect, int iPassItemIndex, int iSlotType );
	tfnCrashItemTrade fnCrashItemTrade = (tfnCrashItemTrade)0x0050BDE0;
	return fnCrashItemTrade( 0x036EFD98, pRect, iPassItemIndex, iSlotType );
}

void CItemWindowHandler::SetTradeRect( RECT * psRect )
{
	(*(DWORD*)0x03706F90) = psRect->left;  // X
	(*(DWORD*)0x03706F8C) = psRect->top;   // Y
	(*(DWORD*)0x03706F98) = psRect->right; // EndX
	(*(DWORD*)0x03706F94) = psRect->bottom;// EndY
}

BOOL CItemWindowHandler::IsTradeUse( Item * psItem )
{
	BOOL bResult = FALSE;

	switch ( psItem->sItemID.ToItemID() )
	{
		case ITEMID_ExpUp24H:
		case ITEMID_ExpUp48H:
		case ITEMID_ExpUp72H:
		case ITEMID_ExpUp1H:
		case ITEMID_ExpUp3H:
		case ITEMID_ThirdEye24H:
		case ITEMID_ThirdEye48H:
		case ITEMID_ThirdEye72H:
		case ITEMID_ThirdEye1H:
		case ITEMID_ThirdEye3H:
		case ITEMID_PhoenixEgg:
		case ITEMID_RecipeXMas01:
		case ITEMID_XmasRecipeGift:
		case ITEMID_RedXmasGiftRune:
		case ITEMID_GreenXmasGiftRune:
		case ITEMID_BlueXmasGiftRune:
		case ITEMID_IceRecipe:
		case ITEMID_IceRune:
		case ITEMID_MysteriousBookRecipe:
		case ITEMID_HopyRune:
		case ITEMID_CuepyRune:
		case ITEMID_GoldenHopyRune:
		case ITEMID_GoldenHopyCostumeMale:
		case ITEMID_GoldenHopyCostumeFemale:
		case ITEMID_GoldenHopyCostumeMaleTime:
		case ITEMID_GoldenHopyCostumeFemaleTime:


		//Costumes
		case ITEMID_PartyCostumeMale:
		case ITEMID_PartyCostumeFemale:
		case ITEMID_IdasCostumeMale:
		case ITEMID_IdasCostumeFemale:
		case ITEMID_ThalesArmorMale:
		case ITEMID_ThalesArmorFemale:
		case ITEMID_Costume4ArmorMale:
		case ITEMID_Costume4ArmorFemale:
		case ITEMID_Costume5ArmorMale:
		case ITEMID_Costume5ArmorFemale:
		case ITEMID_WingArmorMale:
		case ITEMID_WingArmorFemale:
		case ITEMID_DarkWingArmorMale:
		case ITEMID_DarkWingArmorFemale:
		case ITEMID_SummerClotheMale:
		case ITEMID_SummerClotheFemale:
		case ITEMID_SpringCostumeMale:
		case ITEMID_SpringCostumeFemale:
		case ITEMID_YellowSpringCostumeMale:
		case ITEMID_YellowSpringCostumeFemale:
		case ITEMID_Costume11ArmorMale:
		case ITEMID_Costume11ArmorFemale:
		case ITEMID_BlackSuit:
		case ITEMID_WhiteDress:
		case ITEMID_CasualSchoolUniformMale:
		case ITEMID_CasualSchoolUniformFemale:
		case ITEMID_ElegantSchoolUniformMale:
		case ITEMID_ElegantSchoolUniformFemale:
		case ITEMID_Costume16ArmorMale:
		case ITEMID_Costume16ArmorFemale:
		case ITEMID_MarineCostumeMale:
		case ITEMID_MarineCostumeFemale:
		case ITEMID_HopyCostume:
		case ITEMID_CuepyCostume:
		case ITEMID_HopyCostumeIce:
		case ITEMID_CuepyCostumeIce:
		case ITEMID_Costume19MaleArmorFemale:
		case ITEMID_ChemistCostumeMale:
		case ITEMID_ChemistCostumeFemale:
		case ITEMID_DraculaDragonCostumeMale:
		case ITEMID_DraculaDragonCostumeFemale:
		case ITEMID_GaiaArmorMale:
		case ITEMID_GaiaArmorFemale:
		case ITEMID_IriaRobeMale:
		case ITEMID_IriaRobeFemale:
		case ITEMID_DarkGaiaArmorMale:
		case ITEMID_DarkGaiaArmorFemale:
		case ITEMID_DarkIriaRobeMale:
		case ITEMID_DarkIriaRobeFemale:
		case ITEMID_WeddingSuit:
		case ITEMID_WeddingDress:
		case ITEMID_Costume23ArmorMale:
		case ITEMID_Costume23ArmorFemale:
		case ITEMID_Costume24ArmorMale:
		case ITEMID_Costume24ArmorFemale:
		case ITEMID_Costume25ArmorMale:
		case ITEMID_Costume25ArmorFemale:
		case ITEMID_MikalRobeMale:
		case ITEMID_MikalRobeFemale:
		case ITEMID_MartanosRobeMale:
		case ITEMID_MartanosRobeFemale:
		case ITEMID_SilverCostumeMale:
		case ITEMID_SilverCostumeFemale:		
		case ITEMID_SoccerShirt:
		case ITEMID_RomanticSuitF:
		case ITEMID_FootballM:
		case ITEMID_FootballF:
		case ITEMID_PeruvianSoccer:
		case ITEMID_BrazilianSoccer:
		case ITEMID_ArgentineSoccer:
		case ITEMID_ChileanSoccer:
		case ITEMID_PinoySoccer:
		case ITEMID_VietnameseSoccer:
		case ITEMID_FrenchSoccer:
		case ITEMID_GermanySoccer:
		case ITEMID_HollandSoccer:
		case ITEMID_ItalianSoccer:
		case ITEMID_MexicanSoccer:
		case ITEMID_PortugueseSoccer:
		case ITEMID_SpanishSoccer:
		case ITEMID_TeamUSASoccer:
		case ITEMID_ColombianSoccer:
		case ITEMID_JapanSoccer:

		case ITEMID_BroyaleCostumeMale:
		case ITEMID_BroyaleCostumeFemale:
		case ITEMID_SandurrRedCostumeMale:
		case ITEMID_SandurrRedCostumeFemale:
		case ITEMID_SandurrPurpleCostumeMale:
		case ITEMID_SandurrPurpleCostumeFemale:
		case ITEMID_ChaoticCostume:
		case ITEMID_ChaoticCostumeFemale:

		//Easter
		case ITEMID_HopyCostumeTime:
		case ITEMID_CuepyCostumeTime:
			bResult = TRUE;
			break;


		case ITEMID_MechanicCostumeM:
		case ITEMID_MechanicCostumeF:
		case ITEMID_BeginnerAxe:
		case ITEMID_BeginnerClaw:
		case ITEMID_BeginnerHammer:
		case ITEMID_BeginnerJavelin:
		case ITEMID_BeginnerScythe:
		case ITEMID_BeginnerBoots:
		case ITEMID_BeginnerGauntlet:
		case ITEMID_BeginnerBracelet:
		case ITEMID_BeginnerBow:
		case ITEMID_BeginnerDagger:
		case ITEMID_BeginnerPhantom:
		case ITEMID_BeginnerStaff:
		case ITEMID_BeginnerSword:
		case ITEMID_GrumpyArmor:
		case ITEMID_GrumpyRobe:

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

		case ITEMID_IceCostumeMale:
		case ITEMID_IceCostumeFemale:

		case ITEMID_CeruleanRing:
			bResult = FALSE;
			break;

		default:
			break;
	}


	if ( (psItem->sItemID.ToInt() & 0xFFFF0000) == 0x080B0000 || (psItem->sItemID.ToInt() & 0xFFFF0000) == 0x080C0000 || (psItem->sItemID.ToInt() & 0xFFFF0000) == 0x08110000 || (psItem->sItemID.ToInt() & 0xFFFF0000) == 0x03070000 )
		bResult = TRUE;

	if ( (psItem->sItemID.ToInt() == ITEMID_ArgonianGauntlets) || (psItem->sItemID.ToInt() == ITEMID_GhostCastleBracelets) || (psItem->sItemID.ToInt() == ITEMID_GreedyGauntlets) )
		bResult = TRUE;

	if ( psItem->bLockedItemType )
		bResult = FALSE;

	return bResult;
}

BOOL CItemWindowHandler::IsWarehouseUse( ItemData * psItemData )
{
	BOOL bRet = TRUE;

	switch ( psItemData->sItem.sItemID.ToItemID() )
	{
		case ITEMID_BeginnerAxe:
		case ITEMID_BeginnerClaw:
		case ITEMID_BeginnerHammer:
		case ITEMID_BeginnerJavelin:
		case ITEMID_BeginnerScythe:
		case ITEMID_BeginnerBoots:
		case ITEMID_BeginnerGauntlet:
		case ITEMID_BeginnerBracelet:
		case ITEMID_BeginnerBow:
		case ITEMID_BeginnerDagger:
		case ITEMID_BeginnerPhantom:
		case ITEMID_BeginnerStaff:
		case ITEMID_BeginnerSword:
		case ITEMID_GrumpyArmor:
		case ITEMID_GrumpyRobe:
			bRet = FALSE;
			break;
		
		default:
			break;
	}

	return bRet;
}

BOOL CItemWindowHandler::IsPersonalShopUse( ItemData * psItemData )
{
	BOOL bRet = TRUE;

	switch ( psItemData->sBaseItemID.ToItemID() )
	{
		case ITEMID_MechanicCostumeM:
		case ITEMID_MechanicCostumeF:

		case ITEMID_BeginnerAxe:
		case ITEMID_BeginnerClaw:
		case ITEMID_BeginnerHammer:
		case ITEMID_BeginnerJavelin:
		case ITEMID_BeginnerScythe:
		case ITEMID_BeginnerBoots:
		case ITEMID_BeginnerGauntlet:
		case ITEMID_BeginnerBracelet:
		case ITEMID_BeginnerBow:
		case ITEMID_BeginnerDagger:
		case ITEMID_BeginnerPhantom:
		case ITEMID_BeginnerStaff:
		case ITEMID_BeginnerSword:
		case ITEMID_GrumpyArmor:
		case ITEMID_GrumpyRobe:
			bRet = FALSE;
			break;

		default:
			break;
	}

	if ( psItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Quest )
		bRet = FALSE;

	return bRet;
}

BOOL CItemWindowHandler::SetItemToNPC( ItemData * psItemData, BOOL bFlag )
{
	if ( bFlag == FALSE )
	{
		// Check Valid items at slots
		CALL( 0x0050DC50 );

		ItemData * psaMixItemData = (ItemData*)0x03680358;

		// Is Sheltom?
		if ( ITEMWINDOWHANDLER->IsUseMixNPC( &psItemData->sItem ) && (psItemData->sBaseItemID.ToItemID() != ITEMID_FurySheltom) &&
			(psItemData->sBaseItemID.ToItemID() != ITEMID_DarkGuardianSheltom) && (psItemData->sBaseItemID.ToItemID() != ITEMID_ThroneSheltom) )
		{
			// Get Sheltom Slots
			for ( int i = 1; i < 15; i++ )
			{
				ItemData * ps = psaMixItemData + i;
				// Free Slot?
				if ( !ps->bValid )
				{
					// Copy item to slot
					CopyMemory( ps, psItemData, sizeof( ItemData ) );
					ps->sPosition.iX = psItemData->sPlacePosition.iX;
					ps->sPosition.iY = psItemData->sPlacePosition.iY;
					psItemData->bValid = FALSE;
					PLAYMINISOUND( ps->iInvItemSound );

					// Validate Item
					CALLT( 0x0050DC60, 0x03680334 );
					CALL( 0x0050CE80 );

					return TRUE;
				}
			}
		}
		// Respec Stone?
		else if ( psItemData->sBaseItemID.ToItemType() == ITEMTYPE_Respec )
		{
			// Valid?
			if ( psaMixItemData[0].bValid )
			{
				struct SReconstructItem
				{
					int		iItemID;    
					int		iLevelMin; 
					int		iLevelMax;
					int		iCount;
					int		iPrice;
				};

				if ( psaMixItemData[0].sItem.bCostumeItem )
				{
					TITLEBOX( AY_OBFUSCATE( "You can't respec Skin Item!" ) );
					return FALSE;
				}

				SReconstructItem * psRec = (SReconstructItem*)(0x0099A0A8);

				// Get Level to use check
				int iTempLevel = psaMixItemData[0].sItem.iLevel;
				for ( int i = 0; i < 12; i++ )
				{
					if ( psItemData->sBaseItemID.ToInt() == psRec[i].iItemID )
					{
						if ( iTempLevel < psRec[i].iLevelMin || iTempLevel > psRec[i].iLevelMax )
						{
							TITLEBOX( "You cannot use this respec item" );
							return FALSE;
						}
					}
				}
			}
			for ( int i = 0; i < 15; i++ )
			{
				ItemData * ps = psaMixItemData + i;
				// Free Slot?
				if ( !ps->bValid )
				{
					// Copy item to slot
					CopyMemory( ps, psItemData, sizeof( ItemData ) );
					ps->sPosition.iX = psItemData->sPlacePosition.iX;
					ps->sPosition.iY = psItemData->sPlacePosition.iY;
					psItemData->bValid = FALSE;
					PLAYMINISOUND( ps->iInvItemSound );

					// Validate Item
					CALLT( 0x0050DC60, 0x03680334 );
					return TRUE;
				}
			}
		}
		else
		{
			ItemData * ps = psaMixItemData;
			// Free Slot?
			if ( !ps->bValid && ITEMWINDOWHANDLER->IsUseMixNPCItem( &psItemData->sItem ) )
			{
				// Copy item to slot
				CopyMemory( ps, psItemData, sizeof( ItemData ) );
				ps->sPosition.iX = psItemData->sPlacePosition.iX;
				ps->sPosition.iY = psItemData->sPlacePosition.iY;
				psItemData->bValid = FALSE;
				PLAYMINISOUND( ps->iInvItemSound );

				// Validate Item
				CALLT( 0x0050DC60, 0x03680334 );
				return TRUE;
			}
		}
	}

	if ( bFlag == TRUE )
	{
		ItemData * psaAgingItemData = (ItemData*)0x03686C08;

		// Is Sheltom?
		if ( psItemData->sBaseItemID.ToItemType() == ITEMTYPE_Sheltom )
		{
			// Get Sheltom Slots
			for ( int i = 1; i < 15; i++ )
			{
				ItemData * ps = psaAgingItemData + i;
				// Free Slot?
				if ( !ps->bValid )
				{
					// Copy item to slot
					CopyMemory( ps, psItemData, sizeof( ItemData ) );
					ps->sPosition.iX = psItemData->sPlacePosition.iX;
					ps->sPosition.iY = psItemData->sPlacePosition.iY;
					psItemData->bValid = FALSE;
					PLAYMINISOUND( ps->iInvItemSound );

					// Validate Item
					//CALLT( 0x0050DC60, 0x03680334 );

					return TRUE;
				}
			}
		}
		else
		{
			ItemData * ps = psaAgingItemData;
			// Free Slot?
			if ( !ps->bValid && IsUseAgingNPC( psItemData ) )
			{
				// Copy item to slot
				CopyMemory( ps, psItemData, sizeof( ItemData ) );
				ps->sPosition.iX = psItemData->sPlacePosition.iX;
				ps->sPosition.iY = psItemData->sPlacePosition.iY;
				psItemData->bValid = FALSE;
				PLAYMINISOUND( ps->iInvItemSound );

				// Validate Item
				//CALLT( 0x0050DC60, 0x03680334 );

				return TRUE;
			}
		}
	}
	if ( bFlag == 2 )
	{
		ItemData * psaAgingItemData = (ItemData*)0x03686C08;

		// Get Sheltom Slots
		for ( int i = 0; i < 15; i++ )
		{
			ItemData * ps = psaAgingItemData + i;
			// Free Slot?
			if ( !ps->bValid )
			{
				// Copy item to slot
				CopyMemory( ps, psItemData, sizeof( ItemData ) );
				ps->sPosition.iX = psItemData->sPlacePosition.iX;
				ps->sPosition.iY = psItemData->sPlacePosition.iY;
				psItemData->bValid = FALSE;
				PLAYMINISOUND( ps->iInvItemSound );

				// Validate Item
				//CALLT( 0x0050DC60, 0x03680334 );

				return TRUE;
			}
		}
	}
	if ( bFlag == 3 )
	{
		ItemData * psaAgingItemData = (ItemData*)0x03686C08;

		ItemData * ps = psaAgingItemData + 14;

		// Free Slot?
		if ( !ps->bValid )
		{
			// Copy item to slot
			CopyMemory( ps, psItemData, sizeof( ItemData ) );
			ps->sPosition.iX = psItemData->sPlacePosition.iX;
			ps->sPosition.iY = psItemData->sPlacePosition.iY;
			psItemData->bValid = FALSE;
			PLAYMINISOUND( ps->iInvItemSound );

			return TRUE;
		}
	}
	return TRUE;
}

int CItemWindowHandler::GetGoldPersonalShopLimit( int iGold )
{
	int iMax = 0;

	if ( UNITDATA->sCharacterData.iLevel <= 10 )
		iMax = 200000;

	if ( iMax == 0 )
	{
		BOOL bGoldBag = ITEMHANDLER->GetItemInventoryByCode( ITEMID_GoldBag ) ? TRUE : FALSE;

		switch ( UNITDATA->sCharacterData.iRank )
		{
			case CHARACTERRANK_Rank1:
				iMax = (UNITDATA->sCharacterData.iLevel * 200000) - 1800000;
				break;
			case CHARACTERRANK_Rank2:
				iMax = 10000000;		//10kk
				break;
			case CHARACTERRANK_Rank3:
				iMax = 50000000;		//50kk
				break;
			case CHARACTERRANK_Rank4:
				iMax = 500000000;		//500kk
				break;
			case CHARACTERRANK_Rank5:
				iMax = 750000000;		//750kk
				if ( bGoldBag )
					iMax = 1000000000; //1kkk
				break;
		}
	}

	iMax -= (iGold + UNITDATA->sCharacterData.iGold);

	if ( iMax < 0 )
		iMax = 0;

	return iMax;
}

BOOL CItemWindowHandler::PlaceItemCleanNPC( ItemData * pcItemData, int iFlag )
{
	ItemData * pcaItemData	= ((ItemData *)0x036EF600);
	ItemData * pcStoneSlot	= &pcaItemData[1];
	ItemData * pcItemSlot	= &pcaItemData[0];

	//Stone
	if ( iFlag == 1 )
	{
		if ( pcItemSlot->bValid && (pcStoneSlot->bValid == FALSE) )
		{
			CopyMemory( pcStoneSlot, pcItemData, sizeof( ItemData ) );

			pcStoneSlot->sPosition = pcItemData->sPlacePosition;
			pcItemData->bValid = FALSE;

			PLAYMINISOUND( pcStoneSlot->iInvItemSound );
		}
	}
	//Item
	else if ( iFlag == 2 )
	{
		if ( pcItemData->sItem.bCostumeItem )
			return TRUE;

		if ( pcItemSlot->bValid == FALSE )
		{
			CopyMemory( pcItemSlot, pcItemData, sizeof( ItemData ) );

			pcItemSlot->sPosition = pcItemData->sPlacePosition;
			pcItemData->bValid = FALSE;

			PLAYMINISOUND( pcItemSlot->iInvItemSound );
		}
	}
	return TRUE;
}

