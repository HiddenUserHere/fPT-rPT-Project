#include "stdafx.h"
#include "CProcess.h"

CProcess::CProcess()
{
	WSADATA	wsaData;
	WSAStartup( MAKEWORD( 2, 2 ), &wsaData );

	srand( GetTickCount() );
}

CProcess::~CProcess()
{
	WSACleanup();
}

void CProcess::SetInstanceHandle( HINSTANCE _hInstance )
{
	hInstance = _hInstance;
}

void CProcess::ForceExit( UINT uExitCode )
{
	ExitProcess( uExitCode );
}