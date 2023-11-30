#pragma once


struct LoginMemoryData
{
	char										szUnkData[ 0x84 ];
	int									eState;
};

#define LOGINDATA								( LoginMemoryData* )(0x039016D4)

class CLoginModel
{
protected:
	LoginMemoryData								* psLoginData = NULL;
	const int									iWidthServerSelectBox = 230;
	const int									iHeightServerSelectBox = 161;

	UI::Image									* pBoxImageHandler;
	UI::Image									* pImageBGLogin = NULL;

	BOOL										bLoaded = FALSE;
public:
	CLoginModel();
	virtual ~CLoginModel();

	void										RenderServerSelectBox();
	void										OnRender();
	void										Init();

	void										RenderLoginBG();
};

