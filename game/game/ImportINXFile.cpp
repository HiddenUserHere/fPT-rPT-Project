#include "stdafx.h"
#include "ImportINXFile.h"


namespace Import
{

INXFile::INXFile( const std::string & strFilePath ) : File(strFilePath)
{
}

INXFile::~INXFile()
{
	Unload();
}

bool INXFile::Load()
{
	//Try to Open the File
	if( !Open() )
		return false;

	//Debug
	WRITEDBG( "INXFile::Load() [%s]", strFilePath.c_str() );

	//Confirm Size
	if( Size() != sizeof( RawINXFile ) )
	{
		//Close File
		Close();

		return false;
	}

	//Read Raw File
	Read( &sRawFile, sizeof( RawINXFile ), 1 );

	//Decode it
	Decode();

	//Close the File
	Close();

	//OK
	bLoaded = true;

	//Load OK
	return bLoaded;
}

void INXFile::Unload()
{
	if( !bLoaded )
		return;

	//Debug
	WRITEDBG( "INXFile::Unload()" );

	//Set Unloaded
	bLoaded = false;
}

void INXFile::Decode()
{
	for( unsigned int i = 10; i < sRawFile.uAnimationCount; i++ )
	{
		RawINXAnimation * psRawAnimation = sRawFile.saAnimation + i;

		psRawAnimation->iStartFrame	= 
			((psRawAnimation->iStartFrame & 0x000000FF) << 24) | 
			(psRawAnimation->iStartFrame & 0x00FF0000) | 
			((psRawAnimation->iKey1 & 0x00FF0000) >> 8) | 
			(psRawAnimation->iKey1 & 0x000000FF);

		psRawAnimation->iEndFrame = 
			((psRawAnimation->iKey2 & 0x00FF0000) << 8) | 
			((psRawAnimation->iKey2 & 0x000000FF) << 16) | 
			((psRawAnimation->iEndFrame & 0x00FF0000) >> 8) | 
			(psRawAnimation->iEndFrame & 0x000000FF);
	}
}

}
