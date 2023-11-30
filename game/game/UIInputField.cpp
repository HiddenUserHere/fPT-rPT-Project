#include "stdafx.h"
#include "UIInputField.h"

namespace UI
{
InputField::InputField( Rectangle2D rRectangle ) : Element(rRectangle)
{
}

InputField::~InputField()
{
}

void InputField::Clear()
{
	CLEARPTR( pImage );
	UI::ImageLoader::DeleteImage( pImage );

	vInputActions.clear();
	vBindActionsKeyboard.clear();
}

std::string InputField::GetText()
{
	if ( bIsNumber )
	{
		if ( (strText.length() == 0 || atoi( strText.c_str() ) == 0) )
			return "0";

		if ( bIsFormattedNumber )
		{
			std::string strNumber = strText;
			strNumber.erase( std::remove( strNumber.begin(), strNumber.end(), ',' ), strNumber.end() );

			return strNumber;
		}
	}

	return strText;
}

int InputField::GetNumber()
{
	if ( bIsNumber )
	{
		int i = atoi( strText.c_str() );

		if ( i < iMinNum )
			i = iMinNum;
		else if ( i > iMaxNum )
			i = iMaxNum;

		return i;
	}

	return 0;
}

void InputField::SetIsNumber( BOOL b, int iMin, int iMax ) 
{ 
	ClearInput();

	bIsNumber = b; 
	iMinNum = iMin; 
	iMaxNum = iMax; 
	SetText( FormatString( "%d", iMin ) ); 
};

void InputField::Update( float fTime )
{
}

UINT InputField::BuildFormat() const
{
	UINT uFormat = DT_EXPANDTABS;

	switch( eHorizontalAlign )
	{
	default:
	case ALIGN_Left:
		uFormat |= DT_LEFT;
		break;
	case ALIGN_Center:
		uFormat |= DT_CENTER;
		break;
	case ALIGN_Right:
		uFormat |= DT_RIGHT;
		break;
	}

	switch( eVerticalAlign )
	{
	default:
	case ALIGN_Top:
		uFormat |= DT_TOP;
		break;
	case ALIGN_Middle:
		uFormat |= DT_VCENTER;
		break;
	case ALIGN_Bottom:
		uFormat |= DT_BOTTOM;
		break;
	}

	return uFormat;
}

void InputField::Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	if ( IsOpen() )
	{
		int iRenderX		= GetX() + iX;
		int iRenderY		= GetY() + iY;
		int iRenderWidth	= bParentChild ? low( GetWidth(), iWidth - GetX() ) : GetWidth();
		int iRenderHeight	= bParentChild ? low( GetHeight(), iHeight - GetY() ) : GetHeight();

		UI::ImageRender::Render( pImage, iRenderX + iSourceX, iRenderY + iSourceY, iRenderWidth, iRenderHeight, iSourceX, iSourceY, -1 );

        std::string strTextRender = "";
        if ( bMaskPassword )
        {
            for ( size_t i = 0; i < strText.length(); i++ )
                strTextRender.append( szMask );
        }
        else
            strTextRender = strText;

		if ( bCanSelectText == FALSE )
		{
			sSelectedTextBox = {0, 0, 0, 0};
			iSelectedTextPos = 0;
			iSelectedTextLength = 0;
			bSelectingText = FALSE;

			uCurrPosition = strTextRender.length() > 0 ? strTextRender.length() : 0;
		}

		std::string strSelectedText = std::string( strTextRender.begin(), strTextRender.begin() + uCurrPosition );

		int iWidthSelectedText = pFont->Calc( strSelectedText, BuildFormat() );
		int iWidthText = iRenderWidth, iHeightText = iRenderHeight; 
		pFont->Calc( strTextRender, BuildFormat() | DT_NOCLIP, iWidthText, iHeightText );

		bool bClipText = GetWidth() - iWidthSelectedText < 0 ? true : false;

		RECT rRectT;
		rRectT.left		= bClipText ? iRenderX - 10 - (iWidthSelectedText - GetWidth()) : iRenderX;
		rRectT.top		= iRenderY;
		rRectT.right	= rRectT.left + GetWidth();
		rRectT.bottom	= rRectT.top + GetHeight();

		if ( eHorizontalAlign == ALIGN_Left )
			rRectT.left += 2;

		//Compute Scissor
		{
			RECT r = RECT{ iRenderX, iRenderY, iRenderX + GetWidth(), iRenderY + GetHeight() };
			GetGraphicsDevice()->GetScissorRectInside( r );
			GetGraphicsDevice()->PushScissorRect( r );
		}

		pFont->Draw( strTextRender, rRectT, BuildFormat() | DT_NOCLIP, DX::Color( dwColorText ) );
		
		if ( (strTextRender.length() == 0) && (strPlaceHolder.length() > 0) )
			pFont->Draw( strPlaceHolder, rRectT, BuildFormat() | DT_NOCLIP, DX::Color( dwColorPlaceHolder ) );

		if ( bHover || bSelect )
		{
			if ( pGroupMain )
			{
				if ( pGroupMain->GetIndexObject() != GetID() )
					bSelect = FALSE;

				if ( bSelect )
				{
					auto GetWidthSpaceText = [this]( std::string strTextWidth, bool bCountMiddleSpaces ) -> int
					{
						int iLength = strTextWidth.length();
						int iWidthSpace = pFont->Calc( "i i", BuildFormat() | DT_NOCLIP ) - (pFont->Calc( "i", BuildFormat() | DT_NOCLIP ) * 2);

						int iRet = 0;

						if ( bCountMiddleSpaces )
						{
							int iOccurrences = 0;
							for ( size_t i = 0; i < strTextWidth.length(); i++ )
							{
								if ( isspace( strTextWidth.c_str()[i] ) )
									iOccurrences++;
							}

							if ( iOccurrences > 0 )
								iWidthSpace *= iOccurrences;

							iRet = iWidthSpace;
						}
						else
							iRet = iWidthSpace * (iLength - rtrim( strTextWidth ).length());

						return iRet;
					};

					if ( dwTimeBarText < TICKCOUNT )
					{
						dwTimeBarText = TICKCOUNT + iBarTextTime;
						bBarTextShow = bBarTextShow ? FALSE : TRUE;
					}
					else
					{
						if ( bBarTextShow )
						{
							rRectT.left += iWidthSelectedText;
							if ( strSelectedText.length() > 0 )
								rRectT.left += GetWidthSpaceText( strSelectedText, false );

							pFont->Draw( "|", rRectT, BuildFormat() | DT_NOCLIP, DX::Color( dwColorText ) );
						}
					}

					if ( strTextRender.length() > 0 )
					{
						//Render Selected Text
						if ( iSelectedTextLength > 0 )
						{
							int iWidthText = pFont->Calc( strTextRender.substr( iSelectedTextPos, iSelectedTextLength ) ) + GetWidthSpaceText( strTextRender.substr( iSelectedTextPos, iSelectedTextLength ), false );

							GRAPHICENGINE->DrawRectangle2D( Rectangle2D( iRenderX + sSelectedTextBox.iX, iRenderY + sSelectedTextBox.iY, iWidthText, pFont->GetDesc().Height ), D3DCOLOR_ARGB( 100, 255, 255, 255 ) );
						}
						else if ( iSelectedTextLength < 0 )
						{
							int iWidthText = pFont->Calc( strTextRender.substr( iSelectedTextPos - abs( iSelectedTextLength ), iSelectedTextPos + 1 ) ) + GetWidthSpaceText( strTextRender.substr( iSelectedTextPos - abs( iSelectedTextLength ), iSelectedTextPos + 1 ), false );

							GRAPHICENGINE->DrawRectangle2D( Rectangle2D( iRenderX + sSelectedTextBox.iX - iWidthText + sSelectedTextBox.iWidth, iRenderY + sSelectedTextBox.iY, iWidthText, pFont->GetDesc().Height ), D3DCOLOR_ARGB( 100, 255, 255, 255 ) );
						}
					}
				}
			}	
		}

		GetGraphicsDevice()->PopScissorRect();

		Element::Render( iX, iY, iWidth, iHeight, iSourceX, iSourceY );
	}
}

void InputField::OnMouseMove( CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	//Text selected?
	if( bSelectingText && strText.length() > 0 )
	{
        std::string strTextRender = "";
        if ( bMaskPassword )
        {
            for ( size_t i = 0; i < strText.length(); i++ )
                strTextRender.append( szMask );
        }
        else
            strTextRender = strText;

		//Mouse Position
		Point2D * psPosition = pcMouse->GetPosition();

		int iWidthSpace = pFont->Calc( "i i", BuildFormat() | DT_NOCLIP ) - (pFont->Calc( "i", BuildFormat() | DT_NOCLIP ) * 2);

		int iDifference = psPosition->iX - (sSelectedTextBox.iX + GetX() + iX);
		int iCurrentWidth = 0;

		if( iDifference > 0 )
		{
			for( size_t i = iSelectedTextPos; i < strTextRender.length(); i++ )
			{
				char c[2] = {strTextRender[i], 0 };
				iCurrentWidth += isspace( strTextRender[i] ) ? iWidthSpace : pFont->Calc( c );

				if( iCurrentWidth > iDifference )
				{
					iSelectedTextLength = i - iSelectedTextPos + 1;
					break;
				}
			}
		}
		else if( iDifference <= 0 )
		{
			if ( iSelectedTextPos > 0 )
			{
				for ( int i = iSelectedTextPos - 1; i >= 0; i-- )
				{
					char c[2] = {strTextRender[i], 0};
					iCurrentWidth += isspace( strTextRender[i] ) ? iWidthSpace : pFont->Calc( c );

					if ( iCurrentWidth > abs( iDifference ) )
					{
						iSelectedTextLength = i - iSelectedTextPos;
						break;
					}
				}
			}
		}
	}
}

BOOL InputField::OnMouseClick( CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	if ( !IsOpen() || bDisable )
		return FALSE;

	if( ClickDownL != pcMouse->GetEvent() )
	{
		bSelectingText = FALSE;
		return FALSE;
	}

	//Mouse Position
	Point2D * psPosition = pcMouse->GetPosition();

	int iRenderX		= GetX() + iX + iSourceX;
	int iRenderY		= GetY() + iY + iSourceY;
	int iRenderWidth	= bParentChild ? low( GetWidth(), iWidth	- GetX() ) : GetWidth();
	int iRenderHeight	= bParentChild ? low( GetHeight(), iHeight - GetY() ) : GetHeight();

	Rectangle2D rInput = { iRenderX, iRenderY, iRenderWidth, iRenderHeight };
	sSelectedTextBox = { 0, 0, 0, 0 };
	iSelectedTextPos = 0;
	iSelectedTextLength = 0;
	bSelectingText = FALSE;

	int iWidthSpace = pFont->Calc( "i i", BuildFormat() | DT_NOCLIP ) - (pFont->Calc( "i", BuildFormat() | DT_NOCLIP ) * 2);

	//Inside Input?
	if ( rInput.Inside( psPosition ) )
	{
		if ( bSelectType )
		{
			if ( pGroupMain )
			{
				if ( !bSelect )
					pGroupMain->SetIndexObject( GetID() );
				else
				{
                    std::string strTextRender = "";
                    if ( bMaskPassword )
                    {
                        for ( size_t i = 0; i < strText.length(); i++ )
                            strTextRender.append( szMask );
                    }
                    else
                        strTextRender = strText;

					uCurrPosition = strTextRender.length();

					Rectangle2D sTextBox = { iRenderX, iRenderY, iRenderX + GetWidth(), iRenderY + GetHeight() };
					int iCurrentWidthAdd = 0;

					//Loop Text
					for ( size_t i = 0; i < strTextRender.length(); i++ )
					{
						char c[2] = {strTextRender[i], 0 };
						int iCharacterWidth = isspace( strTextRender[i] ) ? iWidthSpace : pFont->Calc( c );
						int iCharacterHeight = pFont->CalcHeight( c );

						Rectangle2D sDetectBox = { sTextBox.iX + iCurrentWidthAdd + 2, sTextBox.iY + 2, iCharacterWidth, iCharacterHeight };
						sSelectedTextBox = { iCurrentWidthAdd + 2, 2, iCharacterWidth, iCharacterHeight };

						//Mouse inside letter bounding box?
						if ( sDetectBox.Inside( psPosition ) )
						{
							bSelectingText = TRUE;
							uCurrPosition = i;
							iSelectedTextPos = i;
							break;
						}

						bSelectingText = TRUE;
						uCurrPosition = strTextRender.length();
						iSelectedTextPos = strTextRender.length();

						iCurrentWidthAdd += iCharacterWidth;
					}
				}

				bSelect = TRUE;
			}
			else
				bSelect = bSelect ? FALSE : TRUE;
		}
		return TRUE;
	}
	else
		bSelect = FALSE;

	return FALSE;
}

BOOL InputField::OnMouseScroll( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	if ( !IsOpen() )
		return FALSE;

	Point2D * psPosition = pcMouse->GetPosition();

	if ( bSelect )
	{
		if ( bIsNumber && GetCanScroll() )
		{
			int iNum = atoi( GetText().c_str() ) + pcMouse->GetDistance();
			if ( iNum > iMaxNum )
				iNum = iMaxNum;

			if ( iNum < iMinNum )
				iNum = iMinNum;

			strText = FormatString( "%d", iNum );
			uCurrPosition = strText.length();

			if ( bIsFormattedNumber )
			{
				UINT uOld = uCurrPosition;
				strText = FormatNumber( _atoi64( GetText().c_str() ) );
				uCurrPosition += strText.length() - uOld;
			}
			return TRUE;
		}
	}
	return FALSE;
}

BOOL InputField::OnKeyPress( class CKeyboard * pcKeyboard )
{
	if( bSelect )
	{
		if( pcKeyboard->GetEvent() == KeyDown )
		{
			for( size_t i = 0; i < vBindActionsKeyboard.size(); i++ )
			{
				if( pcKeyboard->GetKey() == vBindActionsKeyboard[i].dwKey )
				{
					UI::Event::Invoke( vBindActionsKeyboard[i].iEventID );
					return TRUE;
				}
			}
		}

		if( pcKeyboard->GetKey() == VK_RETURN && pcKeyboard->GetEvent() == KeyDown )
		{
			iSelectedTextLength = 0;
			bSelect = FALSE;
			return TRUE;
		}

		//Have a text?
		if( strText.length() > 0 )
		{
			if( pcKeyboard->GetKey() == VK_RIGHT && pcKeyboard->GetEvent() == KeyDown )
			{
				if( (UINT)uCurrPosition < strText.length() )
					uCurrPosition++;
			}
			else if( pcKeyboard->GetKey() == VK_LEFT && pcKeyboard->GetEvent() == KeyDown )
			{
				if( uCurrPosition > 0 )
					uCurrPosition--;
			}
			else if( (pcKeyboard->GetKey() == VK_HOME || pcKeyboard->GetKey() == VK_PRIOR) && pcKeyboard->GetEvent() == KeyDown )
			{
				uCurrPosition = 0;
				return TRUE;
			}
			else if( (pcKeyboard->GetKey() == VK_END || pcKeyboard->GetKey() == VK_NEXT) && pcKeyboard->GetEvent() == KeyDown )
			{
				uCurrPosition = strText.length();
				return TRUE;
			}
			else if( pcKeyboard->GetKey() == VK_DELETE && pcKeyboard->GetEvent() == KeyDown )
			{
				if( iSelectedTextLength > 0 && strText.length() > 0 && (UINT)iSelectedTextLength <= strText.length() )
				{
					strText.erase( iSelectedTextPos, iSelectedTextLength );
					iSelectedTextPos = 0;
					iSelectedTextLength = 0;
				}
				else if( (UINT)abs( iSelectedTextLength ) <= strText.length() && iSelectedTextLength < 0 && strText.length() > 0 )
				{
					strText.erase( iSelectedTextPos - abs( iSelectedTextLength ), abs( iSelectedTextLength ) );
					uCurrPosition -= abs( iSelectedTextLength );
					iSelectedTextPos = 0;
					iSelectedTextLength = 0;
				}
				else if( strText.length() > 0 && uCurrPosition > 0 && (UINT)uCurrPosition <= strText.length() )
				{
					strText.erase( uCurrPosition, 1 );
				}

				if ( strText.length() > uCurrPosition )
					uCurrPosition = strText.length() - 1;

				return TRUE;
			}
		}

		return TRUE;
	}

	return FALSE;
}

BOOL InputField::OnKeyChar( class CKeyboard * pcKeyboard )
{
	if ( bSelect )
	{		
		if( pcKeyboard->GetChar() == VK_BACK )
		{
			if( iSelectedTextLength > 0 && strText.length() > 0 && (UINT)iSelectedTextLength <= strText.length() )
			{
				strText.erase( iSelectedTextPos, iSelectedTextLength );
				iSelectedTextPos = 0;
				iSelectedTextLength = 0;
			}
			else if( (UINT)abs( iSelectedTextLength ) <= strText.length() && iSelectedTextLength < 0 && strText.length() > 0 )
			{
				strText.erase( iSelectedTextPos - abs( iSelectedTextLength ), abs( iSelectedTextLength ) );
				uCurrPosition -= abs( iSelectedTextLength );
				iSelectedTextPos = 0;
				iSelectedTextLength = 0;
			}
			else if( strText.length() > 0 && uCurrPosition > 0 && (UINT)uCurrPosition <= strText.length() )
			{
				strText.erase( uCurrPosition - 1, 1 );
				uCurrPosition--;
			}

			if ( bIsNumber )
			{
/*				if ( strText.length() == 0 )
					strText = FormatString( "%d", iMinNum );
				else if ( atoi( GetText().c_str() ) < iMinNum )
					strText = FormatString( "%d", iMinNum );
*/
				if ( bIsFormattedNumber )
				{
					UINT uOld = uCurrPosition;
					strText = FormatNumber( _atoi64( GetText().c_str() ) );
					uCurrPosition += strText.length() - uOld;
				}
			}

			return TRUE;
		}
		else if( pcKeyboard->GetChar() == VK_TAB )
		{
			return TRUE;
		}
		else if( pcKeyboard->IsKeyDown( VK_CONTROL ) && pcKeyboard->IsKeyDown( 'V' ) )
		{
			AddInput( GetClipboardText() );
			return TRUE;
		}
		else if( pcKeyboard->IsKeyDown( VK_CONTROL ) && pcKeyboard->IsKeyDown( 'C' ) )
		{
			if( iSelectedTextLength > 0 && strText.length() > 0 && (UINT)iSelectedTextLength <= strText.length() )
			{
				SetClipboardText( (char*)strText.substr( iSelectedTextPos, iSelectedTextLength ).c_str() );
				iSelectedTextPos = 0;
				iSelectedTextLength = 0;
			}
			else if( (UINT)abs( iSelectedTextLength ) <= strText.length() && iSelectedTextLength < 0 && strText.length() > 0 )
			{
				SetClipboardText( (char*)strText.substr( iSelectedTextPos - abs( iSelectedTextLength ), abs( iSelectedTextLength ) ).c_str() );
				uCurrPosition -= abs( iSelectedTextLength );
				iSelectedTextPos = 0;
				iSelectedTextLength = 0;
			}

			return TRUE;
		}
		else if( pcKeyboard->IsKeyDown( VK_SHIFT ) && pcKeyboard->IsKeyDown( VK_ESCAPE ) )
			return TRUE;
		else if( pcKeyboard->IsKeyDown( VK_CONTROL ) && pcKeyboard->IsKeyDown( 'Z' ) )
		{
			UndoAction();
			return TRUE;
		}
		else if( pcKeyboard->IsKeyDown( VK_CONTROL ) && pcKeyboard->IsKeyDown( 'A' ) )
		{
			char c[2] = { strText[0], 0 };

			int iCharacterWidth = pFont->Calc( c );
			int iCharacterHeight = pFont->CalcHeight( c );

			sSelectedTextBox = { 2, 2, iCharacterWidth, iCharacterHeight };

			iSelectedTextPos = 0;
			iSelectedTextLength = strText.length();
			uCurrPosition = 0;
			return TRUE;
		}
		else if( pcKeyboard->GetChar() == VK_SPACE )
		{
			AddInput( " " );
			return TRUE;
		}
		else if( strText.length() < ( UINT )iTextLength )
		{
			//if ( pcKeyboard->IsKeyDown( VK_CONTROL ) == FALSE )
			{
				std::stringstream ss;
				std::string s;
				auto c = pcKeyboard->GetChar();

				if ( c < 256 )
					ss << (char)c;
				else
					ss << c;
				ss >> s;



				int iNumOld = atoi( strText.c_str() );

				AddInput( s );

				int iNumNew = atoi( strText.c_str() );

				if ( bIsNumber )
				{
					if ( bIsNumber && strText.length() > 1 && atoi( strText.c_str() ) == 0 )
						strText = "";

					if ( iNumOld == 0 && iNumNew < 10 )
						strText = s;

					if ( atoi( GetText().c_str() ) > iMaxNum )
					{
						strText = FormatString( "%d", iMaxNum );

						if ( bIsFormattedNumber )
						{
							UINT uOld = uCurrPosition;
							strText = FormatNumber( _atoi64( GetText().c_str() ) );
							uCurrPosition += strText.length() - uOld;
						}
					}
				}
			}
		}
		return TRUE;
	}
	return FALSE;
}

void InputField::ClearInput()
{
	iSelectedTextLength = 0;
	uCurrPosition = 0;
	strText.clear();
}

void InputField::SetFont( const char * pszFont, int iHeight, int iWidth, BOOL bItalic, BOOL bBold, DWORD dwColor )
{
	pFont = GetGraphicsDevice()->GetFontFactory()->MakeFont( nullptr, pszFont, iHeight, iWidth, bBold, bItalic );

	iFontHeight		= iHeight;
	dwColorText		= dwColor;
}

void InputField::UndoAction()
{
	std::string strLastAction = vInputActions[vInputActions.size() - 1];
	std::size_t uPosition = strText.rfind( strLastAction );

	if( uPosition != std::string::npos && strLastAction.length() > 0 )
	{
		uCurrPosition -= strLastAction.length();
		strText.erase( uPosition, strLastAction.length() );
		vInputActions.pop_back();
	}
}

void InputField::AddInput( std::string str )
{
	if ( strText.length() >= (UINT)iTextLength )
		return;

	str.erase( std::remove( str.begin(), str.end(), '\n' ), str.end() );
	str.erase( std::remove( str.begin(), str.end(), '\r' ), str.end() );

	if ( bIsNumber )
	{
		for ( UINT i = 0; i < str.length(); i++ )
		{
			char c = str.c_str()[i];

			if ( bIsFormattedNumber && (c == ',') )
				continue;

			if ( c < 0x30 || c > 0x39 )
				return;
		}
	}

	if ( bAllowedRule )
	{
		BOOL bFound = FALSE;

		for ( UINT i = 0; i < str.length(); i++ )
		{
			char c = str.c_str()[i];

			if ( bAllowedRule )
			{
				bFound = FALSE;

				for ( auto v : strAllowed )
				{
					if ( v == c )
					{
						bFound = TRUE;
						break;
					}
				}

				if ( bFound == FALSE )
					return;
			}
		}
	}

	std::string strNew = "";

	if ( strText.length() + str.length() > (UINT)iTextLength )
		strNew = str.substr( 0, (strText.length() + str.length()) - iTextLength );
	else
		strNew = str;

	if ( (UINT)uCurrPosition <= strText.length() )
	{
		strText.insert( uCurrPosition, strNew );
		uCurrPosition += strNew.length();

		vInputActions.push_back( strNew );
	}

	if ( bIsFormattedNumber )
	{
		UINT uOld = uCurrPosition;
		strText = FormatNumber( _atoi64( GetText().c_str() ) );
		uCurrPosition += strText.length() - uOld;
	}
}
}