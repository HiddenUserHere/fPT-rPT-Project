#pragma once

namespace UI
{ 

struct LetterData
{
	int				iID;
	Rectangle2D		sBox;
	int				iPaddingX, iPaddingY;
};

struct FontData
{
	int iLineHeight;
	int iLettersCount;
	int iBaseY;

	std::vector<LetterData> vLetters;
};

typedef std::shared_ptr<class BitmapFont> Bitmapfont_ptr;
class BitmapFont : public Element
{
public:
	//Constructor
										BitmapFont( Rectangle2D rRect );
	virtual							   ~BitmapFont();

	//Render
	void								Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );

	int									GetTextWidth();

	LetterData							GetLetterData( int iID );
	void								ReadFontData( char * pszFile );
	void								SetFontImage( Image * p ){ pImageFont = p; }
	void								SetText( std::string str ){ iWidthText = -1; strText = str; }
	void								SetColor( DWORD d ){ dwColor = d; }
	void								SetScale( float f ){ fScale = f; }
	void								SetHorizontalAlign( EAlign e ){ eHorizontalAlign = e; }

protected:
	DWORD								dwColor = -1;
	std::string							strBuffer;
	FontData							sFont;

	EAlign								eHorizontalAlign = ALIGN_Left;

	float								fScale = 1.0f;

	std::string							strText;

	Image							  * pImageFont;

	int									iWidthText = -1;
};
}