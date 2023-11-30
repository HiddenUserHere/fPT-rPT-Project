#pragma once

enum EReaderID
{
	READERID_OpenReadBinary,
	READERID_OpenWriteBinary,
	READERID_Update,
};

namespace File
{
class CReaderFile
{
public:
	//Constructor
										CReaderFile(){};
	virtual								~CReaderFile(){};

	BOOL								OpenFile( const char * pszFileName, int iMode = READERID_OpenReadBinary );

	void								Read( void * ptr, UINT uSize );
	void								Write( void * ptr, UINT uSize );

	void								Seek( UINT uPos );
	int									GetSize();
	void								Close();

protected:
	FILE								* pFile = NULL;
};
}