#pragma once
#include "UIElement.h"

namespace UI
{
typedef std::shared_ptr<class Window> Window_ptr;
class Window : public Element, std::enable_shared_from_this<Window>
{
public:
	//Constructor
											Window( Rectangle2D rRectangle );
	virtual								   ~Window();

	void									Clear();

	//Getters and Setters
	int										GetType() { return TYPE_Window; };
	void									SetImage( class Image * pImage ) { this->pImage = pImage; };
	void									SetMoveBox( Rectangle2D s ){ rRectSelectMove = s; };
	void									SetCanMove( BOOL b ){ bCanMove = b; }
	void									SetScroll( std::shared_ptr<class Scroll> pScroll ) { pScrollMain = pScroll; };
	void									SetGroup( std::shared_ptr<class Group> pGroup ) { pGroupMain = pGroup; };
	void									SetEvent( int iEvent ) { iEventID = iEvent; };
	void									SetEvent( int iEvent, EMouseEvent e ) { iEventID = iEvent; eMouseEvent = e; };
	void									SetHoverEvent( int iEvent ) { iHoverEventID = iEvent; }
	void									SetBox( Rectangle2D sBox );
	BOOL									IsHover();
	void									SetColorBackground( Color c ){ cColorBackground = c; }

	void									SetWindowScale( float fWindowScale );

    BOOL                                    IsMouseClickEnabled() { return bMouseClickEnabled; }
	void									SetMouseClickEnable( BOOL b ) { bMouseClickEnabled = b; }

	void									SetFocusWindow( BOOL b ){ bFocusWindow = b; }
	BOOL									GetFocusWindow(){ return bFocusWindow; }

	void									Update( float fTime );

	//Render Window
	void									Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY, int iSourceWidth = -1, int iSourceHeight = -1 );

	//Event Handlers
	BOOL									OnMouseClick( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );
	BOOL									OnMouseScroll( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );
	void									OnMouseMove( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );
	BOOL									OnKeyChar( class CKeyboard * pcKeyboard );
	BOOL									OnKeyPress( class CKeyboard * pcKeyboard );

	void									AddElement( Element_ptr pElement );
	BOOL									DelElement( Element_ptr pElement );
	UINT									GetElementCount() { return vElementsChild.size(); }
	auto 									GetElements() { return vElementsChild; }

	template <class F>  
	inline std::shared_ptr<F>				GetElement( int iIDObject )
	{
		// Find object and return pointer to this
		for ( UINT u = 0; u < vElementsChild.size(); u++ )
		{
			if ( vElementsChild[u]->GetID() == iIDObject )
			{
				return std::dynamic_pointer_cast<F>(vElementsChild[u]);
			}
		}
		return NULL;
	};
private:
	class Rectangle2D						rRectViewPort;
	class Rectangle2D						rRectSelectMove;

	class Image								* pImage		= NULL;
	int										iID				= -1;
	int										iEventID		= -1;
	int										iHoverEventID	= -1;
	EMouseEvent								eMouseEvent		= ClickDownL;
	std::shared_ptr<class Group>			pGroupMain		= NULL;
	std::shared_ptr<class Scroll>			pScrollMain		= NULL;
	BOOL									bSelected		= FALSE;
	BOOL									bCanMove		= FALSE;
	BOOL									bIsHover		= FALSE;
	BOOL									bFocusWindow	= TRUE;

	BOOL									bMouseClickEnabled = TRUE;

	Color									cColorBackground = 0;

	Point2D									sPointMouse;
};
}