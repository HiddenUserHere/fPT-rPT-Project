#include "stdafx.h"
#include "CConfigFileReader.h"

CConfigFileReader::CConfigFileReader( string _filepath ) : CConfigFile( _filepath, CONFIGFILEMODE_Read )
{
}

CConfigFileReader::CConfigFileReader( string _filepath, EConfigFileMode _filemode ) : CConfigFile( _filepath, _filemode )
{
}

string CConfigFileReader::ReadString( string section, string key )
{
	BOOL opened = FALSE;

	if( !open )
	{
		if( Open() )
			opened = TRUE;
	}

	string strValue = "";

	if( open )
	{
		int line_section = FindSection( section );
		if( line_section >= 0 )
		{
			int line_key = FindKey( key, line_section );
			if( line_key >= 0 )
				strValue = GetValue( line_key );
		}
	}

	if( opened )
		Close();

	return strValue;
}

int CConfigFileReader::ReadInt( string section, string key )
{
	string sValue = ReadString( section, key );

	return atoi( sValue.c_str() );
}

long CConfigFileReader::ReadLong( string section, string key )
{
	string sValue = ReadString( section, key );

	return atol( sValue.c_str() );
}

bool CConfigFileReader::ReadBool( string section, string key, string value )
{
	string sValue = ReadString( section, key );

	if( sValue.compare( value ) == 0 )
		return true;

	return false;
}

string CConfigFileReader::GetValue( int line_key )
{
	fseek( file, line_key, SEEK_SET );

	char buffer[256];

	fgets( buffer, _countof( buffer ), file );

	char ret_buffer[256];

	int k = 0;
	bool r = false;
	for( int i = 0, j = lstrlenA( buffer ); i < j; i++ )
	{
		char c = buffer[i];

		if( r )
		{
			if( c == 0 )
				break;
			if( c == '\n' )
				break;
			if( c == '\r' )
				break;

			ret_buffer[k++] = c;
		}
		else if( c == '=' )
		{
			r = true;
			continue;
		}
	}

	ret_buffer[k] = 0;

	return ret_buffer;
}