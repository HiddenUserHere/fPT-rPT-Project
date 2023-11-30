#include "stdafx.h"
#include "CInventoryWindowHandler.h"

BOOL ITEM_EARRING_GETTER = FALSE;

CInventoryWindowHandler::CInventoryWindowHandler()
{
	pWindow = std::make_shared<UI::Window>( Rectangle2D( 0, 400, 800, 200 ) );

	pcCostumeWindowHandler = new CCostumeWindowHandler();
}

CInventoryWindowHandler::~CInventoryWindowHandler()
{
	DELET( pcCostumeWindowHandler );

	pWindow = nullptr;
}

void CInventoryWindowHandler::Init()
{
	//Buttons
	{
		UI::Button_ptr pButtonLockItems = std::make_shared<UI::Button>( Rectangle2D( 542, 136, 18, 18 ) );
		pButtonLockItems->SetID( BUTTON_LockItems );
		pButtonLockItems->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\hud\\btnlock.png", "game\\images\\UI\\hud\\btnlock_.png" ) );
		pButtonLockItems->SetSelect( TRUE );
		pWindow->AddElement( pButtonLockItems );
	}

	//Earrings
	{
		UI::ImageBox_ptr pEarringBackground1 = std::make_shared<UI::ImageBox>( Rectangle2D( 509, 91, 22, 22 ) );
		pEarringBackground1->SetID( IMAGE_EarringBackground1 );
		pEarringBackground1->SetImage( UI::ImageLoader::LoadImage( "image\\Sinimage\\Inven\\CoverSmall.bmp" ) );
		pEarringBackground1->Hide();
		pWindow->AddElement( pEarringBackground1 );

		UI::ImageBox_ptr pEarringBackground2 = std::make_shared<UI::ImageBox>( Rectangle2D( 535, 91, 22, 22 ) );
		pEarringBackground2->SetID( IMAGE_EarringBackground2 );
		pEarringBackground2->SetImage( UI::ImageLoader::LoadImage( "image\\Sinimage\\Inven\\CoverSmall.bmp" ) );
		pEarringBackground2->Hide();
		pWindow->AddElement( pEarringBackground2 );

		UI::ItemBox_ptr pEarring1 = std::make_shared<UI::ItemBox>( 509, 91, 1, 1 );
		pEarring1->SetID( ITEMBOX_Earring1 );
		pEarring1->SetMaxItems( 1 );
		pEarring1->SetEnableRightClick( FALSE );
		pEarring1->SetSwapItemSlot( TRUE );
		pEarring1->SetEventOnGetItem( UI::Event::Build( std::bind( &CInventoryWindowHandler::OnGetItemEarring, this, ITEMBOX_Earring1, std::placeholders::_1 ) ) );
		pEarring1->SetEventOnPutItem( UI::Event::Build( std::bind( &CInventoryWindowHandler::OnPutItemEarring, this, ITEMBOX_Earring1, std::placeholders::_1 ) ) );
		pEarring1->SetEventBeforePutItem( UI::Event::Build( std::bind( &CInventoryWindowHandler::OnBeforePutEarring, this, ITEMBOX_Earring1, std::placeholders::_1 ) ) );
		pEarring1->AddRule( UI::ItemBox::ERule::RULE_Allow, UI::ItemBox::ETypeRule::TYPERULE_ItemType, ITEMTYPE_Earring );
		pWindow->AddElement( pEarring1 );

		UI::ItemBox_ptr pEarring2 = std::make_shared<UI::ItemBox>( 535, 91, 1, 1 );
		pEarring2->SetID( ITEMBOX_Earring2 );
		pEarring2->SetMaxItems( 1 );
		pEarring2->SetEnableRightClick( FALSE );
		pEarring2->SetSwapItemSlot( TRUE );
		pEarring2->SetEventOnGetItem( UI::Event::Build( std::bind( &CInventoryWindowHandler::OnGetItemEarring, this, ITEMBOX_Earring2, std::placeholders::_1 ) ) );
		pEarring2->SetEventOnPutItem( UI::Event::Build( std::bind( &CInventoryWindowHandler::OnPutItemEarring, this, ITEMBOX_Earring2, std::placeholders::_1 ) ) );
		pEarring2->SetEventBeforePutItem( UI::Event::Build( std::bind( &CInventoryWindowHandler::OnBeforePutEarring, this, ITEMBOX_Earring2, std::placeholders::_1 ) ) );
		pEarring2->AddRule( UI::ItemBox::ERule::RULE_Allow, UI::ItemBox::ETypeRule::TYPERULE_ItemType, ITEMTYPE_Earring );
		pWindow->AddElement( pEarring2 );
	}

	//Images
	{
		UI::ImageBox_ptr pImageBallonLockItems = std::make_shared<UI::ImageBox>( Rectangle2D( 540, 108, 77, 27 ) );
		pImageBallonLockItems->SetID( IMAGE_LockItemView );
		pImageBallonLockItems->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\LockViewing.png" ) );
		pImageBallonLockItems->Hide();
		pWindow->AddElement( pImageBallonLockItems );
	}

	pcCostumeWindowHandler->Init();
	pWindow->AddElement( pcCostumeWindowHandler->GetWindow() );

	pWindow->Hide();
}

void CInventoryWindowHandler::Render()
{
	if ( (*(BOOL *)0x035EBB20) )
	{
		if ( !pWindow->IsOpen() )
			pWindow->Show();

		pWindow->SetPosition( 0, (DX::Graphic::GetHeightBackBuffer() + 56) - WINDOW_YINVENTORYMOVE );
		pWindow->Render( 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );

		if ( pWindow->GetElement<UI::Button>( BUTTON_LockItems )->IsHover() )
			pWindow->GetElement<UI::ImageBox>( IMAGE_LockItemView )->Show();
		else
			pWindow->GetElement<UI::ImageBox>( IMAGE_LockItemView )->Hide();
	}
	else
		pWindow->Hide();
}

void CInventoryWindowHandler::Update( float fTime )
{
	pcCostumeWindowHandler->Update( fTime );
}

BOOL CInventoryWindowHandler::OnMouseClick( CMouse * pcMouse )
{
	if ( (*(BOOL *)0x035EBB20) && pWindow->IsOpen() )
		pWindow->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );

	if ( ITEM_EARRING_GETTER )
	{
		ITEM_EARRING_GETTER = FALSE;
		return TRUE;
	}

	return pcCostumeWindowHandler->IsBlockedMouse();
}

void CInventoryWindowHandler::OnMouseMove( CMouse * pcMouse )
{
	if ( (*(BOOL *)0x035EBB20) && pWindow->IsOpen() )
		pWindow->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

void CInventoryWindowHandler::OnMouseScroll( CMouse * pcMouse )
{
}

BOOL CInventoryWindowHandler::IsLockItems()
{
	return pWindow->GetElement<UI::Button>( BUTTON_LockItems )->IsSelected();
}

ItemData * CInventoryWindowHandler::GetEarring1()
{
	if ( auto pItemBox1 = pWindow->GetElement<UI::ItemBox>( ITEMBOX_Earring1 ) )
		if ( pItemBox1->GetItems().size() == 1 )
			return pItemBox1->GetItems()[0];

	return nullptr;
}

ItemData * CInventoryWindowHandler::GetEarring2()
{
	if ( auto pItemBox2 = pWindow->GetElement<UI::ItemBox>( ITEMBOX_Earring2 ) )
		if ( pItemBox2->GetItems().size() == 1 )
			return pItemBox2->GetItems()[0];

	return nullptr;
}

bool CInventoryWindowHandler::SetEarring1( ItemData * pcItemData )
{
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
		return pWindow->GetElement<UI::ItemBox>( ITEMBOX_Earring1 )->AddItem( &cItemData ) ? true : false;

	return false;
}

bool CInventoryWindowHandler::SetEarring2( ItemData * pcItemData )
{
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
		return pWindow->GetElement<UI::ItemBox>( ITEMBOX_Earring2 )->AddItem( &cItemData ) ? true : false;

	return false;
}

void CInventoryWindowHandler::SetEarringsEnabled( bool b )
{
	if ( b )
	{
		pWindow->GetElement<UI::ItemBox>( ITEMBOX_Earring1 )->EnableMouse();
		pWindow->GetElement<UI::ItemBox>( ITEMBOX_Earring2 )->EnableMouse();
	}
	else
	{
		pWindow->GetElement<UI::ItemBox>( ITEMBOX_Earring1 )->DisableMouse();
		pWindow->GetElement<UI::ItemBox>( ITEMBOX_Earring2 )->DisableMouse();
	}
}

void CInventoryWindowHandler::OnGetItemEarring( int iItemBoxID, UIEventArgs eArgs )
{
	pWindow->GetElement<UI::ImageBox>( iItemBoxID == ITEMBOX_Earring1 ? IMAGE_EarringBackground1 : IMAGE_EarringBackground2 )->Hide();

	ITEM_EARRING_GETTER = TRUE;
}

void CInventoryWindowHandler::OnPutItemEarring( int iItemBoxID, UIEventArgs eArgs )
{
	pWindow->GetElement<UI::ImageBox>( iItemBoxID == ITEMBOX_Earring1 ? IMAGE_EarringBackground1 : IMAGE_EarringBackground2 )->Show();
}

void CInventoryWindowHandler::OnBeforePutEarring( int iItemBoxID, UIEventArgs eArgs )
{
	if ( auto pItemBox = pWindow->GetElement<UI::ItemBox>( iItemBoxID ) )
	{
		if ( auto pcItemData = pItemBox->GetEventItemData() )
		{
			if ( pcItemData->bValid )
			{
				if ( CGameCharacterStatus::GetInstance()->HasRequirementItem( pcItemData ) )
				{
					pcItemData->sItem.bCanNotUse = FALSE;

					pItemBox->AddItem( pcItemData );
				}
			}
		}
	}
}
