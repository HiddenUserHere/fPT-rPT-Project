#pragma once
#include "CWareHouseWindow.h"

class CWarehouse
{
protected:
	class CWarehouseWindow						* pcWarehouseWindow = NULL;
public:
	CWarehouse();
	virtual ~CWarehouse();

	class CWarehouseWindow						* GetWindow() { return pcWarehouseWindow; };

	void										Init();

	BOOL										OnMouseClick( class CMouse * pcMouse );

	int											GetPage() { return (*(int*)0x3693304) + 1; };
	void										SetPage(int iPage);

	int											OnRenderTime(struct Skill * psSkill );

	void										OpenNPCQuest();

	ItemData									* GetItemData();

	void										HandlePacket( struct PacketWarehouse * psPacket );

	BOOL										OnUnitKilled( class UnitData * pcUnit );

	void										RestoreItems();

	BOOL										IsWarehouseWindowOpen() { return (*(BOOL*)0x036932FC); }

	ItemData									* GetMouseItemHover();
};

