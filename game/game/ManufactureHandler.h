#pragma once

struct ManufactureRune
{
	int RecipeCode;
	int NeedRuneIndex[3];
};


class CManufactureHandler
{

public:
	CManufactureHandler();
	virtual ~CManufactureHandler();

	void												SetManufactureAreaCheck( class ItemData * pcItem );

	BOOL												OnClickItemSlotPut( class ItemData * pcItem );

	void												SetManufacture();

	void												Update();

private:
	BOOL												IsRecipe( EItemID eID );

	BOOL												CanRespec( EItemID eID );

	BOOL												IsRespecStone( EItemID eID );

	BOOL												IsRune( EItemID eID );

	BOOL												IsValidRuneTullaAmulet( EItemID eID );
	BOOL												IsValidRuneThroneAmulet( EItemID eID );
	BOOL												IsValidRuneAbyssRing( EItemID eID );

	BOOL												IsValidRuneFurySheltom( EItemID eID );

	BOOL												HasFurySheltomRune( EItemID eID );

	BOOL												IsValidRuneThroneSheltom(EItemID eID);

	BOOL												HasThroneSheltomRune(EItemID eID);

	BOOL												HaveSlot( EItemID iItemRecipeID, EItemID eRuneID, int iRuneIndex, bool bUniqueSlot, EItemID iItemSlot1ID, int iCount1, EItemID iItemSlot2ID = ITEMID_None, int iCount2 = 0, EItemID iItemSlot3ID = ITEMID_None, int iCount3 = 0 );
};

