#include "stdafx.h"
#include "CInventory.h"

CInventory::CInventory()
{
	pWindow1 = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 357, 535 ) );
}

CInventory::~CInventory()
{
}

void CInventory::Init()
{
	BuildWindow1();
}

void CInventory::Render()
{
	pWindow1->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

void CInventory::Update( float fTime )
{
	pWindow1->Update( fTime );

	//Update Slot Position
	int iBaseX = pWindow1->GetX();
	int iBaseY = pWindow1->GetY();
}

void CInventory::OnMouseMove( CMouse * pcMouse )
{
	pWindow1->OnMouseMove( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

BOOL CInventory::OnMouseClick( CMouse * pcMouse )
{
	if( pWindow1->OnMouseClick( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 ) )
		return TRUE;

	return FALSE;
}

BOOL CInventory::OnKeyPress( CKeyboard * pcKeyboard )
{
	return 0;
}

BOOL CInventory::OnKeyChar( CKeyboard * pcKeyboard )
{
	return 0;
}

void CInventory::BuildWindow1()
{
	pWindow1->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\windowside.png" ) );
	pWindow1->SetMoveBox( pWindow1->GetRect() );
	pWindow1->SetCanMove( TRUE );

	UI::ItemBox_ptr pItemBox = std::make_shared<UI::ItemBox>( 30, 30, 10, 7 );
	pItemBox->SetMaxItems( 100 );
	pItemBox->SetFreePlace( TRUE );
	pWindow1->AddElement( pItemBox );

	pWindow1->Hide();
}
