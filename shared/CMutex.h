#pragma once
#include <Windows.h>

class CMutex
{
public:
	CMutex();
	CMutex( std::string strName, LPCRITICAL_SECTION lp = NULL );

	void							Lock( int iID = 0 );
	BOOL							Unlock();
protected:
	LPCRITICAL_SECTION				psCriticalSection = NULL;
	int								iIDMutex = 0;

	BOOL							bLocked = FALSE;

	char							szName[32];

	DWORD							dwLockTime = 0;

	void							* pReturnAddress = nullptr;
};