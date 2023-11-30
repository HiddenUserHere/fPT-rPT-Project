#include "stdafx.h"
#include "UIBitmapFont.h"

namespace UI
{
BitmapFont::BitmapFont( Rectangle2D rRect ) : Element( rRect )
{
}

BitmapFont::~BitmapFont()
{
}

void BitmapFont::Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	int iRenderX = GetX() + iX;
	int iRenderY = GetY() + iY;

	if( eHorizontalAlign == ALIGN_Center )
		iRenderX += (rRect.iWidth >> 1) - (GetTextWidth() >> 1);

	if( pImageFont )
	{
		int iPaddingX = 0;
		for( UINT i = 0; i < strText.size(); i++ )
		{
			int iLetterID = strText[i];
			LetterData * p = &GetLetterData( iLetterID );
			int iExtraY = p->iPaddingY - sFont.iBaseY;

			UI::ImageRender::Render( pImageFont, iRenderX + iPaddingX, iRenderY + iExtraY, p->sBox.iWidth, p->sBox.iHeight, p->sBox.iX, p->sBox.iY, dwColor, fScale );
		
			if( p->sBox.iWidth )
			{
				int iDifference = abs(p->iPaddingX - p->sBox.iWidth);
				iPaddingX += (int)((float)(p->sBox.iWidth - iDifference) * fScale);
			}
			else
				iPaddingX += p->iPaddingX;
		}
	}

	Element::Render( GetX() + iX, GetY() + iY, GetWidth(), GetHeight(), iSourceX, iSourceY );
}

int BitmapFont::GetTextWidth()
{
	if( iWidthText == -1 )
	{
		if( strText.size() > 0 )
		{
			iWidthText = 0;
			for( UINT i = 0; i < strText.size(); i++ )
			{
				int iLetterID = strText[i];
				LetterData * p = &GetLetterData( iLetterID );

				if( p->sBox.iWidth )
				{
					int iDifference = abs( p->iPaddingX - p->sBox.iWidth );
					iWidthText += (int)((float)(p->sBox.iWidth - iDifference) * fScale );
				}
				else
					iWidthText += p->iPaddingX;
			}
		}
	}

	return iWidthText;
}

LetterData BitmapFont::GetLetterData( int iID )
{
	for( auto &v : sFont.vLetters )
	{
		if( v.iID == iID )
			return v;
	}

	return sFont.vLetters[0];
}

void BitmapFont::ReadFontData( char * pszFile )
{
	INI::CReader cReader( pszFile );

	sFont.iLineHeight = cReader.ReadInt( "Font", "LineHeight" );
	sFont.iLettersCount = cReader.ReadInt( "Font", "Count" );
	sFont.iBaseY = cReader.ReadInt( "Font", "BaseY" );

	for( int i = 0; i < sFont.iLettersCount; i++)
	{
		LetterData sLetterWrapper;
		sLetterWrapper.iID = cReader.ReadInt( FormatString("Letter%d", i), "ID" );
		sLetterWrapper.sBox.iX = cReader.ReadInt( FormatString("Letter%d", i), "X" );
		sLetterWrapper.sBox.iY = cReader.ReadInt( FormatString("Letter%d", i), "Y" );
		sLetterWrapper.sBox.iWidth = cReader.ReadInt( FormatString("Letter%d", i), "WidthSource" );
		sLetterWrapper.sBox.iHeight = cReader.ReadInt( FormatString("Letter%d", i), "HeightSource" );
		sLetterWrapper.iPaddingY = cReader.ReadInt( FormatString("Letter%d", i), "PaddingY" );
		sLetterWrapper.iPaddingX = cReader.ReadInt( FormatString("Letter%d", i), "PaddingX" );

		sFont.vLetters.push_back( sLetterWrapper );
	}
}
}