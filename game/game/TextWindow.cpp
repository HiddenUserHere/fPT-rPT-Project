#include "stdafx.h"
#include "TextWindow.h"


__declspec(naked) void CTextWindow::Open( int iType, int iX, int iY, int iWidth, int iHeight, int iID, char * pszTextFilePath )
{
	JMP( pfnOpen );
}

__declspec( naked ) void CTextWindow::Scroll( int iDistance )
{
	JMP( pfnScroll );
}

void CTextWindow::Closing( int iWindowType )
{
	switch ( iWindowType )
	{
		case QUESTID_Quest110A:


			break;
		default:
			break;
	}
}