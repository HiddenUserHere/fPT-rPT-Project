#pragma once

class CCompareItemHandler
{
public:
	//Constructor
										CCompareItemHandler();
	virtual								~CCompareItemHandler();

	void								Init();

	//Render
	void								Render();

	//Events Handler
	BOOL								OnKeyPress( class CKeyboard * pcKeyBoard );
private:
	class ItemData						* pcTempItemData = NULL;
	BOOL								bAlreadyChecked = FALSE;
	BOOL								bVisible;

	UI::ItemInfoBox_ptr					pItemInfoBox;
};

