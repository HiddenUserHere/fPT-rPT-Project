#pragma once
class CActionFieldChestWindow
{
public:
	CActionFieldChestWindow();
	virtual ~CActionFieldChestWindow();

	void						Init();

	void						Open();
	void						Close();
	void						ClearSlots();

	void						EnableSlots();

	void						Render();

	void						OnResolutionChanged();

	void						OnMouseMove( class CMouse * pcMouse );
	BOOL						OnMouseClick( class CMouse * pcMouse );

private:
	enum
	{
		WINDOWID_Main,

		ITEMBOX_Chest,
		ITEMBOX_Key,
	};

	UI::Window_ptr				pWindow;

	DWORD						dwTimeSend = 0;

	void						BuildWindow();

	void						OnItemNotAllowedChest( UIEventArgs eArgs );
	void						OnItemNotAllowedKey( UIEventArgs eArgs );
	void						OnPutItemChest( UIEventArgs eArgs );
	void						OnPutItemKey( UIEventArgs eArgs );
	void						OnGetItemChest( UIEventArgs eArgs );
	void						OnGetItemKey( UIEventArgs eArgs );

	void						OnButtonCloseClick( UIEventArgs eArgs );
	void						OnButtonExchangeClick( UIEventArgs eArgs );
};

