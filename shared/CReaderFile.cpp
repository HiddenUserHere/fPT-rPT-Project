#include "stdafx.h"
#include "CReaderFile.h"

namespace File
{
BOOL CReaderFile::OpenFile( const char * pszFileName, int iMode )
{
	// Open File
	char szMode[4] = { 0 };
	if( iMode == READERID_OpenReadBinary )
		STRINGCOPY( szMode, "rb" );
	else if( iMode == READERID_OpenWriteBinary )
		STRINGCOPY( szMode, "wb" );
	else if( iMode == READERID_Update )
		STRINGCOPY( szMode, "ab" );

	fopen_s( &pFile, pszFileName, szMode );

	if( !pFile )
		return FALSE;
	return TRUE;
}

void CReaderFile::Read( void * ptr, UINT uSize )
{
	if( pFile )
		fread( ptr, uSize, 1, pFile );
}

void CReaderFile::Write( void * ptr, UINT uSize )
{
	if( pFile )
		fwrite( ptr, uSize, 1, pFile );
}

void CReaderFile::Seek( UINT uPos )
{
	if( pFile )
		fseek( pFile, uPos, SEEK_SET );
}

int CReaderFile::GetSize()
{
	if( pFile )
	{
		fseek( pFile, 0, SEEK_END );
		int iSize = ftell( pFile );
		fseek( pFile, 0, SEEK_SET );
		return iSize;
	}
	return 0;
}

void CReaderFile::Close()
{
	if( pFile )
		fclose( pFile );
}
}