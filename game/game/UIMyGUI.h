#pragma once
namespace UI
{

class UIMyGUI : public Element
{
public:
    UIMyGUI();
    virtual ~UIMyGUI();

    static void                 Init();

    void						OnMouseMove( class CMouse * pcMouse );
    BOOL						OnMouseClick( class CMouse * pcMouse );
    BOOL						OnMouseScroll( class CMouse * pcMouse );

    BOOL						OnKeyPress( class CKeyboard * pcKeyboard );
    BOOL						OnKeyChar( class CKeyboard * pcKeyboard );

private:
    static MyGUI::DirectXPlatform       * pcPlatform;
    static MyGUI::Gui                   * pcGUI;

};

};