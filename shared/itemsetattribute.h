#pragma once

enum EItemSetAttributeBonus
{
	ITEMBONUSATTRIBUTE_None,
	ITEMBONUSATTRIBUTE_HP,
	ITEMBONUSATTRIBUTE_MP,
	ITEMBONUSATTRIBUTE_STM,
	ITEMBONUSATTRIBUTE_DamageMin,
	ITEMBONUSATTRIBUTE_DamageMax,
	ITEMBONUSATTRIBUTE_Defense,
	ITEMBONUSATTRIBUTE_AttackSpeed,
	ITEMBONUSATTRIBUTE_AttackRating,
	ITEMBONUSATTRIBUTE_CooldownReduction,
	ITEMBONUSATTRIBUTE_Evade,
	ITEMBONUSATTRIBUTE_Critical,
};

enum EItemAttributeType
{
	ITEMATTRIBUTE_None,
	ITEMATTRIBUTE_ItemBase,
	ITEMATTRIBUTE_ItemType,
	ITEMATTRIBUTE_ItemID,
	ITEMATTRIBUTE_ItemLevel,
};

class CItemAttributeBonus
{
public:
	CItemAttributeBonus( ECharacterClass iClass, EItemSetAttributeBonus iBonusAdd, float fValueAdd, int iItemCount, BOOL bPercentType = FALSE );
	~CItemAttributeBonus();

	ECharacterClass			GetSpec() { return iClass; }

	void					SetSpec( ECharacterClass e ) { iClass = e; }

	EItemSetAttributeBonus	GetBonus() { return iBonus; }
	float					GetValue() { return fValue; }

	int						GetItemCount() { return iItemCount; }

	BOOL					IsPercent() { return bPercent; }

private:
	ECharacterClass			iClass;

	int						iItemCount;

	EItemSetAttributeBonus	iBonus;
	float					fValue;

	BOOL					bPercent;
};


class CItemAttribute
{
public:
	struct ItemAttributeText
	{
		union 
		{
			int				iItemCount;
			int				iID;
		};
		std::string			strText;

		ECharacterClass		iClass;
	};

	struct ItemDataAttr
	{
		int					iTextID;

		EItemAttributeType	iType;
		int					iItem;
		int					iCount;
	};

	CItemAttribute();
	virtual ~CItemAttribute();

	int						GetID() { return iID; }
	void					SetID( int i ) { iID = i; }

	BOOL					IsUnique() { return bUnique; }
	void					SetUnique( BOOL b ) { bUnique = b; }

	std::string				GetName() { return strName; }
	void					SetName( std::string strNameAttr ) { strName = strNameAttr; }

	void					AddItem( int iTextID, EItemAttributeType iType, int iItem, int iCount = 1 );

	void					AddBonus( CItemAttributeBonus cBonus );

	void					AddTextItem( int iID, std::string strText );
	void					AddTextEffect( int iItemCount, std::string strText, ECharacterClass iClass );

	std::vector<ItemDataAttr *> & GetItems() { return vItems; }
	std::vector<CItemAttributeBonus *> & GetBonuses() { return vBonuses; }

	ItemAttributeText		* GetTextItem( int iID );
	ItemAttributeText		* GetTextEffect( int iID );

	std::vector<ItemAttributeText *> & GetTextEffects() { return vTextEffects; }

private:
	std::vector<ItemAttributeText *> vTextItems;
	std::vector<ItemAttributeText *> vTextEffects;

	int								iID = 0;

	BOOL							bUnique = FALSE;

	std::string						strName;

	std::vector<ItemDataAttr *>			vItems;
	std::vector<CItemAttributeBonus *>	vBonuses;
};