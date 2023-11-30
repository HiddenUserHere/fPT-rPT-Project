#include "stdafx.h"
#include "CConfigFileWriter.h"

CConfigFileWriter::CConfigFileWriter( string _filepath ) : CConfigFileReader( _filepath, CONFIGFILEMODE_ReadWrite )
{
}

void CConfigFileWriter::WriteString( string section, string key, string value )
{
	BOOL opened = FALSE;

	if( !open )
	{
		if( Open() )
			opened = TRUE;
	}

	if( open )
	{
		int line_section = FindSection( section );
		int line_key = -1;

		if( line_section != -1 )
		{
			line_key = FindKey( key, line_section );

			if( line_key != -1 )
			{
				//Just set the value on the line
				SetValue( key, value, line_key );
			}
			else
			{
				//Create a new key with the value
			}
		}
		else
		{
			//Create a new section with the new key and value
		}
	}

	if( opened )
		Close();
}

void CConfigFileWriter::SetValue( string key, string value, int line_key )
{
	EConfigFileMode old_filemode = filemode;

	Close();
	filemode = CONFIGFILEMODE_ReadBinary;
	if( Open() )
	{
		fseek( file, 0, SEEK_END );
		int file_size = ftell( file );
		fseek( file, 0, SEEK_SET );

		char * buffer = new char[file_size];

		fread( buffer, 1, file_size, file );

		Close();
		Remove();

		filemode = CONFIGFILEMODE_WriteBinary;
		if( Open() )
		{
			fwrite( buffer, 1, line_key, file );

			string new_row = key;
			new_row.append( "=" );
			new_row.append( value );

			fwrite( new_row.c_str(), 1, new_row.length(), file );

			int j = 0;
			for( int i = line_key; i < file_size - line_key; i++, j++ )
			{
				if( buffer[i] == 0x0D )
					break;
			}

			j += line_key;

			fwrite( buffer + j, 1, file_size - j, file );
			fflush( file );
			
			Close();

			filemode = old_filemode;
			Open();
		}
		else
		{
			if( fopen_s( &file, filepath.c_str(), "wb" ) == 0 )
			{
				fwrite( buffer, 1, file_size, file );
				fflush( file );
				fclose( file );
			}
		}

		delete[] buffer;
	}
}