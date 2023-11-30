#include "stdafx.h"
#include "CItemBoxWindowHandler.h"


CItemBoxWindowHandler::CItemBoxWindowHandler()
{
	pWindow1 = std::make_shared<UI::Window>( Rectangle2D( 20, 20, 364, 374 ) );
}


CItemBoxWindowHandler::~CItemBoxWindowHandler()
{
	pWindow1 = nullptr;
}

void CItemBoxWindowHandler::Init()
{
	BuildWindow1();
	BuildWindowMain();
	BuildWindowReceive();
	BuildWindowSend();
}

void CItemBoxWindowHandler::Open( EWindowItemBoxID iWindowID )
{
	//Close all windows
	pWindow1->GetElement<UI::Window>( WINDOW_Receive )->GetElement<UI::Window>( WINDOW_ReceiveItems )->GetElement<UI::ItemBox>( ITEMBOX_Items )->ClearItems();
	pWindow1->GetElement<UI::Window>( WINDOW_Main )->Hide();
	pWindow1->GetElement<UI::Window>( WINDOW_Receive )->Hide();
	pWindow1->GetElement<UI::Window>( WINDOW_Send )->Hide();

	switch ( iWindowID )
	{
		case EWindowItemBoxID::WINDOWID_Main:
			pWindow1->GetElement<UI::Window>( WINDOW_Main )->Show();
			break;
		case EWindowItemBoxID::WINDOWID_Receive:
			pWindow1->GetElement<UI::Window>( WINDOW_Receive )->GetElement<UI::Window>( WINDOW_ReceiveItems )->Hide();
			pWindow1->GetElement<UI::Window>( WINDOW_Receive )->GetElement<UI::Window>( WINDOW_ReceiveEmpty )->Hide();
			pWindow1->GetElement<UI::Window>( WINDOW_Receive )->Show();
			break;
		case EWindowItemBoxID::WINDOWID_Send:
			pWindow1->GetElement<UI::Window>( WINDOW_Send )->GetElement<UI::InputField>( INPUT_CharacterName )->ClearInput();
			pWindow1->GetElement<UI::Window>( WINDOW_Send )->Show();
			break;
	}

	this->iWindowID = iWindowID;

	if ( !pWindow1->IsOpen() )
		MOUSEHANDLER->BlockMouse();

	WINDOW_INVENTORY = TRUE;

	pWindow1->Show();
}

void CItemBoxWindowHandler::Close()
{
	if ( pWindow1->IsOpen() )
		MOUSEHANDLER->UnblockMouse();

	pWindow1->GetElement<UI::Window>( WINDOW_Receive )->GetElement<UI::Window>( WINDOW_ReceiveItems )->GetElement<UI::ItemBox>( ITEMBOX_Items )->ClearItems();
	pWindow1->Hide();
}

void CItemBoxWindowHandler::OnMouseMove( CMouse * pcMouse )
{
	pWindow1->OnMouseMove( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

BOOL CItemBoxWindowHandler::OnMouseClick( CMouse * pcMouse )
{
	return pWindow1->OnMouseClick( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

BOOL CItemBoxWindowHandler::OnKeyPress( CKeyboard * pcKeyboard )
{
	return pWindow1->OnKeyPress( pcKeyboard );
}

BOOL CItemBoxWindowHandler::OnKeyChar( CKeyboard * pcKeyboard )
{
	return pWindow1->OnKeyChar( pcKeyboard );
}

void CItemBoxWindowHandler::Render()
{
	pWindow1->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

void CItemBoxWindowHandler::Update( float fTime )
{
	if ( fItemBoxSendBlockTime > 0.0f )
	{
		fItemBoxSendBlockTime -= fTime;

		if ( fItemBoxSendBlockTime <= 0.0f )
		{
			if ( iWindowID == EWindowItemBoxID::WINDOWID_Send )
				pWindow1->GetElement<UI::Window>( WINDOW_Send )->GetElement<UI::ItemBox>( ITEMBOX_Items )->EnableMouse();

			fItemBoxSendBlockTime = 0.0f;
		}
	}
}

void CItemBoxWindowHandler::OnResolutionChanged()
{
	//Base Rectangle
	int iWindowWidth = pWindow1->GetWidth();
	int iWindowHeight = pWindow1->GetHeight();
	int iWindowX = (RESOLUTION_WIDTH >> 2) - (iWindowWidth >> 1) + 30;
	int iWindowY = (RESOLUTION_HEIGHT >> 2) - (iWindowHeight >> 1) + 78;
	pWindow1->SetPosition( iWindowX, iWindowY );
}

void CItemBoxWindowHandler::ClearReceiveWindow()
{
	if ( iWindowID == EWindowItemBoxID::WINDOWID_Receive )
	{
		auto pItemBox = pWindow1->GetElement<UI::Window>( WINDOW_Receive )->GetElement<UI::Window>( WINDOW_ReceiveItems )->GetElement<UI::ItemBox>( ITEMBOX_Items );
		pItemBox->ClearItems();
		pWindow1->GetElement<UI::Window>( WINDOW_Receive )->GetElement<UI::Window>( WINDOW_ReceiveItems )->Hide();
		pWindow1->GetElement<UI::Window>( WINDOW_Receive )->GetElement<UI::Window>( WINDOW_ReceiveEmpty )->Hide();
	}
}

void CItemBoxWindowHandler::ShowEmptyReceiveWindow()
{
	if ( iWindowID == EWindowItemBoxID::WINDOWID_Receive )
		pWindow1->GetElement<UI::Window>( WINDOW_Receive )->GetElement<UI::Window>( WINDOW_ReceiveEmpty )->Show();
}

void CItemBoxWindowHandler::AddItemReceive( const char * pszSenderName, SYSTEMTIME sDate, ItemData * pcItemData )
{
	if ( iWindowID == EWindowItemBoxID::WINDOWID_Receive )
	{
		auto pItemBox = pWindow1->GetElement<UI::Window>( WINDOW_Receive )->GetElement<UI::Window>( WINDOW_ReceiveItems )->GetElement<UI::ItemBox>( ITEMBOX_Items );

		BOOL bSuccess = FALSE;

		ItemData cItemData;
		if ( pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_GoldAndExp )
		{
			CopyMemory( &cItemData, pcItemData, sizeof( ItemData ) );
			cItemData.sBaseItemID = cItemData.sItem.sItemID;
			cItemData.bValid = TRUE;
			bSuccess = CGameInventory::GetInstance()->LoadItemImageEXPGold( &cItemData );
		}
		else
			bSuccess = ITEMHANDLER->LoadItemImage( &pcItemData->sItem, &cItemData );

		if ( bSuccess )
		{
			pItemBox->ClearItems();

			pItemBox->AddItem( &cItemData );

			auto pTextGoldEXP = pWindow1->GetElement<UI::Window>( WINDOW_Receive )->GetElement<UI::Window>( WINDOW_ReceiveItems )->GetElement<UI::Text>( TEXT_GoldEXP );
			pTextGoldEXP->SetText( "" );

			if ( pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_GoldAndExp )
			{
				if ( pcItemData->sItem.sItemID.ToItemID() == ITEMID_Gold )
					pTextGoldEXP->SetTextEx( "Gold: %s gp", FormatNumber( pcItemData->sItem.iSalePrice ) );
				else if ( pcItemData->sItem.sItemID.ToItemID() == ITEMID_Experience )
				{
					LARGE_INTEGER li;
					li.LowPart = pcItemData->sOldPosition.iX;
					li.HighPart = pcItemData->sOldPosition.iY;

					pTextGoldEXP->SetTextEx( "EXP: %s", FormatNumber( li.QuadPart ) );
				}
			}

			auto pTextSender = pWindow1->GetElement<UI::Window>( WINDOW_Receive )->GetElement<UI::Window>( WINDOW_ReceiveItems )->GetElement<UI::Text>( TEXT_Sender );
			pTextSender->SetTextEx( "Sender: %s   Date: %02d-%02d-%04d %02d:%02d", pszSenderName, sDate.wMonth, sDate.wDay, sDate.wYear, sDate.wHour, sDate.wMinute );

			pWindow1->GetElement<UI::Window>( WINDOW_Receive )->GetElement<UI::Window>( WINDOW_ReceiveEmpty )->Hide();
			pWindow1->GetElement<UI::Window>( WINDOW_Receive )->GetElement<UI::Window>( WINDOW_ReceiveItems )->Show();
		}
	}
}

void CItemBoxWindowHandler::RemoveItem( int iItemID, int iChk1, int iChk2 )
{
	if ( iWindowID == EWindowItemBoxID::WINDOWID_Send )
	{
		auto pItemBox = pWindow1->GetElement<UI::Window>( WINDOW_Send )->GetElement<UI::ItemBox>( ITEMBOX_Items );
		if ( pItemBox->GetItems().size() > 0 )
		{
			pItemBox->RemoveItem( iItemID, iChk1, iChk2 );
		}
	}
}

void CItemBoxWindowHandler::ItemBoxSendBlock( BOOL bBlock )
{
	if ( bBlock )
		pWindow1->GetElement<UI::Window>( WINDOW_Send )->GetElement<UI::ItemBox>( ITEMBOX_Items )->DisableMouse();
	else
	{
		pWindow1->GetElement<UI::Window>( WINDOW_Send )->GetElement<UI::ItemBox>( ITEMBOX_Items )->EnableMouse();
		pWindow1->GetElement<UI::Window>( WINDOW_Send )->GetElement<UI::InputField>( INPUT_CharacterName )->ClearInput();
	}

	fItemBoxSendBlockTime = bBlock ? 3000.0f : 0.0f;
}

void CItemBoxWindowHandler::BuildWindow1()
{
	//Base Rectangle
	int iWindowWidth = pWindow1->GetWidth();
	int iWindowHeight = pWindow1->GetHeight();
	int iWindowX = (RESOLUTION_WIDTH >> 2) - (iWindowWidth >> 1) + 30;
	int iWindowY = (RESOLUTION_HEIGHT >> 2) - (iWindowHeight >> 1) + 78;
	pWindow1->SetPosition( iWindowX, iWindowY );

	pWindow1->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\window.png" ) );

	UI::ImageBox_ptr pHeaderBG = std::make_shared<UI::ImageBox>( Rectangle2D( (pWindow1->GetWidth() >> 1) - (181 >> 1), -13, 181, 32 ) );
	pHeaderBG->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\text\\header.png" ) );
	pWindow1->AddElement( pHeaderBG );

	//Header Title
	UI::ImageBox_ptr pHeaderTitle = std::make_shared<UI::ImageBox>( Rectangle2D( (pWindow1->GetWidth() >> 1) - (75 >> 1), -6, 75, 18 ) );
	pHeaderTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\distributor\\title.png" ) );
	pWindow1->AddElement( pHeaderTitle );

	pWindow1->Hide();
}

void CItemBoxWindowHandler::BuildWindowMain()
{
	UI::Window_ptr pWindowMain = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 364, 374 ) );
	pWindowMain->SetID( WINDOW_Main );

	//Text About
	UI::TextWindow_ptr pTextWindow = std::make_shared<UI::TextWindow>( Rectangle2D( 30, 40, 296, 160 ) );
	pTextWindow->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pTextWindow->LoadTextFromFile( "game\\npcs\\itembox\\main.txt" );
	pWindowMain->AddElement( pTextWindow );

	//Button Retrive
	UI::Button_ptr pButtonRetrive = std::make_shared<UI::Button>( Rectangle2D( 70, 180, 165, 21 ) );
	pButtonRetrive->SetID( BUTTON_Receive );
	pButtonRetrive->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\distributor\\retrieveitems.png", "game\\images\\distributor\\retrieveitems_.png" ) );
	pButtonRetrive->SetEvent( UI::Event::Build( std::bind( &CItemBoxWindowHandler::OnButtonRetriveItemWindowClick, this, std::placeholders::_1 ) ) );
	pWindowMain->AddElement( pButtonRetrive );

	//Text Retrive Items
	UI::Text_ptr pTextRetrive = std::make_shared<UI::Text>( Rectangle2D( 70, 201, 165, 20 ) );
	pTextRetrive->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pTextRetrive->SetText( "Check your Item Box for items" );
	pWindowMain->AddElement( pTextRetrive );

	//Button Send
	UI::Button_ptr pButtonSend = std::make_shared<UI::Button>( Rectangle2D( 126, 262, 137, 21 ) );
	pButtonSend->SetID( BUTTON_Send );
	pButtonSend->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\distributor\\senditems.png", "game\\images\\distributor\\senditems_.png" ) );
	pButtonSend->SetEvent( UI::Event::Build( std::bind( &CItemBoxWindowHandler::OnButtonSendItemWindowClick, this, std::placeholders::_1 ) ) );
	pWindowMain->AddElement( pButtonSend );

	//Text Send Items
	UI::Text_ptr pTextSend = std::make_shared<UI::Text>( Rectangle2D( 70, 283, 165, 20 ) );
	pTextSend->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pTextSend->SetText( "Send items to an Item Box of another player" );
	pWindowMain->AddElement( pTextSend );

	//Button Close
	UI::Button_ptr pButtonClose = std::make_shared<UI::Button>( Rectangle2D( (pWindowMain->GetWidth() >> 1) - (93 >> 1), 342, 93, 23 ) );
	pButtonClose->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\btnclose.png" ) );
	pButtonClose->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\btnclose.png", "game\\images\\UI\\window\\btnclose_.png" ) );
	pButtonClose->SetEvent( UI::Event::Build( std::bind( &CItemBoxWindowHandler::OnButtonCloseClick, this, std::placeholders::_1 ) ) );
	pWindowMain->AddElement( pButtonClose );

	pWindow1->AddElement( pWindowMain );
}

void CItemBoxWindowHandler::BuildWindowReceive()
{
	UI::Window_ptr pWindowReceive = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 364, 374 ) );
	pWindowReceive->SetID( WINDOW_Receive );

	//Header Text
	UI::Text_ptr pTextHeader = std::make_shared<UI::Text>( Rectangle2D( 30, 40, 300, 18 ) );
	pTextHeader->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pTextHeader->SetText( "Hold on, let me see if I have anything in your name." );
	pWindowReceive->AddElement( pTextHeader );

	//Empty
	{
		UI::Window_ptr pWindowReceiveEmpty = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 364, 374 ) );
		pWindowReceiveEmpty->SetID( WINDOW_ReceiveEmpty );
		
		UI::Text_ptr pTextEmpty = std::make_shared<UI::Text>( Rectangle2D( 30, 80, 304, 80 ) );
		pTextEmpty->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
		pTextEmpty->SetText( "Hmm... guess not. Yeah, there's nothing for you here at the moment.\n\nFeel free to come back later and I will check again." );
		pTextEmpty->SetMultiLine( TRUE );
		pTextEmpty->SetWordWrap( TRUE );
		pWindowReceiveEmpty->AddElement( pTextEmpty );

		UI::ImageBox_ptr pImageEmptyBox = std::make_shared<UI::ImageBox>( Rectangle2D( (pWindowReceiveEmpty->GetWidth() >> 1) - (159 >> 1), 180, 159, 142 ) );
		pImageEmptyBox->SetImage( UI::ImageLoader::LoadImage( "game\\images\\distributor\\emptyitembox_noglow.png" ) );
		pWindowReceiveEmpty->AddElement( pImageEmptyBox );

		pWindowReceiveEmpty->Show();

		pWindowReceive->AddElement( pWindowReceiveEmpty );
	}

	//With Item
	{
		UI::Window_ptr pWindowReceiveItem = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 364, 374 ) );
		pWindowReceiveItem->SetID( WINDOW_ReceiveItems );

		pWindowReceiveItem->Hide();

		//Item Box
		UI::ItemBox_ptr pItemBox = std::make_shared<UI::ItemBox>( 129, 80, 5, 5 );
		pItemBox->SetMaxItems( 1 );
		pItemBox->SetID( ITEMBOX_Items );
		pItemBox->SetDisableItemSound( TRUE );
		pItemBox->DisableMouse();
		pWindowReceiveItem->AddElement( pItemBox );

		//Sent Text
		UI::Text_ptr pTextSender = std::make_shared<UI::Text>( Rectangle2D( 30, 192, 300, 20 ) );
		pTextSender->SetID( TEXT_Sender );
		pTextSender->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
		pTextSender->SetText( "" );
		pTextSender->SetHorizontalAlign( ALIGN_Center );
		pWindowReceiveItem->AddElement( pTextSender );

		//GoldEXP Text
		UI::Text_ptr pTextGoldEXP = std::make_shared<UI::Text>( Rectangle2D( 30, 172, 300, 20 ) );
		pTextGoldEXP->SetID( TEXT_GoldEXP );
		pTextGoldEXP->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
		pTextGoldEXP->SetText( "" );
		pTextGoldEXP->SetHorizontalAlign( ALIGN_Center );
		pWindowReceiveItem->AddElement( pTextGoldEXP );

		//Get Item
		UI::Button_ptr pButtonGetItem = std::make_shared<UI::Button>( Rectangle2D( 20, 230, 165, 80 ) );
		pButtonGetItem->SetImage( UI::ImageLoader::LoadImage( "game\\images\\distributor\\button_accept.png" ) );
		pButtonGetItem->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\distributor\\button_accept.png", "game\\images\\distributor\\button_accept_.png" ) );
		pButtonGetItem->SetEvent( UI::Event::Build( std::bind( &CItemBoxWindowHandler::OnButtonGetItemClick, this, std::placeholders::_1 ) ) );
		pWindowReceiveItem->AddElement( pButtonGetItem );

		//Discard Item
		UI::Button_ptr pButtonDiscardItem = std::make_shared<UI::Button>( Rectangle2D( pWindowReceiveItem->GetWidth() - 174, 230, 165, 80 ) );
		pButtonDiscardItem->SetImage( UI::ImageLoader::LoadImage( "game\\images\\distributor\\button_discard.png" ) );
		pButtonDiscardItem->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\distributor\\button_discard.png", "game\\images\\distributor\\button_discard_.png" ) );
		pButtonDiscardItem->SetEvent( UI::Event::Build( std::bind( &CItemBoxWindowHandler::OnButtonDiscardItemClick, this, std::placeholders::_1 ) ) );
		pWindowReceiveItem->AddElement( pButtonDiscardItem );

		pWindowReceiveItem->AddElement( pItemBox );

		pWindowReceive->AddElement( pWindowReceiveItem );
	}

	UI::Button_ptr pButtonBack = std::make_shared<UI::Button>( Rectangle2D( (pWindowReceive->GetWidth() >> 1) - (93 >> 1), 342, 93, 23 ) );
	pButtonBack->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\btnclose.png" ) );
	pButtonBack->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\btnclose.png", "game\\images\\UI\\window\\btnclose_.png" ) );
	pButtonBack->SetEvent( UI::Event::Build( std::bind( &CItemBoxWindowHandler::OnButtonBackClick, this, std::placeholders::_1 ) ) );
	pWindowReceive->AddElement( pButtonBack );

	pWindow1->AddElement( pWindowReceive );
}

void CItemBoxWindowHandler::BuildWindowSend()
{
	UI::Window_ptr pWindowSend = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 364, 374 ) );
	pWindowSend->SetID( WINDOW_Send );

	//Text About
	UI::TextWindow_ptr pTextWindow = std::make_shared<UI::TextWindow>( Rectangle2D( 30, 40, 316, 150 ) );
	pTextWindow->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pTextWindow->LoadTextFromFile( "game\\npcs\\itembox\\send.txt" );
	pWindowSend->AddElement( pTextWindow );

	//Item Box Image
	UI::ImageBox_ptr pImageSlot = std::make_shared<UI::ImageBox>( Rectangle2D( 30, 140, 210, 112 ) );
	pImageSlot->SetImage( UI::ImageLoader::LoadImage( "game\\images\\distributor\\itemboxdemonstration.png" ) );
	pWindowSend->AddElement( pImageSlot );

	//Item Box
	UI::ItemBox_ptr pItemBox = std::make_shared<UI::ItemBox>( 129, 141, 5, 5 );
	pItemBox->SetMaxItems( 25 );
	pItemBox->SetID( ITEMBOX_Items );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemBase, ITEMBASE_Potion );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemBase, ITEMBASE_Quest1 );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemBase, ITEMBASE_Quest2 );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_EquippedItem, TRUE );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemTime, FALSE );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_BeginnerAxe );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_BeginnerClaw );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_BeginnerHammer );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_BeginnerSword );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_BeginnerJavelin );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_BeginnerBow );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_BeginnerScythe );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_BeginnerPhantom );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_BeginnerDagger );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_BeginnerStaff );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_BeginnerBoots );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_BeginnerGauntlet );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_BeginnerBracelet );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_GrumpyArmor );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_GrumpyRobe );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_MechanicCostumeM );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_MechanicCostumeF );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_BroyaleCostumeMale );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_BroyaleCostumeFemale );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_HopyCostumeTime );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_CuepyCostumeTime );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_HopyCostumeIce );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_CuepyCostumeIce );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SandurrRedCostumeMale );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SandurrRedCostumeFemale );

	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_HalloweenAxe );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_HalloweenClaw );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_HalloweenDagger );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_HalloweenHammer );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_HalloweenStaff );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_HalloweenWand );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_HalloweenPhantom );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_HalloweenScythe );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_HalloweenBow );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_HalloweenSword );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_HalloweenBladeSword );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_HalloweenJavelin );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_HalloweenShield );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_Succubus );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_VampireSuit );

	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_XmasCostumeOldAxe );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_XmasCostumeOldAxe1Hand );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_XmasCostumeOldClaw );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_XmasCostumeOldDagger );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_XmasCostumeOldHammer );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_XmasCostumeOldStaff );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_XmasCostumeOldPhantom );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_XmasCostumeOldScythe );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_XmasCostumeOldBow );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_XmasCostumeOldSword );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_XmasCostumeOldSword1Hand );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_XmasCostumeOldJavelin );

	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_XmasCostumeAxe );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_XmasCostumeAxe1Hand );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_XmasCostumeClaw );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_XmasCostumeDagger );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_XmasCostumeHammer );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_XmasCostumeStaff );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_XmasCostumeWand );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_XmasCostumePhantom );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_XmasCostumeScythe );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_XmasCostumeBow );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_XmasCostumeSword );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_XmasCostumeSword1Hand );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_XmasCostumeJavelin );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_XmasCostumeShield );

	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_ObscureAxe );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_ObscureAxe1Hand );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_ObscureClaw );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_ObscureDagger );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_ObscureHammer );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_ObscureStaff );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_ObscureWand );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_ObscurePhantom );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_ObscureScythe );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_ObscureBow );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_ObscureSword );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_ObscureSword1Hand );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_ObscureJavelin );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_ObscureShield );

	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SandurrRedAxe );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SandurrRedClaw );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SandurrRedDagger );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SandurrRedHammer );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SandurrRedStaff );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SandurrRedPhantom );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SandurrRedScythe );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SandurrRedBow );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SandurrRedSword );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SandurrRedJavelin );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SandurrRedShield );
																										  
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SandurrRedAxe1Hand );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SandurrPurpleAxe1Hand );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SandurrRedSword1Hand );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SandurrPurpleAxe1Hand );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SandurrPurple2Scythe );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SandurrPurpleAxe );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SandurrPurpleClaw );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SandurrPurpleDagger );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SandurrPurpleHammer );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SandurrPurpleStaff );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SandurrPurplePhantom );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SandurrPurpleScythe );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SandurrPurpleBow );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SandurrPurpleSword );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SandurrPurpleJavelin );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SandurrPurpleShield );

	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_IceCostumeMale );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_IceCostumeFemale );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_GoldenHopyCostumeMaleTime );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_GoldenHopyCostumeFemaleTime );

	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_CandySheltom );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_CeruleanRing );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_LockItemGM, 0 );

	pWindowSend->AddElement( pItemBox );

	//Character Name Description
	UI::Text_ptr pTextCharacterNameDescription = std::make_shared<UI::Text>( Rectangle2D( 30, 260, 260, 20 ) );
	pTextCharacterNameDescription->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pTextCharacterNameDescription->SetText( "Fill in below the recipient's Character Name." );
	pWindowSend->AddElement( pTextCharacterNameDescription );

	//Character Name
	UI::Text_ptr pTextCharacterName = std::make_shared<UI::Text>( Rectangle2D( 30, 290, 260, 20 ) );
	pTextCharacterName->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pTextCharacterName->SetText( "Name:" );
	pWindowSend->AddElement( pTextCharacterName );

	//Input Image
	UI::ImageBox_ptr pImageInput = std::make_shared<UI::ImageBox>( Rectangle2D( 74, 306, 140, 1 ) );
	pImageInput->SetImage( UI::ImageLoader::LoadImage( "game\\images\\distributor\\underline.png" ) );
	pWindowSend->AddElement( pImageInput );

	UI::Group_ptr pGroupInput = std::make_shared<UI::Group>();
	pWindowSend->AddElement( pGroupInput );

	//Character Name
	UI::InputField_ptr pInputName = std::make_shared<UI::InputField>( Rectangle2D( 74, 290, 140, 20 ) );
	pInputName->SetID( INPUT_CharacterName );
	pInputName->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pInputName->SetText( "" );
	pInputName->SetMaxLength( 18 );
	pInputName->SetSelect( TRUE );
	pInputName->SetGroup( pGroupInput );
	pWindowSend->AddElement( pInputName );

	//Button Send
	UI::Button_ptr pButtonSend = std::make_shared<UI::Button>( Rectangle2D( 222, 288, 116, 36 ) );
	pButtonSend->SetImage( UI::ImageLoader::LoadImage( "game\\images\\distributor\\button_send.png" ) );
	pButtonSend->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\distributor\\button_send.png", "game\\images\\distributor\\button_send_.png" ) );
	pButtonSend->SetEvent( UI::Event::Build( std::bind( &CItemBoxWindowHandler::OnButtonSendItemClick, this, std::placeholders::_1 ) ) );
	pWindowSend->AddElement( pButtonSend );

	UI::Button_ptr pButtonBack = std::make_shared<UI::Button>( Rectangle2D( (pWindowSend->GetWidth() >> 1) - (93 >> 1), 342, 93, 23 ) );
	pButtonBack->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\btnclose.png" ) );
	pButtonBack->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\btnclose.png", "game\\images\\UI\\window\\btnclose_.png" ) );
	pButtonBack->SetEvent( UI::Event::Build( std::bind( &CItemBoxWindowHandler::OnButtonBackClick, this, std::placeholders::_1 ) ) );
	pWindowSend->AddElement( pButtonBack );

	pWindow1->AddElement( pWindowSend );
}

void CItemBoxWindowHandler::OnButtonSendItemWindowClick( UIEventArgs eArgs )
{
	Open( EWindowItemBoxID::WINDOWID_Send );
}

void CItemBoxWindowHandler::OnButtonRetriveItemWindowClick( UIEventArgs eArgs )
{
	Open( EWindowItemBoxID::WINDOWID_Receive );

	PacketNPCItemBox sPacket;
	sPacket.bItem = FALSE;
	ITEMBOXHANDLER->ProcessPacket( &sPacket );
}

void CItemBoxWindowHandler::OnButtonBackClick( UIEventArgs eArgs )
{
	if ( iWindowID == EWindowItemBoxID::WINDOWID_Send )
	{
		if ( pWindow1->GetElement<UI::Window>( WINDOW_Send )->GetElement<UI::ItemBox>( ITEMBOX_Items )->GetItems().size() > 0 )
		{
			if ( fItemBoxSendBlockTime == 0.0f )
				TITLEBOX( "Remove items before close the window!" );
			else
				TITLEBOX( "Item box still processing data" );
			return;
		}
	}

	Open( EWindowItemBoxID::WINDOWID_Main );
}

void CItemBoxWindowHandler::OnButtonCloseClick( UIEventArgs eArgs )
{
	Close();
}

void CItemBoxWindowHandler::OnButtonGetItemClick( UIEventArgs eArgs )
{
	HandleReceiveItemReply( TRUE );
}

void CItemBoxWindowHandler::OnButtonDiscardItemClick( UIEventArgs eArgs )
{
	MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
	MESSAGEBOX->SetTitle( "Discard Item" );
	MESSAGEBOX->SetDescription( "Do you want to discard this item?" );
	MESSAGEBOX->SetEvent( std::bind( &CItemBoxWindowHandler::OnAcceptDiscardItemClick, this, std::placeholders::_1 ) );
	MESSAGEBOX->SetAutoClose( 10 );
	MESSAGEBOX->Show();
}

void CItemBoxWindowHandler::OnAcceptDiscardItemClick( UIEventArgs eArgs )
{
	if ( MESSAGEBOX->GetType() )
		HandleReceiveItemReply( FALSE );
}

void CItemBoxWindowHandler::OnButtonSendItemClick( UIEventArgs eArgs )
{
	if ( fItemBoxSendBlockTime == 0.0f )
	{
		auto pItemBox = pWindow1->GetElement<UI::Window>( WINDOW_Send )->GetElement<UI::ItemBox>( ITEMBOX_Items );
		if ( pItemBox->GetItems().size() > 0 )
		{
			int iGold = 75000 * pItemBox->GetItems().size();

			if ( CHARACTERGAME->CanUseGold( iGold ) && (UNITDATA->sCharacterData.iGold - iGold) > 0 )
			{
				auto pInputName = pWindow1->GetElement<UI::Window>( WINDOW_Send )->GetElement<UI::InputField>( INPUT_CharacterName );
				if ( pInputName->GetText().length() > 0 )
				{
					MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
					MESSAGEBOX->SetTitle( "Send Items" );
					MESSAGEBOX->SetDescription( FormatString( "Do you want to send items to %s?", pInputName->GetText().c_str() ) );
					MESSAGEBOX->SetEvent( std::bind( &CItemBoxWindowHandler::OnAcceptSendItemClick, this, std::placeholders::_1 ) );
					MESSAGEBOX->SetAutoClose( 10 );
					MESSAGEBOX->Show();
				}
				else
					TITLEBOX( "Fill the character name" );
			}
			else
				TITLEBOX( "Not enough gold! You need %s gp!", FormatNumber( iGold ) );
		}
		else
			TITLEBOX( "Put an item on item box to send" );
	}
}

void CItemBoxWindowHandler::OnAcceptSendItemClick( UIEventArgs eArgs )
{
	if ( MESSAGEBOX->GetType() )
	{
		auto pInputName = pWindow1->GetElement<UI::Window>( WINDOW_Send )->GetElement<UI::InputField>( INPUT_CharacterName );
		if ( pInputName->GetText().length() > 0 )
		{
			auto pItemBox = pWindow1->GetElement<UI::Window>( WINDOW_Send )->GetElement<UI::ItemBox>( ITEMBOX_Items );
			if ( pItemBox->GetItems().size() > 0 )
			{
				PacketNPCItemBoxSend sPacket;
				STRINGCOPY( sPacket.szCharacterName, pInputName->GetText().c_str() );
				sPacket.iCount = 0;
				for ( const auto pcItemData : pItemBox->GetItems() )
				{
					CopyMemory( &sPacket.cItemData[sPacket.iCount++], pcItemData, sizeof( ItemData ) );

					if ( sPacket.iCount == MAX_ITEMBOX_SEND_QUEUE )
					{
						ITEMBOXHANDLER->ProcessPacket( &sPacket );
						sPacket.iCount = 0;
					}
				}

				if ( sPacket.iCount > 0 )
					ITEMBOXHANDLER->ProcessPacket( &sPacket );

				ItemBoxSendBlock( TRUE );
			}
			else
				TITLEBOX( "Put an item on item box to send" );
		}
		else
			TITLEBOX( "Fill the character name" );
	}
}

void CItemBoxWindowHandler::HandleReceiveItemReply( BOOL bGetItem )
{
	auto pItemBox = pWindow1->GetElement<UI::Window>( WINDOW_Receive )->GetElement<UI::Window>( WINDOW_ReceiveItems )->GetElement<UI::ItemBox>( ITEMBOX_Items );
	if ( pItemBox->GetItems().size() > 0 )
	{
		ItemData * pcItemData = pItemBox->GetItems()[0];
		PacketNPCItemBox sPacket;
		sPacket.bItem = TRUE;
		sPacket.bAccepted = bGetItem;
		CopyMemory( &sPacket.cItemData, pcItemData, sizeof( ItemData ) );

		if ( bGetItem )
		{
			if ( CHARACTERGAME->CheckInventorySpace( pcItemData ) )
				ITEMBOXHANDLER->ProcessPacket( &sPacket );
			else
				TITLEBOX( "Not enough space in inventory" );
		}
		else
		{
			ITEMBOXHANDLER->ProcessPacket( &sPacket );

			pItemBox->ClearItems();

			Open( EWindowItemBoxID::WINDOWID_Receive );
		}

	}
}
