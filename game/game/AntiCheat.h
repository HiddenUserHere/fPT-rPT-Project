#pragma once

#include "CAntiDebuggerHandler.h"

struct SDebugSelf
{
	DWORD dwProcessID;
	DWORD dwThreadID;
};

class CAntiCheat
{
protected:
	struct PacketChecksumFunctionList			* psPacketChecksum		= NULL;
	
	DWORD										dwChecksumRet = 0;
private:
	static int									iaSkillCoolDown[10][20];

	static DWORD								dwUpdateCheckAutoClick;
	static DWORD								dwUpdateDetectedAutoClick;

	BOOL										ChecksumCheckMismatch( struct ChecksumFunction * pChecksum );
	BOOL										GetModuleName( DWORD dwThreadID, char * pszBuf, int iSizeBuf );
	static DWORD WINAPI							EnumWindowsProc( HWND hWnd, LPARAM lParam );
	static DWORD WINAPI							WindowFunctionHandler();

	static DWORD								GetModuleChecksum( const std::string strFile );

	//Handle Skill Cooldown
	BOOL										CheckCooldown();

	float										fTimeUpdate = 0.0f;
	float										fTimeUpdateSelf = 0.0f;

	static DWORD								dwCheckSumDLL;
	static DWORD								dwCheckSumEXE;


public:
	static UINT									uNumberDetectedAutoClick;

	HANDLE										hThread = INVALID_HANDLE_VALUE;

	struct PacketWindowList						* psPacketWindowCheat	= NULL;

	static DWORD								dwThreadsID[8];

	bool										bCheckStart = false;

	CAntiCheat();
	virtual ~CAntiCheat();

	static DWORD								GetChecksumDLL() { return dwCheckSumDLL; }

	static std::string							GetEncDecodeString( std::string str );

	static void									DebugSelf( SDebugSelf * ps );

	BOOL										AddNewThreadException( DWORD dwThreadID );

	void										ChecksumFunctionHandler();

	static DWORD WINAPI							ThreadAntiCheatHandler();

	static void __cdecl							CheckThread();

	void										ReceivePacketHandler( struct PacketChecksumFunctionList * psPacket );

	void										HandlePacket( PacketWindowList * psPacket );

	static void									AntiDebugger();

	static void									UnloadLibrary();

	static void									UpdateSkillCooldown();

	void										AutoClickUpdate( HWND hWnd, bool bInside );

	BOOL										IsHookAPI( BYTE * pbFunction );
	int											GetMultipleWindows( DWORD dwProcessID );

	static void WINAPI							DeepCheck();
	static void WINAPI							DeepCheckSelf();
	static void									DeepAnalyzeProcess( DWORD dwProcessId );

	void										Update( float fTime );

	void										DeepThreadAnalyze( DWORD dwProcessId );

    static void                                 UpdateHardwareID();

};

