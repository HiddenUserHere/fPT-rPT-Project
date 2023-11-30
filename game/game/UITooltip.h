#pragma once

#include "UIElement.h"

enum ETooltipPosition
{
	TOOLTIPPOSITION_None,

	TOOLTIPPOSITION_Top,
	TOOLTIPPOSITION_Right,
	TOOLTIPPOSITION_Left,
	TOOLTIPPOSITION_Bottom,
};

namespace UI
{
typedef std::shared_ptr<class Tooltip> Tooltip_ptr;
class Tooltip : public Element
{
public:
	//Constructor
							Tooltip();
							Tooltip( Rectangle2D sBox );
	virtual				   ~Tooltip();

	void					Init();

	//Getters and Setters
	ETooltipPosition		GetPosition(){ return ePosition; }
	void					SetPosition( ETooltipPosition e ){ ePosition = e; }

	BOOL					GetFollowMouse(){ return bFollowMouse; }
	void					SetFollowMouse( BOOL b ){ bFollowMouse = b; }

	float					GetFadeDuration(){ return fFadeDuration; }
	void					SetFadeDuration( float f ){ fFadeDuration = f; }

	void					SetLinkObject( Element_ptr pObject );
	void					SetFont( char * pszFontName, int iFontSize, DWORD dwColor );
	void					SetTextColor( Color c ){ if( pText )pText->SetColor(c.Get()); }
	void					SetText( std::string strText );
	
	BOOL					IsHover(){ return bHover; }
	void					SetHover( BOOL b ){ bHover = b; }

	void					SetAddAxis( int iX, int iY ) { iAdditionalX = iX; iAdditionalY = iY; }

	void					Open();
	void					Close();

	//Render
	void					Update( float fTime );
	void					Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );

	//Event Handlers
	BOOL					OnMouseClick( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );
	void					OnMouseMove( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );
private:
	ETooltipPosition		ePosition = TOOLTIPPOSITION_None;
	BOOL					bFollowMouse = FALSE;

	float					fFadeDuration = 0.0f;
	float					fProgress = 0.0f;

	Element_ptr				pLinkObject;
	Rectangle2D				sObjectBox;

	Text_ptr				pText;

	BOOL					bHover;
	BOOL					bTransition;

	int						iWidthText = -1;
	int						iHeightText = -1;

	int						iFadeType = 0;

	Image				  * pBorderImages[6];

	int						iAdditionalX = 0;
	int						iAdditionalY = 0;
};
}