#include "stdafx.h"
#include "strings.h"

int lstrnlenA( const char * pszString, int iMaxLen )
{
	int iRet = 0;

	for( int i = 0; i < iMaxLen; i++ )
	{
		if( pszString[i] != 0 )
			iRet++;
		else
			break;
	}

	return iRet;
}

bool lstrncmpiA( const char * pszString1, const char * pszString2, int iNum )
{
	for( int i = 0; i < iNum; i++ )
	{
		char c1 = pszString1[i], c2 = pszString2[i];

		if( (c1 >= 'a') && (c1 <= 'z') )
			c1 -= 32;
		if( (c2 >= 'a') && (c2 <= 'z') )
			c2 -= 32;

		if( c1 != c2 )
			return false;
	}

	return true;
}

char & upper( char & c )
{
	c = toupper( c );
	return c;
}

char * upper( char * s )
{
	for( int i = 0; s[i] != 0; i++ )
		upper( s[i] );

	return s;
}

char & lower( char & c )
{
	c = tolower( c );
	return c;
}

char * lower( char * s )
{
	for( int i = 0; s[i] != 0; i++ )
		lower( s[i] );

	return s;
}

bool strcontains( const char * pszString, const char * pszContains )
{
	bool comparing = false;
	int r = 0;
	for( int i = 0; ; i++ )
	{
		if( pszString[i] == 0 )
		{
			if( pszContains[r] == 0 )
				return true;

			return false;
		}

		if( pszContains[r] == 0 )
		{
			if( comparing )
				return true;

			return false;
		}

		if( pszString[i] == pszContains[r] )
		{
			comparing = true;
			r++;
		}
		else
		{
			comparing = false;
			r = 0;
		}
	}

	return false;
}

bool strcontainsi( const char * pszString, const char * pszContains )
{
	bool comparing = false;
	int r = 0;
	for( int i = 0; ; i++ )
	{
		if( pszString[i] == 0 )
		{
			if( pszContains[r] == 0 )
				return true;

			return false;
		}

		if( pszContains[r] == 0 )
		{
			if( comparing )
				return true;

			return false;
		}

		char a = tolower( pszString[i] );
		char b = tolower( pszContains[r] );

		if( a == b )
		{
			comparing = true;
			r++;
		}
		else
		{
			comparing = false;
			r = 0;
		}
	}

	return false;
}

int low( int a, int b )
{
	if( a < b )
		return a;

	return b;
}

int high( int a, int b )
{
	if( a > b )
		return a;

	return b;
}

void FormatNumber( __int64 iNumber, char * pszBufferOut, int iBufferOutLen )
{
	if( iBufferOutLen <= 0 )
		return;

	BOOL bNegative = FALSE;

	if( iNumber < 0 )
		bNegative = TRUE;

	iNumber = abs( iNumber );

	ZeroMemory( pszBufferOut, iBufferOutLen );

	char szBuffer[256];
	STRINGFORMAT( szBuffer, "%I64d", iNumber );

	while( iNumber >= 1000 )
		iNumber /= 1000;

	int iFirstSeperatorIndex = 1;
	while( iNumber >= 10 )
	{
		iNumber /= 10;

		iFirstSeperatorIndex++;
	}

	int iLen = STRLEN( szBuffer );

	for( int i = 0, j = 0; i < iLen; i++ )
	{
		if( j >= (iBufferOutLen - 1) )
		{
			STRINGCOPYLEN( pszBufferOut, iBufferOutLen, "ERROR BUFFER LENGTH" );
			return;
		}

		if( (i != 0) && (((i - iFirstSeperatorIndex) % 3) == 0) )
			pszBufferOut[j++] = ',';

		pszBufferOut[j++] = szBuffer[i];
	}

	if( bNegative )
	{
		int j = STRLEN( pszBufferOut );

		if( j >= iBufferOutLen )
		{
			STRINGCOPYLEN( pszBufferOut, iBufferOutLen, "ERROR BUFFER LENGTH" );
			return;
		}

		for( int i = j; i > 0; i-- )
			pszBufferOut[i] = pszBufferOut[i-1];

		pszBufferOut[0] = '-';
	}
}

const char * FormatNumber( __int64 iNumber )
{
	static char szValue[128] = { 0 };
	ZeroMemory( szValue, sizeof( szValue ) );

	FormatNumber( iNumber, szValue, _countof( szValue ) );

	return szValue;
}

const char * FormatTimeLong( __int64 iTotalSeconds )
{
	static char szValue[512] = { 0 };
	ZeroMemory( szValue, sizeof( szValue ) );

	__int64 iSeconds	= iTotalSeconds % 60;
	__int64 iMinutes	= (iTotalSeconds / 60) % 60;
	__int64 iHours		= iTotalSeconds / 60 / 60;

	if( iHours > 0 )
	{
		STRINGFORMAT( szValue, "%I64d hour%s", iHours, iHours == 1 ? "" : "s" );
	}

	if( iMinutes > 0 )
	{
		char szTemp[256];

		if( iHours > 0 )
		{
			if( iSeconds > 0 )
				STRINGCONCAT( szValue, ", " );
			else
				STRINGCONCAT( szValue, " and " );
		}

		STRINGFORMAT( szTemp, "%I64d minute%s", iMinutes, iMinutes == 1 ? "" : "s" );
		
		STRINGCONCAT( szValue, szTemp );
	}

	if( iSeconds > 0 )
	{
		char szTemp[256];

		if( (iHours > 0) || (iMinutes > 0) )
			STRINGCONCAT( szValue, " and " );

		STRINGFORMAT( szTemp, "%I64d seconds", iSeconds, iSeconds == 1 ? "" : "s" );
		
		STRINGCONCAT( szValue, szTemp );
	}

	return szValue;
}

const char * FormatString( const char * pszFormat, ... )
{
	va_list vl;

	static char szString[4096];

	va_start( vl, pszFormat );
	_vsnprintf_s( szString, _countof( szString ), _TRUNCATE, pszFormat, vl );
	va_end( vl );

	return szString;
}

std::vector<std::string> split( const std::string & s, char delim )
{
	std::stringstream ss(s);
	std::string item;

	std::vector<std::string> tokens;

	while( getline( ss, item, delim ) )
		tokens.push_back( item );

	return tokens;
}


std::string & ltrim( std::string & s )
{
	s.erase( s.begin(), std::find_if( s.begin(), s.end(), []( int c ) 
	{
		if ((c < 0) || (c > 255))
			return true;
			
		return !std::isspace( c ); 
	} ) );
	return s;
}

std::string & rtrim( std::string & s )
{
	s.erase( std::find_if( s.rbegin(), s.rend(), [](int c)
	{
		if ((c < 0) || (c > 255))
			return true;

		return !std::isspace(c);
	} ).base(), s.end() );
	return s;
}

std::string & trim( std::string & s )
{
	return ltrim( rtrim( s ) );
}

void tolower( const char * pszIn, char * pszOut )
{
	int i = 0;

	while( pszIn[i] != 0 )
	{
		pszOut[i] = tolower( pszIn[i] );

		i++;
	}

	pszOut[i] = 0;
}

std::string tolower( const std::string & s )
{
	std::string r;
	r.reserve( s.length() );

	for( char c : s )
		r.push_back( lower( c ) );

	return r;
}

int GetParameterString( const char * Command, int Arg, char buf[256] )
{
	int iStringSize = lstrlenA( Command );
	if( iStringSize <= 0 )
		return 0;

	int nCountSpaces = 0;

	int i = 0;

	bool bSpaceArg = false;

	while( i < iStringSize )
	{
		if( Command[i] == ' ' )
		{
			if( bSpaceArg == false )
				nCountSpaces++;

			if( ( i + 1 ) < iStringSize )
			{
				if( Command[i + 1] != ' ' )
				{
					if( Command[i + 1] == '\"' )
					{
						if( bSpaceArg == true )
							bSpaceArg = false;
						else
							bSpaceArg = true;
					}

					if( nCountSpaces == Arg )
					{
						bool bFirstCh = false;

						if( Command[i + 1] == '\"' )
						{
							if( buf != 0 )
							{
								strcpy_s( buf, 256, &Command[i + 2] );
								for( int j = 0; j < lstrlenA( buf ); j++ )
								{
									if( buf[j] == '\"' )
									{
										buf[j] = 0;
										break;
									}

								}
							}
						}
						else
						{
							if( buf != 0 )
							{
								strcpy_s( buf, 256, &Command[i + 1] );
								for( int j = 0; j < lstrlenA( buf ); j++ )
								{
									if( buf[j] == ' ' )
									{
										buf[j] = 0;
										break;
									}

								}
							}
						}
						return i + 1;
					}
				}
				if( Command[i + 1] == ' ' )
					nCountSpaces--;
			}
		}
		else
			if( Command[i + 1] == '\"' )
			{
				if( bSpaceArg == true )
					bSpaceArg = false;
			}
		i++;
	}
	return 0;
}

std::string GetFileExtension( const std::string & strFilePath )
{
	auto stDotIndex = strFilePath.rfind( '.' );
	
	if( stDotIndex != std::string::npos )
	{
		auto stSlashIndex = strFilePath.rfind( '/' );

		if( stSlashIndex != std::string::npos )
		{
			if( stSlashIndex > stDotIndex )
				return "";
		}
		
		auto stBackslashIndex = strFilePath.rfind( '\\' );
		
		if( stBackslashIndex != std::string::npos )
		{
			if( stBackslashIndex > stDotIndex )
				return "";
		}

		char szFileExtension[MAX_PATH] = { 0 };
		STRINGCOPY( szFileExtension, strFilePath.substr( stDotIndex + 1 ).c_str() );

		for( int i = 0, j = STRLEN( szFileExtension ); i < j; i++ )
			szFileExtension[i] = tolower( szFileExtension[i] );

		return rtrim( std::string( szFileExtension ) );
	}

	return "";
}

std::string ReplaceFileExtension( const std::string & strFilePath, const std::string & strNewFileExtension )
{
	auto stIndex = strFilePath.rfind( '.' );
	
	if( stIndex != std::string::npos )
	{
		stIndex++;

		if( stIndex < strFilePath.length() )
		{
			std::string strNewFilePath = strFilePath;
			return strNewFilePath.replace( stIndex, std::string::npos, strNewFileExtension );
		}
	}

	return strFilePath;
}

char * DecodeUTF8( const char * pszText, size_t iLength, size_t iOffset, int & iOutLength )
{
	static char ret[5] = { 0 };

	ret[0] = 0;
	ret[1] = 0;
	ret[2] = 0;
	ret[3] = 0;
	ret[4] = 0;

	if( iOffset >= iLength )
		return NULL;

	const unsigned char * buf = (const unsigned char*)pszText + iOffset;

	int length = -1;

	unsigned char byte = buf[0];

	if( (byte & 0x80) == 0 )
		length = 1;
	else if( (byte & 0xE0) == 0xC0 )
		length = 2;
	else if( (byte & 0xF0) == 0xE0 )
		length = 3;
	else if( (byte & 0xF8) == 0xF0 )
		length = 4;

	if( length <= 0 )
		return NULL;

	if( (iOffset + length - 1) >= iLength )
		return NULL;

	if( byte == 0 )
		return NULL;

	ret[0] = byte;

	for( int n = 1; n < length; n++ )
		ret[n] = buf[n];

	iOutLength = length;

	return ret;
}

std::string DecodeUTF8( const std::string & strText, size_t iOffset )
{
	if( iOffset >= strText.length() )
		return "";

	const unsigned char * buf = (const unsigned char*)strText.data() + iOffset;

	int length = -1;

	unsigned char byte = buf[0];

	if( (byte & 0x80) == 0 )
		length = 1;
	else if( (byte & 0xE0) == 0xC0 )
		length = 2;
	else if( (byte & 0xF0) == 0xE0 )
		length = 3;
	else if( (byte & 0xF8) == 0xF0 )
		length = 4;

	if( length <= 0 )
		return "";

	if( (iOffset + length - 1) >= strText.length() )
		return "";

	std::string str( 1, byte );

	for( int n = 1; n < length; n++ )
		str.append( 1, buf[n] );

	return str;
}

std::string ConvertUnicode( const std::wstring & strUnicodeText )
{
	size_t sLength = strUnicodeText.length();

	if( sLength <= 0 )
		return "";

	char * pszBuffer = new char[sLength+2];

	if( !pszBuffer )
		return "";

	memset( pszBuffer, 0, sLength + 2 );

	WideCharToMultiByte( CP_UTF8, 0, strUnicodeText.c_str(), -1, pszBuffer, sLength + 1, NULL, NULL );

	std::string strUTF8 = pszBuffer;

	delete[] pszBuffer;

	return strUTF8;
}