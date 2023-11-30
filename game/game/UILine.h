#pragma once
#include "UI.h"

class CUILine
{
public:
	CUILine();
	virtual ~CUILine();

	void										DrawLine( Rectangle2D r, DWORD Color );

	static void									OnResetDevice();
	static void									OnLostDevice();

private:
	void										DrawLineRect( int x, int y, int w, int h, DWORD Color );
};

