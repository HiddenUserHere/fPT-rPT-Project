#pragma once
#include "UIElement.h"

namespace UI
{
struct ActionKeyboard
{
	DWORD									dwKey;
	int										iEventID;

											ActionKeyboard( DWORD _dwKey, int _iEventID ){ dwKey = _dwKey; iEventID = _iEventID; }
};
typedef std::shared_ptr<class InputField> InputField_ptr;
class InputField : public Element
{
public:
	//Constructor
											InputField( Rectangle2D rRectangle );
	virtual								   ~InputField();

	void									Clear();

	//Getters and Setters
	std::string								GetText();
	std::shared_ptr<class Group>			GetGroup() { return pGroupMain; }
	int										GetNumber();
	void									SetTextAlign( EAlign eHorizontal, EAlign eVertical ) { eHorizontalAlign = eHorizontal; eVerticalAlign = eVertical; }
	void									SetImage( class Image * pImage ) { this->pImage = pImage; };
	void									SetParentChild( BOOL bParent ) { bParentChild = bParent; };
	void									SetMaxLength( int iLen ) { iTextLength = iLen; };
    void                                    SetEnableMaskPassword( BOOL b ) { bMaskPassword = b; }
    void									SetMaskPassword( char c ) { szMask[0] = c; };
	void									SetPlaceHolder( const std::string strPlaceHolderText, DWORD dwPlaceHolderColor = -1 ) { strPlaceHolder = strPlaceHolderText; dwColorPlaceHolder = dwPlaceHolderColor; }
    void									SetSelect( BOOL bSel ) { bSelectType = bSel; };
	void									ClearInput();
	void									SetCanScroll( BOOL b ) { bCanScroll = b; }
	BOOL									GetCanScroll() { return bCanScroll; }
	BOOL									IsParentChild() { return bParentChild; };
	void									SetGroup( std::shared_ptr<class Group> pGroup ) { pGroupMain = pGroup; };
	void									SetFont( const char * pszFont, int iHeight, int iWidth, BOOL bItalic, BOOL bBold, DWORD dwColor );
	void									SetText( std::string strText ) { this->strText = strText; };
	void									SetAllowedCharacters( std::string str ) { strAllowed = str; bAllowedRule = TRUE; };
	BOOL									IsSelected() { return bSelect; };
	void									SetSelected(BOOL b) { bSelect = b; if(pGroupMain && b)pGroupMain->SetIndexObject(GetID()); };
	void									SetCanSelectText( BOOL b ) { bCanSelectText = b; }
	void									SetIsNumber( BOOL b, int iMin, int iMax );
	void									SetIsFormattedNumber( BOOL b ) { bIsFormattedNumber = b; }
	void									BindActionKeyboard( ActionKeyboard s ){ vBindActionsKeyboard.push_back(s); }
	void									AddInput( std::string str );

	void									Update( float fTime );

	//Render
	void									Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );

	//Event Handlers
	void									OnMouseMove( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );
	BOOL									OnMouseClick( CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );
	BOOL									OnMouseScroll( CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );
	BOOL									OnKeyChar( class CKeyboard * pcKeyboard );
	BOOL									OnKeyPress( class CKeyboard * pcKeyboard );
private:
	void									UndoAction();

	std::string								strText;
	class Image								* pImage		= NULL;
	DX::Font_ptr							pFont = nullptr;
	int										iFontHeight		= 0;
	int										iTextLength		= 128;
	BOOL									bHover			= FALSE;
	BOOL									bCanScroll		= FALSE;
	BOOL									bDisable		= FALSE;
	BOOL									bSelect			= FALSE;
	BOOL									bSelectType		= FALSE;
	BOOL									bIsNumber		= FALSE;
	BOOL									bIsFormattedNumber = FALSE;
	int										iMinNum			= 0;
	int										iMaxNum			= 100;
	BOOL									bParentChild	= TRUE;
	BOOL									bShow			= TRUE;
	std::shared_ptr<class Group>			pGroupMain		= NULL;
	DWORD									dwColorText		= -1;
	BOOL									bBarTextShow	= FALSE;
	DWORD									dwTimeBarText	= 0;
	int										iBarTextTime	= 166;
	int										uCurrPosition	= 0;

	Rectangle2D								sSelectedTextBox = Rectangle2D();
	int										iSelectedTextPos = 0;
	int										iSelectedTextLength = 0;
	BOOL									bSelectingText = FALSE;

	int										iClippedTextWidth = 0;

    BOOL                                    bMaskPassword = FALSE;
    char                                    szMask[2] = {'*',0};

	std::string								strAllowed;
	BOOL									bAllowedRule	= FALSE;

	EAlign									eHorizontalAlign = ALIGN_Left;
	EAlign									eVerticalAlign   = ALIGN_Top;

	std::vector<std::string>				vInputActions;
	std::vector<ActionKeyboard>				vBindActionsKeyboard;

	DWORD									dwColorPlaceHolder = -1;
	std::string								strPlaceHolder = "";

	BOOL									bCanSelectText = TRUE;

	UINT									BuildFormat() const;

};
}