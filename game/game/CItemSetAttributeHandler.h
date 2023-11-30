#pragma once


class CItemAttributeInstance
{
public:

	struct ItemDataEquipped
	{
		CItemAttribute::ItemDataAttr	* psItem;

		int								iCount;
	};

	CItemAttributeInstance();
	~CItemAttributeInstance();

	CItemAttribute					* GetItemAttribute() { return pcItemAttribute; }
	void							SetItemAttribute( CItemAttribute * pc ) { pcItemAttribute = pc; }

	void							Update();

	std::vector<CItemAttributeBonus *>	GetBonuses( int iItemCount );
	std::vector<CItemAttributeBonus *>	GetHighestBonuses();

	std::vector<ItemDataEquipped *>	& GetEquippedItems() { return vEquippedItems; }
	
	int								GetItemCount();

private:
	CItemAttribute					* pcItemAttribute;

	std::vector<ItemDataEquipped *>	vEquippedItems;
};


class CItemSetAttributeHandler
{
public:
	CItemSetAttributeHandler();
	virtual ~CItemSetAttributeHandler();

	void							Init();

	std::vector<CItemAttributeInstance *> & GetInstances() { return vInstances; }

	CItemAttributeInstance			* GetAttributes( Item * psItem, BOOL bReverse = FALSE );

	void							AddInstance( CItemAttributeInstance * pc ) { vInstances.push_back( pc ); };

	void							UpdateCharacterData();

	void							UpdateCharacterDataDefense();

	void							Update();


private:
	std::vector<CItemAttributeInstance *> vInstances;

	CItemAttributeInstance * pcCurrentInstance = nullptr;

	CItemAttributeInstance * GetInstance( int _iID );
};

