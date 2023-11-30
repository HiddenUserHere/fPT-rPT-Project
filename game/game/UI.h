#pragma once

#include "UIColor.h"
#include "UIWindow.h"
#include "UIImageLoader.h"
#include "UIImageRender.h"
#include "UIGroup.h"
#include "UIEvent.h"
#include "UIElement.h"
#include "UIScroll.h"
#include "UIText.h"
#include "UIDropdownMenu.h"
#include "UIButton.h"
#include "UIImageBox.h"
#include "UICheckBox.h"
#include "UIInputField.h"
#include "UITextWindow.h"
#include "UIList.h"
#include "UIImage.h"
#include "UIItemInfoBox.h"
#include "UIComboBox.h"
#include "UICounter.h"
#include "UIBitmapFont.h"
#include "UIItemBox.h"
#include "UIAnimation.h"
#include "UIMessageBallon.h"
#include "UISkillInfoBox.h"
#include "UIUnitMesh.h"
#include "UITooltip.h"
#include "UIChargeBar.h"
#include "UIRectangleBox.h"
#include "UIWebViewer.h"
#include "UINoesis.h"

extern BOOL bUIDebug;


#define UIWINDOW_DEFAULT_HEADER_CREATOR \
void					OnMouseMove( class CMouse * pcMouse );         \
BOOL					OnMouseClick( class CMouse * pcMouse );         \
BOOL					OnMouseScroll( class CMouse * pcMouse );        \
                                                                        \
BOOL					OnKeyPress( class CKeyboard * pcKeyboard );     \
BOOL					OnKeyChar( class CKeyboard * pcKeyboard );      \
                                                                        \
void					Update( float fTime );                          \
void					Render();                                       

#define UIWINDOW_DEFAULT_SOURCE_CREATOR(classname, ptr) 										\
void classname::OnMouseMove( class CMouse * pcMouse )                                           \
{                                                                                               \
	ptr->OnMouseMove( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );               \
}                                                                                               \
																								\
BOOL classname::OnMouseClick( class CMouse * pcMouse )                                          \
{                                                                                               \
	if ( ptr->OnMouseClick( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 ) )        \
		return TRUE;                                                                            \
																								\
	return FALSE;                                                                               \
}                                                                                               \
																								\
BOOL classname::OnMouseScroll( CMouse * pcMouse )                                               \
{                                                                                               \
	return ptr->OnMouseScroll( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );      \
}                                                                                               \
																								\
BOOL classname::OnKeyPress( class CKeyboard * pcKeyboard )                                      \
{                                                                                               \
	if ( ptr->OnKeyPress( pcKeyboard ) )                                                        \
		return TRUE;                                                                            \
																								\
	return FALSE;                                                                               \
}                                                                                               \
																								\
BOOL classname::OnKeyChar( class CKeyboard * pcKeyboard )                                       \
{                                                                                               \
	if ( ptr->OnKeyChar( pcKeyboard ) )                                                         \
		return TRUE;                                                                            \
																								\
	return FALSE;                                                                               \
}                                                                                               \
																								\
void classname::Update( float fTime )                                                           \
{                                                                                               \
	ptr->Update( fTime );                                                                       \
}                                                                                               \
																								\
void classname::Render()                                                                        \
{                                                                                               \
	ptr->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );                             \
}
