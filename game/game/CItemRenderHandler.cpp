#include "stdafx.h"
#include "CItemRenderHandler.h"

bool CItemRenderHandler::IdentityMatrix( EXEMatrixI & sMatrix )
{
	for ( int j = 0; j < 4; j++ )
		for ( int k = 0; k < 4; k++ )
			sMatrix.i[j][k] = (j == k ? 256 : 0);

	return true;
}

bool CItemRenderHandler::MultiplyMatrices( EXEMatrixI & sMatrixDest, EXEMatrixI & sMatrixA, EXEMatrixI & sMatrixB )
{
	sMatrixDest = EXEMatrixMultiplyI( sMatrixA, sMatrixB );

	return true;
}

bool CItemRenderHandler::IsShieldStrikeEffect( UnitData * pcUnitData )
{
	return (pcUnitData->GetAnimation() == ANIMATIONTYPE_Skill && pcUnitData->sCharacterData.iClass == CHARACTERCLASS_Atalanta && (pcUnitData->sActiveSkill.cSkillTinyID == SKILLPLAYID_T11 || pcUnitData->sActiveSkill.cSkillTinyID == SKILLPLAYID_T23) && pcUnitData->pcShieldBone);
}

PTModel_ptr CItemRenderHandler::GetLeftHandModel( UnitData * pcUnitData, bool bTown )
{
	PTModel_ptr pcModel = nullptr;

	//Town?
	if ( bTown )
	{
		if ( pcUnitData->sLeftHandTool.pModel )
			pcModel = pcUnitData->sLeftHandTool.pModel;
	}
	else
	{
		if ( pcUnitData->sLeftHandTool.pModel )
		{
			pcModel = pcUnitData->sLeftHandTool.pModel;

			if ( ItemID( pcUnitData->sLeftHandTool.eItemID ).ToItemType() == ITEMTYPE_Shield )
			{
				if ( (pcUnitData->eSkillDisplayFlag & SKILLMASK_SparkShield) && SPARKSHIELD_MODEL && pcUnitData->sCharacterData.iClass == CHARACTERCLASS_Mechanician )
				{
					if ( pcUnitData->GetAnimation() == ANIMATIONTYPE_Skill && pcUnitData->sActiveSkill.cSkillTinyID == SKILLPLAYID_T34 )
					{

					}
					else
						pcModel = SPARKSHIELD_MODEL;
				}
				if ( (pcUnitData->eSkillDisplayFlag & SKILLMASK_SparkShield) && DIVINESHIELD_MODEL && pcUnitData->sCharacterData.iClass == CHARACTERCLASS_Knight )
				{
					if ( pcUnitData->GetAnimation() == ANIMATIONTYPE_Skill && pcUnitData->sActiveSkill.cSkillTinyID == SKILLPLAYID_T32 )
					{

					}
					else
						pcModel = DIVINESHIELD_MODEL;
				}
				if ( (pcUnitData->eSkillDisplayFlag & SKILLMASK_GodlyShield) && GODLYSHIELD_MODEL && pcUnitData->sCharacterData.iClass == CHARACTERCLASS_Knight )
				{
					if ( pcUnitData->GetAnimation() == ANIMATIONTYPE_Skill && pcUnitData->sActiveSkill.cSkillTinyID == SKILLPLAYID_T42 )
					{

					}
					else
						pcModel = GODLYSHIELD_MODEL;
				}
			}
		}
	}

	return pcModel;
}

PTModel_ptr CItemRenderHandler::GetRightHandModel( UnitData * pcUnitData, bool bTown )
{
	PTModel_ptr pcModel = nullptr;

	return pcModel;
}

PTModel_ptr CItemRenderHandler::GetBackSideModel( UnitData * pcUnitData, bool bTown )
{
	PTModel_ptr pcModel = nullptr;

	return pcModel;
}

bool CItemRenderHandler::RenderEffects( PTModel_ptr pcItemModel, UnitData * pcUnitData, EItemRenderType eRenderType, bool bInTown )
{
	return false;
}

bool CItemRenderHandler::Render( PTModel_ptr pcItemModel, UnitData * pcUnitData, ItemID sItemID, EItemRenderType eRenderType, bool bEffects, bool bInTown )
{
	bool bRet = false;

	if ( bEffects )
		RenderEffects( pcItemModel, pcUnitData, eRenderType, bInTown );

	//Shield?
	if ( eRenderType == ITEMRENDERTYPE_Shield )
	{
		if ( CItemRenderHandler::IsShieldStrikeEffect( pcUnitData ) )
		{
			//Set Parent

		}

		//Has Model?
		if ( auto pcModel = CItemRenderHandler::GetLeftHandModel( pcUnitData, bInTown ) )
		{
			//Has Mesh?
			if ( pcModel->uMeshCount > 0 )
			{
				//Update Matrix
				//MultiplyMatrices( pcModel->GetMeshes[0]->GetWorldMatrix(),)
			}
		}
	}

	return bRet;
}

bool CItemRenderHandler::RenderItems( UnitData * pcUnitData )
{
	return false;
}
