#include "stdafx.h"
#include "InputBox.h"

std::vector<std::function<void( UIEventArgs const & )>> vInputBox;

InputBox::InputBox()
{
}


InputBox::~InputBox()
{
}

void InputBox::Init()
{
	pWindow1 = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 241, 178 ) );
	pWindow1->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\messagebox\\background_normal.png" ) );

	UI::Button_ptr pButtonOK = std::make_shared<UI::Button>( Rectangle2D( 32, 113, 88, 35 ) );
	pButtonOK->SetID( BUTTONID_Ok );
	pButtonOK->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\messagebox\\btnok.png", "game\\images\\UI\\messagebox\\btnok_.png" ) );
	pButtonOK->SetEvent( UI::Event::Build( std::bind( &InputBox::OnOKButtonClick, this, TRUE, std::placeholders::_1 ) ) );
	pWindow1->AddElement( pButtonOK );

	UI::Button_ptr pButtonCancel = std::make_shared<UI::Button>( Rectangle2D( 121, 113, 88, 35 ) );
	pButtonCancel->SetID( BUTTONID_Cancel );
	pButtonCancel->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\messagebox\\btncancel.png", "game\\images\\UI\\messagebox\\btncancel_.png" ) );
	pButtonCancel->SetEvent( UI::Event::Build( std::bind( &InputBox::OnOKButtonClick, this, FALSE, std::placeholders::_1 ) ) );
	pWindow1->AddElement( pButtonCancel );

	UI::Bitmapfont_ptr pTextTitle = std::make_shared<UI::BitmapFont>( Rectangle2D( 0, 16, 241, 0 ) );
	pTextTitle->SetID( LABELID_Title );
	pTextTitle->SetHorizontalAlign( ALIGN_Center );
	pTextTitle->ReadFontData( "game\\fonts\\messagebox.dat" );
	pTextTitle->SetFontImage( UI::ImageLoader::LoadImage( "game\\fonts\\messagebox.png" ) );
	pWindow1->AddElement( pTextTitle );

	UI::Text_ptr pTextDescription = std::make_shared<UI::Text>( Rectangle2D( 12, 45, 216, 50 ) );
	pTextDescription->SetID( LABELID_Description );
	pTextDescription->SetFont( "Arial", 16, 0, FALSE, FALSE, D3DCOLOR_ARGB( 255, 245, 245, 245 ) );
	pTextDescription->SetText( "" );
	pTextDescription->SetMultiLine( TRUE );
	pTextDescription->SetWordWrap( TRUE );
	pTextDescription->SetHorizontalAlign( EAlign::ALIGN_Center );
	pWindow1->AddElement( pTextDescription );

	UI::Group_ptr pGroup = std::make_shared<UI::Group>();

	UI::ImageBox_ptr pImageLine = std::make_shared<UI::ImageBox>( Rectangle2D( 12, 108, 216, 1 ) );
	pImageLine->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\box\\line_big.png" ) );
	pWindow1->AddElement( pImageLine );

	UI::InputField_ptr pInputValue = std::make_shared<UI::InputField>( Rectangle2D( 12, 92, 216, 30 ) );
	pInputValue->SetID( INPUTID_Value );
	pInputValue->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pInputValue->SetTextAlign( EAlign::ALIGN_Center, EAlign::ALIGN_Top );
	pInputValue->SetGroup( pGroup );
	pInputValue->SetSelect( TRUE );
	pInputValue->SetCanScroll( TRUE );
	pWindow1->AddElement( pInputValue );

	pWindow1->AddElement( pGroup );

	pWindow1->Hide();
}

void InputBox::Render()
{
	pWindow1->SetPosition( (DX::Graphic::GetWidthBackBuffer() >> 1) - (241 >> 1), (DX::Graphic::GetHeightBackBuffer() >> 1) - (178 >> 1) );
	pWindow1->Render( 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );

	for ( std::vector<SImageAdd*>::iterator it = vImages.begin(); it != vImages.end(); it++ )
	{
		SImageAdd * ps = (*it);

		if ( ps && ps->pcImage )
			UI::ImageRender::Render( ps->pcImage, ps->sBox.iX + pWindow1->GetX(), ps->sBox.iY + pWindow1->GetY(), ps->sBox.iWidth, ps->sBox.iHeight, -1 );
	}
}


void InputBox::OnMouseMove( class CMouse * pcMouse )
{
	pWindow1->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

BOOL InputBox::OnMouseClick( class CMouse * pcMouse )
{
	if ( pWindow1->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;
	return FALSE;
}

BOOL InputBox::OnMouseScroll( CMouse * pcMouse )
{
	return pWindow1->OnMouseScroll( pcMouse, pWindow1->GetX(), pWindow1->GetY(), DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

BOOL InputBox::OnKeyPress( class CKeyboard * pcKeyboard )
{
	if ( pWindow1->OnKeyPress( pcKeyboard ) )
		return TRUE;
	return FALSE;
}

BOOL InputBox::OnKeyChar( class CKeyboard * pcKeyboard )
{
	if ( pWindow1->OnKeyChar( pcKeyboard ) )
		return TRUE;
	return FALSE;
}

int InputBox::GetValue()
{
	UI::InputField_ptr pInput = pWindow1->GetElement<UI::InputField>( INPUTID_Value );
	return atoi( pInput->GetText().c_str() );
}

const char * InputBox::GetText()
{
	UI::InputField_ptr pInput = pWindow1->GetElement<UI::InputField>( INPUTID_Value );
	return pInput->GetText().c_str();
}

void InputBox::SetNumberType( int iMin, int iMax, BOOL bCanSelect )
{
	UI::InputField_ptr pInput = pWindow1->GetElement<UI::InputField>( INPUTID_Value );
	pInput->SetIsNumber( TRUE, iMin, iMax );
	pInput->SetSelect( bCanSelect );
}

void InputBox::SetTitle( char * pszText )
{
	UI::Bitmapfont_ptr pText = pWindow1->GetElement<UI::BitmapFont>( LABELID_Title );
	pText->SetText( pszText );
}

void InputBox::SetDescription( char * pszText )
{
	UI::Text_ptr pText = pWindow1->GetElement<UI::Text>( LABELID_Description );
	pText->SetText( pszText );
}

void InputBox::SetValue( char * pszText )
{
	UI::InputField_ptr pInput = pWindow1->GetElement<UI::InputField>( INPUTID_Value );
	pInput->SetText( pszText );
}

void InputBox::SetEvent( std::function<void( UIEventArgs const& )> const & f )
{
	vInputBox.clear();
	vInputBox.push_back( f );
}

void InputBox::AddImage( UI::Image * pcImage, Rectangle2D sBox )
{
	SImageAdd * ps	= new SImageAdd;
	ps->pcImage		= pcImage;
	ps->sBox		= sBox;

	vImages.push_back( ps );
}

void InputBox::Hide()
{

	UI::InputField_ptr pInput = pWindow1->GetElement<UI::InputField>( INPUTID_Value );
	pInput->SetIsNumber( FALSE, 0, 0 );

	vInputBox.clear();

	bType = FALSE;

	pWindow1->Hide();

	for ( std::vector<SImageAdd*>::iterator it = vImages.begin(); it != vImages.end(); it++ )
	{
		SImageAdd * ps = (*it);
		delete ps;
	}

	vImages.clear();
}

void InputBox::Show()
{
	bType = FALSE;

	if ( pWindow1->IsOpen() == FALSE )
		pWindow1->Show();

	UI::InputField_ptr pInput = pWindow1->GetElement<UI::InputField>( INPUTID_Value );
	pInput->SetSelected( TRUE );

	MOUSEHANDLER->BlockMouse();
}

void InputBox::OnOKButtonClick( BOOL bTyped, UIEventArgs eArgs )
{
	bType = bTyped;

	MOUSEHANDLER->UnblockMouse();

	if ( vInputBox.size() > 0 )
		vInputBox[0]( UIEventArgs{} );

	Hide();
}
