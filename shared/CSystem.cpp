#include "stdafx.h"
#include "CSystem.h"

#include <VersionHelpers.h>

CSystem::CSystem()
{
	GetSystemInfo( (LPSYSTEM_INFO)&sys_info );

	lang_id = GetUserDefaultLangID();
}

ESystemOS CSystem::GetSystemOS()
{
	bool bWindowsServer = IsWindowsServer();

	if ( !IsWindows10OrGreater() )
	{
		if ( !IsWindows8Point1OrGreater() )
		{
			if ( !IsWindows8OrGreater() )
			{
				if ( !IsWindows7OrGreater() )
				{
					if ( IsWindowsVistaOrGreater() )
						return bWindowsServer ? SYSTEMOS_Windows2008 : SYSTEMOS_WindowsVista;
				}
				else
					return bWindowsServer ? SYSTEMOS_Windows2008R2 : SYSTEMOS_Windows7;
			}
			else
				return bWindowsServer ? SYSTEMOS_Windows2012 : SYSTEMOS_Windows8;
		}
		else
			return bWindowsServer ? SYSTEMOS_Windows2012R2 : SYSTEMOS_Windows8_1;
	}
	else
		return bWindowsServer ? SYSTEMOS_Windows2016 : SYSTEMOS_Windows10;

	return SYSTEMOS_Unknown;
}

ESystemLanguage CSystem::GetSystemLanguage()
{
	switch( lang_id )
	{
	case 0x0412:
		return SYSTEMLANGUAGE_Korean;
	case 0x0C04:
	case 0x1404:
	case 0x1004:
	case 0x0004:
	case 0x7C04:
		return SYSTEMLANGUAGE_ChineseSimplified;
	case 0x0411:
		return SYSTEMLANGUAGE_Japanese;
	case 0x041E:
		return SYSTEMLANGUAGE_Thai;
	}

	return SYSTEMLANGUAGE_English;
}

BOOL CSystem::IsMultiCore()
{
	return (sys_info.dwNumberOfProcessors > 1);
}

int CSystem::GetNumberOfCores()
{
	return sys_info.dwNumberOfProcessors;
}

UINT CSystem::GetRAMAmount()
{
	MEMORYSTATUSEX sMemoryStatus;
	sMemoryStatus.dwLength = sizeof( MEMORYSTATUSEX );
	if ( GlobalMemoryStatusEx( &sMemoryStatus ) )
		return (UINT)(sMemoryStatus.ullTotalPhys / 1024000);

	return 0;
}

int CSystem::ComputeFrameDelayTime()
{
	int iNumberOfCores = GetNumberOfCores();

	if( iNumberOfCores > 2 )
		return 50;

	if( iNumberOfCores == 2 )
		return 100;

	return 250;
}

BOOL CSystem::IsVistaOrLater()
{
	return IsWindowsVistaOrGreater() ? TRUE : FALSE;
}

BOOL CSystem::IsElevated()
{
	if( IsVistaOrLater() == FALSE )
		return TRUE;

	HANDLE hToken;

	if( OpenProcessToken( GetCurrentProcess(), TOKEN_QUERY, &hToken ) == FALSE )
		return TRUE;

	TOKEN_ELEVATION te = { 0 };
	DWORD dwReturnLength = 0;

	if( GetTokenInformation( hToken, TokenElevation, &te, sizeof( te ), &dwReturnLength ) == FALSE )
	{
		CloseHandle( hToken );

		return TRUE;
	}

	BOOL bElevated = te.TokenIsElevated ? TRUE : FALSE;

	CloseHandle( hToken );

	return bElevated;
}

BOOL CSystem::OpenURL( char * pszURL )
{
	if( pszURL != NULL )
	{
		if( pszURL[0] == 'h' && 
			pszURL[1] == 't' && 
			pszURL[2] == 't' && 
			pszURL[3] == 'p' )
		{
			UINT iSE = (UINT)ShellExecuteA( NULL, "open", pszURL, NULL, NULL, SW_SHOWNORMAL );

			if( iSE > 32 )
				return TRUE;
		}
	}

	return FALSE;
}

BOOL CSystem::OpenURLEx( char * pszFormatURL, ... )
{
	va_list vl;

	char szURL[MAX_PATH];

	va_start( vl, pszFormatURL );
	_vsnprintf_s( szURL, _countof( szURL ), _TRUNCATE, pszFormatURL, vl );
	va_end( vl );

	return OpenURL( szURL );
}

std::vector<std::string> CSystem::ListFilesInDirectory( const std::string & strFolderPath, const std::string & strFileExtension, bool bIncludeSubDirectories )
{
	std::vector<std::string> vRet;

	for ( auto & p : std::filesystem::recursive_directory_iterator( strFolderPath ) )
	{
		if ( GetFileExtension( p.path().string() ) == strFileExtension )
		{
			if ( bIncludeSubDirectories || (p.path().parent_path().string() == std::filesystem::path( strFolderPath ).parent_path().string()) )
				vRet.push_back( p.path().string() );
		}
	}

	return vRet;
}
