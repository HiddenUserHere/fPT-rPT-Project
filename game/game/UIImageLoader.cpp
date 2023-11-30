#include "stdafx.h"
#include "UIImageLoader.h"

typedef std::shared_ptr<stImageStructure> pImageSt_ptr;

std::vector<UI::Image*> vImageResource;

extern const char * ENCRYPT_HASH_PNGPT;
extern const char * ENCRYPT_HASH_BMPPT;
extern const char * ENCRYPT_HASH_TGAPT;
extern const char * ENCRYPT_HASH_JPGPT;


namespace UI
{
ImageLoader::ImageLoader()
{
}

ImageLoader::~ImageLoader()
{
}

bool __declspec(naked) DecryptBMP( char * pBuffer, UINT uSize )
{
	__asm
	{
		MOV EAX, 0x0063C190;
		JMP EAX;
	}
}

__declspec(naked) bool DecryptTGA( char * pBuffer, UINT uSize )
{
	__asm
	{
		MOV EAX, 0x0063C1E0;
		JMP EAX;
	}
}


void ImageLoader::SetScale( UI::Image * pImage, int iW, int iH, D3DXVECTOR2 * pVector )
{
	if ( pImage->GetWidth() > iW )
		pVector->x = (float)pImage->GetWidth() / (float)iW;
	else
		pVector->x = (float)iW / (float)pImage->GetWidth();

	if ( pImage->GetHeight() > iH )
		pVector->y = (float)pImage->GetHeight() / (float)iH;
	else
		pVector->y = (float)iH / (float)pImage->GetHeight();
}

class Image * ImageLoader::Swap(Image * ptr)
{
	for ( UINT u = 0; u < vImageResource.size(); u++ )
	{
		if ( vImageResource[u]->GetPointer() == ptr )
		{
			char szFileName[256];
			STRINGCOPY( szFileName, vImageResource[u]->GetFileName() );
			vImageResource[u]->SetFile( "t" );
			delete vImageResource[u];
			vImageResource[u] = LoadImage( szFileName );
			return vImageResource[u];
			break;
		}
	}
	return NULL;
}

UI::Image * ImageLoader::LoadImage( const char * pszFile, ... )
{
	char szBuf[ 1024 ];
	ZeroMemory( szBuf, 1024 );
	va_list ap;
	va_start( ap, pszFile );
	_vsnprintf_s( szBuf, 1024, 1023, pszFile, ap );
	va_end( ap );

	// Get the size of string
	int iStrNameSize = lstrlenA( szBuf );
	if ( iStrNameSize <= 4 )
	{
		return NULL;
	}

	for ( UINT u = 0; u < vImageResource.size(); u++ )
	{
		if ( STRINGCOMPARE( vImageResource[u]->GetFileName(), szBuf ) )
		{
			vImageResource[u]->AddRef();
			return vImageResource[u];
		}
	}

	D3DXIMAGE_INFO SrcInfo;

	// Define temp Alloc
	UI::Image * pImage = new UI::Image();

	// Use a magenta colourkey
	D3DCOLOR colorkey = 0xFFFF00FF;

	// Open File Memory
	FILE * fp;
	fopen_s( &fp, szBuf, "rb" );
	if ( fp )
	{
		// Get File Size
		UINT uFileSize;
		fseek( fp, 0, SEEK_END );
		uFileSize = ftell( fp );
		fseek( fp, 0, SEEK_SET );

		// Buffer Read
		char * pBuffer = new char[uFileSize];

		// Read Buffer
		fread( pBuffer, uFileSize, 1, fp );
		fclose( fp );

		// Check Decrypt
		std::string strFileExtension = GetFileExtension( szBuf );
		if ( strFileExtension.compare( "bmp" ) == 0 )
		{
			if ( !DecryptBMP( pBuffer, uFileSize ) )
			{
				DELET( pBuffer );
				return NULL;
			}

			EXETexture::EncryptDecryptTextureBuffer( pBuffer, uFileSize, ENCRYPT_HASH_BMPPT, false );
		}

		if ( strFileExtension.compare( "tga" ) == 0 )
		{
			if ( !DecryptTGA( pBuffer, uFileSize ) )
			{
				DELET( pBuffer );
				return NULL;
			}			
		}

		if ( strFileExtension.compare( "png" ) == 0 )
			EXETexture::EncryptDecryptTextureBuffer( pBuffer, uFileSize, ENCRYPT_HASH_PNGPT, false );

		IDirect3DTexture9 * lpTexture = NULL;

		//WebP?
		if ( strFileExtension.compare( "webp" ) == 0 )
		{
			bool bSuccess = false;

			//Get Image info
			if ( WebPGetInfo( (uint8_t *)pBuffer, uFileSize, (int *)&SrcInfo.Width, (int *)&SrcInfo.Height ) )
			{
				//Create Texture
				if ( SUCCEEDED( DX::Graphic::GetDevice()->CreateTexture( SrcInfo.Width, SrcInfo.Height, 1, 0, GRAPHICS->presentParams.BackBufferFormat == D3DFMT_X4R4G4B4 ? D3DFMT_A4R4G4B4 : D3DFMT_A8R8G8B8, DXGRAPHICENGINE_D3DPOOL, &lpTexture, NULL ) ) )
				{
					WebPDecoderConfig sWebPConfig;
					if ( WebPInitDecoderConfig( &sWebPConfig ) )
					{
						//Copy Pixels to the Texture
						D3DLOCKED_RECT d3dRect;
						RECT rc;
						rc.left		= rc.top = 0;
						rc.right	= SrcInfo.Width;
						rc.bottom	= SrcInfo.Height;
						if ( SUCCEEDED( lpTexture->LockRect( 0, &d3dRect, &rc, D3DLOCK_READONLY ) ) )
						{
							//Setup the Output Image Data
							sWebPConfig.output.colorspace			= MODE_BGRA; //PNG Texture
							sWebPConfig.output.u.RGBA.rgba			= (uint8_t *)d3dRect.pBits;
							sWebPConfig.output.u.RGBA.stride		= d3dRect.Pitch;
							sWebPConfig.output.u.RGBA.size			= SrcInfo.Height * d3dRect.Pitch;
							sWebPConfig.output.is_external_memory	= 1;

							if ( WebPDecode( (uint8_t *)pBuffer, uFileSize, &sWebPConfig ) == VP8_STATUS_OK )
							{
								//Set as PNG (color changes)
								SrcInfo.ImageFileFormat = D3DXIFF_PNG;

								bSuccess = true;
							}

							//Free the Output Data
							WebPFreeDecBuffer( &sWebPConfig.output );

							//Unlock Texture
							lpTexture->UnlockRect( 0 );
						}
					}
				}
			}

			if ( !bSuccess )
			{
				//Release Texture
				RELEASE( lpTexture );

				DELET( pBuffer );

				return NULL;
			}
		}
		else
		{
			// Get Image Info
			D3DXGetImageInfoFromFileInMemory( pBuffer, uFileSize, &SrcInfo );

			if ( SrcInfo.ImageFileFormat == D3DXIFF_BMP )
				colorkey = 0xFF000000;

			// Load image from file.
			if ( FAILED( D3DXCreateTextureFromFileInMemoryEx( DX::Graphic::GetDevice(), pBuffer, uFileSize, SrcInfo.Width, SrcInfo.Height, 1, 0, (D3DFORMAT)D3DX_FROM_FILE, DXGRAPHICENGINE_D3DPOOL, D3DX_DEFAULT, D3DX_DEFAULT, SrcInfo.ImageFileFormat == D3DXIFF_BMP ? D3DCOLOR_ARGB( 255, 0, 0, 0 ) : 0, &SrcInfo, NULL, &lpTexture ) ) )
			{
				if ( FILEEXIST( szBuf ) == false )
				{
					char szTextureError[512] = { 0 };
					STRINGFORMAT( szTextureError, "Could not find file %s\n", szBuf );
					MessageBoxA( 0, szTextureError, "Error", MB_ICONERROR | MB_OK );
					ExitProcess( 0 );
				}
			}
		}

		pImage->SetWidth( SrcInfo.Width );
		pImage->SetHeight( SrcInfo.Height );
		pImage->SetFormat( SrcInfo.ImageFileFormat );
		pImage->SetFile( szBuf );
		pImage->SetID( vImageResource.size(), lpTexture );

		vImageResource.push_back( pImage );

		DELET( pBuffer );

		pImage->AddRef();

		return pImage;
	}

	return NULL;
}

std::pair<UI::Image *, UI::Image *> ImageLoader::LoadActiveImage( const char * pszFile1, const char * pszFile2 )
{
	UI::Image * f = ImageLoader::LoadImage( pszFile1 );
	UI::Image * s = ImageLoader::LoadImage( pszFile2 );
	return std::pair<UI::Image *, UI::Image *>( f, s );
}

void ImageLoader::DeleteImage( Image * ptr )
{
	if ( ptr )
	{
		ptr->DecRef();

		if ( ptr->GetRef() == 0 )
		{
			for ( std::vector<UI::Image*>::iterator it = vImageResource.begin(); it != vImageResource.end(); )
			{
				UI::Image * pc = (*it);
				if ( pc == ptr )
				{
					LPDIRECT3DTEXTURE9 lp = pc->GetTexture();
					RELEASE( lp );
					DELET( pc );

					it = vImageResource.erase( it );
				}
				else
					it++;
			}
		}
	}
}


}