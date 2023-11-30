#pragma once
class CGameBellatra
{
protected:
	IMPFNC						pfnOnMouseLClickDown = 0x004DFA60;

public:
	CGameBellatra();
	~CGameBellatra();

	static CGameBellatra		* GetInstance() { return (CGameBellatra*)0x0362BF88; }

	void						OnMouseLDownClick( int iX, int iY );
};

