#include "stdafx.h"
#include "CoinShopWindow.h"


void CCoinShopWindow::AddString( int iSide, const std::string & strString, DWORD dwColor, BOOL bBold )
{
	std::vector<std::string> vString;

	//Find breakline in String 
	BOOL bMultiLine = FALSE;
	if( strString.find( '\n' ) != std::string::npos )
	{
		vString = split( strString, '\n' );
		bMultiLine = TRUE;
	}

	switch( iSide )
	{
	case 0: //Left Side
		//Is Multiline?
		if( bMultiLine )
		{
			//String from vector
			int i = 0;
			for( const auto & strSplitted : vString )
			{
				UI::Text_ptr pText = std::make_shared<UI::Text>( strSplitted, Rectangle2D( 0, 0, 180, 20 ), bBold, dwColor );
				pText->SetFont( "Arial", 14, 0, FALSE, bBold, dwColor );
				vLeftText.push_back( pText );
				i++;

				if( i != vString.size() )
				{
					UI::Text_ptr pText = std::make_shared<UI::Text>( "", Rectangle2D( 0, 0, 180, 20 ), bBold, dwColor );
					pText->SetFont( "Arial", 14, 0, FALSE, bBold, dwColor );
					vLeftText.push_back( pText );
				}
			}
		}
		else
		{
			UI::Text_ptr pText = std::make_shared<UI::Text>( strString, Rectangle2D( 0, 0, 180, 20 ), bBold, dwColor );
			pText->SetFont( "Arial", 14, 0, FALSE, bBold, dwColor );
			vLeftText.push_back( pText );
		}

		break;

	case 1: //Right Side
		UI::Text_ptr pText = std::make_shared<UI::Text>( strString, Rectangle2D( 0, 0, 180, 20 ), bBold, dwColor );
		pText->SetFont( "Arial", 14, 0, FALSE, bBold, dwColor );
		vRightText.push_back( pText );
		break;
	}
}

BOOL CCoinShopWindow::CheckSlots()
{
	auto pList = pWindow1->GetElement<UI::List>( LISTID_Items );

	UI::Window_ptr pWindowItem = pWindow1->GetElement<UI::Window>( WINDOWID_ItemData );

	UI::ItemBox_ptr pItemBox = pWindowItem->GetElement<UI::ItemBox>( ITEMBOX_Perfectize );

	if ( pItemBox && pItemBox->GetItems().size() > 0 )
	{
		TITLEBOX( "Remove item of Perfectize slot first!" );
		return FALSE;
	}

	pItemBox = pWindowItem->GetElement<UI::ItemBox>( ITEMBOX_Swapper );

	if ( pItemBox && pItemBox->GetItems().size() > 0 )
	{
		TITLEBOX( "Remove item of Item Swap slot first!" );
		return FALSE;
	}

	return TRUE;
}

void CCoinShopWindow::OnBuyCredits( UIEventArgs eArgs )
{
	if ( MESSAGEBOX->GetType() )
	{
		ShellExecute( 0, 0, "https://user.regnumpristontale.com//userpanel/", 0, 0, SW_SHOW );
		TITLEBOX( "Click on \"Donation\" after login on user panel!" );
		ChatGame::AddChatBoxText( "> Click on \"Donation\" after login on user panel!" );
	}

	pWindow1->Show();
}

void CCoinShopWindow::OnBuyAgingRecovery( int iPrice, UIEventArgs eArgs )
{
	if ( MESSAGEBOX->GetType() )
	{
		if ( ACCOUNTHANDLER->GetCoinAmount() >= iPrice )
		{
			// Check iWeight
			if ( (UNITDATA->sCharacterData.sWeight.sCur + 200) > UNITDATA->sCharacterData.sWeight.sMax )
			{
				TITLEBOX( "Weight limit exceeded" );
				return;
			}

			// Check Space
			if ( !CHARACTERGAME->CheckInventorySpace( ITEMHANDLER->FindItemTableByCode( ITEMID_BrazilianSoccer ) ) )
			{
				TITLEBOX( "Not enough space in inventory" );
				return;
			}

			PacketCoinServiceBuy s;
			s.AgingRecovery.iID = iItemAgingRecoveryID;
			s.iType = ITEMCOIN_AgingRecovery;
			COINHANDLER->ProcessPacket( &s );
		}
		else
		{
			TITLEBOX( "Not enough credits" );
			ChatGame::AddChatBoxText( "> Not enough credits!", CHATCOLOR_Global );
		}
	}
}

void CCoinShopWindow::OnItemNotAllowedPerfectize( UIEventArgs eArgs )
{
	TITLEBOX( "This item cannot be Perfect!" );
}

void CCoinShopWindow::OnItemNotAllowedSwapper( UIEventArgs eArgs )
{
	UI::Window_ptr pWindowItem = pWindow1->GetElement<UI::Window>( WINDOWID_ItemData );

	if ( pWindowItem )
	{
		//Get Elements
		UI::ItemBox_ptr pItemBox = pWindowItem->GetElement<UI::ItemBox>( ITEMBOX_Swapper );

		if ( pItemBox )
		{
			ItemData * pcItemData = pItemBox->GetEventItemData();

			if ( pcItemData )
			{
				if ( pcItemData->sItem.bPerfectItem == FALSE )
					TITLEBOX( "Item must be Perfect!" );
				else
					TITLEBOX( "This item is not allowed!" );
			}
		}
	}
}

void CCoinShopWindow::OnPutItemPerfectize( UIEventArgs eArgs )
{
	UI::Window_ptr pWindowItem = pWindow1->GetElement<UI::Window>( WINDOWID_ItemData );

	if ( pWindowItem )
	{
		//Get Elements
		UI::ItemBox_ptr pItemBox = pWindowItem->GetElement<UI::ItemBox>( ITEMBOX_Perfectize );
		UI::Text_ptr pText = pWindowItem->GetElement<UI::Text>( LABELID_Price );

		if ( pItemBox && pItemBox->GetItems().size() > 0 && pText )
		{
			ItemData * pcItemData = pItemBox->GetItems()[0];

			int iCoins = 0;

			//Set Price
			iCoins = Base::Coin::GetPerfectizePrice( &pcItemData->sItem );

			//Global Discount
			if ( COINHANDLER->GetCoinShop()->GetDiscount() > 0 )
				iCoins = (COINHANDLER->GetCoinShop()->GetDiscount() * iCoins) / 100;

			pText->SetText( FormatNumber( iCoins ) );

			//Log to Server
			ITEMHANDLER->ProcessItemSlotAction( ESlotTypeHandle::ITEMSLOTHANDLE_Put, ESlotTypeHandleWhere::ITEMSLOTHANDLEWHERE_Perfectize,
				pcItemData->sItem.sItemID.ToInt(), pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 );
		}
	}
}

void CCoinShopWindow::OnPutItemSwapper( UIEventArgs eArgs )
{
	UI::Window_ptr pWindowItem = pWindow1->GetElement<UI::Window>( WINDOWID_ItemData );

	if ( pWindowItem )
	{
		//Get Elements
		UI::ItemBox_ptr pItemBox = pWindowItem->GetElement<UI::ItemBox>( ITEMBOX_Swapper );
		UI::Text_ptr pText = pWindowItem->GetElement<UI::Text>( LABELID_Price );

		if ( pItemBox && pItemBox->GetItems().size() > 0 && pText )
		{
			ItemData * pcItemData = pItemBox->GetItems()[0];

			int iCoins = 0;

			//Set Price
			iCoins = Base::Coin::GetSwapperPrice( &pcItemData->sItem, iSelectedSwap );

			//Global Discount
			if ( COINHANDLER->GetCoinShop()->GetDiscount() > 0 )
				iCoins = (COINHANDLER->GetCoinShop()->GetDiscount() * iCoins) / 100;

			pText->SetText( FormatNumber( iCoins ) );

			//Log to Server
			ITEMHANDLER->ProcessItemSlotAction( ESlotTypeHandle::ITEMSLOTHANDLE_Put, ESlotTypeHandleWhere::ITEMSLOTHANDLEWHERE_Swap,
				pcItemData->sItem.sItemID.ToInt(), pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 );
		}
	}
}

void CCoinShopWindow::OnGetItemPerfectize( UIEventArgs eArgs )
{
	UI::Window_ptr pWindowItem = pWindow1->GetElement<UI::Window>( WINDOWID_ItemData );

	if ( pWindowItem )
	{
		//Get Elements
		UI::Text_ptr pText = pWindowItem->GetElement<UI::Text>( LABELID_Price );

		if ( pText )
		{
			int iCoins = 0;

			pText->SetText( FormatNumber( iCoins ) );

			UI::ItemBox_ptr pItemBox = pWindowItem->GetElement<UI::ItemBox>( ITEMBOX_Swapper );

			if ( pItemBox )
			{
				ItemData * pcItemData = pItemBox->GetEventItemData();

				//Log to Server
				ITEMHANDLER->ProcessItemSlotAction( ESlotTypeHandle::ITEMSLOTHANDLE_Get, ESlotTypeHandleWhere::ITEMSLOTHANDLEWHERE_Perfectize,
					pcItemData->sItem.sItemID.ToInt(), pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 );
			}
		}
	}
}

void CCoinShopWindow::OnGetItemSwapper( UIEventArgs eArgs )
{
	UI::Window_ptr pWindowItem = pWindow1->GetElement<UI::Window>( WINDOWID_ItemData );

	if ( pWindowItem )
	{
		//Get Elements
		UI::Text_ptr pText = pWindowItem->GetElement<UI::Text>( LABELID_Price );

		if ( pText )
		{
			int iCoins = 0;

			pText->SetText( FormatNumber( iCoins ) );

			UI::ItemBox_ptr pItemBox = pWindowItem->GetElement<UI::ItemBox>( ITEMBOX_Swapper );

			if ( pItemBox )
			{
				ItemData * pcItemData = pItemBox->GetEventItemData();

				//Log to Server
				ITEMHANDLER->ProcessItemSlotAction( ESlotTypeHandle::ITEMSLOTHANDLE_Get, ESlotTypeHandleWhere::ITEMSLOTHANDLEWHERE_Swap,
					pcItemData->sItem.sItemID.ToInt(), pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 );
			}
		}
	}
}

void CCoinShopWindow::ClearString()
{
	vLeftText.clear();
	vRightText.clear();
}

void CCoinShopWindow::SetItemStatus( struct DefinitionItem * psItemDefinition, char * pszName, char * pszDescription )
{
	ClearString();

	bool bBaseStatus, bSpecStatus, bReqStatus = false;

	if( psItemDefinition->IntegrityMin || psItemDefinition->AttackPower1Max || psItemDefinition->AddHPMin || psItemDefinition->HPRegenMin )
		bBaseStatus = true;

	if( psItemDefinition->AddSpecDefenseMax || psItemDefinition->sItem.sSpecData.iSpecCritical || psItemDefinition->AddSpecAbsorbMax || psItemDefinition->sItem.sSpecData.fSpecHPRegen )
		bSpecStatus = true;

	if( psItemDefinition->sItem.iLevel )
		bReqStatus = true;

	if( bBaseStatus )
	{
		AddString( 0, "Base Status", D3DCOLOR_ARGB( 255, 236, 236, 236 ), TRUE );
		AddString( 1, "" );
	}

	if ( psItemDefinition->AttackPower1Max )
	{
		AddString( 0, "Attack Power", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
		AddString( 1, FormatString( "%d - %d", psItemDefinition->AttackPower2Min, psItemDefinition->AttackPower2Max ) );
	}
	if ( psItemDefinition->IntegrityMax )
	{
		AddString( 0, "Integrity", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
		AddString( 1, FormatString( "%d/%d", (int)psItemDefinition->IntegrityMax, (int)psItemDefinition->IntegrityMax ) );
	}
	if ( psItemDefinition->sItem.iAttackSpeed )
	{
		AddString( 0, "Attack Speed", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
		AddString( 1, FormatString( "%d", psItemDefinition->sItem.iAttackSpeed ) );
	}
	if ( psItemDefinition->sItem.iAttackRange )
	{
		AddString( 0, "Attack Range", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
		AddString( 1, FormatString( "%d", psItemDefinition->sItem.iAttackRange ) );
	}
	if ( psItemDefinition->AttackRatingMax )
	{
		AddString( 0, "Attack Rating", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
		AddString( 1, FormatString( "%d", psItemDefinition->AttackRatingMax ) );
	}
	if ( psItemDefinition->sItem.iCritical )
	{
		AddString( 0, "Critical", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
		AddString( 1, FormatString( "%d", psItemDefinition->sItem.iCritical ) );
	}
	if ( psItemDefinition->sItem.saElementalDef[0] )
	{
		AddString( 0, "Organic", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
		AddString( 1, FormatString( "%d", psItemDefinition->sItem.saElementalDef[0] ) );
	}
	if ( psItemDefinition->sItem.saElementalDef[2] )
	{
		AddString( 0, "Fire", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
		AddString( 1, FormatString( "%d", psItemDefinition->sItem.saElementalDef[2] ) );
	}
	if ( psItemDefinition->sItem.saElementalDef[3] )
	{
		AddString( 0, "Frost", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
		AddString( 1, FormatString( "%d", psItemDefinition->sItem.saElementalDef[3] ) );
	}
	if ( psItemDefinition->sItem.saElementalDef[4] )
	{
		AddString( 0, "Lightning", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
		AddString( 1, FormatString( "%d", psItemDefinition->sItem.saElementalDef[4] ) );
	}
	if ( psItemDefinition->sItem.saElementalDef[5] )
	{
		AddString( 0, "Poison", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
		AddString( 1, FormatString( "%d", psItemDefinition->sItem.saElementalDef[5] ) );
	}
	if ( psItemDefinition->AbsorbMax )
	{
		AddString( 0, "Absorb", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
		AddString( 1, FormatString( "%.1f", psItemDefinition->AbsorbMax ) );
	}
	if ( psItemDefinition->DefenseMax )
	{
		AddString( 0, "Defense", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
		AddString( 1, FormatString( "%d", (int)psItemDefinition->DefenseMax ) );
	}
	if ( psItemDefinition->BlockRatingMax )
	{
		AddString( 0, "Block Rating", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
		AddString( 1, FormatString( "%.1f", psItemDefinition->BlockRatingMax ) );
	}
	if ( psItemDefinition->HPRegenMax )
	{
		AddString( 0, "HP Regen", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
		AddString( 1, FormatString( "%.1f", psItemDefinition->HPRegenMax ) );
	}
	if ( psItemDefinition->MPRegenMax )
	{
		AddString( 0, "MP Regen", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
		AddString( 1, FormatString( "%.1f", psItemDefinition->MPRegenMax ) );
	}
	if ( psItemDefinition->STMRegenMax )
	{
		AddString( 0, "STM Regen", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
		AddString( 1, FormatString( "%.1f", psItemDefinition->STMRegenMax ) );
	}
	if ( psItemDefinition->AddHPMax )
	{
		AddString( 0, "Add HP", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
		AddString( 1, FormatString( "%d", psItemDefinition->AddHPMax ) );
	}
	if ( psItemDefinition->AddMPMax )
	{
		AddString( 0, "Add MP", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
		AddString( 1, FormatString( "%d", psItemDefinition->AddMPMax ) );
	}
	if ( psItemDefinition->AddSTMMax )
	{
		AddString( 0, "Add STM", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
		AddString( 1, FormatString( "%d", psItemDefinition->AddSTMMax ) );
	}
	if ( psItemDefinition->sItem.iPotionStorage )
	{
		AddString( 0, "Potion Count", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
		AddString( 1, FormatString( "%d", psItemDefinition->sItem.iPotionStorage ) );
	}
	if ( psItemDefinition->sItem.fMovementSpeed )
	{
		AddString( 0, "Speed", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
		AddString( 1, FormatString( "%.1f", psItemDefinition->sItem.fMovementSpeed ) );
	}

	if( bBaseStatus )
	{
		AddString( 0, "" );
		AddString( 1, "" );
	}

	if( bSpecStatus )
	{
		AddString( 0, "Spec Status", D3DCOLOR_ARGB( 255, 236, 236, 236 ), TRUE );
		AddString( 1, "" );
	}

	if ( psItemDefinition->sItem.sSpecData.iSpecCritical )
	{
		AddString( 0, "Critical", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
		AddString( 1, FormatString( "%d", psItemDefinition->sItem.sSpecData.iSpecCritical ) );
	}
	if ( psItemDefinition->sItem.sSpecData.iSpecAttackRange )
	{
		AddString( 0, "Attack Range", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
		AddString( 1, FormatString( "%d", psItemDefinition->sItem.sSpecData.iSpecAttackRange ) );
	}
	if ( psItemDefinition->sItem.sSpecData.iSpecAttackSpeed )
	{
		AddString( 0, "Attack Speed", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
		AddString( 1, FormatString( "%d", psItemDefinition->sItem.sSpecData.iSpecAttackSpeed ) );
	}
	if ( psItemDefinition->sItem.sSpecData.iSpecAttackRatingDiv )
	{
		AddString( 0, "Attack Rating", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
		AddString( 1, FormatString( "LV/%d", psItemDefinition->sItem.sSpecData.iSpecAttackRatingDiv ) );
	}
	if ( psItemDefinition->sItem.sSpecData.iSpecAttackPowerDiv )
	{
		AddString( 0, "Attack Power", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
		AddString( 1, FormatString( "LV/%d", (int)psItemDefinition->sItem.sSpecData.iSpecAttackPowerDiv ) );
	}
	if ( psItemDefinition->AddSpecAbsorbMax )
	{
		AddString( 0, "Absorption", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
		AddString( 1, FormatString( "%.1f", psItemDefinition->AddSpecAbsorbMax ) );
	}
	if ( psItemDefinition->AddSpecDefenseMax )
	{
		AddString( 0, "Defense", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
		AddString( 1, FormatString( "%d", (int)psItemDefinition->AddSpecDefenseMax ) );
	}
	if ( psItemDefinition->sItem.sSpecData.fSpecHPRegen )
	{
		AddString( 0, "HP Regen", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
		AddString( 1, FormatString( "%.1f", psItemDefinition->sItem.sSpecData.fSpecHPRegen ) );
	}
	if ( psItemDefinition->sItem.sSpecData.fSpecMPRegen )
	{
		AddString( 0, "MP Regen", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
		AddString( 1, FormatString( "%.1f", psItemDefinition->sItem.sSpecData.fSpecMPRegen ) );
	}
	if ( psItemDefinition->sItem.sSpecData.fSpecSPRegen )
	{
		AddString( 0, "STM Regen", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
		AddString( 1, FormatString( "%.1f", psItemDefinition->sItem.sSpecData.fSpecSPRegen ) );
	}
	if ( psItemDefinition->AddSpecRunSpeedMax )
	{
		AddString( 0, "Speed", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
		AddString( 1, FormatString( "%.1f", psItemDefinition->AddSpecRunSpeedMax ) );
	}

	if( bSpecStatus )
	{
		AddString( 0, "" );
		AddString( 1, "" );
	}

	if( bReqStatus )
	{
		AddString( 0, "Req Status", D3DCOLOR_ARGB( 255, 236, 236, 236 ), TRUE );
		AddString( 1, "" );
	}

	// Set Status
	{
		// Get Minor
		int iLevel = low( saItemRequeriments[iSelectedSpec].sLevel[0], saItemRequeriments[iSelectedSpec].sLevel[1] );
		int iStrength = low( saItemRequeriments[iSelectedSpec].sStrength[0], saItemRequeriments[iSelectedSpec].sStrength[1] );
		int iSpirit = low( saItemRequeriments[iSelectedSpec].sSpirit[0], saItemRequeriments[iSelectedSpec].sSpirit[1] );
		int iTalent = low( saItemRequeriments[iSelectedSpec].sTalent[0], saItemRequeriments[iSelectedSpec].sTalent[1] );
		int iAgility = low( saItemRequeriments[iSelectedSpec].sAgility[0], saItemRequeriments[iSelectedSpec].sAgility[1] );
		int iHealth = low( saItemRequeriments[iSelectedSpec].sHealth[0], saItemRequeriments[iSelectedSpec].sHealth[1] );

		if ( psItemDefinition->sItem.iLevel )
		{
			AddString( 0, "Req Level", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
			AddString( 1, FormatString( "%d", psItemDefinition->sItem.iLevel + ((psItemDefinition->sItem.iLevel * iLevel) / 100) ) );
		}
		if ( psItemDefinition->sItem.iStrength )
		{
			AddString( 0, "Req Strength", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
			AddString( 1, FormatString( "%d", psItemDefinition->sItem.iStrength + ((psItemDefinition->sItem.iStrength * iStrength) / 100) ) );
		}
		if ( psItemDefinition->sItem.iSpirit )
		{
			AddString( 0, "Req Spirit", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
			AddString( 1, FormatString( "%d", psItemDefinition->sItem.iSpirit + ((psItemDefinition->sItem.iSpirit * iSpirit) / 100) ) );
		}
		if ( psItemDefinition->sItem.iTalent )
		{
			AddString( 0, "Req Talent", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
			AddString( 1, FormatString( "%d", psItemDefinition->sItem.iTalent + ((psItemDefinition->sItem.iTalent * iTalent) / 100) ) );
		}
		if ( psItemDefinition->sItem.iAgility )
		{
			AddString( 0, "Req Agility", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
			AddString( 1, FormatString( "%d", psItemDefinition->sItem.iAgility + ((psItemDefinition->sItem.iAgility * iAgility) / 100) ) );
		}
		if ( psItemDefinition->sItem.iHealth )
		{
			AddString( 0, "Req Health", D3DCOLOR_ARGB( 255, 236, 236, 236 ), FALSE );
			AddString( 1, FormatString( "%d", psItemDefinition->sItem.iHealth + ((psItemDefinition->sItem.iHealth * iHealth) / 100) ) );
		}
	}

	// Add to List
	UI::List_ptr pListItems = pWindow1->GetElement<UI::List>( LISTID_Items );
	pListItems->Clear();
	pListItems->SetPaddingSeparator( 3 );

	UI::Window_ptr pWindow = pListItems->GetWindow( WINDOWID_ItemInfo );

	BOOL bAddWindow = TRUE;

	if ( pWindow )
	{
		pWindow->Clear();
		bAddWindow = FALSE;
	}
	else
		pWindow = std::make_shared<UI::Window>( Rectangle2D( 170, 0, 170, 60 ) );

	pWindow->SetID( WINDOWID_ItemInfo );

	// Name
	UI::Text_ptr pItemName = std::make_shared<UI::Text>( pszName, Rectangle2D( 0, 10, 240, 16 ), TRUE, D3DCOLOR_ARGB( 255, 194, 165, 110 ) );
	pWindow->AddElement( pItemName );

	// Description
	UI::Text_ptr pItemDecription = std::make_shared<UI::Text>( Rectangle2D( 0, 26, 170, 64 ) );
	pItemDecription->SetText( pszDescription );
	pItemDecription->SetMultiLine( TRUE );
	pItemDecription->SetWordWrap( TRUE );
	pItemDecription->SetColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	pWindow->AddElement( pItemDecription );

	if( bAddWindow )
		pListItems->AddWindow( pWindow );

	UI::Window_ptr pWindowStatus = std::make_shared<UI::Window>( Rectangle2D( 170, 0, 100, 0 ) );
	pWindowStatus->SetID( WINDOWID_ItemStatus );
	for( std::size_t i = 0; i < vLeftText.size(); i++ )
	{
		vLeftText[i]->SetBox( Rectangle2D( 0, pWindowStatus->GetHeight(), 100, 10 ) );
		pWindowStatus->AddElement( vLeftText[i] );

		vRightText[i]->SetBox( Rectangle2D( 80, pWindowStatus->GetHeight(), 100, 10 ) );
		pWindowStatus->AddElement( vRightText[i] );

		pWindowStatus->SetHeight( pWindowStatus->GetHeight() + 13 );
	}
	pListItems->AddWindow( pWindowStatus );
}

void CCoinShopWindow::OnCreditsButtonClick( UIEventArgs eArgs )
{
	MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_OkCancel );
	MESSAGEBOX->SetTitle( "Page Redirect" );
	MESSAGEBOX->SetDescription( "You will be redirected to the donation page" );
	MESSAGEBOX->SetEvent( std::bind( &CCoinShopWindow::OnBuyCredits, this, std::placeholders::_1 ) );
	MESSAGEBOX->Show();
	
	pWindow1->Hide();
}

void CCoinShopWindow::OnCloseButtonClick( UIEventArgs eArgs )
{
	if ( !CheckSlots() )
		return;

	ClearLists();
	ClearString();
	iSelectedSpec = 0;
	iCurChildTab = 0;
	bItemData = FALSE;
	pWindow1->Hide();

	MOUSEHANDLER->UnblockMouse();
}

void CCoinShopWindow::OnHelpButtonClick( UIEventArgs eArgs )
{
}

void CCoinShopWindow::OnMouseMove( class CMouse * pcMouse )
{
	pWindow1->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

BOOL CCoinShopWindow::OnMouseClick( class CMouse * pcMouse )
{
	if ( pWindow1->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;
	return FALSE;
}

BOOL CCoinShopWindow::OnMouseScroll( class CMouse * pcMouse )
{
	UI::Window_ptr pWindowItem = pWindow1->GetElement<UI::Window>( WINDOWID_ItemData );
	if ( pWindow1->IsOpen() && pWindowItem )
	{
		if ( pWindowItem->OnMouseScroll( pcMouse, pWindow1->GetX(), pWindow1->GetY(), DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
			return TRUE;
	}
	if ( pWindow1->OnMouseScroll( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;
	return FALSE;
}

BOOL CCoinShopWindow::OnKeyPress( class CKeyboard * pcKeyboard )
{
	if ( pWindow1->OnKeyPress( pcKeyboard ) )
		return TRUE;
	return FALSE;
}

BOOL CCoinShopWindow::OnKeyChar( class CKeyboard * pcKeyboard )
{
	if ( pWindow1->OnKeyChar( pcKeyboard ) )
		return TRUE;
	return FALSE;
}

void CCoinShopWindow::OnItemButtonClick( class CCoinShopItem * pcItem, UIEventArgs eArgs )
{
	iSelectedSpec = 0;
	PacketCoinOpenItemData sPacket;
	sPacket.iID		= pcItem->GetID();
	sPacket.iTabID	= pcItem->GetTabID();
	COINHANDLER->ProcessPacket( &sPacket );
}

void CCoinShopWindow::OnAgingRecoveryItemButtonClick( int iID, int iPrice , UIEventArgs eArgs )
{
	auto pListItems = pWindow1->GetElement<UI::List>( LISTID_Items );
	if ( pListItems )
	{
		UI::Window_ptr pWindowItm = pListItems->GetWindow( iID );
		if ( pWindowItm )
		{
			UI::Text_ptr pTextName = pWindowItm->GetElement<UI::Text>( LABELID_Name );
			if ( pTextName )
			{
				iItemAgingRecoveryID = iID;

				MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
				MESSAGEBOX->SetTitle( pTextName->GetText() );
				MESSAGEBOX->SetDescription( FormatString( "Do you want to recovery this item?\nCredits: %d", iPrice ) );
				MESSAGEBOX->SetEvent( std::bind( &CCoinShopWindow::OnBuyAgingRecovery, this, iPrice, std::placeholders::_1 ) );
				MESSAGEBOX->Show();
			}
		}
	}
}

void CCoinShopWindow::OnChangeSpecSelect( class CCoinShopItem * pcItem, UIEventArgs eArgs )
{
	UI::Window_ptr pWindowItem = pWindow1->GetElement<UI::Window>( WINDOWID_ItemData );
	if ( pWindowItem )
	{
		UI::ComboBox_ptr pComboBox = pWindowItem->GetElement<UI::ComboBox>( COMBOBOX_Spec );
		if ( pComboBox )
		{
			iSelectedSpec = pComboBox->GetSelectedID();
			if ( bItemData )
				SetItemStatus( &sItemDefinition, pcItem->GetName(), pcItem->GetDescription() );
		}
	}
}

void CCoinShopWindow::OnChangeSwapSelect( CCoinShopItem * pcItem, UIEventArgs eArgs )
{
	UI::Window_ptr pWindowItem = pWindow1->GetElement<UI::Window>( WINDOWID_ItemData );
	if ( pWindowItem )
	{
		UI::ComboBox_ptr pComboBox = pWindowItem->GetElement<UI::ComboBox>( COMBOBOX_Swap );
		UI::ItemBox_ptr pItemBox = pWindowItem->GetElement<UI::ItemBox>( ITEMBOX_Swapper );

		if ( pComboBox && pItemBox && pItemBox->GetItems().size() > 0 )
		{
			ItemData * pcItemData = pItemBox->GetItems()[0];

			iSelectedSwap = pComboBox->GetSelectedID();

			int iCoins = 0;

			//Set Price
			iCoins = Base::Coin::GetSwapperPrice( &pcItemData->sItem, iSelectedSwap );

			//Global Discount
			if ( COINHANDLER->GetCoinShop()->GetDiscount() > 0 )
				iCoins = (COINHANDLER->GetCoinShop()->GetDiscount() * iCoins) / 100;

			UI::Text_ptr pText = pWindowItem->GetElement<UI::Text>( LABELID_Price );
			if ( pText )
			{
				pText->SetText( FormatNumber( iCoins ) );
			}
		}
	}
}

void CCoinShopWindow::OnChangeQuantityItem( class CCoinShopItem * pcItem, UIEventArgs eArgs )
{
	if ( pcItem )
	{
		if ( pcItem->GetIsQuantity() )
		{
			UI::Window_ptr pWindowItem = pWindow1->GetElement<UI::Window>( WINDOWID_ItemData );
			if ( pWindowItem )
			{
				UI::Counter_ptr pCounter = pWindowItem->GetElement<UI::Counter>( COUNTID_Quantity );
				if ( pCounter )
				{
					UI::Text_ptr pText = pWindowItem->GetElement<UI::Text>( LABELID_Price );
					if ( pText )
					{
						int iPrice = (pcItem->GetDiscount() > 0 ? pcItem->GetDiscount() : pcItem->GetValue()) * pCounter->GetCurrent();

						if ( pcItem->GetBulk() )
						{
							UI::Text_ptr pTextD = pWindowItem->GetElement<UI::Text>( LABELID_DiscountBulk );
							if ( pTextD && pCounter->GetCurrent() >= 5 )
							{
								int iBulk = pCounter->GetCurrent() / 5;
									
								if ( (iBulk*pcItem->GetBulk()) > pcItem->GetMaxBulk() )
									iBulk = pcItem->GetMaxBulk() / 5;
								
								
								pTextD->SetText( FormatString( "Bulk: %d%%", iBulk * pcItem->GetBulk() ) );
								iPrice -= (iPrice*(pcItem->GetBulk() * iBulk)) / 100;
							}
							else
								pTextD->SetText( "" );
						}

						pText->SetText( FormatNumber(iPrice) );
					}
				}
			}
		}
	}
}

void CCoinShopWindow::OnChangeLevelUPItem( UIEventArgs eArgs )
{
	UI::Window_ptr pWindowItem = pWindow1->GetElement<UI::Window>( WINDOWID_ItemData );
	if ( pWindowItem )
	{
		UI::Counter_ptr pCounter = pWindowItem->GetElement<UI::Counter>( COUNTID_Quantity );
		if ( pCounter )
		{
			if ( UNITDATA->sCharacterData.iLevel >= pCounter->GetCurrent() )
			{
				pCounter->SetMinMax( UNITDATA->sCharacterData.iLevel + 1, MAX_LEVEL_COINSHOP );
				pCounter->SetCurrent( UNITDATA->sCharacterData.iLevel + 1 );
			}

			UI::Text_ptr pText = pWindowItem->GetElement<UI::Text>( LABELID_Price );
			if ( pText )
			{
				int iLevelUPPrice = Base::Coin::GetLevelUPPrice( UNITDATA->sCharacterData.iLevel, pCounter->GetCurrent() );
				int iPrice = iLevelUPPrice;

				pText->SetText( FormatNumber( iPrice ) );
			}
		}
	}
}

void CCoinShopWindow::OnBuyButtonClick( class CCoinShopItem * pcItem, UIEventArgs eArgs )
{
	if ( !IsBuying() )
	{
		if ( pcItem )
		{
			int iCount = 1;
			if ( pcItem->GetIsQuantity() )
			{
				UI::Window_ptr pWindowItem = pWindow1->GetElement<UI::Window>( WINDOWID_ItemData );
				if ( pWindowItem )
				{
					UI::Counter_ptr pCounter = pWindowItem->GetElement<UI::Counter>( COUNTID_Quantity );
					if ( pCounter )
					{
						iCount = pCounter->GetCurrent();
					}
				}
			}
			PacketCoinBuy sPacket;
			sPacket.iParam1 = iSelectedSpec;
			sPacket.iParam2 = iCount;
			sPacket.iID = pcItem->GetID();
			sPacket.iTabID = pcItem->GetTabID();
			COINHANDLER->ProcessPacket( &sPacket );
			SetBuying( TRUE );
		}
	}
	else
		TITLEBOX( "You are buying process!" );
}

void CCoinShopWindow::OnBuyServiceButtonClick( int iType, UIEventArgs eArgs )
{
	UI::Window_ptr pWindowItem = pWindow1->GetElement<UI::Window>( WINDOWID_ItemData );

	//Do This, due union structs...
	PacketCoinServiceBuy sPacket;

	if ( pWindowItem )
	{
		//Get Elements
		UI::ItemBox_ptr pItemBox = pWindowItem->GetElement<UI::ItemBox>( ITEMBOX_Perfectize );
		UI::ComboBox_ptr pComboBox = pWindowItem->GetElement<UI::ComboBox>( COMBOBOX_Spec );
		UI::InputField_ptr pInput = pWindowItem->GetElement<UI::InputField>( INPUTID_ChangeNick );
		UI::ComboBox_ptr pComboBoxChangeClass = pWindowItem->GetElement<UI::ComboBox>( COMBOBOX_ClassChange );
		UI::CheckBox_ptr pCheckBoxChangePremium = pWindowItem->GetElement<UI::CheckBox>( CHECKBOX_ChangeClassPremium );
		UI::Counter_ptr pCounter = pWindowItem->GetElement<UI::Counter>( COUNTID_Quantity );

		switch ( iType )
		{
			case ITEMCOIN_LevelUP:
				if ( pCounter )
				{
					sPacket.iType = iType;

					sPacket.LevelUP.iLevel = pCounter->GetCurrent();

					//Send
					COINHANDLER->ProcessPacket( &sPacket );
				}
				break;

			case ITEMCOIN_ItemPerfect:

				//Have Elements and have Item?
				if ( pItemBox && pItemBox->GetItems().size() > 0 )
				{
					// Check iWeight
					if ( !CHARACTERGAME->CheckInventoryWeight( pItemBox->GetItems()[0] ) )
					{
						TITLEBOX( "Weight limit exceeded" );
						return;
					}

					// Check Space
					if ( !CHARACTERGAME->CheckInventorySpace( ITEMHANDLER->FindItemTableByCode( ITEMID_BrazilianSoccer ) ) )
					{
						TITLEBOX( "Not enough space in inventory" );
						return;
					}

					//Copy item to Packet
					CopyMemory( &sPacket.ItemPerfect.cItemData, pItemBox->GetItems()[0], sizeof( ItemData ) );

					//Handle Spec
					if ( iSelectedSpec > 0 )
						sPacket.ItemPerfect.iSpec = iSelectedSpec;
					else
						sPacket.ItemPerfect.iSpec = ClassFlagToCharacterClass( (EClassFlag)sPacket.ItemPerfect.cItemData.sItem.eSpecialization );

					sPacket.iType = iType;

					//Block Item Box
					pItemBox->DisableMouse();

					//Send
					COINHANDLER->ProcessPacket( &sPacket );
				}
				break;


			case ITEMCOIN_ItemSwapper:
				pItemBox = pWindowItem->GetElement<UI::ItemBox>( ITEMBOX_Swapper );
				//Have Elements and have Item?
				if ( pItemBox && pItemBox->GetItems().size() > 0 )
				{
					if ( pItemBox->GetItems()[0]->sItem.sItemID.ToItemType() == iSelectedSwap )
					{
						TITLEBOX( "Cannot swap same item type!" );
						return;
					}

					//Check Weight
					if ( !CHARACTERGAME->CheckInventoryWeight( pItemBox->GetItems()[0] ) )
					{
						TITLEBOX( "Weight limit exceeded" );
						return;
					}

					//Check Space
					if ( !CHARACTERGAME->CheckInventorySpace( ITEMHANDLER->FindItemTableByCode( ITEMID_BrazilianSoccer ) ) )
					{
						TITLEBOX( "Not enough space in inventory" );
						return;
					}

					//Copy item to Packet
					CopyMemory( &sPacket.ItemSwapper.cItemData, pItemBox->GetItems()[0], sizeof( ItemData ) );

					//Handle Spec
					if ( iSelectedSpec > 0 )
						sPacket.ItemSwapper.iSpec = iSelectedSpec;
					else
						sPacket.ItemSwapper.iSpec = ClassFlagToCharacterClass( (EClassFlag)sPacket.ItemSwapper.cItemData.sItem.eSpecialization );

					sPacket.ItemSwapper.iItemBase = iSelectedSwap;

					sPacket.iType = iType;

					//Block Item Box
					pItemBox->DisableMouse();

					//Send
					COINHANDLER->ProcessPacket( &sPacket );
				}
				break;

			case ITEMCOIN_ChangeNick:
				if ( pInput )
				{
					if ( pInput->GetText().length() > 0 )
					{
						if ( STRINGCOMPAREI( UNITDATA->sCharacterData.szName, pInput->GetText().c_str() ) == FALSE )
						{
							sPacket.iType = iType;

							if ( pInput->GetText().length() <= 15 )
							{
								STRINGCOPY( sPacket.ChangeNick.szNewCharacterName, pInput->GetText().c_str() );

								COINHANDLER->ProcessPacket( &sPacket );
							}
							else
							{
								TITLEBOX( "Nick is too long!" );
								break;
							}
						}
						else
						{
							TITLEBOX( "You are using this nick, please choose other!" );
							break;
						}
					}
					else
						TITLEBOX( "You need to select a new nick" );
				}
				break;

			case ITEMCOIN_ChangeClass:
				sPacket.iType = iType;

				sPacket.ChangeClass.szNewCharacterName[0]	= 0;
				sPacket.ChangeClass.bPremiumChangeClass		= FALSE;

				if ( pInput )
				{
					if ( pInput->GetText().length() > 0 )
					{
						if ( STRINGCOMPAREI( UNITDATA->sCharacterData.szName, pInput->GetText().c_str() ) == FALSE )
						{
							if ( pInput->GetText().length() <= 15 )
								STRINGCOPY( sPacket.ChangeClass.szNewCharacterName, pInput->GetText().c_str() );
							else
								TITLEBOX( "Nick is too long!" );
						}
					}
				}

				if ( pComboBoxChangeClass )
				{
					if ( pComboBoxChangeClass->GetSelectedID() != (-1) )
					{
						sPacket.ChangeClass.iClass = pComboBoxChangeClass->GetSelectedID();

						if ( UNITDATA->sCharacterData.iClass == sPacket.ChangeClass.iClass )
						{
							TITLEBOX( "You need to choose a new class!" );
							break;
						}

						if ( pCheckBoxChangePremium )
							sPacket.ChangeClass.bPremiumChangeClass = pCheckBoxChangePremium->IsChecked();

						COINHANDLER->ProcessPacket( &sPacket );
					}
					else
					{
						TITLEBOX( "You need to choose a new class!" );
						break;
					}
				}
				
				break;

			default:
				break;
		}
	}

}

void CCoinShopWindow::OnCheckPremiumClass( int iOldPrice, UIEventArgs eArgs )
{
	UI::Window_ptr pWindowItem = pWindow1->GetElement<UI::Window>( WINDOWID_ItemData );

	//Get Discount Percent
	int iDiscount	= (iOldPrice * 100) / 3500;
	int iPrice		= iOldPrice;


	if ( pWindowItem )
	{
		//Get Elements
		UI::CheckBox_ptr pCheckBoxChangePremium = pWindowItem->GetElement<UI::CheckBox>( CHECKBOX_ChangeClassPremium );
		UI::Text_ptr pItemValuePrice = pWindowItem->GetElement<UI::Text>( LABELID_Price );

		if ( pCheckBoxChangePremium->IsChecked() )
			iPrice = (6500 * iDiscount) / 100;

		pItemValuePrice->SetText( FormatNumber( iPrice ) );
	}
}

void CCoinShopWindow::OnParentTabButtonClick( class CCoinShopTab * pcTab, UIEventArgs eArgs )
{
	if ( !CheckSlots() )
		return;

	if ( pcTab )
	{
		// Clear Child Tab List
		ClearChildTabList();

		ClearItemsList();
		ClearString();
		iSelectedSpec = 0;
		bItemData = FALSE;

		// Add Child Tabs to List
		std::vector<CCoinShopTab*> & vTabs = pcTab->GetChildTabs();
		for ( std::vector<CCoinShopTab*>::iterator it = vTabs.begin(); it != vTabs.end(); it++ )
		{
			CCoinShopTab * pcChildTab = *it;
			AddChildTab( pcChildTab );
		}

		// Load Items
		PacketCoinOpenTab sPacket;
		sPacket.iID = pcTab->GetID();
		sPacket.iReturnCode = 0;
		COINHANDLER->ProcessPacket( &sPacket );
	}
}

void CCoinShopWindow::OnChildTabButtonClick( class CCoinShopTab * pcTab, UIEventArgs eArgs )
{
	if ( !CheckSlots() )
		return;

	if ( pcTab )
	{
		// Clear Child Tab List
		ClearItemsList();
		ClearString();

		// Load Items
		PacketCoinOpenTab sPacket;
		sPacket.iID = pcTab->GetID();
		sPacket.iReturnCode = 0;
		COINHANDLER->ProcessPacket( &sPacket );
	}
}

void CCoinShopWindow::OnChildMoveButtonClick( EMoveChildTab eType, UIEventArgs eArgs )
{
	auto pChildList = pWindow1->GetElement<UI::List>( LISTID_ChildTabs );
	if ( pChildList && iCountChildTab > 4 )
	{
		if ( eType == CHILDTABMOVE_Left )
		{
			if ( iCurChildTab > 0 )
			{
				pChildList->AddAxis( 77, 0 );
				iCurChildTab--;
			}
		}
		else
		{
			if ( iCurChildTab < (iCountChildTab - 4) )
			{
				pChildList->AddAxis( -77, 0 );
				iCurChildTab++;
			}
		}
	}
}

void CCoinShopWindow::OnRotateUnitMesh( int iRotateType )
{
	auto pList = pWindow1->GetElement<UI::List>( LISTID_Items );

	UI::Window_ptr pWindowItem = pWindow1->GetElement<UI::Window>( WINDOWID_ItemData );
	UI::Window_ptr pWindowPreview = pWindowItem->GetElement<UI::Window>( WINDOWID_ItemPreview );
	UI::UnitMesh_ptr pUnitMesh = pWindowPreview->GetElement<UI::UnitMesh>( UNITMESHID_HeadPreview );

	if( pUnitMesh )
	{
		if( iRotateType == 1 )
			pUnitMesh->SetAngle( pUnitMesh->GetAngle() + 10 );
		else if( iRotateType == 2 )
			pUnitMesh->SetAngle( pUnitMesh->GetAngle() - 10 );
	}
}

void CCoinShopWindow::ClearParentTabList()
{
	auto pList = pWindow1->GetElement<UI::List>( LISTID_Tabs );
	
	if ( pList )
		pList->Clear();
}

void CCoinShopWindow::ClearChildTabList()
{
	auto pList = pWindow1->GetElement<UI::List>( LISTID_ChildTabs );
	
	iCurChildTab = 0;
	iCountChildTab = 0;

	pList->SetAxis( 0, 0 );

	if ( pList )
		pList->Clear();
}

void CCoinShopWindow::ClearItemsList()
{
	auto pList = pWindow1->GetElement<UI::List>( LISTID_Items );
	
	UI::Window_ptr pWindowItem = pWindow1->GetElement<UI::Window>( WINDOWID_ItemData );

	UI::ItemBox_ptr pItemBox = pWindowItem->GetElement<UI::ItemBox>( ITEMBOX_Perfectize );
	

	pWindowItem->Clear();

	if ( pList )
		pList->Clear();
}

void CCoinShopWindow::ClearLists()
{
	ClearParentTabList();
	ClearChildTabList();
	ClearItemsList();
	ClearString();
}

void CCoinShopWindow::ClearQuantity()
{
	UI::Window_ptr pWindowItem = pWindow1->GetElement<UI::Window>( WINDOWID_ItemData );
	if ( pWindowItem )
	{
		UI::Counter_ptr pCounter = pWindowItem->GetElement<UI::Counter>( COUNTID_Quantity );
		if ( pCounter )
		{
			pCounter->SetCurrent( 1 );
		}
	}
}

void CCoinShopWindow::DeleteAgingRecoveryItem( int iID )
{
	int iCount = 0;
	auto pList = pWindow1->GetElement<UI::List>( LISTID_Items );
	if ( pList )
	{
		for ( std::deque<UI::Window_ptr>::iterator it = pList->GetWindowList().begin(); it != pList->GetWindowList().end(); )
		{
			UI::Window_ptr pc = (*it);

			if ( pc )
			{
				if ( pc->GetID() == iID )
				{
					pc->DecRef();
					if ( pc->GetRef() == 0 )
					{
						pc->Clear();
						pc = nullptr;
					}

					it = pList->GetWindowList().erase( it );

					iCount++;
				}
				else
					it++;
			}
			else
				it++;
		}
	}
}

void CCoinShopWindow::BuildWindow1()
{
	pWindow1 = std::make_shared<UI::Window>( Rectangle2D( ( RESOLUTION_WIDTH >> 1 ) - ( 560 >> 1 ), ( RESOLUTION_HEIGHT >> 1 ) - ( 450 >> 1 ), 560, 450 ) );
	pWindow1->SetImage( pcImageCoinShopBackground );

	// Window Item Data
	UI::Window_ptr pWindow2 = std::make_shared<UI::Window>( Rectangle2D( 196, 92, 328, 328 ) );
	pWindow2->SetID( WINDOWID_ItemData );
	pWindow1->AddElement( pWindow2 );

	// List
	UI::List_ptr pListItems = std::make_shared<UI::List>( Rectangle2D( 198, 94, 330, 328 ) );
	pListItems->SetID( LISTID_Items );
	pListItems->SetPaddingSeparator( 8 );
	pListItems->MakeScrollBar( Rectangle2D( 329, 0, 6, 330 ), "game\\images\\coinshop\\scrollbar.png", "game\\images\\UI\\scroll\\icon_2.png" );
	pWindow1->AddElement( pListItems );

	// List Tab
	UI::List_ptr pListTabs = std::make_shared<UI::List>( Rectangle2D( 23, 58, 161, 258 ) );
	pListTabs->SetID( LISTID_Tabs );
	pListTabs->SetPaddingSeparator( 1 );
	pWindow1->AddElement( pListTabs );

	// List Child Tab
	UI::List_ptr pListChildTabs = std::make_shared<UI::List>( Rectangle2D( 214, 57, 306, 28 ) );
	pListChildTabs->SetID( LISTID_ChildTabs );
	pListChildTabs->SetHorizontal( TRUE );
	pListChildTabs->SetPaddingSeparator( 3 );
	pWindow1->AddElement( pListChildTabs );

	// Buttons
	{
		UI::Button_ptr pButtonCredits = std::make_shared<UI::Button>( Rectangle2D( 24, 334, 93, 23 ) );
		pButtonCredits->SetID( BUTTONID_Credits );
		pButtonCredits->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\coinshop\\btncredits.png", "game\\images\\coinshop\\btncredits_.png" ) );
		pButtonCredits->SetEvent( UI::Event::Build( std::bind( &CCoinShopWindow::OnCreditsButtonClick, this, std::placeholders::_1 ) ) );
		pWindow1->AddElement( pButtonCredits );

		UI::Button_ptr pButtonClose = std::make_shared<UI::Button>( Rectangle2D( 24, 364, 93, 23 ) );
		pButtonClose->SetID( BUTTONID_Close );
		pButtonClose->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\coinshop\\btnclose.png", "game\\images\\coinshop\\btnclose_.png" ) );
		pButtonClose->SetEvent( UI::Event::Build( std::bind( &CCoinShopWindow::OnCloseButtonClick, this, std::placeholders::_1 ) ) );
		pWindow1->AddElement( pButtonClose );

		UI::Button_ptr pButtonHelp = std::make_shared<UI::Button>( Rectangle2D( 156, 396, 26, 30 ) );
		pButtonHelp->SetID( BUTTONID_Help );
		pButtonHelp->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\coinshop\\help.png", "game\\images\\coinshop\\help_.png" ) );
		pButtonHelp->SetEvent( UI::Event::Build( std::bind( &CCoinShopWindow::OnHelpButtonClick, this, std::placeholders::_1 ) ) );
		pWindow1->AddElement( pButtonHelp );

		// Buttons on Child Tab
		UI::Button_ptr pButtonLeft = std::make_shared<UI::Button>( Rectangle2D( 198, 58, 12, 12 ) );
		pButtonLeft->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\coinshop\\left.png", "game\\images\\coinshop\\left_.png" ) );
		pButtonLeft->SetEvent( UI::Event::Build( std::bind( &CCoinShopWindow::OnChildMoveButtonClick, this, CHILDTABMOVE_Left, std::placeholders::_1 ) ) );
		pWindow1->AddElement( pButtonLeft );

		UI::Button_ptr pButtonRight = std::make_shared<UI::Button>( Rectangle2D( 520, 58, 12, 12 ) );
		pButtonRight->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\coinshop\\right.png", "game\\images\\coinshop\\right_.png" ) );
		pButtonRight->SetEvent( UI::Event::Build( std::bind( &CCoinShopWindow::OnChildMoveButtonClick, this, CHILDTABMOVE_Right, std::placeholders::_1 ) ) );
		pWindow1->AddElement( pButtonRight );
	}

	// Texts
	{
		UI::Text_ptr pTextCredits = std::make_shared<UI::Text>( Rectangle2D( 26, 396, 160, 18 ) );
		pTextCredits->SetID( TEXTID_Credits );
		pTextCredits->SetMultiLine( TRUE );
		pTextCredits->SetWordWrap( TRUE );
		pTextCredits->SetFont( "Arial", 14, 6, FALSE, FALSE, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		pTextCredits->SetText( "Welcome\nYou have 0 Credits" );
		pWindow1->AddElement( pTextCredits );
	}

	// Groups
	{
		// Tabs
		UI::Group_ptr pGroupTab = std::make_shared<UI::Group>();
		pGroupTab->SetID( GROUPID_TabButtons );
		pWindow1->AddElement( pGroupTab );

		// Child Tabs
		UI::Group_ptr pGroupChildTab = std::make_shared<UI::Group>();
		pGroupChildTab->SetID( GROUPID_ChildTabButtons );
		pWindow1->AddElement( pGroupChildTab );
	}

	pWindow1->SetMoveBox( Rectangle2D( 0, 0, pWindow1->GetWidth(), 40 ) );
	pWindow1->SetCanMove( TRUE );

	pWindow1->Hide();
}

void CCoinShopWindow::Open()
{
	pWindow1->SetImage( COINHANDLER->IsTimeShop() ? pcImageTimeShopBackground : pcImageCoinShopBackground );

	HIDERIGHTNPCBOX;
	pWindow1->Show();

	MOUSEHANDLER->BlockMouse();
}

void CCoinShopWindow::AddChildTab( class CCoinShopTab * pcTab )
{
	auto pChildList = pWindow1->GetElement<UI::List>( LISTID_ChildTabs );
	auto pGroup = pWindow1->GetElement<UI::Group>( GROUPID_ChildTabButtons );
	if ( pChildList && pGroup )
	{
		char szImageTab1[MAX_PATH], szImageTab2[MAX_PATH];
		STRINGFORMAT( szImageTab1, "game\\images\\coinshop\\categories\\%s.png", pcTab->GetName() );
		STRINGFORMAT( szImageTab2, "game\\images\\coinshop\\categories\\%s_.png", pcTab->GetName() );

		// New Window for button
		UI::Window_ptr pWindowChild = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 74, 14 ) );

		// Button Click
		UI::Button_ptr pButtonChildTab = std::make_shared<UI::Button>( Rectangle2D( 0, 0, 74, 14 ) );
		pButtonChildTab->SetID( pcTab->GetID() );
		pButtonChildTab->SetGroup( pGroup );
		pButtonChildTab->SetEvent( UI::Event::Build( std::bind( &CCoinShopWindow::OnChildTabButtonClick, this, pcTab, std::placeholders::_1 ) ) );
		pButtonChildTab->SetActiveImage( UI::ImageLoader::LoadActiveImage( szImageTab1, szImageTab2 ) );
		pButtonChildTab->SetSelect( TRUE );
		pWindowChild->AddElement( pButtonChildTab );

		pChildList->AddWindow( pWindowChild );

		// Add Count to compute tabs next, prior
		iCountChildTab++;
	}
}

void CCoinShopWindow::AddTab( class CCoinShopTab * pcTab )
{
	auto pList = pWindow1->GetElement<UI::List>( LISTID_Tabs );
	auto pGroup = pWindow1->GetElement<UI::Group>( GROUPID_TabButtons );
	if ( pList && pGroup )
	{
		char szImageTab1[MAX_PATH], szImageTab2[MAX_PATH];
		STRINGFORMAT( szImageTab1, "game\\images\\coinshop\\tabs\\%s.png", pcTab->GetName() );
		STRINGFORMAT( szImageTab2, "game\\images\\coinshop\\tabs\\%s_.png", pcTab->GetName() );

		// New Window for button
		UI::Window_ptr pWindowTab = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 159, 29 ) );

		// Button Click
		UI::Button_ptr pButtonTab = std::make_shared<UI::Button>( Rectangle2D( 0, 0, 159, 29 ) );
		pButtonTab->SetID( pcTab->GetID() );
		pButtonTab->SetGroup( pGroup );
		pButtonTab->SetEvent( UI::Event::Build( std::bind( &CCoinShopWindow::OnParentTabButtonClick, this, pcTab, std::placeholders::_1 ) ) );
		pButtonTab->SetActiveImage( UI::ImageLoader::LoadActiveImage( szImageTab1, szImageTab2 ) );
		pButtonTab->SetSelect( TRUE );
		pWindowTab->AddElement( pButtonTab );

		pList->AddWindow( pWindowTab );
	}
}

void CCoinShopWindow::Load( class CCoinShopTab * pcTab )
{
	auto pListItems = pWindow1->GetElement<UI::List>( LISTID_Items );
	pListItems->SetPaddingSeparator( 8 );

	if ( pListItems )
	{
		// Clear List
		pListItems->Clear();

		// Get Item List
		std::vector<CCoinShopItem*> & vItems = pcTab->GetItems();
		for ( std::vector<CCoinShopItem*>::iterator it = vItems.begin(); it != vItems.end(); it++ )
		{
			CCoinShopItem * pcItem = *it;
			if ( pcItem )
			{
				UI::Image * pItemImage = UI::ImageLoader::LoadImage( pcItem->GetImage() );
				if ( pItemImage )
				{
					int iImageWidth		= pItemImage->GetWidth();
					int iImageHeight	= pItemImage->GetHeight();

					int iImagePaddingX	= 44 - (iImageWidth >> 1);
					int iImagePaddingY	= 44 - (iImageHeight >> 1);

					// Create Window of Item
					UI::Window_ptr pWindowItem = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 324, 90 ) );
					pWindowItem->SetID( pcItem->GetID() );

					// Button Select
					UI::Button_ptr pButtonBuy = std::make_shared<UI::Button>( Rectangle2D( 0, 0, 324, 90 ) );
					pButtonBuy->SetEvent( UI::Event::Build( std::bind( &CCoinShopWindow::OnItemButtonClick, this, pcItem, std::placeholders::_1 ) ) );
					pButtonBuy->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\coinshop\\bgitem.png", "game\\images\\coinshop\\bgitem_.png" ) );
					pWindowItem->AddElement( pButtonBuy );

					// Image
					UI::ImageBox_ptr pItemImageBox = std::make_shared<UI::ImageBox>( Rectangle2D( iImagePaddingX + 3, iImagePaddingY + 1, iImageWidth, iImageHeight ) );
					pItemImageBox->SetImage( pItemImage );
					pWindowItem->AddElement( pItemImageBox );

					// Name
					UI::Text_ptr pItemName = std::make_shared<UI::Text>( Rectangle2D( 89, 13, 235, 16 ) );
					pItemName->SetText( pcItem->GetName() );
					pItemName->SetColor( D3DCOLOR_ARGB( 255, 194, 165, 110 ) );
					pWindowItem->AddElement( pItemName );

					// Description
					UI::Text_ptr pItemDecription = std::make_shared<UI::Text>( Rectangle2D( 89, 28, 235, 34 ) );
					pItemDecription->SetText( pcItem->GetDescription() );
					pItemDecription->SetMultiLine( TRUE );
					pItemDecription->SetWordWrap( TRUE );
					pItemDecription->SetColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
					pWindowItem->AddElement( pItemDecription );

					// Commify Price
					int iPrice = pcItem->GetDiscount() > 0 ? pcItem->GetDiscount() : pcItem->GetValue();

					if ( iPrice > 0 )
					{
						// Coin Image
						UI::ImageBox_ptr pCoinImageBox = std::make_shared<UI::ImageBox>( Rectangle2D( 242, 13, 21, 13 ) );
						pCoinImageBox->SetImage( UI::ImageLoader::LoadImage( "game\\images\\coinshop\\coin.bmp" ) );
						pWindowItem->AddElement( pCoinImageBox );

						// Price
						UI::Text_ptr pItemPrice = std::make_shared<UI::Text>( Rectangle2D( 266, 13, 64, 18 ) );
						pItemPrice->SetFont( "Arial", 14, 0, FALSE, FALSE, D3DCOLOR_ARGB( 255, 205, 170, 60 ) );

						if ( STRINGCOMPARE( pcItem->GetCode(), "AgeRec" ) )
							pItemPrice->SetText( "400-800" );
						else
							pItemPrice->SetTextEx( "%s", FormatNumber( iPrice ) );

						pWindowItem->AddElement( pItemPrice );
					}
					pListItems->AddWindow( pWindowItem );
				}
			}
		}
	}
}

void CCoinShopWindow::Load( class CCoinShop * pcCoinShop )
{
	// Set Coin
	SetCoinAmount( COINHANDLER->IsTimeShop() ? ACCOUNTHANDLER->GetTimePointsAmount() : ACCOUNTHANDLER->GetCoinAmount(), !COINHANDLER->IsTimeShop() );

	ChatGame::AddChatBoxTextEx( CHATCOLOR_Global, "> %s:", pcCoinShop->GetName() );
	ChatGame::AddChatBoxTextEx( CHATCOLOR_Global, "> %s", pcCoinShop->GetMessage() );

	// Clear Tabs
	ClearParentTabList();

	//Add Tabs to Parent Tab List
	std::vector<CCoinShopTab*> & vTabs = pcCoinShop->GetTabs();
	for( std::vector<CCoinShopTab*>::iterator it = vTabs.begin(); it != vTabs.end(); it++ )
	{
		CCoinShopTab * pcTab = *it;

		AddTab( pcTab );
	}
}

void CCoinShopWindow::BuildItemNormal( class CCoinShopItem * pcItem, BOOL bSpec, BOOL bQuantity )
{
	if ( pcItem )
	{
		auto pListItems = pWindow1->GetElement<UI::List>( LISTID_Items );
		if ( pListItems )
		{
			UI::Image * pItemImage = UI::ImageLoader::LoadImage( pcItem->GetImage() );
			if ( pItemImage )
			{
				int iImageWidth = pItemImage->GetWidth();
				int iImageHeight = pItemImage->GetHeight();

				int iImagePaddingX = 44 - (iImageWidth >> 1);
				int iImagePaddingY = 44 - (iImageHeight >> 1);

				// Create Window of Item
				UI::Window_ptr pWindowItem = pWindow1->GetElement<UI::Window>( WINDOWID_ItemData );

				UI::ImageBox_ptr pItemImageBox = std::make_shared<UI::ImageBox>( Rectangle2D( iImagePaddingX + 32, iImagePaddingY + 26, iImageWidth, iImageHeight ) );
				pItemImageBox->SetImage( pItemImage );
				pWindowItem->AddElement( pItemImageBox );

				//Preview of Head Model
				if( pcItem->GetTabID() == 52 || pcItem->GetTabID() == 25 )
					BuildPreviewHead( pWindowItem );

				//Preview of Costume Model
				if( pcItem->GetTabID() == 49 || pcItem->GetTabID() == 50 )
				{
					bool bPreviewCostume = false;
					int iGender = ITEMHANDLER->GetItemGender( GetItemDefinition()->sItem.sItemID.ToInt() );

					if( iGender == UNITDATA->GetCharacterGender() )
						bPreviewCostume = true;
					if( iGender == 0 )
						bPreviewCostume = true;

					if( bPreviewCostume )
					{
						BuildPreviewCostume( pWindowItem );

						UI::CheckBox_ptr pCheckBoxPreview = std::make_shared<UI::CheckBox>( Rectangle2D( 30, 225, 13, 13 ) );
						pCheckBoxPreview->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
						pCheckBoxPreview->SetText( "Preview Costume" );
						pCheckBoxPreview->SetEvent( UI::Event::Build( std::bind( &CCoinShopWindow::OnCheckPreviewCostume, this, pCheckBoxPreview ) ) );
						pCheckBoxPreview->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
						pWindowItem->AddElement( pCheckBoxPreview );
					}
				}

				// Buy Button
				UI::Button_ptr pButtonBuy = std::make_shared<UI::Button>( Rectangle2D( 30, 266 + 24, 75, 26 ) );
				pButtonBuy->SetID( BUTTONID_Buy );
				pButtonBuy->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\coinshop\\btnbuy.png", "game\\images\\coinshop\\btnbuy_.png" ) );
				pButtonBuy->SetEvent( UI::Event::Build( std::bind( &CCoinShopWindow::OnBuyButtonClick, this, pcItem, std::placeholders::_1 ) ) );
				pWindowItem->AddElement( pButtonBuy );

				// Quantity
				if ( bQuantity )
				{
					// Spec Quantity
					UI::Text_ptr pItemDescCount = std::make_shared<UI::Text>( Rectangle2D( 32, 204, 240, 16 ) );
					pItemDescCount->SetText( "Quantity:" );
					pItemDescCount->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
					pWindowItem->AddElement( pItemDescCount );

					pcItem->SetIsQuantity( TRUE );
					UI::Counter_ptr pCounter = std::make_shared<UI::Counter>( Rectangle2D( 32, 222, 80, 26 ) );
					pCounter->SetID( COUNTID_Quantity );
					pCounter->SetMinMax( 1, 100 );
					pCounter->SetEvent( UI::Event::Build( std::bind( &CCoinShopWindow::OnChangeQuantityItem, this, pcItem, std::placeholders::_1 ) ) );
					pCounter->SetImage( UI::ImageLoader::LoadImage( "game\\images\\coinshop\\counter.png" ) );
					pCounter->SetButtonDecrease( Rectangle2D( 41, 0, 19, 26 ), UI::ImageLoader::LoadImage( "game\\images\\coinshop\\counter_down.png" ) );
					pCounter->SetButtonIncrease( Rectangle2D( 61, 0, 19, 26 ), UI::ImageLoader::LoadImage( "game\\images\\coinshop\\counter_up.png" ) );
					pCounter->GetInput()->SetBox( Rectangle2D( 0, 0, 42, 26 ) );
					pCounter->GetInput()->SetSelect( TRUE );
					pCounter->GetInput()->SetTextAlign( ALIGN_Center, ALIGN_Middle );
					pWindowItem->AddElement( pCounter );
				}

				// Commify Price
				int iPrice = pcItem->GetDiscount() > 0 ? pcItem->GetDiscount() : pcItem->GetValue();

				// Price Description
				UI::Text_ptr pItemDescPrice = std::make_shared<UI::Text>( Rectangle2D( 30, 256, 240, 18 ) );
				pItemDescPrice->SetText( "PRICE:" );
				pItemDescPrice->SetFont( "Arial", 18, 0, FALSE, TRUE, -1 );
				pWindowItem->AddElement( pItemDescPrice );

				// Price Value
				UI::Text_ptr pItemValuePrice = std::make_shared<UI::Text>( Rectangle2D( 100, 256, 240, 16 ) );
				pItemValuePrice->SetID( LABELID_Price );
				pItemValuePrice->SetText( FormatNumber(iPrice) );
				pItemValuePrice->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
				pWindowItem->AddElement( pItemValuePrice );

				// Price Discount or Bulk
				if ( pcItem->GetBulk() || pcItem->GetDiscount() )
				{
					UI::Text_ptr pItemDiscount = std::make_shared<UI::Text>( Rectangle2D( 30, 256 + 20, 240, 16 ) );
					pItemDiscount->SetID( LABELID_DiscountBulk );
					pItemDiscount->SetText( "" );
					if ( pcItem->GetDiscount() )
						pItemDiscount->SetText( FormatString( "Discount: %d", pcItem->GetDiscount() ) );
					pItemDiscount->SetFont( "Arial", 12, 0, FALSE, FALSE, -1 );
					pWindowItem->AddElement( pItemDiscount );
				}

				// Spec Select
				if ( bSpec )
				{
					// Spec Label
					UI::Text_ptr pItemDescSpec = std::make_shared<UI::Text>( Rectangle2D( 32, 128, 240, 14 ) );
					pItemDescSpec->SetText( "Spec Item:" );
					pItemDescSpec->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
					pWindowItem->AddElement( pItemDescSpec );

					UI::ComboBox_ptr pComboBox = std::make_shared<UI::ComboBox>( Rectangle2D( 32, 146, 110, 26 ) );
					pComboBox->SetDefaultText( "Select" );
					pComboBox->SetID( COMBOBOX_Spec );
					pComboBox->SetBackgroundImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\combobox\\drop.png", "game\\images\\UI\\combobox\\dropon.png" ) );
					pComboBox->SetBackgroundSelectedImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\combobox\\drop_.png" ) );
					pComboBox->SetBackgroundActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\combobox\\dropbg.png", "game\\images\\UI\\combobox\\dropdown.png" ) );
					pComboBox->SetHoverColor( D3DCOLOR_ARGB( 150, 170, 150, 120 ) );
					//Options
					{
						pComboBox->AddOption( pComboBox->MakeOption( "Fighter", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ) );
						pComboBox->AddOption( pComboBox->MakeOption( "Mechanician", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ) );
						pComboBox->AddOption( pComboBox->MakeOption( "Archer", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ) );
						pComboBox->AddOption( pComboBox->MakeOption( "Pike", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ) );
						pComboBox->AddOption( pComboBox->MakeOption( "Atalanta", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ) );
						pComboBox->AddOption( pComboBox->MakeOption( "Knight", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ) );
						pComboBox->AddOption( pComboBox->MakeOption( "Mage", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ) );
						pComboBox->AddOption( pComboBox->MakeOption( "Priestess", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ) );
						pComboBox->AddOption( pComboBox->MakeOption( "Assassin", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ) );
						pComboBox->AddOption( pComboBox->MakeOption( "Shaman", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ) );
					}
					pComboBox->SetChangeEvent( UI::Event::Build( std::bind( &CCoinShopWindow::OnChangeSpecSelect, this, pcItem, std::placeholders::_1 ) ) );
					pWindowItem->AddElement( pComboBox );
				}
			}
		}
	}
}

void CCoinShopWindow::BuildPerfectize( BOOL bSpec )
{
	auto pListItems = pWindow1->GetElement<UI::List>( LISTID_Items );

	if ( pListItems )
	{
		// Create Window of Item
		UI::Window_ptr pWindowItem = pWindow1->GetElement<UI::Window>( WINDOWID_ItemData );

		if ( pWindowItem )
		{
			UI::ImageBox_ptr pImageSlot = std::make_shared<UI::ImageBox>( Rectangle2D( 30, 30, 68, 90 ) );
			pImageSlot->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\slot.png" ) );
			pWindowItem->AddElement( pImageSlot );

			UI::ItemBox_ptr pItemBox = std::make_shared<UI::ItemBox>( 30, 30, 3, 4 );
			pItemBox->SetID( ITEMBOX_Perfectize );
			pItemBox->SetMaxItems( 1 );
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Allow, UI::ItemBox::ETypeRule::TYPERULE_ItemBase, ITEMBASE_Weapon );
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Allow, UI::ItemBox::ETypeRule::TYPERULE_ItemType, ITEMTYPE_Ring );
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Allow, UI::ItemBox::ETypeRule::TYPERULE_ItemType, ITEMTYPE_Ring2 );
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Allow, UI::ItemBox::ETypeRule::TYPERULE_ItemType, ITEMTYPE_Bracelets );
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Allow, UI::ItemBox::ETypeRule::TYPERULE_ItemType, ITEMTYPE_Amulet );
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Allow, UI::ItemBox::ETypeRule::TYPERULE_ItemType, ITEMTYPE_Sheltom );
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Allow, UI::ItemBox::ETypeRule::TYPERULE_ItemType, ITEMTYPE_Orb );
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Allow, UI::ItemBox::ETypeRule::TYPERULE_ItemBase, ITEMBASE_Defense );
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Allow, UI::ItemBox::ETypeRule::TYPERULE_ItemType, ITEMTYPE_Earring );
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
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_HopyCostumeTime );
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_CuepyCostumeTime );
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_HopyCostumeIce );
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_CuepyCostumeIce );

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

			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_CandySheltom );

			//Spec Stone
			{
				pItemBox->AddRule( UI::ItemBox::ERule::RULE_Allow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SpecStoneFS );
				pItemBox->AddRule( UI::ItemBox::ERule::RULE_Allow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SpecStoneMS );
				pItemBox->AddRule( UI::ItemBox::ERule::RULE_Allow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SpecStoneAS );
				pItemBox->AddRule( UI::ItemBox::ERule::RULE_Allow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SpecStonePS );
				pItemBox->AddRule( UI::ItemBox::ERule::RULE_Allow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SpecStoneATA );
				pItemBox->AddRule( UI::ItemBox::ERule::RULE_Allow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SpecStoneKS );
				pItemBox->AddRule( UI::ItemBox::ERule::RULE_Allow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SpecStoneMG );
				pItemBox->AddRule( UI::ItemBox::ERule::RULE_Allow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SpecStonePRS );
				pItemBox->AddRule( UI::ItemBox::ERule::RULE_Allow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SpecStoneASS );
				pItemBox->AddRule( UI::ItemBox::ERule::RULE_Allow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_SpecStoneSHA );
			}
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_QuestItem, 0 );
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemType, ITEMTYPE_Costume );
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemType, ITEMTYPE_Costume2 );
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemPerfect, TRUE );
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_EquippedItem, TRUE );
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_CostumeFlag, FALSE );
			pItemBox->SetEventOnDeniedItem( UI::Event::Build( std::bind( &CCoinShopWindow::OnItemNotAllowedPerfectize, this, std::placeholders::_1 ) ) );
			pItemBox->SetEventOnPutItem( UI::Event::Build( std::bind( &CCoinShopWindow::OnPutItemPerfectize, this, std::placeholders::_1 ) ) );
			pItemBox->SetEventOnGetItem( UI::Event::Build( std::bind( &CCoinShopWindow::OnGetItemPerfectize, this, std::placeholders::_1 ) ) );
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_LockItemGM, 0 );
			pWindowItem->AddElement( pItemBox );

			// Spec Select
			if ( bSpec )
			{
				
				// Buy Button
				UI::Button_ptr pButtonBuy = std::make_shared<UI::Button>( Rectangle2D( 30, 266 + 24, 75, 26 ) );
				pButtonBuy->SetID( BUTTONID_Buy );
				pButtonBuy->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\coinshop\\btnbuy.png", "game\\images\\coinshop\\btnbuy_.png" ) );
				pButtonBuy->SetEvent( UI::Event::Build( std::bind( &CCoinShopWindow::OnBuyServiceButtonClick, this, ITEMCOIN_ItemPerfect, std::placeholders::_1 ) ) );
				pWindowItem->AddElement( pButtonBuy );

				// Commify Price
				int iPrice = 0;

				// Price Description
				UI::Text_ptr pItemDescPrice = std::make_shared<UI::Text>( Rectangle2D( 30, 256, 240, 18 ) );
				pItemDescPrice->SetText( "PRICE:" );
				pItemDescPrice->SetFont( "Arial", 18, 0, FALSE, TRUE, -1 );
				pWindowItem->AddElement( pItemDescPrice );

				// Price Value
				UI::Text_ptr pItemValuePrice = std::make_shared<UI::Text>( Rectangle2D( 100, 256, 240, 16 ) );
				pItemValuePrice->SetID( LABELID_Price );
				pItemValuePrice->SetText( FormatNumber( iPrice ) );
				pItemValuePrice->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
				pWindowItem->AddElement( pItemValuePrice );	
			}
		}

	}
}

void CCoinShopWindow::BuildSwapper( BOOL bSpec )
{
	auto pListItems = pWindow1->GetElement<UI::List>( LISTID_Items );

	if ( pListItems )
	{
		// Create Window of Item
		UI::Window_ptr pWindowItem = pWindow1->GetElement<UI::Window>( WINDOWID_ItemData );

		if ( pWindowItem )
		{
			UI::ImageBox_ptr pImageSlot = std::make_shared<UI::ImageBox>( Rectangle2D( 30, 30, 68, 90 ) );
			pImageSlot->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\slot.png" ) );
			pWindowItem->AddElement( pImageSlot );

			UI::ItemBox_ptr pItemBox = std::make_shared<UI::ItemBox>( 30, 30, 3, 4 );
			pItemBox->SetID( ITEMBOX_Swapper );
			pItemBox->SetMaxItems( 1 );
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Allow, UI::ItemBox::ETypeRule::TYPERULE_ItemBase, ITEMBASE_Weapon );
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Allow, UI::ItemBox::ETypeRule::TYPERULE_ItemType, ITEMTYPE_Bracelets );
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Allow, UI::ItemBox::ETypeRule::TYPERULE_ItemType, ITEMTYPE_Orb );
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Allow, UI::ItemBox::ETypeRule::TYPERULE_ItemBase, ITEMBASE_Defense );
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_QuestItem, 0 );
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemType, ITEMTYPE_Costume );
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemType, ITEMTYPE_Costume2 );
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemType, ITEMTYPE_Sheltom );
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemPerfect, FALSE );
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_EquippedItem, TRUE );
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_DraxosBoots );
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_MokovaBoots );
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
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_HopyCostumeTime );
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_CuepyCostumeTime );
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_HopyCostumeIce );
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_CuepyCostumeIce );
			pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemID, ITEMID_CandySheltom );
			pItemBox->SetEventOnDeniedItem( UI::Event::Build( std::bind( &CCoinShopWindow::OnItemNotAllowedSwapper, this, std::placeholders::_1 ) ) );
			pItemBox->SetEventOnPutItem( UI::Event::Build( std::bind( &CCoinShopWindow::OnPutItemSwapper, this, std::placeholders::_1 ) ) );
			pItemBox->SetEventOnGetItem( UI::Event::Build( std::bind( &CCoinShopWindow::OnGetItemSwapper, this, std::placeholders::_1 ) ) );
			pWindowItem->AddElement( pItemBox );

			{
				
				// Buy Button
				UI::Button_ptr pButtonBuy = std::make_shared<UI::Button>( Rectangle2D( 30, 266 + 24, 75, 26 ) );
				pButtonBuy->SetID( BUTTONID_Buy );
				pButtonBuy->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\coinshop\\btnbuy.png", "game\\images\\coinshop\\btnbuy_.png" ) );
				pButtonBuy->SetEvent( UI::Event::Build( std::bind( &CCoinShopWindow::OnBuyServiceButtonClick, this, ITEMCOIN_ItemSwapper, std::placeholders::_1 ) ) );
				pWindowItem->AddElement( pButtonBuy );

				// Swap Label
				UI::Text_ptr pItemDescSwap = std::make_shared<UI::Text>( Rectangle2D( 32 + 180, 128, 240, 14 ) );
				pItemDescSwap->SetText( "Swap Item:" );
				pItemDescSwap->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
				pWindowItem->AddElement( pItemDescSwap );

				UI::ComboBox_ptr pComboBoxSwap = std::make_shared<UI::ComboBox>( Rectangle2D( 32 + 180, 146, 110, 26 ) );
				pComboBoxSwap->SetDefaultText( "Select" );
				pComboBoxSwap->SetID( COMBOBOX_Swap );
				pComboBoxSwap->SetBackgroundImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\combobox\\drop.png", "game\\images\\UI\\combobox\\dropon.png" ) );
				pComboBoxSwap->SetBackgroundSelectedImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\combobox\\drop_.png" ) );
				pComboBoxSwap->SetBackgroundActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\combobox\\dropbg.png", "game\\images\\UI\\combobox\\dropdown.png" ) );
				pComboBoxSwap->SetHoverColor( D3DCOLOR_ARGB( 150, 170, 150, 120 ) );

				//Options
				{
					pComboBoxSwap->AddOption( pComboBoxSwap->MakeOption( "Axe", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ), ITEMTYPE_Axe );
					pComboBoxSwap->AddOption( pComboBoxSwap->MakeOption( "Claw", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ), ITEMTYPE_Claw );
					pComboBoxSwap->AddOption( pComboBoxSwap->MakeOption( "Sword", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ), ITEMTYPE_Sword );
					pComboBoxSwap->AddOption( pComboBoxSwap->MakeOption( "Hammer", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ), ITEMTYPE_Hammer );
					pComboBoxSwap->AddOption( pComboBoxSwap->MakeOption( "Javelin", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ), ITEMTYPE_Javelin );
					pComboBoxSwap->AddOption( pComboBoxSwap->MakeOption( "Scythe", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ), ITEMTYPE_Scythe );
					pComboBoxSwap->AddOption( pComboBoxSwap->MakeOption( "Wand/Staff", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ), ITEMTYPE_Wand );
					pComboBoxSwap->AddOption( pComboBoxSwap->MakeOption( "Bow", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ), ITEMTYPE_Bow );
					pComboBoxSwap->AddOption( pComboBoxSwap->MakeOption( "Phantom", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ), ITEMTYPE_Phantom );
					pComboBoxSwap->AddOption( pComboBoxSwap->MakeOption( "Dagger", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ), ITEMTYPE_Dagger );
					pComboBoxSwap->AddOption( pComboBoxSwap->MakeOption( "Armor", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ), ITEMTYPE_Armor );
					pComboBoxSwap->AddOption( pComboBoxSwap->MakeOption( "Robe", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ), ITEMTYPE_Robe );
					pComboBoxSwap->AddOption( pComboBoxSwap->MakeOption( "Bracelet", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ), ITEMTYPE_Bracelets );
					pComboBoxSwap->AddOption( pComboBoxSwap->MakeOption( "Gauntlet", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ), ITEMTYPE_Gauntlets );
					pComboBoxSwap->AddOption( pComboBoxSwap->MakeOption( "Boot", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ), ITEMTYPE_Boots );
					pComboBoxSwap->AddOption( pComboBoxSwap->MakeOption( "Shield", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ), ITEMTYPE_Shield );
					pComboBoxSwap->AddOption( pComboBoxSwap->MakeOption( "Orb", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ), ITEMTYPE_Orb );
				}


				iSelectedSwap = -1;

				pComboBoxSwap->SetChangeEvent( UI::Event::Build( std::bind( &CCoinShopWindow::OnChangeSwapSelect, this, (CCoinShopItem*)NULL, std::placeholders::_1 ) ) );

				pWindowItem->AddElement( pComboBoxSwap );

				// Commify Price
				int iPrice = 0;

				// Price Description
				UI::Text_ptr pItemDescPrice = std::make_shared<UI::Text>( Rectangle2D( 30, 256, 240, 18 ) );
				pItemDescPrice->SetText( "PRICE:" );
				pItemDescPrice->SetFont( "Arial", 18, 0, FALSE, TRUE, -1 );
				pWindowItem->AddElement( pItemDescPrice );

				// Price Value
				UI::Text_ptr pItemValuePrice = std::make_shared<UI::Text>( Rectangle2D( 100, 256, 240, 16 ) );
				pItemValuePrice->SetID( LABELID_Price );
				pItemValuePrice->SetText( FormatNumber( iPrice ) );
				pItemValuePrice->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
				pWindowItem->AddElement( pItemValuePrice );
			}
		}

	}
}

void CCoinShopWindow::BuildLevelUP()
{
	auto pListItems = pWindow1->GetElement<UI::List>( LISTID_Items );

	if ( pListItems )
	{
		// Create Window of Item
		UI::Window_ptr pWindowItem = pWindow1->GetElement<UI::Window>( WINDOWID_ItemData );

		if ( pWindowItem )
		{
			UI::Image * pItemImage = UI::ImageLoader::LoadImage( "game\\images\\coinshop\\levelup.png" );
			if ( pItemImage )
			{
				int iImageWidth = pItemImage->GetWidth();
				int iImageHeight = pItemImage->GetHeight();

				int iImagePaddingX = 44 - (iImageWidth >> 1);
				int iImagePaddingY = 44 - (iImageHeight >> 1);

				UI::ImageBox_ptr pItemImageBox = std::make_shared<UI::ImageBox>( Rectangle2D( iImagePaddingX + 32, iImagePaddingY + 26, iImageWidth, iImageHeight ) );
				pItemImageBox->SetImage( pItemImage );
				pWindowItem->AddElement( pItemImageBox );

				UI::Text_ptr pItemDesc = std::make_shared<UI::Text>( Rectangle2D( 160, 28, 240, 34 ) );
				if ( Base::Coin::CanLevelUP( UNITDATA->sCharacterData.iLevel ) )
					pItemDesc->SetText( "Level UP your Character!" );
				else
					pItemDesc->SetText( "You have already reached \nthe Level UP limit!" );

				pItemDesc->SetMultiLine( TRUE );
				pItemDesc->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
				pWindowItem->AddElement( pItemDesc );

			}

			if ( Base::Coin::CanLevelUP( UNITDATA->sCharacterData.iLevel ) )
			{
				UI::Text_ptr pItemDescCount = std::make_shared<UI::Text>( Rectangle2D( 32, 204, 240, 16 ) );
				pItemDescCount->SetText( "Level:" );
				pItemDescCount->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
				pWindowItem->AddElement( pItemDescCount );

				UI::Counter_ptr pCounter = std::make_shared<UI::Counter>( Rectangle2D( 32, 222, 80, 26 ) );
				pCounter->SetID( COUNTID_Quantity );
				pCounter->SetMinMax( UNITDATA->sCharacterData.iLevel + 1, MAX_LEVEL_COINSHOP );
				pCounter->SetEvent( UI::Event::Build( std::bind( &CCoinShopWindow::OnChangeLevelUPItem, this, std::placeholders::_1 ) ) );
				pCounter->SetImage( UI::ImageLoader::LoadImage( "game\\images\\coinshop\\counter.png" ) );
				pCounter->SetButtonDecrease( Rectangle2D( 41, 0, 19, 26 ), UI::ImageLoader::LoadImage( "game\\images\\coinshop\\counter_down.png" ) );
				pCounter->SetButtonIncrease( Rectangle2D( 61, 0, 19, 26 ), UI::ImageLoader::LoadImage( "game\\images\\coinshop\\counter_up.png" ) );
				pCounter->GetInput()->SetBox( Rectangle2D( 0, 0, 42, 26 ) );
				pCounter->GetInput()->SetSelect( TRUE );
				pCounter->GetInput()->SetTextAlign( ALIGN_Center, ALIGN_Middle );
				pWindowItem->AddElement( pCounter );

				// Buy Button
				UI::Button_ptr pButtonBuy = std::make_shared<UI::Button>( Rectangle2D( 30, 266 + 24, 75, 26 ) );
				pButtonBuy->SetID( BUTTONID_Buy );
				pButtonBuy->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\coinshop\\btnbuy.png", "game\\images\\coinshop\\btnbuy_.png" ) );
				pButtonBuy->SetEvent( UI::Event::Build( std::bind( &CCoinShopWindow::OnBuyServiceButtonClick, this, ITEMCOIN_LevelUP, std::placeholders::_1 ) ) );
				pWindowItem->AddElement( pButtonBuy );

				// Price Description
				UI::Text_ptr pItemDescPrice = std::make_shared<UI::Text>( Rectangle2D( 30, 256, 240, 18 ) );
				pItemDescPrice->SetText( "PRICE:" );
				pItemDescPrice->SetFont( "Arial", 18, 0, FALSE, TRUE, -1 );
				pWindowItem->AddElement( pItemDescPrice );

				// Price Value
				UI::Text_ptr pItemValuePrice = std::make_shared<UI::Text>( Rectangle2D( 100, 256, 240, 16 ) );
				pItemValuePrice->SetID( LABELID_Price );
				int iLevelUPPrice = Base::Coin::GetLevelUPPrice( UNITDATA->sCharacterData.iLevel, UNITDATA->sCharacterData.iLevel + 1 );
				pItemValuePrice->SetText( FormatNumber( iLevelUPPrice ) );
				pItemValuePrice->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
				pWindowItem->AddElement( pItemValuePrice );
			}
		}
	}
}

void CCoinShopWindow::BuildChangeNick( int iPrice )
{
	auto pListItems = pWindow1->GetElement<UI::List>( LISTID_Items );

	if ( pListItems )
	{
		// Create Window of Item
		UI::Window_ptr pWindowItem = pWindow1->GetElement<UI::Window>( WINDOWID_ItemData );

		if ( pWindowItem )
		{
			UI::Image * pItemImage = UI::ImageLoader::LoadImage( "image\\Sinimage\\Items\\Event\\Itsp111.bmp" );
			if ( pItemImage )
			{
				int iImageWidth = pItemImage->GetWidth();
				int iImageHeight = pItemImage->GetHeight();

				int iImagePaddingX = 44 - (iImageWidth >> 1);
				int iImagePaddingY = 44 - (iImageHeight >> 1);

				UI::ImageBox_ptr pItemImageBox = std::make_shared<UI::ImageBox>( Rectangle2D( iImagePaddingX + 32, iImagePaddingY + 26, iImageWidth, iImageHeight ) );
				pItemImageBox->SetImage( pItemImage );
				pWindowItem->AddElement( pItemImageBox );

				UI::Text_ptr pItemDesc = std::make_shared<UI::Text>( Rectangle2D( 160, 28, 240, 34 ) );
				pItemDesc->SetText( "Change Character Name\nYou must relog after buying" );
				pItemDesc->SetMultiLine( TRUE );
				pItemDesc->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
				pWindowItem->AddElement( pItemDesc );

			}

			{
				// Buy Button
				UI::Button_ptr pButtonBuy = std::make_shared<UI::Button>( Rectangle2D( 30, 266 + 24, 75, 26 ) );
				pButtonBuy->SetID( BUTTONID_Buy );
				pButtonBuy->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\coinshop\\btnbuy.png", "game\\images\\coinshop\\btnbuy_.png" ) );
				pButtonBuy->SetEvent( UI::Event::Build( std::bind( &CCoinShopWindow::OnBuyServiceButtonClick, this, ITEMCOIN_ChangeNick, std::placeholders::_1 ) ) );
				pWindowItem->AddElement( pButtonBuy );

				// Swap Label
				UI::Text_ptr pItemDescSwap = std::make_shared<UI::Text>( Rectangle2D( 32, 128, 240, 14 ) );
				pItemDescSwap->SetText( "Change Nick:" );
				pItemDescSwap->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
				pWindowItem->AddElement( pItemDescSwap );

				UI::Group_ptr pGroup1 = std::make_shared<UI::Group>();
				pWindowItem->AddElement(pGroup1);

				UI::InputField_ptr pInput = std::make_shared<UI::InputField>( Rectangle2D( 32, 146, 160, 18 ) );
				pInput->SetID( INPUTID_ChangeNick );
				pInput->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
				pInput->SetText( "" );
				pInput->SetSelect(TRUE);
				pInput->SetSelected(TRUE);
				pInput->SetMaxLength( 15 );
				pInput->SetAllowedCharacters( ALLOWED_CHARS_NICK );
				pInput->SetTextAlign( EAlign::ALIGN_Left, EAlign::ALIGN_Middle );
				pInput->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\input\\minitextbox.png" ) );
				pInput->SetGroup(pGroup1);
				pWindowItem->AddElement( pInput );

				// Price Description
				UI::Text_ptr pItemDescPrice = std::make_shared<UI::Text>( Rectangle2D( 30, 256, 240, 18 ) );
				pItemDescPrice->SetText( "PRICE:" );
				pItemDescPrice->SetFont( "Arial", 18, 0, FALSE, TRUE, -1 );
				pWindowItem->AddElement( pItemDescPrice );

				// Price Value
				UI::Text_ptr pItemValuePrice = std::make_shared<UI::Text>( Rectangle2D( 100, 256, 240, 16 ) );
				pItemValuePrice->SetID( LABELID_Price );
				pItemValuePrice->SetText( FormatNumber( iPrice ) );
				pItemValuePrice->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
				pWindowItem->AddElement( pItemValuePrice );
			}
		}
	}
}

void CCoinShopWindow::BuildAgingRecovery( struct AgingRecoveryDataHandle * psaData, int iCount, int iPrice )
{
	UI::Window_ptr pWindowItem = pWindow1->GetElement<UI::Window>( WINDOWID_ItemData );

	auto pListItems = pWindow1->GetElement<UI::List>( LISTID_Items );

	if ( pWindowItem )
	{
		for ( int i = 0; i < iCount; i++ )
		{
			AgingRecoveryDataHandle * ps = psaData + i;

			// Create Window of Item
			UI::Window_ptr pWindowBG = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 324, 28 ) );
			pWindowBG->SetID( ps->iID );

			// Button Select
			UI::Button_ptr pButtonBuy = std::make_shared<UI::Button>( Rectangle2D( 0, 0, 324, 28 ) );
			pButtonBuy->SetEvent( UI::Event::Build( std::bind( &CCoinShopWindow::OnAgingRecoveryItemButtonClick, this, ps->iID, ps->iPrice, std::placeholders::_1 ) ) );
			pButtonBuy->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\coinshop\\fieldage.png", "game\\images\\coinshop\\fieldage_.png" ) );
			pWindowBG->AddElement( pButtonBuy );

			// # Items
			int iNumber = pListItems->GetCountItemsList() + 1;
			UI::Text_ptr pTextNumber = std::make_shared<UI::Text>( Rectangle2D( 0, 0, 36, 28 ) );
			pTextNumber->SetFont( "Arial", 14, 0, FALSE, FALSE, -1 );
			pTextNumber->SetTextEx( "%d", iNumber );
			pTextNumber->SetHorizontalAlign( EAlign::ALIGN_Center );
			pTextNumber->SetVerticalAlign( EAlign::ALIGN_Center );
			pWindowBG->AddElement( pTextNumber );

			// Name
			UI::Text_ptr pItemName = std::make_shared<UI::Text>( Rectangle2D( 36, 0, 186, 28 ) );
			pItemName->SetID( LABELID_Name );
			pItemName->SetTextEx( "%s +%d", ps->szItemName, ps->iAgeNumber );
			pItemName->SetColor( D3DCOLOR_ARGB( 255, 194, 165, 110 ) );
			pItemName->SetHorizontalAlign( EAlign::ALIGN_Center );
			pItemName->SetVerticalAlign( EAlign::ALIGN_Center );
			pWindowBG->AddElement( pItemName );

			// Date
			UI::Text_ptr pItemDate = std::make_shared<UI::Text>( Rectangle2D( 220, 0, 105, 28 ) );
			pItemDate->SetFont( "Arial", 15, 0, FALSE, FALSE, -1 );
			pItemDate->SetTextEx( "%02d/%02d/%d %02d:%02d", ps->sDate.wDay, ps->sDate.wMonth, ps->sDate.wYear, ps->sDate.wHour, ps->sDate.wMinute );
			pItemDate->SetMultiLine( TRUE );
			pItemDate->SetWordWrap( TRUE );
			pItemDate->SetColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			pItemDate->SetHorizontalAlign( EAlign::ALIGN_Center );
			pItemDate->SetVerticalAlign( EAlign::ALIGN_Center );
			pWindowBG->AddElement( pItemDate );

			pListItems->AddWindow( pWindowBG );
		}
	}
}

void CCoinShopWindow::BuildPreviewCostume( UI::Window_ptr pcWindowItem )
{
	if( pcWindowItem )
	{
		UI::Window_ptr pWindowPreview = std::make_shared<UI::Window>( Rectangle2D( 172, 72, 203, 250 ) );
		pWindowPreview->SetID( WINDOWID_ItemPreview );

		UI::Text_ptr pTextLabel = std::make_shared<UI::Text>( Rectangle2D( 0, 0, 0, 0 ) );
		pTextLabel->SetFont( "Arial", 15, 0, FALSE, TRUE, D3DCOLOR_ARGB( 255, 236, 236, 236 ) );
		pTextLabel->SetText( "Preview:" );
		pTextLabel->SetNoClip( TRUE );
		pWindowPreview->AddElement( pTextLabel );

		UI::ImageBox_ptr pImagePreview = std::make_shared<UI::ImageBox>( Rectangle2D( 0, 21, 150, 190 ) );
		pImagePreview->SetImage( UI::ImageLoader::LoadImage( "game\\images\\coinshop\\previewcostume.png" ) );
		pWindowPreview->AddElement( pImagePreview );

		UI::UnitMesh_ptr pUnitMeshPreview = std::make_shared<UI::UnitMesh>( Rectangle2D( 1, 23, 150, 187 ) );
		pUnitMeshPreview->SetID( UNITMESHID_HeadPreview );
		pUnitMeshPreview->SetRenderBody( TRUE );
		pUnitMeshPreview->PrepareUnitData( UNITDATA );
		pUnitMeshPreview->SetHeadModel( UNITDATA->sCharacterData.Player.szHeadModel );
		pUnitMeshPreview->SetBodyModel( ITEMHANDLER->GetArmorPatternName( UNITDATA, GetItemDefinition()->sItem.sItemID.ToInt() ) );
		pUnitMeshPreview->SetCameraPosition( Point3D( pUnitMeshPreview->GetCameraPosition().iX, pUnitMeshPreview->GetCameraPosition().iY - (2 << 8), pUnitMeshPreview->GetCameraPosition().iZ ) );
		pWindowPreview->AddElement( pUnitMeshPreview );

		UI::Button_ptr pRotateRightButton = std::make_shared<UI::Button>( Rectangle2D( 43, 220, 17, 18 ) );
		pRotateRightButton->SetCanHold( TRUE );
		pRotateRightButton->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\coinshop\\btnrotateright.png", "game\\images\\coinshop\\btnrotateright_.png" ) );
		pRotateRightButton->SetEvent( UI::Event::Build( std::bind( &CCoinShopWindow::OnRotateUnitMesh, this, 1 ) ) );
		pWindowPreview->AddElement( pRotateRightButton );

		UI::Button_ptr pRotateLeftButton = std::make_shared<UI::Button>( Rectangle2D( 92, 220, 17, 18 ) );
		pRotateLeftButton->SetCanHold( TRUE );
		pRotateLeftButton->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\coinshop\\btnrotateleft.png", "game\\images\\coinshop\\btnrotateleft_.png" ) );
		pRotateLeftButton->SetEvent( UI::Event::Build( std::bind( &CCoinShopWindow::OnRotateUnitMesh, this, 2 ) ) );
		pWindowPreview->AddElement( pRotateLeftButton );

		pWindowPreview->Hide();

		pcWindowItem->AddElement( pWindowPreview );
	}
}

void CCoinShopWindow::BuildPreviewHead( UI::Window_ptr pcWindowItem )
{
	if( pcWindowItem )
	{
		UI::Window_ptr pWindowPreview = std::make_shared<UI::Window>( Rectangle2D( 172, 72, 203, 250 ) );
		pWindowPreview->SetID( WINDOWID_ItemPreview );

		UI::Text_ptr pTextLabel = std::make_shared<UI::Text>( Rectangle2D( 0, 41, 0, 0 ) );
		pTextLabel->SetFont( "Arial", 15, 0, FALSE, TRUE, D3DCOLOR_ARGB( 255, 236, 236, 236 ) );
		pTextLabel->SetText( "Preview:" );
		pTextLabel->SetNoClip( TRUE );
		pWindowPreview->AddElement( pTextLabel );

		UI::ImageBox_ptr pImagePreview = std::make_shared<UI::ImageBox>( Rectangle2D( 0, 62, 102, 102 ) );
		pImagePreview->SetImage( UI::ImageLoader::LoadImage( "game\\images\\coinshop\\previewhair.png" ) );
		pWindowPreview->AddElement( pImagePreview );

		UI::UnitMesh_ptr pUnitMeshPreview = std::make_shared<UI::UnitMesh>( Rectangle2D( 1, 64, 100, 99 ) );
		pUnitMeshPreview->SetID( UNITMESHID_HeadPreview );
		pUnitMeshPreview->SetRenderBody( FALSE );
		pUnitMeshPreview->PrepareUnitData( UNITDATA );
		pUnitMeshPreview->SetHeadModel( ITEMHANDLER->GetHeadModel( GetItemDefinition()->sItem.sItemID.ToInt() ) );
		pUnitMeshPreview->SetCameraPosition( Point3D( pUnitMeshPreview->GetCameraPosition().iX, pUnitMeshPreview->GetCameraPosition().iY + (1 << 8), pUnitMeshPreview->GetCameraPosition().iZ ) );
		pWindowPreview->AddElement( pUnitMeshPreview );

		UI::Button_ptr pRotateRightButton = std::make_shared<UI::Button>( Rectangle2D( 18, 172, 17, 18 ) );
		pRotateRightButton->SetCanHold( TRUE );
		pRotateRightButton->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\coinshop\\btnrotateright.png", "game\\images\\coinshop\\btnrotateright_.png" ) );
		pRotateRightButton->SetEvent( UI::Event::Build( std::bind( &CCoinShopWindow::OnRotateUnitMesh, this, 1 ) ) );
		pWindowPreview->AddElement( pRotateRightButton );

		UI::Button_ptr pRotateLeftButton = std::make_shared<UI::Button>( Rectangle2D( 67, 172, 17, 18 ) );
		pRotateLeftButton->SetCanHold( TRUE );
		pRotateLeftButton->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\coinshop\\btnrotateleft.png", "game\\images\\coinshop\\btnrotateleft_.png" ) );
		pRotateLeftButton->SetEvent( UI::Event::Build( std::bind( &CCoinShopWindow::OnRotateUnitMesh, this, 2 ) ) );
		pWindowPreview->AddElement( pRotateLeftButton );

		pcWindowItem->AddElement( pWindowPreview );
	}
}

void CCoinShopWindow::BuildChangeClass( int iPrice )
{
	auto pListItems = pWindow1->GetElement<UI::List>( LISTID_Items );

	if ( pListItems )
	{
		// Create Window of Item
		UI::Window_ptr pWindowItem = pWindow1->GetElement<UI::Window>( WINDOWID_ItemData );

		if ( pWindowItem )
		{

			UI::Image * pItemImage = UI::ImageLoader::LoadImage( "image\\Sinimage\\Items\\Event\\Itsp111.bmp" );
			if ( pItemImage )
			{
				int iImageWidth = pItemImage->GetWidth();
				int iImageHeight = pItemImage->GetHeight();

				int iImagePaddingX = 44 - (iImageWidth >> 1);
				int iImagePaddingY = 44 - (iImageHeight >> 1);

				UI::ImageBox_ptr pItemImageBox = std::make_shared<UI::ImageBox>( Rectangle2D( iImagePaddingX + 32, iImagePaddingY + 26, iImageWidth, iImageHeight ) );
				pItemImageBox->SetImage( pItemImage );
				pWindowItem->AddElement( pItemImageBox );

				UI::Text_ptr pItemDesc = std::make_shared<UI::Text>( Rectangle2D( 160, 28, 240, 34 ) );
				pItemDesc->SetText( "Change Character Class\nYou must relog after buying" );
				pItemDesc->SetMultiLine( TRUE );
				pItemDesc->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
				pWindowItem->AddElement( pItemDesc );

			}

			{
				// Buy Button
				UI::Button_ptr pButtonBuy = std::make_shared<UI::Button>( Rectangle2D( 30, 266 + 24, 75, 26 ) );
				pButtonBuy->SetID( BUTTONID_Buy );
				pButtonBuy->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\coinshop\\btnbuy.png", "game\\images\\coinshop\\btnbuy_.png" ) );
				pButtonBuy->SetEvent( UI::Event::Build( std::bind( &CCoinShopWindow::OnBuyServiceButtonClick, this, ITEMCOIN_ChangeClass, std::placeholders::_1 ) ) );
				pWindowItem->AddElement( pButtonBuy );

				// Swap Label
				UI::Text_ptr pItemDescNick = std::make_shared<UI::Text>( Rectangle2D( 32, 128, 240, 14 ) );
				pItemDescNick->SetText( "Change Nick:" );
				pItemDescNick->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
				pWindowItem->AddElement( pItemDescNick );

				UI::Group_ptr pGroup1 = std::make_shared<UI::Group>();
				pWindowItem->AddElement(pGroup1);

				UI::InputField_ptr pInput = std::make_shared<UI::InputField>( Rectangle2D( 32, 146, 160, 18 ) );
				pInput->SetID( INPUTID_ChangeNick );
				pInput->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
				pInput->SetText( "" );
				pInput->SetSelect(TRUE);
				pInput->SetSelected(TRUE);
				pInput->SetMaxLength( 15 );
				pInput->SetAllowedCharacters( ALLOWED_CHARS_NICK );
				pInput->SetTextAlign( EAlign::ALIGN_Left, EAlign::ALIGN_Middle );
				pInput->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\input\\minitextbox.png" ) );
				pInput->SetGroup(pGroup1);
				pWindowItem->AddElement( pInput );

				// Swap Label
				UI::Text_ptr pItemDescClass = std::make_shared<UI::Text>( Rectangle2D( 32 + 180, 128, 240, 14 ) );
				pItemDescClass->SetText( "Change Class:" );
				pItemDescClass->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
				pWindowItem->AddElement( pItemDescClass );

				UI::ComboBox_ptr pComboBoxClass = std::make_shared<UI::ComboBox>( Rectangle2D( 32 + 180, 146, 110, 26 ) );
				pComboBoxClass->SetDefaultText( "Select" );
				pComboBoxClass->SetID( COMBOBOX_ClassChange );
				pComboBoxClass->SetBackgroundImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\combobox\\drop.png", "game\\images\\UI\\combobox\\dropon.png" ) );
				pComboBoxClass->SetBackgroundSelectedImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\combobox\\drop_.png" ) );
				pComboBoxClass->SetBackgroundActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\combobox\\dropbg.png", "game\\images\\UI\\combobox\\dropdown.png" ) );
				pComboBoxClass->SetHoverColor( D3DCOLOR_ARGB( 150, 170, 150, 120 ) );

				//Options
				{
					pComboBoxClass->AddOption( pComboBoxClass->MakeOption( "Fighter", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ), CHARACTERCLASS_Fighter );
					pComboBoxClass->AddOption( pComboBoxClass->MakeOption( "Mechanician", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ), CHARACTERCLASS_Mechanician );
					pComboBoxClass->AddOption( pComboBoxClass->MakeOption( "Archer", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ), CHARACTERCLASS_Archer );
					pComboBoxClass->AddOption( pComboBoxClass->MakeOption( "Pikeman", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ), CHARACTERCLASS_Pikeman );
					pComboBoxClass->AddOption( pComboBoxClass->MakeOption( "Atalanta", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ), CHARACTERCLASS_Atalanta );
					pComboBoxClass->AddOption( pComboBoxClass->MakeOption( "Knight", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ), CHARACTERCLASS_Knight );
					pComboBoxClass->AddOption( pComboBoxClass->MakeOption( "Magician", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ), CHARACTERCLASS_Magician );
					pComboBoxClass->AddOption( pComboBoxClass->MakeOption( "Priestess", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ), CHARACTERCLASS_Priestess );
					pComboBoxClass->AddOption( pComboBoxClass->MakeOption( "Assassin", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ), CHARACTERCLASS_Assassin );
					pComboBoxClass->AddOption( pComboBoxClass->MakeOption( "Shaman", Rectangle2D( 0, 0, 110, 16 ), FALSE, D3DCOLOR_ARGB( 255, 210, 195, 180 ) ), CHARACTERCLASS_Shaman );
				}
				
				//Premium CheckBox
				UI::CheckBox_ptr pCheckBoxPremiumChange = std::make_shared<UI::CheckBox>( Rectangle2D( 176, 178, 13, 13 ) );
				pCheckBoxPremiumChange->SetID( CHECKBOX_ChangeClassPremium );
				pCheckBoxPremiumChange->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
				pCheckBoxPremiumChange->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
				pCheckBoxPremiumChange->SetText( "Premium Change Class" );
				pCheckBoxPremiumChange->SetEvent( UI::Event::Build( std::bind( &CCoinShopWindow::OnCheckPremiumClass, this, iPrice, std::placeholders::_1 ) ) );
				pCheckBoxPremiumChange->SetCheck( FALSE );
				pWindowItem->AddElement( pCheckBoxPremiumChange );

				pWindowItem->AddElement( pComboBoxClass );

				// Price Description
				UI::Text_ptr pItemDescPrice = std::make_shared<UI::Text>( Rectangle2D( 30, 256, 240, 18 ) );
				pItemDescPrice->SetText( "PRICE:" );
				pItemDescPrice->SetFont( "Arial", 18, 0, FALSE, TRUE, -1 );
				pWindowItem->AddElement( pItemDescPrice );

				// Price Value
				UI::Text_ptr pItemValuePrice = std::make_shared<UI::Text>( Rectangle2D( 100, 256, 240, 16 ) );
				pItemValuePrice->SetID( LABELID_Price );
				pItemValuePrice->SetText( FormatNumber( iPrice ) );
				pItemValuePrice->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
				pWindowItem->AddElement( pItemValuePrice );
			}
		}
	}
}

void CCoinShopWindow::HandleItemService( PacketCoinServiceBuy * psPacket, ItemData * pcItemData )
{
	UI::Window_ptr pWindowItem = pWindow1->GetElement<UI::Window>( WINDOWID_ItemData );

	if ( pWindowItem )
	{
		//Get Elements
		UI::ItemBox_ptr pItemBox = pWindowItem->GetElement<UI::ItemBox>( ITEMBOX_Perfectize );
		UI::ComboBox_ptr pComboBox = pWindowItem->GetElement<UI::ComboBox>( COMBOBOX_Spec );
		UI::InputField_ptr pInput = pWindowItem->GetElement<UI::InputField>( INPUTID_ChangeNick );
		UI::Counter_ptr pCounter = pWindowItem->GetElement<UI::Counter>( COUNTID_Quantity );

		switch ( psPacket->iType )
		{
			case ITEMCOIN_ItemPerfect:

				//Have Elements and have Item?
				if ( pItemBox && pItemBox->GetItems().size() > 0 )
				{
					if ( psPacket->iRetCode == 1 )
					{
						pItemBox->ClearItems();
						ITEMHANDLER->BackItemToInventory( pcItemData, ITEMCHECKINVENTORY_Requirements );
						ITEMHANDLER->ResetInventoryItemChecksum();
						TITLEBOX( "Perfectize Item Sucessful!" );
					}
					//Unblock Item Box
					pItemBox->EnableMouse();
				}
				break;

			case ITEMCOIN_ItemSwapper:
				pItemBox = pWindowItem->GetElement<UI::ItemBox>( ITEMBOX_Swapper );
				//Have Elements and have Item?
				if ( pItemBox && pItemBox->GetItems().size() > 0 )
				{
					if ( psPacket->iRetCode == 1 )
					{
						ItemData sItemData;
						if ( ITEMHANDLER->LoadItemImage( &pcItemData->sItem, &sItemData ) )
						{
							pItemBox->ClearItems();
							ITEMHANDLER->BackItemToInventory( &sItemData, ITEMCHECKINVENTORY_Requirements );
							ITEMHANDLER->ResetInventoryItemChecksum();
							TITLEBOX( "Swap Item Sucessful!" );
						}
					}

					//Unblock Item Box
					pItemBox->EnableMouse();
				}
				break;

			case ITEMCOIN_AgingRecovery:
				if ( psPacket->iRetCode == 1 )
				{
					ItemData sItemData;
					if ( ITEMHANDLER->LoadItemImage( &pcItemData->sItem, &sItemData ) )
					{
						ITEMHANDLER->BackItemToInventory( &sItemData, ITEMCHECKINVENTORY_Requirements );
						ITEMHANDLER->ResetInventoryItemChecksum();
						DeleteAgingRecoveryItem( psPacket->AgingRecovery.iID );
						TITLEBOX( "Recovery Item Sucessful!" );
					}
				}
				break;

			case ITEMCOIN_ChangeNick:
				if ( pInput )
				{
					if ( psPacket->iRetCode == 1 )
					{
						TITLEBOX( "Nick change successful! Relog now to see your new nick!" );
					}
				}
				break;

			case ITEMCOIN_ChangeClass:
				if ( pInput )
				{
					if ( psPacket->iRetCode == 1 )
					{
						TITLEBOX( "Class change successful! Relog now to see your new character!" );
					}
				}
				break;

			case ITEMCOIN_LevelUP:
				if ( pCounter )
				{
					if ( psPacket->iRetCode == 1 )
					{
						TITLEBOX( "Level UP successful!" );
					}
				}
				break;

			default:
				break;
		}
	}
}

void CCoinShopWindow::SetCoinAmount( int iValue, BOOL bCoins )
{
	auto pText = pWindow1->GetElement<UI::Text>( TEXTID_Credits );
	pText->SetTextEx( "Welcome %s!\nYou have %d %s", UNIT->GetName(), iValue, bCoins ? "Credits" : "Time pts" );
}

void CCoinShopWindow::OnCheckPreviewCostume( UI::CheckBox_ptr p )
{
	UI::List_ptr pListItems = pWindow1->GetElement<UI::List>( LISTID_Items );
	UI::Window_ptr pWindow = pListItems->GetWindow( WINDOWID_ItemStatus );
	UI::Window_ptr pWindowPreview = ( pWindow1->GetElement<UI::Window>( WINDOWID_ItemData ) )->GetElement<UI::Window>( WINDOWID_ItemPreview );
	
	if( !pListItems || !pWindow || !pWindowPreview )
		return;

	if( p->IsChecked() )
	{
		pWindow->Hide();
		pWindowPreview->Show();
	}
	else
	{
		pWindow->Show();
		pWindowPreview->Hide();
	}
}

void CCoinShopWindow::OnResolutionChanged()
{
	pWindow1->SetPosition( (RESOLUTION_WIDTH >> 1) - (pWindow1->GetWidth() >> 1), (RESOLUTION_HEIGHT >> 1) - (pWindow1->GetHeight() >> 1) );
}

void CCoinShopWindow::Init()
{
	pcImageCoinShopBackground = UI::ImageLoader::LoadImage( "game\\images\\coinshop\\background.png" );
	pcImageTimeShopBackground = UI::ImageLoader::LoadImage( "game\\images\\coinshop\\background_timeshop.png" );

	BuildWindow1();
}

void CCoinShopWindow::Render()
{
	pWindow1->Render( 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );

	// Render Status
	if ( pWindow1->IsOpen() )
	{
		if ( (*(BOOL*)0x03642090) || (*(BOOL*)0x03653E40) || (*(BOOL*)(0x036932FC)) )
		{
			Close();

			MOUSEHANDLER->UnblockMouse();

			return;
		}
	}
}
