#include "stdafx.h"
#include "LaunchGame.h"
#include <Shellapi.h>

CLaunchGame::CLaunchGame()
{

}

BOOL CLaunchGame::DecryptArg( char * Arg )
{
	int ilstrlenA = lstrlenA( Arg );
	if ( ilstrlenA > 2 )
	{
		if ( ( Arg[ 0 ] != '/' ) || ( Arg[ 1 ] != 'j' ) )
			return FALSE;
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CLaunchGame::IsVMWare()
{
	BOOL rc = TRUE;

	__try
	{
		__asm
		{
			push   edx
				push   ecx
				push   ebx

				mov    eax, 'VMXh'
				mov    ebx, 0			// any value but not the MAGIC VALUE
				mov    ecx, 10			// get VMWare version
				mov    edx, 'VX'		// port number

				in     eax, dx			// read port
										// on return EAX returns the VERSION
				cmp    ebx, 'VMXh'		// is it a reply from VMWare?
				setz[ rc ]				// set return value

				pop    ebx
				pop    ecx
				pop    edx
		}
	}
	__except ( EXCEPTION_EXECUTE_HANDLER )
	{
		rc = FALSE;
	}

	return rc;
}

// IsInsideVPC's exception filter
DWORD __forceinline IsInsideVPC_exceptionFilter( LPEXCEPTION_POINTERS ep )
{
	PCONTEXT ctx = ep->ContextRecord;

	ctx->Ebx = -1;								// Not running VPC
	ctx->Eip += 4;								// skip past the "call VPC" opcodes
	return EXCEPTION_CONTINUE_EXECUTION;
};

BOOL CLaunchGame::IsVirtualPC()
{
	BOOL rc = FALSE;

	__try
	{
		_asm push ebx
		_asm mov  ebx, 0 // It will stay ZERO if VPC is running
		_asm mov  eax, 1 // VPC function number

		// call VPC 
		_asm __emit 0Fh
		_asm __emit 3Fh
		_asm __emit 07h
		_asm __emit 0Bh

		_asm test ebx, ebx
		_asm setz[ rc ]
			_asm pop ebx
	}
	// The except block shouldn't get triggered if VPC is running!!
	__except ( IsInsideVPC_exceptionFilter( GetExceptionInformation() ) )
	{
	}

	return rc;
}

void CLaunchGame::OpenGameHandler( char * Args )
{
	if ( !DecryptArg( Args ) )
	{
#if !defined(_DEBUG) && !defined(_ANTICHEAT_OFF_)
		STARTUPINFOA stStart;
		PROCESS_INFORMATION hProcinfo;
		memset( &stStart, 0, sizeof( stStart ) );
		memset( &hProcinfo, 0, sizeof( hProcinfo ) );

		ShellExecuteA( NULL, "open", "launcher.exe", NULL, NULL, SW_SHOWNORMAL );
		ExitProcess( 0 );
#endif

	}

	if ( IsVMWare() )
		ExitProcess( 0 );
	if ( IsVirtualPC() )
		ExitProcess( 0 );
	
}

CLaunchGame::~CLaunchGame()
{
}
