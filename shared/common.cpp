#include "stdafx.h"
#include "common.h"

#include <cstddef>
#include <exception>
#include <functional>
#include <new>
#include <type_traits>
#include <utility>

short RandomS( short l, short h )
{
	if ( l == h )
		return l;

	if ( l > h )
	{
		short a = l;
		l = h;
		h = a;
	}

	std::random_device rd;
	std::uniform_int_distribution<short> cDist( l, h );
	return cDist( rd );
}

int RandomI( int l, int h )
{
	if ( l == h )
		return l;

	if ( l > h )
	{
		int a = l;
		l = h;
		h = a;
	}

	std::random_device rd;
	std::uniform_int_distribution<int> cDist( l, h );
	return cDist( rd );
}

UINT RandomU( UINT l, UINT h )
{
	if ( l == h )
		return l;

	if ( l > h )
	{
		UINT a = l;
		l = h;
		h = a;
	}

	std::random_device rd;
	std::uniform_int_distribution<UINT> cDist( l, h );
	return cDist( rd );
}

float RandomF( float l, float h )
{
	if ( l == h )
		return l;

	if ( l > h )
	{
		float a = l;
		l = h;
		h = a;
	}

	std::random_device rd;
	std::uniform_real_distribution<float> cDist( l, h );
	return cDist( rd );
}

double RandomD( double l, double h )
{
	if ( l == h )
		return l;

	if ( l > h )
	{
		double a = l;
		l = h;
		h = a;
	}

	std::random_device rd;
	std::uniform_real_distribution<double> cDist( l, h );
	return cDist( rd );
}

BOOL IsNull( const char * pszString )
{
	if( STRLEN( pszString ) <= 0 )
		return TRUE;

	if( STRINGCOMPARE( pszString, "NULL" ) )
		return TRUE;

	return FALSE;
}

BOOL IsNull( int iValue )
{
	if( !iValue )
		return TRUE;

	return FALSE;
}

void ChangeFileExtension( const char * pszFile, const char * pszExt, char * pszBuffer )
{
	int cnt, len;

	STRINGCOPYLEN( pszBuffer, STRLEN( pszFile ), pszFile );
	len = STRLEN( pszBuffer );

	for( cnt = len - 1; cnt >= 0; cnt-- )
	{
		if( pszBuffer[cnt] == '.' )
		{
			STRINGCOPYLEN( &pszBuffer[cnt + 1], STRLEN( pszExt ), pszExt );
			return;
		}
	}

	pszBuffer[len] = '.';
	STRINGCOPYLEN( pszBuffer + len + 1, STRLEN( pszExt ), pszExt );
}

void GetDateTime( char * pszDateTime )
{
	char szDate[9] = { 0 };
	char szTime[9] = { 0 };
	_strdate_s( szDate, 9 );
	_strtime_s( szTime, 9 );

	STRINGFORMATLEN( pszDateTime, 20, "%s %s", szDate, szTime );
}

std::string GetDayOfWeek( int iDay )
{
	switch( iDay )
	{
		case 0:
			return "Sunday";
		case 1:
			return "Monday";
		case 2:
			return "Tuesday";
		case 3:
			return "Wednesday";
		case 4:
			return "Thursday";
		case 5:
			return "Friday";
		case 6:
			return "Saturday";
	}

	return "";
}

BOOL FileExist( const std::string& name )
{
	FILE * fp = NULL;
	fopen_s( &fp, name.c_str(), "rb" );
	if( fp )
	{
		fclose( fp );
		return TRUE;
	}
	return FALSE;
}

void SetClipboardText( char * szText )
{
	HGLOBAL x;
	char *y;
	x = GlobalAlloc( GMEM_DDESHARE | GMEM_MOVEABLE, 0x64 );
	y = ( char* )GlobalLock( x );
	strcpy_s( y, 0x64, szText );
	GlobalUnlock( x );
	OpenClipboard( NULL );
	EmptyClipboard();
	SetClipboardData( CF_TEXT, x );
	CloseClipboard();
}

std::string GetClipboardText()
{
#if defined(_GAME)
	// Try opening the clipboard
	if( !OpenClipboard( nullptr ) )
	{
		return "";
	}

	// Get handle of clipboard object for ANSI text
	HANDLE hData = GetClipboardData( CF_UNICODETEXT );
	if( hData == nullptr )
	{
		return "";
	}

	// Lock the handle to get the actual text pointer
	wchar_t * pszText = static_cast<wchar_t*>( GlobalLock( hData ) );
	if( pszText == nullptr )
	{
		return "";
	}

	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	// Save text in a string class instance
	std::string text( converterX.to_bytes( pszText ) );

	// Release the lock
	GlobalUnlock( hData );

	// Release the clipboard
	CloseClipboard();

	return text;

#endif

	return "";
}

int GetDaysCountMonth( int iMonth, int iYear )
{
	if ( (iMonth == 4) || (iMonth == 6) || (iMonth == 9) || (iMonth == 11) )
		return 30;
	else if ( iMonth == 2 )
	{
		if ( ((iYear % 400) == 0) || (((iYear % 4) == 0) && ((iYear % 100) != 0)) )
			return 29;
		else
			return 28;
	}

	return 31;
}

UINT64 SystemTimeToQWORD( SYSTEMTIME * ps )
{
    if ( ps == NULL )
        return 0;

    const UINT64 NSECS_TIME = 10000000;

    FILETIME ft = {0};
    SYSTEMTIME s;
    CopyMemory( &s, ps, sizeof( SYSTEMTIME ) );
    s.wDayOfWeek = 0;
    s.wMilliseconds = 0;

    SystemTimeToFileTime( &s, &ft );

    ULARGE_INTEGER u;
    u.LowPart = ft.dwLowDateTime;
    u.HighPart = ft.dwHighDateTime;

    return u.QuadPart / NSECS_TIME;
}


bool GetFileLastModifiedSystemTime( const std::string strFile, SYSTEMTIME & sTime )
{
	bool bRet = false;

	if ( HANDLE hFile = CreateFileA( strFile.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL ); hFile != INVALID_HANDLE_VALUE )
	{
		FILETIME sWrite;
		SYSTEMTIME sUTC;

		//Retrieve the file times for the file.
		if ( GetFileTime( hFile, NULL, NULL, &sWrite ) )
		{
			//Convert the last-write time to local time.
			FileTimeToSystemTime( &sWrite, &sUTC );
			SystemTimeToTzSpecificLocalTime( NULL, &sUTC, &sTime );

			bRet = true;
		}

		CloseHandle( hFile );
	}

	return bRet;
}


DWORD GetFileSize( const char * pszFile )
{
	DWORD dwSize = 0;
	FILE * fp = NULL;
	fopen_s( &fp, pszFile, "rb" );
	if( fp )
	{
		fseek( fp, 0, SEEK_END );
		dwSize = ftell( fp );
		fclose( fp );
	}
	return dwSize;
}

bool CEncryptXOR::EncDecrypt( BYTE * pbaData, int iLength, const char * pszHash )
{
#if defined(_GAME)
	char * pszHashXOR = pszHash ? (char*)pszHash : AY_OBFUSCATE( ")4<:^;(|3<5:2#}|]0^0-,7$}8.!>,0%[:$,.>.{}}?>5)>|)[03&7(8*4-#[+}-;59?|63}($*?%|51:^+;1;)$+15-:)@,;+#,}**@^|1!:7@<2:.4.5(87;}674]>" );

	int iStrLength = STRLEN( pszHashXOR );

	for ( int i = 0; i < iLength; i++ )
	{
		pbaData[i] ^= (pszHashXOR[i % iStrLength]);
	}
#endif
	return true;
}


std::wstring StringToWideString( const std::string & str )
{
	static wchar_t szBuffer[10240];
	ZeroMemory( szBuffer, sizeof( wchar_t ) * _countof( szBuffer ) );

	MultiByteToWideChar( CP_ACP, 0, str.c_str(), str.length(), szBuffer, _countof( szBuffer ) );

	return szBuffer;
}

std::string WideStringToString( const std::wstring & wstr )
{
	static char szBuffer[10240];
	ZeroMemory( szBuffer, sizeof( char ) * _countof( szBuffer ) );

	WideCharToMultiByte( CP_ACP, 0, wstr.c_str(), wstr.length(), szBuffer, _countof( szBuffer ), NULL, NULL );

	return szBuffer;
}