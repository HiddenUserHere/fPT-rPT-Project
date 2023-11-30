#pragma once
#include "UIElement.h"
#include "UIController.h"

class CUIScroller : public CUIElement
{
public:
	CUIScroller( Rectangle2D rectangle, BOOL bVertical = TRUE );
	virtual ~CUIScroller();
	
	EID												GetID() { return eIDHandler; };

	void											SetMinMax( float fMin, float fMax );
	void											SetRect( Rectangle2D rectangle );
	void											SetState( BOOL bActive ) {};
	void											SetDisable( BOOL bActive ) {};
	void											SetYMoveObject( int iY ) {};
	void											SetImage( EImage eImageScrollBar, EImage eImageScroller, int iWidth, int iHeight );
	void											SetImageBackground( EImage eImageScrollBarBack );

	void											Draw();

	BOOL											OnMouseClick( EButton eButton );
	BOOL											OnMouseMove();
	BOOL											OnMouseWheel( int iDelta );

	int												GetType() { return UITYPE_Scroller; };
	std::shared_ptr<class CUIGroup>					GetGroup() { return NULL; };
	BOOL											IsChecked() { return FALSE; };
	int												GetScrollPercent();

protected:
	CUIElement										* pElementClass;

	Rectangle2D										r;
	int												iWidthScroller	= 0;
	int												iHeightScroller = 0;


	Rectangle2D										rectScroll;

	BOOL											IsClicked	= FALSE;
	BOOL											IsVertical	= TRUE;
	BOOL											bBackground = FALSE;

	EID												eIDHandler;
	EImage											eImageScrollBarHandler		= -1;
	EImage											eImageScrollerHandler		= -1;
	EImage											eImageScrollBarBackHandler	= -1;

	float											fMinScroll = 0.0f;
	float											fMaxScroll = 100.0f;

private:
	void											OnPosition();

};

