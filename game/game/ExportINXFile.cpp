#include "stdafx.h"
#include "ExportINXFile.h"


namespace Export
{

INXFile::INXFile( const std::string & strFilePath ) : File(strFilePath)
{
}

bool INXFile::Save( const RawINXFile * pRawFile )
{
	//Try to Open the File for Writing
	if( !Open() )
		return false;

	//Debug
	WRITEDBG( "INXFile::Save() [%s]", strFilePath.c_str() );

	//Copy Raw File for Encoding
	memcpy( &sRawFile, pRawFile, sizeof( RawINXFile ) );

	//Encode it
	Encode();

	//Save to File
	Write( &sRawFile, sizeof( RawINXFile ), 1 );

	//Close the File
	Close();

	//Save OK
	return true;
}

void INXFile::Encode()
{
	int iObscurity = -1;

	for( unsigned int i = 10; i < sRawFile.uAnimationCount; i++ )
	{
		RawINXAnimation * psRawAnimation = sRawFile.saAnimation + i;

		int iStartFrame	= psRawAnimation->iStartFrame;
		int iEndFrame	= psRawAnimation->iEndFrame;

		psRawAnimation->iKey1		= (iObscurity & 0xFF000000) | ((iStartFrame & 0x0000FF00) << 8) | ((iObscurity & 0x00FF0000) >> 8) | (iStartFrame & 0x000000FF);
		psRawAnimation->iStartFrame	= ((iObscurity & 0x000000FF) << 24) | (iStartFrame & 0x00FF0000) | (iObscurity & 0x0000FF00) | ((iStartFrame & 0xFF000000) >> 24);

		psRawAnimation->iKey2		= ((iObscurity & 0x000000FF) << 24) | ((iEndFrame & 0xFF000000) >> 8) | (iObscurity & 0x0000FF00) | ((iEndFrame & 0x00FF0000) >> 16);
		psRawAnimation->iEndFrame	= ((iObscurity & 0x00FF0000) << 8) | ((iEndFrame & 0x0000FF00) << 8) | ((iObscurity & 0xFF000000) >> 16) | (iEndFrame & 0x000000FF);
	}
}

}
