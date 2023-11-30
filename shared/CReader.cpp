#include "stdafx.h"
#include "CReader.h"
#include "strings.h"

namespace INI
{
CReader::CReader( char * pszFileName )
{
	sFileName = pszFileName;
}

CReader::~CReader()
{
}

std::string CReader::ReadString( const char * pszSection, const char * pszKey )
{
	char szBuffer[512];
	ZeroMemory( szBuffer, 512 );

	// Get Current Directory
	char szDirectory[512];
	GetCurrentDirectoryA( 512, szDirectory );

	// Format
	char szFormat[512];
	STRINGFORMAT( szFormat, "%s\\%s", szDirectory, sFileName.c_str() );

	// Read
	GetPrivateProfileStringA( pszSection, pszKey, "", szBuffer, 512, szFormat );
	return ( std::string )szBuffer;
}

int CReader::ReadInt( const char * pszSection, const char * pszKey, int iDefault )
{
	// Get Current Directory
	char szDirectory[512];
	GetCurrentDirectoryA( 512, szDirectory );

	// Format
	char szFormat[512];
	STRINGFORMAT( szFormat, "%s\\%s", szDirectory, sFileName.c_str() );

	// Read
	return GetPrivateProfileIntA( pszSection, pszKey, iDefault, szFormat );
}

BOOL CReader::ReadOnOff( const char * pszSection, const char * pszKey )
{
	char szBuffer[512];
	ZeroMemory( szBuffer, 512 );

	// Get Current Directory
	char szDirectory[512];
	GetCurrentDirectoryA( 512, szDirectory );

	// Format
	char szFormat[512];
	STRINGFORMAT( szFormat, "%s\\%s", szDirectory, sFileName.c_str() );

	// Read
	GetPrivateProfileStringA( pszSection, pszKey, "", szBuffer, 512, szFormat );
	if( STRINGCOMPAREI( szBuffer, "on" ) )
		return TRUE;

	return FALSE;
}

BOOL CReader::ReadBool( const char * pszSection, const char * pszKey, BOOL bDefault )
{
	char szBuffer[512];
	ZeroMemory( szBuffer, 512 );

	// Get Current Directory
	char szDirectory[512];
	GetCurrentDirectoryA( 512, szDirectory );

	// Format
	char szFormat[512];
	STRINGFORMAT( szFormat, "%s\\%s", szDirectory, sFileName.c_str() );

	// Read
	GetPrivateProfileStringA( pszSection, pszKey, "", szBuffer, 512, szFormat );
	if( STRINGCOMPAREI( szBuffer, "True" ) )
		return TRUE;

	if ( szBuffer[0] == 0 )
		return bDefault;

	return FALSE;
}

void CReader::WriteString( const char * pszSection, const char * pszKey, std::string strText )
{
	char szBuffer[512];
	ZeroMemory( szBuffer, 512 );

	// Get Current Directory
	char szDirectory[512];
	GetCurrentDirectoryA( 512, szDirectory );

	// Format
	char szFormat[512];
	STRINGFORMAT( szFormat, "%s\\%s", szDirectory, sFileName.c_str() );

	// Read
	WritePrivateProfileStringA( pszSection, pszKey, strText.c_str(), szFormat );
}

void CReader::WriteInt( const char * pszSection, const char * pszKey, int iValue )
{
	char szBuffer[32];
	ZeroMemory( szBuffer, 32 );
	STRINGFORMAT( szBuffer, "%d", iValue );

	// Get Current Directory
	char szDirectory[512];
	GetCurrentDirectoryA( 512, szDirectory );

	// Format
	char szFormat[512];
	STRINGFORMAT( szFormat, "%s\\%s", szDirectory, sFileName.c_str() );

	// Read
	WritePrivateProfileStringA( pszSection, pszKey, szBuffer, szFormat );
}

void CReader::WriteOnOff( const char * pszSection, const char * pszKey, BOOL bValue )
{
	char szBuffer[32];
	ZeroMemory( szBuffer, 32 );
	STRINGFORMAT( szBuffer, "%s", bValue ? "On" : "Off" );

	// Get Current Directory
	char szDirectory[512];
	GetCurrentDirectoryA( 512, szDirectory );

	// Format
	char szFormat[512];
	STRINGFORMAT( szFormat, "%s\\%s", szDirectory, sFileName.c_str() );

	// Read
	WritePrivateProfileStringA( pszSection, pszKey, szBuffer, szFormat );
}


void CReader::WriteBool( const char * pszSection, const char * pszKey, BOOL bValue )
{
	char szBuffer[32];
	ZeroMemory( szBuffer, 32 );
	STRINGFORMAT( szBuffer, "%s", bValue ? "True" : "False" );

	// Get Current Directory
	char szDirectory[512];
	GetCurrentDirectoryA( 512, szDirectory );

	// Format
	char szFormat[512];
	STRINGFORMAT( szFormat, "%s\\%s", szDirectory, sFileName.c_str() );

	// Read
	WritePrivateProfileStringA( pszSection, pszKey, szBuffer, szFormat );
}
}