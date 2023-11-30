#pragma once
class CItemRenderHandler
{
public:
	enum EItemRenderType
	{
		ITEMRENDERTYPE_None,
		ITEMRENDERTYPE_LeftHand,
		ITEMRENDERTYPE_RightHand,
		ITEMRENDERTYPE_BackSide,
		ITEMRENDERTYPE_Shield,
		ITEMRENDERTYPE_LeftRightHand,
	};

	enum EItemRenderEffectType
	{
		ITEMRENDERTYPEEFFECT_None,
		ITEMRENDERTYPEEFFECT_LeftHand,
		ITEMRENDERTYPEEFFECT_RightHand,
		ITEMRENDERTYPEEFFECT_BackSide,
		ITEMRENDERTYPEEFFECT_Shield,
		ITEMRENDERTYPEEFFECT_LeftRightHand,
	};

private:

	static bool						IdentityMatrix( EXEMatrixI & sMatrix );

	static bool						MultiplyMatrices( EXEMatrixI & sMatrixDest, EXEMatrixI & sMatrixA, EXEMatrixI & sMatrixB );

	static bool						IsShieldStrikeEffect( UnitData * pcUnitData );

	static PTModel_ptr				GetLeftHandModel( UnitData * pcUnitData, bool bTown );
	static PTModel_ptr				GetRightHandModel( UnitData * pcUnitData, bool bTown );
	static PTModel_ptr				GetBackSideModel( UnitData * pcUnitData, bool bTown );

public:
	static bool						RenderEffects( PTModel_ptr pcItemModel, UnitData * pcUnitData, EItemRenderType eRenderType, bool bInTown );

	static bool						Render( PTModel_ptr pcItemModel, UnitData * pcUnitData, ItemID sItemID, EItemRenderType eRenderType, bool bEffects, bool bInTown );

	static bool						RenderItems( UnitData * pcUnitData );
};

