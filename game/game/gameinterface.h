#pragma once
class CGameInterface
{
protected:
	IMPFNC						pfnOnMouseLClickDown = 0x004AB1A0;

public:
	CGameInterface();
	~CGameInterface();

	static CGameInterface		* GetInstance() { return (CGameInterface*)0x035E11D0; }

	void						OnMouseLDownClick( int iX, int iY );
};

