#include "stdafx.h"
#include "ExportSMDFile.h"


namespace Export
{

SMDFile::SMDFile( const std::string & strFilePath ) : File(strFilePath)
{
}

bool SMDFile::SaveModel( const RawSMDHeader * psRawHeader, const RawSMDMaterialHeader * psRawMaterialHeader, const RawSMDObject * psaRawObject, UglyRefactor::PTModelVersion & sModelVersion )
{
	//Try to Open the File for Writing
	if( !Open() )
		return false;

	WRITEDBG( "SMDFile::SaveModel() [%s]", strFilePath.c_str() );

	//Build Header
	RawSMDHeader sNewRawHeader;
	memcpy( &sNewRawHeader, psRawHeader, sizeof( RawSMDHeader ) );

	//Set Header
	memcpy( sNewRawHeader.szHeader, sModelVersion.GetStringVersion().c_str(), sModelVersion.GetStringVersion().length() );

	//Fix Header Values
	sNewRawHeader.iFilePointerToMaterials = 0;
	sNewRawHeader.iFilePointerToFirstObject = 0;

	//Fix Header Frames
	for( int i = 0; i < _countof( sNewRawHeader.aFrame ); i++ )
	{
		if( i >= sNewRawHeader.iFrameCount )
			memset( sNewRawHeader.aFrame + i, 0, sizeof( sNewRawHeader.aFrame[i] ) );
	}

	//Write Header
	Write( &sNewRawHeader, sizeof( RawSMDHeader ), 1 );

	//Any Objects?
	if( psRawHeader->iObjectCount > 0 )
	{
		//Compute Materials Block Size
		unsigned int iMaterialsBlockSize = 0;

		//Any Materials?
		if( psRawHeader->iMaterialCount > 0 )
			iMaterialsBlockSize = ComputeMaterialsBlockSize( psRawMaterialHeader );

		//Build Object Info List
		RawSMDObjectInfo * psaRawObjectInfo = new RawSMDObjectInfo[psRawHeader->iObjectCount];
		ZeroMemory( psaRawObjectInfo, sizeof( RawSMDObjectInfo ) * psRawHeader->iObjectCount );

		//Fill Object Info List
		unsigned int iFilePointerToObject = sizeof( RawSMDHeader ) + (sizeof( RawSMDObjectInfo ) * psRawHeader->iObjectCount) + iMaterialsBlockSize;
		for( int i = 0; i < psRawHeader->iObjectCount; i++ )
		{
			RawSMDObjectInfo * psRawObjectInfo = psaRawObjectInfo + i;
			const RawSMDObject * psRawObject = psaRawObject + i;

			//Fill Object Info Data
			STRINGCOPY( psRawObjectInfo->szObjectName, psRawObject->szObjectName );
			psRawObjectInfo->iLength = ComputeObjectBlockSize( psRawObject, sModelVersion );
			psRawObjectInfo->iFilePointerToObject = iFilePointerToObject;

			//Increase File Pointer with Computed Block Size of this Object
			iFilePointerToObject += psRawObjectInfo->iLength;
		}

		//Write Object Info List
		Write( psaRawObjectInfo, sizeof( RawSMDObjectInfo ), psRawHeader->iObjectCount );

		//Delete Object Info List
		delete[] psaRawObjectInfo;
	}

	//Save Materials
	if( psRawHeader->iMaterialCount > 0 )
		SaveMaterials( psRawMaterialHeader );

	//Save Objects
	for( int i = 0; i < psRawHeader->iObjectCount; i++ )
		SaveObject( psaRawObject + i, sModelVersion );

	//Close the File
	Close();

	//Save OK
	return true;
}

bool SMDFile::SaveTerrain( const RawSMDHeader * psRawHeader, const RawSMDMaterialHeader * psRawMaterialHeader, const RawSMDTerrain * psRawTerrain )
{
	//Try to Open the File for Writing
	if( !Open() )
		return false;

	WRITEDBG( "SMDFile::SaveTerrain() [%s]", strFilePath.c_str() );

	//Build Header
	RawSMDHeader sNewRawHeader;
	memcpy( &sNewRawHeader, psRawHeader, sizeof( RawSMDHeader ) );

	//Set Header
	memcpy( sNewRawHeader.szHeader, szTerrainHeader2, 24 );

	//Fix Header Values
	sNewRawHeader.iFilePointerToMaterials = 0;
	sNewRawHeader.iFilePointerToFirstObject = 0;

	//Fix Header Frames
	for( int i = 0; i < _countof( sNewRawHeader.aFrame ); i++ )
	{
		if( i >= sNewRawHeader.iFrameCount )
			memset( sNewRawHeader.aFrame + i, 0, sizeof( sNewRawHeader.aFrame[i] ) );
	}

	//Write Header
	Write( &sNewRawHeader, sizeof( RawSMDHeader ), 1 );

	//Save Terrain
	Write( (void*)psRawTerrain, sizeof( RawSMDTerrain ), 1 );
	
	//Save Materials
	if( psRawHeader->iMaterialCount > 0 )
		SaveMaterials( psRawMaterialHeader );
	
	//Save Vertices
	if( psRawTerrain->iVertexCount > 0 )
		Write( psRawTerrain->paVertex, sizeof( RawSMDTerrainVertex ), psRawTerrain->iVertexCount );
	
	//Save Faces
	if( psRawTerrain->iFaceCount > 0 )
		Write( psRawTerrain->paFace, sizeof( RawSMDTerrainFace ), psRawTerrain->iFaceCount );
	
	//Save Texture Links
	if( psRawTerrain->iTextureLinkCount > 0 )
		Write( psRawTerrain->paTextureLink, sizeof( RawSMDTextureLink ), psRawTerrain->iTextureLinkCount );
	
	//Save Lights
	if( psRawTerrain->iLightCount > 0 )
		Write( psRawTerrain->paLight, sizeof( RawSMDLight ), psRawTerrain->iLightCount );

	//Save Area
	for( int i = 0; i < 256; i++ )
	{
		for( int j = 0; j < 256; j++ )
		{
			if( psRawTerrain->aaArea[j][i] )
			{
				int iLength = (psRawTerrain->aaArea[j][i][0] + 1);
				Write( &iLength, sizeof( int ), 1 );
				Write( psRawTerrain->aaArea[j][i], sizeof( DWORD ), iLength );
			}
		}
	}

	//Close the File
	Close();

	//Save OK
	return true;
}

unsigned int SMDFile::ComputeMaterialsBlockSize( const RawSMDMaterialHeader * psRawMaterialHeader ) const
{
	unsigned int uSize = 0;

	//Header Size
	uSize += sizeof( RawSMDMaterialHeader );

	//Material Sizes
	for( unsigned int i = 0; i < psRawMaterialHeader->uMaterialCount; i++ )
	{
		RawSMDMaterial * psRawMaterial = psRawMaterialHeader->paRawSMDMaterial + i;

		//Material Size
		uSize += sizeof( RawSMDMaterial );

		//In Use?
		if( psRawMaterial->iInUse )
		{
			//Size of Appended Buffer Size integer
			uSize += sizeof( int );

			//Size of Texture File Paths
			for( unsigned int z = 0; z < psRawMaterial->uTextureCount; z++ )
			{
				uSize += lstrlenA( psRawMaterial->paTexture[z]->szTextureFilePath ) + 1;
				uSize += 1;
			}

			//Size of Animated Texture File Paths
			for( unsigned int z = 0; z < psRawMaterial->uAnimatedTextureCount; z++ )
			{
				uSize += lstrlenA( psRawMaterial->paAnimatedTexture[z]->szTextureFilePath ) + 1;
				uSize += 1;
			}
		}
	}

	return uSize;
}

void SMDFile::SaveMaterials( const RawSMDMaterialHeader * psRawMaterialHeader )
{
	//Header
	Write( (void*)psRawMaterialHeader, sizeof( RawSMDMaterialHeader ), 1 );

	//Materials
	for( unsigned int i = 0; i < psRawMaterialHeader->uMaterialCount; i++ )
	{
		RawSMDMaterial * psRawMaterial = psRawMaterialHeader->paRawSMDMaterial + i;

		//Material
		Write( psRawMaterial, sizeof( RawSMDMaterial ), 1 );

		//In Use?
		if( psRawMaterial->iInUse )
		{
			int iSize = 0;

			//Size of Texture File Paths
			for( unsigned int z = 0; z < psRawMaterial->uTextureCount; z++ )
			{
				iSize += lstrlenA( psRawMaterial->paTexture[z]->szTextureFilePath ) + 1;
				iSize += 1;
			}

			//Size of Animated Texture File Paths
			for( unsigned int z = 0; z < psRawMaterial->uAnimatedTextureCount; z++ )
			{
				iSize += lstrlenA( psRawMaterial->paAnimatedTexture[z]->szTextureFilePath ) + 1;
				iSize += 1;
			}

			//Appended Buffer Size integer
			Write( &iSize, sizeof( int ), 1 );

			//Texture File Paths
			for( unsigned int z = 0; z < psRawMaterial->uTextureCount; z++ )
			{
				char szNull[2] = { 0 };

				Write( psRawMaterial->paTexture[z]->szTextureFilePath, 1, lstrlenA( psRawMaterial->paTexture[z]->szTextureFilePath ) );
				Write( szNull, 1, 2 );
			}
			
			//Animated Texture File Paths
			for( unsigned int z = 0; z < psRawMaterial->uAnimatedTextureCount; z++ )
			{
				char szNull[2] = { 0 };

				Write( psRawMaterial->paAnimatedTexture[z]->szTextureFilePath, 1, lstrlenA( psRawMaterial->paAnimatedTexture[z]->szTextureFilePath ) );
				Write( szNull, 1, 2 );
			}
		}
	}
}

unsigned int SMDFile::ComputeObjectBlockSize( const RawSMDObject * psRawObject, UglyRefactor::PTModelVersion & sModelVersion ) const
{
	unsigned int uSize = 0;

	//Object
	uSize += 0x8BC;

	//Vertices
	uSize += sizeof( RawSMDVertex ) * psRawObject->iVertexCount;

	//Faces
	uSize += sizeof( RawSMDFace ) * psRawObject->iFaceCount;

	//Texture Links
	uSize += sizeof( RawSMDTextureLink ) * psRawObject->iTextureLinkCount;

	//Key Rotations
	uSize += sizeof( RawSMDKeyRotation ) * psRawObject->iKeyRotationCount;

	//Key Positions
	uSize += sizeof( RawSMDKeyPosition ) * psRawObject->iKeyPositionCount;

	//Key Scales
	uSize += sizeof( RawSMDKeyScale ) * psRawObject->iKeyScaleCount;
	
	//Key Rotation Matrices
	uSize += sizeof( RawSMDMatrixF ) * psRawObject->iKeyRotationCount;

	//Physique
	if( psRawObject->paPhysique )
		uSize += sizeof( RawSMDVertexPhysique ) * psRawObject->iVertexCount;

	if ( psRawObject->bUseInfluences )
		uSize += (UINT)psRawObject->vInfluences.size() * sizeof( EXEMeshInfluenceVertex );

	if ( psRawObject->vFloatVertices.size() > 0 )
	{
		uSize -= sizeof( EXEVertex ) * psRawObject->iVertexCount;

		uSize += sizeof( EXEVertexTangentF ) * psRawObject->vFloatVertices.size();
	}

	return uSize;
}

void SMDFile::SaveObject( const RawSMDObject * psRawObject, UglyRefactor::PTModelVersion & sModelVersion )
{
	//Object
	if ( sModelVersion.HasInfluences() )
		memcpy( (void*)psRawObject->sMatrixTM.i, psRawObject->sTransformationMatrixF.f, sizeof( float ) * 16 );

	Write( (void*)psRawObject, 0x8BC, 1 );

	if ( sModelVersion.IsEncrypted() )
	{
		//Faces
		if ( psRawObject->iFaceCount > 0 )
			Write( psRawObject->paFace, sizeof( RawSMDFace ), psRawObject->iFaceCount );

		//Vertices
		if ( psRawObject->iVertexCount > 0 )
		{
			if ( sModelVersion.HasInfluences() )
				Write( (void*)psRawObject->vFloatVertices.data(), sizeof( EXEVertexTangentF ), psRawObject->iVertexCount );
			else
				Write( psRawObject->paVertex, sizeof( RawSMDVertex ), psRawObject->iVertexCount );
		}

		//Texture Links
		if ( psRawObject->iTextureLinkCount > 0 )
			Write( psRawObject->paTextureLink, sizeof( RawSMDTextureLink ), psRawObject->iTextureLinkCount );

		//Key Positions
		if ( psRawObject->iKeyPositionCount > 0 )
			Write( psRawObject->paKeyPosition, sizeof( RawSMDKeyPosition ), psRawObject->iKeyPositionCount );

		//Key Rotations
		if ( psRawObject->iKeyRotationCount > 0 )
			Write( psRawObject->paKeyRotation, sizeof( RawSMDKeyRotation ), psRawObject->iKeyRotationCount );

		//Key Rotation Matrices
		if ( psRawObject->iKeyRotationCount > 0 )
			Write( psRawObject->paRotation, sizeof( RawSMDMatrixF ), psRawObject->iKeyRotationCount );

		//Key Scales
		if ( psRawObject->iKeyScaleCount > 0 )
			Write( psRawObject->paKeyScale, sizeof( RawSMDKeyScale ), psRawObject->iKeyScaleCount );

		//Physique
		if ( (psRawObject->paPhysique) && (psRawObject->iVertexCount > 0) )
			Write( psRawObject->paPhysique, sizeof( RawSMDVertexPhysique ), psRawObject->iVertexCount );
	}
	else
	{
		//Vertices
		if ( psRawObject->iVertexCount > 0 )
		{
			if ( sModelVersion.HasInfluences() )
				Write( (void *)psRawObject->vFloatVertices.data(), sizeof( EXEVertexTangentF ), psRawObject->iVertexCount );
			else
				Write( psRawObject->paVertex, sizeof( RawSMDVertex ), psRawObject->iVertexCount );
		}
		
		//Faces
		if ( psRawObject->iFaceCount > 0 )
			Write( psRawObject->paFace, sizeof( RawSMDFace ), psRawObject->iFaceCount );

		//Texture Links
		if ( psRawObject->iTextureLinkCount > 0 )
			Write( psRawObject->paTextureLink, sizeof( RawSMDTextureLink ), psRawObject->iTextureLinkCount );

		//Key Rotations
		if ( psRawObject->iKeyRotationCount > 0 )
			Write( psRawObject->paKeyRotation, sizeof( RawSMDKeyRotation ), psRawObject->iKeyRotationCount );

		//Key Positions
		if ( psRawObject->iKeyPositionCount > 0 )
			Write( psRawObject->paKeyPosition, sizeof( RawSMDKeyPosition ), psRawObject->iKeyPositionCount );

		//Key Scales
		if ( psRawObject->iKeyScaleCount > 0 )
			Write( psRawObject->paKeyScale, sizeof( RawSMDKeyScale ), psRawObject->iKeyScaleCount );

		//Key Rotation Matrices
		if ( psRawObject->iKeyRotationCount > 0 )
			Write( psRawObject->paRotation, sizeof( RawSMDMatrixF ), psRawObject->iKeyRotationCount );

		//Physique
		if ( (psRawObject->paPhysique) && (psRawObject->iVertexCount > 0) )
			Write( psRawObject->paPhysique, sizeof( RawSMDVertexPhysique ), psRawObject->iVertexCount );
	}

	if ( sModelVersion.HasInfluences() && psRawObject->bUseInfluences && (psRawObject->vInfluences.size() > 0) )
		Write( (void*)psRawObject->vInfluences.data(), sizeof( EXEMeshInfluenceVertex ), psRawObject->vInfluences.size() );
}

}
