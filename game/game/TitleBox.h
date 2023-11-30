#pragma once

#define MAX_MESSAGES			3
#define MAX_DURATION_TITLEBOX	10
#define MAX_DURATION_ANIM		600

#include "X3DEasings.h"

enum ETextID
{
	TEXTID_Undefined,

	TEXTID_Message1,
	TEXTID_Message2,
	TEXTID_Message3,
};

struct Title
{
	BOOL						bMoveToDown;
	BOOL						bCanHide;
	Point2D					    sPosition;
	int							iBasePositionY;
	std::string					strText;
	D3DXCOLOR					d3dcolor;
	short						sDuration;
	float						fTime;
	int							iOpacity;

								Title() = default;
								Title( Point2D sPos, std::string str, D3DXCOLOR color, short sDur ){ iOpacity = 255; bCanHide = FALSE; iBasePositionY = 0; bMoveToDown = TRUE; sPosition.iX = 0; sPosition.iY = sPos.iY; strText = str; d3dcolor = color; sDuration = sDur; fTime = MAX_DURATION_ANIM; }
};

class TitleBox
{
protected:

	DX::Font_ptr				pFont = nullptr;

	int							iFontID = -1;
	UI::Image				  * pImageLeft, * pImageCenter, * pImageRight;

	std::vector<Title>			vTitles;
public:
								TitleBox();
	virtual					   ~TitleBox();

	void						Render();

	void						SetText( char * pszText, int iTime = 2000 );
	void						SetText( int iID );
	void						RenderTextMiddle( const char * pszText );

	void						AddTitle( Title psTitle );

	void						Update( float fTime );
	void						Tick();

	void						Init();
};

