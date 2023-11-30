#pragma once

namespace UI
{
typedef std::shared_ptr<class Scroll> Scroll_ptr;
class Scroll : public Element
{
public:
	//Constructor
											Scroll( Rectangle2D rRectangle );
	virtual								   ~Scroll();
	
	void									Clear();

	//Getters and Setters
	void									SetMinMax( int iMin, int iMax ) { fMinScroll = (float)iMin; fMaxScroll = (float)iMax; };
	void									SetPercent( int iCur );
	int										GetPercent();
	void									SetParentChild( BOOL bParent ) { bParentChild = bParent; };
	void									SetVertical( BOOL bType ) { bVertical = bType; };
	BOOL									IsParentChild() { return bParentChild; };
	void									SetEvent( int iEvent ) { iEventID = iEvent; };
	void									SetImage( class Image * pImage ) { this->pImage = pImage; };
	void									SetScroller( class Image * pImage, int iWidth, int iHeight ) { pImageScroller = pImage; iWidthScroller = iWidth; iHeightScroller = iHeight;};
	void									SetImageBackground( class Image * pImage ) { pImageBackground = pImage; };
	void									SetGroup( std::shared_ptr<class Group> pGroup ) { pGroupMain = pGroup; };
	void									SetY(int iY) { rScroll->iY = iY; };

	//Render
	void									Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );

	//Event Handlers
	BOOL									OnMouseClick( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );
	BOOL									OnMouseScroll( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );
	void									OnMouseMove( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );
private:
	Rectangle2D								*rScroll;
	class Image								* pImage			= NULL;
	class Image								* pImageScroller	= NULL;
	class Image								* pImageBackground	= NULL;
	int										iEventID			= -1;
	int										iWidthScroller		= 0;
	int										iHeightScroller		= 0;
	float									fMinScroll			= 0.0f;
	float									fMaxScroll			= 100.0f;
	int										iCurrentPercent		= 0;
	BOOL									bParentChild		= TRUE;
	BOOL									bVertical			= TRUE;
	BOOL									bScrollable			= FALSE; 
	enum EMouseEvent						eMouseEvent			= EMouseEvent::ClickDownL;
	std::shared_ptr<class Group>			pGroupMain			= NULL;
	BOOL									bForceDown			= FALSE;
};
}
