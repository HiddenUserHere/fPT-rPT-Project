#include "stdafx.h"
#include "gamebellatra.h"


CGameBellatra::CGameBellatra()
{
}


CGameBellatra::~CGameBellatra()
{
}

void CGameBellatra::OnMouseLDownClick( int iX, int iY )
{
	JMP( pfnOnMouseLClickDown );
}
