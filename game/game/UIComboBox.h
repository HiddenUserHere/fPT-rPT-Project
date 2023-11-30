#pragma once

namespace UI
{
typedef std::shared_ptr<class ComboBox> ComboBox_ptr;
class ComboBox : public Element
{
public:
	//Constructor
												ComboBox( Rectangle2D rRect );
											   ~ComboBox();

	Text_ptr									MakeOption( const std::string & strText, const Rectangle2D & rBox, BOOL bBold, const DWORD dwColor );
	void										AddOption( Text_ptr p );
	void										AddOption( Text_ptr p, int iID );

	//Render
	void										Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );

	//Getters and Setters
	void										SetHoverColor(DWORD dwColor) { dwHoverColor = dwColor; }
	void										SetBackgroundImage( std::pair<class Image *, class Image *> pPair );
	void										SetBackgroundSelectedImage( class Image * pImage ) { pBackgroundActiveTopImage = pImage; }
	void										SetBackgroundActiveImage( std::pair<class Image *, class Image *> pPair );
	void										SetChangeEvent( int iEvent ) { iEventID = iEvent; };
	void										SetDefaultText( std::string strText ){ pTextValue->SetText( strText ); }
	int											GetSelectedID() { return iSelectedID; }

	std::string									GetTextOption( int iIDObject )
	{
		// Find object and return pointer to this
		for ( UINT u = 0; u < vTextOptions.size(); u++ )
		{
			if ( vTextOptions[u]->GetID() == iIDObject )
			{
				return vTextOptions[ u ]->GetText();
			}
		}
		return "";
	};

	//Events Handlers
	BOOL										OnMouseClick( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );
	void										OnMouseMove( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );

	void										Clear();
private:
	BOOL										bShowMenu = FALSE;
	BOOL										bHover = FALSE;
	std::vector<Text_ptr>						vTextOptions;
	
	Image									  * pBackgroundImage = NULL;
	Image									  * pBackgroundActiveImage = NULL;
	Image									  * pBackgroundActiveTopImage = NULL;
	Image									  * pBackgroundActiveMiddleImage = NULL;
	Image									  * pBackgroundActiveBottomImage = NULL;
	
	Text_ptr									pTextValue;

	int											iEventID = -1;
	int											iHeightMenu = 0;
	int											iSelectedID = -1;

	DWORD										dwHoverColor = -1;
	Rectangle2D									rOptionPosition;
};
}