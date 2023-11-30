#pragma once
class CCostumeWindowHandler
{
private:
	enum
	{
		WINDOW_Main,
		WINDOW_Weapon,
		WINDOW_Shield,
		WINDOW_WeaponTwoHand,

		ITEMBOX_Weapon,
		ITEMBOX_Shield,
	};

public:
	CCostumeWindowHandler();
	virtual ~CCostumeWindowHandler();

	void				Init();

	void				Update( float fTime );

	UI::Window_ptr		GetWindow() { return pWindow; }

	ItemData			* GetWeaponItem();
	ItemData			* GetShieldItem();

	BOOL				IsBlockedMouse();

	bool				SetWeaponCostume( ItemData * pcItemData );
	bool				SetShieldCostume( ItemData * pcItemData );

	void				LoadCacheItems();

	void				SwapInventory( BOOL bOpen, bool bFirstLoad = false );

	bool				IsValidWeaponItem();
	bool				IsValidShieldItem();

	bool				IsSpecialCostume( EItemID eItemID );

	void				SetBlockSwap( bool b ) { bBlockSwap = b; }

private:
	UI::Window_ptr		pWindow = nullptr;

	BOOL				bBlockMouse = FALSE;

	bool				bLoadItem = false;

	bool				bBlockSwap = false;

	ItemData			cWeapon;
	ItemData			cShield;

	UI::ItemBox_ptr		GetWeaponItemBox();
	UI::ItemBox_ptr		GetShieldItemBox();

	void				OnGetItemWeapon( UIEventArgs eArgs );
	void				OnBeforePutItemWeapon( UIEventArgs eArgs );
	void				OnPutItemWeapon( UIEventArgs eArgs );

	void				OnGetItemShield( UIEventArgs eArgs );
	void				OnBeforePutItemShield( UIEventArgs eArgs );
	void				OnPutItemShield( UIEventArgs eArgs );

	void				OnExpireWeapon( UIEventArgs eArgs );
	void				OnExpireShield( UIEventArgs eArgs );

	void				HideWeapon();
	void				HideShield();

	void				UpdateTwoHandSlot( BOOL bShow );

	bool				IsDoubleWeapon( ItemData * pcItemData );
};

