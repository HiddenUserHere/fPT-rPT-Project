#include "stdafx.h"
#include "gamemessagebox.h"


CGameMessageBox::CGameMessageBox()
{
}


CGameMessageBox::~CGameMessageBox()
{
}

NAKED void CGameMessageBox::OnMouseLDownClick( int iX, int iY )
{
	JMP( pfnOnMouseLClickDown );
}
