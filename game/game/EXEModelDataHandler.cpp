#include "stdafx.h"
#include "EXEModelDataHandler.h"

NAKED BOOL __cdecl DecryptFileModelGate( const char * pszFileName, ModelData * psModelAnimation )
{
	JMP( 0x0044F110 );
}

NAKED PTModel * __cdecl LoadPTModelGate( const char * pszFileName, const char * pszFileModelOther = nullptr )
{
	JMP( 0x0047D860 );
}

NAKED PTModel * __cdecl LoadPTModelBoneGate( const char * pszFileName )
{
	JMP( 0x0047B360 );
}

PTModel * LoadPTModelGate1( const char * pszFileName, const char * pszFileModelOther )
{
	return LoadPTModelGate( pszFileName, pszFileModelOther );
}

PTModel * LoadPTModelBoneGate1( const char * pszFileName )
{
	return LoadPTModelBoneGate( pszFileName );
}

int EXEModelDataHandler::GetFreeModel()
{
	for ( int i = 0; i < _countof( saModelData ); i++ )
	{
		if ( saModelData[i].pcModel == nullptr )
			return i;
	}

	return -1;
}

int EXEModelDataHandler::GetModelID( const char * pszFileName )
{
	int iRet = -1;

	for ( int i = 0; i < _countof( saModelData ); i++ )
	{
		if ( saModelData[i].pcModel )
		{
			if ( STRINGCOMPAREI( saModelData[i].szModelName, pszFileName ) )
			{
				iRet = i;
				break;
			}
		}
	}

	return iRet;
}

EXEModelData * EXEModelDataHandler::LoadBoneData( const char * pszFile )
{
	EXEModelData * pcBoneData = nullptr;

	int iModelID = GetModelID( pszFile );
	if ( iModelID == -1 )
	{
		if ( iModelID = GetFreeModel(); iModelID >= 0 )
		{
			pcBoneData = saModelData + iModelID;
			
			//Load Bone
			RENDERER->SetUseNewRenderToLoad( true );
			if ( pcBoneData->pcModel = LoadPTModelBoneGate( pszFile ) )
			{
				pcBoneData->psMotion = nullptr;
				STRINGCOPY( pcBoneData->szModelName, pszFile );

				pcBoneData->iCount = 1;
			}
			RENDERER->ResetUseNewRenderToLoad();
		}
	}
	else
	{
		pcBoneData = saModelData + iModelID;

		pcBoneData->iCount++;
	}

	return pcBoneData;
}

EXEModelData * EXEModelDataHandler::LoadModelData( const char * pszFile )
{
	static ModelData sModelData;

	EXEModelData * pcModelData = nullptr;

	int iModelID = GetModelID( pszFile );

	if ( iModelID == -1 )
	{
		if ( DecryptFileModelGate( pszFile, &sModelData ) )
		{
			//Set Bone to NULL (Ugly method used by Old Version)
			EXEModel::SetBone( NULL );

			if ( iModelID = GetFreeModel(); iModelID >= 0 )
			{
				pcModelData = saModelData + iModelID;

				//Has Bone?
				pcModelData->psMotion = nullptr;
				if ( pcModelDataBone && (sModelData.szBoneModelFilePath[0] != 0) )
				{
					if ( auto psBone = pcModelDataBone->LoadBoneData( sModelData.szBoneModelFilePath ) )
					{
						//Set Bone Model File
						EXEModel::SetBone( psBone->pcModel );

						pcModelData->psMotion = psBone;
					}
					else
						pcModelData = nullptr;
				}

				//Has Talk Bone? (used?)
				if ( pcModelData )
				{
					pcModelData->psUnused = nullptr;
					if ( pcModelDataBone && (sModelData.szTalkBoneModelFilePath[0] != 0) )
					{
						if ( auto psTalkBone = pcModelDataBone->LoadBoneData( sModelData.szTalkBoneModelFilePath ) )
						{
							//Set Talk Bone Model File
							pcModelData->psUnused = psTalkBone;
						}
						else
							pcModelData = nullptr;
					}

					//Load the Model
					if ( pcModelData )
					{
						RENDERER->SetUseNewRenderToLoad( true );

						if ( pcModelData->pcModel = LoadPTModelGate( sModelData.szModelPath ) )
						{
							//Has Bone? Update it's frames... otherwise, use model frames
							auto pcModelFrame = pcModelData->psMotion ? pcModelData->psMotion->pcModel : pcModelData->pcModel;

							//Update Frames
							for ( int i = 0; i < sModelData.iNumModelAnimation; i++ )
							{
								int iFrame = pcModelFrame->saFrame[sModelData.saModelAnimation[i].iSubFileFrameNumber - 1].iStartFrame / 160;
								sModelData.saModelAnimation[i].iBeginFrame += iFrame;
								sModelData.saModelAnimation[i].iEndFrame += iFrame;
							}

							//Done
							pcModelData->psModelData = new ModelData;
							CopyMemory( pcModelData->psModelData, &sModelData, sizeof( ModelData ) );
							STRINGCOPY( pcModelData->szModelName, pszFile );
							pcModelData->iCount = 1;

							EXETexture::GetInstance()->ReadTextures();
						}
						else
						{
							pcModelData = nullptr;

							if ( GM_MODE )
								ChatGame::AddChatBoxTextEx( CHATCOLOR_Error, "Loading Error [%s]", sModelData.szModelPath );
						}

						RENDERER->ResetUseNewRenderToLoad();

						if( pcModelData && pcModelData->pcModel && pcModelData->pcModel->pcNewModel )
						{
							if( pcModelData->m_ModelsGroup = new ModelsGroup(); pcModelData->m_ModelsGroup )
							{
								//High
								if( sModelData.sHighMeshQuality.iCount > 0 )
								{
									for( int i = 0; i < 4; i++ )
									{
										if( sModelData.sHighMeshQuality.szaMeshName[i][0] != 0 )
										{
											for( const auto& mesh : pcModelData->pcModel->pcNewModel->GetMeshes( sModelData.sHighMeshQuality.szaMeshName[i] ) )
											{
												if( mesh )
													pcModelData->m_ModelsGroup->HighModel.meshes.push_back( mesh );
											}
										}
									}
								}

								//Low
								if( sModelData.sLowMeshQuality.iCount > 0 )
								{
									for( int i = 0; i < 4; i++ )
									{
										if( sModelData.sLowMeshQuality.szaMeshName[i][0] != 0 )
										{
											for( const auto& mesh : pcModelData->pcModel->pcNewModel->GetMeshes( sModelData.sLowMeshQuality.szaMeshName[i] ) )
											{
												if( mesh )
													pcModelData->m_ModelsGroup->LowModel.meshes.push_back( mesh );
											}
										}
									}
								}

								//Default
								if( sModelData.sMediumMeshQuality.iCount > 0 )
								{
									for( int i = 0; i < 4; i++ )
									{
										if( sModelData.sMediumMeshQuality.szaMeshName[i][0] != 0 )
										{
											for( const auto& mesh : pcModelData->pcModel->pcNewModel->GetMeshes( sModelData.sMediumMeshQuality.szaMeshName[i] ) )
											{
												if( mesh )
													pcModelData->m_ModelsGroup->DefaultModel.meshes.push_back( mesh );
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	else
	{
		pcModelData = saModelData + iModelID;

		pcModelData->iCount++;
	}

	if ( pcModelData )
		ANIMATIONHANDLER->HandleBoneCache( pcModelData );

	return pcModelData;
}
