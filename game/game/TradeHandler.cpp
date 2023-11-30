#include "stdafx.h"
#include "TradeHandler.h"


CTradeHandler::CTradeHandler()
{
}


CTradeHandler::~CTradeHandler()
{
}

void CTradeHandler::Init()
{
	pWindow = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 344, 264 ) );
	pWindow->SetFocusWindow( FALSE );

	UI::Button_ptr pButtonClose = std::make_shared<UI::Button>( Rectangle2D( pWindow->GetWidth() - 32, 46, 20, 20 ) );
	pButtonClose->SetID( BUTTONID_Close );
	pButtonClose->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\misc\\btnclose.png", "game\\images\\misc\\btnclose_.png" ) );
	pButtonClose->SetEvent( UI::Event::Build( std::bind( &CTradeHandler::OnButtonCloseClick, this, std::placeholders::_1 ) ) );
	pWindow->AddElement( pButtonClose );

	UI::Button_ptr pButtonTradeCheckSelf = std::make_shared<UI::Button>( Rectangle2D( 300, 212, 27, 27 ) );
	pButtonTradeCheckSelf->SetID( BUTTONID_TradeCheckSelf );
	pButtonTradeCheckSelf->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\misc\\tradecheck.png", "game\\images\\misc\\tradechecked.png" ) );
	pButtonTradeCheckSelf->SetEvent( UI::Event::Build( std::bind( &CTradeHandler::OnButtonCheckTradeClick, this, std::placeholders::_1 ) ) );
	pWindow->AddElement( pButtonTradeCheckSelf );

	UI::Text_ptr pTextCoinUnit = std::make_shared<UI::Text>( Rectangle2D( 250, 106, 78, 20 ) );
	pTextCoinUnit->SetID( TEXTID_CoinUnit );
	pTextCoinUnit->SetFont( "Arial", 16, 0, FALSE, FALSE, RGBA( 255, 217, 217, 217 ) );
	pTextCoinUnit->SetText( "0" );
	pTextCoinUnit->SetHorizontalAlign( EAlign::ALIGN_Center );
	pTextCoinUnit->SetVerticalAlign( EAlign::ALIGN_Middle );
	pWindow->AddElement( pTextCoinUnit );

	UI::Text_ptr pTextCoinSelf = std::make_shared<UI::Text>( Rectangle2D( 250, 194, 78, 20 ) );
	pTextCoinSelf->SetID( TEXTID_CoinSelf );
	pTextCoinSelf->SetFont( "Arial", 16, 0, FALSE, FALSE, RGBA( 255, 217, 217, 217 ) );
	pTextCoinSelf->SetText( "0" );
	pTextCoinSelf->SetHorizontalAlign( EAlign::ALIGN_Center );
	pTextCoinSelf->SetVerticalAlign( EAlign::ALIGN_Middle );
	pWindow->AddElement( pTextCoinSelf );

	UI::Button_ptr pButtonCoins = std::make_shared<UI::Button>( Rectangle2D( 234, 194, 96, 20 ) );
	pButtonCoins->SetEvent( UI::Event::Build( std::bind( &CTradeHandler::OnButtonCoinClick, this, std::placeholders::_1 ) ) );
	pWindow->AddElement( pButtonCoins );

	UI::ImageBox_ptr pImageTradeCheckUnitOff = std::make_shared<UI::ImageBox>( Rectangle2D( 300, 124, 27, 27 ) );
	pImageTradeCheckUnitOff->SetImage( UI::ImageLoader::LoadImage( "game\\images\\misc\\tradecheck.png" ) );
	pWindow->AddElement( pImageTradeCheckUnitOff );

	UI::ImageBox_ptr pImageTradeCheckUnit = std::make_shared<UI::ImageBox>( Rectangle2D( 300, 124, 27, 27 ) );
	pImageTradeCheckUnit->SetID( IMAGEID_TradeCheckUnit );
	pImageTradeCheckUnit->SetImage( UI::ImageLoader::LoadImage( "game\\images\\misc\\tradechecked.png" ) );
	pImageTradeCheckUnit->Hide();
	pWindow->AddElement( pImageTradeCheckUnit );

	UI::Text_ptr pTextWeightUnit = std::make_shared<UI::Text>( Rectangle2D( 250, 131, 46, 18 ) );
	pTextWeightUnit->SetID( TEXTID_WeightUnit );
	pTextWeightUnit->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pTextWeightUnit->SetText( "0" );
	pTextWeightUnit->SetHorizontalAlign( EAlign::ALIGN_Center );
	pTextWeightUnit->SetVerticalAlign( EAlign::ALIGN_Middle );
	pWindow->AddElement( pTextWeightUnit );

	pImageTradeAccept = UI::ImageLoader::LoadImage( "image\\Sinimage\\shopall\\Trade_Check_Info.bmp" );
	pImageTradeCancel = UI::ImageLoader::LoadImage( "image\\Sinimage\\shopall\\Trade_CancelInfo.bmp" );
}

void CTradeHandler::OnMouseMove( CMouse * pcMouse )
{
	if ( IsWindowTradeOpen() && !IsDelayTime() )
		pWindow->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

BOOL CTradeHandler::OnMouseClick( class CMouse * pcMouse )
{
	if ( IsWindowTradeOpen() && !IsDelayTime() )
		return pWindow->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
	return FALSE;
}

BOOL CTradeHandler::OnKeyChar( CKeyboard * pcKeyboard )
{
	if ( IsWindowTradeOpen() && !IsDelayTime() )
		return pWindow->OnKeyChar( pcKeyboard );
	return FALSE;
}

void CTradeHandler::Render()
{
	if ( IsWindowTradeOpen() )
	{
		if ( dwTradeTimeDelay == 0 )
		{
			dwTradeTimeDelay = TICKCOUNT + 1000;
		}

		if ( COINHANDLER->GetWindow()->IsOpen() || (*(BOOL*)0x03642090) || (*(BOOL*)0x03653E40) || (*(BOOL*)(0x036932FC)) )
		{
			CloseTradeWindow();

			return;
		}

		//Update position of Trade Window while opening...
		pWindow->SetPosition( WINDOW_TRADEMOVE - 384, 40 );
		pWindow->Render( 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );

		int iWindowX = pWindow->GetX();
		int iWindowY = pWindow->GetY();

		//Trade Changed?
		if ( TRADE_ACCEPT == FALSE )
		{
			UI::Button_ptr pButton = pWindow->GetElement<UI::Button>( BUTTONID_TradeCheckSelf );
			
			//Unselect Button
			if ( pButton->IsSelected() )
				pButton->SetSelected( FALSE );
		}

		UI::ImageBox_ptr pImageBox = pWindow->GetElement<UI::ImageBox>( IMAGEID_TradeCheckUnit );

		//Unit Accept?
		if ( TRADEUNIT_ACCEPT )
		{
			//Show Accept button image
			if ( pImageBox->IsOpen() == FALSE )
				pImageBox->Show();
		}
		else
		{
			//Hide button
			if ( pImageBox->IsOpen() )
				pImageBox->Hide();
		}

		//Weight
		UI::Text_ptr pText = pWindow->GetElement<UI::Text>( TEXTID_WeightUnit );
		if ( TRADEUNIT_WEIGHT >= 0 )
			pText->SetTextEx( "%d", TRADEUNIT_WEIGHT );
		else
			pText->SetText( "0" );

		//Draw Name on Left Side
		GetGraphicsDevice()->GetDefaultFont()->Draw( ((char *)0x03706F48), (WINDOW_TRADEMOVE - 384) + 228, 50 + 40, DT_LEFT, DX::Color( 255, 255, 255 ) );

		//Trade Accept Ballon
		UI::Button_ptr pButton = pWindow->GetElement<UI::Button>( BUTTONID_TradeCheckSelf );
		if ( pButton->IsHover() )
			UI::ImageRender::Render( pImageTradeAccept, iWindowX + pButton->GetX() + (pButton->GetWidth() >> 1) - 10, iWindowY + pButton->GetY() - pImageTradeAccept->GetHeight() - 2, pImageTradeAccept->GetWidth(), pImageTradeAccept->GetHeight(), -1 );

		//Trade Cancel
		pButton = pWindow->GetElement<UI::Button>( BUTTONID_Close );
		if ( pButton->IsHover() )
			UI::ImageRender::Render( pImageTradeCancel, iWindowX + pButton->GetX() + (pButton->GetWidth() >> 1) - 40, iWindowY + pButton->GetY()  - pImageTradeCancel->GetHeight() - 2, pImageTradeCancel->GetWidth(), pImageTradeCancel->GetHeight(), -1 );
	}
}

void CTradeHandler::CloseTradeWindow()
{
	if ( CanCloseTradeWindow() )
	{
		//Send Cancel
		CALL_WITH_ARG2( 0x000628290, TRADE_UNITID, 3 );
		//Set Close
		IsWindowTradeOpen( FALSE );
		
		//Close Inventory
		(*(BOOL*)0x035EBB20) = FALSE;

		//Close
		CALLT( 0x0050C310, 0x036EFD98 );
		//Cancel?
		(*(BOOL*)0x036EFCCC) = FALSE;

		//Closed, Unselect button
		UI::Button_ptr pButton = pWindow->GetElement<UI::Button>( BUTTONID_TradeCheckSelf );
		pButton->SetSelected( FALSE );

		//Hide Unit Button
		UI::ImageBox_ptr pImageBox = pWindow->GetElement<UI::ImageBox>( IMAGEID_TradeCheckUnit );
		pImageBox->Hide();

		dwTradeTimeDelay = 0;
	}
}

void CTradeHandler::SetCheckTradeButton( BOOL bCheck )
{
	if ( bCheck )
	{
		if ( IsValidTrade() )
		{
			//Check Button
			TRADE_ACCEPT = TRUE;

			//Send Trade Items?
			CALL_WITH_ARG1( 0x00628410, TRADE_UNITID );
			CALLT( 0x0050C660, 0x036EFD98 );
		}
	}
	else
	{
		//Check Button
		TRADE_ACCEPT = FALSE;

		//Send Trade Items?
		CALL_WITH_ARG1( 0x00628410, TRADE_UNITID );
	}
}

void CTradeHandler::HandlePacket( PacketCoinsTraded * psPacket )
{
	iMaxCoins = ACCOUNTHANDLER->GetCoinAmount();

	iSelfCoins = 0;
	iUnitCoins = 0;

	UI::Text_ptr pText = pWindow->GetElement<UI::Text>( TEXTID_CoinUnit );

	pText->SetText( "0" );

	UpdateCoins();
}

void CTradeHandler::HandlePacket( PacketTradeCoinSet * psPacket )
{
	if ( iUnitCoins != psPacket->iCoins )
	{
		SetCheckTradeButton( FALSE );

		TITLEBOX( "Trade Partner has changed the Credit Amount" );
	}

	iUnitCoins = psPacket->iCoins;

	UI::Text_ptr pText = pWindow->GetElement<UI::Text>( TEXTID_CoinUnit );
	
	pText->SetText( (char*)FormatNumber( iUnitCoins ) );

}

void CTradeHandler::ProcessPacket( PacketTradeCoinSet * psPacket )
{
	psPacket->iLength = sizeof( PacketTradeCoinSet );
	psPacket->iHeader = PKTHDR_CoinTradeSet;
	
	SENDPACKETG( psPacket );
}

ItemData * CTradeHandler::GetMouseItemHover()
{
	if ( IsWindowTradeOpen() == FALSE )
		return NULL;

	ItemData * psaDataTrade = ((ItemData*)0x03699A88);

	// Warehouse rect box
	RECT rTradeBox{ 21, 195, 21 + (22 * 9), 195 + (22 * 4) };

	ItemData * pcItemData = NULL;

	// Get Item from mouse inside
	for ( int k = 0; k < 20; k++ )
	{
		ItemData * pTradeItem = psaDataTrade + k;

		if ( !pTradeItem->bValid )
			continue;

		Rectangle2D rRectangle;
		rRectangle.iX = pTradeItem->sPosition.iX;
		rRectangle.iY = pTradeItem->sPosition.iY;
		rRectangle.iWidth = pTradeItem->iWidth - 1;
		rRectangle.iHeight = pTradeItem->iHeight - 1;

		if ( rRectangle.Inside( MOUSEHANDLER->GetPosition() ) )
		{
			pcItemData = pTradeItem;
			break;
		}
	}

	return pcItemData;
}

ItemData * CTradeHandler::GetMouseItemHoverOther()
{
	if ( IsWindowTradeOpen() == FALSE )
		return NULL;

	ItemData * psaDataTrade = ((ItemData*)0x0368E668);

	// Warehouse rect box
	RECT rTradeBox{ 21, 86, 21 + (22 * 9), 86 + (22 * 4) };

	ItemData * pcItemData = NULL;

	// Get Item from mouse inside
	for ( int k = 0; k < 20; k++ )
	{
		ItemData * pTradeItem = psaDataTrade + k;

		if ( !pTradeItem->bValid )
			continue;

		Rectangle2D rRectangle;
		rRectangle.iX = pTradeItem->sPosition.iX;
		rRectangle.iY = pTradeItem->sPosition.iY - 109;
		rRectangle.iWidth = pTradeItem->iWidth - 1;
		rRectangle.iHeight = pTradeItem->iHeight - 1;

		if ( rRectangle.Inside( MOUSEHANDLER->GetPosition() ) )
		{
			pcItemData = pTradeItem;
			break;
		}
	}

	return pcItemData;
}

void CTradeHandler::OnButtonCloseClick( UIEventArgs e )
{
	CloseTradeWindow();
}

void CTradeHandler::OnButtonCheckTradeClick( UIEventArgs e )
{
	UI::Button_ptr pButton = pWindow->GetElement<UI::Button>( BUTTONID_TradeCheckSelf );

	//Set Selected button...
	if ( pButton->IsSelected() == FALSE )
	{
		pButton->SetSelected( TRUE );

		SetCheckTradeButton( TRUE );
	}
}

void CTradeHandler::OnButtonCoinClick( UIEventArgs e )
{
	if ( ACCOUNTHANDLER->IsAccountShareDenied( ACCOUNTSHARE_DenyTradeCoins ) )
	{
		TITLEBOX( AY_OBFUSCATE( "You can't Trade Coins, because you're logged in with the Share PW!" ) );
		return;
	}

	if ( TRADE_ACCEPT == FALSE )
	{
		INPUTBOX->SetNumberType( 0, iMaxCoins, TRUE );
		INPUTBOX->SetDescription( "How many credits do you wanna trade?" );
		INPUTBOX->SetTitle( "TRADE CREDITS" );
		INPUTBOX->SetValue( (char*)FormatNumber( iSelfCoins ) );
		INPUTBOX->SetEvent( std::bind( &CTradeHandler::OnInputCoinClick, this, std::placeholders::_1 ) );
		INPUTBOX->Show();
	}
	else
		TITLEBOX( "You already accepted the Trade!" );
}

void CTradeHandler::OnInputCoinClick( UIEventArgs e )
{
	if ( INPUTBOX->GetType() )
	{
		UI::Text_ptr pText = pWindow->GetElement<UI::Text>( TEXTID_CoinSelf );
		
		pText->SetTextEx( "%s", FormatNumber( INPUTBOX->GetValue() ) );

		ProcessPacket( &PacketTradeCoinSet( INPUTBOX->GetValue(), TRADE_UNITID ) );
	}
}

void CTradeHandler::UpdateCoins()
{
	UI::Text_ptr pText = pWindow->GetElement<UI::Text>( TEXTID_CoinSelf );

	pText->SetTextEx( "%s", FormatNumber( iSelfCoins ) );
}

BOOL CTradeHandler::IsDelayTime()
{
	return (dwTradeTimeDelay > TICKCOUNT);
}

NAKED BOOL CTradeHandler::SendTradeCheckItem( int iID )
{
	JMP( 0x00628520 );
}
