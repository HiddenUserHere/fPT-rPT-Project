#pragma once

enum EBlendType
{
	BLENDTYPE_None			= 0,
	BLENDTYPE_Alpha			= 0x00000001,
	BLENDTYPE_Color			= 0x00000002,
	BLENDTYPE_Shadow		= 0x00000003,
	BLENDTYPE_Lamp			= 0x00000004,
	BLENDTYPE_AddColor		= 0x00000005,
	BLENDTYPE_InvShadow		= 0x00000006,
};

enum ETextureModifier
{
	TEXTUREMODIFIER_None,
	TEXTUREMODIFIER_FormX,
	TEXTUREMODIFIER_FormY,
	TEXTUREMODIFIER_FormZ,
	TEXTUREMODIFIER_Scroll,
	TEXTUREMODIFIER_Reflex,
	TEXTUREMODIFIER_Scroll2,
	TEXTUREMODIFIER_Scroll3,
	TEXTUREMODIFIER_Scroll4,
	TEXTUREMODIFIER_Scroll5,
	TEXTUREMODIFIER_Scroll6,
	TEXTUREMODIFIER_Scroll7,
	TEXTUREMODIFIER_Scroll8,
	TEXTUREMODIFIER_Scroll9,
	TEXTUREMODIFIER_Scroll10,
	TEXTUREMODIFIER_ScrollSlow1,
	TEXTUREMODIFIER_ScrollSlow2,
	TEXTUREMODIFIER_ScrollSlow3,
    TEXTUREMODIFIER_ScrollSlow4,
	TEXTUREMODIFIER_NormalMapping,
	TEXTUREMODIFIER_Displacement,

};

struct EXETextureHandle
{
	char				szName[64];
	char				szNameA[64];

	LPDIRECT3DTEXTURE9	pcD3DTexture;

	int					iWidth;
	int					iHeight;

	int					iUseCounter;

	int					iTransparency;

	BOOL				bSwap;
};

struct EXETextureMaterial
{
	BOOL				bInUse;
	int					iTextureCounter;
	struct EXETextureHandle  * psTexture[8];

	int					iaTextureStageState[8];
	int					iaTextureFormState[8];
	int					iReformTexture;

	BOOL				bAlpha;

	int					iTextureType;
	int					iBlendType;

	int					iShade;
	BOOL				bTwoSided;
	ID					iID;

	float				fDiffuseColorR;
	float				fDiffuseColorG;
	float				fDiffuseColorB;

	float				fOpacity;
	float				fSelfIllumination;

	int					iTextureSwap;
	int					iMaterialFrame;
	int					iTextureClip;

	int					iUseState;
	int					iMeshState;

	int					iWindMesh;

	void			  * psAnimatedTexture[32];
	int					iAnimatedTextureCounter;

	int					iFrameMask;
	int					iShiftFrameSpeed;
	int					iAnimationFrame;
};

struct EXETextureLink
{
	float				u[3];
	float				v[3];
	DWORD			  * hTexture;
	EXETextureLink	  * hNextTexture;
};

class EXETextureManager
{
private:
	IMPFNC				pfnReadTextures = 0x0048A9E0;
	IMPFNC				pfnConstructor = 0x0048A2D0;

public:
	DWORD				dwHeader;

	EXETextureMaterial* pcaTexture;
	int					iTextureCount;

	int					iReformTexture;
	int					iMaxTexture;

	int					iLastSearchTexture;
	char				szLastSearchTextureName[64];

	void				ReadTextures(int iMip = 0);

	EXETextureManager( int _iMaxTextures );
	EXETextureManager();

	EXETextureMaterial * GetTexture( UINT uIndex ) { return pcaTexture + uIndex; };
	UINT				LoadedTexture( std::string strFile );
	UINT				LoadTexture( std::string strFilePath );

	UINT				FreeTextureIndex();

	int					GetSaveSize();
	int					SaveFile( HANDLE hFile );

private:
	void				Constructor( int _iMaxTextures );

};

class EXETexture
{
public:

	IMPFNC				pfnLoadTextureDX9 = 0x0063C300;
	IMPFNC				pfnDeleteTexture = 0x004894B0;

	DWORD				dwCheckSum[5000];
	
	EXETextureHandle	saTexture[5000];

	CRITICAL_SECTION	cCriticalSection;

	int					iLastTextureCount;

	int					iTextureCount;

	static int			LoadTexture( EXETextureHandle * psTexture, bool BMipmap, bool bSwap );


	BOOL				ReadTextures();

	BOOL				ReadTexturesSafe();

	static BOOL WINAPI			ReadTexturesWorkerThread( void * ptr );

	EXETextureHandle	* Add( char * pszTexture, char * pszMask = NULL );

	int					GetTextureIndex( std::string strFile );
	EXETextureHandle	* GetTexture( int iIndex );
	
	BOOL				DeleteSafe( EXETextureHandle * ps );

	BOOL				Delete( EXETextureHandle * ps );

	int					GetCount() { return iTextureCount; }

	static EXETexture	* GetInstance() { return ((EXETexture*)0x03500CF8); }

	static bool			EncryptDecryptTextures( const std::string & strPath, bool bEncrypt );
	static bool			EncryptDecryptTexture( const std::string & strFile, const char * pszHash, bool bEncrypt );
	static bool			EncryptDecryptTextureBuffer( char * pBuffer, int iSize, const char * pszHash, bool bEncrypt );
	static const char *	GetHashTextureExtension( const std::string & strFile );

	static int			DecryptExternal( char * pBuffer, int iSize, const char * pszFileName );

private:
	static bool			DecryptBMP( char * pBuffer, int iSize );
	static bool			DecryptTGA( char * pBuffer, int iSize );
	static bool			DecryptJPG( char * pBuffer, int iSize );
	static bool			DecryptPNG( char * pBuffer, int iSize );

};