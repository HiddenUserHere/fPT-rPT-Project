#include "stdafx.h"
#include "EXETexture.h"

const char * ENCRYPT_HASH_PNGPT = AY_OBFUSCATE( "~0[)^'01,%6!,~`*&1!.+~,=(.,1^;589#,!42.7*8^[&+;*9`([&%-*]7-13+'%)94.;/=~6#7[8#';1/$3@.@8-+;2@=1*)7@[8++*5@)/7786,@/(66.()!0,..@@" );
const char * ENCRYPT_HASH_BMPPT = AY_OBFUSCATE( "0$'-/9%'.49]%-1.`&%)#))+3%,1977+4(@(69^1+2+#.^1*#3$;3/)641-[!1-)3@6'1022)5++/4*$+`;%!(-^[;3%7$%5&2'=7/$;2-2-*/1;+[^&58!-^^5`63($" );
const char * ENCRYPT_HASH_TGAPT = AY_OBFUSCATE( "/=9!)(99*34~$&1#,5)]&4`=.^3=(,*-99())=50~*+!-3%6,589$%~('%3@4/0^#^[63$,!%/!(#9^0^==/(~+*9-.,[/-46.=,^+(1#*(([,)-'..&,+$#@)(-&]!#" );
const char * ENCRYPT_HASH_JPGPT = AY_OBFUSCATE( "%-0~3(&(]=-,116^!)$!48@)`&0-!1/;-)]1@4.$(++;#'85%7$5!75*7!]9`005)%~=57'7!0..`/]1#;,=)='5%@&'8,9*(4'($0@^`~2912$')6%;8/(/;[1-87;`" );

static const BYTE ENCRYPT_HASH_TEXTUREPT[] = { 0x7f, 0x20, 0x13, 0xff, 0x1e, 0x67, 0x8d, 0x3d, 0x61, 0x87, 0x48, 0x95, 0x01, 0xe5, 0xaf, 0x28,
									0xf9, 0x4b, 0x86, 0x29, 0xe6, 0xcc, 0x36, 0x49, 0xfa, 0x65, 0x4b, 0xf0, 0x2b, 0xd6, 0xae, 0x46 };


NAKED void EXETextureManager::ReadTextures(int iMip)
{
	JMP( pfnReadTextures );
}

NAKED void EXETextureManager::Constructor( int _iMaxTextures )
{
	JMP( pfnConstructor );
}


EXETextureManager::EXETextureManager( int _iMaxTextures )
{
	Constructor( _iMaxTextures );
}

EXETextureManager::EXETextureManager()
{
}

UINT EXETextureManager::LoadedTexture( std::string strFile )
{
	for ( int i = 0; i < iTextureCount; i++ )
	{
		EXETextureMaterial * pc = pcaTexture + i;

		if ( pc->bInUse && pc->psTexture )
		{
			if ( STRINGCOMPAREI( pc->psTexture[0]->szName, strFile.c_str() ) )
				return (UINT)i;
		}
	}

	return (-1);
}

UINT EXETextureManager::LoadTexture( string strFilePath )
{
	UINT uIndex = LoadedTexture( strFilePath );

	//Already Loaded?
	if ( uIndex != -1 )
		return uIndex;

	uIndex = FreeTextureIndex();

	//No Free Positions?
	if ( uIndex == -1 )
		return 0;

	EXETextureMaterial * pTextureWrapper = pcaTexture + uIndex;

	pTextureWrapper->iTextureCounter = 1;
	pTextureWrapper->psTexture[0] = EXETexture::GetInstance()->Add( (char*)strFilePath.c_str() );

	if ( pTextureWrapper->psTexture[0] == NULL )
	{
		pTextureWrapper->iTextureCounter = 0;

		return 0;
	}

	EXETexture::LoadTexture( pTextureWrapper->psTexture[0], 0, (pTextureWrapper->psTexture[0]->bSwap == TRUE) );

	pTextureWrapper->bInUse = TRUE;

	return uIndex;
}

UINT EXETextureManager::FreeTextureIndex()
{
	for ( UINT i = 0; i < (UINT)iMaxTexture; i++ )
	{
		EXETextureMaterial * pTextureWrapper = pcaTexture + i;

		if ( pTextureWrapper->bInUse == FALSE )
		{
			iTextureCount++;

			return i;
		}
	}

	return -1;
}

NAKED int EXETextureManager::GetSaveSize()
{
	JMP( 0x00489700 );
}

NAKED int EXETextureManager::SaveFile( HANDLE hFile )
{
	JMP( 0x00489830 );
}

int EXETexture::LoadTexture( EXETextureHandle * psTexture, bool bMipmap, bool bSwap )
{
	FILE * pFile = NULL;

	static char * pBuffer = NULL;

	if ( pBuffer == NULL )
		pBuffer = new char[50 * 1000 * 1000];

	fopen_s( &pFile, psTexture->szName, "rb" );

	BOOL bRet = 0;

	if ( pFile )
	{
		fseek( pFile, 0, SEEK_END );
		int iSize = ftell( pFile );
		fseek( pFile, 0, SEEK_SET );

		if ( iSize <= 0 )
		{
			fclose( pFile );
			return 0;
		}

		fread( pBuffer, iSize, 1, pFile );

		fclose( pFile );

		//Get MipMap Count
		UINT uMipLevels = 0;

		//MipMap?
		if ( bMipmap == true )
		{
			char szFilePath[MAX_PATH] = { 0 };
			STRINGCOPY( szFilePath, psTexture->szName );

			const char * iDot = strrchr( szFilePath, '.' );

			if ( iDot != NULL )
			{
				char szBaseFile[MAX_PATH], szFileExtension[MAX_PATH];

				int iLenToDot = iDot - szFilePath;

				//Get Base File
				STRINGCOPY( szBaseFile, szFilePath );
				szBaseFile[iLenToDot] = 0;

				//Get File Extension
				STRINGCOPY( szFileExtension, szBaseFile + iLenToDot + 1 );

				//Find MipMap Levels
				for ( int i = 1; i <= 4; i++ )
				{
					char szBuffer[MAX_PATH];
					STRINGFORMAT( szBuffer, "%s_mm%d.%s", szBaseFile, i, szFileExtension );

					uMipLevels += PathFileExistsA( szBuffer ) ? 1 : 0;
				}
			}
		}

		//Get Extension
		std::string strFileExtension = GetFileExtension( psTexture->szName );

		D3DCOLOR d3dcolorkey = D3DCOLOR_ARGB( 0, 0, 0, 0 );

		if ( EXETexture::EncryptDecryptTextureBuffer( pBuffer, iSize, GetHashTextureExtension( psTexture->szName ), false ) )
		{
			iSize -= _countof( ENCRYPT_HASH_TEXTUREPT );
		}

		if ( strFileExtension.compare( "tga" ) == 0 )
		{
			psTexture->iTransparency = TRUE;

			//Decrypt
			if ( DecryptTGA( pBuffer, iSize ) == false )
			{
				return 0;
			}
		}

		if ( strFileExtension.compare( "bmp" ) == 0 )
		{
			psTexture->iTransparency = FALSE;

			if ( bSwap == false )
				d3dcolorkey = D3DCOLOR_ARGB( 255, 0, 0, 0 );

			//Decrypt
			if ( DecryptBMP( pBuffer, iSize ) == false )
			{
				return 0;
			}
		}

		if ( strFileExtension.compare( "jpg" ) == 0 )
		{
			psTexture->iTransparency = FALSE;
		}

		if ( strFileExtension.compare( "png" ) == 0 )
		{
			psTexture->iTransparency = TRUE;
		}

		UINT uWidth		= D3DX_DEFAULT_NONPOW2;
		UINT uHeight	= D3DX_DEFAULT_NONPOW2;

		D3DXIMAGE_INFO ii;
		ZeroMemory( &ii, sizeof( D3DXIMAGE_INFO ) );

		if ( bSwap == true && (*(int*)0x03A9A334) >= 0 )
		{
			if ( FAILED( D3DXGetImageInfoFromFileInMemory( pBuffer, iSize, &ii ) ) )
			{
				return 0;
			}

			uWidth = ii.Width >> (*(int*)0x03A9A334);
			uHeight = ii.Height >> (*(int*)0x03A9A334);
		}

		LPDIRECT3DTEXTURE9 lpT = NULL;

		HRESULT hr;

		if ( FAILED( hr = D3DXCreateTextureFromFileInMemoryEx(
			GRAPHICENGINE->GetDevice(),
			pBuffer,
			iSize,
			uWidth,
			uHeight,
			uMipLevels == 0 ? D3DX_FROM_FILE : uMipLevels + 1,
			0,
			/*D3DX_FROM_FILE,
			D3DUSAGE_AUTOGENMIPMAP, */
			D3DFMT_FROM_FILE,
			DXGRAPHICENGINE_D3DPOOL,
			D3DX_FILTER_POINT,
			D3DX_DEFAULT,
			d3dcolorkey,
			&ii,
			NULL,
			&lpT ) ) )
		{
			psTexture->pcD3DTexture = lpT;
			return -1;
		}

		//lpT->SetAutoGenFilterType( D3DTEXF_POINT );

		psTexture->pcD3DTexture = lpT;

		psTexture->iWidth = ii.Width;
		psTexture->iHeight = ii.Height;

		bRet = 1;
	}

	return bRet;
}

BOOL EXETexture::ReadTextures()
{
	static DWORD dwTime = 0;
/*
	if ( Game::GetGameMode() == GAMEMODE_InGame )
	{
		if ( dwTime && (dwTime + 5000) < TICKCOUNT )
			CAntiDebuggerHandler::CreateHiddenThread( (LPTHREAD_START_ROUTINE)&EXETexture::ReadTexturesWorkerThread, this, KB16 );
		else
		{
			ReadTexturesSafe();
			dwTime = TICKCOUNT;
		}
	}
	else
		ReadTexturesSafe();
*/
	ReadTexturesSafe();


	return TRUE;
}

BOOL EXETexture::ReadTexturesSafe()
{
	if ( ISTEXTUREREADY )
	{
		EnterCriticalSection( &cCriticalSection );

		for ( int i = 0; i < 5000; i++ )
		{
			EXETextureHandle * ps = &saTexture[i];

			if ( ps->iUseCounter && ps->pcD3DTexture == NULL )
			{
				//Load Texture
				if ( (EXETexture::LoadTexture( ps, 0, (ps->bSwap == TRUE) ? false : true ) < 0) && (ps->pcD3DTexture == NULL) )
					DeleteSafe( ps );
			}
		}

		LeaveCriticalSection( &cCriticalSection );
	}

	return TRUE;
}

BOOL EXETexture::ReadTexturesWorkerThread( void * ptr )
{
	EXETexture * p = (EXETexture *)ptr;
	p->ReadTexturesSafe();

	ExitThread( 0 );
	return 0;
}

NAKED EXETextureHandle * EXETexture::Add( char * pszTexture, char * pszMask )
{
	JMP( 0x004892F0 );
}

int EXETexture::GetTextureIndex( std::string strFile )
{
	for ( int i = 0; i < 5000; i++ )
	{
		EXETextureHandle * ps = &saTexture[i];

		if ( ps->pcD3DTexture )
		{
			if ( STRINGCOMPAREI( ps->szName, strFile.c_str() ) )
				return i;
		}
	}

	return (-1);
}

EXETextureHandle * EXETexture::GetTexture( int iIndex )
{
	if ( iIndex >= 0 && iIndex < 5000 )
		return &saTexture[iIndex];

	return NULL;
}

BOOL EXETexture::DeleteSafe( EXETextureHandle * ps )
{
	if ( ps != NULL )
		return Delete( ps );

	return FALSE;
}

NAKED BOOL EXETexture::Delete( EXETextureHandle * ps )
{
	JMP( pfnDeleteTexture );
}

bool EXETexture::EncryptDecryptTextures( const std::string & strPath, bool bEncrypt )
{
	auto vFiles = SYSTEM->ListFilesInDirectory( strPath, "png" );
	for ( auto strFile : vFiles )
		EncryptDecryptTexture( strFile, ENCRYPT_HASH_PNGPT, bEncrypt );

	vFiles = SYSTEM->ListFilesInDirectory( strPath, "bmp" );
	for ( auto strFile : vFiles )
		EncryptDecryptTexture( strFile, ENCRYPT_HASH_BMPPT, bEncrypt );

	vFiles = SYSTEM->ListFilesInDirectory( strPath, "tga" );
	for ( auto strFile : vFiles )
		EncryptDecryptTexture( strFile, ENCRYPT_HASH_TGAPT, bEncrypt );

	vFiles = SYSTEM->ListFilesInDirectory( strPath, "jpg" );
	for ( auto strFile : vFiles )
		EncryptDecryptTexture( strFile, ENCRYPT_HASH_JPGPT, bEncrypt );

	return true;
}

bool EXETexture::EncryptDecryptTexture( const std::string & strFile, const char * pszHash, bool bEncrypt )
{
	FILE * pFile = nullptr;
	fopen_s( &pFile, strFile.c_str(), "rb" );
	if ( pFile )
	{
		fseek( pFile, 0, SEEK_END );
		int iFileSize = ftell( pFile );
		fseek( pFile, 0, SEEK_SET );

		char * pBuffer = new char[iFileSize];
		fread( pBuffer, iFileSize, 1, pFile );

		fclose( pFile );

		pFile = nullptr;
		fopen_s( &pFile, strFile.c_str(), "wb" );
		if ( pFile )
		{
			if ( EncryptDecryptTextureBuffer( pBuffer, iFileSize, pszHash, bEncrypt ) )
			{
				if ( bEncrypt == false )
					iFileSize -= _countof( ENCRYPT_HASH_TEXTUREPT );

				fwrite( pBuffer, iFileSize, 1, pFile );

				if ( bEncrypt )
					fwrite( ENCRYPT_HASH_TEXTUREPT, _countof( ENCRYPT_HASH_TEXTUREPT ), 1, pFile );
			}
			else
				fwrite( pBuffer, iFileSize, 1, pFile );

			fclose( pFile );
		}

		DELETA( pBuffer );
	}

	return true;
}
bool EXETexture::EncryptDecryptTextureBuffer( char * pBuffer, int iSize, const char * pszHash, bool bEncrypt )
{
	bool bRet = false;

	CEncryptXOR cXOR;

	if ( (bEncrypt == false) && (memcmp( pBuffer + iSize - 32, ENCRYPT_HASH_TEXTUREPT, _countof( ENCRYPT_HASH_TEXTUREPT ) ) == 0) )
	{
		cXOR.EncDecrypt( (BYTE *)pBuffer, iSize - 32, pszHash );

		bRet = true;
	}
	else if ( bEncrypt && (memcmp( pBuffer + iSize - 32, ENCRYPT_HASH_TEXTUREPT, _countof( ENCRYPT_HASH_TEXTUREPT ) ) != 0) )
	{
		cXOR.EncDecrypt( (BYTE *)pBuffer, iSize, pszHash );

		bRet = true;
	}

	return bRet;
}
const char * EXETexture::GetHashTextureExtension( const std::string & strFile )
{
	std::string strFileExtension = GetFileExtension( strFile );

	if ( strFileExtension.compare( "png" ) == 0 )
		return ENCRYPT_HASH_PNGPT;
	if ( strFileExtension.compare( "bmp" ) == 0 )
		return ENCRYPT_HASH_BMPPT;
	if ( strFileExtension.compare( "tga" ) == 0 )
		return ENCRYPT_HASH_TGAPT;
	if ( strFileExtension.compare( "jpg" ) == 0 )
		return ENCRYPT_HASH_JPGPT;

	return nullptr;
}
int EXETexture::DecryptExternal( char * pBuffer, int iSize, const char * pszFileName )
{
	if ( EXETexture::EncryptDecryptTextureBuffer( pBuffer, iSize, GetHashTextureExtension( pszFileName ), false ) )
		return _countof( ENCRYPT_HASH_TEXTUREPT );

	return 0;
}
;

NAKED bool EXETexture::DecryptBMP( char * pBuffer, int iSize )
{
	__asm
	{
		MOV EAX, 0x0063C190;
		JMP EAX;
	}
}

NAKED bool EXETexture::DecryptTGA( char * pBuffer, int iSize )
{
	__asm
	{
		MOV EAX, 0x0063C1E0;
		JMP EAX;
	}
}

bool EXETexture::DecryptJPG( char * pBuffer, int iSize )
{
	return false;
}

bool EXETexture::DecryptPNG( char * pBuffer, int iSize )
{
	return false;
}


