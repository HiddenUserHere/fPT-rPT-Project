#include "stdafx.h"
#include "AntiCheat.h"


std::vector<ChecksumFunction> vChecksumFunction;

int CAntiCheat::iaSkillCoolDown[10][20];
DWORD CAntiCheat::dwThreadsID[8];

UINT CAntiCheat::uNumberDetectedAutoClick	= 0;
DWORD CAntiCheat::dwUpdateCheckAutoClick	= 0;
DWORD CAntiCheat::dwUpdateDetectedAutoClick = 0;
DWORD CAntiCheat::dwCheckSumDLL				= 0;
DWORD CAntiCheat::dwCheckSumEXE				= 0;

HHOOK hHookWindowHandle;

LRESULT __stdcall HookCallbackMouseClick( int nCode, WPARAM wParam, LPARAM lParam )
{
	if ( hHookWindowHandle )
	{
		if ( nCode >= 0 )
		{
			PMSLLHOOKSTRUCT psMouseHook = (PMSLLHOOKSTRUCT)lParam;
			if ( (wParam == WM_LBUTTONDOWN) || (wParam == WM_RBUTTONDOWN) )
			{
				if ( (psMouseHook->flags & 0x00000001) || (psMouseHook->flags & 0x00000002) )
				{
					CAntiCheat::uNumberDetectedAutoClick++;
				}
			}
		}
	}

	return CallNextHookEx( hHookWindowHandle, nCode, wParam, lParam );
}

CAntiCheat::CAntiCheat()
{
	ZeroMemory( &CAntiCheat::dwThreadsID, sizeof( DWORD ) * 8 );

	CAntiCheat::dwCheckSumDLL = CAntiCheat::GetModuleChecksum( "game.dll" );
	CAntiCheat::dwCheckSumEXE = CAntiCheat::GetModuleChecksum( "game.exe" );

	CAntiDebuggerHandler::CreateHiddenThread( (LPTHREAD_START_ROUTINE)&CAntiCheat::DeepCheck, NULL, KB16 );
	
	__asm
	{
		mov eax, fs: [0x30]        //PEB
		mov eax, [eax + 0x0c]    //PEB_LDR_DATA
		mov eax, [eax + 0x0c]    //InOrderModuleList
		mov dword ptr[eax + 0x20], 0x100000 //SizeOfImage
	}
}


CAntiCheat::~CAntiCheat()
{
	DELET( psPacketChecksum );
	DELET( psPacketWindowCheat );
	DELET( psPacketChecksum );
	DELET( psPacketChecksum );

#ifndef _DEBUG
	//TerminateThread( hThread, 0 );
#endif
}

std::string CAntiCheat::GetEncDecodeString( std::string str )
{
	std::string s = str;
	for ( auto & c : s )
		c ^= 0x6A;

	return s;
}

HWND FindWindowPartialName( char * pszWindowName, char * pszTempVar )
{
	char szTempName[256];
	char szTempName2[256];
	char szNameWindow[256];
	int iLenTitle = 0;

	ZeroMemory( szNameWindow, 256 );
	for ( int i = 0; i < lstrlenA( pszWindowName ); i++ )
		szNameWindow[i] = tolower( pszWindowName[i] );

	HWND hWndTemp = FindWindowA( NULL, NULL );
	while ( hWndTemp != 0 )
	{
		iLenTitle = GetWindowTextA( hWndTemp, szTempName, 255 );

		STRINGCOPY( szTempName2, szTempName );

		for ( int i = 0; i < lstrlenA( szTempName ); i++ )
			szTempName[i] = tolower( szTempName[i] );

		if ( lstrlenA( szTempName ) && strstr( szTempName, szNameWindow ) )
		{
			STRINGCOPYLEN( pszTempVar, 256, szTempName2 );
			break;
		}
		hWndTemp = GetWindow(hWndTemp, GW_HWNDNEXT);
	}



	return hWndTemp;
}


BOOL CAntiCheat::ChecksumCheckMismatch( ChecksumFunction * pChecksum )
{
	DWORD dwSize = pChecksum->dwSize;

	DWORD dwCheckSum = dwChecksumRet = 0;

	DWORD dwOld = 0;
	VirtualProtect( (void*)pChecksum->dwAddress, dwSize, PAGE_EXECUTE_READWRITE, &dwOld );

	// Loop checksum generation
	for ( DWORD i = 0; i < dwSize; i++ )
		dwCheckSum += (DWORD)(*(BYTE*)(pChecksum->dwAddress + i)) ^ psPacketChecksum->bKey;

	// Mismatch? return sucesss
	if ( GM_MODE || dwCheckSum == pChecksum->dwCheckSum )
	{
		VirtualProtect( (void*)pChecksum->dwAddress, dwSize, dwOld, NULL );
		return TRUE;
	}

	// Set checksum to global var and return fail
	dwChecksumRet = dwCheckSum;
	VirtualProtect( (void*)pChecksum->dwAddress, dwSize, dwOld, NULL );
	
	return FALSE;
}

BOOL CAntiCheat::GetModuleName( DWORD dwThreadID, char * pszBuf, int iSizeBuf )
{
	MODULEENTRY32 moduleEntry32;

	HANDLE hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE | TH32CS_SNAPALL, GetCurrentProcessId() );

	moduleEntry32.dwSize		= sizeof( MODULEENTRY32 );
	moduleEntry32.th32ModuleID	= 1;

	// First module works?
	if ( Module32First( hSnapshot, &moduleEntry32 ) )
	{
		// Mismatch thread? get module name
		if ( GetModuleHandleA(NULL) == moduleEntry32.hModule )
		{
			STRINGCOPYLEN( pszBuf, iSizeBuf, moduleEntry32.szExePath );
		}
		else
		{
			// Loop next module if first not mismatch
			while ( Module32Next( hSnapshot, &moduleEntry32 ) )
			{
				// Mismatch thread? get module name
				if ( GetModuleHandleA(NULL) == moduleEntry32.hModule )
				{
					STRINGCOPYLEN( pszBuf, iSizeBuf, moduleEntry32.szExePath );
					break;
				}
			}
		}
	}
	CloseHandle( hSnapshot );
	return FALSE;
}

BOOL CAntiCheat::AddNewThreadException( DWORD dwThreadID )
{
	if ( !dwThreadID )
		return FALSE;

	// Already added thread? no need re-add
	for ( int i = 0; i < _countof( CAntiCheat::dwThreadsID ); i++ )
	{
		if ( CAntiCheat::dwThreadsID[i] == dwThreadID )
			return TRUE;
	}
	// Add new thread exception
	for ( int i = 0; i < _countof( CAntiCheat::dwThreadsID ); i++ )
	{
		if ( !CAntiCheat::dwThreadsID[i] )
		{
			CAntiCheat::dwThreadsID[i] = dwThreadID;
			return TRUE;
		}
	}
	return FALSE;
}

void CAntiCheat::ChecksumFunctionHandler()
{
	for ( int i = 0; i < CHECKSUM_FUNCTION_TOTAL; i++ )
	{
		// Have Checksum?
		if ( psPacketChecksum && psPacketChecksum->sCheckSum[i].dwSize )
		{
			// Not mismatch checksum code?
			if ( !ChecksumCheckMismatch( &psPacketChecksum->sCheckSum[i] ) )
			{
				// Send log hack checksum
				PacketLogCheat sPacket;
				sPacket.iLength		= sizeof( PacketLogCheat );
				sPacket.iHeader		= PKTHDR_LogCheat;
				sPacket.iCheatID	= CHEATLOGID_CheckSumFunctionError;
				sPacket.LParam		= psPacketChecksum->sCheckSum[i].dwSize;
				sPacket.SParam		= psPacketChecksum->sCheckSum[i].dwAddress;
				sPacket.EParam		= psPacketChecksum->sCheckSum[i].dwCheckSum;
				sPacket.LxParam		= dwChecksumRet;
				SENDPACKETL( &sPacket );
				break;
			}
		}
	}
}

DWORD CAntiCheat::WindowFunctionHandler()
{
	char szTempName[256];
	for ( int i = 0; i < 100; i++ )
	{
		if ( lstrlenA( ANTICHEATHANDLER->psPacketWindowCheat->szaWindowName[i] ) && FindWindowPartialName( ANTICHEATHANDLER->psPacketWindowCheat->szaWindowName[i], szTempName ) )
		{
			if ( GAMEWINDOW && GAMEWINDOW->Exit() == FALSE && GAME && GAMECORE )
			{
				// Log Window Cheat
				PacketLogCheatEx sPacket;
				sPacket.iLength = sizeof( PacketLogCheatEx );
				sPacket.iHeader = PKTHDR_LogCheat;
				sPacket.iCheatID = CHEATLOGID_WindowHack;
				STRINGCOPY( sPacket.szParam, szTempName );
				SENDPACKETL( &sPacket );
			}
		}
	}

	//Stop Memory Scan (Cheat Engine)
	if ( FindWindowPartialName( "00007fffffffffff", szTempName ) || FindWindowPartialName( "00007FFFFFFFFFFF", szTempName ) || FindWindowPartialName( "0000000000000000", szTempName ) )
	{
		if ( GAMEWINDOW && GAMEWINDOW->Exit() == FALSE && GAME && GAMECORE )
		{
			// Log Window Cheat
			PacketLogCheatEx sPacket;
			sPacket.iLength = sizeof( PacketLogCheatEx );
			sPacket.iHeader = PKTHDR_LogCheat;
			sPacket.iCheatID = CHEATLOGID_WindowHack;
			STRINGCOPY( sPacket.szParam, szTempName );
			SENDPACKETL( &sPacket );
		}
	}

	//Deep Check
	HWND hWndTemp = FindWindowA( NULL, NULL );
	while ( hWndTemp != 0 )
	{

		RECT rect;
		if ( GetWindowRect( hWndTemp, &rect ) )
		{
			DWORD dwStyle = GetWindowLongA( hWndTemp, GWL_STYLE );
			int iWidth = rect.right - rect.left;
			int iHeight = rect.bottom - rect.top;

			//Memory View and Add Address Manually (Cheat Engine)
			if ( iWidth == 145 && iHeight == 19 && dwStyle == 0x56010000 )
			{
				if ( GAMEWINDOW && GAMEWINDOW->Exit() == FALSE && GAME && GAMECORE )
				{
					PacketLogCheatEx sPacket;
					sPacket.iLength = sizeof( PacketLogCheatEx );
					sPacket.iHeader = PKTHDR_LogCheat;
					sPacket.iCheatID = CHEATLOGID_WindowHack;
					STRINGCOPY( sPacket.szParam, "Add Address Manually" );
					SENDPACKETL( &sPacket );
				}
			}
		}

		hWndTemp = GetWindow( hWndTemp, GW_HWNDNEXT );
	}

	ExitThread( 0 );
	return 0;
}

DWORD CAntiCheat::GetModuleChecksum( const std::string strFile )
{
	//Get the address of NT Header
	IMAGE_NT_HEADERS *pNtHdr = ImageNtHeader( GetModuleHandleA( strFile.c_str() ) );

	DWORD dwBaseImage = (DWORD)GetModuleHandleA( strFile.c_str() );

	//After Nt headers comes the table of section, so get the addess of section table
	IMAGE_SECTION_HEADER *pSectionHdr = (IMAGE_SECTION_HEADER *)(pNtHdr + 1);

	struct ImageSectionInfo
	{
		char SectionName[IMAGE_SIZEOF_SHORT_NAME];//the macro is defined WinNT.h
		DWORD SectionAddress;
		DWORD SectionSize;
		ImageSectionInfo( const char* name )
		{
			STRINGCOPY( SectionName, name );
		}
	};

	ImageSectionInfo * pSectionInfo = NULL;

	//iterate through the list of all sections, and check the section name in the if conditon. etc
	for ( int i = 0; i < pNtHdr->FileHeader.NumberOfSections; i++ )
	{
		char *name = (char*)pSectionHdr->Name;
		if ( STRINGCOMPAREI( name, ".text" ) )
		{
			pSectionInfo = new ImageSectionInfo( ".text" );
			pSectionInfo->SectionAddress = pSectionHdr->VirtualAddress + dwBaseImage;

			//range of the data segment - something you're looking for
			pSectionInfo->SectionSize = pSectionHdr->Misc.VirtualSize;
			break;
		}
		pSectionHdr++;
	}

	DWORD dwRet = 0;
	if ( pSectionInfo )
	{
		DWORD dwOld = 0;
		VirtualProtect( (LPVOID)pSectionInfo->SectionAddress, pSectionInfo->SectionSize + 1, PAGE_EXECUTE_READWRITE, &dwOld );

		//Generate Checksum
		for ( UINT i = 0; i < pSectionInfo->SectionSize; i++ )
			dwRet += ((BYTE*)pSectionInfo->SectionAddress)[i] ^ 0x6A;

		VirtualProtect( (LPVOID)pSectionInfo->SectionAddress, pSectionInfo->SectionSize + 1, dwOld, NULL );

		DELET( pSectionInfo );
	}

	return dwRet;
}

BOOL CAntiCheat::CheckCooldown()
{
	if ( UNITDATA )
	{
		int iClass = UNITDATA->sCharacterData.iClass;

		for ( int i = 1; i <= 20; i++ )
		{
			if ( (SKILLSCHARACTER[i].sSkillInfo.iaSkillMastery[0] < CAntiCheat::iaSkillCoolDown[iClass - 1][i - 1]) && (SKILLSCHARACTER[i].sSkillInfo.iaSkillMastery[0] != 0) )
			{
				PacketLogCheat sPacket;
				sPacket.iLength = sizeof( PacketLogCheat );
				sPacket.iHeader = PKTHDR_LogCheat;
				sPacket.iCheatID = CHEATLOGID_SkillCooldown;
				sPacket.SParam = i;
				sPacket.EParam = CAntiCheat::iaSkillCoolDown[iClass - 1][i - 1];
				sPacket.LxParam = SKILLSCHARACTER[i].sSkillInfo.iaSkillMastery[0];
				SENDPACKETL( &sPacket );
				return TRUE;
			}
		}
	}

	return FALSE;
}

void CAntiCheat::DebugSelf( SDebugSelf * ps )
{
	DEBUG_EVENT de;
	ZeroMemory( &de, sizeof( DEBUG_EVENT ) );

	// Continue execution
//	ContinueDebugEvent( ps->dwProcessID, ps->dwThreadID, DBG_CONTINUE );

	// Wait for an event
//	WaitForDebugEvent( &de, INFINITE );

	AntiDebugger();
}

DWORD WINAPI CAntiCheat::ThreadAntiCheatHandler()
{
#if defined(_DEBUG) || defined(_ANTICHEAT_OFF_)
	return TRUE;
#endif

	if ( GAMEWINDOW && GAMEWINDOW->Exit() == FALSE && GAME && GAMECORE )
	{
		ANTICHEATHANDLER->ChecksumFunctionHandler();
		if ( ANTICHEATHANDLER->psPacketWindowCheat )
			CAntiDebuggerHandler::CreateHiddenThread( (LPTHREAD_START_ROUTINE)&CAntiCheat::WindowFunctionHandler, NULL, KB16 );

		if ( IsDebuggerPresent() )
		{
			// Send log hack checksum
			PacketLogCheat sPacket;
			sPacket.iLength = sizeof( PacketLogCheat );
			sPacket.iHeader = PKTHDR_LogCheat;
			sPacket.iCheatID = CHEATLOGID_DebuggerHack;
			SENDPACKETL( &sPacket );
		}

		if ( ANTICHEATHANDLER->IsHookAPI( (BYTE*)&GetTickCount ) )
		{
			// Send log hack checksum
			PacketLogCheat sPacket;
			sPacket.iLength = sizeof( PacketLogCheat );
			sPacket.iHeader = PKTHDR_LogCheat;
			sPacket.iCheatID = CHEATLOGID_GetTickCountHack;
			SENDPACKETL( &sPacket );
		}

		if ( ANTICHEATHANDLER->IsHookAPI( (BYTE*)&QueryPerformanceCounter ) )
		{
			// Send log hack checksum
			PacketLogCheat sPacket;
			sPacket.iLength = sizeof( PacketLogCheat );
			sPacket.iHeader = PKTHDR_LogCheat;
			sPacket.iCheatID = CHEATLOGID_QueryPerformanceHack;
			SENDPACKETL( &sPacket );
		}

/*
		if ( int iCount = ANTICHEATHANDLER->GetMultipleWindows( GetCurrentProcessId() ) > 1 )
		{
			PacketLogCheat sPacket;
			sPacket.iLength = sizeof( PacketLogCheat );
			sPacket.iHeader = PKTHDR_LogCheat;
			sPacket.iCheatID = CHEATLOGID_MultipleWindowHackProcess;
			sPacket.LParam = iCount;
			SENDPACKETL( &sPacket );
		}
*/
		//Cooldown
		if ( ANTICHEATHANDLER->CheckCooldown() )
		{

		}

		//Check Defense...
		if ( (*(DWORD*)0x007C0680) != 0x3E800000 )
		{
			// Send log hack
			PacketLogCheat sPacket;
			sPacket.iLength = sizeof( PacketLogCheat );
			sPacket.iHeader = PKTHDR_LogCheat;
			sPacket.iCheatID = CHEATLOGID_DefenseMultHack;
			sPacket.SParam = (*(int*)0x007C0680);
			SENDPACKETL( &sPacket );
		}

		//Check HP,STM,MP Formula
		if ( (*(DWORD*)0x007C0648) != 0x66666666 || (*(DWORD*)0x007C064C) != 0x3FE66666 )
		{
			// Send log hack
			PacketLogCheat sPacket;
			sPacket.iLength = sizeof( PacketLogCheat );
			sPacket.iHeader = PKTHDR_LogCheat;
			sPacket.iCheatID = CHEATLOGID_RegenFormulaHack;
			sPacket.SParam = (*(int*)0x007C0648);
			sPacket.EParam = (*(int*)0x007C064C);
			SENDPACKETL( &sPacket );
		}

		DWORD dwResChecksum = CAntiCheat::GetModuleChecksum( "game.dll" );
		if ( CAntiCheat::dwCheckSumDLL != dwResChecksum )
		{
			PacketLogCheat sPacket;
			sPacket.iLength = sizeof( PacketLogCheat );
			sPacket.iHeader = PKTHDR_LogCheat;
			sPacket.iCheatID = CHEATLOGID_ModuleSyncError;
			sPacket.LParam = CAntiCheat::dwCheckSumDLL;
			sPacket.SParam = dwResChecksum;
			SENDPACKETL( &sPacket );
		}

		dwResChecksum = CAntiCheat::GetModuleChecksum( "game.exe" );
		if ( CAntiCheat::dwCheckSumEXE != dwResChecksum )
		{
			PacketLogCheat sPacket;
			sPacket.iLength = sizeof( PacketLogCheat );
			sPacket.iHeader = PKTHDR_LogCheat;
			sPacket.iCheatID = CHEATLOGID_EXEModuleSyncError;
			sPacket.LParam = CAntiCheat::dwCheckSumEXE;
			sPacket.SParam = dwResChecksum;
			SENDPACKETL( &sPacket );
		}
	}
	return TRUE;
}

void __cdecl CAntiCheat::CheckThread()
{
#if defined(_DEBUG) || defined(_ANTICHEAT_OFF_)
	return;
#endif

	static DWORD dwThreadSuspendCheckTime = 0;
	static bool bThreadSuspendCheck = false;

	DWORD dwThreadID	= GetCurrentThreadId();
	BOOL bThreadFound	= FALSE;

	// Find threads exception
	for ( int i = 0; i < _countof( CAntiCheat::dwThreadsID ); i++ )
	{
		if ( CAntiCheat::dwThreadsID[i] == dwThreadID )
		{
			bThreadFound = TRUE;
			break;
		}
	}
	
	// Not thread in exception?
	if ( !bThreadFound )
	{
		// Send log hack module thread ( or created remote thread in game process )
		char szModuleName[128] = { 0 };
		PacketLogCheat sPacket;
		sPacket.iLength		= sizeof( PacketLogCheat );
		sPacket.iHeader		= PKTHDR_LogCheat;
		sPacket.iCheatID	= CHEATLOGID_FindThreadHack;
		sPacket.LParam		= dwThreadID;

		// Teleport Hack?
		if ( (*(int*)0x04B0CF14) >= 0 )
		{
			int iMap			= *( int* )( *( int* )( ( *( int* )( ( *( int* )0x0CF481C ) + 0x200 ) * 4 ) + 0x0CF4748 ) + 0x1FD4 );
			sPacket.iCheatID	= CHEATLOGID_TeleportFieldHack;
			sPacket.LParam		= iMap;
			sPacket.SParam		= (*(int*)0x04B0CF14);
		}

		SENDPACKETL( &sPacket );
	}
/*
	if ( ANTICHEATHANDLER )
	{

		if ( ((dwThreadSuspendCheckTime + 1000) < TICKCOUNT) && (bThreadSuspendCheck == false) )
		{
			if ( ANTICHEATHANDLER->hThread != INVALID_HANDLE_VALUE )
			{
				INT iError = SuspendThread( ANTICHEATHANDLER->hThread );
				ResumeThread( ANTICHEATHANDLER->hThread );
				if ( iError != 0 )
				{
					PacketLogCheat sPacket;
					sPacket.iLength = sizeof( PacketLogCheat );
					sPacket.iHeader = PKTHDR_LogCheat;
					sPacket.iCheatID = CHEATLOGID_SuspendThreadHack;
					sPacket.LParam = dwThreadID;

					SENDPACKETL( &sPacket );

					bThreadSuspendCheck = true;
				}
			}

			dwThreadSuspendCheckTime = TICKCOUNT;
		}
	}
	*/
}

void CAntiCheat::ReceivePacketHandler( PacketChecksumFunctionList * psPacket )
{
	if ( !psPacketChecksum )
		psPacketChecksum = new PacketChecksumFunctionList;

	ZeroMemory( psPacketChecksum, sizeof( PacketChecksumFunctionList ) );

	CopyMemory( psPacketChecksum, psPacket, sizeof( PacketChecksumFunctionList ) );
	
	bCheckStart = true;
#if !defined(_DEBUG)
	bCheckStart = true;

	//if ( hThread == INVALID_HANDLE_VALUE )
	//	hThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)ThreadAntiCheatHandler, NULL, NULL, NULL );
#endif
}

void CAntiCheat::HandlePacket( PacketWindowList * psPacket )
{
	if ( !psPacketWindowCheat )
		psPacketWindowCheat = new PacketWindowList;

	ZeroMemory( psPacketWindowCheat, sizeof( PacketWindowList ) );

	CopyMemory( psPacketWindowCheat, psPacket, sizeof( PacketWindowList ) );
}

void CAntiCheat::AntiDebugger()
{
	HMODULE h = LoadLibraryA( "ntdll.dll" );
	if ( h )
	{
		DWORD d = (DWORD)GetProcAddress( h, "DbgUiRemoteBreakin" );
		if ( d )
		{
			DWORD dold = 0;
			VirtualProtect( (void*)d, 8, PAGE_EXECUTE_READWRITE, &dold );
			(*(DWORD*)d) = 0x6A6A6A6A;
			VirtualProtect( (void*)d, 8, dold, NULL );
		}
	}
}

void CAntiCheat::UnloadLibrary()
{
	static BOOL bSuspended = FALSE;

	HMODULE hHandle = GetModuleHandle( "gbiehcef.dll" );

	if ( hHandle )
	{
		if ( bSuspended == FALSE )
		{
			FreeLibraryAndExitThread( hHandle, 0 );

			hHandle = GetModuleHandle( "wslbscr32.dll" );
			if ( hHandle )
				FreeLibraryAndExitThread( hHandle, 0 );

			bSuspended = TRUE;
		}
	}
}

void CAntiCheat::UpdateSkillCooldown()
{
	SkillBase * psFS	= ((SkillBase*)0x00963288);
	SkillBase * psMS	= ((SkillBase*)0x00961B18);
	SkillBase * psAS	= ((SkillBase*)0x00966168);
	SkillBase * psPS	= ((SkillBase*)0x009649F8);
	SkillBase * psATA	= ((SkillBase*)0x0096A7B8);
	SkillBase * psKS	= ((SkillBase*)0x00969048);
	SkillBase * psMG	= ((SkillBase*)0x0096D698);
	SkillBase * psPRS	= ((SkillBase*)0x0096BF28);
	SkillBase * psASS	= ((SkillBase*)0x009678D8);
	SkillBase * psSHA	= ((SkillBase*)0x0096EE08);


	//FS
	for ( int i = 0; i < 20; i++ )
		CAntiCheat::iaSkillCoolDown[CHARACTERCLASS_Fighter - 1][i]		= psFS[i].iaSkillMastery[0];
	//MS
	for ( int i = 0; i < 20; i++ )
		CAntiCheat::iaSkillCoolDown[CHARACTERCLASS_Mechanician - 1][i]	= psMS[i].iaSkillMastery[0];
	//AS
	for ( int i = 0; i < 20; i++ )
		CAntiCheat::iaSkillCoolDown[CHARACTERCLASS_Archer - 1][i]		= psAS[i].iaSkillMastery[0];
	//PS
	for ( int i = 0; i < 20; i++ )
		CAntiCheat::iaSkillCoolDown[CHARACTERCLASS_Pikeman - 1][i]		= psPS[i].iaSkillMastery[0];
	//ATA
	for ( int i = 0; i < 20; i++ )
		CAntiCheat::iaSkillCoolDown[CHARACTERCLASS_Atalanta - 1][i]		= psATA[i].iaSkillMastery[0];
	//KS
	for ( int i = 0; i < 20; i++ )
		CAntiCheat::iaSkillCoolDown[CHARACTERCLASS_Knight - 1][i]		= psKS[i].iaSkillMastery[0];
	//MG
	for ( int i = 0; i < 20; i++ )
		CAntiCheat::iaSkillCoolDown[CHARACTERCLASS_Magician - 1][i]		= psMG[i].iaSkillMastery[0];
	//PRS
	for ( int i = 0; i < 20; i++ )
		CAntiCheat::iaSkillCoolDown[CHARACTERCLASS_Priestess - 1][i]	= psPRS[i].iaSkillMastery[0];
	//ASS
	for ( int i = 0; i < 20; i++ )
		CAntiCheat::iaSkillCoolDown[CHARACTERCLASS_Assassin - 1][i]		= psASS[i].iaSkillMastery[0];
	//SHA
	for ( int i = 0; i < 20; i++ )
		CAntiCheat::iaSkillCoolDown[CHARACTERCLASS_Shaman - 1][i]		= psSHA[i].iaSkillMastery[0];
}

void CAntiCheat::AutoClickUpdate( HWND hWnd, bool bInside )
{
	if ( Game::GetGameMode() == GAMEMODE_InGame )
	{
		if ( GM_MODE == FALSE )
		{
			if ( GetForegroundWindow() == hWnd )
			{
				if ( bInside )
				{
					if ( hHookWindowHandle == NULL )
						hHookWindowHandle = SetWindowsHookExA( WH_MOUSE_LL, HookCallbackMouseClick, NULL, 0 );
				}
				else
				{
					if ( hHookWindowHandle )
						UnhookWindowsHookEx( hHookWindowHandle );

					hHookWindowHandle = NULL;
				}
			}
			else
			{
				if ( hHookWindowHandle )
					UnhookWindowsHookEx( hHookWindowHandle );

				hHookWindowHandle = NULL;
			}

			//Detected?
			if ( CAntiCheat::uNumberDetectedAutoClick > 1 )
			{
				if ( (CAntiCheat::dwUpdateDetectedAutoClick + 300000) < TICKCOUNT )
				{
					PacketLogCheat sPacket;
					sPacket.iLength		= sizeof( PacketLogCheat );
					sPacket.iHeader		= PKTHDR_LogCheat;
					sPacket.iCheatID	= CHEATLOGID_WarningAutoClick;
					sPacket.LParam		= MAP_ID;
					SENDPACKETL( &sPacket, TRUE );

					CAntiCheat::dwUpdateDetectedAutoClick	= TICKCOUNT;

					CAntiCheat::uNumberDetectedAutoClick	= 0;
					CAntiCheat::dwUpdateCheckAutoClick		= TICKCOUNT;
				}
			}

			if ( (CAntiCheat::dwUpdateCheckAutoClick + 1000) < TICKCOUNT )
			{
				CAntiCheat::uNumberDetectedAutoClick	= 0;
				CAntiCheat::dwUpdateCheckAutoClick		= TICKCOUNT;
			}
		}
		else
		{
			if ( hHookWindowHandle )
				UnhookWindowsHookEx( hHookWindowHandle );

			hHookWindowHandle = NULL;
		}
	}
}

BOOL CAntiCheat::IsHookAPI( BYTE * pbFunction )
{
	//JMP
	if ( pbFunction[0] == 0xE9 )
		return TRUE;

	//JMP DWORD PTR DS:[...]
	if ( pbFunction[0] == 0xFF && pbFunction[1] == 0x25 )
		return TRUE;

	return FALSE;
}

int CAntiCheat::GetMultipleWindows( DWORD dwProcessID )
{
	int iCount = 0;

	HWND hCurWnd = NULL;
	do
	{
		hCurWnd = FindWindowExA( NULL, hCurWnd, NULL, NULL );
		DWORD dwProcessId = 0;
		GetWindowThreadProcessId( hCurWnd, &dwProcessID );
		if ( dwProcessId == dwProcessID )
		{
			if ( IsWindowVisible( hCurWnd ) )
				iCount++;
		}
	}
	while ( hCurWnd != NULL );

	return iCount;
}

void GetAllWindowsFromProcessID( DWORD dwProcessID, std::vector <HWND> & vhWnds )
{
	// find all hWnds (vhWnds) associated with a process id (dwProcessID)
	HWND hCurWnd = nullptr;
	do
	{
		hCurWnd = FindWindowEx( nullptr, hCurWnd, nullptr, nullptr );
		DWORD checkProcessID = 0;
		GetWindowThreadProcessId( hCurWnd, &checkProcessID );
		if ( (checkProcessID == dwProcessID) && IsWindowVisible( hCurWnd ) )
		{
			vhWnds.push_back( hCurWnd );
		}
}
	while ( hCurWnd != nullptr );
}

bool bStartedDeepCheck = false;

void CAntiCheat::DeepCheck()
{
#if defined(_DEBUG) || defined(_ANTICHEAT_OFF_)
	ExitThread( 0 );
	return;
#endif

	bStartedDeepCheck = true;

	WRITEDBG( "CAntiCheatHandler::DeepCheck()" );

	//Loop through active Processes
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;

	DWORD dwCurrentProcessId = GetCurrentProcessId();

    while ( TRUE )
    {
        if ( (hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 )) != INVALID_HANDLE_VALUE )
        {
            pe32.dwSize = sizeof( pe32 );

            if ( Process32First( hProcessSnap, &pe32 ) )
            {
                do
                {
                    //Ignore Self
                    if ( pe32.th32ProcessID == dwCurrentProcessId )
                        continue;

                    //Ignore System Idle Process
                    if ( pe32.szExeFile[0] == '[' )
                        continue;

                    //Ignore System
                    if ( STRINGCOMPAREI( pe32.szExeFile, "System" ) )
                        continue;

                    bool bSafeWindow = true;
                    std::vector<HWND> vHwnd;
                    GetAllWindowsFromProcessID( pe32.th32ProcessID, vHwnd );
                    if ( vHwnd.size() == 0 )
                    {
                        vHwnd.clear();
                        continue;
                    }

                    vHwnd.clear();

                    //Ignore System Process
                    if ( STRINGCOMPAREI( pe32.szExeFile, "smss.exe" ) ||
                         STRINGCOMPAREI( pe32.szExeFile, "csrss.exe" ) ||
                         STRINGCOMPAREI( pe32.szExeFile, "userinit.exe" ) ||
                         STRINGCOMPAREI( pe32.szExeFile, "services.exe" ) ||
                         STRINGCOMPAREI( pe32.szExeFile, "svchost.exe" ) ||
                         STRINGCOMPAREI( pe32.szExeFile, "spoolsv.exe" ) ||
                         STRINGCOMPAREI( pe32.szExeFile, "wdfmgr.exe" ) ||
                         STRINGCOMPAREI( pe32.szExeFile, "alg.exe" ) ||
                         STRINGCOMPAREI( pe32.szExeFile, "lsm.exe" ) ||
                         STRINGCOMPAREI( pe32.szExeFile, "wininit.exe" ) ||
                         STRINGCOMPAREI( pe32.szExeFile, "lsass.exe" ) ||
                         STRINGCOMPAREI( pe32.szExeFile, "ctfmon.exe" ) ||
                         STRINGCOMPAREI( pe32.szExeFile, "nvsvc32.exe" ) ||
                         STRINGCOMPAREI( pe32.szExeFile, "nvvsvc.exe" ) ||
                         STRINGCOMPAREI( pe32.szExeFile, "SndVol.exe" ) ||
                         STRINGCOMPAREI( pe32.szExeFile, "GoogleDesktop.exe" ) ||
                         STRINGCOMPAREI( pe32.szExeFile, "dwm.exe" ) ||
                         STRINGCOMPAREI( pe32.szExeFile, "explorer.exe" ) ||
                         STRINGCOMPAREI( pe32.szExeFile, "WerFault.exe" ) ||
                         STRINGCOMPAREI( pe32.szExeFile, "chrome.exe" ) ||
                         STRINGCOMPAREI( pe32.szExeFile, "taskhost.exe" ) ||
                         STRINGCOMPAREI( pe32.szExeFile, "taskmgr.exe" ) ||
                         STRINGCOMPAREI( pe32.szExeFile, "rzsynapse.exe" ) ||
                         STRINGCOMPAREI( pe32.szExeFile, "nvcontainer.exe" ) ||
                         STRINGCOMPAREI( pe32.szExeFile, "audiodg.exe" ) ||
                         STRINGCOMPAREI( pe32.szExeFile, "osk.exe" ) ||
                         STRINGCOMPAREI( pe32.szExeFile, "consHost.exe" ) ||
                         STRINGCOMPAREI( pe32.szExeFile, "RAVBg64.exe" ) ||
                         STRINGCOMPAREI( pe32.szExeFile, "obs64.exe" ) ||
                         STRINGCOMPAREI( pe32.szExeFile, "OverwolfHelper.exe" ) ||
                         STRINGCOMPAREI( pe32.szExeFile, "Discord.exe" ) ||
                         STRINGCOMPAREI( pe32.szExeFile, "UninstallMonitor.exe" ) ||
                         STRINGCOMPAREI( pe32.szExeFile, "AsusTPCenter.exe" ) ||
                         STRINGCOMPAREI( pe32.szExeFile, "winlogon.exe" ) )
                        continue;

                    //WRITEERR( pe32.szExeFile );

                    //Deep Analyze Process
                    DeepAnalyzeProcess( pe32.th32ProcessID );

                    Sleep( 100 );
                }
                while ( Process32Next( hProcessSnap, &pe32 ) );
            }

            CloseHandle( hProcessSnap );
        }

        Sleep( 500 );
    }

	bStartedDeepCheck = false;

	ExitThread( 0 );
}

void __stdcall CAntiCheat::DeepCheckSelf()
{
#if defined(_DEBUG) || defined(_ANTICHEAT_OFF_)
	ExitThread( 0 );
	return;
#endif
	DeepAnalyzeProcess( GetCurrentProcessId() );
	ExitThread( 0 );
}

void CAntiCheat::DeepAnalyzeProcess( DWORD dwProcessId )
{
    UINT uDeepWheel = 0;

	WRITEDBG( "CAntiCheatHandler::DeepAnalyzeProcess( %d )", dwProcessId );

	//Get ntdll.dll Module Handle
	HMODULE hNTDLL = GetModuleHandleA( "ntdll.dll" );

	HANDLE hCurrentProcess = GetCurrentProcess();
	DWORD dwCurrentProcessId = GetCurrentProcessId();

	char szCurrentProcessName[260];
	GetProcessImageFileName( hCurrentProcess, szCurrentProcessName, _countof( szCurrentProcessName ) );

	char szCurrentWindowName[128];
	GetWindowTextA( (HWND)hCurrentProcess, szCurrentWindowName, _countof( szCurrentWindowName ) );

	//Load Nt Functions
	_NtQuerySystemInformation NtQuerySystemInformation = (_NtQuerySystemInformation)GetProcAddress( hNTDLL, "NtQuerySystemInformation" );
	_NtDuplicateObject NtDuplicateObject = (_NtDuplicateObject)GetProcAddress( hNTDLL, "NtDuplicateObject" );
	_NtQueryObject NtQueryObject = (_NtQueryObject)GetProcAddress( hNTDLL, "NtQueryObject" );

	//Get Process Handle
	HANDLE hProcessHandle;

	//Open the Process
	if ( (hProcessHandle = OpenProcess( PROCESS_DUP_HANDLE, FALSE, dwProcessId )) == NULL )
		return;

	//Memory Allocate System Handle Information
	ULONG uHandleInfoSize = 0x10000;
	PSYSTEM_HANDLE_INFORMATION phHandleInfo = (PSYSTEM_HANDLE_INFORMATION)malloc( uHandleInfoSize );

	//NT Status keeper
	NTSTATUS status;

	/* NtQuerySystemInformation won't give us the correct buffer size,
	   so we guess by doubling the buffer size. */
	while ( (status = NtQuerySystemInformation(
		SystemHandleInformation,
		phHandleInfo,
		uHandleInfoSize,
		NULL
	)) == STATUS_INFO_LENGTH_MISMATCH )
		phHandleInfo = (PSYSTEM_HANDLE_INFORMATION)realloc( phHandleInfo, uHandleInfoSize *= 2 );

	/* NtQuerySystemInformation stopped giving us STATUS_INFO_LENGTH_MISMATCH. */
	if ( !NT_SUCCESS( status ) )
	{
		free( phHandleInfo );
		CloseHandle( hProcessHandle );

		return;
	}

	for ( UINT i = 0; i < phHandleInfo->HandleCount; i++ )
	{
		SYSTEM_HANDLE shHandle = phHandleInfo->Handles[i];
		HANDLE hDupHandle = NULL;

        uDeepWheel++;

		//Object should be owned by the Process we are checking
		if ( shHandle.ProcessId != dwProcessId )
			continue;

		/* Duplicate the handle so we can query it. */
		if ( !NT_SUCCESS( NtDuplicateObject(
			hProcessHandle,
			(HANDLE)shHandle.Handle,
			hCurrentProcess,
			&hDupHandle,
			0,
			0,
			0
		) ) )
		{
			//Error
			continue;
		}

		/* Query the object type. */
		POBJECT_TYPE_INFORMATION pObjectTypeInfo = (POBJECT_TYPE_INFORMATION)malloc( 0x1000 );
		if ( !NT_SUCCESS( NtQueryObject(
			hDupHandle,
			ObjectTypeInformation,
			pObjectTypeInfo,
			0x1000,
			NULL
		) ) )
		{
			//Error
			free( pObjectTypeInfo );
			CloseHandle( hDupHandle );
			continue;
		}

		//Is it a Process Handle?
		if ( pObjectTypeInfo->Name.Buffer[0] == 'P' &&
			 pObjectTypeInfo->Name.Buffer[1] == 'r' &&
			 pObjectTypeInfo->Name.Buffer[2] == 'o' &&
			 pObjectTypeInfo->Name.Buffer[3] == 'c' &&
			 pObjectTypeInfo->Name.Buffer[4] == 'e' &&
			 pObjectTypeInfo->Name.Buffer[5] == 's' &&
			 pObjectTypeInfo->Name.Buffer[6] == 's' )
		{
			HANDLE hProcessHandleWithDupPermission = OpenProcess( PROCESS_DUP_HANDLE | PROCESS_QUERY_INFORMATION, FALSE, shHandle.ProcessId );

			//Dupe Process Handle of Target Process with Duplicate Handle Permission
			if ( hProcessHandleWithDupPermission != NULL )
			{
				HANDLE hFoundProcessHandle;

				//Duplicate Handle of Found Process with Query Permission
				if ( DuplicateHandle( hProcessHandleWithDupPermission, (HANDLE)shHandle.Handle, hCurrentProcess, &hFoundProcessHandle, PROCESS_QUERY_INFORMATION, FALSE, 0 ) )
				{
					DWORD dwFoundProcessId = GetProcessId( hFoundProcessHandle );

					//Is Found Process Id same as Current Process Id? Means the Game is opened in another process probably for manipulation
					if ( dwCurrentProcessId == dwFoundProcessId )
					{
						//Another process has opened this process... possible hack detected, get Process Information
						char szProcessName[256];
						GetProcessImageFileName( hProcessHandleWithDupPermission, szProcessName, _countof( szProcessName ) );

						//Another RealmPT instance checking this RealmPT? then ignore
						if ( STRINGCOMPAREI( szProcessName, szCurrentProcessName ) == FALSE )
						{
							//Send process details to Login Server
							if ( SOCKETL )
							{
								PacketLogCheatEx sPacket;
								sPacket.iLength = sizeof( PacketLogCheatEx );
								sPacket.iHeader = PKTHDR_LogCheat;
								sPacket.iCheatID = CHEATLOGID_ProcessHook;
								STRINGCOPY( sPacket.szParam, szProcessName );
								SENDPACKETL( &sPacket );
							}

							//Close Source Handle to disable possible hack
							HANDLE hTerminateHandle;
							if ( DuplicateHandle( hProcessHandleWithDupPermission, (HANDLE)shHandle.Handle, hCurrentProcess, &hTerminateHandle, 0, FALSE, DUPLICATE_CLOSE_SOURCE | DUPLICATE_SAME_ACCESS ) )
							{
								//And close it
								CloseHandle( hTerminateHandle );
							}
						}
						else if ( dwProcessId == GetCurrentProcessId() )
						{
							//Close Source Handle to disable possible hack
							HANDLE hTerminateHandle;
							if ( DuplicateHandle( hProcessHandleWithDupPermission, (HANDLE)shHandle.Handle, hCurrentProcess, &hTerminateHandle, 0, FALSE, DUPLICATE_CLOSE_SOURCE | DUPLICATE_SAME_ACCESS ) )
							{
								//And close it
								CloseHandle( hTerminateHandle );
							}
						}
					}

					CloseHandle( hFoundProcessHandle );
				}
			}

			CloseHandle( hProcessHandleWithDupPermission );
		}

		free( pObjectTypeInfo );
		CloseHandle( hDupHandle );

        std::this_thread::yield();

        if ( (uDeepWheel % 20000) == 0 )
            Sleep( 32 );
	}

	free( phHandleInfo );
	CloseHandle( hProcessHandle );
}


void CAntiCheat::Update( float fTime )
{
	if ( bCheckStart )
	{
		fTimeUpdate += fTime;
		if ( fTimeUpdate >= 10000.0f )
		{
            if ( bStartedDeepCheck == false )
            {
                CAntiDebuggerHandler::CreateHiddenThread( (LPTHREAD_START_ROUTINE)&CAntiCheat::DeepCheck, NULL, KB16 );
            }
			//CreateThread( NULL, KB16, (LPTHREAD_START_ROUTINE)&CAntiCheat::DeepCheck, NULL, STACK_SIZE_PARAM_IS_A_RESERVATION, NULL );
			CAntiCheat::ThreadAntiCheatHandler();
			fTimeUpdate = 0.0f;
		}
	}

	fTimeUpdateSelf += fTime;
	if ( fTimeUpdateSelf >= 1500.0f )
	{
		CAntiDebuggerHandler::Execute();

		//Deep Analyze Process
		CAntiDebuggerHandler::CreateHiddenThread( (LPTHREAD_START_ROUTINE)&CAntiCheat::DeepCheckSelf, NULL, KB16 );
		//CreateThread( NULL, KB16, (LPTHREAD_START_ROUTINE)&CAntiCheat::DeepCheckSelf, NULL, STACK_SIZE_PARAM_IS_A_RESERVATION, NULL );
		fTimeUpdateSelf = 0.0f;
	}
}

void CAntiCheat::DeepThreadAnalyze( DWORD dwProcessId )
{
	if ( HANDLE hThreadSnap = CreateToolhelp32Snapshot( TH32CS_SNAPTHREAD, 0 ); hThreadSnap != INVALID_HANDLE_VALUE )
	{
		//Get First Thread Data
		THREADENTRY32 te32;
		te32.dwSize = sizeof( THREADENTRY32 );
		if ( Thread32First( hThreadSnap, &te32 ) )
		{
			//Check for Safe Threads
			do
			{
				//Process match?
				if ( te32.th32OwnerProcessID == dwProcessId )
				{

				}
			}
			while ( Thread32Next( hThreadSnap, &te32 ) );
		}

		//Free Thread Snapshot Handle
		CloseHandle( hThreadSnap );
	}
}

void CAntiCheat::UpdateHardwareID()
{
    struct HardwareIDData
    {
        char                    szObfuscation1[64];
        char                    szHardwareID[33];
        char                    szObfuscation2[147];
    };

    HardwareIDData sData;

    char szTempPath[MAX_PATH] = {0};
    GetTempPathA( MAX_PATH, szTempPath );

    std::string strFilePath = szTempPath + std::string( AY_OBFUSCATE( "\\10_41198587.rep" ) );

    CWMIReader * pcWMIReader = new CWMIReader();

    Game::SetHardwareID( pcWMIReader->GetHardwareID().c_str() );

    delete pcWMIReader;

    if ( FileExist( strFilePath ) == FALSE )
    {
        for ( int i = 0; i < _countof( sData.szObfuscation1 ); i++ )
            sData.szObfuscation1[i] = RandomI( 30, 200 );

        for ( int i = 0; i < _countof( sData.szObfuscation2 ); i++ )
            sData.szObfuscation2[i] = RandomI( 30, 200 );

        STRINGCOPY( sData.szHardwareID, Game::GetHardwareID() );

        for ( int i = 0; i < _countof( sData.szHardwareID ); i++ )
            sData.szHardwareID[i] ^= 0x37;

        FILE * pFile = nullptr;
        fopen_s( &pFile, strFilePath.c_str(), AY_OBFUSCATE( "wb" ) );
        if ( pFile )
        {
            fwrite( &sData, sizeof( HardwareIDData ), 1, pFile );
            fclose( pFile );
        }
    }
    else
    {
        FILE * pFile = nullptr;
        fopen_s( &pFile, strFilePath.c_str(), AY_OBFUSCATE( "rb" ) );
        if ( pFile )
        {
            fread( &sData, sizeof( HardwareIDData ), 1, pFile );
            fclose( pFile );

            for ( int i = 0; i < _countof( sData.szHardwareID ); i++ )
                sData.szHardwareID[i] ^= 0x37;

            Game::SetHardwareID( sData.szHardwareID );
        }
    }
}
