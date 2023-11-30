#include "stdafx.h"
#include "CCharacterStatusWindowHandler.h"

CCharacterStatusWindowHandler::CCharacterStatusWindowHandler()
{
}

CCharacterStatusWindowHandler::~CCharacterStatusWindowHandler()
{
}

void CCharacterStatusWindowHandler::Init()
{
	pFont = GetGraphicsDevice()->GetDefaultFont();
}

void CCharacterStatusWindowHandler::RenderText()
{
	int DEFAULT_POSITIONX = 0;
	int DEFAULT_POSITIONY = 0;

	Rectangle2D rRect = Rectangle2D( 0, 0, 105, 18 );
	DrawTextCharacter( GetCharacterClassData( UNITDATA->sCharacterData.iClass, UNITDATA->sCharacterData.iRank )->pszNameTranslated, Rectangle2D( 70, 43, 105, 18 ), DX::Color( 255, 255, 255, 255 ) );
	DrawTextCharacter( UNITDATA->sCharacterData.szName, Rectangle2D( 70, 63, 105, 18 ), DX::Color( 255, 255, 255, 255 ) );
	DrawTextCharacter( UNITDATA->sCharacterData.szName, Rectangle2D( 70, 85, 105, 18 ), DX::Color( 255, 255, 255, 255 ) );
	DrawTextCharacter( std::to_string( UNITDATA->sCharacterData.iLevel ), Rectangle2D( 70, 105, 105, 18 ), DX::Color( 255, 255, 255, 255 ) );
}

void CCharacterStatusWindowHandler::DrawTextCharacter( const std::string & strText, Rectangle2D & rRect, const DX::Color & cColor )
{
	RECT r;
	r.left = rRect.iX;
	r.top = rRect.iY;
	r.right = r.left + rRect.iWidth;
	r.bottom = r.top + rRect.iHeight;

	pFont->Draw( strText, r, DT_CENTER, cColor );
}
