#pragma once

struct DXImageSceneData
{
	int					iSize;

	BYTE				* baData;
};

struct DXImageSceneHeader
{
	char				szHeader[32]; //Scene Data 0.1
	INT64				iFileSize;
	int					iBlockCount;
};

namespace DX
{

class DXImageSceneRenderer
{
private:
	std::string				strFileName;

	DXImageSceneHeader		sHeader;

	int						iBlockWheel;

	int						iFramesPerSecond;

	int						iWidth, iHeight;

	std::vector<LPDIRECT3DTEXTURE9>		vTextures;

	LPD3DXSPRITE			lpSprite;

	int						GetFileSize( FILE * fp );

	void					SetNextTexture( DXImageSceneData * ps );

public:
	DXImageSceneRenderer( std::string _strFileName );
	virtual ~DXImageSceneRenderer();

	//Setters

	BOOL					Create( const char * pszDir, const char * pszPrefix, int iCount );

	BOOL					Load();

	void					Update( float fTime );

	void					Render();

	void					Close();
};

};