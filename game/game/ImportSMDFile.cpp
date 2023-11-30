#include "stdafx.h"
#include "ImportSMDFile.h"


namespace Import
{

SMDFile::SMDFile( const std::string & strFilePath ) : File(strFilePath)
{
}

SMDFile::~SMDFile()
{
	Unload();
}

bool SMDFile::Load()
{
	//Try to Open the File
	if( !Open() )
		return false;

	WRITEDBG( "SMDFile::Load() [%s]", strFilePath.c_str() );

	//Read Raw Header
	Read( &sRawHeader, sizeof( RawSMDHeader ), 1 );

	sModelVersion = UglyRefactor::PTModelVersion( sRawHeader.szHeader );

	//Model SMD?
	if( sModelVersion.eVersion != UglyRefactor::PTModelVersion::EPTModelVersion::PTMODELVERSION_Invalid )
	{
		eType = Model;

		//Load Model
		bLoaded = LoadModel();
	}
	else if( STRINGCOMPARE( sRawHeader.szHeader, szTerrainHeader1 ) )
	{
		eType = Terrain;

		//Load Terrain
		bLoaded = LoadTerrain( 2 );
	}
	else if( STRINGCOMPARE( sRawHeader.szHeader, szTerrainHeader2 ) )
	{
		eType = Terrain;

		//Load Terrain
		bLoaded = LoadTerrain( 2 );
	}
	else
	{
		WRITEDBG( "SMDFile::Load() does not recognize Header \"%s\"", sRawHeader.szHeader );
	}

	//Close the File
	Close();

	//Load OK
	return bLoaded;
}

void SMDFile::Unload()
{
	if( !bLoaded )
		return;

	WRITEDBG( "SMDFile::Unload()" );

	//Model SMD?
	if( STRINGCOMPARE( sRawHeader.szHeader, "¥‚¬X.˜)Œ…Ÿ.È„.tŒ.B.¨.„ª" ) )
	{
		//Unload Model
		UnloadModel();
	}
	else if( STRINGCOMPARE( sRawHeader.szHeader, "SMD Stage data Ver 0.72" ) )
	{
		//Unload Terrain
		UnloadTerrain();
	}

	//Set Unloaded
	bLoaded = false;
}

BOOL SMDFile::ToStage( RawSMDTerrain * pcThis )
{


	return TRUE;
}

const char * SMDFile::TypeToString( EType eType )
{
	switch( eType )
	{
	case Model:
		return "Model";
	case Terrain:
		return "Terrain";
	}

	return "Unknown";
}

bool SMDFile::LoadModel()
{
	WRITEDBG( "SMDFile::LoadModel()" );

	//Read Object Info's
	psaRawObjectInfo = new RawSMDObjectInfo[sRawHeader.iObjectCount];
	Read( psaRawObjectInfo, sizeof( RawSMDObjectInfo ), sRawHeader.iObjectCount );

	//Load Materials
	if( sRawHeader.iMaterialCount > 0 )
		LoadMaterials();

	//Read Objects
	psaRawObject = new RawSMDObject[sRawHeader.iObjectCount];
	for( int i = 0; i < sRawHeader.iObjectCount; i++ )
	{
		//Seek File to Object
		Seek( psaRawObjectInfo[i].iFilePointerToObject );

		//Read Raw Object
		Read( psaRawObject + i, 0x08BC, 1 );

		if ( sModelVersion.HasInfluences() )
		{
			memcpy( psaRawObject[i].sTransformationMatrixF.f, psaRawObject[i].sMatrixTM.i, sizeof( float ) * 16 );

			for ( int i = 0; i < 4; i++ )
				for ( int j = 0; j < 4; j++ )
					psaRawObject[i].sMatrixTM.i[i][j] = (int)(psaRawObject[i].sTransformationMatrixF.m[i][j] * 256.0f);
		}

		//Remember Old Texture Link Pointer
		RawSMDTextureLink * paOldTextureLink = psaRawObject[i].paTextureLink;

		if ( sModelVersion.IsEncrypted() )
		{
			//Read Faces
			psaRawObject[i].paFace = new RawSMDFace[psaRawObject[i].iFaceCount];
			Read( psaRawObject[i].paFace, sizeof( RawSMDFace ), psaRawObject[i].iFaceCount );

			//Read Vertices
			if ( sModelVersion.HasInfluences() )
			{
				if ( psaRawObject[i].iVertexCount > 0 )
					psaRawObject[i].vFloatVertices.resize( psaRawObject[i].iVertexCount );

				Read( (void *)psaRawObject[i].vFloatVertices.data(), sizeof( EXEVertexTangentF ), psaRawObject[i].iVertexCount );
			}
			else
			{
				psaRawObject[i].paVertex = new RawSMDVertex[psaRawObject[i].iVertexCount];
				Read( psaRawObject[i].paVertex, sizeof( RawSMDVertex ), psaRawObject[i].iVertexCount );
			}

			//Read Texture Links
			psaRawObject[i].paTextureLink = new RawSMDTextureLink[psaRawObject[i].iTextureLinkCount];
			Read( psaRawObject[i].paTextureLink, sizeof( RawSMDTextureLink ), psaRawObject[i].iTextureLinkCount );

			//Read Key Positions
			psaRawObject[i].paKeyPosition = new RawSMDKeyPosition[psaRawObject[i].iKeyPositionCount];
			Read( psaRawObject[i].paKeyPosition, sizeof( RawSMDKeyPosition ), psaRawObject[i].iKeyPositionCount );

			//Read Key Rotations
			psaRawObject[i].paKeyRotation = new RawSMDKeyRotation[psaRawObject[i].iKeyRotationCount];
			Read( psaRawObject[i].paKeyRotation, sizeof( RawSMDKeyRotation ), psaRawObject[i].iKeyRotationCount );

			//Read Rotations
			psaRawObject[i].paRotation = new RawSMDMatrixF[psaRawObject[i].iKeyRotationCount];
			Read( psaRawObject[i].paRotation, sizeof( RawSMDMatrixF ), psaRawObject[i].iKeyRotationCount );

			//Read Key Scalings
			psaRawObject[i].paKeyScale = new RawSMDKeyScale[psaRawObject[i].iKeyScaleCount];
			Read( psaRawObject[i].paKeyScale, sizeof( RawSMDKeyScale ), psaRawObject[i].iKeyScaleCount );
		}
		else
		{
			//Read Vertices
			if ( sModelVersion.HasInfluences() )
			{
				if ( psaRawObject[i].iVertexCount > 0 )
					psaRawObject[i].vFloatVertices.resize( psaRawObject[i].iVertexCount );

				Read( (void*)psaRawObject[i].vFloatVertices.data(), sizeof( EXEVertexTangentF ), psaRawObject[i].iVertexCount );
			}
			else
			{
				psaRawObject[i].paVertex = new RawSMDVertex[psaRawObject[i].iVertexCount];
				Read( psaRawObject[i].paVertex, sizeof( RawSMDVertex ), psaRawObject[i].iVertexCount );
			}

			//Read Faces
			psaRawObject[i].paFace = new RawSMDFace[psaRawObject[i].iFaceCount];
			Read( psaRawObject[i].paFace, sizeof( RawSMDFace ), psaRawObject[i].iFaceCount );

			//Read Texture Links
			psaRawObject[i].paTextureLink = new RawSMDTextureLink[psaRawObject[i].iTextureLinkCount];
			Read( psaRawObject[i].paTextureLink, sizeof( RawSMDTextureLink ), psaRawObject[i].iTextureLinkCount );

			//Read Key Rotations
			psaRawObject[i].paKeyRotation = new RawSMDKeyRotation[psaRawObject[i].iKeyRotationCount];
			Read( psaRawObject[i].paKeyRotation, sizeof( RawSMDKeyRotation ), psaRawObject[i].iKeyRotationCount );

			//Read Key Positions
			psaRawObject[i].paKeyPosition = new RawSMDKeyPosition[psaRawObject[i].iKeyPositionCount];
			Read( psaRawObject[i].paKeyPosition, sizeof( RawSMDKeyPosition ), psaRawObject[i].iKeyPositionCount );

			//Read Key Scalings
			psaRawObject[i].paKeyScale = new RawSMDKeyScale[psaRawObject[i].iKeyScaleCount];
			Read( psaRawObject[i].paKeyScale, sizeof( RawSMDKeyScale ), psaRawObject[i].iKeyScaleCount );

			//Read Rotations
			psaRawObject[i].paRotation = new RawSMDMatrixF[psaRawObject[i].iKeyRotationCount];
			Read( psaRawObject[i].paRotation, sizeof( RawSMDMatrixF ), psaRawObject[i].iKeyRotationCount );
		}

		//Print Frames
		//WRITEERR( "Frames %d", psaRawObject[i].iFrameCount );
		//for( int j = 0; j < psaRawObject[i].iFrameCount; j++ )
		//{
		//	WRITEERR( "Frame[%d]: Rotation(%d)(%d)(%d)(%d)", j, psaRawObject[i].aRotationFrame[j].iStartFrame, psaRawObject[i].aRotationFrame[j].iEndFrame, psaRawObject[i].aRotationFrame[j].iCount, psaRawObject[i].aRotationFrame[j].iIndex );
		//	WRITEERR( "Frame[%d]: Translation(%d)(%d)(%d)(%d)", j, psaRawObject[i].aTranslationFrame[j].iStartFrame, psaRawObject[i].aTranslationFrame[j].iEndFrame, psaRawObject[i].aTranslationFrame[j].iCount, psaRawObject[i].aTranslationFrame[j].iIndex );
		//	WRITEERR( "Frame[%d]: Scaling(%d)(%d)(%d)(%d)", j, psaRawObject[i].aScalingFrame[j].iStartFrame, psaRawObject[i].aScalingFrame[j].iEndFrame, psaRawObject[i].aScalingFrame[j].iCount, psaRawObject[i].aScalingFrame[j].iIndex );
		//}

		//Read Physique
		if( psaRawObject[i].paPhysique )
		{
			if( (psaRawObject[i].szObjectName[0] == 'B') && 
				(psaRawObject[i].szObjectName[1] == 'i') && 
				(psaRawObject[i].szObjectName[2] == 'p') )
			{
				//Shouldn't have Physique if this is a Bone
				psaRawObject[i].paPhysique = NULL;
			}
			else
			{
				psaRawObject[i].paPhysique = new RawSMDVertexPhysique[psaRawObject[i].iVertexCount];
				Read( psaRawObject[i].paPhysique, sizeof( RawSMDVertexPhysique ), psaRawObject[i].iVertexCount );

				//for( int z = 0; z < psaRawObject[i].iVertexCount; z++ )
				//	WRITEERR( "Vertex[%d] Physique: %s", z, psaRawObject[i].paPhysique[z].szBaseName );
			}
		}

		//Fix Textures Links
		{
			//Fix Linked Links
			for( int j = 0; j < psaRawObject[i].iTextureLinkCount; j++ )
			{
				//Has Link?
				if( psaRawObject[i].paTextureLink[j].pNext )
				{
					//Fix Link
					psaRawObject[i].paTextureLink[j].pNext = psaRawObject[i].paTextureLink + (psaRawObject[i].paTextureLink[j].pNext - paOldTextureLink);
				}
			}
			
			//Fix Root Links
			for( int j = 0; j < psaRawObject[i].iFaceCount; j++ )
			{
				//Has Link?
				if( psaRawObject[i].paFace[j].pTextureLink )
				{
					//Fix Link
					psaRawObject[i].paFace[j].pTextureLink = psaRawObject[i].paTextureLink + (psaRawObject[i].paFace[j].pTextureLink - paOldTextureLink);
				}
			}
		}

		if ( sModelVersion.HasInfluences() )
		{
			psaRawObject[i].bUseInfluences = TRUE;
			psaRawObject[i].vInfluences.resize( psaRawObject[i].iVertexCount );
			Read( psaRawObject[i].vInfluences.data(), sizeof( EXEMeshInfluenceVertex ), psaRawObject[i].iVertexCount );
		}
	}

	return true;
}

void SMDFile::UnloadModel()
{
	//Cleanup Objects
	for( int i = 0; i < sRawHeader.iObjectCount; i++ )
	{
		delete[] psaRawObject[i].paRotation;
		delete[] psaRawObject[i].paKeyScale;
		delete[] psaRawObject[i].paKeyPosition;
		delete[] psaRawObject[i].paKeyRotation;
		delete[] psaRawObject[i].paPhysique;
		delete[] psaRawObject[i].paTextureLink;
		delete[] psaRawObject[i].paFace;
		delete[] psaRawObject[i].paVertex;
	}
	delete[] psaRawObject;

	//Cleanup Object Info's
	delete[] psaRawObjectInfo;

	//Cleanup Materials
	if( sRawHeader.iMaterialCount > 0 )
		UnloadMaterials();
}

bool SMDFile::LoadTerrain( int iTerrainVersion )
{
	WRITEDBG( "SMDFile::LoadTerrain()" );

	this->iTerrainVersion = iTerrainVersion;

	if ( iTerrainVersion == 3 || iTerrainVersion == 4 )
	{
		//Get File Size
		long lSize		= Size();
		char * pbBuffer = new char[lSize];

		//Read Blocks
		int iTotalBlocks = (lSize / 4096);

		if ( iTotalBlocks == 0 )
		{
			Read( pbBuffer, lSize, 1 );
		}
		else
		{
			//Compute Blocks size
			int iSizeBlocks = (iTotalBlocks * 4096);

			Read( pbBuffer, 4096, iTotalBlocks );

			//Still need Read?
			if ( iSizeBlocks < lSize )
				Read( pbBuffer + iSizeBlocks, (lSize - iSizeBlocks), 1 );
		}

		//Set New Buffer
		SetBuffer( pbBuffer );
	}

	//Read Terrain
	psRawTerrain = new RawSMDTerrain;
	Read( psRawTerrain, sizeof( RawSMDTerrain ), 1 );

	//Load Materials
	if( sRawHeader.iMaterialCount > 0 )
		LoadMaterials();

	//Remember Old Texture Link Pointer
	RawSMDTextureLink * paOldTextureLink = psRawTerrain->paTextureLink;

	//Read Vertices
	if( psRawTerrain->iVertexCount > 0 )
	{
		psRawTerrain->paVertex = new RawSMDTerrainVertex[psRawTerrain->iVertexCount];
		Read( psRawTerrain->paVertex, sizeof( RawSMDTerrainVertex ), psRawTerrain->iVertexCount );
	}

	//Read Faces
	if( psRawTerrain->iFaceCount > 0 )
	{
		psRawTerrain->paFace = new RawSMDTerrainFace[psRawTerrain->iFaceCount];
		Read( psRawTerrain->paFace, sizeof( RawSMDTerrainFace ), psRawTerrain->iFaceCount );
	}

	//Read Texture Links
	if( psRawTerrain->iTextureLinkCount > 0 )
	{
		psRawTerrain->paTextureLink = new RawSMDTextureLink[psRawTerrain->iTextureLinkCount];
		Read( psRawTerrain->paTextureLink, sizeof( RawSMDTextureLink ), psRawTerrain->iTextureLinkCount );
	}

	//Read Lights
	if( psRawTerrain->iLightCount > 0 )
	{
		psRawTerrain->paLight = new RawSMDLight[psRawTerrain->iLightCount];
		Read( psRawTerrain->paLight, sizeof( RawSMDLight ), psRawTerrain->iLightCount );
	}

	//Fix Textures Links
	{
		//Fix Linked Links
		for( int j = 0; j < psRawTerrain->iTextureLinkCount; j++ )
		{
			//Has Link?
			if( psRawTerrain->paTextureLink[j].pNext )
			{
				//Fix Link
				psRawTerrain->paTextureLink[j].pNext = psRawTerrain->paTextureLink + (psRawTerrain->paTextureLink[j].pNext - paOldTextureLink);
			}
		}
			
		//Fix Root Links
		for( int j = 0; j < psRawTerrain->iFaceCount; j++ )
		{
			//Has Link?
			if( psRawTerrain->paFace[j].pTextureLink )
			{
				//Fix Link
				psRawTerrain->paFace[j].pTextureLink = psRawTerrain->paTextureLink + (psRawTerrain->paFace[j].pTextureLink - paOldTextureLink);
			}
		}
	}

	//Read Area
	psRawTerrain->pAreaBuffer = new DWORD[psRawTerrain->iAreaSize];

	int iTotalLength = 0;
	for( int i = 0; i < 256; i++ )
	{
		for( int j = 0; j < 256; j++ )
		{
			if( psRawTerrain->aaArea[j][i] )
			{
				int iLength = 0;
				Read( &iLength, sizeof( int ), 1 );

				psRawTerrain->aaArea[j][i] = psRawTerrain->pAreaBuffer + iTotalLength;
				
				if( iTerrainVersion == 2 || iTerrainVersion == 4 )
				{
					Read( psRawTerrain->aaArea[j][i], sizeof( DWORD ), iLength );
				}
				else
				{
					for( int k = 0; k < iLength; k++ )
					{
						WORD wTemp;
						Read( &wTemp, sizeof( WORD ), 1 );

						psRawTerrain->aaArea[j][i][k] = wTemp;
					}
				}

				iTotalLength += iLength;
			}
		}
	}

	if ( iTerrainVersion == 3 || iTerrainVersion == 4 )
		delete[] GetBuffer();

	return true;
}

void SMDFile::UnloadTerrain()
{
	//Clean up Terrain Buffers
	delete[] psRawTerrain->pAreaBuffer;
	delete[] psRawTerrain->paLight;
	delete[] psRawTerrain->paTextureLink;
	delete[] psRawTerrain->paFace;
	delete[] psRawTerrain->paVertex;

	//Cleanup Terrain
	delete psRawTerrain;

	//Cleanup Materials
	if( sRawHeader.iMaterialCount > 0 )
		UnloadMaterials();
}

void SMDFile::LoadMaterials( BOOL bUse )
{
	//Read Material Header
	Read( &sRawMaterialHeader, sizeof( RawSMDMaterialHeader ), 1 );

	//Read Materials
	sRawMaterialHeader.paRawSMDMaterial = new RawSMDMaterial[sRawMaterialHeader.uMaterialCount];
	for( UINT u = 0; u < sRawMaterialHeader.uMaterialCount; u++ )
	{
		Read( sRawMaterialHeader.paRawSMDMaterial + u, sizeof( RawSMDMaterial ), 1 );

		if( sRawMaterialHeader.paRawSMDMaterial[u].iInUse )
		{
			int iAppendedBufferSize = 0;
			Read( &iAppendedBufferSize, sizeof( int ), 1 );

			char * pszAdditionalBuffer = new char[iAppendedBufferSize];
			Read( pszAdditionalBuffer, 1, iAppendedBufferSize );
			long lAdditionalBufferPointer = 0;

			for( UINT v = 0; v < sRawMaterialHeader.paRawSMDMaterial[u].uTextureCount; v++ )
			{
				sRawMaterialHeader.paRawSMDMaterial[u].paTexture[v] = new RawSMDMaterial::Texture();

				STRINGCOPY( sRawMaterialHeader.paRawSMDMaterial[u].paTexture[v]->szTextureFilePath, pszAdditionalBuffer + lAdditionalBufferPointer );

				lAdditionalBufferPointer += lstrlenA( pszAdditionalBuffer + lAdditionalBufferPointer ) + 1;
				lAdditionalBufferPointer += lstrlenA( pszAdditionalBuffer + lAdditionalBufferPointer ) + 1;
			}
		
			lAdditionalBufferPointer = 0;
			for( UINT v = 0; v < sRawMaterialHeader.paRawSMDMaterial[u].uAnimatedTextureCount; v++ )
			{
				sRawMaterialHeader.paRawSMDMaterial[u].paAnimatedTexture[v] = new RawSMDMaterial::Texture();

				STRINGCOPY( sRawMaterialHeader.paRawSMDMaterial[u].paAnimatedTexture[v]->szTextureFilePath, pszAdditionalBuffer + lAdditionalBufferPointer );

				lAdditionalBufferPointer += lstrlenA( pszAdditionalBuffer + lAdditionalBufferPointer ) + 1;
				lAdditionalBufferPointer += lstrlenA( pszAdditionalBuffer + lAdditionalBufferPointer ) + 1;
			}

			delete[] pszAdditionalBuffer;
		}
	}

	//Load if use
	if ( bUse )
	{

	}
}

void SMDFile::UnloadMaterials()
{
	for( UINT u = 0; u < sRawMaterialHeader.uMaterialCount; u++ )
	{
		for( UINT v = 0; v < sRawMaterialHeader.paRawSMDMaterial[u].uTextureCount; v++ )
			delete sRawMaterialHeader.paRawSMDMaterial[u].paTexture[v];
		
		for( UINT v = 0; v < sRawMaterialHeader.paRawSMDMaterial[u].uAnimatedTextureCount; v++ )
			delete sRawMaterialHeader.paRawSMDMaterial[u].paAnimatedTexture[v];
	}
	delete[] sRawMaterialHeader.paRawSMDMaterial;
}

void SMDFile::SetBuffer( char * pbNewBuffer )
{
	pbBuffer		= pbNewBuffer;
	iCurrentBuffer	= 0;
}

void SMDFile::ReadBuffer( void * pBuffer, int iSize, int iCount )
{
	for ( int i = 0; i < iCount; i++ )
	{
		CopyMemory( pBuffer, pbBuffer + iCurrentBuffer,  iSize );
		
		iCurrentBuffer += iSize;
	}
}

void SMDFile::Read( void * pBuffer, size_t iElementSize, size_t iCount )
{
	if ( pbBuffer )
		ReadBuffer( pBuffer, iElementSize, iCount );
	else
		File::Read( pBuffer, iElementSize, iCount );
}

}
