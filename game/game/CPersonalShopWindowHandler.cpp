#include "stdafx.h"
#include "CPersonalShopWindowHandler.h"


CPersonalShopWindowHandler::CPersonalShopWindowHandler()
{
	pWindow1 = std::make_shared<UI::Window>( Rectangle2D( 0, 40, PERSONALSHOP_WINDOW_WIDTH, PERSONALSHOP_WINDOW_HEIGHT ) );
}


CPersonalShopWindowHandler::~CPersonalShopWindowHandler()
{
	pWindow1 = nullptr;
}

void CPersonalShopWindowHandler::Init()
{
	BuildWindow();
}

BOOL CPersonalShopWindowHandler::IsOpen()
{
	return pWindow1->IsOpen();
}

void CPersonalShopWindowHandler::Open( bool bSelfShop )
{
	if ( pWindow1->IsOpen() == FALSE )
	{
		if ( bSelfShop )
		{
			if ( !ACCOUNTHANDLER->IsAccountShareDenied( ACCOUNTSHARE_DenySetupPersonalShop ) )
			{
				SetWindowState( WINDOWSTATE_Opening );

				pWindow1->Show();

				pWindow1->GetElement<UI::Button>( BUTTON_StartSell )->Show();
				pWindow1->GetElement<UI::Button>( BUTTON_StopSell )->Show();

				pWindow1->GetElement<UI::Button>( BUTTON_Buy )->Hide();

				pWindow1->GetElement<UI::Text>( TEXT_Gold1 )->SetText( "0" );
				pWindow1->GetElement<UI::Text>( TEXT_Gold2 )->SetText( FormatNumber( PERSONALSHOPHANDLER->GetCanCarryGold() ) );

				iShopType = PERSONALSHOPTYPE_Seller;

				MOUSEHANDLER->BlockMouse();
			}
			else
				TITLEBOX( AY_OBFUSCATE( "You can't set up a Personal Shop, because you're logged in with the Share PW!" ) );
		}
		else
		{
			SetWindowState( WINDOWSTATE_Opening );

			pWindow1->Show();

			pWindow1->GetElement<UI::Button>( BUTTON_StartSell )->Hide();
			pWindow1->GetElement<UI::Button>( BUTTON_StopSell )->Hide();

			pWindow1->GetElement<UI::Button>( BUTTON_Buy )->Show();

			pWindow1->GetElement<UI::Text>( TEXT_Gold1 )->SetText( "0" );
			pWindow1->GetElement<UI::Text>( TEXT_Gold2 )->SetText( FormatNumber( UNITDATA->sCharacterData.iGold ) );

			pWindow1->GetElement<UI::Text>( TEXT_Gold1 )->SetColor( -1 );

			if ( !PERSONALSHOPHANDLER->GetPinnedShop( PERSONALSHOPHANDLER->GetSellerID() ) )
			{
				pWindow1->GetElement<UI::Button>( BUTTON_Buy )->SetSelected( FALSE );
				PERSONALSHOPHANDLER->AddVisitedShop( PERSONALSHOPHANDLER->GetSellerID() );
			}
			else
				pWindow1->GetElement<UI::Button>( BUTTON_Buy )->SetSelected( TRUE );

			iShopType = PERSONALSHOPTYPE_Buyer;

			MOUSEHANDLER->BlockMouse();
		}
	}
	else if ( pWindow1->GetElement<UI::Button>( BUTTON_StartSell )->IsSelected() )
	{
		SetWindowState( WINDOWSTATE_Opening );
		pWindow1->GetElement<UI::Button>( BUTTON_State )->Hide();
	}
}

void CPersonalShopWindowHandler::Close()
{
	if ( pWindow1->IsOpen() )
	{
		SetWindowState( WINDOWSTATE_Closing );

		MOUSEHANDLER->UnblockMouse();

		PERSONALSHOPHANDLER->ClearShop();

		OnButtonStopSellClick( UIEventArgs{} );
	}
}

void CPersonalShopWindowHandler::ClearItems()
{
	pWindow1->GetElement<UI::ItemBox>( ITEMBOX_Shop )->ClearItems();
}

void CPersonalShopWindowHandler::Render()
{
	pWindow1->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

void CPersonalShopWindowHandler::Update( float fTime )
{
	if ( pWindow1->IsOpen() )
	{
		if ( iWindowState != WINDOWSTATE_None )
		{
			fTimeUpdate += fTime;
			
			int iX		= 0;

			if ( iWindowState == WINDOWSTATE_Opening )
				iX = (-344) + (int)((fTimeUpdate * (float)344) / PERSONALSHOP_WINDOW_TIMEUPDATE);
			else
				iX = -(int)((fTimeUpdate * (float)344) / PERSONALSHOP_WINDOW_TIMEUPDATE);

			pWindow1->SetBox( Rectangle2D( iX, 40, PERSONALSHOP_WINDOW_WIDTH, PERSONALSHOP_WINDOW_HEIGHT ) );

			if ( fTimeUpdate >= PERSONALSHOP_WINDOW_TIMEUPDATE )
			{
				if ( iWindowState == WINDOWSTATE_Closing )
				{
					pWindow1->GetElement<UI::Button>( BUTTON_State )->Hide();
					pWindow1->Hide();
				}

				if ( iWindowState == WINDOWSTATE_Minimizing )
					pWindow1->GetElement<UI::Button>( BUTTON_State )->Show();

				SetWindowState( WINDOWSTATE_None );
			}
		}

		if ( iShopType == PERSONALSHOPTYPE_Buyer )
		{
			pWindow1->GetElement<UI::Text>( TEXT_Gold1 )->SetText( "0" );
			pWindow1->GetElement<UI::Text>( TEXT_Gold1 )->SetColor( -1 );

			ItemData * pcItemData = pWindow1->GetElement<UI::ItemBox>( ITEMBOX_Shop )->GetLastRenderedItem();
			if ( pcItemData && pcItemData->bValid )
			{
				if ( auto[bRet, psPrice] = PERSONALSHOPHANDLER->GetPrice( pcItemData->sItem.sItemID.ToInt(), pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 ); bRet )
				{
					if ( psPrice->bCanBuy )
					{
						pWindow1->GetElement<UI::Text>( TEXT_Gold1 )->SetTextEx( "%s %s", FormatNumber( psPrice->iPrice ), psPrice->bCredits ? "cs" : "gp" );
						pWindow1->GetElement<UI::Text>( TEXT_Gold1 )->SetColor( PERSONALSHOPHANDLER->CanUseGoldShop( psPrice ) ? (psPrice->bCredits ? D3DCOLOR_XRGB( 255, 140, 0 ) : -1) : D3DCOLOR_XRGB( 255, 20, 20 ) );
					}
					else
					{
						pWindow1->GetElement<UI::Text>( TEXT_Gold1 )->SetText( "n/a" );
						pWindow1->GetElement<UI::Text>( TEXT_Gold1 )->SetColor( -1 );
					}
				}
			}
		}
	}
}

void CPersonalShopWindowHandler::OnMouseMove( CMouse * pcMouse )
{
	pWindow1->OnMouseMove( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

BOOL CPersonalShopWindowHandler::OnMouseClick( CMouse * pcMouse )
{
	if ( pWindow1->OnMouseClick( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 ) )
		return TRUE;
	if ( pWindow1->IsOpen() )
	{
		if ( iShopType == PERSONALSHOPTYPE_Seller )
		{
			if ( pcMouse->GetEvent() == EMouseEvent::ClickDownR )
				return TRUE;

			int * piaPersonalShopItemIndex = ((int*)0x03653C50);

			ItemData * pcItemData = ITEMHANDLER->GetMouseItemHover();
			if ( pcItemData )
			{
				int iIndex = CGameInventory::GetInstance()->GetInventoryItemIndex( pcItemData );
				if ( (iIndex >= 0) && (piaPersonalShopItemIndex[iIndex] != 0) )
				{
					return TRUE;
				}	
			}

			if ( pcMouse->GetEvent() == EMouseEvent::ClickDownL )
			{
				pcItemData = ITEMHANDLER->GetMouseItem();
				if ( pcItemData->bValid )
					if ( READDWORD( 0x035EB650 ) && (piaPersonalShopItemIndex[READDWORD( 0x035EB650 ) - 1] != 0) )
						return TRUE;

				if ( MESSAGEBOX->IsOpen() )
					return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CPersonalShopWindowHandler::OnKeyChar( CKeyboard * pcKeyboard )
{
	if ( toupper( pcKeyboard->GetChar() ) == 'B' )
	{
		if ( ITEMBOXHANDLER->GetWindow()->IsOpen() == FALSE )
		{
			if ( Game::GetGameMode() == GAMEMODE_InGame )
			{
				if ( (UNITDATA->iLoadedMapIndex >= 0) && (FIELDGAME[UNITDATA->iLoadedMapIndex]->iMapType == MAPTYPE_Town) )
				{
					if ( pWindow1->IsOpen() == FALSE )
					{
						Open( true );

						if ( pWindow1->IsOpen() )
							return TRUE;
					}
				}
			}
		}
	}

	return pWindow1->OnKeyChar( pcKeyboard );
}

void CPersonalShopWindowHandler::AddItem( ItemData * pcItemData, bool bSelf )
{
	if ( bSelf == false )
	{
		ItemData cItemData;
        if ( ITEMHANDLER->LoadItemImage( &pcItemData->sItem, &cItemData ) )
        {
            auto pItemBoxShop = pWindow1->GetElement<UI::ItemBox>( ITEMBOX_Shop );

            cItemData.sPosition = Point2D( pcItemData->sPosition.iX, pcItemData->sPosition.iY );

            pItemBoxShop->ResetLastRender();

            pItemBoxShop->SetNoCheckSlot( TRUE );

            pItemBoxShop->AddItem( &cItemData );

            pItemBoxShop->SetNoCheckSlot( FALSE );
        }
	}
	else
		pWindow1->GetElement<UI::ItemBox>( ITEMBOX_Shop )->AddItem( pcItemData );
}

void CPersonalShopWindowHandler::RemoveItem( int iItemID, int iChk1, int iChk2 )
{
	pWindow1->GetElement<UI::ItemBox>( ITEMBOX_Shop )->RemoveItem( iItemID, iChk1, iChk2 );
}

ItemData * CPersonalShopWindowHandler::GetItem( int iItemID, int iChk1, int iChk2 )
{
	return pWindow1->GetElement<UI::ItemBox>( ITEMBOX_Shop )->GetItemData( iItemID, iChk1, iChk2 );
}

ItemData * CPersonalShopWindowHandler::GetLastRenderedItem()
{
	return pWindow1->GetElement<UI::ItemBox>( ITEMBOX_Shop )->GetLastRenderedItem();
}

std::vector<ItemData*>& CPersonalShopWindowHandler::GetItems()
{
	return pWindow1->GetElement<UI::ItemBox>( ITEMBOX_Shop )->GetItems();
}

void CPersonalShopWindowHandler::BuildWindow()
{
	//Background
	UI::ImageBox_ptr pImageWindow = std::make_shared<UI::ImageBox>( Rectangle2D( 0, 0, 344, 264 ) );
	pImageWindow->SetImage( UI::ImageLoader::LoadImage( "image\\Sinimage\\shopall\\shop-2.bmp" ) );
	pWindow1->AddElement( pImageWindow );

	//Title
	UI::ImageBox_ptr pImageTitle = std::make_shared<UI::ImageBox>( Rectangle2D( 225, 7, 111, 32 ) );
	pImageTitle->SetImage( UI::ImageLoader::LoadImage( "image\\SinImage\\indishop\\indishop_title.bmp" ) );
	pWindow1->AddElement( pImageTitle );

	//Image Shop
	UI::ImageBox_ptr pImageShop = std::make_shared<UI::ImageBox>( Rectangle2D( 238, 50, 53, 59 ) );
	pImageShop->SetImage( UI::ImageLoader::LoadImage( "image\\SinImage\\indishop\\indishop_image.bmp" ) );
	pWindow1->AddElement( pImageShop );

	//Item Box
	UI::ItemBox_ptr pItemBox = std::make_shared<UI::ItemBox>( 19, 45, 9, 9 );
	pItemBox->SetID( ITEMBOX_Shop );
	pItemBox->SetMaxItems( ITEM_PERSONALSHOP_MAX );
	pItemBox->SetKeepItemValid( TRUE );
	pItemBox->SetDisableItemSound( TRUE );
	pItemBox->SetEventBeforeGetItem( UI::Event::Build( std::bind( &CPersonalShopWindowHandler::OnBeforeGetItemEvent, this, std::placeholders::_1 ) ) );
	pItemBox->SetEventOnGetItem( UI::Event::Build( std::bind( &CPersonalShopWindowHandler::OnGetItemEvent, this, std::placeholders::_1 ) ) );
	pItemBox->SetEventBeforePutItem( UI::Event::Build( std::bind( &CPersonalShopWindowHandler::OnBeforePutItemEvent, this, std::placeholders::_1 ) ) );
    pItemBox->SetFreePlace( TRUE );
	pWindow1->AddElement( pItemBox );

	UI::Text_ptr pTextGold1 = std::make_shared<UI::Text>( Rectangle2D( 250, 124, 76, 18 ) );
	pTextGold1->SetID( TEXT_Gold1 );
	pTextGold1->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pTextGold1->SetText( "0" );
	pTextGold1->SetHorizontalAlign( ALIGN_Center );
	pWindow1->AddElement( pTextGold1 );

	UI::Text_ptr pTextGold2 = std::make_shared<UI::Text>( Rectangle2D( 250, 154, 76, 18 ) );
	pTextGold2->SetID( TEXT_Gold2 );
	pTextGold2->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pTextGold2->SetText( "0" );
	pTextGold2->SetHorizontalAlign( ALIGN_Center );
	pWindow1->AddElement( pTextGold2 );

	//Start Sell Button
	UI::Button_ptr pButtonStartSell = std::make_shared<UI::Button>( Rectangle2D( 238, 216, 28, 28 ) );
	pButtonStartSell->SetID( BUTTON_StartSell );
	pButtonStartSell->SetActiveImage( UI::ImageLoader::LoadActiveImage( "Image\\SinImage\\indishop\\indishop_sale_.bmp", "Image\\SinImage\\indishop\\indishop_sale.bmp" ) );
	pButtonStartSell->SetEvent( UI::Event::Build( std::bind( &CPersonalShopWindowHandler::OnButtonStartSellClick, this, std::placeholders::_1 ) ) );
	pWindow1->AddElement( pButtonStartSell );

	UI::Tooltip_ptr pTooltipStartSell = std::make_shared<UI::Tooltip>( Rectangle2D() );
	pTooltipStartSell->Init();
	pTooltipStartSell->SetFont( "Arial", 16, -1 );
	pTooltipStartSell->SetText( "Start Selling" );
	pTooltipStartSell->SetLinkObject( pButtonStartSell );
	pTooltipStartSell->SetPosition( TOOLTIPPOSITION_Top );
	pTooltipStartSell->SetFollowMouse( FALSE );
	pWindow1->AddElement( pTooltipStartSell );

	//Stop Sell Button
	UI::Button_ptr pButtonStopSell = std::make_shared<UI::Button>( Rectangle2D( 269, 216, 28, 28 ) );
	pButtonStopSell->SetID( BUTTON_StopSell );
	pButtonStopSell->SetActiveImage( UI::ImageLoader::LoadActiveImage( "Image\\SinImage\\indishop\\indishop_nosale_.bmp", "Image\\SinImage\\indishop\\indishop_nosale.bmp" ) );
	pButtonStopSell->SetEvent( UI::Event::Build( std::bind( &CPersonalShopWindowHandler::OnButtonStopSellClick, this, std::placeholders::_1 ) ) );
	pWindow1->AddElement( pButtonStopSell );

	UI::Tooltip_ptr pTooltipStopSell = std::make_shared<UI::Tooltip>( Rectangle2D() );
	pTooltipStopSell->Init();
	pTooltipStopSell->SetFont( "Arial", 16, -1 );
	pTooltipStopSell->SetText( "Stop Selling" );
	pTooltipStopSell->SetLinkObject( pButtonStopSell );
	pTooltipStopSell->SetPosition( TOOLTIPPOSITION_Top );
	pTooltipStopSell->SetFollowMouse( FALSE );
	pWindow1->AddElement( pTooltipStopSell );

	//Buy Button
	UI::Button_ptr pButtonBuy = std::make_shared<UI::Button>( Rectangle2D( 253, 216, 28, 28 ) );
	pButtonBuy->SetID( BUTTON_Buy );
	pButtonBuy->SetActiveImage( UI::ImageLoader::LoadActiveImage( "Image\\SinImage\\indishop\\indishop_exp_.bmp", "Image\\SinImage\\indishop\\indishop_exp.bmp" ) );
	pButtonBuy->SetEvent( UI::Event::Build( std::bind( &CPersonalShopWindowHandler::OnButtonBuyClick, this, std::placeholders::_1 ) ) );
	pWindow1->AddElement( pButtonBuy );

	UI::Tooltip_ptr pTooltipBuy = std::make_shared<UI::Tooltip>( Rectangle2D() );
	pTooltipBuy->Init();
	pTooltipBuy->SetFont( "Arial", 16, -1 );
	pTooltipBuy->SetText( "Pin Shop" );
	pTooltipBuy->SetLinkObject( pButtonBuy );
	pTooltipBuy->SetPosition( TOOLTIPPOSITION_Top );
	pTooltipBuy->SetFollowMouse( FALSE );
	pWindow1->AddElement( pTooltipBuy );


	//Close Button
	UI::Button_ptr pButtonClose = std::make_shared<UI::Button>( Rectangle2D( 304, 223, 20, 20 ) );
	pButtonClose->SetID( BUTTON_Close );
	pButtonClose->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\misc\\btnclose.png", "game\\images\\misc\\btnclose_.png" ) );
	pButtonClose->SetEvent( UI::Event::Build( std::bind( &CPersonalShopWindowHandler::OnButtonCloseClick, this, std::placeholders::_1 ) ) );
	pWindow1->AddElement( pButtonClose );

	//Hidden Shop Button
	UI::Button_ptr pButtonState = std::make_shared<UI::Button>( Rectangle2D( 370, PERSONALSHOP_WINDOW_HEIGHT - 60, 60, 60 ) );
	pButtonState->SetID( BUTTON_State );
	pButtonState->SetActiveImage( UI::ImageLoader::LoadActiveImage( "image\\Sinimage\\shopall\\MyShopButton.tga", "image\\Sinimage\\shopall\\MyShopButton.tga" ) );
	pButtonState->SetAlphaHover( 215 );
	pButtonState->SetEvent( UI::Event::Build( std::bind( &CPersonalShopWindowHandler::OnButtonStateClick, this, std::placeholders::_1 ) ) );
	pButtonState->Hide();
	pWindow1->AddElement( pButtonState );

	pWindow1->Hide();
}

void CPersonalShopWindowHandler::OnButtonStateClick( UIEventArgs eArgs )
{
	if ( pWindow1->GetElement<UI::Button>( BUTTON_StartSell )->IsSelected() )
	{
		SetWindowState( WINDOWSTATE_Opening );
		pWindow1->GetElement<UI::Button>( BUTTON_State )->Hide();
	}
}

void CPersonalShopWindowHandler::OnButtonCloseClick( UIEventArgs eArgs )
{
	if ( MESSAGEBOX->IsOpen() )
		return;

	if ( pWindow1->GetElement<UI::Button>( BUTTON_StartSell )->IsSelected() == FALSE )
		Close();
	else
	{
		SetWindowState( WINDOWSTATE_Minimizing );
	}
}

void CPersonalShopWindowHandler::OnButtonStartSellClick( UIEventArgs eArgs )
{
	if ( MESSAGEBOX->IsOpen() == FALSE )
	{
		if ( iShopType == PERSONALSHOPTYPE_Seller )
		{
			if ( pWindow1->GetElement<UI::Button>( BUTTON_StartSell )->IsSelected() == FALSE )
			{
				if ( GetItems().size() > 0 )
				{
					MESSAGEBOX->ClearExtraElements();
					MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_PersonalShopMessage );
					MESSAGEBOX->SetTitle( "START SELLING" );
					MESSAGEBOX->SetDescription( "Set a message about your personal shop:" );
					MESSAGEBOX->SetPersonalShopMessageLength( 128 - (STRLEN( UNITDATA->sCharacterData.szName ) + 2) );
					MESSAGEBOX->SetEvent( std::bind( &CPersonalShopWindowHandler::OnAcceptStartSelling, this, std::placeholders::_1 ) );
					MESSAGEBOX->Show();
				}
				else
					TITLEBOX( "You need to put an item to start a new personal shop!" );
			}
		}
	}
}

void CPersonalShopWindowHandler::OnButtonStopSellClick( UIEventArgs eArgs )
{
	PacketChatBoxMessage sPacket;
	sPacket.szChatBoxMessage[0] = 0;
	UNITDATA->szPersonalShop[0] = 0;

	UNITDATA->iPersonalShop = 0;
	sPacket.iChatColor		= (EChatColor)UNITDATA->iPersonalShop;

	PERSONALSHOPHANDLER->ProcessPacket( &sPacket );

	pWindow1->GetElement<UI::Button>( BUTTON_StartSell )->SetSelected( FALSE );
}

void CPersonalShopWindowHandler::OnButtonBuyClick( UIEventArgs eArgs )
{
	if ( PERSONALSHOPHANDLER->GetSellerID() )
	{
		if ( !PERSONALSHOPHANDLER->GetPinnedShop( PERSONALSHOPHANDLER->GetSellerID() ) )
		{
			PERSONALSHOPHANDLER->RemoveVisitedShop( PERSONALSHOPHANDLER->GetSellerID() );
			PERSONALSHOPHANDLER->AddPinnedShop( PERSONALSHOPHANDLER->GetSellerID() );

			pWindow1->GetElement<UI::Button>( BUTTON_Buy )->SetSelected( TRUE );
		}
		else
		{
			PERSONALSHOPHANDLER->RemovePinnedShop( PERSONALSHOPHANDLER->GetSellerID() );
			PERSONALSHOPHANDLER->AddVisitedShop( PERSONALSHOPHANDLER->GetSellerID() );

			pWindow1->GetElement<UI::Button>( BUTTON_Buy )->SetSelected( FALSE );
		}
	}
}

void CPersonalShopWindowHandler::SetWindowState( EWindowState iState )
{
	iWindowState	= iState;
	fTimeUpdate		= 0.0f;
}

void CPersonalShopWindowHandler::OnBeforePutItemEvent( UIEventArgs eArgs )
{
	if ( iShopType == PERSONALSHOPTYPE_Buyer )
		return;

	if ( MESSAGEBOX->IsOpen() )
		return;

	if ( pWindow1->GetElement<UI::Button>( BUTTON_StartSell )->IsSelected() )
		return;

	ItemData * pcItemData = pWindow1->GetElement<UI::ItemBox>( ITEMBOX_Shop )->GetEventItemData();
	if ( pcItemData && pcItemData->bValid )
	{
		if ( pcItemData->iItemSlot > 0 )
			return;

		int * piaPersonalShopItemIndex = ((int*)0x03653C50);
	
		int iIndex = CGameInventory::GetInstance()->GetInventoryItemIndex( pcItemData );
		if ( ((iIndex >= 0) && (piaPersonalShopItemIndex[iIndex] == 0)) || (pcItemData == ITEMHANDLER->GetMouseItem())  )
		{
			if ( ITEMWINDOWHANDLER->IsPersonalShopUse( pcItemData ) )
			{
				int iPriceMax = PERSONALSHOPHANDLER->GetCanCarryGold();
				if ( pcItemData->sItem.sItemID.ToItemBase() == ITEMBASE_Potion )
					if ( pcItemData->sItem.iPotionCount > 0 )
						iPriceMax /= pcItemData->sItem.iPotionCount;

				if ( iPriceMax > 0 )
				{
					pcItemData->bValid = FALSE;

					UI::ImageBox_ptr pImageBoxItem = std::make_shared<UI::ImageBox>( Rectangle2D( 30, 30, 0, 0 ) );
					pImageBoxItem->SetImage( UI::ImageLoader::LoadImage( FormatString( "image\\sinimage\\items\\%s\\it%s.bmp", pcItemData->szCategory, pcItemData->szInventoryName ) ) );
					pImageBoxItem->SetBox( Rectangle2D( 64 - (pImageBoxItem->GetImage()->GetWidth() >> 1), 74 - (pImageBoxItem->GetImage()->GetHeight() >> 1), pImageBoxItem->GetImage()->GetWidth(), pImageBoxItem->GetImage()->GetHeight() ) );

					MESSAGEBOX->ClearExtraElements();
					MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_PersonalShop );
					MESSAGEBOX->SetTitle( "SELL ITEM" );
					MESSAGEBOX->SetDescription( "Price:" );
					MESSAGEBOX->AddExtraText( pcItemData->sItem.szItemName, Rectangle2D( 25, 118, 76, 24 ), TRUE, UI::Color( 255, 255, 255, 255 ) );
					MESSAGEBOX->AddExtraElement( pImageBoxItem );
					MESSAGEBOX->SetItemPriceMax( iPriceMax > 0 ? 1 : 0, iPriceMax );
					MESSAGEBOX->SetEvent( std::bind( &CPersonalShopWindowHandler::OnAcceptSellItem, this, pcItemData, std::placeholders::_1 ) );
					MESSAGEBOX->Show();

					bIsSellingTime = TRUE;
				}
				else
					TITLEBOX( "You cannot carry more items on personal shop!" );
			}
			else
				TITLEBOX( "You can't sell this item" );
		}
	}
}

void CPersonalShopWindowHandler::OnBeforeGetItemEvent( UIEventArgs eArgs )
{
	if ( iShopType == PERSONALSHOPTYPE_Seller )
	{
		if ( pWindow1->GetElement<UI::Button>( BUTTON_StartSell )->IsSelected() == FALSE )
		{
			MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
			MESSAGEBOX->SetTitle( "STOP SELLING" );
			MESSAGEBOX->SetDescription( "Do you want to stop selling this item?" );
			MESSAGEBOX->SetEvent( std::bind( &CPersonalShopWindowHandler::OnAcceptGetItem, this, std::placeholders::_1 ) );
			MESSAGEBOX->Show();
		}
	}
	else
	{
		ItemData * pcItemData = pWindow1->GetElement<UI::ItemBox>( ITEMBOX_Shop )->GetEventItemData();
		if ( pcItemData && pcItemData->bValid )
		{
			if ( auto[bRet, psPrice] = PERSONALSHOPHANDLER->GetPrice( pcItemData->sItem.sItemID.ToInt(), pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 ); bRet )
			{
				if ( psPrice->bCanBuy )
				{
					if ( psPrice->iPrice > 0 )
					{
						//Check if have Gold
						if ( PERSONALSHOPHANDLER->CanUseGoldShop( psPrice ) )
						{
							UI::ImageBox_ptr pImageBoxItem = std::make_shared<UI::ImageBox>( Rectangle2D( 30, 30, 0, 0 ) );
							pImageBoxItem->SetImage( UI::ImageLoader::LoadImage( FormatString( "image\\sinimage\\items\\%s\\it%s.bmp", pcItemData->szCategory, pcItemData->szInventoryName ) ) );
							pImageBoxItem->SetBox( Rectangle2D( 64 - (pImageBoxItem->GetImage()->GetWidth() >> 1), 74 - (pImageBoxItem->GetImage()->GetHeight() >> 1), pImageBoxItem->GetImage()->GetWidth(), pImageBoxItem->GetImage()->GetHeight() ) );

							MESSAGEBOX->ClearExtraElements();
							MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_PersonalShopBuy );
							MESSAGEBOX->SetTitle( "BUY ITEM" );
							MESSAGEBOX->SetDescription( FormatString( "Price (in %s):", psPrice->bCredits ? "coin" : "gold" ) );
							MESSAGEBOX->AddExtraText( pcItemData->sItem.szItemName, Rectangle2D( 25, 118, 76, 24 ), TRUE, UI::Color( 255, 255, 255, 255 ) );
							MESSAGEBOX->AddExtraElement( pImageBoxItem );
							MESSAGEBOX->SetItemPriceMax( psPrice->iPrice );
							MESSAGEBOX->SetItemCounter( pcItemData->sItem.sItemID.ToItemBase() == ITEMBASE_Potion ? pcItemData->sItem.iPotionCount : 1 );
							MESSAGEBOX->SetEvent( std::bind( &CPersonalShopWindowHandler::OnAcceptBuyItem, this, pcItemData, std::placeholders::_1 ) );
							MESSAGEBOX->Show();
						}
						else
						{
							if ( psPrice->bCredits )
								TITLEBOX( AY_OBFUSCATE( "Not enough coins" ) );
							else
								TITLEBOX( AY_OBFUSCATE( "Not enough gold" ) );
						}
					}
					else
						TITLEBOX( AY_OBFUSCATE( "Not enough gold" ) );
				}
			}
		}
	}
}

void CPersonalShopWindowHandler::OnGetItemEvent( UIEventArgs eArgs )
{
	ItemData * pcItemData = pWindow1->GetElement<UI::ItemBox>( ITEMBOX_Shop )->GetEventItemData();
	if ( pcItemData )
	{
		int * piaPersonalShopItemIndex = ((int*)0x03653C50);

		int iIndex = CGameInventory::GetInstance()->GetInventoryItemIndex( pcItemData );
		if ( iIndex >= 0 )
		{
			piaPersonalShopItemIndex[iIndex] = 0;
			PERSONALSHOPHANDLER->RemovePrice( pcItemData->sItem.sItemID.ToInt(), pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 );
		}
	}
}

void CPersonalShopWindowHandler::OnAcceptSellItem( ItemData * pcItemData, UIEventArgs eArgs )
{
	bIsSellingTime = FALSE;

	if ( MESSAGEBOX->GetType() )
	{
		if ( CHARACTERGAME->CheckInventorySpace( pcItemData ) )
		{
			pcItemData->bValid = TRUE;

			if ( MESSAGEBOX->GetPriceItem() > 0 )
			{
				if ( pWindow1->GetElement<UI::ItemBox>( ITEMBOX_Shop )->AddItem( pcItemData ) )
				{
					PricePersonalShopData sPriceData;
					sPriceData.sItemID.SetItemID( pcItemData->sItem.sItemID.ToItemID() );
					sPriceData.iChk1 = pcItemData->sItem.iChk1;
					sPriceData.iChk2 = pcItemData->sItem.iChk2;
					sPriceData.iPrice = MESSAGEBOX->GetPriceItem();
					sPriceData.bCredits = MESSAGEBOX->IsSellByCreditsItemPersonalShop() ? true : false;
					sPriceData.iCount = pcItemData->sItem.sItemID.ToItemBase() == ITEMBASE_Potion ? pcItemData->sItem.iPotionCount : 1;
					sPriceData.bCanBuy = MESSAGEBOX->CanBuyItemPersonalShop() ? true : false;
					PERSONALSHOPHANDLER->AddPrice( &sPriceData );

					pWindow1->GetElement<UI::Text>( TEXT_Gold1 )->SetText( FormatNumber( PERSONALSHOPHANDLER->GetGoldSelling() ) );

					int iIndex = CGameInventory::GetInstance()->GetInventoryItemIndex( pcItemData );
					if ( iIndex >= 0 )
					{
						int * piaPersonalShopItemIndex = ((int *)0x03653C50);
						piaPersonalShopItemIndex[iIndex] = 1;
					}
				}
			}
			else
				TITLEBOX( AY_OBFUSCATE( "You need to set a price on this item!" ) );
		}
		else
		{
			pcItemData->bValid = TRUE;

			TITLEBOX( AY_OBFUSCATE( "Not enough space in inventory" ) );
		}
	}
	else
		pcItemData->bValid = TRUE;
}

void CPersonalShopWindowHandler::OnAcceptGetItem( UIEventArgs eArgs )
{
	if ( MESSAGEBOX->GetType() )
		pWindow1->GetElement<UI::ItemBox>( ITEMBOX_Shop )->GetLastItemOnMouse( TRUE );
}

void CPersonalShopWindowHandler::OnAcceptBuyItem( ItemData * pcItemData, UIEventArgs eArgs )
{
	if ( MESSAGEBOX->GetType() )
	{
		if ( auto[bRet, psPrice] = PERSONALSHOPHANDLER->GetPrice( pcItemData->sItem.sItemID.ToInt(), pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 ); bRet )
		{
			if ( psPrice->bCanBuy )
			{
				if ( !CHARACTERGAME->CheckInventorySpace( pcItemData ) )
				{
					TITLEBOX( "Not enough space in inventory" );
					return;
				}

				if ( (dwTimeProcessBuy + 1500) > TICKCOUNT )
				{
					TITLEBOX( "Wait for transaction time process!" );
					return;
				}

				dwTimeProcessBuy = TICKCOUNT;

				if ( PERSONALSHOPHANDLER->CanUseGoldShop( psPrice ) )
				{
					PacketPersonalShopTradeItems sPacket;
					CopyMemory( &sPacket.cItemData, pcItemData, sizeof( ItemData ) );
					CopyMemory( &sPacket.sItemPrice, psPrice, sizeof( PricePersonalShopData ) );

					sPacket.iItemCount = MESSAGEBOX->GetCountItem();

					sPacket.iSellerID = PERSONALSHOPHANDLER->GetSellerID();
					sPacket.iBuyerID = UNITDATA->iID;
					sPacket.bBuy = true;

					PERSONALSHOPHANDLER->SetLastBoughtItem( pcItemData );

					PERSONALSHOPHANDLER->ProcessPacket( &sPacket, false );
				}
				else
				{
					if ( psPrice->bCredits )
						TITLEBOX( AY_OBFUSCATE( "Not enough coins" ) );
					else
						TITLEBOX( AY_OBFUSCATE( "Not enough gold" ) );
				}
			}
		}
	}
}

void CPersonalShopWindowHandler::OnAcceptStartSelling( UIEventArgs eArgs )
{
	if ( MESSAGEBOX->GetType() )
	{
		if ( MESSAGEBOX->GetPersonalShopMessage().length() > 0 )
		{
			PacketChatBoxMessage sPacket;
			STRINGCOPY( sPacket.szChatBoxMessage, MESSAGEBOX->GetPersonalShopMessage().c_str() );
			STRINGFORMAT( UNITDATA->szPersonalShop, "%s: %s", UNITDATA->sCharacterData.szName, sPacket.szChatBoxMessage );

			UNITDATA->iPersonalShop = RandomI( 200, 3000 );
			sPacket.iChatColor		= (EChatColor)UNITDATA->iPersonalShop;

			PERSONALSHOPHANDLER->ProcessPacket( &sPacket );

			pWindow1->GetElement<UI::Button>( BUTTON_StartSell )->SetSelected( TRUE );
		}
		else
			TITLEBOX( "Fill the message on personal shop" );
	}
}
