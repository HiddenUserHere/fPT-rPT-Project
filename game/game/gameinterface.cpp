#include "stdafx.h"
#include "gameinterface.h"


CGameInterface::CGameInterface()
{
}


CGameInterface::~CGameInterface()
{
}

NAKED void CGameInterface::OnMouseLDownClick( int iX, int iY )
{
	JMP( pfnOnMouseLClickDown );
}
