#include "stdafx.h"
#include "CChristmasTreeWindowHandler.h"


CChristmasTreeWindowHandler::CChristmasTreeWindowHandler()
{
	pWindow = std::make_shared<UI::Window>( Rectangle2D( 20, 20, 364, 374 ) );
}


CChristmasTreeWindowHandler::~CChristmasTreeWindowHandler()
{
	pWindow = nullptr;
}

void CChristmasTreeWindowHandler::Init()
{
	BuildWindow();
}

void CChristmasTreeWindowHandler::Update( float fTime )
{
	if ( bReverseAnimation )
	{
		if ( (--uAnimationWheel) == 0 )
			bReverseAnimation = false;
	}
	else
	{
		if ( (++uAnimationWheel) == (MAX_IMAGE_XMASANIMATION - 1) )
			bReverseAnimation = true;
	}

	pWindow->GetElement<UI::ImageBox>( IMAGE_XmasTreeAnimation )->SetImage( pcaImageAnimation[uAnimationWheel] );
}

void CChristmasTreeWindowHandler::Render()
{
	pWindow->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

void CChristmasTreeWindowHandler::Open( const char * pszCharacterName )
{
	pWindow->GetElement<UI::Text>( TEXT_Information )->SetTextEx( "This Xmas Tree belongs to %s.\nYou can give any item using the gift box below.", pszCharacterName );
	if ( pWindow->IsOpen() == FALSE )
	{
		pWindow->GetElement<UI::ItemBox>( ITEMBOX_Items )->ClearItems();
		pWindow->Show();

		MOUSEHANDLER->BlockMouse();
	}
}

void CChristmasTreeWindowHandler::Close()
{
	if ( pWindow->IsOpen() )
	{
		pWindow->GetElement<UI::ItemBox>( ITEMBOX_Items )->ClearItems();
		pWindow->Hide();

		MOUSEHANDLER->UnblockMouse();
	}
}

BOOL CChristmasTreeWindowHandler::OnMouseClick( CMouse * pcMouse )
{
	return pWindow->OnMouseClick( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

void CChristmasTreeWindowHandler::OnMouseMove( CMouse * pcMouse )
{
	pWindow->OnMouseMove( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

void CChristmasTreeWindowHandler::OnResolutionChanged()
{
	//Base Rectangle
	int iWindowWidth = pWindow->GetWidth();
	int iWindowHeight = pWindow->GetHeight();
	int iWindowX = (RESOLUTION_WIDTH >> 2) - (iWindowWidth >> 1) + 30;
	int iWindowY = (RESOLUTION_HEIGHT >> 2) - (iWindowHeight >> 1) + 78;
	pWindow->SetPosition( iWindowX, iWindowY );
}

void CChristmasTreeWindowHandler::BuildWindow()
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
	UI::ImageBox_ptr pHeaderTitle = std::make_shared<UI::ImageBox>( Rectangle2D( (pWindow->GetWidth() >> 1) - (47 >> 1), -4, 47, 16 ) );
	pHeaderTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\text\\header_event.png" ) );
	pWindow->AddElement( pHeaderTitle );

	UI::ImageBox_ptr pHeaderHat = std::make_shared<UI::ImageBox>( Rectangle2D( -30, -12, 94, 128 ) );
	pHeaderHat->SetImage( UI::ImageLoader::LoadImage( "game\\images\\events\\xmas\\xmashat.png" ) );
	pWindow->AddElement( pHeaderHat );

	UI::Text_ptr pInformationText = std::make_shared<UI::Text>( Rectangle2D( 30, 40, 300, 80 ) );
	pInformationText->SetID( TEXT_Information );
	pInformationText->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pInformationText->SetText( "This Xmas Tree belongs to GM-Prog.\nYou can give any item using the gift box below." );
	pInformationText->SetMultiLine( TRUE );
	pWindow->AddElement( pInformationText );

	UI::ImageBox_ptr pImageArrow = std::make_shared<UI::ImageBox>( Rectangle2D( 40, 96, 87, 92 ) );
	pImageArrow->SetImage( UI::ImageLoader::LoadImage( "game\\images\\events\\xmas\\arrow.png" ) );
	pWindow->AddElement( pImageArrow );

	//Image ItemBox
	UI::ImageBox_ptr pImageItemBox = std::make_shared<UI::ImageBox>( Rectangle2D( 127, 86, 113, 113 ) );
	pImageItemBox->SetImage( UI::ImageLoader::LoadImage( "game\\images\\events\\xmas\\itembox.png" ) );
	pWindow->AddElement( pImageItemBox );

	//ItemBox
	UI::ItemBox_ptr pItemBox = std::make_shared<UI::ItemBox>( 128, 87, 5, 5 );
	pItemBox->SetMaxItems( 25 );
	pItemBox->SetID( ITEMBOX_Items );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemBase, ITEMBASE_Potion );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemBase, ITEMBASE_Quest1 );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemBase, ITEMBASE_Quest2 );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_EquippedItem, TRUE );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_ItemTime, FALSE );
	pItemBox->AddRule( UI::ItemBox::ERule::RULE_Disallow, UI::ItemBox::ETypeRule::TYPERULE_LockItemGM, 0 );
	pWindow->AddElement( pItemBox );

	//Image Animation
	UI::ImageBox_ptr pImageAnimation = std::make_shared<UI::ImageBox>( Rectangle2D( 180, 80, 179, 206 ) );
	pImageAnimation->SetID( IMAGE_XmasTreeAnimation );

	for ( int i = 0; i < MAX_IMAGE_XMASANIMATION; i++ )
		pcaImageAnimation[i] = UI::ImageLoader::LoadImage( "game\\images\\events\\xmas\\tree_%02d.png", i + 1 );

	pImageAnimation->SetImage( pcaImageAnimation[0] );
	pWindow->AddElement( pImageAnimation );

	//Gift
	UI::Button_ptr pButtonGift = std::make_shared<UI::Button>( Rectangle2D( (pWindow->GetWidth() >> 1) - (130 >> 1), 294, 130, 30 ) );
	pButtonGift->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\events\\xmas\\btngift.png", "game\\images\\events\\xmas\\btngift_.png" ) );
	pButtonGift->SetEvent( UI::Event::Build( std::bind( &CChristmasTreeWindowHandler::OnButtonGiftClick, this, std::placeholders::_1 ) ) );
	pWindow->AddElement( pButtonGift );

	//Close
	UI::Button_ptr pButtonClose = std::make_shared<UI::Button>( Rectangle2D( (pWindow->GetWidth() >> 1) - (93 >> 1), 342, 93, 23 ) );
	pButtonClose->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\btnclose.png", "game\\images\\UI\\window\\btnclose_.png" ) );
	pButtonClose->SetEvent( UI::Event::Build( std::bind( &CChristmasTreeWindowHandler::OnButtonCloseClick, this, std::placeholders::_1 ) ) );
	pWindow->AddElement( pButtonClose );

	pWindow->Hide();
}

void CChristmasTreeWindowHandler::OnButtonGiftClick( UIEventArgs eArgs )
{
	if ( (dwUpdateTimeClose + MAX_TIMEXMAS_WAITRESPONSE) < TICKCOUNT )
	{
		if ( pWindow->GetElement<UI::ItemBox>( ITEMBOX_Items )->GetItems().size() > 0 )
		{
			MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
			MESSAGEBOX->SetTitle( "Send Gift" );
			MESSAGEBOX->SetDescription( "Do you want to send the gift?" );
			MESSAGEBOX->SetEvent( std::bind( &CChristmasTreeWindowHandler::OnAcceptSendGift, this, std::placeholders::_1 ) );
			MESSAGEBOX->SetAutoClose( 10 );
			MESSAGEBOX->Show();
		}
		else
			TITLEBOX( "You need to put an item on gift item box!" );
	}
	else
		TITLEBOX( "Still in progress..." );
}

void CChristmasTreeWindowHandler::OnButtonCloseClick( UIEventArgs eArgs )
{
	if ( pWindow->GetElement<UI::ItemBox>( ITEMBOX_Items )->GetItems().size() > 0 )
	{
		TITLEBOX( "Remove items from the window first!" );
		return;
	}

	if ( (dwUpdateTimeClose + MAX_TIMEXMAS_WAITRESPONSE) < TICKCOUNT )
		Close();
	else
		TITLEBOX( "Still in progress..." );
}

void CChristmasTreeWindowHandler::OnAcceptSendGift( UIEventArgs eArgs )
{
	if ( MESSAGEBOX->GetType() )
	{
		XMASTREEHANDLER->SendItems( pWindow->GetElement<UI::ItemBox>( ITEMBOX_Items )->GetItems() );

		dwUpdateTimeClose = TICKCOUNT;
	}
}
