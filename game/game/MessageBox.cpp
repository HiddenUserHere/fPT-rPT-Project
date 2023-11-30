#include "stdafx.h"
#include "MessageBox.h"

std::vector<std::function<void( UIEventArgs const & )>> vMsgBox;

CMessageBox::CMessageBox()
{
	pWindow1Main = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 230, 161 ) );
	pWindow1RollDice = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 295, 161 ) );
	pWindow1PersonalShop = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 295, 161 ) );
	pWindow1PersonalShopSeller = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 320, 160 ) );
	pWindow1PersonalShopBuyer = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 295, 161 ) );
}

CMessageBox::~CMessageBox()
{
	pWindow1Main = nullptr;
	pWindow1RollDice = nullptr;
	pWindow1PersonalShop = nullptr;
	pWindow1PersonalShopSeller = nullptr;
	pWindow1PersonalShopBuyer = nullptr;
}

void CMessageBox::SetEvent( std::function<void( UIEventArgs const & )> const & f )
{
	vMsgBox.clear();
	vMsgBox.push_back( f );
}

void CMessageBox::SetTitle( std::string strText )
{
	std::transform( strText.begin(), strText.end(), strText.begin(), ::toupper );

	if( pWindow1 )
		pWindow1->GetElement<UI::BitmapFont>( TextID_Title )->SetText( strText );
}

void CMessageBox::SetDescription( std::string strText )
{
	if( pWindow1 )
		pWindow1->GetElement<UI::Text>( TextID_Text )->SetText( strText );
}

void CMessageBox::SetAutoClose( int iDuration )
{
	if( !pWindow1 )
		return;

	iElapsedTime = iDuration; 
	bAutoClose = TRUE;

	dwTickTime = TICKCOUNT + (iDuration * 1000);

	auto pTextAutoClose = pWindow1->GetElement<UI::Text>( TEXTID_AutoClose );
	if( pTextAutoClose )
	{
		pTextAutoClose->SetTextEx( "Automatically closes in %d sec...", iElapsedTime );
		pTextAutoClose->Show();
	}
}

void CMessageBox::SetItemPriceMax( int iMinPrice, int iMaxPrice )
{
	if ( iBoxType == BOXTYPEID_PersonalShop )
		pWindow1->GetElement<UI::InputField>( INPUTID_ItemPrice )->SetIsNumber( TRUE, iMinPrice, iMaxPrice );
	if ( iBoxType == BOXTYPEID_PersonalShopBuy )
		pWindow1->GetElement<UI::Text>( TEXTID_Price )->SetText( FormatNumber( iMinPrice ) );
}

void CMessageBox::SetItemCounter( int iMax )
{
	if ( (iBoxType == BOXTYPEID_PersonalShop) || (iBoxType == BOXTYPEID_PersonalShopBuy) )
		pWindow1->GetElement<UI::Counter>( COUNTID_Quantity )->SetMinMax( 1, iMax );
}

BOOL CMessageBox::CanBuyItemPersonalShop()
{
	if ( iBoxType == BOXTYPEID_PersonalShop )
		return pWindow1->GetElement<UI::CheckBox>( CHECKBOXID_CanBuyItem )->IsChecked() ? FALSE : TRUE;

	return FALSE;
}

BOOL CMessageBox::IsSellByCreditsItemPersonalShop()
{
	if ( iBoxType == BOXTYPEID_PersonalShop )
		return pWindow1->GetElement<UI::CheckBox>( CHECKBOXID_SellByCreditsItem )->IsChecked();


	return FALSE;
}

int CMessageBox::GetPriceItem()
{
	if ( iBoxType == BOXTYPEID_PersonalShop )
		return atoi( pWindow1->GetElement<UI::InputField>( INPUTID_ItemPrice )->GetText().c_str() );

	return 0;
}

int CMessageBox::GetCountItem()
{
	if ( (iBoxType == BOXTYPEID_PersonalShop) || (iBoxType == BOXTYPEID_PersonalShopBuy) )
		return pWindow1->GetElement<UI::Counter>( COUNTID_Quantity )->GetCurrent();

	return 0;
}

std::string CMessageBox::GetPersonalShopMessage()
{
	if ( iBoxType == BOXTYPEID_PersonalShopMessage )
		return pWindow1->GetElement<UI::InputField>( INPUTID_MessagePersonalShop )->GetText();

	return "";
}

void CMessageBox::SetPersonalShopMessageLength( int iLength )
{
	pWindow1->GetElement<UI::InputField>( INPUTID_MessagePersonalShop )->SetMaxLength( iLength );
}

void CMessageBox::AddExtraText( std::string strText, Rectangle2D sBox, BOOL bBold, UI::Color cColor )
{
	UI::Text_ptr pText = std::make_shared<UI::Text>( strText, sBox, bBold, cColor.Get());
	pText->SetFont( "Arial", 14, 0, FALSE, bBold, cColor.Get());
	pText->SetWordWrap( TRUE );
	pText->SetMultiLine( TRUE );
	pText->SetHorizontalAlign( ALIGN_Center );
	pText->SetVerticalAlign( ALIGN_Middle );

	if( pWindow1 )
		pWindow1->AddElement( pText );

	vExtraElements.push_back( pText );
}

void CMessageBox::Show()
{
	if( !pWindow1 )
		return;

	if ( !pWindow1->IsOpen() )
	{
		pWindow1->Show();
		MOUSEHANDLER->BlockMouse();
	}
}

void CMessageBox::OnYesButtonClick( UIEventArgs eArgs )
{
	MOUSEHANDLER->UnblockMouse();

	bType = TRUE;

	if ( vMsgBox.size() > 0 )
		vMsgBox[0]( UIEventArgs{} );

	vMsgBox.clear();

	bAutoClose = FALSE;
	iElapsedTime = 0;

	if ( pWindow1 )
	{
		for ( std::vector<Element_ptr>::iterator it = vExtraElements.begin(); it != vExtraElements.end(); it++ )
		{
			Element_ptr p = (*it);

			pWindow1->DelElement( p );
		}

		pWindow1->Hide();
	}

	vExtraElements.clear();
}

void CMessageBox::OnNoButtonClick( UIEventArgs eArgs )
{
	MOUSEHANDLER->UnblockMouse();

	bType = FALSE;

	if ( vMsgBox.size() > 0 )
		vMsgBox[0]( UIEventArgs{} );

	vMsgBox.clear();

	bAutoClose = FALSE;
	iElapsedTime = 0;

	if ( pWindow1 )
	{
		for ( std::vector<Element_ptr>::iterator it = vExtraElements.begin(); it != vExtraElements.end(); it++ )
		{
			Element_ptr p = (*it);

			pWindow1->DelElement( p );
		}

		pWindow1->Hide();
	}

	vExtraElements.clear();
}

void CMessageBox::OnRollButtonClick( UIEventArgs eArgs )
{
	MOUSEHANDLER->UnblockMouse();

	bType = TRUE;

	if ( vMsgBox.size() > 0 )
		vMsgBox[0]( UIEventArgs{} );

	vMsgBox.clear();

	pWindow1RollDice->GetElement<UI::Button>( BUTTONID_Yes )->Disable();
	pWindow1RollDice->GetElement<UI::Button>( BUTTONID_No )->Disable();

	if ( pWindow1 )
	{
		for ( std::vector<Element_ptr>::iterator it = vExtraElements.begin(); it != vExtraElements.end(); it++ )
		{
			Element_ptr p = (*it);

			pWindow1->DelElement( p );
		}

		pWindow1->Hide();
	}

	vExtraElements.clear();
}

void CMessageBox::OnPassButtonClick( UIEventArgs eArgs )
{
	MOUSEHANDLER->UnblockMouse();

	bType = FALSE;

	if ( vMsgBox.size() > 0 )
		vMsgBox[0]( UIEventArgs{} );

	vMsgBox.clear();

	pWindow1RollDice->GetElement<UI::Button>( BUTTONID_Yes )->Disable();
	pWindow1RollDice->GetElement<UI::Button>( BUTTONID_No )->Disable();

	if ( pWindow1 )
	{
		for ( std::vector<Element_ptr>::iterator it = vExtraElements.begin(); it != vExtraElements.end(); it++ )
		{
			Element_ptr p = (*it);

			pWindow1->DelElement( p );
		}

		pWindow1->Hide();
	}

	vExtraElements.clear();
}

void CMessageBox::SetBoxType( int iType )
{
	iBoxType = iType;

	if ( pWindow1 )
	{
		for ( std::vector<Element_ptr>::iterator it = vExtraElements.begin(); it != vExtraElements.end(); it++ )
		{
			Element_ptr p = (*it);

			pWindow1->DelElement( p );
		}

		if ( pWindow1->IsOpen() )
			MOUSEHANDLER->UnblockMouse();

		pWindow1->Hide();

		vExtraElements.clear();

		auto pTextAutoClose = pWindow1->GetElement<UI::Text>( TEXTID_AutoClose );
		if ( pTextAutoClose )
			pTextAutoClose->Hide();
	}

	if ( iBoxType == BOXTYPEID_YesNo )
	{
		pWindow1 = pWindow1Main;

		if ( !pWindow1 )
			return;

		pWindow1->GetElement<UI::Button>( BUTTONID_Yes )->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\messagebox\\btnyes.png", "game\\images\\UI\\messagebox\\btnyes_.png" ) );
		pWindow1->GetElement<UI::Button>( BUTTONID_No )->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\messagebox\\btnno.png", "game\\images\\UI\\messagebox\\btnno_.png" ) );
	}
	else if ( iBoxType == BOXTYPEID_OkCancel )
	{
		pWindow1 = pWindow1Main;

		if ( !pWindow1 )
			return;

		pWindow1->GetElement<UI::Button>( BUTTONID_Yes )->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\messagebox\\btnok.png", "game\\images\\UI\\messagebox\\btnok_.png" ) );
		pWindow1->GetElement<UI::Button>( BUTTONID_No )->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\messagebox\\btncancel.png", "game\\images\\UI\\messagebox\\btncancel_.png" ) );
	}
	else if ( iBoxType == BOXTYPEID_RollDice )
	{
		pWindow1 = pWindow1RollDice;

		if ( !pWindow1 )
			return;

		pWindow1->GetElement<UI::Button>( BUTTONID_Yes )->Enable();
		pWindow1->GetElement<UI::Button>( BUTTONID_No )->Enable();
	}
	else if ( iBoxType == BOXTYPEID_PersonalShop )
	{
		pWindow1 = pWindow1PersonalShop;

		if ( !pWindow1 )
			return;

		pWindow1->GetElement<UI::Button>( BUTTONID_Yes )->Enable();
		pWindow1->GetElement<UI::Button>( BUTTONID_No )->Enable();

		pWindow1->GetElement<UI::CheckBox>( CHECKBOXID_CanBuyItem )->SetCheck( FALSE );
	}
	else if ( iBoxType == BOXTYPEID_PersonalShopMessage )
	{
		pWindow1 = pWindow1PersonalShopSeller;

		if ( !pWindow1 )
			return;

		pWindow1->GetElement<UI::Button>( BUTTONID_Yes )->Enable();
		pWindow1->GetElement<UI::Button>( BUTTONID_No )->Enable();

		pWindow1->GetElement<UI::InputField>( INPUTID_MessagePersonalShop )->ClearInput();
	}
	else if ( iBoxType == BOXTYPEID_PersonalShopBuy )
	{
		pWindow1 = pWindow1PersonalShopBuyer;

		if ( !pWindow1 )
			return;

		pWindow1->GetElement<UI::Button>( BUTTONID_Yes )->Enable();
		pWindow1->GetElement<UI::Button>( BUTTONID_No )->Enable();
	}
}

void CMessageBox::Init()
{
	//Window Main
	{
		pWindow1Main->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\messagebox\\background_normal.png" ) );
		pWindow1Main->SetID( WINDOWID_Main );

		UI::Button_ptr pButtonYes = std::make_shared<UI::Button>( Rectangle2D( 26, 116, 88, 35 ) );
		pButtonYes->SetID( BUTTONID_Yes );
		pButtonYes->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\messagebox\\btnyes.png", "game\\images\\UI\\messagebox\\btnyes_.png" ) );
		pButtonYes->SetEvent( UI::Event::Build( std::bind( &CMessageBox::OnYesButtonClick, this, std::placeholders::_1 ) ) );
		pWindow1Main->AddElement( pButtonYes );

		UI::Button_ptr pButtonNo = std::make_shared<UI::Button>( Rectangle2D( 115, 116, 88, 35 ) );
		pButtonNo->SetID( BUTTONID_No );
		pButtonNo->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\messagebox\\btnno.png", "game\\images\\UI\\messagebox\\btnno_.png" ) );
		pButtonNo->SetEvent( UI::Event::Build( std::bind( &CMessageBox::OnNoButtonClick, this, std::placeholders::_1 ) ) );
		pWindow1Main->AddElement( pButtonNo );

		//Text
		UI::Text_ptr pTextDescription = std::make_shared<UI::Text>( Rectangle2D( 35, 50, 160, 44 ) );
		pTextDescription->SetID( TextID_Text );
		pTextDescription->SetFont( "Noto Sans", 16, 0, FALSE, FALSE, D3DCOLOR_XRGB( 228, 218, 212 ) );
		pTextDescription->SetText( "" );
		pTextDescription->SetDropdownShadow( TRUE );
		pTextDescription->SetHorizontalAlign( ALIGN_Center );
		pTextDescription->SetMultiLine( TRUE );
		pTextDescription->SetDropdownColor( D3DCOLOR_ARGB( 100, 0, 0, 0 ) );
		pTextDescription->SetWordWrap( TRUE );
		pWindow1Main->AddElement( pTextDescription );

		UI::Text_ptr pTextAutoClose = std::make_shared<UI::Text>( Rectangle2D( 10, 95, 210, 60 ) );
		pTextAutoClose->SetID( TEXTID_AutoClose );
		pTextAutoClose->SetFont( "Arial", 14, 0, FALSE, FALSE, D3DCOLOR_XRGB( 180, 180, 180 ) );
		pTextAutoClose->SetText( "" );
		pTextAutoClose->SetHorizontalAlign( ALIGN_Center );
		pWindow1Main->AddElement( pTextAutoClose );
		pWindow1Main->Hide();

		UI::Bitmapfont_ptr pTextTitle = std::make_shared<UI::BitmapFont>( Rectangle2D( 12, 16, 207, 0 ) );
		pTextTitle->SetID( TextID_Title );
		pTextTitle->SetHorizontalAlign( ALIGN_Center );
		pTextTitle->ReadFontData( "game\\fonts\\messagebox.dat" );
		pTextTitle->SetFontImage( UI::ImageLoader::LoadImage( "game\\fonts\\messagebox.png" ) );
		pWindow1Main->AddElement( pTextTitle );
	}

	//Window Roll Dice
	{
		pWindow1RollDice->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\messagebox\\background_diceroll.png" ) );
		pWindow1RollDice->SetID( WINDOWID_RollDice );

		UI::ImageBox_ptr pRollDiceIcon = std::make_shared<UI::ImageBox>( Rectangle2D( 128, 12, 39, 30 ) );
		pRollDiceIcon->SetImage( UI::ImageLoader::LoadImage( "game\\images\\diceroll\\icon.png" ) );
		pWindow1RollDice->AddElement( pRollDiceIcon );

		UI::Button_ptr pButtonYes = std::make_shared<UI::Button>( Rectangle2D( 109, 116, 88, 35 ) );
		pButtonYes->SetID( BUTTONID_Yes );
		pButtonYes->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\messagebox\\btnroll.png", "game\\images\\UI\\messagebox\\btnroll_.png" ) );
		pButtonYes->SetEvent( UI::Event::Build( std::bind( &CMessageBox::OnRollButtonClick, this, std::placeholders::_1 ) ) );
		pButtonYes->SetDisabledImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\messagebox\\btnroll.png" ) );
		pWindow1RollDice->AddElement( pButtonYes );

		UI::Button_ptr pButtonNo = std::make_shared<UI::Button>( Rectangle2D( 198, 116, 88, 35 ) );
		pButtonNo->SetID( BUTTONID_No );
		pButtonNo->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\messagebox\\btnpass.png", "game\\images\\UI\\messagebox\\btnpass_.png" ) );
		pButtonNo->SetEvent( UI::Event::Build( std::bind( &CMessageBox::OnPassButtonClick, this, std::placeholders::_1 ) ) );
		pButtonNo->SetDisabledImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\messagebox\\btnpass.png" ) );
		pWindow1RollDice->AddElement( pButtonNo );

		//Text
		UI::Text_ptr pTextDescription = std::make_shared<UI::Text>( Rectangle2D( 118, 45, 160, 44 ) );
		pTextDescription->SetID( TextID_Text );
		pTextDescription->SetFont( "Arial", 16, 0, FALSE, FALSE, D3DCOLOR_XRGB( 228, 218, 212 ) );
		pTextDescription->SetText( "" );
		pTextDescription->SetDropdownShadow( TRUE );
		pTextDescription->SetHorizontalAlign( ALIGN_Center );
		pTextDescription->SetMultiLine( TRUE );
		pTextDescription->SetDropdownColor( D3DCOLOR_ARGB( 100, 0, 0, 0 ) );
		pTextDescription->SetWordWrap( TRUE );
		pWindow1RollDice->AddElement( pTextDescription );

		UI::Text_ptr pTextAutoClose = std::make_shared<UI::Text>( Rectangle2D( 112, 100, 170, 60 ) );
		pTextAutoClose->SetID( TEXTID_AutoClose );
		pTextAutoClose->SetFont( "Arial", 14, 0, FALSE, FALSE, D3DCOLOR_XRGB( 180, 180, 180 ) );
		pTextAutoClose->SetText( "" );
		pTextAutoClose->SetHorizontalAlign( ALIGN_Center );
		pWindow1RollDice->AddElement( pTextAutoClose );

		UI::Bitmapfont_ptr pTextTitle = std::make_shared<UI::BitmapFont>( Rectangle2D( 176, 22, 207, 0 ) );
		pTextTitle->SetID( TextID_Title );
		pTextTitle->ReadFontData( "game\\fonts\\messagebox.dat" );
		pTextTitle->SetFontImage( UI::ImageLoader::LoadImage( "game\\fonts\\messagebox.png" ) );
		pWindow1RollDice->AddElement( pTextTitle );
		pWindow1RollDice->Hide();

		pAutoCloseBar[0] = UI::ImageLoader::LoadImage( "game\\images\\UI\\messagebox\\autoclosebar.png" );
		pAutoCloseBar[1] = UI::ImageLoader::LoadImage( "game\\images\\UI\\messagebox\\autoclosebar_.png" );
		pAutoCloseBar[2] = UI::ImageLoader::LoadImage( "game\\images\\UI\\messagebox\\autoclosediv.png" );
	}

	//Window Sell Item
	{
		pWindow1PersonalShop->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\messagebox\\background_diceroll.png" ) );
		pWindow1PersonalShop->SetID( WINDOWID_RollDice );

		UI::Button_ptr pButtonYes = std::make_shared<UI::Button>( Rectangle2D( 109, 116, 88, 35 ) );
		pButtonYes->SetID( BUTTONID_Yes );
		pButtonYes->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\messagebox\\btnok.png", "game\\images\\UI\\messagebox\\btnok_.png" ) );
		pButtonYes->SetEvent( UI::Event::Build( std::bind( &CMessageBox::OnYesButtonClick, this, std::placeholders::_1 ) ) );
		pWindow1PersonalShop->AddElement( pButtonYes );

		UI::Button_ptr pButtonNo = std::make_shared<UI::Button>( Rectangle2D( 198, 116, 88, 35 ) );
		pButtonNo->SetID( BUTTONID_No );
		pButtonNo->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\messagebox\\btncancel.png", "game\\images\\UI\\messagebox\\btncancel_.png" ) );
		pButtonNo->SetEvent( UI::Event::Build( std::bind( &CMessageBox::OnNoButtonClick, this, std::placeholders::_1 ) ) );
		pWindow1PersonalShop->AddElement( pButtonNo );

		//Text
		UI::Text_ptr pTextDescription = std::make_shared<UI::Text>( Rectangle2D( 118, 35, 160, 18 ) );
		pTextDescription->SetID( TextID_Text );
		pTextDescription->SetFont( "Arial", 16, 0, FALSE, FALSE, D3DCOLOR_XRGB( 228, 218, 212 ) );
		pTextDescription->SetText( "" );
		pTextDescription->SetDropdownShadow( TRUE );
		pTextDescription->SetHorizontalAlign( ALIGN_Left );
		pTextDescription->SetMultiLine( TRUE );
		pTextDescription->SetDropdownColor( D3DCOLOR_ARGB( 100, 0, 0, 0 ) );
		pTextDescription->SetWordWrap( TRUE );
		pWindow1PersonalShop->AddElement( pTextDescription );

		UI::Text_ptr pTextAutoClose = std::make_shared<UI::Text>( Rectangle2D( 112, 100, 170, 60 ) );
		pTextAutoClose->SetID( TEXTID_AutoClose );
		pTextAutoClose->SetFont( "Arial", 14, 0, FALSE, FALSE, D3DCOLOR_XRGB( 180, 180, 180 ) );
		pTextAutoClose->SetText( "" );
		pTextAutoClose->SetHorizontalAlign( ALIGN_Center );
		pWindow1PersonalShop->AddElement( pTextAutoClose );

		UI::Group_ptr pGroupPrice = std::make_shared<UI::Group>();
		pWindow1PersonalShop->AddElement( pGroupPrice );

		UI::InputField_ptr pInput = std::make_shared<UI::InputField>( Rectangle2D( 118, 53, 160, 18 ) );
		pInput->SetID( INPUTID_ItemPrice );
		pInput->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
		pInput->SetText( "" );
		pInput->SetSelected( TRUE );
		pInput->SetSelect( TRUE );
		pInput->SetMaxLength( 16 );
		pInput->SetIsNumber( TRUE, 0, 0 );
		pInput->SetIsFormattedNumber( TRUE );
		pInput->SetTextAlign( EAlign::ALIGN_Middle, EAlign::ALIGN_Middle );
		pInput->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\input\\minitextbox.png" ) );
		pInput->SetGroup( pGroupPrice );
		pWindow1PersonalShop->AddElement( pInput );

		UI::CheckBox_ptr pCheckBoxBuyItem = std::make_shared<UI::CheckBox>( Rectangle2D( 118, 74, 13, 13 ) ); //X=210
		pCheckBoxBuyItem->SetID( CHECKBOXID_CanBuyItem );
		pCheckBoxBuyItem->SetFont( "Arial", 16, 6, FALSE, FALSE, D3DCOLOR_XRGB( 228, 218, 212 ) );
		pCheckBoxBuyItem->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pCheckBoxBuyItem->SetText( "View Only" );
		pCheckBoxBuyItem->SetCheck( FALSE );
		pWindow1PersonalShop->AddElement( pCheckBoxBuyItem );

		UI::CheckBox_ptr pCheckSellByCreditsItem = std::make_shared<UI::CheckBox>( Rectangle2D( 118, 94, 13, 13 ) ); //X=210
		pCheckSellByCreditsItem->SetID( CHECKBOXID_SellByCreditsItem );
		pCheckSellByCreditsItem->SetFont( "Arial", 16, 6, FALSE, FALSE, D3DCOLOR_XRGB( 228, 218, 212 ) );
		pCheckSellByCreditsItem->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pCheckSellByCreditsItem->SetText( "Coins" );
		pCheckSellByCreditsItem->SetCheck( FALSE );
		pWindow1PersonalShop->AddElement( pCheckSellByCreditsItem );

/*		UI::Text_ptr pTextLabel = std::make_shared<UI::Text>( Rectangle2D( 118, 72, 150, 18 ) );
		pTextLabel->SetFont( "Arial", 16, 0, FALSE, FALSE, D3DCOLOR_XRGB( 228, 218, 212 ) );
		pTextLabel->SetText( "Quantity:" );
		pTextLabel->SetHorizontalAlign( ALIGN_Left );
		pWindow1PersonalShop->AddElement( pTextLabel );

		UI::Counter_ptr pCounter = std::make_shared<UI::Counter>( Rectangle2D( 118, 90, 80, 26 ) );
		pCounter->SetID( COUNTID_Quantity );
		pCounter->SetMinMax( 1, 100 );
		pCounter->SetImage( UI::ImageLoader::LoadImage( "game\\images\\coinshop\\counter.png" ) );
		pCounter->SetButtonDecrease( Rectangle2D( 41, 0, 19, 26 ), UI::ImageLoader::LoadImage( "game\\images\\coinshop\\counter_down.png" ) );
		pCounter->SetButtonIncrease( Rectangle2D( 61, 0, 19, 26 ), UI::ImageLoader::LoadImage( "game\\images\\coinshop\\counter_up.png" ) );
		pCounter->GetInput()->SetBox( Rectangle2D( 0, 0, 42, 26 ) );
		pCounter->GetInput()->SetSelect( TRUE );
		pCounter->GetInput()->SetCanScroll( TRUE );
		pCounter->GetInput()->SetTextAlign( ALIGN_Center, ALIGN_Middle );
		pWindow1PersonalShop->AddElement( pCounter );
*/
		UI::Bitmapfont_ptr pTextTitle = std::make_shared<UI::BitmapFont>( Rectangle2D( 118, 20, 160, 0 ) );
		pTextTitle->SetID( TextID_Title );
		pTextTitle->ReadFontData( "game\\fonts\\messagebox.dat" );
		pTextTitle->SetFontImage( UI::ImageLoader::LoadImage( "game\\fonts\\messagebox.png" ) );
		pTextTitle->SetHorizontalAlign( ALIGN_Center );
		pWindow1PersonalShop->AddElement( pTextTitle );
		pWindow1PersonalShop->Hide();
	}

	//Window Message Personal
	{
		pWindow1PersonalShopSeller->SetID( WINDOWID_RollDice );

		UI::RectangleBox_ptr pRectangleBox = std::make_shared<UI::RectangleBox>( Rectangle2D( 0, 0, 320, 160 ) );
		pRectangleBox->SetColor( DX::Color( 235, 45, 39, 27 ), DX::Color( 245, 9, 23, 22 ) );
		pWindow1PersonalShopSeller->AddElement( pRectangleBox );

		UI::Button_ptr pButtonYes = std::make_shared<UI::Button>( Rectangle2D( 68, 90, 88, 35 ) );
		pButtonYes->SetID( BUTTONID_Yes );
		pButtonYes->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\messagebox\\btnok.png", "game\\images\\UI\\messagebox\\btnok_.png" ) );
		pButtonYes->SetEvent( UI::Event::Build( std::bind( &CMessageBox::OnYesButtonClick, this, std::placeholders::_1 ) ) );
		pWindow1PersonalShopSeller->AddElement( pButtonYes );

		UI::Button_ptr pButtonNo = std::make_shared<UI::Button>( Rectangle2D( 158, 90, 88, 35 ) );
		pButtonNo->SetID( BUTTONID_No );
		pButtonNo->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\messagebox\\btncancel.png", "game\\images\\UI\\messagebox\\btncancel_.png" ) );
		pButtonNo->SetEvent( UI::Event::Build( std::bind( &CMessageBox::OnNoButtonClick, this, std::placeholders::_1 ) ) );
		pWindow1PersonalShopSeller->AddElement( pButtonNo );

		//Text
		UI::Text_ptr pTextDescription = std::make_shared<UI::Text>( Rectangle2D( 20, 34, 300, 18 ) );
		pTextDescription->SetID( TextID_Text );
		pTextDescription->SetFont( "Arial", 16, 0, FALSE, FALSE, D3DCOLOR_XRGB( 228, 218, 212 ) );
		pTextDescription->SetText( "" );
		pTextDescription->SetDropdownShadow( TRUE );
		pTextDescription->SetHorizontalAlign( ALIGN_Left );
		pTextDescription->SetMultiLine( TRUE );
		pTextDescription->SetDropdownColor( D3DCOLOR_ARGB( 100, 0, 0, 0 ) );
		pTextDescription->SetWordWrap( TRUE );
		pWindow1PersonalShopSeller->AddElement( pTextDescription );

		UI::Text_ptr pTextAutoClose = std::make_shared<UI::Text>( Rectangle2D( 112, 100, 170, 60 ) );
		pTextAutoClose->SetID( TEXTID_AutoClose );
		pTextAutoClose->SetFont( "Arial", 14, 0, FALSE, FALSE, D3DCOLOR_XRGB( 180, 180, 180 ) );
		pTextAutoClose->SetText( "" );
		pTextAutoClose->SetHorizontalAlign( ALIGN_Center );
		pWindow1PersonalShopSeller->AddElement( pTextAutoClose );

		UI::Group_ptr pGroupMessage = std::make_shared<UI::Group>();
		pWindow1PersonalShopSeller->AddElement( pGroupMessage );

		UI::InputField_ptr pInput = std::make_shared<UI::InputField>( Rectangle2D( 16, 60, 290, 18 ) );
		pInput->SetID( INPUTID_MessagePersonalShop );
		pInput->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
		pInput->SetText( "" );
		pInput->SetSelect( TRUE );
		pInput->SetSelected( TRUE );
		pInput->SetMaxLength( 128 );
		pInput->SetTextAlign( EAlign::ALIGN_Left, EAlign::ALIGN_Middle );
		pInput->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\input\\textbox.png" ) );
		pInput->SetGroup( pGroupMessage );
		pWindow1PersonalShopSeller->AddElement( pInput );

		UI::Bitmapfont_ptr pTextTitle = std::make_shared<UI::BitmapFont>( Rectangle2D( 16, 6, 290, 0 ) );
		pTextTitle->SetID( TextID_Title );
		pTextTitle->ReadFontData( "game\\fonts\\messagebox.dat" );
		pTextTitle->SetFontImage( UI::ImageLoader::LoadImage( "game\\fonts\\messagebox.png" ) );
		pTextTitle->SetHorizontalAlign( ALIGN_Center );
		pWindow1PersonalShopSeller->AddElement( pTextTitle );
		pWindow1PersonalShopSeller->Hide();
	}

	//Window Buy Item
	{
		pWindow1PersonalShopBuyer->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\messagebox\\background_diceroll.png" ) );
		pWindow1PersonalShopBuyer->SetID( WINDOWID_RollDice );

		UI::Button_ptr pButtonYes = std::make_shared<UI::Button>( Rectangle2D( 109, 116, 88, 35 ) );
		pButtonYes->SetID( BUTTONID_Yes );
		pButtonYes->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\messagebox\\btnok.png", "game\\images\\UI\\messagebox\\btnok_.png" ) );
		pButtonYes->SetEvent( UI::Event::Build( std::bind( &CMessageBox::OnYesButtonClick, this, std::placeholders::_1 ) ) );
		pWindow1PersonalShopBuyer->AddElement( pButtonYes );

		UI::Button_ptr pButtonNo = std::make_shared<UI::Button>( Rectangle2D( 198, 116, 88, 35 ) );
		pButtonNo->SetID( BUTTONID_No );
		pButtonNo->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\messagebox\\btncancel.png", "game\\images\\UI\\messagebox\\btncancel_.png" ) );
		pButtonNo->SetEvent( UI::Event::Build( std::bind( &CMessageBox::OnNoButtonClick, this, std::placeholders::_1 ) ) );
		pWindow1PersonalShopBuyer->AddElement( pButtonNo );

		//Text
		UI::Text_ptr pTextDescription = std::make_shared<UI::Text>( Rectangle2D( 118, 35, 160, 18 ) );
		pTextDescription->SetID( TextID_Text );
		pTextDescription->SetFont( "Arial", 16, 0, FALSE, FALSE, D3DCOLOR_XRGB( 228, 218, 212 ) );
		pTextDescription->SetText( "" );
		pTextDescription->SetDropdownShadow( TRUE );
		pTextDescription->SetHorizontalAlign( ALIGN_Left );
		pTextDescription->SetMultiLine( TRUE );
		pTextDescription->SetDropdownColor( D3DCOLOR_ARGB( 100, 0, 0, 0 ) );
		pTextDescription->SetWordWrap( TRUE );
		pWindow1PersonalShopBuyer->AddElement( pTextDescription );

		UI::Text_ptr pTextAutoClose = std::make_shared<UI::Text>( Rectangle2D( 112, 100, 170, 60 ) );
		pTextAutoClose->SetID( TEXTID_AutoClose );
		pTextAutoClose->SetFont( "Arial", 14, 0, FALSE, FALSE, D3DCOLOR_XRGB( 180, 180, 180 ) );
		pTextAutoClose->SetText( "" );
		pTextAutoClose->SetHorizontalAlign( ALIGN_Center );
		pWindow1PersonalShopBuyer->AddElement( pTextAutoClose );

		UI::ImageBox_ptr pImageShop = std::make_shared<UI::ImageBox>( Rectangle2D( 118, 53, 160, 18 ) );
		pImageShop->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\input\\minitextbox.png" ) );
		pWindow1PersonalShopBuyer->AddElement( pImageShop );

		UI::Text_ptr pTextPrice = std::make_shared<UI::Text>( Rectangle2D( 118, 53, 160, 18 ) );
		pTextPrice->SetID( TEXTID_Price );
		pTextPrice->SetFont( "Arial", 16, 0, FALSE, FALSE, D3DCOLOR_XRGB( 228, 218, 212 ) );
		pTextPrice->SetText( "" );
		pTextPrice->SetHorizontalAlign( ALIGN_Center );
		pWindow1PersonalShopBuyer->AddElement( pTextPrice );

		
		UI::Text_ptr pTextLabel = std::make_shared<UI::Text>( Rectangle2D( 118, 72, 150, 18 ) );
		pTextLabel->SetFont( "Arial", 16, 0, FALSE, FALSE, D3DCOLOR_XRGB( 228, 218, 212 ) );
		pTextLabel->SetText( "Quantity:" );
		pTextLabel->SetHorizontalAlign( ALIGN_Left );
		pWindow1PersonalShopBuyer->AddElement( pTextLabel );

		UI::Counter_ptr pCounter = std::make_shared<UI::Counter>( Rectangle2D( 118, 90, 80, 26 ) );
		pCounter->SetID( COUNTID_Quantity );
		pCounter->SetMinMax( 1, 1 );
		pCounter->SetImage( UI::ImageLoader::LoadImage( "game\\images\\coinshop\\counter.png" ) );
		pCounter->SetButtonDecrease( Rectangle2D( 41, 0, 19, 26 ), UI::ImageLoader::LoadImage( "game\\images\\coinshop\\counter_down.png" ) );
		pCounter->SetButtonIncrease( Rectangle2D( 61, 0, 19, 26 ), UI::ImageLoader::LoadImage( "game\\images\\coinshop\\counter_up.png" ) );
		pCounter->GetInput()->SetBox( Rectangle2D( 0, 0, 42, 26 ) );
		pCounter->GetInput()->SetSelect( TRUE );
		pCounter->GetInput()->SetCanScroll( TRUE );
		pCounter->GetInput()->SetTextAlign( ALIGN_Center, ALIGN_Middle );
		pWindow1PersonalShopBuyer->AddElement( pCounter );

		UI::Bitmapfont_ptr pTextTitle = std::make_shared<UI::BitmapFont>( Rectangle2D( 118, 20, 160, 0 ) );
		pTextTitle->SetID( TextID_Title );
		pTextTitle->ReadFontData( "game\\fonts\\messagebox.dat" );
		pTextTitle->SetFontImage( UI::ImageLoader::LoadImage( "game\\fonts\\messagebox.png" ) );
		pTextTitle->SetHorizontalAlign( ALIGN_Center );
		pWindow1PersonalShopBuyer->AddElement( pTextTitle );
		pWindow1PersonalShopBuyer->Hide();
	}
}

void CMessageBox::OnMouseMove( class CMouse * pcMouse )
{
	if( pWindow1 )
		pWindow1->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

BOOL CMessageBox::OnMouseClick( class CMouse * pcMouse )
{
	if( pWindow1 )
		if ( pWindow1->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
			return TRUE;

	return FALSE;
}

BOOL CMessageBox::OnKeyPress( class CKeyboard * pcKeyboard )
{
	if( !pWindow1 )
		return FALSE;

	if( iBoxType == BOXTYPEID_RollDice )
		return FALSE;

	if ( pWindow1->IsOpen() )
	{
		if( pcKeyboard->GetKey() == VK_RETURN )
		{
				bType = TRUE;
				if ( vMsgBox.size() > 0 )
					vMsgBox[0]( UIEventArgs{} );

				vMsgBox.clear();

				pWindow1->Hide();
				MOUSEHANDLER->UnblockMouse();
				return TRUE;
		}
		else if( pcKeyboard->GetKey() == VK_ESCAPE )
		{
				bType = FALSE;

				if ( vMsgBox.size() > 0 )
					vMsgBox[0]( UIEventArgs{} );

				vMsgBox.clear();

				pWindow1->Hide();
				MOUSEHANDLER->UnblockMouse();
				return TRUE;
		}
	}

	if ( pWindow1->OnKeyPress( pcKeyboard ) )
		return TRUE;

	return FALSE;
}

BOOL CMessageBox::OnKeyChar( CKeyboard * pcKeyboard )
{
	if ( !pWindow1 )
		return FALSE;

	return pWindow1->OnKeyChar( pcKeyboard );
}

void CMessageBox::Render()
{
	if( !pWindow1 )
		return;

	if ( pWindow1->IsOpen() )
	{
		int iBaseX = (RESOLUTION_WIDTH >> 1) - (pWindow1->GetWidth() >> 1);
		int iBaseY = (RESOLUTION_HEIGHT >> 1) - (pWindow1->GetHeight() >> 1);

		pWindow1->SetPosition( iBaseX, iBaseY );
		pWindow1->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );

		if ( iBoxType == BOXTYPEID_RollDice )
		{
			if ( pAutoCloseBar[0] && pAutoCloseBar[1] && pAutoCloseBar[2] )
			{
				UI::ImageRender::Render( pAutoCloseBar[0], iBaseX + 122, iBaseY + 87, pAutoCloseBar[0]->GetWidth(), pAutoCloseBar[0]->GetHeight(), -1 );
			
				int iWidth = ((dwTickTime - TICKCOUNT) * pAutoCloseBar[1]->GetWidth()) / (MAX_ROLLDICEDROP_TIME * 1000);
				UI::ImageRender::Render( pAutoCloseBar[1], iBaseX + 123, iBaseY + 88, iWidth, pAutoCloseBar[1]->GetHeight(), iBaseX + 123, 0, -1 );

				int iAlpha = ((dwTickTime - TICKCOUNT) % 65) + 190;
				UI::ImageRender::Render( pAutoCloseBar[2], iBaseX + 122 + iWidth - (pAutoCloseBar[2]->GetWidth() >> 2), iBaseY + 86 - (pAutoCloseBar[2]->GetHeight() >> 2), pAutoCloseBar[2]->GetWidth(), pAutoCloseBar[2]->GetHeight(), D3DCOLOR_ARGB( iAlpha, 255, 255, 255 ) );
			}
		}
	}
}

void CMessageBox::Update()
{
	if( !pWindow1 )
		return;

	if( pWindow1->IsOpen() && bAutoClose )
	{
		iElapsedTime--;

		auto pTextAutoClose = pWindow1->GetElement<UI::Text>( TEXTID_AutoClose );
		if( pTextAutoClose )
			pTextAutoClose->SetTextEx( "Automatically closes in %d sec...", iElapsedTime );

		if( iElapsedTime <= 0 )
			OnNoButtonClick( UIEventArgs{} );
	}
}

void CMessageBox::ClearExtraElements()
{
	for ( auto p : vExtraElements )
		pWindow1->DelElement( p );

	vExtraElements.clear();
}
