#include "stdafx.h"
#include "CCharacterScreenWindowHandler.h"


CCharacterScreenWindowHandler::CCharacterScreenWindowHandler()
{
	pWindow = std::make_shared<UI::Window>( Rectangle2D( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT ) );
}


CCharacterScreenWindowHandler::~CCharacterScreenWindowHandler()
{
}

void CCharacterScreenWindowHandler::Init()
{
	//New Character Button
	UI::Button_ptr pNewCharacterButton = std::make_shared<UI::Button>( Rectangle2D( 0, 0, 256, 64 ) );
	pNewCharacterButton->SetID( BUTTONID_NewCharacter );
	pNewCharacterButton->SetImage( UI::ImageLoader::LoadImage( "StartImage\\login\\CharSelect\\New_char.tga" ) );
	pNewCharacterButton->SetAlphaHover( 180 );
	pNewCharacterButton->SetEvent( UI::Event::Build( std::bind( &CCharacterScreenWindowHandler::OnButtonNewCharacterClick, this, std::placeholders::_1 ) ) );
	pWindow->AddElement( pNewCharacterButton );

	//Character Info
	//UI::Window_ptr pWindowCharacterInfo

	UI::RectangleBox_ptr pRectangleBox = std::make_shared<UI::RectangleBox>( Rectangle2D( (RESOLUTION_WIDTH >> 1) - 130, RESOLUTION_HEIGHT - 80, 260, 80 ) );
	pRectangleBox->SetID( RECTBOXID_Bottom );
	pRectangleBox->SetColor( DX::Color( 235, 45, 39, 27 ), DX::Color( 245, 9, 23, 22 ) );
	pWindow->AddElement( pRectangleBox );

	//Window Bottom
	UI::Window_ptr pWindowBottom = std::make_shared<UI::Window>( Rectangle2D( (RESOLUTION_WIDTH >> 1) - 130, RESOLUTION_HEIGHT - 80, 260, 80 ) );
	pWindowBottom->SetID( WINDOWID_Bottom );
	pWindow->AddElement( pWindowBottom );

	//Back
	UI::Button_ptr pButtonBack = std::make_shared<UI::Button>( Rectangle2D( 130 - (93 >> 1), 40, 93, 40 ) );
	pButtonBack->SetID( BUTTONID_Back );
	pButtonBack->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\btnback.png", "game\\images\\UI\\window\\btnback_.png" ) );
	pButtonBack->SetEvent( UI::Event::Build( std::bind( &CCharacterScreenWindowHandler::OnButtonBackClick, this, std::placeholders::_1 ) ) );
	pWindowBottom->AddElement( pButtonBack );

	UpdateWindowPosition();
}

void CCharacterScreenWindowHandler::Render()
{
	pWindow->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

BOOL CCharacterScreenWindowHandler::OnMouseClick( CMouse * pcMouse )
{
	if ( pWindow->IsOpen() )
		return pWindow->OnMouseClick( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );

	return FALSE;
}

void CCharacterScreenWindowHandler::OnMouseMove( CMouse * pcMouse )
{
	pWindow->OnMouseMove( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

void CCharacterScreenWindowHandler::OnResolutionChanged()
{
	UpdateWindowPosition();
}

void CCharacterScreenWindowHandler::UpdateCharacterInfo( const CharacterData & sCharacterData )
{
	ClearText();

	AddString( "Name:", TRUE, DX::Color( 255, 255, 255, 255 ), true );
	AddString( sCharacterData.szName, TRUE, DX::Color( 255, 255, 255, 255 ), false );

	AddString( "Level:", TRUE, DX::Color( 255, 255, 255, 255 ), true );
	AddString( FormatString( "%d", sCharacterData.iLevel ), TRUE, DX::Color( 255, 255, 255, 255 ), false );

	AddString( "Tribe:", TRUE, DX::Color( 255, 255, 255, 255 ), true );
	AddString( GetCharacterRaceName( GetCharacterRace( sCharacterData.iClass ) ), TRUE, DX::Color( 255, 255, 255, 255 ), false );

	AddString( "Class:", TRUE, DX::Color( 255, 255, 255, 255 ), true );
	AddString( GetCharacterClassData( sCharacterData.iClass, 0 )->pszName, TRUE, DX::Color( 255, 255, 255, 255 ), false );

}

void CCharacterScreenWindowHandler::ClearText()
{
	vTextLeft.clear();
	vTextRight.clear();
}

void CCharacterScreenWindowHandler::AddString( const std::string strText, BOOL bBold, DX::Color cColor, bool bLeft )
{
	CharacterInfoText sCharacterInfoText;
	sCharacterInfoText.strText	= strText;
	sCharacterInfoText.bBold	= bBold;
	sCharacterInfoText.dwColor	= cColor.Get();

	if ( bLeft )
		vTextLeft.push_back( sCharacterInfoText );
	else
		vTextRight.push_back( sCharacterInfoText );
}

void CCharacterScreenWindowHandler::BuildCharacterText()
{
}

void CCharacterScreenWindowHandler::OnButtonNewCharacterClick( UIEventArgs eArgs )
{
}

void CCharacterScreenWindowHandler::OnButtonOkClick( UIEventArgs eArgs )
{
	
}

void CCharacterScreenWindowHandler::OnButtonBackClick( UIEventArgs eArgs )
{
	if ( GetCharacterScreen()->CanBackLoginScreen() )
		GetCharacterScreen()->UpdateScreen( SCREEN_Login );
}

void CCharacterScreenWindowHandler::UpdateWindowPosition()
{
	return;

	int iX = RESOLUTION_WIDTH >> 1;
	int iY = RESOLUTION_HEIGHT >> 1;

	pWindow->SetBox( Rectangle2D( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT ) );

	//Update Bottom
	pWindow->GetElement<UI::Window>( WINDOWID_Bottom )->SetBox( Rectangle2D( (RESOLUTION_WIDTH >> 1) - 130, RESOLUTION_HEIGHT - 110, 260, 200 ) );
	pWindow->GetElement<UI::RectangleBox>( RECTBOXID_Bottom )->SetBox( Rectangle2D( (RESOLUTION_WIDTH >> 1) - 130, RESOLUTION_HEIGHT - 80, 260, 80 ) );

	auto pButtonNewCharacter = pWindow->GetElement<UI::Button>( BUTTONID_NewCharacter );
	pButtonNewCharacter->SetPosition( iX - (pButtonNewCharacter->GetWidth() >> 1), (iY >> 2) + 20 );
}

CCharacterScreen * CCharacterScreenWindowHandler::GetCharacterScreen()
{
	return (CCharacterScreen*)GAMESCREEN->GetActiveScreen();
}
