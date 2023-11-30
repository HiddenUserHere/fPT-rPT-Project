#pragma once
#include "UIElement.h"

namespace UI
{
typedef std::shared_ptr<class ImageBox> ImageBox_ptr;
class ImageBox : public Element
{
public:
	//Constructor
											ImageBox( class Rectangle2D rRectangle );
	virtual								   ~ImageBox();

	void									Clear();

	//Setters and Getters
	void									SetImage( class Image * pImage );
	void									SetActiveImage( std::pair<class Image *, class Image *> pPair );
	void									SetGroup( std::shared_ptr<class Group> pGroupMain );
	void									SetColor( DWORD _dwColor ){ dwColor = _dwColor; }
	void									Swap();
	void									Swap( BOOL bActiveImage );
	void									SetSourceBox( Rectangle2D sBox ){ sSourceBox = sBox; }
	void									SetTexture( LPDIRECT3DTEXTURE9 ps ){ psTexture = ps; }

	Image								  * GetImage() { return pImage; }
	Image								  * GetImageActive1() { return pImage; }
	Image								  * GetImageActive2() { return pImage; }

	//Render
	void									Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );
private:
	class Image								* pImage		= NULL;
	class Image								* pImageActive1	= NULL;
	class Image								* pImageActive2	= NULL;

	std::shared_ptr<class Group>			pGroup			= NULL;

	DWORD									dwColor = -1;

	LPDIRECT3DTEXTURE9						psTexture = NULL;

	Rectangle2D								sSourceBox;
};
}