#include "stdafx.h"
#include "CConfigFile.h"

CConfigFile::CConfigFile( string _filepath, EConfigFileMode _filemode )
{
	filepath	= _filepath;
	filemode	= _filemode;

	open		= FALSE;
	file		= NULL;
}

CConfigFile::~CConfigFile()
{
	if( open )
		Close();
}

BOOL CConfigFile::Open()
{
	if( fopen_s( &file, filepath.c_str(), FileModeToString( filemode ).c_str() ) == 0 )
		open = TRUE;

	return open;
}

void CConfigFile::Close()
{
	if( open )
	{
		fflush( file );
		fclose( file );

		open = FALSE;
	}
}

void CConfigFile::Remove()
{
	remove( filepath.c_str() );
}

int CConfigFile::FindSection( string section )
{
	fseek( file, 0, SEEK_SET );

	char buffer[256];

	while( !feof( file ) )
	{
		fpos_t line;
		fgetpos( file, &line );

		fgets( buffer, _countof( buffer ), file );

		if( buffer[0] == '#' )
			continue;

		if( buffer[0] == '[' )
		{
			string section_comp = section;
			section_comp.append( "]" );

			if( strncmp( buffer + 1, section_comp.c_str(), section_comp.length() ) == 0 )
				return (int)line;
		}
	}

	return -1;
}

int CConfigFile::FindKey( string key, int line_section )
{
	fseek( file, line_section, SEEK_SET );
	
	char buffer[256];
	fgets( buffer, _countof( buffer ), file );

	while( !feof( file ) )
	{
		fpos_t line;
		fgetpos( file, &line );

		fgets( buffer, _countof( buffer ), file );

		if( buffer[0] == '#' )
			continue;

		if( buffer[0] == '[' )
			break;

		string key_comp = key;
		key_comp.append( "=" );

		if( strncmp( buffer, key.c_str(), key.length() ) == 0 )
			return (int)line;
	}

	return -1;
}

string CConfigFile::FileModeToString( EConfigFileMode filemode )
{
	switch( filemode )
	{
	case CONFIGFILEMODE_Read:
		return "rt";
	case CONFIGFILEMODE_Write:
		return "wt";
	case CONFIGFILEMODE_ReadWrite:
		return "rt+";
	case CONFIGFILEMODE_ReadBinary:
		return "rb";
	case CONFIGFILEMODE_WriteBinary:
		return "wb";
	case CONFIGFILEMODE_ReadWriteBinary:
		return "rb+";
	}

	throw "No valid File Mode";
}