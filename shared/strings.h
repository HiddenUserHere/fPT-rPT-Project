#pragma once

#define STRINGSAFE( str )						str[_countof(str)-1] = 0

#define STRLEN( str )							lstrlenA( str )
#define STRLENW( str )							lstrlenW( str )

#define STRINGEMPTY( dest )						ZeroMemory( dest, sizeof( dest ) )

#define STRINGCOPY( dest, src )					StringCchCopyA( dest, _countof( dest ), src )
#define STRINGCOPYW( dest, src )				StringCchCopyW( dest, _countof( dest ), src )
#define STRINGCOPYLEN( dest, len, src )			StringCchCopyA( dest, len, src )
#define STRINGCOPYLENW( dest, len, src )		StringCchCopyW( dest, len, src )

#define STRINGFORMAT( dest, fmt, ... )			StringCbPrintfA( dest, _countof( dest ), fmt, ##__VA_ARGS__ )
#define STRINGFORMATW( dest, fmt, ... )			StringCbPrintfW( dest, _countof( dest ), fmt, ##__VA_ARGS__ )
#define STRINGFORMATLEN( dest, len, fmt, ... )	StringCbPrintfA( dest, len, fmt, ##__VA_ARGS__ )
#define STRINGFORMATLENW( dest, len, fmt, ... )	StringCbPrintfW( dest, len, fmt, ##__VA_ARGS__ )

#define STRINGCONCAT( dest, src )				StringCchCatA( dest, _countof( dest ), src )
#define STRINGCONCATW( dest, src )				StringCchCatW( dest, _countof( dest ), src )

#define STRINGCOMPARE( str1, str2 )				(lstrcmpA( str1, str2 ) == 0)
#define STRINGCOMPAREW( str1, str2 )			(lstrcmpW( str1, str2 ) == 0)
#define STRINGCOMPAREI( str1, str2 )			(lstrcmpiA( str1, str2 ) == 0)
#define STRINGCOMPAREIW( str1, str2 )			(lstrcmpiW( str1, str2 ) == 0)

int lstrnlenA( const char * pszString, int iMaxLen );
bool lstrncmpiA( const char * pszString1, const char * pszString2, int iNum );

char & upper( char & c );
char * upper( char * s );
char & lower( char & c );
char * lower( char * s );

bool strcontains( const char * pszString, const char * pszContains );
bool strcontainsi( const char * pszString, const char * pszContains );

int low( int a, int b );
int high( int a, int b );

void FormatNumber( __int64 iNumber, char * pszBufferOut, int iBufferOutLen );

const char * FormatNumber( __int64 iNumber );

const char * FormatTimeLong( __int64 iTotalSeconds );

const char * FormatString( const char * pszFormat, ... );

std::vector<std::string> split( const std::string & s, char delim );

std::string & ltrim( std::string & s );
std::string & rtrim( std::string & s );
std::string & trim( std::string & s );

void tolower( const char * pszIn, char * pszOut );
std::string tolower( const std::string & s );

int GetParameterString( const char * Command, int Arg, char buf[256] );

std::string GetFileExtension( const std::string & strFilePath );
std::string ReplaceFileExtension( const std::string & strFilePath, const std::string & strNewFileExtension );

char * DecodeUTF8( const char * pszText, size_t iLength, size_t iOffset, int & iOutLength );
std::string DecodeUTF8( const std::string & strText, size_t iOffset );

std::string ConvertUnicode( const std::wstring & strUnicodeText );