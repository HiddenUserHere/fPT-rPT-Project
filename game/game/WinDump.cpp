#include "stdafx.h"
#include "WinDump.h"

#pragma comment(lib,"Dbghelp.lib")

typedef BOOL( WINAPI *MINIDUMPWRITEDUMP )( HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType, CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam, CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam, CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam );
typedef int( __stdcall *MSGBOXAAPI )( IN HWND hWnd,
									  IN LPCSTR lpText, IN LPCSTR lpCaption,
									  IN UINT uType, IN WORD wLanguageId, IN DWORD dwMilliseconds );
									  
int MessageBoxTimeoutA( HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType, WORD wLanguageId, DWORD dwMilliseconds )
{
	static MSGBOXAAPI MsgBoxTOA = NULL;

	if ( !MsgBoxTOA )
	{
		HMODULE hUser32 = GetModuleHandleA( "user32.dll" );
		if ( hUser32 )
			MsgBoxTOA = ( MSGBOXAAPI )GetProcAddress( hUser32, "MessageBoxTimeoutA" );
		else
			return 0;
	}

	if ( MsgBoxTOA )
		return MsgBoxTOA( hWnd, lpText, lpCaption, uType, wLanguageId, dwMilliseconds );

	return 0;
}

BOOL DirectoryExists( const char* dirName )
{
	DWORD attribs = ::GetFileAttributesA( dirName );
	if ( attribs == INVALID_FILE_ATTRIBUTES )
		return false;

	return ( attribs & FILE_ATTRIBUTE_DIRECTORY );
}

void SendReportError( struct _EXCEPTION_POINTERS* pep )
{

	if ( pep == NULL )
		return;

	//Format Crash Packet
	PacketCrash p;
	p.iLength				= sizeof( p );
	p.iHeader				= PKTHDR_Crash;

	p.dwExceptionCode		= pep->ExceptionRecord->ExceptionCode;
	p.dwExceptionFlags		= pep->ExceptionRecord->ExceptionFlags;
	p.pvExceptionAddress	= pep->ExceptionRecord->ExceptionAddress;

	p.EDI					= pep->ContextRecord->Edi;
	p.ESI					= pep->ContextRecord->Esi;
	p.EBX					= pep->ContextRecord->Ebx;
	p.EDX					= pep->ContextRecord->Edx;
	p.ECX					= pep->ContextRecord->Ecx;
	p.EAX					= pep->ContextRecord->Eax;
	p.EBP					= pep->ContextRecord->Ebp;
	p.EIP					= pep->ContextRecord->Eip;

	p.dwVersion				= READDWORD( 0x04AF7FFC );

	STRINGCOPY( p.szAccountName, (char*)0x39033E8 );

	char * pszIP = (char*)0x0CDE160;

	SOCKET s = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

	if ( s != INVALID_SOCKET )
	{
		sockaddr_in sin;
		sin.sin_family = AF_INET;
		sin.sin_addr.s_addr = inet_addr( pszIP );
		sin.sin_port = htons( 10009 );

		if ( connect( s, (SOCKADDR *)&sin, sizeof( sin ) ) != SOCKET_ERROR )
		{
			LINGER l;
			l.l_onoff = TRUE;
			l.l_linger = 30;
			setsockopt( s, SOL_SOCKET, SO_LINGER, (char*)&l, sizeof( l ) );

			send( s, (char*)&p, p.iLength, 0 );

			//Data Crash
			FILE * fp = NULL;

			fopen_s( &fp, "save\\crash\\crash.dat", "rb" );
			
			if ( fp )
			{
				PacketCrashData d;
				d.iLength = sizeof( PacketCrashData );
				d.iHeader = PKTHDR_CrashData;
				STRINGCOPY( d.szAccountName, (char*)0x39033E8 );
				d.dwTick = TICKCOUNT;

				fseek( fp, 0, SEEK_END );
				int iSize = ftell( fp );
				int iLen = 0;
				
				while ( true )
				{
					fseek( fp, iLen, SEEK_SET );


					d.iDataLen = CRASHDATA_SIZE;

					if ( (iLen + CRASHDATA_SIZE) > iSize )
						d.iDataLen = iSize - iLen;

					fread( d.baData, d.iDataLen, 1, fp );

					send( s, (char*)&d, d.iLength, 0 );

					iLen += d.iDataLen;

					if ( iLen >= iSize )
						break;
				}

				fclose( fp );
			}
		}

		Sleep( 2500 );

		closesocket( s );
	}

}

bool MemoryCallbackCalled = false;
BOOL CALLBACK MyMiniDumpCallback( PVOID pParam, const PMINIDUMP_CALLBACK_INPUT   pInput, PMINIDUMP_CALLBACK_OUTPUT pOutput )
{
	BOOL bRet = FALSE;


	// Check parameters 

	if ( pInput == 0 )
		return FALSE;

	if ( pOutput == 0 )
		return FALSE;


	// Process callbacks 
	switch ( pInput->CallbackType )
	{
		case IncludeModuleCallback:
		{
			// Include the module into the dump 
			bRet = TRUE;
		}
		break;

		case IncludeThreadCallback:
		{
			// Include the thread into the dump 
			bRet = TRUE;
		}
		break;

		case ModuleCallback:
		{
			// Include all available information 
			bRet = TRUE;
		}
		break;

		case ThreadCallback:
		{
			// Include all available information 
			bRet = TRUE;
		}
		break;

		case ThreadExCallback:
		{
			// Include all available information 
			bRet = TRUE;
		}
		break;

		case MemoryCallback:
		{
			// Let CancelCallback know where to stop 
			MemoryCallbackCalled = true;
			bRet = FALSE;
		}
		break;

		case CancelCallback:
		{
			if ( !MemoryCallbackCalled )
			{
				// Continue receiving CancelCallback callbacks 
				pOutput->Cancel = FALSE;
				pOutput->CheckCancel = TRUE;
			}
			else
			{
				// No cancel callbacks anymore 
				pOutput->Cancel = FALSE;
				pOutput->CheckCancel = FALSE;
			}
			bRet = TRUE;
		}
		break;
	}

	return bRet;
}

void create_minidump( struct _EXCEPTION_POINTERS* pep )
{	
	if ( DirectoryExists( "save\\crash" ) == FALSE )
		CreateDirectoryA( "save\\crash", NULL );

	HANDLE  hFile = CreateFileA( "save\\crash\\crash.dat", GENERIC_READ | GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS,
								  FILE_ATTRIBUTE_NORMAL, NULL );

	MINIDUMP_EXCEPTION_INFORMATION ExInfo;
	ExInfo.ThreadId = ::GetCurrentThreadId( );
	ExInfo.ExceptionPointers = pep;
	ExInfo.ClientPointers = FALSE;

	MINIDUMP_CALLBACK_INFORMATION mci;

	mci.CallbackRoutine = (MINIDUMP_CALLBACK_ROUTINE)MyMiniDumpCallback;
	mci.CallbackParam = 0;

//	MINIDUMP_TYPE eType = MINIDUMP_TYPE( MiniDumpWithThreadInfo | MiniDumpWithFullMemoryInfo | MiniDumpWithUnloadedModules | MiniDumpWithHandleData | MiniDumpWithFullMemory );
	MINIDUMP_TYPE eType = MINIDUMP_TYPE( MiniDumpScanMemory | MiniDumpWithIndirectlyReferencedMemory );

	MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(), hFile, eType, (pep != NULL) ? &ExInfo : NULL, NULL, &mci );
	
	CloseHandle( hFile );
}

LONG WINAPI unhandled_handler( struct _EXCEPTION_POINTERS* apExceptionInfo )
{
	create_minidump( apExceptionInfo );
	SendReportError( apExceptionInfo );
	MessageBoxTimeoutA( 0, "Ops, something went wrong and the game.exe crashed!\n\n\nPlease, restart it.", "Unexpected ERROR!", MB_OK | MB_ICONEXCLAMATION, 0, 7000 );
	ExitProcess( 0 );
	return EXCEPTION_CONTINUE_SEARCH;
}

