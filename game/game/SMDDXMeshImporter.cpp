#include "stdafx.h"
#include "SMDDXMeshImporter.h"
#include "ImportSMDFile.h"


SMDMeshImporter::SMDMeshImporter()
{
}


SMDMeshImporter::~SMDMeshImporter()
{
}



int GetVertexCopy( std::vector<SMDMeshImporter::SMDMeshVertexData> & vVertexData, SMDMeshImporter::SMDMeshVertexData & sVertex )
{
	int iID = -1;

	for ( auto & sVertexData : vVertexData )
	{
		if ( (sVertexData.sVertex.x == sVertex.sVertex.x) && (sVertexData.sVertex.y == sVertex.sVertex.y) && (sVertexData.sVertex.z == sVertex.sVertex.z) )
		{
			if ( (sVertexData.vUV.size() > 0) && (sVertexData.vUV.size() == sVertex.vUV.size()) )
			{
				bool bNotSame = false;
				for ( size_t i = 0; i < sVertexData.vUV.size(); i++ )
				{
					if ( (sVertexData.vUV[i].fU != sVertex.vUV[i].fU) || (sVertexData.vUV[i].fV != sVertex.vUV[i].fV) )
					{
						bNotSame = true;
						break;
					}
				}

				if ( bNotSame == false )
				{
					iID = sVertexData.iVertexID;
					break;
				}
			}
		}
	}

	return iID;
}


int GetVertexCopyTerrain( std::vector<SMDMeshImporter::SMDMeshVertexData> & vVertexData, SMDMeshImporter::SMDMeshVertexData & sVertex )
{
	return -1;

	int iID = -1;

	for ( auto & sVertexData : vVertexData )
	{
		if ( (sVertexData.sVertex.x == sVertex.sVertex.x) && (sVertexData.sVertex.y == sVertex.sVertex.y) && (sVertexData.sVertex.z == sVertex.sVertex.z) )
		{
			if ( (sVertexData.vUV.size() > 0) && (sVertexData.vUV.size() == sVertex.vUV.size()) )
			{
				bool bNotSame = false;
				for ( size_t i = 0; i < sVertexData.vUV.size(); i++ )
				{
					if ( (sVertexData.vUV[i].fU != sVertex.vUV[i].fU) || (sVertexData.vUV[i].fV != sVertex.vUV[i].fV) )
					{
						bNotSame = true;
						break;
					}
				}

				if ( bNotSame == false )
				{
					iID = sVertexData.iVertexID;
					break;
				}
			}
		}
	}

	return iID;
}

void SMDMeshImporter::SetVertices( DX::Model_ptr pModel, DX::Mesh_ptr pMesh, RawSMDObject * psObject, std::vector<SMDMeshVertexData> & vVertexData, std::vector<DX::Material_ptr> & vMaterials )
{
	std::vector<float> vSkinIndices;

	std::vector<X3D::Vector3> vNormals;
	std::vector<X3D::Vector3> vVertices;
	std::vector<D3DCOLOR> vColors;

	int iVertexID = 0;
	if ( psObject->iVertexCount > 0 )
	{
		for ( int i = 0; i < psObject->iFaceCount; i++ )
		{
			for ( int j = 0; j < 3; j++ )
			{
				SMDMeshVertexData sVertexData;

				//Set Position
				sVertexData.sVertex.x = (float)psObject->paVertex[psObject->paFace[i].saVertex[j]].iX / 256.0f;
				sVertexData.sVertex.y = (float)psObject->paVertex[psObject->paFace[i].saVertex[j]].iY / 256.0f;
				sVertexData.sVertex.z = (float)psObject->paVertex[psObject->paFace[i].saVertex[j]].iZ / 256.0f;

				//Set Normal
				sVertexData.sNormal.x = (float)psObject->paVertex[psObject->paFace[i].saVertex[j]].iNormalX / 256.0f;
				sVertexData.sNormal.y = (float)psObject->paVertex[psObject->paFace[i].saVertex[j]].iNormalY / 256.0f;
				sVertexData.sNormal.z = (float)psObject->paVertex[psObject->paFace[i].saVertex[j]].iNormalZ / 256.0f;

				//Set Color
				sVertexData.d3dColor = D3DCOLOR_ARGB( 255, 255, 255, 255 );

				//Set UV coordinates
				if ( psObject->paFace[i].pTextureLink )
				{
					//Have Material?
					if ( (UINT)psObject->paFace[i].sMaterial < vMaterials.size() )
					{
						auto pTextureLink = psObject->paFace[i].pTextureLink;

						//Add UVs
						for ( size_t k = 0; k < vMaterials[psObject->paFace[i].sMaterial]->GetTextures().size(); k++ )
						{
							if ( pTextureLink == nullptr )
								break;

							UVCoordinates sUV;
							sUV.fU = pTextureLink->u[j];
							sUV.fV = 1.0f - pTextureLink->v[j];
							sVertexData.vUV.push_back( sUV );

							//Have Next?
							if ( pTextureLink->pNext )
								pTextureLink = pTextureLink->pNext;
						}
					}
				}
				
				if ( int iID = GetVertexCopy( vVertexData, sVertexData ); iID >= 0 )
				{
					psObject->paFace[i].saVertex[j] = iID;
				}
				else
				{
					//Has Model and Bone?
					if ( pModel && pModel->GetBoneModel() )
					{
						//Has Physique?
						if ( psObject->paPhysique )
						{
							//Set Skin Indice for the current Vertex
							RawSMDVertexPhysique * psPhysique = psObject->paPhysique + psObject->paFace[i].saVertex[j];
							for ( UINT k = 0; k < pModel->GetBoneModel()->GetMeshes().size(); k++ )
							{
								auto pMesh = pModel->GetBoneModel()->GetMeshes()[k];
								if ( pMesh->GetName().compare( psPhysique->szName ) == 0 )
								{
									sVertexData.iBoneID = k;
									break;
								}
							}
						}
					}

					//Update New Index
					psObject->paFace[i].saVertex[j] = iVertexID;
					sVertexData.iVertexID = iVertexID;
					iVertexID++;

					vVertexData.push_back( sVertexData );
				}
			}
		}

		for ( auto & sVertexData : vVertexData )
		{
			vVertices.push_back( sVertexData.sVertex );
			vNormals.push_back( sVertexData.sNormal );
			vColors.push_back( sVertexData.d3dColor );
			vSkinIndices.push_back( (float)sVertexData.iBoneID );
		}

		pMesh->SetVertices( vVertices );
		pMesh->SetNormals( vNormals );

		SetVerticesBuffers( pMesh, vColors );

		vColors.clear();
	}

	//Set to Mesh
	pMesh->SetSkinIndices( vSkinIndices );

	if ( vSkinIndices.size() > 0 )
	{
		//Create new Skin Indices Vertex Buffer
		auto pSkinIndicesBuffer = GetGraphicsDevice()->CreateVertexBuffer( sizeof( DWORD ), vSkinIndices.size() );
		DWORD * pdwaSkinIndice = (DWORD *)pSkinIndicesBuffer->Lock();

		//Set Index for it's Bone Matrix
		for ( UINT i = 0; i < vSkinIndices.size(); i++ )
		{
			pdwaSkinIndice[i] = (DWORD)vSkinIndices[i];
		}

		//Add to Mesh to be rendered
		pSkinIndicesBuffer->Unlock();
		pMesh->SetSkinIndicesBuffer( pSkinIndicesBuffer );
	}
}

void SMDMeshImporter::SetSkinIndices( DX::Model_ptr pModel, DX::Mesh_ptr pMesh, RawSMDObject * psRawObject )
{
	//Has Bone?
	if ( pModel->GetBoneModel() )
	{
		//Has Physique?
		if ( psRawObject->paPhysique )
		{
			std::vector<float> vSkinIndices;

			for ( int i = 0; i < psRawObject->iVertexCount; i++ )
			{
				RawSMDVertexPhysique * psPhysique = psRawObject->paPhysique + i;
				for ( UINT j = 0; j < pModel->GetBoneModel()->GetMeshes().size(); j++ )
				{
					auto pMesh = pModel->GetBoneModel()->GetMeshes()[j];
					if ( pMesh->GetName().compare( psPhysique->szName ) == 0 )
					{
						vSkinIndices.push_back( (float)j );
						break;
					}
				}
			}

			//Set to Mesh
			pMesh->SetSkinIndices( vSkinIndices );

			if ( vSkinIndices.size() > 0 )
			{
				//Create new Skin Indices Vertex Buffer
				auto pSkinIndicesBuffer = GetGraphicsDevice()->CreateVertexBuffer( sizeof( DWORD ), vSkinIndices.size() );
				DWORD * pbaSkinIndice = (DWORD*)pSkinIndicesBuffer->Lock();

				//Set Index for it's Bone Matrix
				for ( UINT i = 0; i < vSkinIndices.size(); i++ )
					pbaSkinIndice[i] = (DWORD)vSkinIndices[i];

				//Add to Mesh to be rendered
				pSkinIndicesBuffer->Unlock();
				pMesh->SetSkinIndicesBuffer( pSkinIndicesBuffer );
			}
		}
	}
}

void SMDMeshImporter::CreateVertexDeclaration( DX::Mesh_ptr pMesh, DX::MeshPart_ptr pMeshPart )
{
	auto pVertexElements = std::make_shared<DX::VertexElements>();
	int iStreamNumber = 0;

	if ( pMesh->GetVertices().size() > 0 )
		pVertexElements->AddElement( iStreamNumber++, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 );
	
	if ( pMesh->GetNormals().size() > 0 )
		pVertexElements->AddElement( iStreamNumber++, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 );
	
	if ( pMesh->GetVertices().size() > 0 )
		pVertexElements->AddElement( iStreamNumber++, 0, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 );

	if ( pMesh->GetSkinIndices().size() > 0 )
		pVertexElements->AddElement( iStreamNumber++, 0, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0 );

	if ( pMeshPart->GetMaterial() )
	{
		UINT uMaterialCount = pMeshPart->GetMaterial()->GetTextures().size();		
		for ( UINT i = 0; i < uMaterialCount; i++ )
			pVertexElements->AddElement( iStreamNumber + i, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, i );
	}
	auto pVertexDeclaration = GetGraphicsDevice()->CreateVertexDeclaration( pVertexElements );
	pMeshPart->SetVertexDeclaration( pVertexDeclaration );
}


void SMDMeshImporter::CreateIndices( DX::MeshPart_ptr pMeshPart, std::vector<unsigned short> & vIndices )
{
	pMeshPart->SetIndices( vIndices );

	if ( pMeshPart->GetIndices().size() > 0 )
	{
		auto pIndexBuffer = GetGraphicsDevice()->CreateIndexBuffer( sizeof( unsigned short ) * 3, pMeshPart->GetIndices().size() / 3 );

		unsigned short * puaIndices = (unsigned short*)pIndexBuffer->Lock();
		for ( UINT i = 0; i < pMeshPart->GetIndices().size(); i++ )
			puaIndices[i] = pMeshPart->GetIndices()[i];

		pIndexBuffer->Unlock();

		pMeshPart->SetIndexBuffer( pIndexBuffer );
	}
}

void SMDMeshImporter::CreateTextureCoordinates( DX::Mesh_ptr pMesh, RawSMDObject * psObject, std::vector<DX::Material_ptr> & vMaterials, std::vector<SMDMeshVertexData> & vVertexData )
{
	if ( psObject->iTextureLinkCount > 0 )
	{
		for ( size_t i = 0; i < vMaterials.size(); i++ )
		{
			for ( size_t j = 0; j < vMaterials[i]->GetTextures().size(); j++ )
			{
				if ( pMesh->GetTextureCoordBuffers().size() == j )
					pMesh->AddTextureCoordBuffer( GetGraphicsDevice()->CreateVertexBuffer( sizeof( UVCoordinates ), vVertexData.size() ) );

				auto pTextureCoordinateBuffer = pMesh->GetTextureCoordBuffers()[j];
				UVCoordinates * psaTextureCoordinates = (UVCoordinates *)pTextureCoordinateBuffer->Lock();

				for ( int k = 0; k < psObject->iFaceCount; k++ )
				{
					if ( psObject->paFace[k].sMaterial == (short)i )
					{
						auto psTexture = psObject->paFace[k].pTextureLink;
						for ( size_t l = 0; l < j; l++ )
							psTexture = psTexture->pNext;

						for ( int l = 0; l < 3; l++ )
						{
							psaTextureCoordinates[psObject->paFace[k].saVertex[l]].fU = vVertexData[psObject->paFace[k].saVertex[l]].vUV[j].fU;
							psaTextureCoordinates[psObject->paFace[k].saVertex[l]].fV = 1.0f - vVertexData[psObject->paFace[k].saVertex[l]].vUV[j].fV;
						}
					}
				}

				pTextureCoordinateBuffer->Unlock();
			}
		}
	}
}

void SMDMeshImporter::SetFaces( DX::Model_ptr pModel, DX::Mesh_ptr pMesh, RawSMDObject * psObject, std::vector<DX::Material_ptr> & vMaterials )
{
	if ( psObject->iFaceCount > 0 )
	{
		for ( size_t i = 0; i < vMaterials.size(); i++ )
		{
			std::vector<unsigned short> vIndices;
			vIndices.clear();

			for ( int j = 0; j < psObject->iFaceCount; j++ )
			{
				if ( psObject->paFace[j].sMaterial == (short)i )
				{
					for ( int k = 0; k < 3; k++ )
						vIndices.push_back( psObject->paFace[j].saVertex[k] );
				}
			}

			if ( vIndices.size() > 0 )
			{
				auto pMeshPart = std::make_shared<DX::MeshPart>();
				CreateIndices( pMeshPart, vIndices );

				pMeshPart->SetMaterial( vMaterials[i] );

				CreateVertexDeclaration( pMesh, pMeshPart );

				if ( vMaterials[i]->GetTransparent() )
					pMesh->AddTransparentPart( pMeshPart, (vMaterials[i]->GetUseType() & DX::Material::EType::TYPE_RenderLast) );
				else
					pMesh->AddOpaquePart( pMeshPart );
			}
		}
	}
}

void SMDMeshImporter::SetKeys( DX::Mesh_ptr pMesh, RawSMDObject * psObject )
{
	//Set Scaling
	std::vector<DX::KeyScaling> vScaling;
	for ( int i = 0; i < psObject->iKeyScaleCount; i++ )
	{
		RawSMDKeyScale * psScale = psObject->paKeyScale + i;
		DX::KeyScaling sScaling;

		sScaling.iFrame = psScale->iFrame;
		sScaling.vScaling.x = (float)psScale->iX / 256.0f;
		sScaling.vScaling.y = (float)psScale->iY / 256.0f;
		sScaling.vScaling.z = (float)psScale->iZ / 256.0f;
		vScaling.push_back( sScaling );
	}
	pMesh->SetKeyScalings( vScaling );

	//Set Rotation
	std::vector<DX::KeyRotation> vRotation;
	for ( int i = 0; i < psObject->iKeyRotationCount; i++ )
	{
		RawSMDKeyRotation * psRotation = psObject->paKeyRotation + i;
		DX::KeyRotation sRotation;
		sRotation.iFrame = psRotation->iFrame;
		sRotation.qRotation = X3D::Quaternion( psRotation->fW, psRotation->fX, psRotation->fY, psRotation->fZ );

		//Copy Matrix from SMD (Matrix by X3D Wrong??? why?)
		for ( int j = 0; j < 4; j++ )
			for ( int k = 0; k < 4; k++ )
				sRotation.mRotation.m[j][k] = psObject->paRotation[i].f[j][k];

		vRotation.push_back( sRotation );

		//Set Max Frame
		if ( pMesh->GetMaxFrame() < psRotation->iFrame )
			pMesh->SetMaxFrame( psRotation->iFrame );
	}
	pMesh->SetKeyRotations( vRotation );

	//Set Translation
	std::vector<DX::KeyTranslation> vTranslation;
	for ( int i = 0; i < psObject->iKeyPositionCount; i++ )
	{
		RawSMDKeyPosition * psTranslation = psObject->paKeyPosition + i;
		DX::KeyTranslation sPosition;

		sPosition.iFrame = psTranslation->iFrame;
		sPosition.vTranslation.x = psTranslation->fX;
		sPosition.vTranslation.y = psTranslation->fY;
		sPosition.vTranslation.z = psTranslation->fZ;
		vTranslation.push_back( sPosition );

		//Set Max Frame
		if ( pMesh->GetMaxFrame() < psTranslation->iFrame )
			pMesh->SetMaxFrame( psTranslation->iFrame );
	}
	pMesh->SetKeyTranslations( vTranslation );
}

void SMDMeshImporter::SetMatrices( DX::Mesh_ptr pMesh, RawSMDObject * psObject )
{
	//Transformation Matrix
	X3D::Matrix4 sTransformation;
	for ( int i = 0; i < 4; i++ )
		for ( int j = 0; j < 4; j++ )
			sTransformation.m[i][j] = (float)psObject->sMatrixTM.i[i][j] / 256.0f;

	pMesh->SetTransformation( sTransformation );

	//Transformation Rotation Matrix
	X3D::Matrix4 sTransformationRotation;
	for ( int i = 0; i < 4; i++ )
		for ( int j = 0; j < 4; j++ )
			sTransformationRotation.m[i][j] = (float)psObject->sRotationMatrix.i[i][j] / 256.0f;

	pMesh->SetTransformationRotation( sTransformationRotation );

	//Transformation Translation Matrix
	X3D::Matrix4 sTransformationTranslation;
	for ( int i = 0; i < 4; i++ )
	{
		for ( int j = 0; j < 4; j++ )
		{
			//Identity Matrix
			if ( i == j )
				sTransformationTranslation.m[i][j] = 1.0f;
			else
				sTransformationTranslation.m[i][j] = 0.0f;
		}
	}

	//Set Position
	sTransformationTranslation.m[3][0] = (float)psObject->sTranslationVector.iX / 256.0f;
	sTransformationTranslation.m[3][1] = (float)psObject->sTranslationVector.iY / 256.0f;
	sTransformationTranslation.m[3][2] = (float)psObject->sTranslationVector.iZ / 256.0f;

	pMesh->SetTransformationTranslation( sTransformationTranslation );

	//Transformation Inverse Matrix
	X3D::Matrix4 sTransformationInverse;
	for ( int i = 0; i < 4; i++ )
		for ( int j = 0; j < 4; j++ )
			sTransformationInverse.m[i][j] = (float)psObject->sInvertedMatrixTM.i[i][j] / 256.0f;

	pMesh->SetTransformationInverse( sTransformationInverse );
}

void SMDMeshImporter::UpdateMeshParent( std::vector<DX::Mesh_ptr>& vMeshes, RawSMDObject * psaRawObject )
{
	//Update Meshes's Parent
	for ( UINT i = 0; i < vMeshes.size(); i++ )
	{
		//Get Mesh
		RawSMDObject * psRawObject = psaRawObject + i;
		auto pMesh = vMeshes[i];

		//Mesh has a Parent Mesh?
		if ( psRawObject->szParentObjectName[0] != 0 )
		{
			//Search for Mesh Parent by it's name
			for ( UINT j = 0; j < vMeshes.size(); j++ )
			{
				//Found?
				auto pMeshParent = vMeshes[j];
				if ( pMeshParent->GetName().compare( psRawObject->szParentObjectName ) == 0 )
				{
					//Update Mesh
					pMesh->SetParent( pMeshParent );
					break;
				}
			}
		}
	}
}

void SMDMeshImporter::SetVerticesBuffers( DX::Mesh_ptr pMesh, std::vector<D3DCOLOR> & vColors )
{
	//Vertex Buffer
	{
		std::vector<X3D::Vector3> vVertices;

		auto pVertexBuffer = GetGraphicsDevice()->CreateVertexBuffer( sizeof( X3D::Vector3 ), pMesh->GetVertices().size() );

		X3D::Vector3 * pcaVectexBuffer = (X3D::Vector3*)pVertexBuffer->Lock();
		for ( UINT i = 0; i < pMesh->GetVertices().size(); i++ )
			pcaVectexBuffer[i] = pMesh->GetVertices()[i];

		pVertexBuffer->Unlock();

		pMesh->SetVertexBuffer( pVertexBuffer );

		pMesh->SetDynamicVertexBuffer( GetGraphicsDevice()->CreateVertexBuffer( sizeof( X3D::Vector3 ), pMesh->GetVertices().size(), TRUE ) );

		pVertexBuffer = pMesh->GetDynamicVertexBuffer();

		pcaVectexBuffer = (X3D::Vector3 *)pVertexBuffer->Lock();
		for ( UINT i = 0; i < pMesh->GetVertices().size(); i++ )
			pcaVectexBuffer[i] = pMesh->GetVertices()[i];

		pVertexBuffer->Unlock();
	}

	//Normal Vertex Buffer
	if ( pMesh->GetNormals().size() > 0 )
	{
		std::vector<X3D::Vector3> vNormals;

		auto pNormalBuffer = GetGraphicsDevice()->CreateVertexBuffer( sizeof( X3D::Vector3 ), pMesh->GetNormals().size() );

		X3D::Vector3 * pcaNormalVertexBuffer = (X3D::Vector3*)pNormalBuffer->Lock();
		for ( UINT i = 0; i < pMesh->GetNormals().size(); i++ )
			pcaNormalVertexBuffer[i] = pMesh->GetNormals()[i];

		pNormalBuffer->Unlock();

		pMesh->SetNormalBuffer( pNormalBuffer );

		pMesh->SetDynamicNormalBuffer( GetGraphicsDevice()->CreateVertexBuffer( sizeof( X3D::Vector3 ), pMesh->GetNormals().size(), TRUE ) );
	}

	//Color Buffer
	{
		auto pColorBuffer = GetGraphicsDevice()->CreateVertexBuffer( sizeof( D3DCOLOR ), vColors.size() );

		D3DCOLOR * paColor = (D3DCOLOR*)pColorBuffer->Lock();
		for ( UINT i = 0; i < vColors.size(); i++ )
			paColor[i] = vColors[i];

		pColorBuffer->Unlock();

		pMesh->SetColorBuffer( pColorBuffer );
	}
}

void SMDMeshImporter::SetTerrainVertices( DX::Mesh_ptr pMesh, RawSMDTerrain * psSMDTerrain, std::vector<SMDMeshVertexData> & vVertexData, std::vector<DX::Material_ptr> & vMaterials )
{
	std::vector<X3D::Vector3> vNormals;
	std::vector<X3D::Vector3> vVertices;
	std::vector<D3DCOLOR> vColors;

	for ( auto & sVertexData : vVertexData )
	{
		vVertices.push_back( sVertexData.sVertex );
		vColors.push_back( sVertexData.d3dColor );
	}

	pMesh->SetVertices( vVertices );
	pMesh->SetNormals( vNormals );

	SetVerticesBuffers( pMesh, vColors );

	vColors.clear();
}

void SMDMeshImporter::CreateTerrainTextureCoordinates( DX::Mesh_ptr pMesh, RawSMDTerrain * psSMDTerrain, std::vector<DX::Material_ptr> & vMaterials, std::vector<SMDMeshVertexData> & vVertexData )
{
	int iIndices = 0;
	if ( psSMDTerrain->iTextureLinkCount > 0 )
	{
		for ( size_t i = 0; i < vMaterials.size(); i++ )
		{
			for ( size_t j = 0; j < vMaterials[i]->GetTextures().size(); j++ )
			{
				if ( pMesh->GetTextureCoordBuffers().size() == j )
					pMesh->AddTextureCoordBuffer( GetGraphicsDevice()->CreateVertexBuffer( sizeof( UVCoordinates ), vVertexData.size() ) );

				auto pTextureCoordinateBuffer = pMesh->GetTextureCoordBuffers()[j];
				UVCoordinates * psaTextureCoordinates = (UVCoordinates *)pTextureCoordinateBuffer->Lock();

				for ( UINT k = 0; k < (vVertexData.size() / 3); k++ )
				{
					//We just need the first vertex of every face
					auto & sVertex = vVertexData[k * 3];

					if ( sVertex.iBoneID == (short)i )
					{
						for ( int l = 0; l < 3; l++ )
						{
							psaTextureCoordinates[(k * 3) + l].fU = vVertexData[(k * 3) + l].vUV[0].fU;
							psaTextureCoordinates[(k * 3) + l].fV = vVertexData[(k * 3) + l].vUV[0].fV;
						}
					}
				}

				pTextureCoordinateBuffer->Unlock();
			}
		}
	}
}

void SMDMeshImporter::SetTerrainFaces( DX::Mesh_ptr pMesh, RawSMDTerrain * psSMDTerrain, std::vector<DX::Material_ptr> & vMaterials, std::vector<SMDMeshVertexData> & vVertexData )
{
	int iIndices = 0;
	if ( psSMDTerrain->iFaceCount > 0 )
	{
		for ( size_t i = 0; i < vMaterials.size(); i++ )
		{
			std::vector<unsigned short> vIndices;
			vIndices.clear();

			for ( UINT j = 0; j < (vVertexData.size() / 3); j++ )
			{
				//We just need the first vertex of every face
				auto & sVertex = vVertexData[j * 3];

				if ( sVertex.iBoneID == (short)i )
				{
					for ( int k = 0; k < 3; k++ )
						vIndices.push_back( (j * 3) + k );
				}
			}

			if ( vIndices.size() > 0 )
			{
				auto pMeshPart = std::make_shared<DX::MeshPart>();
				CreateIndices( pMeshPart, vIndices );

				pMeshPart->SetMaterial( vMaterials[i] );

				CreateVertexDeclaration( pMesh, pMeshPart );

				if ( vMaterials[i]->GetTransparent() )
					pMesh->AddTransparentPart( pMeshPart, (vMaterials[i]->GetUseType() & DX::Material::EType::TYPE_RenderLast) );
				else
					pMesh->AddOpaquePart( pMeshPart );
			}
		}
	}
}

void SMDMeshImporter::LoadTerrainVertices( RawSMDTerrain * psSMDTerrain, std::vector<SMDMeshVertexData> & vVertexData, std::vector<DX::Material_ptr> & vMaterials )
{
	int iVertexID = 0;
	if ( psSMDTerrain->iVertexCount > 0 )
	{
		for ( int i = 0; i < psSMDTerrain->iFaceCount; i++ )
		{
			for ( int j = 0; j < 3; j++ )
			{
				SMDMeshVertexData sVertexData;

				//Set Position
				sVertexData.sVertex.x = (float)psSMDTerrain->paVertex[psSMDTerrain->paFace[i].saVertex[j]].iX / 256.0f;
				sVertexData.sVertex.y = (float)psSMDTerrain->paVertex[psSMDTerrain->paFace[i].saVertex[j]].iY / 256.0f;
				sVertexData.sVertex.z = (float)psSMDTerrain->paVertex[psSMDTerrain->paFace[i].saVertex[j]].iZ / 256.0f;

				//Set Color
				sVertexData.d3dColor = D3DCOLOR_ARGB(
					psSMDTerrain->paVertex[psSMDTerrain->paFace[i].saVertex[j]].sAlpha,
					psSMDTerrain->paVertex[psSMDTerrain->paFace[i].saVertex[j]].sRed,
					psSMDTerrain->paVertex[psSMDTerrain->paFace[i].saVertex[j]].sGreen,
					psSMDTerrain->paVertex[psSMDTerrain->paFace[i].saVertex[j]].sBlue );

				//Set UV coordinates
				if ( psSMDTerrain->paFace[i].pTextureLink )
				{
					//Have Material?
					if ( (UINT)psSMDTerrain->paFace[i].sMaterial < vMaterials.size() )
					{
						auto pTextureLink = psSMDTerrain->paFace[i].pTextureLink;

						//Add UVs
						for ( size_t k = 0; k < vMaterials[psSMDTerrain->paFace[i].sMaterial]->GetTextures().size(); k++ )
						{
							if ( pTextureLink == nullptr )
								break;

							UVCoordinates sUV;
							sUV.fU = pTextureLink->u[j];
							sUV.fV = pTextureLink->v[j];
							sVertexData.vUV.push_back( sUV );

							//Have Next?
							if ( pTextureLink->pNext )
								pTextureLink = pTextureLink->pNext;
						}
					}
				}

				sVertexData.iVertexID = iVertexID;
				sVertexData.iBoneID = psSMDTerrain->paFace[i].sMaterial;
				iVertexID++;

				vVertexData.push_back( sVertexData );
			}
		}
	}
}
