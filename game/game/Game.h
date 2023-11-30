#pragma once

#include "DX.h"

enum EGameMode
{
	GAMEMODE_None		= 0,
	GAMEMODE_Login		= 1,
	GAMEMODE_InGame		= 2
};


struct EXEDataLoader
{
	Stage * pcStage;
	UnitData * pcUnitData;
	DropItemView * pcItemDrop;

	char			szName[64];
	char			szNameLoader2[64];

	int				iResult;

	BOOL			bCancel;
};

class Game
{
public:

	// Constructor
								Game();
	virtual						~Game();

	// Initialization
	BOOL						Initialize();
	void						Shutdown();
	void						Run();

	// Message Handler
	LRESULT						CALLBACK MessageHandler( HWND, UINT, WPARAM, LPARAM );

	// Mouse iItemSlotFlag
	static void					GetMousePos( D3DXVECTOR2 * pVec );
	static BOOL					IsPoint2D( int iX, int iY, int iW, int iH );
	static void					SetMousePos( int iX, int iY );

	static struct EXEModelData * ReadInx( char * pszFile );

	static void					RenderInit();
	static void					RenderClipStateSet(DWORD dw);
	static void					RenderClip();
	static void					RenderGeomVertex2D(DWORD dwColor);
	static void					RenderD3D();

	static void					AnimObjectTree( void * pObject, int iFrame, int iAX, int iAY, int iAZ );

	static EGameMode			GetGameMode() { return *(EGameMode*)(0x00A1766C); };

	static HINTERNET WINAPI		InternetOpenUrlClan( HINTERNET hInternet, LPCTSTR lpszUrl, LPCTSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwFlags, DWORD_PTR dwContext );

	static void					OnSetRGBATimer();

	static void					SetMouseBlocked(BOOL b) { WRITEDWORD( 0x00CD93DC, b ); }
	static BOOL					GetMouseBlocked() { READDWORD( 0x00CD93DC ); }

	static void WINAPI			LoaderThread();

	static void					CreateLoaderThread( bool bFromPackets = false );
	static void					LoadDataFromLoader( EXEDataLoader * psLoader );

	static void					AddCharacterLoader( UnitData * pcUnitData, const std::string strBody, const std::string strHead );

	static void					HandlePacket( UnitData * pcUnitData, PacketUnitInfo * psPacket );
	static void					HandlePacket( UnitData * pcUnitData, PacketUnitInfoQuick * psPacket );

	static void					HandlePacket( PacketUnitInfo * psPacket );
	static void					HandlePacket( PacketUnitInfoQuick * psPacket );

	static void					ProcessHandlePacket( UnitData * pcUnitData, PacketUnitInfo * psPacket );
	static void					ProcessHandlePacket( UnitData * pcUnitData, PacketUnitInfoQuick * psPacket );

	static void					FlushLoaders();

	static void					DeleteClanTag();

	static void					SetHardwareID( const char * pszHardwareID );
	static char					* GetHardwareID() { return szHardwareID; }

	static UINT					GetGameChecksum() { return uGameChecksum; }
	static void					SetGameChecksum( UINT uChecksum ) { uGameChecksum = uChecksum; }

	static UINT					GetDLLChecksum() { return uDLLChecksum; }
	static void					SetDLLChecksum( UINT uChecksum ) { uDLLChecksum = uChecksum; }

private:
	void						InitializeWindow( int iWidth, int iHeight );
	void						ShutdownWindow();
	void						CreateInx( char * pszFile );

	// Declarations
	LPCSTR						lpAppName;
	HINSTANCE					hInst;
	HWND						hWindow;

	static char					szHardwareID[40];

	static UINT					uGameChecksum;
	static UINT					uDLLChecksum;

	static HANDLE				hEventLoader;
};

