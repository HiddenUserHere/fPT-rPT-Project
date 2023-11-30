#include "stdafx.h"
#include "WinDump.h"
#include "CServer.h"

BOOL HookAPI( char* moduleName, char* apiName, void* callBack, void* backup = NULL )
{
	DWORD old;

	if ( auto hModuleHandle = GetModuleHandleA( moduleName ) )
	{
		void * pAPIHandle = GetProcAddress( hModuleHandle, apiName );

		if ( pAPIHandle == NULL )
			return FALSE;

		if ( backup != NULL )
			MoveMemory( backup, pAPIHandle, 5 );

		VirtualProtect( pAPIHandle, 5, PAGE_EXECUTE_READWRITE, &old );
		*(BYTE *)((DWORD)pAPIHandle) = 0xE9;
		*(DWORD *)((DWORD)pAPIHandle + 1) = (DWORD)callBack - (DWORD)pAPIHandle - 5;
		VirtualProtect( pAPIHandle, 5, old, &old );
	}

	return TRUE;
}

LONG WINAPI RedirectedSetUnhandledExceptionFilter( EXCEPTION_POINTERS * p )
{
	return NULL;
}

void Launch()
{
	STARTUPINFOA s = { 0 };
	PROCESS_INFORMATION p = { 0 };

	CreateProcessA( NULL, """server.exe""", NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &s, &p );
}

extern "C" int __stdcall WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	SetUnhandledExceptionFilter( unhandled_handler );
	HookAPI( "kernel32.dll","SetUnhandledExceptionFilter", RedirectedSetUnhandledExceptionFilter );

	//Aging
	{
		DWORD dwOld = 0;
		VirtualProtect( (void*)0x004C3BD4, 8, PAGE_EXECUTE_READWRITE, &dwOld );
		*(BYTE *)((DWORD)0x004C3BD4) = 0xB9;
		*(DWORD *)((DWORD)0x004C3BD4 + 1) = 1;
		*(WORD *)((DWORD)0x004C3BD4 + 5) = 0x9090;
	}

	//Pointer Window Procedure
	*( UINT* )0x08B813B0 = ( UINT )0x00574000;
	*(UINT*)0x08B867C0 = (UINT)&GetMessageA;

	pcApplication = new CApplication( new CServerWindow() );

	if ( pcApplication->Init( hInstance ) )
	{
		pcApplication->Run();

		if ( pcApplication->Shutdown() )
			Launch();
	}
	else
	{
		if ( pcApplication->GetInitCode() == -1 )
			Launch();
	}

	DELET( pcApplication );

	return 0;
}