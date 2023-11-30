#pragma once
class CGameDump
{
public:
	CGameDump();
	virtual ~CGameDump();

	static void							SendErrorReport( EXCEPTION_POINTERS * pep );

};

