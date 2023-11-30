#pragma once
#include "UIElement.h"
namespace UI
{
typedef std::shared_ptr<class RectangleBox> RectangleBox_ptr;
class RectangleBox : public Element
{
public:
	RectangleBox( Rectangle2D rRectangle );
	~RectangleBox();

	void						Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );

	void						SetColor( DX::Color c1, DX::Color c2 );

private:
	class Image					* pImageBorderLeftTop;
	class Image					* pImageBorderRightTop;
	class Image					* pImageBorderTop;
	class Image					* pImageBorderLeftBottom;
	class Image					* pImageBorderRightBottom;
	class Image					* pImageBorderBottom;
	class Image					* pImageBorderLeft;
	class Image					* pImageBorderRight;

	//Images Box
	class Image					* pImageMaskLeftTop;
	class Image					* pImageMaskRightTop;
	class Image					* pImageMaskLeftBottom;
	class Image					* pImageMaskRightBottom;

	DX::RenderTarget_ptr		pRenderTargetMask;
	DX::Effect_ptr				pEffectMask;

	DX::Color					cColor1;
	DX::Color					cColor2;

	void						DrawInfoBoxMask( int iX, int iY, int iWidth, int iHeight );

};
};