#include "stdafx.h"
#include "CChristmasGiftWindowHandler.h"


CChristmasGiftWindowHandler::CChristmasGiftWindowHandler()
{
	pWindow = std::make_shared<UI::Window>( Rectangle2D( 20, 20, 364, 374 ) );
}


CChristmasGiftWindowHandler::~CChristmasGiftWindowHandler()
{
	pWindow = nullptr;
}

void CChristmasGiftWindowHandler::Init()
{
	BuildWindow();
}

void CChristmasGiftWindowHandler::Update( float fTime )
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

void CChristmasGiftWindowHandler::Render()
{
	pWindow->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

void CChristmasGiftWindowHandler::Open( int iItemCount )
{
	if ( pWindow->IsOpen() == FALSE )
	{
		if ( iItemCount > 0 )
		{
			pWindow->GetElement<UI::Window>( WINDOW_ReceiveEmpty )->Hide();
			pWindow->GetElement<UI::Window>( WINDOW_ReceiveItems )->Show();
			pWindow->GetElement<UI::Window>( WINDOW_ReceiveItems )->GetElement<UI::Text>( TEXT_Sender )->SetTextEx( "You have %d gift%s to open!", iItemCount, iItemCount > 1 ? "s" : "" );
		}
		else
		{
			pWindow->GetElement<UI::Window>( WINDOW_ReceiveEmpty )->Show();
			pWindow->GetElement<UI::Window>( WINDOW_ReceiveItems )->Hide();
		}
		pWindow->Show();
		MOUSEHANDLER->BlockMouse();
	}
}

void CChristmasGiftWindowHandler::Close()
{
	if ( pWindow->IsOpen() )
	{
		pWindow->Hide();
		MOUSEHANDLER->UnblockMouse();
	}
}

BOOL CChristmasGiftWindowHandler::OnMouseClick( CMouse * pcMouse )
{
	return pWindow->OnMouseClick( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

void CChristmasGiftWindowHandler::OnMouseMove( CMouse * pcMouse )
{
	pWindow->OnMouseMove( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

void CChristmasGiftWindowHandler::OnResolutionChanged()
{
	//Base Rectangle
	int iWindowWidth = pWindow->GetWidth();
	int iWindowHeight = pWindow->GetHeight();
	int iWindowX = (RESOLUTION_WIDTH >> 2) - (iWindowWidth >> 1) + 30;
	int iWindowY = (RESOLUTION_HEIGHT >> 2) - (iWindowHeight >> 1) + 78;
	pWindow->SetPosition( iWindowX, iWindowY );
}

void CChristmasGiftWindowHandler::BuildWindow()
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

	//Header Text
	UI::Text_ptr pTextHeader = std::make_shared<UI::Text>( Rectangle2D( 30, 40, 300, 18 ) );
	pTextHeader->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pTextHeader->SetText( "Hold on, let me see if I have anything in your name." );
	pWindow->AddElement( pTextHeader );

	//Empty
	{
		UI::Window_ptr pWindowReceiveEmpty = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 364, 374 ) );
		pWindowReceiveEmpty->SetID( WINDOW_ReceiveEmpty );

		UI::Text_ptr pTextEmpty = std::make_shared<UI::Text>( Rectangle2D( 30, 58, 304, 80 ) );
		pTextEmpty->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
		pTextEmpty->SetText( "Hmm... guess not. Yeah, there's nothing for you here at the moment.\n\nFeel free to come back later and I will check again." );
		pTextEmpty->SetMultiLine( TRUE );
		pTextEmpty->SetWordWrap( TRUE );
		pWindowReceiveEmpty->AddElement( pTextEmpty );

		UI::ImageBox_ptr pImageEmptyBox = std::make_shared<UI::ImageBox>( Rectangle2D( (pWindowReceiveEmpty->GetWidth() >> 1) - (159 >> 1) - 50, 140, 159, 142 ) );
		pImageEmptyBox->SetImage( UI::ImageLoader::LoadImage( "game\\images\\distributor\\emptyitembox_noglow.png" ) );
		pWindowReceiveEmpty->AddElement( pImageEmptyBox );

		pWindowReceiveEmpty->Show();

		pWindow->AddElement( pWindowReceiveEmpty );
	}

	//With Item
	{
		UI::Window_ptr pWindowReceiveItem = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 364, 374 ) );
		pWindowReceiveItem->SetID( WINDOW_ReceiveItems );

		pWindowReceiveItem->Hide();

		//Sent Text
		UI::Text_ptr pTextSender = std::make_shared<UI::Text>( Rectangle2D( 40, 210, 170, 20 ) );
		pTextSender->SetID( TEXT_Sender );
		pTextSender->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
		pTextSender->SetText( "" );
		pTextSender->SetHorizontalAlign( ALIGN_Center );
		pWindowReceiveItem->AddElement( pTextSender );

		//Get Item
		UI::Button_ptr pButtonGetItem = std::make_shared<UI::Button>( Rectangle2D( (pWindowReceiveItem->GetWidth() >> 1) - (144 >> 1) - 30, 60, 144, 142 ) );
		pButtonGetItem->SetImage( UI::ImageLoader::LoadImage( "game\\images\\events\\xmas\\box1.png" ) );
		pButtonGetItem->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\events\\xmas\\box1.png", "game\\images\\events\\xmas\\box2.png" ) );
		pButtonGetItem->SetEvent( UI::Event::Build( std::bind( &CChristmasGiftWindowHandler::OnButtonAcceptClick, this, std::placeholders::_1 ) ) );
		pWindowReceiveItem->AddElement( pButtonGetItem );

		pWindow->AddElement( pWindowReceiveItem );
	}

	//Image Animation
	UI::ImageBox_ptr pImageAnimation = std::make_shared<UI::ImageBox>( Rectangle2D( 180, 122, 179, 206 ) );
	pImageAnimation->SetID( IMAGE_XmasTreeAnimation );

	for ( int i = 0; i < MAX_IMAGE_XMASANIMATION; i++ )
		pcaImageAnimation[i] = UI::ImageLoader::LoadImage( "game\\images\\events\\xmas\\tree_%02d.png", i + 1 );

	pImageAnimation->SetImage( pcaImageAnimation[0] );
	pWindow->AddElement( pImageAnimation );

	UI::Button_ptr pButtonClose = std::make_shared<UI::Button>( Rectangle2D( (pWindow->GetWidth() >> 1) - (93 >> 1), 342, 93, 23 ) );
	pButtonClose->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\btnclose.png" ) );
	pButtonClose->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\btnclose.png", "game\\images\\UI\\window\\btnclose_.png" ) );
	pButtonClose->SetEvent( UI::Event::Build( std::bind( &CChristmasGiftWindowHandler::OnButtonCloseClick, this, std::placeholders::_1 ) ) );
	pWindow->AddElement( pButtonClose );

	pWindow->Hide();
}

void CChristmasGiftWindowHandler::OnButtonAcceptClick( UIEventArgs eArgs )
{
	if ( (dwUpdateTimeClose + MAX_TIMEXMAS_WAITRESPONSE) < TICKCOUNT )
	{
		MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
		MESSAGEBOX->SetTitle( "Open Box" );
		MESSAGEBOX->SetDescription( "Do you want to open the gift box?" );
		MESSAGEBOX->SetEvent( std::bind( &CChristmasGiftWindowHandler::OnAcceptReceiveGift, this, std::placeholders::_1 ) );
		MESSAGEBOX->SetAutoClose( 10 );
		MESSAGEBOX->Show();
	}
	else
		TITLEBOX( "Still in progress..." );
}

void CChristmasGiftWindowHandler::OnButtonCloseClick( UIEventArgs eArgs )
{
	if ( (dwUpdateTimeClose + MAX_TIMEXMAS_WAITRESPONSE) < TICKCOUNT )
		Close();
	else
		TITLEBOX( "Still in progress..." );
}

void CChristmasGiftWindowHandler::OnAcceptReceiveGift( UIEventArgs eArgs )
{
	if ( (dwUpdateTimeClose + MAX_TIMEXMAS_WAITRESPONSE) < TICKCOUNT )
	{
		if ( MESSAGEBOX->GetType() )
		{
			PacketXmasTreeItemNPC sPacket;
			sPacket.bItem = TRUE;
			XMASTREEHANDLER->ProcessPacket( &sPacket );

			dwUpdateTimeClose = TICKCOUNT;
		}
	}
	else
		TITLEBOX( "Still in progress..." );
}
