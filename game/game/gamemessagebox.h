#pragma once
class CGameMessageBox
{
protected:
	IMPFNC						pfnOnMouseLClickDown = 0x004D53D0;

public:
	CGameMessageBox();
	~CGameMessageBox();

	static CGameMessageBox		* GetInstance() { return (CGameMessageBox*)0x0362B210; }

	void						OnMouseLDownClick( int iX, int iY );
};

