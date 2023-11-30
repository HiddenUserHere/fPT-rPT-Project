#include "stdafx.h"
#include "CCostumeWindowHandler.h"

CCostumeWindowHandler::CCostumeWindowHandler()
{
	pWindow = std::make_shared<UI::Window>( Rectangle2D( 575, 38, 215, 107 ) );

	ZeroMemory( &cWeapon, sizeof( ItemData ) );
	ZeroMemory( &cShield, sizeof( ItemData ) );
}

CCostumeWindowHandler::~CCostumeWindowHandler()
{
}

void CCostumeWindowHandler::Init()
{
	UI::Window_ptr pWindowWeapon = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 66, 106 ) );
	pWindowWeapon->SetID( WINDOW_Weapon );
	{
		UI::ItemBox_ptr pItemBoxWeapon = std::make_shared<UI::ItemBox>( 0, 9, 3, 4 );
		pItemBoxWeapon->SetID( ITEMBOX_Weapon );
		pItemBoxWeapon->SetMaxItems( 1 );
		pItemBoxWeapon->SetEventOnPutItem( UI::Event::Build( std::bind( &CCostumeWindowHandler::OnPutItemWeapon, this, std::placeholders::_1 ) ) );
		pItemBoxWeapon->SetEventOnGetItem( UI::Event::Build( std::bind( &CCostumeWindowHandler::OnGetItemWeapon, this, std::placeholders::_1 ) ) );
		pItemBoxWeapon->SetEventBeforePutItem( UI::Event::Build( std::bind( &CCostumeWindowHandler::OnBeforePutItemWeapon, this, std::placeholders::_1 ) ) );
		pItemBoxWeapon->SetEventOnExpireItem( UI::Event::Build( std::bind( &CCostumeWindowHandler::OnExpireWeapon, this, std::placeholders::_1 ) ) );
		pItemBoxWeapon->SetEnableRightClick( FALSE );
		pItemBoxWeapon->SetSwapItemSlot( TRUE );
		pWindowWeapon->AddElement( pItemBoxWeapon );

		UI::Window_ptr pWindowTwoHand = std::make_shared<UI::Window>( Rectangle2D( 148, 0, 66, 106 ) );
		pWindowTwoHand->SetID( WINDOW_WeaponTwoHand );
		pWindowTwoHand->SetMouseClickEnable( FALSE );
		pWindowTwoHand->AddElement( pItemBoxWeapon );
		pWindowTwoHand->Hide();
		pWindow->AddElement( pWindowTwoHand );
	}
	pWindow->AddElement( pWindowWeapon );

	UI::Window_ptr pWindowShield = std::make_shared<UI::Window>( Rectangle2D( 148, 0, 66, 106 ) );
	pWindowShield->SetID( WINDOW_Shield );
	{
		UI::ItemBox_ptr pItemBoxShield = std::make_shared<UI::ItemBox>( 0, 9, 3, 4 );
		pItemBoxShield->SetID( ITEMBOX_Shield );
		pItemBoxShield->SetMaxItems( 1 );
		pItemBoxShield->SetEventOnPutItem( UI::Event::Build( std::bind( &CCostumeWindowHandler::OnPutItemShield, this, std::placeholders::_1 ) ) );
		pItemBoxShield->SetEventOnGetItem( UI::Event::Build( std::bind( &CCostumeWindowHandler::OnGetItemShield, this, std::placeholders::_1 ) ) );
		pItemBoxShield->SetEventBeforePutItem( UI::Event::Build( std::bind( &CCostumeWindowHandler::OnBeforePutItemShield, this, std::placeholders::_1 ) ) );
		pItemBoxShield->SetEventOnExpireItem( UI::Event::Build( std::bind( &CCostumeWindowHandler::OnExpireShield, this, std::placeholders::_1 ) ) );
		pItemBoxShield->SetEnableRightClick( FALSE );
		pItemBoxShield->SetSwapItemSlot( TRUE );
		pWindowShield->AddElement( pItemBoxShield );
	}
	pWindow->AddElement( pWindowShield );

	pWindow->Hide();
}

void CCostumeWindowHandler::Update( float fTime )
{
	pWindow->Update( fTime );
}

ItemData * CCostumeWindowHandler::GetWeaponItem()
{
	if ( GetWeaponItemBox()->GetItems().size() > 0 )
		return GetWeaponItemBox()->GetItems()[0];

	return nullptr;
}

ItemData * CCostumeWindowHandler::GetShieldItem()
{
	if ( GetShieldItemBox()->GetItems().size() > 0 )
		return GetShieldItemBox()->GetItems()[0];

	return nullptr;
}

BOOL CCostumeWindowHandler::IsBlockedMouse()
{
	BOOL b = bBlockMouse;

	bBlockMouse = FALSE;

	return b;
}

bool CCostumeWindowHandler::SetWeaponCostume( ItemData * pcItemData )
{
	if ( bLoadItem == false )
	{
		CopyMemory( &cWeapon, pcItemData, sizeof( ItemData ) );

		return true;
	}

	BOOL bSuccess = FALSE;

	ItemData cItemData;
	if ( pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_GoldAndExp )
	{
		CopyMemory( &cItemData, pcItemData, sizeof( ItemData ) );
		bSuccess = CGameInventory::GetInstance()->LoadItemImageEXPGold( &cItemData );
	}
	else
		bSuccess = ITEMHANDLER->LoadItemImage( &pcItemData->sItem, &cItemData );

	if ( bSuccess )
		return GetWeaponItemBox()->AddItem( &cItemData ) ? true : false;

	return false;
}

bool CCostumeWindowHandler::SetShieldCostume( ItemData * pcItemData )
{
	if ( bLoadItem == false )
	{
		CopyMemory( &cShield, pcItemData, sizeof( ItemData ) );

		return true;
	}

	BOOL bSuccess = FALSE;

	ItemData cItemData;
	if ( pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_GoldAndExp )
	{
		CopyMemory( &cItemData, pcItemData, sizeof( ItemData ) );
		bSuccess = CGameInventory::GetInstance()->LoadItemImageEXPGold( &cItemData );
	}
	else
		bSuccess = ITEMHANDLER->LoadItemImage( &pcItemData->sItem, &cItemData );

	if ( bSuccess )
		return GetShieldItemBox()->AddItem( &cItemData ) ? true : false;

	return false;
}

void CCostumeWindowHandler::LoadCacheItems()
{
	bLoadItem = true;

	if ( cWeapon.bValid )
		SetWeaponCostume( &cWeapon );

	if ( cShield.bValid )
		SetShieldCostume( &cShield );

	ITEMHANDLER->SetCharacterItem( ITEMID_None, ITEMSLOTFLAG_CostumeRight, FALSE );
	ITEMHANDLER->SetCharacterItem( ITEMID_None, ITEMSLOTFLAG_CostumeLeft, FALSE );

	SwapInventory( TRUE );

	pWindow->Hide();
}

void CCostumeWindowHandler::SwapInventory( BOOL bOpen, bool bFirstLoad )
{
	if ( bBlockSwap )
		return;

	if ( bOpen )
	{
		HideShield();

		HideWeapon();

		if ( auto pcItemData = GetWeaponItem() )
		{
			if ( INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex )
			{
				if ( auto pcItemRightHandData = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex - 1] )
				{
					if ( pcItemRightHandData->bValid &&
						(pcItemData->sItem.sItemID.ToItemType() == pcItemRightHandData->sItem.sItemID.ToItemType()) &&
						(pcItemData->IsTwoHandedWeapon() == pcItemRightHandData->IsTwoHandedWeapon()) )
						ITEMHANDLER->SetCharacterItem( pcItemData->sItem.sItemID.ToItemID(), ITEMSLOTFLAG_CostumeRight, TRUE );
				}
			}
		}
		
		if ( auto pcItemData = GetShieldItem() )
		{
			if ( INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex )
			{
				if ( auto pcItemLeftHandData = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex - 1] )
				{
					if ( pcItemLeftHandData->bValid &&
						(pcItemData->sItem.sItemID.ToItemType() == pcItemLeftHandData->sItem.sItemID.ToItemType()) )
						ITEMHANDLER->SetCharacterItem( pcItemData->sItem.sItemID.ToItemID(), ITEMSLOTFLAG_CostumeLeft, TRUE );
				}
			}
		}

		pWindow->Show();
	}
	else
	{
		HideShield();

		HideWeapon();

		pWindow->Hide();
	}
}

bool CCostumeWindowHandler::IsValidWeaponItem()
{
	bool bRet = false;

	if ( auto pcItemData = GetWeaponItem() )
	{
		if ( INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex )
		{
			if ( auto pcItemRightHandData = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex - 1] )
			{
				if ( pcItemRightHandData->bValid &&
					(pcItemData->sItem.sItemID.ToItemType() == pcItemRightHandData->sItem.sItemID.ToItemType()) &&
					(pcItemData->IsTwoHandedWeapon() == pcItemRightHandData->IsTwoHandedWeapon()) )
					bRet = true;
			}
		}
	}

	return bRet;
}

bool CCostumeWindowHandler::IsValidShieldItem()
{
	bool bRet = false;

	if ( auto pcItemData = GetShieldItem() )
	{
		if ( INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex )
		{
			if ( auto pcItemLeftHandData = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex - 1] )
			{
				if ( pcItemLeftHandData->bValid &&
					(pcItemData->sItem.sItemID.ToItemType() == pcItemLeftHandData->sItem.sItemID.ToItemType()) )
					bRet = true;
			}
		}
	}

	return bRet;
}

bool CCostumeWindowHandler::IsSpecialCostume( EItemID eItemID )
{
	bool bRet = false;

	switch ( eItemID )
	{
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
			bRet = true;
			break;
	}

	return bRet;
}

UI::ItemBox_ptr CCostumeWindowHandler::GetWeaponItemBox()
{
	return pWindow->GetElement<UI::Window>( WINDOW_Weapon )->GetElement<UI::ItemBox>( ITEMBOX_Weapon );
}

UI::ItemBox_ptr CCostumeWindowHandler::GetShieldItemBox()
{
	return pWindow->GetElement<UI::Window>( WINDOW_Shield )->GetElement<UI::ItemBox>( ITEMBOX_Shield );
}

void CCostumeWindowHandler::OnGetItemWeapon( UIEventArgs eArgs )
{
	HideWeapon();

	bBlockMouse = TRUE;
}

void CCostumeWindowHandler::OnBeforePutItemWeapon( UIEventArgs eArgs )
{
	if ( auto pcItemData = GetWeaponItemBox()->GetEventItemData() )
	{
		if ( pcItemData->bValid )
		{
			EItemID eItemID = pcItemData->sItem.sItemID.ToItemID();

			bool bAllowedItem = pcItemData->sItem.bCanNotUse ? false : true;
			if ( bAllowedItem )
			{
				bAllowedItem = false;

				switch ( eItemID )
				{
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

						bAllowedItem = true;
						break;
				}

				if ( pcItemData->sItem.sItemID.ToItemBase() == ITEMBASE_Weapon )
					if ( pcItemData->sItem.bCostumeItem )
						bAllowedItem = true;
			}

			if ( bAllowedItem == false )
			{
				
			}

			//Check for Shields
			if ( bAllowedItem )
			{
				if ( auto pcItemShieldData = GetShieldItem() )
				{
					if ( pcItemShieldData->bValid )
					{
						if ( pcItemData->IsTwoHandedWeapon() )
							bAllowedItem = false;
					}
				}
			}

			//Check if are using the same Item Type
			if ( bAllowedItem )
			{
				bAllowedItem = false;

				BOOL bIsTwoHanded = pcItemData->IsTwoHandedWeapon();

				GetWeaponItemBox()->AddItem( pcItemData );

				if ( INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex )
				{
					if ( auto pcItemRightHandData = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex - 1] )
					{
						bAllowedItem = (pcItemRightHandData->bValid && 
									   (ItemID( eItemID ).ToItemType() == pcItemRightHandData->sItem.sItemID.ToItemType()) &&
									   (bIsTwoHanded == pcItemRightHandData->IsTwoHandedWeapon()));
					}
				}

				if ( !bAllowedItem )
					HideWeapon();
			}
		}
	}

	bBlockMouse = TRUE;
}

void CCostumeWindowHandler::OnPutItemWeapon( UIEventArgs eArgs )
{
	ITEMHANDLER->SetCharacterItem( GetWeaponItem()->sItem.sItemID.ToItemID(), ITEMSLOTFLAG_CostumeRight, TRUE );

	UpdateTwoHandSlot( GetWeaponItem()->IsTwoHandedWeapon() );

	bBlockMouse = TRUE;
}

void CCostumeWindowHandler::OnGetItemShield( UIEventArgs eArgs )
{
	HideShield();

	bBlockMouse = TRUE;
}

void CCostumeWindowHandler::OnBeforePutItemShield( UIEventArgs eArgs )
{
	if ( auto pcItemData = GetShieldItemBox()->GetEventItemData() )
	{
		if ( pcItemData->bValid )
		{
			EItemID eItemID = pcItemData->sItem.sItemID.ToItemID();

			bool bAllowedItem = pcItemData->sItem.bCanNotUse ? false : true;
			if ( bAllowedItem )
			{
				bAllowedItem = false;

				switch ( eItemID )
				{
					case ITEMID_XmasCostumeShield:
					case ITEMID_HalloweenShield:
						bAllowedItem = true;
						break;
				}
			}

			if ( bAllowedItem == false )
			{
				if ( pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Shield )
					if ( pcItemData->sItem.bCostumeItem )
						bAllowedItem = true;
			}

			//Check for Two Hand Weapons
			if ( bAllowedItem )
			{
				if ( auto pcItemWeaponData = GetWeaponItem() )
				{
					if ( pcItemWeaponData->bValid )
					{
						if ( pcItemWeaponData->IsTwoHandedWeapon() )
							bAllowedItem = false;
					}
				}
			}

			//Check if are using the same Item Type
			if ( bAllowedItem )
			{
				bAllowedItem = false;

				ItemData cItemDataTemp;
				CopyMemory( &cItemDataTemp, pcItemData, sizeof( ItemData ) );

				GetShieldItemBox()->AddItem( pcItemData );

				if ( INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex )
				{
					if ( auto pcItemLeftHandData = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex - 1] )
					{
						bAllowedItem = (pcItemLeftHandData->bValid &&
							(ItemID( eItemID ).ToItemType() == pcItemLeftHandData->sItem.sItemID.ToItemType()));
					}
				}

				if ( !bAllowedItem )
					HideShield();
			}
		}
	}

	bBlockMouse = TRUE;
}

void CCostumeWindowHandler::OnPutItemShield( UIEventArgs eArgs )
{
	ITEMHANDLER->SetCharacterItem( GetShieldItem()->sItem.sItemID.ToItemID(), ITEMSLOTFLAG_CostumeLeft, TRUE );

	UpdateTwoHandSlot( FALSE );

	bBlockMouse = TRUE;
}

void CCostumeWindowHandler::OnExpireWeapon( UIEventArgs eArgs )
{
	if ( auto pcItemData = GetWeaponItemBox()->GetEventItemData() )
		HideWeapon();
}

void CCostumeWindowHandler::OnExpireShield( UIEventArgs eArgs )
{
	if ( auto pcItemData = GetShieldItemBox()->GetEventItemData() )
		HideShield();
}

void CCostumeWindowHandler::HideWeapon()
{
    if ( auto pcItemData = GetWeaponItem() )
    {
        if ( pcItemData->iVisibleItemType == ITEMSLOTFLAG_LeftHand )
            HideShield();
    }

	if ( INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex )
	{
		auto pcItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex - 1];
		if ( pcItemData->bValid )
			ITEMHANDLER->SetCharacterItem( pcItemData->sItem.sItemID.ToItemID(), ITEMSLOTFLAG_RightHand, TRUE );
		else
			ITEMHANDLER->SetCharacterItem( ITEMID_None, ITEMSLOTFLAG_RightHand, FALSE );
	}
	else
		ITEMHANDLER->SetCharacterItem( ITEMID_None, ITEMSLOTFLAG_RightHand, FALSE );
}

void CCostumeWindowHandler::HideShield()
{
	if ( INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex )
	{
		auto pcItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex - 1];
		if ( pcItemData->bValid && (pcItemData->IsTwoHandedWeapon() == FALSE) && pcItemData->sItem.iChk2 )
			ITEMHANDLER->SetCharacterItem( pcItemData->sItem.sItemID.ToItemID(), ITEMSLOTFLAG_LeftHand, TRUE );
		else
		{
			if ( IsDoubleWeapon( pcItemData ) )
				ITEMHANDLER->SetCharacterItem( pcItemData->sItem.sItemID.ToItemID(), ITEMSLOTFLAG_LeftHand, TRUE );
			else
				ITEMHANDLER->SetCharacterItem( ITEMID_None, ITEMSLOTFLAG_LeftHand, FALSE );
		}
	}
	else
		ITEMHANDLER->SetCharacterItem( ITEMID_None, ITEMSLOTFLAG_LeftHand, FALSE );
}

void CCostumeWindowHandler::UpdateTwoHandSlot( BOOL bShow )
{
	if ( bShow )
		pWindow->GetElement<UI::Window>( WINDOW_WeaponTwoHand )->Show();
	else
		pWindow->GetElement<UI::Window>( WINDOW_WeaponTwoHand )->Hide();
}

bool CCostumeWindowHandler::IsDoubleWeapon( ItemData * pcItemData )
{
	bool bRet = false;

	if ( pcItemData->bValid && pcItemData->IsTwoHandedWeapon() )
	{
		if ( pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Dagger )
			bRet = true;
	}

	return bRet;
}
