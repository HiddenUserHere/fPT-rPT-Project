#pragma once
#include "UIElement.h"

namespace UI
{
typedef std::shared_ptr<class ChargeBar> ChargeBar_ptr;
class ChargeBar : public Element
{
public:
	ChargeBar( Rectangle2D rRectangle );
	virtual ~ChargeBar();

	void				Clear();

	void				Render();

	void				Update( float fTime );

	void				SetPosition( int iX, int iY ) { rRect.iX = iX; rRect.iY = iY; }

	void				SetMinMax( IMinMax s ) { sMinMax = s; iCurrentPercent = s.iMin; }
	void				SetDuration( float f ) { fDuration = f; fCurrentTime = 0.0f; }
	void				SetImage( std::pair<Image *, Image *> pImage ) { pcaImageBar[0] = pImage.first; pcaImageBar[1] = pImage.second; }

	void				SetEvent( int i ) { iEventID = i; }

	void				SetHideOnComplete( BOOL b ) { bHideOnComplete = b; }

	void				Reset();

	void				Charge( BOOL b ) { bIsCharging = b; }

	BOOL				IsCharging() { return bIsCharging; }

	int					GetWidthImage() { return pcaImageBar[0] ? pcaImageBar[0]->GetWidth() : 0; }
	int					GetHeightImage() { return pcaImageBar[0] ? pcaImageBar[0]->GetHeight() : 0; }

private:
	UI::Image			* pcaImageBar[2];

	IMinMax				sMinMax;
	int					iCurrentPercent = 0;

	float				fDuration		= 0.0f;
	float				fCurrentTime	= 0.0f;

	BOOL				bHideOnComplete = FALSE;

	int					iEventID		= -1;

	BOOL				bIsCharging		= FALSE;
};

};