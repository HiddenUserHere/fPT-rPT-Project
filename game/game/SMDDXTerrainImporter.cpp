#include "stdafx.h"
#include "SMDDXTerrainImporter.h"
#include "ImportSMDFile.h"


DX::Octree_ptr SMDTerrainImporter::CreateOctree()
{
	//Create new safe Octree with null pointers
	DX::Octree_ptr pOctree = std::make_shared<DX::Octree>();
	for ( int i = 0; i < _countof( pOctree->paChildren ); i++ )
		pOctree->paChildren[i] = nullptr;

	//Set null to Mesh (prevent crash)
	pOctree->pMesh = nullptr;

	return pOctree;
}

X3D::Vector3 SMDTerrainImporter::GetNodeCenter( const X3D::Vector3 vCenter, float fTerrainWidth, int iIndex )
{
	X3D::Vector3 vNodeCenter = X3D::Vector3( 0.0f, 0.0f, 0.0f );

	switch ( iIndex )
	{
		//Top Left Front
		case 0:
			vNodeCenter = X3D::Vector3( vCenter.x - fTerrainWidth / 4.0f, vCenter.y + fTerrainWidth / 4.0f, vCenter.z + fTerrainWidth / 4.0f );
			break;
		//Top Left Back
		case 1:
			vNodeCenter = X3D::Vector3( vCenter.x - fTerrainWidth / 4.0f, vCenter.y + fTerrainWidth / 4.0f, vCenter.z - fTerrainWidth / 4.0f );
			break;
		//Top Right Back
		case 2:
			vNodeCenter = X3D::Vector3( vCenter.x + fTerrainWidth / 4.0f, vCenter.y + fTerrainWidth / 4.0f, vCenter.z - fTerrainWidth / 4.0f );
			break;
		//Top Right Front
		case 3:
			vNodeCenter = X3D::Vector3( vCenter.x + fTerrainWidth / 4.0f, vCenter.y + fTerrainWidth / 4.0f, vCenter.z + fTerrainWidth / 4.0f );
			break;
		//Bottom Left Front
		case 4:
			vNodeCenter = X3D::Vector3( vCenter.x - fTerrainWidth / 4.0f, vCenter.y - fTerrainWidth / 4.0f, vCenter.z + fTerrainWidth / 4.0f );
			break;
		//Bottom Left Back
		case 5:
			vNodeCenter = X3D::Vector3( vCenter.x - fTerrainWidth / 4.0f, vCenter.y - fTerrainWidth / 4.0f, vCenter.z - fTerrainWidth / 4.0f );
			break;
		//Bottom Right Back
		case 6:
			vNodeCenter = X3D::Vector3( vCenter.x + fTerrainWidth / 4.0f, vCenter.y - fTerrainWidth / 4.0f, vCenter.z - fTerrainWidth / 4.0f );
			break;
		//Bottom Right Front
		case 7:
			vNodeCenter = X3D::Vector3( vCenter.x + fTerrainWidth / 4.0f, vCenter.y - fTerrainWidth / 4.0f, vCenter.z + fTerrainWidth / 4.0f );
			break;
	}

	return vNodeCenter;
}

DX::Mesh_ptr SMDTerrainImporter::CreateMeshes( DWORD * paFaceList, int iFaceCount )
{
	auto pMesh = std::make_shared<DX::Mesh>( "" );
	


	return pMesh;
}

void SMDTerrainImporter::SetTextureCoordinate( DX::Mesh_ptr pMesh, RawSMDTerrain * psRawTerrain, std::vector<std::vector<UVCoordinates>> & vTextureCoordinates )
{
	if ( vTextureCoordinates.size() > 0 )
	{
		for ( auto vTexCoord : vTextureCoordinates )
		{
			auto pTextureCoordinateBuffer = GetGraphicsDevice()->CreateVertexBuffer( sizeof( UVCoordinates ), vTexCoord.size() );
			UVCoordinates * psaTextureCoordinates = (UVCoordinates*)pTextureCoordinateBuffer->Lock();

			for ( UINT i = 0; i < vTexCoord.size(); i++ )
			{
				psaTextureCoordinates[i].fU = vTexCoord[i].fU;
				psaTextureCoordinates[i].fV = vTexCoord[i].fV;
			}

			pTextureCoordinateBuffer->Unlock();

			pMesh->AddTextureCoordBuffer( pTextureCoordinateBuffer );

			vTexCoord.clear();
		}
	}
	vTextureCoordinates.clear();
}

void SMDTerrainImporter::GetDimension( RawSMDTerrain * psRawTerrain, float & fCenterX, float & fCenterY, float & fCenterZ, float & fTerrainWidth )
{
	//Initialize the center position of the mesh to zero.
	fCenterX = 0.0f;
	fCenterY = 0.0f;
	fCenterZ = 0.0f;

	//Sum all the vertices in the mesh.
	for ( int i = 0; i < psRawTerrain->iVertexCount; i++ )
	{
		fCenterX += (float)psRawTerrain->paVertex[i].iX;
		fCenterY += (float)psRawTerrain->paVertex[i].iY;
		fCenterZ += (float)psRawTerrain->paVertex[i].iZ;
	}

	//And then divide it by the number of vertices to find the mid-point of the mesh.
	fCenterX = fCenterX / (float)psRawTerrain->iVertexCount;
	fCenterY = fCenterY / (float)psRawTerrain->iVertexCount;
	fCenterZ = fCenterZ / (float)psRawTerrain->iVertexCount;

	//Initialize the maximum and minimum size of the mesh.
	float fMaxWidth = 0.0f;
	float fMaxDepth = 0.0f;

	float fMinWidth = fabsf( (float)psRawTerrain->paVertex[0].iX - fCenterX );
	float fMinDepth = fabsf( (float)psRawTerrain->paVertex[0].iZ - fCenterZ );

	//Go through all the vertices and find the maximum and minimum width and depth of the mesh.
	for ( int i = 0; i < psRawTerrain->iVertexCount; i++ )
	{
		float fWidth = fabsf( (float)psRawTerrain->paVertex[i].iX - fCenterX );
		float fDepth = fabsf( (float)psRawTerrain->paVertex[i].iZ - fCenterZ );

		if ( fWidth > fMaxWidth )
			fMaxWidth = fWidth;

		if ( fDepth > fMaxDepth )
			fMaxDepth = fDepth;
		
		if ( fWidth < fMinWidth )
			fMinWidth = fWidth;

		if ( fDepth < fMinDepth )
			fMinDepth = fDepth;
	}

	//Find the absolute maximum value between the min and max depth and width.
	float fMaxX = (float)max( fabs( fMinWidth ), fabs( fMaxWidth ) );
	float fMaxZ = (float)max( fabs( fMinDepth ), fabs( fMaxDepth ) );

	//Calculate the maximum diameter of the mesh.
	fTerrainWidth = max( fMaxX, fMaxZ ) * 2.0f;
}

bool SMDTerrainImporter::IsInsideTriangle( RawSMDTerrain * psRawTerrain, int iIndex, const X3D::Vector3 vTerrainCenter, float fWidth )
{
	float fVertexX[3], fVertexY[3], fVertexZ[3];

	//Calculate the radius of this node.
	float fRadius = fWidth / 2.0f;

	//Get vertices position
	for ( int i = 0; i < 3; i++ )
	{
		fVertexX[i] = (float)psRawTerrain->paVertex[psRawTerrain->paFace[iIndex].saVertex[i]].iX;
		fVertexY[i] = (float)psRawTerrain->paVertex[psRawTerrain->paFace[iIndex].saVertex[i]].iY;
		fVertexZ[i] = (float)psRawTerrain->paVertex[psRawTerrain->paFace[iIndex].saVertex[i]].iZ;
	}

	//Check to see if the minimum of the x coordinates of the triangle is inside the node.
	float fMinX = min( fVertexX[0], min( fVertexX[1], fVertexX[2] ) );
	if ( fMinX > (vTerrainCenter.x + fRadius) )
		return false;

	//Check to see if the maximum of the x coordinates of the triangle is inside the node.
	float fMaxX = max( fVertexX[0], max( fVertexX[1], fVertexX[2] ) );
	if ( fMaxX < (vTerrainCenter.x - fRadius) )
		return false;
/*
	// Check to see if the minimum of the y coordinates of the triangle is inside the node.
	float fMinY = min( fVertexY[0], min( fVertexY[1], fVertexY[2] ) );
	if ( fMinY > (vTerrainCenter.y + fRadius) )
		return false;

	// Check to see if the maximum of the y coordinates of the triangle is inside the node.
	float fMaxY = max( fVertexY[0], max( fVertexY[1], fVertexY[2] ) );
	if ( fMaxY < (vTerrainCenter.y - fRadius) )
		return false;
*/
	// Check to see if the minimum of the z coordinates of the triangle is inside the node.
	float fMinZ = min( fVertexZ[0], min( fVertexZ[1], fVertexZ[2] ) );
	if ( fMinZ > ( vTerrainCenter.z + fRadius) )
		return false;

	// Check to see if the maximum of the z coordinates of the triangle is inside the node.
	float fMaxZ = max( fVertexZ[0], max( fVertexZ[1], fVertexZ[2] ) );
	if ( fMaxZ < (vTerrainCenter.z - fRadius) )
		return false;

	return true;
}

void SMDTerrainImporter::GetFaceTriangles( RawSMDTerrain * psRawTerrain, const X3D::Vector3 vTerrainCenter, float fWidth, std::vector<int> & vFaceID )
{
	vFaceID.clear();

	for ( int i = 0; i < psRawTerrain->iFaceCount; i++ )
	{
		if ( IsInsideTriangle( psRawTerrain, i, vTerrainCenter, fWidth ) )
			vFaceID.push_back( i );
	}
}

int SMDTerrainImporter::GetFaceTrianglesCount( RawSMDTerrain * psRawTerrain, const X3D::Vector3 vTerrainCenter, float fWidth )
{
	int iRet = 0;

    for ( int i = 0; i < psRawTerrain->iFaceCount; i++ )
    {
		if ( IsInsideTriangle( psRawTerrain, i, vTerrainCenter, fWidth ) )
			iRet++;
    }

	return iRet;
}

std::vector<int> vFacesAdd;

std::vector<int> vFaceID;

void SMDTerrainImporter::CreateNode( RawSMDTerrain * psRawTerrain, DX::Octree_ptr pOctree, X3D::Vector3 vTerrainCenter, float fTerrainWidth, std::vector<SMDMeshImporter::SMDMeshVertexData> & vVertexData, std::vector<DX::Material_ptr> & vMaterials )
{
	UINT uFaceCount = GetFaceTrianglesCount( psRawTerrain, vTerrainCenter, fTerrainWidth );
	if ( uFaceCount == 0 )
		return;


	auto vCenter = vTerrainCenter;

	float fWidth = fTerrainWidth;

	//Can divide Vertices into more blocks?
	if ( uFaceCount > uMaxTerrainFaces )
	{
		for ( int i = 0; i < 8; i++ )
		{
			X3D::Vector3 vNewCenter = vTerrainCenter;

            vNewCenter.x += (((i & 2) ? -1.0f : 1.0f) * (fTerrainWidth * 0.25f));
            vNewCenter.y += (((i & 4) ? 1.0f : -1.0f) * (fTerrainWidth * 0.25f));
            vNewCenter.z += (((i & 1) ? 1.0f : -1.0f) * (fTerrainWidth * 0.25f));

			uFaceCount = GetFaceTrianglesCount( psRawTerrain, vNewCenter, fTerrainWidth * 0.5f );
			if ( uFaceCount > 0 )
			{
				pOctree->paChildren[i] = CreateOctree();

				CreateNode( psRawTerrain, pOctree->paChildren[i], vNewCenter, fTerrainWidth * 0.5f, vVertexData, vMaterials );
			}
		}
	}
	else
	{
		auto pMesh = std::make_shared<DX::Mesh>( "" );

		std::vector<SMDMeshImporter::SMDMeshVertexData> vSubVertexData;
		for ( int i = 0; i < psRawTerrain->iFaceCount; i++ )
		{
			if ( IsInsideTriangle( psRawTerrain, i, vTerrainCenter, fTerrainWidth ) )
			{
				for ( int j = 0; j < 3; j++ )
					vSubVertexData.push_back( vVertexData[(i * 3) + j] );
			}
		}


		//Add Vertices
		pcMeshImporter->SetTerrainVertices( pMesh, psRawTerrain, vSubVertexData, vMaterials );

		//Set Texture Coordinates
		pcMeshImporter->CreateTerrainTextureCoordinates( pMesh, psRawTerrain, vMaterials, vSubVertexData );

		//Set Faces
		pcMeshImporter->SetTerrainFaces( pMesh, psRawTerrain, vMaterials, vSubVertexData );

		pOctree->pMesh = pMesh;

		vSubVertexData.clear();
	}
}

void SMDTerrainImporter::CreateChildNode( int iIndexNode, RawSMDTerrain * psRawTerrain, DX::Octree_ptr pOctree, X3D::Vector3 vTerrainCenter, float fTerrainWidth, std::vector<bool> vVertexAdd, std::vector<SMDMeshImporter::SMDMeshVertexData> & vVertexData, std::vector<DX::Material_ptr> & vMaterials )
{
	std::vector<SMDMeshImporter::SMDMeshVertexData> vSubVertexData;

	for ( UINT i = 0; i < (vVertexData.size() / 3); i++ )
	{
		if ( (vVertexAdd[i] == true) && (vVertexData[(i * 3)].iVertexID != -1) )
		{
			for ( int j = 0; j < 3; j++ )
			{
				vSubVertexData.push_back( vVertexData[(i * 3) + j] );

				vVertexData[(i * 3) + j].iVertexID = -1;
			}
		}
	}

	auto vCenter = GetNodeCenter( vTerrainCenter, fTerrainWidth, iIndexNode );

	uCurrentBlocksCount++;

	CreateNode( psRawTerrain, pOctree, vCenter, fTerrainWidth / 2.0f, vSubVertexData, vMaterials );

	uCurrentBlocksCount--;

	vSubVertexData.clear();
}

SMDTerrainImporter::SMDTerrainImporter()
{
	pcMeshImporter = new SMDMeshImporter();
	pcMaterialImporter = new SMDMaterialImporter();
}


SMDTerrainImporter::~SMDTerrainImporter()
{
	DELET( pcMaterialImporter );
	DELET( pcMeshImporter );
}

bool SMDTerrainImporter::Load( const std::string & strFilePath )
{
	_strFilePath = strFilePath;

	if ( FileExist( _strFilePath ) )
		return true;

	return false;
}

DX::Terrain_ptr SMDTerrainImporter::Build()
{
	Import::SMDFile impSMDFile( _strFilePath );
	if ( !impSMDFile.Load() )
		return nullptr;

	DX::Terrain_ptr pTerrain = std::make_shared<DX::Terrain>( _strFilePath );

	auto pRootOctree = CreateOctree();
	pTerrain->SetRootOctree( pRootOctree );

	RawSMDTerrain * psRawTerrain = impSMDFile.psRawTerrain;

	//Load Materials if it's a Model (otherwise, Bone)
	std::vector<DX::Material_ptr> vMaterials;
	if ( impSMDFile.sRawHeader.iMaterialCount > 0 )
		vMaterials = pcMaterialImporter->GetLoadMaterials( &impSMDFile.sRawMaterialHeader, TRUE );
	
	float fCenterX = 0.0f;
	float fCenterY = 0.0f;
	float fCenterZ = 0.0f;
	float fTerrainWidth = 0.0f;
	GetDimension( psRawTerrain, fCenterX, fCenterY, fCenterZ, fTerrainWidth );

	fMapCenterX = fCenterX;
	fMapCenterY = fCenterY;
	fMapCenterZ = fCenterZ;

	X3D::Vector3 vCenter = X3D::Vector3( fCenterX, fCenterY, fCenterZ );

	//Load Vertices
	std::vector<SMDMeshImporter::SMDMeshVertexData> vVertexData;
	pcMeshImporter->LoadTerrainVertices( psRawTerrain, vVertexData, vMaterials );

	uMaxTerrainFaces = psRawTerrain->iFaceCount <= MAX_TERRAIN_FACES ? psRawTerrain->iFaceCount / 2 : MAX_TERRAIN_FACES;

	CreateNode( psRawTerrain, pRootOctree, vCenter, fTerrainWidth, vVertexData, vMaterials );

	vVertexData.clear();

	pTerrain->Modified();

	//Clear Materials
	vMaterials.clear();

	return pTerrain;
}
