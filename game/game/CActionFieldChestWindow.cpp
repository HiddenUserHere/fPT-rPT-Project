#include "stdafx.h"
#include "CActionFieldChestWindow.h"


CActionFieldChestWindow::CActionFieldChestWindow()
{
}


CActionFieldChestWindow::~CActionFieldChestWindow()
{
}

void CActionFieldChestWindow::Init()
{
	BuildWindow();
}

void CActionFieldChestWindow::Open()
{
	pWindow->Show();
	MOUSEHANDLER->BlockMouse();
}

void CActionFieldChestWindow::Close()
{
	ClearSlots();
	pWindow->Hide();
	MOUSEHANDLER->UnblockMouse();
}

void CActionFieldChestWindow::ClearSlots()
{
	//Clear Chest
	auto pItemBox1 = pWindow->GetElement<UI::ItemBox>( ITEMBOX_Chest );
	pItemBox1->ClearItems();

	//Clear Keys
	auto pItemBox2 = pWindow->GetElement<UI::ItemBox>( ITEMBOX_Key );
	pItemBox2->ClearItems();

	pItemBox1->EnableMouse();
	pItemBox2->EnableMouse();
}

void CActionFieldChestWindow::EnableSlots()
{
	auto pItemBox1 = pWindow->GetElement<UI::ItemBox>( ITEMBOX_Chest );
	auto pItemBox2 = pWindow->GetElement<UI::ItemBox>( ITEMBOX_Key );

	pItemBox1->EnableMouse();
	pItemBox2->EnableMouse();
}

void CActionFieldChestWindow::Render()
{
	pWindow->Render( 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

void CActionFieldChestWindow::OnResolutionChanged()
{
	//Base Rectangle
	int iWindowWidth = pWindow->GetWidth();
	int iWindowHeight = pWindow->GetHeight();
	int iWindowX = (RESOLUTION_WIDTH >> 2) - (iWindowWidth >> 1) + 30;
	int iWindowY = (RESOLUTION_HEIGHT >> 2) - (iWindowHeight >> 1) + 78;
	pWindow->SetPosition( iWindowX, iWindowY );
}

void CActionFieldChestWindow::OnMouseMove( CMouse * pcMouse )
{
	pWindow->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

BOOL CActionFieldChestWindow::OnMouseClick( CMouse * pcMouse )
{
	return pWindow->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

void CActionFieldChestWindow::BuildWindow()
{
	pWindow = std::make_shared<UI::Window>( Rectangle2D( 20, 20, 364, 374 ) );

	//Base Rectangle
	int iWindowWidth = pWindow->GetWidth();
	int iWindowHeight = pWindow->GetHeight();
	int iWindowX = (RESOLUTION_WIDTH >> 2) - (iWindowWidth >> 1) + 30;
	int iWindowY = (RESOLUTION_HEIGHT >> 2) - (iWindowHeight >> 1) + 78;
	pWindow->SetPosition( iWindowX, iWindowY );

	pWindow->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\window.png" ) );

	//Header
	UI::ImageBox_ptr pHeaderBG = std::make_shared<UI::ImageBox>( Rectangle2D( (pWindow->GetWidth() >> 1) - (181 >> 1), -13, 181, 32 ) );
	pHeaderBG->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\text\\header.png" ) );
	pWindow->AddElement( pHeaderBG );

	//Header Title
	UI::ImageBox_ptr pHeaderTitle = std::make_shared<UI::ImageBox>( Rectangle2D( (pWindow->GetWidth() >> 1) - (47 >> 1), -4, 47, 16 ) );
	pHeaderTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\text\\header_event.png" ) );
	pWindow->AddElement( pHeaderTitle );

	//Text Slot
	UI::Text_ptr pTextDescription = std::make_shared<UI::Text>( Rectangle2D( 30, 30, 280, 50 ) );
	pTextDescription->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pTextDescription->SetText( "Hello,\nHere you can exchange your chest boxes and get rewards! ^^" );
	pTextDescription->SetMultiLine( TRUE );
	pTextDescription->SetWordWrap( TRUE );
	pWindow->AddElement( pTextDescription );

	//Text Slot
	UI::Text_ptr pText1 = std::make_shared<UI::Text>( Rectangle2D( 60, 120, 68, 20 ) );
	pText1->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pText1->SetText( "Chest Box:" );
	pWindow->AddElement( pText1 );

	//Slot Chest
	UI::ImageBox_ptr pImageSlot1 = std::make_shared<UI::ImageBox>( Rectangle2D( 60, 140, 68, 90 ) );
	pImageSlot1->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\slot.png" ) );
	pWindow->AddElement( pImageSlot1 );

	UI::ItemBox_ptr pItemBox1 = std::make_shared<UI::ItemBox>( 60, 140, 3, 4 );
	pItemBox1->SetID( ITEMBOX_Chest );
	pItemBox1->SetMaxItems( 1 );
	pItemBox1->AddRule( UI::ItemBox::ERule::RULE_Allow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SilverLockedChest );
	pItemBox1->AddRule( UI::ItemBox::ERule::RULE_Allow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_BlueLockedChest );
	pItemBox1->AddRule( UI::ItemBox::ERule::RULE_Allow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_PurpleLockedChest );
	pItemBox1->AddRule( UI::ItemBox::ERule::RULE_Allow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_GoldenLockedChest );
	pItemBox1->SetEventOnDeniedItem( UI::Event::Build( std::bind( &CActionFieldChestWindow::OnItemNotAllowedChest, this, std::placeholders::_1 ) ) );
	pItemBox1->SetEventOnPutItem( UI::Event::Build( std::bind( &CActionFieldChestWindow::OnPutItemChest, this, std::placeholders::_1 ) ) );
	pItemBox1->SetEventOnGetItem( UI::Event::Build( std::bind( &CActionFieldChestWindow::OnGetItemChest, this, std::placeholders::_1 ) ) );
	pWindow->AddElement( pItemBox1 );
	
	//Text Slot
	UI::Text_ptr pText2 = std::make_shared<UI::Text>( Rectangle2D( 238, 120, 68, 20 ) );
	pText2->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pText2->SetText( "Key Box:" );
	pWindow->AddElement( pText2 );

	//Slot Key
	UI::ImageBox_ptr pImageSlot2 = std::make_shared<UI::ImageBox>( Rectangle2D( 238, 140, 68, 90 ) );
	pImageSlot2->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\slot.png" ) );
	pWindow->AddElement( pImageSlot2 );

	UI::ItemBox_ptr pItemBox2 = std::make_shared<UI::ItemBox>( 238, 140, 3, 4 );
	pItemBox2->SetID( ITEMBOX_Key );
	pItemBox2->SetMaxItems( 12 );
	pItemBox2->AddRule( UI::ItemBox::ERule::RULE_Allow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_ChestKey );
	pItemBox2->SetEventOnDeniedItem( UI::Event::Build( std::bind( &CActionFieldChestWindow::OnItemNotAllowedChest, this, std::placeholders::_1 ) ) );
	pItemBox2->SetEventOnPutItem( UI::Event::Build( std::bind( &CActionFieldChestWindow::OnPutItemChest, this, std::placeholders::_1 ) ) );
	pItemBox2->SetEventOnGetItem( UI::Event::Build( std::bind( &CActionFieldChestWindow::OnGetItemChest, this, std::placeholders::_1 ) ) );
	pWindow->AddElement( pItemBox2 );

	//Exchange
	UI::Button_ptr pButtonExchange = std::make_shared<UI::Button>( Rectangle2D( (pWindow->GetWidth() >> 1) - (153 >> 1), 250, 153, 29 ) );
	pButtonExchange->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\events\\ghostcastle\\btnexchange.png", "game\\images\\events\\ghostcastle\\btnexchange_.png" ) );
	pButtonExchange->SetEvent( UI::Event::Build( std::bind( &CActionFieldChestWindow::OnButtonExchangeClick, this, std::placeholders::_1 ) ) );
	pWindow->AddElement( pButtonExchange );

	//Close
	UI::Button_ptr pButtonClose = std::make_shared<UI::Button>( Rectangle2D( (pWindow->GetWidth() >> 1) - (93 >> 1), 342, 93, 23 ) );
	pButtonClose->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\btnclose.png" ) );
	pButtonClose->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\btnclose.png", "game\\images\\UI\\window\\btnclose_.png" ) );
	pButtonClose->SetEvent( UI::Event::Build( std::bind( &CActionFieldChestWindow::OnButtonCloseClick, this, std::placeholders::_1 ) ) );
	pWindow->AddElement( pButtonClose );

	pWindow->Hide();
}

void CActionFieldChestWindow::OnItemNotAllowedChest( UIEventArgs eArgs )
{
}

void CActionFieldChestWindow::OnItemNotAllowedKey( UIEventArgs eArgs )
{
}

void CActionFieldChestWindow::OnPutItemChest( UIEventArgs eArgs )
{
}

void CActionFieldChestWindow::OnPutItemKey( UIEventArgs eArgs )
{
}

void CActionFieldChestWindow::OnGetItemChest( UIEventArgs eArgs )
{
}

void CActionFieldChestWindow::OnGetItemKey( UIEventArgs eArgs )
{
}

void CActionFieldChestWindow::OnButtonCloseClick( UIEventArgs eArgs )
{
	//Clear Chest
	auto pItemBox1 = pWindow->GetElement<UI::ItemBox>( ITEMBOX_Chest );
	auto pItemBox2 = pWindow->GetElement<UI::ItemBox>( ITEMBOX_Key );

	if ( (pItemBox1->GetItems().size() == 0) && (pItemBox2->GetItems().size() == 0) )
		Close();
	else
		TITLEBOX( "Please, remove items from slot!" );
}

void CActionFieldChestWindow::OnButtonExchangeClick( UIEventArgs eArgs )
{
	if ( dwTimeSend < TICKCOUNT )
	{
		PacketActionFieldExchange sPacket;

		//Get Chest
		auto pItemBox1 = pWindow->GetElement<UI::ItemBox>( ITEMBOX_Chest );
		std::vector<ItemData *> & vItemChest = pItemBox1->GetItems();
		if ( vItemChest.size() == 1 )
		{
			ItemData * pcItemData = vItemChest[0];
			sPacket.iItemID = pcItemData->sItem.sItemID.ToItemID();
			sPacket.iChk1 = pcItemData->sItem.iChk1;
			sPacket.iChk2 = pcItemData->sItem.iChk2;

			//Get Keys
			auto pItemBox2 = pWindow->GetElement<UI::ItemBox>( ITEMBOX_Key );
			std::vector<ItemData *> & vItemKeys = pItemBox2->GetItems();
			if ( vItemKeys.size() > 0 )
			{
				sPacket.iItemCount = 0;
				for ( auto pcItemData : vItemKeys )
				{
					sPacket.iaKeyItemID[sPacket.iItemCount] = pcItemData->sItem.sItemID.ToItemID();
					sPacket.iaKeyChk1[sPacket.iItemCount] = pcItemData->sItem.iChk1;
					sPacket.iaKeyChk2[sPacket.iItemCount] = pcItemData->sItem.iChk2;
					sPacket.iItemCount++;
				}


				pItemBox1->DisableMouse();
				pItemBox2->DisableMouse();

				ACTIONFIELDHANDLER->ProcessPacket( &sPacket );

				dwTimeSend = TICKCOUNT + 2000;
			}
			else
				TITLEBOX( "You need to put some Chest Keys!" );
		}
		else
			TITLEBOX( "You need to put a Chest Box!" );
	}
}
