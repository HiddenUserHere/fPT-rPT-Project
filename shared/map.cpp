#include "stdafx.h"
#include "map.h"

void BaseMap::SetFileName( char * pszName, char * pszTitle )
{
#if defined(_GAME)
	StringCbPrintfA( szStageFilePath, 64, "field\\%s", pszName );
#else
	StringCbPrintfA( szStageFilePath, 64, "%s", pszName );
#endif
	StringCbPrintfA( szMiniMapNameFilePath, 64, "field\\title\\%st.tga", pszTitle );
	StringCbPrintfA( szMiniMapFilePath, 64, "field\\map\\%s.tga", pszTitle );
	iUnused = 1;
}

void BaseMap::AddBorder( BaseMap * pcMap, int iX, int iZ, int iY )
{
	if( iBorderCount >= MAX_BORDERMAP )
		return;
	saFieldBorder[iBorderCount].iX = iX;
	saFieldBorder[iBorderCount].iY = iY;
	saFieldBorder[iBorderCount].iZ = iZ;
	saFieldBorder[iBorderCount].pcMap = pcMap;
	iBorderCount++;

	if( pcMap->iBorderCount >= MAX_BORDERMAP )
		return;
	pcMap->saFieldBorder[pcMap->iBorderCount].iX = iX;
	pcMap->saFieldBorder[pcMap->iBorderCount].iY = 0;
	pcMap->saFieldBorder[pcMap->iBorderCount].iZ = iZ;
	pcMap->saFieldBorder[pcMap->iBorderCount].pcMap = this;
	pcMap->iBorderCount++;
}

void BaseMap::AddTeleport1( int iX, int iZ, int iY, int iSize, int iHeight, BaseMap * pcMap, int iXMap, int iZMap, int iYMap, int iLevel, int iWarpType )
{
	if( iWarpGateCount >= MAX_BORDERMAP )
		return;
	iWarpGateCurrent = iWarpGateCount;
	saWarpGate[iWarpGateCount].iX = iX << 8;
	saWarpGate[iWarpGateCount].iY = iY << 8;
	saWarpGate[iWarpGateCount].iZ = iZ << 8;
	saWarpGate[iWarpGateCount].iHeight = iHeight << 8;
	saWarpGate[iWarpGateCount].iSize = iSize << 8;
	saWarpGate[iWarpGateCurrent].iEffect = iWarpType;
	saWarpGate[iWarpGateCount].iLevel = iLevel;
	saWarpGate[iWarpGateCount].iOutFieldCount = 0;
	iWarpGateCount++;

	Teleport * psWarp = saWarpGate + iWarpGateCurrent;
	int iOut = psWarp->iOutFieldCount;

	if( psWarp->iOutFieldCount >= MAX_BORDERMAP )
		return;

	psWarp->saOutFieldBorder[iOut].pcMap = pcMap;
	psWarp->saOutFieldBorder[iOut].iX = iXMap << 8;
	psWarp->saOutFieldBorder[iOut].iY = iYMap << 8;
	psWarp->saOutFieldBorder[iOut].iZ = iZMap << 8;
	psWarp->iOutFieldCount++;

	if( pcMap == this )
	{
		sWarpGatePosition.iX = iXMap << 8;
		sWarpGatePosition.iY = iYMap << 8;
		sWarpGatePosition.iZ = iZMap << 8;
	}
}

void BaseMap::AddTeleport2( int iX, int iZ, int iY, int iSize, int iHeight, BaseMap * pcMap, int iXMap, int iZMap, int iYMap, int iXMap2, int iZMap2, int iYMap2, int iLevel, int iWarpType )
{
	if( iWarpGateCount >= MAX_BORDERMAP )
		return;

	iWarpGateCurrent = iWarpGateCount;
	saWarpGate[iWarpGateCount].iX = iX << 8;
	saWarpGate[iWarpGateCount].iY = iY << 8;
	saWarpGate[iWarpGateCount].iZ = iZ << 8;
	saWarpGate[iWarpGateCount].iHeight = iHeight << 8;
	saWarpGate[iWarpGateCount].iSize = iSize << 8;
	saWarpGate[iWarpGateCount].iEffect = iWarpType;
	saWarpGate[iWarpGateCount].iLevel = iLevel;
	iWarpGateCount++;

	Teleport * psWarp = saWarpGate + iWarpGateCurrent;
	int iOut = psWarp->iOutFieldCount;

	if( psWarp->iOutFieldCount >= MAX_BORDERMAP )
		return;

	psWarp->saOutFieldBorder[iOut].pcMap = pcMap;
	psWarp->saOutFieldBorder[iOut].iX = iXMap << 8;
	psWarp->saOutFieldBorder[iOut].iY = iYMap << 8;
	psWarp->saOutFieldBorder[iOut].iZ = iZMap << 8;
	psWarp->iOutFieldCount++;

	if( pcMap == this )
	{
		sWarpGatePosition.iX = iXMap << 8;
		sWarpGatePosition.iY = iYMap << 8;
		sWarpGatePosition.iZ = iZMap << 8;
	}

	iOut = psWarp->iOutFieldCount;

	if( psWarp->iOutFieldCount >= MAX_BORDERMAP )
		return;

	psWarp->saOutFieldBorder[iOut].pcMap = pcMap;
	psWarp->saOutFieldBorder[iOut].iX = iXMap2 << 8;
	psWarp->saOutFieldBorder[iOut].iY = iYMap2 << 8;
	psWarp->saOutFieldBorder[iOut].iZ = iZMap2 << 8;
	psWarp->iOutFieldCount++;
}

void BaseMap::AddSound( int iX, int iY, int iZ, int iRound, int iCode )
{
	if( iAmbientCount >= MAX_AMBIENTMAP )
		return;

	saAmbientPosition[iAmbientCount].iX = iX << 8;
	saAmbientPosition[iAmbientCount].iY = iY << 8;
	saAmbientPosition[iAmbientCount].iZ = iZ << 8;
	saAmbientPosition[iAmbientCount].iRound = iRound;
	saAmbientPosition[iAmbientCount].iNum = iCode;
	iAmbientCount++;
}

void BaseMap::AddStaticModel( char * pszName, int iAnimationBip )
{
	if( iObjectCount >= MAX_OBJECTMAP )
		return;

	StringCbPrintfA( szaObjectName[iObjectCount], 64, "%s", pszName );
	dwaObjectBip[iObjectCount] = iAnimationBip;
	iObjectCount++;
}

void BaseMap::AddModelEx( char * pszFormat, ... )
{
	if( iObjectCount >= MAX_OBJECTMAP )
		return;

	va_list vl;

	char szStr[128];

	va_start( vl, pszFormat );
	_vsnprintf_s( szStr, _countof( szStr ), _TRUNCATE, pszFormat, vl );
	va_end( vl );

	StringCbPrintfA( szaObjectName[iObjectCount], 64, "%s", szStr );
	dwaObjectBip[iObjectCount] = 0;
	iObjectCount++;
}

void BaseMap::SetCamera( int iX, int iY, int iZ, int iLeftX, int iRightX )
{
#if defined(_GAME)
	sActionCamera.sPosition.iX	= iX;
	sActionCamera.sPosition.iY	= iY;
	sActionCamera.sPosition.iZ	= iZ;
	sActionCamera.iLeftX		= iLeftX;
	sActionCamera.iRightX		= iRightX;
#endif
}

NAKED Map::Loot * Map::AddItem( Loot::LootItem * psLootItem, int iX, int iY, int iZ, BOOL bBegin )
{
	JMP( pfnAddItem );
}

BOOL Map::DeleteItem( Map::Loot * psLoot )
{
	for ( int i = 0; i < MAX_LOOT; i++ )
	{
		Map::Loot * ps = saLoot + i;

		if ( ps->bInUse && ps->sPosition == psLoot->sPosition && ps->sLootItem.sItem.sItemID.ToItemID() == psLoot->sLootItem.sItem.sItemID.ToItemID() )
		{
			ps->bInUse = FALSE;
			return TRUE;
		}
	}

	return FALSE;
}

NAKED UnitData * Map::CreateMonster( CharacterData * psCharacterData, int iCount )
{
	JMP( pfnCreateMonster );
}

NAKED void Map::AddUnitData( UnitData * pcUnitData )
{
	JMP( pfnAddUnitData );
}

Stage::Stage()
{
	Constructor1();
}

Stage::Stage( int iNumVertices, int iNumFaces )
{
	Constructor2( iNumVertices, iNumFaces );
}

Stage::~Stage()
{
	Deconstructor();
}

void Stage::Constructor1()
{
    sVectorLight = Point3D( 256, -256, 256 / 2 );

    iBright = 160;
    iContrast = 300;

    bState = FALSE;

    pLight = nullptr;
    iLightCount = 0;

    pdwAreaBuffer = nullptr;

    pTexture = nullptr;
    iTextureCount = 0;

    pFace = nullptr;
    iFaceCount = 0;

    pVertex = nullptr;
    iVertexCount = 0;

    pStageObject = nullptr;

    pMaterial = nullptr;

    iUnknown001 = 0;

#if defined(_GAME)
	fDissolveMapAmount = 1.0f;
#endif
}

void Stage::Constructor2( int iNumVertices, int iNumFaces )
{
    Constructor1();

	Init( iNumVertices, iNumFaces );
}

void Stage::Deconstructor()
{
    bState = 0;

    if ( iLightCount )
        DELET( pLight );

    if ( pdwAreaBuffer )
        DELET( pdwAreaBuffer );

    if ( iTextureCount )
        DELETA( pTexture );

    if ( iFaceCount )
        DELETA( pFace );

    if ( iVertexCount )
        DELETA( pVertex );

    if ( pStageObject )
        DELET( pStageObject );

    if ( pMaterial )
        DELET( pMaterial );

#if defined(_GAME)
	vNormalVertices.clear();
#endif
}


NAKED BOOL Stage::Init( int iNumVertices, int iNumFaces )
{
	JMP( pfnInit );
}

NAKED int Stage::GetHighestPoint( int iX, int iZ )
{
	JMP( pfnGetHighestPoint );
}

NAKED int Stage::GetHighestPoint( int iX, int iY, int iZ, int iMeshHeight )
{
	JMP( pfnGetHighestPointEx );
}

NAKED int Stage::GetPolygonHeight( StageFace * psFace, int iX, int iZ )
{
	JMP( pfnGetPolygonHeight );
}

NAKED int Stage::CanMoveToPosition( Point3D * psPosition, Point3D * psAngle, Point3D * psMovePosition, int iDistance, int iWidth, int iHeight, BOOL bCheckOverlap )
{
	JMP( pfnCanMoveToPosition );
}

NAKED int Stage::MakeFaceList( int iX, int iZ, int iWidth, int iHeight, int iTop, int iBottom )
{
	JMP( pfnMakeFaceList );
}

NAKED int Stage::MakeTriangleLineList( Point3D * psPosition, Point3D * psAngle, int iDistance, int iWidth, int iHeight, void * pLines3D, Point3D * ep )
{
	JMP( pfnMakeTriangleLineList );
}

NAKED int Stage::GetTriangleImpact( StageFace * pStageFace, void * pLines, int iLineCount )
{
	JMP( pfnGetTriangleImpact );
}

NAKED void Stage::Render( int iX, int iY, int iZ, int iAX, int iAY, int iAZ, EXEMatrixE * sRotationMatrix )
{
	JMP( 0x00487380 );
}

NAKED Stage * Stage::LoadStage( const char * pszFileName, Stage * pcStagePtr )
{
	JMP( pfnLoadStage );
}

StageDataOld::StageDataOld()
{
	Constructor1();
}

StageDataOld::~StageDataOld()
{
	Deconstructor();
}

void StageDataOld::Constructor1()
{
    sVectorLight = Point3D( 256, -256, 256 / 2 );

    iBright = 160;
    iContrast = 300;

    bState = FALSE;

    pLight = nullptr;
    iLightCount = 0;

	pdwAreaBuffer = nullptr;

    pTexture = nullptr;
    iTextureCount = 0;

    pFace = nullptr;
    iFaceCount = 0;
  
	pVertex = nullptr;
    iVertexCount = 0;
    
	pStageObject = nullptr;

	pMaterial = nullptr;

    iUnknown001 = 0;
}

void StageDataOld::Constructor2( int iNumVertices, int iNumFaces )
{
	Constructor1();
}

void StageDataOld::Deconstructor()
{
    bState = 0;

    if ( iLightCount )
        DELET( pLight );

    if ( pdwAreaBuffer )
        DELET( pdwAreaBuffer );

    if ( iTextureCount )
        DELETA( pTexture );

    if ( iFaceCount )
        DELETA( pFace );

    if ( iVertexCount )
        DELETA( pVertex );

    if ( pStageObject )
        DELET( pStageObject );

    if ( pMaterial )
        DELET( pMaterial );
}
