#include "stdafx.h"
#include "AnimationHandler.h"
#include "SMD.h"
#include "Import.h"
#include "Export.h"


std::vector<AnimationDataInfo>	CAnimationHandler::vAnimationData;

typedef int( __thiscall *tfnTMAnimation )(void * pcThis, int iFrame, int iAngleX, int iAngleY, int iAngleZ, FrameInfoData * psFrameInfo);
tfnTMAnimation fnTMAnimation = (tfnTMAnimation)0x004759A0;


const BOOL USE_DYNAMIIC_FRAMES_OPTIMIZED = FALSE;


CAnimationHandler::CAnimationHandler()
{
	std::vector<AnimationDataInfo> & vAnimData = CAnimationHandler::vAnimationData;
	vAnimData.reserve( 1024 );
	for ( int i = 0; i < 1024; i++ )
	{
		vAnimData.push_back( AnimationDataInfo() );
		AnimationDataInfo * psAnimationData = &vAnimData[i];
		psAnimationData->bActive = FALSE;
	}
}


CAnimationHandler::~CAnimationHandler()
{
}

void CAnimationHandler::Init()
{
//	CreateThread( NULL, 16384, (LPTHREAD_START_ROUTINE)CAnimationHandler::AnimationThreadData, NULL, STACK_SIZE_PARAM_IS_A_RESERVATION, NULL );
}

AnimationDataInfo * CAnimationHandler::GetFreeData()
{
	std::vector<AnimationDataInfo> & vAnimData = CAnimationHandler::vAnimationData;
	for ( std::vector<AnimationDataInfo>::iterator it = vAnimData.begin(); it != vAnimData.end(); it++ )
	{
		AnimationDataInfo * p = &(*it);
		if ( !p->bActive )
			return p;
	}
	return NULL;
}

DWORD WINAPI CAnimationHandler::AnimationThreadData()
{
	while ( true )
	{
		std::vector<AnimationDataInfo> & vAnimData = CAnimationHandler::vAnimationData;
		for ( std::vector<AnimationDataInfo>::iterator it = vAnimData.begin(); it != vAnimData.end(); it++ )
		{
			AnimationDataInfo * p = &(*it);
			if ( p->bActive )
			{
				fnTMAnimation( p->pcThis, p->iFrame, p->iAngleX, p->iAngleY, p->iAngleZ, p->bFrame == TRUE ? &p->sFrameInfo : NULL );
				p->bActive = FALSE;
			}
		}
		Sleep( 1 );
	}
	return TRUE;
}

int CAnimationHandler::AddAnimationObject( void * pcThis, int iFrame, int iAngleX, int iAngleY, int iAngleZ, FrameInfoData * psFrameInfo )
{
	AnimationDataInfo sAnimationData;
	sAnimationData.pcThis	= pcThis;
	sAnimationData.iFrame	= iFrame;
	sAnimationData.iAngleX	= iAngleX;
	sAnimationData.iAngleY	= iAngleY;
	sAnimationData.iAngleZ	= iAngleZ;

	sAnimationData.bFrame	= FALSE;
	if ( psFrameInfo )
	{
		sAnimationData.bFrame				= TRUE;
		sAnimationData.sFrameInfo.dwStart	= psFrameInfo->dwStart;
		sAnimationData.sFrameInfo.dwEnd		= psFrameInfo->dwEnd;
	}
	sAnimationData.bActive	= TRUE;

	AnimationDataInfo * p = &sAnimationData;

	fnTMAnimation( p->pcThis, p->iFrame, p->iAngleX, p->iAngleY, p->iAngleZ, p->bFrame == TRUE ? &p->sFrameInfo : NULL );

	return 1;

	AnimationDataInfo * psAnimationData = GetFreeData();
	if ( psAnimationData )
		CopyMemory( psAnimationData, &sAnimationData, sizeof( AnimationDataInfo ) );
	else
		vAnimationData.push_back( sAnimationData );

	return 1;
}

void CAnimationHandler::SetFalconMove( FalconDataPattern  * psData )
{
	D3DVECTOR sVecAttract;
	sVecAttract.x = (float)psData->pcUnitData->sPosition.iX - psData->fWorldX;
	sVecAttract.y = ((float)(psData->pcUnitData->sPosition.iY + 17000) + (float)(*(int*)0x038FECB0)) - psData->fWorldY;
	sVecAttract.z = (float)psData->pcUnitData->sPosition.iZ - psData->fWorldZ;

	float fDistance = (float)sqrt( sVecAttract.x*sVecAttract.x + sVecAttract.y*sVecAttract.y + sVecAttract.z*sVecAttract.z );

	if ( fDistance > 300.0f )
	{
		psData->fWorldX = (float)psData->pcUnitData->sPosition.iX;//(sVecAttract.x / fDistance) * (420.0f * (float)psData->pcUnitData->sCharacterData.iMovementSpeed);
		psData->fWorldY = (float)psData->pcUnitData->sPosition.iY;//(sVecAttract.y / fDistance) * (420.0f * (float)psData->pcUnitData->sCharacterData.iMovementSpeed);
		psData->fWorldZ = (float)psData->pcUnitData->sPosition.iZ;//(sVecAttract.z / fDistance) * (420.0f * (float)psData->pcUnitData->sCharacterData.iMovementSpeed);
	}
	psData->fWorldY = ((float)(psData->pcUnitData->sPosition.iY + 17000) + (float)(*(int*)0x038FECB0));
}

BOOL CAnimationHandler::HandleBoneCache( EXEModelData * psModelData )
{
	for ( std::vector<ModelData*>::iterator it = vAnimationModels.begin(); it != vAnimationModels.end(); it++ )
	{
		ModelData * ps = (*it);

		if ( ps )
		{
			if ( STRINGCOMPAREI( ps->szBoneModelFilePath, psModelData->psModelData->szBoneModelFilePath ) )
			{
				CopyMemory( psModelData->psModelData->saModelAnimation, ps->saModelAnimation, sizeof( ModelAnimation ) * ps->iNumModelAnimation );

				psModelData->psModelData->iNumModelAnimation = ps->iNumModelAnimation;

				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CAnimationHandler::AddCacheModelData( ModelData * psModelData )
{
	for ( std::vector<ModelData*>::iterator it = vAnimationModels.begin(); it != vAnimationModels.end(); it++ )
	{
		ModelData * ps = (*it);

		if ( ps )
		{
			if ( STRINGCOMPAREI( ps->szBoneModelFilePath, psModelData->szBoneModelFilePath ) )
				return FALSE;
		}
	}

	vAnimationModels.push_back( psModelData );

	return TRUE;
}

BOOL CAnimationHandler::EncryptBoneFile( char * pszFile )
{
	Import::SMDFile impSMDFile( pszFile );
	Export::SMDFile expSMDFile( pszFile );

	if ( !impSMDFile.Load() )
		return FALSE;

	//Invert Rotations (Z -> X and X -> Z) (every pair times change Z to Y...)
	RawSMDObject * p = impSMDFile.psaRawObject;
	if ( p )
	{
		int iIncrementIndex = 0;

		BOOL bEncrypted = FALSE;

		if ( p->iKeyRotationCount > 0 )
		{
			RawSMDKeyRotation * pk = &p->paKeyRotation[0];

			if ( ((pk->iFrame & 0xF0000000) == 0xF0000000) || ((pk->iFrame & 0xF0000000) == 0xE0000000) )
				bEncrypted = TRUE;
		}

		if ( bEncrypted == FALSE )
		{
			for ( int i = 0; i < p->iKeyRotationCount; i++, iIncrementIndex++ )
			{
				RawSMDKeyRotation * pk	= p->paKeyRotation + i;
				float fOldX				= pk->fX;		

				if ( (iIncrementIndex % 2) == 0 )
				{
					pk->fX				= pk->fY;
					pk->fY				= fOldX;

					pk->iFrame			|= 0xF0000000;
				}
				else
				{
					pk->fX				= pk->fZ;
					pk->fZ				= fOldX;

					pk->iFrame			|= 0xE0000000;
				}
			}

			expSMDFile.SaveModel( &impSMDFile.sRawHeader, &impSMDFile.sRawMaterialHeader, impSMDFile.psaRawObject, impSMDFile.sModelVersion );
		}
	}

	impSMDFile.Unload();

	return TRUE;
}

BOOL CAnimationHandler::DecryptBoneFile( char * pszFile )
{
	Import::SMDFile impSMDFile( pszFile );
	Export::SMDFile expSMDFile( pszFile );

	if ( !impSMDFile.Load() )
		return FALSE;

	//Invert Rotations (Z -> X and X -> Z) and every pair times, change Z to Y...
	RawSMDObject * p = impSMDFile.psaRawObject;
	if ( p )
	{
		int iIncrementIndex = 0;

		for ( int i = 0; i < p->iKeyRotationCount; i++, iIncrementIndex++ )
		{
			RawSMDKeyRotation * psRot = p->paKeyRotation + i;
		
			if ( psRot )
			{
				float fOldX = psRot->fX;

				if ( (psRot->iFrame & 0xF0000000) == 0xF0000000 )
				{
					psRot->iFrame &= 0x0FFFFFFF;

					psRot->fX = psRot->fY;
					psRot->fY = fOldX;

				}
				else if ( (psRot->iFrame & 0xF0000000) == 0xE0000000 )
				{
					psRot->iFrame &= 0x0FFFFFFF;

					psRot->fX = psRot->fZ;
					psRot->fZ = fOldX;
				}
			}
		}
	}

	expSMDFile.SaveModel( &impSMDFile.sRawHeader, &impSMDFile.sRawMaterialHeader, impSMDFile.psaRawObject, impSMDFile.sModelVersion );

	impSMDFile.Unload();

	return TRUE;
}

BOOL CAnimationHandler::DecryptBoneFile( PTModel * pcModelAnimation )
{
	if ( pcModelAnimation )
	{
		for ( UINT u = 0; u < pcModelAnimation->uMeshCount; u++ )
		{
			PTMesh * pcMesh = pcModelAnimation->paMesh[u];

			if ( pcMesh )
			{
				for ( int i = 0; i < pcMesh->iRotateCount; i++ )
				{
					EXEAnimationRotation * psRot = pcMesh->sRot + i;

					if ( psRot )
					{
						float fOldX			= psRot->fX;

						if ( (psRot->iFrame & 0xF0000000) == 0xF0000000 )
						{
							psRot->iFrame	&= 0x0FFFFFFF;

							psRot->fX		= psRot->fY;
							psRot->fY		= fOldX;

						}
						else if ( (psRot->iFrame & 0xF0000000) == 0xE0000000 )
						{
							psRot->iFrame	&= 0x0FFFFFFF;

							psRot->fX		= psRot->fZ;
							psRot->fZ		= fOldX;
						}
					}
				}
			}
		}
	}

	return TRUE;
}

BOOL CAnimationHandler::InjectDynamicAnimation( struct EXEModelData * psModelData, PTModel * pcModelAnimation, int iID, BOOL bLoop, int iClassFlag, int iAreaType, int iaEvent[4], int iItemType, int iTypeAnim )
{
	if ( psModelData == NULL || pcModelAnimation == NULL || psModelData->psMotion == NULL || psModelData->psMotion->pcModel == NULL || psModelData->psModelData == NULL )
		return FALSE;

	PTModel * pcModel = psModelData->psMotion->pcModel;
	ModelData * psModelINX = psModelData->psModelData;

	ModelAnimation * ps = psModelINX->saModelAnimation + psModelINX->iNumModelAnimation;
	ModelAnimation * psOld = psModelINX->saModelAnimation + (psModelINX->iNumModelAnimation - 1);

	int iNewLastFrame = pcModel->uMaxFrame;

	int iNewAnimationStartFrame = iNewLastFrame;

	BOOL bFoundKeyFrames = FALSE;

	//Inject
	for ( int i = 0; i < (int)pcModelAnimation->uMeshCount; i++ )
	{
		PTMesh * pcMeshA	= pcModelAnimation->paMesh[i];

		if ( pcMeshA )
		{
			//Get Parent Mesh
			PTMesh * pcMesh = pcModel->GetMesh( pcMeshA->szName );

			RawSMDObject * psRawObjectFrom = (RawSMDObject*)pcMeshA;
			RawSMDObject * psRawObjectTo = (RawSMDObject*)pcMesh;

			if ( bFoundKeyFrames == FALSE )
			{
				if ( psRawObjectFrom->iKeyRotationCount > 0 )
				{
					iNewLastFrame += psRawObjectFrom->paKeyRotation[psRawObjectFrom->iKeyRotationCount - 1].iFrame;
					bFoundKeyFrames = TRUE;
				}
				else if ( psRawObjectFrom->iKeyPositionCount > 0 )
				{
					iNewLastFrame += psRawObjectFrom->paKeyPosition[psRawObjectFrom->iKeyPositionCount - 1].iFrame;
					bFoundKeyFrames = TRUE;
				}
				else if ( psRawObjectFrom->iKeyScaleCount > 0 )
				{
					iNewLastFrame += psRawObjectFrom->paKeyScale[psRawObjectFrom->iKeyScaleCount - 1].iFrame;
					bFoundKeyFrames = TRUE;
				}
			}

			//Not have Mesh on SMB? ignore it...
			if ( pcMesh == NULL )
				continue;

			RawSMDKeyRotation sKeyRotation;
			sKeyRotation.fW = 0.0f;
			sKeyRotation.fX = 0.0f;
			sKeyRotation.fY = 0.0f;
			sKeyRotation.fZ = 0.0f;

			RawSMDMatrixF sRotation;
			sRotation = *(RawSMDMatrixF*)&EXEMatrixConvertIF( *(EXEMatrixI*)&psRawObjectFrom->sRotationMatrix );

			RawSMDKeyPosition sKeyPosition;
			sKeyPosition.fX = (float)(psRawObjectFrom->sTranslationVector.iX) / 256.0f;
			sKeyPosition.fY = (float)(psRawObjectFrom->sTranslationVector.iY) / 256.0f;
			sKeyPosition.fZ = (float)(psRawObjectFrom->sTranslationVector.iZ) / 256.0f;

			RawSMDKeyScale sKeyScale;
			sKeyScale.iX = psRawObjectFrom->sScalingVector.iX;
			sKeyScale.iY = psRawObjectFrom->sScalingVector.iY;
			sKeyScale.iZ = psRawObjectFrom->sScalingVector.iZ;

			int iIndexFrameNew = USE_DYNAMIIC_FRAMES_OPTIMIZED ? ((_countof( psRawObjectTo->aRotationFrame ) - 8) - 1) : 0;

			RawSMDFrame * psNewRotFrame = psRawObjectTo->aRotationFrame + iIndexFrameNew;
			RawSMDFrame * psNewTraFrame = psRawObjectTo->aTranslationFrame + iIndexFrameNew;
			RawSMDFrame * psNewSclFrame = psRawObjectTo->aScalingFrame + iIndexFrameNew;


			if ( psRawObjectFrom->iKeyRotationCount > 0 )
			{
				for ( int k = 0; k < psRawObjectFrom->iKeyRotationCount; k++ )
				{
					RawSMDKeyRotation * psKeyRotation = psRawObjectFrom->paKeyRotation + k;
					RawSMDMatrixF * psRotation = psRawObjectFrom->paRotation + k;

					psKeyRotation->iFrame += iNewAnimationStartFrame;

					InjectKeyRotation( psRawObjectTo, -1, psKeyRotation->iFrame, psKeyRotation, psRotation );
				}
			}
			else
			{
				InjectKeyRotation( psRawObjectTo, -1, iNewAnimationStartFrame, &sKeyRotation, &sRotation );
				InjectKeyRotation( psRawObjectTo, -1, iNewLastFrame, &sKeyRotation, &sRotation );

				psNewRotFrame->iKeyFrameCount += 2;
			}

			if ( psRawObjectFrom->iKeyPositionCount > 0 )
			{
				for ( int k = 0; k < psRawObjectFrom->iKeyPositionCount; k++ )
				{
					RawSMDKeyPosition * psKeyPosition = psRawObjectFrom->paKeyPosition + k;

					psKeyPosition->iFrame += iNewAnimationStartFrame;

					InjectKeyPosition( psRawObjectTo, -1, psKeyPosition->iFrame, psKeyPosition );
				}
			}
			else
			{
				InjectKeyPosition( psRawObjectTo, -1, iNewAnimationStartFrame, &sKeyPosition );
				InjectKeyPosition( psRawObjectTo, -1, iNewLastFrame, &sKeyPosition );

				psNewTraFrame->iKeyFrameCount += 2;
			}

			if ( psRawObjectFrom->iKeyScaleCount > 0 )
			{
				for ( int k = 0; k < psRawObjectFrom->iKeyScaleCount; k++ )
				{
					RawSMDKeyScale * psKeyScale = psRawObjectFrom->paKeyScale + k;

					psKeyScale->iFrame += iNewAnimationStartFrame;

					InjectKeyScale( psRawObjectTo, -1, psKeyScale->iFrame, psKeyScale );
				}
			}
			else
			{
				InjectKeyScale( psRawObjectTo, -1, iNewAnimationStartFrame, &sKeyScale );
				InjectKeyScale( psRawObjectTo, -1, iNewLastFrame, &sKeyScale );

				psNewSclFrame->iKeyFrameCount += 2;
			}

			MergeKeyRotations( psRawObjectTo );
			MergeKeyPositions( psRawObjectTo );
			MergeKeyScalings( psRawObjectTo );

			psNewRotFrame->iEndFrame = iNewLastFrame;
			psNewTraFrame->iEndFrame = iNewLastFrame;
			psNewSclFrame->iEndFrame = iNewLastFrame;

			psNewRotFrame->iKeyFrameCount = psRawObjectTo->iKeyRotationCount;
			psNewTraFrame->iKeyFrameCount = psRawObjectTo->iKeyPositionCount;
			psNewSclFrame->iKeyFrameCount = psRawObjectTo->iKeyScaleCount;
		}
	}


	pcModel->uMaxFrame		= iNewLastFrame;

	ps->iType				= (EAnimationType)iTypeAnim;
	ps->bLoop				= bLoop;
	ps->eClassFlag			= (EAnimationClassFlag)iClassFlag;
	ps->eMapTypeFlag		= (ModelAnimation::EMapTypeFlag)iAreaType;
	ps->uaSkill[0]			= iID;
	ps->iSubFileFrameNumber = USE_DYNAMIIC_FRAMES_OPTIMIZED ? 24 : 1;
	ps->iBeginFrame			= iNewAnimationStartFrame / 160;
	ps->iEndFrame			= iNewLastFrame / 160;
	ps->iaEventFrame[0]		= iaEvent[0] * 160;
	ps->iaEventFrame[1]		= iaEvent[1] * 160;
	ps->iaEventFrame[2]		= iaEvent[2] * 160;
	ps->iaEventFrame[3]		= iaEvent[3] * 160;

	static const WORD waItemFS1Hand[] = { 0x0000, 0x0001, 0x0002, 0x0003, 0x0005, 0x0006, 0x000A, 0x000D, 0x004E, 0x0060, 0x0061, 0x0062, 0x0064, 0x0065, 0x0066, 0x0067, 0x0069, 0x006A, 0x006C, 0x006F, 0x0083, 0x00B3, 0x00B5 };
	static const WORD waItemFS2Hand[] = { 0x0004, 0x0007, 0x0008, 0x0009, 0x000B, 0x000C, 0x000E, 0x0063, 0x0068, 0x006B, 0x006D, 0x006E, 0x0070, 0x0081, 0x0082, 0x0084, 0x0085, 0x0086, 0x0087, 0x00B2, 0x00B4, 0x00B6, 0x00B7, 0x00B8 };
	static const WORD waItemFSOther[] = { 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x00A1, 0x00A2, 0x00A3, 0x00A4, 0x00A5, 0x00A6, 0x00A7 };

	static const WORD waItemMS1Hand[] = { 0x0000, 0x0001, 0x0002, 0x0003, 0x0005, 0x0006, 0x000A, 0x000D, 0x001E, 0x001F, 0x0020, 0x0021, 0x0022, 0x0025, 0x0026, 0x0029, 0x002A, 0x002E, 0x002F, 0x0030, 0x0033, 0x0034, 0x0038, 0x003A, 
										  0x003E, 0x003F, 0x0040, 0x004E, 0x0060, 0x0061, 0x0062, 0x0064, 0x0065, 0x0066, 0x0067, 0x0069, 0x006A, 0x006C, 0x006F, 0x0083, 0x0091, 0x0094, 0x00B3, 0x00B5 };
	static const WORD waItemMS2Hand[] = { 0x0004, 0x0007, 0x0008, 0x0009, 0x000B, 0x000C, 0x000E, 0x0023, 0x0024, 0x0027, 0x0028, 0x002B, 0x002C, 0x002D, 0x0063, 0x0068, 0x006B, 0x006D, 0x006E, 0x0070, 0x0081, 0x0082, 0x0084, 0x0085,
										  0x0086, 0x0087, 0x0092, 0x0093, 0x0095, 0x0096, 0x0097, 0x00B2, 0x00B4, 0x00B6, 0x00B7, 0x00B8 };
	static const WORD waItemMSOther[] = { 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x00A1, 0x00A2, 0x00A3, 0x00A4, 0x00A5, 0x00A6, 0x00A7 };

	static const WORD waItemAS1Hand[] = { 0x0050, 0x0051, 0x0057 };
	static const WORD waItemAS1HandC[] = { 0x0052 };


	if ( iItemType == 2 )
	{
		if ( iClassFlag == ANIMATIONCLASSFLAG_Fighter )
		{
			CopyMemory( ps->eaItemIndex, waItemFS1Hand, _countof( waItemFS1Hand ) * sizeof( WORD ) );
			ps->iItemIndexCount = _countof( waItemFS1Hand );
		}
		else if ( iClassFlag == ANIMATIONCLASSFLAG_Mechanician )
		{
			CopyMemory( ps->eaItemIndex, waItemMS1Hand, _countof( waItemMS1Hand ) * sizeof( WORD ) );
			ps->iItemIndexCount = _countof( waItemMS1Hand );
		}
		else if ( iClassFlag == ANIMATIONCLASSFLAG_Archer )
		{
			CopyMemory( ps->eaItemIndex, waItemAS1Hand, _countof( waItemAS1Hand ) * sizeof( WORD ) );
			ps->iItemIndexCount = _countof( waItemAS1Hand );
		}
	}
	else if ( iItemType == 3 )
	{
		if ( iClassFlag == ANIMATIONCLASSFLAG_Fighter )
		{
			CopyMemory( ps->eaItemIndex, waItemFS2Hand, _countof( waItemFS2Hand ) * sizeof( WORD ) );
			ps->iItemIndexCount = _countof( waItemFS2Hand );
		}
		else if ( iClassFlag == ANIMATIONCLASSFLAG_Mechanician )
		{
			CopyMemory( ps->eaItemIndex, waItemMS2Hand, _countof( waItemMS2Hand ) * sizeof( WORD ) );
			ps->iItemIndexCount = _countof( waItemMS2Hand );
		}
	}
	else if ( iItemType == 4 )
	{
		if ( iClassFlag == ANIMATIONCLASSFLAG_Archer )
		{
			CopyMemory( ps->eaItemIndex, waItemAS1HandC, _countof( waItemAS1HandC ) * sizeof( WORD ) );
			ps->iItemIndexCount = _countof( waItemAS1HandC );
		}
	}
	else
	{
		if ( iClassFlag == ANIMATIONCLASSFLAG_Fighter )
		{
			CopyMemory( ps->eaItemIndex, waItemFSOther, _countof( waItemFSOther ) * sizeof( WORD ) );
			ps->iItemIndexCount = _countof( waItemFSOther );
		}
		else if ( iClassFlag == ANIMATIONCLASSFLAG_Mechanician )
		{
			CopyMemory( ps->eaItemIndex, waItemMSOther, _countof( waItemMSOther ) * sizeof( WORD ) );
			ps->iItemIndexCount = _countof( waItemMSOther );
		}
	}

	CopyMemory( psModelINX->saModelAnimation + psModelINX->iNumModelAnimation, ps, sizeof( ModelAnimation ) );

	psModelINX->iNumModelAnimation++;

	AddCacheModelData( psModelINX );

	return FALSE;
}

void CAnimationHandler::LoadDynamicAnimation()
{
	char szObjectName[MAX_PATH];
	char szBipName[MAX_PATH];
	char szINX[MAX_PATH];
	char szSMB[MAX_PATH];

	char szSkillName[64];
	
	BOOL bLoop		= FALSE;

	int iItemType	= 1; //1 = all, 2 = one hand, 3 = two hand
	
	int iaEvent[4]	= { 0 };
	int iAreaType	= 0;
	int iClassFlag	= 0;
	int iID			= 0;
	int iTypeAnim	= ANIMATIONTYPE_Skill;

	INI::CReader cReader( "game\\objects\\animskills.ini" );

	int iCount	= cReader.ReadInt( "Skills", "Count" );

	int iNext	= 0;

	std::vector<std::string> vNeedFix;

	for ( int i = 0; i < iCount; i++ )
	{
		STRINGFORMAT( szSkillName, "SkillAnimation%d", (iNext + 1) );

		STRINGCOPY( szObjectName, cReader.ReadString( szSkillName, "AnimationFile" ).c_str() );

		bLoop		= cReader.ReadBool( szSkillName, "Loop" );
		iAreaType	= cReader.ReadInt( szSkillName, "AreaType" );
		iID			= cReader.ReadInt( szSkillName, "ID" );
		iaEvent[0]	= cReader.ReadInt( szSkillName, "Event1" );
		iaEvent[1]	= cReader.ReadInt( szSkillName, "Event2" );
		iaEvent[2]	= cReader.ReadInt( szSkillName, "Event3" );
		iaEvent[3]	= cReader.ReadInt( szSkillName, "Event4" );
		iItemType	= cReader.ReadInt( szSkillName, "ItemType" );
		iTypeAnim	= cReader.ReadInt( szSkillName, "AnimType" );

		if ( iTypeAnim == 0 )
			iTypeAnim = ANIMATIONTYPE_Skill;
		else
		{
		//	iTypeAnim ^= 0x6D6D;
		//	iTypeAnim -= iID;

			
			const int IDANIMM = 5001;

			const int III = ((IDANIMM + ANIMATIONTYPE_Running) ^ 0x6D6D);
			
			const int IIII = ((IDANIMM + ANIMATIONTYPE_Idle) ^ 0x6D6D);
			
		}

		if ( iItemType == 0 )
			iItemType++;

		std::string strClassFlag	= cReader.ReadString( szSkillName, "Class" );
		int iClassFlag				= 0;

		if ( STRINGCOMPAREI( strClassFlag.c_str(), "Fighter" ) )
		{
			iClassFlag = ANIMATIONCLASSFLAG_Fighter;
			STRINGCOPY( szBipName, "char\\tmABCD\\M1Bip.ini" );
		}
		else if ( STRINGCOMPAREI( strClassFlag.c_str(), "Mech" ) )
		{
			iClassFlag = ANIMATIONCLASSFLAG_Mechanician;
			STRINGCOPY( szBipName, "char\\tmABCD\\M1Bip.ini" );
		}	
		else if ( STRINGCOMPAREI( strClassFlag.c_str(), "Archer" ) )
		{
			iClassFlag = ANIMATIONCLASSFLAG_Archer;
			STRINGCOPY( szBipName, "char\\tmABCD\\M2Bip.ini" );
		}
		else if ( STRINGCOMPAREI( strClassFlag.c_str(), "Pikeman" ) )
		{
			iClassFlag = ANIMATIONCLASSFLAG_Pikeman;
			STRINGCOPY( szBipName, "char\\tmABCD\\M4Bip.ini" );
		}
		else if ( STRINGCOMPAREI( strClassFlag.c_str(), "Assassin" ) )
		{
			iClassFlag = ANIMATIONCLASSFLAG_Assassin;
			STRINGCOPY( szBipName, "char\\tmABCD\\M6Bip.ini" );
		}
		else if ( STRINGCOMPAREI( strClassFlag.c_str(), "Knight" ) )
		{
			iClassFlag = ANIMATIONCLASSFLAG_Knight;
			STRINGCOPY( szBipName, "char\\tmABCD\\M1Bip.ini" );
		}
		else if ( STRINGCOMPAREI( strClassFlag.c_str(), "Atalanta" ) )
		{
			iClassFlag = ANIMATIONCLASSFLAG_Atalanta;
			STRINGCOPY( szBipName, "char\\tmABCD\\M2Bip.ini" );
		}
		else if ( STRINGCOMPAREI( strClassFlag.c_str(), "Priestess" ) )
		{
			iClassFlag = ANIMATIONCLASSFLAG_Priestess;
			STRINGCOPY( szBipName, "char\\tmABCD\\M5Bip.ini" );
		}
		else if ( STRINGCOMPAREI( strClassFlag.c_str(), "Magician" ) )
		{
			iClassFlag = ANIMATIONCLASSFLAG_Magician;
			STRINGCOPY( szBipName, "char\\tmABCD\\M3Bip.ini" );
		}
		else if ( STRINGCOMPAREI( strClassFlag.c_str(), "Shaman" ) )
		{
			iClassFlag = ANIMATIONCLASSFLAG_Shaman;
			STRINGCOPY( szBipName, "char\\tmABCD\\M7Bip.ini" );
		}
		if ( szObjectName[0] == 0 || szBipName[0] == 0 )
		{
			iNext++;
			continue;
		}

		EXEModelData * psModelData	= (EXEModelData*)Game::ReadInx( szBipName );	
		PTModel * pcModel			= LoadPTModelPhysique( szObjectName );

		DecryptBoneFile( pcModel );

		STRINGCOPY( szINX, szBipName );
		szINX[STRLEN( szINX ) - 1] = 'x';

		STRINGCOPY( szSMB, psModelData->psModelData->szBoneModelFilePath );

		int iLength = STRLEN( szSMB );
		if ( iLength > 0 )
		{
			szSMB[iLength - 3] = 's';
			szSMB[iLength - 2] = 'm';
			szSMB[iLength - 1] = 'b';
		}

		iLength = STRLEN( szObjectName );
		if ( iLength > 0 )
		{
			szObjectName[iLength - 3] = 's';
			szObjectName[iLength - 2] = 'm';
			szObjectName[iLength - 1] = 'b';
		}

		bool bNeedFix = true;
		for ( auto & str : vNeedFix )
		{
			if ( str.compare( szSMB ) == 0 )
			{
				bNeedFix = false;
				break;
			}
		}

		if ( bNeedFix )
		{
			ANIMATIONHANDLER->FixSMBINXSubFileMotion( szSMB, szINX, szSMB, szINX );

			vNeedFix.push_back( szSMB );
		}

		InjectDynamicAnimation( psModelData, pcModel, iID, bLoop, iClassFlag, iAreaType, iaEvent, iItemType, iTypeAnim );
		
		//EncryptBoneFile( szObjectName );

		DELET( pcModel );

		iNext++;
	}

	vNeedFix.clear();
}


RawSMDKeyRotation * InjectKeyRotation( RawSMDKeyRotation * pArray, int iArraySize, RawSMDKeyRotation & sInjectThis, int iFirstFrame, int iLastFrame, int & iIndexOut )
{
	for( int i = 0; i < iArraySize - 1; i++ )
	{
		RawSMDKeyRotation * p1 = pArray + i;
		RawSMDKeyRotation * p2 = pArray + (i + 1);

		if( iFirstFrame > p1->iFrame && iLastFrame < p2->iFrame )
		{
			int iIndex = i + 1;

			RawSMDKeyRotation * pNewArray = new RawSMDKeyRotation[iArraySize+2];
			CopyMemory( pNewArray, pArray, sizeof( RawSMDKeyRotation ) * (i + 1) );

			sInjectThis.iFrame = iFirstFrame;
			pNewArray[iIndex] = sInjectThis;

			sInjectThis.iFrame = iLastFrame;
			pNewArray[iIndex+1] = sInjectThis;

			CopyMemory( pNewArray + i + 3, pArray + (i + 1), iArraySize - (i + 1) );

			delete[] pArray;

			return pNewArray;
		}
	}

	//WRITEDBG ("wtf failed to inject rotation at [%d,%d]", iFirstFrame, iLastFrame );

	return NULL;
}

int FindKeyRotationIndex( RawSMDObject * psRawObject, int iStartFrame, int iEndFrame )
{
	int iIndex = 0;

	if( psRawObject->iKeyRotationCount > 0 )
	{
		if( iEndFrame < psRawObject->paKeyRotation[0].iFrame )
			return 0;

		if( iStartFrame >= psRawObject->paKeyRotation[psRawObject->iKeyRotationCount-1].iFrame )
			return psRawObject->iKeyRotationCount;

		for( int i = 1; i < psRawObject->iKeyRotationCount; i++ )
		{
			RawSMDKeyRotation * p1 = psRawObject->paKeyRotation + (i - 1);
			RawSMDKeyRotation * p2 = psRawObject->paKeyRotation + i;

			if( (iStartFrame >= p1->iFrame) && (iEndFrame <= p2->iFrame) )
				return i;
		}
	}

	return iIndex;
}

void InjectKeyRotation( RawSMDObject * psRawObject, int iIndex, int iFrame, RawSMDKeyRotation * psKeyRotation, RawSMDMatrixF * psKeyRotationMatrix )
{
	if( iIndex == -1 )
		iIndex = psRawObject->iKeyRotationCount;

	{
		RawSMDKeyRotation * paOldArray = psRawObject->paKeyRotation;
		psRawObject->paKeyRotation = new RawSMDKeyRotation[psRawObject->iKeyRotationCount+1];

		if( iIndex > 0 )
			CopyMemory( psRawObject->paKeyRotation, paOldArray, sizeof( RawSMDKeyRotation ) * iIndex );

		psRawObject->paKeyRotation[iIndex] = *psKeyRotation;
		psRawObject->paKeyRotation[iIndex].iFrame = iFrame;

		if( psRawObject->iKeyRotationCount > iIndex )
			CopyMemory( psRawObject->paKeyRotation + iIndex + 1, paOldArray + iIndex, sizeof( RawSMDKeyRotation ) * (psRawObject->iKeyRotationCount - iIndex) );

		delete[] paOldArray;
	}
	
	{
		RawSMDMatrixF * paOldArray = psRawObject->paRotation;
		psRawObject->paRotation = new RawSMDMatrixF[psRawObject->iKeyRotationCount+1];

		if( iIndex > 0 )
			CopyMemory( psRawObject->paRotation, paOldArray, sizeof( RawSMDMatrixF ) * iIndex );

		psRawObject->paRotation[iIndex] = *psKeyRotationMatrix;

		if( psRawObject->iKeyRotationCount > iIndex )
			CopyMemory( psRawObject->paRotation + iIndex + 1, paOldArray + iIndex, sizeof( RawSMDMatrixF ) * (psRawObject->iKeyRotationCount - iIndex) );

		delete[] paOldArray;
	}

	psRawObject->iKeyRotationCount++;
}

BOOL MergeKeyRotations( RawSMDObject * psRawObject )
{
	int iNewSize = psRawObject->iKeyRotationCount;

	for( int i = 0; i < (iNewSize - 3); )
	{
		RawSMDKeyRotation * p[4];
		p[0] = psRawObject->paKeyRotation + (i + 0);
		p[1] = psRawObject->paKeyRotation + (i + 1);
		p[2] = psRawObject->paKeyRotation + (i + 2);
		p[3] = psRawObject->paKeyRotation + (i + 3);

		RawSMDMatrixF * pm[4];
		pm[0] = psRawObject->paRotation + (i + 0);
		pm[1] = psRawObject->paRotation + (i + 1);
		pm[2] = psRawObject->paRotation + (i + 2);
		pm[3] = psRawObject->paRotation + (i + 3);

		if( (p[0]->fX != 0.0f) || (p[0]->fY != 0.0f) || (p[0]->fZ != 0.0f) || (p[0]->fW != 0.0f) )
		{
			i++;
			continue;
		}
		
		if( (p[1]->fX != 0.0f) || (p[1]->fY != 0.0f) || (p[1]->fZ != 0.0f) || (p[1]->fW != 0.0f) )
		{
			i++;
			continue;
		}
		
		if( (p[2]->fX != 0.0f) || (p[2]->fY != 0.0f) || (p[2]->fZ != 0.0f) || (p[2]->fW != 0.0f) )
		{
			i++;
			continue;
		}
		
		if( (p[3]->fX != 0.0f) || (p[3]->fY != 0.0f) || (p[3]->fZ != 0.0f) || (p[3]->fW != 0.0f) )
		{
			i++;
			continue;
		}

		if( memcmp( pm[0]->f, pm[1]->f, sizeof( RawSMDMatrixF ) ) != 0 )
		{
			i++;
			continue;
		}

		if( memcmp( pm[0]->f, pm[2]->f, sizeof( RawSMDMatrixF ) ) != 0 )
		{
			i++;
			continue;
		}

		if( memcmp( pm[0]->f, pm[3]->f, sizeof( RawSMDMatrixF ) ) != 0 )
		{
			i++;
			continue;
		}

		p[1]->iFrame = p[3]->iFrame;
		
		for( int j = (i + 4); j < iNewSize; j++ )
		{
			RawSMDKeyRotation * c1 = psRawObject->paKeyRotation + (j - 2);
			RawSMDKeyRotation * c2 = psRawObject->paKeyRotation + (j + 0);
			
			RawSMDMatrixF * m1 = psRawObject->paRotation + (j - 2);
			RawSMDMatrixF * m2 = psRawObject->paRotation + (j + 0);

			CopyMemory( c1, c2, sizeof( RawSMDKeyRotation ) );
			CopyMemory( m1, m2, sizeof( RawSMDMatrixF ) );
		}

		iNewSize -= 2;
	}

	if( iNewSize < psRawObject->iKeyRotationCount )
	{
		//WRITEDBG( "Merged Key Rotations (%d -> %d)", psRawObject->iKeyRotationCount, iNewSize );

		{
			RawSMDKeyRotation * paOldArray = psRawObject->paKeyRotation;
			psRawObject->paKeyRotation = new RawSMDKeyRotation[iNewSize];
			CopyMemory( psRawObject->paKeyRotation, paOldArray, sizeof( RawSMDKeyRotation ) * iNewSize );
			delete[] paOldArray;
		}
		
		{
			RawSMDMatrixF * paOldArray = psRawObject->paRotation;
			psRawObject->paRotation = new RawSMDMatrixF[iNewSize];
			CopyMemory( psRawObject->paRotation, paOldArray, sizeof( RawSMDMatrixF ) * iNewSize );
			delete[] paOldArray;
		}
		
		psRawObject->iKeyRotationCount = iNewSize;

		return TRUE;
	}

	return FALSE;
}

BOOL FixKeyRotationArray( RawSMDObject * psRawObject, int iLastFrame )
{
	BOOL bReturn = FALSE;

	if( psRawObject->iKeyRotationCount > 0 )
	{
		if( psRawObject->paKeyRotation[psRawObject->iKeyRotationCount-1].iFrame != iLastFrame )
		{
			{
				RawSMDKeyRotation * paOldArray = psRawObject->paKeyRotation;
				psRawObject->paKeyRotation = new RawSMDKeyRotation[psRawObject->iKeyRotationCount+1];
				CopyMemory( psRawObject->paKeyRotation, paOldArray, sizeof( RawSMDKeyRotation ) * psRawObject->iKeyRotationCount );
				psRawObject->paKeyRotation[psRawObject->iKeyRotationCount] = paOldArray[psRawObject->iKeyRotationCount-1];
				psRawObject->paKeyRotation[psRawObject->iKeyRotationCount].iFrame = iLastFrame;
				delete[] paOldArray;

				bReturn = TRUE;
			}
			
			{
				RawSMDMatrixF * paOldArray = psRawObject->paRotation;
				psRawObject->paRotation = new RawSMDMatrixF[psRawObject->iKeyRotationCount+1];
				CopyMemory( psRawObject->paRotation, paOldArray, sizeof( RawSMDMatrixF ) * psRawObject->iKeyRotationCount );
				psRawObject->paRotation[psRawObject->iKeyRotationCount] = paOldArray[psRawObject->iKeyRotationCount-1];
				delete[] paOldArray;

				bReturn = TRUE;
			}

			psRawObject->iKeyRotationCount++;
		}
	}

	return bReturn;
}

int FindKeyPositionIndex( RawSMDObject * psRawObject, int iStartFrame, int iEndFrame )
{
	int iIndex = 0;

	if( psRawObject->iKeyPositionCount > 0 )
	{
		if( iEndFrame < psRawObject->paKeyPosition[0].iFrame )
			return 0;

		if( iStartFrame >= psRawObject->paKeyPosition[psRawObject->iKeyPositionCount-1].iFrame )
			return psRawObject->iKeyPositionCount;

		for( int i = 1; i < psRawObject->iKeyPositionCount; i++ )
		{
			RawSMDKeyPosition * p1 = psRawObject->paKeyPosition + (i - 1);
			RawSMDKeyPosition * p2 = psRawObject->paKeyPosition + i;

			if( (iStartFrame >= p1->iFrame) && (iEndFrame <= p2->iFrame) )
				return i;
		}
	}

	return iIndex;
}

void InjectKeyPosition( RawSMDObject * psRawObject, int iIndex, int iFrame, RawSMDKeyPosition * psKeyPosition )
{
	if( iIndex == -1 )
		iIndex = psRawObject->iKeyPositionCount;

	RawSMDKeyPosition * paOldArray = psRawObject->paKeyPosition;
	psRawObject->paKeyPosition = new RawSMDKeyPosition[psRawObject->iKeyPositionCount+1];

	if( iIndex > 0 )
		CopyMemory( psRawObject->paKeyPosition, paOldArray, sizeof( RawSMDKeyPosition ) * iIndex );

	psRawObject->paKeyPosition[iIndex] = *psKeyPosition;
	psRawObject->paKeyPosition[iIndex].iFrame = iFrame;

	if( psRawObject->iKeyPositionCount > iIndex )
		CopyMemory( psRawObject->paKeyPosition + iIndex + 1, paOldArray + iIndex, sizeof( RawSMDKeyPosition ) * (psRawObject->iKeyPositionCount - iIndex) );

	delete[] paOldArray;

	psRawObject->iKeyPositionCount++;
}

BOOL MergeKeyPositions( RawSMDObject * psRawObject )
{
	int iNewSize = psRawObject->iKeyPositionCount;

	for( int i = 0; i < (iNewSize - 3); )
	{
		RawSMDKeyPosition * p[4];
		p[0] = psRawObject->paKeyPosition + (i + 0);
		p[1] = psRawObject->paKeyPosition + (i + 1);
		p[2] = psRawObject->paKeyPosition + (i + 2);
		p[3] = psRawObject->paKeyPosition + (i + 3);

		if( (p[0]->fX != p[1]->fX) || (p[0]->fY != p[1]->fY) || (p[0]->fZ != p[1]->fZ) )
		{
			i++;
			continue;
		}
		
		if( (p[0]->fX != p[2]->fX) || (p[0]->fY != p[2]->fY) || (p[0]->fZ != p[2]->fZ) )
		{
			i++;
			continue;
		}
		
		if( (p[0]->fX != p[3]->fX) || (p[0]->fY != p[3]->fY) || (p[0]->fZ != p[3]->fZ) )
		{
			i++;
			continue;
		}

		p[1]->iFrame = p[3]->iFrame;

		for( int j = (i + 4); j < iNewSize; j++ )
		{
			RawSMDKeyPosition * c1 = psRawObject->paKeyPosition + (j - 2);
			RawSMDKeyPosition * c2 = psRawObject->paKeyPosition + (j + 0);

			CopyMemory( c1, c2, sizeof( RawSMDKeyPosition ) );
		}

		iNewSize -= 2;
	}

	if( iNewSize < psRawObject->iKeyPositionCount )
	{
		//WRITEDBG( "Merged Key Positions (%d -> %d)", psRawObject->iKeyPositionCount, iNewSize );

		RawSMDKeyPosition * paOldArray = psRawObject->paKeyPosition;
		psRawObject->paKeyPosition = new RawSMDKeyPosition[iNewSize];
		CopyMemory( psRawObject->paKeyPosition, paOldArray, sizeof( RawSMDKeyPosition ) * iNewSize );
		psRawObject->iKeyPositionCount = iNewSize;
		delete[] paOldArray;

		return TRUE;
	}

	return FALSE;
}

BOOL FixKeyPositionArray( RawSMDObject * psRawObject, int iLastFrame )
{
	if( psRawObject->iKeyPositionCount > 0 )
	{
		if( psRawObject->paKeyPosition[psRawObject->iKeyPositionCount-1].iFrame != iLastFrame )
		{
			RawSMDKeyPosition * paOldArray = psRawObject->paKeyPosition;
			psRawObject->paKeyPosition = new RawSMDKeyPosition[psRawObject->iKeyPositionCount+1];
			CopyMemory( psRawObject->paKeyPosition, paOldArray, sizeof( RawSMDKeyPosition ) * psRawObject->iKeyPositionCount );
			psRawObject->paKeyPosition[psRawObject->iKeyPositionCount] = paOldArray[psRawObject->iKeyPositionCount-1];
			psRawObject->paKeyPosition[psRawObject->iKeyPositionCount].iFrame = iLastFrame;
			psRawObject->iKeyPositionCount++;
			delete[] paOldArray;

			return TRUE;
		}
	}

	return FALSE;
}

int FindKeyScaleIndex( RawSMDObject * psRawObject, int iStartFrame, int iEndFrame )
{
	int iIndex = 0;

	if( psRawObject->iKeyScaleCount > 0 )
	{
		if( iEndFrame < psRawObject->paKeyScale[0].iFrame )
			return 0;

		if( iStartFrame >= psRawObject->paKeyScale[psRawObject->iKeyScaleCount-1].iFrame )
			return psRawObject->iKeyScaleCount;

		for( int i = 1; i < psRawObject->iKeyScaleCount; i++ )
		{
			RawSMDKeyScale * p1 = psRawObject->paKeyScale + (i - 1);
			RawSMDKeyScale * p2 = psRawObject->paKeyScale + i;

			if( (iStartFrame >= p1->iFrame) && (iEndFrame <= p2->iFrame) )
				return i;
		}
	}

	return iIndex;
}

void InjectKeyScale( RawSMDObject * psRawObject, int iIndex, int iFrame, RawSMDKeyScale * psKeyScale )
{
	if( iIndex == -1 )
		iIndex = psRawObject->iKeyScaleCount;

	RawSMDKeyScale * paOldArray = psRawObject->paKeyScale;
	psRawObject->paKeyScale = new RawSMDKeyScale[psRawObject->iKeyScaleCount+1];

	if( iIndex > 0 )
		CopyMemory( psRawObject->paKeyScale, paOldArray, sizeof( RawSMDKeyScale ) * iIndex );

	psRawObject->paKeyScale[iIndex] = *psKeyScale;
	psRawObject->paKeyScale[iIndex].iFrame = iFrame;

	if( psRawObject->iKeyScaleCount > iIndex )
		CopyMemory( psRawObject->paKeyScale + iIndex + 1, paOldArray + iIndex, sizeof( RawSMDKeyScale ) * (psRawObject->iKeyScaleCount - iIndex) );

	delete[] paOldArray;

	psRawObject->iKeyScaleCount++;
}

BOOL MergeKeyScalings( RawSMDObject * psRawObject )
{
	int iNewSize = psRawObject->iKeyScaleCount;

	for( int i = 0; i < (iNewSize - 3); )
	{
		RawSMDKeyScale * p[4];
		p[0] = psRawObject->paKeyScale + (i + 0);
		p[1] = psRawObject->paKeyScale + (i + 1);
		p[2] = psRawObject->paKeyScale + (i + 2);
		p[3] = psRawObject->paKeyScale + (i + 3);

		if( (p[0]->iX != p[1]->iX) || (p[0]->iY != p[1]->iY) || (p[0]->iZ != p[1]->iZ) )
		{
			i++;
			continue;
		}
		
		if( (p[0]->iX != p[2]->iX) || (p[0]->iY != p[2]->iY) || (p[0]->iZ != p[2]->iZ) )
		{
			i++;
			continue;
		}
		
		if( (p[0]->iX != p[3]->iX) || (p[0]->iY != p[3]->iY) || (p[0]->iZ != p[3]->iZ) )
		{
			i++;
			continue;
		}

		p[1]->iFrame = p[3]->iFrame;
		
		for( int j = (i + 4); j < iNewSize; j++ )
		{
			RawSMDKeyScale * c1 = psRawObject->paKeyScale + (j - 2);
			RawSMDKeyScale * c2 = psRawObject->paKeyScale + (j + 0);

			CopyMemory( c1, c2, sizeof( RawSMDKeyScale ) );
		}

		iNewSize -= 2;
	}

	if( iNewSize < psRawObject->iKeyScaleCount )
	{
		//WRITEDBG( "Merged Key Scales (%d -> %d)", psRawObject->iKeyScaleCount, iNewSize );

		RawSMDKeyScale * paOldArray = psRawObject->paKeyScale;
		psRawObject->paKeyScale = new RawSMDKeyScale[iNewSize];
		CopyMemory( psRawObject->paKeyScale, paOldArray, sizeof( RawSMDKeyScale ) * iNewSize );
		psRawObject->iKeyScaleCount = iNewSize;
		delete[] paOldArray;

		return TRUE;
	}

	return FALSE;
}

BOOL FixKeyScalingArray( RawSMDObject * psRawObject, int iLastFrame )
{
	if( psRawObject->iKeyScaleCount > 0 )
	{
		if( psRawObject->paKeyScale[psRawObject->iKeyScaleCount-1].iFrame != iLastFrame )
		{
			RawSMDKeyScale * paOldArray = psRawObject->paKeyScale;
			psRawObject->paKeyScale = new RawSMDKeyScale[psRawObject->iKeyScaleCount+1];
			CopyMemory( psRawObject->paKeyScale, paOldArray, sizeof( RawSMDKeyScale ) * psRawObject->iKeyScaleCount );
			psRawObject->paKeyScale[psRawObject->iKeyScaleCount] = paOldArray[psRawObject->iKeyScaleCount-1];
			psRawObject->paKeyScale[psRawObject->iKeyScaleCount].iFrame = iLastFrame;
			psRawObject->iKeyScaleCount++;
			delete[] paOldArray;

			return TRUE;
		}
	}

	return FALSE;
}

void AddINXAnimation( RawINXFile * psRawFile, RawINXAnimation * psRawAnimation )
{
	unsigned int uIndex = psRawFile->uAnimationCount;

	if( uIndex >= _countof( psRawFile->saAnimation ) )
		return;

	CopyMemory( psRawFile->saAnimation + uIndex, psRawAnimation, sizeof( RawINXAnimation ) );

	psRawFile->uAnimationCount++;
}

void CAnimationHandler::PrintInfoINX( const char * pszFilePath )
{
	//Prepare INX File
	Import::INXFile inxFile( pszFilePath );

	//Load INX File
	//WRITEDBG( "Loading INX File %s", pszFilePath );
	if( !inxFile.Load() )
	{
		//WRITEDBG( "Cant Load INX File %s", pszFilePath );
		return;
	}

	//Open Print File
	std::ofstream ofs;
	ofs.open( "print_inx.txt", std::ofstream::app );

	if( !ofs.is_open() )
	{
		//WRITEDBG( "Error! Print file is not open!" );
		inxFile.Unload();
		return;
	}

	ofs << "Printing Animation List of File " << pszFilePath << std::endl;

	//Print Animations
	ofs << "Animations: " << inxFile.sRawFile.uAnimationCount << std::endl;
	unsigned int i = 10;
	while( (i < 512) && (inxFile.sRawFile.saAnimation[i].iType != 0) )
	{
		RawINXAnimation * psRawAnimation = inxFile.sRawFile.saAnimation + i;

		ofs << "Animation [" << (i - 9) << "/" << (inxFile.sRawFile.uAnimationCount - 10) << "]" << std::endl;
		//ofs << "Type(" << AnimationTypeToString( (EAnimationType)psRawAnimation->iType ) << ")(0x" << std::hex << psRawAnimation->iType << ")" << std::dec << std::endl;
		ofs << "Start(" << psRawAnimation->iStartFrame << ")" << std::endl;
		ofs << "End(" << psRawAnimation->iEndFrame << ")" << std::endl;
		ofs << "Events(" << psRawAnimation->iaEventFrame[0] << "," << psRawAnimation->iaEventFrame[1] << "," << psRawAnimation->iaEventFrame[2] << "," << psRawAnimation->iaEventFrame[3] << ")" << std::endl;

		/*ofs << "Item Count(" << psRawAnimation->iItemIndexCount << ")" << std::endl;
		if( psRawAnimation->iItemIndexCount > 0 )
		{
			ofs << "Item List (";
			for( int j = 0; j < psRawAnimation->iItemIndexCount; j++ )
			{
				if( j != 0 )
					ofs << ",";

				ofs << (int)psRawAnimation->uaItemIndex[j];
			}
			ofs << ")" << std::endl;
		}*/

		ofs << "ClassFlag(0x" << std::hex << psRawAnimation->uClassFlag << ")" << std::dec << std::endl;
		ofs << "Skills(" << (int)psRawAnimation->uaSkill[0] << "," << (int)psRawAnimation->uaSkill[1] << "," << (int)psRawAnimation->uaSkill[2] << "," << (int)psRawAnimation->uaSkill[3] << "," << (int)psRawAnimation->uaSkill[4] << "," << (int)psRawAnimation->uaSkill[5] << "," << (int)psRawAnimation->uaSkill[6] << "," << (int)psRawAnimation->uaSkill[7] << ")" << std::endl;
		ofs << "Loop(" << psRawAnimation->iLoop << ")" << std::endl;
		ofs << "AreaType(" << psRawAnimation->iAreaType << ")" << std::endl;
		ofs << "Values(" << (int)psRawAnimation->uaValue[0] << "," << (int)psRawAnimation->uaValue[1] << "," << (int)psRawAnimation->uaValue[2] << "," << (int)psRawAnimation->uaValue[3] << ")" << std::endl;
		ofs << "SubFileFrameNumber(" << psRawAnimation->iSubFileFrameNumber << ")" << std::endl;

		ofs << std::endl;

		i++;
	}

	//Close Print File
	ofs.close();

	//Unload INX File
	inxFile.Unload();
}

void CAnimationHandler::FixSMBINXSubFileMotion( const char * pszSMBFile, const char * pszINXFile, const char * pszSMBFileOut, const char * pszINXFileOut )
{
	//WRITEDBG( "FixSMBINXSubFileMotion(%s,%s) Working...", pszSMBFile, pszINXFile );

	Import::SMDFile impSMDFile( pszSMBFile );
	Export::SMDFile expSMDFile( pszSMBFileOut );

	Import::INXFile impINXFile( pszINXFile );
	Export::INXFile expINXFile( pszINXFileOut );

	if( !impSMDFile.Load() )
	{
		//WRITEDBG( "FixSMBINXSubFileMotion() failed to load SMB" );
		return;
	}

	if( !impINXFile.Load() )
	{
		//WRITEDBG( "FixSMBINXSubFileMotion() failed to load INX" );
		return;
	}

	if( impSMDFile.sRawHeader.iObjectCount <= 0 )
	{
		//WRITEDBG( "FixSMBINXSubFileMotion() no objects in SMB, there's no point to this!" );
		return;
	}

	//WRITEDBG( "Frame Count: %d", impSMDFile.sRawHeader.iFrameCount );

	if( impSMDFile.sRawHeader.iFrameCount <= 1 )
	{
		//WRITEDBG( "FixSMBINXSubFileMotion() SMB Frame Count is already 0 or 1... there's no point to this!" );
//		return;
	}

	int iLastFrame = 0;

	//Fix SMB Header
	{
		RawSMDFrame * pFirst = impSMDFile.sRawHeader.aFrame + 0;
		pFirst->iStartFrame = 0;
		for( int i = 1; i < impSMDFile.sRawHeader.iFrameCount; i++ )
		{
			RawSMDFrame * p = impSMDFile.sRawHeader.aFrame + i;

			//WRITEDBG( "Frame[%d] : (%d,%d)", i, p->iStartFrame, p->iEndFrame );

			pFirst->iEndFrame += (p->iEndFrame - p->iStartFrame);

			ZeroMemory( p, sizeof( RawSMDFrame ) );
		}
		pFirst->iKeyFrameStartIndex	= 0;
		pFirst->iKeyFrameCount		= 0;
		impSMDFile.sRawHeader.iFrameCount = 1;

		iLastFrame = pFirst->iEndFrame;
	}

	//WRITEDBG( "Last Frame: %d", iLastFrame );
	
	//Fix INX Animations
	{
		RawSMDObject * psFirstObject = impSMDFile.psaRawObject + 0;

		unsigned int i = 10;
		while( (i < 512) && (impINXFile.sRawFile.saAnimation[i].iType != 0) )
		{
			RawINXAnimation * psRawAnimation = impINXFile.sRawFile.saAnimation + i;

			int iIndex = psRawAnimation->iSubFileFrameNumber - 1;

			if( iIndex < 0 )
			{
				//WRITEDBG( "Weird Index in INX Animation [%d] -> %d", i, iIndex );
				return;
			}

			if( iIndex >= psFirstObject->iFrameCount )
			{
				//WRITEDBG( "Too High Index in INX Animation [%d] : [%d] >= [%d]", i, iIndex, psFirstObject->iFrameCount );
				return;
			}

			psRawAnimation->iStartFrame += (psFirstObject->aRotationFrame[iIndex].iStartFrame / 160);
			psRawAnimation->iEndFrame += (psFirstObject->aRotationFrame[iIndex].iStartFrame / 160);

			psRawAnimation->iSubFileFrameNumber = 1;

			i++;
		}
	}

	//Split Animations beetwen Frames MAX 4200 Frames
	BOOL bSplitKeyFrames = FALSE;

	if ( USE_DYNAMIIC_FRAMES_OPTIMIZED )
	{
		if ( iLastFrame > ((_countof( impSMDFile.sRawHeader.aFrame ) - 8) * 160) )
		{
			impSMDFile.sRawHeader.iFrameCount = SplitFrames( &impINXFile.sRawFile, impSMDFile.sRawHeader.aFrame, _countof( impSMDFile.sRawHeader.aFrame ), (_countof( impSMDFile.sRawHeader.aFrame ) - 8), true );

			bSplitKeyFrames = TRUE;
		}
	}


	//Fix SMB Objects Key Frames
	for( int i = 0; i < impSMDFile.sRawHeader.iObjectCount; i++ )
	{
		//WRITEDBG( "----------------------------------------------" );

		RawSMDObject * psRawObject = impSMDFile.psaRawObject + i;
		RawSMDObject * psRawObjectParent = NULL;

		if( psRawObject->szParentObjectName[0] != 0 )
		{
			for( int j = 0; j < impSMDFile.sRawHeader.iObjectCount; j++ )
			{
				RawSMDObject * p = impSMDFile.psaRawObject + j;

				if( STRINGCOMPARE( p->szObjectName, psRawObject->szParentObjectName ) )
				{
					psRawObjectParent = p;
					break;
				}
			}

			if( !psRawObjectParent )
			{
				//WRITEDBG( "Warning! Parent Object %s not found of Object %s", psRawObject->szParentObjectName, psRawObject->szObjectName );
			}
		}

		//WRITEDBG( "Object %s (Parent: %s)", psRawObject->szObjectName, psRawObjectParent ? psRawObjectParent->szObjectName : "None" );

		//WRITEDBG( "Object %s has %d Key Rotations", psRawObject->szObjectName, psRawObject->iKeyRotationCount );
		//WRITEDBG( "Object %s has %d Key Positions", psRawObject->szObjectName, psRawObject->iKeyPositionCount );
		//WRITEDBG( "Object %s has %d Key Scales", psRawObject->szObjectName, psRawObject->iKeyScaleCount );
		
		RawSMDKeyRotation sKeyRotation;
		sKeyRotation.fW = 0.0f;
		sKeyRotation.fX = 0.0f;
		sKeyRotation.fY = 0.0f;
		sKeyRotation.fZ = 0.0f;
		
		RawSMDKeyRotation sKeyRotationTemp;
		sKeyRotationTemp.fW = psRawObject->sRotationQuaternion.fW;
		sKeyRotationTemp.fX = psRawObject->sRotationQuaternion.fX;
		sKeyRotationTemp.fY = psRawObject->sRotationQuaternion.fY;
		sKeyRotationTemp.fZ = psRawObject->sRotationQuaternion.fZ;

		RawSMDMatrixF sRotation;
		sRotation = *(RawSMDMatrixF*)&EXEMatrixConvertIF( *(EXEMatrixI*)&psRawObject->sRotationMatrix );

		//WRITEDBG( "Object %s default Key Rotation: W %.4f X %.4f Y %.4f Z %.4f", psRawObject->szObjectName, sKeyRotationTemp.fW, sKeyRotationTemp.fX, sKeyRotationTemp.fY, sKeyRotationTemp.fZ );

		RawSMDKeyPosition sKeyPosition;
		sKeyPosition.fX = (float)(psRawObject->sTranslationVector.iX) / 256.0f;
		sKeyPosition.fY = (float)(psRawObject->sTranslationVector.iY) / 256.0f;
		sKeyPosition.fZ = (float)(psRawObject->sTranslationVector.iZ) / 256.0f;

		//WRITEDBG( "Object %s default Key Position: X %.4f Y %.4f Z %.4f", psRawObject->szObjectName, sKeyPosition.fX, sKeyPosition.fY, sKeyPosition.fZ );
		
		RawSMDKeyScale sKeyScale;
		sKeyScale.iX = psRawObject->sScalingVector.iX;
		sKeyScale.iY = psRawObject->sScalingVector.iY;
		sKeyScale.iZ = psRawObject->sScalingVector.iZ;

		//WRITEDBG( "Object %s default Key Scale: X %d Y %d Z %d", psRawObject->szObjectName, sKeyScale.iX, sKeyScale.iY, sKeyScale.iZ );
		
		RawSMDFrame * pRot = psRawObject->aRotationFrame + 0;
		RawSMDFrame * pTra = psRawObject->aTranslationFrame + 0;
		RawSMDFrame * pScl = psRawObject->aScalingFrame + 0;

		pRot->iKeyFrameStartIndex	= 0;
		pTra->iKeyFrameStartIndex	= 0;
		pScl->iKeyFrameStartIndex	= 0;

		BOOL bObjectKeysChanged = FALSE;

		for( int z = 1; z < psRawObject->iFrameCount; z++ )
		{
			RawSMDFrame * pRotTemp = psRawObject->aRotationFrame + z;
			RawSMDFrame * pTraTemp = psRawObject->aTranslationFrame + z;
			RawSMDFrame * pSclTemp = psRawObject->aScalingFrame + z;

			pRot->iEndFrame += (pRotTemp->iEndFrame - pRotTemp->iStartFrame);
			pTra->iEndFrame += (pTraTemp->iEndFrame - pTraTemp->iStartFrame);
			pScl->iEndFrame += (pSclTemp->iEndFrame - pSclTemp->iStartFrame);

			//No Rotation Controls?
			if( pRotTemp->iKeyFrameCount == 0 )
			{
				int iIndex = FindKeyRotationIndex( psRawObject, pRotTemp->iStartFrame, pRotTemp->iEndFrame );

				InjectKeyRotation( psRawObject, iIndex, pRotTemp->iStartFrame, &sKeyRotation, &sRotation );
				InjectKeyRotation( psRawObject, iIndex + 1, pRotTemp->iEndFrame, &sKeyRotation, &sRotation );

				bObjectKeysChanged = TRUE;

				pRotTemp->iKeyFrameCount += 2;
			}
				
			//No Translation Controls?
			if( pTraTemp->iKeyFrameCount == 0 )
			{
				int iIndex = FindKeyPositionIndex( psRawObject, pTraTemp->iStartFrame, pTraTemp->iEndFrame );

				InjectKeyPosition( psRawObject, iIndex, pTraTemp->iStartFrame, &sKeyPosition );
				InjectKeyPosition( psRawObject, iIndex + 1, pTraTemp->iEndFrame, &sKeyPosition );

				bObjectKeysChanged = TRUE;

				pTraTemp->iKeyFrameCount += 2;
			}
				
			//No Scaling Controls?
			if( pSclTemp->iKeyFrameCount == 0 )
			{
				int iIndex = FindKeyScaleIndex( psRawObject, pSclTemp->iStartFrame, pSclTemp->iEndFrame );

				InjectKeyScale( psRawObject, iIndex, pSclTemp->iStartFrame, &sKeyScale );
				InjectKeyScale( psRawObject, iIndex + 1, pSclTemp->iEndFrame, &sKeyScale );

				bObjectKeysChanged = TRUE;

				pSclTemp->iKeyFrameCount += 2;
			}

			pRot->iKeyFrameCount += pRotTemp->iKeyFrameCount;
			pTra->iKeyFrameCount += pTraTemp->iKeyFrameCount;
			pScl->iKeyFrameCount += pSclTemp->iKeyFrameCount;
		}

		psRawObject->iFrameCount = 1;

		if ( USE_DYNAMIIC_FRAMES_OPTIMIZED )
		{
			if ( bSplitKeyFrames )
			{
				psRawObject->iFrameCount = SplitFrames( &impINXFile.sRawFile, psRawObject->aRotationFrame, _countof( psRawObject->aRotationFrame ), (_countof( psRawObject->aRotationFrame ) - 8) );
				SplitFrames( &impINXFile.sRawFile, psRawObject->aScalingFrame, _countof( psRawObject->aScalingFrame ), (_countof( psRawObject->aScalingFrame ) - 8) );
				SplitFrames( &impINXFile.sRawFile, psRawObject->aTranslationFrame, _countof( psRawObject->aTranslationFrame ), (_countof( psRawObject->aTranslationFrame ) - 8) );
			}
		}
		
		if( MergeKeyRotations( psRawObject ) )
		{
			bObjectKeysChanged = TRUE;
		}

		if( MergeKeyPositions( psRawObject ) )
		{
			bObjectKeysChanged = TRUE;
		}

		if( MergeKeyScalings( psRawObject ) )
		{
			bObjectKeysChanged = TRUE;
		}

		if( FixKeyRotationArray( psRawObject, iLastFrame ) )
		{
			//WRITEDBG( "Fixed Key Rotation Array of Object %s", psRawObject->szObjectName );

			bObjectKeysChanged = TRUE;
		}

		if( FixKeyPositionArray( psRawObject, iLastFrame ) )
		{
			//WRITEDBG( "Fixed Key Position Array of Object %s", psRawObject->szObjectName );

			bObjectKeysChanged = TRUE;
		}

		if( FixKeyScalingArray( psRawObject, iLastFrame ) )
		{
			//WRITEDBG( "Fixed Key Scaling Array of Object %s", psRawObject->szObjectName );

			bObjectKeysChanged = TRUE;
		}

		pRot->iKeyFrameCount	= psRawObject->iKeyRotationCount;
		pTra->iKeyFrameCount	= psRawObject->iKeyPositionCount;
		pScl->iKeyFrameCount	= psRawObject->iKeyScaleCount;

		//WRITEDBG( "Object %s now has %d Key Rotations", psRawObject->szObjectName, psRawObject->iKeyRotationCount );
		//WRITEDBG( "Object %s now has %d Key Positions", psRawObject->szObjectName, psRawObject->iKeyPositionCount );
		//WRITEDBG( "Object %s now has %d Key Scales", psRawObject->szObjectName, psRawObject->iKeyScaleCount );

		if( bObjectKeysChanged )
		{
			//WRITEDBG( "Object %s Has Changed! Health Check...", psRawObject->szObjectName );
			
			if( psRawObject->iKeyRotationCount > 0 )
			{
				RawSMDKeyRotation * prev = psRawObject->paKeyRotation + 0;

				for( int a = 1; a < psRawObject->iKeyRotationCount; a++ )
				{
					RawSMDKeyRotation * p = psRawObject->paKeyRotation + a;

					if( p->iFrame < prev->iFrame )
					{
						//WRITEDBG( "Key Rotation [%d/%d] Invalid Frame [%d <= %d]", a, psRawObject->iKeyRotationCount, p->iFrame, prev->iFrame );
					}

					prev = p;
				}
			}
			
			if( psRawObject->iKeyPositionCount > 0 )
			{
				RawSMDKeyPosition * prev = psRawObject->paKeyPosition + 0;

				for( int a = 1; a < psRawObject->iKeyPositionCount; a++ )
				{
					RawSMDKeyPosition * p = psRawObject->paKeyPosition + a;

					if( p->iFrame < prev->iFrame )
					{
						//WRITEDBG( "Key Position [%d/%d] Invalid Frame [%d <= %d]", a, psRawObject->iKeyPositionCount, p->iFrame, prev->iFrame );
					}

					prev = p;
				}
			}
			
			if( psRawObject->iKeyScaleCount > 0 )
			{
				RawSMDKeyScale * prev = psRawObject->paKeyScale + 0;

				for( int a = 1; a < psRawObject->iKeyScaleCount; a++ )
				{
					RawSMDKeyScale * p = psRawObject->paKeyScale + a;

					if( p->iFrame < prev->iFrame )
					{
						//WRITEDBG( "Key Scale [%d/%d] Invalid Frame [%d <= %d]", a, psRawObject->iKeyScaleCount, p->iFrame, prev->iFrame );
					}

					prev = p;
				}
			}

			//WRITEDBG( "Object %s Health Check Finished", psRawObject->szObjectName );
		}
	}

	//Save SMB
	expSMDFile.SaveModel( &impSMDFile.sRawHeader, &impSMDFile.sRawMaterialHeader, impSMDFile.psaRawObject, impSMDFile.sModelVersion );

	//Save INX
	expINXFile.Save( &impINXFile.sRawFile );

	//Unload SMB
	impSMDFile.Unload();

	//Done!
	//WRITEDBG( "FixSMBINXSubFileMotion(%s,%s) Finished!", pszSMBFile, pszINXFile );
}

BOOL CAnimationHandler::InjectAnimation( const char * pszSMBFileFrom, const char * pszSMBFileTo, const char * pszINXFileTo, int eAnimationType, int iAreaType, BOOL bLoop, unsigned int uClassFlag, unsigned int uSkillCode )
{
	//WRITEDBG( "InjectionAnimation(%s,%s,%s) Working...", pszSMBFileFrom, pszSMBFileTo, pszINXFileTo );

	Import::SMDFile smbI1( pszSMBFileFrom );
	Import::SMDFile smbI2( pszSMBFileTo );

	if( !smbI1.Load() )
	{
		//WRITEDBG( "Can't load SMB: %s", pszSMBFileFrom );
		return FALSE;
	}

	if( !smbI2.Load() )
	{
		//WRITEDBG( "Can't load SMB: %s", pszSMBFileTo );
		return FALSE;
	}

	Import::INXFile inxI( pszINXFileTo );

	if( !inxI.Load() )
	{
		//WRITEDBG( "Can't load INX: %s", pszINXFileTo );
		return FALSE;
	}

	int iFrameIndex = 0;

	int iNewLastFrame = smbI2.sRawHeader.aFrame[0].iEndFrame;
	int iNewAnimationStartFrame = iNewLastFrame;

	//Find the Last Frame first
	BOOL bFoundKeyFrames = FALSE;
	for( int i = 0; i < smbI1.sRawHeader.iObjectCount; i++ )
	{
		const RawSMDObject * psRawObjectFrom = smbI1.psaRawObject + i;

		if( psRawObjectFrom->iKeyRotationCount > 0 )
		{
			iNewLastFrame += psRawObjectFrom->paKeyRotation[psRawObjectFrom->iKeyRotationCount-1].iFrame;

			bFoundKeyFrames = TRUE;
			break;
		}

		if( psRawObjectFrom->iKeyPositionCount > 0 )
		{
			iNewLastFrame += psRawObjectFrom->paKeyPosition[psRawObjectFrom->iKeyPositionCount-1].iFrame;

			bFoundKeyFrames = TRUE;
			break;
		}

		if( psRawObjectFrom->iKeyScaleCount > 0 )
		{
			iNewLastFrame += psRawObjectFrom->paKeyScale[psRawObjectFrom->iKeyScaleCount-1].iFrame;

			bFoundKeyFrames = TRUE;
			break;
		}
	}

	if( !bFoundKeyFrames )
	{
		//WRITEDBG( "No Animation Data (Key Frames) found in the source SMB file.. this is pointless!" );
		return FALSE;
	}

	//Loop through Objects in SMB to read animation from
	for( int i = 0; i < smbI1.sRawHeader.iObjectCount; i++ )
	{
		const RawSMDObject * psRawObjectFrom = smbI1.psaRawObject + i;
		RawSMDObject * psRawObjectTo = NULL;

		//Find Object in Motion SMB
		int iNewObjectIndex = -1;
		for( int j = 0; j < smbI2.sRawHeader.iObjectCount; j++ )
		{
			RawSMDObject * psTemp = smbI2.psaRawObject + j;

			if( STRINGCOMPARE( psRawObjectFrom->szObjectName, psTemp->szObjectName ) )
			{
				iNewObjectIndex = j;
				psRawObjectTo = psTemp;
				break;
			}
		}

		//Not Found?
		if( psRawObjectTo == NULL )
		{
			//WRITEDBG( "Object %s not found in Motion SMB, ignoring...", psRawObjectFrom->szObjectName );
			continue;
		}
		
		//WRITEDBG( "[%d][%d] Object %s Copying Animation Key Frames...", i, iNewObjectIndex, psRawObjectFrom->szObjectName );

		RawSMDKeyRotation sKeyRotation;
		sKeyRotation.fW = 0.0f;
		sKeyRotation.fX = 0.0f;
		sKeyRotation.fY = 0.0f;
		sKeyRotation.fZ = 0.0f;

		RawSMDMatrixF sRotation;
		sRotation = *(RawSMDMatrixF*)&EXEMatrixConvertIF( *(EXEMatrixI*)&psRawObjectFrom->sRotationMatrix );

		RawSMDKeyPosition sKeyPosition;
		sKeyPosition.fX = (float)(psRawObjectFrom->sTranslationVector.iX) / 256.0f;
		sKeyPosition.fY = (float)(psRawObjectFrom->sTranslationVector.iY) / 256.0f;
		sKeyPosition.fZ = (float)(psRawObjectFrom->sTranslationVector.iZ) / 256.0f;

		RawSMDKeyScale sKeyScale;
		sKeyScale.iX = psRawObjectFrom->sScalingVector.iX;
		sKeyScale.iY = psRawObjectFrom->sScalingVector.iY;
		sKeyScale.iZ = psRawObjectFrom->sScalingVector.iZ;

		RawSMDFrame * psNewRotFrame = psRawObjectTo->aRotationFrame + 0;
		RawSMDFrame * psNewTraFrame = psRawObjectTo->aTranslationFrame + 0;
		RawSMDFrame * psNewSclFrame = psRawObjectTo->aScalingFrame + 0;

		//WRITEDBG( "Object %s Frame Rot Cnt: %d", psRawObjectFrom->szObjectName, psRawObjectFrom->iKeyRotationCount );
		//WRITEDBG( "Object %s Frame Tra Cnt: %d", psRawObjectFrom->szObjectName, psRawObjectFrom->iKeyPositionCount );
		//WRITEDBG( "Object %s Frame Scl Cnt: %d", psRawObjectFrom->szObjectName, psRawObjectFrom->iKeyScaleCount );

		if( psRawObjectFrom->iKeyRotationCount > 0 )
		{
			for( int k = 0; k < psRawObjectFrom->iKeyRotationCount; k++ )
			{
				RawSMDKeyRotation * psKeyRotation = psRawObjectFrom->paKeyRotation + k;
				RawSMDMatrixF * psRotation = psRawObjectFrom->paRotation + k;

				psKeyRotation->iFrame += iNewAnimationStartFrame;

				InjectKeyRotation( psRawObjectTo, -1, psKeyRotation->iFrame, psKeyRotation, psRotation );
			}

			//WRITEDBG( "Copied %d Key Rotations for Frames %d,%d", psRawObjectFrom->iKeyRotationCount, iNewAnimationStartFrame, iNewLastFrame );
		}
		else
		{
			InjectKeyRotation( psRawObjectTo, -1, iNewAnimationStartFrame, &sKeyRotation, &sRotation );
			InjectKeyRotation( psRawObjectTo, -1, iNewLastFrame, &sKeyRotation, &sRotation );

			psNewRotFrame->iKeyFrameCount += 2;

			//WRITEDBG( "Created Key Rotation for Frames %d,%d", iNewAnimationStartFrame, iNewLastFrame );
		}
			
		if( psRawObjectFrom->iKeyPositionCount > 0 )
		{
			for( int k = 0; k < psRawObjectFrom->iKeyPositionCount; k++ )
			{
				RawSMDKeyPosition * psKeyPosition = psRawObjectFrom->paKeyPosition + k;

				psKeyPosition->iFrame += iNewAnimationStartFrame;

				InjectKeyPosition( psRawObjectTo, -1, psKeyPosition->iFrame, psKeyPosition );
			}

			//WRITEDBG( "Copied %d Key Positions for Frames %d,%d", psRawObjectFrom->iKeyPositionCount, iNewAnimationStartFrame, iNewLastFrame );
		}
		else
		{
			InjectKeyPosition( psRawObjectTo, -1, iNewAnimationStartFrame, &sKeyPosition );
			InjectKeyPosition( psRawObjectTo, -1, iNewLastFrame, &sKeyPosition );

			psNewTraFrame->iKeyFrameCount += 2;

			//WRITEDBG( "Created Key Position for Frames %d,%d", iNewAnimationStartFrame, iNewLastFrame );
		}
			
		if( psRawObjectFrom->iKeyScaleCount > 0 )
		{
			for( int k = 0; k < psRawObjectFrom->iKeyScaleCount; k++ )
			{
				RawSMDKeyScale * psKeyScale = psRawObjectFrom->paKeyScale + k;

				psKeyScale->iFrame += iNewAnimationStartFrame;

				InjectKeyScale( psRawObjectTo, -1, psKeyScale->iFrame, psKeyScale );
			}

			//WRITEDBG( "Copied %d Key Scales for Frames %d,%d", psRawObjectFrom->iKeyScaleCount, iNewAnimationStartFrame, iNewLastFrame );
		}
		else
		{
			InjectKeyScale( psRawObjectTo, -1, iNewAnimationStartFrame, &sKeyScale );
			InjectKeyScale( psRawObjectTo, -1, iNewLastFrame, &sKeyScale );

			psNewSclFrame->iKeyFrameCount += 2;

			//WRITEDBG( "Created Key Scale for Frames %d,%d", iNewAnimationStartFrame, iNewLastFrame );
		}

		MergeKeyRotations( psRawObjectTo );
		MergeKeyPositions( psRawObjectTo );
		MergeKeyScalings( psRawObjectTo );

		psNewRotFrame->iEndFrame = iNewLastFrame;
		psNewTraFrame->iEndFrame = iNewLastFrame;
		psNewSclFrame->iEndFrame = iNewLastFrame;

		psNewRotFrame->iKeyFrameCount = psRawObjectTo->iKeyRotationCount;
		psNewTraFrame->iKeyFrameCount = psRawObjectTo->iKeyPositionCount;
		psNewSclFrame->iKeyFrameCount = psRawObjectTo->iKeyScaleCount;

		//WRITEDBG( "Object %s Done!", psRawObjectFrom->szObjectName );
	}

	//Set New Last Frame in SMB Header
	smbI2.sRawHeader.aFrame[0].iEndFrame = iNewLastFrame;

	//Create INX Animation
	RawINXAnimation sINXAnimation;
	ZeroMemory( &sINXAnimation, sizeof( sINXAnimation ) );

	sINXAnimation.iType = eAnimationType;
	sINXAnimation.iStartFrame = iNewAnimationStartFrame / 160;
	sINXAnimation.iEndFrame = iNewLastFrame / 160;
	sINXAnimation.iAreaType = iAreaType;
	sINXAnimation.iSubFileFrameNumber = 1;
	sINXAnimation.uaSkill[0] = uSkillCode;
	sINXAnimation.iLoop = bLoop;
	sINXAnimation.uClassFlag = uClassFlag;

	//Log
	//WRITEDBG( "Injected INX Animation at Index %d", inxI.sRawFile.uAnimationCount );

	//Insert Animation into INX File
	AddINXAnimation( &inxI.sRawFile, &sINXAnimation );

	//Save SMB
	Export::SMDFile smbO( pszSMBFileTo );
	smbO.SaveModel( &smbI2.sRawHeader, &smbI2.sRawMaterialHeader, smbI2.psaRawObject, smbI2.sModelVersion );

	//Save INX
	Export::INXFile inxO( pszINXFileTo );
	inxO.Save( &inxI.sRawFile );
	
	//Done
	//WRITEDBG( "InjectionAnimation() Done!" );

	return TRUE;
}

void CAnimationHandler::LoadTerrain( char * pszFilePath )
{
	Import::SMDFile impt( pszFilePath );

	impt.Load();


	Import::SMDFile impt2( pszFilePath );

	impt2.Load();
}

int CAnimationHandler::SplitFrames( RawINXFile * psRawInx, RawSMDFrame * psSplit, int iMaxCount, int iCount, bool bUpdateInx )
{
	int iIndexRawFrame = 0;
	int iBeginFrame = 0, iTotalFrames = 0, iAmountFrames = 0;
	auto UpdateFramesLambda = [&iIndexRawFrame, &iBeginFrame, &iTotalFrames, &iAmountFrames, psSplit]() -> int
	{
		if ( iIndexRawFrame >= 23 )
			iIndexRawFrame = 23;

		RawSMDFrame * p = psSplit + iIndexRawFrame;

		p->iStartFrame = iBeginFrame;
		p->iEndFrame = iTotalFrames;

		p->iKeyFrameStartIndex = p->iStartFrame / 160;
		p->iKeyFrameCount = (p->iEndFrame - p->iStartFrame) / 160;

		iBeginFrame = p->iEndFrame;

		iIndexRawFrame++;

		iAmountFrames += iTotalFrames;

		iTotalFrames = 0;

		return 1;
	};

	RawSMDFrame * pFirst = psSplit + 0;

	int iLastFrame = pFirst->iEndFrame;

	int iFrameCount = iLastFrame / iCount;
	if ( (iLastFrame % iCount) != 0 )
		iFrameCount += 160;

	unsigned int i = 10;
	while ( (i < 512) && (psRawInx->saAnimation[i].iType != 0) )
	{
		RawINXAnimation * psRawAnimation = psRawInx->saAnimation + i;

		iTotalFrames = psRawAnimation->iEndFrame * 160;

		if ( iTotalFrames >= iFrameCount )
		{
			UpdateFramesLambda();

			if ( bUpdateInx )
				psRawAnimation->iSubFileFrameNumber = iIndexRawFrame;
		}
		else if ( bUpdateInx )
			psRawAnimation->iSubFileFrameNumber = iIndexRawFrame + 1;

		i++;
	}

	iTotalFrames = (iLastFrame - iAmountFrames);
	if ( iTotalFrames > 0 )
		UpdateFramesLambda();

	return iIndexRawFrame;
}
