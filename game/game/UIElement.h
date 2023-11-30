#pragma once

#include <vector>
#include "Mouse.h"

typedef int										EID;
typedef int										EImage;
typedef int										EButton;
typedef int										EButton;

typedef std::shared_ptr<class Element>			Element_ptr;

enum EType
{
	TYPE_None,

	TYPE_Button,
	TYPE_Group,
	TYPE_Window,
	TYPE_Scroller,
	TYPE_Checkbox,
	TYPE_Text,
	TYPE_InputField,
	TYPE_ImageBox,
	TYPE_Tooltip,
	TYPE_List
};

class Element
{
public:
	//Constructor
												Element();
												Element( class Rectangle2D rRectangle );
	virtual									   ~Element();

	virtual void								Clear() {};

	//Getters and Setters
	virtual int									GetID() { return iID; };
	virtual void								SetID( int iIDObject ) { iID = iIDObject; };
	virtual int									GetX() { return (int)((float)rRect.iX * fScale); };
	virtual int									GetY() { return (int)((float)rRect.iY * fScale); };

	virtual void								SetWidth( int i ) { rRect.iWidth = i; };
	virtual int									GetWidth() { return (int)((float)rRect.iWidth * fScale); };

	virtual void								SetHeight( int i ) { rRect.iHeight = i; };
	virtual int									GetHeight() { return (int)((float)rRect.iHeight * fScale); };

	virtual void								SetYAxis( int iY ) {};
	virtual void								SetPosition( int iX, int iY ) { rRect.iX = (int)((float)iX / fScale); rRect.iY = (int)((float)iY / fScale); };

	virtual void								SetType( EType e ){ eType = e; }
	virtual int									GetType() { return eType; };

	virtual void								SetBox(Rectangle2D sBox) { rRect = sBox; };
	virtual Rectangle2D							GetRect();

	DX::Color									GetColorGlow() { return cColorGlow; }
	void										SetColorGlow( DX::Color c ) { cColorGlow = c; }

	void										AddRef() { uRef++; }
	UINT										GetRef() { return uRef; }
	void										DecRef() { uRef--; }

	float										GetScale() { return fScale; }
	void										SetScale( float f ) { fScale = f; }
	bool										GetUseSelfScale() { return bUseSelfScale; }
	void										SetUseSelfScale( bool b ) { bUseSelfScale = b; }

	virtual void								Update( float fTime ) {};

	//Render
	virtual void								Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY, int iSourceWidth, int iSourceHeight ) {};
	virtual void								Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );

	//Event Handlers
	virtual BOOL								OnMouseClick( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY ) { return FALSE; };
	virtual void								OnMouseMove( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY ) {};
	virtual BOOL								OnMouseScroll( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY ) { return FALSE; };
	virtual BOOL								OnKeyChar( class CKeyboard * pcKeyboard ) { return FALSE; };
	virtual BOOL								OnKeyPress( class CKeyboard * pcKeyboard ) { return FALSE; };

	void										Show() { bShow = TRUE; };
	void										Hide() { bShow = FALSE; };

	BOOL										IsOpen() { return bShow; };
	virtual BOOL								IsParentChild() { return TRUE; };

	static float								GetResolutionScale();

	std::vector<Element_ptr>					vElementsChild;
protected:
	int											iID = -1;
	BOOL										bShow = TRUE;
	Rectangle2D									rRect;

	EType										eType = TYPE_None;

	DX::Color									cColorGlow;

	UINT										uRef = 0;

	float										fScale = 1.0f;
	bool										bUseSelfScale = false;
};

