#include "stdafx.h"
#include "UIMessageBox.h"
#include "Timer.h"
#include "UI.h"


#define WIDTH_MSGBOX			169
#define HEIGHT_MSGBOX			125


int iHandlerWindowMSGBOX = -1;
int	iHandlerButton_yes_1 = -1;
int	iHandlerButton_yes_2 = -1;
int	iHandlerButton_no_1 = -1;
int	iHandlerButton_no_2 = -1;
int	iHandlerButton_ok_1 = -1;
int	iHandlerButton_ok_2 = -1;
int	iHandlerButton_cancel_1 = -1;
int	iHandlerButton_cancel_2 = -1;

BOOL bCreatedWindowMSGBOX = FALSE;

CUIMessageBox::CUIMessageBox( int iType, const char * pszTitle, const char * pszText )
{
	strTitle = pszTitle;
	strText  = pszText;
	iTypeBox = iType;

	// Load Images Windowbox
	if ( !bCreatedWindowMSGBOX )
	{
		iHandlerWindowMSGBOX	= UI::ImageLoader::LoadImage( "game\\images\\UI\\box\\box_small.png" );

		iHandlerButton_yes_1	= UI::ImageLoader::LoadImage( "game\\images\\UI\\box\\button_yes.png" );
		iHandlerButton_yes_2	= UI::ImageLoader::LoadImage( "game\\images\\UI\\box\\button_yes_.png" );

		iHandlerButton_no_1		= UI::ImageLoader::LoadImage( "game\\images\\UI\\box\\button_no.png" );
		iHandlerButton_no_2		= UI::ImageLoader::LoadImage( "game\\images\\UI\\box\\button_no_.png" );

		iHandlerButton_ok_1		= UI::ImageLoader::LoadImage( "game\\images\\UI\\box\\button_ok.png" );
		iHandlerButton_ok_2		= UI::ImageLoader::LoadImage( "game\\images\\UI\\box\\button_ok_.png" );

		iHandlerButton_cancel_1 = UI::ImageLoader::LoadImage( "game\\images\\UI\\box\\button_cancel.png" );
		iHandlerButton_cancel_2 = UI::ImageLoader::LoadImage( "game\\images\\UI\\box\\button_cancel_.png" );

		bCreatedWindowMSGBOX = TRUE;
	}

	// Load Fonts
	{
		if ( !pFontTitle )
		{
			pFontTitle = std::make_shared<UI::Font>();
			pFontTitle->SetFontSet( "Arial", FALSE, TRUE, 15, 6, -1 );
			pFontTitle->SetText( strTitle.c_str() );
		}
		if ( !pFontText )
		{
			pFontText = std::make_shared<UI::Font>();
			pFontText->SetFontSet( "Arial", FALSE, FALSE, 14, 6, -1 );
			pFontText->SetText( strText.c_str() );
		}
	}
}


CUIMessageBox::~CUIMessageBox()
{
}

void CUIMessageBox::AddButton( Rectangle2D rect, EImage eImage1, EImage eImage2, EButton eButton )
{
	if ( eButton == EBUTTON_1 )
	{
		UI::Button_ptr pButton1 = std::make_shared<UI::Button>( rect );
		pButton1->SetID( eButton );
		pButton1->SetActiveImage( eImage1 );
		pButton1->SetEvent( iEventMsgBox );
		pControllerCheckBox->AddElement( pButton1 );
	}
	else if ( eButton == EBUTTON_2 )
	{
		UI::Button_ptr pButton2 = std::make_shared<UI::Button>( rect );
		pButton2->SetID( eButton );
		pButton2->SetActiveImage( eImage1 );
		pButton2->SetEvent( iEventMsgBox );
		pControllerCheckBox->AddElement( pButton2 );
	}
}

void CUIMessageBox::CreateType()
{
	// Controller Create
	{
		if ( !pControllerCheckBox )
			pControllerCheckBox = std::make_shared<UI::Controller>();
		
		Rectangle2D rBox = { ( DX::Graphic::GetWidthBackBuffer() >> 1 ) - ( WIDTH_MSGBOX >> 1 ), ( DX::Graphic::GetHeightBackBuffer() >> 1 ) - ( HEIGHT_MSGBOX >> 1 ), WIDTH_MSGBOX, HEIGHT_MSGBOX };
		pControllerCheckBox->SetControllerRect( rBox );
		UI::Window_ptr pWindow = std::make_shared<UI::Window>( Rectangle2D{ 0, 0, WIDTH_MSGBOX, HEIGHT_MSGBOX } );
		pWindow->SetImage( iHandlerWindowMSGBOX );
		pControllerCheckBox->AddElement( pWindow );

	}

	switch ( iTypeBox )
	{
		case IDTYPE_YesNo:
			AddButton( Rectangle2D{ 40, HEIGHT_MSGBOX - 30, 27, 10 }, iHandlerButton_yes_1, iHandlerButton_yes_2, EBUTTON_1 );
			AddButton( Rectangle2D{ WIDTH_MSGBOX - 40 - 18, HEIGHT_MSGBOX - 30, 18, 10 }, iHandlerButton_no_1, iHandlerButton_no_2, EBUTTON_2 );
			break;

		case IDTYPE_OkCancel:
			AddButton( Rectangle2D{ 35, HEIGHT_MSGBOX - 30, 19, 10 }, iHandlerButton_ok_1, iHandlerButton_ok_2, EBUTTON_1 );
			AddButton( Rectangle2D{ WIDTH_MSGBOX - 35 - 47, HEIGHT_MSGBOX - 30, 47, 10 }, iHandlerButton_cancel_1, iHandlerButton_cancel_2, EBUTTON_2 );
			break;

		default:
			break;
	}
}

int CUIMessageBox::GetWidthButton( EButton eButton )
{
	if ( iTypeBox == IDTYPE_YesNo )
	{
		if ( eButton == EBUTTON_1 )
			return 27;
		else
			return 18;
	}
	else
	{
		if ( eButton == EBUTTON_1 )
			return 19;
		else
			return 47;
	}
}

int CUIMessageBox::GetHeightButton( EButton eButton )
{
	return 10;
}

int CUIMessageBox::GetXButton( EButton eButton )
{
	if ( iTypeBox == IDTYPE_YesNo )
	{
		if ( eButton == EBUTTON_1 )
			return 40;
		else
			return WIDTH_MSGBOX - 40 - 18;
	}
	else
	{
		if ( eButton == EBUTTON_1 )
			return 35;
		else
			return WIDTH_MSGBOX - 35 - 47;
	}
}

int CUIMessageBox::GetYButton( EButton eButton )
{
	return HEIGHT_MSGBOX - 30;
}

void CUIMessageBox::SetAutomaticCloseTimer( float fTime, EButton eButton )
{
	eButtonAutomaticFire = eButton;

	fTimeBox = fTime;
	pTimer = std::make_shared<Timer>( fTime );
	pTimer->SetMaxTicks( 1 );
	pTimer->SetEvent( UI::Event::Build( std::bind( &CUIMessageBox::OnAutomaticClose, this, std::placeholders::_1 ) ) );
	bStartedTime = FALSE;
}

void CUIMessageBox::OnAutomaticClose( UIEventArgs eArgs )
{
	if ( bClosedMessageBox )
		return;
	
	bClosedMessageBox = TRUE;

	if ( iEventCloseMsgBox != -1 )
		UI::Event::Invoke( iEventCloseMsgBox );
}

void CUIMessageBox::OnEventClose()
{
	if ( bClosedMessageBox )
		return;

	bClosedMessageBox = TRUE;

	if ( iEventCloseMsgBox != -1 )
		UI::Event::Invoke( iEventCloseMsgBox );
}

void CUIMessageBox::SetEvent( int iEvent )
{
	iEventMsgBox = iEvent;
}

void CUIMessageBox::SetCloseEvent( int iEvent )
{
	iEventCloseMsgBox = iEvent;
}

BOOL CUIMessageBox::OnMouseClick( EButton eButton )
{

	if ( bClosedMessageBox )
		return FALSE;

	Rectangle2D rBox = { ( DX::Graphic::GetWidthBackBuffer() >> 1 ) - ( WIDTH_MSGBOX >> 1 ), ( DX::Graphic::GetHeightBackBuffer() >> 1 ) - ( HEIGHT_MSGBOX >> 1 ), WIDTH_MSGBOX, HEIGHT_MSGBOX };

	if ( UI::pGame::IsMousePosition( rBox.x + GetXButton( EBUTTON_1 ), rBox.y + GetYButton( EBUTTON_1 ), GetWidthButton( EBUTTON_1 ), GetHeightButton( EBUTTON_1 ) ) )
	{
		bClosedMessageBox = TRUE;
		UI::Event::Invoke( iEventMsgBox );
		return TRUE;
	}
	if ( UI::pGame::IsMousePosition( rBox.x + GetXButton( EBUTTON_2 ), rBox.y + GetYButton( EBUTTON_2 ), GetWidthButton( EBUTTON_2 ), GetHeightButton( EBUTTON_2 ) ) )
	{
		OnEventClose();
		return TRUE;
	}

	return FALSE;
}

void CUIMessageBox::Draw()
{
	if ( bClosedMessageBox )
		return;

	if ( !pControllerCheckBox )
		CreateType();

	if ( !bStartedTime && pTimer )
	{
		pTimer->Start();
		bStartedTime = TRUE;
	}
	pControllerCheckBox->Draw();

	if ( pFontTitle && pFontText )
	{
		
		int iXTitle = ( WIDTH_MSGBOX >> 1 ) - ( pFontTitle->GetWidth() >> 1 );
		pFontTitle->Draw( iXTitle + pControllerCheckBox->r.x, pControllerCheckBox->r.y + 10 );
		
		RECT rBox = { ( DX::Graphic::GetWidthBackBuffer() >> 1 ) - ( WIDTH_MSGBOX >> 1 ), ( DX::Graphic::GetHeightBackBuffer() >> 1 ) - ( HEIGHT_MSGBOX >> 1 ), WIDTH_MSGBOX, HEIGHT_MSGBOX };
		pFontText->Draw( rBox );
	}
}
