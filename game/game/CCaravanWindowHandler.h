#pragma once
class CCaravanWindowHandler
{
public:
	CCaravanWindowHandler();
	virtual ~CCaravanWindowHandler();

	void						Init();

	void						Open();

	BOOL						IsOpen() { return pWindow->IsOpen(); }

	void						Close();

	void						ClearItems();

	void						AddItem( ItemData * pcItemData );

	void						UpdateCaravanData( const std::string strName, bool bFollow );

	void						OnMouseMove( CMouse * pcMouse );
	BOOL						OnMouseClick( CMouse * pcMouse );

	BOOL						OnKeyPress( CKeyboard * pcKeyboard );
	BOOL						OnKeyChar( CKeyboard * pcKeyboard );

	void						Render();
	void						Update( float fTime );
	void						OnResolutionChanged();

private:
	enum
	{
		ITEMBOX_Items,

		CHECKBOX_Follow,

		INPUT_Name,
	};

	UI::Window_ptr				pWindow = nullptr;

	void						BuildWindow();

	void						OnButtonCloseClick( UIEventArgs eArgs );
};

