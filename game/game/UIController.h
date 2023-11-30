#pragma once
#include "UIElement.h"


struct Rectangle2D
{
	int x;
	int y;
	int w;
	int h;
};



class CUIController :
	public CUIElement
{
public:
	CUIController();
	virtual ~CUIController();

	void													SetControllerRect( Rectangle2D rectangle );
	void													SetScroller( std::shared_ptr<class CUIScroller> pScroller );
	void													SetFreePosition();

	EID														GetID() { return eIDHandler; };

	void													Draw( int iX, int iY ) {};
	void													Draw();

	BOOL													OnMouseClick( EButton eButton );
	BOOL													OnMouseMove();
	BOOL													OnMouseWheel( int iDelta );
	BOOL													OnKeyChar( char cChar );

	void													SetState( BOOL bActive ) { bIsShowing = bActive; };
	void													SetDisable( BOOL bActive ) {};
	void													SetYMoveObject( int iY ) {};
	void													SetGlobalClick( BOOL bGlobal ) { IsNotGlobalClick = bGlobal ? FALSE : TRUE; };

	int														GetType() { return UITYPE_Button; };
	std::shared_ptr<class CUIGroup>							GetGroup() { return NULL; };
	BOOL													GetCheckedState( EID eID ) { return CUIElement::GetCheckedState( eID ); };
	BOOL													IsChecked() { return FALSE; };
	CUIElement *											GetElement() { return pElement; };

	Rectangle2D												r;

private:

	void													ReposRect();

	int														iYPosScroller = 0;

	BOOL													bIsShowing = TRUE;
	BOOL													IsFreeMove = FALSE;
	BOOL													IsClicked = FALSE;
	BOOL													IsNotGlobalClick = FALSE;
	D3DXVECTOR2												vMousePos;
	D3DXVECTOR2												vMousePosInitial;

	EID														eIDHandler;
	CUIElement *											pElement;
	std::shared_ptr<class CUIScroller>						pScrollControl = NULL;
};

