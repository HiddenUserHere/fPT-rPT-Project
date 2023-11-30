#include "stdafx.h"
#include "UIChargeBar.h"

namespace UI
{

ChargeBar::ChargeBar( Rectangle2D rRectangle ) : Element( rRectangle )
{
}


ChargeBar::~ChargeBar()
{
}

void ChargeBar::Clear()
{
	CLEARPTR( pcaImageBar[0] );
	CLEARPTR( pcaImageBar[1] );
	UI::ImageLoader::DeleteImage( pcaImageBar[0] );
	UI::ImageLoader::DeleteImage( pcaImageBar[1] );
}


void ChargeBar::Render()
{
	if ( !IsOpen() )
		return;

	if ( pcaImageBar[0] && pcaImageBar[1] )
	{
		UI::ImageRender::Render( pcaImageBar[0], rRect.iX, rRect.iY, pcaImageBar[0]->GetWidth(), pcaImageBar[0]->GetHeight(), 0, 0, -1 );

		int iWidth = (pcaImageBar[1]->GetWidth() * iCurrentPercent) / (sMinMax.iMin + sMinMax.iMax);

		int iX = rRect.iX + ((pcaImageBar[0]->GetWidth() >> 2) - (pcaImageBar[1]->GetWidth() >> 2));
		int iY = rRect.iY + ((pcaImageBar[0]->GetHeight() >> 2) - (pcaImageBar[1]->GetHeight() >> 2));

		UI::ImageRender::Render( pcaImageBar[1], iX, iY, iWidth, pcaImageBar[1]->GetHeight(), 0, 0, -1 );
	}
}

void ChargeBar::Update( float fTime )
{
	if ( !IsOpen() )
		return;

	if ( !bIsCharging )
		return;

	if ( fCurrentTime < fDuration )
	{
		fCurrentTime += fTime;

		if ( fCurrentTime >= fDuration )
		{
			fCurrentTime = fDuration;

			if ( bHideOnComplete )
			{
				Hide();
				Reset();
			}

			if ( iEventID != -1 )
				Event::Invoke( iEventID );
		}

		iCurrentPercent = (int)((fCurrentTime * (float)(sMinMax.iMin + sMinMax.iMax)) / fDuration);
	}
}

void ChargeBar::Reset()
{
	iCurrentPercent = sMinMax.iMin;

	fCurrentTime	= 0.0f;
}

};