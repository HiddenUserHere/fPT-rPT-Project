#include "stdafx.h"
#include "EXEModel.h"

EXEModel::EXEModel()
{
	Init();
}

EXEModel::~EXEModel()
{
	Close();
}

void EXEModel::Init()
{
	uMeshCount = 0;

	WRITEDWORD( szPadding2, NULL );

	pcBoneContainer = nullptr;

	uMaxFrame = 0;
	uFrame = 0;

	uMeshWidth = 0;
	uMeshHeight = 0;

	iBound = 0;
	iUnknown1 = 0;

	iFrameCount = 0;

	iLastFrame = -1;
	sLastAngle = Point3D( -1, -1, -1 );

	for ( int i = 0; i < _countof( paMesh ); i++ )
		uaMeshID[i] = i;

	pcTextureManager = nullptr;

	pcNewModel = nullptr;
}

void EXEModel::Close()
{
	for ( int i = uMeshCount - 1; i >= 0; i-- )
		DELET( paMesh[i] );

	DELET( pcTextureManager );

    if ( pcNewModel )
        DELET( pcNewModel );
}

BOOL EXEModel::SetPosition( Point3D * psPosition, Point3D * psAngle )
{
	sPosition.iX = psPosition->iX;
	sPosition.iY = psPosition->iY;
	sPosition.iZ = psPosition->iZ;

	sAngle.iX = psAngle->iX;
	sAngle.iY = psAngle->iY;
	sAngle.iZ = psAngle->iZ;

	for ( UINT i = 0; i < uMeshCount; i++ )
	{
		if( paMesh[uaMeshID[i]] )
			paMesh[uaMeshID[i]]->SetPosition( psPosition, psAngle );
	}

	if( pcNewModel )
	{
		Math::Vector3 pos( psPosition->iX / 256.f, psPosition->iY / 256.f, psPosition->iZ / 256.f );
		Math::Vector3Int ang( psAngle->iX, psAngle->iY, psAngle->iZ );
		pcNewModel->SetPositionRotation( &pos, &ang );
	}

	return TRUE;
}

NAKED BOOL EXEModel::SetFixedPosition()
{
	JMP( pfnSetFixedPosition );
}

void EXEModel::ClearVertex()
{
	for ( UINT i = 0; i < uMeshCount; i++ )
	{
		paMesh[i]->ClearVertex();
	}
}

void EXEModel::UpdateClipState( DWORD dwState )
{
	for ( UINT i = 0; i < uMeshCount; i++ )
	{
		paMesh[i]->dwUnused = dwState;
	}
}

NAKED void EXEModel::UpdateMeshes()
{
	JMP( pfnUpdateMeshes );
}

BOOL EXEModel::AddMesh( EXEMesh * pcMesh )
{
	if ( uMeshCount < 128 )
	{
		paMesh[uMeshCount] = pcMesh;
		uMeshCount++;

		int iFrame = 0;

		if ( pcMesh->iRotateCount > 0 && pcMesh->sRot )
			iFrame = pcMesh->sRot[pcMesh->iRotateCount - 1].iFrame;
		
		if ( pcMesh->iPosCount > 0 && pcMesh->sPos )
			iFrame = pcMesh->sPos[pcMesh->iPosCount - 1].iFrame;
		
		if ( iFrame > (int)uMaxFrame )
			uMaxFrame = (UINT)iFrame;

		if ( pcMesh->sX.iMin > (int)uMeshWidth )
			uMeshWidth = (UINT)pcMesh->sX.iMin;

		if ( pcMesh->sZ.iMin > (int)uMeshWidth )
			uMeshWidth = (UINT)pcMesh->sZ.iMin;

		if ( pcMesh->sY.iMin > (int)uMeshHeight )
			uMeshHeight = (UINT)pcMesh->sY.iMin;

		if ( pcMesh->iBound > iBound )
		{
			iBound		= pcMesh->iBound;
			iUnknown1	= pcMesh->iUnk1;
		}

		return TRUE;
	}
	return FALSE;
}

void EXEModel::SetParentBone( EXEModel * pcBone, EXEMesh * pcMesh )
{
	pcBoneContainer = pcBone;

	for ( UINT i = 0; i < uMeshCount; i++ )
	{
		if ( paMesh[i] )
			paMesh[i]->pParentMesh = pcMesh;
	}

	if( pcNewModel )
		pcNewModel->SetParent( (pcBone && pcBone->pcNewModel) ? pcBone->pcNewModel : nullptr, (pcMesh && pcMesh->pcNewMesh) ? pcMesh->pcNewMesh : nullptr );
}

EXEMesh * EXEModel::GetMesh( const std::string _strName )
{
	for ( UINT i = 0; i < uMeshCount; i++ )
	{
		if ( paMesh[i] )
		{
			if ( STRINGCOMPARE( paMesh[i]->szName, _strName.c_str() ) )
				return paMesh[i];
		}
	}

	return nullptr;
}

BOOL EXEModel::IsInView( int iX, int iY, int iZ, int iBoundingBox, Point3D * psLocalAngle, Point3D * psWorldAngle )
{
	if ( EXERender::bNoClipFaceRender )
		return TRUE;

	EXEPoint	cCheckVertex;
	EXEVertex	cZeroVertex;
	int			iaTrig[6] = {0};

	//Zero Vertex
	cZeroVertex.sPosition.iX = 0;
	cZeroVertex.sPosition.iY = 0;
	cZeroVertex.sPosition.iZ = 0;

	//Mask World Position
	int iWorldX = ( -psWorldAngle->iX ) & 4095;
	int iWorldY = ( -psWorldAngle->iY ) & 4095;
	int iWorldZ = ( -psWorldAngle->iZ ) & 4095;

	cCheckVertex.SetFromVertex( &cZeroVertex );
	cCheckVertex.Move( iX, iY, iZ );

	//Trigonometry Settings
	iaTrig[0] = PTSEN[iWorldX] >> 8;
	iaTrig[1] = PTCOS[iWorldX] >> 8;
	iaTrig[2] = PTSEN[iWorldY] >> 8;
	iaTrig[3] = PTCOS[iWorldY] >> 8;
	iaTrig[4] = PTSEN[iWorldZ] >> 8;
	iaTrig[5] = PTCOS[iWorldZ] >> 8;

	cCheckVertex.WorldRotate( iaTrig );

	//Get Position Z 2D
	int iPositionZ = cCheckVertex.sPosi.iZ >> 8;

	if( ( iPositionZ + iBoundingBox ) > 0 && ( iPositionZ < *(int*)0x82BF34 ) )
	{
		if( iPositionZ == 0 )
			iPositionZ = 1;

		iPositionZ = abs( iPositionZ );
		int iBound = ( iBoundingBox * (int)READDWORD( 0x82BF0C ) ) / iPositionZ;

		//Check if Rectangle 2D is in View
		if( ( cCheckVertex.sPosi2D.iX + iBound ) > 0 &&
			( cCheckVertex.sPosi2D.iX - iBound ) < RESOLUTION_WIDTH &&
			( cCheckVertex.sPosi2D.iY + iBound ) > 0 &&
			( cCheckVertex.sPosi2D.iY - iBound ) < RESOLUTION_HEIGHT )
			return TRUE;
	}

	return FALSE;
}

BOOL EXEModel::Animate( int iFrame, int iAngleX, int iAngleY, int iAngleZ, EXEFrame * pFrameInfo )
{
	if( ( iLastFrame == iFrame ) && ( sLastAngle.iX == iAngleX ) && ( sLastAngle.iY == iAngleY ) && ( sLastAngle.iZ == iAngleZ ) )
		return TRUE;

	iLastFrame = iFrame;
	sLastAngle.iX = iAngleX;
	sLastAngle.iY = iAngleY;
	sLastAngle.iZ = iAngleZ;

	if ( uMeshCount <= _countof( paMesh ) )
	{
		for ( UINT i = 0; i < uMeshCount; i++ )
		{
			if ( uaMeshID[i] < _countof( paMesh ) )
			{
				EXEMesh * p = paMesh[uaMeshID[i]];

				if ( p )
					p->Animate( iFrame, iAngleX, iAngleY, iAngleZ, pFrameInfo );
			}
		}
	}
	return TRUE;
}

BOOL EXEModel::Animate( int iFrame, int iAngleX, int iAngleY, int iAngleZ, EXEModel * pcTalkAnimation, EXEFrame * pFrameInfo )
{
	if ( (iLastFrame == iFrame) && (sLastAngle.iX == iAngleX) && (sLastAngle.iY == iAngleY) && (sLastAngle.iZ == iAngleZ) )
		return TRUE;

	iLastFrame = iFrame;
	sLastAngle.iX = iAngleX;
	sLastAngle.iY = iAngleY;
	sLastAngle.iZ = iAngleZ;

	if ( uMeshCount <= _countof( paMesh ) )
	{
		for ( UINT i = 0; i < uMeshCount; i++ )
		{
			if ( uaMeshID[i] < _countof( paMesh ) )
			{
				EXEMesh * p = paMesh[uaMeshID[i]];

				if ( p )
					p->Animate( iFrame, iAngleX, iAngleY, iAngleZ, pcTalkAnimation, pFrameInfo );
			}
		}
	}

	return TRUE;
}

BOOL EXEModel::AnimateTalk( int iFrame )
{
	if ( iLastFrame == iFrame )
		return TRUE;

	iLastFrame = iFrame;

	for ( UINT i = 0; i < uMeshCount; i++ )
	{
		paMesh[uaMeshID[i]]->AnimateTalk( iFrame );
	}

	return TRUE;
}

int EXEModel::Render( Point3D * psCamera, Point3D * psAngle, EXEFrame * psFrameInfo )
{
	Point3D s = { 0,0,0 };

	int iX = ( sPosition.iX - psCamera->iX ) >> 8;
	int iY = ( sPosition.iY - psCamera->iY ) >> 8;
	int iZ = ( sPosition.iZ - psCamera->iZ ) >> 8;

	int iRenderedCount;
	int iRenderCount = 0;

	if( pcTextureManager )
	{
		EXERender::GetRender()->SetTextureManager( pcTextureManager );

		if( pcTextureManager->iReformTexture )
		{
			if( IsInView( iX, iY, iZ, iBound, &sAngle, psAngle ) )
			{
				SetAnimationFrame( uFrame, psFrameInfo );

				for( UINT i = 0; i < uMeshCount; i++ )
				{
					if( paMesh[i]->PreRender( psCamera, psAngle ) )
					{
						for( int j = 0; j < (int)pcTextureManager->iTextureCount; j++ )
						{
							EXETextureMaterial * psTexture = &pcTextureManager->pcaTexture[j];
							if( psTexture->iReformTexture )
							{
								for( int k = 0; k < (int)psTexture->iTextureCounter; k++ )
								{
									if( psTexture->iaTextureFormState[k] )
									{
										Point3D sSize;
										sSize.iX = iBound << 9;
										sSize.iY = iBound << 9;
										sSize.iZ = iBound << 9;
										paMesh[i]->ReformTexture( psTexture->iaTextureFormState[k], j, k, &s, &sSize );
										break;
									}
								}
							}
						}

						iRenderedCount = paMesh[i]->FinalRender();
						iRenderCount += iRenderedCount;
					}
				}
				return iRenderCount;
			}
		}
	}

	if( IsInView( iX, iY, iZ, iBound, &sAngle, psAngle ) )
	{
		SetAnimationFrame( uFrame, psFrameInfo );

		for( UINT i = 0; i < uMeshCount; i++ )
		{
			iRenderedCount = paMesh[i]->Render( psCamera, psAngle );
			iRenderCount += iRenderedCount;
		}

		(*(DWORD*)0x02619820)++;
	}

	return iRenderCount;
}

int EXEModel::Render( Point3D * psCamera, Point3D * psAngle, EXEModel * pcModelTalk, EXEFrame * psFrameInfo )
{
	Point3D s = { 0,0,0 };

	int iX = (sPosition.iX - psCamera->iX) >> 8;
	int iY = (sPosition.iY - psCamera->iY) >> 8;
	int iZ = (sPosition.iZ - psCamera->iZ) >> 8;

	int iRenderedCount;
	int iRenderCount = 0;

	if ( pcTextureManager )
	{
		EXERender::GetRender()->SetTextureManager( pcTextureManager );

		if ( pcTextureManager->iReformTexture )
		{
			if ( IsInView( iX, iY, iZ, iBound, &sAngle, psAngle ) )
			{
				SetAnimationFrame( uFrame, pcModelTalk, psFrameInfo );

				for ( UINT i = 0; i < uMeshCount; i++ )
				{
					if ( paMesh[i]->PreRender( psCamera, psAngle ) )
					{
						for ( int j = 0; j < (int)pcTextureManager->iTextureCount; j++ )
						{
							EXETextureMaterial * psTexture = &pcTextureManager->pcaTexture[j];
							if ( psTexture->iReformTexture )
							{
								for ( int k = 0; k < (int)psTexture->iTextureCounter; k++ )
								{
									if ( psTexture->iaTextureFormState[k] )
									{
										Point3D sSize;
										sSize.iX = iBound << 9;
										sSize.iY = iBound << 9;
										sSize.iZ = iBound << 9;
										paMesh[i]->ReformTexture( psTexture->iaTextureFormState[k], j, k, &s, &sSize );
										break;
									}
								}
							}
						}

						iRenderedCount = paMesh[i]->FinalRender();
						iRenderCount += iRenderedCount;
					}
				}
				return iRenderCount;
			}
		}
	}

	if ( IsInView( iX, iY, iZ, iBound, &sAngle, psAngle ) )
	{
		SetAnimationFrame( uFrame, pcModelTalk, psFrameInfo );

		for ( UINT i = 0; i < uMeshCount; i++ )
		{
			iRenderedCount = paMesh[i]->Render( psCamera, psAngle );
			iRenderCount += iRenderedCount;
		}

		(*(DWORD*)0x02619820)++;
	}

	return iRenderCount;
}

int EXEModel::PreRender( Point3D * psCamera, Point3D * psAngle, char ** pszMeshList, int iMeshListCount, EXEModel * pcModelTalk, EXEFrame * psFrameInfo )
{
	int iX = (sPosition.iX - psCamera->iX) >> 8;
	int iY = (sPosition.iY - psCamera->iY) >> 8;
	int iZ = (sPosition.iZ - psCamera->iZ) >> 8;

	int iRenderCount = 0;

	if ( pcTextureManager )
		RENDERER->SetTextureManager( pcTextureManager );

	if ( IsInView( iX, iY, iZ, iBound, &sAngle, psAngle ) )
	{
		SetAnimationFrame( uFrame, pcModelTalk, psFrameInfo );

		for ( int i = 0; i < iMeshListCount; i++ )
		{
			for ( UINT j = 0; j < uMeshCount; j++ )
			{
				if ( STRINGCOMPAREI( pszMeshList[i], paMesh[j]->szName ) )
				{
					iRenderCount += paMesh[j]->Render( psCamera, psAngle );
				}
			}
		}

		(*(DWORD*)0x02619820)++;
	}

	return iRenderCount;
}

int EXEModel::PreRender( Point3D * psCamera, Point3D * psAngle, char ** pszMeshList, int iMeshListCount, EXEFrame * psFrameInfo )
{
	int iX = (sPosition.iX - psCamera->iX) >> 8;
	int iY = (sPosition.iY - psCamera->iY) >> 8;
	int iZ = (sPosition.iZ - psCamera->iZ) >> 8;

	int iRenderCount = 0;

	if ( pcTextureManager )
		RENDERER->SetTextureManager( pcTextureManager );

	if ( IsInView( iX, iY, iZ, iBound, &sAngle, psAngle ) )
	{
		SetAnimationFrame( uFrame, psFrameInfo );

		for ( int i = 0; i < iMeshListCount; i++ )
		{
			for ( UINT j = 0; j < uMeshCount; j++ )
			{
				if ( STRINGCOMPAREI( pszMeshList[i], paMesh[j]->szName ) )
				{
					iRenderCount += paMesh[j]->Render( psCamera, psAngle );
				}
			}
		}

		(*(DWORD*)0x02619820)++;
	}

	return iRenderCount;
}

BOOL EXEModel::SetAnimationFrame( int iFrame, EXEFrame * psFrameInfo )
{
	//Frame less than 0?
	if( iFrame < 0 )
		return FALSE;

	//Parent Mesh? Sync animation
	if( pcBoneContainer )
	{
		pcBoneContainer->Animate( iFrame, sAngle.iX, sAngle.iY, sAngle.iZ, psFrameInfo );

		for( UINT i = 0; i < uMeshCount; i++ )
		{
			if ( paMesh[i] == NULL )
				return TRUE;

			if ( paMesh[i]->pParentMesh )
				paMesh[i]->GetWorldMatrix() = paMesh[i]->pParentMesh->GetWorldMatrix();
		}
	}
	else
		Animate( iFrame, sAngle.iX, sAngle.iY, sAngle.iZ, psFrameInfo );

	return TRUE;
}

BOOL EXEModel::SetAnimationFrame( int iFrame, EXEModel * pcModelTalk, EXEFrame * psFrameInfo )
{
	//Frame less than 0?
	if ( iFrame < 0 || (int)pcModelTalk->uFrame < 0 )
		return FALSE;

	pcModelTalk->AnimateTalk( iFrame );

	//Parent Mesh? Sync animation
	if ( pcBoneContainer )
		pcBoneContainer->Animate( iFrame, sAngle.iX, sAngle.iY, sAngle.iZ, pcModelTalk, psFrameInfo );
	else
		Animate( iFrame, sAngle.iX, sAngle.iY, sAngle.iZ, pcModelTalk, psFrameInfo );

	return TRUE;
}

NAKED EXEModel * EXEModel::ReadModel( char * pszFileName, char * pszModelName )
{
	JMP( pfnReadModel );
}

NAKED DWORD EXEModel::ReadModelMap( char * pszFileName, char * pszModelName )
{
	JMP( pfnReadModelMap );
}

NAKED EXEModel * EXEModel::ReadBone( char * pszFileName )
{
	JMP( pfnReadBone );
}

NAKED EXEMesh * EXEModel::GetMesh( char * pszName )
{
	JMP( pfnGetMeshFromName );
}

EXEMesh * EXEModel::GetMesh( int iMeshID )
{
	if ( uMeshCount > 0 )
	{
		if ( (iMeshID >= 0) && (iMeshID < (int)uMeshCount) )
			return paMesh[iMeshID];
	}

	return nullptr;
}

int EXEModel::GetMeshIndex( const char * pszName )
{
	for ( UINT i = 0; i < uMeshCount; i++ )
	{
		if ( STRINGCOMPAREI( pszName, paMesh[i]->szName ) )
			return i;
	}

	return -1;
}

NAKED int EXEModel::SaveFile( char * pszFile )
{
	JMP( pfnSaveFile );
}

EXEModel * EXEModel::NewModelGate( EXEModelClearOld * pcOldModel )
{
	pcOldModel->Init();
	pcOldModel->Close();

	DELET( pcOldModel );

	return new EXEModel();
}

void EXEModel::CloseNewModelGate( EXEModel * pcModel )
{
	pcModel->Close();
}

EXEModelClearOld::EXEModelClearOld()
{
	Init();
}

EXEModelClearOld::~EXEModelClearOld()
{
	Close();
}

void EXEModelClearOld::Init()
{
    uMeshCount = 0;

    WRITEDWORD( szPadding2, NULL );

    pcBoneContainer = nullptr;

    uMaxFrame = 0;
    uFrame = 0;

    uMeshWidth = 0;
    uMeshHeight = 0;

    iBound = 0;
    iUnknown1 = 0;

    iFrameCount = 0;

    iLastFrame = -1;
    sLastAngle = Point3D( -1, -1, -1 );

    for ( int i = 0; i < _countof( paMesh ); i++ )
        uaMeshID[i] = i;

    pcTextureManager = nullptr;
}

void EXEModelClearOld::Close()
{
    for ( int i = uMeshCount - 1; i >= 0; i-- )
        DELET( paMesh[i] );

    DELET( pcTextureManager );
}
