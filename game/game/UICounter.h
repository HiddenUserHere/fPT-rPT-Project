#pragma once

namespace UI
{
typedef std::shared_ptr<class Counter> Counter_ptr;
class Counter : public Element
{
public:
	//Constructor
											Counter( Rectangle2D rRectangle );
	virtual								   ~Counter();
private:
	InputField_ptr							pInputField		= NULL;
	int										iEventID		= -1;
	int										iMin			= 0;
	int										iMax			= 100;
	int										iCurrent		= 0;
	EMouseEvent								eMouseEvent		= EMouseEvent::ClickDownL;
	class Image								* pImage		= NULL;
	class Image								* pImageBtn1	= NULL;
	class Image								* pImageBtn2	= NULL;
	Rectangle2D								rButton1;
	Rectangle2D								rButton2;
	BOOL									bHover			= FALSE;
	BOOL									bHoverBtn1		= FALSE;
	BOOL									bHoverBtn2		= FALSE;
	BOOL									bDisable		= FALSE;
	BOOL									bSelect			= FALSE;
	BOOL									bSelectType		= FALSE;
	BOOL									bParentChild	= TRUE;
	std::shared_ptr<class Group>			pGroupMain		= NULL;

	
public:

	void									Clear();

	//Getters and Setters
	InputField_ptr							GetInput() { return pInputField; };
	void									SetGroup( std::shared_ptr<class Group> pGroup ) { pGroupMain = pGroup; };
	void									SetMinMax( int iMin, int iMax ) { this->iMin = iMin; this->iMax = iMax; iCurrent = iMin; pInputField->SetIsNumber( TRUE, iMin, iMax ); }
	int										GetCurrent() { return iCurrent; }
	void									SetCurrent( int i );
	void									SetImage( class Image * pImage ) { this->pImage = pImage; };
	void									SetButtonIncrease( Rectangle2D rRectangle, class Image * pImage ) { rButton1 = rRectangle; this->pImageBtn1 = pImage; };
	void									SetButtonDecrease( Rectangle2D rRectangle, class Image * pImage ) { rButton2 = rRectangle; this->pImageBtn2 = pImage; };
	void									SetEvent( int iEvent ) { iEventID = iEvent; };

	//Render
	void									Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );

	//Event Handlers
	BOOL									OnMouseClick( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );
	void									OnMouseMove( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );
	BOOL									OnMouseScroll( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );

	BOOL									OnKeyChar( class CKeyboard * pcKeyboard ) { return pInputField->OnKeyChar( pcKeyboard ); }
	BOOL									OnKeyPress( class CKeyboard * pcKeyboard ) { return pInputField->OnKeyPress( pcKeyboard ); }
};
}

