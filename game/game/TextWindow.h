#pragma once

class CTextWindow
{
private:
	static const DWORD		pfnOpen		= 0x0049FDA0;
	static const DWORD		pfnScroll	= 0x0049CC90;


public:
	static CTextWindow									* GetInstance() { return (CTextWindow*)0x035E05D8; };

	void												Open( int iType, int iX, int iY, int iWidth, int iHeight, int iID, char * pszTextFilePath );
	void												Scroll( int iDistance );

	void												Closing( int iWindowType );

};

#define TEXTWINDOW_OPEN(iID, tf)						{ CTextWindow::GetInstance()->Open( iID, 50, 45, 9, 10, 0x7D000F80, tf ); }

