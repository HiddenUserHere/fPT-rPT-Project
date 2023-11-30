#include "stdafx.h"
#include "EXEMesh.h"

EXEVertex * psWorldVertex = (EXEVertex*)0x2444C20;

EXEMesh::EXEMesh()
{
	Constructor();
}

EXEMesh::~EXEMesh()
{
	Destructor();
}

EXEMesh::EXEMesh( int iNumVertexCount, int iNumFaceCount )
{
	Constructor( iNumVertexCount, iNumFaceCount );
}

void EXEMesh::Constructor()
{
	szName[0] = 0;
	szParentName[0] = 0;
	EXEMatrixIdentityI( sTm );

	pParentMesh = nullptr;

	sRot = nullptr;
	sPos = nullptr;
	sScale = nullptr;

	iRotateCount = 0;
	iPosCount = 0;
	iScaleCount = 0;

	sPreviousRotate = nullptr;

	pMeshFace = nullptr;
	pMeshVertex = nullptr;
	pMeshFacePosition = nullptr;
	iFaceCount = 0;
	iVertexCount = 0;
	iTextureLinkCount = 0;

	pVertexBoneList = nullptr;

	pcNewMesh = nullptr;

	bUseInfluences = FALSE;
}

void EXEMesh::Destructor()
{
	DELET( sScale );
	DELET( sPos );
	DELET( sRot );
	DELET( sPreviousRotate );

	DELET( pMeshFacePosition );
	DELET( pVertexBoneList );
	DELET( pMeshFace );
	DELET( pMeshVertex );
}

void EXEMesh::Constructor( int iNumVertices, int iNumFaces )
{
	Constructor();

	CALLT_WITH_ARG3( 0x00473E40, (DWORD)this, iNumVertices, iNumFaces, 0 );

}

NAKED int EXEMesh::BeginRender()
{
	JMP( pfnBeginRender );
}

BOOL EXEMesh::IsInView( int iX, int iY, int iZ, Point3D * psLocalAngle, Point3D * psWorldAngle )
{
	if ( EXERender::bNoClipFaceRender )
		return TRUE;

	EXEPoint	cCheckVertex;
	int			iaTrig[6] = { 0 };

	//Mask World Position
	int iWorldX = ( -psWorldAngle->iX ) & 4095;
	int iWorldY = ( -psWorldAngle->iY ) & 4095;
	int iWorldZ = ( -psWorldAngle->iZ ) & 4095;

	cCheckVertex.SetFromVertex( &sZeroVertex );
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

	int iDist = *(int*)0x82BF34;

	if( ( iPositionZ + iBound ) > 0 && ( iPositionZ < iDist) )
	{
		if( iPositionZ == 0 )
			iPositionZ = 1;

		iPositionZ = abs( iPositionZ );
		int iBounding = ( iBound * (int)READDWORD( 0x82BF0C ) ) / iPositionZ;

		//Check if Rectangle 2D is in View
		if( ( cCheckVertex.sPosi2D.iX + iBounding ) > 0 &&
			( cCheckVertex.sPosi2D.iX - iBounding ) < RESOLUTION_WIDTH &&
			( cCheckVertex.sPosi2D.iY + iBounding ) > 0 &&
			( cCheckVertex.sPosi2D.iY - iBounding ) < RESOLUTION_HEIGHT )
			return TRUE;
	}

	return FALSE;
}

BOOL EXEMesh::Animate( int iFrame, int iAngleX, int iAngleY, int iAngleZ, EXEFrame * psFrameInfo )
{
	EXEMatrixF sMatrixF;
	EXEMatrixI sMatrix;
	EXEMatrixF sM;

	EXEMatrixIdentityF( sMatrixF );

	int iRotate = GetFrameRotate( iFrame );
	int iPosition = GetFramePosition( iFrame );
	int iScale = GetFrameScale( iFrame );



	if( ( !iFrameCount && (iRotateCount > 0 || iPosCount > 0 || iScaleCount > 0 ) ) || ( iFrameCount && (iRotate >= 0 || iPosition >= 0 || iScale >= 0 ) ) )
	{
		if( iFrameCount )
		{
			//Rotate
			if( iRotateCount > 0 && iRotate >= 0 && iRotate < iRotateCount )
				GetFrameRotate( sMatrixF, &sPreviousRotate[iRotate], &sRot[iRotate], iRotate, iFrame, psFrameInfo );
			else
				sMatrixF = EXEMatrixConvertIF( sTmRotate );

			//Scale
			if( iScaleCount > 0 && iScale >= 0 && iScale < iScaleCount )
			{
				EXEMatrixIdentityI( sMatrix );
				GetFrameScale( sMatrix.i[0][0], sMatrix.i[1][1], sMatrix.i[2][2], &sScale[iScale], iFrame );

				sM = EXEMatrixConvertIF( sMatrix );
				sMatrixF = EXEMatrixMultiplyF( sMatrixF, sM );
			}

			//Position
			if( iPosCount > 0 && iPosition >= 0 && iPosition < iPosCount )
				GetFramePosition( sMatrixF.f[3][0], sMatrixF.f[3][1], sMatrixF.f[3][2], &sPos[iPosition], iFrame );
			else
			{
				sMatrixF.f[3][0] = float( px ) / 256;
				sMatrixF.f[3][1] = float( py ) / 256;
				sMatrixF.f[3][2] = float( pz ) / 256;
			}
		}
		else
		{
			//Rotate
			if( iRotateCount > 0 && sRot[iRotateCount-1].iFrame > iFrame )
				GetFrameRotate( sMatrixF, sPreviousRotate, sRot, iRotate, iFrame, psFrameInfo );
			else
				sMatrixF = EXEMatrixConvertIF( sTmRotate );

			//Scale
			if( iScaleCount > 0 && sScale[iScaleCount-1].iFrame > iFrame )
			{
				EXEMatrixIdentityI( sMatrix );
				GetFrameScale( sMatrix.i[0][0], sMatrix.i[1][1], sMatrix.i[2][2], sScale, iFrame );

				sM = EXEMatrixConvertIF( sMatrix );
				sMatrixF = EXEMatrixMultiplyF( sMatrixF, sM );
			}

			//Position
			if( iPosCount > 0 && sPos[iPosCount-1].iFrame > iFrame )
				GetFramePosition( sMatrixF.f[3][0], sMatrixF.f[3][1], sMatrixF.f[3][2], sPos, iFrame );
			else
			{
				sMatrixF.f[3][0] = float( px ) / 256;
				sMatrixF.f[3][1] = float( py ) / 256;
				sMatrixF.f[3][2] = float( pz ) / 256;
			}
		}
	}
	else
	{
		//Parent Mesh?
		if( pParentMesh )
		{
			sMatrix = EXEMatrixMultiplyI( sTm, pParentMesh->sTmInvert );
			sMatrixF = EXEMatrixConvertIF( sMatrix );
		}
		else
			sMatrixF = EXEMatrixConvertIF( sTm );
	}

	//Parent Mesh?
	if( pParentMesh )
		sTmResult = EXEMatrixMultiplyF( sMatrixF, pParentMesh->sTmResult );
	else
		CopyMemory( &sTmResult, &sMatrixF, sizeof( EXEMatrixF ) );

	if( iAngleX == 0 && iAngleY == 0 && iAngleZ == 0 )
	{
		EXEMatrixIdentityI( sLocal );
		sWorld = EXEMatrixConvertFI( sTmResult );
	}
	else
	{
		//Identity Matrix
		EXEMatrixI rX, rY, rZ;
		EXEMatrixIdentityI( rX );
		EXEMatrixIdentityI( rY );
		EXEMatrixIdentityI( rZ );

		//Rotate Matrix
		EXEMatrixRotateXI( rX, iAngleX );
		EXEMatrixRotateZI( rY, iAngleY );
		EXEMatrixRotateYI( rZ, iAngleZ );

		//Multiply Matrix
		if( EXERender::GetRender()->iRenderType )
		{
			sLocal = EXEMatrixMultiplyI( rY, rX );
			sLocal = EXEMatrixMultiplyI( sLocal, rZ );
		}
		else
		{
			sLocal = EXEMatrixMultiplyI( rZ, rX );
			sLocal = EXEMatrixMultiplyI( sLocal, rY );
		}

		sMatrix = EXEMatrixConvertFI( sTmResult );
		sWorld = EXEMatrixMultiplyI( sMatrix, sLocal );
	}

	return TRUE;
}

BOOL EXEMesh::Animate( int iFrame, int iAngleX, int iAngleY, int iAngleZ, EXEModel * pcTalkAnimation, EXEFrame * psFrameInfo )
{
	EXEMatrixF sMatrixF;
	EXEMatrixI sMatrix;
	EXEMatrixF sM;

	EXEMatrixIdentityF( sMatrixF );

	int iRotate = GetFrameRotate( iFrame );
	int iPosition = GetFramePosition( iFrame );
	int iScale = GetFrameScale( iFrame );

	EXEMatrixF * psTalkMatrix = NULL;

	if ( pcTalkAnimation )
	{
		for ( UINT i = 0; i < pcTalkAnimation->uMeshCount; i++ )
		{
			if ( pcTalkAnimation->paMesh[i] )
			{
				if ( STRINGCOMPAREI( pcTalkAnimation->paMesh[i]->szName, szName ) )
				{
					psTalkMatrix = &pcTalkAnimation->paMesh[i]->sTmResult;
					break;
				}
			}
		}
	}

	if ( psTalkMatrix == NULL )
	{
		if ( (!iFrameCount && (iRotateCount > 0 || iPosCount > 0 || iScaleCount > 0)) || (iFrameCount && (iRotate >= 0 || iPosition >= 0 || iScale >= 0)) )
		{
			if ( iFrameCount )
			{
				//Rotate
				if ( iRotateCount > 0 && iRotate >= 0 && iRotate < iRotateCount )
					GetFrameRotate( sMatrixF, &sPreviousRotate[iRotate], &sRot[iRotate], iRotate, iFrame, psFrameInfo );
				else
					sMatrixF = EXEMatrixConvertIF( sTmRotate );

				//Scale
				if ( iScaleCount > 0 && iScale >= 0 && iScale < iScaleCount )
				{
					EXEMatrixIdentityI( sMatrix );
					GetFrameScale( sMatrix.i[0][0], sMatrix.i[1][1], sMatrix.i[2][2], &sScale[iScale], iFrame );

					sM = EXEMatrixConvertIF( sMatrix );
					sMatrixF = EXEMatrixMultiplyF( sMatrixF, sM );
				}

				//Position
				if ( iPosCount > 0 && iPosition >= 0 && iPosition < iPosCount )
					GetFramePosition( sMatrixF.f[3][0], sMatrixF.f[3][1], sMatrixF.f[3][2], &sPos[iPosition], iFrame );
				else
				{
					sMatrixF.f[3][0] = float( px ) / 256;
					sMatrixF.f[3][1] = float( py ) / 256;
					sMatrixF.f[3][2] = float( pz ) / 256;
				}
			}
			else
			{
				//Rotate
				if ( iRotateCount > 0 && sRot[iRotateCount - 1].iFrame > iFrame )
					GetFrameRotate( sMatrixF, sPreviousRotate, sRot, 0, iFrame, psFrameInfo );
				else
					sMatrixF = EXEMatrixConvertIF( sTmRotate );

				//Scale
				if ( iScaleCount > 0 && sScale[iScaleCount - 1].iFrame > iFrame )
				{
					EXEMatrixIdentityI( sMatrix );
					GetFrameScale( sMatrix.i[0][0], sMatrix.i[1][1], sMatrix.i[2][2], sScale, iFrame );

					sM = EXEMatrixConvertIF( sMatrix );
					sMatrixF = EXEMatrixMultiplyF( sMatrixF, sM );
				}

				//Position
				if ( iPosCount > 0 && sPos[iPosCount - 1].iFrame > iFrame )
					GetFramePosition( sMatrixF.f[3][0], sMatrixF.f[3][1], sMatrixF.f[3][2], sPos, iFrame );
				else
				{
					sMatrixF.f[3][0] = float( px ) / 256;
					sMatrixF.f[3][1] = float( py ) / 256;
					sMatrixF.f[3][2] = float( pz ) / 256;
				}
			}
		}
		else
		{
			//Parent Mesh?
			if ( pParentMesh )
			{
				sMatrix = EXEMatrixMultiplyI( sTm, pParentMesh->sTmInvert );
				sMatrixF = EXEMatrixConvertIF( sMatrix );
			}
			else
				sMatrixF = EXEMatrixConvertIF( sTm );
		}
	}
	else
		CopyMemory( &sMatrixF, psTalkMatrix, sizeof( EXEMatrixF ) );

	//Parent Mesh?
	if ( pParentMesh )
		sTmResult = EXEMatrixMultiplyF( sMatrixF, pParentMesh->sTmResult );
	else
		CopyMemory( &sTmResult, &sMatrixF, sizeof( EXEMatrixF ) );

	if ( iAngleX == 0 && iAngleY == 0 && iAngleZ == 0 )
	{
		EXEMatrixIdentityI( sLocal );
		sWorld = EXEMatrixConvertFI( sTmResult );
	}
	else
	{
		//Identity Matrix
		EXEMatrixI rX, rY, rZ;
		EXEMatrixIdentityI( rX );
		EXEMatrixIdentityI( rY );
		EXEMatrixIdentityI( rZ );

		//Rotate Matrix
		EXEMatrixRotateXI( rX, iAngleX );
		EXEMatrixRotateZI( rY, iAngleY );
		EXEMatrixRotateYI( rZ, iAngleZ );

		//Multiply Matrix
		if ( EXERender::GetRender()->iRenderType )
		{
			sLocal = EXEMatrixMultiplyI( rY, rX );
			sLocal = EXEMatrixMultiplyI( sLocal, rZ );
		}
		else
		{
			sLocal = EXEMatrixMultiplyI( rZ, rX );
			sLocal = EXEMatrixMultiplyI( sLocal, rY );
		}

		sMatrix = EXEMatrixConvertFI( sTmResult );
		sWorld = EXEMatrixMultiplyI( sMatrix, sLocal );
	}
	return TRUE;
}

BOOL EXEMesh::AnimateTalk( int iFrame )
{
	EXEMatrixF sMatrixF;
	EXEMatrixI sMatrix;
	EXEMatrixF sM;

	EXEMatrixIdentityF( sMatrixF );

	int iRotate = GetFrameRotate( iFrame );
	int iPosition = GetFramePosition( iFrame );
	int iScale = GetFrameScale( iFrame );

	if ( iFrameCount && (iRotate >= 0 || iPosition >= 0 || iScale >= 0) )
	{
		//Rotate
		if ( iRotateCount > 0 && iRotate >= 0 && iRotate < iRotateCount )
			GetFrameRotate( sMatrixF, &sPreviousRotate[iRotate], &sRot[iRotate], iRotate, iFrame );
		else
			sMatrixF = EXEMatrixConvertIF( sTmRotate );

		//Scale
		if ( iScaleCount > 0 && iScale >= 0 && iScale < iScaleCount )
		{
			EXEMatrixIdentityI( sMatrix );
			GetFrameScale( sMatrix.i[0][0], sMatrix.i[1][1], sMatrix.i[2][2], &sScale[iScale], iFrame );

			sM = EXEMatrixConvertIF( sMatrix );
			sMatrixF = EXEMatrixMultiplyF( sMatrixF, sM );
		}

		//Position
		if ( iPosCount > 0 && iPosition >= 0 && iPosition < iPosCount )
			GetFramePosition( sMatrixF.f[3][0], sMatrixF.f[3][1], sMatrixF.f[3][2], &sPos[iPosition], iFrame );
		else
		{
			sMatrixF.f[3][0] = float( px ) / 256;
			sMatrixF.f[3][1] = float( py ) / 256;
			sMatrixF.f[3][2] = float( pz ) / 256;
		}

		CopyMemory( &sTmResult, &sMatrixF, sizeof( EXEMatrixF ) );
	}
	else
		EXEMatrixIdentityF( sTmResult );

	return TRUE;
}

BOOL EXEMesh::SetPosition( Point3D * psPosition, Point3D * psAngle )
{
	sPosition.iX = psPosition->iX;
	sPosition.iY = psPosition->iY;
	sPosition.iZ = psPosition->iZ;

	sAngle.iX = psAngle->iX;
	sAngle.iY = psAngle->iY;
	sAngle.iZ = psAngle->iZ;

	return TRUE;
}

void EXEMesh::ClearVertex()
{
	for ( int i = 0; i < iVertexCount; i++ )
	{
		pMeshVertex[i].sPosition2 = Point3D( 0, 0, 0 );
	}

	dwHeader &= (~0x80000000);
}

int EXEMesh::GetFrameRotate( int iFrame )
{
	if( iFrameCount > 0 )
	{
		for( int i = 0; i < iFrameCount; i++ )
		{
			if( saRotateFrame[i].iPositionCount > 0 && saRotateFrame[i].iStartFrame <= iFrame && saRotateFrame[i].iEndFrame > iFrame )
				return saRotateFrame[i].iPositionID;
		}
	}

	return -1;
}

int EXEMesh::GetFrameRotate( EXEMatrixF & sMatrix, EXEMatrixF * prevMatrix, EXEAnimationRotation * pRot, int iRotate, int iFrame, EXEFrame * psFrameInfo )
{
	float ax, ay, az, aw;
	float bx, by, bz, bw;
	int s, e, ch, sh;
	float alpha;

	int iCount = 0;

	if ( iRotateCount > 0 )
	{
		if ( pRot[iCount].iFrame > iFrame )
			return iFrame;

		while ( TRUE )
		{
			s = pRot[iCount].iFrame;
			e = pRot[iCount + 1].iFrame;

			if ( s <= iFrame && e > iFrame )
				break;

			iCount++;

			if ( iCount >= 0 && (iCount + iRotate) >= iRotateCount )
				return iFrame;
		}
	}

	ch = e - s;
	sh = iFrame - s;

	alpha = ((float)sh / (float)ch);

	ax = pRot[iCount].fX;
	ay = pRot[iCount].fY;
	az = pRot[iCount].fZ;
	aw = pRot[iCount].fW;

	bx = pRot[iCount + 1].fX;
	by = pRot[iCount + 1].fY;
	bz = pRot[iCount + 1].fZ;
	bw = pRot[iCount + 1].fW;


	if( psFrameInfo && e > (int)psFrameInfo->iEndFrame )
	{
		if ( iRotateCount > 0 && iCount >= 0 && (iCount + iRotate) < iRotateCount )
			CopyMemory( &sMatrix, &prevMatrix[iCount], sizeof(EXEMatrixF) );

		return s;
	}

	Vector4DF Q( 0, 0, 0, 0 );
	Vector4DF v0( 0, 0, 0, 0 );
	Vector4DF v1( bx, by, bz, bw );

	Q = EXEQuaternionSlerp( v0, v1, alpha );

	EXEMatrixFromQuaterion( sMatrix, Q.fX, Q.fY, Q.fZ, Q.fW );

	if ( iRotateCount > 0 && iCount >= 0 && (iCount + iRotate) < iRotateCount )
		sMatrix = EXEMatrixMultiplyF( prevMatrix[iCount], sMatrix );

	return s;
}

int EXEMesh::GetFramePosition( int iFrame )
{
	if( iFrameCount > 0 )
	{
		for( int i = 0; i < iFrameCount; i++ )
		{
			if( saPosFrame[i].iPositionCount > 0 && saPosFrame[i].iStartFrame <= iFrame && saPosFrame[i].iEndFrame > iFrame )
				return saPosFrame[i].iPositionID;
		}
	}

	return -1;
}

int EXEMesh::GetFramePosition( float & x, float & y, float & z, EXEAnimationPosition * pPos, int iFrame )
{
	int s, e, ch, sh;
	int	iCount = 0;
	float alpha;

	if( pPos[iCount].iFrame > iFrame )
		return iFrame;

	while( TRUE )
	{
		s = pPos[iCount].iFrame;
		e = pPos[iCount + 1].iFrame;

		if( s <= iFrame && e> iFrame ) 
			break;

		iCount++;
	}

	ch = e - s;
	sh = iFrame - s;

	alpha = float( sh ) / float( ch );

	x = pPos[iCount].fX + ( ( pPos[iCount + 1].fX - pPos[iCount].fX ) * alpha );
	y = pPos[iCount].fY + ( ( pPos[iCount + 1].fY - pPos[iCount].fY ) * alpha );
	z = pPos[iCount].fZ + ( ( pPos[iCount + 1].fZ - pPos[iCount].fZ ) * alpha );

	return -1;
}

int EXEMesh::GetFrameScale( int iFrame )
{
	if( iFrameCount > 0 )
	{
		for( int i = 0; i < iFrameCount; i++ )
		{
			if( saScaleFrame[i].iPositionCount > 0 && saScaleFrame[i].iStartFrame <= iFrame && saScaleFrame[i].iEndFrame > iFrame )
				return saScaleFrame[i].iPositionID;
		}
	}

	return -1;
}

int EXEMesh::GetFrameScale( int & x, int & y, int & z, EXEAnimationScale * pScale, int iFrame )
{
	int s, e, ch, sh;
	int alpha;

	int iCount = 0;

	if( pScale[iCount].iFrame > iFrame )
		return iFrame;

	while( TRUE )
	{
		s = pScale[iCount].iFrame;
		e = pScale[iCount + 1].iFrame;

		if( s <= iFrame && e > iFrame ) 
			break;

		iCount++;
	}

	ch = e - s;
	sh = iFrame - s;
	alpha = ( sh << 8 ) / ch;

	x = pScale[iCount].iX + ( ( ( pScale[iCount + 1].iX - pScale[iCount].iX ) * alpha ) >> 8 );
	y = pScale[iCount].iY + ( ( ( pScale[iCount + 1].iY - pScale[iCount].iY ) * alpha ) >> 8 );
	z = pScale[iCount].iZ + ( ( ( pScale[iCount + 1].iZ - pScale[iCount].iZ ) * alpha ) >> 8 );

	return s;
}

NAKED BOOL EXEMesh::ReformTexture( int iType, int iMaterialID, int iMaterialCount, Point3D * psPosi, Point3D * psSize )
{
	JMP( pfnReformTexture );
}

BOOL EXEMesh::PreRender( Point3D * psCamera, Point3D * psAngle )
{
	int iX = ( sPosition.iX - psCamera->iX ) >> 8;
	int iY = ( sPosition.iY - psCamera->iY ) >> 8;
	int iZ = ( sPosition.iZ - psCamera->iZ ) >> 8;

	if( IsInView( iX, iY, iZ, &sAngle, psAngle ) )
	{
		if( READDWORD( 0x2619824 ) )
		{
			sCameraPosition.iX = psCamera->iX;
			sCameraPosition.iY = psCamera->iY;
			sCameraPosition.iZ = psCamera->iZ;
		}
		else
		{
			sCameraPosition.iX = psCamera->iX - sPosition.iX;
			sCameraPosition.iY = psCamera->iY - sPosition.iY;
			sCameraPosition.iZ = psCamera->iZ - sPosition.iZ;
		}

		EXERender::GetRender()->SetCameraPosition( sCameraPosition.iX, sCameraPosition.iY, sCameraPosition.iZ, psAngle->iX, psAngle->iY, psAngle->iZ );

		WorldForm();

		return TRUE;
	}

	return FALSE;
}

int EXEMesh::Render( Point3D * psCamera, Point3D * psAngle )
{
	int iX = ( sPosition.iX - psCamera->iX ) >> 8;
	int iY = ( sPosition.iY - psCamera->iY ) >> 8;
	int iZ = ( sPosition.iZ - psCamera->iZ ) >> 8;

	if( IsInView( iX, iY, iZ, &sAngle, psAngle ) )
	{
		if( *(BOOL*)( 0x2619824 ) )
		{
			sCameraPosition.iX = psCamera->iX;
			sCameraPosition.iY = psCamera->iY;
			sCameraPosition.iZ = psCamera->iZ;
		}
		else
		{
			sCameraPosition.iX = psCamera->iX - sPosition.iX;
			sCameraPosition.iY = psCamera->iY - sPosition.iY;
			sCameraPosition.iZ = psCamera->iZ - sPosition.iZ;
		}

		WorldForm();
		EXERender::GetRender()->SetCameraPosition( sCameraPosition.iX, sCameraPosition.iY, sCameraPosition.iZ, psAngle->iX, psAngle->iY, psAngle->iZ );

		int i = 0;

		i = FinalRender();

		return i;
	}

	return FALSE;
}

NAKED int EXEMesh::FinalRender()
{
	JMP( pfnFinalRender );
}

BOOL EXEMesh::WorldForm()
{
	EXEVertex * psVertex;
	EXEMatrixI * psMatrix;

	if( pVertexBoneList )
	{
		for( int i = 0; i < iVertexCount; i++ )
		{
			psVertex = &pMeshVertex[i];
			psMatrix = &pVertexBoneList[i]->GetWorldMatrix();

			//Update Main Position
			int rx = psVertex->sPosition.iX * psMatrix->i[0][0] + psVertex->sPosition.iY * psMatrix->i[1][0] + psVertex->sPosition.iZ * psMatrix->i[2][0];
			int ry = psVertex->sPosition.iX * psMatrix->i[0][1] + psVertex->sPosition.iY * psMatrix->i[1][1] + psVertex->sPosition.iZ * psMatrix->i[2][1];
			int rz = psVertex->sPosition.iX * psMatrix->i[0][2] + psVertex->sPosition.iY * psMatrix->i[1][2] + psVertex->sPosition.iZ * psMatrix->i[2][2];

			psWorldVertex[i].sPosition.iX = ( rx >> 8 ) + psMatrix->i[3][0];
			psWorldVertex[i].sPosition.iZ = ( ry >> 8 ) + psMatrix->i[3][1];
			psWorldVertex[i].sPosition.iY = ( rz >> 8 ) + psMatrix->i[3][2];

			if( dwHeader & 0x80000000 )
				psMatrix = &pVertexBoneList[i]->GetWorldMatrix();
			else
				psMatrix = &pVertexBoneList[i]->sLocal;

			//Update Unk Position
			rx = psVertex->sPosition2.iX * psMatrix->i[0][0] + psVertex->sPosition2.iY * psMatrix->i[1][0] + psVertex->sPosition2.iZ * psMatrix->i[2][0];
			ry = psVertex->sPosition2.iX * psMatrix->i[0][1] + psVertex->sPosition2.iY * psMatrix->i[1][1] + psVertex->sPosition2.iZ * psMatrix->i[2][1];
			rz = psVertex->sPosition2.iX * psMatrix->i[0][2] + psVertex->sPosition2.iY * psMatrix->i[1][2] + psVertex->sPosition2.iZ * psMatrix->i[2][2];
		
			psWorldVertex[i].sPosition2.iX = ( rx >> 8 ) + psMatrix->i[3][0];
			psWorldVertex[i].sPosition2.iZ = ( ry >> 8 ) + psMatrix->i[3][1];
			psWorldVertex[i].sPosition2.iY = ( rz >> 8 ) + psMatrix->i[3][2];
		}
	}
	else
	{
		for( int i = 0; i < iVertexCount; i++ )
		{
			psVertex = &pMeshVertex[i];

			//Update Main Position
			int rx = psVertex->sPosition.iX * sWorld.i[0][0] + psVertex->sPosition.iY * sWorld.i[1][0] + psVertex->sPosition.iZ * sWorld.i[2][0];
			int ry = psVertex->sPosition.iX * sWorld.i[0][1] + psVertex->sPosition.iY * sWorld.i[1][1] + psVertex->sPosition.iZ * sWorld.i[2][1];
			int rz = psVertex->sPosition.iX * sWorld.i[0][2] + psVertex->sPosition.iY * sWorld.i[1][2] + psVertex->sPosition.iZ * sWorld.i[2][2];

			psWorldVertex[i].sPosition.iX = ( rx >> 8 ) + sWorld.i[3][0];
			psWorldVertex[i].sPosition.iZ = ( ry >> 8 ) + sWorld.i[3][1];
			psWorldVertex[i].sPosition.iY = ( rz >> 8 ) + sWorld.i[3][2];

			if( dwHeader & 0x80000000 )
				psMatrix = &sWorld;
			else
				psMatrix = &sLocal;

			//Update Unk Position
			rx = psVertex->sPosition2.iX * psMatrix->i[0][0] + psVertex->sPosition2.iY * psMatrix->i[1][0] + psVertex->sPosition2.iZ * psMatrix->i[2][0];
			ry = psVertex->sPosition2.iX * psMatrix->i[0][1] + psVertex->sPosition2.iY * psMatrix->i[1][1] + psVertex->sPosition2.iZ * psMatrix->i[2][1];
			rz = psVertex->sPosition2.iX * psMatrix->i[0][2] + psVertex->sPosition2.iY * psMatrix->i[1][2] + psVertex->sPosition2.iZ * psMatrix->i[2][2];

			psWorldVertex[i].sPosition2.iX = ( rx >> 8 ) + psMatrix->i[3][0];
			psWorldVertex[i].sPosition2.iZ = ( ry >> 8 ) + psMatrix->i[3][1];
			psWorldVertex[i].sPosition2.iY = ( rz >> 8 ) + psMatrix->i[3][2];
		}
	}

	return TRUE;
}

void EXEMesh::UpdateVertexPhysique()
{
	for ( int i = 0; i < iVertexCount; i++ )
	{
		auto pcBone = pVertexBoneList[i];
		{
			EXEMatrixI sInversePT;
			CALL_WITH_ARG2( 0x0048BA60, (DWORD)&sInversePT, (DWORD)&pcBone->sTm );

			Point3D sNewVertex;

			EXEVertex & sVertex = pMeshVertex[i];

			sNewVertex.iX = pMeshVertex[i].sPosition.iX * sInversePT.i[0][0] +
							pMeshVertex[i].sPosition.iY * sInversePT.i[1][0] +
							pMeshVertex[i].sPosition.iZ * sInversePT.i[2][0];

			sNewVertex.iY = pMeshVertex[i].sPosition.iX * sInversePT.i[0][1] +
							pMeshVertex[i].sPosition.iY * sInversePT.i[1][1] +
							pMeshVertex[i].sPosition.iZ * sInversePT.i[2][1];

			sNewVertex.iZ = pMeshVertex[i].sPosition.iX * sInversePT.i[0][2] +
							pMeshVertex[i].sPosition.iY * sInversePT.i[1][2] +
							pMeshVertex[i].sPosition.iZ * sInversePT.i[2][2];


			pMeshVertex[i].sPosition.iX = (sNewVertex.iX >> 8) + sInversePT.i[3][0];
			pMeshVertex[i].sPosition.iY = (sNewVertex.iY >> 8) + sInversePT.i[3][1];
			pMeshVertex[i].sPosition.iZ = (sNewVertex.iZ >> 8) + sInversePT.i[3][2];


			sNewVertex.iX = pMeshVertex[i].sPosition2.iX * sInversePT.i[0][0] +
							pMeshVertex[i].sPosition2.iY * sInversePT.i[1][0] +
							pMeshVertex[i].sPosition2.iZ * sInversePT.i[2][0];

			sNewVertex.iY = pMeshVertex[i].sPosition2.iX * sInversePT.i[0][1] +
							pMeshVertex[i].sPosition2.iY * sInversePT.i[1][1] +
							pMeshVertex[i].sPosition2.iZ * sInversePT.i[2][1];

			sNewVertex.iZ = pMeshVertex[i].sPosition2.iX * sInversePT.i[0][2] +
							pMeshVertex[i].sPosition2.iY * sInversePT.i[1][2] +
							pMeshVertex[i].sPosition2.iZ * sInversePT.i[2][2];


			pMeshVertex[i].sPosition2.iX = (sNewVertex.iX >> 8) + sInversePT.i[3][0];
			pMeshVertex[i].sPosition2.iY = (sNewVertex.iY >> 8) + sInversePT.i[3][1];
			pMeshVertex[i].sPosition2.iZ = (sNewVertex.iZ >> 8) + sInversePT.i[3][2];
		}
	}
}

NAKED BOOL EXEMesh::LoadFromHandle( HANDLE hFile, EXEModel * pcBone )
{
	JMP( 0x00477110 );
}

FILE * EXEMesh::ReadGeometry( FILE * fp, EXEMesh * pcOut, char * pszNode )
{
	static char szLine[256];
	static char szDecode[256];

	static char szBuffer[256];

	int iVertexCount = 0;
	int iFaceCount = 0;
	int iTVertexCount = 0;

	BOOL bLightMap = FALSE;

	while ( !feof( fp ) )
	{
		if ( fgets( szLine, 255, fp ) == NULL )
		{
			fclose( fp );
			return NULL;
		}

		char * p = (char*)GETWORDPT( szDecode, szLine );

		if ( STRINGCOMPARE( szDecode, "*NODE_NAME" ) )
		{
			p = GETWORDVALUEPT( szBuffer, p );
			
			STRINGCOPY( pcOut->szName, szBuffer );
			
			if ( pszNode && pszNode[0] != 0 && STRINGCOMPAREI( szBuffer, pszNode ) == FALSE )
			{
				pcOut->iFaceCount = 0;
				pcOut->iVertexCount = 0;

				return fp;
			}
		}

		if ( STRINGCOMPARE( szDecode, "*NODE_PARENT" ) )
		{
			p = GETWORDVALUEPT( szBuffer, p );

			STRINGCOPY( pcOut->szParentName, szBuffer );
		}

		if ( STRINGCOMPARE( szDecode, "*MESH_NUMVERTEX" ) )
		{
			p = GETWORDVALUEPT( szBuffer, p );

			iVertexCount = atoi( szBuffer );
		}

		if ( STRINGCOMPARE( szDecode, "*MESH_NUMFACES" ) )
		{
			p = GETWORDVALUEPT( szBuffer, p );

			iFaceCount = atoi( szBuffer );
		}

		if ( STRINGCOMPARE( szDecode, "*MESH_NUMTVERTEX" ) )
		{
			p = GETWORDVALUEPT( szBuffer, p );

			iTVertexCount = atoi( szBuffer );

			if ( bLightMap )
			{

			}
		}

	}

	return fp;
}

int EXEMesh::GetSaveSize( bool bUseOldSMD )
{
	int iLength = 0;

	iLength += 0x8BC;
	iLength += sizeof( EXEVertex ) * iVertexCount;
	iLength += sizeof( EXEFace ) * iFaceCount;
	iLength += sizeof( EXEFacePosition ) * iTextureLinkCount;
			
	iLength += sizeof( EXEAnimationRotation ) * iRotateCount;
	iLength += sizeof( EXEAnimationPosition ) * iPosCount;
	iLength += sizeof( EXEAnimationScale ) * iScaleCount;
	iLength += sizeof( EXEMatrixF ) * iRotateCount;

	if ( pVertexBoneList && pVertexBoneList[0] )
		iLength += iVertexCount * 32;

    if ( bUseInfluences && (bUseOldSMD == false) )
        iLength += (int)vInfluences.size() * sizeof( EXEMeshInfluenceVertex );

	if ( (vFloatVertices.size() > 0) && (bUseOldSMD == false) )
	{
		iLength -= sizeof( EXEVertex ) * iVertexCount;

		iLength += sizeof( EXEVertexTangentF ) * vFloatVertices.size();
	}

	return iLength;
}

void EXEMesh::SortVertices()
{
    int cnt1, cnt2, cnt3, cnt4;
    int x, y, z;

	bool bHasFloatVertices = vFloatVertices.size() == (size_t)iVertexCount;

    for ( cnt1 = 1; cnt1 < iVertexCount; cnt1++ )
    {
        for ( cnt2 = 0; cnt2 < cnt1; cnt2++ )
        {
            x = abs( pMeshVertex[cnt1].sPosition.iX - pMeshVertex[cnt2].sPosition.iX );
            y = abs( pMeshVertex[cnt1].sPosition.iY - pMeshVertex[cnt2].sPosition.iY );
            z = abs( pMeshVertex[cnt1].sPosition.iZ - pMeshVertex[cnt2].sPosition.iZ );

            if ( x == 0 && y == 0 && z == 0 && cnt1 < iVertexCount )
            {
                for ( cnt3 = 0; cnt3 < iFaceCount; cnt3++ )
                {

                    if ( pMeshFace[cnt3].uVertex1 == cnt1 ) pMeshFace[cnt3].uVertex1 = cnt2;
                    if ( pMeshFace[cnt3].uVertex2 == cnt1 ) pMeshFace[cnt3].uVertex2 = cnt2;
                    if ( pMeshFace[cnt3].uVertex3 == cnt1 ) pMeshFace[cnt3].uVertex3 = cnt2;

                    if ( pMeshFace[cnt3].uVertex1 > cnt1 ) pMeshFace[cnt3].uVertex1--;
                    if ( pMeshFace[cnt3].uVertex2 > cnt1 ) pMeshFace[cnt3].uVertex2--;
                    if ( pMeshFace[cnt3].uVertex3 > cnt1 ) pMeshFace[cnt3].uVertex3--;

                }

                iVertexCount--;
                if ( cnt1 != iVertexCount )
                {
					for ( cnt4 = cnt1; cnt4 < iVertexCount; cnt4++ )
					{
						memcpy( &pMeshVertex[cnt4], &pMeshVertex[cnt4 + 1], sizeof( EXEVertex ) );

						if ( bHasFloatVertices )
							vFloatVertices[cnt4] = vFloatVertices[cnt4 + 1];
					}

					if ( bHasFloatVertices )
						vFloatVertices.resize( iVertexCount );

                    if ( pVertexBoneList )
                    {
						for ( cnt4 = cnt1; cnt4 < iVertexCount; cnt4++ )
						{
							pVertexBoneList[cnt4] = pVertexBoneList[cnt4 + 1];

							if ( bUseInfluences )
								vInfluences[cnt4] = vInfluences[cnt4 + 1];
						}

						if ( bUseInfluences )
							vInfluences.resize( iVertexCount );
                    }
                }
            }
        }
    }


    short * pUseVertex = new short[iVertexCount];

    ZeroMemory( pUseVertex, sizeof( short ) * iVertexCount );
    for ( cnt1 = 0; cnt1 < iFaceCount; cnt1++ )
    {
        pUseVertex[pMeshFace[cnt1].uVertex1]++;
        pUseVertex[pMeshFace[cnt1].uVertex2]++;
        pUseVertex[pMeshFace[cnt1].uVertex3]++;

        if ( pMeshFace[cnt1].uVertex1 >= iVertexCount ||
			 pMeshFace[cnt1].uVertex2 >= iVertexCount ||
			 pMeshFace[cnt1].uVertex3 >= iVertexCount )
        {

            cnt1 = cnt1;

        }

    }

    for ( cnt1 = 0; cnt1 < iVertexCount; cnt1++ )
    {
        if ( pUseVertex[cnt1] == 0 )
        {
            for ( cnt3 = 0; cnt3 < iFaceCount; cnt3++ )
            {
                if ( pMeshFace[cnt3].uVertex1 > cnt1 ) pMeshFace[cnt3].uVertex1--;
                if ( pMeshFace[cnt3].uVertex2 > cnt1 ) pMeshFace[cnt3].uVertex2--;
                if ( pMeshFace[cnt3].uVertex3 > cnt1 ) pMeshFace[cnt3].uVertex3--;
            }
            iVertexCount--;
            if ( cnt1 != iVertexCount )
            {
				for ( cnt4 = cnt1; cnt4 < iVertexCount; cnt4++ )
				{
					memcpy( &pMeshVertex[cnt4], &pMeshVertex[cnt4 + 1], sizeof( EXEVertex ) );
				
					if ( bHasFloatVertices )
						vFloatVertices[cnt4] = vFloatVertices[cnt4 + 1];
				}

				if ( bHasFloatVertices )
					vFloatVertices.resize( iVertexCount );

                if ( pVertexBoneList )
                {
					for ( cnt4 = cnt1; cnt4 < iVertexCount; cnt4++ )
					{
						pVertexBoneList[cnt4] = pVertexBoneList[cnt4 + 1];

                        if ( bUseInfluences )
                            vInfluences[cnt4] = vInfluences[cnt4 + 1];
					}

                    if ( bUseInfluences )
                        vInfluences.resize( iVertexCount );
                }
            }
        }
    }

    delete pUseVertex;
}

NAKED void EXEMesh::AddVertex( int iX, int iY, int iZ )
{
	JMP( 0x00473F20 );
}

void EXEMesh::ComputeNormals()
{
	int cnt;
	float i, j, k, d;
	EXEVertex * p1, * p2, * p3;
	int * VNormalCnt;

	VNormalCnt = new int[iVertexCount];

	for ( cnt = 0; cnt < iVertexCount; cnt++ )
	{
		pMeshVertex[cnt].sPosition2.iX = 0;
		pMeshVertex[cnt].sPosition2.iY = 256;
		pMeshVertex[cnt].sPosition2.iZ = 0;
		VNormalCnt[cnt] = 0;

        if ( bUseInfluences && vFloatVertices.size() > 0 )
        {
            vFloatVertices[cnt].sPosition2 = X3D::Vector3( 0.0f, 0.0f, 0.0f );
            vFloatVertices[cnt].sTangent = X3D::Vector3( 0.0f, 0.0f, 0.0f );
            vFloatVertices[cnt].sBitangent = X3D::Vector3( 0.0f, 0.0f, 0.0f );
        }
	}

	for ( cnt = 0; cnt < iFaceCount; cnt++ )
	{

		p1 = &pMeshVertex[pMeshFace[cnt].uVertex1];
		p2 = &pMeshVertex[pMeshFace[cnt].uVertex2];
		p3 = &pMeshVertex[pMeshFace[cnt].uVertex3];


		i = (float)(((p2->sPosition.iY - p1->sPosition.iY) * (p3->sPosition.iZ - p1->sPosition.iZ)) - ((p2->sPosition.iZ - p1->sPosition.iZ) * (p3->sPosition.iY - p1->sPosition.iY)));
		j = (float)(((p2->sPosition.iZ - p1->sPosition.iZ) * (p3->sPosition.iX - p1->sPosition.iX)) - ((p2->sPosition.iX - p1->sPosition.iX) * (p3->sPosition.iZ - p1->sPosition.iZ)));
		k = (float)(((p2->sPosition.iX - p1->sPosition.iX) * (p3->sPosition.iY - p1->sPosition.iY)) - ((p2->sPosition.iY - p1->sPosition.iY) * (p3->sPosition.iX - p1->sPosition.iX)));

		d = (float)sqrt( (double)((i * i) + (j * j) + (k * k)) );
		i = (float)(i / d * 32767);
		j = (float)(j / d * 32767);
		k = (float)(k / d * 32767);

		p1->sPosition2.iX += (int)i; p1->sPosition2.iY += (int)j; p1->sPosition2.iZ += (int)k;
		VNormalCnt[pMeshFace[cnt].uVertex1]++;

		p2->sPosition2.iX += (int)i; p2->sPosition2.iY += (int)j; p2->sPosition2.iZ += (int)k;
		VNormalCnt[pMeshFace[cnt].uVertex2]++;

		p3->sPosition2.iX += (int)i; p3->sPosition2.iY += (int)j; p3->sPosition2.iZ += (int)k;
		VNormalCnt[pMeshFace[cnt].uVertex3]++;

		//Compute Tangents and Bitangents
		if ( bUseInfluences && vFloatVertices.size() > 0 )
		{
			if ( pMeshFace[cnt].pcTextureLink )
			{
				auto pTextureLink = (EXETextureLink*)pMeshFace[cnt].pcTextureLink;

				auto sVertex0 = vFloatVertices[pMeshFace[cnt].uVertex1].sPosition;
				auto sVertex1 = vFloatVertices[pMeshFace[cnt].uVertex2].sPosition;
				auto sVertex2 = vFloatVertices[pMeshFace[cnt].uVertex3].sPosition;

				auto uv0 = X3D::Vector2( pTextureLink->u[0], pTextureLink->v[0] );
				auto uv1 = X3D::Vector2( pTextureLink->u[1], pTextureLink->v[1] );
				auto uv2 = X3D::Vector2( pTextureLink->u[2], pTextureLink->v[2] );


				auto vDeltaPosition1 = sVertex0 - sVertex1;
				auto vDelttaPosition2 = sVertex0 - sVertex2;

				auto sDeltaUV1 = uv0 - uv1;
				auto sDeltaUV2 = uv0 - uv2;

                float r = 1.0f / (sDeltaUV1.x * sDeltaUV2.y - sDeltaUV1.y * sDeltaUV2.x);
                auto vTangent = (vDeltaPosition1 * sDeltaUV2.y - vDelttaPosition2 * sDeltaUV1.y) * r;
                auto vBitangent = (vDelttaPosition2 * sDeltaUV1.x - vDeltaPosition1 * sDeltaUV2.x) * r;

                vFloatVertices[pMeshFace[cnt].uVertex1].sTangent += vTangent;
                vFloatVertices[pMeshFace[cnt].uVertex2].sTangent += vTangent;
                vFloatVertices[pMeshFace[cnt].uVertex3].sTangent += vTangent;

                vFloatVertices[pMeshFace[cnt].uVertex1].sBitangent += vBitangent;
                vFloatVertices[pMeshFace[cnt].uVertex2].sBitangent += vBitangent;
                vFloatVertices[pMeshFace[cnt].uVertex3].sBitangent += vBitangent;
			}
		}
	}

	for ( cnt = 0; cnt < iVertexCount; cnt++ )
	{
		if ( VNormalCnt[cnt] )
		{
			pMeshVertex[cnt].sPosition2.iX /= VNormalCnt[cnt];
			pMeshVertex[cnt].sPosition2.iY /= VNormalCnt[cnt];
			pMeshVertex[cnt].sPosition2.iZ /= VNormalCnt[cnt];

			pMeshVertex[cnt].sPosition2.iX += pMeshVertex[cnt].sPosition.iX;
			pMeshVertex[cnt].sPosition2.iY += pMeshVertex[cnt].sPosition.iY;
			pMeshVertex[cnt].sPosition2.iZ += pMeshVertex[cnt].sPosition.iZ;

			if ( bUseInfluences && vFloatVertices.size() > 0 )
			{
				vFloatVertices[cnt].sPosition2 = X3D::Vector3( (float)pMeshVertex[cnt].sPosition2.iX / 256.0f, (float)pMeshVertex[cnt].sPosition2.iY / 256.0f, (float)pMeshVertex[cnt].sPosition2.iZ / 256.0f );

                vFloatVertices[cnt].sTangent.x /= (float)VNormalCnt[cnt];
                vFloatVertices[cnt].sTangent.y /= (float)VNormalCnt[cnt];
                vFloatVertices[cnt].sTangent.z /= (float)VNormalCnt[cnt];

                vFloatVertices[cnt].sBitangent.x /= (float)VNormalCnt[cnt];
                vFloatVertices[cnt].sBitangent.y /= (float)VNormalCnt[cnt];
                vFloatVertices[cnt].sBitangent.z /= (float)VNormalCnt[cnt];
			}
		}
	}

	delete VNormalCnt;
}

void EXEMesh::ComputeTransformation()
{
    EXEMatrixI q;

	if ( pParentMesh == nullptr )
	{
		int scale = (sx + sy + sz) / 3;
		if ( scale != 0 )
		{
			sTm._11 = (sTm._11 << 8) / scale;
			sTm._12 = (sTm._12 << 8) / scale;
			sTm._13 = (sTm._13 << 8) / scale;
			sTm._21 = (sTm._21 << 8) / scale;
			sTm._22 = (sTm._22 << 8) / scale;
			sTm._23 = (sTm._23 << 8) / scale;
			sTm._31 = (sTm._31 << 8) / scale;
			sTm._32 = (sTm._32 << 8) / scale;
			sTm._33 = (sTm._33 << 8) / scale;
		}
	}
	else
	{
		int scale = (pParentMesh->sx + pParentMesh->sy + pParentMesh->sz) / 3;

		if ( scale != 0 )
		{
			sTm._11 = (sTm._11 << 8) / scale;
			sTm._12 = (sTm._12 << 8) / scale;
			sTm._13 = (sTm._13 << 8) / scale;
			sTm._21 = (sTm._21 << 8) / scale;
			sTm._22 = (sTm._22 << 8) / scale;
			sTm._23 = (sTm._23 << 8) / scale;
			sTm._31 = (sTm._31 << 8) / scale;
			sTm._32 = (sTm._32 << 8) / scale;
			sTm._33 = (sTm._33 << 8) / scale;
		}
	}

	auto ConvertMatrixToFloat = []( const EXEMatrixI b )
	{
		Math::Matrix4 m;

		for ( int i = 0; i < 4; i++ )
			for ( int j = 0; j < 4; j++ )
				m.m[i][j] = (float)b.i[i][j] / 256.0f;

		return m;
	};

	
	if ( bUseInfluences )
	{
		if ( pParentMesh == nullptr )
		{
			float scale = ((float)(sx + sy + sz) / 3.0f) / 256.0f;
			if ( scale != 0 )
			{
				sTransformationMatrixF._11 = sTransformationMatrixF._11 / scale;
				sTransformationMatrixF._12 = sTransformationMatrixF._12 / scale;
				sTransformationMatrixF._13 = sTransformationMatrixF._13 / scale;
				sTransformationMatrixF._21 = sTransformationMatrixF._21 / scale;
				sTransformationMatrixF._22 = sTransformationMatrixF._22 / scale;
				sTransformationMatrixF._23 = sTransformationMatrixF._23 / scale;
				sTransformationMatrixF._31 = sTransformationMatrixF._31 / scale;
				sTransformationMatrixF._32 = sTransformationMatrixF._32 / scale;
				sTransformationMatrixF._33 = sTransformationMatrixF._33 / scale;
			}
		}
		else
		{
			float scale = ((float)(pParentMesh->sx + pParentMesh->sy + pParentMesh->sz) / 3.0f) / 256.0f;

			if ( scale != 0 )
			{
				sTransformationMatrixF._11 = sTransformationMatrixF._11 / scale;
				sTransformationMatrixF._12 = sTransformationMatrixF._12 / scale;
				sTransformationMatrixF._13 = sTransformationMatrixF._13 / scale;
				sTransformationMatrixF._21 = sTransformationMatrixF._21 / scale;
				sTransformationMatrixF._22 = sTransformationMatrixF._22 / scale;
				sTransformationMatrixF._23 = sTransformationMatrixF._23 / scale;
				sTransformationMatrixF._31 = sTransformationMatrixF._31 / scale;
				sTransformationMatrixF._32 = sTransformationMatrixF._32 / scale;
				sTransformationMatrixF._33 = sTransformationMatrixF._33 / scale;
			}
		}
	}

	CALL_WITH_ARG2( 0x0048BA60, (DWORD)&q, (DWORD)&sTm );
    memcpy( &sTmInvert, &q, sizeof( EXEMatrixI ) );


    for ( int i = 0; i < iVertexCount; i++ )
    {
        Point3D sNewVertex;

        EXEVertex & sVertex = pMeshVertex[i];

        sNewVertex.iX = pMeshVertex[i].sPosition.iX * sTmInvert.i[0][0] +
            pMeshVertex[i].sPosition.iY * sTmInvert.i[1][0] +
            pMeshVertex[i].sPosition.iZ * sTmInvert.i[2][0];

        sNewVertex.iY = pMeshVertex[i].sPosition.iX * sTmInvert.i[0][1] +
            pMeshVertex[i].sPosition.iY * sTmInvert.i[1][1] +
            pMeshVertex[i].sPosition.iZ * sTmInvert.i[2][1];

        sNewVertex.iZ = pMeshVertex[i].sPosition.iX * sTmInvert.i[0][2] +
            pMeshVertex[i].sPosition.iY * sTmInvert.i[1][2] +
            pMeshVertex[i].sPosition.iZ * sTmInvert.i[2][2];


        pMeshVertex[i].sPosition.iX = (sNewVertex.iX >> 8) + sTmInvert.i[3][0];
        pMeshVertex[i].sPosition.iY = (sNewVertex.iY >> 8) + sTmInvert.i[3][1];
        pMeshVertex[i].sPosition.iZ = (sNewVertex.iZ >> 8) + sTmInvert.i[3][2];


        sNewVertex.iX = pMeshVertex[i].sPosition2.iX * sTmInvert.i[0][0] +
            pMeshVertex[i].sPosition2.iY * sTmInvert.i[1][0] +
            pMeshVertex[i].sPosition2.iZ * sTmInvert.i[2][0];

        sNewVertex.iY = pMeshVertex[i].sPosition2.iX * sTmInvert.i[0][1] +
            pMeshVertex[i].sPosition2.iY * sTmInvert.i[1][1] +
            pMeshVertex[i].sPosition2.iZ * sTmInvert.i[2][1];

        sNewVertex.iZ = pMeshVertex[i].sPosition2.iX * sTmInvert.i[0][2] +
            pMeshVertex[i].sPosition2.iY * sTmInvert.i[1][2] +
            pMeshVertex[i].sPosition2.iZ * sTmInvert.i[2][2];


        pMeshVertex[i].sPosition2.iX = (sNewVertex.iX >> 8) + sTmInvert.i[3][0];
        pMeshVertex[i].sPosition2.iY = (sNewVertex.iY >> 8) + sTmInvert.i[3][1];
        pMeshVertex[i].sPosition2.iZ = (sNewVertex.iZ >> 8) + sTmInvert.i[3][2];
    }
}

EXEMesh * EXEMesh::NewMeshGate( EXEMesh * pcMeshGame )
{
    EXEMeshOldClearData * p = (EXEMeshOldClearData *)pcMeshGame;
	p->Constructor();
	p->Destructor();

    delete p;

	return new EXEMesh();
}

EXEMesh * EXEMesh::NewMeshGateVertexFaces( EXEMesh * pcMeshGame, int iNumVertices, int iNumFaces )
{
	EXEMeshOldClearData * p = (EXEMeshOldClearData *)pcMeshGame;
	p->Constructor();
	p->Destructor();

	delete p;

	return new EXEMesh( iNumVertices, iNumFaces );
}

void EXEMeshOldClearData::Constructor()
{
    szName[0] = 0;
    szParentName[0] = 0;
    EXEMatrixIdentityI( sTm );

    pParentMesh = nullptr;

    sRot = nullptr;
    sPos = nullptr;
    sScale = nullptr;

    iRotateCount = 0;
    iPosCount = 0;
    iScaleCount = 0;

    sPreviousRotate = nullptr;

    pMeshFace = nullptr;
    pMeshVertex = nullptr;
    pMeshFacePosition = nullptr;
    iFaceCount = 0;
    iVertexCount = 0;
    iTextureLinkCount = 0;

    pVertexBoneList = nullptr;
}

void EXEMeshOldClearData::Destructor()
{
    DELET( sScale );
    DELET( sPos );
    DELET( sRot );
    DELET( sPreviousRotate );

    DELET( pMeshFacePosition );
    DELET( pVertexBoneList );
    DELET( pMeshFace );
    DELET( pMeshVertex );
}

EXEMeshOldClearData::EXEMeshOldClearData()
{
}

EXEMeshOldClearData::~EXEMeshOldClearData()
{
}
