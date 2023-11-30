#include "stdafx.h"
#include "CAntiDebuggerHandler.h"

DWORD CAntiDebuggerHandler::dwTimeUpdateCrash = 0;

void CAntiDebuggerHandler::Execute()
{
#if defined(_DEBUG) || defined(_ANTICHEAT_OFF_)
	return;
#endif

	if ( CAntiDebuggerHandler::dwTimeUpdateCrash != 0 )
	{
		if ( (CAntiDebuggerHandler::dwTimeUpdateCrash + 2000) < TICKCOUNT )
		{
			CAntiDebuggerHandler::dwTimeUpdateCrash = 0;
			CRASH;
		}
	}
	else
	{
		if ( SOCKETL == NULL )
			return;

		CAntiDebuggerHandler::FindDebugger1();
		CAntiDebuggerHandler::FindDebugger2();
		CAntiDebuggerHandler::FindDebugger3();
		CAntiDebuggerHandler::FindDebugger4();
		CAntiDebuggerHandler::FindDebugger5();
		CAntiDebuggerHandler::FindDebugger6();
		CAntiDebuggerHandler::FindDebugger7();
		CAntiDebuggerHandler::FindDebugger8();
		CAntiDebuggerHandler::FindDebugger9();
		CAntiDebuggerHandler::FindDebugger10();
		CAntiDebuggerHandler::FindDebugger11();
	}
}

HANDLE CAntiDebuggerHandler::CreateHiddenThread( LPTHREAD_START_ROUTINE pAddr, LPVOID pParameter, DWORD dwStackSize )
{
	//Set Flags
	DWORD dwFlags = CREATE_SUSPENDED;
	if ( dwStackSize != 0 )
		dwFlags |= STACK_SIZE_PARAM_IS_A_RESERVATION;

	//Create Thread (Suspended)
	HANDLE hThreadHandle = CreateRemoteThread( GetCurrentProcess(), 0, dwStackSize, (LPTHREAD_START_ROUTINE)GetModuleHandleA( AY_OBFUSCATE( "ntdll.dll" ) ), pParameter, dwFlags, 0 );

	//Get Thread Context
	CONTEXT sThreadContext;
	sThreadContext.ContextFlags = CONTEXT_INTEGER;
	GetThreadContext( hThreadHandle, &sThreadContext );
	
	//Update Thread Start Address
	sThreadContext.Eax = (DWORD)pAddr;
	sThreadContext.ContextFlags = CONTEXT_INTEGER;
	SetThreadContext( hThreadHandle, &sThreadContext );

	//Resume Thread
	ResumeThread( hThreadHandle );

	return hThreadHandle;
}

void CAntiDebuggerHandler::FindDebugger1()
{
	__try { __asm INT 0x2D }
	__except ( EXCEPTION_EXECUTE_HANDLER ) { return; }

	CAntiDebuggerHandler::SendDebuggerPresent();
}

void CAntiDebuggerHandler::FindDebugger2()
{
	__try { __asm INT 0x03 }
	__except ( EXCEPTION_EXECUTE_HANDLER ) { return; }	

	CAntiDebuggerHandler::SendDebuggerPresent();
}

void CAntiDebuggerHandler::FindDebugger3()
{
	CONTEXT ctx = { 0 };
	ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;

	auto hThread = GetCurrentThread();

	if ( GetThreadContext( hThread, &ctx ) == 0 )
	{
		return;
	}

	//Debugger Found
	if ( ctx.Dr0 != 0 || ctx.Dr1 != 0 || ctx.Dr2 != 0 || ctx.Dr3 != 0 )
	{
		CAntiDebuggerHandler::SendDebuggerPresent();
	}
}

void CAntiDebuggerHandler::FindDebugger4()
{
	//Crash OllyDBG v1.10
	OutputDebugString( "%s%s%s%s" );
}

void CAntiDebuggerHandler::FindDebugger5()
{
	__try { RaiseException( DBG_RIPEXCEPTION, 0, 0, 0 ); }
	__except ( EXCEPTION_EXECUTE_HANDLER ) { return; }	

	CAntiDebuggerHandler::SendDebuggerPresent();
}

void CAntiDebuggerHandler::FindDebugger6()
{
	std::vector<const char *> drivers = {
		AY_OBFUSCATE( "\\\\.\\EXTREM" ),
		AY_OBFUSCATE( "\\\\.\\ICEEXT" ),
		AY_OBFUSCATE( "\\\\.\\NDBGMSG.VXD" ),
		AY_OBFUSCATE( "\\\\.\\RING0" ),
		AY_OBFUSCATE( "\\\\.\\SIWVID" ),
		AY_OBFUSCATE( "\\\\.\\SYSER" ),
		AY_OBFUSCATE( "\\\\.\\TRW" ),
		AY_OBFUSCATE( "\\\\.\\SYSERBOOT" ) };

	for ( const auto & driver : drivers )
	{
		auto h = CreateFileA( driver, 0, 0, 0, OPEN_EXISTING, 0, 0 );
		if ( h != INVALID_HANDLE_VALUE )
		{
			CloseHandle( h );

			CAntiDebuggerHandler::SendDebuggerPresent();
		}
	}
}

void CAntiDebuggerHandler::FindDebugger7()
{
	HANDLE hDebugObject = NULL;

	//ProcessDebugObjectHandle
//	if ( NtQueryInformationProcess( GetCurrentProcess(), (PROCESSINFOCLASS)0x1E, &hDebugObject, 4, NULL ) != 0 )
	{
		return;
	}

	//Debugger Found
	if ( hDebugObject )
	{
		//CAntiDebuggerHandler::SendDebuggerPresent();
	}
}

void CAntiDebuggerHandler::FindDebugger8()
{
	unsigned char * pMem = nullptr;
	SYSTEM_INFO sysinfo = { 0 };
	DWORD OldProtect = 0;
	void * pAllocation = nullptr;

	GetSystemInfo( &sysinfo );
	pAllocation = VirtualAlloc( NULL, sysinfo.dwPageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE );

	//Not Found
	if ( pAllocation == NULL )
	{	
		return;
	}

	//Write a ret to the buffer (opcode 0xc3)
	pMem = (unsigned char *)pAllocation;
	*pMem = 0xc3;

	//Make the page a guard page         
	if ( VirtualProtect( pAllocation, sysinfo.dwPageSize, PAGE_EXECUTE_READWRITE | PAGE_GUARD, &OldProtect ) == 0 )
	{		
		return;
	}

	__try
	{
		__asm
		{
			mov eax, pAllocation
			//This is the address we'll return to if we're under a debugger
			push MemBpBeingDebugged
			jmp eax //Exception or execution, which shall it be :D?
		}
	}
	__except ( EXCEPTION_EXECUTE_HANDLER )
	{
		//The exception occured and no debugger was detected
		VirtualFree( pAllocation, NULL, MEM_RELEASE );
		
		return;
	}

	__asm {MemBpBeingDebugged:}
	VirtualFree( pAllocation, NULL, MEM_RELEASE );

	CAntiDebuggerHandler::SendDebuggerPresent();
}

void CAntiDebuggerHandler::FindDebugger9()
{
	__try
	{
		__asm __emit 0xF3 //0xF3 0x64 disassembles as PREFIX REP:
		__asm __emit 0x64
		__asm __emit 0xF1 //One byte INT 1
	}
	__except ( EXCEPTION_EXECUTE_HANDLER )
	{	
		return;
	}	

	CAntiDebuggerHandler::SendDebuggerPresent();
}

void CAntiDebuggerHandler::FindDebugger10()
{
	BOOL bFound = FALSE;
	_asm
	{
		xor eax, eax;			//clear eax
		mov eax, fs: [0x30] ;	//Reference start of the PEB
		mov eax, [eax + 0x68];	//PEB+0x68 points to NtGlobalFlags
		and eax, 0x00000070;	//check three flags
		mov bFound, eax;		//Copy result into 'found'
	}

	if ( bFound )
	{
		CAntiDebuggerHandler::SendDebuggerPresent();
	}
}

void CAntiDebuggerHandler::FindDebugger11()
{
	HANDLE hInvalid = (HANDLE)0xDEADBEEF; //An invalid handle
	BOOL bFound = FALSE;

	__try { CloseHandle( hInvalid ); }
	__except ( EXCEPTION_EXECUTE_HANDLER ) { bFound = TRUE; }

	if ( bFound )
	{
		CAntiDebuggerHandler::SendDebuggerPresent();
	}
}

void CAntiDebuggerHandler::SendDebuggerPresent()
{
	//Send log hack checksum
	PacketLogCheat sPacket;
	sPacket.iLength = sizeof( PacketLogCheat );
	sPacket.iHeader = PKTHDR_LogCheat;
	sPacket.iCheatID = CHEATLOGID_DebuggerHack;

	if ( SOCKETL )
		SENDPACKETL( &sPacket );

	CAntiDebuggerHandler::dwTimeUpdateCrash = TICKCOUNT;
}
