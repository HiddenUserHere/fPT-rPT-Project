
#include "stdafx.h"

#include "WinDump.h"

typedef	BOOL( WINAPI * MINIDUMP_WRITE_DUMP )(
	IN HANDLE			hProcess,
	IN DWORD			ProcessId,
	IN HANDLE			hFile,
	IN MINIDUMP_TYPE	DumpType,
	IN CONST PMINIDUMP_EXCEPTION_INFORMATION	ExceptionParam, OPTIONAL
	IN PMINIDUMP_USER_STREAM_INFORMATION		UserStreamParam, OPTIONAL
	IN PMINIDUMP_CALLBACK_INFORMATION			CallbackParam OPTIONAL
	);

void create_minidump( struct _EXCEPTION_POINTERS* apExceptionInfo )
{

	HMODULE	hDbgHelp;
	MINIDUMP_WRITE_DUMP	MiniDumpWriteDump_;

	if ( hDbgHelp = LoadLibraryA( "DBGHELP.DLL" ) )
	{
		if ( MiniDumpWriteDump_ = (MINIDUMP_WRITE_DUMP)GetProcAddress( hDbgHelp, "MiniDumpWriteDump" ) )
		{
			MINIDUMP_EXCEPTION_INFORMATION  M;
			HANDLE  hDump_File;

			M.ThreadId = GetCurrentThreadId();
			M.ExceptionPointers = apExceptionInfo;  //got by GetExceptionInformation()
			M.ClientPointers = 0;


			hDump_File = CreateFileA( ".\\crash.dat", GENERIC_WRITE, 0,
				NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

			MiniDumpWriteDump_( GetCurrentProcess(), GetCurrentProcessId(),
				hDump_File, MiniDumpWithFullMemory,
				(apExceptionInfo) ? &M : NULL, NULL, NULL );

			CloseHandle( hDump_File );
		}
	}
}

LONG WINAPI unhandled_handler( struct _EXCEPTION_POINTERS* apExceptionInfo )
{
	create_minidump( apExceptionInfo );
	ExitProcess( 0 );
	return EXCEPTION_CONTINUE_SEARCH;
}

