#include "stdafx.h"
#include "UIElement.h"

Element::Element()
{
	
}

Element::Element( Rectangle2D rRectangle ) : rRect( rRectangle )
{

}

Element::~Element()
{
}

Rectangle2D Element::GetRect()
{
	Rectangle2D rRectScaled = rRect;
	rRectScaled.iX = (int)((float)rRectScaled.iX * fScale);
	rRectScaled.iY = (int)((float)rRectScaled.iY * fScale);
	rRectScaled.iWidth = (int)((float)rRectScaled.iWidth * fScale);
	rRectScaled.iHeight = (int)((float)rRectScaled.iHeight * fScale);

    return rRectScaled;
}

void Element::Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	if( bUIDebug )
	{
		iWidth	= (int)((float)iWidth * GetScale());
		iHeight = (int)((float)iHeight * GetScale());

		GRAPHICENGINE->DrawRectangle2D( Rectangle2D(iX, iY, iWidth, 1), -1 );
		GRAPHICENGINE->DrawRectangle2D( Rectangle2D( iX, iY + iHeight - 1, iWidth, 1 ), -1 );
		GRAPHICENGINE->DrawRectangle2D( Rectangle2D( iX, iY, 1, iHeight ), -1 );
		GRAPHICENGINE->DrawRectangle2D( Rectangle2D( iX + iWidth - 1, iY, 1, iHeight ), -1 );
	}
}

float Element::GetResolutionScale()
{
	float fWidth = ((float)RESOLUTION_WIDTH / 1024.0f);
	float fHeight = ((float)RESOLUTION_HEIGHT / 768.0f);
	
	if ( (fWidth > 1.0f) && (fHeight > 1.0f) )
		return fHeight < fWidth ? fHeight : fWidth;

	return 1.0f;
}
