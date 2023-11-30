#include "stdafx.h"
#include "ExportFile.h"

namespace Export
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

bool File::Open()
{
	if( pFile )
		Close();

	if( fopen_s( &pFile, strFilePath.c_str(), "wb" ) == 0 )
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

void File::Write( void * pBuffer, size_t iElementSize, size_t iCount )
{
	fwrite( pBuffer, iElementSize, iCount, pFile );
}

void File::Close()
{
	if( pFile )
		fclose( pFile );

	pFile = NULL;
}

}