#include "stdafx.h"
#include "CCharacterRaceWindowHandler.h"


CCharacterRaceWindowHandler::CCharacterRaceWindowHandler()
{
	pWindow = std::make_shared<UI::Window>( Rectangle2D( (RESOLUTION_WIDTH >> 1) - (304 >> 1), (RESOLUTION_HEIGHT >> 2), 304, 148 ) );
}


CCharacterRaceWindowHandler::~CCharacterRaceWindowHandler()
{
}

void CCharacterRaceWindowHandler::Init()
{
	UI::Button_ptr pButtonMorion = std::make_shared<UI::Button>( Rectangle2D( 10, 10, 128, 128 ) );
	pButtonMorion->SetID( BUTTONID_Tempskron );
	pButtonMorion->SetImage( UI::ImageLoader::LoadImage( "StartImage\\login\\CharSelect\\mark_M.tga" ) );
	pButtonMorion->SetAlphaHover( 180 );
	pButtonMorion->SetEvent( UI::Event::Build( std::bind( &CCharacterRaceWindowHandler::OnMorionButtonClick, this, std::placeholders::_1 ) ) );
	pWindow->AddElement( pButtonMorion );

	UI::Button_ptr pButtonTempskron = std::make_shared<UI::Button>( Rectangle2D( 148, 10, 128, 128 ) );
	pButtonTempskron->SetID( BUTTONID_Tempskron );
	pButtonTempskron->SetImage( UI::ImageLoader::LoadImage( "StartImage\\login\\CharSelect\\mark_T.tga" ) );
	pButtonTempskron->SetAlphaHover( 180 );
	pButtonTempskron->SetEvent( UI::Event::Build( std::bind( &CCharacterRaceWindowHandler::OnTempskronButtonClick, this, std::placeholders::_1 ) ) );
	pWindow->AddElement( pButtonTempskron );
}

void CCharacterRaceWindowHandler::Render()
{
	pWindow->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

BOOL CCharacterRaceWindowHandler::OnMouseClick( CMouse * pcMouse )
{
	if ( pWindow->IsOpen() )
		return pWindow->OnMouseClick( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );

	return FALSE;
}

void CCharacterRaceWindowHandler::OnMouseMove( CMouse * pcMouse )
{
	pWindow->OnMouseMove( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

void CCharacterRaceWindowHandler::OnResolutionChanged()
{
	pWindow->SetPosition( (RESOLUTION_WIDTH >> 1) - (304 >> 1), (RESOLUTION_HEIGHT >> 2) );
}

void CCharacterRaceWindowHandler::OnTempskronButtonClick( UIEventArgs eArgs )
{
}

void CCharacterRaceWindowHandler::OnMorionButtonClick( UIEventArgs eArgs )
{
}
