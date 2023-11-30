#include "stdafx.h"
#include "UILine.h"


ID3DXLine *pLineUIGlobal = NULL;
BOOL bCreatedLineUI = FALSE;

CUILine::CUILine()
{
	if ( !bCreatedLineUI )
	{
		D3DXCreateLine( DX::Graphic::GetDevice(), &pLineUIGlobal );
		bCreatedLineUI = TRUE;
	}
}

CUILine::~CUILine()
{
}

void CUILine::DrawLineRect( int x, int y, int w, int h, DWORD Color )
{
	D3DXVECTOR2 vLine[ 2 ];

	pLineUIGlobal->SetWidth( ( float )w );
	pLineUIGlobal->SetAntialias( false );
	pLineUIGlobal->SetGLLines( true );

	vLine[ 0 ].x = ( float )x + ( float )w / 2;
	vLine[ 0 ].y = ( float )y;
	vLine[ 1 ].x = ( float )x + ( float )w / 2;
	vLine[ 1 ].y = ( float )y + ( float )h;

	pLineUIGlobal->Begin();
	pLineUIGlobal->Draw( vLine, 2, Color );
	pLineUIGlobal->End();
}

void CUILine::DrawLine( Rectangle2D r, DWORD Color )
{
	
#ifdef PT_DEV_MODE_UI
	this->DrawLineRect( r.x, r.y, r.w, 1, Color );
	this->DrawLineRect( r.x, r.y + r.h - 1, r.w, 1, Color );
	this->DrawLineRect( r.x, r.y, 1, r.h, Color );
	this->DrawLineRect( r.x + r.w - 1, r.y, 1, r.h, Color );
#endif
	
}

void CUILine::OnResetDevice()
{
	if ( pLineUIGlobal )
		pLineUIGlobal->OnResetDevice();
}

void CUILine::OnLostDevice()
{
	if ( pLineUIGlobal )
		pLineUIGlobal->OnLostDevice();
}
