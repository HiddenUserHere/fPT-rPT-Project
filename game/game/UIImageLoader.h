#pragma once

struct stImageStructure
{
	IDirect3DTexture9 *hTexHandler;
	char szFilename[ 256 ];
	int width;
	int height;
	int iFileFormat;
};

namespace UI
{
class ImageLoader 
{
public:
														ImageLoader();
	virtual												~ImageLoader();


	static void												SetScale( class Image * pImage, int iW, int iH, D3DXVECTOR2 * pVector );
	static class Image										* Swap( Image * ptr );
	static class Image										* LoadImage( const char * pszFile, ... );
	static std::pair<class Image *, class Image *>			LoadActiveImage( const char * pszFile1, const char * pszFile2 );

	static void												DeleteImage( Image * ptr );


};
}