#include "stdafx.h"
#include "debug.h"

bool Debug::bPrepared = false;

void Debug::WriteDebugger( char * msg )
{
	OutputDebugString( msg );
}

void Debug::WriteFile( char * msg )
{
	SYSTEMTIME time;
	GetLocalTime( &time );

	char szMessage[1024];

	StringCbPrintfA( szMessage, _countof( szMessage ), "%02d:%02d:%02d:%03d - %s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, msg );

	FILE * f;
	if( fopen_s( &f, "DEBUG.log", "a" ) == 0 )
	{
		fputs( szMessage, f );
		fclose( f );
	}
}

void Debug::Prepare()
{
	if( !bPrepared )
	{
		bPrepared = true;

		pcMutex = new CMutex();

		Clear();
	}
}

void Debug::Clear()
{
	DeleteFileA( "THREAD.log" );
	DeleteFileA( "DEBUG.log" );
	DeleteFileA( "IN.dat" );
	DeleteFileA( "OUT.dat" );
	DeleteFileA( "ERR.dat" );
}

void Debug::ErrorBox( const char * msg, ... )
{
	va_list vl;

	char szMessage[1024];

	va_start( vl, msg );
	_vsnprintf_s( szMessage, _countof( szMessage ), _TRUNCATE, msg, vl );
	va_end( vl );

	MessageBoxA( NULL, szMessage, "Error", MB_OK | MB_ICONEXCLAMATION );
}

void Debug::WriteLine( char * file, int line, char * msg, ... )
{
	pcMutex->Lock();

	va_list vl;

	char szMessage[2048];

	va_start( vl, msg );
	_vsnprintf_s( szMessage, _countof( szMessage ), _TRUNCATE, msg, vl );
	va_end( vl );

	char szFormattedMessage[2048];
	StringCbPrintfA( szFormattedMessage, 2048, "[%04d] [%04d] %s(%d): %s", GetCurrentProcessId(), GetCurrentThreadId(), file, line, szMessage );

	WriteDebugger( szFormattedMessage );
	WriteFile( szFormattedMessage );

	pcMutex->Unlock();
}

void Debug::WriteDump( int file, void * dmp, int size )
{
	pcMutex->Lock();

	char szFileName[32];
	switch( file )
	{
	case 0:
		StringCchCopyA( szFileName, 32, "IN.dat" );
		break;
	case 1:
		StringCchCopyA( szFileName, 32, "OUT.dat" );
		break;
	case 2:
		StringCchCopyA( szFileName, 32, "ERR.dat" );
		break;
	}

	FILE * f;
	if( fopen_s( &f, szFileName, "ab+" ) == 0 )
	{
		fwrite( dmp, size, 1, f );

		fclose( f );
	}

	pcMutex->Unlock();
}

void WriteFast( char * fn, char * msg, ... )
{
	SYSTEMTIME sSystemTime;
	GetLocalTime( &sSystemTime );

	va_list vl;

	char szMessage[1024];

	va_start( vl, msg );
	_vsnprintf_s( szMessage, _countof( szMessage ), _TRUNCATE, msg, vl );
	va_end( vl );

	char szFormattedMessage[1024];
	StringCbPrintfA( szFormattedMessage, 1024, "@ %02d-%02d-%04d %02d:%02d:%02d : [%04d] [%04d] : %s\r\n", sSystemTime.wDay, sSystemTime.wMonth, sSystemTime.wYear, sSystemTime.wHour, sSystemTime.wMinute, sSystemTime.wSecond, GetCurrentProcessId(), GetCurrentThreadId(), szMessage );

	FILE * f;
	if( (fopen_s( &f, fn, "a" )) == 0 )
	{
		fputs( szFormattedMessage, f );
		fclose( f );
	}
}