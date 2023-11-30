#pragma once
#include "UIElement.h"

struct ParamsWindow
{
	std::string								strParam;
	std::string								strValue;
};

namespace UI
{
typedef std::shared_ptr<class TextWindow> TextWindow_ptr;
class TextWindow : public Element
{
public:
	//Constructor
											TextWindow( Rectangle2D rRectangle );
	virtual								   ~TextWindow();

	void									Clear();
	
	//Getters and Setters
	std::string								GetText() { return strText; };

	void									SetFont( const char * pszFont, int iHeight, int iWidth, BOOL bItalic, BOOL bBold, DWORD dwColor );
	void									SetText( std::string str ) { strText = str; };
	void									SetParam( std::string strParam, std::string strValue );
	void									SetParentChild( BOOL bParent ) { bParentChild = bParent; };
	BOOL									IsParentChild() { return bParentChild; };
	void									SetBox( Rectangle2D sBox );

	int										GetHeightText();

	//Render
	void									Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );

	void									LoadTextFromFile( const char * pszFile );
private:
	std::string								strText;
	DX::Font_ptr							pFont = nullptr;
	BOOL									bHover			= FALSE;
	BOOL									bDisable		= FALSE;
	BOOL									bSelect			= FALSE;
	BOOL									bSelectType		= FALSE;
	BOOL									bParentChild	= TRUE;
	BOOL									bShow			= TRUE;
	DWORD									dwColorText		= -1;
	std::shared_ptr<class Group>			pGroupMain		= NULL;
	std::vector<ParamsWindow*>				vParams;
};
}
