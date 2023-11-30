#include "stdafx.h"
#include "UIMyGUI.h"

namespace UI
{

MyGUI::DirectXPlatform * UIMyGUI::pcPlatform = nullptr;
MyGUI::Gui * UIMyGUI::pcGUI = nullptr;


UIMyGUI::UIMyGUI()
{
}

UIMyGUI::~UIMyGUI()
{
    pcGUI->shutdown();
    DELET( pcGUI );

    pcPlatform->shutdown();
    DELET( pcPlatform );
}

void UIMyGUI::Init()
{
    
    pcPlatform = new MyGUI::DirectXPlatform();
    pcPlatform->initialise( GetGraphicsDevice()->Get() );
    pcGUI = new MyGUI::Gui();
    pcGUI->initialise();
    
}

void UIMyGUI::OnMouseMove( CMouse * pcMouse )
{
    
}

BOOL UIMyGUI::OnMouseClick( CMouse * pcMouse )
{

    return 0;
}

BOOL UIMyGUI::OnMouseScroll( CMouse * pcMouse )
{
    return 0;
}

BOOL UIMyGUI::OnKeyPress( CKeyboard * pcKeyboard )
{
    return 0;
}

BOOL UIMyGUI::OnKeyChar( CKeyboard * pcKeyboard )
{
    MyGUI::InputManager::getInstance().injectMouseMove( 0, 0, 0 );

    return 0;
}

};