#pragma once

#include "CCostumeWindowHandler.h"

class CInventoryWindowHandler
{
private:
	enum
	{
		WINDOW_Main,
		BUTTON_LockItems,
		IMAGE_LockItemView,

		IMAGE_EarringBackground1,
		IMAGE_EarringBackground2,

		ITEMBOX_Earring1,
		ITEMBOX_Earring2,

	};

public:
	CInventoryWindowHandler();
	virtual ~CInventoryWindowHandler();

	CCostumeWindowHandler * GetWindowCostume() { return pcCostumeWindowHandler; };

	void				Init();

	void				Render();

	void				Update( float fTime );

	BOOL				OnMouseClick( CMouse * pcMouse );
	void				OnMouseMove( CMouse * pcMouse );
	void				OnMouseScroll( CMouse * pcMouse );

	BOOL				IsLockItems();

	ItemData			* GetEarring1();
	ItemData			* GetEarring2();

	bool				SetEarring1( ItemData * pcItemData );
	bool				SetEarring2( ItemData * pcItemData );

	void				SetEarringsEnabled( bool b );

private:
	UI::Window_ptr		pWindow = nullptr;

	CCostumeWindowHandler * pcCostumeWindowHandler = nullptr;

	void				OnGetItemEarring( int iItemBoxID, UIEventArgs eArgs );
	void				OnPutItemEarring( int iItemBoxID, UIEventArgs eArgs );

	void				OnBeforePutEarring( int iItemBoxID, UIEventArgs eArgs );

};

