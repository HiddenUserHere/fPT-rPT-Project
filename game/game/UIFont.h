#pragma once
class CUIFont
{
public:
	CUIFont();
	virtual ~CUIFont();

	void										SetFontSet( const char * pszFaceName, BOOL bItalic, BOOL bBold, int iHeight, int iWidth, UINT uColor );
	void										SetText( const char * pszText );
	void										DeleteChar();
	void										AddChar( char cChar );
	int											GetWidth();
	void										Draw( int iX, int iY );
	void										DrawBarText( int iX, int iY, bool bActive );
	void										Draw( RECT rBox );
	void										SetMaxChar( int iMax ) { iMaxChar = iMax; };
protected:
	int											iHandlerFont = 0;
	DX::Font_ptr								pFont = NULL;
	std::string									strText;
	UINT										uColorText = 0;
	int											iMaxChar = 32;
};

