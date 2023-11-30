#include "stdafx.h"
#include "CCaravanWindowHandler.h"


CCaravanWindowHandler::CCaravanWindowHandler()
{
	pWindow = std::make_shared<UI::Window>( Rectangle2D( 20, 20, 364, 374 ) );
}


CCaravanWindowHandler::~CCaravanWindowHandler()
{
	pWindow = nullptr;
}

void CCaravanWindowHandler::Init()
{
	BuildWindow();
}

void CCaravanWindowHandler::Open()
{
	WINDOW_INVENTORY = TRUE;

	if ( !pWindow->IsOpen() )
		MOUSEHANDLER->BlockMouse();

	pWindow->Show();
}

void CCaravanWindowHandler::Close()
{
	if ( pWindow->IsOpen() )
		MOUSEHANDLER->UnblockMouse();

	ClearItems();

	pWindow->Hide();
}

void CCaravanWindowHandler::ClearItems()
{
	pWindow->GetElement<UI::ItemBox>( ITEMBOX_Items )->ClearItems();
}

void CCaravanWindowHandler::AddItem( ItemData * pcItemData )
{
	auto pItemBox = pWindow->GetElement<UI::ItemBox>( ITEMBOX_Items );

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
	{
		cItemData.sPosition = Point2D( pcItemData->sPosition.iX, pcItemData->sPosition.iY );

		pItemBox->ResetLastRender();

		pItemBox->SetNoCheckSlot( TRUE );

		pItemBox->AddItem( &cItemData );

		pItemBox->SetNoCheckSlot( FALSE );
	}
}

void CCaravanWindowHandler::UpdateCaravanData( const std::string strName, bool bFollow )
{
	auto pCheckbox = pWindow->GetElement<UI::CheckBox>( CHECKBOX_Follow );
	auto pInputName = pWindow->GetElement<UI::InputField>( INPUT_Name );

	pCheckbox->SetCheck( bFollow ? TRUE : FALSE );
	pInputName->SetText( strName );
}

void CCaravanWindowHandler::OnMouseMove( CMouse * pcMouse )
{
	pWindow->OnMouseMove( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

BOOL CCaravanWindowHandler::OnMouseClick( CMouse * pcMouse )
{
	return pWindow->OnMouseClick( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

BOOL CCaravanWindowHandler::OnKeyPress( CKeyboard * pcKeyboard )
{
	return pWindow->OnKeyPress( pcKeyboard );
}

BOOL CCaravanWindowHandler::OnKeyChar( CKeyboard * pcKeyboard )
{
	return pWindow->OnKeyChar( pcKeyboard );
}

void CCaravanWindowHandler::Render()
{
	pWindow->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

void CCaravanWindowHandler::Update( float fTime )
{
	if ( IsOpen() )
	{
		if ( auto pcItemTimer = CARAVANHANDLER->GetCaravanItemTimer() )
		{
			if ( pcItemTimer->dwTimeLeft <= 5 )
				OnButtonCloseClick( UIEventArgs{} );
		}
	}
}

void CCaravanWindowHandler::OnResolutionChanged()
{
	//Base Rectangle
	int iWindowWidth = pWindow->GetWidth();
	int iWindowHeight = pWindow->GetHeight();
	int iWindowX = (RESOLUTION_WIDTH >> 2) - (iWindowWidth >> 1) + 30;
	int iWindowY = (RESOLUTION_HEIGHT >> 2) - (iWindowHeight >> 1) + 78;
	pWindow->SetPosition( iWindowX, iWindowY );
}

void CCaravanWindowHandler::BuildWindow()
{
	//Base Rectangle
	int iWindowWidth = pWindow->GetWidth();
	int iWindowHeight = pWindow->GetHeight();
	int iWindowX = (RESOLUTION_WIDTH >> 2) - (iWindowWidth >> 1) + 30;
	int iWindowY = (RESOLUTION_HEIGHT >> 2) - (iWindowHeight >> 1) + 78;
	pWindow->SetPosition( iWindowX, iWindowY );

	pWindow->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\window.png" ) );

	UI::ImageBox_ptr pHeaderBG = std::make_shared<UI::ImageBox>( Rectangle2D( (pWindow->GetWidth() >> 1) - (181 >> 1), -13, 181, 32 ) );
	pHeaderBG->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\text\\header.png" ) );
	pWindow->AddElement( pHeaderBG );

	//Header Title
	UI::ImageBox_ptr pHeaderTitle = std::make_shared<UI::ImageBox>( Rectangle2D( (pWindow->GetWidth() >> 1) - (75 >> 1), -6, 75, 18 ) );
	pHeaderTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\caravan\\title.png" ) );
	pWindow->AddElement( pHeaderTitle );

	//Image ItemBox
	UI::ImageBox_ptr pImageItemBox = std::make_shared<UI::ImageBox>( Rectangle2D( 80, 40, 201, 201 ) );
	pImageItemBox->SetImage( UI::ImageLoader::LoadImage( "game\\images\\caravan\\itemslot.png" ) );
	pWindow->AddElement( pImageItemBox );

	//ItemBox
	UI::ItemBox_ptr pItemBox = std::make_shared<UI::ItemBox>( 81, 41, 9, 9 );
	pItemBox->SetMaxItems( 81 );
	pItemBox->SetID( ITEMBOX_Items );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemBase, ITEMBASE_Potion );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemBase, ITEMBASE_Quest1 );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemBase, ITEMBASE_Quest2 );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_EquippedItem, TRUE );
	pItemBox->SetFreePlace( TRUE );
	pWindow->AddElement( pItemBox );

	UI::Group_ptr pGroupInput = std::make_shared<UI::Group>();
	pWindow->AddElement( pGroupInput );

	//Text
	UI::Text_ptr pTextName = std::make_shared<UI::Text>( Rectangle2D( 30, 290, 38, 18 ) );
	pTextName->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pTextName->SetText( "Name:" );
	pWindow->AddElement( pTextName );

	//Input Image
	UI::ImageBox_ptr pImageInput = std::make_shared<UI::ImageBox>( Rectangle2D( 74, 306, 150, 1 ) );
	pImageInput->SetImage( UI::ImageLoader::LoadImage( "game\\images\\caravan\\underline.png" ) );
	pWindow->AddElement( pImageInput );

	//Input
	UI::InputField_ptr pInputName = std::make_shared<UI::InputField>( Rectangle2D( 74, 290, 150, 18 ) );
	pInputName->SetID( INPUT_Name );
	pInputName->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pInputName->SetText( "" );
	pInputName->SetMaxLength( 20 );
	pInputName->SetSelect( TRUE );
	pInputName->SetGroup( pGroupInput );
	pWindow->AddElement( pInputName );

	//Text
	UI::Text_ptr pTextRename = std::make_shared<UI::Text>( Rectangle2D( 74, 310, 100, 18 ) );
	pTextRename->SetFont( "Arial", 14, 0, FALSE, FALSE, -1 );
	pTextRename->SetText( "(Max. 20 characters)" );
	pWindow->AddElement( pTextRename );


	UI::ImageBox_ptr pImageBlank = std::make_shared<UI::ImageBox>( Rectangle2D( 236, 290, 60, 18 ) );
	pWindow->AddElement( pImageBlank );

	//Follow
	UI::CheckBox_ptr pCheckBoxFollow = std::make_shared<UI::CheckBox>( Rectangle2D( 236, 290, 13, 13 ) );
	pCheckBoxFollow->SetID( CHECKBOX_Follow );
	pCheckBoxFollow->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pCheckBoxFollow->SetText( "Follow me!" );
	pCheckBoxFollow->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindow->AddElement( pCheckBoxFollow );

	UI::Tooltip_ptr pTooltipFollow = std::make_shared<UI::Tooltip>();
	pTooltipFollow->Init();
	pTooltipFollow->SetFollowMouse( FALSE );
	pTooltipFollow->SetPosition( TOOLTIPPOSITION_Top );
	pTooltipFollow->SetFont( "Arial", 14, -1 );
	pTooltipFollow->SetText( "Choose if caravan should follow you!" );
	pTooltipFollow->SetLinkObject( pImageBlank );
	pWindow->AddElement( pTooltipFollow );

	//Close
	UI::Button_ptr pButtonClose = std::make_shared<UI::Button>( Rectangle2D( (pWindow->GetWidth() >> 1) - (93 >> 1), 342, 93, 23 ) );
	pButtonClose->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\btnclose.png", "game\\images\\UI\\window\\btnclose_.png" ) );
	pButtonClose->SetEvent( UI::Event::Build( std::bind( &CCaravanWindowHandler::OnButtonCloseClick, this, std::placeholders::_1 ) ) );
	pWindow->AddElement( pButtonClose );

	pWindow->Hide();
}

void CCaravanWindowHandler::OnButtonCloseClick( UIEventArgs eArgs )
{
	auto pItemBox = pWindow->GetElement<UI::ItemBox>( ITEMBOX_Items );
	auto pCheckbox = pWindow->GetElement<UI::CheckBox>( CHECKBOX_Follow );
	auto pInputName = pWindow->GetElement<UI::InputField>( INPUT_Name );
	CARAVANHANDLER->SaveCaravan( pItemBox->GetItems(), pInputName->GetText(), pCheckbox->IsChecked() ? true : false );
	Close();
}
