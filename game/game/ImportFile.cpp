#include "stdafx.h"
#include "ImportFile.h"

namespace Import
{

File::File( const std::string & strFilePath ) : strFilePath(strFilePath)
{
}

bool File::Exists() const
{
	FILE * f;
	if( fopen_s( &f, strFilePath.c_str(), "r" ) == 0 )
	{
		fclose( f );
		return true;
	}

	return false;
}

bool File::IsOpen() const
{
	return pFile != NULL;
}

const std::string File::GetFileExtension()
{
	if ( strFilePath.find_last_of( "." ) != std::string::npos )
		return strFilePath.substr( strFilePath.find_last_of( "." ) + 1 );

	return "";
}

bool File::Open()
{
	if( pFile )
		Close();

	if( fopen_s( &pFile, strFilePath.c_str(), "rb" ) == 0 )
		return true;

	return false;
}

long File::Tell()
{
	return ftell( pFile );
}

long File::Size()
{
	long pos = Tell();

	fseek( pFile, 0, SEEK_END );
	long ret = ftell( pFile );

	Seek( pos );

	return ret;
}

void File::Seek( long lPosition )
{
	fseek( pFile, lPosition, SEEK_SET );
}

void File::Read( void * pBuffer, size_t iElementSize, size_t iCount )
{
	fread( pBuffer, iElementSize, iCount, pFile );
}

void File::Close()
{
	if( pFile )
		fclose( pFile );

	pFile = NULL;
}

}