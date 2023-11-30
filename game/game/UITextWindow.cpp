#include "stdafx.h"
#include "UITextWindow.h"

char szBufferTextWindow[65535];

namespace UI
{
TextWindow::TextWindow( Rectangle2D rRectangle ) : Element( rRectangle )
{
}

TextWindow::~TextWindow()
{
	for ( std::vector<ParamsWindow*>::iterator it = vParams.begin(); it != vParams.end(); it++ )
	{
		ParamsWindow * ps = (*it);

		DELET( ps );
	}

	vParams.clear();
}

void TextWindow::Clear()
{
	for ( std::vector<ParamsWindow*>::iterator it = vParams.begin(); it != vParams.end(); it++ )
	{
		ParamsWindow * ps = (*it);

		DELET( ps );
	}

	vParams.clear();
}

void TextWindow::SetFont( const char * pszFont, int iHeight, int iWidth, BOOL bItalic, BOOL bBold, DWORD dwColor )
{
	pFont = GetGraphicsDevice()->GetFontFactory()->MakeFont( nullptr, pszFont, iHeight, iWidth, bBold, bItalic );
	dwColorText		= dwColor;
}

void TextWindow::SetBox( Rectangle2D sBox )
{
	rRect = sBox;
}

int TextWindow::GetHeightText()
{
	int iRenderX = GetX();
	int iRenderY = GetY();

	int iWidthT = rRect.iWidth;
	int iHeightT = RESOLUTION_HEIGHT;

	pFont->Calc( strText, DT_WORDBREAK, iWidthT, iHeightT );

	return iHeightT;
}

void TextWindow::Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	int iRenderX		= GetX() + iX;
	int iRenderY		= GetY() + iY;
	std::string strLocal = strText;

	// Parameters
	for ( const auto &v : vParams )
	{
		std::size_t pos = strLocal.find( v->strParam );
		if ( pos != std::string::npos )
			strLocal.replace( strLocal.begin() + pos, strLocal.begin() + pos + v->strParam.length(), v->strValue.begin(), v->strValue.end() );
	}

	RECT rect{ iRenderX, iRenderY, iRenderX + rRect.iWidth, rRect.iHeight + iRenderY };
	pFont->Draw( strLocal, rect, DT_WORDBREAK, DX::Color( dwColorText ) );
}

void TextWindow::SetParam( std::string strParam, std::string strValue )
{
	for ( const auto &v : vParams )
	{
		if ( v->strParam.compare(strParam) == 0 )
		{
			v->strValue = strValue;
			return;
		}
	}
	ParamsWindow * pParam = new ParamsWindow;
	pParam->strParam = strParam;
	pParam->strValue = strValue;
	vParams.push_back( pParam );
}

void TextWindow::LoadTextFromFile( const char * pszFile )
{
	FILE * pFile = NULL;

	ZeroMemory( szBufferTextWindow, _countof( szBufferTextWindow ) );

	fopen_s( &pFile, pszFile, "rb" );
	if ( pFile )
	{
		fseek( pFile, 0, SEEK_END );
		int iSizeFile = ftell( pFile );
		fseek( pFile, 0, SEEK_SET );

		if ( iSizeFile > _countof( szBufferTextWindow ) )
			iSizeFile = _countof( szBufferTextWindow );
		
		fread( szBufferTextWindow, iSizeFile, 1, pFile );
		fclose( pFile );
	}
	strText = szBufferTextWindow;
}
}